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
 *   Author : Bhavin                                                     *
 **************************************************************************/

#ifndef SSMIPACLWORKER_H
#define SSMIPACLWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Utils/DceLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalObjectManager;
    class SsmGlobalAddIpAclPolicyMessage;
    class SsmGlobalRemIpAclPolicyMessage;
    class SsmGlobalAssocIpRuleToPolicyMessage;
	class SsmGlobalReseqIpAclPolicyMessage;
	class SsmGlobalGetIpAclPolicyMessage;
	class SsmIpAclWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SsmGlobalAddIpAclPolicyMessageHandler( SsmGlobalAddIpAclPolicyMessage *pSsmGlobalAddIpAclPolicyMessage);
            void SsmGlobalAssocIpRuleToPolicyMessageHandler (SsmGlobalAssocIpRuleToPolicyMessage *pSsmGlobalAssocIpRuleToPolicyMessage);
            void SsmGlobalRemIpAclPolicyMessageHandler( SsmGlobalRemIpAclPolicyMessage *pSsmGlobalRemIpAclPolicyMessage);
            void createIpAclPolicy(PrismLinearSequencerContext *seqContext);
            void deleteIpAclPolicy(PrismLinearSequencerContext *seqContext);
            void createIpAclPolicyMO(PrismLinearSequencerContext *seqContext);
            void disAssocIpAclRuleToPolicy(DceLinearSeqContext *seqContext);
            void assocIpAclRuleToPolicy(DceLinearSeqContext *seqContext);
            void udpateIpAclRuleToPolicy(PrismLinearSequencerContext *seqContext);
            void deleteIpAclPolicyMO(DceLinearSeqContext *seqContext);
            void QuerychildValidation(WaveManagedObjectSynchronousQueryContext &queryCtxtForChild, ResourceId &status, bool &ruleFound, bool &seqFound, UI32 opcode, UI32 UserEnteredSeqid, UI64 &seqid, UI32 policyType);
			void validateRule(DceLinearSeqContext *seqContext);
            void ssmIpAclPolicySendToClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void ssmIpAclRuleSendToClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void validatePolicy(PrismLinearSequencerContext *seqContext);
            void validateDeletePolicy(DceLinearSeqContext *seqContext);
            ResourceId getIpAclPolicyType(const string &policyName, ObjectId &policyObjId, UI32 &policyType);
            void deleteAssociationwithDisconnectedPolicy (PrismLinearSequencerContext *seqContext);
			void deleteOldIpAclPolicyMO(PrismLinearSequencerContext *seqContext);
			void SsmGlobalReseqIpAclPolicyMessageHandler(SsmGlobalReseqIpAclPolicyMessage *pSsmGlobalReesqIpAclPolicyMessage);
       		void resequenceIpAclPolicy(PrismLinearSequencerContext *seqContext);
            void updateresequenceIpAclPolicy(PrismLinearSequencerContext *seqContext);
            void ssmIpAclReseqPolicySendToClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext); 
			void SsmGlobalGetIpAclPolicyMessageHandler(SsmGlobalGetIpAclPolicyMessage *pSsmGlobalGetIpAclPolicyMessage);
			UI32 ssmIsipPolicyAttachInterface(const string& policy, DceLinearSeqContext *seqContext);
			int isIpPolicyAttachToMgmtInterface(const string& policyName);
		protected:
        public:
            SsmIpAclWorker ( SsmGlobalObjectManager *pSsmGlobalObjectManager);
            virtual    ~SsmIpAclWorker ();

        private:
        protected:
        public:
    };
}
#endif                                            //SSMIPACLWORKER_H
