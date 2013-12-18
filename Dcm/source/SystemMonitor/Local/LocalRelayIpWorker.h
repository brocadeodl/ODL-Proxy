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

#ifndef LOCALRELAYIPWORKER_H
#define LOCALRELAYIPWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class SystemMonitorLocalObjectManager;
    class RelayIpLocalConfigMessage;
    class RelayIpLocalSetMessage;
    class RelayIpLocalDelMessage;

    class LocalRelayIpWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            void RelayIpLocalConfigMessageHandler( RelayIpLocalConfigMessage *pRelayIpLocalConfigMessage);
            void RelayIpLocalSetMessageHandler( RelayIpLocalSetMessage *pRelayIpLocalSetMessage);
            void RelayIpLocalDelMessageHandler( RelayIpLocalDelMessage *pRelayIpLocalDelMessage);
			/*
            void setContactSTCCB(WaveNs::WaveSendToClusterContext *pWaveSendToClusterContext);
            void setLocationSTCCB(WaveNs::WaveSendToClusterContext *pWaveSendToClusterContext);
            void setsysDescrSTCCB(WaveNs::WaveSendToClusterContext *pWaveSendToClusterContext);
			*/
        protected:
        public:
            LocalRelayIpWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager);
            virtual    ~LocalRelayIpWorker ();
			void fwBackendRelay ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //LOCALRELAYIPWORKER_H
