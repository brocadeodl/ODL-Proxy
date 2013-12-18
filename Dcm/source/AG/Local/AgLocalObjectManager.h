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
 *   Author : ranantha/rhkumar                                             *
 **************************************************************************/

#ifndef AGLOCALOBJECTMANAGER_H
#define AGLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForConfigReplayEnd.h"
#include "ClientInterface/AG/AgClientConfigAllMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class AgLocalWorker;
    class AgMappingsPluginWorker;
    class AgPGPluginWorker;

    class AgLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

            int isFactoryConfigRequired(LocationId locId); 
            void boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            void printAgLocalMO(void);
            int  downloadAgFactoryConfig(LocationId locId);
            void addPortToSortedList(string port, vector<string> &portList);
            int  updateAgMO(vector<string> policy, bool fportTrunkEnabled, LocationId locId);
            int  updateNPortMapMO(string nport, vector<string> fportList, bool failover, bool failback, LocationId locId);
            int  updatePortGroupMO(int pgId, bool pgEnabled, vector<string> pgFlags, string pgName, vector<string> nportList, vector<string> fportList, LocationId locId);
            int  pushConfigToAgDaemon(LocationId locId);
            void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);
            int  updateNPortMapToConfigAllClientMsg(AgClientConfigAllMessage *msg_p, LocationId locId);
            int  updatePGToConfigAllClientMsg(AgClientConfigAllMessage *msg_p, LocationId locId);
            void removeAgConfigFromRunningConfig(LocationId locId);
            void clearAGLocalMO(LocationId locId, bool &agDisableFirstRebootFlag);
            void clearAgNPortMapLocalMO(LocationId locId);
            void clearAgPortGroupLocalMO(LocationId locId);
            int  pushStaticMappingConfigToAgDaemon(LocationId locId);
            int  configAgDefaults(LocationId locId);
            int  configNportsDefault(bool nportFlag);


//          ResourceId updateMOsFromBackendStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
//          ResourceId getStatusFromBackendStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
//          ResourceId updateMOsFromStatusStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
//          void callInstallAfterBootComplete (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
//          void installAfterBootComplete (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);

        protected:
        public:
            AgLocalObjectManager ();
            virtual ~AgLocalObjectManager ();
            static AgLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

	        static bool compareByPort(string a, string b);
			
// Now the data members

        private:
            AgLocalWorker  *m_pAgLocalWorker;

            AgMappingsPluginWorker *m_pAgMappingsPluginWorker;

            AgPGPluginWorker *m_pAgPGPluginWorker;


        protected:
        public:
    };

    inline string  AgLocalObjectManager::getClassName()
    {
        return ("AgLocalObjectManager");
    }

}
#endif                                            //AGOBJECTMANAGER_H
