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
 *   Author : vivekk													   *
 **************************************************************************/

#ifndef RMONLOCALOBJECTMANAGER_H
#define RMONLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class RmonCreateRmonEventSPMessage;
    class RmonDeleteRmonEventSPMessage;
    class RmonCreateRmonAlarmSPMessage;
    class RmonDeleteRmonAlarmSPMessage;
	class DcmManagementInterfaceMessage;
    class RmonLocalShowMessage;
	class RmonClearMessage;

    class RmonLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void RmonCreateRmonEventSPMessageHandler( RmonCreateRmonEventSPMessage *pRmonCreateRmonEventSPMessage);
            void RmonDeleteRmonEventSPMessageHandler( RmonDeleteRmonEventSPMessage *pRmonDeleteRmonEventSPMessage);
            void RmonCreateRmonAlarmSPMessageHandler( RmonCreateRmonAlarmSPMessage *pRmonCreateRmonAlarmSPMessage);
            void RmonDeleteRmonAlarmSPMessageHandler( RmonDeleteRmonAlarmSPMessage *pRmonDeleteRmonAlarmSPMessage);
			ResourceId createRmonEventLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteRmonEventLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId createRmonAlarmLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteRmonAlarmLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId mapBackEndErrorToDcmdError(int rc);
			ResourceId errorCode(PrismMessage *pMsg, DcmManagementInterfaceMessage *clientMsg, ResourceId status);
			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);
			void configReplayRmonAlarm();
			void configReplayRmonEvent();
            void
            RmonLocalShowMessageHandler (RmonLocalShowMessage *pRmonLocalShowMessage);
            void 
            RmonLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            void
            sendShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            ResourceId
            sendShowToRmonDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
			void RmonLocalClearMessageHandler(RmonClearMessage *pRmonClearMessage);
			void sendRmonClearCommandToCluster( PrismLinearSequencerContext *seq_ctx_p);
			void RmonLocalClearSendToClusterCallback ( WaveSendToClusterContext *ctx_p);
			ResourceId sendClearToRmonDaemon( PrismSynchronousLinearSequencerContext *ctx_p);


        protected:
        public:
            RmonLocalObjectManager ();
            virtual    ~RmonLocalObjectManager ();
            static RmonLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

// Now the data members

        private:

        protected:
        public:
    };
}
#endif                                            //RMONLOCALOBJECTMANAGER_H
