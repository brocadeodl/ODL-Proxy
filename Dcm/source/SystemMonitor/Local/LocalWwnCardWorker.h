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

#ifndef LOCALWWNCARDWORKER_H
#define LOCALWWNCARDWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include <thresh/fwdPub.h>
#include <thresh/fwd.h>

using namespace WaveNs;

namespace DcmNs
{

    class SystemMonitorLocalObjectManager;
    class SystemMonitorLocalChangeWwnCardThresholdMessage;
    class SystemMonitorLocalChangeWwnCardAlertMessage;

    class LocalWwnCardWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SystemMonitorLocalChangeWwnCardAlertMessageHandler( SystemMonitorLocalChangeWwnCardAlertMessage *pSystemMonitorLocalChangeWwnCardAlertMessage);
            void SystemMonitorLocalChangeWwnCardThresholdMessageHandler( SystemMonitorLocalChangeWwnCardThresholdMessage *pSystemMonitorLocalChangeWwnCardThresholdMessage);
	        UI32 DcmtoFwAction (UI32 actionValue);
            UI32 DcmtoFwState (UI32 stateValue);
        protected:
        public:
            LocalWwnCardWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager);
            virtual    ~LocalWwnCardWorker ();
            int  fwBackendOpWwnCardAlert (fw_fruObj_t *fruCfg);
            int fwBackendOpWwnCardThreshold (swPolicyList_t *pList);
			UI32 fwtoDcmState (UI32 stateValue);
			UI32 fwtoDcmAction (UI32 actionValue);
// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //LOCALWWNCARDWORKER_H
