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
 *   Author : ugodavar                                                     *
 **************************************************************************/

#ifndef SPANLOCALCONFIGWORKER_H
#define SPANLOCALCONFIGWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Utils/DceSynchronousLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

    class SpanLocalLocalObjectManager;
    class SpanLocalSpanLocalConfigMessage;
	class SpanLocalShowMessage;
    class DcmManagementInterfaceMessage;

    class SpanLocalConfigWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SpanLocalSpanLocalConfigMessageHandler( SpanLocalSpanLocalConfigMessage *pSpanLocalSpanLocalConfigMessage);
			ResourceId validateConfigMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId ConfigMessageStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			void SpanLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            void sendShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            ResourceId sendShowToSpanDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void SpanLocalShowMessageHandler (SpanLocalShowMessage *pSpanLocalShowMessage);
//            ResourceId errorCode(PrismMessage* loc_msg_p, DcmManagementInterfaceMessage* c_msg_p, ResourceId status);
			ResourceId mapBackEndErrorToDcmdError(int rc);
        protected:
        public:
            SpanLocalConfigWorker ( SpanLocalLocalObjectManager *pSpanLocalLocalObjectManager);
            virtual    ~SpanLocalConfigWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //SPANLOCALCONFIGWORKER_H
