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
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef GLOBALRELAYIPWORKER_H
#define GLOBALRELAYIPWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class SystemMonitorGlobalObjectManager;
    class RelayIpConfigMessage;
    class RelayIpSetMessage;
    class RelayIpDelMessage;

    class GlobalRelayIpWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void RelayIpConfigMessageHandler( RelayIpConfigMessage *pRelayIpConfigMessage);
            void RelayIpSetMessageHandler( RelayIpSetMessage *pRelayIpSetMessage);
            void RelayIpDelMessageHandler( RelayIpDelMessage *pRelayIpDelMessage);
            void setRelaySTCCB(WaveNs::WaveSendToClusterContext *pWaveSendToClusterContext);
            void addRelaySTCCB(WaveNs::WaveSendToClusterContext *pWaveSendToClusterContext);
            void delRelaySTCCB(WaveNs::WaveSendToClusterContext *pWaveSendToClusterContext);
        protected:
        public:
            GlobalRelayIpWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager);
            virtual    ~GlobalRelayIpWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //GLOBALRELAYIPWORKER_H