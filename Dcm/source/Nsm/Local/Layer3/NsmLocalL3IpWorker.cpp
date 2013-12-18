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
 *   Author : sanjeevj                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/VLAN/GvlanConfigUtils.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/Nsm/nsmVrfIntf.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmCStatus.h"

#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

#include "Nsm/Local/Layer3/NsmIntfLoopbackManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"

#include "Nsm/Local/Layer3/NsmIpConfigManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrConfigManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrManagedObject.h"
#include "Nsm/Local/Layer3/NsmLocalPhyIpMessage.h"
#include "Nsm/Local/Layer3/NsmLocalNodeIpMessage.h"
#include "Nsm/Local/Layer3/NsmPhyIntfVrfConfigMessage.h"
#include "Nsm/Local/Layer3/NsmVeIntfVrfConfigMessage.h"


#include "Nsm/Local/Layer3/NsmLocalL3IpWorker.h"

#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Vrf/Local/VrfUtils.h"
#include "ClientInterface/Nsm/NsmClientCreateIntIpConfigSPMessage.h"
#include "ClientInterface/Nsm/NsmClientConfigVRFMessage.h"

#include "ClientInterface/VRRP/VRRPClientIntfConfigSPMessage.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"
#include "Vrf/Local/VrfLocalManagedObject.h"

namespace DcmNs
{

    NsmLocalL3IpWorker::NsmLocalL3IpWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
        : WaveWorker  (pNsmLocalObjectManager)
    {
        NsmNodePoManagedObject NsmNodePoManagedObject(pNsmLocalObjectManager);
        NsmNodePoManagedObject.setupOrm ();
        addManagedClass(NsmNodePoManagedObject::getClassName (), this);

        NsmIntfSviManagedObject NsmIntfSviManagedObject(pNsmLocalObjectManager);
        NsmIntfSviManagedObject.setupOrm ();
        addManagedClass(NsmIntfSviManagedObject::getClassName (), this);

        NsmL3ReadyManagedObject NsmL3ReadyManagedObject(pNsmLocalObjectManager);
        NsmL3ReadyManagedObject.setupOrm ();
        addManagedClass(NsmL3ReadyManagedObject::getClassName (), this);

        NsmIpAddrConfigManagedObject NsmIpAddrConfigManagedObject(pNsmLocalObjectManager);
        NsmIpAddrConfigManagedObject.setupOrm ();
        addManagedClass(NsmIpAddrConfigManagedObject::getClassName (), this);

        NsmIpConfigManagedObject NsmIpConfigManagedObject(pNsmLocalObjectManager);
        NsmIpConfigManagedObject.setupOrm ();
        addManagedClass(NsmIpConfigManagedObject::getClassName (), this);

        NsmIpAddrManagedObject NsmIpAddrManagedObject(pNsmLocalObjectManager);
        NsmIpAddrManagedObject.setupOrm ();
        addManagedClass(NsmIpAddrManagedObject::getClassName (), this);

        addOperationMap (NSMLOCALPHYIPMSG, reinterpret_cast<PrismMessageHandler> (&NsmLocalL3IpWorker::NsmLocalPhyIpMessageHandler));

        addOperationMap (NSMLOCALNODEIPMSG, reinterpret_cast<PrismMessageHandler> (&NsmLocalL3IpWorker::NsmLocalNodeIpMessageHandler));

        addOperationMap (NSMPHYINTFVRFCONF, reinterpret_cast<PrismMessageHandler> (&NsmLocalL3IpWorker::NsmPhyIntfVrfConfigMessageHandler));

        addOperationMap (NSMVEINTFVRFCONF, reinterpret_cast<PrismMessageHandler> (&NsmLocalL3IpWorker::NsmVeIntfVrfConfigMessageHandler));
    }

    NsmLocalL3IpWorker::~NsmLocalL3IpWorker ()
    {
    }

    PrismMessage  *NsmLocalL3IpWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode) {
		    case NSMLOCALPHYIPMSG:
                pPrismMessage = new NsmLocalPhyIpMessage();
                break;

		    case NSMLOCALNODEIPMSG:
                pPrismMessage = new NsmLocalNodeIpMessage();
                break;
            case NSMPHYINTFVRFCONF:
                pPrismMessage = new NsmPhyIntfVrfConfigMessage();
                break;
            case NSMVEINTFVRFCONF:
                pPrismMessage = new NsmVeIntfVrfConfigMessage();
                break;

            default:
                break;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *NsmLocalL3IpWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((NsmL3ReadyManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NsmL3ReadyManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((NsmIpAddrManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NsmIpAddrManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((NsmIpConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NsmIpConfigManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((NsmNodePoManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NsmNodePoManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((NsmIntfSviManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NsmIntfSviManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((NsmIpAddrConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NsmIpAddrConfigManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((NsmIpAddrManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NsmIpAddrManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "NsmLocalL3IpWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }
	
////

    void  NsmLocalL3IpWorker::NsmLocalNodeIpMessageHandler(NsmLocalNodeIpMessage *pMessage)
    {
        trace (TRACE_LEVEL_INFO, "NsmLocalL3IpWorker::NsmLocalNodeIpMessageHandler");
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
// Validations and Queries should go here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::validateLocalNodeIpStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::doLocalNodeIpStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::doLocalNodeIpMoStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerCommitTransactionStep),

// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId NsmLocalL3IpWorker::validateLocalNodeIpStep (DceSynchronousLinearSeqContext *pContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        NsmLocalNodeIpMessage *pMessage = dynamic_cast<NsmLocalNodeIpMessage *> (pContext->getPPrismMessage());

        // Only for SVIs doing validation here.
        // For NodePOs validation handled in Global plugin.
        if (VLAN_INT == pMessage->getIfType()) {
            status = validateVlanIntfIp(pContext);
        } else if (IF_TYPE_INTF_LOOPBACK == pMessage->getIfType()) {
            status = validateLoIntfIp(pContext);
        }

        return status;
    }

    ResourceId NsmLocalL3IpWorker::validateVlanIntfIp(DceSynchronousLinearSeqContext *pContext) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        NsmLocalNodeIpMessage *pMessage = dynamic_cast<NsmLocalNodeIpMessage *> (pContext->getPPrismMessage());
        UI32 ifId = pMessage->getIfId();
        string ifName = pMessage->getIfName();
        SI32 mapId = 0;

        string func_name = "NsmLocalL3IpWorker::validateVlanIntfIp() ";
        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
            " IfId: " + ifId +
            " IfType: " + pMessage->getIfType() +
            " OP-CODE: " + pMessage->getOpCode() +
            " CMD-CODE: " + pMessage->getCmdCode() +
            " status: " + pMessage->getStatus() +
            " switchport: " + pMessage->getSwitchport() +
            " secondary: " + pMessage->getSecond() +
            " Ospf-Passive: " + pMessage->getOspfPassive() +
            " Ospf-Ignore: " + pMessage->getOspfIgnore() +
            " IP-ADDR: " + pMessage->getIpv4Address());

        mapId = DcmToolKit :: getLocalMappedId();

        VlanIntfManagedObject *pVlanMo = NsmL3Util :: VlanIdGetVlanIfMO(ifId, mapId, pContext, false);
        pContext->addManagedObjectForGarbageCollection(pVlanMo);
        pContext->getCache()->put(VlanIntfManagedObject::getClassName(), pVlanMo);
        if (pVlanMo == NULL) {
            trace (TRACE_LEVEL_ERROR, func_name + "VlanIntfMO not found! - " + ifName);
            return (WRC_NSM_API_ERR_VLAN_NOT_EXIST);
        }

        if(pVlanMo->getPVlanType() == PVLAN_TYPE_ISOLATED || pVlanMo->getPVlanType() == PVLAN_TYPE_COMMUNITY){
        	 return (WRC_NSM_PVLAN_VLAN_VE);
        }
        if(pVlanMo->getRemoteSpanEnabled()) {
        	NSM_PLUG_DBG_ERR_S(string("The VLAN is a remote-span VLAN. Cannot configure Ve"));
        	return WRC_SPAN_CONFIG_NOT_ALLOWED_ON_RSPAN_VLAN;
        }

        NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
        NsmIntfSviManagedObject *pIntfSviMo = NsmL3Util :: GetIntfSviMO(pVlanMo, mapId, pContext, false);
        if (pIntfSviMo) {
            trace (TRACE_LEVEL_DEBUG, string("NsmLocalL3IpWorker::validateVlanIntfIp. Found NsmIntfSviManagedObject for IntfName: ") + ifName);
            pL3ReadyMo = (WaveManagedObjectPointer<NsmL3ReadyManagedObject>(pIntfSviMo->getL3Ready())).operator->();
            pContext->addManagedObjectForGarbageCollection(pIntfSviMo);
            pContext->getCache()->put(NsmIntfSviManagedObject::getClassName(), pIntfSviMo);

            //If SVI creation message come and SVI already exists as in case of Create-over-create, 
            if ((NSM_MSG_OP_CREATE == pMessage->getOpCode()) && (NSM_MSG_CMD_NODE_VLAN == pMessage->getCmdCode())) {
                trace (TRACE_LEVEL_ERROR, string("NsmLocalL3IpWorker::validateVlanIntfIp. Already created SVI: ") + ifName);
                pMessage->setValidationBackendStepFlag(true);
                return WAVE_MESSAGE_SUCCESS;
            }
        } else {
            //If SVI deletion message come and SVI doesn't exists, return error
            if ((NSM_MSG_OP_DELETE == pMessage->getOpCode()) && (NSM_MSG_CMD_NODE_VLAN == pMessage->getCmdCode())) {
                trace (TRACE_LEVEL_ERROR, string("NsmLocalL3IpWorker::validateVlanIntfIp. Already deleted SVI: ") + ifName);
                return WRC_NSM_API_ERR_SVI_ALREADY_DELETED;
            }
        }
        if (pL3ReadyMo) {
            pL3ReadyMo->setIntfType(IF_TYPE_VL);
        }

        //Skip this step if message has got the flag set
        if (pMessage->getValidationBackendStepFlag()) {
            return WAVE_MESSAGE_SUCCESS;
        }

        switch(pMessage->getCmdCode()) {
            case NSM_MSG_CMD_IP_ADDRESS:
            case NSM_MSG_CMD_ALL_IPADDR:
                if (pL3ReadyMo) {
                    ObjectId vrfObjectId = pIntfSviMo->getVrfObjectId();
                    if (vrfObjectId !=  ObjectId::NullObjectId) { 
                        status = VrfUtils::validateVRFBinding(vrfObjectId);
                        if (status != WAVE_MESSAGE_SUCCESS) {
                            return status;
                        }
                    }
                    status = validateVlanIpAddrConf(pL3ReadyMo, pContext);
                } else {
                    trace (TRACE_LEVEL_DEBUG, func_name + "IntfName: " + ifName + " L3 is NOT ready. IP Addr can't be configured!");
                    status = WRC_NSM_API_ERR_L3_NOT_READY_NO_IPADDR;
                }
                break;

            case NSM_MSG_CMD_MTU:
            case NSM_MSG_CMD_IP_DIRECTED_BROADCAST:
            case NSM_MSG_CMD_PROXY_ARP:
            case NSM_MSG_CMD_ARP_AGING_TIMEOUT:
                if (pL3ReadyMo == NULL) {
                    trace (TRACE_LEVEL_DEBUG, func_name + "IntfName: " + ifName + " L3 is NOT ready. IP attributes can't be configured!");
                    status = WRC_NSM_API_ERR_L3_NOT_READY;
                }
                break;

            case NSM_MSG_CMD_NODE_VLAN:
            case NSM_MSG_CMD_INTF_SHUTDOWN:
                break;

            default:
                trace (TRACE_LEVEL_DEBUG, func_name + "IntfName: " + ifName + " Wrong CmdCode: " + pMessage->getCmdCode());
                status = WAVE_MESSAGE_ERROR;
                break;
        }

        return status;
    }

    ResourceId NsmLocalL3IpWorker::validateVlanIpAddrConf(NsmL3ReadyManagedObject *pL3ReadyMo, DceSynchronousLinearSeqContext *pContext) {
        string func_name = "NsmLocalL3IpWorker::validateVlanIpAddrConf ";
        NsmLocalNodeIpMessage *pMessage = dynamic_cast<NsmLocalNodeIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        bool secondary = pMessage->getSecond();
        string ipAddr = pMessage->getIpv4Address();

        NsmIpAddrConfigManagedObject *pIpAddrCfgMo = NULL;
        if (pL3ReadyMo) {
            pIpAddrCfgMo = (WaveManagedObjectPointer<NsmIpAddrConfigManagedObject>(pL3ReadyMo->getIpAddrConfig())).operator ->();
        }

        if (NSM_MSG_OP_DELETE != pMessage->getOpCode()) {
            if (pIpAddrCfgMo == NULL) {
                if (secondary) {
                    trace (TRACE_LEVEL_DEBUG, func_name + "IfName: " + ifName + " IpAddrConfigMo is not ready. Secondary Ip can't be configured!");
                    return WRC_NSM_API_SET_ERR_CANT_SET_SECONDARY_FIRST;
                }
            } else {
                if (pIpAddrCfgMo->findIpAddrIndex(ipAddr) >= 0) {
                    trace (TRACE_LEVEL_DEBUG, func_name + "IfName: " + ifName + " IpAddr: " + ipAddr + " already exists!");
                    return WRC_NSM_API_ERR_IP_ADDRESS_DUPLICATED;
                }
            }
        } else { // Delete Case
            if (pIpAddrCfgMo == NULL) {
                trace (TRACE_LEVEL_DEBUG, func_name + " IfName: " + ifName + " No IP Address is configured!");
                return WRC_NSM_API_SET_ERR_ADDRESS_NOT_EXIST;
            }

            if (NSM_MSG_CMD_ALL_IPADDR != pMessage->getCmdCode()) {
                if (pIpAddrCfgMo->findIpAddrIndex(ipAddr) < 0) {
                    trace (TRACE_LEVEL_DEBUG, func_name + "IfName: " + ifName + " IpAddr: " + ipAddr + " doesn't exists!");
                    return WRC_NSM_API_SET_ERR_ADDRESS_NOT_EXIST;
                }
                if (pIpAddrCfgMo->isPrimaryIpAddr(ipAddr)) {
                    pMessage->setSecond(false);
                } else {
                    pMessage->setSecond(true);
                }
            }
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId NsmLocalL3IpWorker::doLocalNodeIpStep (DceSynchronousLinearSeqContext *pContext)
    {

        int status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmLocalL3IpWorker::doLocalNodeIpStep() ";
        NsmLocalNodeIpMessage *pMessage =
            dynamic_cast<NsmLocalNodeIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();
        UI32 opCode = pMessage->getOpCode();

        trace (TRACE_LEVEL_DEBUG, func_name + "Entering ...");

        trace (TRACE_LEVEL_DEBUG , func_name + " IntfName: " + ifName +
            " IfId: " + pMessage->getIfId() +
            " IfType: " + pMessage->getIfType() +
            " OP-CODE: " + pMessage->getOpCode() +
            " CMD-CODE: " + pMessage->getCmdCode() +
            " status: " + pMessage->getStatus() +
            " switchport: " + pMessage->getSwitchport() +
            " secondary: " + pMessage->getSecond() +
            " OspfPassive: " + pMessage->getOspfPassive() +
            " OspfIgnore: " + pMessage->getOspfIgnore() +
            " IP-ADDR: " + pMessage->getIpv4Address());


        //Skip this step if message has got the flag set
        if (pMessage->getValidationBackendStepFlag()) {
            return WAVE_MESSAGE_SUCCESS;
        }

//// Send to backend
        auto_ptr<NsmClientCreateIntIpConfigSPMessage> m(
                new NsmClientCreateIntIpConfigSPMessage());

        UI32 opcode = NsmUtils::getClientOpCode(pMessage->getCmdCode(),
            pMessage->getOpCode());

        m->setOpcode(opcode);
        m->setIfId(pMessage->getIfId());
        if(pMessage->getIfType() == IF_TYPE_INTF_LOOPBACK) {
            m->setIfName(ifName);
        } else {
            // For PO and VLAN, don't use ifName directly
            m->setIfName(string("") + pMessage->getIfId());
        }

        if (VLAN_INT == pMessage->getIfType() &&
                NSM_MSG_OP_CREATE == pMessage->getOpCode()) {
            trace (TRACE_LEVEL_DEBUG, func_name +  "Provisioning for vlan " +
                    pMessage->getIfId());
            PROVISION_VLAN(pContext, pMessage->getIfId());
        }
        m->setIntfType(pMessage->getIfType());

        m->setIp_addr_mask(pMessage->getIpv4Address());
        m->setSec(pMessage->getSecond());
        m->setOspfPassive(pMessage->getOspfPassive());
        m->setOspfIgnore(pMessage->getOspfIgnore());

        m->setMtu(pMessage->getMtu());
        m->setStatus(pMessage->getStatus());
        m->setDirectedBroadcast(pMessage->getDirectedBroadcast());
        m->setProxy_arp(pMessage->getProxyArp());
        m->setArp_ageing_timeout(pMessage->getArpAgingTimeout());

        trace (TRACE_LEVEL_INFO, func_name +  "Message to BACKEND" +
            " IntfId: " + m->getIfId() +
            " IntfName: " + m->getIfName() +
            " OP-CODE: " + m->getOpcode() +
            " status: " + m->getStatus() +
            " secondary: " + m->getSec() +
            " Ospf-Passive: " + m->getOspfPassive() +
            " Ospf-Ignore: " + m->getOspfIgnore() +
            " IP-ADDR: " + m->getIp_addr_mask() +
            " MTU: " + m->getMtu() +
            " DirectedBroadcast: " + m->getDirectedBroadcast() +
            " ProxyArp: " + m->getProxy_arp() +
            " ARP-AGING-TIMEOUT: " + m->getArp_ageing_timeout() +
            "");

        /* Send the message to back end daemon */
        if (cmdCode == NSM_MSG_CMD_PROXY_ARP 
            || cmdCode == NSM_MSG_CMD_ARP_AGING_TIMEOUT
            )
        {
            status = sendSynchronouslyToWaveClient ("arp", m.get());
            if (status == WAVE_MESSAGE_SUCCESS) {
                status = m->getCompletionStatus();
                if (status != WAVE_MESSAGE_SUCCESS) {
                    trace (TRACE_LEVEL_ERROR, func_name + " Intf: " + ifName +
                        " CmdCode=" + cmdCode + " OpCode=" + opCode +
                        " (ARP) getCompletionStatus() FAILED" + 
                        " status = " + status);
                }
                else if (m->getClientStatus()) {
                    int err = m->getClientStatus();

                    status = NsmUtils::ArpBackendErrorMapToDcmStatus(err);
                    trace (TRACE_LEVEL_ERROR, func_name + " Intf: " + ifName +
                        " CmdCode=" + cmdCode + " OpCode=" + opCode +
                        " (ARP) getClientStatus() FAILED" +
                        " Arp Error = " + err + " Dcmd Status = " + status);
                }
            }
            pMessage->setCompletionStatus(status);
            trace(TRACE_LEVEL_DEBUG, func_name + " Intf: " + ifName +
                " CmdCode=" + cmdCode + " OpCode=" + opCode +
                " - Message has been sent to ARP client. status=" + status);
        }
        else {
            status = sendSynchronouslyToWaveClient ("nsm", m.get());
            if (status == WAVE_MESSAGE_SUCCESS) {
                status = m->getCompletionStatus();
                if (status != WAVE_MESSAGE_SUCCESS) {
                    trace (TRACE_LEVEL_ERROR, func_name + " Intf: " + ifName +
                        " CmdCode=" + cmdCode + " OpCode=" + opCode +
                        " (NSM) getCompletionStatus() FAILED" + 
                        " status = " + status);
                }
                else if (m->getClientStatus()) {
                    int err = m->getClientStatus();

                    status = NsmUtils::NsmBackendErrorMapToDcmStatus(err);
                    trace (TRACE_LEVEL_ERROR, func_name + " Intf: " + ifName +
                        " CmdCode=" + cmdCode + " OpCode=" + opCode +
                        " (NSM) getClientStatus() FAILED" +
                        " Nsm Error = " + err + " Dcmd Status = " + status);
                }
                HANDLE_PROVISION_RESPONSE(m);
            }
            pMessage->setCompletionStatus(status);
            trace(TRACE_LEVEL_INFO, func_name + " Intf: " + ifName +
                " CmdCode=" + cmdCode + " OpCode=" + opCode +
                " - Message has been sent to NSM client. status=" + status);

            // Send a message to vrrp backend now       
            if ((status == WAVE_MESSAGE_SUCCESS) &&
                ((opcode == NSM_MSG_NO_SEC_IP_ADDRESS)
                     || (opcode == NSM_MSG_NO_SEC_IP_ADDRESS_ALL)))
            {   
                int status_vrrp;
                VRRPClientIntfConfigSPMessage *m_vrrp = new VRRPClientIntfConfigSPMessage();
                UI32 ifType = IF_TYPE_INVALID;

                if (strncasecmp(ifName.c_str(), "vl", 2) == 0) {
                    ifType = IF_TYPE_VL;
                } else if (strncasecmp(ifName.c_str(), "po", 2) == 0) {
                    ifType = IF_TYPE_PO;
                }
                m_vrrp->setIfType(ifType);

                size_t tmp_sep = ifName.find('-');
                string intf_id(ifName,tmp_sep+1);
                m_vrrp->setIfName(intf_id);

                string ipaddr = pMessage->getIpv4Address();
                size_t sep = ipaddr.find('/');
                string ip_str(ipaddr, 0, sep);
                string len_str(ipaddr, sep+1);
                UI32 ip = ntohl(inet_addr(ip_str.c_str()));
                UI32 prefix_len = atoi(len_str.c_str());
                m_vrrp->setIntf_ip_addr(ip);
                m_vrrp->setIntf_ip_mask(prefix_len);

                m_vrrp->setChoice(pMessage->getSecond() ? NO_INTF_IP_SEC : NO_INTF_IP_REAL);
                if ( pMessage->getCmdCode() == NSM_MSG_CMD_ALL_IPADDR ) {
                    m_vrrp->setChoice(NO_INTF_IP_ALL);
                }
                m_vrrp->setVrrp_type(VRRP_TYPE_NONE);
                m_vrrp->setVrid(0);

                status_vrrp = sendSynchronouslyToWaveClient ("vrrp", m_vrrp);
                trace(TRACE_LEVEL_DEBUG, func_name + " Intf: " + ifName +
                        " IfType: " + ifType + " Ipv4Address: " +
                        pMessage->getIpv4Address() +
                        " - Message has been sent to VRRP client. status = "
                        + status_vrrp);
                m->setCompletionStatus(status_vrrp);

                if(WAVE_MESSAGE_SUCCESS == status_vrrp)
                {
                    if(WAVE_MESSAGE_SUCCESS != m_vrrp->getCompletionStatus())
                    {   
                        status_vrrp = m_vrrp->getCompletionStatus();
                    }
                    else if(m_vrrp->getClientStatus())
                    {   
                        int err = m_vrrp->getClientStatus();
                        status_vrrp = NsmUtils::NsmBackendErrorMapToDcmStatus(err);
                        m_vrrp->setCompletionStatus(status_vrrp);
                        trace(TRACE_LEVEL_ERROR, string("NsmLocalL3IpWorker::doLocalNodeIpStep::errorCode: Error : ") + status_vrrp);
                        trace(TRACE_LEVEL_ERROR, string("NsmLocalL3IpWorker::doLocalNodeIpStep::errorCode: Status : ") + FrameworkToolKit::localize(status_vrrp));
                    }
                }
            delete (m_vrrp);
            }
        }

        return status;
    }

    ResourceId NsmLocalL3IpWorker::doLocalNodeIpMoStep (DceSynchronousLinearSeqContext *pContext)
    {
        string func_name = "NsmLocalL3IpWorker::doLocalNodeIpMoStep() ";
        NsmLocalNodeIpMessage *pMessage = dynamic_cast<NsmLocalNodeIpMessage *> (pContext->getPPrismMessage());

        // As of now we are handling SVIs and Loopback interfaces only in this step
        // For NodePOs, skip this step. NodePOs are taken care in Global plugin
        if (VLAN_INT == pMessage->getIfType()) {
            return doOverallSviMoStep(pContext);
        } else if (IF_TYPE_INTF_LOOPBACK == pMessage->getIfType()) {
            return loopbackIntfMOStep(pContext);
        } else {
            return WAVE_MESSAGE_SUCCESS;
        }
        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId NsmLocalL3IpWorker::doOverallSviMoStep (DceSynchronousLinearSeqContext *pContext) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmLocalL3IpWorker::doOverallSviMoStep() ";
        NsmLocalNodeIpMessage *pMessage = dynamic_cast<NsmLocalNodeIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();

        NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
        NsmIntfSviManagedObject *pIntfSviMo = dynamic_cast<NsmIntfSviManagedObject *>(pContext->getCache()->getWaveManagedObject(NsmIntfSviManagedObject::getClassName()));
        if (pIntfSviMo) {
            pL3ReadyMo = (WaveManagedObjectPointer<NsmL3ReadyManagedObject>(pIntfSviMo->getL3Ready())).operator->();
        }
        if (pL3ReadyMo) {
            pL3ReadyMo->setIntfType(IF_TYPE_VL);
        }

        switch(pMessage->getCmdCode()) {
            case NSM_MSG_CMD_NODE_VLAN:
                status = doSviMo(pContext);
                break;
            case NSM_MSG_CMD_INTF_SHUTDOWN:
                if (pIntfSviMo == NULL) {
                    status = WAVE_MESSAGE_ERROR;
                } else {
                    pIntfSviMo->setStatus(pMessage->getStatus());
                    updateWaveManagedObject(pIntfSviMo);
                    trace (TRACE_LEVEL_DEBUG, func_name + " IfName: " + ifName + "shutdown = " + pMessage->getStatus());
                    status = WAVE_MESSAGE_SUCCESS;
                }
                break;
            case NSM_MSG_CMD_IP_ADDRESS:
            case NSM_MSG_CMD_ALL_IPADDR:
                if (pL3ReadyMo != NULL) {
                    status = doSviIpAddrMo(pL3ReadyMo, pContext);
                } else {
                    trace (TRACE_LEVEL_INFO, func_name + "IfName: " + ifName + " L3 is not ready. IP Addr can't be configured!");
                }
                break;
            default:
                if (NSM_MSG_OP_DELETE == pMessage->getOpCode()) {
                    if (pL3ReadyMo != NULL) {
                        status = deleteSviIpConfigMo(pL3ReadyMo, pContext);
                    } else {
                        trace (TRACE_LEVEL_INFO, func_name + "IfName: " + ifName + " L3 is not ready. Delete operation can't be performed!");
                        status = WAVE_MESSAGE_ERROR;
                    }
                } else {
                    if (pL3ReadyMo != NULL) {
                        status = updateSviIpConfigMo(pL3ReadyMo, pContext);
                    } else {
                        trace (TRACE_LEVEL_INFO, func_name + "IfName: " + ifName + " L3 is not ready. Configuration can't be performed!");
                    }
                }
                break;
        }

        return status;
    }

    ResourceId NsmLocalL3IpWorker::doSviMo(DceSynchronousLinearSeqContext *pContext) {
        string func_name = "NsmLocalL3IpWorker::doSviMo ";
        trace (TRACE_LEVEL_DEBUG, func_name + "Entering ...");
        NsmLocalNodeIpMessage *pMessage = dynamic_cast<NsmLocalNodeIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();

        VlanIntfManagedObject *pVlanMo = dynamic_cast<VlanIntfManagedObject *>(pContext->getCache()->getWaveManagedObject(VlanIntfManagedObject::getClassName()));
        NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
        NsmIntfSviManagedObject *pIntfSviMo = dynamic_cast<NsmIntfSviManagedObject *>(pContext->getCache()->getWaveManagedObject(NsmIntfSviManagedObject::getClassName()));
        if (pIntfSviMo) {
            pL3ReadyMo = (WaveManagedObjectPointer<NsmL3ReadyManagedObject>(pIntfSviMo->getL3Ready())).operator->();
        }

        if (NSM_MSG_OP_DELETE != pMessage->getOpCode()) {
            if (pIntfSviMo == NULL) { // Create SVI case
                pIntfSviMo = new NsmIntfSviManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                pContext->addManagedObjectForGarbageCollection(pIntfSviMo);
                pIntfSviMo->setIfId(pMessage->getIfId());
                pIntfSviMo->setVlanId(pVlanMo->getObjectId());

                pL3ReadyMo = new NsmL3ReadyManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                pL3ReadyMo->setInterfaceName(getString(pMessage->getIfId()));
                pL3ReadyMo->setIntfType(IF_TYPE_VL);
                pIntfSviMo->setL3Ready(pL3ReadyMo);
                trace (TRACE_LEVEL_DEBUG, func_name + " IfName: " + ifName + " Now L3 is enabled");
                trace (TRACE_LEVEL_DEBUG, func_name + " IfName: " + ifName + " Now SVI is enabled");
            } else {
                trace (TRACE_LEVEL_DEBUG, func_name + " IfName: " + ifName + " SVI is already enabled");
            }
        } else { // Delete SVI case
            if (pIntfSviMo != NULL) {
                /*if (pL3ReadyMo) {
                    deleteWaveManagedObject(pL3ReadyMo->getObjectId());
                }*/
                deleteWaveManagedObject(pIntfSviMo->getObjectId());
                trace (TRACE_LEVEL_DEBUG, func_name + "IfName: " + ifName + " SVI is disabled");
            } else {
                trace (TRACE_LEVEL_DEBUG, func_name + "IfName: " + ifName + " SVI is already disabled");
            }
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId NsmLocalL3IpWorker::doSviIpAddrMo(NsmL3ReadyManagedObject *pL3ReadyMo, DceSynchronousLinearSeqContext *pContext) {
        string func_name = "NsmLocalL3IpWorker::doSviIpAddrMo ";
        NsmLocalNodeIpMessage *pMessage = dynamic_cast<NsmLocalNodeIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        bool secondary = pMessage->getSecond();
        bool ospfPassive = pMessage->getOspfPassive();
        bool ospfIgnore = pMessage->getOspfIgnore();
        string ipAddr = pMessage->getIpv4Address();

        NsmIpAddrConfigManagedObject *pIpAddrCfgMo = NULL;
        NsmIpAddrManagedObject *pIpAddrMo = NULL;

        if (pL3ReadyMo) {
            pIpAddrCfgMo = (WaveManagedObjectPointer<NsmIpAddrConfigManagedObject>(pL3ReadyMo->getIpAddrConfig())).operator ->();
        }

        if (NSM_MSG_OP_DELETE != pMessage->getOpCode()) {
            if (pIpAddrCfgMo == NULL) { // Create Case
                if (secondary) {
                    trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " IpAddriConfig MO  is NOT ready. Secondary Ip-Addr can't be configured!");
                    return WAVE_MESSAGE_SUCCESS;
                }
                pIpAddrCfgMo = new NsmIpAddrConfigManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));

                // Enable proxy-arp on the 1st ip-address
                NsmIpConfigManagedObject *pIpConfigMo = (WaveManagedObjectPointer<NsmIpConfigManagedObject>(pL3ReadyMo->getIpConfig())).operator->();

                if (pIpConfigMo == NULL) {
                    pIpConfigMo = new NsmIpConfigManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                    pL3ReadyMo->setIpConfig(pIpConfigMo);
                    pL3ReadyMo->addAttributeToBeUpdated("ipConfig");
                    updateWaveManagedObject(pL3ReadyMo);
                }
                pIpConfigMo->setProxyArp(true);
                pIpConfigMo->addAttributeToBeUpdated("proxy_arp");
                updateWaveManagedObject(pIpConfigMo);
            } else {
                if (pIpAddrCfgMo->findIpAddrIndex(ipAddr) >= 0) {
                    trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " IpAddr: " + ipAddr + " exists!");
                    return WAVE_MESSAGE_SUCCESS;
                }
            }
            if (secondary) {
                pIpAddrMo = new NsmIpAddrManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                pIpAddrMo->setIpv4AddrMask(pMessage->getIpv4Address());
                pIpAddrMo->setSecond(true);
                pIpAddrMo->setOspfPassive(false);
                pIpAddrMo->setOspfIgnore(false);
                pIpAddrCfgMo->addIpAddrMO(pIpAddrMo);
                pIpAddrCfgMo->addAttributeToBeUpdated("ipAddr");
                updateWaveManagedObject(pIpAddrCfgMo);
                trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " Secondary IP-ADDRESS is added!");
            } else {
                pIpAddrMo = new NsmIpAddrManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                pIpAddrMo->setIpv4AddrMask(pMessage->getIpv4Address());
                pIpAddrMo->setSecond(false);
                if (ospfPassive) { 
                    pIpAddrMo->setOspfPassive(true);
                    pIpAddrMo->setOspfIgnore(false);
                }
                else if (ospfIgnore) {
                    pIpAddrMo->setOspfPassive(false);
                    pIpAddrMo->setOspfIgnore(true);
                }
                else {
                    pIpAddrMo->setOspfPassive(false);
                    pIpAddrMo->setOspfIgnore(false);
                }
                if (NULL == pIpAddrCfgMo->getPrimaryIpAddrMO()) {
                	pIpAddrCfgMo->addIpAddrMO(pIpAddrMo);
                    pL3ReadyMo->setIpAddrConfig(pIpAddrCfgMo);
                    pL3ReadyMo->addAttributeToBeUpdated("ipAddrCfg");
                    updateWaveManagedObject(pL3ReadyMo);
                    trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " Primary IP-ADDRESS is added!");
                } else {
                	pIpAddrCfgMo->addIpAddrMO(pIpAddrMo);
                	pIpAddrCfgMo->addAttributeToBeUpdated("ipAddr");
                    updateWaveManagedObject(pIpAddrCfgMo);
                    trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " New Primary IP-ADDRESS is Added!");
                }
            }
        } else { // Delete Case
            if (NSM_MSG_CMD_ALL_IPADDR == pMessage->getCmdCode()) {
                notifyVRRPIPDeletion(pL3ReadyMo, ipAddr, true);
                pL3ReadyMo->setIpAddrConfig(NULL);
                pL3ReadyMo->addAttributeToBeUpdated("ipAddrCfg");
                updateWaveManagedObject(pL3ReadyMo);
                trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " All Ip-Addr are deleted!");
                return WAVE_MESSAGE_SUCCESS;
            }
            if (pIpAddrCfgMo->findIpAddrIndex(ipAddr) < 0) {
                trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " +  ifName + " IpAddr: " + ipAddr + " doesn't exist!");
                return WAVE_MESSAGE_SUCCESS;
            }
            if (pIpAddrCfgMo->isPrimaryIpAddr(ipAddr)) {
                notifyVRRPIPDeletion(pL3ReadyMo, ipAddr, false);
                pIpAddrCfgMo->deleteIpAddr(ipAddr);
                if (NULL == pIpAddrCfgMo->getPrimaryIpAddrMO()) {
                    pL3ReadyMo->setIpAddrConfig(NULL);
                	pL3ReadyMo->addAttributeToBeUpdated("ipAddrCfg");
                    updateWaveManagedObject(pL3ReadyMo);
                } else {
                    updateWaveManagedObject(pIpAddrCfgMo);
                }
                trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " Delete primary ip-address.");
            } else {
            	pIpAddrCfgMo->deleteIpAddr(ipAddr);
            	pIpAddrCfgMo->addAttributeToBeUpdated("ipAddr");
                updateWaveManagedObject(pIpAddrCfgMo);
                trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " Delete secondary ip-address.");
            }
        }

        trace (TRACE_LEVEL_DEBUG, func_name + "IfName: " + ifName + " MO Update done.");

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId NsmLocalL3IpWorker::deleteSviIpConfigMo(NsmL3ReadyManagedObject *pL3ReadyMo, DceSynchronousLinearSeqContext *pContext) {
        string func_name = "NsmLocalL3IpWorker::deleteSviIpConfigMo ";
        NsmLocalNodeIpMessage *pMessage = dynamic_cast<NsmLocalNodeIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();

        NsmIpConfigManagedObject *pIpConfigMo = NULL;

        if (pL3ReadyMo) {
            pIpConfigMo = (WaveManagedObjectPointer<NsmIpConfigManagedObject>(pL3ReadyMo->getIpConfig())).operator ->();
        }

        if (pIpConfigMo == NULL) {
            trace (TRACE_LEVEL_ERROR, func_name + " IntfName: " + ifName + " NSMIpConfigMO is NULL!");
            return WAVE_MESSAGE_SUCCESS;
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
                pIpConfigMo->setArpAgingTimeout(NSM_IP_ARP_AGING_TIMEOUT_DEF_VAL);
                updateWaveManagedObject(pIpConfigMo);
                break;

            default:
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " Wrong CmdCode: " + cmdCode);
                break;
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId NsmLocalL3IpWorker::updateSviIpConfigMo(NsmL3ReadyManagedObject *pL3ReadyMo, DceSynchronousLinearSeqContext *pContext) {
        string func_name = "NsmLocalL3IpWorker::updateSviIpConfigMo ";
        NsmLocalNodeIpMessage *pMessage = dynamic_cast<NsmLocalNodeIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();

        NsmIpConfigManagedObject *pIpConfigMo = NULL;

        if (pL3ReadyMo) {
            pIpConfigMo = (WaveManagedObjectPointer<NsmIpConfigManagedObject>(pL3ReadyMo->getIpConfig())).operator ->();
            if (pIpConfigMo == NULL) {
                pIpConfigMo = new NsmIpConfigManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                pL3ReadyMo->setIpConfig(pIpConfigMo);
                updateWaveManagedObject(pL3ReadyMo);
            }
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
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " Wrong CmdCode: " + cmdCode);
                break;
        }

        return WAVE_MESSAGE_SUCCESS;
    }

////

    void  NsmLocalL3IpWorker::NsmLocalPhyIpMessageHandler(NsmLocalPhyIpMessage *pMessage)
    {
        trace (TRACE_LEVEL_INFO, "NsmLocalL3IpWorker::NsmLocalPhyIpMessageHandler");
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
// Validations and Queries should go here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::validatePhyIntfIpStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::doPhyIntfIpStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::doPhyIntfIpMoStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerCommitTransactionStep),

// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerFailedStep)
        };
        DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pDceSynchronousLinearSeqContext->execute ();
    }

    ResourceId NsmLocalL3IpWorker::doPhyIntfIpStep (DceSynchronousLinearSeqContext *pContext)
    {
        int status = WAVE_MESSAGE_SUCCESS;

        string func_name = "NsmLocalL3IpWorker::doPhyIntfIpStep() ";
        NsmLocalPhyIpMessage *pMessage =
            dynamic_cast<NsmLocalPhyIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();
        UI32 opCode = pMessage->getOpCode();
        UI32 ifType = pMessage->getIfType();

        trace (TRACE_LEVEL_DEBUG, func_name + "Entering ...");
        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
            " IfId: " + pMessage->getIfId() +
            " IfType: " + pMessage->getIfType() +
            " OP-CODE: " + pMessage->getOpCode() +
            " CMD-CODE: " + pMessage->getCmdCode() +
            " switchport: " + pMessage->getSwitchport() +
            " secondary: " + pMessage->getSecond() +
            " OspfPassive: " + pMessage->getOspfPassive() +
            " OspfIgnore: " + pMessage->getOspfIgnore() +
            " IP-ADDR: " + pMessage->getIpv4Address());

        NsmIpConfigManagedObject *pIpConfigMo = NsmL3Util::PhyIfNameGetIpConfigMO(ifName, pContext);
		if(pIpConfigMo)
            pContext->addManagedObjectForGarbageCollection(pIpConfigMo);
        if (opCode == NSM_MSG_OP_DELETE && pIpConfigMo == NULL) {
            // Don't send to backend.
            return WAVE_MESSAGE_SUCCESS;
        }
            
//// Send to backend
        NsmClientCreateIntIpConfigSPMessage *m = new NsmClientCreateIntIpConfigSPMessage();
    
        UI32 opcode = NsmUtils::getClientOpCode(pMessage->getCmdCode(),
            pMessage->getOpCode());
        m ->setOpcode(opcode);

        // For PhyIntf, use ifName directly
        // Backend don't recognize 3-tuple ifName
        if (WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()) {
            NsmUtils::getTwoTupleIfName(pMessage->getIfName(), ifName);
        }
        m->setIfName(ifName);

        // Since we started to support mutiple-speed interface: gi/te/fo,
        // interface type was retrieved from MO in validation step.
        m->setIntfType(ifType);

        m->setIp_addr_mask(pMessage->getIpv4Address());
        m->setSec(pMessage->getSecond());
		m->setOspfPassive(pMessage->getOspfPassive());
		m->setOspfIgnore(pMessage->getOspfIgnore());

        m->setMtu(pMessage->getMtu());
        m->setDirectedBroadcast(pMessage->getDirectedBroadcast());
        m->setProxy_arp(pMessage->getProxyArp());
        m->setArp_ageing_timeout(pMessage->getArpAgingTimeout());

        trace (TRACE_LEVEL_INFO, func_name +  "Message to BACKEND" +
            " IntfName: " + m->getIfName() +
            " OP-CODE: " + m->getOpcode() +
            " secondary: " + m->getSec() +
			" OspfPassive: " + m->getOspfPassive() +
			" OspfIgnore: " + m->getOspfIgnore() +
            " IP-ADDR: " + m->getIp_addr_mask() +
            " MTU: " + m->getMtu() +
            " DirectedBroadcast: " + m->getDirectedBroadcast() +
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
                    trace (TRACE_LEVEL_ERROR, func_name +
                        " Intf: " + ifName + " IfType: " + ifType +
                        " CmdCode=" + cmdCode + " OpCode=" + opCode +
                        " (ARP) getCompletionStatus() FAILED" + 
                        " status = " + status);
                }
                else if (m->getClientStatus()) {
                    int err = m->getClientStatus();

                    status = NsmUtils::ArpBackendErrorMapToDcmStatus(err);
                    trace (TRACE_LEVEL_ERROR, func_name +
                        " Intf: " + ifName + " IfType: " + ifType +
                        " CmdCode=" + cmdCode + " OpCode=" + opCode +
                        " (ARP) getClientStatus() FAILED" +
                        " Arp Error = " + err + " Dcmd Status = " + status);
                }
            }
            pMessage->setCompletionStatus(status);
            trace(TRACE_LEVEL_DEBUG, func_name +
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

            // Send a message to vrrp backend now       
            if ((status == WAVE_MESSAGE_SUCCESS) &&
                ((opcode == NSM_MSG_NO_SEC_IP_ADDRESS)
                     || (opcode == NSM_MSG_NO_SEC_IP_ADDRESS_ALL)))
            {   
                int status_vrrp;
                VRRPClientIntfConfigSPMessage *m_vrrp = new VRRPClientIntfConfigSPMessage();

                if (WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()) {
                    NsmUtils::getTwoTupleIfName(pMessage->getIfName(), ifName);
                }

                m_vrrp->setIfName(ifName);
                m_vrrp->setIfType(ifType);

                string ipaddr = pMessage->getIpv4Address();
                size_t sep = ipaddr.find('/');
                string ip_str(ipaddr, 0, sep);
                string len_str(ipaddr, sep+1);
                UI32 ip = ntohl(inet_addr(ip_str.c_str()));
                UI32 prefix_len = atoi(len_str.c_str());
                m_vrrp->setIntf_ip_addr(ip);
                m_vrrp->setIntf_ip_mask(prefix_len);

                m_vrrp->setChoice(pMessage->getSecond() ? NO_INTF_IP_SEC : NO_INTF_IP_REAL);
                m_vrrp->setVrrp_type(VRRP_TYPE_NONE);
                m_vrrp->setVrid(0);

                status_vrrp = sendSynchronouslyToWaveClient ("vrrp", m_vrrp);
                trace(TRACE_LEVEL_INFO, func_name + " Intf: " + ifName +
                        " IfType: " + ifType + " Ipv4Address: " +
                        pMessage->getIpv4Address() +
                        " - Message has been sent to VRRP client. status = "
                        + status_vrrp);
                m->setCompletionStatus(status_vrrp);

                if(WAVE_MESSAGE_SUCCESS == status_vrrp)
                {
                    if(WAVE_MESSAGE_SUCCESS != m_vrrp->getCompletionStatus())
                    {   
                        status_vrrp = m_vrrp->getCompletionStatus();
                    }
                    else if(m_vrrp->getClientStatus())
                    {   
                        int err = m_vrrp->getClientStatus();
                        status_vrrp = NsmUtils::NsmBackendErrorMapToDcmStatus(err);
                        m_vrrp->setCompletionStatus(status_vrrp);
                        trace(TRACE_LEVEL_ERROR, string("NsmLocalL3IpWorker::doPhyIntfIpStep::errorCode: Error : ") + status_vrrp);
                        trace(TRACE_LEVEL_ERROR, string("NsmLocalL3IpWorker::doPhyIntfIpStep::errorCode: Status : ") + FrameworkToolKit::localize(status_vrrp));
                    }
                }
            delete (m_vrrp);
            }
        }
        delete m;

        if (status == WAVE_MESSAGE_SUCCESS && 
            opcode == NSM_MSG_SEC_IP_ADDRESS) 
        { // Add ip-address
		  NsmIpAddrConfigManagedObject *pIpCfgMo = NULL;
		  NsmL3ReadyManagedObject *pL3ReadyMo = dynamic_cast<NsmL3ReadyManagedObject *> (pContext->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));
          pIpCfgMo = (WaveManagedObjectPointer<NsmIpAddrConfigManagedObject> (pL3ReadyMo->getIpAddrConfig())).operator->();	
            if (pIpCfgMo) { // Not the 1st ip-address
                return status;
            }

            // Send another message to enable proxy-arp on the 1st ip-address
            NsmClientCreateIntIpConfigSPMessage *m1 = 
                new NsmClientCreateIntIpConfigSPMessage();
    
            m1->setOpcode(NSM_MSG_IP_PROXY_ARP);
            m1->setIfName(ifName);
            m1->setIntfType(ifType);
            m1->setProxy_arp(true);

            trace (TRACE_LEVEL_INFO, func_name +  
                " Another Proxy-Arp Message to arpd" +
                " IntfName: " + m1->getIfName() +
                " IntfType: " + m1->getIntfType() +
                " OP-CODE: " + m1->getOpcode() +
                " ProxyArp: " + m1->getProxy_arp() +
                "");

            /* Send the message to back end daemon */
            status = sendSynchronouslyToWaveClient ("arp", m1);
            if (status == WAVE_MESSAGE_SUCCESS) {
                status = m1->getCompletionStatus();
                if (status != WAVE_MESSAGE_SUCCESS) {
                    trace (TRACE_LEVEL_INFO, func_name + 
                        " Intf: " + ifName + " IfType: " + ifType +
                        " CmdCode=" + cmdCode + " OpCode=" + opCode +
                        " (ARP) getCompletionStatus() FAILED" + 
                        " status = " + status);
                }
                else if (m1->getClientStatus()) {
                    int err = m1->getClientStatus();

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

            delete m1;
        }

        return  status;
    }

    ResourceId NsmLocalL3IpWorker::doPhyIntfIpAddrMoStep(DceSynchronousLinearSeqContext *pContext)
    {
        string func_name = "NsmLocalL3IpWorker::doPhyIntfIpAddrMoStep() ";
        NsmLocalPhyIpMessage *pMessage =
            dynamic_cast<NsmLocalPhyIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        bool second = pMessage->getSecond();
        bool ospfPassive = pMessage->getOspfPassive();
        bool ospfIgnore = pMessage->getOspfIgnore();
        string ipaddr = pMessage->getIpv4Address();

        trace (TRACE_LEVEL_INFO, "Inside " + func_name + " IntfName: " + ifName);
		NsmL3ReadyManagedObject *pL3ReadyMo = dynamic_cast<NsmL3ReadyManagedObject *> (pContext->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));

/* has been checked 
        if (pL3ReadyMo == NULL) {
            trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + 
                " L3 is NOT ready. Ip-Addr can't be configured!");
            return WAVE_MESSAGE_SUCCESS;
        }
*/
        NsmIpAddrConfigManagedObject *pIpCfgMo = NULL;
        NsmIpAddrManagedObject *pIpAddrMo;
		pIpCfgMo = (WaveManagedObjectPointer<NsmIpAddrConfigManagedObject> (pL3ReadyMo->getIpAddrConfig())).operator->();

        if (pMessage->getOpCode() != NSM_MSG_OP_DELETE) {
            if (pIpCfgMo == NULL) {
                if (second) {
                    trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + 
                        " IpAddriConfig MO  is NOT ready. "
                        "Secondary Ip-Addr can't be configured!");
                    return WAVE_MESSAGE_SUCCESS;
                }
                pIpCfgMo = new NsmIpAddrConfigManagedObject(
                    dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));

                // Enable proxy-arp on the 1st ip-address
                NsmIpConfigManagedObject *pIpConfigMo = 
                    (WaveManagedObjectPointer<NsmIpConfigManagedObject>
                    (pL3ReadyMo->getIpConfig())).operator->();

                if (!pIpConfigMo) {
                    pIpConfigMo = new NsmIpConfigManagedObject(
                        dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
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
            	        dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
            	pIpAddrMo->setIpv4AddrMask(pMessage->getIpv4Address());
            	pIpAddrMo->setSecond(true);
                pIpAddrMo->setOspfPassive(false);
                pIpAddrMo->setOspfIgnore(false);
            	pIpCfgMo->addIpAddrMO(pIpAddrMo);
            	pIpCfgMo->addAttributeToBeUpdated("ipAddr");
            	updateWaveManagedObject(pIpCfgMo);

            	trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName +
            	    " Secondary IP-ADDRESS is added!");
            }
            else {
                pIpAddrMo = new NsmIpAddrManagedObject(
                	dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                pIpAddrMo->setIpv4AddrMask(pMessage->getIpv4Address());
                pIpAddrMo->setSecond(false);
                if (ospfPassive) {
                    pIpAddrMo->setOspfPassive(true);
                    pIpAddrMo->setOspfIgnore(false);	
                }	
                else if (ospfIgnore) {
                    pIpAddrMo->setOspfPassive(false);
                    pIpAddrMo->setOspfIgnore(true);	
                }	
                else {
                    pIpAddrMo->setOspfPassive(false);
                    pIpAddrMo->setOspfIgnore(false);	
                }

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
            if (pMessage->getCmdCode() == NSM_MSG_CMD_ALL_IPADDR) {
            	notifyVRRPIPDeletion(pL3ReadyMo, ipaddr, true, false);
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
            	notifyVRRPIPDeletion(pL3ReadyMo, ipaddr, false, false);
                pIpCfgMo->deleteIpAddr(ipaddr);
                pIpCfgMo->addAttributeToBeUpdated("ipAddr");
                if (NULL == pIpCfgMo->getPrimaryIpAddrMO()) {
                	pL3ReadyMo->setIpAddrConfig(NULL);
                	pL3ReadyMo->addAttributeToBeUpdated("ipAddrCfg");
                	updateWaveManagedObject(pL3ReadyMo);
                }
                else {
                	updateWaveManagedObject(pIpCfgMo);
                }
                trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + 
                    ifName + " Delete primary ip-address.");
            }
            else {
            	notifyVRRPIPDeletion(pL3ReadyMo, ipaddr, false, true);
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

    ResourceId NsmLocalL3IpWorker::doPhyIntfIpMoStep(DceSynchronousLinearSeqContext *pContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmLocalL3IpWorker::doPhyIntfIpMoStep() ";
        NsmLocalPhyIpMessage *pMessage =
        dynamic_cast<NsmLocalPhyIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        UI32 opCode = pMessage->getOpCode();

        trace (TRACE_LEVEL_INFO, "Inside " + func_name + " IntfName: " + ifName);

        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + 
            " OP-CODE: " + pMessage->getOpCode() +
            " CMD-CODE: " + pMessage->getCmdCode() +
            " switchport: " + pMessage->getSwitchport() +
            " secondary: " + pMessage->getSecond() +
            " OspfPassive: " + pMessage->getOspfPassive() +
            " OspfIgnore: " + pMessage->getOspfIgnore() +
            " IP-ADDR: " + pMessage->getIpv4Address() +
            " MTU: " + pMessage->getMtu() +
            " DirectedBroadcast: " + pMessage->getDirectedBroadcast() +
            " ProxyArp: " + pMessage->getProxyArp() +
            " ARP-AGING-TIMEOUT: " + pMessage->getArpAgingTimeout() +
            "");

		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *> (pContext->getCache()->getWaveManagedObject(PhyIntfLocalManagedObject::getClassName()));
        if (pPhyIntfLocalManagedObject == NULL) {
            trace (TRACE_LEVEL_ERROR, func_name + "PhyIntfLocalMO not found!");
            return (WAVE_MESSAGE_ERROR);
        }
        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " Found PhyIntfLocalMO!");

        NsmL3Util::PhyIfNameCheckL3Ready(ifName, pContext);
        NsmL3Util::PhyIfNameCheckIpAddrReady(ifName, pContext);

		NsmL3ReadyManagedObject *pL3ReadyMo = dynamic_cast<NsmL3ReadyManagedObject *> (pContext->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));
        NsmIpConfigManagedObject *pIpConfigMo = NsmL3Util::PhyIfNameGetIpConfigMO(ifName, pContext);
        if (pIpConfigMo) {
            pContext->addManagedObjectForGarbageCollection(pIpConfigMo);
        }

        switch(pMessage->getCmdCode()) {
            case NSM_MSG_CMD_SWITCHPORT:
            {
                if (pMessage->getSwitchport()) {
                    if (pL3ReadyMo != NULL) {
                        // TO-DO
                        pPhyIntfLocalManagedObject->setL3Ready(NULL);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("layer3Ready");
                        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " Now L3 is disabled!");
                        updateWaveManagedObject(pPhyIntfLocalManagedObject);
                    }
                    else {
                        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " L3 is aleady disabled!");
                    }
                }
                else {
                    if (pL3ReadyMo == NULL) {
                        pL3ReadyMo = new NsmL3ReadyManagedObject(
                            dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                        pL3ReadyMo->setInterfaceName(ifName);
                        pL3ReadyMo->setIntfType(IF_TYPE_PHY);

                        pPhyIntfLocalManagedObject->setL3Ready(pL3ReadyMo);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("layer3Ready");
                        updateWaveManagedObject(pPhyIntfLocalManagedObject);
                        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " Now L3 is enabled!");
                    }
                    else {
                        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " L3 is aleady enabled!");
                    }
               }
               break;
            }
            case NSM_MSG_CMD_L3READY_MO:
            {
                if((opCode == NSM_MSG_OP_DELETE) && (pL3ReadyMo != NULL))
                {
                    pPhyIntfLocalManagedObject->setL3Ready(NULL);
                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("layer3Ready");
                    trace (TRACE_LEVEL_INFO, func_name + "NSM_MSG_CMD_L3READY_MO " + " IntfName: " + ifName + " Now L3 is disabled!");
                    updateWaveManagedObject(pPhyIntfLocalManagedObject);
                }    

                else if ((opCode == NSM_MSG_OP_CREATE) && (pL3ReadyMo == NULL))
                {
                    pL3ReadyMo = new NsmL3ReadyManagedObject(
                        dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                    pL3ReadyMo->setInterfaceName(ifName);
                    pL3ReadyMo->setIntfType(IF_TYPE_PHY);
                    pPhyIntfLocalManagedObject->setL3Ready(pL3ReadyMo);
                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("layer3Ready");
                    updateWaveManagedObject(pPhyIntfLocalManagedObject);
                    trace (TRACE_LEVEL_INFO, func_name + "NSM_MSG_CMD_L3READY_MO " + " IntfName: " + ifName + " Now L3 is enabled!");
                }

                break;
            }    
            case NSM_MSG_CMD_IP_ADDRESS: 
            case NSM_MSG_CMD_ALL_IPADDR: 
                if (pL3ReadyMo != NULL) {
                    status = doPhyIntfIpAddrMoStep(pContext);
                }
                else {
                    trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + 
                        " L3 is NOT ready. Ip-Addr can't be configured!");
                }
                break;

            default:
            {
                if (opCode == NSM_MSG_OP_DELETE) {
                    status = deletePhyIntfIpConfigMoStep(pContext);
                }
                else {
                    status = updatePhyIntfIpConfigMoStep(pContext);
                }

                break;
            }
        }

        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + " Update MO done!");

        return status;
    }

    ResourceId NsmLocalL3IpWorker::updatePhyIntfIpConfigMoStep(DceSynchronousLinearSeqContext *pContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmLocalL3IpWorker::updatePhyIntfIpConfigMoStep() ";
        NsmLocalPhyIpMessage *pMessage =
        dynamic_cast<NsmLocalPhyIpMessage *> (pContext->getPPrismMessage());
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
		NsmIpConfigManagedObject *pIpConfigMo = NULL;
		NsmL3ReadyManagedObject *pL3ReadyMo = dynamic_cast<NsmL3ReadyManagedObject *> (pContext->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));
		pIpConfigMo = (WaveManagedObjectPointer<NsmIpConfigManagedObject> (pL3ReadyMo->getIpConfig())).operator->();

        if(!pIpConfigMo) {
           pIpConfigMo = new NsmIpConfigManagedObject(
               dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
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

    ResourceId NsmLocalL3IpWorker::deletePhyIntfIpConfigMoStep(DceSynchronousLinearSeqContext *pContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmLocalL3IpWorker::deletePhyIntfIpConfigMoStep() ";
        NsmLocalPhyIpMessage *pMessage =
        dynamic_cast<NsmLocalPhyIpMessage *> (pContext->getPPrismMessage());
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


		NsmIpConfigManagedObject *pIpConfigMo = NULL;
        NsmL3ReadyManagedObject *pL3ReadyMo =
          dynamic_cast<NsmL3ReadyManagedObject *>
          (pContext->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));
        pIpConfigMo = (WaveManagedObjectPointer<NsmIpConfigManagedObject> (pL3ReadyMo->getIpConfig())).operator->();

        if (!pIpConfigMo) {
            trace (TRACE_LEVEL_ERROR, func_name + " IntfName: " + ifName + 
                " NSMIpConfigMO is NULL!");
            return WAVE_MESSAGE_SUCCESS;
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

    ResourceId NsmLocalL3IpWorker::validatePhyIntfIpStep(DceSynchronousLinearSeqContext *pContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmLocalL3IpWorker::validPhyIntfIpStep() ";
        NsmLocalPhyIpMessage *pMessage =
        dynamic_cast<NsmLocalPhyIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();

        trace (TRACE_LEVEL_DEBUG, "Inside " + func_name + " IntfName: " + ifName);

        trace (TRACE_LEVEL_INFO, func_name + " IntfName: " + ifName + 
            " OP-CODE: " + pMessage->getOpCode() +
            " CMD-CODE: " + pMessage->getCmdCode() +
            " switchport: " + pMessage->getSwitchport() +
            " secondary: " + pMessage->getSecond() +
            " OspfPassive: " + pMessage->getOspfPassive() +
            " OspfIgnore: " + pMessage->getOspfIgnore() +
            " IP-ADDR: " + pMessage->getIpv4Address());

        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = 
            NsmL3Util::PhyIfNameGetPhyIfLocalMO(ifName, pContext);
        if (pPhyIntfLocalManagedObject == NULL) {
            trace (TRACE_LEVEL_ERROR, func_name + "PhyIntfLocalMO not found!");
            return (WAVE_MESSAGE_ERROR);
        }
		pContext->getCache()->put(PhyIntfLocalManagedObject::getClassName(), pPhyIntfLocalManagedObject, true);
        bool isISLPort = (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pPhyIntfLocalManagedObject->getPortrole());
        trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " Found PhyIntfLocalMO!");

		NsmL3ReadyManagedObject *pL3ReadyMo = NsmL3Util::PhyIfNameGetL3ReadyMO(ifName, pContext);
		pContext->getCache()->put(NsmL3ReadyManagedObject::getClassName(), pL3ReadyMo, true);

        pMessage->setIfType(pPhyIntfLocalManagedObject->getPhyType());
        switch(pMessage->getCmdCode()) {
            case NSM_MSG_CMD_SWITCHPORT:
            {
                if (pMessage->getSwitchport()) {
                    if (pL3ReadyMo == NULL) {
                        trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " L3 is aleady disabled!");
                    }
                    else if (!(pL3ReadyMo->getCanDeleteL3ReadyMO()))
                        status = WRC_NSM_ERR_REMOVE_L3_CONFIG;
                }
                else {
                    if (pL3ReadyMo != NULL) {
                        trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " L3 is aleady enabled!");
                    }
               }
               break;
            }
            case NSM_MSG_CMD_L3READY_MO:
                if ((pL3ReadyMo) && (pMessage->getOpCode() == NSM_MSG_OP_DELETE))
                    if (!(pL3ReadyMo->getCanDeleteL3ReadyMO()))
                        status =  WRC_NSM_ERR_REMOVE_L3_CONFIG;
                break;
            case NSM_MSG_CMD_IP_ADDRESS: 
            case NSM_MSG_CMD_ALL_IPADDR: 
                if (pL3ReadyMo != NULL) {
                    ObjectId vrfObjectId = pPhyIntfLocalManagedObject->getVrfObjectId();
                    if (vrfObjectId !=  ObjectId::NullObjectId) { 
                        status = VrfUtils::validateVRFBinding(vrfObjectId);
                        if (status != WAVE_MESSAGE_SUCCESS) {
                            return status;
                        }
                    }
                    status = validatePhyIntfIpAddrStep(pContext);
                }
                else {
                    if (pPhyIntfLocalManagedObject->getSwModeFlag()) {
                        // Interface is in switchport mode i.e. L2
                        status = WRC_NSM_API_ERR_L3_NOT_READY_NO_IPADDR;
                    }
                    else {
                        // Interface isn't in switchport mode. 
                        // It must be in SPAN mirrored mode, i.e. destination port
                        status = WRC_NSM_API_ERR_NO_IPADDR_ON_MIRRORED_PORT;
                    }
                }
                break;

            case NSM_MSG_CMD_MTU:
            case NSM_MSG_CMD_IP_DIRECTED_BROADCAST:
            case NSM_MSG_CMD_PROXY_ARP:
            case NSM_MSG_CMD_ARP_AGING_TIMEOUT:
            	if(true == isISLPort) {
            		trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " ISL Port! No L3 Config allowed");
            		return WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
            	}
                if (pL3ReadyMo == NULL) {
                    if (pPhyIntfLocalManagedObject->getSwModeFlag()) {
                        // Interface is in switchport mode i.e. L2
                        status = WRC_NSM_API_ERR_L3_NOT_READY;
                    }
                    else {
                        // Interface isn't in switchport mode. 
                        // It must be in SPAN mirrored mode, i.e. destination port
                        status = WRC_NSM_API_ERR_NO_IPCONFIG_ON_MIRRORED_PORT;
                    }
                }
                break;

            default:
                trace (TRACE_LEVEL_ERROR, func_name + " IntfName: " + ifName + 
                    " Wrong CmdCode: " + pMessage->getCmdCode());
                status = WAVE_MESSAGE_ERROR;

                break;
        }
        pMessage->setCompletionStatus(status);
        return status;
    }

    ResourceId NsmLocalL3IpWorker::validatePhyIntfIpAddrStep(DceSynchronousLinearSeqContext *pContext)
    {
        string func_name = "NsmLocalL3IpWorker::validatePhyIntfIpAddrStep() ";
        NsmLocalPhyIpMessage *pMessage =
            dynamic_cast<NsmLocalPhyIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        bool second = pMessage->getSecond();
        string ipaddr = pMessage->getIpv4Address();

        trace (TRACE_LEVEL_INFO, "Inside " + func_name + " IntfName: " + ifName);
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *> (pContext->getCache()->getWaveManagedObject(PhyIntfLocalManagedObject::getClassName()));
        if (pPhyIntfLocalManagedObject == NULL) {
            trace (TRACE_LEVEL_ERROR, func_name + "PhyIntfLocalMO not found!");
            return (WAVE_MESSAGE_ERROR);
        }

		if ( (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pPhyIntfLocalManagedObject->getPortrole()) ) {
            trace (TRACE_LEVEL_ERROR, func_name + "Cannot configure IP address in ISL mode!");
            return (WRC_NSM_ERR_ISL_IP_ADDR);
		}

        NsmIpAddrConfigManagedObject *pIpCfgMo = NULL;

		NsmL3ReadyManagedObject *pL3ReadyMo =
          dynamic_cast<NsmL3ReadyManagedObject *> (pContext->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));
        pIpCfgMo = (WaveManagedObjectPointer<NsmIpAddrConfigManagedObject> (pL3ReadyMo->getIpAddrConfig())).operator->();

        if (pMessage->getOpCode() != NSM_MSG_OP_DELETE) {
            if (pIpCfgMo == NULL) {
                if (second) {
                    trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName +
                        " IpAddriConfig MO  is NOT ready. "
                        "Secondary Ip-Addr can't be configured!");
                    return WRC_NSM_API_SET_ERR_CANT_SET_SECONDARY_FIRST;
                }
            }
            else {
                if (pIpCfgMo->findIpAddrIndex(ipaddr) >= 0) {
                    trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " +
                        ifName + " IpAddr: " + ipaddr + " exists!");


                    //pMessage->setCompletionStatus(WRC_NSM_API_ERR_IP_ADDRESS_DUPLICATED);
                    return WRC_NSM_API_ERR_IP_ADDRESS_DUPLICATED;
                }
            }
        }
        else {  // DELETE
            if (pIpCfgMo == NULL) {
                trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName +
                    " No IP-ADDR is configured on this interface!");
                return WRC_NSM_API_SET_ERR_ADDRESS_NOT_EXIST;
            }
            if (pMessage->getCmdCode() != NSM_MSG_CMD_ALL_IPADDR) {
                if (pIpCfgMo->findIpAddrIndex(ipaddr) < 0) {
                    trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " +
                        ifName + " IpAddr: " + ipaddr + " doesn't exist!");
                    return WRC_NSM_API_SET_ERR_ADDRESS_NOT_EXIST;
                }
                if (pIpCfgMo->isPrimaryIpAddr(ipaddr)) {
                    pMessage->setSecond(false);
                }
                else {
                    pMessage->setSecond(true);
                }
            }
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    void NsmLocalL3IpWorker::notifyVRRPIPDeletion(NsmL3ReadyManagedObject* l3ready, const string& ipaddr, bool all, bool secondary)
    {
        vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> > intfConfigs =
        											l3ready->getVRRPIntfConfig();

        vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> >::iterator it;
        for (it = intfConfigs.begin(); it != intfConfigs.end(); ++it) {
        	VRRPSessionConfigLocalManagedObject* sess = it->operator ->();
        	bool updated = false;
        	if (all) {
        		updated = sess->deleteVirtualIPAll();
        	} else {
				if (secondary) {
					if (sess->m_vip.size() == 1) {
						sess->m_owner = false;
						sess->m_prio = 100;
						trace (TRACE_LEVEL_ERROR, string ("secondary and vip == 1"));
					} else {
        				updated = sess->deleteVirtualIPMO(ipaddr);
					}
				} else {
        			updated = sess->deleteVirtualIPMO(ipaddr);
				}
			}
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

    void  NsmLocalL3IpWorker::NsmPhyIntfVrfConfigMessageHandler(NsmPhyIntfVrfConfigMessage *pMessage)
    {
        trace (TRACE_LEVEL_INFO, "NsmLocalL3IpWorker::NsmPhyIntfVrfConfigMessageHandler");
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::validateVRFConfigStep),
            // Enable this step at the time of backend integration
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::vrfConfigUpdateBackendStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::vrfConfigUpdateMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::updateErrorCodeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerFailedStep)
        };
        DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pDceSynchronousLinearSeqContext->execute ();

    }

    void  NsmLocalL3IpWorker::NsmVeIntfVrfConfigMessageHandler(NsmVeIntfVrfConfigMessage *pMessage)
    {
        trace (TRACE_LEVEL_INFO, "NsmLocalL3IpWorker::NsmVeIntfVrfConfigMessageHandler");
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::validateVeVRFConfigStep),
            // Enable this step at the time of backend integration
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::veVrfConfigUpdateBackendStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::veVrfConfigUpdateMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::updateErrorCodeStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmLocalL3IpWorker::prismSynchronousLinearSequencerFailedStep)
        };
        DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pDceSynchronousLinearSeqContext->execute ();

    }

    ResourceId NsmLocalL3IpWorker::updateErrorCodeStep(DceSynchronousLinearSeqContext *pContext)
    {

        /* Pass the completion status in previous step as status */

        PrismMessage *pPrismMessage = pContext->getPPrismMessage();
        ResourceId status = pPrismMessage->getCompletionStatus();

        return (status);
    }



    ResourceId NsmLocalL3IpWorker::validateVeVRFConfigStep(DceSynchronousLinearSeqContext *pContext)
    {

        string vrfName;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        NsmVeIntfVrfConfigMessage *pMessage = 
            dynamic_cast<NsmVeIntfVrfConfigMessage*> (pContext->getPPrismMessage());
        UI32 ifId = pMessage->getIfId();
        UI32 cmdCode = pMessage->getCmdCode();

        if (cmdCode == NSM_MSG_VRF_BIND_INTF) {
            vrfName = pMessage->getVrfName();
        }
        /*
        vector <string> selectField;
        selectField.push_back("vrfid");
        selectField.push_back("layer3Ready");
        */

        LocationId locationId = FrameworkToolKit::getThisLocationId ();

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NsmIntfSviManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeUI32(ifId, "vlanId"));
        //syncQueryCtxt.addSelectFields(selectField);
        vector<WaveManagedObject *> *pResults = 
                querySynchronouslyLocalManagedObjectsForLocationId (locationId, &syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            status =  WRC_NSM_INTERFACE_DOES_NOT_EXIST;
            pMessage->setCompletionStatus(status);
            return (status);
        }

        NsmIntfSviManagedObject *sviMO = dynamic_cast<NsmIntfSviManagedObject*>((*pResults)[0]);
        if (sviMO != NULL) {
            pContext->getCache()->put(NsmIntfSviManagedObject::getClassName(), sviMO, true);
            if(sviMO->getVrfObjectId() != ObjectId::NullObjectId &&
                       cmdCode == NSM_MSG_VRF_BIND_INTF) {
                trace (TRACE_LEVEL_ERROR, "validateVeVRFConfigStep:Already VRF is configured!");
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
            string vrf = VrfUtils::getVrfNameByObjectId(sviMO->getVrfObjectId());
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

    ResourceId NsmLocalL3IpWorker::veVrfConfigUpdateBackendStep (DceSynchronousLinearSeqContext *pContext)
    {
        NsmVeIntfVrfConfigMessage *pMessage = 
            dynamic_cast<NsmVeIntfVrfConfigMessage*> (pContext->getPPrismMessage());
        UI32 cmdCode = pMessage->getCmdCode();
        string vrfName = pMessage->getVrfName();
        ResourceId status = WAVE_MESSAGE_ERROR;

        NsmClientConfigVRFMessage *m = new NsmClientConfigVRFMessage();
        m->setOpcode(cmdCode);
        m->setVrfName(vrfName);
        m->setIfType(IF_TYPE_VL);
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
        trace (TRACE_LEVEL_DEBUG, "veVrfConfigUpdateBackendStep: Completed backend config");

        delete (m); 
        pMessage->setCompletionStatus(status);
        return (status);

    }
        
    ResourceId NsmLocalL3IpWorker::veVrfConfigUpdateMOStep(DceSynchronousLinearSeqContext *pContext)
    {
        NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
        NsmVeIntfVrfConfigMessage *pMessage = 
            dynamic_cast<NsmVeIntfVrfConfigMessage*> (pContext->getPPrismMessage());
        string ifName = getString(pMessage->getIfId());
        UI32 cmdCode = pMessage->getCmdCode();
        ResourceId status = WAVE_MESSAGE_ERROR;


        NsmIntfSviManagedObject *sviMO = dynamic_cast<NsmIntfSviManagedObject*>
               (pContext->getCache()->getWaveManagedObject(NsmIntfSviManagedObject::getClassName()));
        if (sviMO) {
            if (cmdCode == NSM_MSG_VRF_BIND_INTF) {
                ObjectId vrfObjectId = pContext->getCache()->getObjectId("VrfObjectId");
                sviMO->setVrfObjectId(vrfObjectId);
            } else if (cmdCode == NSM_MSG_VRF_UNBIND_INTF) {
                sviMO->setVrfObjectId(ObjectId::NullObjectId);
            }

            pL3ReadyMo = new NsmL3ReadyManagedObject(
                     dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
            if (pL3ReadyMo != NULL) {
                pL3ReadyMo->setInterfaceName(ifName);
                pL3ReadyMo->setIntfType(IF_TYPE_VL);
                sviMO->setL3Ready(pL3ReadyMo);
            } else {
                // Malloc problem so just return Generic Error
                trace(TRACE_LEVEL_ERROR, "NsmLocalL3IpWorker::vrfConfigUpdateMOStep Malloc Failed!!");
                pMessage->setCompletionStatus(status);
                return (status);
            }

            //sviMO->addAttributeToBeUpdated("layer3Ready");
            //sviMO->addAttributeToBeUpdated("vrfid");
            updateWaveManagedObject(sviMO);
        }

        trace (TRACE_LEVEL_DEBUG, "veVrfConfigUpdateMOStep: Completed MO updation");
        //status = WRC_NSM_VRF_INTERFACE_BIND_WARNING;
        status = WAVE_MESSAGE_SUCCESS;
        pMessage->setCompletionStatus(status);
        return (WAVE_MESSAGE_SUCCESS);

    }


    ResourceId NsmLocalL3IpWorker::validateVRFConfigStep(DceSynchronousLinearSeqContext *pContext)
    {
        string vrfName;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<string> selectField;
        NsmPhyIntfVrfConfigMessage *pMessage = 
            dynamic_cast<NsmPhyIntfVrfConfigMessage*> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();

        if (cmdCode == NSM_MSG_VRF_BIND_INTF) {
            vrfName = pMessage->getVrfName();
        }

        selectField.push_back("vrfid");
        selectField.push_back("phyType");
        selectField.push_back("layer3Ready");
        selectField.push_back("swModeFlag");
        selectField.push_back("Portrole");
        selectField.push_back("isdestinationMirrorportEnable");


        // Check if VRF is already configured
        PhyIntfLocalManagedObject *phyIntfMO = 
            NsmL3Util::PhyIfNameGetPhyIfLocalMO(ifName, pContext, true, &selectField);

        if (phyIntfMO != NULL) {

            pContext->getCache()->put(PhyIntfLocalManagedObject::getClassName(), phyIntfMO, true);

            //check if ISL port
            bool isISLPort = (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (phyIntfMO->getPortrole());
            if (true == isISLPort) {
                return (WRC_NSM_VRF_ISL_ERROR);
            }

            //check if SPAN port
            if (phyIntfMO->getDestinationmirrorportEnable()) {
                return (WRC_VRF_MIRROR_PORT_ERROR);
            }

            pMessage->setIfType(phyIntfMO->getPhyType());
            NsmL3ReadyManagedObject *pL3ReadyMo = (WaveManagedObjectPointer<NsmL3ReadyManagedObject>(
                                    phyIntfMO->getL3Ready())).operator->();
            if (pL3ReadyMo == NULL) {
                if (phyIntfMO->getSwModeFlag()) {
                   // Interface is in switchport mode i.e. L2
                    status = WRC_NSM_API_ERR_L3_NOT_READY_NO_IPADDR;
                } else {
                   // Interface isn't in switchport mode. 
                   // It must be in SPAN mirrored mode, i.e. destination port
                   status = WRC_NSM_API_ERR_NO_IPADDR_ON_MIRRORED_PORT;
                }
                pMessage->setCompletionStatus(status);
                return (status);
            }
            
            if(phyIntfMO->getVrfObjectId() != ObjectId::NullObjectId &&
                       cmdCode == NSM_MSG_VRF_BIND_INTF) {
                trace (TRACE_LEVEL_ERROR, "validateVRFConfigStep:Already VRF is configured!");
                status = WRC_NSM_INTFERFACE_VRF_ALREADY_CONFIGURED_ERROR;
                pMessage->setCompletionStatus(status);
                return (status);
            }
        } else {
            trace (TRACE_LEVEL_ERROR, "validateVRFConfigStep:phyIntfMO not found!");
            status = WRC_NSM_INTERFACE_DOES_NOT_EXIST;
            pMessage->setCompletionStatus(status);
            return (status);
        }
            trace (TRACE_LEVEL_DEBUG , "validateVRFConfigStep:phyIntfMO  found! " +  ifName + phyIntfMO->getPhyType() + vrfName);

        if (cmdCode == NSM_MSG_VRF_UNBIND_INTF) {
            string vrf = VrfUtils::getVrfNameByObjectId(phyIntfMO->getVrfObjectId());
            pMessage->setVrfName(vrf);
            pMessage->setCompletionStatus(status);
            return (status);
        }

        // Check if VRF exists

            trace (TRACE_LEVEL_DEBUG , "validateVRFConfigStep:bind optoin");
        ObjectId vrfObjectId = ObjectId::NullObjectId;
        status = VrfUtils::getVrfObjectIdByName(vrfName, vrfObjectId);
        if (status == WAVE_MESSAGE_SUCCESS && vrfObjectId != ObjectId::NullObjectId) {
            trace (TRACE_LEVEL_DEBUG, "validateVRFConfigStep:stasus success");
            pContext->getCache()->put("VrfObjectId", vrfObjectId);
        }

        pMessage->setCompletionStatus(status);
        return (status);
    }

    ResourceId NsmLocalL3IpWorker::vrfConfigUpdateBackendStep (DceSynchronousLinearSeqContext *pContext)
    {
        NsmPhyIntfVrfConfigMessage *pMessage = 
            dynamic_cast<NsmPhyIntfVrfConfigMessage*> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();
        UI32 ifType = pMessage->getIfType();
        string vrfName = pMessage->getVrfName();
        ResourceId status = WAVE_MESSAGE_ERROR;

        if(true  == NsmUtils::isValidThreeTuple(ifName) ) {
            string twoTuppleIfName;
            NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
            ifName = twoTuppleIfName;
        } 

        NsmClientConfigVRFMessage *m = new NsmClientConfigVRFMessage();
        m->setOpcode(cmdCode);
        m->setVrfName(vrfName);
        m->setIfType(ifType);
        m->setIfName(ifName);

        status = sendSynchronouslyToWaveClient ("nsm", m);
        if (status == WAVE_MESSAGE_SUCCESS) {
            if (m->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) {
                status = m->getCompletionStatus ();
            } else {
                int err = m->getClientStatus();
                status = NsmUtils::NsmBackendErrorMapToDcmStatus(err);
            }
        }
        trace (TRACE_LEVEL_DEBUG, string("vrfConfigUpdateBackendStep: Completed backend config") + status);

        delete (m); 
        pMessage->setCompletionStatus(status);
        return (status);

    }

    ResourceId NsmLocalL3IpWorker::vrfConfigUpdateMOStep(DceSynchronousLinearSeqContext *pContext)
    {
        NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
        NsmPhyIntfVrfConfigMessage *pMessage = 
            dynamic_cast<NsmPhyIntfVrfConfigMessage*> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        UI32 cmdCode = pMessage->getCmdCode();
        ResourceId status = WAVE_MESSAGE_ERROR;


        PhyIntfLocalManagedObject *phyIntfMO = dynamic_cast<PhyIntfLocalManagedObject *>
               (pContext->getCache()->getWaveManagedObject(PhyIntfLocalManagedObject::getClassName()));
        if (phyIntfMO) {
            if (cmdCode == NSM_MSG_VRF_BIND_INTF) {
                ObjectId vrfObjectId = pContext->getCache()->getObjectId("VrfObjectId");
                phyIntfMO->setVrfObjectId(vrfObjectId);
            } else if (cmdCode == NSM_MSG_VRF_UNBIND_INTF) {
                phyIntfMO->setVrfObjectId(ObjectId::NullObjectId);
            }

            pL3ReadyMo = new NsmL3ReadyManagedObject(
                     dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
            if (pL3ReadyMo != NULL) {
                pL3ReadyMo->setInterfaceName(ifName);
                pL3ReadyMo->setIntfType(IF_TYPE_PHY);
                phyIntfMO->setL3Ready(pL3ReadyMo);
            } else {
                // Malloc problem so just return Generic Error
                trace(TRACE_LEVEL_ERROR, "NsmLocalL3IpWorker::vrfConfigUpdateMOStep Malloc Failed!!");
                pMessage->setCompletionStatus(status);
                return (status);
            }

            phyIntfMO->addAttributeToBeUpdated("layer3Ready");
            phyIntfMO->addAttributeToBeUpdated("vrfid");
            updateWaveManagedObject(phyIntfMO);
        }

        trace (TRACE_LEVEL_DEBUG, "vrfConfigUpdateMOStep: Completed MO updation");
        //status = WRC_NSM_VRF_INTERFACE_BIND_WARNING;
        status = WAVE_MESSAGE_SUCCESS;
        pMessage->setCompletionStatus(status);
        return (WAVE_MESSAGE_SUCCESS);

    }

    ResourceId NsmLocalL3IpWorker::validateLoIntfIp(DceSynchronousLinearSeqContext *pContext) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        SI32 mapId = DcmToolKit :: getLocalMappedId();
        NsmLocalNodeIpMessage *pMessage = dynamic_cast<NsmLocalNodeIpMessage *> (pContext->getPPrismMessage());
        UI32 ifId = pMessage->getIfId();
        string ifName = pMessage->getIfName();
        string func_name = "NsmLocalL3IpWorker::validateLoIntfIp() ";
        NsmIntfLoopbackManagedObject *pLoMO = NsmL3Util::loIdGetLoopbackIfMO(ifId, mapId);
        if (pLoMO == NULL)
        {
            trace (TRACE_LEVEL_ERROR, func_name + "LoopbackIntfMO not found! - " + ifName);
            return (WRC_INTF_LOOPBACK_PORT_NOT_FOUND_ERROR);
        }
        ObjectId vrfObjectId = pLoMO->getVrfObjectId();
        if (vrfObjectId !=  ObjectId::NullObjectId) { 
            status = VrfUtils::validateVRFBinding(vrfObjectId);
            if (status != WAVE_MESSAGE_SUCCESS) {
                return status;
            }
        }
        pContext->addManagedObjectForGarbageCollection(pLoMO);
        pContext->getCache()->put(NsmIntfLoopbackManagedObject::getClassName(), pLoMO);
        return status;
    }

    ResourceId NsmLocalL3IpWorker::loopbackIntfMOStep(DceSynchronousLinearSeqContext *pContext) {
        string func_name = "NsmLocalL3IpWorker::loopbackIntfMOStep";
        NsmLocalNodeIpMessage *pMessage = dynamic_cast<NsmLocalNodeIpMessage *> (pContext->getPPrismMessage());
        string ifName = pMessage->getIfName();
        string ipAddr = pMessage->getIpv4Address();

        NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
        NsmIpAddrConfigManagedObject *pIpAddrCfgMo = NULL;
        NsmIpAddrManagedObject *pIpAddrMo = NULL;

        NsmIntfLoopbackManagedObject *pLoMO = dynamic_cast<NsmIntfLoopbackManagedObject *>(pContext->getCache()->getWaveManagedObject(NsmIntfLoopbackManagedObject::getClassName()));
        if (!pLoMO)
            return WAVE_MESSAGE_ERROR;

        pL3ReadyMo = (WaveManagedObjectPointer<NsmL3ReadyManagedObject>(pLoMO->getL3Ready())).operator->();
        if (!pL3ReadyMo)
            return WAVE_MESSAGE_ERROR;

        if (NSM_MSG_OP_DELETE != pMessage->getOpCode())
        {
            // CREATE or SET
            pIpAddrMo = new NsmIpAddrManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
            pIpAddrMo->setIpv4AddrMask(pMessage->getIpv4Address());
            pIpAddrMo->setSecond(false);
            pIpAddrCfgMo = new NsmIpAddrConfigManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
            pIpAddrCfgMo->addIpAddrMO(pIpAddrMo);
            pL3ReadyMo->setIpAddrConfig(pIpAddrCfgMo);
            pL3ReadyMo->addAttributeToBeUpdated("ipAddrCfg");
            updateWaveManagedObject(pL3ReadyMo);
            trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " Primary IP-ADDRESS is added!");
        }
        else
        {
            // DELETE
            pL3ReadyMo->setIpAddrConfig(NULL);
            pL3ReadyMo->addAttributeToBeUpdated("ipAddrCfg");
            updateWaveManagedObject(pL3ReadyMo);
            trace (TRACE_LEVEL_DEBUG, func_name + " IntfName: " + ifName + " All Ip-Addr are deleted!");
        }
        trace (TRACE_LEVEL_DEBUG, func_name + "IfName: " + ifName + " MO Update done.");
        return WAVE_MESSAGE_SUCCESS;
    }

} // DcmNs
