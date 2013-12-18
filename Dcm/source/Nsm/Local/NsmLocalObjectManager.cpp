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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Attributes/AttributeUI32Range.h"

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Eld/EldLocalWorker.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/Igmps/VlanIntfIgmpsLocalConfWorker.h"
#include "Nsm/Local/OSPF/OSPFInterfaceLocalConfWorker.h"
#include "Nsm/Local/IntfConfLocalWorker.h"
#include "Nsm/Local/VlanClassifierLocalConfWorker.h"
#include "Nsm/Local/PVlanLocalConfWorker.h"
#include "Nsm/Local/IntfXstpLocalConfWorker.h"
#include "Nsm/Local/Dot1xLocalWorker.h"
#include "Nsm/Local/PortSecurityLocalConfWorker.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/ConnectorConfigLocalManagedObject.h"
#include "Nsm/Local/QosCosToTailDropManagedObject.h"
#include "Nsm/Local/NsmDot1xUpdatePortConfigMessage.h"
#include "ClientInterface/Dot1X/NsmClientDot1xUpdatePortConfigMessage.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Nsm/Local/NsmCreatePhyIntfMessage.h"

#include "DcmResourceIdEnums.h"

#include "ClientInterface/OSPF/OSPFInterfaceConfigMessage.h"
/* ELD */
#include "ClientInterface/Eld/EldClientUpdatePhyIntfMessage.h"
#include "ClientInterface/Eld/EldPoIntfMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"

//QOS
#include "Qos/Global/CeeMapManagedObject.h"
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "Qos/Global/DscpToTrafficClassMapManagedObject.h"
#include "Qos/Global/DscpToDscpMutationMapManagedObject.h"
#include "Qos/Global/DscpToCosMapManagedObject.h"
#include "Nsm/Local/PfcFlowControlLocalManagedObject.h"
#include "Nsm/Local/QosCosToProfileIdManagedObject.h"
#include "Nsm/Local/QosCosToTailDropManagedObject.h"
#include "ClientInterface/Ssm/QosClientConfigIntfNsmAttributesMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

//Sflow
#include "ClientInterface/Sflow/SflowMessageDef.h"
//FCOE
#include "Nsm/Local/FcoeWorker/NsmFcoeLocalWorker.h"
#include "Fcoe/Common/FcoeLinearSequencerContext.h"
#include "Fcoe/Global/FcoeMapManagedObject.h"
#include "Fcoe/Global/FcoeFabricMapManagedObject.h"
#include "Fcoe/Common/FcoePluginDebug.h"
#include "Fcoe/Common/FcoePluginTypes.h"
#include "Fcoe/Common/FcoePluginUtils.h"
#include "ClientInterface/Fcoe/FcoeClientEthConfigMessage.h"
//SPAN
#include "Span/Local/SpanLocalLocalObjectManager.h"
#include "Span/Local/SpanLocalConfigWorker.h"
#include "Span/Local/SpanLocalTypes.h"
#include "Span/Global/SpanGlobalConfigManagedObject.h"
#include "ClientInterface/Span/SpanClientLocalSpanLocalConfigMessage.h"
#include "ClientInterface/Span/SpanMessageDef.h"

//LLDP
#include "Nsm/Local/LldpLocalWorker.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "Lldp/Global/LldpGlobalProfileConfManagedObject.h"
#include "Nsm/Local/LldpIntfConfLocalManagedObject.h"
#include "ClientInterface/Lldp/NsmLldpClientIntfConfSetMessage.h"

//VRRP
#include "Nsm/Local/VRRPWorker/VRRPLocalConfWorker.h"

// Config Mgmt
#include "Nsm/Local/ConfigMgmt/ConfigMgmtLocalWorker.h"
//Vlan Classifier
#include "Nsm/Global/VlanClassifierGroupAssocRulesManagedObject.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"

#include "ClientInterface/Nsm/NsmClientCreateVlanClassifierRuleSPMessage.h"
#include "ClientInterface/Nsm/NsmClientDeleteVlanClassifierRuleSPMessage.h"
#include "ClientInterface/Nsm/NsmClientUpdateVlanClassifierGroupSPMessage.h"
#include "ClientInterface/Nsm/NsmClientDeleteVlanClassifierGroupSPMessage.h"
#include "ClientInterface/Nsm/NsmClientAddVlanClassifierToPoSPMessage.h"
#include "ClientInterface/Nsm/NsmClientDeleteVlanClassifierToPoSPMessage.h"
#include "ClientInterface/Nsm/NsmPhyIntfMessage.h"
#include "ClientInterface/Nsm/NsmClientUpdateGlobalConfigMessage.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "ClientInterface/Nsm/NsmVlanIntfMessage.h"
#include "ClientInterface/Nsm/NsmPoIntfMessage.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

#include "Nsm/Global/NsmGlobalCreateVlanClassifierRuleMessage.h"
#include "Nsm/Global/NsmGlobalDeleteVlanClassifierRuleMessage.h"
#include "Nsm/Global/NsmGlobalUpdateVlanClassifierGroupMessage.h"
#include "Nsm/Global/NsmGlobalDeleteVlanClassifierGroupMessage.h"
#include "Nsm/Global/NsmGlobalAddVlanClassifierToPoMessage.h"
#include "Nsm/Global/NsmGlobalDeleteVlanClassifierToPoMessage.h"
#include "Nsm/Global/VlanClassifierRulesManagedObject.h"
#include "Nsm/Local/NsmCreateVlanClassifierRuleSPMessage.h"
#include "Nsm/Local/NsmDeleteVlanClassifierRuleSPMessage.h"
#include "Nsm/Local/NsmUpdateVlanClassifierGroupSPMessage.h"
#include "Nsm/Local/NsmDeleteVlanClassifierGroupSPMessage.h"
#include "Nsm/Local/NsmDeleteVlanClassifierToPoSPMessage.h"

#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include "ClientInterface/Nsm/NsmClientUpdateLacpConfigMessage.h"
#include "ClientInterface/Nsm/NsmPhyIntfMessage.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Global/VlanClassifierActivateGlobalManagedObject.h"
#include "Nsm/Local/NsmAddVlanClassifierToPoSPMessage.h"
#include "Nsm/Global/VlanClassifierRulesManagedObject.h"

#include "Nsm/Global/LacpConfigManagedObject.h"
#include "Nsm/Local/EcmpConfigManagedObject.h"
#include "ClientInterface/Nsm/NsmEcmpConfigMessage.h"

#include "Nsm/Local/QosLocalInterfaceWorker.h"
#include "Nsm/Local/XstpPortInstanceConfigManagedObject.h"
#include "Nsm/Global/XstpPortVlanConfigManagedObject.h"
#include "Mstp/Global/InstanceBaseManagedObject.h"
#include "Nsm/Local/XstpPortConfigManagedObject.h"
#include "Nsm/Local/XstpPortInstanceConfigManagedObject.h"
#include "Nsm/Local/NsmUpdatePoStpConfigSPMessage.h"
#include "Nsm/Local/NsmUpdatePoInstanceStpConfigSPMessage.h"
#include "ClientInterface/Mstp/MstpPortConfigMessage.h"
#include "ClientInterface/Mstp/MstpPortVlanConfigMessage.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpVlanConfigMessage.h"

#include "Nsm/Local/FcoeWorker/NsmFcoeLocalWorker.h"
#include "Qos/Global/CeeMapManagedObject.h"

#include "ClientInterface/Nsm/NsmFcoeportAttrMessage.h"
#include "ClientInterface/InterfaceCommon.h"


#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Utils/ProvisioningUtil.h"

#include "ClientInterface/Nsm/NsmDefaultConfigurationCmdsMessage.h"
#include "ClientInterface/Rmon/RmonClientCreateCollectionStatisticsMessage.h"
#include "ClientInterface/Rmon/RmonClientCreateCollectionHistoryMessage.h"
#include "Nsm/Local/Rmon/NsmRmonLocalWorker.h"
#include "Nsm/Local/Rmon/RmonStatisticsLocalManagedObject.h"
#include "Nsm/Local/Rmon/RmonHistoryLocalManagedObject.h"
#include "Nsm/Local/Rmon/RmonCreateRmonCollectionStatisticsMessage.h"
#include "Nsm/Local/Rmon/RmonCreateRmonCollectionHistoryMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/VLAN/GvlanConfigUtils.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/Nsm/NsmClientCreateIntIpConfigSPMessage.h"
#include "ClientInterface/Arp/ArpMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"

#include "ClientInterface/Ssm/SsmMacAclIntfAssocBulkMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "Ssm/Global/MacAclPolicyManagedObject.h"

//RMON
#include "ClientInterface/Rmon/RmonUtils.h"
#include "Rmon/Global/RmonAlarmManagedObject.h"
#include "Rmon/Local/RmonCreateRmonAlarmSPMessage.h"
#include "ClientInterface/Rmon/RmonClientCreateRmonAlarmMessage.h"

//#if 0
/* APPM */
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalConfWorker.h"
#include "APPM/Global/AMPPProfileCompositeManagedObject.h"
#include "APPM/Global/AMPPProfileDomainCompositeManagedObject.h"
#include "APPM/Utils/APPMUtils.h"
#include "ClientInterface/APPM/PortProfileIfAssociationMessage.h"
//#endif

#include "Nsm/Local/Layer3/NsmLocalL3IpWorker.h"
#include "Nsm/Local/Layer3/NsmLocalIntfLoopbackWorker.h"

//PIM
#include "Nsm/Local/Pim/PimLocalIntfWorker.h"
#include "ssmAcl_dcmd_public.h"

// IGMP
#include "Nsm/Local/Igmp/IgmpLocalWorker.h"
#include "Igmp/Local/IgmpLocalObjectManager.h"

// DHCP Relay
#include "Nsm/Local/Dhcp/DhcpIntfLocalWorker.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalObjectManager.h"

//IGMPS
#include "ClientInterface/Igmps/IgmpsMessageDef.h"
#include "ClientInterface/Igmps/IgmpsClientUpdateMrtrVlanConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsClientUpdateSgVlanConfigMessage.h"
#include "Nsm/Global/Igmps/IgmpsMrtrVlanConfigManagedObject.h"
#include "Nsm/Global/Igmps/IgmpsSgVlanConfigManagedObject.h"
#include "Nsm/Global/Igmps/IgmpsVlanIntfConfManagedObject.h"
#include "Igmps/Local/IgmpsLocalObjectManager.h"
#include "ClientInterface/Igmps/IgmpsClientUpdateVlanConfigMessage.h"

#include "ClientInterface/Dot1X/Dot1XRadiusMessage.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"
#include "AAA/Global/RadiusHostManagedObject.h"


//ELD
#include "ClientInterface/Eld/EldClientUpdateRxlimConfigMessage.h" 


// postboot
#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpConfigManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrConfigManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"

#include "Nsm/Local/Layer3/L3PostBootUtil.h"

#include "ClientInterface/Dot1X/Dot1XRadiusMessage.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"
#include "AAA/Global/RadiusHostManagedObject.h"

/* ELD */
#include "ClientInterface/Eld/EldClientUpdatePhyIntfMessage.h"
#include "ClientInterface/Eld/EldPoIntfMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
#include "Nsm/Local/Eld/EldLocalWorker.h"
#include "ClientInterface/Eld/EldClientUpdateRxlimConfigMessage.h"

#include "Nsm/Local/IntfUdldConfLocalWorker.h"
#include "Vrf/Local/VrfUtils.h"
#include "ClientInterface/Nsm/NsmClientPVlanTrunkAssocMessage.h"
#include "ClientInterface/Nsm/NsmClientPVlanMappingMessage.h"
#include "ClientInterface/Nsm/PVlanCommon.h"
#include "VRRP/Local/VRRPLocalObjectManager.h"
#include "Pim/Local/PimLocalObjectManager.h"
#include "ClientInterface/Nsm/PortSecurityClientMessage.h"
#include "ClientInterface/Nsm/SwitchportVlanClassificationMessage.h"
#include "Nsm/Local/MacBasedVlanClassificationMO.h"
#include "Nsm/Local/CtagBasedVlanClassificationMO.h"
#include "Nsm/Local/RspanGVlanAccessPortManagedObject.h"
#include "Nsm/Local/RspanGVlanTrunkPortManagedObject.h"
#include "Nsm/Global/MacGroupManagedObject.h"
#include "ClientInterface/Nsm/MacGroupConfigMessage.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"

namespace DcmNs
{

    NsmLocalObjectManager::NsmLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {

        m_allowIslToEdge = false;

	    associateWithVirtualWaveObjectManager (NsmGlobalObjectManager::getInstance());

		m_pDot1xLocalWorker = new Dot1xLocalWorker (this);
        prismAssert (NULL != m_pDot1xLocalWorker, __FILE__, __LINE__);

		m_pPortSecurityLocalConfWorker = new PortSecurityLocalConfWorker (this);
        prismAssert (NULL != m_pPortSecurityLocalConfWorker, __FILE__, __LINE__);

		m_pIntfConfLocalWorker = new IntfConfLocalWorker (this);
        prismAssert (NULL != m_pIntfConfLocalWorker, __FILE__, __LINE__);
	m_pVlanClassifierLocalConfWorker = new VlanClassifierLocalConfWorker (this);
        prismAssert (NULL != m_pVlanClassifierLocalConfWorker, __FILE__, __LINE__);
		m_pIntfXstpLocalConfWorker = new IntfXstpLocalConfWorker (this);
        prismAssert (NULL != m_pIntfXstpLocalConfWorker, __FILE__, __LINE__);

        //PVLAN
        m_pPVlanLocalConfWorker = new PVlanLocalConfWorker(this);
        prismAssert (NULL != m_pPVlanLocalConfWorker, __FILE__, __LINE__);

        // FCOE Worker
        m_pNsmFcoeLocalWorker = new NsmFcoeLocalWorker(this);
        prismAssert (NULL != m_pNsmFcoeLocalWorker, __FILE__, __LINE__);


		//LLDP Worker
        m_pLldpLocalWorker = new LldpLocalWorker (this);
        prismAssert (NULL != m_pLldpLocalWorker, __FILE__, __LINE__);

        //VRRP Worker
        m_pVRRPLocalConfWorker = new VRRPLocalConfWorker (this);
        prismAssert (NULL != m_pVRRPLocalConfWorker, __FILE__, __LINE__);

		//CONFIG MGMT Worker
        m_pConfigMgmtLocalWorker = new ConfigMgmtLocalWorker (this);
        prismAssert (NULL != m_pConfigMgmtLocalWorker, __FILE__, __LINE__);

        m_pQosLocalInterfaceWorker = new QosLocalInterfaceWorker(this);
        prismAssert (NULL != m_pQosLocalInterfaceWorker, __FILE__, __LINE__);

        //Igmps Worker
        m_pVlanIntfIgmpsLocalConfWorker = new VlanIntfIgmpsLocalConfWorker (this);
        prismAssert (NULL != m_pVlanIntfIgmpsLocalConfWorker, __FILE__, __LINE__);
		
		//OSPF Worker
        m_pOSPFInterfaceLocalConfWorker = new OSPFInterfaceLocalConfWorker (this);
        prismAssert (NULL != m_pOSPFInterfaceLocalConfWorker, __FILE__, __LINE__);


        //PIM Worker
        m_pPimLocalIntfWorker = new PimLocalIntfWorker(this);
        prismAssert (NULL != m_pPimLocalIntfWorker, __FILE__, __LINE__);

        m_pNsmLocalL3IpWorker = new NsmLocalL3IpWorker (this);
        prismAssert (NULL != m_pNsmLocalL3IpWorker, __FILE__, __LINE__);
        m_pNsmLocalIntfLoopbackWorker = new NsmLocalIntfLoopbackWorker (this);
        prismAssert (NULL != m_pNsmLocalIntfLoopbackWorker, __FILE__, __LINE__);


        // IGMP Worker
        m_pIgmpLocalWorker = new IgmpLocalWorker (this);
        prismAssert (NULL != m_pIgmpLocalWorker, __FILE__, __LINE__);
        trace(TRACE_LEVEL_INFO, __FUNCTION__);

        // Dhcp Relay Config Worker
        m_pDhcpRelLocalWorker = new DhcpRelayLocalWorker (this);
        prismAssert (NULL != m_pDhcpRelLocalWorker, __FILE__, __LINE__);
        trace(TRACE_LEVEL_INFO, __FUNCTION__);

        addEventType    (NSM_SERVICE_READY);
        addEventType    (NSM_CHASSIS_READY);
        addEventType    (NSM_LINECARD_READY);
        addEventType    (NSM_LINECARD_ONLINE);

		// ELD Worker
		m_pEldLocalWorker = new EldLocalWorker (this);
		prismAssert (NULL != m_pEldLocalWorker, __FILE__, __LINE__);

        // Rmon  Worker
        m_pNsmRmonLocalWorker = new NsmRmonLocalWorker(this);
        prismAssert (NULL != m_pNsmRmonLocalWorker, __FILE__, __LINE__);

        m_pIntfUdldConfLocalWorker = new IntfUdldConfLocalWorker(this);
        prismAssert(NULL != m_pIntfUdldConfLocalWorker, __FILE__, __LINE__);
    }

    NsmLocalObjectManager::~NsmLocalObjectManager ()
    {
		delete m_pIntfXstpLocalConfWorker;

        delete m_pNsmFcoeLocalWorker;
	
		/* RJAIN */
        delete m_pPimLocalIntfWorker;
        delete m_pNsmLocalL3IpWorker;
        delete m_pNsmLocalIntfLoopbackWorker;
        delete m_pIgmpLocalWorker;
        delete m_pDhcpRelLocalWorker;

        delete m_pIntfUdldConfLocalWorker;
    }

    NsmLocalObjectManager  *NsmLocalObjectManager::getInstance()
    {
        static NsmLocalObjectManager *pNsmLocalObjectManager = new NsmLocalObjectManager ();

        WaveNs::prismAssert (NULL != pNsmLocalObjectManager, __FILE__, __LINE__);

        return (pNsmLocalObjectManager);
    }

    void NsmLocalObjectManager::setAllowIslToEdge(bool allowIslToEdge)
    {
        m_allowIslToEdge = allowIslToEdge;
    }

    bool NsmLocalObjectManager::getAllowIslToEdge()
    {
        return m_allowIslToEdge;
    }

    void NsmLocalObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases) {
        if (m_pIntfConfLocalWorker) {
            m_pIntfConfLocalWorker->bootPhaseInterfaceStateReplay();
        }
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();
    }

    bool NsmLocalObjectManager::isEnabled() {
        return getIsEnabled();
    }

    PrismServiceId  NsmLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *NsmLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *NsmLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    IntfXstpLocalConfWorker* NsmLocalObjectManager::getIntfXstpLocalConfWorker()
    {
        return m_pIntfXstpLocalConfWorker;
    }

    void NsmLocalObjectManager::postBootConfigureVlanClassifierRules()
    {
        UI32                                ruleId = 0;
        EncapType                           encap = ENCAP_TYPE_UNKNOWN;
        string                              mac = "";
        ProtoType                           protoType = PROTO_TYPE_UNKNOWN;
        string                              appType = "protoval";
        string                              protoTypeStr = "";
        UI32                                opcode = 0;
        trace (TRACE_LEVEL_DEBUG, "Entering NsmUtils::postBootConfigureVlanClassifierRules");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierRulesManagedObject::getClassName());
        vector<WaveManagedObject * >*            pResults                          = querySynchronously(&syncQueryCtxt);
        VlanClassifierRulesManagedObject*        pVlanClassifierRulesManagedObject = NULL;

        if(NULL != pResults && pResults -> size() > 0)
        {
            for(unsigned int i = 0;i <  pResults -> size(); i++)
            {
                pVlanClassifierRulesManagedObject = dynamic_cast<VlanClassifierRulesManagedObject*>((*pResults)[i]);
                if(pVlanClassifierRulesManagedObject -> getConfigTypeChoice() == CREATE_MAC_VLAN_CLASSIFIER)
                {
                    ruleId = pVlanClassifierRulesManagedObject -> getRuleId();
                    mac = pVlanClassifierRulesManagedObject -> getMac();
                    opcode = CREATE_MAC_VLAN_CLASSIFIER;
					encap = ENCAP_TYPE_UNKNOWN;
					protoType = PROTO_TYPE_UNKNOWN;

                }
                else if(pVlanClassifierRulesManagedObject -> getConfigTypeChoice() == CREATE_PROTO_VLAN_CLASSIFIER)
                {
                    ruleId = pVlanClassifierRulesManagedObject -> getRuleId();
					mac = "";
                    protoType = pVlanClassifierRulesManagedObject -> getProtoValEnum();
                    encap = pVlanClassifierRulesManagedObject -> getEncap();
                    opcode = CREATE_PROTO_VLAN_CLASSIFIER;
                }

                NsmClientCreateVlanClassifierRuleSPMessage *m = new NsmClientCreateVlanClassifierRuleSPMessage(ruleId, mac, protoType, encap, opcode);
                trace (TRACE_LEVEL_DEVEL, "sending synchronously to nsm client");
                sendSynchronouslyToWaveClient ("nsm",m);
                delete m;
                delete pVlanClassifierRulesManagedObject;
            }
            if(pResults)
            {
                //bpatel : No need to do it since we do not need to reuse it.pResults->clear();
                delete pResults;
            }

        }
    }

     void NsmLocalObjectManager::postBootConfigureVlanClassifierGroup()
    {
        trace (TRACE_LEVEL_DEBUG, "Entering NsmUtils::postBootConfigureVlanClassifierGroup");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierGroupAssocRulesManagedObject::getClassName());
        vector<WaveManagedObject * >*               pResults                                    = querySynchronously(&syncQueryCtxt);
        VlanClassifierGroupAssocRulesManagedObject* pVlanClassifierGroupAssocRulesManagedObject = NULL;
        if (NULL != pResults && pResults -> size() > 0)
        {
            for(unsigned int i = 0;i <  pResults -> size(); i++)
            {
                pVlanClassifierGroupAssocRulesManagedObject = dynamic_cast<VlanClassifierGroupAssocRulesManagedObject*>((*pResults)[i]);
                UI32 groupId = pVlanClassifierGroupAssocRulesManagedObject -> getGroupId();
                ObjectId rule_oid = pVlanClassifierGroupAssocRulesManagedObject -> getRuleId();
                UI32 ruleId = NsmUtils::getVlanClassifierRuleByObjectId(rule_oid, false);
                UI32 oper = pVlanClassifierGroupAssocRulesManagedObject -> getoper();
                NsmClientUpdateVlanClassifierGroupSPMessage  *m = new NsmClientUpdateVlanClassifierGroupSPMessage(groupId, oper, ruleId);
                trace (TRACE_LEVEL_DEVEL, "sending synchronously to nsm client");
                sendSynchronouslyToWaveClient ("nsm",m);
                delete m;
                delete pVlanClassifierGroupAssocRulesManagedObject;
            }
            if(pResults)
            {
                //bpatel: Reason same as above pResults->clear();
                delete pResults;
            }
        }

        return ;
    }
     
	 void NsmLocalObjectManager::postBootConfigureDot1xGlobalConfig()
    {
        trace (TRACE_LEVEL_DEBUG, "Entering NsmUtils::postBootConfigureDot1xGlobalConfig");

		WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
		ResourceId dot1xstatus;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(LacpConfigManagedObject::getClassName());
        vector<WaveManagedObject * >*               pResults                                    = querySynchronously(&syncQueryCtxt);
        LacpConfigManagedObject *pLacpConfigManagedObject = NULL;
        if (NULL != pResults && pResults -> size() > 0)
        {
			pLacpConfigManagedObject = dynamic_cast<LacpConfigManagedObject*>((*pResults)[0]);
			bool dot1xenable = pLacpConfigManagedObject->getDot1xenable();
			UI32 dot1xTestTimeout = pLacpConfigManagedObject->getDot1xtesttimeout();
			NsmClientUpdateLacpConfigMessage *m = new NsmClientUpdateLacpConfigMessage();
			m->setDot1xenable(dot1xenable);
			m->setCmdCode(DOT1XUPDATE_STATUS);
			trace (TRACE_LEVEL_DEVEL, string("sending synchronously to dauthd dot1xenable") + dot1xenable);
			sendSynchronouslyToWaveClient ("dauthd",m);
			
			delete m;
			//replay test timeout config
			m = new NsmClientUpdateLacpConfigMessage();
			m->setDot1xtesttimeout(dot1xTestTimeout);
			m->setCmdCode(DOT1XUPDATE_TESTTIMEOUT);
            trace (TRACE_LEVEL_DEVEL, string("sending synchronously to dauthd dot1xtesttimeout") + dot1xTestTimeout);
            sendSynchronouslyToWaveClient ("dauthd",m);
			delete m;
			delete pLacpConfigManagedObject;
		}
        if(pResults)
        {
			//bpatel : No need to do it reason same as above pResults->clear();
			delete pResults;
		}

	 // Retrieve radius hosts from Wave DB
    synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RadiusHostManagedObject::getClassName());
    pResults = querySynchronously (synchronousQueryContext);
    RadiusHostManagedObject *pRadiusObj = NULL;

    for (UI32 i = 0; i < pResults->size(); i++)
    {
        pRadiusObj = (dynamic_cast<RadiusHostManagedObject *> ((*pResults)[i]));

		Dot1XRadiusMessage *m = new Dot1XRadiusMessage();

        m->setHost(pRadiusObj->getHost());
        m->setKey(pRadiusObj->getKey());
        m->setTimeout(pRadiusObj->getTimeout());
        m->setRetransmit(pRadiusObj->getRetransmit());
        m->setPort(pRadiusObj->getPort());
        m->setProtocol(pRadiusObj->getProtocol());
        m->setCmdCode(DOT1X_RADIUS_CONFIG_ADD);

        dot1xstatus = sendSynchronouslyToWaveClient ("dauthd", m);
        if (dot1xstatus == WAVE_MESSAGE_SUCCESS)
        {
            trace (TRACE_LEVEL_DEBUG, "Radius host " + pRadiusObj->getHost() + string(" is added successfully to the dot1x daemon"));
        } else {
            trace (TRACE_LEVEL_ERROR, "Adding Radius host " + pRadiusObj->getHost() + string(" failed"));
        }
        delete m;

    }
	
	if(pResults != NULL)
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        return ;
    }

	 void NsmLocalObjectManager::postBootConfigureEcmpGlobalConfig()
	 {
		 trace (TRACE_LEVEL_DEBUG, "Entering NsmUtils::postBootConfigureEcmpGlobalConfig");
		 WaveManagedObjectSynchronousQueryContext syncQueryCtxt(EcmpConfigManagedObject::getClassName());
		 vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);

		 EcmpConfigManagedObject *pEcmpConfigManagedObject = NULL;
		 if (NULL != pResults && pResults -> size() > 0)
		 {
			 pEcmpConfigManagedObject = dynamic_cast<EcmpConfigManagedObject*>((*pResults)[0]);
			 UI32 flavor = pEcmpConfigManagedObject->getLoadBalanceMode();
			 UI32UC hash_swap = pEcmpConfigManagedObject->getHashSwap();
			 NsmEcmpConfigMessage *m = new NsmEcmpConfigMessage();
			 m->setLoadBalanceMode(flavor);
			 if(hash_swap.getIsUserConfigured())
			 {
			 	m->setHashSwap(hash_swap.getUI32Value());
				m->setCmdCode(NSMUPDATE_ECMP_BULK_CONF);
			 }
			 else{
			 	 m->setCmdCode(NSMUPDATE_ECMP_LOADBALANCE);
			 }
			 sendSynchronouslyToWaveClient("nsm",m);
			 delete m;
			 delete pEcmpConfigManagedObject;
		 }
		 if(pResults)
		 {
			 delete pResults;
		 }
		 return;
	 }

	 void NsmLocalObjectManager::postBootConfigureLacpGlobalConfig()
    {
        trace (TRACE_LEVEL_DEBUG, "Entering NsmUtils::postBootConfigureLacpGlobalConfig");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(LacpConfigManagedObject::getClassName());
        vector<WaveManagedObject * >*               pResults                                    = querySynchronously(&syncQueryCtxt);
        LacpConfigManagedObject *pLacpConfigManagedObject = NULL;
        if (NULL != pResults && pResults -> size() > 0)
        {
			pLacpConfigManagedObject = dynamic_cast<LacpConfigManagedObject*>((*pResults)[0]);
			UI32 systemPriority = pLacpConfigManagedObject->getSystemPriority();
			NsmClientUpdateLacpConfigMessage *m = new NsmClientUpdateLacpConfigMessage();
			m->setSystemPriority(systemPriority);
			m->setCmdCode(LACPUPDATE_SYS_PRIO);
			trace (TRACE_LEVEL_DEVEL, string("sending synchronously to lacp : systemPriority ") + systemPriority);
			sendSynchronouslyToWaveClient ("lacp",m);
			delete m;
			delete pLacpConfigManagedObject;
		}
        if(pResults)
        {
			// bpatel: Reason same as above pResults->clear();
			delete pResults;
		}
        return ;
    }
	 
	 void NsmLocalObjectManager::postBootConfigureNativeVlanGlobalConfig()
    {
        trace (TRACE_LEVEL_DEBUG, "Entering NsmUtils::postBootConfigureNativeVlanGlobalConfig");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(LacpConfigManagedObject::getClassName());
        vector<WaveManagedObject * >*               pResults                                    = querySynchronously(&syncQueryCtxt);
        LacpConfigManagedObject *pLacpConfigManagedObject = NULL;
        if (NULL != pResults && pResults -> size() > 0)
        {
			pLacpConfigManagedObject = dynamic_cast<LacpConfigManagedObject*>((*pResults)[0]);
			
			NsmClientUpdateGlobalConfigMessage *nsmMsg = new NsmClientUpdateGlobalConfigMessage();
			bool nVlanDisable = pLacpConfigManagedObject->getNativevlanDisable();
			nsmMsg->setNativevlanDisable(nVlanDisable);
			nsmMsg->setCmdCode(NSMUPDATE_NATIVE_VLAN);
			trace (TRACE_LEVEL_DEVEL, string("sending synchronously to nsm: NativeVlan ") + nVlanDisable);
			sendSynchronouslyToWaveClient ("nsm",nsmMsg);
			delete nsmMsg;
			delete pLacpConfigManagedObject;
		}
        if(pResults)
        {
			//bpatel: No need to do it Reason same as above pResults->clear();
			delete pResults;
		}
        return ;
    }

/*
	const string NsmLocalObjectManager::getPolicyNameByObjId(const ObjectId objId)
	{
		WaveManagedObject   *pWaveManagedObject = NULL;
		MacAclPolicyManagedObject *policyMO = NULL;
		string policyName;

		pWaveManagedObject = queryManagedObject (objId);
		if (pWaveManagedObject != NULL) {
			policyMO = dynamic_cast<MacAclPolicyManagedObject *>(pWaveManagedObject);
			policyName = policyMO->getPolicyName();
			delete pWaveManagedObject;
		}

		return (policyName);
	}
*/
	void NsmLocalObjectManager::postBootConfigureVlanClassifierGroupToPo(vector<PoIntfManagedObject *> &poMOs) {
        trace(TRACE_LEVEL_DEBUG, "Entering NsmLocalObjectManager::postBootConfigureVlanClassifierGroupToPo");
        PoIntfManagedObject* pPoIntfManagedObject;
        for (unsigned int i = 0; i < poMOs.size(); i++) {
            pPoIntfManagedObject = poMOs[i];
            vector<WaveManagedObjectPointer<VlanClassifierActivateGlobalManagedObject> > m_vlanClassifierActivate =
                    pPoIntfManagedObject->getVlanClassifierActivate();
            for (unsigned int j = 0; j < m_vlanClassifierActivate.size(); j++) {
                UI32 PoId = pPoIntfManagedObject->getId();
                string PoName = getString(PoId);
                PoName.insert(0, "po");
                UI32 groupID = m_vlanClassifierActivate[j]->getGroupId();
                ObjectId Vlan_oid = m_vlanClassifierActivate[j]->getVlanId();
                UI32 vlanId = NsmUtils::getVlanIdByObjectId(Vlan_oid, false);
                UI32 Opcode = VLAN_CLASSIFIER_PO_ACTIVATE;
                NsmClientAddVlanClassifierToPoSPMessage* m =
                        new NsmClientAddVlanClassifierToPoSPMessage(PoId,
                                PoName, 0, groupID, vlanId);
                m->setOpcode(Opcode);
                trace(TRACE_LEVEL_DEVEL, "sending synchronously to nsm client");
                sendSynchronouslyToWaveClient("nsm", m);
                delete m;

            }
        }
    }

	void NsmLocalObjectManager::configReplaySpanInterface(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{
		SpanGlobalConfigManagedObject *pSpanGlobalConfigManagedObject = NULL;
       	ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SpanGlobalConfigManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = NULL;
		pResults = querySynchronously(&syncQueryCtxt);
		string sourcestr_twoTupleIfName;
		string deststr_twoTupleIfName;

		trace (TRACE_LEVEL_INFO,"Entered NSM Span config replay\n");

		if(pResults == NULL || pResults->size() == 0)
			trace (TRACE_LEVEL_INFO,"Not possible case\n");

        if (pResults != NULL && pResults->size() != 0)
        {
            UI32 numberOfResults = pResults->size();

		   trace (TRACE_LEVEL_INFO,string("pResults for span is not NULL\n") + numberOfResults);
           for (UI32 i=0; i< numberOfResults; i++)
            {
                 pSpanGlobalConfigManagedObject = dynamic_cast<SpanGlobalConfigManagedObject *>((*pResults)[i]);

                 SI32 srcMappedId = 0;
                 SI32 dstMappedId = 0;
                 UI32 rspanVlanId = 0;

				 sourcestr_twoTupleIfName = pSpanGlobalConfigManagedObject->getSource_ifName();
				 deststr_twoTupleIfName = pSpanGlobalConfigManagedObject->getDest_ifName();
				 trace (TRACE_LEVEL_INFO,string("Src Interface:")+sourcestr_twoTupleIfName);
				 trace (TRACE_LEVEL_INFO,string("Dst Interface:")+deststr_twoTupleIfName);
				 trace (TRACE_LEVEL_INFO,string("Physical Interface Name:")+pPhyIntfLocalManagedObject->getIfName());


                 if (pSpanGlobalConfigManagedObject && ( (deststr_twoTupleIfName == pPhyIntfLocalManagedObject->getIfName()) || (sourcestr_twoTupleIfName == pPhyIntfLocalManagedObject->getIfName()) ) ) 
				  { 
                	 trace (TRACE_LEVEL_INFO,string("Creating the session first"));
                	 SpanClientLocalSpanLocalConfigMessage *createSessionMsg = new SpanClientLocalSpanLocalConfigMessage();
                	 createSessionMsg->setSession_id(pSpanGlobalConfigManagedObject->getSession_id());
                	 createSessionMsg->setOpCode(SPAN_MSG_SET);
                	 createSessionMsg->setMessageType(SPAN_LOCAL_MSG_SESSION);
					 status = sendSynchronouslyToWaveClient ("nsm", createSessionMsg);
					 if (status == WAVE_MESSAGE_SUCCESS) {

						if(pSpanGlobalConfigManagedObject->getDescription() != "") {
							trace (TRACE_LEVEL_INFO,string("Configuring the session description"));
							SpanClientLocalSpanLocalConfigMessage *descriptionMsg = new SpanClientLocalSpanLocalConfigMessage();
							descriptionMsg->setSession_id(pSpanGlobalConfigManagedObject->getSession_id());
							descriptionMsg->setDescription(pSpanGlobalConfigManagedObject->getDescription());
							descriptionMsg->setOpCode(SPAN_MSG_SET);
							descriptionMsg->setMessageType(SPAN_LOCAL_MSG_DESC);
						    status = sendSynchronouslyToWaveClient ("nsm", descriptionMsg);
						    if (status == WAVE_MESSAGE_SUCCESS) {
							  trace(TRACE_LEVEL_SUCCESS, string("Span Description configured "));
						    }
						    else {
							  trace(TRACE_LEVEL_ERROR, string("Failed to configure the Span Description"));
						    }
						    delete descriptionMsg;
						}



						trace (TRACE_LEVEL_INFO,string("Forming the SpanClientLocalSpanLocalConfigMessage:"));
						SpanClientLocalSpanLocalConfigMessage *m = new SpanClientLocalSpanLocalConfigMessage();

						m->setSession_id(pSpanGlobalConfigManagedObject->getSession_id());

						if(true  == NsmUtils::isValidThreeTuple(pSpanGlobalConfigManagedObject->getSource_ifName()))
						  {
							NsmUtils::getTwoTupleIfName(pSpanGlobalConfigManagedObject->getSource_ifName(), sourcestr_twoTupleIfName);
							srcMappedId = NsmUtils::getMappedIdFromIfName(pSpanGlobalConfigManagedObject->getSource_ifName());
							trace (TRACE_LEVEL_INFO,string("srcMappedId:")+srcMappedId);
							m->setSource_ifName(sourcestr_twoTupleIfName);
						  }
						else
							m->setSource_ifName(pSpanGlobalConfigManagedObject->getSource_ifName());

						if(true  == NsmUtils::isValidThreeTuple(pSpanGlobalConfigManagedObject->getDest_ifName()))
						  {
							NsmUtils::getTwoTupleIfName(pSpanGlobalConfigManagedObject->getDest_ifName(), deststr_twoTupleIfName);
							m->setDest_ifName(deststr_twoTupleIfName);
							dstMappedId = NsmUtils::getMappedIdFromIfName(pSpanGlobalConfigManagedObject->getDest_ifName());
							trace (TRACE_LEVEL_INFO,string("dstMappedId:")+dstMappedId);
						  }
						else
							m->setDest_ifName(pSpanGlobalConfigManagedObject->getDest_ifName());

						 m->setDirection(pSpanGlobalConfigManagedObject->getDirection());
						 m->setSrcethernet(pSpanGlobalConfigManagedObject->getSource_tengiga());
						 m->setDestethernet(pSpanGlobalConfigManagedObject->getDest_tengiga());
						 m->setDescription(pSpanGlobalConfigManagedObject->getDescription());
						 m->setOpCode(SPAN_MSG_SET);
						 m->setMessageType(SPAN_LOCAL_MSG_CONFIG);


						 m->setTrillSA(srcMappedId);
						 m->setTrillDA(dstMappedId);

						 if(pSpanGlobalConfigManagedObject->getDest_tengiga() == DESTINATION_TYPE_RSPAN) {
							 rspanVlanId = pSpanGlobalConfigManagedObject->getDestVlanId();
						 }
						 m->setRspanVlanId(rspanVlanId);

						 trace (TRACE_LEVEL_INFO,string("Session Id:")+m->getSession_id());
						 trace (TRACE_LEVEL_INFO,string("Source Ifc :")+m->getSource_ifName());
						 trace (TRACE_LEVEL_INFO,string("Destination Ifc :")+m->getDest_ifName());
						 trace (TRACE_LEVEL_INFO,string("DestEthernet:")+m->getDestethernet());
						 trace (TRACE_LEVEL_INFO,string("SrcEthernet:")+m->getSrcethernet());
						 trace (TRACE_LEVEL_INFO,string("Direction:")+m->getDirection());
						 trace (TRACE_LEVEL_INFO,string("Trill SA:")+m->getTrillSA());
						 trace (TRACE_LEVEL_INFO,string("Trill DA:")+m->getTrillDA());
						 trace (TRACE_LEVEL_INFO,string("RSPAN Vlan Id:")+m->getRspanVlanId());

						 status = sendSynchronouslyToWaveClient ("nsm", m);
						 if (status == WAVE_MESSAGE_SUCCESS)
						  {
							 trace(TRACE_LEVEL_SUCCESS, string("Completed the SpanConfigReplay for the Interface:")+pPhyIntfLocalManagedObject->getIfName());
						  }
						 else
						  {
							 trace(TRACE_LEVEL_ERROR, string("Span Config for Given Interface: Error"));
						  }
						 if(m->getClientStatus() != WAVE_MESSAGE_SUCCESS)
							 trace(TRACE_LEVEL_ERROR, string("\nNsm msg Client status\n") + status + "\n"+  m->getClientStatus());
						 delete m;
						 trace (TRACE_LEVEL_DEBUG,"Span ISL-EDGE port done\n");
					 }
					 else {
						 trace(TRACE_LEVEL_ERROR, string("Span Session Config failed for the session :")+createSessionMsg->getSession_id());
						 if(createSessionMsg->getClientStatus() != WAVE_MESSAGE_SUCCESS)
							 trace(TRACE_LEVEL_ERROR, string("Nsm msg Client status for create session:") + status + createSessionMsg->getClientStatus());
						 delete createSessionMsg;
					 }
				  }
				 else {
                	 trace (TRACE_LEVEL_INFO,string("Span Config Replay is not executed"));
                 }
	       }
		   if(pResults)
		   {
		     //To Cleanup the query results outside the commit block
			   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		   }
       }
		trace (TRACE_LEVEL_DEBUG,"Exiting Span config replay\n");
	}
	
	void  NsmLocalObjectManager::configReplayVlanClassifierGroupToPo(PoIntfManagedObject *pPoIntfManagedObject)  
	{
		ResourceId status = WAVE_MESSAGE_ERROR;
		if(NULL == pPoIntfManagedObject)
		{
			NSM_PLUG_DBG("PoIntfManagedObject is not valid");
			return;
		}
		vector<WaveManagedObjectPointer<VlanClassifierActivateGlobalManagedObject> > m_vlanClassifierActivate = pPoIntfManagedObject -> getVlanClassifierActivate();
		for (unsigned int j = 0; j < m_vlanClassifierActivate.size(); j++)
		{

			UI32     poId   = pPoIntfManagedObject -> getId();
			string poName   = getString(poId);
			poName.insert(0, "po");
			UI32     groupId  = m_vlanClassifierActivate[j] -> getGroupId();
			ObjectId vlan_oid = m_vlanClassifierActivate[j] -> getVlanId();
			UI32     vlanId   = NsmUtils::getVlanIdByObjectId(vlan_oid, false);
			UI32     opcode   = VLAN_CLASSIFIER_PO_ACTIVATE;
			NsmClientAddVlanClassifierToPoSPMessage* m = new NsmClientAddVlanClassifierToPoSPMessage(poId, poName, 0, groupId, vlanId);
			m -> setOpcode(opcode);
			NSM_PLUG_DBG("sending synchronously to nsm client");
			status = sendSynchronouslyToWaveClient("nsm", m);
			if(WAVE_MESSAGE_SUCCESS == status)
			{
				if(WAVE_MESSAGE_SUCCESS != (status = NsmUtils::mapBackEndErrorsToDcmdErrors(m -> getClientStatus() ) ) )
				{
					NSM_PLUG_DBG_S(string("Error From Nsm Client") + status + FrameworkToolKit::localize(status));
				}
			}
			delete m;
		}
		return ;
	}


    void  NsmLocalObjectManager::configReplayVlanClassifierGroupToPhy(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
		NSM_PLUG_DBG("Entering ...");
        ResourceId status = WAVE_MESSAGE_ERROR;
        if(NULL == pPhyIntfLocalManagedObject)
        {
			NSM_PLUG_DBG("PhyIntfLocalManagedObject is not valid");
			return;
		}
        vector<WaveManagedObjectPointer<VlanClassifierActivateLocalManagedObject> > m_vlanClassifierActivate = pPhyIntfLocalManagedObject -> getVlanClassifierActivate();
		NSM_PLUG_DBG_S(string("m_vlanClassifierActivate.size() =") + m_vlanClassifierActivate.size());
		for (unsigned int j = 0; j < m_vlanClassifierActivate.size(); j++)
		{

			string   ifId   = pPhyIntfLocalManagedObject -> getId();
			string ifName   = ifId;
			char buffer[IFNAME_MAX_STR + 1];
			buffer[IFNAME_MAX_STR] = '\0';
			UI32 bufferSize = sizeof (buffer);

            InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
            NSM_PLUG_INFO_S(string(__FUNCTION__) +
                " ifName: " + ifName + " ifType=" + ifType + "\n");

			if(true  == NsmUtils::isValidThreeTuple(ifName) )
			{
				string twoTuppleIfName ;
				NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
				ifName = twoTuppleIfName;
			}	
			GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, bufferSize);
			ifName = buffer;
			//ifName.insert(0, "te");
			UI32     groupId  = m_vlanClassifierActivate[j] -> getGroupId();
			ObjectId vlan_oid = m_vlanClassifierActivate[j] -> getVlanId();
			UI32     vlanId   = NsmUtils::getVlanIdByObjectId(vlan_oid, false);
			UI32     opcode   = VLAN_CLASSIFIER_TE_ACTIVATE;
			NSM_PLUG_DBG_S(string("Edge Replay group Id =") + groupId);
			NsmClientAddVlanClassifierToPoSPMessage* m      = new NsmClientAddVlanClassifierToPoSPMessage(0, ifName, 0, groupId, vlanId);
			m -> setOpcode(opcode);
			NSM_PLUG_DBG("sending synchronously to nsm client");
			status = sendSynchronouslyToWaveClient("nsm", m);
			if(WAVE_MESSAGE_SUCCESS == status)
			{
				if(WAVE_MESSAGE_SUCCESS != (status = NsmUtils::mapBackEndErrorsToDcmdErrors(m -> getClientStatus() ) ) )
				{
					NSM_PLUG_DBG_S(string("Error From Nsm Client") + status + FrameworkToolKit::localize(status) );
				}
			}
			delete m;
		}
		return ;
	}

    void NsmLocalObjectManager::postBootConfigureDot1xIntefaceConfig()
    {
        trace (TRACE_LEVEL_DEBUG, "Entering NsmUtils::postBootConfigureDot1xIntefaceConfig");
        WaveManagedObjectSynchronousQueryContext SyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        vector<WaveManagedObject * >*            pResults = querySynchronously(&SyncQueryCtxt);
        PhyIntfLocalManagedObject*                     pPhyIntfLocalManagedObject;
        if (NULL != pResults && pResults -> size() > 0)
        {
            for(unsigned int i = 0;i <  pResults -> size(); i++)
            {
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[i]);
                WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> pDot1xPortConfigLocalManagedObject = WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> (pPhyIntfLocalManagedObject->getDot1xIntfConfig());

				Dot1xPortConfigLocalManagedObject *tmp =  pDot1xPortConfigLocalManagedObject.operator-> ();

				if (tmp == NULL) {
				  continue;
				}

				NsmClientDot1xUpdatePortConfigMessage *m = new NsmClientDot1xUpdatePortConfigMessage() ;

				m->setIfName(pDot1xPortConfigLocalManagedObject->getIfName());
				m->setEnabled(pDot1xPortConfigLocalManagedObject->getEnabled());
				m->setPort_ctrl_type(pDot1xPortConfigLocalManagedObject->getPort_ctrl_type());
				m->setProto_version(pDot1xPortConfigLocalManagedObject->getProto_version());
				m->setQuiet_interval(pDot1xPortConfigLocalManagedObject->getQuiet_interval());
				m->setMax_reauth_attempt(pDot1xPortConfigLocalManagedObject->getMax_reauth_attempt());
				m->setEnable_auth(pDot1xPortConfigLocalManagedObject->getEnable_auth());
				m->setReauth_timeout_interval(pDot1xPortConfigLocalManagedObject->getReauth_timeout_interval());
				m->setServer_timeout_interval(pDot1xPortConfigLocalManagedObject->getServer_timeout_interval());
				m->setSupp_timeout_interval(pDot1xPortConfigLocalManagedObject->getSupp_timeout_interval());
				m->setTx_timeout_interval(pDot1xPortConfigLocalManagedObject->getTx_timeout_interval());

                InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
                NSM_PLUG_INFO_S(string(__FUNCTION__) +
                    " ifName: " + m->getIfName() + " ifType=" + ifType + "\n");
        		m->setIfType(ifType);

                if(true == NsmUtils::isValidThreeTuple(m->getIfName())) {
                    LocationId locationId = NsmUtils::getLocationId(m->getIfName());
                    if (locationId == NsmUtils::getLocalLocationId()) {
                        string twoTuppleIfName;
                        NsmUtils::getTwoTupleIfName(m->getIfName(), twoTuppleIfName);
                        m->setIfName(twoTuppleIfName);
                    } else {
                        /* If this command does not belong to this node, discard.
                         * It will be replayed by corresponding node.
                         */
                        delete m;
                        continue;
                    }
                }

				trace (TRACE_LEVEL_INFO, string(" Entering  Dot1x Post boot ") + pDot1xPortConfigLocalManagedObject->getIfName() +
					m->getEnabled() +
					m->getPort_ctrl_type() +
					m->getProto_version() +
					m->getQuiet_interval() +
					m->getMax_reauth_attempt() +
					m->getEnable_auth() +
					m->getReauth_timeout_interval() +
					m->getServer_timeout_interval() +
					m->getSupp_timeout_interval() +
					m->getTx_timeout_interval()
					);


				m->setOpcode(DOT1X_SET_OP);
				m->setCmdCode(DOT1X_OPCODE_ALL);

				trace (TRACE_LEVEL_DEVEL, "sending synchronously to dauthd client");
				sendSynchronouslyToWaveClient("dauthd", m);
				delete m;
            }
        }
        if(pResults)
        {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

    }

/* ELD Global config replay */
    UI32 setGlobalConfigForReplay (EldConfigManagedObject
                                        *pEldConfigMO, EldClientUpdateRxlimConfigMessage *m)
    {
        UI32 cmdcode = 0x00000000;

        m->setRxlim(pEldConfigMO->getRxlim());
        cmdcode |= ELD_GLOBAL_CONF_VCS_RX_LIMIT;

        m->setHelloInt(pEldConfigMO->getHello());
        cmdcode |= ELD_GLOBAL_CONF_HELLO_TIME;

        m->setShutdnTime(pEldConfigMO->getShutdown());
        cmdcode |= ELD_GLOBAL_CONF_SHUTDOWN_TIME;

        m->setOpCode(cmdcode);
        return 1;
    }

    void NsmLocalObjectManager::ConfigReplayEldGlobalScalars()
    {
        EldConfigManagedObject *pEldConfigMO = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(EldConfigManagedObject::getClassName());
        vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);

        if (NULL != pResultsPMO) {
            UI32 numberOfResults = pResultsPMO->size ();
            trace(TRACE_LEVEL_DEVEL, string("EldConfigManagedObject: NumResults - ") + numberOfResults);

            if (numberOfResults == 1) {
                EldClientUpdateRxlimConfigMessage *m = new EldClientUpdateRxlimConfigMessage();
                pEldConfigMO = dynamic_cast<EldConfigManagedObject *>((*pResultsPMO)[0]);

				if (NULL == pEldConfigMO) {
            		trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager:ConfigReplayEldGlobalScalars:: pEldConfigMO is NULL."));
					return;
				}

                status = setGlobalConfigForReplay (pEldConfigMO, m);

                if (status != 0) {
                    status = sendSynchronouslyToWaveClient ("eld", m);
                    if (status == WAVE_MESSAGE_SUCCESS) {
                        trace(TRACE_LEVEL_SUCCESS, string("EldConfigManagedObject: configured "));
                    } else {
                        trace(TRACE_LEVEL_ERROR, string("EldConfigManagedObject: configured"));
                    }
                } else {
                    trace(TRACE_LEVEL_ERROR, string("EldConfigManagedObject: default config"));
                }
                delete m;
            } else {
                trace(TRACE_LEVEL_DEBUG, string("EldConfigManagedObject: no config present "));
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
        }
    }
/* ELD Global config replay */

	UI32 setLocalConfigForReplay(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject, EldClientUpdatePhyIntfMessage *K1)
	{
		UI32 cmdcode = 0x00000000;

		K1->setPrio(pPhyIntfLocalManagedObject->getEldPrio());
		cmdcode |= ELD_INTF_CONF_PRIORITY;
		cmdcode |= ELD_INTF_CONF_ELD_ENABLE;

		K1->setOpCode(cmdcode);
		string ifName = pPhyIntfLocalManagedObject->getIfName();
		if(true  == NsmUtils::isValidThreeTuple(ifName))
		{
			string twoTuppleIfName;
			NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
			ifName = twoTuppleIfName;
		}

		K1->setIfName(ifName);
		if (pPhyIntfLocalManagedObject->getIfType() != IF_TYPE_INVALID)
		{
			K1->setIfType(pPhyIntfLocalManagedObject->getIfType());
		}
		else
			return 0;

		return 1;
	}

	void NsmLocalObjectManager::configReplayEldInteface(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        NSM_PLUG_INFO_S("Entering NsmUtils::configReplayEldInteface");
		ResourceId status = WAVE_MESSAGE_ERROR;

		if (pPhyIntfLocalManagedObject == NULL)
		{
			NSM_PLUG_INFO_S("PhyIntfLocalManagedObject is not valid");
			return;
		}
		EldClientUpdatePhyIntfMessage *m = new EldClientUpdatePhyIntfMessage();
		unsigned int j = 0;
		WaveManagedObject   *pWaveManagedObject = NULL;
		VlanIntfManagedObject *pVlanIntfManagedObject;
		vector<UI32>    vlanIdVector;

		vector<ObjectId>  m_vlanid = pPhyIntfLocalManagedObject->getVlanVector();
		for (j = 0; j < m_vlanid.size(); j++) {
			if (m_vlanid[j] == ObjectId::NullObjectId) {
				NSM_PLUG_INFO_S(j + string ("VLAN ObjId Null: continue"));
				continue;
			}
			pWaveManagedObject = queryManagedObject (m_vlanid[j]);
			if (NULL != pWaveManagedObject) {
				pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>(pWaveManagedObject);
				if (NULL != pVlanIntfManagedObject) {
					NSM_PLUG_INFO_S(string ("PhyIntfLocalManagedObject VLANID: ") + pVlanIntfManagedObject->getId());
					vlanIdVector.push_back(pVlanIntfManagedObject->getId());
					delete pVlanIntfManagedObject;
				}
				else {
					trace(TRACE_LEVEL_ERROR, string("configReplayEldInteface: pVlanIntfManagedObject is NULL for a physical interface."));
					return;
				}
			}
		}

		unsigned int i;

		trace(TRACE_LEVEL_DEVEL, string("Size of the vector is ") + vlanIdVector.size());
        for (i = 0; i < vlanIdVector.size(); i++)
        {
            trace (TRACE_LEVEL_INFO, string ("\t setLocalConfigForReplay: i = ") + i + string(" vlan_id = ") + vlanIdVector[i]);
        }
		m->setVlanidList(vlanIdVector);

		status = setLocalConfigForReplay (pPhyIntfLocalManagedObject, m);
		if (status != 0) {
			status = sendSynchronouslyToWaveClient ("eld", m);
			if (status == WAVE_MESSAGE_SUCCESS) {
				NSM_PLUG_INFO_S("PhyIntfLocalManagedObject: configured ");
			} else {
				NSM_PLUG_INFO_S("PhyIntfLocalManagedObject: Not configured");
			}
		} else {
			NSM_PLUG_INFO_S("PhyIntfLocalManagedObject: default config");
		}
		delete m;
		/* ELD Global config replay */
		ConfigReplayEldGlobalScalars();
		/* ELD Global config replay */
		return;
	}

    UI32 setLocalConfigForPcReplay(PoIntfManagedObject *pPoIntfManagedObject, EldPoIntfMessage *K1)
    {
        UI32 cmdcode = 0x00000000;

        K1->setPrio(pPoIntfManagedObject->getEldPrio());
        NSM_PLUG_INFO_S("PoIntfManagedObject: prio is " + K1->getPrio());
        cmdcode |= ELD_INTF_PO_CONF_PRIORITY;
        cmdcode |= ELD_INTF_PO_CONF_ELD_ENABLE;

        K1->setCmdCode(cmdcode);
        NSM_PLUG_INFO_S(string("PoIntfManagedObject: cmdcode ") + K1->getCmdCode());
		K1->setPoId(pPoIntfManagedObject->getIfName());
        NSM_PLUG_INFO_S("PoIntfManagedObject: po is " + K1->getPoId());

        return 1;
    }

    void NsmLocalObjectManager::configReplayEldPcInteface(PoIntfManagedObject *pPoIntfManagedObject)
    {
        NSM_PLUG_INFO_S("Entering NsmUtils::configReplayEldPcInteface");
        ResourceId status = WAVE_MESSAGE_ERROR;

        if (pPoIntfManagedObject == NULL)
        {
            NSM_PLUG_INFO_S("PoIntfManagedObject is not valid");
            return;
        }
		unsigned int j = 0;
		WaveManagedObject   *pWaveManagedObject = NULL;
		VlanIntfManagedObject *pVlanIntfManagedObject;
		vector<UI32>    vlanIdVector;

		vector<ObjectId>  m_vlanid = pPoIntfManagedObject->getVlanVector();
		for (j = 0; j < m_vlanid.size(); j++) {
			if (m_vlanid[j] == ObjectId::NullObjectId) {
				NSM_PLUG_INFO_S(j + string ("VLAN ObjId Null: continue"));
				continue;
			}
			pWaveManagedObject = queryManagedObject (m_vlanid[j]);
			if (NULL != pWaveManagedObject) {
				pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>(pWaveManagedObject);
				if (NULL != pVlanIntfManagedObject) {
					NSM_PLUG_INFO_S(string ("PoIntfManagedObject vlanid: ") + pVlanIntfManagedObject->getId());
					vlanIdVector.push_back(pVlanIntfManagedObject->getId());
					delete pVlanIntfManagedObject;
				}
				else {
					trace(TRACE_LEVEL_ERROR, string("configReplayEldPcInteface: pVlanIntfManagedObject is NULL  for a port-channel."));
					return;
				}
			}
		}
		unsigned int i;

		trace(TRACE_LEVEL_DEBUG, string("Size of the vector is ") + vlanIdVector.size());
        for (i = 0; i < vlanIdVector.size(); i++)
        {
            trace (TRACE_LEVEL_DEBUG, string ("\t setLocalConfigPcForReplay: i = ") + i + string(" VLAN_ID = ") + vlanIdVector[i]);
        }
		EldPoIntfMessage *P1 = new EldPoIntfMessage();
		P1->setVlanidList(vlanIdVector);
        status = setLocalConfigForPcReplay (pPoIntfManagedObject, P1);
        if (status != 0) {
            status = sendSynchronouslyToWaveClient ("eld", P1);
        	NSM_PLUG_INFO_S(string("PoIntfManagedObject: cmdcode ") + P1->getCmdCode());
            if (status == WAVE_MESSAGE_SUCCESS) {
                NSM_PLUG_INFO_S("PoIntfManagedObject: configured ");
            } else {
                NSM_PLUG_INFO_S("PoIntfManagedObject: Not configured");
            }
        } else {
            NSM_PLUG_INFO_S("PoIntfManagedObject: default config");
        }
		if (P1)
		{
			delete P1;
			P1 = NULL;
		}
        return;
    }

	void NsmLocalObjectManager::configReplayDot1xInteface(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        NSM_PLUG_DBG("Entering NsmUtils::postBootConfigureDot1xIntefaceConfig");
		if (pPhyIntfLocalManagedObject == NULL)
		{
			NSM_PLUG_DBG("PhyIntfLocalManagedObject is not valid");
			return;
		}
		WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> pDot1xPortConfigLocalManagedObject = WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> (pPhyIntfLocalManagedObject->getDot1xIntfConfig());

		Dot1xPortConfigLocalManagedObject *tmp =  pDot1xPortConfigLocalManagedObject.operator-> ();
		if (tmp == NULL)
	   	{
			return;
		}

		NsmClientDot1xUpdatePortConfigMessage *m = new NsmClientDot1xUpdatePortConfigMessage() ;
		m->setIfName(pDot1xPortConfigLocalManagedObject->getIfName());
		m->setEnabled(pDot1xPortConfigLocalManagedObject->getEnabled());
		m->setPort_ctrl_type(pDot1xPortConfigLocalManagedObject->getPort_ctrl_type());
		m->setProto_version(pDot1xPortConfigLocalManagedObject->getProto_version());
		m->setQuiet_interval(pDot1xPortConfigLocalManagedObject->getQuiet_interval());
		m->setMax_reauth_attempt(pDot1xPortConfigLocalManagedObject->getMax_reauth_attempt());
		m->setEnable_auth(pDot1xPortConfigLocalManagedObject->getEnable_auth());
		m->setReauth_timeout_interval(pDot1xPortConfigLocalManagedObject->getReauth_timeout_interval());
		m->setServer_timeout_interval(pDot1xPortConfigLocalManagedObject->getServer_timeout_interval());
		m->setSupp_timeout_interval(pDot1xPortConfigLocalManagedObject->getSupp_timeout_interval());
		m->setTx_timeout_interval(pDot1xPortConfigLocalManagedObject->getTx_timeout_interval());
		InterfaceType phyType = pPhyIntfLocalManagedObject->getPhyType();
        m->setIfType(phyType);

		if(true == NsmUtils::isValidThreeTuple(m->getIfName())) 
		{
			LocationId locationId = NsmUtils::getLocationId(m->getIfName());
			if (locationId == NsmUtils::getLocalLocationId()) 
			{
				string twoTuppleIfName;
				NsmUtils::getTwoTupleIfName(m->getIfName(), twoTuppleIfName);
				m->setIfName(twoTuppleIfName);
			} else 
			{
				/* If this command does not belong to this node, discard.
				* It will be replayed by corresponding node.
				*/
				delete m;
				return;
			}
		}

		NSM_PLUG_DBG_S( string(" Entering  Dot1x Post boot ") + pDot1xPortConfigLocalManagedObject->getIfName() +
			m->getEnabled() +
			m->getPort_ctrl_type() +
			m->getProto_version() +
			m->getQuiet_interval() +
			m->getMax_reauth_attempt() +
			m->getEnable_auth() +
			m->getReauth_timeout_interval() +
			m->getServer_timeout_interval() +
			m->getSupp_timeout_interval() +
			m->getTx_timeout_interval()
			);

		m->setOpcode(DOT1X_SET_OP);
		m->setCmdCode(DOT1X_OPCODE_ALL);

		NSM_PLUG_DBG("sending synchronously to dauthd client");
		sendSynchronouslyToWaveClient("dauthd", m);
		delete m;
    }

	  void NsmLocalObjectManager::ConfigReplayPerIntfLacpConfig(PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject)
	  {
		if (pPhyIntfLocalManagedObject == NULL) {
       		NSM_PLUG_INFO_S(string("LLDP::pPhyIntfLocalManagedObject == NULL"));
			return;
		}

		if (pPhyIntfLocalManagedObject->getLacpTimeout() != LACP_TIMEOUT_DEFAULT) {

			NsmPhyIntfMessage *m = new NsmPhyIntfMessage();
			/* Three tupple to two tupple conversion */
			if (true  == NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName ())) {
				string twoTuppleIfName;
				string threeTupple = pPhyIntfLocalManagedObject->getIfName ();
				NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
				m->setIfName (twoTuppleIfName.c_str());
			} else {
				m->setIfName (pPhyIntfLocalManagedObject->getIfName());
			}
		    InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
	    	m->setIfType(ifType);
	        NSM_PLUG_INFO_S(string(__FUNCTION__) +
	            " ifName: " + m->getIfName() +
	            " ifType=" + m->getIfType() + "\n");
			m->setLacpTimeout(pPhyIntfLocalManagedObject->getLacpTimeout());
			NSM_PLUG_INFO_S (string("LACP:: ifName: ") + m->getIfName() +
							 string(" Timeout: ") + m->getLacpTimeout());
			m->setCmdCode(NSMUPDATE_PHY_TIMEOUT);
			NSM_PLUG_INFO_S (string("LACP:: sending synchronously to lacp client"));
			sendSynchronouslyToWaveClient("lacp", m);
			delete m;
		}
		
		if (pPhyIntfLocalManagedObject->getLacpPortPriority() != DCM_LACP_DEFAULT_PORT_PRIORITY) {

			NsmPhyIntfMessage *m = new NsmPhyIntfMessage();
			/* Three tupple to two tupple conversion */
			if (true  == NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName ())) {
				string twoTuppleIfName;
				string threeTupple = pPhyIntfLocalManagedObject->getIfName ();
				NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
				m->setIfName (twoTuppleIfName.c_str());
			} else {
				m->setIfName (pPhyIntfLocalManagedObject->getIfName());
			}
		    InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
	    	m->setIfType(ifType);
	        NSM_PLUG_INFO_S(string(__FUNCTION__) +
	            " ifName: " + m->getIfName() +
	            " ifType=" + m->getIfType() + "\n");
			m->setLacpPortPriority(pPhyIntfLocalManagedObject->getLacpPortPriority());
			NSM_PLUG_INFO_S (string("LACP:: ifName: ") + m->getIfName() +
							 string(" Port Priority: ") + m->getLacpPortPriority());
			m->setCmdCode(NSMUPDATE_PHY_PORT_PRIORITY);
			NSM_PLUG_INFO_S (string("LACP:: sending synchronously to lacp client"));
			sendSynchronouslyToWaveClient("lacp", m);
			delete m;
		}

        if(pPhyIntfLocalManagedObject->getLacpDefaultUp() != LACP_DEFAULT_DEFAULT_UP)
        {

    		NsmPhyIntfMessage *m = new NsmPhyIntfMessage();
    		/* Three tupple to two tupple conversion */
    		if (true  == NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName ())) {
    			string twoTuppleIfName;
    			string threeTupple = pPhyIntfLocalManagedObject->getIfName ();
    			NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
    			m->setIfName (twoTuppleIfName.c_str());
    		} else {
    			m->setIfName (pPhyIntfLocalManagedObject->getIfName());
    		}
    	    InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
        	m->setIfType(ifType);
            NSM_PLUG_INFO_S(string(__FUNCTION__) +
                " ifName: " + m->getIfName() +
                " ifType=" + m->getIfType() + "\n");
        	NSM_PLUG_INFO_S( string ("set lacp default-up ") + m->getLacpDefaultUp());
        	m->setLacpDefaultUp(pPhyIntfLocalManagedObject->getLacpDefaultUp());
        	m->setCmdCode(NSMUPDATE_PHY_LACP_DEFAULT_UP);
    		NSM_PLUG_INFO_S (string("LACP:: sending synchronously to lacp client"));
    		sendSynchronouslyToWaveClient("lacp", m);
    		delete m;
        }

	  }

    ResourceId
    NsmLocalObjectManager::postBootFcoeProvisioningConfigForPoInterface(PoIntfManagedObject *if_mo_p,
                                                                        bool nsm_send_override)
    {
        NsmFcoeportAttrMessage          *dmn_msg_p = NULL;
        FcoeMapManagedObject            *map_mo_p = NULL;
        FcoeFabricMapManagedObject      *fab_map_mo_p = NULL;
        string                          map_name;
        CeeMapManagedObject             *cee_map_mo_p = NULL;
        vector<ObjectId>                fab_map_vec;
        ObjectId                        fab_map_object_id;
        UI32                            vlan_id = 0;
        UI32                            j = 0;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        string                          if_id;
        WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject> fcoeport_mo_p = NULL;
        UI32                            oper_bmp = FCOEPORT_ATTR_LAG_CREATE;
        UI32                            loc_id = 0;
        bool                            send_to_nsm = true;

        fcoeport_mo_p = if_mo_p->getFcoeport();
        if (fcoeport_mo_p.operator ->() == NULL) {
            return WAVE_MESSAGE_SUCCESS;
        }
        
        FCOE_PLUGIN_LICENSE_CHECK();
        
        NSM_PLUG_INFO_S(string("PO INTF Po ") + if_mo_p->getId() + 
                        string("is an FCOE port, replaying config......."));

        map_mo_p = dynamic_cast<FcoeMapManagedObject *>(queryManagedObject(fcoeport_mo_p->getMap()));
        if (map_mo_p == NULL) {
            NSM_PLUG_DBG_ERR_S(string("Failed to find FCOE-Map MO"));
            return FCOE_PORT_ERR_POSTBOOT_MAP_NOT_FOUND;
        }
    
        cee_map_mo_p = dynamic_cast<CeeMapManagedObject *> (queryManagedObject(map_mo_p->getCee_map()));
        if (cee_map_mo_p == NULL) {
            delete map_mo_p;
            NSM_PLUG_DBG_ERR_S(string("Failed to find CEE-Map MO"));
            return FCOE_PORT_ERR_POSTBOOT_CEE_MAP_NOT_FOUND;
        }
        
        dmn_msg_p = new NsmFcoeportAttrMessage();
        if (dmn_msg_p == NULL) {
            delete map_mo_p;
            delete cee_map_mo_p;
            NSM_PLUG_DBG_ERR_S(string("Failed to create client message"));
            return FCOE_PORT_ERR_POSTBOOT_FAILED_TO_ALLOC_CLIENT_MESSAGE;
        }

        
        // Set the Oper_bmp in the Daemon Message
        dmn_msg_p->setOper_bmp(oper_bmp);
        dmn_msg_p->setMap_name(map_mo_p->getMap_name());
        dmn_msg_p->setCeeMapName(cee_map_mo_p->getCeeMapName());
		dmn_msg_p->setPhyType(IF_TYPE_PO);
        string PoName   = getString(if_mo_p->getId());
        dmn_msg_p->setIfId(PoName);
    
        fab_map_vec = map_mo_p->getFabric_map_list();
        trace(TRACE_LEVEL_INFO, string("FOUND FAB-MAP Vector of Size=") + fab_map_vec.size());
        for (j = 0; j < fab_map_vec.size(); j++) {
            fab_map_object_id = fab_map_vec.at(j);

            fab_map_mo_p = dynamic_cast<FcoeFabricMapManagedObject *>(queryManagedObject(fab_map_object_id));
            if (fab_map_mo_p == NULL) {
                delete map_mo_p;
                delete cee_map_mo_p;
                NSM_PLUG_DBG_ERR_S(string("Failed to find FAB MAP MO"));
                return FCOE_PORT_ERR_POSTBOOT_FAB_MAP_NOT_FOUND;
            }
               
            NSM_PLUG_INFO_S(string("FOUND FCOE-Fab Map for VLAN=") + 
                            fab_map_mo_p->getVlan());
            
            vlan_id = fab_map_mo_p->getVlan();
        
            //Add FCOE Vlans to the message
            dmn_msg_p->addFcoeVlan(vlan_id);

            delete fab_map_mo_p;
        }

        // Send to FCOE Daemon
        status = FcoeLocalSendEthConfigMessageToDaemon(dmn_msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR_S(string("Failed to process EthConfigMessage in FCOE"));
            FCOE_PLUGIN_DELETE(map_mo_p);
            FCOE_PLUGIN_DELETE(dmn_msg_p);
            FCOE_PLUGIN_DELETE(cee_map_mo_p);

            return (status);
        }

        if (nsm_send_override == false) {
            loc_id = FrameworkToolKit::getThisLocationId ();
            if ((NsmUtils::isPoIntfHasMemberByLocation(if_mo_p->getObjectId(), loc_id) != WAVE_MESSAGE_SUCCESS)) {
                send_to_nsm = false;
                NSM_PLUG_DBG_S(string("Po ") + if_mo_p->getId() +
                               string(" has no members, not sending to NSM daemon."));
            }
        } else {
            NSM_PLUG_DBG_S(string("Po ") + if_mo_p->getId() +
                           string(" has NSM_OVERRIDE, sending to NSM daemon."));
        }

        if (send_to_nsm) {
            // Send to NSM Daemon
            status = sendSynchronouslyToWaveClient ("nsm",dmn_msg_p);
            if (status != WAVE_MESSAGE_SUCCESS) {
                NSM_PLUG_DBG_ERR_S(string("Message Status = ") + status);
            }
            if (dmn_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                NSM_PLUG_DBG_ERR_S(string("Completion Status = ") + 
                                   dmn_msg_p->getCompletionStatus());
                status = dmn_msg_p->getCompletionStatus();
            }

            if (dmn_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
                NSM_PLUG_DBG_ERR_S(string("Client Status = ") + 
                                   dmn_msg_p->getClientStatus());
                status = dmn_msg_p->getClientStatus();
            }
        }

        delete map_mo_p;
        delete cee_map_mo_p;
        delete dmn_msg_p;

        return status;
    }

    ResourceId
    NsmLocalObjectManager::FcoeLocalSendEthConfigMessageToDaemon (NsmFcoeportAttrMessage *msg_p)
    {
        FcoeClientEthConfigMessage  *dmn_msg_p = NULL;
        vector<UI32> vlan_vec;
        vector<UI64> ifindex_vec;
        UI32 oper = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        
        oper = msg_p->getOper_bmp();
        dmn_msg_p = new FcoeClientEthConfigMessage();

        if (dmn_msg_p == NULL) {
            return WAVE_MESSAGE_SUCCESS;
        }
        dmn_msg_p->setOper_bmp(oper);
        dmn_msg_p->setMap_name(msg_p->getMap_name());
        dmn_msg_p->setCeeMapName(msg_p->getCeeMapName());
        dmn_msg_p->setIfindex(msg_p->getIfindex());
        dmn_msg_p->setIfId(msg_p->getIfId());
        dmn_msg_p->setFcoeVlanVec(msg_p->getFcoeVlanVec());
        
        if (oper == FCOEPORT_ATTR_LAG_CREATE ||
            oper == FCOEPORT_ATTR_LAG_REMOVE) {
            UI32 i = 0;
            NsmUtils::getPoMemberIfindexListForLocalNode(msg_p->getIfId(), ifindex_vec);
            //NsmUtils::getPoMemberIfindexList(msg_p->getIfId(), ifindex_vec);
            FCOE_PLUG_DBG_S(string("Retrieved Ifindex List of Size=") +
                            ifindex_vec.size());
            for (i = 0; i < ifindex_vec.size(); i++) {
                FCOE_PLUG_DBG_S(string("Retrieved Ifindex=") + ifindex_vec.at(i));
            }
            dmn_msg_p->setFcoeMbrIfindexVec(ifindex_vec);
        }

        status = sendSynchronouslyToWaveClient (FCOE_DCM_CLIENT_NAME, dmn_msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR_S(string("Message Status = ") + status);
        }

        if (dmn_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR_S(string("Client Status = ") + dmn_msg_p->getClientStatus());
            status = dmn_msg_p->getClientStatus();
        }

        if (dmn_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR_S(string("COmpletion Status = ") + dmn_msg_p->getCompletionStatus());
            status = dmn_msg_p->getCompletionStatus();
        }

        FCOE_PLUG_DBG_S(string("Final Status") + status);
        FCOE_PLUGIN_DELETE(dmn_msg_p);

        return (status);
    }


      
    ResourceId
    NsmLocalObjectManager::postBootFcoeProvisioningConfigForInterface(PhyIntfLocalManagedObject *if_mo_p)
    {
        NsmFcoeportAttrMessage          *dmn_msg_p = NULL;
        FcoeMapManagedObject            *map_mo_p = NULL;
        FcoeFabricMapManagedObject      *fab_map_mo_p = NULL;
        string                          map_name;
        CeeMapManagedObject             *cee_map_mo_p = NULL;
        vector<ObjectId>                fab_map_vec;
        ObjectId                        fab_map_object_id;
        UI32                            vlan_id = 0;
        UI32                            j = 0;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        string                          if_id;
        WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> fcoeport_mo_p = NULL;
        UI32                            oper_bmp = FCOEPORT_ATTR_CREATE;

        if (if_mo_p->getPortrole()) {
            NSM_PLUG_DBG_ERR_S(string("Fcoeport config not allowed for ISL member"));
            return WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
        }
        
        fcoeport_mo_p = if_mo_p->getFcoeport();
        if (fcoeport_mo_p.operator ->() == NULL) {
            return WAVE_MESSAGE_SUCCESS;
        }
        
        FCOE_PLUGIN_LICENSE_CHECK();
        
        NSM_PLUG_INFO_S(string("Phy INTF Te") + if_mo_p->getId() + 
                        string("is an FCOE port, replaying config......."));

        map_mo_p = dynamic_cast<FcoeMapManagedObject *>(queryManagedObject(fcoeport_mo_p->getMap()));
        if (map_mo_p == NULL) {
            NSM_PLUG_DBG_ERR_S(string("Failed to find FCOE-Map MO"));
            return FCOE_PORT_ERR_POSTBOOT_MAP_NOT_FOUND;
        }
    
        cee_map_mo_p = dynamic_cast<CeeMapManagedObject *> (queryManagedObject(map_mo_p->getCee_map()));
        if (cee_map_mo_p == NULL) {
            delete map_mo_p;
            NSM_PLUG_DBG_ERR_S(string("Failed to find CEE-Map MO"));
            return FCOE_PORT_ERR_POSTBOOT_CEE_MAP_NOT_FOUND;
        }
        
        dmn_msg_p = new NsmFcoeportAttrMessage();
        if (dmn_msg_p == NULL) {
            delete map_mo_p;
            delete cee_map_mo_p;
            NSM_PLUG_DBG_ERR_S(string("Failed to create client message"));
            return FCOE_PORT_ERR_POSTBOOT_FAILED_TO_ALLOC_CLIENT_MESSAGE;
        }

        
        // Set the Oper_bmp in the Daemon Message
        dmn_msg_p->setOper_bmp(oper_bmp);
        dmn_msg_p->setMap_name(map_mo_p->getMap_name());
        dmn_msg_p->setCeeMapName(cee_map_mo_p->getCeeMapName());
        dmn_msg_p->setIfId(if_mo_p->getId());
        dmn_msg_p->setIfindex(if_mo_p->getIfIndex());
		dmn_msg_p->setPhyType(if_mo_p->getPhyType());
        
        fab_map_vec = map_mo_p->getFabric_map_list();
        trace(TRACE_LEVEL_INFO, string("FOUND FAB-MAP Vector of Size=") + fab_map_vec.size());
        for (j = 0; j < fab_map_vec.size(); j++) {
            fab_map_object_id = fab_map_vec.at(j);

            fab_map_mo_p = dynamic_cast<FcoeFabricMapManagedObject *>(queryManagedObject(fab_map_object_id));
            if (fab_map_mo_p == NULL) {
                delete map_mo_p;
                delete cee_map_mo_p;
                NSM_PLUG_DBG_ERR_S(string("Failed to find FAB MAP MO"));
                return FCOE_PORT_ERR_POSTBOOT_FAB_MAP_NOT_FOUND;
            }
               
            NSM_PLUG_INFO_S(string("FOUND FCOE-Fab Map for VLAN=") + 
                            fab_map_mo_p->getVlan());
            
            vlan_id = fab_map_mo_p->getVlan();
        
            //Add FCOE Vlans to the message
            dmn_msg_p->addFcoeVlan(vlan_id);

            delete fab_map_mo_p;
        }

        // Send to FCOE Daemon
        status = FcoeLocalSendEthConfigMessageToDaemon(dmn_msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR_S(string("Failed to process EthConfigMessage in FCOE"));
            FCOE_PLUGIN_DELETE(map_mo_p);
             FCOE_PLUGIN_DELETE(dmn_msg_p);
            FCOE_PLUGIN_DELETE(cee_map_mo_p);

            return (status);
        }


        // Send to NSM Daemon
        status = sendSynchronouslyToWaveClient ("nsm",dmn_msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("Message Status = ") + status);
        }
        if (dmn_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("Completion Status = ") + 
                               dmn_msg_p->getCompletionStatus());
            status = dmn_msg_p->getCompletionStatus();
        }

        if (dmn_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("Client Status = ") + 
                               dmn_msg_p->getClientStatus());
            status = dmn_msg_p->getClientStatus();
        }

        delete map_mo_p;
        delete cee_map_mo_p;
        delete dmn_msg_p;

        return status;
    }

    ResourceId
    NsmLocalObjectManager::postBootFcoeProvisioningConfig()
    {
        UI32                            i = 0;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        PhyIntfLocalManagedObject       *if_mo_p = NULL;

        trace (TRACE_LEVEL_INFO, "PostBoot FCOE provisioning config");
        WaveManagedObjectSynchronousQueryContext SyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        vector<WaveManagedObject * >*pResults = querySynchronously(&SyncQueryCtxt);
        if ((pResults == NULL)) {
            trace (TRACE_LEVEL_INFO, "Query failed");
            return FCOE_PORT_ERR_POSTBOOT_NO_PHY_INTFS_FOUND;
        }
        
        if (pResults -> size() == 0) {
            trace (TRACE_LEVEL_INFO, "No PHY Interfaces found");
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return FCOE_PORT_ERR_POSTBOOT_NO_PHY_INTFS_FOUND;
        }

        for (i = 0; i < pResults->size(); i++) {
            if_mo_p = dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[i]);

            status = NsmLocalObjectManager::postBootFcoeProvisioningConfigForInterface(if_mo_p);
            if (status != WAVE_MESSAGE_SUCCESS) {
                NSM_PLUG_DBG_ERR_S(string("Fcoeport replay failed for Interface Te ") +
                                   if_mo_p->getId());
            }
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        return WAVE_MESSAGE_SUCCESS;
    }
	
	void NsmLocalObjectManager::ConfigReplayPerIntfLldpConfig(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{
       	NSM_PLUG_INFO_S(string("LLDP::Entering.."));
		NsmLldpClientIntfConfSetMessage *m = new NsmLldpClientIntfConfSetMessage();
		WaveManagedObjectPointer<LldpIntfConfLocalManagedObject> pLldpIntfConfLocalManagedObject;
		if (pPhyIntfLocalManagedObject == NULL) {
       		NSM_PLUG_INFO_S(string("LLDP::pPhyIntfLocalManagedObject == NULL"));
			return;
		}
       	NSM_PLUG_INFO_S(string("LLDP:: ifName ") + pPhyIntfLocalManagedObject->getIfName ());
		pLldpIntfConfLocalManagedObject = WaveManagedObjectPointer<LldpIntfConfLocalManagedObject> (pPhyIntfLocalManagedObject->getLldpIntfConfig());
		LldpGlobalProfileConfManagedObject *pLldpGlobalProfileConfMO = NULL;
		ObjectId profileObjId = ObjectId::NullObjectId;
		WaveManagedObject   *pWaveManagedObject = NULL;
		ResourceId status;
		UI32 cmdcode = 0;
		
		/* Three tupple to two tupple conversion */
		if (true  == NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName ())) {
			string twoTuppleIfName;
			string threeTupple = pPhyIntfLocalManagedObject->getIfName ();
			NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
			m->setIfName (twoTuppleIfName.c_str());
		} else {
			m->setIfName (pPhyIntfLocalManagedObject->getIfName());
		}
        InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
        m->setIfType(ifType);
        NSM_PLUG_INFO_S(string(__FUNCTION__) +
            " LLDP:: ifName: " + m->getIfName() +
            " ifType=" + m->getIfType() + "\n");

		if (pLldpIntfConfLocalManagedObject->getDcbxVersion() != LLDP_DCBX_VERSION_AUTO) {
			m->setDcbxVersion(pLldpIntfConfLocalManagedObject->getDcbxVersion());
			NSM_PLUG_INFO_S (string("LLDP:: DcbxVersion: ") +
							pLldpIntfConfLocalManagedObject->getDcbxVersion());
			cmdcode |= LLDP_INTF_CONF_DCBXVERSION;
		}
		if (pLldpIntfConfLocalManagedObject->getDisable() != LLDP_DEFAULT_DISABLE) {
			m->setDisable(pLldpIntfConfLocalManagedObject->getDisable());
			NSM_PLUG_INFO_S (string("LLDP:: Disable: ") +
							pLldpIntfConfLocalManagedObject->getDisable());
			cmdcode |= LLDP_INTF_CONF_DISABLE;
		}
		if (pLldpIntfConfLocalManagedObject->getIscsiPriority() != DEFAULT_ISCSI_PRIORITY) {
			m->setIscsiPriority(pLldpIntfConfLocalManagedObject->getIscsiPriority());
			NSM_PLUG_INFO_S (string("LLDP:: IscsiPriority: ") +
							pLldpIntfConfLocalManagedObject->getIscsiPriority());
			cmdcode |= LLDP_INTF_CONF_ISCSIPRIORITY;
		}
		if (!(pPhyIntfLocalManagedObject->getDesc()).empty()) {
			m->setDescription(pPhyIntfLocalManagedObject->getDesc());
			NSM_PLUG_INFO_S (string("LLDP:: Description: ") +
							pPhyIntfLocalManagedObject->getDesc());
			cmdcode |= LLDP_INTF_CONF_DESCRIPTION;
		}

		profileObjId = pLldpIntfConfLocalManagedObject->getProfileId();
		if (profileObjId != ObjectId::NullObjectId) {
			pWaveManagedObject = queryManagedObject (profileObjId);
			pLldpGlobalProfileConfMO = dynamic_cast<LldpGlobalProfileConfManagedObject *>(pWaveManagedObject);
			NSM_PLUG_INFO_S (string("LLDP:: profileName ") + pLldpGlobalProfileConfMO->getProfileName());
			m->setProfile(pLldpGlobalProfileConfMO->getProfileName());
			cmdcode = cmdcode | LLDP_INTF_CONF_PROFILE;
		}
		m->setCmdcode(cmdcode);
		m->setMsgType(LLDP_CREATE_CONF);
		
		status = sendSynchronouslyToWaveClient (LLDP_DCM_CLIENT_NAME, m);
		if (status == WAVE_MESSAGE_SUCCESS) {
			NSM_PLUG_INFO_S(string("LLDP:: configuration successful "));
		} else {
			NSM_PLUG_INFO_S(string("LLDP:: configuration failed"));
		}
		delete m;
        if (pLldpGlobalProfileConfMO != NULL) {
            delete pLldpGlobalProfileConfMO;
            pLldpGlobalProfileConfMO = NULL;
        }
	}
	
   void NsmLocalObjectManager::QueryNsmToCreatePhyIntfMO()
    {
        trace (TRACE_LEVEL_DEVEL,"Entering NsmLocalObjectManager::QueryNsmToCreatePhyIntfMO() ...");

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        NsmDefaultConfigurationCmdsMessage *msg_p = NULL;
        UI32 size = 0;
        UI32 outbuffNum;
        void *data;

        msg_p = new NsmDefaultConfigurationCmdsMessage(DEFAULT_INTERFACE);

        trace (TRACE_LEVEL_INFO,"Quering Nsm Backend NsmDefaultConfigurationCmdsMessage(DEFAULT_INTERFACE)");
        status = sendSynchronouslyToWaveClient("nsm", (msg_p));

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace (TRACE_LEVEL_ERROR,"Failed to get interface list from NSM Daemon");
            trace (TRACE_LEVEL_ERROR,string("ERROR=") + status);
            //bpatel: To avoid memory leak
			if(msg_p){
				delete msg_p;
			}
			return;
        }

        outbuffNum = NSM_DEFAULT_CONFIG_RESPONSE;
        msg_p->getDuplicateInterfaceOutputBuffer(outbuffNum,size,data);
        
        if(size != 0)
        {
            switch (outbuffNum) {
                case NSM_DEFAULT_CONFIG_RESPONSE:
                    {
                        nsm_intf_all_msg_t *resp_p;
                        resp_p = (nsm_intf_all_msg_t *)data;
                        u_int32_t i = 0;

                        trace (TRACE_LEVEL_INFO, string("Total no of physical interfaces Received : ") + resp_p->num_records);
                        startTransaction();
                        for (i = 0; i < resp_p->num_records; i++) {
                            
                            trace (TRACE_LEVEL_DEVEL, string(" Interface Name : ") + resp_p->intfdetails[i].ifName );

                            if(strlen(resp_p->intfdetails[i].ifName) > 2)
                            {
                                if((strncmp(resp_p->intfdetails[i].ifName,"te",2) == 0) ||
                                	(strncmp(resp_p->intfdetails[i].ifName,"gi",2) == 0))
                                {
                                    NsmCreatePhyIntfMessage message(resp_p->intfdetails[i].ifName+2,resp_p->intfdetails[i].m_ifIndex,resp_p->intfdetails[i].mtu,resp_p->intfdetails[i].desc);
									UI32 islCapability = resp_p->intfdetails[i].isl_capable;
									message.setIslCapability(islCapability);
                                    m_pIntfConfLocalWorker->createPhyIntfMO(&message,NULL);
                                }
                            }
                            /*
                                    resp_p->intfdetails[i].desc;
                                    resp_p->intfdetails[i].m_ifIndex;
                                    resp_p->intfdetails[i].mtu;
                            */
                        }
                        status = commitTransaction ();
                        if (status == FRAMEWORK_SUCCESS) {
                            status = WAVE_MESSAGE_SUCCESS;
                            trace (TRACE_LEVEL_ERROR,"Success ");
                        } else {
                        trace (TRACE_LEVEL_ERROR,"Committ Failed ");
                        }
                    }
                    break;
            }
        } else {

            trace (TRACE_LEVEL_ERROR, string("Total no of physical interfaces Received :  0 "));
        }
		//bpatel: To avoid memory leak
		if(msg_p){
			delete msg_p;
		}
    } 

   void NsmLocalObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
   {
       NSM_PLUG_INFO_S( string("Entering ...."));
       listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&NsmLocalObjectManager::bootCompleteForThisLocationEventHandler));

       listenForEvent(PrismFrameworkObjectManager::getPrismServiceId(),
			FRAMEWORK_OBJECT_MANAGER_NODE_READY_FOR_ALL_COMMANDS_EVENT,
			reinterpret_cast<PrismEventHandler>(&NsmLocalObjectManager::nodeReadyForAllCommandsEventHandler));

       pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
       pWaveAsynchronousContextForBootPhases->callback ();
   }


   void NsmLocalObjectManager::nodeReadyForAllCommandsEventHandler (const NodeReadyForAllCommandsEvent *&pEvent)
   {
	   trace(TRACE_LEVEL_INFO, "Entering NsmLocalObjectManager:: nodeReadyForAllCommandsEventHandler");
	   m_pIntfConfLocalWorker->setGlobalPortChannelReplayDone (false);

		UI32 gVlanState = GVLAN_STATE_DISABLE;
		Attribute *readConfigAttr = new AttributeUI32(&gVlanState, NSM_GVLAN_PERSISTENCE_MO_NAME);
		bool foundConfig = false;
		string configName(NSM_GVLAN_PERSISTENCE_MO_NAME);
		getWaveConfigEntry(configName, readConfigAttr, foundConfig);

		if(foundConfig)
		{
			string configValue;
			readConfigAttr->getPlainString(configValue);
			gVlanState = atoi(configValue.c_str());

			if(gVlanState == GVLAN_STATE_ENABLE)
			{
				ValClientSynchronousConnection *pValClientSynchronousConnection = DcmToolKit::getInterDcmCcmClient();

				prismAssert(NULL != pValClientSynchronousConnection, __FILE__, __LINE__);

				char featureName[] = NSM_GVLAN_PERSISTENCE_MO_NAME;
				pValClientSynchronousConnection->setHasmFeature(featureName, gVlanState);

				trace(TRACE_LEVEL_INFO, string("NsmLocalObjectManager::nodeReadyForAllCommandsEventHandler:set GVlan feature flag via HASM with the stage:") + gVlanState);
			}
		}
		else
		{
			trace (TRACE_LEVEL_INFO, string ("NsmLocalObjectManager::nodeReadyForAllCommandsEventHandler:GVlan config is not found. No need to update HASM."));
		}

       /* Event reply to framework */
       reply (reinterpret_cast<const PrismEvent *&> (pEvent));
   }

   void NsmLocalObjectManager::bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
   {
       reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));

       NSM_PLUG_INFO_S( string("Entering ...."));
       // Cleanup portRole status of interfaces only when mode is VCS and FC mode.
       // In Logical Chassis mode, this event comes after backendSyncUp which
       // over-writes the correct portRole status in Db which were set during
       // backendSyncUp. Don't cleanup in logical-chassis mode.
       if ((DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) &&
           (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == DcmToolKit::getThisVcsNodeClusterType())) {
           NSM_PLUG_INFO_S( string("Resetting PortRole in Fabric Cluster mode...."));
           m_pIntfConfLocalWorker->setAllPhyMo_PortRole_Edge();
       }

       return;
   }

	/*
     * Sudhanshu: Hercules support for postboot for IGMPS
     */
	void NsmLocalObjectManager::ConfigReplayIgmpsIntfConfig()
	{
		trace (TRACE_LEVEL_INFO, "NsmLocalObjectManager::ConfigReplayIgmpsIntfConfig: Entered....");

		WaveManagedObjectSynchronousQueryContext vlanSyncQueryCtxt(VlanIntfManagedObject::getClassName());
		vlanSyncQueryCtxt.setLoadOneToManyRelationships(false);
		vlanSyncQueryCtxt.setLoadCompositions(false);
		vlanSyncQueryCtxt.addSelectFieldForObjectId("objectId");
		vlanSyncQueryCtxt.addSelectField("id");
		vector<WaveManagedObject *> *pVlanResults = querySynchronously (&vlanSyncQueryCtxt);
		configReplayIgmpsIntfConfig(pVlanResults);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pVlanResults);

		trace (TRACE_LEVEL_INFO, "NsmLocalObjectManager::ConfigReplayIgmpsIntfConfig: Ended....");

	}

	void NsmLocalObjectManager::configReplayIgmpsIntfConfig(vector<WaveManagedObject *>* &pVlanResults) {

		if (NULL == pVlanResults) {
			return;
		}

		UI32 numberOfResults = pVlanResults->size ();
		if (0 == numberOfResults) {
			return;
		}

		ResourceId status = WAVE_MESSAGE_ERROR;

		VlanIntfManagedObject *pVlanIntfManagedObject = NULL;
		ObjectId vlanObjectId = ObjectId::NullObjectId;
		UI32 vlanId = 0;
		vector<ObjectId> vlanObjectIds;
		map<ObjectId, UI32> vlanIdObjectIdMap;
		for (UI32 i = 0; i < numberOfResults; i++) {
			pVlanIntfManagedObject  = dynamic_cast<VlanIntfManagedObject *>((*pVlanResults)[i]);
			vlanObjectId = pVlanIntfManagedObject->getObjectId();
			vlanId = pVlanIntfManagedObject->getId();
			vlanIdObjectIdMap[vlanObjectId] = vlanId;
		}

		WaveManagedObjectSynchronousQueryContext mrtrSyncQueryCtxt(IgmpsMrtrVlanConfigManagedObject::getClassName());
		vector<WaveManagedObject *> *pMrtrResultsPMO = querySynchronously (&mrtrSyncQueryCtxt);

		if (NULL != pMrtrResultsPMO) {
			numberOfResults = pMrtrResultsPMO->size();
			trace(TRACE_LEVEL_DEVEL, string("NsmLocalObjectManager::ConfigReplayIgmpsIntfConfig: Mrtr NumResults - ")+numberOfResults);

			IgmpsMrtrVlanConfigManagedObject *pIgmpsMrtrVlanConfigManagedObject = NULL;

			for (UI32 j = 0; j < numberOfResults; j++) {
				pIgmpsMrtrVlanConfigManagedObject = dynamic_cast<IgmpsMrtrVlanConfigManagedObject *>((*pMrtrResultsPMO)[j]);
				if (! pIgmpsMrtrVlanConfigManagedObject)
					continue;

				/* Processing should be done only for fortygigabit/tengigabit/onegigabit ethernet nodes */
				if(IGMPS_IF_TYPE_FORTYGIGABIT != pIgmpsMrtrVlanConfigManagedObject->getIfType() &&
				   (IGMPS_IF_TYPE_TENGIGABIT != pIgmpsMrtrVlanConfigManagedObject->getIfType()) &&
				   (IGMPS_IF_TYPE_GIGABIT != pIgmpsMrtrVlanConfigManagedObject->getIfType()))
				{
					continue;
				}

                IgmpsClientUpdateMrtrVlanConfigMessage *m = new IgmpsClientUpdateMrtrVlanConfigMessage ();

				m->setVlanId(vlanIdObjectIdMap[pIgmpsMrtrVlanConfigManagedObject->getOwnerManagedObjectId()]);
				m->setIfType(pIgmpsMrtrVlanConfigManagedObject->getIfType());
				m->setIfName(pIgmpsMrtrVlanConfigManagedObject->getIfName());
                m->setNegation(false);

                if(true == NsmUtils::isValidThreeTuple(m->getIfName())) {
                    LocationId locationId = NsmUtils::getLocationId(m->getIfName());
                    if (locationId == NsmUtils::getLocalLocationId()) {
                        string twoTuppleIfName;
                        NsmUtils::getTwoTupleIfName(m->getIfName(), twoTuppleIfName);
                        m->setIfName(twoTuppleIfName);
                    } else {
                        /* If this command does not belong to this node, discard.
                         * It will be replayed by corresponding node.
                         */
                        delete m;
                        continue;
                    }
                }

				trace (TRACE_LEVEL_INFO, string("NsmLocalObjectManager::ConfigReplayIgmpsIntfConfig: Replay Mr ") + m->getIfType()   + m->getIfName());
                status = sendSynchronouslyToWaveClient ("IGMP", m);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    trace(TRACE_LEVEL_SUCCESS, string("NsmLocalObjectManager::ConfigReplayIgmpsIntfConfig: sendToClient success"));
                } else {
                    trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::ConfigReplayIgmpsIntfConfig: sendToClient failed"));
                }
                delete m;

			}
			if (pMrtrResultsPMO->size() > 0)
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pMrtrResultsPMO);
		}

		WaveManagedObjectSynchronousQueryContext sgSyncQueryCtxt(IgmpsSgVlanConfigManagedObject::getClassName());
		vector<WaveManagedObject *> *pSgResultsPMO = querySynchronously (&sgSyncQueryCtxt);

		if(NULL != pSgResultsPMO) {
			numberOfResults = pSgResultsPMO->size();
			trace(TRACE_LEVEL_DEVEL, string("NsmLocalObjectManager::ConfigReplayIgmpsIntfConfig: Static Group NumResults - ")+numberOfResults);

			IgmpsSgVlanConfigManagedObject *pIgmpsSgVlanConfigManagedObject = NULL;
			for (UI32 j = 0; j < numberOfResults; j++) {
				pIgmpsSgVlanConfigManagedObject = dynamic_cast<IgmpsSgVlanConfigManagedObject *>((*pSgResultsPMO)[j]);
				if (! pIgmpsSgVlanConfigManagedObject)
					continue;

				if((IGMPS_IF_TYPE_FORTYGIGABIT != pIgmpsSgVlanConfigManagedObject->getIfType()) &&
				   (IGMPS_IF_TYPE_TENGIGABIT != pIgmpsSgVlanConfigManagedObject->getIfType()) &&
				   (IGMPS_IF_TYPE_GIGABIT != pIgmpsSgVlanConfigManagedObject->getIfType()))
				{
					continue;
				}
				IgmpsClientUpdateSgVlanConfigMessage *m = new IgmpsClientUpdateSgVlanConfigMessage ();

				m->setVlanId(vlanIdObjectIdMap[pIgmpsSgVlanConfigManagedObject->getOwnerManagedObjectId()]);
				m->setGroupAddress(pIgmpsSgVlanConfigManagedObject->getGroupAddress());
				m->setIfType(pIgmpsSgVlanConfigManagedObject->getIfType());
				m->setIfName(pIgmpsSgVlanConfigManagedObject->getIfName());
                m->setNegation(false);
                if(true == NsmUtils::isValidThreeTuple(m->getIfName())) {
                    LocationId locationId = NsmUtils::getLocationId(m->getIfName());
                    if (locationId == NsmUtils::getLocalLocationId()) {
                        string twoTuppleIfName;
                        NsmUtils::getTwoTupleIfName(m->getIfName(), twoTuppleIfName);
                        m->setIfName(twoTuppleIfName);

                    } else {
                        /* If this command does not belong to this node, discard.
                         * It will be replayed by corresponding node.
                         */
                        delete m;
                        continue;
                    }
                }

				trace (TRACE_LEVEL_INFO, string("NsmLocalObjectManager::ConfigReplayIgmpsIntfConfig: Replay Sg")+ m->getIfType()   + m->getIfName());
                status = sendSynchronouslyToWaveClient ("IGMP", m);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    trace(TRACE_LEVEL_SUCCESS, string("NsmLocalObjectManager::ConfigReplayIgmpsIntfConfig:sendToClient success"));
                } else {
                    trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::ConfigReplayIgmpsIntfConfig:sendToClient failed"));
                }
                delete m;
            }	
			if (pSgResultsPMO->size() > 0)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pSgResultsPMO);
		}

		return;
    }

    void NsmLocalObjectManager::postBootPassShutdownPhyConfiguration()
    {
        trace (TRACE_LEVEL_DEBUG, "Entering NsmLocalObjectManager::postBootPassShutdownPhyConfiguration ..");
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
        LocationId                          locationId = 0;
        locationId = FrameworkToolKit::getThisLocationId ();
        vector<WaveManagedObject *> *pPhyResults = querySynchronouslyLocalManagedObjectsForLocationId (locationId, PhyIntfLocalManagedObject::getClassName ());
        if (pPhyResults && pPhyResults->size() > 0) {
            for (UI32 PhyIndex = 0; PhyIndex < pPhyResults->size(); PhyIndex++)
            {
                pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pPhyResults)[PhyIndex]);
                PassShutdownPhyConfiguration(pPhyIntfLocalManagedObject);
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pPhyResults);
        }
    }

    void NsmLocalObjectManager::configReplayEnd (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd)
    {
        NSM_PLUG_INFO_S( string("Entering ...."));
            //work around to unblock sanity, need to use right API to 
            //blcok shut/no shut getting played twice on interface
            //if(FrameworkToolKit::getIsConfigFileReplayGoingToBeDone ()) {
			    postBootPassShutdownPhyConfiguration ();
			//}
        pWaveAsynchronousContextForConfigReplayEnd->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForConfigReplayEnd->callback ();
            
    }
    
    void NsmLocalObjectManager::slotFailover (
        WaveAsynchronousContextForSlotFailover 
        *pWaveAsynchronousContextForSlotFailover)
    {
        string func_name = "NsmLocalObjectManager::slotFailover() ";
        int32_t slotId = pWaveAsynchronousContextForSlotFailover->
            getSlotNumber();
        LocationId locId = FrameworkToolKit::getThisLocationId();

        trace (TRACE_LEVEL_INFO, func_name + " Entered with " +
            " SlotId = " + slotId +
            " LocationId = " + locId);
         
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
        vector<PhyIntfLocalManagedObject *> results;
         
        NsmUtils::SlotGetPhyIntfMOs(results, slotId, locId);

        trace (TRACE_LEVEL_INFO, func_name + " Found " + results.size() +
            " Interfaces.");

        // Cleanup Connector MOs associated with given slotId
        ResourceId isVcsEnabled = DcmToolKit::isVcsEnabled ();
        string regex;

        if(isVcsEnabled == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            // if the local mappedId is 2 and the slot id is 3, then we query for all the connector MOs with connectorName "2/3/xxx"
            regex = getString(DcmToolKit::getLocalMappedId()) + string("/") + getString(slotId) + string("/%");
        } else {
            // if the slot id is 3, then we query for all the connector MOs with connectorName "3/xxx"
            regex = getString(slotId) + string("/%");
        }
        WaveManagedObjectSynchronousQueryContextForDeletion *connectorDelSyncQueryCtxt = new WaveManagedObjectSynchronousQueryContextForDeletion (ConnectorConfigLocalManagedObject::getClassName());
        connectorDelSyncQueryCtxt->addAndAttribute(new AttributeString(regex, "connectorName"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);

        IntfXstpFailoverContext xstpContext;

        startTransaction();
        // Delete Connector MOs
        deleteMultipleWaveManagedObjects (connectorDelSyncQueryCtxt);

        for (UI32 i = 0; i < results.size(); i++) {
            pPhyIntfLocalManagedObject = results[i];

            xstpContext.recordPhyIntfDelete(pPhyIntfLocalManagedObject);

            trace (TRACE_LEVEL_DEBUG, func_name + " Removing Intf: " +
                pPhyIntfLocalManagedObject->getIfName()); 

            delete pPhyIntfLocalManagedObject;
        }
        ResourceId commitStatus = commitTransaction();
        if (FRAMEWORK_SUCCESS != commitStatus) {
            trace (TRACE_LEVEL_FATAL, "NsmLocalObjectManager::slotFailover can not commit local managed object delete to database ");
        }


        m_pIntfXstpLocalConfWorker->failover(xstpContext);

        pWaveAsynchronousContextForSlotFailover->
            setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForSlotFailover->callback();

        trace (TRACE_LEVEL_INFO, func_name + " DONE! " +
            " SlotId = " + slotId +
            " LocationId = " + locId);
    }

     void NsmLocalObjectManager::warmRecoveryPostBoot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
	 {
		trace (TRACE_LEVEL_INFO, string("warmRecoveryPostBoot for NsmLocalObjectManager"));

        string pass_name = pWaveAsynchronousContextForPostBootPhase-> getPassName();
        int32_t slotId = pWaveAsynchronousContextForPostBootPhase-> getSlotNum();
        LocationId locId = FrameworkToolKit::getThisLocationId();
		trace (TRACE_LEVEL_INFO, string("pass_name:") + pass_name + string("SlotId= ") + slotId + string(" LocationId= ") + locId);

		if (pass_name == "DCM_POSTBOOT_GLOBAL_STAGE2") {
            const NsmIpConfigManagedObject *pIpConfigMo = NULL;
            NsmIntfSviManagedObject *pIntfSviManagedObject = NULL;
            vector<NsmIntfSviManagedObject *> results;
            string ifName;

            NsmL3Util::NodeVlanIfGetMOs(results, locId);

            for(unsigned int i = 0; i < results.size(); i++) {
                pIntfSviManagedObject = results[i];

                if (!pIntfSviManagedObject) {
                    continue;
                }
                ifName = toString(pIntfSviManagedObject->getIfId());
                pIpConfigMo = pIntfSviManagedObject->getIpConfigMO();
                if (pIpConfigMo) {
                    trace (TRACE_LEVEL_INFO,string("Reply svi configuration for:") + ifName);
                    L3PostBootUtil::configReplayIntfArpConfig(pIpConfigMo, ifName, VLAN_INT);
                }
                delete pIntfSviManagedObject;
            }
        }
		else if (pass_name == "DCM_POSTBOOT_SLOT_STAGE1") {
            setAllowIslToEdge(true);

            PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
            vector<PhyIntfLocalManagedObject *> results;

            NsmUtils::SlotGetPhyIntfMOs(results, slotId, locId);
            trace (TRACE_LEVEL_INFO, "number of phy-intf found: " + results.size());

            for (UI32 i = 0; i < results.size(); i++)
            {
                pPhyIntfLocalManagedObject = results[i];

                if(pPhyIntfLocalManagedObject->getPortrole() == false)
                {
                    trace (TRACE_LEVEL_INFO,string("Reply configuration Edge Port :") + pPhyIntfLocalManagedObject->getIfName());

                    const NsmIpConfigManagedObject *pIpConfigMo = pPhyIntfLocalManagedObject->getIpConfigMO();
                    if (pIpConfigMo)
                    {
                        L3PostBootUtil::configReplayIntfArpConfig(pIpConfigMo, pPhyIntfLocalManagedObject->getIfName(),
                        pPhyIntfLocalManagedObject->getIfType());
                    }
                    (VRRPLocalObjectManager::getInstance())->configReplayVRRPPhyIntf(pPhyIntfLocalManagedObject);
                    (NsmLocalObjectManager::getInstance())->configReplayPhyOSPFConfig(pPhyIntfLocalManagedObject);
                    (NsmLocalObjectManager::getInstance())->configReplayRmonCmds(pPhyIntfLocalManagedObject);
                    (PimLocalObjectManager::getInstance())->configReplayPimPhyIntf(pPhyIntfLocalManagedObject);
                    (IgmpLocalObjectManager::getInstance())->replayIgmpPhyConfig(pPhyIntfLocalManagedObject);
                    (DhcpRelLocalObjectManager::getInstance())->replayDhcpRelPhyIntfConfig(pPhyIntfLocalManagedObject);
                }
                delete pPhyIntfLocalManagedObject;
            }
        }
        return;
	 }
    
     void NsmLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
		UI32 recoveryType = pWaveAsynchronousContextForPostBootPhase->getRecoveryType();
		if (recoveryType == WRC_WAVE_HA_WARM_RECOVERY )
		{
			warmRecoveryPostBoot(pWaveAsynchronousContextForPostBootPhase);
			pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
			pWaveAsynchronousContextForPostBootPhase->callback ();
			return;
		}
        string func_name = "NsmLocalObjectManager::postboot() ";
        // SI8 PassNum  = pWaveAsynchronousContextForPostBootPhase->getPassNum();
        string pass_name = pWaveAsynchronousContextForPostBootPhase->
            getPassName();
        int32_t slotId = pWaveAsynchronousContextForPostBootPhase->
            getSlotNum();
        LocationId locId = FrameworkToolKit::getThisLocationId();

        trace (TRACE_LEVEL_INFO, func_name + " Entered with pass: " +
            pass_name + " SlotId = " + slotId +
            " LocationId = " + locId);

        if (pass_name == "DCM_POSTBOOT_GLOBAL_STAGE1") {
            postBootConfigureVlanClassifierRules();
            postBootConfigureVlanClassifierGroup();
            postBootConfigureLacpGlobalConfig();
            postBootConfigureNativeVlanGlobalConfig();
            postBootConfigureDot1xGlobalConfig();
			configReplayMacGroup();
        }
        else if (pass_name == "DCM_POSTBOOT_GLOBAL_STAGE2") {

            configReplayForVlan();

			//moved to PostbootUtil
			L3PostBootUtil::ConfigReplayIntfLoopbackIpConfig(locId);

            L3PostBootUtil::ConfigReplayVlanIpConfig(locId);
            if (WAVE_PERSISTENCE_CLUSTER_ENABLED !=
                DcmToolKit::isVcsEnabled())
            {
                L3PostBootUtil::ConfigReplayPoIpConfig(locId);
            }
			if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled())
			postBootConfigureEcmpGlobalConfig();
				
        }
        else if (pass_name == "DCM_POSTBOOT_GLOBAL_STAGE3") {

            vector<PoIntfManagedObject *> poMOs;
            NsmUtils::GetPoIntfMOs(poMOs);

            if (!poMOs.empty()) {
                postBootConfigureVlanClassifierGroupToPo(poMOs);
                //ConfigReplayMacAclIntfAssoc(VLAN_INTF_TYPE, "all");
                //ConfigReplayMacAclIntfAssoc(PO_INTF_TYPE, "all");

                postbootXstpConfig(poMOs);
                postBoot3PoEldConfiguration(poMOs);
                for (UI32 i = 0; i < poMOs.size(); i++) {
                    delete poMOs[i];
                }
            }

            /*
             * Sudhanshu: Hercules support for postboot()
             */
            // ConfigReplayIgmpsIntfConfig(pVlanResults);
        }
        else if (pass_name == "DCM_POSTBOOT_SLOT_STAGE1") {

            // ConfigReplayLacpIntfConfig(slotId, locId);
            // postBootPassShutdownPhyConfiguration(slotId, locId);

            postBootPhyConfiguration(slotId, locId);

            //ConfigReplayPhyIpConfig(slotId, locId);

            setAllowIslToEdge(true);
			postBootPassLongDistanceConfiguration();
        }
        else if (pass_name == "DCM_POSTBOOT_SLOT_END") {
            if (m_pIntfConfLocalWorker) {
                m_pIntfConfLocalWorker->notifySlotReplayCompleted(slotId);
            }
        }
        else {
            trace (TRACE_LEVEL_ERROR, func_name + 
                " Not supported pass_name: " + pass_name);
        }
        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();

        return;
    }

     void NsmLocalObjectManager::configReplayForVlan(const SI32 slotId) {
        const char *func = "NsmLocalObjectManager::configReplayForVlan: ";
        WaveManagedObjectSynchronousQueryContext vlanSyncQueryCtxt(
                VlanIntfManagedObject::getClassName());
        vector<WaveManagedObject *> *pVlanResults = NULL;
        VlanInformation vlanInformation;
        UI32Range diffToProvision;
        if (ProvisioningUtil::isGvlanEnabled()) {
            ProvisioningUtil::addProvisionedVlan(NsmUtils::DEFAULT_DOT1Q_VLAN);
            tracePrintf(TRACE_LEVEL_INFO, "%s populating vlan info for slot %d",
                    func, slotId);
            NsmUtils::populateVlanInformation(vlanInformation, slotId);
            if (!vlanInformation.provisionedVlans.isEmpty()) {
                diffToProvision = vlanInformation.provisionedVlans -
                        ProvisioningUtil::getProviosionedMORange();
                if (!diffToProvision.isEmpty()) {
                    vlanSyncQueryCtxt.setAttributeRangeForQuery(
                        new AttributeUI32Range(diffToProvision, "id"));
                    tracePrintf(TRACE_LEVEL_INFO,
                            "%s Querying slot %d vlans %s to be provisioned for ",
                            func, slotId, diffToProvision.toString().c_str());
                    pVlanResults = querySynchronously(&vlanSyncQueryCtxt);
                }
            }
        } else {
            pVlanResults = querySynchronously(&vlanSyncQueryCtxt);
        }
        if ( pVlanResults == NULL || pVlanResults->size() == 0 ) {
            tracePrintf(TRACE_LEVEL_INFO,
                    "%s No vlans to be provisioned", func);
        } else {

            tracePrintf(TRACE_LEVEL_INFO,
                    "%s Provisioning %d vlans", func, pVlanResults->size());
            postBootPass1Configuration(pVlanResults, slotId == -1);

            ConfigReplayIgmpsVlanIntfConfig(pVlanResults);
            ConfigReplayIgmpsMrtrVlanConfig(pVlanResults);
            ConfigReplayIgmpsSgVlanConfig(pVlanResults);
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pVlanResults);
        }
        //  Tracking all the VLANs as provisioned irrespective of the fact
        // whether or not it succeeded ....TODO: Need to change
        if (ProvisioningUtil::isGvlanEnabled()) {
            if (!diffToProvision.isEmpty()) {
                ProvisioningUtil::addProvisionedVlan(
                        diffToProvision);
            }
            if (!vlanInformation.unProvisionedVlans.isEmpty()) {
                tracePrintf(TRACE_LEVEL_INFO,
                        "%s playing config-only vlan info", func);
                GvlanConfigUtils util(NULL);
                ResourceId provisionStatus = util.provision(
                        vlanInformation.unProvisionedVlans,
                        PROVISION_NOT_REQUIRED);
                if (WAVE_MESSAGE_SUCCESS != provisionStatus) {
                    tracePrintf(
                            TRACE_LEVEL_ERROR,
                            "Failed to configure Vlans %s status=%u",
                            vlanInformation.unProvisionedVlans.toString().c_str(),
                            provisionStatus);
                }
            }
        }
    }

     void NsmLocalObjectManager::postBoot3PoEldConfiguration(vector<PoIntfManagedObject *> &poMOs) {
        trace(TRACE_LEVEL_DEBUG, "Entering NsmLocalObjectManager::postBoot3PoEldConfiguration ..");
        PoIntfManagedObject *pPoIntfManagedObject = NULL;
        for (UI32 poIndex = 0; poIndex < poMOs.size(); poIndex++) {
            pPoIntfManagedObject = poMOs[poIndex];
            LocationId locationId = FrameworkToolKit::getThisLocationId();
            if ((NsmUtils::isPoIntfHasMemberByLocation(pPoIntfManagedObject->getObjectId(), locationId) == WAVE_MESSAGE_SUCCESS)
                    || (WAVE_PERSISTENCE_CLUSTER_ENABLED != DcmToolKit::isVcsEnabled())) {
                (NsmLocalObjectManager::getInstance())->configReplayEldPcInteface(pPoIntfManagedObject);
            } else {
                NSM_PLUG_INFO_S(string(
                    "Port-channel interface is not configured : No Memebers(LocationId :") +
                    locationId + string(")in .. PoName :") + pPoIntfManagedObject->getIfName());
            }

        }
    }

#if 0

  ResourceId NsmLocalObjectManager::getPoMoByPoId(UI32 PoId, PoIntfManagedObject * * ppPoIntfManagedObject)
    {
        ResourceId                               status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());

        syncQueryCtxt.addAndAttribute(new AttributeUI32(&PoId, "id"));

        vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);

        if (pResults && (pResults -> size() == 1))
        {
            *ppPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
            status                 = WAVE_MESSAGE_SUCCESS;
        }

        // delete  pPoIntfManagedObject;
        return status;
    }

#endif


  void NsmLocalObjectManager:: configReplayRmonStats(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
  {
	  NSM_PLUG_DBG("Entering ...");
	  char buffer[IFNAME_MAX_STR + 1];
	  buffer[IFNAME_MAX_STR] = '\0';

	  if(NULL == pPhyIntfLocalManagedObject)
	  {
		  NSM_PLUG_DBG("Invalid pPhyIntfLocalManagedObject");
		  return;
	  }

	  string ifName = pPhyIntfLocalManagedObject->getIfName();
	  if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
	  {
		  NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
	  }
      InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
      NSM_PLUG_INFO_S(string(__FUNCTION__) +
          " ifName: " + ifName + " ifType=" + ifType + "\n");

	  GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, IFNAME_MAX_STR);
	  ifName = buffer;
	 // ifName.insert(0, "te");
	  NSM_PLUG_DBG_S(string("Configuring the interface") + ifName);
	  vector<WaveManagedObjectPointer<RmonStatisticsLocalManagedObject> > rmonStatsMOVector =  pPhyIntfLocalManagedObject->getRmonStats();
	  unsigned int size = rmonStatsMOVector.size();
	  unsigned int j;
	  for (j = 0; j < size; j++)
	  {
		  RmonClientCreateCollectionStatisticsMessage *m = new RmonClientCreateCollectionStatisticsMessage();
		  m->setIfName(ifName);
		  m->setStatsIndex(rmonStatsMOVector[j]->getStatsIndex());
		  m->setOwner(rmonStatsMOVector[j]->getOwner());
		  NSM_PLUG_DBG("Sending RmonStats");
		  sendSynchronouslyToWaveClient ("rmon", m);
		  delete m;
	  }

  }
  void NsmLocalObjectManager::configReplayRmonStats()
  {	
	  NSM_PLUG_DBG("Entering configReplayRmonStats");
	  WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
	  vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

		PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject = NULL;
		if (pResults != NULL)
		{		
	        if (pResults->size() > 0)
    	    {
        	    for(unsigned int i = 0;i <  pResults->size(); i++)
            	{
                	pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[i]);
					if (pPhyIntfLocalManagedObject != NULL)
					{
						string ifName = pPhyIntfLocalManagedObject->getIfName();
	  					char buffer[IFNAME_MAX_STR + 1];
	  					buffer[IFNAME_MAX_STR] = '\0';
						if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
						{
							NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
						}

                        InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
                        NSM_PLUG_INFO_S(string(__FUNCTION__) +
                            " ifName: " + ifName + " ifType=" + ifType + "\n");

	  					GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, IFNAME_MAX_STR);
	  					ifName = buffer;
						//ifName.insert(0, "te");
						NSM_PLUG_DBG_S(string("Configuring the interface") + ifName);
						vector<WaveManagedObjectPointer<RmonStatisticsLocalManagedObject> > rmonStatsMOVector =  pPhyIntfLocalManagedObject->getRmonStats();
                	    unsigned int size = rmonStatsMOVector.size();
						unsigned int j;
						for (j = 0; j < size; j++)
						{
							RmonClientCreateCollectionStatisticsMessage *m = new RmonClientCreateCollectionStatisticsMessage();
							m->setIfName(ifName);
							m->setStatsIndex(rmonStatsMOVector[j]->getStatsIndex());
							m->setOwner(rmonStatsMOVector[j]->getOwner());
							NSM_PLUG_DBG("Sending RmonStats");
							sendSynchronouslyToWaveClient ("rmon", m);
							delete m;
						}
					}
    	        }
        	    if(pResults)
            	{
                	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    	        }
        	}
		}
        return ;
    }
  void NsmLocalObjectManager::configReplayRmonHistory(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
  {
	  char buffer[IFNAME_MAX_STR + 1];
	  buffer[IFNAME_MAX_STR] = '\0';
	  NSM_PLUG_DBG("Entering ...");
	  if(NULL == pPhyIntfLocalManagedObject)
	  {
		  NSM_PLUG_DBG("Invalid pPhyIntfLocalManagedObject");
		  return;
	  }
	  string ifName = pPhyIntfLocalManagedObject->getIfName();
	  if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
	  {
		  NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
	  }
      InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
          NSM_PLUG_INFO_S(string(__FUNCTION__) +
          " ifName: " + ifName + " ifType=" + ifType + "\n");

	  GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, IFNAME_MAX_STR);
	  ifName = buffer;
	  //ifName.insert(0, "te");
	  NSM_PLUG_DBG_S(string("Configuring the interface") + ifName);
	  vector<WaveManagedObjectPointer<RmonHistoryLocalManagedObject> > rmonHistoryMOVector =  pPhyIntfLocalManagedObject->getRmonHistory();
	  unsigned int size = rmonHistoryMOVector.size();
	  unsigned int j;
	  for (j = 0; j < size; j++)
	  {
		  RmonClientCreateCollectionHistoryMessage *m = new RmonClientCreateCollectionHistoryMessage();
		  m->setIfName(ifName);
		  m->setHistoryIndex(rmonHistoryMOVector[j]->getHistoryIndex());
		  m->setBuckets(rmonHistoryMOVector[j]->getBuckets());
		  m->setInterval(rmonHistoryMOVector[j]->getInterval());
		  m->setOwner(rmonHistoryMOVector[j]->getOwner());
		  NSM_PLUG_DBG("Sending RmonStats");
		  sendSynchronouslyToWaveClient ("rmon", m);
		  delete m;
	  }
  }

    void NsmLocalObjectManager::configReplayRmonHistory()
    {
        NSM_PLUG_DBG("Entering configReplayRmonHistory");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

		PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject = NULL;
		if (pResults != NULL)
		{		
	        if (pResults->size() > 0)
    	    {
        	    for(unsigned int i = 0;i <  pResults->size(); i++)
            	{
                	pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[i]);
					if (pPhyIntfLocalManagedObject != NULL)
					{
	  					char buffer[IFNAME_MAX_STR + 1];
	  					buffer[IFNAME_MAX_STR] = '\0';
						string ifName = pPhyIntfLocalManagedObject->getIfName();
						if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
						{
							NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
						}

                        InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
                            NSM_PLUG_INFO_S(string(__FUNCTION__) +
                            " ifName: " + ifName + " ifType=" + ifType + "\n");

	  					GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, IFNAME_MAX_STR);
	  					ifName = buffer;
						//ifName.insert(0, "te");
						NSM_PLUG_DBG_S(string("Configuring the interface") + ifName);
						vector<WaveManagedObjectPointer<RmonHistoryLocalManagedObject> > rmonHistoryMOVector =  pPhyIntfLocalManagedObject->getRmonHistory();
                	    unsigned int size = rmonHistoryMOVector.size();
						unsigned int j;
						for (j = 0; j < size; j++)
						{
							RmonClientCreateCollectionHistoryMessage *m = new RmonClientCreateCollectionHistoryMessage();
							m->setIfName(ifName);
							m->setHistoryIndex(rmonHistoryMOVector[j]->getHistoryIndex());
							m->setBuckets(rmonHistoryMOVector[j]->getBuckets());
							m->setInterval(rmonHistoryMOVector[j]->getInterval());
							m->setOwner(rmonHistoryMOVector[j]->getOwner());
							NSM_PLUG_DBG("Sending RmonStats");
							sendSynchronouslyToWaveClient ("rmon", m);
							delete m;
						}
					}
    	        }
        	    if(pResults)
            	{
                	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    	        }
        	}
		}
        return ;
    }

	void NsmLocalObjectManager::configReplayRmonAlarm(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{
		NSM_PLUG_DBG("Entering ...");

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonAlarmManagedObject::getClassName());
		vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);
        vector<WaveManagedObjectPointer<RmonStatisticsLocalManagedObject> > m_rmonStats = pPhyIntfLocalManagedObject->getRmonStats();  

		RmonAlarmManagedObject * pRmonAlarmManagedObject = NULL;
		if (NULL != pResults && pResults->size() > 0)
		{
			for(unsigned int j = 0;j< m_rmonStats.size();j++)
			{
				for(unsigned int i = 0;i <  pResults->size(); i++)
				{
					pRmonAlarmManagedObject = dynamic_cast<RmonAlarmManagedObject *>((*pResults)[i]);
					if(m_rmonStats[j]->getStatsIndex() != getEtherStatsIndexFromAlarmOid(pRmonAlarmManagedObject->getSnmpOid()) )  
					{
						NSM_PLUG_DBG_S(string("stats id ") + m_rmonStats[j]->getStatsIndex() + string("used for rmon alarm") );
						continue;
					} 
					NSM_PLUG_DBG_S(string("stats id ") + m_rmonStats[j]->getStatsIndex() + string("is  used for rmon alarm") );

					RmonClientCreateRmonAlarmMessage *m = new RmonClientCreateRmonAlarmMessage();

					m->setAlarmIndex(pRmonAlarmManagedObject->getAlarmIndex());
					m->setSnmpOid(pRmonAlarmManagedObject->getSnmpOid());
					m->setInterval(pRmonAlarmManagedObject->getInterval());
					m->setSampleType(pRmonAlarmManagedObject->getSampleType());
					m->setRisingThreshold(pRmonAlarmManagedObject->getRisingThreshold());
					m->setRisingEventId(pRmonAlarmManagedObject->getRisingEventId());
					m->setFallingThreshold(pRmonAlarmManagedObject->getFallingThreshold());
					m->setFallingEventId(pRmonAlarmManagedObject->getFallingEventId());
					m->setOwner(pRmonAlarmManagedObject->getOwner());
					RMON_PLUGIN_TRACE_DEBUG(string("Sending RmonClientCreateRmonAlarmMessage to rmon Client\n"));
					sendSynchronouslyToWaveClient ("rmon", m);
					delete m;
				}
            }
			if(pResults)
			{
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			}
		}
		return ;
	}
    
	void NsmLocalObjectManager::configReplayRmonCmds(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{
        configReplayRmonHistory(pPhyIntfLocalManagedObject);
        configReplayRmonStats(pPhyIntfLocalManagedObject);
		configReplayRmonAlarm(pPhyIntfLocalManagedObject);
	}

    void NsmLocalObjectManager::configReplayPhyQosMaps(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        NSM_PLUG_DBG("Entering::configReplayPhyQosMaps");
	  	char buffer[IFNAME_MAX_STR + 1];
	  	buffer[IFNAME_MAX_STR] = '\0';
		if(NULL == pPhyIntfLocalManagedObject)
		{
			NSM_PLUG_DBG("PhyIntfLocalManagedObject is not valid");
			return;
		}
		if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
		{
			return;
		}
		string ifName;
	
		if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
		{
			NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
		}
		else
		{
			ifName = pPhyIntfLocalManagedObject->getIfName();
		}
        InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
            NSM_PLUG_INFO_S(string(__FUNCTION__) +
            " ifName: " + ifName + " ifType=" + ifType + "\n");

	  	GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, IFNAME_MAX_STR);
	  	ifName = buffer;
		//ifName.insert(0, "te");
		NSM_PLUG_DBG_S(string("Configuring the interface") + ifName);
	
		if(pPhyIntfLocalManagedObject->getQosCosMap() != ObjectId::NullObjectId)
		{   
			ObjectId cosMapObjectId;
			cosMapObjectId = pPhyIntfLocalManagedObject->getQosCosMap();		
			CosToCosMutationMapManagedObject *pCosObject;
			pCosObject = getCosToCosMutationMapMOByObjectId(cosMapObjectId);	
			if(pCosObject != NULL)
			{
				QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
				m->setIfName(ifName);
				m->setOpCode(CONFIGURE_INTERFACE_QOS);
				m->setCosToCosMutationMapName(pCosObject->getCosToCosMutationMapName());
				m->setCmdCode(QOS_INTERFACE_COS_MUTATION);
				NSM_PLUG_DBG("Sending CosToCosMutationMap");
				sendSynchronouslyToWaveClient ("qos",m);
				delete m;
                delete pCosObject;
			}
		}
		if(pPhyIntfLocalManagedObject->getQosTrafficClassMap() != ObjectId::NullObjectId)
		{
			ObjectId cosMapObjectId;
			cosMapObjectId = pPhyIntfLocalManagedObject->getQosTrafficClassMap();
			CosToTrafficClassMapManagedObject *pCosObject;
			pCosObject = getCosToTrafficClassMapMOByObjectId(cosMapObjectId);
			if(pCosObject != NULL)
			{	
				QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
				m->setIfName(ifName);
				m->setOpCode(CONFIGURE_INTERFACE_QOS);
				m->setCosToTrafficClassMapName(pCosObject->getCosToTrafficClassMapName());
				m->setCmdCode(QOS_INTERFACE_COS_TRAFFIC_CLASS);
				NSM_PLUG_DBG("Sending CosToCosTrafficClassMap");
				sendSynchronouslyToWaveClient ("qos",m);
				delete m;
                delete pCosObject;
			}
		}
		if(pPhyIntfLocalManagedObject->getQosDscpMap() != ObjectId::NullObjectId)
        {
            ObjectId dscpMapObjectId;
            dscpMapObjectId = pPhyIntfLocalManagedObject->getQosDscpMap();
            DscpToDscpMutationMapManagedObject *pDscpObject;
            pDscpObject = getDscpToDscpMutationMapMOByObjectId(dscpMapObjectId);
            if(pDscpObject != NULL)
            {
                QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
                m->setIfName(ifName);
                m->setOpCode(CONFIGURE_INTERFACE_QOS);
                m->setDscpToDscpMutationMapName(pDscpObject->getDscpToDscpMutationMapName());
                m->setCmdCode(QOS_INTERFACE_DSCP_MUTATION);
                NSM_PLUG_DBG("Sending DscpToDscpMutationMap");
                sendSynchronouslyToWaveClient ("qos",m);
                delete m;
                delete pDscpObject;
            }
        }
        if(pPhyIntfLocalManagedObject->getQosDscpTrafficClassMap() != ObjectId::NullObjectId)
        {
            ObjectId dscpMapObjectId;
            dscpMapObjectId = pPhyIntfLocalManagedObject->getQosDscpTrafficClassMap();
            DscpToTrafficClassMapManagedObject *pDscpObject;
            pDscpObject = getDscpToTrafficClassMapMOByObjectId(dscpMapObjectId);
            if(pDscpObject != NULL)
            {
                QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
                m->setIfName(ifName);
                m->setOpCode(CONFIGURE_INTERFACE_QOS);
                m->setDscpToTrafficClassMapName(pDscpObject->getDscpToTrafficClassMapName());
                m->setCmdCode(QOS_INTERFACE_DSCP_TRAFFIC_CLASS);
                NSM_PLUG_DBG("Sending DscpToDscpTrafficClassMap");
                sendSynchronouslyToWaveClient ("qos",m);
                delete m;
                delete pDscpObject;
            }
        }
		if(pPhyIntfLocalManagedObject->getQosDscpCosMap() != ObjectId::NullObjectId)
        {
            ObjectId cosMapObjectId;
            cosMapObjectId = pPhyIntfLocalManagedObject->getQosDscpCosMap();
            DscpToCosMapManagedObject *pDscpCosObject;
            pDscpCosObject = getDscpToCosMapMOByObjectId(cosMapObjectId);
            if(pDscpCosObject != NULL)
            {
                QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
                m->setIfName(ifName);
                m->setOpCode(CONFIGURE_INTERFACE_QOS);
                m->setDscpToCosMapName(pDscpCosObject->getDscpToCosMapName());
                m->setCmdCode(QOS_INTERFACE_DSCP_COS);
                NSM_PLUG_DBG("Sending DscpToCosTrafficClassMap");
                sendSynchronouslyToWaveClient ("qos",m);
                delete m;
                delete pDscpCosObject;
            }
        }
		if(pPhyIntfLocalManagedObject->getQosDscpTrust())
        {
            QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
            m->setIfName(ifName);
            m->setOpCode(CONFIGURE_INTERFACE_QOS);
            m->setCmdCode(QOS_INTERFACE_TRUST_DSCP);
            m->setQosDscpTrust(pPhyIntfLocalManagedObject->getQosDscpTrust());
            NSM_PLUG_DBG("Sending TrustDscp");
            sendSynchronouslyToWaveClient ("qos",m);
            delete m;
        }
		if(pPhyIntfLocalManagedObject->getQosTrust())
		{
			QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
			m->setIfName(ifName);
			m->setOpCode(CONFIGURE_INTERFACE_QOS);
			m->setCmdCode(QOS_INTERFACE_TRUST_COS);
			m->setQosTrust(pPhyIntfLocalManagedObject->getQosTrust());
			NSM_PLUG_DBG("Sending TrustCos");
			sendSynchronouslyToWaveClient ("qos",m);
			delete m;
		}
		{
			QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
			m->setIfName(ifName);
			m->setOpCode(CONFIGURE_INTERFACE_QOS);
			m->setCmdCode(QOS_INTERFACE_DEFAULT_COS);
			m->setDefaultCosValue(pPhyIntfLocalManagedObject->getDefaultCos());
			NSM_PLUG_DBG("Sending DefaultCos");
			sendSynchronouslyToWaveClient ("qos",m);
			delete m;
		}
		if((pPhyIntfLocalManagedObject->getFlowControlTx() == 1) || (pPhyIntfLocalManagedObject->getFlowControlRx() == 1))
		{
			QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
			m->setIfName(ifName);
			m->setOpCode(CONFIGURE_INTERFACE_QOS);
			m->setCmdCode(QOS_INTERFACE_FLOW_CONTROL_BOTH);
			m->setFlowControlTx(pPhyIntfLocalManagedObject->getFlowControlTx());
			m->setFlowControlRx(pPhyIntfLocalManagedObject->getFlowControlRx());
			NSM_PLUG_DBG("Sending FlowControl");
			sendSynchronouslyToWaveClient ("qos",m);
			delete m;
		}
		return ;
    }

	void NsmLocalObjectManager::configReplayPhyQosCosThresholds(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{
	  	char buffer[IFNAME_MAX_STR + 1];
	  	buffer[IFNAME_MAX_STR] = '\0';
        NSM_PLUG_DBG("Entering::configReplayPhyQosMaps");
		if(NULL == pPhyIntfLocalManagedObject)
		{
			NSM_PLUG_DBG("PhyIntfLocalManagedObject is not valid");
			return;
		}
		string ifName;

		if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
		{
			NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
		}
		else
		{
			ifName = pPhyIntfLocalManagedObject->getIfName();
		}
      	InterfaceType ifType = NsmUtils::getInterfaceType(pPhyIntfLocalManagedObject->getIslCapability());
	  	GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, IFNAME_MAX_STR);
	  	ifName = buffer;
		NSM_PLUG_DBG_S(string("Configuring the interface") + ifName);

		WaveManagedObjectPointer<QosCosToTailDropManagedObject> pCosTailDropManagedObject = 
			WaveManagedObjectPointer<QosCosToTailDropManagedObject> (pPhyIntfLocalManagedObject->getCosTailDropConfig());
		QosCosToTailDropManagedObject *pCosTailDropMO = pCosTailDropManagedObject.operator->();

		if (pCosTailDropMO == NULL) {
			NSM_PLUG_DBG("No CoS Threshold configured for the interface");
			return;
		}
		QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
		m->setIfName(ifName);
		m->setOpCode(CONFIGURE_INTERFACE_QOS);
		m->setCos0Threshold(pCosTailDropMO->getCos0Threshold());
		m->setCos1Threshold(pCosTailDropMO->getCos1Threshold());
		m->setCos2Threshold(pCosTailDropMO->getCos2Threshold());
		m->setCos3Threshold(pCosTailDropMO->getCos3Threshold());
		m->setCos4Threshold(pCosTailDropMO->getCos4Threshold());
		m->setCos5Threshold(pCosTailDropMO->getCos5Threshold());
		m->setCos6Threshold(pCosTailDropMO->getCos6Threshold());
		m->setCos7Threshold(pCosTailDropMO->getCos7Threshold());
		m->setCmdCode(QOS_INTERFACE_COS_THRESHOLD);
		NSM_PLUG_DBG("Sending CoSTailDrop Threshold");
		sendSynchronouslyToWaveClient ("qos", m);
		delete m;
	}
	
    void NsmLocalObjectManager::configReplayPoQosMaps(PoIntfManagedObject *pPoIntfManagedObject)
    {
        NSM_PLUG_DBG("Entering configReplayPoQosMaps");
		if(NULL == pPoIntfManagedObject)
		{
			NSM_PLUG_DBG("PoIntfManagedObject is not valid");
			return;
		}
		stringstream poName;
	
		poName << pPoIntfManagedObject->getId();
		string ifName = poName.str();
		ifName.insert(0, "po");
		NSM_PLUG_DBG_S(string("Configuring the Portchannel") + ifName);

		if(pPoIntfManagedObject->getQosCosMap() != ObjectId::NullObjectId)
		{               
			ObjectId cosMapObjectId;
			cosMapObjectId = pPoIntfManagedObject->getQosCosMap();		
			CosToCosMutationMapManagedObject *pCosObject;
			pCosObject = getCosToCosMutationMapMOByObjectId(cosMapObjectId);	
			if(pCosObject != NULL)
			{	
				QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
				m->setIfName(ifName);
				m->setOpCode(CONFIGURE_INTERFACE_QOS);
				m->setCosToCosMutationMapName(pCosObject->getCosToCosMutationMapName());
				m->setCmdCode(QOS_INTERFACE_COS_MUTATION);
				NSM_PLUG_DBG("Sending CosToCosMutationMap");
				sendSynchronouslyToWaveClient ("qos",m);
				delete m;
                delete pCosObject;
			}
		}
		if(pPoIntfManagedObject->getQosTrafficClassMap() != ObjectId::NullObjectId)
		{               
			ObjectId cosMapObjectId;
			cosMapObjectId = pPoIntfManagedObject->getQosTrafficClassMap();
			CosToTrafficClassMapManagedObject *pCosObject;
			pCosObject = getCosToTrafficClassMapMOByObjectId(cosMapObjectId);
			if(pCosObject != NULL)
			{	
				QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
				m->setIfName(ifName);
				m->setOpCode(CONFIGURE_INTERFACE_QOS);
				m->setCosToTrafficClassMapName(pCosObject->getCosToTrafficClassMapName());
				m->setCmdCode(QOS_INTERFACE_COS_TRAFFIC_CLASS);
				NSM_PLUG_DBG("Sending CosToCosTrafficClassMap");
				sendSynchronouslyToWaveClient ("qos",m);
				delete m;
                delete pCosObject;
			}
		}
		if(pPoIntfManagedObject->getQosDscpMap() != ObjectId::NullObjectId)
        {
            ObjectId dscpMapObjectId;
            dscpMapObjectId = pPoIntfManagedObject->getQosDscpMap();
            DscpToDscpMutationMapManagedObject *pDscpObject;
            pDscpObject = getDscpToDscpMutationMapMOByObjectId(dscpMapObjectId);
            if(pDscpObject != NULL)
            {
                QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
                m->setIfName(ifName);
                m->setOpCode(CONFIGURE_INTERFACE_QOS);
                m->setDscpToDscpMutationMapName(pDscpObject->getDscpToDscpMutationMapName());
                m->setCmdCode(QOS_INTERFACE_DSCP_MUTATION);
                NSM_PLUG_DBG("Sending DscpToDscpMutationMap");
                sendSynchronouslyToWaveClient ("qos",m);
                delete m;
                delete pDscpObject;
            }
        }
        if(pPoIntfManagedObject->getQosDscpTrafficClassMap() != ObjectId::NullObjectId)
        {
            ObjectId dscpMapObjectId;
            dscpMapObjectId = pPoIntfManagedObject->getQosDscpTrafficClassMap();
            DscpToTrafficClassMapManagedObject *pDscpObject;
            pDscpObject = getDscpToTrafficClassMapMOByObjectId(dscpMapObjectId);
            if(pDscpObject != NULL)
            {
                QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
                m->setIfName(ifName);
                m->setOpCode(CONFIGURE_INTERFACE_QOS);
                m->setDscpToTrafficClassMapName(pDscpObject->getDscpToTrafficClassMapName());
                m->setCmdCode(QOS_INTERFACE_DSCP_TRAFFIC_CLASS);
                NSM_PLUG_DBG("Sending DscpToTrafficClassMap");
                sendSynchronouslyToWaveClient ("qos",m);
                delete m;
                delete pDscpObject;
            }
        }
		if(pPoIntfManagedObject->getQosDscpCosMap() != ObjectId::NullObjectId)
        {
            ObjectId cosMapObjectId;
            cosMapObjectId = pPoIntfManagedObject->getQosDscpCosMap();
            DscpToCosMapManagedObject *pDscpCosObject;
            pDscpCosObject = getDscpToCosMapMOByObjectId(cosMapObjectId);
            if(pDscpCosObject != NULL)
            {
                QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
                m->setIfName(ifName);
                m->setOpCode(CONFIGURE_INTERFACE_QOS);
                m->setDscpToCosMapName(pDscpCosObject->getDscpToCosMapName());
                m->setCmdCode(QOS_INTERFACE_DSCP_COS);
                NSM_PLUG_DBG("Sending DscpToCosTrafficClassMap");
                sendSynchronouslyToWaveClient ("qos",m);
                delete m;
                delete pDscpCosObject;
            }
        }
		if(pPoIntfManagedObject->getQosDscpTrust())
        {
            QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
            m->setIfName(ifName);
            m->setOpCode(CONFIGURE_INTERFACE_QOS);
            m->setCmdCode(QOS_INTERFACE_TRUST_DSCP);
            m->setQosDscpTrust(pPoIntfManagedObject->getQosDscpTrust());
            NSM_PLUG_DBG("Sending TrustDscp");
            sendSynchronouslyToWaveClient ("qos",m);
            delete m;
        }
		if(pPoIntfManagedObject->getQosTrust())
		{	
			QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
			m->setIfName(ifName);
			m->setOpCode(CONFIGURE_INTERFACE_QOS);
			m->setCmdCode(QOS_INTERFACE_TRUST_COS);
			m->setQosTrust(pPoIntfManagedObject->getQosTrust());
			NSM_PLUG_DBG("Sending TrustCos");
			sendSynchronouslyToWaveClient ("qos",m);
			delete m;
		}
		{
			QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
			m->setIfName(ifName);
			m->setOpCode(CONFIGURE_INTERFACE_QOS);
			m->setCmdCode(QOS_INTERFACE_DEFAULT_COS);
			m->setDefaultCosValue(pPoIntfManagedObject->getDefaultCos());
			NSM_PLUG_DBG("Sending DefaultCos");
			sendSynchronouslyToWaveClient ("qos",m);
			delete m;
		}
		if((pPoIntfManagedObject->getFlowControlTx()  == 1) || (pPoIntfManagedObject->getFlowControlRx()  == 1))
		{
			QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
			m->setIfName(ifName);
			m->setOpCode(CONFIGURE_INTERFACE_QOS);
			m->setCmdCode(QOS_INTERFACE_FLOW_CONTROL_BOTH);
			m->setFlowControlTx(pPoIntfManagedObject->getFlowControlTx());
			m->setFlowControlRx(pPoIntfManagedObject->getFlowControlRx());
			NSM_PLUG_DBG("Sending FlowControl");
			sendSynchronouslyToWaveClient ("qos",m);
			delete m;
		}
		return ;
	}

    void NsmLocalObjectManager::configReplayPhyQosPfc(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        NSM_PLUG_DBG("Entering::configReplayPhyQosPfc");
	  	char buffer[IFNAME_MAX_STR + 1];
	  	buffer[IFNAME_MAX_STR] = '\0';

		if(NULL == pPhyIntfLocalManagedObject)
		{
			NSM_PLUG_DBG("PhyIntfLocalManagedObject is not valid");
			return;
		}
		if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
		{
			return;
		}
		string ifName;
	
		if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
		{
			NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
		}
		else
		{
			ifName = pPhyIntfLocalManagedObject->getIfName();
		}
        InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
            NSM_PLUG_INFO_S(string(__FUNCTION__) +
            " ifName: " + ifName + " ifType=" + ifType + "\n");

	  	GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, IFNAME_MAX_STR);
	  	ifName = buffer;
		//ifName.insert(0, "te");
		NSM_PLUG_DBG_S(string("Configuring the interface") + ifName);
	
		vector<WaveManagedObjectPointer<PfcFlowControlLocalManagedObject> > pfcMOVector =  pPhyIntfLocalManagedObject->getPfcFlowControl();
		unsigned int size = pfcMOVector.size();
		unsigned int j; 
		for (j = 0; j < size; j++)
		{
			QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
			m->setIfName(ifName);
			m->setOpCode(CONFIGURE_INTERFACE_QOS);
			m->setCmdCode(QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH);
			m->setPfcFlowControlCos(pfcMOVector[j]->getCos());
			m->setPfcFlowControlTx(pfcMOVector[j]->getTx());
			m->setPfcFlowControlRx(pfcMOVector[j]->getRx());
			NSM_PLUG_DBG("Sending PfcFlowControl");
			sendSynchronouslyToWaveClient ("qos",m);
			delete m;
		}
		return ;
    }

	void NsmLocalObjectManager::configReplayPhyQosCosProfile(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        NSM_PLUG_DBG("Entering::configReplayPhyQosCosProfile");
	  	char buffer[IFNAME_MAX_STR + 1];
	  	buffer[IFNAME_MAX_STR] = '\0';

        if(NULL == pPhyIntfLocalManagedObject)
        {
            NSM_PLUG_DBG("PhyIntfLocalManagedObject is not valid");
            return;
        }
        if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
        {
            return;
        }
        string ifName;

        if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
        {
            NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
        }
        else
        {
            ifName = pPhyIntfLocalManagedObject->getIfName();
        }

        InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
            NSM_PLUG_INFO_S(string(__FUNCTION__) +
            " ifName: " + ifName + " ifType=" + ifType + "\n");

	  	GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, IFNAME_MAX_STR);
	  	ifName = buffer;
        NSM_PLUG_DBG_S(string("Configuring the interface") + ifName);

        vector<WaveManagedObjectPointer<QosCosToProfileIdManagedObject> > cosProfileMOVector =  pPhyIntfLocalManagedObject->getCosProfile();
        unsigned int size = cosProfileMOVector.size();
        unsigned int j;
        for (j = 0; j < size; j++)
        {
            QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
            m->setIfName(ifName);
            m->setOpCode(CONFIGURE_INTERFACE_QOS);
            m->setCmdCode(QOS_INTERFACE_COS_PROFILE);
            m->setCos(cosProfileMOVector[j]->getCos());
            m->setRedProfile(cosProfileMOVector[j]->getRedProfile());
            //m->setPfcFlowControlRx(pfcMOVector[j]->getRx());
            NSM_PLUG_DBG("Sending cos profile");
            sendSynchronouslyToWaveClient ("qos",m);
            delete m;
        }
		return ;
    }

	 void NsmLocalObjectManager::configReplayPoQosCosProfile(PoIntfManagedObject *pPoIntfManagedObject)
    {
        NSM_PLUG_DBG("Entering::configReplayPoQosCosProfile");

        if(NULL == pPoIntfManagedObject)
        {
            NSM_PLUG_DBG("PoIntfManagedObject is not valid");
            return;
        }
        stringstream poName;

        poName << pPoIntfManagedObject->getId();
        string ifName = poName.str();
        ifName.insert(0, "po");
        NSM_PLUG_DBG_S(string("Configuring the Portchannel") + ifName);

        vector<WaveManagedObjectPointer<QosCosToProfileIdManagedObject> > cosProfileMOVector =  pPoIntfManagedObject->getCosProfile();
        unsigned int size = cosProfileMOVector.size();
        unsigned int j;
        for (j = 0; j < size; j++)
        {
            QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
            m->setIfName(ifName);
            m->setOpCode(CONFIGURE_INTERFACE_QOS);
            m->setCmdCode(QOS_INTERFACE_COS_PROFILE);
            m->setCos(cosProfileMOVector[j]->getCos());
            m->setRedProfile(cosProfileMOVector[j]->getRedProfile());
            //m->setPfcFlowControlRx(pfcMOVector[j]->getRx());
            NSM_PLUG_DBG("Sending cos profile");
            sendSynchronouslyToWaveClient ("qos",m);
            if( NULL != m ) {
            	delete m;
            }
        }
        return ;
    }

	void NsmLocalObjectManager::configReplayPhyQosCosTailDrop(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        NSM_PLUG_DBG("Entering::configReplayPhyQosTailDrop");

        if(NULL == pPhyIntfLocalManagedObject)
        {
            NSM_PLUG_DBG("PhyIntfLocalManagedObject is not valid");
            return;
        }
        if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
        {
            return;
        }
        string ifName;

        if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
        {
            NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
        }
        else
        {
            ifName = pPhyIntfLocalManagedObject->getIfName();
        }
        ifName.insert(0, "te");
        NSM_PLUG_DBG_S(string("Configuring the interface") + ifName);

        vector<WaveManagedObjectPointer<QosCosToTailDropManagedObject> > TailDropMOVector =  pPhyIntfLocalManagedObject->getCosTailDrop();
        unsigned int size = TailDropMOVector.size();
        unsigned int j;
        for (j = 0; j < size; j++)
        {
            QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
            m->setIfName(ifName);
            m->setOpCode(CONFIGURE_INTERFACE_QOS);
            m->setCmdCode(QOS_INTERFACE_TAIL_DROP);
            // COMPILE m->setCos(TailDropMOVector[j]->getCos());
            // COMPILE m->setTailDrop(TailDropMOVector[j]->getTailDropThreshold());
            //m->setPfcFlowControlRx(pfcMOVector[j]->getRx());
            NSM_PLUG_DBG("Sending tail drop");
            sendSynchronouslyToWaveClient ("qos",m);
            delete m;
        }
        return ;
    }


    void NsmLocalObjectManager::configReplayPoQosCosTailDrop(PoIntfManagedObject *pPoIntfManagedObject)
    {
        NSM_PLUG_DBG("Entering::configReplayPoQosTailDrop");

        if(NULL == pPoIntfManagedObject)
        {
            NSM_PLUG_DBG("PoIntfManagedObject is not valid");
            return;
        }
        stringstream poName;

        poName << pPoIntfManagedObject->getId();
        string ifName = poName.str();
        ifName.insert(0, "po");
        NSM_PLUG_DBG_S(string("Configuring the Portchannel") + ifName);

        vector<WaveManagedObjectPointer<QosCosToTailDropManagedObject> > TailDropMOVector =  pPoIntfManagedObject->getCosTailDrop();
        unsigned int size = TailDropMOVector.size();
        unsigned int j;
        for (j = 0; j < size; j++)
        {
            QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
            m->setIfName(ifName);
            m->setOpCode(CONFIGURE_INTERFACE_QOS);
            m->setCmdCode(QOS_INTERFACE_TAIL_DROP);
            // COMPILE m->setCos(TailDropMOVector[j]->getCos());
            // COMPILE m->setTailDrop(TailDropMOVector[j]->getTailDropThreshold());
            //m->setPfcFlowControlRx(pfcMOVector[j]->getRx());
            NSM_PLUG_DBG("Sending tail drop");
            sendSynchronouslyToWaveClient ("qos",m);
            if(NULL!=m) {
            	delete m;
            }
        }
        return ;
    }

    void NsmLocalObjectManager::configReplayPoQosPfc(PoIntfManagedObject *pPoIntfManagedObject)
    {
        NSM_PLUG_DBG("Entering::configReplayPoQosPfc");

		if(NULL == pPoIntfManagedObject)
		{
			NSM_PLUG_DBG("PoIntfManagedObject is not valid");
			return;
		}
		stringstream poName;
	
		poName << pPoIntfManagedObject->getId();
		string ifName = poName.str();
		ifName.insert(0, "po");
		NSM_PLUG_DBG_S(string("Configuring the Portchannel") + ifName);

		vector<WaveManagedObjectPointer<PfcFlowControlLocalManagedObject> > pfcMOVector =  pPoIntfManagedObject->getPfcFlowControl();
		unsigned int size = pfcMOVector.size();
		unsigned int j; 
		for (j = 0; j < size; j++)
		{
			QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
			m->setIfName(ifName);
			m->setOpCode(CONFIGURE_INTERFACE_QOS);
			m->setCmdCode(QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH);
			m->setPfcFlowControlCos(pfcMOVector[j]->getCos());
			m->setPfcFlowControlTx(pfcMOVector[j]->getTx());
			m->setPfcFlowControlRx(pfcMOVector[j]->getRx());
			NSM_PLUG_DBG("Sending FlowControl");
			sendSynchronouslyToWaveClient ("qos",m);
			if(NULL!=m) {
				delete m;
			}
		}
		return ;
    }

    void NsmLocalObjectManager::configReplayPhyCee(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        NSM_PLUG_DBG("Entering::configReplayPhyCee");
	  	char buffer[IFNAME_MAX_STR + 1];
	  	buffer[IFNAME_MAX_STR] = '\0';

		if(NULL == pPhyIntfLocalManagedObject)
		{
			NSM_PLUG_DBG("PhyIntfLocalManagedObject is not valid");
			return;
		}
		string ifName;
	
		if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
		{
			NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
		}
		else
		{
			ifName = pPhyIntfLocalManagedObject->getIfName();
		}
        InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
            NSM_PLUG_INFO_S(string(__FUNCTION__) +
            " ifName: " + ifName + " ifType=" + ifType + "\n");

	  	GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, IFNAME_MAX_STR);
	  	ifName = buffer;
		//ifName.insert(0, "te");
		NSM_PLUG_DBG_S(string("Configuring the interface") + ifName);
					
		if(pPhyIntfLocalManagedObject->getCeeMap() != ObjectId::NullObjectId)
		{   
			ObjectId ceeMapObjectId;
			ceeMapObjectId = pPhyIntfLocalManagedObject->getCeeMap();		
			CeeMapManagedObject *pCeeObject;
			pCeeObject = getCeeMapMOByObjectId(ceeMapObjectId);	
			if(pCeeObject != NULL)
			{
				QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
				m->setIfName(ifName);
				m->setOpCode(CONFIGURE_INTERFACE_QOS);
				m->setCeeMapName(pCeeObject->getCeeMapName());
				m->setCmdCode(QOS_INTERFACE_CEE_MAP);
				NSM_PLUG_DBG("Sending CeeMap");
				sendSynchronouslyToWaveClient ("qos",m);
				delete m;
                delete pCeeObject;
			}
		}
		return ;
    }

    void NsmLocalObjectManager::configReplayPoCee(PoIntfManagedObject *pPoIntfManagedObject)
    {
        NSM_PLUG_DBG("Entering::configReplayPoCee");
		if(NULL == pPoIntfManagedObject)
		{
			NSM_PLUG_DBG("PoIntfManagedObject is not valid");
			return;
		}
		stringstream poName;
	
		poName << pPoIntfManagedObject->getId();
		string ifName = poName.str();
		ifName.insert(0, "po");
		NSM_PLUG_DBG_S(string("Configuring the Portchannel") + ifName);

		if(pPoIntfManagedObject->getCeeMap() != ObjectId::NullObjectId)
		{   
			ObjectId ceeMapObjectId;
			ceeMapObjectId = pPoIntfManagedObject->getCeeMap();		
			CeeMapManagedObject *pCeeObject;
			pCeeObject = getCeeMapMOByObjectId(ceeMapObjectId);	
			if(pCeeObject != NULL)
			{
				QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
				m->setIfName(ifName);
				m->setOpCode(CONFIGURE_INTERFACE_QOS);
				m->setCeeMapName(pCeeObject->getCeeMapName());
				m->setCmdCode(QOS_INTERFACE_CEE_MAP);
				NSM_PLUG_DBG("Sending CeeMap");
				sendSynchronouslyToWaveClient ("qos",m);
				delete m;
                delete pCeeObject;
			}
		}
		return ;
    }

    void NsmLocalObjectManager::configReplayPoPriorityTag(PoIntfManagedObject *pPoIntfManagedObject)
    {
        NSM_PLUG_DBG("Entering::NSM Local configReplayPoPriorityTag");
        if(NULL == pPoIntfManagedObject)
        {
            NSM_PLUG_DBG("PoIntfManagedObject is not valid");
            return;
        }

        stringstream poName;

        poName << pPoIntfManagedObject->getId();
        string ifName = poName.str();
        ifName.insert(0, "po");
        NSM_PLUG_DBG_S(string("Configuring the Portchannel") + ifName);

        if(pPoIntfManagedObject->getPriorityTag() != false)
        {
            QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
            m->setIfName(ifName);
            m->setOpCode(CONFIGURE_INTERFACE_QOS);
            m->setCmdCode(QOS_INTERFACE_PRIORITY_TAG);
            NSM_PLUG_DBG("Sending Priority Tag");
            sendSynchronouslyToWaveClient ("qos",m);
            delete m;
        }
        return ;        
    }

    void NsmLocalObjectManager::configReplayPhyPriorityTag(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        NSM_PLUG_DBG("Entering NsmLocalObjectManager::configReplayPhyPriorityTag");
	  	char buffer[IFNAME_MAX_STR + 1];
	  	buffer[IFNAME_MAX_STR] = '\0';

        if(NULL == pPhyIntfLocalManagedObject)
        {
            NSM_PLUG_DBG("PhyIntfLocalManagedObject is not valid");
            return;
        }
        if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
        {
            return;
        }
        string ifName;

        if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
        {
            NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
        }
        else
        {
            ifName = pPhyIntfLocalManagedObject->getIfName();
        }
        InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
            NSM_PLUG_INFO_S(string(__FUNCTION__) +
            " ifName: " + ifName + " ifType=" + ifType + "\n");

	  	GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, IFNAME_MAX_STR);
	  	ifName = buffer;
        //ifName.insert(0, "te");
        NSM_PLUG_DBG_S(string("Configuring the interface") + ifName);

        if(pPhyIntfLocalManagedObject->getPriorityTag() != false)
        {
            QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
            m->setIfName(ifName);
            m->setOpCode(CONFIGURE_INTERFACE_QOS);
            m->setCmdCode(QOS_INTERFACE_PRIORITY_TAG);
            NSM_PLUG_DBG("Sending Priority Tag");
            sendSynchronouslyToWaveClient ("qos",m);
            delete m;
		}
		return ;
    }

CosToCosMutationMapManagedObject * NsmLocalObjectManager::getCosToCosMutationMapMOByObjectId(const ObjectId &oId)
{
	NSM_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToCosMutationMapManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(oId.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(oId.getInstanceId(), "objectidinstanceid") );
	vector<WaveManagedObject *>* pResults = querySynchronously(&syncQueryCtxt);
	CosToCosMutationMapManagedObject *mapMO = NULL;
	if(pResults)
	{
		if(pResults->size() == 1)
		{
			mapMO =  dynamic_cast<CosToCosMutationMapManagedObject*>((*pResults)[0]);
		}
		else
		{
			NSM_PLUG_DBG("could not find MO");
		}
        pResults->clear();
        delete pResults;
	}
	return mapMO;
}

DscpToDscpMutationMapManagedObject * NsmLocalObjectManager::getDscpToDscpMutationMapMOByObjectId(const ObjectId &oId)
{
    NSM_PLUG_DBG("Entering ...");
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToDscpMutationMapManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute(new AttributeUI32(oId.getClassId(), "objectidclassid") );
    syncQueryCtxt.addAndAttribute(new AttributeUI64(oId.getInstanceId(), "objectidinstanceid") );
    vector<WaveManagedObject *>* pResults = querySynchronously(&syncQueryCtxt);
    DscpToDscpMutationMapManagedObject *mapMO = NULL;
    if(pResults)
    {
        if(pResults->size() == 1)
        {
            mapMO =  dynamic_cast<DscpToDscpMutationMapManagedObject*>((*pResults)[0]);
        }
        else
        {
            NSM_PLUG_DBG("could not find MO");
        }
    }
    return mapMO;
}

CosToTrafficClassMapManagedObject * NsmLocalObjectManager::getCosToTrafficClassMapMOByObjectId(const ObjectId &oId)
{
	NSM_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToTrafficClassMapManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(oId.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(oId.getInstanceId(), "objectidinstanceid") );
	vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
	CosToTrafficClassMapManagedObject *mapMO = NULL;
	if(pResults)
	{
		if(pResults->size() == 1)
		{
			mapMO =  dynamic_cast<CosToTrafficClassMapManagedObject *>((*pResults)[0]);
		}
		else
		{
			NSM_PLUG_DBG("could not find MO");
		}
        pResults->clear();
        delete pResults;
	}
	return mapMO;
}

DscpToTrafficClassMapManagedObject * NsmLocalObjectManager::getDscpToTrafficClassMapMOByObjectId(const ObjectId &oId)
{
    NSM_PLUG_DBG("Entering ...");
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToTrafficClassMapManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute(new AttributeUI32(oId.getClassId(), "objectidclassid") );
    syncQueryCtxt.addAndAttribute(new AttributeUI64(oId.getInstanceId(), "objectidinstanceid") );
    vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
    DscpToTrafficClassMapManagedObject *mapMO = NULL;
    if(pResults)
    {
        if(pResults->size() == 1)
        {
            mapMO =  dynamic_cast<DscpToTrafficClassMapManagedObject *>((*pResults)[0]);
        }
        else
        {
            NSM_PLUG_DBG("could not find MO");
        }
    }
    return mapMO;
}

DscpToCosMapManagedObject * NsmLocalObjectManager::getDscpToCosMapMOByObjectId(const ObjectId &oId)
{
    NSM_PLUG_DBG("Entering ...");
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToCosMapManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute(new AttributeUI32(oId.getClassId(), "objectidclassid") );
    syncQueryCtxt.addAndAttribute(new AttributeUI64(oId.getInstanceId(), "objectidinstanceid") );
    vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
    DscpToCosMapManagedObject *mapMO = NULL;
    if(pResults)
    {
        if(pResults->size() == 1)
        {
            mapMO =  dynamic_cast<DscpToCosMapManagedObject *>((*pResults)[0]);
        }
        else
        {
            NSM_PLUG_DBG("could not find MO");
        }
    }
    return mapMO;
}

CeeMapManagedObject * NsmLocalObjectManager::getCeeMapMOByObjectId(const ObjectId &oId)
{
	NSM_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(oId.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(oId.getInstanceId(), "objectidinstanceid") );
	vector<WaveManagedObject *>* pResults = querySynchronously(&syncQueryCtxt);
	CeeMapManagedObject *mapMO = NULL;
	if(pResults)
	{
		if(pResults->size() == 1)
		{
			mapMO =  dynamic_cast<CeeMapManagedObject*>((*pResults)[0]);
		}
		else
		{
			NSM_PLUG_DBG("could not find MO");
		}
        pResults->clear();
        delete pResults;
	}
	return mapMO;
}

void NsmLocalObjectManager::configReplayInterfaceDesc(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {

        NSM_PLUG_DBG("Entering NsmLocalObjectManager::configReplayInterfaceDesc");
                if (pPhyIntfLocalManagedObject == NULL)
                {
                        NSM_PLUG_DBG("PhyIntfLocalManagedObject is not valid");
                        return;
                }

        NsmPhyIntfMessage *m = new NsmPhyIntfMessage();
        string ifName = pPhyIntfLocalManagedObject->getIfName();

        if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
        {
            NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
        }

	InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();	
        m->setIfType(ifType);
        m->setIfName(ifName);

        /* create Phy desc */
        if(!(pPhyIntfLocalManagedObject->getDesc()).empty())
        {
            trace (TRACE_LEVEL_INFO, string ("set Phy Desc ") + m->getBulkCmdCode());
            m->setDesc(pPhyIntfLocalManagedObject->getDesc());
            /* Merged the defect fix from 3.0.1 patch(defect423036) */
            m->setCmdCode(NSMUPDATE_PHY_DESC);
            sendSynchronouslyToWaveClient ("nsm",m);
            NSM_PLUG_DBG("End NsmLocalObjectManager::configReplayInterfaceDesc");
        }


        if(m)
             delete m;

        return;
    }

    void NsmLocalObjectManager::configReplaySflowInterface(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        NSM_PLUG_DBG("Entering NsmLocalObjectManager::configReplaySflowInterface");
		if (pPhyIntfLocalManagedObject == NULL)
		{
			NSM_PLUG_DBG("PhyIntfLocalManagedObject is not valid");
			return;
		}

		if (pPhyIntfLocalManagedObject->getEnable())
		{
			NsmPhyIntfMessage *m = new NsmPhyIntfMessage();
			string ifName = pPhyIntfLocalManagedObject->getIfName();
			if(true  == NsmUtils::isValidThreeTuple(ifName))
			{
				string twoTuppleIfName;
				NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
				ifName = twoTuppleIfName;
			}
            InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
            NSM_PLUG_INFO_S(string(__FUNCTION__) +
                " ifName: " + ifName + " ifType=" + ifType + "\n");

        	m->setIfType(ifType);

			m->setIfName(ifName);
			m->setEnable(pPhyIntfLocalManagedObject->getEnable());
			if(pPhyIntfLocalManagedObject->getIsPollingIntervalSet())
			{
				m->setPollingInterval(pPhyIntfLocalManagedObject->getPollingInterval());
			}
			else
			{
				m->setPollingInterval(SFLOW_OUT_OF_RANGE_VALUE);
			}
			if(pPhyIntfLocalManagedObject->getIsSampleRateSet())
			{
				m->setSampleRate(pPhyIntfLocalManagedObject->getSampleRate());
			}
			else
			{
				m->setSampleRate(SFLOW_OUT_OF_RANGE_VALUE);
			}
			m->setCmdCode(NSMUPDATE_PHY_SFLOW_SET_ALL);
			sendSynchronouslyToWaveClient ("Sflow",m);
			delete m;
			return ;
    	}
	}

    void NsmLocalObjectManager::configReplayXstpVlanConfig(vector<WaveManagedObject *>* pVlanResults)
    {
	    ResourceId status = WAVE_MESSAGE_ERROR;
	    VlanIntfManagedObject * pVlanMo = NULL;
	    UI32 i = 0;

	    trace (TRACE_LEVEL_DEBUG, "NsmLocalObjectManager::configReplayXstpVlanConfig:  ");

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());

		if (pVlanResults && !pVlanResults->empty()) {
		    vector<UI32> vlanids;
		    size_t vlanMOs = pVlanResults->size();
		    for (i=0; i < vlanMOs; i++) {
		        pVlanMo = dynamic_cast<VlanIntfManagedObject*>((*pVlanResults)[i]);
		        vlanids.push_back(pVlanMo->getId());
		    }
            syncQueryCtxt.setAttributeRangeForQuery(
                    new AttributeUI32Range(UI32Range(vlanids), "id"));
		}

		// For vlan < 4096, by default stp is enabled
		syncQueryCtxt.createConditionSet("VLAN_DOT_1Q");
		syncQueryCtxt.addAttributeToConditionSet("VLAN_DOT_1Q", new AttributeBool(true, "stpDisable"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
		syncQueryCtxt.addAttributeToConditionSet("VLAN_DOT_1Q", new AttributeUI32(MAX_DOT1Q_VLAN_ID, "id"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_LESS_THAN_OR_EQUAL_TO);

		syncQueryCtxt.createConditionSet("VLAN_NON_DOT_1Q");
		syncQueryCtxt.addAttributeToConditionSet("VLAN_NON_DOT_1Q", new AttributeBool(false, "stpDisable"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
		syncQueryCtxt.addAttributeToConditionSet("VLAN_NON_DOT_1Q", new AttributeUI32(MAX_DOT1Q_VLAN_ID, "id"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN);

		syncQueryCtxt.combineConditionSets("VLAN_DOT_1Q+VLAN_NON_DOT_1Q");

		vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);

		size_t count = pResults ? pResults->size() : 0;
        tracePrintf(TRACE_LEVEL_INFO, "NsmLocalObjectManager::configReplayXstpVlanConfig: "
                "Found %d vlans to send to backend..", count);

        for (i=0; i < count; i++) {
            pVlanMo = dynamic_cast<VlanIntfManagedObject*>((*pResults)[i]);
            MstpVlanConfigMessage  message;
            message.setId(pVlanMo->getId());
            if (pVlanMo->getStpDisable()) {
            	message.setCmdCode(MSTP_MSG_VLAN_STP_DISABLE);
            } else {
            	message.setCmdCode(MSTP_MSG_VLAN_NO_STP_DISABLE);
            }

            status = sendSynchronouslyToWaveClient(MSTP_DCM_CLIENT_NAME, &message);

            trace (TRACE_LEVEL_DEBUG, "NsmLocalObjectManager::configReplayXstpPortConfig Client status "+toString((UI32)message.getClientStatus()));

            if (status == WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_SUCCESS, string("MstpVlanConfigMessage: configured "));
            } else {
                trace(TRACE_LEVEL_ERROR, string("MstpVlanConfigMessage: configuration failed"));
            }
        }

		if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}

/*
		if (pResultsGvlan) {
			delete pResultsGvlan;
		}
*/

	}

    ResourceId NsmLocalObjectManager::Pass1VlanConfiguration(VlanIntfManagedObject *pVlanIntfManagedObject,bool createVlanFlag)
    {
        UI32 status = 0;                                                                       
        NsmVlanIntfMessage *m = NULL;
        
        if(pVlanIntfManagedObject == NULL )
        {
			trace (TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::Pass1VlanConfiguration: pVlanIntfManagedObject = NULL"));
            return status;
        }
        if(createVlanFlag)
        {
            m = new NsmVlanIntfMessage(NSMUPDATE_VLAN_CREATE_CMD);
            m->setVlanName(pVlanIntfManagedObject->getIfName());
        }
        else
        {
            m = new NsmVlanIntfMessage();
        }

        /* create vlan interface */
        m->setVlanId(pVlanIntfManagedObject->getId());

        /* create vlan desc */
        if(!(pVlanIntfManagedObject->getDesc()).empty())
            m->setDesc(pVlanIntfManagedObject->getDesc());

        /* set pvlan type*/
        if(pVlanIntfManagedObject->getPVlanType())
            m->setPvlanType(pVlanIntfManagedObject->getPVlanType());

        /* set RSPAN config*/
        m->setRemoteSpanEnabled(pVlanIntfManagedObject->getRemoteSpanEnabled());

        m->setCmdCode(NSMUPDATE_VLAN_BULK_CONF);

        status = sendSynchronouslyToWaveClient ("nsm",m);
		//bpatel: To avoid memory leak
		if(m){
			delete m;
		}

		trace (TRACE_LEVEL_DEBUG, string("End:: NsmLocalObjectManager::Pass1VlanConfiguration:: Vlan = ") + pVlanIntfManagedObject->getIfName() + string("  status ") +	status);
        return status;
    }

    void NsmLocalObjectManager::configReplayVlanPVlan(VlanIntfManagedObject *pVlanIntfManagedObject)
    {
        if(pVlanIntfManagedObject == NULL )
        {
            return;
        }
        if(pVlanIntfManagedObject->getPvlanSecAssocRange().toString() != "")
        {
        	NsmVlanIntfMessage *m =  new NsmVlanIntfMessage();
        	m->setVlanId(pVlanIntfManagedObject->getId());
        	m->setPvlanSecAssocRange(pVlanIntfManagedObject->getPvlanSecAssocRange());
        	m->setCmdCode(NSMUPDATE_VLAN_BULK_CONF);
        	trace (TRACE_LEVEL_INFO, string("setting PVlanAssocRange to:") + pVlanIntfManagedObject->getPvlanSecAssocRange().toString());
        	sendSynchronouslyToWaveClient ("nsm",m);
        	if(m)
        	{
        		delete m;
        	}
        }
        return;
    }

    void NsmLocalObjectManager::postBootPass1VlanConfiguration(vector<WaveManagedObject *>* &pVlanResults, bool createFlag)
    {
        VlanIntfManagedObject *pVlanIntfManagedObject = NULL;
        if (pVlanResults && pVlanResults->size() > 0) {
            for (UI32 vlanIndex = 0; vlanIndex < pVlanResults->size(); vlanIndex++)
            {
                pVlanIntfManagedObject =  dynamic_cast<VlanIntfManagedObject*>((*pVlanResults)[vlanIndex]);
                /* TODO : if it is default vlan , pass false*/
                Pass1VlanConfiguration(pVlanIntfManagedObject, createFlag);
            }
            for (UI32 vlanIndex = 0; vlanIndex < pVlanResults->size(); vlanIndex++)
            {
                pVlanIntfManagedObject =  dynamic_cast<VlanIntfManagedObject*>((*pVlanResults)[vlanIndex]);
                /* TODO : if it is default vlan , pass false*/
                configReplayVlanPVlan(pVlanIntfManagedObject);
            }
        }
    }

    ResourceId NsmLocalObjectManager::Pass1PoConfiguration(PoIntfManagedObject *pPoIntfManagedObject,bool createPoFlag)
    {
        UI32 status = 0;                                                    
        NsmPoIntfMessage *m = NULL;
        bool isProfiledPort = false;

        if(pPoIntfManagedObject == NULL )
        {
			trace (TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::configReplayXstpVlanConfig:pPoIntfManagedObject = NULL"));

            return status;
        }

        status = APPMUtils::isPortProfilePort(pPoIntfManagedObject, isProfiledPort);
        
		NSM_PLUG_INFO_S( string("Entering .. PoName :") +
						pPoIntfManagedObject->getIfName());

        if(createPoFlag)
            m = new NsmPoIntfMessage(NSMUPDATE_PO_CREATE_CMD);
        else
            m = new NsmPoIntfMessage();


        /* create po interface */
        m->setPoId(pPoIntfManagedObject->getIfName());

        /* create Po desc */
        if(!(pPoIntfManagedObject->getDesc()).empty())
        {
            NSM_PLUG_INFO_S( string ("set Po Desc ") + m->getBulkCmdCode());
            m->setDesc(pPoIntfManagedObject->getDesc());
        }

        /* set Po [no] shutdown */
            NSM_PLUG_INFO_S( string ("set Po Flag ") + m->getBulkCmdCode());
            m->setFlag(pPoIntfManagedObject->getStatus());

        /* set Po [no] ignore-split */
            NSM_PLUG_INFO_S( string ("set Po Ignore-Split ") + m->getBulkCmdCode());
            m->setIgnoreSplit(pPoIntfManagedObject->getIgnoreSplit());

        /* set Po [no] mtu */
        if(pPoIntfManagedObject->getMtu() != NSM_MTU_DEF_VAL)
        {
            NSM_PLUG_INFO_S( string ("set Po Mtu ") + m->getBulkCmdCode());
            m->setMtu(pPoIntfManagedObject->getMtu());
        }

        /* set Po [no] minimum-links */
        if(pPoIntfManagedObject->getPoMinLinks() != NSM_MINLINKS_DEF_VAL)
        {
            NSM_PLUG_INFO_S( string ("set Po Minimum-links ") + m->getBulkCmdCode());
            m->setPoMinLinks(pPoIntfManagedObject->getPoMinLinks());
		}

		/*set Po speed */
		if(pPoIntfManagedObject->getPoAllowedSpeed() != (LineSpeed)2)
		{
			NSM_PLUG_INFO_S( string ("set Po Allowed Member Speed ") + m->getBulkCmdCode());
			m->setPoAllowedSpeed(pPoIntfManagedObject->getPoAllowedSpeed());
		}

		/* set Po load-balance */
		if(pPoIntfManagedObject->getPoLoadBalanceMode() != (poLoadBalanceModeEnum)LOADBAL_MODE_SRC_DST_IP_MAC_VID_PORT)
		{
			 NSM_PLUG_INFO_S( string ("set Po Load-balance bulkcmdcode ") + m->getBulkCmdCode());
             m->setPoLoadBalanceMode(pPoIntfManagedObject->getPoLoadBalanceMode());
		}

        if(pPoIntfManagedObject->getSwModeFlag())
        {
            /* set sw */
            NSM_PLUG_INFO_S( string ("set Po Sw ") + m->getBulkCmdCode());
            m->setSwModeFlag(pPoIntfManagedObject->getSwModeFlag());

            /* sw mode trunk */  
            NSM_PLUG_INFO_S( string ("set Po Sw Mode ") + m->getBulkCmdCode());
            m->setSwMode(pPoIntfManagedObject->getSwMode());
            if(pPoIntfManagedObject->getRaGuard())
                m->setRaGuard(pPoIntfManagedObject->getRaGuard());

            if(pPoIntfManagedObject->getSwMode() == SW_MODE_TRUNK )
            {
                /* sw trunk allowed vlan all */ 
                if(pPoIntfManagedObject->getIsAllowedVlanAll()) {
                    NSM_PLUG_INFO_S( string ("set Po Sw allowed all ") + m->getBulkCmdCode());
                    m->setIsAllowedVlanAll(pPoIntfManagedObject->getIsAllowedVlanAll());
					if(pPoIntfManagedObject->getVlanRangeRemove() != UI32Range(""))
						m->setVlanRange(pPoIntfManagedObject->getVlanRangeRemove(), NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN_CMD);
                }else {
                    /* sw trunk allowed vlan add 5 */
                    /* Requried to handle for multiple add / remove, except */
                    if(pPoIntfManagedObject->getVlanRangeAdd() != UI32Range(""))
                    {
                        NSM_PLUG_INFO_S( string ("set Phy Sw trunk add vlan id") + m->getBulkCmdCode());
						m->setVlanRange(pPoIntfManagedObject->getVlanRangeAdd(), NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN_CMD);
                    }
                }
				
				/* sw trunk native-vlan */
				WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanIntfConfLocalManagedObject;
				pNativeVlanIntfConfLocalManagedObject = WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (pPoIntfManagedObject->getNativeVlanIntfConfig());

			    if(pNativeVlanIntfConfLocalManagedObject.operator->() != NULL) {
					m->setNativeVlanId(pNativeVlanIntfConfLocalManagedObject->getNativeVlanId());
					m->setNativevlanDisable(pNativeVlanIntfConfLocalManagedObject->getNativevlanDisable());
                NSM_PLUG_INFO_S( string ("set Phy Sw trunk native-vlan:: id ::" 
										+  m->getNativeVlanId()));
            }
            }
            else if (pPoIntfManagedObject->getSwMode() == SW_MODE_ACCESS )
            {
                // Access Po - TODO
                ObjectId obj = pPoIntfManagedObject->getAccessVlanObjectId();
				UI32Range accessVlan;
				accessVlan.fromString(NsmUtils::getVlanIntfNameByObjectId(obj));
                NSM_PLUG_INFO_S( string ("set Po Sw allowed access add vlan id") + m->getBulkCmdCode());
                m->setVlanRange(accessVlan,NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN_CMD);    
            }
        }

		/*
		 * Set Profiled-Port status
		 */
		if(pPoIntfManagedObject->getIsPortProfiled())
		{
			NSM_PLUG_INFO_S( string ("set Port-Profile-Port "));
			m->setIsPortProfiled(true);
        } else if (isProfiledPort) {
            configReplayPoIfToPortProfileAssociation(pPoIntfManagedObject, pPoIntfManagedObject->getIfName());
		}

        // channel group TODO  
        NSM_PLUG_INFO_S( string ("set Po Mode & Type ") + m->getBulkCmdCode());
        m->setPoTypeAndMode(pPoIntfManagedObject->getPoType(),pPoIntfManagedObject->getPoMode());

        m->setCmdCode(NSMUPDATE_PO_BULK_CONF);

        NSM_PLUG_INFO_S( string ("UP_PO_L Replaying Port-channel Configuraiton Po :: ") + pPoIntfManagedObject->getIfName() + string(" cmdCode :") + m->getCmdCode() + string(" BulkCmdCode:") + m->getBulkCmdCode());
        status = sendSynchronouslyToWaveClient ("nsm",m);
		
        NSM_PLUG_INFO_S( string ("UP_PO_L Replaying Port-channel Configuration response from Backend status :") + status + string("backend_clientStatus :") + m->getClientStatus());

        if( status == WAVE_MESSAGE_SUCCESS)
        {
            if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
                return  m->getCompletionStatus();

            if ( (0 != m->getClientStatus() ) && ( WRC_WAVE_MESSAGE_SUCCESS != m->getClientStatus() ) )  {
                trace(TRACE_LEVEL_ERROR, string("Resp:  Client Error MsgId: ") +
                        m->getMessageId() +
                        FrameworkToolKit::localize(m->getClientStatus()));
                return NsmUtils::mapBackEndErrorsToDcmdErrors(m->getClientStatus());
            }
        }

        //bpatel: To avoid memeory leak
		if(m){
			delete m;
		}

		if(pPoIntfManagedObject->getSwModeFlag())
		{
			configReplayPoPortSecurity(pPoIntfManagedObject);
			configReplayPoVlanClassification(pPoIntfManagedObject, pPoIntfManagedObject->getIfName());

			if(pPoIntfManagedObject->getSwMode() == SW_MODE_UNKNOWN)
			{
				configReplayPoPVlan(pPoIntfManagedObject, pPoIntfManagedObject->getIfName());
			}
		}

		return status;
    }


void  NsmLocalObjectManager::configReplayChannelGroupToPhy(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
{
	string ifName;

	if(pPhyIntfLocalManagedObject == NULL )
	{
		trace (TRACE_LEVEL_ERROR, string("Given Physical Intf MO is Null"));
		return;
	}

	NSM_PLUG_INFO_S ( string("Entering NsmLocalObjectManager::configReplayChannelGroupToPhy ..") +
		pPhyIntfLocalManagedObject->getIfName());

	NsmPhyIntfMessage *m = new NsmPhyIntfMessage();

	/* create Phy interface */
	ifName = pPhyIntfLocalManagedObject->getIfName();

	if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
	{
		NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
	}

	m->setIfName(ifName);

	// channel group TODO
	if(pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
	{
		trace (TRACE_LEVEL_INFO, string ("set Phy Channel Group ") + m->getBulkCmdCode());
		m->setPortChannelName(NsmUtils::getPoIntfNameByObjectId(pPhyIntfLocalManagedObject->getPoId()),pPhyIntfLocalManagedObject->getPoMode(),pPhyIntfLocalManagedObject->getPoType());
	}

	m->setCmdCode(NSMUPDATE_PHY_BULK_CONF);
    InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
    NSM_PLUG_INFO_S(string(__FUNCTION__) +
        " ifName: " + ifName + " ifType=" + ifType + "\n");
	m->setIfType(ifType);

	sendSynchronouslyToWaveClient ("nsm",m);
	//bpatel: To avoid memory leak
	if(m){
		delete m;
	}
	return ;
}

void NsmLocalObjectManager::postBootPass1Phy1Configuration()
{
	trace (TRACE_LEVEL_DEBUG, "Entering NsmLocalObjectManager::postBootPass1Phy1Configuration ..");
	PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
	LocationId                          locationId = 0;
	locationId = FrameworkToolKit::getThisLocationId ();
	vector<WaveManagedObject *> *pPhyResults = querySynchronouslyLocalManagedObjectsForLocationId (locationId, PhyIntfLocalManagedObject::getClassName ());
	if (pPhyResults && pPhyResults->size() > 0) {
		for (UI32 PhyIndex = 0; PhyIndex < pPhyResults->size(); PhyIndex++)
		{
			pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pPhyResults)[PhyIndex]);
			if(pPhyIntfLocalManagedObject->getPortrole() == false)
			{
				NSM_PLUG_INFO_S( string("Reply configuration Edge Port :") + pPhyIntfLocalManagedObject->getIfName());
				configReplayChannelGroupToPhy(pPhyIntfLocalManagedObject);
			}
			else
			{
				NSM_PLUG_INFO_S( string("No Reply configuration ISL Port :") + pPhyIntfLocalManagedObject->getIfName());
			}
		}

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pPhyResults);
	}
}


    void NsmLocalObjectManager::postBootPass1PoConfiguration()
    {
        trace (TRACE_LEVEL_DEBUG, "Entering NsmLocalObjectManager::postBootPass1PoConfiguration ..");
        PoIntfManagedObject *pPoIntfManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext poSyncQueryCtxt(PoIntfManagedObject::getClassName());
        vector<WaveManagedObject *> *pPoResults = querySynchronously (&poSyncQueryCtxt);
        if (pPoResults && pPoResults->size() > 0) {
            for (UI32 poIndex = 0; poIndex < pPoResults->size(); poIndex++)
            {
                pPoIntfManagedObject =  dynamic_cast<PoIntfManagedObject*>((*pPoResults)[poIndex]);
                if( pPoIntfManagedObject != NULL) {
                    LocationId  locationId = 0;
                    locationId  = FrameworkToolKit::getThisLocationId ();
                    if ( ( NsmUtils::isPoIntfHasMemberByLocation( pPoIntfManagedObject->getObjectId(),locationId) == WAVE_MESSAGE_SUCCESS)
                         || ( WAVE_PERSISTENCE_CLUSTER_ENABLED != DcmToolKit::isVcsEnabled()) ) {
                        Pass1PoConfiguration(pPoIntfManagedObject,true);
                    } else {
                        NSM_PLUG_INFO_S( string("Port-channel interface is not configured : No Memebers(LocationId :") + 
                                    locationId + string(")in .. PoName :") + pPoIntfManagedObject->getIfName());
                    }

                    // Fcoe config needs to be pushed irrespective of whether it has members or not.
                    NsmLocalObjectManager::postBootFcoeProvisioningConfigForPoInterface(pPoIntfManagedObject);
                }

            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pPoResults);
            m_pIntfConfLocalWorker->setGlobalPortChannelReplayDone (true);
        }
    }

    ResourceId NsmLocalObjectManager::PassShutdownPhyConfiguration(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        UI32 status = 0;
        string ifName;

        if(pPhyIntfLocalManagedObject == NULL )
        {
			trace (TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::PassShutdownPhyConfiguration:pPhyIntfLocalManagedObject = NULL"));
            return status;
        }
        
		NSM_PLUG_INFO_S ( string("Entering NsmLocalObjectManager::PassShutdownPhyConfiguration ..") +
            pPhyIntfLocalManagedObject->getIfName());

        NsmPhyIntfMessage *m = new NsmPhyIntfMessage();

        /* create Phy interface */
        ifName = pPhyIntfLocalManagedObject->getIfName();
        
        if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
        {
            NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
        }
        
        m->setIfName(ifName);

        /* set Phy [no] shutdown */
        trace (TRACE_LEVEL_DEBUG, string ("set Phy Flag ") + m->getBulkCmdCode());
        m->setFlag(pPhyIntfLocalManagedObject->getStatus());

        m->setCmdCode(NSMUPDATE_PHY_BULK_CONF);
		InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
        m->setIfType(ifType);
        NSM_PLUG_INFO_S(string("PassShutdownPhyConfiguration() ") + string(" ifName: ") + ifName + string(" ifType=") + m->getIfType() + string(" Admin State = ") + (pPhyIntfLocalManagedObject->getStatus() ? "Shut" : "No Shut") + "\n");

        status = sendSynchronouslyToWaveClient ("nsm",m);
		
		NSM_PLUG_INFO_S( string("End:: NsmLocalObjectManager::PassShutdownPhyConfiguration:: status ") +
						status);
		//bpatel: To avoid memory leak
		if(m){
			delete m;
		}
        return status;
    }

    ResourceId NsmLocalObjectManager::PassLongDistancePhyConfiguration(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        UI32 status = 0;
        string ifName;
		char buffer[MAX_STR];
		buffer[MAX_STR - 1] = '\0';
		UI32 bufferSize = sizeof (buffer);


		if(pPhyIntfLocalManagedObject == NULL )
		{
			trace (TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::PassLongDistancePhyConfiguration:pPhyIntfLocalManagedObject = NULL"));
			return status;
		}
        
		NSM_PLUG_INFO_S ( string("Entering NsmLocalObjectManager::PassLongDistancePhyConfiguration ..") +
				pPhyIntfLocalManagedObject->getIfName());

		QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();

        ifName = pPhyIntfLocalManagedObject->getIfName();
        
        if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
        {
            NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
        }
        InterfaceType ifType = NsmUtils::getInterfaceType(pPhyIntfLocalManagedObject->getIslCapability());
		GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, bufferSize);
		ifName = buffer;

		NSM_PLUG_INFO_S ( string("PassLongDistancePhyConfiguration ..ifname") +
				ifName);

        m->setIfName(ifName);
        m->setOpCode(CONFIGURE_INTERFACE_QOS);
		m->setCmdCode(QOS_INTERFACE_LONG_DISTANCE);
		m->setDistance(pPhyIntfLocalManagedObject->getDistance());

        status = sendSynchronouslyToWaveClient ("qos",m);
		
		NSM_PLUG_INFO_S( string("End:: NsmLocalObjectManager::PassLongDistancePhyConfiguration:: status ") +
						status);
		if(m){
			delete m;
		}
        return status;
    }

    void NsmLocalObjectManager::configReplayPoPVlan(PoIntfManagedObject *pPoMO, const string &ifName)
	{
		swModeEnum swMode;
		if (pPoMO->getPvlanModeTrunkHost()) {
			swMode = SW_MODE_PVLAN_TRUNK_HOST;
		} else if (pPoMO->getPvlanModeHost()) {
			swMode = SW_MODE_PVLAN_HOST;
		} else if (pPoMO->getPvlanModeTrunkPromis()) {
			swMode = SW_MODE_PVLAN_TRUNK_PROMISCUOUS;
		} else if (pPoMO->getPvlanModePromiscuous()) {
			swMode = SW_MODE_PVLAN_PROMISCUOUS;
		} else {
			swMode = SW_MODE_PVLAN_TRUNK;
		}
				
		{
			//Set the PVLAN mode first
			NsmPoIntfMessage *m = new NsmPoIntfMessage();
			m->setPoId(ifName);
			m->setSwMode(swMode);
			switch (swMode)
			{
				case SW_MODE_PVLAN_HOST:
					m->setCmdCode(NSMUPDATE_PO_PVLAN_MODE_HOST);
					break;
				case SW_MODE_PVLAN_TRUNK_HOST:
					m->setCmdCode(NSMUPDATE_PO_PVLAN_MODE_TRUNK_HOST);
					break;
				case SW_MODE_PVLAN_PROMISCUOUS:
					m->setCmdCode(NSMUPDATE_PO_PVLAN_MODE_PROMISCUOUS);
					break;
				case SW_MODE_PVLAN_TRUNK_PROMISCUOUS:
					m->setCmdCode(NSMUPDATE_PO_PVLAN_MODE_TRUNK_PROMISCUOUS);
					break;
				case SW_MODE_PVLAN_TRUNK:
					m->setCmdCode(NSMUPDATE_PO_PVLAN_MODE_TRUNK);
					break;
				default:
					trace(TRACE_LEVEL_ERROR, string("This case shouldn't occur"));
					delete m;
					return;
			}	
    	    sendSynchronouslyToWaveClient ("nsm", m);
			delete m;
		}
		if((swMode == SW_MODE_PVLAN_HOST) || (swMode == SW_MODE_PVLAN_TRUNK_HOST))
		{
			if(pPoMO->getPvlanHostPrimVlanId() && pPoMO->getPvlanHostSecVlanId())
			{
				NsmPoIntfMessage *m = new NsmPoIntfMessage();
				m->setPoId(ifName);
				m->setPvlanHostPrimVlanId(pPoMO->getPvlanHostPrimVlanId());
				m->setPvlanHostSecVlanId(pPoMO->getPvlanHostSecVlanId());
				m->setCmdCode(NSMUPDATE_PO_PVLAN_HOST_ASSOCIATION);
    	    	sendSynchronouslyToWaveClient ("nsm", m);
				delete m;
			}	
		} else if((swMode == SW_MODE_PVLAN_PROMISCUOUS) || (swMode == SW_MODE_PVLAN_TRUNK_PROMISCUOUS))	{
			vector<WaveManagedObjectPointer<PVlanMappingManagedObject> > mappingVector = pPoMO->getPVlanMapping();
			if(mappingVector.size())
			{
				for (unsigned int idx = 0; idx < mappingVector.size(); idx++)
				{
					NsmClientPVlanMappingMessage *m = new NsmClientPVlanMappingMessage();
					m->setIfName(ifName);
					m->setIfType(IF_TYPE_PO);
					m->setPrimaryVlanId(mappingVector[idx]->getPrimaryVlanId());
					m->setSecVlanRange(mappingVector[idx]->getSecPvlanRange());
					m->setoper(PVLAN_MAPPING_ADD);
					m->setOpcode(PVLAN_MAPPING_PO);
    	    		sendSynchronouslyToWaveClient ("nsm", m);
					delete m;
				}
			}

			if(swMode == SW_MODE_PVLAN_TRUNK_PROMISCUOUS)
			{
				NsmPoIntfMessage *m = new NsmPoIntfMessage();
				m->setPoId(ifName);

                /* sw trunk allowed vlan all */
				if(pPoMO->getIsAllowedVlanAll())
				{
                    m->setIsAllowedVlanAll(pPoMO->getIsAllowedVlanAll());
					if(pPoMO->getVlanRangeRemove() != UI32Range(""))
					{
						m->setVlanRange(pPoMO->getVlanRangeRemove(), NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN_CMD);
				}
				}
				else
				{
					/* sw trunk allowed vlan add 5 */
					/* Requried to handle for multiple add / remove, except */
					if(pPoMO->getVlanRangeAdd() != UI32Range(""))
					{
						m->setVlanRange(pPoMO->getVlanRangeAdd(), NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN_CMD);
					}
				}

				/* sw trunk native-vlan */
				WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanMO;
				pNativeVlanMO = WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (pPoMO->getNativeVlanIntfConfig());

				if(pNativeVlanMO.operator->() != NULL)
				{
					m->setNativeVlanId(pNativeVlanMO->getNativeVlanId());
					m->setNativevlanDisable(pNativeVlanMO->getNativevlanDisable());
				}

				m->setCmdCode(NSMUPDATE_PO_BULK_CONF);
				sendSynchronouslyToWaveClient ("nsm",m);
				delete m;
			}

		} else if(swMode == SW_MODE_PVLAN_TRUNK) {
			//first send Native Vlan
			if(pPoMO->getPvlanNativeVlanId())
			{
				NsmPoIntfMessage *m = new NsmPoIntfMessage();
				m->setPoId(ifName);
				m->setPvlanNativeVlanId(pPoMO->getPvlanNativeVlanId());
				m->setCmdCode(NSMUPDATE_PO_PVLAN_NATIVE_VLANID);
    	    	sendSynchronouslyToWaveClient ("nsm", m);
				delete m;
			}
			//send vlanRangeAdd or (isAllowedVlanAll and [vlanRangeRemove])
			if(pPoMO->getPvlanRangeAdd().toString() != "")
			{
				NsmPoIntfMessage *m = new NsmPoIntfMessage();
				m->setPoId(ifName);
				m->setPvlanRange(pPoMO->getPvlanRangeAdd());
				m->setCmdCode(NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ADD);
    	    	sendSynchronouslyToWaveClient ("nsm", m);
				delete m;
			} else if(pPoMO->getPvlanIsAllowedVlanAll()){
				/*code for is allowedVlanAll*/
				NsmPoIntfMessage *m = new NsmPoIntfMessage();
				m->setPoId(ifName);
				m->setPvlanIsAllowedVlanAll(true);
				m->setCmdCode(NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ALL);
    	    	sendSynchronouslyToWaveClient ("nsm", m);
				delete m;
				if(pPoMO->getPvlanRangeRemove().toString() != "")
				{
					NsmPoIntfMessage *pMsg = new NsmPoIntfMessage();
					pMsg->setPoId(ifName);
					pMsg->setPvlanRange(pPoMO->getPvlanRangeRemove());
					pMsg->setCmdCode(NSMUPDATE_PO_PVLAN_TRUNK_VLAN_REMOVE);
    	    		sendSynchronouslyToWaveClient ("nsm", pMsg);
					delete pMsg;
				}
			}
			vector<WaveManagedObjectPointer<PVlanTrunkAssocManagedObject> > trunkAssocVector = pPoMO->getPVlanTrunkAssoc();
			if(trunkAssocVector.size())
			{
				for (unsigned int idx = 0; idx < trunkAssocVector.size(); idx++)
				{
					NsmClientPVlanTrunkAssocMessage *m = new NsmClientPVlanTrunkAssocMessage();
					m->setIfName(ifName);
					m->setIfType(IF_TYPE_PO);
					m->setPrimaryVlanId(trunkAssocVector[idx]->getPrimaryVlanId());
					m->setSecondaryVlanId(trunkAssocVector[idx]->getSecondaryVlanId());
					m->setOpcode(PVLAN_TRUNK_ASSOC_PO);
   		    		sendSynchronouslyToWaveClient ("nsm", m);
					delete m;
				}
			}
		}
		return;
	}

    void NsmLocalObjectManager::configReplayPhyPVlan(PhyIntfLocalManagedObject *pPhyMO, const string &ifName)
	{
		swModeEnum swMode;
		if (pPhyMO->getPvlanModeTrunkHost()) {
			swMode = SW_MODE_PVLAN_TRUNK_HOST;
		} else if (pPhyMO->getPvlanModeHost()) {
			swMode = SW_MODE_PVLAN_HOST;
		} else if (pPhyMO->getPvlanModeTrunkPromis()) {
			swMode = SW_MODE_PVLAN_TRUNK_PROMISCUOUS;
		} else if (pPhyMO->getPvlanModePromiscuous()) {
			swMode = SW_MODE_PVLAN_PROMISCUOUS;
		} else {
			swMode = SW_MODE_PVLAN_TRUNK;
		}
				
		{
			//Set the PVLAN mode first
			NsmPhyIntfMessage *m = new NsmPhyIntfMessage();
    	    m->setIfType(pPhyMO->getPhyType());
   	    	m->setIfName(ifName);
			m->setSwMode(swMode);
			switch (swMode)
			{
				case SW_MODE_PVLAN_HOST:
					m->setCmdCode(NSMUPDATE_PHY_PVLAN_MODE_HOST);
					break;
				case SW_MODE_PVLAN_TRUNK_HOST:
					m->setCmdCode(NSMUPDATE_PHY_PVLAN_MODE_TRUNK_HOST);
					break;
				case SW_MODE_PVLAN_PROMISCUOUS:
					m->setCmdCode(NSMUPDATE_PHY_PVLAN_MODE_PROMISCUOUS);
					break;
				case SW_MODE_PVLAN_TRUNK_PROMISCUOUS:
					m->setCmdCode(NSMUPDATE_PHY_PVLAN_MODE_TRUNK_PROMISCUOUS);
					break;
				case SW_MODE_PVLAN_TRUNK:
					m->setCmdCode(NSMUPDATE_PHY_PVLAN_MODE_TRUNK);
					break;
				default:
					trace(TRACE_LEVEL_ERROR, string("This case shouldn't occur"));
					delete m;
					return;
			}	
    	    sendSynchronouslyToWaveClient ("nsm", m);
			delete m;
		}
		if((swMode == SW_MODE_PVLAN_HOST) || (swMode == SW_MODE_PVLAN_TRUNK_HOST))
		{
			if(pPhyMO->getPvlanHostPrimVlanId() && pPhyMO->getPvlanHostSecVlanId())
			{
				NsmPhyIntfMessage *m = new NsmPhyIntfMessage();
    		    m->setIfType(pPhyMO->getPhyType());
   	    		m->setIfName(ifName);
				m->setPvlanHostPrimVlanId(pPhyMO->getPvlanHostPrimVlanId());
				m->setPvlanHostSecVlanId(pPhyMO->getPvlanHostSecVlanId());
				m->setCmdCode(NSMUPDATE_PHY_PVLAN_HOST_ASSOCIATION);
    	    	sendSynchronouslyToWaveClient ("nsm", m);
				delete m;
			}	
		} else if((swMode == SW_MODE_PVLAN_PROMISCUOUS) || (swMode == SW_MODE_PVLAN_TRUNK_PROMISCUOUS))	{
			vector<WaveManagedObjectPointer<PVlanMappingManagedObject> > mappingVector = pPhyMO->getPVlanMapping();
			if(mappingVector.size())
			{
				for (unsigned int idx = 0; idx < mappingVector.size(); idx++)
				{
					NsmClientPVlanMappingMessage *m = new NsmClientPVlanMappingMessage();
					m->setIfName(ifName);
					m->setIfType(pPhyMO->getPhyType());
					m->setPrimaryVlanId(mappingVector[idx]->getPrimaryVlanId());
					m->setSecVlanRange(mappingVector[idx]->getSecPvlanRange());
					m->setoper(PVLAN_MAPPING_ADD);
					m->setOpcode(PVLAN_MAPPING_PHY);
    	    		sendSynchronouslyToWaveClient ("nsm", m);
					delete m;
				}
			}

			if(swMode == SW_MODE_PVLAN_TRUNK_PROMISCUOUS)
			{
				NsmPhyIntfMessage *m = new NsmPhyIntfMessage();
				m->setIfType(pPhyMO->getPhyType());
				m->setIfName(ifName);

				/* sw trunk allowed vlan all */
				if(pPhyMO->getIsAllowedVlanAll())
				{
					m->setIsAllowedVlanAll(pPhyMO->getIsAllowedVlanAll());
					if(pPhyMO->getVlanRangeRemove() != UI32Range(""))
					{
						m->setVlanRange(pPhyMO->getVlanRangeRemove(), NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN_CMD);
					}
				}
				else
				{
					/* sw trunk allowed vlan add 5 */
					/* Requried to handle for multiple add / remove, except */
					if(pPhyMO->getVlanRangeAdd() != UI32Range(""))
					{
						m->setVlanRange(pPhyMO->getVlanRangeAdd(), NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN_CMD);
					}
				}

				WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanMO;
				pNativeVlanMO = WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (pPhyMO->getNativeVlanIntfConfig());
				if(pNativeVlanMO.operator->() != NULL)
				{
					m->setNativeVlanId(pNativeVlanMO->getNativeVlanId());
					m->setNativevlanDisable(pNativeVlanMO->getNativevlanDisable());
				}

				m->setCmdCode(NSMUPDATE_PHY_BULK_CONF);
				sendSynchronouslyToWaveClient ("nsm",m);
				delete m;
			}

		} else if(swMode == SW_MODE_PVLAN_TRUNK) {
			//first send Native Vlan
			if(pPhyMO->getPvlanNativeVlanId())
			{
				NsmPhyIntfMessage *m = new NsmPhyIntfMessage();
   			    m->setIfType(pPhyMO->getPhyType());
				m->setIfName(ifName);
				m->setPvlanNativeVlanId(pPhyMO->getPvlanNativeVlanId());
				m->setCmdCode(NSMUPDATE_PHY_PVLAN_NATIVE_VLANID);
    	    	sendSynchronouslyToWaveClient ("nsm", m);
				delete m;
			}
			//send vlanRangeAdd or (isAllowedVlanAll and [vlanRangeRemove])
			if(pPhyMO->getPvlanRangeAdd().toString() != "")
			{
				NsmPhyIntfMessage *m = new NsmPhyIntfMessage();
   			    m->setIfType(pPhyMO->getPhyType());
				m->setIfName(ifName);
				m->setPvlanRange(pPhyMO->getPvlanRangeAdd());
				m->setCmdCode(NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ADD);
    	    	sendSynchronouslyToWaveClient ("nsm", m);
				delete m;
			} else if(pPhyMO->getPvlanIsAllowedVlanAll()){
				/*code for is allowedVlanAll*/
				NsmPhyIntfMessage *m = new NsmPhyIntfMessage();
   			    m->setIfType(pPhyMO->getPhyType());
				m->setIfName(ifName);
				m->setPvlanIsAllowedVlanAll(true);
				m->setCmdCode(NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ALL);
    	    	sendSynchronouslyToWaveClient ("nsm", m);
				delete m;
				if(pPhyMO->getPvlanRangeRemove().toString() != "")
				{
					NsmPhyIntfMessage *pMsg = new NsmPhyIntfMessage();
	  			    pMsg->setIfType(pPhyMO->getPhyType());
					pMsg->setIfName(ifName);
					pMsg->setPvlanRange(pPhyMO->getPvlanRangeRemove());
					pMsg->setCmdCode(NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_REMOVE);
    	    		sendSynchronouslyToWaveClient ("nsm", pMsg);
					delete pMsg;
				}
			}
			vector<WaveManagedObjectPointer<PVlanTrunkAssocManagedObject> > trunkAssocVector = pPhyMO->getPVlanTrunkAssoc();
			if(trunkAssocVector.size())
			{
				for (unsigned int idx = 0; idx < trunkAssocVector.size(); idx++)
				{
					NsmClientPVlanTrunkAssocMessage *m = new NsmClientPVlanTrunkAssocMessage();
					m->setIfName(ifName);
					m->setIfType(pPhyMO->getPhyType());
					m->setPrimaryVlanId(trunkAssocVector[idx]->getPrimaryVlanId());
					m->setSecondaryVlanId(trunkAssocVector[idx]->getSecondaryVlanId());
					m->setOpcode(PVLAN_TRUNK_ASSOC_PHY);
   		    		sendSynchronouslyToWaveClient ("nsm", m);
					delete m;
				}
			}
		}
		return;
	}

    ResourceId NsmLocalObjectManager::provisionVlansForPoPhy(
            const PoPhyIntfManagedObject *pPoPhyMo) {
        string ifName = pPoPhyMo->getIfName();

        ResourceId provisionStatus = WAVE_MESSAGE_SUCCESS;
        tracePrintf(TRACE_LEVEL_INFO, "Populating vlan info for ifName %s",
                ifName.c_str());
        VlanInformation vlanInformation;
        NsmUtils::populateVlanInformation(vlanInformation, INVALID_SLOT_ID, ifName);
        if (!vlanInformation.provisionedVlans.isEmpty()) {
            UI32Range diffToProvision;
            diffToProvision = vlanInformation.provisionedVlans
                    - ProvisioningUtil::getProviosionedMORange();
            if (!diffToProvision.isEmpty()) {
                GvlanConfigUtils util(NULL);
                provisionStatus = util.provision(diffToProvision,
                        PROVISION_SPECIFIED);

            } else {
                tracePrintf(TRACE_LEVEL_INFO, "No vlans to be provisioned");
            }
        }
        return provisionStatus;

    }
    ResourceId NsmLocalObjectManager::Pass1PhyConfiguration(
            PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject,
            CfgReplayModes cfgReplayMode)
    {
        UI32 status = 0;
        string ifName;

        if(pPhyIntfLocalManagedObject == NULL )
        {
			trace (TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::Pass1PhyConfiguration:pPhyIntfLocalManagedObject = NULL"));
            return status;
        }
        /* create Phy interface */
        ifName = pPhyIntfLocalManagedObject->getIfName();
        
		NSM_PLUG_INFO_S ( string("Entering NsmLocalObjectManager::Pass1PhyConfiguration ..") +
                ifName);

        
        if (CFGREPLAY_ISL_TO_EDGE == cfgReplayMode &&
                ProvisioningUtil::isGvlanEnabled()) {
            ResourceId provisionStatus = provisionVlansForPoPhy(
                    pPhyIntfLocalManagedObject);
            if (WAVE_MESSAGE_SUCCESS != provisionStatus) {
                tracePrintf(TRACE_LEVEL_ERROR,
                        "Failed to configure Vlans for Phy %s status=%u",
                        ifName.c_str(), provisionStatus);
            }

        }

        NsmPhyIntfMessage *m = new NsmPhyIntfMessage();

        bool isProfilePort = false;

        status = APPMUtils::isPortProfilePort(pPhyIntfLocalManagedObject, isProfilePort);
        if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
        {
            NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
        }
        
        InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
        NSM_PLUG_INFO_S(string(__FUNCTION__) +
            " ifName: " + ifName + " ifType=" + ifType + "\n");

        m->setIfType(ifType);
        m->setIfName(ifName);

        /* create Phy desc */
        if(!(pPhyIntfLocalManagedObject->getDesc()).empty())
        {
            trace (TRACE_LEVEL_DEBUG, string ("set Phy Desc ") + m->getBulkCmdCode());
            m->setDesc(pPhyIntfLocalManagedObject->getDesc());
        }

        /* set Phy [no] shutdown */
        /* Shutdown will play at the last phase */
        //    trace (TRACE_LEVEL_DEBUG, string ("set Phy Flag ") + m->getBulkCmdCode());
        //    m->setFlag(pPhyIntfLocalManagedObject->getStatus());

        /* set Phy [no] mtu */
        if(pPhyIntfLocalManagedObject->getMtu() != NSM_MTU_DEF_VAL )
        {
            trace (TRACE_LEVEL_DEBUG, string ("set Phy Mtu ") + m->getBulkCmdCode());
            m->setMtu(pPhyIntfLocalManagedObject->getMtu());
        }

        /* set Phy [no] vepa */
        if(pPhyIntfLocalManagedObject->getVepa() != false)
        {
            NSM_PLUG_INFO_S( string ("set Phy Vepa ") + m->getBulkCmdCode());
            m->setVepa(pPhyIntfLocalManagedObject->getVepa());
        }

        if(pPhyIntfLocalManagedObject->getLineSpeed() != LINE_SPEED_AUTO)
        {
            NSM_PLUG_INFO_S( string ("set Phy LineSpeed") + m->getBulkCmdCode());
            m->setLineSpeed(pPhyIntfLocalManagedObject->getLineSpeed());
        }

        if(pPhyIntfLocalManagedObject->getSwModeFlag())
        {
            /* set sw */
            trace (TRACE_LEVEL_DEBUG, string ("set Phy Sw ") + m->getBulkCmdCode());
            m->setSwModeFlag(pPhyIntfLocalManagedObject->getSwModeFlag());

            /* sw mode trunk */  
            trace (TRACE_LEVEL_DEBUG, string ("set Phy Sw Mode ") + m->getBulkCmdCode());
            m->setSwMode(pPhyIntfLocalManagedObject->getSwMode());
            if(pPhyIntfLocalManagedObject->getRaGuard())
                m->setRaGuard(pPhyIntfLocalManagedObject->getRaGuard());

            if(pPhyIntfLocalManagedObject->getSwMode() == SW_MODE_TRUNK )
            {
                /* sw trunk allowed vlan all */ 
                if(pPhyIntfLocalManagedObject->getIsAllowedVlanAll()) {
                    trace (TRACE_LEVEL_DEBUG, string ("set Phy Sw allowed all ") + m->getBulkCmdCode());
                    m->setIsAllowedVlanAll(pPhyIntfLocalManagedObject->getIsAllowedVlanAll());
					if(pPhyIntfLocalManagedObject->getVlanRangeRemove() != UI32Range(""))
						m->setVlanRange(pPhyIntfLocalManagedObject->getVlanRangeRemove(), NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN_CMD);
                }else {
                    /* sw trunk allowed vlan add 5 */
                    /* Requried to handle for multiple add / remove, except */
                    if(pPhyIntfLocalManagedObject->getVlanRangeAdd() != UI32Range(""))
                    {
                        trace (TRACE_LEVEL_DEBUG, string ("set Phy Sw trunk add vlan id") + m->getBulkCmdCode());
						m->setVlanRange(pPhyIntfLocalManagedObject->getVlanRangeAdd(), NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN_CMD);
                    }
                }
				WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanIntfConfLocalManagedObject;
				pNativeVlanIntfConfLocalManagedObject = WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (pPhyIntfLocalManagedObject->getNativeVlanIntfConfig());
			    if(pNativeVlanIntfConfLocalManagedObject.operator->() != NULL) {
					m->setNativeVlanId(pNativeVlanIntfConfLocalManagedObject->getNativeVlanId());
					m->setNativevlanDisable(pNativeVlanIntfConfLocalManagedObject->getNativevlanDisable());
                trace (TRACE_LEVEL_DEBUG, string ("set Phy Sw trunk native-vlan:: id ::" 
										+ m->getNativeVlanId()));
            }
            }
            else if (pPhyIntfLocalManagedObject->getSwMode() == SW_MODE_ACCESS )
            {
            	UI32 rspanAccessVlan = pPhyIntfLocalManagedObject->getAccessRspanVlan();
            	NSM_PLUG_INFO_S(string(" rspanAccessVlan=")+rspanAccessVlan);
            	if(rspanAccessVlan != DCM_NSM_VLAN_DEFAULT_VID) {
        	    	stringstream vlanif_str;
        	    	vlanif_str << rspanAccessVlan;
        	    	string accessVlanString = vlanif_str.str();
        	    	NSM_PLUG_INFO_S(string(" accessVlanString=")+accessVlanString);
    				UI32Range accessVlan;
    				accessVlan.fromString(accessVlanString);
    				m->setVlanRange(accessVlan,NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN_CMD);
    				NSM_PLUG_INFO_S(string(" Sending the Rspan Access Vlan ")+accessVlanString+" to the backend for replay");
            	}
            	else {
					// Access Phy - TODO
					ObjectId obj = pPhyIntfLocalManagedObject->getAccessVlanObjectId();
					UI32Range accessVlan;
					accessVlan.fromString(NsmUtils::getVlanIntfNameByObjectId(obj));
					trace (TRACE_LEVEL_DEBUG, string ("set Phy Sw allowed access add vlan id") + m->getBulkCmdCode());
					m->setVlanRange(accessVlan,NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN_CMD);
            	}
			}
        } else if (pPhyIntfLocalManagedObject->getIsPortProfiled()) {
		  m->setIsPortProfiled(true , NSMUPDATE_PHY_ADD_PORTPROFILEPORT_CMD);
        } else if (isProfilePort) {
            configReplayPhyIfToPortProfileAssociation(pPhyIntfLocalManagedObject, ifName);
        }

        // channel group TODO  
        if(pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
        {
            string poName = NsmUtils::getPoIntfNameByObjectId(pPhyIntfLocalManagedObject->getPoId());
            trace (TRACE_LEVEL_DEBUG, string ("set Phy Channel Group ") + poName);

            if(!poName.empty() && (strtoul(poName.c_str (), NULL, 0) > 0))
            {
            	m->setPortChannelName(poName,pPhyIntfLocalManagedObject->getPoMode(),pPhyIntfLocalManagedObject->getPoType());
            }
        }

		/* setting Phy [no] neighbor-discovery disable */
		if (pPhyIntfLocalManagedObject->getNbrDiscoveryDisable() != false) {
       		m->setNbrDiscoveryDisable(pPhyIntfLocalManagedObject->getNbrDiscoveryDisable());
        }

        m->setCmdCode(NSMUPDATE_PHY_BULK_CONF);

        status = sendSynchronouslyToWaveClient ("nsm",m);
		
		NSM_PLUG_INFO_S ( string("End:: NsmLocalObjectManager::Pass1PhyConfiguration:: status ") +
						status);
		//bpatel: To avoid memory leak
		if(m){
			delete m;
		}

		if(pPhyIntfLocalManagedObject->getSwModeFlag())
		{
			configReplayPhyVlanClassification(pPhyIntfLocalManagedObject, ifName);

			if(pPhyIntfLocalManagedObject->getSwMode() == SW_MODE_UNKNOWN)
			{
				configReplayPhyPVlan(pPhyIntfLocalManagedObject, ifName);
			} else if(pPhyIntfLocalManagedObject->getSwMode() == SW_MODE_TRUNK)
			{
				configReplaySwitchportRspanVlan(pPhyIntfLocalManagedObject,ifName);
			}
		}

        return status;
    }


void NsmLocalObjectManager::configReplayPhyIfToPortProfileAssociation
(PhyIntfLocalManagedObject *pMO, const string &ifName) {

    if(pMO == NULL ) {
        trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::configReplayPhyIfToPortProfileAssociation: pMO is NULL."));
        return;
    }

    InterfaceType ifType = pMO->getPhyType();
    UI32 index = 0;
    UI32 cmdCode = 0;
    string portProfileDomain = "";
    vector <string> portProfiles;

    PortProfileInterfaceAssociationMessage *m = new PortProfileInterfaceAssociationMessage();

    WaveManagedObjectPointer<AMPPProfileDomainCompositeManagedObject> associatedPortProfileDomain = 
        pMO->getAssociatedPortProfileDomain() ;
    vector<WaveManagedObjectPointer<AMPPProfileCompositeManagedObject> > associatedPortProfiles = 
        pMO->getAssociatedPortProfiles();

    if(NULL != associatedPortProfileDomain.operator->()) {
        cmdCode = NSMUPDATE_ASSOCIATE_BULK_PORT_PROFILE_DOMAIN_WITH_PHY_PORT;
        portProfileDomain = associatedPortProfileDomain->getPortProfileDomainName();
    } else if(associatedPortProfiles.size() > 0 ) {
        cmdCode = NSMUPDATE_ASSOCIATE_BULK_PORT_PROFILE_WITH_PHY_PORT;
    }

    for(index = 0 ; index < associatedPortProfiles.size(); index++) {
        string profileName = associatedPortProfiles[index]->getPortProfileName();
        portProfiles.push_back(profileName);
    }

    m->setCmdCode(cmdCode);
    m->setIfType(ifType);
    m->setIfName(ifName);
    m->setAssociatedPortProfiles(portProfiles);
    m->setAssociatedPortProfileDomain(portProfileDomain);

    ResourceId status = sendSynchronouslyToWaveClient ("nsm",m);

    if (status == WAVE_MESSAGE_SUCCESS) {
        if(WAVE_MESSAGE_SUCCESS == m->getCompletionStatus()) {
            if(WAVE_MESSAGE_SUCCESS != m->getClientStatus()) {
                trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::configReplayPhyIfToPortProfileAssociation: client status error<"));
            }
        } else {
            trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::configReplayPhyIfToPortProfileAssociation: completion status error"));
        }
    } else {
        trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::configReplayPhyIfToPortProfileAssociation: sendToClient failed"));
    }

    delete m;

}


void NsmLocalObjectManager::configReplayPoIfToPortProfileAssociation
(PoIntfManagedObject *pMO, const string &ifName) {

    if(pMO == NULL ) {
        trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::configReplayPoIfToPortProfileAssociation: pMO is NULL."));
        return;
    }

    InterfaceType ifType = IF_TYPE_PO;
    UI32 index = 0;
    UI32 cmdCode = 0;
    string portProfileDomain = "";
    vector <string> portProfiles;

    PortProfileInterfaceAssociationMessage *m = new PortProfileInterfaceAssociationMessage();

    WaveManagedObjectPointer<AMPPProfileDomainCompositeManagedObject> associatedPortProfileDomain =
        pMO->getAssociatedPortProfileDomain() ;
    vector<WaveManagedObjectPointer<AMPPProfileCompositeManagedObject> > associatedPortProfiles =
        pMO->getAssociatedPortProfiles();

    if(NULL != associatedPortProfileDomain.operator->()) {
        cmdCode = NSMUPDATE_ASSOCIATE_BULK_PORT_PROFILE_DOMAIN_WITH_PO;
        portProfileDomain = associatedPortProfileDomain->getPortProfileDomainName();
    } else if(associatedPortProfiles.size() > 0 ) {
        cmdCode = NSMUPDATE_ASSOCIATE_BULK_PORT_PROFILE_WITH_PO;
    }

    for(index = 0 ; index < associatedPortProfiles.size(); index++) {
        string profileName = associatedPortProfiles[index]->getPortProfileName();
        portProfiles.push_back(profileName);
    }

    m->setCmdCode(cmdCode);
    m->setIfType(ifType);
    m->setIfName(ifName);
    m->setAssociatedPortProfiles(portProfiles);
    m->setAssociatedPortProfileDomain(portProfileDomain);

    ResourceId status = sendSynchronouslyToWaveClient ("nsm",m);

    if (status == WAVE_MESSAGE_SUCCESS) {
        if(WAVE_MESSAGE_SUCCESS == m->getCompletionStatus()) {
            if(WAVE_MESSAGE_SUCCESS != m->getClientStatus()) {
                trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::configReplayPoIfToPortProfileAssociation: client status error<"));
            }
        } else {
            trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::configReplayPoIfToPortProfileAssociation: completion status error"));
        }
    } else {
        trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::configReplayPoIfToPortProfileAssociation: sendToClient failed"));
    }

    delete m;

}





    void NsmLocalObjectManager::postBootPassLongDistanceConfiguration()
    {
        trace (TRACE_LEVEL_DEBUG, "Entering NsmLocalObjectManager::postBootPassLongDistanceConfiguration ..");
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
        LocationId                          locationId = 0;
        locationId = FrameworkToolKit::getThisLocationId ();
        vector<WaveManagedObject *> *pPhyResults = querySynchronouslyLocalManagedObjectsForLocationId (locationId, PhyIntfLocalManagedObject::getClassName ());
        if (pPhyResults && pPhyResults->size() > 0) {
            for (UI32 PhyIndex = 0; PhyIndex < pPhyResults->size(); PhyIndex++)
            {
                pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pPhyResults)[PhyIndex]);
				if(pPhyIntfLocalManagedObject == NULL )
				{
					continue;
				}
				if (pPhyIntfLocalManagedObject->getDistance() == LONG_DISTANCE_200) 
				{									
					continue;
				}
				PassLongDistancePhyConfiguration(pPhyIntfLocalManagedObject);
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pPhyResults);
        }
    }


    //Below method is not used.
    void NsmLocalObjectManager::ConfigReplayAppmPortProfileConfig(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
		trace (TRACE_LEVEL_DEVEL, "NsmLocalObjectManager::ConfigReplayAppmPortProfileConfig: Entered....");
		NsmPhyIntfMessage *m = NULL;
        string ifName;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (pPhyIntfLocalManagedObject->getIsPortProfiled()) {
			/*
			 * If its a profiled port then send the
			 * message to backend, no need to send it to
			 * SSM for validation now as having a config itself 
			 * means that validation was a success.
			 */

			m = new NsmPhyIntfMessage();

			/*
			 * Convert IfName from 3 tuple to 2 tuple.
			 */

			ifName = pPhyIntfLocalManagedObject->getIfName();

			if(true  == NsmUtils::isValidThreeTuple(ifName) ) {
				string twoTuppleIfName;
				NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
				ifName = twoTuppleIfName;
			}

		    InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
            m->setIfType(ifType);
            NSM_PLUG_INFO_S(string(__FUNCTION__) +
                " ifName: " + ifName +
                " ifType=" + m->getIfType() + "\n");

			m->setCmdCode(NSMUPDATE_PHY_PORT_PROFILED);
			m->setIsPortProfiled(true);
			m->setIfName(ifName);
			status = sendSynchronouslyToWaveClient ("nsm",m);
			if (status == WAVE_MESSAGE_SUCCESS) {
				trace(TRACE_LEVEL_SUCCESS, string("NsmLocalObjectManager::ConfigReplayAppmPortProfileConfig: sendToClient success"));
			} else {
				trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::ConfigReplayAppmPortProfileConfig: sendToClient failed"));
			}

			delete m;
		}
    }

    void NsmLocalObjectManager::postBootPass1PhyConfiguration (
        int32_t slotId, LocationId locId)
    {
        NSM_PLUG_INFO_S (string("PHY-PASS1::Entering..") +
            "SlotId = " + slotId + " LocationId = " + locId);

        vector<PhyIntfLocalManagedObject *> results;
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        NsmUtils::SlotGetPhyIntfMOs(results, slotId, locId);

        for(unsigned int i = 0; i <  results.size(); i++) {
            pPhyIntfLocalManagedObject = results[i];
            Pass1PhyConfiguration(pPhyIntfLocalManagedObject, CFGREPLAY_POSTBOOT);
            delete pPhyIntfLocalManagedObject;
        }

        return;
    }

    void NsmLocalObjectManager::postBootPhyConfiguration (
        int32_t slotId, LocationId locId)
    {
        string func_name = "NsmLocalObjectManager::postBootPhyConfiguration() ";

        trace (TRACE_LEVEL_INFO, string("Entering .. ") + func_name + 
            "SlotId = " + slotId +
            " LocationId = " + locId);
        if (ProvisioningUtil::isGvlanEnabled()) {
            configReplayForVlan(slotId);
        }

        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
        vector<PhyIntfLocalManagedObject *> results;

        NsmUtils::SlotGetPhyIntfMOs(results, slotId, locId);
        trace (TRACE_LEVEL_INFO, string("Entering .. ") + func_name + 
            "SlotId = " + slotId +
            " LocationId = " + locId +
            " number of phy-intf found: " + results.size());

        for (UI32 i = 0; i < results.size(); i++) {
            pPhyIntfLocalManagedObject = results[i];

            if(pPhyIntfLocalManagedObject->getPortrole() == false)
            {
                 NSM_PLUG_INFO_S( string("Reply configuration Edge Port :")
                     + pPhyIntfLocalManagedObject->getIfName());
                 m_pIntfConfLocalWorker->replyConfigPhyIntfMO(
                     pPhyIntfLocalManagedObject, CFGREPLAY_POSTBOOT);
                 m_pIntfUdldConfLocalWorker->postbootIntfUdldConfig(pPhyIntfLocalManagedObject);
            }
            else
            {
                 NSM_PLUG_INFO_S( string("Replay only Sflow and Span configuration on ISL Port :"
                     ) + pPhyIntfLocalManagedObject->getIfName());
                 (NsmLocalObjectManager::getInstance())->configReplaySflowInterface(pPhyIntfLocalManagedObject);
                 (NsmLocalObjectManager::getInstance())->configReplaySpanInterface(pPhyIntfLocalManagedObject);
                 (NsmLocalObjectManager::getInstance())->configReplayInterfaceDesc(pPhyIntfLocalManagedObject);
            }
            delete pPhyIntfLocalManagedObject;
        }

        /*Replay IGMP Snooping configuration*/
        ConfigReplayIgmpsIntfConfig();

        return;
    }

    void NsmLocalObjectManager::postBootPass1Configuration (vector<WaveManagedObject *>* pVlanResults, const bool poCreateFlag)
    {
        trace (TRACE_LEVEL_DEBUG, "Entering NsmLocalObjectManager::"
            "postBootPass1Configuration ..");

        postBootPass1VlanConfiguration(pVlanResults);
        if (poCreateFlag) {
        postBootPass1PoConfiguration();
        configReplayXstpVlanConfig();
        } else {
            configReplayXstpVlanConfig(pVlanResults);
        }

    }

    void NsmLocalObjectManager::postbootXstpConfig(vector<PoIntfManagedObject *> &pos)
    {
        LocationId loc = FrameworkToolKit::getMyLocationId();
        PoIntfManagedObject *po = NULL;

        for (UI32 i = 0; i < pos.size(); ++i) {
            po = pos[i];

            // playback STP configs for POs with member on this node and
            // which are in switchport mode
            if (!po->getSwModeFlag() || (loc != 0 &&
                    NsmUtils::isPoIntfHasMemberByLocation(po->getObjectId(), loc) != WAVE_MESSAGE_SUCCESS)) {
                continue;
            }

            configReplayBpduGuardConfig(NULL, po);
            configReplayXstpPortConfig(po);
            configReplayXstpPortInstanceConfig(po);
            configReplayXstpPortVlanConfig(po);
        }
    }

    void NsmLocalObjectManager::configReplayXstpPortConfig (
        PoPhyIntfManagedObject *pPoPhyMo)
    {
	    ResourceId status = WAVE_MESSAGE_ERROR;
	    XstpPortConfigManagedObject *pXstpPortMo = NULL;

        if (pPoPhyMo == NULL || !pPoPhyMo->getSwModeFlag()) return;

        pXstpPortMo = (pPoPhyMo->getPortStpConfig()).operator->();
        if (pXstpPortMo == NULL) return;

        MstpPortConfigMessage *pMsg = new MstpPortConfigMessage();
        if (pMsg == NULL) return;

        pXstpPortMo->getAttributeToMessage(pMsg);
        pMsg->setCmdCode(MSTP_MSG_POST_BOOT_IF_XSTP_CONFIG);

        string ifName = pXstpPortMo->getPort();

        // TODO optimize
        if (pXstpPortMo->getStpEnable() && MstpToolKit::isVcsEnabled()) {
            InterfaceType ifType = NsmUtils::getIfType(pXstpPortMo->getOwnerManagedObjectId());
            UI32 uid = 0;

            status = MstpToolKit::allocatePortId(uid, ifType, ifName);

            if (status == WAVE_MESSAGE_SUCCESS) {
                pMsg->setPortId(uid);
            }
            else if (status != WRC_UIDSVC_ID_EXHAUSTED_ERROR 
                    && (uid = pXstpPortMo->getPortUid()) > 0) {
                string port = NsmUtils::getIfShortName(ifType, ifName);
                tracePrintf(TRACE_LEVEL_WARN, "NsmLocalObjectManager::configReplayXstpPortConfig : Cannot allocate port id for %s; err=%u ( %s ); using fallback value %u",
                        port.c_str(), status, FrameworkToolKit::localizeToSourceCodeEnum(status).c_str(), uid);
                pMsg->setPortId(uid);
            }
            else {
                MstpToolKit::logPortIdAllocError(status, "postboot", ifType, ifName);
                //pMsg->setStpEnable(false);
            }
        }

        pMsg->printMsg();

        status = MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, pMsg);

        if (status == WAVE_MESSAGE_SUCCESS) {
            tracePrintf(TRACE_LEVEL_SUCCESS, true, false, "NsmLocalObjectManager::%s : %s configured", __FUNCTION__, ifName.c_str());
        } 
        else {
             tracePrintf(TRACE_LEVEL_ERROR, true, false, "NsmLocalObjectManager::%s : %s configuration failed", __FUNCTION__, ifName.c_str());
        }

        delete pMsg;

        return;
    }

    void NsmLocalObjectManager::configReplayXstpPortInstanceConfig (
        PoPhyIntfManagedObject *pPoPhyMo)
    {
	    ResourceId status = WAVE_MESSAGE_ERROR;

        if (pPoPhyMo == NULL) return;

        XstpPortInstanceConfigManagedObject * pXstpPortInstanceMo = NULL;
        vector<WaveManagedObjectPointer<XstpPortInstanceConfigManagedObject> >
            xStpPortInstanceConfigMoVector = pPoPhyMo->getPortInstanceXstpConfig();

        for (UI32 i = 0; i < xStpPortInstanceConfigMoVector.size(); ++i) {
            pXstpPortInstanceMo = 
                (xStpPortInstanceConfigMoVector[i]).operator->();
            
            MstpPortConfigMessage *pMsg = new MstpPortConfigMessage();
            if (pMsg == NULL) return;

            pXstpPortInstanceMo->getAttributeToMessage(pMsg);
            pMsg->setCmdCode(MSTP_MSG_POST_BOOT_IF_INST_MSTP_CONFIG);

            pMsg->printMsg();

            status = MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, pMsg);

            if (status == WAVE_MESSAGE_SUCCESS) {
                tracePrintf(TRACE_LEVEL_SUCCESS, true, false, "NsmLocalObjectManager::%s : %s, inst %u configured", __FUNCTION__, pXstpPortInstanceMo->getPort().c_str(), (UI32) pXstpPortInstanceMo->getId());
            } 
            else {
                 tracePrintf(TRACE_LEVEL_ERROR, true, false, "NsmLocalObjectManager::%s : %s, inst %u configuration failed", __FUNCTION__, pXstpPortInstanceMo->getPort().c_str(), (UI32) pXstpPortInstanceMo->getId());
            }

            delete pMsg;
        }

        return;
    }

    void NsmLocalObjectManager::configReplayXstpPortVlanConfig (
        PoPhyIntfManagedObject *pPoPhyMo)
    {
	    ResourceId status = WAVE_MESSAGE_ERROR;

        if (pPoPhyMo == NULL) return;

        vector<WaveManagedObjectPointer<XstpPortVlanConfigManagedObject> >
            xStpPortVlanMoVector = pPoPhyMo->getPortVlanXstpConfig();
        XstpPortVlanConfigManagedObject * pPortVlanMo = NULL;

        for (UI32 i = 0; i < xStpPortVlanMoVector.size(); ++i) {
            pPortVlanMo = (xStpPortVlanMoVector[i]).operator->();
 
            MstpPortVlanConfigMessage *pMsg = new MstpPortVlanConfigMessage();
            if (pMsg == NULL) break;

            pPortVlanMo->getAttributeToMessage(pMsg);
            pMsg->setCmdCode(MSTP_MSG_POST_BOOT_IF_VLAN_CONFIG);

            status = MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, pMsg);

            if (status == WAVE_MESSAGE_SUCCESS) {
                tracePrintf(TRACE_LEVEL_SUCCESS, true, false, "NsmLocalObjectManager::%s : %s, inst %u configured", __FUNCTION__, pPortVlanMo->getPort().c_str(), pPortVlanMo->getId());
            } 
            else {
                tracePrintf(TRACE_LEVEL_ERROR, true, false, "NsmLocalObjectManager::%s : %s, inst %u configuration failed", __FUNCTION__, pPortVlanMo->getPort().c_str(), pPortVlanMo->getId());
            }

            delete pMsg;
        }

        return;
    }


    void NsmLocalObjectManager::configReplayBpduGuardConfig ( PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject,
                                                                PoIntfManagedObject *pPoIntfManagedObject )
    {
        XstpPortConfigManagedObject *pXstpPortMo = NULL;

        if (pPhyIntfLocalManagedObject != NULL)  {
            tracePrintf(TRACE_LEVEL_DEBUG, "NsmLocalObjectManager::configReplayBpduGuardConfig : Called for PhyIntf %s", pPhyIntfLocalManagedObject->getId().c_str());
            pXstpPortMo = pPhyIntfLocalManagedObject->getPortStpConfig().operator->();
        } else if (pPoIntfManagedObject != NULL) {
            tracePrintf(TRACE_LEVEL_DEBUG, "NsmLocalObjectManager::configReplayBpduGuardConfig : Called for PoIntf %u", pPoIntfManagedObject->getId());
            pXstpPortMo = pPoIntfManagedObject->getPortStpConfig().operator->();
        } else {
            tracePrintf(TRACE_LEVEL_ERROR, "NsmLocalObjectManager::configReplayBpduGuardConfig : Both intf pointers are NULL :");
            return;
        }

        if (!pXstpPortMo || !(pXstpPortMo->getBpduGuardEnable() || pXstpPortMo->getTaggedBpduEnable())) {
            tracePrintf(TRACE_LEVEL_DEBUG, "NsmLocalObjectManager::configReplayBpduGuardConfig : Nothing to playback..");
            return;
        }

        MstpPortConfigMessage *pMsg = new MstpPortConfigMessage();
        pXstpPortMo->getAttributeToMessage(pMsg);
        pMsg->setCmdCode (DCM_SSM_XSTP_POSTBOOT);

        pMsg->printMsg();

        ResourceId status = sendSynchronouslyToWaveClient ("ssm", pMsg);

        tracePrintf(TRACE_LEVEL_DEBUG, "NsmLocalObjectManager::configReplayBpduGuardConfig : Client status %d", pMsg->getClientStatus());

        if (status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_SUCCESS, "NsmLocalObjectManager::configReplayBpduGuardConfig : configured ");
        } else {
            trace(TRACE_LEVEL_ERROR, "NsmLocalObjectManager::configReplayBpduGuardConfig : configuration failed");
        }
        delete pMsg;
    }

	void  NsmLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig(vector<WaveManagedObject *>* &pResultsPMO)
    {
        UI32 i;
        ResourceId status = WAVE_MESSAGE_ERROR;

        trace (TRACE_LEVEL_INFO, "NsmLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: Entered....");

        WaveManagedObjectPointer<IgmpsVlanIntfConfManagedObject> pIgmpsVlanConfigManagedObject = NULL;

        VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

//      WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
//      vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);
        trace (TRACE_LEVEL_INFO, "NsmLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: Entered....");
        if (NULL == pResultsPMO) {
            return;
        }

        UI32 numberOfResults = pResultsPMO->size ();
        if (0 == numberOfResults) {
            return;
        }
        trace(TRACE_LEVEL_DEVEL, string("NsmLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: NumResults - ")+numberOfResults);
        trace (TRACE_LEVEL_INFO, "NsmLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: Entered....");
        for (i = 0; i < numberOfResults; i++) {
            pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResultsPMO)[i]);
            if (! pVlanIntfManagedObject)
                continue;

            pIgmpsVlanConfigManagedObject = pVlanIntfManagedObject->getIgmpsVlanIntfConfig();
            if(pIgmpsVlanConfigManagedObject.operator->() == NULL)
                continue;

            IgmpsClientUpdateVlanConfigMessage *m = new IgmpsClientUpdateVlanConfigMessage ();

            m->setLmqi(pIgmpsVlanConfigManagedObject->getLmqi());
            m->setQmrt(pIgmpsVlanConfigManagedObject->getQmrt());
            m->setEnable(pIgmpsVlanConfigManagedObject->getEnable());
            m->setFastLeave(pIgmpsVlanConfigManagedObject->getFastLeave());
            m->setQueryInterval(pIgmpsVlanConfigManagedObject->getQueryInterval());
            m->setQuerierEnable(pIgmpsVlanConfigManagedObject->getQuerierEnable());
            m->setMrouterTimeout(pIgmpsVlanConfigManagedObject->getMrouterTimeout());
            m->setVlanId(pVlanIntfManagedObject->getId());
			
			UI32 command = IGMPS_VLAN_LMQI | IGMPS_VLAN_QMRT | IGMPS_VLAN_QUERY_INTERVAL |
                IGMPS_VLAN_MROUTER_TIMEOUT | IGMPS_VLAN_SNOOP_ENABLE | IGMPS_VLAN_FAST_LEAVE | IGMPS_VLAN_QUERIER_ENABLE;
            m->setOpCode(command);

            if(! ( (pIgmpsVlanConfigManagedObject->getLmqi() == 1000) &&
                (pIgmpsVlanConfigManagedObject->getQmrt() == 10 ) &&
            (!pIgmpsVlanConfigManagedObject->getEnable() ) &&
                (!pIgmpsVlanConfigManagedObject->getFastLeave() ) &&
                (pIgmpsVlanConfigManagedObject->getQueryInterval() ==125) &&
                (!pIgmpsVlanConfigManagedObject->getQuerierEnable() ) &&
                (pIgmpsVlanConfigManagedObject->getMrouterTimeout() == 300) ))
            {
            status = sendSynchronouslyToWaveClient ("IGMP", m);
            if (status == WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_SUCCESS, string("NsmLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: sendToClient success"));
            } else {
                trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: sendToClient failed"));
            }
            }
            //delete pIgmpsVlanConfigManagedObject;
            delete m;
        }
        trace (TRACE_LEVEL_INFO, "NsmLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: Entered....");

    //    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
        return;
    }


	void NsmLocalObjectManager::ConfigReplayIgmpsMrtrVlanConfig(vector<WaveManagedObject *>* &pResultsPMO)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;

        trace (TRACE_LEVEL_INFO, "NsmLocalObjectManager::ConfigReplayIgmpsMrtrVlanConfig: Entered....");

        VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

//      WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
//      vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);
        if (NULL == pResultsPMO) {
            return;
        }

        UI32 numberOfResults = pResultsPMO->size ();
        if (0 == numberOfResults) {
            return;
        }
        trace(TRACE_LEVEL_DEVEL, string("NsmLocalObjectManager::ConfigReplayIgmpsMrtrVlanConfig: NumResults - ")+numberOfResults);
        for (UI32 j = 0; j < numberOfResults; j++) {
            pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResultsPMO)[j]);
            if (! pVlanIntfManagedObject)
                continue;

            vector<WaveManagedObjectPointer<IgmpsMrtrVlanConfigManagedObject> > igmpsMrtrVlanConfigMOVector =
                pVlanIntfManagedObject->getIgmpsMrtrVlanConfig();

            for (UI32 i = 0; i < igmpsMrtrVlanConfigMOVector.size(); i++) {
                if((IGMPS_IF_TYPE_FORTYGIGABIT == igmpsMrtrVlanConfigMOVector[i]->getIfType())
				   || (IGMPS_IF_TYPE_TENGIGABIT == igmpsMrtrVlanConfigMOVector[i]->getIfType()) 
				   || (IGMPS_IF_TYPE_GIGABIT == igmpsMrtrVlanConfigMOVector[i]->getIfType()))
                {
                    /* Do not do postboot for FortyGigabit/Tengigabit/Onegigabit ethernet. This will be done in NSM module. */
                    continue;
                }
                IgmpsClientUpdateMrtrVlanConfigMessage *m = new IgmpsClientUpdateMrtrVlanConfigMessage ();

                m->setVlanId(pVlanIntfManagedObject->getId());
                m->setIfType(igmpsMrtrVlanConfigMOVector[i]->getIfType());
                m->setIfName(igmpsMrtrVlanConfigMOVector[i]->getIfName());
                m->setNegation(false);

                if(true == NsmUtils::isValidThreeTuple(m->getIfName())) {
                    LocationId locationId = NsmUtils::getLocationId(m->getIfName());
                    if (locationId == NsmUtils::getLocalLocationId()) {
					string twoTuppleIfName;
                        NsmUtils::getTwoTupleIfName(m->getIfName(), twoTuppleIfName);
                        m->setIfName(twoTuppleIfName);
                    } else {
                        /* If this command does not belong to this node, discard.
                         * It will be replayed by corresponding node.
                        */
                        delete m;
                        continue;
                    }
                }

                trace (TRACE_LEVEL_INFO, string("NsmLocalObjectManager::ConfigReplayIgmpsMrtrVlanConfig: Replay Mr ")+ m->getIfType()+ m->getIfName());
                status = sendSynchronouslyToWaveClient ("IGMP", m);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    trace(TRACE_LEVEL_SUCCESS, string("NsmLocalObjectManager::ConfigReplayIgmpsMrtrVlanConfig: sendToClient success"));
                } else {
                    trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::ConfigReplayIgmpsMrtrVlanConfig: sendToClient failed"));
                }
                delete m;
            }
        }

//        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
        return;
    }
	void NsmLocalObjectManager::ConfigReplayIgmpsSgVlanConfig(vector<WaveManagedObject *>* &pResultsPMO)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;

        trace (TRACE_LEVEL_INFO, "NsmLocalObjectManager::ConfigReplayIgmpsSgVlanConfig: Entered....");

        VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

//      WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
//      vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);
        if (NULL == pResultsPMO) {
            return;
        }

        UI32 numberOfResults = pResultsPMO->size ();
        if (0 == numberOfResults) {
            return;
        }
        trace(TRACE_LEVEL_DEVEL, string("NsmLocalObjectManager::ConfigReplayIgmpsSgVlanConfig: NumResults - ")+numberOfResults);
        for (UI32 j = 0; j < numberOfResults; j++) {
            pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResultsPMO)[j]);
            if (! pVlanIntfManagedObject)
                continue;

            vector<WaveManagedObjectPointer<IgmpsSgVlanConfigManagedObject> > igmpsSgVlanConfigMOVector =
                pVlanIntfManagedObject->getIgmpsSgVlanConfig();

            for (UI32 i = 0; i < igmpsSgVlanConfigMOVector.size(); i++) {
                if((IGMPS_IF_TYPE_FORTYGIGABIT == igmpsSgVlanConfigMOVector[i]->getIfType()) 
				   || (IGMPS_IF_TYPE_TENGIGABIT == igmpsSgVlanConfigMOVector[i]->getIfType()) 
				   || (IGMPS_IF_TYPE_GIGABIT == igmpsSgVlanConfigMOVector[i]->getIfType()))
                {
                    /* Do not do postboot for FortyGigabit/Tengigabit/Onegigabit ethernet. This will be done in NSM module. */
                    continue;
                }

                IgmpsClientUpdateSgVlanConfigMessage *m = new IgmpsClientUpdateSgVlanConfigMessage ();

                m->setVlanId(pVlanIntfManagedObject->getId());
                m->setGroupAddress(igmpsSgVlanConfigMOVector[i]->getGroupAddress());
                m->setIfType(igmpsSgVlanConfigMOVector[i]->getIfType());
                m->setIfName(igmpsSgVlanConfigMOVector[i]->getIfName());
                m->setNegation(false);
				
				if(true == NsmUtils::isValidThreeTuple(m->getIfName())) {
                    LocationId locationId = NsmUtils::getLocationId(m->getIfName());
                    if (locationId == NsmUtils::getLocalLocationId()) {
                        string twoTuppleIfName;
                        NsmUtils::getTwoTupleIfName(m->getIfName(), twoTuppleIfName);
                        m->setIfName(twoTuppleIfName);
                        } else {
                        /* If this command does not belong to this node, discard.
                         * It will be replayed by corresponding node.
                         */
                        delete m;
                        continue;
                    }
                }

                trace (TRACE_LEVEL_INFO, string("NsmLocalObjectManager::ConfigReplayIgmpsSgVlanConfig Replay Sg")+m->getIfType()   + m->getIfName());
                status = sendSynchronouslyToWaveClient ("IGMP", m);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    trace(TRACE_LEVEL_SUCCESS, string("NsmLocalObjectManager::ConfigReplayIgmpsSgVlanConfig: sendToClient success"));
                } else {
                    trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::ConfigReplayIgmpsSgVlanConfig: sendToClient failed"));
                }
                delete m;
            }
        }

//        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
        return;
    }
#if 0
	ResourceId NsmLocalObjectManager::getPhyIntfByName(string & ifName, PhyIntfLocalManagedObject * &pIntfMo)
	{
		return (m_pIntfXstpLocalConfWorker->getPhyIntfByName(ifName, pIntfMo, NULL));
	}

#endif

	bool NsmLocalObjectManager::isPhyPortInISLMode(const string ifName, bool isMarkForDeletion ,PrismSynchronousLinearSequencerContext *seq_ctx_p)
	{
        bool ret = true;
		NSM_PLUG_DBG("Entering ...");
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute(new AttributeString(ifName, "id"));
		vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		if (pResults && (pResults -> size() == 1))
		{
			NSM_PLUG_DBG_S(string("Found Phy Mo for") + ifName);
			pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);
			ret =  pPhyIntfLocalManagedObject->getPortrole();
			if(isMarkForDeletion) 
			{	
				if(seq_ctx_p)
				{	
					//seq_ctx_p->addManagedObjectsForGarbageCollection(*pResults);
					pResults->clear();
					delete pResults;
					seq_ctx_p->addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);
				}
			}	

			else
			{	
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			}	

		}
		return ret;
	}	
   

//GVLAN TODO: Below method is used by fcoe plugin, to validate "fcoeport default"
//against port-profile-port config. Below must be changed to "port-profile-port" with 
//default profile applied on the interface or domain having default profile applied on the port.
bool NsmLocalObjectManager::isProfileSpecificFcoeApplied(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{
		bool ret = false;
		bool globalFlag = false;
		if (pPhyIntfLocalManagedObject) {
			globalFlag = APPMGlobalConfWorker::isFcoeEnabledInDefaultProfile();
			ret = (globalFlag && pPhyIntfLocalManagedObject->getIsPortProfiled());
			trace(TRACE_LEVEL_DEBUG, string("APPM NsmLocalObjectManager::isProfileSpecificFcoeApplied "
					" Global flag: ") + globalFlag + string("profiled flag: ") + pPhyIntfLocalManagedObject->getIsPortProfiled());
		}
		return (ret);
	}

//TODO : Below method is not used.
bool NsmLocalObjectManager::isProfileSpecificFcoeApplied(string ifName)
	{
		bool ret = false;
		bool globalFlag = false;
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"id"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (pResults && pResults->size() == 1) {
			pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);
			globalFlag = APPMGlobalConfWorker::isFcoeEnabledInDefaultProfile();
			ret = (globalFlag && pPhyIntfLocalManagedObject->getIsPortProfiled());
			trace(TRACE_LEVEL_DEBUG, string("APPM NsmLocalObjectManager::isProfileSpecificFcoeApplied "
					" Global flag: ") + globalFlag + string("profiled flag: ") + pPhyIntfLocalManagedObject->getIsPortProfiled());
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
		return (ret);
	}

	//TODO: Purpose is not clear, used during fcoe profile config.
    bool NsmLocalObjectManager::isFcoeConfiguredOnAnyProfiledPort()
	{
		bool ret = false;
		string fcoeMapName("default");
		vector<WaveManagedObject *> *pResults = NULL;
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		WaveManagedObject   *pWaveManagedObject = NULL;
		FcoeportAttrLocalManagedObject *pFcoeportAttrLocalManagedObject = NULL;

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeportAttrLocalManagedObject::getClassName());
		ObjectId phyInterfaceObjId = ObjectId::NullObjectId;
		ObjectId fcoeMapObjId = fcoeMapGetObjectIdByName(fcoeMapName);

		unsigned iter = 0;

		if (fcoeMapObjId == ObjectId::NullObjectId) {
			return (false);
		}
		//get the pResults from the API() i/p fcoemap name and o/p pResults vector of FcoeportAttrLocalManagedObject
		syncQueryCtxt.addAndAttribute (new AttributeObjectIdAssociation(&fcoeMapObjId, "map", 
				FcoeportAttrLocalManagedObject::getClassName()));
		pResults = querySynchronously (&syncQueryCtxt);

		if (pResults && pResults->size()) {
			for(iter = 0; iter < pResults->size(); iter++) {
				pFcoeportAttrLocalManagedObject = dynamic_cast<FcoeportAttrLocalManagedObject *>
					((*pResults)[iter]);
				prismAssert (NULL != pFcoeportAttrLocalManagedObject, __FILE__, __LINE__);
				phyInterfaceObjId = pFcoeportAttrLocalManagedObject->getOwnerManagedObjectId();
				pWaveManagedObject = queryManagedObject(phyInterfaceObjId);
				pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>(pWaveManagedObject);
				ret = pPhyIntfLocalManagedObject->getIsPortProfiled();
				if (ret) {
					trace(TRACE_LEVEL_ERROR, string("APPM NsmLocalObjectManager::isFcoeConfiguredOnAnyProfiledPort" 
							" FCoE configured explicitly on profiled port ")+ pPhyIntfLocalManagedObject->getId());
					break;
				}
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
		return (ret);
	}

	FcoeMapManagedObject* NsmLocalObjectManager::fcoeMapGetMoByName (string &map_name,
		FcoeLinearSequencerContext *ctx_p)
	{
		FcoeMapManagedObject *map_mo_p = NULL;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeMapManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&map_name, "map_name"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		UI32 numberOfResults = pResults->size ();
		if (numberOfResults == 0 || numberOfResults > 1) {
			trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
			if (ctx_p) {
				ctx_p->addMOVectorToReleaseVector(pResults);
			} else {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			}
			return NULL;
		}

		map_mo_p = dynamic_cast<FcoeMapManagedObject *> ((*pResults)[0]);

		pResults->clear ();
		delete pResults;
		return map_mo_p;
	}

	ObjectId NsmLocalObjectManager::fcoeMapGetObjectIdByName (string &map_name,
		FcoeLinearSequencerContext *ctx_p)
	{
		ObjectId                obj_id;
		FcoeMapManagedObject    *map_mo_p = fcoeMapGetMoByName(map_name);
		if (map_mo_p == NULL) {
			return ObjectId::NullObjectId;
		} else {
			obj_id = map_mo_p->getObjectId();
		}
		if (ctx_p) {
			ctx_p->addMOPointerToReleaseVector(map_mo_p);
		} else {
			delete(map_mo_p);
		}
		return obj_id;
	}

    void NsmLocalObjectManager::configReplayPhyOSPFConfig(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        NSM_PLUG_DBG("Entering NsmLocalObjectManager::configReplayPhyOSPFConfig ..");
        const NsmL3ReadyManagedObject *pL3ReadyMO = pPhyIntfLocalManagedObject->getL3ReadyMO();
        if (pL3ReadyMO) {
            NSM_PLUG_DBG("NsmLocalObjectManager::configReplayPhyOSPFConfig: L3ReadyMO..So get OSPFInterfaceLocalManagedObject");
            OSPFInterfaceLocalManagedObject *pOSPFIntfMO =
                (WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject>(
                        pL3ReadyMO->getOSPFIntfConfig())).operator->();
            if (pOSPFIntfMO != NULL) {
                NSM_PLUG_DBG("configReplayPhyOSPFConfig: OSPFInterfaceLocalManagedObject in pL3ReadyMO is not NULL ");
                OSPFInterfaceConfigMessage *msg = new OSPFInterfaceConfigMessage();
                /*
                 * Fill up the message
                 */
                pOSPFIntfMO->updateClientMessageForReplay(msg);
                sendSynchronouslyToWaveClient("ospf", msg);
                delete msg;
            }
        }
    }

    NsmServiceReadyEvent::NsmServiceReadyEvent ()
    : PrismEvent (NsmLocalObjectManager::getPrismServiceId (), NSM_SERVICE_READY)
    {
    }

    NsmServiceReadyEvent::~NsmServiceReadyEvent ()
    {
    }

    NsmChassisReadyEvent::NsmChassisReadyEvent ()
    : PrismEvent (NsmLocalObjectManager::getPrismServiceId (), NSM_CHASSIS_READY)
    {
    }

    NsmChassisReadyEvent::~NsmChassisReadyEvent()
    {
    }

    NsmLinecardReadyEvent::NsmLinecardReadyEvent()
    : PrismEvent (NsmLocalObjectManager::getPrismServiceId (), NSM_LINECARD_READY)
    {
        m_slotId = 0;
    }

    NsmLinecardReadyEvent::~NsmLinecardReadyEvent ()
    {
    }

    void NsmLinecardReadyEvent::setupAttributesForSerialization ()
    {
        PrismEvent::setupAttributesForSerialization ();
         addSerializableAttribute (new AttributeUI32   (&m_slotId, "slotId"));
    }

    UI32 NsmLinecardReadyEvent::getSlotId () const
    {
        return (m_slotId);
    }

    void NsmLinecardReadyEvent::setSlotId (const UI32 &slotId)
    {
       m_slotId = slotId;
    }

    NsmLinecardOnlineEvent::NsmLinecardOnlineEvent()
    : PrismEvent (NsmLocalObjectManager::getPrismServiceId (), NSM_LINECARD_ONLINE)
    {
        m_slotId = 0;
    }

    NsmLinecardOnlineEvent::~NsmLinecardOnlineEvent ()
    {
    }

    void NsmLinecardOnlineEvent::setupAttributesForSerialization ()
    {
        PrismEvent::setupAttributesForSerialization ();
         addSerializableAttribute (new AttributeUI32   (&m_slotId, "slotId"));
    }

    UI32 NsmLinecardOnlineEvent::getSlotId () const
    {
        return (m_slotId);
    }

    void NsmLinecardOnlineEvent::setSlotId (const UI32 &slotId)
    {
       m_slotId = slotId;
    }

    void NsmLocalObjectManager::multiPartitionCleanup (WaveAsynchronousContextForMultiPartitionCleanup *cleanupContext)
    {
        trace (TRACE_LEVEL_DEVEL, "NsmLocalObjectManager::multiPartitionCleanup: Cleaning up VRF interface association");
		cleanupInterfaceVRFBind(cleanupContext);
	}

	void NsmLocalObjectManager::cleanupInterfaceVRFBind(WaveAsynchronousContextForMultiPartitionCleanup *cleanupContext, bool fullCleanup)
	{
        vector<string> selectField;
        UI32 numberOfResults;
        NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
        int rc = FRAMEWORK_SUCCESS;
        LocationId location = FrameworkToolKit::getThisLocationId();

        ObjectId ownerPartitionManagedObjectId = cleanupContext->getOwnerPartitionManagedObjectId ();
        ObjectId vrfMoObjectId = VrfUtils::getVrfMoObjIdByPartitionMoObjId(ownerPartitionManagedObjectId);

        if (vrfMoObjectId == ObjectId::NullObjectId) {
            cleanupContext->setCompletionStatus(WAVE_MESSAGE_ERROR);
            cleanupContext->callback ();
            return;
        }

        trace (TRACE_LEVEL_DEVEL, "NsmLocalObjectManager::cleanupInterfaceVRFBind: Cleanup Phy Intf association");
        startTransaction();
        selectField.push_back("vrfid");
        selectField.push_back("ifName");
        selectField.push_back("layer3Ready");

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeObjectId(&vrfMoObjectId, "vrfid"));
        syncQueryCtxt.setLoadOneToManyRelationships(false);
        syncQueryCtxt.setLocationIdFilter(location);
        syncQueryCtxt.addSelectFields(selectField);
        vector<WaveManagedObject *> *pResultsPMO = querySynchronously(&syncQueryCtxt);
        if (NULL != pResultsPMO) {
            numberOfResults = pResultsPMO->size ();
            for (UI32 phyIndex = 0; phyIndex < numberOfResults; phyIndex++) {
                PhyIntfLocalManagedObject *phyIntfMO = dynamic_cast <PhyIntfLocalManagedObject*>((*pResultsPMO)[phyIndex]);
				if (fullCleanup) {
                	phyIntfMO->setVrfObjectId(ObjectId::NullObjectId);
				}	
                pL3ReadyMo = new NsmL3ReadyManagedObject(
                     dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                if (pL3ReadyMo != NULL) {
                    pL3ReadyMo->setInterfaceName(phyIntfMO->getIfName());
                    pL3ReadyMo->setIntfType(IF_TYPE_PHY);
                    phyIntfMO->setL3Ready(pL3ReadyMo);
                } else {
                  prismAssert(false, __FILE__, __LINE__);
                }
                phyIntfMO->addAttributeToBeUpdated("layer3Ready");
                phyIntfMO->addAttributeToBeUpdated("vrfid");
                updateWaveManagedObject(phyIntfMO);
            }
        }

        trace (TRACE_LEVEL_DEVEL, "NsmLocalObjectManager::cleanupInterfaceVRFBind: Cleanup Ve association");
        selectField.clear();
        selectField.push_back("vrfid");
        selectField.push_back("vlanId");
        selectField.push_back("layer3Ready");
        WaveManagedObjectSynchronousQueryContext veQueryCtxt(NsmIntfSviManagedObject::getClassName());
        veQueryCtxt.addAndAttribute (new AttributeObjectId(&vrfMoObjectId, "vrfid"));
        veQueryCtxt.setLoadOneToManyRelationships(false);
        veQueryCtxt.setLocationIdFilter(location);
        veQueryCtxt.addSelectFields(selectField);
        vector<WaveManagedObject *> *pResultsVeMO = querySynchronously(&veQueryCtxt);
        if (NULL != pResultsVeMO) {
            numberOfResults = pResultsVeMO->size ();
            for (UI32 veIndex = 0; veIndex < numberOfResults; veIndex++) {
                NsmIntfSviManagedObject *sviMO = dynamic_cast <NsmIntfSviManagedObject *>((*pResultsVeMO)[veIndex]);
				if (fullCleanup) {
                	sviMO->setVrfObjectId(ObjectId::NullObjectId);
				}
                pL3ReadyMo = new NsmL3ReadyManagedObject(
                     dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                if (pL3ReadyMo != NULL) {
                    pL3ReadyMo->setInterfaceName(getString(sviMO->getIfId()));
                    pL3ReadyMo->setIntfType(IF_TYPE_VL);
                    sviMO->setL3Ready(pL3ReadyMo);
                } else {
                    prismAssert(false, __FILE__, __LINE__);
                }
                sviMO->addAttributeToBeUpdated("layer3Ready");
                sviMO->addAttributeToBeUpdated("vrfid");
                updateWaveManagedObject(sviMO);
            }
        }

        trace (TRACE_LEVEL_DEVEL, "NsmLocalObjectManager::cleanupInterfaceVRFBind: Cleanup loopback association");
        selectField.clear();
        selectField.push_back("vrfid");
        selectField.push_back("layer3Ready");
        selectField.push_back("ifId");

        WaveManagedObjectSynchronousQueryContext loopBackQueryCtxt(NsmIntfLoopbackManagedObject::getClassName());
        loopBackQueryCtxt.addAndAttribute (new AttributeObjectId(&vrfMoObjectId, "vrfid"));
        loopBackQueryCtxt.setLoadOneToManyRelationships(false);
        loopBackQueryCtxt.setLocationIdFilter(location);
        loopBackQueryCtxt.addSelectFields(selectField);
        vector<WaveManagedObject *> *pResultsloMO = querySynchronously(&loopBackQueryCtxt);
        if (NULL != pResultsloMO) {
            numberOfResults = pResultsloMO->size ();
            for (UI32 lpIndex = 0; lpIndex < numberOfResults; lpIndex++) {
                NsmIntfLoopbackManagedObject *loopBackMO = dynamic_cast <NsmIntfLoopbackManagedObject*>((*pResultsloMO)[lpIndex]);

				if (fullCleanup) {
                	loopBackMO->setVrfObjectId(ObjectId::NullObjectId);
				}
                pL3ReadyMo = new NsmL3ReadyManagedObject(
                     dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                if (pL3ReadyMo != NULL) {
                    pL3ReadyMo->setInterfaceName(loopBackMO->getIfName());
                    pL3ReadyMo->setIntfType(IF_TYPE_INTF_LOOPBACK);
                    loopBackMO->setL3Ready(pL3ReadyMo);
                } else {
                    prismAssert(false, __FILE__, __LINE__);
                }
                loopBackMO->addAttributeToBeUpdated("vrfid");
                loopBackMO->addAttributeToBeUpdated("layer3Ready");
                updateWaveManagedObject(loopBackMO);
            }
        }

        rc = commitTransaction();
        if (rc != FRAMEWORK_SUCCESS) {
            trace (TRACE_LEVEL_ERROR, "NsmLocalObjectManager::cleanupInterfaceVRFBind: Cleanup Failed!!");
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsVeMO);
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsloMO);
        cleanupContext->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        cleanupContext->callback ();

    }
    void NsmLocalObjectManager::multiPartitionPartialCleanup (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup)
    {
        trace (TRACE_LEVEL_DEVEL, "NsmLocalObjectManager::multiPartitionPartialCleanup: Cleaning up VRF interface L3 config");
		cleanupInterfaceVRFBind(pWaveAsynchronousContextForMultiPartitionCleanup, false);
    }

    void NsmLocalObjectManager::configReplaySwitchportRspanVlan(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject,const string &ifName)
	{
    	NSM_PLUG_INFO_S(string(" Entering"));
        UI32 status = 0;
        if(pPhyIntfLocalManagedObject == NULL )
        {
        	NSM_PLUG_DBG_ERR_S(string("pPhyIntfLocalManagedObject = NULL, Cannot proceed"));
            return;
        }
        if(pPhyIntfLocalManagedObject->getSwModeFlag())
        {
            if(pPhyIntfLocalManagedObject->getSwMode() == SW_MODE_TRUNK)
            {
            	NSM_PLUG_INFO_S(string("Interface is is switchport trunk mode. Check if rspanvlan config replay is required for ")+ifName);

            	UI32Range rspanTrunkVlans = pPhyIntfLocalManagedObject->getRspanVlanRangeAdd();
            	if(rspanTrunkVlans != UI32Range("")) {
                    InterfaceType ifType = pPhyIntfLocalManagedObject->getPhyType();
                    NsmPhyIntfMessage *m = new NsmPhyIntfMessage();
                    m->setIfType(ifType);
                    m->setIfName(ifName);
					m->setVlanRange(rspanTrunkVlans);
					m->setCmdCode(NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN);
					NSM_PLUG_INFO_S(string(" Sending the Rspan Trunk Vlan Range ")+rspanTrunkVlans.toString()+" to the backend for replay");
	            	status = sendSynchronouslyToWaveClient ("nsm",m);
	            	NSM_PLUG_INFO_S(string("Rspan vlan config replay Status ")+status);
	            	delete m;
            	}
            }
        }
	}

	void NsmLocalObjectManager::configReplayAllowedMacs(PortSecurityLocalManagedObject *pPortSecMO)
	{
		vector<WaveManagedObjectPointer<PortSecAllowedMacsLocalManagedObject> > allowedMacs;
		PortSecAllowedMacsLocalManagedObject *pAllowedMac = NULL;
		UI32 size, i;

		allowedMacs = pPortSecMO->getAllowedMacs();
		size = allowedMacs.size();

		trace(TRACE_LEVEL_INFO, string("Size of allowed MACs vector is: ") + size);

		for (i = 0; i < size; i++) {
			PortSecurityClientMessage *m = new PortSecurityClientMessage();
			pAllowedMac = allowedMacs[i].operator->();
		
			if (pAllowedMac == NULL) {
				trace(TRACE_LEVEL_ERROR, "NsmLocalObjectManager::configReplayAllowedMacs : Allowed MAC object is NULL");
				delete(m);
				return;
			}

			trace(TRACE_LEVEL_INFO, string("Data from allowed MAC object is - MAC: ") + pAllowedMac->getAllowedMac());
			m->setOpCode(PORT_SECURITY_MAC);
			m->setAllowedMac(pAllowedMac->getAllowedMac());
			m->setVlanId(pAllowedMac->getVlanId());
			m->setIfName(pAllowedMac->getIfName());
       		sendSynchronouslyToWaveClient("l2sys", m);

			delete(m);
		}
	}

	void NsmLocalObjectManager::configReplayStickyMacs(PortSecurityLocalManagedObject *pPortSecMO)
	{
		vector<WaveManagedObjectPointer<PortSecStickyMacsLocalManagedObject> > stickyMacs;
		PortSecStickyMacsLocalManagedObject *pStickyMac = NULL;
		UI32 size, i;

		stickyMacs = pPortSecMO->getStickyMacs();
		size = stickyMacs.size();

		trace(TRACE_LEVEL_INFO, string("Size of sticky MACs vector is: ") + size);

		for (i = 0; i < size; i++) {
			PortSecurityClientMessage *m = new PortSecurityClientMessage();
			pStickyMac = stickyMacs[i].operator->();
		
			if (pStickyMac == NULL) {
				trace(TRACE_LEVEL_ERROR, "NsmLocalObjectManager::configReplayStickyMacs : Sticky MAC object is NULL");
				delete(m);
				return;
			}

			trace(TRACE_LEVEL_INFO, string("Data from sticky MAC object is - MAC: ") + pStickyMac->getStickyMac());
			m->setOpCode(PORT_SECURITY_STICKY_MAC);
			m->setAllowedMac(pStickyMac->getStickyMac());
			m->setVlanId(pStickyMac->getVlanId());
			m->setIfName(pStickyMac->getIfName());
       		sendSynchronouslyToWaveClient("l2sys", m);

			delete(m);
		}
	}

	void NsmLocalObjectManager::configReplayOuis(PortSecurityLocalManagedObject *pPortSecMO)
	{
		vector<WaveManagedObjectPointer<PortSecOuiLocalManagedObject> > ouis;
		PortSecOuiLocalManagedObject *pOui = NULL;
		UI32 size, i;

		ouis = pPortSecMO->getOuis();
		size = ouis.size();

		trace(TRACE_LEVEL_INFO, string("Size of OUIs vector is: ") + size);

		for (i = 0; i < size; i++) {
			PortSecurityClientMessage *m = new PortSecurityClientMessage();
			pOui = ouis[i].operator->();

			if (pOui == NULL) {
				trace(TRACE_LEVEL_ERROR, "NsmLocalObjectManager::configReplayOuis : OUI object is NULL");
				delete(m);
				return;
			}

			m->setOpCode(PORT_SECURITY_OUI);
			m->setOui(pOui->getOui());
			m->setIfName(pOui->getIfName());
        	sendSynchronouslyToWaveClient("l2sys", m);

			delete(m);
		}
	}

	void NsmLocalObjectManager::configReplayPortSecurity (PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{
		PortSecurityLocalManagedObject *pPortSecurityLocalManagedObject = NULL;
		WaveManagedObjectPointer<PortSecurityLocalManagedObject> portSecCfg;

		if (pPhyIntfLocalManagedObject == NULL) {
			trace(TRACE_LEVEL_INFO, "NsmLocalObjectManager::configReplayPortSecurity : interface object passed as NULL");
			return;
		}

		string ifName = pPhyIntfLocalManagedObject->getIfName();
        portSecCfg = pPhyIntfLocalManagedObject->getPortSecConfig();
        pPortSecurityLocalManagedObject = portSecCfg.operator->();

		//pPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);
		if (pPortSecurityLocalManagedObject == NULL) {
			trace(TRACE_LEVEL_INFO, string("NsmLocalObjectManager::configReplayPortSecurity : port-security configuration is not present for interface: ") + ifName);
			return;
		}

		trace(TRACE_LEVEL_INFO, string("Just before port-security MO post boot for interface: ") + ifName);

		PortSecurityClientMessage *m = new PortSecurityClientMessage();
		m->setOpCode(PORT_SECURITY_CONFIG_REPLAY);
		m->setIfName(ifName);
		m->setIfType(pPortSecurityLocalManagedObject->getIfType());
		m->setNumAllowedMacs(pPortSecurityLocalManagedObject->getNumAllowedMacs());
		m->setAction(pPortSecurityLocalManagedObject->getAction());
		m->setShutdownTime(pPortSecurityLocalManagedObject->getShutdownTime());
		m->setSticky(pPortSecurityLocalManagedObject->getSticky());
        sendSynchronouslyToWaveClient("l2sys", m);

		trace(TRACE_LEVEL_INFO, string("After port-security MO post boot for interface: ") + ifName);

		delete(m);

		/* Replaying the config of configured secure MACs */
		configReplayAllowedMacs(pPortSecurityLocalManagedObject);

		/* Replaying the config of configured OUIs */
		configReplayOuis(pPortSecurityLocalManagedObject);

		/* Replaying the config of sticky MACs */
		configReplayStickyMacs(pPortSecurityLocalManagedObject);
	}

	void NsmLocalObjectManager::configReplayPoPortSecurity (PoIntfManagedObject *pPoIntfMO)
	{
		PortSecurityLocalManagedObject *pPortSecurityLocalManagedObject = NULL;
		WaveManagedObjectPointer<PortSecurityLocalManagedObject> portSecCfg;

		if (pPoIntfMO == NULL) {
			trace(TRACE_LEVEL_INFO, "NsmLocalObjectManager::configReplayPoPortSecurity : interface object passed as NULL");
			return;
		}

		string ifName = pPoIntfMO->getIfName();
        portSecCfg = pPoIntfMO->getPortSecConfig();
        pPortSecurityLocalManagedObject = portSecCfg.operator->();

		//pPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);
		if (pPortSecurityLocalManagedObject == NULL) {
			trace(TRACE_LEVEL_INFO, string("NsmLocalObjectManager::configReplayPoPortSecurity : port-security configuration is not present for interface: ") + ifName);
			return;
		}

		trace(TRACE_LEVEL_INFO, string("Just before port-security MO post boot for interface: ") + ifName);

		PortSecurityClientMessage *m = new PortSecurityClientMessage();
		m->setOpCode(PORT_SECURITY_CONFIG_REPLAY);
		m->setIfName(ifName);
		m->setIfType(pPortSecurityLocalManagedObject->getIfType());
		m->setNumAllowedMacs(pPortSecurityLocalManagedObject->getNumAllowedMacs());
		m->setAction(pPortSecurityLocalManagedObject->getAction());
		m->setShutdownTime(pPortSecurityLocalManagedObject->getShutdownTime());
		m->setSticky(pPortSecurityLocalManagedObject->getSticky());
        sendSynchronouslyToWaveClient("l2sys", m);

		trace(TRACE_LEVEL_INFO, string("After port-security MO post boot for interface: ") + ifName);

		delete(m);

		/* Replaying the config of configured secure MACs */
		configReplayAllowedMacs(pPortSecurityLocalManagedObject);

		/* Replaying the config of configured OUIs */
		configReplayOuis(pPortSecurityLocalManagedObject);

		/* Replaying the config of sticky MACs */
		configReplayStickyMacs(pPortSecurityLocalManagedObject);
	}
	void NsmLocalObjectManager::configReplayPoVlanClassification(PoIntfManagedObject *pMO, const string &ifName)
	{
		if(pMO == NULL )
		{
			trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::configReplayVlanClassification: pMO is NULL."));

			return;
		}

		InterfaceType ifType = IF_TYPE_PO;
		SwitchportVlanClassificationMessage *m = new SwitchportVlanClassificationMessage();
		UI32 index = 0;

		if(pMO->getSwMode() == SW_MODE_ACCESS)
		{
			vector<WaveManagedObjectPointer<MacAddressVlanClassificationMO> > macAddressVector = pMO->getMacAddressVlanClassification();
			vector<WaveManagedObjectPointer<MacGroupVlanClassificationMO> > macGroupVector = pMO->getMacGroupVlanClassification();

			for (index = 0; index < macAddressVector.size(); index++)
			{
				m->setMacAddressRec(SW_VLAN_CLASSIFICATION_SET_MAC_ADDR, ifType, ifName, macAddressVector[index]->getAccessVlanId(), macAddressVector[index]->getMac());
			}

			for (index = 0; index < macGroupVector.size(); index++)
			{
				m->setMacGroupIdRec(SW_VLAN_CLASSIFICATION_SET_MAC_GROUP, ifType, ifName, macGroupVector[index]->getVlanId(), macGroupVector[index]->getMacGroupId());
			}
		}
	   	else if((pMO->getSwMode() == SW_MODE_TRUNK) || (pMO->getPvlanModeTrunkPromis()) || (pMO->getPvlanModeTrunkHost()))
		{
			vector<WaveManagedObjectPointer<CtagVlanClassificationMO> > ctagVlanVector = pMO->getCtagVlanClassification();

			for (index = 0; index < ctagVlanVector.size(); index++)
			{
				m->setCtagIdRec(SW_VLAN_CLASSIFICATION_SET_CTAG, ifType, ifName, ctagVlanVector[index]->getVlanId(), ctagVlanVector[index]->getCtagId());
			}
		}
	   	else if(pMO->getPvlanModeTrunkBas())
		{
			vector<WaveManagedObjectPointer<CtagPVlanClassificationMO> > ctagPVlanVector = pMO->getCtagPVlanClassification();

			for (index = 0; index < ctagPVlanVector.size(); index++)
			{
				m->setCtagIdRec(SW_VLAN_CLASSIFICATION_SET_PVLAN_CTAG, ifType, ifName, ctagPVlanVector[index]->getVlanId(), ctagPVlanVector[index]->getCtagId());
			}
		}

		if (m->getNumRec() > 0)
		{
			trace(TRACE_LEVEL_INFO, string("NsmLocalObjectManager::configReplayVlanClassification:numRecords in message is:") + m->getNumRec());

			sendSynchronouslyToWaveClient ("nsm", m);
		}

		delete m;
	}

	void NsmLocalObjectManager::configReplayPhyVlanClassification(PhyIntfLocalManagedObject *pMO, const string &ifName)
	{
		if(pMO == NULL )
		{
			trace(TRACE_LEVEL_ERROR, string("NsmLocalObjectManager::configReplayVlanClassification: pMO is NULL."));

			return;
		}

		InterfaceType ifType = pMO->getPhyType();
		SwitchportVlanClassificationMessage *m = new SwitchportVlanClassificationMessage();
		UI32 index = 0;

		if(pMO->getSwMode() == SW_MODE_ACCESS)
		{
			vector<WaveManagedObjectPointer<MacAddressVlanClassificationMO> > macAddressVector = pMO->getMacAddressVlanClassification();
			vector<WaveManagedObjectPointer<MacGroupVlanClassificationMO> > macGroupVector = pMO->getMacGroupVlanClassification();
			vector<WaveManagedObjectPointer<RspanMacAddressVlanClassificationMO> > macAddressRspanVector = pMO->getMacAddressRspanVlanClassification();
			vector<WaveManagedObjectPointer<RspanMacGroupVlanClassificationMO> > macGroupRspanVector = pMO->getMacGroupRspanVlanClassification();

			for (index = 0; index < macAddressVector.size(); index++)
			{
				m->setMacAddressRec(SW_VLAN_CLASSIFICATION_SET_MAC_ADDR, ifType, ifName, macAddressVector[index]->getAccessVlanId(), macAddressVector[index]->getMac());
			}

			for (index = 0; index < macGroupVector.size(); index++)
			{
				m->setMacGroupIdRec(SW_VLAN_CLASSIFICATION_SET_MAC_GROUP, ifType, ifName, macGroupVector[index]->getVlanId(), macGroupVector[index]->getMacGroupId());
			}

			for (index = 0; index < macAddressRspanVector.size(); index++)
			{
				m->setMacAddressRec(SW_VLAN_CLASSIFICATION_SET_RSPAN_MAC_ADDR, ifType, ifName, macAddressRspanVector[index]->getAccessVlanId(), macAddressRspanVector[index]->getMac());
			}

			for (index = 0; index < macGroupRspanVector.size(); index++)
			{
				m->setMacGroupIdRec(SW_VLAN_CLASSIFICATION_SET_RSPAN_MAC_GROUP, ifType, ifName, macGroupRspanVector[index]->getVlanId(), macGroupRspanVector[index]->getMacGroupId());
			}
		}
	   	else if(pMO->getSwMode() == SW_MODE_TRUNK)
		{
			vector<WaveManagedObjectPointer<CtagVlanClassificationMO> > ctagVlanVector = pMO->getCtagVlanClassification();
			vector<WaveManagedObjectPointer<RspanGVlanTrunkPortManagedObject> > ctagRspanVlanVector = pMO->getCtagRspanVlanClassification();

			for (index = 0; index < ctagVlanVector.size(); index++)
			{
				m->setCtagIdRec(SW_VLAN_CLASSIFICATION_SET_CTAG, ifType, ifName, ctagVlanVector[index]->getVlanId(), ctagVlanVector[index]->getCtagId());
			}

			for (index = 0; index < ctagRspanVlanVector.size(); index++)
			{
				m->setCtagIdRec(SW_VLAN_CLASSIFICATION_SET_RSPAN_CTAG, ifType, ifName, ctagRspanVlanVector[index]->getVlanId(), ctagRspanVlanVector[index]->getCtagId());
			}
		}
	   	else if((pMO->getPvlanModeTrunkPromis()) || (pMO->getPvlanModeTrunkHost()))
		{
			vector<WaveManagedObjectPointer<CtagVlanClassificationMO> > ctagVlanVector = pMO->getCtagVlanClassification();

			for (index = 0; index < ctagVlanVector.size(); index++)
			{
				m->setCtagIdRec(SW_VLAN_CLASSIFICATION_SET_CTAG, ifType, ifName, ctagVlanVector[index]->getVlanId(), ctagVlanVector[index]->getCtagId());
			}
		}
	   	else if(pMO->getPvlanModeTrunkBas())
		{
			vector<WaveManagedObjectPointer<CtagPVlanClassificationMO> > ctagPVlanVector = pMO->getCtagPVlanClassification();

			for (index = 0; index < ctagPVlanVector.size(); index++)
			{
				m->setCtagIdRec(SW_VLAN_CLASSIFICATION_SET_PVLAN_CTAG, ifType, ifName, ctagPVlanVector[index]->getVlanId(), ctagPVlanVector[index]->getCtagId());
			}
		}

		if (m->getNumRec() > 0)
		{
			trace(TRACE_LEVEL_INFO, string("NsmLocalObjectManager::configReplayVlanClassification:numRecords in message is:") + m->getNumRec());

			sendSynchronouslyToWaveClient ("nsm", m);
		}

		delete m;
	}

	void NsmLocalObjectManager::configReplayMacGroup()
	{
        WaveManagedObjectSynchronousQueryContext queryCtxt(MacGroupManagedObject::getClassName());
        vector<WaveManagedObject *> *pMacGroupResults = querySynchronously(&queryCtxt);

        if((pMacGroupResults != NULL) && (pMacGroupResults->size() > 0))
        {
            for(UI32 macGroupIndex = 0; macGroupIndex < pMacGroupResults->size(); macGroupIndex++)
            {
                MacGroupManagedObject *pMacGroupMO = dynamic_cast<MacGroupManagedObject *>((*pMacGroupResults)[macGroupIndex]);
				MacGroupConfigMessage *pClientMsg = new MacGroupConfigMessage();

				pClientMsg->setCmdCode(MAC_GROUP_CREATE);
				pClientMsg->setMacGroupId(pMacGroupMO->getMacGroupId());
				vector<WaveManagedObjectPointer<MacGroupEntryManagedObject> > macGroupEntryVector = pMacGroupMO->getMacGroupEntry();

				if(macGroupEntryVector.size() > 0)
				{
					for(UI32 macGroupEntryIndex = 0; macGroupEntryIndex < macGroupEntryVector.size(); macGroupEntryIndex++)
					{
						pClientMsg->setMacEntry(macGroupEntryVector[macGroupEntryIndex]->getAddress(), macGroupEntryVector[macGroupEntryIndex]->getMask());
					}
				}

				sendSynchronouslyToWaveClient("nsm", pClientMsg);
				delete pClientMsg;
			}

			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pMacGroupResults);
		}

		return;
	}

}//namespace ends here                                                                                                                                                                                                                                                                                            
