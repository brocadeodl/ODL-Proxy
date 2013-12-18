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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "DcmResourceIdEnums.h"
#include <algorithm>
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForUpdate.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForUpgradePhase.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Utils/StringUtils.h"
#include "WaveResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/IntfConfGlobalWorker.h"
#include "Nsm/Local/IntfConfLocalWorker.h"
#include "Nsm/Global/NsmGlobalCreateVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdateVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalDeleteVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalCreatePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalAddPhyIntfToPoIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdatePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalDeletePoIntfMessage.h"
#include "Nsm/Local/NsmDeletePhyIntfToPoIntfMessage.h"
#include "Nsm/Global/NsmGlobalAddPoToVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalDeletePoToVlanIntfMessage.h"
#include "Nsm/Local/PortSecurityLocalManagedObject.h"
#include "APPM/Global/APPMGlobalAPPMVlanMessage.h"
#include "APPM/Global/APPMGlobalConfWorker.h"
#include "APPM/Global/AMPPProfileCompositeManagedObject.h"
#include "APPM/Global/AMPPProfileDomainCompositeManagedObject.h"
#include "APPM/Utils/APPMUtils.h"
#include "Utils/ProvisioningUtil.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "Nsm/Global/IntfManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Local/XstpPortConfigManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Local/NsmCreateVlanIntfSPMessage.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/NsmDeleteVlanIntfSPMessage.h"
#include "Nsm/Local/NsmUpdateVlanIntfSPMessage.h"
#include "Nsm/Local/NsmUpdatePoIntfSPMessage.h"
#include "Nsm/Local/NsmCreatePoIntfSPMessage.h"
#include "Nsm/Local/NsmDeletePoIntfSPMessage.h"
#include "Nsm/Local/NsmAddPhyIntfToPoIntfMessage.h"
#include "Nsm/Local/Layer3/NsmLocalNodeIpMessage.h"


/* Ip Acl */
#include "Nsm/Global/NsmGlobalReAssocIpAclPolicyMessage.h"
#include "Nsm/Global/NsmGlobalAssocIpAclPolicyPoVlanIntfMessage.h"
#include "Nsm/Local/Ssm/NsmAssocIpAclPolicyPoVlanIntfSPMessage.h"
#include "Ssm/Global/SsmGlobalGetIpAclPolicyMessage.h"
#include "Nsm/Local/Ssm/NsmLocalIngressEgressIpAclPolicyManagedObject.h"

#include "Nsm/Local/NsmAssocMacAclPolicyPoVlanIntfSPMessage.h"
#include "Nsm/Local/Ssm/NsmLocalIngressEgressMacAclPolicyManagedObject.h"
#include "Nsm/Global/NsmGlobalAssocMacAclPolicyPoVlanIntfMessage.h"
#include "Ssm/Global/SsmGlobalGetMacAclPolicyMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "Ssm/Global/Ip6AclPolicyManagedObject.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Local/OneGigLocalManagedObject.h"
#include "ClientInterface/Ssm/qoscommon.h"

//PVLAN
#include "Nsm/Common/PVlanUtils.h"

/* Lacp Header files */
#include "Nsm/Global/NsmGlobalUpdateLacpConfigMessage.h"
#include "Nsm/Global/LacpConfigManagedObject.h"
#include "Nsm/Local/NsmUpdateLacpConfigSPMessage.h"

#include "Nsm/Global/NSMGlobalReservedVlanMessage.h"
#include "Nsm/Local/NSMLocalReserevedVlanMessage.h"
/* ELD */
#include "ClientInterface/Eld/EldClientUpdatePhyIntfMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
/* ELD */

/* FCOE VLAN */
#include "Nsm/Global/NsmGlobalFcoeVlanMessage.h"
#include "Nsm/Local/NsmLocalFcoeVlanMessage.h"
#include "Fcoe/Global/FcoeMapManagedObject.h"
#include "Fcoe/Common/FcoePluginDebug.h"
#include "Nsm/Local/FcoeWorker/FcoeportAttrLocalManagedObject.h"
#include "Nsm/Global/FcoeWorker/FcoeportAttrGlobalManagedObject.h"
#include "Nsm/Global/IntfXstpGlobalConfWorker.h"
/* Qos */
#include "Nsm/Global/NsmGlobalQosPOConfigMessage.h"
#include "Nsm/Local/NsmQosPOConfigSPMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Global/QosGlobalCosProfileAndThresholdConfigMessage.h"
#include "Nsm/Local/QosPoProfileAndThresholdSPMessage.h"

#include <sstream>
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "Qos/Global/CeeMapManagedObject.h"
#include "Qos/Global/DscpToDscpMutationMapManagedObject.h"
#include "Qos/Global/DscpToTrafficClassMapManagedObject.h"
#include "Qos/Global/DscpToCosMapManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/PriorityMapManagedObject.h"
#include "Qos/Common/QosUtils.h"
#include "Nsm/Global/NsmGlobalQosPfcConfigMessage.h"
#include "Qos/Global/CeeMapManagedObject.h"
#include "Nsm/Local/NsmQosPfcConfigSPMessage.h"
#include "Nsm/Local/PfcFlowControlLocalManagedObject.h"
#include "Nsm/Global/NsmGlobalQosMapCleanUpMessage.h"
#include "Nsm/Local/QosCosToProfileIdManagedObject.h"
#include "Nsm/Local/QosCosToTailDropManagedObject.h"
#include "Nsm/Global/Igmps/IgmpsVlanIntfConfManagedObject.h"
#include "Igmps/Global/IgmpsGlobalConfigManagedObject.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "Ssm/Global/MacAclPolicyManagedObject.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Global/VlanClassifierActivateGlobalManagedObject.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "VcsFabric/Local/VcsNodeLocalMessagingContext.h"

#include "Nsm/Global/NsmGlobalReAssocMacAclPolicyMessage.h"
/* ELD */
#include "ClientInterface/Eld/EldClientUpdatePhyIntfMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
/* ELD */
#include "Utils/DceClusterUtils.h"
#include "Utils/DceLinearSeqContext.h"
#include "Nsm/Global/Ssm/SsmDeleteAssociationwithDisconnectedMessage.h"

#include "ReservedVlanManagedObject.h"
#include "Utils/Capabilities.h"
#include "Qos/Common/QosUtils.h"
#include "Qos/Global/QosGlobalTypes.h"

/* PBR */
#include "Nsm/Local/Ssm/PBRVeIntfMessage.h"
#include "Nsm/Local/Ssm/PBRPhyIntfMessage.h"
#include "Nsm/Global/Ssm/PBRPoVlanIntfMessage.h"
#include "Nsm/Local/Ssm/PBRPoVlanIntfSPMessage.h"
#include "Nsm/Local/Ssm/NsmLocalPBRManagedObject.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "IpPolicy/Framework/IpPolicyGetPBRObjectMessage.h"
#include "IpPolicy/Framework/IpPolicyDeleteGlobalPBRMessage.h"

//QoS Policy Map
#include "Policer/Global/PolicyClassMapManagedObject.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"
#include "ssm_policer_public.h"
#include <stdio.h>
#include <stdlib.h>
#include "Nsm/Global/SwitchportVlanConfigGlobalMessage.h"
#include "Nsm/Local/MacBasedVlanClassificationMO.h"
#include "Nsm/Local/CtagBasedVlanClassificationMO.h"

#define INTF_XSTP_GLOBAL_WORKER (NsmGlobalObjectManager::getInstance()->getIntfXstpGlobalConfWorker())
#include "Nsm/Global/MacGroupManagedObject.h"
#include "Nsm/Global/MacGroupConfigGlobalMessage.h"
#include "Nsm/Local/MacGroupConfigLocalMessage.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Nsm/Local/RspanGVlanAccessPortManagedObject.h"
#include "Nsm/Local/RspanGVlanTrunkPortManagedObject.h"
#include "Nsm/Global/VLAN/GVlanEnableGlobalMessage.h"
#include "Nsm/Local/VLAN/GVlanEnableLocalMessage.h"
#include "Utils/ProvisioningUtil.h"


namespace DcmNs
{

    IntfConfGlobalWorker::IntfConfGlobalWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : WaveWorker  (pNsmGlobalObjectManager)
    {
        IntfManagedObject    IntfManagedObject    (pNsmGlobalObjectManager);
        ReservedVlanManagedObject reservedVlanManagedObject (pNsmGlobalObjectManager);
        VlanIntfManagedObject    VlanIntfManagedObject    (pNsmGlobalObjectManager);
        PoPhyIntfManagedObject    PoPhyIntfManagedObject    (pNsmGlobalObjectManager);
        PoIntfManagedObject    PoIntfManagedObject    (pNsmGlobalObjectManager);
        LacpConfigManagedObject    LacpConfigManagedObject    (pNsmGlobalObjectManager);
        
        IntfManagedObject.setupOrm ();
        addManagedClass (IntfManagedObject::getClassName (), this);
        
        reservedVlanManagedObject.setupOrm ();
        addManagedClass (ReservedVlanManagedObject::getClassName (), this);   
            
        VlanIntfManagedObject.setupOrm ();
        addManagedClass (VlanIntfManagedObject::getClassName (), this);
        PoPhyIntfManagedObject.setupOrm ();
        addManagedClass (PoPhyIntfManagedObject::getClassName (), this);
        PoIntfManagedObject.setupOrm ();
        addManagedClass (PoIntfManagedObject::getClassName (), this);
        LacpConfigManagedObject.setupOrm ();
        addManagedClass (LacpConfigManagedObject::getClassName (), this);
        MacGroupManagedObject MacGroupManagedObject(pNsmGlobalObjectManager);
        MacGroupManagedObject.setupOrm ();
        addManagedClass (MacGroupManagedObject::getClassName (), this);
        MacGroupEntryManagedObject MacGroupEntryManagedObject(pNsmGlobalObjectManager);
        MacGroupEntryManagedObject.setupOrm ();
        addManagedClass (MacGroupEntryManagedObject::getClassName (), this);
        
        addOperationMap (NSMGLOBALCREATEVLANINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalCreateVlanIntfMessageHandler));
        addOperationMap (NSMGLOBALUPDATEVLANINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalUpdateVlanIntfMessageHandler));
        addOperationMap (NSMGLOBALDELETEVLANINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalDeleteVlanIntfMessageHandler));
        addOperationMap (NSMGLOBALCREATEPOINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalCreatePoIntfMessageHandler));
        addOperationMap (NSMGLOBALADDPHYINTFTOPOINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalAddPhyIntfToPoIntfMessageHandler));
        addOperationMap (NSMGLOBALUPDATEPOINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalUpdatePoIntfMessageHandler));
        addOperationMap (NSMGLOBALDELETEPOINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalDeletePoIntfMessageHandler));
        addOperationMap (NSMGLOBALADDPOTOVLANINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalAddPoToVlanIntfMessageHandler));
        addOperationMap (NSMGLOBALUPDATELACPCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalUpdateLacpConfigMessageHandler));
        addOperationMap (NSMGLOBALASSOCMACACLPOLICYPOVLANINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalAssocMacAclPolicyPoVlanIntfMessageHandler));
        addOperationMap (NSMGLOBALFCOEVLAN, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalFcoeVlanMessageHandler));
        addOperationMap (NSMGLOBALRESERVEDVLAN, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalReservedVlanMessageHandler));
        addOperationMap (NSMGLOBALQOSPOCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalQosPOConfigMessageHandler));
        addOperationMap (NSMGLOBALCOSPROFILEANDTHRESHOLDCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalCosProfileAndThresholdConfigHandler));
        addOperationMap (NSMGLOBALQOSPFCCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalQosPfcConfigMessageHandler));
        addOperationMap (NSMGLOBALREASSOCMACACLPOLICY, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalReAssocMacAclPolicyMessageHandler));
        addOperationMap (SSMDELETEASSOCIATIONWITHDISCONNECTEDMESSAGE, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::SsmDeleteAssociationwithDisconnectedMessageHandler));  
		addOperationMap (NSMGLOBALQOSMAPCLEANUP, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalQosMapCleanUpMessageHandler));
        addOperationMap (NSMGLOBALREASSOCIPACLPOLICY, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalReAssocIpAclPolicyMessageHandler));
		addOperationMap (NSMGLOBALASSOCIPACLPOLICYPOVLANINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalAssocIpAclPolicyPoVlanIntfMessageHandler));
		addOperationMap (NSMGLOBALPBRPOINTF, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalPBRPoVlanIntfMessageHandler));
		addOperationMap (IPPOLICYDELETEGLOBALPBR, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::NsmGlobalPBRDeleteIntfMessageHandler));
		addOperationMap (SWITCHPORT_VLAN_CONFIG_GLOBAL_MESSAGE, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::switchportVlanConfigGlobalMessageHandler));
		addOperationMap (MAC_GROUP_CONFIG_GLOBAL_MESSAGE, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::macGroupConfigGlobalMessageHandler));
		addOperationMap (GVLAN_ENABLE_GLOBAL_MESSAGE, reinterpret_cast<PrismMessageHandler> (&IntfConfGlobalWorker::gVlanEnableGlobalMessageHandler));
    }

    IntfConfGlobalWorker::~IntfConfGlobalWorker ()
    {
    }

    PrismMessage  *IntfConfGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMGLOBALCREATEVLANINTF :
                pPrismMessage = new NsmGlobalCreateVlanIntfMessage ();
                break;
            case NSMGLOBALUPDATEVLANINTF :
                pPrismMessage = new NsmGlobalUpdateVlanIntfMessage ();
                break;
            case NSMGLOBALDELETEVLANINTF :
                pPrismMessage = new NsmGlobalDeleteVlanIntfMessage ();
                break;
            case NSMGLOBALCREATEPOINTF :
                pPrismMessage = new NsmGlobalCreatePoIntfMessage ();
                break;
            case NSMGLOBALADDPHYINTFTOPOINTF :
                pPrismMessage = new NsmGlobalAddPhyIntfToPoIntfMessage ();
                break;
            case NSMGLOBALUPDATEPOINTF :
                pPrismMessage = new NsmGlobalUpdatePoIntfMessage ();
                break;
            case NSMGLOBALDELETEPOINTF :
                pPrismMessage = new NsmGlobalDeletePoIntfMessage ();
                break;
            case NSMGLOBALADDPOTOVLANINTF :
                pPrismMessage = new NsmGlobalAddPoToVlanIntfMessage ();
                break;
            case NSMGLOBALDELETEPOTOVLANINTF :
                pPrismMessage = new NsmGlobalDeletePoToVlanIntfMessage ();
                break;
            case NSMGLOBALUPDATELACPCONFIG :
                pPrismMessage = new NsmGlobalUpdateLacpConfigMessage ();
				break;
			case NSMGLOBALASSOCMACACLPOLICYPOVLANINTF:
				pPrismMessage = new NsmGlobalAssocMacAclPolicyPoVlanIntfMessage ();
				break;
            case NSMGLOBALFCOEVLAN:
                pPrismMessage = new NsmGlobalFcoeVlanMessage();
                break;
           case NSMGLOBALRESERVEDVLAN:
                pPrismMessage = new NsmGlobalReservedVlanMessage();
                break;
			case NSMGLOBALQOSPOCONFIG:
				pPrismMessage = new NsmGlobalQosPOConfigMessage();
				break;
            case NSMGLOBALQOSPFCCONFIG:
                pPrismMessage = new NsmGlobalQosPfcConfigMessage();
                break;
            case NSMGLOBALCOSPROFILEANDTHRESHOLDCONFIG:
				pPrismMessage = new QosGlobalCosProfileAndThresholdConfigMessage();
				break;
            case NSMGLOBALREASSOCMACACLPOLICY:
            	pPrismMessage = new NsmGlobalReAssocMacAclPolicyMessage();
            	break;
			case SSMDELETEASSOCIATIONWITHDISCONNECTEDMESSAGE:
				pPrismMessage = new SsmDeleteAssociationwithDisconnectedMessage();
				break;
			case NSMGLOBALQOSMAPCLEANUP:
				pPrismMessage = new NsmGlobalQosMapCleanUpMessage();
				break;
			case NSMGLOBALREASSOCIPACLPOLICY:
            	pPrismMessage = new NsmGlobalReAssocIpAclPolicyMessage();
            	break;
			case NSMGLOBALASSOCIPACLPOLICYPOVLANINTF:
				pPrismMessage = new NsmGlobalAssocIpAclPolicyPoVlanIntfMessage ();
				break;
			case NSMGLOBALPBRPOINTF:
				pPrismMessage = new PBRPoVlanIntfMessage ();
				break;
			case IPPOLICYDELETEGLOBALPBR:
				pPrismMessage= new IpPolicyDeleteGlobalPBRMessage();
				break;
			case SWITCHPORT_VLAN_CONFIG_GLOBAL_MESSAGE:
				pPrismMessage = new SwitchportVlanConfigGlobalMessage();
				break;
			case MAC_GROUP_CONFIG_GLOBAL_MESSAGE:
				pPrismMessage = new MacGroupConfigGlobalMessage();
				break;
			case GVLAN_ENABLE_GLOBAL_MESSAGE:
				pPrismMessage = new GVlanEnableGlobalMessage();
				break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *IntfConfGlobalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((IntfManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IntfManagedObject(getPWaveObjectManager());
        }
        else if ((ReservedVlanManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ReservedVlanManagedObject (dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((VlanIntfManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new VlanIntfManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((PoPhyIntfManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PoPhyIntfManagedObject(getPWaveObjectManager());
        }
        else if ((PoIntfManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PoIntfManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
		else if ((LacpConfigManagedObject::getClassName ()) == managedClassName)
        {
            //trace (TRACE_LEVEL_INFO, "IntfConfGlobalWorker::createManagedObjectInstance : Managed Class Name : " + managedClassName);
            pWaveManagedObject = new LacpConfigManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((MacGroupManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new MacGroupManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((MacGroupEntryManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new MacGroupEntryManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "IntfConfGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void
    IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback (WaveSendToClusterContext  *ctx_p)
    {
        trace (TRACE_LEVEL_DEBUG, "Inside IntfConfGlobalSendToClusterCallback");
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        PrismLinearSequencerContext             *seq_ctx_p;
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
        delete ctx_p->getPPrismMessageForPhase1();
        delete ctx_p->getPPrismMessageForPhase2();
        delete ctx_p;
        trace (TRACE_LEVEL_DEBUG, string("IntfConfGlobalSendToClusterCallback:") + sendToClusterCompletionStatus); 
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        return;
    }

    void
    IntfConfGlobalWorker::IntfConfGlobalSendToVlanCreateClusterCallback(WaveSendToClusterContext  *ctx_p)
    {
        NsmCreateVlanIntfSPMessage      *sw_resp_p  = NULL;
        NsmGlobalCreateVlanIntfMessage  *pNsmGlobalCreateVlanIntfMessage = NULL;
        PrismLinearSequencerContext     *pPrismLinearSequencerContext = NULL;
        map<LocationId, UI32> locationIdsToRbridgeIds;
        map<UI32, map<SI32, vector<UI32> > > vlanFailureStatusRbridgeMap;
        map<UI32, UI32> failedVlanNodeCountMap;
        vector<UI32> allRbridgeList;

        ResourceId status = DcmToolKit::getMapOfLocationIdsToRbridgeIds (locationIdsToRbridgeIds);
        if (DCMTOOLKIT_SUCCESS != status) {
            trace (TRACE_LEVEL_ERROR, "IntfConfGlobalWorker::IntfConfGlobalSendToVlanCreateClusterCallback: DcmToolKit::getMapOfLocationIdsToRbridgeIds API failed");
            return;
        }

        pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
        pNsmGlobalCreateVlanIntfMessage = dynamic_cast<NsmGlobalCreateVlanIntfMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);

        trace (TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::IntfConfGlobalSendToVlanCreateClusterCallback: sendToClusterCompletionStatus:- ") + FrameworkToolKit::localize(sendToClusterCompletionStatus));

        if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus ) {
            string RangeVlanStr = pNsmGlobalCreateVlanIntfMessage->getRangeVlanStr();
            UI32Range givenVlanRange = UI32Range (RangeVlanStr);
            vector<UI32> givenVlanRangeVector;
            givenVlanRange.getUI32RangeVector(givenVlanRangeVector);

            vector<LocationId> locations = ctx_p->getLocationsToSendToForPhase1();
            for (UI32 i = 0; i < locations.size(); i++) {
                UI32 rbridgeId = locationIdsToRbridgeIds[locations[i]];
                allRbridgeList.push_back(rbridgeId);
                ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locations[i]);
                trace (TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::IntfConfGlobalSendToVlanCreateClusterCallback: Processing for Rbridge:") + rbridgeId + string(" status:") + statusPerLocationId);

                if (WAVE_MESSAGE_SUCCESS == statusPerLocationId) {
                    sw_resp_p = dynamic_cast<NsmCreateVlanIntfSPMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
                    vector<UI32> failedVlanIds = sw_resp_p->getFailedVlanIds();
                    vector<SI32> failureStatus = sw_resp_p->getFailureStatus();

                    for (UI32 idx = 0; idx < failedVlanIds.size(); idx++) {
                        SI32 failureCode = failureStatus[idx];
                        UI32 failedVlanId = failedVlanIds[idx];

                        ((vlanFailureStatusRbridgeMap[failedVlanId])[failureCode]).push_back(rbridgeId);

                        if (failedVlanNodeCountMap.end() == failedVlanNodeCountMap.find(failedVlanId)) {
                            failedVlanNodeCountMap[failedVlanId] = 1;
                        } else {
                            UI32 count = failedVlanNodeCountMap[failedVlanId];
                            failedVlanNodeCountMap[failedVlanId] = ++count;
                        }
                    }

                } else {
                    trace (TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::IntfConfGlobalSendToVlanCreateClusterCallback: Rbridge:- ") + rbridgeId + string(" Error:") + FrameworkToolKit::localize(statusPerLocationId));
                }
            }

            /* Remove Vlans which were failed on all nodes so that no MO is created for such Vlans */
            vector<UI32> failedVlanIdsOnAllNodes;
            for (map<UI32, UI32>::iterator iter = failedVlanNodeCountMap.begin(); iter != failedVlanNodeCountMap.end(); ++iter) {
                if ((*iter).second == locations.size()) {
                    failedVlanIdsOnAllNodes.push_back((*iter).first);
                    trace (TRACE_LEVEL_INFO, string("callback: Failed VlansId - ") + (*iter).first);
                }
            }

            if (failedVlanIdsOnAllNodes.size() == givenVlanRangeVector.size()) {
                sendToClusterCompletionStatus = WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES;
            } else {
                sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;

                UI32Range failedVlanRange = UI32Range(UI32Range::getUI32RangeStringFromVector(failedVlanIdsOnAllNodes));
                UI32Range successfulVlansRange = givenVlanRange - failedVlanRange; 
                pNsmGlobalCreateVlanIntfMessage->setRangeVlanStr(successfulVlansRange.toString());
            }

            /* Put the failed Vlans information back to message for reporting error messages on Nos Cli. */
            for (map<UI32, map<SI32, vector<UI32> > >::iterator iter = vlanFailureStatusRbridgeMap.begin(); iter != vlanFailureStatusRbridgeMap.end(); ++iter) {
                UI32 failedVlanId = (*iter).first;
                map<SI32, vector<UI32> > failStatusRbridgeMap = (*iter).second;

                vector<string> failStatusRbridgeRangeList;
                /* get range of all rbridge-id where vlans were created */
                UI32Range successfulRBridgesRange = UI32Range(UI32Range::getUI32RangeStringFromVector(allRbridgeList));

                for (map<SI32, vector<UI32> >::iterator innerIter = failStatusRbridgeMap.begin(); innerIter != failStatusRbridgeMap.end(); ++innerIter) {
                    SI32 failedStatus = (*innerIter).first;
                    vector<UI32> rbrList = (*innerIter).second;
                    string rbridgeIdRangeStr = "";
                    if (rbrList.size() > 0) {
                        sort(rbrList.begin(), rbrList.end());
                        rbridgeIdRangeStr = UI32Range::getUI32RangeStringFromVector(rbrList);
                        /* Deduct failed rbridges for remaining successful rbridges */
                        successfulRBridgesRange = successfulRBridgesRange - UI32Range(rbridgeIdRangeStr);

                        /* store failed status for each vlan on different rbridge in form of <errorCode>#<rbridgeId range> */
                        string str = string("") + failedStatus + string("#") + rbridgeIdRangeStr;
                        failStatusRbridgeRangeList.push_back(str);
                    }
                }

                pNsmGlobalCreateVlanIntfMessage->updateFailureStatus(failedVlanId, failStatusRbridgeRangeList, successfulRBridgesRange.toString());
            }
        } else {
            trace (TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::IntfConfGlobalSendToVlanCreateClusterCallback: Error - ") + FrameworkToolKit::localize(sendToClusterCompletionStatus));
        }

        delete ctx_p->getPPrismMessageForPhase1();
        delete ctx_p->getPPrismMessageForPhase2();
        delete ctx_p;
        trace (TRACE_LEVEL_DEBUG, string("IntfConfGlobalWorker::IntfConfGlobalSendToVlanCreateClusterCallback: ") + sendToClusterCompletionStatus); 
        pPrismLinearSequencerContext->executeNextStep(sendToClusterCompletionStatus);
        return;
    }

    void
    IntfConfGlobalWorker::IntfConfGlobalAddToPoCallback (WaveSendToClusterContext  *ctx_p)
    {
        trace (TRACE_LEVEL_DEBUG, "Inside IntfConfGlobalAddToPoCallback");
		LocationId locationId;
        PrismLinearSequencerContext             *seq_ctx_p;
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
		NsmAddPhyIntfToPoIntfMessage *pNsmAddPhyIntfToPoIntfMessage = (NsmAddPhyIntfToPoIntfMessage *)ctx_p->getPPrismMessageForPhase1();
		if((WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()) && (pNsmAddPhyIntfToPoIntfMessage->getReplayPoConfigs() == true))
		{
			UI32 mappedId;
			string threeTupleIfName;
			mappedId = (UI32) NsmUtils::getMappedIdFromIfName(pNsmAddPhyIntfToPoIntfMessage->getIfName());
			locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
			printf("In IntfConfGlobalAddToPoCallback, intName = %s \n postr=%s\n",pNsmAddPhyIntfToPoIntfMessage->getIfName().c_str(),
					pNsmAddPhyIntfToPoIntfMessage->getPoName().c_str());
			//sendPoConfigToLocal(seq_ctx_p,pNsmAddPhyIntfToPoIntfMessage->getPoName(),locationId);
		}
        NsmUtils::ClusterGetFirstFailureLocation(ctx_p, locationId);
        ResourceId sendToClusterCompletionStatus = ctx_p->getCompletionStatusForPhase1(locationId);
        delete ctx_p  -> getPPrismMessageForPhase1();
        delete ctx_p->getPPrismMessageForPhase2();
        delete ctx_p;
        trace (TRACE_LEVEL_DEBUG, string("IntfConfGlobalAddToPoCallback:") + sendToClusterCompletionStatus); 
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        return;
    }

    void
    IntfConfGlobalWorker::IntfConfGlobalReplayConfigCallback (WaveSendToClusterContext  *ctx_p)
    {
        //PrismLinearSequencerContext             *seq_ctx_p;
        //seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
        delete ctx_p  -> getPPrismMessageForPhase1();
        return;
    }

    void IntfConfGlobalWorker::NsmGlobalPBRDeleteIntfMessageHandler (IpPolicyDeleteGlobalPBRMessage *pPBRGlobalDelete)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::deletePBRGlobal),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };
        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPBRGlobalDelete,
                        this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

    void IntfConfGlobalWorker::deletePBRGlobal(PrismLinearSequencerContext *pLinearSeqContext)
    {
        int status                          = WAVE_MESSAGE_ERROR;
        set<string> compositionName;
        compositionName.insert("interfaceRouteMap");
        IpPolicyDeleteGlobalPBRMessage *pPBRGlobalDelete =
                        reinterpret_cast<IpPolicyDeleteGlobalPBRMessage *>(pLinearSeqContext->getPPrismMessage());
        string ifName           = pPBRGlobalDelete->getIfName();
        //UI32 *poId                = new UI32();
        UI32 poId                   = atoi(ifName.c_str());
        WaveManagedObjectSynchronousQueryContextForDeletion *syncQueryCt =
                                        new WaveManagedObjectSynchronousQueryContextForDeletion(PoIntfManagedObject::getClassName());
        syncQueryCt->addAndAttribute (new AttributeUI32(poId,"id"));
        trace(TRACE_LEVEL_INFO, string("poId = ") + poId);
        deleteManagedObjectCompositions (syncQueryCt, compositionName);
        status = WAVE_MESSAGE_SUCCESS;
/*
        WaveManagedObjectSynchronousQueryContext syncQueryCtxtCHild(PoIntfManagedObject::getClassName());
        syncQueryCtxtCHild.addAndAttribute (new AttributeUI32(poId,"id"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxtCHild);
        PoIntfManagedObject *pPoIntfManagedObject = NULL;
        trace(TRACE_LEVEL_INFO, string("poId = ") + poId);

        int status                          = WAVE_MESSAGE_ERROR;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                status = WAVE_MESSAGE_ERROR;
            }else if (1 == numberOfResults) {
                trace(TRACE_LEVEL_INFO, string("Found Existing PO"));
                pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
                //pPoIntfManagedObject->setPBR(NULL);
                updateWaveManagedObject(pPoIntfManagedObject);
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
        pLinearSeqContext->executeNextStep(status);
        return;
    }

	/* PBR Po handler */
    void IntfConfGlobalWorker::NsmGlobalPBRPoVlanIntfMessageHandler (PBRPoVlanIntfMessage *pPBRPoVlanIntfMessage)
    {
            PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::validatePBRPoStep),
                reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::configurePBRPoIntfStep),
                reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
                reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updatePBRPoIntfStep),
                reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
                reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
            };
            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPBRPoVlanIntfMessage,
                            this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
            pPrismLinearSequencerContext->holdAll();
            pPrismLinearSequencerContext->start ();
    }

    void IntfConfGlobalWorker::validatePBRPoStep(PrismLinearSequencerContext *pLinearSeqContext)
    {
        PBRPoVlanIntfMessage *pMsg =
                reinterpret_cast<PBRPoVlanIntfMessage *>(pLinearSeqContext->getPPrismMessage());
        ResourceId status   = WAVE_MESSAGE_SUCCESS;
        UI32 ifId           = pMsg->getIfId();

        /* Fix Me : Check value of direction in commit transaction */        

        tracePrintf(TRACE_LEVEL_INFO, "In %s, type = %d, cmdCode = %d ",
                __FUNCTION__, pMsg->getIfType (), pMsg->getCmdCode());

        if (pMsg->getCmdCode() == SSM_CLIENT_SET_ROUTEMAP_APPLY_INTERFACE) {

            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifId,"id"));

            vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

            if (pResults && pResults->size() > 0) {
                PoIntfManagedObject *pPoIntfManagedObject  = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
                if (pPoIntfManagedObject) {
                    trace (TRACE_LEVEL_INFO,string("In function IntfConfGloba/* Adding check whether that port is profiled or not */"));
                    }
                }
            if (pResults)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
        	pLinearSeqContext->executeNextStep(status);
        return;
    }

    void IntfConfGlobalWorker::configurePBRPoIntfStep(PrismLinearSequencerContext *pLinearSeqContext)
    {
        PBRPoVlanIntfMessage *pMsg    			    = reinterpret_cast<PBRPoVlanIntfMessage *>(pLinearSeqContext->getPPrismMessage());
        UI32   ifId                                         = pMsg->getIfId ();
        string routeMapName                                 = pMsg->getRouteMapName ();
        UI8    ifType                                       = pMsg->getIfType ();
        UI32   cmdCode                                      = pMsg->getCmdCode ();

        trace(TRACE_LEVEL_DEBUG, "configurePBRPoIntfStep: Sending config message to nodes...");

        PBRPoVlanIntfSPMessage *localMsg = new PBRPoVlanIntfSPMessage (ifId, ifType, routeMapName, cmdCode);

        if (localMsg) {
            WaveSendToClusterContext    *send_ctx_p;
            send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback),
                pLinearSeqContext);
            prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
            send_ctx_p->setPPrismMessageForPhase1(localMsg);
            /* In case of PO Interface changing location ids */

            int status  = SET_LOCATION_ID_API_ERROR;
            status      = NsmUtils::setLocationIdsForPoSubModeCmds(pMsg->getIfId(),send_ctx_p);

            if(status != SET_LOCATION_ID_API_SUCCESS)
            {
                trace(TRACE_LEVEL_ERROR, "In configurePBRPoIntfStep, call to setLocationIdsForPoSubModeCmds failed ");
                pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
                pLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
                if (localMsg) {
                    delete localMsg;
                }
                if (send_ctx_p) {
                    delete send_ctx_p;
                }
                return;
            }

            sendToWaveCluster(send_ctx_p);
            pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            return;
        }
    }

    void IntfConfGlobalWorker::updatePBRPoIntfStep(PrismLinearSequencerContext *pLinearSeqContext)
    {
        trace (TRACE_LEVEL_DEVEL, "****Inside updatePBRPoIntfStep");
        PBRPoVlanIntfMessage *pMsg = reinterpret_cast<PBRPoVlanIntfMessage *>(pLinearSeqContext->getPPrismMessage());
        ResourceId status = WAVE_MESSAGE_ERROR;

        if (pMsg->getIfType () == IF_TYPE_PO) {
            if (pMsg->getCmdCode () == SSM_CLIENT_SET_ROUTEMAP_APPLY_INTERFACE) {
                status = updatePoIntfPBR(pLinearSeqContext);
            } else {
                status = removePBRPoIntf(pLinearSeqContext);
            }
        }

        pLinearSeqContext->executeNextStep (status);
    }

	ResourceId IntfConfGlobalWorker::updatePoIntfPBR(PrismLinearSequencerContext *pLinearSeqContext)
    {

        PBRPoVlanIntfMessage *pMsg        = reinterpret_cast<PBRPoVlanIntfMessage *>(pLinearSeqContext->getPPrismMessage());
        ResourceId          status 		  = WAVE_MESSAGE_SUCCESS;
        PoIntfManagedObject *pPoIntfManagedObject               = NULL;

        UI32 poId                                               = pMsg->getIfId();
        string routeMapName                                          = pMsg->getRouteMapName();
        //ObjectId PBRObjectId;


        trace (TRACE_LEVEL_DEVEL, "****Inside updatePoIntfPBR");
        //IpPolicyGetPBRObjectMessage *policyMsg = new IpPolicyGetPBRObjectMessage(routeMapName);

        //if (policyMsg) {
        //    trace (TRACE_LEVEL_DEVEL, "updatePoIntfPBR:3 - Obtained the Policy MO");
        //    status = sendSynchronously (policyMsg);
        //    if (status == WAVE_MESSAGE_ERROR) {
        //        trace (TRACE_LEVEL_ERROR, "updatePoIntfPBR: SSM Message returned with  error");
        //        delete policyMsg;
        //        return (status);
        //    } else {
        //        trace (TRACE_LEVEL_INFO, "updatePoIntfPBR: SSM Message returned success");
        //    }
        //    PBRObjectId = policyMsg->getPBRRouteMapObjId();

            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
            vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

            if (NULL != pResults) {
                UI32 numberOfResults = pResults->size ();
                if (0 == numberOfResults) {
                    status = WAVE_MESSAGE_ERROR;
                }else if (1 == numberOfResults) {
                    trace (TRACE_LEVEL_DEVEL, "updatePoIntfPBR:4- Obtained the Port Channel object ");
                    pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);

                    NsmLocalPBRManagedObject *NsmLocalPBRMO = new
                        NsmLocalPBRManagedObject(dynamic_cast
                        <NsmGlobalObjectManager*>(getPWaveObjectManager()), routeMapName);
                    pPoIntfManagedObject->setPBR (NsmLocalPBRMO);

                    updateWaveManagedObject(pPoIntfManagedObject);
                    status = WAVE_MESSAGE_SUCCESS;
                } else {
                    status = WAVE_MESSAGE_ERROR;
                }
                pLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
                delete pResults;
            }
            //delete policyMsg;

        //} else {
        //    status = WAVE_MESSAGE_ERROR;
        //}

        return (status);
    }
    ResourceId IntfConfGlobalWorker::removePBRPoIntf(PrismLinearSequencerContext *pLinearSeqContext)
    {
        PBRPoVlanIntfMessage *pMsg    			    = reinterpret_cast<PBRPoVlanIntfMessage *>(pLinearSeqContext->getPPrismMessage());
        ResourceId status                                   = WAVE_MESSAGE_ERROR;

        if (pMsg->getIfType () == IF_TYPE_PO) {
            UI32 poId                                       = pMsg->getIfId();
            PoIntfManagedObject *pPoIntfManagedObject = NULL;
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
            vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

            if (NULL != pResults) {
                UI32 numberOfResults = pResults->size ();
                if (0 == numberOfResults) {
                    status = WAVE_MESSAGE_ERROR;
                }else if (1 == numberOfResults) {
                    trace (TRACE_LEVEL_DEVEL, "removePBRPoIntf - Obtained the Port Channel object ");
                    pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
                    if (pPoIntfManagedObject) {
                        //pPoIntfManagedObject->setPBR (NULL);
					  	pPoIntfManagedObject->setPBR(NULL);
                        updateWaveManagedObject(pPoIntfManagedObject);
                        status = WAVE_MESSAGE_SUCCESS;
                        trace (TRACE_LEVEL_DEVEL, "removePBRPoIntf: Removed routeMap from Po ");
                    }
                } else {
                    status = WAVE_MESSAGE_ERROR;
                }
                /* Mark objects for GC */
                pLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
                delete pResults;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
        }

        return (status);
    }

	/* IP Acl Vlan and PO code Begins */
    void IntfConfGlobalWorker::NsmGlobalAssocIpAclPolicyPoVlanIntfMessageHandler(NsmGlobalAssocIpAclPolicyPoVlanIntfMessage *pNsmGlobalAssocIpAclPolicyPoVlanIntfMessage)
	{
			PrismLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::validateIpAclToPoStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::configureIpAclToPoIntfStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updatePoIntfIpAclStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
			};
			PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNsmGlobalAssocIpAclPolicyPoVlanIntfMessage,
							this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
			pPrismLinearSequencerContext->holdAll();
			pPrismLinearSequencerContext->start ();
	}
	
	void IntfConfGlobalWorker::validateIpAclToPoStep(PrismLinearSequencerContext *pLinearSeqContext)
	{
		NsmGlobalAssocIpAclPolicyPoVlanIntfMessage *pMsg =
				reinterpret_cast<NsmGlobalAssocIpAclPolicyPoVlanIntfMessage *>(pLinearSeqContext->getPPrismMessage());
		ResourceId status 	= WAVE_MESSAGE_SUCCESS;
		UI32 found 			= 0;
		UI32 ifId 			= pMsg->getIfId();
		string policyName	= pMsg->getAclName();
		UI32 count          = 0;

		/* Fix Me : Check value of direction in commit transaction */
		ssmAcl_binding_direction direction = pMsg->getDirection();
		
		tracePrintf(TRACE_LEVEL_INFO, "In %s, type = %d, cmdCode = %d ",
				__FUNCTION__, pMsg->getIfType (), pMsg->getCmdCode());
 		
		if (pMsg->getCmdCode() == SSMACL_DCMD_BIND_L3_IPV4_ACL)
		{
			/* (a) Check for Mac Acl
			 * (b) First Check whether ACL Policy with same name exists for IP ACL
			 * (c) Check whether ACL Policy with same name exists for IPv6 ACL*/

			WaveManagedObjectSynchronousQueryContext syncQueryCtxtMacAcl(MacAclPolicyManagedObject::getClassName());
			syncQueryCtxtMacAcl.addAndAttribute (new AttributeString (policyName, "policyName"));
			syncQueryCtxtMacAcl.setLoadOneToManyRelationships(false);
			querySynchronouslyForCount(&syncQueryCtxtMacAcl, count);

			if (count == 0)
			{
				WaveManagedObjectSynchronousQueryContext syncQueryCtxtIpAcl(IpAclPolicyManagedObject::getClassName());
				syncQueryCtxtIpAcl.addAndAttribute (new AttributeString (policyName, "policyName"));
				syncQueryCtxtIpAcl.setLoadOneToManyRelationships(false);
				querySynchronouslyForCount(&syncQueryCtxtIpAcl, count);

				if(count == 0)
				{
					WaveManagedObjectSynchronousQueryContext syncQueryCtxtIpv6Acl(Ip6AclPolicyManagedObject::getClassName());
					syncQueryCtxtIpv6Acl.addAndAttribute (new AttributeString (policyName, "policyName"));
					syncQueryCtxtIpv6Acl.setLoadOneToManyRelationships(false);
					querySynchronouslyForCount(&syncQueryCtxtIpv6Acl, count);
				}
			}

			if ( 0 == count )
			{
				status = WRC_SSM_DCM_ERR_ACL_EEXIST;
				pLinearSeqContext->executeNextStep(status);
				return;
			}

        	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        	syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifId,"id"));
			
        	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

			if (pResults && pResults->size() > 0) {
				PoIntfManagedObject *pPoIntfManagedObject  = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
				if (pPoIntfManagedObject) {
					trace (TRACE_LEVEL_INFO,string("In function IntfConfGlobalWorker::validateIpAclToPoStep obtained pPoIntfManagedObject"));
					/* Adding check whether that port is profiled or not */

                    bool isPortProfiled = false;
                    APPMUtils::isPortProfilePort(pPoIntfManagedObject, isPortProfiled);

					if (isPortProfiled == true) {
						status = SSM_DCM_ERR_ACL_APPM_PROFILED_PORT;
					} else {
						found = pPoIntfManagedObject->getIngressEgressIpAclPolicyDirectioncount (direction);
						if (found) {
							status = WRC_SSM_DCM_ERR_ACL_RULE_DUP;
						}
					}
				}
			}
			if (pResults)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
		pLinearSeqContext->executeNextStep(status);
		return;
	}

	void IntfConfGlobalWorker::configureIpAclToPoIntfStep(PrismLinearSequencerContext *pLinearSeqContext)
	{
		NsmGlobalAssocIpAclPolicyPoVlanIntfMessage *pMsg	= reinterpret_cast<NsmGlobalAssocIpAclPolicyPoVlanIntfMessage *>(pLinearSeqContext->getPPrismMessage());
		UI32   ifId											= pMsg->getIfId ();
		string aclName										= pMsg->getAclName ();
		UI8    ifType										= pMsg->getIfType ();
		UI32   cmdCode										= pMsg->getCmdCode ();
		ssmAcl_binding_direction direction					= pMsg->getDirection();
        
		trace(TRACE_LEVEL_DEBUG, "configureIpAclToPoIntfStep: Sending config message to nodes...");
		
		NsmAssocIpAclPolicyPoVlanIntfSPMessage *localMsg = new NsmAssocIpAclPolicyPoVlanIntfSPMessage (ifId, ifType, aclName, cmdCode, direction);

		if (localMsg) {
        	WaveSendToClusterContext    *send_ctx_p;
			send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback),
				pLinearSeqContext);
			prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
			send_ctx_p->setPPrismMessageForPhase1(localMsg);
			/* In case of PO Interface changing location ids */

            int status 	= SET_LOCATION_ID_API_ERROR;
            status 		= NsmUtils::setLocationIdsForPoSubModeCmds(pMsg->getIfId(),send_ctx_p);

            if(status != SET_LOCATION_ID_API_SUCCESS)
            {
                trace(TRACE_LEVEL_ERROR, "In configureIpAclToPoIntfStep, call to setLocationIdsForPoSubModeCmds failed ");
                pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
                pLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
                if (localMsg) {
                    delete localMsg;
                }
                if (send_ctx_p) {
                    delete send_ctx_p;
                }
                return;
            }
			
            send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);
            send_ctx_p->setPartialSuccessFlag(true);
			sendToWaveCluster(send_ctx_p);
        	pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			return;
		}
	}
	
	void IntfConfGlobalWorker::configureIpAclToPoIntfClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		ResourceId sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatus ();
		PrismLinearSequencerContext *seqContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());
		trace(TRACE_LEVEL_DEBUG, "Entering configureIpAclToPoIntfClusterCallback.....");
		prismAssert (NULL != seqContext, __FILE__, __LINE__);
		delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
		delete pWaveSendToClusterContext;
		trace(TRACE_LEVEL_DEBUG, string("configureIpAclToPoIntfClusterCallback - status: ") + sendToClusterCompletionStatus);
		seqContext->executeNextStep(sendToClusterCompletionStatus);
	}
	
	void IntfConfGlobalWorker::updatePoIntfIpAclStep(PrismLinearSequencerContext *pLinearSeqContext)
	{
        trace (TRACE_LEVEL_DEVEL, "****Inside updatePoIntfIpAclStep");
		NsmGlobalAssocIpAclPolicyPoVlanIntfMessage *pMsg = reinterpret_cast<NsmGlobalAssocIpAclPolicyPoVlanIntfMessage *>(pLinearSeqContext->getPPrismMessage());
		ResourceId status = WAVE_MESSAGE_ERROR;

		if (pMsg->getIfType () == PO_INTF_TYPE) {
			if (pMsg->getCmdCode () == SSMACL_DCMD_BIND_L3_IPV4_ACL) {
				status = updatePoIntfIpAcl(pLinearSeqContext);
			} else {
				status = removeIpAclPoIntf(pLinearSeqContext);
			}
		}

		pLinearSeqContext->executeNextStep (status);
	}

	ResourceId IntfConfGlobalWorker::removeIpAclPoIntf(PrismLinearSequencerContext *pLinearSeqContext)
	{
		NsmGlobalAssocIpAclPolicyPoVlanIntfMessage *pMsg 	= reinterpret_cast<NsmGlobalAssocIpAclPolicyPoVlanIntfMessage *>(pLinearSeqContext->getPPrismMessage());
		ResourceId status 									= WAVE_MESSAGE_ERROR;
		ssmAcl_binding_direction direction               	= pMsg->getDirection();
		
		if (pMsg->getIfType () == PO_INTF_TYPE) {
			UI32 poId										= pMsg->getIfId();
			PoIntfManagedObject *pPoIntfManagedObject = NULL;
        	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
			syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
        	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        	if (NULL != pResults) {
            	UI32 numberOfResults = pResults->size ();
            	if (0 == numberOfResults) {
                	status = WAVE_MESSAGE_ERROR;
            	}else if (1 == numberOfResults) {
           			trace (TRACE_LEVEL_DEVEL, "removeIpAclPoIntf- Obtained the Port Channel object ");
                	pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
					if (pPoIntfManagedObject) {
						pPoIntfManagedObject->delIngressEgressIpAclManagedObjectFromVector(direction);		                	
						updateWaveManagedObject(pPoIntfManagedObject);
						status = WAVE_MESSAGE_SUCCESS;
						trace (TRACE_LEVEL_DEVEL, "removeIpAclPoIntf: Removed Ip acl from Po ");
					}
            	} else {
                	status = WAVE_MESSAGE_ERROR;
            	}
                /* Mark objects for GC */
                pLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
                delete pResults;
        	} else {
				status = WAVE_MESSAGE_ERROR;
			}
		}

		return (status);
	}

	ResourceId IntfConfGlobalWorker::updatePoIntfIpAcl(PrismLinearSequencerContext *pLinearSeqContext)
	{

		NsmGlobalAssocIpAclPolicyPoVlanIntfMessage *pMsg 		= reinterpret_cast<NsmGlobalAssocIpAclPolicyPoVlanIntfMessage *>(pLinearSeqContext->getPPrismMessage());
		ResourceId 			status;
        PoIntfManagedObject *pPoIntfManagedObject 				= NULL;

		UI32 poId 												= pMsg->getIfId();
		string aclName 											= pMsg->getAclName();
		ssmAcl_binding_direction direction  					= pMsg->getDirection();
		ObjectId policyObjectId;
		
		
        trace (TRACE_LEVEL_DEVEL, "****Inside updatePoIntfIpAclStep");
		SsmGlobalGetIpAclPolicyMessage *policyMsg = new SsmGlobalGetIpAclPolicyMessage(aclName);

		if (policyMsg) {
           	trace (TRACE_LEVEL_DEVEL, "updatePoIntfIpAclStep:3 - Obtained the Policy MO");
			status = sendSynchronously (policyMsg);
			if (status == WAVE_MESSAGE_ERROR) {
				trace (TRACE_LEVEL_ERROR, "updatePoIntfIpAclStep: SSM Message returned with  error");
                delete policyMsg;
				return (status);
			} else {
				trace (TRACE_LEVEL_INFO, "updatePoIntfIpAclStep: SSM Message returned success");
			}
			policyObjectId = policyMsg->getPolicyObjId();

        	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        	syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
        	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        	if (NULL != pResults) {
            	UI32 numberOfResults = pResults->size ();
            	if (0 == numberOfResults) {
                	status = WAVE_MESSAGE_ERROR;
            	}else if (1 == numberOfResults) {
           			trace (TRACE_LEVEL_DEVEL, "updatePoIntfIpAclStep:4- Obtained the Port Channel object ");
                	pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
 
					NsmLocalIngressEgressIpAclPolicyManagedObject *NsmIngressEgressIpAclMO = new 
						NsmLocalIngressEgressIpAclPolicyManagedObject(dynamic_cast
						<NsmGlobalObjectManager*>(getPWaveObjectManager()), policyObjectId, direction); 	
					pPoIntfManagedObject->addIngressEgressIpAclManagedObjectToVector
						(NsmIngressEgressIpAclMO);	
						
                	updateWaveManagedObject(pPoIntfManagedObject);
                	status = WAVE_MESSAGE_SUCCESS;
            	} else {
                	status = WAVE_MESSAGE_ERROR;
            	}
                /* Mark objects for GC */
                pLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
                delete pResults;
        	}
			delete policyMsg;

		} else {
			status = WAVE_MESSAGE_ERROR;
		}

		return (status);
	}

	void IntfConfGlobalWorker::NsmGlobalReAssocIpAclPolicyMessageHandler(NsmGlobalReAssocIpAclPolicyMessage *pNsmGlobalReAssocIpAclPolicyMessage)
    {
        ObjectId oldPolicy			= pNsmGlobalReAssocIpAclPolicyMessage->getOldPolicyId();
        ObjectId newPolicy 			= pNsmGlobalReAssocIpAclPolicyMessage->getNewPolicyId();
        ResourceId status 			= WAVE_MESSAGE_SUCCESS;
        IntfManagedObject *intfMO 	= NULL;
		UI32 size					= 0;
		UI32 j						= 0;
		
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IntfManagedObject::getClassName());
        vector<WaveManagedObject *> *pResultsMO = querySynchronously (&syncQueryCtxt);

        if (NULL != pResultsMO) {
            startTransaction ();
            UI32 numberOfResults = pResultsMO->size ();
            for (UI32 i = 0; i < numberOfResults; i++) {
                intfMO = dynamic_cast<IntfManagedObject *>((*pResultsMO)[i]);
                if (intfMO != NULL) {
					vector<WaveManagedObjectPointer<NsmLocalIngressEgressIpAclPolicyManagedObject> > 
						NsmIngressEgressIpAclPolicyMoVector = intfMO->getIngressEgressIpAclPolicy();
					
					size = NsmIngressEgressIpAclPolicyMoVector.size();
					for (j = 0; j < size; j++) {
						if (((NsmIngressEgressIpAclPolicyMoVector[j])->getIngressEgressIpAcl()) == oldPolicy) {
							((NsmIngressEgressIpAclPolicyMoVector[j])->setIngressEgressIpAcl(newPolicy));
							trace(TRACE_LEVEL_INFO, "Updating NsmIngressEgressIpAclPolicyMoVector with NewPolicy");
							NsmLocalIngressEgressIpAclPolicyManagedObject  *pNsmIngressEgressIpAclPolicyMO = NsmIngressEgressIpAclPolicyMoVector[j].operator->();
						    updateWaveManagedObject (pNsmIngressEgressIpAclPolicyMO);
						}
					}
                }
            }
            status = commitTransaction ();
            if (status != FRAMEWORK_SUCCESS) {
                trace (TRACE_LEVEL_ERROR, "NsmGlobalReAssocIpAclPolicyMessageHandler: Commit Failed");
                status = WAVE_MESSAGE_ERROR;
            } else {
                status = WAVE_MESSAGE_SUCCESS;
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsMO);
        }

        pNsmGlobalReAssocIpAclPolicyMessage->setCompletionStatus (status);
        reply(pNsmGlobalReAssocIpAclPolicyMessage);
	}	
    /* IP Acl Vlan and PO code Ends */

    void IntfConfGlobalWorker::NsmGlobalReAssocMacAclPolicyMessageHandler(NsmGlobalReAssocMacAclPolicyMessage *pNsmGlobalReAssocMacAclPolicyMessage)
    {
    	ObjectId oldPolicy = pNsmGlobalReAssocMacAclPolicyMessage->getOldPolicyId();
    	ObjectId newPolicy = pNsmGlobalReAssocMacAclPolicyMessage->getNewPolicyId();
    	ResourceId status = WAVE_MESSAGE_SUCCESS;
    	IntfManagedObject *intfMO = NULL;
		UI32 size					= 0;
		UI32 j						= 0;

    	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IntfManagedObject::getClassName());
    	vector<WaveManagedObject *> *pResultsMO = querySynchronously (&syncQueryCtxt);

    	if (NULL != pResultsMO) {
    		startTransaction ();
    		UI32 numberOfResults = pResultsMO->size ();
    		for (UI32 i = 0; i < numberOfResults; i++) {
    			intfMO = dynamic_cast<IntfManagedObject *>((*pResultsMO)[i]);
                if (intfMO != NULL) {
					vector<WaveManagedObjectPointer<NsmLocalIngressEgressMacAclPolicyManagedObject> > 
						NsmIngressEgressMacAclPolicyMoVector = intfMO->getIngressEgressMacAclPolicy();
					size = NsmIngressEgressMacAclPolicyMoVector.size();
					for (j = 0; j < size; j++) {
						if ( ((NsmIngressEgressMacAclPolicyMoVector[j])->getIngressEgressMacAcl()) == oldPolicy) {
							trace(TRACE_LEVEL_INFO, "Updating NsmIngressEgressMacAclPolicyMoVector with NewPolicy");
							((NsmIngressEgressMacAclPolicyMoVector[j])->setIngressEgressMacAcl(newPolicy));
							NsmLocalIngressEgressMacAclPolicyManagedObject  *pNsmIngressEgressMacAclPolicyMO = NsmIngressEgressMacAclPolicyMoVector[j].operator->();
						    updateWaveManagedObject (pNsmIngressEgressMacAclPolicyMO);
						}
					}
                }
			}

			status = commitTransaction ();
			if (status != FRAMEWORK_SUCCESS) {
				trace (TRACE_LEVEL_ERROR, "NsmGlobalReAssocMacAclPolicyMessageHandler: Commit Failed");
				status = WAVE_MESSAGE_ERROR;
			} else {
				status = WAVE_MESSAGE_SUCCESS;
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsMO);
		}

		pNsmGlobalReAssocMacAclPolicyMessage->setCompletionStatus (status);
		reply(pNsmGlobalReAssocMacAclPolicyMessage);

	}


    void IntfConfGlobalWorker::NsmGlobalAssocMacAclPolicyPoVlanIntfMessageHandler(NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *pNsmGlobalAssocMacAclPolicyPoVlanIntfMessage)
	{
			PrismLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::validateMacAclToPoVlanStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::configureMacAclToPoVlanIntfStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updatePoVlanIntfMacAclStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
			};
			DceLinearSeqContext *pDCELinearSeqContext = new DceLinearSeqContext(pNsmGlobalAssocMacAclPolicyPoVlanIntfMessage,
							this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
			pDCELinearSeqContext->holdAll ();
			pDCELinearSeqContext->start ();
	}

	void IntfConfGlobalWorker::configureMacAclToPoVlanIntfClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		ResourceId sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatus ();
		PrismLinearSequencerContext *seqContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());
		trace(TRACE_LEVEL_DEBUG, "Entering configureMacAclToPoVlanIntfClusterCallback.....");
		prismAssert (NULL != seqContext, __FILE__, __LINE__);
		delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
		delete pWaveSendToClusterContext;
		trace(TRACE_LEVEL_DEBUG, string("configureMacAclToPoVlanIntfClusterCallback - status: ") + sendToClusterCompletionStatus);
		seqContext->executeNextStep(sendToClusterCompletionStatus);
	}

	void IntfConfGlobalWorker::validateMacAclToPoVlanStep(DceLinearSeqContext *pDceLinearSeqContext)
	{
		NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *pMsg =
				reinterpret_cast<NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *>(pDceLinearSeqContext->getPPrismMessage());
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		ssmAcl_binding_direction direction 				= pMsg->getDirection();
		string className 								= "";
		UI32 found 										= 0;
		UI32 ifType 									= pMsg->getIfType ();	
		UI32 ifId 										= pMsg->getIfId();	
	    vector<string> selectFields;
		string policyName							    = pMsg->getAclName();
		UI32 count                                      = 0;
		
		tracePrintf(TRACE_LEVEL_INFO, "In %s, type = %d, cmdCode = %d ",
				__FUNCTION__, pMsg->getIfType (), pMsg->getCmdCode());

		if ( pMsg->getCmdCode() == SSMACL_DCMD_BIND_L2_ACL )
		{
	        /* (a) Check for Mac Acl
	         * (b) First Check whether ACL Policy with same name exists for IP ACL
	         * (c) Check whether ACL Policy with same name exists for IPv6 ACL*/

	        WaveManagedObjectSynchronousQueryContext syncQueryCtxtMacAcl(MacAclPolicyManagedObject::getClassName());
	        syncQueryCtxtMacAcl.addAndAttribute (new AttributeString (policyName, "policyName"));
	        syncQueryCtxtMacAcl.setLoadOneToManyRelationships(false);
	        querySynchronouslyForCount(&syncQueryCtxtMacAcl, count);

	        if (count == 0)
	        {
				WaveManagedObjectSynchronousQueryContext syncQueryCtxtIpAcl(IpAclPolicyManagedObject::getClassName());
				syncQueryCtxtIpAcl.addAndAttribute (new AttributeString (policyName, "policyName"));
				syncQueryCtxtIpAcl.setLoadOneToManyRelationships(false);
				querySynchronouslyForCount(&syncQueryCtxtIpAcl, count);

				if(count == 0)
				{
					WaveManagedObjectSynchronousQueryContext syncQueryCtxtIpv6Acl(Ip6AclPolicyManagedObject::getClassName());
					syncQueryCtxtIpv6Acl.addAndAttribute (new AttributeString (policyName, "policyName"));
					syncQueryCtxtIpv6Acl.setLoadOneToManyRelationships(false);
					querySynchronouslyForCount(&syncQueryCtxtIpv6Acl, count);
				}
			}

	        if ( 0 == count )
	        {
	        	status = WRC_SSM_DCM_ERR_ACL_EEXIST;
	        	pDceLinearSeqContext->executeNextStep(status);
	        	return;
	        }
		}

		if (ifType == PO_INTF_TYPE) {
            className = PoIntfManagedObject::getClassName();
			if(pMsg->getCmdCode() == SSMACL_DCMD_BIND_L2_ACL){
			    selectFields.push_back("swModeFlag");
				selectFields.push_back("swBasic");
			}
        } else if (ifType == VLAN_INTF_TYPE) {
            className = VlanIntfManagedObject::getClassName();
            selectFields.push_back("remoteSpanEnabled");
        }

		if (((pMsg->getIfType () == PO_INTF_TYPE) || (pMsg->getIfType () == VLAN_INTF_TYPE)) &&
				pMsg->getCmdCode() == SSMACL_DCMD_BIND_L2_ACL) {

        	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
        	syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifId,"id"));
	        selectFields.push_back("ingressEgressMacAclPolicy");
			// adding the fields into the query context to make use of the
			// partial query
	        syncQueryCtxt.addSelectFields(selectFields);

        	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
			
			if (pResults && pResults->size() > 0) {
				if (ifType == PO_INTF_TYPE) {
                    PoIntfManagedObject *pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);

					status = WRC_SSM_DCM_ERR_ACL_MAC_PORT;
					if ((pPoIntfManagedObject) && (pPoIntfManagedObject->getSwModeFlag())) {
					  // temporarily storing the MO into the sequencer context to reuse it.
					    pDceLinearSeqContext->setPWaveManagedObject(pPoIntfManagedObject);
						found = pPoIntfManagedObject->getIngressEgressMacAclPolicyDirectioncount (direction);

						if (found)
							status = WRC_SSM_DCM_ERR_ACL_RULE_DUP;
						else 
							status = WAVE_MESSAGE_SUCCESS;
					}
				} else if (ifType == VLAN_INTF_TYPE) {
                  VlanIntfManagedObject *pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResults)[0]);
					if (pVlanIntfManagedObject) {
						if(NsmUtils::isAclAppliedOnVlanInterfaceMember(pVlanIntfManagedObject->getObjectId(), pVlanIntfManagedObject->getId(), direction))
						{
							status = WRC_SSM_DCM_ERR_ACL_RULE_DUP;
						} else {
						  // temporarily storing the MO into the sequencer context to reuse it.
							pDceLinearSeqContext->setPWaveManagedObject(pVlanIntfManagedObject);
							found = pVlanIntfManagedObject->getIngressEgressMacAclPolicyDirectioncount (direction);

							if (found)
								status = WRC_SSM_DCM_ERR_ACL_RULE_DUP;
							else if(PVlanUtils::isSecondaryVlan(ifId))
								status = WRC_NSM_PVLAN_L2_ACL_SEC;
							else if(pVlanIntfManagedObject->getRemoteSpanEnabled()) {
								NSM_PLUG_DBG_ERR_S(string("Cannot configure ACL on RSPAN VLAN"));
								status = WRC_SPAN_CANNOT_CONFIGURE_ACL_ON_RSPAN_VLAN;
							}
						}
					}
				}
			}
			
            /* GC collection */
			if (pResults) {
			    pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
   	            delete pResults;
			}
		}
		pDceLinearSeqContext->executeNextStep(status);
		return;
	}

	bool IntfConfGlobalWorker::ifVlanExistsByVlanId (UI32 vlanId, PrismLinearSequencerContext *pPrismLinearSequencerContext )
    {
        UI32 vlanCount = 0;
        bool vlanExists = true;
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlanId,"id"));
        status = querySynchronouslyForCount (&syncQueryCtxt, vlanCount);

        if (status != WAVE_MESSAGE_SUCCESS)
            {
                trace (TRACE_LEVEL_INFO, string ("querySynchronouslyForCount on vlanId failed"));
                pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
                return 0;
            }
            else
        {
            if (vlanCount != 0)
            {
                trace (TRACE_LEVEL_INFO, string ("querySynchronouslyForCount on vlanId= ") + vlanCount);
            }
            else
            vlanExists = false;
        }
        return vlanExists;
    }

	void IntfConfGlobalWorker::configureMacAclToPoVlanIntfStep(PrismLinearSequencerContext *pLinearSeqContext)
	{
		NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *pMsg = reinterpret_cast<NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *>(pLinearSeqContext->getPPrismMessage());
		UI32   ifId = pMsg->getIfId ();
		string aclName = pMsg->getAclName ();
		UI8    ifType = pMsg->getIfType ();
		UI32   cmdCode = pMsg->getCmdCode ();
		ssmAcl_binding_direction direction 					= pMsg->getDirection();

		trace(TRACE_LEVEL_DEBUG, "configureMacAclToPoVlanIntfStep: Sending config message to nodes...");

		NsmAssocMacAclPolicyPoVlanIntfSPMessage *localMsg 	= new NsmAssocMacAclPolicyPoVlanIntfSPMessage (ifId, ifType, aclName, 		cmdCode, direction);
		
		if (localMsg) {
        	WaveSendToClusterContext    *send_ctx_p;
			send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback),
				pLinearSeqContext);
			prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
			send_ctx_p->setPPrismMessageForPhase1(localMsg);
			
            if (pMsg->getIfType () == PO_INTF_TYPE) {            
                int status = SET_LOCATION_ID_API_ERROR;
                status = NsmUtils::setLocationIdsForPoSubModeCmds(pMsg->getIfId(),send_ctx_p);

                if(status != SET_LOCATION_ID_API_SUCCESS)
                {
                    trace(TRACE_LEVEL_ERROR, "In configureMacAclToPoVlanIntfStep, call to setLocationIdsForPoSubModeCmds failed ");
                    pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
                    pLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
                    if (localMsg) {
                        delete localMsg;
                    }
                    if (send_ctx_p) {
                        delete send_ctx_p;
                    }
                    return;
                }
            }
            send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);
            send_ctx_p->setPartialSuccessFlag(true);
			sendToWaveCluster(send_ctx_p);
        	pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			return;
		}
	}

	void IntfConfGlobalWorker::updatePoVlanIntfMacAclStep(DceLinearSeqContext *pDceLinearSeqContext)
	{
        trace (TRACE_LEVEL_DEVEL, "****Inside updatePoVlanIntfMacAclStep");
		NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *pMsg = reinterpret_cast<NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *>(pDceLinearSeqContext->getPPrismMessage());
		ResourceId status = WAVE_MESSAGE_ERROR;

		if (pMsg->getIfType () == PO_INTF_TYPE) {
			if (pMsg->getCmdCode () == SSMACL_DCMD_BIND_L2_ACL) {
				status = updatePoIntfMacAclStep(pDceLinearSeqContext);
			} else {
				status = removeMacAclPoVlanIntf(pDceLinearSeqContext);
			}
		} else if (pMsg->getIfType () == VLAN_INTF_TYPE) {
			if (pMsg->getCmdCode () == SSMACL_DCMD_BIND_L2_ACL) {
				status = updateVlanIntfMacAclStep(pDceLinearSeqContext);
			} else {
				status = removeMacAclPoVlanIntf(pDceLinearSeqContext);
			}
		}
		pDceLinearSeqContext->executeNextStep (status);
	}

	ResourceId IntfConfGlobalWorker::removeMacAclPoVlanIntf(PrismLinearSequencerContext *pLinearSeqContext)
	{
		NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *pMsg 	= reinterpret_cast<NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *>
			(pLinearSeqContext->getPPrismMessage());
		ResourceId status 									= WAVE_MESSAGE_ERROR;
		ssmAcl_binding_direction direction                  = pMsg->getDirection();

		if (pMsg->getIfType () == PO_INTF_TYPE) {
			UI32 poId = pMsg->getIfId();
			PoIntfManagedObject *pPoIntfManagedObject = NULL;
        	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        	syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
        	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        	if (NULL != pResults) {
            	UI32 numberOfResults = pResults->size ();
            	if (0 == numberOfResults) {
                	status = WAVE_MESSAGE_ERROR;
            	}else if (1 == numberOfResults) {
           			trace (TRACE_LEVEL_DEVEL, "removeMacAclPoVlanIntf- Obtained the Port Channel object ");
                	pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
					if (pPoIntfManagedObject) {
						pPoIntfManagedObject->delIngressEgressMacAclManagedObjectFromVector(direction);		                	
                	
                	updateWaveManagedObject(pPoIntfManagedObject);
                	status = WAVE_MESSAGE_SUCCESS;
           			trace (TRACE_LEVEL_DEVEL, "removeMacAclPoVlanIntf: Removed Mac acl from Po ");
					}
            	} else {
                	status = WAVE_MESSAGE_ERROR;
            	}
                // PASU: Mark objects for GC
                pLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
                delete pResults;
        	} else {
				status = WAVE_MESSAGE_ERROR;
			}
		} else if (pMsg->getIfType () == VLAN_INTF_TYPE) {
			UI32 vlanId = pMsg->getIfId();
			VlanIntfManagedObject *pVlanIntfManagedObject = NULL;
        	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
        	syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlanId,"id"));
        	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        	if (NULL != pResults) {
            	UI32 numberOfResults = pResults->size ();
            	if (0 == numberOfResults) {
                	status = WAVE_MESSAGE_ERROR;
            	}else if (1 == numberOfResults) {
           			trace (TRACE_LEVEL_DEVEL, "removeMacAclPoVlanIntf Obtained the Vlan object ");
                	pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResults)[0]);
					if (pVlanIntfManagedObject) {
						pVlanIntfManagedObject->delIngressEgressMacAclManagedObjectFromVector(direction);
                	updateWaveManagedObject(pVlanIntfManagedObject);
                	status = WAVE_MESSAGE_SUCCESS;
           			trace (TRACE_LEVEL_DEVEL, "removeMacAclPoVlanIntf: Removed Mac acl from Vlan ");
					}  
            	} else {
                	status = WAVE_MESSAGE_ERROR;
            	}
                // PASU: Mark objects for GC
                pLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
                delete pResults;
        	} else {
				status = WAVE_MESSAGE_ERROR;
			}
		}

		return (status);
	}


	ResourceId IntfConfGlobalWorker::updateVlanIntfMacAclStep(DceLinearSeqContext *pDceLinearSeqContext)
	{

		NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *pMsg = reinterpret_cast<NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *>(pDceLinearSeqContext->getPPrismMessage());
		ResourceId status;
		string aclName 										= pMsg->getAclName ();
		ssmAcl_binding_direction direction              	= pMsg->getDirection();
		ObjectId policyObjectId;	
		
        trace (TRACE_LEVEL_DEVEL, "****Inside updateVlanIntfMacAclStep");
		SsmGlobalGetMacAclPolicyMessage *policyMsg = new SsmGlobalGetMacAclPolicyMessage(aclName);

		if (policyMsg) {
           	trace (TRACE_LEVEL_DEVEL, "updateVlanIntfMacAclStep:3 - Obtained the Policy MO");
			status = sendSynchronously (policyMsg);
			if (status == WAVE_MESSAGE_SUCCESS && policyMsg->getCompletionStatus() == WAVE_MESSAGE_SUCCESS) {
				trace (TRACE_LEVEL_INFO, "updateVlanIntfMacAclStep: SSM Message returned success");
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				status = WAVE_MESSAGE_ERROR;
				trace (TRACE_LEVEL_ERROR, "updateVlanIntfMacAclStep: SSM Message returned with  error");
				delete policyMsg;
				return (status);
			}
			policyObjectId = policyMsg->getPolicyObjId();

			// retrieving the MO from the sequencer context to reuse it.
			VlanIntfManagedObject *pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>(pDceLinearSeqContext->getPWaveManagedObject());

			if(pVlanIntfManagedObject){
			  // sets the field in the MO to make use of the partial commit
                pVlanIntfManagedObject->addAttributeToBeUpdated("ingressEgressMacAclPolicy"); 
				trace (TRACE_LEVEL_DEVEL, "updateVlanIntfMacAclStep:4- Obtained	the Vlan  object ");
				NsmLocalIngressEgressMacAclPolicyManagedObject *NsmIngressEgressMacAclMO = new NsmLocalIngressEgressMacAclPolicyManagedObject(dynamic_cast
	                          <NsmGlobalObjectManager*>(getPWaveObjectManager()), policyObjectId, direction);
				pVlanIntfManagedObject->addIngressEgressMacAclManagedObjectToVector (NsmIngressEgressMacAclMO);
				updateWaveManagedObject(pVlanIntfManagedObject);
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				status = WAVE_MESSAGE_ERROR;
			}
			delete policyMsg;

		} else {
			status = WAVE_MESSAGE_ERROR;
		}

		return (status);

	}


	ResourceId IntfConfGlobalWorker::updatePoIntfMacAclStep(DceLinearSeqContext	*pDceLinearSeqContext)
	{

		NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *pMsg = reinterpret_cast<NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *>(pDceLinearSeqContext->getPPrismMessage());
		ResourceId status;
		string aclName = pMsg->getAclName();
		ssmAcl_binding_direction direction  				= pMsg->getDirection();
		ObjectId policyObjectId;
		
		
        trace (TRACE_LEVEL_DEVEL, "****Inside updatePoIntfMacAclStep");
		SsmGlobalGetMacAclPolicyMessage *policyMsg = new SsmGlobalGetMacAclPolicyMessage(aclName);

		if (policyMsg) {
           	trace (TRACE_LEVEL_DEVEL, "updatePoIntfMacAclStep:3 - Obtained the Policy MO");
			status = sendSynchronously (policyMsg);
			if (status == WAVE_MESSAGE_ERROR) {
				trace (TRACE_LEVEL_ERROR, "updatePoIntfMacAclStep: SSM Message returned with  error");
                delete policyMsg;
				return (status);
			} else {
				trace (TRACE_LEVEL_INFO, "updatePoIntfMacAclStep: SSM Message returned success");
			}
			policyObjectId = policyMsg->getPolicyObjId();

			// retrieving the MO from the sequencer context to reuse it.
			PoIntfManagedObject *pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>(pDceLinearSeqContext->getPWaveManagedObject());     

			if (pPoIntfManagedObject) {
			  // sets the field in the MO to make use of the partial commit
			    pPoIntfManagedObject->addAttributeToBeUpdated("ingressEgressMacAclPolicy");
				NsmLocalIngressEgressMacAclPolicyManagedObject *NsmIngressEgressMacAclMO = new 
					NsmLocalIngressEgressMacAclPolicyManagedObject(dynamic_cast
							<NsmGlobalObjectManager*>(getPWaveObjectManager()), policyObjectId, direction); 	
				pPoIntfManagedObject->addIngressEgressMacAclManagedObjectToVector (NsmIngressEgressMacAclMO);	
				updateWaveManagedObject(pPoIntfManagedObject);
				status = WAVE_MESSAGE_SUCCESS;
    		} else {
               	status = WAVE_MESSAGE_ERROR;
        	}
			delete policyMsg;

		} else {
			status = WAVE_MESSAGE_ERROR;
		}

		return (status);
	}
						

    void  IntfConfGlobalWorker::NsmGlobalAddPoToVlanIntfMessageHandler( NsmGlobalAddPoToVlanIntfMessage *pNsmGlobalAddPoToVlanIntfMessage)
    {
// Validations and Queries should go here

//        pNsmGlobalAddPoToVlanIntfMessage->setIsSynchronousMessage(true); 
        

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfGlobalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfGlobalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfGlobalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfGlobalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmGlobalAddPoToVlanIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IntfConfGlobalWorker:: getVlanIntfObjectIdByVlanIfName (UI32 vlanName, ObjectId &poIntf_oid,
									  bool removeImmediate, DceLinearSeqContext *pDceLinearSeqContext)
	{
		ResourceId  status   = WAVE_MESSAGE_ERROR;
		VlanIntfManagedObject *pVlanIntfManagedObject = NULL;
        
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());                      
		syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlanName,"id"));
        syncQueryCtxt.addSelectFieldForObjectId ("objectId");
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);                                
		if (pResults && pResults->size() > 0) 
		{
			pVlanIntfManagedObject =  dynamic_cast<VlanIntfManagedObject*>((*pResults)[0]);
			poIntf_oid = pVlanIntfManagedObject->getObjectId();  
			status = WAVE_MESSAGE_SUCCESS;                                                                                  
		}
		if(pResults)
		{
			if(removeImmediate)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			else
			{
				if(pDceLinearSeqContext)
					pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
				pResults->clear();
				delete pResults;
			}
		}
		return status;                                                                                                  
	} 
    
    ResourceId IntfConfGlobalWorker:: isPoIntfHasMember (ObjectId &poIntf_oid)
    {
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeObjectId (&poIntf_oid,"poId"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
            pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);
            if(pPhyIntfLocalManagedObject->getPoId() == poIntf_oid)
            {
                status = WAVE_MESSAGE_SUCCESS;
            }
        }

        if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);	
        }
        return status;
    }

    PoIntfManagedObject * IntfConfGlobalWorker:: getPoIntfObjectByPoIfName (string poName)
    {
        PoIntfManagedObject *pPoIntfManagedObject = NULL;
        //ResourceId  status   = WAVE_MESSAGE_ERROR;
        UI32 po_id;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        po_id = strtoul(poName.c_str (), NULL, 0);
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&po_id,"id"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
            pPoIntfManagedObject =  dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
            return pPoIntfManagedObject;
            //status = WAVE_MESSAGE_SUCCESS;
        }

        if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);	
            //pResults->clear();
            //delete pResults;
        }
        return pPoIntfManagedObject;
    }
        
    ResourceId IntfConfGlobalWorker:: getPoIntfObjectIdByPoIfName (string poName, ObjectId &poIntf_oid, 
									  bool removeImmediate, PrismLinearSequencerContext *pPrismLinearSequencerContext)
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

		if(pResults)
		{
			if(removeImmediate)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);	
			else
			{
				if(pPrismLinearSequencerContext)
					pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
				pResults->clear();
				delete pResults;
			}
		}
        return status;
    }

   ResourceId IntfConfGlobalWorker::getMacFromDomainId(string &mac, UI32 domainId)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (domainId, "domainId"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

        if (pResults && pResults->size() == 1) {
            pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *>((*pResults)[0]);
            mac = pVcsNodeLocalManagedObject->getSwitchMac().toString ();
            status = WAVE_MESSAGE_SUCCESS;
        }

        if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);	
        }
        return status;
    } 

    ObjectId
    IntfConfGlobalWorker::FcoeMapGetObjectIdByName (string &map_name,PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        FcoeMapManagedObject *map_mo_p;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&map_name, "map_name"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
       
        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
			if(pPrismLinearSequencerContext)
				pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			pResults->clear ();
			delete pResults;
            return ObjectId::NullObjectId;
        }

        map_mo_p = dynamic_cast<FcoeMapManagedObject *> ((*pResults)[0]);
	    ObjectId oid = map_mo_p->getObjectId();

		if(pResults)
		{
			if(pPrismLinearSequencerContext)
				pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			pResults->clear ();
			delete pResults;
		}
        return oid;
    }

	NativeVlanIntfConfLocalManagedObject*
    IntfConfGlobalWorker::NativeVlanPoGetChildMoByParentMo(PoIntfManagedObject *pPoIntfManagedObject)
    {
        trace (TRACE_LEVEL_DEBUG, "IntfConfGlobalWorker::NativeVlanPoGetChildMoByParentMo: Entered....");

        NativeVlanIntfConfLocalManagedObject *pNativeVlanIntfConfLocalManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NativeVlanIntfConfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeObjectId (pPoIntfManagedObject->getObjectId(), "ownerManagedObjectId"));
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


    /* Code Starts Here */
    void IntfConfGlobalWorker::createPoIntfMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        trace(TRACE_LEVEL_INFO, "Entering createPoIntfMOStep.....");

        ResourceId  status;
        NsmGlobalCreatePoIntfMessage *pNsmGlobalCreatePoIntfMessage = dynamic_cast<NsmGlobalCreatePoIntfMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
        status = createPoIntfMO(pNsmGlobalCreatePoIntfMessage, pPrismLinearSequencerContext);
        pPrismLinearSequencerContext->executeNextStep(status);
    }

    ResourceId IntfConfGlobalWorker::createPoIntfMO (NsmGlobalCreatePoIntfMessage *pNsmGlobalCreatePoIntfMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
      string null_str = "";
        NSM_PLUG_DBG("Entering ...");
        
        
        /* If the po interface already exists in the database. If present don't create it again.*/
        /* checking po entry is existing or not */
        PoIntfManagedObject *pPoIntfMO = NULL;
        UI32 poId = strtoul(pNsmGlobalCreatePoIntfMessage->getPoName().c_str (), NULL, 0);
        UI64 ifIndex = 0; // TBDDCMOSS : ifindex_create_trunk_idx(poId);
        NSM_PLUG_DBG_S(string("port-channel ") + poId + string(" if index = ") + ifIndex);

        ResourceId status = NsmUtils::getPoMoByPoId(poId, pPoIntfMO);
        NSM_PLUG_DBG_S("Retrieved PoINtfMO for poId " + poId + ", status = "  + status);
        //TODO: Status is not used in any fashion...?

        if (pPoIntfMO != NULL) {
            NSM_PLUG_DBG_ERR_S(string("CreatePoIntfMO: Po Interface already exists ..") + pNsmGlobalCreatePoIntfMessage->getPoId());
    		pNsmGlobalCreatePoIntfMessage->setPoRef(pPoIntfMO);
    		pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pPoIntfMO);
            return WAVE_MESSAGE_SUCCESS;
        }

        PoIntfManagedObject *pPoIntfManagedObject = new PoIntfManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));

        prismAssert (NULL != pPoIntfManagedObject, __FILE__, __LINE__);

        pPoIntfManagedObject->setId (strtoul((pNsmGlobalCreatePoIntfMessage->getPoId()).c_str (), NULL, 0));
        pPoIntfManagedObject->setIfName (pNsmGlobalCreatePoIntfMessage->getPoName());
        pPoIntfManagedObject->setIfIndex (ifIndex);
        pPoIntfManagedObject->setSwMode ((swModeEnum)SW_MODE_UNKNOWN);
        pPoIntfManagedObject->setMtu (NSM_MTU_DEF_VAL);
        pPoIntfManagedObject->setSwModeFlag (false);
        pPoIntfManagedObject->setStatus(true);
        pPoIntfManagedObject->setIsAllowedVlanAll(false);
        pPoIntfManagedObject->setDesc (pNsmGlobalCreatePoIntfMessage->getDesc());
		pPoIntfManagedObject->setPoMode(NSM_PO_MODE_ON);
		pPoIntfManagedObject->setPoType(NSM_PO_TYPE_STANDARD);
		pPoIntfManagedObject->setPoMinLinks(NSM_MINLINKS_DEF_VAL);
		pPoIntfManagedObject->setPoAllowedSpeed ((LineSpeed)2);
		pPoIntfManagedObject->setPoLoadBalanceMode((poLoadBalanceModeEnum)LOADBAL_MODE_SRC_DST_IP_MAC_VID_PORT);
        pPoIntfManagedObject->setPoPrimaryLinkDomainId(0);
        pPoIntfManagedObject->setPoPartnerSystemPriority(0);
        pPoIntfManagedObject->setPoPartnerKey(0);
        pPoIntfManagedObject->setPoPartnerSystem(null_str);

        pPoIntfManagedObject->setFcoeport(WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject>(NULL));
		pPoIntfManagedObject->setQosCosMap(ObjectId::NullObjectId);
		pPoIntfManagedObject->setQosDscpMap(ObjectId::NullObjectId);
		pPoIntfManagedObject->setQosDscpTrafficClassMap(ObjectId::NullObjectId);
		pPoIntfManagedObject->setQosTrafficClassMap(ObjectId::NullObjectId);
		pPoIntfManagedObject->setQosCosMap(ObjectId::NullObjectId);
		pPoIntfManagedObject->setCeeMap(ObjectId::NullObjectId);

         
        pPoIntfManagedObject->setAccessVlanObjectId(ObjectId::NullObjectId);

        pPoIntfManagedObject->setPoMode(pNsmGlobalCreatePoIntfMessage->getPoMode());
        pPoIntfManagedObject->setPoType(pNsmGlobalCreatePoIntfMessage->getPoType());
        pPoIntfManagedObject->setPoLacpSystemId(pNsmGlobalCreatePoIntfMessage->getPoLacpSystemId());
        pPoIntfManagedObject->setPoLacpSystemIdDomainId(pNsmGlobalCreatePoIntfMessage->getPoLacpSystemIdDomainId());

        if(WAVE_PERSISTENCE_CLUSTER_ENABLED != DcmToolKit::isVcsEnabled()){
        	pPoIntfManagedObject->setIgnoreSplit(false);
        }

        /* creating MO at init, otherwise 'no spanning-tree shut' fails */
	XstpPortConfigManagedObject *pXstpPortConfigManagedObject = new XstpPortConfigManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
	prismAssert (NULL != pXstpPortConfigManagedObject, __FILE__, __LINE__);

	string ifName = pPoIntfManagedObject->getIfName();
	pXstpPortConfigManagedObject->setDefaultAttribute(ifName);
	pPoIntfManagedObject->setPortStpConfig(WaveManagedObjectPointer<XstpPortConfigManagedObject>(pXstpPortConfigManagedObject));
	//pPoIntfManagedObject->updatePortStpConfig(pXstpPortConfigManagedObject);
	//pPoIntfManagedObject->setPortStpConfig(WaveManagedObjectPointer<XstpPortConfigManagedObject>(NULL));

	pPoIntfManagedObject->setNativeVlanIntfConfig (WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (NULL));

    pNsmGlobalCreatePoIntfMessage->setPoRef(pPoIntfManagedObject);

    NSM_PLUG_DBG_S(string("Entering createPoIntfMO.....") + WAVE_MESSAGE_SUCCESS);

	if(pPrismLinearSequencerContext)
	{
		pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pPoIntfManagedObject);    
	}
    return WAVE_MESSAGE_SUCCESS;

    }

    void IntfConfGlobalWorker::createPoIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {

        NsmGlobalCreatePoIntfMessage *pNsmGlobalCreatePoIntfMessage = dynamic_cast<NsmGlobalCreatePoIntfMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

		if((WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()) && (!pNsmGlobalCreatePoIntfMessage->getSendToLocal()))
		{
			pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        	//pNsmGlobalCreatePoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			return;
		}
		
        NsmCreatePoIntfSPMessage *m = new NsmCreatePoIntfSPMessage();
        //"tm","tt",1,"testing");
        WaveSendToClusterContext    *send_ctx_p;

        m->setPoId (pNsmGlobalCreatePoIntfMessage->getPoId());
        m->setPoName (pNsmGlobalCreatePoIntfMessage->getPoName());
        m->setDesc (pNsmGlobalCreatePoIntfMessage->getDesc());

		send_ctx_p = new WaveSendToClusterContext(this, 
			reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback),pPrismLinearSequencerContext);

		prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
		
		if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
		{
			vector <LocationId> locationIdVector;
			locationIdVector.push_back(pNsmGlobalCreatePoIntfMessage->getLocationId());
			send_ctx_p->setLocationsToSendToForPhase1(locationIdVector);
		}				
						
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);

        pNsmGlobalCreatePoIntfMessage->setPoIfIndex(m->getPoIfIndex());
                
        pNsmGlobalCreatePoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        return;
    }

      void IntfConfGlobalWorker::validatePhyIntfToPoIntfStep (DceLinearSeqContext *pDceLinearSeqContext)
      {                                                                       
          NSM_PLUG_DBG("Entering ...");
  
		  int PoSize = 0;
          PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
          ResourceId  status   = WAVE_MESSAGE_ERROR;
          NsmGlobalAddPhyIntfToPoIntfMessage *pNsmGlobalAddPhyIntfToPoIntfMessage = dynamic_cast<NsmGlobalAddPhyIntfToPoIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());
  
          ResourceId isVCS = DcmToolKit::isVcsEnabled ();
          string intf_name = pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName();
          string poName = pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName();
          PoSize = NsmUtils::getPoNumOfMemberPorts(poName);
          UI32 po_id = strtoul(poName.c_str(), NULL, 0);

		if(po_id < 0 || po_id > DCM_PORTCHANNEL_MAX_LAGS)
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validatePhyIntfToPoIntfStep: Invalid port-channel id:") + po_id);

			status = WRC_NSM_ERR_INVALID_PO_ID;
			pNsmGlobalAddPhyIntfToPoIntfMessage->setCompletionStatus(WRC_NSM_ERR_INVALID_PO_ID);
			pDceLinearSeqContext->executeNextStep(status);

			return;
		}

 		  if(NSM_PO_TYPE_BROCADE == pNsmGlobalAddPhyIntfToPoIntfMessage->getPoType())
		  {

			  if( (PoSize != 0) && (WAVE_PERSISTENCE_CLUSTER_ENABLED == isVCS) )
			  {
				  vector<LocationId> PoLocationIds;
				  LocationId IntfLocationId = NsmUtils::getLocationId(intf_name);
				  NsmUtils::getPoMemberLocationIds(poName, PoLocationIds);
					
				  if(IntfLocationId != PoLocationIds[0])
				  {
					  NSM_PLUG_DBG_ERR("Ports from different chip can't be aggregated for brocade trunks");
					  status = WRC_NSM_LACP_ERROR_CHIP_NUM_MISMATCH;
					  pDceLinearSeqContext->executeNextStep(status);
					  return;
				  }
			  }
			if (NsmUtils::isFcoeConfiguredForLagPort(po_id) == true) {
				status = WRC_NSM_PO_FCOE_CONFIG_PHY_BRCD;
				pDceLinearSeqContext->executeNextStep(status);
				return;
			}
		  }

          /* Removing this check in Nos4.0.0 to make it enable to add members from unlimited number of rbridges in MC mode.
          // Check for number of Rbridges participating in vLAG
          if (WAVE_PERSISTENCE_CLUSTER_ENABLED == isVCS) {
              vector<SI32> memberRbridges;
              SI32 intfMappedId = NsmUtils::getMappedIdFromIfName(intf_name);
              int rbridgeCount = NsmUtils::getPoNumOfRBridges(poName, memberRbridges);
              if((DCM_PORTCHANNEL_MAX_RBRIDGES == rbridgeCount) &&
                  (find(memberRbridges.begin(), memberRbridges.end(), intfMappedId) == memberRbridges.end()))
		  {
				NSM_PLUG_DBG_ERR("Reached maximum RBridges participate in vLAG");
				status = WRC_NSM_API_ERR_PORTCHANNEL_MAX_RBRIDGES;
          		pDceLinearSeqContext->executeNextStep(status);
          		return;
		  }
          }
          */
		  PoIntfManagedObject *pPoIntfManagedObject = NULL;
		  WaveManagedObjectSynchronousQueryContext poSyncQueryCtxt(PoIntfManagedObject::getClassName());
		  poSyncQueryCtxt.addAndAttribute (new AttributeUI32(&po_id,"id"));
		  vector<WaveManagedObject *> *poIntResults = querySynchronously (&poSyncQueryCtxt);

		  if (poIntResults && poIntResults->size() > 0) {
			 pPoIntfManagedObject =  dynamic_cast<PoIntfManagedObject*>((*poIntResults)[0]);
             pDceLinearSeqContext->addManagedObjectForGarbageCollection(pPoIntfManagedObject);
			 poIntResults->clear();
			 delete poIntResults;
		  } else {
			  ResourceId resId = validatePortChannelCount();
			  if (WAVE_MESSAGE_SUCCESS != resId) {
				  if (poIntResults) {
					  WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(poIntResults);
				  }
				  pDceLinearSeqContext->executeNextStep(resId);
				  return;
			  }
		  }
		  
          WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
          syncQueryCtxt.addAndAttribute (new AttributeString (&intf_name,"Id"));
          vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
  
          if (pResults && pResults->size() == 1) {
              pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);
              pDceLinearSeqContext->addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);
			  pResults->clear();
			  delete pResults;

              bool isPortProfiled = false;
              APPMUtils::isPortProfilePort(pPhyIntfLocalManagedObject, isPortProfiled);

              if (isPortProfiled){
                  status = WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED;
                  pDceLinearSeqContext->executeNextStep(status);
                  return;
              }
			  /* Brocade Trunk port on 40G interface should not be allowed */
			  if(NSM_PO_TYPE_BROCADE == pNsmGlobalAddPhyIntfToPoIntfMessage->getPoType()) {
				  if (IF_TYPE_FO == pPhyIntfLocalManagedObject->getPhyType()) {
					  status = LACP_UNABLE_TO_SET_40G_BROCADE_TRUNK; 	  
					  pDceLinearSeqContext->executeNextStep(status);
					  return;	  
				  }
                  if(true == (pPhyIntfLocalManagedObject->getUdldEnable())) {
                      tracePrintf(TRACE_LEVEL_ERROR, 
                              "Brocade trunking cannot be enabled on an UDLD enabled port.");
                      status = WRC_UDLD_API_ERR_BROCADE_TRUNKING_NOT_ALLOWED_ON_UDLD_ENABLED_IF;
                      pDceLinearSeqContext->executeNextStep(status);
                      return;
                  }
			  }			  
              // Add check for L3 configurations. If L3 configs like PIM, OSPF or IGMP configured
              // on interface, then do not allow channel-group
              NsmL3ReadyManagedObject *l3ReadyMO = pPhyIntfLocalManagedObject->getL3ReadyMO();
              status = validateL3ConfigForChannelGroup(l3ReadyMO);
              if (WAVE_MESSAGE_SUCCESS != status) {
                  pDceLinearSeqContext->executeNextStep(status);
                  return;
              }

			  /* Check if new interface type is same as existing member interface type */
			  if (PoSize > 0) {
				  // Get the first member interface and compare its type with new member
                  // If PO has first member as 40G, then it should not allow any other type
                  // If PO has either 1G or 10G as first member, then it should not allow 40G
                  InterfaceType firstMemberIntfType = getPoFirstMemberPhyIntfType(poName);
                  InterfaceType newMemberIntfType = pPhyIntfLocalManagedObject->getPhyType();
                  if ((IF_TYPE_FO == firstMemberIntfType && IF_TYPE_FO != newMemberIntfType) ||
                      (IF_TYPE_GI == firstMemberIntfType && IF_TYPE_FO == newMemberIntfType) ||
                      (IF_TYPE_TE == firstMemberIntfType && IF_TYPE_FO == newMemberIntfType)) { 
					  pDceLinearSeqContext->executeNextStep(WRC_NSM_PORTCHANNEL_INTERFACE_TYPE_MISMATCH);
					  return;
				  }
			  }
              pNsmGlobalAddPhyIntfToPoIntfMessage->setPhyRef(pPhyIntfLocalManagedObject);

			  /* Qos Validation -- starts */

			  /* If Phy port is 1G and Port-Channel has PFC configured, throw error */
			  if (pPhyIntfLocalManagedObject->getPhyType() == IF_TYPE_GI )
			  {
			      if (pPoIntfManagedObject)
				  {
				      if (pPoIntfManagedObject->getPfcFlowControl().size() != 0)
                      {
				  		  pDceLinearSeqContext->executeNextStep(WRC_SSM_INTF_CANNOT_ADD_1G_CU_TO_PO_WITH_PFC);
				  		  return;
                      }
				  }
			  }

              if(pPhyIntfLocalManagedObject->getPoId() == ObjectId::NullObjectId)
              {
                  NSM_PLUG_DBG_S("The port is not configured for dynamic aggregation");
                  status = WAVE_MESSAGE_SUCCESS;
				  //Check whether PO MO exists, if exists then do validation
				  if(pPoIntfManagedObject != NULL)
				  {
				  	  /*
					   * If LAG has QoS config , then do not allow adding fcoe port or 
				  	   * port having cee map. Allow adding port with non-cee
					   * config, as non-cee configuration on the port will be deleted. 
					   */

					if ((QosUtils::isInterfaceQosNonCeeConfigured(pPoIntfManagedObject) ||
						QosUtils::isInterfaceQosCeeConfigured(pPoIntfManagedObject) ||
						QosUtils::isInterfaceMQCConfigured(pPoIntfManagedObject))
						&& (QosUtils::isInterfaceQosCeeConfigured(pPhyIntfLocalManagedObject) ||
						QosUtils::isInterfaceFcoeConfigured(pPhyIntfLocalManagedObject))) {

						  status =  WRC_SSM_INTF_QOS_ON_LAG_IS_ALREADY_APPLIED;
                          goto last;
					}
				  	  /*
					   * If LAG has Fcoe config , then do not allow adding fcoe port or 
				  	   * port having cee map. Allow adding port with non-cee
					   * config, as non-cee configuration on the port will be deleted. 
					   */
					else if (QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject) &&
						(QosUtils::isInterfaceQosCeeConfigured(pPhyIntfLocalManagedObject) ||
						 QosUtils::isInterfaceFcoeConfigured(pPhyIntfLocalManagedObject))) {

						  status =  WRC_SSM_INTF_FCOE_ON_LAG_IS_ALREADY_APPLIED;
                          goto last;
					}

				  }
                  //Irrespective of po is empty ot not, do not allow a physical port containing mls/mqc to be added to port-channel.
                    if(QosUtils::isInterfaceQosNonCeeConfigured(pPhyIntfLocalManagedObject)
                                || QosUtils::isInterfaceMQCConfigured(pPhyIntfLocalManagedObject)) {
                        status =  WRC_SSM_INTF_QOS_ON_LAG_MEMBER_IS_ALREADY_APPLIED;
                        goto last;
                    }

                    if( (QOS_DEFAULT_COS_VALUE != pPhyIntfLocalManagedObject->getDefaultCos())
                    		/*|| (0 != (pPhyIntfLocalManagedObject->getBumConfig()).size())*/) {
                        status =  WRC_SSM_INTF_QOS_ON_LAG_MEMBER_IS_ALREADY_APPLIED;
                        goto last;
                    }

              }
			  /* Qos Validation -- end */
              else
              {
                  NSM_PLUG_DBG_ERR("The port is already configured for dynamic aggregation");
                  status = WAVE_MESSAGE_ERROR;
              }
              if(NSM_PO_TYPE_BROCADE == pNsmGlobalAddPhyIntfToPoIntfMessage->getPoType())
              {
                  if (pPhyIntfLocalManagedObject->getPhyType() == IF_TYPE_GI)
                  {
                      NSM_PLUG_DBG_ERR("Cannot configure channel-group on carme1G");
                      status = WRC_NSM_ERR_CMD_NOT_SUPPORTED_ON_1GIGA_INT;
                      NSM_PLUG_DBG_S("Cannot configure channel-group on carme1G " + FrameworkToolKit::localize(status));
                  }
              }

last:	    if (status != WAVE_MESSAGE_SUCCESS){
				  pDceLinearSeqContext->executeNextStep(status);
				  return;
			  }

          }
		  
          pDceLinearSeqContext->executeNextStep(status);
          return;
      }

    void IntfConfGlobalWorker::addPhyIntfToPoIntfLocalDBCommitStep (DceLinearSeqContext *pDceLinearSeqContext)
    {
        NsmGlobalAddPhyIntfToPoIntfMessage *pNsmGlobalAddPhyIntfToPoIntfMessage = dynamic_cast<NsmGlobalAddPhyIntfToPoIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

        LocationId locationId = 0;
        UI32 mappedId = 0;

        NSM_PLUG_INFO_S("Entering ...");

        WaveSendToClusterContext    *send_ctx_p;
        NsmAddPhyIntfToPoIntfMessage *m = new NsmAddPhyIntfToPoIntfMessage();
        //"tm","tt",1,"testing");
        m->setIfType (pNsmGlobalAddPhyIntfToPoIntfMessage->getIfType());
        

        m->setIfName (pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName());
        m->setPoName (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName());
        m->setPoMode (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoMode());
        m->setPoType (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoType());
        m->setPoLacpSystemId (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoLacpSystemId());
        m->setPoLacpSystemIdDomainId (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoLacpSystemIdDomainId());
		m->setReplayPoConfigs(pNsmGlobalAddPhyIntfToPoIntfMessage->getReplayPoConfigs());
		m->setCmdCode(NSMUPDATE_PHY_ADD_CHANNELGROUP_DBCOMMIT_CMD);

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalAddToPoCallback),
			pDceLinearSeqContext);

		prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);

		if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
		{
			mappedId = (UI32) NsmUtils::getMappedIdFromIfName(pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName());
			locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
			vector <LocationId> locationIdVector;
			locationIdVector.push_back(locationId);

            NSM_PLUG_DBG_S( string("Send to target location ::") + locationId);
			send_ctx_p->setLocationsToSendToForPhase1(locationIdVector);
		}

        NSM_PLUG_INFO_S("Sending to local nsm plugin ");
		sendToWaveCluster(send_ctx_p);

        pNsmGlobalAddPhyIntfToPoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        return;
    }

    void IntfConfGlobalWorker::addPhyIntfToPoIntfStep (DceLinearSeqContext *pDceLinearSeqContext)
    {
        /*
         * if(!po)
         * {
         *      systemid, macaddress get from inputMsg
         *          ifName->mappedid->domainid->macaddress
         *          systemid
         *          
         *      mode and type get form inputMsg
         *
         *      - update to channel-group Message (systemid, mac address)
         *      
         *       Plugin - > backend -> plugin
         *
         *       if(success)
         *       {
         *          - create po (systemid, mac address)
         *          - update po (systemid, mac address) 
         *       }
         *      
         * }
         * else
         * {
         *      if( no members in po intf)
         *      {
         *          systemid, macaddress get from inputMsg
         *              ifName->mappedid->domainid->macaddress
         *              systemid
         *          mode and type get form inputMsg
         *          
         *          - update to channel-group Message (systemid, mac address)
         *
         *          Plugin - > backend -> plugin    
         *          
         *          if( success)
         *          {
         *              - update po (systemid, mac address)
         *              - update phy intf ( pomode, potype )
         *          }
         *          
         *      } else {
         *                  
         *          - Validation Phase
         *                  From the MO Get MO_poMode , Mo_poType 
         *                  From the Input Get In_Mode , In_Type
         *                  
         *                  if( MO_poMode(on) == inputMode(on))
         *                  {
         *                  }
         *                  
         *                  if( Mo_poType(x) == inputType(x)
         *                  {
         *                  }
         *
         *          From the MO get systemid , mac address
         *          mode and type from inputMsg
         *          
         *          - update to channel-group Message (systemid, mac address)
         *
         *          plugin -> backend -> plugin
         *
         *          if(channel-group memeber is the first member of the Rbridge)
         *              - Play back pointf cmd from  Plugin -> backend on target
         *              Rbridge.
         *
         *          if(success)
         *          {
         *              - update phy intf (pomode, potype)
         *          }
         *      }
         * }
         */ 

        NsmGlobalAddPhyIntfToPoIntfMessage *pNsmGlobalAddPhyIntfToPoIntfMessage = dynamic_cast<NsmGlobalAddPhyIntfToPoIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

        ResourceId          vcs_status = WAVE_MESSAGE_ERROR;
        LocationId locationId = 0;
        UI32 mappedId = 0;
        UI32 domainId;
		string switchMacAddress;
		ResourceId  status   = WAVE_MESSAGE_ERROR;
		ObjectId poObjectId;
		UI32 prio = 128;
		vector<UI32>    vlanIdVector;

        NSM_PLUG_INFO_S("Entering ...");

        vcs_status = DcmToolKit::isVcsEnabled ();
        pNsmGlobalAddPhyIntfToPoIntfMessage->setPoLacpSystemId("");
        pNsmGlobalAddPhyIntfToPoIntfMessage->setPoLacpSystemIdDomainId(0);
        pNsmGlobalAddPhyIntfToPoIntfMessage->setReplayPoConfigs(false);

        /* checking po entry is existing or not */
        PoIntfManagedObject *pPoIntfManagedObject = NULL;
        UI32 po_id;
        WaveManagedObjectSynchronousQueryContext poSyncQueryCtxt(PoIntfManagedObject::getClassName());
        po_id = strtoul(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName().c_str(), NULL, 0);
        poSyncQueryCtxt.addAndAttribute (new AttributeUI32(&po_id,"id"));
        vector<WaveManagedObject *> *poResults = querySynchronously (&poSyncQueryCtxt);


	    mappedId = (UI32) NsmUtils::getMappedIdFromIfName(pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName());

        if (poResults && poResults->size() > 0) {
            pPoIntfManagedObject =  dynamic_cast<PoIntfManagedObject*>((*poResults)[0]);
        }

        if(pPoIntfManagedObject == NULL) {
            /* Po-Mode and Po-Type get from inputMsg */
            NSM_PLUG_DBG_S( string("PoIntf Entry does not exist : ") + pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName());

            if(WAVE_PERSISTENCE_CLUSTER_ENABLED == vcs_status) {

                /*
                 * systemid, macaddress get from inputMsg
                 * - Update to channel-group Message (systemid, mac address)
                 */ 

                mappedId = (UI32) NsmUtils::getMappedIdFromIfName(pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);

                /* Retrieve the base MAC address of the switch that*/
                /* owns the physical port being added to the vLAG */
                domainId = DcmToolKit::getDomainIdFromMappedId(mappedId);

                if(getMacFromDomainId(switchMacAddress, domainId) == WAVE_MESSAGE_SUCCESS)
                { 

                    NSM_PLUG_DBG_S( string("switchMacAddress : ") + switchMacAddress + string ("domainid : ") + domainId);
                    pNsmGlobalAddPhyIntfToPoIntfMessage->setPoLacpSystemId(switchMacAddress);
                    pNsmGlobalAddPhyIntfToPoIntfMessage->setPoLacpSystemIdDomainId(domainId);

                } 
            }
            /* To create po interface on MO*/
            pNsmGlobalAddPhyIntfToPoIntfMessage->setPoCmdCode(NSMCREATE_PO_INTF);

        } else {	/* Found object id for port channel */
            pNsmGlobalAddPhyIntfToPoIntfMessage->setPoRef(pPoIntfManagedObject);
            poObjectId =  pPoIntfManagedObject->getObjectId();
            
            NSM_PLUG_DBG_S( string("PoIntf Entry exists : ") + pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName());
            
            /* Retrive physical port managed objects of physical ports already assigned to the port channel */
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute (new AttributeObjectId(poObjectId,"poId"));
            vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

            if(WAVE_PERSISTENCE_CLUSTER_ENABLED == vcs_status) 
                pNsmGlobalAddPhyIntfToPoIntfMessage->setReplayPoConfigs(true);
             
           int 	count =	NsmUtils::queryPoEdgeMembersCountForLocationId(poObjectId,NsmUtils::getLocationId(pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName()));
                    NSM_PLUG_DBG_S( string("PO edge member count :-1 ") +count + string ("domainid : ") + pPoIntfManagedObject->getPoLacpSystemId());
            if ((pResults->size() == 0) || count == 0) { /* No physical ports assigned to port-channel  or no physical ports from this localtion Is a member port*/ 
				if(WAVE_PERSISTENCE_CLUSTER_ENABLED == vcs_status) {
					vector<ObjectId>  m_vlanid = pPoIntfManagedObject->getVlanVector();

					if (m_vlanid.size() > 0) {
						NSM_PLUG_INFO_S("addPhyIntfToPoIntfStep: VlanVector is Non-NULL");
						unsigned int j = 0;
						WaveManagedObject   *pWaveManagedObject = NULL;
						VlanIntfManagedObject *pVlanIntfManagedObject;
						for (j = 0; j < m_vlanid.size(); j++) {
							if (m_vlanid[j] == ObjectId::NullObjectId) {
								NSM_PLUG_INFO_S(j + string ("VLAN ObjId Null: continue"));
								continue;
							}
							pWaveManagedObject = queryManagedObject (m_vlanid[j]);
							if (NULL != pWaveManagedObject) {
								pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>(pWaveManagedObject);
								if (NULL != pVlanIntfManagedObject) {
									vlanIdVector.push_back(pVlanIntfManagedObject->getId());
									delete pVlanIntfManagedObject;
								}
								else {
									trace(TRACE_LEVEL_ERROR, string("addPhyIntfToPoIntfStep: pVlanIntfManagedObject is NULL  for a port-channel."));
                                    if (poResults) {
                                        pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*poResults);
                                        poResults->clear();
                                        delete poResults;
                                    }
									return;
								}
							}
						}
					}
					prio = pPoIntfManagedObject->getEldPrio();
                }
            }
            /* if( no members in po intf) */
            if (pResults->size() == 0) { /* No physical ports assigned to port-channel */
                NSM_PLUG_DBG_S( "No physical ports assigned to port-channel");
            

                if(WAVE_PERSISTENCE_CLUSTER_ENABLED == vcs_status) {

                    /* Systemid, macaddress get from inputMsg
                     * po-Mode and Po-Type get form inputMsg
                     * - update to channel-group Message (systemid, mac address)
                     */

                    mappedId = (UI32) NsmUtils::getMappedIdFromIfName(pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName());
                    locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);

                    /* Retrieve the base MAC address of the switch that*/
                    /* owns the physical port being added to the vLAG */
                    domainId = DcmToolKit::getDomainIdFromMappedId(mappedId);

                    if(getMacFromDomainId(switchMacAddress, domainId) == WAVE_MESSAGE_SUCCESS) {
                        NSM_PLUG_DBG_S( string("switchMacAddress : ") + switchMacAddress + string ("domainid : ") + domainId);
                        pNsmGlobalAddPhyIntfToPoIntfMessage->setPoLacpSystemId(switchMacAddress);
                        pNsmGlobalAddPhyIntfToPoIntfMessage->setPoLacpSystemIdDomainId(domainId);
                    }
                }
                pNsmGlobalAddPhyIntfToPoIntfMessage->setPoCmdCode(NSMUPDATE_PO_INTF);

            } else {

                NSM_PLUG_DBG_S( "Physical ports already assigned to port-channel");
                /*   - Validation PhaSE
                 *     if( MO_poMode(on) == inputMode(on))
                 *          error
                 *     if( Mo_poType(x) == inputType(x)
                 *         error
                 */

                if((pPoIntfManagedObject->getPoMode() == NSM_PO_MODE_ON ) || (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoMode() == NSM_PO_MODE_ON) ) {
                    if(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoMode() != pPoIntfManagedObject->getPoMode()) {
                        NSM_PLUG_DBG_ERR( "po mode mismatch with existing entry..");
                        status = WRC_NSM_LACP_ERROR_TRUNK_TYPE_MISMATCH;
                        if (poResults) {
                            pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*poResults);
                            poResults->clear();
                            delete poResults;
                        }
                        if (pResults) {
                            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                        }
                        pDceLinearSeqContext->executeNextStep(status);
                        return;
                    }
                }

                if(pPoIntfManagedObject->getPoType() != pNsmGlobalAddPhyIntfToPoIntfMessage->getPoType()) {
                    NSM_PLUG_DBG_ERR( "po type mismatch with existing entry..");
                    status = WRC_NSM_LACP_ERROR_TRUNK_TYPE_MISMATCH;
                    if (poResults) {
                        pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*poResults);
                        poResults->clear();
                        delete poResults;
                    }
                    if (pResults) {
                        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                    }
                    pDceLinearSeqContext->executeNextStep(status);
                    return;
                }
                /*          From the MO get systemid , mac address
                 *          mode and type from inputMsg
                 *
                 *          - update to channel-group Message (systemid, mac address)
                 */

                if(WAVE_PERSISTENCE_CLUSTER_ENABLED == vcs_status) {
                    //UI32 resultIter = 0;
                    //UI32 currMappedId = 0;
                    //PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;


                    NSM_PLUG_DBG_S( string("switchMacAddress : ") + pPoIntfManagedObject->getPoLacpSystemIdDomainId() + string ("domainid : ") + pPoIntfManagedObject->getPoLacpSystemId());
                    pNsmGlobalAddPhyIntfToPoIntfMessage->setPoLacpSystemIdDomainId(pPoIntfManagedObject->getPoLacpSystemIdDomainId());
                    pNsmGlobalAddPhyIntfToPoIntfMessage->setPoLacpSystemId(pPoIntfManagedObject->getPoLacpSystemId());

	   /* Replay config iff all member ports are ISL. Even atleast one member
		* port is EDGE, donot replay the config*/
	   //Get no.of edge member ports of PO of this location ID
		count =	NsmUtils::queryPoEdgeMembersCountForLocationId(
										poObjectId, 
										NsmUtils::getLocationId(pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName()));
                    NSM_PLUG_DBG_S( string("PO edge member count : ") +count + string ("domainid : ") + pPoIntfManagedObject->getPoLacpSystemId());
		if(count > 0)
		{
			//Don't replay PO config
        	pNsmGlobalAddPhyIntfToPoIntfMessage->setReplayPoConfigs(false);
            /* No Operation on MO */    
            pNsmGlobalAddPhyIntfToPoIntfMessage->setPoCmdCode(0);
		}
#if 0                             
                    while (resultIter != pResults->size()) {
                        pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[resultIter]);
                        if(pPhyIntfLocalManagedObject->getPoId() == poObjectId)
                        {
                            currMappedId = (UI32) NsmUtils::getMappedIdFromIfName(pPhyIntfLocalManagedObject->getIfName());
                            if(mappedId == currMappedId)/*The backend of this node already knows about this port channel. So do not create
                                                          create or replay PO to the backend*/
                                pNsmGlobalAddPhyIntfToPoIntfMessage->setReplayPoConfigs(false);
                                /* No Operation on MO */    
                                pNsmGlobalAddPhyIntfToPoIntfMessage->setPoCmdCode(0);

                            break;           

                        }
                        resultIter++;
                    }
#endif
                }

            }
            if (pResults) {
            	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }

        }
        WaveSendToClusterContext    *send_ctx_p;
        NsmAddPhyIntfToPoIntfMessage *m = new NsmAddPhyIntfToPoIntfMessage();
        //"tm","tt",1,"testing");
        m->setIfType (pNsmGlobalAddPhyIntfToPoIntfMessage->getIfType());
        

        m->setIfName (pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName());
        m->setPoName (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName());
        m->setPoMode (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoMode());
        m->setPoType (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoType());
        m->setPoLacpSystemId (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoLacpSystemId());
        m->setPoLacpSystemIdDomainId (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoLacpSystemIdDomainId());
		m->setReplayPoConfigs(pNsmGlobalAddPhyIntfToPoIntfMessage->getReplayPoConfigs());
		m->setCmdCode(NSMUPDATE_PHY_ADD_CHANNELGROUP_CMD);
    /* TR 370798 - ELD */
        if((WAVE_PERSISTENCE_CLUSTER_ENABLED == vcs_status)
                &&
            (pNsmGlobalAddPhyIntfToPoIntfMessage->getReplayPoConfigs())) {
            m->setPrio(prio);
            NSM_PLUG_INFO_S("PoIntfManagedObject: PRIO is " + m->getPrio());
            m->setVlanidList(vlanIdVector);
        }
    /* TR 370798 - ELD */

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalAddToPoCallback),
			pDceLinearSeqContext);

		prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);

		if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
		{
			mappedId = (UI32) NsmUtils::getMappedIdFromIfName(pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName());
			locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
			vector <LocationId> locationIdVector;
			locationIdVector.push_back(locationId);

            NSM_PLUG_DBG_S( string("Send to target location ::") + locationId);
			send_ctx_p->setLocationsToSendToForPhase1(locationIdVector);
		}

        NSM_PLUG_INFO_S("Sending to local nsm plugin ");
		sendToWaveCluster(send_ctx_p);

        pNsmGlobalAddPhyIntfToPoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		
		if(poResults)
		{
			pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*poResults);
//            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(poResults);
			poResults->clear();
			delete poResults;
		}
        return;
    }

    void IntfConfGlobalWorker::returnSucceededStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        trace (TRACE_LEVEL_DEVEL, "IntfConfGlobalWorker::returnSucceededStep : Entering ...");

        PrismMessage             *pPrismMessage             = pPrismLinearSequencerContext->getPPrismMessage ();
        PrismAsynchronousContext *pPrismAsynchronousContext = pPrismLinearSequencerContext->getPPrismAsynchronousContext ();
        ResourceId                completionStatus          = pPrismLinearSequencerContext->getCompletionStatus ();

		if (true == (pPrismLinearSequencerContext->getIsHoldAllRequested ()))
		{
			pPrismLinearSequencerContext->unholdAll ();
		}

        if (NULL != pPrismMessage)
        {
            pPrismMessage->setCompletionStatus (completionStatus);
        }
        else if (NULL != pPrismAsynchronousContext)
        {
            pPrismAsynchronousContext->setCompletionStatus (completionStatus);
            pPrismAsynchronousContext->callback ();
        }
        return;
    }

    void IntfConfGlobalWorker::addPhyIntfToPoIntfMOStep (DceLinearSeqContext *pDceLinearSeqContext)
    {
        NSM_PLUG_DBG("Entering ...");
        
        ResourceId status;
        
        NsmGlobalAddPhyIntfToPoIntfMessage *pNsmGlobalAddPhyIntfToPoIntfMessage = dynamic_cast<NsmGlobalAddPhyIntfToPoIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

        NSM_PLUG_INFO_S(string("Channel-group Cmd  Details  phyName,poName : ") + pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName() + string(",")+ pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName() + string(" poMode,poType : ") + pNsmGlobalAddPhyIntfToPoIntfMessage->getPoMode() + string(",") + pNsmGlobalAddPhyIntfToPoIntfMessage->getPoType() + string(" domainid,systemmac :") + pNsmGlobalAddPhyIntfToPoIntfMessage->getPoLacpSystemId() + string(",") + pNsmGlobalAddPhyIntfToPoIntfMessage->getPoLacpSystemIdDomainId()); 

        if(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoCmdCode() == NSMCREATE_PO_INTF)
        {   

            NSM_PLUG_INFO_S(string("Creating PoIntf through channel-group cmd phyName,poName : ") + pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName() + string(",")+ pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName() );
            
            NsmGlobalCreatePoIntfMessage *pNsmGlobalCreatePoIntfMessage = NULL;

            pNsmGlobalCreatePoIntfMessage = new NsmGlobalCreatePoIntfMessage ();
            pNsmGlobalCreatePoIntfMessage->setPoId(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName());
            pNsmGlobalCreatePoIntfMessage->setPoName(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName());
            pNsmGlobalCreatePoIntfMessage->setDesc("");
            pNsmGlobalCreatePoIntfMessage->setPoMode(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoMode());
            pNsmGlobalCreatePoIntfMessage->setPoType(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoType());
            pNsmGlobalCreatePoIntfMessage->setPoLacpSystemId(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoLacpSystemId());
            pNsmGlobalCreatePoIntfMessage->setPoLacpSystemIdDomainId(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoLacpSystemIdDomainId());
            pNsmGlobalCreatePoIntfMessage->setSendToLocal(true);

            if ( createPoIntfMO(pNsmGlobalCreatePoIntfMessage,pDceLinearSeqContext) != WAVE_MESSAGE_SUCCESS)
            {
                NSM_PLUG_DBG_ERR_S(string("Failed : Creating PoIntf through channel-group cmd phyName,poName : ") + pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName() + string(",")+ pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName() );
                pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_ERROR);
                return;
            }

            pNsmGlobalAddPhyIntfToPoIntfMessage->setPoRef(pNsmGlobalCreatePoIntfMessage->getPoRef());

            delete pNsmGlobalCreatePoIntfMessage;
        }
        else if (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoCmdCode() == NSMUPDATE_PO_INTF)
        {

            NSM_PLUG_INFO_S(string("Updating PoIntf through channel-group cmd  phyName,poName : ") + pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName() + string(",")+ pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName() );
            NsmGlobalUpdatePoIntfMessage *updateMsg = NULL;

            updateMsg = new NsmGlobalUpdatePoIntfMessage ();
            updateMsg->setPoId(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName());
            updateMsg->setPoName(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName());
            updateMsg->setDesc(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName());
            updateMsg->setPoMode(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoMode());
            updateMsg->setPoType(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoType());
            updateMsg->setPoLacpSystemId(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoLacpSystemId());
            updateMsg->setPoLacpSystemIdDomainId(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoLacpSystemIdDomainId());
            updateMsg->setCmdCode(NSMUPDATE_PO_CHANNELGROUP_FIRSTMEMBER);
            
            if( updatePoIntfMO(updateMsg,pDceLinearSeqContext) != WAVE_MESSAGE_SUCCESS)
            {
                NSM_PLUG_DBG_ERR_S(string("Failed : Updating PoIntf through channel-group cmd phyName,poName : ") + pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName() + string(",")+ pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName() );
                pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_ERROR);
                return;
            }

            pNsmGlobalAddPhyIntfToPoIntfMessage->setPoRef(updateMsg->getPoRef());
            
            delete updateMsg;
        }

        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = pNsmGlobalAddPhyIntfToPoIntfMessage->getPhyRef();
        if (pPhyIntfLocalManagedObject == NULL) {
            prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);
        } else {
    
            /*   The below PhyIntfMO updation code moved to Nsm Local plugin 
            NSM_PLUG_INFO_S(string("Updating PhyIntf through channel-group cmd  phyName,poName : ") + pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName() + string(",")+ pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName() );
        
            
            ObjectId poObjectId;
 
            updateWaveManagedObject (pPhyIntfLocalManagedObject);
			
            prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);

            if(pNsmGlobalAddPhyIntfToPoIntfMessage->getPoRef() == NULL)
            {
                pDceLinearSeqContext->addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);
                pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_ERROR);
                return;
            }

            poObjectId = (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoRef())->getObjectId(); 

            pPhyIntfLocalManagedObject->setPoId(poObjectId);
            pPhyIntfLocalManagedObject->setPoMode (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoMode());
            pPhyIntfLocalManagedObject->setPoType (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoType());

			// set to default timeout for dynamic trunk 
			if (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoMode() != NSM_PO_MODE_ON) {
				if (pNsmGlobalAddPhyIntfToPoIntfMessage->getPoType() == NSM_PO_TYPE_STANDARD) {
					pPhyIntfLocalManagedObject->setLacpTimeout(LACP_TIMEOUT_LONG);
				} else {
					pPhyIntfLocalManagedObject->setLacpTimeout(LACP_TIMEOUT_SHORT);
				}
			}

			pPhyIntfLocalManagedObject->setFlowControlRx(QOS_DEFAULT_FLOWCONTROL);
			pPhyIntfLocalManagedObject->setFlowControlTx(QOS_DEFAULT_FLOWCONTROL);
            pPhyIntfLocalManagedObject->setQosTrust(QOS_DEFAULT_QOS_TRUST);
			pPhyIntfLocalManagedObject->setQosDscpTrust(QOS_DEFAULT_QOS_TRUST);
            pPhyIntfLocalManagedObject->setDefaultCos(QOS_DEFAULT_COS_VALUE);
            pPhyIntfLocalManagedObject->setQosCosMap(ObjectId::NullObjectId);
            pPhyIntfLocalManagedObject->setQosTrafficClassMap(ObjectId::NullObjectId);
			pPhyIntfLocalManagedObject->setQosDscpMap(ObjectId::NullObjectId);
            pPhyIntfLocalManagedObject->setQosDscpTrafficClassMap(ObjectId::NullObjectId);
			pPhyIntfLocalManagedObject->setQosCosMap(ObjectId::NullObjectId);
            pPhyIntfLocalManagedObject->deletePfcAll();
            pPhyIntfLocalManagedObject->deleteCosProfileAll();
			pPhyIntfLocalManagedObject->deleteCosTailDropAll();
	        // pPhyIntfLocalManagedObject->setCeeMap(ObjectId::NullObjectId);
 
            */    
            NSM_PLUG_INFO_S(string("Updating PhyIntf through channel-group cmd  end... phyName,poName : ") + pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName() + string(",")+ pNsmGlobalAddPhyIntfToPoIntfMessage->getPoName() );
        }

        status = pNsmGlobalAddPhyIntfToPoIntfMessage->getCompletionStatus ();
        pDceLinearSeqContext->executeNextStep(status);
    } 

    void  IntfConfGlobalWorker::NsmGlobalAddPhyIntfToPoIntfMessageHandler( NsmGlobalAddPhyIntfToPoIntfMessage *pNsmGlobalAddPhyIntfToPoIntfMessage)
    {
        NSM_PLUG_DBG("Entering ...");
        PrismLinearSequencerStep sequencerSteps[] =
        {

            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::validatePhyIntfToPoIntfStep),
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::addPhyIntfToPoIntfStep),
			reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::addPhyIntfToPoIntfMOStep),
		 	reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::addPhyIntfToPoIntfLocalDBCommitStep),
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };

        DceLinearSeqContext *pDceLinearSeqContext = new DceLinearSeqContext (pNsmGlobalAddPhyIntfToPoIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pDceLinearSeqContext->holdAll();
        pDceLinearSeqContext->start();
    }

	/*SENTHIL
	failover( locationid)
	{

		Query the po intf based of (sysemid == locationid)
		refPoList:: Of pointf MOs

		Query the physical interface based on locationid();
			Refer Nsm/Local/IntfLocalObjectMabager.cpp :: failover method();
		if (physical interface has poid configured)
			Addinto polist;
		refPhyPoList = of PhyIntfMos


		finalPoList ( refPoList == refPhyPoList)

		Loop the final PoList
		{
			NamGlobalUpdateMsg = new...

			Query PhyintfMo with condition ( !locationId Matched) && finalPoList[i] == physicalIntfMO->poId)
			if( null)
			{
				Delete the systemid  & lacpsystemid
					- Assign the default values 0
 				pNsmGlobaliUpdatePoIntfMessage->setPoLacpSystemIdDomainId(0);
				pNsmGlobalUpdatePoIntfMessage->setLacpSystemId(0);

			} else {
				Update the systemid & lacpsystemid from physical IntfMo[0]

				- Get mac addres and rbridgeid based on ifname pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName()

				mappedId = (UI32) NsmUtils::getMappedIdFromIfName(pNsmGlobalAddPhyIntfToPoIntfMessage->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
				pNsmGlobaliUpdatePoIntfMessage->setPoLacpSystemIdDomainId(locationId);

				mac = GetMacFromLocationId(locationID);
				pNsmGlobalUpdatePoIntfMessage->setLacpSystemId(mac);

			}

			NsmGlobalUpdatePoIntfMessage->setCmdCode = DELETE_SW / REASSIGN SYSTEMID				

			createliner sequence step for update (context)
				- Follow the same for create above....

		}		
		
	}
	*/

	void 
	IntfConfGlobalWorker::sendPoConfigToLocal (PrismLinearSequencerContext *pPrismLinearSequencerContext, const string& ifName, LocationId& locationId)
	{
        UI32 po_id;
        string poId = ifName;
        //ResourceId  status   = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        po_id = strtoul(poId.c_str (), NULL, 0);
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&po_id,"id"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        PoIntfManagedObject *pPoIntfManagedObject = NULL;
        trace(TRACE_LEVEL_INFO, "Entering sendPoConfigToLocal: ");

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
        		trace(TRACE_LEVEL_INFO, "Entering sendPoConfigToLocal: PoIntfManagedObject not found ");
                //status = WAVE_MESSAGE_ERROR;
				return;
            }else if (1 == numberOfResults) {
        		trace(TRACE_LEVEL_INFO, "Entering sendPoConfigToLocal: Found PoIntfManagedObject ");
                pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
                //status = WAVE_MESSAGE_SUCCESS;
            } else {
        		trace(TRACE_LEVEL_INFO, "Entering sendPoConfigToLocal: Found more than 1 PoIntfManagedObject ");
                //status = WAVE_MESSAGE_ERROR;
				return;
            }
        }


        if (pPoIntfManagedObject == NULL) {
        	trace(TRACE_LEVEL_ERROR, "PoIntfManagedObject not found, returning ");
            return;
        } 
		else 
		{
			vector <LocationId> locationIdVector;
			locationIdVector.push_back(locationId);


			NsmUpdatePoIntfSPMessage *m1 = new NsmUpdatePoIntfSPMessage();
			m1->setPoId (ifName);
			m1->setCmdCode (NSMUPDATE_PO_DESC);
            m1->setDesc (pPoIntfManagedObject->getDesc());

        	WaveSendToClusterContext    *send_ctx_p1;
			send_ctx_p1 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>
				(&IntfConfGlobalWorker::IntfConfGlobalReplayConfigCallback),pPrismLinearSequencerContext);
			prismAssert(NULL != send_ctx_p1, __FILE__, __LINE__);
			send_ctx_p1->setPPrismMessageForPhase1(m1);

			if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
			{
				send_ctx_p1->setLocationsToSendToForPhase1(locationIdVector);
			}

			sendToWaveCluster(send_ctx_p1);

			NsmUpdatePoIntfSPMessage *m2 = new NsmUpdatePoIntfSPMessage();
			m2->setPoId (ifName);

			if(pPoIntfManagedObject->getStatus() == false)
				m2->setCmdCode (NSMUPDATE_PO_DELSTATUS);
			else
				m2->setCmdCode (NSMUPDATE_PO_STATUS);
			
        	WaveSendToClusterContext    *send_ctx_p2;
			send_ctx_p2 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>
				(&IntfConfGlobalWorker::IntfConfGlobalReplayConfigCallback),pPrismLinearSequencerContext);

			prismAssert(NULL != send_ctx_p2, __FILE__, __LINE__);
			send_ctx_p2->setPPrismMessageForPhase1(m2);

			if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
			{
				send_ctx_p2->setLocationsToSendToForPhase1(locationIdVector);
			}

			sendToWaveCluster(send_ctx_p2);

			NsmUpdatePoIntfSPMessage *m2a = new NsmUpdatePoIntfSPMessage();
			m2a->setPoId (ifName);
            m2a->setIgnoreSplit(pPoIntfManagedObject->getIgnoreSplit());

			if(pPoIntfManagedObject->getIgnoreSplit() == false) {
				m2a->setCmdCode (NSMUPDATE_PO_NO_IGNORE_SPLIT);
			}
			else {
				m2a->setCmdCode (NSMUPDATE_PO_IGNORE_SPLIT);
			}
			
        	WaveSendToClusterContext    *send_ctx_p2a;
			send_ctx_p2a = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>
				(&IntfConfGlobalWorker::IntfConfGlobalReplayConfigCallback),pPrismLinearSequencerContext);

			prismAssert(NULL != send_ctx_p2a, __FILE__, __LINE__);
			send_ctx_p2a->setPPrismMessageForPhase1(m2a);

			if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
			{
				send_ctx_p2a->setLocationsToSendToForPhase1(locationIdVector);
			}

			sendToWaveCluster(send_ctx_p2a);

			NsmUpdatePoIntfSPMessage *m3 = new NsmUpdatePoIntfSPMessage();
			m3->setPoId (ifName);
			m3->setCmdCode (NSMUPDATE_PO_MTU);
            m3->setMtu(pPoIntfManagedObject->getMtu());

        	WaveSendToClusterContext    *send_ctx_p3;
			send_ctx_p3 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>
				(&IntfConfGlobalWorker::IntfConfGlobalReplayConfigCallback),pPrismLinearSequencerContext);

			prismAssert(NULL != send_ctx_p3, __FILE__, __LINE__);
			send_ctx_p3->setPPrismMessageForPhase1(m3);

			if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
			{
				send_ctx_p3->setLocationsToSendToForPhase1(locationIdVector);
			}

			sendToWaveCluster(send_ctx_p3);

			NsmUpdatePoIntfSPMessage *m4 = new NsmUpdatePoIntfSPMessage();
			m4->setPoId (ifName);
			if(pPoIntfManagedObject->getSwModeFlag() == false)
				m4->setCmdCode(NSMUPDATE_PO_DELSW);
			else
				m4->setCmdCode (NSMUPDATE_PO_SW);

        	WaveSendToClusterContext    *send_ctx_p4;
			send_ctx_p4 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>
				(&IntfConfGlobalWorker::IntfConfGlobalReplayConfigCallback),pPrismLinearSequencerContext);

			prismAssert(NULL != send_ctx_p4, __FILE__, __LINE__);
			send_ctx_p4->setPPrismMessageForPhase1(m4);
			
			if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
			{
				send_ctx_p4->setLocationsToSendToForPhase1(locationIdVector);
			}

			sendToWaveCluster(send_ctx_p4);

			NsmUpdatePoIntfSPMessage *m5 = new NsmUpdatePoIntfSPMessage();
			m5->setPoId (ifName);
			m5->setCmdCode (NSMUPDATE_PO_MODE);
            m5->setSwMode(pPoIntfManagedObject->getSwMode());

        	WaveSendToClusterContext    *send_ctx_p5;
			send_ctx_p5 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>
				(&IntfConfGlobalWorker::IntfConfGlobalReplayConfigCallback),pPrismLinearSequencerContext);

			prismAssert(NULL != send_ctx_p5, __FILE__, __LINE__);
			send_ctx_p5->setPPrismMessageForPhase1(m5);

			if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
			{
				send_ctx_p5->setLocationsToSendToForPhase1(locationIdVector);
			}

			sendToWaveCluster(send_ctx_p5);

			NsmUpdatePoIntfSPMessage *m6 = new NsmUpdatePoIntfSPMessage();
			m6->setPoId (ifName);
			if(pPoIntfManagedObject->getIsAllowedVlanAll() == false)
				m6->setCmdCode (NSMUPDATE_PO_SW_TRUNK_VLAN_NONE);
			else
				m6->setCmdCode (NSMUPDATE_PO_SW_TRUNK_VLAN_ALL);
			
        	WaveSendToClusterContext    *send_ctx_p6;
			send_ctx_p6 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>
				(&IntfConfGlobalWorker::IntfConfGlobalReplayConfigCallback),pPrismLinearSequencerContext);

			prismAssert(NULL != send_ctx_p6, __FILE__, __LINE__);
			send_ctx_p6->setPPrismMessageForPhase1(m6);

			if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
			{
				send_ctx_p6->setLocationsToSendToForPhase1(locationIdVector);
			}

			sendToWaveCluster(send_ctx_p6);

			trace(TRACE_LEVEL_INFO, "sendPoConfigToLocal: Sending NSMUPDATE_PO_RT_LACP_SYSTEM_ID");
			/* Send updated LACP System Id to switches */
			NsmUpdatePoIntfSPMessage *m7 = new NsmUpdatePoIntfSPMessage();
			m7->setPoId (ifName);
			m7->setCmdCode (NSMUPDATE_PO_RT_LACP_SYSTEM_ID);
                        m7->setPoLacpSystemId (pPoIntfManagedObject->getPoLacpSystemId());

        	        WaveSendToClusterContext    *send_ctx_p7;
			send_ctx_p7 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>
				(&IntfConfGlobalWorker::IntfConfGlobalReplayConfigCallback),pPrismLinearSequencerContext);
			prismAssert(NULL != send_ctx_p7, __FILE__, __LINE__);
			send_ctx_p7->setPPrismMessageForPhase1(m7);

			if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
			{
				send_ctx_p7->setLocationsToSendToForPhase1(locationIdVector);
			}
			sendToWaveCluster(send_ctx_p7);
        }
        if (NULL != pResults)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}
    
    /* Code Ends Here */
	ResourceId IntfConfGlobalWorker:: getCosMapObjectIdByCosMapName(string cosMapName, ObjectId &cosMapObjectId, PrismLinearSequencerContext *pPrismLinearSequencerContext)
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
			if(pPrismLinearSequencerContext) {
				pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        	pResults->clear();
            delete pResults;
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
        }
																	
        return status;
	}

	ResourceId IntfConfGlobalWorker:: getTcMapObjectIdByTcMapName(string cosMapName, ObjectId &cosMapObjectId, PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		ResourceId  status   = WAVE_MESSAGE_ERROR;
		CosToTrafficClassMapManagedObject *pCosToTrafficClassMapManagedObject = NULL;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToTrafficClassMapManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&cosMapName,"cosToTrafficClassMapName"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (pResults && pResults->size() > 0) {
          pCosToTrafficClassMapManagedObject = dynamic_cast<CosToTrafficClassMapManagedObject*>((*pResults)[0]);
          cosMapObjectId = pCosToTrafficClassMapManagedObject->getObjectId();
            status = WAVE_MESSAGE_SUCCESS; 
		}
        if (pResults) {
			if(pPrismLinearSequencerContext) {
				pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        	pResults->clear();
            delete pResults;
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
        }
        return status;
	}

    ResourceId IntfConfGlobalWorker:: getCeeMapObjectIdByCeeMapName(string ceeMapName, ObjectId &ceeMapObjectId, PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        CeeMapManagedObject *pCeeMapManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ceeMapName,"ceeMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
        	pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResults)[0]);
        	ceeMapObjectId = pCeeMapManagedObject->getObjectId();
            status = WAVE_MESSAGE_SUCCESS;
        }
        if (pResults) {
			if(pPrismLinearSequencerContext) {
				pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            pResults->clear();
            delete pResults;
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
        }
        return status;
    }

	ResourceId IntfConfGlobalWorker::getDscpMapObjectIdByDscpMapName(string dscpMapName, ObjectId &dscpMapObjectId,
                                                            PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        DscpToDscpMutationMapManagedObject *pDscpToDscpMutationMapManagedObject = NULL;
        printf("PRINTING ---- querying database --\n");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToDscpMutationMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&dscpMapName,"dscpToDscpMutationMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
          pDscpToDscpMutationMapManagedObject = dynamic_cast<DscpToDscpMutationMapManagedObject*>((*pResults)[0]);
          dscpMapObjectId = pDscpToDscpMutationMapManagedObject->getObjectId();
          printf("PRINTING ------ successfully stored dscp map object id ---\n");
            status = WAVE_MESSAGE_SUCCESS;
        }
        if (pResults) {
            if(pPrismLinearSequencerContext)
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            pResults->clear();
            delete pResults;
        }
        return status;
    }

	ResourceId IntfConfGlobalWorker::getDscpTcMapObjectIdByDscpTcMapName(string dscpTcMapName, ObjectId &dscpTcMapObjectId,
                                                            PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        DscpToTrafficClassMapManagedObject *pDscpToTrafficClassMapManagedObject = NULL;
        printf("PRINTING ---- querying database --\n");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt( DscpToTrafficClassMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&dscpTcMapName,"dscpToTrafficClassMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
          pDscpToTrafficClassMapManagedObject = dynamic_cast<DscpToTrafficClassMapManagedObject*>((*pResults)[0]);
          dscpTcMapObjectId = pDscpToTrafficClassMapManagedObject->getObjectId();
          printf("PRINTING ------ successfully stored dscp to tc map object id ---\n");
            status = WAVE_MESSAGE_SUCCESS;
        }
        if (pResults) {
            if(pPrismLinearSequencerContext)
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            pResults->clear();
            delete pResults;
        }
        return status;
    }

	ResourceId IntfConfGlobalWorker:: getDscpCosMapObjectIdByDscpCosMapName(string dscpCosMapName, ObjectId &dscpCosMapObjectId,
                                                            PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        DscpToCosMapManagedObject *pDscpToCosMapManagedObject = NULL;
        printf("PRINTING ---- querying database --\n");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToCosMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&dscpCosMapName,"dscpToCosMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
          pDscpToCosMapManagedObject = dynamic_cast<DscpToCosMapManagedObject*>((*pResults)[0]);
          dscpCosMapObjectId = pDscpToCosMapManagedObject->getObjectId();
          printf("PRINTING ------ successfully stored cee map object id ---\n");
            status = WAVE_MESSAGE_SUCCESS;
        }
        if (pResults) {
            if(pPrismLinearSequencerContext)
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            pResults->clear();
            delete pResults;
        }
        return status;
    }

    void  IntfConfGlobalWorker::NsmGlobalCreatePoIntfMessageHandler( NsmGlobalCreatePoIntfMessage *pNsmGlobalCreatePoIntfMessage)
    {

		pNsmGlobalCreatePoIntfMessage->setSendToLocal(false);
        PrismLinearSequencerStep sequencerSteps[] =
        {
        		// Validations and Queries should go here
        		reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::validateCreatePoIntfStep),

// Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::createPoIntfStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),

// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::createPoIntfMOStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNsmGlobalCreatePoIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }

    /* Code Starts Here */
    void IntfConfGlobalWorker::createVlanIntfMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        ResourceId  status;
        NsmGlobalCreateVlanIntfMessage *pNsmGlobalCreateVlanIntfMessage = dynamic_cast<NsmGlobalCreateVlanIntfMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
        status =  createVlanIntfMO(pNsmGlobalCreateVlanIntfMessage, pPrismLinearSequencerContext);
        pPrismLinearSequencerContext->executeNextStep(status);
    }
    
    ResourceId IntfConfGlobalWorker::createVlanIntfMO (NsmGlobalCreateVlanIntfMessage *pNsmGlobalCreateVlanIntfMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext,bool removeImmediate)
    {
		bool vlanExists = false;
        UI32 vlanId = 0;
        vector<UI32> RangeVlanVector;

        /** check if the config request is for bulk vlan configuration **/
        if (pNsmGlobalCreateVlanIntfMessage->isBulkVlanConfig()) {
            string RangeVlanStr = pNsmGlobalCreateVlanIntfMessage->getRangeVlanStr();
            trace(TRACE_LEVEL_INFO, string("CreateVlanIntfMO: RangeVlanStr:(") + RangeVlanStr + string(")") );

            if (! RangeVlanStr.empty()) {
                /**
                 * Derive the final list of Vlans for which MOs needs to created. So first query for Vlans out of given
                 * range which exists in Db. Then subtract those Vlans from the given range.
                 */
                UI32Range givenVlanRange = UI32Range (RangeVlanStr);
                WaveManagedObjectSynchronousQueryContext vlanRangeQueryCtxt(VlanIntfManagedObject::getClassName());
                vlanRangeQueryCtxt.setAttributeRangeForQuery(new AttributeUI32Range(givenVlanRange, "id"));
                vector<WaveManagedObject *> *pResults = querySynchronously(&vlanRangeQueryCtxt);
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                if (pResults && pResults->size() > 0) {
                    vector<UI32> existingVlanIds;
                    for (UI32 i = 0; i < pResults->size(); i++) {
                        VlanIntfManagedObject *vlanObj = dynamic_cast<VlanIntfManagedObject *> ((*pResults)[i]);
                        existingVlanIds.push_back(vlanObj->getId());
                    }
                    string existingVlanRangeStr = UI32Range::getUI32RangeStringFromVector(existingVlanIds);
                    UI32Range existingVlanRange = UI32Range(existingVlanRangeStr);
                    trace(TRACE_LEVEL_INFO, string("CreateVlanIntfMO: existingVlanRangeStr:(") + existingVlanRangeStr + string(") vector size=") + existingVlanIds.size());

                    UI32Range finalRangeForMOCreate = givenVlanRange - existingVlanRange;
                    finalRangeForMOCreate.getUI32RangeVector(RangeVlanVector);
                    trace(TRACE_LEVEL_INFO, string("CreateVlanIntfMO: finalRangeForMOCreate:(") + finalRangeForMOCreate.toString() + string(") vector size=") + RangeVlanVector.size());
                } else {
                    givenVlanRange.getUI32RangeVector(RangeVlanVector);
                    trace(TRACE_LEVEL_INFO, string("CreateVlanIntfMO: givenVlanRange:(") + givenVlanRange.toString() + string(") vector size=") + RangeVlanVector.size());
                }
            } else {
                trace(TRACE_LEVEL_ERROR, string("CreateVlanIntfMO: For Bulk Vlan creation, there are NO vlans in Range provided"));
                return WAVE_MESSAGE_SUCCESS;
            }
        } else {
            vlanId = pNsmGlobalCreateVlanIntfMessage->getVlanId();
		vlanExists = ifVlanExistsByVlanId(vlanId,pPrismLinearSequencerContext);
		if(vlanExists){
            trace(TRACE_LEVEL_ERROR, string("CreateVlanIntfMO: Vlan Interface already exists ..") + pNsmGlobalCreateVlanIntfMessage->getVlanId());
            return WAVE_MESSAGE_SUCCESS;
        }		

            RangeVlanVector.push_back(vlanId);
        }

        /*
         * Moved the Igmp Global MO query out of loop to avoid execution of same query multiple times
         */
        bool g_enable = false;
        UI32 count = 0;

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IgmpsGlobalConfigManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeBool(true, "enable"));
        querySynchronouslyForCount(&syncQueryCtxt, count);
        
        if(count == 1)
        {
            g_enable = true;
        }
    
        for (UI32 i = 0; i < RangeVlanVector.size(); i++)
        {
            vlanId = RangeVlanVector[i];
            string vlanStr = string("") + vlanId;
        
            const UI32 t_id = vlanId;
            UI64 ifIndex = 0; // TBDDCMOSS : ifindex_create_svi_idx(0, vlanId);
            
            VlanIntfManagedObject *pVlanIntfManagedObject = new VlanIntfManagedObject(dynamic_cast<NsmGlobalObjectManager  *>(getPWaveObjectManager()),t_id, pNsmGlobalCreateVlanIntfMessage->getType(), vlanStr, ifIndex,pNsmGlobalCreateVlanIntfMessage->getDesc(), true, NSM_MTU_DEF_VAL);

            prismAssert (NULL != pVlanIntfManagedObject, __FILE__, __LINE__);

            NSM_PLUG_DBG_S(string("Create Vlan ") + vlanId + string(" if index = ") + ifIndex);

            //pVlanIntfManagedObject->setIngressMacAclPolicy(ObjectId::NullObjectId);
            /* Initialize IGMPS MO */
       
        IgmpsVlanIntfConfManagedObject* m_pIgmpsVlanIntfConfManagedObject =
            new IgmpsVlanIntfConfManagedObject (dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));

        m_pIgmpsVlanIntfConfManagedObject->setLmqi (1000);
        m_pIgmpsVlanIntfConfManagedObject->setQmrt (10);
        m_pIgmpsVlanIntfConfManagedObject->setEnable (g_enable);
        m_pIgmpsVlanIntfConfManagedObject->setFastLeave (false);
        m_pIgmpsVlanIntfConfManagedObject->setQueryInterval (125);
        m_pIgmpsVlanIntfConfManagedObject->setQuerierEnable (false);
        m_pIgmpsVlanIntfConfManagedObject->setMrouterTimeout (300);
        m_pIgmpsVlanIntfConfManagedObject->setVlanId(vlanId);

        pVlanIntfManagedObject->setIgmpsVlanIntfConfig (m_pIgmpsVlanIntfConfManagedObject);
        trace(TRACE_LEVEL_DEVEL, string("Exit IntfConfGlobalWorker::createVlanIntfMO () ...") + pNsmGlobalCreateVlanIntfMessage->getCompletionStatus ());

        if (pPrismLinearSequencerContext)
        {
        	pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pVlanIntfManagedObject);    
        }

        /*
		if(pPrismLinearSequencerContext)
		{
        	pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(m_pIgmpsVlanIntfConfManagedObject);    
		}
        */
        }  /*  End of Vlan Vector Iterator for loop */
        return pNsmGlobalCreateVlanIntfMessage->getCompletionStatus ();
    }

    void IntfConfGlobalWorker::createVlanIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {

        NsmGlobalCreateVlanIntfMessage *pNsmGlobalCreateVlanIntfMessage = dynamic_cast<NsmGlobalCreateVlanIntfMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;

        /** check if the config request is for bulk vlan configuration **/
        if (pNsmGlobalCreateVlanIntfMessage->isBulkVlanConfig()) {
			string vlanRangeStr = pNsmGlobalCreateVlanIntfMessage->getRangeVlanStr();
			UI32Range vlanRange(vlanRangeStr);
			vector<UI32> vlanRangeVector;

			if (!vlanRangeStr.empty()) {
				vlanRange.getUI32RangeVector(vlanRangeVector);
			}
			tracePrintf(TRACE_LEVEL_INFO, "IntfConfGlobalWorker::createVlanIntfStep:vlanRangeStr:%s, size:%d.\n", vlanRangeStr.c_str(), vlanRangeVector.size());//TODO:Venkat. To be removed before Checking-in.

			if(ProvisioningUtil::isGvlanEnabled() == GVLAN_STATE_DISABLE) {
				for(UI32 vlanVectorIdx = 0; vlanVectorIdx < vlanRangeVector.size(); vlanVectorIdx++) {
					if(vlanRangeVector[vlanVectorIdx] >= VLAN_ID_4091) {
						trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::createVlanIntfStep::Cannot create vlans > 4091, when GVlan is disabled."));
						status = WRC_NSM_GVLAN_NOT_CAPABLE_FABRIC;
						break;
					}
				}
			}
			else {
				for(UI32 vlanVectorIdx = 0; vlanVectorIdx < vlanRangeVector.size(); vlanVectorIdx++) {
					if((vlanRangeVector[vlanVectorIdx] >= VLAN_ID_4091) && (vlanRangeVector[vlanVectorIdx] < MAX_DOT1Q_VLAN_ID)) {
						status = WRC_NSM_CANNOT_CREATE_VLAN;
						trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::createVlanIntfStep::Cannot create vlans from 4091 to 4095."));
						break;
					}
				}
			}

			if(status != WAVE_MESSAGE_SUCCESS) {
	           	pNsmGlobalCreateVlanIntfMessage->setCompletionStatus(status);
    	        pPrismLinearSequencerContext->executeNextStep(status);
				return;
			}

            createBulkVlanIntfStep (pPrismLinearSequencerContext);
            return;
        }

        // Check the given Vlan Id, if zero or greater than max vlan allowed.
        UI32 vlanId = pNsmGlobalCreateVlanIntfMessage->getVlanId();
        if (vlanId == 0 || vlanId > MAX_ALLOWED_VLAN_ID) {
            tracePrintf (TRACE_LEVEL_ERROR, "Invalid Vlan Id: %d", vlanId);
            pNsmGlobalCreateVlanIntfMessage->setCompletionStatus(WRC_NSM_API_ERR_INVALID_VLAN);
            pPrismLinearSequencerContext->executeNextStep(WRC_NSM_API_ERR_INVALID_VLAN);
            return;
        }

        if (ifVlanExistsByVlanId (vlanId, pPrismLinearSequencerContext)) {
            trace (TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::createVlanIntfStep. Vlan already exists"));
            pNsmGlobalCreateVlanIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }

		if(vlanId >= VLAN_ID_4091) {
			//1. When GVlan is disabled, creation of vlans >= 4091 should not be allowed.
			//2. When GVlan is enabled, creation of vlans between 4091 and 4096 should not be allowed.
			//TODO: Need to get a clear picture on the lower limit, 4091 or 4090.
			if(ProvisioningUtil::isGvlanEnabled() == GVLAN_STATE_DISABLE) {
				trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::createVlanIntfStep::Cannot create vlan >= 4091, when GVlan is disabled."));
				status = WRC_NSM_GVLAN_NOT_CAPABLE_FABRIC;
			} else if(vlanId <= MAX_DOT1Q_VLAN_ID) {
				trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::createVlanIntfStep::Cannot create vlan between 4091 and 4095."));
				status = WRC_NSM_CANNOT_CREATE_VLAN;
			}

			if(status != WAVE_MESSAGE_SUCCESS) {
	           	pNsmGlobalCreateVlanIntfMessage->setCompletionStatus(status);
    	        pPrismLinearSequencerContext->executeNextStep(status);
				return;
			}
		}

        WaveSendToClusterContext    *send_ctx_p;
        NsmCreateVlanIntfSPMessage *m = new NsmCreateVlanIntfSPMessage();
        //"tm","tt",1,"testing");

        m->setVlanId (pNsmGlobalCreateVlanIntfMessage->getVlanId());
        m->setVlanName (pNsmGlobalCreateVlanIntfMessage->getVlanName());
        m->setVlanIfIndex (pNsmGlobalCreateVlanIntfMessage->getVlanIfIndex());
        m->setDesc (pNsmGlobalCreateVlanIntfMessage->getDesc());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback),
			pPrismLinearSequencerContext);
		prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);

        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);

        pNsmGlobalCreateVlanIntfMessage->setVlanIfIndex(m->getVlanIfIndex());
        
        
        pNsmGlobalCreateVlanIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        trace(TRACE_LEVEL_DEVEL, string("Exit IntfConfGlobalWorker::createVlanIntfStep () ...") + WAVE_MESSAGE_SUCCESS );
        return;
    }

    void IntfConfGlobalWorker::createBulkVlanIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        NsmGlobalCreateVlanIntfMessage *pNsmGlobalCreateVlanIntfMessage = dynamic_cast<NsmGlobalCreateVlanIntfMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
        string vlanRangeStr = pNsmGlobalCreateVlanIntfMessage->getRangeVlanStr();
        if (! vlanRangeStr.empty()) {
            trace(TRACE_LEVEL_INFO, string("Enter IntfConfGlobalWorker::createBulkVlanIntfStep RangeVlanStr:(") + vlanRangeStr + string(")"));
            WaveSendToClusterContext    *send_ctx_p;
            NsmCreateVlanIntfSPMessage *m = new NsmCreateVlanIntfSPMessage();
            m->setVlanName (pNsmGlobalCreateVlanIntfMessage->getVlanName());
            m->setRangeVlanStr(pNsmGlobalCreateVlanIntfMessage->getRangeVlanStr());
            m->setIsBulkVlanConfig(true);

            send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToVlanCreateClusterCallback),
                pPrismLinearSequencerContext);
            prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
            send_ctx_p->setPPrismMessageForPhase1(m);

            send_ctx_p->setPartialSuccessFlag(true);
            send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
            sendToWaveCluster(send_ctx_p);

            pNsmGlobalCreateVlanIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

            return;
        }
    }

   void IntfConfGlobalWorker::NsmReservedVlanSendtoWaveClusterStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
   {
      trace(TRACE_LEVEL_DEVEL, string("Entering NsmReservedVlanSendtoWaveClusterStep ()."));

      NsmGlobalReservedVlanMessage *pNsmGlobalReservedVlanMessage = dynamic_cast<NsmGlobalReservedVlanMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
      prismAssert(NULL != pNsmGlobalReservedVlanMessage, __FILE__, __LINE__);

      NsmLocalReservedVlanMessage  *loc_msg_p = new NsmLocalReservedVlanMessage();
      prismAssert(NULL != loc_msg_p, __FILE__, __LINE__);
      
      loc_msg_p->setStart(pNsmGlobalReservedVlanMessage->getStart());
      loc_msg_p->setEnd(pNsmGlobalReservedVlanMessage->getEnd());
      loc_msg_p->setCmdCode(pNsmGlobalReservedVlanMessage->getCmdCode());

      WaveSendToClusterContext   *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback), pPrismLinearSequencerContext);
      prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
      
      send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
      send_ctx_p->setPartialSuccessFlag(true);
      send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
      
      sendToWaveCluster(send_ctx_p);
      pNsmGlobalReservedVlanMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

      return;
    }


    /* Code Ends Here */

	void  IntfConfGlobalWorker::NsmGlobalCreateVlanIntfMessageHandler( NsmGlobalCreateVlanIntfMessage *pNsmGlobalCreateVlanIntfMessage)
    {
        /* Sending Msg from Global Plugin to Local Plugin */
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::createVlanIntfStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::createVlanIntfMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };
        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNsmGlobalCreateVlanIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }

    void IntfConfGlobalWorker::deletePoIntfLocalDBUpdateStep (DceLinearSeqContext *pDceLinearSeqContext) {
        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfGlobalWorker::deletePoIntfLocalDBUpdateStep... ");
        // Send message to all local plugins to update DB only.
        NsmGlobalDeletePoIntfMessage *pNsmGlobalDeletePoIntfMessage = dynamic_cast<NsmGlobalDeletePoIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());
		//PoIntfManagedObject* pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>(pDceLinearSeqContext->getCache()->getWaveManagedObject(PoIntfManagedObject::getClassName()));

        WaveSendToClusterContext    *send_ctx_p;
        NsmDeletePoIntfSPMessage *m = new NsmDeletePoIntfSPMessage();
        //ObjectId poObjectId = pPoIntfManagedObject->getObjectId();

        m->setPoId (pNsmGlobalDeletePoIntfMessage->getPoId());
        m->setNeedSurrogateSupportFlag(true);
        m->setDBUpdateRequest(true);
        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback),
            pDceLinearSeqContext);
        prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);

        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
        sendToWaveCluster(send_ctx_p);

        pNsmGlobalDeletePoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        return;
    }

void IntfConfGlobalWorker::deletePoIntfMOStep (DceLinearSeqContext *pDceLinearSeqContext)
    {
        NsmGlobalDeletePoIntfMessage *pNsmGlobalDeletePoIntfMessage = 
				dynamic_cast<NsmGlobalDeletePoIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

		PoIntfManagedObject* pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>(pDceLinearSeqContext->getCache()->getWaveManagedObject(PoIntfManagedObject::getClassName()));
        if (pPoIntfManagedObject == NULL) {
            pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
        }

        /** This is an extra code to delete PO reference from PhyIntfMO. It is already taken care in previous step **/
        // Commenting this part of code
        /*
        ObjectId poObjectId = pPoIntfManagedObject->getObjectId();
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		WaveManagedObjectSynchronousQueryContext remPoFromPhysyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		remPoFromPhysyncQueryCtxt.addAndAttribute (new AttributeObjectId(poObjectId,"poId"));
		vector<WaveManagedObject *> *remPoFromPhypResults = querySynchronously (&remPoFromPhysyncQueryCtxt);

		NsmDeletePhyIntfToPoIntfMessage *m = new NsmDeletePhyIntfToPoIntfMessage();
		UI32 resultIter = 0;
		UI32 mappedId = 0;
		LocationId locationId;
        ResourceId  status   = WAVE_MESSAGE_ERROR;

		while (resultIter != remPoFromPhypResults->size()) 
		{
			pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*remPoFromPhypResults)[resultIter]);
			if(pPhyIntfLocalManagedObject->getPoId() == poObjectId)
			{
				m->setIfName(pPhyIntfLocalManagedObject->getIfName());
				mappedId = (UI32) NsmUtils::getMappedIdFromIfName(pPhyIntfLocalManagedObject->getIfName());
				locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
        		status = sendSynchronously (m,locationId);
				if(status == WAVE_MESSAGE_ERROR)
				{
					pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_ERROR);
					remPoFromPhypResults->clear();
					delete remPoFromPhypResults;
					return;
				}
			}
			resultIter++;
		}

		if(m)
			delete m;

		if (remPoFromPhypResults) 
		{
			remPoFromPhypResults->clear();
			delete remPoFromPhypResults;
		}
        */

        // Free spanning tree port id
        // FC Mode issue : port id is deleted even if the PO is deleted from only one node
        if (pPoIntfManagedObject->getSwModeFlag() && MstpToolKit::isVcsEnabled()) {
            char name[16];
            snprintf(name, sizeof name, "%u", pPoIntfManagedObject->getId());
            ResourceId status = MstpToolKit::freePortId(IF_TYPE_PO, name, true, pDceLinearSeqContext);
            if (status != WAVE_MESSAGE_SUCCESS) {
                tracePrintf(TRACE_LEVEL_ERROR, "IntfConfGlobalWorker::deletePoIntfMOStep : "
                        "Cannot free port id for PO %s; err=%u", name, status);
            }
        }

        trace (TRACE_LEVEL_INFO, string ("\tRemoving within transaction PoId :: ") + pPoIntfManagedObject->getId());
        // This will remove from the cache and mark it for delete as well
        pDceLinearSeqContext->getCache()->remove(PoIntfManagedObject::getClassName());
        
		pDceLinearSeqContext->executeNextStep(pNsmGlobalDeletePoIntfMessage->getCompletionStatus ());
        return;
    }

void IntfConfGlobalWorker::deletePoIntfStep (DceLinearSeqContext *pDceLinearSeqContext)
    {
        NsmGlobalDeletePoIntfMessage *pNsmGlobalDeletePoIntfMessage = dynamic_cast<NsmGlobalDeletePoIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

        UI32 poId = strtoul(pNsmGlobalDeletePoIntfMessage->getPoId().c_str (), NULL, 0);
        PoIntfManagedObject *pPoIntfManagedObject = NULL;
        ResourceId  status = NsmUtils::getPoMoByPoId(poId, pPoIntfManagedObject);

        if (status == WAVE_MESSAGE_SUCCESS) {
            pDceLinearSeqContext->getCache()->put(PoIntfManagedObject::getClassName(), pPoIntfManagedObject, true);
            ObjectId poObjectId = pPoIntfManagedObject->getObjectId();
            status = isPoIntfHasMember (poObjectId);
        } else {
            //TODO: This case is not handled, need to revisit
            trace(TRACE_LEVEL_ERROR, "In deletePoIntfStep, retrieving PoIntfManagedObject failed ");
        }

        if((WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ()) && (status == WAVE_MESSAGE_ERROR))
		{
            NSM_PLUG_INFO_S(string("sending delete portchanel channel messgae to fcoe daemon")); 

            NsmDeletePoIntfSPMessage *m = new NsmDeletePoIntfSPMessage();
            m->setPoId (pNsmGlobalDeletePoIntfMessage->getPoId());
            //This fix will not scale for MC
            fcoeDelPoIntf(m);
            delete m;

			pNsmGlobalDeletePoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

			// should send_ctx_p be deleted?
			return;
		}

        WaveSendToClusterContext    *send_ctx_p;
        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback),
			pDceLinearSeqContext);
		prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);

		int ret = SET_LOCATION_ID_API_ERROR;
		ret = NsmUtils::setLocationIdsForPoSubModeCmds(pNsmGlobalDeletePoIntfMessage->getPoId(),send_ctx_p,true);

		if(ret != SET_LOCATION_ID_API_SUCCESS)
		{
        	trace(TRACE_LEVEL_ERROR, "In deletePoIntfStep, call to setLocationIdsForPoSubModeCmds failed ");
            if(ret == PORT_NOT_PART_OF_ANY_INTERFACE)
            {
                NSM_PLUG_INFO_S(string("sending delete portchanel channel messgae to fcoe daemon")); 

                NsmDeletePoIntfSPMessage *m = new NsmDeletePoIntfSPMessage();
                m->setPoId (pNsmGlobalDeletePoIntfMessage->getPoId());
                //This fix will not scale for MC
                fcoeDelPoIntf(m);
                delete m;
            }

            // Dont stop the cleanup of LAG 
        	pNsmGlobalDeletePoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            if (NULL != send_ctx_p) {
                delete send_ctx_p;
                send_ctx_p = NULL;
            }
			return;
		}

        NsmDeletePoIntfSPMessage *m = new NsmDeletePoIntfSPMessage();
        //"tm","tt",1,"testing");

        m->setPoId (pNsmGlobalDeletePoIntfMessage->getPoId());
        m->setNeedSurrogateSupportFlag(true);

        send_ctx_p->setPPrismMessageForPhase1(m);
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);

        pNsmGlobalDeletePoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        return;
    }


    void  IntfConfGlobalWorker::NsmGlobalDeletePoIntfMessageHandler( NsmGlobalDeletePoIntfMessage *pNsmGlobalDeletePoIntfMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::deletePoIntfStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::deletePoIntfLocalDBUpdateStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::deletePoIntfMOStep),
        
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };

        DceLinearSeqContext *pDceLinearSeqContext = new DceLinearSeqContext (pNsmGlobalDeletePoIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pDceLinearSeqContext->holdAll ();
        pDceLinearSeqContext->start ();
    }

    void  IntfConfGlobalWorker::NsmGlobalDeletePoToVlanIntfMessageHandler( NsmGlobalDeletePoToVlanIntfMessage *pNsmGlobalDeletePoToVlanIntfMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfGlobalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfGlobalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfGlobalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfConfGlobalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmGlobalDeletePoToVlanIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void IntfConfGlobalWorker::deleteVlanIntfMOStep (DceLinearSeqContext *pDceLinearSeqContext)
    {
        NsmGlobalDeleteVlanIntfMessage *pNsmGlobalDeleteVlanIntfMessage = dynamic_cast<NsmGlobalDeleteVlanIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

        UI32 vlanId = pNsmGlobalDeleteVlanIntfMessage->getVlanId();
		ObjectId vlanObjectId = pDceLinearSeqContext->getWaveManagedObjectId();

		deletePoIntfVlanReference(vlanId, pDceLinearSeqContext);
		deleteWaveManagedObject (vlanObjectId);

        pDceLinearSeqContext->executeNextStep(pNsmGlobalDeleteVlanIntfMessage->getCompletionStatus ());
        return;
    }

    void IntfConfGlobalWorker::validateVlanIntfDeleteStep (DceLinearSeqContext *pDceLinearSeqContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		bool isChangeVlanList = false;
        NsmGlobalDeleteVlanIntfMessage *pNsmGlobalDeleteVlanIntfMessage = dynamic_cast<NsmGlobalDeleteVlanIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

        UI32 vlan_id = pNsmGlobalDeleteVlanIntfMessage->getVlanId();

        if (! ifVlanExistsByVlanId (vlan_id, pDceLinearSeqContext)) {
            trace (TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateVlanIntfDeleteStep. Vlan doesn't exists"));
            status = WRC_NSM_API_ERR_VLAN_NOT_EXIST;
        }

		if(NsmUtils::isVlanPartOfActiveProfile(vlan_id, isChangeVlanList))
		{
			status = WRC_NSM_API_ERR_ACTIVE_PROFILES;
		}

        ObjectId vlanObjectId;
		NsmUtils::getVlanIntfObjectIdByVlanIfName (vlan_id, vlanObjectId, false, pDceLinearSeqContext);
		pDceLinearSeqContext->setWaveManagedObjectId(vlanObjectId);

		pNsmGlobalDeleteVlanIntfMessage->setIsChangeAPPMVlanList(isChangeVlanList);
		pNsmGlobalDeleteVlanIntfMessage->setCompletionStatus(status);
		pDceLinearSeqContext->executeNextStep(status);
	
		return;
	
	}

    void IntfConfGlobalWorker::deleteVlanIntfStep (DceLinearSeqContext *pDceLinearSeqContext)
    {
        NsmGlobalDeleteVlanIntfMessage *pNsmGlobalDeleteVlanIntfMessage = dynamic_cast<NsmGlobalDeleteVlanIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

        WaveSendToClusterContext    *send_ctx_p;
        NsmDeleteVlanIntfSPMessage *m = new NsmDeleteVlanIntfSPMessage();
		ObjectId vlanObjectId = pDceLinearSeqContext->getWaveManagedObjectId();
        //"tm","tt",1,"testing");

        m->setVlanId (pNsmGlobalDeleteVlanIntfMessage->getVlanId());
		m->setVlanObjectID(vlanObjectId);
        m->setNeedSurrogateSupportFlag(true);
		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback),
			pDceLinearSeqContext);
		prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);

        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);


        pNsmGlobalDeleteVlanIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        return;
    }

    void IntfConfGlobalWorker::deleteVlanIntfLocalDBUpdateStep (DceLinearSeqContext *pDceLinearSeqContext)
    {
        trace(TRACE_LEVEL_DEBUG, "Entering IntfConfGlobalWorker::deleteVlanIntfLocalDBUpdateStep... ");
        // Send message to all local plugins to update DB only.
        NsmGlobalDeleteVlanIntfMessage *pNsmGlobalDeleteVlanIntfMessage = dynamic_cast<NsmGlobalDeleteVlanIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

        WaveSendToClusterContext    *send_ctx_p;
        NsmDeleteVlanIntfSPMessage *m = new NsmDeleteVlanIntfSPMessage();
        ObjectId vlanObjectId = pDceLinearSeqContext->getWaveManagedObjectId();

        m->setVlanId (pNsmGlobalDeleteVlanIntfMessage->getVlanId());
        m->setVlanObjectID(vlanObjectId);
        m->setNeedSurrogateSupportFlag(true);
        m->setDBUpdateRequest(true);
        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback),
            pDceLinearSeqContext);
        prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);

        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
        sendToWaveCluster(send_ctx_p);

        pNsmGlobalDeleteVlanIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        return;
    }

    void IntfConfGlobalWorker::deleteVlanIntfXstpStep (DceLinearSeqContext *pDceLinearSeqContext)
	{
		int vlanId;
		ResourceId status = WAVE_MESSAGE_ERROR;
        NsmGlobalDeleteVlanIntfMessage *pMsg = dynamic_cast<NsmGlobalDeleteVlanIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());
        vlanId = pMsg->getVlanId();
		status = (NsmGlobalObjectManager::getInstance()->getIntfXstpGlobalConfWorker())->deleteVlanIntfXstp(vlanId, pDceLinearSeqContext); 
        pDceLinearSeqContext->executeNextStep(status);
		if(status == WAVE_MESSAGE_ERROR)
			
		return;
	}

	void IntfConfGlobalWorker::deletePVlanReferences(DceLinearSeqContext *pDceLinearSeqContext)
    {
        NsmGlobalDeleteVlanIntfMessage *pNsmGlobalDeleteVlanIntfMessage = dynamic_cast<NsmGlobalDeleteVlanIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

        UI32 vlanId = pNsmGlobalDeleteVlanIntfMessage->getVlanId();
        removePVlanAssociations(vlanId, pDceLinearSeqContext);
        removePVlanHostAssociation(vlanId, pDceLinearSeqContext);
        removePVlanTrunkAssociation(vlanId, pDceLinearSeqContext);
        removePVlanMapping(vlanId, pDceLinearSeqContext);
		pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        NSM_PLUG_DBG("Exiting deletePVlanReferences ...");
        return ;
    }


	 void IntfConfGlobalWorker::removePVlanAssociations(const UI32 &vlanId, PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {

     NSM_PLUG_DBG("Entering removePVlanAssociations ...");

	 WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
	 	 syncQueryCtxt.addAndAttribute(new AttributeUI8(PVLAN_TYPE_PRIMARY,"pVlanType"));
	 	 syncQueryCtxt.addAndAttribute(new AttributeUI32Range(UI32Range(""),"pVlanSecAssocRange"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);

 	 syncQueryCtxt.addSelectField("pVlanSecAssocRange");

     vector<WaveManagedObject * >*               pVlanResults                   =  querySynchronously(&syncQueryCtxt);

	 	 if ((pVlanResults->size()))
	 	 {
		 for(unsigned int j = 0;j < pVlanResults -> size() ;j++)
		 {
				   VlanIntfManagedObject *pVlanIntfMO = dynamic_cast<VlanIntfManagedObject*>((*pVlanResults)[j]);

			UI32Range moSecVlanrange = pVlanIntfMO->getPvlanSecAssocRange();
				   if(moSecVlanrange.find(vlanId))
				   {
					   UI32Range rangeFromMsg(vlanId, vlanId);
					   pVlanIntfMO->setPvlanSecAssocRange(moSecVlanrange - rangeFromMsg);
					pVlanIntfMO->addAttributeToBeUpdated("pVlanSecAssocRange");
					updateWaveManagedObject(pVlanIntfMO);
					break;
				}
			}
		}

	   if (NULL != pVlanResults)
	   {
		   pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pVlanResults);
	   }
  }


	void IntfConfGlobalWorker::removePVlanHostAssociation(const UI32 &vlanId, PrismLinearSequencerContext *pPrismLinearSequencerContext, UI32Range secVlanRange)
   {
      NSM_PLUG_DBG("Entering removePVlanHostAssociation...");
	   WaveManagedObjectSynchronousQueryContextForUpdate *qc = new WaveManagedObjectSynchronousQueryContextForUpdate (PoPhyIntfManagedObject::getClassName());

	   if(secVlanRange.isEmpty())
	   {
	   qc->addOrAttribute (new AttributeUI32(vlanId,"pVlanHostPrimVlanId"));
	   qc->addOrAttribute (new AttributeUI32(vlanId,"pVlanHostSecVlanId"));

	   }
	   else
	   {
		   qc->setAttributeRangeForQuery(new AttributeUI32Range(secVlanRange, "pVlanHostSecVlanId"));
	   }

	   trace (TRACE_LEVEL_DEBUG, string("removePVlanHostAssociation "));
	   PoPhyIntfManagedObject *criteriaMo = dynamic_cast<PoPhyIntfManagedObject*> ( qc->getWaveManagedObjectToAddUpdateAttributes());
	   criteriaMo->setPvlanHostPrimVlanId(0);
	   criteriaMo->setPvlanHostSecVlanId(0);
	   criteriaMo->addAttributeToBeUpdated("pVlanHostPrimVlanId");
	   criteriaMo->addAttributeToBeUpdated("pVlanHostSecVlanId");
	   trace (TRACE_LEVEL_DEBUG, string("Goin to update db "));
	   updateMultipleWaveManagedObjects (qc);

   }



	 void IntfConfGlobalWorker::removePVlanTrunkAssociation(const UI32 &vlanId, PrismLinearSequencerContext *pPrismLinearSequencerContext, UI32Range secVlanRange)
   {
      NSM_PLUG_DBG("Entering removePVlanTrunkAssociation...");

      vector<string> selectFields;
	  selectFields.push_back ("objectId");
	  selectFields.push_back ("ownerManagedObjectId");

      WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PVlanTrunkAssocManagedObject::getClassName());

	   if(secVlanRange.isEmpty())
	   {
	  syncQueryCtxt.addOrAttribute (new AttributeUI32(vlanId,"primaryVlanId"));
	  syncQueryCtxt.addOrAttribute (new AttributeUI32(vlanId,"secondaryVlanId"));
	   }
	   else
	   {
		   syncQueryCtxt.setAttributeRangeForQuery(new AttributeUI32Range(secVlanRange, "secondaryVlanId"));
	   }


      trace (TRACE_LEVEL_DEBUG, string("removePVlanTrunkAssociation"));
      syncQueryCtxt.addSelectFields(selectFields);
      vector<WaveManagedObject * >*               pTrunkAssocResults                   =  querySynchronously(&syncQueryCtxt);

	   if ((pTrunkAssocResults->size())) {

		 for(unsigned int j = 0;j < pTrunkAssocResults -> size() ;j++)
		 {

		   PVlanTrunkAssocManagedObject *pPVlanTrunkAssocManagedObject = dynamic_cast<PVlanTrunkAssocManagedObject*>((*pTrunkAssocResults)[j]);

		   deleteFromComposition(PoPhyIntfManagedObject::getClassName(), PVlanTrunkAssocManagedObject::getClassName(), "pVlanTrunkAssoc",
			   pPVlanTrunkAssocManagedObject->getOwnerManagedObjectId(),
			   pPVlanTrunkAssocManagedObject->getObjectId());

		 }
	   }

	   if (NULL != pTrunkAssocResults)
	   {
		   pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pTrunkAssocResults);
	   }
   }



	 void IntfConfGlobalWorker::removePVlanMapping(const UI32 &vlanId, PrismLinearSequencerContext *pPrismLinearSequencerContext, UI32Range secVlanRange)
   {
      NSM_PLUG_DBG("Entering removePVlanMapping ...");

      WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PVlanMappingManagedObject::getClassName());
      vector<WaveManagedObject * >*               pMappingResults                   =  querySynchronously(&syncQueryCtxt);

	   if ((pMappingResults->size())) {

		 for(unsigned int j = 0;j < pMappingResults -> size() ;j++)
		 {
		   PVlanMappingManagedObject *pPVlanMappingManagedObject = dynamic_cast<PVlanMappingManagedObject*>((*pMappingResults)[j]);

		   if(pPVlanMappingManagedObject->getPrimaryVlanId() == vlanId && secVlanRange.isEmpty()){
			   NSM_PLUG_DBG_S(string("In primary ") + vlanId);
			   deleteFromComposition(PoPhyIntfManagedObject::getClassName(), PVlanMappingManagedObject::getClassName(), "pVlanMapping",
			  			   pPVlanMappingManagedObject->getOwnerManagedObjectId(),
			  			   pPVlanMappingManagedObject->getObjectId());
		   }
		   else
		   {

				UI32Range moSecVlanrange = pPVlanMappingManagedObject->getSecPvlanRange();
			   UI32Range updatedVlanRange;
			   bool isRangeUpdated = false;

			   if(secVlanRange.isEmpty() && moSecVlanrange.find(vlanId))
			   {
				   UI32Range rangeFromMsg(vlanId, vlanId);
				   updatedVlanRange = moSecVlanrange - rangeFromMsg;
				   isRangeUpdated = true;
			   }
			   else if(moSecVlanrange.containsAny(secVlanRange))
			   {
				   updatedVlanRange = moSecVlanrange - secVlanRange;
				   isRangeUpdated = true;
			   }

			   if(updatedVlanRange.getCount() > 0)
			   {
				   pPVlanMappingManagedObject->setSecPvlanRange(updatedVlanRange);
							pPVlanMappingManagedObject->addAttributeToBeUpdated("secVlanRange");
							updateWaveManagedObject (pPVlanMappingManagedObject);
			   }
			   else if(isRangeUpdated)
			   {
				   NSM_PLUG_DBG_S(string("In Secondary Only entry going to delete") + j );
				   deleteFromComposition(PoPhyIntfManagedObject::getClassName(), PVlanMappingManagedObject::getClassName(), "pVlanMapping",
						   pPVlanMappingManagedObject->getOwnerManagedObjectId(),
						   pPVlanMappingManagedObject->getObjectId());
				}
			}
		}
	   }

	   if (NULL != pMappingResults)
	   {
		   pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pMappingResults);
	   }
   }


	void IntfConfGlobalWorker::deleteVlanClassifierVlanReference(DceLinearSeqContext *pDceLinearSeqContext)
    {
		ObjectId vlanObjectId = pDceLinearSeqContext->getWaveManagedObjectId();
        removeVlanClassifierPoIntf(vlanObjectId);
		pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        NSM_PLUG_DBG("Exiting ...");
        return ;
    }

    void IntfConfGlobalWorker::deleteSviIntfStep (DceLinearSeqContext *pDceLinearSeqContext) {
        NsmGlobalDeleteVlanIntfMessage *pMsg = dynamic_cast<NsmGlobalDeleteVlanIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());
        UI32 ifId = pMsg->getVlanId();
        trace(TRACE_LEVEL_DEBUG, string("Entering IntfConfGlobalWorker::deleteSviIntfStep: Ve Intf: ") + ifId);

        //Check if any VE interfaces exists corresponding to given vlan Id. Then only send message to Local plugin.
        WaveManagedObjectSynchronousQueryContext queryContext(NsmIntfSviManagedObject::getClassName());
        queryContext.addAndAttribute(new AttributeUI32(&ifId, "vlanId"));
        UI32 count = 0;
        ResourceId resId = querySynchronouslyForCount(&queryContext, count);
        if (resId != WAVE_MESSAGE_SUCCESS) {
            trace (TRACE_LEVEL_INFO, string ("IntfConfGlobalWorker::deleteSviIntfStep : querySynchronouslyForCount on vlanId failed"));
            pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
        }
        if (count == 0) {
            trace (TRACE_LEVEL_INFO, string ("IntfConfGlobalWorker::deleteSviIntfStep : No SVIs configured to be deleted"));
            pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }

        NsmLocalNodeIpMessage *pMessage = new NsmLocalNodeIpMessage();
        pMessage->setOpCode(NSM_MSG_OP_DELETE);
        pMessage->setCmdCode(NSM_MSG_CMD_NODE_VLAN);
        pMessage->setIfType(VLAN_INT);

        string ifName = string("vlan-");
		ifName += getString(ifId);
        pMessage->setIfId(ifId);
        pMessage->setIfName(ifName);
        pMessage->setValidationBackendStepFlag(true);
        pMessage->setNeedSurrogateSupportFlag(true);

        WaveSendToClusterContext *send_ctx_p;
		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback), pDceLinearSeqContext);
		send_ctx_p->setPPrismMessageForPhase1(pMessage);
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);
        pMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    }

    void  IntfConfGlobalWorker::NsmGlobalDeleteVlanIntfMessageHandler( NsmGlobalDeleteVlanIntfMessage *pNsmGlobalDeleteVlanIntfMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::validateVlanIntfDeleteStep),
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::deleteVlanIntfStep),
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::deleteVlanIntfLocalDBUpdateStep),
			reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::deleteVlanClassifierVlanReference),
			reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::deletePVlanReferences),
			reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::deleteVlanClassificationReferences),
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::deleteVlanIntfXstpStep),
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::deleteSviIntfStep),
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::deletePortProfileVlanReference),

// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::deleteVlanIntfMOStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };

        DceLinearSeqContext *pDceLinearSeqContext = new DceLinearSeqContext (pNsmGlobalDeleteVlanIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pDceLinearSeqContext->holdAll ();
        pDceLinearSeqContext->start();


    }

    void IntfConfGlobalWorker::validateCreatePoIntfStep(
            PrismLinearSequencerContext *pPrismLinearSequencerContext) {

        NsmGlobalCreatePoIntfMessage *pNsmGlobalCreatePoIntfMessage =
                dynamic_cast<NsmGlobalCreatePoIntfMessage *>(pPrismLinearSequencerContext->getPPrismMessage());

		UI32 poId = strtoul(pNsmGlobalCreatePoIntfMessage->getPoId().c_str (), NULL, 0);
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		if(poId < 0 || poId > DCM_PORTCHANNEL_MAX_LAGS)
			status = WRC_NSM_ERR_INVALID_PO_ID;
		else
			status = validatePortChannelCount();
#if 0
        // Check for maximum brocade trunks per chip
        if (DCM_MAX_BRCD_AGGREGATOR_PER_CHIP)
        {
        }
#endif
        pNsmGlobalCreatePoIntfMessage->setCompletionStatus(status);

        pPrismLinearSequencerContext->executeNextStep(status);
    }

ResourceId IntfConfGlobalWorker::validatePoIntfStep (DceLinearSeqContext *pDceLinearSeqContext)
    {
        trace (TRACE_LEVEL_DEVEL,"Entering IntfConfGlobalWorker::validatePoIntfStep ... ");
        
        NsmGlobalUpdatePoIntfMessage *pNsmGlobalUpdatePoIntfMessage = dynamic_cast<NsmGlobalUpdatePoIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

        UI32 poId = strtoul(pNsmGlobalUpdatePoIntfMessage->getPoId().c_str (), NULL, 0);

		if(poId < 0 || poId > DCM_PORTCHANNEL_MAX_LAGS)
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validatePoIntfStep: Invalid port-channel id:") + poId);

			pNsmGlobalUpdatePoIntfMessage->setCompletionStatus(WRC_NSM_ERR_INVALID_PO_ID);
			pDceLinearSeqContext->executeNextStep(WRC_NSM_ERR_INVALID_PO_ID);

			return WRC_NSM_ERR_INVALID_PO_ID;
		}

        PoIntfManagedObject *pPoIntfManagedObject = NULL;
        ResourceId  status = NsmUtils::getPoMoByPoId(poId, pPoIntfManagedObject);

        trace(TRACE_LEVEL_INFO, string("validatePoIntfStep complete: ") + pNsmGlobalUpdatePoIntfMessage->getCmdCode());

        if (pPoIntfManagedObject == NULL || status != WAVE_MESSAGE_SUCCESS) {
			pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_ERROR);
            trace(TRACE_LEVEL_INFO, string("pPoIntfManagedObject is NULL: ") + pNsmGlobalUpdatePoIntfMessage->getCmdCode());
            return (WAVE_MESSAGE_ERROR);
        } else {
            pDceLinearSeqContext->getCache()->put(PoIntfManagedObject::getClassName(), pPoIntfManagedObject, true);

            ResourceId rspanValidationStatus = validateVlanForRSPANConfig(pNsmGlobalUpdatePoIntfMessage,pDceLinearSeqContext);
            if(WAVE_MESSAGE_SUCCESS !=  rspanValidationStatus) {
            	NSM_PLUG_DBG_S(string("RSPAN Validation failed. Configuration will not proceed"));
            	status = rspanValidationStatus;
            }

            // Should this be done here? at this time, no changes are being made
            // updateWaveManagedObject (pPoIntfManagedObject);

            switch(pNsmGlobalUpdatePoIntfMessage->getCmdCode())
	{
                case NSMUPDATE_PO_MODE :
                case NSMUPDATE_PO_DELMODE:
		{
                         /* Checking sw is enabled*/
                         if(!pPoIntfManagedObject->getSwModeFlag())
                         {
                             trace (TRACE_LEVEL_ERROR, string ("\t Interface not configured for switching "));
                             status = WRC_SWITCHING_NOT_ENABLED;
                         }
                         break;
		}

          case NSMUPDATE_PO_MTU: {
			if (pNsmGlobalUpdatePoIntfMessage->getMtu() < 2500) {
				if (NsmUtils::isAnyPoMemberFcoeConfigured(pNsmGlobalUpdatePoIntfMessage->getPoId())) {
					trace(TRACE_LEVEL_ERROR,string("IntfConfGlobalWorker:validatePoIntfStep: NSMUPDATE_PO_MTU"));
					status = WRC_PORTCHANNEL_MBR_FCOE_ENABLED;
				}
			}
			break;
		}


			case NSMUPDATE_PO_PVLAN_MODE_TRUNK:
			case NSMUPDATE_PO_PVLAN_MODE_TRUNK_HOST:
			case NSMUPDATE_PO_PVLAN_MODE_TRUNK_PROMISCUOUS:
			case NSMUPDATE_PO_PVLAN_MODE_PROMISCUOUS:
			case NSMUPDATE_PO_PVLAN_MODE_HOST:
			{
				/* Checking sw is enabled*/
                if(!pPoIntfManagedObject->getSwModeFlag())
                {
                    trace (TRACE_LEVEL_ERROR, string ("\t Interface not configured for switching "));
                    status = WRC_SWITCHING_NOT_ENABLED;
                }

                //isClassifierPresent
                else if(PVlanUtils::isClassifierPresentOnIntf(pPoIntfManagedObject->getIfName(), true))
                {
                    trace (TRACE_LEVEL_ERROR, string ("\t PVlanUtils::isClassifierPresentOnIntf"));
                    status = WRC_NSM_PVLAN_PMODE_CLASSIFIER_PRESENT;
                }

                //isSpanningTree
                else if ((pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_PVLAN_MODE_TRUNK_HOST ||  pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_PVLAN_MODE_HOST)
         	    		 && pPoIntfManagedObject->getPortStpConfig()->getStpEnable()) {
         	    	 trace (TRACE_LEVEL_ERROR, string ("Spanning tree is enabled ,so cannot set mode as host."));
         	         status = WRC_NSM_PVLAN_HOST_MODE_STP_PRESENT;
         	     }

                break;
			}

  	 case NSMUPDATE_PO_PVLAN_HOST_ASSOCIATION :
  	 {
			trace(TRACE_LEVEL_DEBUG, string("\t IN VALidations prim"));
			UI32 primVId = pNsmGlobalUpdatePoIntfMessage->getPvlanHostPrimVlanId();
			UI32 secVId = pNsmGlobalUpdatePoIntfMessage->getPvlanHostSecVlanId();
			NSM_PLUG_DBG_S(" IN VALidations primVId " + primVId);
			UI32Range defaultRange = UI32Range("");

			//in case of update operation corresponding to primary vlan
			if (primVId == 0) {
				primVId = pPoIntfManagedObject->getPvlanHostPrimVlanId();
				pNsmGlobalUpdatePoIntfMessage->setPvlanHostPrimVlanId(primVId);
				trace(TRACE_LEVEL_DEBUG, string("NSMUPDATE_PO_PVLAN_HOST_ASSOCIATION Validation step : Now prim vlan Id ")+ pNsmGlobalUpdatePoIntfMessage->getPvlanHostPrimVlanId());
			}

			//sw enabled
			if (!pPoIntfManagedObject->getSwModeFlag()) {
				trace(TRACE_LEVEL_ERROR,string(" Interface not configured for switching "));
				status = WRC_SWITCHING_NOT_ENABLED;
			}

			//sw mode host set
			else if (!(pPoIntfManagedObject->getPvlanModeHost() == true || pPoIntfManagedObject->getPvlanModeTrunkHost() == true)) {
				trace(TRACE_LEVEL_ERROR, string("IN VALidations sw mode host set"));
				status = WRC_PVLAN_MODE_NOT_HOST;
			}

			//is Primary vlan
			else if (!PVlanUtils::isPrimaryVlan(primVId)) {
				trace(TRACE_LEVEL_ERROR, string(" IN VALidations is Primary vlan"));
				status = WRC_NSM_PVLAN_NOT_PRIM_VLAN;
			}

			//Is Secondary vlan
			else if (!PVlanUtils::isSecondaryVlan(secVId)){
				status = WRC_NSM_PVLAN_NOT_SEC_VLAN;
			}

			//doesAssociation exists
			else if (!PVlanUtils::doesAssociationExists(primVId, secVId, pDceLinearSeqContext)){
				trace(TRACE_LEVEL_ERROR, string(" IN VALidations doesAssociationExists"));
				status = WRC_PVLAN_ASSOC_NOT_PRESENT;
			}

		   //When
		   //1. The port is in pritave-vlan-trunk-host mode,
		   //2. The secondary vlan is >= 4096,
		   //3. The secondary vlan should have a ctag associated with, already.
		   //else, return ERROR.
		   else if(pPoIntfManagedObject->getPvlanModeTrunkHost() && (secVId >= MAX_DOT1Q_VLAN_ID) && (NsmUtils::getVlanClassificationMOCount(CtagVlanClassificationMO::getClassName(), secVId, "", 0, 0, pPoIntfManagedObject->getObjectId()) == 0))
		   {
				status = WRC_NSM_GVLAN_SECONDARY_HAVE_NO_CTAG;
				trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validatePoIntfStep:Secondary Vlan is not associated with a ctag."));
		   }

			trace(TRACE_LEVEL_INFO, string("NSMUPDATE_PO_PVLAN_HOST_ASSOCIATION Validation step ")+ primVId);
			break;
		}

		//If port-profiled port config is applied on a PO, then switchport configurations cannot be applied on the same.

     case NSMUPDATE_PO_SW:
     {
         bool isPortProfiled = false;
         APPMUtils::isPortProfilePort(pPoIntfManagedObject, isPortProfiled);
         if (isPortProfiled) {
             status = WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED;
         }
         break;
     }
        case NSMUPDATE_PO_PORT_PROFILED :
        case NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PO :
        case NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PO :
        {
            if(pPoIntfManagedObject->getSwModeFlag()) {
                status = WRC_NSM_ERR_DCM_PORT_PROFILE_PORT_NOT_ALLOWED;
            } else if (ObjectId::NullObjectId != pPoIntfManagedObject->getCeeMap()){
                status = WRC_NSM_ERR_DCM_CEE_MAP_ON_PORT_CHANNEL;
            } else if ((pPoIntfManagedObject->getDefaultCos() != 0)
                    || (QosUtils::isInterfaceQosNonCeeConfigured(pPoIntfManagedObject))) {
                status = WRC_NSM_ERR_DCM_QOS_CONFIG_ON_PORT_CHANNEL;
            } else if (QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject)) {
                status = SSM_DCM_ERR_APPM_FCOE_PORT_CONFIG_ON_PO;
            }

            if(WAVE_MESSAGE_SUCCESS != status) {
                break;
            }
            
            UI32 cmdCode = pNsmGlobalUpdatePoIntfMessage->getCmdCode();
            string portProfileName = pNsmGlobalUpdatePoIntfMessage->getPortProfileName();
            string portProfileDomainName = pNsmGlobalUpdatePoIntfMessage->getPortProfileDomainName();

            if(NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PO == cmdCode) {
                status = APPMUtils::doesPortProfileExist(portProfileName);
                if( WAVE_MESSAGE_SUCCESS != status ) {
                    break;
                }
                if( !(ProvisioningUtil::isGvlanEnabled()) ) {
                    status = WRC_APPM_INTERFACE_CANNOT_BE_ASSOCIATED_WITH_PORT_PROFILE_OR_DOMAIN_WHEN_VF_IS_DISABLED;
                    break;
                }
            } else if (NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PO == cmdCode) {
                status = APPMUtils::doesPortProfileDomainExist(portProfileDomainName);
                if(WAVE_MESSAGE_SUCCESS != status) {
                    break;
                }
                status = APPMUtils::isInterfaceAlreadyAssociatedWithAPortProfileDomain
                    (pPoIntfManagedObject);
                if(WAVE_MESSAGE_SUCCESS != status) {
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
                (pPoIntfManagedObject, cmdCode);
            if(WAVE_MESSAGE_SUCCESS != status) {
                break;
            }
            break;
        }
				 case NSMUPDATE_PO_ALLOWEDSPEED:
		{
			/*Checking if port-channel is admin down */
			if(!pPoIntfManagedObject->getStatus())
			{
				trace (TRACE_LEVEL_ERROR, string ("Interface is not admin down.Speed configuration cannot be allowed"));

				status = WRC_PO_ADMIN_UP_SPEED_FAILED;
			}
			break;
		}
				 case NSMUPDATE_PO_IGNORE_SPLIT:
		{
			/*Checking if port-channel is dynamic and standard type LAG*/
			if((pPoIntfManagedObject->getPoMode() == NSM_PO_MODE_ON) ||(pPoIntfManagedObject->getPoType() == NSM_PO_TYPE_BROCADE)){
				trace (TRACE_LEVEL_ERROR, string ("Interface is not a dynamic and standard type LAG.Hence Ignore split cannot be configured"));
				status =  WRC_PO_IGNORE_SPLIT_FAILED;
                         }
                         break;
		}
                 case NSMUPDATE_PO_SW_ADD_ACCESS_VLAN:
		{
                         UI32 accessVlanId = atoi(pNsmGlobalUpdatePoIntfMessage->getVlanRange().toString().c_str());
						 ObjectId poObjectId = pPoIntfManagedObject->getObjectId();
                         /* Checking sw is enabled*/
                         if(!pPoIntfManagedObject->getSwModeFlag())
                         {
                             trace (TRACE_LEVEL_ERROR, string ("\t Interface not configured for switching "));
                             status = WRC_SWITCHING_NOT_ENABLED;
                         }

                         /* Checking sw is access*/
						 else if(pPoIntfManagedObject->getSwMode() != SW_MODE_ACCESS)
                         {
                             trace (TRACE_LEVEL_ERROR, string ("\t Port mode not set to access"));
                             status = WRC_SW_ACCESSMODE_NOT_ENABLED;
                         }

                         else if(PVlanUtils::isPVlan(accessVlanId))
                         {
                             status = WRC_NSM_PVLAN_VLAN;
                         }
						//If there is a mac/macGroup vlanClassification configured on the same port with the same access vlan, then reject the command.
						else if((NsmUtils::getVlanClassificationMOCount(MacAddressVlanClassificationMO::getClassName(), accessVlanId, "", 0, 0, poObjectId)) || (NsmUtils::getVlanClassificationMOCount(MacGroupVlanClassificationMO::getClassName(), accessVlanId, "", 0, 0, poObjectId)))
						{
							trace(TRACE_LEVEL_ERROR, string("IntfConfLocalWorker::validatePhyIntfStep: The access vlan id has already been used in a vlan classification."));
							status = WRC_NSM_GVLAN_VLAN_IS_USED_IN_CLASSIFICATION;
						}

                         break;
		}
                 case NSMUPDATE_PO_SW_TRUNK_VLAN_ALL:
                 case NSMUPDATE_PO_SW_TRUNK_VLAN_NONE:
                 case NSMUPDATE_PO_SW_ADD_TRUNK_VLAN:
                 case NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN:
                 case NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN:
                 case NSMUPDATE_PO_NATIVEVLANDISABLE:
                 case NSMUPDATE_PO_NATIVEVLANID:
		{
                         /* Checking sw is enabled*/
                         if(!pPoIntfManagedObject->getSwModeFlag())
                         {
                             trace (TRACE_LEVEL_ERROR, string ("\t Interface not configured for switching "));
                             status = WRC_SWITCHING_NOT_ENABLED;
                         }
                         
                         /* Checking sw is trunk */
						 else if(!(pPoIntfManagedObject->getSwMode() == SW_MODE_TRUNK || pPoIntfManagedObject->getPvlanModeTrunkPromis() == true))
                         {
                             trace (TRACE_LEVEL_ERROR, string ("\t Port mode not set to Trunk"));
                             status = WRC_SW_TRUNKMODE_NOT_ENABLED;
                         }
     
						else if ((pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN) ||
                           (pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN))
                        {
                                  UI32Range vlanAddRange = pNsmGlobalUpdatePoIntfMessage->getVlanRange();
                                  if ( NsmUtils::isNativeVlanPresentInList(vlanAddRange,  NULL, pPoIntfManagedObject)){
                                          NSM_PLUG_DBG_ERR(" Can't remove Native vlan");
                                          status = WRC_NSM_API_ERR_DEL_NATIVE_VLAN;
                                  }
                        }
						 else if(pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_NATIVEVLANID)
						 {
							 UI32 nativeVlanId = pNsmGlobalUpdatePoIntfMessage->getNativeVlanId();
							 if(pPoIntfManagedObject->getPvlanModeTrunkPromis() == true && PVlanUtils::isSecondaryVlan(nativeVlanId))
							 {
								 trace(TRACE_LEVEL_ERROR, string("\t Native vlan configuration not allowed for secondary private-vlan "));
								 status = WRC_NSM_PVLAN_NATIVE_SEC;
							 }
							 else if(pPoIntfManagedObject->getSwMode() == SW_MODE_TRUNK && true == PVlanUtils::isPVlan(nativeVlanId))
							 {
                        		 trace(TRACE_LEVEL_ERROR, string("\t SW_MODE_TRUNK --Native vlan configuration not allowed for private-vlan "));
                        		 status = WRC_NSM_PVLAN_AS_NATIVE_VLAN;
							 }
							 else if(pPoIntfManagedObject->getSwMode() == SW_MODE_TRUNK && true == NsmUtils::isRemoteSpanVlan(pNsmGlobalUpdatePoIntfMessage->getPvlanNativeVlanId()))
							 {
								 NSM_PLUG_DBG_ERR_S(string("SW_MODE_TRUNK --Native vlan configuration not allowed for remote-span vlan"));
				                 status = WRC_SPAN_NATIVE_VLAN_CANNOT_BE_RSPAN_VLAN;
							 }
							else if(NsmUtils::getVlanClassificationMOCount(0, nativeVlanId, pPoIntfManagedObject->getObjectId()))
							{
								trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validatePoIntfStep:Native vlan id has already been configured as ctag on this port."));
								status = WRC_NSM_VLAN_ERR_VLAN_USED_AS_CTAG;
							}
						 }
                   
						else if (pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_SW_ADD_TRUNK_VLAN)
                        {
							UI32Range vlanAddRange = pNsmGlobalUpdatePoIntfMessage->getVlanRange();

							//Is secondary Vlan present
							if (PVlanUtils::isSecondaryVlanRange(vlanAddRange) || PVlanUtils::isPrimaryVlanRange(vlanAddRange))
							{
								status = WRC_NSM_PVLAN_VLAN;
								trace(TRACE_LEVEL_ERROR,string("NSMUPDATE_PO_SW_ADD_TRUNK_VLAN -- private vlan not allowed"));
							}
							else if(NsmUtils::getVlanClassificationMOCount(vlanAddRange, pPoIntfManagedObject->getObjectId()))
							{
								trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validatePoIntfStep:One of the vlans has been configured as ctag on this port."));

								status = WRC_NSM_GVLAN_ONE_OF_THE_VLANS_IS_A_CTAG;
							 }
                        }
						else if(pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_SW_TRUNK_VLAN_ALL)
						{
							if(NsmUtils::getVlanClassificationMOCount(0, 0, pPoIntfManagedObject->getObjectId()))
							{
								trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validatePhyIntfStep:Vlan Classification on this port exists."));
								status = WRC_NSM_GVLAN_CONFIGURATION_EXISTS;
							}
						}

                         break;
                     }

     			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ALL:
    			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_NONE:
     			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ADD:
     			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_REMOVE:
     			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_EXCEPT:
     			case NSMUPDATE_PO_PVLAN_NATIVE_VLANID:
                 {

                         /* Checking sw is enabled*/
                         if(!pPoIntfManagedObject->getSwModeFlag())
                         {
                             trace (TRACE_LEVEL_ERROR, string ("\t Interface not configured for switching "));
                             status = WRC_SWITCHING_NOT_ENABLED;
                         }
                  		else if (!(pPoIntfManagedObject->getPvlanModeTrunkBas()))
                  		{
                    		 trace(TRACE_LEVEL_ERROR, string("\t Port mode not set to Pvlan Trunk"));
                    		 status = WRC_PVLAN_MODE_NOT_TRUNK;
                  		}

                         //native Vlan Id check
                 		else if((pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_PVLAN_TRUNK_VLAN_REMOVE) ||
                                (pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_PVLAN_TRUNK_VLAN_EXCEPT))
                 		{
                            if ( PVlanUtils::isNativeVlanPresentInList(pNsmGlobalUpdatePoIntfMessage->getPvlanRange(), pPoIntfManagedObject->getPvlanNativeVlanId())){
                                    NSM_PLUG_DBG_ERR(" Can't remove private Native vlan");
                                    status = WRC_NSM_PVLAN_ERR_DEL_NATIVE_VLAN;
                            }
                 		}

                        else if ((pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ADD))
                        {
                         	 UI32Range vlanAddRange = pNsmGlobalUpdatePoIntfMessage->getPvlanRange();

                         	 if (PVlanUtils::isSecondaryVlanRange(vlanAddRange) || PVlanUtils::isPrimaryVlanRange(vlanAddRange))
                         	 {
                         		 status = WRC_NSM_PVLAN_VLAN;
                         		 trace(TRACE_LEVEL_ERROR,string("NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ADD -- private vlan not allowed"));
                         	 }
							else if(NsmUtils::getVlanClassificationMOCount(vlanAddRange, pPoIntfManagedObject->getObjectId()))
							{
								trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validatePoIntfStep:One of the vlans has been configured as ctag on this port."));

								status = WRC_NSM_GVLAN_ONE_OF_THE_VLANS_IS_A_CTAG;
							 }
                        }
						else if((pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_PVLAN_NATIVE_VLANID) && (NsmUtils::getVlanClassificationMOCount(0, pNsmGlobalUpdatePoIntfMessage->getPvlanNativeVlanId(), pPoIntfManagedObject->getObjectId())))
						{
							trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validatePoIntfStep:Native vlan id has already been configured as ctag on this port."));
							status = WRC_NSM_VLAN_ERR_VLAN_USED_AS_CTAG;
						}
						else if((pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ALL) && (NsmUtils::getVlanClassificationMOCount(0, 0, pPoIntfManagedObject->getObjectId())))
						{
							trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validatePhyIntfStep:Vlan Classification on this port exists."));
							status = WRC_NSM_GVLAN_CONFIGURATION_EXISTS;
						}

                 	break;
                  }


                case NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_OVERRIDE:
                    {
                        /* Override current value.  It will always success */
                        trace (TRACE_LEVEL_DEVEL, string ("\t Validating Po Interface NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_OVERRIDE"));
                        break;
                    }
                case NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID:
                    {
                        trace (TRACE_LEVEL_DEVEL, string ("\t Validating Po Interface NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID new/cur") + pNsmGlobalUpdatePoIntfMessage->getPoPartnerSystem() + pPoIntfManagedObject->getPoPartnerSystem());
		      string null_str = "";
                        /* If partner information already exists for the vLAG, then */
                        /* the partner information in this request must match */
                        if (pPoIntfManagedObject->getPoPartnerSystemPriority() != 0) {
                            if (pNsmGlobalUpdatePoIntfMessage->getPoPartnerSystemPriority() != 0) {
                                if (pNsmGlobalUpdatePoIntfMessage->getPoPartnerSystemPriority() !=
                                        pPoIntfManagedObject->getPoPartnerSystemPriority()) {
                                    pNsmGlobalUpdatePoIntfMessage->setReplyCode(VLAG_PARTNER_MISMATCH);
                                    trace(TRACE_LEVEL_DEVEL, "LACP Partner System Priority mismatch:");
                                    status = WRC_VLAG_REQUEST_FAILED;
                                    break;
                                }
                            }
                        }

                        if (pPoIntfManagedObject->getPoPartnerKey() != 0) {
                            if (pNsmGlobalUpdatePoIntfMessage->getPoPartnerKey() != 0) {
                                if (pNsmGlobalUpdatePoIntfMessage->getPoPartnerKey() !=
                                        pPoIntfManagedObject->getPoPartnerKey()) {
                                    pNsmGlobalUpdatePoIntfMessage->setReplyCode(VLAG_PARTNER_MISMATCH);
                                    trace(TRACE_LEVEL_DEVEL, "LACP Partner Key mismatch:");
                                    status = WRC_VLAG_REQUEST_FAILED;
                                    break;
                                }
                            }
                        }

                        if (pPoIntfManagedObject->getPoPartnerSystem().compare (null_str) != 0) {
                            if (pNsmGlobalUpdatePoIntfMessage->getPoPartnerSystem() != null_str) {
                               if (memcmp(pNsmGlobalUpdatePoIntfMessage->getPoPartnerSystem().c_str(),
                                       pPoIntfManagedObject->getPoPartnerSystem().c_str(), VLAG_LACP_GRP_ADDR_LEN) != 0) {
                                    pNsmGlobalUpdatePoIntfMessage->setReplyCode(VLAG_PARTNER_MISMATCH);
                                    trace(TRACE_LEVEL_DEVEL, string("LACP Partner System  mismatch:") + pNsmGlobalUpdatePoIntfMessage->getPoPartnerSystem() + pPoIntfManagedObject->getPoPartnerSystem());
                                    status = WRC_VLAG_REQUEST_FAILED;
                                    break;
                               }
                            }
                        }          

                        /* If the primary link RBridge has already been selected for the vLAG, */
                        /* the database is not updated. */
                        if (pPoIntfManagedObject->getPoPrimaryLinkDomainId() != 0) {
                            pNsmGlobalUpdatePoIntfMessage->setReplyCode(VLAG_PRIMARY_LINK_ALREADY_SELECTED);
                            trace(TRACE_LEVEL_DEVEL, "VLAG Primary Link already selected:");

                            status = WRC_VLAG_REQUEST_FAILED;
                            break;
                        }

                        break;
                    }
                case NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_CLEAR:
                    {
                        trace (TRACE_LEVEL_DEVEL, string ("\t Validating Po Interface NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_CLEAR"));
                        /* Only the RBridge that is holding the primary link is allowed to clear the column */
                        if( pPoIntfManagedObject->getPoPrimaryLinkDomainId() == 0) {
                            pNsmGlobalUpdatePoIntfMessage->setReplyCode(VLAG_PRIMARY_LINK_CLEARED);
                            trace(TRACE_LEVEL_DEVEL, "VLAG Primary Link RBridge Id already cleared");
                            status = WRC_VLAG_REQUEST_FAILED;
                            break;
                        }

                        if( pPoIntfManagedObject->getPoPrimaryLinkDomainId() != pNsmGlobalUpdatePoIntfMessage->getPoPrimaryLinkDomainId() ) {
                            pNsmGlobalUpdatePoIntfMessage->setReplyCode(VLAG_PRIMARY_LINK_NOT_SELECTED);
                            trace(TRACE_LEVEL_DEVEL, "Switch can not clear Primary Link RBridge Id:");
                            status = WRC_VLAG_REQUEST_FAILED;
                            break;
                        }

                        break;
                    }
                case NSMUPDATE_PO_RT_LACP_SYSTEM_ID:
                    {
                        /* Override current value.  It will always success */
                        trace (TRACE_LEVEL_DEVEL, string ("\t Validating Po Interface NSMUPDATE_PO_RT_LACP_SYSTEM_ID"));
                        break;
                    }
                case NSMUPDATE_PO_SET_IPV6_RA_GUARD:
                    {
                        /* Checking sw is enabled*/
                        if(!pPoIntfManagedObject->getSwModeFlag())
                        {
                            trace (TRACE_LEVEL_ERROR, string ("\t Interface not configured for switching "));
                            status = WRC_SWITCHING_NOT_ENABLED;
                        }
                        break;
                    }
                case ELD_INTF_PO_CONF_ELD_ENABLE:
                {
                     trace(TRACE_LEVEL_DEBUG, string("IntfConfGlobalWorker::validatePoIntfStep :ELD_INTF_PO_CONF_ELD_ENABLE:"));
                     vector<ObjectId>  m_eldVlanid = pPoIntfManagedObject->getVlanVector();
                     if (m_eldVlanid.size() > 0)
                     { 
                         trace(TRACE_LEVEL_ERROR,"IntfConfGlobalWorker::validatePoIntfStep:ELD can be enabled only on single vlan");
                         status =  WRC_ELD_SUPPORTED_ON_SINGLE_VLAN;
                     }
                     else  if (pPoIntfManagedObject->getPortStpConfig()->getStpEnable()) {
                        tracePrintf(TRACE_LEVEL_ERROR, "Spanning tree is enabled on Port-channel %u; cannot enable ELD.", poId);
                        status = WRC_NSM_ELD_STP_ENABLE_ERROR;
                    }
                    else if(true == PVlanUtils::isPrivateVlanMode(pNsmGlobalUpdatePoIntfMessage->getPoId()))
                    {
                        tracePrintf(TRACE_LEVEL_ERROR, "ELD configuration not allowed as interface %u is in private-vlan mode.", poId);
                        status = WRC_NSM_PVLAN_ELD_PMODE_PRESENT;
                    }
                }
                    break;
                case ELD_INTF_PO_CONF_PRIORITY:
                    trace(TRACE_LEVEL_DEBUG, string("ELD_INTF_PO_CONF_ELD_PRIORITY:"));
                    if (pPoIntfManagedObject->getPortStpConfig()->getStpEnable()) {
                        tracePrintf(TRACE_LEVEL_ERROR, "Spanning tree is enabled on Port-channel %u; cannot enable ELD.", poId);
                        status = WRC_NSM_ELD_STP_ENABLE_ERROR;
                    }

                    else if(true == PVlanUtils::isPrivateVlanMode(pNsmGlobalUpdatePoIntfMessage->getPoId()))
					{
                    	tracePrintf(TRACE_LEVEL_ERROR, "ELD configuration not allowed as interface %u is in private-vlan mode.", poId);
						status = WRC_NSM_PVLAN_ELD_PMODE_PRESENT;
					}

                    break;

                default:
                {
                	trace(TRACE_LEVEL_INFO,string("Reached Default"));
                	break;
                }
            }
            trace (TRACE_LEVEL_DEVEL, string ("\t cmdCode - status :  ") + pNsmGlobalUpdatePoIntfMessage->getCmdCode() + string ("--") + status);
        }

        pNsmGlobalUpdatePoIntfMessage->setCompletionStatus(status);

		pDceLinearSeqContext->executeNextStep(status);
        return status;
    }


/* This API can be used to remove native vlan id from allowed vlan list in the MO
 * while configuring Native vlan.
 */

int IntfConfGlobalWorker::deleteNativeVlanFromMoVlanList (NsmGlobalUpdatePoIntfMessage *pNsmGlobalUpdatePoIntfMessage, 
													PoIntfManagedObject *pPoIntfManagedObject)
{                             
		bool isVlanIdPresent = false;
		UI32 nativeVlanId = pNsmGlobalUpdatePoIntfMessage->getNativeVlanId();

		vector <UI32> vlanRangeVector;
		UI32Range vlanAddrange = UI32Range("");
		
		vlanAddrange = pPoIntfManagedObject->getVlanRangeAdd();
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
		pPoIntfManagedObject->setVlanRangeAdd(vlanAddrange);
	
		if (!isVlanIdPresent) {
			/* Remove vlan range */
			UI32Range vlanRemoverange = UI32Range("");
			vlanRemoverange = pPoIntfManagedObject->getVlanRangeRemove();
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
			pPoIntfManagedObject->setVlanRangeRemove(vlanRemoverange);
		}

	
		if (!isVlanIdPresent) {
			/* Except vlan range */
			UI32Range vlanExceptrange = UI32Range("");
			vlanExceptrange = pPoIntfManagedObject->getVlanRangeExcept();
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
			pPoIntfManagedObject->setVlanRangeExcept(vlanExceptrange);
		}
       
		if (isVlanIdPresent)
			return 1;
		return 0;
}

						 
/* This API can be used to remove native vlan id from user input allowed vlan list */
int IntfConfGlobalWorker::deleteNativeVlanFromMsgVlanList (NsmGlobalUpdatePoIntfMessage *pNsmGlobalUpdatePoIntfMessage,
												PoIntfManagedObject *pPoIntfManagedObject)
{                             
		bool isVlanIdPresent = false;
		vector<UI32>::iterator msgiter;
			
		trace (TRACE_LEVEL_INFO, string ("\t deleteNativeVlanFromMsgVlanList"));

		WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanIntfConfLocalManagedObject;
        pNativeVlanIntfConfLocalManagedObject = pPoIntfManagedObject->getNativeVlanIntfConfig();

		if(pNativeVlanIntfConfLocalManagedObject.operator->() == NULL) {
			return 0;
		}

		UI32 nativeVlanId = pNativeVlanIntfConfLocalManagedObject->getNativeVlanId();
		string vlanRangeStr;

		vector <UI32> vlanRangeVector;
		
		
		switch (pNsmGlobalUpdatePoIntfMessage->getCmdCode())
		{
			case NSMUPDATE_PO_SW_ADD_TRUNK_VLAN:
			{
				UI32Range vlanAddrange = UI32Range("");
				
				vlanAddrange = pNsmGlobalUpdatePoIntfMessage->getVlanRange();
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
				pNsmGlobalUpdatePoIntfMessage->setVlanRange(vlanAddrange);
			}
			break;

			case NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN:
			{
				/* Remove vlan range */
				UI32Range vlanRemoverange = UI32Range("");
		
				vlanRemoverange = pNsmGlobalUpdatePoIntfMessage->getVlanRange();
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
				pNsmGlobalUpdatePoIntfMessage->setVlanRange(vlanRemoverange);
			}
			break;
			
			case NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN:
			{
				/* Except vlan range */
				UI32Range vlanExceptrange = UI32Range("");
				vlanExceptrange = pNsmGlobalUpdatePoIntfMessage->getVlanRange();
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
				pNsmGlobalUpdatePoIntfMessage->setVlanRange(vlanExceptrange);
			}
			break;

			default :
			{
				trace (TRACE_LEVEL_INFO, string ("\t Unknown command code :: ") + 
								pNsmGlobalUpdatePoIntfMessage->getCmdCode());
			}
			break;
		}
		
		if (isVlanIdPresent)
			return 1;
		return 0;
}

void IntfConfGlobalWorker::updatePoIntfMOStep (DceLinearSeqContext *pDceLinearSeqContext)
    {
        NsmGlobalUpdatePoIntfMessage *pNsmGlobalUpdatePoIntfMessage = dynamic_cast<NsmGlobalUpdatePoIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_ERROR;
		status = updatePoIntfMO(pNsmGlobalUpdatePoIntfMessage,pDceLinearSeqContext);
		pDceLinearSeqContext->executeNextStep(status);
		return;
    }

    ResourceId IntfConfGlobalWorker::updatePoIntfMO(NsmGlobalUpdatePoIntfMessage *pNsmGlobalUpdatePoIntfMessage, 
            DceLinearSeqContext *pDceLinearSeqContext)
    {
		NSM_PLUG_DBG("Entering ..."); 
		vector<ObjectId>::iterator vlanid;
		UI32 j;
		UI32 native_vlanid = 0;
		ObjectId pVlanIntfObjId = ObjectId::NullObjectId;      
		vector<UI32>::iterator vid;
        ObjectId vlanOId;
        ResourceId  status   = WAVE_MESSAGE_SUCCESS;
		string null_str = "";

        NSM_PLUG_INFO_S(string("Update PoMO poName,CmdCode : ") + pNsmGlobalUpdatePoIntfMessage->getPoId() + string(",") + pNsmGlobalUpdatePoIntfMessage->getCmdCode());

        // This method is called from two different handlers, at this time only one of them uses cache..
        // this is temporary hack to restrain changes to Po specific, need to revisit

        PoIntfManagedObject* pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>(pDceLinearSeqContext->getCache()->getWaveManagedObject(PoIntfManagedObject::getClassName()));

        if (pPoIntfManagedObject == NULL) {
            UI32 poId = strtoul(pNsmGlobalUpdatePoIntfMessage->getPoName().c_str (), NULL, 0);
            status = NsmUtils::getPoMoByPoId(poId, pPoIntfManagedObject);
            NSM_PLUG_DBG_S("Retrieved PoIntfManagedObject from NsmUtils for poId " + poId);
            pDceLinearSeqContext->addManagedObjectForGarbageCollection(pPoIntfManagedObject);
        }

        if (pPoIntfManagedObject == NULL || WAVE_MESSAGE_ERROR == status) {
            NSM_PLUG_DBG_ERR_S(string("Po Entry not found poName :") + pNsmGlobalUpdatePoIntfMessage->getPoId());
            return (WAVE_MESSAGE_ERROR);
        } else {

            updateWaveManagedObject (pPoIntfManagedObject);

            prismAssert (NULL != pPoIntfManagedObject, __FILE__, __LINE__);

		    NativeVlanIntfConfLocalManagedObject *pNativeVlanIntfConfLocalManagedObject = NULL;

            switch(pNsmGlobalUpdatePoIntfMessage->getCmdCode())
            {
                case NSMUPDATE_PO_PORT_PROFILED : 
                    {
                        pPoIntfManagedObject->setIsPortProfiled (pNsmGlobalUpdatePoIntfMessage->getIsPortProfiled());
                        pPoIntfManagedObject->setStatus(!(pNsmGlobalUpdatePoIntfMessage->getIsPortProfiled()));
                        break;
                    }
                case NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PO :
                    {
                        AMPPProfileCompositeManagedObject* pAMPPProfileCompositeManagedObject =
                            new AMPPProfileCompositeManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
                        pAMPPProfileCompositeManagedObject->setPortProfileName(pNsmGlobalUpdatePoIntfMessage->getPortProfileName());
                        pPoIntfManagedObject->addPortProfileToInterface(pAMPPProfileCompositeManagedObject);
                        pPoIntfManagedObject->setStatus(false);
                        break;
                    }
                case NSMUPDATE_DISSOCIATE_PORT_PROFILE_WITH_PO :
                    {
                        vector<WaveManagedObjectPointer<AMPPProfileCompositeManagedObject> > associatedPortProfiles
                            = pPoIntfManagedObject->getAssociatedPortProfiles();
                        //If the already associated port-profiles is 1 and dissociate request is success
                        //port is becoming a non-port-profile-port. Do reset the status of interface.
                        if(1 == associatedPortProfiles.size()) {
                            pPoIntfManagedObject->setStatus(true);
                        }
                        pPoIntfManagedObject->removePortProfileFromInterface(pNsmGlobalUpdatePoIntfMessage->getPortProfileName());
                        break;
                    }
                case NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PO :
                    {
                        AMPPProfileDomainCompositeManagedObject* pAMPPProfileDomainCompositeManagedObject =
                            new AMPPProfileDomainCompositeManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
                        pAMPPProfileDomainCompositeManagedObject->setPortProfileDomainName
                            (pNsmGlobalUpdatePoIntfMessage->getPortProfileDomainName());
                        pPoIntfManagedObject->setAssociatedPortProfileDomain(pAMPPProfileDomainCompositeManagedObject);
                        pPoIntfManagedObject->setStatus(false);
                        break;
                    }
                case NSMUPDATE_DISSOCIATE_PORT_PROFILE_DOMAIN_WITH_PO :
                    {
                        pPoIntfManagedObject->setAssociatedPortProfileDomain
                            (WaveManagedObjectPointer<AMPPProfileDomainCompositeManagedObject>(NULL));
                        pPoIntfManagedObject->setStatus(true);
                        break;
                    }
                
                case NSMUPDATE_PO_DESC :
                case NSMUPDATE_PO_DELDESC :
                    NSM_PLUG_INFO_S(string("Desc / NoDesc : ") + pNsmGlobalUpdatePoIntfMessage->getDesc()); 
                    pPoIntfManagedObject->setDesc (pNsmGlobalUpdatePoIntfMessage->getDesc());
                    break;
				case NSMUPDATE_PO_MTU :
				case NSMUPDATE_PO_DELMTU:
                    NSM_PLUG_INFO_S(string("Mtu / setMtu : ") + pNsmGlobalUpdatePoIntfMessage->getMtu()); 
                    pPoIntfManagedObject->setMtu(
                            pNsmGlobalUpdatePoIntfMessage->getMtu());
					break;
				case NSMUPDATE_PO_MINLINKS :
				case NSMUPDATE_PO_DELMINLINKS:
                    NSM_PLUG_INFO_S(string("MinLinks / setPoMinLinks : ") + pNsmGlobalUpdatePoIntfMessage->getPoMinLinks()); 
                    pPoIntfManagedObject->setPoMinLinks(
                            pNsmGlobalUpdatePoIntfMessage->getPoMinLinks());
					break;
				case NSMUPDATE_PO_PVLAN_MODE_TRUNK:
				case NSMUPDATE_PO_PVLAN_MODE_TRUNK_HOST:
				case NSMUPDATE_PO_PVLAN_MODE_TRUNK_PROMISCUOUS:
				case NSMUPDATE_PO_PVLAN_MODE_PROMISCUOUS:
				case NSMUPDATE_PO_PVLAN_MODE_HOST:
				{
					pPoIntfManagedObject->cleanUpPvlanAll();
					pPoIntfManagedObject->cleanUpAccessTrunkAll();
					pPoIntfManagedObject->setNativeVlanIntfConfig (WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (NULL));

					if(pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_PVLAN_MODE_TRUNK){
						pPoIntfManagedObject->setPvlanModeTrunkBas(true);
						trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW NSMUPDATE_PO_PVLAN_MODE_TRUNK  "));

					}else if(pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_PVLAN_MODE_TRUNK_PROMISCUOUS){

						pPoIntfManagedObject->setPvlanModeTrunkPromis(true);
						trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW NSMUPDATE_PO_PVLAN_MODE_TRUNK_PROMISCUOUS  "));

					}else if(pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_PVLAN_MODE_TRUNK_HOST){

						pPoIntfManagedObject->setPvlanModeTrunkHost(true);
						trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW NSMUPDATE_PO_PVLAN_MODE_TRUNK_HOST "));

					}
					else if(pNsmGlobalUpdatePoIntfMessage->getCmdCode() == NSMUPDATE_PO_PVLAN_MODE_PROMISCUOUS){

						pPoIntfManagedObject->setPvlanModePromiscuous(true);
						trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW NSMUPDATE_PO_PVLAN_MODE_PROMISCUOUS "));

					}else{
						pPoIntfManagedObject->setPvlanModeHost(true);
						trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW NSMUPDATE_PO_PVLAN_MODE_HOST "));

					}
                    pPoIntfManagedObject->deleteMacAddressVlanClassificationAll();
                    pPoIntfManagedObject->deleteMacGroupVlanClassificationAll();
                    pPoIntfManagedObject->deleteCtagVlanClassificationAll();
                    pPoIntfManagedObject->deleteCtagPVlanClassificationAll();
					break;
				}


				case NSMUPDATE_PO_PVLAN_NATIVE_VLANID:
				case NSMUPDATE_PO_PVLAN_NATIVE_VLANID_DISABLE:
                    pPoIntfManagedObject->setPvlanNativeVlanId
						(pNsmGlobalUpdatePoIntfMessage->getPvlanNativeVlanId());
                    trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW nativeVlan ")+pNsmGlobalUpdatePoIntfMessage->getPvlanNativeVlanId());
					break;

				 case NSMUPDATE_PO_PVLAN_HOST_ASSOCIATION:
				 case NSMUPDATE_PO_DELPVLAN_HOST_ASSOCIATION_ALL:
				 {
					 pPoIntfManagedObject->setPvlanHostPrimVlanId(pNsmGlobalUpdatePoIntfMessage->getPvlanHostPrimVlanId());
					 pPoIntfManagedObject->setPvlanHostSecVlanId(pNsmGlobalUpdatePoIntfMessage->getPvlanHostSecVlanId());
					 pPoIntfManagedObject->addAttributeToBeUpdated("pVlanHostPrimVlanId");
					 pPoIntfManagedObject->addAttributeToBeUpdated("pVlanHostSecVlanId");
					 trace(TRACE_LEVEL_INFO,string("NSMUPDATE_PO_PVLAN_HOST_ASSOCIATION nativeVlan ")+pNsmGlobalUpdatePoIntfMessage->getPvlanHostPrimVlanId());
					 break;
				 }
				
				case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ALL:

					NSM_PLUG_INFO_S(string(" sw private-vlan trunk allowed vlan all...."));
					pPoIntfManagedObject->setPvlanIsAllowedVlanNone(false);
					pPoIntfManagedObject->setPvlanRangeAdd(UI32Range(""));
					pPoIntfManagedObject->setPvlanRangeRemove(UI32Range(""));
					pPoIntfManagedObject->setPvlanRangeExcept(UI32Range(""));
					pPoIntfManagedObject->setPvlanIsAllowedVlanAll
						(pNsmGlobalUpdatePoIntfMessage->getPvlanIsAllowedVlanAll());
					break;
					
				case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_NONE:
				{
					NSM_PLUG_INFO_S(string(" sw private-vlan trunk allowed vlan none...."));
					pPoIntfManagedObject->setPvlanIsAllowedVlanAll(false);
					pPoIntfManagedObject->setPvlanRangeAdd(UI32Range(""));
					pPoIntfManagedObject->setPvlanRangeRemove(UI32Range(""));
					pPoIntfManagedObject->setPvlanRangeExcept(UI32Range(""));
					pPoIntfManagedObject->setPvlanIsAllowedVlanNone
						(pNsmGlobalUpdatePoIntfMessage->getPvlanIsAllowedVlanAll());

					VlanIdList vlist (pPoIntfManagedObject->getPvlanNativeVlanId());
					INTF_XSTP_GLOBAL_WORKER->handleIntfVlanAssocUpdate(
					        pPoIntfManagedObject, vlist, pDceLinearSeqContext, false);

					break;
				}
				case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ADD:
					{
						NSM_PLUG_INFO_S(string(" sw private-vlan trunk allowed vlan add...."));
						vector<UI32>::iterator msgiter;
						vector <UI32> msgVlanRangeVector;
						UI32Range msgVlanrange = UI32Range("");
						
						msgVlanrange = pNsmGlobalUpdatePoIntfMessage->getPvlanRange();
						msgVlanrange.getUI32RangeVector(msgVlanRangeVector);
						msgVlanrange = UI32Range("");
						
						if (pPoIntfManagedObject->getPvlanIsAllowedVlanAll()) {
							/*Check the remove list*/
							UI32Range moRemoveVlanrange = UI32Range("");
							vector <UI32> moRemoveVlanRangeVector;
							vector<UI32>::iterator moremiter;

							moRemoveVlanrange = pPoIntfManagedObject->getPvlanRangeRemove();
							moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
							moRemoveVlanrange = UI32Range("");
							trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW moRemoveVlanrange "));

							for (msgiter = msgVlanRangeVector.begin();
								msgiter != msgVlanRangeVector.end(); msgiter++) {
								trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW msgiter "));

								for (moremiter = moRemoveVlanRangeVector.begin();
									moremiter != moRemoveVlanRangeVector.end(); moremiter++) {

									if (*moremiter == *msgiter) {
									/*Found in remove list, so now erase the element from
									 * the remove vector*/
										moRemoveVlanRangeVector.erase(moremiter);
										trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW moremiter "));
										break;
									}
								}
							}

							string vlanRemoveRangeStr =
								UI32Range::getUI32RangeStringFromVector (moRemoveVlanRangeVector);

							NSM_PLUG_INFO_S (string
								(" case sw private-vlan allowed all-remove range str : ") + 
								vlanRemoveRangeStr);
							trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW moRemoveVlanrange "));
							moRemoveVlanrange.fromString(vlanRemoveRangeStr); 
							pPoIntfManagedObject->setPvlanRangeRemove(moRemoveVlanrange);
							pPoIntfManagedObject->setPvlanRangeAdd(UI32Range(""));
							pPoIntfManagedObject->setPvlanRangeExcept(UI32Range(""));

						 } else {

							 vector <UI32> moVlanRangeVector;
							 UI32Range moVlanrange = UI32Range("");

							 moVlanrange = pPoIntfManagedObject->getPvlanRangeAdd();
							 moVlanrange.getUI32RangeVector(moVlanRangeVector);
							 moVlanrange = UI32Range("");
							 trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW moVlanrange "));
							 vector<UI32>::iterator moiter;
							 vector<UI32>::iterator moindex = moVlanRangeVector.begin();
							 bool isVlanIdPresent = true;

							for (msgiter=msgVlanRangeVector.begin();
								msgiter != msgVlanRangeVector.end(); msgiter++) {
								
			 					for (moiter=moVlanRangeVector.begin();
									moiter != moVlanRangeVector.end(); moiter++) {
			 						 trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW moiter "));
				 					if(*moiter == *msgiter) {
					 					break;
					 				} else if(*moiter > *msgiter) {
					 				/* Since this is a sorted vector, and we have already exceeded
									 * the required VLAN ID, we cannot find it in further parses,
									 * so break */
						 				moindex = moVlanRangeVector.begin();
					 					isVlanIdPresent = false;
					 					break;
				 					}
			 					}
							
					 			if (!isVlanIdPresent)
					 				while (*moindex < *msgiter)
					 				moindex++;
							
						 		if (moiter == moVlanRangeVector.end()) {
							 		isVlanIdPresent = false;
							 		moindex = moVlanRangeVector.end();
						 		}
							
								ResourceId vlanstatus = WAVE_MESSAGE_ERROR;
								/* Vlan not found in MO, add it at appropriate position to keep 
							 	* the vector sorted*/
				 				if (!isVlanIdPresent) {
					     			UI32 vlan_id = *msgiter;
						 			WaveManagedObjectSynchronousQueryContext 
										vlanMOSyncQueryCtxt(VlanIntfManagedObject::getClassName());
							 		vlanMOSyncQueryCtxt.addAndAttribute
										(new AttributeUI32(&vlan_id,"id"));
							 		vector<WaveManagedObject *> *pVlanMOResults = querySynchronously
										(&vlanMOSyncQueryCtxt);
							 		pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*pVlanMOResults);

							 		VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

							 		if (NULL != pVlanMOResults) {
								 		UI32 numberOfResults = pVlanMOResults->size ();
									
								 		if (0 == numberOfResults) {
									 		vlanstatus = WAVE_MESSAGE_ERROR;
							 			} else if (1 == numberOfResults) {
									 		pVlanIntfManagedObject = 
												dynamic_cast<VlanIntfManagedObject *>
													((*pVlanMOResults)[0]);

									 		prismAssert (NULL != pVlanIntfManagedObject, __FILE__, __LINE__);

								 			vlanstatus = WAVE_MESSAGE_SUCCESS;
							 			} else {
								 			vlanstatus = WAVE_MESSAGE_ERROR;
							 			}
							 			pVlanMOResults->clear ();
								 		delete pVlanMOResults;
							 		}
						 		}
							
					 			if (vlanstatus == WAVE_MESSAGE_SUCCESS)
				       				moVlanRangeVector.insert(moindex, *msgiter);
						 			moindex = moVlanRangeVector.begin();
							 		isVlanIdPresent = true;
			 				}
						
				 			string vlanRangeStr = 
								UI32Range::getUI32RangeStringFromVector(moVlanRangeVector);
				 			trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW vlanRangeStr ")+vlanRangeStr);
 	                		NSM_PLUG_INFO_S(string
								("sw private-vlan trunk allowed vlan add: add-str:")+ vlanRangeStr);

					 		moVlanrange.fromString(vlanRangeStr); 
							pPoIntfManagedObject->setPvlanRangeAdd(moVlanrange);
							pPoIntfManagedObject->setPvlanIsAllowedVlanNone(false);
							pPoIntfManagedObject->setPvlanRangeRemove(UI32Range(""));
							pPoIntfManagedObject->setPvlanRangeExcept(UI32Range(""));
						}
						break;
					}
					
				case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_REMOVE:
					{
						NSM_PLUG_INFO_S(string(" sw private-vlan trunk allowed vlan remove...."));

						vector<UI32>::iterator moindex;
						vector<UI32>::iterator msgiter;
						vector <UI32> msgVlanRangeVector;
						UI32Range msgVlanrange = UI32Range("");
						bool isVlanIdPresent = true;
						VlanIdList vlist;

						msgVlanrange = pNsmGlobalUpdatePoIntfMessage->getPvlanRange();
						msgVlanrange.getUI32RangeVector(msgVlanRangeVector);
						msgVlanrange = UI32Range("");

						if (pPoIntfManagedObject->getPvlanIsAllowedVlanAll()) {
							/*Check the remove list*/

							UI32Range moRemoveVlanrange = UI32Range("");
							vector <UI32> moRemoveVlanRangeVector;
							vector<UI32>::iterator moremiter;

							moindex = moRemoveVlanRangeVector.begin();
							moRemoveVlanrange = pPoIntfManagedObject->getPvlanRangeRemove();
							moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
							moRemoveVlanrange = UI32Range("");
							trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW inrem "));
							for (msgiter=msgVlanRangeVector.begin();
								msgiter != msgVlanRangeVector.end(); msgiter++) {

								for (moremiter=moRemoveVlanRangeVector.begin();
									moremiter != moRemoveVlanRangeVector.end(); moremiter++) {

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
									while(*moindex < *msgiter)
									moindex++;

								if (moremiter == moRemoveVlanRangeVector.end()) {
									isVlanIdPresent = false;
									moindex = moRemoveVlanRangeVector.end();
								}

								if (!isVlanIdPresent)
								/*Vlan not found in MO, add it at appropriate position to keep
								 * the vector sorted*/
								moRemoveVlanRangeVector.insert(moindex, *msgiter);
								moindex = moRemoveVlanRangeVector.begin();
								isVlanIdPresent = true;
							}

							string vlanRemoveRangeStr =
								UI32Range::getUI32RangeStringFromVector(moRemoveVlanRangeVector);
							moRemoveVlanrange.fromString(vlanRemoveRangeStr); 
							trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW vlanRemoveRangeStr ")+vlanRemoveRangeStr);
							pPoIntfManagedObject->setPvlanRangeRemove(moRemoveVlanrange);
							pPoIntfManagedObject->setPvlanRangeAdd(UI32Range(""));
							pPoIntfManagedObject->setPvlanRangeExcept(UI32Range(""));

							vlist.m_vlanIds = moRemoveVlanRangeVector;
							vlist.m_type = NONMEMBER_VLAN_LIST;

						} else {

							vector <UI32> moVlanRangeVector;
							UI32Range moVlanrange = UI32Range("");

							moVlanrange = pPoIntfManagedObject->getPvlanRangeAdd();
							moVlanrange.getUI32RangeVector(moVlanRangeVector);
							moVlanrange = UI32Range("");

							vector<UI32>::iterator moiter;
							moindex = moVlanRangeVector.begin();

							for (msgiter=msgVlanRangeVector.begin();
								msgiter != msgVlanRangeVector.end(); msgiter++) {

								for (moiter=moVlanRangeVector.begin();
									moiter != moVlanRangeVector.end(); moiter++) {
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
							trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW vlanRangeStr ")+vlanRangeStr);
							pPoIntfManagedObject->setPvlanRangeAdd(moVlanrange);
							pPoIntfManagedObject->setPvlanRangeRemove(UI32Range(""));
							pPoIntfManagedObject->setPvlanRangeExcept(UI32Range(""));

							vlist.m_vlanIds = moVlanRangeVector;
							if (pPoIntfManagedObject->m_pVlanNativeVlanId != 0) {
							    vlist.m_vlanIds.push_back(pPoIntfManagedObject->m_pVlanNativeVlanId);
							}
						}

						INTF_XSTP_GLOBAL_WORKER->handleIntfVlanAssocUpdate(
						        pPoIntfManagedObject, vlist, pDceLinearSeqContext, false);
						break;
					}

				case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_EXCEPT:
					{
						NSM_PLUG_INFO_S(string(" sw private-vlan trunk allowed vlan except...."));

						UI32Range msgVlanrange = UI32Range("");
													
						msgVlanrange = pNsmGlobalUpdatePoIntfMessage->getPvlanRange();

						pPoIntfManagedObject->setPvlanIsAllowedVlanAll(true);
						pPoIntfManagedObject->setPvlanIsAllowedVlanNone(false);
						pPoIntfManagedObject->setPvlanRangeRemove(msgVlanrange);
						pPoIntfManagedObject->setPvlanRangeAdd(UI32Range(""));
						pPoIntfManagedObject->setPvlanRangeExcept(UI32Range(""));

						VlanIdList vlist (msgVlanrange, NONMEMBER_VLAN_LIST);
						INTF_XSTP_GLOBAL_WORKER->handleIntfVlanAssocUpdate(
						        pPoIntfManagedObject, vlist, pDceLinearSeqContext, false);
						break;
					}
				case NSMUPDATE_PO_ALLOWEDSPEED :
					pPoIntfManagedObject->setPoAllowedSpeed (pNsmGlobalUpdatePoIntfMessage->getPoAllowedSpeed());
					break;

				case NSMUPDATE_PO_LOADBALANCE:
				case NSMUPDATE_PO_DELLOADBALANCE:
					 NSM_PLUG_INFO_S(string("IntfConfGlobalWorker::updatePoIntfMO setPoLoadBalanceMode : ") + pNsmGlobalUpdatePoIntfMessage->getPoLoadBalanceMode()); 
                    pPoIntfManagedObject->setPoLoadBalanceMode(pNsmGlobalUpdatePoIntfMessage->getPoLoadBalanceMode());
					break;
                    
                case NSMUPDATE_PO_SW :
                    {
                        NSM_PLUG_INFO_S(string(" SW ")); 
                        pPoIntfManagedObject->setSwModeFlag (pNsmGlobalUpdatePoIntfMessage->getSwModeFlag());
                        (NsmGlobalObjectManager::getInstance()->getIntfXstpGlobalConfWorker())->createPortXstp(pPoIntfManagedObject); 
                        /* Default access enty added */                                                                                      ObjectId vlanObjectId;
                        getVlanIntfObjectIdByVlanIfName (NSM_NATIVE_VLAN_DEFAULT, vlanObjectId,false,pDceLinearSeqContext);
                        pPoIntfManagedObject->setAccessVlanObjectId (vlanObjectId);

                        pPoIntfManagedObject->setSwMode(SW_MODE_ACCESS);
						pPoIntfManagedObject->setVlanRangeAdd(UI32Range(""));
						pPoIntfManagedObject->setVlanRangeRemove(UI32Range(""));
						pPoIntfManagedObject->setVlanRangeExcept(UI32Range(""));
                        break;
                    }
                case NSMUPDATE_PO_DELSW :
                    {
                        pPoIntfManagedObject->setSwModeFlag (pNsmGlobalUpdatePoIntfMessage->getSwModeFlag());

						// delete port vlan stp mo
						if (!pNsmGlobalUpdatePoIntfMessage->getSwModeFlag())
						{
						    INTF_XSTP_GLOBAL_WORKER->deletePortXstp(pPoIntfManagedObject, pDceLinearSeqContext);

							pPoIntfManagedObject-> deleteAllVlanClassifierActivatedGroup();

						}

                        pPoIntfManagedObject->setSwMode(SW_MODE_UNKNOWN);

                        /* Delete trunk mode entries */
                        pPoIntfManagedObject->setIsAllowedVlanAll(false);

						pPoIntfManagedObject->setNativeVlanIntfConfig (WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (NULL));

                        /* Delete access mode entries */
                        pPoIntfManagedObject->setAccessVlanObjectId(ObjectId::NullObjectId);
						pPoIntfManagedObject->setVlanRangeAdd(UI32Range(""));
						pPoIntfManagedObject->setVlanRangeRemove(UI32Range(""));
						pPoIntfManagedObject->setVlanRangeExcept(UI32Range(""));
						trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW del "));
						pPoIntfManagedObject->cleanUpPvlanAll();
						 /* ELD vlan-list disabling for "no sw" */

						vector<ObjectId>  m_vlanid = pPoIntfManagedObject->getVlanVector();
						trace (TRACE_LEVEL_INFO, string ("CLI: no switchport; for logical interface ") + pNsmGlobalUpdatePoIntfMessage->getPoName().c_str());
						if (m_vlanid.size() > 0) {
							trace (TRACE_LEVEL_INFO, string ("CLI: no switchport;  Remove ELD vlans."));
							for (vlanid = m_vlanid.begin(); vlanid != m_vlanid.end(); vlanid++) {
								 trace (TRACE_LEVEL_DEBUG, string ("CLI: no switchport;  Removing ELD vlan."));
								pPoIntfManagedObject->deleteVlanObjectId(*vlanid);
							}
						}
//                        pPoIntfManagedObject->setEldPrio (128);
                         /* ELD vlan-list disabling for "no sw" */
						pPoIntfManagedObject->setRaGuard(false);
                    	pPoIntfManagedObject->deleteMacAddressVlanClassificationAll();
                    	pPoIntfManagedObject->deleteMacGroupVlanClassificationAll();
                    	pPoIntfManagedObject->deleteCtagVlanClassificationAll();
                    	pPoIntfManagedObject->deleteCtagPVlanClassificationAll();

						/* Clear port-security configuration */
						PortSecurityLocalManagedObject *pPortSecMO = (pPoIntfManagedObject->getPortSecConfig()).operator->();
                		if (pPortSecMO != NULL) {
                    		set<string> compositionSet;
                    		compositionSet.insert("portSecurity");
                    		deleteManagedObjectCompositions(pPoIntfManagedObject->getObjectId(), compositionSet);
                    		pPoIntfManagedObject->addAttributeToBeUpdated("portSecurity");
                    		updateWaveManagedObject(pPoIntfManagedObject);
                		}

                        NSM_PLUG_INFO_S(string(" No Sw ")); 
                        break;
                    }
            case NSMUPDATE_PO_CHANNELGROUP_FIRSTMEMBER:        
            {
            	// these 3 cases come as part of single cmd, so keeping break for NSMUPDATE_PO_POTYPE only
                    NSM_PLUG_INFO_S(string(" NSMUPDATE_PO_CHANNELGROUP_FIRSTMEMBER DomainId,MacAddress :  ") + pNsmGlobalUpdatePoIntfMessage->getPoLacpSystemIdDomainId() + string (",") + pNsmGlobalUpdatePoIntfMessage->getPoLacpSystemIdDomainId());  
                    pPoIntfManagedObject->setPoLacpSystemId(pNsmGlobalUpdatePoIntfMessage->getPoLacpSystemId());
                    pPoIntfManagedObject->setPoLacpSystemIdDomainId(pNsmGlobalUpdatePoIntfMessage->getPoLacpSystemIdDomainId());
            }
            case NSMUPDATE_PO_POMODE :
            {
                    NSM_PLUG_INFO_S(string(" PoMode :  ") + pNsmGlobalUpdatePoIntfMessage->getPoMode()); 
                    pPoIntfManagedObject->setPoMode(pNsmGlobalUpdatePoIntfMessage->getPoMode());
            }
            case NSMUPDATE_PO_POTYPE :
            {
                    NSM_PLUG_INFO_S(string(" PoType :  ") + pNsmGlobalUpdatePoIntfMessage->getPoType()); 
                    pPoIntfManagedObject->setPoType(pNsmGlobalUpdatePoIntfMessage->getPoType());
                    break;
            }
            case NSMUPDATE_PO_MODE :
            case NSMUPDATE_PO_DELMODE:
					{

						NSM_PLUG_INFO_S(string(" sw Mode :  ") + pNsmGlobalUpdatePoIntfMessage->getSwMode()); 
						if(pNsmGlobalUpdatePoIntfMessage->getSwMode() == SW_MODE_ACCESS )
						{
							/* Default access enty added */                                                                                      ObjectId vlanObjectId;
							getVlanIntfObjectIdByVlanIfName (NSM_NATIVE_VLAN_DEFAULT, vlanObjectId, false, pDceLinearSeqContext);
							pPoIntfManagedObject->setAccessVlanObjectId (vlanObjectId);

							/* Delete trunk mode entries */
							pPoIntfManagedObject->setIsAllowedVlanAll(false);


						} else if ( pNsmGlobalUpdatePoIntfMessage->getSwMode() == SW_MODE_TRUNK ) {
							/* Delete access mode entreis */
							pPoIntfManagedObject->setAccessVlanObjectId(ObjectId::NullObjectId);
							pPoIntfManagedObject->deleteAllVlanClassifierActivatedGroup();
						}

						if(pPoIntfManagedObject->getSwMode() != SW_MODE_ACCESS &&
							pNsmGlobalUpdatePoIntfMessage->getSwMode() == SW_MODE_ACCESS)
						{/*Clear the trunk allowed vlan list*/
							pPoIntfManagedObject->setVlanRangeAdd(UI32Range(""));
							pPoIntfManagedObject->setVlanRangeRemove(UI32Range(""));
							pPoIntfManagedObject->setVlanRangeExcept(UI32Range(""));

							/* Clear the native vlan config and set it to default*/
							pPoIntfManagedObject->setNativeVlanIntfConfig(
								WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (NULL));
                    		pPoIntfManagedObject->deleteCtagVlanClassificationAll();
                    		pPoIntfManagedObject->deleteCtagPVlanClassificationAll();
						}

						if(pPoIntfManagedObject->getSwMode() != SW_MODE_TRUNK &&
							pNsmGlobalUpdatePoIntfMessage->getSwMode() == SW_MODE_TRUNK) {
							/* set the native vlan config */
							pNativeVlanIntfConfLocalManagedObject = 
								new NativeVlanIntfConfLocalManagedObject( 
									dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
							pNativeVlanIntfConfLocalManagedObject->setNativevlanDisable(true);
							pNativeVlanIntfConfLocalManagedObject->setNativeVlanId(NSM_NATIVE_VLAN_DEFAULT);
							pNativeVlanIntfConfLocalManagedObject->setNativevlanPresence(true);
							pPoIntfManagedObject->setNativeVlanIntfConfig(
								WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (
									pNativeVlanIntfConfLocalManagedObject));
                    		pPoIntfManagedObject->deleteMacAddressVlanClassificationAll();
                    		pPoIntfManagedObject->deleteMacGroupVlanClassificationAll();
                    		pPoIntfManagedObject->deleteCtagPVlanClassificationAll();
						}

						pPoIntfManagedObject->setSwMode(pNsmGlobalUpdatePoIntfMessage->getSwMode());

						// Ideally, we should retain settings for vlan 1.. However backend is not doing it!
						// so, removing everything..
						INTF_XSTP_GLOBAL_WORKER->clearXstpPortInstAndPortVlanMOs(pPoIntfManagedObject, false);

//						VlanIdList vlist (1);
//						INTF_XSTP_GLOBAL_WORKER->handleIntfVlanAssocUpdate(
//						        pPoIntfManagedObject, vlist, pDceLinearSeqContext, false);

						//PVLAN handling- cleaning all Pvlan enteries
						pPoIntfManagedObject->cleanUpPvlanAll();

						/* ELD vlan-list disabling  for changing mode */

						vector<ObjectId>  m_vlanid = pPoIntfManagedObject->getVlanVector();
						trace (TRACE_LEVEL_INFO, string ("Changing mode for logical interface ") + pNsmGlobalUpdatePoIntfMessage->getPoName().c_str());
						if (m_vlanid.size() > 0) 
						{
							trace (TRACE_LEVEL_INFO, string ("Changing mode for physical interface;  Remove ELD vlans."));
							for (vlanid = m_vlanid.begin(); vlanid != m_vlanid.end(); vlanid++) {
								trace (TRACE_LEVEL_DEBUG, string ("CLI: no switchport;  Removing ELD vlan."));
								pPoIntfManagedObject->deleteVlanObjectId(*vlanid);
							}
						}
//                        pPoIntfManagedObject->setEldPrio (128);
						/* ELD vlan-list disabling  for changing mode */
						break;
					}

                case NSMUPDATE_PO_STATUS :
                case NSMUPDATE_PO_DELSTATUS :
                case NSMUPDATE_PO_EXCEPTION_STATUS :
                    NSM_PLUG_INFO_S(string(" status/delstatus :  ") + pNsmGlobalUpdatePoIntfMessage->getFlag());
                    pPoIntfManagedObject->setStatus (pNsmGlobalUpdatePoIntfMessage->getFlag());
                    break;
				case NSMUPDATE_PO_IGNORE_SPLIT :
				case NSMUPDATE_PO_NO_IGNORE_SPLIT :
					pPoIntfManagedObject->setIgnoreSplit (pNsmGlobalUpdatePoIntfMessage->getIgnoreSplit());
					NSM_PLUG_INFO_S(string(" IntfConfGlobalWorker::updatePoIntfMO - ignore_split :  ") + pPoIntfManagedObject->getIgnoreSplit());
					break;
                case NSMUPDATE_PO_SW_ADD_ACCESS_VLAN:
                    {
                        ObjectId vlanObjectId;
						//ObjectId preVlanObjectId = pPoIntfManagedObject->getAccessVlanObjectId();
                        trace (TRACE_LEVEL_INFO, string ("\t Sw VlanName :: ") + 
					     	(pNsmGlobalUpdatePoIntfMessage->getVlanRange()).toString());
                        getVlanIntfObjectIdByVlanIfName (atoi(pNsmGlobalUpdatePoIntfMessage->getVlanRange().toString().c_str()), vlanObjectId, 
														  false, pDceLinearSeqContext);
                        pPoIntfManagedObject->setAccessVlanObjectId (vlanObjectId);

                        INTF_XSTP_GLOBAL_WORKER->clearXstpPortInstAndPortVlanMOs(pPoIntfManagedObject, false);

//                        VlanIdList vlist (vlanObjectId);
//                        INTF_XSTP_GLOBAL_WORKER->handleIntfVlanAssocUpdate(
//                                pPoIntfManagedObject, vlist, pDceLinearSeqContext, false);
						 
						/* ELD vlan-list disabling  for change of access vlan */

						 vector<ObjectId>  m_vlanid = pPoIntfManagedObject->getVlanVector();
						 trace (TRACE_LEVEL_INFO, string ("Changing access vlan for logical interface ") + pNsmGlobalUpdatePoIntfMessage->getPoName().c_str());
						 if (m_vlanid.size() > 0)
						 {
							 trace (TRACE_LEVEL_INFO, string ("Changing access vlan for physical interface; Remove ELD vlans. "));
							 for (vlanid = m_vlanid.begin(); vlanid != m_vlanid.end(); vlanid++) {
								 trace (TRACE_LEVEL_DEBUG, string ("CLI: no switchport;  Removing ELD vlan."));
								 pPoIntfManagedObject->deleteVlanObjectId(*vlanid);
							 }
						 }
//                        pPoIntfManagedObject->setEldPrio (128);
						/* ELD vlan-list disabling  for change of access vlan */
                        break;
                    }
                case NSMUPDATE_PO_SW_DELETE_ACCESS_VLAN:
                    {
                        NSM_PLUG_INFO_S(string(" del access vlan :  ") );

                        ObjectId vlanObjectId;
                        ObjectId preVlanObjectId;
						
						preVlanObjectId = pPoIntfManagedObject->getAccessVlanObjectId();
                        getVlanIntfObjectIdByVlanIfName (DCM_NSM_VLAN_DEFAULT_VID, vlanObjectId, false, pDceLinearSeqContext);
                        pPoIntfManagedObject->setAccessVlanObjectId (vlanObjectId);

						// delete port vlan stp mo
						if (preVlanObjectId != vlanObjectId) {
							VlanIdList vlist (vlanObjectId);
							INTF_XSTP_GLOBAL_WORKER->handleIntfVlanAssocUpdate(
							        pPoIntfManagedObject, vlist, pDceLinearSeqContext, false);
						}
                        break;
                    }
                case NSMUPDATE_PO_SW_TRUNK_VLAN_ALL:
					 {
                         NSM_PLUG_INFO_S(string(" sw trunk all : ") +  pNsmGlobalUpdatePoIntfMessage->getIsAllowedVlanAll());
						 pPoIntfManagedObject->setIsAllowedVlanAll(pNsmGlobalUpdatePoIntfMessage->getIsAllowedVlanAll());
						 pPoIntfManagedObject->setIsAllowedVlanNone(false);
						 pPoIntfManagedObject->setVlanRangeAdd(UI32Range(""));
						 pPoIntfManagedObject->setVlanRangeRemove(UI32Range(""));
						 pPoIntfManagedObject->setVlanRangeExcept(UI32Range(""));
						 break;
					 }
                case NSMUPDATE_PO_SW_TRUNK_VLAN_NONE:
					 {
                         NSM_PLUG_INFO_S(string(" sw trunk none ") );
						 pPoIntfManagedObject->setIsAllowedVlanNone(false);
						 pPoIntfManagedObject->setIsAllowedVlanAll(false);
						 pPoIntfManagedObject->setVlanRangeAdd(UI32Range(""));
						 pPoIntfManagedObject->setVlanRangeRemove(UI32Range(""));
						 pPoIntfManagedObject->setVlanRangeExcept(UI32Range(""));

						 vector<ObjectId>  m_vlanid = pPoIntfManagedObject->getVlanVector();

						 if (m_vlanid.size() > 0)
						 {
							WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanIntfConfLocalManagedObjectpo;
							pNativeVlanIntfConfLocalManagedObjectpo = pPoIntfManagedObject->getNativeVlanIntfConfig();
							if(pNativeVlanIntfConfLocalManagedObjectpo.operator->() == NULL) {
								return 0;
							}

							UI32 nativeVlanId = pNativeVlanIntfConfLocalManagedObjectpo->getNativeVlanId();
							trace(TRACE_LEVEL_DEBUG, string("IntfConfGlobalWorker::updatePoIntfMO:Checking for ELD-records for native vlan ") + nativeVlanId);
							getVlanIntfObjectIdByVlanIfName(nativeVlanId, pVlanIntfObjId, false,NULL);
							for ( j = 0; j< m_vlanid.size(); j++) {
								/*if (m_vlanid[j] == ObjectId::NullObjectId)
								{
									NSM_PLUG_DBG_S(j + string ("VLAN ObjId Null: continue"));
									continue;
								}*/
							
							        if(m_vlanid[j] != pVlanIntfObjId)
							        {     
								    trace(TRACE_LEVEL_DEBUG, string("IntfConfGlobalWorker::validatePoIntfStep:Removing for VLAN  ")  );
								    pPoIntfManagedObject->deleteVlanObjectId(m_vlanid[j]);
							        }
                                                        }
						 }
						 VlanIdList vlist;
						 if (pPoIntfManagedObject->m_nativeVlanIntfConfig.operator ->() != NULL) {
						     vlist.m_vlanIds.push_back(pPoIntfManagedObject->m_nativeVlanIntfConfig->getNativeVlanId());
						 }
						 INTF_XSTP_GLOBAL_WORKER->handleIntfVlanAssocUpdate(
						         pPoIntfManagedObject, vlist, pDceLinearSeqContext, false);

						 break;
					 }
                case NSMUPDATE_PO_SW_ADD_TRUNK_VLAN:
					 {
						 vector<UI32>::iterator msgiter;
						 vector <UI32> msgVlanRangeVector;
						 UI32Range msgVlanrange = UI32Range("");
						
						 /* Delete Native vlan from vlanRange if it is present */
						 deleteNativeVlanFromMsgVlanList (pNsmGlobalUpdatePoIntfMessage, pPoIntfManagedObject);

						 msgVlanrange = pNsmGlobalUpdatePoIntfMessage->getVlanRange();
						 msgVlanrange.getUI32RangeVector(msgVlanRangeVector);
						 msgVlanrange = UI32Range("");

						 if(pPoIntfManagedObject->getIsAllowedVlanAll())
						 {/*Check the remove list*/
							 UI32Range moRemoveVlanrange = UI32Range("");
							 vector <UI32> moRemoveVlanRangeVector;
							 vector<UI32>::iterator moremiter;

							 moRemoveVlanrange = pPoIntfManagedObject->getVlanRangeRemove();
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
                             NSM_PLUG_INFO_S(string(" case sw allowed all - remove range str : ") +  vlanRemoveRangeStr);
							 moRemoveVlanrange.fromString(vlanRemoveRangeStr); 
							 pPoIntfManagedObject->setVlanRangeRemove(moRemoveVlanrange);
							 pPoIntfManagedObject->setVlanRangeAdd(UI32Range(""));
							 pPoIntfManagedObject->setVlanRangeExcept(UI32Range(""));
						 }
						 else
						 {

							 vector <UI32> moVlanRangeVector;
							 UI32Range moVlanrange = UI32Range("");

							 moVlanrange = pPoIntfManagedObject->getVlanRangeAdd();
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
									 //syncQueryCtxt.addAndAttribute (new AttributeString(&vlanId,"vlanId"));
									 vlanMOSyncQueryCtxt.addAndAttribute (new AttributeUI32(&vlan_id,"id"));
									 vector<WaveManagedObject *> *pVlanMOResults = querySynchronously (&vlanMOSyncQueryCtxt);

									 if (NULL != pVlanMOResults) {
										 UI32 numberOfResults = pVlanMOResults->size ();
										 if (0 == numberOfResults) {
											 vlanstatus = WAVE_MESSAGE_ERROR;
										 }else if (1 == numberOfResults) {
											 vlanstatus = WAVE_MESSAGE_SUCCESS;
										 } else {
											 vlanstatus = WAVE_MESSAGE_ERROR;
										 }

										 pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*pVlanMOResults);
										 delete pVlanMOResults;
									 }

								 }

								 if(vlanstatus == WAVE_MESSAGE_SUCCESS)
						         	moVlanRangeVector.insert(moindex, *msgiter);

								 moindex = moVlanRangeVector.begin();
								 isVlanIdPresent = true;
							 }
							 string vlanRangeStr = UI32Range::getUI32RangeStringFromVector(moVlanRangeVector);
                             NSM_PLUG_INFO_S(string(" case sw add vlan - add vlan str : ") +  vlanRangeStr);
							 moVlanrange.fromString(vlanRangeStr); 
							 pPoIntfManagedObject->setVlanRangeAdd(moVlanrange);
							 pPoIntfManagedObject->setVlanRangeRemove(UI32Range(""));
							 pPoIntfManagedObject->setVlanRangeExcept(UI32Range(""));
							 pPoIntfManagedObject->setIsAllowedVlanNone(false);
						 }
						 break;
					 }
                case NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN:
						{
							vector<UI32>::iterator moindex;
							vector<UI32>::iterator msgiter;
							vector <UI32> msgVlanRangeVector;
							UI32Range msgVlanrange = UI32Range("");
							bool isVlanIdPresent = true;
							VlanIdList vlist;

						 	/* Delete Native vlan from vlanRange if it is present */
							deleteNativeVlanFromMsgVlanList (pNsmGlobalUpdatePoIntfMessage, pPoIntfManagedObject);
							
							msgVlanrange = pNsmGlobalUpdatePoIntfMessage->getVlanRange();
							msgVlanrange.getUI32RangeVector(msgVlanRangeVector);
							msgVlanrange = UI32Range("");

							if(pPoIntfManagedObject->getIsAllowedVlanAll())
							{/*Check the remove list*/

								UI32Range moRemoveVlanrange = UI32Range("");
								vector <UI32> moRemoveVlanRangeVector;
								vector<UI32>::iterator moremiter;

								moindex = moRemoveVlanRangeVector.begin();
								moRemoveVlanrange = pPoIntfManagedObject->getVlanRangeRemove();
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
								pPoIntfManagedObject->setVlanRangeRemove(moRemoveVlanrange);
								pPoIntfManagedObject->setVlanRangeAdd(UI32Range(""));
								pPoIntfManagedObject->setVlanRangeExcept(UI32Range(""));

								vlist.m_vlanIds = moRemoveVlanRangeVector;
								vlist.m_type = NONMEMBER_VLAN_LIST;
							}
							else
							{

								vector <UI32> moVlanRangeVector;
								UI32Range moVlanrange = UI32Range("");

								moVlanrange = pPoIntfManagedObject->getVlanRangeAdd();
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
								pPoIntfManagedObject->setVlanRangeAdd(moVlanrange);
								pPoIntfManagedObject->setVlanRangeRemove(UI32Range(""));
								pPoIntfManagedObject->setVlanRangeExcept(UI32Range(""));

								vlist.m_vlanIds = moVlanRangeVector;
								if (pPoIntfManagedObject->m_nativeVlanIntfConfig.operator ->() != NULL) {
								    vlist.m_vlanIds.push_back(pPoIntfManagedObject->m_nativeVlanIntfConfig->getNativeVlanId());
								}
							}

							INTF_XSTP_GLOBAL_WORKER->handleIntfVlanAssocUpdate(
							        pPoIntfManagedObject, vlist, pDceLinearSeqContext, false);

							{
								/* 
								 * ELD records for these specific VLANs are being removed.
								 */
							
								for (vid = msgVlanRangeVector.begin(); vid != msgVlanRangeVector.end(); vid++)
								{
									getVlanIntfObjectIdByVlanIfName(*vid, pVlanIntfObjId, false, pDceLinearSeqContext);
									vector<ObjectId>  m_vlanid = pPoIntfManagedObject->getVlanVector();
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
												pPoIntfManagedObject->deleteVlanObjectId(m_vlanid[j]);
												break;
											}
										}
									}
								}
							}
	
							break;
						}
                case NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN:
						{
							UI32Range msgVlanrange = UI32Range("");
							UI32Range moVlanrange = UI32Range("");

						 	/* Delete Native vlan from vlanRange if it is present */
							deleteNativeVlanFromMsgVlanList (pNsmGlobalUpdatePoIntfMessage, pPoIntfManagedObject);

							msgVlanrange = pNsmGlobalUpdatePoIntfMessage->getVlanRange();

							pPoIntfManagedObject->setIsAllowedVlanAll(true);
							pPoIntfManagedObject->setIsAllowedVlanNone(false);
							pPoIntfManagedObject->setVlanRangeRemove(msgVlanrange);
							pPoIntfManagedObject->setVlanRangeAdd(UI32Range(""));
							pPoIntfManagedObject->setVlanRangeExcept(UI32Range(""));

							VlanIdList vlist (msgVlanrange, NONMEMBER_VLAN_LIST);
							INTF_XSTP_GLOBAL_WORKER->handleIntfVlanAssocUpdate(
							        pPoIntfManagedObject, vlist, pDceLinearSeqContext, false);
							 /*
								* ELD records for these specific VLANs are being removed.
							*/
							vector <UI32> msgVlanRangeVector;
							msgVlanrange.getUI32RangeVector(msgVlanRangeVector);

							for (vid = msgVlanRangeVector.begin(); vid != msgVlanRangeVector.end(); vid++)
							{
								getVlanIntfObjectIdByVlanIfName(*vid, pVlanIntfObjId, false, pDceLinearSeqContext);
								vector<ObjectId>  m_vlanid = pPoIntfManagedObject->getVlanVector();
								if (m_vlanid.size() > 0)
								{
									trace(TRACE_LEVEL_DEBUG, string("IntfConfGlobalWorker::updatePoIntfMO(except vlan)m_vlanid.size() = ") + m_vlanid.size());
									for (j = 0; j < m_vlanid.size(); j++) {
										/*if (m_vlanid[j] == ObjectId::NullObjectId) {
											NSM_PLUG_INFO_S(j + string ("VLAN ObjId Null: continue"));
											continue;
										}*/
										if(m_vlanid[j] == pVlanIntfObjId)
										{
											trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::validatePoIntfStep(except vlan):Removing for VLAN = ") + *vid);
											pPoIntfManagedObject->deleteVlanObjectId(m_vlanid[j]);
											updateWaveManagedObject(pPoIntfManagedObject);
											break;
										}
									}
								}
							}

							break;
						}
		    case NSMUPDATE_PO_NATIVEVLANDISABLE :
				{
			    	trace (TRACE_LEVEL_INFO, string ("\t Native Vlan Disable :: ") + 
									pNsmGlobalUpdatePoIntfMessage->getNativevlanDisable());
					 
			    	pNativeVlanIntfConfLocalManagedObject =
							NativeVlanPoGetChildMoByParentMo(pPoIntfManagedObject);

			    	if(pNativeVlanIntfConfLocalManagedObject != NULL)
			    	{
			    		pNativeVlanIntfConfLocalManagedObject->setNativevlanDisable(
								pNsmGlobalUpdatePoIntfMessage->getNativevlanDisable());
						updateWaveManagedObject (pNativeVlanIntfConfLocalManagedObject);

						if (pDceLinearSeqContext)
		        			pDceLinearSeqContext->addManagedObjectForGarbageCollection(pNativeVlanIntfConfLocalManagedObject);
			    	}

                    /* Native vlan should be removed from Remove and Except lists */
                    UI32Range vlanRemoverange =  pPoIntfManagedObject->getVlanRangeRemove();
                    if (NsmUtils::isNativeVlanPresentInList(vlanRemoverange, pNsmGlobalUpdatePoIntfMessage->getNativevlanDisable(), NULL, pPoIntfManagedObject))  {   
                        NSM_PLUG_DBG(" native vlan from remove list");
                        pPoIntfManagedObject->setVlanRangeRemove(vlanRemoverange);
                    }
                    UI32Range vlanExceptrange =  pPoIntfManagedObject->getVlanRangeExcept();
                    if (NsmUtils::isNativeVlanPresentInList(vlanExceptrange, pNsmGlobalUpdatePoIntfMessage->getNativevlanDisable(), NULL, pPoIntfManagedObject))  {   
                        NSM_PLUG_DBG(" native vlan from Except list");
                        pPoIntfManagedObject->setVlanRangeExcept(vlanExceptrange);
                    }

		   			break;
				}
		    case NSMUPDATE_PO_NATIVEVLANID :
				{
			    	trace (TRACE_LEVEL_INFO, string ("\t Native Vlan Id :: ") + 
									pNsmGlobalUpdatePoIntfMessage->getNativeVlanId());
                    /* Native vlan should be removed from Remove and Except lists */
                    UI32Range vlanRemoverange =  pPoIntfManagedObject->getVlanRangeRemove();
                    if (NsmUtils::isNativeVlanPresentInList(vlanRemoverange, pNsmGlobalUpdatePoIntfMessage->getNativevlanDisable(), NULL, pPoIntfManagedObject))  {   
                        NSM_PLUG_DBG(" native vlan from remove list");
                        pPoIntfManagedObject->setVlanRangeRemove(vlanRemoverange);
                    }
                    UI32Range vlanExceptrange =  pPoIntfManagedObject->getVlanRangeExcept();
                    if (NsmUtils::isNativeVlanPresentInList(vlanExceptrange, pNsmGlobalUpdatePoIntfMessage->getNativevlanDisable(), NULL, pPoIntfManagedObject))  {   
                        NSM_PLUG_DBG(" native vlan from Except list");
                        pPoIntfManagedObject->setVlanRangeExcept(vlanExceptrange);
                    }
					 
			    	pNativeVlanIntfConfLocalManagedObject = NativeVlanPoGetChildMoByParentMo(pPoIntfManagedObject);

                    if(pNativeVlanIntfConfLocalManagedObject == NULL)
                    {
						pNativeVlanIntfConfLocalManagedObject = new NativeVlanIntfConfLocalManagedObject(
																	dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
						pNativeVlanIntfConfLocalManagedObject->setNativevlanDisable(true);
						pNativeVlanIntfConfLocalManagedObject->setNativeVlanId(pNsmGlobalUpdatePoIntfMessage->getNativeVlanId());
						pNativeVlanIntfConfLocalManagedObject->setNativevlanPresence(true);
						pPoIntfManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (
																		pNativeVlanIntfConfLocalManagedObject));
						trace(TRACE_LEVEL_INFO, string("SW_MODE_PVLAN_TR_PROMISCUOUS native vlan: ") + pNsmGlobalUpdatePoIntfMessage->getNativeVlanId());

                    }
                    else
                    {
                      	 native_vlanid = pNativeVlanIntfConfLocalManagedObject->getNativeVlanId();
      					 pNativeVlanIntfConfLocalManagedObject->setNativeVlanId(pNsmGlobalUpdatePoIntfMessage->getNativeVlanId());
      					 trace(TRACE_LEVEL_INFO, string("Old native vlan: ") + native_vlanid);
      					 if (pDceLinearSeqContext)
     	        			pDceLinearSeqContext->addManagedObjectForGarbageCollection(pNativeVlanIntfConfLocalManagedObject);

                    }

					updateWaveManagedObject (pNativeVlanIntfConfLocalManagedObject);

					if (pNsmGlobalUpdatePoIntfMessage->getNativeVlanId() != native_vlanid && native_vlanid != 0)
					{

						/* 
						 * ELD records (if any) for the previous Native-VLAN are
						 * being removed for the interface.
						 */
                        NSM_PLUG_INFO_S("updatePoIntfMO for deletion..");

						vector<ObjectId>  m_vlanid = pPoIntfManagedObject->getVlanVector();
						trace(TRACE_LEVEL_INFO, string("m_vlanid size is") + m_vlanid.size());
						if (m_vlanid.size() > 0) {
							trace(TRACE_LEVEL_INFO, string("Checking for ELD-records for native vlan ") + native_vlanid);

							getVlanIntfObjectIdByVlanIfName(native_vlanid, pVlanIntfObjId, false, pDceLinearSeqContext);

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
                                    moAddVlanrange = pPoIntfManagedObject->getVlanRangeAdd();
                                    moAddVlanrange.getUI32RangeVector(moAddVlanRangeVector);
                                    vector<UI32>::iterator result;
                                    vector <UI32> moExceptVlanRangeVector;
                                    UI32Range moExceptVlanrange = UI32Range("");
                                    vector<UI32>::iterator exceptresult;
                                    vector <UI32> moRemoveVlanRangeVector;
                                    UI32Range moRemoveVlanrange = UI32Range("");
                                    vector<UI32>::iterator removeresult;
                                    result = find (moAddVlanRangeVector.begin(), moAddVlanRangeVector.end(), native_vlanid);
                                    moExceptVlanrange =pPoIntfManagedObject->getVlanRangeExcept();
                                    moExceptVlanrange.getUI32RangeVector(moExceptVlanRangeVector);
                                    exceptresult = find (moExceptVlanRangeVector.begin(), moExceptVlanRangeVector.end(), native_vlanid);
                                    moRemoveVlanrange =pPoIntfManagedObject->getVlanRangeRemove();
                                    moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
                                    removeresult = find (moRemoveVlanRangeVector.begin(), moRemoveVlanRangeVector.end(), native_vlanid);




                                    if (((!pPoIntfManagedObject->getIsAllowedVlanAll())&&(result==moAddVlanRangeVector.end())) ||
                                         ((pPoIntfManagedObject->getIsAllowedVlanAll())&&((exceptresult!= moExceptVlanRangeVector.end())||(removeresult !=moRemoveVlanRangeVector.end()))))
									{
										trace(TRACE_LEVEL_INFO, string("Deleting ELD-records for native vlan ") + native_vlanid);
										pPoIntfManagedObject->deleteVlanObjectId(m_vlanid[j]);
								 		pPoIntfManagedObject->addAttributeToBeUpdated("vlanid");
										updateWaveManagedObject(pPoIntfManagedObject);
									}
									else 
									{
										trace(TRACE_LEVEL_INFO, string("Not deleting ELD-records for native vlan ") + native_vlanid);
									}
									break;
								}
							}
						}
					}
				
		   			break;
				}

            case NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_OVERRIDE :
                {
                        trace (TRACE_LEVEL_DEVEL, string ("\t Update Po Interface NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_OVERRIDE"));
                    /* Request from NSM to override current vLAG primary link domain id and partner info*/
                    pPoIntfManagedObject->setPoPrimaryLinkDomainId(pNsmGlobalUpdatePoIntfMessage->getPoPrimaryLinkDomainId());
                    pNsmGlobalUpdatePoIntfMessage->setReplyCode(VLAG_PRIMARY_LINK_SELECTED);
                    break;
                }
            case NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID :
                {
                        trace (TRACE_LEVEL_DEVEL, string ("\t Update Po Interface NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID new/cur ") + pNsmGlobalUpdatePoIntfMessage->getPoPartnerSystem() + pPoIntfManagedObject->getPoPartnerSystem());
                    /* Request from NSM to set vLAG primary link domain id */
                    if (pPoIntfManagedObject->getPoPrimaryLinkDomainId() == 0) {	/* Primary Link has not been selected */
                         pPoIntfManagedObject->setPoPrimaryLinkDomainId(pNsmGlobalUpdatePoIntfMessage->getPoPrimaryLinkDomainId());
                    }
                    if (pPoIntfManagedObject->getPoPartnerSystemPriority() == 0) {	/* Partner System Priority has not been set */
                         pPoIntfManagedObject->setPoPartnerSystemPriority(pNsmGlobalUpdatePoIntfMessage->getPoPartnerSystemPriority());
                    }
                    if (pPoIntfManagedObject->getPoPartnerKey() == 0) {	/* Partner Key has not been set */
                         pPoIntfManagedObject->setPoPartnerKey(pNsmGlobalUpdatePoIntfMessage->getPoPartnerKey());
                    }
                    if (pPoIntfManagedObject->getPoPartnerSystem().compare(null_str) == 0) {	/* Partner System has not been set */
                        trace (TRACE_LEVEL_DEVEL, string ("\t Update Po Interface NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID empty string ") + pNsmGlobalUpdatePoIntfMessage->getPoPartnerSystem());
                         pPoIntfManagedObject->setPoPartnerSystem(pNsmGlobalUpdatePoIntfMessage->getPoPartnerSystem());
                    } else {
                        trace (TRACE_LEVEL_DEVEL, string ("\t Update Po Interface NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID not empty string ") + pPoIntfManagedObject->getPoPartnerSystem());
		    }
                    pNsmGlobalUpdatePoIntfMessage->setReplyCode(VLAG_PRIMARY_LINK_SELECTED);
                    break;
                }
            case NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_CLEAR :
                {
                        trace (TRACE_LEVEL_DEVEL, string ("\t Update Po Interface NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_CLEAR"));
                    pPoIntfManagedObject->setPoPrimaryLinkDomainId(0);
                    pPoIntfManagedObject->setPoPartnerSystemPriority(0);
                    pPoIntfManagedObject->setPoPartnerKey(0);
                    pPoIntfManagedObject->setPoPartnerSystem(null_str);
                    pNsmGlobalUpdatePoIntfMessage->setReplyCode(VLAG_PRIMARY_LINK_CLEARED);
                    break;
                }
            case ELD_INTF_PO_CONF_PRIORITY:
				{
					if(pPoIntfManagedObject != NULL) {
						/* Retrive physical port managed objects of physical ports already assigned to the port channel */
						ObjectId poObjectId;
						poObjectId =  pPoIntfManagedObject->getObjectId();
						WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
						syncQueryCtxt.addAndAttribute (new AttributeObjectId(poObjectId,"poId"));
						vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

						/* if( no members in po intf) */
						if (pResults->size() == 0) { /* No physical ports assigned to port-channel */
							trace(TRACE_LEVEL_ERROR, string("No physical ports assigned to port-channel"));
							return WRC_ELD_NOT_ENABLED_ON_EMPTY_PO;
						}

                     pPoIntfManagedObject->setEldPrio (pNsmGlobalUpdatePoIntfMessage->getEldPrioVal());
					 pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
                     trace (TRACE_LEVEL_DEBUG, string ("\tELD PRIO MO ") + pNsmGlobalUpdatePoIntfMessage->getCmdCode());
					}
                    break;
				}
			case ELD_INTF_PO_CONF_ELD_ENABLE:
				{
					if(pPoIntfManagedObject != NULL) {
						/* Retrive physical port managed objects of physical ports already assigned to the port channel */
						ObjectId poObjectId;
						poObjectId =  pPoIntfManagedObject->getObjectId();
						WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
						syncQueryCtxt.addAndAttribute (new AttributeObjectId(poObjectId,"poId"));
						vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

						/* if( no members in po intf) */
						if (pResults->size() == 0) { /* No physical ports assigned to port-channel */
							trace(TRACE_LEVEL_ERROR, string("No physical ports assigned to port-channel"));
							return WRC_ELD_NOT_ENABLED_ON_EMPTY_PO;
						}

						pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);
						pPoIntfManagedObject->setVlanidList (pNsmGlobalUpdatePoIntfMessage->getVlanidList());
						trace (TRACE_LEVEL_DEBUG, string ("\tENABLE: VlanRange :: ") + (pNsmGlobalUpdatePoIntfMessage->getVlanIdRange()).toString());
						getVlanIntfObjectIdByVlanIfName (atoi(pNsmGlobalUpdatePoIntfMessage->getVlanIdRange().toString().c_str()), vlanOId, false, pDceLinearSeqContext);
						if (vlanOId == ObjectId::NullObjectId) {

							trace(TRACE_LEVEL_INFO, string("ENABLE: Vlan Interface does not exist .."));
							return WAVE_MESSAGE_SUCCESS;
						}
						else {
							trace(TRACE_LEVEL_INFO, string("ENABLE: Vlan Interface does exist .."));
						}
						for (vlanid = pNsmGlobalUpdatePoIntfMessage->m_vlanid.begin(); vlanid != pNsmGlobalUpdatePoIntfMessage->m_vlanid.end(); vlanid++)
						{
							if(  pNsmGlobalUpdatePoIntfMessage->getCmdCode() == ELD_INTF_PO_CONF_ELD_ENABLE )  {
								pPoIntfManagedObject->addVlanObjectId(*vlanid);
							}
							else {
								trace (TRACE_LEVEL_INFO, "pPoIntfManagedObject::updatePhyIntfMO:  Unsupported cmd :" + pNsmGlobalUpdatePoIntfMessage->getCmdCode());
							}
						}

						trace (TRACE_LEVEL_DEBUG, string ("\tENABLE: ELD VLAN MO ") + pNsmGlobalUpdatePoIntfMessage->getCmdCode());
					}	
					break;
				}
			case ELD_INTF_PO_CONF_ELD_DISABLE:
				{
					trace (TRACE_LEVEL_DEBUG, string ("\tDISABLE: VlanRange :: ") + (pNsmGlobalUpdatePoIntfMessage->getVlanIdRange()).toString());
					getVlanIntfObjectIdByVlanIfName (atoi(pNsmGlobalUpdatePoIntfMessage->getVlanIdRange().toString().c_str()), vlanOId, false, pDceLinearSeqContext);
					if (vlanOId == ObjectId::NullObjectId) {

						trace(TRACE_LEVEL_INFO, string("DISABLE: Vlan Interface does not exist .."));
						return WAVE_MESSAGE_SUCCESS;
					}
					else {
						trace(TRACE_LEVEL_INFO, string("DISABLE: Vlan Interface does exist .."));
					}
					if(pNsmGlobalUpdatePoIntfMessage->m_vlanid.empty())
					{
						trace(TRACE_LEVEL_DEBUG,"vlanid is empty for ELD and range not empty due to no po member");
						if (NsmUtils::isAllUI32RangeVlanConfigured(pNsmGlobalUpdatePoIntfMessage->getVlanIdRange(), &pNsmGlobalUpdatePoIntfMessage->m_vlanid,pDceLinearSeqContext) == WAVE_MESSAGE_ERROR)
						{
							trace (TRACE_LEVEL_DEBUG, "IntfConfGlobalWorker::updatePoIntfMO: :  one or more vlan doesn't exist   ");
						}
					}
					for (vlanid = pNsmGlobalUpdatePoIntfMessage->m_vlanid.begin(); vlanid != pNsmGlobalUpdatePoIntfMessage->m_vlanid.end(); vlanid++)
					{
						if(  pNsmGlobalUpdatePoIntfMessage->getCmdCode() == ELD_INTF_PO_CONF_ELD_DISABLE )  {
						trace(TRACE_LEVEL_DEVEL, string(" IntfConfGlobalWorker::updatePoIntf:MODISABLE: in for .invoking deleteVlanObjectID"));
							pPoIntfManagedObject->deleteVlanObjectId(*vlanid);
						}
						else {
							trace (TRACE_LEVEL_INFO, "pPoIntfManagedObject::updatePhyIntfMO:  Unsupported cmd :" + pNsmGlobalUpdatePoIntfMessage->getCmdCode());
						}
					}

					trace (TRACE_LEVEL_DEBUG, string ("\tDISABLE: ELD VLAN MO ") + pNsmGlobalUpdatePoIntfMessage->getCmdCode());
					break;
				}	

            case NSMUPDATE_PO_RT_LACP_SYSTEM_ID :
                    trace (TRACE_LEVEL_DEVEL, string ("\t:updatePoIntfMO NSMUPDATE_PO_RT_LACP_SYSTEM_ID :: ") + pNsmGlobalUpdatePoIntfMessage->getPoLacpSystemId());
                    pPoIntfManagedObject->setPoLacpSystemId(pNsmGlobalUpdatePoIntfMessage->getPoLacpSystemId());
                    break;
			case NSMUPDATE_PO_SET_IPV6_RA_GUARD:
			case NSMUPDATE_PO_RESET_IPV6_RA_GUARD:
					pPoIntfManagedObject->setRaGuard(pNsmGlobalUpdatePoIntfMessage->getRaGuard());
					break;
            }
        }

             pNsmGlobalUpdatePoIntfMessage->setPoRef(pPoIntfManagedObject);
             trace (TRACE_LEVEL_INFO, string ("\tDesc :: ") + pPoIntfManagedObject->getDesc());

             pNsmGlobalUpdatePoIntfMessage->setCompletionStatus(status);
             NSM_PLUG_INFO_S(string ("\tStatus : ") + status);
        	 return(status);
    }

void IntfConfGlobalWorker::updatePoIntfStep (DceLinearSeqContext *pDceLinearSeqContext)
    {
        NsmGlobalUpdatePoIntfMessage *pNsmGlobalUpdatePoIntfMessage = dynamic_cast<NsmGlobalUpdatePoIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

		UI32 native_vlanid = 0;
		ObjectId pVlanIntfObjId = ObjectId::NullObjectId;
		NativeVlanIntfConfLocalManagedObject *pNativeVlanIntfConfLocalManagedObject = NULL;
		PoIntfManagedObject *pPoIntfManagedObject1;
		vector<ObjectId>  m_vlanid;
		vector<ObjectId>::iterator search_native;
        vector <UI32> moAddVlanRangeVector;
        UI32Range moAddVlanrange = UI32Range("");
        vector<UI32>::iterator result;
        vector <UI32> moExceptVlanRangeVector;
        UI32Range moExceptVlanrange = UI32Range("");
        vector<UI32>::iterator exceptresult;
        vector <UI32> moRemoveVlanRangeVector;
        UI32Range moRemoveVlanrange = UI32Range("");
        vector<UI32>::iterator removeresult;



        
        WaveSendToClusterContext    *send_ctx_p;
		PoIntfManagedObject* pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>(pDceLinearSeqContext->getCache()->getWaveManagedObject(PoIntfManagedObject::getClassName()));

       NSM_PLUG_DBG_S("Retrieved PoIntfManagedObject from DataCache...");
       NSM_PLUG_INFO_S("UPDATEPOINTFSTEP..");
       ObjectId poObjectId = pPoIntfManagedObject->getObjectId();
       // Should this be checked?
       ResourceId  status = isPoIntfHasMember (poObjectId);

       //In VCS mode : Back end creates a PO object only when PO has atleast one member port.
       if((WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()) && (status == WAVE_MESSAGE_ERROR))
       {
           pNsmGlobalUpdatePoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
           pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
           return;
       }

        NsmUpdatePoIntfSPMessage *m = new NsmUpdatePoIntfSPMessage();
        m->setPoId (pNsmGlobalUpdatePoIntfMessage->getPoId());
        m->setCmdCode (pNsmGlobalUpdatePoIntfMessage->getCmdCode());

        switch(m->getCmdCode())
        {
            case NSMUPDATE_PO_DESC :
            case NSMUPDATE_PO_DELDESC :
                    m->setDesc (pNsmGlobalUpdatePoIntfMessage->getDesc());
                    break;
            case NSMUPDATE_PO_SW :
            case NSMUPDATE_PO_DELSW :
                    m->setSwModeFlag (pNsmGlobalUpdatePoIntfMessage->getSwModeFlag());
                    break;
            case NSMUPDATE_PO_MTU :
            case NSMUPDATE_PO_DELMTU:
                    m->setMtu(pNsmGlobalUpdatePoIntfMessage->getMtu());
                    break;
            case NSMUPDATE_PO_MINLINKS :
            case NSMUPDATE_PO_DELMINLINKS:
                    m->setPoMinLinks(pNsmGlobalUpdatePoIntfMessage->getPoMinLinks());
                    break;
			case NSMUPDATE_PO_ALLOWEDSPEED :
					m->setPoAllowedSpeed(pNsmGlobalUpdatePoIntfMessage->getPoAllowedSpeed());
					break;
			case NSMUPDATE_PO_LOADBALANCE:
			case NSMUPDATE_PO_DELLOADBALANCE:
					NSM_PLUG_INFO_S(string("IntfConfGlobalWorker::updatePoIntfStep ") + pNsmGlobalUpdatePoIntfMessage->getPoLoadBalanceMode()); 
                    m->setPoLoadBalanceMode(pNsmGlobalUpdatePoIntfMessage->getPoLoadBalanceMode());
					break;
            case NSMUPDATE_PO_POMODE :
                    m->setPoMode(pNsmGlobalUpdatePoIntfMessage->getPoMode());
            case NSMUPDATE_PO_POTYPE :
                    m->setPoType(pNsmGlobalUpdatePoIntfMessage->getPoType());
                    break;
            case NSMUPDATE_PO_MODE :
            case NSMUPDATE_PO_DELMODE:
                    m->setSwMode(pNsmGlobalUpdatePoIntfMessage->getSwMode());
                    break;
 
            case NSMUPDATE_PO_STATUS :
            case NSMUPDATE_PO_DELSTATUS :
                    m->setFlag (pNsmGlobalUpdatePoIntfMessage->getFlag());
                    break;

            case NSMUPDATE_PO_IGNORE_SPLIT :
            case NSMUPDATE_PO_NO_IGNORE_SPLIT :
                    m->setIgnoreSplit (pNsmGlobalUpdatePoIntfMessage->getIgnoreSplit());
                    break;

            case NSMUPDATE_PO_SW_TRUNK_VLAN_NONE:
                    m->setIsAllowedVlanAll(false);
                    break;
            case NSMUPDATE_PO_SW_TRUNK_VLAN_ALL:
                    m->setIsAllowedVlanAll(true);
                    break;
			case NSMUPDATE_PO_PVLAN_MODE_TRUNK:
				m->setSwMode(pNsmGlobalUpdatePoIntfMessage->getSwMode());
				break;
			case NSMUPDATE_PO_PVLAN_MODE_TRUNK_HOST:
				m->setSwMode(pNsmGlobalUpdatePoIntfMessage->getSwMode());
				break;
			case NSMUPDATE_PO_PVLAN_MODE_TRUNK_PROMISCUOUS: 
				m->setSwMode(pNsmGlobalUpdatePoIntfMessage->getSwMode());
				trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW NSMUPDATE_PO_PVLAN_MODE_TRUNK_PROMISCUOUS "));
				break;

			case NSMUPDATE_PO_PVLAN_MODE_PROMISCUOUS:
				m->setSwMode(pNsmGlobalUpdatePoIntfMessage->getSwMode());
				break;
				
			case NSMUPDATE_PO_PVLAN_MODE_HOST:
				m->setSwMode(pNsmGlobalUpdatePoIntfMessage->getSwMode());
				break;
			case NSMUPDATE_PO_PVLAN_NATIVE_VLANID:
					m->setPvlanNativeVlanId(pNsmGlobalUpdatePoIntfMessage->getPvlanNativeVlanId());
					break;
			case NSMUPDATE_PO_PVLAN_NATIVE_VLANID_DISABLE:
					m->setPvlanNativeVlanId(0);
					break;

			case NSMUPDATE_PO_PVLAN_HOST_ASSOCIATION:
			case NSMUPDATE_PO_DELPVLAN_HOST_ASSOCIATION_ALL:
			{
         		//PVLAN todo
				trace(TRACE_LEVEL_INFO,string("IntfConfGlobalWorker sending to local -- updatePhyIntf "));
         		m->setPvlanHostPrimVlanId(pNsmGlobalUpdatePoIntfMessage->getPvlanHostPrimVlanId());
         		m->setPvlanHostSecVlanId(pNsmGlobalUpdatePoIntfMessage->getPvlanHostSecVlanId());
         		break;
			}

			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ALL:
					m->setPvlanIsAllowedVlanAll(true);
					break;
					
			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_NONE:
					m->setPvlanIsAllowedVlanAll(false);
					break;
					
			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ADD:
			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_REMOVE:
			case NSMUPDATE_PO_PVLAN_TRUNK_VLAN_EXCEPT:
					m->setPvlanRange(pNsmGlobalUpdatePoIntfMessage->getPvlanRange());
					break;
            case NSMUPDATE_PO_SW_ADD_ACCESS_VLAN:
            case NSMUPDATE_PO_SW_DELETE_ACCESS_VLAN:
            case NSMUPDATE_PO_SW_ADD_TRUNK_VLAN:
            case NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN:
            case NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN:
                    m->setVlanRange(pNsmGlobalUpdatePoIntfMessage->getVlanRange());
                    break;
			case NSMUPDATE_PO_NATIVEVLANDISABLE:
			{
				m->setNativevlanDisable(pNsmGlobalUpdatePoIntfMessage->getNativevlanDisable());
				/* 
				 * To enable LW create a message for sending to eld for TR 374321 
				 */
				pPoIntfManagedObject1 = dynamic_cast<PoIntfManagedObject *>
					(pDceLinearSeqContext->getCache()->getWaveManagedObject(PoIntfManagedObject::getClassName()));
				pNativeVlanIntfConfLocalManagedObject = 
					NativeVlanPoGetChildMoByParentMo(pPoIntfManagedObject1);
				// To check if ELD is configured at this native-vlan on this intf
				{
					if(pNativeVlanIntfConfLocalManagedObject != NULL)
					{
						native_vlanid = pNativeVlanIntfConfLocalManagedObject->getNativeVlanId();
						if (pDceLinearSeqContext)
		        			pDceLinearSeqContext->addManagedObjectForGarbageCollection(pNativeVlanIntfConfLocalManagedObject);

					}

					m_vlanid = pPoIntfManagedObject1->getVlanVector();
					trace(TRACE_LEVEL_INFO, string("m_vlanid size is") + m_vlanid.size());

					if (m_vlanid.size() > 0 && native_vlanid != 0)
					{
						trace(TRACE_LEVEL_INFO, string("Check for ELD-status of native vlan ") + native_vlanid);
						getVlanIntfObjectIdByVlanIfName(native_vlanid, pVlanIntfObjId, false, pDceLinearSeqContext);

						search_native = find(m_vlanid.begin(), m_vlanid.end(), pVlanIntfObjId);

						if (search_native != m_vlanid.end())
						{
					    	m->setNativeVlanId(native_vlanid);
							trace(TRACE_LEVEL_INFO, string (
									"ELD will be enabled on Intf: ") + pNsmGlobalUpdatePoIntfMessage->getPoId() + 
								" at native-vlan: " + native_vlanid);
						}
						else {
					    	m->setNativeVlanId(0);
							trace(TRACE_LEVEL_INFO, string ("Intf: ") + pNsmGlobalUpdatePoIntfMessage->getPoId() + 
								" : ELD was not configured at native-vlan: " + native_vlanid);
						} 
					}
                                        else if (m_vlanid.size()==0){
						m->setNativeVlanId(0);
						trace(TRACE_LEVEL_INFO, string ("Intf: ") + pNsmGlobalUpdatePoIntfMessage->getPoId() + 
							" : ELD was not configured on this po- native-vlan is : " + native_vlanid);


                                       }
				}

				break;
			}

	    case NSMUPDATE_PO_NATIVEVLANID:

			m->setNativeVlanId(pNsmGlobalUpdatePoIntfMessage->getNativeVlanId());

			trace(TRACE_LEVEL_INFO, string("Native vlan: noted in message is ") + pNsmGlobalUpdatePoIntfMessage->getNativeVlanId());

             pPoIntfManagedObject1 = dynamic_cast<PoIntfManagedObject *>  (pDceLinearSeqContext->getCache()->getWaveManagedObject(PoIntfManagedObject::getClassName()));
             pNativeVlanIntfConfLocalManagedObject =
                   NativeVlanPoGetChildMoByParentMo(pPoIntfManagedObject1);

             if(pNativeVlanIntfConfLocalManagedObject != NULL)
             {
                 // To check if ELD is configured at this native-vlan on this intf
              native_vlanid = pNativeVlanIntfConfLocalManagedObject->getNativeVlanId();
              moAddVlanrange = pPoIntfManagedObject1->getVlanRangeAdd();
              moAddVlanrange.getUI32RangeVector(moAddVlanRangeVector);

             result = find (moAddVlanRangeVector.begin(), moAddVlanRangeVector.end(), native_vlanid);
             moExceptVlanrange =pPoIntfManagedObject1->getVlanRangeExcept();
             moExceptVlanrange.getUI32RangeVector(moExceptVlanRangeVector);
             exceptresult = find (moExceptVlanRangeVector.begin(), moExceptVlanRangeVector.end(), native_vlanid);
             moRemoveVlanrange =pPoIntfManagedObject1->getVlanRangeRemove();
             moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
             removeresult = find (moRemoveVlanRangeVector.begin(), moRemoveVlanRangeVector.end(), native_vlanid);


             if (((pPoIntfManagedObject1->getIsAllowedVlanAll())&&(exceptresult== moExceptVlanRangeVector.end())&&(removeresult ==moRemoveVlanRangeVector.end()))||
                 (result !=moAddVlanRangeVector.end()))
 			{
 				m_vlanid = pPoIntfManagedObject1->getVlanVector();
 				trace(TRACE_LEVEL_INFO, string("m_vlanid size is") + m_vlanid.size());

 				if (m_vlanid.size() > 0)
 				{
					trace(TRACE_LEVEL_INFO, string("Check for ELD-status of native vlan ") + native_vlanid);
					getVlanIntfObjectIdByVlanIfName(native_vlanid, pVlanIntfObjId, false, pDceLinearSeqContext);

 					search_native = find(m_vlanid.begin(), m_vlanid.end(), pVlanIntfObjId);

 					if (search_native != m_vlanid.end())
 					{
 						m->setEldAllowedVlanAll(native_vlanid);
 						trace(TRACE_LEVEL_INFO, string (
 								"ELD will be enabled on Intf: ") + pNsmGlobalUpdatePoIntfMessage->getPoId() +
 							" at native-vlan: " + native_vlanid);
 					}
 					else {
 						m->setEldAllowedVlanAll(0);
 						trace(TRACE_LEVEL_INFO, string ("Intf: ") + pNsmGlobalUpdatePoIntfMessage->getPoId() +
 							" : ELD was not configured at native-vlan: " + native_vlanid);
 					}
 				}
 			}
             if (pDceLinearSeqContext)
             	pDceLinearSeqContext->addManagedObjectForGarbageCollection(pNativeVlanIntfConfLocalManagedObject);

             }

			break;

            /*  No need to send to Backend  for cmd NSMUPDATE_PO_EXCEPTION_STATUS  */
            case NSMUPDATE_PO_EXCEPTION_STATUS :
                trace (TRACE_LEVEL_DEBUG, string ("\t IntfConfGlobalWorker::updatePoIntfStep: NSMUPDATE_PO_EXCEPTION_STATUS..."));
                if (m != NULL) {
                    delete m;
                }
                pNsmGlobalUpdatePoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
                pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
                return ;
                /* The following are RunTime request from NSM to update */
                /* the Primary Link domain id.  We return since there */
                /* is not need to inform NSM */
            case NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID :
            case NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_OVERRIDE :
            case NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_CLEAR :
                    trace (TRACE_LEVEL_DEVEL, string ("\t updatePoIntfStep: NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID..."));
                    if (m != NULL) {
                        delete m;
                    }
                    pNsmGlobalUpdatePoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
                    pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
                    return ;

                /* The following is a RunTime request change the LACP */
                /* System Id due to the switch that owned the old */
                /* LACP System Id is no longer available */
            case NSMUPDATE_PO_RT_LACP_SYSTEM_ID:
                    m->setPoLacpSystemId(pNsmGlobalUpdatePoIntfMessage->getPoLacpSystemId());
                    trace (TRACE_LEVEL_INFO, string ("\t:updatePoIntfStep NSMUPDATE_PO_RT_LACP_SYSTEM_ID :: ") + pNsmGlobalUpdatePoIntfMessage->getPoLacpSystemId());
                    break;

            case NSMUPDATE_PO_PORT_PROFILED :
            case NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PO :
            case NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PO :
            case NSMUPDATE_DISSOCIATE_PORT_PROFILE_WITH_PO :
            case NSMUPDATE_DISSOCIATE_PORT_PROFILE_DOMAIN_WITH_PO :
                    {
                        m->setIsPortProfiled(pNsmGlobalUpdatePoIntfMessage->getIsPortProfiled());
                        m->setPortProfileName(pNsmGlobalUpdatePoIntfMessage->getPortProfileName());
                        m->setPortProfileDomainName(pNsmGlobalUpdatePoIntfMessage->getPortProfileDomainName());
                        trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::updatePoIntfStep:: Sending portProfile association message to local plugin "
                                    "poname<") + m->getPoId() + "> isPortProfiled<" + m->getIsPortProfiled() 
                                + "> port-profile-domain-name<" + m->getPortProfileDomainName() + ">port-profile-name<" + m->getPortProfileName() 
                                + "> cmdCode<" + m->getCmdCode()+ ">");
                        break;
                    }
            case ELD_INTF_PO_CONF_PRIORITY:
                 m->setPrio(pNsmGlobalUpdatePoIntfMessage->getEldPrioVal());

                 NSM_PLUG_INFO_S(string(" Cmdcode :") +  m->getCmdCode() + (" ELD - PRIO :") + m->getPrio());
                 break;
            case ELD_INTF_PO_CONF_ELD_ENABLE:
                 if (NsmUtils::isAllUI32RangeVlanConfigured(pNsmGlobalUpdatePoIntfMessage->getVlanIdRange(),  pNsmGlobalUpdatePoIntfMessage->m_vlanid) == WAVE_MESSAGE_ERROR) {
                     trace (TRACE_LEVEL_ERROR, "IntfConfGlobalWorker::updatePoIntfStep:  one or more vlan doesn't exist   ");
                 }
                 m->setVlanidList(pNsmGlobalUpdatePoIntfMessage->getVlanidList());
                 NSM_PLUG_INFO_S(string(" Cmdcode :") +  m->getCmdCode());
                 break;
            case ELD_INTF_PO_CONF_ELD_DISABLE:
                 if (NsmUtils::isAllUI32RangeVlanConfigured(pNsmGlobalUpdatePoIntfMessage->getVlanIdRange(),  pNsmGlobalUpdatePoIntfMessage->m_vlanid) == WAVE_MESSAGE_ERROR) {
                     trace (TRACE_LEVEL_ERROR, "IntfConfGlobalWorker::updatePoIntfStep:  one or more vlan doesn't exist   ");
                 }
                 m->setVlanidList(pNsmGlobalUpdatePoIntfMessage->getVlanidList());
                 NSM_PLUG_INFO_S(string(" Cmdcode :") +  m->getCmdCode());
                 break;
            case NSMUPDATE_PO_SET_IPV6_RA_GUARD:
            case NSMUPDATE_PO_RESET_IPV6_RA_GUARD:
                m->setRaGuard(pNsmGlobalUpdatePoIntfMessage->getRaGuard());
                break;
        }


		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback),
			pDceLinearSeqContext);
		prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);

		int ret = SET_LOCATION_ID_API_ERROR;
		ret = NsmUtils::setLocationIdsForPoSubModeCmds(pNsmGlobalUpdatePoIntfMessage->getPoId(), send_ctx_p, false);

		if(ret != SET_LOCATION_ID_API_SUCCESS)
		{
        	trace(TRACE_LEVEL_ERROR, "In updatePoIntfStep, call to setLocationIdsForPoSubModeCmds failed ");
        	pNsmGlobalUpdatePoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            if (NULL != send_ctx_p) {
                delete send_ctx_p;
                send_ctx_p = NULL;
            }
			return;
		}
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);

        pNsmGlobalUpdatePoIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        return;
    }

    void  IntfConfGlobalWorker::NsmGlobalUpdatePoIntfMessageHandler( NsmGlobalUpdatePoIntfMessage *pNsmGlobalUpdatePoIntfMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::validatePoIntfStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updatePoIntfStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updatePoIntfMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };

        DceLinearSeqContext *pDceLinearSeqContext = new DceLinearSeqContext (pNsmGlobalUpdatePoIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pDceLinearSeqContext->holdAll ();
        pDceLinearSeqContext->start();
    }

   void IntfConfGlobalWorker::validateVlanIntfUpdateStep (DceLinearSeqContext *pPrismLinearSequencerContext)
	{

	NsmGlobalUpdateVlanIntfMessage *pNsmGlobalUpdateVlanIntfMessage =
			dynamic_cast<NsmGlobalUpdateVlanIntfMessage *>(pPrismLinearSequencerContext->getPPrismMessage());
	UI32 cmdCode = pNsmGlobalUpdateVlanIntfMessage->getCmdCode();
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	UI32 vlanId = pNsmGlobalUpdateVlanIntfMessage->getVlanId();

	WaveManagedObjectSynchronousQueryContext vlanQueryCtxt(VlanIntfManagedObject::getClassName());
	vlanQueryCtxt.addAndAttribute (new AttributeUI32(&vlanId,"id"));
	vector<WaveManagedObject *> *pResults = querySynchronously (&vlanQueryCtxt);
	VlanIntfManagedObject *pVlanMO = NULL;

	if ((pResults != NULL) && (pResults->size() != 0))
	{
		pVlanMO = dynamic_cast<VlanIntfManagedObject *>((*pResults)[0]);
		pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
		pResults->clear();
		delete pResults;
	}
	else
	{
		status = WRC_NSM_API_ERR_VLAN_NOT_EXIST;
		pPrismLinearSequencerContext->executeNextStep(status);

		return;
	}
   
   tracePrintf(TRACE_LEVEL_DEBUG, "IntfConfGlobalWorker::validateVlanIntfUpdateStep cmdCode= %d, flag=%u", cmdCode, pNsmGlobalUpdateVlanIntfMessage->getFlag());

    if (cmdCode == NSMUPDATE_VLAN_STP_STATUS && pNsmGlobalUpdateVlanIntfMessage->getFlag() == false) {
        // If it is a GVLAN and MSTP is configured, then do not allow it
        if (pNsmGlobalUpdateVlanIntfMessage->getVlanId() > MAX_DOT1Q_VLAN_ID) {
            bool mstpConfigured = true;
            status = MstpToolKit::isMstpEnabled(mstpConfigured);
            tracePrintf(TRACE_LEVEL_DEBUG, "IntfConfGlobalWorker::validateVlanIntfUpdateStep MSTP configuration state is %d ", mstpConfigured);
            if (status == WAVE_MESSAGE_SUCCESS) {
                if (mstpConfigured == true) {
                    pPrismLinearSequencerContext->executeNextStep(WRC_NSM_API_ERR_GVLAN_MSTP_ENABLE_MSTP_CONFIGURED);
                    return;
                }
            } else {
                pPrismLinearSequencerContext->executeNextStep(status);
            }
        }
    }

	bool isReplayInProgress = (FrameworkToolKit::isFileReplayInProgress() || FrameworkToolKit::isConfigReplayInProgress());

	if(isReplayInProgress){
		trace(TRACE_LEVEL_DEBUG,string(" Global isReplayInProgress  --\n"));
	}

	if ((cmdCode == NSMUPDATE_VLAN_PVLAN_ADD_SEC_ASSOC_TYPE
			|| cmdCode == NSMUPDATE_VLAN_PVLAN_DEL_SEC_ASSOC_TYPE) && !isReplayInProgress) {

		//is Valid range
		if (NsmUtils::isAllUI32RangeVlanConfigured(pNsmGlobalUpdateVlanIntfMessage->getPvlanSecAssocRange())== WAVE_MESSAGE_ERROR) {
			status = WRC_NSM_API_ERR_VLAN_NOT_CONFIGURED;
		}

		//is Primary and secondary same
		else if (PVlanUtils::isPVlanIdPresent(pNsmGlobalUpdateVlanIntfMessage->getPvlanSecAssocRange(), vlanId)){
			status = WRC_NSM_PVLAN_SAME_PRIM_SEC_VLAN;
		}

		//is Primary Vlan
		else if (pVlanMO->getPVlanType() != PVLAN_TYPE_PRIMARY) {
			status = WRC_NSM_PVLAN_NOT_PRIM_VLAN;
		}

		//isIsolated Vlan Count greater than 1
		else if (PVlanUtils::isIsolatedVlanCountMore(pNsmGlobalUpdateVlanIntfMessage->getPvlanSecAssocRange())) {
			status = WRC_NSM_PVLAN_ISOLATED_VLAN_COUNT;
			trace(TRACE_LEVEL_ERROR,	string("IntfConfGlobalWorker  validateVlanIntfUpdateStep isIsolatedVlanCountMore"));
		}

		//Are all secondary Vlans
		else if (!PVlanUtils::isSecondaryVlanRange(pNsmGlobalUpdateVlanIntfMessage->getPvlanSecAssocRange())) {
			status = WRC_NSM_PVLAN_NOT_SEC_VLAN;
			trace(TRACE_LEVEL_ERROR,string("IntfConfGlobalWorker  validateVlanIntfUpdateStep isSecondaryVlanRange"));
		}

		else if((cmdCode == NSMUPDATE_VLAN_PVLAN_ADD_SEC_ASSOC_TYPE) )
		{

			if(PVlanUtils::doesSecPVlanRangeExceedMax (vlanId, pNsmGlobalUpdateVlanIntfMessage->getPvlanSecAssocRange(), pPrismLinearSequencerContext))
			{
				// here we check if after assoc new sec pvlans, if it exceeds max supported assoc limit, we throw an error
				status = WRC_NSM_ASSOC_PVLAN_EXCEED_MAX;
			}

			//isSecondary Vlan Part of any domain
			else if(PVlanUtils::isSecVlanPartOfDomain(vlanId, pNsmGlobalUpdateVlanIntfMessage->getPvlanSecAssocRange(), pPrismLinearSequencerContext))
			{
				status = WRC_NSM_PVLAN_SEC_ALREADY_IN_DOMAIN;
				trace(TRACE_LEVEL_ERROR,string(" validateVlanIntfUpdateStep isSecVlanPartOfDomain"));
			}

		}

	}

	 else if(cmdCode == NSMUPDATE_VLAN_PVLAN_TYPE){


		//Default Vlan 
		if(vlanId == DCM_NSM_VLAN_DEFAULT_VID){
			status = WRC_NSM_PVLAN_DEFAULT_VLAN;
		}

		//isVe configured
		else if (PVlanUtils::isVeConfigured(vlanId, pPrismLinearSequencerContext, pVlanMO)) {
			status = WRC_NSM_PVLAN_VE_VLAN;
		}

		//check static mac configured
		else if(PVlanUtils::isStaticMacConfigured(vlanId)){
			status = WRC_NSM_PVLAN_STATIC_MAC_VLAN;
		}

		else if(NsmUtils::isAnyPortAMemberofVlan(vlanId, pPrismLinearSequencerContext) != WRC_WAVE_MESSAGE_SUCCESS)
		{
			status = WRC_NSM_PVLAN_MEMBER_PORTS_VLAN;
		}

		//if added to MSTP instance
		else if(PVlanUtils::isMSTPInstancePresent(vlanId, pPrismLinearSequencerContext, pVlanMO))
		{
			status = WRC_NSM_PVLAN_MSTP_VLAN;
		}

		//isClassifier present
		else if(PVlanUtils::isClassifierPresentOnVlan(vlanId, pPrismLinearSequencerContext, pVlanMO))
		{
			status = WRC_NSM_PVLAN_CLASSIFIER_PRESENT;
		}

		//isL2ACL configured
		else if(pVlanMO->getIngressEgressMacAclPolicy().size() > 0)
		{
			status = WRC_NSM_PVLAN_L2ACL_CONFIGURED;
		}

		//isRSpan configured
		if(pVlanMO->getRemoteSpanEnabled())
		{
			status = WRC_NSM_PVLAN_RSPAN_PRESENT;
		}

		//if Igmp snooping is enabled on this vlan, return error for
		//secondary vlan configuration - This validation is removed as this was blocking PVLAN configuration on vlans on which IGMP snooping is not enabled.
                //This is because when IGMP snooping is enabled globally, plugin sets IGMP snooping enable in each and every vlan in MO though backend enables it on 256 vlans only
		else if(pNsmGlobalUpdateVlanIntfMessage->getPvlanType() == PVLAN_TYPE_PRIMARY)
		{

			UI32 count = PVlanUtils::getPVlanDomainCount();;

			trace(TRACE_LEVEL_DEBUG,string(" pvlan domain count is ") + count);

			if((count == NSM_MAX_NUM_PVLAN_DOMAIN_SUPPORTED_CAL) && Capabilities::isCapabilityPresent(FEAT_CAPABILITY_31_PVLAN_DOMAIN) ) {

				status = WRC_NSM_PVLAN_DOMAIN_EXCEED_MAX_CAL;

			} else if((count == NSM_MAX_NUM_PVLAN_DOMAIN_SUPPORTED_MER) && Capabilities::isCapabilityPresent(FEAT_CAPABILITY_1000_PVLAN_DOMAIN))  {

				status = WRC_NSM_PVLAN_DOMAIN_EXCEED_MAX_MER;

			} 
	      } 
	}
	else if(cmdCode == NSMUPDATE_VLAN_REMOTE_SPAN){
		if(vlanId != DCM_NSM_VLAN_DEFAULT_VID) {
			if(NsmUtils::isAnyPortAMemberofVlan(vlanId,
				pPrismLinearSequencerContext) != WRC_WAVE_MESSAGE_SUCCESS) {
				NSM_PLUG_DBG_ERR_S(string("The VLAN has member ports. Cannot configure/unconfigure RSPAN VLAN"));
				status = WRC_NSM_RSPAN_VLAN_HAS_MEMBER_PORTS;
			}
			if(pNsmGlobalUpdateVlanIntfMessage->getRemoteSpanEnabled()) {
				if (pVlanMO->getPVlanType() != PVLAN_TYPE_UNKNOWN) {
					NSM_PLUG_DBG_S(string("Error: Private Vlan cannot be made RSPAN Vlan"));
					status = WRC_SPAN_PRIVATE_VLAN_CANNOT_BE_RSPAN_VLAN;
				}
				//if the vlan has any spanning tree configuration, it should not be made a remote-span vlan.
				else if(PVlanUtils::isMSTPInstancePresent(vlanId, pPrismLinearSequencerContext, pVlanMO) || MstpToolKit::isXstpConfigPresent(vlanId))
				{
					NSM_PLUG_DBG_S(string("The VLAN has spanning tree configs. Cannot make it remote-span VLAN "));
					status = WRC_SPAN_STPCONFIGURED_CANNOT_BE_RSPAN_VLAN;
				}
				else if(pVlanMO->getIngressEgressMacAclPolicy().size() > 0)
				{
					NSM_PLUG_DBG_ERR_S(string("The VLAN has ACL Configuration. Cannot configure RSPAN VLAN"));
					status = WRC_SPAN_ACL_PRESENT_CANNOT_BE_RSPAN_VLAN;
				}
				else if(NsmUtils::isIgmpSnoopingConfiguredOnVlan(vlanId))
				{
					NSM_PLUG_DBG_ERR_S(string("The VLAN has IGMP Snooping Configuration. Cannot configure RSPAN VLAN"));
					status = WRC_SPAN_IGMPS_PRESENT_CANNOT_BE_RSPAN_VLAN;
				}
				else if(PVlanUtils::isClassifierPresentOnVlan(vlanId, pPrismLinearSequencerContext, pVlanMO))
				{
					NSM_PLUG_DBG_ERR_S(string("The VLAN has classfier Configuration. Cannot configure RSPAN VLAN"));
					status = WRC_SPAN_CLASSIFIER_PRESENT_CANNOT_BE_RSPAN_VLAN;
				}
				else if(PVlanUtils::isStaticMacConfigured(vlanId))
				{
					NSM_PLUG_DBG_ERR_S(string("The VLAN has static mac Configuration. Cannot configure RSPAN VLAN"));
					status = WRC_SPAN_STATICMAC_PRESENT_CANNOT_BE_RSPAN_VLAN;
				}
				else if (PVlanUtils::isVeConfigured(vlanId, pPrismLinearSequencerContext, pVlanMO)) {
					NSM_PLUG_DBG_ERR_S(string("The VLAN has associated Ve config. Cannot configure RSPAN VLAN"));
					status = WRC_SPAN_VE_PRESENT_CANNOT_BE_RSPAN_VLAN;
				}
			}
			else {
				//Trying to unconfigure remote-span on vlan. Validate if there is any active session with this remote-span valan.
				NSM_PLUG_DBG_S(string("Unconfiguring remote-span on vlan ")+vlanId+string(".Check if there is any active RSPAN session with this vlan"));
				if(NsmUtils::isVlanPartOfRspanSession(vlanId)) {
					NSM_PLUG_DBG_S(string("Error: Vlan is part of active RSPAN session"));
					status = WRC_NSM_RSPAN_VLAN_PARTOF_ACTIVE_RSPAN_CONFIG_CANNOT_BE_DELETED;
				}
			}
		}
		else {
			NSM_PLUG_DBG_S("Error: Default Vlan cannot be made RSPAN Vlan");
			status = WRC_SPAN_DEFAULT_VLAN_CANNOT_BE_RSPAN_VLAN;
		}
	}

	pPrismLinearSequencerContext->executeNextStep(status);
	return;
}


    void IntfConfGlobalWorker::updateVlanIntfMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        NsmGlobalUpdateVlanIntfMessage *pNsmGlobalUpdateVlanIntfMessage = dynamic_cast<NsmGlobalUpdateVlanIntfMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

        UI32 vlan_id = pNsmGlobalUpdateVlanIntfMessage->getVlanId();
        //ResourceId  status   = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlan_id,"id"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                //status = WAVE_MESSAGE_ERROR;
            }else if (1 == numberOfResults) {
                pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResults)[0]);
                //status = WAVE_MESSAGE_SUCCESS;
            } else {
                //status = WAVE_MESSAGE_ERROR;
            }
        }


        if (pVlanIntfManagedObject == NULL) {
	  		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);	
            return;
        } else {

            updateWaveManagedObject (pVlanIntfManagedObject);

            prismAssert (NULL != pVlanIntfManagedObject, __FILE__, __LINE__);

            trace (TRACE_LEVEL_INFO, string ("\tDesc :: ") + pVlanIntfManagedObject->getDesc());
            switch(pNsmGlobalUpdateVlanIntfMessage->getCmdCode())
            {
                case NSMUPDATE_VLAN_DESC :
                case NSMUPDATE_VLAN_DELDESC :
                    pVlanIntfManagedObject->setDesc (pNsmGlobalUpdateVlanIntfMessage->getDesc());
                    pVlanIntfManagedObject->addAttributeToBeUpdated("description");
                    break;
                case NSMUPDATE_VLAN_STP_STATUS :
                    pVlanIntfManagedObject->setStpDisable (pNsmGlobalUpdateVlanIntfMessage->getFlag());
                    pVlanIntfManagedObject->addAttributeToBeUpdated("stpDisable");
                    break;
                case NSMUPDATE_VLAN_MTU :
                case NSMUPDATE_VLAN_DELMTU :
                    pVlanIntfManagedObject->setMtu (pNsmGlobalUpdateVlanIntfMessage->getMtu());
                    pVlanIntfManagedObject->addAttributeToBeUpdated("mtu");
                    break;
				case NSMUPDATE_VLAN_PVLAN_TYPE:
				{
					pVlanIntfManagedObject->setPVlanType(pNsmGlobalUpdateVlanIntfMessage->getPvlanType());
					pVlanIntfManagedObject->addAttributeToBeUpdated("pvlantype");
					trace(TRACE_LEVEL_INFO, string("IntfConfGlobalW NSMUPDATE_VLAN_PVLAN_TYPE")+ pNsmGlobalUpdateVlanIntfMessage->getPvlanType());
					break;
				}

				case NSMUPDATE_VLAN_DELPVLAN_TYPE: {
					pVlanIntfManagedObject->setPVlanType(pNsmGlobalUpdateVlanIntfMessage->getPvlanType());
					trace(TRACE_LEVEL_INFO, string("IntfConfGlobalW NSMUPDATE_VLAN_DELPVLAN_TYPE")+ pNsmGlobalUpdateVlanIntfMessage->getPvlanType());
					trace(TRACE_LEVEL_DEBUG, string("IntfConfGlobalW vlanTypeRem -- Going to remove all pvlan associations"));
					pVlanIntfManagedObject->setPvlanSecAssocRange(UI32Range(""));
					pVlanIntfManagedObject->addAttributeToBeUpdated("pvlantype");
					pVlanIntfManagedObject->addAttributeToBeUpdated("pVlanSecAssocRange");
					//removePVlanAssociations(vlan_id, pPrismLinearSequencerContext);
					removePVlanHostAssociation(vlan_id, pPrismLinearSequencerContext);
					removePVlanTrunkAssociation(vlan_id, pPrismLinearSequencerContext);
					removePVlanMapping(vlan_id, pPrismLinearSequencerContext);
					break;
				}

		case NSMUPDATE_VLAN_PVLAN_ADD_SEC_ASSOC_TYPE:
		{
			trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW NSMUPDATE_VLAN_PVLAN_ADD_SEC_ASSOC_TYPE  updateVlanIntfMOStep"));

			UI32Range msgVlanrange = pNsmGlobalUpdateVlanIntfMessage->getPvlanSecAssocRange();
			UI32Range moVlanrange = pVlanIntfManagedObject->getPvlanSecAssocRange();

			pVlanIntfManagedObject->setPvlanSecAssocRange(moVlanrange + msgVlanrange);
			pVlanIntfManagedObject->addAttributeToBeUpdated("pVlanSecAssocRange");
			break;
		}
		case NSMUPDATE_VLAN_PVLAN_DEL_SEC_ASSOC_TYPE:
		{
			trace(TRACE_LEVEL_INFO,string(" IntfConfGlobalW NSMUPDATE_VLAN_PVLAN_DEL_SEC_ASSOC_TYPE  updateVlanIntfMOStep"));

			UI32Range msgVlanrange = pNsmGlobalUpdateVlanIntfMessage->getPvlanSecAssocRange();
			UI32Range moVlanrange = pVlanIntfManagedObject->getPvlanSecAssocRange();

			pVlanIntfManagedObject->setPvlanSecAssocRange(moVlanrange - msgVlanrange);
			pVlanIntfManagedObject->addAttributeToBeUpdated("pVlanSecAssocRange");

			removePVlanHostAssociation(vlan_id, pPrismLinearSequencerContext, msgVlanrange);
			removePVlanTrunkAssociation(vlan_id, pPrismLinearSequencerContext, msgVlanrange);
			removePVlanMapping(vlan_id, pPrismLinearSequencerContext, msgVlanrange);
			break;
		}
		case NSMUPDATE_VLAN_DELPVLAN_SEC_ASSOC_TYPE :
		{
			trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW sec pvlan assoc  updateVlanIntfMOStep NSMUPDATE_VLAN_DELPVLAN_SEC_ASSOC_TYPE"));
			pVlanIntfManagedObject->setPvlanSecAssocRange(pNsmGlobalUpdateVlanIntfMessage->getPvlanSecAssocRange());
			pVlanIntfManagedObject->addAttributeToBeUpdated("pVlanSecAssocRange");
			break;
		}
		case NSMUPDATE_VLAN_REMOTE_SPAN :
		{
			trace (TRACE_LEVEL_INFO, string ("IntfConfGlobalWorker: Updating RemoteSpan Configuration to ")+
			pNsmGlobalUpdateVlanIntfMessage->getRemoteSpanEnabled());
			pVlanIntfManagedObject->setRemoteSpanEnabled(pNsmGlobalUpdateVlanIntfMessage->getRemoteSpanEnabled());
			pVlanIntfManagedObject->addAttributeToBeUpdated("remoteSpanEnabled");
			break;
		}
	}
	}
		if (NULL != pResults) 
		{
			if(pPrismLinearSequencerContext)
				pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			delete pResults;
		}

		trace (TRACE_LEVEL_INFO, string ("\tDesc :: ") + pNsmGlobalUpdateVlanIntfMessage->getCompletionStatus());
		pPrismLinearSequencerContext->executeNextStep(pNsmGlobalUpdateVlanIntfMessage->getCompletionStatus ());

		return;
	}

void IntfConfGlobalWorker::updateVlanIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        NsmGlobalUpdateVlanIntfMessage *pNsmGlobalUpdateVlanIntfMessage = dynamic_cast<NsmGlobalUpdateVlanIntfMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
		trace (TRACE_LEVEL_DEBUG, string("IntfConfGlobalWorker::updateVlanIntfStep: Start"));

		if (pNsmGlobalUpdateVlanIntfMessage->getCmdCode() == NSMUPDATE_VLAN_STP_STATUS) {
	        UI32 vlanId = pNsmGlobalUpdateVlanIntfMessage->getVlanId();

	        pNsmGlobalUpdateVlanIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS); // Workaround

			(NsmGlobalObjectManager::getInstance()->getIntfXstpGlobalConfWorker())->updateVlanIntfXstp(
			        vlanId, pNsmGlobalUpdateVlanIntfMessage->getFlag(), pPrismLinearSequencerContext);
			return;
		}

        WaveSendToClusterContext    *send_ctx_p;
        NsmUpdateVlanIntfSPMessage *m = new NsmUpdateVlanIntfSPMessage();
        //"tm","tt",1,"testing");

        m->setVlanId (pNsmGlobalUpdateVlanIntfMessage->getVlanId());
        m->setCmdCode (pNsmGlobalUpdateVlanIntfMessage->getCmdCode());

        switch(m->getCmdCode())
        {
            case NSMUPDATE_VLAN_DESC :
            case NSMUPDATE_VLAN_DELDESC :
                    m->setDesc (pNsmGlobalUpdateVlanIntfMessage->getDesc());
                    break;
            case NSMUPDATE_VLAN_MTU :
            case NSMUPDATE_VLAN_DELMTU :
                    m->setMtu (pNsmGlobalUpdateVlanIntfMessage->getMtu());
                    break;
            case NSMUPDATE_VLAN_PVLAN_TYPE :
            case NSMUPDATE_VLAN_DELPVLAN_TYPE :
                    m->setPvlanType (pNsmGlobalUpdateVlanIntfMessage->getPvlanType());
                    trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW delPvlan ")+pNsmGlobalUpdateVlanIntfMessage->getPvlanType());
            		break;


            case NSMUPDATE_VLAN_PVLAN_ADD_SEC_ASSOC_TYPE :
            case NSMUPDATE_VLAN_PVLAN_DEL_SEC_ASSOC_TYPE :
            case NSMUPDATE_VLAN_DELPVLAN_SEC_ASSOC_TYPE :
            {
            	m->setPvlanSecAssocRange(pNsmGlobalUpdateVlanIntfMessage->getPvlanSecAssocRange());
            	 trace(TRACE_LEVEL_INFO,string("IntfConfGlobalW add sec assoc updateVlanIntfStep"));

            	bool isReplayInProgress = (FrameworkToolKit::isFileReplayInProgress() || FrameworkToolKit::isConfigReplayInProgress());
            	if(isReplayInProgress)
            	{
            		trace(TRACE_LEVEL_DEBUG,string(" Global  isReplayInProgress  set --\n"));
            		m->setReplayInProgress(isReplayInProgress);
            	}

            	break;
            }
            case NSMUPDATE_VLAN_REMOTE_SPAN :
            {
            	trace (TRACE_LEVEL_DEVEL, string("IntfConfGlobalWorker::updateVlanIntfStep: Changing the "
            			"remote-span config to =") + pNsmGlobalUpdateVlanIntfMessage->getRemoteSpanEnabled());
            	m->setRemoteSpanEnabled(pNsmGlobalUpdateVlanIntfMessage->getRemoteSpanEnabled());
            	break;
            }
            default:
                    break;
        }

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback),
			pPrismLinearSequencerContext);
		prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);

        pNsmGlobalUpdateVlanIntfMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        return;
    }

    void  IntfConfGlobalWorker::NsmGlobalUpdateVlanIntfMessageHandler( NsmGlobalUpdateVlanIntfMessage *pNsmGlobalUpdateVlanIntfMessage)
    {

// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::validateVlanIntfUpdateStep),

            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::updateVlanIntfStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::updateVlanIntfMOStep),
 
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };
//        if(pNsmGlobalUpdateVlanIntfMessage->getCmdCode() == )
        DceLinearSeqContext *pDceLinearSeqContext = new DceLinearSeqContext(pNsmGlobalUpdateVlanIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pDceLinearSeqContext->holdAll ();
        pDceLinearSeqContext->start ();
    }

	void
	IntfConfGlobalWorker::LacpGlobalConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
		trace (TRACE_LEVEL_DEVEL, "LacpGlobalConfigSendToClusterCallback::Entering");
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
	    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>(pWaveSendToClusterContext->getPCallerContext());
		if(sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS)
			trace (TRACE_LEVEL_DEVEL, "Phase 1 message succeeded on all locations");
//		else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
//			trace (TRACE_LEVEL_DEVEL, "Phase 1 message failed on all locations");
		else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED)
			trace (TRACE_LEVEL_DEVEL, "Phase 1 message failed on some of the locations and Rollback succeeded on all of the nodes");
		else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_FAILED)
			trace (TRACE_LEVEL_DEVEL, "Phase 1 message failed on some of the locations and Rollback attempt on the succeeded nodes failed on some or all nodes");
		else
			trace (TRACE_LEVEL_DEVEL, string("getCompletionStatus:: Unknown Error : ") + sendToClusterCompletionStatus);
				

        pPrismLinearSequencerContext->executeNextStep(sendToClusterCompletionStatus);
		delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
		delete pWaveSendToClusterContext;
		trace (TRACE_LEVEL_DEVEL, "LacpGlobalConfigSendToClusterCallback::Done");
        return;
    }
	
	void IntfConfGlobalWorker::updateLacpConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        NsmGlobalUpdateLacpConfigMessage *pNsmGlobalUpdateLacpConfigMessage = dynamic_cast<NsmGlobalUpdateLacpConfigMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

//      int status;
        NsmUpdateLacpConfigSPMessage *m = new NsmUpdateLacpConfigSPMessage();
		WaveSendToClusterContext    *pWaveSendToClusterContext;

		/* create clustercontext and initialize with callback func */
		pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::LacpGlobalConfigSendToClusterCallback), pPrismLinearSequencerContext);

		trace (TRACE_LEVEL_INFO, "IntfConfGlobalWorker::updateLacpConfigStep");
        m->setSystemPriority (pNsmGlobalUpdateLacpConfigMessage->getSystemPriority());
        m->setDot1xenable (pNsmGlobalUpdateLacpConfigMessage->getDot1xenable());
        m->setDot1xtesttimeout (pNsmGlobalUpdateLacpConfigMessage->getDot1xtesttimeout());
        m->setNativevlanDisable (pNsmGlobalUpdateLacpConfigMessage->getNativevlanDisable());
        m->setCmdCode (pNsmGlobalUpdateLacpConfigMessage->getCmdCode());

		/* Set Phase 1 message */
		pWaveSendToClusterContext->setPPrismMessageForPhase1(m);
		/* Send the message to all nodes in the cluster */
       
		pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true); 
		pWaveSendToClusterContext->setPartialSuccessFlag(true);
        sendToWaveCluster (pWaveSendToClusterContext);
		pNsmGlobalUpdateLacpConfigMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        return;
    }

    /* Code Starts Here */
    void IntfConfGlobalWorker::updateLacpConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        NsmGlobalUpdateLacpConfigMessage *pNsmGlobalUpdateLacpConfigMessage = dynamic_cast<NsmGlobalUpdateLacpConfigMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
        
        //ResourceId  					status   = WAVE_MESSAGE_ERROR;
		vector<WaveManagedObject *>     *pResults    = querySynchronously (LacpConfigManagedObject::getClassName ());
        
		NSM_PLUG_DBG("Entering ...");
		if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
				NSM_PLUG_DBG("Global Config Managed Object doesn't exist...");
                //status = WAVE_MESSAGE_ERROR;
            }else if (1 == numberOfResults) {
        		LacpConfigManagedObject *pLacpConfigManagedObject = NULL;
                pLacpConfigManagedObject = dynamic_cast<LacpConfigManagedObject *>((*pResults)[0]);
				updateWaveManagedObject (pLacpConfigManagedObject);
				if (pNsmGlobalUpdateLacpConfigMessage->getCmdCode() == LACPUPDATE_SYS_PRIO) {
	        		pLacpConfigManagedObject->setSystemPriority (
						pNsmGlobalUpdateLacpConfigMessage->getSystemPriority());
					NSM_PLUG_DBG_S(string("LACP:SystemPriority: ") + 
						pNsmGlobalUpdateLacpConfigMessage->getSystemPriority());
				} else if (pNsmGlobalUpdateLacpConfigMessage->getCmdCode() == DOT1XUPDATE_STATUS) {
		        	pLacpConfigManagedObject->setDot1xenable (
						pNsmGlobalUpdateLacpConfigMessage->getDot1xenable());
					NSM_PLUG_DBG_S(string("DOT1X:Dot1xenable: ") + 
						pNsmGlobalUpdateLacpConfigMessage->getDot1xenable());
				} else if (pNsmGlobalUpdateLacpConfigMessage->getCmdCode() == NSMUPDATE_NATIVE_VLAN){
	        		pLacpConfigManagedObject->setNativevlanDisable (
						pNsmGlobalUpdateLacpConfigMessage->getNativevlanDisable());
					NSM_PLUG_DBG_S(string("NATIVE-VLAN:NativevlanDisable: ") + 
						pNsmGlobalUpdateLacpConfigMessage->getNativevlanDisable());
				} else if (pNsmGlobalUpdateLacpConfigMessage->getCmdCode() == DOT1XUPDATE_TESTTIMEOUT) {
                    pLacpConfigManagedObject->setDot1xtesttimeout (
                        pNsmGlobalUpdateLacpConfigMessage->getDot1xtesttimeout());
                    NSM_PLUG_DBG_S(string("DOT1X:Dot1xtesttimeout: ") +
                        pNsmGlobalUpdateLacpConfigMessage->getDot1xtesttimeout());
				}
                //status = WAVE_MESSAGE_SUCCESS;
            } else {
                //status = WAVE_MESSAGE_ERROR;
            }
            // Mobin: mark the query results for GC.  
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(
                    *pResults);
            delete pResults;
        }

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

		return;
    }
	
    
	void  IntfConfGlobalWorker::NsmGlobalUpdateLacpConfigMessageHandler( NsmGlobalUpdateLacpConfigMessage *pNsmGlobalUpdateLacpConfigMessage)
    {
    
        PrismLinearSequencerStep sequencerSteps[] =
        {
			// Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updateLacpConfigStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
			// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updateLacpConfigMOStep),
            
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };

		PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNsmGlobalUpdateLacpConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();

    }

    void
    IntfConfGlobalWorker::NsmGlobalFcoeVlanSendToClusterCallback (PrismAsynchronousContext *ctx_p)
    {
        PrismLinearSequencerContext             *seq_ctx_p = NULL;
        WaveSendToClusterContext                *send_ctx_p = NULL;
        PrismMessage                            *gl_msg_p = NULL;
        PrismMessage                            *loc_msg_p = NULL;
        vector<LocationId>                      loc_vector;
        ResourceId                              status = WAVE_MESSAGE_SUCCESS;
        ResourceId                              final_status = WAVE_MESSAGE_SUCCESS;
        UI32                                    i = 0;

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
        
        gl_msg_p = seq_ctx_p->getPPrismMessage ();
        NSM_PLUG_ASSERT_PTR(gl_msg_p);

        send_ctx_p = dynamic_cast<WaveSendToClusterContext *> (ctx_p);

        loc_vector = send_ctx_p->getLocationsToSendToForPhase1();
        for (i = 0; i < loc_vector.size(); i++) {
            status = ClusterGetPhase1StatusFromContextForLocation(send_ctx_p, loc_vector[i]);
            FCOE_PLUG_DBG_S(string("Apply Status of Location[") + loc_vector[i] +
                            string("] = ") + FrameworkToolKit::localize(status));
            loc_msg_p = send_ctx_p->getResultingMessageForPhase1(loc_vector[i]);
            FCOE_PLUG_ASSERT(loc_msg_p);
            loc_msg_p->transferAllBuffers(gl_msg_p);
        }

        /* Dump the Rollback Status */
        loc_vector = send_ctx_p->getLocationsToSendToForPhase2();
        for (i = 0; i < loc_vector.size(); i++) {
            status = ClusterGetPhase2StatusFromContextForLocation(send_ctx_p, loc_vector[i]);
            FCOE_PLUG_DBG_S(string("Rollback Status of Location[") + loc_vector[i] +
                            string("] = ") + FrameworkToolKit::localize(status));
        }

        final_status = ClusterGetStatusFromContext(send_ctx_p);
        FCOE_PLUG_DBG_S(string("Final status of Cluster Operation=") +
                        FrameworkToolKit::localize(final_status));
	    if(final_status  == WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED) {
                  final_status = FCOE_COMMON_ERR_VLAN_ROLLBACK_DUE_TO_PROVISION;
		}
        gl_msg_p->setCompletionStatus(final_status);
        
        seq_ctx_p->executeNextStep(final_status);

        // Delete the APPLY message
        loc_msg_p = send_ctx_p->getPPrismMessageForPhase1();
        if (loc_msg_p) {
            delete loc_msg_p;
        }

        // Delete the ROLLBACK message
        loc_msg_p = send_ctx_p->getPPrismMessageForPhase2();
        if (loc_msg_p) {
            delete loc_msg_p;
        }

        delete send_ctx_p;
        
        return;
    }


    void
    IntfConfGlobalWorker::NsmGlobalFcoeVlanSendToLocal(PrismLinearSequencerContext *seq_ctx_p)
    {
        NsmLocalFcoeVlanMessage     *loc_msg_p = NULL;
        NsmLocalFcoeVlanMessage     *rbk_msg_p = NULL;
        NsmGlobalFcoeVlanMessage    *msg_p = NULL;
        WaveSendToClusterContext    *send_ctx_p = NULL;
        
        msg_p = dynamic_cast<NsmGlobalFcoeVlanMessage *>(seq_ctx_p->getPPrismMessage ());

        trace(TRACE_LEVEL_INFO, string("Sending to NSM Local Cmd=") + msg_p->getCmdCode() +
                                string(" Old-Vlan=") + msg_p->getOldVlan() +
                                string("New-Vlan=") + msg_p->getNewVlan());

        loc_msg_p = new NsmLocalFcoeVlanMessage(msg_p->getCmdCode(), msg_p->getOldVlan(),
                                                msg_p->getNewVlan(), msg_p->getVlanName(),
                                                msg_p->getVlanIfIndex(), msg_p->getDesc());
        if (loc_msg_p == NULL) {
            seq_ctx_p->executeNextStep(FCOE_FAB_MAP_ERR_FAILED_TO_ALLOC_VLAN_MSG_TO_NSM_LOCAL);
            return;
        }

        rbk_msg_p = new NsmLocalFcoeVlanMessage(msg_p->getCmdCode(), msg_p->getNewVlan(),
                                                msg_p->getOldVlan(), msg_p->getVlanName(),
                                                msg_p->getVlanIfIndex(), msg_p->getDesc());
        if (rbk_msg_p == NULL) {
            seq_ctx_p->executeNextStep(FCOE_FAB_MAP_ERR_FAILED_TO_ALLOC_RBK_VLAN_MSG_TO_NSM_LOCAL);
            delete loc_msg_p;
            return;
        }

        send_ctx_p = new WaveSendToClusterContext(this,
                                                  reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::NsmGlobalFcoeVlanSendToClusterCallback),
                                                  seq_ctx_p);
        if (send_ctx_p == NULL) {
            seq_ctx_p->executeNextStep(FCOE_COMMON_ERR_FAILED_TO_ALLOC_CLUSTER_CONTEXT);
            delete loc_msg_p;
            delete rbk_msg_p;
            return;
        }

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        send_ctx_p->setPPrismMessageForPhase2(rbk_msg_p);
        sendToWaveCluster(send_ctx_p);

        return;
    }
/* Kunjan - No Need to create VlanIntfMO for FCOE Vlan
    ResourceId IntfConfGlobalWorker::NsmGlobalFcoeVlanHandleMO (PrismSynchronousLinearSequencerContext *ctx_p)
    {
        NsmGlobalFcoeVlanMessage *msg_p = dynamic_cast<NsmGlobalFcoeVlanMessage *>(ctx_p->getPPrismMessage ());
        VlanIntfManagedObject *vlan_mo_p;
        UI32 vlan_id;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = NULL;
        trace (TRACE_LEVEL_INFO, string("Recieved FCOE Update VLAN message Oper=")+msg_p->getCmdCode()); 
        switch (msg_p->getCmdCode()) {
            case NSM_FCOE_CREATE_VLAN:
                prismAssert (0, __FILE__, __LINE__);
                break;

            case NSM_FCOE_DELETE_VLAN:
                prismAssert (0, __FILE__, __LINE__);
                break;

            case NSM_FCOE_UPDATE_VLAN:
                trace (TRACE_LEVEL_INFO, "Recieved FCOE Update VLAN message");
                vlan_id = msg_p->getOldVlan();
                syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlan_id,"id"));
                pResults = querySynchronously (&syncQueryCtxt);
                if (NULL == pResults) {
                    trace (TRACE_LEVEL_INFO, "Query for Old VLAN returned NULL objects");
                    return WAVE_MESSAGE_ERROR;
                }
                
                if (pResults->size() == 1) {
                    vlan_mo_p = dynamic_cast<VlanIntfManagedObject *>((*pResults)[0]);
                    if (vlan_mo_p == NULL) {
						ctx_p->addManagedObjectsForGarbageCollection(*pResults);
						delete pResults;
                        return WAVE_MESSAGE_ERROR;
                    }

                    trace (TRACE_LEVEL_INFO, string ("\tDeleteing FCOE VlanId :: ") + vlan_mo_p->getId());
                    delete vlan_mo_p;
                } else {
                    trace (TRACE_LEVEL_INFO, "Old Vlan not found, Creating a new VLAN");
                }

                // First Delete the OLD FCOE VLAN
                vlan_mo_p = new VlanIntfManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
                
                trace (TRACE_LEVEL_INFO, string ("\tAdding FCOE VlanId :: ") + msg_p->getNewVlan()); 
                vlan_mo_p->setId(msg_p->getNewVlan());
                vlan_mo_p->setType(NSM_VLAN_INTF_TYPE_FCOE);
                vlan_mo_p->setIfName(toString(msg_p->getNewVlan())); //msg_p->getVlanName());
                vlan_mo_p->setIfIndex(msg_p->getVlanIfIndex());
                vlan_mo_p->setDesc(msg_p->getDesc());

				if (NULL != pResults) {
					delete pResults;
				}
				
				if (ctx_p) {
					ctx_p->addManagedObjectForGarbageCollection(vlan_mo_p);
				}

                break;

            default:
                break;
        }
       

        return msg_p->getCompletionStatus();
    }*/
    void IntfConfGlobalWorker::NsmGlobalReservedVlanMOStep (PrismLinearSequencerContext *ctx_p)
    {
        ResourceId statusForNextStep = WAVE_MESSAGE_ERROR;
        
        NsmGlobalReservedVlanMessage *msg_p = dynamic_cast<NsmGlobalReservedVlanMessage *>(ctx_p->getPPrismMessage ());
        prismAssert (NULL != msg_p, __FILE__, __LINE__);
        
        trace (TRACE_LEVEL_INFO, string("Recieved Reserved VLAN message Oper=")+msg_p->getCmdCode()); 
        
        switch (msg_p->getCmdCode())
        {
            case 1:
            {
                WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ReservedVlanManagedObject::getClassName());
                vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
                
                if (NULL == pResults)
                {
                    trace (TRACE_LEVEL_INFO, "Query for Reserved VLAN MO returned NULL objects");
                }
                else if (1 == pResults->size())
                {
                    ReservedVlanManagedObject *reservedvlan_mo_p = dynamic_cast<ReservedVlanManagedObject *>((*pResults)[0]);

                    if (NULL == reservedvlan_mo_p)
                    {
                        trace (TRACE_LEVEL_FATAL, "IntfConfGlobalWorker::NsmGlobalReservedVlanMOStep : dynamic cast to ReservedVlanManagedObject failed. System failure.");
                        prismAssert (false, __FILE__, __LINE__);
                    }
                    else
                    {
                        if (msg_p->getStart() > 0)
                        {
                          reservedvlan_mo_p->setStart(msg_p->getStart());
                        }
                        if (msg_p->getEnd() > 0)
                        {
                          reservedvlan_mo_p->setEnd(msg_p->getEnd());
                        }

                        updateWaveManagedObject (reservedvlan_mo_p);
                        
                        statusForNextStep = WAVE_MESSAGE_SUCCESS;
                    }
                } 
                else if (0 == pResults->size())
                {
                    trace (TRACE_LEVEL_INFO, "OM Reserved Vlan MO not found!");
                }
                else
                {
                    tracePrintf (TRACE_LEVEL_FATAL, "IntfConfGlobalWorker::NsmGlobalReservedVlanMOStep : number of ReservedVlanManagedObject [%u]. Only one Managed Object Expected.",  pResults->size());
                    prismAssert (false, __FILE__, __LINE__);
                }

                if (NULL != pResults)
                {
          				  ctx_p->addManagedObjectsForGarbageCollection(*pResults);
                    delete pResults;
                }
            }  
            break;

            default:
            {
                tracePrintf (TRACE_LEVEL_FATAL, "IntfConfGlobalWorker::NsmGlobalReservedVlanMOStep : Unknown commandCode [%u]", msg_p->getCmdCode());
                prismAssert (false, __FILE__,__LINE__);
            }
            break;
        }
       
        ctx_p->executeNextStep (statusForNextStep);
    }


    void IntfConfGlobalWorker::NsmGlobalFcoeVlanMessageHandler(NsmGlobalFcoeVlanMessage *fcoe_vlan_msg_p)
	{
			PrismLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::NsmGlobalFcoeVlanSendToLocal),
				/*reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismSynchronousLinearSequencerStartTransactionStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::NsmGlobalFcoeVlanHandleMO),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismSynchronousLinearSequencerCommitTransactionStep),*/
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
			};
			PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (fcoe_vlan_msg_p,
                							this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));\
            pPrismLinearSequencerContext->start ();
	}
    void IntfConfGlobalWorker::NsmGlobalReservedVlanMessageHandler(NsmGlobalReservedVlanMessage *msg_p)
	{
	 		PrismLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::NsmReservedVlanSendtoWaveClusterStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::NsmGlobalReservedVlanMOStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
			};
		  PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (msg_p , this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
      pPrismLinearSequencerContext->start ();
	}

	void IntfConfGlobalWorker::NsmGlobalQosPoValidationStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
        ObjectId poObjectId;
		bool isVcsEnabled = false;
		SI32 cmdCode = 0;
		NsmGlobalQosPOConfigMessage *pNsmGlobalQosPOConfigMessage =
		  dynamic_cast<NsmGlobalQosPOConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;

		//Validate existence of PO.
		PoIntfManagedObject *pPoIntfManagedObject = NULL;
		UI32 poId = pNsmGlobalQosPOConfigMessage->getPoId();
		WaveManagedObjectSynchronousQueryContext poIntsyncQueryCtxt(PoIntfManagedObject::getClassName());
		poIntsyncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
		vector<WaveManagedObject *> *poIntpResults = querySynchronously (&poIntsyncQueryCtxt);
		if (NULL != poIntpResults) {
			UI32 numberOfResults = poIntpResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
				status = WAVE_MESSAGE_ERROR;
			}else if (1 == numberOfResults) {
				trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
				pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*poIntpResults)[0]);
				trace (TRACE_LEVEL_DEBUG, string("pPoIntfManagedObject poId=") + pPoIntfManagedObject->getId());
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				status = WAVE_MESSAGE_ERROR;
			}
		}
        if (NULL == pPoIntfManagedObject) {
			pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            trace(TRACE_LEVEL_INFO, string("pPoIntfManagedObject is NULL: ") + pNsmGlobalQosPOConfigMessage->getCmdCode());
            status = WAVE_MESSAGE_ERROR;
        } 

		prismAssert (NULL != pPoIntfManagedObject, __FILE__, __LINE__);

        //If PO is port-profiled-port, qos configurations are not allowed on the same.
        //This validation is added in plugin, because empty PO doesnt exist in back end.
        bool isPortProfiled = false;
        APPMUtils::isPortProfilePort(pPoIntfManagedObject, isPortProfiled);
        if(isPortProfiled) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(poIntpResults);
            pPrismLinearSequencerContext->executeNextStep(SSM_QOS_CONFIG_NOT_ALLOWED_ON_PORT_PROFILED_PO);
            return;
        }

		cmdCode = pNsmGlobalQosPOConfigMessage->getCmdCode(); 
		//MQC and CEE cannot coexist, fix here
		if ((cmdCode != QOS_INTERFACE_CEE_MAP) && (cmdCode != QOS_INTERFACE_PRIORITY_TAG) && (cmdCode != QOS_INTERFACE_DEFAULT_COS))
		{
			if (pPoIntfManagedObject->getCeeMap() != ObjectId::NullObjectId)
			{
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(poIntpResults);
				pPrismLinearSequencerContext->executeNextStep(WRC_SSM_INTF_QOS_NOT_IN_NON_CEE);
				return;
			}
		}

		//CEE config conflicts with MLS/FCOE/MQC
		if (cmdCode == QOS_INTERFACE_CEE_MAP) {
            if (QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject)) {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(poIntpResults);
				pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_CEE_FCOE);
				return;
			}

            if (QosUtils::isInterfaceMQCConfigured(pPoIntfManagedObject)) {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
						poIntpResults);
				pPrismLinearSequencerContext->executeNextStep(
						WRC_SSM_QOS_FLOWBASED_SERVICE_POLICY_AND_CEE_CANNOT_CO_EXIST);
				return;
			}

            if (QosUtils::isInterfaceQosNonCeeConfigured(pPoIntfManagedObject)) {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
						poIntpResults);
				pPrismLinearSequencerContext->executeNextStep(
						WRC_SSM_INTF_QOS_CONFIG_CONFLICT);
				return;
			}
		}

		//Fcoe and MQC cannot coexist on an interface [physical interface /po]
		if (((FB_POLICYMAP_BIND_INTERFACE_IN == cmdCode)
			|| (FB_POLICYMAP_BIND_INTERFACE_OUT == cmdCode))) {

			if ((QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject))) {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
						poIntpResults);
				pPrismLinearSequencerContext->executeNextStep(
						WRC_SSM_QOS_FLOWBASED_SERVICE_POLICY_AND_FCOE_CANNOT_CO_EXIST);
				return;
			}

			if ((QosUtils::isInterfaceQosCeeConfigured(pPoIntfManagedObject))) {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
						poIntpResults);
				pPrismLinearSequencerContext->executeNextStep(
						WRC_SSM_QOS_FLOWBASED_SERVICE_POLICY_AND_CEE_CANNOT_CO_EXIST);
				return;
			}

			if ((QosUtils::isInterfaceQosMLSConfigured(pPoIntfManagedObject))) {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
						poIntpResults);
				pPrismLinearSequencerContext->executeNextStep(
						WRC_SSM_INTF_QOS_CONFIG_CONFLICT);
				return;
			}
		}

        poObjectId =  pPoIntfManagedObject->getObjectId();

        /* Retrive physical port managed objects of physical ports already assigned to the port channel */
        WaveManagedObjectSynchronousQueryContext pIntLocalsyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        pIntLocalsyncQueryCtxt.addAndAttribute (new AttributeObjectId(poObjectId,"poId"));
        vector<WaveManagedObject *> *pIntLocalpResults = querySynchronously (&pIntLocalsyncQueryCtxt);
        /* if( no members in po intf) */
        if (pIntLocalpResults->size() == 0) { /* No physical ports assigned to port-channel */
            NSM_PLUG_DBG_S( "No physical ports assigned to port-channel");
		} else {
                UI32 resultIter = 0;
                PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
				if(pNsmGlobalQosPOConfigMessage->getCmdCode() != QOS_INTERFACE_DEFAULT_COS) {
                	while (resultIter != pIntLocalpResults->size()) {
                    	pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pIntLocalpResults)[resultIter]);
                    	if(pPhyIntfLocalManagedObject->getPoId() == poObjectId)
                    	{
							//Check if CEE map is applied on LAG member
							if(ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getCeeMap()) {
								status = WRC_SSM_INTF_QOS_ON_LAG_MEMBER_IS_ALREADY_APPLIED;
                       			break;           
							}
							//Check if lag member is fcoe port	
						  	WaveManagedObjectPointer<FcoeportAttrLocalManagedObject>  pFcoeportAttrLocalManagedObject;
						  	pFcoeportAttrLocalManagedObject = WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> (pPhyIntfLocalManagedObject->getFcoeport());

							if(NULL != pFcoeportAttrLocalManagedObject.operator-> ()) {
								status = WRC_SSM_INTF_QOS_ON_LAG_FCOE_MEMBER;
								break;
							}
                            
                            //TR000392214 : Co existence of "qos flowcontrol" on the LAG and policer config on any LAG member.

                            if ((CONFIGURE_INTERFACE_QOS == pNsmGlobalQosPOConfigMessage->getOpCode()) && 
                                ((QOS_INTERFACE_FLOW_CONTROL_BOTH == pNsmGlobalQosPOConfigMessage->getCmdCode()) || 
                                (QOS_INTERFACE_FLOW_CONTROL_RX == pNsmGlobalQosPOConfigMessage->getCmdCode()) || 
                                (QOS_INTERFACE_FLOW_CONTROL_TX == pNsmGlobalQosPOConfigMessage->getCmdCode())) &&  
                                ((ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getInputPolicyMapId()) || 
                                 (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getOutputPolicyMapId())) && 
                                 ((pNsmGlobalQosPOConfigMessage->getFlowControlTx() == 1) || 
                                 (pNsmGlobalQosPOConfigMessage->getFlowControlRx() == 1))) {
                                
                                status = SSM_INTF_QOS_FLOWCONTROL_ON_LAG_CONFLICTS_WITH_POLICER_ON_MEMBER_PORT;
                                break;
                                
                            }

                    	}
                    	resultIter++;
                	}
				}
        		if (NULL != pIntLocalpResults)
				{
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pIntLocalpResults);
				}	
		}

		if( status != WAVE_MESSAGE_SUCCESS) {
       		if (NULL != poIntpResults)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(poIntpResults);
			pPrismLinearSequencerContext->executeNextStep(status);
			return;
		}

		if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled())
		{
			isVcsEnabled = true;
		}

		switch (pNsmGlobalQosPOConfigMessage->getOpCode())
		{
			case CONFIGURE_INTERFACE_QOS:
			{
				switch (pNsmGlobalQosPOConfigMessage->getCmdCode())
				{
					case QOS_INTERFACE_COS_MUTATION:
					{
					  	string mapName = pNsmGlobalQosPOConfigMessage->getCosToCosMutationMapName();
					    if(!QosUtils::doesMapExist(COS_MUTATION_MAP, mapName, status)) // status is set appropriately by this API 
						{
							trace (TRACE_LEVEL_DEBUG, string ("QOS_INTERFACE_COS_MUTATION map doesnt exist"));
						}

						if ((QosUtils::isInterfaceMQCConfigured(pPoIntfManagedObject)) || (QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject))) {
							status = WRC_SSM_INTF_QOS_CONFIG_CONFLICT;
						}

						break;
					}
					case QOS_INTERFACE_COS_TRAFFIC_CLASS:
					{
					  	string mapName = pNsmGlobalQosPOConfigMessage->getCosToTrafficClassMapName();
					    if(!QosUtils::doesMapExist(COS_TRAFFIC_CLASS_MAP, mapName, status)) // status is set appropriately by this API 
						{
							trace (TRACE_LEVEL_DEBUG, string ("QOS_INTERFACE_COS_TRAFFIC_CLASS map doesnt exist"));
						}
						if ((QosUtils::isInterfaceMQCConfigured(pPoIntfManagedObject)) || (QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject))) {
							status = WRC_SSM_INTF_QOS_CONFIG_CONFLICT;
						}
						break;
					}
					case QOS_INTERFACE_DSCP_MUTATION:
            		{
					  	string mapName = pNsmGlobalQosPOConfigMessage->getDscpToDscpMutationMapName();
					    if(!QosUtils::doesMapExist(DSCP_MUTATION_MAP, mapName, status)) // status is set appropriately by this API 
						{
							trace (TRACE_LEVEL_DEBUG, string ("QOS_INTERFACE_DSCP_MUTATION map doesnt exist"));
						}
						if ((QosUtils::isInterfaceMQCConfigured(pPoIntfManagedObject)) || (QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject))) {
							status = WRC_SSM_INTF_QOS_CONFIG_CONFLICT;
						}
						break;
					}
					case QOS_INTERFACE_DSCP_TRAFFIC_CLASS:
            		{
					  	string mapName = pNsmGlobalQosPOConfigMessage->getDscpToTrafficClassMapName();
						int reservePriorityBitmap = 0;
					    if(!QosUtils::doesMapExist(DSCP_TRAFFIC_CLASS_MAP, mapName, status)) // status is set appropriately by this API 
						{
							trace (TRACE_LEVEL_DEBUG, string ("QOS_INTERFACE_DSCP_TRAFFIC_CLASS map doesnt exist"));
						}
						if (status == WRC_WAVE_MESSAGE_SUCCESS)
						{
							if ((DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED))
							{
								status = QosUtils::getReservePriorityBitMapFromCeeMap(reservePriorityBitmap);
								if (status == WRC_WAVE_MESSAGE_SUCCESS)
								{
									status = QosUtils::vcsL3QosSanity(QOS_INTERFACE_DSCP_TRAFFIC_CLASS, mapName, true, reservePriorityBitmap, 0);
								}
							}
						}
						if ((QosUtils::isInterfaceMQCConfigured(pPoIntfManagedObject)) || (QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject))) {
							status = WRC_SSM_INTF_QOS_CONFIG_CONFLICT;
						}
						break;
					}
					case QOS_INTERFACE_DSCP_COS:
            		{
					  	string mapName = pNsmGlobalQosPOConfigMessage->getDscpToCosMapName();
						int reservePriorityBitmap = 0;
					    if(!QosUtils::doesMapExist(DSCP_COS_MAP, mapName, status)) // status is set appropriately by this API 
						{
							trace (TRACE_LEVEL_DEBUG, string ("QOS_INTERFACE_DSCP_COS map doesnt exist"));
						}
						if (status == WRC_WAVE_MESSAGE_SUCCESS)
						{
							if ((DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED))
							{
								status = QosUtils::getReservePriorityBitMapFromCeeMap(reservePriorityBitmap);
								if (status == WRC_WAVE_MESSAGE_SUCCESS)
								{
									status = QosUtils::vcsL3QosSanity(QOS_INTERFACE_DSCP_COS, mapName, true, reservePriorityBitmap, 0);
								}
							}
						}
						if ((QosUtils::isInterfaceMQCConfigured(pPoIntfManagedObject)) || (QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject))) {
							status = WRC_SSM_INTF_QOS_CONFIG_CONFLICT;
						}
						break;
					}
					case QOS_INTERFACE_FLOW_CONTROL_BOTH: {
						status = WAVE_MESSAGE_SUCCESS;
						if (pPoIntfManagedObject->getPfcFlowControl().size() != 0) {
							status = WRC_SSM_INTF_PFC_ALREADY_APPLIED;
						}
						if ((QosUtils::isInterfaceMQCConfigured(pPoIntfManagedObject)) || (QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject))) {
							status = WRC_SSM_INTF_QOS_CONFIG_CONFLICT;
						}
						break;
					}

					case QOS_INTERFACE_TRUST_COS:
					{
						if (isVcsEnabled)
						{
							status = WRC_SSM_INTF_QOS_TRUST_COS_IN_VCS_NON_CEE;
						}
						if ((QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject))) {
							status = WRC_SSM_INTF_QOS_CONFIG_CONFLICT;
						}
						break;
					}
					case QOS_INTERFACE_DEFAULT_COS:
					{
						if (isVcsEnabled)
						{
							if (pNsmGlobalQosPOConfigMessage->getDefaultCosValue() == 7)
								status = WRC_SSM_INTF_COS7_NOT_ALLOWED_IN_VCS;
						}
						break;
					}
					case QOS_INTERFACE_TRUST_DSCP:
            		{
						if (isVcsEnabled)
						{
							status = WRC_SSM_INTF_QOS_TRUST_DSCP_IN_VCS_NON_CEE;
						}
						if ((QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject))) {
							status = WRC_SSM_INTF_QOS_CONFIG_CONFLICT;
						}
						break;
					}

					case FB_POLICYMAP_BIND_INTERFACE_IN :
					case FB_POLICYMAP_BIND_INTERFACE_OUT: {
						if(!QosUtils::doesMapExist(MQC_POLICY_MAP, pNsmGlobalQosPOConfigMessage->getQosPolicyMapName(), status)) {
								status = SSM_QOS_FLOWBASED_POLICY_MAP_NOT_FOUND;
						}
						break;
					}

				}
				break;
			}
		}
       	if (NULL != poIntpResults)
		{
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(poIntpResults);
		}	
		trace (TRACE_LEVEL_DEBUG, string ("status=") + status);
		pPrismLinearSequencerContext->executeNextStep(status);
	}

	void IntfConfGlobalWorker::NsmGlobalQosPoSendToLocal (PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {

	NsmGlobalQosPOConfigMessage *pNsmGlobalQosPOConfigMessage = dynamic_cast<NsmGlobalQosPOConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
    trace (TRACE_LEVEL_INFO, string("----------Inside NsmGlobalQosPoSendToLocal-------"));
    NsmQosPOConfigSPMessage *m = new NsmQosPOConfigSPMessage();
    WaveSendToClusterContext    *send_ctx_p;

	PoIntfManagedObject *pPoIntfManagedObject = NULL;
	UI32 poId = pNsmGlobalQosPOConfigMessage->getPoId();
	ResourceId  status   = WAVE_MESSAGE_ERROR;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

	if (NULL != pResults) {
		UI32 numberOfResults = pResults->size ();
		if (0 == numberOfResults) {
			trace (TRACE_LEVEL_INFO, string("PResults is ZERO **** -------"));
			status = WAVE_MESSAGE_ERROR;
		}else if (1 == numberOfResults) {
			trace (TRACE_LEVEL_INFO, string("PResults is ONE**** -------"));
			pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
			trace (TRACE_LEVEL_INFO, string("pPoIntfManagedObject poId=") + pPoIntfManagedObject->getId());
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			status = WAVE_MESSAGE_ERROR;
		}
	}

	/* VCS QoS Sanity check */
	ResourceId  qos_status = WAVE_MESSAGE_SUCCESS;
	switch (pNsmGlobalQosPOConfigMessage->getOpCode())
	{
	  case CONFIGURE_INTERFACE_QOS:
	  {
		switch (pNsmGlobalQosPOConfigMessage->getCmdCode())
		{
		  case QOS_INTERFACE_COS_MUTATION:
		  {
			qos_status = QosUtils::vcsQosSanity(QOS_INTERFACE_COS_MUTATION, pNsmGlobalQosPOConfigMessage->getCosToCosMutationMapName(), true, 0, 0,0);
			break;
		  }
          //The below 2 defects contradict each other.
          //TR000407684 [FC: Change in behavior of "default CoS" from Hercules+ to Hydra.] => Added below case to fix thos.
          //and TR000454577 FC: PFC enable should be allowed on default-CoS to support end-to-end legacy pause.  => Commenting the below conde, to fix this.

          /*case QOS_INTERFACE_DEFAULT_COS: {
            qos_status = QosUtils::vcsQosSanity(QOS_INTERFACE_DEFAULT_COS, "", true, 0, 0, pNsmGlobalQosPOConfigMessage->getDefaultCosValue());
            break;
          }*/
          
          case QOS_INTERFACE_FLOW_CONTROL_BOTH:
          {
			//TR342243, allow Pause on PO
/*              if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
              {
		          pNsmGlobalQosPOConfigMessage->setCompletionStatus(WRC_SSM_SWITCH_IN_VCS_MODE);
		          pPrismLinearSequencerContext->executeNextStep(WRC_SSM_SWITCH_IN_VCS_MODE);
		          return;
              }
*/
			  break;
          }
		  default:
			break;
		}
        break;
	  }
	  case DELETE_INTERFACE_QOS:
	  {
        /* Nothing to be done. cmdCode and opCode will convey the info */
            trace (TRACE_LEVEL_DEBUG, string("DELETE_INTERFACE_QOS"));
            break;
	  }

	  default: {
		break;
      }
	}

    if (qos_status != WAVE_MESSAGE_SUCCESS) {
        pNsmGlobalQosPOConfigMessage->setCompletionStatus(qos_status);
        pPrismLinearSequencerContext->executeNextStep(qos_status);
        if(m) {
            delete m;
        }
        if (NULL != pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        return;
    }
	ObjectId poObjectId;

	status = getPoIntfObjectIdByPoIfName(pPoIntfManagedObject->getIfName(),poObjectId,true,pPrismLinearSequencerContext);
    trace (TRACE_LEVEL_DEBUG, string("getPoIntfObjectIdByPoIfName status=")+ status);

	if(status == WAVE_MESSAGE_SUCCESS)
		status = isPoIntfHasMember (poObjectId);

	if((WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ()) && (status == WAVE_MESSAGE_ERROR))
	{
		trace (TRACE_LEVEL_DEBUG, string("here only"));
		pNsmGlobalQosPOConfigMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        if(m){
            delete m;
        }
    	if (NULL != pResults) {
    		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    	}
		return;
	}

    m->setPoId(pNsmGlobalQosPOConfigMessage->getPoId());
    m->setOpCode(pNsmGlobalQosPOConfigMessage->getOpCode());
    m->setCmdCode(pNsmGlobalQosPOConfigMessage->getCmdCode());

    trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPOConfigMessage->getOpCode()=") + pNsmGlobalQosPOConfigMessage->getOpCode());
    trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPOConfigMessage->getCmdCode()=") + pNsmGlobalQosPOConfigMessage->getCmdCode());
    switch (pNsmGlobalQosPOConfigMessage->getOpCode())
    {
      case CONFIGURE_INTERFACE_QOS: {

          switch (pNsmGlobalQosPOConfigMessage->getCmdCode()) {

			case QOS_INTERFACE_FLOW_CONTROL_TX: {
                m->setFlowControlTx(pNsmGlobalQosPOConfigMessage->getFlowControlTx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPOConfigMessage->getFlowControlTx()=") + pNsmGlobalQosPOConfigMessage->getFlowControlTx());
                break;
            }
            case QOS_INTERFACE_FLOW_CONTROL_RX: {
                m->setFlowControlRx(pNsmGlobalQosPOConfigMessage->getFlowControlRx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPOConfigMessage->getFlowControlRx()=") + pNsmGlobalQosPOConfigMessage->getFlowControlRx());
                break;
            }
            case QOS_INTERFACE_FLOW_CONTROL_BOTH: {
                m->setFlowControlTx(pNsmGlobalQosPOConfigMessage->getFlowControlTx());
                m->setFlowControlRx(pNsmGlobalQosPOConfigMessage->getFlowControlRx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPOConfigMessage->getFlowControlTx()=") + pNsmGlobalQosPOConfigMessage->getFlowControlTx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPOConfigMessage->getFlowControlRx()=") + pNsmGlobalQosPOConfigMessage->getFlowControlRx());
                break;
            }
            case QOS_INTERFACE_DEFAULT_COS: {
				trace(TRACE_LEVEL_INFO, "-----------NsmGlobalQosPoSendToLocal:: set default cos------------");
                m->setDefaultCosValue(pNsmGlobalQosPOConfigMessage->getDefaultCosValue());
                break;
            }
            case QOS_INTERFACE_TRUST_COS: {
                m->setQosTrust(pNsmGlobalQosPOConfigMessage->getQosTrust());
                break;
            }
			case QOS_INTERFACE_TRUST_DSCP: {
                m->setQosDscpTrust(pNsmGlobalQosPOConfigMessage->getQosDscpTrust());
                break;
            }
			case QOS_INTERFACE_COS_MUTATION: {
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPOConfigMessage->getCosToCosMutationMapName()=") + pNsmGlobalQosPOConfigMessage->getCosToCosMutationMapName());
                m->setCosToCosMutationMapName(pNsmGlobalQosPOConfigMessage->getCosToCosMutationMapName());
              break;
            }
            case QOS_INTERFACE_COS_TRAFFIC_CLASS: {
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPOConfigMessage->getCosToTrafficClassMapName()=") + pNsmGlobalQosPOConfigMessage->getCosToTrafficClassMapName());
                m->setCosToTrafficClassMapName(pNsmGlobalQosPOConfigMessage->getCosToTrafficClassMapName());
              break;
            }
            case QOS_INTERFACE_CEE_MAP: {
				trace (TRACE_LEVEL_INFO, string("pNsmGlobalQosPOConfigMessage->getCeeMapName()=") + pNsmGlobalQosPOConfigMessage->getCeeMapName());
                m->setCeeMapName(pNsmGlobalQosPOConfigMessage->getCeeMapName());
                break;
			}
            case QOS_INTERFACE_PRIORITY_TAG: {
                trace (TRACE_LEVEL_INFO, string("IntfConfGlobalWorker:: set priority tag"));
                break;
            }
			case QOS_INTERFACE_DSCP_MUTATION: {
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPOConfigMessage->getDscpToDscpMutationMapName()=") + pNsmGlobalQosPOConfigMessage->getDscpToDscpMutationMapName());
                m->setDscpToDscpMutationMapName(pNsmGlobalQosPOConfigMessage->getDscpToDscpMutationMapName());
              break;
            }
            case QOS_INTERFACE_DSCP_TRAFFIC_CLASS: {
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPOConfigMessage->getDscpToTrafficClassMapName()=") + pNsmGlobalQosPOConfigMessage->getDscpToTrafficClassMapName());
                m->setDscpToTrafficClassMapName(pNsmGlobalQosPOConfigMessage->getDscpToTrafficClassMapName());
              break;
            }
            case QOS_INTERFACE_DSCP_COS: {
                trace (TRACE_LEVEL_INFO, string("pNsmGlobalQosPOConfigMessage->getDscpCosMapName()=") + pNsmGlobalQosPOConfigMessage->getDscpToCosMapName());
                m->setDscpToCosMapName(pNsmGlobalQosPOConfigMessage->getDscpToCosMapName());
                break;
            }
            case FB_POLICYMAP_BIND_INTERFACE_IN:
			case FB_POLICYMAP_BIND_INTERFACE_OUT: {
				m->setQosPolicyMapName(
						pNsmGlobalQosPOConfigMessage->getQosPolicyMapName());
				break;
			}

        }
            break;
        }
	  
		case DELETE_INTERFACE_QOS: {

		switch (pNsmGlobalQosPOConfigMessage->getCmdCode()) {

		case FB_POLICYMAP_UNBIND_INTERFACE_IN: {

			ObjectId PolicyId = pPoIntfManagedObject->getInputPolicyMapId();
			if (PolicyId != ObjectId::NullObjectId) {

				WaveManagedObjectSynchronousQueryContext polsyncQueryCtxt(
						PolicyClassMapManagedObject::getClassName());
				polsyncQueryCtxt.addAndAttribute(
						new AttributeObjectId(PolicyId, "objectId"));
				polsyncQueryCtxt.addSelectField("policyClassMapName");
				vector<WaveManagedObject *> *polResults = querySynchronously(
						&polsyncQueryCtxt);
				PolicyClassMapManagedObject *policyMapMO =
						dynamic_cast<PolicyClassMapManagedObject *>((*polResults)[0]);

				if (policyMapMO) {
					m->setQosPolicyMapName(
							policyMapMO->getPolicyClassMapName());
					pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(
							policyMapMO);
				} else {
					trace(TRACE_LEVEL_DEBUG,
							string("MO not found for policyMap"));
					delete m;
					pPrismLinearSequencerContext->executeNextStep(
							WRC_SSM_POL_ERR_POLICY_MAP_NOT_FOUND);
					if (NULL != pResults) {
						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
								pResults);
					}
                    if(NULL != polResults) {
                        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
                                polResults);
                    }
					return;
				}
			} else {
				trace(TRACE_LEVEL_DEBUG, string("No Policy Map Attached"));
				delete m;
				pPrismLinearSequencerContext->executeNextStep(
						WAVE_MESSAGE_SUCCESS);
				if (NULL != pResults) {
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
							pResults);
				}
				return;
			}
			break;
		}

		case FB_POLICYMAP_UNBIND_INTERFACE_OUT: {

			ObjectId PolicyId = pPoIntfManagedObject->getOutputPolicyMapId();
			if (PolicyId != ObjectId::NullObjectId) {

				WaveManagedObjectSynchronousQueryContext polsyncQueryCtxt(
						PolicyClassMapManagedObject::getClassName());
				polsyncQueryCtxt.addAndAttribute(
						new AttributeObjectId(PolicyId, "objectId"));
				polsyncQueryCtxt.addSelectField("policyClassMapName");
				vector<WaveManagedObject *> *polResults = querySynchronously(
						&polsyncQueryCtxt);
				PolicyClassMapManagedObject *policyMapMO =
						dynamic_cast<PolicyClassMapManagedObject *>((*polResults)[0]);

				if (policyMapMO) {
					m->setQosPolicyMapName(
							policyMapMO->getPolicyClassMapName());
					pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(
							policyMapMO);
				} else {
					trace(TRACE_LEVEL_DEBUG,
							string("MO not found for policyMap"));
					delete m;
					pPrismLinearSequencerContext->executeNextStep(
							WRC_SSM_POL_ERR_POLICY_MAP_NOT_FOUND);
					if (NULL != pResults) {
						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
								pResults);
					}
                    if(NULL != polResults) {
                        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
                                polResults);
                    }
					return;
				}
			} else {
				trace(TRACE_LEVEL_DEBUG, string("No Policy Map Attached"));
				delete m;
				pPrismLinearSequencerContext->executeNextStep(
						WAVE_MESSAGE_SUCCESS);
				if (NULL != pResults) {
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
							pResults);
				}
				return;
			}
			break;
		}

		}
		/* Nothing to be done. cmdCode and opCode will convey the info */
		trace(TRACE_LEVEL_DEBUG, string("DELETE_INTERFACE_QOS"));
		break;
	}

    }
    trace (TRACE_LEVEL_DEBUG, string("sendSynchronouslyTo Nsm Local"));
    m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
	send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback),
		pPrismLinearSequencerContext);
	prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
	send_ctx_p->setPPrismMessageForPhase1(m);

	int ret = SET_LOCATION_ID_API_ERROR;
	ret = NsmUtils::setLocationIdsForPoSubModeCmds(pNsmGlobalQosPOConfigMessage->getPoId(), send_ctx_p);

	if(ret != SET_LOCATION_ID_API_SUCCESS)
	{
		trace(TRACE_LEVEL_ERROR, "In NsmGlobalQosPoSendToLocal, call to setLocationIdsForPoSubModeCmds failed ");
		pNsmGlobalQosPOConfigMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
		delete m;
    	if (NULL != pResults) {
    		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    	}
        if (NULL != send_ctx_p) {
            delete send_ctx_p;
            send_ctx_p = NULL;
        }
		return;
	}

	trace (TRACE_LEVEL_INFO, "--------sendSynchronouslyTo Nsm Local--------");
	trace(
				TRACE_LEVEL_DEBUG,
				string("sendSynchronouslyTo Nsm Local NsmGlobalQosPoSendToLocal OPCODE<")
						+ m->getOpCode() + "> CMDCODE<" + m->getCmdCode() + "> POID<"+ m->getPoId() +
						"> MAPNAME<" + m->getQosPolicyMapName() + ">") ;

	send_ctx_p->setPartialSuccessFlag(true);
    send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
	sendToWaveCluster(send_ctx_p);
	if (NULL != pResults) {
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}
	return;
}

void IntfConfGlobalWorker::updatePoIntfQosMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
	trace (TRACE_LEVEL_DEBUG, string("Inside updatePoIntfQosMOStep-------"));
	NsmGlobalQosPOConfigMessage *pNsmGlobalQosPOConfigMessage = dynamic_cast<NsmGlobalQosPOConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
	trace (TRACE_LEVEL_DEBUG, string("DB pNsmGlobalQosPOConfigMessage->getOpCode()=") + pNsmGlobalQosPOConfigMessage->getOpCode());
	trace (TRACE_LEVEL_DEBUG, string("DB pNsmGlobalQosPOConfigMessage->getCmdCode()=") + pNsmGlobalQosPOConfigMessage->getCmdCode());
	trace (TRACE_LEVEL_DEBUG, string("DB pNsmGlobalQosPOConfigMessage->getPoId()=") + pNsmGlobalQosPOConfigMessage->getPoId());
	PoIntfManagedObject *pPoIntfManagedObject = NULL;
	UI32 poId = pNsmGlobalQosPOConfigMessage->getPoId();
	ResourceId  status   = WAVE_MESSAGE_SUCCESS;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

	if (NULL != pResults) {
		UI32 numberOfResults = pResults->size ();
		if (0 == numberOfResults) {
			trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
			//status = WAVE_MESSAGE_ERROR;
		}else if (1 == numberOfResults) {
			trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
			pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
			trace (TRACE_LEVEL_DEBUG, string("pPoIntfManagedObject poId=") + pPoIntfManagedObject->getId());
			//status = WAVE_MESSAGE_SUCCESS;
		} else {
			//status = WAVE_MESSAGE_ERROR;
		}
	}

	prismAssert (NULL != pPoIntfManagedObject, __FILE__, __LINE__);
	updateWaveManagedObject (pPoIntfManagedObject);
	switch(pNsmGlobalQosPOConfigMessage->getOpCode())
	{
	case CONFIGURE_INTERFACE_QOS:
		trace (TRACE_LEVEL_DEBUG, string("DB UPDATE CONFIGURE_INTERFACE_QOS-------"));
		switch (pNsmGlobalQosPOConfigMessage->getCmdCode()) {
		case QOS_INTERFACE_FLOW_CONTROL_TX:
			trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_FLOW_CONTROL_TX-------"));
			pPoIntfManagedObject->setFlowControlTx(pNsmGlobalQosPOConfigMessage->getFlowControlTx());
			break;
		case QOS_INTERFACE_FLOW_CONTROL_RX:
			trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_FLOW_CONTROL_RX-------"));
			pPoIntfManagedObject->setFlowControlRx(pNsmGlobalQosPOConfigMessage->getFlowControlRx());
			break;
		case QOS_INTERFACE_FLOW_CONTROL_BOTH:
			trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_FLOW_CONTROL_BOTH-------"));
			pPoIntfManagedObject->setFlowControlTx(pNsmGlobalQosPOConfigMessage->getFlowControlTx());
			pPoIntfManagedObject->setFlowControlRx(pNsmGlobalQosPOConfigMessage->getFlowControlRx());
			break;
		case QOS_INTERFACE_DEFAULT_COS:
			trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_DEFAULT_COS-------=") + pNsmGlobalQosPOConfigMessage->getDefaultCosValue());
			pPoIntfManagedObject->setDefaultCos(pNsmGlobalQosPOConfigMessage->getDefaultCosValue());
			break;
		case QOS_INTERFACE_TRUST_COS:
			trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_TRUST_COS-------=")+ pNsmGlobalQosPOConfigMessage->getQosTrust());
			pPoIntfManagedObject->setQosTrust(true);
			break;
		case QOS_INTERFACE_TRUST_DSCP:
            trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_TRUST_DSCP-------=")+ pNsmGlobalQosPOConfigMessage->getQosDscpTrust());
            pPoIntfManagedObject->setQosDscpTrust(true);
            break;
		case QOS_INTERFACE_COS_MUTATION:
			{
				trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_COS_MUTATION-------="));
				string cosName = pNsmGlobalQosPOConfigMessage->getCosToCosMutationMapName();
				ObjectId cosMapObjectId;
				getCosMapObjectIdByCosMapName(cosName, cosMapObjectId,pPrismLinearSequencerContext);
				pPoIntfManagedObject->setQosCosMap(cosMapObjectId);
				break;
			}
		case QOS_INTERFACE_COS_TRAFFIC_CLASS:
			{
				trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_COS_TRAFFIC_CLASS-------="));
				string cosName = pNsmGlobalQosPOConfigMessage->getCosToTrafficClassMapName();
				ObjectId cosTcObjectId;
				getTcMapObjectIdByTcMapName(cosName, cosTcObjectId, pPrismLinearSequencerContext);
				pPoIntfManagedObject->setQosTrafficClassMap(cosTcObjectId);
				break;
			}
		case QOS_INTERFACE_DSCP_MUTATION:
            {
                trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_DSCP_MUTATION-------="));
                string dscpName = pNsmGlobalQosPOConfigMessage->getDscpToDscpMutationMapName();
                ObjectId dscpMapObjectId;
                getDscpMapObjectIdByDscpMapName(dscpName, dscpMapObjectId,pPrismLinearSequencerContext);
                pPoIntfManagedObject->setQosDscpMap(dscpMapObjectId);
                break;
            }
        case QOS_INTERFACE_DSCP_TRAFFIC_CLASS:
            {
                trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_DSCP_TRAFFIC_CLASS-------="));
                string dscpName = pNsmGlobalQosPOConfigMessage->getDscpToTrafficClassMapName();
                ObjectId dscpTcObjectId;
                getDscpTcMapObjectIdByDscpTcMapName(dscpName, dscpTcObjectId, pPrismLinearSequencerContext);
                pPoIntfManagedObject->setQosDscpTrafficClassMap(dscpTcObjectId);
                break;
            }
		case QOS_INTERFACE_DSCP_COS:
            {
                trace (TRACE_LEVEL_DEBUG, string("DB UPDATE QOS_INTERFACE_DSCP_COS-------="));
                string dscpName = pNsmGlobalQosPOConfigMessage->getDscpToCosMapName();
                ObjectId dscpCosObjectId;
                getDscpCosMapObjectIdByDscpCosMapName(dscpName, dscpCosObjectId, pPrismLinearSequencerContext);
                pPoIntfManagedObject->setQosDscpCosMap(dscpCosObjectId);
                break;
            }
		case QOS_INTERFACE_CEE_MAP:
			{
            	trace (TRACE_LEVEL_INFO, string("DB UPDATE QOS_INTERFACE_CEE_MAP-------="));
                string ceeName = pNsmGlobalQosPOConfigMessage->getCeeMapName();
                ObjectId ceeObjectId;
                getCeeMapObjectIdByCeeMapName(ceeName, ceeObjectId, pPrismLinearSequencerContext);
                pPoIntfManagedObject->setCeeMap(ceeObjectId);
				// remove the non-CEE config
				pPoIntfManagedObject->setQosCosMap(ObjectId::NullObjectId);
				pPoIntfManagedObject->setQosTrafficClassMap(ObjectId::NullObjectId);
				pPoIntfManagedObject->setQosTrust(QOS_DEFAULT_QOS_TRUST);
				pPoIntfManagedObject->setFlowControlTx(QOS_DEFAULT_FLOWCONTROL);
				pPoIntfManagedObject->setFlowControlRx(QOS_DEFAULT_FLOWCONTROL);
				pPoIntfManagedObject->deletePfcAll();
				break;
				}
			case QOS_INTERFACE_PRIORITY_TAG:
			{
				trace (TRACE_LEVEL_INFO, string("DB UPDATE QOS_INTERFACE_PRIORITY_TAG-------="));
				pPoIntfManagedObject->setPriorityTag(true);
				break;
			}

			case FB_POLICYMAP_BIND_INTERFACE_IN: {

				ObjectId PolicyId = ObjectId::NullObjectId;
				trace(TRACE_LEVEL_DEBUG,
						string("DB UPDATE CONFIGURE_INTERFACE_QOS-------"));
				status = QosUtils::getQosMapObjectIdByMapName(
						pNsmGlobalQosPOConfigMessage->getQosPolicyMapName(),
						PolicyId, MQC_POLICY_MAP, pPrismLinearSequencerContext);

				if (status != WAVE_MESSAGE_SUCCESS) {
					trace(TRACE_LEVEL_ERROR,
							string("getQosMapObjectIdByMapName tcMap failed"));
					status = WAVE_MESSAGE_ERROR;
					break;
				}

				trace(TRACE_LEVEL_DEBUG,
						string("DB UPDATE FB_POLICYMAP_BIND_INTERFACE_IN-------"));
				if (PolicyId != ObjectId::NullObjectId) {
					pPoIntfManagedObject->setInputPolicyMapId(PolicyId);
					pPoIntfManagedObject->addAttributeToBeUpdated(
							"inputPolicyMapId");
				} else {
					status = WRC_SSM_POL_ERR_POLICY_MAP_NOT_FOUND;
				}

				break;
			}

			case FB_POLICYMAP_BIND_INTERFACE_OUT: {

				ObjectId PolicyId = ObjectId::NullObjectId;
				trace(TRACE_LEVEL_DEBUG,
						string("DB UPDATE CONFIGURE_INTERFACE_QOS-------"));
				status = QosUtils::getQosMapObjectIdByMapName(
						pNsmGlobalQosPOConfigMessage->getQosPolicyMapName(),
						PolicyId, MQC_POLICY_MAP, pPrismLinearSequencerContext);

				if (status != WAVE_MESSAGE_SUCCESS) {
					trace(TRACE_LEVEL_ERROR,
							string("getQosMapObjectIdByMapName tcMap failed"));
					status = WAVE_MESSAGE_ERROR;
					break;
				}

				trace(TRACE_LEVEL_DEBUG,
						string("DB UPDATE FB_POLICYMAP_BIND_INTERFACE_OUT-------"));
				if (PolicyId != ObjectId::NullObjectId) {
					pPoIntfManagedObject->setOutputPolicyMapId(PolicyId);
					pPoIntfManagedObject->addAttributeToBeUpdated(
							"outputPolicyMapId");
				} else {
					status = WRC_SSM_POL_ERR_POLICY_MAP_NOT_FOUND;
				}
				break;
			}
		}
		break;
	case DELETE_INTERFACE_QOS:
		{
			trace (TRACE_LEVEL_DEBUG, string("DB UPDATE DELETE_INTERFACE_QOS-------"));
			switch (pNsmGlobalQosPOConfigMessage->getCmdCode()) {
			case QOS_INTERFACE_FLOW_CONTROL_BOTH:
				pPoIntfManagedObject->setFlowControlTx(QOS_DEFAULT_FLOWCONTROL);
				pPoIntfManagedObject->setFlowControlRx(QOS_DEFAULT_FLOWCONTROL);
				break;
			case QOS_INTERFACE_DEFAULT_COS:
				pPoIntfManagedObject->setDefaultCos(0);
				break;
			case QOS_INTERFACE_TRUST_COS:
				pPoIntfManagedObject->setQosTrust(QOS_DEFAULT_QOS_TRUST);
				break;
			case QOS_INTERFACE_TRUST_DSCP:
                pPoIntfManagedObject->setQosDscpTrust(QOS_DEFAULT_QOS_TRUST);
                break;
			case QOS_INTERFACE_COS_MUTATION:
				trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_COS_MUTATION-------="));
				pPoIntfManagedObject->setQosCosMap(ObjectId::NullObjectId);
				break;
			case QOS_INTERFACE_COS_TRAFFIC_CLASS:
				trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_COS_TRAFFIC_CLASS-------="));
				pPoIntfManagedObject->setQosTrafficClassMap(ObjectId::NullObjectId);
				break;
			case QOS_INTERFACE_DSCP_MUTATION:
                trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_DSCP_MUTATION-------="));
                pPoIntfManagedObject->setQosDscpMap(ObjectId::NullObjectId);
                break;
            case QOS_INTERFACE_DSCP_TRAFFIC_CLASS:
                trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_DSCP_TRAFFIC_CLASS-------="));
                pPoIntfManagedObject->setQosDscpTrafficClassMap(ObjectId::NullObjectId);
                break;
			case QOS_INTERFACE_DSCP_COS:
                trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_DSCP_COS-------="));
                pPoIntfManagedObject->setQosDscpCosMap(ObjectId::NullObjectId);
                break;
					case QOS_INTERFACE_CEE_MAP:
						trace (TRACE_LEVEL_INFO, string("DB DELETE QOS_INTERFACE_CEE_MAP-------="));
						pPoIntfManagedObject->setCeeMap(ObjectId::NullObjectId);
				break;
			case QOS_INTERFACE_PRIORITY_TAG:
				trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_PRIORITY_TAG-------="));
				pPoIntfManagedObject->setPriorityTag(false);
				break;

			case FB_POLICYMAP_UNBIND_INTERFACE_IN: {
				trace(TRACE_LEVEL_DEBUG,
						string("DB UPDATE DELETE_INTERFACE_QOS-------"));
					trace(TRACE_LEVEL_DEBUG,
							string("DB UPDATE FB_POLICYMAP_UNBIND_INTERFACE_IN-------"));
					pPoIntfManagedObject->setInputPolicyMapId(ObjectId::NullObjectId);
					pPoIntfManagedObject->addAttributeToBeUpdated("inputPolicyMapId");
				break;
			}

			case FB_POLICYMAP_UNBIND_INTERFACE_OUT: {
				trace(TRACE_LEVEL_DEBUG,
						string("DB UPDATE DELETE_INTERFACE_QOS-------"));
				trace(TRACE_LEVEL_DEBUG,
						string("DB UPDATE FB_POLICYMAP_UNBIND_INTERFACE_OUT-------"));
				pPoIntfManagedObject->setOutputPolicyMapId(ObjectId::NullObjectId);
				pPoIntfManagedObject->addAttributeToBeUpdated("outputPolicyMapId");
				break;
			}
			}
			break;
		}

	}
    if (pResults) { 
		if(pPrismLinearSequencerContext) {
        	pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
		delete pResults;
        } else {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
	}

	pPrismLinearSequencerContext->executeNextStep(status);
	return;
}



    void
	  IntfConfGlobalWorker::NsmGlobalQosPOConfigMessageHandler(NsmGlobalQosPOConfigMessage *pNsmGlobalQosPOConfigMessage)
	{
			PrismLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::NsmGlobalQosPoValidationStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::NsmGlobalQosPoSendToLocal),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updatePoIntfQosMOStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
			};
			PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNsmGlobalQosPOConfigMessage,
                							this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
            pPrismLinearSequencerContext->holdAll ();
			pPrismLinearSequencerContext->start ();
	}

	void IntfConfGlobalWorker::updateQosPfcPoIntfValidationStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		trace (TRACE_LEVEL_DEBUG, string("IntfConfGlobalWorker::updateQosPfcPoIntfValidationStep"));
		PoIntfManagedObject *pPoIntfManagedObject = NULL;
    	NsmGlobalQosPfcConfigMessage *pNsmGlobalQosPfcConfigMessage =
	  		dynamic_cast<NsmGlobalQosPfcConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		if ((DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED)) {
			status = WRC_SSM_INTF_QOS_FLOW_CONTROL_BLOCKED_IN_VCS_MODE;
			pPrismLinearSequencerContext->executeNextStep(status);
			return;
		}

		UI32 poId = pNsmGlobalQosPfcConfigMessage->getPoId();
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		if (pResults != NULL)
		{
			if (pResults->size() == 1)
			{
				trace (TRACE_LEVEL_DEBUG, string("PResults is ONE"));
				pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
			}
			else {
				pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
				trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO"));
				return; 
			}
		}
		else
		{
			pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
			trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
			return; 
		}
		
		/* If PO is in CEE mode, throw error */
		if (pPoIntfManagedObject->getCeeMap() != ObjectId::NullObjectId)
		{
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			pPrismLinearSequencerContext->executeNextStep(WRC_SSM_INTF_QOS_NOT_IN_NON_CEE);
			return;
		}
		
		/* If 802.3x pause enabled on interface, throw error */
		if ((pPoIntfManagedObject->getFlowControlTx() == 1) || (pPoIntfManagedObject->getFlowControlRx() == 1))
		{
			status = WRC_SSM_INTF_PAUSE_ALREADY_APPLIED; 
			if (pResults)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			pPrismLinearSequencerContext->executeNextStep(status);
			return;
		}

		/* If PO has 1G member, throw error */
		ObjectId poObjectId = pPoIntfManagedObject->getObjectId();
		UI32 oneGigCount = 0;
		WaveManagedObjectSynchronousQueryContext sync1GCtxt(OneGigLocalManagedObject::getClassName());
		sync1GCtxt.addAndAttribute (new AttributeObjectIdAssociation(poObjectId, "poId", OneGigLocalManagedObject::getClassName()));
		status = querySynchronouslyForCount (&sync1GCtxt, oneGigCount);
		if (status != WAVE_MESSAGE_SUCCESS)
		{
			trace (TRACE_LEVEL_INFO, string ("querySynchronouslyForCount on 1G failed"));
			if (pResults)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
			return;
		}
		else
		{
			if (oneGigCount != 0)
			{
			    trace (TRACE_LEVEL_INFO, string ("querySynchronouslyForCount on 1G= ") + oneGigCount);
				if (pResults)
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				pPrismLinearSequencerContext->executeNextStep(WRC_SSM_INTF_PFC_NOT_SUPPORTED_ON_PO_WITH_1G_CU);
				return;
			}
		}

		/* Check if PFC is enabled for cos with default traffic class mapping */
		if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ())
		{
			SI32 pfcFlowControlCos = pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos(); 
			ObjectId mapObjectId = pPoIntfManagedObject->getQosCosMap();
			SI32 cosValue[8];
			CosToCosMutationMapManagedObject *cosMapMo = NULL;
			if (mapObjectId  == ObjectId::NullObjectId) 
			{
				status = WRC_SSM_INTF_PFC_ON_COS_WITH_NON_DEFAULT_TC;
				trace (TRACE_LEVEL_DEBUG, string("Map not set"));
			}
			else
			{
				WaveManagedObjectSynchronousQueryContext mapQuery (CosToCosMutationMapManagedObject::getClassName());
				mapQuery.addAndAttribute (new AttributeObjectId(&mapObjectId,"objectid"));
				vector<WaveManagedObject *> *pMapResults = querySynchronously (&mapQuery);
				if (pMapResults)
				{
					if (1 == pMapResults->size())
					{
						cosMapMo = dynamic_cast<CosToCosMutationMapManagedObject*>((*pMapResults)[0]);
						trace (TRACE_LEVEL_DEBUG, string("Got Cos Map"));
					}
					else
					{
						pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
						return;
					}
				}
				else
				{
					pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
					return;
				}

				cosValue[0] = cosMapMo->getCos0MutatedCosValue();
				cosValue[1] = cosMapMo->getCos1MutatedCosValue();
				cosValue[2] = cosMapMo->getCos2MutatedCosValue();
				cosValue[3] = cosMapMo->getCos3MutatedCosValue();
				cosValue[4] = cosMapMo->getCos4MutatedCosValue();
				cosValue[5] = cosMapMo->getCos5MutatedCosValue();
				cosValue[6] = cosMapMo->getCos6MutatedCosValue();
				cosValue[7] = cosMapMo->getCos7MutatedCosValue();

				if (pfcFlowControlCos != cosValue[pfcFlowControlCos])
				{
					status = WRC_SSM_INTF_PFC_ON_COS_WITH_NON_DEFAULT_TC;
					trace (TRACE_LEVEL_DEBUG, string("Non default traffic class-") + pfcFlowControlCos + cosValue[pfcFlowControlCos]);
				}
				if (pMapResults)
				{
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pMapResults);
				}
			}
		}
		if (pResults)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		pPrismLinearSequencerContext->executeNextStep(status);
		return;
	}

	void IntfConfGlobalWorker::updateQosPfcPoIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
    NsmGlobalQosPfcConfigMessage *pNsmGlobalQosPfcConfigMessage =
	  dynamic_cast<NsmGlobalQosPfcConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
    trace (TRACE_LEVEL_DEBUG, string("Inside updateQosPfcPoIntfStep-------"));
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    NsmQosPfcConfigSPMessage *m = NULL;
	WaveSendToClusterContext    *send_ctx_p;

	PoIntfManagedObject *pPoIntfManagedObject = NULL;
	UI32 poId = pNsmGlobalQosPfcConfigMessage->getPoId();
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

	if (pResults != NULL)
	{
		if (pResults->size() == 1)
		{
			trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
			pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
		}
		else
		{
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			pNsmGlobalQosPfcConfigMessage->setCompletionStatus(WAVE_MESSAGE_ERROR);
			pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
			trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
			return; 
		}
	}
	else
	{
			pNsmGlobalQosPfcConfigMessage->setCompletionStatus(WAVE_MESSAGE_ERROR);
			pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
			trace (TRACE_LEVEL_DEBUG, string("PResults is NULL **** -------"));
			return; 
	}


	ObjectId poObjectId;

	status = getPoIntfObjectIdByPoIfName(pPoIntfManagedObject->getIfName(),poObjectId,true,pPrismLinearSequencerContext);
    trace (TRACE_LEVEL_DEBUG, string("getPoIntfObjectIdByPoIfName status=")+ status);

	if(status == WAVE_MESSAGE_SUCCESS)
		status = isPoIntfHasMember (poObjectId);

    if (pResults != NULL)
	    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	if((WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ()) && (status == WAVE_MESSAGE_ERROR))
	{
		trace (TRACE_LEVEL_DEBUG, string("Returning success for empty PO in VCS Mode"));
		pNsmGlobalQosPfcConfigMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
		return;
	}

	m = new NsmQosPfcConfigSPMessage();
    m->setPoId(pNsmGlobalQosPfcConfigMessage->getPoId());
    m->setOpCode(pNsmGlobalQosPfcConfigMessage->getOpCode());
    m->setCmdCode(pNsmGlobalQosPfcConfigMessage->getCmdCode());

    trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPfcConfigMessage->getOpCode()=") + pNsmGlobalQosPfcConfigMessage->getOpCode());
    trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPfcConfigMessage->getCmdCode()=") + pNsmGlobalQosPfcConfigMessage->getCmdCode());
	switch (pNsmGlobalQosPfcConfigMessage->getOpCode())
    {
		case CONFIGURE_INTERFACE_QOS:
        {
          switch (pNsmGlobalQosPfcConfigMessage->getCmdCode())
          {
            case QOS_INTERFACE_PFC_FLOW_CONTROL_TX:
                m->setPfcFlowControlCos(pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos());
				m->setPfcFlowControlTx(pNsmGlobalQosPfcConfigMessage->getPfcFlowControlTx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos()=") +
					pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos());
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPfcConfigMessage->getPfcFlowControlTx()=") +
					pNsmGlobalQosPfcConfigMessage->getPfcFlowControlTx());
                break;
            case QOS_INTERFACE_PFC_FLOW_CONTROL_RX:
                m->setPfcFlowControlCos(pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos());
				m->setPfcFlowControlRx(pNsmGlobalQosPfcConfigMessage->getPfcFlowControlRx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos()=") +
					pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos());
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPfcConfigMessage->getPfcFlowControlRx()=") +
					pNsmGlobalQosPfcConfigMessage->getPfcFlowControlRx());
                break;

            case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
            {
                m->setPfcFlowControlCos(pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos());
				m->setPfcFlowControlTx(pNsmGlobalQosPfcConfigMessage->getPfcFlowControlTx());
				m->setPfcFlowControlRx(pNsmGlobalQosPfcConfigMessage->getPfcFlowControlRx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos()=") +
					pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos());
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPfcConfigMessage->getPfcFlowControlTx()=") +
					pNsmGlobalQosPfcConfigMessage->getPfcFlowControlTx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmGlobalQosPfcConfigMessage->getPfcFlowControlRx()=") +
					pNsmGlobalQosPfcConfigMessage->getPfcFlowControlRx());
                break;
            }

        }
            break;
        }
		case DELETE_INTERFACE_QOS:
        {
            trace (TRACE_LEVEL_DEBUG, string("DELETE_INTERFACE_QOS"));
			switch(pNsmGlobalQosPfcConfigMessage->getCmdCode()) {
                case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
                	m->setPfcFlowControlCos(pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos());
                break;
            }
       break;
       }

	}
	trace (TRACE_LEVEL_DEBUG, string("sendSynchronouslyTo Nsm Local"));
    m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
	send_ctx_p = new WaveSendToClusterContext(this,
		reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);

    int ret = SET_LOCATION_ID_API_ERROR;
    ret = NsmUtils::setLocationIdsForPoSubModeCmds(pNsmGlobalQosPfcConfigMessage->getPoId(), send_ctx_p);

    if(ret != SET_LOCATION_ID_API_SUCCESS)
    {
        trace(TRACE_LEVEL_ERROR, "In updateQosPfcPoIntfStep, call to setLocationIdsForPoSubModeCmds failed ");
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        pNsmGlobalQosPfcConfigMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        if(NULL != m) {
            delete m;
            m = NULL;
        }
        if (NULL != send_ctx_p) {
            delete send_ctx_p;
            send_ctx_p = NULL;
        }
        return;
    }

    trace (TRACE_LEVEL_INFO, "sendSynchronouslyTo Nsm Local");
	send_ctx_p->setPartialSuccessFlag(true);
    send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return;
	}


	void IntfConfGlobalWorker::updateQosPfcPoIntfMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {
	trace (TRACE_LEVEL_DEBUG, string("Inside updateQosPfcIntfMOStep-------"));
    NsmGlobalQosPfcConfigMessage *pNsmGlobalQosPfcConfigMessage = dynamic_cast<NsmGlobalQosPfcConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
    trace (TRACE_LEVEL_DEBUG, string("DB pNsmGlobalQosPfcConfigMessage->getOpCode()=") + pNsmGlobalQosPfcConfigMessage->getOpCode());
    trace (TRACE_LEVEL_DEBUG, string("DB pNsmGlobalQosPfcConfigMessage->getCmdCode()=") + pNsmGlobalQosPfcConfigMessage->getCmdCode());
    PoIntfManagedObject *pPoIntfManagedObject= NULL;
	UI32 poId = pNsmGlobalQosPfcConfigMessage->getPoId();
    //ResourceId  status   = WAVE_MESSAGE_ERROR;
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);


	if (NULL != pResults) {
    	UI32 numberOfResults = pResults->size ();
       	if (0 == numberOfResults) {
       		trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
           	//status = WAVE_MESSAGE_ERROR;
		 } else if (1 == numberOfResults) {
          	trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
           	pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
           	//status = WAVE_MESSAGE_SUCCESS;
          } else {
                //status = WAVE_MESSAGE_ERROR;
           }
      }

        prismAssert (NULL != pPoIntfManagedObject, __FILE__, __LINE__);
        updateWaveManagedObject (pPoIntfManagedObject);
		switch(pNsmGlobalQosPfcConfigMessage->getOpCode())
        {
            case CONFIGURE_INTERFACE_QOS:
				{
                    trace (TRACE_LEVEL_DEBUG, string("DB UPDATE CONFIGURE_INTERFACE_QOS-------"));
                	switch (pNsmGlobalQosPfcConfigMessage->getCmdCode()) {
						case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
						{
                    	trace (TRACE_LEVEL_DEBUG, string("DB QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH pfc =") + pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos());
						UI32 tx = 0, rx = 0;
                        SI32 ret = pPoIntfManagedObject->getPfcFlowControlRow(pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos(), tx, rx);
                        if (!ret) {
                    		trace (TRACE_LEVEL_DEBUG, string("DB got the pfc row"));
							pPoIntfManagedObject->deletePfc(pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos());
            			} 
						trace (TRACE_LEVEL_DEBUG, string("DB new pfc row"));
							PfcFlowControlLocalManagedObject *pfcMO = 
							new PfcFlowControlLocalManagedObject( dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()),
								pNsmGlobalQosPfcConfigMessage->getPfcFlowControlTx(),
								pNsmGlobalQosPfcConfigMessage->getPfcFlowControlRx(), pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos());
						pPoIntfManagedObject->addPfc(pfcMO);
						}
							break;
						}
					break;
				}
			case DELETE_INTERFACE_QOS:
				{
                    trace (TRACE_LEVEL_DEBUG, string("DB UPDATE DELETE_INTERFACE_QOS-------"));
                	switch (pNsmGlobalQosPfcConfigMessage->getCmdCode()) {
						case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
						{
                    	trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH pfc =") + pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos());
						pPoIntfManagedObject->deletePfc(pNsmGlobalQosPfcConfigMessage->getPfcFlowControlCos());
						break;
						}
					}
				break;
				}
		}

		if (NULL != pResults) 
		{
			if(pPrismLinearSequencerContext) {
				pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			delete pResults;
            } else {
		        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            } 
		}

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return ;
  }


    void  IntfConfGlobalWorker::NsmGlobalQosPfcConfigMessageHandler( NsmGlobalQosPfcConfigMessage *pNsmGlobalQosPfcConfigMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
		  	reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updateQosPfcPoIntfValidationStep),
		  	reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updateQosPfcPoIntfStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
		  	reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updateQosPfcPoIntfMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNsmGlobalQosPfcConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
	}



//During deletion of a qos map:
    // Need to cleanup the references of the qos map, from all the physical interface MO, on all the disconnected nodes in a management cluster.

    void IntfConfGlobalWorker::NsmGlobalQosMapCleanUp(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {

	NsmGlobalQosMapCleanUpMessage *pNsmGlobalQosMapCleanUpMessage =
			dynamic_cast<NsmGlobalQosMapCleanUpMessage*>(pPrismLinearSequencerContext->getPPrismMessage());

	UI32 mapType = pNsmGlobalQosMapCleanUpMessage->getMapType();
	string mapName = pNsmGlobalQosMapCleanUpMessage->getMapName();
	PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
	ObjectId qosMapObjectId;
	ResourceId status = WAVE_MESSAGE_ERROR;
	trace(TRACE_LEVEL_DEBUG,
			string("IntfConfGlobalWorker::mapName=") + mapName);

	WaveManagedObjectSynchronousQueryContext syncInactivePhyQueryCtxt(
			PhyIntfLocalManagedObject::getClassName());
	syncInactivePhyQueryCtxt.setFilterByDisconnectedNodes();

	switch (mapType) {

	case QOS_INTERFACE_COS_MUTATION: {

		/* Check if map is activated in disconnected Physical interface. If yes, deactivate map association */
		status = QosUtils::getQosMapObjectIdByMapName(mapName, qosMapObjectId,
				COS_MUTATION_MAP, pPrismLinearSequencerContext);
		if (status != WAVE_MESSAGE_SUCCESS) {
			break;
		}

		syncInactivePhyQueryCtxt.addAndAttribute(
				new AttributeObjectIdAssociation(qosMapObjectId, "qosCosMap",
						PhyIntfLocalManagedObject::getClassName()));
		syncInactivePhyQueryCtxt.addSelectField("qosCosMap");

		vector<WaveManagedObject *> *pInactivePhyResults = querySynchronously(
				&syncInactivePhyQueryCtxt);
		trace(TRACE_LEVEL_DEBUG, string("IntfConfGlobalWorker::COS MAP"));

		if (NULL != pInactivePhyResults) {
			UI32 numberOfResults = pInactivePhyResults->size();
			trace(
					TRACE_LEVEL_DEBUG,
					string("NsmGlobalQosMapCleanUp::results=")
							+ numberOfResults);

			startTransaction();
			for (UI32 i = 0; i < numberOfResults; i++) {
				pPhyIntfLocalManagedObject =
						dynamic_cast<PhyIntfLocalManagedObject*>((*pInactivePhyResults)[i]);
				if (pPhyIntfLocalManagedObject) {
					trace(
							TRACE_LEVEL_DEBUG,
							string("Remove") + mapName + string("from ifName=")
									+ pPhyIntfLocalManagedObject->getId());
					pPhyIntfLocalManagedObject->addAttributeToBeUpdated(
							"qosCosMap");
					pPhyIntfLocalManagedObject->setQosCosMap(
							ObjectId::NullObjectId);
					updateWaveManagedObject(pPhyIntfLocalManagedObject);
				}
			}
			commitTransaction();
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
					pInactivePhyResults);
		}
		break;
	}

	case QOS_INTERFACE_COS_TRAFFIC_CLASS: {
		/* Check if map is activated in disconnected Physical interface. If yes, deactivate map association */
		status = QosUtils::getQosMapObjectIdByMapName(mapName, qosMapObjectId,
				COS_TRAFFIC_CLASS_MAP, pPrismLinearSequencerContext);

		if (status != WAVE_MESSAGE_SUCCESS) {
			break;
		}

		syncInactivePhyQueryCtxt.addAndAttribute(
				new AttributeObjectIdAssociation(qosMapObjectId,
						"qosTrafficClassMap",
						PhyIntfLocalManagedObject::getClassName()));
		syncInactivePhyQueryCtxt.addSelectField("qosTrafficClassMap");
		vector<WaveManagedObject *> *pInactivePhyResults = querySynchronously(
				&syncInactivePhyQueryCtxt);
		trace(TRACE_LEVEL_DEBUG,
				string("IntfConfGlobalWorker::COS TRAFFIC CLASS MAP"));

		if (NULL != pInactivePhyResults) {
			UI32 numberOfResults = pInactivePhyResults->size();
			trace(
					TRACE_LEVEL_DEBUG,
					string("NsmGlobalQosMapCleanUp::results=")
							+ numberOfResults);

			startTransaction();
			for (UI32 i = 0; i < numberOfResults; i++) {
				pPhyIntfLocalManagedObject =
						dynamic_cast<PhyIntfLocalManagedObject*>((*pInactivePhyResults)[i]);
				if (pPhyIntfLocalManagedObject) {
					trace(
							TRACE_LEVEL_DEBUG,
							string("Remove") + mapName + string("from ifName=")
									+ pPhyIntfLocalManagedObject->getId());
					pPhyIntfLocalManagedObject->addAttributeToBeUpdated(
							"qosTrafficClassMap");
					pPhyIntfLocalManagedObject->setQosTrafficClassMap(
							ObjectId::NullObjectId);
					updateWaveManagedObject(pPhyIntfLocalManagedObject);
				}
			}
			commitTransaction();
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
					pInactivePhyResults);
		}
		break;
	}

	case QOS_INTERFACE_DSCP_COS: {
		/* Check if map is activated in disconnected Physical interface. If yes, deactivate map association */
		status = QosUtils::getQosMapObjectIdByMapName(mapName, qosMapObjectId,
				DSCP_COS_MAP, pPrismLinearSequencerContext);

		if (status != WAVE_MESSAGE_SUCCESS) {
			break;
		}

		syncInactivePhyQueryCtxt.addAndAttribute(
				new AttributeObjectIdAssociation(qosMapObjectId,
						"qosDscpCosMap",
						PhyIntfLocalManagedObject::getClassName()));
		syncInactivePhyQueryCtxt.addSelectField("qosDscpCosMap");
		vector<WaveManagedObject *> *pInactivePhyResults = querySynchronously(
				&syncInactivePhyQueryCtxt);
		trace(TRACE_LEVEL_DEBUG,
				string("IntfConfGlobalWorker::DSCP TO COS MAP"));

		if (NULL != pInactivePhyResults) {
			UI32 numberOfResults = pInactivePhyResults->size();
			trace(
					TRACE_LEVEL_DEBUG,
					string("NsmGlobalQosMapCleanUp::results=")
							+ numberOfResults);

			startTransaction();
			for (UI32 i = 0; i < numberOfResults; i++) {
				pPhyIntfLocalManagedObject =
						dynamic_cast<PhyIntfLocalManagedObject*>((*pInactivePhyResults)[i]);
				if (pPhyIntfLocalManagedObject) {
					trace(
							TRACE_LEVEL_DEBUG,
							string("Remove") + mapName + string("from ifName=")
									+ pPhyIntfLocalManagedObject->getId());
					pPhyIntfLocalManagedObject->addAttributeToBeUpdated(
							"qosDscpCosMap");
					pPhyIntfLocalManagedObject->setQosDscpCosMap(
							ObjectId::NullObjectId);
					updateWaveManagedObject(pPhyIntfLocalManagedObject);
				}
			}
			commitTransaction();
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
					pInactivePhyResults);
		}
		break;
	}

	case QOS_INTERFACE_DSCP_MUTATION: {
		/* Check if map is activated in disconnected Physical interface. If yes, deactivate map association */
		status = QosUtils::getQosMapObjectIdByMapName(mapName, qosMapObjectId,
				DSCP_MUTATION_MAP, pPrismLinearSequencerContext);

		if (status != WAVE_MESSAGE_SUCCESS) {
			break;
		}

		syncInactivePhyQueryCtxt.addAndAttribute(
				new AttributeObjectIdAssociation(qosMapObjectId, "qosDscpMap",
						PhyIntfLocalManagedObject::getClassName()));
		syncInactivePhyQueryCtxt.addSelectField("qosDscpMap");
		vector<WaveManagedObject *> *pInactivePhyResults = querySynchronously(
				&syncInactivePhyQueryCtxt);
		trace(TRACE_LEVEL_DEBUG,
				string("IntfConfGlobalWorker::DSCP MUTATION MAP"));

		if (NULL != pInactivePhyResults) {
			UI32 numberOfResults = pInactivePhyResults->size();
			trace(
					TRACE_LEVEL_DEBUG,
					string("NsmGlobalQosMapCleanUp::results=")
							+ numberOfResults);

			startTransaction();
			for (UI32 i = 0; i < numberOfResults; i++) {
				pPhyIntfLocalManagedObject =
						dynamic_cast<PhyIntfLocalManagedObject*>((*pInactivePhyResults)[i]);
				if (pPhyIntfLocalManagedObject) {
					trace(
							TRACE_LEVEL_DEBUG,
							string("Remove") + mapName + string("from ifName=")
									+ pPhyIntfLocalManagedObject->getId());
					pPhyIntfLocalManagedObject->addAttributeToBeUpdated(
							"qosDscpMap");
					pPhyIntfLocalManagedObject->setQosDscpMap(
							ObjectId::NullObjectId);
					updateWaveManagedObject(pPhyIntfLocalManagedObject);
				}
			}
			commitTransaction();
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
					pInactivePhyResults);
		}
		break;
	}

	case QOS_INTERFACE_DSCP_TRAFFIC_CLASS: {
		/* Check if map is activated in disconnected Physical interface. If yes, deactivate map association */
		status = QosUtils::getQosMapObjectIdByMapName(mapName, qosMapObjectId,
				DSCP_TRAFFIC_CLASS_MAP, pPrismLinearSequencerContext);

		if (status != WAVE_MESSAGE_SUCCESS) {
			break;
		}

		syncInactivePhyQueryCtxt.addAndAttribute(
				new AttributeObjectIdAssociation(qosMapObjectId,
						"qosDscpTrafficClassMap",
						PhyIntfLocalManagedObject::getClassName()));
		syncInactivePhyQueryCtxt.addSelectField("qosDscpTrafficClassMap");
		vector<WaveManagedObject *> *pInactivePhyResults = querySynchronously(
				&syncInactivePhyQueryCtxt);
		trace(TRACE_LEVEL_DEBUG,
				string("IntfConfGlobalWorker::DSCP TRAFFIC CLASS MAP"));

		if (NULL != pInactivePhyResults) {
			UI32 numberOfResults = pInactivePhyResults->size();
			trace(
					TRACE_LEVEL_DEBUG,
					string("NsmGlobalQosMapCleanUp::results=")
							+ numberOfResults);

			startTransaction();
			for (UI32 i = 0; i < numberOfResults; i++) {
				pPhyIntfLocalManagedObject =
						dynamic_cast<PhyIntfLocalManagedObject*>((*pInactivePhyResults)[i]);
				if (pPhyIntfLocalManagedObject) {
					trace(
							TRACE_LEVEL_DEBUG,
							string("Remove") + mapName + string("from ifName=")
									+ pPhyIntfLocalManagedObject->getId());
					pPhyIntfLocalManagedObject->addAttributeToBeUpdated(
							"qosDscpTrafficClassMap");
					pPhyIntfLocalManagedObject->setQosDscpTrafficClassMap(
							ObjectId::NullObjectId);
					updateWaveManagedObject(pPhyIntfLocalManagedObject);
				}
			}
			commitTransaction();
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
					pInactivePhyResults);
		}
		break;
	}

	case QOS_INTERFACE_SERVICE_POLICY: {
		/* Check if map is activated in disconnected Physical interface. If yes, deactivate map association */
		status = QosUtils::getQosMapObjectIdByMapName(mapName, qosMapObjectId,
				MQC_POLICY_MAP, pPrismLinearSequencerContext);

		if (status != WAVE_MESSAGE_SUCCESS) {
			break;
		}

		syncInactivePhyQueryCtxt.addOrAttribute(
				new AttributeObjectIdAssociation(qosMapObjectId,
						"inputPolicyMapId",
						PhyIntfLocalManagedObject::getClassName()));
		syncInactivePhyQueryCtxt.addOrAttribute(
				new AttributeObjectIdAssociation(qosMapObjectId,
						"outputPolicyMapId",
						PhyIntfLocalManagedObject::getClassName()));
		syncInactivePhyQueryCtxt.addSelectField("inputPolicyMapId");
		syncInactivePhyQueryCtxt.addSelectField("outputPolicyMapId");
		vector<WaveManagedObject *> *pInactivePhyResults = querySynchronously(
				&syncInactivePhyQueryCtxt);
		trace(TRACE_LEVEL_DEBUG,
				string("IntfConfGlobalWorker::MQC MAP"));

		if (NULL != pInactivePhyResults) {
			UI32 numberOfResults = pInactivePhyResults->size();
			trace(
					TRACE_LEVEL_DEBUG,
					string("NsmGlobalQosMapCleanUp::results=")
							+ numberOfResults);

			startTransaction();
			for (UI32 i = 0; i < numberOfResults; i++) {
				pPhyIntfLocalManagedObject =
						dynamic_cast<PhyIntfLocalManagedObject*>((*pInactivePhyResults)[i]);
				if (pPhyIntfLocalManagedObject) {
					trace(
							TRACE_LEVEL_DEBUG,
							string("Remove") + mapName + string("from ifName=")
									+ pPhyIntfLocalManagedObject->getId());

					if(qosMapObjectId == pPhyIntfLocalManagedObject->getInputPolicyMapId()) {
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated(
								"inputPolicyMapId");
						pPhyIntfLocalManagedObject->setInputPolicyMapId(
								ObjectId::NullObjectId);
					}

					if (qosMapObjectId == pPhyIntfLocalManagedObject->getOutputPolicyMapId()) {
						pPhyIntfLocalManagedObject->addAttributeToBeUpdated(
								"outputPolicyMapId");
						pPhyIntfLocalManagedObject->setOutputPolicyMapId(
								ObjectId::NullObjectId);
					}

					updateWaveManagedObject(pPhyIntfLocalManagedObject);
				}
			}
			commitTransaction();
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
					pInactivePhyResults);
		}
		break;
	}

	default: {
		trace(TRACE_LEVEL_DEBUG, string("hit default MapType=") + mapType);
		break;
	}
	}
	pPrismLinearSequencerContext->executeNextStep(WRC_WAVE_MESSAGE_SUCCESS);
	return;
}


	void
    IntfConfGlobalWorker::NsmGlobalQosMapCleanUpMessageHandler(NsmGlobalQosMapCleanUpMessage *pNsmGlobalQosMapCleanUpMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::NsmGlobalQosMapCleanUp),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext =
          new PrismLinearSequencerContext (pNsmGlobalQosMapCleanUpMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
	}

	void  IntfConfGlobalWorker::NsmGlobalCosProfileAndThresholdConfigHandler( QosGlobalCosProfileAndThresholdConfigMessage *pQosGlobalConfigMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updateQosProfThresPoIntfStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updateQosProfThresPoIntfMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }



 void IntfConfGlobalWorker::updateQosProfThresPoIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
    QosGlobalCosProfileAndThresholdConfigMessage *pQosGlobalConfigMessage =
      dynamic_cast<QosGlobalCosProfileAndThresholdConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
    trace (TRACE_LEVEL_DEBUG, string("Inside updateQosProfThresPoIntfStep-------"));
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    QosPoProfileAndThresholdSPMessage *m = NULL;
    WaveSendToClusterContext    *send_ctx_p;

    PoIntfManagedObject *pPoIntfManagedObject = NULL;
    UI32 poId = pQosGlobalConfigMessage->getPoId();
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

    if (pResults != NULL)
    {
        if (pResults->size() == 1)
        {
            trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
            pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
        }
        else
        {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            pQosGlobalConfigMessage->setCompletionStatus(WAVE_MESSAGE_ERROR);
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
            return;
        }
    }
    else
    {
            pQosGlobalConfigMessage->setCompletionStatus(WAVE_MESSAGE_ERROR);
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            trace (TRACE_LEVEL_DEBUG, string("PResults is NULL **** -------"));
            return;
    }

    m = new QosPoProfileAndThresholdSPMessage();
    m->setPoId(pQosGlobalConfigMessage->getPoId());
    m->setOpCode(pQosGlobalConfigMessage->getOpCode());
    m->setCmdCode(pQosGlobalConfigMessage->getCmdCode());

    trace (TRACE_LEVEL_DEBUG, string("pQosGlobalConfigMessage->getOpCode()=") + pQosGlobalConfigMessage->getOpCode());
    trace (TRACE_LEVEL_DEBUG, string("pQosGlobalConfigMessage->getCmdCode()=") + pQosGlobalConfigMessage->getCmdCode());
    switch (pQosGlobalConfigMessage->getOpCode())
    {
        case CONFIGURE_INTERFACE_QOS:
        {
          switch (pQosGlobalConfigMessage->getCmdCode())
          {
				case QOS_INTERFACE_COS_PROFILE: {

					if((QosUtils::isInterfaceMQCConfigured(pPoIntfManagedObject))
                        || (QosUtils::isInterfaceQosCeeConfigured(pPoIntfManagedObject))
                        || (QosUtils::isInterfaceFcoeConfigured(pPoIntfManagedObject))){

						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
						pPrismLinearSequencerContext->executeNextStep(WRC_SSM_INTF_QOS_CONFIG_CONFLICT);
                        if(NULL != m) {
                            delete m;
                        }
						return;
					}

					m->setCos(pQosGlobalConfigMessage->getCos());
					m->setRedProfile(pQosGlobalConfigMessage->getRedProfile());
					trace (TRACE_LEVEL_DEBUG, string("pQosGlobalConfigMessage->getCos()=") +
						pQosGlobalConfigMessage->getCos());
					trace (TRACE_LEVEL_DEBUG, string("pQosGlobalConfigMessage->getRedProfile()=") +
						pQosGlobalConfigMessage->getRedProfile());
					break;
				}
            case QOS_INTERFACE_TAIL_DROP:
                m->setCos(pQosGlobalConfigMessage->getCos());
                m->setTailDrop(pQosGlobalConfigMessage->getTailDrop());
                trace (TRACE_LEVEL_DEBUG, string("pQosGlobalConfigMessage->getCos()=") +
                    pQosGlobalConfigMessage->getCos());
                trace (TRACE_LEVEL_DEBUG, string("pQosGlobalConfigMessage->getTailDrop()=") +
                    pQosGlobalConfigMessage->getTailDrop());
                break;

            }
			break;
        }
        case DELETE_INTERFACE_QOS:
        {
            trace (TRACE_LEVEL_DEBUG, string("DELETE_INTERFACE_QOS"));
            m->setCos(pQosGlobalConfigMessage->getCos());   
       	    break;
        }

    }

    ObjectId poObjectId;

    status = getPoIntfObjectIdByPoIfName(pPoIntfManagedObject->getIfName(),poObjectId,true,pPrismLinearSequencerContext);
    trace (TRACE_LEVEL_DEBUG, string("getPoIntfObjectIdByPoIfName status=")+ status);

    if(status == WAVE_MESSAGE_SUCCESS)
        status = isPoIntfHasMember (poObjectId);

    if((WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ()) && (status == WAVE_MESSAGE_ERROR))
    {
        trace (TRACE_LEVEL_DEBUG, string("Returning success for empty PO in VCS Mode"));
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        pQosGlobalConfigMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        if(NULL != m) {
            delete m;
        }
        return;
    }

    trace (TRACE_LEVEL_DEBUG, string("sendSynchronouslyTo Nsm Local"));
    m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    send_ctx_p = new WaveSendToClusterContext(this,
        reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);

    int ret = SET_LOCATION_ID_API_ERROR;
    ret = NsmUtils::setLocationIdsForPoSubModeCmds(pQosGlobalConfigMessage->getPoId(), send_ctx_p);

    if(ret != SET_LOCATION_ID_API_SUCCESS)
    {
        trace(TRACE_LEVEL_ERROR, "In updateQosPfcPoIntfStep, call to setLocationIdsForPoSubModeCmds failed ");
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        pQosGlobalConfigMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        if(NULL != m) {
            delete m;
            m = NULL;
        }
        if (NULL != send_ctx_p) {
            delete send_ctx_p;
            send_ctx_p = NULL;
        }
        return;
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    trace (TRACE_LEVEL_INFO, "--------sendSynchronouslyTo Nsm Local--------");
    
    send_ctx_p->setPartialSuccessFlag(true);
    send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);

    sendToWaveCluster(send_ctx_p);
    return;
    
    }


    void IntfConfGlobalWorker::updateQosProfThresPoIntfMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
    trace (TRACE_LEVEL_DEBUG, string("Inside updateQosProfThresIntfMOStep-------"));
    QosGlobalCosProfileAndThresholdConfigMessage *pQosGlobalConfigMessage = dynamic_cast<QosGlobalCosProfileAndThresholdConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
    trace (TRACE_LEVEL_DEBUG, string("DB pQosGlobalConfigMessage->getOpCode()=") + pQosGlobalConfigMessage->getOpCode());
    trace (TRACE_LEVEL_DEBUG, string("DB pQosGlobalConfigMessage->getCmdCode()=") + pQosGlobalConfigMessage->getCmdCode());
    PoIntfManagedObject *pPoIntfManagedObject= NULL;
    UI32 poId = pQosGlobalConfigMessage->getPoId();
    //ResourceId  status   = WAVE_MESSAGE_ERROR;
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);


    if (NULL != pResults) {
        UI32 numberOfResults = pResults->size ();
        if (0 == numberOfResults) {
            trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
            //status = WAVE_MESSAGE_ERROR;
         } else if (1 == numberOfResults) {
            trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
            pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
            //status = WAVE_MESSAGE_SUCCESS;
          } else {
                //status = WAVE_MESSAGE_ERROR;
           }
      }

        prismAssert (NULL != pPoIntfManagedObject, __FILE__, __LINE__);
        updateWaveManagedObject (pPoIntfManagedObject);
        switch(pQosGlobalConfigMessage->getOpCode())
        {
            case CONFIGURE_INTERFACE_QOS:
                {
                    trace (TRACE_LEVEL_DEBUG, string("DB UPDATE CONFIGURE_INTERFACE_QOS-------"));
                    switch (pQosGlobalConfigMessage->getCmdCode()) {
                        case QOS_INTERFACE_COS_PROFILE:
						{
                        {
                        trace (TRACE_LEVEL_DEBUG, string("DB QOS_INTERFACE_COS_PROFILE cos =") + pQosGlobalConfigMessage->getCos());
                        UI32 red_profile = 0;
                        SI32 ret = pPoIntfManagedObject->getCosProfileRow(pQosGlobalConfigMessage->getCos(), red_profile);
                        if (!ret) {
                            trace (TRACE_LEVEL_DEBUG, string("DB got the cos profile row"));
                            pPoIntfManagedObject->deleteCosProfile(pQosGlobalConfigMessage->getCos());
                        }
                        trace (TRACE_LEVEL_DEBUG, string("DB new cos profile row"));
                            QosCosToProfileIdManagedObject *cosProfileMO =
                            new QosCosToProfileIdManagedObject( dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()),
                               	pQosGlobalConfigMessage->getCos(),
				pQosGlobalConfigMessage->getRedProfile());
                        pPoIntfManagedObject->addCosProfile(cosProfileMO);
                        }
                            break;
                        }
                    break;
                }
				break;
			}
            case DELETE_INTERFACE_QOS:
                {
                    trace (TRACE_LEVEL_DEBUG, string("DB UPDATE DELETE_INTERFACE_QOS-------"));
                    
		    		switch (pQosGlobalConfigMessage->getCmdCode()) {
                        case QOS_INTERFACE_COS_PROFILE:
						{
                    		trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_COS_MUTATION=") + pQosGlobalConfigMessage->getCos());
                    		pPoIntfManagedObject->deleteCosProfile(pQosGlobalConfigMessage->getCos());
                    		break;
						}
		    		}
                }
        }

        if (NULL != pResults)
        {
            if(pPrismLinearSequencerContext)
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }

        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return ;
   }

	void IntfConfGlobalWorker::deletePortProfileVlanReference(DceLinearSeqContext *pDceLinearSeqContext)
    {
        UI32 vlan_id;
		ResourceId appmStatus = WAVE_MESSAGE_ERROR;
        NsmGlobalDeleteVlanIntfMessage *pNsmGlobalDeleteVlanIntfMessage = dynamic_cast<NsmGlobalDeleteVlanIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

        vlan_id = pNsmGlobalDeleteVlanIntfMessage->getVlanId();

        trace (TRACE_LEVEL_DEBUG, string("Inside deletePortProfileVlanReference:")+ vlan_id);
        APPMGlobalAPPMVlanMessage *updateMsg = NULL;

        updateMsg = new APPMGlobalAPPMVlanMessage();
		updateMsg->setOpCode(APPM_SET_ELEM);
		updateMsg->setCmdCode(VLAN_NSM_INTF_DELETE);
		updateMsg->setAccessVlan(vlan_id);
		updateMsg->setIsChangeVlanList(pNsmGlobalDeleteVlanIntfMessage->getIsChangeAPPMVlanList());

        ResourceId sendStatus = sendSynchronously(updateMsg);
        trace (TRACE_LEVEL_DEBUG, string("Inside deletePortProfileVlanReference: sendStatus = ")+ FrameworkToolKit::localize(sendStatus));
		appmStatus = updateMsg->getCompletionStatus();
        delete updateMsg;

        pDceLinearSeqContext->executeNextStep(appmStatus);
    }

	void IntfConfGlobalWorker::deletePoIntfVlanReference(const UI32 &vlanId, DceLinearSeqContext *pDceLinearSeqContext)
    {
		trace (TRACE_LEVEL_DEBUG, string("Inside deletePoIntfVlanReference:")+ vlanId);

		UI32 vlan_id = vlanId;
		PoIntfManagedObject *pPoCfgMO = NULL;
		vector<string> selectFields;
		ObjectId vlanObjectId = pDceLinearSeqContext->getWaveManagedObjectId();

		selectFields.push_back ("isAllowedVlanNone");
		selectFields.push_back ("isAllowedVlanAll");
		selectFields.push_back ("nativeVlanIntfConfig");
		selectFields.push_back ("vlanRangeRemove");
		selectFields.push_back ("swMode");
		selectFields.push_back ("vlanObjectId");
		selectFields.push_back ("vlanRangeAdd");
		selectFields.push_back ("eldprio");
		selectFields.push_back ("vlanid");

		selectFields.push_back ("pVlanModeTrunkBas");
		selectFields.push_back ("pVlanModeTrunkPromis");
        selectFields.push_back ("pVlanRangeAdd");
        selectFields.push_back ("pVlanRangeRemove");
        selectFields.push_back ("pVlanIsAllowedVlanAll");
        selectFields.push_back ("pVlanNativeVlanId");

		WaveManagedObjectSynchronousQueryContext pochSyncQueryCtxt(PoIntfManagedObject::getClassName());
        pochSyncQueryCtxt.addSelectFields(selectFields);
		pochSyncQueryCtxt.addOrAttribute(new AttributeEnum(SW_MODE_TRUNK, "swMode"));
		pochSyncQueryCtxt.addOrAttribute(new AttributeBool(true, "pVlanModeTrunkBas"));
		pochSyncQueryCtxt.addOrAttribute(new AttributeBool(true, "pVlanModeTrunkPromis"));
		pochSyncQueryCtxt.addOrAttribute(new AttributeObjectIdAssociation(&vlanObjectId, "vlanObjectId", VlanIntfManagedObject::getClassName()));

		vector<WaveManagedObject *> *pPoResults = querySynchronously (&pochSyncQueryCtxt);

		if (NULL != pPoResults && !(pPoResults->empty()) )
        {
            ObjectId defaultVlanObjectId = ObjectId::NullObjectId;

			UI32 numberOfResults = pPoResults->size ();
				UI32 count = 0;
				while(count != numberOfResults)
				{
					pPoCfgMO = dynamic_cast<PoIntfManagedObject *>
						((*pPoResults)[count]);

					if(!(pPoCfgMO->getIsAllowedVlanNone()))
					{
						if(pPoCfgMO->getIsAllowedVlanAll())
						{
	                        WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanMo =
	                            pPoCfgMO->getNativeVlanIntfConfig();

                            NativeVlanIntfConfLocalManagedObject *nativeTmp = pNativeVlanMo.operator-> ();
                            if(nativeTmp->getNativeVlanId() == vlan_id)
                            {
                            	NativeVlanIntfConfLocalManagedObject *pNativeVlanIntfConfLocalManagedObject =
                                	new NativeVlanIntfConfLocalManagedObject(
                                    	dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));

                                pNativeVlanIntfConfLocalManagedObject->setNativevlanDisable(nativeTmp->getNativevlanDisable());
                                pNativeVlanIntfConfLocalManagedObject->setNativevlanPresence(true);
                                pNativeVlanIntfConfLocalManagedObject->setNativeVlanId(NSM_NATIVE_VLAN_DEFAULT);

                               	pPoCfgMO->setNativeVlanIntfConfig(
                               	WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> (
                               		pNativeVlanIntfConfLocalManagedObject));
                                pPoCfgMO->addAttributeToBeUpdated ("nativeVlanIntfConfig");
							}

							vector<UI32>::iterator moremoveiter;	
							UI32Range moRemoveVlanrange = UI32Range("");
							vector <UI32> moRemoveVlanRangeVector;
							moRemoveVlanrange = pPoCfgMO->getVlanRangeRemove();
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
									pPoCfgMO->setVlanRangeRemove(moRemoveVlanrange);
                                    pPoCfgMO->addAttributeToBeUpdated ("vlanRangeRemove");
									break;
								}
							}
						}

						else
						{/*Search if it is a part of trunk or access vlan*/
							if(pPoCfgMO->getSwMode() == SW_MODE_ACCESS)
					{

								if(pPoCfgMO->getAccessVlanObjectId() == vlanObjectId)
								{
                                    if (ObjectId::NullObjectId == defaultVlanObjectId) {
                                        getVlanIntfObjectIdByVlanIfName (
                                            NSM_NATIVE_VLAN_DEFAULT, defaultVlanObjectId, false, pDceLinearSeqContext);
                                    }
									pPoCfgMO->setAccessVlanObjectId (defaultVlanObjectId);
                                     pPoCfgMO->addAttributeToBeUpdated ("vlanObjectId");
								}
					}
							else if (pPoCfgMO->getSwMode() == SW_MODE_TRUNK || pPoCfgMO->getPvlanModeTrunkPromis())
							{
								WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanMo = 
								    pPoCfgMO->getNativeVlanIntfConfig();

								NativeVlanIntfConfLocalManagedObject *nativeTmp = pNativeVlanMo.operator-> (); 
								if(pNativeVlanMo.operator->() != NULL) {
									if(nativeTmp->getNativeVlanId() == vlan_id)
									{ 
									
										NativeVlanIntfConfLocalManagedObject *pNativeVlanIntfConfLocalManagedObject = 
											NativeVlanPoGetChildMoByParentMo(pPoCfgMO);
                                        
										if(pNativeVlanIntfConfLocalManagedObject != NULL)
										{
											pNativeVlanIntfConfLocalManagedObject->setNativeVlanId(NSM_NATIVE_VLAN_DEFAULT);
											updateWaveManagedObject (pNativeVlanIntfConfLocalManagedObject);
											if (pDceLinearSeqContext)
												pDceLinearSeqContext->addManagedObjectForGarbageCollection(pNativeVlanIntfConfLocalManagedObject);
										}

				                  }
								}
								// else
								{
									vector <UI32> moAddVlanRangeVector;
									UI32Range moAddVlanrange = UI32Range("");
									vector<UI32>::iterator moadditer;

									moAddVlanrange = pPoCfgMO->getVlanRangeAdd();
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
									pPoCfgMO->setVlanRangeAdd(moAddVlanrange);
                                    pPoCfgMO->addAttributeToBeUpdated ("vlanRangeAdd");
								}
							}
						}
					}

					if(pPoCfgMO->getPvlanModeTrunkBas())
					{
						if(pPoCfgMO->getPvlanNativeVlanId() == vlan_id)
						{
							pPoCfgMO->setPvlanNativeVlanId(0);
							pPoCfgMO->addAttributeToBeUpdated ("pVlanNativeVlanId");
						}

						if(pPoCfgMO->getPvlanIsAllowedVlanAll())
						{
							if(pPoCfgMO->getPvlanRangeRemove().find(vlan_id))
							{
								UI32Range rangeFromMsg(vlan_id, vlan_id);
								UI32Range moVlanrange = pPoCfgMO->getPvlanRangeRemove();
								pPoCfgMO->setPvlanRangeRemove(moVlanrange - rangeFromMsg);
								pPoCfgMO->addAttributeToBeUpdated("pVlanRangeRemove");
							}

						}
						else if(pPoCfgMO->getPvlanRangeAdd().find(vlan_id))
						{
							UI32Range rangeFromMsg(vlan_id, vlan_id);
							UI32Range moVlanrange = pPoCfgMO->getPvlanRangeAdd();
							pPoCfgMO->setPvlanRangeAdd(moVlanrange - rangeFromMsg);
							pPoCfgMO->addAttributeToBeUpdated("pVlanRangeAdd");
						}
					}

					/* 
					 * ELD records for the VLAN to be deleted are
					 * being removed for the interface.
					 */
					UI32 j;
					ObjectId pVlanIntfObjId = ObjectId::NullObjectId;
					getVlanIntfObjectIdByVlanIfName(vlan_id, pVlanIntfObjId, false, pDceLinearSeqContext);
					vector<ObjectId>  m_vlanid = pPoCfgMO->getVlanVector();
					if (m_vlanid.size() > 0) {
						for (j = 0; j < m_vlanid.size(); j++) {
							if (m_vlanid[j] == ObjectId::NullObjectId) {
								NSM_PLUG_INFO_S(j + string ("VLAN ObjId Null: continue"));
								continue;
							}
							if(m_vlanid[j] == pVlanIntfObjId)
							{
								pPoCfgMO->deleteVlanObjectId(m_vlanid[j]);
                                                                pPoCfgMO->addAttributeToBeUpdated ("vlanid");
								break;
							}
						}
					}
					// Its possible that none of the above conditions are met
					// Do an update MO iff we have any attributes to be updated
                                        if (!pPoCfgMO->getAttributesToBeUpdated().empty()) {
                                          updateWaveManagedObject(pPoCfgMO);
                                        }
					count++;
			 }

			if(pDceLinearSeqContext)
				pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*pPoResults);
			pPoResults->clear ();
			delete pPoResults;
		}
    }

	 void IntfConfGlobalWorker::removeVlanClassifierPoIntf(const ObjectId &vlanObjectId)
    {
       NSM_PLUG_DBG("Entering ...");
       string parentClass = PoIntfManagedObject::getClassName();
       string childClass = VlanClassifierActivateGlobalManagedObject::getClassName();
       string compositionName = "vlanClassifierActivate";
	   vector<string> selectFields;

	   selectFields.push_back ("objectId");
	   selectFields.push_back ("ownerManagedObjectId");


       WaveManagedObjectSynchronousQueryContext syncDeActivateQueryCtxt(VlanClassifierActivateGlobalManagedObject::getClassName());
       syncDeActivateQueryCtxt.addAndAttribute(new AttributeObjectId(vlanObjectId, "vlan"));
       syncDeActivateQueryCtxt.addSelectFields(selectFields);
       vector<WaveManagedObject * >*               pDeActivateResults                   =  querySynchronously(&syncDeActivateQueryCtxt);

	   if ((pDeActivateResults->size())) {


		 for(unsigned int j = 0;j < pDeActivateResults -> size() ;j++)
		 {
		   NSM_PLUG_DBG_S(string("j = ") + j);
		   VlanClassifierActivateGlobalManagedObject *pVlanClassifierActivateGlobalManagedObject = dynamic_cast<VlanClassifierActivateGlobalManagedObject*>((*pDeActivateResults)[j]);
		   deleteFromComposition(parentClass, childClass, compositionName,
			   pVlanClassifierActivateGlobalManagedObject->getOwnerManagedObjectId(),
			   pVlanClassifierActivateGlobalManagedObject->getObjectId());

		 }

	   }

	   if (NULL != pDeActivateResults)
	   {
		   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pDeActivateResults);
	   }
    }

    ResourceId IntfConfGlobalWorker::getPoMoByPoId(UI32 PoId, PoIntfManagedObject * * ppPoIntfManagedObject)
    {
        ResourceId                               status = WAVE_MESSAGE_SUCCESS;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());

        syncQueryCtxt.addAndAttribute(new AttributeUI32(&PoId, "id"));
        vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);
        if (pResults && (pResults -> size() == 1))
        {
           *ppPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
           status                 = WAVE_MESSAGE_SUCCESS;
		   pResults->clear();
		   delete pResults;
        }
        return status;
    }

    void IntfConfGlobalWorker::defaultConfMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        ResourceId  status;
        
        /* Installing default Reserved vlan MO */
        ReservedVlanManagedObject *pReservedVlanManagedObject = new ReservedVlanManagedObject (dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        tracePrintf (TRACE_LEVEL_DEBUG, "IntfConfGlobalWorker::defaultConfMOStep : ReservedVlanManagedObject %p ", pReservedVlanManagedObject);
        /* Add Mo to garbage collector to prevent memory leaks */
        pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pReservedVlanManagedObject);
        /* Need to update vlan index */
        NsmGlobalCreateVlanIntfMessage  message          (NSM_NATIVE_VLAN_DEFAULT,"1",1500,"");
        message.setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        status = createVlanIntfMO(&message,pPrismLinearSequencerContext,true);
	

	  
        /* Kunjan - No need to create VlanIntfMO object for FCOE Vlan during startup. So commented this code.
		if vcs enabled , create default vlan entries vlan 4095,4093 
        if (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            NsmGlobalCreateVlanIntfMessage  message1          ("1002","1002",1500,"",NSM_VLAN_INTF_TYPE_FCOE);
            message1.setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            status = createVlanIntfMO(&message1,pPrismLinearSequencerContext,true);
        }*/

        LacpConfigManagedObject *pLacpConfigManagedObject = new LacpConfigManagedObject
                        (dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        pLacpConfigManagedObject->setSystemPriority (32768);
        pLacpConfigManagedObject->setDot1xenable (false);
        pLacpConfigManagedObject->setDot1xtesttimeout (10);
        pLacpConfigManagedObject->setNativevlanDisable (true);

        pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pLacpConfigManagedObject);


		UI32 gVlanState = GVLAN_STATE_DISABLE;
		Attribute *configAttribute = new AttributeUI32(&gVlanState, NSM_GVLAN_PERSISTENCE_MO_NAME);
		addWaveConfigEntry(configAttribute);
		delete configAttribute;
		trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::defaultConfMOStep:Added GVlan config state with value:") + gVlanState);

        WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = dynamic_cast<WaveAsynchronousContextForBootPhases *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext());

        pWaveAsynchronousContextForBootPhases->setCompletionStatus(status);

        pPrismLinearSequencerContext->executeNextStep(status);

        trace(TRACE_LEVEL_INFO, string("Exit defaultConfMOStep ...  "));
    }

	void IntfConfGlobalWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
		string default_val;
        trace(TRACE_LEVEL_INFO, string("Entering IntfConfGlobalWorker::install () ..."));

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        if ((pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_FIRST_TIME_BOOT) && (pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT)) {
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }

        trace(TRACE_LEVEL_INFO, string("Entering IntfConfGlobalWorker::install () ... Create Default Entries ..."));

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&IntfConfGlobalWorker::defaultConfMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveAsynchronousContextForBootPhases, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }

	ResourceId IntfConfGlobalWorker::deleteIntfAssociatedIngressEgressAcl (ObjectId policyId, UI32 aclType)
	{
		trace(TRACE_LEVEL_DEBUG, "Entering IntfGlobalWorker::deleteIntfAssociatedIngressEgressAcl..");

		ResourceId  status   = WAVE_MESSAGE_SUCCESS;
		string moClassName = "";
		string associatiedAttrName = "";
		string compositionName = "";

		if(aclType == SSMACL_TYPE_L2_STD_E || aclType == SSMACL_TYPE_L2_EXT_E) {
			moClassName = NsmLocalIngressEgressMacAclPolicyManagedObject::getClassName ();
			associatiedAttrName = "ingressEgressMacAcl";
			compositionName = "ingressEgressMacAclPolicy";
		} else if(aclType == SSMACL_TYPE_L3_IPV4_STD_E || aclType == SSMACL_TYPE_L3_IPV4_EXT_E) {
			moClassName = NsmLocalIngressEgressIpAclPolicyManagedObject::getClassName ();
			associatiedAttrName = "ingressEgressIpAcl";
			compositionName = "ingressEgressIpAclPolicy";
		}

		WaveManagedObjectSynchronousQueryContext ingressEgressAclQueryCtx (moClassName);
		ingressEgressAclQueryCtx.addAndAttribute( new AttributeObjectIdAssociation(
				policyId, associatiedAttrName, moClassName));
		ingressEgressAclQueryCtx.addSelectField("objectId");
		ingressEgressAclQueryCtx.addSelectField("ownerManagedObjectId");
		vector<WaveManagedObject *> *pResults = querySynchronously (&ingressEgressAclQueryCtx);
		WaveManagedObject *pIngressEgressAclManagedObject = NULL;

		if ( NULL != pResults )
		{
			startTransaction ();

			for(size_t i = 0; i < pResults->size() ; i++)
			{
				if(aclType == SSMACL_TYPE_L2_STD_E || aclType == SSMACL_TYPE_L2_EXT_E) {
					pIngressEgressAclManagedObject = dynamic_cast<NsmLocalIngressEgressMacAclPolicyManagedObject *>((*pResults)[i]);
				} else if(aclType == SSMACL_TYPE_L3_IPV4_STD_E || aclType == SSMACL_TYPE_L3_IPV4_EXT_E) {
					pIngressEgressAclManagedObject = dynamic_cast<NsmLocalIngressEgressIpAclPolicyManagedObject *>((*pResults)[i]);
				}
				trace(TRACE_LEVEL_DEBUG, string("deleteIntfAssociatedIngressEgressAcl : deleting objectId --> ")+
						pIngressEgressAclManagedObject->getObjectId().toString());
				trace(TRACE_LEVEL_DEBUG, string("deleteIntfAssociatedIngressEgressAcl : associated owner objectId --> ")+
						pIngressEgressAclManagedObject->getOwnerManagedObjectId().toString());

				deleteFromComposition(IntfManagedObject::getClassName(),moClassName, compositionName,
						pIngressEgressAclManagedObject->getOwnerManagedObjectId(), pIngressEgressAclManagedObject->getObjectId());
				delete pIngressEgressAclManagedObject;
			}

			commitTransaction ();
		}

		return status;
	}

    void IntfConfGlobalWorker::SsmDeleteAssociationwithDisconnectedMessageHandler (
        SsmDeleteAssociationwithDisconnectedMessage *pSsmDeleteAssociationwithDisconnectedMessage)
    {
		trace(TRACE_LEVEL_INFO, "Entering IntfGlobalWorker::SsmDeleteAssociationwithDisconnectedMessageHandler \n");

        ResourceId  status   = WAVE_MESSAGE_SUCCESS;

        ObjectId policyId = pSsmDeleteAssociationwithDisconnectedMessage->getPolicyId();
        UI32 aclType = pSsmDeleteAssociationwithDisconnectedMessage->getAclType();

        status = deleteIntfAssociatedIngressEgressAcl(policyId, aclType);

    	pSsmDeleteAssociationwithDisconnectedMessage->setCompletionStatus (status);

		reply (pSsmDeleteAssociationwithDisconnectedMessage);
		return;
	}

    ResourceId IntfConfGlobalWorker :: validatePortChannelCount() {
    	UI32 countPoIntfs = 0;
    	WaveManagedObjectSynchronousQueryContext poQueryCtxt(PoIntfManagedObject::getClassName());
    	querySynchronouslyForCount(&poQueryCtxt, countPoIntfs);
        trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker :: validatePortChannelCount. Count of POs: ") + countPoIntfs);

        // In case of VCS Mode, port-channel created is not pushed to backend.
        // it will be pushed to backend only when any interface is added to it.
        // Since number of interfaces are specific to platform so maximum number
        // of port-channel created will be same as number of interfaces. So in 
        // case of VCS mode, there is no need to check for port-channel count.
        // But in case of SA mode, even empty port-channel will be pushed to 
        // backend, so there is a risk of creating max of 6144 port-channels
        // and pushed to backend. Thus we need to check for port-channel
        // count in SA mode.

        // Though check is not required for VCS mode, but here we are checking
        // that count should not exceed allowable port-channel Ids.
        //
        //  |==================================================================|
        //  | Platform                  |  Max no. of LAGS created per node    |
        //  |===========================|======================================|
        //  | Callisto-24, Callisto-60, |  64                                  |
        //  | Carme                     |                                      |
        //  |---------------------------|--------------------------------------|
        //  | Mercury-4                 |  192                                 |
        //  |---------------------------|--------------------------------------|
        //  | Mercury-8                 |  384                                 |
        //  |---------------------------|--------------------------------------|
        //  | Mercury-16                |  xxx                                 |
        //  |==================================================================|
        //  

    	if (WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ()) {
    		if (DCM_PORTCHANNEL_MAX_VLAGS == countPoIntfs) {
    			return WRC_NSM_API_ERR_PORTCHANNEL_MAX_VLAGS;
    		}
    	} else {
            if ((Capabilities :: isCapabilityPresent(FEAT_CAPABILITY_64_PORT_CHANNEL) && countPoIntfs == 64) ||
                (Capabilities :: isCapabilityPresent(FEAT_CAPABILITY_192_PORT_CHANNEL) && countPoIntfs == 192) ||
                (Capabilities :: isCapabilityPresent(FEAT_CAPABILITY_384_PORT_CHANNEL) && countPoIntfs == 384)) {
                return WRC_NSM_API_ERR_PORTCHANNEL_MAX_LAGS;
            }
    	}
    	return WAVE_MESSAGE_SUCCESS;
    }

    InterfaceType IntfConfGlobalWorker :: getPoFirstMemberPhyIntfType(const string &poName) {
        InterfaceType intfType = IF_TYPE_INVALID;
        ObjectId PoOid;
        NsmUtils::getPoIntfObjectIdByPoIfName(poName, PoOid, false);

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeObjectId(PoOid, "poId"));
        syncQueryCtxt.addSelectField("phyType");
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (pResults && pResults->size() > 0) {
            pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);
            intfType = pPhyIntfLocalManagedObject->getPhyType();
        }
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        return intfType;
    }

    /*
     * This method will check for presence of L3 Configs on NsmL3ReadyMO while
     * channel-group command
     */
    ResourceId IntfConfGlobalWorker :: validateL3ConfigForChannelGroup(NsmL3ReadyManagedObject *pL3ReadyMo) {
        if (pL3ReadyMo) {
            WaveManagedObjectPointer<PimIntfCfgLocalManagedObject> pimsmCfg = pL3ReadyMo->getPimSmCfg();
            WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject> ospfIntfConfig = pL3ReadyMo->getOSPFIntfConfig();
            if (pimsmCfg.operator->() || ospfIntfConfig.operator->() || isIgmpConfigPresent(pL3ReadyMo)) {
                return WRC_NSM_ERR_CHANNEL_GROUP_NOT_ALLOWEED;
            }

        }
        return WAVE_MESSAGE_SUCCESS;
    }

    bool IntfConfGlobalWorker :: isIgmpConfigPresent(NsmL3ReadyManagedObject *pL3ReadyMo) {
        IgmpIntfConfManagedObject* igmpIntfConfig = pL3ReadyMo->getIgmpIntfConf();
        vector<WaveManagedObjectPointer<IgmpSgIntfConfManagedObject> > igmpSgMOs  = pL3ReadyMo->getIgmpSgIntfConf();
        int numIgmpSg = igmpSgMOs.size();

        if  (igmpIntfConfig && igmpIntfConfig->hasNonDefaultConfig()) {
            return true; 
        } 
        if(numIgmpSg > 0) {
            return true;
        }

        return false;
    } 

    ResourceId IntfConfGlobalWorker::validateVlanForRSPANConfig(NsmGlobalUpdatePoIntfMessage *pMsg,DceLinearSeqContext *seq_ctx_p) {
    	ResourceId status = WAVE_MESSAGE_SUCCESS;
    	NSM_PLUG_DBG_S(string("Entering "));
    	if(pMsg->getCmdCode() == NSMUPDATE_PO_SW_ADD_ACCESS_VLAN || pMsg->getCmdCode() == NSMUPDATE_PO_SW_ADD_TRUNK_VLAN || pMsg->getCmdCode() == NSMUPDATE_PO_SW_TRUNK_VLAN_ALL|| pMsg->getCmdCode() == NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN) {
			UI32Range vlanAddRange = pMsg->getVlanRange();
			vector <UI32> msgVlanRangeVector;
			vlanAddRange.getUI32RangeVector(msgVlanRangeVector);
			size_t vidCount = msgVlanRangeVector.size();
			NSM_PLUG_DBG_S(string("number of vlans=")+vidCount);
			if (msgVlanRangeVector.empty()) {
				NSM_PLUG_DBG_S(string("No VLAN from the message "));
				return status;
			}
			vector<Attribute *> vidAttributes;
			for(size_t i = 0; i < vidCount; ++i) {
				UI32 vlanId = msgVlanRangeVector[i];
				if(NsmUtils::isRemoteSpanVlan(vlanId)) {
				   status = WRC_NSM_RSPAN_VLAN_CANNOT_HAVE_PO_MEMBER;
				   NSM_PLUG_DBG_S(string("VLAN ")+vlanId+" has RSPAN configured");
				   break;
				}
			}
    	}
        NSM_PLUG_DBG_S(string("Exiting "));
    	return status;
    }

	void IntfConfGlobalWorker::switchportVlanConfigGlobalMessageHandler(SwitchportVlanConfigGlobalMessage *pSwitchportVlanConfigGlobalMessage)
	{
		PrismLinearSequencerStep sequencerSteps[] =
		{
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::validateSwitchportVlanConfigStep),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::switchportVlanConfigStep),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::switchportVlanConfigMOStep),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
		};

		DceLinearSeqContext *pDceLinearSeqContext = new DceLinearSeqContext (pSwitchportVlanConfigGlobalMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
		pDceLinearSeqContext->holdAll();
		pDceLinearSeqContext->start ();
	}

	void IntfConfGlobalWorker::validateSwitchportVlanConfigStep(DceLinearSeqContext *pContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		if(ProvisioningUtil::isGvlanEnabled() == GVLAN_STATE_DISABLE)
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:GVlan is not enabled."));

			status = WRC_NSM_GVLAN_NOT_CAPABLE_FABRIC;
			pContext->executeNextStep(status);

			return;
		}

		SwitchportVlanConfigGlobalMessage *pMsg = dynamic_cast<SwitchportVlanConfigGlobalMessage*> (pContext->getPPrismMessage ());
		UI32 cmdCode = pMsg->getCmdCode(), count = 0, poId = atoi(pMsg->getIfName().c_str()), ctagId = pMsg->getCtagId();
		PoIntfManagedObject *pPoMO = NULL; 

		WaveManagedObjectSynchronousQueryContext poSyncQueryCtxt (PoIntfManagedObject::getClassName ());
		poSyncQueryCtxt.addAndAttribute (new AttributeUI32 (poId, "id"));
	    poSyncQueryCtxt.addSelectField ("swModeFlag");
	    poSyncQueryCtxt.addSelectField ("swMode");
        poSyncQueryCtxt.addSelectField ("isPortProfiled");
        poSyncQueryCtxt.addSelectField ("associatedPortProfiles");
        poSyncQueryCtxt.addSelectField ("associatedPortProfileDomain");
	    poSyncQueryCtxt.addSelectField ("pVlanModeTrunkBas");
	    poSyncQueryCtxt.addSelectField ("pVlanModeTrunkPromis");
	    poSyncQueryCtxt.addSelectField ("pVlanModeTrunkHost");
		poSyncQueryCtxt.addSelectField ("isAllowedVlanAll");
		poSyncQueryCtxt.addSelectField ("vlanRangeAdd");
		poSyncQueryCtxt.addSelectField ("vlanRangeAdd");
		poSyncQueryCtxt.addSelectField ("pVlanIsAllowedVlanAll");
		poSyncQueryCtxt.addSelectField ("pVlanRangeAdd");
		poSyncQueryCtxt.addSelectField ("pVlanRangeRemove");
		poSyncQueryCtxt.addSelectField ("vlanObjectId");

		vector<WaveManagedObject *> *pPoResults = querySynchronously (&poSyncQueryCtxt);

		if((pPoResults != NULL) && (pPoResults->size() != 0))
		{
			pContext->addManagedObjectsForGarbageCollection(*pPoResults);
			pPoMO =  dynamic_cast<PoIntfManagedObject*>((*pPoResults)[0]);
			pPoResults->clear();
			delete pPoResults;
			pPoResults = NULL;
		}

		if (pPoMO == NULL)
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep: Interface does not exist."));

			status = WRC_NSM_ERR_PHYINTF_INVALID;
			pContext->executeNextStep(status);

			return;
		}

		UI32 vlanId = pMsg->getVlanId();
		WaveManagedObjectSynchronousQueryContext vlanSyncQueryCtxt (VlanIntfManagedObject::getClassName ());
		vlanSyncQueryCtxt.addOrAttribute (new AttributeUI32 (vlanId, "id"));
		vlanSyncQueryCtxt.addAndAttribute(new AttributeBool(false, "remoteSpanEnabled"));
		vector<WaveManagedObject *> *pVlanResults = querySynchronously (&vlanSyncQueryCtxt);
		VlanIntfManagedObject *pVlanMO = NULL;

		if((pVlanResults != NULL) && (pVlanResults->size() != 0))
		{
			pContext->addManagedObjectsForGarbageCollection(*pVlanResults);
			pVlanMO =  dynamic_cast<VlanIntfManagedObject*>((*pVlanResults)[0]);
			pVlanResults->clear();
			delete pVlanResults;
			pVlanResults = NULL;
		}

		if(pVlanMO == NULL)
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Vlan is not configured."));

			status = WRC_NSM_API_ERR_VLAN_NOT_CONFIGURED;
			pContext->executeNextStep(status);

			return;
		}

        bool isPortProfiled = false;
        APPMUtils::isPortProfilePort(pPoMO, isPortProfiled);
        if(isPortProfiled) {
            status = WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED;
            pContext->executeNextStep(status);
            return;
        }

		/* Check if the swtchport mode is enabled */
		if(!pPoMO->getSwModeFlag())
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Interface is not configured for switching."));

			status = WRC_SWITCHING_NOT_ENABLED;
			pContext->executeNextStep(status);

			return;
		}

		pvlanTypeEnum pVlanType = pVlanMO->getPVlanType();

		switch(cmdCode)
		{
			case NSMUPDATE_PO_SET_MAC_ADDRESS:
			{
				/* Check if the switchport mode is access */
				if(pPoMO->getSwMode() != SW_MODE_ACCESS)
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Interface is not in switchport access mode."));

					status = WRC_SW_ACCESSMODE_NOT_ENABLED;
					break;
				}

				// If the vlan is a primary vlan, then return ERROR.
				if(pVlanType != PVLAN_TYPE_UNKNOWN)
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Only regular vlans can be configured with a ctag in trunk mode."));

					status = WRC_NSM_PVLAN_VLAN;
					break;
				}

				if(pPoMO->getAccessVlanObjectId() == pVlanMO->getObjectId())
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker:validateSwitchportVlanConfigStep:Vlan Id has already been configured as an access vlan on this port."));

					status = WRC_NSM_GVLAN_VLAN_IS_USED_AS_ACCESSVLAN;
					break;
				}

			}
			break;

			case NSMUPDATE_PO_SET_MAC_GROUP:
			{
				/* Check if the switchport mode is access */
				if(pPoMO->getSwMode() != SW_MODE_ACCESS)
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Interface is not in switchport access mode."));

					status = WRC_SW_ACCESSMODE_NOT_ENABLED;
					break;
				}

				// If the vlan is a primary vlan, then return ERROR.
				if(pVlanType != PVLAN_TYPE_UNKNOWN)
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Only regular vlans can be configured with a ctag in trunk mode."));

					status = WRC_NSM_PVLAN_VLAN;
					break;
				}

				if(pPoMO->getAccessVlanObjectId() == pVlanMO->getObjectId())
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker:validateSwitchportVlanConfigStep:Vlan Id has already been configured as an access vlan on this port."));

					status = WRC_NSM_GVLAN_VLAN_IS_USED_AS_ACCESSVLAN;
					break;
				}

				count = 0;
				WaveManagedObjectSynchronousQueryContext macGroupQueryCtxt (MacGroupManagedObject::getClassName ());
				macGroupQueryCtxt.addOrAttribute (new AttributeUI32 (pMsg->getMacGroupId(), "id"));
				querySynchronouslyForCount (&macGroupQueryCtxt, count);

				if(count != 1)
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep: MacGroup is not configured."));

					status = WRC_NSM_ERR_MAC_GROUP_NOT_CONFIGURED;
					break;
				}
			}
			break;

			case NSMUPDATE_PO_DEL_MAC_ADDRESS:
			case NSMUPDATE_PO_DEL_MAC_GROUP:
			{
				/* Check if the switchport mode is access */
				if(pPoMO->getSwMode() != SW_MODE_ACCESS)
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Interface is not in switchport access mode."));

					status = WRC_SW_ACCESSMODE_NOT_ENABLED;
					break;
				}
			}
			break;

			case NSMUPDATE_PO_SET_CTAG:
			{
				/* The port should be in Trunk or Private-VLAN-Trunk-Promis mode. */
				if(!((pPoMO->getSwMode() == SW_MODE_TRUNK) || (pPoMO->getPvlanModeTrunkPromis()) || (pPoMO->getPvlanModeTrunkHost())))
				{
					trace(TRACE_LEVEL_ERROR,string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Interface is not in trunk or pvlan-trunk-promis or pvlan-trunk-host mode."));

					status = WRC_NSM_ERR_PORT_NOT_TRUNK_OR_TRUNK_PROMIS;
					break;
				}

				//Validation about vlan begins  here.
				//--------------------------------------------------
				//		Interface mode				|	vlan-type
				//--------------------------------------------------
				//trunk								|	regular vlan
				//private-vlan-trunk-promiscuous	|	regular vlan + primary-vlan
				//private-vlan-trunk-host			|	secondary vlan
				if(pPoMO->getSwMode() == SW_MODE_TRUNK)
				{
					if(pVlanType != PVLAN_TYPE_UNKNOWN)
					{
						trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Only regular vlans can be configured with a ctag in trunk mode."));

						status = WRC_NSM_PVLAN_VLAN;
						break;
					}
				}
				else if(pPoMO->getPvlanModeTrunkPromis())
				{
					if(!((pVlanType == PVLAN_TYPE_UNKNOWN) || (pVlanType == PVLAN_TYPE_PRIMARY)))
					{
						trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Only regular or primary vlans can be configured with a ctag in private-vlan-trunk-promiscuous mode."));

						status = WRC_NSM_PVLAN_SEC_VLAN;
						break;
					}
				}
				else if(pPoMO->getPvlanModeTrunkHost())
				{
					if(!((pVlanType == PVLAN_TYPE_ISOLATED) || (pVlanType == PVLAN_TYPE_COMMUNITY)))
					{
						trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Only secondary vlans can be configured with a ctag in private-vlan-trunk-host mode."));

						status = WRC_NSM_PVLAN_NOT_SEC_VLAN;
						break;
					}
				}

				if(pPoMO->getIsAllowedVlanAll())
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Cannot configure ctags when allowed vlan all is set."));

					status = WRC_NSM_GVLAN_CANNOT_ADD_VLAN_CLASSIFICATION_WHEN_TRUNK_ALL;
					break;
				}

				// Validation about ctag begins.
				// If ctag is in the allowed range of trunk/trunk rspan, then reject the configuration.
				if(pPoMO->getVlanRangeAdd().find(ctagId))
				{
					trace(TRACE_LEVEL_ERROR,string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:ctag is in trunk allowed range."));

					status = WRC_NSM_GVLAN_CTAG_IS_IN_ALLOWED_ADD_RANGE;
					break;
				}

				/* If the ctag is used for another vlan on the same port, then reject the configuration. */
				if(NsmUtils::getVlanClassificationMOCount(0, ctagId, pPoMO->getObjectId()))
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Ctag is already used for another vlan on the same port."));

					status = WRC_NSM_VLAN_ERR_CTAG_ALREADY_IN_USE;
					break;
				}
			}
			break;

			case NSMUPDATE_PO_DEL_CTAG:
			{
				/* The port should be in Trunk or Private-VLAN-Trunk-Promis mode. */
				if(!((pPoMO->getSwMode() == SW_MODE_TRUNK) || (pPoMO->getPvlanModeTrunkPromis()) || (pPoMO->getPvlanModeTrunkHost())))
				{
					trace(TRACE_LEVEL_ERROR,string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Interface is not in trunk or pvlan-trunk-promis or pvlan-trunk-host mode."));

					status = WRC_NSM_ERR_PORT_NOT_TRUNK_OR_TRUNK_PROMIS;
					break;
				}

				// When the port is in private-vlan-trunk-host mode and if the vlanId is configured as the pVlanHostSecVlanId for host-association, then return ERROR.
				if(pPoMO->getPvlanModeTrunkHost())
				{
					if(pPoMO->getPvlanHostSecVlanId() == vlanId)
					{
						trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Cannot remove ctag when the vlan is configured as secondary vlan for host association on the same port."));

						status = WRC_NSM_VLAN_AS_SEC_IN_HOST_ASSOC;
						break;
					}
				}

				// When the port is in private-vlan-trunk-promiscuous mode and if the vlanId is configured as the primaryVlanId for PVlanMapping, then return ERROR.
				if(pPoMO->getPvlanModeTrunkPromis())
				{
					if(PVlanUtils::getVlanAsPrimaryInPVlanMappingCount(vlanId, pPoMO->getObjectId()))
					{
						trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Cannot remove ctag when the vlan is configured as secondary vlan for host association on the same port."));

						status = WRC_NSM_VLAN_AS_PRI_IN_PVLAN_MAPPING;
						break;
					}
				}

			}
			break;

			case NSMUPDATE_PO_PVLAN_SET_CTAG:
			{
				/* Check if the switchport mode is private-vlan trunk mode */
				if(!pPoMO->getPvlanModeTrunkBas())
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Interface is not in switchport private-vlan trunk promiscuous mode."));

					status = WRC_PVLAN_MODE_NOT_TRUNK_PROMIS;
					break;
				}

				//If the vlan is not a primary vlan, then reject the configuration.
				if(pVlanType == PVLAN_TYPE_UNKNOWN)
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Only private-vlans can be configured with a ctag with private-vlan-trunk-add."));

					status = WRC_NSM_PVLAN_ERR_NOT_CONFIGURED;
					break;
				}

				if(pPoMO->getPvlanIsAllowedVlanAll())
				{
					trace(TRACE_LEVEL_ERROR,string("IntfConfLocalWorker::validateSwitchportVlanConfigStep:Cannot configure ctags when private-vlan allowed vlan all is set."));

					status = WRC_NSM_GVLAN_CANNOT_ADD_VLAN_CLASSIFICATION_WHEN_TRUNK_ALL;
					break;
				}

				//Validations of ctag begins here.
				//If the ctag is in the allowed private-vlan range on this port, then reject the configuration.
				if(pPoMO->getPvlanRangeAdd().find(ctagId))
				{
					trace(TRACE_LEVEL_ERROR,string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:ctag is in trunk pvlan allowed range."));

					status = WRC_NSM_GVLAN_CTAG_IS_IN_ALLOWED_ADD_RANGE;
					break;
				}

				/* If the ctag is used for another vlan on the same port, then reject the configuration. */
				if(NsmUtils::getVlanClassificationMOCount(0, ctagId, pPoMO->getObjectId()))
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Ctag is already used for another vlan on the same port."));

					status = WRC_NSM_VLAN_ERR_CTAG_ALREADY_IN_USE;
					break;
				}
			}
			break;

			case NSMUPDATE_PO_PVLAN_DEL_CTAG:
			{
				/* Check if the switchport mode is private-vlan trunk mode */
				if(!pPoMO->getPvlanModeTrunkBas())
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Interface is not in switchport private-vlan trunk promiscuous mode."));

					status = WRC_PVLAN_MODE_NOT_TRUNK_PROMIS;
					break;
				}

				// When the port is in private-vlan-trunk mode and if the vlanId is configured either as primaryVlanId or secondaryVlanId, for pvlan-trunk-association, then return ERROR.
				if(PVlanUtils::getVlanIdInPVlanTrunkAssociationCount(vlanId, pPoMO->getObjectId()))
				{
					trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep:Cannot remove private-vlan-ctag when the vlan is configured as primary or secondary in a trunk association on the same port."));

					status = WRC_NSM_VLAN_AS_PRI_OR_SEC_in_PVLAN_TRUNK_ASSOC;
					break;
				}
			}
			break;

			default:
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateSwitchportVlanConfigStep: return ERROR as an invalid command has been entered."));

				status = WAVE_MESSAGE_ERROR;
			}
			break;
		}

		pContext->executeNextStep(status);

		return;
	}

	void IntfConfGlobalWorker::switchportVlanConfigStep(DceLinearSeqContext *pContext)
	{
		int ret = SET_LOCATION_ID_API_ERROR;
		SwitchportVlanConfigGlobalMessage *pMsg = dynamic_cast<SwitchportVlanConfigGlobalMessage*> (pContext->getPPrismMessage ());
		NsmUpdatePoIntfSPMessage *pLocalMsg = new NsmUpdatePoIntfSPMessage();
		UI32Range vlanRange(pMsg->getVlanId(), pMsg->getVlanId());
		string poId = pMsg->getIfName();

		pLocalMsg->setCmdCode(pMsg->getCmdCode());
		pLocalMsg->setPoId(poId);
		pLocalMsg->setVlanRange(vlanRange);
		pLocalMsg->setMac(pMsg->getMac());
		pLocalMsg->setCtagId(pMsg->getCtagId());
		pLocalMsg->setMacGroupId(pMsg->getMacGroupId());

		WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback), pContext);

		prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);

		send_ctx_p->setPPrismMessageForPhase1(pLocalMsg);
		ret = NsmUtils::setLocationIdsForPoSubModeCmds(poId, send_ctx_p, false);

		if(ret != SET_LOCATION_ID_API_SUCCESS)
		{
			trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::switchportVlanConfigStep:setLocationIdsForPoSubModeCmds returned error as this is an empty port-channel on all the locations."));

			pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			pContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
			delete send_ctx_p;

			return;
		}

		send_ctx_p->setPartialSuccessFlag(true);
		send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);
		pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

		return;
	}

	void IntfConfGlobalWorker::switchportVlanConfigMOStep(DceLinearSeqContext *pContext)
	{
		SwitchportVlanConfigGlobalMessage *pMsg = dynamic_cast<SwitchportVlanConfigGlobalMessage*> (pContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 vlanId = pMsg->getVlanId(), cmdCode = pMsg->getCmdCode(), poId = atoi(pMsg->getIfName().c_str());
		PoIntfManagedObject *pPoMO = NULL; 

		WaveManagedObjectSynchronousQueryContext poSyncQueryCtxt (PoIntfManagedObject::getClassName ());
		poSyncQueryCtxt.addAndAttribute (new AttributeUI32 (poId, "id"));
		poSyncQueryCtxt.addSelectField("objectId");
		vector<WaveManagedObject *> *pPoResults = querySynchronously (&poSyncQueryCtxt);

		if((pPoResults != NULL) && (pPoResults->size() != 0))
		{
			pContext->addManagedObjectsForGarbageCollection(*pPoResults);
			pPoMO =  dynamic_cast<PoIntfManagedObject*>((*pPoResults)[0]);
			pPoResults->clear();
			delete pPoResults;
			pPoResults = NULL;
		}

		if(pPoMO == NULL)
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::switchportVlanConfigMOStep: Interface does not exist."));

			status = WRC_NSM_ERR_PHYINTF_INVALID;
			pContext->executeNextStep(status);

			return;
		}

		switch(cmdCode)
		{
			case NSMUPDATE_PO_SET_MAC_ADDRESS:
			{
				MacAddressVlanClassificationMO *pMO = new MacAddressVlanClassificationMO (dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
				pContext->addManagedObjectForGarbageCollection(pMO);
				pMO->setOwnerManagedObjectId(pPoMO->getObjectId());
				pMO->setAccessVlanId(vlanId);
				pMO->setMac(pMsg->getMac());
				addToComposition(PoPhyIntfManagedObject::getClassName(), MacAddressVlanClassificationMO::getClassName(), "macAddressVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
			}
			break;

			case NSMUPDATE_PO_DEL_MAC_ADDRESS:
			{
				WaveManagedObjectSynchronousQueryContext accessPortSyncQueryCtxt (MacAddressVlanClassificationMO::getClassName ());
				accessPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (vlanId, "vlanId"));
				accessPortSyncQueryCtxt.addAndAttribute (new AttributeString (pMsg->getMac(), "mac"));
				accessPortSyncQueryCtxt.addAndAttribute (new AttributeObjectId (pPoMO->getObjectId(), "ownerManagedObjectId"));
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

			case NSMUPDATE_PO_SET_MAC_GROUP:
			{
				MacGroupVlanClassificationMO *pMO = new MacGroupVlanClassificationMO (dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
				pContext->addManagedObjectForGarbageCollection(pMO);
				pMO->setOwnerManagedObjectId(pPoMO->getObjectId());
				pMO->setVlanId(vlanId);
				pMO->setMacGroupId(pMsg->getMacGroupId());
				addToComposition(PoPhyIntfManagedObject::getClassName(), MacGroupVlanClassificationMO::getClassName(), "macGroupVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
			}
			break;

			case NSMUPDATE_PO_DEL_MAC_GROUP:
			{
				WaveManagedObjectSynchronousQueryContext macGroupQueryCtxt (MacGroupVlanClassificationMO::getClassName ());
				macGroupQueryCtxt.addAndAttribute (new AttributeUI32 (vlanId, "vlanId"));
				macGroupQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getMacGroupId(), "macGroupId"));
				macGroupQueryCtxt.addAndAttribute (new AttributeObjectId (pPoMO->getObjectId(), "ownerManagedObjectId"));
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

			case NSMUPDATE_PO_SET_CTAG:
			{
				CtagVlanClassificationMO *pMO = new CtagVlanClassificationMO (dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
				pContext->addManagedObjectForGarbageCollection(pMO);
				pMO->setOwnerManagedObjectId(pPoMO->getObjectId());
				pMO->setVlanId(vlanId);
				pMO->setCtagId(pMsg->getCtagId());
				addToComposition(PoPhyIntfManagedObject::getClassName(), CtagVlanClassificationMO::getClassName(), "ctagVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
			}
			break;

			case NSMUPDATE_PO_DEL_CTAG:
			{
				WaveManagedObjectSynchronousQueryContext trunkPortSyncQueryCtxt (CtagVlanClassificationMO::getClassName ());
				trunkPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (vlanId, "vlanId"));
				trunkPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getCtagId(), "ctagId"));
				trunkPortSyncQueryCtxt.addAndAttribute (new AttributeObjectId (pPoMO->getObjectId(), "ownerManagedObjectId"));
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

			case NSMUPDATE_PO_PVLAN_SET_CTAG:
			{
				CtagPVlanClassificationMO *pMO = new CtagPVlanClassificationMO (dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
				pContext->addManagedObjectForGarbageCollection(pMO);
				pMO->setOwnerManagedObjectId(pPoMO->getObjectId());
				pMO->setVlanId(vlanId);
				pMO->setCtagId(pMsg->getCtagId());
				addToComposition(PoPhyIntfManagedObject::getClassName(), CtagPVlanClassificationMO::getClassName(), "ctagPVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
			}
			break;

			case NSMUPDATE_PO_PVLAN_DEL_CTAG:
			{
				WaveManagedObjectSynchronousQueryContext trunkPortSyncQueryCtxt (CtagPVlanClassificationMO::getClassName ());
				trunkPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (vlanId, "vlanId"));
				trunkPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pMsg->getCtagId(), "ctagId"));
				trunkPortSyncQueryCtxt.addAndAttribute (new AttributeObjectId (pPoMO->getObjectId(), "ownerManagedObjectId"));
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
				trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::switchportVlanConfigMOStep: Invalid command has been entered."));

				status = WAVE_MESSAGE_ERROR;
				pContext->executeNextStep(status);

				return;
			}
			break;
		}

		pContext->executeNextStep(status);

		return;
	}

	void IntfConfGlobalWorker::macGroupConfigGlobalMessageHandler(MacGroupConfigGlobalMessage *pMacGroupConfigGlobalMessage)
	{
		PrismLinearSequencerStep sequencerSteps[] =
		{
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::validateMacGroupConfigStep),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::macGroupConfigStep),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::deleteMacGroupReferences),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::macGroupConfigMOStep),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
			reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
		};

		DceLinearSeqContext *pDceLinearSeqContext = new DceLinearSeqContext (pMacGroupConfigGlobalMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
		pDceLinearSeqContext->holdAll();
		pDceLinearSeqContext->start ();
	}

	void IntfConfGlobalWorker::validateMacGroupConfigStep(DceLinearSeqContext *pContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		if(ProvisioningUtil::isGvlanEnabled() == GVLAN_STATE_DISABLE)
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateMacGroupConfigStep:GVlan is not enabled."));

			status = WRC_NSM_GVLAN_NOT_CAPABLE_FABRIC;
			pContext->executeNextStep(status);

			return;
		}

		pContext->executeNextStep(status);

		return;
	}

	void IntfConfGlobalWorker::macGroupConfigStep(DceLinearSeqContext *pContext)
	{
		MacGroupConfigGlobalMessage *pMsg = dynamic_cast<MacGroupConfigGlobalMessage*> (pContext->getPPrismMessage ());
		MacGroupConfigLocalMessage *pLocalMsg = new MacGroupConfigLocalMessage();

		pLocalMsg->setCmdCode(pMsg->getCmdCode());
		pLocalMsg->setMacGroupId(pMsg->getMacGroupId());
		pLocalMsg->setMac(pMsg->getMac());
		pLocalMsg->setMask(pMsg->getMask());

		WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::IntfConfGlobalSendToClusterCallback), pContext);

		prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);

		send_ctx_p->setPPrismMessageForPhase1(pLocalMsg);
		send_ctx_p->setPartialSuccessFlag(true);
		send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);
		return;
	}

	void IntfConfGlobalWorker::macGroupConfigMOStep(DceLinearSeqContext *pContext)
	{
		MacGroupConfigGlobalMessage *pMsg = dynamic_cast<MacGroupConfigGlobalMessage*> (pContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 macGroupId = pMsg->getMacGroupId(), cmdCode = pMsg->getCmdCode();
		MacGroupManagedObject *pMacGroupMO = NULL;

		if((cmdCode == MAC_GROUP_SET_ENTRY) || (cmdCode == MAC_GROUP_DEL_ENTRY))
		{
			WaveManagedObjectSynchronousQueryContext macGroupSyncQueryCtxt (MacGroupManagedObject::getClassName ());
			macGroupSyncQueryCtxt.addAndAttribute (new AttributeUI32 (macGroupId, "id"));
			macGroupSyncQueryCtxt.addSelectField("objectId");
			vector<WaveManagedObject *> *pMacGroupResults = querySynchronously(&macGroupSyncQueryCtxt);

			if((pMacGroupResults != NULL) && (pMacGroupResults->size() != 0))
			{
				pContext->addManagedObjectsForGarbageCollection(*pMacGroupResults);
				pMacGroupMO =  dynamic_cast<MacGroupManagedObject*>((*pMacGroupResults)[0]);
				pMacGroupResults->clear();
				delete pMacGroupResults;
				pMacGroupResults = NULL;
			}
		}

		switch(cmdCode)
		{
			case MAC_GROUP_CREATE:
			{
				pMacGroupMO = new MacGroupManagedObject (dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
				pMacGroupMO->setMacGroupId(macGroupId);
			}
			break;

			case MAC_GROUP_DELETE:
			{
				WaveManagedObjectSynchronousQueryContextForDeletion deleteQueryCtxt(MacGroupManagedObject::getClassName());
				deleteQueryCtxt.addAndAttribute (new AttributeUI32 (macGroupId, "id"));
				deleteWaveManagedObject(&deleteQueryCtxt);
			}
			break;

			case MAC_GROUP_SET_ENTRY:
			{
				if(pMacGroupMO == NULL)
				{
					break;
				}

				MacGroupEntryManagedObject *pMacGroupEntryMO = new MacGroupEntryManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
				pContext->addManagedObjectForGarbageCollection(pMacGroupEntryMO);
				pMacGroupEntryMO->setOwnerManagedObjectId(pMacGroupMO->getObjectId());
				pMacGroupEntryMO->setAddress(pMsg->getMac());
				pMacGroupEntryMO->setMask(pMsg->getMask());
				addToComposition(MacGroupManagedObject::getClassName(), MacGroupEntryManagedObject::getClassName(), "entry", pMacGroupEntryMO->getOwnerManagedObjectId(), pMacGroupEntryMO->getObjectId());
			}
			break;

			case MAC_GROUP_DEL_ENTRY:
			{
				if(pMacGroupMO == NULL)
				{
					break;
				}

				WaveManagedObjectSynchronousQueryContext macGroupEntrySyncQueryCtxt (MacGroupEntryManagedObject::getClassName ());
				macGroupEntrySyncQueryCtxt.addAndAttribute (new AttributeString (pMsg->getMac(), "address"));
				macGroupEntrySyncQueryCtxt.addAndAttribute (new AttributeObjectId (pMacGroupMO->getObjectId(), "ownerManagedObjectId"));
				vector<WaveManagedObject *> *pMacGroupEntryResults = querySynchronously (&macGroupEntrySyncQueryCtxt);

				if((pMacGroupEntryResults != NULL) && (pMacGroupEntryResults->size() > 0))
				{
					pContext->addManagedObjectsForGarbageCollection(*pMacGroupEntryResults);
					MacGroupEntryManagedObject *pMacGroupEntryMO =  dynamic_cast<MacGroupEntryManagedObject*>((*pMacGroupEntryResults)[0]);
					deleteFromComposition (MacGroupManagedObject::getClassName(), MacGroupEntryManagedObject::getClassName(), "entry", pMacGroupEntryMO->getOwnerManagedObjectId(), pMacGroupEntryMO->getObjectId());

					pMacGroupEntryResults->clear();
					delete pMacGroupEntryResults;
				}
			}
			break;

			default:
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::switchportVlanConfigMOStep: Invalid command has been entered."));

				status = WAVE_MESSAGE_ERROR;
				pContext->executeNextStep(status);

				return;
			}
			break;
		}

		pContext->executeNextStep(status);

		return;
	}

	void IntfConfGlobalWorker::deleteVlanClassificationReferences(DceLinearSeqContext *pDceLinearSeqContext)
	{
		NsmGlobalDeleteVlanIntfMessage *pNsmGlobalDeleteVlanIntfMessage = dynamic_cast<NsmGlobalDeleteVlanIntfMessage *> (pDceLinearSeqContext->getPPrismMessage ());

		UI32 vlanId = pNsmGlobalDeleteVlanIntfMessage->getVlanId();
		clearVlanClassificationWithVlan(vlanId, pDceLinearSeqContext, PoPhyIntfManagedObject::getClassName(), MacAddressVlanClassificationMO::getClassName(), "macAddressVlanClassification");
		clearVlanClassificationWithVlan(vlanId, pDceLinearSeqContext, PoPhyIntfManagedObject::getClassName(), MacGroupVlanClassificationMO::getClassName(), "macGroupVlanClassification");
		clearVlanClassificationWithVlan(vlanId, pDceLinearSeqContext, PoPhyIntfManagedObject::getClassName(), CtagVlanClassificationMO::getClassName(), "ctagVlanClassification");
		clearVlanClassificationWithVlan(vlanId, pDceLinearSeqContext, PoPhyIntfManagedObject::getClassName(), CtagPVlanClassificationMO::getClassName(), "ctagPVlanClassification");
		clearVlanClassificationWithVlan(vlanId, pDceLinearSeqContext, PhyIntfLocalManagedObject::getClassName(), RspanMacAddressVlanClassificationMO::getClassName(), "rspan_macaddress_vlan_classififcation");
		clearVlanClassificationWithVlan(vlanId, pDceLinearSeqContext, PhyIntfLocalManagedObject::getClassName(), RspanMacGroupVlanClassificationMO::getClassName(), "rspan_macgroup_vlan_classififcation");
		clearVlanClassificationWithVlan(vlanId, pDceLinearSeqContext, PhyIntfLocalManagedObject::getClassName(), RspanGVlanTrunkPortManagedObject::getClassName(), "rspan_gvlan_trunk");
		pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

		return ;
	}

	void IntfConfGlobalWorker::clearVlanClassificationWithVlan(const UI32 &vlanId, DceLinearSeqContext *pDceLinearSeqContext, const string &parentMoName, const string &childMoName, const string &columnName)
	{
		vector<string> selectFields;
		selectFields.push_back("objectId");
		selectFields.push_back("ownerManagedObjectId");

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(childMoName);
		syncQueryCtxt.addOrAttribute(new AttributeUI32(vlanId, "vlanId"));
		syncQueryCtxt.addSelectFields(selectFields);
		vector<WaveManagedObject * >* pMOResults = querySynchronously(&syncQueryCtxt);

		if((pMOResults != NULL) && (pMOResults->size()))
		{
			for(UI32 moIndex = 0; moIndex < pMOResults->size(); moIndex++)
			{
				WaveManagedObject *pMO = dynamic_cast<WaveManagedObject *>((*pMOResults)[moIndex]);
				deleteFromComposition(parentMoName, childMoName, columnName, pMO->getOwnerManagedObjectId(), pMO->getObjectId());
			}

			pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*pMOResults);
		}

		return;
	}

	void IntfConfGlobalWorker::deleteMacGroupReferences(DceLinearSeqContext *pContext)
	{
		MacGroupConfigGlobalMessage *pMsg = dynamic_cast<MacGroupConfigGlobalMessage*> (pContext->getPPrismMessage ());
		UI32 macGroupId = pMsg->getMacGroupId(), cmdCode = pMsg->getCmdCode();

		if(cmdCode != MAC_GROUP_DELETE)
		{
			pContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

			return;
		}

		string childMOName = MacGroupVlanClassificationMO::getClassName();
		string parentMOName = PoPhyIntfManagedObject::getClassName();
		vector<string> selectFields;
		selectFields.push_back("objectId");
		selectFields.push_back("ownerManagedObjectId");

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(childMOName);
		syncQueryCtxt.addAndAttribute(new AttributeUI32(macGroupId, "macGroupId"));
		syncQueryCtxt.addSelectFields(selectFields);
		vector<WaveManagedObject * >* pMOResults = querySynchronously(&syncQueryCtxt);

		if((pMOResults != NULL) && (pMOResults->size()))
		{
			for(UI32 moIndex = 0; moIndex < pMOResults->size(); moIndex++)
			{
				WaveManagedObject *pMO = dynamic_cast<WaveManagedObject *>((*pMOResults)[moIndex]);
				deleteFromComposition(parentMOName, childMOName, "macGroupVlanClassification", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
			}

			pContext->addManagedObjectsForGarbageCollection(*pMOResults);
		}

		pContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

		return;
	}

	void IntfConfGlobalWorker::gVlanEnableGlobalMessageHandler(GVlanEnableGlobalMessage *pGlobalMsg)
	{
		if (pGlobalMsg->getUndo())
		{
			PrismLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::enableGVlanStg4),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::enableGVlanStg3),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::enableGVlanStg2),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::enableGVlanStg1),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
			};

			DceLinearSeqContext *pDceLinearSeqContext = new DceLinearSeqContext (pGlobalMsg, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
			pDceLinearSeqContext->holdAll();
			pDceLinearSeqContext->start ();
		}
		else
		{
			PrismLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::validateGVlanEnableStg0),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::enableGVlanStg1),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::enableGVlanStg2),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::enableGVlanStg3),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::enableGVlanStg4),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerStartTransactionStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::updateGVlanConfigMOStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerCommitTransactionStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerSucceededStep),
				reinterpret_cast<PrismLinearSequencerStep>(&IntfConfGlobalWorker::prismLinearSequencerFailedStep)
			};

			DceLinearSeqContext *pDceLinearSeqContext = new DceLinearSeqContext (pGlobalMsg, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
			pDceLinearSeqContext->holdAll();
			pDceLinearSeqContext->start ();
		}
	}

	void IntfConfGlobalWorker::GVlanEnableSendToClusterCallback (WaveSendToClusterContext  *pClusterContext)
	{
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pClusterContext);
		PrismLinearSequencerContext *pContext = reinterpret_cast<PrismLinearSequencerContext *>(pClusterContext->getPCallerContext());

		trace (TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::GVlanEnableSendToClusterCallback:status:") + FrameworkToolKit::localize(sendToClusterCompletionStatus));

		if(sendToClusterCompletionStatus != WAVE_MESSAGE_SUCCESS)
		{
			GVlanEnableLocalMessage *pLocalMsg = reinterpret_cast<GVlanEnableLocalMessage *>(pClusterContext->getPPrismMessageForPhase1());
			GVlanEnableGlobalMessage *pGlobalMsg = dynamic_cast<GVlanEnableGlobalMessage *> (pContext->getPPrismMessage());
			gvlan_enable_stage_t failedStage = pLocalMsg->getCmdCode();
			pGlobalMsg->setFailedStage(failedStage);

			trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::GVlanEnableSendToClusterCallback:failedStage in Callback:") + pLocalMsg->getCmdCode());

			if(pGlobalMsg->getUndo())
			{
				trace(TRACE_LEVEL_FATAL, string("IntfConfGlobalWorker::GVlanEnableSendToClusterCallback:failed in Undo operation !!"));
			}
		}

		delete pClusterContext->getPPrismMessageForPhase1();
		delete pClusterContext->getPPrismMessageForPhase2();
		delete pClusterContext;
		pContext->executeNextStep(sendToClusterCompletionStatus); 
  
		return;
	}

	void IntfConfGlobalWorker::sendGVlanEnableCmdToLocalWorker(DceLinearSeqContext *pContext, const gvlan_enable_stage_t &cmdCode)
	{
		trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::sendGVlanEnableCmdToLocalWorker:Entered."));

		GVlanEnableGlobalMessage *pGlobalMsg = dynamic_cast<GVlanEnableGlobalMessage *> (pContext->getPPrismMessage());

		// When the Undo flag is set and the cmdCode is less then the failedStage, then skip sending the message to local worker.
		if(pGlobalMsg->getUndo() && (pGlobalMsg->getFailedStage() < cmdCode))
		{
			pGlobalMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			pContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

			return;
		}

		GVlanEnableLocalMessage *pLocalMsg = new GVlanEnableLocalMessage();
		pLocalMsg->setCmdCode(cmdCode);
		pLocalMsg->setUndo(pGlobalMsg->getUndo());
		pLocalMsg->setEnable(pGlobalMsg->getEnable());

		WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&IntfConfGlobalWorker::GVlanEnableSendToClusterCallback), pContext);
		send_ctx_p->setPPrismMessageForPhase1(pLocalMsg);
		send_ctx_p->setPartialSuccessFlag(true);
		send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);

		return;
	}

	void IntfConfGlobalWorker::validateGVlanEnableStg0(DceLinearSeqContext *pContext)
	{
		trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::validateGVlanEnableStg0:Entered."));

		ResourceId status = WAVE_MESSAGE_SUCCESS;
		GVlanEnableGlobalMessage *pGlobalMsg = dynamic_cast<GVlanEnableGlobalMessage *> (pContext->getPPrismMessage ());

		if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT != DcmToolKit::getThisVcsNodeClusterType ())
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateGVlanEnableStg0:GVlan can be enabled only in MC."));

			status = WRC_NSM_GVLAN_SUPPORTED_ONLY_IN_MC;
		}
		else if(!DcmToolKit::isClusterGvlanCapable())
		{
			trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateGVlanEnableStg0:This fabric is not capable of supporting GVlan configuration."));

			status = WRC_NSM_GVLAN_NOT_CAPABLE_FABRIC;
		}

		if(status != WAVE_MESSAGE_SUCCESS)
		{
			pGlobalMsg->setFailedStage(GVLAN_ENABLE_VLD_0);
			pGlobalMsg->setCompletionStatus(status);
			pContext->executeNextStep(status);

			return;
		}

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

			trace (TRACE_LEVEL_INFO, string ("IntfConfGlobalWorker::validateGVlanEnableStg0:Current Config value of GVlan is:") + configValue);
		}
		else
		{
			trace (TRACE_LEVEL_FATAL, string ("IntfConfGlobalWorker::validateGVlanEnableStg0:Current GVlan config value is not found !"));
		}

		//When the GVlan is already enabled and the user is trying to enable it again, we should return error.
		//Same is the case with GVlan disable.
	   	if(pGlobalMsg->getEnable())
		{
			if(foundConfig && (gVlanState != GVLAN_STATE_DISABLE))
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateGVlanEnableStg0:GVlan is already enabled."));

				status = WRC_NSM_GVLAN_ALREADY_ENABLED;
			}
		}
		else
		{
			if(foundConfig && (gVlanState == GVLAN_STATE_DISABLE))
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateGVlanEnableStg0:GVlan is already disabled."));

				status = WRC_NSM_GVLAN_ALREADY_DISABLED;
			}
			else if(NsmUtils::isGVlanConfigPresent())
			{
				trace(TRACE_LEVEL_ERROR, string("IntfConfGlobalWorker::validateGVlanEnableStg0:GVlanConfig is present."));

				status = WRC_NSM_GVLAN_CONFIGURATION_EXISTS;
			}
		}

		if(status != WAVE_MESSAGE_SUCCESS)
		{
			pGlobalMsg->setFailedStage(GVLAN_ENABLE_VLD_0);
		}

		pGlobalMsg->setCompletionStatus(status);
		pContext->executeNextStep(status);

		return;
	}

	void IntfConfGlobalWorker::enableGVlanStg4(DceLinearSeqContext *pContext)
	{
		trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::enableGVlanStg4:Entered."));

		sendGVlanEnableCmdToLocalWorker(pContext, GVLAN_ENABLE_STG_4);

		return;
	}

	void IntfConfGlobalWorker::enableGVlanStg1(DceLinearSeqContext *pContext)
	{
		trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::enableGVlanStg1:Entered."));

		sendGVlanEnableCmdToLocalWorker(pContext, GVLAN_ENABLE_STG_1);

		return;
	}

	void IntfConfGlobalWorker::enableGVlanStg2(DceLinearSeqContext *pContext)
	{
		trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::enableGVlanStg2:Entered."));

		sendGVlanEnableCmdToLocalWorker(pContext, GVLAN_ENABLE_STG_2);

		return;
	}

	void IntfConfGlobalWorker::enableGVlanStg3(DceLinearSeqContext *pContext)
	{
		trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::enableGVlanStg3:Entered."));

		sendGVlanEnableCmdToLocalWorker(pContext, GVLAN_ENABLE_STG_3);

		return;
	}

	void IntfConfGlobalWorker::updateGVlanConfigMOStep(DceLinearSeqContext *pContext)
	{
		GVlanEnableGlobalMessage *pGlobalMsg = dynamic_cast<GVlanEnableGlobalMessage *> (pContext->getPPrismMessage ());
		UI32 gVlanState = GVLAN_STATE_DISABLE;

		if(pGlobalMsg->getEnable())
		{
			gVlanState = GVLAN_STATE_ENABLE;
		}

		Attribute *configAttribute = new AttributeUI32(&gVlanState, NSM_GVLAN_PERSISTENCE_MO_NAME);
		updateWaveConfigEntry(configAttribute);
		delete configAttribute;

		trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::updateGVlanConfigMOStep:Updated the GVlan config state with value:") + gVlanState);

		sendGVlanEnableCmdToLocalWorker(pContext, GVLAN_ENABLE_STG_5);

		return;
	}

	void IntfConfGlobalWorker::upgrade(WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase)
	{
		UI32 gVlanState = GVLAN_STATE_DISABLE;
		Attribute *readConfigAttr = new AttributeUI32(&gVlanState, NSM_GVLAN_PERSISTENCE_MO_NAME);
		bool foundConfig = false;
		string configName(NSM_GVLAN_PERSISTENCE_MO_NAME);
		getWaveConfigEntry(configName, readConfigAttr, foundConfig);
		delete readConfigAttr;
	    ResourceId status = WAVE_MESSAGE_SUCCESS;

		if(!foundConfig)
		{
            startTransaction ();
			gVlanState = GVLAN_STATE_DISABLE;
			Attribute *configAttr = new AttributeUI32(&gVlanState, NSM_GVLAN_PERSISTENCE_MO_NAME);
			addWaveConfigEntry(configAttr);
            status = commitTransaction();

			if(status != FRAMEWORK_SUCCESS)
		   	{
                status = WAVE_MESSAGE_ERROR;

				trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::install:Adding GVlan config failed."));
            }
		   	else
		   	{
                status = WAVE_MESSAGE_SUCCESS;

				trace(TRACE_LEVEL_INFO, string("IntfConfGlobalWorker::install:Added GVlan config state with value:") + gVlanState);
            }
			delete configAttr;
		}
        pWaveAsynchronousContextForUpgradePhase->setCompletionStatus (status);
        pWaveAsynchronousContextForUpgradePhase->callback ();
	}
}
