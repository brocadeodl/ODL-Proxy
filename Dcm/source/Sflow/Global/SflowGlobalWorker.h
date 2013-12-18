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
 *   Author : vsatyana                                                     *
 **************************************************************************/

#ifndef SFLOWGLOBALWORKER_H
#define SFLOWGLOBALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Utils/DceLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

    class SflowGlobalObjectManager;
    class SflowGlobalCreateSflowGlobalConfigMessage;
    class SflowGlobalDeleteSflowGlobalConfigMessage;
    class SflowGlobalSflowCollectorMessage;
    class SflowGlobalSflowProfileMessage;

    class SflowGlobalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SflowGlobalCreateSflowGlobalConfigMessageHandler( SflowGlobalCreateSflowGlobalConfigMessage *pSflowGlobalCreateSflowGlobalConfigMessage);
            void SflowGlobalDeleteSflowGlobalConfigMessageHandler( SflowGlobalDeleteSflowGlobalConfigMessage *pSflowGlobalDeleteSflowGlobalConfigMessage);
	    void createSflowGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            ResourceId createSflowGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
	    void deleteSflowGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            ResourceId deleteSflowGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);

            void SflowGlobalSflowCollectorMessageHandler( SflowGlobalSflowCollectorMessage *pMessage);
            ResourceId SflowCollectorValidateStep (DceLinearSeqContext *pPrismLinearSequencerContext);
            void SflowCollectorUpdateStep (DceLinearSeqContext *pPrismLinearSequencerContext);
            ResourceId SflowCollectorUpdateMOStep (DceLinearSeqContext *pPrismLinearSequencerContext);

            void SflowGlobalSflowProfileMessageHandler( SflowGlobalSflowProfileMessage *pMessage);
            ResourceId SflowProfileValidateStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void SflowProfileUpdateStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            ResourceId SflowProfileUpdateMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);

            void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);


	    void SflowGlobalSendToClusterCallback (WaveSendToClusterContext *ctx_p);
        protected:
        public:
            SflowGlobalWorker ( SflowGlobalObjectManager *pSflowGlobalObjectManager);
            virtual    ~SflowGlobalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //SFLOWGLOBALWORKER_H
