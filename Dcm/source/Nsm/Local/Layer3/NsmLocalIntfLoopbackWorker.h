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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Justin Huang                                                 *
 **************************************************************************/

#ifndef NSMLOCALINTFLOOPBACKWORKER_H
#define NSMLOCALINTFLOOPBACKWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/Layer3/NsmIntfLoopbackManagedObject.h"
#include "Nsm/Local/Layer3/NsmLocalIntfLoopbackMessage.h"
#include "Utils/DceSynchronousLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{
    class NsmLocalObjectManager;

    class NsmLocalIntfLoopbackWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance (const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance (
                const string &managedClassName);

            void NsmLocalIntfLoopbackMessageHandler (NsmLocalIntfLoopbackMessage 
                *pMessage);

            void LoopBackIntfVrfConfigHandler (NsmLocalIntfLoopbackMessage *pMessage);
            void LoopBackIntfConfigHandler (NsmLocalIntfLoopbackMessage *pMessage);
             

            ResourceId validateIntfLoopbackVrfConfigStep(DceSynchronousLinearSeqContext *pContext);
            ResourceId loopbackVrfConfigBackendStep(DceSynchronousLinearSeqContext *pContext);
            ResourceId updateVrfConfigMOStep (DceSynchronousLinearSeqContext *pContext);
			ResourceId validateIntfLoopbackStep (
                PrismSynchronousLinearSequencerContext *pContext);
			ResourceId doIntfLoopbackStep (
                PrismSynchronousLinearSequencerContext *pContext);
			ResourceId doIntfLoopbackMOStep (
                PrismSynchronousLinearSequencerContext *pContext);
            ResourceId updateErrorCodeStep(DceSynchronousLinearSeqContext *pContext);

        protected:
        public:
            NsmLocalIntfLoopbackWorker (NsmLocalObjectManager 
                *pWaveObjectManager);
            virtual  ~NsmLocalIntfLoopbackWorker ();

            NsmIntfLoopbackManagedObject* findIntfLoopbackMO(UI32 ifId,
                const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext *pContext);

// Now the data members

        private:
    };

}
#endif                                            //NSMLOCALINTFLOOPBACKWORKER_H

