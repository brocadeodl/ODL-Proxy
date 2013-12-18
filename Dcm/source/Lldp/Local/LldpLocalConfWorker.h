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
 *   Author : mmohan                                                     *
 **************************************************************************/

#ifndef LLDPLOCALCONFWORKER_H
#define LLDPLOCALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class LldpLocalObjectManager;
    class LldpScalarsConfSetMessage;
    class LldpProfileConfSetMessage;
    class LldpLocalShowMessage;
    class LldpLocalClearMessage;
    class LldpLocalLldpDebugMessage;

    class LldpLocalConfWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance (const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance 
                (const string &managedClassName);
            void 
            LldpProfileConfSetMessageHandler (
                    LldpProfileConfSetMessage *pLldpProfileConfSetMessage);
            void 
            LldpScalarsConfSetMessageHandler (
                    LldpScalarsConfSetMessage *pLldpScalarsConfSetMessage);
            void
            LldpLocalShowMessageHandler (LldpLocalShowMessage *pLldpLocalShowMessage);
            
			void
            LldpLocalClearMessageHandler (LldpLocalClearMessage *pLldpLocalClearMessage);

            void
            LldpLocalLldpDebugMessageHandler (LldpLocalLldpDebugMessage *pLldpLocalLldpDebugMessage);
            
            void 
            LldpLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);

            void 
            LldpLocalClearSendToClusterCallback (WaveSendToClusterContext *ctx_p);

            void 
            LldpLocalLldpDebugSendToClusterCallback (WaveSendToClusterContext *ctx_p);

            void
            sendShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);

            void
            sendClearCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);

            void
            sendLldpDebugCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);

            ResourceId
            sendShowToLldpDaemon (PrismSynchronousLinearSequencerContext *ctx_p);

            ResourceId
            sendClearToLldpDaemon (PrismSynchronousLinearSequencerContext *ctx_p);

            ResourceId
            sendLldpDebugToLldpDaemon (PrismSynchronousLinearSequencerContext *ctx_p);

        protected:
        public:
                LldpLocalConfWorker ( LldpLocalObjectManager *pLldpLocalObjectManager);
                virtual    ~LldpLocalConfWorker ();

                // Now the data members
                ResourceId updateLldpScalarsConf (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                ResourceId updateLldpProfileConf (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                ResourceId updateLldpScalarsConfMO (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                ResourceId updateLldpProfileConfMO (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

        private:
        protected:
        public:
    };
}
#endif                                            //LLDPLOCALCONFWORKER_H
