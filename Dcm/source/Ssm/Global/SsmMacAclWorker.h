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
 *   Author : pasu                                                     *
 **************************************************************************/

#ifndef SSMMACACLWORKER_H
#define SSMMACACLWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Utils/DceLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalObjectManager;
    class SsmGlobalAddMacAclPolicyMessage;
    class SsmGlobalRemMacAclPolicyMessage;
    class SsmGlobalAssocRuleToPolicyMessage;
    class SsmGlobalAssocPolicyToInterfaceMessage;
    class SsmGlobalGetMacAclPolicyMessage;
    class SsmGlobalReseqMacAclPolicyMessage;

    class SsmMacAclWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SsmGlobalAddMacAclPolicyMessageHandler( SsmGlobalAddMacAclPolicyMessage *pSsmGlobalAddMacAclPolicyMessage);
            void SsmGlobalAssocRuleToPolicyMessageHandler( SsmGlobalAssocRuleToPolicyMessage *pSsmGlobalAssocRuleToPolicyMessage);
            void SsmGlobalRemMacAclPolicyMessageHandler( SsmGlobalRemMacAclPolicyMessage *pSsmGlobalRemMacAclPolicyMessage);
            void SsmGlobalGetMacAclPolicyMessageHandler(SsmGlobalGetMacAclPolicyMessage *pSsmGlobalGetMacAclPolicyMessage);
            void SsmGlobalReseqMacAclPolicyMessageHandler(SsmGlobalReseqMacAclPolicyMessage *pSsmGlobalReesqMacAclPolicyMessage);
            void createMacAclPolicy(PrismLinearSequencerContext *seqContext);
            void deleteMacAclPolicy(PrismLinearSequencerContext *seqContext);
            //void deleteAssociationwithDisconnectedPolicy (PrismLinearSequencerContext *seqContext);
			void createMacAclPolicyMO(PrismLinearSequencerContext *seqContext);
            void disAssocMacAclRuleToPolicy(DceLinearSeqContext *seqContext);
            void assocMacAclRuleToPolicy(DceLinearSeqContext *seqContext);
            void udpateMacAclRuleToPolicy(PrismLinearSequencerContext *seqContext);
            void deleteMacAclPolicyMO(DceLinearSeqContext *seqContext);
           	void QuerychildValidation(WaveManagedObjectSynchronousQueryContext &queryCtxtForChild, ResourceId &status, bool &ruleFound, bool &seqFound, UI32 opcode, UI32 UserEnteredSeqid, UI64 &seqid, UI32 policyType); 
			
			void validateRule(DceLinearSeqContext *seqContext);
            void appmSecProfileUpdateStep(PrismLinearSequencerContext *seqContext);
            void ssmMacAclPolicySendToClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void ssmMacAclRuleSendToClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void validatePolicy(PrismLinearSequencerContext *seqContext);
            void validateDeletePolicy(DceLinearSeqContext *seqContext);
            ResourceId getMacAclPolicyType(const string &policyName, ObjectId &policyObjId, UI32 &policyType);
            void deleteAssociationwithDisconnectedPolicy (PrismLinearSequencerContext *seqContext);
			void deleteOldMacAclPolicyMO(PrismLinearSequencerContext *seqContext);

            void resequenceMacAclPolicy(PrismLinearSequencerContext *seqContext);
            void updateresequenceMacAclPolicy(PrismLinearSequencerContext *seqContext);
	        void ssmMacAclReseqPolicySendToClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	        void validateResqPolicy(PrismLinearSequencerContext *seqContext);

			UI32  ssmIsmacPolicyAttachInterface(const string& policy, DceLinearSeqContext *seqContext);
        protected:
        public:
            SsmMacAclWorker ( SsmGlobalObjectManager *pSsmGlobalObjectManager);
            virtual    ~SsmMacAclWorker ();
            string convertToLower(string strToConvert);

        private:
        protected:
        public:
    };
}
#endif                                            //SSMMACACLWORKER_H
