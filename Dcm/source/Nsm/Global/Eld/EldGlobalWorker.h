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
 *   Author : Amitayu Das                                                  *
 **************************************************************************/

#ifndef ELDGLOBALWORKER_H
#define ELDGLOBALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Global/Eld/NsmGlobalEldUpdateConfigMessage.h"

using namespace WaveNs;

namespace DcmNs
{
	class NsmSetRxlimMessage;
    class NsmGlobalObjectManager;
    class NsmGlobalEldUpdateConfigMessage;
    class NsmGlobalSetRxlimMessage;

    class EldGlobalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmGlobalEldUpdateConfigMessageHandler(NsmGlobalEldUpdateConfigMessage *pNsmGlobalEldUpdateConfigMessage);
			void updateEldRxlimStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void updateEldGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void NsmGlobalEldRxlimConfigMessageHandler(NsmGlobalSetRxlimMessage *pNsmGlobalSetRxlimMessage);
			void EldConfGlobalSendToClusterCallback (WaveSendToClusterContext  *ctx_p);
        protected:
        public:
            EldGlobalWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager);
            virtual    ~EldGlobalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //ELDGLOBALWORKER_H
