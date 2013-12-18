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

#ifndef QOSLOCALWORKER_H
#define QOSLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosLocalObjectManager;
	class QosCreateCeeRemapSPMessage;
    class QosCreateCeeMapSPMessage;
    class QosDeleteCeeMapSPMessage;
    class QosCreatePriorityGroupSPMessage;
    class QosDeletePriorityGroupSPMessage;
    class QosCreatePriorityMapSPMessage;
    class QosDeletePriorityMapSPMessage;
    class QosCreateQosQueueSchedulerSPMessage;
    class QosDeleteQosQueueSchedulerSPMessage;
    class QosCreateQosQueueMulticastSchedulerSPMessage;
    class QosDeleteQosQueueMulticastSchedulerSPMessage;
    class QosCreateCosToCosMutationMapSPMessage;
    class QosDeleteCosToCosMutationMapSPMessage;
    class QosCreateCosToTrafficClassMapSPMessage;
    class QosDeleteCosToTrafficClassMapSPMessage;
    class QosCreateQosRcvQueueMulticastSettingSPMessage;
    class QosDeleteQosRcvQueueMulticastSettingSPMessage;
	class DcmManagementInterfaceMessage;
    class QosLocalShowMessage;
    class QosCreateDscpToDscpMutationMapSPMessage;
    class QosDeleteDscpToDscpMutationMapSPMessage;
    class QosCreateDscpToTrafficClassMapSPMessage;
    class QosDeleteDscpToTrafficClassMapSPMessage;
    class QosCreateDscpToCosMapSPMessage;
    class QosDeleteDscpToCosMapSPMessage;
    class QosCreateDscpMarkListSPMessage;
    class QosDeleteDscpMarkListSPMessage;
    class QosCreateRedProfileSPMessage;
    class QosDeleteRedProfileSPMessage;
	class QosLocalNasMessage;
	class QosLocalNasShowMessage;

    class QosLocalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			void QosCreateCeeRemapSPMessageHandler( QosCreateCeeRemapSPMessage *pQosCreateCeeRemapSPMessage);
            void QosCreateCeeMapSPMessageHandler( QosCreateCeeMapSPMessage *pQosCreateCeeMapSPMessage);
            void QosCreateCosToCosMutationMapSPMessageHandler( QosCreateCosToCosMutationMapSPMessage *pQosCreateCosToCosMutationMapSPMessage);
            void QosCreateCosToTrafficClassMapSPMessageHandler( QosCreateCosToTrafficClassMapSPMessage *pQosCreateCosToTrafficClassMapSPMessage);
            void QosCreatePriorityGroupSPMessageHandler( QosCreatePriorityGroupSPMessage *pQosCreatePriorityGroupSPMessage);
            void QosCreatePriorityMapSPMessageHandler( QosCreatePriorityMapSPMessage *pQosCreatePriorityMapSPMessage);
            void QosCreateQosQueueMulticastSchedulerSPMessageHandler( QosCreateQosQueueMulticastSchedulerSPMessage *pQosCreateQosQueueMulticastSchedulerSPMessage);
            void QosCreateQosQueueSchedulerSPMessageHandler( QosCreateQosQueueSchedulerSPMessage *pQosCreateQosQueueSchedulerSPMessage);
            void QosCreateQosRcvQueueMulticastSettingSPMessageHandler( QosCreateQosRcvQueueMulticastSettingSPMessage *pQosCreateQosRcvQueueMulticastSettingSPMessage);
            void QosDeleteCeeMapSPMessageHandler( QosDeleteCeeMapSPMessage *pQosDeleteCeeMapSPMessage);
            void QosDeleteCosToCosMutationMapSPMessageHandler( QosDeleteCosToCosMutationMapSPMessage *pQosDeleteCosToCosMutationMapSPMessage);
            void QosDeleteCosToTrafficClassMapSPMessageHandler( QosDeleteCosToTrafficClassMapSPMessage *pQosDeleteCosToTrafficClassMapSPMessage);
            void QosDeletePriorityGroupSPMessageHandler( QosDeletePriorityGroupSPMessage *pQosDeletePriorityGroupSPMessage);
            void QosDeletePriorityMapSPMessageHandler( QosDeletePriorityMapSPMessage *pQosDeletePriorityMapSPMessage);
            void QosDeleteQosQueueMulticastSchedulerSPMessageHandler( QosDeleteQosQueueMulticastSchedulerSPMessage *pQosDeleteQosQueueMulticastSchedulerSPMessage);
            void QosDeleteQosQueueSchedulerSPMessageHandler( QosDeleteQosQueueSchedulerSPMessage *pQosDeleteQosQueueSchedulerSPMessage);
            void QosDeleteQosRcvQueueMulticastSettingSPMessageHandler( QosDeleteQosRcvQueueMulticastSettingSPMessage *pQosDeleteQosRcvQueueMulticastSettingSPMessage);
			/* NagP: New */
            void QosCreateDscpMarkListSPMessageHandler( QosCreateDscpMarkListSPMessage *pQosCreateDscpMarkListSPMessage);
            void QosCreateDscpToCosMapSPMessageHandler( QosCreateDscpToCosMapSPMessage *pQosCreateDscpToCosMapSPMessage);
            void QosCreateDscpToDscpMutationMapSPMessageHandler( QosCreateDscpToDscpMutationMapSPMessage *pQosCreateDscpToDscpMutationMapSPMessage);
            void QosCreateDscpToTrafficClassMapSPMessageHandler( QosCreateDscpToTrafficClassMapSPMessage *pQosCreateDscpToTrafficClassMapSPMessage);
            void QosCreateRedProfileSPMessageHandler( QosCreateRedProfileSPMessage *pQosCreateRedProfileSPMessage);
            void QosDeleteDscpMarkListSPMessageHandler( QosDeleteDscpMarkListSPMessage *pQosDeleteDscpMarkListSPMessage);
            void QosDeleteDscpToCosMapSPMessageHandler( QosDeleteDscpToCosMapSPMessage *pQosDeleteDscpToCosMapSPMessage);
            void QosDeleteDscpToDscpMutationMapSPMessageHandler( QosDeleteDscpToDscpMutationMapSPMessage *pQosDeleteDscpToDscpMutationMapSPMessage);
            void QosDeleteDscpToTrafficClassMapSPMessageHandler( QosDeleteDscpToTrafficClassMapSPMessage *pQosDeleteDscpToTrafficClassMapSPMessage);
            void QosDeleteRedProfileSPMessageHandler( QosDeleteRedProfileSPMessage *pQosDeleteRedProfileSPMessage);
			/* NagP: New */
	    	void QosNasShowMessageHandler(QosLocalNasShowMessage *pQosLocalNasShowMessage);
	    	void sendNasShowToCluster(PrismLinearSequencerContext *seq_ctx_p);
	    	ResourceId sendNasShowToQosDaemon(PrismSynchronousLinearSequencerContext *ctx_p);
			void QosNasMessageHandler( QosLocalNasMessage *pQosLocalNasMessage);
	    	void QosLocalShowMessageHandler (QosLocalShowMessage *pQosLocalShowMessage);
	    	void QosLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
	    	void QosLocalNasShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
	    	void sendShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
	    	ResourceId sendShowToQosDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            ResourceId createCeeRemapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId createCeeMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId createPriorityGroupStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId createPriorityMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId deleteCeeMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId deletePriorityGroupStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId deletePriorityMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createCosToCosMutationMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteCosToCosMutationMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createCosToTrafficClassMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteCosToTrafficClassMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createMcastSettingStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteMCastSettingStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createMCastSchedulerStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteMCastSchedulerStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createSchedulerStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteSchedulerStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId errorCode(PrismMessage* pMsg, DcmManagementInterfaceMessage* clientMsg, ResourceId status);
			ResourceId createDscpToDscpMutationMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteDscpToDscpMutationMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createDscpToTrafficClassMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteDscpToTrafficClassMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createDscpToCosMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteDscpToCosMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createDscpMarkListStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteDscpMarkListStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createRedProfileStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteRedProfileStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId NasUpdateBackendStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId NasValidationStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

        protected:
        public:
            QosLocalWorker ( QosLocalObjectManager *pQosLocalObjectManager);
            virtual    ~QosLocalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //QOSLOCALWORKER_H
