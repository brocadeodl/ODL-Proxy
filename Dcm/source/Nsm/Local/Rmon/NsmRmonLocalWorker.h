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

#ifndef NSMRMONLOCALWORKER_H
#define NSMRMONLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class RmonCreateRmonCollectionStatisticsMessage;
    class RmonCreateRmonCollectionHistoryMessage;
    class RmonDeleteRmonCollectionStatisticsMessage;
    class RmonDeleteRmonCollectionHistoryMessage;
	class DcmManagementInterfaceMessage;
	class NsmLocalObjectManager;

    class NsmRmonLocalWorker: public WaveWorker 
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void RmonCreateRmonCollectionHistoryMessageHandler( RmonCreateRmonCollectionHistoryMessage *pRmonCreateRmonCollectionHistoryMessage);
            void RmonCreateRmonCollectionStatisticsMessageHandler( RmonCreateRmonCollectionStatisticsMessage *pRmonCreateRmonCollectionStatisticsMessage);
            void RmonDeleteRmonCollectionHistoryMessageHandler( RmonDeleteRmonCollectionHistoryMessage *pRmonDeleteRmonCollectionHistoryMessage);
            void RmonDeleteRmonCollectionStatisticsMessageHandler( RmonDeleteRmonCollectionStatisticsMessage *pRmonDeleteRmonCollectionStatisticsMessage);
			ResourceId mapBackEndErrorToDcmdError(int rc);
			ResourceId errorCode(PrismMessage *pMsg, DcmManagementInterfaceMessage *clientMsg, ResourceId status);

			ResourceId createRmonStatsValidationStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createRmonStatsStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createRmonStatsMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createRmonStatsBookKeepMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId createRmonHistoryValidationStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createRmonHistoryStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createRmonHistoryMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId createRmonHistoryBookKeepMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteRmonStatsValidationStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteRmonStatsStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteRmonStatsMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteRmonStatsBookKeepMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteRmonHistoryStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteRmonHistoryMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteRmonHistoryBookKeepMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        protected:
        public:
            NsmRmonLocalWorker (NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual    ~NsmRmonLocalWorker ();

// Now the data members

        private:

        protected:
        public:
    };
}
#endif                                            //NSMRMONLOCALWORKER_H
