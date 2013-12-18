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
 *   Author : krangara                                                     *
 **************************************************************************/

#ifndef FSPFSERVICELOCALOBJECTMANAGER_H
#define FSPFSERVICELOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"

#include "FspfService/Local/BcastPriorityLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class FspfServiceGetBroadcastInfoMessage;
    class FspfServiceGetTopologyInfoMessage;
    class FspfServiceGetNeighborStateinfoInfoMessage;
    class FspfServiceUpdateBcastPriorityInfoMessage;

    class FspfServiceLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

            /* BcastShow */
            void FspfServiceGetBroadcastInfoMessageHandler( FspfServiceGetBroadcastInfoMessage *pFspfServiceGetBroadcastInfoMessage);
            ResourceId getBcastEntries (PrismSynchronousLinearSequencerContext *pFspfServiceGetBroadcastInfoMessageContext);

            /* TopologyShow */
            void FspfServiceGetTopologyInfoMessageHandler( FspfServiceGetTopologyInfoMessage *pFspfServiceGetTopologyInfoMessage);
            ResourceId getFabricTopologyInfo(PrismSynchronousLinearSequencerContext *pFspfServiceGetTopologyInfoMessageContext);

            /* nbrstateshow */
            void FspfServiceGetNeighborStateinfoInfoMessageHandler( FspfServiceGetNeighborStateinfoInfoMessage *pFspfServiceGetNeighborStateinfoInfoMessage);
            ResourceId getFabricNeighborStateInfo(PrismSynchronousLinearSequencerContext *pFspfServiceNeighborStateInfoMessageContext);

            /* BcastPriority */
            void FspfServiceUpdateBcastPriorityInfoMessageHandler( FspfServiceUpdateBcastPriorityInfoMessage *pFspfServiceUpdateBcastPriorityInfoMessage);
            void install( WaveAsynchronousContextForBootPhases * pWaveAsynchronousContextForBootPhases);
			void boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            void externalStateSynchronization(WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);
            ResourceId updateBcastPriority (PrismSynchronousLinearSequencerContext *pFspfServiceUpdateBcastPriorityInfoMessageContext);
            BcastPriorityLocalManagedObject *QueryBcastLocalManagedObject(UI32 switchId);
        protected:
        public:
            FspfServiceLocalObjectManager ();
            virtual    ~FspfServiceLocalObjectManager ();
            static FspfServiceLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

// Now the data members

        private:

        protected:
        public:
    };
}
#endif                                            //FSPFSERVICELOCALOBJECTMANAGER_H
