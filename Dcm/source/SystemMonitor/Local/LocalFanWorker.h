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

#ifndef LOCALFANWORKER_H
#define LOCALFANWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include <thresh/fwdPub.h>
#include <thresh/fwd.h>
using namespace WaveNs;

namespace DcmNs
{

    class SystemMonitorLocalObjectManager;
    class SystemMonitorLocalChangeFanThresholdMessage;
    class SystemMonitorLocalChangeFanAlertMessage;

    class LocalFanWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SystemMonitorLocalChangeFanAlertMessageHandler( SystemMonitorLocalChangeFanAlertMessage *pSystemMonitorLocalChangeFanAlertMessage);
            void SystemMonitorLocalChangeFanThresholdMessageHandler( SystemMonitorLocalChangeFanThresholdMessage *pSystemMonitorLocalChangeFanThresholdMessage);
            void  externalStateSynchronization (WaveAsynchronousContextForExternalStateSynchronization
                    *pWaveAsynchronousContextForExternalStateSynchronization);
			void install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            UI32 DcmtoFwAction (UI32 actionValue);
            UI32 DcmtoFwState (UI32 stateValue);
            
        protected:
        public:
            LocalFanWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager);
            virtual    ~LocalFanWorker ();
            int  fwBackendOpFanAlert (fw_fruObj_t *fruCfg);
            int fwBackendOpFanThreshold (swPolicyList_t *pList);
			UI32 fwtoDcmState (UI32 stateValue);
			UI32 fwtoDcmAction (UI32 actionValue);
// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //LOCALFANWORKER_H