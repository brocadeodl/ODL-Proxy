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

#ifndef INTFCONFGLOBALWORKER_H
#define INTFCONFGLOBALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Utils/DceLinearSeqContext.h"
using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class NsmGlobalCreateVlanIntfMessage;
    class NsmGlobalUpdateVlanIntfMessage;
    class NsmGlobalDeleteVlanIntfMessage;
    class NsmGlobalCreatePoIntfMessage;
    class NsmGlobalUpdatePoIntfMessage;
    class NsmGlobalDeletePoIntfMessage;
    class NsmGlobalAddPoToVlanIntfMessage;
    class NsmGlobalDeletePoToVlanIntfMessage;
    class NsmGlobalVlanAssocMessage;
    class NsmGlobalVlanDissocMessage;
    class NsmGlobalPoPhyAddMappingMessage;
    class NsmGlobalPoPhyDelMappingMessage;
	class NsmGlobalUpdateLacpConfigMessage;
	class NsmGlobalAssocMacAclPolicyPoVlanIntfMessage;
    class NsmGlobalFcoeVlanMessage;
    class NsmGlobalReservedVlanMessage;
	class NsmGlobalQosPOConfigMessage;
    class NsmGlobalAddPhyIntfToPoIntfMessage;
	class NsmGlobalQosPfcConfigMessage;
	class QosGlobalCosProfileAndThresholdConfigMessage;
	class NsmGlobalReAssocMacAclPolicyMessage;
	class SsmDeleteAssociationwithDisconnectedMessage;
	class NsmGlobalQosMapCleanUpMessage;
	class NsmGlobalReAssocIpAclPolicyMessage;
	class NsmGlobalAssocIpAclPolicyPoVlanIntfMessage;
	class PBRPoVlanIntfMessage;
	class IpPolicyDeleteGlobalPBRMessage;
	class SwitchportVlanConfigGlobalMessage;
	class MacGroupConfigGlobalMessage;
	class GVlanEnableGlobalMessage;
	
    class IntfConfGlobalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmGlobalAddPoToVlanIntfMessageHandler( NsmGlobalAddPoToVlanIntfMessage *pNsmGlobalAddPoToVlanIntfMessage);
            void NsmGlobalVlanAssocMessageHandler( NsmGlobalVlanAssocMessage *pNsmGlobalVlanAssocMessage);
            void NsmGlobalPoPhyAddMappingMessageHandler(NsmGlobalPoPhyAddMappingMessage *pNsmGlobalPoPhyAddMappingMessage);
            void NsmGlobalCreatePoIntfMessageHandler( NsmGlobalCreatePoIntfMessage *pNsmGlobalCreatePoIntfMessage);
            void NsmGlobalCreateVlanIntfMessageHandler( NsmGlobalCreateVlanIntfMessage *pNsmGlobalCreateVlanIntfMessage);
            void NsmGlobalDeletePoIntfMessageHandler( NsmGlobalDeletePoIntfMessage *pNsmGlobalDeletePoIntfMessage);
            void NsmGlobalDeletePoToVlanIntfMessageHandler( NsmGlobalDeletePoToVlanIntfMessage *pNsmGlobalDeletePoToVlanIntfMessage);
            void NsmGlobalVlanDissocMessageHandler( NsmGlobalVlanDissocMessage *pNsmGlobalVlanDissocMessage);
            void NsmGlobalPoPhyDelMappingMessageHandler(NsmGlobalPoPhyDelMappingMessage *pNsmGlobalPoPhydelMappingMessage);
            void NsmGlobalDeleteVlanIntfMessageHandler( NsmGlobalDeleteVlanIntfMessage *pNsmGlobalDeleteVlanIntfMessage);
            void NsmGlobalUpdatePoIntfMessageHandler( NsmGlobalUpdatePoIntfMessage *pNsmGlobalUpdatePoIntfMessage);
            void NsmGlobalUpdateVlanIntfMessageHandler( NsmGlobalUpdateVlanIntfMessage *pNsmGlobalUpdateVlanIntfMessage);
            void NsmGlobalUpdateLacpConfigMessageHandler( NsmGlobalUpdateLacpConfigMessage *pNsmGlobalUpdateLacpConfigMessage);
            void NsmGlobalAssocMacAclPolicyPoVlanIntfMessageHandler(NsmGlobalAssocMacAclPolicyPoVlanIntfMessage *pNsmGlobalAssocMacAclPolicyPoVlanIntfMessage);
            void NsmGlobalAssocIpAclPolicyPoVlanIntfMessageHandler(NsmGlobalAssocIpAclPolicyPoVlanIntfMessage *pNsmGlobalAssocIpAclPolicyPoVlanIntfMessage);
            void NsmGlobalFcoeVlanMessageHandler(NsmGlobalFcoeVlanMessage *fcoe_vlan_msg_p);
            void NsmGlobalReservedVlanMessageHandler(NsmGlobalReservedVlanMessage *reserved_vlan_msg_p);
			void NsmGlobalQosMapCleanUp (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void NsmGlobalQosMapCleanUpMessageHandler(NsmGlobalQosMapCleanUpMessage *pNsmGlobalQosMapCleanUpMessage);
			void NsmGlobalQosPOConfigMessageHandler(NsmGlobalQosPOConfigMessage *pNsmGlobalQosPOConfigMessage);
			void NsmGlobalQosPfcConfigMessageHandler(NsmGlobalQosPfcConfigMessage *pNsmGlobalQosPfcConfigMessage);
			void NsmGlobalCosProfileAndThresholdConfigHandler(QosGlobalCosProfileAndThresholdConfigMessage *pQosGlobalConfigMessage);
			void SsmDeleteAssociationwithDisconnectedMessageHandler(SsmDeleteAssociationwithDisconnectedMessage *pSsmDeleteAssociationwithDisconnectedMessage);
			void NsmGlobalPBRPoVlanIntfMessageHandler (PBRPoVlanIntfMessage *pPBRPoVlanIntfMessage);
			void NsmGlobalPBRDeleteIntfMessageHandler (IpPolicyDeleteGlobalPBRMessage *pPBRGlobalDelete);
			void deletePBRGlobal(PrismLinearSequencerContext *pLinearSeqContext);
			ResourceId deleteIntfAssociatedIngressEgressAcl (ObjectId policyId, UI32 aclType);
            /* Code Starts Here */
			void validatePBRPoStep (PrismLinearSequencerContext *pLinearSeqContext);
			void configurePBRPoIntfStep (PrismLinearSequencerContext *pLinearSeqContext);
			void updatePBRPoIntfStep(PrismLinearSequencerContext *pLinearSeqContext);
			ResourceId updatePoIntfPBR(PrismLinearSequencerContext *pLinearSeqContext);
			ResourceId removePBRPoIntf(PrismLinearSequencerContext *pLinearSeqContext);
			void validateCreatePoIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createPoIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createPoIntfMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void updatePoIntfStep (DceLinearSeqContext *pDceLinearSeqContext);

            ResourceId validatePoIntfStep (DceLinearSeqContext *pDceLinearSeqContext);
            void updatePoIntfMOStep (DceLinearSeqContext *pDceLinearSeqContext);
			ResourceId updatePoIntfMO(NsmGlobalUpdatePoIntfMessage *pNsmGlobalUpdatePoIntfMessage, 
			        DceLinearSeqContext *pDceLinearSeqContext);
			void deletePortProfileVlanReference(DceLinearSeqContext *pDceLinearSeqContext);
			void deletePoIntfVlanReference(const UI32 &vlanId, DceLinearSeqContext *pDceLinearSeqContext);

            void deletePoIntfStep (DceLinearSeqContext *pDceLinearSeqContext);
            void deletePoIntfLocalDBUpdateStep (DceLinearSeqContext *pDceLinearSeqContext);
            void deletePoIntfMOStep (DceLinearSeqContext *pDceLinearSeqContext);
            void updateVlanIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void updateVlanIntfMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void validateVlanIntfUpdateStep (DceLinearSeqContext *pDceLinearSeqContext);
            void validateVlanIntfDeleteStep (DceLinearSeqContext *pDceLinearSeqContext);
            void deleteVlanIntfStep (DceLinearSeqContext *pDceLinearSeqContext);
            void deleteVlanIntfLocalDBUpdateStep (DceLinearSeqContext *pDceLinearSeqContext);
            void deleteVlanIntfMOStep (DceLinearSeqContext *pDceLinearSeqContext);
			void deleteVlanIntfXstpStep (DceLinearSeqContext *pDceLinearSeqContext);
			void deleteSviIntfStep (DceLinearSeqContext *pDceLinearSeqContext);
            void createVlanIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createVlanIntfMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createBulkVlanIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            
			void updateLacpConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void updateLacpConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void LacpGlobalConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
      		void validatePhyIntfToPoIntfStep (DceLinearSeqContext *pDceLinearSeqContext);
            void addPhyIntfToPoIntfStep (DceLinearSeqContext *pDceLinearSeqContext);
            void addPhyIntfToPoIntfLocalDBCommitStep (DceLinearSeqContext *pDceLinearSeqContext);

			void NsmGlobalQosPoValidationStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void NsmGlobalQosPoSendToLocal (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void updatePoIntfQosMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void updateQosPfcPoIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void updateQosPfcPoIntfValidationStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void updateQosPfcPoIntfMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void updateQosProfThresPoIntfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void updateQosProfThresPoIntfMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void validateMacAclToPoVlanStep(DceLinearSeqContext *pDceLinearSeqContext);
			void configureMacAclToPoVlanIntfStep(PrismLinearSequencerContext *pLinearSeqContext);
			void updatePoVlanIntfMacAclStep(DceLinearSeqContext *pDceLinearSeqContext);
			void configureMacAclToPoVlanIntfClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
			
			void validateIpAclToPoStep(PrismLinearSequencerContext *pLinearSeqContext);
			void configureIpAclToPoIntfStep(PrismLinearSequencerContext *pLinearSeqContext);
			void updatePoIntfIpAclStep(PrismLinearSequencerContext *pLinearSeqContext);
			void configureIpAclToPoIntfClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
			
			bool ifVlanExistsByVlanId(UI32 vlanId, PrismLinearSequencerContext	*pPrismLinearSequencerContext);
			
			ResourceId updatePoIntfMacAclStep(DceLinearSeqContext *pDceLinearSeqContext);
			ResourceId updateVlanIntfMacAclStep(DceLinearSeqContext *pDceLinearSeqContext);
			ResourceId removeMacAclPoVlanIntf(PrismLinearSequencerContext *pLinearSeqContext);
			
			ResourceId 	updatePoIntfIpAcl(PrismLinearSequencerContext *pLinearSeqContext);
			ResourceId 	removeIpAclPoIntf(PrismLinearSequencerContext *pLinearSeqContext);
			void       NsmGlobalReAssocMacAclPolicyMessageHandler(NsmGlobalReAssocMacAclPolicyMessage *pNsmGlobalReAssocMacAclPolicyMessage);
            
            
			void       	NsmGlobalReAssocIpAclPolicyMessageHandler(NsmGlobalReAssocIpAclPolicyMessage *pNsmGlobalReAssocIpAclPolicyMessage);
            void NsmGlobalFcoeVlanSendToClusterCallback (PrismAsynchronousContext *ctx_p);
            void NsmGlobalFcoeVlanSendToLocal(PrismLinearSequencerContext *pLinearSeqContext);
            void NsmReservedVlanSendtoWaveClusterStep(PrismLinearSequencerContext *pLinearSeqContext);

            /*ResourceId NsmGlobalFcoeVlanHandleMO(PrismSynchronousLinearSequencerContext *pLinearSeqContext);*/
            void NsmGlobalReservedVlanMOStep(PrismLinearSequencerContext *pLinearSeqContext);

            ResourceId getPoIntfObjectIdByPoIfName (string poName, ObjectId &poIntf_oid,
			                                      bool removeImmediate, PrismLinearSequencerContext *pPrismLinearSequencerContext); 
            ResourceId getMacFromDomainId(string &mac, UI32 domainId);
            ResourceId getVlanIntfObjectIdByVlanIfName (UI32 vlanName, ObjectId &poIntf_oid,
			                                      bool removeImmediate, DceLinearSeqContext *pDceLinearSeqContext);
            ResourceId isPoIntfHasMember (ObjectId &poIntf_oid);
            PoIntfManagedObject * getPoIntfObjectByPoIfName (string poName);
            ObjectId   FcoeMapGetObjectIdByName (string &map_name,PrismLinearSequencerContext *pPrismLinearSequencerContext);

            void  NsmGlobalAddPhyIntfToPoIntfMessageHandler( NsmGlobalAddPhyIntfToPoIntfMessage *pNsmGlobalAddPhyIntfToPoIntfMessage);
			ResourceId getCosMapObjectIdByCosMapName(string cosMapName, ObjectId &cosMapObjectId, 
													 PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ResourceId getTcMapObjectIdByTcMapName(string cosMapName, ObjectId &cosMapObjectId, 
												   PrismLinearSequencerContext *pPrismLinearSequencerContext);
		    ResourceId getCeeMapObjectIdByCeeMapName(string ceeMapName, ObjectId &ceeMapObjectId, 
													 PrismLinearSequencerContext *pPrismLinearSequencerContext);
            ResourceId getDscpMapObjectIdByDscpMapName(string dscpMapName, ObjectId &dscpMapObjectId,
                                                   PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ResourceId getDscpTcMapObjectIdByDscpTcMapName(string dscpMapName, ObjectId &dscpMapObjectId,
                                                   PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ResourceId getDscpCosMapObjectIdByDscpCosMapName(string dscpMapName, ObjectId &dscpMapObjectId,
													 PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void returnSucceededStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void IntfConfGlobalSendToClusterCallback (WaveSendToClusterContext  *ctx_p);
            void IntfConfGlobalSendToVlanCreateClusterCallback(WaveSendToClusterContext  *ctx_p);
            void IntfConfGlobalReplayConfigCallback (WaveSendToClusterContext  *ctx_p);
            void IntfConfGlobalAddToPoCallback (WaveSendToClusterContext  *ctx_p);
			void removeVlanClassifierPoIntf(const ObjectId &vlanObjectId);
            ResourceId getPoMoByPoId(UI32 PoId, PoIntfManagedObject **ppPoIntfManagedObject);
            void deleteVlanClassifierVlanReference(DceLinearSeqContext *pDceLinearSeqContext);

            //PVLAN
            void deletePVlanReferences(DceLinearSeqContext *pDceLinearSeqContext);
            void removePVlanAssociations(const UI32 &vlanId,PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void removePVlanHostAssociation(const UI32 &vlanId,PrismLinearSequencerContext *pPrismLinearSequencerContext, UI32Range range = UI32Range(""));
			void removePVlanTrunkAssociation(const UI32 &vlanId,PrismLinearSequencerContext *pPrismLinearSequencerContext, UI32Range range = UI32Range(""));
			void removePVlanMapping(const UI32 &vlanId,PrismLinearSequencerContext *pPrismLinearSequencerContext, UI32Range range = UI32Range(""));

			void sendPoConfigToLocal (PrismLinearSequencerContext *pPrismLinearSequencerContext,const string& ifName, LocationId& locationId);
            void defaultConfMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        	void upgrade (WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase);
            void addPhyIntfToPoIntfMOStep (DceLinearSeqContext *pDceLinearSeqContext);
			NativeVlanIntfConfLocalManagedObject*
			NativeVlanPoGetChildMoByParentMo(PoIntfManagedObject *pPoIntfManagedObject);
			int  deleteNativeVlanFromMoVlanList (NsmGlobalUpdatePoIntfMessage *pNsmGlobalUpdatePoIntfMessage, 
													PoIntfManagedObject *pPoIntfManagedObject);
			int  deleteNativeVlanFromMsgVlanList (NsmGlobalUpdatePoIntfMessage *pNsmGlobalUpdatePoIntfMessage,
												PoIntfManagedObject *pPoIntfManagedObject);
			ResourceId validatePortChannelCount();
			ResourceId validateVlanForRSPANConfig(NsmGlobalUpdatePoIntfMessage *pmsg,DceLinearSeqContext *seq_ctx_p);
            InterfaceType getPoFirstMemberPhyIntfType(const string &poName);
            ResourceId validateL3ConfigForChannelGroup(NsmL3ReadyManagedObject *pL3ReadyMo);
            bool isIgmpConfigPresent(NsmL3ReadyManagedObject *pL3ReadyMo);
			void switchportVlanConfigGlobalMessageHandler(SwitchportVlanConfigGlobalMessage *pSwitchportVlanConfigGlobalMessage);
			void validateSwitchportVlanConfigStep(DceLinearSeqContext *pContext);
			void switchportVlanConfigStep(DceLinearSeqContext *pContext);
			void switchportVlanConfigMOStep(DceLinearSeqContext *pContext);
			void macGroupConfigGlobalMessageHandler(MacGroupConfigGlobalMessage *pMacGroupConfigGlobalMessage);
			void validateMacGroupConfigStep(DceLinearSeqContext *pContext);
			void macGroupConfigStep(DceLinearSeqContext *pContext);
			void macGroupConfigMOStep(DceLinearSeqContext *pContext);
			void deleteVlanClassificationReferences(DceLinearSeqContext *pDceLinearSeqContext);
			void clearVlanClassificationWithVlan(const UI32 &vlanId, DceLinearSeqContext *pDceLinearSeqContext, const string &parentMoName, const string &childMoName, const string &columnName);
			void deleteMacGroupReferences(DceLinearSeqContext *pContext);
			void gVlanEnableGlobalMessageHandler(GVlanEnableGlobalMessage *pGVlanEnableGlobalMessage);
			void validateGVlanEnableStg0(DceLinearSeqContext *pContext);
			void enableGVlanStg4(DceLinearSeqContext *pContext);
			void enableGVlanStg1(DceLinearSeqContext *pContext);
			void enableGVlanStg2(DceLinearSeqContext *pContext);
			void enableGVlanStg3(DceLinearSeqContext *pContext);
			void GVlanEnableSendToClusterCallback (WaveSendToClusterContext *ctx_p);
			void sendGVlanEnableCmdToLocalWorker(DceLinearSeqContext *pContext, const gvlan_enable_stage_t &cmdCode);
			void updateGVlanConfigMOStep(DceLinearSeqContext *pContext);
            /* Code Ends Here */
        protected:
        public:
            IntfConfGlobalWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager);
            ResourceId createVlanIntfMO (NsmGlobalCreateVlanIntfMessage *pNsmGlobalCreateVlanIntfMessage,
										 PrismLinearSequencerContext *pPrismLinearSequencerContext,
                                         bool removeImmediate = false);
            ResourceId createPoIntfMO (NsmGlobalCreatePoIntfMessage *pNsmGlobalCreatePoIntfMessage, 
									   PrismLinearSequencerContext *pPrismLinearSequencerContext);
            virtual    ~IntfConfGlobalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //INTFCONFGLOBALWORKER_H
