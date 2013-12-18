/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***************************************************************************
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Justin Huang                                                 *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmCStatus.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/Nsm/nsmVrfIntf.h"

#include "Vrf/Local/VrfUtils.h"
#include "Vrf/Local/VrfLocalManagedObject.h"
#include "ClientInterface/Nsm/NsmClientCreateIntIpConfigSPMessage.h"
#include "ClientInterface/Nsm/NsmClientConfigVRFMessage.h"
#include "Nsm/Local/Layer3/NsmIntfLoopbackManagedObject.h"
#include "Nsm/Local/Layer3/NsmLocalIntfLoopbackMessage.h"
#include "Nsm/Local/Layer3/NsmLocalIntfLoopbackWorker.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"

namespace DcmNs
{

    NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker (NsmLocalObjectManager *pWaveObjectManager)
        : WaveWorker (pWaveObjectManager)
    {
        NsmIntfLoopbackManagedObject NsmIntfLoopbackManagedObject (pWaveObjectManager);
        NsmIntfLoopbackManagedObject.setupOrm ();
        addManagedClass (NsmIntfLoopbackManagedObject::getClassName (), this);

        addOperationMap (NSMLOCALINTFLOOPBACKMSG, 
            reinterpret_cast<PrismMessageHandler> (&NsmLocalIntfLoopbackWorker::
            NsmLocalIntfLoopbackMessageHandler));
    }

    NsmLocalIntfLoopbackWorker::~NsmLocalIntfLoopbackWorker ()
    {
    }

    PrismMessage  *NsmLocalIntfLoopbackWorker::createMessageInstance(
        const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMLOCALINTFLOOPBACKMSG: 
                pPrismMessage = new NsmLocalIntfLoopbackMessage();
                break;

            default :
                trace (TRACE_LEVEL_FATAL, 
                    "NsmLocalIntfLoopbackWorker::createMessageInstance : "
                    "Unknown OperationCode : " + operationCode);
                break;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *NsmLocalIntfLoopbackWorker::createManagedObjectInstance(
        const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((NsmIntfLoopbackManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NsmIntfLoopbackManagedObject(
                dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "NsmLocalIntfLoopbackWorker::"
                "createManagedObjectInstance : Unknown Managed Class Name : " + 
                managedClassName);
        }
        return (pWaveManagedObject);
    }

    ResourceId NsmLocalIntfLoopbackWorker::validateIntfLoopbackStep (
        PrismSynchronousLinearSequencerContext *pContext)
    {
        int status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmLocalIntfLoopbackWorker::validIntfLoopbackStep() ";
        NSM_PLUG_INFO_S(func_name);
        NsmLocalIntfLoopbackMessage *pMessage = 
            dynamic_cast<NsmLocalIntfLoopbackMessage *> 
            (pContext->getPPrismMessage ());
        prismAssert (NULL != pMessage, __FILE__, __LINE__);

        SI32 mapId = DcmToolKit::getLocalMappedId();
        UI32 ifId = pMessage->getIfId();
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();
        UI32 opCode = pMessage->getOpCode();

        NSM_PLUG_INFO_S(func_name + 
           " ifId: " + ifId + 
           " ifName: " + ifName + 
           " comCode: "+ cmdCode +
           " opCode: "+ opCode +
           " status: " + pMessage->getStatus() +
           " ipAddr: " + pMessage->getIpv4Addr() +
           "");
        
        NsmIntfLoopbackManagedObject *pMo = findIntfLoopbackMO(ifId, mapId, pContext); 
        if (pMo) pContext->addManagedObjectForGarbageCollection(pMo);

        switch (pMessage->getCmdCode())
        {
            case NSM_MSG_CMD_INTF_LOOPBACK:
            {
                if (opCode == NSM_MSG_OP_CREATE) {
                    if (pMo != NULL) {
                        NSM_PLUG_INFO_S(func_name + "ERROR: Loopback port: " + 
                            ifId + " had already been created!");
                        return WRC_NSM_API_ERR_GENERIC;
                    }
                }
                else {
                    if (pMo == NULL) {
                        NSM_PLUG_INFO_S(func_name + "ERROR: Loopback port: " + 
                            ifId + " doesn't exist!");
                        return WRC_NSM_INTERFACE_DOES_NOT_EXIST;
                    }
                }
                break;
            }
            default:
                break;
        }
                                        
        return status;
    }

    ResourceId NsmLocalIntfLoopbackWorker::doIntfLoopbackStep (
        PrismSynchronousLinearSequencerContext *pContext)
    {
        int status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmLocalIntfLoopbackWorker::doIntfLoopbackStep() ";

        NsmLocalIntfLoopbackMessage *pMessage =
            dynamic_cast<NsmLocalIntfLoopbackMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();
        UI32 opCode = pMessage->getOpCode();
        UI32 ifType = IF_TYPE_INTF_LOOPBACK;

        trace (TRACE_LEVEL_INFO, func_name + "Entering ...");
        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
            " IfId: " + pMessage->getIfId() +
            " IfType: " + ifType +
            " OP-CODE: " + pMessage->getOpCode() +
            " CMD-CODE: " + pMessage->getCmdCode() +
            " Status: " + pMessage->getStatus() +
            " IP-ADDR: " + pMessage->getIpv4Addr());

//// Send to backend
        NsmClientCreateIntIpConfigSPMessage *m = new NsmClientCreateIntIpConfigSPMessage();
    
        UI32 opcode = NsmUtils::getClientOpCode(pMessage->getCmdCode(),
            pMessage->getOpCode());
        m->setOpcode(opcode);
        m->setIfId(pMessage->getIfId());
        m->setIfName(ifName);
        m->setIntfType(ifType);
        m->setSec(false);

        m->setIp_addr_mask(pMessage->getIpv4Addr());
        m->setStatus(pMessage->getStatus());

        trace (TRACE_LEVEL_INFO, func_name +  "Message to BACKEND" +
            " IntfId: " + m->getIfId() +
            " IntfName: " + m->getIfName() +
            " OP-CODE: " + m->getOpcode() +
            " Status: " + m->getStatus() +
            " IP-ADDR: " + m->getIp_addr_mask() +
            " MTU: " + m->getMtu() +
            " ProxyArp: " + m->getProxy_arp() +
            " ARP-AGING-TIMEOUT: " + m->getArp_ageing_timeout() +
            "");

        /* Send the message to back end daemon */
        if (cmdCode == NSM_MSG_CMD_PROXY_ARP 
            || cmdCode == NSM_MSG_CMD_ARP_AGING_TIMEOUT
            )
        {
            status = sendSynchronouslyToWaveClient ("arp", m);
            if (status == WAVE_MESSAGE_SUCCESS) {
                status = m->getCompletionStatus();
                if (status != WAVE_MESSAGE_SUCCESS) {
                    trace (TRACE_LEVEL_INFO, func_name + 
                        " Intf: " + ifName + " IfType: " + ifType +
                        " CmdCode=" + cmdCode + " OpCode=" + opCode +
                        " (ARP) getCompletionStatus() FAILED" + 
                        " status = " + status);
                }
                else if (m->getClientStatus()) {
                    int err = m->getClientStatus();

                    status = NsmUtils::ArpBackendErrorMapToDcmStatus(err);
                    trace (TRACE_LEVEL_INFO, func_name + 
                        " Intf: " + ifName + " IfType: " + ifType +
                        " CmdCode=" + cmdCode + " OpCode=" + opCode +
                        " (ARP) getClientStatus() FAILED" +
                        " Arp Error = " + err + " Dcmd Status = " + status);
                }
            }
            pMessage->setCompletionStatus(status);
            trace(TRACE_LEVEL_INFO, func_name + 
                " Intf: " + ifName + " IfType: " + ifType +
                " CmdCode=" + cmdCode + " OpCode=" + opCode +
                " - Message has been sent to ARP client. status=" + status);
        }
        else {
            status = sendSynchronouslyToWaveClient ("nsm", m);
            if (status == WAVE_MESSAGE_SUCCESS) {
                status = m->getCompletionStatus();
                if (status != WAVE_MESSAGE_SUCCESS) {
                    trace (TRACE_LEVEL_INFO, func_name + 
                        " Intf: " + ifName + " IfType: " + ifType +
                        " CmdCode=" + cmdCode + " OpCode=" + opCode +
                        " (NSM) getCompletionStatus() FAILED" + 
                        " status = " + status);
                }
                else if (m->getClientStatus()) {
                    int err = m->getClientStatus();

                    status = NsmUtils::NsmBackendErrorMapToDcmStatus(err);
                    trace (TRACE_LEVEL_INFO, func_name + 
                        " Intf: " + ifName + " IfType: " + ifType +
                        " CmdCode=" + cmdCode + " OpCode=" + opCode +
                        " (NSM) getClientStatus() FAILED" +
                        " Nsm Error = " + err + " Dcmd Status = " + status);
                }
            }
            pMessage->setCompletionStatus(status);
            trace(TRACE_LEVEL_INFO, func_name + 
                " Intf: " + ifName + " IfType: " + ifType +
                " CmdCode=" + cmdCode + " OpCode=" + opCode +
                " - Message has been sent to NSM client. status=" + status);
        }
        delete m;

        return  status;

    }

    ResourceId NsmLocalIntfLoopbackWorker::doIntfLoopbackMOStep ( 
        PrismSynchronousLinearSequencerContext *pContext)
    {
        string func_name = "NsmLocalIntfLoopbackWorker::doIntfLoopbackMOStep() ";
        SI32 mapId = DcmToolKit::getLocalMappedId();
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        NsmLocalIntfLoopbackMessage *pMessage = 
            dynamic_cast<NsmLocalIntfLoopbackMessage *> 
            (pContext->getPPrismMessage ());
        prismAssert (NULL != pMessage, __FILE__, __LINE__);

        UI32 ifId = pMessage->getIfId();
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();
        UI32 opCode = pMessage->getOpCode();

        NSM_PLUG_INFO_S(func_name + 
           " ifId: " + ifId + 
           " ifName: " + ifName + 
           " comCode: "+ cmdCode +
           " opCode: "+ opCode +
           " status: " + pMessage->getStatus() +
           " ipAddr: " + pMessage->getIpv4Addr() +
           "");

        NsmIntfLoopbackManagedObject *pMo = findIntfLoopbackMO(ifId, mapId, pContext); 

        switch (cmdCode) {
            case NSM_MSG_CMD_INTF_LOOPBACK:
                if (opCode != NSM_MSG_OP_DELETE) {
                    NSM_PLUG_INFO_S(func_name + " NOT-DELETE ID");
                    prismAssert (NULL == pMo, __FILE__, __LINE__);

                    pMo= new NsmIntfLoopbackManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                    pMo->setIfId(ifId);
                    pMo->setIfName(ifName);
                    NsmL3ReadyManagedObject *pL3ReadyMO = new NsmL3ReadyManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                    pL3ReadyMO->setInterfaceName(ifName);
                    pL3ReadyMO->setIntfType(IF_TYPE_INTF_LOOPBACK);
                    pMo->setL3Ready(const_cast<NsmL3ReadyManagedObject *>(pL3ReadyMO));
                    pMo->setStatus(true);
                    updateWaveManagedObject(pMo);
                }
                else {
                    NSM_PLUG_INFO_S(func_name + " DELETE ID");
                    prismAssert (NULL != pMo, __FILE__, __LINE__);

                    delete pMo;
                    pMo = NULL;
                }
                break;

            case NSM_MSG_CMD_INTF_SHUTDOWN:
                prismAssert (NULL != pMo, __FILE__, __LINE__);
                if (opCode != NSM_MSG_OP_DELETE) {
                    NSM_PLUG_INFO_S(func_name + " NOT-DELETE STATUS");
                }
                else {
                    NSM_PLUG_INFO_S(func_name + " DELETE STATUS");
                }
                pMo->setStatus(pMessage->getStatus());
                updateWaveManagedObject(pMo);

                break;
            default:
                NSM_PLUG_INFO_S(func_name + " Unsupported comCode: " + cmdCode);
                break;
         }
  
        if (pMo) pContext->addManagedObjectForGarbageCollection(pMo);

        return status;
    }

    void  NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackMessageHandler (
        NsmLocalIntfLoopbackMessage *pMessage)
    {

        if (pMessage->getCmdCode() == NSM_MSG_VRF_BIND_INTF ||
            pMessage->getCmdCode() == NSM_MSG_VRF_UNBIND_INTF) {
            LoopBackIntfVrfConfigHandler(pMessage);
        } else {
            LoopBackIntfConfigHandler(pMessage);
        }
    }

    void NsmLocalIntfLoopbackWorker::LoopBackIntfVrfConfigHandler(NsmLocalIntfLoopbackMessage *pMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::validateIntfLoopbackVrfConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::loopbackVrfConfigBackendStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::updateVrfConfigMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::updateErrorCodeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::prismSynchronousLinearSequencerFailedStep)
        };
        DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pDceSynchronousLinearSeqContext->execute ();

    }

    ResourceId NsmLocalIntfLoopbackWorker::updateErrorCodeStep(DceSynchronousLinearSeqContext *pContext)
    {

        /* Pass the completion status in previous step as status */

        PrismMessage *pPrismMessage = pContext->getPPrismMessage();
        ResourceId status = pPrismMessage->getCompletionStatus();

        return (status);
    }

    void NsmLocalIntfLoopbackWorker::LoopBackIntfConfigHandler(NsmLocalIntfLoopbackMessage *pMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::validateIntfLoopbackStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::doIntfLoopbackStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::doIntfLoopbackMOStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalIntfLoopbackWorker::NsmLocalIntfLoopbackWorker::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

    }

    NsmIntfLoopbackManagedObject* NsmLocalIntfLoopbackWorker::
        findIntfLoopbackMO(UI32 ifId, const SI32 &mapId,
        PrismSynchronousLinearSequencerContext  *pContext)
    {
        vector<WaveManagedObject *> *pResults;

        WaveManagedObjectSynchronousQueryContext 
            syncQueryCtxt(NsmIntfLoopbackManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeUI32(&ifId, "ifId"));
        if (mapId > 0) {
            LocationId locId = DcmToolKit::getLocationIdFromMappedId(mapId);
            syncQueryCtxt.setLocationIdFilter(locId);
        }

        pResults = querySynchronously(&syncQueryCtxt);

        NsmIntfLoopbackManagedObject *pNsmIntfLoopbackManagedObject = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();

            if (1 == numberOfResults) {
                trace(TRACE_LEVEL_DEBUG, string(__FILE__) + __LINE__ +
                    string("Record found!"));

                pNsmIntfLoopbackManagedObject =
                    dynamic_cast<NsmIntfLoopbackManagedObject *>((*pResults)[0]);
            }
            else {
                trace(TRACE_LEVEL_DEBUG, string(__FILE__) + __LINE__ +
                    string("Record not found! (!= 1)"));
            }

            // user shoudld add it to GC
            // pContext->addManagedObjectsForGarbageCollection(*pResults);
            //pResults->clear ();
            delete pResults;
        }
        else {
            trace(TRACE_LEVEL_DEBUG, string(__FILE__) + __LINE__ +
                string("Record not found! (NULL)"));
        }

        return pNsmIntfLoopbackManagedObject;
    }

    ResourceId NsmLocalIntfLoopbackWorker::validateIntfLoopbackVrfConfigStep(DceSynchronousLinearSeqContext *pContext)
    {

        string vrfName;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        NsmLocalIntfLoopbackMessage *pMessage = 
            dynamic_cast<NsmLocalIntfLoopbackMessage*> (pContext->getPPrismMessage());
        UI32 ifId = pMessage->getIfId();
        UI32 cmdCode = pMessage->getCmdCode();

        if (cmdCode == NSM_MSG_VRF_BIND_INTF) {
            vrfName = pMessage->getVrfName();
        }

        LocationId locationId = FrameworkToolKit::getThisLocationId ();

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NsmIntfLoopbackManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeUI32(&ifId, "ifId"));
        vector<WaveManagedObject *> *pResults = 
                querySynchronouslyLocalManagedObjectsForLocationId (locationId, &syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            status =  WRC_NSM_INTERFACE_DOES_NOT_EXIST;
            pMessage->setCompletionStatus(status);
            return (status);
        }

        NsmIntfLoopbackManagedObject *loopbackIntfMO = dynamic_cast<NsmIntfLoopbackManagedObject*>((*pResults)[0]);
        if (loopbackIntfMO != NULL) {
            pContext->getCache()->put(NsmIntfLoopbackManagedObject::getClassName(), loopbackIntfMO, true);
            if(loopbackIntfMO->getVrfObjectId() != ObjectId::NullObjectId &&
                       cmdCode == NSM_MSG_VRF_BIND_INTF) {
                trace (TRACE_LEVEL_ERROR, "validateIntfLoopbackStep VRF is configured!");
                status = WRC_NSM_INTFERFACE_VRF_ALREADY_CONFIGURED_ERROR;
                pMessage->setCompletionStatus(status);
                return (status);
            }
        } else {
            status =  WRC_NSM_INTERFACE_DOES_NOT_EXIST;
            pMessage->setCompletionStatus(status);
            return (status);
        }

        if (cmdCode == NSM_MSG_VRF_UNBIND_INTF) {
            string vrf = VrfUtils::getVrfNameByObjectId(loopbackIntfMO->getVrfObjectId());
            pMessage->setVrfName(vrf);
            pMessage->setCompletionStatus(status);
            return (status);
        }

        ObjectId vrfObjectId = ObjectId::NullObjectId;
        status = VrfUtils::getVrfObjectIdByName(vrfName, vrfObjectId);
        if (status == WAVE_MESSAGE_SUCCESS && vrfObjectId != ObjectId::NullObjectId) {
            pContext->getCache()->put("VrfObjectId", vrfObjectId);
        }

        pMessage->setCompletionStatus(status);
        return (status);
    }

    ResourceId NsmLocalIntfLoopbackWorker::loopbackVrfConfigBackendStep (DceSynchronousLinearSeqContext *pContext)
    {
        NsmLocalIntfLoopbackMessage *pMessage = 
            dynamic_cast<NsmLocalIntfLoopbackMessage*> (pContext->getPPrismMessage());
        UI32 cmdCode = pMessage->getCmdCode();
        string vrfName = pMessage->getVrfName();
        ResourceId status = WAVE_MESSAGE_ERROR;

        NsmClientConfigVRFMessage *m = new NsmClientConfigVRFMessage();
        m->setOpcode(cmdCode);
        m->setVrfName(vrfName);
        m->setIfType(IF_TYPE_INTF_LOOPBACK);
        m->setIfName(getString(pMessage->getIfId()));

        status = sendSynchronouslyToWaveClient ("nsm", m);
        if (status == WAVE_MESSAGE_SUCCESS) {
            if (m->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) {
                status = m->getCompletionStatus ();
            } else {
                int err = m->getClientStatus();
                status = NsmUtils::NsmBackendErrorMapToDcmStatus(err);
            }
        }
        trace (TRACE_LEVEL_INFO, "loopbackVrfConfigBackendStep: Completed backend config");

        delete (m); 
        pMessage->setCompletionStatus(status);
        return (status);

    }

    ResourceId NsmLocalIntfLoopbackWorker::updateVrfConfigMOStep(DceSynchronousLinearSeqContext *pContext)
    {
        NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
        NsmLocalIntfLoopbackMessage *pMessage = 
            dynamic_cast<NsmLocalIntfLoopbackMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();
        ResourceId status = WAVE_MESSAGE_ERROR;


        NsmIntfLoopbackManagedObject *loopbackIntfMO = dynamic_cast<NsmIntfLoopbackManagedObject*>
               (pContext->getCache()->getWaveManagedObject(NsmIntfLoopbackManagedObject::getClassName()));
        if (loopbackIntfMO) {
            if (cmdCode == NSM_MSG_VRF_BIND_INTF) {
                ObjectId vrfObjectId = pContext->getCache()->getObjectId("VrfObjectId");
                loopbackIntfMO->setVrfObjectId(vrfObjectId);
            } else if (cmdCode == NSM_MSG_VRF_UNBIND_INTF) {
                loopbackIntfMO->setVrfObjectId(ObjectId::NullObjectId);
            }

            pL3ReadyMo = new NsmL3ReadyManagedObject(
                     dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
            if (pL3ReadyMo != NULL) {
                pL3ReadyMo->setInterfaceName(ifName);
                pL3ReadyMo->setIntfType(IF_TYPE_INTF_LOOPBACK);
                loopbackIntfMO->setL3Ready(pL3ReadyMo);
            } else {
                // Malloc problem so just return Generic Error
                trace(TRACE_LEVEL_ERROR, "NsmLocalIntfLoopbackWorker:updateVrfConfigMOStep Malloc Failed!");
                pMessage->setCompletionStatus(status);
                return (status);
            }
            updateWaveManagedObject(loopbackIntfMO);
        }

        trace (TRACE_LEVEL_INFO, "updateVrfConfigMOStep: Completed MO updation");
        //pMessage->setCompletionStatus(WRC_NSM_VRF_INTERFACE_BIND_WARNING);
        pMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        return (WAVE_MESSAGE_SUCCESS);

    }

}

