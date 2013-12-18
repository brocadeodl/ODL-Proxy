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
#include <map>
#include <algorithm>
#include "DcmResourceIdEnums.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForUpdate.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Postboot/PrismPostbootMessages.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "DcmCore/DcmToolKit.h"
//Fcoe
#include "ClientInterface/Fcoe/FcoeClientClearMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/IntfConfLocalWorker.h"
#include "Nsm/Local/NsmCreatePhyIntfMessage.h"
#include "Nsm/Local/NsmUpdatePhyIntfMessage.h"
#include "Nsm/Local/NsmUpdateBulkPhyIntfMessage.h"
#include "Nsm/Local/NsmDeletePhyIntfMessage.h"
#include "Nsm/Local/NsmAddPhyIntfToVlanIntfMessage.h"
#include "Nsm/Local/NsmDeletePhyIntfToVlanIntfMessage.h"
#include "Nsm/Local/NsmCreateVlanIntfSPMessage.h"
#include "Nsm/Local/NsmUpdateVlanIntfSPMessage.h"
#include "Nsm/Local/NsmDeleteVlanIntfSPMessage.h"
#include "Nsm/Local/NsmCreatePoIntfSPMessage.h"
#include "Nsm/Local/NsmUpdatePoIntfSPMessage.h"
#include "Nsm/Local/NsmDeletePoIntfSPMessage.h"
#include "Nsm/Local/NsmAddPoToVlanIntfSPMessage.h"
#include "Nsm/Local/NsmDeletePoToVlanIntfSPMessage.h"
#include "Nsm/Local/NsmAddPhyIntfToPoIntfMessage.h"
#include "Nsm/Local/NsmDeletePhyIntfToPoIntfMessage.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/TenGigLocalManagedObject.h"
#include "Nsm/Local/OneGigLocalManagedObject.h"
#include "Nsm/Local/FortyGigLocalManagedObject.h"
#include "Nsm/Local/NsmAddPhyIntfToPoIntfMessage.h"
#include "Nsm/Local/XstpPortConfigManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/IntfXstpLocalConfWorker.h"
#include "Nsm/Local/NsmEventMessage.h"
#include "Nsm/Local/NsmLocalShowIntfMessage.h"
#include "Nsm/Local/NsmLocalPortMediaShowIntfMessage.h"
#include "Nsm/Local/NsmLocalDebugMessage.h"
#include "Nsm/Local/NsmUpdatePoPrimaryLinkMessage.h"
#include "Nsm/Local/NsmLocalClearMessage.h"
#include "Nsm/Local/NsmLocalMacRebalanceMessage.h"
#include "Nsm/Local/NsmLocalPolicyMapMessage.h"
#include "Nsm/Local/PortSecurityLocalManagedObject.h"
#include "Nsm/Local/VLAN/GvlanConfigUtils.h"
#include "Policer/Global/PolicyClassMapManagedObject.h"
#include "Policer/Local/PolicerLocalObjectManager.h"
#include "Nsm/Local/PolicerLocalShowMessage.h"
#include "ClientInterface/Policer/PolicerShowMessage.h"
#include "Nsm/Local/PolicerLocalClearMessage.h"
#include "ClientInterface/Policer/PolicerClearMessage.h"
#include "ClientInterface/Policer/PolicerClientTeBulkMessage.h"
/* Span Mirrored port message */
#include "Nsm/Local/Span/NsmLocaldestinationMirrorportMessage.h"
/* StormControl */
#include "ClientInterface/StormControl/StormControlClientClearMessage.h"
#include "ssm_policer_dcmd_public.h"
#include "ClientInterface/Qos/qos_bum_dcm_common.h"

#include "Nsm/Global/NsmGlobalCreateVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalDeletePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalCreatePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdateVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdatePoIntfMessage.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"

#include "Nsm/Common/PVlanUtils.h"
#include "Framework/ObjectModel/WaveObjectManager.h"

#include "ClientInterface/InterfaceCommon.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmVlanIntfMessage.h"
#include "ClientInterface/Nsm/NsmPoIntfMessage.h"
#include "ClientInterface/Nsm/NsmPhyIntfMessage.h"
#include "ClientInterface/Nsm/NsmFcoeVlanMessage.h"
#include "ClientInterface/Nsm/NSMClientReservedVlanMessage.h"
#include "ClientInterface/Nsm/NsmShowIntfMessage.h"
#include "ClientInterface/Nsm/NsmPortMediaShowMessage.h"
#include "ClientInterface/Nsm/NsmDebugMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/Nsm/NsmClientUpdateGlobalConfigMessage.h"

#include "Nsm/Local/NsmAssocMacAclPolicyPoVlanIntfSPMessage.h"
#include "Nsm/Local/NsmAssocMacAclPolicyPhyIntfMessage.h"
#include "Ssm/Global/SsmGlobalGetMacAclPolicyMessage.h"
#include "Ssm/Global/MacAclPolicyManagedObject.h"
#include "ClientInterface/Ssm/SsmMacAclPolicyMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "ClientInterface/Ssm/AclClearStatsMessage.h"

/* PBR */
#include "Nsm/Local/Ssm/PBRVeIntfMessage.h"
#include "Nsm/Local/Ssm/PBRPhyIntfMessage.h"
#include "Nsm/Global/Ssm/PBRPoVlanIntfMessage.h"
#include "Nsm/Local/Ssm/PBRPoVlanIntfSPMessage.h"
#include "Nsm/Local/Ssm/NsmLocalPBRManagedObject.h"
#include "IpPolicy/Framework/IpPolicyGetPBRObjectMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "IpPolicy/Framework/IpPolicyLocalManagedObjects.h"
#include "IpPolicy/Framework/IpPolicyDeletePBRMessage.h"

/* Ip Acl Heaader files */
#include "Nsm/Local/Ssm/NsmAssocIpAclPolicyPoVlanIntfSPMessage.h"
#include "Nsm/Local/Ssm/NsmAssocIpAclPolicyPhyIntfMessage.h"
#include "Nsm/Local/Ssm/NsmLocalAssocIpAclPolicyVeIntfMessage.h"
#include "Ssm/Global/IpAclPolicyManagedObject.h"
#include "Ssm/Global/SsmGlobalGetIpAclPolicyMessage.h"
#include "ClientInterface/Ssm/SsmIpAclPolicyMessage.h"
#include "ssmAcl_dcmd_public.h"

#include "DcmCore/DcmToolKit.h"
/* Lacp header files */
#include "Nsm/Local/NsmUpdateLacpConfigSPMessage.h"
#include "ClientInterface/Nsm/NsmClientUpdateLacpConfigMessage.h"
#include "ClientInterface/Nsm/NsmDefaultConfigurationCmdsMessage.h"
#include "Nsm/Local/LacpLocalShowMessage.h"
#include "ClientInterface/Nsm/LacpShowMessage.h"

#include "Nsm/Local/Dot1XLocalShowMessage.h"
#include "ClientInterface/Dot1X/Dot1XShowMessage.h"
#include "Nsm/Local/Dot1XLocalReauthenticateMessage.h"
#include "ClientInterface/Dot1X/Dot1XReauthenticateMessage.h"
#include "Nsm/Local/Dot1XLocalTestMessage.h"
#include "ClientInterface/Dot1X/Dot1XTestMessage.h"
#include "Nsm/Local/Dot1XLocalClearMessage.h"
#include "ClientInterface/Dot1X/Dot1XClearMessage.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"
#include "ClientInterface/Nsm/NsmClearMessage.h"
#include "Nsm/Local/LacpLocalClearMessage.h"
#include "ClientInterface/Nsm/LacpClearMessage.h"

#include "Nsm/Local/NSMLocalReserevedVlanMessage.h"
/* ELD */
#include "ClientInterface/Eld/EldClientUpdatePhyIntfMessage.h"
#include "ClientInterface/Eld/EldPoIntfMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"

/* Fcoe */
#include "Nsm/Local/NsmLocalFcoeVlanMessage.h"
#include "Fcoe/Global/FcoeMapManagedObject.h"
#include "ClientInterface/Fcoe/FcoeClientEthConfigMessage.h"

/* Lldp */
#include "Nsm/Local/LldpIntfConfLocalManagedObject.h"
#include "Lldp/Global/LldpGlobalProfileGetObjectIdMessage.h"
#include "ClientInterface/Lldp/NsmLldpClientIntfConfSetMessage.h"

/* Storm Control */
#include "Nsm/Local/Ssm/NsmStormControlIntfConfLocalManagedObject.h"
#include "Nsm/Local/Ssm/NsmStormControlIntfSetMessage.h"
#include "ClientInterface/StormControl/NsmStormControlClientIntfConfSetMsg.h"
#include "Nsm/Local/Dot1xPortConfigLocalManagedObject.h"

/* Qos */
#include "Nsm/Local/QosCosToTailDropManagedObject.h"
#include "Nsm/Local/NsmLocalConfigQosAttributesMessage.h"
#include "Nsm/Local/NsmQosPOConfigSPMessage.h"
#include "ClientInterface/Ssm/QosClientConfigIntfNsmAttributesMessage.h"
#include <sstream>
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "Qos/Global/CeeMapManagedObject.h"
#include "Qos/Global/DscpToDscpMutationMapManagedObject.h"
#include "Qos/Global/DscpToTrafficClassMapManagedObject.h"
#include "Qos/Global/DscpToCosMapManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/PriorityMapManagedObject.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "ClientInterface/Ssm/qoscommon.h"

#include "ClientInterface/Ssm/QosClientClearMessage.h"
#include "Qos/Common/QosUtils.h"
/* Sflow*/
#include "Sflow/Global/SflowGlobalConfigManagedObject.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"

/* Appm */
#include "APPM/Local/APPMLocalConfWorker.h"
#include "APPM/Global/FcoeProfileCfgManagedObject.h"
#include "APPM/Global/VlanProfileCfgManagedObject.h"
#include "APPM/Global/AMPPProfileCompositeManagedObject.h"
#include "APPM/Global/AMPPProfileDomainCompositeManagedObject.h"
#include "APPM/Utils/APPMUtils.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "DcmCStatus.h"
#include "Cluster/Local/WaveNode.h"
#include "Utils/DceClusterUtils.h"

/*Ecmp load balancing */
#include "Nsm/Local/NsmLocalEcmpConfigMessage.h"
#include "ClientInterface/Nsm/NsmEcmpConfigMessage.h"
#include "Nsm/Local/EcmpConfigManagedObject.h"
#include "Nsm/Local/NsmLocalShowEcmpMessage.h"
#include "ClientInterface/Nsm/NsmShowEcmpMessage.h"

/* no debug all*/
#include "WyserEaGateway/GenericClientMessage.h"
#include "ClientInterface/Nsm/LacpDebugMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/Lldp/LldpClientDebugMessage.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"
#include "ClientInterface/Igmps/IgmpsClientDebugMessage.h"
#include "ClientInterface/Mstp/MstpShowMessage.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"
#include "ClientInterface/Dot1X/Dot1XDebugCommand.h"
#include "ClientInterface/Nsm/MacRebalanceMessage.h"
#include "ClientInterface/OSPF/OSPFDebugMessage.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"
#include "ClientInterface/VRRP/VRRPDebugMessage.h"

#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "ClientInterface/Ssm/SsmMacAclIntfAssocBulkMessage.h"
#include "VRRP/Local/VRRPLocalObjectManager.h"
#include "Igmp/Local/IgmpLocalObjectManager.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalObjectManager.h"
#include "ManagementInterface/ClientInterface/UnifiedClientNotifyMessage.h"

/* ELD */
#include "ClientInterface/Eld/EldClientUpdatePhyIntfMessage.h"
#include "ClientInterface/Eld/EldPoIntfMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"

/*Vlag LoadBalancing */
#include "Nsm/Local/NsmLocalVlagConfigMessage.h"
#include "Nsm/Local/NsmVlagConfigManagedObject.h"
#include "Nsm/Local/NsmLocalShowVlagMessage.h"
#include "ClientInterface/Nsm/NsmVlagConfigMessage.h"
#include "ClientInterface/Nsm/NsmShowVlagMessage.h"

#include "Utils/DceSynchronousLinearSeqContext.h"
#include "Utils/Capabilities.h"

/* Postboot */
#include "Nsm/Local/Layer3/L3PostBootUtil.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Shell/ShellDebug.h"
#include "WaveResourceIds.h"

#include "Framework/Utils/StringUtils.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"
#include "ClientInterface/Udld/udld_dcm_def.h"
//QoS Policy Map
#include "ClientInterface/Policer/PolicerClientTeMessage.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"
#include "Nsm/Local/ConnectorConfigLocalManagedObject.h"
#include "Nsm/Local/ConnectorConfigLocalMessage.h"

#include "VcsFabric/Local/VcsSlotLocalManagedObject.h"
#include "utils/mqueue.h"
#include "Span/Global/SpanGlobalSpanGlobalConfigMessage.h"
#include "Span/Global/SpanGlobalConfigManagedObject.h"
#include "L2sys/Global/L2sysGlobalStaticMacManagedObject.h"
#include "L2sys/Global/L2sysGlobalUpdateL2sysGlobalStaticMacMessage.h"
#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "Nsm/Local/MacBasedVlanClassificationMO.h"
#include "Nsm/Local/SwitchportVlanConfigLocalMessage.h"
#include "Nsm/Local/CtagBasedVlanClassificationMO.h"
#include "Nsm/Local/RspanGVlanAccessPortManagedObject.h"
#include "Nsm/Local/RspanGVlanTrunkPortManagedObject.h"
#include "Fcoe/Local/FcoeETIntfLocalManagedObject.h"
#include "Fcoe/Local/FcoeLocalETIntfMessage.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "Utils/ProvisioningUtil.h"

//PIM
#include "Pim/Local/PimLocalObjectManager.h"

#define INTF_XSTP_LOCAL_WORKER (NsmLocalObjectManager::getInstance()->getIntfXstpLocalConfWorker())
#define PORT_ROLE "portrole"

#include "Nsm/Local/MacGroupConfigLocalMessage.h"
#include "Nsm/Local/NsmLocalProvisioningInfoMessage.h"
#include "ClientInterface/Nsm/MacGroupConfigMessage.h"
#include "Nsm/Global/MacGroupManagedObject.h"
#include "Nsm/Local/VLAN/GVlanEnableLocalMessage.h"
#include "ClientInterface/Nsm/GVlanEnableMessage.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"

#define LLDP_RECORDS_PER_PAGE 25

namespace DcmNs
{

    // Hold slotId in vector to reply Interface state in Boot handler when plugin was down
    static vector<UI32> m_intfStateReplay_slotIds;
    static PrismMutex m_intfStateReplay_Mutex;
    IntfConfLocalWorker::IntfConfLocalWorker ( NsmLocalObjectManager *pWaveObjectManager)
        : WaveWorker  (pWaveObjectManager)
    {
        PhyIntfLocalManagedObject    PhyIntfLocalManagedObject    (pWaveObjectManager);
        PhyIntfLocalManagedObject.setupOrm ();
        addManagedClass (PhyIntfLocalManagedObject::getClassName (), this);
        TenGigLocalManagedObject    TenGigLocalManagedObject    (pWaveObjectManager);
        TenGigLocalManagedObject.setupOrm ();
        addManagedClass (TenGigLocalManagedObject::getClassName (), this);
        OneGigLocalManagedObject    OneGigLocalManagedObject    (pWaveObjectManager);
        OneGigLocalManagedObject.setupOrm ();
        addManagedClass (OneGigLocalManagedObject::getClassName (), this);
        NativeVlanIntfConfLocalManagedObject    NativeVlanIntfConfLocalManagedObject    (pWaveObjectManager);
        FortyGigLocalManagedObject    FortyGigLocalManagedObject    (pWaveObjectManager);
        FortyGigLocalManagedObject.setupOrm ();
        addManagedClass (FortyGigLocalManagedObject::getClassName (), this);
        NativeVlanIntfConfLocalManagedObject.setupOrm ();
        addManagedClass (NativeVlanIntfConfLocalManagedObject::getClassName (), this);
		EcmpConfigManagedObject  EcmpConfigManagedObject(pWaveObjectManager);
		EcmpConfigManagedObject.setupOrm();
		addManagedClass (EcmpConfigManagedObject::getClassName(),this);

        	/*Mac Acl */
        NsmLocalIngressEgressMacAclPolicyManagedObject NsmLocalIngressEgressMacAclPolicyManagedObject (pWaveObjectManager);
		NsmLocalIngressEgressMacAclPolicyManagedObject.setupOrm ();
		addManagedClass (NsmLocalIngressEgressMacAclPolicyManagedObject::getClassName (), this);

		/* Vlag loadbalancing */
		NsmVlagConfigManagedObject    NsmVlagConfigManagedObject    (pWaveObjectManager);
		NsmVlagConfigManagedObject.setupOrm ();
		addManagedClass (NsmVlagConfigManagedObject::getClassName (), this);


		/*Storm Control*/
		NsmStormControlIntfConfLocalManagedObject    NsmStormControlIntfConfLocalManagedObject    (pWaveObjectManager);
		NsmStormControlIntfConfLocalManagedObject.setupOrm ();
		addManagedClass (NsmStormControlIntfConfLocalManagedObject::getClassName (), this);
		
		/* Ip Acl */
        NsmLocalIngressEgressIpAclPolicyManagedObject NsmLocalIngressEgressIpAclPolicyManagedObject (pWaveObjectManager);
        NsmLocalIngressEgressIpAclPolicyManagedObject.setupOrm ();
        addManagedClass (NsmLocalIngressEgressIpAclPolicyManagedObject::getClassName (), this);

        NsmLocalPBRManagedObject NsmLocalPBRManagedObject (pWaveObjectManager);
        NsmLocalPBRManagedObject.setupOrm ();
        addManagedClass (NsmLocalPBRManagedObject::getClassName (), this);

        ConnectorConfigLocalManagedObject ConnectorConfigLocalManagedObject (pWaveObjectManager);
        ConnectorConfigLocalManagedObject.setupOrm ();
        addManagedClass (ConnectorConfigLocalManagedObject::getClassName (), this);

        MacAddressVlanClassificationMO MacAddressVlanClassificationMO (pWaveObjectManager);
        MacAddressVlanClassificationMO.setupOrm ();
        addManagedClass (MacAddressVlanClassificationMO::getClassName (), this);

        VlanClassificationMO VlanClassificationMO (pWaveObjectManager);
        VlanClassificationMO.setupOrm ();
        addManagedClass (VlanClassificationMO::getClassName (), this);

        CtagVlanClassificationMO CtagVlanClassificationMO (pWaveObjectManager);
        CtagVlanClassificationMO.setupOrm ();
        addManagedClass (CtagVlanClassificationMO::getClassName (), this);

        CtagVlanClassificationRemoveMO CtagVlanClassificationRemoveMO (pWaveObjectManager);
        CtagVlanClassificationRemoveMO.setupOrm ();
        addManagedClass (CtagVlanClassificationRemoveMO::getClassName (), this);

        CtagPVlanClassificationMO CtagPVlanClassificationMO (pWaveObjectManager);
        CtagPVlanClassificationMO.setupOrm ();
        addManagedClass (CtagPVlanClassificationMO::getClassName (), this);

        CtagPVlanClassificationRemoveMO CtagPVlanClassificationRemoveMO (pWaveObjectManager);
        CtagPVlanClassificationRemoveMO.setupOrm ();
        addManagedClass (CtagPVlanClassificationRemoveMO::getClassName (), this);

        MacGroupVlanClassificationMO MacGroupVlanClassificationMO (pWaveObjectManager);
        MacGroupVlanClassificationMO.setupOrm ();
        addManagedClass (MacGroupVlanClassificationMO::getClassName (), this);

        RspanMacAddressVlanClassificationMO RspanMacAddressVlanClassificationMO (pWaveObjectManager);
        RspanMacAddressVlanClassificationMO.setupOrm ();
        addManagedClass (RspanMacAddressVlanClassificationMO::getClassName (), this);

        RspanMacGroupVlanClassificationMO RspanMacGroupVlanClassificationMO (pWaveObjectManager);
        RspanMacGroupVlanClassificationMO.setupOrm ();
        addManagedClass (RspanMacGroupVlanClassificationMO::getClassName (), this);

        RspanGVlanTrunkPortManagedObject RspanGVlanTrunkPortManagedObject (pWaveObjectManager);
        RspanGVlanTrunkPortManagedObject.setupOrm ();
        addManagedClass (RspanGVlanTrunkPortManagedObject::getClassName (), this);

        RspanGVlanTrunkPortRemoveManagedObject RspanGVlanTrunkPortRemoveManagedObject (pWaveObjectManager);
        RspanGVlanTrunkPortRemoveManagedObject.setupOrm ();
        addManagedClass (RspanGVlanTrunkPortRemoveManagedObject::getClassName (), this);

        AMPPProfileCompositeManagedObject AMPPProfileCompositeManagedObject(pWaveObjectManager);
        AMPPProfileCompositeManagedObject.setupOrm();
        addManagedClass(AMPPProfileCompositeManagedObject::getClassName(),this);

        AMPPProfileDomainCompositeManagedObject AMPPProfileDomainCompositeManagedObject(pWaveObjectManager);
        AMPPProfileDomainCompositeManagedObject.setupOrm();
        addManagedClass(AMPPProfileDomainCompositeManagedObject::getClassName(),this);


        m_isGlobalPortChannelReplayDone = false;
        m_lastSlotIdMadeOffline = 0;

        addOperationMap (NSMCREATEPHYINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmCreatePhyIntfMessageHandler));
        addOperationMap (NSMUPDATEPHYINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmUpdatePhyIntfMessageHandler));
        addOperationMap (NSMUPDATEBULKPHYINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmUpdateBulkPhyIntfMessageHandler));
        addOperationMap (NSMDELETEPHYINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmDeletePhyIntfMessageHandler));
        addOperationMap (NSMADDPHYINTFTOVLANINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmAddPhyIntfToVlanIntfMessageHandler));
        addOperationMap (NSMDELETEPHYINTFTOVLANINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmDeletePhyIntfToVlanIntfMessageHandler));
        addOperationMap (NSMCREATEVLANINTFSP, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmCreateVlanIntfSPMessageHandler));
        addOperationMap (NSMUPDATEVLANINTFSP, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmUpdateVlanIntfSPMessageHandler));
        addOperationMap (NSMDELETEVLANINTFSP, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmDeleteVlanIntfSPMessageHandler));
        addOperationMap (NSMCREATEPOINTFSP, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmCreatePoIntfSPMessageHandler));
        addOperationMap (NSMUPDATEPOINTFSP, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmUpdatePoIntfSPMessageHandler));
        addOperationMap (NSMDELETEPOINTFSP, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmDeletePoIntfSPMessageHandler));
        addOperationMap (NSMADDPOTOVLANINTFSP, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmAddPoToVlanIntfSPMessageHandler));
        addOperationMap (NSMDELETEPOTOVLANINTFSP, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmDeletePoToVlanIntfSPMessageHandler));
        addOperationMap (NSMADDPHYINTFTOPOINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmAddPhyIntfToPoIntfMessageHandler));
        addOperationMap (NSMDELETEPHYINTFTOPOINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmDeletePhyIntfToPoIntfMessageHandler));
        addOperationMap (NSMUPDATELACPCONFIGSP, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmUpdateLacpConfigSPMessageHandler));
        addOperationMap (NSMASSOCMACACLPOLICYPOVLANINTFSP, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmAssocMacAclPolicyPoVlanIntfSPMessageHandler));
        addOperationMap (NSMASSOCMACACLPOLICYPHYINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmAssocMacAclPolicyPhyIntfMessageHandler));
		/* SPAN Destination Mirrored Port Information */
		addOperationMap (NSMLOCALDESTINATIONMIRRORPORT, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocaldestinationMirrorportMessageHandler));
        addOperationMap (NSMASSOCIPACLPOLICYPOVLANINTFSP, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmAssocIpAclPolicyPoVlanIntfSPMessageHandler));
		addOperationMap (NSMLOCALPBRPOINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmPBRPoVlanIntfSPMessageHandler));
		addOperationMap (IPPOLICYDELETEPBR, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmPBRDeleteMessageHandler));
        addOperationMap (NSMASSOCIPACLPOLICYPHYINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmAssocIpAclPolicyPhyIntfMessageHandler));
        addOperationMap (NSMLOCALPBRPHYINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmPBRPhyIntfMessageHandler));
        addOperationMap (NSMLOCALPBRVEINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalPBRVeIntfMessageHandler));
        addOperationMap (NSMLOCALASSOCIPACLPOLICYVEINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalAssocIpAclPolicyVeIntfMessageHandler));
		addOperationMap (NSMSTORMCONTROLINTFSET,reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmStormControlIntfSetMessageHandler));
        addOperationMap (NSMLOCALFCOEVLAN, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalFcoeVlanMessageHandler));
        addOperationMap (NSMLOCALRESERVEDVLAN, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalReservedVlanMessageHandler)); 
        addOperationMap (NSMLOCALCONFIGQOSATTRIBUTES, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalConfigQosAttributesMessageHandler));
        addOperationMap (NSMQOSPOCONFIGSP, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmQosPOConfigSPMessageHandler));
        addOperationMap (NSMEVENT, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmEventMessageHandler));
        addOperationMap (LACPLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::LacpLocalShowMessageHandler));
        addOperationMap (DOT1X_LOCAL_SHOW_ALL, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::Dot1XLocalShowMessageHandler));
        addOperationMap (DOT1X_LOCAL_REAUTHENTICATE_ALL, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::Dot1XLocalReauthenticateMessageHandler));
        addOperationMap (DOT1X_LOCAL_TEST_ALL, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::Dot1XLocalTestMessageHandler));
		addOperationMap (DOT1X_LOCAL_CLEAR_ALL, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::Dot1XLocalClearMessageHandler));
		addOperationMap (LACPLOCALCLEAR, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::LacpLocalClearMessageHandler));
		addOperationMap (NSMLOCALSHOWINTERFACE, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalShowMessageHandler));
        addOperationMap (NSMLOCALPORTMEDIASHOWINTERFACE, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalPortMediaShowMessageHandler));
        addOperationMap (NSMLOCALDEBUGCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalDebugMessageHandler));
        addOperationMap (NSMUPDATEPOPRIMARYLINK, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmUpdatePoPrimaryLinkMessageHandler));
        addOperationMap (NSM_LOCAL_CLEAR_MSG, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalClearMessageHandler));
        addOperationMap (NSMLOCALSHOWECMP,reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalShowEcmpMessageHandler));
        addOperationMap (NSMLOCALECMPCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalEcmpConfigMessageHandler));
        addOperationMap (VLAGEXMREBALANCE, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalMacRebalanceMessageHandler));
		addOperationMap (NSMLOCALPOLICYMAP,reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalPolicyMapMessageHandler));
		addOperationMap (POLICER_LOCAL_SHOW_ALL, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::PolicerLocalShowMessageHandler));
		addOperationMap (POLICER_LOCAL_CLEAR_ALL, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::PolicerLocalClearMessageHandler));
		addOperationMap (NSMLOCALVLAGCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalVlagConfigMessageHandler));
		addOperationMap (NSMLOCALSHOWVLAG,reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalShowVlagMessageHandler));
		addOperationMap (CONNECTOR_CONFIG_LOCAL_MESSAGE,reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::connectorConfigMessageHandler));
		addOperationMap (SWITCHPORT_VLAN_CONFIG_LOCAL_MESSAGE, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::switchportVlanConfigLocalMessageHandler));
		addOperationMap (NSM_SLOT_OFFLINE_MSG, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalSlotOfflineMessageHandler));
		addOperationMap (MAC_GROUP_CONFIG_LOCAL_MESSAGE, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::macGroupConfigLocalMessageHandler));
        addOperationMap (PROVISIONING_INFO, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::NsmLocalProvisioningInfoMessageHandler));
		addOperationMap (INTERFACE_STATE_REPLAY_MSG, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::interfaceStateReplayMessageHandler));
		addOperationMap (GVLAN_ENABLE_LOCAL_MESSAGE, reinterpret_cast<PrismMessageHandler> (&IntfConfLocalWorker::gVlanEnableLocalMessageHandler));


        addServiceIndependentOperationMap (INTERFACE_STATE_REPLAY_MSG, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&IntfConfLocalWorker::interfaceStateReplayServiceIndependentMessageHandler));
    }

    IntfConfLocalWorker::~IntfConfLocalWorker ()
    {
    }

    PrismMessage  *IntfConfLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMCREATEPHYINTF :
                pPrismMessage = new NsmCreatePhyIntfMessage ();
                break;
            case NSMUPDATEPHYINTF :
                pPrismMessage = new NsmUpdatePhyIntfMessage ();
                break;
			case NSMUPDATEBULKPHYINTF:
				pPrismMessage = new NsmUpdateBulkPhyIntfMessage();
				break;
            case NSMDELETEPHYINTF :
                pPrismMessage = new NsmDeletePhyIntfMessage ();
                break;
            case NSMADDPHYINTFTOVLANINTF :
                pPrismMessage = new NsmAddPhyIntfToVlanIntfMessage ();
                break;
            case NSMDELETEPHYINTFTOVLANINTF :
                pPrismMessage = new NsmDeletePhyIntfToVlanIntfMessage ();
                break;
            case NSMCREATEVLANINTFSP :
                pPrismMessage = new NsmCreateVlanIntfSPMessage ();
                break;
            case NSMUPDATEVLANINTFSP :
                pPrismMessage = new NsmUpdateVlanIntfSPMessage ();
                break;
            case NSMDELETEVLANINTFSP :
                pPrismMessage = new NsmDeleteVlanIntfSPMessage ();
                break;
            case NSMCREATEPOINTFSP :
                pPrismMessage = new NsmCreatePoIntfSPMessage ();
                break;
            case NSMUPDATEPOINTFSP :
                pPrismMessage = new NsmUpdatePoIntfSPMessage ();
                break;
            case NSMDELETEPOINTFSP :
                pPrismMessage = new NsmDeletePoIntfSPMessage ();
                break;
            case NSMADDPOTOVLANINTFSP :
                pPrismMessage = new NsmAddPoToVlanIntfSPMessage ();
                break;
            case NSMDELETEPOTOVLANINTFSP :
                pPrismMessage = new NsmDeletePoToVlanIntfSPMessage ();
                break;
            case NSMADDPHYINTFTOPOINTF :
                pPrismMessage = new NsmAddPhyIntfToPoIntfMessage ();
                break;
            case NSMDELETEPHYINTFTOPOINTF :
                pPrismMessage = new NsmDeletePhyIntfToPoIntfMessage ();
                break;
            case NSMUPDATELACPCONFIGSP :
                pPrismMessage = new NsmUpdateLacpConfigSPMessage ();
				break;
			case NSMASSOCMACACLPOLICYPOVLANINTFSP :
				pPrismMessage = new NsmAssocMacAclPolicyPoVlanIntfSPMessage ();
				break;
			case NSMASSOCMACACLPOLICYPHYINTF :
				pPrismMessage = new NsmAssocMacAclPolicyPhyIntfMessage ();
				break;
			case NSMLOCALASSOCIPACLPOLICYVEINTF :
				pPrismMessage = new NsmLocalAssocIpAclPolicyVeIntfMessage ();
				break;
			case NSMLOCALECMPCONFIG :
				pPrismMessage = new NsmLocalEcmpConfigMessage();
				break;
			case NSMLOCALSHOWECMP:
				pPrismMessage= new NsmLocalShowEcmpMessage();
				break;
			/* PBR */
			case NSMLOCALPBRPOINTF:
				pPrismMessage= new PBRPoVlanIntfSPMessage();
				break;
            case NSMLOCALPBRPHYINTF:
                pPrismMessage= new PBRPhyIntfMessage();
                break;
			case NSMLOCALPBRVEINTF:
				pPrismMessage= new PBRVeIntfMessage();
				break;
			case IPPOLICYDELETEPBR:
				pPrismMessage= new IpPolicyDeletePBRMessage();
				break;
            /* Ip Acl */
            case NSMASSOCIPACLPOLICYPOVLANINTFSP :
                pPrismMessage = new NsmAssocIpAclPolicyPoVlanIntfSPMessage ();
                break;
            case NSMASSOCIPACLPOLICYPHYINTF :
                pPrismMessage = new NsmAssocIpAclPolicyPhyIntfMessage ();
                break;
            case NSMLOCALRESERVEDVLAN:
                pPrismMessage = new NsmLocalReservedVlanMessage();
                break;

			/* SPAN Destination Mirrored Port Information */
			case NSMLOCALDESTINATIONMIRRORPORT:
				pPrismMessage = new NsmLocaldestinationMirrorportMessage();
				break;
			
			/* STORM CONTROL */

			case NSMSTORMCONTROLINTFSET:
				pPrismMessage = new NsmStormControlIntfSetMessage ();
				break;

            /* FCOE */
            case NSMLOCALFCOEVLAN:
                pPrismMessage = new NsmLocalFcoeVlanMessage();
                break;

            case NSMLOCALCONFIGQOSATTRIBUTES:
                pPrismMessage = new NsmLocalConfigQosAttributesMessage();
                break;
            case NSMQOSPOCONFIGSP:
                pPrismMessage = new NsmQosPOConfigSPMessage();
                break;
            case NSMEVENT:
            	pPrismMessage = new NsmEventMessage();
            	break;
            case LACPLOCALSHOW:
                pPrismMessage = new LacpLocalShowMessage ();
                break;
            case DOT1X_LOCAL_SHOW_ALL:
                pPrismMessage = new Dot1XLocalShowMessage ();
                break;
            case DOT1X_LOCAL_REAUTHENTICATE_ALL:
				pPrismMessage = new Dot1XLocalReauthenticateMessage ();
                break;
            case DOT1X_LOCAL_TEST_ALL:
				pPrismMessage = new Dot1XLocalTestMessage ();
                break;
			case DOT1X_LOCAL_CLEAR_ALL:
                pPrismMessage = new Dot1XLocalClearMessage ();
                break;
            case LACPLOCALCLEAR :
                pPrismMessage = new LacpLocalClearMessage();
                break;

			case NSMLOCALSHOWINTERFACE:
            	pPrismMessage = new NsmLocalShowIntfMessage();
            	break;
            case NSMLOCALPORTMEDIASHOWINTERFACE:
            	pPrismMessage = new NsmLocalPortMediaShowIntfMessage();
            	break;
            case NSMLOCALDEBUGCONFIG:
            	pPrismMessage = new NsmLocalDebugMessage();
            	break;
            case NSMUPDATEPOPRIMARYLINK:
            	pPrismMessage = new NsmUpdatePoPrimaryLinkMessage();
            	break;
            case NSM_LOCAL_CLEAR_MSG:
            	pPrismMessage = new NsmLocalClearMessage();
            	break;
            case VLAGEXMREBALANCE:
            	pPrismMessage = new NsmLocalMacRebalanceMessage();
            	break;
			case NSMLOCALPOLICYMAP:
				pPrismMessage = new NsmLocalPolicyMapMessage();
                break;
            case POLICER_LOCAL_SHOW_ALL:
                pPrismMessage = new PolicerLocalShowMessage ();
                break;
			case POLICER_LOCAL_CLEAR_ALL:
                pPrismMessage = new PolicerLocalClearMessage ();
                break;
            case NSMLOCALVLAGCONFIG:
                  pPrismMessage = new NsmLocalVlagConfigMessage();
                  break;
			case NSMLOCALSHOWVLAG:
				  pPrismMessage = new NsmLocalShowVlagMessage();
				  break;
			case CONNECTOR_CONFIG_LOCAL_MESSAGE:
				  pPrismMessage = new ConnectorConfigLocalMessage();
				  break;
			case SWITCHPORT_VLAN_CONFIG_LOCAL_MESSAGE:
				  pPrismMessage = new SwitchportVlanConfigLocalMessage();
				  break;
			case NSM_SLOT_OFFLINE_MSG:
				  pPrismMessage = new NsmLocalSlotOfflineMessage();
				  break;
			case INTERFACE_STATE_REPLAY_MSG:
				  pPrismMessage = new InterfaceStateReplayServiceIndependentMessage();
				  break;

			case MAC_GROUP_CONFIG_LOCAL_MESSAGE:
				  pPrismMessage = new MacGroupConfigLocalMessage();
				  break;
            case PROVISIONING_INFO: {
                  pPrismMessage = new NsmLocalProvisioningInfoMessage();
                  break;
            }
			case GVLAN_ENABLE_LOCAL_MESSAGE:
				  pPrismMessage = new GVlanEnableLocalMessage();
				  break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *IntfConfLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((PhyIntfLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PhyIntfLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ( (TenGigLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new TenGigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ( (OneGigLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new OneGigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ( (FortyGigLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FortyGigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ( (NativeVlanIntfConfLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NativeVlanIntfConfLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
		else if ((EcmpConfigManagedObject::getClassName ()) == managedClassName)
		{
			trace(TRACE_LEVEL_INFO,string("created ecmp managed object"));
			pWaveManagedObject = new EcmpConfigManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
        else if ((NsmLocalIngressEgressMacAclPolicyManagedObject::getClassName ()) == managedClassName)
        {
             pWaveManagedObject = new NsmLocalIngressEgressMacAclPolicyManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((NsmLocalIngressEgressIpAclPolicyManagedObject::getClassName ()) == managedClassName)
        {
			pWaveManagedObject = new NsmLocalIngressEgressIpAclPolicyManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((NsmStormControlIntfConfLocalManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new NsmStormControlIntfConfLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
        else if ((NsmLocalPBRManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NsmLocalPBRManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
		else if ( (NsmVlagConfigManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new NsmVlagConfigManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((ConnectorConfigLocalManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new ConnectorConfigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((VlanClassificationMO::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new VlanClassificationMO(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((MacAddressVlanClassificationMO::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new MacAddressVlanClassificationMO(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((MacGroupVlanClassificationMO::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new MacGroupVlanClassificationMO(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((CtagVlanClassificationMO::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new CtagVlanClassificationMO(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((CtagVlanClassificationRemoveMO::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new CtagVlanClassificationRemoveMO(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((CtagPVlanClassificationMO::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new CtagPVlanClassificationMO(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((CtagPVlanClassificationRemoveMO::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new CtagPVlanClassificationRemoveMO(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((RspanMacAddressVlanClassificationMO::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new RspanMacAddressVlanClassificationMO(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((RspanMacGroupVlanClassificationMO::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new RspanMacGroupVlanClassificationMO(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((RspanGVlanTrunkPortManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new RspanGVlanTrunkPortManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((RspanGVlanTrunkPortRemoveManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new RspanGVlanTrunkPortRemoveManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
        else if ((AMPPProfileCompositeManagedObject::getClassName() == managedClassName) )
        {
            pWaveManagedObject = new AMPPProfileCompositeManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if((AMPPProfileDomainCompositeManagedObject::getClassName() == managedClassName)) 
        {
            pWaveManagedObject = new AMPPProfileDomainCompositeManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
		else
		{
			trace (TRACE_LEVEL_FATAL, "IntfConfLocalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    ResourceId IntfConfLocalWorker::getPhyIntfObjectIdByIfName (string ifName, ObjectId &intfOid,
																PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"id"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() == 1) {
            pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);
            intfOid = pPhyIntfLocalManagedObject->getObjectId();
            status = WAVE_MESSAGE_SUCCESS;
        }

        if(pResults)
        {
			if(pPrismSynchronousLinearSequencerContext)
				pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            pResults->clear ();
            delete pResults;
        }

        return status;
    }

    ResourceId IntfConfLocalWorker:: getVlanIntfObjectIdByVlanIfName (string vlanName, ObjectId &poIntf_oid,
															bool removeImmediate,
															PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

        UI32 po_id;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
        po_id = strtoul(vlanName.c_str (), NULL, 0);
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&po_id,"id"));
        syncQueryCtxt.addSelectFieldForObjectId ("objectId");
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
            pVlanIntfManagedObject =  dynamic_cast<VlanIntfManagedObject*>((*pResults)[0]);
            poIntf_oid = pVlanIntfManagedObject->getObjectId();
            status = WAVE_MESSAGE_SUCCESS;
        }

        if (pResults) {
			if(removeImmediate)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			else
			{
				if(pPrismSynchronousLinearSequencerContext)
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
				pResults->clear();
				delete pResults;
			}
        }
        return status;
    }

    ResourceId IntfConfLocalWorker:: getPoIntfObjectIdByPoIfName (string poName, ObjectId &poIntf_oid,
                                                            bool removeImmediate,
                                                            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)

    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        PoIntfManagedObject *pPoIntfManagedObject = NULL;

        UI32 po_id;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        po_id = strtoul(poName.c_str (), NULL, 0);
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&po_id,"id"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
            pPoIntfManagedObject =  dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
            poIntf_oid = pPoIntfManagedObject->getObjectId();
            status = WAVE_MESSAGE_SUCCESS;
        }

        if (pResults) {
			if(removeImmediate)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			else
			{
				if(pPrismSynchronousLinearSequencerContext)
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
				pResults->clear();
				delete pResults;
			}
        }
//        delete pPoIntfManagedObject;
        return status;
    }

    void IntfConfLocalWorker::sendNsmClearCommandToCluster (PrismLinearSequencerContext *seq_ctx_p)
    {
        NSM_PLUG_DBG("Entering ...");
        NsmLocalClearMessage        *msg_p = NULL;
        NsmLocalClearMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext     *send_ctx_p = NULL;
        UI32                        locationId = 0;
        SI32                        mappedId = 0;
        vector<UI32>                locations;
        bool                        sendToLocal = false;
		InterfaceType 				ifType = IF_TYPE_INVALID;


        msg_p = dynamic_cast<NsmLocalClearMessage*> (seq_ctx_p->getPPrismMessage ());

        loc_msg_p = new NsmLocalClearMessage (msg_p->getIfName(),
                                             msg_p->getSlotId(),
                                             msg_p->getPortChannelId(),
                                             msg_p->getVlanId(),
                                             msg_p->getCmdCode(),
                                             false);

         NSM_PLUG_DBG_S(string("Sending Clear Command to Cluster") +
                        string (" getIfName =")  + msg_p->getIfName() +
                        string (" slotId =")  + msg_p->getSlotId() +
                        string (" portChannelId =")  + msg_p->getPortChannelId() +
                        string (" vlanId =")  + msg_p->getVlanId() +
                        string(" CmdCode =") + msg_p->getCmdCode() +
                        string (" false"));

        send_ctx_p = new WaveSendToClusterContext(this,
                reinterpret_cast<PrismAsynchronousCallback>
                (&IntfConfLocalWorker::NsmLocalClearSendToClusterCallback),
                seq_ctx_p);

        if (0 == msg_p->getIfName().length()) {
            NSM_PLUG_DBG("Sending Clear Command to Cluster getIfName().length == zero");
            // If the interface context is not there, send msg to all switches in cluster
			sendToLocal = false;
        } else {
			if(strncasecmp(msg_p->getInterface().c_str(), "po", 2) == 0)
			{
				if(PORT_NOT_PART_OF_ANY_INTERFACE == NsmUtils::setLocationIdsForPoSubModeCmds(msg_p->getIfName(), send_ctx_p))
				{
					NSM_PLUG_DBG("Port not part of any interface");
					seq_ctx_p -> executeNextStep(WAVE_MESSAGE_SUCCESS);
					delete loc_msg_p;
					delete send_ctx_p;
					return;
				}
				sendToLocal = false;
			}
			else if(IS_VALID_PHYSICAL_INTERFACE(msg_p->getInterface().c_str()))
			{
            if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  {
                if(IS_VALID_TE_INTERFACE(msg_p->getInterface().c_str()))
                {
                    ifType = IF_TYPE_TE;
                }
                if(IS_VALID_GI_INTERFACE(msg_p->getInterface().c_str()))
                {
                    ifType = IF_TYPE_GI;
                }
                if (ifType != IF_TYPE_INVALID)
                {
                    if(!NsmUtils::isValidInterface(msg_p->getIfName(), ifType))
                    {
                        //return;
                        NSM_PLUG_DBG(" Wrong interface ");
                        seq_ctx_p ->executeNextStep(WRC_NSM_API_SET_ERR_NO_SUCH_INTERFACE);
                        return;
                    }
                }

                NSM_PLUG_DBG("Sending Clear Command to Cluster isValidTwoTuple true");
				NSM_PLUG_DBG_S(string(" M_INTERFACE=") + msg_p->getInterface());
                sendToLocal = true;
            } else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  {
                /* Send to the targetted LocationId pointed to by the
                 * mappedId.
                 */
				if(IS_VALID_TE_INTERFACE(msg_p->getInterface().c_str()))
		        {
            		ifType = IF_TYPE_TE;
        		}
		        if(IS_VALID_GI_INTERFACE(msg_p->getInterface().c_str()))
        		{
        		    ifType = IF_TYPE_GI;
		        }
				if (ifType != IF_TYPE_INVALID)
				{
					if(!NsmUtils::isValidInterface(msg_p->getIfName(), ifType))
					{
						//return;
					  	NSM_PLUG_DBG(" Wrong interface ");
						seq_ctx_p ->executeNextStep(WRC_NSM_API_SET_ERR_NO_SUCH_INTERFACE);
						return;
					}
				}

                NSM_PLUG_DBG("Sending CLear Command to Cluster isValidThreeTuple true");
				NSM_PLUG_DBG_S(string(" M_INTERFACE=") + msg_p->getInterface());
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
                locations.push_back(locationId);
                send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
        }
            LocationId loc;
            string ifName;
            if(-1 == getIfShortName(msg_p->getInterface().c_str(), msg_p->getIfName().c_str(), ifName, loc) ) {
                if (NULL != send_ctx_p) {
                    delete send_ctx_p;
                    send_ctx_p = NULL;
                }
                return ;
            }
            loc_msg_p->setIfName(ifName);

        }

        if (sendToLocal) {
            /* It is not interface specific command or the interface is
             * two tuple one on the local switch.
             */
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) {
 				locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        sendToWaveCluster(send_ctx_p);

        return;
    }

    void IntfConfLocalWorker::NsmLocalClearSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        NSM_PLUG_DBG("Entering ...");
        PrismLinearSequencerContext     *seq_ctx_p = NULL;
        NsmLocalClearMessage            *resp_msg_p = NULL;



        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());


        resp_msg_p =  dynamic_cast<NsmLocalClearMessage *>
                     (ctx_p->getPPrismMessageForPhase1());


        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }

	ResourceId IntfConfLocalWorker::sendClearToNsmDaemon (PrismSynchronousLinearSequencerContext *ctx_p)
	{
		NSM_PLUG_DBG("Entering ...");
		NsmLocalClearMessage *msg_p = dynamic_cast<NsmLocalClearMessage *>
			(ctx_p->getPPrismMessage ());
		NsmClearMessage *cl_msg_p = NULL;
		ResourceId      status;
		SI32            mappedId = 0;
		string          ifName = "";


		NSM_PLUG_DBG_S(string(" IfName=") + msg_p->getIfName() + string(" CmdCode=") + msg_p->getCmdCode());
		mappedId = DcmToolKit::getLocalMappedId();

		if (mappedId == (-1)) {
			mappedId = 0;
		}


		cl_msg_p = new NsmClearMessage(msg_p->getIfName(), msg_p->getSlotId(), msg_p->getPortChannelId(), msg_p->getVlanId(), msg_p->getCmdCode());

		switch(msg_p->getCmdCode())
		{
		case NSM_CLEAR_COUNTERS_ALL:
			{
				FcoeClientClearMessage *fcoe_intf_clear_msg_p = NULL;

				fcoe_intf_clear_msg_p = new FcoeClientClearMessage();
				if (fcoe_intf_clear_msg_p) {

					fcoe_intf_clear_msg_p->setCmdCode(FCOE_CLEAR_COUNTERS_CMD_CODE_ALL);

					status = sendSynchronouslyToWaveClient ("fcoed", fcoe_intf_clear_msg_p);
					if ((status != WAVE_MESSAGE_SUCCESS) ||
						(fcoe_intf_clear_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)) {
						NSM_PLUG_DBG("Error: Failed to clear FCOE interface counters");
					}

					delete fcoe_intf_clear_msg_p;
				}
			}
			{
				QosClientClearMessage *pQosClientClearMessage  = NULL;

				pQosClientClearMessage = new QosClientClearMessage();
				if (pQosClientClearMessage) {

					pQosClientClearMessage->setCmdCode(SSM_QOS_CLEAR_COUNTERS_ALL);

					status = sendSynchronouslyToWaveClient ("qos", pQosClientClearMessage);
					if ((status != WAVE_MESSAGE_SUCCESS) ||
						(pQosClientClearMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)) {
						NSM_PLUG_DBG("Error: Failed to clear QOS counters");
					}

					delete pQosClientClearMessage;

				}
			}
			{
				AclClearStatsMessage *pAclClearStatsMessage = NULL;

				pAclClearStatsMessage = new AclClearStatsMessage();

				if (pAclClearStatsMessage) {
					pAclClearStatsMessage->setOpCode(SSMACL_DCMD_CLEAR_STATS_ALL);
					pAclClearStatsMessage->setAclType(SSMACL_TYPE_UNKNOWN_E);
					pAclClearStatsMessage->setDirection(SSMACL_BINDING_DIRECTION_BOTH_E);
					pAclClearStatsMessage->setPolicyName("");
					pAclClearStatsMessage->setIfName("");

					status = sendSynchronouslyToWaveClient ("ssm", pAclClearStatsMessage);
					if ((status != WAVE_MESSAGE_SUCCESS) ||
						(pAclClearStatsMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)) {
						NSM_PLUG_DBG("Error: Failed to clear SSM ACL counters");
					}

					delete pAclClearStatsMessage;

				}
			}
			{
				if(Capabilities::instance()->isCapabilityPresent(FEAT_CAPABILITY_POLICER))
				{
					PolicerClearMessage *pPolicerClearMessage = new PolicerClearMessage;
					
					if(pPolicerClearMessage) {
						pPolicerClearMessage->setDirection(FB_POLICY_DIR_BOTH);
						pPolicerClearMessage->setCmdCode(FB_CLEAR_POLICYMAP_COUNTER_ALL);
						status = sendSynchronouslyToWaveClient ("qos",pPolicerClearMessage);
					
						if ((status != WAVE_MESSAGE_SUCCESS) ||
								(pPolicerClearMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)) {
								NSM_PLUG_DBG("Error: Failed to clear SSM POLICER counters");
						}

						delete pPolicerClearMessage;
					}
					else
						NSM_PLUG_DBG("Error: new operation failed for PolicerClearMessage");
				}
			}
            {
				if(Capabilities::instance()->isCapabilityPresent(FEAT_CAPABILITY_STORMCONTROL))
				{
					StormControlClientClearMessage *pStormControlClientClearMessage =
						new StormControlClientClearMessage;
					
					if (pStormControlClientClearMessage)
					{
						pStormControlClientClearMessage->setOpCode(QOS_BUM_DCMD_CLEAR_SYSTEM);
						pStormControlClientClearMessage->setProtocol(QOS_BUM_PROTOCOL_UNKNOWN_E);
						status = sendSynchronouslyToWaveClient ("qos", pStormControlClientClearMessage);

						if ((status != WAVE_MESSAGE_SUCCESS) ||
							(pStormControlClientClearMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)) {
							NSM_PLUG_DBG("Error: Failed to clear STORM CONTROL counters");
						}
						delete pStormControlClientClearMessage;
					}
				}
				else 
					NSM_PLUG_DBG("Info: STORM CONTROL is not supported on this platform");
            }
				
			    break;
     		case NSM_CLEAR_COUNTERS_IF:
			{
				NSM_PLUG_DBG_S(string("sending clear counter interface to ssm ;if name = ") + msg_p->getIfName());
				QosClientClearMessage *pQosClientClearMessage  = NULL;

				pQosClientClearMessage = new QosClientClearMessage();
				if (pQosClientClearMessage) {

					pQosClientClearMessage->setCmdCode(SSM_QOS_CLEAR_COUNTERS_INTERFACE);
					pQosClientClearMessage->setIfName( msg_p->getIfName());
					status = sendSynchronouslyToWaveClient ("qos", pQosClientClearMessage);
					if ((status != WAVE_MESSAGE_SUCCESS) ||
						(pQosClientClearMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)) {
						NSM_PLUG_DBG("Error: Failed to clear QOS counters");
					}
					delete pQosClientClearMessage;
				}

			}
			break;
		    default:
			    break;
		}

		status = sendSynchronouslyToWaveClient("nsm", cl_msg_p);
		if (status == WAVE_MESSAGE_SUCCESS) {
			if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
				trace(TRACE_LEVEL_ERROR,
					string("Resp: Client Completion Error MsgId: ") +
					cl_msg_p->getMessageId() +
					FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
				msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
				status = msg_p->getCompletionStatus();
				delete cl_msg_p;
				return status;
			}

			if (cl_msg_p->getClientStatus() != 0) {
				trace(TRACE_LEVEL_ERROR, string("Resp:  Client Error MsgId: ") +
					cl_msg_p->getMessageId() +
					FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
				msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
				status = msg_p->getCompletionStatus();
				delete cl_msg_p;
				return status;
			}

		}

		if (status != WAVE_MESSAGE_SUCCESS) {
			trace(TRACE_LEVEL_DEBUG,
				string("Client Message sending failed for MsgId: ") +
				cl_msg_p->getMessageId() +
				string("reason:") +  FrameworkToolKit::localize(status));
			//bpatel: to avoid memory leak
			delete cl_msg_p;
			return status;
		}

		trace(TRACE_LEVEL_DEBUG,
			string("Client Message processing succesful for MsgId: ") +
			cl_msg_p->getMessageId());
		status = cl_msg_p->getCompletionStatus();
		delete cl_msg_p;
		return status;
		return WAVE_MESSAGE_SUCCESS;

	}


	void IntfConfLocalWorker::NsmLocalClearMessageHandler(NsmLocalClearMessage* pNsmLocalClearMessageHandler)
	{

		if (pNsmLocalClearMessageHandler->getIsGwRequest())

	{
			PrismLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::sendNsmClearCommandToCluster),
				reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),
				reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
			};

			PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pNsmLocalClearMessageHandler, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
			pPrismLinearSequencerContext->start ();
			return;
		} else {
			PrismSynchronousLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::sendClearToNsmDaemon),
				reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
				reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
			};

			PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext( pNsmLocalClearMessageHandler, this, sequencerSteps,
				sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
			pPrismSynchronousLinearSequencerContext->execute ();

		}
	}

    ResourceId IntfConfLocalWorker::addPhyIntfToPoIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NSM_PLUG_DBG("Entering ...");
        NsmAddPhyIntfToPoIntfMessage *pNsmAddPhyIntfToPoIntfMessage = dynamic_cast<NsmAddPhyIntfToPoIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        pNsmAddPhyIntfToPoIntfMessage->setCompletionStatus(WAVE_MESSAGE_ERROR);
        string threeTupleIfName = pNsmAddPhyIntfToPoIntfMessage->getIfName();
        //ResourceId  status   = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&threeTupleIfName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                //status = WAVE_MESSAGE_ERROR;
            }else if (1 == numberOfResults) {
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                //status = WAVE_MESSAGE_SUCCESS;
            } else {
                //status = WAVE_MESSAGE_ERROR;
            }
        }

        if (pPhyIntfLocalManagedObject == NULL) {
            return (WAVE_MESSAGE_ERROR);
        } else {
            ObjectId poObjectId;
            updateWaveManagedObject (pPhyIntfLocalManagedObject);

            prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);

            getPoIntfObjectIdByPoIfName (pNsmAddPhyIntfToPoIntfMessage->getPoName(), poObjectId,false,pPrismSynchronousLinearSequencerContext);

            pPhyIntfLocalManagedObject->setPoId(poObjectId);
            pPhyIntfLocalManagedObject->setPoMode (pNsmAddPhyIntfToPoIntfMessage->getPoMode());
            pPhyIntfLocalManagedObject->setPoType (pNsmAddPhyIntfToPoIntfMessage->getPoType());

            /* set to default timeout for dynamic trunk */
            if (pNsmAddPhyIntfToPoIntfMessage->getPoMode() != NSM_PO_MODE_ON) {
                if (pNsmAddPhyIntfToPoIntfMessage->getPoType() == NSM_PO_TYPE_STANDARD) {
                    pPhyIntfLocalManagedObject->setLacpTimeout(LACP_TIMEOUT_LONG);
                } else {
                    pPhyIntfLocalManagedObject->setLacpTimeout(LACP_TIMEOUT_SHORT);
                }
            }

            NSM_PLUG_INFO_S(string("Updating PhyIntf through channel-group cmd  end... phyName,poName : ") + pNsmAddPhyIntfToPoIntfMessage->getIfName() + string(",")+ pNsmAddPhyIntfToPoIntfMessage->getPoName() );

        }
		if(pResults)
		{
			if(pPrismSynchronousLinearSequencerContext)
				pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			pResults->clear();
			delete pResults;
		}
        pNsmAddPhyIntfToPoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        return pNsmAddPhyIntfToPoIntfMessage->getCompletionStatus ();
    }

    ResourceId
    IntfConfLocalWorker::fcoeAddPhyIntfToPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        UI32    po_id;
        FcoeClientEthConfigMessage  *dmn_msg_p = NULL;
        vector<UI64> ifindex_vec;
        UI64 lag_ifindex = 0;
        UI64 mbr_ifindex = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        if (NsmUtils::isFsbEnabled() == false) {
        NsmAddPhyIntfToPoIntfMessage *pNsmAddPhyIntfToPoIntfMessage = dynamic_cast<NsmAddPhyIntfToPoIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        po_id = strtoul(pNsmAddPhyIntfToPoIntfMessage->getPoName().c_str(), NULL, 0);
		status = validatePoId(po_id);

		if(status != WAVE_MESSAGE_SUCCESS)
		{
			return status;
		}

        if (NsmUtils::isFcoeConfiguredForLagPort(po_id) == false) {
            NSM_PLUG_INFO_S(string("Fcoe is not configured for Po ") + pNsmAddPhyIntfToPoIntfMessage->getPoName());
            return WAVE_MESSAGE_SUCCESS;
        }

        mbr_ifindex = NsmUtils::getPhyPortIfindexByName(pNsmAddPhyIntfToPoIntfMessage->getIfName());

        NSM_PLUG_INFO_S(string("Adding Member=") + pNsmAddPhyIntfToPoIntfMessage->getIfName() +
                        string("Ifindex=") + mbr_ifindex);

        ifindex_vec.push_back(mbr_ifindex);

        dmn_msg_p = new FcoeClientEthConfigMessage();
        dmn_msg_p->setOper_bmp(FCOE_ETH_INTF_OPER_LAG_ADD_MBR);
        dmn_msg_p->setIfindex(lag_ifindex);
        dmn_msg_p->setIfId(pNsmAddPhyIntfToPoIntfMessage->getPoName());
        dmn_msg_p->setFcoeMbrIfindexVec(ifindex_vec);

        status = sendSynchronouslyToWaveClient (FCOE_DCM_CLIENT_NAME, dmn_msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("Message Status = ") + status);
        }

        if (dmn_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("Client Status = ") + dmn_msg_p->getClientStatus());
            status = dmn_msg_p->getClientStatus();
        }

        if (dmn_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("COmpletion Status = ") + dmn_msg_p->getCompletionStatus());
            status = dmn_msg_p->getCompletionStatus();
        }

        NSM_PLUG_INFO_S(string("Final Status") + status);
        delete (dmn_msg_p);
       }
        return status;
    }

	ResourceId IntfConfLocalWorker::validatePoId (UI32 poId)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		if(poId < 0 || poId > DCM_PORTCHANNEL_MAX_LAGS)
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validatePoId: Invalid port-channel id:") + poId);

			status = WRC_NSM_ERR_INVALID_PO_ID;
		}

		return status;
	}

    ResourceId IntfConfLocalWorker::validatePhyIntfToPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NSM_PLUG_DBG("Entering ...");
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		int PoSize = 0;
        ResourceId isVCS = DcmToolKit::isVcsEnabled ();

        NsmAddPhyIntfToPoIntfMessage *pNsmAddPhyIntfToPoIntfMessage = dynamic_cast<NsmAddPhyIntfToPoIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        string poName = pNsmAddPhyIntfToPoIntfMessage->getPoName();
        PoSize = NsmUtils::getPoNumOfMemberPortsByLocationId(poName, FrameworkToolKit::getThisLocationId ());

        if (NSM_PO_TYPE_STANDARD == pNsmAddPhyIntfToPoIntfMessage->getPoType()) { // Po Type is Standard
            if((WAVE_PERSISTENCE_CLUSTER_ENABLED == isVCS && DCM_MAX_AGG_LINKS_PER_NODE_VCS == PoSize) ||
                (WAVE_PERSISTENCE_CLUSTER_DISABLED == isVCS && DCM_MAX_AGGREGATOR_LINKS_SA == PoSize)) {
                NSM_PLUG_DBG_ERR("Reached maximum ports per port-channel(standard)");
                status = WRC_NSM_API_ERR_PORTCHANNEL_MAX_LINKS_STD;
            }
        } /*Max link count check for brocade po will be done by backend code */

        pNsmAddPhyIntfToPoIntfMessage->setCompletionStatus (status);
        return status;
    }

    ResourceId IntfConfLocalWorker::addPhyIntfToPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NSM_PLUG_DBG("Entering ...");
        NsmAddPhyIntfToPoIntfMessage *pNsmAddPhyIntfToPoIntfMessage = dynamic_cast<NsmAddPhyIntfToPoIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        int status;
        vector<UI64> ifindex_vec;
        UI64 lag_ifindex = 0;
        UI64 mbr_ifindex = 0;
        FcoeClientEthConfigMessage  *dmn_msg_p = NULL;
		string twoTupleIfName;
		if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
			NsmUtils::getTwoTupleIfName(pNsmAddPhyIntfToPoIntfMessage->getIfName(), twoTupleIfName);
		else
			twoTupleIfName = pNsmAddPhyIntfToPoIntfMessage->getIfName();
        trace(TRACE_LEVEL_INFO,string("IntfConfLocalWo addPhyIntfToPoIntfStep twoTupleIfName ifIndex ")+twoTupleIfName + "  "+ pNsmAddPhyIntfToPoIntfMessage->getSwIfIndex());
        NsmPhyIntfMessage *m = new NsmPhyIntfMessage(twoTupleIfName,pNsmAddPhyIntfToPoIntfMessage->getSwIfIndex(),NSM_MTU_DEF_VAL,NSM_OPENFLOW_DEF_STATUS,"",false,false,1,false,NSM_ADD_CHANNELGROUP,LACP_TIMEOUT_DEFAULT,DCM_LACP_DEFAULT_PORT_PRIORITY,(LineSpeed)1,1,pNsmAddPhyIntfToPoIntfMessage->getPoName(),false,20,32768,false,0,0,0,false);

		m->setIfType(pNsmAddPhyIntfToPoIntfMessage->getIfType());

        m->setPoMode(pNsmAddPhyIntfToPoIntfMessage->getPoMode());
        m->setPoType(pNsmAddPhyIntfToPoIntfMessage->getPoType());

        m->setPoLacpSystemId(pNsmAddPhyIntfToPoIntfMessage->getPoLacpSystemId());
        m->setPoLacpSystemIdDomainId(pNsmAddPhyIntfToPoIntfMessage->getPoLacpSystemIdDomainId());

        NSM_PLUG_INFO_S( string ("UP_PO_L Channel-group Cmd Sending to Backend  PhyName : ") + twoTupleIfName + string (" poName :") + pNsmAddPhyIntfToPoIntfMessage->getPoName() + string(" PoMode : ") + pNsmAddPhyIntfToPoIntfMessage->getPoMode() +string (" poType :") + pNsmAddPhyIntfToPoIntfMessage->getPoType());

        status = sendSynchronouslyToWaveClient ("nsm",m);

        NSM_PLUG_INFO_S( string ("UP_PO_L Receiving Channel-group cmd response from Backend status :") + status + string("backend_clientStatus :") + m->getClientStatus());

        pNsmAddPhyIntfToPoIntfMessage->setCompletionStatus(m->getCompletionStatus());

        if( status == WAVE_MESSAGE_SUCCESS)
        {
            if(  pNsmAddPhyIntfToPoIntfMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
				//bpatel:
				status = pNsmAddPhyIntfToPoIntfMessage->getCompletionStatus();
				delete m;
				return status;
			}
            if ( (0 != m->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != m->getClientStatus() ) )  {
                trace(TRACE_LEVEL_ERROR, string("Resp:  Client Error MsgId: ") +
                        m->getMessageId() +
                        FrameworkToolKit::localize(m->getClientStatus()));
				//bpatel: why two return statements
                mbr_ifindex = NsmUtils::getPhyPortIfindexByName(pNsmAddPhyIntfToPoIntfMessage->getIfName());
				NSM_PLUG_INFO_S(string("Deleting Member=") + pNsmAddPhyIntfToPoIntfMessage->getIfName() +
                            string("Ifindex=") + mbr_ifindex);

                ifindex_vec.push_back(mbr_ifindex);
                dmn_msg_p = new FcoeClientEthConfigMessage();
                dmn_msg_p->setOper_bmp(FCOE_ETH_INTF_OPER_LAG_DEL_MBR);
                dmn_msg_p->setIfindex(lag_ifindex);
                dmn_msg_p->setIfId(pNsmAddPhyIntfToPoIntfMessage->getPoName());
                dmn_msg_p->setFcoeMbrIfindexVec(ifindex_vec);
				 
                status = sendSynchronouslyToWaveClient (FCOE_DCM_CLIENT_NAME, dmn_msg_p);
                if (status != WAVE_MESSAGE_SUCCESS) {
                   NSM_PLUG_DBG_ERR_S(string("Message Status = ") + status);
                }
				
                if (dmn_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
                   NSM_PLUG_DBG_ERR_S(string("Client Status = ") + dmn_msg_p->getClientStatus());
                   status = dmn_msg_p->getClientStatus();
                }
			
                if (dmn_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                   NSM_PLUG_DBG_ERR_S(string("COmpletion Status = ") + dmn_msg_p->getCompletionStatus());
                   status = dmn_msg_p->getCompletionStatus();
                }
			
                NSM_PLUG_INFO_S(string("Final Status") + status);
                delete (dmn_msg_p);

				status = m->getClientStatus();
				delete m;
				m = NULL;
				return NsmUtils::mapBackEndErrorsToDcmdErrors(status);
            }
        }
        //bpatel
		if(m){
			delete m;
		}

		if((WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()) && (pNsmAddPhyIntfToPoIntfMessage->getReplayPoConfigs() == true))
		{
			NSM_PLUG_INFO_S( string ("UP_PO_L Channel-group Replaying Port-channel Configuraiton Po :: ") + pNsmAddPhyIntfToPoIntfMessage->getPoName());
			status = sendPoConfigToLocal(pNsmAddPhyIntfToPoIntfMessage->getPoName());//Replay for ELD is called within the function sendPoConfigToLocal. 
			//Hence the code to replay ELD is removed here.
		}

		/**
		 *  Replay the ACL config for the port-channel, if the getReplayPoConfigs() returns true.
		 */
		if( ( WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled() ) && ( pNsmAddPhyIntfToPoIntfMessage->getReplayPoConfigs() == true ) )
		{
			trace(TRACE_LEVEL_DEVEL, string("addPhyIntfToPoIntfStep: ACL assignment replay : Port-channel name: ") + pNsmAddPhyIntfToPoIntfMessage->getPoName());

			char buffer[25];
			buffer[24] = '\0';
			UI32 bufferSize = sizeof (buffer);
			string ifname = "";
			UI32 ifId = 0;
			ssmAcl_binding_direction direction = SSMACL_BINDING_DIRECTION_IN_E;
			int noOfDirection = 1;

			if ( Capabilities::isCapabilityPresent(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS) )
			{
				noOfDirection = 2;
			}

			PoIntfManagedObject *pPoIntfManagedObject = NULL;

			WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());

			ifId = strtoul(pNsmAddPhyIntfToPoIntfMessage->getPoName().c_str (), NULL, 0);

			syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifId,"id"));

			vector<WaveManagedObject *> *pResults = NULL;

			pResults = querySynchronously (&syncQueryCtxt);

			if ( NULL != pResults )
			{
				UI32 numberOfResults = pResults->size ();

				if (numberOfResults == 1)
				{
					pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);

					if ( pPoIntfManagedObject )
					{
						for( int aclTypeIdx = 0; aclTypeIdx < 2 ; aclTypeIdx ++ ) // to replay both MAC ACL and IP ACL.
						{
							for( int idx = 0; idx < noOfDirection ; idx ++ ) // to replay both directions Ingress and Egress.
							{
								string policyName;

								if( idx == 1 )
								{
									direction = SSMACL_BINDING_DIRECTION_OUT_E;
								}

								trace(TRACE_LEVEL_DEVEL, string("addPhyIntfToPoIntfStep: ACL direction: ") + direction);

								int found = 0;
								string moName = "";
								ObjectId policyObjId = ObjectId::NullObjectId;

								if( aclTypeIdx == 0 )
								{
									moName = MacAclPolicyManagedObject::getClassName();
									found = pPoIntfManagedObject->getIngressEgressMacAclPolicyObjectId (policyObjId, direction);
								}
								else if( aclTypeIdx == 1 )
								{
									moName = IpAclPolicyManagedObject::getClassName();
									found = pPoIntfManagedObject->getIngressEgressIpAclPolicyObjectId (policyObjId, direction);
								}

								trace(TRACE_LEVEL_DEVEL, string("addPhyIntfToPoIntfStep: ACL found: ") + found );

								if ( 1 == found )
								{
									if (policyObjId != ObjectId::NullObjectId)
									{
								        WaveManagedObjectSynchronousQueryContext syncQueryAclCtxt(moName);
								        syncQueryAclCtxt.addAndAttribute (new AttributeObjectId (policyObjId, "objectid"));
								        syncQueryAclCtxt.setLoadOneToManyRelationships(false);
								        syncQueryAclCtxt.addSelectField ("policyName");
								        vector<WaveManagedObject *> *pResultsP  = querySynchronously (&syncQueryAclCtxt);

								        if ( pResultsP )
								        {
								            if ( pResultsP->size() > 0 )
								            {
									        	if( aclTypeIdx == 0 )
									        	{
									            	MacAclPolicyManagedObject* policyMO = dynamic_cast<MacAclPolicyManagedObject *>((*pResultsP)[0]);
									                if (policyMO)
									                {
									                	policyName = policyMO->getPolicyName();
									                }
									        	}
									        	else if( aclTypeIdx == 1 )
									        	{
									            	IpAclPolicyManagedObject* policyMO = dynamic_cast<IpAclPolicyManagedObject *>((*pResultsP)[0]);
									                if (policyMO)
									                {
									                	policyName = policyMO->getPolicyName();
									                }
									        	}
								            }

											if( pResultsP )
											{
									        	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsP);
											}
								        }
									}

									trace(TRACE_LEVEL_DEVEL, string("addPhyIntfToPoIntfStep: ACL policy name: ") + policyName );

									SsmIpAclPolicyMessage *cMsg = new SsmIpAclPolicyMessage();

									if (cMsg)
									{
										snprintf(buffer, bufferSize, "po%s", pNsmAddPhyIntfToPoIntfMessage->getPoName().c_str());

										ifname = buffer;

										cMsg->setPolicyName(policyName);
										cMsg->setDirection(direction);
										cMsg->setIfname(ifname);

										if( aclTypeIdx == 0 )
										{
											cMsg->setOpCode(SSMACL_DCMD_BIND_L2_ACL);
											cMsg->setPolicyType(SSMACL_TYPE_L2_E);
										}
										else if( aclTypeIdx == 1 )
										{
											cMsg->setOpCode(SSMACL_DCMD_BIND_L3_IPV4_ACL);
											cMsg->setPolicyType(SSMACL_TYPE_L3_IPV4_E);
										}

										status = sendSynchronouslyToWaveClient ("ssm", cMsg);

										if (status == WAVE_MESSAGE_SUCCESS)
										{
											status = cMsg->getClientStatus();

											trace(TRACE_LEVEL_DEVEL, string("addPhyIntfToPoIntfStep: Replaying ACL assignment on Port-channel client status: ") + status);

											if ( status == WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS_MAC )
											{
												status = WRC_SSM_DCM_ERR_APPLY_MAC_ACL_ON_INTF;
											}
										}
										else
										{
											trace(TRACE_LEVEL_ERROR, string("addPhyIntfToPoIntfStep: Replaying ACL assignment on Port-channel Failed: ") + cMsg->getClientStatus());
										}

										delete cMsg;
										cMsg = NULL;
									}
									else
									{
										status = WAVE_MESSAGE_ERROR;
										trace (TRACE_LEVEL_ERROR, "addPhyIntfToPoIntfStep: Replaying ACL assignment: Message creation failed");
									}
								}
							}
						}
					}
				}

				if(pResults)
				{
		        	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				}
			}
		}

        return status;

    }

    ResourceId IntfConfLocalWorker::sendPoConfigToLocal (const string& ifName)
    {
        NSM_PLUG_DBG("Entering ...");
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        PoIntfManagedObject *pPoIntfManagedObject =NULL;
        UI32 portId = (UI32) strtoul(ifName.c_str(), NULL, 0);
        NsmUtils::getPoMoByPoId(portId, pPoIntfManagedObject);
        NSM_PLUG_ASSERT_PTR(pPoIntfManagedObject);


        if (ProvisioningUtil::isGvlanEnabled()) {

            bool isPortProfiled = false;
            APPMUtils::isPortProfilePort(pPoIntfManagedObject, isPortProfiled);

            if (isPortProfiled) {

                UI32Range toBeProvisionedVlanRange;
                vector<string> portProfiles;
                string portProfileDomainName = "";
                APPMUtils::getProfilesAndDomainsofPoPhy(pPoIntfManagedObject, portProfiles, portProfileDomainName);
                
                getAllVlansToBeProvisioned(portProfiles, portProfileDomainName, toBeProvisionedVlanRange);
                
                tracePrintf(
                    TRACE_LEVEL_INFO,
                    "PoCOnfig PortProfiled, toBeProvisionedVlanRange<%s>",
                    toBeProvisionedVlanRange.toString().c_str());
                //Better write a util in APPM and use it here, the same can be
                //reused to populate all the vlans present in a given pp domain.
                PROVISION_VLAN_RANGE(NULL, toBeProvisionedVlanRange,
                    PROVISION_SPECIFIED);
            } else if(pPoIntfManagedObject->getSwModeFlag()) {
                    provisionVlansForSwitchportPo(pPoIntfManagedObject);
            }

        }

        NsmLocalObjectManager *nsmLocalObjMgr = NsmLocalObjectManager::getInstance();

        status = nsmLocalObjMgr->Pass1PoConfiguration(pPoIntfManagedObject,false);
        if (status == WAVE_MESSAGE_SUCCESS) {

            SsmMacAclIntfAssocBulkMessage *intfBulkMsg = NULL;
            intfBulkMsg = SsmLocalObjectManager::ConfigReplayMacAclGlobalIntfAssoc(PO_INTF_TYPE, pPoIntfManagedObject);
			if (intfBulkMsg) {
				status = sendSynchronouslyToWaveClient ("ssm",intfBulkMsg);
				if (status == WAVE_MESSAGE_SUCCESS) {
					NSM_PLUG_INFO_S(string ("PortChannel Interface Mac Acl configuration successful Nsm IntfConflocalWorker") + ifName);
				} else {
					NSM_PLUG_DBG_ERR_S(string("PortChannel Inerface Mac Acl Interface configuration failed - ") + status);
				}
				delete intfBulkMsg;
				intfBulkMsg = NULL;
			}

			nsmLocalObjMgr->configReplayPoQosMaps(pPoIntfManagedObject);
			nsmLocalObjMgr->configReplayPoQosPfc(pPoIntfManagedObject);
			nsmLocalObjMgr->configReplayPoQosCosProfile(pPoIntfManagedObject);
			//nsmLocalObjMgr->configReplayPoQosCosTailDrop(pPoIntfManagedObject);
			nsmLocalObjMgr->configReplayPoCee(pPoIntfManagedObject);
			nsmLocalObjMgr->configReplayPoPriorityTag(pPoIntfManagedObject);
			nsmLocalObjMgr->configReplayVlanClassifierGroupToPo(pPoIntfManagedObject);

            nsmLocalObjMgr->configReplayXstpPortConfig(pPoIntfManagedObject);
            nsmLocalObjMgr->configReplayXstpPortInstanceConfig(pPoIntfManagedObject);
            nsmLocalObjMgr->configReplayXstpPortVlanConfig(pPoIntfManagedObject);
			nsmLocalObjMgr->configReplayBpduGuardConfig(NULL, pPoIntfManagedObject);

			nsmLocalObjMgr->postBootFcoeProvisioningConfigForPoInterface(pPoIntfManagedObject, true);
            nsmLocalObjMgr->configReplayEldPcInteface(pPoIntfManagedObject);


            if(Capabilities::isCapabilityPresent(FEAT_CAPABILITY_POLICER)) { 
                PolicerLocalObjectManager::configReplayPhyPolicyMaps(
                        (PhyIntfLocalManagedObject*) NULL, pPoIntfManagedObject);
            } else {
                tracePrintf(TRACE_LEVEL_INFO,"MQC not supported on this platform.");
            }
		}

        if(pPoIntfManagedObject != NULL)
            delete pPoIntfManagedObject;

        return status;
    }

	void  IntfConfLocalWorker::NsmLocalPolicyMapMessageHandler(NsmLocalPolicyMapMessage *pNsmLocalPolicyMapMessage)
	{
		PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::addPolicyMapToIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::addPolicyMapToIntfMOStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pNsmLocalPolicyMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
	}

	ResourceId IntfConfLocalWorker::addPolicyMapToIntfStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
   {
	   	if(!Capabilities::instance()->isCapabilityPresent(FEAT_CAPABILITY_POLICER)) {
			return WRC_SSM_POL_ERR_NOT_SUPPORTED_PLATFORM;
	   	}

		NsmLocalPolicyMapMessage *pNsmLocalPolicyMapMessage = dynamic_cast<NsmLocalPolicyMapMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		trace (TRACE_LEVEL_DEBUG, string("Inside addPolicyMapToIntfStep -------"));
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		PolicerClientTeMessage *pPolicerClientMessage = new PolicerClientTeMessage();
		trace (TRACE_LEVEL_DEBUG, string("pNsmLocalPolicyMapMessage->getIfName()=")+ pNsmLocalPolicyMapMessage->getIfName());
		string ifName = pNsmLocalPolicyMapMessage->getIfName();
	    PhyIntfLocalManagedObject* pInterface = NULL;
    	InterfaceType ifType = IF_TYPE_INVALID;
		ObjectId PolicyId = ObjectId::NullObjectId;

		//NsmUtils::getPhyMoByPhyName(ifName, pInterface);
        
        //Partial Query instead of loading the complete PhyIntfLocalManagedObject.
        WaveManagedObjectSynchronousQueryContext phyIntfSyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        phyIntfSyncQueryCtxt.addAndAttribute(new AttributeString(&ifName, "id"));
        vector<string> listOfAttributes;
        listOfAttributes.push_back("bumConfig");
        listOfAttributes.push_back("isPortProfiled");
        listOfAttributes.push_back("associatedPortProfiles");
        listOfAttributes.push_back("associatedPortProfileDomain");
        listOfAttributes.push_back("isdestinationMirrorportEnable");
        listOfAttributes.push_back("phyType");
        listOfAttributes.push_back("inputPolicyMapId");
        listOfAttributes.push_back("outputPolicyMapId");
        listOfAttributes.push_back("Portrole");
        listOfAttributes.push_back("flowControlTx");
        listOfAttributes.push_back("flowControlRx");
        listOfAttributes.push_back("poId");
        listOfAttributes.push_back("fcoeport");

        phyIntfSyncQueryCtxt.addSelectFields(listOfAttributes);
        vector<WaveManagedObject * >* pResultsIntf = querySynchronously(&phyIntfSyncQueryCtxt);

        if (pResultsIntf && (pResultsIntf -> size() == 1)) {
            NSM_PLUG_DBG_S(string("Found Phy Mo for") + ifName);
            pInterface  = dynamic_cast<PhyIntfLocalManagedObject*>((*pResultsIntf)[0]);
        } 

		if (pInterface) {
            
            //Set the pInterface in DCESynchronousLinearSeqContext, so the same object can be accessed in successive sequencer steps.
            
            pPrismSynchronousLinearSequencerContext->setPWaveManagedObject(pInterface);
			if(pNsmLocalPolicyMapMessage->getCmdCode()== FB_POLICYMAP_BIND_INTERFACE_IN)
			{
				vector <WaveManagedObjectPointer<NsmStormControlIntfConfLocalManagedObject> > bumConfigMO =
				                    pInterface->getBumConfig();
			
				if(bumConfigMO.size()!=0)
				{
					delete pPolicerClientMessage;
					status = WRC_SSM_POL_ERR_STORM_CONTROL_ENABLED;
                    if ( pPrismSynchronousLinearSequencerContext ) {
                        pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsIntf);
                    }
                    return status;
				}
			}
            
            //Co Existence of port-profile port and policer config on an interface.
            bool isPortProfiled = false;
            APPMUtils::isPortProfilePort(pInterface, isPortProfiled);

            if (((pNsmLocalPolicyMapMessage->getCmdCode()
				== FB_POLICYMAP_BIND_INTERFACE_IN)
				|| (pNsmLocalPolicyMapMessage->getCmdCode()
						== FB_POLICYMAP_BIND_INTERFACE_OUT))
				&& (isPortProfiled)){

                status = WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED;
                delete pPolicerClientMessage;
                if ( pPrismSynchronousLinearSequencerContext ) {
                    pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsIntf);
                }
                return status;
            }


            //Co Existence of "qos flowcontrol" on LAG and policer config on a LAG member contradict.
            ObjectId poObjectId = ObjectId::NullObjectId;
            poObjectId = pInterface->getPoId();

            PoIntfManagedObject *pPoIntfManagedObject = NULL;
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute(new AttributeUI32(poObjectId.getClassId(), "objectidclassid") );
            syncQueryCtxt.addAndAttribute(new AttributeUI64(poObjectId.getInstanceId(), "objectidinstanceid") );
            syncQueryCtxt.addSelectField("flowControlTx");
            syncQueryCtxt.addSelectField("flowControlRx");


            if ( ObjectId::NullObjectId != poObjectId ) {

                vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
                if (pResults && pResults->size()  == 1) {
                    pPoIntfManagedObject =  dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
                }

                if ( pResults ) {
                    if(pPrismSynchronousLinearSequencerContext) {
                        pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                    }
                    pResults->clear();
                    delete pResults;
                }
                
                //If the "qos flowcontrol tx on/off rx on/off" is enabled on the PO,
                //Policer cannot be applied on a member port.
                if(pPoIntfManagedObject->getFlowControlTx() == 1 || pPoIntfManagedObject->getFlowControlRx() == 1 ) {
                    status =
						SSM_INTF_POLICER_ON_MEMBER_PORT_CONFLICTS_WITH_QOS_FLOWCONTROL_ON_LAG;
					if (pPrismSynchronousLinearSequencerContext) {
						pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(
								*pResultsIntf);
					}
					return status;
                }
            }



			if (pInterface->getDestinationmirrorportEnable())
			{
				delete pPolicerClientMessage;
				status = WRC_NSM_INTERFACE_IS_DESTINATION_MIRROR_PORT;
				if (pPrismSynchronousLinearSequencerContext) {
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(
							*pResultsIntf);
				}
				return status;
			}

			
			ifType = pInterface->getPhyType();

			switch(pNsmLocalPolicyMapMessage->getCmdCode()) {

				case FB_POLICYMAP_BIND_INTERFACE_IN:
				case FB_POLICYMAP_UNBIND_INTERFACE_IN: {
					PolicyId = pInterface->getInputPolicyMapId();
					break;
				}
				case FB_POLICYMAP_BIND_INTERFACE_OUT:
				case FB_POLICYMAP_UNBIND_INTERFACE_OUT: {
					PolicyId = pInterface->getOutputPolicyMapId();
					break;
				}
			}

            //Config of qos service-policy on an isl is blocked, but unconfig is allowed on the same.
            if ((DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pInterface->getPortrole())) {

                if( ((FB_POLICYMAP_BIND_INTERFACE_IN == pNsmLocalPolicyMapMessage->getCmdCode())
                            || (FB_POLICYMAP_BIND_INTERFACE_OUT == pNsmLocalPolicyMapMessage->getCmdCode()))) {
                    status = WRC_SSM_POL_ERR_ISL_PORT;
                }
                delete pPolicerClientMessage;
                if (pPrismSynchronousLinearSequencerContext) {
                    pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(
                            *pResultsIntf);
                }
                return status;
            }
			else if((pInterface->getFlowControlTx() == 1)||(pInterface->getFlowControlRx() == 1))
			{
				delete pPolicerClientMessage;
				status = WRC_SSM_POL_ERR_FLOW_CONTROL_ENABLED;
				if (pPrismSynchronousLinearSequencerContext) {
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(
							*pResultsIntf);
				}
				return status;

			}
			//service-policy and fcoe config cannot co-exist
			if (((FB_POLICYMAP_BIND_INTERFACE_IN
					== pNsmLocalPolicyMapMessage->getCmdCode())
					|| (FB_POLICYMAP_BIND_INTERFACE_OUT
							== pNsmLocalPolicyMapMessage->getCmdCode()))
					&& (QosUtils::isInterfaceFcoeConfigured(pInterface))) {
				delete pPolicerClientMessage;
				status =
						WRC_SSM_QOS_FLOWBASED_SERVICE_POLICY_AND_FCOE_CANNOT_CO_EXIST;
				if (pPrismSynchronousLinearSequencerContext) {
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(
							*pResultsIntf);
				}
				return status;

			}


//			delete pInterface;

            //Add the pInterface for garbage collection, do not delete pInterface here, because the same instance is needed in other sequencer steps.
            if ( pPrismSynchronousLinearSequencerContext ) {
                pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsIntf);
            }
		}
		else
		{
			trace (TRACE_LEVEL_DEBUG, string("Interface MO can't be retrived"));
			delete pPolicerClientMessage;
			return WRC_SSM_POL_ERR_INTF_NOT_FOUND;
		}

		if(true  == NsmUtils::isValidThreeTuple(ifName))
		{
			string twoTuppleIfName;
			NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
			ifName = twoTuppleIfName;
		}

		if(pNsmLocalPolicyMapMessage->getOpCode() == DELETE_INTERFACE_QOS) {

				if(PolicyId != ObjectId::NullObjectId)
				{
					PolicyClassMapManagedObject *policyMapMO = dynamic_cast<PolicyClassMapManagedObject *>(queryManagedObject(PolicyId));
					if(policyMapMO)
					{
						pPolicerClientMessage->setMapName(policyMapMO->getPolicyClassMapName());
                        pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(policyMapMO);
					}
					else
					{
						trace (TRACE_LEVEL_DEBUG, string("MO not found for policyMap"));
						delete pPolicerClientMessage;
						return WRC_SSM_POL_ERR_POLICY_MAP_NOT_FOUND;
					}
				}
				else
				{
					trace (TRACE_LEVEL_DEBUG, string("No Policy Map Attached"));
					delete pPolicerClientMessage;
					return WAVE_MESSAGE_SUCCESS;
				}
		}
		else {
			pPolicerClientMessage->setMapName(pNsmLocalPolicyMapMessage->getMapName());
		}

	    char buffer[MAX_STR];
   	 	buffer[MAX_STR - 1] = '\0';
    	UI32 bufferSize = sizeof (buffer);
		GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer, bufferSize);
		ifName = buffer;
		trace (TRACE_LEVEL_DEBUG, string("pNsmLocalPolicyMapMessage->ifName()=") + ifName);

		pPolicerClientMessage->setIfName(ifName);
		pPolicerClientMessage->setOpCode(pNsmLocalPolicyMapMessage->getOpCode());
		pPolicerClientMessage->setCmdCode(pNsmLocalPolicyMapMessage->getCmdCode());

		trace (TRACE_LEVEL_DEBUG, string("MapName=")+pPolicerClientMessage->getMapName());
		trace (TRACE_LEVEL_DEBUG, string("pPolicerClientMessage->getOpCode()=") + pPolicerClientMessage->getOpCode());
		trace (TRACE_LEVEL_DEBUG, string("pPolicerClientMessage-->getCmdCode()=") + pPolicerClientMessage->getCmdCode());

		status = sendSynchronouslyToWaveClient ("qos",pPolicerClientMessage);
		if(WAVE_MESSAGE_SUCCESS != status)
		{
			NSM_PLUG_DBG_S(string("sendSynchronouslyToWaveClient Failed") + status);
			delete pPolicerClientMessage;
			return status;
		}

		status = pPolicerClientMessage->getClientStatus();
		NSM_PLUG_DBG_S(string("return client status is ")+ FrameworkToolKit::localize(status));
		pNsmLocalPolicyMapMessage->setCompletionStatus(status);
		//pNsmLocalPolicyMapMessage->copyAllBuffers(*m);
		if(pPolicerClientMessage){
			delete pPolicerClientMessage;
		}

		return status;
 	}

	ResourceId IntfConfLocalWorker::addPolicyMapToIntfMOStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {
		trace (TRACE_LEVEL_DEBUG, string("Inside addPolicyMapToIntfMOStep-------"));
		NsmLocalPolicyMapMessage *pNsmLocalPolicyMapMessage = dynamic_cast<NsmLocalPolicyMapMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		trace (TRACE_LEVEL_DEBUG, string("DB pNsmLocalPolicyMapMessage->getOpCode()=") + pNsmLocalPolicyMapMessage->getOpCode());
		trace (TRACE_LEVEL_DEBUG, string("DB pNsmLocalPolicyMapMessage->getCmdCode()=") + pNsmLocalPolicyMapMessage->getCmdCode());
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		string ifName = pNsmLocalPolicyMapMessage->getIfName();
		ResourceId  status   = WAVE_MESSAGE_SUCCESS;

        pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *> (pPrismSynchronousLinearSequencerContext->getPWaveManagedObject());
	
        if (pPhyIntfLocalManagedObject == NULL) {
			return (WRC_SSM_POL_ERR_INTF_NOT_FOUND);
		}

		prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);

		ObjectId PolicyId;
		switch(pNsmLocalPolicyMapMessage->getOpCode()) {

			case CONFIGURE_INTERFACE_QOS: {
				trace (TRACE_LEVEL_DEBUG, string("DB UPDATE CONFIGURE_INTERFACE_QOS-------"));
				switch (pNsmLocalPolicyMapMessage->getCmdCode()) {
					case FB_POLICYMAP_BIND_INTERFACE_IN: {
						trace (TRACE_LEVEL_DEBUG, string("DB UPDATE FB_POLICYMAP_BIND_INTERFACE_IN-------"));
						PolicyId = getObjectIdFromPolicyMapName(pNsmLocalPolicyMapMessage->getMapName(), pPrismSynchronousLinearSequencerContext);
						if(PolicyId != ObjectId::NullObjectId) {
                            pPhyIntfLocalManagedObject->setInputPolicyMapId(PolicyId);
                            pPhyIntfLocalManagedObject->addAttributeToBeUpdated("inputPolicyMapId");
                        } else {
							status = WRC_SSM_POL_ERR_POLICY_MAP_NOT_FOUND;
                        }
						break;
					}
					case FB_POLICYMAP_BIND_INTERFACE_OUT: {
						trace (TRACE_LEVEL_DEBUG, string("DB UPDATE FB_POLICYMAP_BIND_INTERFACE_OUT-------"));
						PolicyId = getObjectIdFromPolicyMapName(pNsmLocalPolicyMapMessage->getMapName(), pPrismSynchronousLinearSequencerContext);
						if(PolicyId != ObjectId::NullObjectId) {
							pPhyIntfLocalManagedObject->setOutputPolicyMapId(PolicyId);
                            pPhyIntfLocalManagedObject->addAttributeToBeUpdated("outputPolicyMapId");
                        } else {
							status = WRC_SSM_POL_ERR_POLICY_MAP_NOT_FOUND;
                        }
						break;
					}
				}
				break;
			}
			case DELETE_INTERFACE_QOS: {
				trace (TRACE_LEVEL_DEBUG, string("DB UPDATE DELETE_INTERFACE_QOS-------"));
				switch (pNsmLocalPolicyMapMessage->getCmdCode()) {
					case FB_POLICYMAP_UNBIND_INTERFACE_IN: {
						trace (TRACE_LEVEL_DEBUG, string("DB UPDATE FB_POLICYMAP_UNBIND_INTERFACE_IN-------"));
						pPhyIntfLocalManagedObject->setInputPolicyMapId(ObjectId::NullObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("inputPolicyMapId");
                        break;
					}
					case FB_POLICYMAP_UNBIND_INTERFACE_OUT: {
						trace (TRACE_LEVEL_DEBUG, string("DB UPDATE FB_POLICYMAP_UNBIND_INTERFACE_OUT-------"));
						pPhyIntfLocalManagedObject->setOutputPolicyMapId(ObjectId::NullObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("outputPolicyMapId");
                        break;
					}
				}
				break;
			}
		}
        
        updateWaveManagedObject (pPhyIntfLocalManagedObject);
        
        return status;
	}


	ObjectId IntfConfLocalWorker::getObjectIdFromPolicyMapName(string Name, PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		PolicyClassMapManagedObject *pPolicyClassMapManagedObject;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PolicyClassMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&Name,"policyClassMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
                return ObjectId::NullObjectId;
            }else if (1 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
                pPolicyClassMapManagedObject = dynamic_cast<PolicyClassMapManagedObject *>((*pResults)[0]);
                if(pPolicyClassMapManagedObject != NULL)
				{
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
					return pPolicyClassMapManagedObject->getObjectId();
				}
				else
					return ObjectId::NullObjectId;
            } else {
                return ObjectId::NullObjectId;
            }
        }
		return ObjectId::NullObjectId;
	}

	 void
    IntfConfLocalWorker::PolicerLocalShowMessageHandler (
            PolicerLocalShowMessage *pPolicerLocalShowMessage)
    {

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::PolicerLocalShowMessageHandler\n");
        trace(TRACE_LEVEL_DEBUG, "IntfConfLocalWorker::Obtained PolicerLocalShowMessage at local plugin\n");

        if (pPolicerLocalShowMessage->getIsGwRequest()) {
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::sendPolicerShowCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext =
                new PrismLinearSequencerContext(pPolicerLocalShowMessage,
                    this, sequencerSteps,
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&IntfConfLocalWorker::sendShowToPolicerDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
                };

            PrismSynchronousLinearSequencerContext
                *pPrismSynchronousLinearSequencerContext=
                new PrismSynchronousLinearSequencerContext(
                        pPolicerLocalShowMessage, this, sequencerSteps,
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }


    void
    IntfConfLocalWorker::sendPolicerShowCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
    {
        PolicerLocalShowMessage        *msg_p = NULL;
        PolicerLocalShowMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext     *send_ctx_p = NULL;
        vector<UI32>                locations;

        trace(TRACE_LEVEL_DEBUG,
			"Entering IntfConfLocalWorker::sendPolicerShowCommandToCluster\n");
	msg_p =
			dynamic_cast<PolicerLocalShowMessage *>(seq_ctx_p->getPPrismMessage());

	loc_msg_p = new PolicerLocalShowMessage(msg_p->getInterface(),
			msg_p->getIfName(), msg_p->getDirection(), msg_p->getCmdCode(),
			false, msg_p->getPolicyMapName(), msg_p->getLocationId());

	trace(
			TRACE_LEVEL_DEBUG,
			string("Sending Show Command to Cluster") + string(" CmdCode =")
					+ msg_p->getCmdCode() + string(" Direction =")
					+ msg_p->getDirection() + string(" getInterface =")
					+ msg_p->getInterface() + string(" ifName =")
					+ msg_p->getIfName() + string(" false") + "PolicyMapName: "
					+ msg_p->getPolicyMapName());

        send_ctx_p = new WaveSendToClusterContext(this,
                reinterpret_cast<PrismAsynchronousCallback>
                (&IntfConfLocalWorker::PolicerLocalShowSendToClusterCallback),
                seq_ctx_p);


    //If the locationId is set, it means the message is intended for a single node.
    //Hence send the message to the specific node only.
    if (FB_INVALID_LOCATION_ID != msg_p->getLocationId()) {
		locations.push_back(msg_p->getLocationId());
		send_ctx_p->setLocationsToSendToForPhase1(locations);
	}
	//For PO, send message only to the nodes containing memeber ports.
	else if (msg_p->getIfName().substr(0, 2).compare("po") == 0) {
		string poId = msg_p->getIfName().substr(2,
				(msg_p->getIfName().length()) - 1);
		NsmUtils::getPoMemberLocationIds(poId, locations);
		for (unsigned it = 0; it < locations.size(); it++) {
			trace(
					TRACE_LEVEL_DEVEL,
					string("it<") + it + string("> Location ID<")
							+ locations[it] + string(">"));
		}
		send_ctx_p->setLocationsToSendToForPhase1(locations);
	} else {

	}


        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
		send_ctx_p->setPartialSuccessFlag(true);
		send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);

        sendToWaveCluster(send_ctx_p);

        return;
    }



    ResourceId
    IntfConfLocalWorker::sendShowToPolicerDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        PolicerLocalShowMessage *msg_p = dynamic_cast<PolicerLocalShowMessage *>
                                      (ctx_p->getPPrismMessage ());
		
        PolicerShowMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
        string          ifName = "";

        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) {
            mappedId = 0;
        }

        ifName = msg_p->getIfName();

        cl_msg_p = new PolicerShowMessage(msg_p->getInterface(), ifName, msg_p->getDirection(),

msg_p->getCmdCode(), mappedId, msg_p->getPolicyMapName());

        trace(TRACE_LEVEL_DEBUG,
			"Entering IntfConfLocalWorker::sendShowToQoSDaemon");
        trace(
			TRACE_LEVEL_DEBUG,
			string("Sending PolicerShowMessage to qos daemon ::") + string(" IfName<")
					+ cl_msg_p->getIfName() + string("> CmdCode<")
					+ cl_msg_p->getCmdCode() + "> PolicyMapName<"
					+ cl_msg_p->getPolicyMapName() + "> Direction<"
					+ cl_msg_p->getDirection());

        status = sendSynchronouslyToWaveClient("qos", cl_msg_p);

        trace(
			TRACE_LEVEL_DEBUG,
			string("Received PolicerShowMessage from back end :")
					+ "Completionstatus<" + cl_msg_p->getCompletionStatus()
					+ "> Clientstatus<" + cl_msg_p->getClientStatus() + ">");

		if (status == WAVE_MESSAGE_SUCCESS) {
			if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
				trace(
						TRACE_LEVEL_ERROR,
						string("Error in completion status for MsgId: ")
								+ cl_msg_p->getMessageId()
								+ FrameworkToolKit::localize(
										cl_msg_p->getCompletionStatus()));
				msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
				//bpatel : To avoid memory leak
				status = msg_p->getCompletionStatus();
				delete cl_msg_p;
				return status;
			}

			if (cl_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
				trace(
						TRACE_LEVEL_ERROR,
						string("Error in client status for MsgId:  ")
								+ cl_msg_p->getMessageId()
								+ FrameworkToolKit::localize(
										cl_msg_p->getClientStatus()));
				msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
				//bpatel : To avoid memory leak
				status = msg_p->getCompletionStatus();
				delete cl_msg_p;
				return status;
			}

		}

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG,
            string("Client PolicerShowMessage sending failed for MsgId: ") +
                    cl_msg_p->getMessageId() +
                    string("reason:") +  FrameworkToolKit::localize(status));
             //bpatel: to avoid memory leak
            delete cl_msg_p;
            return status;
        }

        trace(TRACE_LEVEL_DEBUG,
                string("Client PolicerShowMessage processing succesful for MsgId: ") +
                cl_msg_p->getMessageId());

		trace(TRACE_LEVEL_DEBUG,string("buffer in msg ") + cl_msg_p->getNumberOfBuffers());
        msg_p->copyAllBuffers(*cl_msg_p);
        //bpatel: to avoid memory leak
        status = cl_msg_p->getCompletionStatus();
        delete cl_msg_p;
        return status;
    }


    void
    IntfConfLocalWorker::PolicerLocalShowSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext     *seq_ctx_p = NULL;
        PolicerLocalShowMessage           *req_msg_p = NULL,
                                        *resp_msg_p = NULL,
                                        *sw_resp_p = NULL;
                                        
        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::PolicerLocalShowSendToClusterCallbac\n");

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<PolicerLocalShowMessage *>
                    (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<PolicerLocalShowMessage *>
                     (ctx_p->getPPrismMessageForPhase1());
                     
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        
        if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus ) {
            
            vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
            UI32    i = 0;
            
            for (i = 0; i < locations.size(); i++) {
                
                ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locations[i]);
                
                if(WAVE_MESSAGE_SUCCESS == statusPerLocationId) {
                    sw_resp_p = dynamic_cast<PolicerLocalShowMessage *>
        		                (ctx_p->getResultingMessageForPhase1(locations[i]));
                    trace(TRACE_LEVEL_DEVEL, string("copy buffers") + sw_resp_p->getNumberOfBuffers());
                    req_msg_p->copyAllBuffers(*sw_resp_p);
                } else {
                    trace(TRACE_LEVEL_ERROR, string("Response processing: The location: ")
                            + locations[i] + string("returned error: ")  + FrameworkToolKit::localize(statusPerLocationId));
                }
            }
        } else {
            trace(TRACE_LEVEL_ERROR,string("PolicerLocalShowSendToClusterCallback returned error: ")  
                            + FrameworkToolKit::localize(sendToClusterCompletionStatus));
        }

        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }

	void
    IntfConfLocalWorker::PolicerLocalClearMessageHandler (
            PolicerLocalClearMessage *pPolicerLocalClearMessage)
    {

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::PolicerLocalClearMessageHandler\n");
        if (pPolicerLocalClearMessage->getIsGwRequest()) {
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::sendPolicerClearCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext =
                new PrismLinearSequencerContext(pPolicerLocalClearMessage,
                    this, sequencerSteps,
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&IntfConfLocalWorker::sendClearToPolicerDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
                };

            PrismSynchronousLinearSequencerContext
                *pPrismSynchronousLinearSequencerContext=
                new PrismSynchronousLinearSequencerContext(
                        pPolicerLocalClearMessage, this, sequencerSteps,
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }


    void
    IntfConfLocalWorker::sendPolicerClearCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
    {
        PolicerLocalClearMessage        *msg_p = NULL;
        PolicerLocalClearMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext     *send_ctx_p = NULL;
    	vector<UI32> locations;

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::sendPolicerClearCommandToCluster\n");

        msg_p = dynamic_cast<PolicerLocalClearMessage *> (seq_ctx_p->getPPrismMessage ());

        loc_msg_p = new PolicerLocalClearMessage(msg_p->getInterface(),
			msg_p->getIfName(), msg_p->getMapName(), msg_p->getDirection(),
			msg_p->getCmdCode(), false);

        trace(
			TRACE_LEVEL_DEBUG,
			string("Sending Clear Command to Cluster") + string(" CmdCode =")
					+ msg_p->getCmdCode() + string(" Direction =")
					+ msg_p->getDirection() + string(" getInterface =")
					+ msg_p->getInterface() + string(" ifName =")
					+ msg_p->getIfName() + string(" mapName =")
					+ msg_p->getMapName() + string(" false"));

        send_ctx_p = new WaveSendToClusterContext(this,
                reinterpret_cast<PrismAsynchronousCallback>
                (&IntfConfLocalWorker::PolicerLocalClearSendToClusterCallback),
                seq_ctx_p);


        //For PO, send message only to the nodes containing memeber ports.
	if (msg_p->getIfName().substr(0, 2).compare("po") == 0) {
		string poId = msg_p->getIfName().substr(2,
				(msg_p->getIfName().length()) - 1);
		NsmUtils::getPoMemberLocationIds(poId, locations);
		for (unsigned it = 0; it < locations.size(); it++) {
			trace(
					TRACE_LEVEL_DEVEL,
					string("it<") + it + string("> Location ID<")
							+ locations[it] + string(">"));
		}
		send_ctx_p->setLocationsToSendToForPhase1(locations);
	}


        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
		send_ctx_p->setPartialSuccessFlag(true);
		send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);

        sendToWaveCluster(send_ctx_p);

        return;
    }



    ResourceId
    IntfConfLocalWorker::sendClearToPolicerDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        PolicerLocalClearMessage *msg_p = dynamic_cast<PolicerLocalClearMessage *>
                                      (ctx_p->getPPrismMessage ());
        PolicerClearMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
        string          ifName = "";

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::sendClearToPolicerDaemon");
        trace(TRACE_LEVEL_DEBUG, string("IntfConfLocalWorker::sendClearToPolicerDaemon::getInterface=")
                        + msg_p->getInterface() + string(" IfName=") + msg_p->getIfName() + string(" CmdCode=") + msg_p->getCmdCode() + string(" mapName=") + msg_p->getMapName());
        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) {
            mappedId = 0;
        }

        ifName = msg_p->getIfName();

        cl_msg_p = new PolicerClearMessage(msg_p->getInterface(), ifName, msg_p->getMapName(), msg_p->getDirection(),

msg_p->getCmdCode(), mappedId);

        status = sendSynchronouslyToWaveClient("qos", cl_msg_p);
        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR,
                        string("Resp: Client Completion Error MsgId: ") +
                        cl_msg_p->getMessageId() +
                       FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                //bpatel : To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;
                return status;
            }

            if (cl_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR, string("Resp:  Client Error MsgId: ") +
                                  cl_msg_p->getMessageId() +
                FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                //bpatel : To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;
                return status;
            }

        }

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG,
            string("Client Message sending failed for MsgId: ") +
                    cl_msg_p->getMessageId() +
                    string("reason:") +  FrameworkToolKit::localize(status));
             //bpatel: to avoid memory leak
            delete cl_msg_p;
            return status;
        }

        trace(TRACE_LEVEL_DEBUG,
                string("Client Message processing succesful for MsgId: ") +
                cl_msg_p->getMessageId());

        msg_p->copyAllBuffers(*cl_msg_p);
        //bpatel: to avoid memory leak
        status = cl_msg_p->getCompletionStatus();
        delete cl_msg_p;
        return status;
    }


    void
    IntfConfLocalWorker::PolicerLocalClearSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext     *seq_ctx_p = NULL;
        PolicerLocalClearMessage           *req_msg_p = NULL,
                                        *resp_msg_p = NULL,
                                        *sw_resp_p = NULL;

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::PolicerLocalClearSendToClusterCallbac\n");

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<PolicerLocalClearMessage *>
                    (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<PolicerLocalClearMessage *>
                     (ctx_p->getPPrismMessageForPhase1());
                     
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        
        if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus ) {
            
            vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
            UI32    i = 0;
            
            for (i = 0; i < locations.size(); i++) {
                
                ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locations[i]);

                if(WAVE_MESSAGE_SUCCESS == statusPerLocationId) {
                    sw_resp_p = dynamic_cast<PolicerLocalClearMessage *>
                                    (ctx_p->getResultingMessageForPhase1(locations[i]));
                    req_msg_p->copyAllBuffers(*sw_resp_p);
                } else {
                    trace(TRACE_LEVEL_ERROR, string("Response processing: The location: ")
                            + locations[i] + string("returned error: ")  + FrameworkToolKit::localize(statusPerLocationId));
                }
            }
        } else {
            trace( TRACE_LEVEL_ERROR,string("PolicerLocalClearSendToClusterCallback returned error: ")
							+ FrameworkToolKit::localize(sendToClusterCompletionStatus));
        }

        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }

    void  IntfConfLocalWorker::NsmAddPhyIntfToPoIntfMessageHandler( NsmAddPhyIntfToPoIntfMessage *pNsmAddPhyIntfToPoIntfMessage)
    {
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = NULL;

        if(pNsmAddPhyIntfToPoIntfMessage->getCmdCode() == NSMUPDATE_PHY_ADD_CHANNELGROUP_CMD )
        {

            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::validatePhyIntfToPoIntfStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::fcoeAddPhyIntfToPoIntfStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::addPhyIntfToPoIntfStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };

            pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmAddPhyIntfToPoIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        } else if(pNsmAddPhyIntfToPoIntfMessage->getCmdCode() == NSMUPDATE_PHY_ADD_CHANNELGROUP_DBCOMMIT_CMD )
        {

            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {

                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::addPhyIntfToPoIntfMOStep),

                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };

            pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmAddPhyIntfToPoIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        }

        if(pPrismSynchronousLinearSequencerContext != NULL)
            pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  IntfConfLocalWorker::NsmAddPhyIntfToVlanIntfMessageHandler( NsmAddPhyIntfToVlanIntfMessage *pNsmAddPhyIntfToVlanIntfMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmAddPhyIntfToVlanIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  IntfConfLocalWorker::NsmAddPoToVlanIntfSPMessageHandler( NsmAddPoToVlanIntfSPMessage *pNsmAddPoToVlanIntfSPMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmAddPoToVlanIntfSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ObjectId
    IntfConfLocalWorker::FcoeMapGetObjectIdByName (string &map_name)
    {
        FcoeMapManagedObject *map_mo_p;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&map_name, "map_name"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
            return ObjectId::NullObjectId;
        }

        map_mo_p = dynamic_cast<FcoeMapManagedObject *> ((*pResults)[0]);

		if(pResults)
        	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        return map_mo_p->getObjectId();
    }

	ResourceId IntfConfLocalWorker:: getCosMapObjectIdByCosMapName(string cosMapName, ObjectId &cosMapObjectId,
															PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        CosToCosMutationMapManagedObject *pCosToCosMutationMapManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToCosMutationMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&cosMapName,"cosToCosMutationMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
          pCosToCosMutationMapManagedObject = dynamic_cast<CosToCosMutationMapManagedObject*>((*pResults)[0]);
          cosMapObjectId = pCosToCosMutationMapManagedObject->getObjectId();
            status = WAVE_MESSAGE_SUCCESS;
        }
        if (pResults) {
			if(pPrismSynchronousLinearSequencerContext) {
            	pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            pResults->clear();
           delete pResults;
            } else {
        	    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
        }
        return status;
    }

    ResourceId IntfConfLocalWorker:: getTcMapObjectIdByTcMapName(string cosMapName, ObjectId &cosMapObjectId,
															PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        CosToTrafficClassMapManagedObject *pCosToTrafficClassMapManagedObject = NULL;
        printf("PRINTING ---- querying database --\n");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToTrafficClassMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&cosMapName,"cosToTrafficClassMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
          pCosToTrafficClassMapManagedObject = dynamic_cast<CosToTrafficClassMapManagedObject*>((*pResults)[0]);
          cosMapObjectId = pCosToTrafficClassMapManagedObject->getObjectId();
          printf("PRINTING ------ successfully stored qos map object id ---\n");
            status = WAVE_MESSAGE_SUCCESS;
        }
        if (pResults) {
			if(pPrismSynchronousLinearSequencerContext) {
            	pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            pResults->clear();
           delete pResults;
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
        }
        return status;
    }

    ResourceId IntfConfLocalWorker::triggerPostBootProgramming(const UI32 eventId, const UI32 parameter)
	{
#if 0
        trace (TRACE_LEVEL_INFO, string("Postboot triggering started....... "));
        ExecutePostbootPassTableMessage *pMessage = new ExecutePostbootPassTableMessage();
        ResourceId status = sendOneWay(pMessage);
       // ResourceId status = sendSynchronously(pMessage);
        trace (TRACE_LEVEL_INFO, string("Postboot triggering status:   ") + status);
		//bpatel: To avoid memory leak
        if(pMessage){
            delete pMessage;
        }
		return (status);
#endif
        NsmServiceReadyEvent  *pNsmServiceReadyEvent  = NULL;
        NsmChassisReadyEvent  *pNsmChassisReadyEvent  = NULL;
        NsmLinecardReadyEvent *pNsmLinecardReadyEvent = NULL;
        NsmLinecardOnlineEvent *pNsmLinecardOnlineEvent = NULL;

        // Broadcast NSM_SLOT_READY event
        switch (eventId)
        {
        case NSM_SERVICE_READY:
			{
        	pNsmServiceReadyEvent = new NsmServiceReadyEvent ();
			broadcast (pNsmServiceReadyEvent);
			}
            break;

        case NSM_CHASSIS_READY:
			{
            pNsmChassisReadyEvent = new NsmChassisReadyEvent ();
			broadcast (pNsmChassisReadyEvent);
			}
            break;

        case NSM_LINECARD_READY:
			{
            pNsmLinecardReadyEvent = new NsmLinecardReadyEvent ();
            pNsmLinecardReadyEvent->setSlotId(parameter);
			broadcast (pNsmLinecardReadyEvent);
			}
            break;

        case NSM_LINECARD_ONLINE:
            pNsmLinecardOnlineEvent = new NsmLinecardOnlineEvent ();
            pNsmLinecardOnlineEvent->setSlotId(parameter);
            broadcast (pNsmLinecardOnlineEvent);
            break;
        }
        return (WAVE_MESSAGE_SUCCESS);
	}


    ResourceId IntfConfLocalWorker::configReplayIntfAdminState(const UI32 parameter)
    {
        trace (TRACE_LEVEL_INFO, "Entering IntfConfLocalWorker::configReplayIntfAdminState ..");
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
        LocationId                          locId = 0;
        int status = 0;
		int32_t slotId = parameter;
        locId = FrameworkToolKit::getThisLocationId ();
		vector<PhyIntfLocalManagedObject *> results;
		NsmUtils::SlotGetPhyIntfMOs(results, slotId, locId);
		
        if (results.size() > 0) {
			for (UI32 i = 0; i < results.size(); i++) {
				pPhyIntfLocalManagedObject = results[i];
                status = (NsmLocalObjectManager::getInstance())->PassShutdownPhyConfiguration(pPhyIntfLocalManagedObject);

                if (0 != status)
                {
                    // FIXME : Sagar : WE need to handle this case.  Otherwise, remove the status variable.
                }

				delete pPhyIntfLocalManagedObject;
            }
        }
        return (WAVE_MESSAGE_SUCCESS);
    }


    ResourceId IntfConfLocalWorker:: getCeeMapObjectIdByCeeMapName(string ceeMapName, ObjectId &ceeMapObjectId,
															PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        CeeMapManagedObject *pCeeMapManagedObject = NULL;
        printf("PRINTING ---- querying database --\n");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ceeMapName,"ceeMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
          pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResults)[0]);
          ceeMapObjectId = pCeeMapManagedObject->getObjectId();
          printf("PRINTING ------ successfully stored cee map object id ---\n");
            status = WAVE_MESSAGE_SUCCESS;
        }
        if (pResults) {
			if(pPrismSynchronousLinearSequencerContext) {
            	pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            pResults->clear();
            delete pResults;
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
        }
        return status;
    }

	ResourceId IntfConfLocalWorker:: getDscpMapObjectIdByDscpMapName(string dscpMapName, ObjectId &dscpMapObjectId,
                                                            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId  status   = WRC_SSM_QOS_MAP_NOT_FOUND;
        DscpToDscpMutationMapManagedObject *pDscpToDscpMutationMapManagedObject = NULL;
        printf("PRINTING ---- querying database --\n");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToDscpMutationMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&dscpMapName,"dscpToDscpMutationMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
          pDscpToDscpMutationMapManagedObject = dynamic_cast<DscpToDscpMutationMapManagedObject*>((*pResults)[0]);
          dscpMapObjectId = pDscpToDscpMutationMapManagedObject->getObjectId();
          printf("PRINTING ------ successfully stored dscp to dscp map object id ---\n");
            status = WAVE_MESSAGE_SUCCESS;
        }
        if (pResults) {
            if(pPrismSynchronousLinearSequencerContext)
                pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            pResults->clear();
            delete pResults;
        }
        return status;
    }

	ResourceId IntfConfLocalWorker::getDscpTcMapObjectIdByTcMapName(string dscpTcMapName, ObjectId &dscpTcMapObjectId,
                                                            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId  status   = WRC_SSM_QOS_MAP_NOT_FOUND;
        DscpToTrafficClassMapManagedObject *pDscpToTrafficClassMapManagedObject = NULL;
        printf("PRINTING ---- querying database --\n");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt( DscpToTrafficClassMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&dscpTcMapName,"dscpToTrafficClassMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
          pDscpToTrafficClassMapManagedObject = dynamic_cast<DscpToTrafficClassMapManagedObject*>((*pResults)[0]);
          dscpTcMapObjectId = pDscpToTrafficClassMapManagedObject->getObjectId();
          printf("PRINTING ------ successfully stored dscp to traffic class map object id ---\n");
            status = WAVE_MESSAGE_SUCCESS;
        }
        if (pResults) {
            if(pPrismSynchronousLinearSequencerContext)
                pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            pResults->clear();
            delete pResults;
        }
        return status;
    }

	ResourceId IntfConfLocalWorker:: getDscpCosMapObjectIdByDscpCosMapName(string dscpCosMapName, ObjectId &dscpCosMapObjectId,
                                                            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId  status   = WRC_SSM_QOS_MAP_NOT_FOUND;
        DscpToCosMapManagedObject *pDscpToCosMapManagedObject = NULL;
        printf("PRINTING ---- querying database --\n");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToCosMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&dscpCosMapName,"dscpToCosMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
          pDscpToCosMapManagedObject = dynamic_cast<DscpToCosMapManagedObject*>((*pResults)[0]);
          dscpCosMapObjectId = pDscpToCosMapManagedObject->getObjectId();
          printf("PRINTING ------ successfully stored dscp to cos map object id ---\n");
            status = WAVE_MESSAGE_SUCCESS;
        }
        if (pResults) {
            if(pPrismSynchronousLinearSequencerContext)
                pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            pResults->clear();
            delete pResults;
        }
        return status;
    }

/* Code Starts Here */
    ResourceId IntfConfLocalWorker::createPhyIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NSM_PLUG_DBG("Entering ...");
        NsmCreatePhyIntfMessage *pNsmCreatePhyIntfMessage = dynamic_cast<NsmCreatePhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        return createPhyIntfMO(pNsmCreatePhyIntfMessage, pPrismSynchronousLinearSequencerContext);
    }

    ResourceId IntfConfLocalWorker::createPhyIntfMO(NsmCreatePhyIntfMessage *pNsmCreatePhyIntfMessage,
													PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        string ifName;
        string name;
        ResourceId ret = 0;
        ObjectId pPhyIntfObjId = ObjectId::NullObjectId;
        NsmL3ReadyManagedObject *pL3ReadyMo = NULL;

        NSM_PLUG_DBG("Entering ...");

        name = pNsmCreatePhyIntfMessage->getIfName();
        InterfaceType ifType = NsmUtils::getIntfTypeFromName(name);
        if (ifType == IF_TYPE_GI || ifType == IF_TYPE_TE ||
            ifType == IF_TYPE_FO)
        {
            name = name.substr(2);
        }

        ret = NsmUtils::getThreeTupleIfName(name, ifName, 0);

        if (WAVE_PERSISTENCE_CLUSTER_DISABLED == ret) {
            // ifName = pNsmCreatePhyIntfMessage->getIfName();
            ifName = name;
        }

        // starting here, ifName doesn't have interface type prefix.
        trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ +
            " Interface name without type prefix: "  + ifName);

        /*
         * During Persistent reboot verify if the Phy interface already exists
         * in the database. If present don't create it again.
         */
        getPhyIntfObjectIdByIfName(ifName, pPhyIntfObjId,pPrismSynchronousLinearSequencerContext);

        if (pPhyIntfObjId != ObjectId::NullObjectId) {
        	NSM_PLUG_DBG_ERR_S(string("CR_PHY_L CreatePhyIntfMO: Interface already exists ..") + ifName);

            PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject =
                NsmL3Util::PhyIfNameGetPhyIfLocalMO(ifName,
                pPrismSynchronousLinearSequencerContext);
            if (pPhyIntfLocalManagedObject != NULL) {
                pPrismSynchronousLinearSequencerContext->
                    addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);

                // Temporary Code
                int32_t slotId = NsmUtils::getSlotId(ifName);

                if ((int32_t)(pPhyIntfLocalManagedObject->getAssociatedSlot())
                    != slotId)
                {
                    pPhyIntfLocalManagedObject->associateWithSlot (slotId);
                    trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ +
                        string(" Existing IntfName: ") +
                        ifName + " Re-associate slotId=" +
                        pPhyIntfLocalManagedObject->getAssociatedSlot());
                    //updateWaveManagedObject(pPhyIntfLocalManagedObject);
                }
                ////

                if (!pPhyIntfLocalManagedObject->getSwModeFlag()) {
                    pL3ReadyMo = pPhyIntfLocalManagedObject->getL3ReadyMO();
                    if (pL3ReadyMo == NULL) {
                        pL3ReadyMo = new NsmL3ReadyManagedObject(
                            dynamic_cast<NsmLocalObjectManager *>
                            (getPWaveObjectManager()));
                         pL3ReadyMo->setInterfaceName(ifName);
                         pL3ReadyMo->setIntfType(IF_TYPE_PHY);
                        pPhyIntfLocalManagedObject->setL3Ready(const_cast<NsmL3ReadyManagedObject *>(pL3ReadyMo));
                        //updateWaveManagedObject(pPhyIntfLocalManagedObject);
                    }

                    trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ +
                        string(" existing IntfName: ") +
                        ifName + " Create L3Ready MO done!");
                }
                else {
                    trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ +
                        string(" existing IntfName: ") +
                        ifName + " switchport was set. L3 can't be enabled!");
                }
            }
            else {
                trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ +
                    " pPhyIntfLocalManagedObject == NULL");
            }
			pPhyIntfLocalManagedObject->setActive(true);
			pPhyIntfLocalManagedObject->setPortrole(false);
			updateWaveManagedObject(pPhyIntfLocalManagedObject);
        	return WAVE_MESSAGE_SUCCESS;
		} else {
            NSM_PLUG_INFO_S(string("CR_PHY_L Creating PhyInterface MO  :: ") + ifName);
        }

        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ +
            string(" new IntfName: ") + ifName + "IntfType=" + ifType);

        if (ifType == IF_TYPE_GI) {
            pPhyIntfLocalManagedObject = new OneGigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if (ifType == IF_TYPE_TE) {
            pPhyIntfLocalManagedObject = new TenGigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if (ifType == IF_TYPE_FO) {
            pPhyIntfLocalManagedObject = new FortyGigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if (ifType == IF_TYPE_PHY) {
            // Temporarily support old style, delete after hsl generates new
            // style of ifName and stop using isIslCapability
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ +
                string(" OLD STYLE - new IntfName: ") + ifName +
                "IntfType=" + ifType);
            if(pNsmCreatePhyIntfMessage->getIslCapability())
                pPhyIntfLocalManagedObject = new TenGigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
            else
                pPhyIntfLocalManagedObject = new OneGigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else {
            trace(TRACE_LEVEL_FATAL, string(__FILE__) + __LINE__ +
                string(" INVALID INTF-TYPE IntfName: ") + ifName +
                "IntfType=" + ifType);
        }

        LldpIntfConfLocalManagedObject *pLldpIntfConfLocalManagedObject = new LldpIntfConfLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));

        prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);

        pPhyIntfLocalManagedObject->setId(ifName);
		pPhyIntfLocalManagedObject->setActive(true);
        pPhyIntfLocalManagedObject->setIfName(ifName);
        pPhyIntfLocalManagedObject->setIfIndex(
        pNsmCreatePhyIntfMessage->getSwIfIndex());
        pPhyIntfLocalManagedObject->setDesc (pNsmCreatePhyIntfMessage->getDesc());
        pPhyIntfLocalManagedObject->setIslCapability(pNsmCreatePhyIntfMessage->getIslCapability());
        pPhyIntfLocalManagedObject->setLacpTimeout (LACP_TIMEOUT_DEFAULT);
		pPhyIntfLocalManagedObject->setLacpPortPriority (DCM_LACP_DEFAULT_PORT_PRIORITY);
		pPhyIntfLocalManagedObject->setLacpDefaultUp (LACP_DEFAULT_DEFAULT_UP);
        pPhyIntfLocalManagedObject->setLineSpeed (LINE_SPEED_AUTO);
 		pPhyIntfLocalManagedObject->setPortrole (pNsmCreatePhyIntfMessage->getPortrole());
        pL3ReadyMo = new NsmL3ReadyManagedObject(
            dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        pL3ReadyMo->setInterfaceName(ifName);
        pL3ReadyMo->setIntfType(IF_TYPE_PHY);
        pPhyIntfLocalManagedObject->setL3Ready(const_cast<NsmL3ReadyManagedObject *>(pL3ReadyMo));
        trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ +
            string(" new IntfName: ") +
            ifName + "Create L3Ready MO done!");

		/*
         * By default set all physical interface status as "no shutdown" in the
         * case of vcs enabled.
         */
        if( DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED)
            pPhyIntfLocalManagedObject->setStatus(false);
        else
            pPhyIntfLocalManagedObject->setStatus(true);

        pPhyIntfLocalManagedObject->setMtu(NSM_MTU_DEF_VAL);
        pPhyIntfLocalManagedObject->setOpenflow(NSM_OPENFLOW_DEF_STATUS);
        pPhyIntfLocalManagedObject->setSwModeFlag(false);
        pPhyIntfLocalManagedObject->setIsAllowedVlanAll(false);
		pLldpIntfConfLocalManagedObject->setProfileId(ObjectId::NullObjectId);
		pLldpIntfConfLocalManagedObject->setDisable(0);
		pLldpIntfConfLocalManagedObject->setDcbxVersion(LLDP_DCBX_VERSION_AUTO);
        pLldpIntfConfLocalManagedObject->setIscsiPriority(DEFAULT_ISCSI_PRIORITY);
        trace(TRACE_LEVEL_INFO,string("IntfConfLocalWo createPhyIntfMO "));
        pPhyIntfLocalManagedObject->setPvlanModePromiscuous(false);
        pPhyIntfLocalManagedObject->setPvlanModeHost(false);
        pPhyIntfLocalManagedObject->setPvlanModeTrunkBas(false);
        pPhyIntfLocalManagedObject->setPvlanModeTrunkPromis(false);
        pPhyIntfLocalManagedObject->setPvlanModeTrunkHost(false);
		pPhyIntfLocalManagedObject->setPvlanNativeVlanId(0);
		pPhyIntfLocalManagedObject->setPvlanIsAllowedVlanAll(false);
		pLldpIntfConfLocalManagedObject->setLldpCeeStatus(LLDP_CEE_OFF);
		pPhyIntfLocalManagedObject->setLldpIntfConfig (WaveManagedObjectPointer<LldpIntfConfLocalManagedObject> (pLldpIntfConfLocalManagedObject));
		pPhyIntfLocalManagedObject->setNativeVlanIntfConfig (WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (NULL));
		pPhyIntfLocalManagedObject->setCosTailDropConfig(WaveManagedObjectPointer<QosCosToTailDropManagedObject> (NULL));
        pPhyIntfLocalManagedObject->setSwMode ((swModeEnum)SW_MODE_UNKNOWN);

        pPhyIntfLocalManagedObject->setDot1xIntfConfig (WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> (NULL));
        pPhyIntfLocalManagedObject->setFcoeport(WaveManagedObjectPointer<FcoeportAttrLocalManagedObject>(NULL));


        /*Default channel-group cmd values */

        pPhyIntfLocalManagedObject->setPoId(ObjectId::NullObjectId);
        pPhyIntfLocalManagedObject->setAccessVlanObjectId(ObjectId::NullObjectId);
        pPhyIntfLocalManagedObject->setPoMode (0);
        pPhyIntfLocalManagedObject->setPoType (0);

        pPhyIntfLocalManagedObject->setIsPortProfiled(false);
		pPhyIntfLocalManagedObject->setQosCosMap(ObjectId::NullObjectId);
        pPhyIntfLocalManagedObject->setQosTrafficClassMap(ObjectId::NullObjectId);
		pPhyIntfLocalManagedObject->setCeeMap(ObjectId::NullObjectId);
        pPhyIntfLocalManagedObject->setDistance(LONG_DISTANCE_200);
		pPhyIntfLocalManagedObject->setQosDscpMap(ObjectId::NullObjectId);
        pPhyIntfLocalManagedObject->setQosDscpTrafficClassMap(ObjectId::NullObjectId);
        pPhyIntfLocalManagedObject->setQosDscpCosMap(ObjectId::NullObjectId);

        /* creating MO at init, otherwise 'no spanning-tree shut' fails */
        ifName = pPhyIntfLocalManagedObject->getIfName();
        XstpPortConfigManagedObject *pXstpPortConfigManagedObject = new XstpPortConfigManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        prismAssert (NULL != pXstpPortConfigManagedObject, __FILE__, __LINE__);

        pXstpPortConfigManagedObject->setDefaultAttribute(ifName);
        ifName = pPhyIntfLocalManagedObject->getIfName();
        //pPhyIntfLocalManagedObject->updatePortStpConfig(XstpPortConfigManagedObject);
        pPhyIntfLocalManagedObject->setPortStpConfig(WaveManagedObjectPointer<XstpPortConfigManagedObject>(pXstpPortConfigManagedObject));

        int32_t slotId = NsmUtils::getSlotId(ifName);
        pPhyIntfLocalManagedObject->associateWithSlot (slotId);

        trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ +
            string(" new IntfName: ") +
            ifName + "associate slotId=" + slotId);

        NSM_PLUG_INFO_S(string("CR_PHY_L Creating PhyInterface status MO  :: ") + ifName + string("status : ") + pNsmCreatePhyIntfMessage->getCompletionStatus ());
	if(pPhyIntfLocalManagedObject && pPrismSynchronousLinearSequencerContext)
    	pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);
	/*
	if(pLldpIntfConfLocalManagedObject && pPrismSynchronousLinearSequencerContext)
    	pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pLldpIntfConfLocalManagedObject);
    */
        return pNsmCreatePhyIntfMessage->getCompletionStatus ();
    }

    ResourceId IntfConfLocalWorker::createPhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NSM_PLUG_DBG("Entering ...");

        NsmCreatePhyIntfMessage *pNsmCreatePhyIntfMessage = dynamic_cast<NsmCreatePhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        int status = WAVE_MESSAGE_SUCCESS;

        pNsmCreatePhyIntfMessage->setCompletionStatus(status);

        NSM_PLUG_INFO_S(string("CR_PHY_L status : ") + status);

        if( status == WAVE_MESSAGE_SUCCESS)
            if(  pNsmCreatePhyIntfMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
                return  pNsmCreatePhyIntfMessage->getCompletionStatus();
        return status;

    }
    /* Code Ends Here */
ResourceId IntfConfLocalWorker::validatePhyIntfStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        NSM_PLUG_DBG("Entering ...");
        NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage = dynamic_cast<NsmUpdatePhyIntfMessage *> ( pDceSynchronousLinearSeqContext->getPPrismMessage ());

        string ifName = pNsmUpdatePhyIntfMessage->getIfName();

        /* ISL port role change case : Nsm sends two tuple name */
        if ( (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (false  == NsmUtils::isValidThreeTuple(ifName) ) ) {
                NsmUtils::getThreeTupleIfName( pNsmUpdatePhyIntfMessage->getIfName(),ifName, 0);
                pNsmUpdatePhyIntfMessage->setIfName(ifName);
        }

        // Keeping prismContext as well - else required changes in many other files
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = dynamic_cast<PrismSynchronousLinearSequencerContext *> (pDceSynchronousLinearSeqContext);

        ResourceId  status   = WAVE_MESSAGE_ERROR;
		string profileName("\0");
		FcoeProfileCfgManagedObject *pFcoeProfileCfgManagedObject = NULL;

		vector<string> selectFields;
		selectFields.push_back ("active");
		selectFields.push_back ("islCapability");
		selectFields.push_back ("Portrole");
		selectFields.push_back ("swModeFlag");
		selectFields.push_back ("isPortProfiled");
        selectFields.push_back ("associatedPortProfiles");
        selectFields.push_back ("associatedPortProfileDomain");
		selectFields.push_back ("swMode");
		selectFields.push_back ("ingressEgressMacAclPolicy");
        selectFields.push_back ("ingressEgressIpAclPolicy");
        selectFields.push_back ("inputPolicyMapId");
        selectFields.push_back ("outputPolicyMapId");
        selectFields.push_back ("nativeVlanIntfConfig");
        selectFields.push_back ("lacpDefaultUp");
        selectFields.push_back ("poMode");
        selectFields.push_back ("poType");
        selectFields.push_back ("pVlanHostPrimVlanId");
        selectFields.push_back ("pVlanModeHost");
        selectFields.push_back ("pVlanModeTrunkPromis");
        selectFields.push_back ("pVlanModeTrunkHost");
        selectFields.push_back ("pVlanModeTrunkBas");
        selectFields.push_back ("pVlanNativeVlanId");
        selectFields.push_back ("ifName");
        selectFields.push_back ("vrfid");
        selectFields.push_back ("poId");


        switch (pNsmUpdatePhyIntfMessage->getCmdCode()) {
        case ELD_INTF_CONF_ELD_ENABLE:
            selectFields.push_back ("vlanid");
            selectFields.push_back("portStpConfig");
            break;
        case NSMUPDATE_PHY_PVLAN_MODE_TRUNK_HOST:
        case NSMUPDATE_PHY_PVLAN_MODE_HOST:
        {
            selectFields.push_back("portStpConfig");
            break;
        }
        case ELD_INTF_CONF_PRIORITY:
            selectFields.push_back("isdestinationMirrorportEnable");
            selectFields.push_back("portStpConfig");

        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addSelectFields(selectFields);
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		WaveManagedObjectSynchronousQueryContext fcoesyncQueryCtxt(FcoeProfileCfgManagedObject::getClassName());
        vector<WaveManagedObject *> *pPortProfileResults = NULL;
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                status = WAVE_MESSAGE_ERROR;
            }else if (1 == numberOfResults) {
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
        }


        status = WAVE_MESSAGE_SUCCESS;

        if (pPhyIntfLocalManagedObject == NULL) {
            NSM_PLUG_DBG_ERR_S( string("UP_PHY_L Physical Interface MO not found ..:") + ifName);
            return (WAVE_MESSAGE_ERROR);
        } else {

//            updateWaveManagedObject (pPhyIntfLocalManagedObject);

            prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);

            NSM_PLUG_INFO_S( string ("UP_PHY_L Validating Physical Interface MO : ") + ifName + string("Cmdcode :") + pNsmUpdatePhyIntfMessage->getCmdCode());

			if(false == pPhyIntfLocalManagedObject->getActive())
			{

        		if(pResults)
				{
        			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				}
				return WRC_NSM_INTERFACE_NOT_IN_SERVICE;
			}

			switch(pNsmUpdatePhyIntfMessage->getCmdCode()) {
              case NSM_PLUGIN_ISL_TO_EDGE:
              case NSM_PLUGIN_EDGE_TO_ISL:
                pNsmUpdatePhyIntfMessage->setIfType(pPhyIntfLocalManagedObject->getPhyType());
            }

            /*Check ISL Port or Not */
            if ( (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pPhyIntfLocalManagedObject->getPortrole()) ) {
                switch(pNsmUpdatePhyIntfMessage->getCmdCode()) {
                    case NSMUPDATE_PHY_DESC:
                    case NSMUPDATE_PHY_DELDESC:
                    case NSMUPDATE_PHY_STATUS:
                    case NSMUPDATE_PHY_DELSTATUS:
                    case NSM_PLUGIN_ISL_TO_EDGE:
					case NSMUPDATE_PHY_SFLOW_ENABLE:
					case NSMUPDATE_PHY_DELSFLOW_ENABLE:
					case NSMUPDATE_PHY_SFLOW_POLLING_INTERVAL:
					case NSMUPDATE_PHY_DELSFLOW_POLLING_INTERVAL:
					case NSMUPDATE_PHY_SFLOW_SAMPLE_RATE:
					case NSMUPDATE_PHY_DELSFLOW_SAMPLE_RATE:
            		case NSMUPDATE_PHY_EXCEPTION_STATUS:
                    case NSMUPDATE_PHY_PORT_PROFILED: {
                        break; /* these cmds are allowed on isl link */
                    }

                    default : {
                        trace (TRACE_LEVEL_ERROR, string ("\t Interface Port Role is ISL, nothing to be done CmdCode: ")
                                + pNsmUpdatePhyIntfMessage->getCmdCode() + string(" Ifname : ") + ifName );
        				if(pResults)
						{
        					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
						}

                        return (WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR);
                    }
                }
            }

             switch(pNsmUpdatePhyIntfMessage->getCmdCode())
             {
                 case NSMUPDATE_PHY_MODE :
                 case NSMUPDATE_PHY_DELMODE:
                     {
                         /* Checking sw is enabled*/
                         if(!pPhyIntfLocalManagedObject->getSwModeFlag())
                         {
                             trace (TRACE_LEVEL_ERROR, string ("\t Interface not configured for switching "));
                             status = WRC_SWITCHING_NOT_ENABLED;
                         }
                         break;
                     }

                 case NSMUPDATE_PHY_PVLAN_MODE_TRUNK:
                 case NSMUPDATE_PHY_PVLAN_MODE_TRUNK_HOST:
                 case NSMUPDATE_PHY_PVLAN_MODE_TRUNK_PROMISCUOUS:
                 case NSMUPDATE_PHY_PVLAN_MODE_PROMISCUOUS:
                 case NSMUPDATE_PHY_PVLAN_MODE_HOST:
                 {
                	 /* Checking sw is enabled*/
                     if(!pPhyIntfLocalManagedObject->getSwModeFlag())
                     {
                         trace (TRACE_LEVEL_ERROR, string ("\t Interface not configured for switching "));
                         status = WRC_SWITCHING_NOT_ENABLED;
                     }
                     //isClassifierPresent
                     else if(PVlanUtils::isClassifierPresentOnIntf(pPhyIntfLocalManagedObject->getIfName(), false))
                     {
                         trace (TRACE_LEVEL_ERROR, string ("\t PVlanUtils::isClassifierPresentOnIntf"));
                         status = WRC_NSM_PVLAN_PMODE_CLASSIFIER_PRESENT;
                     }

                     //isSpanningTree
                     else if ((pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_PVLAN_MODE_TRUNK_HOST ||  pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_PVLAN_MODE_HOST)
              	    		 && pPhyIntfLocalManagedObject->getPortStpConfig()->getStpEnable()) {
              	         tracePrintf(TRACE_LEVEL_ERROR, "Spanning tree is enabled on %s; cannot set mode as host.", ifName.c_str());
              	         status = WRC_NSM_PVLAN_HOST_MODE_STP_PRESENT;
              	     }

                     break;
                 }

              	 case NSMUPDATE_PHY_PVLAN_HOST_ASSOCIATION :
              	 {

              		trace (TRACE_LEVEL_DEBUG, string ("\t IN NSMUPDATE_PHY_PVLAN_HOST_ASSOCIATION Validation "));
              		 UI32 primVId = pNsmUpdatePhyIntfMessage->getPvlanHostPrimVlanId();
              		 UI32 secVId = pNsmUpdatePhyIntfMessage->getPvlanHostSecVlanId();
              		 //in case of update operation corresponding to primary vlan
              		 if(primVId==0){
              			primVId = pPhyIntfLocalManagedObject->getPvlanHostPrimVlanId();
              			pNsmUpdatePhyIntfMessage->setPvlanHostPrimVlanId(primVId);
              			trace(TRACE_LEVEL_DEBUG, string("NSMUPDATE_PO_PVLAN_HOST_ASSOCIATION Validation step : Now prim vlan Id ")+ pNsmUpdatePhyIntfMessage->getPvlanHostPrimVlanId());
              		 }

              		 NSM_PLUG_DBG_S("\t IN NSMUPDATE_PHY_PVLAN_HOST_ASSOCIATION  primVid" + primVId );

                       //sw enabled
                       if(!pPhyIntfLocalManagedObject->getSwModeFlag())
                       {
                           trace (TRACE_LEVEL_ERROR, string ("Interface not configured for switching "));
                           status = WRC_SWITCHING_NOT_ENABLED;
                       }

                       //sw mode host set
                       else if(!(pPhyIntfLocalManagedObject->getPvlanModeHost() == true || pPhyIntfLocalManagedObject->getPvlanModeTrunkHost() == true))
                       {
                      	 status = WRC_PVLAN_MODE_NOT_HOST;
                      	trace (TRACE_LEVEL_ERROR, string ("IntfConfLocalW  validateVlanIntfUpdateStep : mode Host"));
                       }

                       //is Primary vlan
                       else if(!PVlanUtils::isPrimaryVlan(primVId))
                       {
              			status = WRC_NSM_PVLAN_NOT_PRIM_VLAN;
              			trace (TRACE_LEVEL_ERROR, string ("IntfConfLocalW  validateVlanIntfUpdateStep : isPrimaryVlan"));
                       }

                       //Is secondary Vlan
                       else if (!PVlanUtils::isSecondaryVlan(secVId))
                       {
           				status = WRC_NSM_PVLAN_NOT_SEC_VLAN;
           				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalW  validateVlanIntfUpdateStep : isSecondaryVlan"));
                       }

                       //doesAssociation exists
                       else if (!PVlanUtils::doesAssociationExists(primVId, secVId, pDceSynchronousLinearSeqContext))
                       {
                    	   status = WRC_PVLAN_ASSOC_NOT_PRESENT ;
                    	   trace (TRACE_LEVEL_ERROR, string ("IntfConfLocalW  validateVlanIntfUpdateStep : doesAssociationExists"));

                       }

					   //When
					   //1. The port is in pritave-vlan-trunk-host mode,
					   //2. The secondary vlan is >= 4096,
					   //3. The secondary vlan should have a ctag associated with, already.
					   //else, return ERROR.
					   else if(pPhyIntfLocalManagedObject->getPvlanModeTrunkHost() && (secVId >= MAX_DOT1Q_VLAN_ID) && (NsmUtils::getVlanClassificationMOCount(CtagVlanClassificationMO::getClassName(), secVId, "", 0, 0, pPhyIntfLocalManagedObject->getObjectId()) == 0))
					   {
							status = WRC_NSM_GVLAN_SECONDARY_HAVE_NO_CTAG;
							trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validatePhyIntfStep:Secondary Vlan is not associated with a ctag."));
					   }


              		 trace(TRACE_LEVEL_INFO,string("NSMUPDATE_PHY_PVLAN_HOST_ASSOCIATION Validation step ")+ primVId );
              		 break;
              	 }

				 case NSMUPDATE_PHY_PORT_PROFILED: 
                 case NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT:
                 case NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT:
                 {
                     //ISL Validation only for the config command and not for the unconfig commands
                     if ((DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pPhyIntfLocalManagedObject->getPortrole())) {

                         pDceSynchronousLinearSeqContext->getCache()->put(PORT_ROLE,"true");

                         if((pNsmUpdatePhyIntfMessage->getIsPortProfiled()) 
                                || (NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT == (pNsmUpdatePhyIntfMessage->getCmdCode())) 
                                || (NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT == (pNsmUpdatePhyIntfMessage->getCmdCode())) ) {
                             status= WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
                             break;
                         }
                     }


                     //L3 ACL and port profile config coexistence.
                     int isL3ACLApplied = pPhyIntfLocalManagedObject->getIngressEgressIpAclPolicyDirectioncount(SSMACL_BINDING_DIRECTION_BOTH_E);

                     if ( isL3ACLApplied == 1 ) {
                         trace(TRACE_LEVEL_DEBUG, string("L3 ACL is already applied on the interface, cannot configure the port as profiled port. "));
                         status = NSM_L3ACL_PORT_PROFILE_CONFLICT;
                         break;
                     }
                     
                     //Policer and port profile config coexistence.
                     if ( (pPhyIntfLocalManagedObject->getInputPolicyMapId() != ObjectId::NullObjectId) 
                            || (pPhyIntfLocalManagedObject->getOutputPolicyMapId() != ObjectId::NullObjectId) ) {

                         trace(TRACE_LEVEL_DEBUG, string("Service policy is already applied on the interface, cannot configure the port as profiled port. "));
                         status = NSM_POLICER_PORT_PROFILE_CONFLICT;
                         break;
                     }

                     UI32 cmdCode = pNsmUpdatePhyIntfMessage->getCmdCode();
                     string portProfileName = pNsmUpdatePhyIntfMessage->getPortProfileName();
                     string portProfileDomainName = pNsmUpdatePhyIntfMessage->getPortProfileDomainName();

                     if(NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT == cmdCode) {
                         status = APPMUtils::doesPortProfileExist(portProfileName);
                         if(WAVE_MESSAGE_SUCCESS != status) {
                             break;
                         }
                         if( !(ProvisioningUtil::isGvlanEnabled()) ) {
                             status = WRC_APPM_INTERFACE_CANNOT_BE_ASSOCIATED_WITH_PORT_PROFILE_OR_DOMAIN_WHEN_VF_IS_DISABLED;
                             break;
                         }
                     } else if (NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT == cmdCode) {
                         status = APPMUtils::doesPortProfileDomainExist(portProfileDomainName);
                         if(WAVE_MESSAGE_SUCCESS != status) {
                             break;
                         }
                         status = APPMUtils::isInterfaceAlreadyAssociatedWithAPortProfileDomain
                             (pPhyIntfLocalManagedObject);
                         if( WAVE_MESSAGE_SUCCESS != status ) {
                             break;
                         }
                         if( !(ProvisioningUtil::isGvlanEnabled()) ) {
                             status = WRC_APPM_INTERFACE_CANNOT_BE_ASSOCIATED_WITH_PORT_PROFILE_OR_DOMAIN_WHEN_VF_IS_DISABLED;
                             break;
                         }
                     }

                     if(WAVE_MESSAGE_SUCCESS != status) {
                         break;
                     } 

                     status = APPMUtils::validateMutuallyExclusivePortProfilePortModes
                         (pPhyIntfLocalManagedObject, cmdCode);
                     if(WAVE_MESSAGE_SUCCESS != status) {
                         break;
                     }

                     //Check for L3 configuration on the interface     
                     if((pNsmUpdatePhyIntfMessage->getIsPortProfiled()) || ("" != pNsmUpdatePhyIntfMessage->getPortProfileName())
                            || ("" != pNsmUpdatePhyIntfMessage->getPortProfileDomainName()))
                     {
                        NsmL3ReadyManagedObject *pL3ReadyMo =
                            NsmL3Util::PhyIfNameGetL3ReadyMO(ifName, pPrismSynchronousLinearSequencerContext);
                            
                        if (pL3ReadyMo) {
                            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pL3ReadyMo);
                            if (!(pL3ReadyMo->getCanDeleteL3ReadyMO())) {
                                status =  WRC_NSM_ERR_REMOVE_L3_CONFIG;
                                break;
                            }
                        }
                     }
                     //port-profiled-port command is not allowed on a port-channel member.
                     if((ObjectId::NullObjectId) != (pPhyIntfLocalManagedObject->getPoId())) {
                         status = WRC_NSM_ERR_PORT_PROFILE_ON_VLAG;
                         break;
                     }

					 profileName = "default";
					 fcoesyncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
					 pPortProfileResults = querySynchronously (&fcoesyncQueryCtxt);

					 if (NULL != pPortProfileResults)
					 {
						 UI32 numberOfResults = pPortProfileResults->size ();
						 if (0 == numberOfResults) {
							 trace(TRACE_LEVEL_ERROR, " for "
								 "the profile " + profileName + " fcoe-profile is not found");
						 }else if (1 == numberOfResults) {
							 pFcoeProfileCfgManagedObject = dynamic_cast<FcoeProfileCfgManagedObject *>((*pPortProfileResults)[0]);
							 if ( (pPhyIntfLocalManagedObject->getPhyType() ==  IF_TYPE_GI) && ( pFcoeProfileCfgManagedObject->getFcoePort()))
							 {
								 NSM_PLUG_DBG_ERR("Cannot configure port-profile-port on carme1G.Default profile has fcoe configuration");
								 status = WRC_NSM_ERR_DEFAULT_PROFILE_FCOE_CONFIG_CARME;
								 NSM_PLUG_DBG_S("Cannot configure channel-group on carme1G.Default profile has fcoe configuration" + FrameworkToolKit::localize(status));
							 }
							 trace(TRACE_LEVEL_DEVEL, " for "
								 " the profile " + profileName + "fcoe-profile is found");
						 } else {
							 trace(TRACE_LEVEL_ERROR, " for "
								 " the profile " + profileName + " multiple fcoe-profiles are found");
							 status = WAVE_MESSAGE_ERROR;
						 }
					 }

					 break;
					 }
				 case NSMUPDATE_PHY_SW:
                    {
                        bool isPortProfiled = false;
                        APPMUtils::isPortProfilePort(pPhyIntfLocalManagedObject, isPortProfiled);

				 		if(isPortProfiled){
						    status = WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED;
						}
                        else {
                            ObjectId vrfObjId = pPhyIntfLocalManagedObject->getVrfObjectId();
                            if (vrfObjId !=  ObjectId::NullObjectId) {
                                status = WRC_NSM_ERR_REMOVE_VRF_BIND;
                                break;
                            }
                            //Check for L3 configuration on the interface     
                            if((pNsmUpdatePhyIntfMessage->getSwModeFlag()))
                            {
                                NsmL3ReadyManagedObject *pL3ReadyMo =
                                    NsmL3Util::PhyIfNameGetL3ReadyMO(ifName, pPrismSynchronousLinearSequencerContext);

                                if (pL3ReadyMo) {
                                    if (!(pL3ReadyMo->getCanDeleteL3ReadyMO()))
                                        status =  WRC_NSM_ERR_REMOVE_L3_CONFIG;
                                }

                                if (pL3ReadyMo) {
                                	pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pL3ReadyMo);
                                }
                            }    
                        }
						break;
                    }
				/* Adding Case for MAC Acl: Port can not be made no sw if ACL applied */
				 case NSMUPDATE_PHY_DELSW:
					{
						if (pPhyIntfLocalManagedObject->getIngressEgressMacAclPolicyDirectioncount
							(SSMACL_BINDING_DIRECTION_BOTH_E)) {
							trace (TRACE_LEVEL_INFO, string ("NsmIntfConfLoclWorker:: Cannot make no swithport when mac acl is aplied"));
							status = WRC_NSM_API_ERR_IF_ACL_APPLIED;
						}
						break;
					}
                 case NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN:
                 case NSMUPDATE_PHY_SW_ACCESS_ADD_RSPANVLAN:
                     {
						UI32 accessVlanId = atoi(pNsmUpdatePhyIntfMessage->getVlanRange().toString().c_str());
						ObjectId phyObjectId = pPhyIntfLocalManagedObject->getObjectId();
                    	 NSM_PLUG_INFO_S(string("case NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN or NSMUPDATE_PHY_SW_ACCESS_ADD_RSPANVLAN"));
                        bool isPortProfiled = false;
                        APPMUtils::isPortProfilePort(pPhyIntfLocalManagedObject, isPortProfiled);

                         if(isPortProfiled){
							status = WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED;
						 }

                         /* Checking sw is enabled*/
                         else if(!pPhyIntfLocalManagedObject->getSwModeFlag())
                         {
                             trace (TRACE_LEVEL_ERROR, string ("\t Interface not configured for switching "));
                             status = WRC_SWITCHING_NOT_ENABLED;
                         }

                         /* Checking sw is access*/
                         else if(pPhyIntfLocalManagedObject->getSwMode() != SW_MODE_ACCESS)
                         {
                             trace (TRACE_LEVEL_ERROR, string ("\t Port mode not set to access"));
                             status = WRC_SW_ACCESSMODE_NOT_ENABLED;
                         }

                         else if(PVlanUtils::isPVlan(accessVlanId))
                         {
                             status = WRC_NSM_PVLAN_VLAN;
                         }
						//If there is a mac/macGroup vlanClassification configured on the same port with the same access vlan, then reject the command.
						else if((NsmUtils::getVlanClassificationMOCount(MacAddressVlanClassificationMO::getClassName(), accessVlanId, "", 0, 0, phyObjectId)) || (NsmUtils::getVlanClassificationMOCount(MacGroupVlanClassificationMO::getClassName(), accessVlanId, "", 0, 0, phyObjectId)) || (NsmUtils::getVlanClassificationMOCount(RspanMacAddressVlanClassificationMO::getClassName(), accessVlanId, "", 0, 0, phyObjectId)) || (NsmUtils::getVlanClassificationMOCount(RspanMacGroupVlanClassificationMO::getClassName(), accessVlanId, "", 0, 0, phyObjectId)))
						{
							trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validatePhyIntfStep: The access vlan id has already been used in a vlan classification."));
							status = WRC_NSM_GVLAN_VLAN_IS_USED_IN_CLASSIFICATION;
						}

                         break;
                     }
				 case NSM_PLUGIN_EDGE_TO_ISL:
                    status = WAVE_MESSAGE_SUCCESS;
                    break;
                 case NSMUPDATE_PHY_SW_TRUNK_VLAN_ALL:
                 case NSMUPDATE_PHY_SW_TRUNK_VLAN_NONE:
                 case NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN:
                 case NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN:
                 case NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN:
                 case NSMUPDATE_PHY_NATIVEVLANDISABLE:
                 case NSMUPDATE_PHY_NATIVEVLANID:
                 case NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN:
                 case NSMUPDATE_PHY_SW_TRUNK_DEL_RSPANVLAN:
                     {
                        bool isPortProfiled = false;
                        APPMUtils::isPortProfilePort(pPhyIntfLocalManagedObject, isPortProfiled);

                         if(isPortProfiled){
							status = WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED;
							break;
					     }
                         /* Checking sw is enabled*/
                         if(!pPhyIntfLocalManagedObject->getSwModeFlag())
                         {
                             trace (TRACE_LEVEL_ERROR, string ("\t Interface not configured for switching "));
                             status = WRC_SWITCHING_NOT_ENABLED;
                         }

                         /* Checking sw is trunk */
                         else if(!(pPhyIntfLocalManagedObject->getSwMode() == SW_MODE_TRUNK  || pPhyIntfLocalManagedObject->getPvlanModeTrunkPromis() == true))
                         {
                             trace (TRACE_LEVEL_ERROR, string ("\t Port mode not set to Trunk or  trunk promiscuous"));
                             status = WRC_SW_TRUNKMODE_NOT_ENABLED;
                         }
						 else if ((pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN) || (pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN) || (pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_TRUNK_DEL_RSPANVLAN))
						{
							NSM_PLUG_INFO_S(string("Case - delete/except trunk vlan or delete rspan trunk vlan"));
							UI32Range vlanAddRange = pNsmUpdatePhyIntfMessage->getVlanRange();
							if (NsmUtils::isNativeVlanPresentInList(vlanAddRange, pPhyIntfLocalManagedObject, NULL))
							{
								NSM_PLUG_DBG_ERR(" Can't remove Native vlan");
								status = WRC_NSM_API_ERR_DEL_NATIVE_VLAN;
							}
                         }
						 else if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_NATIVEVLANID) {
							 UI32 nativeVlanId = pNsmUpdatePhyIntfMessage->getNativeVlanId();
							 NSM_PLUG_DBG_S(string("Validate whether the configured native vlandi ")+nativeVlanId+(" is a remote-span vlan"));
							 if(NsmUtils::isRemoteSpanVlan(nativeVlanId)) {
								 NSM_PLUG_DBG_ERR_S(string("RSPAN Vlan cannot be configured as the Native Vlan"));
								 status = WRC_SPAN_NATIVE_VLAN_CANNOT_BE_RSPAN_VLAN;
							 }
							 else if(pPhyIntfLocalManagedObject->getSwMode() == SW_MODE_TRUNK && true == PVlanUtils::isPVlan(nativeVlanId))
							 {
                        		 trace(TRACE_LEVEL_ERROR, string("\t SW_MODE_TRUNK --Native vlan configuration not allowed for private-vlan "));
                        		 status = WRC_NSM_PVLAN_AS_NATIVE_VLAN;
							 }
                        	 else if(pPhyIntfLocalManagedObject->getPvlanModeTrunkPromis() == true && PVlanUtils::isSecondaryVlan(nativeVlanId))
                        	 {
                        		 trace(TRACE_LEVEL_ERROR, string("\t Native vlan configuration not allowed for secondary private-vlan "));
                        		 status = WRC_NSM_PVLAN_NATIVE_SEC;
                        	 }
							else if(NsmUtils::getVlanClassificationMOCount(0, nativeVlanId, pPhyIntfLocalManagedObject->getObjectId()))
							{
								trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validatePhyIntfStep:Native vlan id has already been configured as ctag on this port."));
								status = WRC_NSM_VLAN_ERR_VLAN_USED_AS_CTAG;
							}
						 }

                         else if ((pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN || pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN))
                         {
                        	 NSM_PLUG_INFO_S(string("Case - add trunk vlan or add rspan trunk vlan"));
                        	 UI32Range vlanAddRange = pNsmUpdatePhyIntfMessage->getVlanRange();

                        	 //Is secondary Vlan present TODO primary vlan
                        	 if (PVlanUtils::isSecondaryVlanRange(vlanAddRange) || PVlanUtils::isPrimaryVlanRange(vlanAddRange))
                        	 {
                        		 status = WRC_NSM_PVLAN_VLAN;
                        		 trace(TRACE_LEVEL_ERROR,string("NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN -- private vlan not allowed"));
                        	 }
							 else if(NsmUtils::getVlanClassificationMOCount(vlanAddRange, pPhyIntfLocalManagedObject->getObjectId()))
							 {
								trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validatePhyIntfStep:One of the vlans has been configured as ctag on this port."));

								status = WRC_NSM_GVLAN_ONE_OF_THE_VLANS_IS_A_CTAG;
							 }

	                     } else if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_TRUNK_VLAN_ALL)
						 {
							if(NsmUtils::getVlanClassificationMOCount(0, 0, pPhyIntfLocalManagedObject->getObjectId()))
							{
								trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validatePhyIntfStep:Vlan Classification on this port exists."));
								status = WRC_NSM_GVLAN_CONFIGURATION_EXISTS;
							}
						 }

                         break;
                     }

                 case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ALL:
             	 case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_NONE:
             	 case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ADD:
              	 case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_REMOVE:
              	 case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_EXCEPT:
              	 case NSMUPDATE_PHY_PVLAN_NATIVE_VLANID:
              	 {

              		 /* Checking sw is enabled*/
              		 if (!pPhyIntfLocalManagedObject->getSwModeFlag()) {
              			 trace(TRACE_LEVEL_ERROR,
              					 string("\t Interface not configured for switching "));
              			 status = WRC_SWITCHING_NOT_ENABLED;
              		 }

              		else if (!(pPhyIntfLocalManagedObject->getPvlanModeTrunkBas()))
              		{
                		 trace(TRACE_LEVEL_ERROR, string("\t Port mode not set to Pvlan Trunk"));
                		 status = WRC_PVLAN_MODE_NOT_TRUNK;
              		}

                     //native Vlan Id check
             		else if((pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_REMOVE) ||
                            (pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_EXCEPT))
             		{
                        if ( PVlanUtils::isNativeVlanPresentInList(pNsmUpdatePhyIntfMessage->getPvlanRange(), pPhyIntfLocalManagedObject->getPvlanNativeVlanId())){
                                NSM_PLUG_DBG_ERR(" Can't remove private Native vlan");
                                status = WRC_NSM_PVLAN_ERR_DEL_NATIVE_VLAN;
                        }
             		}


                    else if ((pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ADD))
                    {
                     	 UI32Range vlanAddRange = pNsmUpdatePhyIntfMessage->getPvlanRange();

                     	 if (PVlanUtils::isSecondaryVlanRange(vlanAddRange) || PVlanUtils::isPrimaryVlanRange(vlanAddRange))
                     	 {
                     		 status = WRC_NSM_PVLAN_VLAN;
                     		 trace(TRACE_LEVEL_ERROR,string("NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ADD -- private vlan not allowed"));
                     	 }
						 else if(NsmUtils::getVlanClassificationMOCount(vlanAddRange, pPhyIntfLocalManagedObject->getObjectId()))
						 {
							trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validatePhyIntfStep:One of the vlans has been configured as ctag on this port."));

							status = WRC_NSM_GVLAN_ONE_OF_THE_VLANS_IS_A_CTAG;
						 }
                    }
					else if((pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_PVLAN_NATIVE_VLANID) && (NsmUtils::getVlanClassificationMOCount(0, pNsmUpdatePhyIntfMessage->getPvlanNativeVlanId(), pPhyIntfLocalManagedObject->getObjectId())))
					{
						trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validatePhyIntfStep:Native vlan id has already been configured as ctag on this port."));
						status = WRC_NSM_VLAN_ERR_VLAN_USED_AS_CTAG;
					}
					else if((pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ALL) && (NsmUtils::getVlanClassificationMOCount(0, 0, pPhyIntfLocalManagedObject->getObjectId())))
					{
						trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validatePhyIntfStep:Vlan Classification on this port exists."));
						status = WRC_NSM_GVLAN_CONFIGURATION_EXISTS;
					}

              		break;
              	 }

              	 case ELD_INTF_CONF_ELD_ENABLE: 
                 {
                    if (pPhyIntfLocalManagedObject->getPortStpConfig()->getStpEnable()) {
                        tracePrintf(TRACE_LEVEL_ERROR, "Spanning tree is enabled on %s; cannot enable ELD.", ifName.c_str());
                        status = WRC_NSM_ELD_STP_ENABLE_ERROR;
                    }
                     else if (( pPhyIntfLocalManagedObject->getVlanVector()).size() > 0 )
                     {
                          trace(TRACE_LEVEL_ERROR, string("ENABLE:ELD can be configured on a single vlan .."));
                          status =  WRC_ELD_SUPPORTED_ON_SINGLE_VLAN;
                     }
                     else if(true == PVlanUtils::isPrivateVlanMode(ifName))
                     {
                          tracePrintf(TRACE_LEVEL_ERROR, "ELD configuration not allowed as interface %s is in private-vlan mode.", ifName.c_str());
                          status = WRC_NSM_PVLAN_ELD_PMODE_PRESENT;
                     }
                 }

                    break;
                 case ELD_INTF_CONF_PRIORITY:
                     trace(TRACE_LEVEL_DEBUG,"eld priority");
              	     if (pPhyIntfLocalManagedObject->getPortStpConfig()->getStpEnable()) {
              	         tracePrintf(TRACE_LEVEL_ERROR, "Spanning tree is enabled on %s; cannot enable ELD priority.", ifName.c_str());
              	         status = WRC_NSM_ELD_STP_ENABLE_ERROR;
                         break;
              	     }

              	     else if(true == PVlanUtils::isPrivateVlanMode(ifName))
              	     {
              	    	tracePrintf(TRACE_LEVEL_ERROR, "ELD configuration not allowed as interface %s is in private-vlan mode.", ifName.c_str());
						status = WRC_NSM_PVLAN_ELD_PMODE_PRESENT;
              	     }

 	                 else if (pPhyIntfLocalManagedObject->getDestinationmirrorportEnable())
                        {
              	         tracePrintf(TRACE_LEVEL_ERROR, "Interface  %s is a mirror port; cannot enable ELD priority.", ifName.c_str());
                                status = WRC_NSM_INTERFACE_IS_DESTINATION_MIRROR_PORT;
                        }
                       else 
              	         tracePrintf(TRACE_LEVEL_DEBUG, "Interface  %s is not a mirror port;", ifName.c_str());


              	     break;

                 case NSMUPDATE_PHY_TIMEOUT:
                 {
                	 UI32 mode = pPhyIntfLocalManagedObject->getPoMode();
                	 if (!(mode == NSM_PO_MODE_ACTIVE || mode == NSM_PO_MODE_PASSIVE)) {
                		 status = WRC_LACP_UNABLE_TO_SET_TIMEOUT;
                	 }
                	 break;
                 }
                 case NSMUPDATE_PHY_PORT_PRIORITY:
                 {
                	 UI32 mode = pPhyIntfLocalManagedObject->getPoMode();
                	 if (!(mode == NSM_PO_MODE_ACTIVE || mode == NSM_PO_MODE_PASSIVE)) {
                		 status = WRC_LACP_UNABLE_TO_SET_PORT_PRIORITY;
                	 }
                	 break;
                 }
                 case NSMUPDATE_PHY_LACP_DEFAULT_UP:
                 {
                	 UI32 mode = pPhyIntfLocalManagedObject->getPoMode();
                     if(!(mode==NSM_PO_MODE_ACTIVE || mode==NSM_PO_MODE_PASSIVE)){
                		 status = WRC_LACP_UNABLE_TO_SET_DEFAULT_UP_MODE;
                	 }else if(pPhyIntfLocalManagedObject->getPoType()!=NSM_PO_TYPE_STANDARD){
                		 status = WRC_LACP_UNABLE_TO_SET_DEFAULT_UP_TYPE;
                	 }
                	 break;
                 }
				case NSMUPDATE_PHY_SET_IPV6_RA_GUARD:
				{
					if(!(Capabilities::instance()->isCapabilityPresent(IPV6_RA_GUARD)))
					{
						status = WRC_NSM_IPV6_RA_GUARD_NOT_SUPPORTED;
					} else if(!pPhyIntfLocalManagedObject->getSwModeFlag())
					{
						status = WRC_SWITCHING_NOT_ENABLED;
					}

					break;
				}
				case NSMUPDATE_PHY_RESET_IPV6_RA_GUARD:
				{
					if(!(Capabilities::instance()->isCapabilityPresent(IPV6_RA_GUARD)))
					{
						status = WRC_NSM_IPV6_RA_GUARD_NOT_SUPPORTED;
					}
					break;
				}

             	 default:
             		 break;
             }
        }
        NSM_PLUG_INFO_S( string ("UP_PHY_L Validation Physical Interface MO Status : ") + ifName + string("Cmdcode :") + pNsmUpdatePhyIntfMessage->getCmdCode() + string("status :") + status);
        if(pResults)
        	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        if(pPortProfileResults)
        	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pPortProfileResults);
        pNsmUpdatePhyIntfMessage->setCompletionStatus(status);
        return status;
    }

/* This API can be used to remove native vlan id from allowed vlan list in the MO
 * while configuring Native vlan.
 */

int IntfConfLocalWorker::deleteNativeVlanFromMoVlanList (NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage,
													PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
{
		bool isVlanIdPresent = false;
		UI32 nativeVlanId = pNsmUpdatePhyIntfMessage->getNativeVlanId();

		vector <UI32> vlanRangeVector;
		UI32Range vlanAddrange = UI32Range("");

		vlanAddrange = pPhyIntfLocalManagedObject->getVlanRangeAdd();
		vlanAddrange.getUI32RangeVector(vlanRangeVector);
		vlanAddrange = UI32Range("");

		vector<UI32>::iterator moiter;

		for (moiter=vlanRangeVector.begin(); moiter != vlanRangeVector.end(); moiter++) {
			if(*moiter == nativeVlanId) {
				/*Vlan Id exists, remove from vlan list*/
				trace (TRACE_LEVEL_INFO, string ("\t Removed from vlanAdd list:: ") +
																			nativeVlanId);
				vlanRangeVector.erase(moiter);
				isVlanIdPresent = true;
				break;
			}
		}

		string vlanRangeStr = UI32Range::getUI32RangeStringFromVector(vlanRangeVector);
		vlanAddrange.fromString(vlanRangeStr);
		pPhyIntfLocalManagedObject->setVlanRangeAdd(vlanAddrange);
		pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeAdd");

		if (!isVlanIdPresent) {
			/* Remove vlan range */
			UI32Range vlanRemoverange = UI32Range("");
			vlanRemoverange = pPhyIntfLocalManagedObject->getVlanRangeRemove();
			vlanRemoverange.getUI32RangeVector(vlanRangeVector);
			vlanRemoverange = UI32Range("");

			for (moiter=vlanRangeVector.begin(); moiter != vlanRangeVector.end(); moiter++) {
				if(*moiter == nativeVlanId) {
					/*Vlan Id exists, remove from vlan list*/
					trace (TRACE_LEVEL_INFO, string ("\t Removed from vlanRemove list:: ") +
																				nativeVlanId);
					vlanRangeVector.erase(moiter);
					isVlanIdPresent = true;
					break;
				}
			}

			vlanRangeStr = UI32Range::getUI32RangeStringFromVector(vlanRangeVector);
			vlanRemoverange.fromString(vlanRangeStr);
			pPhyIntfLocalManagedObject->setVlanRangeRemove(vlanRemoverange);
			pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
		}


		if (!isVlanIdPresent) {
			/* Except vlan range */
			UI32Range vlanExceptrange = UI32Range("");
			vlanExceptrange = pPhyIntfLocalManagedObject->getVlanRangeExcept();
			vlanExceptrange.getUI32RangeVector(vlanRangeVector);
			vlanExceptrange = UI32Range("");

			for (moiter=vlanRangeVector.begin(); moiter != vlanRangeVector.end(); moiter++)	{
				if(*moiter == nativeVlanId) {
					/*Vlan Id exists, remove from vlan list*/
					trace (TRACE_LEVEL_INFO, string ("\t Removed from vlanExcept list:: ") +
																				nativeVlanId);
					vlanRangeVector.erase(moiter);
					isVlanIdPresent = true;
					break;
				}
			}
			vlanRangeStr = UI32Range::getUI32RangeStringFromVector(vlanRangeVector);
			vlanExceptrange.fromString(vlanRangeStr);
			pPhyIntfLocalManagedObject->setVlanRangeExcept(vlanExceptrange);
			pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");
		}

		if (isVlanIdPresent)
			return 1;
		return 0;

}

/* This API can be used to remove native vlan id from user input allowed vlan list */
int IntfConfLocalWorker::deleteNativeVlanFromMsgVlanList (NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage,
												PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
{
		bool isVlanIdPresent = false;
		vector<UI32>::iterator msgiter;

		trace (TRACE_LEVEL_INFO, string ("\tdeleteNativeVlanFromVlanList"));

		WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanIntfConfLocalManagedObject;
        pNativeVlanIntfConfLocalManagedObject = WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (
													pPhyIntfLocalManagedObject->getNativeVlanIntfConfig());

		/* return as the Native Vlan Configuration doesn't exist */
		if(pNativeVlanIntfConfLocalManagedObject.operator->() == NULL) {
			return 0;
		}

		UI32 nativeVlanId = pNativeVlanIntfConfLocalManagedObject->getNativeVlanId();

		string vlanRangeStr;

		vector <UI32> vlanRangeVector;

		switch (pNsmUpdatePhyIntfMessage->getCmdCode())
		{
			case NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN:
			{
				UI32Range vlanAddrange = UI32Range("");

				vlanAddrange = pNsmUpdatePhyIntfMessage->getVlanRange();
				vlanAddrange.getUI32RangeVector(vlanRangeVector);
				vlanAddrange = UI32Range("");

				for (msgiter=vlanRangeVector.begin(); msgiter != vlanRangeVector.end(); msgiter++) {
					if(*msgiter == nativeVlanId) {
						/*Vlan Id exists, remove from vlan list*/
						trace (TRACE_LEVEL_INFO, string ("\t Native vlan present in vlanAdd list:: "
											"Hence removing from vlanAdd list vlad id ") +
																					nativeVlanId);
						vlanRangeVector.erase(msgiter);
						isVlanIdPresent = true;
						break;
					}
				}
				vlanRangeStr = UI32Range::getUI32RangeStringFromVector(vlanRangeVector);
				vlanAddrange.fromString(vlanRangeStr);
				pNsmUpdatePhyIntfMessage->setVlanRange(vlanAddrange);
			}
			break;

			case NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN:
			case NSMUPDATE_PHY_SW_TRUNK_DEL_RSPANVLAN:
			{
				NSM_PLUG_INFO_S(string("case delete trunk vlan or delete rspan trunk vlan"));
				/* Remove vlan range */
				UI32Range vlanRemoverange = UI32Range("");

				vlanRemoverange = pNsmUpdatePhyIntfMessage->getVlanRange();
				vlanRemoverange.getUI32RangeVector(vlanRangeVector);
				vlanRemoverange = UI32Range("");

				for (msgiter=vlanRangeVector.begin(); msgiter != vlanRangeVector.end(); msgiter++) {
					if(*msgiter == nativeVlanId) {
						/*Vlan Id exists, remove from vlan list*/
						trace (TRACE_LEVEL_INFO, string ("\t Native vlan present in vlanRemove list:: "
											"Hence removing from vlanremove list vlad id ") +
																					nativeVlanId);
						vlanRangeVector.erase(msgiter);
						isVlanIdPresent = true;
						break;
					}
				}

				vlanRangeStr = UI32Range::getUI32RangeStringFromVector(vlanRangeVector);
				vlanRemoverange.fromString(vlanRangeStr);
				pNsmUpdatePhyIntfMessage->setVlanRange(vlanRemoverange);
			}
			break;

			case NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN:
			{
				/* Except vlan range */
				UI32Range vlanExceptrange = UI32Range("");
				vlanExceptrange = pNsmUpdatePhyIntfMessage->getVlanRange();
				vlanExceptrange.getUI32RangeVector(vlanRangeVector);
				vlanExceptrange = UI32Range("");

				for (msgiter=vlanRangeVector.begin(); msgiter != vlanRangeVector.end(); msgiter++)	{
					if(*msgiter == nativeVlanId) {
						/*Vlan Id exists, remove from vlan list*/
						trace (TRACE_LEVEL_INFO, string ("\t Native vlan present in vlanExcept list:: "
											"Hence removing from vlanremove list vlad id ") +
																						nativeVlanId);
						vlanRangeVector.erase(msgiter);
						isVlanIdPresent = true;
						break;
					}
				}

				vlanRangeStr = UI32Range::getUI32RangeStringFromVector(vlanRangeVector);
				vlanExceptrange.fromString(vlanRangeStr);
				pNsmUpdatePhyIntfMessage->setVlanRange(vlanExceptrange);
			}
			break;

			default :
			{
				trace (TRACE_LEVEL_INFO, string ("\t Unknown command code :: ") +
								pNsmUpdatePhyIntfMessage->getCmdCode());
			}
			break;
		}

		if (isVlanIdPresent)
			return 1;
		return 0;
}
    NativeVlanIntfConfLocalManagedObject*
    IntfConfLocalWorker::NativeVlanPhyGetChildMoByParentMo(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        trace (TRACE_LEVEL_DEBUG, "IntfConfLocalWorker::NativeVlanPhyGetChildMoByParentMo: Entered....");

        NativeVlanIntfConfLocalManagedObject *pNativeVlanIntfConfLocalManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NativeVlanIntfConfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeObjectId (pPhyIntfLocalManagedObject->getObjectId(), "ownerManagedObjectId"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (numberOfResults == 0 || numberOfResults > 1) {
                trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
                pNativeVlanIntfConfLocalManagedObject = NULL;
            } else {
                pNativeVlanIntfConfLocalManagedObject = dynamic_cast<NativeVlanIntfConfLocalManagedObject*>((*pResults)[0]);
            }
            pResults->clear ();
            delete pResults;
        }
        return pNativeVlanIntfConfLocalManagedObject;
    }

ResourceId IntfConfLocalWorker::getPhyIntfLocalMOByCmdCode(string PhyName, UI32 cmdCode, PhyIntfLocalManagedObject* &pPhyIntfLocalManagedObject, DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        NSM_PLUG_DBG("Entering ...getPhyIntfLocalMOByCmdCode");
        //LocationId                          locationId = 0;
        //locationId = FrameworkToolKit::getThisLocationId ();
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&PhyName,"ifName"));

        vector<string> selectFields;
        selectFields.push_back ("ifName");

        switch(cmdCode)
        {
            case NSMUPDATE_PHY_DESC :
            case NSMUPDATE_PHY_DELDESC :
                selectFields.push_back ("description");
                break;
            case NSM_PLUGIN_EDGE_TO_ISL:
                selectFields.push_back ("Portrole");
                selectFields.push_back ("status");
                break;
            case NSM_PLUGIN_ISL_TO_EDGE:
                selectFields.push_back ("Portrole");
                selectFields.push_back ("status");
                break;
            case NSMUPDATE_PHY_MTU :
            case NSMUPDATE_PHY_DELMTU :
                selectFields.push_back ("mtu");
                break;
            case NSMUPDATE_PHY_OPENFLOW :
            case NSMUPDATE_PHY_DELOPENFLOW :
                selectFields.push_back ("openflow_enable");
                break;
            case NSMUPDATE_PHY_VEPA :
                selectFields.push_back ("vepa_enable");
                break;
            case ELD_INTF_CONF_PRIORITY:
                selectFields.push_back ("eldprio");
                break;
            case ELD_INTF_CONF_ELD_ENABLE:
                selectFields.push_back ("vlanid");
                break;
            case ELD_INTF_CONF_ELD_DISABLE:
                selectFields.push_back ("vlanid");
                break;
            case NSMUPDATE_PHY_SW :
                selectFields.push_back ("swModeFlag");
                selectFields.push_back ("swBasic");
                selectFields.push_back ("ifName");
                selectFields.push_back ("vlanObjectId");
                selectFields.push_back ("swMode");
                selectFields.push_back ("vlanRangeAdd");
                selectFields.push_back ("vlanRangeRemove");
				selectFields.push_back ("vlanRangeExcept");
                selectFields.push_back ("rspan_trunkvlanrange");
                selectFields.push_back ("rspan_accessvlan");
                break;
            case NSMUPDATE_PHY_DELSW :
                selectFields.push_back ("swModeFlag");
                selectFields.push_back ("swBasic");
                selectFields.push_back ("ifName");
                selectFields.push_back ("phyType");
                selectFields.push_back ("vlanClassifierActivate");
                selectFields.push_back ("swMode");
                selectFields.push_back ("isAllowedVlanAll");
                selectFields.push_back ("nativeVlanIntfConfig");
                selectFields.push_back ("vlanObjectId");
                selectFields.push_back ("vlanRangeAdd");
                selectFields.push_back ("vlanRangeRemove");
                selectFields.push_back ("vlanRangeExcept");
                selectFields.push_back ("pVlanTrunkAssoc");
                selectFields.push_back ("pVlanMapping");
                selectFields.push_back ("eldprio");
                selectFields.push_back ("vlanid");
                selectFields.push_back ("rspan_trunkvlanrange");
                selectFields.push_back ("rspan_accessvlan");
                selectFields.push_back ("portSecurity");
                selectFields.push_back ("macAddressVlanClassification");
                selectFields.push_back ("macGroupVlanClassification");
                selectFields.push_back ("ctagVlanClassification");
                selectFields.push_back ("ctagPVlanClassification");
                selectFields.push_back ("rspan_macaddress_vlan_classififcation");
                selectFields.push_back ("rspan_macgroup_vlan_classififcation");
                selectFields.push_back ("rspan_gvlan_trunk");
                break;
            case NSMUPDATE_PHY_MODE :
            case NSMUPDATE_PHY_DELMODE:
                selectFields.push_back ("swMode");
                selectFields.push_back ("vlanObjectId");
                selectFields.push_back ("isAllowedVlanAll");
                selectFields.push_back ("vlanRangeAdd");
                selectFields.push_back ("vlanRangeRemove");
                selectFields.push_back ("vlanRangeExcept");
                selectFields.push_back ("nativeVlanIntfConfig");
                selectFields.push_back ("pVlanTrunkAssoc");
                selectFields.push_back ("pVlanMapping");
                selectFields.push_back ("objectId");
                selectFields.push_back ("eldprio");
                selectFields.push_back ("vlanid");
                selectFields.push_back ("portVlanXstp");
                selectFields.push_back ("portInstanceXstp");
                selectFields.push_back ("rspan_trunkvlanrange");
                selectFields.push_back ("rspan_accessvlan");
                selectFields.push_back ("macAddressVlanClassification");
                selectFields.push_back ("macGroupVlanClassification");
                selectFields.push_back ("ctagVlanClassification");
                selectFields.push_back ("ctagPVlanClassification");
                selectFields.push_back ("rspan_macaddress_vlan_classififcation");
                selectFields.push_back ("rspan_macgroup_vlan_classififcation");
                selectFields.push_back ("rspan_gvlan_trunk");
                break;
			 case NSMUPDATE_PHY_PVLAN_MODE_TRUNK:
			 case NSMUPDATE_PHY_PVLAN_MODE_TRUNK_HOST:
			 case NSMUPDATE_PHY_PVLAN_MODE_TRUNK_PROMISCUOUS:
			 case NSMUPDATE_PHY_PVLAN_MODE_PROMISCUOUS:
			 case NSMUPDATE_PHY_PVLAN_MODE_HOST:
	            selectFields.push_back ("pVlanTrunkAssoc");
                selectFields.push_back ("pVlanMapping");
				selectFields.push_back ("nativeVlanIntfConfig");
                selectFields.push_back ("macAddressVlanClassification");
                selectFields.push_back ("macGroupVlanClassification");
                selectFields.push_back ("ctagVlanClassification");
                selectFields.push_back ("ctagPVlanClassification");
                selectFields.push_back ("rspan_macaddress_vlan_classififcation");
                selectFields.push_back ("rspan_macgroup_vlan_classififcation");
                selectFields.push_back ("rspan_gvlan_trunk");
                break;
            case NSMUPDATE_PHY_STATUS :
            case NSMUPDATE_PHY_DELSTATUS :
                selectFields.push_back ("status");
                     break;
            case NSMUPDATE_PHY_ACTVIVE_STATUS:
            case NSMUPDATE_PHY_INACTVIVE_STATUS:
                selectFields.push_back ("active");
                     break;
            case NSMUPDATE_PHY_TIMEOUT :
                selectFields.push_back ("lacpTimeout");
                     break;
            case NSMUPDATE_PHY_PORT_PRIORITY :
                selectFields.push_back ("lacpPortPriority");
                break;
            case NSMUPDATE_PHY_LACP_DEFAULT_UP :
                selectFields.push_back ("lacpDefaultUp");
                break;
            case NSMUPDATE_PHY_LINESPEED :
                selectFields.push_back ("lineSpeed");
                     break;
			case NSMUPDATE_PHY_NATIVEVLANDISABLE :
				selectFields.push_back ("nativeVlanIntfConfig");
                selectFields.push_back ("vlanRangeAdd");
                selectFields.push_back ("vlanRangeRemove");
                selectFields.push_back ("vlanRangeExcept");
                selectFields.push_back ("objectId");		 
                break;
            case NSMUPDATE_PHY_NATIVEVLANID :
				selectFields.push_back ("nativeVlanIntfConfig");
                selectFields.push_back ("vlanRangeAdd");
				selectFields.push_back ("vlanRangeRemove");
				selectFields.push_back ("vlanRangeExcept");
                selectFields.push_back ("objectId");
                selectFields.push_back ("eldprio");
                selectFields.push_back ("vlanid");
                selectFields.push_back ("isAllowedVlanAll");
                selectFields.push_back ("pVlanModeTrunkPromis");
                break;
            case NSMUPDATE_PHY_PORT_PROFILED:
                {
                selectFields.push_back ("isPortProfiled");
                selectFields.push_back ("status");
                selectFields.push_back ("fcoeport");
                break;
                }
            case NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT: 
            case NSMUPDATE_DISSOCIATE_PORT_PROFILE_WITH_PHY_PORT:
                {
                    selectFields.push_back ("status");
                    selectFields.push_back ("fcoeport");
                    selectFields.push_back ("associatedPortProfiles");
                    break;
                }
            case NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT:
            case NSMUPDATE_DISSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT:
                {
                    selectFields.push_back ("status");
                    selectFields.push_back ("fcoeport");
                    selectFields.push_back ("associatedPortProfileDomain");
                    break;
                }
            case NSMUPDATE_PHY_SFLOW_ENABLE :
            case NSMUPDATE_PHY_DELSFLOW_ENABLE :
                selectFields.push_back ("isSampleRateSet");
                selectFields.push_back ("sampleRate");
                selectFields.push_back ("isPollingIntervalSet");
                selectFields.push_back ("pollingInterval");
                selectFields.push_back ("enable");
                break;
            case NSMUPDATE_PHY_SFLOW_POLLING_INTERVAL :
            case NSMUPDATE_PHY_DELSFLOW_POLLING_INTERVAL :
                selectFields.push_back ("isPollingIntervalSet");
                selectFields.push_back ("pollingInterval");
                break;
            case NSMUPDATE_PHY_SFLOW_SAMPLE_RATE :
            case NSMUPDATE_PHY_DELSFLOW_SAMPLE_RATE :
                selectFields.push_back ("isSampleRateSet");
                selectFields.push_back ("sampleRate");
                break;
            case NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN:
            case NSMUPDATE_PHY_SW_DELETE_ACCESS_VLAN:
            case NSMUPDATE_PHY_SW_ACCESS_ADD_RSPANVLAN:
            case NSMUPDATE_PHY_SW_ACCESS_DEL_RSPANVLAN:
                selectFields.push_back ("vlanObjectId");
                selectFields.push_back ("portVlanXstp");
                selectFields.push_back ("portInstanceXstp");
                selectFields.push_back ("eldprio");
                selectFields.push_back ("vlanid");
                selectFields.push_back ("rspan_accessvlan");
                break;
            case NSMUPDATE_PHY_SW_TRUNK_VLAN_ALL:
                selectFields.push_back ("isAllowedVlanAll");
                selectFields.push_back ("isAllowedVlanNone");
                selectFields.push_back ("vlanRangeAdd");
                selectFields.push_back ("vlanRangeRemove");
                selectFields.push_back ("vlanRangeExcept");
                break;
            case NSMUPDATE_PHY_SW_TRUNK_VLAN_NONE:
                selectFields.push_back ("isAllowedVlanAll");
                selectFields.push_back ("isAllowedVlanNone");
                selectFields.push_back ("vlanRangeAdd");
                selectFields.push_back ("vlanRangeRemove");
                selectFields.push_back ("vlanRangeExcept");
                selectFields.push_back ("portVlanXstp");
                selectFields.push_back ("portInstanceXstp");
                selectFields.push_back ("nativeVlanIntfConfig");
                selectFields.push_back ("vlanid");
                break;
			case NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN:
            case NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN:
            case NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN:
            case NSMUPDATE_PHY_SW_TRUNK_DEL_RSPANVLAN:
            case NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN:
                selectFields.push_back ("nativeVlanIntfConfig");
                selectFields.push_back ("isAllowedVlanAll");
                selectFields.push_back ("vlanRangeRemove");
                selectFields.push_back ("vlanRangeAdd");
                selectFields.push_back ("vlanRangeExcept");
                selectFields.push_back ("isAllowedVlanNone");
                selectFields.push_back ("portVlanXstp");
                selectFields.push_back ("portInstanceXstp");
                selectFields.push_back ("eldprio");
                selectFields.push_back ("vlanid");
                selectFields.push_back ("rspan_trunkvlanrange");
                break;

			case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ADD:
                selectFields.push_back ("pVlanIsAllowedVlanAll");
                selectFields.push_back ("pVlanRangeAdd");
                selectFields.push_back ("pVlanRangeRemove");
                break;

            case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_REMOVE:
                selectFields.push_back ("pVlanIsAllowedVlanAll");
                selectFields.push_back ("pVlanRangeAdd");
                selectFields.push_back ("pVlanRangeRemove");
                selectFields.push_back ("portVlanXstp");
                selectFields.push_back ("portInstanceXstp");
                selectFields.push_back ("isAllowedVlanAll");
                selectFields.push_back ("vlanRangeAdd");
                selectFields.push_back ("vlanRangeRemove");
                break;

            case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_EXCEPT:
                selectFields.push_back ("portVlanXstp");
                selectFields.push_back ("portInstanceXstp");
                selectFields.push_back ("isAllowedVlanAll");
                selectFields.push_back ("vlanRangeAdd");
                selectFields.push_back ("vlanRangeRemove");
                break;

        }

        syncQueryCtxt.addSelectFields(selectFields);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (!selectFields.empty())
          selectFields.clear();
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                status = WAVE_MESSAGE_ERROR;
            }else if (1 == numberOfResults) {
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
        }
        if(pResults)
             {
                 if(pDceSynchronousLinearSeqContext)
                    pDceSynchronousLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
                 pResults->clear();
                 delete pResults;
             }
    return status;
    }


ResourceId IntfConfLocalWorker::updatePhyIntfMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        NSM_PLUG_DBG("Entering ...");
        NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage = dynamic_cast<NsmUpdatePhyIntfMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());
		return updatePhyIntfMO(pNsmUpdatePhyIntfMessage, pDceSynchronousLinearSeqContext);
	}

	ResourceId IntfConfLocalWorker::updatePhyIntfMO(NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage,
													DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
		NSM_PLUG_DBG("Entering ...");

		UI32 j;
		char buf[6] = {0};
		string vlanId;
		UI32 native_vlanid = 0;
		ObjectId pVlanIntfObjId = ObjectId::NullObjectId;
		vector<UI32>::iterator vid;
		vector<ObjectId>::iterator vlanid;
		ObjectId vlanOId;
//		VlanIntfManagedObject *pVlanMo;

		string ifName = pNsmUpdatePhyIntfMessage->getIfName();
		ResourceId  status   = WAVE_MESSAGE_ERROR;
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

		status = getPhyIntfLocalMOByCmdCode(ifName, pNsmUpdatePhyIntfMessage->getCmdCode(), pPhyIntfLocalManagedObject, pDceSynchronousLinearSeqContext);

        if (pPhyIntfLocalManagedObject == NULL) {
            NSM_PLUG_DBG_ERR_S( string("UP_PHY_L Physical Interface MO not found ..:") + ifName);
            return (WAVE_MESSAGE_ERROR);
        } else {

            // MO will be updated if flag ifUpdateRequired == true
            bool ifUpdateRequired = true;

            // Keeping prismContext as well - else required changes in many other files
            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = dynamic_cast<PrismSynchronousLinearSequencerContext *> (pDceSynchronousLinearSeqContext);

            prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);

             NSM_PLUG_INFO_S( string ("UP_PHY_L Updating Physical Interface MO : ") + ifName + string("Cmdcode :") + pNsmUpdatePhyIntfMessage->getCmdCode());

             NativeVlanIntfConfLocalManagedObject *pNativeVlanIntfConfLocalManagedObject = NULL;

             switch(pNsmUpdatePhyIntfMessage->getCmdCode())
             {
                 case NSMUPDATE_PHY_DESC :
                 case NSMUPDATE_PHY_DELDESC :
                     pPhyIntfLocalManagedObject->setDesc (pNsmUpdatePhyIntfMessage->getDesc());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("description");
                     break;
                 case NSM_PLUGIN_EDGE_TO_ISL:
                     /*Portrole is set to ISL */
                     pPhyIntfLocalManagedObject->setPortrole(true);
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("Portrole");
					 if ( pNsmUpdatePhyIntfMessage->getException()) {
					 	pPhyIntfLocalManagedObject->setStatus (true);
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("status");
                     }
                	 trace (TRACE_LEVEL_INFO, string ("\tPortrole set to ISL ") + pNsmUpdatePhyIntfMessage->getCmdCode());
                     break;
                 case NSM_PLUGIN_ISL_TO_EDGE:
                     /*Portrole is set to EDGE*/
                     pPhyIntfLocalManagedObject->setPortrole(false);
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("Portrole");
					 if (pNsmUpdatePhyIntfMessage->getException()) {
					 	pPhyIntfLocalManagedObject->setStatus (true);
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("status");
                     }
                     trace (TRACE_LEVEL_INFO, string ("\tPortrole set to EDGE") + pNsmUpdatePhyIntfMessage->getCmdCode());
                     break;

				 case NSMUPDATE_PHY_NBR_DISCOVERY_DISABLE:
					 pPhyIntfLocalManagedObject->setNbrDiscoveryDisable(true);
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("nbrDiscoveryDisable");
 					 break;

				 case NSMUPDATE_PHY_DELNBR_DISCOVERY_DISABLE:
					 pPhyIntfLocalManagedObject->setNbrDiscoveryDisable(false);
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("nbrDiscoveryDisable");
 					 break;

                 case NSMUPDATE_PHY_MTU :
                 case NSMUPDATE_PHY_DELMTU :
                     pPhyIntfLocalManagedObject->setMtu (pNsmUpdatePhyIntfMessage->getMtu());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("mtu");
                     break;

                 case NSMUPDATE_PHY_OPENFLOW :
                 case NSMUPDATE_PHY_DELOPENFLOW :
                     pPhyIntfLocalManagedObject->setOpenflow (pNsmUpdatePhyIntfMessage->getOpenflow());
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("openflow_enable");
                     break;

                 case NSMUPDATE_PHY_VEPA :
                     pPhyIntfLocalManagedObject->setVepa (pNsmUpdatePhyIntfMessage->getVepa());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vepa_enable");
                     trace (TRACE_LEVEL_INFO, string ("VEPA MO") + pNsmUpdatePhyIntfMessage->getCmdCode());
                     break;
				 case ELD_INTF_CONF_PRIORITY:
                     pPhyIntfLocalManagedObject->setEldPrio (pNsmUpdatePhyIntfMessage->getEldPrioVal());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("eldprio");
                     pPhyIntfLocalManagedObject->setIfType (pNsmUpdatePhyIntfMessage->getIfType());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("ifType");
					 break;
				 case ELD_INTF_CONF_ELD_ENABLE:
						 trace (TRACE_LEVEL_INFO, string ("\tENABLE: VlanRange :: ") + (pNsmUpdatePhyIntfMessage->getVlanIdRange()).toString());
						 getVlanIntfObjectIdByVlanIfName ((pNsmUpdatePhyIntfMessage->getVlanIdRange()).toString(), vlanOId, false,pDceSynchronousLinearSeqContext);
						 if (vlanOId == ObjectId::NullObjectId) {
							 trace(TRACE_LEVEL_INFO, string("ENABLE: Vlan Interface does not exist .."));
							 return WAVE_MESSAGE_SUCCESS;
						 } 
						 else {
                     		pPhyIntfLocalManagedObject->setVlanidList (pNsmUpdatePhyIntfMessage->getVlanidList());
                             pPhyIntfLocalManagedObject->setIfType (pNsmUpdatePhyIntfMessage->getIfType());
					 		 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("ifType");
						 }
					 for (vlanid = pNsmUpdatePhyIntfMessage->m_vlanid.begin(); vlanid != pNsmUpdatePhyIntfMessage->m_vlanid.end(); vlanid++)
					 {
						 pPhyIntfLocalManagedObject->addVlanObjectId(*vlanid);
					 } 
				     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanid");

					 trace (TRACE_LEVEL_INFO, string ("\tENABLE: ELD VLAN MO ") + pNsmUpdatePhyIntfMessage->getCmdCode());
					 break;
				 case ELD_INTF_CONF_ELD_DISABLE:
						 trace (TRACE_LEVEL_INFO, string ("\tDISABLE: VlanRange :: ") + (pNsmUpdatePhyIntfMessage->getVlanIdRange()).toString());
						 getVlanIntfObjectIdByVlanIfName ((pNsmUpdatePhyIntfMessage->getVlanIdRange()).toString(), vlanOId, false,pDceSynchronousLinearSeqContext);
						 if (vlanOId == ObjectId::NullObjectId) {
							 trace(TRACE_LEVEL_INFO, string("DISABLE: Vlan Interface does not exist .."));
							 return WAVE_MESSAGE_SUCCESS;
						 } 
						 else {
							 trace(TRACE_LEVEL_INFO, string("DISABLE: Vlan Interface does exist for interface "));
						 }
					 for (vlanid = pNsmUpdatePhyIntfMessage->m_vlanid.begin(); vlanid != pNsmUpdatePhyIntfMessage->m_vlanid.end(); vlanid++)
					 {
						 pPhyIntfLocalManagedObject->deleteVlanObjectId(*vlanid);
					 } 
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanid");
                     trace (TRACE_LEVEL_INFO, string ("\tDISABLE: ELD VLAN MO ") + pNsmUpdatePhyIntfMessage->getCmdCode());
					 break; 
                 case NSMUPDATE_PHY_SW :
                     {
						if(pPhyIntfLocalManagedObject->getSwModeFlag() == pNsmUpdatePhyIntfMessage->getSwModeFlag()) {
                            ifUpdateRequired = false;
							break;
                        }

                         pPhyIntfLocalManagedObject->setSwModeFlag (pNsmUpdatePhyIntfMessage->getSwModeFlag());
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("swModeFlag");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("swBasic");

                         (NsmLocalObjectManager::getInstance()->getIntfXstpLocalConfWorker())->createPortXstp(pPhyIntfLocalManagedObject);
                         /* Default access enty added */
                         ObjectId vlanObjectId;
                         getVlanIntfObjectIdByVlanIfName ("1", vlanObjectId,false,pDceSynchronousLinearSeqContext);
                         pPhyIntfLocalManagedObject->setAccessVlanObjectId (vlanObjectId);
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanObjectId");

                         pPhyIntfLocalManagedObject->setSwMode(SW_MODE_ACCESS);
						 pPhyIntfLocalManagedObject->setVlanRangeAdd(UI32Range(""));
						 pPhyIntfLocalManagedObject->setVlanRangeRemove(UI32Range(""));
						 pPhyIntfLocalManagedObject->setVlanRangeExcept(UI32Range(""));

						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("swMode");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeAdd");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");

                         pPhyIntfLocalManagedObject->setRspanVlanRangeAdd(UI32Range(""));
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_trunkvlanrange");
                         pPhyIntfLocalManagedObject->setAccessRspanVlan (DCM_NSM_VLAN_DEFAULT_VID);
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_accessvlan");
                         break;
                     }
                 case NSMUPDATE_PHY_DELSW :
                     {
						if(pPhyIntfLocalManagedObject->getSwModeFlag() == pNsmUpdatePhyIntfMessage->getSwModeFlag()) {
                            ifUpdateRequired = false;
							break;
                        }

                         pPhyIntfLocalManagedObject->setSwModeFlag (pNsmUpdatePhyIntfMessage->getSwModeFlag());
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("swModeFlag");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("swBasic");

                         // delete port vlan stp mo
                         if (!pNsmUpdatePhyIntfMessage->getSwModeFlag())
                         {
                             INTF_XSTP_LOCAL_WORKER->deletePortXstp(pPhyIntfLocalManagedObject, pDceSynchronousLinearSeqContext);

							 pPhyIntfLocalManagedObject -> deleteAllVlanClassifierActivatedGroup();
							 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanClassifierActivate");

                         }

                         pPhyIntfLocalManagedObject->setSwMode(SW_MODE_UNKNOWN);
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("swMode");

                         /* Delete trunk mode entries */
                         pPhyIntfLocalManagedObject->setIsAllowedVlanAll(false);
						 pPhyIntfLocalManagedObject->setNativeVlanIntfConfig (WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (NULL));
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isAllowedVlanAll");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("nativeVlanIntfConfig");

                         /* Delete access mode entries */
                         pPhyIntfLocalManagedObject->setAccessVlanObjectId(ObjectId::NullObjectId);
						 pPhyIntfLocalManagedObject->setVlanRangeAdd(UI32Range(""));
						 pPhyIntfLocalManagedObject->setVlanRangeRemove(UI32Range(""));
						 pPhyIntfLocalManagedObject->setVlanRangeExcept(UI32Range(""));

						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanObjectId");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeAdd");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");
                         pPhyIntfLocalManagedObject->cleanUpPvlanAll();
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModePromiscuous");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModeHost");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModeTrunkBas");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModeTrunkPromis");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModeTrunkHost");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanNativeVlanId");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanAll");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanNone");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeAdd");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeRemove");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeExcept");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanHostPrimVlanId");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanHostSecVlanId");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanTrunkAssoc");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanMapping");

						 pPhyIntfLocalManagedObject->setRspanVlanRangeAdd(UI32Range(""));
						 pPhyIntfLocalManagedObject->setAccessRspanVlan(DCM_NSM_VLAN_DEFAULT_VID);
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_trunkvlanrange");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_accessvlan");

						 trace(TRACE_LEVEL_INFO,string("IntfConfLocalWo updatePhyIntfMOStep * "));
						 /* ELD vlan-list disabling  for "no sw" */

						 vector<ObjectId>  m_vlanid = pPhyIntfLocalManagedObject->getVlanVector();
						 trace (TRACE_LEVEL_INFO, string ("CLI: no switchport; for physical interface ") + ifName);
						 if (m_vlanid.size() > 0)
						 {
						 	 trace (TRACE_LEVEL_INFO, string ("CLI: no switchport;  Remove ELD vlans."));
							 for (vlanid = m_vlanid.begin(); vlanid != m_vlanid.end(); vlanid++) {
						 	 	 trace (TRACE_LEVEL_DEBUG, string ("CLI: no switchport;  Removing ELD vlan."));
								 pPhyIntfLocalManagedObject->deleteVlanObjectId(*vlanid);
   						     	 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanid");
							 }
						 }

						 /* Clear Router-advertisement-guard
						  * configuration for "no sw" */
						 pPhyIntfLocalManagedObject->setRaGuard(false);
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("raGuard");
                         pPhyIntfLocalManagedObject->deleteMacAddressVlanClassificationAll();
                         pPhyIntfLocalManagedObject->deleteMacGroupVlanClassificationAll();
                         pPhyIntfLocalManagedObject->deleteCtagVlanClassificationAll();
                         pPhyIntfLocalManagedObject->deleteCtagPVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("macAddressVlanClassification");
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("macGroupVlanClassification");
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("ctagVlanClassification");
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("ctagPVlanClassification");
                         pPhyIntfLocalManagedObject->deleteRspanMacAddressVlanClassificationAll();
                         pPhyIntfLocalManagedObject->deleteRspanMacGroupVlanClassificationAll();
                         pPhyIntfLocalManagedObject->deleteRspanCtagVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_macaddress_vlan_classififcation");
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_macgroup_vlan_classififcation");
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_gvlan_trunk");

						/* Clear port-security configuration */
						PortSecurityLocalManagedObject *pPortSecMO = (pPhyIntfLocalManagedObject->getPortSecConfig()).operator->();

                		if (pPortSecMO != NULL) {
                    		set<string> compositionSet;
                    		compositionSet.insert("portSecurity");
                    		deleteManagedObjectCompositions(pPhyIntfLocalManagedObject->getObjectId(), compositionSet);
                    		pPhyIntfLocalManagedObject->addAttributeToBeUpdated("portSecurity");
                		}

						 break;
					 }
                 case NSMUPDATE_PHY_MODE :
                 case NSMUPDATE_PHY_DELMODE:
			{

			         // When mode is changed the interface would be left with vlan 1..
			         // However backend is not handling this.. so cleanup everything!!
                     INTF_XSTP_LOCAL_WORKER->clearXstpPortInstAndPortVlanMOs(pPhyIntfLocalManagedObject, true);

//			         VlanIdList vlist (1);
//			         INTF_XSTP_LOCAL_WORKER->handleIntfVlanAssocUpdate(
//			                 pPhyIntfLocalManagedObject, vlist, pDceSynchronousLinearSeqContext, true);

                     if(pNsmUpdatePhyIntfMessage->getSwMode() == SW_MODE_ACCESS )
                     {
                         /* Default access enty added */
                         ObjectId vlanObjectId;
    					 getVlanIntfObjectIdByVlanIfName ("1", vlanObjectId,false,pDceSynchronousLinearSeqContext);
                         pPhyIntfLocalManagedObject->setAccessVlanObjectId (vlanObjectId);
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanObjectId");
						 pPhyIntfLocalManagedObject->setAccessRspanVlan(DCM_NSM_VLAN_DEFAULT_VID);
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_accessvlan");

                         /* Delete trunk mode entries */
						 pPhyIntfLocalManagedObject->setIsAllowedVlanAll(false);
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isAllowedVlanAll");

                     } else if ( pNsmUpdatePhyIntfMessage->getSwMode() == SW_MODE_TRUNK ) {
						 /*Remove vlan classifier Provisioning*/
						 pPhyIntfLocalManagedObject -> deleteAllVlanClassifierActivatedGroup();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanClassifierActivate");
                        /* Delete access mode entreis */
                        pPhyIntfLocalManagedObject->setAccessVlanObjectId(ObjectId::NullObjectId);
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanObjectId");

                     }

					 if(pPhyIntfLocalManagedObject->getSwMode() != SW_MODE_ACCESS &&
						 pNsmUpdatePhyIntfMessage->getSwMode() == SW_MODE_ACCESS)
					 {/*Clear the trunk allowed vlan list*/
						 pPhyIntfLocalManagedObject->setVlanRangeAdd(UI32Range(""));
						 pPhyIntfLocalManagedObject->setVlanRangeRemove(UI32Range(""));
						 pPhyIntfLocalManagedObject->setVlanRangeExcept(UI32Range(""));

						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeAdd");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");

                         pPhyIntfLocalManagedObject->setRspanVlanRangeAdd(UI32Range(""));
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_trunkvlanrange");

						 /* Clear the native vlan config */
						 pPhyIntfLocalManagedObject->setNativeVlanIntfConfig (WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (NULL));
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("nativeVlanIntfConfig");
						 pPhyIntfLocalManagedObject->deleteCtagVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("ctagVlanClassification");
						 pPhyIntfLocalManagedObject->deleteCtagPVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("ctagPVlanClassification");
						 pPhyIntfLocalManagedObject->deleteRspanCtagVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_gvlan_trunk");
					 }

					 if(pPhyIntfLocalManagedObject->getSwMode() != SW_MODE_TRUNK &&
						 pNsmUpdatePhyIntfMessage->getSwMode() == SW_MODE_TRUNK) {
						 /* set the native vlan config */
						 pNativeVlanIntfConfLocalManagedObject = new NativeVlanIntfConfLocalManagedObject(
										 dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
						 pNativeVlanIntfConfLocalManagedObject->setNativevlanDisable(true);
						 pNativeVlanIntfConfLocalManagedObject->setNativeVlanId(NSM_NATIVE_VLAN_DEFAULT);
						 pNativeVlanIntfConfLocalManagedObject->setNativevlanPresence(true);
						 pPhyIntfLocalManagedObject->setNativeVlanIntfConfig(
										 WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (
												 					pNativeVlanIntfConfLocalManagedObject));
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("nativeVlanIntfConfig");

						 pPhyIntfLocalManagedObject->setAccessRspanVlan(DCM_NSM_VLAN_DEFAULT_VID);
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_accessvlan");
						 pPhyIntfLocalManagedObject->deleteMacAddressVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("macAddressVlanClassification");
						 pPhyIntfLocalManagedObject->deleteMacGroupVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("macGroupVlanClassification");
						 pPhyIntfLocalManagedObject->deleteCtagPVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("ctagPVlanClassification");
						 pPhyIntfLocalManagedObject->deleteRspanMacAddressVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_macaddress_vlan_classififcation");
						 pPhyIntfLocalManagedObject->deleteRspanMacGroupVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_macgroup_vlan_classififcation");
					 }

					 pPhyIntfLocalManagedObject->setSwMode(pNsmUpdatePhyIntfMessage->getSwMode());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("swMode");
					 /* ELD vlan-list disabling for changing mode */

					 vector<ObjectId>  m_vlanid = pPhyIntfLocalManagedObject->getVlanVector();
					 trace (TRACE_LEVEL_INFO, string ("Changing mode for physical interface ") + ifName);
					 if (m_vlanid.size() > 0)
					 {
						 trace (TRACE_LEVEL_INFO, string ("Changing mode for physical interface;  Remove ELD vlans."));
						 for (vlanid = m_vlanid.begin(); vlanid != m_vlanid.end(); vlanid++) {
							 trace (TRACE_LEVEL_DEBUG, string ("CLI: change mode;  Removing ELD vlan."));
							 pPhyIntfLocalManagedObject->deleteVlanObjectId(*vlanid);
   						     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanid");
						 }
					 }


					 /* ELD vlan-list disabling for changing mode */

                     pPhyIntfLocalManagedObject->cleanUpPvlanAll();
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModePromiscuous");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModeHost");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModeTrunkBas");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModeTrunkPromis");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModeTrunkHost");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanNativeVlanId");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanAll");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanNone");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeAdd");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeRemove");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeExcept");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanHostPrimVlanId");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanHostSecVlanId");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanTrunkAssoc");
                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanMapping");

                     break;
			}
                 case NSMUPDATE_PHY_EXCEPTION_STATUS :
                      NSM_PLUG_INFO_S( string ("UP_PHY_L Exception Status  ifName : ") + ifName + string(" Status Flag : ") + pNsmUpdatePhyIntfMessage->getFlag());
                 case NSMUPDATE_PHY_STATUS :
                 case NSMUPDATE_PHY_DELSTATUS :
                     pPhyIntfLocalManagedObject->setStatus (pNsmUpdatePhyIntfMessage->getFlag());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("status");
                     break;
	 			 case NSMUPDATE_PHY_ACTVIVE_STATUS:
	 			 case NSMUPDATE_PHY_INACTVIVE_STATUS:
                     pPhyIntfLocalManagedObject->setActive (pNsmUpdatePhyIntfMessage->getActive());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("active");
                     break;
				 case NSMUPDATE_PHY_TIMEOUT :
                     pPhyIntfLocalManagedObject->setLacpTimeout (pNsmUpdatePhyIntfMessage->getLacpTimeout());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("lacpTimeout");
                     break;
				 case NSMUPDATE_PHY_PORT_PRIORITY :
   					 pPhyIntfLocalManagedObject->setLacpPortPriority (pNsmUpdatePhyIntfMessage->getLacpPortPriority());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("lacpPortPriority");
					 break;
				 case NSMUPDATE_PHY_LACP_DEFAULT_UP:
   					 pPhyIntfLocalManagedObject->setLacpDefaultUp (pNsmUpdatePhyIntfMessage->getLacpDefaultUp());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("lacpDefaultUp");
					 break;
				 case NSMUPDATE_PHY_LINESPEED :
                	 pPhyIntfLocalManagedObject->setLineSpeed (pNsmUpdatePhyIntfMessage->getLineSpeed());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("lineSpeed");
                     break;

				 case NSMUPDATE_PHY_NATIVEVLANDISABLE :
				 {
					ifUpdateRequired = false;
					/* Native vlan should be removed from Remove and Except lists */
					UI32Range vlanRemoverange =  pPhyIntfLocalManagedObject->getVlanRangeRemove();

					if ( NsmUtils::isNativeVlanPresentInList(vlanRemoverange, pNsmUpdatePhyIntfMessage->getNativevlanDisable(), pPhyIntfLocalManagedObject, NULL))
					{
						NSM_PLUG_DBG(" native vlan from remove list");
						pPhyIntfLocalManagedObject->setVlanRangeRemove(vlanRemoverange);
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
						ifUpdateRequired = true;
					}

					UI32Range vlanExceptrange =  pPhyIntfLocalManagedObject->getVlanRangeExcept();

					if (NsmUtils::isNativeVlanPresentInList(vlanExceptrange, pNsmUpdatePhyIntfMessage->getNativevlanDisable(), pPhyIntfLocalManagedObject, NULL))
					{
						NSM_PLUG_DBG(" native vlan from Except list");
						pPhyIntfLocalManagedObject->setVlanRangeExcept(vlanExceptrange);
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");
						ifUpdateRequired = true;
					}

					trace (TRACE_LEVEL_INFO, string ("\t Native Vlan Disable :: ") + pNsmUpdatePhyIntfMessage->getNativevlanDisable());
                    pNativeVlanIntfConfLocalManagedObject = pPhyIntfLocalManagedObject->getNativeVlanIntfConfig().operator->();

                     if(pNativeVlanIntfConfLocalManagedObject != NULL)
                     {
    					 pNativeVlanIntfConfLocalManagedObject->setNativevlanDisable(pNsmUpdatePhyIntfMessage->getNativevlanDisable());
    					 pNativeVlanIntfConfLocalManagedObject->addAttributeToBeUpdated("nativevlanDisable");
						 updateWaveManagedObject (pNativeVlanIntfConfLocalManagedObject);
                     }

                     break;
				 }
				 case NSMUPDATE_PHY_NATIVEVLANID :
				{
					ifUpdateRequired = false;
					/* Native vlan should be removed from Remove and Except lists */
					UI32Range vlanRemoverange =  pPhyIntfLocalManagedObject->getVlanRangeRemove();
					if ( NsmUtils::isNativeVlanPresentInList(vlanRemoverange, pNsmUpdatePhyIntfMessage->getNativeVlanId(), pPhyIntfLocalManagedObject, NULL))
					{
						NSM_PLUG_DBG(" native vlan from remove list");
						pPhyIntfLocalManagedObject->setVlanRangeRemove(vlanRemoverange);
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
						ifUpdateRequired = true;
					}
					UI32Range vlanExceptrange =  pPhyIntfLocalManagedObject->getVlanRangeExcept();
					if ( NsmUtils::isNativeVlanPresentInList(vlanExceptrange, pNsmUpdatePhyIntfMessage->getNativeVlanId(), pPhyIntfLocalManagedObject, NULL))
					{
						NSM_PLUG_DBG(" native vlan from Except list");
						pPhyIntfLocalManagedObject->setVlanRangeExcept(vlanExceptrange);
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");
						ifUpdateRequired = true;
					}
                     pNativeVlanIntfConfLocalManagedObject = pPhyIntfLocalManagedObject->getNativeVlanIntfConfig().operator->();

                     if(pNativeVlanIntfConfLocalManagedObject == NULL)
                     {

						 pNativeVlanIntfConfLocalManagedObject = new NativeVlanIntfConfLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
						 pNativeVlanIntfConfLocalManagedObject->setNativevlanDisable(true);
						 pNativeVlanIntfConfLocalManagedObject->setNativeVlanId(pNsmUpdatePhyIntfMessage->getNativeVlanId());
						 pNativeVlanIntfConfLocalManagedObject->setNativevlanPresence(true);
						 pPhyIntfLocalManagedObject->setNativeVlanIntfConfig(
										 WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (
												 					pNativeVlanIntfConfLocalManagedObject));
						 trace(TRACE_LEVEL_INFO, string("SW_MODE_PVLAN_TR_PROMISCUOUS native vlan: ") + pNsmUpdatePhyIntfMessage->getNativeVlanId());
					 	 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("nativeVlanIntfConfig");
						 ifUpdateRequired = true;
                     }
                     else
                     {
                    	 native_vlanid = pNativeVlanIntfConfLocalManagedObject->getNativeVlanId();
    					 pNativeVlanIntfConfLocalManagedObject->setNativeVlanId(pNsmUpdatePhyIntfMessage->getNativeVlanId());
					 	 pNativeVlanIntfConfLocalManagedObject->addAttributeToBeUpdated("nativevlanId");
					 	 updateWaveManagedObject (pNativeVlanIntfConfLocalManagedObject);

						 trace(TRACE_LEVEL_INFO, string("Old native vlan: ") + native_vlanid);
                     }


					 if (pNsmUpdatePhyIntfMessage->getNativeVlanId() != native_vlanid && native_vlanid != 0)
					 {

						 /* 
						  * ELD records (if any) for the previous Native-VLAN are
						  * being removed for the interface.
						  */

						 vector<ObjectId>  m_vlanid = pPhyIntfLocalManagedObject->getVlanVector();
						 trace(TRACE_LEVEL_INFO, string("m_vlanid size is") + m_vlanid.size());
						 if (m_vlanid.size() > 0) {
							 sprintf(buf, "%d", native_vlanid);	
							 vlanId = std::string(buf);
							 trace(TRACE_LEVEL_INFO, string("To delete native vlan ") + buf);

							 getVlanIntfObjectIdByVlanIfName(vlanId, pVlanIntfObjId, false, pDceSynchronousLinearSeqContext);

							 for (j = 0; j < m_vlanid.size(); j++) {
								 if (m_vlanid[j] == ObjectId::NullObjectId) {
									 NSM_PLUG_INFO_S(j + string ("VLAN ObjId Null: continue"));
									 continue;
								 }
								 if(m_vlanid[j] == pVlanIntfObjId)
								 {
									 trace(TRACE_LEVEL_INFO, string("Native vlan matched"));
 									/* If Native-vlan is within the allowed list, then 
                                	   ELD records for that VLAN need not be cleaned 
                              		*/
                                    vector <UI32> moAddVlanRangeVector;
                                    UI32Range moAddVlanrange = UI32Range("");
                                    moAddVlanrange = pPhyIntfLocalManagedObject->getVlanRangeAdd();
                                    moAddVlanrange.getUI32RangeVector(moAddVlanRangeVector);
                                    vector<UI32>::iterator result;
                                    vector <UI32> moExceptVlanRangeVector;
                                    UI32Range moExceptVlanrange = UI32Range("");
                                    vector<UI32>::iterator exceptresult;
                                    vector <UI32> moRemoveVlanRangeVector;
                                    UI32Range moRemoveVlanrange = UI32Range("");
                                    vector<UI32>::iterator removeresult;
                                    result = find (moAddVlanRangeVector.begin(), moAddVlanRangeVector.end(), native_vlanid);
                                    moExceptVlanrange =pPhyIntfLocalManagedObject->getVlanRangeExcept();
                                    moExceptVlanrange.getUI32RangeVector(moExceptVlanRangeVector);
                                    exceptresult = find (moExceptVlanRangeVector.begin(), moExceptVlanRangeVector.end(), native_vlanid);
                                    moRemoveVlanrange =pPhyIntfLocalManagedObject->getVlanRangeRemove();
                                    moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
                                    removeresult = find (moRemoveVlanRangeVector.begin(), moRemoveVlanRangeVector.end(), native_vlanid);


                                    /* ELD configuration should be deleted only if the current native vlan is not part of Allowed vlan list and allowed vlan all is not set.*/ 
									 if (((!pPhyIntfLocalManagedObject->getIsAllowedVlanAll())&&(result==moAddVlanRangeVector.end())) ||
                                         ((pPhyIntfLocalManagedObject->getIsAllowedVlanAll())&&((exceptresult!= moExceptVlanRangeVector.end())||(removeresult !=moRemoveVlanRangeVector.end()))))
									 {
										 pPhyIntfLocalManagedObject->deleteVlanObjectId(m_vlanid[j]);
										 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanid");
						 				 ifUpdateRequired = true;
									 }
                                 
									 if (((pPhyIntfLocalManagedObject->getIsAllowedVlanAll())&&(exceptresult== moExceptVlanRangeVector.end())&&(removeresult ==moRemoveVlanRangeVector.end()))||
                                          (result !=moAddVlanRangeVector.end()))//If allowed vlan is all or native vlan range is within allowed vlan list
									  	    // need to replay the config for native-vlan as back-end 
											// removes that following notification from nsm
									 {
										 EldClientUpdatePhyIntfMessage *K1 = new EldClientUpdatePhyIntfMessage();
										 InterfaceType ifType = (InterfaceType)pNsmUpdatePhyIntfMessage->getIfType();
        								 string ifName1 = pNsmUpdatePhyIntfMessage->getIfName();
										 vector<UI32>    vlanIdVector;

										 if (ifType == IF_TYPE_GI)
										 {
											 K1->setIfType(PHY_INT_GI);
										 }
										 else if (ifType == IF_TYPE_TE)
										 {
											 K1->setIfType(PHY_INT_TE);
										 }
                                         else if (ifType == IF_TYPE_FO)
                                         {
                                             K1->setIfType(PHY_INT_FO);
                                         }


										 K1->setOpCode(ELD_INTF_CONF_ELD_ENABLE);
										 if(true  == NsmUtils::isValidThreeTuple(ifName1) ) {
											 string twoTuppleIfName;
											 NsmUtils::getTwoTupleIfName(ifName1, twoTuppleIfName);
											 ifName1 = twoTuppleIfName;
										 }
										 K1->setIfName (ifName1);
										 trace(TRACE_LEVEL_INFO, string ("Replay of NATIVE-VLAN - ifName :") + ifName1);
										 vlanIdVector.push_back(native_vlanid);
										 K1->setVlanidList(vlanIdVector);

										 status = sendSynchronouslyToWaveClient ("eld",K1);
										 pNsmUpdatePhyIntfMessage->setCompletionStatus(K1->getCompletionStatus());	
										 NSM_PLUG_INFO_S( string ("UP_PHY_L Response from Backend : ") + ifName + string("Cmdcode :") 
																	+ pNsmUpdatePhyIntfMessage->getCmdCode() + string("status :")
																	+ status + string("backend clientStatus :") + K1->getClientStatus());
										 if( status == WAVE_MESSAGE_SUCCESS) {
											 if(  pNsmUpdatePhyIntfMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)                  {
												 status = pNsmUpdatePhyIntfMessage->getCompletionStatus();
												 delete K1;
												 return  status;
											 }

											 if ( (0 != K1->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != K1->getClientStatus() ) )  {
												 trace(TRACE_LEVEL_ERROR, string("Resp: UP_PHY_L  Client Error MsgId: ") +
													 K1->getMessageId() +
													 FrameworkToolKit::localize(K1->getClientStatus()));
												 status = NsmUtils::mapBackEndErrorsToDcmdErrors(K1->getClientStatus()) ;
												 delete K1;
												 return status;
											 }
										 }
										 delete K1;
										 K1 = NULL;
									 }
									 break;
								 }
							 }
						 }
					 }
					
                     break;
					 }


				 case NSMUPDATE_PHY_PORT_PROFILED :
					 {
						 WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeProfileCfgManagedObject::getClassName());
						 FcoeProfileCfgManagedObject *pFcoeProfileManagedObject = NULL;
						 string profileName = "default";
						 int numberOfResults = 0;
						 syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));;
						 vector<WaveManagedObject *> *pResults = NULL;

						 trace(TRACE_LEVEL_DEBUG, "APPM IntfConfLocalWorker::updatePhyIntfMOStep ProfiledPort notification"
							 " to "+ pNsmUpdatePhyIntfMessage->getIfName());
						 pPhyIntfLocalManagedObject->setIsPortProfiled(pNsmUpdatePhyIntfMessage->getIsPortProfiled());
						 pPhyIntfLocalManagedObject->setStatus(!(pNsmUpdatePhyIntfMessage->getIsPortProfiled()));

						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isPortProfiled");
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("status");
						 if(!(pNsmUpdatePhyIntfMessage->getIsPortProfiled()))
						 {
							 pResults = querySynchronously (&syncQueryCtxt);
							 if (pResults) {
								 numberOfResults = pResults->size ();
								 if (numberOfResults == 1) {	
									 pFcoeProfileManagedObject = dynamic_cast<FcoeProfileCfgManagedObject *>((*pResults)[0]);
									 APPM_PLUG_TRACE (TRACE_LEVEL_DEVEL, " SET_ELEM: Found the" 
										 " profile MO with key: " + profileName);
									 trace(TRACE_LEVEL_INFO, "APPM IntfConfLocalWorker:: Cleaning Fcoe Config " );
									 if(pFcoeProfileManagedObject->getFcoePort()){
										 pPhyIntfLocalManagedObject->
											 setFcoeport(WaveManagedObjectPointer<FcoeportAttrLocalManagedObject>(NULL));
										 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("fcoeport");
									 }
								 }
							 }  
						 }

						 trace(TRACE_LEVEL_INFO, string("APPM IntfConfLocalWorker::updatePhyIntfMOStep online " +
								 (!(pNsmUpdatePhyIntfMessage->getIsPortProfiled()))));
						 break;
					 }

                 //TODO : FCOE HANDLING????
                 case NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT :
                     {
                         AMPPProfileCompositeManagedObject* pAMPPProfileCompositeManagedObject =
                             new AMPPProfileCompositeManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                         pAMPPProfileCompositeManagedObject->setPortProfileName(pNsmUpdatePhyIntfMessage->getPortProfileName());
                         pPhyIntfLocalManagedObject->addPortProfileToInterface(pAMPPProfileCompositeManagedObject);
                         pPhyIntfLocalManagedObject->setStatus(false);
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("associatedPortProfiles");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("status");
                         break;
                     }
                 case NSMUPDATE_DISSOCIATE_PORT_PROFILE_WITH_PHY_PORT :
                     {
                         vector<WaveManagedObjectPointer<AMPPProfileCompositeManagedObject> > associatedPortProfiles 
                             = pPhyIntfLocalManagedObject->getAssociatedPortProfiles();
                         //If the already associated port-profiles is 1 and dissociate request is success
                         //port is becoming a non-port-profile-port. Do reset the status of interface.
                         if(1 == associatedPortProfiles.size()) {
                             pPhyIntfLocalManagedObject->setStatus(true);
                             pPhyIntfLocalManagedObject->addAttributeToBeUpdated("status");
                         }
                         pPhyIntfLocalManagedObject->removePortProfileFromInterface(pNsmUpdatePhyIntfMessage->getPortProfileName());
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("associatedPortProfiles");
                         break;
                     }
                 case NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT :
                     {
                         AMPPProfileDomainCompositeManagedObject* pAMPPProfileDomainCompositeManagedObject =
                             new AMPPProfileDomainCompositeManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                         pAMPPProfileDomainCompositeManagedObject->setPortProfileDomainName(pNsmUpdatePhyIntfMessage->getPortProfileDomainName());
                         pPhyIntfLocalManagedObject->setAssociatedPortProfileDomain(pAMPPProfileDomainCompositeManagedObject);
                         pPhyIntfLocalManagedObject->setStatus(false);
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("associatedPortProfileDomain");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("status");
                         break;
                     }
                 case NSMUPDATE_DISSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT : 
                     {
                         pPhyIntfLocalManagedObject->setAssociatedPortProfileDomain
                             (WaveManagedObjectPointer<AMPPProfileDomainCompositeManagedObject>(NULL));
                         pPhyIntfLocalManagedObject->setStatus(true);
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("associatedPortProfileDomain");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("status");
                         break;
                     }
				case NSMUPDATE_PHY_SFLOW_ENABLE :
                {
                    trace (TRACE_LEVEL_DEBUG, string ("\tIntfConfLocalWorker::Case NSMUPDATE_PHY_SFLOW_ENABLE "));
                    if(pNsmUpdatePhyIntfMessage->getEnable())
                    {
                        if(!(pPhyIntfLocalManagedObject->getIsSampleRateSet()))
						{
                            pPhyIntfLocalManagedObject->setSampleRate(SFLOW_OUT_OF_RANGE_VALUE);
							pPhyIntfLocalManagedObject->addAttributeToBeUpdated("sampleRate");
						}
                        if(!(pPhyIntfLocalManagedObject->getIsPollingIntervalSet()))
						{
                            pPhyIntfLocalManagedObject->setPollingInterval(SFLOW_OUT_OF_RANGE_VALUE);
							pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pollingInterval");
						}
                        pPhyIntfLocalManagedObject->setEnable(pNsmUpdatePhyIntfMessage->getEnable());
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("enable");
                    }
                    else
                    {
                        pPhyIntfLocalManagedObject->setEnable(SFLOW_DEFAULT_ENABLE);
                        pPhyIntfLocalManagedObject->setSampleRate(SFLOW_OUT_OF_RANGE_VALUE);
                        pPhyIntfLocalManagedObject->setIsSampleRateSet(SFLOW_DEFAULT_ENABLE);
                        pPhyIntfLocalManagedObject->setPollingInterval(SFLOW_OUT_OF_RANGE_VALUE);
                        pPhyIntfLocalManagedObject->setIsPollingIntervalSet(SFLOW_DEFAULT_ENABLE);

						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("enable");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("sampleRate");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isSampleRateSet");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pollingInterval");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isPollingIntervalSet");
                    }
                    break;
                }
                case NSMUPDATE_PHY_DELSFLOW_ENABLE :
                {
                    trace (TRACE_LEVEL_DEBUG, string ("\tIntfConfLocalWorker::Case NSMUPDATE_PHY_SFLOW_ENABLE "));
                    pPhyIntfLocalManagedObject->setEnable(SFLOW_DEFAULT_ENABLE);
                    pPhyIntfLocalManagedObject->setSampleRate(SFLOW_OUT_OF_RANGE_VALUE);
                    pPhyIntfLocalManagedObject->setIsSampleRateSet(SFLOW_DEFAULT_ENABLE);
                    pPhyIntfLocalManagedObject->setPollingInterval(SFLOW_OUT_OF_RANGE_VALUE);
                    pPhyIntfLocalManagedObject->setIsPollingIntervalSet(SFLOW_DEFAULT_ENABLE);

					pPhyIntfLocalManagedObject->addAttributeToBeUpdated("enable");
                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("sampleRate");
                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isSampleRateSet");
                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pollingInterval");
                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isPollingIntervalSet");
                    break;
                }
                case NSMUPDATE_PHY_SFLOW_POLLING_INTERVAL :
                    trace (TRACE_LEVEL_DEBUG, string ("\tIntfConfLocalWorker::Case NSMUPDATE_PHY_SFLOW_POLLING_INTERVAL "));
                    pPhyIntfLocalManagedObject->setPollingInterval(pNsmUpdatePhyIntfMessage->getPollingInterval());
                    pPhyIntfLocalManagedObject->setIsPollingIntervalSet(true);
					pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pollingInterval");
                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isPollingIntervalSet");
                    break;
                case NSMUPDATE_PHY_DELSFLOW_POLLING_INTERVAL :
                    trace (TRACE_LEVEL_DEBUG, string ("\tIntfConfLocalWorker::Case NSMUPDATE_PHY_DELSFLOW_POLLING_INTERVAL "));
                    pPhyIntfLocalManagedObject->setPollingInterval(SFLOW_OUT_OF_RANGE_VALUE);
                    pPhyIntfLocalManagedObject->setIsPollingIntervalSet(false);
					pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isPollingIntervalSet");
                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pollingInterval");
                    break;
                case NSMUPDATE_PHY_SFLOW_SAMPLE_RATE :
                    trace (TRACE_LEVEL_DEBUG, string ("\tIntfConfLocalWorker::Case NSMUPDATE_PHY_SFLOW_SAMPLE_RATE"));
                    pPhyIntfLocalManagedObject->setSampleRate(pNsmUpdatePhyIntfMessage->getSampleRate());
                    pPhyIntfLocalManagedObject->setIsSampleRateSet(true);
					pPhyIntfLocalManagedObject->addAttributeToBeUpdated("sampleRate");
                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isSampleRateSet");
                    break;
                case NSMUPDATE_PHY_DELSFLOW_SAMPLE_RATE :
                    trace (TRACE_LEVEL_DEBUG, string ("\tIntfConfLocalWorker::Case NSMUPDATE_PHY_DELSFLOW_SAMPLE_RATE"));
                    pPhyIntfLocalManagedObject->setSampleRate(SFLOW_OUT_OF_RANGE_VALUE);
                    pPhyIntfLocalManagedObject->setIsSampleRateSet(false);

					pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isSampleRateSet");
                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("sampleRate");
                    break;
				 case NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN:
				 case NSMUPDATE_PHY_SW_ACCESS_ADD_RSPANVLAN:
					 {
						 ObjectId vlanObjectId;
						 trace (TRACE_LEVEL_INFO, string ("\t VlanRange :: ") +
					     	(pNsmUpdatePhyIntfMessage->getVlanRange()).toString());

                         if(NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN == pNsmUpdatePhyIntfMessage->getCmdCode()) {
							 getVlanIntfObjectIdByVlanIfName ((pNsmUpdatePhyIntfMessage->getVlanRange()).toString(), vlanObjectId,
															   false,pPrismSynchronousLinearSequencerContext);
							 pPhyIntfLocalManagedObject->setAccessVlanObjectId (vlanObjectId);
							 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanObjectId");

							 pPhyIntfLocalManagedObject->setAccessRspanVlan (DCM_NSM_VLAN_DEFAULT_VID);
							 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_accessvlan");
                         }
                         else {
                        	 NSM_PLUG_INFO_S(string("Update the Rspan Access Vlan"));
							 pPhyIntfLocalManagedObject->setAccessVlanObjectId(ObjectId::NullObjectId);
							 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanObjectId");
							 string accessRange = pNsmUpdatePhyIntfMessage->getVlanRange().toString();
							 UI32 rspanAccessVlan = strtoul(accessRange.c_str(), NULL, 0);
							 pPhyIntfLocalManagedObject->setAccessRspanVlan (rspanAccessVlan);
							 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_accessvlan");
                         }
						 
						 // Backend is not retaining port-instance configs even if new vlan is part of
						 // same instance!!! So as a workaround delete everything on access vlan change..
						 INTF_XSTP_LOCAL_WORKER->clearXstpPortInstAndPortVlanMOs(pPhyIntfLocalManagedObject, true);

//						 VlanIdList vlist (vlanObjectId);
//						 INTF_XSTP_LOCAL_WORKER->handleIntfVlanAssocUpdate(
//						         pPhyIntfLocalManagedObject, vlist, pDceSynchronousLinearSeqContext, true);

						 /* ELD vlan-list disabling for change of access vlan */

						 vector<ObjectId>  m_vlanid = pPhyIntfLocalManagedObject->getVlanVector();
						 trace (TRACE_LEVEL_INFO, string ("Changing access vlan for physical interface ") + ifName);
						 if (m_vlanid.size() > 0)
						 {
						 	 trace (TRACE_LEVEL_INFO, string ("Changing access vlan for physical interface; Remove ELD vlans. "));
							 for (vlanid = m_vlanid.begin(); vlanid != m_vlanid.end(); vlanid++) {
								 trace (TRACE_LEVEL_DEBUG, string ("CLI: no switchport;  Removing ELD vlan."));
								 pPhyIntfLocalManagedObject->deleteVlanObjectId(*vlanid);
   						     	 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanid");
							 }
						 }

                         /* ELD vlan-list disabling for change of access vlan */

						 break;
					 }
				 case NSMUPDATE_PHY_SW_DELETE_ACCESS_VLAN:
				 case NSMUPDATE_PHY_SW_ACCESS_DEL_RSPANVLAN:
					 {
						 ObjectId vlanObjectId;
						ObjectId preVlanObjectId = pPhyIntfLocalManagedObject->getAccessVlanObjectId();

						 getVlanIntfObjectIdByVlanIfName ("1", vlanObjectId,false,pDceSynchronousLinearSeqContext);
						 pPhyIntfLocalManagedObject->setAccessVlanObjectId (vlanObjectId);
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanObjectId");

						if (vlanObjectId != preVlanObjectId) {
	                        VlanIdList vlist (vlanObjectId);
	                        INTF_XSTP_LOCAL_WORKER->handleIntfVlanAssocUpdate(
	                                pPhyIntfLocalManagedObject, vlist, pDceSynchronousLinearSeqContext, true);
						}
						// Update the rspan access vlan field
						if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_ACCESS_DEL_RSPANVLAN) {
							 NSM_PLUG_INFO_S(string("Set default value for rspan access vlan"));
	                         pPhyIntfLocalManagedObject->setAccessRspanVlan (DCM_NSM_VLAN_DEFAULT_VID);
							 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_accessvlan");
						}
						 break;
					 }
				 case NSMUPDATE_PHY_SW_TRUNK_VLAN_ALL:
					 {
						 pPhyIntfLocalManagedObject->setIsAllowedVlanAll(pNsmUpdatePhyIntfMessage->getIsAllowedVlanAll());
						 pPhyIntfLocalManagedObject->setIsAllowedVlanNone(false);
						 pPhyIntfLocalManagedObject->setVlanRangeAdd(UI32Range(""));
						 pPhyIntfLocalManagedObject->setVlanRangeRemove(NsmUtils::getRspanVlanUI32Range(pDceSynchronousLinearSeqContext));
						 pPhyIntfLocalManagedObject->setVlanRangeExcept(UI32Range(""));

						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isAllowedVlanAll");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isAllowedVlanNone");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeAdd");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");
						 break;
					 }
				 case NSMUPDATE_PHY_SW_TRUNK_VLAN_NONE:
					 {
						 pPhyIntfLocalManagedObject->setIsAllowedVlanNone(false);
						 pPhyIntfLocalManagedObject->setIsAllowedVlanAll(false);
						 pPhyIntfLocalManagedObject->setVlanRangeAdd(UI32Range(""));
						 pPhyIntfLocalManagedObject->setVlanRangeRemove(UI32Range(""));
						 pPhyIntfLocalManagedObject->setVlanRangeExcept(UI32Range(""));

						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isAllowedVlanAll");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isAllowedVlanNone");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeAdd");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");

                         if (pPhyIntfLocalManagedObject->m_nativeVlanIntfConfig.operator ->() != NULL)
                         {
                        	 VlanIdList vlist (pPhyIntfLocalManagedObject->m_nativeVlanIntfConfig->getNativeVlanId());
                        	 INTF_XSTP_LOCAL_WORKER->handleIntfVlanAssocUpdate(
                        			 pPhyIntfLocalManagedObject, vlist, pDceSynchronousLinearSeqContext, true);
                         }

                         //Delete ELD records except Native VLAN
                                 
                         vector<ObjectId>  m_eldVlanid = pPhyIntfLocalManagedObject->getVlanVector();
                         NSM_PLUG_DBG_S(string("deleting eld vlan :size") +m_eldVlanid.size());
                         if (m_eldVlanid.size() > 0)
                         {
                              WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanIntfConfLocalManagedObjectpo;
                              pNativeVlanIntfConfLocalManagedObjectpo = pPhyIntfLocalManagedObject->getNativeVlanIntfConfig();
                              if(pNativeVlanIntfConfLocalManagedObjectpo.operator->() == NULL) 
                              {
                                   NSM_PLUG_INFO_S(string("Native vlan 0"));
                                   break;
                              }

                              UI32 nativeVlanId = pNativeVlanIntfConfLocalManagedObjectpo->getNativeVlanId();
                              trace(TRACE_LEVEL_DEBUG, string("Checking for ELD-records for native vlan ") + buf);
                             NsmUtils:: getVlanIntfObjectIdByVlanIfName(nativeVlanId, pVlanIntfObjId, true,pDceSynchronousLinearSeqContext);
                              for ( j = 0; j< m_eldVlanid.size(); j++) {
                              /*  if (m_eldVlanid[j] == ObjectId::NullObjectId)
                                  {
                                       NSM_PLUG_INFO_S(j + string ("VLAN ObjId Null: continue"));
                                       continue;
                                  }*/
                                  if(m_eldVlanid[j] != pVlanIntfObjId)
                                  {
                                       pPhyIntfLocalManagedObject->deleteVlanObjectId(m_eldVlanid[j]);
                                  }
                              } 
                              pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanid");
                          }
						 break;
					 }
				 case NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN:
				 case NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN:
					 {
						 NSM_PLUG_INFO_S(string("CmdCode = NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN or CmdCode = NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN"));
						 vector<UI32>::iterator msgiter;
						 vector <UI32> msgVlanRangeVector;
						 UI32Range msgVlanrange = UI32Range("");

						 /* Delete Native vlan from vlanRange if it is present */
						 deleteNativeVlanFromMsgVlanList(pNsmUpdatePhyIntfMessage, pPhyIntfLocalManagedObject);

						 msgVlanrange = pNsmUpdatePhyIntfMessage->getVlanRange();
						 msgVlanrange.getUI32RangeVector(msgVlanRangeVector);
						 msgVlanrange = UI32Range("");

						 if(pPhyIntfLocalManagedObject->getIsAllowedVlanAll() && pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN)
						 {/*Check the remove list*/
							 NSM_PLUG_DBG_S(string("Cmd: allowed vlan all"));
							 UI32Range moRemoveVlanrange = UI32Range("");
							 vector <UI32> moRemoveVlanRangeVector;
							 vector<UI32>::iterator moremiter;

							 moRemoveVlanrange = pPhyIntfLocalManagedObject->getVlanRangeRemove();
							 moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
							 moRemoveVlanrange = UI32Range("");

							 for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
							 {
								 for (moremiter=moRemoveVlanRangeVector.begin(); moremiter != moRemoveVlanRangeVector.end(); moremiter++)
								 {
									 if(*moremiter == *msgiter)
									 {/*Found in remove list, so now erase the element from the remove vector*/
										 moRemoveVlanRangeVector.erase(moremiter);
										 break;
									 }

								 }

							 }
							 string vlanRemoveRangeStr = UI32Range::getUI32RangeStringFromVector(moRemoveVlanRangeVector);
							 moRemoveVlanrange.fromString(vlanRemoveRangeStr);
							 pPhyIntfLocalManagedObject->setVlanRangeRemove(moRemoveVlanrange);
							 pPhyIntfLocalManagedObject->setVlanRangeAdd(UI32Range(""));
							 pPhyIntfLocalManagedObject->setVlanRangeExcept(UI32Range(""));

							 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
                             pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeAdd");
                             pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");
						 }
						 else
						 {
							 vector <UI32> moVlanRangeVector;
							 UI32Range moVlanrange = UI32Range("");
							 if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN) {
								 NSM_PLUG_INFO_S(string("Cmd: NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN"));
								 moVlanrange = pPhyIntfLocalManagedObject->getRspanVlanRangeAdd();
								 NSM_PLUG_INFO_S(string("Existing rspan vlan range:")+moVlanrange.toString());
							 }
							 else {
								 moVlanrange = pPhyIntfLocalManagedObject->getVlanRangeAdd();
							 }
							 moVlanrange.getUI32RangeVector(moVlanRangeVector);
							 moVlanrange = UI32Range("");

							 vector<UI32>::iterator moiter;
							 vector<UI32>::iterator moindex = moVlanRangeVector.begin();
							 bool isVlanIdPresent = true;

							 for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
							 {
								 for (moiter=moVlanRangeVector.begin(); moiter != moVlanRangeVector.end(); moiter++)
								 {
									 if(*moiter == *msgiter)
									 {
										 break;
									 }
									 else if(*moiter > *msgiter)/*Since this is a sorted vector, and we have already exceeded the required
																  VLAN ID, we cannot find it in further parses, so break*/
									 {
										 moindex = moVlanRangeVector.begin();
										 isVlanIdPresent = false;
										 break;
									 }
								 }

								 if(!isVlanIdPresent)
									 while(*moindex < *msgiter)
										 moindex++;

								 if(moiter == moVlanRangeVector.end())
								 {
									 isVlanIdPresent = false;
									 moindex = moVlanRangeVector.end();
								 }

								 if(!isVlanIdPresent)
						         	moVlanRangeVector.insert(moindex, *msgiter);

								 moindex = moVlanRangeVector.begin();
								 isVlanIdPresent = true;
							 }

							 string vlanRangeStr = UI32Range::getUI32RangeStringFromVector(moVlanRangeVector);
							 moVlanrange.fromString(vlanRangeStr);
							 NSM_PLUG_INFO_S(string("Updated vlan range:")+moVlanrange.toString());
							 if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN) {
								 NSM_PLUG_INFO_S(string("Update the rspanvlanrangeadd field in the interface MO"));
								 pPhyIntfLocalManagedObject->setRspanVlanRangeAdd(moVlanrange);
								 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_trunkvlanrange");
							 }
							 else {
								 pPhyIntfLocalManagedObject->setVlanRangeAdd(moVlanrange);
								 pPhyIntfLocalManagedObject->setVlanRangeRemove(UI32Range(""));
								 pPhyIntfLocalManagedObject->setVlanRangeExcept(UI32Range(""));
								 pPhyIntfLocalManagedObject->setIsAllowedVlanNone(false);
								 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeAdd");
								 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
								 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");
								 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isAllowedVlanNone");
							 }
							
						 }
						 break;
					 }
				 case NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN:
				 case NSMUPDATE_PHY_SW_TRUNK_DEL_RSPANVLAN:
						{
							vector<UI32>::iterator moindex;
							vector<UI32>::iterator msgiter;
							vector <UI32> msgVlanRangeVector;
							UI32Range msgVlanrange = UI32Range("");
							bool isVlanIdPresent = true;
							VlanIdList vlist;

							/* Delete Native vlan from vlanRange if it is present */
							deleteNativeVlanFromMsgVlanList(pNsmUpdatePhyIntfMessage, pPhyIntfLocalManagedObject);

							msgVlanrange = pNsmUpdatePhyIntfMessage->getVlanRange();
							msgVlanrange.getUI32RangeVector(msgVlanRangeVector);
							NSM_PLUG_INFO_S(string("From message: vlan range to be deleted from MO:")+msgVlanrange.toString());
							msgVlanrange = UI32Range("");

							if(pPhyIntfLocalManagedObject->getIsAllowedVlanAll() && pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN)
							{/*Check the remove list*/
								UI32Range moRemoveVlanrange = UI32Range("");
								vector <UI32> moRemoveVlanRangeVector;
								vector<UI32>::iterator moremiter;

								moindex = moRemoveVlanRangeVector.begin();
								moRemoveVlanrange = pPhyIntfLocalManagedObject->getVlanRangeRemove();
								moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
								moRemoveVlanrange = UI32Range("");

								for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
								{
									for (moremiter=moRemoveVlanRangeVector.begin(); moremiter != moRemoveVlanRangeVector.end(); moremiter++)
									{
										if(*moremiter == *msgiter)
										{/*Found in remove list, so do nothing*/
											break;
										}

										else if(*moremiter > *msgiter)/*Since this is a sorted vector, and we have already exceeded the required 																		VLAN ID, we cannot find it in further parses, so break*/
										{
											moindex = moRemoveVlanRangeVector.begin();
											isVlanIdPresent = false;
											break;
										}
									}

									if(!isVlanIdPresent)
										while(*moindex < *msgiter)
											moindex++;

									if(moremiter == moRemoveVlanRangeVector.end())
									{
										isVlanIdPresent = false;
										moindex = moRemoveVlanRangeVector.end();
									}

									if(!isVlanIdPresent)/*Vlan not found in MO, add it at appropriate position to keep the vector sorted*/
										moRemoveVlanRangeVector.insert(moindex, *msgiter);

									moindex = moRemoveVlanRangeVector.begin();
									isVlanIdPresent = true;
								}

								string vlanRemoveRangeStr = UI32Range::getUI32RangeStringFromVector(moRemoveVlanRangeVector);
								moRemoveVlanrange.fromString(vlanRemoveRangeStr);
								pPhyIntfLocalManagedObject->setVlanRangeRemove(moRemoveVlanrange);
								pPhyIntfLocalManagedObject->setVlanRangeAdd(UI32Range(""));
								pPhyIntfLocalManagedObject->setVlanRangeExcept(UI32Range(""));

							    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");
                            	pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeAdd");
                            	pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");	

                            	vlist.m_vlanIds = moRemoveVlanRangeVector;
                            	vlist.m_type = NONMEMBER_VLAN_LIST;
							}
							else
							{
								vector <UI32> moVlanRangeVector;
								UI32Range moVlanrange = UI32Range("");

								 if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_TRUNK_DEL_RSPANVLAN) {
									 NSM_PLUG_INFO_S(string("Cmd: NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN"));
									 moVlanrange = pPhyIntfLocalManagedObject->getRspanVlanRangeAdd();
									 NSM_PLUG_INFO_S(string("Existing rspan vlan range:")+moVlanrange.toString());
								 }
								 else {
									 moVlanrange = pPhyIntfLocalManagedObject->getVlanRangeAdd();
								 }

								moVlanrange.getUI32RangeVector(moVlanRangeVector);
								moVlanrange = UI32Range("");

								vector<UI32>::iterator moiter;
								moindex = moVlanRangeVector.begin();

								for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
								{
									for (moiter=moVlanRangeVector.begin(); moiter != moVlanRangeVector.end(); moiter++)
									{
										if(*moiter == *msgiter)
										{/*Vlan Id exists, remove from vlan list*/
											moVlanRangeVector.erase(moiter);
											break;
										}
									}
								}

								string vlanRangeStr = UI32Range::getUI32RangeStringFromVector(moVlanRangeVector);

								moVlanrange.fromString(vlanRangeStr);
								NSM_PLUG_INFO_S(string("Updated vlan range:")+moVlanrange.toString());
								if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN) {
									pPhyIntfLocalManagedObject->setVlanRangeAdd(moVlanrange);
									pPhyIntfLocalManagedObject->setVlanRangeRemove(UI32Range(""));
									pPhyIntfLocalManagedObject->setVlanRangeExcept(UI32Range(""));

									pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");
									pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeAdd");
									pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
								}
								else {
									pPhyIntfLocalManagedObject->setRspanVlanRangeAdd(moVlanrange);
									pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_trunkvlanrange");
								}

                            	vlist.m_vlanIds = moVlanRangeVector;
                            	if (pPhyIntfLocalManagedObject->m_nativeVlanIntfConfig.operator ->() != NULL) {
                            	    vlist.m_vlanIds.push_back(pPhyIntfLocalManagedObject->m_nativeVlanIntfConfig->getNativeVlanId());
                            	}
							}

							INTF_XSTP_LOCAL_WORKER->handleIntfVlanAssocUpdate(
							        pPhyIntfLocalManagedObject, vlist, pDceSynchronousLinearSeqContext, true);

							/* 
							 * ELD records for these specific VLANs are
							 * being removed.
							 */

							for (vid = msgVlanRangeVector.begin(); vid != msgVlanRangeVector.end(); vid++)
							{
								sprintf(buf, "%d", *vid);	
								vlanId = std::string(buf);
								getVlanIntfObjectIdByVlanIfName(vlanId, pVlanIntfObjId, false, pPrismSynchronousLinearSequencerContext);
								vector<ObjectId>  m_vlanid = pPhyIntfLocalManagedObject->getVlanVector();
								if (m_vlanid.size() > 0) {
									trace(TRACE_LEVEL_INFO, string("m_vlanid.size() = ") + m_vlanid.size());
									for (j = 0; j < m_vlanid.size(); j++) {
										if (m_vlanid[j] == ObjectId::NullObjectId) {
											NSM_PLUG_INFO_S(j + string ("VLAN ObjId Null: continue"));
											continue;
										}
										if(m_vlanid[j] == pVlanIntfObjId)
										{
											trace(TRACE_LEVEL_INFO, string("Removing for VLAN = ") + *vid);
											pPhyIntfLocalManagedObject->deleteVlanObjectId(m_vlanid[j]);
							 				pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanid");
//											updateWaveManagedObject(pPhyIntfLocalManagedObject);
											break;
										}
									}
								}
								memset(buf, 0, 6);
							}
							break;
						}
				 case NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN:
						{
							UI32Range msgVlanrange = UI32Range("");
							UI32Range moVlanrange = UI32Range("");

							/* Delete Native vlan from vlanRange if it is present */
							deleteNativeVlanFromMsgVlanList(pNsmUpdatePhyIntfMessage, pPhyIntfLocalManagedObject);

							msgVlanrange = pNsmUpdatePhyIntfMessage->getVlanRange();

							VlanIdList vlist (msgVlanrange, NONMEMBER_VLAN_LIST);
							INTF_XSTP_LOCAL_WORKER->handleIntfVlanAssocUpdate(
							        pPhyIntfLocalManagedObject, vlist, pDceSynchronousLinearSeqContext, true);
							//We include the existing rspan-vlans as well in the vlanRangeRemove
							msgVlanrange = msgVlanrange + NsmUtils::getRspanVlanUI32Range(pDceSynchronousLinearSeqContext);

							pPhyIntfLocalManagedObject->setIsAllowedVlanAll(true);
							pPhyIntfLocalManagedObject->setIsAllowedVlanNone(false);
							pPhyIntfLocalManagedObject->setVlanRangeRemove(msgVlanrange);
							pPhyIntfLocalManagedObject->setVlanRangeAdd(UI32Range(""));
							pPhyIntfLocalManagedObject->setVlanRangeExcept(UI32Range(""));

							pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isAllowedVlanAll");
							pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isAllowedVlanNone");
							pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeAdd");
							pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
							pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");
							/* 
							 * ELD records for these specific VLANs are
							 * being removed.
							 */
							vector <UI32> msgVlanRangeVector;
							msgVlanrange.getUI32RangeVector(msgVlanRangeVector);


							for (vid = msgVlanRangeVector.begin(); vid != msgVlanRangeVector.end(); vid++)
							{
								sprintf(buf, "%d", *vid);	
								vlanId = std::string(buf);
								getVlanIntfObjectIdByVlanIfName(vlanId, pVlanIntfObjId, false, pPrismSynchronousLinearSequencerContext);
								vector<ObjectId>  m_eldVlanid = pPhyIntfLocalManagedObject->getVlanVector();
								if (m_eldVlanid.size() > 0) {
									for (j = 0; j < m_eldVlanid.size(); j++) {
										/*if (m_eldVlanid[j] == ObjectId::NullObjectId) {
											NSM_PLUG_INFO_S(j + string ("VLAN ObjId Null: continue"));
											continue;
										}*/
										if(m_eldVlanid[j] == pVlanIntfObjId)
										{
											NSM_PLUG_INFO_S(string("Removing for VLAN = ") + *vid);
											pPhyIntfLocalManagedObject->deleteVlanObjectId(m_eldVlanid[j]);
											pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanid");
											break;
										}
									}
								}
							}
						}
						break;
				case NSMUPDATE_PHY_SET_IPV6_RA_GUARD:
				case NSMUPDATE_PHY_RESET_IPV6_RA_GUARD:
				{
					pPhyIntfLocalManagedObject->setRaGuard(pNsmUpdatePhyIntfMessage->getRaGuard());
					pPhyIntfLocalManagedObject->addAttributeToBeUpdated("raGuard");
					break;
				}
				 case NSMUPDATE_PHY_PVLAN_MODE_TRUNK:
				 case NSMUPDATE_PHY_PVLAN_MODE_TRUNK_HOST:
				 case NSMUPDATE_PHY_PVLAN_MODE_TRUNK_PROMISCUOUS:
				 case NSMUPDATE_PHY_PVLAN_MODE_PROMISCUOUS:
				 case NSMUPDATE_PHY_PVLAN_MODE_HOST:
				 {
						pPhyIntfLocalManagedObject->cleanUpPvlanAll();
						pPhyIntfLocalManagedObject->cleanUpAccessTrunkAll();
						pPhyIntfLocalManagedObject->setNativeVlanIntfConfig (WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (NULL));

						if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_PVLAN_MODE_TRUNK){
							pPhyIntfLocalManagedObject->setPvlanModeTrunkBas(true);
							trace(TRACE_LEVEL_INFO,string("IntfConfLocalW NSMUPDATE_PHY_PVLAN_MODE_TRUNK  "));

						} else if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_PVLAN_MODE_TRUNK_HOST){

							pPhyIntfLocalManagedObject->setPvlanModeTrunkHost(true);
							trace(TRACE_LEVEL_INFO,string("IntfConfLocalW NSMUPDATE_PHY_PVLAN_MODE_TRUNK_HOST "));

						} else if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_PVLAN_MODE_TRUNK_PROMISCUOUS){

							pPhyIntfLocalManagedObject->setPvlanModeTrunkPromis(true);
							trace(TRACE_LEVEL_INFO,string("IntfConfLocalW  NSMUPDATE_PHY_PVLAN_MODE_TRUNK_PROMISCUOUS "));
						} else if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_PVLAN_MODE_PROMISCUOUS){

							pPhyIntfLocalManagedObject->setPvlanModePromiscuous(true);
							trace(TRACE_LEVEL_INFO,string("IntfConfLocalW NSMUPDATE_PHY_PVLAN_MODE_PROMISCUOUS "));

						}else{
							pPhyIntfLocalManagedObject->setPvlanModeHost(true);
							trace(TRACE_LEVEL_INFO,string("IntfConfLocalW  NSMUPDATE_PHY_PVLAN_MODE_HOST"));

						}

							//PVLAN attributes
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModePromiscuous");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModeHost");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModeTrunkBas");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModeTrunkPromis");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanModeTrunkHost");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanNativeVlanId");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanAll");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanNone");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeAdd");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeRemove");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeExcept");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanHostPrimVlanId");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanHostSecVlanId");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanTrunkAssoc");
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanMapping");

	                     //sw trunk/access attributes
	                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("swMode");
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isAllowedVlanAll");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("isAllowedVlanNone");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeAdd");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeRemove");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("vlanRangeExcept");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated ("vlanObjectId");
                         pPhyIntfLocalManagedObject->addAttributeToBeUpdated("nativeVlanIntfConfig");
						 pPhyIntfLocalManagedObject->deleteMacAddressVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("macAddressVlanClassification");
						 pPhyIntfLocalManagedObject->deleteMacGroupVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("macGroupVlanClassification");
						 pPhyIntfLocalManagedObject->deleteCtagVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("ctagVlanClassification");
						 pPhyIntfLocalManagedObject->deleteCtagPVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("ctagPVlanClassification");
						 pPhyIntfLocalManagedObject->deleteRspanMacAddressVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_macaddress_vlan_classififcation");
						 pPhyIntfLocalManagedObject->deleteRspanMacGroupVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_macgroup_vlan_classififcation");
						 pPhyIntfLocalManagedObject->deleteRspanCtagVlanClassificationAll();
						 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("rspan_gvlan_trunk");
						break;

				 }

				 case NSMUPDATE_PHY_PVLAN_HOST_ASSOCIATION:
				 case NSMUPDATE_PHY_DELPVLAN_HOST_ASSOCIATION_ALL:
				 {
					 pPhyIntfLocalManagedObject->setPvlanHostPrimVlanId(pNsmUpdatePhyIntfMessage->getPvlanHostPrimVlanId());
					 pPhyIntfLocalManagedObject->setPvlanHostSecVlanId(pNsmUpdatePhyIntfMessage->getPvlanHostSecVlanId());
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanHostPrimVlanId");
					 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanHostSecVlanId");
					 trace(TRACE_LEVEL_INFO,string("IntfConfLocalWo NSMUPDATE_PHY_PVLAN_HOST_ASSOCIATION "));
					 break;
				 }

				 case NSMUPDATE_PHY_PVLAN_NATIVE_VLANID:
					{
						pPhyIntfLocalManagedObject->setPvlanNativeVlanId(pNsmUpdatePhyIntfMessage->getPvlanNativeVlanId());
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanNativeVlanId");
						break;
					}
				 case NSMUPDATE_PHY_PVLAN_NATIVE_VLANID_DISABLE:
					{
						pPhyIntfLocalManagedObject->setPvlanNativeVlanId(0);
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanNativeVlanId");
						break;
					}
				 case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ALL:
					{
						NSM_PLUG_INFO_S(string("sw private-vlan trunk allowed vlan all...."));

						pPhyIntfLocalManagedObject->setPvlanIsAllowedVlanNone(false);
						pPhyIntfLocalManagedObject->setPvlanRangeAdd(UI32Range(""));
						pPhyIntfLocalManagedObject->setPvlanRangeRemove(UI32Range(""));
						pPhyIntfLocalManagedObject->setPvlanRangeExcept(UI32Range(""));
						pPhyIntfLocalManagedObject->setPvlanIsAllowedVlanAll(true);

						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanAll");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanNone");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeAdd");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeRemove");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeExcept");
						break;
					}
				 case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_NONE:
					{
						NSM_PLUG_INFO_S(string("sw private-vlan trunk allowed vlan none...."));

						pPhyIntfLocalManagedObject->setPvlanIsAllowedVlanAll(false);
						pPhyIntfLocalManagedObject->setPvlanIsAllowedVlanNone(false);
						pPhyIntfLocalManagedObject->setPvlanRangeAdd(UI32Range(""));
						pPhyIntfLocalManagedObject->setPvlanRangeRemove(UI32Range(""));
						pPhyIntfLocalManagedObject->setPvlanRangeExcept(UI32Range(""));

						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanAll");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanNone");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeAdd");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeRemove");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeExcept");

						break;
					}
				 case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ADD:
					{
						 NSM_PLUG_INFO_S(string("sw private-vlan trunk allowed vlan add...."));

						 vector<UI32>::iterator msgiter;
						 vector <UI32> msgVlanRangeVector;
						 UI32Range msgVlanrange = UI32Range("");

						 msgVlanrange = pNsmUpdatePhyIntfMessage->getPvlanRange();
						 msgVlanrange.getUI32RangeVector(msgVlanRangeVector);
						 msgVlanrange = UI32Range("");

						 if (pPhyIntfLocalManagedObject->getPvlanIsAllowedVlanAll()) {
						 	/*Check the remove list*/
							 UI32Range moRemoveVlanrange = UI32Range("");
							 vector <UI32> moRemoveVlanRangeVector;
							 vector<UI32>::iterator moremiter;

							 moRemoveVlanrange = pPhyIntfLocalManagedObject->getPvlanRangeRemove();
							 moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
							 moRemoveVlanrange = UI32Range("");

							 for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
							 {
								 for (moremiter=moRemoveVlanRangeVector.begin(); moremiter != moRemoveVlanRangeVector.end(); moremiter++)
								 {
									 if (*moremiter == *msgiter) {
									 	/* Found in remove list, so now erase the element from
										 * the remove vector*/
										 moRemoveVlanRangeVector.erase(moremiter);
										 break;
									 }

								 }

							 }

							 string vlanRemoveRangeStr =
								 UI32Range::getUI32RangeStringFromVector(moRemoveVlanRangeVector);
							 moRemoveVlanrange.fromString(vlanRemoveRangeStr); 
							 pPhyIntfLocalManagedObject->setPvlanRangeRemove(moRemoveVlanrange);
							 pPhyIntfLocalManagedObject->setPvlanRangeAdd(UI32Range(""));
							 pPhyIntfLocalManagedObject->setPvlanRangeExcept(UI32Range(""));

		                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeAdd");
		                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeRemove");
		                     pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeExcept");

						 } else {
							 vector <UI32> moVlanRangeVector;
							 UI32Range moVlanrange = UI32Range("");

							 moVlanrange = pPhyIntfLocalManagedObject->getPvlanRangeAdd();
							 moVlanrange.getUI32RangeVector(moVlanRangeVector);
							 moVlanrange = UI32Range("");

							 vector<UI32>::iterator moiter;
							 vector<UI32>::iterator moindex = moVlanRangeVector.begin();
							 bool isVlanIdPresent = true;

							 for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
							 {
								 for (moiter=moVlanRangeVector.begin(); moiter != moVlanRangeVector.end(); moiter++)
								 {
									 if(*moiter == *msgiter)
									 {
										 break;
									 }
									 else if(*moiter > *msgiter)/*Since this is a sorted vector, and we have already exceeded the required 
																  VLAN ID, we cannot find it in further parses, so break*/
									 {
										 moindex = moVlanRangeVector.begin();
										 isVlanIdPresent = false;
										 break;
									 }
								 }

								 if(!isVlanIdPresent)
									 while(*moindex < *msgiter)
										 moindex++;

								 if(moiter == moVlanRangeVector.end())
								 {
									 isVlanIdPresent = false;
									 moindex = moVlanRangeVector.end();
								 }

							     ResourceId  vlanstatus   = WAVE_MESSAGE_ERROR;

								 if(!isVlanIdPresent)/*Vlan not found in MO, add it at appropriate position to keep the vector sorted*/
								 {
								     UI32 vlan_id = *msgiter;
									 WaveManagedObjectSynchronousQueryContext vlanMOSyncQueryCtxt(VlanIntfManagedObject::getClassName());
									 vlanMOSyncQueryCtxt.addAndAttribute (new AttributeUI32(&vlan_id,"id"));
									 vector<WaveManagedObject *> *pVlanMOResults = querySynchronously (&vlanMOSyncQueryCtxt);
									 VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

									 if (NULL != pVlanMOResults) {
										 UI32 numberOfResults = pVlanMOResults->size ();
										 if (0 == numberOfResults) {
											 vlanstatus = WAVE_MESSAGE_ERROR;
										 }else if (1 == numberOfResults) {
											 pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pVlanMOResults)[0]);

											 prismAssert (NULL != pVlanIntfManagedObject, __FILE__, __LINE__);

											 vlanstatus = WAVE_MESSAGE_SUCCESS;
										 } else {
											 vlanstatus = WAVE_MESSAGE_ERROR;
										 }
										 if(pDceSynchronousLinearSeqContext)
										 	pDceSynchronousLinearSeqContext->addManagedObjectsForGarbageCollection(*pVlanMOResults);
										 pVlanMOResults->clear();
										 delete pVlanMOResults;
									 }
								 }
									
								 if(vlanstatus == WAVE_MESSAGE_SUCCESS) {
						         	moVlanRangeVector.insert(moindex, *msgiter);
								 }

								 moindex = moVlanRangeVector.begin();
								 isVlanIdPresent = true;
 							 }

							 string vlanRangeStr = UI32Range::getUI32RangeStringFromVector(moVlanRangeVector);
							 NSM_PLUG_INFO_S(string
								("sw private-vlan trunk allowed vlan add: add-str:")+ vlanRangeStr);
							 
							 moVlanrange.fromString(vlanRangeStr); 
							 pPhyIntfLocalManagedObject->setPvlanRangeAdd(moVlanrange);
							 pPhyIntfLocalManagedObject->setPvlanRangeRemove(UI32Range(""));
							 pPhyIntfLocalManagedObject->setPvlanRangeExcept(UI32Range(""));
							 pPhyIntfLocalManagedObject->setPvlanIsAllowedVlanNone(false);

							 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanNone");
							 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeAdd");
							 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeRemove");
							 pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeExcept");

						 }
						 break;
					 }
				 case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_REMOVE:
					{
						NSM_PLUG_INFO_S(string("sw private-vlan trunk allowed vlan remove...."));
							
						vector<UI32>::iterator moindex;
						vector<UI32>::iterator msgiter;
						vector <UI32> msgVlanRangeVector;
						UI32Range msgVlanrange = UI32Range("");
						bool isVlanIdPresent = true;
						VlanIdList vlist;

						msgVlanrange = pNsmUpdatePhyIntfMessage->getPvlanRange();
						msgVlanrange.getUI32RangeVector(msgVlanRangeVector);
						msgVlanrange = UI32Range("");

							if (pPhyIntfLocalManagedObject->getPvlanIsAllowedVlanAll()) {
								/*Check the remove list*/
								UI32Range moRemoveVlanrange = UI32Range("");
								vector <UI32> moRemoveVlanRangeVector;
								vector<UI32>::iterator moremiter;

								moindex = moRemoveVlanRangeVector.begin();
								moRemoveVlanrange = pPhyIntfLocalManagedObject->getPvlanRangeRemove();
								moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
								moRemoveVlanrange = UI32Range("");

								for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
								{
									for (moremiter=moRemoveVlanRangeVector.begin(); moremiter != moRemoveVlanRangeVector.end(); moremiter++)
									{
										if (*moremiter == *msgiter) {
											/*Found in remove list, so do nothing*/
											break;
										} else if (*moremiter > *msgiter) {
											/* Since this is a sorted vector, and we have already
											 * exceeded the required VLAN ID, we cannot find it in
											 * further parses, so break*/
											moindex = moRemoveVlanRangeVector.begin();
											isVlanIdPresent = false;
											break;
										}
									}

									if (!isVlanIdPresent)
										while (*moindex < *msgiter)
											moindex++;

									if (moremiter == moRemoveVlanRangeVector.end()) {
										isVlanIdPresent = false;
										moindex = moRemoveVlanRangeVector.end();
									}

									if (!isVlanIdPresent)
										/* Vlan not found in MO, add it at appropriate position to
										 * keep the vector sorted*/
										moRemoveVlanRangeVector.insert(moindex, *msgiter);

									moindex = moRemoveVlanRangeVector.begin();
									isVlanIdPresent = true;
								}

								string vlanRemoveRangeStr =
								UI32Range::getUI32RangeStringFromVector(moRemoveVlanRangeVector);
								moRemoveVlanrange.fromString(vlanRemoveRangeStr); 
								pPhyIntfLocalManagedObject->setPvlanRangeRemove(moRemoveVlanrange);
								pPhyIntfLocalManagedObject->setPvlanRangeAdd(UI32Range(""));
								pPhyIntfLocalManagedObject->setPvlanRangeExcept(UI32Range(""));
								
		                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeAdd");
		                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeRemove");
		                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeExcept");

		                        vlist.m_vlanIds = moRemoveVlanRangeVector;
		                        vlist.m_type = NONMEMBER_VLAN_LIST;
							} else {

								vector <UI32> moVlanRangeVector;
								UI32Range moVlanrange = UI32Range("");

								moVlanrange = pPhyIntfLocalManagedObject->getPvlanRangeAdd();
								moVlanrange.getUI32RangeVector(moVlanRangeVector);
								moVlanrange = UI32Range("");

								vector<UI32>::iterator moiter;
								moindex = moVlanRangeVector.begin();

								for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
								{
									for (moiter=moVlanRangeVector.begin(); moiter != moVlanRangeVector.end(); moiter++)
									{
										if (*moiter == *msgiter) {
											/*Vlan Id exists, remove from vlan list*/
											moVlanRangeVector.erase(moiter); 
											break;
										}
									}
								}

								string vlanRangeStr =
									UI32Range::getUI32RangeStringFromVector(moVlanRangeVector);
								moVlanrange.fromString(vlanRangeStr); 
								pPhyIntfLocalManagedObject->setPvlanRangeAdd(moVlanrange);
								pPhyIntfLocalManagedObject->setPvlanRangeRemove(UI32Range(""));
								pPhyIntfLocalManagedObject->setPvlanRangeExcept(UI32Range(""));

		                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeAdd");
		                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeRemove");
		                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeExcept");

								vlist.m_vlanIds = moVlanRangeVector;
								if (pPhyIntfLocalManagedObject->m_pVlanNativeVlanId > 0) {
								    vlist.m_vlanIds.push_back(pPhyIntfLocalManagedObject->m_pVlanNativeVlanId);
								}
							}

							INTF_XSTP_LOCAL_WORKER->handleIntfVlanAssocUpdate(
							        pPhyIntfLocalManagedObject, vlist, pDceSynchronousLinearSeqContext, true);
							break;
					}
				 case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_EXCEPT:
					{
						NSM_PLUG_INFO_S(string("sw private-vlan trunk allowed vlan except...."));
						
						UI32Range msgVlanrange = UI32Range("");

						msgVlanrange = pNsmUpdatePhyIntfMessage->getPvlanRange();

						VlanIdList vlist (msgVlanrange, NONMEMBER_VLAN_LIST);
                        INTF_XSTP_LOCAL_WORKER->handleIntfVlanAssocUpdate(
                                pPhyIntfLocalManagedObject, vlist, pDceSynchronousLinearSeqContext, true);

						pPhyIntfLocalManagedObject->setPvlanIsAllowedVlanAll(true);
						pPhyIntfLocalManagedObject->setPvlanIsAllowedVlanNone(false);
						pPhyIntfLocalManagedObject->setPvlanRangeRemove(msgVlanrange);
						pPhyIntfLocalManagedObject->setPvlanRangeAdd(UI32Range(""));
						pPhyIntfLocalManagedObject->setPvlanRangeExcept(UI32Range(""));

						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanAll");
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanIsAllowedVlanNone");
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeAdd");
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeRemove");
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanRangeExcept");
						break;
					}
				 default:
					 break;
			 }
             if (ifUpdateRequired) {
                 updateWaveManagedObject (pPhyIntfLocalManagedObject);
             }
		}

		NSM_PLUG_INFO_S( string ("UP_PHY_L Updating Physical Interface MO Status : ") + ifName + string("Cmdcode :") + pNsmUpdatePhyIntfMessage->getCmdCode() + string("status :") + pNsmUpdatePhyIntfMessage->getCompletionStatus());

        return pNsmUpdatePhyIntfMessage->getCompletionStatus ();
    }

    ResourceId IntfConfLocalWorker::replyConfigPhyIntfMO(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject, CfgReplayModes CfgReplayMode)
    {
        NSM_PLUG_DBG("Entering IntfConfLocalWorker::replyConfigPhyIntfMO ..");

        string replayModeStr = "CFGREPLAY_DEFAULT";
        switch (CfgReplayMode) {
            case CFGREPLAY_POSTBOOT:
                replayModeStr = "CFGREPLAY_POSTBOOT";
                break;
            case CFGREPLAY_ISL_TO_EDGE:
                replayModeStr = "CFGREPLAY_ISL_TO_EDGE";
                break;
            default:
                break;
        }

        int status, count = 0;
        ObjectId poObjectId = pPhyIntfLocalManagedObject->getPoId();

       status = (NsmLocalObjectManager::getInstance())->Pass1PhyConfiguration(pPhyIntfLocalManagedObject,CfgReplayMode);
	   /* Replay config iff all member ports are ISL. Even atleast one member
		* port is EDGE, donot replay the config*/
	   if(poObjectId != ObjectId::NullObjectId)
	   {
	   		//Get no.of edge  member ports of PO of this location ID
		   count =	NsmUtils::queryPoEdgeMembersCountForLocationId (poObjectId,
									   NsmUtils::getLocationId(pPhyIntfLocalManagedObject->getIfName()));

           // We have to replay PO configs when (count == 0) or when lineCard postboot happened (NOT complete postboot)
           // When slot is powered-off interfaces are deleted from backend which can delete PO also if one or more interfaces from this slot are member of PO.
           // when channel-group is replayed for interface, it will create new PO in backend. So we have replay PO configs again. To avoid multiple replay of
           // same PO, set is used.
           bool isPOAlreadyReplayed = (m_portchannelReplayedSet.find(poObjectId) == m_portchannelReplayedSet.end());

			NSM_PLUG_INFO_S( string("isGlobalPortChannelReplayDone ")+ (((m_isGlobalPortChannelReplayDone) ? "TRUE" : "FALSE")) + string("count = ") + count + string(" replayMode = ") + replayModeStr);
            NSM_PLUG_INFO_S(string("Is Slot Level Port-channel already replayed = ") + ((isPOAlreadyReplayed) ? "False" : "True"));
		   if((0 == count) || ((CFGREPLAY_POSTBOOT == CfgReplayMode) && (!m_isGlobalPortChannelReplayDone) && (isPOAlreadyReplayed)))
		   {
			   /*Replay PO config*/
			   sendPoConfigToLocal(NsmUtils::getPoIntfNameByObjectId(poObjectId));
               if (CFGREPLAY_POSTBOOT == CfgReplayMode) {
                   m_portchannelReplayedSet.insert (poObjectId);

                   // Maintaining map of slotId and  PO objectId to facilitate cleanup of m_portchannelReplayedSet
                   // whenever slot is powered off/on.
                   UI32 slotId = NsmUtils::getSlotId(pPhyIntfLocalManagedObject->getIfName());
                   (m_slotIdPortChannelOidMap[slotId]).push_back(poObjectId);
               }
		   }
	   }
       // VLAG postboot fix
       NSM_PLUG_INFO_S( string("File replay = ")+ (((FrameworkToolKit::getIsConfigFileReplayGoingToBeDone ()) ? "TRUE" : "FALSE")) + string("CfgReplayMode = ") + replayModeStr);
       if ( ( !( (CfgReplayMode == CFGREPLAY_POSTBOOT ) && (FrameworkToolKit::getIsConfigFileReplayGoingToBeDone ())) ) &&  !(CfgReplayMode == CFGREPLAY_ISL_TO_EDGE) )
        {
            status = (NsmLocalObjectManager::getInstance())->PassShutdownPhyConfiguration(pPhyIntfLocalManagedObject);
            NSM_PLUG_INFO_S( string ("Replayed Shutdown configuration for PhyIntf: ") + pPhyIntfLocalManagedObject->getIfName());
        } else {
            NSM_PLUG_INFO_S( string (" PhysicalInterface Shutdown Cmd will not be replayedback to backend because configReply going to be execute...") + string ("getIsConfigFileReplayGoingToBeDone :: ") + ((FrameworkToolKit::getIsConfigFileReplayGoingToBeDone ()) ? "TRUE" : "FALSE"));
        }

       (NsmLocalObjectManager::getInstance())->configReplayPhyQosMaps(pPhyIntfLocalManagedObject);
       (NsmLocalObjectManager::getInstance())->configReplayPhyQosPfc(pPhyIntfLocalManagedObject);
	   (NsmLocalObjectManager::getInstance())->configReplayPhyQosCosThresholds(pPhyIntfLocalManagedObject);
       (NsmLocalObjectManager::getInstance())->configReplayPhyQosCosProfile(pPhyIntfLocalManagedObject);
	   //(NsmLocalObjectManager::getInstance())->configReplayPhyQosCosTailDrop(pPhyIntfLocalManagedObject);
	   //Storm control config replay.
	   SsmLocalObjectManager::ConfigReplayStormControlPerInterface(
			pPhyIntfLocalManagedObject);
       (NsmLocalObjectManager::getInstance())->configReplayPhyCee(pPhyIntfLocalManagedObject);
       (NsmLocalObjectManager::getInstance())->configReplayPhyPriorityTag(pPhyIntfLocalManagedObject);
       //(NsmLocalObjectManager::getInstance())->ConfigReplayAppmPortProfileConfig(pPhyIntfLocalManagedObject);
       (NsmLocalObjectManager::getInstance())->configReplayDot1xInteface(pPhyIntfLocalManagedObject);
	   if(CfgReplayMode != CFGREPLAY_ISL_TO_EDGE)
       (NsmLocalObjectManager::getInstance())->configReplaySflowInterface(pPhyIntfLocalManagedObject);
       (NsmLocalObjectManager::getInstance())->configReplayVlanClassifierGroupToPhy(pPhyIntfLocalManagedObject);
       (NsmLocalObjectManager::getInstance())->configReplayRmonCmds(pPhyIntfLocalManagedObject);
	   /*Nsm ConfiReplay*/
	   L3PostBootUtil::configReplayPhyIntfIpConfig(pPhyIntfLocalManagedObject);
       //(NsmLocalObjectManager::getInstance())->configReplayPhyIntfIpConfig(pPhyIntfLocalManagedObject);
       (NsmLocalObjectManager::getInstance())->configReplayXstpPortConfig(pPhyIntfLocalManagedObject);
       (NsmLocalObjectManager::getInstance())->configReplayXstpPortInstanceConfig(pPhyIntfLocalManagedObject);
       (NsmLocalObjectManager::getInstance())->configReplayXstpPortVlanConfig(pPhyIntfLocalManagedObject);
       (VRRPLocalObjectManager::getInstance())->configReplayVRRPPhyIntf(pPhyIntfLocalManagedObject);
       (NsmLocalObjectManager::getInstance())->configReplayPhyOSPFConfig(pPhyIntfLocalManagedObject);
       (IgmpLocalObjectManager::getInstance())->replayIgmpPhyConfig(pPhyIntfLocalManagedObject);
       (DhcpRelLocalObjectManager::getInstance())->replayDhcpRelPhyIntfConfig(pPhyIntfLocalManagedObject);
	   (PimLocalObjectManager::getInstance())->configReplayPimPhyIntf(pPhyIntfLocalManagedObject);

	/* replay bpdu-guard */
       (NsmLocalObjectManager::getInstance())->configReplayBpduGuardConfig(pPhyIntfLocalManagedObject,
																		   (PoIntfManagedObject *)NULL);
       (NsmLocalObjectManager::getInstance())->configReplayPortSecurity(pPhyIntfLocalManagedObject);

		if (CfgReplayMode != CFGREPLAY_ISL_TO_EDGE) {
			(NsmLocalObjectManager::getInstance())->postBootFcoeProvisioningConfigForInterface(pPhyIntfLocalManagedObject);
		}

       if (status == WAVE_MESSAGE_SUCCESS) {

            /* Trigger Lacp Config Replay*/
            (NsmLocalObjectManager::getInstance())->ConfigReplayPerIntfLacpConfig(pPhyIntfLocalManagedObject);

            /* Trigger Lldp Config Replay*/
            (NsmLocalObjectManager::getInstance())->ConfigReplayPerIntfLldpConfig(pPhyIntfLocalManagedObject);
            /* Trigger Span after Lldp config replay */
            trace (TRACE_LEVEL_DEBUG,"Calling NSM Span Config Replay");
            (NsmLocalObjectManager::getInstance())->configReplaySpanInterface(pPhyIntfLocalManagedObject);

        }
       else {
    	   trace (TRACE_LEVEL_DEBUG,"\nNSM Span Config Replay NOT called!!\n");
       }

       typedef DcmManagementInterfaceMessage* (*msg_factory)(PhyIntfLocalManagedObject*);
       vector< pair<string, msg_factory> > callbacks;

	   if (CfgReplayMode == CFGREPLAY_ISL_TO_EDGE) {

		 	 NSM_PLUG_INFO_S( string ("Doing L3 Postboot replay after ISL_TO_EDGE"));
			 L3PostBootUtil::triggerPostBootAfterIslToEdge(pPhyIntfLocalManagedObject);

		     callbacks.push_back(make_pair("ssm", SsmLocalObjectManager::ConfigReplayMacAclIntfAssoc));
             if(Capabilities::isCapabilityPresent(FEAT_CAPABILITY_POLICER)) {
                 PolicerLocalObjectManager::configReplayPhyPolicyMaps(
                         pPhyIntfLocalManagedObject, (PoIntfManagedObject*) NULL);
             } else {
                 tracePrintf(TRACE_LEVEL_INFO,"MQC not supported on this platform.");
             }

	   }

       // ***** inserting more messasge factory here as needed

       for (vector< pair<string, msg_factory> >::iterator it = callbacks.begin(); it != callbacks.end(); ++it) {
    	   DcmManagementInterfaceMessage* msg = (DcmManagementInterfaceMessage*)it->second(pPhyIntfLocalManagedObject);
    	   if (msg) {
    		   status = sendSynchronouslyToWaveClient (it->first,msg);
    		   delete msg;
    	   }
       }

	   (NsmLocalObjectManager::getInstance())->configReplayEldInteface(pPhyIntfLocalManagedObject);
       return status;
    }

// update PhyIntfType from MO
ResourceId IntfConfLocalWorker::getPhyIntfTypeFromMo(NsmUpdatePhyIntfMessage *pMessage,
    PrismSynchronousLinearSequencerContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    string ifName = pMessage->getIfName();
    WaveManagedObjectSynchronousQueryContext
        syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeString(&ifName, "ifName"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    PhyIntfLocalManagedObject *pPhyMo = NULL;

    if (NULL != pResults) {
        UI32 numberOfResults = pResults->size ();
        if (0 == numberOfResults) {
            status = WAVE_MESSAGE_ERROR;
        }else if (1 == numberOfResults) {
            pPhyMo = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
            status = WAVE_MESSAGE_SUCCESS;
        } else {
            status = WAVE_MESSAGE_ERROR;
        }
    }

    if (pPhyMo == NULL) {
        NSM_PLUG_DBG_ERR_S( string("getPhyIntfTypeFromMo() "
            "Physical Interface MO not found ..:") + ifName);
        return (WAVE_MESSAGE_ERROR);
    }
    if (pContext) {
        pContext-> addManagedObjectForGarbageCollection(pPhyMo);
    }
    pMessage->setIfType(pPhyMo->getPhyType());

  //  (NsmLocalObjectManager::getInstance())->configReplayEldInteface(pPhyMo);

    return status;
}

ResourceId IntfConfLocalWorker::postPhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
{
	NSM_PLUG_DBG("Entering ...");
	NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage = dynamic_cast<NsmUpdatePhyIntfMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());
	return postPhyIntf(pNsmUpdatePhyIntfMessage,pDceSynchronousLinearSeqContext);
}

ResourceId IntfConfLocalWorker::postPhyIntf(NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage, DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
	{
		NSM_PLUG_DBG("Entering ...");

		int status = WAVE_MESSAGE_SUCCESS;

		switch(pNsmUpdatePhyIntfMessage->getCmdCode())
		{
			case NSMUPDATE_PHY_ACTVIVE_STATUS:
			case NSM_PLUGIN_ISL_TO_EDGE:
				{
					if( NsmLocalObjectManager::getInstance()->getAllowIslToEdge()) {

						PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
						vector<string> selectFields;

						WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
						syncQueryCtxt.addAndAttribute (new AttributeString(pNsmUpdatePhyIntfMessage->getIfName(),"ifName"));
						selectFields.push_back("id");
						selectFields.push_back("ifIndex");
						selectFields.push_back("fcoeport");
						selectFields.push_back("Portrole");
						syncQueryCtxt.addSelectFields(selectFields);

						vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

						if (NULL != pResults) {
							UI32 numberOfResults = pResults->size ();
							if (0 == numberOfResults) {
								status = WAVE_MESSAGE_ERROR;
							} else if (1 == numberOfResults) {
								pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
								status = WAVE_MESSAGE_SUCCESS;
							} else {
								status = WAVE_MESSAGE_ERROR;
							}

							if (status == WAVE_MESSAGE_SUCCESS) {
								/* Call this interface after the database has been updated. */
       							(NsmLocalObjectManager::getInstance())->postBootFcoeProvisioningConfigForInterface(pPhyIntfLocalManagedObject);
							}
							WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
						}
					} else {
                        NSM_PLUG_INFO_S(string(" NsmLocalObjectManager::getAllowIslToEdge() is False."));
                    }
					break;
				}
			default:
				break;
		}
		return (status);
	}


ResourceId IntfConfLocalWorker::updatePhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
{
    NSM_PLUG_DBG("Entering ...");
    NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage = dynamic_cast<NsmUpdatePhyIntfMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());
    return updatePhyIntf(pNsmUpdatePhyIntfMessage,pDceSynchronousLinearSeqContext);
}

ResourceId IntfConfLocalWorker::updatePhyIntf(NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage, DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        NSM_PLUG_DBG("Entering ...");

		int send_native_vlan_disable_to_eld_flag = 0;
        int status;
        string ifName = pNsmUpdatePhyIntfMessage->getIfName();
        UI32Range toBeProvisionedVlanRange;
        if(true  == NsmUtils::isValidThreeTuple(ifName) ) {
        	string twoTuppleIfName;
			NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
			ifName = twoTuppleIfName;
		}

        status = getPhyIntfTypeFromMo(pNsmUpdatePhyIntfMessage,
            pDceSynchronousLinearSeqContext);

        if (status == WAVE_MESSAGE_SUCCESS)
        {
            NSM_PLUG_INFO_S(string(" updatePhyIntf(), getPhyIntfTypeFromMo ") +
                "finished. ifName: " + ifName +
                " iftype=" +  pNsmUpdatePhyIntfMessage->getIfType() +
                " return=" + status + "\n");
        }
        else {
            NSM_PLUG_INFO_S(string(" updatePhyIntf(), getPhyIntfTypeFromMo ") +
                "failed. ifName: " + ifName +
                " iftype=" +  pNsmUpdatePhyIntfMessage->getIfType() +
                " return=" + status + "\n");
            return status;
        }

        //TODO : Use APPMUtils instead.
        PORT_BASED_PROVISION(pDceSynchronousLinearSeqContext,
                pNsmUpdatePhyIntfMessage->getCmdCode(),
                pNsmUpdatePhyIntfMessage->getIsPortProfiled());

        auto_ptr<NsmPhyIntfMessage> m ( new NsmPhyIntfMessage(ifName,
                pNsmUpdatePhyIntfMessage->getSwIfIndex(),
                pNsmUpdatePhyIntfMessage->getMtu(),
                pNsmUpdatePhyIntfMessage->getOpenflow(),
                pNsmUpdatePhyIntfMessage->getDesc(),
                pNsmUpdatePhyIntfMessage->getFlag(),
                pNsmUpdatePhyIntfMessage->getSwModeFlag(),
                pNsmUpdatePhyIntfMessage->getSwMode(),
                pNsmUpdatePhyIntfMessage->getIsAllowedVlanAll(),
                pNsmUpdatePhyIntfMessage->getCmdCode(),
                pNsmUpdatePhyIntfMessage->getLacpTimeout(),
                pNsmUpdatePhyIntfMessage->getLacpPortPriority(),
                pNsmUpdatePhyIntfMessage->getLineSpeed(),
                pNsmUpdatePhyIntfMessage->getNativeVlanId(), "",
                pNsmUpdatePhyIntfMessage->getEnable(),
                pNsmUpdatePhyIntfMessage->getPollingInterval(),
                pNsmUpdatePhyIntfMessage->getSampleRate(),
                pNsmUpdatePhyIntfMessage->getIsPortProfiled(),
                pNsmUpdatePhyIntfMessage->getMsgId(),
                pNsmUpdatePhyIntfMessage->getException(),
                pNsmUpdatePhyIntfMessage->getPvlanNativeVlanId(),
                pNsmUpdatePhyIntfMessage->getPvlanIsAllowedVlanAll()));

        switch(pNsmUpdatePhyIntfMessage->getCmdCode())
        {
            case NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT:
                {
                    string portProfileName = pNsmUpdatePhyIntfMessage->getPortProfileName();
                    vector<string> portProfiles;
                    portProfiles.push_back(portProfileName);
                    string portProfileDomainName = pNsmUpdatePhyIntfMessage->getPortProfileDomainName();

                    tracePrintf(TRACE_LEVEL_INFO, "NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT or NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT port-profile <%s>", portProfileName.c_str()); 
                    tracePrintf(TRACE_LEVEL_INFO, "NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT or NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT port-profile-domain <%s>", portProfileDomainName.c_str());

                    status = getAllVlansToBeProvisioned(portProfiles, portProfileDomainName, toBeProvisionedVlanRange);

                    tracePrintf(TRACE_LEVEL_INFO, "NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT or "
                        "NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT, toBeProvisionedVlanRange<%s>"
                        , toBeProvisionedVlanRange.toString().c_str() );
                    //Better write a util in APPM and use it here, the same can be
                    //reused to populate all the vlans present in a given pp domain.
                    PROVISION_VLAN_RANGE(pDceSynchronousLinearSeqContext,toBeProvisionedVlanRange,PROVISION_SPECIFIED);
                    m->setPortProfileName(portProfileName);
                    m->setPortProfileDomainName(portProfileDomainName);
                    break;
                }
                case NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT : 
                {
                    string portProfileName = pNsmUpdatePhyIntfMessage->getPortProfileName();
                    vector<string> portProfiles;
                    string portProfileDomainName = pNsmUpdatePhyIntfMessage->getPortProfileDomainName();

                    tracePrintf(TRACE_LEVEL_INFO, "NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT or NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT port-profile <%s>", portProfileName.c_str());
                    tracePrintf(TRACE_LEVEL_INFO, "NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT or NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT port-profile-domain <%s>", portProfileDomainName.c_str());

                    status = getAllVlansToBeProvisioned(portProfiles,portProfileDomainName, toBeProvisionedVlanRange);

                    tracePrintf(TRACE_LEVEL_INFO, "NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT or "
                            "NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT, toBeProvisionedVlanRange<%s>"
                            , toBeProvisionedVlanRange.toString().c_str() );
                    //Better write a util in APPM and use it here, the same can be
                    //reused to populate all the vlans present in a given pp domain.
                    PROVISION_VLAN_RANGE(pDceSynchronousLinearSeqContext,toBeProvisionedVlanRange,PROVISION_SPECIFIED);
                    m->setPortProfileName(portProfileName);
                    m->setPortProfileDomainName(portProfileDomainName);
                    break;
                }
            case NSMUPDATE_DISSOCIATE_PORT_PROFILE_WITH_PHY_PORT :
            case NSMUPDATE_DISSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT :
                {
                    m->setPortProfileName(pNsmUpdatePhyIntfMessage->getPortProfileName());
                    m->setPortProfileDomainName(pNsmUpdatePhyIntfMessage->getPortProfileDomainName());
                    break;
                }
        }

        trace(TRACE_LEVEL_INFO,string("IntfConfLocalWo updatePhyIntf ** "));
		m->setNativevlanDisable(pNsmUpdatePhyIntfMessage->getNativevlanDisable());
		m->setIfType(pNsmUpdatePhyIntfMessage->getIfType());

		EldClientUpdatePhyIntfMessage *K1 = new EldClientUpdatePhyIntfMessage();

        //Below message is used for port-profile-port command on physical interface.
        NsmPhyIntfMessage *mSsm = new NsmPhyIntfMessage();
        mSsm->setCmdCode(pNsmUpdatePhyIntfMessage->getCmdCode());
        mSsm->setIsPortProfiled(pNsmUpdatePhyIntfMessage->getIsPortProfiled());
        mSsm->setIfName(ifName);
		mSsm->setIfType(pNsmUpdatePhyIntfMessage->getIfType());
        mSsm->setPortProfileName(pNsmUpdatePhyIntfMessage->getPortProfileName());
        mSsm->setPortProfileDomainName(pNsmUpdatePhyIntfMessage->getPortProfileDomainName());

        trace(
                TRACE_LEVEL_INFO,
                string(
                        "IntfConfLocalWorker::UpdatePhyIntf UP_PHY_L Sending to backend : ")
                        + ifName + string("Cmdcode :")
                        + pNsmUpdatePhyIntfMessage->getCmdCode()
                        + string("IfType: ")
                        + pNsmUpdatePhyIntfMessage->getIfType());


        switch(pNsmUpdatePhyIntfMessage->getCmdCode())
        {
			case NSMUPDATE_PHY_TIMEOUT :
                 status = sendSynchronouslyToWaveClient ("lacp",m.get());
				 if (status != WAVE_MESSAGE_SUCCESS) {
					NSM_PLUG_INFO_S(string("LACP:: Timeout: Send To Daemon, Message-Status=[") +
                                    status + string("]:") +
                                    FrameworkToolKit::localize((status)));
				 } else if (m->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
					NSM_PLUG_INFO_S(string("LACP:: Timeout: Send To Daemon, Client-Status=[") +
                                    m->getClientStatus() + string("]:") +
									FrameworkToolKit::localize(m->getClientStatus()));
					status = m->getClientStatus();
					pNsmUpdatePhyIntfMessage->setCompletionStatus(status);
                    if (mSsm) delete mSsm;
					if(K1) delete K1; 
					return status;
				 }
				 break;

			case NSMUPDATE_PHY_PORT_PRIORITY :
                 status = sendSynchronouslyToWaveClient ("lacp", m.get());
				 if (status != WAVE_MESSAGE_SUCCESS) {
					 NSM_PLUG_INFO_S(string("LACP:: Port prio: Send To Daemon, Message-Status=[") +
						 status + string("]:") +
						 FrameworkToolKit::localize((status)));
				 } else if (m->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
					 NSM_PLUG_INFO_S(string("LACP:: Port prio: Send To Daemon, Client-Status=[") +
						 m->getClientStatus() + string("]:") +
						 FrameworkToolKit::localize(m->getClientStatus()));
					 status = m->getClientStatus();
					 pNsmUpdatePhyIntfMessage->setCompletionStatus(status);
					if(mSsm) delete mSsm;
					if(K1) delete K1; 
					return status;
				 }
				 break;

			case NSMUPDATE_PHY_LACP_DEFAULT_UP:
				 m->setLacpDefaultUp(pNsmUpdatePhyIntfMessage->getLacpDefaultUp());
				 NSM_PLUG_INFO_S( string ("Going to call lacp") );
                 status = sendSynchronouslyToWaveClient ("lacp", m.get());
				 break;
		 	case NSMUPDATE_PHY_SFLOW_ENABLE :
                     status = sendSynchronouslyToWaveClient ("Sflow", m.get());
			     break;
		 	case NSMUPDATE_PHY_SFLOW_POLLING_INTERVAL :
                     status = sendSynchronouslyToWaveClient ("Sflow", m.get());
			     break;
		 	case NSMUPDATE_PHY_SFLOW_SAMPLE_RATE :
                     status = sendSynchronouslyToWaveClient ("Sflow", m.get());
			     break;
		 	case NSMUPDATE_PHY_DELSFLOW_ENABLE :
                     status = sendSynchronouslyToWaveClient ("Sflow", m.get());
			     break;
		 	case NSMUPDATE_PHY_DELSFLOW_POLLING_INTERVAL :
                     status = sendSynchronouslyToWaveClient ("Sflow", m.get());
			     break;
		 	case NSMUPDATE_PHY_DELSFLOW_SAMPLE_RATE :
                     status = sendSynchronouslyToWaveClient ("Sflow", m.get());
			     break;
            case NSMUPDATE_PHY_PORT_PROFILED :
            case NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT :
            case NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT :
            case NSMUPDATE_DISSOCIATE_PORT_PROFILE_WITH_PHY_PORT :
            case NSMUPDATE_DISSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT :
                 {
                     if( 0 == (string("true").compare(pDceSynchronousLinearSeqContext->getCache()->getStringValue(PORT_ROLE)))
                             && (!(pNsmUpdatePhyIntfMessage->getIsPortProfiled())
                                 || (NSMUPDATE_DISSOCIATE_PORT_PROFILE_WITH_PHY_PORT == pNsmUpdatePhyIntfMessage->getCmdCode())
                                 || (NSMUPDATE_DISSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT == pNsmUpdatePhyIntfMessage->getCmdCode()) )) {

                         return WAVE_MESSAGE_SUCCESS;
                     }

                     trace(TRACE_LEVEL_INFO, string("NSMLocal sending port-profile-port message OpCode<")
                                 + " CmdCode<" + mSsm->getCmdCode() + "> InterfaceType<" + mSsm->getIfType()
                                 + "> IfName<"  + mSsm->getIfName() + "> isPortProfiled<" + mSsm->getIsPortProfiled()
                                 + "> port-profile-name <" + mSsm->getPortProfileName() + "> port-profile-domain name<"
                                 + mSsm->getPortProfileDomainName() + "> to qosd");

                 status = sendSynchronouslyToWaveClient ("qos",mSsm);
                     /*status = WAVE_MESSAGE_SUCCESS;
                     mSsm->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
                     mSsm->setClientStatus(WAVE_MESSAGE_SUCCESS);*/

                     trace(TRACE_LEVEL_INFO, string("NSMLocal received  port-profile-port response from qosd status <")
                             + FrameworkToolKit::localize(status) + "> completion status<" + FrameworkToolKit::localize(mSsm->getCompletionStatus())
                                 + "> client status<" + FrameworkToolKit::localize(mSsm->getClientStatus())+ ">");

                     if(status == WAVE_MESSAGE_SUCCESS) {
                         /*If Interface has Non-Default Qos config, then send the message to Nsm as well.*/
                         status = mSsm->getClientStatus();
                         trace(TRACE_LEVEL_INFO, string("NSMLocal received port-profile-port response from qosd, status: ")
                                 + mSsm->getClientStatus());
                         if (status == WAVE_MESSAGE_SUCCESS) {
                             trace(TRACE_LEVEL_INFO, string("NSMLocal sending port-profile-port message")
                                         + " CmdCode<" + m->getCmdCode() + "> InterfaceType<" + m->getIfType()
                                         + "> IfName<"  + m->getIfName() + "> isPortProfiled<" + m->getIsPortProfiled()
                                         + "> port-profile-name <" + m->getPortProfileName() + "> port-profile-domain name<"
                                         + m->getPortProfileDomainName() + "> to nsmd");

                             status = sendSynchronouslyToWaveClient ("nsm", m.get());
                             /*status = WAVE_MESSAGE_SUCCESS;
                             m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
                             m->setClientStatus(WAVE_MESSAGE_SUCCESS);*/

                             trace(TRACE_LEVEL_INFO, string("NSMLocal received  port-profile-port response from nsmd status <")
                                     + FrameworkToolKit::localize(status) + "> completion status<" + FrameworkToolKit::localize(m->getCompletionStatus())
                                         + "> client status<" + FrameworkToolKit::localize(m->getClientStatus())+ ">");

                         } else {
                             trace(TRACE_LEVEL_INFO, string("NSMLocal received port-profile-port response from qosd with an error<")
                                     + FrameworkToolKit::localize(status) + ">");
                             if (mSsm) delete mSsm;
                             if(K1) delete K1;
                             return status;
                         }
                     }
                     break;


/*


                 if( 0 == (string("true").compare(pDceSynchronousLinearSeqContext->getCache()->getStringValue(PORT_ROLE)))
                     && (!(pNsmUpdatePhyIntfMessage->getIsPortProfiled()) 
                            || (NSMUPDATE_DISSOCIATE_PORT_PROFILE_WITH_PHY_PORT == pNsmUpdatePhyIntfMessage->getCmdCode()) 
                            || (NSMUPDATE_DISSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT == pNsmUpdatePhyIntfMessage->getCmdCode()) )) {

                     return WAVE_MESSAGE_SUCCESS;
                 }
				  status = mSsm->getClientStatus();
				  trace(TRACE_LEVEL_INFO, string("client status: ") + mSsm->getClientStatus());
				  trace(TRACE_LEVEL_INFO, string("WAVE_MESSAGE_SUCCESS: ") + WaveNs::WRC_WAVE_MESSAGE_SUCCESS);
				  if (status == WaveNs::WRC_WAVE_MESSAGE_SUCCESS) {
					  trace(TRACE_LEVEL_INFO, string("APPM Sending Profile-port to NSM\n"));
				  } else {
					  trace(TRACE_LEVEL_INFO, string("APPM Sending Profile-port QoS Returned Error\n"));
					   //bpatel : ADded below lines to avoid memory leak
					  if (mSsm) delete mSsm;
					  if(K1) delete K1;
					  return status;
				  }
                 break; */
            }
            case NSM_PLUGIN_EDGE_TO_ISL:

                    /* Kunjan I did it since we should not send back to the backend*/
                 m->setMsgId(pNsmUpdatePhyIntfMessage->getMsgId());
                 m->setCmdCode(NSM_PLUGIN_EDGE_TO_ISL_RESP);
                 status = sendSynchronouslyToWaveClient("nsm", m.get());
                 break;
            case NSMUPDATE_PHY_ACTVIVE_STATUS:
            case NSM_PLUGIN_ISL_TO_EDGE:
                 {
                     trace(TRACE_LEVEL_DEBUG, string("case: NSMUPDATE_PHY_ACTVIVE_STATUS/NSM_PLUGIN_ISL_TO_EDGE\n"));
                     m->setMsgId(pNsmUpdatePhyIntfMessage->getMsgId());
                     m->setCmdCode(NSM_PLUGIN_ISL_TO_EDGE_RESP);
                     status = sendSynchronouslyToWaveClient("nsm", m.get());

                     if( NsmLocalObjectManager::getInstance()->getAllowIslToEdge())
                     {
                         PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
                         WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
                         syncQueryCtxt.addAndAttribute (new AttributeString(pNsmUpdatePhyIntfMessage->getIfName(),"ifName"));
                         vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

                         if (NULL != pResults) {
                             UI32 numberOfResults = pResults->size ();
                             if (0 == numberOfResults) {
                                 status = WAVE_MESSAGE_ERROR;
                             }else if (1 == numberOfResults) {
                                 pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                                 status = WAVE_MESSAGE_SUCCESS;
                             } else {
                                 status = WAVE_MESSAGE_ERROR;
                             }
                             if(status == WAVE_MESSAGE_SUCCESS) {
                                     replyConfigPhyIntfMO(pPhyIntfLocalManagedObject, CFGREPLAY_ISL_TO_EDGE);
                             }
                             WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                         }
                     } else {
                        NSM_PLUG_INFO_S(string(" NsmLocalObjectManager::getAllowIslToEdge() is False."));
                     }
                     break;
                 }

            case NSMUPDATE_PHY_VEPA:
                 m->setVepa(pNsmUpdatePhyIntfMessage->getVepa());
                 status = sendSynchronouslyToWaveClient ("nsm", m.get());
                 NSM_PLUG_INFO_S( string ("VEPA - ifName :") + ifName + string(" Cmdcode (NSMUPDATE_PHY_VEPA) :") + pNsmUpdatePhyIntfMessage->getCmdCode());
                 break;
            case ELD_INTF_CONF_PRIORITY:
                 K1->setOpCode(pNsmUpdatePhyIntfMessage->getCmdCode());
                 K1->setPrio(pNsmUpdatePhyIntfMessage->getEldPrioVal());
                 K1->setIfName (ifName);
				 K1->setIfType(pNsmUpdatePhyIntfMessage->getIfType());
				 NSM_PLUG_INFO_S( string ("ELD - ifType :") + K1->getIfType());
                 NSM_PLUG_INFO_S( string ("ELD - ifName :") + K1->getIfName() + string(" Cmdcode :") +  K1->getOpCode() + (" ELD - PRIO :") + K1->getPrio());
                 break;

            case ELD_INTF_CONF_ELD_ENABLE:
                 if (NsmUtils::isAllUI32RangeVlanConfigured(pNsmUpdatePhyIntfMessage->getVlanIdRange(),  pNsmUpdatePhyIntfMessage->m_vlanid) == WAVE_MESSAGE_ERROR) {
                     trace (TRACE_LEVEL_ERROR, "IntfConfLocalWorker::updatePhyIntf:  one or more vlan doesn't exist   ");
					if (mSsm) delete mSsm;
					if(K1) delete K1;
					 return (WRC_NSM_API_ERR_VLAN_NOT_CONFIGURED);
                 }
                 K1->setOpCode(pNsmUpdatePhyIntfMessage->getCmdCode());
                 K1->setVlanidList(pNsmUpdatePhyIntfMessage->getVlanidList());
                 K1->setIfName (ifName);
				 K1->setIfType(pNsmUpdatePhyIntfMessage->getIfType());
				 NSM_PLUG_INFO_S( string ("ELD - ifType :") + K1->getIfType());
                 NSM_PLUG_INFO_S( string ("ELD - ifName :") + K1->getIfName() + string(" Cmdcode :") +  K1->getOpCode() + (" ELD - PRIO :") + K1->getPrio());
                 break;

              case ELD_INTF_CONF_ELD_DISABLE:
                 if (NsmUtils::isAllUI32RangeVlanConfigured(pNsmUpdatePhyIntfMessage->getVlanIdRange(),  pNsmUpdatePhyIntfMessage->m_vlanid) == WAVE_MESSAGE_ERROR) {
                     trace (TRACE_LEVEL_ERROR, "IntfConfLocalWorker::updatePhyIntf:  one or more vlan doesn't exist   ");
					if (mSsm) delete mSsm;
					if(K1) delete K1;
					 return (WRC_NSM_API_ERR_VLAN_NOT_CONFIGURED);
                 }
                 K1->setOpCode(pNsmUpdatePhyIntfMessage->getCmdCode());
                 K1->setVlanidList(pNsmUpdatePhyIntfMessage->getVlanidList());
                 K1->setIfName (ifName);
				 K1->setIfType(pNsmUpdatePhyIntfMessage->getIfType());
				 NSM_PLUG_INFO_S( string ("ELD - ifType :") + K1->getIfType());
                 NSM_PLUG_INFO_S( string ("ELD - ifName :") + K1->getIfName() + string(" Cmdcode :") +  K1->getOpCode() + (" ELD - PRIO :") + K1->getPrio());
                 break;

            case NSMUPDATE_PHY_EXCEPTION_STATUS:
                 NSM_PLUG_INFO_S( string ("UP_PHY_L : In Exception update case, Not Required to send backend ifName :") + ifName + string(" Cmdcode (NSMUPDATE_PHY_EXCEPTION_STATUS) :") + pNsmUpdatePhyIntfMessage->getCmdCode());
                 m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
                 m->setClientStatus(WAVE_MESSAGE_SUCCESS);
                 status = WAVE_MESSAGE_SUCCESS;
                 break;
		    case NSMUPDATE_PHY_NATIVEVLANDISABLE:
			{
				 /* ELD specific message replaying  for TR 374321*/
				 InterfaceType ifType = (InterfaceType)pNsmUpdatePhyIntfMessage->getIfType();
				 if (ifType == IF_TYPE_GI)
				 {
                 	 K1->setIfType(PHY_INT_GI);
				 }
				 else if (ifType == IF_TYPE_TE)
				 {
                 	 K1->setIfType(PHY_INT_TE);
				 }
                 else if (ifType == IF_TYPE_FO)
                 {
                     K1->setIfType(PHY_INT_FO);
                 }
    

                 K1->setOpCode(ELD_INTF_CONF_ELD_ENABLE);
				 K1->setIfName (ifName);
				 trace(TRACE_LEVEL_INFO, string ("NATIVE-VLAN - ifName :") + ifName);
				 /* 
				  * Retrieve the native-vlan id and put that into the message
				  */
				 UI32 native_vlanid = 0;
				 vector<UI32>    vlanIdVector;
				 WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
				 syncQueryCtxt.addAndAttribute (new AttributeString(pNsmUpdatePhyIntfMessage->getIfName(),"ifName"));
				 vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
				 PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

				 if (NULL != pResults) 
				 {
					 UI32 numberOfResults = pResults->size ();
					 if (0 == numberOfResults) 
					 {
						 status = WAVE_MESSAGE_ERROR;
					 }
					 else if (1 == numberOfResults)
					 {
						 pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
						 status = WAVE_MESSAGE_SUCCESS;
					 } 
					 else 
					 {
						 status = WAVE_MESSAGE_ERROR;
					 }
				 }

				 if (pPhyIntfLocalManagedObject != NULL)
				 {
					 NativeVlanIntfConfLocalManagedObject *pNativeVlanIntfConfLocalManagedObject = NULL;
					 pNativeVlanIntfConfLocalManagedObject = NativeVlanPhyGetChildMoByParentMo(pPhyIntfLocalManagedObject);

					 if(pNativeVlanIntfConfLocalManagedObject != NULL)
					 {

						 native_vlanid = pNativeVlanIntfConfLocalManagedObject->getNativeVlanId();
						 if (pDceSynchronousLinearSeqContext)
							 pDceSynchronousLinearSeqContext->addManagedObjectForGarbageCollection(pNativeVlanIntfConfLocalManagedObject);
						 vlanIdVector.push_back(native_vlanid);
						 K1->setVlanidList(vlanIdVector);

	                     // To check if ELD is configured at this native-vlan on this intf
	                     {
							 char buf[6] = {0};
	        				 string vlanId;
							 ObjectId pVlanIntfObjId = ObjectId::NullObjectId;
							 vector<ObjectId>::iterator search_native;

	                         vector<ObjectId>  m_vlanid = pPhyIntfLocalManagedObject->getVlanVector();
	                         trace(TRACE_LEVEL_INFO, string("m_vlanid size is") + m_vlanid.size());

							 if (m_vlanid.size() > 0)
							 {
								 sprintf(buf, "%d", native_vlanid);
								 vlanId = std::string(buf);
								 trace(TRACE_LEVEL_INFO, string("Check for ELD-status of native vlan ") + buf);
								 getVlanIntfObjectIdByVlanIfName(vlanId, pVlanIntfObjId, false, pDceSynchronousLinearSeqContext);

								 search_native = find(m_vlanid.begin(), m_vlanid.end(), pVlanIntfObjId);

								 if (search_native != m_vlanid.end())
								 {
	                             send_native_vlan_disable_to_eld_flag = 1;
						         trace(TRACE_LEVEL_INFO, string (
	                                "ELD will be enabled on Intf: ") + ifName +
										 " at native-vlan: " + native_vlanid);
	                         }
	                         else {
						         trace(TRACE_LEVEL_INFO, string ("Intf: ") + ifName +
										 " : ELD was not configured at native-vlan: " + native_vlanid);
								 }
	                         }
	                     }

						 trace(TRACE_LEVEL_INFO, string ("NATIVE-VLAN-ID :") + native_vlanid);

					 }

				 }
				 if(pResults)
				 {
					 if(pDceSynchronousLinearSeqContext)
						 pDceSynchronousLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
					 pResults->clear();
					 delete pResults;
				 }
                 status = sendSynchronouslyToWaveClient ("nsm", m.get());
				 break;
			}
 
            case NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN:
            case NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN:
            case NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN:
            case NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN:
            case NSMUPDATE_PHY_SW_ACCESS_ADD_RSPANVLAN:
            case NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN:
            case NSMUPDATE_PHY_SW_TRUNK_DEL_RSPANVLAN:
			{
				 if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN) {
					 NSM_PLUG_INFO_S("Update the cmd code to NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN");
					 m->setCmdCode(NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN);
				 }
				 else if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_TRUNK_DEL_RSPANVLAN){
					 NSM_PLUG_INFO_S("Update the cmd code to NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN");
					 m->setCmdCode(NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN);
				 }
				 else if(pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_SW_ACCESS_ADD_RSPANVLAN){
					  NSM_PLUG_INFO_S(string("Update the cmd code to NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN"));
					  m->setCmdCode(NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN);
				 }

				 UI32Range vlanRange = pNsmUpdatePhyIntfMessage->getVlanRange();
                 CMDCODE_BASED_PROVISION(pDceSynchronousLinearSeqContext,
                        pNsmUpdatePhyIntfMessage->getCmdCode(), vlanRange);
                 m->setVlanRange(vlanRange);
                 status = sendSynchronouslyToWaveClient ("nsm", m.get());
				 break;
			}
			case NSMUPDATE_PHY_SET_IPV6_RA_GUARD:
			case NSMUPDATE_PHY_RESET_IPV6_RA_GUARD:
			{
				m->setRaGuard(pNsmUpdatePhyIntfMessage->getRaGuard());
                status = sendSynchronouslyToWaveClient ("nsm", m.get());
				break;
			 }
			case NSMUPDATE_PHY_NATIVEVLANID:
			{
				PROVISION_VLAN(pDceSynchronousLinearSeqContext, pNsmUpdatePhyIntfMessage->getNativeVlanId());
                status = sendSynchronouslyToWaveClient ("nsm", m.get());
				break;
			}

			case NSMUPDATE_PHY_PVLAN_NATIVE_VLANID:
            {
                PROVISION_VLAN(pDceSynchronousLinearSeqContext,
                        pNsmUpdatePhyIntfMessage->getPvlanNativeVlanId());
            }
			case NSMUPDATE_PHY_PVLAN_NATIVE_VLANID_DISABLE:
			{
                 m->setPvlanNativeVlanId(pNsmUpdatePhyIntfMessage->getPvlanNativeVlanId());
                 status = sendSynchronouslyToWaveClient ("nsm", m.get());
                 break;
			}
			case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ALL:
			case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_NONE:
			{
                 m->setPvlanIsAllowedVlanAll(pNsmUpdatePhyIntfMessage->getPvlanIsAllowedVlanAll());
                 status = sendSynchronouslyToWaveClient ("nsm", m.get());
                 break;
			}
			case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ADD:
			case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_REMOVE:
			case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_EXCEPT:
			{
                UI32Range vlanRange = pNsmUpdatePhyIntfMessage->getPvlanRange();
                CMDCODE_BASED_PROVISION(pDceSynchronousLinearSeqContext,
                        pNsmUpdatePhyIntfMessage->getCmdCode(), vlanRange);
                m->setPvlanRange(vlanRange);
                status = sendSynchronouslyToWaveClient ("nsm", m.get());
                break;
			}
			case NSMUPDATE_PHY_PVLAN_HOST_ASSOCIATION:
            {
                PROVISION_VLAN(pDceSynchronousLinearSeqContext,
                        pNsmUpdatePhyIntfMessage->getPvlanHostSecVlanId());
                PROVISION_VLAN(pDceSynchronousLinearSeqContext,
                        pNsmUpdatePhyIntfMessage->getPvlanHostPrimVlanId());
            }
			case NSMUPDATE_PHY_DELPVLAN_HOST_ASSOCIATION_ALL:
			{
				trace(TRACE_LEVEL_INFO,string("sending to backend IntfConfLocalWo updatePhyIntf "));
         		m->setPvlanHostPrimVlanId(pNsmUpdatePhyIntfMessage->getPvlanHostPrimVlanId());
         		m->setPvlanHostSecVlanId(pNsmUpdatePhyIntfMessage->getPvlanHostSecVlanId());
                status = sendSynchronouslyToWaveClient ("nsm", m.get());
                break;
			}
			case NSMUPDATE_PHY_SW_ACCESS_DEL_RSPANVLAN:
			{
				//Deleting access rspan-vlan is same as deleting access vlan in the backend. The difference is only in updating the MO
				 NSM_PLUG_INFO_S(string("Update the cmd code to NSMUPDATE_PHY_SW_DELETE_ACCESS_VLAN"));
				 m->setCmdCode(NSMUPDATE_PHY_SW_DELETE_ACCESS_VLAN);
                 status = sendSynchronouslyToWaveClient ("nsm", m.get());
				 break;
			}
            default :
            {
            	 trace(TRACE_LEVEL_DEBUG,string("before sending to backend IntfConfLocalWo updatePhyIntf "));
                 status = sendSynchronouslyToWaveClient ("nsm", m.get());
			     trace(TRACE_LEVEL_DEBUG,string("after sending to backend IntfConfLocalWo updatePhyIntf "));
				 break;
            }
		}
		
		UI32 eld_cmdcode = (ELD_INTF_CONF_PRIORITY | ELD_INTF_CONF_ELD_ENABLE |
			                ELD_INTF_CONF_ELD_DISABLE);
		if (
			((pNsmUpdatePhyIntfMessage->getCmdCode()) != ELD_INTF_CONF_PRIORITY) 
			&& 
			(
			 ((pNsmUpdatePhyIntfMessage->getCmdCode()) != ELD_INTF_CONF_ELD_ENABLE)	
			 && 
			 ((pNsmUpdatePhyIntfMessage->getCmdCode()) != ELD_INTF_CONF_ELD_DISABLE)	
			)
		   ) 
		{
			pNsmUpdatePhyIntfMessage->setCompletionStatus(m->getCompletionStatus());
			NSM_PLUG_INFO_S( string ("UP_PHY_L Response from Backend : ") + ifName + string("Cmdcode :") + pNsmUpdatePhyIntfMessage->getCmdCode() + string("status :") + status + string("backend clientStatus :") + m->getClientStatus());
			if( status == WAVE_MESSAGE_SUCCESS)
			{
				if(  pNsmUpdatePhyIntfMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
					//bpatel : to avoid memory leak
					status = pNsmUpdatePhyIntfMessage->getCompletionStatus();
					if(K1) delete K1;
					if(mSsm) delete mSsm;
					return  status;
				}

				if ( (0 != m->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != m->getClientStatus() ) )  {
					trace(TRACE_LEVEL_ERROR, string("Resp: UP_PHY_L  Client Error MsgId: ") +
						m->getMessageId() +
						FrameworkToolKit::localize(m->getClientStatus()));
					//bpatel : to avoid memory leak
					status = NsmUtils::mapBackEndErrorsToDcmdErrors(m->getClientStatus()) ;
					if(K1) delete K1;
					if(mSsm) delete mSsm;
					return status;
				}
                HANDLE_PROVISION_RESPONSE(m);
			}
    }
		
    if (IS_CMDCODE_SET(pNsmUpdatePhyIntfMessage->getCmdCode(), eld_cmdcode)
        || ( send_native_vlan_disable_to_eld_flag)) {
      if (send_native_vlan_disable_to_eld_flag)
        trace(TRACE_LEVEL_INFO, string("Sending for NATIVE_VLAN"));
      status = sendSynchronouslyToWaveClient ("eld",K1);
        pNsmUpdatePhyIntfMessage->setCompletionStatus(K1->getCompletionStatus());
        NSM_PLUG_INFO_S( string ("UP_PHY_L Response from Backend : ") + ifName + string("Cmdcode :") + pNsmUpdatePhyIntfMessage->getCmdCode() + string("status :") + status + string("backend clientStatus :") + K1->getClientStatus());
        if( status == WAVE_MESSAGE_SUCCESS) {
          if(  pNsmUpdatePhyIntfMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)					 {
            status = pNsmUpdatePhyIntfMessage->getCompletionStatus();
           if(K1) delete K1;
			if(mSsm) delete mSsm;
            return  status;
          }

          if ( (0 != K1->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != K1->getClientStatus() ) )  {
            trace(TRACE_LEVEL_ERROR, string("Resp: UP_PHY_L  Client Error MsgId: ") +
                K1->getMessageId() +
                FrameworkToolKit::localize(K1->getClientStatus()));
            status = NsmUtils::mapBackEndErrorsToDcmdErrors(K1->getClientStatus()) ;
            if(K1)delete K1;
			if(mSsm) delete mSsm;
            return status;
          }
        }
      }

        if(K1){
            delete K1;
			K1 = NULL;
        }
		//bpatel : ADded below lines to avoid memory leak
        if(mSsm){
            delete mSsm;
			mSsm = NULL;
        }

        switch(pNsmUpdatePhyIntfMessage->getCmdCode())
        {
			case NSMUPDATE_PHY_DESC :
			case NSMUPDATE_PHY_DELDESC :
			{
				/*If port is ISL return. Otherwise send to lldp also */
				PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

				WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
				syncQueryCtxt.addAndAttribute (new AttributeString(pNsmUpdatePhyIntfMessage->getIfName(),"ifName"));
				syncQueryCtxt.addSelectField("Portrole");
				vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

				if (pResults != NULL && pResults->size () == 1)
				{
					pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
					if ((DcmToolKit::isVcsEnabled () != WAVE_PERSISTENCE_CLUSTER_ENABLED) ||
						(!pPhyIntfLocalManagedObject->getPortrole()))
					{
				/* send description to lldp also */
				status = updateLldpIntfDescription(pNsmUpdatePhyIntfMessage);
					}
				}

				/*Free pResults to avoid memory leak*/
        		if(pResults)
        			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

				break;
			}
            default :
				break;
		}
        //PVLAN TODO hk
        //return WAVE_MESSAGE_SUCCESS;
        return status;
    }

	ResourceId IntfConfLocalWorker::updateLldpIntfDescription(NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage)
	{
        int status;
		NsmLldpClientIntfConfSetMessage *m = new NsmLldpClientIntfConfSetMessage();

		NSM_PLUG_INFO_S(string ("Entering.."));

		m->setCmdcode (LLDP_INTF_CONF_DESCRIPTION);
		m->setIfType(pNsmUpdatePhyIntfMessage->getIfType());

		/* Three tupple to two tupple conversion */
        if (true  == NsmUtils::isValidThreeTuple(pNsmUpdatePhyIntfMessage->getIfName ())) {
			string twoTuppleIfName;
			string threeTupple = pNsmUpdatePhyIntfMessage->getIfName ();
			NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
			m->setIfName (twoTuppleIfName.c_str());
		} else {
			m->setIfName (pNsmUpdatePhyIntfMessage->getIfName());
		}
		m->setDescription (pNsmUpdatePhyIntfMessage->getDesc());

		status = sendSynchronouslyToWaveClient (LLDP_DCM_CLIENT_NAME, m);

		pNsmUpdatePhyIntfMessage->setCompletionStatus(m->getCompletionStatus());
		if (status == WAVE_MESSAGE_SUCCESS) {
			if (m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
				NSM_PLUG_INFO_S(string ("Response processing: CompletionStatus ")
                		    + string("returned error: ")  +
                  FrameworkToolKit::localize(m->getCompletionStatus()));
				//To avoid memory leak
				status = pNsmUpdatePhyIntfMessage->getCompletionStatus();
				delete m;
				return status;
			}
			NSM_PLUG_INFO_S(string ("Response processing: CompletionStatus ")
                		    + string("returned success: ")  +
                  FrameworkToolKit::localize(m->getCompletionStatus()));
			int ClientStatus = m->getClientStatus();
			if (ClientStatus != WAVE_MESSAGE_SUCCESS) {
				NSM_PLUG_INFO_S(string ("Response processing: ClientStatus ")
                		    + string("returned error: ")  +
                  	FrameworkToolKit::localize(ClientStatus));
				pNsmUpdatePhyIntfMessage->setCompletionStatus(ClientStatus);
				//To avoid memory leak
				status = pNsmUpdatePhyIntfMessage->getCompletionStatus();
				delete m;
				return status;
			}
		} else {
			NSM_PLUG_INFO_S(string ("Response processing: sendSynchronously")
                		    + string(" to Backend Failed "));
		}
		//To avoid memory leak
		if(m){
			delete m;
		}

		return status;
	}

 ResourceId IntfConfLocalWorker::deletePhyIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NSM_PLUG_DBG("Entering ...");

        NsmDeletePhyIntfMessage *pNsmDeletePhyIntfMessage = dynamic_cast<NsmDeletePhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        string ifName = pNsmDeletePhyIntfMessage->getIfName();
        //ResourceId  status   = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                //status = WAVE_MESSAGE_ERROR;
            }else if (1 == numberOfResults) {
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                //status = WAVE_MESSAGE_SUCCESS;
            } else {
                //status = WAVE_MESSAGE_ERROR;
            }
            pResults->clear ();
            delete pResults;
        }


        if (pPhyIntfLocalManagedObject == NULL) {
            NSM_PLUG_DBG_ERR_S(string("DL_PHY_L DeletingMO: Interface already exists ..") + ifName);
            return (WAVE_MESSAGE_ERROR);
        } else {

            prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);

            NSM_PLUG_INFO_S(string("DL_PHY_L Deleting PhyInterface MO  :: ") + ifName);
            delete pPhyIntfLocalManagedObject;
        }

        return pNsmDeletePhyIntfMessage->getCompletionStatus ();
    }

ResourceId IntfConfLocalWorker::deletePhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NSM_PLUG_DBG("Entering ...");
        NsmDeletePhyIntfMessage *pNsmDeletePhyIntfMessage = dynamic_cast<NsmDeletePhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        int status;
        NsmDeletePhyIntfMessage *m = new NsmDeletePhyIntfMessage();
        //"tm","tt",1,"testing");

        m->setIfName (pNsmDeletePhyIntfMessage->getIfName());

        status = sendSynchronouslyToWaveClient ("nsm",m);


        pNsmDeletePhyIntfMessage->setCompletionStatus(m->getCompletionStatus());

        if( status == WAVE_MESSAGE_SUCCESS){
            if(  pNsmDeletePhyIntfMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
				//bpatel: To avoid memory leak
                status = pNsmDeletePhyIntfMessage->getCompletionStatus();
                delete m;
                return status;
			}
		}
		//bpatel: To avoid memory leak
        if (m){
            delete m;
        }
        return status;
    }

	void IntfConfLocalWorker::NsmEventMessageHandler(NsmEventMessage *pNsmEventMessage)
	{
        NSM_PLUG_DBG("Entering ...");

		UI32 eventid = pNsmEventMessage->getEventId();
        UI32 parameter = pNsmEventMessage->getParameter();
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		NsmClientUpdateGlobalConfigMessage *pMessage;

        pNsmEventMessage->setCompletionStatus(status);

		trace(TRACE_LEVEL_DEBUG, string("Sending Event Reply Msg ..."));
		reply(pNsmEventMessage);

        trace(TRACE_LEVEL_INFO, string("System Ready Event Received from Nsm backend : event ") + eventid + string(" para") + parameter);

        // LINECARD_ONLINE is only used for simulation */
		if ((eventid == CHASSIS_READY) || (eventid == SERVICE_READY) || (eventid == LINECARD_READY) || (eventid == LINECARD_ONLINE))
		{
			status = triggerPostBootProgramming(eventid, parameter);
		    trace(TRACE_LEVEL_DEBUG, string("NsmEventMessageHandler: triggerPostBootProgramming Status - ")+ status);
		}
		if(eventid == INTERFACE_STATE_REPLAY)
		{
			status = configReplayIntfAdminState(parameter);
			trace(TRACE_LEVEL_DEBUG, string("NsmEventMessageHandler: configReplayIntfAdminState Status - ")+ status);
			/* Send message to nsmd after completion of config replay */
			pMessage = new NsmClientUpdateGlobalConfigMessage();
			pMessage->setCmdCode(NSM_PLUGIN_INTF_STATEREPLAY_RESP);
			pMessage->setSlotId(parameter);
			status = sendSynchronouslyToWaveClient("nsm",pMessage);
			delete pMessage;
			trace(TRACE_LEVEL_INFO, string("NsmEventMessageHandler: INTERFACE_STATE_REPLAY sending resp to nsmd Status - ")+ status);
		}
	}

    void  IntfConfLocalWorker::NsmCreatePhyIntfMessageHandler( NsmCreatePhyIntfMessage *pNsmCreatePhyIntfMessage)
    {
        NSM_PLUG_DBG("Entering ...");

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            /* Dummy handler - No need to send to backend*/
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::createPhyIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            /* Creating Phyintf MO */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::createPhyIntfMOStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmCreatePhyIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }


ResourceId IntfConfLocalWorker::createPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NSM_PLUG_DBG("Entering ...");
        int status;
        NsmCreatePoIntfSPMessage *pNsmCreatePoIntfSPMessage = dynamic_cast<NsmCreatePoIntfSPMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace(TRACE_LEVEL_INFO,string("IntfConfLocalWo getPoId "+pNsmCreatePoIntfSPMessage->getPoId()));
        NsmPoIntfMessage *m = new NsmPoIntfMessage(pNsmCreatePoIntfSPMessage->getPoId(),pNsmCreatePoIntfSPMessage->getPoName(),pNsmCreatePoIntfSPMessage->getPoIfIndex(),NSM_MTU_DEF_VAL,"",true,false,(swModeEnum)SW_MODE_ACCESS,false,0,0,NSM_MINLINKS_DEF_VAL,(LineSpeed)2,(poLoadBalanceModeEnum)LOADBAL_MODE_SRC_DST_IP_MAC_VID_PORT,0,0,0,"","",NSMCREATE_PO_INTF, false,0,false);
        NSM_PLUG_INFO_S( string ("CR_PO_L Sending to Backend : ") + pNsmCreatePoIntfSPMessage->getPoId() + string("Cmdcode :") + NSMCREATE_PO_INTF);
        status = sendSynchronouslyToWaveClient ("nsm",m);

        pNsmCreatePoIntfSPMessage->setPoIfIndex(m->getPoIfIndex());


        NSM_PLUG_INFO_S( string ("CR_PO_L Receiving response from Backend : ") + pNsmCreatePoIntfSPMessage->getPoId() + string("Cmdcode :") + NSMCREATE_PO_INTF + string("status :") + status + string("backend_clientStatus :") + m->getClientStatus());

        if( status == WAVE_MESSAGE_SUCCESS)
        {
            if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
				 //bpatel: To avoid memory leak
                status =  m->getCompletionStatus();
                delete m;
                return  status;
            }

            if ( (0 != m->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != m->getClientStatus() ) )  {
                trace(TRACE_LEVEL_ERROR, string("Resp: CR_PO_L Client Error MsgId: ") +
                        m->getMessageId() +
                        FrameworkToolKit::localize(m->getClientStatus()));
            	//bpatel: To avoid memory leak
                status = m->getClientStatus();
                delete m;
                return status;
            }
        }
		//bpatel: To avoid memory leak
        if(m){
            delete m;
        }
        return status;
    }
    /* Code Ends Here */

    void  IntfConfLocalWorker::NsmCreatePoIntfSPMessageHandler( NsmCreatePoIntfSPMessage *pNsmCreatePoIntfSPMessage)
    {
        NSM_PLUG_DBG("Entering ...");

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            /* Sending Msg to backedn daemon */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::createPoIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmCreatePoIntfSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    /* Code Starts Here */
    ResourceId IntfConfLocalWorker::createVlanIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int status;
        NsmCreateVlanIntfSPMessage *pNsmCreateVlanIntfSPMessage = dynamic_cast<NsmCreateVlanIntfSPMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        if (pNsmCreateVlanIntfSPMessage->isBulkVlanConfig()) {
            return createBulkVlanIntfStep (pPrismSynchronousLinearSequencerContext);
        }

        trace(TRACE_LEVEL_INFO,string("IntfConfLocalWo NsmVlanIntfMessage getVlanId ")+pNsmCreateVlanIntfSPMessage->getVlanId());
        NsmVlanIntfMessage *m = new NsmVlanIntfMessage(pNsmCreateVlanIntfSPMessage->getVlanId(),pNsmCreateVlanIntfSPMessage->getVlanName(),pNsmCreateVlanIntfSPMessage->getVlanIfIndex(),pNsmCreateVlanIntfSPMessage->getDesc(),false,NSMCREATE_VLAN_INTF,0);
        status = sendSynchronouslyToWaveClient ("nsm",m);

        pNsmCreateVlanIntfSPMessage->setVlanIfIndex(m->getVlanIfIndex());

        NSM_PLUG_INFO_S( string ("CR_VLAN_L Receiving response from backend  ") + pNsmCreateVlanIntfSPMessage->getVlanId() + string("Cmdcode :") + NSMCREATE_VLAN_INTF + string("status :") + status + string("backend_clientStatus :") + m->getClientStatus());

        if( status == WAVE_MESSAGE_SUCCESS)
        {
            if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
				//bpatel: To avoid memory leak
                status = m->getCompletionStatus();
                delete m;
                return status;
            }

            if ( (0 != m->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != m->getClientStatus() ) )  {
                trace(TRACE_LEVEL_ERROR, string("Resp: CR_VLAN_L Client Error MsgId: ") +
                        m->getMessageId() +
                        FrameworkToolKit::localize(m->getClientStatus()));
				//bpatel: To avoid memory leak
                status = m->getClientStatus();
                delete m;
                return status;
            }
            HANDLE_PROVISION_RESPONSE(m);
        }
		//bpatel: To avoid memory leak
        if(m){
            delete m;
        }
        return status;
    }
    /* Code Ends Here */

    ResourceId IntfConfLocalWorker::createBulkVlanIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        UI32 failedVlanCount = 0;
        vector<UI32> vlanRangeVector;
        NsmCreateVlanIntfSPMessage *pNsmCreateVlanIntfSPMessage = dynamic_cast<NsmCreateVlanIntfSPMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        string vlanRangeStr = pNsmCreateVlanIntfSPMessage->getRangeVlanStr();

        if (! vlanRangeStr.empty()) {
            trace(TRACE_LEVEL_INFO, string("Enter IntfConfLocalWorker::createBulkVlanIntfStep vlanRangeStr:(") + vlanRangeStr + string(")"));
            UI32Range vlanRange = UI32Range(vlanRangeStr);
            vlanRange.getUI32RangeVector(vlanRangeVector);

            for (UI32 i = 0; i < vlanRangeVector.size(); i++) {
                 UI32 vlanId = vlanRangeVector[i];
                 string vlanStr = string("") + vlanId;
                 trace(TRACE_LEVEL_DEVEL, string("IntfConfLocalWorker::createBulkVlanIntfStep: vlanId:(") + vlanId + string("),vlanStr:(")+ vlanStr + string(")"));

                NsmVlanIntfMessage *m = new NsmVlanIntfMessage(vlanId, vlanStr, pNsmCreateVlanIntfSPMessage->getVlanIfIndex(),pNsmCreateVlanIntfSPMessage->getDesc(),false,NSMCREATE_VLAN_INTF,0);
                ResourceId status = sendSynchronouslyToWaveClient ("nsm",m);

                //pNsmCreateVlanIntfSPMessage->setVlanIfIndex(m->getVlanIfIndex());
                trace (TRACE_LEVEL_DEBUG, string ("CR_VLAN_L Receiving response from backend  ") + vlanId + string(" Cmdcode : NSMCREATE_VLAN_INTF ") + string("status :") + status + string(" backend_clientStatus :") + m->getClientStatus());

                if(status == WAVE_MESSAGE_SUCCESS)
                {
                    if(m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
                        status = m->getCompletionStatus();
                        pNsmCreateVlanIntfSPMessage->updateFailureStatus(vlanId, status);
                        ++failedVlanCount;
                        delete m;
                        continue;
                    }

                    if ( (0 != m->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != m->getClientStatus() ) )  {
                        trace(TRACE_LEVEL_ERROR, string("Resp: CR_VLAN_L Client Error MsgId: ") + m->getMessageId() + FrameworkToolKit::localize(m->getClientStatus()));
                        status = m->getClientStatus();
                        pNsmCreateVlanIntfSPMessage->updateFailureStatus(vlanId, status);
                        ++failedVlanCount;
                        delete m;
                        continue;
                    }
                } else {
                    trace (TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::createBulkVlanIntfStep: Error:- ") + FrameworkToolKit::localize(status));
                    pNsmCreateVlanIntfSPMessage->updateFailureStatus(vlanId, status);
                    ++failedVlanCount;
                    delete m;
                    continue;
                }
                if(m){
                    delete m;
                    m = NULL;
                }
            }
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    void  IntfConfLocalWorker::NsmCreateVlanIntfSPMessageHandler( NsmCreateVlanIntfSPMessage *pNsmCreateVlanIntfSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
        /* Sending Msg to backedn daemon */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::createVlanIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmCreateVlanIntfSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

    }

    void  IntfConfLocalWorker::NsmDeletePhyIntfMessageHandler( NsmDeletePhyIntfMessage *pNsmDeletePhyIntfMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            /* Not requried to send to backend */
//          reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::deletePhyIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::deletePhyIntfMOStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmDeletePhyIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IntfConfLocalWorker::deletePhyIntfToPoIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NsmDeletePhyIntfToPoIntfMessage  *pNsmDeletePhyIntfToPoIntfMessage  = dynamic_cast<NsmDeletePhyIntfToPoIntfMessage  *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		return deletePhyIntfToPoIntfMO(pNsmDeletePhyIntfToPoIntfMessage,pPrismSynchronousLinearSequencerContext);
    }

    ResourceId IntfConfLocalWorker::deletePhyIntfToPoIntfMO(NsmDeletePhyIntfToPoIntfMessage *pNsmDeletePhyIntfToPoIntfMessage,PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
      {

        string ifName = pNsmDeletePhyIntfToPoIntfMessage ->getIfName();
        //ResourceId  status   = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
           		trace (TRACE_LEVEL_ERROR, "IntfConfLocalWorker::deletePhyIntfToPoIntfMOStep - PhyIntfLocalManagedObject not found");
                //status = WAVE_MESSAGE_ERROR;
            }else if (1 == numberOfResults) {
           		trace (TRACE_LEVEL_DEVEL, "IntfConfLocalWorker::deletePhyIntfToPoIntfMOStep - PhyIntfLocalManagedObject found");
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                //status = WAVE_MESSAGE_SUCCESS;
            } else {
           		trace (TRACE_LEVEL_ERROR, "IntfConfLocalWorker::deletePhyIntfToPoIntfMOStep - More than PhyIntfLocalManagedObject found");
                //status = WAVE_MESSAGE_ERROR;
            }
        }

        if (pPhyIntfLocalManagedObject == NULL) {
            return (WAVE_MESSAGE_ERROR);
        } else {
            updateWaveManagedObject (pPhyIntfLocalManagedObject);

            prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);

            pPhyIntfLocalManagedObject->setPoId(ObjectId::NullObjectId);
            pPhyIntfLocalManagedObject->setPoMode (0);
            pPhyIntfLocalManagedObject->setPoType (0);
        	pPhyIntfLocalManagedObject->setLacpTimeout (LACP_TIMEOUT_DEFAULT);
			pPhyIntfLocalManagedObject->setLacpPortPriority (DCM_LACP_DEFAULT_PORT_PRIORITY);
			pPhyIntfLocalManagedObject->setLacpDefaultUp (LACP_DEFAULT_DEFAULT_UP);
        }
		if(pResults)
		{
            trace (TRACE_LEVEL_ERROR, "IntfConfLocalWorker::deletePhyIntfToPoIntfMOStep - More th.... ");
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			pResults->clear ();
			delete pResults;
		}

		return pNsmDeletePhyIntfToPoIntfMessage ->getCompletionStatus ();
    }

    ResourceId
    IntfConfLocalWorker::fcoeDelPhyIntfToPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        UI32    po_id;
        FcoeClientEthConfigMessage  *dmn_msg_p = NULL;
        vector<UI64> ifindex_vec;
        UI64 lag_ifindex = 0;
        UI64 mbr_ifindex = 0;
        ObjectId poObjectId = ObjectId::NullObjectId;
        PoIntfManagedObject *pPoIntfManagedObject = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        if (NsmUtils::isFsbEnabled() == false) {
        NsmDeletePhyIntfToPoIntfMessage  *pNsmDeletePhyIntfToPoIntfMessage  = dynamic_cast<NsmDeletePhyIntfToPoIntfMessage  *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		WaveManagedObjectSynchronousQueryContext findObjectIdsyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		findObjectIdsyncQueryCtxt.addAndAttribute (new AttributeString(pNsmDeletePhyIntfToPoIntfMessage->getIfName(),"id"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&findObjectIdsyncQueryCtxt);

        if (pResults && pResults->size() == 1) {
            pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
        } else {
            NSM_PLUG_DBG_ERR_S(string("Failed to find Physical IF MO for name = ") +
                               pNsmDeletePhyIntfToPoIntfMessage->getIfName());
            return WAVE_MESSAGE_SUCCESS;
        }
 
		poObjectId = pPhyIntfLocalManagedObject->getPoId();
		pPoIntfManagedObject = NsmUtils::getPoIntfObjectByObjectId(poObjectId);

        po_id = strtoul(pPoIntfManagedObject->getIfName().c_str(), NULL, 0);

        if (NsmUtils::isFcoeConfiguredForLagPort(po_id) == false) {
            NSM_PLUG_INFO_S(string("Fcoe is not configured for Po ") + 
                            pPoIntfManagedObject->getIfName());
            if (pPoIntfManagedObject) {
                delete pPoIntfManagedObject;
            }
            if (pResults) {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
            return WAVE_MESSAGE_SUCCESS;
        }

        mbr_ifindex = NsmUtils::getPhyPortIfindexByName(pNsmDeletePhyIntfToPoIntfMessage->getIfName());

        NSM_PLUG_INFO_S(string("Adding Member=") + pNsmDeletePhyIntfToPoIntfMessage->getIfName() +
                        string("Ifindex=") + mbr_ifindex);

        ifindex_vec.push_back(mbr_ifindex);

        dmn_msg_p = new FcoeClientEthConfigMessage();
        dmn_msg_p->setOper_bmp(FCOE_ETH_INTF_OPER_LAG_DEL_MBR);
        dmn_msg_p->setIfindex(lag_ifindex);
        dmn_msg_p->setIfId(pPoIntfManagedObject->getIfName());
        dmn_msg_p->setFcoeMbrIfindexVec(ifindex_vec);

        status = sendSynchronouslyToWaveClient (FCOE_DCM_CLIENT_NAME, dmn_msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("Message Status = ") + status);
        }

        if (dmn_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("Client Status = ") + dmn_msg_p->getClientStatus());
            status = dmn_msg_p->getClientStatus();
        }

        if (dmn_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("COmpletion Status = ") + dmn_msg_p->getCompletionStatus());
            status = dmn_msg_p->getCompletionStatus();
        }

        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

        delete pPoIntfManagedObject;

        NSM_PLUG_INFO_S(string("Final Status") + status);
        delete (dmn_msg_p);
        }
        return status;
    }


  ResourceId IntfConfLocalWorker::deletePhyIntfToPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NsmDeletePhyIntfToPoIntfMessage  *pNsmDeletePhyIntfToPoIntfMessage  = dynamic_cast<NsmDeletePhyIntfToPoIntfMessage  *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace(TRACE_LEVEL_INFO,string("IntfConfLocalWo NsmPhyIntfMessage ifName "+pNsmDeletePhyIntfToPoIntfMessage->getIfName()));
        NsmPhyIntfMessage *m = new NsmPhyIntfMessage(pNsmDeletePhyIntfToPoIntfMessage->getIfName(),pNsmDeletePhyIntfToPoIntfMessage->getSwIfIndex(),NSM_MTU_DEF_VAL,NSM_OPENFLOW_DEF_STATUS,"",false,false,1,false,NSM_DELETE_CHANNELGROUP,LACP_TIMEOUT_DEFAULT,DCM_LACP_DEFAULT_PORT_PRIORITY,(LineSpeed)1,1,"",false,20,32768, false,0,0,0,false);

        ResourceId  status   = WAVE_MESSAGE_ERROR;
		UI32 noOfLocalPoInterface = 0;
        ObjectId poObjectId = ObjectId::NullObjectId;
		string poName;
	    PoIntfManagedObject *pPoIntfManagedObject = NULL;
		if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled())
		{
			string twoTupleIfName;
			NsmUtils::getTwoTupleIfName(m->getIfName(),twoTupleIfName);
			m->setIfName(twoTupleIfName);
			PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
			WaveManagedObjectSynchronousQueryContext findObjectIdsyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
			findObjectIdsyncQueryCtxt.addAndAttribute (new AttributeString(pNsmDeletePhyIntfToPoIntfMessage->getIfName(),"id"));
			vector<WaveManagedObject *> *pResults = querySynchronously (&findObjectIdsyncQueryCtxt);

			UI32 resultIter = 0;
			if (NULL != pResults) {
            	UI32 numberOfResults = pResults->size ();
            	if (0 == numberOfResults) {
           			trace (TRACE_LEVEL_ERROR, "IntfConfLocalWorker::deletePhyIntfToPoIntfStep, could not find PhyIntfLocalManagedObject");
                	status = WAVE_MESSAGE_ERROR;
            	}else if (1 == numberOfResults) {
           			trace (TRACE_LEVEL_DEVEL, "IntfConfLocalWorker::deletePhyIntfToPoIntfStep, found PhyIntfLocalManagedObject");
                	pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                	status = WAVE_MESSAGE_SUCCESS;
            	} else {
           			trace (TRACE_LEVEL_ERROR, "IntfConfLocalWorker::deletePhyIntfToPoIntfStep, found more than one PhyIntfLocalManagedObject");
                	status = WAVE_MESSAGE_ERROR;
            	}
			}
        	else
				status = WAVE_MESSAGE_ERROR;

			if (status == WAVE_MESSAGE_SUCCESS)
			{
				poObjectId = pPhyIntfLocalManagedObject->getPoId();
				pPoIntfManagedObject = NsmUtils::getPoIntfObjectByObjectId(poObjectId);

				UI32 currMappedId;
				UI32 mappedId;
				mappedId = (UI32) NsmUtils::getMappedIdFromIfName(pNsmDeletePhyIntfToPoIntfMessage->getIfName());
				if(pResults)
            		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

				pPhyIntfLocalManagedObject = NULL;
				WaveManagedObjectSynchronousQueryContext findLocalIntfsyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
				findLocalIntfsyncQueryCtxt.addAndAttribute (new AttributeObjectId(poObjectId,"poId"));
				pResults = querySynchronously(&findLocalIntfsyncQueryCtxt);

				resultIter = 0;

				while (resultIter != pResults->size()) {
					pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[resultIter]);
					if(pPhyIntfLocalManagedObject->getPoId() == poObjectId)
					{
						currMappedId = (UI32) NsmUtils::getMappedIdFromIfName(pPhyIntfLocalManagedObject->getIfName());
						if(mappedId == currMappedId)
							noOfLocalPoInterface++;
					}
					resultIter++;
				}

				if (pResults)
            		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			}
		}

		m->setIfType(pNsmDeletePhyIntfToPoIntfMessage->getIfType());
        status = sendSynchronouslyToWaveClient ("nsm",m);

		if((status == WAVE_MESSAGE_SUCCESS) &&
			( (0 == m->getClientStatus()) || (WAVE_MESSAGE_SUCCESS == m->getClientStatus()) ) )
		{
			if((WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()) && noOfLocalPoInterface == 1 && pPoIntfManagedObject != NULL)
			{/*This is the last member port for this port-channel on this interface. So delete the port-channel from backend.*/
				NsmPoIntfMessage *mPoDelMsg = new NsmPoIntfMessage(pPoIntfManagedObject->getIfName(),"",(UI32)pPoIntfManagedObject->getIfIndex(),NSM_MTU_DEF_VAL, "",true,false,(swModeEnum)SW_MODE_ACCESS,false,0,0,NSM_MINLINKS_DEF_VAL,(LineSpeed)2,(poLoadBalanceModeEnum)LOADBAL_MODE_SRC_DST_IP_MAC_VID_PORT,0,0,0,"","",NSMDELETE_PO_INTF, false,0,false);

				status = sendSynchronouslyToWaveClient ("nsm",mPoDelMsg);

				if (status == WAVE_MESSAGE_SUCCESS) {
                    startTransaction();
                    deletePoReferenceInPhyIntfMo (pPoIntfManagedObject->getIfName(),pPrismSynchronousLinearSequencerContext);
                    commitTransaction();
				}
				//bpatel: To avoid memory leak
                if(mPoDelMsg){
                    delete mPoDelMsg;
					mPoDelMsg = NULL;
                }

			}
		}
		if (pPoIntfManagedObject)
		{
			delete pPoIntfManagedObject;
			pPoIntfManagedObject = NULL;
		}

        pNsmDeletePhyIntfToPoIntfMessage ->setCompletionStatus(m->getCompletionStatus());

        if( status == WAVE_MESSAGE_SUCCESS)
        {
            if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
				 //bpatel : To avoid memory leak
                status = m->getCompletionStatus();
                delete m;
                return status;
            }

            if ( (0 != m->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != m->getClientStatus() ) )  {
                trace(TRACE_LEVEL_ERROR, string("Resp:  Client Error MsgId: ") +
                        m->getMessageId() +
                        FrameworkToolKit::localize(m->getClientStatus()));
                //bpatel : To avoid memory leak
                status = NsmUtils::mapBackEndErrorsToDcmdErrors(m->getClientStatus());
                pNsmDeletePhyIntfToPoIntfMessage->setCompletionStatus(status);
                delete m;
                return status;
            }
        }
		//bpatel : To avoid memory leak
        if(m){
            delete m;
        }
        return status;
    }

    void  IntfConfLocalWorker::NsmDeletePhyIntfToPoIntfMessageHandler( NsmDeletePhyIntfToPoIntfMessage *pNsmDeletePhyIntfToPoIntfMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::fcoeDelPhyIntfToPoIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::deletePhyIntfToPoIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::deletePhyIntfToPoIntfMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmDeletePhyIntfToPoIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  IntfConfLocalWorker::NsmDeletePhyIntfToVlanIntfMessageHandler( NsmDeletePhyIntfToVlanIntfMessage *pNsmDeletePhyIntfToVlanIntfMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmDeletePhyIntfToVlanIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId
    IntfConfLocalWorker::fcoeDelPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NsmDeletePoIntfSPMessage *pNsmDeletePoIntfSPMessage = dynamic_cast<NsmDeletePoIntfSPMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        return fcoeDelPoIntf(pNsmDeletePoIntfSPMessage);

     }

     ResourceId
     fcoeDelPoIntf(NsmDeletePoIntfSPMessage *pNsmDeletePoIntfSPMessage)
     {
        UI64 lag_ifindex = 0;
        UI32    po_id;
        FcoeClientEthConfigMessage  *dmn_msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string po_id_str = "";
        UI32 i = 0;
        vector<UI64> ifindex_vec;

        po_id_str = pNsmDeletePoIntfSPMessage->getPoId();
        po_id = strtol(po_id_str.c_str(), NULL, 0);

        if (NsmUtils::isFcoeConfiguredForLagPort(po_id) == false) {
            NSM_PLUG_INFO_S(string("Fcoe is not configured for Po ") +
                            pNsmDeletePoIntfSPMessage->getPoId());
            return WAVE_MESSAGE_SUCCESS;
        }

        dmn_msg_p = new FcoeClientEthConfigMessage();
        dmn_msg_p->setOper_bmp(FCOE_ETH_INTF_OPER_FCOEPORT_LAG_DEL);
        dmn_msg_p->setIfindex(lag_ifindex);
        dmn_msg_p->setIfId(pNsmDeletePoIntfSPMessage->getPoId());
        
        NsmUtils::getPoMemberIfindexList(po_id, ifindex_vec);
        NSM_PLUG_DBG_S(string("Retrieved Ifindex List of Size=") +
                       ifindex_vec.size());
        for (i = 0; i < ifindex_vec.size(); i++) {
            NSM_PLUG_DBG_S(string("Retrieved Ifindex=") + ifindex_vec.at(i));
        }
        dmn_msg_p->setFcoeMbrIfindexVec(ifindex_vec);

        status = WaveObjectManagerToolKit::sendSynchronouslyToWaveClient (FCOE_DCM_CLIENT_NAME, dmn_msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("Message Status = ") + status);
        }

        if (dmn_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("Client Status = ") + dmn_msg_p->getClientStatus());
            status = dmn_msg_p->getClientStatus();
        }

        if (dmn_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("COmpletion Status = ") + dmn_msg_p->getCompletionStatus());
            status = dmn_msg_p->getCompletionStatus();
        }

        NSM_PLUG_INFO_S(string("Final Status") + status);
        delete (dmn_msg_p);

        // Dont stop the cleanup of LAG based on FCoE failure.
        return WAVE_MESSAGE_SUCCESS;
    }


/* Code Starts Here */
    ResourceId IntfConfLocalWorker::deletePoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NSM_PLUG_DBG("Entering ...");
        ResourceId status;
        NsmDeletePoIntfSPMessage *pNsmDeletePoIntfSPMessage = dynamic_cast<NsmDeletePoIntfSPMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        trace(TRACE_LEVEL_INFO,string("IntfConfLocalWo pNsmDeletePoIntfSPMessage ifName "+pNsmDeletePoIntfSPMessage->getPoId()));
        NsmPoIntfMessage *m = new NsmPoIntfMessage(pNsmDeletePoIntfSPMessage->getPoId(),"",pNsmDeletePoIntfSPMessage->getPoIfIndex(),NSM_MTU_DEF_VAL,"",true,false,(swModeEnum)SW_MODE_ACCESS,false,0,0,NSM_MINLINKS_DEF_VAL,(LineSpeed)2,(poLoadBalanceModeEnum)LOADBAL_MODE_SRC_DST_IP_MAC_VID_PORT,0,0,0,"","",NSMDELETE_PO_INTF,false,0,false);
        NSM_PLUG_INFO_S( string ("DL_PO_L Sending to Backend : ") + pNsmDeletePoIntfSPMessage->getPoId() + string("Cmdcode :") + NSMDELETE_PO_INTF);

        m->setPoId (pNsmDeletePoIntfSPMessage->getPoId());


        status = sendSynchronouslyToWaveClient ("nsm",m);

        NSM_PLUG_INFO_S( string ("DL_PO_L Sending to Backend : ") + pNsmDeletePoIntfSPMessage->getPoId() + string("Cmdcode :") + NSMDELETE_PO_INTF + string("status :") + status + string("backend_clientStatus :") + m->getClientStatus());

        if( status == WAVE_MESSAGE_SUCCESS)
        {
            if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
				//bpatel : To avoid memory leak
                status = m->getCompletionStatus();
                delete m;
                return status ;
            }

            if ( (0 != m->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != m->getClientStatus() ) )  {
                WaveNs::trace(TRACE_LEVEL_ERROR, string("Resp:  DL_PO_L Client Error MsgId: ") +
                        m->getMessageId() +
                        FrameworkToolKit::localize(m->getClientStatus()));
                //bpatel : To avoid memory leak
                status = m->getClientStatus();
                delete m;
                return status;
            }
        }

		 //bpatel : To avoid memory leak
        if(m){
            delete m;
        }
        return status;
    }

    ResourceId IntfConfLocalWorker::deletePhyIntfPoReference(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NSM_PLUG_INFO_S( string("Entering ... "));

        NsmDeletePoIntfSPMessage *pNsmDeletePoIntfSPMessage = dynamic_cast<NsmDeletePoIntfSPMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        //string poId = pNsmGlobalDeletePoIntfMessage->getPoId();
        string poId = pNsmDeletePoIntfSPMessage->getPoId();
        deletePoReferenceInPhyIntfMo (poId, pPrismSynchronousLinearSequencerContext);
        return WAVE_MESSAGE_SUCCESS;
    }

    /* Code Ends Here */


    void IntfConfLocalWorker::deletePoReferenceInPhyIntfMo (const string &poId, PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
		LocationId locationId;
        ObjectId poObjectId;

        NSM_PLUG_INFO_S( string("Entering ... poName : ")+ poId);

        getPoIntfObjectIdByPoIfName (poId, poObjectId, false, pPrismSynchronousLinearSequencerContext);
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeObjectId(poObjectId,"poId"));

        NsmUtils::setAllCompositionfalse(syncQueryCtxt);
        syncQueryCtxt.addSelectField("ifName");
        syncQueryCtxt.addSelectField("poId");


        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
        for(unsigned int i = 0;i < pResults->size(); i++)
        {
            pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[i]);
            locationId = NsmUtils::getLocationId(pPhyIntfLocalManagedObject->getIfName());

            if (locationId != FrameworkToolKit::getThisLocationId())
                continue;

			if (!(pPhyIntfLocalManagedObject->getPoId() == ObjectId::NullObjectId))
			{
                // Already within the transaction context
                pPhyIntfLocalManagedObject->addAttributeToBeUpdated("poId");
                pPhyIntfLocalManagedObject->setPoId(ObjectId::NullObjectId);

                pPhyIntfLocalManagedObject->addAttributeToBeUpdated("poMode");
                pPhyIntfLocalManagedObject->setPoMode (0);

                pPhyIntfLocalManagedObject->addAttributeToBeUpdated("poType");
                pPhyIntfLocalManagedObject->setPoType (0);

                pPhyIntfLocalManagedObject->addAttributeToBeUpdated("lacpTimeout");
                pPhyIntfLocalManagedObject->setLacpTimeout (LACP_TIMEOUT_DEFAULT);

                pPhyIntfLocalManagedObject->addAttributeToBeUpdated("lacpPortPriority");
                pPhyIntfLocalManagedObject->setLacpPortPriority (DCM_LACP_DEFAULT_PORT_PRIORITY);

                pPhyIntfLocalManagedObject->addAttributeToBeUpdated("lacpDefaultUp");
                pPhyIntfLocalManagedObject->setLacpDefaultUp (LACP_DEFAULT_DEFAULT_UP);

                updateWaveManagedObject (pPhyIntfLocalManagedObject);
			 }
            NSM_PLUG_INFO_S(string("Deleted PO reference for PhyIntf : ") + pPhyIntfLocalManagedObject->getIfName());
        }
		if(pResults)
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
    }


    void  IntfConfLocalWorker::NsmDeletePoIntfSPMessageHandler( NsmDeletePoIntfSPMessage *pNsmDeletePoIntfSPMessage)
    {
        NSM_PLUG_DBG("Entering ...");
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = NULL;
        tracePrintf(TRACE_LEVEL_DEBUG, "IntfConfLocalWorker::NsmDeletePoIntfSPMessageHandler: For Db update-- %s",(pNsmDeletePoIntfSPMessage->isDBUpdateRequest() ? "True" : "False"));
        if (pNsmDeletePoIntfSPMessage->isDBUpdateRequest()) {
            /*  Update Local Db  */
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::deletePhyIntfPoReference),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };
            pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmDeletePoIntfSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        } else {
            /*  Sending to backend  */
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::fcoeDelPoIntfStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::deletePoIntfStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };
            pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmDeletePoIntfSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        }
        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  IntfConfLocalWorker::NsmDeletePoToVlanIntfSPMessageHandler( NsmDeletePoToVlanIntfSPMessage *pNsmDeletePoToVlanIntfSPMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmDeletePoToVlanIntfSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

     /* Code Starts Here */
    ResourceId IntfConfLocalWorker::deleteVlanIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        int status;
        NsmDeleteVlanIntfSPMessage *pNsmDeleteVlanIntfSPMessage =
                dynamic_cast<NsmDeleteVlanIntfSPMessage *>(pDceSynchronousLinearSeqContext->getPPrismMessage());

        UI32 vlanId = pNsmDeleteVlanIntfSPMessage->getVlanId();
        trace(TRACE_LEVEL_INFO,string("IntfConfLocalWo pNsmDeleteVlanIntfSPMessage vlanId ")+vlanId);

        if(NsmUtils::isVlanPartOfRspanSession(vlanId)) {
        	return WRC_NSM_RSPAN_VLAN_PARTOF_ACTIVE_RSPAN_CONFIG_CANNOT_BE_DELETED;
        }

        NsmVlanIntfMessage *m = new NsmVlanIntfMessage(vlanId, "",
                pNsmDeleteVlanIntfSPMessage->getVlanIfIndex(), "", false,
                NSMDELETE_VLAN_INTF, 0);
        status = sendSynchronouslyToWaveClient ("nsm",m);

        NSM_PLUG_INFO_S(
                string ("DL_VLAN_L Receiving response from backend  ") + vlanId + string("Cmdcode :") + NSMDELETE_VLAN_INTF + string("status :") + status + string("backend_clientStatus :") + m->getClientStatus());

        if( status == WAVE_MESSAGE_SUCCESS)
        {
            if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
				//bpatel : To avoid memory leak
                status =  m->getCompletionStatus();
                delete m;
                return status;
            }

            if ( (0 != m->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != m->getClientStatus() ) )  {
                trace(TRACE_LEVEL_ERROR, string("Resp: DL_VLAN_L  Client Error MsgId: ") +
                        m->getMessageId() +
                        FrameworkToolKit::localize(m->getClientStatus()));

                //bpatel : To avoid memory leak
                status = m->getClientStatus();
                delete m;
                return status;
            }
        }

		pDceSynchronousLinearSeqContext->setWaveManagedObjectId(pNsmDeleteVlanIntfSPMessage->getVlanObjectID());
        // As now the vlan is deleted (unprovisioned), mark accordingly
        ProvisioningUtil::removeUnProvisionedVlan(vlanId);
		//bpatel : To avoid memory leak
        if(m){
            delete m;
        }
        return status;
    }

    ResourceId IntfConfLocalWorker::deleteVlanIntfXstpStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        int status = WAVE_MESSAGE_SUCCESS;
		int vid;

        NsmDeleteVlanIntfSPMessage *pNsmDeleteVlanIntfSPMessage = dynamic_cast<NsmDeleteVlanIntfSPMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

        vid = pNsmDeleteVlanIntfSPMessage->getVlanId();
		trace (TRACE_LEVEL_DEBUG, string("IntfConfLocalWorker::deleteVlanIntfXstpStep: vid = ")+ pNsmDeleteVlanIntfSPMessage->getVlanId());

		(NsmLocalObjectManager::getInstance()->getIntfXstpLocalConfWorker())->deleteVlanIntfXstp(vid, pDceSynchronousLinearSeqContext);

        return status;
    }


    ResourceId IntfConfLocalWorker::deletePhyIntfVlanReference(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        NsmDeleteVlanIntfSPMessage *pNsmDeleteVlanIntfSPMessage = dynamic_cast<NsmDeleteVlanIntfSPMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());
        trace (TRACE_LEVEL_DEBUG, string("Inside deletePhyIntfVlanReference:")+ pNsmDeleteVlanIntfSPMessage->getVlanId());
        ObjectId vlanObjectId;
        PhyIntfLocalManagedObject *pPhyIntfLocalMO = NULL;
        UI32 vlan_id = pNsmDeleteVlanIntfSPMessage->getVlanId();
        LocationId                          locationId = 0;
        locationId = FrameworkToolKit::getThisLocationId ();


        vlanObjectId = pNsmDeleteVlanIntfSPMessage->getVlanObjectID();
        pDceSynchronousLinearSeqContext->setWaveManagedObjectId(vlanObjectId);

		vector<string> selectFields;

		selectFields.push_back ("isAllowedVlanNone");
		selectFields.push_back ("isAllowedVlanAll");
		selectFields.push_back ("nativeVlanIntfConfig");
		selectFields.push_back ("vlanRangeRemove");
		selectFields.push_back ("swMode");
		selectFields.push_back ("vlanObjectId");
		selectFields.push_back ("vlanRangeAdd");
        selectFields.push_back ("eldprio");
        selectFields.push_back ("vlanid");
        selectFields.push_back ("rspan_trunkvlanrange");
        selectFields.push_back ("rspan_accessvlan");

        selectFields.push_back ("pVlanModeTrunkBas");
        selectFields.push_back ("pVlanModeTrunkPromis");
        selectFields.push_back ("pVlanRangeAdd");
        selectFields.push_back ("pVlanRangeRemove");
        selectFields.push_back ("pVlanIsAllowedVlanAll");
        selectFields.push_back ("pVlanNativeVlanId");

		WaveManagedObjectSynchronousQueryContext PhyIntfConfSyncQryCtxt(PhyIntfLocalManagedObject::getClassName ());
		PhyIntfConfSyncQryCtxt.setLocationIdFilter(locationId);
		PhyIntfConfSyncQryCtxt.addOrAttribute(new AttributeEnum(SW_MODE_TRUNK, "swMode"));
		PhyIntfConfSyncQryCtxt.addOrAttribute(new AttributeEnum(SW_MODE_ACCESS, "swMode"));
		PhyIntfConfSyncQryCtxt.addOrAttribute(new AttributeBool(true, "pVlanModeTrunkBas"));
		PhyIntfConfSyncQryCtxt.addOrAttribute(new AttributeBool(true, "pVlanModeTrunkPromis"));
		PhyIntfConfSyncQryCtxt.addOrAttribute(new AttributeObjectIdAssociation(&vlanObjectId, "vlanObjectId", VlanIntfManagedObject::getClassName()));

		PhyIntfConfSyncQryCtxt.addSelectFields(selectFields);

		vector<WaveManagedObject *> *pPhyResults = querySynchronouslyLocalManagedObjectsForLocationId (locationId, &PhyIntfConfSyncQryCtxt);

		if (NULL != pPhyResults && !(pPhyResults->empty()))
		{
			UI32 numberOfResults = pPhyResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_DEBUG,
					"IntfConfGlobalWorker::deletePhyIntfVlanReference  No MO in PhyIntfLocalManagedObject found");
			}else {
				UI32 count = 0;
                ObjectId defaultVlanObjectId = ObjectId::NullObjectId;
				while(count != numberOfResults)
		{
					pPhyIntfLocalMO = dynamic_cast<PhyIntfLocalManagedObject *>
						((*pPhyResults)[count]);


					if(!(pPhyIntfLocalMO->getIsAllowedVlanNone()))
					{
						if(pPhyIntfLocalMO->getIsAllowedVlanAll())
						{
                        	WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanMo =
                           		pPhyIntfLocalMO->getNativeVlanIntfConfig();

                            NativeVlanIntfConfLocalManagedObject *nativeTmp = pNativeVlanMo.operator-> ();

                            if(nativeTmp->getNativeVlanId() == vlan_id)
                            {
								trace (TRACE_LEVEL_DEBUG, string("Inside deletePhyIntfVlanReference getNativeVlanId:")+ vlan_id);
                            	NativeVlanIntfConfLocalManagedObject *pNativeVlanIntfConfLocalManagedObject =
                                	new NativeVlanIntfConfLocalManagedObject(
                                    	dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));

                                pNativeVlanIntfConfLocalManagedObject->setNativevlanDisable(nativeTmp->getNativevlanDisable());
                                pNativeVlanIntfConfLocalManagedObject->setNativevlanPresence(true);
                                pNativeVlanIntfConfLocalManagedObject->setNativeVlanId(NSM_NATIVE_VLAN_DEFAULT);

                                pPhyIntfLocalMO->setNativeVlanIntfConfig(
                                	WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (
                                    	pNativeVlanIntfConfLocalManagedObject));

                                pPhyIntfLocalMO->addAttributeToBeUpdated ("nativeVlanIntfConfig");
                            }

							vector<UI32>::iterator moremoveiter;
							UI32Range moRemoveVlanrange = UI32Range("");
							vector <UI32> moRemoveVlanRangeVector;
							moRemoveVlanrange = pPhyIntfLocalMO->getVlanRangeRemove();
							moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
							moRemoveVlanrange = UI32Range("");

							for (moremoveiter = moRemoveVlanRangeVector.begin();
								moremoveiter != moRemoveVlanRangeVector.end();moremoveiter++)
							{
								if(*moremoveiter == vlan_id)
								{
									moRemoveVlanRangeVector.erase(moremoveiter);
									string vlanRangeStr =
										UI32Range::getUI32RangeStringFromVector(moRemoveVlanRangeVector);

									moRemoveVlanrange.fromString(vlanRangeStr);
									pPhyIntfLocalMO->setVlanRangeRemove(moRemoveVlanrange);
									pPhyIntfLocalMO->addAttributeToBeUpdated ("vlanRangeRemove");
									break;
								}
							}
						}

						else
						{/*Search if it is a part of trunk or access vlan*/
							if(pPhyIntfLocalMO->getSwMode() == SW_MODE_ACCESS)
			{
								if(pPhyIntfLocalMO->getAccessVlanObjectId() == vlanObjectId)
								{
                                    if (ObjectId::NullObjectId == defaultVlanObjectId) {
									getVlanIntfObjectIdByVlanIfName (
										"1",defaultVlanObjectId,false, pDceSynchronousLinearSeqContext);
                                    }

									pPhyIntfLocalMO->setAccessVlanObjectId (defaultVlanObjectId);
									pPhyIntfLocalMO->addAttributeToBeUpdated ("vlanObjectId");

								}
							}
							else if (pPhyIntfLocalMO->getSwMode() == SW_MODE_TRUNK || pPhyIntfLocalMO->getPvlanModeTrunkPromis())
				  {
								WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanMo =
								    pPhyIntfLocalMO->getNativeVlanIntfConfig();

								NativeVlanIntfConfLocalManagedObject *nativeTmp = pNativeVlanMo.operator-> ();
			    				if(pNativeVlanMo.operator->() != NULL)
								{
									trace (TRACE_LEVEL_DEBUG, "IntfConfLocalWorker::deletePhyIntfVlanReference Clean up native vlan");
								if(nativeTmp->getNativeVlanId() == vlan_id)
					{

									NativeVlanIntfConfLocalManagedObject *pNativeVlanIntfConfLocalManagedObject =
											NativeVlanPhyGetChildMoByParentMo(pPhyIntfLocalMO);

									if(pNativeVlanIntfConfLocalManagedObject != NULL)
									{
										pNativeVlanIntfConfLocalManagedObject->setNativeVlanId(NSM_NATIVE_VLAN_DEFAULT);
										updateWaveManagedObject (pNativeVlanIntfConfLocalManagedObject);
										if (pDceSynchronousLinearSeqContext)
											pDceSynchronousLinearSeqContext->addManagedObjectForGarbageCollection(pNativeVlanIntfConfLocalManagedObject);

									}

									}
					}
									trace (TRACE_LEVEL_DEBUG, "IntfConfLocalWorker::deletePhyIntfVlanReference Clean up allowed vlan");
									vector <UI32> moAddVlanRangeVector;
									UI32Range moAddVlanrange = UI32Range("");
									vector<UI32>::iterator moadditer;

									moAddVlanrange = pPhyIntfLocalMO->getVlanRangeAdd();
									moAddVlanrange.getUI32RangeVector(moAddVlanRangeVector);
									moAddVlanrange = UI32Range("");

									moadditer = moAddVlanRangeVector.begin();

									for (moadditer=moAddVlanRangeVector.begin();
										moadditer != moAddVlanRangeVector.end(); moadditer++)
									{
										if(*moadditer == vlan_id)
										{/*Vlan Id exists, remove from vlan list*/
											moAddVlanRangeVector.erase(moadditer);
					break;
				  }
									}

									string vlanRangeStr =
										UI32Range::getUI32RangeStringFromVector(moAddVlanRangeVector);

									moAddVlanrange.fromString(vlanRangeStr);
									pPhyIntfLocalMO->setVlanRangeAdd(moAddVlanrange);
									pPhyIntfLocalMO->addAttributeToBeUpdated ("vlanRangeAdd");
							}
							}
						}
					//Even if the config is switchport trunk allowed vlan none, rspan vlans can be associated
					if(pPhyIntfLocalMO->getSwMode() == SW_MODE_ACCESS) {
						NSM_PLUG_INFO_S(string("Check whether the rspan_accessvlan needs to be updated"));
						NSM_PLUG_INFO_S(string("Deleted Vlan Id:")+vlan_id);
						NSM_PLUG_INFO_S(string("Rspan Access Vlan Id :")+pPhyIntfLocalMO->getAccessRspanVlan());
						if(vlan_id == pPhyIntfLocalMO->getAccessRspanVlan()) {
							NSM_PLUG_INFO_S(string("Update the rspan_accessvlan"));
							//Update the rspan_accessvlan
							pPhyIntfLocalMO->setAccessRspanVlan (DCM_NSM_VLAN_DEFAULT_VID);
							pPhyIntfLocalMO->addAttributeToBeUpdated("rspan_accessvlan");

							//Update the access vlan
                            if (ObjectId::NullObjectId == defaultVlanObjectId) {
                            	getVlanIntfObjectIdByVlanIfName ("1",defaultVlanObjectId,false, pDceSynchronousLinearSeqContext);
                            }
							pPhyIntfLocalMO->setAccessVlanObjectId (defaultVlanObjectId);
							pPhyIntfLocalMO->addAttributeToBeUpdated ("vlanObjectId");
						}
					}
					else if(pPhyIntfLocalMO->getSwMode() == SW_MODE_TRUNK) {
						NSM_PLUG_INFO_S(string("Check whether the rspan_trunkvlanrange needs to be updated"));
						UI32Range rangeFromMO = pPhyIntfLocalMO->getRspanVlanRangeAdd();
						NSM_PLUG_INFO_S(string("Range from MO:")+rangeFromMO.toString());
						if(rangeFromMO.find(vlan_id)) {
							NSM_PLUG_INFO_S(string("Need to update rspan_trunkvlanrange"));
							vector <UI32> vectorFromMsg;
							vectorFromMsg.push_back(vlan_id);
							string rangeStrFromMsg = UI32Range::getUI32RangeStringFromVector(vectorFromMsg);
							UI32Range rangeFromMsg = UI32Range(rangeStrFromMsg);
							NSM_PLUG_INFO_S(string("Range from msg:")+rangeFromMsg.toString());
							UI32Range updatedRange = rangeFromMO.operator -(rangeFromMsg);
							NSM_PLUG_INFO_S(string("Updated Range:")+updatedRange.toString());
							pPhyIntfLocalMO->setRspanVlanRangeAdd(updatedRange);
							pPhyIntfLocalMO->addAttributeToBeUpdated ("rspan_trunkvlanrange");
						}
					}

					if(pPhyIntfLocalMO->getPvlanModeTrunkBas())
					{
						NSM_PLUG_DBG_S(string("private-vlan trunk mode")+vlan_id);
						if(pPhyIntfLocalMO->getPvlanNativeVlanId() == vlan_id)
						{
							NSM_PLUG_DBG_S(string("Native vlan id deletion -- private-vlan trunk mode")+vlan_id);
							pPhyIntfLocalMO->setPvlanNativeVlanId(0);
							pPhyIntfLocalMO->addAttributeToBeUpdated ("pVlanNativeVlanId");
						}

						if(pPhyIntfLocalMO->getPvlanIsAllowedVlanAll())
						{
							if(pPhyIntfLocalMO->getPvlanRangeRemove().find(vlan_id))
							{
								NSM_PLUG_DBG_S(string("vlan id deletion from pVlanRangeRemove-- private-vlan trunk mode")+vlan_id);
								UI32Range rangeFromMsg(vlan_id, vlan_id);
								UI32Range moVlanrange = pPhyIntfLocalMO->getPvlanRangeRemove();
								pPhyIntfLocalMO->setPvlanRangeRemove(moVlanrange - rangeFromMsg);
								pPhyIntfLocalMO->addAttributeToBeUpdated("pVlanRangeRemove");
							}

						}
						else if(pPhyIntfLocalMO->getPvlanRangeAdd().find(vlan_id))
						{
							NSM_PLUG_DBG_S(string("vlan id deletion from pVlanRangeAdd-- private-vlan trunk mode")+vlan_id);
							UI32Range rangeFromMsg(vlan_id, vlan_id);
							UI32Range moVlanrange = pPhyIntfLocalMO->getPvlanRangeAdd();
							pPhyIntfLocalMO->setPvlanRangeAdd(moVlanrange - rangeFromMsg);
							pPhyIntfLocalMO->addAttributeToBeUpdated("pVlanRangeAdd");
						}
					}

					if (!pPhyIntfLocalMO->getAttributesToBeUpdated().empty()) {
					    updateWaveManagedObject(pPhyIntfLocalMO);
					}
					/* 
					 * ELD records for the VLAN to be deleted are
					 * being removed for the interface.
					 */
					UI32 j;
                    vector<ObjectId>  m_vlanid = pPhyIntfLocalMO->getVlanVector();
                    if (m_vlanid.size() > 0) {
						trace (TRACE_LEVEL_INFO, string ("CLI: no int vlan X;  Remove ELD vlans."));
                        for (j = 0; j < m_vlanid.size(); j++) {
                            if (m_vlanid[j] == ObjectId::NullObjectId) {
                                NSM_PLUG_INFO_S(j + string ("VLAN ObjId Null: continue"));
                                continue;
                            }
                            if(m_vlanid[j] == vlanObjectId)
                            {
						 	 	trace (TRACE_LEVEL_DEBUG, string ("CLI: no int vlan X;  Removing ELD vlan."));
                                pPhyIntfLocalMO->deleteVlanObjectId(m_vlanid[j]);
							 	pPhyIntfLocalMO->addAttributeToBeUpdated("vlanid");
                                updateWaveManagedObject(pPhyIntfLocalMO);
                                break;
                            }
                        }
                    }					
					count++;
				  }
			}
			if(pDceSynchronousLinearSeqContext)
				pDceSynchronousLinearSeqContext->addManagedObjectsForGarbageCollection(*pPhyResults);
			pPhyResults->clear ();
			delete pPhyResults;
		}


		removeVlanClassifierOnPhy(vlanObjectId);
		return WAVE_MESSAGE_SUCCESS;
	}

    /* Code Ends Here */

    void  IntfConfLocalWorker::NsmDeleteVlanIntfSPMessageHandler( NsmDeleteVlanIntfSPMessage *pNsmDeleteVlanIntfSPMessage)
    {
        NSM_PLUG_DBG("Entering ...");
        DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = NULL;
        tracePrintf(TRACE_LEVEL_DEBUG, "IntfConfLocalWorker::NsmDeleteVlanIntfSPMessageHandler: For Db update-- %s",(pNsmDeleteVlanIntfSPMessage->isDBUpdateRequest() ? "True" : "False"));

        if (pNsmDeleteVlanIntfSPMessage->isDBUpdateRequest()) {
            /*  Update Local DB  */
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::deletePhyIntfVlanReference),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::deleteVlanIntfXstpStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };
            pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext(pNsmDeleteVlanIntfSPMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
        } else {
            /*  Sending to Backend  */
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::deleteVlanIntfStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };
            pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext(pNsmDeleteVlanIntfSPMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
        }

        pDceSynchronousLinearSeqContext->execute ();
    }

ResourceId IntfConfLocalWorker::updateBulkPhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
{
	NSM_PLUG_DBG("Entering ...");

	NsmUpdateBulkPhyIntfMessage *pNsmUpdateBulkPhyIntfMessage = dynamic_cast<NsmUpdateBulkPhyIntfMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());
	UI32 size = 0;
	UI32 outbuffNum;
	void *data;
	string null_str = "";

	outbuffNum = NSM_UPDATE_INTf_CONFIG;
    pNsmUpdateBulkPhyIntfMessage->getDuplicateInterfaceOutputBuffer(outbuffNum,size,data);

	NSM_PLUG_DBG_S(string("++++ Command Code = ") +   pNsmUpdateBulkPhyIntfMessage->getCmdCode());

	if(size != 0)
	{
		switch (outbuffNum) {
		case NSM_UPDATE_INTf_CONFIG:
			{
				nsm_intf_all_msg_t *resp_p;
				resp_p = (nsm_intf_all_msg_t *)data;
				u_int32_t i = 0;

				NSM_PLUG_DBG_S(string("NsmLocalInstall - Chassis disable/enable Bulk Message : Total no of physical interfaces Received : ") + resp_p->num_records);
				for (i = 0; i < resp_p->num_records; i++) {

					NSM_PLUG_DBG_S(string("NsmLocalInstall -  Chassis disable/enable Bulk Message : Interface Name : ") + resp_p->intfdetails[i].ifName );

					NsmUpdatePhyIntfMessage message;
					string ifName = resp_p->intfdetails[i].ifName+ 2;
					NsmUtils::getThreeTupleIfName( resp_p->intfdetails[i].ifName+2, ifName, 0);
					message.setIfName(ifName);

					switch(pNsmUpdateBulkPhyIntfMessage->getCmdCode())
					{
					case SLOT_ONLINE:
                        if (i == 0) {
                            NSM_PLUG_INFO_S(string("IntfConfLocalWorker::updateBulkPhyIntfStep -- CmdCode = SLOT_ONLINE"));
                        }
						message.setActive(true);
						message.setCmdCode(NSMUPDATE_PHY_ACTVIVE_STATUS);
						updatePhyIntf(&message, pDceSynchronousLinearSeqContext);
						break;
					}
				}
			}
			break;
		}
	}

	return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfConfLocalWorker::updateBulkPhyIntfMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
{
	NSM_PLUG_DBG("Entering ...");

	NsmUpdateBulkPhyIntfMessage *pNsmUpdateBulkPhyIntfMessage = dynamic_cast<NsmUpdateBulkPhyIntfMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	UI32 size = 0;
	UI32 outbuffNum;
	void *data;
	string null_str = "";

	outbuffNum = NSM_UPDATE_INTf_CONFIG;
    pNsmUpdateBulkPhyIntfMessage->getDuplicateInterfaceOutputBuffer(outbuffNum,size,data);

	NSM_PLUG_DBG_S(string("++++ Command Code = ") +   pNsmUpdateBulkPhyIntfMessage->getCmdCode());

	if(size != 0)
	{
		switch (outbuffNum) {
		case NSM_UPDATE_INTf_CONFIG:
			{
				nsm_intf_all_msg_t *resp_p;
				resp_p = (nsm_intf_all_msg_t *)data;
				u_int32_t i = 0;

				NSM_PLUG_DBG_S(string("NsmLocalInstall - New Node : Total no of physical interfaces Received : ") + resp_p->num_records);
				for (i = 0; i < resp_p->num_records; i++) {

					NSM_PLUG_DBG_S(string("NsmLocalInstall - New Node : Interface Name : ") + resp_p->intfdetails[i].ifName );

					NsmUpdatePhyIntfMessage message;
					string ifName = resp_p->intfdetails[i].ifName+ 2 ;
					NsmUtils::getThreeTupleIfName(resp_p->intfdetails[i].ifName+2, ifName, 0);
					message.setIfName(ifName);

					switch(pNsmUpdateBulkPhyIntfMessage->getCmdCode())
					{
					case CHASSIS_DISABLE:
						/*shutdown*/
                        if (i == 0) {
                            NSM_PLUG_INFO_S(string("IntfConfLocalWorker::updateBulkPhyIntfMOStep -- CmdCode = CHASSIS_DISABLE"));
                        }
						message.setFlag(true);
						message.setCmdCode(NSMUPDATE_PHY_STATUS);
						break;
					case CHASSIS_ENABLE:
						/*no shutdown*/
                        if (i == 0) {
                            NSM_PLUG_INFO_S(string("IntfConfLocalWorker::updateBulkPhyIntfMOStep -- CmdCode = CHASSIS_ENABLE"));
                        }
						message.setFlag(false);
						message.setCmdCode(NSMUPDATE_PHY_DELSTATUS);
						break;
					case SLOT_OFFLINE:
                        if (i == 0) {
                            NSM_PLUG_INFO_S(string("IntfConfLocalWorker::updateBulkPhyIntfMOStep -- CmdCode = SLOT_OFFLINE"));
                        }
						message.setActive(false);
						message.setCmdCode(NSMUPDATE_PHY_INACTVIVE_STATUS);
						break;
					case SLOT_ONLINE:
                        if (i == 0) {
                            NSM_PLUG_INFO_S(string("IntfConfLocalWorker::updateBulkPhyIntfMOStep -- CmdCode = SLOT_ONLINE"));
                        }
						message.setActive(true);
						message.setCmdCode(NSMUPDATE_PHY_ACTVIVE_STATUS);
						break;
					}
					updatePhyIntfMO(&message, pDceSynchronousLinearSeqContext);
				}
			}
			break;
		}
	}

	return status;
}



    void  IntfConfLocalWorker::NsmUpdateBulkPhyIntfMessageHandler( NsmUpdateBulkPhyIntfMessage *pNsmUpdateBulkPhyIntfMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateBulkPhyIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateBulkPhyIntfMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

		DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pNsmUpdateBulkPhyIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
		pDceSynchronousLinearSeqContext->execute();
    }

    ResourceId IntfConfLocalWorker::updatePhyIntfMOWhenSlotOffline (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        NSM_PLUG_DBG("Entering ...");
        m_lastSlotIdMadeOffline = 0;

        NsmLocalSlotOfflineMessage *pNsmLocalSlotOfflineMessage = dynamic_cast<NsmLocalSlotOfflineMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

        UI32 cmdCode = pNsmLocalSlotOfflineMessage->getCmdCode();
        UI32 slotId = pNsmLocalSlotOfflineMessage->getSlotId();

        if (SLOT_OFFLINE == cmdCode) {
            WaveManagedObjectSynchronousQueryContextForUpdate *qctx = new WaveManagedObjectSynchronousQueryContextForUpdate (PhyIntfLocalManagedObject::getClassName());
            qctx->setLocationIdFilter (FrameworkToolKit::getThisLocationId());
            qctx->setSlotFilter (slotId);
            PhyIntfLocalManagedObject *criteriaMo = dynamic_cast<PhyIntfLocalManagedObject *> (qctx->getWaveManagedObjectToAddUpdateAttributes());
            criteriaMo->setActive (false);
            criteriaMo->addAttributeToBeUpdated ("active");
            string sqlStr = "";
            qctx->getSqlForUpdate (sqlStr);
            trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::updatePhyIntfMOWhenSlotOffline: Slot offline/faulty query: - ")+ sqlStr);
            updateMultipleWaveManagedObjects (qctx);
            trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::updatePhyIntfMOWhenSlotOffline: Moved interfaces to inactive state on receiving Slot offline/faulty for slot: - ")+ slotId);

            m_lastSlotIdMadeOffline = slotId;
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    void IntfConfLocalWorker::NsmLocalSlotOfflineMessageHandler (NsmLocalSlotOfflineMessage *pNsmLocalSlotOfflineMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updatePhyIntfMOWhenSlotOffline),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pNsmLocalSlotOfflineMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pDceSynchronousLinearSeqContext->execute();
    }

    void  IntfConfLocalWorker::NsmUpdatePhyIntfMessageHandler( NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[8];

        sequencerSteps[0] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::validatePhyIntfStep);
        sequencerSteps[5] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::postPhyIntfStep);
        sequencerSteps[6] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep);
        sequencerSteps[7] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep);

        if ( (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pNsmUpdatePhyIntfMessage->getCmdCode() == NSMUPDATE_PHY_STATUS) )
        {
            sequencerSteps[1] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep);
            sequencerSteps[2] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updatePhyIntfMOStep);
            sequencerSteps[3] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep);
            sequencerSteps[4] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updatePhyIntfStep);
            /* rollback needs to handle */

        } else {
            sequencerSteps[1] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updatePhyIntfStep);
            sequencerSteps[2] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep);
            sequencerSteps[3] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updatePhyIntfMOStep);
            sequencerSteps[4] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep);
        }

		DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pNsmUpdatePhyIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
		pDceSynchronousLinearSeqContext->execute();
    }

ResourceId IntfConfLocalWorker::updatePoPrimaryLinkStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int status;
        NsmUpdatePoPrimaryLinkMessage *pNsmUpdatePoPrimaryLinkMessage = dynamic_cast<NsmUpdatePoPrimaryLinkMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        switch (pNsmUpdatePoPrimaryLinkMessage->getCmdCode())
        {
            case NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID :
            case NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_OVERRIDE :
            case NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_CLEAR :
            {
                NsmGlobalUpdatePoIntfMessage               message;

                message.setPoId(pNsmUpdatePoPrimaryLinkMessage->getPoId());
                message.setPoName(pNsmUpdatePoPrimaryLinkMessage->getPoName());
                message.setPoPrimaryLinkDomainId(pNsmUpdatePoPrimaryLinkMessage->getPoPrimaryLinkDomainId());
                message.setPoPartnerSystemPriority(pNsmUpdatePoPrimaryLinkMessage->getPoPartnerSystemPriority());
                message.setPoPartnerKey(pNsmUpdatePoPrimaryLinkMessage->getPoPartnerKey());
                message.setPoPartnerSystem(pNsmUpdatePoPrimaryLinkMessage->getPoPartnerSystem());
                message.setCmdCode(pNsmUpdatePoPrimaryLinkMessage->getCmdCode());

                status = sendSynchronously(&message);

                pNsmUpdatePoPrimaryLinkMessage->setReplyCode(message.getReplyCode());
                pNsmUpdatePoPrimaryLinkMessage->setCompletionStatus(message.getCompletionStatus());
                                                                                                                                                            if( status == WAVE_MESSAGE_SUCCESS)
                {                                                                                                                                               return  message.getCompletionStatus();

                }
                return WAVE_MESSAGE_ERROR;
            }
            break;
        }

        return WAVE_MESSAGE_ERROR;
    }

void  IntfConfLocalWorker::NsmUpdatePoPrimaryLinkMessageHandler( NsmUpdatePoPrimaryLinkMessage *pNsmUpdatePoPrimaryLinkMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updatePoPrimaryLinkStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmUpdatePoPrimaryLinkMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

ResourceId IntfConfLocalWorker::updatePoIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {


        NSM_PLUG_DBG("Entering ...");
        ResourceId  status   = WAVE_MESSAGE_ERROR;
		int send_native_vlan_disable_to_eld_flag = 0;
		EldPoIntfMessage *P1 = NULL;
//		NsmPoIntfMessage *m = NULL;
		vector<UI32>    vlanIdVector;
		UI32 native_vlanid = 0, eldAllowedVlanAll = 0;
        UI32Range toBeProvisionedVlanRange;
        NsmUpdatePoIntfSPMessage *pNsmUpdatePoIntfSPMessage = dynamic_cast<NsmUpdatePoIntfSPMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

        PORT_BASED_PROVISION(pDceSynchronousLinearSeqContext,
                pNsmUpdatePoIntfSPMessage->getCmdCode(),
                pNsmUpdatePoIntfSPMessage->getIsPortProfiled());

        auto_ptr<NsmPoIntfMessage> m ( new NsmPoIntfMessage(
                pNsmUpdatePoIntfSPMessage->getPoId(),
                pNsmUpdatePoIntfSPMessage->getPoName(),
                pNsmUpdatePoIntfSPMessage->getPoIfIndex(),
                pNsmUpdatePoIntfSPMessage->getMtu(),
                pNsmUpdatePoIntfSPMessage->getDesc(),
                pNsmUpdatePoIntfSPMessage->getFlag(),
                pNsmUpdatePoIntfSPMessage->getSwModeFlag(),
                pNsmUpdatePoIntfSPMessage->getSwMode(),
                pNsmUpdatePoIntfSPMessage->getIsAllowedVlanAll(),
                pNsmUpdatePoIntfSPMessage->getPoMode(),
                pNsmUpdatePoIntfSPMessage->getPoType(),
                pNsmUpdatePoIntfSPMessage->getPoMinLinks(),
                pNsmUpdatePoIntfSPMessage->getPoAllowedSpeed(),
                pNsmUpdatePoIntfSPMessage->getPoLoadBalanceMode(),
                pNsmUpdatePoIntfSPMessage->getPoPrimaryLinkDomainId(),
                pNsmUpdatePoIntfSPMessage->getPoPartnerSystemPriority(),
                pNsmUpdatePoIntfSPMessage->getPoPartnerKey(),
                pNsmUpdatePoIntfSPMessage->getPoPartnerSystem(),
                pNsmUpdatePoIntfSPMessage->getPoLacpSystemId(),
                pNsmUpdatePoIntfSPMessage->getCmdCode(),
                pNsmUpdatePoIntfSPMessage->getIsPortProfiled(),
                pNsmUpdatePoIntfSPMessage->getPvlanNativeVlanId(),
                pNsmUpdatePoIntfSPMessage->getPvlanIsAllowedVlanAll()));

        switch(pNsmUpdatePoIntfSPMessage->getCmdCode())
        {
            case NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PO :
            case NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PO :
                {
                    string portProfileName = pNsmUpdatePoIntfSPMessage->getPortProfileName();
                    vector<string> portProfiles;
                    if("" != portProfileName) {
                        portProfiles.push_back(portProfileName);
                    }
                    string portProfileDomainName = pNsmUpdatePoIntfSPMessage->getPortProfileDomainName();
                    status = getAllVlansToBeProvisioned(portProfiles, portProfileDomainName, toBeProvisionedVlanRange);
                    tracePrintf(TRACE_LEVEL_INFO, "NSMUPDATE_PO_PORT_PROFILED, toBeProvisionedVlanRange<%s>"
                            , toBeProvisionedVlanRange.toString().c_str() );
                    //Better write a util in APPM and use it here, the same can be
                    //reused to populate all the vlans present in a given pp domain.
                    PROVISION_VLAN_RANGE(pDceSynchronousLinearSeqContext,toBeProvisionedVlanRange,PROVISION_SPECIFIED);
                    m->setPortProfileName(portProfileName);
                    m->setPortProfileDomainName(portProfileDomainName);
                    trace (TRACE_LEVEL_INFO, string ("Sending po-port-profile association to backends poId<") + m->getPoId() 
                            + "> Cmdcode <" + m->getCmdCode() + "> isPortProfiled <" + m->getIsPortProfiled() 
                            + "> port-profile-name<" + m->getPortProfileName() 
                            + "> port-profile-domain-name <" + m->getPortProfileDomainName());
                    break;
                }
                //TODO: For all the other commands which result in vlan provision, add logic here.
            case NSMUPDATE_DISSOCIATE_PORT_PROFILE_WITH_PO :
            case NSMUPDATE_DISSOCIATE_PORT_PROFILE_DOMAIN_WITH_PO : 
                {
                    m->setPortProfileName(pNsmUpdatePoIntfSPMessage->getPortProfileName());
                    m->setPortProfileDomainName(pNsmUpdatePoIntfSPMessage->getPortProfileDomainName());
                    trace (TRACE_LEVEL_INFO,string ("Sending po-port-profile association to backends poId<") + m->getPoId()
                            + ">Cmdcode <" + m->getCmdCode() + "> isPortProfiled <" + m->getIsPortProfiled()
                            + "> port-profile-name<" + m->getPortProfileName()
                            + "> port-profile-domain-name <" + m->getPortProfileDomainName());

                    break;
                }
        }


		UI32 eld_cmdcode = (ELD_INTF_PO_CONF_PRIORITY | ELD_INTF_PO_CONF_ELD_ENABLE |
			                ELD_INTF_PO_CONF_ELD_DISABLE);
		if (IS_CMDCODE_SET(pNsmUpdatePoIntfSPMessage->getCmdCode(), eld_cmdcode) ||
			(NSMUPDATE_PO_NATIVEVLANID == pNsmUpdatePoIntfSPMessage->getCmdCode()) || 
			(NSMUPDATE_PO_NATIVEVLANDISABLE == pNsmUpdatePoIntfSPMessage->getCmdCode())) {
			P1 = new EldPoIntfMessage();
			P1->setPoId(pNsmUpdatePoIntfSPMessage->getPoId());
			P1->setPoName(pNsmUpdatePoIntfSPMessage->getPoName());
			P1->setPoIfIndex(pNsmUpdatePoIntfSPMessage->getPoIfIndex()); 
			P1->setCmdCode(pNsmUpdatePoIntfSPMessage->getCmdCode());
		}

        NSM_PLUG_INFO_S( string ("UP_PO_L Sending to Backend : ") + pNsmUpdatePoIntfSPMessage->getPoId() + string("Cmdcode :") + pNsmUpdatePoIntfSPMessage->getCmdCode());


        switch(pNsmUpdatePoIntfSPMessage->getCmdCode())
        {
            case NSMUPDATE_PO_SW_ADD_ACCESS_VLAN:
            case NSMUPDATE_PO_SW_ADD_TRUNK_VLAN:
            case NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN:
            case NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN:
            {
                UI32Range vlanRange = pNsmUpdatePoIntfSPMessage->getVlanRange();
                CMDCODE_BASED_PROVISION(pDceSynchronousLinearSeqContext,
                        pNsmUpdatePoIntfSPMessage->getCmdCode(), vlanRange);
                m->setVlanRange(vlanRange);
                break;
            }
            case NSMUPDATE_PO_SET_MAC_ADDRESS:
            case NSMUPDATE_PO_SET_CTAG:
            case NSMUPDATE_PO_SET_MAC_GROUP:
			case NSMUPDATE_PO_PVLAN_SET_CTAG:
			{
				UI32Range vlanRange = pNsmUpdatePoIntfSPMessage->getVlanRange();
				PROVISION_VLAN_RANGE(pDceSynchronousLinearSeqContext, pNsmUpdatePoIntfSPMessage->getVlanRange(), PROVISION_SPECIFIED);

                m->setVlanRange(vlanRange);
				m->setMac(pNsmUpdatePoIntfSPMessage->getMac());
				m->setCtagId(pNsmUpdatePoIntfSPMessage->getCtagId());
				m->setMacGroupId(pNsmUpdatePoIntfSPMessage->getMacGroupId());
				break;
			}
            case NSMUPDATE_PO_DEL_MAC_ADDRESS:
            case NSMUPDATE_PO_DEL_MAC_GROUP:
            case NSMUPDATE_PO_DEL_CTAG:
			case NSMUPDATE_PO_PVLAN_DEL_CTAG:
			{
                m->setVlanRange(pNsmUpdatePoIntfSPMessage->getVlanRange());
				m->setMac(pNsmUpdatePoIntfSPMessage->getMac());
				m->setCtagId(pNsmUpdatePoIntfSPMessage->getCtagId());
				m->setMacGroupId(pNsmUpdatePoIntfSPMessage->getMacGroupId());
				break;
			}

		    case NSMUPDATE_PO_NATIVEVLANDISABLE:
			  {
				  m->setNativevlanDisable(pNsmUpdatePoIntfSPMessage->getNativevlanDisable());
				  native_vlanid = pNsmUpdatePoIntfSPMessage->getNativeVlanId();

				  /* ELD specific message replaying for TR 374321*/
				  if (native_vlanid != 0)
				  {
					  P1->setCmdCode(ELD_INTF_PO_CONF_ELD_ENABLE);
					  vlanIdVector.push_back(native_vlanid);
					  P1->setVlanidList(vlanIdVector);
					  send_native_vlan_disable_to_eld_flag = 1;
					  trace(TRACE_LEVEL_INFO, string ("NSMUPDATE_PO_NATIVEVLANDISABLE, Replay for poname :") + P1->getPoName());
					  trace(TRACE_LEVEL_INFO, string ("NSMUPDATE_PO_NATIVEVLANDISABLE, Replay for Native-vlan :") + native_vlanid);
				  }
				  break;
		   	  } 

            case NSMUPDATE_PO_IGNORE_SPLIT:
            case NSMUPDATE_PO_NO_IGNORE_SPLIT:
                m->setIgnoreSplit(pNsmUpdatePoIntfSPMessage->getIgnoreSplit());
                NSM_PLUG_INFO_S(string(" IntfConfLocalWorker::updatePoIntfStep,  ignore_split :  ") + m->getIgnoreSplit());

                break;

			case NSMUPDATE_PO_NATIVEVLANID:
				{
                   PROVISION_VLAN(pDceSynchronousLinearSeqContext,
                        pNsmUpdatePoIntfSPMessage->getNativeVlanId());
					m->setNativeVlanId(pNsmUpdatePoIntfSPMessage->getNativeVlanId());
					eldAllowedVlanAll = pNsmUpdatePoIntfSPMessage->getEldAllowedVlanAll();

					/* ELD specific message replaying for TR 376145*/
					if (eldAllowedVlanAll != 0)
					{
						P1->setCmdCode(ELD_INTF_PO_CONF_ELD_ENABLE);
						vlanIdVector.push_back(eldAllowedVlanAll);
						P1->setVlanidList(vlanIdVector);
						send_native_vlan_disable_to_eld_flag = 1;
						trace(TRACE_LEVEL_INFO, string ("NSMUPDATE_PO_NATIVEVLANID, Replay for poname :") + P1->getPoName());
						trace(TRACE_LEVEL_INFO, string ("NSMUPDATE_PO_NATIVEVLANID, Replay for Native-vlan :") + eldAllowedVlanAll);
					}

					break;
				}

			case NSMUPDATE_PO_PVLAN_NATIVE_VLANID:
            {
               PROVISION_VLAN(pDceSynchronousLinearSeqContext,
                    pNsmUpdatePoIntfSPMessage->getPvlanNativeVlanId());
            }
			case NSMUPDATE_PO_PVLAN_NATIVE_VLANID_DISABLE:
                 m->setPvlanNativeVlanId(pNsmUpdatePoIntfSPMessage->getPvlanNativeVlanId());
                 break;

			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ALL:
			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_NONE:
                 m->setPvlanIsAllowedVlanAll(pNsmUpdatePoIntfSPMessage->getPvlanIsAllowedVlanAll());
                 break;

			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ADD:
			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_REMOVE:
			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_EXCEPT:
            {
                UI32Range vlanRange = pNsmUpdatePoIntfSPMessage->getPvlanRange();
                CMDCODE_BASED_PROVISION(pDceSynchronousLinearSeqContext,
                        pNsmUpdatePoIntfSPMessage->getCmdCode(), vlanRange);
                m->setPvlanRange(vlanRange);
            }
                 break;

			case NSMUPDATE_PO_PVLAN_HOST_ASSOCIATION:
            {
                PROVISION_VLAN(pDceSynchronousLinearSeqContext,
                        pNsmUpdatePoIntfSPMessage->getPvlanHostSecVlanId());
                PROVISION_VLAN(pDceSynchronousLinearSeqContext,
                        pNsmUpdatePoIntfSPMessage->getPvlanHostPrimVlanId());
            }
			case NSMUPDATE_PO_DELPVLAN_HOST_ASSOCIATION_ALL:
			{
				trace(TRACE_LEVEL_INFO,string("sending to in intfconfLocalWorker updatePoIntf "));
         		m->setPvlanHostPrimVlanId(pNsmUpdatePoIntfSPMessage->getPvlanHostPrimVlanId());
         		m->setPvlanHostSecVlanId(pNsmUpdatePoIntfSPMessage->getPvlanHostSecVlanId());
         		break;
			}
            case ELD_INTF_PO_CONF_PRIORITY:
                 P1->setPrio(pNsmUpdatePoIntfSPMessage->getPrio()); 

                 NSM_PLUG_INFO_S(string(" Cmdcode :") +  P1->getCmdCode() + (" ELD - PRIO :") + P1->getPrio());
                 break;
            case ELD_INTF_PO_CONF_ELD_ENABLE:
                 P1->setVlanidList(pNsmUpdatePoIntfSPMessage->getVlanidList()); 
                 NSM_PLUG_INFO_S(string(" Cmdcode :") +  P1->getCmdCode());
                 break;
            case ELD_INTF_PO_CONF_ELD_DISABLE:
                 P1->setVlanidList(pNsmUpdatePoIntfSPMessage->getVlanidList()); 
                 NSM_PLUG_INFO_S(string(" Cmdcode :") +  P1->getCmdCode());
                 break;
            case NSMUPDATE_PO_SET_IPV6_RA_GUARD:
            case NSMUPDATE_PO_RESET_IPV6_RA_GUARD:
                m->setRaGuard(pNsmUpdatePoIntfSPMessage->getRaGuard());
                break;
        }

		if (
				((pNsmUpdatePoIntfSPMessage->getCmdCode()) != ELD_INTF_PO_CONF_PRIORITY) 
									&& 
				(
			 		((pNsmUpdatePoIntfSPMessage->getCmdCode()) != 
					 					ELD_INTF_PO_CONF_ELD_ENABLE)	
										&& 
			 		((pNsmUpdatePoIntfSPMessage->getCmdCode()) != 
					 					ELD_INTF_PO_CONF_ELD_DISABLE)	
				)
			) 
		{
			trace(TRACE_LEVEL_DEBUG,string("before sending to backend intfconfLocalWorker updatePoIntf "));
            status = sendSynchronouslyToWaveClient ("nsm", m.get());
			trace(TRACE_LEVEL_DEBUG,string("after sending to backend intfconfLocalWorker updatePoIntf "));
			NSM_PLUG_INFO_S(string(" getPvlanHostPrimVlanId intfconfLocalWorker  :") +  m->getPvlanHostPrimVlanId() );

        NSM_PLUG_INFO_S( string ("UP_PO_L Receiving response from Backend : ") + pNsmUpdatePoIntfSPMessage->getPoId() + string("Cmdcode :") + pNsmUpdatePoIntfSPMessage->getCmdCode() + string("status :") + status + string("backend_clientStatus :") + m->getClientStatus());
			if( status == WAVE_MESSAGE_SUCCESS)
			{
				if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
                    status = m->getCompletionStatus();
					return status;
					}

				if ( (0 != m->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != m->getClientStatus() ) )  {
					trace(TRACE_LEVEL_ERROR, string("Resp:  UP_PO_L Client Error MsgId: ") +
							m->getMessageId() +
							FrameworkToolKit::localize(m->getClientStatus()));
                    status = m->getClientStatus();
					return status;
				}
			}
            HANDLE_PROVISION_RESPONSE(m);
		}	
      if (IS_CMDCODE_SET(pNsmUpdatePoIntfSPMessage->getCmdCode(), eld_cmdcode) ||
          (send_native_vlan_disable_to_eld_flag)) {

        if (send_native_vlan_disable_to_eld_flag)
          trace(TRACE_LEVEL_INFO, string("Sending for NATIVE_VLAN"));
          status = sendSynchronouslyToWaveClient ("eld",P1);
          NSM_PLUG_INFO_S( string ("UP_PO_L Receiving response from Backend : ") + pNsmUpdatePoIntfSPMessage->getPoId() + string("Cmdcode :") + pNsmUpdatePoIntfSPMessage->getCmdCode() + string("status :") + status + string("backend_clientStatus :") + P1->getClientStatus());

          if( status == WAVE_MESSAGE_SUCCESS)
          {
            if(P1->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
              status = P1->getCompletionStatus();
              delete P1;
              return status;
            }

            if ( (0 != P1->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != P1->getClientStatus() ) )  {
              trace(TRACE_LEVEL_ERROR, string("Resp:  UP_PO_L Client Error MsgId: ") +
                  P1->getMessageId() +
                  FrameworkToolKit::localize(P1->getClientStatus()));
              status = P1->getClientStatus();
              delete P1;
              return status;
            }
          }
        }

        if(P1){
          delete P1;
          P1 = NULL;
        }
    return status;
    }



	ResourceId IntfConfLocalWorker::validatePoIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        NSM_PLUG_DBG("Entering validatePoIntfStep...");
        ResourceId  status   = WAVE_MESSAGE_SUCCESS;
        NsmUpdatePoIntfSPMessage *pNsmUpdatePoIntfSPMessage = dynamic_cast<NsmUpdatePoIntfSPMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());
        if (pNsmUpdatePoIntfSPMessage->getCmdCode() == NSMUPDATE_PO_ALLOWEDSPEED) {
        	if ((LINE_SPEED_40000 == pNsmUpdatePoIntfSPMessage->getPoAllowedSpeed()) && (!(Capabilities::isCapabilityPresent(PO_40G_SPEED_CAPABILITY)))) {
        		trace (TRACE_LEVEL_ERROR, string ("IntfConfLocalWorker::validatePoIntfStep : 40G speed not supported."));
        		status = WRC_NSM_PO_40G_SPEED_NOT_SUPPORTED_ERROR;
        	}
        }

        if (pNsmUpdatePoIntfSPMessage->getCmdCode() == NSMUPDATE_PO_SET_IPV6_RA_GUARD) {
            if(!(Capabilities::instance()->isCapabilityPresent(IPV6_RA_GUARD))) {
        	    trace (TRACE_LEVEL_ERROR, string ("IntfConfLocalWorker::validatePoIntfStep : IPv6 RA gaurd setting no allowed on this platform"));
                status = WRC_NSM_IPV6_RA_GUARD_NOT_SUPPORTED;
            }
        }
        pNsmUpdatePoIntfSPMessage->setCompletionStatus(status);
        return status;
    }


	void  IntfConfLocalWorker::NsmUpdatePoIntfSPMessageHandler( NsmUpdatePoIntfSPMessage *pNsmUpdatePoIntfSPMessage)
    {
        NSM_PLUG_DBG("Entering ...");
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
        	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::validatePoIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updatePoIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pContext = new DceSynchronousLinearSeqContext (pNsmUpdatePoIntfSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pContext->execute ();
    }

    ResourceId IntfConfLocalWorker::updateVlanIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int status = WAVE_MESSAGE_SUCCESS;
        NsmUpdateVlanIntfSPMessage *pNsmUpdateVlanIntfSPMessage = dynamic_cast<NsmUpdateVlanIntfSPMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        UI32 vlanId = pNsmUpdateVlanIntfSPMessage->getVlanId();
        trace(TRACE_LEVEL_INFO,string("IntfConfLocalWo updateVlanIntfStep vlanId  ")+vlanId);
        if (!ProvisioningUtil::isVlanProvisioned(vlanId)) {
            trace(TRACE_LEVEL_INFO,string("IntfConfLocalWo updateVlanIntfStep VLAN is not provisioned...  "));
            return status;
        }

        UI32 cmdCode = pNsmUpdateVlanIntfSPMessage->getCmdCode();
        UI32Range pVlanSecAssocRange = pNsmUpdateVlanIntfSPMessage->getPvlanSecAssocRange();

        if((cmdCode == NSMUPDATE_VLAN_PVLAN_ADD_SEC_ASSOC_TYPE) && (!pVlanSecAssocRange.isEmpty())) {
            PROVISION_VLAN_RANGE(NULL, pVlanSecAssocRange, PROVISION_SPECIFIED);
        }

        auto_ptr<NsmVlanIntfMessage> m ( new NsmVlanIntfMessage(vlanId,
                pNsmUpdateVlanIntfSPMessage->getVlanName(),
                pNsmUpdateVlanIntfSPMessage->getVlanIfIndex(),
                pNsmUpdateVlanIntfSPMessage->getDesc(),
                pNsmUpdateVlanIntfSPMessage->getFlag(), cmdCode,
                pNsmUpdateVlanIntfSPMessage->getPvlanType()));
        NSM_PLUG_INFO_S( string ("Intfconflocalworker Sending to Backend  update: ") + string("Cmdcode :") + cmdCode);
        m->setPvlanSecAssocRange(pVlanSecAssocRange);
        m->setRemoteSpanEnabled(pNsmUpdateVlanIntfSPMessage->getRemoteSpanEnabled());

        bool isReplayInProgress = pNsmUpdateVlanIntfSPMessage->isReplayInProgress();
        m->setReplayInProgress(isReplayInProgress);
        NSM_PLUG_DBG_S( string (" Local  isReplayInProgress  --\n") + isReplayInProgress);

        trace(TRACE_LEVEL_DEBUG,string("before sending to backend  updateVlanIntfStep"));
        status = sendSynchronouslyToWaveClient ("nsm",m.get());
        trace(TRACE_LEVEL_DEBUG,string("after  sending to backend  updateVlanIntfStep"));

        NSM_PLUG_INFO_S( string ("UP_VLAN_L Receiving response from backend  ") + vlanId + string("Cmdcode :") +  cmdCode + string(":status :") + status + string("backend_clientStatus :") + m->getClientStatus());

        if( status == WAVE_MESSAGE_SUCCESS)
        {
            if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
                status = m->getCompletionStatus();
                return status;
            }

            if ( (0 != m->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != m->getClientStatus() ) )  {
                trace(TRACE_LEVEL_ERROR, string("Resp: UP_VLAN_L  Client Error MsgId: ") +
                        m->getMessageId() +
                        FrameworkToolKit::localize(m->getClientStatus()));
                status = m->getClientStatus();
				return status;
            }
            HANDLE_PROVISION_RESPONSE(m.get()); 
        }
        //PVLAN TODO hk
        //return WAVE_MESSAGE_SUCCESS;
        return status;
    }

    void  IntfConfLocalWorker::NsmUpdateVlanIntfSPMessageHandler( NsmUpdateVlanIntfSPMessage *pNsmUpdateVlanIntfSPMessage)
    {
        NSM_PLUG_DBG("Entering ...");

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            /* Sending to backend */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateVlanIntfStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmUpdateVlanIntfSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

ResourceId IntfConfLocalWorker::sendVlagConfigToNsmStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
      NsmLocalVlagConfigMessage *pNsmLocalVlagConfigMessage = dynamic_cast<NsmLocalVlagConfigMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

      NsmVlagConfigMessage *m = new NsmVlagConfigMessage();
      int status = WAVE_MESSAGE_SUCCESS;
      trace(TRACE_LEVEL_INFO,string("Entered sendVlagConfigToNsmStep"));
      m->setCmdCode(pNsmLocalVlagConfigMessage->getCmdCode());
      trace(TRACE_LEVEL_INFO,string("Entered cmdCode ")+pNsmLocalVlagConfigMessage->getCmdCode());

      switch(pNsmLocalVlagConfigMessage->getCmdCode()){
      case NSMUPDATE_VLAG_LOADBALANCE:
          {
              m->setLoadBalanceMode(pNsmLocalVlagConfigMessage->getLoadBalanceMode());
              m->setPoId(pNsmLocalVlagConfigMessage->getPoId());
              trace(TRACE_LEVEL_INFO,string("loadbalance is ")+m->getLoadBalanceMode());
              trace(TRACE_LEVEL_INFO,string("port-channel id is ")+m->getPoId());
              break;
          }
      default :
          break;
      }

      status = sendSynchronouslyToWaveClient("nsm",m);
      if(status != WAVE_MESSAGE_SUCCESS)
      {
          NSM_PLUG_INFO_S(string("VLAG:: Sys Priority: Send To Daemon, Message-Status=[") +
              status + string("]:") +
              FrameworkToolKit::localize((status)));
      }

      trace(TRACE_LEVEL_INFO,string("client msg status is ")+status);
      if(m)
          delete m;
      return status;
  }


  ResourceId IntfConfLocalWorker::updateVlagConfigMOStep (PrismSynchronousLinearSequencerContext
      *pPrismSynchronousLinearSequencerContext)
  {
      NsmLocalVlagConfigMessage *pNsmLocalVlagConfigMessage = dynamic_cast<NsmLocalVlagConfigMessage *>
          (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

      int createFlag = 0;
      UI32 numberOfResults = 0;
	  UI32 poId = pNsmLocalVlagConfigMessage->getPoId();
	  UI32 defaultMode = (poLoadBalanceModeEnum)LOADBAL_MODE_SRC_DST_IP_MAC_VID_PORT;;
	//  UI32 rbridgeId = DcmToolKit::getLocalMappedId();
      ResourceId status   = WAVE_MESSAGE_ERROR;

      trace(TRACE_LEVEL_INFO,string("cmdCode is ")+ pNsmLocalVlagConfigMessage->getCmdCode());

      WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NsmVlagConfigManagedObject::getClassName());
      syncQueryCtxt.addAndAttribute(new AttributeUI32(poId, "poId"));
	  //syncQueryCtxt.addAndAttribute(new AttributeUI32(poId, "rbridgeId"));
      vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

      NsmVlagConfigManagedObject *pNsmVlagConfigManagedObject = NULL;

      if (NULL != pResults) {
          numberOfResults = pResults->size ();
          if (0 == numberOfResults) {
              NSM_PLUG_DBG("Ecmp Local Config Managed Object doesn't exist...");
              createFlag = 1;
          }else if (1 == numberOfResults) {
              createFlag = 0;
          } else {
              trace(TRACE_LEVEL_INFO,string("Entered numberOfREsults WAVE_MESSAGE_ERROR"));
              status = WAVE_MESSAGE_ERROR;
          }
		  /* do not update MO for default config*/
          if(createFlag == 1 && pNsmLocalVlagConfigMessage->getLoadBalanceMode()!= defaultMode){
              pNsmVlagConfigManagedObject = new
                  NsmVlagConfigManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
              pNsmVlagConfigManagedObject->setLoadBalanceMode(7);
              pNsmVlagConfigManagedObject->setPoId(0);
	//		  pNsmVlagConfigManagedObject->setRbridgeId(0);
              trace(TRACE_LEVEL_INFO,string("created MO!!!"));
          }
      }
    if(createFlag == 0)
      {
          pNsmVlagConfigManagedObject = dynamic_cast<NsmVlagConfigManagedObject *>((*pResults)[0]);
          trace(TRACE_LEVEL_INFO,string("updated MO!!!"));
      }
	if(pNsmLocalVlagConfigMessage->getLoadBalanceMode()!= defaultMode){
      switch(pNsmLocalVlagConfigMessage->getCmdCode()){
      case NSMUPDATE_VLAG_LOADBALANCE:
          {
              pNsmVlagConfigManagedObject->setLoadBalanceMode (
                  pNsmLocalVlagConfigMessage->getLoadBalanceMode());
              pNsmVlagConfigManagedObject->setPoId(
                  pNsmLocalVlagConfigMessage->getPoId());
              trace(TRACE_LEVEL_INFO,string("loadbalancemode written in db")+
                  pNsmVlagConfigManagedObject->getLoadBalanceMode());
              trace(TRACE_LEVEL_INFO,string("Port-channel id written in db")+
                  pNsmVlagConfigManagedObject->getPoId());
              break;
          }
      default:
          break;
      }
		updateWaveManagedObject(pNsmVlagConfigManagedObject);
      //  pNsmVlagConfigManagedObject->setRbridgeId(rbridgeId);
        //trace(TRACE_LEVEL_INFO,string("rbridgeid is")+pNsmVlagConfigManagedObject->getRbridgeId());
      pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
      delete pResults;
	}
else {
	/* delete the MO if default config is set*/
	if(pNsmVlagConfigManagedObject){
		delete pNsmVlagConfigManagedObject;
	}
}
      status = WAVE_MESSAGE_SUCCESS;

      pNsmLocalVlagConfigMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

      trace(TRACE_LEVEL_INFO,string("status is")+status);
      return status;
  }


  void  IntfConfLocalWorker::NsmLocalVlagConfigMessageHandler( NsmLocalVlagConfigMessage *pNsmLocalVlagConfigMessage)
  {
      trace(TRACE_LEVEL_INFO,string("Entered NsmLocalVlagConfigMessageHandler"));
      PrismSynchronousLinearSequencerStep sequencerSteps[] =
      {   /* Sending to backend */
          reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::sendVlagConfigToNsmStep),
          reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),

          reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateVlagConfigMOStep),

          reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),

          reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
          reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
      };

      PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmLocalVlagConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

      pPrismSynchronousLinearSequencerContext->execute ();
  }


	void  IntfConfLocalWorker::NsmUpdateLacpConfigSPMessageHandler( NsmUpdateLacpConfigSPMessage *pNsmUpdateLacpConfigSPMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
//          reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

//          reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateLacpConfigSPStep),

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

		DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pNsmUpdateLacpConfigSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
		pDceSynchronousLinearSeqContext->execute();
    }



    ResourceId IntfConfLocalWorker::updateLacpConfigSPStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        NsmUpdateLacpConfigSPMessage *pNsmUpdateLacpConfigSPMessage = dynamic_cast<NsmUpdateLacpConfigSPMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

		int status = WAVE_MESSAGE_SUCCESS;
    	NsmClientUpdateLacpConfigMessage *m = new NsmClientUpdateLacpConfigMessage();
    	NsmClientUpdateGlobalConfigMessage *nsmMsg = new NsmClientUpdateGlobalConfigMessage();


        m->setSystemPriority (pNsmUpdateLacpConfigSPMessage->getSystemPriority());
        m->setDot1xenable (pNsmUpdateLacpConfigSPMessage->getDot1xenable());
        m->setDot1xtesttimeout (pNsmUpdateLacpConfigSPMessage->getDot1xtesttimeout());
        m->setCmdCode (pNsmUpdateLacpConfigSPMessage->getCmdCode());

		trace (TRACE_LEVEL_INFO, string ("\tupdateLacpConfigSPStep :: received message "));

		switch (pNsmUpdateLacpConfigSPMessage->getCmdCode()) {

		  case LACPUPDATE_SYS_PRIO:
			status = sendSynchronouslyToWaveClient ("lacp",m);
			if (status != WAVE_MESSAGE_SUCCESS) {
            	NSM_PLUG_INFO_S(string("LACP:: Sys Priority: Send To Daemon, Message-Status=[") +
                                    status + string("]:") +
                                    FrameworkToolKit::localize((status)));
			} else if (m->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
	            NSM_PLUG_INFO_S(string("LACP:: Sys Priority: Send To Daemon, Client-Status=[") +
                                    m->getClientStatus() + string("]:") +
                                    FrameworkToolKit::localize(m->getClientStatus()));
				status = m->getClientStatus();
				pNsmUpdateLacpConfigSPMessage->setCompletionStatus(status);
			}
			break;
		  case DOT1XUPDATE_STATUS:
			status = sendSynchronouslyToWaveClient ("dauthd",m);
			break;
		  case DOT1XUPDATE_TESTTIMEOUT:
			status = sendSynchronouslyToWaveClient ("dauthd",m);
			break;
		  case NSMUPDATE_NATIVE_VLAN:
			{
				nsmMsg->setNativevlanDisable(pNsmUpdateLacpConfigSPMessage->getNativevlanDisable());
				nsmMsg->setCmdCode (pNsmUpdateLacpConfigSPMessage->getCmdCode());
				status = sendSynchronouslyToWaveClient ("nsm",nsmMsg);
				if( status == WAVE_MESSAGE_SUCCESS){
					if(  nsmMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
						//bpatel : To avoid memory leak
                        status = nsmMsg->getCompletionStatus();
                        delete nsmMsg;
                        if(m){
							delete m;
						}
                        return status;
                    }
				//bpatel : To avoid memory leak
                if(nsmMsg){
                    delete nsmMsg;
                }
                //bpatel : To avoid memory leak
                if(m){
                    delete m;
                }
 

                /*fix added for defect 379169 for Physical interfaces*/
				{	
					UI32 v, native_vlanid = 0;
					char buf[6] = {0};
					string vlanId;
					vector<ObjectId>::iterator search_native;
					PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
					WaveManagedObjectSynchronousQueryContext syncQueryCtxt1 (PhyIntfLocalManagedObject::getClassName());
					vector<WaveManagedObject *> *pResultsManagedObjects = querySynchronously (&syncQueryCtxt1);
					ObjectId pVlanIntfObjId = ObjectId::NullObjectId;
					NativeVlanIntfConfLocalManagedObject *pNativeVlanIntfConfLocalManagedObject = NULL;
					if (NULL != pResultsManagedObjects) {
						UI32 numberOfResults = pResultsManagedObjects->size ();
						if (0 == numberOfResults) {
							trace(TRACE_LEVEL_DEBUG," No MO in PhyIntfLocalManagedObject found");
						}
                        else {
						    trace(TRACE_LEVEL_DEBUG, string(" numberOfResults for PhyIntfLocalManagedObject = ") + numberOfResults);
							for(v = 0; v <numberOfResults; v++)       // loop through all physical interfaces
							{
								pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResultsManagedObjects)[v]);
                                pNativeVlanIntfConfLocalManagedObject = NativeVlanPhyGetChildMoByParentMo(pPhyIntfLocalManagedObject);
                                if(NULL == pNativeVlanIntfConfLocalManagedObject)
                                   trace (TRACE_LEVEL_DEBUG, string ("\tupdateLacpConfigSPStep :: pNativeVlanIntfConfLocalManagedObject is NULL "));
                                else {
                                       native_vlanid = pNativeVlanIntfConfLocalManagedObject->getNativeVlanId();
                                       if (pDceSynchronousLinearSeqContext)
						                  pDceSynchronousLinearSeqContext->addManagedObjectForGarbageCollection(pNativeVlanIntfConfLocalManagedObject);
                                       vector<ObjectId>  m_vlanid = pPhyIntfLocalManagedObject->getVlanVector();
								       if (m_vlanid.size() > 0) { // . ELD is configured and Native Vlan is in the range of allowed Vlans
                                          sprintf(buf, "%d", native_vlanid);
                                          vlanId = std::string(buf);
									      trace(TRACE_LEVEL_INFO, string("Check for ELD-status of native vlan ") + buf);
									      getVlanIntfObjectIdByVlanIfName(vlanId, pVlanIntfObjId, false, pDceSynchronousLinearSeqContext);

                                          search_native = find(m_vlanid.begin(), m_vlanid.end(), pVlanIntfObjId);
                                          trace(TRACE_LEVEL_INFO, string("native_Vlan  ") + native_vlanid );

                                          if (search_native != m_vlanid.end())
									      {
										      // create the message to replay and send it to eldd daemon.
									    	  EldClientUpdatePhyIntfMessage *K1 = new EldClientUpdatePhyIntfMessage();
										      InterfaceType ifType = (InterfaceType)pPhyIntfLocalManagedObject->getIfType();
										      string ifName = pPhyIntfLocalManagedObject->getIfName();
										      vector<UI32>    vlanIdVector;

                                              trace(TRACE_LEVEL_DEBUG, string ("IFtYPE :") + ifType);
										      if (ifType == IF_TYPE_GI)
										      {
                                                  K1->setIfType(PHY_INT_GI);
										      }
										      else if (ifType == IF_TYPE_TE)
										      {
											      K1->setIfType(PHY_INT_TE);
                                              }
                                              else if (ifType == IF_TYPE_FO)
                                              {
                                                  K1->setIfType(PHY_INT_FO);
                                              }
                                              K1->setOpCode(ELD_INTF_CONF_ELD_ENABLE);
										      if(true  == NsmUtils::isValidThreeTuple(ifName) ) {
                                                  string twoTuppleIfName;
											      NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
											      ifName = twoTuppleIfName;
										      }
										      K1->setIfName (ifName);
										      trace(TRACE_LEVEL_INFO, string ("Replay of NATIVE-VLAN - ifName :") + ifName);
										      vlanIdVector.push_back(native_vlanid);
										      K1->setVlanidList(vlanIdVector);

										      status = sendSynchronouslyToWaveClient ("eld",K1);
										      pNsmUpdateLacpConfigSPMessage->setCompletionStatus(K1->getCompletionStatus());	
										      NSM_PLUG_DBG_S( string ("UP_PHY_L Response from Backend : ") + ifName + string("Cmdcode :") 
											  + pNsmUpdateLacpConfigSPMessage->getCmdCode() + string("status :")
											  + status + string("backend clientStatus :") + K1->getClientStatus());
										      if( status == WAVE_MESSAGE_SUCCESS) {
											      if( pNsmUpdateLacpConfigSPMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)  {
											          status = pNsmUpdateLacpConfigSPMessage->getCompletionStatus();
											          delete K1;
											          return  status;
											      }

											      if ( (0 != K1->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != K1->getClientStatus() ) )  {
												      trace(TRACE_LEVEL_ERROR, string("Resp: UP_PHY_L  Client Error MsgId: ") +
													  K1->getMessageId() +
													  FrameworkToolKit::localize(K1->getClientStatus()));
                                                      status = NsmUtils::mapBackEndErrorsToDcmdErrors(K1->getClientStatus()) ;
												      delete K1;
											          return status;
											     } 
										      }/*End ( status == WAVE_MESSAGE_SUCCESS)*/
										      delete K1;
										      K1 = NULL;
									      }//(search_native != m_vlanid.end()
								       } // m_vlanid.size() > 0 
                                    }//else ends
							     } // for loop ends
                              }// else (0 == numberOfResults)
                              WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsManagedObjects);  
                           }//(NULL != pResultsManagedObjects)
				        }//Fix end for defect 379169 for physical interfaces.
                        {
                            /* Fix begin for defect 379169 for PO interfaces */
                            UI32 vPo, native_vlanidPo = 0;
                            char bufPo[6] = {0};
                            string vlanIdPo;
                            vector<ObjectId>::iterator search_nativePo;
                            PoIntfManagedObject *pPoIntfManagedObject = NULL;
                            WaveManagedObjectSynchronousQueryContext syncQueryCtxt2 (PoIntfManagedObject::getClassName());
                            vector<WaveManagedObject *> *pResultsManagedObjectsPo = querySynchronously (&syncQueryCtxt2);
                            ObjectId pVlanIntfObjIdPo = ObjectId::NullObjectId;
                            WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanIntfConfLocalManagedObjectPo;
                            if (NULL != pResultsManagedObjectsPo) {
                               UI32 numberOfResultsPo = pResultsManagedObjectsPo->size ();
                               if (0 == numberOfResultsPo) {
                                  trace(TRACE_LEVEL_DEBUG," No MO in PoIntfManagedObject t found");
                               }
                               else
                               {
                                   trace(TRACE_LEVEL_DEBUG, string(" numberOfResults for PoIntfManagedObject = ") + numberOfResultsPo);
                                   for(vPo = 0; vPo <numberOfResultsPo; vPo++)       // loop through all PO interfaces
                                   {
                                       pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResultsManagedObjectsPo)[vPo]);
                                       pNativeVlanIntfConfLocalManagedObjectPo = WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (pPoIntfManagedObject->getNativeVlanIntfConfig());
                                       if(NULL == pNativeVlanIntfConfLocalManagedObjectPo.operator->())
                                          trace (TRACE_LEVEL_DEBUG, string ("\tupdateLacpConfigSPStep-po :: pNativeVlanIntfConfLocalManagedObjectPo is NULL "));
                                        else {
                                            native_vlanidPo = pNativeVlanIntfConfLocalManagedObjectPo->getNativeVlanId();
                                            vector<ObjectId>  m_vlanidPo = pPoIntfManagedObject->getVlanVector();
                                            if (m_vlanidPo.size() > 0) { // . ELD is configured 
                                               sprintf(bufPo, "%d", native_vlanidPo);
                                               vlanIdPo = std::string(bufPo);
                                               trace(TRACE_LEVEL_INFO, string("Check for ELD-status of native vlan ") + bufPo);
                                               getVlanIntfObjectIdByVlanIfName(vlanIdPo, pVlanIntfObjIdPo, false, pDceSynchronousLinearSeqContext);
                                               search_nativePo = find(m_vlanidPo.begin(), m_vlanidPo.end(), pVlanIntfObjIdPo);
                                               if (search_nativePo != m_vlanidPo.end())
                                               { 
                                                   /* create the message to replay and send it to eldd daemon.*/
                                                   EldPoIntfMessage *P1 = new EldPoIntfMessage();
                                                   vector<UI32>    vlanIdVectorPo;
                                                   P1->setCmdCode(ELD_INTF_PO_CONF_ELD_ENABLE);
                                                   P1->setPoId(getString(pPoIntfManagedObject->getId()));
                                                   P1->setPoName(pPoIntfManagedObject->getIfName());
                                                   P1->setPoIfIndex(pPoIntfManagedObject->getIfIndex());
                                                   vlanIdVectorPo.push_back(native_vlanidPo);
                                                   P1->setVlanidList(vlanIdVectorPo);
                                                   status = sendSynchronouslyToWaveClient ("eld",P1);
                                                   pNsmUpdateLacpConfigSPMessage->setCompletionStatus(P1->getCompletionStatus());
												   NSM_PLUG_DBG_S( string ("UP_PO_L Response from Backend : ") + pPoIntfManagedObject->getIfName() + string("Cmdcode :")
                                                   + pNsmUpdateLacpConfigSPMessage->getCmdCode() + string("status :")
                                                   + status + string("backend clientStatus :") + P1->getClientStatus());
                                                   if( status == WAVE_MESSAGE_SUCCESS) {
                                                       if( pNsmUpdateLacpConfigSPMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                                                           status = pNsmUpdateLacpConfigSPMessage->getCompletionStatus();
                                                           delete P1;
                                                           return  status;
                                                       }
                                                       if ( (0 != P1->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != P1->getClientStatus() ) )  {
                                                            trace(TRACE_LEVEL_ERROR, string("Resp: UP_PHY_L  Client Error MsgId: ") +
                                                            P1->getMessageId() +
                                                            FrameworkToolKit::localize(P1->getClientStatus()));
                                                            status = NsmUtils::mapBackEndErrorsToDcmdErrors(P1->getClientStatus()) ;
                                                            delete P1;
                                                            return status;
                                                       }
                                                   }
                                                   delete P1;
                                                   P1 = NULL;
                                               }/*search-native != m_vlanid.end()*/
                                          }/*m_vlanid.size() > 0)*/
                                      }/*end else*/
                                  }/* end for */
                              } /* number of results >0 */
                              WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsManagedObjectsPo); 
                         }/* END (NULL != pResultsManagedObjectsPo) */
                     }//End Fix for defect 379169 for PO Interfaces */
				return status;
			 }
			}
			break;
		}

        if( status == WAVE_MESSAGE_SUCCESS){
            if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
				//bpatel : To avoid memory leak
                 status = m->getCompletionStatus();
                 delete nsmMsg;
                 delete m;
                 return status;
            }
		}
		//bpatel : To avoid memory leak
         if(nsmMsg){
             delete nsmMsg;
         }
         //bpatel : To avoid memory leak
        if (m){
            delete m;
        }
        return status;
    }

ResourceId IntfConfLocalWorker::updateEcmpConfigMOStep (PrismSynchronousLinearSequencerContext
	*pPrismSynchronousLinearSequencerContext)
{
	NsmLocalEcmpConfigMessage *pNsmLocalEcmpConfigMessage = dynamic_cast<NsmLocalEcmpConfigMessage *>
		(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

	int createFlag = 0;
	UI32 numberOfResults = 0;
	UI32UC hash_swap;
	ResourceId status   = WAVE_MESSAGE_ERROR;
    UI32 cmdCode = pNsmLocalEcmpConfigMessage->getCmdCode();

	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(EcmpConfigManagedObject::getClassName());
    syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

	EcmpConfigManagedObject *pEcmpConfigManagedObject = NULL;

	if (NULL != pResults) {
		numberOfResults = pResults->size ();
		if (0 == numberOfResults) {
			NSM_PLUG_DBG("Ecmp Local Config Managed Object doesn't exist...");
            // If MO is not present and Delete request comes, return.
            if (NSMUPDATE_ECMP_DEL_LOADBALANCE == cmdCode) {
                trace (TRACE_LEVEL_INFO, string("IntfConfLocalWorker::updateEcmpConfigMOStep :: Delete loadbalance request when ManagedObject is present"));
                pNsmLocalEcmpConfigMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
                delete pResults;
                return WAVE_MESSAGE_SUCCESS;
            }
			createFlag = 1;
		}else if (1 == numberOfResults) {
			createFlag = 0;
		} else {
			trace(TRACE_LEVEL_INFO,string("IntfConfLocalWorker::updateEcmpConfigMOStep: numberOfResults= ") + numberOfResults);
			status = WAVE_MESSAGE_ERROR;
		}
		if(createFlag == 1){
			pEcmpConfigManagedObject = new
				EcmpConfigManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
			pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pEcmpConfigManagedObject);
			
			pEcmpConfigManagedObject->setLoadBalanceMode(7);
			pEcmpConfigManagedObject->setRbridgeId(0);
			pEcmpConfigManagedObject->setHashSwap(0,false);
		
		}
	}
	if(createFlag == 0)
	{
		pEcmpConfigManagedObject = dynamic_cast<EcmpConfigManagedObject *>((*pResults)[0]);
	}
	switch(pNsmLocalEcmpConfigMessage->getCmdCode()){
	case NSMUPDATE_ECMP_LOADBALANCE:
		{
			pEcmpConfigManagedObject->setLoadBalanceMode (
				pNsmLocalEcmpConfigMessage->getLoadBalanceMode());
			trace(TRACE_LEVEL_INFO,string("loadbalancemode written in db is ")+
				pEcmpConfigManagedObject->getLoadBalanceMode());

		    pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			break;
		}
	case NSMUPDATE_ECMP_HASHSWAP:
		{
			pEcmpConfigManagedObject->setHashSwap(pNsmLocalEcmpConfigMessage->getHashSwap(),true);
			trace(TRACE_LEVEL_INFO,string("hash swap written in db is ")+
				pEcmpConfigManagedObject->getHashSwap().getUI32Value());

		    pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			break;
		}
	case NSMUPDATE_ECMP_DEL_LOADBALANCE:
		{
            if ((pEcmpConfigManagedObject != NULL) && (createFlag == 0)) {
                delete pEcmpConfigManagedObject;
                pEcmpConfigManagedObject = NULL;
            }
			trace(TRACE_LEVEL_INFO, string("loadbalance removed from db"));
			break;
		}
	default:
		break;
	}

    if (pEcmpConfigManagedObject != NULL) {
        pEcmpConfigManagedObject->setRbridgeId(DcmToolKit::getLocalMappedId());
    }
	//We shouldnt updating MO when created
	if((0 == createFlag) && (NSMUPDATE_ECMP_DEL_LOADBALANCE != pNsmLocalEcmpConfigMessage->getCmdCode())) 
	    updateWaveManagedObject(pEcmpConfigManagedObject);

	status = WAVE_MESSAGE_SUCCESS;
    pResults->clear();
	delete pResults;

	pNsmLocalEcmpConfigMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

	trace(TRACE_LEVEL_INFO,string("status is")+status);
	return status;
}


ResourceId IntfConfLocalWorker::sendEcmpConfigToNsmStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
	NsmLocalEcmpConfigMessage *pNsmLocalEcmpConfigMessage = dynamic_cast<NsmLocalEcmpConfigMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

	NsmEcmpConfigMessage *m = new NsmEcmpConfigMessage();
	int status = WAVE_MESSAGE_SUCCESS;
	trace(TRACE_LEVEL_INFO,string("Entered sendEcmpConfigToNsmStep"));
    if (NSMUPDATE_ECMP_DEL_LOADBALANCE == pNsmLocalEcmpConfigMessage->getCmdCode()) {
	    m->setCmdCode(NSMUPDATE_ECMP_LOADBALANCE);
    } else {
	    m->setCmdCode(pNsmLocalEcmpConfigMessage->getCmdCode());
    }
	trace(TRACE_LEVEL_INFO,string("Entered cmdCode ")+pNsmLocalEcmpConfigMessage->getCmdCode());

	switch(pNsmLocalEcmpConfigMessage->getCmdCode()){
	case NSMUPDATE_ECMP_LOADBALANCE:
	case NSMUPDATE_ECMP_DEL_LOADBALANCE:
		{
			m->setLoadBalanceMode(pNsmLocalEcmpConfigMessage->getLoadBalanceMode());
			trace(TRACE_LEVEL_INFO,string("loadbalance is ")+m->getLoadBalanceMode());
			break;
		}
	case NSMUPDATE_ECMP_HASHSWAP:
		{
			m->setHashSwap(pNsmLocalEcmpConfigMessage->getHashSwap());
			trace(TRACE_LEVEL_INFO,string("hashSwap is ")+m->getHashSwap());
			break;
		}
	default :
		break;
	}

	status = sendSynchronouslyToWaveClient("nsm",m);
	if(status != WAVE_MESSAGE_SUCCESS)
	{
		NSM_PLUG_INFO_S(string("ECMP:: Sys Priority: Send To Daemon, Message-Status=[") +
			status + string("]:") +
			FrameworkToolKit::localize((status)));
	}

	trace(TRACE_LEVEL_INFO,string("client msg status is ")+status);
	if(m)
		delete m;
	return status;
}

void  IntfConfLocalWorker::NsmLocalEcmpConfigMessageHandler( NsmLocalEcmpConfigMessage *pNsmLocalEcmpConfigMessage)
{
	trace(TRACE_LEVEL_INFO,string("Entered NsmLocalEcmpConfigMessageHandler"));
	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
		/* Sending to backend */
		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::sendEcmpConfigToNsmStep),
		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),

		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateEcmpConfigMOStep),

		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),

		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmLocalEcmpConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
}


    void IntfConfLocalWorker::NsmAssocMacAclPolicyPoVlanIntfSPMessageHandler (NsmAssocMacAclPolicyPoVlanIntfSPMessage *pMsg)
	{
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateMacAclConfigPoVlanIntf),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMsg, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
	}

	int IntfConfLocalWorker::getMacAclPolicyNameForInterface(const UI32 ifType, const string ifName, string &policyName, const ssmAcl_binding_direction direction)
	{
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		PoIntfManagedObject *pPoIntfManagedObject = NULL;
		VlanIntfManagedObject *pVlanIntfManagedObject = NULL;
		WaveManagedObject   *pWaveManagedObject = NULL;
		MacAclPolicyManagedObject *policyMO = NULL;
		string pName;
		string className;
		UI32 ifId = 0;
		int status = -1;


		if (ifType == PHY_INTF_TYPE) {
			className = PhyIntfLocalManagedObject::getClassName();
			pName = ifName;
		} else if (ifType == PO_INTF_TYPE) {
			className = PoIntfManagedObject::getClassName();
			ifId = strtoul(ifName.c_str (), NULL, 0);
		} else if (ifType == VLAN_INTF_TYPE) {
			className = VlanIntfManagedObject::getClassName();
			ifId = strtoul(ifName.c_str (), NULL, 0);
		} else  {
			return (status);
		}

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
		if (ifType == PHY_INTF_TYPE) {
			syncQueryCtxt.addAndAttribute (new AttributeString(&pName,"id"));
		} else if ((ifType == PO_INTF_TYPE || ifType == VLAN_INTF_TYPE)) {
			syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifId,"id"));
		}

		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (numberOfResults == 1) {
				pWaveManagedObject = NULL;
				pPhyIntfLocalManagedObject = NULL;
				pPoIntfManagedObject = NULL;
				pVlanIntfManagedObject = NULL;
				ObjectId policyObjId = ObjectId::NullObjectId;

				if (ifType == PHY_INTF_TYPE) {
					trace (TRACE_LEVEL_INFO, string("ifType == PHY_INTF_TYPE "));
					pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
					if (pPhyIntfLocalManagedObject) {
						(pPhyIntfLocalManagedObject->getIngressEgressMacAclPolicyObjectId (policyObjId, direction));
						trace (TRACE_LEVEL_INFO, string("Obtained policyObjId for Ingress Mac Acl for UNBIND"));
					}
					/* Fix Me Return with specific error */
				} else if (ifType == PO_INTF_TYPE) {
					pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
					if (pPoIntfManagedObject) {
						(pPoIntfManagedObject->getIngressEgressMacAclPolicyObjectId (policyObjId, direction));
						trace (TRACE_LEVEL_INFO, string("Obtained policyObjId for Ingress Mac Acl for UNBIND"));
					}
					/* Fix Me Return with specific error */
				} else if (ifType == VLAN_INTF_TYPE) {
					pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResults)[0]);
					if (pVlanIntfManagedObject) {
						(pVlanIntfManagedObject->getIngressEgressMacAclPolicyObjectId (policyObjId, direction));
						trace (TRACE_LEVEL_INFO, string("Obtained policyObjId for Ingress Mac Acl for UNBIND"));
					}
				}

				if (policyObjId != ObjectId::NullObjectId) {
					pWaveManagedObject = queryManagedObject (policyObjId);
					if (pWaveManagedObject != NULL) {
						policyMO = dynamic_cast<MacAclPolicyManagedObject *>(pWaveManagedObject);
						policyName = policyMO->getPolicyName();
						status = 0;
						delete pWaveManagedObject;
					}
				}
			}
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
		}

		return (status);
	}

	ResourceId IntfConfLocalWorker::updateMacAclConfigPoVlanIntf (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		char buffer[MAX_STR];
		buffer[MAX_STR - 1] = '\0';
		//UI32 bufferSize = sizeof (buffer);
		string ifname = "";
		NsmAssocMacAclPolicyPoVlanIntfSPMessage *pMsg 	= dynamic_cast<NsmAssocMacAclPolicyPoVlanIntfSPMessage *>
			(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		const ssmAcl_binding_direction direction 		= pMsg->getDirection();

		if(direction == SSMACL_BINDING_DIRECTION_OUT_E) {
			if(!Capabilities::isCapabilityPresent(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS))
			{
				trace (TRACE_LEVEL_DEBUG, "Applying MAC ACL policy in egress direction not supported on this platform.");
				return SSM_DCM_ERR_ACL_APPLIED_INTF_EGRESS;
			}
		}

        if (pMsg->getIfType() == VLAN_INTF_TYPE) {
            UI32 vlanId = pMsg->getIfId();
            if (!ProvisioningUtil::isVlanProvisioned(vlanId)) {
                trace(TRACE_LEVEL_INFO,string("updateMacAclConfigPoVlanIntf VLAN is not provisioned...  "));
                return status;
            }
        }

		SsmMacAclPolicyMessage *cMsg 					= new SsmMacAclPolicyMessage();
		trace(TRACE_LEVEL_DEVEL, string("updateMacAclConfigPoVlanIntf: polName: ") + pMsg->getAclName());

		if (cMsg) {

			string policyName;
			if (pMsg->getCmdCode() == SSMACL_DCMD_UNBIND_L2_ACL) {
				char buf[6];
				int ret;
				if (pMsg->getIfType() == PO_INTF_TYPE) {
					sprintf(buf, "%d", pMsg->getIfId());
				} else if (pMsg->getIfType() == VLAN_INTF_TYPE) {
					sprintf(buf, "%d", pMsg->getIfId());
				}

				ifname = buf;

				ret = getMacAclPolicyNameForInterface(pMsg->getIfType(), ifname, policyName, direction);
				if (ret == 0) {
					trace(TRACE_LEVEL_DEVEL, string("updateMacAclConfigPoVlanIntf: polName: ")+ policyName);
					cMsg->setPolicyName(policyName);
				} else {
					trace(TRACE_LEVEL_DEVEL, string("updateMacAclConfigPoVlanIntf: GetPolicyname failed" ));
					return (WAVE_MESSAGE_ERROR);
				}
			} else {
				cMsg->setPolicyName(pMsg->getAclName());
			}

			if (pMsg->getIfType() == PO_INTF_TYPE) {
				sprintf(buffer, "po%d", pMsg->getIfId());
			} else if (pMsg->getIfType() == VLAN_INTF_TYPE) {
				sprintf(buffer, "vlan0.%d", pMsg->getIfId());
			}

			ifname = buffer;

			cMsg->setOpCode(pMsg->getCmdCode ());
			cMsg->setDirection(direction);
			cMsg->setIfname(ifname);
			/* Set Acl Type to differentiate between ip and mac acl */
			cMsg->setPolicyType(SSMACL_TYPE_L2_E);

			trace(TRACE_LEVEL_DEVEL, string("If Name is ") + cMsg->getIfname());
			status = sendSynchronouslyToWaveClient ("ssm", cMsg);
			if (status == WAVE_MESSAGE_SUCCESS) {
				status = cMsg->getClientStatus();
				trace(TRACE_LEVEL_INFO, string("updateMacAclConfigPoVlanIntf: Success: ") + status);
				if ( status == WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS_IP )
				{
					status = WRC_SSM_DCM_ERR_APPLY_IP_ACL_ON_INTF;
				}
				pMsg->setCompletionStatus(status);
			} else {
				trace(TRACE_LEVEL_INFO, string("updateMacAclConfigPoVlanIntf: Failed: ") +  cMsg->getClientStatus());
			}
			delete cMsg;
			cMsg = NULL;

		} else {
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "updateMacAclConfigPoVlanIntf: Message creation failed");
		}

		return (status);
	}


	void IntfConfLocalWorker::NsmAssocMacAclPolicyPhyIntfMessageHandler (NsmAssocMacAclPolicyPhyIntfMessage *pNsmAssocMacAclPolicyPhyIntfMessage)
	{
		PrismSynchronousLinearSequencerStep sequencerSteps[] =
		{
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::validateMacAclPhyIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateMacAclPhyIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateMacAclPhyIntfMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

		DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pNsmAssocMacAclPolicyPhyIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
		pDceSynchronousLinearSeqContext->execute ();
	}

	ResourceId IntfConfLocalWorker::validateMacAclPhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
	{
		NsmAssocMacAclPolicyPhyIntfMessage *pMsg = dynamic_cast<NsmAssocMacAclPolicyPhyIntfMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		ssmAcl_binding_direction direction 						= pMsg->getDirection();
		string ifName											= pMsg->getIfName();
		UI32 found												= 0;
		vector<string> selectFields;

		// Validate 1: validating the command support on the platform.
		if(direction == SSMACL_BINDING_DIRECTION_OUT_E) {
			if(!Capabilities::isCapabilityPresent(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS))
			{
				trace (TRACE_LEVEL_DEBUG, "Applying MAC ACL policy in egress direction supported on this platform.");
				return SSM_DCM_ERR_ACL_APPLIED_INTF_EGRESS;
			}
		}

		/* Validate 2: Check if port mode is ISL.
		 * If yes block the ACL configuration
		 */

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        selectFields.push_back("swModeFlag");
		selectFields.push_back("Portrole");
		selectFields.push_back("ingressEgressMacAclPolicy");
		// adds the fields into the query context to make use of the partial query
        syncQueryCtxt.addSelectFields(selectFields);

		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (pResults != NULL && pResults->size () == 1) {
        	pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
			if (pPhyIntfLocalManagedObject) {
			  // temporarily storing the MO into the sequencer context to re-use it in the update step.
				pDceSynchronousLinearSeqContext->setPWaveManagedObject(pPhyIntfLocalManagedObject);
        		if ((DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pPhyIntfLocalManagedObject->getPortrole())) {
        			status = WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
				} else if (!pPhyIntfLocalManagedObject->getSwModeFlag()) {
						status = WRC_SSM_DCM_ERR_ACL_MAC_PORT;
				} else if (pMsg->getCmdCode() == SSMACL_DCMD_BIND_L2_ACL) {
					found = pPhyIntfLocalManagedObject->getIngressEgressMacAclPolicyDirectioncount (direction);

					if (found)
						status = WRC_SSM_DCM_ERR_ACL_RULE_DUP;
				}
			}
		} else {
			status = WRC_NSM_INTERFACE_DOES_NOT_EXIST;
		}

        /* GC collection */
	   if (pResults) {
           pDceSynchronousLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
	       delete pResults;
	   }

		return (status);
	}


	ResourceId IntfConfLocalWorker::updateMacAclPhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status 	= WAVE_MESSAGE_SUCCESS;
		char buffer[MAX_STR];
		buffer[MAX_STR -1] 	= '\0';
		UI32 bufferSize     = sizeof (buffer);
		string ifname 		= "";

		NsmAssocMacAclPolicyPhyIntfMessage *pMsg = dynamic_cast<NsmAssocMacAclPolicyPhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ssmAcl_binding_direction direction = pMsg->getDirection();

		trace(TRACE_LEVEL_DEVEL, string("updateAclPhyIntf: polName: ") + pMsg->getAclName());

		SsmMacAclPolicyMessage *cMsg = new SsmMacAclPolicyMessage();
		if (cMsg) {
			string policyName = "";
			if (pMsg->getCmdCode() == SSMACL_DCMD_UNBIND_L2_ACL) {
				int ret = getMacAclPolicyNameForInterface(PHY_INTF_TYPE, pMsg->getIfName(), policyName, direction);

				if (ret == 0) {
					trace(TRACE_LEVEL_DEVEL, string("updateMacAclConfigPhyIntf: polName: ")+ policyName);
					cMsg->setPolicyName(policyName);
				} else {
					trace(TRACE_LEVEL_DEVEL, string("updateMacAclConfigPhyIntf: GetPolicyname failed" ));
					return (WAVE_MESSAGE_ERROR);
				}

			} else {
				cMsg->setPolicyName(pMsg->getAclName());
			}

			if (true  == NsmUtils::isValidThreeTuple(pMsg->getIfName ())) {
				string twoTuppleIfName;
				string threeTupple = pMsg->getIfName ();
				NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
				GENERATE_SHORT_IF_NAME_FOR_BACK_END(pMsg->getIfType(), twoTuppleIfName.c_str(), buffer, bufferSize);
			} else {
				GENERATE_SHORT_IF_NAME_FOR_BACK_END(pMsg->getIfType(), pMsg->getIfName().c_str(), buffer, bufferSize);
			}

			ifname = buffer;
			cMsg->setIfname(ifname);
			cMsg->setDirection(direction);
			cMsg->setOpCode(pMsg->getCmdCode ());
			/* Set Acl Type to differentiate between ip and mac acl */
			cMsg->setPolicyType(SSMACL_TYPE_L2_E);


			status = sendSynchronouslyToWaveClient ("ssm", cMsg);
			if (status == WAVE_MESSAGE_SUCCESS) {
				status = cMsg->getClientStatus();
				trace(TRACE_LEVEL_INFO, string("updateMacAclPhyIntfStep: Success: ") + status);
				if ( status == WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS_IP )
				{
					status = WRC_SSM_DCM_ERR_APPLY_IP_ACL_ON_INTF;
				}
				pMsg->setCompletionStatus(status);
			} else {
				trace(TRACE_LEVEL_INFO, string("updateMacAclPhyIntfStep: Failed: ") + cMsg->getClientStatus());
			}

			delete cMsg;
		} else {
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "updateMacAclPhyIntfStep: Message creation failed");
		}

		return (status);
	}

	ResourceId IntfConfLocalWorker::updateMacAclPhyIntfMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
	{
        ResourceId  status   = WAVE_MESSAGE_ERROR;
		NsmAssocMacAclPolicyPhyIntfMessage *pMsg = dynamic_cast<NsmAssocMacAclPolicyPhyIntfMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

		if (pMsg->getCmdCode () == SSMACL_DCMD_BIND_L2_ACL) {
			status = assignMacAclPhyIntfMOStep(pDceSynchronousLinearSeqContext);
		} else if (pMsg->getCmdCode () == SSMACL_DCMD_UNBIND_L2_ACL) {
			status = removeMacAclPhyIntfMOStep(pDceSynchronousLinearSeqContext);
		}

		return (status);
	}

	ResourceId IntfConfLocalWorker::assignMacAclPhyIntfMOStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
	{
        ResourceId  status   = WAVE_MESSAGE_ERROR;
		NsmAssocMacAclPolicyPhyIntfMessage *pMsg = dynamic_cast<NsmAssocMacAclPolicyPhyIntfMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());
		string aclName = pMsg->getAclName();
		string ifName = pMsg->getIfName();
		ssmAcl_binding_direction direction  					= pMsg->getDirection();

        trace (TRACE_LEVEL_DEVEL, "****Inside assignMacAclPhyIntfMOStep");
		SsmGlobalGetMacAclPolicyMessage *policyMsg = new SsmGlobalGetMacAclPolicyMessage(aclName);

		if (policyMsg) {
           	trace (TRACE_LEVEL_DEVEL, "assignMacAclPhyIntfMOStep3 - Obtained the Policy MO");
			status = sendSynchronously (policyMsg);
			if (status == WAVE_MESSAGE_SUCCESS && policyMsg->getCompletionStatus() == WAVE_MESSAGE_SUCCESS) {
				status = WAVE_MESSAGE_SUCCESS;
				trace (TRACE_LEVEL_INFO, "assignMacAclPhyIntfMOStep SSM Message returned success");
			} else {
				status = WAVE_MESSAGE_ERROR;
				trace (TRACE_LEVEL_ERROR, "assignMacAclPhyIntfMOStep SSM Message returned with  error");
				delete policyMsg;
				return (status);
			}
			
			// retrieving the MO from the sequencer context, to re-use it.
			PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>(pDceSynchronousLinearSeqContext->getPWaveManagedObject());

			if (pPhyIntfLocalManagedObject) {
			    //adding the field for the partial commit
				pPhyIntfLocalManagedObject->addAttributeToBeUpdated("ingressEgressMacAclPolicy");
				NsmLocalIngressEgressMacAclPolicyManagedObject *NsmIngressEgressMacAclMO = new
					NsmLocalIngressEgressMacAclPolicyManagedObject(dynamic_cast
					<NsmLocalObjectManager*>(getPWaveObjectManager()), policyMsg->getPolicyObjId(), direction);
				pPhyIntfLocalManagedObject->addIngressEgressMacAclManagedObjectToVector
					(NsmIngressEgressMacAclMO);
				updateWaveManagedObject(pPhyIntfLocalManagedObject);
				status = WAVE_MESSAGE_SUCCESS;
			} else {
		  	    status = WAVE_MESSAGE_ERROR;
		    }
  	
			delete policyMsg;
		}
		return (status);

	}

	ResourceId IntfConfLocalWorker::removeMacAclPhyIntfMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
        ResourceId  status   = WAVE_MESSAGE_ERROR;
		NsmAssocMacAclPolicyPhyIntfMessage *pMsg = dynamic_cast<NsmAssocMacAclPolicyPhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		string ifName 											= pMsg->getIfName();
		ssmAcl_binding_direction direction 						= pMsg->getDirection();


        trace (TRACE_LEVEL_DEVEL, "****Inside removeMacAclPhyIntfMOStep");

       	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
       	syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
       	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
       	if (NULL != pResults) {
           	UI32 numberOfResults = pResults->size ();
           	if (0 == numberOfResults) {
               	status = WAVE_MESSAGE_ERROR;
           	}else if (1 == numberOfResults) {
      			trace (TRACE_LEVEL_DEVEL, "removeMacAclPhyIntfMOStep4- Obtained the Port Channel object ");
               	pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
               	if (pPhyIntfLocalManagedObject) {
					pPhyIntfLocalManagedObject->delIngressEgressMacAclManagedObjectFromVector(direction);

					updateWaveManagedObject(pPhyIntfLocalManagedObject);
					status = WAVE_MESSAGE_SUCCESS;
				}
           	} else {
               	status = WAVE_MESSAGE_ERROR;
       		}
            //PASU: GC collection
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
           	delete pResults;
       	} else {
			status = WAVE_MESSAGE_ERROR;
		}

		return (status);

	}

    /* Ip Acl Ve Intf code Begins */
    void IntfConfLocalWorker::NsmLocalPBRVeIntfMessageHandler(PBRVeIntfMessage *PBRVeIntfMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::validatePBRVeIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::configurePBRVeIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateVeIntfPBRStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (PBRVeIntfMessage,
                        this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IntfConfLocalWorker::validatePBRVeIntfStep(PrismSynchronousLinearSequencerContext *pLinearSeqContext)
    {
        PBRVeIntfMessage *pMsg =
                reinterpret_cast<PBRVeIntfMessage *>(pLinearSeqContext->getPPrismMessage());
        ResourceId status   = WAVE_MESSAGE_SUCCESS;
        UI32 found          = 0;
        UI32 ifId           = pMsg->getIfId();
        NsmIntfSviManagedObject *pNsmIntfSviManagedObject;
        /* Fix Me : Check value of direction in commit transaction */        

        tracePrintf(TRACE_LEVEL_INFO, "In %s, type = %d, cmdCode = %d ",
                __FUNCTION__, pMsg->getIfType (), pMsg->getCmdCode());


        if (pMsg->getCmdCode() == SSM_CLIENT_SET_ROUTEMAP_APPLY_INTERFACE) {

            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NsmIntfSviManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifId,"vlanId"));

            vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (FrameworkToolKit::getThisLocationId(), &syncQueryCtxt);

            if (pResults && pResults->size() > 0) {
                pNsmIntfSviManagedObject    = NULL;
                pNsmIntfSviManagedObject = dynamic_cast<NsmIntfSviManagedObject *>((*pResults)[0]);
                if (pNsmIntfSviManagedObject) {
                    trace (TRACE_LEVEL_INFO,string("In function IntfConfLocalWorker::validatePBRVeIntfStep obtained pNsmIntfSviManagedObject"));
		    NsmLocalPBRManagedObject *pNsmLocalPBRManagedObject = NULL;
		    pNsmLocalPBRManagedObject = pNsmIntfSviManagedObject->getPBRMO();
                    trace (TRACE_LEVEL_INFO,string("AFTER ==> validateIpAclToVeIntfStep obtained pNsmIntfSviManagedObject -- found => ") + found);
                    if (pNsmLocalPBRManagedObject != NULL) {
                        status = WRC_SSM_DCM_ERR_ACL_RULE_DUP;
                    }
                }
            }
            if (pResults)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        trace (TRACE_LEVEL_INFO,string("validatePBRVeIntfStep before returning "));
        return status;
    }

    ResourceId IntfConfLocalWorker::configurePBRVeIntfStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId status                               = WAVE_MESSAGE_SUCCESS;
        char buffer[25];
        string ifname = "";
        PBRVeIntfMessage *pMsg     = dynamic_cast<PBRVeIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());


        trace(TRACE_LEVEL_DEVEL, string("configurePBRVeIntfStep: Name: "));


            string routeMapName;
		    vector<string> args;
		    int op_code = 0;

            if (pMsg->getIfType() == IF_TYPE_VE) {
                sprintf(buffer, "ve0.%d", pMsg->getIfId());
            }
            ifname = buffer;

            op_code = pMsg->getCmdCode ();            
            
            if (pMsg->getCmdCode() == SSM_CLIENT_SET_ROUTEMAP_REMOVE_INTERFACE) {
                char buf[6];
                int ret;

                sprintf(buf, "%d", pMsg->getIfId());

                ifname = buf;

                ret = getRouteMapNameForInterface(pMsg->getIfType(), ifname, routeMapName);

                if (ret == 0) {
                    trace(TRACE_LEVEL_DEVEL, string("configurePBRVeIntfStep: polName: ")+ routeMapName);
                } else {
                    trace(TRACE_LEVEL_DEVEL, string("configurePBRVeIntfStep: routeMapName failed" ));
                    return (WAVE_MESSAGE_ERROR);
                }
				ifname = buffer;
				args.push_back(routeMapName);
				args.push_back(ifname);
				trace(TRACE_LEVEL_INFO, string("configurePBRVeIntfStep - OPCODE: ") + op_code);
				for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
				  	trace(TRACE_LEVEL_INFO, string("configurePBRVeIntfStep - ARG. VALUE: ") + args[tmpIdx]);
				}
				GenericClientMessage_ client_msg(args, op_code);
				status = sendSynchronouslyToWaveClient ("ssm", &client_msg);
   	    	    if(status == WAVE_MESSAGE_SUCCESS) {
                	status = client_msg.getClientStatus();
					tracePrintf(TRACE_LEVEL_INFO, "configurePBRVeIntfStep: Success %lld",  status);
				} else {
					tracePrintf(TRACE_LEVEL_INFO,  "configurePBRVeIntfStep: Failed: %lld",  client_msg.getClientStatus());
				}
		    	return status;            

            } else {
				args.push_back(pMsg->getRouteMapName());
				args.push_back(ifname);
				trace(TRACE_LEVEL_INFO, string("configurePBRVeIntfStep - OPCODE: ") + op_code);

				for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
				  	trace(TRACE_LEVEL_INFO, string("configurePBRVeIntfStep - ARG. VALUE: ") + args[tmpIdx]);
				}

				GenericClientMessage_ client_msg(args, op_code);
				status = sendSynchronouslyToWaveClient ("ssm", &client_msg);
                if(status == WAVE_MESSAGE_SUCCESS) {
                    status = client_msg.getClientStatus();
                    tracePrintf(TRACE_LEVEL_INFO, "configurePBRVeIntfStep: Success %lld",  status);
                } else {
                    tracePrintf(TRACE_LEVEL_INFO,  "configurePBRVeIntfStep: Failed: %lld",  client_msg.getClientStatus());
                }
				return status;
            }

        return (status);
    }

    ResourceId IntfConfLocalWorker::updateVeIntfPBRStep(PrismSynchronousLinearSequencerContext *pLinearSeqContext)
    {
        trace (TRACE_LEVEL_DEVEL, "Inside updateVeIntfPBRStep");
        PBRVeIntfMessage *pMsg = reinterpret_cast<PBRVeIntfMessage *>(pLinearSeqContext->getPPrismMessage());
        ResourceId status = WAVE_MESSAGE_ERROR;

        NsmIntfSviManagedObject *pNsmIntfSviManagedObject   = NULL;
        UI32 vlanId                                         = pMsg->getIfId ();        

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NsmIntfSviManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlanId,"vlanId"));
        vector<WaveManagedObject *> *pResults =
                querySynchronouslyLocalManagedObjectsForLocationId (FrameworkToolKit::getThisLocationId(), &syncQueryCtxt);

        if (pMsg->getCmdCode () == SSM_CLIENT_SET_ROUTEMAP_APPLY_INTERFACE)
        {
            string routeMapName                                      = pMsg->getRouteMapName ();
            //ObjectId PBRObjectId;
            //IpPolicyGetPBRObjectMessage *policyMsg = new IpPolicyGetPBRObjectMessage(routeMapName);

            //if (policyMsg) {
            //    trace (TRACE_LEVEL_DEVEL, "updateVeIntfPBRStep:Step:3 - Obtained the Policy MO");
            //    status = sendSynchronously (policyMsg);
            //    if (status == WAVE_MESSAGE_SUCCESS && policyMsg->getCompletionStatus() == WAVE_MESSAGE_SUCCESS) {
            //        trace (TRACE_LEVEL_INFO, "updateVeIntfIpAclStep: SSM Message returned success");
            //        status = WAVE_MESSAGE_SUCCESS;
            //    } else {
            //        status = WAVE_MESSAGE_ERROR;
            //        trace (TRACE_LEVEL_ERROR, "updateVeIntfIpAclStep: SSM Message returned with  error");
            //        delete policyMsg;
            //        return (status);
            //    }
            //    PBRObjectId = policyMsg->getPBRRouteMapObjId();

                if (NULL != pResults) {
                    UI32 numberOfResults = pResults->size ();
                    if (0 == numberOfResults) {
                        status = WAVE_MESSAGE_ERROR;
                    } else if (1 == numberOfResults) {
                        trace (TRACE_LEVEL_DEVEL, "updateVeIntfPBRStep:4- Obtained the IntfSVI object ");

                        pNsmIntfSviManagedObject = dynamic_cast<NsmIntfSviManagedObject *>((*pResults)[0]);
                        NsmLocalPBRManagedObject *NsmPBRMO = new
                                NsmLocalPBRManagedObject(dynamic_cast
                                        <NsmLocalObjectManager*>(getPWaveObjectManager()), routeMapName);
                        pNsmIntfSviManagedObject->setPBR(NsmPBRMO);
                        updateWaveManagedObject(pNsmIntfSviManagedObject);
                        status = WAVE_MESSAGE_SUCCESS;
                    } else {
                        status = WAVE_MESSAGE_ERROR;
                    }
                    /* Mark objects for GC */
                    pLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
                    delete pResults;
                }
            //    delete policyMsg;
            //}
        } else if (pMsg->getCmdCode () == SSM_CLIENT_SET_ROUTEMAP_REMOVE_INTERFACE) {
            if (NULL != pResults) {
                UI32 numberOfResults = pResults->size ();
                if (0 == numberOfResults) {
                    status = WAVE_MESSAGE_ERROR;
                }else if (1 == numberOfResults) {
                    trace (TRACE_LEVEL_DEVEL, "removeIpAclVeIntf Obtained the Svi Intf object ");
                    pNsmIntfSviManagedObject = dynamic_cast<NsmIntfSviManagedObject *>((*pResults)[0]);
                    if (pNsmIntfSviManagedObject) {
                        pNsmIntfSviManagedObject->setPBR(NULL);
                        updateWaveManagedObject(pNsmIntfSviManagedObject);
                        status = WAVE_MESSAGE_SUCCESS;
                        trace (TRACE_LEVEL_DEVEL, "removeIpAclVeIntf: Removed Ip acl from Ve interface ");
                    }
                }
                /* Mark objects for GC */
                pLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
                delete pResults;
            }
        }
        return status;
    }

    int IntfConfLocalWorker::getRouteMapNameForInterface(const UI32 ifType, const string ifName, string &routeMapName)
    {
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject   = NULL;
        PoIntfManagedObject *pPoIntfManagedObject               = NULL;
        NsmIntfSviManagedObject *pNsmIntfSviManagedObject       = NULL;
        //WaveManagedObject   *pWaveManagedObject                 = NULL;
        //RouteMapLocalManagedObject *routeMapMO                  = NULL;
        string pName;
        string className                                        = "";
        UI32 ifId                                               =  0;
        int status                                              = -1;

        trace (TRACE_LEVEL_INFO, string("Inside getRouteMapNameForInterface Method"));
		trace (TRACE_LEVEL_INFO, string("getRouteMapNameForInterface: ifType : ") + ifType);

        if ((ifType == IF_TYPE_GI) || (ifType == IF_TYPE_TE) || (ifType == IF_TYPE_FO)) {
		//if(ifType == PHY_INTF_TYPE) {
            className = PhyIntfLocalManagedObject::getClassName();
            pName = ifName;
        } else if (ifType == IF_TYPE_PO) {
            className = PoIntfManagedObject::getClassName();
            ifId = strtoul(ifName.c_str (), NULL, 0);
        } else if (ifType == IF_TYPE_VE) {
            className = NsmIntfSviManagedObject::getClassName();
            ifId = strtoul(ifName.c_str (), NULL, 0);
        } else  {
            return (status);
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
        if ((ifType == IF_TYPE_GI) || (ifType == IF_TYPE_TE) || (ifType == IF_TYPE_FO)) {
		//if(ifType == PHY_INTF_TYPE) {
           trace (TRACE_LEVEL_INFO, string("Query to Phy Interface"));
            syncQueryCtxt.addAndAttribute (new AttributeString(&pName,"id"));
        } else if (ifType == IF_TYPE_VE) {
            syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifId,"vlanId"));
            syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
        } else if (ifType == IF_TYPE_PO) {
            syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifId,"id"));
        }

        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (numberOfResults == 1) {
                //pWaveManagedObject          = NULL;
			//ResourceId handleError();
                pPhyIntfLocalManagedObject  = NULL;
                pPoIntfManagedObject        = NULL;
                pNsmIntfSviManagedObject    = NULL;
                //ObjectId routeMapObjId        = ObjectId::NullObjectId;
                trace (TRACE_LEVEL_INFO, string(" Number of Results are 1 "));
                if ((ifType == IF_TYPE_GI) || (ifType == IF_TYPE_TE) || (ifType == IF_TYPE_FO)) {
				//if(ifType == PHY_INTF_TYPE) {
                    trace (TRACE_LEVEL_INFO, string("ifType == PHY_INTF_TYPE "));
                    pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                    if (pPhyIntfLocalManagedObject) {
                        (pPhyIntfLocalManagedObject->getPBRNameFromInterface (routeMapName));
                        trace (TRACE_LEVEL_INFO, string("Obtained routeMapName for PBR for UNBIND"));
						trace (TRACE_LEVEL_INFO, string("routeMapName : ") + routeMapName);
						status = 0;
                    }
                    /* Fix Me Return with specific error */
                } else if (ifType == IF_TYPE_PO) {
                    pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
                    if (pPoIntfManagedObject) {
                        (pPoIntfManagedObject->getPBRNameFromInterface (routeMapName));
                        trace (TRACE_LEVEL_INFO, string("Obtained routeMapName for PBR for UNBIND"));
						status = 0;
                    }
                    /* Fix Me Return with specific error */
                } else if (ifType == IF_TYPE_VE) {
                    pNsmIntfSviManagedObject = dynamic_cast<NsmIntfSviManagedObject *>((*pResults)[0]);
                    if (pNsmIntfSviManagedObject) {
                        (pNsmIntfSviManagedObject->getPBRNameFromInterface (routeMapName));
                        trace (TRACE_LEVEL_INFO, string("Obtained routeMapName for PBR for UNBIND"));
						status = 0;
                    }
                }
				
				/*
                if (routeMapObjId != ObjectId::NullObjectId) {
                    pWaveManagedObject = queryManagedObject (routeMapObjId);
                    if (pWaveManagedObject != NULL) {
                        trace (TRACE_LEVEL_INFO, string ("ObjectId is not NULL"));
                        routeMapMO    = dynamic_cast<RouteMapLocalManagedObject *>(pWaveManagedObject);
                        routeMapName  = routeMapMO->getName ();
                        status      = 0;
                        delete pWaveManagedObject;
                    }
                } */
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
        }

        return (status);
    }

	/* Ip Acl Ve Intf code Begins */
    void IntfConfLocalWorker::NsmLocalAssocIpAclPolicyVeIntfMessageHandler(NsmLocalAssocIpAclPolicyVeIntfMessage *NsmLocalAssocIpAclPolicyVeIntfMessage)
	{
    	PrismSynchronousLinearSequencerStep sequencerSteps[] =
    	{
    		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::validateIpAclToVeIntfStep),
    		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::configureIpAclToVeIntfStep),
    		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
    		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateVeIntfIpAclStep),
    		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
    		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
    		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
    	};
    	DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (NsmLocalAssocIpAclPolicyVeIntfMessage,
    					this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    	pPrismSynchronousLinearSequencerContext->execute ();
	}

    ResourceId IntfConfLocalWorker::validateIpAclToVeIntfStep(DceSynchronousLinearSeqContext *pLinearSeqContext)
    {
		NsmLocalAssocIpAclPolicyVeIntfMessage *pMsg =
				reinterpret_cast<NsmLocalAssocIpAclPolicyVeIntfMessage *>(pLinearSeqContext->getPPrismMessage());
		ResourceId status 	= WAVE_MESSAGE_SUCCESS;
		UI32 found 			= 0;
		UI32 ifId 			= pMsg->getIfId();
		NsmIntfSviManagedObject *pNsmIntfSviManagedObject;
		/* Fix Me : Check value of direction in commit transaction */
		ssmAcl_binding_direction direction = pMsg->getDirection();

		tracePrintf(TRACE_LEVEL_INFO, "In %s, type = %d, cmdCode = %d ",
				__FUNCTION__, pMsg->getIfType (), pMsg->getCmdCode());

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NsmIntfSviManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifId,"vlanId"));

        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId(FrameworkToolKit::getThisLocationId(), &syncQueryCtxt);

        if (NULL == pResults) {
            if (pMsg->getCmdCode() == SSMACL_DCMD_UNBIND_L3_IPV4_ACL) {
                trace(TRACE_LEVEL_INFO, "IntfConfLocalWorker::validateIpAclToVeIntfStep: Delete with nonexistent VE");

                // To prevent a request to delete a nonexistent binding from causing an error (which is needed for Brocade WyserEa
                // optimizations), we skip the rest of the sequence and cause it to return success.
                status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
        }
        else if (pResults->size() != 1) {
            tracePrintf(TRACE_LEVEL_INFO, "IntfConfLocalWorker::validateIpAclToVeIntfStep: pResults->size() == %d", pResults->size());
            if (pResults->size() == 0) {
                trace(TRACE_LEVEL_INFO, "IntfConfLocalWorker::validateIpAclToVeIntfStep: Create with nonexistent VE");
                status = SSM_DCM_ERR_ACL_INTERFACE_NOT_FOUND;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        else {
            pNsmIntfSviManagedObject 	= NULL;
            pNsmIntfSviManagedObject = dynamic_cast<NsmIntfSviManagedObject *>((*pResults)[0]);
            if (pNsmIntfSviManagedObject) {
                pLinearSeqContext->getCache()->put(NsmIntfSviManagedObject::getClassName(), pNsmIntfSviManagedObject);
                ObjectId oldPolicyObjId;
                found = pNsmIntfSviManagedObject->getIngressEgressIpAclPolicyObjectId(oldPolicyObjId, direction);
                trace (TRACE_LEVEL_INFO,string("validateIpAclToVeIntfStep Obtained pNsmIntfSviManagedObject : found = ")+found);

                if (pMsg->getCmdCode() == SSMACL_DCMD_BIND_L3_IPV4_ACL) {
                    string aclName = pMsg->getAclName ();
                    SsmGlobalGetIpAclPolicyMessage *policyMsg = new SsmGlobalGetIpAclPolicyMessage(aclName);

                    if (policyMsg) {
                        trace (TRACE_LEVEL_DEVEL, "validateIpAclToVeIntfStep: Obtaining the Policy MO");
                        status = sendSynchronously (policyMsg);
                        if (status == WAVE_MESSAGE_SUCCESS && policyMsg->getCompletionStatus() == WAVE_MESSAGE_SUCCESS) {
                            trace (TRACE_LEVEL_INFO, "validateIpAclToVeIntfStep: SSM Message returned success");

                            ObjectId newPolicyObjId = policyMsg->getPolicyObjId();
                            pLinearSeqContext->getCache()->put(IpAclPolicyManagedObject::getClassName(), newPolicyObjId);

                            if (found) {
                                if (newPolicyObjId == oldPolicyObjId) {
                                    trace(TRACE_LEVEL_INFO, "IntfConfLocalWorker::validateIpAclToVeIntfStep: Create for existing object");

                                    // To prevent a request to create an already-existing config from causing any issues (which
                                    // is needed for Brocade WyserEa read optimizations), we skip the rest of the sequence and
                                    // cause it to return success.
                                    status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
                                } else {
                                    status = WRC_SSM_DCM_ERR_ACL_RULE_DUP;
                                }
                            }
                        } else {
                            status = WAVE_MESSAGE_ERROR;
                            trace (TRACE_LEVEL_ERROR, "validateIpAclToVeIntfStep: SSM Message returned with error");
                        }

                        delete policyMsg;
                    } else {
                        status = WAVE_MESSAGE_ERROR_MEMORY_EXHAUSTED;
                    }
                } else if (pMsg->getCmdCode() == SSMACL_DCMD_UNBIND_L3_IPV4_ACL) {
                    if (!found) {
                        trace(TRACE_LEVEL_INFO, "IntfConfLocalWorker::validateIpAclToVeIntfStep: Delete for nonexistent object");

                        // To prevent a request to delete a nonexistent binding from causing an error (which is needed for Brocade WyserEa
                        // optimizations), we skip the rest of the sequence and cause it to return success.
                        status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
                    }
                }
            }

            pLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }

    	return status;
    }

    ResourceId IntfConfLocalWorker::configureIpAclToVeIntfStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
		ResourceId status 								= WAVE_MESSAGE_SUCCESS;
		char buffer[25];
		string ifname = "";
		NsmLocalAssocIpAclPolicyVeIntfMessage *pMsg 	= dynamic_cast<NsmLocalAssocIpAclPolicyVeIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		const ssmAcl_binding_direction direction 		= pMsg->getDirection();

		if(direction == SSMACL_BINDING_DIRECTION_OUT_E) {
			if(!Capabilities::isCapabilityPresent(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS))
			{
				trace (TRACE_LEVEL_DEBUG, "Applying IP ACL policy in egress direction not supported on this platform.");
				return SSM_DCM_ERR_ACL_APPLIED_INTF_EGRESS;
			}
		}

		SsmIpAclPolicyMessage *cMsg 					= new SsmIpAclPolicyMessage();

		trace(TRACE_LEVEL_DEVEL, string("updateIpAclConfigVeIntf: polName: ") + pMsg->getAclName());

		if (cMsg) {

			string policyName;
			if (pMsg->getCmdCode() == SSMACL_DCMD_UNBIND_L3_IPV4_ACL) {
				char buf[6];
				int ret;

				sprintf(buf, "%d", pMsg->getIfId());

				ifname = buf;

				ret = getIpAclPolicyNameForInterface(pMsg->getIfType(), ifname, policyName, direction);

				if (ret == 0) {
					trace(TRACE_LEVEL_DEVEL, string("updateIpAclConfigVeIntf: polName: ")+ policyName);
					cMsg->setPolicyName(policyName);
				} else {
					trace(TRACE_LEVEL_DEVEL, string("updateIpAclConfigVeIntf: GetPolicyname failed" ));
					return (WAVE_MESSAGE_ERROR);
				}

			} else {
				cMsg->setPolicyName(pMsg->getAclName());
			}

			if (pMsg->getIfType() == VE_INTF_TYPE) {
				sprintf(buffer, "ve0.%d", pMsg->getIfId());
			}
			ifname = buffer;

			cMsg->setOpCode(pMsg->getCmdCode ());
			cMsg->setDirection(direction);
			cMsg->setIfname(ifname);
            /* Set Acl Type to differentiate between ip and mac acl */
            cMsg->setPolicyType(SSMACL_TYPE_L3_IPV4_E);

			status = sendSynchronouslyToWaveClient ("ssm", cMsg);
			if (status == WAVE_MESSAGE_SUCCESS) {
				status = cMsg->getClientStatus();
				trace(TRACE_LEVEL_INFO, string("updateIpAclConfigVeIntf: Success: ") + status);
				if ( status == WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS_MAC )
				{
					status = WRC_SSM_DCM_ERR_APPLY_MAC_ACL_ON_INTF;
				}
				pMsg->setCompletionStatus(status);
			} else {
				trace(TRACE_LEVEL_INFO, string("updateIpAclConfigVeIntf: Failed: ") + cMsg->getClientStatus());
			}

			delete cMsg;
			cMsg = NULL;
		} else {
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "updateIpAclConfigVeIntf: Message creation failed");
		}
		return (status);
    }

    ResourceId IntfConfLocalWorker::updateVeIntfIpAclStep(DceSynchronousLinearSeqContext *pLinearSeqContext)
    {
    	trace (TRACE_LEVEL_DEVEL, "Inside updateVeIntfIpAclStep");
    	NsmLocalAssocIpAclPolicyVeIntfMessage *pMsg = reinterpret_cast<NsmLocalAssocIpAclPolicyVeIntfMessage *>(pLinearSeqContext->getPPrismMessage());
    	ResourceId status = WAVE_MESSAGE_ERROR;
    	ssmAcl_binding_direction direction					= pMsg->getDirection();

    	NsmIntfSviManagedObject *pNsmIntfSviManagedObject = dynamic_cast<NsmIntfSviManagedObject *> (pLinearSeqContext->getCache()->getWaveManagedObject(NsmIntfSviManagedObject::getClassName()));

        if (NULL == pNsmIntfSviManagedObject) {
            status = WAVE_MESSAGE_ERROR;
        }
    	else if (pMsg->getCmdCode () == SSMACL_DCMD_BIND_L3_IPV4_ACL)
    	{
    		ObjectId policyObjectId = pLinearSeqContext->getCache()->getObjectId(IpAclPolicyManagedObject::getClassName());

    		if (ObjectId::NullObjectId != policyObjectId) {
    			trace (TRACE_LEVEL_DEVEL, "updateVeIntfIpAclStep:3 - Obtained the Policy MO");

                NsmLocalIngressEgressIpAclPolicyManagedObject *NsmIngressEgressIpAclMO = new
                        NsmLocalIngressEgressIpAclPolicyManagedObject(dynamic_cast
                                <NsmLocalObjectManager*>(getPWaveObjectManager()), policyObjectId, direction);
                pNsmIntfSviManagedObject->addIngressEgressIpAclManagedObjectToVector
                (NsmIngressEgressIpAclMO);
                updateWaveManagedObject(pNsmIntfSviManagedObject);
                status = WAVE_MESSAGE_SUCCESS;
                trace (TRACE_LEVEL_DEVEL, "updateVeIntfIpAclStep:4- Obtained the IntfSVI object ");
    		}
    	} else if (pMsg->getCmdCode () == SSMACL_DCMD_UNBIND_L3_IPV4_ACL) {
            trace (TRACE_LEVEL_DEVEL, "removeIpAclVeIntf Obtained the Svi Intf object ");

            pNsmIntfSviManagedObject->delIngressEgressIpAclManagedObjectFromVector(direction);
            updateWaveManagedObject(pNsmIntfSviManagedObject);
            status = WAVE_MESSAGE_SUCCESS;
            trace (TRACE_LEVEL_DEVEL, "removeIpAclVeIntf: Removed Ip acl from Ve interface ");
    	}
    	return status;
    }

    void IntfConfLocalWorker::NsmPBRPhyIntfMessageHandler (PBRPhyIntfMessage *pPBRPhyIntfMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::validatePBRPhyIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updatePBRPhyIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updatePBRPhyIntfMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPBRPhyIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId IntfConfLocalWorker::validatePBRPhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        PBRPhyIntfMessage *pMsg                 				= dynamic_cast<PBRPhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        ResourceId status                                       = WAVE_MESSAGE_SUCCESS;
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject   = NULL;        
        string ifName                                           = pMsg->getIfName();              

        /* Validate 2: Check if port mode is ISL.
         * If yes block the ACL configuration
         */

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults != NULL && pResults->size () == 1) {
            pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);

            if (pPhyIntfLocalManagedObject) {
                if ((DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pPhyIntfLocalManagedObject->getPortrole())) {
                    status = WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
                }
                else if (pMsg->getCmdCode() == SSM_CLIENT_SET_ROUTEMAP_APPLY_INTERFACE) {
                	if(pPhyIntfLocalManagedObject->getSwModeFlag())
                	{
                		status = WRC_NSM_ERR_INTF_MODE_L2;
                	}
                    bool isPortProfiled = false;
                    APPMUtils::isPortProfilePort(pPhyIntfLocalManagedObject, isPortProfiled);
                    if (pPhyIntfLocalManagedObject->getDestinationmirrorportEnable()) {
                        status = WRC_NSM_INTERFACE_IS_DESTINATION_MIRROR_PORT;
                    } 
                    else if (isPortProfiled) {
                        status = SSM_DCM_ERR_ACL_APPM_PROFILED_PORT;
                    } else {
						NsmLocalPBRManagedObject *pNsmLocalPBRManagedObject = NULL;
						pNsmLocalPBRManagedObject = pPhyIntfLocalManagedObject->getPBRMO();   
						
						if (pNsmLocalPBRManagedObject != NULL) {
							status = WRC_SSM_DCM_ERR_ACL_RULE_DUP;
						}	                        

                        if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
                            status = WRC_SSM_DCM_ERR_ACL_PORTCHANNEL_PHYINTF_CONFLICT;
                    }
                }
            }
        } else {
            status = WRC_NSM_INTERFACE_DOES_NOT_EXIST;
        }

        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

        return (status);
    }

    ResourceId IntfConfLocalWorker::updatePBRPhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId status   = WAVE_MESSAGE_SUCCESS;
        char buffer[MAX_STR];
        buffer[MAX_STR -1]  = '\0';
        string ifname       = "";
        UI32 bufferSize     = sizeof (buffer);
        PBRPhyIntfMessage *pMsg 		   = dynamic_cast<PBRPhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());        
        trace(TRACE_LEVEL_INFO, string("updatePBRPhyIntfStep: polName: ") + pMsg->getRouteMapName());
                
		string routeMapName = "";
		trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::updatePBRPhyIntfStep pMsg->getCmdCode() ==> ") + pMsg->getCmdCode());
		int op_code = pMsg->getCmdCode();

		if (true  == NsmUtils::isValidThreeTuple(pMsg->getIfName ())) {
			string twoTuppleIfName;
			string threeTupple = pMsg->getIfName ();
			NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
			GENERATE_SHORT_IF_NAME_FOR_BACK_END(pMsg->getIfType(), twoTuppleIfName.c_str(), buffer, bufferSize);
		} else {
			GENERATE_SHORT_IF_NAME_FOR_BACK_END(pMsg->getIfType(), pMsg->getIfName().c_str(), buffer, bufferSize);
		}
		ifname = buffer;				
		vector<string> args;
		trace(TRACE_LEVEL_INFO, string("updatePBRPhyIntfStep: ifname: ") + ifname);
		
		/* Checking Only while removing policy as we permit unknown type policy creation */
		if (pMsg->getCmdCode() == SSM_CLIENT_SET_ROUTEMAP_REMOVE_INTERFACE) {
			int ret = getRouteMapNameForInterface(pMsg->getIfType(), pMsg->getIfName(), routeMapName);
			if (ret == 0) {
				trace(TRACE_LEVEL_INFO, string("updatePBRPhyIntfStep: polName: ")+ routeMapName);				
			} else {
				trace(TRACE_LEVEL_INFO, string("updatePBRPhyIntfStep: GetrouteMapName failed" ));
				return (WAVE_MESSAGE_ERROR);
			}
			args.push_back(routeMapName);
			args.push_back(ifname);

            for(size_t tmpIndx = 0; tmpIndx < args.size(); tmpIndx++) {
              trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::updatePBRPhyIntfStep ARG VALUES ## ==> ") + args[tmpIndx]);
            }

			GenericClientMessage_ client_msg(args, op_code);
			trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::updatePBRPhyIntfStep op_code ==> ") + op_code);
			status = sendSynchronouslyToWaveClient ("ssm", &client_msg);
		    if(status == WAVE_MESSAGE_SUCCESS) {
                status = client_msg.getClientStatus();
                tracePrintf(TRACE_LEVEL_INFO, "configurePBRVeIntfStep: Success %lld",  status);
            } else {
                tracePrintf(TRACE_LEVEL_INFO,  "configurePBRVeIntfStep: Failed: %lld",  client_msg.getClientStatus());
            }
			return status;						
			//return WAVE_MESSAGE_SUCCESS;
		} else {
			args.push_back(pMsg->getRouteMapName());
			args.push_back(ifname);

            for(size_t tmpIndx = 0; tmpIndx < args.size(); tmpIndx++) {
              trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::updatePBRPhyIntfStep ARG VALUES ## ==> ") + args[tmpIndx]);
            }
			trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::updatePBRPhyIntfStep op_code ==> ") + op_code);	
			GenericClientMessage_ client_msg(args, op_code);
			status = sendSynchronouslyToWaveClient ("ssm", &client_msg);
			if(status == WAVE_MESSAGE_SUCCESS) {
				status = client_msg.getClientStatus();
				tracePrintf(TRACE_LEVEL_INFO, "configurePBRVeIntfStep: Success %lld",  status);
			} else {
				tracePrintf(TRACE_LEVEL_INFO,  "configurePBRVeIntfStep: Failed: %lld",  client_msg.getClientStatus());
			}
			return status;			
			//return WAVE_MESSAGE_SUCCESS;
		}						

        return (status);
		//return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId IntfConfLocalWorker::updatePBRPhyIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
		tracePrintf(TRACE_LEVEL_INFO, "Inside updatePBRPhyIntfMOStep : Debug 1");
        PBRPhyIntfMessage *pMsg = dynamic_cast<PBRPhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		tracePrintf(TRACE_LEVEL_INFO, "Inside updatePBRPhyIntfMOStep : Debug 2");

        if (pMsg->getCmdCode () == SSM_CLIENT_SET_ROUTEMAP_APPLY_INTERFACE) {
			tracePrintf(TRACE_LEVEL_INFO, "Inside updatePBRPhyIntfMOStep : Debug 3");
            status = assignPBRPhyIntfMOStep(pPrismSynchronousLinearSequencerContext);
        } else if (pMsg->getCmdCode () == SSM_CLIENT_SET_ROUTEMAP_REMOVE_INTERFACE) {
			tracePrintf(TRACE_LEVEL_INFO, "Inside updatePBRPhyIntfMOStep : Debug 4");
            status = removePBRPhyIntfMOStep(pPrismSynchronousLinearSequencerContext);
        }

        return (status);
    }

    ResourceId IntfConfLocalWorker::assignPBRPhyIntfMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId  status                                      = WAVE_MESSAGE_ERROR;
        PBRPhyIntfMessage *pMsg                 				= dynamic_cast<PBRPhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject   = NULL;
        string routeMapName                                     = pMsg->getRouteMapName();
		trace (TRACE_LEVEL_INFO, "****Debug : RouteMapName ==> " + routeMapName);
        string ifName                                           = pMsg->getIfName();        

        trace (TRACE_LEVEL_INFO, "****Inside assignPBRPhyIntfMOStep");
        //IpPolicyGetPBRObjectMessage *policyMsg = new IpPolicyGetPBRObjectMessage(routeMapName);

        //if (policyMsg) {
        //    trace (TRACE_LEVEL_DEVEL, "assignIpAclPhyIntfMOStep3 - Obtained the Policy MO");

        //    status = sendSynchronously (policyMsg);
        //    if (status == WAVE_MESSAGE_SUCCESS && policyMsg->getCompletionStatus() == WAVE_MESSAGE_SUCCESS) {
        //        status = WAVE_MESSAGE_SUCCESS;
        //        trace (TRACE_LEVEL_INFO, "assignPBRPhyIntfMOStep SSM Message returned success");
        //    } else {
        //        status = WAVE_MESSAGE_ERROR;
        //        trace (TRACE_LEVEL_ERROR, "assignPBRPhyIntfMOStep SSM Message returned with  error");
        //        delete policyMsg;
        //        return (status);
        //    }        						

            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
			trace (TRACE_LEVEL_INFO, "****Debug : 1");
            vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
            if (NULL != pResults) {
			  	trace (TRACE_LEVEL_INFO, "****Debug : 2");
                UI32 numberOfResults = pResults->size ();
                if (0 == numberOfResults) {
				  	trace (TRACE_LEVEL_INFO, "****Debug : 3");
                    status = WAVE_MESSAGE_ERROR;
                }else if (1 == numberOfResults) {
				  	trace (TRACE_LEVEL_INFO, "****Debug : 4");
                    trace (TRACE_LEVEL_INFO, "assignPBRPhyIntfMOStep- Obtained the PhyIntfLocalManagedObject");
                    pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);

                    NsmLocalPBRManagedObject *NsmPBRMO = new NsmLocalPBRManagedObject(dynamic_cast
                        <NsmLocalObjectManager*>(getPWaveObjectManager()), routeMapName);
					if(NsmPBRMO == NULL)
					  	trace (TRACE_LEVEL_INFO, "****Debug : NsmPBRMO is NULL");					
                    pPhyIntfLocalManagedObject->setPBR(NsmPBRMO);

                    updateWaveManagedObject(pPhyIntfLocalManagedObject);
                    status = WAVE_MESSAGE_SUCCESS;
                } else {
                    status = WAVE_MESSAGE_ERROR;
                }
                /* GC collection */
                pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                delete pResults;
            }
          //  delete policyMsg;

        //} else {
        //    status = WAVE_MESSAGE_ERROR;
        //}

        return (status);

    }

    ResourceId IntfConfLocalWorker::removePBRPhyIntfMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId  status                                      = WAVE_MESSAGE_ERROR;
        PBRPhyIntfMessage *pMsg                				    = dynamic_cast<PBRPhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject   = NULL;
        string ifName                                           = pMsg->getIfName();        
        trace (TRACE_LEVEL_DEVEL, "****Inside removePBRPhyIntfMOStep");

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                status = WAVE_MESSAGE_ERROR;
            }else if (1 == numberOfResults) {
                trace (TRACE_LEVEL_DEVEL, "removeIpAclPhyIntfMOStep4- Obtained the PhyInterface object ");
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                if (pPhyIntfLocalManagedObject) {
                    pPhyIntfLocalManagedObject->setPBR(NULL);

                    updateWaveManagedObject(pPhyIntfLocalManagedObject);
                    status = WAVE_MESSAGE_SUCCESS;
                }
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
            /* GC collection */
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        } else {
            status = WAVE_MESSAGE_ERROR;
        }

        return (status);

    }

	void IntfConfLocalWorker::NsmAssocIpAclPolicyPhyIntfMessageHandler (NsmAssocIpAclPolicyPhyIntfMessage *pNsmAssocIpAclPolicyPhyIntfMessage)
	{
		PrismSynchronousLinearSequencerStep sequencerSteps[] =
		{
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::validateIpAclPhyIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateIpAclPhyIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateIpAclPhyIntfMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmAssocIpAclPolicyPhyIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
	}

	ResourceId IntfConfLocalWorker::validateIpAclPhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		NsmAssocIpAclPolicyPhyIntfMessage *pMsg 				= dynamic_cast<NsmAssocIpAclPolicyPhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ResourceId status 										= WAVE_MESSAGE_SUCCESS;
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject 	= NULL;
		ssmAcl_binding_direction direction 						= pMsg->getDirection();
		string ifName											= pMsg->getIfName();
		UI32 found												= 0;

		// Validate 1: validating the command support on the platform.
		if(direction == SSMACL_BINDING_DIRECTION_OUT_E) {
			if(!Capabilities::isCapabilityPresent(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS))
			{
				trace (TRACE_LEVEL_DEBUG, "Applying IP ACL policy in egress direction command not supported on this platform.");
				return SSM_DCM_ERR_ACL_APPLIED_INTF_EGRESS;
			}
		}

		/* Validate 2: Check if port mode is ISL.
		 * If yes block the ACL configuration
		 */

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults != NULL && pResults->size () == 1) {
        	pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
			
			if (pPhyIntfLocalManagedObject) {
				if ((DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pPhyIntfLocalManagedObject->getPortrole())) {
					status = WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
				}
				else if (pMsg->getCmdCode() == SSMACL_DCMD_BIND_L3_IPV4_ACL) {
                    bool isPortProfiled = false;
                    APPMUtils::isPortProfilePort(pPhyIntfLocalManagedObject, isPortProfiled);
					if (pPhyIntfLocalManagedObject->getDestinationmirrorportEnable()) {
						status = WRC_NSM_INTERFACE_IS_DESTINATION_MIRROR_PORT; 
					} 
                    else if (isPortProfiled) {
						status = SSM_DCM_ERR_ACL_APPM_PROFILED_PORT;
					} else {
						found = pPhyIntfLocalManagedObject->getIngressEgressIpAclPolicyDirectioncount (direction);

                    	if (found)
                        	status = WRC_SSM_DCM_ERR_ACL_RULE_DUP;

						if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
							status = WRC_SSM_DCM_ERR_ACL_PORTCHANNEL_PHYINTF_CONFLICT;
					}
				}
			}
		} else {
			status = WRC_NSM_INTERFACE_DOES_NOT_EXIST;
		}

		if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}

		return (status);
	}

	ResourceId IntfConfLocalWorker::updateIpAclPhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status 	= WAVE_MESSAGE_SUCCESS;
		char buffer[MAX_STR];
		buffer[MAX_STR -1] 	= '\0';
		string ifname 		= "";
		UI32 bufferSize 	= sizeof (buffer);
		NsmAssocIpAclPolicyPhyIntfMessage *pMsg = dynamic_cast<NsmAssocIpAclPolicyPhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ssmAcl_binding_direction direction = pMsg->getDirection();
		trace(TRACE_LEVEL_DEVEL, string("updateAclPhyIntf: polName: ") + pMsg->getAclName());

		SsmIpAclPolicyMessage *cMsg = new SsmIpAclPolicyMessage();
		if (cMsg) {
			string policyName = "";
			/* Checking Only while removing policy as we permit unknown type policy creation */
			if (pMsg->getCmdCode() == SSMACL_DCMD_UNBIND_L3_IPV4_ACL) {
				int ret = getIpAclPolicyNameForInterface(PHY_INTF_TYPE, pMsg->getIfName(), policyName, direction);
				if (ret == 0) {
					trace(TRACE_LEVEL_DEVEL, string("updateIpAclConfigPhyIntf: polName: ")+ policyName);
					cMsg->setPolicyName(policyName);
				} else {
					trace(TRACE_LEVEL_DEVEL, string("updateIpAclConfigPhyIntf: GetPolicyname failed" ));
					return (WAVE_MESSAGE_ERROR);
				}
			} else {
				cMsg->setPolicyName(pMsg->getAclName());
			}

			if (true  == NsmUtils::isValidThreeTuple(pMsg->getIfName ())) {
				string twoTuppleIfName;
				string threeTupple = pMsg->getIfName ();
				NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
              	GENERATE_SHORT_IF_NAME_FOR_BACK_END(pMsg->getIfType(), twoTuppleIfName.c_str(), buffer, bufferSize);
            } else {
                GENERATE_SHORT_IF_NAME_FOR_BACK_END(pMsg->getIfType(), pMsg->getIfName().c_str(), buffer, bufferSize);
            }
			ifname = buffer;
			cMsg->setIfname(ifname);
			cMsg->setDirection(direction);
			cMsg->setOpCode(pMsg->getCmdCode ());
            /* Set Acl Type to differentiate between ip and mac acl */
			cMsg->setPolicyType(SSMACL_TYPE_L3_IPV4_E);

			status = sendSynchronouslyToWaveClient ("ssm", cMsg);

			if (status == WAVE_MESSAGE_SUCCESS) {
				status = cMsg->getClientStatus();
				trace(TRACE_LEVEL_INFO, string("updateIpAclPhyIntfStep: Success: ") + status);
				if ( status == WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS_MAC )
				{
					status = WRC_SSM_DCM_ERR_APPLY_MAC_ACL_ON_INTF;
				}
				pMsg->setCompletionStatus(status);
			} else {
				trace(TRACE_LEVEL_INFO, string("updateIpAclPhyIntfStep: Failed: ") + cMsg->getClientStatus());
				status = WAVE_MESSAGE_ERROR;
			}

			delete cMsg;
		} else {
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "updateIpAclPhyIntfStep: Message creation failed");
		}

		return (status);
	}

	ResourceId IntfConfLocalWorker::updateIpAclPhyIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
        ResourceId  status   = WAVE_MESSAGE_ERROR;
		NsmAssocIpAclPolicyPhyIntfMessage *pMsg = dynamic_cast<NsmAssocIpAclPolicyPhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

		if (pMsg->getCmdCode () == SSMACL_DCMD_BIND_L3_IPV4_ACL) {
			status = assignIpAclPhyIntfMOStep(pPrismSynchronousLinearSequencerContext);
		} else if (pMsg->getCmdCode () == SSMACL_DCMD_UNBIND_L3_IPV4_ACL) {
			status = removeIpAclPhyIntfMOStep(pPrismSynchronousLinearSequencerContext);
		}

		return (status);
	}

	ResourceId IntfConfLocalWorker::assignIpAclPhyIntfMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
        ResourceId  status   									= WAVE_MESSAGE_ERROR;
		NsmAssocIpAclPolicyPhyIntfMessage *pMsg 				= dynamic_cast<NsmAssocIpAclPolicyPhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject 	= NULL;
		string aclName											= pMsg->getAclName();
		string ifName 											= pMsg->getIfName();
		ssmAcl_binding_direction direction  					= pMsg->getDirection();

		trace (TRACE_LEVEL_DEVEL, "****Inside assignIpAclPhyIntfMOStep");
		SsmGlobalGetIpAclPolicyMessage *policyMsg = new SsmGlobalGetIpAclPolicyMessage(aclName);

		if (policyMsg) {
           	trace (TRACE_LEVEL_DEVEL, "assignIpAclPhyIntfMOStep3 - Obtained the Policy MO");

			status = sendSynchronously (policyMsg);
			if (status == WAVE_MESSAGE_SUCCESS && policyMsg->getCompletionStatus() == WAVE_MESSAGE_SUCCESS) {
				status = WAVE_MESSAGE_SUCCESS;
				trace (TRACE_LEVEL_INFO, "assignIpAclPhyIntfMOStep SSM Message returned success");
			} else {
				status = WAVE_MESSAGE_ERROR;
				trace (TRACE_LEVEL_ERROR, "assignIpAclPhyIntfMOStep SSM Message returned with  error");
				delete policyMsg;
				return (status);
			}
			policyMsg->getPolicyObjId();

        	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        	syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        	if (NULL != pResults) {
            	UI32 numberOfResults = pResults->size ();
            	if (0 == numberOfResults) {
                	status = WAVE_MESSAGE_ERROR;
            	}else if (1 == numberOfResults) {
           			trace (TRACE_LEVEL_INFO, "assignIpAclPhyIntfMOStep4- Obtained the PhyIntfLocalManagedObject");
                	pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);

                	NsmLocalIngressEgressIpAclPolicyManagedObject *NsmIngressEgressIpAclMO = new
						NsmLocalIngressEgressIpAclPolicyManagedObject(dynamic_cast
						<NsmLocalObjectManager*>(getPWaveObjectManager()), policyMsg->getPolicyObjId(), direction);
					pPhyIntfLocalManagedObject->addIngressEgressIpAclManagedObjectToVector
						(NsmIngressEgressIpAclMO);

					updateWaveManagedObject(pPhyIntfLocalManagedObject);
                	status = WAVE_MESSAGE_SUCCESS;
            	} else {
                	status = WAVE_MESSAGE_ERROR;
            	}
                /* GC collection */
                pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            	delete pResults;
        	}
			delete policyMsg;

		} else {
			status = WAVE_MESSAGE_ERROR;
		}

		return (status);

	}

	ResourceId IntfConfLocalWorker::removeIpAclPhyIntfMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
        ResourceId  status   									= WAVE_MESSAGE_ERROR;
		NsmAssocIpAclPolicyPhyIntfMessage *pMsg 				= dynamic_cast<NsmAssocIpAclPolicyPhyIntfMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject 	= NULL;
		string ifName 											= pMsg->getIfName();
		ssmAcl_binding_direction direction 						= pMsg->getDirection();
        trace (TRACE_LEVEL_DEVEL, "****Inside removeIpAclPhyIntfMOStep");

       	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
       	syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
       	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
       	if (NULL != pResults) {
           	UI32 numberOfResults = pResults->size ();
           	if (0 == numberOfResults) {
               	status = WAVE_MESSAGE_ERROR;
           	}else if (1 == numberOfResults) {
      			trace (TRACE_LEVEL_DEVEL, "removeIpAclPhyIntfMOStep4- Obtained the PhyInterface object ");
               	pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
               	if (pPhyIntfLocalManagedObject) {
					pPhyIntfLocalManagedObject->delIngressEgressIpAclManagedObjectFromVector(direction);

					updateWaveManagedObject(pPhyIntfLocalManagedObject);
					status = WAVE_MESSAGE_SUCCESS;
				}
           	} else {
               	status = WAVE_MESSAGE_ERROR;
       		}
            /* GC collection */
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
           	delete pResults;
       	} else {
			status = WAVE_MESSAGE_ERROR;
		}

		return (status);

	}

	/* Storm Control */
    void IntfConfLocalWorker::NsmStormControlIntfSetMessageHandler( NsmStormControlIntfSetMessage *pNsmStormControlIntfSetMessage)
	{
        // Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::validateStormControlPhyIntfConf),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateStormControlPhyIntfConf),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateStormControlPhyIntfMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
		};

        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pNsmStormControlIntfSetMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
	}

    ResourceId IntfConfLocalWorker::validateStormControlPhyIntfConf 
		(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {
		NSM_PLUG_INFO_S(string ("Entered IntfConfLocalWorker::validateStormControlPhyIntfConf"));

	    // Validate 1: Check if the command is supported on the platform.
	    if (!(Capabilities::instance()->isCapabilityPresent(FEAT_CAPABILITY_STORMCONTROL))) {
	 	    trace (TRACE_LEVEL_DEBUG, "STORMCONTROL commands not supported on this platform.");
		    return WRC_NSM_ERR_DCM_STORMCONTROL_NOT_SUPPORTED;
	    }


        NsmStormControlIntfSetMessage *pNsmStormControlIntfSetMessage =
           dynamic_cast<NsmStormControlIntfSetMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ()); 

    trace(TRACE_LEVEL_DEBUG,
			string("IFTYPE : ") + pNsmStormControlIntfSetMessage->getIfType());
	trace(TRACE_LEVEL_DEBUG,
			string("limit-bps") + pNsmStormControlIntfSetMessage->getBps());
	trace(
			TRACE_LEVEL_DEBUG,
			string("ProtocolType")
					+ pNsmStormControlIntfSetMessage->getProtocolType());

	UI64 rate_limit_bps = pNsmStormControlIntfSetMessage->getBps();

	//Validate the limit-bps values for storm-control based on the IFTYPE.

	if ((pNsmStormControlIntfSetMessage->getOpCode() == QOS_BUM_DCMD_ENABLE)
			&& ((pNsmStormControlIntfSetMessage->getRateFormat())
					== QOS_BUM_RATE_FORMAT_BPS_E)) {

		switch (pNsmStormControlIntfSetMessage->getIfType()) {

		case IF_TYPE_GI: {
			trace(TRACE_LEVEL_DEBUG, string("Onegig found. "));
			UI64 oneGig = 1000000000;
			if ((rate_limit_bps < 0) || (rate_limit_bps > oneGig)) {
				return WRC_NSM_ERR_DCM_STORMCONTROL_LIMIT_BPS_OUT_OF_RANGE;
			}
			break;
		}

		case IF_TYPE_TE: {
			trace(TRACE_LEVEL_DEBUG, string("Tengig found. "));
			UI64 tenGig = 10000000000ULL;
			if ((rate_limit_bps < 0) || (rate_limit_bps > tenGig)) {
				return WRC_NSM_ERR_DCM_STORMCONTROL_LIMIT_BPS_OUT_OF_RANGE;
			}
			break;
		}

		case IF_TYPE_FO: {
			trace(TRACE_LEVEL_DEBUG, string("Fortygig found. "));
			UI64 fortyGig = 40000000000ULL;
			if ((rate_limit_bps < 0) || (rate_limit_bps > fortyGig)) {
				return WRC_NSM_ERR_DCM_STORMCONTROL_LIMIT_BPS_OUT_OF_RANGE;
			}
			break;
		}
		}

	}

        
		ResourceId 					status 						= WAVE_MESSAGE_SUCCESS;
        PhyIntfLocalManagedObject 	*pPhyIntfLocalManagedObject = NULL;
		string 						ifName 						= pNsmStormControlIntfSetMessage->getIfName();
		UI32						protocolType				= pNsmStormControlIntfSetMessage->getProtocolType();

      	NSM_PLUG_INFO_S(string("NsmStormControlIntfSetMessage:ifName: ") 
			+ ifName);

      	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());          
	   	syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size();
            if (0 == numberOfResults)
            {
                trace (TRACE_LEVEL_DEBUG, string("P-Results is ZERO ***** -------"));
                status = WRC_NSM_INTERFACE_DOES_NOT_EXIST;
            }
            else if (1 == numberOfResults)
            {
                trace (TRACE_LEVEL_DEBUG, string("P-Results is ONE ***** -------"));
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);

                // Validate 2: Check if port has policer enabled in ingress direction.
                if(pPhyIntfLocalManagedObject->getInputPolicyMapId() != ObjectId::NullObjectId)
                {
                    status = WRC_NSM_ERR_DCM_POLICER_CONFIGURED;
                    goto last;
                }

                // Validate 3: Check if the port is ISL.
                if ((DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED)
                        && (pPhyIntfLocalManagedObject->getPortrole())) {

                    pPrismSynchronousLinearSequencerContext->getCache()->put(PORT_ROLE,"true");
                    if((QOS_BUM_DCMD_ENABLE) == pNsmStormControlIntfSetMessage->getOpCode()) {
                        status = WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
                    }
                    goto last;
                }

                // Validate 4: Check if the port is the destination mirror port.
                if (pPhyIntfLocalManagedObject->getDestinationmirrorportEnable())
                {
                    status = WRC_NSM_INTERFACE_IS_DESTINATION_MIRROR_PORT;
                    goto last;
                }

                /* Validate 5: Check if the configuration related to the key broadcast/multicast/uucast already exists. */
                vector <WaveManagedObjectPointer<NsmStormControlIntfConfLocalManagedObject> > bumConfigMO =
                    pPhyIntfLocalManagedObject->getBumConfig();

                for (unsigned int i = 0; i < bumConfigMO.size(); i++)
				{
                    if ((bumConfigMO[i]->getProtocolType() == protocolType)
						&& ((pNsmStormControlIntfSetMessage->getOpCode() == QOS_BUM_DCMD_ENABLE)))
					{
                       status = WRC_SSM_STORMCONTROL_ERR_ALREADY_CONFIGURED;
					   goto last;
					}
					
				}
			}
        }
        else
            status = WAVE_MESSAGE_ERROR;

last:
        NSM_PLUG_INFO_S(string("returned status: ")  
			+ FrameworkToolKit::localize(status));

        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        return (status);
    }

    ResourceId IntfConfLocalWorker::updateStormControlPhyIntfConf 
		(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {
        NSM_PLUG_INFO_S(string("Entered IntfConfLocalWorker::updateStormControlPhyIntfConf"));

        NsmStormControlIntfSetMessage *pNsmStormControlIntfSetMessage
            = dynamic_cast<NsmStormControlIntfSetMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        ResourceId							status 			= WAVE_MESSAGE_ERROR;
        string 								ifNamestr 		= "";
        char 								buffer[25];
        buffer[24]      									= '\0';
        UI32 								bufferSize 		= sizeof (buffer);

        NsmStormControlClientIntfConfSetMsg *pNsmStormControlClientIntfConfSetMsg = new NsmStormControlClientIntfConfSetMsg();

        if (NULL == pNsmStormControlIntfSetMessage)
        {
            NSM_PLUG_INFO_S(string("pNsmStormControlIntfSetMessage is NULL:"));
            return (WAVE_MESSAGE_ERROR);
        }

        if(0 == (string("true").compare(pPrismSynchronousLinearSequencerContext->getCache()->getStringValue(PORT_ROLE)))
                && (QOS_BUM_DCMD_DISABLE == (pNsmStormControlIntfSetMessage->getOpCode()))) {
            pNsmStormControlIntfSetMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            return WAVE_MESSAGE_SUCCESS;
        }

        /* Three tupple to two tupple conversion */
        if (true  == NsmUtils::isValidThreeTuple(pNsmStormControlIntfSetMessage->getIfName ()))
        {
            string twoTuppleIfName;
            string threeTupple = pNsmStormControlIntfSetMessage->getIfName ();
            NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
            ifNamestr =  (twoTuppleIfName.c_str());
        }
        else
        {
            ifNamestr = (pNsmStormControlIntfSetMessage->getIfName());
        }

        NSM_PLUG_INFO_S(string("ifNamestr:- ") 
			+ ifNamestr + string("  ifType:- ") 
			+  pNsmStormControlIntfSetMessage->getIfType());

		if (pNsmStormControlIntfSetMessage->getIfType()  == IF_TYPE_TE)
			snprintf(buffer, bufferSize, "te%s", ifNamestr.c_str());
		else if (pNsmStormControlIntfSetMessage->getIfType()  == IF_TYPE_GI)
            snprintf(buffer, bufferSize, "gi%s", ifNamestr.c_str());
        else if (pNsmStormControlIntfSetMessage->getIfType()  == IF_TYPE_FO)
			snprintf(buffer, bufferSize, "fo%s", ifNamestr.c_str());

        pNsmStormControlClientIntfConfSetMsg->setIfName(buffer);
        pNsmStormControlClientIntfConfSetMsg->setOpCode(pNsmStormControlIntfSetMessage->getOpCode());
        pNsmStormControlClientIntfConfSetMsg->setProtocolType(pNsmStormControlIntfSetMessage->getProtocolType());
        pNsmStormControlClientIntfConfSetMsg->setRateFormat(pNsmStormControlIntfSetMessage->getRateFormat());
        pNsmStormControlClientIntfConfSetMsg->setAction(pNsmStormControlIntfSetMessage->getAction());
        if ((pNsmStormControlIntfSetMessage->getRateFormat()) == QOS_BUM_RATE_FORMAT_BPS_E)            
			pNsmStormControlClientIntfConfSetMsg->setBps(pNsmStormControlIntfSetMessage->getBps());
        else if ((pNsmStormControlIntfSetMessage->getRateFormat()) == QOS_BUM_RATE_FORMAT_PRECENTAGE_E)
            pNsmStormControlClientIntfConfSetMsg->setPercentage(pNsmStormControlIntfSetMessage->getPercentage());

        NSM_PLUG_INFO_S(string("Sending message to qos Backend" ));

        if (pNsmStormControlClientIntfConfSetMsg!=NULL)
            status = sendSynchronouslyToWaveClient ("qos", pNsmStormControlClientIntfConfSetMsg);

        pNsmStormControlIntfSetMessage->setCompletionStatus (pNsmStormControlClientIntfConfSetMsg->getCompletionStatus());
        if( status == WAVE_MESSAGE_SUCCESS)
        {
            if(pNsmStormControlClientIntfConfSetMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
            {
                NSM_PLUG_INFO_S(string ("Response processing: CompletionStatus ")
                + string("returned error: ")  
				+ FrameworkToolKit::localize(pNsmStormControlClientIntfConfSetMsg->getCompletionStatus()));

                status = pNsmStormControlIntfSetMessage->getCompletionStatus();

                delete pNsmStormControlClientIntfConfSetMsg;
                return (status);
            }

            NSM_PLUG_INFO_S(string ("Response processing: CompletionStatus ")
            	+ string("returned success: ")  
				+ FrameworkToolKit::localize(pNsmStormControlClientIntfConfSetMsg->getCompletionStatus()));

            ResourceId ClientStatus = pNsmStormControlClientIntfConfSetMsg->getClientStatus();

            if (ClientStatus != WAVE_MESSAGE_SUCCESS)
            {
                NSM_PLUG_INFO_S(string ("Response processing: ClientStatus ")
                	+ string("returned error: ")  
					+ FrameworkToolKit::localize(ClientStatus));

                pNsmStormControlIntfSetMessage->setCompletionStatus(ClientStatus);

                delete pNsmStormControlClientIntfConfSetMsg;
                return (ClientStatus);
            }
			else
				NSM_PLUG_INFO_S(string ("Response processing: ClientStatus ") 
                	+ string("returned: ")  
					+ FrameworkToolKit::localize(ClientStatus));
        }
        else
            NSM_PLUG_INFO_S(string ("Response processing: sendSynchronously")
                            + string(" to Backend Failed "));

        if(pNsmStormControlClientIntfConfSetMsg)
            delete pNsmStormControlClientIntfConfSetMsg;
        return (status);
    }

    ResourceId IntfConfLocalWorker::updateStormControlPhyIntfMO(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {
        NSM_PLUG_INFO_S(string("Entered IntfConfLocalWorker::updateStormControlPhyIntfMO"));

        NsmStormControlIntfSetMessage *pNsmStormControlIntfSetMessage =
            dynamic_cast<NsmStormControlIntfSetMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        ResourceId  status   = WAVE_MESSAGE_ERROR;
		
                                                                                                                                      if (pNsmStormControlIntfSetMessage->getOpCode() == QOS_BUM_DCMD_ENABLE) {
            status = configureBumPhyIntfMOStep(pPrismSynchronousLinearSequencerContext);
																																	  } else if (pNsmStormControlIntfSetMessage->getOpCode() == QOS_BUM_DCMD_DISABLE) {
            status = unconfigureBumPhyIntfMOStep(pPrismSynchronousLinearSequencerContext);
        }
                                                                                                                                      return (status);
    }

    ResourceId IntfConfLocalWorker:: configureBumPhyIntfMOStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
		NSM_PLUG_INFO_S(string("Entered IntfConfLocalWorker::configureBumPhyIntfMOStep"));

		NsmStormControlIntfSetMessage    *pNsmStormControlIntfSetMessage    =
            dynamic_cast<NsmStormControlIntfSetMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		
		ResourceId  status  			= WAVE_MESSAGE_ERROR;
        string      ifName         		= pNsmStormControlIntfSetMessage->getIfName();

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        if (pNsmStormControlIntfSetMessage == NULL) {
            NSM_PLUG_INFO_S(string("pNsmStormControlIntfSetMessage == NULL"));
			return (WAVE_MESSAGE_ERROR);
		}	
        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults)
                status = WAVE_MESSAGE_ERROR;
            else if (1 == numberOfResults)
            {
                trace (TRACE_LEVEL_INFO, "Bum Control MO Configure step- Obtained the PhyIntfLocalManagedObject");
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);

				NsmStormControlIntfConfLocalManagedObject *pNsmStormControlIntfConfLocalManagedObject = new
                    NsmStormControlIntfConfLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()), 
					pNsmStormControlIntfSetMessage->getProtocolType(), pNsmStormControlIntfSetMessage->getRateFormat(), 
					pNsmStormControlIntfSetMessage->getBps(), pNsmStormControlIntfSetMessage->getPercentage(), 
					pNsmStormControlIntfSetMessage->getAction());
				
				pPhyIntfLocalManagedObject->addBumControlManagedObjectToVector(pNsmStormControlIntfConfLocalManagedObject);
                updateWaveManagedObject(pPhyIntfLocalManagedObject);

                status = WAVE_MESSAGE_SUCCESS;
            }
            else
                status = WAVE_MESSAGE_ERROR;

            if(pPrismSynchronousLinearSequencerContext)
                pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			
         	if (pResults){
    	        delete pResults;
			}
        }
        return (status);
    }
		
    ResourceId IntfConfLocalWorker::unconfigureBumPhyIntfMOStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {
        NsmStormControlIntfSetMessage   *pNsmStormControlIntfSetMessage           =
            dynamic_cast<NsmStormControlIntfSetMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        if (pNsmStormControlIntfSetMessage == NULL) {
            NSM_PLUG_INFO_S(string("pNsmStormControlIntfSetMessage == NULL"));
			return (WAVE_MESSAGE_ERROR);
		}
		
		ResourceId                      status          		= WAVE_MESSAGE_ERROR;
        string                          ifName          		= pNsmStormControlIntfSetMessage->getIfName();
        UI32                            protocolType    		= pNsmStormControlIntfSetMessage->getProtocolType();
		
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults)
                status = WAVE_MESSAGE_ERROR;
            else if (1 == numberOfResults)
            {
                trace (TRACE_LEVEL_INFO, "Bum Control MO UnConfigure step- Obtained the PhyIntfLocalManagedObject");
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
				if (pPhyIntfLocalManagedObject)
	                pPhyIntfLocalManagedObject->delBumControlManagedObjectFromVector(protocolType);
                updateWaveManagedObject(pPhyIntfLocalManagedObject);

                status = WAVE_MESSAGE_SUCCESS;
            }
            else
                status = WAVE_MESSAGE_ERROR;

            if(pPrismSynchronousLinearSequencerContext)
                pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            if (pResults)
            delete pResults;
        }
        return (status);
    }

   void IntfConfLocalWorker::NsmLocaldestinationMirrorportMessageHandler(NsmLocaldestinationMirrorportMessage *pNsmLocaldestinationMirrorportMessage)
    {
        string						DestinationifName			= pNsmLocaldestinationMirrorportMessage->getDestifName();
		UI32						opCode						= pNsmLocaldestinationMirrorportMessage->getOpcode();
		ResourceId 					status 						= WAVE_MESSAGE_SUCCESS;
        PhyIntfLocalManagedObject 	*pPhyIntfLocalManagedObject = NULL;
		int							found						= 0;		
		trace(TRACE_LEVEL_DEBUG,string("Entering IntfConfLocalWorker::NsmLocaldestinationMirrorportMessageHandler"));
		
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&DestinationifName,"ifName"));	
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		
		startTransaction ();
        if (pResults != NULL && pResults->size () == 1) {
            pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
			
            if (pPhyIntfLocalManagedObject) {
				/* First Check for opcode */
				if (opCode == SPAN_ADD_MIRRORED_PORT) {
					NSM_PLUG_INFO_S(string("opCode == SPAN_ADD_MIRRORED_PORT"));
					/* First Checking for Any ip acl applied on that interface 
					 	if Yes do not allow mirroring 
						Keep both as direction */
					found = pPhyIntfLocalManagedObject->getIngressEgressIpAclPolicyDirectioncount(SSMACL_BINDING_DIRECTION_BOTH_E);
					if (found) {
						status = NSM_SPAN_DEST_PORT_IN_ACL;
						trace(TRACE_LEVEL_DEBUG,string("IntfConfLocalWorker::NsmLocaldestinationMirrorportMessageHandler Returning due to ip acl applied"));
						rollbackTransaction();						
						if (pResults) {
							WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
						}
						pNsmLocaldestinationMirrorportMessage->setCompletionStatus (status);	
						reply(pNsmLocaldestinationMirrorportMessage);
						return;
					} else if(true == (pPhyIntfLocalManagedObject->getUdldEnable())) {
						status = UDLD_ENABLED_PORT_CANNOT_BE_DEST_MIRROR_PORT;
						trace(TRACE_LEVEL_DEBUG,string("IntfConfLocalWorker::NsmLocaldestinationMirrorportMessageHandler Returning due to port being mirrored port"));
						rollbackTransaction();
						if (pResults) {
							WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
						}
						pNsmLocaldestinationMirrorportMessage->setCompletionStatus (status);
						reply(pNsmLocaldestinationMirrorportMessage);
						return;
					}
					else {
						pPhyIntfLocalManagedObject->setDestinationmirrorportEnable (true);
						//pPhyIntfLocalManagedObject->setMtu(NSM_MTU_PORTMIRROR_DESTINATION_VAL);
						updateWaveManagedObject(pPhyIntfLocalManagedObject);
						trace(TRACE_LEVEL_DEBUG, string("IntfConfLocalWorker::NsmLocaldestinationMirrorportMessageHandler Setting destination as mirrored port enable"));
					}
				}
				else if (opCode == SPAN_DEL_MIRRORED_PORT) {
					NSM_PLUG_INFO_S(string("opCode == SPAN_DEL_MIRRORED_PORT"));
					pPhyIntfLocalManagedObject->setMtu(NSM_MTU_DEF_VAL);
					pPhyIntfLocalManagedObject->setDestinationmirrorportEnable (false);
					updateWaveManagedObject(pPhyIntfLocalManagedObject);
					trace(TRACE_LEVEL_DEBUG, string("IntfConfLocalWorker::NsmLocaldestinationMirrorportMessageHandler Setting destination as mirrored port disalbe"));
				}
			}
        } 

		status = commitTransaction ();
		if (status != FRAMEWORK_SUCCESS) {
			trace (TRACE_LEVEL_ERROR, "IntfConfLocalWorker::NsmLocaldestinationMirrorportMessageHandler: Commit Failed");
			status = WAVE_MESSAGE_ERROR;
			rollbackTransaction();
		} else {
			status = WAVE_MESSAGE_SUCCESS;
		}
			
        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

        pNsmLocaldestinationMirrorportMessage->setCompletionStatus (status);
        reply(pNsmLocaldestinationMirrorportMessage);		  
		return;
    }

	ResourceId IntfConfLocalWorker::deletePBRLocal (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		IpPolicyDeletePBRMessage *pMsg    = dynamic_cast<IpPolicyDeletePBRMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		string ifName           = pMsg->getIfName();
		UI32 ifType             = pMsg->getIfType();
		int status                          = WAVE_MESSAGE_ERROR;
		//NsmLocalPBRManagedObject *pbrMO = NULL;
		set<string> compositionName;
		compositionName.insert("interfaceRouteMap");

		if(ifType == IF_TYPE_VE)
		{
			//NsmIntfSviManagedObject *pNsmIntfSviManagedObject   = NULL;
			//UI32 *vlanId = new UI32();
			UI32 vlanId = atoi(ifName.c_str());
			trace(TRACE_LEVEL_INFO, string("Inside IF_TYPE_VE "));
			WaveManagedObjectSynchronousQueryContextForDeletion *syncQueryCt =
								new WaveManagedObjectSynchronousQueryContextForDeletion(NsmIntfSviManagedObject::getClassName());
			syncQueryCt->addAndAttribute (new AttributeUI32(vlanId,"vlanId"));
			trace(TRACE_LEVEL_INFO, string("VLANID = ") + vlanId);
			deleteManagedObjectCompositions (syncQueryCt, compositionName);
			status = WAVE_MESSAGE_SUCCESS;

			/*vector<WaveManagedObject *> *pResults =
							querySynchronouslyLocalManagedObjectsForLocationId (FrameworkToolKit::getThisLocationId(), &syncQueryCtxt);
			if (NULL != pResults) {
				UI32 numberOfResults = pResults->size ();
				if (0 == numberOfResults) {
					status = WAVE_MESSAGE_ERROR;
				}else if (1 == numberOfResults) {
					trace(TRACE_LEVEL_INFO, string("Found Existing VE"));
					pNsmIntfSviManagedObject = dynamic_cast<NsmIntfSviManagedObject *>((*pResults)[0]);
					pNsmIntfSviManagedObject->setPBR(NULL);
					updateWaveManagedObject(pNsmIntfSviManagedObject);
					status = WAVE_MESSAGE_SUCCESS;
				} else {
					trace(TRACE_LEVEL_INFO, string("Two or more object Exists"));
					status = WAVE_MESSAGE_ERROR;
				}
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			} else {
				status = WAVE_MESSAGE_ERROR;
				trace(TRACE_LEVEL_INFO, string("PResults is NULL"));
			} */
		} else if ((ifType == IF_TYPE_GI) || (ifType == IF_TYPE_TE) || (ifType == IF_TYPE_FO))
		{
			//PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject   = NULL;

			//string *ifNameTemp = new string();
			//ifNameTemp->assign(pMsg->getIfName());
			trace(TRACE_LEVEL_INFO, string("Inside IF_TYPE_PHY "));
			//WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
			//syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
			//vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
			WaveManagedObjectSynchronousQueryContextForDeletion *syncQueryCt =
					new WaveManagedObjectSynchronousQueryContextForDeletion(PhyIntfLocalManagedObject::getClassName());
			syncQueryCt->addAndAttribute (new AttributeString(ifName,"ifName"));
			deleteManagedObjectCompositions (syncQueryCt, compositionName);
			//commitTransaction();
			trace(TRACE_LEVEL_INFO, string("IFNAME = ") + (ifName));
			status = WAVE_MESSAGE_SUCCESS;
/*
			if (NULL != pResults) {
				UI32 numberOfResults = pResults->size ();
				if (0 == numberOfResults) {
					status = WAVE_MESSAGE_ERROR;
				}else if (1 == numberOfResults) {
					trace(TRACE_LEVEL_INFO, string("Found Existing Interface"));
					pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
					if(pPhyIntfLocalManagedObject->getPBRMO() == NULL)
						trace(TRACE_LEVEL_INFO, string("PBRMO is NULL "));
					if(pPhyIntfLocalManagedObject->getPBRMO() != NULL) {
						pbrMO = pPhyIntfLocalManagedObject->getPBRMO();
						trace(TRACE_LEVEL_INFO, string("PBRMO is Not NULL "));
					}
					deleteWaveManagedObject(pbrMO->getObjectId());
					pPhyIntfLocalManagedObject->setPBR(NULL);
					//updateWaveManagedObject(pPhyIntfLocalManagedObject);
					status = WAVE_MESSAGE_SUCCESS;
				} else {
					trace(TRACE_LEVEL_INFO, string("Two or more object Exists"));
					status = WAVE_MESSAGE_ERROR;
				}
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			} else {
				status = WAVE_MESSAGE_ERROR;
				trace(TRACE_LEVEL_INFO, string("PResults is NULL"));
			}
*/
		}
		return status;
	}

	void IntfConfLocalWorker::NsmPBRDeleteMessageHandler (IpPolicyDeletePBRMessage *pMsg)
	{
		PrismSynchronousLinearSequencerStep sequencerSteps[] =
		{
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::deletePBRLocal),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
		};

		PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext
																  (pMsg, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
		pPrismSynchronousLinearSequencerContext->execute ();
	}

    void IntfConfLocalWorker::NsmPBRPoVlanIntfSPMessageHandler (PBRPoVlanIntfSPMessage *pMsg)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::sendPBRConfigPoIntf),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMsg, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IntfConfLocalWorker::sendPBRConfigPoIntf (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId status                               = WAVE_MESSAGE_SUCCESS;
        string ifname = "";
		vector<string> args;
        PBRPoVlanIntfSPMessage *pMsg    = dynamic_cast<PBRPoVlanIntfSPMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());                     char buf[6];

            string routeMapName;
			int op_code = pMsg->getCmdCode();
            if (pMsg->getCmdCode() == SSM_CLIENT_SET_ROUTEMAP_REMOVE_INTERFACE) {
                int ret;
                if (pMsg->getIfType() == IF_TYPE_PO) {
                    sprintf(buf, "%d", pMsg->getIfId());
                }

                ifname = buf;

                ret = getRouteMapNameForInterface(pMsg->getIfType(), ifname, routeMapName);
                if (ret == 0) {
                    trace(TRACE_LEVEL_DEVEL, string("sendPBRConfigPoIntf: polName: ")+ routeMapName);                    
                } else {
                    trace(TRACE_LEVEL_DEVEL, string("sendPBRConfigPoIntf: GetPolicyname failed" ));
                    return (WAVE_MESSAGE_ERROR);
                }

				if (pMsg->getIfType() == IF_TYPE_PO) {
				  sprintf(buf, "po%d", pMsg->getIfId());
				}
				ifname = buf;
				args.push_back(routeMapName);
				args.push_back(ifname);
				trace(TRACE_LEVEL_INFO, string("sendPBRConfigPoIntf: op_code ->")+ op_code);
				for(size_t idx = 0; idx < args.size(); idx++) {
				  trace(TRACE_LEVEL_INFO, string("sendPBRConfigPoIntf: args -> ")+ args[idx]);
				}
				GenericClientMessage_ client_msg(args, op_code);
				status = sendSynchronouslyToWaveClient ("ssm", &client_msg);
				if(status == WAVE_MESSAGE_SUCCESS) {
				  status = client_msg.getClientStatus();
				  tracePrintf(TRACE_LEVEL_INFO, "sendPBRConfigPoIntf: Success %lld",  status);
				} else {
				  tracePrintf(TRACE_LEVEL_INFO,  "sendPBRConfigPoIntf: Failed: %lld",  client_msg.getClientStatus());
				}
				return status;

            } else {

				if (pMsg->getIfType() == IF_TYPE_PO) {
				  sprintf(buf, "po%d", pMsg->getIfId());
				}
				ifname = buf;
				args.push_back(pMsg->getRouteMapName());
				args.push_back(ifname);
				trace(TRACE_LEVEL_INFO, string("sendPBRConfigPoIntf: op_code ->")+ op_code);
				for(size_t idx = 0; idx < args.size(); idx++) {
				  trace(TRACE_LEVEL_INFO, string("sendPBRConfigPoIntf: args -> ")+ args[idx]);
				}
				GenericClientMessage_ client_msg(args, op_code);
				status = sendSynchronouslyToWaveClient ("ssm", &client_msg);
				if(status == WAVE_MESSAGE_SUCCESS) {
				  status = client_msg.getClientStatus();
				  tracePrintf(TRACE_LEVEL_INFO, "sendPBRConfigPoIntf: Success %lld",  status);
				} else {
				  tracePrintf(TRACE_LEVEL_INFO,  "sendPBRConfigPoIntf: Failed: %lld",  client_msg.getClientStatus());
				}
				return status;

            }

        return (status);
    }


	/* Port Channel and Vlan Related Code */

    void IntfConfLocalWorker::NsmAssocIpAclPolicyPoVlanIntfSPMessageHandler (NsmAssocIpAclPolicyPoVlanIntfSPMessage *pMsg)
	{
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateIpAclConfigPoIntf),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMsg, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
	}

	ResourceId IntfConfLocalWorker::updateIpAclConfigPoIntf (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status 								= WAVE_MESSAGE_SUCCESS;
		char buffer[25];
		string ifname = "";
		NsmAssocIpAclPolicyPoVlanIntfSPMessage *pMsg 	= dynamic_cast<NsmAssocIpAclPolicyPoVlanIntfSPMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		const ssmAcl_binding_direction direction 		= pMsg->getDirection();

		if(direction == SSMACL_BINDING_DIRECTION_OUT_E) {
			if(!Capabilities::isCapabilityPresent(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS))
			{
				trace (TRACE_LEVEL_DEBUG, "Applying IP ACL policy in egress direction not supported on this platform.");
				return SSM_DCM_ERR_ACL_APPLIED_INTF_EGRESS;
			}
		}

		SsmIpAclPolicyMessage *cMsg 					= new SsmIpAclPolicyMessage();

		trace(TRACE_LEVEL_DEVEL, string("updateIpAclConfigPoIntf: polName: ") + pMsg->getAclName());

		if (cMsg) {

			string policyName;
			if (pMsg->getCmdCode() == SSMACL_DCMD_UNBIND_L3_IPV4_ACL) {
				char buf[6];
				int ret;
				if (pMsg->getIfType() == PO_INTF_TYPE) {
					sprintf(buf, "%d", pMsg->getIfId());
				}

				ifname = buf;

				ret = getIpAclPolicyNameForInterface(pMsg->getIfType(), ifname, policyName, direction);
				if (ret == 0) {
					trace(TRACE_LEVEL_DEVEL, string("updateIpAclConfigPoIntf: polName: ")+ policyName);
					cMsg->setPolicyName(policyName);
				} else {
					trace(TRACE_LEVEL_DEVEL, string("updateIpAclConfigPoIntf: GetPolicyname failed" ));
					return (WAVE_MESSAGE_ERROR);
				}

			} else {
				cMsg->setPolicyName(pMsg->getAclName());
			}

			if (pMsg->getIfType() == PO_INTF_TYPE) {
				sprintf(buffer, "po%d", pMsg->getIfId());
			}

			ifname = buffer;

			cMsg->setOpCode(pMsg->getCmdCode ());
			cMsg->setDirection(direction);
			cMsg->setIfname(ifname);
            /* Set Acl Type to differentiate between ip and mac acl */
            cMsg->setPolicyType(SSMACL_TYPE_L3_IPV4_E);

			status = sendSynchronouslyToWaveClient ("ssm", cMsg);
			if (status == WAVE_MESSAGE_SUCCESS) {
				status = cMsg->getClientStatus();
				trace(TRACE_LEVEL_INFO, string("updateIpAclConfigPoIntf: Success: ") + status);
				if ( status == WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS_MAC )
				{
					status = WRC_SSM_DCM_ERR_APPLY_MAC_ACL_ON_INTF;
				}
				pMsg->setCompletionStatus(status);
			} else {
				trace(TRACE_LEVEL_INFO, string("updateIpAclConfigPoIntf: Failed: ") + cMsg->getClientStatus());
				status = WAVE_MESSAGE_ERROR;
			}

			delete cMsg;
			cMsg = NULL;
		} else {
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "updateIpAclConfigPoIntf: Message creation failed");
		}
		return (status);
	}

	int IntfConfLocalWorker::getIpAclPolicyNameForInterface(const UI32 ifType, const string ifName, string &policyName, const ssmAcl_binding_direction direction)
	{
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject 	= NULL;
		PoIntfManagedObject *pPoIntfManagedObject				= NULL;
		NsmIntfSviManagedObject *pNsmIntfSviManagedObject		= NULL;

		WaveManagedObject   *pWaveManagedObject 				= NULL;
		IpAclPolicyManagedObject *policyMO 						= NULL;
		string pName;
		string className										= "";
		UI32 ifId 												=  0;
		int status 												= -1;

		trace (TRACE_LEVEL_INFO, string("Inside getIpAclPolicyNameForInterface Method"));

		if (ifType == PHY_INTF_TYPE) {
			className = PhyIntfLocalManagedObject::getClassName();
			pName = ifName;
		} else if (ifType == PO_INTF_TYPE) {
			className = PoIntfManagedObject::getClassName();
			ifId = strtoul(ifName.c_str (), NULL, 0);
		} else if (ifType == VE_INTF_TYPE) {
			className = NsmIntfSviManagedObject::getClassName();
			ifId = strtoul(ifName.c_str (), NULL, 0);
		} else  {
			return (status);
		}

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
		if (ifType == PHY_INTF_TYPE) {
			trace (TRACE_LEVEL_INFO, string("Query to Phy Interface"));
			syncQueryCtxt.addAndAttribute (new AttributeString(&pName,"id"));
		} else if (ifType == VE_INTF_TYPE) {
			syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifId,"vlanId"));
			syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
		} else if (ifType == PO_INTF_TYPE) {
			syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifId,"id"));
		}

		vector<WaveManagedObject *> *pResults = NULL;

		if (ifType == VE_INTF_TYPE) {
			pResults = querySynchronouslyLocalManagedObjectsForLocationId(FrameworkToolKit::getThisLocationId(), &syncQueryCtxt);
		} else {
			pResults = querySynchronously (&syncQueryCtxt);
		}

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (numberOfResults == 1) {
				pWaveManagedObject 			= NULL;
				pPhyIntfLocalManagedObject 	= NULL;
				pPoIntfManagedObject 		= NULL;
				pNsmIntfSviManagedObject 	= NULL;
				ObjectId policyObjId 		= ObjectId::NullObjectId;
				trace (TRACE_LEVEL_INFO, string(" Number of Results are 1 "));
				if (ifType == PHY_INTF_TYPE) {
					trace (TRACE_LEVEL_INFO, string("ifType == PHY_INTF_TYPE "));
					pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
					if (pPhyIntfLocalManagedObject) {
						(pPhyIntfLocalManagedObject->getIngressEgressIpAclPolicyObjectId (policyObjId, direction));
						trace (TRACE_LEVEL_INFO, string("Obtained policyObjId for Ingress Ip Acl for UNBIND"));
					}
					/* Fix Me Return with specific error */
				} else if (ifType == PO_INTF_TYPE) {
					pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
					if (pPoIntfManagedObject) {
						(pPoIntfManagedObject->getIngressEgressIpAclPolicyObjectId (policyObjId, direction));
						trace (TRACE_LEVEL_INFO, string("Obtained policyObjId for Ingress Ip Acl for UNBIND"));
					}
					/* Fix Me Return with specific error */
				} else if (ifType == VE_INTF_TYPE) {
					pNsmIntfSviManagedObject = dynamic_cast<NsmIntfSviManagedObject *>((*pResults)[0]);
					if (pNsmIntfSviManagedObject) {
						(pNsmIntfSviManagedObject->getIngressEgressIpAclPolicyObjectId (policyObjId, direction));
						trace (TRACE_LEVEL_INFO, string("Obtained policyObjId for Ingress Ip Acl for UNBIND"));
					}
				}

				if (policyObjId != ObjectId::NullObjectId) {
					pWaveManagedObject = queryManagedObject (policyObjId);
					if (pWaveManagedObject != NULL) {
						trace (TRACE_LEVEL_INFO, string ("ObjectId is not NULL"));
						policyMO 	= dynamic_cast<IpAclPolicyManagedObject *>(pWaveManagedObject);
						policyName 	= policyMO->getPolicyName();
						status 		= 0;
						delete pWaveManagedObject;
					}
				}
			}
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
		}

		return (status);
	}



    ResourceId IntfConfLocalWorker::NsmLocalFcoeVlanSendToDaemon(PrismSynchronousLinearSequencerContext *ctx_p)
    {
        NsmLocalFcoeVlanMessage     *loc_msg_p = NULL;
        NsmFcoeVlanMessage          *dmn_msg_p;
        ResourceId                  status = WAVE_MESSAGE_SUCCESS;

        trace (TRACE_LEVEL_INFO, "Recieved NSM LOCAL FCOE VLAN MESSAGE");

        loc_msg_p = dynamic_cast<NsmLocalFcoeVlanMessage *> (ctx_p->getPPrismMessage ());
        trace(TRACE_LEVEL_INFO, string("Sending to NSM Daemon Cmd=") + loc_msg_p->getCmdCode() +
                                string(" Old-Vlan=") + loc_msg_p->getOldVlan() +
                                string("New-Vlan=") + loc_msg_p->getNewVlan());

        dmn_msg_p = new NsmFcoeVlanMessage(loc_msg_p->getCmdCode(),
                                           loc_msg_p->getOldVlan(),
                                           loc_msg_p->getNewVlan(),
                                           loc_msg_p->getVlanName(),
                                           loc_msg_p->getVlanIfIndex(),
                                           loc_msg_p->getDesc());


        status = sendSynchronouslyToWaveClient ("nsm",dmn_msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_INFO, string("FCOE VLAN: Send To Daemon, Message-Status=[") +
                                    status + string("]:") +
                                    FrameworkToolKit::localize((status)));
        } else if (dmn_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_INFO, string("FCOE VLAN: Send To Daemon, Client-Status=[") +
                                    dmn_msg_p->getClientStatus() + string("]:") +
                                    FrameworkToolKit::localize(dmn_msg_p->getClientStatus()));
            status = dmn_msg_p->getClientStatus();
        }

        delete dmn_msg_p;

        return status;
    }

    ResourceId IntfConfLocalWorker::NsmLocalReservedVlanSendToDaemon(PrismSynchronousLinearSequencerContext *ctx_p)
    {
        ResourceId returnStatus = WAVE_MESSAGE_SUCCESS;
        
        trace (TRACE_LEVEL_INFO, "IntfConfLocalWorker::NsmLocalReservedVlanSendToDaemon Recieved NSM LOCAL RESERVED VLAN MESSAGE");

        NsmLocalReservedVlanMessage *loc_msg_p = dynamic_cast<NsmLocalReservedVlanMessage *> (ctx_p->getPPrismMessage ());
        prismAssert (NULL != loc_msg_p, __FILE__, __LINE__);
        
        trace(TRACE_LEVEL_INFO, string("Sending NsmClientReservedVlanMessage to NSM Daemon Cmd=") + loc_msg_p->getCmdCode() + string(" Start=") + loc_msg_p->getStart() + string(" End=") + loc_msg_p->getEnd());

        NsmClientReservedVlanMessage *dmn_msg_p = new NsmClientReservedVlanMessage(loc_msg_p->getCmdCode(), loc_msg_p->getStart(), loc_msg_p->getEnd()); 
        prismAssert (NULL != dmn_msg_p, __FILE__, __LINE__);

        ResourceId sendStatus = sendSynchronouslyToWaveClient ("nsm",dmn_msg_p);
        
        if (sendStatus != WAVE_MESSAGE_SUCCESS)
        {
            trace(TRACE_LEVEL_INFO, string("Reserved VLAN: Send To Daemon, Message-Status=") + FrameworkToolKit::localize((sendStatus)));

            returnStatus = sendStatus;
        }
        else if (0 != dmn_msg_p->getClientStatus())
        {
            trace(TRACE_LEVEL_ERROR, string("Reserved VLAN: Send To Daemon, Client-Status=[") + dmn_msg_p->getClientStatus() + string("]:") + FrameworkToolKit::localize(dmn_msg_p->getClientStatus()));
            
            returnStatus = dmn_msg_p->getClientStatus();
        }

        delete dmn_msg_p;

        return returnStatus;
    }

    void  IntfConfLocalWorker::NsmLocalReservedVlanMessageHandler(NsmLocalReservedVlanMessage *msg_p)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::NsmLocalReservedVlanSendToDaemon),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (msg_p, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }


    void  IntfConfLocalWorker::NsmLocalFcoeVlanMessageHandler( NsmLocalFcoeVlanMessage *msg_p)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::NsmLocalFcoeVlanSendToDaemon),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (msg_p, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId IntfConfLocalWorker::updateQosPhyIntfValidationStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {
        NsmLocalConfigQosAttributesMessage *pNsmLocalConfigQosAttributesMessage =
          dynamic_cast<NsmLocalConfigQosAttributesMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		//Validate whether Port channel has Qos Config
	    PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		string intf_name = pNsmLocalConfigQosAttributesMessage->getIfName();
		WaveManagedObjectSynchronousQueryContext phyLocSyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		phyLocSyncQueryCtxt.addAndAttribute (new AttributeString (&intf_name,"Id"));
		vector<WaveManagedObject *> *phyIntPResults = querySynchronously (&phyLocSyncQueryCtxt);

		if (phyIntPResults && phyIntPResults->size() == 1) {
			pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*phyIntPResults)[0]);
            
            //Save the pPhyIntfLocalManagedObject in DceSynchronousLinearSeqContext, which can be used in all sequencer steps.
            pPrismSynchronousLinearSequencerContext->setPWaveManagedObject(pPhyIntfLocalManagedObject);
			/*Check ISL Port or Not */
			if (pPhyIntfLocalManagedObject)
			{
            	if ( (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pPhyIntfLocalManagedObject->getPortrole())) {

                    //WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(phyIntPResults);
                    if ( pPrismSynchronousLinearSequencerContext && NULL != phyIntPResults ) {
                        pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*phyIntPResults);
                        phyIntPResults->clear();
                        delete phyIntPResults;
                    }
					if (pNsmLocalConfigQosAttributesMessage->getCmdCode() == QOS_INTERFACE_LONG_DISTANCE) {
						status = WAVE_MESSAGE_SUCCESS;
						pNsmLocalConfigQosAttributesMessage->setCompletionStatus(status);
						return (status);
					}
                    if(CONFIGURE_INTERFACE_QOS == pNsmLocalConfigQosAttributesMessage->getOpCode()) {
                        status = WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
                    }
                    pPrismSynchronousLinearSequencerContext->getCache()->put(PORT_ROLE,"true");
                    return status;
                }
			}

			if((pPhyIntfLocalManagedObject != NULL ) && (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)) {
				PoIntfManagedObject *pPoIntfManagedObject = NULL;
				//pPoIntfManagedObject = NsmUtils:: getPoIntfObjectByObjectId(pPhyIntfLocalManagedObject->getPoId());

                //Optimize the query:
                WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
                syncQueryCtxt.addAndAttribute(new AttributeUI32(pPhyIntfLocalManagedObject->getPoId().getClassId(), "objectidclassid") );
                syncQueryCtxt.addAndAttribute(new AttributeUI64(pPhyIntfLocalManagedObject->getPoId().getInstanceId(), "objectidinstanceid") );
                syncQueryCtxt.addSelectField("ceeMap");
                syncQueryCtxt.addSelectField("qosTrust");
                syncQueryCtxt.addSelectField("flowControlTx");
                syncQueryCtxt.addSelectField("flowControlRx");
                syncQueryCtxt.addSelectField("qosTrafficClassMap");
                syncQueryCtxt.addSelectField("qosCosMap");
                syncQueryCtxt.addSelectField("fcoeport");
                syncQueryCtxt.addSelectField("pfcFlowControl");
                syncQueryCtxt.addSelectField("qosDscpMap");
                syncQueryCtxt.addSelectField("qosDscpTrafficClassMap");
                syncQueryCtxt.addSelectField("qosDscpCosMap");
                syncQueryCtxt.addSelectField("qosDscpTrust");
                syncQueryCtxt.addSelectField("cosProfile");
                syncQueryCtxt.addSelectField("cosTailDrop");

                vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
                if (pResults && pResults->size()  == 1) {
                    pPoIntfManagedObject =  dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
                }

                if ( pResults ){
                    pResults->clear();
                    delete pResults;
                }

				 if (pPoIntfManagedObject != NULL)
                {
                    if (QosUtils::isInterfaceQosNonCeeConfigured(pPoIntfManagedObject)||
                     QosUtils::isInterfaceQosCeeConfigured(pPoIntfManagedObject)) {
					status = WRC_SSM_INTF_QOS_ON_LAG_IS_ALREADY_APPLIED;
                    }
                    else if (QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject))
                    {
                        status = WRC_SSM_INTF_FCOE_ON_LAG_IS_ALREADY_APPLIED;
                    }

                    if ( pPrismSynchronousLinearSequencerContext && NULL != phyIntPResults ) {
                        pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*phyIntPResults);
                        phyIntPResults->clear();
                        delete phyIntPResults;
                    }
					//WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(phyIntPResults);
					delete pPoIntfManagedObject;
                	pNsmLocalConfigQosAttributesMessage->setCompletionStatus(status);
                	return (status);
				}
			}
			//WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(phyIntPResults);
            if ( pPrismSynchronousLinearSequencerContext ) {
                pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*phyIntPResults);
                phyIntPResults->clear();
                delete phyIntPResults;
            }
		}

        /*if ( pPrismSynchronousLinearSequencerContext ) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);
        }*/

        switch (pNsmLocalConfigQosAttributesMessage->getOpCode())
        {
            case CONFIGURE_INTERFACE_QOS:
            {
                switch (pNsmLocalConfigQosAttributesMessage->getCmdCode())
                {
                    case QOS_INTERFACE_COS_MUTATION:
                    {
                        UI32 count = 0;
                        ResourceId  status2 = WAVE_MESSAGE_SUCCESS;
                        string mapName = pNsmLocalConfigQosAttributesMessage->getCosToCosMutationMapName();
                        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToCosMutationMapManagedObject::getClassName());
                        syncQueryCtxt.addAndAttribute (new AttributeString(&mapName,"cosToCosMutationMapName"));
                        status2 = querySynchronouslyForCount (&syncQueryCtxt, count);
                        if (0 == count) {
                            status2 = WRC_SSM_QOS_MAP_NOT_FOUND;
                        } else{
                            status2 = WAVE_MESSAGE_SUCCESS;
                        }
                        
                        pNsmLocalConfigQosAttributesMessage->setCompletionStatus(status2);
                        return (status2);
                    }
					case QOS_INTERFACE_COS_TRAFFIC_CLASS:
                    {
                        UI32 count = 0;
                        ResourceId  status3 = WAVE_MESSAGE_SUCCESS;
                        string mapName = pNsmLocalConfigQosAttributesMessage->getCosToTrafficClassMapName();
                        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToTrafficClassMapManagedObject::getClassName());
                        syncQueryCtxt.addAndAttribute (new AttributeString(&mapName,"cosToTrafficClassMapName"));
                        status3 = querySynchronouslyForCount (&syncQueryCtxt,count);
                        if (0 == count) {
                            status3 = WRC_SSM_QOS_MAP_NOT_FOUND;
                        } else {
                            status3 = WAVE_MESSAGE_SUCCESS;
                        }
                        
                        pNsmLocalConfigQosAttributesMessage->setCompletionStatus(status3);
                        return (status3);
                    }
                }
            }

        }

        pNsmLocalConfigQosAttributesMessage->setCompletionStatus(status);
        return (status);
    }

	ResourceId IntfConfLocalWorker::updateQosPhyIntfStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext) {

    NsmLocalConfigQosAttributesMessage *pNsmLocalConfigQosAttributesMessage = 
                dynamic_cast<NsmLocalConfigQosAttributesMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	trace (TRACE_LEVEL_DEBUG, string("Inside updateQosDaemonStep -------"));

    PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
    char buffer[MAX_STR];
    buffer[MAX_STR - 1] = '\0';
    UI32 bufferSize = sizeof (buffer);
    string ifName = pNsmLocalConfigQosAttributesMessage->getIfName();
    ResourceId  status   = WAVE_MESSAGE_SUCCESS;
    pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *> (pPrismSynchronousLinearSequencerContext->getPWaveManagedObject());
    
    if (pPhyIntfLocalManagedObject == NULL) {
        trace (TRACE_LEVEL_DEBUG, string("Inside updateQosDaemonStep ------- pPhyIntfLocalManagedObject is null"));
        return (WAVE_MESSAGE_ERROR);
    }

	//if policer is configured on this interface, flow control cannot be enabled
	if( (pNsmLocalConfigQosAttributesMessage->getOpCode() == CONFIGURE_INTERFACE_QOS)
        &&((pNsmLocalConfigQosAttributesMessage->getCmdCode() == QOS_INTERFACE_FLOW_CONTROL_TX) 
            || (pNsmLocalConfigQosAttributesMessage->getCmdCode() == QOS_INTERFACE_FLOW_CONTROL_RX) 
            || (pNsmLocalConfigQosAttributesMessage->getCmdCode() == QOS_INTERFACE_FLOW_CONTROL_BOTH)) 
        && ((pNsmLocalConfigQosAttributesMessage->getFlowControlTx() == 1)
            || (pNsmLocalConfigQosAttributesMessage->getFlowControlRx() == 1 ))) {

		if((pPhyIntfLocalManagedObject->getInputPolicyMapId() != ObjectId::NullObjectId)
            ||(pPhyIntfLocalManagedObject->getOutputPolicyMapId() != ObjectId::NullObjectId)) {
			 return (WRC_SSM_INTF_QOS_FLOW_CONTROL_ON_POLICED_PORT);
		}
	}

    QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
	trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getIfName()=") + pNsmLocalConfigQosAttributesMessage->getIfName());
	if (NsmUtils::isValidThreeTuple(pNsmLocalConfigQosAttributesMessage->getIfName()))
   	{
        NsmUtils::getTwoTupleIfName(pNsmLocalConfigQosAttributesMessage->getIfName(), ifName);
    }

    GENERATE_SHORT_IF_NAME_FOR_BACK_END(pNsmLocalConfigQosAttributesMessage->getIfType(), ifName.c_str(), buffer, bufferSize);
    ifName = buffer;
	//ifName.insert(0, "te");
	trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage-ifName()=") + ifName);

	m->setIfName(ifName);
	//m->setIfName(pNsmLocalConfigQosAttributesMessage->getIfName());
    m->setOpCode(pNsmLocalConfigQosAttributesMessage->getOpCode());
    m->setCmdCode(pNsmLocalConfigQosAttributesMessage->getCmdCode());

	trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getOpCode()=") + pNsmLocalConfigQosAttributesMessage->getOpCode());
	trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getCmdCode()=") + pNsmLocalConfigQosAttributesMessage->getCmdCode());
	switch (pNsmLocalConfigQosAttributesMessage->getOpCode())
	{
	  case CONFIGURE_INTERFACE_QOS:
		{
		  switch (pNsmLocalConfigQosAttributesMessage->getCmdCode())
		  {
			case QOS_INTERFACE_FLOW_CONTROL_TX:
				m->setFlowControlTx(pNsmLocalConfigQosAttributesMessage->getFlowControlTx());
				trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getFlowControlTx()=") + pNsmLocalConfigQosAttributesMessage->getFlowControlTx());
				break;
			case QOS_INTERFACE_FLOW_CONTROL_RX:
				m->setFlowControlRx(pNsmLocalConfigQosAttributesMessage->getFlowControlRx());
				trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getFlowControlRx()=") + pNsmLocalConfigQosAttributesMessage->getFlowControlRx());
				break;

			case QOS_INTERFACE_FLOW_CONTROL_BOTH:
			{
				m->setFlowControlTx(pNsmLocalConfigQosAttributesMessage->getFlowControlTx());
				m->setFlowControlRx(pNsmLocalConfigQosAttributesMessage->getFlowControlRx());
				trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getFlowControlTx()=") + pNsmLocalConfigQosAttributesMessage->getFlowControlTx());
				trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getFlowControlRx()=") + pNsmLocalConfigQosAttributesMessage->getFlowControlRx());
				break;
			}
            
            //The below 2 defects contradict each other.
            //TR000407684 [FC: Change in behavior of "default CoS" from Hercules+ to Hydra.] => Added below case to fix thos.
            //and TR000454577 FC: PFC enable should be allowed on default-CoS to support end-to-end legacy pause.  => Commenting the below conde, to fix this.

			case QOS_INTERFACE_DEFAULT_COS: {

                //status = QosUtils::vcsQosSanity(QOS_INTERFACE_DEFAULT_COS, "", true, 0, 0, pNsmLocalConfigQosAttributesMessage->getDefaultCosValue());

                //if(WAVE_MESSAGE_SUCCESS == status) {
                    m->setDefaultCosValue(pNsmLocalConfigQosAttributesMessage->getDefaultCosValue());
                    trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getDefaultCosValue()=") 
                        + pNsmLocalConfigQosAttributesMessage->getDefaultCosValue());
                //}
				break;
			}
			case QOS_INTERFACE_TRUST_COS:
			{
				trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getQosTrust()") );
				break;
			}
			case QOS_INTERFACE_TRUST_DSCP:
            {
                trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getQosDscpTrust()") );
                break;
            }
			case QOS_INTERFACE_COS_MUTATION:
			{
	            if ((status = QosUtils::vcsQosSanity(QOS_INTERFACE_COS_MUTATION, pNsmLocalConfigQosAttributesMessage->getCosToCosMutationMapName(),true,0,0,0)) == WAVE_MESSAGE_SUCCESS) {
				trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getCosToCosMutationMapName()=") + pNsmLocalConfigQosAttributesMessage->getCosToCosMutationMapName());
				m->setCosToCosMutationMapName(pNsmLocalConfigQosAttributesMessage->getCosToCosMutationMapName());
           	    }
			  break;
			}
			case QOS_INTERFACE_COS_TRAFFIC_CLASS:
			{
				trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getCosToTrafficClassMapName()=") + pNsmLocalConfigQosAttributesMessage->getCosToTrafficClassMapName());
				m->setCosToTrafficClassMapName(pNsmLocalConfigQosAttributesMessage->getCosToTrafficClassMapName());
			  break;
			}
			case QOS_INTERFACE_DSCP_MUTATION:
            {
				ObjectId dscpMapObjectId;
                trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getDscpToDscpMutationMapName()=") + pNsmLocalConfigQosAttributesMessage->getDscpToDscpMutationMapName());
                ResourceId mapPresent = WAVE_MESSAGE_ERROR;
				mapPresent = getDscpMapObjectIdByDscpMapName(pNsmLocalConfigQosAttributesMessage->getDscpToDscpMutationMapName(), dscpMapObjectId, pPrismSynchronousLinearSequencerContext);
				if(mapPresent != WAVE_MESSAGE_SUCCESS) {
                    if(NULL != m) {
                        delete m;
                        m = NULL;
                    }
					return mapPresent;
				}
				else
				{
					m->setDscpToDscpMutationMapName(pNsmLocalConfigQosAttributesMessage->getDscpToDscpMutationMapName());
				}
              	break;
            }
            case QOS_INTERFACE_DSCP_TRAFFIC_CLASS:
            {
				ObjectId dscpMapObjectId;
                trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getDscpToTrafficClassMapName()=") + pNsmLocalConfigQosAttributesMessage->getDscpToTrafficClassMapName());
                ResourceId mapPresent = WAVE_MESSAGE_ERROR;
                mapPresent = getDscpTcMapObjectIdByTcMapName(pNsmLocalConfigQosAttributesMessage->getDscpToTrafficClassMapName(), dscpMapObjectId, pPrismSynchronousLinearSequencerContext);
                if(mapPresent != WAVE_MESSAGE_SUCCESS) {
                    if(NULL != m) {
                        delete m;
                        m = NULL;
                    }
                    return mapPresent;
                } else {
					m->setDscpToTrafficClassMapName(pNsmLocalConfigQosAttributesMessage->getDscpToTrafficClassMapName());
              	}
				break;
            }
			case QOS_INTERFACE_DSCP_COS:
			{
				ObjectId dscpMapObjectId;
				trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getDscpToCosMapName()=") + pNsmLocalConfigQosAttributesMessage->getDscpToCosMapName());
                ResourceId mapPresent = WAVE_MESSAGE_ERROR;
                mapPresent = getDscpCosMapObjectIdByDscpCosMapName(pNsmLocalConfigQosAttributesMessage->getDscpToCosMapName(), dscpMapObjectId, pPrismSynchronousLinearSequencerContext);
                if(mapPresent != WAVE_MESSAGE_SUCCESS) {
                    if(NULL != m) {
                        delete m;
                        m = NULL;
                    }
                    return mapPresent;
                }
                else
                {
					m->setDscpToCosMapName(pNsmLocalConfigQosAttributesMessage->getDscpToCosMapName());
                }
				break;
            }
			case QOS_INTERFACE_CEE_MAP:
			{
#if 0			/* No need to check when cee applied on interface, because CEE always on ISL port */
				if ((status = vcsQosSanity(QOS_INTERFACE_CEE_MAP, pNsmLocalConfigQosAttributesMessage->getCeeMapName(), 0, true, pPhyIntfLocalManagedObject, 0)) != WAVE_MESSAGE_SUCCESS) {
					delete (m);
                    pNsmLocalConfigQosAttributesMessage->setCompletionStatus(status);
					return (status);
				}
#endif
                trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getCeeMapName()=") + pNsmLocalConfigQosAttributesMessage->getCeeMapName());
                m->setCeeMapName(pNsmLocalConfigQosAttributesMessage->getCeeMapName());
                break;
			}
			case QOS_INTERFACE_LONG_DISTANCE:
			{
                trace (TRACE_LEVEL_DEBUG, string("pNsmLocalConfigQosAttributesMessage->getDistance()=") + pNsmLocalConfigQosAttributesMessage->getDistance());
                m->setDistance(pNsmLocalConfigQosAttributesMessage->getDistance());
                break;
			}

            case QOS_INTERFACE_PRIORITY_TAG:
            {
                trace (TRACE_LEVEL_INFO, string("IntfConfLocalWorker:: set Priority Tagging") );
                break;
			}
		}
			break;
		}

	  case DELETE_INTERFACE_QOS:
		{
            if(0 == (string("true").compare(pPrismSynchronousLinearSequencerContext->getCache()->getStringValue(PORT_ROLE)))) {
                status = WAVE_MESSAGE_SUCCESS;
                if(NULL != m) {
                    delete m;
                    m = NULL;
                }
                pNsmLocalConfigQosAttributesMessage->setCompletionStatus(status);
                return status;
            }
			switch (pNsmLocalConfigQosAttributesMessage->getCmdCode())
			{
				case QOS_INTERFACE_COS_MUTATION:
				{
					break;
				}
                //The below 2 defects contradict each other.
                //TR000407684 [FC: Change in behavior of "default CoS" from Hercules+ to Hydra.] => Added below case to fix thos.
                //and TR000454577 FC: PFC enable should be allowed on default-CoS to support end-to-end legacy pause.  => Commenting the below conde, to fix this.

				/*case QOS_INTERFACE_DEFAULT_COS: {
                    status = QosUtils::vcsQosSanity(QOS_INTERFACE_DEFAULT_COS, "", true, 0, 0, 0);
                    break;
				}*/
				default:
					break;
			}
		   /* Nothing to be done. cmdCode and opCode will convey the info */
			trace (TRACE_LEVEL_DEBUG, string("DELETE_INTERFACE_QOS"));
			break;
		}
	}

    if (status != WAVE_MESSAGE_SUCCESS) {
        if (NULL != m) {
            delete m;
            m = NULL;
        }
		pNsmLocalConfigQosAttributesMessage->setCompletionStatus(status);
        return status;
    }

    //Fix for the defect TR000377133
         if ( ( pNsmLocalConfigQosAttributesMessage->getOpCode() == CONFIGURE_INTERFACE_QOS || pNsmLocalConfigQosAttributesMessage->getOpCode() == DELETE_INTERFACE_QOS ) && ( pNsmLocalConfigQosAttributesMessage->getCmdCode() == QOS_INTERFACE_PRIORITY_TAG)  ) {
             trace (TRACE_LEVEL_DEBUG, "sending synchronously to nsm client");
             status = sendSynchronouslyToWaveClient ("nsm",m);
        } else {
            trace (TRACE_LEVEL_DEBUG, "sending synchronously to ssm client");
            status = sendSynchronouslyToWaveClient ("qos",m);
        }

	trace (TRACE_LEVEL_INFO, string("sendSynchronouslyToWaveClient  ***** status= ") + status);
	pNsmLocalConfigQosAttributesMessage->setCompletionStatus(status);

	if( status == WAVE_MESSAGE_SUCCESS)
   	{
		if( m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
		{
			status = m->getCompletionStatus();
			trace(TRACE_LEVEL_DEBUG, string("Completion status=") + status);
		}
    		else if ( m->getClientStatus() != WAVE_MESSAGE_SUCCESS )
		{
			status = m->getClientStatus();
			trace(TRACE_LEVEL_DEBUG, string("Client status=") + status);
		}
		pNsmLocalConfigQosAttributesMessage->setCompletionStatus(status);
	}
        //bpatel: To avoid memory leak
    if(NULL != m){
        delete m;
        m = NULL;
    }
	return status;
}
	ResourceId IntfConfLocalWorker::updatePhyIntQosMOStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
  {
	trace (TRACE_LEVEL_DEBUG, string("Inside updatePhyIntQosMOStep-------"));
	NsmLocalConfigQosAttributesMessage *pNsmLocalConfigQosAttributesMessage = dynamic_cast<NsmLocalConfigQosAttributesMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	trace (TRACE_LEVEL_DEBUG, string("DB pNsmLocalConfigQosAttributesMessage->getOpCode()=") + pNsmLocalConfigQosAttributesMessage->getOpCode());
	trace (TRACE_LEVEL_DEBUG, string("DB pNsmLocalConfigQosAttributesMessage->getCmdCode()=") + pNsmLocalConfigQosAttributesMessage->getCmdCode());
	PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
    //string ifName = pNsmLocalConfigQosAttributesMessage->getIfName();
    //ResourceId  status   = WAVE_MESSAGE_SUCCESS;
    /*WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifname"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
				trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
                status = WAVE_MESSAGE_ERROR;
            }else if (1 == numberOfResults) {
				trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
				cout << string("pPhyIntfLocalManagedObject ifname=") + pPhyIntfLocalManagedObject->getIfName();
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
        }*/
        
        pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *> (pPrismSynchronousLinearSequencerContext->getPWaveManagedObject());

        if (pPhyIntfLocalManagedObject == NULL) {
            trace (TRACE_LEVEL_DEBUG, string("Inside updatePhyIntQosMOStep------- pPhyIntfLocalManagedObject is NULL "));
            return (WAVE_MESSAGE_ERROR);
        }


 		updateWaveManagedObject (pPhyIntfLocalManagedObject);
        prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);
        switch(pNsmLocalConfigQosAttributesMessage->getOpCode())
        {
			case CONFIGURE_INTERFACE_QOS:
					trace (TRACE_LEVEL_DEBUG, string("DB UPDATE CONFIGURE_INTERFACE_QOS-------"));
				switch (pNsmLocalConfigQosAttributesMessage->getCmdCode()) {
					case QOS_INTERFACE_FLOW_CONTROL_TX:
						trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_FLOW_CONTROL_TX-------"));
						pPhyIntfLocalManagedObject->setFlowControlTx(pNsmLocalConfigQosAttributesMessage->getFlowControlTx());
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("flowControlTx");
					break;
					case QOS_INTERFACE_FLOW_CONTROL_RX:
						trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_FLOW_CONTROL_RX-------"));
						pPhyIntfLocalManagedObject->setFlowControlRx(pNsmLocalConfigQosAttributesMessage->getFlowControlRx());
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("flowControlRx");
					break;
					case QOS_INTERFACE_FLOW_CONTROL_BOTH:
						trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_FLOW_CONTROL_BOTH-------"));
						pPhyIntfLocalManagedObject->setFlowControlTx(pNsmLocalConfigQosAttributesMessage->getFlowControlTx());
						pPhyIntfLocalManagedObject->setFlowControlRx(pNsmLocalConfigQosAttributesMessage->getFlowControlRx());
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("flowControlTx");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("flowControlRx");
					break;
					case QOS_INTERFACE_DEFAULT_COS:
						trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_DEFAULT_COS-------=") + pNsmLocalConfigQosAttributesMessage->getDefaultCosValue());
						pPhyIntfLocalManagedObject->setDefaultCos(pNsmLocalConfigQosAttributesMessage->getDefaultCosValue());
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("defaultCos");
						break;
					case QOS_INTERFACE_TRUST_COS:
						trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_TRUST_COS-------="));
						pPhyIntfLocalManagedObject->setQosTrust(true);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosTrust");
						break;
					case QOS_INTERFACE_TRUST_DSCP:
                        trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_TRUST_DSCP-------="));
                        pPhyIntfLocalManagedObject->setQosDscpTrust(true);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosDscpTrust");
                        break;
					case QOS_INTERFACE_COS_MUTATION:
						{
						trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_COS_MUTATION-------="));
						string cosName = pNsmLocalConfigQosAttributesMessage->getCosToCosMutationMapName();
						ObjectId cosMapObjectId;
						getCosMapObjectIdByCosMapName(cosName, cosMapObjectId, pPrismSynchronousLinearSequencerContext);
						pPhyIntfLocalManagedObject->setQosCosMap(cosMapObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosCosMap");
						break;
						}
					case QOS_INTERFACE_COS_TRAFFIC_CLASS:
						{
						trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_COS_TRAFFIC_CLASS-------="));
						string cosName = pNsmLocalConfigQosAttributesMessage->getCosToTrafficClassMapName();
						ObjectId cosTcObjectId;
						getTcMapObjectIdByTcMapName(cosName, cosTcObjectId, pPrismSynchronousLinearSequencerContext);
						pPhyIntfLocalManagedObject->setQosTrafficClassMap(cosTcObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosTrafficClassMap");
						break;
						}
				    case QOS_INTERFACE_DSCP_MUTATION:
                        {
                        trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_DSCP_MUTATION-------="));
                        string dscpName = pNsmLocalConfigQosAttributesMessage->getDscpToDscpMutationMapName();
                        ObjectId dscpMapObjectId;
                        getDscpMapObjectIdByDscpMapName(dscpName, dscpMapObjectId, pPrismSynchronousLinearSequencerContext);
						pPhyIntfLocalManagedObject->setQosDscpMap(dscpMapObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosDscpMap");
                        break;
                        }
                    case QOS_INTERFACE_DSCP_TRAFFIC_CLASS:
                        {
                        trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_DSCP_TRAFFIC_CLASS-------="));
                        string dscpName = pNsmLocalConfigQosAttributesMessage->getDscpToTrafficClassMapName();
                        ObjectId dscpTcObjectId;
                        getDscpTcMapObjectIdByTcMapName(dscpName, dscpTcObjectId, pPrismSynchronousLinearSequencerContext);
                        pPhyIntfLocalManagedObject->setQosDscpTrafficClassMap(dscpTcObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosDscpTrafficClassMap");
                        break;
                        }
					case QOS_INTERFACE_DSCP_COS:
                        {
                        trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_DSCP_COS-------="));
                        string dscpName = pNsmLocalConfigQosAttributesMessage->getDscpToCosMapName();
                        ObjectId dscpCosObjectId;
						getDscpCosMapObjectIdByDscpCosMapName(dscpName, dscpCosObjectId, pPrismSynchronousLinearSequencerContext);
						pPhyIntfLocalManagedObject->setQosDscpCosMap(dscpCosObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosDscpCosMap");
						break;
                        }
                    case QOS_INTERFACE_CEE_MAP:
                        {
                        trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_CEE_MAP-------="));
                        string ceeName = pNsmLocalConfigQosAttributesMessage->getCeeMapName();
                        ObjectId ceeObjectId;
                        getCeeMapObjectIdByCeeMapName(ceeName, ceeObjectId, pPrismSynchronousLinearSequencerContext);

                        vector<string> listOfAttributes;
                        pPhyIntfLocalManagedObject->setCeeMap(ceeObjectId);
						//remove non-CEE config
						pPhyIntfLocalManagedObject->setQosTrafficClassMap(ObjectId::NullObjectId);
						pPhyIntfLocalManagedObject->setQosCosMap(ObjectId::NullObjectId);
						pPhyIntfLocalManagedObject->setQosDscpMap(ObjectId::NullObjectId);
                        pPhyIntfLocalManagedObject->setQosDscpCosMap(ObjectId::NullObjectId);
						pPhyIntfLocalManagedObject->setQosDscpTrafficClassMap(ObjectId::NullObjectId);
        				pPhyIntfLocalManagedObject->setFlowControlRx(QOS_DEFAULT_FLOWCONTROL);
				        pPhyIntfLocalManagedObject->setFlowControlTx(QOS_DEFAULT_FLOWCONTROL);
			            pPhyIntfLocalManagedObject->setQosTrust(QOS_DEFAULT_QOS_TRUST);
						pPhyIntfLocalManagedObject->setQosDscpTrust(QOS_DEFAULT_QOS_TRUST);
						pPhyIntfLocalManagedObject->deletePfcAll();
						pPhyIntfLocalManagedObject->deleteCosProfileAll();
						pPhyIntfLocalManagedObject->deleteCosTailDropAll();


                        listOfAttributes.push_back("ceeMap");
                        listOfAttributes.push_back("qosTrafficClassMap");
                        listOfAttributes.push_back("qosCosMap");
                        listOfAttributes.push_back("qosDscpMap");
                        listOfAttributes.push_back("qosDscpCosMap");
                        listOfAttributes.push_back("qosDscpTrafficClassMap");
                        listOfAttributes.push_back("flowControlRx");
                        listOfAttributes.push_back("flowControlTx");
                        listOfAttributes.push_back("qosTrust");
                        listOfAttributes.push_back("qosDscpTrust");
                        listOfAttributes.push_back("pfcFlowControl");
                        listOfAttributes.push_back("cosProfile");
                        listOfAttributes.push_back("cosTailDrop");

                        pPhyIntfLocalManagedObject->setAttributesToBeUpdated(listOfAttributes);
                        break;
                        }
					case QOS_INTERFACE_LONG_DISTANCE:
						{
                        trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_LONG_DISTANCE-------="));
						pPhyIntfLocalManagedObject->setDistance(pNsmLocalConfigQosAttributesMessage->getDistance());
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("Distance");
						break;
						}
                    case QOS_INTERFACE_PRIORITY_TAG:
                        trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_PRIORITY_TAG-------="));
                        pPhyIntfLocalManagedObject->setPriorityTag(true);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("priorityTag");
                        break;
				}
				break;
			case DELETE_INTERFACE_QOS:
					{
					trace (TRACE_LEVEL_DEBUG, string("DB UPDATE DELETE_INTERFACE_QOS-------"));
					switch (pNsmLocalConfigQosAttributesMessage->getCmdCode()) {
					  case QOS_INTERFACE_FLOW_CONTROL_BOTH:
						pPhyIntfLocalManagedObject->setFlowControlTx(3);
						pPhyIntfLocalManagedObject->setFlowControlRx(3);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("flowControlTx");
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("flowControlRx");

						break;
						case QOS_INTERFACE_DEFAULT_COS:
						pPhyIntfLocalManagedObject->setDefaultCos(0);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("defaultCos");
						break;
						case QOS_INTERFACE_TRUST_COS:
						pPhyIntfLocalManagedObject->setQosTrust(false);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosTrust");
						break;
						case QOS_INTERFACE_TRUST_DSCP:
                        pPhyIntfLocalManagedObject->setQosDscpTrust(false);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosDscpTrust");
                        break;
						case QOS_INTERFACE_COS_MUTATION:
						trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_COS_MUTATION-------="));
						pPhyIntfLocalManagedObject->setQosCosMap(ObjectId::NullObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosCosMap");
						break;
						case QOS_INTERFACE_COS_TRAFFIC_CLASS:
						trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_COS_TRAFFIC_CLASS-------="));
						pPhyIntfLocalManagedObject->setQosTrafficClassMap(ObjectId::NullObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosTrafficClassMap");
						break;
						case QOS_INTERFACE_DSCP_MUTATION:
                        trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_DSCP_MUTATION-------="));
                        pPhyIntfLocalManagedObject->setQosDscpMap(ObjectId::NullObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosDscpMap");
                        break;
                        case QOS_INTERFACE_DSCP_TRAFFIC_CLASS:
                        trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_DSCP_TRAFFIC_CLASS-------="));
                        pPhyIntfLocalManagedObject->setQosDscpTrafficClassMap(ObjectId::NullObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosDscpTrafficClassMap");
                        break;
						case QOS_INTERFACE_DSCP_COS:
                        trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_DSCP_TRAFFIC_CLASS-------="));
                        pPhyIntfLocalManagedObject->setQosDscpCosMap(ObjectId::NullObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("qosDscpCosMap");
                        break;
						case QOS_INTERFACE_CEE_MAP:
                        trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_CEE_MAP-------="));
                        pPhyIntfLocalManagedObject->setCeeMap(ObjectId::NullObjectId);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("ceeMap");
                        break;
						case QOS_INTERFACE_LONG_DISTANCE:
                        trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_LONG_DISTANCE-------="));
						pPhyIntfLocalManagedObject->setDistance(LONG_DISTANCE_200);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("Distance");
						break;
	                    case QOS_INTERFACE_PRIORITY_TAG:
    	                trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_PRIORITY_TAG-------="));
        	            pPhyIntfLocalManagedObject->setPriorityTag(false);
                        pPhyIntfLocalManagedObject->addAttributeToBeUpdated("priorityTag");
            	        break;
					}
					break;
					}
		}
//		updateWaveManagedObject (pPhyIntfLocalManagedObject);

		trace (TRACE_LEVEL_DEBUG, string("DB ***** completionstatus = ") + pNsmLocalConfigQosAttributesMessage->getCompletionStatus());

        return pNsmLocalConfigQosAttributesMessage->getCompletionStatus ();
  }


	void  IntfConfLocalWorker::NsmLocalConfigQosAttributesMessageHandler( NsmLocalConfigQosAttributesMessage *pNsmLocalConfigQosAttributesMessage)
  {

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateQosPhyIntfValidationStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateQosPhyIntfStep),
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updatePhyIntQosMOStep),
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
    };

    DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pNsmLocalConfigQosAttributesMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pPrismSynchronousLinearSequencerContext->execute ();
  }



	ResourceId IntfConfLocalWorker::updateQosPoIntfStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
    NsmQosPOConfigSPMessage *pNsmQosPOConfigSPMessage = dynamic_cast<NsmQosPOConfigSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	trace (TRACE_LEVEL_INFO, string("-------Inside updateQosPoIntfStep -------"));

    ResourceId  status   = WAVE_MESSAGE_ERROR;
    SI32 cmdCode = pNsmQosPOConfigSPMessage->getCmdCode();


    if ((FB_POLICYMAP_BIND_INTERFACE_IN == cmdCode)
			|| (FB_POLICYMAP_BIND_INTERFACE_OUT == cmdCode)
			|| (FB_POLICYMAP_UNBIND_INTERFACE_IN == cmdCode)
			|| (FB_POLICYMAP_UNBIND_INTERFACE_OUT == cmdCode)) {

    //Check whether the node supports MQC, if not, return the error
    if(!Capabilities::instance()->isCapabilityPresent(FEAT_CAPABILITY_POLICER)) {
        status = WRC_SSM_POL_ERR_NOT_SUPPORTED_PLATFORM;
        pNsmQosPOConfigSPMessage->setCompletionStatus(status);
        return status;
    }

    status = WAVE_MESSAGE_ERROR;
    PolicerClientTeMessage *m = new PolicerClientTeMessage();
	stringstream poName;

	trace(
			TRACE_LEVEL_DEBUG,
			string("pNsmQosPOConfigSPMessage->getPoId()=")
					+ pNsmQosPOConfigSPMessage->getPoId());
	poName << pNsmQosPOConfigSPMessage->getPoId();
	string ifName = poName.str();
	ifName.insert(0, "po");
	trace(TRACE_LEVEL_DEBUG,
			string("pNsmQosPOConfigSPMessage-ifName()=") + ifName);

	m->setIfName(ifName);
	m->setOpCode(pNsmQosPOConfigSPMessage->getOpCode());
	m->setCmdCode(pNsmQosPOConfigSPMessage->getCmdCode());
	m->setMapName(pNsmQosPOConfigSPMessage->getQosPolicyMapName());

	trace(TRACE_LEVEL_DEVEL, "sending synchronously to ssm client");
	trace(
				TRACE_LEVEL_DEBUG,
				string("sendSynchronouslyTo qos daemon sendToBackEndNsmQoSPolicyConfigLocalMessageStep OPCODE<")
						+ m->getOpCode() + "> CMDCODE<" + m->getCmdCode() + "> POID<"+ m->getIfName() +
						"> MAPNAME<" + m->getMapName() + ">") ;
	status = sendSynchronouslyToWaveClient("qos", m);

	trace(TRACE_LEVEL_DEBUG,
			string("sendSynchronouslyToWaveClient  ***** status= ") + status);
	pNsmQosPOConfigSPMessage->setCompletionStatus(status);

	if (status == WAVE_MESSAGE_SUCCESS) {
		if (m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
			status = m->getCompletionStatus();
			trace(TRACE_LEVEL_DEBUG, string("Completion status=") + status);
			pNsmQosPOConfigSPMessage->setCompletionStatus(status);
		} else {
			status = m->getClientStatus();
			trace(TRACE_LEVEL_DEBUG, string("Client status=") + status);
			pNsmQosPOConfigSPMessage->setCompletionStatus(status);
		}
	}
	if (m) {
		delete m;
	}
	return status;
} else {
    QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
	stringstream poName;

	trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage->getPoId()=") + pNsmQosPOConfigSPMessage->getPoId());
	poName << pNsmQosPOConfigSPMessage->getPoId();
	string ifName = poName.str();
	ifName.insert(0, "po");
	trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage-ifName()=") + ifName);

	m->setIfName(ifName);
    m->setOpCode(pNsmQosPOConfigSPMessage->getOpCode());
    m->setCmdCode(pNsmQosPOConfigSPMessage->getCmdCode());

	trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage->getOpCode()=") + pNsmQosPOConfigSPMessage->getOpCode());
	trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage->getCmdCode()=") + pNsmQosPOConfigSPMessage->getCmdCode());
	switch (pNsmQosPOConfigSPMessage->getOpCode())
	{
	  case CONFIGURE_INTERFACE_QOS:
		{
		  switch (pNsmQosPOConfigSPMessage->getCmdCode())
		  {
			case QOS_INTERFACE_FLOW_CONTROL_TX:
				m->setFlowControlTx(pNsmQosPOConfigSPMessage->getFlowControlTx());
				trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage->getFlowControlTx()=") + pNsmQosPOConfigSPMessage->getFlowControlTx());
				break;
			case QOS_INTERFACE_FLOW_CONTROL_RX:
				m->setFlowControlRx(pNsmQosPOConfigSPMessage->getFlowControlRx());
				trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage->getFlowControlRx()=") + pNsmQosPOConfigSPMessage->getFlowControlRx());
				break;

			case QOS_INTERFACE_FLOW_CONTROL_BOTH:
			{
				m->setFlowControlTx(pNsmQosPOConfigSPMessage->getFlowControlTx());
				m->setFlowControlRx(pNsmQosPOConfigSPMessage->getFlowControlRx());
				trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage->getFlowControlTx()=") + pNsmQosPOConfigSPMessage->getFlowControlTx());
				trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage->getFlowControlRx()=") + pNsmQosPOConfigSPMessage->getFlowControlRx());
				break;
			}

			case QOS_INTERFACE_DEFAULT_COS:
			{
				m->setDefaultCosValue(pNsmQosPOConfigSPMessage->getDefaultCosValue());
				break;
			}
			case QOS_INTERFACE_TRUST_COS:
			{
				m->setQosTrust(pNsmQosPOConfigSPMessage->getQosTrust());
				break;
			}
			case QOS_INTERFACE_TRUST_DSCP:
            {
                m->setQosDscpTrust(pNsmQosPOConfigSPMessage->getQosDscpTrust());
                break;
            }
			case QOS_INTERFACE_COS_MUTATION:
            {
                trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage->getCosToCosMutationMapName()=") + pNsmQosPOConfigSPMessage->getCosToCosMutationMapName());
                m->setCosToCosMutationMapName(pNsmQosPOConfigSPMessage->getCosToCosMutationMapName());
              break;
            }
            case QOS_INTERFACE_COS_TRAFFIC_CLASS:
            {
                trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage->getCosToTrafficClassMapName()=") + pNsmQosPOConfigSPMessage->getCosToTrafficClassMapName());
                m->setCosToTrafficClassMapName(pNsmQosPOConfigSPMessage->getCosToTrafficClassMapName());
              break;
            }
			case QOS_INTERFACE_DSCP_MUTATION: {

				ObjectId dscpMapObjectId;
                trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage->getDscpToDscpMutationMapName()=") + pNsmQosPOConfigSPMessage->getDscpToDscpMutationMapName());
                ResourceId mapPresent = WAVE_MESSAGE_ERROR;
                mapPresent = getDscpMapObjectIdByDscpMapName(pNsmQosPOConfigSPMessage->getDscpToDscpMutationMapName(), dscpMapObjectId, pPrismSynchronousLinearSequencerContext);
                if(mapPresent != WAVE_MESSAGE_SUCCESS)
                {
                    return mapPresent;
                }
                else
                {
					m->setDscpToDscpMutationMapName(pNsmQosPOConfigSPMessage->getDscpToDscpMutationMapName());
              	}
				break;
            }
            case QOS_INTERFACE_DSCP_TRAFFIC_CLASS:
            {
				ObjectId dscpMapObjectId;
                trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage->getDscpToTrafficClassMapName()=") + pNsmQosPOConfigSPMessage->getDscpToTrafficClassMapName());
                ResourceId mapPresent = WAVE_MESSAGE_ERROR;
                mapPresent = getDscpTcMapObjectIdByTcMapName(pNsmQosPOConfigSPMessage->getDscpToTrafficClassMapName(), dscpMapObjectId, pPrismSynchronousLinearSequencerContext);
                if(mapPresent != WAVE_MESSAGE_SUCCESS)
                {
                    return mapPresent;
                }
                else
                {
					m->setDscpToTrafficClassMapName(pNsmQosPOConfigSPMessage->getDscpToTrafficClassMapName());
              	}
				break;
            }
			case QOS_INTERFACE_DSCP_COS:
            {
				ObjectId dscpMapObjectId;
                trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage->getDscpToCosMapName()=") + pNsmQosPOConfigSPMessage->getDscpToCosMapName());
				ResourceId mapPresent = WAVE_MESSAGE_ERROR;
                mapPresent = getDscpCosMapObjectIdByDscpCosMapName(pNsmQosPOConfigSPMessage->getDscpToCosMapName(), dscpMapObjectId, pPrismSynchronousLinearSequencerContext);
                if(mapPresent != WAVE_MESSAGE_SUCCESS)
                {
                    return mapPresent;
                }
                else
                {
                	m->setDscpToCosMapName(pNsmQosPOConfigSPMessage->getDscpToCosMapName());
              	}
				break;
            }
            case QOS_INTERFACE_CEE_MAP:
            {
                trace (TRACE_LEVEL_INFO, string("pNsmQosPOConfigSPMessage->getCeeMapName()=") + pNsmQosPOConfigSPMessage->getCeeMapName());
                m->setCeeMapName(pNsmQosPOConfigSPMessage->getCeeMapName());
              break;
            }

            case QOS_INTERFACE_PRIORITY_TAG:
            {
                trace (TRACE_LEVEL_INFO, string("IntfConfLocalWorker:: set Priority Tagging") );
                break;
			}

		}
			break;
		}

	  case DELETE_INTERFACE_QOS:
		{
		   /* Nothing to be done. cmdCode and opCode will convey the info */
			trace (TRACE_LEVEL_DEBUG, string("DELETE_INTERFACE_QOS"));
			break;
		}


	}

	//Fix for the defect TR000377133
	    if ( ( pNsmQosPOConfigSPMessage->getOpCode() == CONFIGURE_INTERFACE_QOS || pNsmQosPOConfigSPMessage->getOpCode() == DELETE_INTERFACE_QOS ) && (pNsmQosPOConfigSPMessage->getCmdCode() == QOS_INTERFACE_PRIORITY_TAG)  ) {
	        trace (TRACE_LEVEL_DEVEL, "sending synchronously to nsm client");
	        status = sendSynchronouslyToWaveClient ("nsm",m);

	    } else {
	trace (TRACE_LEVEL_DEVEL, "sending synchronously to ssm client");
    status = sendSynchronouslyToWaveClient ("qos",m);
	    }

	trace (TRACE_LEVEL_DEBUG, string("sendSynchronouslyToWaveClient  ***** status= ") + status);
	pNsmQosPOConfigSPMessage->setCompletionStatus(status);

	if( status == WAVE_MESSAGE_SUCCESS)
   	{
		if( m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
		{
			status = m->getCompletionStatus();
			trace(TRACE_LEVEL_DEBUG, string("Completion status=") + status);
			pNsmQosPOConfigSPMessage->setCompletionStatus(status);
		}
		else
		{
			status = m->getClientStatus();
			trace(TRACE_LEVEL_DEBUG, string("Client status=") + status);
			pNsmQosPOConfigSPMessage->setCompletionStatus(status);
		}
	}
 	//bpatel: To avoid memory leak
    if(m){
        delete m;
    }
	return status;

    }
  }

	void  IntfConfLocalWorker::NsmQosPOConfigSPMessageHandler (NsmQosPOConfigSPMessage *pNsmQosPOConfigSPMessage)
  {
    // Validations and Queries should go here

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::updateQosPoIntfStep),
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmQosPOConfigSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pPrismSynchronousLinearSequencerContext->execute ();
  }


  void IntfConfLocalWorker::backendSyncUp (PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        NSM_PLUG_INFO_S( string("Entering install phase for LOCAL NSM Plug, creating default INTF MOs"));

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::backendSyncUpConfMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPrismAsynchronousContext, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

  void IntfConfLocalWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        NSM_PLUG_INFO_S( string("Entering install phase for LOCAL NSM Plug, creating default INTF MOs"));

        if ((pWaveAsynchronousContextForBootPhases->getBootReason() == WAVE_BOOT_FIRST_TIME_BOOT) || (pWaveAsynchronousContextForBootPhases->getBootReason() == WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT)) {
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }

        trace (TRACE_LEVEL_INFO,"IntfConfLocalWorker::install() ... Adding PhyInterfaces for New Node...");

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::defaultConfMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pWaveAsynchronousContextForBootPhases, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IntfConfLocalWorker::backendSyncUpConfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        NsmDefaultConfigurationCmdsMessage *msg_p = NULL;
        UI32 size = 0;
        UI32 outbuffNum;
        void *data;

        PrismAsynchronousContext *pPrismAsynchronousContext = dynamic_cast<PrismAsynchronousContext *> (pPrismSynchronousLinearSequencerContext->getPPrismAsynchronousContext());

        msg_p = new NsmDefaultConfigurationCmdsMessage(DEFAULT_INTERFACE);

        status = sendSynchronouslyToWaveClient("nsm", (msg_p));

        if (status != WAVE_MESSAGE_SUCCESS)
        {
            NSM_PLUG_DBG_ERR(" Failed to get interface properties from NSM Daemon");
            NSM_PLUG_DBG_ERR_S(string("ERROR=") + status);
            pPrismAsynchronousContext->setCompletionStatus (status);
            delete msg_p;
            return status;
        }

        outbuffNum = NSM_DEFAULT_CONFIG_RESPONSE;
        msg_p->getDuplicateInterfaceOutputBuffer(outbuffNum,size,data);

        if(size != 0)
        {
            switch (outbuffNum)
            {
                case NSM_DEFAULT_CONFIG_RESPONSE:
                {
                    nsm_intf_all_msg_t *resp_p;
                    resp_p = (nsm_intf_all_msg_t *)data;

                    trace(TRACE_LEVEL_DEBUG , string("IntfConfLocalWorker::backendSyncUPConfMOStep: Total no of physical interfaces Received : ") + resp_p->num_records);

                    PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
                    ResourceId ret = 0;
                    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
                    LocationId locationId = FrameworkToolKit::getThisLocationId ();

                    syncQueryCtxt.addSelectField("ifName");
                    syncQueryCtxt.setLocationIdFilter(locationId);
                    vector<WaveManagedObject *> *pPhyResults = querySynchronously (&syncQueryCtxt);

                    if (pPhyResults && (pPhyResults->size() > 0))
                    {
                        map<string, PhyIntfLocalManagedObject *> phyMOMap;

                        for (UI32 PhyIndex = 0; PhyIndex < pPhyResults->size(); PhyIndex++)
                        {
                            pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pPhyResults)[PhyIndex]);
                            phyMOMap[pPhyIntfLocalManagedObject->getIfName()] = pPhyIntfLocalManagedObject;
                        }

                        for (UI32 i = 0; i < resp_p->num_records; i++)
                        {
                            trace (TRACE_LEVEL_DEBUG, string(" IntfConfLocalWorker::backendSyncUpConfMOStep. Update Interface Name : ") + resp_p->intfdetails[i].ifName );

                            if(strlen(resp_p->intfdetails[i].ifName) > 2)
                            {
                                if((strncmp(resp_p->intfdetails[i].ifName,"te",2) == 0) ||
                                   (strncmp(resp_p->intfdetails[i].ifName,"gi",2) == 0) ||
                                   (strncmp(resp_p->intfdetails[i].ifName,"fo",2) == 0))
                                {
                                    string ifName;
                                    ret = NsmUtils::getThreeTupleIfName( resp_p->intfdetails[i].ifName+2,ifName, 0);

                                    if (WAVE_PERSISTENCE_CLUSTER_DISABLED == ret)
                                        ifName = resp_p->intfdetails[i].ifName+2;

                                    pPhyIntfLocalManagedObject = NULL;
                                    pPhyIntfLocalManagedObject = phyMOMap[ifName];

                                    if(pPhyIntfLocalManagedObject == NULL)
                                    {
                                        trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::backendSyncUpConfMOStep:couldn't find the MO for ") + ifName);

                                        NsmCreatePhyIntfMessage *pCreateMsg = new NsmCreatePhyIntfMessage(resp_p->intfdetails[i].ifName, resp_p->intfdetails[i].m_ifIndex, resp_p->intfdetails[i].mtu, resp_p->intfdetails[i].desc);

                                        if(resp_p->intfdetails[i].islState == ISL)
										{
                                            pCreateMsg->setPortrole(true);
										}

                                        //UI32 islCapability = resp_p->intfdetails[i].isl_capable;
                                        pCreateMsg->setIslCapability(resp_p->intfdetails[i].isl_capable);
                                        createPhyIntfMO(pCreateMsg, pPrismSynchronousLinearSequencerContext);
										delete pCreateMsg;
                                        continue;
                                    }

                                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("description");
                                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("status");
                                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("active");
                                    pPhyIntfLocalManagedObject->addAttributeToBeUpdated("Portrole");
                                    updateWaveManagedObject (pPhyIntfLocalManagedObject);
                                    pPhyIntfLocalManagedObject->setDesc (resp_p->intfdetails[i].desc);

                                    trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::backendSyncUpConfMOStep: got status : ") +
                                    		((resp_p->intfdetails[i].status == SHUTDOWN) ? "shutdown" : "no shutdown") + " and PortRole :" +
                                    				((resp_p->intfdetails[i].islState == ISL) ? "ISL" : "Edge" )+ " from backend for " + ifName);

                                    if(resp_p->intfdetails[i].status == SHUTDOWN)
									{
                                        pPhyIntfLocalManagedObject->setStatus (true);
									}
                                    else
									{
                                        pPhyIntfLocalManagedObject->setStatus (false);
									}

                                    if(resp_p->intfdetails[i].islState == ISL)
                                    {
                                        pPhyIntfLocalManagedObject->setPortrole(true);
                                    }
                                    else
                                    {
                                        pPhyIntfLocalManagedObject->setPortrole(false);
                                    }

                                    pPhyIntfLocalManagedObject->setActive (true);
                                }
                            }
                        }

                        if(pPrismSynchronousLinearSequencerContext)
                            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pPhyResults);

                        pPhyResults->clear();
                        delete pPhyResults;
                    }
                }
                break;
            }
        }

        pPrismAsynchronousContext->setCompletionStatus(status);

        if(msg_p)
            delete msg_p;

        // If a Mercury is secondary node in a cluster and slot, whose interfaces are ISL ports
        // connecting it with rest of cluster, made power-off. Then this mercury gets separated
        // from cluster and it will primary. then backendSyncUp get called which will retreives
        // ports details from backend. backend does not interfaces corresponding to that slot
        // so these interfaces will not be updated in Db. Hence this additional logic is added
        // to query slotMO which is offline and made all its interfaces as inactive.
        if ((m_lastSlotIdMadeOffline > 0) &&
            (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) &&
            (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType()))
        {
            UI32 count = 0;
            WaveManagedObjectSynchronousQueryContext slotOfflineQueryCtxt (WaveSlotLocalManagedObject::getClassName());
            slotOfflineQueryCtxt.addAndAttribute(new AttributeUI32(m_lastSlotIdMadeOffline, "slotNumber"));
            slotOfflineQueryCtxt.addAndAttribute(new AttributeUI32(SLOT_OFFLINE, "slotState"));
            ResourceId resId = querySynchronouslyForCount(&slotOfflineQueryCtxt, count);
            if ((WAVE_MESSAGE_SUCCESS == resId) && (count > 0)) {
                WaveManagedObjectSynchronousQueryContextForUpdate *qctx = new WaveManagedObjectSynchronousQueryContextForUpdate (PhyIntfLocalManagedObject::getClassName());
                qctx->setLocationIdFilter (FrameworkToolKit::getThisLocationId());
                qctx->setSlotFilter (m_lastSlotIdMadeOffline);
                PhyIntfLocalManagedObject *criteriaMo = dynamic_cast<PhyIntfLocalManagedObject *> (qctx->getWaveManagedObjectToAddUpdateAttributes());
                criteriaMo->setActive (false);
                criteriaMo->addAttributeToBeUpdated ("active");
                string sqlStr = "";
                qctx->getSqlForUpdate (sqlStr);
                trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::backendSyncUpConfMOStep: Slot offline/faulty query: - ")+ sqlStr);
                updateMultipleWaveManagedObjects (qctx);
                trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::backendSyncUpConfMOStep: Moved interfaces to inactive state on receiving Slot offline/faulty for slot: - ")+ m_lastSlotIdMadeOffline);
            }

            // reset the class level variable for furtheri use.
            m_lastSlotIdMadeOffline = 0;
        }

        return status;
    }

    ResourceId IntfConfLocalWorker::defaultConfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        NsmDefaultConfigurationCmdsMessage *msg_p = NULL;
        UI32 size = 0;
        UI32 outbuffNum;
        void *data;

        trace (TRACE_LEVEL_DEVEL,"NsmLocalInstall - New Node : Quering Nsm Backend NsmDefaultConfigurationCmdsMessage(DEFAULT_INTERFACE)");

        WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = dynamic_cast<WaveAsynchronousContextForBootPhases *> (pPrismSynchronousLinearSequencerContext->getPPrismAsynchronousContext());

        msg_p = new NsmDefaultConfigurationCmdsMessage(DEFAULT_INTERFACE);

        status = sendSynchronouslyToWaveClient("nsm", (msg_p));

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace (TRACE_LEVEL_ERROR,"NsmLocalInstall - New Node : Failed to get interface list from NSM Daemon");
            trace (TRACE_LEVEL_ERROR,string("ERROR=") + status);
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            //bpatel: To avoid memory leak might be a change
            delete msg_p;
			return status;
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

                        trace (TRACE_LEVEL_INFO, string("NsmLocalInstall - New Node : Total no of physical interfaces Received : ") + resp_p->num_records);

                        for (i = 0; i < resp_p->num_records; i++) {

                            trace (TRACE_LEVEL_INFO, string("NsmLocalInstall - New Node : Interface Name : ") + resp_p->intfdetails[i].ifName  + string("ISL State : ") + (resp_p->intfdetails[i].islState == ISL ? "True" : "False") + string("    Isl Capable: ") + resp_p->intfdetails[i].isl_capable);

                            if(strlen(resp_p->intfdetails[i].ifName) > 2)
                            {
                                if((strncmp(resp_p->intfdetails[i].ifName,SHORT_TEN_GIGABIT_ETHERNET_TOKEN,2) == 0) || (strncmp(resp_p->intfdetails[i].ifName,SHORT_GIGABIT_ETHERNET_TOKEN,2) == 0) || (strncmp(resp_p->intfdetails[i].ifName,SHORT_FORTYGIGABIT_ETHERNET_TOKEN,2) == 0))
                                {
                                    NsmCreatePhyIntfMessage message(resp_p->intfdetails[i].ifName,resp_p->intfdetails[i].m_ifIndex,resp_p->intfdetails[i].mtu,resp_p->intfdetails[i].desc);
                                    if(resp_p->intfdetails[i].islState == ISL)
                                        message.setPortrole(true);
									UI32 islCapability = resp_p->intfdetails[i].isl_capable;
									message.setIslCapability(islCapability);
                                    createPhyIntfMO(&message, pPrismSynchronousLinearSequencerContext);
                                }
                            }
                        }
                    }
                    break;
            }
        }

                                                                                                                                                    pWaveAsynchronousContextForBootPhases->setCompletionStatus(status);

		 //bpatel : To avoid memory leak
        if(msg_p){
            delete msg_p;
			msg_p = NULL;
        }

        return status;
    }
     void IntfConfLocalWorker::NsmLocalShowMessageHandler(NsmLocalShowIntfMessage* pNsmLocalShowIntfMessage)
     {
		NSM_PLUG_DBG_S(string(" Debug 2 : "));
        if (pNsmLocalShowIntfMessage->getIsGwRequest())
    	{
		  	NSM_PLUG_DBG_S(string(" Debug 3 : "));
            PrismLinearSequencerStep sequencerSteps[] =
	        {
                 reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::sendNsmIntfShowCommandToCluster),
                 reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),
                 reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
             };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pNsmLocalShowIntfMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->start ();
            return;
        } else {
		  	NSM_PLUG_DBG_S(string(" Debug 4 : "));
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::sendNsmIntfShowCommandToNsmDaemon),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };

            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext( pNsmLocalShowIntfMessage, this, sequencerSteps,
            sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }

void IntfConfLocalWorker::NsmLocalShowEcmpSendToClusterCallback(WaveSendToClusterContext *ctx_p)
{
	trace(TRACE_LEVEL_INFO,string("entered NsmLocalShowEcmpSendToClusterCallback"));
	PrismLinearSequencerContext            *seq_ctx_p = NULL;
	NsmLocalShowEcmpMessage                *req_msg_p = NULL;
	NsmLocalShowEcmpMessage                *resp_msg_p = NULL;
	NsmLocalShowEcmpMessage                *sw_resp_p = NULL;

	seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
	NSM_PLUG_ASSERT_PTR(seq_ctx_p);
	req_msg_p = dynamic_cast<NsmLocalShowEcmpMessage *> (seq_ctx_p->getPPrismMessage ());
	resp_msg_p =  dynamic_cast<NsmLocalShowEcmpMessage *> (ctx_p->getPPrismMessageForPhase1());

	NSM_PLUG_ASSERT_PTR(req_msg_p);
	NSM_PLUG_ASSERT_PTR(resp_msg_p);

	vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
	UI32    i = 0;

	NSM_PLUG_DBG_S(string("Response processing: Num resp:") + locations.size());

	for (i = 0; i < locations.size(); i++)
	{
		if (i == 0)
		{
			NSM_PLUG_DBG_S(string("locations:") + locations[i]);
		}
		else
		{
			NSM_PLUG_DBG_S(string(":") + locations[i]);
		}

	}
	for (i = 0; i < locations.size(); i++)
	{
		sw_resp_p = dynamic_cast<NsmLocalShowEcmpMessage*> (ctx_p->getResultingMessageForPhase1(locations[i]));

		NSM_PLUG_ASSERT_PTR(sw_resp_p);
		if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
		{
			req_msg_p->copyAllBuffers(*sw_resp_p);
		}
		else
		{
			NSM_PLUG_DBG_S((string("Response processing: The location: ") + locations[i] ) );
			NSM_PLUG_DBG_S (string("returned error: ")  +
				FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
		}
	}
	seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

	delete resp_msg_p;
	delete ctx_p;
	return;
}

void  IntfConfLocalWorker::sendNsmShowEcmpCommandToCluster(PrismLinearSequencerContext *seq_ctx_p)
{
	SI32 locationId = -1;
	trace(TRACE_LEVEL_INFO,string("entered sendNsmShowEcmpCommandToCluster"));

	NsmLocalShowEcmpMessage *msg_p = dynamic_cast<NsmLocalShowEcmpMessage*> (seq_ctx_p->getPPrismMessage ());
	NSM_PLUG_ASSERT_PTR(msg_p);

	NsmLocalShowEcmpMessage *loc_msg_p = new NsmLocalShowEcmpMessage(msg_p->getCmdCode(),msg_p->getRbridgeId(),false);
	NSM_PLUG_ASSERT_PTR(loc_msg_p);

	WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&IntfConfLocalWorker::NsmLocalShowEcmpSendToClusterCallback), seq_ctx_p);
	NSM_PLUG_ASSERT_PTR(send_ctx_p);

	send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

	/*if locationId is set send only to that location Id
	 * use case show interface 2/0/1 */
	locationId = DcmToolKit::getLocationIdFromMappedId(msg_p->getRbridgeId());

	if( -1 != locationId && 0 != locationId)
	{
		vector<UI32>                locations;
		locations.push_back(locationId);
		send_ctx_p->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster(send_ctx_p);
	return;
}

ResourceId IntfConfLocalWorker::sendNsmShowEcmpCommandToNsmDaemon(PrismLinearSequencerContext *seq_ctx_p)
{
	trace(TRACE_LEVEL_INFO,string("entered sendNsmShowEcmpCommandToNsm"));
	SI32 mappedId =0;
	mappedId= DcmToolKit::getLocalMappedId();

	NsmLocalShowEcmpMessage *msg_p = dynamic_cast<NsmLocalShowEcmpMessage*> (seq_ctx_p->getPPrismMessage ());
	NsmShowEcmpMessage *m = new NsmShowEcmpMessage (msg_p -> getCmdCode(),msg_p -> getRbridgeId());
	if(msg_p->getRbridgeId()==0)
		m->setRbridgeId(mappedId);

	ResourceId status = sendSynchronouslyToWaveClient ("nsm",m);

	if(WAVE_MESSAGE_SUCCESS != status)
	{
		NSM_PLUG_DBG_S(string("sendSynchronouslyToWaveClient Failed") + status);
		delete m;
		return status;
	}
	msg_p->copyAllBuffers(*m);
	status = m->getCompletionStatus();
	trace(TRACE_LEVEL_DEBUG,string("status is ")+status);

	if(m){
		delete m;
	}
	return status;
}

void IntfConfLocalWorker::NsmLocalShowEcmpMessageHandler(NsmLocalShowEcmpMessage* pNsmLocalShowEcmpMessage)
{
	trace(TRACE_LEVEL_INFO,string("Entered NsmEcmpSHowMessagHandler"));

	if(pNsmLocalShowEcmpMessage->getIsGwRequest())
	{
		PrismLinearSequencerStep sequencerSteps[] =
		{
			reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::sendNsmShowEcmpCommandToCluster),
			reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),
			reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
		};

		PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pNsmLocalShowEcmpMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
		pPrismLinearSequencerContext->start ();
		return;
	}
	else{
		PrismSynchronousLinearSequencerStep sequencerSteps[] =
		{
			reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::sendNsmShowEcmpCommandToNsmDaemon),
			reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
		};

		PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext(pNsmLocalShowEcmpMessage, this, sequencerSteps,sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
		pPrismSynchronousLinearSequencerContext->execute ();
	}
}

ResourceId IntfConfLocalWorker::sendNsmShowVlagCommandToNsmDaemon(PrismLinearSequencerContext *seq_ctx_p)
{
    trace(TRACE_LEVEL_INFO,string("entered sendNsmShowVlagCommandToNsm"));

    NsmLocalShowVlagMessage *msg_p = dynamic_cast<NsmLocalShowVlagMessage*> (seq_ctx_p->getPPrismMessage ());
    NsmShowVlagMessage *m = new NsmShowVlagMessage (msg_p -> getCmdCode(),msg_p -> getPoId());

    ResourceId status = sendSynchronouslyToWaveClient ("nsm",m);

    if(WAVE_MESSAGE_SUCCESS != status)
    {
        NSM_PLUG_DBG_S(string("sendSynchronouslyToWaveClient Failed") + status);
        delete m;
        return status;
    }
    msg_p->copyAllBuffers(*m);
    status = m->getCompletionStatus();
    trace(TRACE_LEVEL_INFO,string("status is ")+status);

    if(m){
        delete m;
    }
    return status;
}

void IntfConfLocalWorker::NsmLocalShowVlagMessageHandler(NsmLocalShowVlagMessage* pNsmLocalShowVlagMessage)
{
	trace(TRACE_LEVEL_INFO,string("Entered NsmVlagShowMessagHandler"));

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
		reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::sendNsmShowVlagCommandToNsmDaemon),
		reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
		reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pNsmLocalShowVlagMessage, this, sequencerSteps,sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
	pPrismSynchronousLinearSequencerContext->execute ();
}

     void IntfConfLocalWorker::NsmLocalDebugMessageHandler(NsmLocalDebugMessage* pNsmLocalDebugMessage)
     {

        if (pNsmLocalDebugMessage->getIsGwRequest())
    	{
            PrismLinearSequencerStep sequencerSteps[] =
	        {
                 reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::sendNsmDebugCommandToCluster),
                 reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),
                 reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
             };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pNsmLocalDebugMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->start ();
            return;
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::sendNsmDebugCommandToNsmDaemon),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };

            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext(pNsmLocalDebugMessage, this, sequencerSteps,
            sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }

     void IntfConfLocalWorker::NsmLocalPortMediaShowMessageHandler(NsmLocalPortMediaShowIntfMessage* pNsmLocalPortMediaShowIntfMessage)
     {

        if (pNsmLocalPortMediaShowIntfMessage->getIsGwRequest())
    	{
            PrismLinearSequencerStep sequencerSteps[] =
	        {
                 reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::sendNsmIntfPortMediaShowCommandToCluster),
                 reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),
                 reinterpret_cast<PrismLinearSequencerStep> (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
             };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pNsmLocalPortMediaShowIntfMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->start ();
            return;
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::sendNsmIntfPortMediaShowCommandToNsmDaemon),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };

            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext( pNsmLocalPortMediaShowIntfMessage, this, sequencerSteps,
            sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }

    void IntfConfLocalWorker::sendNsmIntfShowCommandToCluster(PrismLinearSequencerContext *seq_ctx_p)
    {
		SI32 locationId = -1;
		NSM_PLUG_DBG("Entering ...");

		NsmLocalShowIntfMessage *msg_p = dynamic_cast<NsmLocalShowIntfMessage*> (seq_ctx_p->getPPrismMessage ());
		NSM_PLUG_ASSERT_PTR(msg_p);

		NsmLocalShowIntfMessage *loc_msg_p = new NsmLocalShowIntfMessage(msg_p -> getIfName(), msg_p -> getCmdCode(), false);
		loc_msg_p->setStartPoId(msg_p->getStartPoId());
		loc_msg_p->setEndPoId(msg_p->getEndPoId());
		loc_msg_p->setReqNoRecords(msg_p->getReqNoRecords());
		loc_msg_p->setReqStartIndex(msg_p->getReqStartIndex());
		loc_msg_p->setReqEndIndex(msg_p->getReqEndIndex());
		loc_msg_p->setReqIfType(msg_p->getReqIfType());
		loc_msg_p->setReqRequestType(msg_p->getReqRequestType());
		NSM_PLUG_ASSERT_PTR(loc_msg_p);

		WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&IntfConfLocalWorker::NsmLocalShowSendToClusterCallback), seq_ctx_p);
		NSM_PLUG_ASSERT_PTR(send_ctx_p);

		send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

		/*if locationId is set send only to that location Id
		 * use case show interface 2/0/1 */

		if( -1 != (locationId = msg_p -> getLocationId()) )
		{
			vector<UI32>                locations;
			locations.push_back(locationId);
			send_ctx_p->setLocationsToSendToForPhase1(locations);
		}

		// get locations from message and set to WaveSendToClusterContext
		vector<UI32> *destLocs = msg_p->getDestinationLocations();
		if (destLocs == NULL) {
			NSM_PLUG_DBG("Got NULL destination locations vector.");
		} else if (destLocs->size() > 0) {
			NSM_PLUG_DBG("Got valid destination locations vector. Sending to all locations");
			send_ctx_p->setLocationsToSendToForPhase1(*destLocs);
		}

		sendToWaveCluster(send_ctx_p);
		return;
	}


    void IntfConfLocalWorker::sendNsmDebugCommandToCluster(PrismLinearSequencerContext *seq_ctx_p)
    {
		UI32                        locationId = 0;
		vector<UI32>                locations;
		NSM_PLUG_DBG("Entering ...");

		NsmLocalDebugMessage *msg_p = dynamic_cast<NsmLocalDebugMessage*> (seq_ctx_p->getPPrismMessage ());
		NSM_PLUG_ASSERT_PTR(msg_p);

		NsmLocalDebugMessage *loc_msg_p = new NsmLocalDebugMessage(msg_p -> getDebugType(), msg_p -> getCmdCode(), false);
		NSM_PLUG_ASSERT_PTR(loc_msg_p);

		WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&IntfConfLocalWorker::NsmLocalDebugSendToClusterCallback), seq_ctx_p);
		NSM_PLUG_ASSERT_PTR(send_ctx_p);

		send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        locationId = NsmUtils::getLocalLocationId();
        if (locationId) {
            locations.push_back(locationId);
        }
        send_ctx_p->setLocationsToSendToForPhase1(locations);

		sendToWaveCluster(send_ctx_p);
		return;
	}


    void IntfConfLocalWorker::sendNsmIntfPortMediaShowCommandToCluster(PrismLinearSequencerContext *seq_ctx_p)
    {
		UI32                        locationId = 0;
		SI32                        mappedId = 0;
		vector<UI32>                locations;
		bool                        sendToLocal = false;

		NSM_PLUG_DBG("Entering ...");

		NsmLocalPortMediaShowIntfMessage *msg_p = dynamic_cast<NsmLocalPortMediaShowIntfMessage*> (seq_ctx_p->getPPrismMessage ());
		NSM_PLUG_ASSERT_PTR(msg_p);

		NsmLocalPortMediaShowIntfMessage *loc_msg_p = new NsmLocalPortMediaShowIntfMessage(msg_p -> getIfTypeName(),msg_p->getIfName(), msg_p -> getCmdSlotId(),msg_p -> getCmdCode(), false);
		NSM_PLUG_ASSERT_PTR(loc_msg_p);

		WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&IntfConfLocalWorker::NsmLocalPortMediaShowSendToClusterCallback), seq_ctx_p);
		NSM_PLUG_ASSERT_PTR(send_ctx_p);

		send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        if (0 == msg_p->getIfName().length()) {

        if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  {
        	 trace (TRACE_LEVEL_DEBUG, string("Sending Show Command to Cluster isValidTwoTuple true"));
             sendToLocal = true;
        } else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  {
                /* Send to the targetted LocationId pointed to by the
                 * mappedId.
                 */
        		trace (TRACE_LEVEL_DEBUG, string("Sending Show Command to Cluster isValidThreeTuple true"));
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
		if (locationId == 0) {
		    delete send_ctx_p;
 		    delete loc_msg_p;
		    seq_ctx_p->executeNextStep(NSM_INTERFACE_DOES_NOT_EXIST);
                    return;
		}
                locations.push_back(locationId);
                send_ctx_p->setLocationsToSendToForPhase1(locations);
            }


        if (sendToLocal) {
            /* It is not interface specific command or the interface is
             * two tuple one on the local switch.
             */
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) {
               locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }

		}

        sendToWaveCluster(send_ctx_p);

        return;
    }


    ResourceId IntfConfLocalWorker::sendNsmIntfShowCommandToNsmDaemon(PrismLinearSequencerContext *seq_ctx_p)
    {
        NSM_PLUG_DBG("Entering ...");
        SI32 mappedId = DcmToolKit::getLocalMappedId();
        if(mappedId == -1)
            mappedId = 0;

        NsmLocalShowIntfMessage *msg_p = dynamic_cast<NsmLocalShowIntfMessage*> (seq_ctx_p->getPPrismMessage ());
        NsmShowIntfMessage *m = new NsmShowIntfMessage (msg_p -> getIfName(), msg_p -> getCmdCode(),mappedId);
        NSM_PLUG_DBG_S(string("IntfConfLocalWorker::sendNsmIntfShowCommandToNsmDaemon startPoId: ") + msg_p->getStartPoId() + string(" -- endPoId: ") + msg_p->getEndPoId());
        m->setStartPoId(msg_p->getStartPoId());
        m->setEndPoId(msg_p->getEndPoId());
		m->setReqNoRecords(msg_p->getReqNoRecords());
		m->setReqStartIndex(msg_p->getReqStartIndex());
		m->setReqEndIndex(msg_p->getReqEndIndex());
		m->setReqIfType(msg_p->getReqIfType());
		m->setReqRequestType(msg_p->getReqRequestType());

		//If daemon doesn't responsd within 2 min, message will be timeout.
		m->setTimeOutInMilliSeconds (120000);
        ResourceId status = sendSynchronouslyToWaveClient ("nsm",m);

        if(WAVE_MESSAGE_SUCCESS != status)
        {
            NSM_PLUG_DBG_S(string("sendSynchronouslyToWaveClient Failed") + status);
            delete m;
            return status;
        }
		msg_p->copyAllBuffers(*m);
		 //bpatel: To avoid memory leak
        if(m){
            delete m;
        }
		return status;
	}


    ResourceId IntfConfLocalWorker::sendNsmDebugCommandToNsmDaemon(PrismLinearSequencerContext *seq_ctx_p)
    {
        NSM_PLUG_DBG("Entering ...");
        SI32 mappedId = DcmToolKit::getLocalMappedId();
        if(mappedId == -1)
            mappedId = 0;
		NsmLocalDebugMessage *msg_p = dynamic_cast<NsmLocalDebugMessage*> (seq_ctx_p->getPPrismMessage ());
	ResourceId      status   = WAVE_MESSAGE_ERROR;
	ResourceId      Err      = WAVE_MESSAGE_SUCCESS;

	/* Send 'no debug all' to all modules */

	if(msg_p->getCmdCode() == ALLDEBUGSET_ALL_NO)
	{
		/* LACP */
		{
			LacpDebugMessage *m = new LacpDebugMessage ();
			m->setNegate(true);
			m->setCmdCode(LACP_DEBUG_CMDCODE_ALL);

			sendMessageToClient("lacp", m, Err);
		}
		/* LLDP */
		{
		  	sendMessageToClient(LLDP_DCM_CLIENT_NAME,
					new LldpClientDebugMessage("", LLDP_DEBUG_CMD_CODE_PACKET_ALL, 0, 0, 0, LLDP_RECORDS_PER_PAGE), Err);	
		}
		/* DOT1X */
		{
			sendMessageToClient("dauthd", new Dot1XDebugMessage("0", "0", DEBUG_NO_AUTH,DEBUG_ALL_CMD,0,0), Err);
		}
		/* IGMP */
		{
			sendMessageToClient("IGMP", new IgmpsClientDebugMessage("", "", IGMPS_DEBUG_ALL_CLI, true), Err);
		}
		/* NSM */
		{
			sendMessageToClient("nsm", new NsmDebugMessage (0,NSMDEBUGSET_ALL_NO,mappedId), Err);
		}
		/* VRRP */
		{
			VRRPDebugMessage *m = new VRRPDebugMessage();
			m->setNegation (true);
			m->setCmdCode (VRRP_DEBUG_CMD_ALL);

			sendMessageToClient("vrrp", m, Err);
		}
		/* OSPF */
		{
			char *argv[2] = {(char*)"all-vrfs", (char*)"all"};
			OSPFDebugMessage *m = new OSPFDebugMessage();
			m->setCmdCode(DEBUG_IP_OSPF_ALL);
			m->setNoArgv(argv, 2);

			sendMessageToClient("ospf", m, Err);
		}
		/* MSTP */
		{
            MstpShowMessage *m = new MstpShowMessage();
            m->setCmdCode(MSTP_NO_DEBUG_SPT_ALL);
			sendMessageToClient("mstp", m, Err);
		}
		/* RTM */
		{
			vector<string> argv;
			StringUtils::tokenize("no debug ip rtm all", argv);
			sendMessageToClient(RTM_DCM_CLIENT_NAME, new GenericClientMessage_(argv, RTMNODEBUG), Err);
		}
		/* BGP */
        {
            ResourceId bgpStatus;
            UI32 value = BGPCLIENTNODEBUGALL;
            UnifiedClientNotifyMessage *bgpMsg = new UnifiedClientNotifyMessage();
            bgpMsg->setbackendNotifyValue(value);
            bgpStatus = sendSynchronouslyToWaveClient("bgp", bgpMsg);
            if (bgpStatus != WAVE_MESSAGE_SUCCESS || bgpMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                tracePrintf(TRACE_LEVEL_ERROR, "IntfConfLocalWorker::BGP no debug all failed: message status=%u, completion status=%u",
                    bgpStatus, bgpMsg->getCompletionStatus());
                Err = WAVE_MESSAGE_ERROR;
            }
			delete bgpMsg;
        }
		return Err;
	}
	else{

        NsmDebugMessage *m = new NsmDebugMessage (msg_p->getDebugType(),msg_p->getCmdCode(),mappedId);
	    status = sendSynchronouslyToWaveClient ("nsm",m);
        if(WAVE_MESSAGE_SUCCESS != status)
        {
            NSM_PLUG_DBG_S(string("sendSynchronouslyToWaveClient Failed") + status);
            delete m;
            return status;
        }
        //NsmLocalDebugMessage *msg_p = dynamic_cast<NsmLocalDebugMessage*> (seq_ctx_p->getPPrismMessage ());
		msg_p->copyAllBuffers(*m);
		 //bpatel: To avoid memory leak
        if(m){
            delete m;
        }
		return status;
	}
}

	void IntfConfLocalWorker::sendMessageToClient (const string &name, DcmManagementInterfaceMessage *pMessage, ResourceId &errStatus)
    {
        tracePrintf(TRACE_LEVEL_DEBUG, "IntfConfLocalWorker::sendMessageToClient : Sending message to %s", name.c_str());
        ResourceId status = sendSynchronouslyToWaveClient(name, pMessage);

        if (status != WAVE_MESSAGE_SUCCESS || pMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS || pMessage->getClientStatus() != 0) {
            tracePrintf(TRACE_LEVEL_ERROR, "IntfConfLocalWorker::sendMessageToClient : Error sending message to %s; message status=%u, completion status=%u, client status=%d",
            name.c_str(), status, pMessage->getCompletionStatus(), pMessage->getClientStatus());
            errStatus = WAVE_MESSAGE_ERROR;
        }

        delete pMessage;
    }

    ResourceId IntfConfLocalWorker::sendNsmIntfPortMediaShowCommandToNsmDaemon(PrismLinearSequencerContext *seq_ctx_p)
    {
		string twoTupleIfName;
        NSM_PLUG_DBG("Entering ...");
        SI32 mappedId = DcmToolKit::getLocalMappedId();
        if(mappedId == -1)
            mappedId = 0;

		NsmLocalPortMediaShowIntfMessage *msg_p = dynamic_cast<NsmLocalPortMediaShowIntfMessage*> (seq_ctx_p->getPPrismMessage ());

		if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
			NsmUtils::getTwoTupleIfName(msg_p->getIfName(), twoTupleIfName);
		else
			twoTupleIfName = msg_p->getIfName();

        NsmPortMediaShowIntfMessage *m = new NsmPortMediaShowIntfMessage (msg_p -> getIfTypeName(),twoTupleIfName, msg_p -> getCmdSlotId(),msg_p -> getCmdCode(), mappedId);
        ResourceId status = sendSynchronouslyToWaveClient ("nsm",m);
        if(WAVE_MESSAGE_SUCCESS != status)
        {
            NSM_PLUG_DBG_S(string("sendSynchronouslyToWaveClient Failed") + status);
            delete m;
            return status;
        }

		status = m->getClientStatus();
        if (status == 0) {
            trace (TRACE_LEVEL_INFO, string("IntfConfLocalWorker::"
                "sendNsmIntfPortMediaShowCommandToNsmDaemon() "
                " client status=0, convert it to WAVE_MESSAGE_SUCCESS - ") +
                WAVE_MESSAGE_SUCCESS);
            status = WAVE_MESSAGE_SUCCESS;
        }

        NSM_PLUG_DBG_S(string("return client status is ")+ FrameworkToolKit::localize(status));
		msg_p->setCompletionStatus(status);
        //NsmLocalPortMediaShowIntfMessage *msg_p = dynamic_cast<NsmLocalPortMediaShowIntfMessage*> (seq_ctx_p->getPPrismMessage ());
		msg_p->copyAllBuffers(*m);
		//bpatel : To avoid memory leak
        if(m){
            delete m;
        }
		return status;
	}

    void IntfConfLocalWorker::NsmLocalShowSendToClusterCallback(WaveSendToClusterContext *ctx_p)
	{
		NSM_PLUG_DBG("Entering...");
        PrismLinearSequencerContext            *seq_ctx_p = NULL;
        NsmLocalShowIntfMessage                *req_msg_p = NULL;
        NsmLocalShowIntfMessage                *resp_msg_p = NULL;
        NsmLocalShowIntfMessage                *sw_resp_p = NULL;

        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

        NSM_PLUG_ASSERT_PTR(seq_ctx_p);

        req_msg_p = dynamic_cast<NsmLocalShowIntfMessage *> (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<NsmLocalShowIntfMessage *> (ctx_p->getPPrismMessageForPhase1());

        NSM_PLUG_ASSERT_PTR(req_msg_p);
        NSM_PLUG_ASSERT_PTR(resp_msg_p);

        if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus )
        {

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        NSM_PLUG_DBG_S(string("Response processing: Num resp:") + locations.size());

        for (i = 0; i < locations.size(); i++)
        {
        	ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locations[i]);

        	if(WAVE_MESSAGE_SUCCESS == statusPerLocationId)
        	{
        		sw_resp_p = dynamic_cast<NsmLocalShowIntfMessage*> (ctx_p->getResultingMessageForPhase1(locations[i]));
        		NSM_PLUG_ASSERT_PTR(sw_resp_p);
        		req_msg_p->copyAllBuffers(*sw_resp_p);
        	}
        	else
        	{
                NSM_PLUG_DBG_S((string("Response processing: The location: ") + locations[i] ) );
				NSM_PLUG_DBG_S (string("returned error: ")  + FrameworkToolKit::localize(statusPerLocationId));
        	}
		}
        }
        else
        {
        	NSM_PLUG_DBG_ERR_S(string("NsmLocalShowSendToClusterCallback : ") + string("returned error: ")  + FrameworkToolKit::localize(sendToClusterCompletionStatus));
        }

        NSM_PLUG_DBG_S(string("NsmLocalShowSendToClusterCallback : MC return status: ") + sendToClusterCompletionStatus);
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);

        delete resp_msg_p;
        delete ctx_p;
        return;

	}

    void IntfConfLocalWorker::NsmLocalPortMediaShowSendToClusterCallback(WaveSendToClusterContext *ctx_p)
	{
		NSM_PLUG_DBG("Entering...");
        PrismLinearSequencerContext            *seq_ctx_p = NULL;
        NsmLocalPortMediaShowIntfMessage       *req_msg_p = NULL;
        NsmLocalPortMediaShowIntfMessage       *resp_msg_p = NULL;
        NsmLocalPortMediaShowIntfMessage       *sw_resp_p = NULL;
		ResourceId status = WAVE_MESSAGE_SUCCESS;


        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

        NSM_PLUG_ASSERT_PTR(seq_ctx_p);

        req_msg_p = dynamic_cast<NsmLocalPortMediaShowIntfMessage *> (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<NsmLocalPortMediaShowIntfMessage *> (ctx_p->getPPrismMessageForPhase1());

        NSM_PLUG_ASSERT_PTR(req_msg_p);
        NSM_PLUG_ASSERT_PTR(resp_msg_p);
        //req_msg_p->copyBuffersFrom(*resp_msg_p);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        NSM_PLUG_DBG_S(string("Response processing: Num resp:") + locations.size());

        for (i = 0; i < locations.size(); i++)
        {
            if (i == 0)
			{
                NSM_PLUG_DBG_S(string("locations:") + locations[i]);
			}
            else
			{
                NSM_PLUG_DBG_S(string(":") + locations[i]);
			}

        }

        for (i = 0; i < locations.size(); i++)
        {
            sw_resp_p = dynamic_cast<NsmLocalPortMediaShowIntfMessage*> (ctx_p->getResultingMessageForPhase1(locations[i]));
            NSM_PLUG_ASSERT_PTR(sw_resp_p);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
            {
                req_msg_p->copyAllBuffers(*sw_resp_p);
            }
            else
            {
                NSM_PLUG_DBG_S((string("Response processing: The location: ") + locations[i] ) );
				NSM_PLUG_DBG_S (string("returned error: ")  + FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
				status = sw_resp_p->getCompletionStatus();
            }
		}
        seq_ctx_p->executeNextStep(status);

        delete resp_msg_p;
        delete ctx_p;
        return;

	}

    void IntfConfLocalWorker::NsmLocalDebugSendToClusterCallback(WaveSendToClusterContext *ctx_p)
	{
		NSM_PLUG_DBG("Entering...");
        PrismLinearSequencerContext            *seq_ctx_p = NULL;
        NsmLocalDebugMessage       *req_msg_p = NULL;
        NsmLocalDebugMessage       *resp_msg_p = NULL;
        NsmLocalDebugMessage       *sw_resp_p = NULL;


        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

        NSM_PLUG_ASSERT_PTR(seq_ctx_p);

        req_msg_p = dynamic_cast<NsmLocalDebugMessage *> (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<NsmLocalDebugMessage *> (ctx_p->getPPrismMessageForPhase1());

        NSM_PLUG_ASSERT_PTR(req_msg_p);
        NSM_PLUG_ASSERT_PTR(resp_msg_p);
        //req_msg_p->copyBuffersFrom(*resp_msg_p);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        NSM_PLUG_DBG_S(string("Response processing: Num resp:") + locations.size());

        for (i = 0; i < locations.size(); i++)
        {
            if (i == 0)
			{
                NSM_PLUG_DBG_S(string("locations:") + locations[i]);
			}
            else
			{
                NSM_PLUG_DBG_S(string(":") + locations[i]);
			}

        }

        for (i = 0; i < locations.size(); i++)
        {
            sw_resp_p = dynamic_cast<NsmLocalDebugMessage*> (ctx_p->getResultingMessageForPhase1(locations[i]));
            NSM_PLUG_ASSERT_PTR(sw_resp_p);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
            {
                req_msg_p->copyAllBuffers(*sw_resp_p);
            }
            else
            {
                NSM_PLUG_DBG_S((string("Response processing: The location: ") + locations[i] ) );
				NSM_PLUG_DBG_S (string("returned error: ")  + FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
            }
		}
        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;
        return;

	}



    void
    IntfConfLocalWorker::LacpLocalShowSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext     *seq_ctx_p = NULL;
        LacpLocalShowMessage           *req_msg_p = NULL,
                                        *resp_msg_p = NULL,
                                        *sw_resp_p = NULL;

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::LacpLocalShowSendToClusterCallbac\n");

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<LacpLocalShowMessage *>
                    (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<LacpLocalShowMessage *>
                     (ctx_p->getPPrismMessageForPhase1());

        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus ) {
            vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
            UI32    i = 0;

            for (i = 0; i < locations.size(); i++) {
                ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locations[i]);
                if(WAVE_MESSAGE_SUCCESS == statusPerLocationId) {
                    sw_resp_p = dynamic_cast<LacpLocalShowMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
                    req_msg_p->copyAllBuffers(*sw_resp_p);
                } else {
                     trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::LacpLocalShowSendToClusterCallback: Location: ") + locations[i] + string(" ==> error: ") + FrameworkToolKit::localize(statusPerLocationId));
                }
            }
        } else {
            trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::LacpLocalShowSendToClusterCallback: Error: ") + FrameworkToolKit::localize(sendToClusterCompletionStatus));
        }

        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }

    void
    IntfConfLocalWorker::sendLacpShowCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
    {
        LacpLocalShowMessage        *msg_p = NULL;
        LacpLocalShowMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext     *send_ctx_p = NULL;
	      UI32                        locationId = 0;
        SI32                        mappedId = 0;
        vector<UI32>                locations;
    	bool                        sendToLocal = false;

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::sendLacpShowCommandToCluster\n");
        msg_p = dynamic_cast<LacpLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());

        loc_msg_p = new LacpLocalShowMessage (msg_p->getIfName(),
                                             msg_p->getPoNum(),
                                             msg_p->getCmdCode(),
                                             false);
		loc_msg_p->setIfType(msg_p->getIfType());
		loc_msg_p->setStartPoId(msg_p->getStartPoId());
		loc_msg_p->setEndPoId(msg_p->getEndPoId());

        trace (TRACE_LEVEL_DEBUG, string("Sending Show Command to Cluster") +
                        string(" CmdCode =") + msg_p->getCmdCode() +
                        string (" PoNum =")  + msg_p->getPoNum() +
                        string (" ifName =")  + msg_p->getIfName() +
						string (" false & ifType =") + msg_p->getIfType() +
						string (" start PoId =") + msg_p->getStartPoId() +
						string (" end PoId =") + msg_p->getEndPoId());

        send_ctx_p = new WaveSendToClusterContext(this,
                reinterpret_cast<PrismAsynchronousCallback>
                (&IntfConfLocalWorker::LacpLocalShowSendToClusterCallback),
                seq_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        // get locations from message and set to WaveSendToClusterContext
        vector<UI32> *destLocs = msg_p->getDestinationLocations();
        if (destLocs == NULL) {
            NSM_PLUG_DBG("Got NULL destination locations vector.");
        } else if (destLocs->size() > 0) {
        	trace (TRACE_LEVEL_DEBUG, "IntfConfLocalWorker::sendLacpShowCommandToCluster. Got valid destination locations vector. Sending to all locations");
        	send_ctx_p->setLocationsToSendToForPhase1(*destLocs);
        }

        if (0 == msg_p->getIfName().length()) {
        	trace (TRACE_LEVEL_DEBUG, string("Sending Show Command to Cluster getIfName().length == zero"));
            /* If the interface context is not there, go to the local only
             * if it is LLDP_MGMT_CMD_SHOW_GBL
             */
        } else {
            if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  {
        		trace (TRACE_LEVEL_DEBUG, string("Sending Show Command to Cluster isValidTwoTuple true"));
                sendToLocal = true;
            } else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  {
                /* Send to the targetted LocationId pointed to by the
                 * mappedId.
                 */
        		trace (TRACE_LEVEL_DEBUG, string("Sending Show Command to Cluster isValidThreeTuple true"));
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
		if (locationId == 0) {
		    delete send_ctx_p;
 		    delete loc_msg_p;
		    seq_ctx_p->executeNextStep(NSM_INTERFACE_DOES_NOT_EXIST);
                    return;
		}
                locations.push_back(locationId);
                send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
        }

        if (sendToLocal) {
            /* It is not interface specific command or the interface is
             * two tuple one on the local switch.
             */
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) {
               locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }


        sendToWaveCluster(send_ctx_p);

        return;
    }

    ResourceId
    IntfConfLocalWorker::sendShowToLacpDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        LacpLocalShowMessage *msg_p = dynamic_cast<LacpLocalShowMessage *>
                                      (ctx_p->getPPrismMessage ());
        LacpShowMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
		string          ifName = "";

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::sendShowToIgmpsDaemon");
        trace(TRACE_LEVEL_DEBUG, string("IntfConfLocalWorker::sendShowToLacpDaemon::PNum=")
						+ msg_p->getPoNum() + string(" IfName=") + msg_p->getIfName() + string(" CmdCode=") + msg_p->getCmdCode() + string (" ifType=") + msg_p->getIfType());
        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) {
            mappedId = 0;
        }

        if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) {
            NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
        } else {
            ifName = msg_p->getIfName();
        }

        cl_msg_p = new LacpShowMessage(ifName, msg_p->getPoNum(), msg_p->getCmdCode(), mappedId);
		cl_msg_p->setIfType(msg_p->getIfType());
		cl_msg_p->setStartPoId(msg_p->getStartPoId());
		cl_msg_p->setEndPoId(msg_p->getEndPoId());

        status = sendSynchronouslyToWaveClient("lacp", cl_msg_p);
        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR,
                        string("Resp: Client Completion Error MsgId: ") +
                        cl_msg_p->getMessageId() +
                       FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                //bpatel: To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;
                return status;
            }

            if (cl_msg_p->getClientStatus() != 0) {
                trace(TRACE_LEVEL_ERROR, string("Resp:  Client Error MsgId: ") +
                                  cl_msg_p->getMessageId() +
                FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                //bpatel: To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;
                return status;
            }

        }

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG,
            string("Client Message sending failed for MsgId: ") +
                    cl_msg_p->getMessageId() +
                    string("reason:") +  FrameworkToolKit::localize(status));
             //bpatel: To avoid memory leak might be a change
            delete cl_msg_p;
            return status;
        }

        trace(TRACE_LEVEL_DEBUG,
                string("Client Message processing succesful for MsgId: ") +
                cl_msg_p->getMessageId());

        msg_p->copyAllBuffers(*cl_msg_p);
		//bpatel: To avoid memory leak
        status = cl_msg_p->getCompletionStatus();
        delete cl_msg_p;
        return status;
    }

    void
    IntfConfLocalWorker::LacpLocalShowMessageHandler (
            LacpLocalShowMessage *pLacpLocalShowMessage)
    {

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::LacpLocalShowMessageHandler\n");
        if (pLacpLocalShowMessage->getIsGwRequest()) {
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::sendLacpShowCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext =
                new PrismLinearSequencerContext(pLacpLocalShowMessage,
                    this, sequencerSteps,
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&IntfConfLocalWorker::sendShowToLacpDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
                };

            PrismSynchronousLinearSequencerContext
                *pPrismSynchronousLinearSequencerContext=
                new PrismSynchronousLinearSequencerContext(
                        pLacpLocalShowMessage, this, sequencerSteps,
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }

	void IntfConfLocalWorker::removeVlanClassifierOnPhy(const ObjectId &vlanObjectId)
    {
       NSM_PLUG_DBG("Entering ...");
       string parentClass = PhyIntfLocalManagedObject::getClassName();
       string childClass = VlanClassifierActivateLocalManagedObject::getClassName();
       string compositionName = "vlanClassifierActivate";
	   vector<string> selectFields;

	   selectFields.push_back ("objectId");
	   selectFields.push_back ("ownerManagedObjectId");

       
       WaveManagedObjectSynchronousQueryContext syncDeActivateQueryCtxt(VlanClassifierActivateLocalManagedObject::getClassName());
       syncDeActivateQueryCtxt.addAndAttribute(new AttributeObjectId(vlanObjectId, "vlan"));
       syncDeActivateQueryCtxt.addSelectFields(selectFields);

       vector<WaveManagedObject * >*               pDeActivateResults                   =  querySynchronously(&syncDeActivateQueryCtxt);


	   if ((pDeActivateResults-> size())) {

       for(unsigned int j = 0;j < pDeActivateResults -> size() ;j++)
       {
           NSM_PLUG_DBG_S(string("j = ") + j);
           VlanClassifierActivateLocalManagedObject *pVlanClassifierActivateLocalManagedObject = dynamic_cast<VlanClassifierActivateLocalManagedObject*>((*pDeActivateResults)[j]);
           deleteFromComposition(parentClass, childClass, compositionName,
                           pVlanClassifierActivateLocalManagedObject->getOwnerManagedObjectId(),
                           pVlanClassifierActivateLocalManagedObject->getObjectId());
       }
	   }
       
	   if(pDeActivateResults)
       	   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pDeActivateResults);
	   return ;
    }

     ResourceId IntfConfLocalWorker::getPhyMoByPhyName(string phyName, PhyIntfLocalManagedObject * * ppPhyIntfLocalManagedObject)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());

        *ppPhyIntfLocalManagedObject = NULL;
        syncQueryCtxt.addAndAttribute(new AttributeString(phyName, "id"));
        vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);
        if (pResults && (pResults -> size() == 1))
        {
            *ppPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);
            status                       = WAVE_MESSAGE_SUCCESS;
			pResults->clear();
			delete pResults;
        }
/*
    	if(pResults)
       	   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
*/

        return status;
    }


    ResourceId IntfConfLocalWorker::setAllPhyMo_PortRole_Edge()
    {

        ResourceId status = WAVE_MESSAGE_ERROR;
        LocationId                          locationId = FrameworkToolKit::getThisLocationId ();


        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (WaveNode::getClassName());
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (locationId, "locationId"));
        vector<WaveManagedObject *> *pResultsWaveNodes = querySynchronously (&synchronousQueryContext);

        if (NULL == pResultsWaveNodes)
        {
            NSM_PLUG_DBG_S( string("No wave node found for this location id"));
            prismAssert (false, __FILE__, __LINE__);
        }

        if (1 != pResultsWaveNodes->size ())
        {
            NSM_PLUG_DBG_S( string("No of Node found ") + pResultsWaveNodes->size ());
            prismAssert (false, __FILE__, __LINE__);
        }

        WaveNode *pWaveNode = dynamic_cast<WaveNode *>((*pResultsWaveNodes)[0]);
        ObjectId objectId = pWaveNode->getObjectId ();



        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeObjectId (objectId, "ownerWaveNodeObjectId"));
        vector<WaveManagedObject * >* pPhyResults = querySynchronously(&syncQueryCtxt);

        NSM_PLUG_INFO_S( string("Reset All Physical interface state as Edge... LocationId :" ) + locationId) ;

        if (pPhyResults && pPhyResults->size() > 0) {
            startTransaction();
            for (UI32 PhyIndex = 0; PhyIndex < pPhyResults->size(); PhyIndex++) {
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject*>((*pPhyResults)[PhyIndex]);
                updateWaveManagedObject( pPhyIntfLocalManagedObject);
				//By default we are keeping all ports as non-isl and active`
                pPhyIntfLocalManagedObject->setPortrole(false);
				pPhyIntfLocalManagedObject->setActive(true);
            }
            commitTransaction();
        }

        if(pPhyResults)
           WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pPhyResults);

        if(pResultsWaveNodes)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsWaveNodes);

        return status;
    }

    void
    IntfConfLocalWorker::Dot1XLocalShowSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext     *seq_ctx_p = NULL;
        Dot1XLocalShowMessage           *req_msg_p = NULL,
                                        *resp_msg_p = NULL,
                                        *sw_resp_p = NULL;

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::Dot1XLocalShowSendToClusterCallbac\n");
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<Dot1XLocalShowMessage *>
                    (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<Dot1XLocalShowMessage *>
                     (ctx_p->getPPrismMessageForPhase1());

        if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus )
        {
			vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
			UI32    i = 0;

			for (i = 0; i < locations.size(); i++) {
				ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locations[i]);
				if(WAVE_MESSAGE_SUCCESS == statusPerLocationId)
				{
				    sw_resp_p = dynamic_cast<Dot1XLocalShowMessage *>
					(ctx_p->getResultingMessageForPhase1(locations[i]));

					req_msg_p->copyAllBuffers(*sw_resp_p);
				} else {
					 trace(TRACE_LEVEL_ERROR, string("Response processing: The location: ")
								+ locations[i] + string("returned error: ")  +
					 FrameworkToolKit::localize(statusPerLocationId));
				}
			}
        }
        else
        {
        	NSM_PLUG_DBG_ERR_S(string("returned error: ")  + FrameworkToolKit::localize(sendToClusterCompletionStatus));
        }

        NSM_PLUG_INFO_S(string(": MC return status: ") + sendToClusterCompletionStatus);
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }

    void
    IntfConfLocalWorker::sendDot1XShowCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
    {
        Dot1XLocalShowMessage        *msg_p = NULL;
        Dot1XLocalShowMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext     *send_ctx_p = NULL;
	    UI32                        locationId = 0;
        SI32                        mappedId = 0;
        vector<UI32>                locations;
    	bool                        sendToLocal = false;

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::sendDot1XShowCommandToCluster\n");
        msg_p = dynamic_cast<Dot1XLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());

        loc_msg_p = new Dot1XLocalShowMessage (msg_p->getInterface(),
                                             msg_p->getIfName(),
                                             msg_p->getCmdCode(),
                                             false);

        trace (TRACE_LEVEL_DEBUG, string("Sending Show Command to Cluster") +
                        string(" CmdCode =") + msg_p->getCmdCode() +
                        string (" getInterface =")  + msg_p->getInterface() +
                        string (" ifName =")  + msg_p->getIfName() +
						string (" false"));

        send_ctx_p = new WaveSendToClusterContext(this,
                reinterpret_cast<PrismAsynchronousCallback>
                (&IntfConfLocalWorker::Dot1XLocalShowSendToClusterCallback),
                seq_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        if (0 == msg_p->getIfName().length()) {
        	trace (TRACE_LEVEL_DEBUG, string("Sending Show Command to Cluster getIfName().length == zero"));
			if(msg_p->getCmdCode()== DOT1X_CMD_SHOW){
                sendToLocal = true;
            }
			else /*send to all the nodes in the cluster*/
			{
				sendToLocal = false;
            }
        } else {
            if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  {
        		trace (TRACE_LEVEL_DEBUG, string("Sending Show Command to Cluster isValidTwoTuple true"));
                sendToLocal = true;
            } else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  {
                /* Send to the targetted LocationId pointed to by the
                 * mappedId.
                 */
        		trace (TRACE_LEVEL_DEBUG, string("Sending Show Command to Cluster isValidThreeTuple true"));
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
		if (locationId == 0) {
		    delete send_ctx_p;
 		    delete loc_msg_p;
		    seq_ctx_p->executeNextStep(AUTH_API_SET_ERR_IF_NOT_EXIST);
                    return;
		}
                locations.push_back(locationId);
                send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
        }

        if (sendToLocal) {
            /* It is not interface specific command or the interface is
             * two tuple one on the local switch.
             */
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) {
               locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }


        sendToWaveCluster(send_ctx_p);

        return;
    }

    ResourceId
    IntfConfLocalWorker::sendShowToDot1XDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        Dot1XLocalShowMessage *msg_p = dynamic_cast<Dot1XLocalShowMessage *>
                                      (ctx_p->getPPrismMessage ());
        Dot1XShowMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
		string          ifName = "";

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::sendShowToDot1XDaemon");
        trace(TRACE_LEVEL_DEBUG, string("IntfConfLocalWorker::sendShowToDot1XDaemon::getInterface=")
						+ msg_p->getInterface() + string(" IfName=") + msg_p->getIfName() + string(" CmdCode=") + msg_p->getCmdCode());
        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) {
            mappedId = 0;
        }

        if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) {
            NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
        } else {
            ifName = msg_p->getIfName();
        }

        cl_msg_p = new Dot1XShowMessage(msg_p->getInterface(), ifName, msg_p->getCmdCode(), mappedId);

        status = sendSynchronouslyToWaveClient("dauthd", cl_msg_p);
        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR,
                        string("Resp: Client Completion Error MsgId: ") +
                        cl_msg_p->getMessageId() +
                       FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                //bpatel : To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;
                return status;
            }

            if (cl_msg_p->getClientStatus() != 0) {
                trace(TRACE_LEVEL_ERROR, string("Resp:  Client Error MsgId: ") +
                                  cl_msg_p->getMessageId() +
                FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                //bpatel : To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;
                return status;
            }

        }

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG,
            string("Client Message sending failed for MsgId: ") +
                    cl_msg_p->getMessageId() +
                    string("reason:") +  FrameworkToolKit::localize(status));
             //bpatel: to avoid memory leak
            delete cl_msg_p;
			return status;
        }

        trace(TRACE_LEVEL_DEBUG,
                string("Client Message processing succesful for MsgId: ") +
                cl_msg_p->getMessageId());

        msg_p->copyAllBuffers(*cl_msg_p);
		//bpatel: to avoid memory leak
        status = cl_msg_p->getCompletionStatus();
        delete cl_msg_p;
        return status;
    }

    void
    IntfConfLocalWorker::Dot1XLocalShowMessageHandler (
            Dot1XLocalShowMessage *pDot1XLocalShowMessage)
    {

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::Dot1XLocalShowMessageHandler\n");
        if (pDot1XLocalShowMessage->getIsGwRequest()) {
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::sendDot1XShowCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext =
                new PrismLinearSequencerContext(pDot1XLocalShowMessage,
                    this, sequencerSteps,
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&IntfConfLocalWorker::sendShowToDot1XDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
                };

            PrismSynchronousLinearSequencerContext
                *pPrismSynchronousLinearSequencerContext=
                new PrismSynchronousLinearSequencerContext(
                        pDot1XLocalShowMessage, this, sequencerSteps,
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }

	void
    IntfConfLocalWorker::Dot1XLocalReauthenticateMessageHandler (
            Dot1XLocalReauthenticateMessage *pDot1XLocalReauthenticateMessage)
    {

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::Dot1XLocalReauthenticateMessageHandler\n");
        if (pDot1XLocalReauthenticateMessage->getIsGwRequest())
		{
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::sendDot1XReauthenticateCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext =
                new PrismLinearSequencerContext(pDot1XLocalReauthenticateMessage,
                    this, sequencerSteps,
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        }
		else
		{
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&IntfConfLocalWorker::sendReauthenticateToDot1XDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
                };

            PrismSynchronousLinearSequencerContext
                *pPrismSynchronousLinearSequencerContext=
                new PrismSynchronousLinearSequencerContext(pDot1XLocalReauthenticateMessage,
					this, sequencerSteps,
                    sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }

	void
    IntfConfLocalWorker::Dot1XLocalTestMessageHandler (
            Dot1XLocalTestMessage *pDot1XLocalTestMessage)
	{

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::Dot1XLocalTestMessageHandler\n");
        if (pDot1XLocalTestMessage->getIsGwRequest())
		{
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::sendDot1XTestCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext =
                new PrismLinearSequencerContext(pDot1XLocalTestMessage,
                    this, sequencerSteps,
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        }
		else
		{
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&IntfConfLocalWorker::sendTestToDot1XDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
                };

            PrismSynchronousLinearSequencerContext
                *pPrismSynchronousLinearSequencerContext=
                new PrismSynchronousLinearSequencerContext(pDot1XLocalTestMessage,
					this, sequencerSteps,
                    sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }

	void IntfConfLocalWorker::Dot1XLocalClearMessageHandler (
            Dot1XLocalClearMessage *pDot1XLocalClearMessage)
    {

    	trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::Dot1XLocalClearMessageHandler\n");
        if (pDot1XLocalClearMessage->getIsGwRequest())
        {
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::sendDot1XClearCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext =
                new PrismLinearSequencerContext(pDot1XLocalClearMessage,
                    this, sequencerSteps,
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        }
        else
        {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&IntfConfLocalWorker::sendClearToDot1XDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
                };

            PrismSynchronousLinearSequencerContext
                *pPrismSynchronousLinearSequencerContext=
                new PrismSynchronousLinearSequencerContext(pDot1XLocalClearMessage,
                    this, sequencerSteps,
                    sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }


	void
    IntfConfLocalWorker::sendDot1XReauthenticateCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
    {
        Dot1XLocalReauthenticateMessage        *msg_p = NULL;
        Dot1XLocalReauthenticateMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext     *send_ctx_p = NULL;
        UI32                        locationId = 0;
        SI32                        mappedId = 0;
        vector<UI32>                locations;
        bool                        sendToLocal = false;

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::sendDot1XReauthenticateCommandToCluster\n");
        msg_p = dynamic_cast<Dot1XLocalReauthenticateMessage *> (seq_ctx_p->getPPrismMessage ());

        loc_msg_p = new Dot1XLocalReauthenticateMessage (msg_p->getInterface(),
                                             msg_p->getIfName(),
                                             msg_p->getCmdCode(),
                                             false);

        trace (TRACE_LEVEL_DEBUG, string("Sending Reauthenticate Command to Cluster") +
                        string(" CmdCode =") + msg_p->getCmdCode() +
                        string (" getInterface =")  + msg_p->getInterface() +
                        string (" ifName =")  + msg_p->getIfName() +
                        string (" false"));

        send_ctx_p = new WaveSendToClusterContext(this,
                reinterpret_cast<PrismAsynchronousCallback>
                (&IntfConfLocalWorker::Dot1XLocalReauthenticateSendToClusterCallback),
                seq_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        if (0 == msg_p->getIfName().length()) {
            trace (TRACE_LEVEL_DEBUG, string("Sending Reauthenticate Command to Cluster getIfName().length == zero"));
            /* If the interface context is not there, go to the local only
             */

            if ((msg_p->getCmdCode() >= 0) && (msg_p->getCmdCode() < DOT1X_CMDCODE_MAX)) {
                sendToLocal = true;
            }
        } else {
            if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  {
                trace (TRACE_LEVEL_DEBUG, string("Sending Reauthenticate Command to Cluster isValidTwoTuple true"));
                sendToLocal = true;
            } else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  {
                /* Send to the targetted LocationId pointed to by the
                 * mappedId.
                 */
                trace (TRACE_LEVEL_DEBUG, string("Sending Reauthenticate Command to Cluster isValidThreeTuple true"));
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
		if (locationId == 0) {
		    delete send_ctx_p;
 		    delete loc_msg_p;
		    seq_ctx_p->executeNextStep(NSM_INTERFACE_DOES_NOT_EXIST);
                    return;
		}
                locations.push_back(locationId);
                send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
        }

        if (sendToLocal) {
            /* It is not interface specific command or the interface is
             * two tuple one on the local switch.
             */
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) {
               locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }


        sendToWaveCluster(send_ctx_p);

        return;
    }

	void
    IntfConfLocalWorker::sendDot1XTestCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
	{
        Dot1XLocalTestMessage        *msg_p = NULL;
        Dot1XLocalTestMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext     *send_ctx_p = NULL;
        UI32                        locationId = 0;
        SI32                        mappedId = 0;
        vector<UI32>                locations;
        bool                        sendToLocal = false;

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::sendDot1XTestCommandToCluster\n");
        msg_p = dynamic_cast<Dot1XLocalTestMessage *> (seq_ctx_p->getPPrismMessage ());

        loc_msg_p = new Dot1XLocalTestMessage (msg_p->getInterface(),
                                             msg_p->getIfName(),
                                             msg_p->getCmdCode(),
                                             false);

        trace (TRACE_LEVEL_DEBUG, string("Sending Test Command to Cluster") +
                        string(" CmdCode =") + msg_p->getCmdCode() +
                        string (" getInterface =")  + msg_p->getInterface() +
                        string (" ifName =")  + msg_p->getIfName() +
                        string (" false"));

        send_ctx_p = new WaveSendToClusterContext(this,
                reinterpret_cast<PrismAsynchronousCallback>
                (&IntfConfLocalWorker::Dot1XLocalTestSendToClusterCallback),
                seq_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        if (0 == msg_p->getIfName().length()) {
            trace (TRACE_LEVEL_DEBUG, string("Sending Test Command to Cluster getIfName().length == zero"));
            /* If the interface context is not there, go to the local only
             */

            if ((msg_p->getCmdCode() >= 0) && (msg_p->getCmdCode() < DOT1X_CMDCODE_MAX)) {
                sendToLocal = true;
            }
		} else {
			if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  {
				trace (TRACE_LEVEL_DEBUG, string("Sending Test Command to Cluster isValidTwoTuple true"));
				sendToLocal = true;
			} else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  {
				/* Send to the targetted LocationId pointed to by the
				 * mappedId.
				 */
				trace (TRACE_LEVEL_DEBUG, string("Sending Test Command to Cluster isValidThreeTuple true"));
				sendToLocal = false;
				mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
				locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
				if (locationId == 0) {
					delete send_ctx_p;
					delete loc_msg_p;
					seq_ctx_p->executeNextStep(NSM_INTERFACE_DOES_NOT_EXIST);
					return;
				}
				locations.push_back(locationId);
				send_ctx_p->setLocationsToSendToForPhase1(locations);
			}
		}

        if (sendToLocal) {
            /* It is not interface specific command or the interface is
             * two tuple one on the local switch.
             */
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) {
               locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }


        sendToWaveCluster(send_ctx_p);

        return;
    }

	void
    IntfConfLocalWorker::sendDot1XClearCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
    {
        Dot1XLocalClearMessage        *msg_p = NULL;
        Dot1XLocalClearMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext     *send_ctx_p = NULL;
        UI32                        locationId = 0;
        SI32                        mappedId = 0;
        vector<UI32>                locations;
        bool                        sendToLocal = false;

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::sendDot1XClearCommandToCluster\n");
        msg_p = dynamic_cast<Dot1XLocalClearMessage *> (seq_ctx_p->getPPrismMessage ());

        loc_msg_p = new Dot1XLocalClearMessage (msg_p->getInterface(),
                                             msg_p->getIfName(),
                                             msg_p->getCmdCode(),
                                             false);

        trace (TRACE_LEVEL_DEBUG, string("Sending Clear Command to Cluster") +
                        string(" CmdCode =") + msg_p->getCmdCode() +
                        string (" getInterface =")  + msg_p->getInterface() +
                        string (" ifName =")  + msg_p->getIfName() +
                        string (" false"));

        send_ctx_p = new WaveSendToClusterContext(this,
                reinterpret_cast<PrismAsynchronousCallback>
                (&IntfConfLocalWorker::Dot1XLocalClearSendToClusterCallback),
                seq_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        if (0 == msg_p->getIfName().length()) {
            trace (TRACE_LEVEL_DEBUG, string("Sending Clear Command to Cluster getIfName().length == zero"));
                sendToLocal = false;
        } else {
            if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  {
                trace (TRACE_LEVEL_DEBUG, string("Sending Clear Command to Cluster isValidTwoTuple true"));
                sendToLocal = true;
            } else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  {
                /* Send to the targetted LocationId pointed to by the
                 * mappedId.
                 */
                trace (TRACE_LEVEL_DEBUG, string("Sending CLear Command to Cluster isValidThreeTuple true"));
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
		if (locationId == 0) {
		    delete send_ctx_p;
 		    delete loc_msg_p;
		    seq_ctx_p->executeNextStep(NSM_INTERFACE_DOES_NOT_EXIST);
                    return;
		}
                locations.push_back(locationId);
                send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
        }

        if (sendToLocal) {
            /* It is not interface specific command or the interface is
             * two tuple one on the local switch.
             */
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) {
 				locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }


        sendToWaveCluster(send_ctx_p);

        return;
    }

 	void
    IntfConfLocalWorker::Dot1XLocalReauthenticateSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext     *seq_ctx_p = NULL;
        Dot1XLocalReauthenticateMessage           *req_msg_p = NULL,
                                        *resp_msg_p = NULL,
                                        *sw_resp_p = NULL;

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::Dot1XLocalReauthenticateSendToClusterCallbac\n");

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<Dot1XLocalReauthenticateMessage *>
                    (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<Dot1XLocalReauthenticateMessage *>
                     (ctx_p->getPPrismMessageForPhase1());

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        for (i = 0; i < locations.size(); i++) {
            sw_resp_p = dynamic_cast<Dot1XLocalReauthenticateMessage *>
                (ctx_p->getResultingMessageForPhase1(locations[i]));
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } else {
                 trace(TRACE_LEVEL_ERROR, string("Response processing: The location: ")
                            + locations[i] + string("returned error: ")  +
                 FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
            }
        }

        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }

    void
    IntfConfLocalWorker::Dot1XLocalTestSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
	{
        PrismLinearSequencerContext     *seq_ctx_p = NULL;
        Dot1XLocalTestMessage           *req_msg_p = NULL,
                                        *resp_msg_p = NULL,
                                        *sw_resp_p = NULL;

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::Dot1XLocalTestSendToClusterCallbac\n");

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<Dot1XLocalTestMessage *>
                    (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<Dot1XLocalTestMessage *>
                     (ctx_p->getPPrismMessageForPhase1());

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        for (i = 0; i < locations.size(); i++) {
            sw_resp_p = dynamic_cast<Dot1XLocalTestMessage *>
                (ctx_p->getResultingMessageForPhase1(locations[i]));
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } else {
                 trace(TRACE_LEVEL_ERROR, string("Response processing: The location: ")
                            + locations[i] + string("returned error: ")  +
                 FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
            }
        }

        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }

    void
    IntfConfLocalWorker::Dot1XLocalClearSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext     *seq_ctx_p = NULL;
        Dot1XLocalClearMessage           *req_msg_p = NULL,
                                        *resp_msg_p = NULL,
                                        *sw_resp_p = NULL;

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::Dot1XLocalClearSendToClusterCallbac\n");

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<Dot1XLocalClearMessage *>
                    (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<Dot1XLocalClearMessage *>
                     (ctx_p->getPPrismMessageForPhase1());

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        for (i = 0; i < locations.size(); i++) {
            sw_resp_p = dynamic_cast<Dot1XLocalClearMessage *>
                (ctx_p->getResultingMessageForPhase1(locations[i]));
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } else {
                 trace(TRACE_LEVEL_ERROR, string("Response processing: The location: ")
                            + locations[i] + string("returned error: ")  +
                 FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
            }
        }

        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }

	ResourceId
	IntfConfLocalWorker::sendReauthenticateToDot1XDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        Dot1XLocalReauthenticateMessage *msg_p = dynamic_cast<Dot1XLocalReauthenticateMessage *>
                                      (ctx_p->getPPrismMessage ());
        Dot1XReauthenticateMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
        string          ifName = "";

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::sendReauthenticateToDot1XDaemon");
        trace(TRACE_LEVEL_DEBUG, string("IntfConfLocalWorker::sendReauthenticateToDot1XDaemon::getInterface=")
                        + msg_p->getInterface() + string(" IfName=") + msg_p->getIfName() + string(" CmdCode=") + msg_p->getCmdCode());
        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) {
            mappedId = 0;
        }

        if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) {
            NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
        } else {
            ifName = msg_p->getIfName();
        }

        cl_msg_p = new Dot1XReauthenticateMessage(msg_p->getInterface(), ifName, msg_p->getCmdCode(), mappedId);

        status = sendSynchronouslyToWaveClient("dauthd", cl_msg_p);
        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR,
                        string("Resp: Client Completion Error MsgId: ") +
                        cl_msg_p->getMessageId() +
                       FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                //bpatel : To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;
                return status;
            }

            if (cl_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR, string("Resp:  Client Error MsgId: ") +
                                  cl_msg_p->getMessageId() +
                FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                //bpatel : To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;
                return status;
            }

        }

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG,
            string("Client Message sending failed for MsgId: ") +
                    cl_msg_p->getMessageId() +
                    string("reason:") +  FrameworkToolKit::localize(status));
             //bpatel: to avoid memory leak
            delete cl_msg_p;
            return status;
	}

        trace(TRACE_LEVEL_DEBUG,
                string("Client Message processing succesful for MsgId: ") +
                cl_msg_p->getMessageId());

        msg_p->copyAllBuffers(*cl_msg_p);
        //bpatel: to avoid memory leak
        status = cl_msg_p->getCompletionStatus();
        delete cl_msg_p;
        return status;
    }


	ResourceId
	IntfConfLocalWorker::sendTestToDot1XDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
	{
        Dot1XLocalTestMessage *msg_p = dynamic_cast<Dot1XLocalTestMessage *>
                                      (ctx_p->getPPrismMessage ());
        Dot1XTestMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
        string          ifName = "";

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::sendTestToDot1XDaemon");
        trace(TRACE_LEVEL_DEBUG, string("IntfConfLocalWorker::sendTestToDot1XDaemon::getInterface=")
                        + msg_p->getInterface() + string(" IfName=") + msg_p->getIfName() + string(" CmdCode=") + msg_p->getCmdCode());
        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) {
            mappedId = 0;
        }

        if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) {
            NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
        } else {
            ifName = msg_p->getIfName();
        }

        cl_msg_p = new Dot1XTestMessage(msg_p->getInterface(), ifName, msg_p->getCmdCode(), mappedId);

        status = sendSynchronouslyToWaveClient("dauthd", cl_msg_p);
        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR,
                        string("Resp: Client Completion Error MsgId: ") +
                        cl_msg_p->getMessageId() +
                       FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                //bpatel : To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;
                return status;
            }

            if (cl_msg_p->getClientStatus() != 0) {
                trace(TRACE_LEVEL_ERROR, string("Resp:  Client Error MsgId: ") +
                                  cl_msg_p->getMessageId() +
                FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                //bpatel : To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;
                return status;
            }

        }

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG,
            string("Client Message sending failed for MsgId: ") +
                    cl_msg_p->getMessageId() +
                    string("reason:") +  FrameworkToolKit::localize(status));
             //bpatel: to avoid memory leak
            delete cl_msg_p;
            return status;
	}

        trace(TRACE_LEVEL_DEBUG,
                string("Client Message processing succesful for MsgId: ") +
                cl_msg_p->getMessageId());

        msg_p->copyAllBuffers(*cl_msg_p);
        //bpatel: to avoid memory leak
        status = cl_msg_p->getCompletionStatus();
        delete cl_msg_p;
        return status;
    }


	ResourceId
    IntfConfLocalWorker::sendClearToDot1XDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        Dot1XLocalClearMessage *msg_p = dynamic_cast<Dot1XLocalClearMessage *>
                                      (ctx_p->getPPrismMessage ());
        Dot1XClearMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
        string          ifName = "";

        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfLocalWorker::sendClearToDot1XDaemon");
        trace(TRACE_LEVEL_DEBUG, string("IntfConfLocalWorker::sendClearToDot1XDaemon::getInterface=")
                        + msg_p->getInterface() + string(" IfName=") + msg_p->getIfName() + string(" CmdCode=") + msg_p->getCmdCode());
        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) {
            mappedId = 0;
        }

        if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) {
            NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
        } else {
            ifName = msg_p->getIfName();
        }

        cl_msg_p = new Dot1XClearMessage(msg_p->getInterface(), ifName, msg_p->getCmdCode(), mappedId);

        status = sendSynchronouslyToWaveClient("dauthd", cl_msg_p);
        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR,
                        string("Resp: Client Completion Error MsgId: ") +
                        cl_msg_p->getMessageId() +
                       FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                //bpatel : To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;
                return status;
            }

            if (cl_msg_p->getClientStatus() != 0) {
                trace(TRACE_LEVEL_ERROR, string("Resp:  Client Error MsgId: ") +
                                  cl_msg_p->getMessageId() +
                FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                //bpatel : To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;
                return status;
            }

        }

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG,
            string("Client Message sending failed for MsgId: ") +
                    cl_msg_p->getMessageId() +
                    string("reason:") +  FrameworkToolKit::localize(status));
             //bpatel: to avoid memory leak
            delete cl_msg_p;
            return status;
    	}

        trace(TRACE_LEVEL_DEBUG,
                string("Client Message processing succesful for MsgId: ") +
                cl_msg_p->getMessageId());

        msg_p->copyAllBuffers(*cl_msg_p);
        //bpatel: to avoid memory leak
        status = cl_msg_p->getCompletionStatus();
        delete cl_msg_p;
        return status;
    }

	void
    IntfConfLocalWorker::LacpLocalClearSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext             *seq_ctx_p = NULL;
        LacpLocalClearMessage                    *req_msg_p = NULL,
                                                *resp_msg_p = NULL,
                                                *sw_resp_p = NULL;
		ResourceId sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<LacpLocalClearMessage *>
                    (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<LacpLocalClearMessage *>
                     (ctx_p->getPPrismMessageForPhase1());

        NSM_PLUG_ASSERT_PTR(req_msg_p);
        NSM_PLUG_ASSERT_PTR(resp_msg_p);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        NSM_PLUG_INFO_S(string("Response processing: Num resp:")
                       + locations.size());

        for (i = 0; i < locations.size(); i++) {
            if (i ==0) {
                NSM_PLUG_INFO_S(string("locations:") + locations[i]);
            } else {
                NSM_PLUG_INFO_S(string(":") + locations[i]);
            }
        }

        for (i = 0; i < locations.size(); i++) {
            sw_resp_p = dynamic_cast<LacpLocalClearMessage *>
                (ctx_p->getResultingMessageForPhase1(locations[i]));
            NSM_PLUG_ASSERT_PTR(sw_resp_p);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
                NSM_PLUG_INFO_S(string("WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus() "));
//		        req_msg_p->copyAllBuffers(*sw_resp_p);
            } else {
                 NSM_PLUG_DBG_ERR_S(string("Response processing: The location: ")
                            + locations[i] + string("returned error: ")  +
                 FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
				 sendToClusterCompletionStatus = sw_resp_p->getCompletionStatus();
				 req_msg_p->setCompletionStatus(sw_resp_p->getCompletionStatus());

            }
        }

        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }


    void
    IntfConfLocalWorker::sendLacpClearCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
    {
        LacpLocalClearMessage        *msg_p = NULL;
        LacpLocalClearMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext    *send_ctx_p = NULL;

        msg_p = dynamic_cast<LacpLocalClearMessage *>
            (seq_ctx_p->getPPrismMessage ());

        NSM_PLUG_ASSERT_PTR(msg_p);


        loc_msg_p = new LacpLocalClearMessage(msg_p->getPortchannelNumber(),
                                             msg_p->getCmdCode(),
                                             false);
        NSM_PLUG_ASSERT_PTR(loc_msg_p);

        NSM_PLUG_INFO_S(string("Sending Clear Command to Cluster") +
                        string(" CmdCode =") + msg_p->getCmdCode() +
                        string ("ifName =")  + msg_p->getPortchannelNumber());

        send_ctx_p = new WaveSendToClusterContext(this,
                reinterpret_cast<PrismAsynchronousCallback>
                (&IntfConfLocalWorker::LacpLocalClearSendToClusterCallback),
                seq_ctx_p);
        NSM_PLUG_ASSERT_PTR(send_ctx_p);

        if (!send_ctx_p) {
            return;
        }

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        if (LACP_CLEAR_PORTCHANNEL_COUNTERS == msg_p->getCmdCode()) {
			int ret = SET_LOCATION_ID_API_ERROR;
			ret = NsmUtils::setLocationIdsForPoSubModeCmds(msg_p->getPortchannelNumber(), send_ctx_p);
			if(ret != SET_LOCATION_ID_API_SUCCESS) {
				NSM_PLUG_DBG_ERR_S(string("Call to setLocationIdsForPoSubModeCmds failed"));
				msg_p->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
				seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
				delete send_ctx_p;
				delete loc_msg_p;
				return;
			}
		}

		sendToWaveCluster(send_ctx_p);

        return;
    }


    ResourceId
    IntfConfLocalWorker::sendClearToLacpDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        LacpLocalClearMessage *msg_p = dynamic_cast<LacpLocalClearMessage *>
                                      (ctx_p->getPPrismMessage ());
        LacpClearMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
        string          ifName = "";

        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) {
            mappedId = 0;
        }

        cl_msg_p = new LacpClearMessage(msg_p->getPortchannelNumber(), msg_p->getCmdCode(), mappedId);

        status = sendSynchronouslyToWaveClient("lacp",
                                               cl_msg_p);

        NSM_PLUG_INFO_S(string("Sending Clear Command to Cluster") +
                        string(" MsgId =")  + cl_msg_p->getMessageId()  +
                        string(" CmdCode =") + cl_msg_p->getCmdCode() +
                        string(" portChannelNumber =")  + cl_msg_p->getPortchannelNumber() +
                        string(" mappedId =") + cl_msg_p->getMappedId());

        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                NSM_PLUG_DBG_ERR_S(
                        string("Resp: Client Completion Error MsgId: ") +
                        cl_msg_p->getMessageId() +
                       FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
				status = msg_p->getCompletionStatus();
				if(cl_msg_p){
					delete cl_msg_p;
				}
				return status;
            }

            if (cl_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
                NSM_PLUG_DBG_ERR_S(string("Resp:  Client Error MsgId: ") +
                                  cl_msg_p->getMessageId() +
                FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                status = msg_p->getCompletionStatus();
				if(cl_msg_p){
					delete cl_msg_p;
				}
                return status;
            }

        }

        if (status != WAVE_MESSAGE_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(
            string("Client Message sending failed for MsgId: ") +
                    cl_msg_p->getMessageId() +
                    string("reason:") +  FrameworkToolKit::localize(status));
			if(cl_msg_p){
				delete msg_p;
			}
			return status;
        }

        NSM_PLUG_INFO_S(
                string("Client Message processing succesful for MsgId: ") +
                cl_msg_p->getMessageId());

//      msg_p->copyAllBuffers(*cl_msg_p);

		status = cl_msg_p->getCompletionStatus();
		delete  cl_msg_p;
		return  status;
    }

    void
    IntfConfLocalWorker::LacpLocalClearMessageHandler (
            LacpLocalClearMessage *pLacpLocalClearMessage)
    {

        if (pLacpLocalClearMessage->getIsGwRequest()) {
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::sendLacpClearCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IntfConfLocalWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext =
                new PrismLinearSequencerContext(pLacpLocalClearMessage,
                    this, sequencerSteps,
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&IntfConfLocalWorker::sendClearToLacpDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
                };

            PrismSynchronousLinearSequencerContext
                *pPrismSynchronousLinearSequencerContext=
                new PrismSynchronousLinearSequencerContext(
                        pLacpLocalClearMessage, this, sequencerSteps,
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }

    ResourceId IntfConfLocalWorker::sendMacRebalanceCommandToNsm(PrismSynchronousLinearSequencerContext *ctx_p) {
        NsmLocalMacRebalanceMessage *msg_p = NULL;
        MacRebalanceMessage *cl_msg_p = NULL;
        ResourceId status;

        trace(TRACE_LEVEL_INFO, "Entering IntfConfLocalWorker::sendMacRebalanceCommandToNsm");


        msg_p = dynamic_cast<NsmLocalMacRebalanceMessage *>(ctx_p->getPPrismMessage ());
        cl_msg_p = new MacRebalanceMessage();
        cl_msg_p->setPoIfIndex(msg_p->getPoIfIndex());

        trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::sendMacRebalanceCommandToNsm::getPoIfIndex=") + msg_p->getPoIfIndex());
        trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::sendMacRebalanceCommandToNsm: sending message to NSM with Port-channel = ") + cl_msg_p->getPoIfIndex());

        status = sendSynchronouslyToWaveClient("nsm", cl_msg_p);

        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR, string("Resp: Client Completion Error MsgId: ") + cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                status = msg_p->getCompletionStatus();
                if (cl_msg_p) {
                    delete cl_msg_p;
                }
                return status;
            }

            if (cl_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR, string("Resp:  Client Error MsgId: ") + cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                status = msg_p->getCompletionStatus();
                if (cl_msg_p) {
                    delete cl_msg_p;
                }
                return status;
            }
        } 
        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_INFO, string("Client Message sending failed for MsgId: ") + cl_msg_p->getMessageId() + string("reason:") +  FrameworkToolKit::localize(status));
            if (cl_msg_p) {
                delete cl_msg_p;
            }
            return status;
        }

        trace(TRACE_LEVEL_INFO, string("Client Message processing succesful for MsgId: ") + cl_msg_p->getMessageId());
        status = cl_msg_p->getCompletionStatus();
        trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::sendMacRebalanceCommandToNsm::Completion Status: ") + status);
        if (cl_msg_p) {
            delete cl_msg_p;
        }
        return status;
    }

    void IntfConfLocalWorker::NsmLocalMacRebalanceMessageHandler(NsmLocalMacRebalanceMessage *pMacRebalanceMsg) {
        trace(TRACE_LEVEL_INFO, string("Entering IntfConfLocalWorker::NsmLocalMacRebalanceMessageHandler... "));
        PrismSynchronousLinearSequencerStep sequencerSteps[] = {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::sendMacRebalanceCommandToNsm),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pMacRebalanceMsg, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismSynchronousLinearSequencerContext->execute ();
    }

void IntfConfLocalWorker::connectorConfigMessageHandler (ConnectorConfigLocalMessage *pConnectorConfigLocalMessage)
{

// Validations and Queries should go here
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::validateConnectorConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::connectorConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::clearSpanConnectorConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::clearL2sysConnectorConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::clearStpPortIdConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::clearFcoeConnectorConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
        // Your configuration change code goes here

        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::connectorConfigMOStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
        // Programming Protocol Daemons goes here

        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
    };

    DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pConnectorConfigLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pDceSynchronousLinearSeqContext->execute ();
}

// This method verifies whether the given [mappedId/]Slot/Linecard exists or not.
// ASSUMPTIONS:
// 1. Connector name is entered as [1/]2/3 format and the linecard number, here, is derived as "2"
// 2. The linecard is an Ethernet card and a corresponding entry of
//    VcsSlotLocalManagedObject already exists, before making an entry
//    for ConnectorConfigLocalManagedObject.
// 3. No validation is made, here, on the connector number (such as 99 in the
//    case of the connector 1/99). We assume NsmLocalObjectManager
//    will have a corresponding entry or PhyIntfLocalManagedObject and
//    an ERROR will be returned when there is no connector.
ResourceId IntfConfLocalWorker::validateConnectorConfigStep (DceSynchronousLinearSeqContext *pContext)
{
    ConnectorConfigLocalMessage *pMsg = dynamic_cast<ConnectorConfigLocalMessage*> (pContext->getPPrismMessage ());
	UI32 linecardNumber = 0, cmdCode = pMsg->getCmdCode();
	SI32 mappedId = DcmToolKit::getLocalMappedId();
	LocationId locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
	ObjectId waveNodeObjectId = FrameworkToolKit::getObjectIdForLocationId (locationId);
	string connectorName = pMsg->getName();
    VcsSlotLocalManagedObject *pSlotMO = NULL; 
   	PhyIntfLocalManagedObject *pPhyMO = NULL; 

    /** Allow configuration when it is coming from file replay specifically during config snapshot restore.**/
    /** Allow configuration when Bulk request comes from EarlyConfigurationReplay **/
    if ((CONNECTOR_CONFIG_BULK_UPDATE == cmdCode) || (FrameworkToolKit::isFileReplayInProgress())) {
        return WAVE_MESSAGE_SUCCESS;
    }

	linecardNumber = (UI32) NsmUtils::getSlotId(connectorName);

	WaveManagedObjectSynchronousQueryContext slotSyncQueryCtxt (VcsSlotLocalManagedObject::getClassName ());
	slotSyncQueryCtxt.addAndAttribute (new AttributeUI32 (linecardNumber, "slotNumber"));
	slotSyncQueryCtxt.addAndAttribute(new AttributeObjectId (waveNodeObjectId , "ownerWaveNodeObjectId"));
	slotSyncQueryCtxt.addSelectField("slotState");
	vector<WaveManagedObject *> *pSlotResults = querySynchronously (&slotSyncQueryCtxt);

	if((pSlotResults != NULL) && (pSlotResults->size() != 0))
	{
		pContext->addManagedObjectsForGarbageCollection(*pSlotResults);
    	pSlotMO =  dynamic_cast<VcsSlotLocalManagedObject*>((*pSlotResults)[0]);
		pSlotResults->clear();
		delete pSlotResults;
		pSlotResults = NULL;
	}

	WaveManagedObjectSynchronousQueryContext phySyncQueryCtxt (PhyIntfLocalManagedObject::getClassName ());
	phySyncQueryCtxt.addAndAttribute (new AttributeString (connectorName, "ifName"));
	phySyncQueryCtxt.addAndAttribute (new AttributeEnum ((UI32)IF_TYPE_FO, "phyType"));
	phySyncQueryCtxt.addSelectField("status");
	phySyncQueryCtxt.addSelectField("poId");
	vector<WaveManagedObject *> *pPhyResults = querySynchronously (&phySyncQueryCtxt);

	if((pPhyResults != NULL) && (pPhyResults->size() != 0))
	{
		pContext->addManagedObjectsForGarbageCollection(*pPhyResults);
    	pPhyMO =  dynamic_cast<PhyIntfLocalManagedObject*>((*pPhyResults)[0]);
		pPhyResults->clear();
		delete pPhyResults;
		pPhyResults = NULL;
	}

	switch(cmdCode)
	{
		case CONNECTOR_CONFIG_SET_NAME:
		{
			//If slot does not exist, return ERROR.
			if(pSlotMO == NULL)
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as the linecard does not exist."));

				return WRC_LINECARD_IS_NOT_INSERTED;
			}

			//If the interface does not exist, return ERROR.
			if(pPhyMO == NULL)
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as the FortyGig interface does not exist."));

				return WRC_NSM_ERR_PHYINTF_INVALID_FORTYGIGE;
			}
		}
		break;

		case CONNECTOR_CONFIG_SET_BREAKOUT:
		{
			//If slot does not exist, return ERROR.
			if(pSlotMO == NULL)
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as the linecard does not exist."));

				return WRC_LINECARD_IS_NOT_INSERTED;
			}

			//If the interface does not exist, return ERROR.
			if(pPhyMO == NULL)
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as the FortyGig interface does not exist."));

				return WRC_NSM_ERR_PHYINTF_INVALID_FORTYGIGE;
			}

			//If this is a mercury platform and the slot is not in offline state, return ERROR.
			if(linecardNumber != 0)
			{
				if(pSlotMO->getSlotState() != 2)
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as the linecard is not powered-off. State is:") + pSlotMO->getSlotState());

					return WRC_LINECARD_SHOULD_BE_IN_OFFLINE_STATE;
				}
                else if (pPhyMO->getPoId() != ObjectId::NullObjectId)
                {
					trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as interface is member of Port-channel"));
                    return WRC_CONNECTOR_BREAKOUT_FAILED_PORTCHANNEL_MEMBER;
                }
				else
				{
					return WAVE_MESSAGE_SUCCESS;
				}
			}

			//If this is a castor platform and the interface is not in shutdown state, return ERROR.
			if(Capabilities::instance()->isCapabilityPresent(SFP_BREAKOUT_CAPABILITY))
			{
				if(pPhyMO->getStatus() != true)
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as the Interface is not in shutdown state."));

					return WRC_CONNECTOR_INTERFACES_SHOULD_BE_SHUTDOWN;
				}
                else if (pPhyMO->getPoId() != ObjectId::NullObjectId)
                {
					trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as interface is member of Port-channel"));
                    return WRC_CONNECTOR_BREAKOUT_FAILED_PORTCHANNEL_MEMBER;
                }
				else
				{
					return WAVE_MESSAGE_SUCCESS;
				}
			}

			trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as the command cannot be executed on this platform."));

			return WRC_LINECARD_MANAGEMENT_FAILED_PLATFORM_NOT_SUPPORTED;
		}
		break;

		case CONNECTOR_CONFIG_DEL_NAME:
		{
			//Do not allow the user to delete the connector MOs, once created.
			trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as the command cannot be executed on this platform."));

			return WRC_CONNECTORS_CANNOT_BE_DELETED;
		}
		break;

		case CONNECTOR_CONFIG_DEL_BREAKOUT:
		{
			//If slot does not exist, return ERROR.
			if(pSlotMO == NULL)
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as the linecard does not exist."));

				return WRC_LINECARD_IS_NOT_INSERTED;
			}

			//If this is a mercury platform and the slot is not in offline state, return ERROR.
			if(linecardNumber != 0)
			{
			   if(pSlotMO->getSlotState() != 2)
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as the linecard is not powered-off.. State is:") + pSlotMO->getSlotState());

					return WRC_LINECARD_SHOULD_BE_IN_OFFLINE_STATE;
				}
               else if (WRC_CONNECTOR_BREAKOUT_FAILED_PORTCHANNEL_MEMBER == validateBreakoutInterfacesConfig(connectorName, pContext))
                {
                    return WRC_CONNECTOR_BREAKOUT_FAILED_PORTCHANNEL_MEMBER;
                }
				else
				{
					return WAVE_MESSAGE_SUCCESS;
				}
			}

			if(Capabilities::instance()->isCapabilityPresent(SFP_BREAKOUT_CAPABILITY))
			{
				//Make sure that all the ports belong to this connector are in shutdown state.
                return validateBreakoutInterfacesConfig(connectorName, pContext);
			}

			trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as the command cannot be executed on this platform."));

			return WRC_LINECARD_MANAGEMENT_FAILED_PLATFORM_NOT_SUPPORTED;
		}
		break;

		default:
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as an invalid command has been entered."));

			return WRC_LINECARD_MANAGEMENT_FAILED_PLATFORM_NOT_SUPPORTED;
		}
		break;
	}

	return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfConfLocalWorker::validateBreakoutInterfacesConfig (const string &connectorName, DceSynchronousLinearSeqContext *pContext) {
    string breakoutIfExp = connectorName + string(":%");
    WaveManagedObjectSynchronousQueryContext breakoutPhyQueryCtxt (PhyIntfLocalManagedObject::getClassName ());
    breakoutPhyQueryCtxt.addAndAttribute (new AttributeString (breakoutIfExp, "ifName"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
    breakoutPhyQueryCtxt.addSelectField("status");
    breakoutPhyQueryCtxt.addSelectField("poId");
    vector<WaveManagedObject *> *pBreakoutPhyResults = querySynchronously (&breakoutPhyQueryCtxt);

    if((pBreakoutPhyResults != NULL) && (pBreakoutPhyResults->size() != 0)) {
        pContext->addManagedObjectsForGarbageCollection(*pBreakoutPhyResults);
		for(UI32 breakoutIndex = 0; breakoutIndex < pBreakoutPhyResults->size(); breakoutIndex++) {
            PhyIntfLocalManagedObject *pBreakoutPhyMO = NULL;
            pBreakoutPhyMO =  dynamic_cast<PhyIntfLocalManagedObject*>((*pBreakoutPhyResults)[breakoutIndex]);

            if(pBreakoutPhyMO->getStatus() != true)	{
                trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as a breakout Interface is not in shutdown state."));
                pBreakoutPhyResults->clear();
                delete pBreakoutPhyResults;
                return WRC_CONNECTOR_INTERFACES_SHOULD_BE_SHUTDOWN;
            }
            if (pBreakoutPhyMO->getPoId() != ObjectId::NullObjectId) {
                trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateConnectorConfigStep: return ERROR as interface is member of Port-channel"));
                pBreakoutPhyResults->clear();
                delete pBreakoutPhyResults;
                return WRC_CONNECTOR_BREAKOUT_FAILED_PORTCHANNEL_MEMBER;
            }
		}
        pBreakoutPhyResults->clear();
        delete pBreakoutPhyResults;
    }
    return WAVE_MESSAGE_SUCCESS;
}

// This method sends the configuration message to the backend daemons,
// as required, which would happen for dynamic breakout configuration in QSFP 
// As of now, QSFP Breakout configuration is static. So, we return
// SUCCESS in this method.
ResourceId IntfConfLocalWorker::connectorConfigStep (DceSynchronousLinearSeqContext *pContext)
{
	return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfConfLocalWorker::deleteConnectorPhyIntfMO (string connectorName, AttributeConditionOperator attributeConditionOperator)
{
	PhyIntfLocalManagedObject *pPhyMO = NULL;
	WaveManagedObjectSynchronousQueryContext phySyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
	phySyncQueryCtxt.addAndAttribute (new AttributeString(&connectorName,"ifName"), attributeConditionOperator);
	vector<WaveManagedObject *> *pResults = querySynchronously (&phySyncQueryCtxt);

	if(pResults != NULL)
	{
		for(UI32 i = 0; i < pResults->size(); i++)
		{
            pPhyMO =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[i]);

			trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::deleteConnectorPhyIntfMO deleting the PhyMO with ifName:") + pPhyMO->getIfName());

			delete pPhyMO;
		}

		pResults->clear();
		delete pResults;
	}

	return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfConfLocalWorker::connectorConfigMOStep (DceSynchronousLinearSeqContext *pContext)
{
    ConnectorConfigLocalMessage *pMsg = dynamic_cast<ConnectorConfigLocalMessage*> (pContext->getPPrismMessage ());
	string connectorName = pMsg->getName();
	UI32 cmdCode = pMsg->getCmdCode();
	ConnectorConfigLocalManagedObject *pMO = NULL;

    if (CONNECTOR_CONFIG_BULK_UPDATE == cmdCode) {
        return updateBulkConfigsToConnectorMO(pMsg->getBulkConfigsVector(), pContext);
    }

	WaveManagedObjectSynchronousQueryContext syncQueryCtxt (ConnectorConfigLocalManagedObject::getClassName ());
	syncQueryCtxt.addAndAttribute(new AttributeString(connectorName, "connectorName"));
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    pContext->addManagedObjectsForGarbageCollection(*pResults);

	if((pResults == NULL) || (pResults->size() == 0))
	{
		pMO = new ConnectorConfigLocalManagedObject (dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
        pContext->addManagedObjectForGarbageCollection(pMO);
	} else if (pResults->size() == 1)
	{
		pMO = dynamic_cast<ConnectorConfigLocalManagedObject *>((*pResults)[0]);
	} else
   	{
		trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::connectorConfigMOStep returning ERROR as pResults is not 1 for ConnectorConfigLocalManagedObject"));

		return WAVE_MESSAGE_ERROR;
	}	

    prismAssert (NULL != pMO, __FILE__, __LINE__);

	switch(cmdCode)
	{
		case CONNECTOR_CONFIG_SET_NAME:
		{
			pMO->setName(connectorName);
			updateWaveManagedObject(pMO);

		}
			break;
		case CONNECTOR_CONFIG_DEL_NAME:
		{
			delete pMO;
		}
			break;
		case CONNECTOR_CONFIG_SET_BREAKOUT:
		{
			pMO->setBreakout(true);
			updateWaveManagedObject(pMO);

			deleteConnectorPhyIntfMO(connectorName, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
		}
			break;
		case CONNECTOR_CONFIG_DEL_BREAKOUT:
		{
			pMO->setBreakout(false);
			updateWaveManagedObject(pMO);

			//When the connectorName is 1/2/3, then delete all the ifnames which are of the form "1/2/3:%"
			//where "%" is a wildcard.
			connectorName = connectorName + ":%";
			deleteConnectorPhyIntfMO(connectorName, WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
		}
			break;
		default:
		{
			trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::connectorConfigConfigMOStep default case occured"));
		}
			break;
	}

	pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
	return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfConfLocalWorker::clearSpanConnectorConfigStep (DceSynchronousLinearSeqContext *pContext)
{
    ConnectorConfigLocalMessage *pMsg = dynamic_cast<ConnectorConfigLocalMessage*> (pContext->getPPrismMessage ());

    /** Allow configuration when it is coming from file replay specifically during config snapshot restore.**/
    /** Allow configuration when Bulk request comes from EarlyConfigurationReplay **/
    if ((CONNECTOR_CONFIG_BULK_UPDATE == pMsg->getCmdCode()) || (FrameworkToolKit::isFileReplayInProgress())) {
        return WAVE_MESSAGE_SUCCESS;
    }

	SpanGlobalConfigManagedObject *pMO = NULL;
	string connectorName = pMsg->getName();
	UI32 cmdCode = pMsg->getCmdCode();
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	if(!((cmdCode == CONNECTOR_CONFIG_SET_BREAKOUT) || (cmdCode == CONNECTOR_CONFIG_DEL_BREAKOUT)))
		return WAVE_MESSAGE_SUCCESS;

	WaveManagedObjectSynchronousQueryContext queryCtxt (SpanGlobalConfigManagedObject::getClassName ());

	if(cmdCode == CONNECTOR_CONFIG_SET_BREAKOUT)
	{
		queryCtxt.addOrAttribute (new AttributeString (connectorName, "source_ifName"));
		queryCtxt.addOrAttribute (new AttributeString (connectorName, "dest_ifName"));
	}
	else
	{
		string breakoutIfExp = connectorName + string(":%");
		queryCtxt.addOrAttribute (new AttributeString (breakoutIfExp, "source_ifName"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
		queryCtxt.addOrAttribute (new AttributeString (breakoutIfExp, "dest_ifName"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
	}

	vector<WaveManagedObject *> *pResults = querySynchronously (&queryCtxt);

	if((pResults != NULL) && (pResults->size() != 0))
	{
		pContext->addManagedObjectsForGarbageCollection(*pResults);

		for(UI32 index = 0; index < pResults->size(); index++)
		{
			pMO = dynamic_cast<SpanGlobalConfigManagedObject*>((*pResults)[index]);
			SpanGlobalSpanGlobalConfigMessage *pMessage = new SpanGlobalSpanGlobalConfigMessage();

			pMessage->setOpCode(SPAN_MSG_NO_SET);
			pMessage->setMessageType(SPAN_SFP_BREAKOUT_CLEAR_LOCAL_MSG_CONFIG);
			pMessage->setSession_id(pMO->getSession_id());
			pMessage->setSource_ifName(pMO->getSource_ifName());
			pMessage->setDest_ifName(pMO->getDest_ifName());
			pMessage->setDirection(pMO->getDirection());
			pMessage->setSrcethernet(pMO->getSource_tengiga());
			pMessage->setDestethernet(pMO->getDest_tengiga());
			pMessage->setDestVlanId(pMO->getDestVlanId());
			status = sendSynchronously(pMessage);
			delete pMessage;

			if(status != WAVE_MESSAGE_SUCCESS)
			{
				NSM_PLUG_DBG_ERR_S(string("Failed to clear the span config after sfp breakout"));
				pResults->clear();
				delete pResults;
				return status;
			}
			else {
				NSM_PLUG_INFO_S(string("Successfully deleted the span config after sfp breakout"));
			}
		}

		pResults->clear();
		delete pResults;
	}

	return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfConfLocalWorker::clearL2sysConnectorConfigStep (DceSynchronousLinearSeqContext *pContext)
{
    ConnectorConfigLocalMessage *pMsg = dynamic_cast<ConnectorConfigLocalMessage*> (pContext->getPPrismMessage ());

    /** Allow configuration when it is coming from file replay specifically during config snapshot restore.**/
    /** Allow configuration when Bulk request comes from EarlyConfigurationReplay **/
    if ((CONNECTOR_CONFIG_BULK_UPDATE == pMsg->getCmdCode()) || (FrameworkToolKit::isFileReplayInProgress())) {
        return WAVE_MESSAGE_SUCCESS;
    }

	L2sysGlobalStaticMacManagedObject *pMO = NULL;
	string connectorName = pMsg->getName();
	UI32 cmdCode = pMsg->getCmdCode();
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	if(!((cmdCode == CONNECTOR_CONFIG_SET_BREAKOUT) || (cmdCode == CONNECTOR_CONFIG_DEL_BREAKOUT)))
		return WAVE_MESSAGE_SUCCESS;

	WaveManagedObjectSynchronousQueryContext queryCtxt (L2sysGlobalStaticMacManagedObject::getClassName ());

	if(cmdCode == CONNECTOR_CONFIG_SET_BREAKOUT)
	{
		queryCtxt.addAndAttribute (new AttributeString (connectorName, "ifname"));
	}
	else
	{
		string breakoutIfExp = connectorName + string(":%");
		queryCtxt.addAndAttribute (new AttributeString (breakoutIfExp, "ifname"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
	}

	vector<WaveManagedObject *> *pResults = querySynchronously (&queryCtxt);

	if((pResults != NULL) && (pResults->size() != 0))
	{
		pContext->addManagedObjectsForGarbageCollection(*pResults);

		for(UI32 index = 0; index < pResults->size(); index++)
		{
			pMO = dynamic_cast<L2sysGlobalStaticMacManagedObject*>((*pResults)[index]);
			L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pMessage = new L2sysGlobalUpdateL2sysGlobalStaticMacMessage();

			pMessage->setOpcode(L2SYS_DELETE_OP);
			pMessage->setMac(pMO->getMac());
			pMessage->setForward(pMO->getForward());
			pMessage->setInterface_type(pMO->getInterface_type());
			pMessage->setIfname(pMO->getIfname());
			pMessage->setVlan(pMO->getVlan());
			pMessage->setSlotid(pMO->getSlotid());
			pMessage->setVlanid(pMO->getVlanid());

			if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled())
			{
				UI32 rbridgeId = NsmUtils::getMappedIdFromIfName(pMO->getIfname());
				pMessage->setRbridgeid(rbridgeId);
			}

			trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::clearL2sysConnectorConfig:rbridge: ") + pMessage->getRbridgeid());
			status = sendSynchronously(pMessage);
			delete pMessage;

			if(status != WAVE_MESSAGE_SUCCESS)
			{
				pResults->clear();
				delete pResults;
				return status;
			}
		}

		pResults->clear();
		delete pResults;
	}

	return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfConfLocalWorker::clearFcoeConnectorConfigStep (DceSynchronousLinearSeqContext *pContext)
{
	if(WAVE_PERSISTENCE_CLUSTER_ENABLED != DcmToolKit::isVcsEnabled())
		return WAVE_MESSAGE_SUCCESS;

	ConnectorConfigLocalMessage *pMsg = dynamic_cast<ConnectorConfigLocalMessage*> (pContext->getPPrismMessage ());

    /** Allow configuration when it is coming from file replay specifically during config snapshot restore.**/
    /** Allow configuration when Bulk request comes from EarlyConfigurationReplay **/
    if ((CONNECTOR_CONFIG_BULK_UPDATE == pMsg->getCmdCode()) || (FrameworkToolKit::isFileReplayInProgress())) {
        return WAVE_MESSAGE_SUCCESS;
    }

	FcoeETIntfLocalManagedObject *pMO = NULL;
	string connectorName = pMsg->getName();
	UI32 cmdCode = pMsg->getCmdCode();
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	if(!((cmdCode == CONNECTOR_CONFIG_SET_BREAKOUT) || (cmdCode == CONNECTOR_CONFIG_DEL_BREAKOUT)))
		return WAVE_MESSAGE_SUCCESS;

	WaveManagedObjectSynchronousQueryContext queryCtxt (FcoeETIntfLocalManagedObject::getClassName ());
	queryCtxt.addSelectField("et_if_name");

	if(cmdCode == CONNECTOR_CONFIG_SET_BREAKOUT)
	{
		queryCtxt.addAndAttribute (new AttributeString (connectorName, "et_if_name"));
	}
	else
	{
		string breakoutIfExp = connectorName + string(":%");
		queryCtxt.addAndAttribute (new AttributeString (breakoutIfExp, "et_if_name"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
	}

	vector<WaveManagedObject *> *pResults = querySynchronously (&queryCtxt);

	if((pResults != NULL) && (pResults->size() != 0))
	{
		pContext->addManagedObjectsForGarbageCollection(*pResults);

		for(UI32 index = 0; index < pResults->size(); index++)
		{
			pMO = dynamic_cast<FcoeETIntfLocalManagedObject*>((*pResults)[index]);
			FcoeLocalETIntfMessage *pMessage = new FcoeLocalETIntfMessage();

			pMessage->setOper_bmp(FCOE_ET_INTF_OPER_REMOVE);
			pMessage->setIfName(pMO->getName());

			status = sendSynchronously(pMessage);
			delete pMessage;
            trace (TRACE_LEVEL_INFO, string("IntfConfLocalWorker::clearFcoeConnectorConfigStep. Message sent to Fcoe Plugin to delete ET_Intf: ") + pMO->getName());

			if(status != WAVE_MESSAGE_SUCCESS)
			{
				pResults->clear();
				delete pResults;
				return status;
			}
		}

		pResults->clear();
		delete pResults;
	}

	return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfConfLocalWorker::clearStpPortIdConfigStep (DceSynchronousLinearSeqContext *pContext) {
    ConnectorConfigLocalMessage *pMsg = dynamic_cast<ConnectorConfigLocalMessage*> (pContext->getPPrismMessage ());

    /** Allow configuration when it is coming from file replay specifically during config snapshot restore.**/
    /** Allow configuration when Bulk request comes from EarlyConfigurationReplay **/
    if ((CONNECTOR_CONFIG_BULK_UPDATE == pMsg->getCmdCode()) || (FrameworkToolKit::isFileReplayInProgress())) {
        return WAVE_MESSAGE_SUCCESS;
    }
    if(!((pMsg->getCmdCode() == CONNECTOR_CONFIG_SET_BREAKOUT) || (pMsg->getCmdCode() == CONNECTOR_CONFIG_DEL_BREAKOUT)))
        return WAVE_MESSAGE_SUCCESS;

    string connectorName = pMsg->getName();

    WaveManagedObjectSynchronousQueryContext phySyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
    if(pMsg->getCmdCode() == CONNECTOR_CONFIG_SET_BREAKOUT) {
        phySyncQueryCtxt.addAndAttribute (new AttributeString (connectorName, "ifname"));
    } else {
        string breakoutIfExp = connectorName + string(":%");
        phySyncQueryCtxt.addAndAttribute (new AttributeString (breakoutIfExp, "ifname"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
    }
    phySyncQueryCtxt.addSelectField("phyType");
    phySyncQueryCtxt.addSelectField("id");

    vector<WaveManagedObject *> *pResults = querySynchronously (&phySyncQueryCtxt);
    if((pResults != NULL) && (pResults->size() > 0)) {
        pContext->addManagedObjectsForGarbageCollection(*pResults);
        for(UI32 i = 0; i < pResults->size(); i++) {
            PhyIntfLocalManagedObject *pPhyMO =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[i]);
            MstpToolKit::freePortId(pPhyMO->getPhyType(), pPhyMO->getId(), true, pContext);
            trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::clearStpPortIdConfigStep. Cleared Stp port Id for ") + pPhyMO->getId());
        }
        pResults->clear();
        delete pResults;
    }
    return WAVE_MESSAGE_SUCCESS;
}

bool IntfConfLocalWorker::isGlobalPortChannelReplayDone () {
    return m_isGlobalPortChannelReplayDone;
}

void IntfConfLocalWorker::setGlobalPortChannelReplayDone(bool replayDone) {
    m_isGlobalPortChannelReplayDone = replayDone;
    m_portchannelReplayedSet.clear();
    m_slotIdPortChannelOidMap.clear();
}

ResourceId IntfConfLocalWorker::updateBulkConfigsToConnectorMO (const vector<string> &bulkConfigs, DceSynchronousLinearSeqContext *pContext) {
    ConnectorConfigLocalManagedObject *pMO;

    if (bulkConfigs.size() > 0) {
        for (UI32 i = 0; i < bulkConfigs.size(); i++) {
            string configStr = bulkConfigs[i];
            vector<string>  token;
            tokenize (configStr, token, '#');
            UI32 numberOfToken = token.size ();
            if (numberOfToken == 2) {
                string connectorName = token[0];
                bool isBreakoutEnabled = (bool) atoi(token[1].c_str());

                pMO = new ConnectorConfigLocalManagedObject (dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
                pContext->addManagedObjectForGarbageCollection(pMO);
                pMO->setName(connectorName);
                pMO->setBreakout(isBreakoutEnabled);
                updateWaveManagedObject(pMO);
            }
        }
    }
    return WAVE_MESSAGE_SUCCESS;
}

/**
 * This is private function for replaying Admin state of physical Interfaces
 * on request of Nsmd (event INTERFACE_STATE_REPLAY) when slot power-on happens
 */
void IntfConfLocalWorker::interfaceStateReplay (UI32 slotId) {
    // Replay interface states to backend
    ResourceId intfReplayStatus = IntfConfLocalWorker::configReplayIntfAdminState(slotId);
    trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::interfaceStateReplay -- replayed Interface states for slot: ") + slotId + string(" status= ") + FrameworkToolKit::localize(intfReplayStatus));

    /* Send message to nsmd after completion of interface state replay of slot */
    NsmClientUpdateGlobalConfigMessage *pMessage = new NsmClientUpdateGlobalConfigMessage();
    pMessage->setCmdCode(NSM_PLUGIN_INTF_STATEREPLAY_RESP);
    pMessage->setSlotId(slotId);
    ResourceId sendStatus = sendSynchronouslyToWaveClient ("nsm",pMessage);
    delete pMessage;
    trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::interfaceStateReplay -- resp to nsmd Status - ")+ FrameworkToolKit::localize(sendStatus));
}

/**
 * This function will be called from boot function of NsmLocalObjectManager.
 * It will check if any pending requests for INTERFACE_STATE_REPLAY from
 * cache which were made when plugin was shut
 */
void IntfConfLocalWorker::bootPhaseInterfaceStateReplay() {
    m_intfStateReplay_Mutex.lock();
    UI32 numOfSlots = m_intfStateReplay_slotIds.size();
    if (numOfSlots > 0) {
        trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::bootPhaseInterfaceStateReplay -- Replaying Interface State during plugin boot phase"));
        for (UI32 i = 0; i < numOfSlots; i++) {
            UI32 slotId = m_intfStateReplay_slotIds[i]; 
            interfaceStateReplay(slotId);
        }

        //Cleanup the SlotId vector as it processed
        m_intfStateReplay_slotIds.clear();
    }
    m_intfStateReplay_Mutex.unlock();
}

/**
 * Regular handler function of InterfaceStateReplayServiceIndependentMessage message
 */
void IntfConfLocalWorker::interfaceStateReplayMessageHandler (InterfaceStateReplayServiceIndependentMessage *msg) {
    UI32 slotId = msg->getParameter();
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    msg->setCompletionStatus(status);
    trace(TRACE_LEVEL_DEBUG, string("Sending Event Reply Msg ..."));
    reply(msg);

    interfaceStateReplay(slotId);
}

/**
 * Static handler function of InterfaceStateReplayServiceIndependentMessage message
 */
void IntfConfLocalWorker::interfaceStateReplayServiceIndependentMessageHandler (InterfaceStateReplayServiceIndependentMessage *msg) {
    UI32 slotId = msg->getParameter();

    if ((NsmLocalObjectManager::getInstance())->isEnabled()) {
        msg->setIsPluginEnabled(true);
    } else {
        WaveNs::tracePrintf(TRACE_LEVEL_INFO, "IntfConfLocalWorker::interfaceStateReplayServiceIndependentMessageHandler -- Nsm Local Plugin is down. Caching SlotId: %d", slotId);

        msg->setIsPluginEnabled(false);
        // Push slotId to cache to processed in boot phase
        m_intfStateReplay_Mutex.lock();
        m_intfStateReplay_slotIds.push_back(slotId);
        m_intfStateReplay_Mutex.unlock();
    }
    msg->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
}

ResourceId IntfConfLocalWorker::notifySlotReplayCompleted (UI32 slotId) {
    /* Send message to nsmd nofifying Slot Config replay completed */
    NsmClientUpdateGlobalConfigMessage *pMessage = new NsmClientUpdateGlobalConfigMessage();
    pMessage->setCmdCode(NSM_PLUGIN_SLOT_CONFIG_REPLAY_COMPLETE_RESP);
    pMessage->setSlotId(slotId);
    ResourceId sendStatus = sendSynchronouslyToWaveClient ("nsm",pMessage);
    delete pMessage;
    trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::notifySlotReplayCompleted for SlotId: ") + slotId + string("-- response from nsmd Status: ")+ FrameworkToolKit::localize(sendStatus));

    // When Slot Replay is completed, delete those POs from POReplayedSet which has all members from this slot only.
    cleanupPOReplayedSetWhenSlotReplayDone (DcmToolKit::getLocalMappedId(), slotId);
    return sendStatus;
}

void IntfConfLocalWorker::cleanupPOReplayedSetWhenSlotReplayDone (UI32 rbridgeId, UI32 slotId) {
    if (WAVE_PERSISTENCE_CLUSTER_DISABLED == DcmToolKit::isVcsEnabled()) {
        m_portchannelReplayedSet.clear();
        return;
    }
    vector<ObjectId> poIds = m_slotIdPortChannelOidMap[slotId];
    for (UI32 i = 0; i < poIds.size(); i++) {
        ObjectId poObjectId = poIds[i];

        m_portchannelReplayedSet.erase (poObjectId);
        trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::cleanupPOReplayedSetWhenSlotReplayDone for SlotId: ") + slotId + string("--deleted PO Object id: ") + poObjectId.toString());
    }
    poIds.clear();
    m_slotIdPortChannelOidMap.erase(slotId);
}

void IntfConfLocalWorker::switchportVlanConfigLocalMessageHandler (SwitchportVlanConfigLocalMessage *pSwitchportVlanConfigLocalMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::validateSwitchportVlanConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::switchportVlanConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::switchportVlanConfigMOStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
    };

    DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pSwitchportVlanConfigLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pDceSynchronousLinearSeqContext->execute ();
}

ResourceId IntfConfLocalWorker::validateSwitchportVlanConfigStep (DceSynchronousLinearSeqContext *pContext)
{
	if(ProvisioningUtil::isGvlanEnabled() == GVLAN_STATE_DISABLE)
	{
		trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:GVlan is not enabled."));

		return WRC_NSM_GVLAN_NOT_CAPABLE_FABRIC;
	}

	SwitchportVlanConfigLocalMessage *pMsg = dynamic_cast<SwitchportVlanConfigLocalMessage*> (pContext->getPPrismMessage ());
	UI32 vlanId = pMsg->getVlanId(), cmdCode = pMsg->getCmdCode(), count = 0, ctagId = pMsg->getCtagId();
	string ifName = pMsg->getIfName();
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	PhyIntfLocalManagedObject *pPhyMO = NULL; 

	WaveManagedObjectSynchronousQueryContext phySyncQueryCtxt (PhyIntfLocalManagedObject::getClassName ());
	phySyncQueryCtxt.addAndAttribute (new AttributeString (ifName, "ifName"));
	phySyncQueryCtxt.addSelectField ("phyType");
	phySyncQueryCtxt.addSelectField ("swModeFlag");
	phySyncQueryCtxt.addSelectField ("swMode");
	phySyncQueryCtxt.addSelectField ("isPortProfiled");
    phySyncQueryCtxt.addSelectField ("associatedPortProfiles");
    phySyncQueryCtxt.addSelectField ("associatedPortProfileDomain");
	phySyncQueryCtxt.addSelectField ("pVlanModeTrunkPromis");
	phySyncQueryCtxt.addSelectField ("pVlanModeTrunkBas");
	phySyncQueryCtxt.addSelectField ("pVlanModeTrunkHost");
	phySyncQueryCtxt.addSelectField ("isAllowedVlanAll");
	phySyncQueryCtxt.addSelectField ("vlanRangeAdd");
	phySyncQueryCtxt.addSelectField ("vlanRangeAdd");
	phySyncQueryCtxt.addSelectField ("pVlanIsAllowedVlanAll");
	phySyncQueryCtxt.addSelectField ("pVlanRangeAdd");
	phySyncQueryCtxt.addSelectField ("pVlanRangeRemove");
	phySyncQueryCtxt.addSelectField ("rspan_trunkvlanrange");
	phySyncQueryCtxt.addSelectField ("vlanObjectId");
	phySyncQueryCtxt.addSelectField ("rspan_accessvlan");
	phySyncQueryCtxt.addSelectField ("pVlanHostSecVlanId");
	vector<WaveManagedObject *> *pPhyResults = querySynchronously (&phySyncQueryCtxt);

	if((pPhyResults != NULL) && (pPhyResults->size() != 0))
	{
		pContext->addManagedObjectsForGarbageCollection(*pPhyResults);
		pPhyMO =  dynamic_cast<PhyIntfLocalManagedObject*>((*pPhyResults)[0]);
		pPhyResults->clear();
		delete pPhyResults;
		pPhyResults = NULL;
	}

	if (pPhyMO == NULL)
	{
		status = WRC_NSM_ERR_PHYINTF_INVALID;

		trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep: return ERROR as the interface does not exist."));

		return status;
	}
	else
	{
		UI32 ifType = pPhyMO->getPhyType();
		pContext->getCache()->put("ifType", ifType);
	}

	bool rspanEnabled = false;
	if(pMsg->getCmdCode() == RSPAN_GVLAN_ACCESS_PORT_SET_MAC_ADDRESS || pMsg->getCmdCode() == RSPAN_GVLAN_ACCESS_PORT_DEL_MAC_ADDRESS ||
	   pMsg->getCmdCode() == RSPAN_GVLAN_TRUNK_PORT_SET_CTAG || pMsg->getCmdCode() == RSPAN_GVLAN_TRUNK_PORT_DEL_CTAG ||
	   pMsg->getCmdCode() == RSPAN_GVLAN_ACCESS_PORT_SET_MAC_GROUP || pMsg->getCmdCode() == RSPAN_GVLAN_ACCESS_PORT_DEL_MAC_GROUP) {
		rspanEnabled = true;
	}

	WaveManagedObjectSynchronousQueryContext vlanSyncQueryCtxt (VlanIntfManagedObject::getClassName ());
	vlanSyncQueryCtxt.addOrAttribute (new AttributeUI32 (vlanId, "id"));
	vlanSyncQueryCtxt.addAndAttribute(new AttributeBool(rspanEnabled, "remoteSpanEnabled"));
	vector<WaveManagedObject *> *pVlanResults = querySynchronously (&vlanSyncQueryCtxt);
	VlanIntfManagedObject *pVlanMO = NULL;

	if((pVlanResults != NULL) && (pVlanResults->size() != 0))
	{
		pContext->addManagedObjectsForGarbageCollection(*pVlanResults);
		pVlanMO = dynamic_cast<VlanIntfManagedObject*>((*pVlanResults)[0]);
		pVlanResults->clear();
		delete pVlanResults;
		pVlanResults = NULL;
	}

	if (pVlanMO == NULL)
	{
		status = WRC_NSM_API_ERR_VLAN_NOT_CONFIGURED;

		trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Vlan is not configured."));

		return status;
	}

    bool isPortProfiled = false;
    APPMUtils::isPortProfilePort(pPhyMO, isPortProfiled);
    if(isPortProfiled) {
		status = WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED;
		return status;
	}

	/* Check if the swtchport mode is enabled */
	if(!pPhyMO->getSwModeFlag())
	{
		trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Interface is not configured for switching."));

		status = WRC_SWITCHING_NOT_ENABLED;

		return status;
	}

	pvlanTypeEnum pVlanType = pVlanMO->getPVlanType();

	switch(cmdCode)
	{
		case GVLAN_ACCESS_PORT_SET_MAC_ADDRESS:
		case RSPAN_GVLAN_ACCESS_PORT_SET_MAC_ADDRESS:
		{
			/* Check if the switchport mode is access */
			if(pPhyMO->getSwMode() != SW_MODE_ACCESS)
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker:validateSwitchportVlanConfigStep:Interface is not in switchport access mode."));

				status = WRC_SW_ACCESSMODE_NOT_ENABLED;

				return status;
			}

			// If the vlan is a primary vlan, return ERROR.
			if(pVlanType != PVLAN_TYPE_UNKNOWN)
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Only regular vlans can be configured with a ctag in trunk mode."));

				status = WRC_NSM_PVLAN_VLAN;

				return status;
			}

			if(pPhyMO->getAccessVlanObjectId() == pVlanMO->getObjectId())
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker:validateSwitchportVlanConfigStep:Vlan Id has already been configured as an access vlan on this port."));

				status = WRC_NSM_GVLAN_VLAN_IS_USED_AS_ACCESSVLAN;

				return status;
			}

			if(pPhyMO->getAccessRspanVlan() == vlanId)
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker:validateSwitchportVlanConfigStep:Vlan Id has already been configured as an access rspan-vlan on this port."));

				status = WRC_NSM_GVLAN_VLAN_IS_USED_AS_ACCESSRSPANVLAN;

				return status;
			}
		}
		break;

		case GVLAN_ACCESS_PORT_SET_MAC_GROUP:
		case RSPAN_GVLAN_ACCESS_PORT_SET_MAC_GROUP:
		{
			/* Check if the switchport mode is access */
			if(pPhyMO->getSwMode() != SW_MODE_ACCESS)
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker:validateSwitchportVlanConfigStep:Interface is not in switchport access mode."));

				status = WRC_SW_ACCESSMODE_NOT_ENABLED;

				return status;
			}

			// If the vlan is a primary vlan, return ERROR.
			if(pVlanType != PVLAN_TYPE_UNKNOWN)
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Only regular vlans can be configured with a ctag in trunk mode."));

				status = WRC_NSM_PVLAN_VLAN;

				return status;
			}

			if(pPhyMO->getAccessVlanObjectId() == pVlanMO->getObjectId())
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker:validateSwitchportVlanConfigStep:Vlan Id has already been configured as an access vlan on this port."));

				status = WRC_NSM_GVLAN_VLAN_IS_USED_AS_ACCESSVLAN;

				return status;
			}

			if(pPhyMO->getAccessRspanVlan() == vlanId)
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker:validateSwitchportVlanConfigStep:Vlan Id has already been configured as an access rspan vlan on this port."));

				status = WRC_NSM_GVLAN_VLAN_IS_USED_AS_ACCESSRSPANVLAN;

				return status;
			}

			count = 0;
			WaveManagedObjectSynchronousQueryContext macGroupQueryCtxt (MacGroupManagedObject::getClassName ());
			macGroupQueryCtxt.addOrAttribute (new AttributeUI32 (pMsg->getMacGroupId(), "id"));
			querySynchronouslyForCount (&macGroupQueryCtxt, count);

			if(count != 1)
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep: MacGroup is not configured."));

				status = WRC_NSM_ERR_MAC_GROUP_NOT_CONFIGURED;

				return status;
			}
		}
		break;

		case GVLAN_ACCESS_PORT_DEL_MAC_ADDRESS:
		case RSPAN_GVLAN_ACCESS_PORT_DEL_MAC_ADDRESS:
		case GVLAN_ACCESS_PORT_DEL_MAC_GROUP:
		case RSPAN_GVLAN_ACCESS_PORT_DEL_MAC_GROUP:
		{
			/* Check if the switchport mode is access */
			if(pPhyMO->getSwMode() != SW_MODE_ACCESS)
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker:validateSwitchportVlanConfigStep:Interface is not in switchport access mode."));

				status = WRC_SW_ACCESSMODE_NOT_ENABLED;

				return status;
			}
		}
		break;

		case GVLAN_TRUNK_PORT_SET_CTAG:
		{
			/* The port should be in Trunk or Private-VLAN-Trunk-Promis mode. */
			if(!((pPhyMO->getSwMode() == SW_MODE_TRUNK) || (pPhyMO->getPvlanModeTrunkPromis()) || (pPhyMO->getPvlanModeTrunkHost())))
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Interface is not in trunk or pvlan-trunk-promis or pvlan-trunk-host mode."));

				status = WRC_NSM_ERR_PORT_NOT_TRUNK_OR_TRUNK_PROMIS;

				return status;
			}

			//Validation about vlan begins  here.
			//--------------------------------------------------
			//		Interface mode				|	vlan-type
			//--------------------------------------------------
			//trunk								|	regular vlan
			//private-vlan-trunk-promiscuous	|	regular vlan + primary-vlan
			//private-vlan-trunk-host			|	secondary vlan
			if(pPhyMO->getSwMode() == SW_MODE_TRUNK)
			{
				if(pVlanType != PVLAN_TYPE_UNKNOWN)
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Only regular vlans can be configured with a ctag in trunk mode."));

					status = WRC_NSM_PVLAN_VLAN;

					return status;
				}
			}
			else if(pPhyMO->getPvlanModeTrunkPromis())
			{
				if(!((pVlanType == PVLAN_TYPE_UNKNOWN) || (pVlanType == PVLAN_TYPE_PRIMARY)))
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Only regular or primary vlans can be configured with a ctag in private-vlan-trunk-promiscuous mode."));

					status = WRC_NSM_PVLAN_SEC_VLAN;

					return status;
				}
			}
			else if(pPhyMO->getPvlanModeTrunkHost())
			{
				if(!((pVlanType == PVLAN_TYPE_ISOLATED) || (pVlanType == PVLAN_TYPE_COMMUNITY)))
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Only secondary vlans can be configured with a ctag in private-vlan-trunk-host mode."));

					status = WRC_NSM_PVLAN_NOT_SEC_VLAN;

					return status;
				}
			}

			if(pPhyMO->getIsAllowedVlanAll())
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Cannot configure ctags when allowed vlan all is set."));

				status = WRC_NSM_GVLAN_CANNOT_ADD_VLAN_CLASSIFICATION_WHEN_TRUNK_ALL;

				return status;
			}

			// Validation about ctag begins here.
			// If ctag is in the allowed range of trunk/trunk rspan, then reject the configuration.
			if((pPhyMO->getRspanVlanRangeAdd().find(ctagId)) || (pPhyMO->getVlanRangeAdd().find(ctagId)))
			{
				status = WRC_NSM_GVLAN_CTAG_IS_IN_ALLOWED_ADD_RANGE;

				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:ctag is in trunk [rspan] allowed range."));

				return status;
			}

			/* If the ctag is used for another vlan on the same port, then reject the configuration. */
			if(NsmUtils::getVlanClassificationMOCount(0, ctagId, pPhyMO->getObjectId()))
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Ctag is already used for another vlan on the same port."));

				status = WRC_NSM_VLAN_ERR_CTAG_ALREADY_IN_USE;

				return status;
			}

		}
		break;

		case GVLAN_TRUNK_PORT_DEL_CTAG:
		{
			/* The port should be in Trunk or Private-VLAN-Trunk-Promis mode. */
			if(!((pPhyMO->getSwMode() == SW_MODE_TRUNK) || (pPhyMO->getPvlanModeTrunkPromis()) || (pPhyMO->getPvlanModeTrunkHost())))
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Interface is not in trunk or pvlan-trunk-promis or pvlan-trunk-host mode."));

				status = WRC_NSM_ERR_PORT_NOT_TRUNK_OR_TRUNK_PROMIS;

				return status;
			}

			// When the port is in private-vlan-trunk-host mode and if the vlanId is configured as the pVlanHostSecVlanId for host-association, then return ERROR.
			if(pPhyMO->getPvlanModeTrunkHost())
			{
				if(pPhyMO->getPvlanHostSecVlanId() == vlanId)
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Cannot remove ctag when the vlan is configured as secondary vlan for host association on the same port."));

					status = WRC_NSM_VLAN_AS_SEC_IN_HOST_ASSOC;

					return status;
				}
			}

			// When the port is in private-vlan-trunk-promiscuous mode and if the vlanId is configured as the primaryVlanId for PVlanMapping, then return ERROR.
			if(pPhyMO->getPvlanModeTrunkPromis())
			{
				if(PVlanUtils::getVlanAsPrimaryInPVlanMappingCount(vlanId, pPhyMO->getObjectId()))
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Cannot remove ctag when the vlan is configured as secondary vlan for host association on the same port."));

					status = WRC_NSM_VLAN_AS_PRI_IN_PVLAN_MAPPING;

					return status;
				}
			}


		}
		break;

		case RSPAN_GVLAN_TRUNK_PORT_SET_CTAG:
		{
			/* Check if the switchport mode is trunk */
			if(pPhyMO->getSwMode() != SW_MODE_TRUNK)
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Interface is not in switchport trunk mode."));

				status = WRC_SW_TRUNKMODE_NOT_ENABLED;

				return status;
			}

			if(pPhyMO->getIsAllowedVlanAll())
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Cannot configure ctags when allowed vlan all is set."));

				status = WRC_NSM_ERR_PORT_NOT_TRUNK_OR_TRUNK_PROMIS; // find an error message.

				return status;
			}

			/* Validation about ctag */
			if((pPhyMO->getRspanVlanRangeAdd().find(ctagId)) || (pPhyMO->getVlanRangeAdd().find(ctagId)))
			{
				status = WRC_NSM_GVLAN_CTAG_IS_IN_ALLOWED_ADD_RANGE;

				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:ctag is in trunk [rspan] allowed range."));

				return status;
			}

			/* If the ctag is used for another vlan on the same port, then reject the configuration. */
			if(NsmUtils::getVlanClassificationMOCount(0, ctagId, pPhyMO->getObjectId()))
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Ctag is already used for another vlan on the same port."));

				status = WRC_NSM_VLAN_ERR_CTAG_ALREADY_IN_USE;

				return status;
			}

			//Validation of vlan begins here.
			//No need to validate for an rspan-vlan, because the validation has already been done, before this switch case.
		}
		break;

		case RSPAN_GVLAN_TRUNK_PORT_DEL_CTAG:
		{
			/* Check if the switchport mode is trunk */
			if(pPhyMO->getSwMode() != SW_MODE_TRUNK)
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Interface is not in switchport trunk mode."));

				status = WRC_SW_TRUNKMODE_NOT_ENABLED;

				return status;
			}
		}
		break;

		case GVLAN_PVLAN_TRUNK_PORT_SET_CTAG:
		{
			if(!pPhyMO->getPvlanModeTrunkBas())
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Interface is not in private-vlan trunk mode."));

				status = WRC_PVLAN_MODE_NOT_TRUNK;

				return status;
			}

			//Only private vlans should be allowed to be configured with a ctag for private-vlan-trunk-add-ctag.
			if(pVlanType == PVLAN_TYPE_UNKNOWN)
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Only private-vlans can be configured with private-vlan-trunk-add-ctag command."));

				status = WRC_NSM_PVLAN_ERR_NOT_CONFIGURED;

				return status;
			}

			if(pPhyMO->getPvlanIsAllowedVlanAll())
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Cannot configure ctags when private-vlan allowed vlan all is set."));

				status = WRC_NSM_GVLAN_CANNOT_ADD_VLAN_CLASSIFICATION_WHEN_TRUNK_ALL;

				return status;
			}

			if(pPhyMO->getPvlanRangeAdd().find(ctagId))
			{
				status = WRC_NSM_GVLAN_CTAG_IS_IN_ALLOWED_ADD_RANGE;

				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:ctag is in trunk pvlan allowed range."));

				return status;
			}

			/* If the ctag is used for another vlan on the same port, then reject the configuration. */
			if(NsmUtils::getVlanClassificationMOCount(0, ctagId, pPhyMO->getObjectId()))
			{
				trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Ctag is already used for another vlan on the same port."));

				status = WRC_NSM_VLAN_ERR_CTAG_ALREADY_IN_USE;

				return status;
			}

		}
		break;

		case GVLAN_PVLAN_TRUNK_PORT_DEL_CTAG:
		{
			if(!pPhyMO->getPvlanModeTrunkBas())
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Interface is not in private-vlan trunk mode."));

				status = WRC_PVLAN_MODE_NOT_TRUNK;

				return status;
			}

			// When the port is in private-vlan-trunk mode and if the vlanId is configured either as primaryVlanId or secondaryVlanId, for pvlan-trunk-association, then return ERROR.
			if(PVlanUtils::getVlanIdInPVlanTrunkAssociationCount(vlanId, pPhyMO->getObjectId()))
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Cannot remove private-vlan-ctag when the vlan is configured as primary or secondary in a trunk association on the same port."));

				status = WRC_NSM_VLAN_AS_PRI_OR_SEC_in_PVLAN_TRUNK_ASSOC;

				return status;
			}
		}
		break;

		default:
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Invalid command has been entered."));

			status = WAVE_MESSAGE_ERROR;
		}
		break;
	}

	return status;
}

ResourceId IntfConfLocalWorker::switchportVlanConfigStep (DceSynchronousLinearSeqContext *pContext)
{
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	SwitchportVlanConfigLocalMessage *pMsg = dynamic_cast<SwitchportVlanConfigLocalMessage*> (pContext->getPPrismMessage ());
	NsmPhyIntfMessage *pClientMsg = new NsmPhyIntfMessage();
	UI32 ifType = pContext->getCache()->getIntValue("ifType");
	UI32Range vlanRange(pMsg->getVlanId(), pMsg->getVlanId());
	tracePrintf(TRACE_LEVEL_INFO, "IntfConfLocalWorker::switchportVlanConfigStep: vlanRange:%s.", vlanRange.toString().c_str());

	if(true == NsmUtils::isValidThreeTuple(pMsg->getIfName ())) 
	{
		string twoTuppleIfName;
		string threeTupple = pMsg->getIfName ();
		NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
		pClientMsg->setIfName(twoTuppleIfName);
	} else
	{
		pClientMsg->setIfName (pMsg->getIfName());
	}

	pClientMsg->setIfType(ifType);
	pClientMsg->setVlanRange(vlanRange);
	pClientMsg->setMac(pMsg->getMac());
	pClientMsg->setCtagId(pMsg->getCtagId());
	pClientMsg->setMacGroupId(pMsg->getMacGroupId());
	pClientMsg->setCmdCode(pMsg->getCmdCode());

	switch(pMsg->getCmdCode())
	{
		case RSPAN_GVLAN_TRUNK_PORT_SET_CTAG:
		case RSPAN_GVLAN_ACCESS_PORT_SET_MAC_ADDRESS:
		case RSPAN_GVLAN_ACCESS_PORT_SET_MAC_GROUP:
		case GVLAN_ACCESS_PORT_SET_MAC_ADDRESS:
		case GVLAN_ACCESS_PORT_SET_MAC_GROUP:
		case GVLAN_TRUNK_PORT_SET_CTAG:
		case GVLAN_PVLAN_TRUNK_PORT_SET_CTAG:
		{
            NSM_PLUG_INFO_S(string("Need to provision the vlan ")+pClientMsg->getVlanRange().toString()+" at the local Rbridge");
            PROVISION_VLAN_RANGE(pContext,pClientMsg->getVlanRange(),PROVISION_SPECIFIED);
			break;
		}
		default:
		{
			NSM_PLUG_INFO_S(string("Provisioning the vlan is not required"));
			break;
		}
	}

	NSM_PLUG_INFO_S(string(" : CmdCode=")+pClientMsg->getCmdCode()+",Vlan Range="+pClientMsg->getVlanRange().toString()+",Ctag Id="+pClientMsg->getCtagId()+",MacAddress="+pClientMsg->getMac()+",IfName="+pClientMsg->getIfName());
	status = sendSynchronouslyToWaveClient("nsm", pClientMsg);

	if(status == WAVE_MESSAGE_SUCCESS)
	{
		status = pClientMsg->getCompletionStatus();

		if(status == WAVE_MESSAGE_SUCCESS)
		{
			status = pClientMsg->getClientStatus();
			//TODO: Switch on the cmd code in pClientMsg
			switch(pMsg->getCmdCode())
			{
				case RSPAN_GVLAN_TRUNK_PORT_DEL_CTAG:
				case RSPAN_GVLAN_ACCESS_PORT_DEL_MAC_ADDRESS:
				case RSPAN_GVLAN_ACCESS_PORT_DEL_MAC_GROUP:
				case GVLAN_ACCESS_PORT_DEL_MAC_ADDRESS:
				case GVLAN_ACCESS_PORT_DEL_MAC_GROUP:
				case GVLAN_TRUNK_PORT_DEL_CTAG:
				case GVLAN_PVLAN_TRUNK_PORT_DEL_CTAG:
				{
	       			//Need to see if unprovision needs to be handled
	       			NSM_PLUG_INFO_S(string("Need to see if unprovision is required for the vlan ")+pClientMsg->getVlanRange().toString());
	       			HANDLE_PROVISION_RESPONSE(pClientMsg);
					break;
				}
				default:
				{
					NSM_PLUG_INFO_S(string("UnProvisioning the vlan need not be verified"));
					break;
				}
			}
		}
	}

	delete pClientMsg;
	pMsg->setCompletionStatus(status);

	return status;
}

ResourceId IntfConfLocalWorker::switchportVlanConfigMOStep (DceSynchronousLinearSeqContext *pContext)
{
	SwitchportVlanConfigLocalMessage *pMsg = dynamic_cast<SwitchportVlanConfigLocalMessage*> (pContext->getPPrismMessage ());
	UI32 cmdCode = pMsg->getCmdCode();
	PhyIntfLocalManagedObject *pPhyMO = NULL; 

	WaveManagedObjectSynchronousQueryContext phySyncQueryCtxt (PhyIntfLocalManagedObject::getClassName ());
	phySyncQueryCtxt.addAndAttribute (new AttributeString (pMsg->getIfName(), "ifName"));
	phySyncQueryCtxt.addSelectField("objectId");
	vector<WaveManagedObject *> *pPhyResults = querySynchronously (&phySyncQueryCtxt);

	if((pPhyResults != NULL) && (pPhyResults->size() != 0))
	{
		pContext->addManagedObjectsForGarbageCollection(*pPhyResults);
		pPhyMO =  dynamic_cast<PhyIntfLocalManagedObject*>((*pPhyResults)[0]);
		pPhyResults->clear();
		delete pPhyResults;
		pPhyResults = NULL;
	}
	else
	{
		trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::switchportVlanConfigMOStep: Interface does not exist."));

		return WRC_NSM_ERR_PHYINTF_INVALID;
	}

	switch(cmdCode)
	{
		case GVLAN_ACCESS_PORT_SET_MAC_ADDRESS:
		{
			MacAddressVlanClassificationMO *pMO = new MacAddressVlanClassificationMO (dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
			pContext->addManagedObjectForGarbageCollection(pMO);
			pMO->setOwnerManagedObjectId(pPhyMO->getObjectId());
			pMO->setAccessVlanId(pMsg->getVlanId());
			pMO->setMac(pMsg->getMac());
			addToComposition(PoPhyIntfManagedObject::getClassName(), MacAddressVlanClassificationMO::getClassName(), "macAddressVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
		}
		break;

		case GVLAN_ACCESS_PORT_DEL_MAC_ADDRESS:
		{
			WaveManagedObjectSynchronousQueryContext accessPortSyncQueryCtxt (MacAddressVlanClassificationMO::getClassName ());
			accessPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getVlanId(), "vlanId"));
			accessPortSyncQueryCtxt.addAndAttribute (new AttributeString (pMsg->getMac(), "mac"));
			accessPortSyncQueryCtxt.addAndAttribute (new AttributeObjectId (pPhyMO->getObjectId(), "ownerManagedObjectId"));
			vector<WaveManagedObject *> *pAccessResults = querySynchronously (&accessPortSyncQueryCtxt);

			if((pAccessResults != NULL) && pAccessResults->size() > 0)
			{
				pContext->addManagedObjectsForGarbageCollection(*pAccessResults);
				MacAddressVlanClassificationMO *pMO =  dynamic_cast<MacAddressVlanClassificationMO*>((*pAccessResults)[0]);
				deleteFromComposition (PoPhyIntfManagedObject::getClassName(), MacAddressVlanClassificationMO::getClassName(), "macAddressVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());

				pAccessResults->clear();
				delete pAccessResults;
			}
		}
		break;

		case GVLAN_ACCESS_PORT_SET_MAC_GROUP:
		{
			MacGroupVlanClassificationMO *pMO = new MacGroupVlanClassificationMO (dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
			pContext->addManagedObjectForGarbageCollection(pMO);
			pMO->setOwnerManagedObjectId(pPhyMO->getObjectId());
			pMO->setVlanId(pMsg->getVlanId());
			pMO->setMacGroupId(pMsg->getMacGroupId());
			addToComposition(PoPhyIntfManagedObject::getClassName(), MacGroupVlanClassificationMO::getClassName(), "macGroupVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
		}
		break;

		case GVLAN_ACCESS_PORT_DEL_MAC_GROUP:
		{
			WaveManagedObjectSynchronousQueryContext macGroupQueryCtxt (MacGroupVlanClassificationMO::getClassName ());
			macGroupQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getVlanId(), "vlanId"));
			macGroupQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getMacGroupId(), "macGroupId"));
			macGroupQueryCtxt.addAndAttribute (new AttributeObjectId (pPhyMO->getObjectId(), "ownerManagedObjectId"));
			vector<WaveManagedObject *> *pMacGroupResults = querySynchronously (&macGroupQueryCtxt);

			if((pMacGroupResults != NULL) && pMacGroupResults->size() > 0)
			{
				pContext->addManagedObjectsForGarbageCollection(*pMacGroupResults);
				MacGroupVlanClassificationMO *pMO =  dynamic_cast<MacGroupVlanClassificationMO*>((*pMacGroupResults)[0]);
				deleteFromComposition (PoPhyIntfManagedObject::getClassName(), MacGroupVlanClassificationMO::getClassName(), "macGroupVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());

				pMacGroupResults->clear();
				delete pMacGroupResults;
			}
		}
		break;

		case GVLAN_TRUNK_PORT_SET_CTAG:
		{
			CtagVlanClassificationMO *pMO = new CtagVlanClassificationMO (dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
			pContext->addManagedObjectForGarbageCollection(pMO);
			pMO->setOwnerManagedObjectId(pPhyMO->getObjectId());
			pMO->setVlanId(pMsg->getVlanId());
			pMO->setCtagId(pMsg->getCtagId());
			addToComposition(PoPhyIntfManagedObject::getClassName(), CtagVlanClassificationMO::getClassName(), "ctagVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
		}
		break;

		case GVLAN_TRUNK_PORT_DEL_CTAG:
		{
			WaveManagedObjectSynchronousQueryContext trunkPortSyncQueryCtxt (CtagVlanClassificationMO::getClassName ());
			trunkPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getVlanId(), "vlanId"));
			trunkPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getCtagId(), "ctagId"));
			trunkPortSyncQueryCtxt.addAndAttribute (new AttributeObjectId (pPhyMO->getObjectId(), "ownerManagedObjectId"));
			vector<WaveManagedObject *> *pTrunkResults = querySynchronously (&trunkPortSyncQueryCtxt);

			if((pTrunkResults != NULL) && (pTrunkResults->size() > 0))
			{
				pContext->addManagedObjectsForGarbageCollection(*pTrunkResults);
				CtagVlanClassificationMO *pMO =  dynamic_cast<CtagVlanClassificationMO*>((*pTrunkResults)[0]);
				deleteFromComposition (PoPhyIntfManagedObject::getClassName(), CtagVlanClassificationMO::getClassName(), "ctagVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());

				pTrunkResults->clear();
				delete pTrunkResults;
			}
		}
		break;

		case RSPAN_GVLAN_ACCESS_PORT_SET_MAC_ADDRESS:
		{
			RspanMacAddressVlanClassificationMO *pMO = new RspanMacAddressVlanClassificationMO (dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
			pMO->setOwnerManagedObjectId(pPhyMO->getObjectId());
			pMO->setAccessVlanId(pMsg->getVlanId());
			pMO->setMac(pMsg->getMac());
			addToComposition(PhyIntfLocalManagedObject::getClassName(), RspanMacAddressVlanClassificationMO::getClassName(), "rspan_macaddress_vlan_classififcation", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
			break;
		}

		case RSPAN_GVLAN_ACCESS_PORT_DEL_MAC_ADDRESS:
		{
			WaveManagedObjectSynchronousQueryContext accessPortSyncQueryCtxt (RspanMacAddressVlanClassificationMO::getClassName ());
			accessPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getVlanId(), "vlanId"));
			accessPortSyncQueryCtxt.addAndAttribute (new AttributeString (pMsg->getMac(), "mac"));
			accessPortSyncQueryCtxt.addAndAttribute (new AttributeObjectId (pPhyMO->getObjectId(), "ownerManagedObjectId"));
			vector<WaveManagedObject *> *pAccessResults = querySynchronously (&accessPortSyncQueryCtxt);
			pContext->addManagedObjectsForGarbageCollection(*pAccessResults);
			if((pAccessResults != NULL) && pAccessResults->size() > 0)
			{
				RspanMacAddressVlanClassificationMO *pMO =  dynamic_cast<RspanMacAddressVlanClassificationMO*>((*pAccessResults)[0]);
				deleteFromComposition (PhyIntfLocalManagedObject::getClassName(), RspanMacAddressVlanClassificationMO::getClassName(), "rspan_macaddress_vlan_classififcation", pMO->getOwnerManagedObjectId(), pMO->getObjectId());

				pAccessResults->clear();
				delete pAccessResults;
			}
			break;
		}

		case RSPAN_GVLAN_ACCESS_PORT_SET_MAC_GROUP:
		{
			RspanMacGroupVlanClassificationMO *pMO = new RspanMacGroupVlanClassificationMO (dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
			pContext->addManagedObjectForGarbageCollection(pMO);
			pMO->setOwnerManagedObjectId(pPhyMO->getObjectId());
			pMO->setVlanId(pMsg->getVlanId());
			pMO->setMacGroupId(pMsg->getMacGroupId());
			addToComposition(PhyIntfLocalManagedObject::getClassName(), RspanMacGroupVlanClassificationMO::getClassName(), "rspan_macgroup_vlan_classififcation", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
		}
		break;

		case RSPAN_GVLAN_ACCESS_PORT_DEL_MAC_GROUP:
		{
			WaveManagedObjectSynchronousQueryContext macGroupQueryCtxt (RspanMacGroupVlanClassificationMO::getClassName ());
			macGroupQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getVlanId(), "vlanId"));
			macGroupQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getMacGroupId(), "macGroupId"));
			macGroupQueryCtxt.addAndAttribute (new AttributeObjectId (pPhyMO->getObjectId(), "ownerManagedObjectId"));
			vector<WaveManagedObject *> *pMacGroupResults = querySynchronously (&macGroupQueryCtxt);

			if((pMacGroupResults != NULL) && pMacGroupResults->size() > 0)
			{
				pContext->addManagedObjectsForGarbageCollection(*pMacGroupResults);
				RspanMacGroupVlanClassificationMO *pMO =  dynamic_cast<RspanMacGroupVlanClassificationMO*>((*pMacGroupResults)[0]);
				deleteFromComposition (PhyIntfLocalManagedObject::getClassName(), RspanMacGroupVlanClassificationMO::getClassName(), "rspan_macgroup_vlan_classififcation", pMO->getOwnerManagedObjectId(), pMO->getObjectId());

				pMacGroupResults->clear();
				delete pMacGroupResults;
			}
		}
		break;

		case RSPAN_GVLAN_TRUNK_PORT_SET_CTAG:
		{
			RspanGVlanTrunkPortManagedObject *pMO = new RspanGVlanTrunkPortManagedObject (dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
			pMO->setOwnerManagedObjectId(pPhyMO->getObjectId());
			pMO->setVlanId(pMsg->getVlanId());
			pMO->setCtagId(pMsg->getCtagId());
			addToComposition(PhyIntfLocalManagedObject::getClassName(), RspanGVlanTrunkPortManagedObject::getClassName(), "rspan_gvlan_trunk", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
			break;
		}

		case RSPAN_GVLAN_TRUNK_PORT_DEL_CTAG:
		{
			WaveManagedObjectSynchronousQueryContext trunkPortSyncQueryCtxt (RspanGVlanTrunkPortManagedObject::getClassName ());
			trunkPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getVlanId(), "vlanId"));
			trunkPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getCtagId(), "ctagId"));
			trunkPortSyncQueryCtxt.addAndAttribute (new AttributeObjectId (pPhyMO->getObjectId(), "ownerManagedObjectId"));
			vector<WaveManagedObject *> *pTrunkResults = querySynchronously (&trunkPortSyncQueryCtxt);
			pContext->addManagedObjectsForGarbageCollection(*pTrunkResults);

			if((pTrunkResults != NULL) && (pTrunkResults->size() > 0))
			{
				RspanGVlanTrunkPortManagedObject *pMO =  dynamic_cast<RspanGVlanTrunkPortManagedObject*>((*pTrunkResults)[0]);
				deleteFromComposition (PhyIntfLocalManagedObject::getClassName(), RspanGVlanTrunkPortManagedObject::getClassName(), "rspan_gvlan_trunk", pMO->getOwnerManagedObjectId(), pMO->getObjectId());

				pTrunkResults->clear();
				delete pTrunkResults;
			}
			break;
		}

		case GVLAN_PVLAN_TRUNK_PORT_SET_CTAG:
		{
			CtagPVlanClassificationMO *pMO = new CtagPVlanClassificationMO (dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
			pContext->addManagedObjectForGarbageCollection(pMO);
			pMO->setOwnerManagedObjectId(pPhyMO->getObjectId());
			pMO->setVlanId(pMsg->getVlanId());
			pMO->setCtagId(pMsg->getCtagId());
			addToComposition(PoPhyIntfManagedObject::getClassName(), CtagPVlanClassificationMO::getClassName(), "ctagPVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
		}
		break;

		case GVLAN_PVLAN_TRUNK_PORT_DEL_CTAG:
		{
			WaveManagedObjectSynchronousQueryContext trunkPortSyncQueryCtxt (CtagPVlanClassificationMO::getClassName ());
			trunkPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getVlanId(), "vlanId"));
			trunkPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getCtagId(), "ctagId"));
			trunkPortSyncQueryCtxt.addAndAttribute (new AttributeObjectId (pPhyMO->getObjectId(), "ownerManagedObjectId"));
			vector<WaveManagedObject *> *pTrunkResults = querySynchronously (&trunkPortSyncQueryCtxt);

			if((pTrunkResults != NULL) && (pTrunkResults->size() > 0))
			{
				pContext->addManagedObjectsForGarbageCollection(*pTrunkResults);
				CtagPVlanClassificationMO *pMO =  dynamic_cast<CtagPVlanClassificationMO*>((*pTrunkResults)[0]);
				deleteFromComposition (PoPhyIntfManagedObject::getClassName(), CtagPVlanClassificationMO::getClassName(), "ctagPVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());

				pTrunkResults->clear();
				delete pTrunkResults;
			}
		}
		break;

		default:
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::switchportVlanConfigMOStep: Invalid command has been entered."));

			return WAVE_MESSAGE_ERROR;
		}
		break;
	}

	return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfConfLocalWorker::getAllCtagVlans(UI32Range &allCtagVlans) {
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    //Get all the vlan ids and populate in a vector of UI32.
    //TODO : MO iterator??
    WaveManagedObjectSynchronousQueryContext vlanIntfQueryCtxt(VlanIntfManagedObject::getClassName());
    vlanIntfQueryCtxt.addSelectField("id");
    vector<WaveManagedObject *> *vlanIntfResults = querySynchronously (&vlanIntfQueryCtxt);
    VlanIntfManagedObject* vlanIntfMO = NULL;
    vector<UI32> allVlans;

    if(NULL != vlanIntfResults) {
        UI32 numberOfVlans = vlanIntfResults->size();
        for(unsigned int vlanIfIter = 0 ; vlanIfIter < numberOfVlans ; vlanIfIter++) {
            vlanIntfMO = dynamic_cast<VlanIntfManagedObject*> ((*vlanIntfResults)[vlanIfIter]);
            if(vlanIntfMO->getId() < 4096 ) {
                allCtagVlans+=vlanIntfMO->getId();
            }
        }
    } else {
        tracePrintf(TRACE_LEVEL_ERROR, "no vlan interfaces found in the system.");
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(vlanIntfResults);

    return status;
}

ResourceId IntfConfLocalWorker::getAllVlansToBeProvisioned
(vector<string>  &portProfiles, string &portProfileDomainName, UI32Range &toBeProvisionedVlanRange) {

    tracePrintf(TRACE_LEVEL_INFO, "Inside port-profile-domain <%s>", portProfileDomainName.c_str());

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if( ((portProfiles.size()) > 0 ) && (!portProfileDomainName.empty())) {
        status = WAVE_MESSAGE_ERROR;
        return status;
    }

    WaveManagedObjectSynchronousQueryContext vlanProfilesQueryCtxt(VlanProfileCfgManagedObject::getClassName());

    vlanProfilesQueryCtxt.addSelectField("isAllowedVlanAll");
    vlanProfilesQueryCtxt.addSelectField("isAllowedVlanAllV2");
    vlanProfilesQueryCtxt.addSelectField("isAllowedVlanNone");
    vlanProfilesQueryCtxt.addSelectField("accessVlan");
    vlanProfilesQueryCtxt.addSelectField("vlanRangeAdd");
    vlanProfilesQueryCtxt.addSelectField("vlanRangeRemove");
    vlanProfilesQueryCtxt.addSelectField("vlanRangeExcept");
    vlanProfilesQueryCtxt.addSelectField("nativeVlanIntfConfig");
    vlanProfilesQueryCtxt.addSelectField("swModeFlag");
    vlanProfilesQueryCtxt.addSelectField("swMode");
    vlanProfilesQueryCtxt.addSelectField("macVlanClassifications");
    vlanProfilesQueryCtxt.addSelectField("macGroupVlanClassifications");
    vlanProfilesQueryCtxt.addSelectField("ctagVlanClassifications");

    /*if(0 == (portProfileDomainName.compare("default"))) {
        //Without f/w upgrade, doesnt make sense to add code here.
        //Need to read all the vlan profiles to get union of all 802.1 vlans.
        tracePrintf(TRACE_LEVEL_INFO,"Inside getAllVlansToBeProvisioned port-profile-domain <default> is getting associated.");
    } else*/ if( portProfiles.size() > 0) {
        //vlanProfilesQueryCtxt.addAndAttribute(new AttributeString(portProfileName),"description");
        unsigned int it = 0 ;
        for(it = 0 ; it < portProfiles.size() ; it++) {
            string profileName = portProfiles[it];
            tracePrintf(TRACE_LEVEL_INFO,"Inside getAllVlansToBeProvisioned port-profile <%s> is getting associated.", profileName.c_str());
            vlanProfilesQueryCtxt.addOrAttribute(new AttributeString(profileName,"description"));
        }
    } else if(!portProfileDomainName.empty()) {
        vector<string> portProfilesOfADomain;
        APPMUtils::getPortProfilesOfAPortProfileDomain(portProfileDomainName, portProfilesOfADomain);
        tracePrintf(TRACE_LEVEL_INFO,"Inside getAllVlansToBeProvisioned <%s> prort-profile is getting associated.", portProfileDomainName.c_str());
        unsigned int it = 0 ;
        for(it = 0 ; it < portProfilesOfADomain.size() ; it++) {
            string profileName = portProfilesOfADomain[it];
            tracePrintf(TRACE_LEVEL_INFO,"Inside getAllVlansToBeProvisioned <%s> prort-profile is getting associated.", profileName.c_str());
            vlanProfilesQueryCtxt.addOrAttribute(new AttributeString(profileName,"description"));
        }
    }

    vector<WaveManagedObject *> *vlanProfileResults = querySynchronously (&vlanProfilesQueryCtxt);
    bool switchPortAll = false;
    UI32Range allVlanRange;
    UI32Range addVlanRange;
    UI32Range removeRange;

    vector<WaveManagedObjectPointer<APPMMacAddressVlanClassificationMO> >   macVlanClassifications;
    vector<WaveManagedObjectPointer<APPMMacGroupVlanClassificationMO> >     macGroupVlanClassifications;
    vector<WaveManagedObjectPointer<APPMCtagVlanClassificationMO> >         ctagVlanClassifications;

    if(NULL != vlanProfileResults) {
        UI32 numberOfVlanProfiles = vlanProfileResults->size();
        for(unsigned int i = 0 ; i < numberOfVlanProfiles; i++) {
            VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject*>((*vlanProfileResults)[i]);
            APPMNativeVlanManagedObject* pAPPMNativeVlanManagedObject = NULL;

            if(true == (pVlanProfileCfgManagedObject->getSwModeFlag())) {
                if(VLAN_SW_MODE_TRUNK == (pVlanProfileCfgManagedObject->getSwMode())) {
                    //TODO : none field of vlan is not reliable, as its not updated always.
                    if(true == (pVlanProfileCfgManagedObject->getIsAllowedVlanNone())) {
                        continue;
                    }
                    if(true == (pVlanProfileCfgManagedObject->getIsAllowedVlanAllV2())) {
                        if( false == switchPortAll) {
                            switchPortAll = true;
                            getAllCtagVlans(allVlanRange);
                            removeRange =(pVlanProfileCfgManagedObject->getVlanRangeRemove());
                        } else {
                            removeRange = removeRange.intersection(pVlanProfileCfgManagedObject->getVlanRangeRemove());
                        }
                    } else {

                        addVlanRange+=(pVlanProfileCfgManagedObject->getVlanRangeAdd());
                        
                        //Get all the vlans having classifications and add to the addRange.
                        ctagVlanClassifications = pVlanProfileCfgManagedObject->getCtagVlanClassifications();
                        unsigned int ctagIt = 0;
                        for(ctagIt = 0 ; ctagIt < ctagVlanClassifications.size(); ctagIt++) {
                            addVlanRange+=ctagVlanClassifications[ctagIt]->getVlanId();
                        }
                        ctagVlanClassifications.clear();
                    }
                    pAPPMNativeVlanManagedObject = ((pVlanProfileCfgManagedObject->getNativeVlanIntfConfig()).operator->());
                    if((APPM_NSM_NATIVE_VLAN_DEFAULT) != (pAPPMNativeVlanManagedObject->getNativeVlanId())) {
                        addVlanRange+=(pAPPMNativeVlanManagedObject->getNativeVlanId());
                    }
                } else if(VLAN_SW_MODE_ACCESS == (pVlanProfileCfgManagedObject->getSwMode())) {

                    addVlanRange+=(pVlanProfileCfgManagedObject->getAccessVlan());

                    macVlanClassifications = pVlanProfileCfgManagedObject->getMacVlanClassifications();
                    macGroupVlanClassifications = pVlanProfileCfgManagedObject->getMacGroupVlanClassifications();

                    unsigned int macIt = 0;
                    for(macIt = 0 ; macIt < macVlanClassifications.size(); macIt++) {
                        addVlanRange+=macVlanClassifications[macIt]->getAccessVlanId();
                    }
                    macVlanClassifications.clear();

                    unsigned int macGroupIt = 0;
                    for(macGroupIt = 0 ; macGroupIt < macGroupVlanClassifications.size(); macGroupIt++) {
                        addVlanRange+=macGroupVlanClassifications[macGroupIt]->getVlanId();
                    }
                    macGroupVlanClassifications.clear();

                } else {
                }
            }
        }

        if(true == switchPortAll) {
            toBeProvisionedVlanRange = (allVlanRange - removeRange) + addVlanRange;
        } else {
            toBeProvisionedVlanRange = addVlanRange;
        }
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(vlanProfileResults);
    } else {
        status = WAVE_MESSAGE_ERROR;
        tracePrintf(TRACE_LEVEL_INFO,"No vlan profiles found in the system.");
    }

    return status;
}    



    ResourceId IntfConfLocalWorker::provisionRequiredVlansForPort(
            DceContext *context, const UI32 cmdCode, const bool profiledPort) {
        switch (cmdCode) {
            case NSMUPDATE_PHY_SW_TRUNK_VLAN_ALL:
            case NSMUPDATE_PO_SW_TRUNK_VLAN_ALL:
            case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ALL:
            case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ALL: {
                PROVISION_ALL_VLANS(context);
                break;
            }
            case NSMUPDATE_PHY_PORT_PROFILED:
            case NSMUPDATE_PO_PORT_PROFILED: {
                if (profiledPort) {
                    UI32Range toBeProvisionedVlanRange;
                    //string portProfileName = "";
                    string portProfileDomainName = "default";
                    vector<string> portProfiles;
                    //portProfiles.push_back(portProfileName);

                    getAllVlansToBeProvisioned(portProfiles, portProfileDomainName,
                            toBeProvisionedVlanRange);
                    tracePrintf(
                            TRACE_LEVEL_INFO,
                            "NSMUPDATE-Po/Phy PortProfiled, toBeProvisionedVlanRange<%s>",
                            toBeProvisionedVlanRange.toString().c_str());
                    //Better write a util in APPM and use it here, the same can be
                    //reused to populate all the vlans present in a given pp domain.
                    PROVISION_VLAN_RANGE(context, toBeProvisionedVlanRange,
                            PROVISION_SPECIFIED);
                }
                break;
            }
            default:
                return WAVE_MESSAGE_SUCCESS;
                break;
        }
        return WAVE_MESSAGE_SUCCESS;
    }
    vlan_provision_code IntfConfLocalWorker::mapCmdCodeToProvisionCode(
            const UI32 &cmdCode) {
        vlan_provision_code provisionCode;
        switch (cmdCode) {
            case NSMUPDATE_PO_SW_ADD_ACCESS_VLAN:
            case NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN:
            case NSMUPDATE_PHY_SW_ACCESS_ADD_RSPANVLAN:
			case GVLAN_ACCESS_PORT_SET_MAC_ADDRESS:
			case GVLAN_ACCESS_PORT_SET_MAC_GROUP:
                provisionCode = PROVISION_SPECIFIED;
                break;
            case NSMUPDATE_PO_SW_ADD_TRUNK_VLAN:
            case NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN:
            case NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN:
            case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ADD:
            case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ADD:
			case GVLAN_TRUNK_PORT_SET_CTAG:
			case GVLAN_PVLAN_TRUNK_PORT_SET_CTAG:
                provisionCode = PROVISION_SPECIFIED;
                break;
            case NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN:
            case NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN:
            case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_EXCEPT:
            case NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_EXCEPT:
                provisionCode = PROVISION_EXCEPT;
                break;
            default:
                provisionCode = PROVISION_NOT_USED;
                break;
        }
        return provisionCode;
    }

void IntfConfLocalWorker::macGroupConfigLocalMessageHandler (MacGroupConfigLocalMessage *pMacGroupConfigLocalMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::macGroupConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
    };

    DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pMacGroupConfigLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pDceSynchronousLinearSeqContext->execute ();
}

ResourceId IntfConfLocalWorker::macGroupConfigStep (DceSynchronousLinearSeqContext *pContext)
{
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	MacGroupConfigLocalMessage *pMsg = dynamic_cast<MacGroupConfigLocalMessage*> (pContext->getPPrismMessage ());
	MacGroupConfigMessage *pClientMsg = new MacGroupConfigMessage();

	pClientMsg->setCmdCode(pMsg->getCmdCode());
	pClientMsg->setMacGroupId(pMsg->getMacGroupId());

	if((pMsg->getCmdCode() == MAC_GROUP_SET_ENTRY) || (pMsg->getCmdCode() == MAC_GROUP_DEL_ENTRY))
	{
		pClientMsg->setMacEntry(pMsg->getMac(), pMsg->getMask());
	}

	status = sendSynchronouslyToWaveClient("nsm", pClientMsg);

	if(status == WAVE_MESSAGE_SUCCESS)
	{
		status = pClientMsg->getCompletionStatus();

		if(status == WAVE_MESSAGE_SUCCESS)
		{
			status = pClientMsg->getClientStatus();
		}
	}

	pMsg->setCompletionStatus(status);
	delete pClientMsg;

	return status;
}

//Below sequencer steps handle the provisioning message, used to aggregate the 
//number of provisioned Mos in a cluster.

void IntfConfLocalWorker::NsmLocalProvisioningInfoMessageHandler 
(NsmLocalProvisioningInfoMessage* pNsmLocalProvisioningInfoMessage) {

    if(pNsmLocalProvisioningInfoMessage->getIsGwRequest()) {
        PrismLinearSequencerStep sequencerSteps[] =  {
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfLocalWorker::sendNsmLocalProvisioningInfoMessageToCluster),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfLocalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfLocalWorker::prismLinearSequencerFailedStep)
        };
        PrismLinearSequencerContext *pPrismLinearSequencerContext =
            new PrismLinearSequencerContext(pNsmLocalProvisioningInfoMessage,
                    this, sequencerSteps,
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
        return;

    } else {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =  {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::populateProvisionedCacheDataOfThisNode),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext *pPrismLinearSequencerContext =
            new PrismSynchronousLinearSequencerContext(pNsmLocalProvisioningInfoMessage,
                    this, sequencerSteps,
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

        pPrismLinearSequencerContext->execute ();
    }
}

void IntfConfLocalWorker::sendNsmLocalProvisioningInfoMessageToCluster
(PrismLinearSequencerContext* pPrismLinearSequencerContext) {

    NsmLocalProvisioningInfoMessage* pNsmLocalProvisioningInfoMessage = NULL;
    NsmLocalProvisioningInfoMessage* pNsmLocalProvisioningInfoMessageLocal = NULL;
    WaveSendToClusterContext *pWaveSendToClusterContext = NULL;
    pNsmLocalProvisioningInfoMessage 
        = dynamic_cast<NsmLocalProvisioningInfoMessage*>  (pPrismLinearSequencerContext->getPPrismMessage());

    if (NULL == pNsmLocalProvisioningInfoMessage)
    {
        // FIXME : Sagar : Please handle this condition.
    }

    pNsmLocalProvisioningInfoMessageLocal = new NsmLocalProvisioningInfoMessage();
    pNsmLocalProvisioningInfoMessageLocal->setIsGwRequest(false);

    pWaveSendToClusterContext = new WaveSendToClusterContext(this, 
            reinterpret_cast<PrismAsynchronousCallback>(&IntfConfLocalWorker::
                sendNsmLocalProvisioningInfoMessageToClusterCallBack),pPrismLinearSequencerContext);

    pWaveSendToClusterContext->setPPrismMessageForPhase1(pNsmLocalProvisioningInfoMessageLocal);
    pWaveSendToClusterContext->setPartialSuccessFlag(true);
    pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
    sendToWaveCluster(pWaveSendToClusterContext);
    return;
}

void IntfConfLocalWorker::sendNsmLocalProvisioningInfoMessageToClusterCallBack
(WaveSendToClusterContext* pWaveSendToClusterContext) {

    PrismLinearSequencerContext* pPrismLinearSequencerContext = NULL;
    NsmLocalProvisioningInfoMessage* req_msg_p = NULL;
    NsmLocalProvisioningInfoMessage* resp_msg_p = NULL;
    NsmLocalProvisioningInfoMessage* sw_resp_p = NULL;

    pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext*>
        (pWaveSendToClusterContext->getPCallerContext());
    req_msg_p = dynamic_cast<NsmLocalProvisioningInfoMessage*>
        (pPrismLinearSequencerContext->getPPrismMessage());
    resp_msg_p = dynamic_cast<NsmLocalProvisioningInfoMessage*>
        (pWaveSendToClusterContext->getPPrismMessageForPhase1());

    UI32Range totalNumberOfProvisionedVlans;

    ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
    if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus) {
        vector<LocationId> locations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
        UI32 i = 0;

        for(i = 0; i < locations.size() ; i++ ) {
            ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation
                (pWaveSendToClusterContext, locations[i]);
            if ( WAVE_MESSAGE_SUCCESS == statusPerLocationId ) {
                sw_resp_p = dynamic_cast<NsmLocalProvisioningInfoMessage*>
                    (pWaveSendToClusterContext->getResultingMessageForPhase1(locations[i]));
                UI32Range temp = sw_resp_p->getProvisionedMORange();
                totalNumberOfProvisionedVlans += temp;
            } else {
                tracePrintf(TRACE_LEVEL_ERROR,
                    "IntfConfLocalWorker::sendNsmLocalProvisioningInfoMessageToClusterCallBack Error: <%s> On Location : <%d>" 
                    ,(FrameworkToolKit::localize(statusPerLocationId)).c_str(), locations[i]);
            }
        }
    } else {
        tracePrintf(TRACE_LEVEL_ERROR,
                    "IntfConfLocalWorker::sendNsmLocalProvisioningInfoMessageToClusterCallBack returned error: <%s>"
                    ,(FrameworkToolKit::localize(sendToClusterCompletionStatus)).c_str());
    }

    tracePrintf(TRACE_LEVEL_INFO, 
            "sendNsmLocalProvisioningInfoMessageToClusterCallBack totalNumberOfProvisionedVlans <%s>",
            totalNumberOfProvisionedVlans.toString().c_str());

    req_msg_p->setProvisionedMORange(totalNumberOfProvisionedVlans);

    pPrismLinearSequencerContext->executeNextStep(sendToClusterCompletionStatus);
    
    delete resp_msg_p;
    delete pWaveSendToClusterContext;

    return;
}

ResourceId IntfConfLocalWorker::populateProvisionedCacheDataOfThisNode
(PrismSynchronousLinearSequencerContext* pPrismSynchronousLinearSequencerContext) {

    NsmLocalProvisioningInfoMessage* pNsmLocalProvisioningInfoMessage =
        dynamic_cast<NsmLocalProvisioningInfoMessage*> 
        (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
    tracePrintf(TRACE_LEVEL_INFO, "populateProvisionedCacheDataOfThisNode: number of provisioned mos <%s>"
        , (ProvisioningUtil::getProviosionedMORange()).toString().c_str());
    pNsmLocalProvisioningInfoMessage->setProvisionedMORange(ProvisioningUtil::getProviosionedMORange());
    pNsmLocalProvisioningInfoMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfConfLocalWorker::provisionVlansForSwitchportPo(PoIntfManagedObject *pPoMO)
{
	UI32 index = 0;
	UI32Range provisionVlanRange;
	string poId = pPoMO->getIfName();

	if (pPoMO->getSwMode() == SW_MODE_ACCESS)
	{
		UI32Range accessVlan;
		ObjectId obj = pPoMO->getAccessVlanObjectId();
		vector<WaveManagedObjectPointer<MacAddressVlanClassificationMO> > macAddressVector = pPoMO->getMacAddressVlanClassification();
		vector<WaveManagedObjectPointer<MacGroupVlanClassificationMO> > macGroupVector = pPoMO->getMacGroupVlanClassification();

		accessVlan.fromString(NsmUtils::getVlanIntfNameByObjectId(obj));
		provisionVlanRange += accessVlan;

		for (index = 0; index < macAddressVector.size(); index++)
		{
			provisionVlanRange += macAddressVector[index]->getAccessVlanId(); 
		}

		for (index = 0; index < macGroupVector.size(); index++)
		{
			provisionVlanRange += macGroupVector[index]->getVlanId();
		}
	}
	else if(pPoMO->getSwMode() == SW_MODE_TRUNK)
	{
		WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanMO = pPoMO->getNativeVlanIntfConfig();
		vector<WaveManagedObjectPointer<CtagVlanClassificationMO> > ctagVlanVector = pPoMO->getCtagVlanClassification();

		if(pPoMO->getIsAllowedVlanAll()) 
		{
			if(pPoMO->getVlanRangeRemove().isEmpty())
			{
				trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::provisionVlansForSwitchportPo:Provision All Vlans for trunk mode on po:") + poId);

				PROVISION_ALL_VLANS(NULL);
			}
			else
			{
				trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::provisionVlansForSwitchportPo:Provision Except Vlans for trunk mode on po:") + poId + string(" with except vlan range:") + pPoMO->getVlanRangeRemove().toString());

				PROVISION_VLAN_RANGE(NULL, pPoMO->getVlanRangeRemove(), PROVISION_EXCEPT);
			}
		}
		else
		{
			if(!(pPoMO->getVlanRangeAdd().isEmpty()))
			{
				provisionVlanRange += pPoMO->getVlanRangeAdd();
			}
		}

		if(pNativeVlanMO.operator->() != NULL)
		{
			provisionVlanRange += pNativeVlanMO->getNativeVlanId();
		}

		for (index = 0; index < ctagVlanVector.size(); index++)
		{
			provisionVlanRange += ctagVlanVector[index]->getVlanId();
		}
	}
	else if (pPoMO->getPvlanModeTrunkHost())
	{
		vector<WaveManagedObjectPointer<CtagVlanClassificationMO> > ctagVlanVector = pPoMO->getCtagVlanClassification();
		provisionVlanRange += pPoMO->getPvlanHostPrimVlanId();
		//provisionVlanRange += pPoMO->getPvlanHostSecVlanId();

		for (index = 0; index < ctagVlanVector.size(); index++)
		{
			provisionVlanRange += ctagVlanVector[index]->getVlanId();
		}
	}
	else if (pPoMO->getPvlanModeHost())
	{
		provisionVlanRange += pPoMO->getPvlanHostPrimVlanId();
		//provisionVlanRange += pPoMO->getPvlanHostSecVlanId();
	}
	else if (pPoMO->getPvlanModeTrunkPromis())
	{
		WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanMO = pPoMO->getNativeVlanIntfConfig();
		vector<WaveManagedObjectPointer<PVlanMappingManagedObject> > pVlanMappingVector = pPoMO->getPVlanMapping();
		vector<WaveManagedObjectPointer<CtagVlanClassificationMO> > ctagVlanVector = pPoMO->getCtagVlanClassification();

		if(pPoMO->getIsAllowedVlanAll())
		{
			if(pPoMO->getVlanRangeRemove().isEmpty())
			{
				trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::provisionVlansForSwitchportPo:Provision All Vlans for trunk promiscuous mode on po:") + poId);

				PROVISION_ALL_VLANS(NULL);
			}
			else
			{
				trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::provisionVlansForSwitchportPo:Provision Except Vlans for trunk promiscuous mode on po:") + poId + string(" with except vlan range:") + pPoMO->getVlanRangeRemove().toString());

				PROVISION_VLAN_RANGE(NULL, pPoMO->getVlanRangeRemove(), PROVISION_EXCEPT);
			}
		}
		else
		{
			if(!(pPoMO->getVlanRangeAdd().isEmpty()))
			{
				provisionVlanRange += pPoMO->getVlanRangeAdd();
			}
		}

		if(pNativeVlanMO.operator->() != NULL)
		{
			provisionVlanRange += pNativeVlanMO->getNativeVlanId();
		}

		for (index = 0; index < pVlanMappingVector.size(); index++)
		{
			provisionVlanRange += pVlanMappingVector[index]->getPrimaryVlanId();
			//privisionVlanRange += pVlanMappingVector[index]->getSecPvlanRange();
		}

		for (index = 0; index < ctagVlanVector.size(); index++)
		{
			provisionVlanRange += ctagVlanVector[index]->getVlanId();
		}
	}
	else if (pPoMO->getPvlanModePromiscuous())
	{
		vector<WaveManagedObjectPointer<PVlanMappingManagedObject> > pVlanMappingVector = pPoMO->getPVlanMapping();

		for (index = 0; index < pVlanMappingVector.size(); index++)
		{
			provisionVlanRange += pVlanMappingVector[index]->getPrimaryVlanId();
			//privisionVlanRange += pVlanMappingVector[index]->getSecPvlanRange();
		}
	}
	else if (pPoMO->getPvlanModeTrunkBas())
	{
		vector<WaveManagedObjectPointer<PVlanTrunkAssocManagedObject> > pVlanTrunkAssocVector = pPoMO->getPVlanTrunkAssoc();
		vector<WaveManagedObjectPointer<CtagPVlanClassificationMO> > ctagPVlanVector = pPoMO->getCtagPVlanClassification();

		provisionVlanRange += pPoMO->getPvlanNativeVlanId();

		if(pPoMO->getPvlanIsAllowedVlanAll())
		{
			if(pPoMO->getPvlanRangeRemove().isEmpty())
			{
				//TODO: Provision all Vlans or PVlans
				trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::provisionVlansForSwitchportPo: PVlan Trunk All is set."));
			}
			else
			{
				//TODO: Provision all but remove Vlans or PVlans
				trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::provisionVlansForSwitchportPo: PVlan Trunk Except is set."));
			}
		}
		else if(!(pPoMO->getPvlanRangeAdd().isEmpty()))
		{
			provisionVlanRange += pPoMO->getPvlanRangeAdd();
		}

		for (index = 0; index < pVlanTrunkAssocVector.size(); index++)
		{
			provisionVlanRange += pVlanTrunkAssocVector[index]->getPrimaryVlanId();
			//provisionVlanRange += pVlanTrunkAssocVector[index]->getSecondaryVlanId();
		}

		for (index = 0; index < ctagPVlanVector.size(); index++)
		{
			provisionVlanRange += ctagPVlanVector[index]->getVlanId();
		}
	}
	else
	{
		trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::provisionVlansForSwitchportPo: Invalid Switchport Mode."));
	}

	provisionVlanRange = provisionVlanRange - NSM_NATIVE_VLAN_DEFAULT;

	if (!(provisionVlanRange.isEmpty()))
	{
		trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::privisionVlansForSwitchportPo: Provisioning the following Vlans for port-channel:") + provisionVlanRange.toString());

		PROVISION_VLAN_RANGE(NULL, provisionVlanRange, PROVISION_SPECIFIED);
	}

	return WAVE_MESSAGE_SUCCESS;
}

void IntfConfLocalWorker::gVlanEnableLocalMessageHandler (GVlanEnableLocalMessage *pGVlanEnableLocalMessage)
{
	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::enableGVlanStg),
		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pGVlanEnableLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pDceSynchronousLinearSeqContext->execute ();
}

ResourceId IntfConfLocalWorker::enableGVlanStg(DceSynchronousLinearSeqContext *pContext)
{
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	GVlanEnableLocalMessage *pLocalMsg = dynamic_cast<GVlanEnableLocalMessage *> (pContext->getPPrismMessage ());
	gvlan_enable_stage_t cmdCode = pLocalMsg->getCmdCode();
	bool gVlanEnableFlag = pLocalMsg->getEnable();

	trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::enableGVlanStg:received gvlan enable stage:") + cmdCode);

	if (cmdCode != GVLAN_ENABLE_STG_5)
	{
		GVlanEnableMessage *pClientMsg = new GVlanEnableMessage();
		pClientMsg->setCmdCode(cmdCode);
		pClientMsg->setEnable(gVlanEnableFlag);
		pClientMsg->setUndo(pLocalMsg->getUndo());
		status = sendSynchronouslyToWaveClient("nsm", pClientMsg);

		if(status == WAVE_MESSAGE_SUCCESS)
		{
			status = pClientMsg->getCompletionStatus();

			if(status == WAVE_MESSAGE_SUCCESS)
			{
				status = pClientMsg->getClientStatus();
			}
		}
	}
	else
	{
		// enable GVlan feature through HASM for firmware upgrade/downgrade purposes.
		ValClientSynchronousConnection *pValClientSynchronousConnection = DcmToolKit::getInterDcmCcmClient();

		prismAssert(NULL != pValClientSynchronousConnection, __FILE__, __LINE__);

		UI32 gVlanState = gVlanEnableFlag;
		char featureName[] = NSM_GVLAN_PERSISTENCE_MO_NAME;

		if(gVlanState == GVLAN_STATE_DISABLE)
		{
			pValClientSynchronousConnection->setHasmFeature(featureName, GVLAN_STATE_DISABLE);
		}
		else
		{
			pValClientSynchronousConnection->setHasmFeature(featureName, GVLAN_STATE_ENABLE);
		}

		trace(TRACE_LEVEL_INFO, string("IntfConfLocalWorker::enableGVlanStg:set GVlan feature flag via HASM with the stage:") + gVlanState);
	}

	pLocalMsg->setCompletionStatus(status);

	return status;
}
}
