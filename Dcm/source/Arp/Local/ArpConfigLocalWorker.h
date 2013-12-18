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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : vkarnati                                                     *
 **************************************************************************/

#ifndef ARPCONFIGLOCALWORKER_H
#define ARPCONFIGLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class ArpLocalObjectManager;
    class ArpStaticConfigLocalMessage;
    class ArpConfigLocalMessage;
    class ArpResetLocalMessage;
    class ArpClearLocalMessage;
    class ArpShowLocalMessage;
    class ArpDebugLocalMessage;

    class ArpConfigLocalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void ArpClearLocalMessageHandler( ArpClearLocalMessage *pArpClearLocalMessage);
            void ArpConfigLocalMessageHandler( ArpConfigLocalMessage *pArpConfigLocalMessage);
            ResourceId updateArpStaticConfigMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateArpStaticConfigStep (ArpStaticConfigLocalMessage *pArpStaticConfigLocalMessage);
            ResourceId sendDebugToArpDaemon(PrismSynchronousLinearSequencerContext *ctx_p);
            void ArpDebugLocalMessageHandler( ArpDebugLocalMessage *pArpDebugLocalMessage);

            void ArpResetLocalMessageHandler( ArpResetLocalMessage *pArpResetLocalMessage);
            void ArpShowLocalMessageHandler( ArpShowLocalMessage *pArpShowLocalMessage);
            void ArpStaticConfigLocalMessageHandler( ArpStaticConfigLocalMessage *pArpStaticConfigLocalMessage);
            ResourceId updateArpConfigMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateArpConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
      			int32_t getSlotFromIfName (UI32 ifType, string ifName);
            void ArpShowCmdClusterCallback(WaveSendToClusterContext *ctx_p);
            ResourceId sendArpShowCommandToArpDaemon(PrismLinearSequencerContext *seq_ctx_p);
            void sendArpShowToCommandToCluster(PrismLinearSequencerContext *seq_ctx_p);
            ResourceId ArpClearHandler(PrismSynchronousLinearSequencerContext *ctx_p);
            ResourceId validateArpStaticConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId validateIfName (ArpStaticConfigLocalMessage *pArpStaticConfigLocalMessage);
        protected:
        public:
            ArpConfigLocalWorker ( ArpLocalObjectManager *pArpLocalObjectManager);
            virtual    ~ArpConfigLocalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //ARPCONFIGLOCALWORKER_H
