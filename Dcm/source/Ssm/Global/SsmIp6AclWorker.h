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
 * Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 * All rights reserved.                                                  *
 * Author : Charanjith Reddy Kunduru                                     *
 **************************************************************************/

#ifndef SSMIP6ACLWORKER_H
#define SSMIP6ACLWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Utils/DceLinearSeqContext.h"
#include "Ip6AclPolicyManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

	class SsmGlobalObjectManager;
	class SsmGlobalIp6AclPolicyMessage;
//	class SsmGlobalRemIpAclPolicyMessage;
	class SsmGlobalAssocIp6RuleToPolicyMessage;
//	class SsmGlobalReseqIpAclPolicyMessage;
//	class SsmGlobalGetIpAclPolicyMessage;

	class SsmIp6AclWorker: public WaveWorker
	{
	  private:
		PrismMessage *createMessageInstance(const UI32 &operationCode);
		WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
		int getPolicyEnforcedSwitches (string &policyName, vector<LocationId> &locVector); // , IpAclVersion aclVersion);
		int processExtIp6AclRules (SsmGlobalAssocIp6RuleToPolicyMessage *pGlobalAssocIp6RuleToPolicyMessage);
		void extAclRuleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
		void updateIpAclRuleToAAA(SsmGlobalAssocIp6RuleToPolicyMessage *pGlobalAssocIp6RuleToPolicyMessage, vector <LocationId> &locVector);
		void assocIpAclRuleToPolicy(SsmGlobalAssocIp6RuleToPolicyMessage *pGlobalAssocIp6RuleToPolicyMessage, Ip6AclPolicyManagedObject *pIp6AclPolicyManagedObject);
		void SsmGlobalAssocIp6RuleToPolicyMessageHandler (SsmGlobalAssocIp6RuleToPolicyMessage *pGlobalAssocIp6RuleToPolicyMessage);	
		ResourceId validateRule(SsmGlobalAssocIp6RuleToPolicyMessage *pGlobalAssocIp6RuleToPolicyMessage);
		void QuerychildValidation(WaveManagedObjectSynchronousQueryContext &queryCtxtForChild, ResourceId &status, bool &ruleFound, bool &seqFound, UI32 opcode, UI32 UserEnteredSeqid, UI64 &seqid, UI32 policyType);
		void SsmGlobalIp6AclPolicyMessageHandler (SsmGlobalIp6AclPolicyMessage *pGlobalIpAclPolicyMessage);
//		void AAAGlobalStdIp6AclRuleMessageHandler (AAAGlobalStdIpAclRuleMessage *pGlobalStdIpAclRuleMessage);
//		void AAAGlobalExtIp6AclRuleMessageHandler (AAAGlobalExtIpAclRuleMessage *pGlobalExtIpAclRuleMessage);
//		void stdAclRuleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
		void updateAclRuleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
		ResourceId verifyIPMACPolicyexists(string policyName);

	public:
		SsmIp6AclWorker(SsmGlobalObjectManager *pSsmGlobalObjectManager);
		virtual ~SsmIp6AclWorker();
	};

}
#endif
