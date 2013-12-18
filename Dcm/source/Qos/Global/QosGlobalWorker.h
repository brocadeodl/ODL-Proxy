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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef QOSGLOBALWORKER_H
#define QOSGLOBALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Qos/Global/QosGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalObjectManager;
    class QosGlobalCreateCosToCosMutationMapMessage;
    class QosGlobalDeleteCosToCosMutationMapMessage;
    class QosGlobalCreateCosToTrafficClassMapMessage;
    class QosGlobalDeleteCosToTrafficClassMapMessage;
    class QosGlobalCreateQosQueueSchedulerMessage;
    class QosGlobalDeleteQosQueueSchedulerMessage;
    class QosGlobalCreateQosQueueMulticastSchedulerMessage;
    class QosGlobalDeleteQosQueueMulticastSchedulerMessage;
    class QosGlobalCreateQosRcvQueueMulticastSettingMessage;
    class QosGlobalDeleteQosRcvQueueMulticastSettingMessage;
	class QosGlobalCreateQosQueueMulticastSchedulerMessage;
	class QosGlobalDeleteQosQueueMulticastSchedulerMessage;
	class QosGlobalCreateQosQueueSchedulerMessage;
	class QosGlobalDeleteQosQueueSchedulerMessage;
    class QosGlobalCreateDscpMarkListMessage;
    class QosGlobalDeleteDscpMarkListMessage;
    class QosGlobalCreateDscpToDscpMutationMapMessage;
    class QosGlobalDeleteDscpToDscpMutationMapMessage;
    class QosGlobalCreateDscpToTrafficClassMapMessage;
    class QosGlobalDeleteDscpToTrafficClassMapMessage;
    class QosGlobalCreateDscpToCosMapMessage;
    class QosGlobalDeleteDscpToCosMapMessage;
    class QosGlobalCreateRedProfileMessage;
    class QosGlobalDeleteRedProfileMessage;
	class QosGlobalNasMessage;
	class NasConfigManagedObject;

    class QosGlobalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void QosGlobalCreateCosToCosMutationMapMessageHandler( QosGlobalCreateCosToCosMutationMapMessage *pQosGlobalCreateCosToCosMutationMapMessage);
            void QosGlobalCreateCosToTrafficClassMapMessageHandler( QosGlobalCreateCosToTrafficClassMapMessage *pQosGlobalCreateCosToTrafficClassMapMessage);
            void QosGlobalCreateQosQueueMulticastSchedulerMessageHandler( QosGlobalCreateQosQueueMulticastSchedulerMessage *pQosGlobalCreateQosQueueMulticastSchedulerMessage);
            void QosGlobalCreateQosQueueSchedulerMessageHandler( QosGlobalCreateQosQueueSchedulerMessage *pQosGlobalCreateQosQueueSchedulerMessage);
            void QosGlobalCreateQosRcvQueueMulticastSettingMessageHandler( QosGlobalCreateQosRcvQueueMulticastSettingMessage *pQosGlobalCreateQosRcvQueueMulticastSettingMessage);
            void QosGlobalDeleteCosToCosMutationMapMessageHandler( QosGlobalDeleteCosToCosMutationMapMessage *pQosGlobalDeleteCosToCosMutationMapMessage);
            void QosGlobalDeleteCosToTrafficClassMapMessageHandler( QosGlobalDeleteCosToTrafficClassMapMessage *pQosGlobalDeleteCosToTrafficClassMapMessage);
            void QosGlobalDeleteQosQueueMulticastSchedulerMessageHandler( QosGlobalDeleteQosQueueMulticastSchedulerMessage *pQosGlobalDeleteQosQueueMulticastSchedulerMessage);
            void QosGlobalDeleteQosQueueSchedulerMessageHandler( QosGlobalDeleteQosQueueSchedulerMessage *pQosGlobalDeleteQosQueueSchedulerMessage);
            void QosGlobalDeleteQosRcvQueueMulticastSettingMessageHandler( QosGlobalDeleteQosRcvQueueMulticastSettingMessage *pQosGlobalDeleteQosRcvQueueMulticastSettingMessage);
			/* NagP: New */
            void QosGlobalCreateDscpMarkListMessageHandler( QosGlobalCreateDscpMarkListMessage *pQosGlobalCreateDscpMarkListMessage);
            void QosGlobalCreateDscpToCosMapMessageHandler( QosGlobalCreateDscpToCosMapMessage *pQosGlobalCreateDscpToCosMapMessage);
            void QosGlobalCreateDscpToDscpMutationMapMessageHandler( QosGlobalCreateDscpToDscpMutationMapMessage *pQosGlobalCreateDscpToDscpMutationMapMessage);
            void QosGlobalCreateDscpToTrafficClassMapMessageHandler( QosGlobalCreateDscpToTrafficClassMapMessage *pQosGlobalCreateDscpToTrafficClassMapMessage);
            void QosGlobalCreateRedProfileMessageHandler( QosGlobalCreateRedProfileMessage *pQosGlobalCreateRedProfileMessage);
            void QosGlobalDeleteDscpMarkListMessageHandler( QosGlobalDeleteDscpMarkListMessage *pQosGlobalDeleteDscpMarkListMessage);
            void QosGlobalDeleteDscpToCosMapMessageHandler( QosGlobalDeleteDscpToCosMapMessage *pQosGlobalDeleteDscpToCosMapMessage);
            void QosGlobalDeleteDscpToDscpMutationMapMessageHandler( QosGlobalDeleteDscpToDscpMutationMapMessage *pQosGlobalDeleteDscpToDscpMutationMapMessage);
            void QosGlobalDeleteDscpToTrafficClassMapMessageHandler( QosGlobalDeleteDscpToTrafficClassMapMessage *pQosGlobalDeleteDscpToTrafficClassMapMessage);
            void QosGlobalDeleteRedProfileMessageHandler( QosGlobalDeleteRedProfileMessage *pQosGlobalDeleteRedProfileMessage);
			/* NagP: New */
			void QosGlobalNasMessageHandler( QosGlobalNasMessage *pQosGlobalNasMessage);
			void createCosToCosMutationMapStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
             void createCosToCosMutationMapMOStep (PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void deleteCosToCosMutationMapValidationStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);

			void cleanupCosMutationMapReferenceInFailedOverNodes(
					PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void cleanupCosToTrafficClassMapReferenceInFailedOverNodes(
					PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void cleanupDscpToCosMapReferenceInFailedOverNodes(
					PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void cleanupDscpMutationMapReferenceInFailedOverNodes(
					PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void cleanupDscpToTrafficClassMapReferenceInFailedOverNodes(
					PrismLinearSequencerContext *pPrismLinearSequencerContext);

			void deleteCosToCosMutationMapStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
             void deleteCosToCosMutationMapMOStep (PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void createCosToTrafficClassMapStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void createCosToTrafficClassMapMOStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void deleteCosToTrafficClassMapValidationStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void deleteCosToTrafficClassMapStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void deleteCosToTrafficClassMapMOStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void createMcastSettingStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void createMcastSettingMOStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void deleteMCastSettingStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void deleteMCastSettingMOStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void createMcastSchedulerStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void createMcastSchedulerMOStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void deleteMcastSchedulerStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void deleteMcastSchedulerMOStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void createSchedulerStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void createSchedulerMOStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void deleteSchedulerStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
			void deleteSchedulerMOStep(PrismLinearSequencerContext
                 *pPrismLinearSequencerContext);
            void appmQosProfileMutationUpdateStep(PrismLinearSequencerContext 
                *pPrismLinearSequencerContext);
            void appmQosProfileTrafficClassUpdateStep(PrismLinearSequencerContext 
                *pPrismLinearSequencerContext);
			void QosGlobalSendToClusterCallback(WaveSendToClusterContext  *ctx_p);
            void createDscpMarkListStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createDscpMarkListMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createDscpToCosMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createDscpToCosMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createDscpToDscpMutationMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createDscpToDscpMutationMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createDscpToTrafficClassMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createDscpToTrafficClassMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createRedProfileStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createRedProfileMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deleteDscpMarkListStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deleteDscpMarkListMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void deleteDscpToCosMapValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deleteDscpToCosMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deleteDscpToCosMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void deleteDscpToDscpMutationMapValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deleteDscpToDscpMutationMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deleteDscpToDscpMutationMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void deleteDscpToTrafficClassMapValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deleteDscpToTrafficClassMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deleteDscpToTrafficClassMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deleteRedProfileStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deleteRedProfileMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void NasValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void NasSendToLocalWorkerStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void NasCommitDBStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void createNasConfigAndReflectInCeeMap(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void deleteNasConfigAndReflectInCeeMap(PrismLinearSequencerContext *pPrismLinearSequencerContext,
				NasConfigManagedObject *pNasConfigManagedObject);
			void setNasCosValueAndReflectInCeeMap(PrismLinearSequencerContext *pPrismLinearSequencerContext,
				NasConfigManagedObject *pNasConfigManagedObject,const UI32 &cosVlaue);
			void deleteNasCosValueAndReflectInCeeMap(PrismLinearSequencerContext *pPrismLinearSequencerContext,
				NasConfigManagedObject *pNasConfigManagedObject);
			void deleteNasServerIpConfig(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void createNasServerIpConfig(PrismLinearSequencerContext *pPrismLinearSequencerContext);

        protected:
        public:
            QosGlobalWorker ( QosGlobalObjectManager *pQosGlobalObjectManager);
            virtual    ~QosGlobalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //QOSGLOBALWORKER_H
