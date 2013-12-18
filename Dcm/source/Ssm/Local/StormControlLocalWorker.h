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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Ojasvita                                                     *
 **************************************************************************/

#ifndef STORMCONTROLLOCALWORKER_H
#define STORMCONTROLLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"

#include "Ssm/Local/StormControlLocalShowMessage.h"
#include "Ssm/Local/StormControlLocalClearMessage.h"
#include "Ssm/Local/SsmLocalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{
    class SsmLocalObjectManager;
	class DcmManagementInterfaceMessage;
    class StormControlLocalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			void StormControlLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            void sendShowStormControlCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            ResourceId sendShowStormControlToSsmDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void StormControlLocalShowMessageHandler (StormControlLocalShowMessage *pStormControlLocalShowMessage);
            void StormControlLocalClearMessageHandler (StormControlLocalClearMessage *pStormControlLocalClearMessage);
            void StormControlLocalClearSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            void sendClearStormControlCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            ResourceId sendClearStormControlToSsmDaemon (PrismSynchronousLinearSequencerContext *ctx_p);

			/* CONFIG REPLAY */
           	//void ConfigReplayStormControl (const UI32 ifType); 
			//void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);

		protected:
        public:
            StormControlLocalWorker ( SsmLocalObjectManager *pSsmLocalObjectManager);
            virtual    ~StormControlLocalWorker ();

		// Now the data members
        private:
        protected:
        public:
    };
}
#endif                                            //STORMCONTROLLOCALWORKER_H
