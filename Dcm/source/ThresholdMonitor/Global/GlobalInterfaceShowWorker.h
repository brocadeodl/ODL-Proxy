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
 *   Author : Yugant Chandrakar                                            *
 **************************************************************************/

#ifndef GLOBALINTERFACESHOWWORKER_H
#define GLOBALINTERFACESHOWWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Utils/DceLinearSeqContext.h"
//#include "ThresholdMonitor/Global/InterfaceGlobalShowMessage.h"
//#include "ThresholdMonitor/Global/ThresholdMonitorGlobalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{
    class ThresholdMonitorGlobalObjectManager;
    class InterfaceGlobalShowMessage;
    class GlobalInterfaceShowWorker : public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            void InterfaceGlobalShowMessageHandler (InterfaceGlobalShowMessage *pInterfaceGlobalShowMessage);
            void sendShowInterfaceCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            void sendShowInterfaceCommandToClusterCallback (WaveSendToClusterContext *ctx_p);
        protected:
        public:
            GlobalInterfaceShowWorker (ThresholdMonitorGlobalObjectManager *pThresholdMonitorGlobalObjectManager);
            virtual ~GlobalInterfaceShowWorker ();
        private:
        protected:
        public:
    };
}

#endif

