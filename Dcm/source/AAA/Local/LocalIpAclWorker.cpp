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
 *   Author : ckunduru                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include <sstream>

#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/LocalIpAclWorker.h"
#include "AAA/Local/AAALocalIpAclConfigMessage.h"
#include "AAA/Local/AAALocalTypes.h"
#include "AAA/Local/IpAclLocalManagedObjects.h"
#include "AAA/Global/IpAclManagedObjects.h"
// #include "AAA/Global/Ip6AclManagedObjects.h"
#include "Ssm/Global/Ip6AclPolicyManagedObject.h"
#include "Ssm/Global/StdIp6AclPolicyManagedObject.h"
#include "Ssm/Global/ExtIp6AclPolicyManagedObject.h"
#include "AAA/Local/AAALocalIpAclDeleteMessage.h"
#include "AAA/Local/AAALocalIpAclMessages.h"
#include "AAA/Local/AAALocalIpAclReseqMessage.h"
#include "AAA/Global/AAASequencerContexts.h"

extern "C" {
	#include "security/public.h"
	#include "security/ipfilter_public.h"
}

namespace DcmNs
{
/* the message AAALOCALIPACLDELETE is not required as of now */
LocalIpAclWorker::LocalIpAclWorker ( AAALocalLocalObjectManager *pAAALocalLocalObjectManager)
	: WaveWorker  (pAAALocalLocalObjectManager)
{
	IpAclPolicyLocalManagedObject ipAclPolicyLocalMO (pAAALocalLocalObjectManager);
	ipAclPolicyLocalMO.setupOrm ();
	addManagedClass (IpAclPolicyLocalManagedObject::getClassName (), this);
	
	Ip6AclPolicyLocalManagedObject ip6AclPolicyLocalMO (pAAALocalLocalObjectManager);
	ip6AclPolicyLocalMO.setupOrm ();
	addManagedClass (Ip6AclPolicyLocalManagedObject::getClassName (), this);

    addOperationMap (AAALOCALIPACLCONFIG, reinterpret_cast<PrismMessageHandler> (&LocalIpAclWorker::AAALocalIpAclConfigMessageHandler));
	addOperationMap (AAALOCALIPACLDELETE, reinterpret_cast<PrismMessageHandler> (&LocalIpAclWorker::AAALocalIpAclDeleteMessageHandler));
	addOperationMap (AAALOCALASSOCIPRULETOPOLICY, reinterpret_cast<PrismMessageHandler> (&LocalIpAclWorker::AAALocalAssocIpRuleToPolicyMessageHandler));
	addOperationMap (AAALOCALASSOCIP6RULETOPOLICY, reinterpret_cast<PrismMessageHandler> (&LocalIpAclWorker::AAALocalAssocIp6RuleToPolicyMessageHandler));
	addOperationMap (AAALOCALIPACLRESEQ, reinterpret_cast<PrismMessageHandler> (&LocalIpAclWorker::AAALocalIpAclReseqMessageHandler));
	addOperationMap (AAALOCALSHOWIPACL, reinterpret_cast<PrismMessageHandler> (&LocalIpAclWorker::AAAIpAclLocalShowMessageHandler));
}

LocalIpAclWorker::~LocalIpAclWorker ()
{
}

PrismMessage  *LocalIpAclWorker::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{
		case AAALOCALIPACLCONFIG :
			pPrismMessage = new AAALocalIpAclConfigMessage ();
			break;
		case AAALOCALIPACLDELETE :
			pPrismMessage = new AAALocalIpAclDeleteMessage ();
			break;
		case AAALOCALASSOCIPRULETOPOLICY :
			pPrismMessage = new AAALocalAssocIpRuleToPolicyMessage ();
			break;
		case AAALOCALIPACLRESEQ :
			pPrismMessage = new AAALocalIpAclReseqMessage ();
			break;

		case AAALOCALASSOCIP6RULETOPOLICY :
			pPrismMessage = new AAALocalAssocIp6RuleToPolicyMessage ();
			break;

		default :
			pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject  *LocalIpAclWorker::createManagedObjectInstance(const string &managedClassName)
{
	WaveManagedObject *pWaveManagedObject = NULL;

	if ((IpAclPolicyLocalManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new IpAclPolicyLocalManagedObject(dynamic_cast<AAALocalLocalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((Ip6AclPolicyLocalManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new Ip6AclPolicyLocalManagedObject(dynamic_cast<AAALocalLocalObjectManager *>(getPWaveObjectManager()));
	}

	return (pWaveManagedObject);
}

/* As per NOS, for the empty policy enforcement, accept the ingress traffic from every host. */
int  LocalIpAclWorker::enforceEmptyPolicyRules(string ipAclPolicy)
{
	return (addStdRulesBackdb(ipAclPolicy, 0, IPF_ACCEPT, "any"));
}

int  LocalIpAclWorker::addStdRulesBackdb(string ipAclPolicy, int seqnum, int action, string host)
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::addStdRulesBackdb()");
	trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdRulesBackdb() ipacl policy: ") + ipAclPolicy + " action:" + action + " seqnum:" + seqnum + " host:" + host);
	int ret = ipfPolicyRuleAdd(ipAclPolicy.c_str(), STD_ACL_INDEX(seqnum), host.c_str(), 1, (IPF_MAX_PORTNUM - 1), IPPROTO_TCP, action);
	if (ret) {
		trace (TRACE_LEVEL_ERROR, string("Enforcing the policy in the backend failed: ipfPolicyRuleAdd(): ") + ret);
		return (-1);
	} else {
		ret = ipfPolicyRuleAdd(ipAclPolicy.c_str(), STD_ACL_INDEX(seqnum) + 1, host.c_str(), 1, (IPF_MAX_PORTNUM - 1), IPPROTO_UDP, action);
		if (ret) {
			trace (TRACE_LEVEL_ERROR, string("Enforcing the policy in the backend failed: ipfPolicyRuleAdd(): ") + ret);
			return (-1);
		}
	}
	trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::addStdRulesBackdb()");
	return (0);
}

int  LocalIpAclWorker::addExtRulesBackdbOptimized(string ipAclPolicy, int seqnum, int action,
							string shost, UI32 dport0, UI32 dport1, string dhost, UI32 sport0,
							UI32 sport1, int proto, string preceDscpTos, UI32 urg, UI32 ack, UI32 psh,
							UI32 fin, UI32 rst, UI32 sync, UI32 neqSportFlag, UI32 neqDportFlag)
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::addExtRulesBackdbOptimized()");
	ipfExtnRule_t ext_rule = {0};

	ext_rule.action = action;
	ext_rule.proto_num = proto;
	ext_rule.dPort = dport0;
	ext_rule.dEndport = dport1;
	//if (shost.compare("") != 0) {
		strncpy(ext_rule.saddrPref, shost.c_str(), sizeof (ext_rule.saddrPref));
	//}
	//if (dhost.compare("") != 0) {
		strncpy(ext_rule.daddrPref, dhost.c_str(), sizeof (ext_rule.daddrPref));
	//}
	if (sport0 >= 0) {
		ext_rule.sPort = sport0;
	}
	if (sport1 >= 0) {
		ext_rule.sEndport = sport1;
	}
	//if (precDscpTos.compare("") != 0) {
		strncpy(ext_rule.preceDscpTos, preceDscpTos.c_str(),
						sizeof (ext_rule.preceDscpTos));

	ext_rule.urg = urg;
	ext_rule.ack = ack;
	ext_rule.psh = psh;
	ext_rule.fin = fin;
	ext_rule.rst = rst;
	ext_rule.syn = sync;
	ext_rule.neqSportFlag	 = neqSportFlag;
	ext_rule.neqDportFlag	 = neqDportFlag;

	int ret = ipfPolicyRuleAddNos(ipAclPolicy.c_str(), EXT_ACL_INDEX(seqnum), &ext_rule);
	if (ret) {
		trace (TRACE_LEVEL_ERROR, string("Enforcing the policy in the backend failed: ipfPolicyRuleAdd(): ") + ret);
		return (-1);
	}
	trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::addExtRulesBackdbOptimized()");
	return (0);	
}

int  LocalIpAclWorker::addExtRulesBackdb(string ipAclPolicy, int seqnum, int action, string host, UI32 dport0, UI32 dport1, int proto)
{
    trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::addExtRulesBackdb()");
    trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addExtRulesBackdb() ipacl policy: ") + ipAclPolicy + " action:" + action + " seqnum:" + seqnum + "proto:" + proto + " host:" + host + "dport0:" + dport0 + "dport1:" + dport1);
    int ret = ipfPolicyRuleAdd(ipAclPolicy.c_str(), EXT_ACL_INDEX(seqnum), host.c_str(), dport0, dport1, proto, action);
    if (ret) {
        trace (TRACE_LEVEL_ERROR, string("Enforcing the policy in the backend failed: ipfPolicyRuleAdd(): ") + ret);
        return (-1);
    }
    trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::addExtRulesBackdb()");
    return (0);
}

void  LocalIpAclWorker::AAALocalIpAclDeleteMessageHandler( AAALocalIpAclDeleteMessage *pAAALocalIpAclDeleteMessage)
{
	string switchId = pAAALocalIpAclDeleteMessage->getSwitchId();
	int activeItf = findActiveInterface();
	int itf = getItfIdfromSwitchId(switchId);
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	if (itf != activeItf) {
		trace (TRACE_LEVEL_INFO, "The Interface specified is that on Standby");
		status = IP_ACL_INVALID_MGMT_INTF;
	}
	if (status == WAVE_MESSAGE_SUCCESS)
	{
		if (pAAALocalIpAclDeleteMessage->getAclVersion() == ACL_IPV4) {
			deleteIpPolicy(pAAALocalIpAclDeleteMessage);
		}
	 	else
		{
			deleteIp6Policy(pAAALocalIpAclDeleteMessage);
		}
	}
	else
	{
		pAAALocalIpAclDeleteMessage->setCompletionStatus(status);
		reply(pAAALocalIpAclDeleteMessage);
	}
		
}

void  LocalIpAclWorker::deleteIp6Policy( AAALocalIpAclDeleteMessage *pAAALocalIpAclDeleteMessage)
{
		trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::AAALocalIp6AclDeleteMessageHandler()");
		prismAssert(pAAALocalIpAclDeleteMessage != NULL, __FILE__, __LINE__);

		// Getting the configuration from the message.
		string switchId = pAAALocalIpAclDeleteMessage->getSwitchId();
		//string ipAclPolicy = pAAALocalIpAclDeleteMessage->getPolicyName();

		Ip6AclPolicyLocalManagedObject *pIpAclPolicyLocalMO = NULL, *pIpAclPolicyStandbyMO = NULL;

		WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
		vector<WaveManagedObject *> *pResults = NULL;

		ResourceId status = WAVE_MESSAGE_SUCCESS;
		ipf_txn_hdl_t txnHdl = 0;
		int ret;

		/* Get the MO instance of the Local Mgmt I/F's policy */
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyLocalManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeString (switchId, "switchid"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults || pResults->size() != 1)
		{
			trace (TRACE_LEVEL_FATAL, "Instance of the Mgmt I/F does not exist in the database");
			if (pResults != NULL) WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			delete pSyncQueryCtx;
			pAAALocalIpAclDeleteMessage->setCompletionStatus(FRAMEWORK_ERROR);
			reply(pAAALocalIpAclDeleteMessage);
			return;
		}
		else
		{
			pIpAclPolicyLocalMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>((*pResults)[0]);
			if (!pIpAclPolicyLocalMO) {
                status = WAVE_MESSAGE_ERROR;
                trace (TRACE_LEVEL_INFO, "Deleting the policy in the back end failed");
				delete pResults;
				delete pSyncQueryCtx;
                pAAALocalIpAclDeleteMessage->setCompletionStatus(status);
                reply(pAAALocalIpAclDeleteMessage);
                return;
			}
		}
		delete pResults;
		delete pSyncQueryCtx;

		if (getItfIdfromSwitchId(switchId) != 0) {
			/* Get the MO instance of the Standby Mgmt I/F's policy */
			string switchId2;
			int localId = DcmToolKit::getLocalMappedId();

			if (getItfIdfromSwitchId(switchId) == 1) {
				switchId2 = localId + string ("/") + 2;
			} else {
				switchId2 = localId + string ("/") + 1;
			}
			trace (TRACE_LEVEL_INFO, string ("the switchId2 is ") + switchId2);
			pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyLocalManagedObject::getClassName());
			pSyncQueryCtx->addAndAttribute (new AttributeString (switchId2, "switchid"));
			pResults = querySynchronously (pSyncQueryCtx);
			if (!pResults || pResults->size() != 1)
			{
				trace (TRACE_LEVEL_FATAL, "Instance of the Mgmt I/F does not exist in the database");
				if (pResults != NULL) WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				delete pSyncQueryCtx;
				pAAALocalIpAclDeleteMessage->setCompletionStatus(FRAMEWORK_ERROR);
				reply(pAAALocalIpAclDeleteMessage);
				return;
			}
			else
			{
				pIpAclPolicyStandbyMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>((*pResults)[0]);
				if (!pIpAclPolicyStandbyMO) {
					status = WAVE_MESSAGE_ERROR;
					trace (TRACE_LEVEL_INFO, "Deleting the policy in the back end failed");
					delete pResults;
					delete pSyncQueryCtx;
					pAAALocalIpAclDeleteMessage->setCompletionStatus(status);
					reply(pAAALocalIpAclDeleteMessage);
	                return;
				}
			}
			delete pResults;
			delete pSyncQueryCtx;
		}

		txnHdl = ipfTransBegin();
		if (txnHdl > 0) {
			// Active the default policy
			ret = ipfPolicyActivate(IPF_DEFAULT6_POLICY_NAME);
			//TODO:Handle the return errors properly.
			if (ret) {
				ipfTransAbort(txnHdl);
				status = WAVE_MESSAGE_ERROR;
				delete pIpAclPolicyLocalMO;
				trace (TRACE_LEVEL_INFO, string("Deleting the policy in the back end failed: ipfPolicyActivate(): ") + ret);
				pAAALocalIpAclDeleteMessage->setCompletionStatus(status);
				reply(pAAALocalIpAclDeleteMessage);
				return;
			}
			string oldPolicyName;	
			oldPolicyName = pIpAclPolicyLocalMO->getIpv6AclPolicy();
			ret = ipfPolicyDelete(oldPolicyName.c_str());
			if (ret != 0) {
				status = WAVE_MESSAGE_ERROR;
				ipfTransAbort(txnHdl);
				delete pIpAclPolicyLocalMO;
				trace (TRACE_LEVEL_INFO, string("Deleting the policy in the back end failed: ipfPolicyDelete(): ") + ret);
				pAAALocalIpAclDeleteMessage->setCompletionStatus(status);
				reply(pAAALocalIpAclDeleteMessage);
				return;
			}
			//TODO: Need to handle these error cases ????? 
			ipfPolicySave(NULL);
			ipfTransEnd(txnHdl);
			/* Delete the MO instance in the database */
			startTransaction();
			//updateWaveManagedObject(pIpAclPolicyLocalMO);
			delete pIpAclPolicyLocalMO;
			status = commitTransaction();

			if (pIpAclPolicyStandbyMO)
			{
				// delete the MO instance of standby interface
				startTransaction();
				delete pIpAclPolicyStandbyMO;
				status = commitTransaction();
			}

			if (status == FRAMEWORK_SUCCESS)
			{
				trace (TRACE_LEVEL_INFO, "Deleting the policy in the DB is successful.");
				status = WAVE_MESSAGE_SUCCESS;
			}
			else
			{
				status = WAVE_MESSAGE_ERROR;
				trace (TRACE_LEVEL_FATAL, "Deleting the policy in the database FAILED!!.");
			}
		} else {
			status = WAVE_MESSAGE_ERROR;
			delete pIpAclPolicyLocalMO;
			trace (TRACE_LEVEL_INFO, string("Transaction is failed to start: ipfTransbegin(): ") + ret);
		}
		pAAALocalIpAclDeleteMessage->setCompletionStatus (status);
		reply (pAAALocalIpAclDeleteMessage);
		trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::AAALocalIp6AclDeleteMessageHandler()");
}

void  LocalIpAclWorker::deleteIpPolicy( AAALocalIpAclDeleteMessage *pAAALocalIpAclDeleteMessage)
{
		trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::AAALocalIpAclDeleteMessageHandler()");
		prismAssert(pAAALocalIpAclDeleteMessage != NULL, __FILE__, __LINE__);

		// Getting the configuration from the message.
		string switchId = pAAALocalIpAclDeleteMessage->getSwitchId();
		string ipAclPolicy = pAAALocalIpAclDeleteMessage->getPolicyName();

		IpAclPolicyLocalManagedObject *pIpAclPolicyLocalMO = NULL, *pIpAclPolicyStandbyMO = NULL;

		WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
		vector<WaveManagedObject *> *pResults = NULL;

		ResourceId status = WAVE_MESSAGE_SUCCESS;
		ipf_txn_hdl_t txnHdl = 0;
		int ret;

		/* Get the MO instance of the Local Mgmt I/F's policy */
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyLocalManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeString (switchId, "switchid"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults || pResults->size() != 1)
		{
			trace (TRACE_LEVEL_FATAL, "Instance of the Mgmt I/F does not exist in the database");
			if (pResults != NULL) WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			delete pSyncQueryCtx;
			pAAALocalIpAclDeleteMessage->setCompletionStatus(FRAMEWORK_ERROR);
			reply(pAAALocalIpAclDeleteMessage);
			return;
		}
		else
		{
			pIpAclPolicyLocalMO = dynamic_cast<IpAclPolicyLocalManagedObject *>((*pResults)[0]);
			if (!pIpAclPolicyLocalMO) {
                status = WAVE_MESSAGE_ERROR;
                trace (TRACE_LEVEL_INFO, "Deleting the policy in the back end failed");
				delete pResults;
				delete pSyncQueryCtx;
                pAAALocalIpAclDeleteMessage->setCompletionStatus(status);
                reply(pAAALocalIpAclDeleteMessage);
                return;
			}
		}
		delete pResults;
		delete pSyncQueryCtx;

		if (getItfIdfromSwitchId(switchId) != 0) {
			string switchId2;
			int localId = DcmToolKit::getLocalMappedId();
			trace (TRACE_LEVEL_INFO, string ("Local id is ") + localId);
			if (getItfIdfromSwitchId(switchId) == 1) {
				switchId2 = localId + string ("/") + 2;
			} else {
				switchId2 = localId + string ("/") + 1;
			}
			trace (TRACE_LEVEL_INFO, string ("the switchId2 is ") + switchId2);
			/* Get the MO instance of the Local Mgmt I/F's policy */
			pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyLocalManagedObject::getClassName());
			pSyncQueryCtx->addAndAttribute (new AttributeString (switchId2, "switchid"));
			pResults = querySynchronously (pSyncQueryCtx);
			if (!pResults || pResults->size() != 1)
			{
				trace (TRACE_LEVEL_FATAL, "Instance of the Mgmt I/F does not exist in the database");
				if (pResults != NULL) WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				delete pSyncQueryCtx;
				pAAALocalIpAclDeleteMessage->setCompletionStatus(FRAMEWORK_ERROR);
				reply(pAAALocalIpAclDeleteMessage);
				return;
			}
			else
			{
				pIpAclPolicyStandbyMO = dynamic_cast<IpAclPolicyLocalManagedObject *>((*pResults)[0]);
				if (!pIpAclPolicyStandbyMO) {
   	             status = WAVE_MESSAGE_ERROR;
   	             trace (TRACE_LEVEL_INFO, "Deleting the policy in the back end failed");
					delete pResults;
					delete pSyncQueryCtx;
   	             pAAALocalIpAclDeleteMessage->setCompletionStatus(status);
   	             reply(pAAALocalIpAclDeleteMessage);
   	             return;
				}
			}
			delete pResults;
			delete pSyncQueryCtx;
		}

		txnHdl = ipfTransBegin();
		if (txnHdl > 0) {
			// Active the default policy
			ret = ipfPolicyActivate(IPF_DEFAULT_POLICY_NAME);
			//TODO:Handle the return errors properly.
			if (ret) {
				ipfTransAbort(txnHdl);
				status = WAVE_MESSAGE_ERROR;
				delete pIpAclPolicyLocalMO;
				trace (TRACE_LEVEL_INFO, string("Deleting the policy in the back end failed: ipfPolicyActivate(): ") + ret);
				pAAALocalIpAclDeleteMessage->setCompletionStatus(status);
				reply(pAAALocalIpAclDeleteMessage);
				return;
			}
			string oldPolicyName = pIpAclPolicyLocalMO->getIpAclPolicy();
			ret = ipfPolicyDelete(oldPolicyName.c_str());
			if (ret != 0) {
				status = WAVE_MESSAGE_ERROR;
				ipfTransAbort(txnHdl);
				delete pIpAclPolicyLocalMO;
				trace (TRACE_LEVEL_INFO, string("Deleting the policy in the back end failed: ipfPolicyDelete(): ") + ret);
				pAAALocalIpAclDeleteMessage->setCompletionStatus(status);
				reply(pAAALocalIpAclDeleteMessage);
				return;
			}
			//TODO: Need to handle these error cases ????? 
			ipfPolicySave(NULL);
			ipfTransEnd(txnHdl);
			/* Delete the MO instance in the database */
			startTransaction();
			//updateWaveManagedObject(pIpAclPolicyLocalMO);
			delete pIpAclPolicyLocalMO;
			status = commitTransaction();

			if (pIpAclPolicyStandbyMO) {
				//delete the MO instance of the standby intf
				startTransaction();
				delete pIpAclPolicyStandbyMO;
				status = commitTransaction();
			}

			if (status == FRAMEWORK_SUCCESS)
			{
				trace (TRACE_LEVEL_INFO, "Deleting the policy in the DB is successful.");
				status = WAVE_MESSAGE_SUCCESS;
			}
			else
			{
				status = WAVE_MESSAGE_ERROR;
				trace (TRACE_LEVEL_FATAL, "Deleting the policy in the database FAILED!!.");
			}
		} else {
			status = WAVE_MESSAGE_ERROR;
			delete pIpAclPolicyLocalMO;
			trace (TRACE_LEVEL_INFO, string("Transaction is failed to start: ipfTransbegin(): ") + ret);
		}
		pAAALocalIpAclDeleteMessage->setCompletionStatus (status);
		reply (pAAALocalIpAclDeleteMessage);
		trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::AAALocalIpAclDeleteMessageHandler()");
}

int  LocalIpAclWorker::enforceIp6AclPolicy(string ipAclPolicy, string switchId, bool isStoreDb, IpAclVersion version) 
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::enforceIp6AclPolicy()");
	Ip6AclPolicyManagedObject *pIpAclPolicyMO = NULL;
	Ip6AclPolicyLocalManagedObject *pIpAclPolicyLocalMO = NULL, *pIpAclPolicyStandbyMO = NULL;
	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResultsPMO = NULL;
	UI8 action = 0;
	int ret = 0;

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	bool isNewLocalMO = false;

	UI32	policyType = ACL_TYPE_UNKNOWN;
	//ObjectId	policyObjId = ObjectId::NullObjectId;


	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIp6AclPolicyManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (ipAclPolicy, "policyName"));
	pResultsPMO = querySynchronously (pSyncQueryCtx);
	if (!pResultsPMO)
	{
		delete pSyncQueryCtx;
		return (FRAMEWORK_ERROR);
	}
	else if (pResultsPMO->size () == 1)
	{
		trace (TRACE_LEVEL_INFO, "its an ACL_TYPE_L3_IPV4_STD_E");
		policyType = STANDARD_IP_ACL;
	}
	else
	{
		/* the policy is not standard, we will check if its an extended policy */
		delete pResultsPMO;
		delete pSyncQueryCtx;
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIp6AclPolicyManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeString (ipAclPolicy, "policyName"));
		pResultsPMO = querySynchronously (pSyncQueryCtx);
		if (!pResultsPMO)
		{
			delete pSyncQueryCtx;
			return (FRAMEWORK_ERROR);
		}
		else if (pResultsPMO->size () == 1)
		{
			trace (TRACE_LEVEL_INFO, "its an ACL_TYPE_L3_IPV4_EXT_E");
			policyType = EXTENDED_IP_ACL;
		}
	}
	delete pSyncQueryCtx;

	/* Get the MO instance of the Mgmt I/F */
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyLocalManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (switchId, "switchid"));
	vector<WaveManagedObject *> *pResults = querySynchronously (pSyncQueryCtx);
	// Size should be 0 or 1, If the size is 0, no instance is created(add operation)
	// If size is 1 instance is already there and changing the existing policy now for the instance
	// (Change operation).
	if (!pResults || (pResults->size() > 1))
	{
		trace (TRACE_LEVEL_FATAL, string("Instance of the Mgmt I/F does not exist in the database ") + switchId);
		if (pResults != NULL)
		{
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			delete pSyncQueryCtx;
			return (IP_ACL_INTERNAL_ERROR);
		}
		else
		{
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
			delete pSyncQueryCtx;
			return (FRAMEWORK_ERROR); //TODO: It should be frame work error only if pRsults is NULL. Othercases it is IP acl module error. 
		}
	}
	else if (pResults->size() == 0)
	{
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		trace (TRACE_LEVEL_INFO, "Its a new ACL");
		isNewLocalMO = true;
	}
	else
	{
		pIpAclPolicyLocalMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>((*pResults)[0]);
		if (!pIpAclPolicyLocalMO) {
			trace (TRACE_LEVEL_ERROR, "IP ACL internal error");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
			delete pSyncQueryCtx;
			delete pResults;
			return (IP_ACL_INTERNAL_ERROR);
		}
	}
	delete pResults;
	delete pSyncQueryCtx;

// comment it for x86 testing
#if 1
	// Transaction Starts here. In error cases transaction should be aborted.
	ipf_txn_hdl_t txnHdl = ipfTransBegin();
	if (txnHdl <= 0) {
		if (pIpAclPolicyLocalMO) {
			delete pIpAclPolicyLocalMO;
		}
		return (IP_ACL_DB_LOCKED);
	}
	/* Enforce the ACL to the Mgmt I/F hardware (through IP Stack) */
	// Add the ACL Policy to SECd
	ret = ipfPolicyCreate(ipAclPolicy.c_str(), IPF_IPV6);
	if (ret) {
		trace (TRACE_LEVEL_ERROR, string("Creating the IPv4 policy failed: ipfPolicyCreate(): ") + ret);
		/*TODO: Need to take care the return values for transaction abort. */
		ipfTransAbort(txnHdl);
		if (pIpAclPolicyLocalMO) {
			delete pIpAclPolicyLocalMO;
		}
		return (IP_ACL_INTERNAL_ERROR);
	}
#endif //x86 ipv6 testing

	trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::enforceIp6AclPolicy - Get the rules in the ACL");

/* get the rules in the list */
        int i = 0;
        ExtIp6AclPolicyManagedObject *extPolicyMO = NULL;
        StdIp6AclPolicyManagedObject *stdPolicyMO = NULL;
        vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > extRuleMoVector;
        vector<WaveManagedObjectPointer<StdIp6AclRuleManagedObject> > stdRuleMoVector;

	if (policyType == STANDARD_IP_ACL)
	{
		stdPolicyMO = dynamic_cast<StdIp6AclPolicyManagedObject *>((*pResultsPMO)[0]);
		stdPolicyMO->sortRules();
		stdRuleMoVector = stdPolicyMO->getRules();
	}
	else if (policyType == EXTENDED_IP_ACL)
	{
		extPolicyMO = dynamic_cast<ExtIp6AclPolicyManagedObject *>((*pResultsPMO)[0]);
		extPolicyMO->sortRules();
		extRuleMoVector = extPolicyMO->getRules();
	}

	if (policyType == STANDARD_IP_ACL && stdPolicyMO != NULL)
	{
		trace(TRACE_LEVEL_INFO, string("ip access-list standard ") + stdPolicyMO->getPolicyName().c_str());

		for (i = 0; ((i < (int) stdRuleMoVector.size()) && (i < (IPF_MAX_NUM_RULES/2))); i++)
		{
			string stdSrcIp = "";

			if (stdRuleMoVector[i]->getAction () == SSMACL_FORWARDING_RESULT_PERMIT_E)
				action = IPF_ACCEPT;
			else if (stdRuleMoVector[i]->getAction () == SSMACL_FORWARDING_RESULT_SOFT_DROP_E)
				action = IPF_DROP;
			else if (stdRuleMoVector[i]->getAction () == SSMACL_FORWARDING_RESULT_HARD_DROP_E)
				action = IPF_DROP;

			if ((stdRuleMoVector[i]->getSrcUnion().compare("srcType") == 0)
					&& (stdRuleMoVector[i]->getSrcType() == IP_ADDR_TYPE_ANY))
				stdSrcIp = "any";
			else if ((stdRuleMoVector[i]->getSrcUnion().compare("srcIpAddress") == 0)
					&& (stdRuleMoVector[i]->getSrcType() == IP_ADDR_TYPE_NONE))
			{
				string SrcIpAddress = ((stdRuleMoVector[i]->getSrcIpAddress()).toString());
//				string SrcIpAddressMask = ((stdRuleMoVector[i]->getSrcIpAddressMask()).toString());
				stdSrcIp = SrcIpAddress.c_str();
//				stdSrcIp += " ";
//				stdSrcIp += SrcIpAddressMask.c_str();
			}
			else if ((stdRuleMoVector[i]->getSrcUnion().compare("srcType") == 0)
				&& (stdRuleMoVector[i]->getSrcType() == IP_ADDR_TYPE_HOST))
			{
//                        stdSrcIp = "host ";
				string SrcHostIpAddress = ((stdRuleMoVector[i]->getSrcHostIpAddress()).toString());
				stdSrcIp += SrcHostIpAddress.c_str();
			}
			trace(TRACE_LEVEL_INFO, string("std acl vals ") + ipAclPolicy + stdSrcIp);
			ret = addStdRulesBackdb(ipAclPolicy, stdRuleMoVector[i]->getRuleid(), action, stdSrcIp);
			if (ret) {
				trace (TRACE_LEVEL_ERROR, "addStdRulesBackDb failed");
				ipfTransAbort(txnHdl);
				delete pIpAclPolicyMO;
				if (pIpAclPolicyLocalMO) {
						delete pIpAclPolicyLocalMO;
				}
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
				return (IP_ACL_INTERNAL_ERROR);
			}

		}
	}



			if (policyType == EXTENDED_IP_ACL && extPolicyMO != NULL)
            {


                for (i = 0; i < (int) extRuleMoVector.size(); i++)
                {

                    string  extSrcIp    = "";
                    string  extDstIp    = "";
                    UI32    urg         = 0;
                    UI32    ack         = 0;
                    UI32    push        = 0;
                    UI32    fin         = 0;
                    UI32    rst         = 0;
                    UI32    sync        = 0;
                    //UI32    log         = 0;
                    //UI32    count       = 0;

                    /* Action set */
                    if (extRuleMoVector[i]->getAction () == SSMACL_FORWARDING_RESULT_PERMIT_E)
                        action = IPF_ACCEPT;
                    else if (extRuleMoVector[i]->getAction () == SSMACL_FORWARDING_RESULT_SOFT_DROP_E)
                        action = IPF_DROP;
                    else if (extRuleMoVector[i]->getAction () == SSMACL_FORWARDING_RESULT_HARD_DROP_E)
                        action = IPF_DROP;

                    /* Setting source parameters */
                    if ((extRuleMoVector[i]->getSrcUnion().compare("srcType") == 0)
                        && (extRuleMoVector[i]->getSrcType() == IP_ADDR_TYPE_ANY))
                        extSrcIp = "any";
                    else if ((extRuleMoVector[i]->getSrcUnion().compare("srcIpAddress") == 0)
                        && (extRuleMoVector[i]->getSrcType() == IP_ADDR_TYPE_NONE))
                    {
                        string SrcIpAddress = ((extRuleMoVector[i]->getSrcIpAddress()).toString());
//                        string SrcIpAddressMask = ((extRuleMoVector[i]->getSrcIpAddressMask()).toString());
                        extSrcIp = SrcIpAddress.c_str();
//                        extSrcIp += " ";
//                        extSrcIp += SrcIpAddressMask.c_str();
                    }
                    else if ((extRuleMoVector[i]->getSrcUnion().compare("srcType") == 0)
                        && (extRuleMoVector[i]->getSrcType() == IP_ADDR_TYPE_HOST))
                    {
                        string SrcHostIpAddress = ((extRuleMoVector[i]->getSrcHostIpAddress()).toString());
                        extSrcIp += SrcHostIpAddress.c_str();
                    }

                    /* Setting destination parameters */
                    if ((extRuleMoVector[i]->getDstUnion().compare("dstType") == 0)
                        && (extRuleMoVector[i]->getDstType() == IP_ADDR_TYPE_ANY))
                        extDstIp = "any";
                    else if ((extRuleMoVector[i]->getDstUnion().compare("dstIpAddress") == 0)
                        && (extRuleMoVector[i]->getDstType() == IP_ADDR_TYPE_NONE))
                    {
                        string DstIpAddress = ((extRuleMoVector[i]->getDstIpAddress()).toString());
//                        string DstIpAddressMask = ((extRuleMoVector[i]->getDstIpAddressMask()).toString());
                        extDstIp = DstIpAddress.c_str();
//                        extDstIp += " ";
//                        extDstIp += DstIpAddressMask.c_str();
                    }
                    else if ((extRuleMoVector[i]->getDstUnion().compare("dstType") == 0)
                        && (extRuleMoVector[i]->getDstType() == IP_ADDR_TYPE_HOST))
                    {
                        string DstHostIpAddress = ((extRuleMoVector[i]->getDstHostIpAddress()).toString());
                        extDstIp += DstHostIpAddress.c_str();
                    }

                    /* Setting prototype*/
                    string          protoType               = "";
                    UI32            customProtoType         = 0;
                    stringstream    customProtoTypeString;
					UI32			proto					= 0;

                    if (extRuleMoVector[i]->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_ICMP6)
                    {
					    protoType = "ipv6-icmp ";
						proto = L3L4_PROTOTYPE_ICMP6;
					}
                    else if (extRuleMoVector[i]->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_IP)
                    {
					    protoType = "ip ";
						proto = IPPROTO_IP;
					}
                    else if (extRuleMoVector[i]->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_TCP)
                    {
					    protoType = "tcp ";
						proto = IPPROTO_TCP;
					}
                    else if (extRuleMoVector[i]->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_UDP)
                    {
					    protoType = "udp ";
						proto = IPPROTO_UDP;
					}
                    else
                    {
                        customProtoType = extRuleMoVector[i]->getL3l4CustomProtoType();
						proto = customProtoType;
                    }

                    /* Set Port specific parameters */
                    string          srcPortOp           = "";
                    string          dstPortOp           = "";
                    UI32            srcPortNum          = 1;
                    UI32            dstPortNum          = 1;
                    UI32            srcHighNum          = IPF_MAX_PORTNUM-1;
                    UI32    		dstHighNum  		= IPF_MAX_PORTNUM-1;
					string			srcPort				= "";
					string			dstPort				= "";
					UI32			neqSportFlag		= 0;
					UI32			neqDportFlag		= 0;
					


                    if (extRuleMoVector[i]->getSrcPortOperator() == PORTOPERATOR_EQ)
                        srcPortOp = "eq ";
                    else if (extRuleMoVector[i]->getSrcPortOperator() == PORTOPERATOR_GT)
                        srcPortOp = "gt ";
                    else if (extRuleMoVector[i]->getSrcPortOperator() == PORTOPERATOR_LT)
                        srcPortOp = "lt ";
                    else if (extRuleMoVector[i]->getSrcPortOperator() == PORTOPERATOR_NEQ)
                        srcPortOp = "neq ";
                    else if (extRuleMoVector[i]->getSrcPortOperator() == PORTOPERATOR_RANGE)
                        srcPortOp = "range ";

                    if (extRuleMoVector[i]->getDstPortOperator() == PORTOPERATOR_EQ)
                        dstPortOp = "eq ";
                    else if (extRuleMoVector[i]->getDstPortOperator() == PORTOPERATOR_GT)
                        dstPortOp = "gt ";
                    else if (extRuleMoVector[i]->getDstPortOperator() == PORTOPERATOR_LT)
                        dstPortOp = "lt ";
                    else if (extRuleMoVector[i]->getDstPortOperator() == PORTOPERATOR_NEQ)
                        dstPortOp = "neq ";
                    else if (extRuleMoVector[i]->getDstPortOperator() == PORTOPERATOR_RANGE)
                        dstPortOp = "range ";


                    if ((srcPortOp.compare("range ") != 0) && (srcPortOp.compare("") != 0))
                    {
                        if ((srcPortOp.compare("lt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            srcPortNum = 1;
							srcHighNum = extRuleMoVector[i]->getSrcPortltWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcHighNum))
							{
                                srcHighNum = extRuleMoVector[i]->getSrcPortltCustomNumberUdp();
                            }
                        }
                        else if ((srcPortOp.compare("lt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            srcPortNum = 1;
							srcHighNum = extRuleMoVector[i]->getSrcPortltWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcHighNum))
							{
								srcHighNum = extRuleMoVector[i]->getSrcPortltCustomNumberTcp();
							}
                        }
                        else if ((srcPortOp.compare("gt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortgtWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
							{
								srcPortNum = extRuleMoVector[i]->getSrcPortgtCustomNumberUdp();
							}
							srcHighNum = (IPF_MAX_PORTNUM-1);
                        }
                        else if ((srcPortOp.compare("gt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortgtWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
							{
								srcPortNum = extRuleMoVector[i]->getSrcPortgtCustomNumberTcp();
							}
							srcHighNum = (IPF_MAX_PORTNUM-1);
                        }
                        else if ((srcPortOp.compare("eq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
							{
								srcPortNum = extRuleMoVector[i]->getSrcPortCustomNumberUdp();
							}
							srcHighNum = srcPortNum;
                        }
                        else if ((srcPortOp.compare("eq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
							{
								srcPortNum = extRuleMoVector[i]->getSrcPortCustomNumberTcp();
							}
							srcHighNum = srcPortNum;
                        }
                        else if ((srcPortOp.compare("neq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = extRuleMoVector[i]->getSrcPortCustomNumberUdp();
                            }
                            srcHighNum = srcPortNum;
							//if(action == IPF_ACCEPT)action = IPF_DROP;
							//else action = IPF_ACCEPT;
							neqSportFlag = 1;
                        }
                        else if ((srcPortOp.compare("neq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = extRuleMoVector[i]->getSrcPortCustomNumberTcp();
                            }
                            srcHighNum = srcPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqSportFlag = 1;
                        }
                    }
                    else if (srcPortOp.compare("range ") == 0)
                    {

                        if (protoType.compare("udp ") == 0)
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortWellknownrangeLowerUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
							{
								srcPortNum = extRuleMoVector[i]->getSrcPortCustomrangeLowerUdp();
							}
                            srcHighNum = extRuleMoVector[i]->getSrcPortWellknownrangeHigherUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcHighNum))
							{
								srcHighNum = extRuleMoVector[i]->getSrcPortCustomrangeHigherUdp();
							}
                        }
                        else if (protoType.compare("tcp ") == 0)
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortWellknownrangeLowerTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
							{
								srcPortNum = extRuleMoVector[i]->getSrcPortCustomrangeLowerTcp();
							}
                            srcHighNum = extRuleMoVector[i]->getSrcPortWellknownrangeHigherTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcHighNum))
							{
								srcHighNum = extRuleMoVector[i]->getSrcPortCustomrangeHigherTcp();
							}
                        }
                    }
					trace(TRACE_LEVEL_INFO, string("before dstPortOp.compare") + dstPortOp + protoType);
                    if ((dstPortOp.compare("range ") != 0) && (dstPortOp.compare("") != 0))
                    {
                        if ((dstPortOp.compare("lt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            dstPortNum = 1;
							dstHighNum = extRuleMoVector[i]->getDstPortltWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstHighNum))
                            {
                                srcHighNum = extRuleMoVector[i]->getDstPortltCustomNumberUdp();
							}
							
                        }
                        else if ((dstPortOp.compare("lt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            dstPortNum = 1;
							dstHighNum = extRuleMoVector[i]->getDstPortltWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstHighNum))
							{
								dstHighNum = extRuleMoVector[i]->getDstPortltCustomNumberTcp();
							}
                        }
                        else if ((dstPortOp.compare("gt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortgtWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
							{
								dstPortNum = extRuleMoVector[i]->getDstPortgtCustomNumberUdp();
							}
							dstHighNum = (IPF_MAX_PORTNUM-1);
                        }
                        else if ((dstPortOp.compare("gt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortgtWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
							{
								dstPortNum = extRuleMoVector[i]->getDstPortgtCustomNumberTcp();
							}
                            dstHighNum = (IPF_MAX_PORTNUM-1);
                        }
                        else if ((dstPortOp.compare("eq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
							{
								dstPortNum = extRuleMoVector[i]->getDstPortCustomNumberUdp();
							}
							dstHighNum = dstPortNum;
                        }
                        else if ((dstPortOp.compare("eq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
							{
								dstPortNum = extRuleMoVector[i]->getDstPortCustomNumberTcp();
							}
							dstHighNum = dstPortNum;
                        }
                        else if ((dstPortOp.compare("neq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = extRuleMoVector[i]->getDstPortCustomNumberUdp();
                            }
                            dstHighNum = dstPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqDportFlag = 1;
                        }
                        else if ((dstPortOp.compare("neq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = extRuleMoVector[i]->getDstPortCustomNumberTcp();
                            }
                            dstHighNum = dstPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqDportFlag = 1;
                        }
                    }
                    else if (dstPortOp.compare("range ") == 0)
                    {
trace(TRACE_LEVEL_INFO, string("Entering in the else if case"));
                        if (protoType.compare("udp ") == 0)
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortWellknownrangeLowerUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
							{
								dstPortNum = extRuleMoVector[i]->getDstPortCustomrangeLowerUdp();
							}
                            dstHighNum = extRuleMoVector[i]->getDstPortWellknownrangeHigherUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstHighNum))
							{
								dstHighNum = extRuleMoVector[i]->getDstPortCustomrangeHigherUdp();
							}
                        }
                        else if (protoType.compare("tcp ") == 0)
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortWellknownrangeLowerTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = extRuleMoVector[i]->getDstPortCustomrangeLowerTcp();
                            }
                            dstHighNum = extRuleMoVector[i]->getDstPortWellknownrangeHigherTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstHighNum))
							{
								dstHighNum = extRuleMoVector[i]->getDstPortCustomrangeHigherTcp();
							}
                        }
                    }

                    /* Setting precedence, tos, dscp */
                    string  PrecDscpTos     = "";
                    UI32    PrecDscpTosNum  = 0;

                    PrecDscpTosNum = extRuleMoVector[i]->getWellknownPrecedence();
                    PrecDscpTos = IsWellKnownPrecedence(PrecDscpTosNum);
                    if (PrecDscpTos.compare("") == 0)
                    {
                        stringstream PrecString;
                        PrecString << extRuleMoVector[i]->getCustomPrecedence();
						PrecDscpTosNum = extRuleMoVector[i]->getCustomPrecedence();
                        PrecDscpTos = PrecString.str();
                    }

                    if ((PrecDscpTos.compare("") != 0) && (PrecDscpTos.compare("64") != 0))
                    {
                        PrecDscpTos = "precedence " + PrecDscpTos;
                        //PrecDscpTos += " ";
                    }
                    else
                    {
                        PrecDscpTos = "";
                        PrecDscpTosNum = extRuleMoVector[i]->getWellknownDscp();
                        PrecDscpTos = IsWellKnownDscp(PrecDscpTosNum);
                        if (PrecDscpTos.compare("") == 0)
                        {
                            stringstream DscpString;
                            DscpString << extRuleMoVector[i]->getCustomDscp();
							PrecDscpTosNum = extRuleMoVector[i]->getCustomDscp();
                            PrecDscpTos = DscpString.str();
                        }

                        if ((PrecDscpTos.compare("") != 0) && (PrecDscpTos.compare("64") != 0))
                        {
                            PrecDscpTos = "dscp " + PrecDscpTos;
                            //PrecDscpTos += " ";
                        }
                        else
                        {
                            PrecDscpTos = "";
                            PrecDscpTosNum = extRuleMoVector[i]->getWellknownTos();
                            PrecDscpTos = IsWellKnownTos(PrecDscpTosNum);
                          if (PrecDscpTos.compare("") == 0)
                            {
                                stringstream TosString;
                                TosString << extRuleMoVector[i]->getCustomTos();
								PrecDscpTosNum = extRuleMoVector[i]->getCustomTos();
                                PrecDscpTos = TosString.str();
                            }

                            if ((PrecDscpTos.compare("") != 0) && (PrecDscpTos.compare("64") != 0))
                            {
                                PrecDscpTos = "tos " + PrecDscpTos;
                                //PrecDscpTos += " ";
                            }
                            else
                                PrecDscpTos = "";
                        }
                    }

                    /* setting ack, fin, rst, sync */
                    urg = extRuleMoVector[i]->getUrg();
                    ack = extRuleMoVector[i]->getAck();
                    push = extRuleMoVector[i]->getPush();
                    fin = extRuleMoVector[i]->getFin();
                    rst = extRuleMoVector[i]->getRst();
                    sync = extRuleMoVector[i]->getSync();

                    /* count ang log parameter */
                    //count = extRuleMoVector[i]->getIsCount();
                    //log = extRuleMoVector[i]->getIsLog();


                    trace(TRACE_LEVEL_INFO, string("Ext acl vals: ") + ipAclPolicy + action + extSrcIp + dstPortNum + dstHighNum + proto + extDstIp + neqSportFlag + neqDportFlag);
					//ret = addExtRulesBackdb(ipAclPolicy, extRuleMoVector[i]->getRuleid(), action, extSrcIp, dstPortNum, dstHighNum, proto);
					ret = addExtRulesBackdbOptimized(ipAclPolicy, extRuleMoVector[i]->getRuleid(),
							action, extSrcIp, dstPortNum, dstHighNum, extDstIp, srcPortNum,
							srcHighNum, proto, PrecDscpTos, urg, ack, push, fin, rst, sync, neqSportFlag,
							neqDportFlag);
                    if (ret) {
                        trace (TRACE_LEVEL_ERROR, "addExtRulesBackDb failed");
                        ipfTransAbort(txnHdl);
                        delete pIpAclPolicyMO;
                        if (pIpAclPolicyLocalMO) {
                            delete pIpAclPolicyLocalMO;
                        }
                        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
                        return (IP_ACL_INTERNAL_ERROR);
                    }

                }
            }


	if (pResultsPMO) {
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
	}
#if 1
	/* enforceEmptyPolicyRules if there are no rules in the policy, backend will not activate empty policy */ 
	if ((((int) stdRuleMoVector.size()) == 0) && (((int) extRuleMoVector.size()) == 0))
	{
		ret = enforceEmptyPolicyRules(ipAclPolicy);
		if (ret)
		{
			ipfTransAbort(txnHdl);
			if (pIpAclPolicyLocalMO)
			{
				delete pIpAclPolicyLocalMO;
			}
			trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::enforceIpAclPolicy : enforceEmptyPolicyRules Failed");
			return (IP_ACL_INTERNAL_ERROR);
		}
	}


	// Activate the newly added policy
	ret = ipfPolicyActivate(ipAclPolicy.c_str());
	if (!ret) {
		trace(TRACE_LEVEL_INFO, "enforceIpAclPolicy....before isStoredb");
		if (isStoreDb == TRUE) {
			if (isNewLocalMO ==  false) {	
			// Remove the old non-default policy, if active/exists
				string oldPolicyName = pIpAclPolicyLocalMO->getIpv6AclPolicy();
				ret = ipfPolicyDelete(oldPolicyName.c_str());
				if (ret) {
					//TODO:After activating the new policy what todo if we couldn't delete the exisitng policy	
					ipfTransAbort(txnHdl);
					if (pIpAclPolicyLocalMO) {
						delete pIpAclPolicyLocalMO;
					}
					return (IP_ACL_INTERNAL_ERROR);
				}
			}

			/* Make changes to the IPAdm MO instance in the database */
			trace(TRACE_LEVEL_INFO, "Make changes to the IPAdm MO instance in the database");
			startTransaction();
			if (isNewLocalMO ==  false) {
				updateWaveManagedObject(pIpAclPolicyLocalMO);
			} else {
				trace(TRACE_LEVEL_INFO, "ITs a new  ACL");
				pIpAclPolicyLocalMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>(createManagedObjectInstance(Ip6AclPolicyLocalManagedObject::getClassName()));
				pIpAclPolicyLocalMO->setSwitchId(switchId);
			}
			pIpAclPolicyLocalMO->setIpv6AclPolicy(ipAclPolicy);
			pIpAclPolicyLocalMO->setIpv6AclTrafFlow(ACL_TRAFFIC_IN);
			pIpAclPolicyLocalMO->setIpv6AclType(policyType);
			trace(TRACE_LEVEL_INFO, "commitTransaction of ACL");
			status = commitTransaction();

			if (getItfIdfromSwitchId(switchId) != 0)
			{
				trace(TRACE_LEVEL_INFO, "Add on the standby interface");
				// Add onto the other itf
				string switchId2;
				int localId = DcmToolKit::getLocalMappedId();
				if (getItfIdfromSwitchId(switchId) == 1) {
					switchId2 = localId + string ("/") + 2;
				} else {
					switchId2 = localId + string ("/") + 1;
				}
				pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyLocalManagedObject::getClassName());
				pSyncQueryCtx->addAndAttribute (new AttributeString (switchId2, "switchid"));
				vector<WaveManagedObject *> *pResultsStdby = querySynchronously (pSyncQueryCtx);
				delete pSyncQueryCtx;
				if (pResultsStdby)
				{
						startTransaction();
						if (isNewLocalMO ==  false) {
							if (pResultsStdby->size() == 1)
							{
								pIpAclPolicyStandbyMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>((*pResultsStdby)[0]);
							}
							if (pIpAclPolicyStandbyMO) {
								trace(TRACE_LEVEL_INFO, "Updating standby ACL entry");
								updateWaveManagedObject(pIpAclPolicyStandbyMO);
							}
							else
							{
								trace(TRACE_LEVEL_INFO, "Error fetching standby ACL MO");
								return (FRAMEWORK_ERROR);
							}
						} else {
							trace(TRACE_LEVEL_INFO, "ITs a new  ACL in Standby");
							pIpAclPolicyStandbyMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>(createManagedObjectInstance(Ip6AclPolicyLocalManagedObject::getClassName()));
							pIpAclPolicyStandbyMO->setSwitchId(switchId2);
						}
						pIpAclPolicyStandbyMO->setIpv6AclPolicy(ipAclPolicy);
						pIpAclPolicyStandbyMO->setIpv6AclTrafFlow(ACL_TRAFFIC_IN);
						pIpAclPolicyStandbyMO->setIpv6AclType(policyType);
						trace(TRACE_LEVEL_INFO, "commitTransaction of ACL");
						status = commitTransaction();
				}
			}
		}
		//TODO: Need to handle these error cases ????? 
		ipfPolicySave(NULL);
		ipfTransEnd(txnHdl);
	} else {
		ipfTransAbort(txnHdl);
		trace (TRACE_LEVEL_ERROR, string("Activating the new policy failed: ipfPolicyActivate(): ") + ret);
		status = IP_ACL_INTERNAL_ERROR;
	}
#endif
// remove the below commit code after testing in x86 build
#if 0
            trace(TRACE_LEVEL_INFO, "Make changes to the IPAdm MO instance in the database");
            startTransaction();
            if (isNewLocalMO ==  false) {
                updateWaveManagedObject(pIpAclPolicyLocalMO);
            } else {
                trace(TRACE_LEVEL_INFO, "ITs a new  ACL");
                pIpAclPolicyLocalMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>(createManagedObjectInstance(Ip6AclPolicyLocalManagedObject::getClassName()));
                pIpAclPolicyLocalMO->setSwitchId(switchId);
            }
            pIpAclPolicyLocalMO->setIpv6AclPolicy(ipAclPolicy);
            pIpAclPolicyLocalMO->setIpv6AclTrafFlow(ACL_TRAFFIC_IN);
            trace(TRACE_LEVEL_INFO, "commitTransaction of ACL");
            status = commitTransaction();
            // Add onto the other itf
            string switchId2("1/2");
            startTransaction();
            if (isNewLocalMO ==  false) {
                updateWaveManagedObject(pIpAclPolicyLocalMO);
            } else {
                trace(TRACE_LEVEL_INFO, "ITs a new  ACL");
                pIpAclPolicyLocalMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>(createManagedObjectInstance(Ip6AclPolicyLocalManagedObject::getClassName()));
                pIpAclPolicyLocalMO->setSwitchId(switchId2);
            }
            pIpAclPolicyLocalMO->setIpv6AclPolicy(ipAclPolicy);
            pIpAclPolicyLocalMO->setIpv6AclTrafFlow(ACL_TRAFFIC_IN);
            trace(TRACE_LEVEL_INFO, "commitTransaction of ACL");
            status = commitTransaction();

#endif
// removed till here


	if ((status == FRAMEWORK_SUCCESS) || (status == WAVE_MESSAGE_SUCCESS))
	{
		trace (TRACE_LEVEL_INFO, "Making changes for the rule in the DB is successful.");
		status = WAVE_MESSAGE_SUCCESS;
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Making changes for the rule in the database FAILED!!.");
	}

	delete pIpAclPolicyMO;
	if (pIpAclPolicyLocalMO) {
		delete pIpAclPolicyLocalMO;
	}
	if (pIpAclPolicyStandbyMO) {
		delete pIpAclPolicyStandbyMO;
	}

	trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::enforceIp6AclPolicy()");
	return (status);
}

int  LocalIpAclWorker::enforceIpAclPolicy(string ipAclPolicy, string switchId, bool isStoreDb, IpAclVersion version) 
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::enforceIpAclPolicy()");
	IpAclPolicyManagedObject *pIpAclPolicyMO = NULL;
	IpAclPolicyLocalManagedObject *pIpAclPolicyLocalMO = NULL, *pIpAclPolicyStandbyMO = NULL;
	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResultsPMO = NULL;
	UI8 action = 0;

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	bool isNewLocalMO = false;

	UI32	policyType = SSMACL_TYPE_UNKNOWN_E;
	//ObjectId	policyObjId = ObjectId::NullObjectId;


	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIpAclPolicyManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (ipAclPolicy, "policyName"));
	pResultsPMO = querySynchronously (pSyncQueryCtx);
	if (!pResultsPMO)
	{
		delete pSyncQueryCtx;
		return (FRAMEWORK_ERROR);
	}
	else if (pResultsPMO->size () == 1)
	{
		trace (TRACE_LEVEL_INFO, "its an SSMACL_TYPE_L3_IPV4_STD_E");
		policyType = SSMACL_TYPE_L3_IPV4_STD_E;
	}
	else
	{
		/* the policy is not standard, we will check if its an extended policy */
		delete pResultsPMO;
		delete pSyncQueryCtx;
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIpAclPolicyManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeString (ipAclPolicy, "policyName"));
		pResultsPMO = querySynchronously (pSyncQueryCtx);
		if (!pResultsPMO)
		{
			delete pSyncQueryCtx;
			return (FRAMEWORK_ERROR);
		}
		else if (pResultsPMO->size () == 1)
		{
			trace (TRACE_LEVEL_INFO, "its an SSMACL_TYPE_L3_IPV4_EXT_E");
			policyType = SSMACL_TYPE_L3_IPV4_EXT_E;
		}
	}
	delete pSyncQueryCtx;

	/* Get the MO instance of the Mgmt I/F */
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyLocalManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (switchId, "switchid"));
	vector<WaveManagedObject *> *pResults = querySynchronously (pSyncQueryCtx);
	// Size should be 0 or 1, If the size is 0, no instance is created(add operation)
	// If size is 1 instance is already there and changing the existing policy now for the instance
	// (Change operation).
	if (!pResults || (pResults->size() > 1))
	{
		trace (TRACE_LEVEL_FATAL, string("Instance of the Mgmt I/F does not exist in the database ") + switchId);
		if (pResults != NULL)
		{
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			delete pSyncQueryCtx;
			return (IP_ACL_INTERNAL_ERROR);
		}
		else
		{
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
			delete pSyncQueryCtx;
			return (FRAMEWORK_ERROR); //TODO: It should be frame work error only if pRsults is NULL. Othercases it is IP acl module error. 
		}
	}
	else if (pResults->size() == 0)
	{
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		trace (TRACE_LEVEL_INFO, "Its a new ACL");
		isNewLocalMO = true;
	}
	else
	{
		pIpAclPolicyLocalMO = dynamic_cast<IpAclPolicyLocalManagedObject *>((*pResults)[0]);
		if (!pIpAclPolicyLocalMO) {
			trace (TRACE_LEVEL_ERROR, "IP ACL internal error");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
			delete pSyncQueryCtx;
			delete pResults;
			return (IP_ACL_INTERNAL_ERROR);
		}
	}
	delete pResults;
	delete pSyncQueryCtx;

	// Transaction Starts here. In error cases transaction should be aborted.
	ipf_txn_hdl_t txnHdl = ipfTransBegin();
	if (txnHdl <= 0) {
		if (pIpAclPolicyLocalMO) {
			delete pIpAclPolicyLocalMO;
		}
		return (IP_ACL_DB_LOCKED);
	}
	/* Enforce the ACL to the Mgmt I/F hardware (through IP Stack) */
	// Add the ACL Policy to SECd
	int ret = ipfPolicyCreate(ipAclPolicy.c_str(), IPF_IPV4);
	if (ret) {
		trace (TRACE_LEVEL_ERROR, string("Creating the IPv4 policy failed: ipfPolicyCreate(): ") + ret);
		/*TODO: Need to take care the return values for transaction abort. */
		ipfTransAbort(txnHdl);
		if (pIpAclPolicyLocalMO) {
			delete pIpAclPolicyLocalMO;
		}
		return (IP_ACL_INTERNAL_ERROR);
	}

	trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::enforceIpAclPolicy - Get the rules in the ACL");

/* get the rules in the list */
        int i = 0;
        ExtIpAclPolicyManagedObject *extPolicyMO = NULL;
        StdIpAclPolicyManagedObject *stdPolicyMO = NULL;
        vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> > extRuleMoVector;
        vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> > stdRuleMoVector;

	if (policyType == SSMACL_TYPE_L3_IPV4_STD_E)
	{
		stdPolicyMO = dynamic_cast<StdIpAclPolicyManagedObject *>((*pResultsPMO)[0]);
		stdPolicyMO->sortRules();
		stdRuleMoVector = stdPolicyMO->getRules();
	}
	else if (policyType == SSMACL_TYPE_L3_IPV4_EXT_E)
	{
		extPolicyMO = dynamic_cast<ExtIpAclPolicyManagedObject *>((*pResultsPMO)[0]);
		extPolicyMO->sortRules();
		extRuleMoVector = extPolicyMO->getRules();
	}

	if (policyType == SSMACL_TYPE_L3_IPV4_STD_E && stdPolicyMO != NULL)
	{
		trace(TRACE_LEVEL_INFO, string("ip access-list standard ") + stdPolicyMO->getPolicyName().c_str());

		for (i = 0; ((i < (int) stdRuleMoVector.size()) && (i < (IPF_MAX_NUM_RULES/2))); i++)
		{
			string stdSrcIp = "";

			if (stdRuleMoVector[i]->getAction () == SSMACL_FORWARDING_RESULT_PERMIT_E)
				action = IPF_ACCEPT;
			else if (stdRuleMoVector[i]->getAction () == SSMACL_FORWARDING_RESULT_SOFT_DROP_E)
				action = IPF_DROP;
			else if (stdRuleMoVector[i]->getAction () == SSMACL_FORWARDING_RESULT_HARD_DROP_E)
				action = IPF_DROP;

			if ((stdRuleMoVector[i]->getSrcUnion().compare("srcType") == 0)
					&& (stdRuleMoVector[i]->getSrcType() == IP_ADDR_TYPE_ANY))
				stdSrcIp = "any";
			else if ((stdRuleMoVector[i]->getSrcUnion().compare("srcIpAddress") == 0)
					&& (stdRuleMoVector[i]->getSrcType() == IP_ADDR_TYPE_NONE))
			{
				string SrcIpAddress = ((stdRuleMoVector[i]->getSrcIpAddress()).toString());
				string SrcIpAddressMask = ((stdRuleMoVector[i]->getSrcIpAddressMask()).toString());
				stdSrcIp = SrcIpAddress.c_str();
                string cidrMask = netmaskToCidr(SrcIpAddressMask);
                if(cidrMask.length()!=0)
                    stdSrcIp += (string("/") + cidrMask);
			}
			else if ((stdRuleMoVector[i]->getSrcUnion().compare("srcType") == 0)
				&& (stdRuleMoVector[i]->getSrcType() == IP_ADDR_TYPE_HOST))
			{
//                        stdSrcIp = "host ";
				string SrcHostIpAddress = ((stdRuleMoVector[i]->getSrcHostIpAddress()).toString());
				stdSrcIp += SrcHostIpAddress.c_str();
			}
			trace(TRACE_LEVEL_INFO, string("std acl vals ") + ipAclPolicy + stdSrcIp);
			ret = addStdRulesBackdb(ipAclPolicy, stdRuleMoVector[i]->getRuleid(), action, stdSrcIp);
			if (ret) {
				trace (TRACE_LEVEL_ERROR, "addStdRulesBackDb failed");
				ipfTransAbort(txnHdl);
				delete pIpAclPolicyMO;
				if (pIpAclPolicyLocalMO) {
						delete pIpAclPolicyLocalMO;
				}
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
				return (IP_ACL_INTERNAL_ERROR);
			}

		}
	}



			if (policyType == SSMACL_TYPE_L3_IPV4_EXT_E && extPolicyMO != NULL)
            {


                for (i = 0; i < (int) extRuleMoVector.size() && (i < IPF_MAX_NUM_RULES); i++)
                {

                    string  extSrcIp    = "";
                    string  extDstIp    = "";
                    UI32    urg         = 0;
                    UI32    ack         = 0;
                    UI32    push        = 0;
                    UI32    fin         = 0;
                    UI32    rst         = 0;
                    UI32    sync        = 0;
                    //UI32    log         = 0;
                    //UI32    count       = 0;

                    /* Action set */
                    if (extRuleMoVector[i]->getAction () == SSMACL_FORWARDING_RESULT_PERMIT_E)
                        action = IPF_ACCEPT;
                    else if (extRuleMoVector[i]->getAction () == SSMACL_FORWARDING_RESULT_SOFT_DROP_E)
                        action = IPF_DROP;
                    else if (extRuleMoVector[i]->getAction () == SSMACL_FORWARDING_RESULT_HARD_DROP_E)
                        action = IPF_DROP;

                    /* Setting source parameters */
                    if ((extRuleMoVector[i]->getSrcUnion().compare("srcType") == 0)
                        && (extRuleMoVector[i]->getSrcType() == IP_ADDR_TYPE_ANY))
                        extSrcIp = "any";
                    else if ((extRuleMoVector[i]->getSrcUnion().compare("srcIpAddress") == 0)
                        && (extRuleMoVector[i]->getSrcType() == IP_ADDR_TYPE_NONE))
                    {
                        string SrcIpAddress = ((extRuleMoVector[i]->getSrcIpAddress()).toString());
                        string SrcIpAddressMask = ((extRuleMoVector[i]->getSrcIpAddressMask()).toString());
                        extSrcIp = SrcIpAddress.c_str();
                        string cidrMask = netmaskToCidr(SrcIpAddressMask);
                        if(cidrMask.length()!=0)
                            extSrcIp += (string("/") + cidrMask);
                    }
                    else if ((extRuleMoVector[i]->getSrcUnion().compare("srcType") == 0)
                        && (extRuleMoVector[i]->getSrcType() == IP_ADDR_TYPE_HOST))
                    {
                        string SrcHostIpAddress = ((extRuleMoVector[i]->getSrcHostIpAddress()).toString());
                        extSrcIp += SrcHostIpAddress.c_str();
                    }

                    /* Setting destination parameters */
                    if ((extRuleMoVector[i]->getDstUnion().compare("dstType") == 0)
                        && (extRuleMoVector[i]->getDstType() == IP_ADDR_TYPE_ANY))
                        extDstIp = "any";
                    else if ((extRuleMoVector[i]->getDstUnion().compare("dstIpAddress") == 0)
                        && (extRuleMoVector[i]->getDstType() == IP_ADDR_TYPE_NONE))
                    {
                        string DstIpAddress = ((extRuleMoVector[i]->getDstIpAddress()).toString());
                        string DstIpAddressMask = ((extRuleMoVector[i]->getDstIpAddressMask()).toString());
                        extDstIp = DstIpAddress.c_str();
                        string cidrMask = netmaskToCidr(DstIpAddressMask);
                        if(cidrMask.length()!=0)
                            extDstIp += (string("/") + cidrMask); 
                    }
                    else if ((extRuleMoVector[i]->getDstUnion().compare("dstType") == 0)
                        && (extRuleMoVector[i]->getDstType() == IP_ADDR_TYPE_HOST))
                    {
                        string DstHostIpAddress = ((extRuleMoVector[i]->getDstHostIpAddress()).toString());
                        extDstIp += DstHostIpAddress.c_str();
                    }

                    /* Setting prototype*/
                    string          protoType               = "";
                    UI32            customProtoType         = 0;
                    stringstream    customProtoTypeString;
					UI32			proto					= 0;

                    if (extRuleMoVector[i]->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_ICMP)
                    {
					    protoType = "icmp ";
						proto = IPPROTO_ICMP;
					}
                    else if (extRuleMoVector[i]->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_IP)
                    {
					    protoType = "ip ";
						proto = IPPROTO_IP;
					}
                    else if (extRuleMoVector[i]->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_TCP)
                    {
					    protoType = "tcp ";
						proto = IPPROTO_TCP;
					}
                    else if (extRuleMoVector[i]->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_UDP)
                    {
					    protoType = "udp ";
						proto = IPPROTO_UDP;
					}
                    else
                    {
                        customProtoType = extRuleMoVector[i]->getL3l4CustomProtoType();
						proto = customProtoType;
                    }

                    /* Set Port specific parameters */
                    string          srcPortOp           = "";
                    string          dstPortOp           = "";
                    UI32            srcPortNum          = 1;
                    UI32            dstPortNum          = 1;
                    UI32            srcHighNum          = IPF_MAX_PORTNUM-1;
                    UI32    		dstHighNum  		= IPF_MAX_PORTNUM-1;
					UI32			neqSportFlag		= 0;
					UI32			neqDportFlag		= 0;

                    if (extRuleMoVector[i]->getSrcPortOperator() == PORTOPERATOR_EQ)
                        srcPortOp = "eq ";
                    else if (extRuleMoVector[i]->getSrcPortOperator() == PORTOPERATOR_GT)
                        srcPortOp = "gt ";
                    else if (extRuleMoVector[i]->getSrcPortOperator() == PORTOPERATOR_LT)
                        srcPortOp = "lt ";
                    else if (extRuleMoVector[i]->getSrcPortOperator() == PORTOPERATOR_NEQ)
                        srcPortOp = "neq ";
                    else if (extRuleMoVector[i]->getSrcPortOperator() == PORTOPERATOR_RANGE)
                        srcPortOp = "range ";

                    if (extRuleMoVector[i]->getDstPortOperator() == PORTOPERATOR_EQ)
                        dstPortOp = "eq ";
                    else if (extRuleMoVector[i]->getDstPortOperator() == PORTOPERATOR_GT)
                        dstPortOp = "gt ";
                    else if (extRuleMoVector[i]->getDstPortOperator() == PORTOPERATOR_LT)
                        dstPortOp = "lt ";
                    else if (extRuleMoVector[i]->getDstPortOperator() == PORTOPERATOR_NEQ)
                        dstPortOp = "neq ";
                    else if (extRuleMoVector[i]->getDstPortOperator() == PORTOPERATOR_RANGE)
                        dstPortOp = "range ";

                    if ((srcPortOp.compare("range ") != 0) && (srcPortOp.compare("") != 0))
                    {
                        if ((srcPortOp.compare("lt ") == 0) && (protoType.compare("udp ") == 0))
                        {
							srcHighNum = extRuleMoVector[i]->getSrcPortltWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcHighNum))
                            {
                                srcHighNum = extRuleMoVector[i]->getSrcPortltCustomNumberUdp();
                            }
                        }
                        else if ((srcPortOp.compare("lt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
							srcHighNum = extRuleMoVector[i]->getSrcPortltWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcHighNum))
                            {
                                srcHighNum = extRuleMoVector[i]->getSrcPortltCustomNumberTcp();
                            }
                        }
                        else if ((srcPortOp.compare("gt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortgtWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = extRuleMoVector[i]->getSrcPortgtCustomNumberUdp();
                            }
                        }
                        else if ((srcPortOp.compare("gt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortgtWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = extRuleMoVector[i]->getSrcPortgtCustomNumberTcp();
                            }
                        }
                        else if ((srcPortOp.compare("eq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = extRuleMoVector[i]->getSrcPortCustomNumberUdp();
                            }
							srcHighNum = srcPortNum;
                        }
                        else if ((srcPortOp.compare("eq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = extRuleMoVector[i]->getSrcPortCustomNumberTcp();
                            }
							srcHighNum = srcPortNum;
                        }
                        else if ((srcPortOp.compare("neq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = extRuleMoVector[i]->getSrcPortCustomNumberUdp();
                            }
                            srcHighNum = srcPortNum;
							//if(action == IPF_ACCEPT)action = IPF_DROP;
							//else action = IPF_ACCEPT;
							neqSportFlag = 1;
                        }
                        else if ((srcPortOp.compare("neq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = extRuleMoVector[i]->getSrcPortCustomNumberTcp();
                            }
                            srcHighNum = srcPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqSportFlag = 1;
                        }
                    }
                    else if (srcPortOp.compare("range ") == 0)
                    {

                        if (protoType.compare("udp ") == 0)
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortWellknownrangeLowerUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = extRuleMoVector[i]->getSrcPortCustomrangeLowerUdp();
                            }
                            srcHighNum = extRuleMoVector[i]->getSrcPortWellknownrangeHigherUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcHighNum))
                            {
                                srcHighNum = extRuleMoVector[i]->getSrcPortCustomrangeHigherUdp();
                            }
                        }
                        else if (protoType.compare("tcp ") == 0)
                        {
                            srcPortNum = extRuleMoVector[i]->getSrcPortWellknownrangeLowerTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = extRuleMoVector[i]->getSrcPortCustomrangeLowerTcp();
                            }
                            srcHighNum = extRuleMoVector[i]->getSrcPortWellknownrangeHigherTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcHighNum))
                            {
                                srcHighNum = extRuleMoVector[i]->getSrcPortCustomrangeHigherTcp();
                            }
                        }
                    }

                    if ((dstPortOp.compare("range ") != 0) && (dstPortOp.compare("") != 0))
                    {
                        if ((dstPortOp.compare("lt ") == 0) && (protoType.compare("udp ") == 0))
                        {
							dstHighNum = extRuleMoVector[i]->getDstPortltWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstHighNum))
                            {
                                srcHighNum = extRuleMoVector[i]->getDstPortltCustomNumberUdp();
                            }
                        }
                        else if ((dstPortOp.compare("lt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
							dstHighNum = extRuleMoVector[i]->getDstPortltWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstHighNum))
                            {
                                dstHighNum = extRuleMoVector[i]->getDstPortltCustomNumberTcp();
                            }
                        }
                        else if ((dstPortOp.compare("gt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortgtWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = extRuleMoVector[i]->getDstPortgtCustomNumberUdp();
                            }
                        }
                        else if ((dstPortOp.compare("gt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortgtWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = extRuleMoVector[i]->getDstPortgtCustomNumberTcp();
                            }
                        }
                        else if ((dstPortOp.compare("eq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = extRuleMoVector[i]->getDstPortCustomNumberUdp();
                            }
							dstHighNum = dstPortNum;
                        }
                        else if ((dstPortOp.compare("eq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = extRuleMoVector[i]->getDstPortCustomNumberTcp();
                            }
							dstHighNum = dstPortNum;
                        }
                        else if ((dstPortOp.compare("neq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = extRuleMoVector[i]->getDstPortCustomNumberUdp();
                            }
                            dstHighNum = dstPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqDportFlag = 1;
                        }
                        else if ((dstPortOp.compare("neq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = extRuleMoVector[i]->getDstPortCustomNumberTcp();
                            }
                            dstHighNum = dstPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqDportFlag = 1;
                        }
                    }
                    else if (dstPortOp.compare("range ") == 0)
                    {

                        if (protoType.compare("udp ") == 0)
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortWellknownrangeLowerUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = extRuleMoVector[i]->getDstPortCustomrangeLowerUdp();
                            }
                            dstHighNum = extRuleMoVector[i]->getDstPortWellknownrangeHigherUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstHighNum))
                            {
                                dstHighNum = extRuleMoVector[i]->getDstPortCustomrangeHigherUdp();
                            }
                        }
                        else if (protoType.compare("tcp ") == 0)
                        {
                            dstPortNum = extRuleMoVector[i]->getDstPortWellknownrangeLowerTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = extRuleMoVector[i]->getDstPortCustomrangeLowerTcp();
                            }
                            dstHighNum = extRuleMoVector[i]->getDstPortWellknownrangeHigherTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstHighNum))
                            {
                                dstHighNum = extRuleMoVector[i]->getDstPortCustomrangeHigherTcp();
                            }
                        }
                    }

                    /* Setting precedence, tos, dscp */
                    string  PrecDscpTos     = "";
                    UI32    PrecDscpTosNum  = 0;

                    PrecDscpTosNum = extRuleMoVector[i]->getWellknownPrecedence();
                    PrecDscpTos = IsWellKnownPrecedence(PrecDscpTosNum);
                    if (PrecDscpTos.compare("") == 0)
                    {
                        stringstream PrecString;
                        PrecString << extRuleMoVector[i]->getCustomPrecedence();
						PrecDscpTosNum = extRuleMoVector[i]->getCustomPrecedence();
                        PrecDscpTos = PrecString.str();
                    }

                    if ((PrecDscpTos.compare("") != 0) && (PrecDscpTos.compare("64") != 0))
                    {
                        PrecDscpTos = "precedence " + PrecDscpTos;
                        //PrecDscpTos += " ";
                    }
                    else
                    {
                        PrecDscpTos = "";
                        PrecDscpTosNum = extRuleMoVector[i]->getWellknownDscp();
                        PrecDscpTos = IsWellKnownDscp(PrecDscpTosNum);
                        if (PrecDscpTos.compare("") == 0)
                        {
                            stringstream DscpString;
                            DscpString << extRuleMoVector[i]->getCustomDscp();
							PrecDscpTosNum = extRuleMoVector[i]->getCustomDscp();
                            PrecDscpTos = DscpString.str();
                        }

                        if ((PrecDscpTos.compare("") != 0) && (PrecDscpTos.compare("64") != 0))
                        {
                            PrecDscpTos = "dscp " + PrecDscpTos;
                            //PrecDscpTos += " ";
                        }
                        else
                        {
                            PrecDscpTos = "";
                            PrecDscpTosNum = extRuleMoVector[i]->getWellknownTos();
                            PrecDscpTos = IsWellKnownTos(PrecDscpTosNum);
                          if (PrecDscpTos.compare("") == 0)
                            {
                                stringstream TosString;
                                TosString << extRuleMoVector[i]->getCustomTos();
								PrecDscpTosNum = extRuleMoVector[i]->getCustomTos();
                                PrecDscpTos = TosString.str();
                            }

                            if ((PrecDscpTos.compare("") != 0) && (PrecDscpTos.compare("64") != 0))
                            {
                                PrecDscpTos = "tos " + PrecDscpTos;
                                //PrecDscpTos += " ";
                            }
                            else {
                                PrecDscpTos = "";
								PrecDscpTosNum = 0;
							}
                        }
                    }

                    /* setting ack, fin, rst, sync */
                    urg = extRuleMoVector[i]->getUrg();
                    ack = extRuleMoVector[i]->getAck();
                    push = extRuleMoVector[i]->getPush();
                    fin = extRuleMoVector[i]->getFin();
                    rst = extRuleMoVector[i]->getRst();
                    sync = extRuleMoVector[i]->getSync();

                    /* count ang log parameter */
                    //count = extRuleMoVector[i]->getIsCount();
                    //log = extRuleMoVector[i]->getIsLog();

                    trace(TRACE_LEVEL_INFO, string("Ext acl vals: ") + "PolicyName " + ipAclPolicy + " action " + action + " SourceIp " + extSrcIp + " DestIp " + extDstIp +  + " DestPortRange " +  dstPortNum + " to " + dstHighNum + " protocol " + proto + "tcp flags" + urg + ack + push + fin + rst + sync + "neq flags" + neqSportFlag + neqDportFlag);
					//ret = addExtRulesBackdb(ipAclPolicy, extRuleMoVector[i]->getRuleid(), action, extSrcIp, dstPortNum, dstHighNum, proto);
					ret = addExtRulesBackdbOptimized(ipAclPolicy, extRuleMoVector[i]->getRuleid(),
							action, extSrcIp, dstPortNum, dstHighNum, extDstIp,
							srcPortNum, srcHighNum, proto, PrecDscpTos, urg,
                            ack, push, fin, rst, sync, neqSportFlag, neqDportFlag);
                    if (ret) {
                        trace (TRACE_LEVEL_ERROR, "addExtRulesBackDb failed");
                        ipfTransAbort(txnHdl);
                        delete pIpAclPolicyMO;
                        if (pIpAclPolicyLocalMO) {
                            delete pIpAclPolicyLocalMO;
                        }
                        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
                        return (IP_ACL_INTERNAL_ERROR);
                    }


                }
            }


	if (pResultsPMO) {
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
	}

	/* enforceEmptyPolicyRules if there are no rules in the policy, backend will not activate empty policy */ 
	if ((((int) stdRuleMoVector.size()) == 0) && (((int) extRuleMoVector.size()) == 0))
	{
		ret = enforceEmptyPolicyRules(ipAclPolicy);
		if (ret)
		{
			ipfTransAbort(txnHdl);
			if (pIpAclPolicyLocalMO)
			{
				delete pIpAclPolicyLocalMO;
			}
			trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::enforceIpAclPolicy : enforceEmptyPolicyRules Failed");
			return (IP_ACL_INTERNAL_ERROR);
		}
	}


	// Activate the newly added policy
	ret = ipfPolicyActivate(ipAclPolicy.c_str());
	if (!ret) {
		trace(TRACE_LEVEL_INFO, "enforceIpAclPolicy....before isStoredb");
		if (isStoreDb == TRUE) {
			if (isNewLocalMO ==  false) {	
			// Remove the old non-default policy, if active/exists
				string oldPolicyName = pIpAclPolicyLocalMO->getIpAclPolicy();
				ret = ipfPolicyDelete(oldPolicyName.c_str());
				if (ret) {
					//TODO:After activating the new policy what todo if we couldn't delete the exisitng policy	
					ipfTransAbort(txnHdl);
					if (pIpAclPolicyLocalMO) {
						delete pIpAclPolicyLocalMO;
					}
					return (IP_ACL_INTERNAL_ERROR);
				}
			}

			/* Make changes to the IPAdm MO instance in the database */
			trace(TRACE_LEVEL_INFO, "Make changes to the IPAdm MO instance in the database");
			startTransaction();
			if (isNewLocalMO ==  false) {
				updateWaveManagedObject(pIpAclPolicyLocalMO);
			} else {
				trace(TRACE_LEVEL_INFO, "ITs a new  ACL");
				pIpAclPolicyLocalMO = dynamic_cast<IpAclPolicyLocalManagedObject *>(createManagedObjectInstance(IpAclPolicyLocalManagedObject::getClassName()));
				pIpAclPolicyLocalMO->setSwitchId(switchId);
			}
			pIpAclPolicyLocalMO->setIpAclPolicy(ipAclPolicy);
			pIpAclPolicyLocalMO->setIpAclTrafFlow(ACL_TRAFFIC_IN);
			pIpAclPolicyLocalMO->setIpAclType(policyType);
			trace(TRACE_LEVEL_INFO, "commitTransaction of ACL");
			status = commitTransaction();

			if (getItfIdfromSwitchId(switchId) != 0)
			{
				// Add onto the other itf
				string switchId2;
				int localId = DcmToolKit::getLocalMappedId();
				trace (TRACE_LEVEL_INFO, string ("Local id is ") + localId);
				if (getItfIdfromSwitchId(switchId) == 1) {
					switchId2 = localId + string ("/") + 2;
				} else {
					switchId2 = localId + string ("/") + 1;
				}
				trace (TRACE_LEVEL_INFO, string ("the switchId2 is ") + switchId2);
				pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyLocalManagedObject::getClassName());
				pSyncQueryCtx->addAndAttribute (new AttributeString (switchId2, "switchid"));
				vector<WaveManagedObject *> *pResultsStdby = querySynchronously (pSyncQueryCtx);
				delete pSyncQueryCtx;
				if (pResultsStdby)
				{
					startTransaction();
					if (isNewLocalMO == false) {
						if (pResultsStdby->size() == 1)
						{
							pIpAclPolicyStandbyMO = dynamic_cast<IpAclPolicyLocalManagedObject *>((*pResultsStdby)[0]);
						}
						if (pIpAclPolicyStandbyMO) {
							trace(TRACE_LEVEL_INFO, "update ACL in standby");
							updateWaveManagedObject(pIpAclPolicyStandbyMO);
						}
						else
						{
							trace(TRACE_LEVEL_INFO, "Error fetching stanby's ACL Mo");
							return (FRAMEWORK_ERROR);
						}
					} else {
						trace(TRACE_LEVEL_INFO, "ITs a new  ACL in Standby");
						pIpAclPolicyStandbyMO = dynamic_cast<IpAclPolicyLocalManagedObject *>(createManagedObjectInstance(IpAclPolicyLocalManagedObject::getClassName()));
						pIpAclPolicyStandbyMO->setSwitchId(switchId2);
					}
					pIpAclPolicyStandbyMO->setIpAclPolicy(ipAclPolicy);
					pIpAclPolicyStandbyMO->setIpAclTrafFlow(ACL_TRAFFIC_IN);
					pIpAclPolicyStandbyMO->setIpAclType(policyType);
					trace(TRACE_LEVEL_INFO, "commitTransaction of ACL");
					status = commitTransaction();
				}
			}
		}
		//TODO: Need to handle these error cases ????? 
		ipfPolicySave(NULL);
		ipfTransEnd(txnHdl);
		} else {
			ipfTransAbort(txnHdl);
			trace (TRACE_LEVEL_ERROR, string("Activating the new policy failed: ipfPolicyActivate(): ") + ret);
			status = IP_ACL_INTERNAL_ERROR;
		}
		

	if ((status == FRAMEWORK_SUCCESS) || (status == WAVE_MESSAGE_SUCCESS))
	{
		trace (TRACE_LEVEL_INFO, "Making changes for the rule in the DB is successful.");
		status = WAVE_MESSAGE_SUCCESS;
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Making changes for the rule in the database FAILED!!.");
	}

	delete pIpAclPolicyMO;
	if (pIpAclPolicyLocalMO) {
		delete pIpAclPolicyLocalMO;
	}
	if (pIpAclPolicyStandbyMO) {
		delete pIpAclPolicyStandbyMO;
	}

	trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::enforceIpAclPolicy()");
	return (status);
}
void  LocalIpAclWorker::AAALocalIpAclConfigMessageHandler( AAALocalIpAclConfigMessage *pAAALocalIpAclConfigMessage)
{
		trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::AAALocalIpAclConfigMessageHandler()");
		prismAssert(pAAALocalIpAclConfigMessage != NULL, __FILE__, __LINE__);
		int itf = 0;
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		string switchId = pAAALocalIpAclConfigMessage->getSwitchId();
		int activeItf = findActiveInterface();
		itf = getItfIdfromSwitchId(switchId);

		if (itf != activeItf)
		{
			trace (TRACE_LEVEL_INFO, "The Interface specified is that on "
							"Standby");
			status = IP_ACL_INVALID_MGMT_INTF;
		}
		if (status == WAVE_MESSAGE_SUCCESS)
		{
			string ipAclPolicy = pAAALocalIpAclConfigMessage->getPolicyName();
			if (pAAALocalIpAclConfigMessage->getAclVersion() == ACL_IPV4) {
				status = enforceIpAclPolicy(ipAclPolicy, switchId, TRUE, ACL_IPV4);
			}
			 else {
				status = enforceIp6AclPolicy(ipAclPolicy, switchId, TRUE, ACL_IPV6);
			}
		}
		pAAALocalIpAclConfigMessage->setCompletionStatus (status);
		reply (pAAALocalIpAclConfigMessage);
		trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::AAALocalIpAclConfigMessageHandler()");
}

int  LocalIpAclWorker::addStdIpAclRules (AAALocalAssocIpRuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage) 
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::addStdIpAclRules()");
	// Get the rule attributes from the message.
	string ipAclPolicy = pLocalAssocIpRuleToPolicyMessage->getPolicyName();
	UI64 seqNum = pLocalAssocIpRuleToPolicyMessage->getRuleid();
	UI32 action = pLocalAssocIpRuleToPolicyMessage->getAction();
	IpAddrType srcIpType = pLocalAssocIpRuleToPolicyMessage->getSrcType();

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	vector <LocationId> locVector;
	SI32 mappedId = -1;
	LocationId localLocationId = 0;
	
	int ret = getPolicyEnforcedSwitches(ipAclPolicy, locVector, ACL_IPV4);
	if (ret == FRAMEWORK_ERROR) {
		trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
		return(-1);
	}

	mappedId = DcmToolKit::getLocalMappedId();
	if (mappedId != (-1) ) {
		localLocationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
	}

	UI32 iter_loc;

	for (iter_loc = 0; iter_loc < locVector.size(); iter_loc++)
	{
		if (localLocationId == locVector[iter_loc])
		{
			trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIpAclRules location is present"));
			break;
		}
	}

	if (iter_loc == locVector.size())
	{
		trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIpAclRules location is not present"));
		return (status);
	}

	if (action == SSMACL_FORWARDING_RESULT_PERMIT_E)
	{
		action = IPF_ACCEPT;
			trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIpAclRules action is permit"));
	}
	else
	{
		action = IPF_DROP;
	}
			
	string srcIp;
	if (srcIpType == IP_ADDR_TYPE_ANY) {
		trace (TRACE_LEVEL_INFO, string("IP_ADDR_TYPE_ANY"));
		srcIp = "any";
	} else if (srcIpType == IP_ADDR_TYPE_HOST) {
		trace (TRACE_LEVEL_INFO, string("IP_ADDR_TYPE_HOST"));
		srcIp = (pLocalAssocIpRuleToPolicyMessage->getSrcHostIpAddress().toString());
	} else if (srcIpType == IP_ADDR_TYPE_NONE) {
		trace (TRACE_LEVEL_INFO, string("IP_ADDR_TYPE_NONE"));
		string SrcIpAddress = ((pLocalAssocIpRuleToPolicyMessage->getSrcIpAddress()).toString());
		string SrcIpAddressMask = ((pLocalAssocIpRuleToPolicyMessage->getSrcIpAddressMask()).toString());
		srcIp = SrcIpAddress.c_str();
        string cidrMask = netmaskToCidr(SrcIpAddressMask);
        if(cidrMask.length()!=0)
            srcIp += (string("/") + cidrMask);
	} else {
		trace (TRACE_LEVEL_ERROR, string("Getting Source IP from DB failed srcipType: ") + srcIpType);
		return (IP_ACL_INTERNAL_ERROR);
	}
	trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIpAclRules values are: ") + "SourceIp " + srcIp + " AclName " + ipAclPolicy + " RuleNum " + seqNum);

	trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIpAclRules() srcIpType: ") + srcIpType + " hostAddr: " + srcIp);
	ipf_txn_hdl_t txnHdl = ipfTransBegin();
	if (txnHdl <= 0) {
		return (IP_ACL_DB_LOCKED);
	}

    vector<WaveManagedObject *> *pResultsPMO = NULL;
    vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> > stdRuleMoVector;

    WaveManagedObjectSynchronousQueryContext syncQueryCtx(StdIpAclPolicyManagedObject::getClassName());
    syncQueryCtx.addAndAttribute (new AttributeString (ipAclPolicy, "policyName"));
    syncQueryCtx.setLoadCompositions(false);
    syncQueryCtx.setLoadOneToManyRelationships(false);
    syncQueryCtx.addSelectField("objectId");
    pResultsPMO = querySynchronously (&syncQueryCtx);
    if (!pResultsPMO)
    {
        return (FRAMEWORK_ERROR);
    }
    StdIpAclPolicyManagedObject *stdPolicyMO = NULL;
    stdPolicyMO = dynamic_cast<StdIpAclPolicyManagedObject *>((*pResultsPMO)[0]);
    ObjectId objectId = stdPolicyMO->getObjectId();
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);

    UI32 numrules = 0;
    WaveManagedObjectSynchronousQueryContext syncQueryCtx2(StdIpAclRuleManagedObject::getClassName());
    syncQueryCtx2.addAndAttribute (new AttributeObjectId (objectId, "ownerManagedObjectId"));
    syncQueryCtx2.setLoadCompositions(false);
    syncQueryCtx2.setLoadOneToManyRelationships(false);
    querySynchronouslyForCount(&syncQueryCtx2, numrules);

	ipfExtnPolicy_t ptable;
	ret = ipfPolicyGet(ipAclPolicy.c_str(), &ptable);
	if (ret)
	{
		trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::addStdIpAclRules - ipfPolicyGet failed");
		ipfTransAbort(txnHdl);
		return (IP_ACL_INTERNAL_ERROR);
	}
	/* if the policy was empty, then remove previous 2 rules */
	if (((numrules == 0) || (numrules == 1)) && ((ptable.numRules == 2)))
	{
		ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), 2);
		if (ret) {
			trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy: ipfPolicyRuleDelete(): ") + ret);
			ipfTransAbort(txnHdl);
				return (IP_ACL_INTERNAL_ERROR);
		}
		ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), 1);
		if (ret) {
			trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy: ipfPolicyRuleDelete(): ") + ret);
			ipfTransAbort(txnHdl);
			return (IP_ACL_INTERNAL_ERROR);
		}
	}

	ret = addStdRulesBackdb(ipAclPolicy, seqNum, action, srcIp);
	if (ret == 0) {
		ret = ipfPolicyActivate(ipAclPolicy.c_str());
		if (ret == 0) {
			//TODO: Need to handle these error cases ????? 
			ipfPolicySave(NULL);
			ipfTransEnd(txnHdl);
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" is added successfully to the std policy: ipfPolicyActivate()") + ret);
		} else {
			ipfTransAbort(txnHdl);
			status = IP_ACL_INTERNAL_ERROR;
			trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added to std the policy: ipfPolicyActivate()") + ret);
		}
	} else {
		ipfTransAbort(txnHdl);
		status = IP_ACL_INTERNAL_ERROR;
		trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added successfully to the std policy: addStdRulesBackdb()") + ret);
		/* If the first operation fails, transaction aborts automatically. */
	}

	trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::addStdIpAclRules()");
	return (status);
}

#if 0
int  LocalIpAclWorker::changeStdIpAclRules( AAALocalStdIpAclRuleMessage *pLocalStdIpAclRuleMessage) 
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::changeStdIpAclRules()");
	string ipAclPolicy = pLocalStdIpAclRuleMessage->getPolicyName();
	UI64 seqNum = pLocalStdIpAclRuleMessage->getSeqNum();
	IpAclAction actionEnum = pLocalStdIpAclRuleMessage->getAction();
	IpAddrType srcIpType = pLocalStdIpAclRuleMessage->getSrcIpType();
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	int ret, action;

	if (actionEnum == ACL_PERMIT)
		action = IPF_ACCEPT;
	else
		action = IPF_DROP;
				
	string srcIp;
	if (srcIpType == IPV4_ADDR_ANY) {
		srcIp = "any";
	} else if (srcIpType == IPV4_ADDR_HOST) {
		srcIp = pLocalStdIpAclRuleMessage->getSrcHost();
	} else {
		trace (TRACE_LEVEL_ERROR, string("Getting Source IP from DB failed srcipType: ") + srcIpType);
		return (IP_ACL_INTERNAL_ERROR);
	}
	trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::changeStdIpAclRules() srcIpType: ") + srcIpType + "hostAddr: " + srcIp);

	ipf_txn_hdl_t txnHdl = ipfTransBegin();
	if (txnHdl <= 0) {
		return (IP_ACL_DB_LOCKED);
	}
	/* Change operation is 'delete' and 'add'. */
	ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), STD_ACL_INDEX(seqNum) + 1);
	if (ret == 0) {
		ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), STD_ACL_INDEX(seqNum));
		if (ret) {
			ipfTransAbort(txnHdl);
			status = IP_ACL_INTERNAL_ERROR;
			trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy ipfPolicyRuleDelete()") + ret);
		} 
	} else {
		ipfTransAbort(txnHdl);
		status = IP_ACL_INTERNAL_ERROR;
		trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy ipfPolicyRuleDelete()") + ret);
	}

	if (ret == 0) 
	{
		status = WAVE_MESSAGE_SUCCESS;
		// if the first rule needs to be added in the policy, need to remove already existing 
		// rules in the back end db. These rules only exist in the backend. 
		ret = addStdRulesBackdb(ipAclPolicy, seqNum, action, srcIp);
		if (ret == 0) {
			ret = ipfPolicyActivate(ipAclPolicy.c_str());
			if (ret == 0) {
				//TODO: Need to handle these error cases ????? 
				ipfPolicySave(NULL);
				ipfTransEnd(txnHdl);
				status = WAVE_MESSAGE_SUCCESS;
				trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" is added successfully to the std policy  ipfPolicyActivate ()") + ipAclPolicy);
			} else {
				ipfTransAbort(txnHdl);
				status = IP_ACL_INTERNAL_ERROR;
				trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added to std the policy: ipfPolicyActivate()) ") + ret);
			}
		} else {
			ipfTransAbort(txnHdl);
			status = IP_ACL_INTERNAL_ERROR;
			trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added successfully to the std policy: addStdRulesBackdb(): ") + ret);
			/* If the first operation fails, transaction aborts automatically. */
		}
	} else {
		ipfTransAbort(txnHdl);
		status = IP_ACL_INTERNAL_ERROR;
		trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the policy ipfPolicyRuleDelete() ") + ret);
	}

	trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::changeStdIpAclRules()");
	return (status);
}
#endif

int  LocalIpAclWorker::deleteStdIpAclRules(AAALocalAssocIpRuleToPolicyMessage *pLocalStdIpAclRuleMessage) 
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::deleteStdIpAclRules()");
	// Get the rule attributes from the message.
	string ipAclPolicy = pLocalStdIpAclRuleMessage->getPolicyName();
	UI64 seqNum = pLocalStdIpAclRuleMessage->getRuleid();
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	vector <LocationId> locVector;
	SI32 mappedId = -1;
	LocationId localLocationId = 0;


	int ret = getPolicyEnforcedSwitches(ipAclPolicy, locVector, ACL_IPV4);
	if (ret == FRAMEWORK_ERROR) {
		 trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
		return(-1);
	}

	mappedId = DcmToolKit::getLocalMappedId();
	if (mappedId != (-1) ) {
		localLocationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
	}

	UI32 iter_loc;

	for (iter_loc = 0; iter_loc < locVector.size(); iter_loc++)
	{
		if (localLocationId == locVector[iter_loc])
		{
			trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIpAclRules location is present"));
			break;
		}
	}

	if (iter_loc == locVector.size())
	{
		return (status);
	}

	ipf_txn_hdl_t txnHdl = ipfTransBegin();
	if (txnHdl <= 0) {
		return (IP_ACL_DB_LOCKED);
	}

	ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), STD_ACL_INDEX(seqNum) + 1);
	if (ret == 0) {
		ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), STD_ACL_INDEX(seqNum));
	}
	if (ret == 0) {
		ipfExtnPolicy_t ptable;

		ret = ipfPolicyGet(ipAclPolicy.c_str(), &ptable);
		if (ret)
		{
			trace (TRACE_LEVEL_INFO, "ipfPolicyGet failed");
			ipfTransAbort(txnHdl);
			status = IP_ACL_INTERNAL_ERROR;
		}
		else if (ptable.numRules == 0)
		{
			/* Default Rules with seqnum 1 and 2 were added in the backend only incase of empty policy. */
			ret = enforceEmptyPolicyRules(ipAclPolicy);
			if (ret) {
				trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy: enforceEmptyPolicyRules() ") + ret);
				status = IP_ACL_INTERNAL_ERROR;
				ipfTransAbort(txnHdl);
			}
		}
		if (ret == 0) {	
			ret = ipfPolicyActivate(ipAclPolicy.c_str());
			if (ret) {
				status = IP_ACL_INTERNAL_ERROR;
				trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be activated successfully from the std policy ipfPolicyActivate() ") + ret);
				ipfTransAbort(txnHdl);
			} else {
				//TODO: Need to handle these error cases ????? 
				ipfPolicySave(NULL);
				ipfTransEnd(txnHdl);
			}
		}
	} else {
		ipfTransAbort(txnHdl);
		status = IP_ACL_INTERNAL_ERROR;
		trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully to the std policy ipfPolicyRuleDelete() ") + ret);
	}
	trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::deleteStdIpAclRules()");
	/* SSM is always returned a pass value, so status not used as of now */
	return (WAVE_MESSAGE_SUCCESS);
}

void  LocalIpAclWorker::AAALocalAssocIpRuleToPolicyMessageHandler (AAALocalAssocIpRuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage)
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::AAALocalAssocIpAclRuleMessageHandler()");
	prismAssert(pLocalAssocIpRuleToPolicyMessage != NULL, __FILE__, __LINE__);
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	UI32 opcode	=	pLocalAssocIpRuleToPolicyMessage->getOpcode();
	UI32 policyType = pLocalAssocIpRuleToPolicyMessage->getPolicyType();

	if (opcode == SSMACL_DCMD_ADD_RULE) {
		if (policyType == SSMACL_TYPE_L3_IPV4_STD_E)
		{
			status = addStdIpAclRules(pLocalAssocIpRuleToPolicyMessage);
		}
		else if (policyType == SSMACL_TYPE_L3_IPV4_EXT_E)
		{
			trace (TRACE_LEVEL_INFO, "AAALocalAssocIpRuleToPolicyMessageHandler: calling addExtIpAclRules");
			status = addExtIpAclRules(pLocalAssocIpRuleToPolicyMessage);
		}
		if (status != WAVE_MESSAGE_SUCCESS) {
			status = IP_ACL_RULE_ADD_ERROR;
		}
	} else if (opcode == SSMACL_DCMD_DEL_RULE) {
        if (policyType == SSMACL_TYPE_L3_IPV4_STD_E)
		{
            status = deleteStdIpAclRules(pLocalAssocIpRuleToPolicyMessage);
        }
		else if (policyType == SSMACL_TYPE_L3_IPV4_EXT_E)
		{
            status = deleteExtIpAclRules(pLocalAssocIpRuleToPolicyMessage);
        }
		if (status != WAVE_MESSAGE_SUCCESS) {
			status = IP_ACL_RULE_DEL_ERROR;
		}
	}
	/* As per discussion with SSM team, AAA will rely on raslog message generated by secd, so SUCCESS */
/*
	status = WAVE_MESSAGE_SUCCESS;
	pLocalAssocIpRuleToPolicyMessage->setCompletionStatus(status);
	reply (pLocalAssocIpRuleToPolicyMessage);
*/
	if (pLocalAssocIpRuleToPolicyMessage)
	{
		delete pLocalAssocIpRuleToPolicyMessage;
	}

	trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::AAALocalAssocIpRuleToPolicyMessageHandler");
}

void LocalIpAclWorker::AAAIpAclLocalShowMessageHandler (AAAIpAclLocalShowMessage *pAAAIpAclLocalShowMessage)
{
	trace (TRACE_LEVEL_DEBUG, "Enter LocalIpAclWorker::AAAIpAclLocalShowMessageHandler()");
	trace (TRACE_LEVEL_DEBUG, string("policyName:")+pAAAIpAclLocalShowMessage->getPolicyName()+" ifName:"+pAAAIpAclLocalShowMessage->getIfName() +" opCode:"+ pAAAIpAclLocalShowMessage->getOpCode() + " isGatewayReq:"+pAAAIpAclLocalShowMessage->getIsGwRequest() + " aclType:"+ pAAAIpAclLocalShowMessage->getAclType() + " ifType:" +pAAAIpAclLocalShowMessage->getIfType() + " direction:" + pAAAIpAclLocalShowMessage->getDirection());
	string ipAclPolicy = pAAAIpAclLocalShowMessage->getPolicyName();
	UI32 opCode = pAAAIpAclLocalShowMessage->getOpCode();
	string ifName = pAAAIpAclLocalShowMessage->getIfName();
	UI32 aclType = pAAAIpAclLocalShowMessage->getAclType();

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	IpAclPolicyLocalManagedObject *pIpAclPolicyLocalMO = NULL;
	Ip6AclPolicyLocalManagedObject *pIp6AclPolicyLocalMO = NULL;

	ResourceId status = IP_ACL_INTERNAL_ERROR;

	LocationId localLocationId = 0;
	SI32 mappedId = DcmToolKit::getLocalMappedId();
	if (mappedId != (-1) ) {
		localLocationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
	}
	trace (TRACE_LEVEL_DEBUG, string("LocalIpAclWorker::AAAIpAclLocalShowMessageHandler localLocationId:") + localLocationId);

	if(aclType == SSMACL_TYPE_L3_IPV6_E)
	{
		trace(TRACE_LEVEL_DEBUG, "LocalIpAclWorker::AAAIpAclLocalShowMessageHandler IPv6 Call..");
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyLocalManagedObject::getClassName());
		if (opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ACL)
			pSyncQueryCtx->addAndAttribute (new AttributeString(ipAclPolicy, "ip6aclpolicy"));
		else if (opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ACL_ON_INTERFACE)
		{
			pSyncQueryCtx->addAndAttribute (new AttributeString(ipAclPolicy, "ip6aclpolicy"));
			pSyncQueryCtx->addAndAttribute (new AttributeString(ifName, "switchid"));
		}
		pResults = querySynchronously (pSyncQueryCtx);

		if (!pResults)
		{
			trace (TRACE_LEVEL_FATAL, "Ip/Ip6AclPolicyLocalManagedObject database failed");
			status = IP_ACL_INTERNAL_ERROR;
			pAAAIpAclLocalShowMessage->setCompletionStatus(status);
			reply (pAAAIpAclLocalShowMessage);
			delete pSyncQueryCtx;
			return ;
		}
		else if (pResults->size() == 0)
		{
			trace (TRACE_LEVEL_DEBUG, string ("None of the policies are enforced on this switchid:") + ifName);
			status = SSM_DCM_ERR_ACL_NOT_APPLIED;
			pAAAIpAclLocalShowMessage->setCompletionStatus(status);
			reply (pAAAIpAclLocalShowMessage);
			delete pSyncQueryCtx;
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			return ;
		}
		trace(TRACE_LEVEL_DEBUG, string("querySynchronously pResults->size() == ")+ pResults->size());

		if (opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ACL)
		{
			AAAIpAclLocalShowMessage *msg_p;
			msg_p = new AAAIpAclLocalShowMessage(pAAAIpAclLocalShowMessage->getPolicyName(),pAAAIpAclLocalShowMessage->getIfName(),pAAAIpAclLocalShowMessage->getOpCode(),pAAAIpAclLocalShowMessage->getIsGwRequest(),pAAAIpAclLocalShowMessage->getAclType(),pAAAIpAclLocalShowMessage->getIfType(),pAAAIpAclLocalShowMessage->getDirection());
			for (UI32 j = 0; j < pResults->size(); j++)
			{
				pIp6AclPolicyLocalMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>((*pResults)[j]);
				if (pIp6AclPolicyLocalMO) {
					trace (TRACE_LEVEL_DEBUG, string ("SSMACL_DCMD_SHOW_ACCESSLIST_ACL on switchid:") + pIp6AclPolicyLocalMO->getSwitchId() + " policyName:" + pIp6AclPolicyLocalMO->getIpv6AclPolicy());
					msg_p->setPolicyName(pIp6AclPolicyLocalMO->getIpv6AclPolicy());
					msg_p->setAclType(pIp6AclPolicyLocalMO->getIpv6AclType());
					msg_p->setIfName(pIp6AclPolicyLocalMO->getSwitchId());
//					delete pIp6AclPolicyLocalMO;
				}
				status = showIpAclRules(msg_p,j);
			}
			trace (TRACE_LEVEL_DEBUG, string (" LocalIpAclWorker::AAAIpAclLocalShowMessageHandler() num of buffer=") + msg_p->getNumberOfBuffers());
			pAAAIpAclLocalShowMessage->copyAllBuffers(*msg_p);
			delete msg_p;
		}
		else if (opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ACL_ON_INTERFACE)
		{
			pIp6AclPolicyLocalMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>((*pResults)[0]);
			if (pIp6AclPolicyLocalMO) {
				trace (TRACE_LEVEL_DEBUG, string ("SSMACL_DCMD_SHOW_ACCESSLIST_ACL_ON_INTERFACE on switchid:") + pIp6AclPolicyLocalMO->getSwitchId() + " policyName:" + ipAclPolicy + " ifName:" + ifName);
				pAAAIpAclLocalShowMessage->setAclType(pIp6AclPolicyLocalMO->getIpv6AclType());
			}
			mappedId = atoi(ifName.c_str());
			if(localLocationId == DcmToolKit::getLocationIdFromMappedId(mappedId))
				status = showIpAclRules(pAAAIpAclLocalShowMessage,0);
		}
		else if (opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ALL)
		{
			AAAIpAclLocalShowMessage *msg_p;
			msg_p = new AAAIpAclLocalShowMessage(pAAAIpAclLocalShowMessage->getPolicyName(),pAAAIpAclLocalShowMessage->getIfName(),pAAAIpAclLocalShowMessage->getOpCode(),pAAAIpAclLocalShowMessage->getIsGwRequest(),pAAAIpAclLocalShowMessage->getAclType(),pAAAIpAclLocalShowMessage->getIfType(),pAAAIpAclLocalShowMessage->getDirection());
			for (UI32 j = 0; j < pResults->size(); j++)
			{
				pIp6AclPolicyLocalMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>((*pResults)[j]);
				if (pIp6AclPolicyLocalMO) {
					trace (TRACE_LEVEL_DEBUG, string ("SSMACL_DCMD_SHOW_ACCESSLIST_ALL on switchid:") + pIp6AclPolicyLocalMO->getSwitchId() + " policyName:" + pIp6AclPolicyLocalMO->getIpv6AclPolicy());
					msg_p->setPolicyName(pIp6AclPolicyLocalMO->getIpv6AclPolicy());
					msg_p->setAclType(pIp6AclPolicyLocalMO->getIpv6AclType());
					msg_p->setIfName(pIp6AclPolicyLocalMO->getSwitchId());
				}
				status = showIpAclRules(msg_p,j);
			}
			trace (TRACE_LEVEL_DEBUG, string (" LocalIpAclWorker::AAAIpAclLocalShowMessageHandler() num of buffer=") + msg_p->getNumberOfBuffers());
			pAAAIpAclLocalShowMessage->copyAllBuffers(*msg_p);
			delete msg_p;
		}


		delete pSyncQueryCtx;
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		pAAAIpAclLocalShowMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		reply (pAAAIpAclLocalShowMessage);
		return;
	}


	if (opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ON_INTERFACE)
    {
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyLocalManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeString(ifName, "switchid"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults) {
            trace (TRACE_LEVEL_FATAL, "Ip/Ip6AclPolicyLocalManagedObject database failed");
            pAAAIpAclLocalShowMessage->setCompletionStatus(IP_ACL_INTERNAL_ERROR);
            reply (pAAAIpAclLocalShowMessage);
            delete pSyncQueryCtx;
            return ;
        }
        else if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_DEBUG, string ("None of the ip policies are enforced on this switchid:") + ifName);
			status = SSM_DCM_ERR_ACL_NOT_APPLIED;
            delete pSyncQueryCtx;
        }
		else {
			trace(TRACE_LEVEL_DEBUG, string("querySynchronously pResults->size() == ")+ pResults->size());
	        pIpAclPolicyLocalMO = dynamic_cast<IpAclPolicyLocalManagedObject *>((*pResults)[0]);
    	    if (pIpAclPolicyLocalMO) {
        	    ipAclPolicy = pIpAclPolicyLocalMO->getIpAclPolicy();
            	pAAAIpAclLocalShowMessage->setAclType(pIpAclPolicyLocalMO->getIpAclType());
	            trace (TRACE_LEVEL_DEBUG, string ("SSMACL_DCMD_SHOW_ACCESSLIST_ON_INTERFACE on switchid:") + pIpAclPolicyLocalMO->getSwitchId() + " policyName:" + ipAclPolicy);
    	        delete pIpAclPolicyLocalMO;
        	}
	        pAAAIpAclLocalShowMessage->setPolicyName(ipAclPolicy);
    	    mappedId = atoi(ifName.c_str());
        	if(localLocationId == DcmToolKit::getLocationIdFromMappedId(mappedId))
            	status = showIpAclRules(pAAAIpAclLocalShowMessage,0);
			delete pSyncQueryCtx;
		}

//        delete pSyncQueryCtx;
        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyLocalManagedObject::getClassName());
        pSyncQueryCtx->addAndAttribute (new AttributeString(ifName, "switchid"));
        pResults = querySynchronously (pSyncQueryCtx);
        if (!pResults) {
            trace (TRACE_LEVEL_FATAL, "Ip/Ip6AclPolicyLocalManagedObject database failed");
            pAAAIpAclLocalShowMessage->setCompletionStatus(IP_ACL_INTERNAL_ERROR);
            reply (pAAAIpAclLocalShowMessage);
            delete pSyncQueryCtx;
            return ;
        }
        else if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_DEBUG, string ("None of the ipv6 policies are enforced on this switchid:") + ifName);
			status = SSM_DCM_ERR_ACL_NOT_APPLIED;
            delete pSyncQueryCtx;
        }
        else {
			trace(TRACE_LEVEL_DEBUG, string("querySynchronously pResults->size() == ")+ pResults->size());
            pIp6AclPolicyLocalMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>((*pResults)[0]);
            if (pIp6AclPolicyLocalMO) {
                ipAclPolicy = pIp6AclPolicyLocalMO->getIpv6AclPolicy();
                pAAAIpAclLocalShowMessage->setAclType(pIp6AclPolicyLocalMO->getIpv6AclType());
                trace (TRACE_LEVEL_DEBUG, string ("SSMACL_DCMD_SHOW_ACCESSLIST_ON_INTERFACE on switchid:") + pIp6AclPolicyLocalMO->getSwitchId() + " policyName:" + ipAclPolicy);
                delete pIp6AclPolicyLocalMO;
            }
            pAAAIpAclLocalShowMessage->setPolicyName(ipAclPolicy);
            mappedId = atoi(ifName.c_str());
            if(localLocationId == DcmToolKit::getLocationIdFromMappedId(mappedId))
                status = showIpAclRules(pAAAIpAclLocalShowMessage,1);
			delete pSyncQueryCtx;
        }
    }
	else
	{
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyLocalManagedObject::getClassName());
		if(opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ON_INTERFACE)
			pSyncQueryCtx->addAndAttribute (new AttributeString(ifName, "switchid"));
		else if (opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ACL)
			pSyncQueryCtx->addAndAttribute (new AttributeString(ipAclPolicy, "ipaclpolicy"));
		else if (opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ACL_ON_INTERFACE)
		{
			pSyncQueryCtx->addAndAttribute (new AttributeString(ipAclPolicy, "ipaclpolicy"));
			pSyncQueryCtx->addAndAttribute (new AttributeString(ifName, "switchid"));
		}
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults)
		{
			trace (TRACE_LEVEL_FATAL, "Ip/Ip6AclPolicyLocalManagedObject database failed");
			status = IP_ACL_INTERNAL_ERROR;
			pAAAIpAclLocalShowMessage->setCompletionStatus(status);
			reply (pAAAIpAclLocalShowMessage);
			delete pSyncQueryCtx;
			return ;
		}
		else if (pResults->size() == 0)
		{
			trace (TRACE_LEVEL_DEBUG, string ("None of the policies are enforced on this switchid:") + ifName);
			status = SSM_DCM_ERR_IP_ACL_EEXIST;
			pAAAIpAclLocalShowMessage->setCompletionStatus(status);
			reply (pAAAIpAclLocalShowMessage);
			delete pSyncQueryCtx;
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			return ;
		}
		trace(TRACE_LEVEL_DEBUG, string("querySynchronously pResults->size() == ")+ pResults->size());
	}

	if (opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ACL)
	{
		AAAIpAclLocalShowMessage *msg_p;
		msg_p = new AAAIpAclLocalShowMessage(pAAAIpAclLocalShowMessage->getPolicyName(),pAAAIpAclLocalShowMessage->getIfName(),pAAAIpAclLocalShowMessage->getOpCode(),pAAAIpAclLocalShowMessage->getIsGwRequest(),pAAAIpAclLocalShowMessage->getAclType(),pAAAIpAclLocalShowMessage->getIfType(),pAAAIpAclLocalShowMessage->getDirection());
		for (UI32 j = 0; j < pResults->size(); j++)
		{
			pIpAclPolicyLocalMO = dynamic_cast<IpAclPolicyLocalManagedObject *>((*pResults)[j]);
			if (pIpAclPolicyLocalMO) {
				trace (TRACE_LEVEL_DEBUG, string ("SSMACL_DCMD_SHOW_ACCESSLIST_ACL on switchid:") + pIpAclPolicyLocalMO->getSwitchId() + " policyName:" + pIpAclPolicyLocalMO->getIpAclPolicy());
				msg_p->setPolicyName(pIpAclPolicyLocalMO->getIpAclPolicy());
				msg_p->setAclType(pIpAclPolicyLocalMO->getIpAclType());
				msg_p->setIfName(pIpAclPolicyLocalMO->getSwitchId());
				delete pIpAclPolicyLocalMO;
			}
			status = showIpAclRules(msg_p,j);
		}
		trace (TRACE_LEVEL_DEBUG, string (" LocalIpAclWorker::AAAIpAclLocalShowMessageHandler() num of buffer=") + msg_p->getNumberOfBuffers());
		pAAAIpAclLocalShowMessage->copyAllBuffers(*msg_p);
		delete msg_p;
	}
	else if (opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ACL_ON_INTERFACE)
	{
		pIpAclPolicyLocalMO = dynamic_cast<IpAclPolicyLocalManagedObject *>((*pResults)[0]);
		if (pIpAclPolicyLocalMO)
		{
			trace (TRACE_LEVEL_DEBUG, string ("SSMACL_DCMD_SHOW_ACCESSLIST_ACL_ON_INTERFACE on switchid:") + pIpAclPolicyLocalMO->getSwitchId() + " policyName:" + ipAclPolicy + " ifName:" + ifName);
			pAAAIpAclLocalShowMessage->setAclType(pIpAclPolicyLocalMO->getIpAclType());
			delete pIpAclPolicyLocalMO;
		}
		mappedId = atoi(ifName.c_str());
		if(localLocationId == DcmToolKit::getLocationIdFromMappedId(mappedId))
			status = showIpAclRules(pAAAIpAclLocalShowMessage,0);
	}
	else if (opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ALL)
	{
		AAAIpAclLocalShowMessage *msg_p;
		msg_p = new AAAIpAclLocalShowMessage(pAAAIpAclLocalShowMessage->getPolicyName(),pAAAIpAclLocalShowMessage->getIfName(),pAAAIpAclLocalShowMessage->getOpCode(),pAAAIpAclLocalShowMessage->getIsGwRequest(),pAAAIpAclLocalShowMessage->getAclType(),pAAAIpAclLocalShowMessage->getIfType(),pAAAIpAclLocalShowMessage->getDirection());
		for (UI32 j = 0; j < pResults->size(); j++)
		{
			pIpAclPolicyLocalMO = dynamic_cast<IpAclPolicyLocalManagedObject *>((*pResults)[j]);
			if (pIpAclPolicyLocalMO) {
				trace (TRACE_LEVEL_DEBUG, string ("SSMACL_DCMD_SHOW_ACCESSLIST_ALL on switchid:") + pIpAclPolicyLocalMO->getSwitchId() + " policyName:" + pIpAclPolicyLocalMO->getIpAclPolicy());
				msg_p->setPolicyName(pIpAclPolicyLocalMO->getIpAclPolicy());
				msg_p->setAclType(pIpAclPolicyLocalMO->getIpAclType());
				msg_p->setIfName(pIpAclPolicyLocalMO->getSwitchId());
//				msg_p->setAclType();
				delete pIpAclPolicyLocalMO;
			}
			status = showIpAclRules(msg_p,j);
		}
		trace (TRACE_LEVEL_DEBUG, string (" LocalIpAclWorker::AAAIpAclLocalShowMessageHandler() num of buffer=") + msg_p->getNumberOfBuffers());
		pAAAIpAclLocalShowMessage->copyAllBuffers(*msg_p);
		delete msg_p;

	}

	pAAAIpAclLocalShowMessage->setCompletionStatus(status);
	reply (pAAAIpAclLocalShowMessage);
	trace (TRACE_LEVEL_DEBUG, "Exit LocalIpAclWorker::AAAIpAclLocalShowMessageHandler()");

}

int LocalIpAclWorker::showIpAclRules(AAAIpAclLocalShowMessage *pAAAIpAclLocalShowMessage,UI32 tag)
{
	trace (TRACE_LEVEL_DEBUG, "Enter LocalIpAclWorker::showIpAclRules()");
	string ipAclPolicy = pAAAIpAclLocalShowMessage->getPolicyName();

	struct show_acl_data
	{
		char switchid[10];
		UI32 ipAclType;
		ipfExtnPolicy_t ptable;
		ipfExtnRule_t prules[IPF_MAX_NUM_RULES];
	};
	struct show_acl_data data;

	data.ipAclType = pAAAIpAclLocalShowMessage->getAclType();

	int ret;

	if (data.ipAclType==SSMACL_TYPE_L3_IPV4_STD_E || data.ipAclType==SSMACL_TYPE_L3_IPV4_EXT_E)
	{
		ret = ipfActivePolicyGet(IPF_IPV4, &data.ptable);
	}	
	else
	{
		ret = ipfActivePolicyGet(IPF_IPV6, &data.ptable);
	}

	if (ret)
	{
		trace (TRACE_LEVEL_ERROR, "Couldn't get the active policy from the switch");
		return (IP_ACL_INTERNAL_ERROR);
	}

	if(strcmp(data.ptable.name,ipAclPolicy.c_str())!=0)
	{
		trace (TRACE_LEVEL_ERROR, "Policy name doesn't match with active policy from the switch");
		return (IP_ACL_INTERNAL_ERROR);
	}

	int numRules = data.ptable.numRules;
	ret = ipfPolicyRuleGet(ipAclPolicy.c_str(), data.prules, &numRules);
	if (ret)
	{
		trace (TRACE_LEVEL_DEBUG, "LocalIpAclWorker::showIpAclRules: ipfPolicyRuleGet failed" + numRules);
		return (IP_ACL_INTERNAL_ERROR);
	}

	data.ipAclType = pAAAIpAclLocalShowMessage->getAclType();
	strncpy(data.switchid,pAAAIpAclLocalShowMessage->getIfName().c_str(),10); 

	pAAAIpAclLocalShowMessage->addBuffer(tag,sizeof(data),&data,false);

	trace (TRACE_LEVEL_DEBUG, "LocalIpAclWorker::showIpAclRules");
	return (WAVE_MESSAGE_SUCCESS);

}

void LocalIpAclWorker::AAALocalIpAclReseqMessageHandler (AAALocalIpAclReseqMessage *pLocalIpAclReseqMessage)
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::AAALocalIpAclReseqMessageHandler()");
	string ipAclPolicy = pLocalIpAclReseqMessage->getPolicyName();
	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResultsPMO = NULL;

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	UI32	policyType = SSMACL_TYPE_UNKNOWN_E;
	//ObjectId	policyObjId = ObjectId::NullObjectId;


	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIpAclPolicyManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (ipAclPolicy, "policyName"));
	pResultsPMO = querySynchronously (pSyncQueryCtx);
	if (!pResultsPMO)
	{
		delete pSyncQueryCtx;
	}
	else if (pResultsPMO->size () == 1)
	{
		policyType = SSMACL_TYPE_L3_IPV4_STD_E;
	}
	else
	{
		if (pResultsPMO)
		{
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
		}
		delete pSyncQueryCtx;
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIpAclPolicyManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeString (ipAclPolicy, "policyName"));
		pResultsPMO = querySynchronously (pSyncQueryCtx);
		if (!pResultsPMO)
		{
			delete pSyncQueryCtx;
		}
		else if (pResultsPMO->size () == 1)
		{
			policyType = SSMACL_TYPE_L3_IPV4_EXT_E;
		}
	}
	
	if (policyType == SSMACL_TYPE_L3_IPV4_STD_E)
	{
		status = resequenceStdIpAclRules(pLocalIpAclReseqMessage);
	}
	else if (policyType == SSMACL_TYPE_L3_IPV4_EXT_E)
	{
		status = resequenceExtIpAclRules(pLocalIpAclReseqMessage);
	}

	if (pResultsPMO)
	{
		delete pSyncQueryCtx;
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
	}
	/* As per discussion with SSM team, AAA will rely on raslog message generated by secd, so SUCCESS */

	if (WAVE_MESSAGE_SUCCESS != status)
	{
	    // For now nothing to handle;
	    status = WAVE_MESSAGE_SUCCESS;
	}

/*
	pLocalIpAclReseqMessage->setCompletionStatus(status);
	reply (pLocalIpAclReseqMessage);
*/
	if (pLocalIpAclReseqMessage)
	{
		delete pLocalIpAclReseqMessage;
	}

	trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::AAALocalIpAclReseqMessageHandler()");
}

int  LocalIpAclWorker::resequenceStdIpAclRules(AAALocalIpAclReseqMessage *pLocalIpAclReseqMessage)
{
	string ipAclPolicy = pLocalIpAclReseqMessage->getPolicyName();
	UI64 seqId	= pLocalIpAclReseqMessage->getSeqId();
	ipfExtnPolicy_t ptable;
	ipfExtnRule_t prules[IPF_MAX_NUM_RULES];
	int seqNum	= -1, iter_rule = -1;
	int ret;

    ptable.numRules = 0;
	ret = ipfPolicyGet(ipAclPolicy.c_str(), &ptable);
	if (ret)
	{
		trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::resequenceIpAclRules : ipfPolicyGet failed");
		return (IP_ACL_INTERNAL_ERROR);
	}

	int numRules = ptable.numRules;
	ret = ipfPolicyRuleGet(ipAclPolicy.c_str(), prules, &numRules);
	if (ret)
	{
		trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::resequenceIpAclRules : ipfPolicyRuleGet failed" + numRules);
		return (IP_ACL_INTERNAL_ERROR);
	}

	ipf_txn_hdl_t txnHdl = ipfTransBegin();
	if (txnHdl <= 0) {
		trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::resequenceIpAclRules : ipfTransBegin failed");
		return (IP_ACL_DB_LOCKED);
	}

	/* first delete the rules */
	for (iter_rule = 0; iter_rule < ptable.numRules; iter_rule++)
	{
		seqNum = prules[iter_rule].rule_num;
		ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), seqNum);
		if (ret) {
			ipfTransAbort(txnHdl);
			trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy ipfPolicyRuleDelete()") + ret);
			return(IP_ACL_INTERNAL_ERROR);
		}
	}

	if (ptable.numRules)
	{
	/* add the first rule at the seqId location specified by the user */
		ret = addStdRulesBackdb(ipAclPolicy, seqId, prules[0].action, prules[0].saddrPref);
		if (ret)
		{
			ipfTransAbort(txnHdl);
			trace (TRACE_LEVEL_INFO, string("Rule ") + seqId + string(" couldn't be added successfully to the std policy: addStdRulesBackdb(): ") + ret);
			return(IP_ACL_INTERNAL_ERROR);
		}
	}

	UI64 incseqId = pLocalIpAclReseqMessage->getIncSeqId();
	seqNum = seqId;
	/* now add the resequenced rules */
	for (iter_rule = 2; iter_rule < ptable.numRules; )
	{
//		seqNum = ((prules[iter_rule].rule_num) / 2);
//		seqNum += (seqId + incseqId);
		seqNum += incseqId;
		/* for every rule entry addStdRulesBackdb will add two rules for TCP and UDP as well */
		ret = addStdRulesBackdb(ipAclPolicy, seqNum, prules[iter_rule].action, prules[iter_rule].saddrPref);

		if (ret)
		{
			ipfTransAbort(txnHdl);
			trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added successfully to the std policy: addStdRulesBackdb(): ") + ret);
			return(IP_ACL_INTERNAL_ERROR);
		}		
		/* addStdRulesBackdb has added the two rules */
		iter_rule += 2;
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	ret = ipfPolicyActivate(ipAclPolicy.c_str());
	if (ret == 0)
	{
		ipfPolicySave(NULL);
		ipfTransEnd(txnHdl);
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" is added successfully to the std policy  ipfPolicyActivate ()") + ipAclPolicy);
	} else {
		ipfTransAbort(txnHdl);
		status = IP_ACL_INTERNAL_ERROR;
		trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added to std the policy: ipfPolicyActivate()) ") + ret);
	}
	trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::resequenceIpAclRules");
	return (status);

}

int  LocalIpAclWorker::addExtIpAclRules(AAALocalAssocIpRuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage) 
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::addExtIpAclRules()");
	string ipAclPolicy = pLocalAssocIpRuleToPolicyMessage->getPolicyName();
	UI64 seqNum = pLocalAssocIpRuleToPolicyMessage->getRuleid();
    UI32 action = pLocalAssocIpRuleToPolicyMessage->getAction();
    IpAddrType srcIpType = pLocalAssocIpRuleToPolicyMessage->getSrcType();
	IpAddrType destIpType = pLocalAssocIpRuleToPolicyMessage->getDstType();
	//AclPortType destPortType = pLocalExtIpAclRuleMessage->getDestPortType();
	//UI16 destPort = pLocalExtIpAclRuleMessage->getDestPort();
	//UI16 destEndPort = pLocalExtIpAclRuleMessage->getDestEndPort();

	ResourceId status = WAVE_MESSAGE_SUCCESS;

    vector <LocationId> locVector;
    SI32 mappedId = -1;
    LocationId localLocationId = 0;

    int ret = getPolicyEnforcedSwitches(ipAclPolicy, locVector, ACL_IPV4);
    if (ret == FRAMEWORK_ERROR) {
        trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
        return(-1);
    }

    mappedId = DcmToolKit::getLocalMappedId();
    if (mappedId != (-1) ) {
        localLocationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
    }
	else
	{
		return (FRAMEWORK_ERROR);
	}

    UI32 iter_loc;

    for (iter_loc = 0; iter_loc < locVector.size(); iter_loc++)
    {
        if (localLocationId == locVector[iter_loc])
        {
            trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addExtIpAclRules location is present"));
            break;
        }
    }

    if (iter_loc == locVector.size())
    {
        return (status);
    }

    if (action == SSMACL_FORWARDING_RESULT_PERMIT_E)
    {
        action = IPF_ACCEPT;
            trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addExtIpAclRules action is permit"));
    }
    else
    {
        action = IPF_DROP;
    }

    string srcIp;
    if (srcIpType == IP_ADDR_TYPE_ANY) {
        srcIp = "any";
    } else if (srcIpType == IP_ADDR_TYPE_HOST) {
        srcIp = (pLocalAssocIpRuleToPolicyMessage->getSrcHostIpAddress().toString());
    } else if (srcIpType == IP_ADDR_TYPE_NONE) {
        string SrcIpAddress = ((pLocalAssocIpRuleToPolicyMessage->getSrcIpAddress()).toString());
        string SrcIpAddressMask = ((pLocalAssocIpRuleToPolicyMessage->getSrcIpAddressMask()).toString());
        srcIp = SrcIpAddress.c_str();
        string cidrMask = netmaskToCidr(SrcIpAddressMask);
        if(cidrMask.length()!=0)
            srcIp += (string("/") + cidrMask); 
    } else {
        trace (TRACE_LEVEL_ERROR, string("Getting Source IP from DB failed srcipType: ") + srcIpType);
        return (IP_ACL_INTERNAL_ERROR);
    }
    string destIp;
    if (destIpType == IP_ADDR_TYPE_ANY) {
        destIp = "any";
    } else if (destIpType == IP_ADDR_TYPE_HOST) {
        destIp = (pLocalAssocIpRuleToPolicyMessage->getDstHostIpAddress().toString());
	} else if (destIpType == IP_ADDR_TYPE_NONE) {
        string DstIpAddress = ((pLocalAssocIpRuleToPolicyMessage->getDstIpAddress()).toString());
        string DstIpAddressMask = ((pLocalAssocIpRuleToPolicyMessage->getDstIpAddressMask()).toString());
        destIp = DstIpAddress.c_str();
        string cidrMask = netmaskToCidr(DstIpAddressMask);
        if(cidrMask.length()!=0)
            destIp += (string("/") + cidrMask); 
    } else {
        trace (TRACE_LEVEL_ERROR, string("Getting Source IP from DB failed srcipType: ") + destIpType);
        return (IP_ACL_INTERNAL_ERROR);
    }

                    /* Setting prototype*/
                    string          protoType               = "";
                    UI32            customProtoType         = 0;
                    stringstream    customProtoTypeString;
                    UI32            proto                   = 0;

                    if (pLocalAssocIpRuleToPolicyMessage->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_ICMP)
                    {
                        protoType = "icmp ";
                        proto = IPPROTO_ICMP;
                    }
                    else if (pLocalAssocIpRuleToPolicyMessage->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_IP)
                    {
                        protoType = "ip ";
                        proto = IPPROTO_IP;
                    }
                    else if (pLocalAssocIpRuleToPolicyMessage->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_TCP)
                    {
                        protoType = "tcp ";
                        proto = IPPROTO_TCP;
                    }
                    else if (pLocalAssocIpRuleToPolicyMessage->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_UDP)
                    {
                        protoType = "udp ";
                        proto = IPPROTO_UDP;
                    }
                    else
                    {
                        customProtoType = pLocalAssocIpRuleToPolicyMessage->getL3l4CustomProtoType();
                        proto = customProtoType;
                    }

                    /* Set Port specific parameters */
                    string          srcPortOp           = "";
                    string          dstPortOp           = "";
                    UI32            srcPortNum          = 1;
                    UI32            dstPortNum          = 1;
                    UI32            srcHighNum          = IPF_MAX_PORTNUM-1;
                    UI32            dstHighNum          = IPF_MAX_PORTNUM-1;
					UI32			neqSportFlag		= 0;
					UI32			neqDportFlag		= 0;


                    if (pLocalAssocIpRuleToPolicyMessage->getSrcPortOperator() == PORTOPERATOR_EQ)
                        srcPortOp = "eq ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getSrcPortOperator() == PORTOPERATOR_GT)
                        srcPortOp = "gt ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getSrcPortOperator() == PORTOPERATOR_LT)
                        srcPortOp = "lt ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getSrcPortOperator() == PORTOPERATOR_NEQ)
                        srcPortOp = "neq ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getSrcPortOperator() == PORTOPERATOR_RANGE)
                        srcPortOp = "range ";

                    if (pLocalAssocIpRuleToPolicyMessage->getDstPortOperator() == PORTOPERATOR_EQ)
                        dstPortOp = "eq ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getDstPortOperator() == PORTOPERATOR_GT)
                        dstPortOp = "gt ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getDstPortOperator() == PORTOPERATOR_LT)
                        dstPortOp = "lt ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getDstPortOperator() == PORTOPERATOR_NEQ)
                        dstPortOp = "neq ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getDstPortOperator() == PORTOPERATOR_RANGE)
                        dstPortOp = "range ";

                    if ((srcPortOp.compare("range ") != 0) && (srcPortOp.compare("") != 0))
                    {
                        if ((srcPortOp.compare("lt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortltWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcHighNum))
                            {
								srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortltCustomNumberUdp();
							}
                        }
                        else if ((srcPortOp.compare("lt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortltWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcHighNum))
                            {
                                srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortltCustomNumberTcp();
							}
                        }
                        else if ((srcPortOp.compare("gt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortgtWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortgtCustomNumberUdp();
							}
                        }
                        else if ((srcPortOp.compare("gt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortgtWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortgtCustomNumberTcp();
							}
                        }
                        else if ((srcPortOp.compare("eq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomNumberUdp();
							}
                            srcHighNum = srcPortNum;
                        }
                        else if ((srcPortOp.compare("eq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomNumberTcp();
							}
                            srcHighNum = srcPortNum;
                        }
                        else if ((srcPortOp.compare("neq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomNumberUdp();
                            }
                            srcHighNum = srcPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqSportFlag = 1;
                        }
                        else if ((srcPortOp.compare("neq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomNumberTcp();
                            }
                            srcHighNum = srcPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqSportFlag = 1;
                        }
                    }
                    else if (srcPortOp.compare("range ") == 0)
                    {

                        if (protoType.compare("udp ") == 0)
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownrangeLowerUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomrangeLowerUdp();
                            }
                            srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownrangeHigherUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcHighNum))
                            {
                                srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomrangeHigherUdp();
                            }
                        }
                        else if (protoType.compare("tcp ") == 0)
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownrangeLowerTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomrangeLowerTcp();
                            }
                            srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownrangeHigherTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcHighNum))
                            {
                                srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomrangeHigherTcp();
                            }
                        }
                    }

                    if ((dstPortOp.compare("range ") != 0) && (dstPortOp.compare("") != 0))
                    {
                        if ((dstPortOp.compare("lt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortltWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstHighNum))
                            {
                                dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortltCustomNumberUdp();
                            }
                        }
                        else if ((dstPortOp.compare("lt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortltWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstHighNum))
                            {
                                dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortltCustomNumberTcp();
                            }
                        }
                        else if ((dstPortOp.compare("gt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortgtWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortgtCustomNumberUdp();
                            }
                        }
                        else if ((dstPortOp.compare("gt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortgtWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortgtCustomNumberTcp();
                            }
                        }
                        else if ((dstPortOp.compare("eq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomNumberUdp();
                            }
                            dstHighNum = dstPortNum;
                        }
                        else if ((dstPortOp.compare("eq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomNumberTcp();
                            }
                            dstHighNum = dstPortNum;
                        }
                        else if ((dstPortOp.compare("neq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomNumberUdp();
                            }
                            dstHighNum = dstPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqDportFlag = 1;
                        }
                        else if ((dstPortOp.compare("neq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomNumberTcp();
                            }
                            dstHighNum = dstPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqDportFlag = 1;
                        }
                    }
                    else if (dstPortOp.compare("range ") == 0)
                    {

                        if (protoType.compare("udp ") == 0)
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownrangeLowerUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomrangeLowerUdp();
                            }
                            dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownrangeHigherUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstHighNum))
                            {
                                dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomrangeHigherUdp();
                            }
                        }
                        else if (protoType.compare("tcp ") == 0)
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownrangeLowerTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomrangeLowerTcp();
                            }
                            dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownrangeHigherTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstHighNum))
                            {
                                dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomrangeHigherTcp();
                            }
                        }
                    }
					UI32    urg         = 0;
                    UI32    ack         = 0;
                    UI32    push        = 0;
                    UI32    fin         = 0;
                    UI32    rst         = 0;
                    UI32    sync        = 0;
                    /*UI32    log         = 0;
                    UI32    count       = 0;*/
					string  PrecDscpTos     = "";
					stringstream PrecDscpTosString;
					UI32	PrecDscpTosNum	= 0;

					if (pLocalAssocIpRuleToPolicyMessage->getPrecedenceUnion().compare("wellknownPrecedence") == 0) {
						//PrecDscpTosNum = IsWellKnownPrecedence(pLocalAssocIpRuleToPolicyMessage->getWellknownPrecedence());
						PrecDscpTos = "";
						PrecDscpTosNum = pLocalAssocIpRuleToPolicyMessage->getWellknownPrecedence();
						if (PrecDscpTosNum == WELLKNOWN_PRECEDENCE_UNKNOWN) {
							PrecDscpTos = "";
						}
						else {
							PrecDscpTosString << PrecDscpTosNum;
                            PrecDscpTos = PrecDscpTosString.str();
							PrecDscpTos = "precedence " + PrecDscpTos;
						}
					} else if (pLocalAssocIpRuleToPolicyMessage->getPrecedenceUnion().compare("customPrecedence") == 0) {
						PrecDscpTosNum = pLocalAssocIpRuleToPolicyMessage->getCustomPrecedence();
						PrecDscpTos = "";
						if (PrecDscpTosNum == DEFAULT_PREC_DSCP_TOS_VAL) {
							PrecDscpTos = "";
						}
						else {
							PrecDscpTosString << PrecDscpTosNum;
							PrecDscpTos = PrecDscpTosString.str();
							PrecDscpTos = "precedence " + PrecDscpTos;
						}
					}

					if (pLocalAssocIpRuleToPolicyMessage->getTosUnion().compare("wellknownTos") == 0) {
						PrecDscpTos = "";
						//PrecDscpTosNum = IsWellKnownTos(pLocalAssocIpRuleToPolicyMessage->getWellknownTos());
						PrecDscpTosNum = pLocalAssocIpRuleToPolicyMessage->getWellknownTos();
						if (PrecDscpTosNum == WELLKNOWN_TOS_UNKNOWN) {
							PrecDscpTos = "";
						} else {
							PrecDscpTosString << PrecDscpTosNum;
							PrecDscpTos = PrecDscpTosString.str();
							PrecDscpTos = "tos " + PrecDscpTos;
						}
					} else if (pLocalAssocIpRuleToPolicyMessage->getTosUnion().compare("customTos") == 0) {
						PrecDscpTos = "";
						PrecDscpTosNum = pLocalAssocIpRuleToPolicyMessage->getCustomTos();
						if (PrecDscpTosNum == DEFAULT_PREC_DSCP_TOS_VAL) {
							PrecDscpTos = "";
						} else {
							PrecDscpTosString << PrecDscpTosNum;
							PrecDscpTos = PrecDscpTosString.str();
							PrecDscpTos = "tos " + PrecDscpTos;
						}
					}

					if (pLocalAssocIpRuleToPolicyMessage->getDscpUnion().compare("wellknownDscp") == 0) {
						PrecDscpTos = "";
						PrecDscpTosNum = pLocalAssocIpRuleToPolicyMessage->getWellknownDscp();
						// PrecDscpTos = IsWellKnownDscp(PrecDscpTosNum);
						if (PrecDscpTosNum == WELLKNOWN_DSCP_UNKNOWN) {
							PrecDscpTos = "";
						}
						else {
							PrecDscpTosString << PrecDscpTosNum;
							PrecDscpTos = PrecDscpTosString.str();
							PrecDscpTos = "dscp " + PrecDscpTos;
						}
												
					} else if (pLocalAssocIpRuleToPolicyMessage->getDscpUnion().compare("customDscp") == 0) {
						PrecDscpTos = "";
						PrecDscpTosNum = pLocalAssocIpRuleToPolicyMessage->getCustomDscp();
						if (PrecDscpTosNum == DEFAULT_PREC_DSCP_TOS_VAL) {
							PrecDscpTos = "";
						} else {
							PrecDscpTosString << PrecDscpTosNum;
							PrecDscpTos = PrecDscpTosString.str();
							PrecDscpTos = "dscp " + PrecDscpTos;
						}
					}

					trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addExtIpAclRules ") + "PrecDscpTos Value: " + PrecDscpTos);

                    /* setting ack, fin, rst, sync */
                    urg = pLocalAssocIpRuleToPolicyMessage->getUrg();
                    ack = pLocalAssocIpRuleToPolicyMessage->getAck();
                    push = pLocalAssocIpRuleToPolicyMessage->getPush();
                    fin = pLocalAssocIpRuleToPolicyMessage->getFin();
                    rst = pLocalAssocIpRuleToPolicyMessage->getRst();
                    sync = pLocalAssocIpRuleToPolicyMessage->getSync();

                    /* count ang log parameter 
                    count = pLocalAssocIpRuleToPolicyMessage->getIsCount();
                    log = pLocalAssocIpRuleToPolicyMessage->getIsLog();
					*/

    //Ext. IP ACL query optimization
	vector<WaveManagedObject *> *pResultsPMO = NULL;
	vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> > extRuleMoVector;

	WaveManagedObjectSynchronousQueryContext syncQueryCtx(ExtIpAclPolicyManagedObject::getClassName());
	syncQueryCtx.addAndAttribute (new AttributeString (ipAclPolicy, "policyName"));
	syncQueryCtx.setLoadCompositions(false);
	syncQueryCtx.setLoadOneToManyRelationships(false);
	syncQueryCtx.addSelectField("objectId");
	pResultsPMO = querySynchronously (&syncQueryCtx);
	if (!pResultsPMO)
	{
		return (FRAMEWORK_ERROR);
	}
	ExtIpAclPolicyManagedObject *extPolicyMO = NULL;
	extPolicyMO = dynamic_cast<ExtIpAclPolicyManagedObject *>((*pResultsPMO)[0]);
	ObjectId objectId = extPolicyMO->getObjectId();
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);

	UI32 numrules = 0;
	WaveManagedObjectSynchronousQueryContext syncQueryCtx2(ExtIpAclRuleManagedObject::getClassName());
	syncQueryCtx2.addAndAttribute (new AttributeObjectId (objectId, "ownerManagedObjectId"));
	syncQueryCtx2.setLoadCompositions(false);
	syncQueryCtx2.setLoadOneToManyRelationships(false);
	querySynchronouslyForCount(&syncQueryCtx2, numrules);

    trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addExtIpAclRules values are: ") + "AclName " + ipAclPolicy + " action " + action + " SourceIp " + srcIp + " DestPortRange " + dstPortNum + " to " + dstHighNum + " protocol " + proto);

    trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addExtIpAclRules() srcIpType: ") + srcIpType + " hostAddr: " + srcIp);
    ipf_txn_hdl_t txnHdl = ipfTransBegin();
    if (txnHdl <= 0) {
        return (IP_ACL_DB_LOCKED);
    }

	ipfExtnPolicy_t ptable;
	ret = ipfPolicyGet(ipAclPolicy.c_str(), &ptable);
	if (ret)
	{
		trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::addExtIpAclRules - ipfPolicyGet failed");
		ipfTransAbort(txnHdl);
		return (IP_ACL_INTERNAL_ERROR);
	}
    /* if the policy was empty, then remove previous 2 rules */
    if (((numrules == 0) || (numrules == 1)) && ((ptable.numRules == 2)))
    {
        ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), 2);
        if (ret) {
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy: ipfPolicyRuleDelete(): ") + ret);
            ipfTransAbort(txnHdl);
                return (IP_ACL_INTERNAL_ERROR);
        }
        ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), 1);
        if (ret) {
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy: ipfPolicyRuleDelete(): ") + ret);
            ipfTransAbort(txnHdl);
            return (IP_ACL_INTERNAL_ERROR);
        }
    }

    // ret = addExtRulesBackdb(ipAclPolicy, seqNum, action, srcIp.c_str(), dstPortNum, dstHighNum, proto);
	ret = addExtRulesBackdbOptimized(ipAclPolicy, seqNum, action, srcIp.c_str(), dstPortNum, dstHighNum,
							destIp.c_str(), srcPortNum, srcHighNum, proto, PrecDscpTos,
							urg, ack, push, fin, rst, sync, neqSportFlag, neqDportFlag);
    if (ret == 0) {
        ret = ipfPolicyActivate(ipAclPolicy.c_str());
        if (ret == 0) {
            //TODO: Need to handle these error cases ?????
            ipfPolicySave(NULL);
            ipfTransEnd(txnHdl);
            status = WAVE_MESSAGE_SUCCESS;
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" is added successfully to the std policy: ipfPolicyActivate()") + ret);
        } else {
            ipfTransAbort(txnHdl);
            status = IP_ACL_INTERNAL_ERROR;
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added to std the policy: ipfPolicyActivate()") + ret);
        }
    } else {
        ipfTransAbort(txnHdl);
        status = IP_ACL_INTERNAL_ERROR;
        trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added successfully to the std policy: addStdRulesBackdb()") + ret);
        /* If the first operation fails, transaction aborts automatically. */
    }

    trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::addExtIpAclRules()");
    return (status);


}

#if 0 
int  LocalIpAclWorker::changeExtIpAclRules( AAALocalExtIpAclRuleMessage *pAAALocalExtIpAclRuleMessage) 
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::changeExtIpAclRules()");

	string ipAclPolicy = pAAALocalExtIpAclRuleMessage->getPolicyName();
	UI64 seqNum = pAAALocalExtIpAclRuleMessage->getSeqNum();
	IpAclAction actionEnum = pAAALocalExtIpAclRuleMessage->getAction();
	AclProtocol protocolEnum = pAAALocalExtIpAclRuleMessage->getProtocol();
	IpAddrType srcIpType = pAAALocalExtIpAclRuleMessage->getSrcIpType();
	//IpAddrType destIpType = pLocalExtIpAclRuleMessage->getDestIpType();
	AclPortType destPortType = pAAALocalExtIpAclRuleMessage->getDestPortType();
//	UI16 destPort = pLocalExtIpAclRuleMessage->getDestPort();
//	UI16 destEndPort = pLocalExtIpAclRuleMessage->getDestEndPort();
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	int action;
	if (actionEnum == ACL_PERMIT)
		action = IPF_ACCEPT;
	else
		action = IPF_DROP;
				
	string srcIp;
	if (srcIpType == IPV4_ADDR_ANY) {
		srcIp = "any";
	} else if (srcIpType == IPV4_ADDR_HOST) {
		srcIp = pAAALocalExtIpAclRuleMessage->getSrcHost();
	} else {
		trace (TRACE_LEVEL_ERROR, string("Getting Source IP from DB failed srcipType: ") + srcIpType);
		return (IP_ACL_INTERNAL_ERROR);
	}
	int protocol;
	if (protocolEnum == ACL_PROTO_TCP) 
	{
		protocol = IPPROTO_TCP;
	} else if (protocolEnum == ACL_PROTO_UDP)
	{
		protocol = IPPROTO_UDP;
	} else {
		//TODO: Need to handle the error cases.	
		return (IP_ACL_INTERNAL_ERROR);
	}

	UI32 destPort;
	UI32 destEndPort;
	if (destPortType == ACL_PORT_EQ) {
		destPort = pAAALocalExtIpAclRuleMessage->getDestPort();
		destEndPort = pAAALocalExtIpAclRuleMessage->getDestPort();
	} else if (destPortType == ACL_PORT_RANGE) {
		destPort = pAAALocalExtIpAclRuleMessage->getDestPort();
		destEndPort = pAAALocalExtIpAclRuleMessage->getDestEndPort();
	} else {
		destPort = 0;
		destEndPort = IPF_MAX_PORTNUM - 1;
		//return (IP_ACL_INTERNAL_ERROR);
	}
	
	ipf_txn_hdl_t txnHdl = ipfTransBegin();
	if (txnHdl <= 0) {
		return (IP_ACL_DB_LOCKED);
	}
	status = WAVE_MESSAGE_SUCCESS;

	/* Change operation is 'delete' and 'add'. */
	int ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), EXT_ACL_INDEX(seqNum));
	if (ret == 0) 
	{
		status = WAVE_MESSAGE_SUCCESS;
		// if the first rule needs to be added in the policy, need to remove already existing 
		// rules in the back end db. These rules only exist in the backend. 
		ret = ipfPolicyRuleAdd(ipAclPolicy.c_str(), EXT_ACL_INDEX(seqNum), srcIp.c_str(), destPort, destEndPort, protocol, action);
		if (ret == 0) {
			ret = ipfPolicyActivate(ipAclPolicy.c_str());
			if (ret == 0) {
				//TODO: Need to handle these error cases ????? 
				ipfPolicySave(NULL);
				ipfTransEnd(txnHdl);
				status = WAVE_MESSAGE_SUCCESS;
				trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" is added successfully to the ext policy ") + ipAclPolicy);
			} else {
				ipfTransAbort(txnHdl);
				status = IP_ACL_INTERNAL_ERROR;
				trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added to ext the policy ipfPolicyActivate() ") + ret);
			}
		} else {
			ipfTransAbort(txnHdl);
			status = IP_ACL_INTERNAL_ERROR;
			trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added successfully to the ext policy ipfPolicyRuleAdd()") + ret);
			/* If the first operation fails, transaction aborts automatically. */
		}
	} else {
		ipfTransAbort(txnHdl);
		status = IP_ACL_INTERNAL_ERROR;
		trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the policy ipfPolicyRuleDelete() ") + ret);
	}

	trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::changeExtIpAclRules()");
	return (status);
}
#endif

int  LocalIpAclWorker::deleteExtIpAclRules(AAALocalAssocIpRuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage) 
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::deleteExtIpAclRules()");
	// Get the rule attributes from the message.
	string ipAclPolicy = pLocalAssocIpRuleToPolicyMessage->getPolicyName();
	UI64 seqNum = pLocalAssocIpRuleToPolicyMessage->getRuleid();
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	vector <LocationId> locVector;
    SI32 mappedId = -1;
    LocationId localLocationId = 0;


    int ret = getPolicyEnforcedSwitches(ipAclPolicy, locVector, ACL_IPV4);
    if (ret == FRAMEWORK_ERROR) {
         trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
        return(-1);
    }

    mappedId = DcmToolKit::getLocalMappedId();
    if (mappedId != (-1) ) {
        localLocationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
    }

    UI32 iter_loc;

    for (iter_loc = 0; iter_loc < locVector.size(); iter_loc++)
    {
        if (localLocationId == locVector[iter_loc])
        {
            trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIpAclRules location is present"));
            break;
        }
    }

    if (iter_loc == locVector.size())
    {
        return (status);
    }

	ipf_txn_hdl_t txnHdl = ipfTransBegin();
	if (txnHdl <= 0) {
		return (IP_ACL_DB_LOCKED);
	}

	ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), EXT_ACL_INDEX(seqNum));
	if (ret == 0) {
		 ipfExtnPolicy_t ptable;

        ret = ipfPolicyGet(ipAclPolicy.c_str(), &ptable);
        if (ret)
        {
            trace (TRACE_LEVEL_INFO, "ipfPolicyGet failed");
            ipfTransAbort(txnHdl);
            status = IP_ACL_INTERNAL_ERROR;
		} 
		else if (ptable.numRules == 0) {
			/* Default Rules with seqnum 1 and 2 were added in the backend only incase of empty policy. */
			ret = enforceEmptyPolicyRules(ipAclPolicy.c_str());
			if (ret) {
				trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" empty policy couldn't be enforced successfully from the ext policy enforceEmptyPolicyRules() ") + ret);
				status = IP_ACL_INTERNAL_ERROR;
			}
		}
		if (ret == 0) {	
			ret = ipfPolicyActivate(ipAclPolicy.c_str());
			if (ret) {
				status = IP_ACL_INTERNAL_ERROR;
				trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" deleted rule couldn't be activated successfully from the ext policy ipfPolicyActivate() ") + ret);
				ipfTransAbort(txnHdl);
			} else {
				//TODO: Need to handle these error cases ????? 
				ipfPolicySave(NULL);
				ipfTransEnd(txnHdl);
			}
		}
	} else {
		ipfTransAbort(txnHdl);
		status = IP_ACL_INTERNAL_ERROR;
		trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully to the ext policy ipfPolicyRuleDelete()") + ret);
		/* If the first operation fails, transaction aborts automatically. */
	}

	trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::deleteExtIpAclRules()");
	return (status);
}

int  LocalIpAclWorker::resequenceExtIpAclRules(AAALocalIpAclReseqMessage *pLocalIpAclReseqMessage)
{
	trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::resequenceExtIpAclRules--Enter");
    string ipAclPolicy = pLocalIpAclReseqMessage->getPolicyName();
    UI64 seqId  = pLocalIpAclReseqMessage->getSeqId();
    ipfExtnPolicy_t ptable;
    ipfExtnRule_t prules[IPF_MAX_NUM_RULES];
    int seqNum  = -1, iter_rule = -1;
    int ret;

    ptable.numRules = 0;
    ret = ipfPolicyGet(ipAclPolicy.c_str(), &ptable);
    if (ret)
    {
        trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::resequenceExtIpAclRules : ipfPolicyGet failed");
        return (IP_ACL_INTERNAL_ERROR);
    }

    int numRules = ptable.numRules;
    ret = ipfPolicyRuleGet(ipAclPolicy.c_str(), prules, &numRules);
    if (ret)
    {
        trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::resequenceExtIpAclRules : ipfPolicyRuleGet failed" + numRules);
        return (IP_ACL_INTERNAL_ERROR);
    }

    ipf_txn_hdl_t txnHdl = ipfTransBegin();
    if (txnHdl <= 0) {
        trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::resequenceExtIpAclRules : ipfTransBegin failed");
        return (IP_ACL_DB_LOCKED);
    }

    /* first delete the rules */
    for (iter_rule = 0; iter_rule < ptable.numRules; iter_rule++)
    {
        seqNum = prules[iter_rule].rule_num;
        ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), seqNum);
        if (ret) {
            ipfTransAbort(txnHdl);
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy ipfPolicyRuleDelete()") + ret);
            return(IP_ACL_INTERNAL_ERROR);
        }
    }

    if (ptable.numRules)
    {
    /* add the first rule at the seqId location specified by the user */
        //ret = addExtRulesBackdb(ipAclPolicy, seqId, prules[0].action, prules[0].saddrPref, prules[0].dPort,
				 //prules[0].dEndport, prules[0].proto_num);
		ret = addExtRulesBackdbOptimized(ipAclPolicy, seqId, prules[0].action, prules[0].saddrPref,
							prules[0].dPort, prules[0].dEndport, prules[0].daddrPref, prules[0].sPort,
							prules[0].sEndport, prules[0].proto_num, prules[0].preceDscpTos, prules[0].urg,
							prules[0].ack, prules[0].psh, prules[0].fin, prules[0].rst, prules[0].syn,
							prules[0].neqSportFlag, prules[0].neqDportFlag);
        if (ret)
        {
            ipfTransAbort(txnHdl);
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqId + string(" couldn't be added successfully to the std policy: addExtRulesBackdbOptimized(): ") + ret);
            return(IP_ACL_INTERNAL_ERROR);
        }
    }

    UI64 incseqId = pLocalIpAclReseqMessage->getIncSeqId();
	seqNum = seqId;
    /* now add the resequenced rules */
    for (iter_rule = 1; iter_rule < ptable.numRules; iter_rule++)
    {
//		seqNum = prules[iter_rule].rule_num;
//      seqNum += (seqId + incseqId);
		seqNum += incseqId;
        //ret = addExtRulesBackdb(ipAclPolicy, seqNum, prules[iter_rule].action, prules[iter_rule].saddrPref, prules[iter_rule].dPort,
				 //prules[iter_rule].dEndport, prules[iter_rule].proto_num);
		ret = addExtRulesBackdbOptimized(ipAclPolicy, seqNum, prules[iter_rule].action,
							prules[iter_rule].saddrPref, prules[iter_rule].dPort,
                            prules[iter_rule].dEndport, prules[iter_rule].daddrPref,
							prules[iter_rule].sPort, prules[iter_rule].sEndport, 
							prules[iter_rule].proto_num, prules[iter_rule].preceDscpTos,
							prules[iter_rule].urg, prules[iter_rule].ack, prules[iter_rule].psh,
							prules[iter_rule].fin, prules[iter_rule].rst, prules[iter_rule].syn,
							prules[iter_rule].neqSportFlag, prules[iter_rule].neqDportFlag);

        if (ret)
        {
            ipfTransAbort(txnHdl);
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added successfully to the std policy: addExtRulesBackdbOptimized(): ") + ret);
            return(IP_ACL_INTERNAL_ERROR);
        }
    }

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    ret = ipfPolicyActivate(ipAclPolicy.c_str());
    if (ret == 0)
    {
        ipfPolicySave(NULL);
        ipfTransEnd(txnHdl);
        status = WAVE_MESSAGE_SUCCESS;
        trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" is added successfully to the std policy  ipfPolicyActivate ()") + ipAclPolicy);
    } else {
        ipfTransAbort(txnHdl);
        status = IP_ACL_INTERNAL_ERROR;
        trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added to std the policy: ipfPolicyActivate()) ") + ret);
    }
    trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::resequenceExtIpAclRules--Exit");
    return (status);
}

#if 0
void  LocalIpAclWorker::AAALocalExtIpAclRuleMessageHandler( AAALocalExtIpAclRuleMessage *pAAALocalExtIpAclRuleMessage)
{
	trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::AAALocalExtIpAclRuleMessageHandler()");
	prismAssert(pAAALocalExtIpAclRuleMessage != NULL, __FILE__, __LINE__);
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	if (pAAALocalExtIpAclRuleMessage->getOpType() == AAA_ADD_OPER) {
		status = addExtIpAclRules(pAAALocalExtIpAclRuleMessage);
		if (status != WAVE_MESSAGE_SUCCESS) {
			status = IP_ACL_RULE_ADD_ERROR;
		}
	} else if (pAAALocalExtIpAclRuleMessage->getOpType() == AAA_CHANGE_OPER) {
		status = changeExtIpAclRules(pAAALocalExtIpAclRuleMessage);
		if (status != WAVE_MESSAGE_SUCCESS) {
			status = IP_ACL_RULE_CHANGE_ERROR;
		}
	} else if (pAAALocalExtIpAclRuleMessage->getOpType() == AAA_DELETE_OPER){
		status = deleteExtIpAclRules(pAAALocalExtIpAclRuleMessage);
		if (status != WAVE_MESSAGE_SUCCESS) {
			status = IP_ACL_RULE_DEL_ERROR;
		}
	}

	pAAALocalExtIpAclRuleMessage->setCompletionStatus(status);
	reply (pAAALocalExtIpAclRuleMessage);

	trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::AAALocalExtIpAclRuleMessageHandler()");
}
ResourceId LocalIpAclWorker::getIpAclPolicyType(const string &policyName, ObjectId &policyObjId, UI32 &policyType)
{
	IpAclPolicyManagedObject *policyMO 	= NULL;
	ResourceId status 					= WAVE_MESSAGE_ERROR;


	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IpAclPolicyManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

	if (NULL != pResults) {
		UI32 numberOfResults = pResults->size ();
		if ( 0 == numberOfResults) {
			status = WAVE_MESSAGE_SUCCESS;
		}else if (1 == numberOfResults) {
			policyMO = dynamic_cast<IpAclPolicyManagedObject *>((*pResults)[0]);
			policyObjId = policyMO->getObjectId ();
			const string tableName = OrmRepository::getTableNameById (policyObjId.getClassId ());

			if (tableName.compare("ExtIpAclPolicyManagedObject") == 0) {
				policyType = SSMACL_TYPE_L3_IPV4_EXT_E;
			} else if (tableName.compare("StdIpAclPolicyManagedObject") == 0) {
				policyType = SSMACL_TYPE_L3_IPV4_STD_E;
			} else {
				policyType = SSMACL_TYPE_UNKNOWN_E;
			}
			status = WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS;
		} else {
			status = WAVE_MESSAGE_ERROR;
		}
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	} 

	return (status);
}
#endif

int LocalIpAclWorker::getPolicyEnforcedSwitches (string &policyName, vector<LocationId> &locVector, IpAclVersion version) 
{
	IpAclPolicyLocalManagedObject *pIpAclPolicyLocalMO = NULL;
	Ip6AclPolicyLocalManagedObject *pIp6AclPolicyLocalMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	// Get all the switches enforced this policy.
	if (version == ACL_IPV4) {
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyLocalManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeString(policyName, "ipaclpolicy"));
	} else {
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyLocalManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeString(policyName, "ip6aclpolicy"));
	}
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults)
	{
		trace (TRACE_LEVEL_FATAL, "Ip/Ip6AclPolicyLocalManagedObject database failed");
		delete pSyncQueryCtx;
		return (FRAMEWORK_ERROR);
	}
	else if (pResults->size() == 0)
	{
		trace (TRACE_LEVEL_INFO, string ("None of the switches are enforced with this policy") + policyName);
		delete pSyncQueryCtx;
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		return (FRAMEWORK_SUCCESS);
	}
	for (UI32 j = 0; j < pResults->size(); j++) 
	{
		string switchId;	
		if (version == ACL_IPV4) {
			pIpAclPolicyLocalMO = dynamic_cast<IpAclPolicyLocalManagedObject *>((*pResults)[j]);
			if (pIpAclPolicyLocalMO) {
				switchId = pIpAclPolicyLocalMO->getSwitchId();
				delete pIpAclPolicyLocalMO;
			}
		} else {

			pIp6AclPolicyLocalMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>((*pResults)[j]);
			if (pIp6AclPolicyLocalMO) {
				switchId = pIp6AclPolicyLocalMO->getSwitchId();
				delete pIp6AclPolicyLocalMO;
			}

		}
		if (switchId.length()) {
			trace (TRACE_LEVEL_INFO, string ("policy enforced switch is ") + switchId + policyName);
			UI32 mappedId = atoi(switchId.c_str());
			locVector.push_back(DcmToolKit::getLocationIdFromMappedId(mappedId));
		}
	}
	delete pResults;
	delete pSyncQueryCtx;

	return (FRAMEWORK_SUCCESS);
}

int LocalIpAclWorker::isIpPolicyAttachToMgmtInterface(const string& policy)
{
	vector <LocationId> locVector;
	string policyName = policy;
	int ret = getPolicyEnforcedSwitches(policyName, locVector, ACL_IPV4);
	if (ret == FRAMEWORK_ERROR)
	{
		trace (TRACE_LEVEL_ERROR, "LocalIpAclWorker::isIpPolicyAttachToMgmtInterface : Framework error while getting policy enforced switches");
		return (0);
	}

	return ((locVector.size()) ? 1 : 0);
}

void LocalIpAclWorker::ipAclRuleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::ipAclRuleSTCCallback() Enter");
#if 0
//	AAAIpAclRuleContext *pAclRuleContext = (AAAIpAclRuleContext *)pWaveSendToClusterContext->getPCallerContext();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
//	AAALocalAssocIpRuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage = (AAALocalAssocIpRuleToPolicyMessage *)pAclRuleContext->getPPrismMessage();

	if (status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		trace (TRACE_LEVEL_ERROR, "LocalIpAclWorker::ipAclRuleSTCCallback - Change in ACL failed");
	}
#endif
	trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::ipAclRuleSTCCallback() Exit");	
}

void LocalIpAclWorker::ipAclReseqSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::stdAclRuleSTCCallback() Enter");
#if 0
//	AAAIpAclRuleContext *pAclRuleContext = (AAAIpAclRuleContext *)pWaveSendToClusterContext->getPCallerContext();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
//	AAALocalAssocIpRuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage = (AAALocalAssocIpRuleToPolicyMessage *)pAclRuleContext->getPPrismMessage();

	if (status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		trace (TRACE_LEVEL_ERROR, "LocalIpAclWorker::ipAclReseqSTCCallback - Resequence operation failed");
	}
#endif
	trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::ipAclReseqSTCCallback() Exit");	
}

string LocalIpAclWorker::netmaskToCidr(string netmask)
{
	trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::netmaskToCidr Enter-- netmask is ") + netmask);
	int count;
	unsigned int int_netmask,mask;
	char netm[16];
	char * pos;
	string ret="";

	strncpy(netm,netmask.c_str(),16);
	int_netmask = 0;

//	convert string to unsigned int netmask
	pos = strtok(netm,".");
	while(pos!=NULL)
	{
	int_netmask = int_netmask<<8;
	int_netmask = int_netmask | atoi(pos);
	pos = strtok(NULL,".");
	}

	if (!(int_netmask & (1<<31)))
	{
		int_netmask = ~int_netmask;
	}
//	count set-bits from left to right, and reset them to check later for error
	count = 0;
	mask = 1;
	mask = mask<<31;
	while(int_netmask & mask)
	{
	count++;
	int_netmask = int_netmask &(~mask);
	mask = mask>>1;
	}

//	check if there is any set-bit left. if there is set-bit, show error
	ret = ret + count;
	trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::netmaskToCidr conversion ") + ret);
	if(int_netmask > 0)
		return "";
	else
		return ret;
}

void  LocalIpAclWorker::AAALocalAssocIp6RuleToPolicyMessageHandler (AAALocalAssocIp6RuleToPolicyMessage *pLocalAssocIp6RuleToPolicyMessage)
{
    trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::AAALocalAssocIp6AclRuleMessageHandler()");
    prismAssert(pLocalAssocIp6RuleToPolicyMessage != NULL, __FILE__, __LINE__);
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    UI32 opcode =   pLocalAssocIp6RuleToPolicyMessage->getOpcode();
    UI32 policyType = pLocalAssocIp6RuleToPolicyMessage->getPolicyType();

    if (opcode == IP6_ADD_OPER) {
        if (policyType == STANDARD_IP6_ACL)
        {
            trace (TRACE_LEVEL_INFO, "AAA_ADD_OPER STANDARD_IP_ACL");
            status = addStdIp6AclRules(pLocalAssocIp6RuleToPolicyMessage, ACL_IPV6);
        }
        else if (policyType == EXTENDED_IP6_ACL)
        {
            trace (TRACE_LEVEL_INFO, "AAALocalAssocIp6RuleToPolicyMessageHandler: calling addExtIpAclRules");
            status = addExtIp6AclRules(pLocalAssocIp6RuleToPolicyMessage, ACL_IPV6);
        }
        if (status != WAVE_MESSAGE_SUCCESS) {
            status = IP_ACL_RULE_ADD_ERROR;
        }
    } else if (opcode == IP6_DELETE_OPER) {
		trace (TRACE_LEVEL_INFO, "AAA_DELETE_OPER");
        if (policyType == STANDARD_IP_ACL)
        {
            status = deleteStdIp6AclRules(pLocalAssocIp6RuleToPolicyMessage, ACL_IPV6);
        }
        else if (policyType == EXTENDED_IP_ACL)
        {
            status = deleteExtIp6AclRules(pLocalAssocIp6RuleToPolicyMessage, ACL_IPV6);
        }
        if (status != WAVE_MESSAGE_SUCCESS) {
            status = IP_ACL_RULE_DEL_ERROR;
        }
    }
//  status = WAVE_MESSAGE_SUCCESS;
/*
    pLocalAssocIp6RuleToPolicyMessage->setCompletionStatus(status);
    reply (pLocalAssocIp6RuleToPolicyMessage);
*/
	if (pLocalAssocIp6RuleToPolicyMessage)
	{
		delete pLocalAssocIp6RuleToPolicyMessage;
	}
    trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::AAALocalAssocIp6RuleToPolicyMessageHandler");
}

int  LocalIpAclWorker::addStdIp6AclRules(AAALocalAssocIp6RuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage, IpAclVersion version)
{
    trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::addStdIp6AclRules()");
    // Get the rule attributes from the message.
    string ipAclPolicy = pLocalAssocIpRuleToPolicyMessage->getPolicyName();
    UI64 seqNum = pLocalAssocIpRuleToPolicyMessage->getRuleid();
    UI32 action = pLocalAssocIpRuleToPolicyMessage->getAction();
    IpAddrType srcIpType = pLocalAssocIpRuleToPolicyMessage->getSrcType();
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector <LocationId> locVector;
    SI32 mappedId = -1;
    LocationId localLocationId = 0;

    int ret = getPolicyEnforcedSwitches(ipAclPolicy, locVector, version);
    if (ret == FRAMEWORK_ERROR) {
        trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
        return(-1);
    }
    mappedId = DcmToolKit::getLocalMappedId();
    if (mappedId != (-1) ) {
        localLocationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
    }

    UI32 iter_loc;

    for (iter_loc = 0; iter_loc < locVector.size(); iter_loc++)
    {
        if (localLocationId == locVector[iter_loc])
        {
            trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIp6AclRules location is present"));
            break;
        }
    }

    if (iter_loc == locVector.size())
    {
        return (status);
    }

    if (action == SSMACL_FORWARDING_RESULT_PERMIT_E)
    {
        action = IPF_ACCEPT;
            trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIp6AclRules action is permit"));
    }
    else
    {
        action = IPF_DROP;
    }

    string srcIp;
    if (srcIpType == IP_ADDR_TYPE_ANY) {
        srcIp = "any";
    } else if (srcIpType == IP_ADDR_TYPE_HOST) {
        srcIp = (pLocalAssocIpRuleToPolicyMessage->getSrcHostIpAddress().toString());
	} else if (srcIpType == IP_ADDR_TYPE_NONE) {
		trace (TRACE_LEVEL_ERROR, string("IP_ADDR_TYPE_NONE")); 
		srcIp = (pLocalAssocIpRuleToPolicyMessage->getSrcIpAddress().toString());
    } else {
        trace (TRACE_LEVEL_ERROR, string("Getting Source IP from DB failed srcipType: ") + srcIpType);
        return (IP_ACL_INTERNAL_ERROR);
    }
    trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIp6AclRules values are") + srcIp +  ipAclPolicy + seqNum);

    trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIp6AclRules() srcIpType: ") + srcIpType + "hostAddr: " + srcIp);
    ipf_txn_hdl_t txnHdl = ipfTransBegin();
    if (txnHdl <= 0) {
        return (IP_ACL_DB_LOCKED);
    }

    vector<WaveManagedObject *> *pResultsPMO = NULL;
    vector<WaveManagedObjectPointer<StdIp6AclRuleManagedObject> > stdRuleMoVector;

    WaveManagedObjectSynchronousQueryContext syncQueryCtx(StdIp6AclPolicyManagedObject::getClassName());
    syncQueryCtx.addAndAttribute (new AttributeString (ipAclPolicy, "policyName"));
    syncQueryCtx.setLoadCompositions(false);
    syncQueryCtx.setLoadOneToManyRelationships(false);
    syncQueryCtx.addSelectField("objectId");
    pResultsPMO = querySynchronously (&syncQueryCtx);
    if (!pResultsPMO)
    {
        return (FRAMEWORK_ERROR);
    }
    StdIp6AclPolicyManagedObject *stdPolicyMO = NULL;
    stdPolicyMO = dynamic_cast<StdIp6AclPolicyManagedObject *>((*pResultsPMO)[0]);
    ObjectId objectId = stdPolicyMO->getObjectId();
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);

    UI32 numrules = 0;
    WaveManagedObjectSynchronousQueryContext syncQueryCtx2(StdIp6AclRuleManagedObject::getClassName());
    syncQueryCtx2.addAndAttribute (new AttributeObjectId (objectId, "ownerManagedObjectId"));
    syncQueryCtx2.setLoadCompositions(false);
    syncQueryCtx2.setLoadOneToManyRelationships(false);
    querySynchronouslyForCount(&syncQueryCtx2, numrules);

    ipfExtnPolicy_t ptable;
    //ipfExtnRule_t prules[IPF_MAX_NUM_RULES];
    ret = ipfPolicyGet(ipAclPolicy.c_str(), &ptable);
    if (ret)
    {
        trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::addStdIp6AclRules - ipfPolicyGet failed");
        ipfTransAbort(txnHdl);
        return (IP_ACL_INTERNAL_ERROR);
    }
#if 0
	/* Doing nothing with this parameters numRules, prules etc... */
    int numRules = ptable.numRules;
    ret = ipfPolicyRuleGet(ipAclPolicy.c_str(), prules, &numRules);
    if (ret)
    {
        trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::addStdIp6AclRules - ipfPolicyRuleGet failed" + numRules);
        ipfTransAbort(txnHdl);
        return (IP_ACL_INTERNAL_ERROR);
    }
	trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIp6AclRules prules ") +  prules[0].rule_num);
#endif
    /* if the policy was empty previously , then remove the previous 2 default rules at the backend*/
	if (((numrules == 0) || (numrules == 1)) && (ptable.numRules == 2))
    {
        ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), 2);
        if (ret) {
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy: ipfPolicyRuleDelete(): ") + ret);
            ipfTransAbort(txnHdl);
                return (IP_ACL_INTERNAL_ERROR);
        }
        ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), 1);
        if (ret) {
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy: ipfPolicyRuleDelete(): ") + ret);
            ipfTransAbort(txnHdl);
            return (IP_ACL_INTERNAL_ERROR);
        }
    }

    ret = addStdRulesBackdb(ipAclPolicy, seqNum, action, srcIp);
    if (ret == 0) {
        ret = ipfPolicyActivate(ipAclPolicy.c_str());
        if (ret == 0) {
            //TODO: Need to handle these error cases ?????
            ipfPolicySave(NULL);
            ipfTransEnd(txnHdl);
            status = WAVE_MESSAGE_SUCCESS;
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" is added successfully to the std policy: ipfPolicyActivate()") + ret);
        } else {
            ipfTransAbort(txnHdl);
            status = IP_ACL_INTERNAL_ERROR;
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added to std the policy: ipfPolicyActivate()") + ret);
        }
    } else {
        ipfTransAbort(txnHdl);
        status = IP_ACL_INTERNAL_ERROR;
        trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added successfully to the std policy: addStdRulesBackdb()") + ret);
        /* If the first operation fails, transaction aborts automatically. */
    }

    trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::addStdIp6AclRules()");
    return (status);
}

int  LocalIpAclWorker::addExtIp6AclRules(AAALocalAssocIp6RuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage, IpAclVersion version)
{
    trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::addExtIpAclRules()");
    string ipAclPolicy = pLocalAssocIpRuleToPolicyMessage->getPolicyName();
    UI64 seqNum = pLocalAssocIpRuleToPolicyMessage->getRuleid();
    UI32 action = pLocalAssocIpRuleToPolicyMessage->getAction();
    IpAddrType srcIpType = pLocalAssocIpRuleToPolicyMessage->getSrcType();
    IpAddrType destIpType = pLocalAssocIpRuleToPolicyMessage->getDstType();
    //AclPortType destPortType = pLocalExtIpAclRuleMessage->getDestPortType();
//  UI16 destPort = pLocalExtIpAclRuleMessage->getDestPort();
//  UI16 destEndPort = pLocalExtIpAclRuleMessage->getDestEndPort();
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector <LocationId> locVector;
    SI32 mappedId = -1;
    LocationId localLocationId = 0;

    int ret = getPolicyEnforcedSwitches(ipAclPolicy, locVector, version);
    if (ret == FRAMEWORK_ERROR) {
        trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
        return(-1);
    }

    mappedId = DcmToolKit::getLocalMappedId();
    if (mappedId != (-1) ) {
        localLocationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
    }
    else
    {
        return (FRAMEWORK_ERROR);
    }

    UI32 iter_loc;

    for (iter_loc = 0; iter_loc < locVector.size(); iter_loc++)
    {
        if (localLocationId == locVector[iter_loc])
        {
            trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addExtIpAclRules location is present"));
            break;
        }
    }

    if (iter_loc == locVector.size())
    {
        return (status);
    }

    if (action == SSMACL_FORWARDING_RESULT_PERMIT_E)
    {
        action = IPF_ACCEPT;
            trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addExtIpAclRules action is permit"));
    }
    else
    {
        action = IPF_DROP;
    }

    string srcIp;
    if (srcIpType == IP_ADDR_TYPE_ANY) {
        srcIp = "any";
    } else if (srcIpType == IP_ADDR_TYPE_HOST) {
        srcIp = (pLocalAssocIpRuleToPolicyMessage->getSrcHostIpAddress().toString());
    } else if (srcIpType == IP_ADDR_TYPE_NONE) {
        string SrcIpAddress = ((pLocalAssocIpRuleToPolicyMessage->getSrcIpAddress()).toString());
//        string SrcIpAddressMask = ((pLocalAssocIpRuleToPolicyMessage->getSrcIpAddressMask()).toString());
        srcIp = SrcIpAddress.c_str();
//        srcIp += " ";
//        srcIp += SrcIpAddressMask.c_str();
    } else {
        trace (TRACE_LEVEL_ERROR, string("Getting Source IP from DB failed srcipType: ") + srcIpType);
        return (IP_ACL_INTERNAL_ERROR);
    }
    string destIp;
    if (destIpType == IP_ADDR_TYPE_ANY) {
        destIp = "any";
    } else if (destIpType == IP_ADDR_TYPE_HOST) {
        destIp = (pLocalAssocIpRuleToPolicyMessage->getDstHostIpAddress().toString());
    } else if (destIpType == IP_ADDR_TYPE_NONE) {
        string DstIpAddress = ((pLocalAssocIpRuleToPolicyMessage->getDstIpAddress()).toString());
//        string DstIpAddressMask = ((pLocalAssocIpRuleToPolicyMessage->getDstIpAddressMask()).toString());
        destIp = DstIpAddress.c_str();
//        destIp += " ";
//        destIp += DstIpAddressMask.c_str();
    } else {
        trace (TRACE_LEVEL_ERROR, string("Getting Source IP from DB failed srcipType: ") + destIpType);
        return (IP_ACL_INTERNAL_ERROR);
    }

                    /* Setting prototype*/
                    string          protoType               = "";
                    UI32            customProtoType         = 0;
                    stringstream    customProtoTypeString;
                    UI32            proto                   = 0;

                    if (pLocalAssocIpRuleToPolicyMessage->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_ICMP6)
                    {
                        protoType = "ipv6-icmp ";
                        proto = L3L4_PROTOTYPE_ICMP6;
                    }
                    else if (pLocalAssocIpRuleToPolicyMessage->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_IP)
                    {
                        protoType = "ip ";
                        proto = IPPROTO_IP;
                    }
                    else if (pLocalAssocIpRuleToPolicyMessage->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_TCP)
                    {
                        protoType = "tcp ";
                        proto = IPPROTO_TCP;
                    }
                    else if (pLocalAssocIpRuleToPolicyMessage->getL3l4WellknownProtoType() == L3L4_PROTOTYPE_UDP)
                    {
                        protoType = "udp ";
                        proto = IPPROTO_UDP;
                    }
                    else
                    {
                        customProtoType = pLocalAssocIpRuleToPolicyMessage->getL3l4CustomProtoType();
                        proto = customProtoType;
                    }

                    /* Set Port specific parameters */
                    string          srcPortOp           = "";
                    string          dstPortOp           = "";
                    UI32            srcPortNum          = 0;
                    UI32            dstPortNum          = 0;
                    UI32            srcHighNum          = IPF_MAX_PORTNUM-1;
                    UI32            dstHighNum          = IPF_MAX_PORTNUM-1;
					UI32			neqSportFlag		= 0;
					UI32			neqDportFlag		= 0;

                    if (pLocalAssocIpRuleToPolicyMessage->getSrcPortOperator() == PORTOPERATOR_EQ)
                        srcPortOp = "eq ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getSrcPortOperator() == PORTOPERATOR_GT)
                        srcPortOp = "gt ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getSrcPortOperator() == PORTOPERATOR_LT)
                        srcPortOp = "lt ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getSrcPortOperator() == PORTOPERATOR_NEQ)
                        srcPortOp = "neq ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getSrcPortOperator() == PORTOPERATOR_RANGE)
                        srcPortOp = "range ";

                    if (pLocalAssocIpRuleToPolicyMessage->getDstPortOperator() == PORTOPERATOR_EQ)
                        dstPortOp = "eq ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getDstPortOperator() == PORTOPERATOR_GT)
                        dstPortOp = "gt ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getDstPortOperator() == PORTOPERATOR_LT)
                        dstPortOp = "lt ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getDstPortOperator() == PORTOPERATOR_NEQ)
                        dstPortOp = "neq ";
                    else if (pLocalAssocIpRuleToPolicyMessage->getDstPortOperator() == PORTOPERATOR_RANGE)
                        dstPortOp = "range ";

                    if ((srcPortOp.compare("range ") != 0) && (srcPortOp.compare("") != 0))
                    {
                        if ((srcPortOp.compare("lt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            srcPortNum = 1;
                            srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortltWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcHighNum))
                            {
                                srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortltCustomNumberUdp();
                            }
                        }
                        else if ((srcPortOp.compare("lt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            srcPortNum = 1;
                            srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortltWellknownNumberTcp();
                            if ((WELLKNOWNPORT_TCP_UNKNOWN == srcHighNum))
                            {
                                srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortltCustomNumberTcp();
                            }
                        }
                        else if ((srcPortOp.compare("gt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortgtWellknownNumberUdp();
                            if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortgtCustomNumberUdp();
                            }
                            srcHighNum = (IPF_MAX_PORTNUM-1);
                        }
                        else if ((srcPortOp.compare("gt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortgtWellknownNumberTcp();
                            if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortgtCustomNumberTcp();
                            }
                            srcHighNum = (IPF_MAX_PORTNUM-1);
                        }
                        else if ((srcPortOp.compare("eq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownNumberUdp();
                            if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomNumberUdp();
                            }
                            srcHighNum = srcPortNum;
                        }
                        else if ((srcPortOp.compare("eq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownNumberTcp();
                            if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomNumberTcp();
                            }
                            srcHighNum = srcPortNum;
                        }
                        else if ((srcPortOp.compare("neq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownNumberUdp();
                            if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomNumberUdp();
                            }
                            srcHighNum = srcPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqSportFlag = 1;
                        }
                        else if ((srcPortOp.compare("neq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomNumberTcp();
                            }
                            srcHighNum = srcPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqSportFlag = 1 ;
                        }
                    }
                    else if (srcPortOp.compare("range ") == 0)
                    {

                        if (protoType.compare("udp ") == 0)
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownrangeLowerUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomrangeLowerUdp();
                            }
                            srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownrangeHigherUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == srcHighNum))
                            {
                                srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomrangeHigherUdp();
                            }
                        }
                        else if (protoType.compare("tcp ") == 0)
                        {
                            srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownrangeLowerTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcPortNum))
                            {
                                srcPortNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomrangeLowerTcp();
                            }
                            srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortWellknownrangeHigherTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == srcHighNum))
                            {
                                srcHighNum = pLocalAssocIpRuleToPolicyMessage->getSrcPortCustomrangeHigherTcp();
                            }
                        }
                    }

                    if ((dstPortOp.compare("range ") != 0) && (dstPortOp.compare("") != 0))
                    {
                        if ((dstPortOp.compare("lt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            dstPortNum = 1;
                            dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortltWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstHighNum))
                            {
                                dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortltCustomNumberUdp();
                            }
                        }
                        else if ((dstPortOp.compare("lt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            dstPortNum = 1;
                            dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortltWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstHighNum))
                            {
                                dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortltCustomNumberTcp();
                            }
                        }
                        else if ((dstPortOp.compare("gt ") == 0) && (protoType.compare("udp ") == 0))
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortgtWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortgtCustomNumberUdp();
                            }
                            dstHighNum = (IPF_MAX_PORTNUM-1);
                        }
                        else if ((dstPortOp.compare("gt ") == 0) && (protoType.compare("tcp ") == 0))
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortgtWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortgtCustomNumberTcp();
                            }
                            dstHighNum = (IPF_MAX_PORTNUM-1);
                        }
                        else if ((dstPortOp.compare("eq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomNumberUdp();
                            }
                            dstHighNum = dstPortNum;
                        }
                        else if ((dstPortOp.compare("eq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomNumberTcp();
                            }
                            dstHighNum = dstPortNum;
                        }
                        else if ((dstPortOp.compare("neq ") == 0) && protoType.compare("udp ") == 0)
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownNumberUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomNumberUdp();
                            }
                            dstHighNum = dstPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqDportFlag = 1;
                        }
                        else if ((dstPortOp.compare("neq ") == 0) && protoType.compare("tcp ") == 0)
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownNumberTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomNumberTcp();
                            }
                            dstHighNum = dstPortNum;
                            //if(action == IPF_ACCEPT)action = IPF_DROP;
                            //else action = IPF_ACCEPT;
							neqDportFlag = 1;
                        }
                    }
                    else if (dstPortOp.compare("range ") == 0)
                    {

                        if (protoType.compare("udp ") == 0)
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownrangeLowerUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomrangeLowerUdp();
                            }
                            dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownrangeHigherUdp();
							if ((WELLKNOWNPORT_UDP_UNKNOWN == dstHighNum))
                            {
                                dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomrangeHigherUdp();
                            }
                        }
                        else if (protoType.compare("tcp ") == 0)
                        {
                            dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownrangeLowerTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstPortNum))
                            {
                                dstPortNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomrangeLowerTcp();
                            }
                            dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortWellknownrangeHigherTcp();
							if ((WELLKNOWNPORT_TCP_UNKNOWN == dstHighNum))
                            {
                                dstHighNum = pLocalAssocIpRuleToPolicyMessage->getDstPortCustomrangeHigherTcp();
                            }
                        }
                    }
					UI32    urg         = 0;
                    UI32    ack         = 0;
                    UI32    push        = 0;
                    UI32    fin         = 0;
                    UI32    rst         = 0;
                    UI32    sync        = 0;
                    /*UI32    log         = 0;
                    UI32    count       = 0;*/
					string  PrecDscpTos     = "";
                    stringstream PrecDscpTosString;
                    UI32    PrecDscpTosNum  = 64;

					if (pLocalAssocIpRuleToPolicyMessage->getPrecedenceUnion().compare("wellknownPrecedence") == 0) {                  
                        //PrecDscpTosNum = IsWellKnownPrecedence(pLocalAssocIpRuleToPolicyMessage->getWellknownPrecedence());          
                        PrecDscpTos = "";
                        PrecDscpTosNum = pLocalAssocIpRuleToPolicyMessage->getWellknownPrecedence();
                        if (PrecDscpTosNum == WELLKNOWN_PRECEDENCE_UNKNOWN) {
                            PrecDscpTos = "";
                        }
                        else {
                            PrecDscpTosString << PrecDscpTosNum;
                            PrecDscpTos = PrecDscpTosString.str();
                            PrecDscpTos = "precedence " + PrecDscpTos;
                        } 
                    } else if (pLocalAssocIpRuleToPolicyMessage->getPrecedenceUnion().compare("customPrecedence") == 0) {                  
                        PrecDscpTosNum = pLocalAssocIpRuleToPolicyMessage->getCustomPrecedence();
                        PrecDscpTos = "";
                        if (PrecDscpTosNum == DEFAULT_PREC_DSCP_TOS_VAL) {
                            PrecDscpTos = "";
                        }
                        else {
                            PrecDscpTosString << PrecDscpTosNum;
                            PrecDscpTos = PrecDscpTosString.str();
                            PrecDscpTos = "precedence " + PrecDscpTos;
                        }
                    }

                    if (pLocalAssocIpRuleToPolicyMessage->getTosUnion().compare("wellknownTos") == 0) {
                        PrecDscpTos = "";
                        //PrecDscpTosNum = IsWellKnownTos(pLocalAssocIpRuleToPolicyMessage->getWellknownTos());
                        PrecDscpTosNum = pLocalAssocIpRuleToPolicyMessage->getWellknownTos();
                        if (PrecDscpTosNum == WELLKNOWN_TOS_UNKNOWN) {
                            PrecDscpTos = "";
                        } else {
                            PrecDscpTosString << PrecDscpTosNum;
                            PrecDscpTos = PrecDscpTosString.str();
                            PrecDscpTos = "tos " + PrecDscpTos;
                        }
                    } else if (pLocalAssocIpRuleToPolicyMessage->getTosUnion().compare("customTos") == 0) {
                        PrecDscpTos = "";
                        PrecDscpTosNum = pLocalAssocIpRuleToPolicyMessage->getCustomTos();
                        if (PrecDscpTosNum == DEFAULT_PREC_DSCP_TOS_VAL) {
                            PrecDscpTos = "";
                        } else {
                            PrecDscpTosString << PrecDscpTosNum;
                            PrecDscpTos = PrecDscpTosString.str();
                            PrecDscpTos = "tos " + PrecDscpTos;
                        }
                    }

                    if (pLocalAssocIpRuleToPolicyMessage->getDscpUnion().compare("wellknownDscp") == 0) {
                        PrecDscpTos = "";
                        PrecDscpTosNum = pLocalAssocIpRuleToPolicyMessage->getWellknownDscp();
                        // PrecDscpTos = IsWellKnownDscp(PrecDscpTosNum);
                        if (PrecDscpTosNum == WELLKNOWN_DSCP_UNKNOWN) {
                            PrecDscpTos = "";
                        }
                        else {
                            PrecDscpTosString << PrecDscpTosNum;
                            PrecDscpTos = PrecDscpTosString.str();
                            PrecDscpTos = "dscp " + PrecDscpTos;
                        }

                    } else if (pLocalAssocIpRuleToPolicyMessage->getDscpUnion().compare("customDscp") == 0) {
                        PrecDscpTos = "";
                        PrecDscpTosNum = pLocalAssocIpRuleToPolicyMessage->getCustomDscp();
                        if (PrecDscpTosNum == DEFAULT_PREC_DSCP_TOS_VAL) {
                            PrecDscpTos = "";
                        } else {
                            PrecDscpTosString << PrecDscpTosNum;
                            PrecDscpTos = PrecDscpTosString.str();
                            PrecDscpTos = "dscp " + PrecDscpTos;
                        }
                    }
                    trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addExtIpAclRules ") + "PrecDscpTos Value: " + PrecDscpTos);
                    /* setting ack, fin, rst, sync */

                    urg = pLocalAssocIpRuleToPolicyMessage->getUrg();
                    ack = pLocalAssocIpRuleToPolicyMessage->getAck();
                    push = pLocalAssocIpRuleToPolicyMessage->getPush();
                    fin = pLocalAssocIpRuleToPolicyMessage->getFin();
                    rst = pLocalAssocIpRuleToPolicyMessage->getRst();
                    sync = pLocalAssocIpRuleToPolicyMessage->getSync();

/*                  count ang log parameter
                    count = pLocalAssocIpRuleToPolicyMessage->getIsCount();
                    log = pLocalAssocIpRuleToPolicyMessage->getIsLog();
*/

    trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addExtIpAclRules values are") + ipAclPolicy + action + srcIp + dstPortNum + dstHighNum + proto + destIp);

    trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addExtIpAclRules() srcIpType: ") + srcIpType + "hostAddr: " + srcIp);
    ipf_txn_hdl_t txnHdl = ipfTransBegin();
    if (txnHdl <= 0) {
        return (IP_ACL_DB_LOCKED);
    }

    vector<WaveManagedObject *> *pResultsPMO = NULL;
    vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > extRuleMoVector;

    WaveManagedObjectSynchronousQueryContext syncQueryCtx(ExtIp6AclPolicyManagedObject::getClassName());
    syncQueryCtx.addAndAttribute (new AttributeString (ipAclPolicy, "policyName"));
    syncQueryCtx.setLoadCompositions(false);
    syncQueryCtx.setLoadOneToManyRelationships(false);
    syncQueryCtx.addSelectField("objectId");
    pResultsPMO = querySynchronously (&syncQueryCtx);
    if (!pResultsPMO)
    {
        return (FRAMEWORK_ERROR);
    }
    ExtIp6AclPolicyManagedObject *extPolicyMO = NULL;
    extPolicyMO = dynamic_cast<ExtIp6AclPolicyManagedObject *>((*pResultsPMO)[0]);
    ObjectId objectId = extPolicyMO->getObjectId();
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);

    UI32 numrules = 0;
    WaveManagedObjectSynchronousQueryContext syncQueryCtx2(ExtIp6AclRuleManagedObject::getClassName());
    syncQueryCtx2.addAndAttribute (new AttributeObjectId (objectId, "ownerManagedObjectId"));
    syncQueryCtx2.setLoadCompositions(false);
    syncQueryCtx2.setLoadOneToManyRelationships(false);
    querySynchronouslyForCount(&syncQueryCtx2, numrules);

    ipfExtnPolicy_t ptable;
    //ipfExtnRule_t prules[IPF_MAX_NUM_RULES];
    ret = ipfPolicyGet(ipAclPolicy.c_str(), &ptable);
    if (ret)
    {
        trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::addExtIp6AclRules - ipfPolicyGet failed");
        ipfTransAbort(txnHdl);
        return (IP_ACL_INTERNAL_ERROR);
    }
#if 0
	/* Doing nothing with this parameters numRules, prules etc... */
    int numRules = ptable.numRules;
    ret = ipfPolicyRuleGet(ipAclPolicy.c_str(), prules, &numRules);
    if (ret)
    {
        trace (TRACE_LEVEL_INFO, "LocalIpAclWorker::addExtIp6AclRules - ipfPolicyRuleGet failed" + numRules);
        ipfTransAbort(txnHdl);
        return (IP_ACL_INTERNAL_ERROR);
    }

    /* if the policy was previously empty, then remove previous 2 default rules at the backend*/
	trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addExtIp6AclRules prules ") +  prules[0].rule_num);
#endif
	if (((numrules == 0) || (numrules == 1)) && (ptable.numRules == 2))
    {
        ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), 2);
        if (ret) {
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy: ipfPolicyRuleDelete(): ") + ret);
            ipfTransAbort(txnHdl);
                return (IP_ACL_INTERNAL_ERROR);
        }
        ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), 1);
        if (ret) {
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy: ipfPolicyRuleDelete(): ") + ret);
            ipfTransAbort(txnHdl);
            return (IP_ACL_INTERNAL_ERROR);
        }
    }

    // ret = addExtRulesBackdb(ipAclPolicy, seqNum, action, srcIp.c_str(), dstPortNum, dstHighNum, proto);
	ret = addExtRulesBackdbOptimized(ipAclPolicy, seqNum, action, srcIp.c_str(), dstPortNum, dstHighNum,
										destIp.c_str(), srcPortNum, srcHighNum, proto, PrecDscpTos,
										urg, ack, push, fin, rst, sync, neqSportFlag, neqDportFlag);
    if (ret == 0) {
        ret = ipfPolicyActivate(ipAclPolicy.c_str());
        if (ret == 0) {
            //TODO: Need to handle these error cases ?????
            ipfPolicySave(NULL);
            ipfTransEnd(txnHdl);
            status = WAVE_MESSAGE_SUCCESS;
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" is added successfully to the std policy: ipfPolicyActivate()") + ret);
        } else {
            ipfTransAbort(txnHdl);
            status = IP_ACL_INTERNAL_ERROR;
            trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added to std the policy: ipfPolicyActivate()") + ret);
        }
    } else {
        ipfTransAbort(txnHdl);
        status = IP_ACL_INTERNAL_ERROR;
        trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be added successfully to the std policy: addStdRulesBackdb()") + ret);
        /* If the first operation fails, transaction aborts automatically. */
    }

    trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::addExtIp6AclRules()");
    return (status);


}

int  LocalIpAclWorker::deleteStdIp6AclRules(AAALocalAssocIp6RuleToPolicyMessage *pLocalStdIpAclRuleMessage, IpAclVersion version)
{
    trace (TRACE_LEVEL_INFO, "Enter LocalIpAclWorker::deleteStdIp6AclRules()");
    // Get the rule attributes from the message.
    string ipAclPolicy = pLocalStdIpAclRuleMessage->getPolicyName();
    UI64 seqNum = pLocalStdIpAclRuleMessage->getRuleid();
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector <LocationId> locVector;
    SI32 mappedId = -1;
    LocationId localLocationId = 0;


    int ret = getPolicyEnforcedSwitches(ipAclPolicy, locVector, version);
    if (ret == FRAMEWORK_ERROR) {
         trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
        return(-1);
    }

    mappedId = DcmToolKit::getLocalMappedId();
    if (mappedId != (-1) ) {
        localLocationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
    }

    UI32 iter_loc;

    for (iter_loc = 0; iter_loc < locVector.size(); iter_loc++)
    {
        if (localLocationId == locVector[iter_loc])
        {
            trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::deleteStdIp6AclRules location is present"));
            break;
        }
    }

    if (iter_loc == locVector.size())
    {
        return (status);
    }

    ipf_txn_hdl_t txnHdl = ipfTransBegin();
    if (txnHdl <= 0) {
        return (IP_ACL_DB_LOCKED);
    }

    ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), STD_ACL_INDEX(seqNum) + 1);
    if (ret == 0) {
        ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), STD_ACL_INDEX(seqNum));
    }
    if (ret == 0) {
        ipfExtnPolicy_t ptable;

        ret = ipfPolicyGet(ipAclPolicy.c_str(), &ptable);
        if (ret)
        {
            trace (TRACE_LEVEL_INFO, "ipfPolicyGet failed");
            ipfTransAbort(txnHdl);
            status = IP_ACL_INTERNAL_ERROR;
        }
        else if (ptable.numRules == 0)
        {
            /* Default Rules with seqnum 1 and 2 were added in the backend only incase of empty policy. */
            ret = enforceEmptyPolicyRules(ipAclPolicy);
            if (ret) {
                trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully from the std policy: enforceEmptyPolicyRules() ") + ret);
                status = IP_ACL_INTERNAL_ERROR;
                ipfTransAbort(txnHdl);
            }
        }
        if (ret == 0) {
            ret = ipfPolicyActivate(ipAclPolicy.c_str());
            if (ret) {
                status = IP_ACL_INTERNAL_ERROR;
                trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be activated successfully from the std policy ipfPolicyActivate() ") + ret);
                ipfTransAbort(txnHdl);
            } else {
                //TODO: Need to handle these error cases ?????
                ipfPolicySave(NULL);
                ipfTransEnd(txnHdl);
            }
        }
    } else {
        ipfTransAbort(txnHdl);
        status = IP_ACL_INTERNAL_ERROR;
        trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully to the std policy ipfPolicyRuleDelete() ") + ret);
    }
    trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::deleteStdIp6AclRules()");
    return (status);
}

int  LocalIpAclWorker::deleteExtIp6AclRules(AAALocalAssocIp6RuleToPolicyMessage *pLocalAssocIpRuleToPolicyMessage, IpAclVersion version)
{
    trace (TRACE_LEVEL_INFO, "Enter LocalIp6AclWorker::deleteExtIpAclRules()");
    // Get the rule attributes from the message.
    string ipAclPolicy = pLocalAssocIpRuleToPolicyMessage->getPolicyName();
    UI64 seqNum = pLocalAssocIpRuleToPolicyMessage->getRuleid();
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector <LocationId> locVector;
    SI32 mappedId = -1;
    LocationId localLocationId = 0;


    int ret = getPolicyEnforcedSwitches(ipAclPolicy, locVector, version);
    if (ret == FRAMEWORK_ERROR) {
         trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
        return(-1);
    }

    mappedId = DcmToolKit::getLocalMappedId();
    if (mappedId != (-1) ) {
        localLocationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
    }

    UI32 iter_loc;

    for (iter_loc = 0; iter_loc < locVector.size(); iter_loc++)
    {
        if (localLocationId == locVector[iter_loc])
        {
            trace (TRACE_LEVEL_INFO, string("LocalIpAclWorker::addStdIpAclRules location is present"));
            break;
        }
    }

    if (iter_loc == locVector.size())
    {
        return (status);
    }

    ipf_txn_hdl_t txnHdl = ipfTransBegin();
    if (txnHdl <= 0) {
        return (IP_ACL_DB_LOCKED);
    }

    ret = ipfPolicyRuleDelete(ipAclPolicy.c_str(), EXT_ACL_INDEX(seqNum));
    if (ret == 0) {
         ipfExtnPolicy_t ptable;

        ret = ipfPolicyGet(ipAclPolicy.c_str(), &ptable);
        if (ret)
        {
            trace (TRACE_LEVEL_INFO, "ipfPolicyGet failed");
            ipfTransAbort(txnHdl);
            status = IP_ACL_INTERNAL_ERROR;
        }
        else if (ptable.numRules == 0) {
            /* Default Rules with seqnum 1 and 2 were added in the backend only incase of empty policy. */
            ret = enforceEmptyPolicyRules(ipAclPolicy.c_str());
            if (ret) {
                trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" empty policy couldn't be enforced successfully from the ext policy enforceEmptyPolicyRules() ") + ret);
                status = IP_ACL_INTERNAL_ERROR;
            }
        }
        if (ret == 0) {
            ret = ipfPolicyActivate(ipAclPolicy.c_str());
            if (ret) {
                status = IP_ACL_INTERNAL_ERROR;
                trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" deleted rule couldn't be activated successfully from the ext policy ipfPolicyActivate() ") + ret);
                ipfTransAbort(txnHdl);
            } else {
                //TODO: Need to handle these error cases ?????
                ipfPolicySave(NULL);
                ipfTransEnd(txnHdl);
            }
        }
    } else {
        ipfTransAbort(txnHdl);
        status = IP_ACL_INTERNAL_ERROR;
        trace (TRACE_LEVEL_INFO, string("Rule ") + seqNum + string(" couldn't be deleted successfully to the ext policy ipfPolicyRuleDelete()") + ret);
        /* If the first operation fails, transaction aborts automatically. */
    }

    trace (TRACE_LEVEL_INFO, "Exit LocalIpAclWorker::deleteExtIp6AclRules()");
    return (status);
}

int LocalIpAclWorker::getItfIdfromSwitchId (string switchId)
{

			size_t position = 0;

			position = switchId.find_last_of("/");
			if (position == string::npos) {
				trace (TRACE_LEVEL_ERROR, "IPADM getMappedIdfrom character error");
				return (0);
			}
			char itf = switchId.at(position+1);

			return (itf - '0');
}

	const char* LocalIpAclWorker:: IsWellKnownPrecedence (UI32 PrecDscpTosNum)
    {
        const char *PrecDscpTosString = "";
        
        if (PrecDscpTosNum == WELLKNOWN_PRECEDENCE_CRITICAL)
            PrecDscpTosString = "critical";
        else if (PrecDscpTosNum == WELLKNOWN_PRECEDENCE_FLASH)
            PrecDscpTosString = "flash";
        else if (PrecDscpTosNum == WELLKNOWN_PRECEDENCE_FLASH_OVERRIDE)
            PrecDscpTosString = "flash-override";
        else if (PrecDscpTosNum == WELLKNOWN_PRECEDENCE_IMMEDIATE)
            PrecDscpTosString = "immediate";
        else if (PrecDscpTosNum == WELLKNOWN_PRECEDENCE_INTERNET)
            PrecDscpTosString = "internet";
        else if (PrecDscpTosNum == WELLKNOWN_PRECEDENCE_NETWORK)
            PrecDscpTosString = "network";
        else if (PrecDscpTosNum == WELLKNOWN_PRECEDENCE_PRIORITY)
            PrecDscpTosString = "priority";
        else if (PrecDscpTosNum == WELLKNOWN_PRECEDENCE_ROUTINE)
            PrecDscpTosString = "routine";
        else if (PrecDscpTosNum == WELLKNOWN_PRECEDENCE_UNKNOWN)
            PrecDscpTosString = "";

        return PrecDscpTosString;
    }
	const char* LocalIpAclWorker:: IsWellKnownDscp (UI32 PrecDscpTosNum)
    {
        const char *PrecDscpTosString = "";

        if (PrecDscpTosNum == WELLKNOWN_DSCP_AF11)
            PrecDscpTosString = "af11";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_AF12)
            PrecDscpTosString = "af12";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_AF13)
            PrecDscpTosString = "af13";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_AF21)
            PrecDscpTosString = "af21";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_AF22)
            PrecDscpTosString = "af22";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_AF23)
            PrecDscpTosString = "af23";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_AF31)
            PrecDscpTosString = "af31";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_AF32)
            PrecDscpTosString = "af32";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_AF33)
            PrecDscpTosString = "af33";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_AF41)
            PrecDscpTosString = "af41";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_AF42)
            PrecDscpTosString = "af42";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_AF43)
            PrecDscpTosString = "af43";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_CS1)
            PrecDscpTosString = "cs1";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_CS2)
            PrecDscpTosString = "cs2";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_CS3)
            PrecDscpTosString = "cs3";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_CS4)
            PrecDscpTosString = "cs4";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_CS5)
            PrecDscpTosString = "cs5";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_CS6)
            PrecDscpTosString = "cs6";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_CS7)
            PrecDscpTosString = "cs7";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_EF)
            PrecDscpTosString = "ef";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_DEFAULT)
            PrecDscpTosString = "0";
        else if (PrecDscpTosNum == WELLKNOWN_DSCP_UNKNOWN)
            PrecDscpTosString = "";

        return PrecDscpTosString;
    }

	const char* LocalIpAclWorker:: IsWellKnownTos (UI32 PrecDscpTosNum)
    {
        const char *PrecDscpTosString = "";

        if (PrecDscpTosNum == WELLKNOWN_TOS_MAX_RELIABILITY)
            PrecDscpTosString = "max-reliability";
        else if (PrecDscpTosNum == WELLKNOWN_TOS_MAX_THROUGHPUT)
            PrecDscpTosString = "max-throughput";
        else if (PrecDscpTosNum == WELLKNOWN_TOS_MIN_DELAY)
            PrecDscpTosString = "min-delay";
        else if (PrecDscpTosNum == WELLKNOWN_TOS_NORMAL)
            PrecDscpTosString = "normal";
        else if (PrecDscpTosNum == WELLKNOWN_TOS_UNKNOWN)
            PrecDscpTosString = "";

        return PrecDscpTosString;
    }
}
