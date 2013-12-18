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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : jrodrigu                                                     *
 **************************************************************************/

#ifndef ZONELOCALOBJECTMANAGER_H
#define ZONELOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "Framework/Core/ClusterPhase3CompleteEvent.h"
#include "dcm/fos_headers.h"

using namespace WaveNs;

namespace DcmNs
{

    class ZoneLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            
            void phase3CompleteEventHandler (const ClusterPhase3CompleteEvent* &pEvent);
            void zoneAbortedEventHandler (const VcsNodeZoneAbortedEvent *&pEvent);
			void listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        protected:
        public:
            ZoneLocalObjectManager ();
            virtual    ~ZoneLocalObjectManager ();
            static ZoneLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

// Now the data members

        private:

        protected:
        public:
    };
}
#endif //ZONELOCALOBJECTMANAGER_H
