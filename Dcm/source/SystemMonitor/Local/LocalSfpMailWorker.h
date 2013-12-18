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

#ifndef LOCALSFPMAILWORKER_H
#define LOCALSFPMAILWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"

using namespace WaveNs;

namespace DcmNs
{

    class SystemMonitorLocalObjectManager;
    class SystemMonitorLocalSfpMailSetEmailMessage;
    class SystemMonitorLocalSfpMailSetCfgMessage;

    class LocalSfpMailWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SystemMonitorLocalSfpMailSetCfgMessageHandler( SystemMonitorLocalSfpMailSetCfgMessage *pSystemMonitorLocalSfpMailSetCfgMessage);
            void SystemMonitorLocalSfpMailSetEmailMessageHandler( SystemMonitorLocalSfpMailSetEmailMessage *pSystemMonitorLocalSfpMailSetEmailMessage);
        protected:
        public:
            LocalSfpMailWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager);
            virtual    ~LocalSfpMailWorker ();
            ResourceId pushBackend();
            void boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            int fwBackendOpSfpMail ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //LOCALSFPMAILWORKER_H
