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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Ankit Jindal                                                 *
 **************************************************************************/
#include "Utils/Capabilities.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Attributes/AttributeSqlIn.h"

#include "DcmCore/DcmToolKit.h"
#include "DcmCStatus.h"
#include "vcs.h"
#include "Cluster/Local/WaveNode.h"

#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/Nsm/nsmVrfIntf.h"
#include "L3PostBootUtil.h"
#include "NsmIpConfigManagedObject.h"
#include "NsmIpAddrConfigManagedObject.h"
#include "NsmIntfSviManagedObject.h"
#include "NsmNodePoManagedObject.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Nsm/NsmClientCreateIntIpConfigSPMessage.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Vrf/Local/VrfUtils.h"
#include "Nsm/Local/Layer3/NsmIntfLoopbackManagedObject.h"
#include "ClientInterface/Mstp/MstpToolKit.h"

#include "ClientInterface/Nsm/NsmClientConfigVRFMessage.h"
#include "ClientInterface/Ssm/SsmIpAclIntfAssocBulkMessage.h"
#include "Ssm/Global/SsmPlugUtils.h"
#include "WyserEaGateway/GenericClientMessage.h"

#include "Nsm/Global/VlanIntfManagedObject.h"


namespace DcmNs
{

void L3PostBootUtil::triggerPostBootAfterIslToEdge (
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{
		LocationId locId = FrameworkToolKit::getThisLocationId();

		trace(TRACE_LEVEL_INFO,
				+" Entered triggerPostBootAfterIslToEdge  LocationId = " + locId);

		/*Config replay for ISL to EDGE case */

		/* L3 ACL Intf config replay */
		if(pPhyIntfLocalManagedObject) {

			/**
			 * Replaying the acl for the phy. interface
			 */
			ConfigReplayIpAclIntf(pPhyIntfLocalManagedObject);
			ConfigReplayPBRPhyIntfIslToEdge(pPhyIntfLocalManagedObject);

			if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId) {
				/**
				 * Query the PoIntfManagedObject, and replay the acl for the port-channel.
				 */
				PoIntfManagedObject *pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>(queryManagedObject(pPhyIntfLocalManagedObject->getPoId()));
				if(pPoIntfManagedObject != NULL) {
					ConfigReplayIpAclGlobalIntf(pPoIntfManagedObject);
					delete pPoIntfManagedObject;
				} else {
					trace(TRACE_LEVEL_DEBUG,"L3PostBootUtil::triggerPostBootAfterIslToEdge - IP ACL replay - PoIntfManagedObject is NULL");
				}
			}
		} else {
			trace(TRACE_LEVEL_DEBUG,"L3PostBootUtil::triggerPostBootAfterIslToEdge - IP ACL replay - PhyIntfLocalManagedObject is NULL ");
		}

		return;
	}

    void L3PostBootUtil::ConfigReplayVeConfig (
        NsmIntfSviManagedObject *pIntfSviManagedObject)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 ifId;
        string ifName;
        NsmClientCreateIntIpConfigSPMessage *m = NULL;

        trace (TRACE_LEVEL_INFO, string(__FUNCTION__) + " Entered ...");

        if (!pIntfSviManagedObject) {
            return;
        }

        ifId = pIntfSviManagedObject->getIfId();
        ifName = toString(ifId);

        // sending ve creation
        m = new NsmClientCreateIntIpConfigSPMessage();
        if (m == NULL) return;
        m->setIfId(ifId);
        m->setIfName(ifName);
        m->setIntfType(VLAN_INT);
        m->setOpcode(NSM_MSG_VE_SET);
        status = sendSynchronouslyToWaveClient ("nsm", m);
        if (status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_SUCCESS, string(__FUNCTION__) + " : (VE) sendToClient success");
        } else {
            trace(TRACE_LEVEL_ERROR, string(__FUNCTION__) + " : (VE) sendToClient failed");
        }
        delete m;

        trace(TRACE_LEVEL_DEBUG, string(__FUNCTION__) + " : (VE) sending vrf bind...");
        L3PostBootUtil::ConfigReplayVrfBind(ifName, IF_TYPE_VL, pIntfSviManagedObject->getVrfObjectId());

        // Sending "(no)shutdown"
        m = new NsmClientCreateIntIpConfigSPMessage();
        if (m == NULL) return;
        m->setIfId(ifId);
        m->setIfName(ifName);
        m->setIntfType(VLAN_INT);
        m->setStatus(pIntfSviManagedObject->getStatus());
        if (m->getStatus()) {
            m->setOpcode(NSM_MSG_INTF_SHUTDOWN);
        }
        else {
            m->setOpcode(NSM_MSG_INTF_NO_SHUTDOWN);
        }
        status = sendSynchronouslyToWaveClient ("nsm", m);
        if (status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_SUCCESS, string(__FUNCTION__) + " : (VE-STATUS) sendToClient success");
        } else {
            trace(TRACE_LEVEL_ERROR, string(__FUNCTION__) + " : (VE-STATUS) sendToClient failed");
        }
        delete m;

        return;
    }

    void L3PostBootUtil::ConfigReplayVlanIpConfig (
        LocationId locId)
    {
        UI32 ifType = VLAN_INT;
        string ifName;
        string func_name = "L3PostBootUtil::ConfigReplayVlanIpConfig() ";
        NSM_PLUG_INFO_S (func_name + "Vlan-IP::Entering ... LocationId = " + locId);

        const NsmIpConfigManagedObject *pIpConfigMo = NULL;
        const NsmIpAddrConfigManagedObject *pIpAddrConfigMo = NULL;
        NsmIntfSviManagedObject *pIntfSviManagedObject = NULL;
        vector<NsmIntfSviManagedObject *> results;

        NsmL3Util::NodeVlanIfGetMOs(results, locId);

        for(unsigned int i = 0; i < results.size(); i++) {
            pIntfSviManagedObject = results[i];

            if (!pIntfSviManagedObject) {
                continue;
            }

            ifName = toString(pIntfSviManagedObject->getIfId());

            L3PostBootUtil::ConfigReplayVeConfig(pIntfSviManagedObject);

            pIpAddrConfigMo = pIntfSviManagedObject->getIpAddrConfigMO();
            if (pIpAddrConfigMo != NULL) {
			  L3PostBootUtil::ConfigReplayIpAddr(ifName, ifType, pIpAddrConfigMo);
            }

            pIpConfigMo = pIntfSviManagedObject->getIpConfigMO();
            if (pIpConfigMo != NULL) {
			  L3PostBootUtil::ConfigReplayIpConfig(ifName, ifType, pIpConfigMo);
            }
            delete pIntfSviManagedObject;
        }

		return;
	}

    void L3PostBootUtil::ConfigReplayNodePoConfig (
        UI32 ifId, string ifName)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string func_name = "L3PostBootUtil::ConfigReplayNodePoConfig() ";

        trace (TRACE_LEVEL_INFO, func_name + " Entered ...");

        NsmClientCreateIntIpConfigSPMessage *m =
            new NsmClientCreateIntIpConfigSPMessage();
        if (m == NULL) return;
        m->setIfId(ifId);
        m->setIfName(ifName);
        m->setIntfType(PO_INT);
        m->setOpcode(NSM_MSG_SET_NODE_PO);
        status = sendSynchronouslyToWaveClient ("nsm", m);
        if (status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_SUCCESS, func_name + " : (NODE-PO) sendToClient success");
        } else {
            trace(TRACE_LEVEL_ERROR, func_name + " : (NODE-PO) sendToClient failed");
        }
        delete m;

        return;
    }

    void L3PostBootUtil::ConfigReplayPoIpConfig (
        LocationId locId)
    {
        string func_name = "L3PostBootUtil:: ConfigReplayPoIpConfig()";
        UI32 ifId;
        string ifName;
        SI32 mapId = 0;
        NSM_PLUG_INFO_S (func_name + "PO-IP::Entering ... LocationId = " + locId);

        const NsmIpConfigManagedObject *pIpConfigMo = NULL;
        const NsmIpAddrConfigManagedObject *pIpAddrConfigMo = NULL;
        NsmNodePoManagedObject *pNodePoManagedObject = NULL;
        vector<NsmNodePoManagedObject *> results;

        NsmL3Util::NodePoIfGetMOs(results, mapId);

        for(unsigned int i = 0; i < results.size(); i++) {
            pNodePoManagedObject = results[i];
            ifId = pNodePoManagedObject->getIfId();
            ifName = toString(ifId);

            ConfigReplayNodePoConfig(ifId, ifName);

            pIpAddrConfigMo = pNodePoManagedObject->getIpAddrConfigMO();
            if (pIpAddrConfigMo != NULL) {
				L3PostBootUtil::ConfigReplayIntfLoopbackIpConfig(locId);
				L3PostBootUtil::ConfigReplayIpAddr(ifName, PO_INT, pIpAddrConfigMo);
            }

            pIpConfigMo = pNodePoManagedObject->getIpConfigMO();
            if (pIpConfigMo != NULL) {
			  L3PostBootUtil::ConfigReplayIpConfig(ifName, PO_INT, pIpConfigMo);
            }
            delete pNodePoManagedObject;
        }

		return;
    }

void L3PostBootUtil::configReplayPhyIntfIpConfig (
        const PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        string ifName;
        string func_name = "NsmLocalObjectManager::"
            "ConfigReplayPhyIntfIpConfig() ";
        NSM_PLUG_INFO_S (func_name + "PHY-IP::Entering..");
        InterfaceType ifType;

        if (pPhyIntfLocalManagedObject == NULL) return;

        ifType = pPhyIntfLocalManagedObject->getPhyType();
        ifName = pPhyIntfLocalManagedObject->getIfName();

        NSM_PLUG_INFO_S (func_name +
            " Intf-Name: " + ifName +
            " Intf-Type: " + ifType);

        // Backend don't recognize 3-tuple ifName
        if (WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()) {
            NsmUtils::getTwoTupleIfName(
                pPhyIntfLocalManagedObject->getIfName(), ifName);

            NSM_PLUG_INFO_S (func_name +
                " postboot-replay VCS 2-Tuple Intf-Name: " + ifName);
            L3PostBootUtil::ConfigReplayVrfBind(ifName, ifType, pPhyIntfLocalManagedObject->getVrfObjectId());
        }

        const NsmIpAddrConfigManagedObject *pIpAddrConfigMo =
            pPhyIntfLocalManagedObject->getIpAddrConfigMO();

        if (pIpAddrConfigMo != NULL) {
          L3PostBootUtil::ConfigReplayIpAddr(ifName, ifType, pIpAddrConfigMo);
        }

        const NsmIpConfigManagedObject *pIpConfigMo =
            pPhyIntfLocalManagedObject->getIpConfigMO();
        if (pIpConfigMo != NULL) {
          L3PostBootUtil::ConfigReplayIpConfig(ifName, ifType, pIpConfigMo);
        }

        return;
    }

void L3PostBootUtil::ConfigReplayVrfBind(const string ifName, const UI32 ifType, ObjectId vrfId)
{
    ResourceId status;
    

    trace (TRACE_LEVEL_DEBUG, string(__FUNCTION__) + " Entered for ifname..." + ifName);

    if (vrfId == ObjectId::NullObjectId) {
        return;
    }

    string vrfName = VrfUtils::getVrfNameByObjectId(vrfId);

    NsmClientConfigVRFMessage *m = new NsmClientConfigVRFMessage();
    m->setOpcode(NSM_MSG_VRF_BIND_INTF);
    m->setVrfName(vrfName);
    m->setIfType(ifType);
    m->setIfName(ifName);

    status = sendSynchronouslyToWaveClient ("nsm", m);
    if (status == WAVE_MESSAGE_SUCCESS) {
        if (m->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_ERROR, string(__FUNCTION__) + 
                           "Completion status failed");
        } else if (m->getClientStatus() != 0) {
            trace(TRACE_LEVEL_ERROR, string(__FUNCTION__) + 
                           "Client status failed");
        }
    }   
    delete (m); 
    return;
}



void L3PostBootUtil::ConfigReplayIntfLoopbackIpConfig ( LocationId locId)
    {

	 UI32 ifType = IF_TYPE_INTF_LOOPBACK;
        string ifName;
        NSM_PLUG_INFO_S (string(__FUNCTION__) + "Intf-Loopback-IP::Entering ..." +
            " LocationId = " + locId);

        const NsmIpConfigManagedObject *pIpConfigMo = NULL;
        const NsmIpAddrConfigManagedObject *pIpAddrConfigMo = NULL;
        NsmIntfLoopbackManagedObject *pIntfLoopbackMo = NULL;
        vector<NsmIntfLoopbackManagedObject *> results;

        NsmL3Util::GetIntfLoopbackMOs(results, locId);

        for(unsigned int i = 0; i < results.size(); i++) {
            pIntfLoopbackMo = results[i];

            if (!pIntfLoopbackMo) continue;

            ifName = pIntfLoopbackMo->getIfName();
            ConfigReplayIntfLoopbackConfig(pIntfLoopbackMo);
			string ifid = getString (pIntfLoopbackMo->getIfId());
            L3PostBootUtil::ConfigReplayVrfBind(ifid, ifType, pIntfLoopbackMo->getVrfObjectId());

            pIpAddrConfigMo = pIntfLoopbackMo->getIpAddrConfigMO();
            if (pIpAddrConfigMo != NULL) {
                ConfigReplayIpAddr(ifName, ifType, pIpAddrConfigMo);
            }

            pIpConfigMo = pIntfLoopbackMo->getIpConfigMO();
            if (pIpConfigMo != NULL) {
                ConfigReplayIpConfig(ifName, ifType, pIpConfigMo);
            }
            delete pIntfLoopbackMo;
        }

        return;

	}

void L3PostBootUtil::ConfigReplayIntfLoopbackConfig (
        NsmIntfLoopbackManagedObject *pIntfLoopbackMo)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 ifType = IF_TYPE_INTF_LOOPBACK;
        UI32 ifId;
        string ifName;
        NsmClientCreateIntIpConfigSPMessage *m = NULL;

        trace (TRACE_LEVEL_INFO, string(__FUNCTION__) + " Entered ...");

        if (!pIntfLoopbackMo) {
            return;
        }

        ifId = pIntfLoopbackMo->getIfId();
        ifName = pIntfLoopbackMo->getIfName();

        // sending intf-loopback creation
        m = new NsmClientCreateIntIpConfigSPMessage();
        if (m == NULL) return;
        m->setIfId(ifId);
        m->setIfName(ifName);
        m->setIntfType(ifType);
        m->setOpcode(NSM_MSG_INTF_LOOPBACK_SET);
        status = sendSynchronouslyToWaveClient ("nsm", m);
        if (status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_SUCCESS, string(__FUNCTION__) +
                " : (INTF-LOOPBACK) sendToClient success");
        } else {
            trace(TRACE_LEVEL_ERROR, string(__FUNCTION__) +
                " : (INTF-LOOPBACK) sendToClient failed");
        }
        delete m;

        // Sending "(no)shutdown"
        m = new NsmClientCreateIntIpConfigSPMessage();
        if (m == NULL) return;
        m->setIfId(ifId);
        m->setIfName(ifName);
        m->setIntfType(ifType);
        m->setStatus(pIntfLoopbackMo->getStatus());
        if (m->getStatus()) {
            m->setOpcode(NSM_MSG_INTF_SHUTDOWN);
        }
        else {
            m->setOpcode(NSM_MSG_INTF_NO_SHUTDOWN);
        }
        status = sendSynchronouslyToWaveClient ("nsm", m);
        if (status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_SUCCESS, string(__FUNCTION__) +
                " : (INTF-LOOPBACK-STATUS) sendToClient success");
        } else {
            trace(TRACE_LEVEL_ERROR, string(__FUNCTION__) +
                " : (INTF-LOOPBACK-STATUS) sendToClient failed");
				}
        delete m;

        return;
    }

void L3PostBootUtil::ConfigReplayIpAddr (
        string ifName, UI32 ifType,
        const NsmIpAddrConfigManagedObject *pIpAddrConfigMo)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmLocalObjectManager::"
            "ConfigReplayIpAddr() ";

        trace (TRACE_LEVEL_INFO, func_name + " Entered ...");

        if (pIpAddrConfigMo == NULL) return;

        vector<WaveManagedObjectPointer<NsmIpAddrManagedObject> >
            ipAddrVector = pIpAddrConfigMo->getIpAddr();

        /*
         * ipAddrVector has mix of primary and secondary IPs in random order.
         * For config replay, primary IP address should be created before secondary.
         * To ensure this order, collecting all messages related to secondary IPs in
         * a vector and sending these messages after sending primary address messages.
         */
        vector<NsmClientCreateIntIpConfigSPMessage *> msgVector;

        for (UI32 i = 0; i < ipAddrVector.size(); i++) {
            NsmClientCreateIntIpConfigSPMessage *m =
                new NsmClientCreateIntIpConfigSPMessage();
            if (m == NULL) {
            	for (UI32 k = 0; k < msgVector.size(); k++) {
            		delete (msgVector[k]);
            	}
            	msgVector.clear();
            	return;
            }

            m->setOpcode(NSM_MSG_SEC_IP_ADDRESS);
            m->setIfName(ifName);
            m->setIntfType(ifType);

            m->setIp_addr_mask(ipAddrVector[i]->getIpv4AddrMask());
            m->setSec(ipAddrVector[i]->isSecond());
			m->setOspfPassive(ipAddrVector[i]->isOspfPassive());
			m->setOspfIgnore(ipAddrVector[i]->isOspfIgnore());

            if (ipAddrVector[i]->isSecond()) {
            	msgVector.push_back(m);
                continue;
            }

            status = sendSynchronouslyToWaveClient ("nsm", m);
            if (status == WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_SUCCESS, func_name +
                    " : sendToClient success");
            } else {
                trace(TRACE_LEVEL_ERROR, func_name +
                    " : sendToClient failed");
            }

            delete (m);
        }

        for (UI32 i = 0; i < msgVector.size(); i++) {
        	NsmClientCreateIntIpConfigSPMessage *msg = msgVector[i];
        	status = sendSynchronouslyToWaveClient ("nsm", msg);
        	if (status == WAVE_MESSAGE_SUCCESS) {
        		trace(TRACE_LEVEL_SUCCESS, func_name +
        				" : sendToClient success");
        	} else {
        		trace(TRACE_LEVEL_ERROR, func_name +
        				" : sendToClient failed");
        	}

        	delete (msg);
        }
        msgVector.clear();
        return;
    }

void L3PostBootUtil::ConfigReplayIpConfig (
        string ifName, UI32 ifType,
        const NsmIpConfigManagedObject *pIpConfigMo)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string func_name = "NsmLocalObjectManager::"
            "ConfigReplayIpConfig() ";

        trace (TRACE_LEVEL_INFO, func_name + " Entered ...");
        if (pIpConfigMo == NULL) return;

        if(pIpConfigMo->getMtu() != NSM_MTU_DEF_VAL ) {
            NsmClientCreateIntIpConfigSPMessage *m =
                new NsmClientCreateIntIpConfigSPMessage();
            if (m == NULL) return;
            m->setIfName(ifName);
            m->setIntfType(ifType);

            m->setOpcode(NSM_MSG_IP_MTU);
            m->setMtu(pIpConfigMo->getMtu());
            status = sendSynchronouslyToWaveClient ("nsm", m);
            if (status == WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_SUCCESS, func_name +
                    " : (IP-MTU) sendToClient success");
            } else {
                trace(TRACE_LEVEL_ERROR, func_name +
                    " : (IP-MTU) sendToClient failed");
            }
            delete m;
        }

        if(pIpConfigMo->getDirectedBroadcast()) {
            NsmClientCreateIntIpConfigSPMessage *m =
                new NsmClientCreateIntIpConfigSPMessage();
            if (m == NULL) return;
            m->setIfName(ifName);
            m->setIntfType(ifType);

            m->setOpcode(NSM_MSG_IP_DIRECTED_BROADCAST);
            m->setDirectedBroadcast(pIpConfigMo->getDirectedBroadcast());
            status = sendSynchronouslyToWaveClient ("nsm", m);
            if (status == WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_SUCCESS, func_name +
                    " : (IP-Directed-Broadcast) sendToClient success");
            } else {
                trace(TRACE_LEVEL_ERROR, func_name +
                    " : (IP-Directed-Broadcast) sendToClient failed");
            }
            delete m;
        }
		configReplayIntfArpConfig(pIpConfigMo, ifName, ifType);
    }

void L3PostBootUtil::configReplayIntfArpConfig(const NsmIpConfigManagedObject *pIpConfigMo, string ifName, UI32 ifType)
{
  		ResourceId status = WAVE_MESSAGE_SUCCESS;

		//if physical interface, get 2 tuple ifname
        if (ifType == IF_TYPE_GI || ifType == IF_TYPE_TE || ifType ==  IF_TYPE_FO) {
            string input_ifName = ifName;
            int ret = 0;

            // Backend don't recognize 3-tuple ifName
            if (WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()) {
                ret = NsmUtils::convertThreeTuppleToTwoTupple(input_ifName, ifName);
                trace(TRACE_LEVEL_INFO, string ("configReplayIntfArpConfig ret=") + ret + string("ifname =") + ifName);
            }
        }

        // NSM Backend won't force enabling proxy-arp on ip-address any more.
        // Hence, no need to force sending proxy-arp status.
        // Only send proxy-arp message when it is enabled.
        if (pIpConfigMo->getProxyArp())
        {
            NsmClientCreateIntIpConfigSPMessage *m =
                new NsmClientCreateIntIpConfigSPMessage();
            if (m == NULL) return;
            m->setIfName(ifName);
            m->setIntfType(ifType);

            m->setOpcode(NSM_MSG_IP_PROXY_ARP);
            m->setProxy_arp(true);
            status = sendSynchronouslyToWaveClient ("arp", m);
            if (status == WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_SUCCESS, string("configReplayIntfArpConfig") +
                    string(" : (PROXY-ARP) sendToClient success"));
            } else {
                trace(TRACE_LEVEL_ERROR, string("configReplayIntfArpConfig") +
                    string(" : (PROXY-ARP) sendToClient failed"));
            }
            delete m;
        }

        if(pIpConfigMo->getArpAgingTimeout() !=
            NSM_IP_ARP_AGING_TIMEOUT_DEF_VAL)
        {
            NsmClientCreateIntIpConfigSPMessage *m =
                new NsmClientCreateIntIpConfigSPMessage();
            if (m == NULL) return;
            m->setIfName(ifName);
            m->setIntfType(ifType);

            m->setOpcode(NSM_MSG_ARP_AGEING_TIMEOUT);
            m->setArp_ageing_timeout(pIpConfigMo->getArpAgingTimeout());
            status = sendSynchronouslyToWaveClient ("arp", m);
            if (status == WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_SUCCESS, string("configReplayIntfArpConfig") +
                    string(" : (ARP-AGING-TIMEOUT) sendToClient success"));
            } else {
                trace(TRACE_LEVEL_ERROR, string("configReplayIntfArpConfig") +
                    string(" : (ARP-AGING-TIMEOUT) sendToClient failed"));
            }
            delete m;
        }
}


void L3PostBootUtil::ConfigReplayIpAclIntf(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{

	WaveNs::trace(TRACE_LEVEL_INFO, string("Inside L3PostBootUtil: ConfigReplayIpAclIntf"));

		SsmIpAclIntfAssocBulkMessage *pSsmIpAclIntfAssocBulkMessage =
				L3PostBootUtil::ConfigReplayIpAclIntfAssoc(pPhyIntfLocalManagedObject);

		if (pSsmIpAclIntfAssocBulkMessage) {
			ResourceId status = WAVE_MESSAGE_ERROR;
			status = sendSynchronouslyToWaveClient ("ssm", pSsmIpAclIntfAssocBulkMessage);
			if (status == WAVE_MESSAGE_SUCCESS) {
				SSM_PLUG_INFO_LVL_S("Phy Interface Ip Acl configuration successful ");
			} else {
				SSM_PLUG_INFO_LVL_S(string("Phy Inerface Ip Acl Interface configuration failed - ") + status);
			}
			delete pSsmIpAclIntfAssocBulkMessage;
			pSsmIpAclIntfAssocBulkMessage = NULL;
		} else {
			SSM_PLUG_INFO_LVL_S("SsmIpAclIntfAssocBulkMessage is NULL ");
		}

		return;
	}

	void L3PostBootUtil::ConfigReplayPBRPhyIntfIslToEdge(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{

		WaveNs::trace(TRACE_LEVEL_DEVEL, string("L3PostBootUtil: Inside ConfigReplayPBRPhyIntfIslToEdge"));

		string routeMapName;
		int op_code = 0;
		vector<string> args;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		char buffer[MAX_STR];
		UI32 bufferSize                        = sizeof (buffer);
		string ifname;
		GenericClientMessage_ *client_msg      = NULL;

		if (pPhyIntfLocalManagedObject == NULL)
			return;

		UI8 Iftype                              = pPhyIntfLocalManagedObject->getPhyType();

		if (pPhyIntfLocalManagedObject->getPBR().operator ->() != NULL) {

			if (true  == NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getId())) {
				string twoTuppleIfName;
				string threeTupple = pPhyIntfLocalManagedObject->getId();
				NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
				GENERATE_SHORT_IF_NAME_FOR_BACK_END(Iftype, twoTuppleIfName.c_str(), buffer, bufferSize);
			} else {
				GENERATE_SHORT_IF_NAME_FOR_BACK_END(Iftype, pPhyIntfLocalManagedObject->getId().c_str(), buffer, bufferSize);
			}

			ifname               = buffer;
			op_code = SSM_CLIENT_SET_ROUTEMAP_APPLY_INTERFACE;
			pPhyIntfLocalManagedObject->getPBRNameFromInterface (routeMapName);
			args.clear();
			args.push_back(routeMapName);
			args.push_back(ifname);
			for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
				trace(TRACE_LEVEL_INFO, string("ConfigReplayPBRPhyIntfIslToEdge - ARG. VALUE: ") + args[tmpIdx]);
			}
			client_msg = new GenericClientMessage_(args, op_code);

			if (client_msg) {
				trace(TRACE_LEVEL_INFO, "L3PostBootUtil :: ConfigReplayPBRPhyIntfIslToEdge : debug_3");
				status = WAVE_MESSAGE_ERROR;
				status = sendSynchronouslyToWaveClient ("ssm", client_msg);
				if (status == WAVE_MESSAGE_SUCCESS) {
					trace(TRACE_LEVEL_INFO, "Phy Interface PBR configuration successful ");
				} else {
					trace(TRACE_LEVEL_INFO, string("Phy Inerface PBR Interface configuration failed - ") + status);
				}

				delete client_msg;
				client_msg = NULL;
			}
		}
	}

void L3PostBootUtil::ConfigReplayIpAclGlobalIntf(PoIntfManagedObject *pPoIntfManagedObject)
	{
		WaveNs::trace(TRACE_LEVEL_INFO, string("Inside L3PostBootUtil: ConfigReplayIpAclGlobalIntf"));
		ResourceId status = WAVE_MESSAGE_ERROR;

		SsmIpAclIntfAssocBulkMessage *pSsmIpAclIntfAssocBulkMessage =
				L3PostBootUtil::ConfigReplayIpAclGlobalIntfAssoc(pPoIntfManagedObject);

		if (pSsmIpAclIntfAssocBulkMessage) {
				status = sendSynchronouslyToWaveClient ("ssm",pSsmIpAclIntfAssocBulkMessage);
			 if (status == WAVE_MESSAGE_SUCCESS) {
				 SSM_PLUG_INFO_LVL_S("Po Interface Ip Acl configuration successful ");
			 } else {
				 SSM_PLUG_INFO_LVL_S(string("Po Inerface Ip Acl Interface configuration failed - ") + status);
			 }
			 delete pSsmIpAclIntfAssocBulkMessage;
			 pSsmIpAclIntfAssocBulkMessage = NULL;
		}
		else
		{
			SSM_PLUG_INFO_LVL_S("SsmIpAclIntfAssocBulkMessage is NULL ");
		}
	}

SsmIpAclIntfAssocBulkMessage *L3PostBootUtil::ConfigReplayIpAclGlobalIntfAssoc(PoIntfManagedObject *pPoIntfManagedObject)
	{

		SsmIpAclIntfAssocBulkMessage *intfBulkMsg      		= NULL;
		UI64 num                                        	= 0;
		char buffer[25];
		buffer[24] 	= '\0';
		UI32 bufferSize 									= sizeof (buffer);
		ObjectId IngresspolicyObjId                     	= ObjectId::NullObjectId;
		ObjectId EgresspolicyObjId                      	= ObjectId::NullObjectId;
		UI32 Ingressfound                               	= 0;
		UI32 Egressfound                                	= 0;

		if (pPoIntfManagedObject) {
			 if ((Ingressfound = pPoIntfManagedObject->getIngressEgressIpAclPolicyObjectId
				 (IngresspolicyObjId, SSMACL_BINDING_DIRECTION_IN_E)))
					num = num + 1;
			 if ((Egressfound = pPoIntfManagedObject->getIngressEgressIpAclPolicyObjectId
				 (EgresspolicyObjId, SSMACL_BINDING_DIRECTION_OUT_E)))
					num = num + 1;

			 snprintf(buffer, bufferSize, "po%d", pPoIntfManagedObject->getId());
		 } else {
			 return intfBulkMsg;
		 }

		if(Egressfound) {
			if(!Capabilities::isCapabilityPresent(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS)){
				WaveNs::trace(TRACE_LEVEL_DEBUG, "Applying ACL policy in egress direction not supported in this platform");
				Egressfound = 0;
				num = num-1;
			}
		}

		if ((Ingressfound) || (Egressfound)) {
			string IngresspolicyName    = "";
			string EgresspolicyName     = "";
			string ifname               = buffer;
			intfBulkMsg                 = new SsmIpAclIntfAssocBulkMessage();

			if ((Ingressfound) && (Egressfound)) {
				IngresspolicyName   = L3PostBootUtil::getPolicyNameByObjId(IngresspolicyObjId);
				EgresspolicyName    = L3PostBootUtil::getPolicyNameByObjId(EgresspolicyObjId);
				intfBulkMsg->setIntfDetails(ifname, IngresspolicyName, SSMACL_DCMD_BIND_L3_IPV4_ACL,
				   SSMACL_BINDING_DIRECTION_IN_E);
				intfBulkMsg->setIntfDetails(ifname, EgresspolicyName, SSMACL_DCMD_BIND_L3_IPV4_ACL,
					SSMACL_BINDING_DIRECTION_OUT_E);
			} else if (Ingressfound) {
				IngresspolicyName   = L3PostBootUtil::getPolicyNameByObjId(IngresspolicyObjId);
				intfBulkMsg->setIntfDetails(ifname, IngresspolicyName, SSMACL_DCMD_BIND_L3_IPV4_ACL,
					SSMACL_BINDING_DIRECTION_IN_E);
			} else if (Egressfound) {
				EgresspolicyName    = L3PostBootUtil::getPolicyNameByObjId(EgresspolicyObjId);
				intfBulkMsg->setIntfDetails(ifname, EgresspolicyName, SSMACL_DCMD_BIND_L3_IPV4_ACL,
					SSMACL_BINDING_DIRECTION_OUT_E);
			}
			intfBulkMsg->setNumRecords(num);
		}
		return (intfBulkMsg);
	}

	GenericClientMessage_ *L3PostBootUtil::ConfigReplayPBRPhyIntf(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{

		WaveNs::trace(TRACE_LEVEL_DEVEL, string("L3PostBootUtil: Inside ConfigReplayPBRPhyIntf"));

		string routeMapName;
		int op_code = 0;
		vector<string> args;
		char buffer[MAX_STR];
		UI32 bufferSize                        = sizeof (buffer);
		string ifname;
		GenericClientMessage_ *client_msg      = NULL;

		if (pPhyIntfLocalManagedObject == NULL)
			return (0);

		UI8 Iftype                              = pPhyIntfLocalManagedObject->getPhyType();

		if (pPhyIntfLocalManagedObject->getPBR().operator ->() != NULL) {

			if (true  == NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getId())) {
				string twoTuppleIfName;
				string threeTupple = pPhyIntfLocalManagedObject->getId();
				NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
				GENERATE_SHORT_IF_NAME_FOR_BACK_END(Iftype, twoTuppleIfName.c_str(), buffer, bufferSize);
			} else {
				GENERATE_SHORT_IF_NAME_FOR_BACK_END(Iftype, pPhyIntfLocalManagedObject->getId().c_str(), buffer, bufferSize);
			}

			ifname               = buffer;
			op_code = SSM_CLIENT_SET_ROUTEMAP_APPLY_INTERFACE;
			pPhyIntfLocalManagedObject->getPBRNameFromInterface (routeMapName);
			args.clear();
			args.push_back(routeMapName);
			args.push_back(ifname);
			for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
				trace(TRACE_LEVEL_INFO, string("ConfigReplayPBRPhyIntf - ARG. VALUE: ") + args[tmpIdx]);
			}
			client_msg = new GenericClientMessage_(args, op_code);
		}

		return client_msg;
	}

	SsmIpAclIntfAssocBulkMessage *L3PostBootUtil::ConfigReplayIpAclIntfAssoc(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{

		WaveNs::trace(TRACE_LEVEL_DEVEL, string("L3PostBootUtil: Inside ConfigReplayIpAclIntfAssoc"));

		ObjectId IngresspolicyObjId                 = ObjectId::NullObjectId;
		ObjectId EgresspolicyObjId                  = ObjectId::NullObjectId;
		UI32 Ingressfound                           = 0;
		UI32 Egressfound                            = 0;
		UI32 NumofRecords                           = 0;
		char buffer[MAX_STR];
		UI32 bufferSize 							= sizeof (buffer);
		SsmIpAclIntfAssocBulkMessage *intfBulkMsg 	= NULL;

		if (pPhyIntfLocalManagedObject == NULL)
			return (0);

		UI8 Iftype 		  = pPhyIntfLocalManagedObject->getPhyType();
		if ((Ingressfound = pPhyIntfLocalManagedObject->getIngressEgressIpAclPolicyObjectId (IngresspolicyObjId, SSMACL_BINDING_DIRECTION_IN_E)))
			NumofRecords = NumofRecords + 1 ;
		if ((Egressfound = pPhyIntfLocalManagedObject->getIngressEgressIpAclPolicyObjectId (EgresspolicyObjId, SSMACL_BINDING_DIRECTION_OUT_E)))
			NumofRecords = NumofRecords + 1 ;

		if (true  == NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getId())) {
			string twoTuppleIfName;
			string threeTupple = pPhyIntfLocalManagedObject->getId();
			NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
			GENERATE_SHORT_IF_NAME_FOR_BACK_END(Iftype, twoTuppleIfName.c_str(), buffer, bufferSize);
		} else {
			GENERATE_SHORT_IF_NAME_FOR_BACK_END(Iftype, pPhyIntfLocalManagedObject->getId().c_str(), buffer, bufferSize);
		}

		if(Egressfound) {
			if(!Capabilities::isCapabilityPresent(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS)){
				WaveNs::trace(TRACE_LEVEL_DEBUG, "Applying ACL policy in egress direction not supported in this platform");
				Egressfound = 0;
				NumofRecords = NumofRecords-1;
			}
		}

		if ((Ingressfound) || (Egressfound)) {
			string IngresspolicyName    = "";
			string EgresspolicyName     = "";
			string ifname               = buffer;
			intfBulkMsg 				= new SsmIpAclIntfAssocBulkMessage();
			if ((Ingressfound) && (Egressfound)) {
				IngresspolicyName   = L3PostBootUtil::getPolicyNameByObjId(IngresspolicyObjId);
				EgresspolicyName    = L3PostBootUtil::getPolicyNameByObjId(EgresspolicyObjId);
				intfBulkMsg->setIntfDetails(ifname, IngresspolicyName, SSMACL_DCMD_BIND_L3_IPV4_ACL,
					SSMACL_BINDING_DIRECTION_IN_E);
				intfBulkMsg->setIntfDetails(ifname, EgresspolicyName, SSMACL_DCMD_BIND_L3_IPV4_ACL,
					SSMACL_BINDING_DIRECTION_OUT_E);
			} else if (Ingressfound) {
				IngresspolicyName   = L3PostBootUtil::getPolicyNameByObjId(IngresspolicyObjId);
				intfBulkMsg->setIntfDetails(ifname, IngresspolicyName, SSMACL_DCMD_BIND_L3_IPV4_ACL,
					SSMACL_BINDING_DIRECTION_IN_E);
			} else if (Egressfound) {
				EgresspolicyName    = L3PostBootUtil::getPolicyNameByObjId(EgresspolicyObjId);
				intfBulkMsg->setIntfDetails(ifname, EgresspolicyName, SSMACL_DCMD_BIND_L3_IPV4_ACL,
					SSMACL_BINDING_DIRECTION_OUT_E);
			}
			intfBulkMsg->setNumRecords(NumofRecords);
		}
		return (intfBulkMsg);
	}

const string L3PostBootUtil::getPolicyNameByObjId(const ObjectId objId)
{
           vector <WaveManagedObject   *>*pWaveManagedObject = NULL;
           IpAclPolicyManagedObject *policyMO = NULL;
           string policyName;
           WaveManagedObjectSynchronousQueryContext queryCtxt("IpAclPolicyManagedObject");
	   queryCtxt.addAndAttribute(new AttributeObjectId(objId, "objectId"));
           queryCtxt.addSelectField("policyName");

           pWaveManagedObject = WaveObjectManagerToolKit::querySynchronously(&queryCtxt);
           if (!pWaveManagedObject->size()) {
               return NULL;
           }
           policyMO = dynamic_cast<IpAclPolicyManagedObject *>((*pWaveManagedObject)[0]);
           policyName = policyMO->getPolicyName();
           WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pWaveManagedObject);
           /*
           pWaveManagedObject = WaveObjectManagerToolKit::queryManagedObject (objId);
           if (pWaveManagedObject != NULL) {
                   policyMO     = dynamic_cast<IpAclPolicyManagedObject *>(pWaveManagedObject);
                   policyName   = policyMO->getPolicyName();
                   delete pWaveManagedObject;
           } */

           return (policyName);

}


}
