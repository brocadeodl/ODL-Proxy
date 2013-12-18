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
 *   Author : pahuja                                                       *
 **************************************************************************/

#ifndef GLOBALINTMAILWORKER_H
#define GLOBALINTMAILWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

using namespace WaveNs;

namespace DcmNs
{

    class SystemMonitorGlobalObjectManager;
    class SystemMonitorGlobalIntMailAddEmailMessage;
    class SystemMonitorGlobalIntMailRemoveEmailMessage;
    class SystemMonitorGlobalIntMailSetCfgMessage;

    class GlobalIntMailWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SystemMonitorGlobalIntMailAddEmailMessageHandler( SystemMonitorGlobalIntMailAddEmailMessage *pSystemMonitorGlobalIntMailAddEmailMessage);
            void addEmailSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void SystemMonitorGlobalIntMailRemoveEmailMessageHandler( SystemMonitorGlobalIntMailRemoveEmailMessage *pSystemMonitorGlobalIntMailRemoveEmailMessage);
            void removeEmailSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void SystemMonitorGlobalIntMailSetCfgMessageHandler( SystemMonitorGlobalIntMailSetCfgMessage *pSystemMonitorGlobalIntMailSetCfgMessage);
            void setEmailFlagSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
        protected:
        public:
            GlobalIntMailWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager);
            virtual    ~GlobalIntMailWorker ();


// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //GLOBALINTMAILWORKER_H
