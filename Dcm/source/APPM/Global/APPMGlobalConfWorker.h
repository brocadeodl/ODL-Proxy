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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef APPMGLOBALCONFWORKER_H
#define APPMGLOBALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Utils/DceLinearSeqContext.h"

#include "APPM/Global/APPMGlobalTypes.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "correlator/PortProfileObject.h"
using namespace WaveNs;

namespace DcmNs
{

    class APPMGlobalObjectManager;
    class APPMGlobalAPPMProfileMessage;
    class APPMGlobalProfileDomainMessage;
    class APPMGlobalAPPMVlanMessage;
    class APPMGlobalAPPMQosMessage;
    class APPMGlobalAPPMFcoeMessage;
    class APPMGlobalAPPMSecurityMessage;
	class APPMGlobalAPPMProfileBulkMessage;
	class VcenterGetAllPortProfileListMessage;

    class APPMGlobalConfWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void APPMGlobalConfSendToClusterCallback (WaveSendToClusterContext  *ctx_p);
            void APPMGlobalPortProfileDomainSendToClusterCallback (WaveSendToClusterContext  *ctx_p);
			void APPMGlobalConfSendToClusterCallbackForBulk  (WaveSendToClusterContext *ctx_p);
			void APPMGlobalConfSendToClusterCallbackWithoutNextstep (WaveSendToClusterContext  *ctx_p);
            void ClusterGetFirstFailureLocation(WaveSendToClusterContext  *ctx_p , LocationId &locationid);
            void APPMGlobalAPPMFcoeMessageHandler( APPMGlobalAPPMFcoeMessage *pAPPMGlobalAPPMFcoeMessage);
            void APPMGlobalAPPMProfileMessageHandler( APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage);
            void APPMGlobalPortProfileDomainMessageHandler ( APPMGlobalProfileDomainMessage* pAPPMGlobalProfileDomainMessage);
            void APPMGlobalAPPMQosMessageHandler( APPMGlobalAPPMQosMessage *pAPPMGlobalAPPMQosMessage);
            void APPMGlobalAPPMSecurityMessageHandler( APPMGlobalAPPMSecurityMessage *pAPPMGlobalAPPMSecurityMessage);
            void APPMGlobalAPPMVlanMessageHandler( APPMGlobalAPPMVlanMessage *pAPPMGlobalAPPMVlanMessage);
			void APPMGlobalAPPMProfileBulkMessageHandler( APPMGlobalAPPMProfileBulkMessage *pAPPMGlobalAPPMProfileBulkMessage);
            void VcenterGetAllPortProfileListMessageHandler ( VcenterGetAllPortProfileListMessage *pVcenterGetAllPortProfileListMessage);
            void PortProfileObjectMessageHandler            (PortProfileObject *pProfile);

		    ResourceId profileValidationStep
			    (DceLinearSeqContext *pPrismLinearSequencerContext);
		    void profileMessageStep
			    (DceLinearSeqContext *pPrismLinearSequencerContext);
		    void profileMessageMoStep
			    (DceLinearSeqContext *pPrismLinearSequencerContext);

            ResourceId portProfileDomainValidationStep
                (DceLinearSeqContext *pPrismLinearSequencerContext);
            void portProfileDomainSendToClusterStep
                (DceLinearSeqContext *pPrismLinearSequencerContext);
            void portProfileDomainMoUpdateStep
                (DceLinearSeqContext *pPrismLinearSequencerContext);

            ResourceId 		profileBulkValidationStep           (DceLinearSeqContext *pPrismLinearSequencerContext);
            void 			profileBulkMessageStep              (DceLinearSeqContext *pPrismLinearSequencerContext);
            void 			profileBulkMessageMoStep            (DceLinearSeqContext *pPrismLinearSequencerContext);
			ResourceId 		profileBulkIntraValidation     	   	(APPMGlobalAPPMProfileBulkMessage *pAPPMGlobalAPPMProfileBulkMessage);
			ResourceId 		defaultProfileBulkValidation		(APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileBulkMessage);
			AppmReturnCode 	updateBulkAssociateInfo     		(PrismLinearSequencerContext  *pPrismLinearSequencerContext, APPMGlobalAPPMProfileBulkMessage *pMsg);

		    ResourceId securityProfileValidationStep
			    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
		    void qosMessageStep
			    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
		    void qosMessageMoStep
			    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ResourceId fcoeProfileValidationStep
				(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void fcoeMessageStep
			    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
		    void fcoeMessageMoStep
			    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
		    ResourceId qosProfileValidationStep
			    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
		    void securityMessageStep
			    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
		    void securityMessageMoStep
			    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ResourceId vlanProfileValidationStep
				(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void vlanMessageStep
				(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void vlanMessageMoStep
				(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            ResourceId secProfileRemoveAcl(PrismLinearSequencerContext *pPrismLinearSequencerContext);

			string getClassName();
			bool isProfileActivated(string profileName, ResourceId &status, PrismLinearSequencerContext *pPrismLinearSequencerContext);
            ResourceId appmQosVcsSanity(string profile_name, UI8 cmdCode, 
                string CeeMapName, string CosMutationMapName, SI32 Cos);
            ResourceId appmQosRunVcsSanity(string profile_name, UI8 cmdCode, 
                string mapName, SI32 Cos, bool isApply);

			bool isActiveProfilePresent();
            ResourceId validateProfileAlreadyCreatedOrAlreadyDeleted(APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage);
			ResourceId defaultProfileValidation(APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage);
			ResourceId profileAssocDeassocValidation(APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage, DceLinearSeqContext *dceLinearSeqContext);
			ResourceId profileIntraValidation(APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage);
			AppmReturnCode updateAssociateInfo(PrismLinearSequencerContext *pPrismLinearSequencerContext,APPMGlobalAPPMProfileMessage *pMsg);
			ResourceId profileBulkAssocValidation(APPMGlobalAPPMProfileBulkMessage *pAPPMGlobalAPPMProfileBulkMessage, DceLinearSeqContext *dceLinearSeqContext);



            ResourceId addRemoveProfilesToDefaultDomainWhenVirtualFabricIsDisabled
                (string &portProfileName, AppmOpCode cmdCode, DceLinearSeqContext* pPrismLinearSequencerContext);

            void upgrade (WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase);

		protected:
		public:
			APPMGlobalConfWorker ( APPMGlobalObjectManager *pAPPMGlobalObjectManager);
			virtual    ~APPMGlobalConfWorker ();
			static bool isFcoeEnabledInDefaultProfile(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = NULL,
				PrismLinearSequencerContext *pPrismLinearSequencerContext = NULL);

		private:
		protected:
		public:
	};
	extern UI32 getNextPPid();
	extern void setGlobalPPid(UI32 );
	extern void unSetGlobalPPid(UI32 );
}
#endif                                            //APPMGLOBALCONFWORKER_H
