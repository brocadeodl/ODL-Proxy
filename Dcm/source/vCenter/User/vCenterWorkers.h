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
 *   Author : sharis                                                     *
 **************************************************************************/

#ifndef VCENTERWORKERS_H
#define VCENTERWORKERS_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "vCenter/Framework/vCenterLocalMessages.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "VcsFabric/Local/VcsNodeDomainIdMessages.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "vCenter/Framework/vCenterManagedObjects.h"
#include "Framework/Core/WaveNewNodesAddedEvent.h"
#include "Framework/Core/PrimaryChangedEvent.h"
#include "Framework/Core/NodeReadyForAllCommandsEvent.h"

using namespace WaveNs;

namespace DcmNs
{

    class vCenterObjectManager;
    class vCenterCreateVcenterMessage;
    class vCenterUpdateVcenterMessage;
    class vCenterDeleteVcenterMessage;

    class VcenterWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

            void vCenterCreateVcenterMessageHandler( vCenterCreateVcenterMessage *pvCenterCreateVcenterMessage);
            void vCenterDeleteVcenterMessageHandler( vCenterDeleteVcenterMessage *pvCenterDeleteVcenterMessage);
            void vCenterUpdateVcenterMessageHandler( vCenterUpdateVcenterMessage *pvCenterUpdateVcenterMessage);

            void doGlobalConfigStep   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void doGlobalUpdateStep   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void doGlobalDeconfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void ppDeconfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);

            void startDiscoveryOnBootup (const NodeReadyForAllCommandsEvent* &pEvent);
            void nodeAddedEventHandler(const WaveNewNodesAddedEvent* &pEvent);
            void primaryChangedEventHandler (const PrimaryChangedEvent* &pEvent);
            void sendMsgToDiscoverPlugin (PrismLinearSequencerContext *pPrismLinearSequencerContext);

            void GlobalSendToClusterCallback(WaveSendToClusterContext  *ctx_p);
            void ClusterGetFirstFailureLocation(WaveSendToClusterContext  *ctx_p , LocationId &locationid);

            void createManagedObject (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void updateManagedObject (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deleteManagedObject (PrismLinearSequencerContext *pPrismLinearSequencerContext);

            void createConfigPhpFile(vCenterVcenterMessage *m);
			void createFreshConfigFile(VcenterManagedObject *pMO, LocationId locationid);
            void cleanPersistedDBFiles();
			int fileexists(const char *fname);
            void cleanupDBfiles();

        protected:
        public:
            void listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            VcenterWorker ( vCenterObjectManager *pvCenterObjectManager);
            virtual    ~VcenterWorker ();

// Now the data members

        private:
        protected:
        public:
    };

}
#endif                                            // VCENTERWORKERS_H


