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
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef SYSTEMMONITORLOCALOBJECTMANAGER_H
#define SYSTEMMONITORLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosDomainEvents.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForConfigReplayEnd.h"
#include "Framework/Core/ConfigReplayStartedEvent.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include <thresh/fwdPub.h>
#include <thresh/fwd.h>
using namespace WaveNs;

namespace DcmNs
{

    class ShowMonitorWorker;
    class LocalFanWorker;
    class LocalPowerWorker;
    class LocalTempWorker;
    class LocalWwnCardWorker;
    class LocalSfpWorker;
    class LocalCompactFlashWorker;
    class LocalFaultyPortsWorker;
    class LocalMissingSfpWorker;
    class LocalMMWorker;
    class LocalLineCardWorker;
    class LocalSfmWorker;
    class LocalFruMailWorker;
	class LocalSecMailWorker;
    class LocalSfpMailWorker;
    class LocalIntMailWorker;
    class LocalRelayIpWorker;

    class SystemMonitorLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			void boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
			void initManagedObjects();
        protected:
        public:
            SystemMonitorLocalObjectManager ();
            virtual    ~SystemMonitorLocalObjectManager ();
            static SystemMonitorLocalObjectManager *getInstance();
            static string getClassName();
            void setFlag (bool flagValue);
            bool getFlag ();
            static PrismServiceId getPrismServiceId();
			void configReplayEndEventHandler (const ConfigReplayStartedEvent* &pEvent);
			void listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
			int backendSync ();
			int backendRelaySync ();
            fw_fruObj_t m_defFruCfg;
            swPolicyList_t m_defpList;
            
// Now the data members

        private:
            ShowMonitorWorker  *m_pShowMonitorWorker;
            LocalFanWorker  *m_pLocalFanWorker;
            LocalPowerWorker  *m_pLocalPowerWorker;
            LocalTempWorker  *m_pLocalTempWorker;
            LocalWwnCardWorker  *m_pLocalWwnCardWorker;
            LocalSfpWorker  *m_pLocalSfpWorker;
            LocalCompactFlashWorker  *m_pLocalCompactFlashWorker;
            LocalFaultyPortsWorker  *m_pLocalFaultyPortsWorker;
            LocalMissingSfpWorker  *m_pLocalMissingSfpWorker;
            LocalMMWorker  *m_pLocalMMWorker;
            LocalLineCardWorker  *m_pLocalLineCardWorker;
            LocalSfmWorker  *m_pLocalSfmWorker;
            LocalFruMailWorker  *m_pLocalFruMailWorker;
			LocalSecMailWorker  *m_pLocalSecMailWorker;
            LocalSfpMailWorker  *m_pLocalSfpMailWorker;
            LocalIntMailWorker  *m_pLocalIntMailWorker;
            LocalRelayIpWorker  *m_pLocalRelayIpWorker;
            bool            m_flag;
//            void listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
  //          void domainValidEventHandler(const VcsNodeFabosDomainValidEvent *&pEvent);
            void externalStateSynchronization (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);
        protected:
        public:
            friend class VcsNodeFabosInterfaceObjectManager;
    };
}
#endif                                            //SYSTEMMONITORLOCALOBJECTMANAGER_H
