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
 *   Author : vsatyana                                                     *
 **************************************************************************/

#ifndef SFLOWLOCALCONFIGWORKER_H
#define SFLOWLOCALCONFIGWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"
using namespace WaveNs;

namespace DcmNs
{

    class SflowLocalObjectManager;
    class SflowCreateSflowGlobalConfigSPMessage;
    class SflowDeleteSflowGlobalConfigSPMessage;
	class SflowLocalShowMessage;
    class DcmManagementInterfaceMessage;
    class SflowShowMessage;
    class SflowProfilePrismMessage;
    class SflowLocalConfigWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SflowCreateSflowGlobalConfigSPMessageHandler( SflowCreateSflowGlobalConfigSPMessage *pSflowCreateSflowGlobalConfigSPMessage);
            void SflowDeleteSflowGlobalConfigSPMessageHandler( SflowDeleteSflowGlobalConfigSPMessage *pSflowDeleteSflowGlobalConfigSPMessage);
			ResourceId createSflowLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteSflowLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			void SflowLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
			void sendShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
			ResourceId sendShowToSflowDaemon (PrismLinearSequencerContext *ctx_p);
			void SflowLocalShowMessageHandler (SflowLocalShowMessage *pSflowLocalShowMessage);
			void SflowProfileConfigMessageHandler( SflowProfilePrismMessage *pMessage);
			ResourceId sflowProfileLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId errorCode(PrismMessage* loc_msg_p, DcmManagementInterfaceMessage* c_msg_p, ResourceId status);
			ResourceId mapBackEndErrorToDcmdError(int rc);
            void SflowSendToClientsCallback (WaveSendToClientsContext *ctx_p);
             static ManagementInterfaceMessage *createManagementInterfaceMessageInstance (const UI32 &operationCode);
        protected:
        public:
            SflowLocalConfigWorker ( SflowLocalObjectManager *pSflowLocalObjectManager);
            virtual    ~SflowLocalConfigWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //SFLOWLOCALCONFIGWORKER_H
