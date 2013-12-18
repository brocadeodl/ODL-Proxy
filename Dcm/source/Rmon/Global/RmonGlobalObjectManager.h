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
 *   Author : vivekk                                                     *
 **************************************************************************/

#ifndef RMONGLOBALOBJECTMANAGER_H
#define RMONGLOBALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

using namespace WaveNs;

namespace DcmNs
{

    class RmonGlobalCreateRmonEventMessage;
    class RmonGlobalDeleteRmonEventMessage;
    class RmonGlobalCreateRmonAlarmMessage;
    class RmonGlobalDeleteRmonAlarmMessage;

    class RmonGlobalObjectManager: public WaveObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void RmonGlobalCreateRmonEventMessageHandler( RmonGlobalCreateRmonEventMessage *pRmonGlobalCreateRmonEventMessage);
            void RmonGlobalDeleteRmonEventMessageHandler( RmonGlobalDeleteRmonEventMessage *pRmonGlobalDeleteRmonEventMessage);
            void RmonGlobalCreateRmonAlarmMessageHandler( RmonGlobalCreateRmonAlarmMessage *pRmonGlobalCreateRmonAlarmMessage);
            void RmonGlobalDeleteRmonAlarmMessageHandler( RmonGlobalDeleteRmonAlarmMessage *pRmonGlobalDeleteRmonAlarmMessage);
        protected:
        public:
            RmonGlobalObjectManager ();
            virtual    ~RmonGlobalObjectManager ();
            static RmonGlobalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
			void deleteRmonEventValidationStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void createRmonEventGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void deleteRmonEventGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void createRmonEventGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void deleteRmonEventGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void createRmonAlarmGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void deleteRmonAlarmGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void createRmonAlarmGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void deleteRmonAlarmGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void rmonGlobalSendToClusterCallback(WaveSendToClusterContext *ctx_p);
			void ClusterGetFirstFailureLocation(WaveSendToClusterContext  *ctx_p , LocationId &locationid);
			void clusterCreateCollectValidationData(WaveObjectManagerCollectValidationDataMessage *pMessage,
                PrismAsynchronousContext *pPrismAsynchronousContext);
            void clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage,
                PrismAsynchronousContext *pPrismAsynchronousContext);
			void failover (FailoverAsynchronousContext *pFailoverAsynchronousContext);
            ResourceId RmonGlobalHandleFailoverForLocation (UI32 &location_id);
            ResourceId RmonGlobalCleanupForLocation (UI32 &location_id);


// Now the data members

        private:

        protected:
        public:
    };
}
#endif                                            //RMONGLOBALOBJECTMANAGER_H
