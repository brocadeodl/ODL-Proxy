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

#ifndef INTFCONFLOCALWORKER_H
#define INTFCONFLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/LacpLocalShowMessage.h"
#include "Nsm/Local/NsmLocalEcmpConfigMessage.h"
#include "Nsm/Local/NsmLocalShowEcmpMessage.h"
#include "Nsm/Local/Dot1XLocalShowMessage.h"
#include "Nsm/Local/Dot1XLocalReauthenticateMessage.h"
#include "Nsm/Local/Dot1XLocalTestMessage.h"
#include "Nsm/Local/Dot1XLocalClearMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/NativeVlanIntfConfLocalManagedObject.h"
#include "Nsm/Local/NsmLocalPolicyMapMessage.h"
#include "Nsm/Local/PolicerLocalShowMessage.h"
#include "Nsm/Local/PolicerLocalClearMessage.h"
#include "ssmAcl_dcmd_public.h"
#include "Utils/DceSynchronousLinearSeqContext.h"
#include "Nsm/Local/NsmLocalSlotOfflineMessage.h"
#include "Nsm/Local/InterfaceStateReplayServiceIndependentMessage.h"
#include "Nsm/Local/NsmLocalProvisioningInfoMessage.h"


#define CMDCODE_BASED_PROVISION(context, cmdCode, vlanRange) { \
            vlan_provision_code provisionCode = mapCmdCodeToProvisionCode( \
                    cmdCode); \
            if (PROVISION_NOT_USED != provisionCode) { \
                PROVISION_VLAN_RANGE(context, vlanRange, provisionCode); \
            } \
        }

#define PORT_BASED_PROVISION(context, cmdCode, portProfiled) { \
            ResourceId portBasedProvisioningStatus = \
                provisionRequiredVlansForPort(context, cmdCode, portProfiled); \
            if (WAVE_MESSAGE_SUCCESS != portBasedProvisioningStatus) { \
                return portBasedProvisioningStatus; \
            } \
        }



using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmCreatePhyIntfMessage;
    class NsmUpdatePhyIntfMessage;
    class NsmDeletePhyIntfMessage;
    class NsmAddPhyIntfToVlanIntfMessage;
    class NsmDeletePhyIntfToVlanIntfMessage;
    class NsmCreateVlanIntfSPMessage;
    class NsmUpdateVlanIntfSPMessage;
    class NsmDeleteVlanIntfSPMessage;
    class NsmCreatePoIntfSPMessage;
    class NsmUpdatePoIntfSPMessage;
    class NsmDeletePoIntfSPMessage;
    class NsmAddPoToVlanIntfSPMessage;
    class NsmDeletePoToVlanIntfSPMessage;
    class NsmAddPhyIntfToPoIntfMessage;
    class NsmDeletePhyIntfToPoIntfMessage;
    class NsmUpdateLacpConfigSPMessage;
    class NsmUpdateDot1XConfigSPMessage;
    class NsmAssocMacAclPolicyPoVlanIntfSPMessage;
    class NsmAssocMacAclPolicyPhyIntfMessage;
    class NsmAssocIpAclPolicyPoVlanIntfSPMessage;
    class NsmAssocIpAclPolicyPhyIntfMessage;
	class PBRVeIntfMessage;
	class PBRPhyIntfMessage;
	class PBRPoVlanIntfSPMessage;
	class IpPolicyDeletePBRMessage;
    class NsmLocalFcoeVlanMessage;
    class NsmLocalReservedVlanMessage;
	class NsmLocalConfigQosAttributesMessage;
	class NsmQosPOConfigSPMessage;
	class NsmEventMessage;
	class NsmLocalShowIntfMessage;
	class NsmLocalPortMediaShowIntfMessage;
	class NsmLocalDebugMessage;
    class NsmUpdatePoPrimaryLinkMessage;
    class NsmUpdateBulkPhyIntfMessage;
	class NsmLocalClearMessage;
    class LacpLocalClearMessage;
	class NsmLocalEcmpConfigMesage;
	class NsmLocalShowEcmpMessage;
	class NsmLocalShowVlagMessage;
    class NsmLocalMacRebalanceMessage;
	class NsmLocalVlagConfigMessage;
	class NsmStormControlIntfSetMessage;
	class NsmLocaldestinationMirrorportMessage;
	class NsmLocalAssocIpAclPolicyVeIntfMessage;
    class DcmManagementInterfaceMessage;
	class ConnectorConfigLocalMessage;
	class SwitchportVlanConfigLocalMessage;
	class MacGroupConfigLocalMessage;
	class GVlanEnableLocalMessage;

    class IntfConfLocalWorker: public WaveWorker
    {
        private:
            static vlan_provision_code mapCmdCodeToProvisionCode(const UI32 &cmdCode);
            ResourceId getAllVlansToBeProvisioned(vector<string>  &portProfiles, string &portProfileDomainName,
                    UI32Range &toBeProvisionedVlanRange);
            ResourceId getAllCtagVlans(UI32Range &allCtagVlans);
            ResourceId provisionRequiredVlansForPort(DceContext *context,
                const UI32 cmdCode, const bool profiledPort);
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmAddPhyIntfToPoIntfMessageHandler( NsmAddPhyIntfToPoIntfMessage *pNsmAddPhyIntfToPoIntfMessage);
            void NsmAddPhyIntfToVlanIntfMessageHandler( NsmAddPhyIntfToVlanIntfMessage *pNsmAddPhyIntfToVlanIntfMessage);
            void NsmAddPoToVlanIntfSPMessageHandler( NsmAddPoToVlanIntfSPMessage *pNsmAddPoToVlanIntfSPMessage);
            void NsmCreatePhyIntfMessageHandler( NsmCreatePhyIntfMessage *pNsmCreatePhyIntfMessage);
            void NsmCreatePoIntfSPMessageHandler( NsmCreatePoIntfSPMessage *pNsmCreatePoIntfSPMessage);
            void NsmCreateVlanIntfSPMessageHandler( NsmCreateVlanIntfSPMessage *pNsmCreateVlanIntfSPMessage);
            void NsmDeletePhyIntfMessageHandler( NsmDeletePhyIntfMessage *pNsmDeletePhyIntfMessage);
            void NsmDeletePhyIntfToPoIntfMessageHandler( NsmDeletePhyIntfToPoIntfMessage *pNsmDeletePhyIntfToPoIntfMessage);
            void NsmDeletePhyIntfToVlanIntfMessageHandler( NsmDeletePhyIntfToVlanIntfMessage *pNsmDeletePhyIntfToVlanIntfMessage);
            void NsmDeletePoIntfSPMessageHandler( NsmDeletePoIntfSPMessage *pNsmDeletePoIntfSPMessage);
            void NsmDeletePoToVlanIntfSPMessageHandler( NsmDeletePoToVlanIntfSPMessage *pNsmDeletePoToVlanIntfSPMessage);
            void NsmDeleteVlanIntfSPMessageHandler( NsmDeleteVlanIntfSPMessage *pNsmDeleteVlanIntfSPMessage);
            void NsmUpdatePhyIntfMessageHandler( NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage);
            void NsmUpdateBulkPhyIntfMessageHandler( NsmUpdateBulkPhyIntfMessage *pNsmUpdateBulkPhyIntfMessage);
            void NsmUpdatePoIntfSPMessageHandler( NsmUpdatePoIntfSPMessage *pNsmUpdatePoIntfSPMessage);
            void NsmUpdateVlanIntfSPMessageHandler( NsmUpdateVlanIntfSPMessage *pNsmUpdateVlanIntfSPMessage);
            void NsmUpdateLacpConfigSPMessageHandler( NsmUpdateLacpConfigSPMessage *pNsmUpdateLacpConfigSPMessage);
            void NsmUpdateDot1XConfigSPMessageHandler( NsmUpdateDot1XConfigSPMessage *pNsmUpdateDot1XConfigSPMessage);
            void NsmLocalFcoeVlanMessageHandler (NsmLocalFcoeVlanMessage *msg_p);
            void NsmLocalReservedVlanMessageHandler (NsmLocalReservedVlanMessage *msg_p);
			void NsmLocalConfigQosAttributesMessageHandler (NsmLocalConfigQosAttributesMessage *pNsmLocalConfigQosAttributesMessage);
			void NsmQosPOConfigSPMessageHandler (NsmQosPOConfigSPMessage *pNsmQosPOConfigSPMessage);
            void NsmUpdatePoPrimaryLinkMessageHandler( NsmUpdatePoPrimaryLinkMessage *pNsmUpdatePoPrimaryLinkMessage);
			void NsmLocalEcmpConfigMessageHandler(NsmLocalEcmpConfigMessage *pNsmLocalEcmpConfigMessage);
   			void NsmLocalShowEcmpMessageHandler(NsmLocalShowEcmpMessage *pNsmLocalShowEcmpMessage);
            void NsmLocalPolicyMapMessageHandler(NsmLocalPolicyMapMessage *pNsmLocalPolicyMapMessage); 
			ResourceId sendShowToPolicerDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void sendPolicerShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            void PolicerLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            void PolicerLocalShowMessageHandler (PolicerLocalShowMessage *pPolicerLocalShowMessage);
			ResourceId sendClearToPolicerDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void sendPolicerClearCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            void PolicerLocalClearSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            void PolicerLocalClearMessageHandler (PolicerLocalClearMessage *pPolicerLocalClearMessage);
            void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

            ResourceId updatePoPrimaryLinkStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            /* Code Starts Here */

            /* Port-Channel Interface */
            ResourceId createPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePoIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId deletePoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId validatePoIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            
            /* Vlan Interface */
            ResourceId createVlanIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId deleteVlanIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId updateVlanIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteVlanIntfXstpStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId createBulkVlanIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            /* Physical Interface */
            ResourceId backendSyncUpConfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContex);
            ResourceId defaultConfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContex);
            ResourceId createPhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId deletePhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            ResourceId validatePhyIntfStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId updatePhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId updatePhyIntf(NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage, DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);

            ResourceId createPhyIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId deletePhyIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePhyIntfMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId updateBulkPhyIntfMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId updateBulkPhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
			ResourceId updatePhyIntfMO(NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage, DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);

			ResourceId getPhyIntfLocalMOByCmdCode(string PhyName, UI32 cmdCode, PhyIntfLocalManagedObject* &pPhyIntfLocalManagedObject, DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
			ResourceId postPhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
			ResourceId postPhyIntf(NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage,
									DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);

			ResourceId deletePhyIntfVlanReference(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
			void removeVlanClassifierOnPhy(const ObjectId &vlanObjectId);
			ResourceId getPhyMoByPhyName(string phyName, PhyIntfLocalManagedObject **ppPhyIntfLocalManagedObject);
    		NativeVlanIntfConfLocalManagedObject* NativeVlanPhyGetChildMoByParentMo(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			int deleteNativeVlanFromMoVlanList (NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage, 
													PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			int deleteNativeVlanFromMsgVlanList (NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage, 
												PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);

			ResourceId addPolicyMapToIntfStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            ResourceId addPolicyMapToIntfMOStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ObjectId getObjectIdFromPolicyMapName(string name, PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            ResourceId fcoeDelPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            /* Add Physical Interface To Port-Channel Interface*/
            ResourceId fcoeAddPhyIntfToPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId addPhyIntfToPoIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId deletePhyIntfToPoIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId deletePhyIntfToPoIntfMO(NsmDeletePhyIntfToPoIntfMessage *pNsmDeletePhyIntfToPoIntfMessage,PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId deletePhyIntfPoReference(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            void deletePoReferenceInPhyIntfMo (const string &poId, PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            ResourceId validatePhyIntfToPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId fcoeDelPhyIntfToPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId addPhyIntfToPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId deletePhyIntfToPoIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            ResourceId getPoIntfObjectIdByPoIfName (string poName, ObjectId &poIntf_oid,bool removeImmediate,
										PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId getPhyIntfObjectIdByIfName (string ifName, ObjectId &intfOid,
										PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId triggerPostBootProgramming (const UI32 eventid, const UI32 parameter);
			ResourceId configReplayIntfAdminState (const UI32 parameter);
            void       NsmEventMessageHandler(NsmEventMessage *pNsmEventMessage);
            
            /*Ecmp Load Balancing*/
            ResourceId updateEcmpConfigMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId sendEcmpConfigToNsmStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            /*Show Ecmp Load Balancing */
            ResourceId sendNsmShowEcmpCommandToNsmDaemon(PrismLinearSequencerContext *seq_ctx_p);
            void sendNsmShowEcmpCommandToCluster(PrismLinearSequencerContext *seq_ctx_p);
            void NsmLocalShowEcmpSendToClusterCallback(WaveSendToClusterContext *ctx_p);
            
			/* Lacp */
			ResourceId updateLacpConfigSPStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId sendShowToLacpDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void sendLacpShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            void LacpLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
			void LacpLocalShowMessageHandler (LacpLocalShowMessage *pLacpLocalShowMessage);
						
            /*Vlag Load Balancing */
            void NsmLocalVlagConfigMessageHandler(NsmLocalVlagConfigMessage *pNsmLocalVlagConfigMessage);
            ResourceId sendVlagConfigToNsmStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateVlagConfigMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            void NsmLocalShowVlagMessageHandler(NsmLocalShowVlagMessage *pNsmLocalShowVlagMessage);
            ResourceId sendNsmShowVlagCommandToNsmDaemon(PrismLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			/* Dot1x */
			ResourceId updateDot1XConfigSPStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId sendShowToDot1XDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void sendDot1XShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            void Dot1XLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
			void Dot1XLocalShowMessageHandler (Dot1XLocalShowMessage *pDot1XLocalShowMessage);
		    ResourceId sendReauthenticateToDot1XDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void sendDot1XReauthenticateCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            void Dot1XLocalReauthenticateSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            void Dot1XLocalReauthenticateMessageHandler (Dot1XLocalReauthenticateMessage *pDot1XLocalReauthenticateMessage);
		    ResourceId sendTestToDot1XDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void sendDot1XTestCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            void Dot1XLocalTestSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            void Dot1XLocalTestMessageHandler (Dot1XLocalTestMessage *pDot1XLocalTestMessage);
			ResourceId sendClearToDot1XDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void sendDot1XClearCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            void Dot1XLocalClearSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            void Dot1XLocalClearMessageHandler (Dot1XLocalClearMessage *pDot1XLocalClearMessage);

			void
            LacpLocalClearMessageHandler (LacpLocalClearMessage *pLacpLocalClearMessage);
            void 
            LacpLocalClearSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            void
            sendLacpClearCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            ResourceId
            sendClearToLacpDaemon (PrismSynchronousLinearSequencerContext *ctx_p);

			/* SPAN Mirrored Port */
			void NsmLocaldestinationMirrorportMessageHandler(NsmLocaldestinationMirrorportMessage *pNsmLocaldestinationMirrorportMessage);
			/* Storm Control */
			void NsmStormControlIntfSetMessageHandler(NsmStormControlIntfSetMessage *pNsmStormControlIntfSetMessage);
			
			ResourceId  validateStormControlPhyIntfConf(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updateStormControlPhyIntfConf(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updateStormControlPhyIntfMO(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId configureBumPhyIntfMOStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);                                             
			ResourceId unconfigureBumPhyIntfMOStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
        
			/* PBR */

			ResourceId deletePBRLocal (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			void NsmLocalPBRVeIntfMessageHandler(PBRVeIntfMessage *PBRVeIntfMessage);
			void NsmPBRDeleteMessageHandler (IpPolicyDeletePBRMessage *pMsg);
			void NsmPBRPhyIntfMessageHandler (PBRPhyIntfMessage *pPBRPhyIntfMessage);
			void NsmPBRPoVlanIntfSPMessageHandler (PBRPoVlanIntfSPMessage *pMsg);
			ResourceId validatePBRVeIntfStep(PrismSynchronousLinearSequencerContext *pLinearSeqContext);
			ResourceId configurePBRVeIntfStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updateVeIntfPBRStep(PrismSynchronousLinearSequencerContext *pLinearSeqContext);
			int getRouteMapNameForInterface(const UI32 ifType, const string ifName, string &routeMapName);
			ResourceId validatePBRPhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updatePBRPhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updatePBRPhyIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId assignPBRPhyIntfMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId removePBRPhyIntfMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId sendPBRConfigPoIntf (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            /* L3 ACL */

            ResourceId	updateIpAclConfigPoIntf (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            void       	NsmAssocIpAclPolicyPoVlanIntfSPMessageHandler (NsmAssocIpAclPolicyPoVlanIntfSPMessage *pMsg);
            ResourceId 	updateIpAclPhyIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId 	updateIpAclPhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            void       	NsmAssocIpAclPolicyPhyIntfMessageHandler (NsmAssocIpAclPolicyPhyIntfMessage *pNsmAssocIpAclPolicyPhyIntfMessage);
            ResourceId 	removeIpAclPhyIntfMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId 	assignIpAclPhyIntfMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            int 		getIpAclPolicyNameForInterface(const UI32 ifType, const string ifName, string &policyName, const ssmAcl_binding_direction direction);
            ResourceId 	validateIpAclPhyIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			void NsmLocalAssocIpAclPolicyVeIntfMessageHandler (NsmLocalAssocIpAclPolicyVeIntfMessage *pNsmLocalAssocIpAclPolicyVeIntfMessage);
			ResourceId validateIpAclToVeIntfStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId configureIpAclToVeIntfStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updateVeIntfIpAclStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);

			/* L2 ACL */
			ResourceId  updateMacAclConfigPoVlanIntf(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			void       NsmAssocMacAclPolicyPoVlanIntfSPMessageHandler (NsmAssocMacAclPolicyPoVlanIntfSPMessage *pMsg);
			ResourceId updateMacAclPhyIntfMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
			ResourceId updateMacAclPhyIntfStep  (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			void       NsmAssocMacAclPolicyPhyIntfMessageHandler (NsmAssocMacAclPolicyPhyIntfMessage *pNsmAssocMacAclPolicyPhyIntfMessage);
			ResourceId removeMacAclPhyIntfMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId assignMacAclPhyIntfMOStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
			int 		getMacAclPolicyNameForInterface(const UI32 ifType, const string ifName, string &policyName, const ssmAcl_binding_direction direction);
			ResourceId validateMacAclPhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);

            ResourceId NsmLocalReservedVlanSendToDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            /* Fcoe */
            ResourceId NsmLocalFcoeVlanSendToDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            ObjectId   FcoeMapGetObjectIdByName (string &map_name);

            /* sw access/trunk */
            ResourceId addPhyIntfToVlanIntfMoStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId getVlanIntfObjectIdByVlanIfName (string vlanName, ObjectId &poIntf_oid, bool removeImmediate,
									PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

			/*Qos */
			ResourceId updateQosPhyIntfValidationStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updateQosPhyIntfStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updatePhyIntQosMOStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updateQosPoIntfStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId getCosMapObjectIdByCosMapName(string cosMapName, ObjectId &cosMapObjectId,
										   PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId getTcMapObjectIdByTcMapName(string cosMapName, ObjectId &cosTcObjectId,
                                           PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
		    ResourceId getCeeMapObjectIdByCeeMapName(string ceeMapName, ObjectId &ceeMapObjectId,
                                           PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId vcsQosSanity(SI32 oper, string mapName, int defaultCos, bool apply, PhyIntfLocalManagedObject *p_intf, int cosMutationBitmap, PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

			ResourceId getDscpMapObjectIdByDscpMapName(string dscpMapName, ObjectId &dscpMapObjectId,
                                           PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId getDscpTcMapObjectIdByTcMapName(string dscpMapName, ObjectId &dscpTcObjectId,
                                           PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId getDscpCosMapObjectIdByDscpCosMapName(string dscpMapName, ObjectId &dscpMapObjectId,
                                           PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            void sendPoConfigToLocal (const string& ifName, LocationId& locationId);
			void NsmLocalShowSendToClusterCallback(WaveSendToClusterContext *ctx_p);
			ResourceId sendNsmIntfShowCommandToNsmDaemon(PrismLinearSequencerContext *seq_ctx_p);
			void  sendNsmIntfShowCommandToCluster(PrismLinearSequencerContext *seq_ctx_p);
			void NsmLocalShowMessageHandler(NsmLocalShowIntfMessage* pNsmLocalShowIntfMessage);
			void NsmLocalPortMediaShowSendToClusterCallback(WaveSendToClusterContext *ctx_p);
			ResourceId sendNsmIntfPortMediaShowCommandToNsmDaemon(PrismLinearSequencerContext *seq_ctx_p);
			void  sendNsmIntfPortMediaShowCommandToCluster(PrismLinearSequencerContext *seq_ctx_p);
			void NsmLocalPortMediaShowMessageHandler(NsmLocalPortMediaShowIntfMessage* pNsmLocalPortMediaShowIntfMessage);

			void NsmLocalDebugSendToClusterCallback(WaveSendToClusterContext *ctx_p);
			ResourceId sendNsmDebugCommandToNsmDaemon(PrismLinearSequencerContext *seq_ctx_p);
			void  sendNsmDebugCommandToCluster(PrismLinearSequencerContext *seq_ctx_p);
			void NsmLocalDebugMessageHandler(NsmLocalDebugMessage* pNsmLocalDebugMessage);
            ResourceId sendPoConfigToLocal (const string& ifName);
			void NsmLocalClearMessageHandler(NsmLocalClearMessage* pNsmLocalClearMessageHandler);
			void sendNsmClearCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
			void NsmLocalClearSendToClusterCallback (WaveSendToClusterContext *ctx_p);
			ResourceId sendClearToNsmDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
	
			/* Lldp */
			ResourceId updateLldpIntfDescription(NsmUpdatePhyIntfMessage *pNsmUpdatePhyIntfMessage);

            void NsmLocalMacRebalanceMessageHandler(NsmLocalMacRebalanceMessage *pMacRebalanceMsg);
            ResourceId sendMacRebalanceCommandToNsm(PrismSynchronousLinearSequencerContext *ctx_p);
            /* Code Ends Here */

            void sendMessageToClient (const string &name, DcmManagementInterfaceMessage *pMessage, ResourceId &errStatus);
			void connectorConfigMessageHandler (ConnectorConfigLocalMessage *pConnectorConfigMessage);
			ResourceId validateConnectorConfigStep (DceSynchronousLinearSeqContext *pContext);
			ResourceId validateBreakoutInterfacesConfig (const string &connectorName, DceSynchronousLinearSeqContext *pContext);
			ResourceId connectorConfigStep (DceSynchronousLinearSeqContext *pContext);
			ResourceId connectorConfigMOStep (DceSynchronousLinearSeqContext *pContext);
			ResourceId deleteConnectorPhyIntfMO (string connectorName, AttributeConditionOperator attributeConditionOperator);
			ResourceId validatePoId(UI32 poId);
			ResourceId clearSpanConnectorConfigStep (DceSynchronousLinearSeqContext *pContext);
			ResourceId clearL2sysConnectorConfigStep (DceSynchronousLinearSeqContext *pContext);
			ResourceId clearFcoeConnectorConfigStep (DceSynchronousLinearSeqContext *pContext);
			ResourceId clearStpPortIdConfigStep (DceSynchronousLinearSeqContext *pContext);
            ResourceId updateBulkConfigsToConnectorMO (const vector<string> &bulkConfigs, DceSynchronousLinearSeqContext *pContext);
            ResourceId updatePhyIntfMOWhenSlotOffline (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            void NsmLocalSlotOfflineMessageHandler (NsmLocalSlotOfflineMessage *pNsmLocalSlotOfflineMessage);
            void interfaceStateReplay (UI32 slotId);
            void cleanupPOReplayedSetWhenSlotReplayDone (UI32 rbridgeId, UI32 slotId);
			void switchportVlanConfigLocalMessageHandler (SwitchportVlanConfigLocalMessage *pSwitchportVlanConfigLocalMessage);
			ResourceId validateSwitchportVlanConfigStep (DceSynchronousLinearSeqContext *pContext);
			ResourceId switchportVlanConfigStep (DceSynchronousLinearSeqContext *pContext);
			ResourceId switchportVlanConfigMOStep (DceSynchronousLinearSeqContext *pContext);
			void macGroupConfigLocalMessageHandler (MacGroupConfigLocalMessage *pSwitchportVlanConfigLocalMessage);
			ResourceId macGroupConfigStep (DceSynchronousLinearSeqContext *pContext);

            void NsmLocalProvisioningInfoMessageHandler
                (NsmLocalProvisioningInfoMessage* pNsmLocalProvisioningInfoMessage);
            void sendNsmLocalProvisioningInfoMessageToCluster
                (PrismLinearSequencerContext* pPrismLinearSequencerContext);
            void sendNsmLocalProvisioningInfoMessageToClusterCallBack
                (WaveSendToClusterContext* pWaveSendToClusterContext);
            ResourceId populateProvisionedCacheDataOfThisNode
                (PrismSynchronousLinearSequencerContext* pPrismSynchronousLinearSequencerContext);
			ResourceId provisionVlansForSwitchportPo(PoIntfManagedObject *pPoMO);
			void gVlanEnableLocalMessageHandler (GVlanEnableLocalMessage *pGVlanEnableLocalMessage);
			ResourceId enableGVlanStg(DceSynchronousLinearSeqContext *pContext);

            
        protected:
            ResourceId getPhyIntfTypeFromMo(NsmUpdatePhyIntfMessage *pMessage, 
                PrismSynchronousLinearSequencerContext *pContext);
        public:
            IntfConfLocalWorker ( NsmLocalObjectManager *pWaveObjectManager);
            ResourceId createPhyIntfMO(NsmCreatePhyIntfMessage *pNsmCreatePhyIntfMessage,
									   PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            virtual    ~IntfConfLocalWorker ();
            ResourceId setAllPhyMo_PortRole_Edge();
            ResourceId replyConfigPhyIntfMO(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject, CfgReplayModes CfgReplayMode);
            void backendSyncUp (PrismAsynchronousContext *pPrismAsynchronousContext);

            bool isGlobalPortChannelReplayDone ();
            void setGlobalPortChannelReplayDone(bool replayDone);

            void bootPhaseInterfaceStateReplay();
            void interfaceStateReplayMessageHandler (InterfaceStateReplayServiceIndependentMessage *msg);
            static void interfaceStateReplayServiceIndependentMessageHandler (InterfaceStateReplayServiceIndependentMessage *msg);
            ResourceId notifySlotReplayCompleted (UI32 slotId);
// Now the data members

        private:
            bool m_isGlobalPortChannelReplayDone;
            set<ObjectId> m_portchannelReplayedSet;
            UI32 m_lastSlotIdMadeOffline;
            map<UI32, vector<ObjectId> > m_slotIdPortChannelOidMap;
        protected:
        public:
    };
    ResourceId fcoeDelPoIntf(NsmDeletePoIntfSPMessage *pNsmDeletePoIntfSPMessage);
}
#endif                                            //INTFCONFLOCALWORKER_H
