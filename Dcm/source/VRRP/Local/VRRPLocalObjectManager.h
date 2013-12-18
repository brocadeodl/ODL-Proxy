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
 *   Author : krao                                                     *
 **************************************************************************/

#ifndef VRRPLOCALOBJECTMANAGER_H
#define VRRPLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Vrf/Local/VrfUtils.h"
using namespace WaveNs;

namespace DcmNs
{

    class VRRPLocalConfigWorker;
	class VRRPSessionConfigLocalManagedObject;
	class VRRPESessionConfigLocalManagedObject;
	class PhyIntfLocalManagedObject;

    class VRRPLocalObjectManager: public MultiPartitionCleanup
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void ConfigReplayVRRPGlobalConfig();
			void ConfigReplayVRRPVlanIntfConfig(LocationId locId);
			void ConfigReplayVRRPPoIntfConfig(LocationId locId);
            void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);
        protected:
        public:
            VRRPLocalObjectManager ();
            virtual    ~VRRPLocalObjectManager ();
            static VRRPLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
			void configReplayVRRPPhyIntf(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void ConfigReplayVRRPInterfaceConfig(vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> > &vrrpSessionConfigVector);
			void ConfigReplayVRRPEInterfaceConfig(vector<WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> > &vrrpeSessionConfigVector);
			static  vector<string> getObjectToDelete();
// Now the data members

        private:
            VRRPLocalConfigWorker  *m_pVRRPLocalConfigWorker;

        protected:
        public:
    };
}
#endif                                            //VRRPLOCALOBJECTMANAGER_H
