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
 *   Author : Amitayu Das                                                     *
 **************************************************************************/

#ifndef ELDLOCALCONFWORKER_H
#define ELDLOCALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/Eld/EldLocalShowMessage.h"
#include "Nsm/Local/Eld/EldLocalClearMessage.h"
using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
	class NsmSetRxlimMessage;

    class EldLocalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmUpdateEldRxlimLocalConfigMessageHandler(NsmSetRxlimMessage *pNsmSetRxlimMessage);
            ResourceId updateEldRxlimLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            void EldShowCmdClusterCallback(WaveSendToClusterContext *ctx_p);
            ResourceId sendEldShowCommandToEldDaemon(PrismLinearSequencerContext *seq_ctx_p);
            void sendEldShowCommandToCluster(PrismLinearSequencerContext *seq_ctx_p);
            void EldLocalShowMessageHandler( EldLocalShowMessage *pEldLocalShowMessage);
            void EldClearCmdClusterCallback(WaveSendToClusterContext *ctx_p);
            ResourceId sendEldClearCommandToEldDaemon(PrismLinearSequencerContext *seq_ctx_p);
            void sendEldClearCommandToCluster(PrismLinearSequencerContext *seq_ctx_p);
            void EldLocalClearMessageHandler( EldLocalClearMessage *pEldLocalClearMessage);

        protected:
        public:
            EldLocalWorker (NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual    ~EldLocalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //ELDLOCALCONFWORKER_H
