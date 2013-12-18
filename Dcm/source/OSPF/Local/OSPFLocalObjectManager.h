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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef OSPFLOCALOBJECTMANAGER_H
#define OSPFLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Vrf/Local/VrfUtils.h"
using namespace WaveNs;

namespace DcmNs
{

    class OSPFLocalConfWorker;
	class OSPFInterfaceLocalManagedObject;

    class OSPFLocalObjectManager: public MultiPartitionCleanup
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
        protected:
        public:
            OSPFLocalObjectManager ();
            virtual    ~OSPFLocalObjectManager ();
            static OSPFLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);
			void configReplayGlobalConfig(LocationId locId);
			void configReplayVlanIntfConfig(LocationId locId);
			void configReplayPoIntfConfig(LocationId locId);
			void configReplayPhyIntfConfig(SI32 slotId, LocationId locId);
			void configReplayLoopbackIntfConfig(LocationId locId);
			void configReplayIntfConfig(OSPFInterfaceLocalManagedObject *pOSPFIntfMO);
			void clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
			void clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
                        static  vector<string> getObjectToDelete();

// Now the data members

        private:
            OSPFLocalConfWorker  *m_pOSPFLocalConfWorker;

        protected:
        public:
    };
}
#endif                                            //OSPFLOCALOBJECTMANAGER_H
