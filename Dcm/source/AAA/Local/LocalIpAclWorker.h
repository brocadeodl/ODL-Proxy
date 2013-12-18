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
 *   Author : Kumar Ghanta                                                 *
 **************************************************************************/

#ifndef LOCALIPACLWORKER_H
#define LOCALIPACLWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "AAA/Local/AAALocalIpAclConfigMessage.h"
#include "AAA/Local/AAALocalIpAclDeleteMessage.h"
#include "AAA/Local/AAALocalIpAclMessages.h"
#include "AAA/Local/AAALocalIpAclReseqMessage.h"
#include "AAA/Local/AAALocalAssocIp6RuleToPolicyMessage.h"
#include "AAA/Global/AAAGlobalTypes.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmIpAclWorker.h"
#include "Ssm/Global/SsmGlobalAddIpAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalRemIpAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalAssocIpRuleToPolicyMessage.h"
#include "Ssm/Global/SsmGlobalReseqIpAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalGetIpAclPolicyMessage.h"
#include "Ssm/Global/IpAclPolicyManagedObject.h"
#include "Ssm/Global/StdIpAclPolicyManagedObject.h"
#include "Ssm/Global/ExtIpAclPolicyManagedObject.h"
#include "Ssm/Global/StdIpAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Ssm/Local/SsmTypes.h"
#include "Ssm/Local/SsmLocalServiceAddIpAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceRemIpAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceAssocIpRuleToPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceResqIpAclPolicyMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "DcmCStatus.h"
#include "AAA/Local/AAAIpAclLocalShowMessage.h"
#include "Nsm/Global/NsmGlobalReAssocIpAclPolicyMessage.h"

#include "Ssm/Global/MacAclPolicyManagedObject.h"
#include "Nsm/Local/Ssm/NsmLocalIngressEgressIpAclPolicyManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class AAALocalLocalObjectManager;

class LocalIpAclWorker: public WaveWorker
{
  private:
	PrismMessage *createMessageInstance(const UI32 &operationCode);
	WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
	int  enforceEmptyPolicyRules(string ipAclPolicy);
	int  addStdRulesBackdb(string ipAclPolicy, int seqnum, int action, string host);
	int  addExtRulesBackdb(string ipAclPolicy, int seqnum, int action, string host, UI32 dport0, UI32 dport1, int proto);
	int  addExtRulesBackdbOptimized(string ipAclPolicy, int seqnum, int action, string shost, UI32 dport0,
                            UI32 dport1, string dhost, UI32 sport0, UI32 sport1, int proto, string dscp,
                            UI32 urg, UI32 ack, UI32 psh, UI32 fin, UI32 rst, UI32 sync, UI32 neqSportFlag,
							UI32 neqDportFlag);
	int addStdIpAclRules (AAALocalAssocIpRuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage);
	int deleteStdIpAclRules(AAALocalAssocIpRuleToPolicyMessage *pLocalStdIpAclRuleMessage);
	int addExtIpAclRules (AAALocalAssocIpRuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage);
	int deleteExtIpAclRules(AAALocalAssocIpRuleToPolicyMessage *pLocalStdIpAclRuleMessage);
	int addExtIp6AclRules (AAALocalAssocIp6RuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage, IpAclVersion version);
	int deleteExtIp6AclRules(AAALocalAssocIp6RuleToPolicyMessage *pLocalStdIpAclRuleMessage, IpAclVersion version);
	int resequenceStdIpAclRules(AAALocalIpAclReseqMessage *pLocalIpAclReseqMessage);
	int resequenceExtIpAclRules(AAALocalIpAclReseqMessage *pLocalIpAclReseqMessage);
	int showIpAclRules(AAAIpAclLocalShowMessage *pAAAIpAclLocalShowMessage,UI32 tag);

  public:
	LocalIpAclWorker ( AAALocalLocalObjectManager *pAAALocalLocalObjectManager);
	virtual ~LocalIpAclWorker ();
	void AAALocalIpAclConfigMessageHandler (AAALocalIpAclConfigMessage *pLocalIpAclConfigMessage);
	void AAALocalIpAclDeleteMessageHandler (AAALocalIpAclDeleteMessage *pLocalIpAclDeleteMessage);
	void AAALocalAssocIpRuleToPolicyMessageHandler (AAALocalAssocIpRuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage);
	void AAALocalAssocIp6RuleToPolicyMessageHandler (AAALocalAssocIp6RuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage);
	void AAALocalIpAclReseqMessageHandler (AAALocalIpAclReseqMessage *pLocalIpAclReseqMessage);
	void AAAIpAclLocalShowMessageHandler (AAAIpAclLocalShowMessage *pAAAIpAclLocalShowMessage);
	int isIpPolicyAttachToMgmtInterface(const string& policy);
//	void AAALocalStdIpAclRuleMessageHandler(AAALocalStdIpAclRuleMessage *pLocalStdIpAclRuleMessage);
	int  addStdIp6AclRules(AAALocalAssocIp6RuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage, IpAclVersion version);
	int  deleteStdIp6AclRules(AAALocalAssocIp6RuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage, IpAclVersion version);
	int enforceIpAclPolicy(string ipAclPolicy, string switchId, bool isStoreDb, IpAclVersion version);
	int  enforceIp6AclPolicy(string ipAclPolicy, string switchId, bool isStoreDb, IpAclVersion version);
	void  deleteIpPolicy( AAALocalIpAclDeleteMessage *pAAALocalIpAclDeleteMessage);
	void  deleteIp6Policy( AAALocalIpAclDeleteMessage *pAAALocalIpAclDeleteMessage);
	int	getPolicyEnforcedSwitches (string &policyName, vector<LocationId> &locVector, IpAclVersion version);
	// just for ssm testing
	void ipAclRuleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void ipAclReseqSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	string	netmaskToCidr(string netmask);
	int getItfIdfromSwitchId (string switchId);

	const char *IsWellKnownPrecedence (UI32 PrecDscpTosNum);
	const char *IsWellKnownDscp (UI32 PrecDscpTosNum);
	const char *IsWellKnownTos (UI32 PrecDscpTosNum);
};

}
#endif	//LOCALDISCRETEOBJSWORKER_H
