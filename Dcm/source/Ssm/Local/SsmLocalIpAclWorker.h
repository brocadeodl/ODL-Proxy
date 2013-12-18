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
 *   Author : bpatel                                                     *
 **************************************************************************/

#ifndef SSMLOCALIPACLWORKER_H
#define SSMLOCALIPACLWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"
using namespace WaveNs;

namespace DcmNs
{

    class SsmLocalObjectManager;
    class SsmLocalServiceAssocIpRuleToPolicyMessage;
    class SsmLocalServiceAddIpAclPolicyMessage;
    class SsmLocalServiceRemIpAclPolicyMessage;
	class SsmLocalServiceResqIpAclPolicyMessage;
	class SsmMacIpAclLocalClearMessage;
	class SsmMacIpAclLocalShowMessage;
	class DcmManagementInterfaceMessage;
	class PhyIntfLocalManagedObject;
	class SsmIpAclIntfAssocBulkMessage;
    class SsmLocalIpAclWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SsmLocalServiceAddIpAclPolicyMessageHandler(SsmLocalServiceAddIpAclPolicyMessage *pSsmLocalServiceAddIpAclPolicyMessage);
            void SsmLocalServiceAssocIpRuleToPolicyMessageHandler(SsmLocalServiceAssocIpRuleToPolicyMessage *pSsmLocalServiceAssocIpRuleToPolicyMessage);
            void SsmLocalServiceRemIpAclPolicyMessageHandler(SsmLocalServiceRemIpAclPolicyMessage *pSsmLocalServiceRemIpAclPolicyMessage);
        	void SsmLocalServiceResqIpAclPolicyMessageHandler(SsmLocalServiceResqIpAclPolicyMessage *pSsmLocalServiceResqIpAclPolicyMessage);
            /* Show Cmd Prototypes */
			void SsmMacIpAclLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            void sendShowMacIpCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            ResourceId sendShowMacIpToSsmDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void SsmMacIpAclLocalShowMessageHandler (SsmMacIpAclLocalShowMessage *pSsmMacIpAclLocalShowMessage);
            void SsmMacIpAclLocalClearMessageHandler (SsmMacIpAclLocalClearMessage *pSsmMacIpAclLocalClearMessage);
            void SsmMacIpAclLocalClearSendToClusterCallback (WaveSendToClusterContext *ctx_p);
            void sendMacIpAclClearCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            ResourceId sendClearToSsmDaemon (PrismSynchronousLinearSequencerContext *ctx_p);

            void ConfigReplayIpAclPolicy();
            void ConfigReplayStdIpAclPolicy();    
            void ConfigReplayExtIpAclPolicy();
			void ConfigReplayIpAclIntf(SI32 slotNum);
           	void ConfigReplayIpAclGlobalIntf(const UI32 ifType); 
			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);
			void setLocationIdsForActiveVlanId(const ObjectId vlanObjectId, UI32 vlanId, vector<LocationId> &locationIds);
			ResourceId getResourceIdFromClientStatus(const SI32 &clientStatus);
			ResourceId getErrorStatusPerPlatform(ResourceId status);

		protected:
        public:
            SsmLocalIpAclWorker ( SsmLocalObjectManager *pSsmLocalObjectManager);
            virtual    ~SsmLocalIpAclWorker ();
			static SsmIpAclIntfAssocBulkMessage *ConfigReplayIpAclGlobalIntfAssoc(const UI32 ifType, WaveManagedObject *pWaveManagedObject);

// Now the data members

        private:
			const static SI32 SSM_ERR_ACL_NOT_APPLIED_TMP = 140;
			const static SI32 SSM_ERR_ACL_CMD_FAIL_RETRY_TMP = 5;

        protected:
        public:
    };
}
#endif                                            //SSMLOCALIPACLWORKER_H
