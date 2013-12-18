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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#ifndef IGMPSLOCALCONFIGWORKER_H
#define IGMPSLOCALCONFIGWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Igmps/Local/IgmpsLocalShowMessage.h"
#include "Igmps/Local/IgmpsLocalDebugMessage.h"
#include "Igmps/Local/IgmpsLocalClearMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class IgmpsLocalObjectManager;
    class IgmpsUpdateIgmpsConfigMessage;

    class IgmpsLocalConfigWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

            void IgmpsUpdateIgmpsConfigMessageHandler( IgmpsUpdateIgmpsConfigMessage *pIgmpsUpdateIgmpsConfigMessage);
            void IgmpsLocalShowMessageHandler (IgmpsLocalShowMessage *pIgmpsLocalShowMessage);
            void IgmpsLocalDebugMessageHandler (IgmpsLocalDebugMessage *pIgmpsLocalDebugMessage);
            void IgmpsLocalClearMessageHandler (IgmpsLocalClearMessage *pIgmpsLocalClearMessage);

            ResourceId updateIgmpsLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId sendShowToIgmpsDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            ResourceId sendDebugToIgmpsDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void sendShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            void IgmpsLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            ResourceId sendClearToIgmpsDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void sendClearCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            void IgmpsLocalClearSendToClusterCallback (WaveSendToClusterContext *ctx_p);
        protected:
        public:
            IgmpsLocalConfigWorker ( IgmpsLocalObjectManager *pIgmpsLocalObjectManager);
            virtual    ~IgmpsLocalConfigWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //IGMPSLOCALCONFIGWORKER_H
