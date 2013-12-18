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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#ifndef FCOELOCALLOCALOBJECTMANAGER_H
#define FCOELOCALLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeLocalWorker;

    class FcoeLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
        protected:
        public:
            FcoeLocalObjectManager ();
            virtual    ~FcoeLocalObjectManager ();
            static FcoeLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
            ResourceId FcoeLocalPostBootFabricMapConfig();
            ResourceId FcoeLocalPostBootMapConfig();
            ResourceId FcoeLocalPostBootFipConfig();
            ResourceId FcoeLocalPostBootFsbConfig();
            ResourceId FcoeLocalPostBootFcoeVlans();
            void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase); 
            void externalStateSynchronization (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);
            void slotFailover (WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover);
            void   getFcoeEnodes(UI32 &fcoe_enodes, ResourceId  &status);
            void   fcoeEnodeSanityCheckForSystemUpgrade(UI32 &fcoe_enodes, ResourceId  &status);

            ResourceId FcoeLocalDownloadInterfaceBinding();
            ResourceId FcoeLocalDownloadETInterfaceBinding();
            bool intfCountMaxEnodeChange(void);


// Now the data members

        private:
            FcoeLocalWorker  *m_pFcoeLocalWorker;

        protected:
        public:
            bool postbootStartedInAgMode;
            WaveAsynchronousContextForPostbootPhase *pPostBootPhaseCtxAgMode;
    };

    inline string  FcoeLocalObjectManager::getClassName()
    {
        return ("FcoeLocal");
    }

}
#endif                                            //FCOELOCALLOCALOBJECTMANAGER_H
