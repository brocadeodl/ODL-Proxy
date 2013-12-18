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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : aantony                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/NsmLocalObjectManager.h"

#include "Nsm/Global/NsmGlobalTypes.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmResourceIds.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"

#include "Nsm/Local/Layer3/NsmIpConfigManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrConfigManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrManagedObject.h"

#include "Nsm/Global/Layer3/NsmGlobalPoIpMessage.h"
#include "Nsm/Global/Layer3/NsmGlobalVlanIpMessage.h"
#include "Nsm/Local/Layer3/NsmLocalNodeIpMessage.h"

#include "Nsm/Global/Layer3/NsmGlobalL3IpWorker.h"

namespace DcmNs
{

    NsmGlobalL3IpWorker::NsmGlobalL3IpWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : WaveWorker  (pNsmGlobalObjectManager)
    {
        addOperationMap (NSMGLOBALPOIPMSG, reinterpret_cast<PrismMessageHandler> (&NsmGlobalL3IpWorker::NsmGlobalPoIpMessageHandler));
    }

    NsmGlobalL3IpWorker::~NsmGlobalL3IpWorker ()
    {
    }

    PrismMessage  *NsmGlobalL3IpWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMGLOBALPOIPMSG :
                pPrismMessage = new NsmGlobalPoIpMessage ();
                break;

            case NSMGLOBALVLANIPMSG :
                pPrismMessage = new NsmGlobalVlanIpMessage ();
                break;

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *NsmGlobalL3IpWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        return (pWaveManagedObject);
    }

////

    void  NsmGlobalL3IpWorker::NsmGlobalPoIpMessageHandler(NsmGlobalPoIpMessage *pMessage)
    {
// Validations and Queries should go here


        trace(TRACE_LEVEL_INFO, string("NsmGlobalPoIpMessageHandler") +
            " OpCode: " + pMessage->getOpCode() +
            " ComCode: " + pMessage->getCmdCode() +
            " rbridge: " + pMessage->getRbridge() +
            " ID: " + pMessage->getIfId() +
            " Name: " + pMessage->getIfName() +
            " ipv4-addr: " + pMessage->getIpv4Address() +
            " switchport: " + pMessage->getSwitchport() +
            + "\n");


        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
// Validations and Queries should go here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmGlobalL3IpWorker::validatePoIntfIpStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmGlobalL3IpWorker::doPoIntfIpStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmGlobalL3IpWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmGlobalL3IpWorker::doPoIntfIpMoStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmGlobalL3IpWorker::prismSynchronousLinearSequencerCommitTransactionStep),

// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmGlobalL3IpWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmGlobalL3IpWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId NsmGlobalL3IpWorker::doPoIntfIpStep(
    		DceSynchronousLinearSeqContext *pContext)
    {
        int status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmGlobalL3IpWorker::doPoIntfIpStep() ";

        trace (TRACE_LEVEL_INFO, "Inside " + func_name);

        NsmGlobalPoIpMessage *pMessage =
            dynamic_cast<NsmGlobalPoIpMessage *> (pContext->getPPrismMessage());

        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();
        UI32 opCode = pMessage->getOpCode();

        // NodePo Creation/Deletion, no need to send to local or client
        if (cmdCode == NSM_MSG_CMD_NODE_PO) {
            status = WAVE_MESSAGE_SUCCESS;
            pMessage->setCompletionStatus(status);
            trace(TRACE_LEVEL_INFO, func_name + " Intf: " + ifName + 
                " CmdCode=" + cmdCode + " OpCode=" + opCode +
                " - Message is not needed to send to Local Worker. status=" + 
                status);
            return status;
        }

        NsmLocalNodeIpMessage *msg = new NsmLocalNodeIpMessage();
        msg->setCmdCode(pMessage->getCmdCode());
        msg->setOpCode(pMessage->getOpCode());
        msg->setIfType(pMessage->getIfType());
        msg->setIfId(pMessage->getIfId());
        msg->setIfName(pMessage->getIfName());
        msg->setSwitchport(pMessage->getSwitchport());
        msg->setIpv4Address(pMessage->getIpv4Address());
        msg->setSecond(pMessage->getSecond());

        msg->setMtu(pMessage->getMtu());
        msg->setDirectedBroadcast(pMessage->getDirectedBroadcast());
        msg->setProxyArp(pMessage->getProxyArp());
        msg->setArpAgingTimeout(pMessage->getArpAgingTimeout());

        trace (TRACE_LEVEL_DEBUG, func_name + " GLOBAL IntfName: " + ifName +
            " IfId: " + pMessage->getIfId() +
            " IfType: " + pMessage->getIfType() +
            " OP-CODE: " + pMessage->getOpCode() +
            " CMD-CODE: " + pMessage->getCmdCode() +
            " switchport: " + pMessage->getSwitchport() +
            " secondary: " + pMessage->getSecond() +
            " IP-ADDR: " + pMessage->getIpv4Address() +
            " MTU: " + pMessage->getMtu() +
            " DirectedBroadcast: " + pMessage->getDirectedBroadcast() +
            " ProxyArp: " + pMessage->getProxyArp() +
            " ARP-AGING-TIMEOUT: " + pMessage->getArpAgingTimeout() +
            "");

        trace (TRACE_LEVEL_DEBUG, func_name + " LOCAL IntfName: " + ifName +
            " IfId: " + msg->getIfId() +
            " IfType: " + msg->getIfType() +
            " OP-CODE: " + msg->getOpCode() +
            " CMD-CODE: " + msg->getCmdCode() +
            " switchport: " + msg->getSwitchport() +
            " secondary: " + msg->getSecond() +
            " IP-ADDR: " + msg->getIpv4Address() +
            " MTU: " + msg->getMtu() +
            " DirectedBroadcast: " + msg->getDirectedBroadcast() +
            " ProxyArp: " + msg->getProxyArp() +
            " ARP-AGING-TIMEOUT: " + msg->getArpAgingTimeout() +
            "");


        trace (TRACE_LEVEL_DEBUG, func_name + 
            "Prepare to send message ...");
        status = sendSynchronously(msg);
        if (status == WAVE_MESSAGE_SUCCESS) {
            status = msg->getCompletionStatus();
            if (status != WAVE_MESSAGE_SUCCESS) {
                trace (TRACE_LEVEL_INFO, func_name + " Intf: " + ifName + 
                    " CmdCode=" + cmdCode + " OpCode=" + opCode +
                    " getCompletionStatus() FAILED" + " status = " + status);
            }
        }

        pMessage->setCompletionStatus(status);
        trace(TRACE_LEVEL_INFO, func_name + " Intf: " + ifName + 
            " CmdCode=" + cmdCode + " OpCode=" + opCode +
            " - Message has been sent to Local Worker. status=" + status);

        delete msg;

        if (status == WAVE_MESSAGE_SUCCESS &&
            pMessage->getOpCode() != NSM_MSG_OP_DELETE &&
            pMessage->getCmdCode() == NSM_MSG_CMD_IP_ADDRESS)
        { // Add ip-address
            NsmIpAddrConfigManagedObject *pIpCfgMo = NULL;
            NsmL3ReadyManagedObject *pL3ReadyMo =
				dynamic_cast<NsmL3ReadyManagedObject *>(pContext->getCache()->getWaveManagedObject(
						NsmL3ReadyManagedObject::getClassName()));
            if (pL3ReadyMo) {
            	pIpCfgMo = (WaveManagedObjectPointer<NsmIpAddrConfigManagedObject>(pL3ReadyMo->getIpAddrConfig())).operator ->();
            }
            if (pIpCfgMo) { // Not the 1st ip-address
                return status;
            }

            // Send another message to enable proxy-arp on the 1st ip-address
            NsmLocalNodeIpMessage *m1 = new NsmLocalNodeIpMessage();

            m1->setCmdCode(NSM_MSG_CMD_PROXY_ARP);
            m1->setOpCode(NSM_MSG_OP_UPDATE);
            m1->setIfType(pMessage->getIfType());
            m1->setIfId(pMessage->getIfId());
            m1->setIfName(pMessage->getIfName());

            m1->setProxyArp(pMessage->getProxyArp());

            trace (TRACE_LEVEL_INFO, func_name + " Proxy-Arp Message: " + 
                ifName +
                " IfId: " + m1->getIfId() +
                " IfType: " + m1->getIfType() +
                " OP-CODE: " + m1->getOpCode() +
                " CMD-CODE: " + m1->getCmdCode() +
                " ProxyArp: " + m1->getProxyArp() +
                "");

            trace (TRACE_LEVEL_INFO, func_name + 
                "Prepare to send another proxy-arp message ...");
            status = sendSynchronously(m1);
            if (status == WAVE_MESSAGE_SUCCESS) {
                status = m1->getCompletionStatus();
                if (status != WAVE_MESSAGE_SUCCESS) {
                    trace (TRACE_LEVEL_INFO, func_name + " Intf: " + ifName + 
                        " CmdCode=" + cmdCode + " OpCode=" + opCode +
                        " getCompletionStatus() FAILED" + " status = " + status);
                }
            }

            pMessage->setCompletionStatus(status);
            trace(TRACE_LEVEL_INFO, func_name + " Intf: " + ifName + 
                " CmdCode=" + cmdCode + " OpCode=" + opCode +
                " - Proxy-Arp Message has been sent to Local Worker. status=" + 
                status);

            delete m1;
        }

        return  status;
    }

    ResourceId NsmGlobalL3IpWorker::doPoIntfIpAddrMoStep(NsmL3ReadyManagedObject *pL3ReadyMo,
        PrismSynchronousLinearSequencerContext *pContext)
    {
        string func_name = "NsmGlobalL3IpWorker::doPoIntfIpAddrMoStep() ";
        NsmGlobalPoIpMessage *pMessage =
            dynamic_cast<NsmGlobalPoIpMessage *> (pContext->getPPrismMessage());
        string rbridge = pMessage->getRbridge();
        string ifName = pMessage->getIfName();
        bool second = pMessage->getSecond();
        string ipaddr = pMessage->getIpv4Address();

        trace (TRACE_LEVEL_INFO, "Inside " + func_name + 
            " Rbridge: " + rbridge +
            " IntfName: " + ifName);

        NsmIpAddrConfigManagedObject *pIpCfgMo;
        NsmIpAddrManagedObject *pIpAddrMo;

        pIpCfgMo = (WaveManagedObjectPointer<NsmIpAddrConfigManagedObject>(pL3ReadyMo->getIpAddrConfig())).operator ->();

        if (pMessage->getOpCode() != NSM_MSG_OP_DELETE) {
            if (pIpCfgMo == NULL) {
                if (second) {
                    trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
                        " IpAddriConfig MO  is NOT ready. "
                        "Secondary Ip-Addr can't be configured!");
                    return WAVE_MESSAGE_SUCCESS;
                }
                pIpCfgMo = new NsmIpAddrConfigManagedObject(
                    dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));

                // Enable proxy-arp on the 1st ip-address
                NsmIpConfigManagedObject *pIpConfigMo = 
                    (WaveManagedObjectPointer<NsmIpConfigManagedObject>
                    (pL3ReadyMo->getIpConfig())).operator->();

                if (!pIpConfigMo) {
                    pIpConfigMo = new NsmIpConfigManagedObject(
                        dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
                    pL3ReadyMo->setIpConfig(pIpConfigMo);
					pL3ReadyMo->addAttributeToBeUpdated("ipConfig");
                    updateWaveManagedObject(pL3ReadyMo);
                }
                pIpConfigMo->setProxyArp(true);
				pIpConfigMo->addAttributeToBeUpdated("proxy_arp");
                updateWaveManagedObject(pIpConfigMo);
                ////
            }
            else {
                if (pIpCfgMo->findIpAddrIndex(ipaddr) >= 0) {
                    trace (TRACE_LEVEL_INFO, func_name + " IntfName: " +
                        ifName + " IpAddr: " + ipaddr + " exists!");
                    return WAVE_MESSAGE_SUCCESS;
                }
            }

            if (second) {
    	        pIpAddrMo = new NsmIpAddrManagedObject(
    	                dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
    	        pIpAddrMo->setIpv4AddrMask(pMessage->getIpv4Address());
    	        pIpAddrMo->setSecond(true);
    	        //prismAssert (NULL != pIpCfgMo, __FILE__, __LINE__);
    	        pIpCfgMo->addIpAddrMO(pIpAddrMo);
    	        pIpCfgMo->addAttributeToBeUpdated("ipAddr");
    	        updateWaveManagedObject(pIpCfgMo);

    	        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
    	            " Secondary IP-ADDRESS is added!");
            }
            else {
                pIpAddrMo = new NsmIpAddrManagedObject(
                    dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
                pIpAddrMo->setIpv4AddrMask(pMessage->getIpv4Address());
                pIpAddrMo->setSecond(false);
                if (NULL == pIpCfgMo->getPrimaryIpAddrMO()) {
                	pIpCfgMo->addIpAddrMO(pIpAddrMo);
                    pL3ReadyMo->setIpAddrConfig(pIpCfgMo);
					pL3ReadyMo->addAttributeToBeUpdated("ipAddrCfg");
                    updateWaveManagedObject(pL3ReadyMo);

                    trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
                        " Primary IP-ADDRESS is added!");
                }
                else {
                	pIpCfgMo->addIpAddrMO(pIpAddrMo);
    	        	pIpCfgMo->addAttributeToBeUpdated("ipAddr");
                    updateWaveManagedObject(pIpCfgMo);

                    trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
                        " New Primary IP-ADDRESS is Added!");
                }
            }
        }
        else {  // DELETE
            //prismAssert (NULL != pIpCfgMo, __FILE__, __LINE__);

            if (pMessage->getCmdCode() == NSM_MSG_CMD_ALL_IPADDR) {
            	notifyVRRPIPDeletion(pL3ReadyMo, ipaddr, true);
                pL3ReadyMo->setIpAddrConfig(NULL);
				pL3ReadyMo->addAttributeToBeUpdated("ipAddrCfg");
                updateWaveManagedObject(pL3ReadyMo);
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
                    " All Ip-Addr are deleted!");
                return WAVE_MESSAGE_SUCCESS;
            }
            if (pIpCfgMo->findIpAddrIndex(ipaddr) < 0) {
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " +
                        ifName + " IpAddr: " + ipaddr + " doesn't exist!");
                return WAVE_MESSAGE_SUCCESS;
            }
            if (pIpCfgMo->isPrimaryIpAddr(ipaddr)) {
                notifyVRRPIPDeletion(pL3ReadyMo, ipaddr, false);
                pIpCfgMo->deleteIpAddr(ipaddr);
                if (NULL == pIpCfgMo->getPrimaryIpAddrMO()) {
                    pL3ReadyMo->setIpAddrConfig(NULL);
					pL3ReadyMo->addAttributeToBeUpdated("ipAddrCfg");
                    updateWaveManagedObject(pL3ReadyMo);
                }
                else {
                	pIpCfgMo->addAttributeToBeUpdated("ipAddr");
                    updateWaveManagedObject(pIpCfgMo);
                }
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " +
                    ifName + " Delete primary ip-address.");
            }
            else {
                pIpCfgMo->deleteIpAddr(ipaddr);
                pIpCfgMo->addAttributeToBeUpdated("ipAddr");
                updateWaveManagedObject(pIpCfgMo);
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " +
                    ifName + " Delete secondary ip-address.");
            }
        }

        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " Update MO done!");

        return WAVE_MESSAGE_SUCCESS;
    }


    ResourceId NsmGlobalL3IpWorker::doPoIntfSwitchportMoStep(
    		DceSynchronousLinearSeqContext *pContext)
    {
        string func_name = "NsmGlobalL3IpWorker::doPoIntfSwitchportMoStep() ";
        NsmGlobalPoIpMessage *pMessage =
            dynamic_cast<NsmGlobalPoIpMessage *> (pContext->getPPrismMessage());
        string rbridge = pMessage->getRbridge();
        string ifName = pMessage->getIfName();

        trace (TRACE_LEVEL_INFO, "Inside " + func_name + 
            " Rbridge: " + rbridge +
            " IntfName: " + ifName);

        PoIntfManagedObject *pPoIntfManagedObject =
			dynamic_cast<PoIntfManagedObject *>(pContext->getCache()->getWaveManagedObject(
					PoIntfManagedObject::getClassName()));
        prismAssert (NULL != pPoIntfManagedObject, __FILE__, __LINE__);

        NsmNodePoManagedObject *pNodePoMo =
			(WaveManagedObjectPointer<NsmNodePoManagedObject>(
					pPoIntfManagedObject->getNodePo())).operator->();
        if (pNodePoMo == NULL) {
            trace (TRACE_LEVEL_INFO, func_name + 
                " Rbridge: " + rbridge +
                " IntfName: " + ifName +
                " NodePoMO is not ready, switchport is ignored!");
            return WAVE_MESSAGE_SUCCESS;
        }

        NsmL3ReadyManagedObject *pL3ReadyMo =
			dynamic_cast<NsmL3ReadyManagedObject *>(pContext->getCache()->getWaveManagedObject(
					NsmL3ReadyManagedObject::getClassName()));

        if (pMessage->getSwitchport()) {
            if (pL3ReadyMo != NULL) {
                // TO-DO
                pNodePoMo->setL3Ready(NULL);
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + 
                    ifName + " Now L3 is disabled!");
                updateWaveManagedObject(pNodePoMo);
            }
            else {
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + 
                    ifName + " L3 is aleady disabled!");
            }
        }
        else {
            if (pL3ReadyMo == NULL) {
                pL3ReadyMo = new NsmL3ReadyManagedObject(
                    dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));

                pL3ReadyMo->setInterfaceName(getString(pMessage->getIfId()));
                pL3ReadyMo->setIntfType(IF_TYPE_PO);
                pNodePoMo->setL3Ready(pL3ReadyMo);
                updateWaveManagedObject(pNodePoMo);
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + 
                    ifName + " Now L3 is enabled!");
            }
            else {
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + 
                    ifName + " L3 is aleady enabled!");
            }
        }
        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId NsmGlobalL3IpWorker::doNodePoMoStep(
    		DceSynchronousLinearSeqContext *pContext)
    {
        string func_name = "NsmGlobalL3IpWorker::doNodePoMoStep() ";
        NsmGlobalPoIpMessage *pMessage =
            dynamic_cast<NsmGlobalPoIpMessage *> (pContext->getPPrismMessage());
        string rbridge = pMessage->getRbridge();
        UI32 opcode = pMessage->getOpCode();
        UI32 ifId = pMessage->getIfId();
        string ifName = pMessage->getIfName();

        trace (TRACE_LEVEL_INFO, "Inside " + func_name + 
            " Rbridge: " + rbridge +
            " IntfName: " + ifName);

        PoIntfManagedObject *pPoIntfManagedObject =
			dynamic_cast<PoIntfManagedObject *>(pContext->getCache()->getWaveManagedObject(
					PoIntfManagedObject::getClassName()));
        prismAssert (NULL != pPoIntfManagedObject, __FILE__, __LINE__);

        NsmNodePoManagedObject *pNodePoMo =
			(WaveManagedObjectPointer<NsmNodePoManagedObject>(
					pPoIntfManagedObject->getNodePo())).operator->();

        NsmL3ReadyManagedObject *pL3ReadyMo =
			dynamic_cast<NsmL3ReadyManagedObject *>(pContext->getCache()->getWaveManagedObject(
					NsmL3ReadyManagedObject::getClassName()));

        if (opcode != NSM_MSG_OP_DELETE) {
            if (pNodePoMo == NULL) {
                pNodePoMo = new NsmNodePoManagedObject(
                    dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
                pNodePoMo->setIfId(ifId);

                if (pPoIntfManagedObject->getSwModeFlag()) {
                  trace (TRACE_LEVEL_INFO, func_name + " IntfName: " +
                    ifName + " switchport has been set. "
                    " L3 can't be enabled!");
                }
                else {
                    pL3ReadyMo = new NsmL3ReadyManagedObject(
                        dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
                   pL3ReadyMo->setInterfaceName(getString(pMessage->getIfId()));
                   pL3ReadyMo->setIntfType(IF_TYPE_PO);
                   pNodePoMo->setL3Ready(pL3ReadyMo);

                    trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + 
                        ifName + " Now L3 is enabled!");
                }
                pPoIntfManagedObject->setNodePo(pNodePoMo);
                updateWaveManagedObject(pPoIntfManagedObject);
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + 
                    ifName + " Now NodePo is enabled!");
            }
            else { // pNodePoMo != NULL
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + 
                    ifName + " NodePo is aleady enabled!");

                if (pL3ReadyMo == NULL) {
                    if (pPoIntfManagedObject->getSwModeFlag()) {
                        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " +
                          ifName + " switchport has been set. "
                          " L3 can't be enabled!");
                    }
                    else {
                        pL3ReadyMo = new NsmL3ReadyManagedObject(
                            dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
                        pL3ReadyMo->setInterfaceName(getString(pMessage->getIfId()));
                        pL3ReadyMo->setIntfType(IF_TYPE_PO);
                        pNodePoMo->setL3Ready(pL3ReadyMo);
                        updateWaveManagedObject(pNodePoMo);
                        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + 
                            ifName + " Now L3 is enabled!");
                    }
                }
                else { // pL3ReadyMo != NULL
                    trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + 
                        ifName + " L3 is aleady enabled!");
                }
            }
        }
        else { // DELETE NODE-PO
            if (pNodePoMo != NULL) {
                // TO-DO
                pPoIntfManagedObject->setNodePo(NULL);
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + 
                    ifName + " Now NodePo is disabled!");
                updateWaveManagedObject(pPoIntfManagedObject);
            }
            else {
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + 
                    ifName + " NodePo is aleady disabled!");
            }
            ////
        }
        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId NsmGlobalL3IpWorker::doPoIntfIpMoStep(
    		DceSynchronousLinearSeqContext *pContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmGlobalL3IpWorker::doPoIntfIpMoStep() ";
        NsmGlobalPoIpMessage *pMessage =
            dynamic_cast<NsmGlobalPoIpMessage *> (pContext->getPPrismMessage());
        string rbridge = pMessage->getRbridge();
        UI32 opCode = pMessage->getOpCode();
        UI32 ifId = pMessage->getIfId();
        string ifName = pMessage->getIfName();

        trace (TRACE_LEVEL_INFO, "Inside " + func_name + 
            " Rbridge: " + rbridge +
            " IntfName: " + ifName);

        trace (TRACE_LEVEL_INFO, func_name + " Rbridge: " + rbridge +
            " IntfId: " + ifId +
            " IntfName: " + ifName +
            " OP-CODE: " + pMessage->getOpCode() +
            " CMD-CODE: " + pMessage->getCmdCode() +
            " switchport: " + pMessage->getSwitchport() +
            " secondary: " + pMessage->getSecond() +
            " IP-ADDR: " + pMessage->getIpv4Address());

        PoIntfManagedObject *pPoIntfManagedObject =
			dynamic_cast<PoIntfManagedObject *>(pContext->getCache()->getWaveManagedObject(
					PoIntfManagedObject::getClassName()));
        if (pPoIntfManagedObject == NULL) {
            trace (TRACE_LEVEL_ERROR, func_name + 
                "PoIntfMO not found!" +
                "PoId=" + ifId);

            return WAVE_MESSAGE_SUCCESS;
        }

        prismAssert (NULL != pPoIntfManagedObject, __FILE__, __LINE__);

        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " Found PoIntfMO!");

        // NsmL3Util::PoIdCheckNodePo(ifId, mapId, pContext);
        // NsmL3Util::PoIdCheckL3Ready(ifId, mapId, pContext);
        // NsmL3Util::PoIdCheckIpAddrReady(ifId, mapId, pContext);

        NsmL3ReadyManagedObject *pL3ReadyMo =
			dynamic_cast<NsmL3ReadyManagedObject *>(pContext->getCache()->getWaveManagedObject(
					NsmL3ReadyManagedObject::getClassName()));

        switch(pMessage->getCmdCode()) {
            case NSM_MSG_CMD_NODE_PO:
            {
                status = doNodePoMoStep(pContext);
                break;
            }
            case NSM_MSG_CMD_SWITCHPORT:
            {
                status = doPoIntfSwitchportMoStep(pContext);
               break;
            }
            case NSM_MSG_CMD_IP_ADDRESS:
            case NSM_MSG_CMD_ALL_IPADDR:
                if (pL3ReadyMo != NULL) {
                    status = doPoIntfIpAddrMoStep(pL3ReadyMo, pContext);
                }
                else {
                    trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
                        " L3 is NOT ready. Ip-Addr can't be configured!");
                }
                break;

            default:
            {
                if (opCode == NSM_MSG_OP_DELETE) {
                	if (pL3ReadyMo != NULL) {
                		status = deletePoIntfIpConfigMoStep(pL3ReadyMo, pContext);
                	} else {
                		trace (TRACE_LEVEL_INFO, func_name + "IntfName: " + ifName +
                				" L3 is not ready. Delete operation can't be performed!");
                		status = WAVE_MESSAGE_ERROR;
                	}
                }
                else {
                	if (pL3ReadyMo != NULL) {
                		status = updatePoIntfIpConfigMoStep(pL3ReadyMo, pContext);
                	} else {
                		trace (TRACE_LEVEL_INFO, func_name + "IntfName: " + ifName +
                				" L3 is not ready. Configuration can't be performed!");
                	}
                }

                break;
            }
        }

        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " Update MO done!");


        return status;
    }

    ResourceId NsmGlobalL3IpWorker::updatePoIntfIpConfigMoStep(NsmL3ReadyManagedObject *pL3ReadyMo,
                PrismSynchronousLinearSequencerContext *pContext)
    { 
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmGlobalL3IpWorker::updatePoIntfIpConfigMoStep() ";
        NsmGlobalPoIpMessage *pMessage =
        dynamic_cast<NsmGlobalPoIpMessage *> (pContext->getPPrismMessage());
        UI32 ifId = pMessage->getIfId();
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();

        trace (TRACE_LEVEL_INFO, "Inside " + func_name +
            " IntfName: " + ifName);

        trace (TRACE_LEVEL_INFO, func_name + 
            " IntfId: " + ifId + 
            " IntfName: " + ifName + 
            " OP-CODE: " + pMessage->getOpCode() +
            " CMD-CODE: " + pMessage->getCmdCode() +
            " switchport: " + pMessage->getSwitchport() +
            " secondary: " + pMessage->getSecond() +
            " IP-ADDR: " + pMessage->getIpv4Address() +
            " MTU: " + pMessage->getMtu() +
            " DirectedBroadcast: " + pMessage->getDirectedBroadcast() +
            " ProxyArp: " + pMessage->getProxyArp() +
            " ARP-AGING-TIMEOUT: " + pMessage->getArpAgingTimeout() +
            "");

        NsmIpConfigManagedObject *pIpConfigMo = (WaveManagedObjectPointer<NsmIpConfigManagedObject>(pL3ReadyMo->getIpConfig())).operator ->();
        if (pIpConfigMo == NULL) {
           pIpConfigMo = new NsmIpConfigManagedObject(
               dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
           pL3ReadyMo->setIpConfig(pIpConfigMo);
           updateWaveManagedObject(pL3ReadyMo);
        }

        switch (cmdCode) {
            case NSM_MSG_CMD_MTU:
                pIpConfigMo->setMtu(pMessage->getMtu());
                updateWaveManagedObject(pIpConfigMo);
                break;

            case NSM_MSG_CMD_IP_DIRECTED_BROADCAST:
                pIpConfigMo->setDirectedBroadcast(pMessage->getDirectedBroadcast());
                updateWaveManagedObject(pIpConfigMo);
                break;

            case NSM_MSG_CMD_PROXY_ARP:
                pIpConfigMo->setProxyArp(pMessage->getProxyArp());
                updateWaveManagedObject(pIpConfigMo);
                break;

            case NSM_MSG_CMD_ARP_AGING_TIMEOUT:
                pIpConfigMo->setArpAgingTimeout(pMessage->getArpAgingTimeout());
                updateWaveManagedObject(pIpConfigMo);
                break;

            default:
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
                    " Wrong CmdCode: " + cmdCode);
                break;
        }

        return status;
    }

    ResourceId NsmGlobalL3IpWorker::deletePoIntfIpConfigMoStep(NsmL3ReadyManagedObject *pL3ReadyMo,
                PrismSynchronousLinearSequencerContext *pContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmGlobalL3IpWorker::deletePoIntfIpConfigMoStep() ";
        NsmGlobalPoIpMessage *pMessage =
        dynamic_cast<NsmGlobalPoIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();

        trace (TRACE_LEVEL_INFO, "Inside " + func_name +
            " IntfName: " + ifName);

        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + 
            " OP-CODE: " + pMessage->getOpCode() +
            " CMD-CODE: " + pMessage->getCmdCode() +
            " switchport: " + pMessage->getSwitchport() +
            " secondary: " + pMessage->getSecond() +
            " IP-ADDR: " + pMessage->getIpv4Address() +
            " MTU: " + pMessage->getMtu() +
            " DirectedBroadcast: " + pMessage->getDirectedBroadcast() +
            " ProxyArp: " + pMessage->getProxyArp() +
            " ARP-AGING-TIMEOUT: " + pMessage->getArpAgingTimeout() +
            "");

        NsmIpConfigManagedObject *pIpConfigMo = (WaveManagedObjectPointer<NsmIpConfigManagedObject>(pL3ReadyMo->getIpConfig())).operator ->();
        if (pIpConfigMo == NULL) {
            trace (TRACE_LEVEL_ERROR, func_name + " IntfName: " + ifName +
                " NSMIpConfigMO is NULL!");
            return WAVE_MESSAGE_ERROR;
        }

        switch (cmdCode) {
            case NSM_MSG_CMD_MTU:
                pIpConfigMo->setMtu(NSM_IP_MTU_DEF_VAL);
                updateWaveManagedObject(pIpConfigMo);
                break;

            case NSM_MSG_CMD_IP_DIRECTED_BROADCAST:
                pIpConfigMo->setDirectedBroadcast(false);
                updateWaveManagedObject(pIpConfigMo);
                break;

            case NSM_MSG_CMD_PROXY_ARP:
                pIpConfigMo->setProxyArp(false);
                updateWaveManagedObject(pIpConfigMo);
                break;

            case NSM_MSG_CMD_ARP_AGING_TIMEOUT:
                pIpConfigMo->setArpAgingTimeout(
                    NSM_IP_ARP_AGING_TIMEOUT_DEF_VAL);
                updateWaveManagedObject(pIpConfigMo);
                break;

            default:
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
                    " Wrong CmdCode: " + cmdCode);
                break;
        }

        return status;
    }

    ResourceId NsmGlobalL3IpWorker::validatePoIntfIpAddrStep(
    		NsmL3ReadyManagedObject *pL3ReadyMo,
            PrismSynchronousLinearSequencerContext *pContext) {
        string func_name = "NsmGlobalL3IpWorker::validatePoIntfIpAddrStep() ";
        NsmGlobalPoIpMessage *pMessage =
                dynamic_cast<NsmGlobalPoIpMessage *>(pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        bool second = pMessage->getSecond();
        string ipaddr = pMessage->getIpv4Address();

        trace(TRACE_LEVEL_INFO, "Inside " + func_name + " IntfName: " + ifName);

        NsmIpAddrConfigManagedObject *pIpCfgMo = NULL;

        pIpCfgMo = (WaveManagedObjectPointer<NsmIpAddrConfigManagedObject>(pL3ReadyMo->getIpAddrConfig())).operator ->();

        if (pMessage->getOpCode() != NSM_MSG_OP_DELETE) {
            if (pIpCfgMo == NULL) {
                if (second) {
                    trace(
                            TRACE_LEVEL_INFO,
                            func_name + " IntfName: " + ifName
                                    + " IpAddriConfig MO  is NOT ready. "
                                            "Secondary Ip-Addr can't be configured!");
                    return WRC_NSM_API_SET_ERR_CANT_SET_SECONDARY_FIRST;
                }
            } else {
                if (pIpCfgMo->findIpAddrIndex(ipaddr) >= 0) {
                    trace(
                            TRACE_LEVEL_INFO,
                            func_name + " IntfName: " + ifName + " IpAddr: "
                                    + ipaddr + " exists!");

                    return WRC_NSM_API_ERR_IP_ADDRESS_DUPLICATED;
                }
            }
        } else { // DELETE
            if (pIpCfgMo == NULL) {
                trace(
                        TRACE_LEVEL_INFO,
                        func_name + " IntfName: " + ifName
                                + " No IP-ADDR is configured on this interface!");
                return WRC_NSM_API_SET_ERR_ADDRESS_NOT_EXIST;
            }

            if (pMessage->getCmdCode() != NSM_MSG_CMD_ALL_IPADDR) {
                if (pIpCfgMo->findIpAddrIndex(ipaddr) < 0) {
                    trace(
                            TRACE_LEVEL_INFO,
                            func_name + " IntfName: " + ifName + " IpAddr: "
                                    + ipaddr + " doesn't exist!");
                    return WRC_NSM_API_SET_ERR_ADDRESS_NOT_EXIST;
                }
                if (pIpCfgMo->isPrimaryIpAddr(ipaddr)) {
                    pMessage->setSecond(false);
                } else {
                    pMessage->setSecond(true);
                }
            }
        }

        return (WAVE_MESSAGE_SUCCESS);
    }

    ResourceId NsmGlobalL3IpWorker::validatePoIntfIpStep(
    		DceSynchronousLinearSeqContext *pContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmGlobalL3IpWorker::validPoIntfIpStep() ";
        NsmGlobalPoIpMessage *pMessage =
            dynamic_cast<NsmGlobalPoIpMessage *> (pContext->getPPrismMessage());
        UI32 ifId = pMessage->getIfId();
        string ifName = pMessage->getIfName();
        SI32 mapId = 0;

        trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName +
            " OP-CODE: " + pMessage->getOpCode() +
            " CMD-CODE: " + pMessage->getCmdCode() +
            " switchport: " + pMessage->getSwitchport() +
            " secondary: " + pMessage->getSecond() +
            " IP-ADDR: " + pMessage->getIpv4Address());

        PoIntfManagedObject *pPoIntfManagedObject =
            NsmL3Util::PoIdGetPoIfMO(ifId, mapId, pContext);
        if (pPoIntfManagedObject == NULL) {
            trace (TRACE_LEVEL_ERROR, func_name + "PoIntfMO not found! - " +
                ifName);
            return (WRC_NSM_API_ERR_PO_NOT_EXIST);
        }

        trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " Found PoIntfMO!");

        // Store PoIntfMo in cache to use in subsequent steps
        pContext->getCache()->put(PoIntfManagedObject::getClassName(), pPoIntfManagedObject);
        pContext->addManagedObjectForGarbageCollection(pPoIntfManagedObject);

        NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
        NsmNodePoManagedObject *pNodePoMo =
			(WaveManagedObjectPointer<NsmNodePoManagedObject>(
					pPoIntfManagedObject->getNodePo())).operator->();
        if (pNodePoMo) {
        	pL3ReadyMo = (WaveManagedObjectPointer<NsmL3ReadyManagedObject>(pNodePoMo->getL3Ready())).operator->();
        }
        if (pL3ReadyMo) {
        	pL3ReadyMo->setIntfType(IF_TYPE_PO);
        	pContext->getCache()->put(NsmL3ReadyManagedObject::getClassName(), pL3ReadyMo);
        }

        switch(pMessage->getCmdCode()) {
            case NSM_MSG_CMD_NODE_PO:
                break;

            case NSM_MSG_CMD_SWITCHPORT:
            {
                if (pMessage->getSwitchport()) {
                    if (pL3ReadyMo == NULL) {
                        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " L3 is aleady disabled!");
                    }
                }
                else {
                    if (pL3ReadyMo != NULL) {
                        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " L3 is aleady enabled!");
                    }
               }
               break;
            }
            case NSM_MSG_CMD_IP_ADDRESS:
            case NSM_MSG_CMD_ALL_IPADDR:
                if (pL3ReadyMo != NULL) {
                	status = validatePoIntfIpAddrStep(pL3ReadyMo, pContext);
                }
                else {
                    trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
                        " L3 is NOT ready. Ip-Addr can't be configured!");
                    status = WRC_NSM_API_ERR_L3_NOT_READY_NO_IPADDR;
                }
                break;

            case NSM_MSG_CMD_MTU:
            case NSM_MSG_CMD_IP_DIRECTED_BROADCAST:
            case NSM_MSG_CMD_PROXY_ARP:
            case NSM_MSG_CMD_ARP_AGING_TIMEOUT:
                if (pL3ReadyMo == NULL) {
                    trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
                        " L3 is NOT ready. Ip attributes can't be configured!");
                    status = WRC_NSM_API_ERR_L3_NOT_READY;
                }
                break;

            default:
                trace (TRACE_LEVEL_ERROR, func_name + " IntfName: " + ifName +
                    " Wrong CmdCode: " + pMessage->getCmdCode());
                status = WAVE_MESSAGE_ERROR;

                break;
        }


        return status;
    }

    void NsmGlobalL3IpWorker::notifyVRRPIPDeletion(NsmL3ReadyManagedObject* l3ready, const string& ipaddr, bool all)
    {
        vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> > intfConfigs =
        											l3ready->getVRRPIntfConfig();

        vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> >::iterator it;
        for (it = intfConfigs.begin(); it != intfConfigs.end(); ++it) {
        	VRRPSessionConfigLocalManagedObject* sess = it->operator ->();
        	bool updated = false;
        	if (all)
        		updated = sess->deleteVirtualIPAll();
        	else
        		updated = sess->deleteVirtualIPMO(ipaddr);
        	if (updated)
        		updateWaveManagedObject(sess);
        }

        vector<WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> > eIntfConfigs =
        											l3ready->getVRRPEIntfConfig();

        vector<WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> >::iterator it2;
        for (it2 = eIntfConfigs.begin(); it2 != eIntfConfigs.end(); ++it2) {
        	VRRPESessionConfigLocalManagedObject* sess = it2->operator ->();
        	bool updated = false;
        	if (all)
        		updated = sess->deleteVirtualIPAll();
        	else
        		updated = sess->deleteVirtualIPMO(ipaddr);
        	if (updated)
        		updateWaveManagedObject(sess);
        }

        return;
    }


} // DcmNs
