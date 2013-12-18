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

#ifndef GLOBALIPACLWORKER_H
#define GLOBALIPACLWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalObjectManager;
class AAAGlobalIpAclPolicyMessage;
class AAAGlobalStdIpAclRuleMessage;
class AAAGlobalExtIpAclRuleMessage;

class GlobalIpAclWorker: public WaveWorker
{
  private:
	PrismMessage *createMessageInstance(const UI32 &operationCode);
	WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
	int getPolicyEnforcedSwitches (string &policyName, vector<LocationId> &locVector, IpAclVersion aclVersion);
	void processStdAclRules (AAAGlobalStdIpAclRuleMessage *pGlobalStdIpAclRuleMessage);
	void processExtAclRules (AAAGlobalExtIpAclRuleMessage *pGlobalExtIpAclRuleMessage);
	void processStdIp6AclRules (AAAGlobalStdIpAclRuleMessage *pGlobalStdIpAclRuleMessage);
	void processExtIp6AclRules (AAAGlobalExtIpAclRuleMessage *pGlobalExtIpAclRuleMessage);
	void extAclRuleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
 public:
	GlobalIpAclWorker(AAAGlobalObjectManager *pAAAGlobalObjectManager);
	virtual ~GlobalIpAclWorker();
	void AAAGlobalAclPolicyMessageHandler (AAAGlobalIpAclPolicyMessage *pGlobalIpAclPolicyMessage);
	void AAAGlobalStdAclRuleMessageHandler (AAAGlobalStdIpAclRuleMessage *pGlobalStdIpAclRuleMessage);
	void AAAGlobalExtAclRuleMessageHandler (AAAGlobalExtIpAclRuleMessage *pGlobalExtIpAclRuleMessage);
	void AAAGlobalIpAclPolicyMessageHandler (AAAGlobalIpAclPolicyMessage *pGlobalIpAclPolicyMessage);
	void AAAGlobalStdIpAclRuleMessageHandler (AAAGlobalStdIpAclRuleMessage *pGlobalStdIpAclRuleMessage);
	void AAAGlobalExtIpAclRuleMessageHandler (AAAGlobalExtIpAclRuleMessage *pGlobalExtIpAclRuleMessage);
	void AAAGlobalIp6AclPolicyMessageHandler (AAAGlobalIpAclPolicyMessage *pGlobalIpAclPolicyMessage);
	void AAAGlobalStdIp6AclRuleMessageHandler (AAAGlobalStdIpAclRuleMessage *pGlobalStdIpAclRuleMessage);
	void AAAGlobalExtIp6AclRuleMessageHandler (AAAGlobalExtIpAclRuleMessage *pGlobalExtIpAclRuleMessage);
	void stdAclRuleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
};

}
#endif
