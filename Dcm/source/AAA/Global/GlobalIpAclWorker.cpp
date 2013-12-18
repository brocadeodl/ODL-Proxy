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

#if 0 // use commonMO
/***************************************************************************
 * Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 * All rights reserved.                                                  *
 * Author : Charanjith Reddy Kunduru                                     *
 **************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"

#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/GlobalIpAclWorker.h"
#include "AAA/Global/IpAclManagedObjects.h"
#include "AAA/Global/Ip6AclManagedObjects.h"
#include "AAA/Local/IpAclLocalManagedObjects.h"
#include "AAA/Global/AAAGlobalIpAclMessages.h"
#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Global/AAASequencerContexts.h"
#include "AAA/Local/AAALocalIpAclMessages.h"
using namespace WaveNs;

namespace DcmNs
{
GlobalIpAclWorker::GlobalIpAclWorker (AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: WaveWorker (pAAAGlobalObjectManager)
{
	IpAclRuleManagedObject ipAclRuleMO (pAAAGlobalObjectManager);
	ipAclRuleMO.setupOrm ();
	addManagedClass (IpAclRuleManagedObject::getClassName (), this);

	StdIpAclRuleManagedObject stdIpAclRuleMO (pAAAGlobalObjectManager);
	stdIpAclRuleMO.setupOrm ();
	addManagedClass (StdIpAclRuleManagedObject::getClassName (), this);

	ExtIpAclRuleManagedObject extIpAclRuleMO (pAAAGlobalObjectManager);
	extIpAclRuleMO.setupOrm ();
	addManagedClass (ExtIpAclRuleManagedObject::getClassName (), this);

	IpAclPolicyManagedObject ipAclPolicyMO (pAAAGlobalObjectManager);
	ipAclPolicyMO.setupOrm ();
	addManagedClass (IpAclPolicyManagedObject::getClassName (), this);

	StdIpAclPolicyManagedObject stdIpAclPolicyMO (pAAAGlobalObjectManager);
	stdIpAclPolicyMO.setupOrm ();
	addManagedClass (StdIpAclPolicyManagedObject::getClassName (), this);

	ExtIpAclPolicyManagedObject extIpAclPolicyMO (pAAAGlobalObjectManager);
	extIpAclPolicyMO.setupOrm ();
	addManagedClass (ExtIpAclPolicyManagedObject::getClassName (), this);
	
	Ip6AclRuleManagedObject ip6AclRuleMO (pAAAGlobalObjectManager);
	ip6AclRuleMO.setupOrm ();
	addManagedClass (Ip6AclRuleManagedObject::getClassName (), this);

	StdIp6AclRuleManagedObject stdIp6AclRuleMO (pAAAGlobalObjectManager);
	stdIp6AclRuleMO.setupOrm ();
	addManagedClass (StdIp6AclRuleManagedObject::getClassName (), this);

	ExtIp6AclRuleManagedObject extIp6AclRuleMO (pAAAGlobalObjectManager);
	extIp6AclRuleMO.setupOrm ();
	addManagedClass (ExtIp6AclRuleManagedObject::getClassName (), this);

	Ip6AclPolicyManagedObject ip6AclPolicyMO (pAAAGlobalObjectManager);
	ip6AclPolicyMO.setupOrm ();
	addManagedClass (Ip6AclPolicyManagedObject::getClassName (), this);

	StdIp6AclPolicyManagedObject stdIp6AclPolicyMO (pAAAGlobalObjectManager);
	stdIp6AclPolicyMO.setupOrm ();
	addManagedClass (StdIp6AclPolicyManagedObject::getClassName (), this);

	ExtIp6AclPolicyManagedObject extIp6AclPolicyMO (pAAAGlobalObjectManager);
	extIp6AclPolicyMO.setupOrm ();
	addManagedClass (ExtIp6AclPolicyManagedObject::getClassName (), this);
	
	addOperationMap (AAAGLOBALIPACLPOLICY, reinterpret_cast<PrismMessageHandler> (&GlobalIpAclWorker::AAAGlobalAclPolicyMessageHandler));
	addOperationMap (AAAGLOBALSTDIPACLRULE, reinterpret_cast<PrismMessageHandler> (&GlobalIpAclWorker::AAAGlobalStdAclRuleMessageHandler));
	addOperationMap (AAAGLOBALEXTIPACLRULE, reinterpret_cast<PrismMessageHandler> (&GlobalIpAclWorker::AAAGlobalExtAclRuleMessageHandler));
}

GlobalIpAclWorker::~GlobalIpAclWorker ()
{
}

PrismMessage *GlobalIpAclWorker::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{
		case AAAGLOBALIPACLPOLICY :
			pPrismMessage = new AAAGlobalIpAclPolicyMessage ();
			break;
		case AAAGLOBALSTDIPACLRULE :
			pPrismMessage = new AAAGlobalStdIpAclRuleMessage ();
			break;
		case AAAGLOBALEXTIPACLRULE :
			pPrismMessage = new AAAGlobalExtIpAclRuleMessage ();
			break;
		default :
			pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject *GlobalIpAclWorker::createManagedObjectInstance(const string &managedClassName)
{
	WaveManagedObject *pWaveManagedObject = NULL;

	if ((StdIpAclRuleManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new StdIpAclRuleManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((ExtIpAclRuleManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new ExtIpAclRuleManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((IpAclPolicyManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new IpAclPolicyManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((StdIpAclPolicyManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new StdIpAclPolicyManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((ExtIpAclPolicyManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new ExtIpAclPolicyManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((StdIp6AclRuleManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new StdIp6AclRuleManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((ExtIp6AclRuleManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new ExtIp6AclRuleManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((Ip6AclPolicyManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new Ip6AclPolicyManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((StdIp6AclPolicyManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new StdIp6AclPolicyManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((ExtIp6AclPolicyManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new ExtIp6AclPolicyManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "GlobalIpAclWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
	}

	return (pWaveManagedObject);
}

void GlobalIpAclWorker::AAAGlobalAclPolicyMessageHandler (AAAGlobalIpAclPolicyMessage *pGlobalIpAclPolicyMessage)
{
	if (pGlobalIpAclPolicyMessage->getAclVersion() == ACL_IPV4) {
		AAAGlobalIpAclPolicyMessageHandler (pGlobalIpAclPolicyMessage);
	} else {
		AAAGlobalIp6AclPolicyMessageHandler (pGlobalIpAclPolicyMessage);
	}
}

void GlobalIpAclWorker::AAAGlobalIpAclPolicyMessageHandler (AAAGlobalIpAclPolicyMessage *pGlobalIpAclPolicyMessage)
{
	trace (TRACE_LEVEL_INFO, "Enter GlobalIpAclWorker::AAAGlobalIpAclPolicyMessageHandler()");
	prismAssert(pGlobalIpAclPolicyMessage != NULL, __FILE__, __LINE__);

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	if (AAA_ADD_OPER == pGlobalIpAclPolicyMessage->getOpType())
	{
		//Same policy name shouldn't exist in both standard and extended ACL list.	
		WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
		vector<WaveManagedObject *> *pResults = NULL;
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyManagedObject::getClassName());
		string name = pGlobalIpAclPolicyMessage->getName();
		pSyncQueryCtx->addAndAttribute(new AttributeString(name, "aclname"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (NULL == pResults)
		{
			trace (TRACE_LEVEL_ERROR, "IpPolicy DB Failure.");
			delete pSyncQueryCtx;
			status = FRAMEWORK_ERROR;
			pGlobalIpAclPolicyMessage->setCompletionStatus(status);
			reply(pGlobalIpAclPolicyMessage);
			return;
		} else if (pResults->size() == 1) {
			trace (TRACE_LEVEL_ERROR, "IPv4 Policy Already Exists with same name.");
			delete pSyncQueryCtx;
			delete pResults;
			pGlobalIpAclPolicyMessage->setCompletionStatus(IP_ACL_NAME_EXISTS_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		delete pSyncQueryCtx;
		delete pResults;

		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyManagedObject::getClassName());
		name = pGlobalIpAclPolicyMessage->getName();
		pSyncQueryCtx->addAndAttribute(new AttributeString(name, "aclname"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (NULL == pResults)
		{
			trace (TRACE_LEVEL_ERROR, "IpPolicy DB Failure.");
			delete pSyncQueryCtx;
			status = FRAMEWORK_ERROR;
			pGlobalIpAclPolicyMessage->setCompletionStatus(status);
			reply(pGlobalIpAclPolicyMessage);
			return;
		} else if (pResults->size() == 1) {
			trace (TRACE_LEVEL_ERROR, "IPv6 Policy Already Exists with same name.");
			delete pSyncQueryCtx;
			delete pResults;
			pGlobalIpAclPolicyMessage->setCompletionStatus(IP_ACL_NAME_EXISTS_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		delete pSyncQueryCtx;
		delete pResults;

		IpAclPolicyManagedObject *pAclPolicyObj = NULL;
		startTransaction();

		if (STANDARD_IP_ACL == pGlobalIpAclPolicyMessage->getAclType())
		{
			pAclPolicyObj = dynamic_cast<StdIpAclPolicyManagedObject *>(createManagedObjectInstance(StdIpAclPolicyManagedObject::getClassName()));
		}
		else if (EXTENDED_IP_ACL == pGlobalIpAclPolicyMessage->getAclType())
		{
			pAclPolicyObj = dynamic_cast<ExtIpAclPolicyManagedObject *>(createManagedObjectInstance(ExtIpAclPolicyManagedObject::getClassName()));
		}
		pAclPolicyObj->setName(pGlobalIpAclPolicyMessage->getName());
		//pAclPolicyObj->setAclVersion(ACL_IPV4);
		pAclPolicyObj->setAclType(pGlobalIpAclPolicyMessage->getAclType());
		status = commitTransaction();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Adding ACL policy to the DB is successful.");
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "DB Commit transaction failed." + FrameworkToolKit::localize (status));
			status = IP_ACL_ADD_ERROR;
		}
		delete pAclPolicyObj;
	}
	else if (AAA_DELETE_OPER == pGlobalIpAclPolicyMessage->getOpType())
	{
		WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
		vector<WaveManagedObject *> *pResults = NULL;
		IpAclPolicyManagedObject *pIpAclPolicyMO = NULL;

		string name = pGlobalIpAclPolicyMessage->getName();

		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute(new AttributeString(name, "aclname"));
		pResults = querySynchronously (pSyncQueryCtx);
		if ((NULL == pResults) || (pResults->size() != 1))
		{
			trace (TRACE_LEVEL_ERROR, "Policy not found.");
			status = FRAMEWORK_ERROR;
			if (pResults != NULL)
				delete pResults;
		}
		else
		{
			pIpAclPolicyMO = dynamic_cast<IpAclPolicyManagedObject *>((*pResults)[0]);
		}
		delete pSyncQueryCtx;
		delete pResults;
		
		// Verify that this policy is not enforced.
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyLocalManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeString(name, "ipaclpolicy"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults)
		{
			trace (TRACE_LEVEL_FATAL, "IpAclPolicyLocalManagedObject database failed");
			delete pSyncQueryCtx;
			delete pIpAclPolicyMO;
			pGlobalIpAclPolicyMessage->setCompletionStatus(FRAMEWORK_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		else if (pResults->size() > 0)
		{
			trace (TRACE_LEVEL_INFO, string ("some of the switches are enforced with this policy") + name);
			delete pSyncQueryCtx;
			delete pIpAclPolicyMO;
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			pGlobalIpAclPolicyMessage->setCompletionStatus(IP_ACL_ENFORCED_POLICY_DEL_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		delete pSyncQueryCtx;
		delete pResults;


		if (STANDARD_IP_ACL == pGlobalIpAclPolicyMessage->getAclType())
		{
			pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIpAclRuleManagedObject::getClassName());
		} else if (EXTENDED_IP_ACL == pGlobalIpAclPolicyMessage->getAclType()) {
			pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIpAclRuleManagedObject::getClassName());
		}
		pSyncQueryCtx->addAndAttribute (new AttributeObjectId (pIpAclPolicyMO->getObjectId(), "ownerManagedObjectId"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults)
		{
			trace (TRACE_LEVEL_ERROR, "Acl Rule DB Error");
			delete pIpAclPolicyMO;
			delete pSyncQueryCtx;
			pGlobalIpAclPolicyMessage->setCompletionStatus(FRAMEWORK_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		else if (pResults->size() > 0)
		{
			trace (TRACE_LEVEL_INFO, string ("Rules exists in this policy") + name);
			delete pSyncQueryCtx;
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			delete pIpAclPolicyMO;
			pGlobalIpAclPolicyMessage->setCompletionStatus(IP_ACL_RULES_EXIST_DEL_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		if (NULL != pSyncQueryCtx)
		{
			delete pSyncQueryCtx;
		}
		delete pResults;
		startTransaction();
		delete pIpAclPolicyMO;
		status = commitTransaction();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Removing ACL policy from DB is successful.");
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "DB Commit transaction failed." + FrameworkToolKit::localize (status));
			status = IP_ACL_DEL_ERROR;
		}

	}

	pGlobalIpAclPolicyMessage->setCompletionStatus(status);
	reply(pGlobalIpAclPolicyMessage);
	trace (TRACE_LEVEL_DEBUG, "Exit GlobalIpAclWorker::AAAGlobalIpAclPolicyMessageHandler()");
}

void GlobalIpAclWorker::AAAGlobalIp6AclPolicyMessageHandler (AAAGlobalIpAclPolicyMessage *pGlobalIpAclPolicyMessage)
{
	trace (TRACE_LEVEL_INFO, "Enter GlobalIpAclWorker::AAAGlobalIp6AclPolicyMessageHandler()");
	prismAssert(pGlobalIpAclPolicyMessage != NULL, __FILE__, __LINE__);

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	if (AAA_ADD_OPER == pGlobalIpAclPolicyMessage->getOpType())
	{
		//Same policy name shouldn't exist in IPv4/IPv6 standard and extended ACL list.	
		WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
		vector<WaveManagedObject *> *pResults = NULL;
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyManagedObject::getClassName());
		string name = pGlobalIpAclPolicyMessage->getName();
		pSyncQueryCtx->addAndAttribute(new AttributeString(name, "aclname"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (NULL == pResults)
		{
			trace (TRACE_LEVEL_ERROR, "IpPolicy DB Failure.");
			delete pSyncQueryCtx;
			status = FRAMEWORK_ERROR;
			pGlobalIpAclPolicyMessage->setCompletionStatus(status);
			reply(pGlobalIpAclPolicyMessage);
			return;
		} else if (pResults->size() == 1) {
			trace (TRACE_LEVEL_ERROR, "IPv4 Policy Already Exists with the same name.");
			delete pSyncQueryCtx;
			delete pResults;
			pGlobalIpAclPolicyMessage->setCompletionStatus(IP_ACL_NAME_EXISTS_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		delete pSyncQueryCtx;
		delete pResults;

		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyManagedObject::getClassName());
		name = pGlobalIpAclPolicyMessage->getName();
		pSyncQueryCtx->addAndAttribute(new AttributeString(name, "aclname"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (NULL == pResults)
		{
			trace (TRACE_LEVEL_ERROR, "IpPolicy DB Failure.");
			delete pSyncQueryCtx;
			status = FRAMEWORK_ERROR;
			pGlobalIpAclPolicyMessage->setCompletionStatus(status);
			reply(pGlobalIpAclPolicyMessage);
			return;
		} else if (pResults->size() == 1) {
			trace (TRACE_LEVEL_ERROR, "IPv4 Policy Already Exists with the same name.");
			delete pSyncQueryCtx;
			delete pResults;
			pGlobalIpAclPolicyMessage->setCompletionStatus(IP_ACL_NAME_EXISTS_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		delete pSyncQueryCtx;
		delete pResults;

		Ip6AclPolicyManagedObject *pAclPolicyObj = NULL;
		startTransaction();

		if (STANDARD_IP_ACL == pGlobalIpAclPolicyMessage->getAclType())
		{
			pAclPolicyObj = dynamic_cast<StdIp6AclPolicyManagedObject *>(createManagedObjectInstance(StdIp6AclPolicyManagedObject::getClassName()));
		}
		else if (EXTENDED_IP_ACL == pGlobalIpAclPolicyMessage->getAclType())
		{
			pAclPolicyObj = dynamic_cast<ExtIp6AclPolicyManagedObject *>(createManagedObjectInstance(ExtIp6AclPolicyManagedObject::getClassName()));
		}
		pAclPolicyObj->setName(pGlobalIpAclPolicyMessage->getName());
		//pAclPolicyObj->setAclVersion(ACL_IPV4);
		pAclPolicyObj->setAclType(pGlobalIpAclPolicyMessage->getAclType());
		status = commitTransaction();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Adding ACL policy to the DB is successful.");
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "DB Commit transaction failed." + FrameworkToolKit::localize (status));
			status = IP_ACL_ADD_ERROR;
		}
		delete pAclPolicyObj;
	}
	else if (AAA_DELETE_OPER == pGlobalIpAclPolicyMessage->getOpType())
	{
		WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
		vector<WaveManagedObject *> *pResults = NULL;
		Ip6AclPolicyManagedObject *pIpAclPolicyMO = NULL;

		string name = pGlobalIpAclPolicyMessage->getName();

		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute(new AttributeString(name, "aclname"));
		pResults = querySynchronously (pSyncQueryCtx);
		if ((NULL == pResults) || (pResults->size() != 1))
		{
			trace (TRACE_LEVEL_ERROR, "Policy not found.");
			status = FRAMEWORK_ERROR;
			if (pResults != NULL)
				delete pResults;
		}
		else
		{
			pIpAclPolicyMO = dynamic_cast<Ip6AclPolicyManagedObject *>((*pResults)[0]);
		}
		delete pSyncQueryCtx;
		delete pResults;
		
		// Verify that this policy is not enforced.
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyLocalManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeString(name, "ip6aclpolicy"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults)
		{
			trace (TRACE_LEVEL_FATAL, "Ip6AclPolicyLocalManagedObject database failed");
			delete pSyncQueryCtx;
			delete pIpAclPolicyMO;
			pGlobalIpAclPolicyMessage->setCompletionStatus(FRAMEWORK_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		else if (pResults->size() > 0)
		{
			trace (TRACE_LEVEL_INFO, string ("some of the switches are enforced with this policy") + name);
			delete pSyncQueryCtx;
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			delete pIpAclPolicyMO;
			pGlobalIpAclPolicyMessage->setCompletionStatus(IP_ACL_ENFORCED_POLICY_DEL_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		delete pSyncQueryCtx;
		delete pResults;


		if (STANDARD_IP_ACL == pGlobalIpAclPolicyMessage->getAclType())
		{
			pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIp6AclRuleManagedObject::getClassName());
		} else if (EXTENDED_IP_ACL == pGlobalIpAclPolicyMessage->getAclType()) {
			pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIp6AclRuleManagedObject::getClassName());
		}
		pSyncQueryCtx->addAndAttribute (new AttributeObjectId (pIpAclPolicyMO->getObjectId(), "ownerManagedObjectId"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults)
		{
			trace (TRACE_LEVEL_ERROR, "Acl Rule DB Error");
			delete pIpAclPolicyMO;
			delete pSyncQueryCtx;
			pGlobalIpAclPolicyMessage->setCompletionStatus(FRAMEWORK_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		else if (pResults->size() > 0)
		{
			trace (TRACE_LEVEL_INFO, string ("Rules exists in this policy") + name);
			delete pSyncQueryCtx;
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			delete pIpAclPolicyMO;
			pGlobalIpAclPolicyMessage->setCompletionStatus(IP_ACL_RULES_EXIST_DEL_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		delete pSyncQueryCtx;
		delete pResults;
		startTransaction();
		delete pIpAclPolicyMO;
		status = commitTransaction();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Removing ACL policy from DB is successful.");
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "DB Commit transaction failed." + FrameworkToolKit::localize (status));
			status = IP_ACL_DEL_ERROR;
		}

	}

	pGlobalIpAclPolicyMessage->setCompletionStatus(status);
	reply(pGlobalIpAclPolicyMessage);
	trace (TRACE_LEVEL_DEBUG, "Exit GlobalIpAclWorker::AAAGlobalIp6AclPolicyMessageHandler()");
}

int GlobalIpAclWorker::getPolicyEnforcedSwitches (string &policyName, vector<LocationId> &locVector, IpAclVersion version) 
{
	IpAclPolicyLocalManagedObject *pIpAclPolicyLocalMO = NULL;
	Ip6AclPolicyLocalManagedObject *pIp6AclPolicyLocalMO = NULL;
	//IpAclPolicyManagedObject *pIpAclPolicyMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	/* Check if the ipAclPolicy exists  */
	if (version == ACL_IPV4) {
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyManagedObject::getClassName());
	} else {
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyManagedObject::getClassName());
	}
	pSyncQueryCtx->addAndAttribute (new AttributeString (policyName, "aclname"));
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults || pResults->size() != 1)
	{
		trace (TRACE_LEVEL_ERROR, "IP ACL policy doesn't exist in the DB");
		if (pResults != NULL) WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		delete pSyncQueryCtx;
		return (FRAMEWORK_ERROR);
	}
/*	else
	{
		pIpAclPolicyMO = dynamic_cast<IpAclPolicyManagedObject *>((*pResults)[0]);
	}*/
	if(NULL != pResults)
	{
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}
	delete pSyncQueryCtx;

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
			UI32 mappedId = atoi(switchId.c_str());
			locVector.push_back(DcmToolKit::getLocationIdFromMappedId(mappedId));
		}
	}
	delete pResults;
	delete pSyncQueryCtx;

	return (FRAMEWORK_SUCCESS);
}

void GlobalIpAclWorker::AAAGlobalStdAclRuleMessageHandler (AAAGlobalStdIpAclRuleMessage *pGlobalStdIpAclRuleMessage)
{
	if (pGlobalStdIpAclRuleMessage->getAclVersion() == ACL_IPV4) 
	{
		AAAGlobalStdIpAclRuleMessageHandler (pGlobalStdIpAclRuleMessage);
	} else
	{
		AAAGlobalStdIp6AclRuleMessageHandler (pGlobalStdIpAclRuleMessage);
	}
}

void GlobalIpAclWorker::AAAGlobalStdIp6AclRuleMessageHandler (AAAGlobalStdIpAclRuleMessage *pGlobalStdIpAclRuleMessage)
{
	trace (TRACE_LEVEL_INFO, "Enter GlobalIpAclWorker::AAAGlobalStdIp6AclRuleMessageHandler()");
	prismAssert(pGlobalStdIpAclRuleMessage != NULL, __FILE__, __LINE__);

	StdIp6AclPolicyManagedObject *pStdAclPolicyMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	/* Get the MO instance of the Std Acl Policy */
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIp6AclPolicyManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (pGlobalStdIpAclRuleMessage->getPolicyName(), "aclname"));
	//pSyncQueryCtx->addAndAttribute (new AttributeUI32 (ACL_IPV4, "aclVersion"));
	pSyncQueryCtx->setLoadOneToManyRelationships(false);
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults || pResults->size() != 1)
	{
		trace (TRACE_LEVEL_ERROR, "Policy does not exist");
		if (pResults != NULL)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		delete pSyncQueryCtx;
		status = IP_ACL_DOESNT_EXIST;
		pGlobalStdIpAclRuleMessage->setCompletionStatus(status);
		reply(pGlobalStdIpAclRuleMessage);
		return;
	}
	else
	{
		pStdAclPolicyMO = dynamic_cast<StdIp6AclPolicyManagedObject *>((*pResults)[0]);
	}
	delete pResults;
	delete pSyncQueryCtx;

	// Send messages to all local services
	vector <LocationId> locVector;
	string policyName = pGlobalStdIpAclRuleMessage->getPolicyName();
	int ret = getPolicyEnforcedSwitches(policyName, locVector, pGlobalStdIpAclRuleMessage->getAclVersion());
	if (ret == FRAMEWORK_ERROR) {
		trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
		pGlobalStdIpAclRuleMessage->setCompletionStatus(IP_ACL_INTERNAL_ERROR);
		reply(pGlobalStdIpAclRuleMessage);
		delete pStdAclPolicyMO;
		return;
	} else if (locVector.size() == 0) {
		/*In this case, this particular policy is not enabled in any of the Mgmt I/F instances. */
		processStdIp6AclRules(pGlobalStdIpAclRuleMessage);	
		delete pStdAclPolicyMO;
		return;
	}
	AAALocalStdIpAclRuleMessage *pMessage;
	if (pGlobalStdIpAclRuleMessage->getOpType() == AAA_CHANGE_OPER) {
		StdIp6AclRuleManagedObject *pStdAclRuleMO = NULL;
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIp6AclRuleManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeUI64 (pGlobalStdIpAclRuleMessage->getSeqNum(), "seqNum"));
		pSyncQueryCtx->addAndAttribute (new AttributeObjectId (pStdAclPolicyMO->getObjectId(), "ownerManagedObjectId"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults || pResults->size() != 1)
		{
			trace (TRACE_LEVEL_ERROR, "Rule does not exist");
			if (pResults != NULL)
				delete pResults;
			delete pSyncQueryCtx;
			delete pStdAclPolicyMO;
			status = IP_ACL_RULE_DOESNT_EXIST;
			pGlobalStdIpAclRuleMessage->setCompletionStatus(status);
			reply(pGlobalStdIpAclRuleMessage);
			return;
		}
		else
		{
			pStdAclRuleMO = dynamic_cast<StdIp6AclRuleManagedObject *>((*pResults)[0]);
		}
		delete pResults;
		delete pSyncQueryCtx;
		IpAclAction action = pGlobalStdIpAclRuleMessage->getAction();
		string srcHostAddr = pGlobalStdIpAclRuleMessage->getSrcHost();
		IpAddrType srcIpType = pGlobalStdIpAclRuleMessage->getSrcIpType();
		//WaveUnion srcIpUnion = pGlobalStdIpAclRuleMessage->getSrcIpUnion();

		/*  */
		if (action == ACL_ACTION_UNDEFINED)
		{
			action = pStdAclRuleMO->getAction();
		}
		// If source IP Address is changed.
		if (srcIpType == IPV4_ADDR_UNDEFINED)
		{
			srcIpType = pStdAclRuleMO->getSrcIpType();
			srcHostAddr = pStdAclRuleMO->getSrcHost();
		} else if (srcIpType == IPV4_ADDR_ANY) {
			srcHostAddr = "";
		}
		delete pStdAclRuleMO;
		pMessage = new AAALocalStdIpAclRuleMessage (pGlobalStdIpAclRuleMessage->getPolicyName(), pGlobalStdIpAclRuleMessage->getSeqNum(), action, srcHostAddr, srcIpType, ACL_IPV6, pGlobalStdIpAclRuleMessage->getOpType());
	} else {
		pMessage = new AAALocalStdIpAclRuleMessage (pGlobalStdIpAclRuleMessage->getPolicyName(), pGlobalStdIpAclRuleMessage->getSeqNum(), pGlobalStdIpAclRuleMessage->getAction(), pGlobalStdIpAclRuleMessage->getSrcHost(), pGlobalStdIpAclRuleMessage->getSrcIpType(), ACL_IPV6, pGlobalStdIpAclRuleMessage->getOpType());
	}
	delete pStdAclPolicyMO;
	AAAStdAclRuleContext *pAclRuleContext = new AAAStdAclRuleContext (pGlobalStdIpAclRuleMessage);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalIpAclWorker::stdAclRuleSTCCallback), pAclRuleContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	pWaveSendToClusterContext->setLocationsToSendToForPhase1(locVector);
	sendToWaveCluster (pWaveSendToClusterContext);
	trace (TRACE_LEVEL_INFO, "Exit GlobalIpAclWorker::AAAGlobalStdIp6AclRuleMessageHandler()");
}

void GlobalIpAclWorker::AAAGlobalStdIpAclRuleMessageHandler (AAAGlobalStdIpAclRuleMessage *pGlobalStdIpAclRuleMessage)
{
	trace (TRACE_LEVEL_INFO, "Enter GlobalIpAclWorker::AAAGlobalStdIpAclRuleMessageHandler()");
	prismAssert(pGlobalStdIpAclRuleMessage != NULL, __FILE__, __LINE__);

	StdIpAclPolicyManagedObject *pStdAclPolicyMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	/* Get the MO instance of the Std Acl Policy */
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIpAclPolicyManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (pGlobalStdIpAclRuleMessage->getPolicyName(), "aclname"));
	//pSyncQueryCtx->addAndAttribute (new AttributeUI32 (ACL_IPV4, "aclVersion"));
	pSyncQueryCtx->setLoadOneToManyRelationships(false);
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults || pResults->size() != 1)
	{
		trace (TRACE_LEVEL_ERROR, "Policy does not exist");
		if (pResults != NULL)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		delete pSyncQueryCtx;
		status = IP_ACL_DOESNT_EXIST;
		pGlobalStdIpAclRuleMessage->setCompletionStatus(status);
		reply(pGlobalStdIpAclRuleMessage);
		return;
	}
	else
	{
		pStdAclPolicyMO = dynamic_cast<StdIpAclPolicyManagedObject *>((*pResults)[0]);
	}
	delete pResults;
	delete pSyncQueryCtx;

	// Send messages to all local services
	vector <LocationId> locVector;
	string policyName = pGlobalStdIpAclRuleMessage->getPolicyName();
	int ret = getPolicyEnforcedSwitches(policyName, locVector, pGlobalStdIpAclRuleMessage->getAclVersion());
	if (ret == FRAMEWORK_ERROR) {
		trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
		delete pStdAclPolicyMO;
		pGlobalStdIpAclRuleMessage->setCompletionStatus(IP_ACL_INTERNAL_ERROR);
		reply(pGlobalStdIpAclRuleMessage);
		return;
	} else if (locVector.size() == 0) {
		/*In this case, this particular policy is not enabled in any of the Mgmt I/F instances. */
		processStdAclRules(pGlobalStdIpAclRuleMessage);
		delete pStdAclPolicyMO;
		return;
	}
	AAALocalStdIpAclRuleMessage *pMessage;
	if (pGlobalStdIpAclRuleMessage->getOpType() == AAA_CHANGE_OPER) {
		StdIpAclRuleManagedObject *pStdAclRuleMO = NULL;
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIpAclRuleManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeUI64 (pGlobalStdIpAclRuleMessage->getSeqNum(), "seqNum"));
		pSyncQueryCtx->addAndAttribute (new AttributeObjectId (pStdAclPolicyMO->getObjectId(), "ownerManagedObjectId"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults || pResults->size() != 1)
		{
			trace (TRACE_LEVEL_ERROR, "Rule does not exist");
			if (pResults != NULL)
				delete pResults;
			delete pSyncQueryCtx;
			delete pStdAclPolicyMO;
			status = IP_ACL_RULE_DOESNT_EXIST;
			pGlobalStdIpAclRuleMessage->setCompletionStatus(status);
			reply(pGlobalStdIpAclRuleMessage);
			return;
		}
		else
		{
			pStdAclRuleMO = dynamic_cast<StdIpAclRuleManagedObject *>((*pResults)[0]);
			if (NULL == pStdAclRuleMO)
			{
				pGlobalStdIpAclRuleMessage->setCompletionStatus (IP_ACL_RULE_DOESNT_EXIST);
				if (pResults != NULL) delete pResults;
				delete pSyncQueryCtx;
				delete pStdAclPolicyMO;
				reply (pGlobalStdIpAclRuleMessage);
				return;
			}
		}
		delete pResults;
		delete pSyncQueryCtx;
		IpAclAction action = pGlobalStdIpAclRuleMessage->getAction();
		string srcHostAddr = pGlobalStdIpAclRuleMessage->getSrcHost();
		IpAddrType srcIpType = pGlobalStdIpAclRuleMessage->getSrcIpType();
		//WaveUnion srcIpUnion = pGlobalStdIpAclRuleMessage->getSrcIpUnion();

		/*  */
		if (action == ACL_ACTION_UNDEFINED)
		{
			action = pStdAclRuleMO->getAction();
		}
		// If source IP Address is changed.
		if (srcIpType == IPV4_ADDR_UNDEFINED)
		{
			srcIpType = pStdAclRuleMO->getSrcIpType();
			srcHostAddr = pStdAclRuleMO->getSrcHost();
		} else if (srcIpType == IPV4_ADDR_ANY) {
			srcHostAddr = "";
		}
	
		delete pStdAclRuleMO;
		pMessage = new AAALocalStdIpAclRuleMessage (pGlobalStdIpAclRuleMessage->getPolicyName(), pGlobalStdIpAclRuleMessage->getSeqNum(), action, srcHostAddr, srcIpType, ACL_IPV4, pGlobalStdIpAclRuleMessage->getOpType());
	} else {
		pMessage = new AAALocalStdIpAclRuleMessage (pGlobalStdIpAclRuleMessage->getPolicyName(), pGlobalStdIpAclRuleMessage->getSeqNum(), pGlobalStdIpAclRuleMessage->getAction(), pGlobalStdIpAclRuleMessage->getSrcHost(), pGlobalStdIpAclRuleMessage->getSrcIpType(), ACL_IPV4, pGlobalStdIpAclRuleMessage->getOpType());
	}
	delete pStdAclPolicyMO;
	AAAStdAclRuleContext *pAclRuleContext = new AAAStdAclRuleContext (pGlobalStdIpAclRuleMessage);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalIpAclWorker::stdAclRuleSTCCallback), pAclRuleContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	pWaveSendToClusterContext->setLocationsToSendToForPhase1(locVector);
	sendToWaveCluster (pWaveSendToClusterContext);
	trace (TRACE_LEVEL_INFO, "Exit GlobalIpAclWorker::AAAGlobalStdIpAclRuleMessageHandler()");
}

void GlobalIpAclWorker::processStdIp6AclRules(AAAGlobalStdIpAclRuleMessage *pGlobalStdIpAclRuleMessage)
{
	trace (TRACE_LEVEL_INFO, "GlobalIpAclWorker::processStdIp6AclRules Enter");
	StdIp6AclPolicyManagedObject *pStdAclPolicyMO = NULL;
	StdIp6AclRuleManagedObject *pStdAclRuleMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	string policyName = pGlobalStdIpAclRuleMessage->getPolicyName();
	UI64 sequenceNumber = pGlobalStdIpAclRuleMessage->getSeqNum();
	IpAclAction action = pGlobalStdIpAclRuleMessage->getAction();
	IpAddrType srcIpType = pGlobalStdIpAclRuleMessage->getSrcIpType();
	string srcHostAddr = pGlobalStdIpAclRuleMessage->getSrcHost();
	AAAGlobalMessageOperationType operationType = pGlobalStdIpAclRuleMessage->getOpType();

	/* Get the MO instance of the Std Acl Policy */
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIp6AclPolicyManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (policyName, "aclname"));
	pSyncQueryCtx->setLoadOneToManyRelationships(false);
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults || pResults->size() != 1)
	{
		trace (TRACE_LEVEL_ERROR, "Policy does not exist");
		if (pResults != NULL)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		delete pSyncQueryCtx;
		status = IP_ACL_DOESNT_EXIST;
		pGlobalStdIpAclRuleMessage->setCompletionStatus(status);
		reply(pGlobalStdIpAclRuleMessage);
		return;
	}
	else
	{
		pStdAclPolicyMO = dynamic_cast<StdIp6AclPolicyManagedObject *>((*pResults)[0]);
	}
	delete pResults;
	delete pSyncQueryCtx;
	vector<WaveManagedObjectPointer<StdIp6AclRuleManagedObject> > stdRuleMoVector;
	/* For CHANGE or DELETE operation, get the MO instance of the rule */
	if (AAA_CHANGE_OPER == operationType || AAA_DELETE_OPER == operationType)
	{
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIp6AclRuleManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeUI64 (sequenceNumber, "seqNum"));
		pSyncQueryCtx->addAndAttribute (new AttributeObjectId (pStdAclPolicyMO->getObjectId(), "ownerManagedObjectId"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults || pResults->size() != 1)
		{
			trace (TRACE_LEVEL_ERROR, "Rule does not exist");
			if (pResults != NULL)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			delete pSyncQueryCtx;
			delete pStdAclPolicyMO;
			status = IP_ACL_RULE_DOESNT_EXIST;
			pGlobalStdIpAclRuleMessage->setCompletionStatus(status);
			reply(pGlobalStdIpAclRuleMessage);
			return;
		}
		else
		{
			pStdAclRuleMO = dynamic_cast<StdIp6AclRuleManagedObject *>((*pResults)[0]);
		}
		delete pResults;
		delete pSyncQueryCtx;
	}

	/* Make changes to DCM database */
	if (AAA_DELETE_OPER == operationType)
	{
		startTransaction();
		trace (TRACE_LEVEL_INFO, string("Deleting the standard IP ACL rule#") + sequenceNumber + string(" from the database ..."));
		deleteFromComposition(StdIp6AclPolicyManagedObject::getClassName(), StdIp6AclRuleManagedObject::getClassName(), "stdRules", pStdAclPolicyMO->getObjectId(), pStdAclRuleMO->getObjectId());
		delete pStdAclRuleMO;
		status = commitTransaction();
	}
	else
	{
		startTransaction();
		if (AAA_ADD_OPER == operationType)
		{
			trace (TRACE_LEVEL_INFO, string("Creating a new standard IP ACL rule#") + sequenceNumber + string(" in the database ..."));
			pStdAclRuleMO = dynamic_cast<StdIp6AclRuleManagedObject *>(createManagedObjectInstance(StdIp6AclRuleManagedObject::getClassName()));
			pStdAclRuleMO->setOwnerManagedObjectId(pStdAclPolicyMO->getObjectId());

			addToComposition(StdIp6AclPolicyManagedObject::getClassName(), StdIp6AclRuleManagedObject::getClassName(), "stdRules", pStdAclPolicyMO->getObjectId(), pStdAclRuleMO->getObjectId());

			pStdAclRuleMO->setSeqNum(sequenceNumber);
			//pStdAclPolicyMO->addRule(pStdAclRuleMO);
		}
		else if (AAA_CHANGE_OPER == operationType)
		{
			trace (TRACE_LEVEL_INFO, string("Changing a standard IP ACL rule#") + sequenceNumber + string(" final values action") + action +string("srcIpType: ") + srcIpType + " srcHostAddr: " + srcHostAddr);
			/*  */
			if (action == ACL_ACTION_UNDEFINED)
			{
				action = pStdAclRuleMO->getAction();
			}
			// The following 'if' block should be a no-op if 'Source' is the last leaf/choice.
			if (srcIpType == IPV4_ADDR_UNDEFINED)
			{
				srcIpType = pStdAclRuleMO->getSrcIpType();
				srcHostAddr = pStdAclRuleMO->getSrcHost();
			} else if (srcIpType == IPV4_ADDR_ANY) {
				srcHostAddr = "";
			}

			updateWaveManagedObject(pStdAclRuleMO);
		}
		trace (TRACE_LEVEL_INFO, string("Changing a standard IP ACL rule#") + sequenceNumber + string(" with action") + action +string("srcIpType: ") + srcIpType + " srcHostAddr: " + srcHostAddr);

		pStdAclRuleMO->setAction((IpAclAction)action);
		pStdAclRuleMO->setSrcHost(srcHostAddr);
		pStdAclRuleMO->setSrcIpType(srcIpType);

		status = commitTransaction();
		delete pStdAclRuleMO;
	}
	if (status == FRAMEWORK_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "Making changes for the rule in the DB is successful.");
		status = WAVE_MESSAGE_SUCCESS;
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Making changes for the rule in the database FAILED!!.");
	}

	delete pStdAclPolicyMO;
	pGlobalStdIpAclRuleMessage->setCompletionStatus(status);
	reply(pGlobalStdIpAclRuleMessage);
	trace (TRACE_LEVEL_INFO, "GlobalIpAclWorker::processStdIp6AclRules Exit");
}

void GlobalIpAclWorker::processStdAclRules(AAAGlobalStdIpAclRuleMessage *pGlobalStdIpAclRuleMessage)
{
	trace (TRACE_LEVEL_INFO, "GlobalIpAclWorker::processStdAclRules Enter");
	StdIpAclPolicyManagedObject *pStdAclPolicyMO = NULL;
	StdIpAclRuleManagedObject *pStdAclRuleMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	string policyName = pGlobalStdIpAclRuleMessage->getPolicyName();
	UI64 sequenceNumber = pGlobalStdIpAclRuleMessage->getSeqNum();
	IpAclAction action = pGlobalStdIpAclRuleMessage->getAction();
	IpAddrType srcIpType = pGlobalStdIpAclRuleMessage->getSrcIpType();
	string srcHostAddr = pGlobalStdIpAclRuleMessage->getSrcHost();
	AAAGlobalMessageOperationType operationType = pGlobalStdIpAclRuleMessage->getOpType();

	/* Get the MO instance of the Std Acl Policy */
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIpAclPolicyManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (policyName, "aclname"));
	pSyncQueryCtx->setLoadOneToManyRelationships(false);
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults || pResults->size() != 1)
	{
		trace (TRACE_LEVEL_ERROR, "Policy does not exist");
		if (pResults != NULL)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		delete pSyncQueryCtx;
		status = IP_ACL_DOESNT_EXIST;
		pGlobalStdIpAclRuleMessage->setCompletionStatus(status);
		reply(pGlobalStdIpAclRuleMessage);
		return;
	}
	else
	{
		pStdAclPolicyMO = dynamic_cast<StdIpAclPolicyManagedObject *>((*pResults)[0]);
	}
	delete pResults;
	delete pSyncQueryCtx;
	vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> > stdRuleMoVector;
	/* For CHANGE or DELETE operation, get the MO instance of the rule */
	if (AAA_CHANGE_OPER == operationType || AAA_DELETE_OPER == operationType)
	{
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIpAclRuleManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeUI64 (sequenceNumber, "seqNum"));
		pSyncQueryCtx->addAndAttribute (new AttributeObjectId (pStdAclPolicyMO->getObjectId(), "ownerManagedObjectId"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults || pResults->size() != 1)
		{
			trace (TRACE_LEVEL_ERROR, "Rule does not exist");
			if (pResults != NULL)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			delete pSyncQueryCtx;
			delete pStdAclPolicyMO;
			status = IP_ACL_RULE_DOESNT_EXIST;
			pGlobalStdIpAclRuleMessage->setCompletionStatus(status);
			reply(pGlobalStdIpAclRuleMessage);
			return;
		}
		else
		{
			pStdAclRuleMO = dynamic_cast<StdIpAclRuleManagedObject *>((*pResults)[0]);
			if (NULL == pStdAclRuleMO)
			{
				delete pSyncQueryCtx;
				delete pStdAclPolicyMO;
				pGlobalStdIpAclRuleMessage->setCompletionStatus (IP_ACL_RULE_DOESNT_EXIST);
				if (pResults != NULL)
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				reply (pGlobalStdIpAclRuleMessage);
				return;
			}
		}
		delete pResults;
		delete pSyncQueryCtx;
	}

	/* Make changes to DCM database */
	if (AAA_DELETE_OPER == operationType)
	{
		startTransaction();
		trace (TRACE_LEVEL_INFO, string("Deleting the standard IP ACL rule#") + sequenceNumber + string(" from the database ..."));
		deleteFromComposition(StdIpAclPolicyManagedObject::getClassName(), StdIpAclRuleManagedObject::getClassName(), "stdRules", pStdAclPolicyMO->getObjectId(), pStdAclRuleMO->getObjectId());
		delete pStdAclRuleMO;
		status = commitTransaction();
	}
	else
	{
		startTransaction();
		if (AAA_ADD_OPER == operationType)
		{
			trace (TRACE_LEVEL_INFO, string("Creating a new standard IP ACL rule#") + sequenceNumber + string(" in the database ..."));
			pStdAclRuleMO = dynamic_cast<StdIpAclRuleManagedObject *>(createManagedObjectInstance(StdIpAclRuleManagedObject::getClassName()));
			pStdAclRuleMO->setOwnerManagedObjectId(pStdAclPolicyMO->getObjectId());

			addToComposition(StdIpAclPolicyManagedObject::getClassName(), StdIpAclRuleManagedObject::getClassName(), "stdRules", pStdAclPolicyMO->getObjectId(), pStdAclRuleMO->getObjectId());

			pStdAclRuleMO->setSeqNum(sequenceNumber);
			//pStdAclPolicyMO->addRule(pStdAclRuleMO);
		}
		else if (AAA_CHANGE_OPER == operationType)
		{
			trace (TRACE_LEVEL_INFO, string("Changing a standard IP ACL rule#") + sequenceNumber + string(" final values action") + action +string("srcIpType: ") + srcIpType + " srcHostAddr: " + srcHostAddr);
			/*  */
			if (action == ACL_ACTION_UNDEFINED)
			{
				action = pStdAclRuleMO->getAction();
			}
			// The following 'if' block should be a no-op if 'Source' is the last leaf/choice.
			if (srcIpType == IPV4_ADDR_UNDEFINED)
			{
				srcIpType = pStdAclRuleMO->getSrcIpType();
				srcHostAddr = pStdAclRuleMO->getSrcHost();
			} else if (srcIpType == IPV4_ADDR_ANY) {
				srcHostAddr = "";
			}

			updateWaveManagedObject(pStdAclRuleMO);
		}
		trace (TRACE_LEVEL_INFO, string("Changing a standard IP ACL rule#") + sequenceNumber + string(" with action") + action +string("srcIpType: ") + srcIpType + " srcHostAddr: " + srcHostAddr);

		pStdAclRuleMO->setAction((IpAclAction)action);
		pStdAclRuleMO->setSrcHost(srcHostAddr);
		pStdAclRuleMO->setSrcIpType(srcIpType);

		status = commitTransaction();
		delete pStdAclRuleMO;
	}
	if (status == FRAMEWORK_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "Making changes for the rule in the DB is successful.");
		status = WAVE_MESSAGE_SUCCESS;
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Making changes for the rule in the database FAILED!!.");
	}

	delete pStdAclPolicyMO;
	pGlobalStdIpAclRuleMessage->setCompletionStatus(status);
	reply(pGlobalStdIpAclRuleMessage);
	trace (TRACE_LEVEL_INFO, "GlobalIpAclWorker::processStdAclRules Exit");
}

void GlobalIpAclWorker::stdAclRuleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalIpAclWorker::stdAclRuleSTCCallback() Enter");
	AAAStdAclRuleContext *pAAAContext = (AAAStdAclRuleContext *)pWaveSendToClusterContext->getPCallerContext();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalStdIpAclRuleMessage *pGlobalStdIpAclRuleMessage = (AAAGlobalStdIpAclRuleMessage *)pAAAContext->getPPrismMessage();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
	status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Change user operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Change user operation failed on all nodes");

		vector<LocationId> cxLocations;
		FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalIpAclWorker::stdAclRuleSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? USER_CHANGE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? USER_CHANGE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		if (pGlobalStdIpAclRuleMessage->getAclVersion() == ACL_IPV4) {
			processStdAclRules(pGlobalStdIpAclRuleMessage);	
		} else {
			processStdIp6AclRules(pGlobalStdIpAclRuleMessage);	
		}
	} else {
		pGlobalStdIpAclRuleMessage->setCompletionStatus(status);
		reply(pGlobalStdIpAclRuleMessage);
	}
	trace (TRACE_LEVEL_INFO, "GlobalIpAclWorker::stdAclRuleSTCCallback() Exit");
}

void GlobalIpAclWorker::AAAGlobalExtAclRuleMessageHandler (AAAGlobalExtIpAclRuleMessage *pGlobalExtIpAclRuleMessage)
{
	if (pGlobalExtIpAclRuleMessage->getAclVersion() == ACL_IPV4) 
	{
		AAAGlobalExtIpAclRuleMessageHandler (pGlobalExtIpAclRuleMessage);
	} else
	{
		AAAGlobalExtIp6AclRuleMessageHandler (pGlobalExtIpAclRuleMessage);
	}
}

void GlobalIpAclWorker::AAAGlobalExtIp6AclRuleMessageHandler (AAAGlobalExtIpAclRuleMessage *pGlobalExtIpAclRuleMessage)
{
	trace (TRACE_LEVEL_INFO, "Enter GlobalIpAclWorker::AAAGlobalExtIp6AclRuleMessageHandler()");
	prismAssert(pGlobalExtIpAclRuleMessage != NULL, __FILE__, __LINE__);

	ExtIp6AclPolicyManagedObject *pExtAclPolicyMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	/* Get the MO instance of the Ext Acl Policy */
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIp6AclPolicyManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (pGlobalExtIpAclRuleMessage->getPolicyName(), "aclname"));
	pSyncQueryCtx->setLoadOneToManyRelationships(false);
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults || pResults->size() != 1)
	{
		trace (TRACE_LEVEL_ERROR, "Policy does not exist");
		if (pResults != NULL)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		delete pSyncQueryCtx;
		status = IP_ACL_DOESNT_EXIST;
		pGlobalExtIpAclRuleMessage->setCompletionStatus(IP_ACL_DOESNT_EXIST);
		reply(pGlobalExtIpAclRuleMessage);
		return;
	}
	else
	{
		pExtAclPolicyMO = dynamic_cast<ExtIp6AclPolicyManagedObject *>((*pResults)[0]);
	}
	delete pResults;
	delete pSyncQueryCtx;
	
	//Validating the range/eq when adding a new rule.
	if (pGlobalExtIpAclRuleMessage->getOpType() == AAA_ADD_OPER) {
		if (pGlobalExtIpAclRuleMessage->getDestPortType() == ACL_PORT_RANGE) {
			if ((pGlobalExtIpAclRuleMessage->getDestPort() == IP_ACL_NO_PORT_VAL) ||
				(pGlobalExtIpAclRuleMessage->getDestEndPort() == IP_ACL_NO_PORT_VAL) ||	
				pGlobalExtIpAclRuleMessage->getDestEndPort() < pGlobalExtIpAclRuleMessage->getDestPort()) {
				delete pExtAclPolicyMO;
				status = IP_ACL_INVALID_DESTPORT_RANGE;
				pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
				reply(pGlobalExtIpAclRuleMessage);
				return;
			}
		} else if (pGlobalExtIpAclRuleMessage->getDestPortType() == ACL_PORT_EQ) {
			if (pGlobalExtIpAclRuleMessage->getDestPort() == IP_ACL_NO_PORT_VAL) {
				delete pExtAclPolicyMO;
				status = IP_ACL_INVALID_DESTPORT;
				pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
				reply(pGlobalExtIpAclRuleMessage);
				return;
			}
		}
	}
	// Send messages to all local services
	vector <LocationId> locVector;
	string policyName = pGlobalExtIpAclRuleMessage->getPolicyName();
	int ret = getPolicyEnforcedSwitches(policyName, locVector, pGlobalExtIpAclRuleMessage->getAclVersion());
	if (ret == FRAMEWORK_ERROR) {
		trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
		delete pExtAclPolicyMO;
		pGlobalExtIpAclRuleMessage->setCompletionStatus(IP_ACL_INTERNAL_ERROR);
		reply(pGlobalExtIpAclRuleMessage);
		return;
	} else if (locVector.size() == 0) {
		/*In this case, this particular policy is not enabled in any of the Mgmt I/F instances. */
		delete pExtAclPolicyMO;
		processExtIp6AclRules(pGlobalExtIpAclRuleMessage);
		return;
	}
	//TODO: Need to cleanup and optimize the code. Rather than reading twice from the DB data should be read only once	
	AAALocalExtIpAclRuleMessage *pMessage;

	if (pGlobalExtIpAclRuleMessage->getOpType() == AAA_CHANGE_OPER) {
		ExtIp6AclRuleManagedObject *pExtAclRuleMO = NULL;
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIp6AclRuleManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeUI64 (pGlobalExtIpAclRuleMessage->getSeqNum(), "seqNum"));
		pSyncQueryCtx->addAndAttribute (new AttributeObjectId (pExtAclPolicyMO->getObjectId(), "ownerManagedObjectId"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults || pResults->size() != 1)
		{
			trace (TRACE_LEVEL_ERROR, "Rule does not exist");
			if (pResults != NULL)
				delete pResults;
			delete pSyncQueryCtx;
			delete pExtAclPolicyMO;
			status = IP_ACL_RULE_DOESNT_EXIST;
			pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
			reply(pGlobalExtIpAclRuleMessage);
			return;
		}
		else
		{
			pExtAclRuleMO = dynamic_cast<ExtIp6AclRuleManagedObject *>((*pResults)[0]);
		}
		delete pResults;
		delete pSyncQueryCtx;

		IpAclAction action = pGlobalExtIpAclRuleMessage->getAction();
		IpAddrType srcIpType = pGlobalExtIpAclRuleMessage->getSrcIpType();
		string srcHostAddr = pGlobalExtIpAclRuleMessage->getSrcHost();
		AclProtocol protocol = pGlobalExtIpAclRuleMessage->getProtocol();
		//IPAddrType destIpType = pGlobalExtIpAclRuleMessage->getDestIpType();
		AclPortType destPortType = pGlobalExtIpAclRuleMessage->getDestPortType();
		UI32 destPort = pGlobalExtIpAclRuleMessage->getDestPort();
		UI32 destEndPort = pGlobalExtIpAclRuleMessage->getDestEndPort();

		/*  */
		if (action == ACL_ACTION_UNDEFINED)
		{
			action = pExtAclRuleMO->getAction();
		}
		if (protocol == ACL_PROTO_UNDEFINED)
		{
			protocol = pExtAclRuleMO->getProtocol();
		}

		if (srcIpType == IPV4_ADDR_UNDEFINED)
		{
			srcIpType = pExtAclRuleMO->getSrcIpType();
			srcHostAddr = pExtAclRuleMO->getSrcHost();
		} else if (srcIpType == IPV4_ADDR_ANY) {
			srcHostAddr = "";
		}
		IpAddrType destIpType = IPV4_ADDR_ANY;
		if (destPortType == ACL_PORT_UNDEFINED) {
			destPortType = pExtAclRuleMO->getDestPortType();
			if (destPortType == ACL_PORT_RANGE) {
				if (destPort == IP_ACL_NO_PORT_VAL) {
					destPort = pExtAclRuleMO->getDestPort();		
				}
				if (destEndPort == IP_ACL_NO_PORT_VAL) {
					destEndPort = pExtAclRuleMO->getDestEndPort();		
				}
			} else if (destPortType == ACL_PORT_EQ) {
				if (destPort == IP_ACL_NO_PORT_VAL) {
					destPort = pExtAclRuleMO->getDestPort();		
				}
			}
		} else if (destPortType == ACL_PORT_RANGE) {
			if (destPort == IP_ACL_NO_PORT_VAL) {
				destPort = pExtAclRuleMO->getDestPort();		
				if (destPort == IP_ACL_NO_PORT_VAL) {
					delete pExtAclPolicyMO;
					delete pExtAclRuleMO;
					status = IP_ACL_INVALID_DESTPORT_RANGE;
					pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
					reply(pGlobalExtIpAclRuleMessage);
					return;
				}
			}
		} else if (destPortType == ACL_PORT_EQ) {
			if (destPort == IP_ACL_NO_PORT_VAL) {
				destPort = pExtAclRuleMO->getDestPort();
				if (destPort == IP_ACL_NO_PORT_VAL) {
					delete pExtAclPolicyMO;
					delete pExtAclRuleMO;
					status = IP_ACL_INVALID_DESTPORT;
					pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
					reply(pGlobalExtIpAclRuleMessage);
					return;
				}
			}
		}

		if (destPortType == ACL_PORT_RANGE) {
			if (destEndPort < destPort) {
				delete pExtAclPolicyMO;
				delete pExtAclRuleMO;
				status = IP_ACL_INVALID_DESTPORT_RANGE;
				pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
				reply(pGlobalExtIpAclRuleMessage);
				return;
			}
		}
		delete pExtAclRuleMO;

		pMessage = new AAALocalExtIpAclRuleMessage (pGlobalExtIpAclRuleMessage->getPolicyName(), pGlobalExtIpAclRuleMessage->getSeqNum(), action, srcHostAddr, srcIpType, ACL_IPV6, protocol, destIpType, destPortType, destPort, destEndPort, pGlobalExtIpAclRuleMessage->getOpType());
	} else {	
		pMessage = new AAALocalExtIpAclRuleMessage (pGlobalExtIpAclRuleMessage->getPolicyName(), pGlobalExtIpAclRuleMessage->getSeqNum(), pGlobalExtIpAclRuleMessage->getAction(), pGlobalExtIpAclRuleMessage->getSrcHost(), pGlobalExtIpAclRuleMessage->getSrcIpType(), ACL_IPV6, pGlobalExtIpAclRuleMessage->getProtocol(), pGlobalExtIpAclRuleMessage->getDestIpType(), pGlobalExtIpAclRuleMessage->getDestPortType(), pGlobalExtIpAclRuleMessage->getDestPort(), pGlobalExtIpAclRuleMessage->getDestEndPort(), pGlobalExtIpAclRuleMessage->getOpType());
	}
	delete pExtAclPolicyMO;
	AAAExtAclRuleContext *pAclRuleContext = new AAAExtAclRuleContext (pGlobalExtIpAclRuleMessage);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalIpAclWorker::extAclRuleSTCCallback), pAclRuleContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	pWaveSendToClusterContext->setLocationsToSendToForPhase1(locVector);
	sendToWaveCluster (pWaveSendToClusterContext);
	trace (TRACE_LEVEL_INFO, "Exit GlobalIpAclWorker::AAAGlobalExtIp6AclRuleMessageHandler()");
}

void GlobalIpAclWorker::AAAGlobalExtIpAclRuleMessageHandler (AAAGlobalExtIpAclRuleMessage *pGlobalExtIpAclRuleMessage)
{
	trace (TRACE_LEVEL_INFO, "Enter GlobalIpAclWorker::AAAGlobalExtIpAclRuleMessageHandler()");
	prismAssert(pGlobalExtIpAclRuleMessage != NULL, __FILE__, __LINE__);

	ExtIpAclPolicyManagedObject *pExtAclPolicyMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	/* Get the MO instance of the Ext Acl Policy */
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIpAclPolicyManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (pGlobalExtIpAclRuleMessage->getPolicyName(), "aclname"));
	pSyncQueryCtx->setLoadOneToManyRelationships(false);
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults || pResults->size() != 1)
	{
		trace (TRACE_LEVEL_ERROR, "Policy does not exist");
		if (pResults != NULL)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		delete pSyncQueryCtx;
		status = IP_ACL_DOESNT_EXIST;
		pGlobalExtIpAclRuleMessage->setCompletionStatus(IP_ACL_DOESNT_EXIST);
		reply(pGlobalExtIpAclRuleMessage);
		return;
	}
	else
	{
		pExtAclPolicyMO = dynamic_cast<ExtIpAclPolicyManagedObject *>((*pResults)[0]);
	}
	delete pResults;
	delete pSyncQueryCtx;
	
	//Validating the range/eq when adding a new rule.
	if (pGlobalExtIpAclRuleMessage->getOpType() == AAA_ADD_OPER) {
		if (pGlobalExtIpAclRuleMessage->getDestPortType() == ACL_PORT_RANGE) {
			if ((pGlobalExtIpAclRuleMessage->getDestPort() == IP_ACL_NO_PORT_VAL) ||
				(pGlobalExtIpAclRuleMessage->getDestEndPort() == IP_ACL_NO_PORT_VAL) ||	
				pGlobalExtIpAclRuleMessage->getDestEndPort() < pGlobalExtIpAclRuleMessage->getDestPort()) {
				delete pExtAclPolicyMO;
				status = IP_ACL_INVALID_DESTPORT_RANGE;
				pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
				reply(pGlobalExtIpAclRuleMessage);
				return;
			}
		} else if (pGlobalExtIpAclRuleMessage->getDestPortType() == ACL_PORT_EQ) {
			if (pGlobalExtIpAclRuleMessage->getDestPort() == IP_ACL_NO_PORT_VAL) {
				delete pExtAclPolicyMO;
				status = IP_ACL_INVALID_DESTPORT;
				pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
				reply(pGlobalExtIpAclRuleMessage);
				return;
			}
		}
	}
	// Send messages to all local services
	vector <LocationId> locVector;
	string policyName = pGlobalExtIpAclRuleMessage->getPolicyName();
	int ret = getPolicyEnforcedSwitches(policyName, locVector, pGlobalExtIpAclRuleMessage->getAclVersion());
	if (ret == FRAMEWORK_ERROR) {
		trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
		delete pExtAclPolicyMO;
		pGlobalExtIpAclRuleMessage->setCompletionStatus(IP_ACL_INTERNAL_ERROR);
		reply(pGlobalExtIpAclRuleMessage);
		return;
	} else if (locVector.size() == 0) {
		/*In this case, this particular policy is not enabled in any of the Mgmt I/F instances. */
		processExtAclRules(pGlobalExtIpAclRuleMessage);
		delete pExtAclPolicyMO;
		return;
	}
	//TODO: Need to cleanup and optimize the code. Rather than reading twice from the DB data should be read only once	
	AAALocalExtIpAclRuleMessage *pMessage;

	if (pGlobalExtIpAclRuleMessage->getOpType() == AAA_CHANGE_OPER) {
		ExtIpAclRuleManagedObject *pExtAclRuleMO = NULL;
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIpAclRuleManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeUI64 (pGlobalExtIpAclRuleMessage->getSeqNum(), "seqNum"));
		pSyncQueryCtx->addAndAttribute (new AttributeObjectId (pExtAclPolicyMO->getObjectId(), "ownerManagedObjectId"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults || pResults->size() != 1)
		{
			trace (TRACE_LEVEL_ERROR, "Rule does not exist");
			if (pResults != NULL)
				delete pResults;
			delete pSyncQueryCtx;
			delete pExtAclPolicyMO;
			status = IP_ACL_RULE_DOESNT_EXIST;
			pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
			reply(pGlobalExtIpAclRuleMessage);
			return;
		}
		else
		{
			pExtAclRuleMO = dynamic_cast<ExtIpAclRuleManagedObject *>((*pResults)[0]);
			if (NULL == pExtAclRuleMO)
			{
		delete pSyncQueryCtx;
				delete pExtAclPolicyMO;
				pGlobalExtIpAclRuleMessage->setCompletionStatus (IP_ACL_RULE_DOESNT_EXIST);
				if (pResults != NULL) 
					delete pResults;
				reply (pGlobalExtIpAclRuleMessage);
				return;
			}
		}
		delete pResults;
		delete pSyncQueryCtx;

		IpAclAction action = pGlobalExtIpAclRuleMessage->getAction();
		IpAddrType srcIpType = pGlobalExtIpAclRuleMessage->getSrcIpType();
		string srcHostAddr = pGlobalExtIpAclRuleMessage->getSrcHost();
		AclProtocol protocol = pGlobalExtIpAclRuleMessage->getProtocol();
		//IPAddrType destIpType = pGlobalExtIpAclRuleMessage->getDestIpType();
		AclPortType destPortType = pGlobalExtIpAclRuleMessage->getDestPortType();
		UI32 destPort = pGlobalExtIpAclRuleMessage->getDestPort();
		UI32 destEndPort = pGlobalExtIpAclRuleMessage->getDestEndPort();

		/*  */
		if (action == ACL_ACTION_UNDEFINED)
		{
			action = pExtAclRuleMO->getAction();
		}
		if (protocol == ACL_PROTO_UNDEFINED)
		{
			protocol = pExtAclRuleMO->getProtocol();
		}

		if (srcIpType == IPV4_ADDR_UNDEFINED)
		{
			srcIpType = pExtAclRuleMO->getSrcIpType();
			srcHostAddr = pExtAclRuleMO->getSrcHost();
		} else if (srcIpType == IPV4_ADDR_ANY) {
			srcHostAddr = "";
		}
		IpAddrType destIpType = IPV4_ADDR_ANY;
		if (destPortType == ACL_PORT_UNDEFINED) {
			destPortType = pExtAclRuleMO->getDestPortType();
			if (destPortType == ACL_PORT_RANGE) {
				if (destPort == IP_ACL_NO_PORT_VAL) {
					destPort = pExtAclRuleMO->getDestPort();		
				}
				if (destEndPort == IP_ACL_NO_PORT_VAL) {
					destEndPort = pExtAclRuleMO->getDestEndPort();		
				}
			} else if (destPortType == ACL_PORT_EQ) {
				if (destPort == IP_ACL_NO_PORT_VAL) {
					destPort = pExtAclRuleMO->getDestPort();		
				}
			}
		} else if (destPortType == ACL_PORT_RANGE) {
			if (destPort == IP_ACL_NO_PORT_VAL) {
				destPort = pExtAclRuleMO->getDestPort();		
				if (destPort == IP_ACL_NO_PORT_VAL) {
					delete pExtAclPolicyMO;
					delete pExtAclRuleMO;
					status = IP_ACL_INVALID_DESTPORT_RANGE;
					pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
					reply(pGlobalExtIpAclRuleMessage);
					return;
				}
			}
		} else if (destPortType == ACL_PORT_EQ) {
			if (destPort == IP_ACL_NO_PORT_VAL) {
				destPort = pExtAclRuleMO->getDestPort();
				if (destPort == IP_ACL_NO_PORT_VAL) {
					delete pExtAclPolicyMO;
					delete pExtAclRuleMO;
					status = IP_ACL_INVALID_DESTPORT;
					pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
					reply(pGlobalExtIpAclRuleMessage);
					return;
				}
			}
		}

		if (destPortType == ACL_PORT_RANGE) {
			if (destEndPort < destPort) {
				delete pExtAclPolicyMO;
				delete pExtAclRuleMO;
				status = IP_ACL_INVALID_DESTPORT_RANGE;
				pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
				reply(pGlobalExtIpAclRuleMessage);
				return;
			}
		}
		delete pExtAclRuleMO;

		pMessage = new AAALocalExtIpAclRuleMessage (pGlobalExtIpAclRuleMessage->getPolicyName(), pGlobalExtIpAclRuleMessage->getSeqNum(), action, srcHostAddr, srcIpType, ACL_IPV4, protocol, destIpType, destPortType, destPort, destEndPort, pGlobalExtIpAclRuleMessage->getOpType());
	} else {	
		pMessage = new AAALocalExtIpAclRuleMessage (pGlobalExtIpAclRuleMessage->getPolicyName(), pGlobalExtIpAclRuleMessage->getSeqNum(), pGlobalExtIpAclRuleMessage->getAction(), pGlobalExtIpAclRuleMessage->getSrcHost(), pGlobalExtIpAclRuleMessage->getSrcIpType(), ACL_IPV4, pGlobalExtIpAclRuleMessage->getProtocol(), pGlobalExtIpAclRuleMessage->getDestIpType(), pGlobalExtIpAclRuleMessage->getDestPortType(), pGlobalExtIpAclRuleMessage->getDestPort(), pGlobalExtIpAclRuleMessage->getDestEndPort(), pGlobalExtIpAclRuleMessage->getOpType());
	}
	delete pExtAclPolicyMO;
	AAAExtAclRuleContext *pAclRuleContext = new AAAExtAclRuleContext (pGlobalExtIpAclRuleMessage);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalIpAclWorker::extAclRuleSTCCallback), pAclRuleContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	pWaveSendToClusterContext->setLocationsToSendToForPhase1(locVector);
	sendToWaveCluster (pWaveSendToClusterContext);
	trace (TRACE_LEVEL_INFO, "Exit GlobalIpAclWorker::AAAGlobalExtIpAclRuleMessageHandler()");
}

void GlobalIpAclWorker::processExtIp6AclRules(AAAGlobalExtIpAclRuleMessage *pGlobalExtIpAclRuleMessage)
{
	trace (TRACE_LEVEL_INFO, "GlobalIpAclWorker::processExtIp6AclRules() Enter");
	ExtIp6AclPolicyManagedObject *pExtAclPolicyMO = NULL;
	ExtIp6AclRuleManagedObject *pExtAclRuleMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	string policyName = pGlobalExtIpAclRuleMessage->getPolicyName();
	UI64 sequenceNumber = pGlobalExtIpAclRuleMessage->getSeqNum();
	IpAclAction action = pGlobalExtIpAclRuleMessage->getAction();
	IpAddrType srcIpType = pGlobalExtIpAclRuleMessage->getSrcIpType();
	string srcHostAddr = pGlobalExtIpAclRuleMessage->getSrcHost();
	AclProtocol protocol = pGlobalExtIpAclRuleMessage->getProtocol();
	IpAddrType destIpType = pGlobalExtIpAclRuleMessage->getDestIpType();
	AclPortType destPortType = pGlobalExtIpAclRuleMessage->getDestPortType();
	UI32 destPort = pGlobalExtIpAclRuleMessage->getDestPort();
	UI32 destEndPort = pGlobalExtIpAclRuleMessage->getDestEndPort();
	AAAGlobalMessageOperationType operationType = pGlobalExtIpAclRuleMessage->getOpType();
	
	/* Get the MO instance of the Ext Acl Policy */
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIp6AclPolicyManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (policyName, "aclname"));
	pSyncQueryCtx->setLoadOneToManyRelationships(false);
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults || pResults->size() != 1)
	{
		trace (TRACE_LEVEL_ERROR, "Policy does not exist");
		if (pResults != NULL)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		delete pSyncQueryCtx;
		status = IP_ACL_DOESNT_EXIST;
		pGlobalExtIpAclRuleMessage->setCompletionStatus(IP_ACL_DOESNT_EXIST);
		reply(pGlobalExtIpAclRuleMessage);
		return;
	}
	else
	{
		pExtAclPolicyMO = dynamic_cast<ExtIp6AclPolicyManagedObject *>((*pResults)[0]);
	}
	delete pResults;
	delete pSyncQueryCtx;

	vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > extRuleMoVector;
	/* For CHANGE or DELETE operation, get the MO instance of the rule */
	if (AAA_CHANGE_OPER == operationType || AAA_DELETE_OPER == operationType)
	{
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIp6AclRuleManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeUI64 (sequenceNumber, "seqNum"));
		pSyncQueryCtx->addAndAttribute (new AttributeObjectId (pExtAclPolicyMO->getObjectId(), "ownerManagedObjectId"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults || pResults->size() != 1)
		{
			trace (TRACE_LEVEL_ERROR, "Rule does not exist");
			if (pResults != NULL)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			delete pSyncQueryCtx;
			delete pExtAclPolicyMO;
			status = IP_ACL_RULE_DOESNT_EXIST;
			pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
			reply(pGlobalExtIpAclRuleMessage);
			return;
		}
		else
		{
			pExtAclRuleMO = dynamic_cast<ExtIp6AclRuleManagedObject *>((*pResults)[0]);
		}
		delete pResults;
		delete pSyncQueryCtx;
	}

	/* Make changes to DCM database */
	if (AAA_DELETE_OPER == operationType)
	{
		startTransaction();
		trace (TRACE_LEVEL_INFO, string("Deleting the standard IP ACL rule#") + sequenceNumber + string(" from the database ..."));
		pExtAclPolicyMO->deleteRule(sequenceNumber);
		deleteFromComposition(ExtIp6AclPolicyManagedObject::getClassName(), ExtIp6AclRuleManagedObject::getClassName(), "extRules", pExtAclPolicyMO->getObjectId(), pExtAclRuleMO->getObjectId());
		delete pExtAclRuleMO;
		status = commitTransaction();
	}
	else
	{
		if (AAA_ADD_OPER == operationType)
		{
			trace (TRACE_LEVEL_INFO, string("Creating a new standard IP ACL rule#") + sequenceNumber + string(" in the database ..."));

			startTransaction();
			pExtAclRuleMO = dynamic_cast<ExtIp6AclRuleManagedObject *>(createManagedObjectInstance(ExtIp6AclRuleManagedObject::getClassName()));
			pExtAclRuleMO->setOwnerManagedObjectId(pExtAclPolicyMO->getObjectId());

			addToComposition(ExtIp6AclPolicyManagedObject::getClassName(), ExtIp6AclRuleManagedObject::getClassName(), "extRules", pExtAclPolicyMO->getObjectId(), pExtAclRuleMO->getObjectId());

			pExtAclRuleMO->setSeqNum(sequenceNumber);
			//pExtAclPolicyMO->addRule(pExtAclRuleMO);
		}
		else if (AAA_CHANGE_OPER == operationType)
		{
			/*  */
			if (action == ACL_ACTION_UNDEFINED)
			{
				action = pExtAclRuleMO->getAction();
			}
			if (protocol == ACL_PROTO_UNDEFINED)
			{
				protocol = pExtAclRuleMO->getProtocol();
			}

			if (srcIpType == IPV4_ADDR_UNDEFINED)
			{
				srcIpType = pExtAclRuleMO->getSrcIpType();
				srcHostAddr = pExtAclRuleMO->getSrcHost();
			} else if (srcIpType == IPV4_ADDR_ANY) {
				srcHostAddr = "";
			}
			destIpType = IPV4_ADDR_ANY;
			if (destPortType == ACL_PORT_UNDEFINED) {
				destPortType = pExtAclRuleMO->getDestPortType();
				if (destPortType == ACL_PORT_RANGE) {
					if (destPort == IP_ACL_NO_PORT_VAL) {
						destPort = pExtAclRuleMO->getDestPort();		
					}
					if (destEndPort == IP_ACL_NO_PORT_VAL) {
						destEndPort = pExtAclRuleMO->getDestEndPort();		
					}
				} else if (destPortType == ACL_PORT_EQ) {
					if (destPort == IP_ACL_NO_PORT_VAL) {
						destPort = pExtAclRuleMO->getDestPort();		
					}
				}
			} else if (destPortType == ACL_PORT_RANGE) {
				if (destPort == IP_ACL_NO_PORT_VAL) {
					destPort = pExtAclRuleMO->getDestPort();
					if (destPort == IP_ACL_NO_PORT_VAL) {
						delete pExtAclPolicyMO;
						delete pExtAclRuleMO;
						status = IP_ACL_INVALID_DESTPORT_RANGE;
						pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
						reply(pGlobalExtIpAclRuleMessage);
						return;
					}
				}
			} else if (destPortType == ACL_PORT_EQ) {
				if (destPort == IP_ACL_NO_PORT_VAL) {
					destPort = pExtAclRuleMO->getDestPort();
					if (destPort == IP_ACL_NO_PORT_VAL) {
						delete pExtAclPolicyMO;
						delete pExtAclRuleMO;
						status = IP_ACL_INVALID_DESTPORT;
						pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
						reply(pGlobalExtIpAclRuleMessage);
						return;
					}
				}
			}
			if (destPortType == ACL_PORT_RANGE) {
				if (destEndPort < destPort) {
					delete pExtAclPolicyMO;
					delete pExtAclRuleMO;
					status = IP_ACL_INVALID_DESTPORT_RANGE;
					pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
					reply(pGlobalExtIpAclRuleMessage);
					return;
				}
			}
			startTransaction();
			updateWaveManagedObject(pExtAclRuleMO);
		}

		trace (TRACE_LEVEL_INFO, string("Creating a destination port ") + destPort + string("destEndPort ") + destEndPort);
		pExtAclRuleMO->setAction((IpAclAction)action);
		pExtAclRuleMO->setSrcHost(srcHostAddr);
		pExtAclRuleMO->setSrcIpType(srcIpType);
		pExtAclRuleMO->setProtocol((AclProtocol)protocol);
		pExtAclRuleMO->setDestIpType(destIpType);
		pExtAclRuleMO->setDestPortType(destPortType);
		//if (destPort != IP_ACL_NO_PORT_VAL) {
		pExtAclRuleMO->setDestPort(destPort);
		//}
		//if (destEndPort != IP_ACL_NO_PORT_VAL) {
		pExtAclRuleMO->setDestEndPort(destEndPort);
		//}

		status = commitTransaction();
		delete pExtAclRuleMO;
	}
	if (status == FRAMEWORK_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "Making changes for the rule in the DB is successful.");
		status = WAVE_MESSAGE_SUCCESS;
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Making changes for the rule in the database FAILED!!.");
	}

	delete pExtAclPolicyMO;
	pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
	reply(pGlobalExtIpAclRuleMessage);
	trace (TRACE_LEVEL_INFO, "GlobalIpAclWorker::processExtIp6AclRules() Exit");
}
void GlobalIpAclWorker::processExtAclRules(AAAGlobalExtIpAclRuleMessage *pGlobalExtIpAclRuleMessage)
{
	trace (TRACE_LEVEL_INFO, "GlobalIpAclWorker::processExtAclRules() Enter");
	ExtIpAclPolicyManagedObject *pExtAclPolicyMO = NULL;
	ExtIpAclRuleManagedObject *pExtAclRuleMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	string policyName = pGlobalExtIpAclRuleMessage->getPolicyName();
	UI64 sequenceNumber = pGlobalExtIpAclRuleMessage->getSeqNum();
	IpAclAction action = pGlobalExtIpAclRuleMessage->getAction();
	IpAddrType srcIpType = pGlobalExtIpAclRuleMessage->getSrcIpType();
	string srcHostAddr = pGlobalExtIpAclRuleMessage->getSrcHost();
	AclProtocol protocol = pGlobalExtIpAclRuleMessage->getProtocol();
	IpAddrType destIpType = pGlobalExtIpAclRuleMessage->getDestIpType();
	AclPortType destPortType = pGlobalExtIpAclRuleMessage->getDestPortType();
	UI32 destPort = pGlobalExtIpAclRuleMessage->getDestPort();
	UI32 destEndPort = pGlobalExtIpAclRuleMessage->getDestEndPort();
	AAAGlobalMessageOperationType operationType = pGlobalExtIpAclRuleMessage->getOpType();
	
	/* Get the MO instance of the Ext Acl Policy */
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIpAclPolicyManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString (policyName, "aclname"));
	pSyncQueryCtx->setLoadOneToManyRelationships(false);
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults || pResults->size() != 1)
	{
		trace (TRACE_LEVEL_ERROR, "Policy does not exist");
		if (pResults != NULL)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		delete pSyncQueryCtx;
		status = IP_ACL_DOESNT_EXIST;
		pGlobalExtIpAclRuleMessage->setCompletionStatus(IP_ACL_DOESNT_EXIST);
		reply(pGlobalExtIpAclRuleMessage);
		return;
	}
	else
	{
		pExtAclPolicyMO = dynamic_cast<ExtIpAclPolicyManagedObject *>((*pResults)[0]);
	}
	delete pResults;
	delete pSyncQueryCtx;

	vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> > extRuleMoVector;
	/* For CHANGE or DELETE operation, get the MO instance of the rule */
	if (AAA_CHANGE_OPER == operationType || AAA_DELETE_OPER == operationType)
	{
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIpAclRuleManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeUI64 (sequenceNumber, "seqNum"));
		pSyncQueryCtx->addAndAttribute (new AttributeObjectId (pExtAclPolicyMO->getObjectId(), "ownerManagedObjectId"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults || pResults->size() != 1)
		{
			trace (TRACE_LEVEL_ERROR, "Rule does not exist");
			if (pResults != NULL)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			delete pSyncQueryCtx;
			delete pExtAclPolicyMO;
			status = IP_ACL_RULE_DOESNT_EXIST;
			pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
			reply(pGlobalExtIpAclRuleMessage);
			return;
		}
		else
		{
			pExtAclRuleMO = dynamic_cast<ExtIpAclRuleManagedObject *>((*pResults)[0]);
			if (NULL == pExtAclRuleMO)
			{
				delete pSyncQueryCtx;
				delete pExtAclPolicyMO;
				pGlobalExtIpAclRuleMessage->setCompletionStatus (IP_ACL_RULE_DOESNT_EXIST);
				if (pResults != NULL)
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				reply (pGlobalExtIpAclRuleMessage);
				return;
			}
		}
		delete pResults;
		delete pSyncQueryCtx;
	}

	/* Make changes to DCM database */
	if (AAA_DELETE_OPER == operationType)
	{
		startTransaction();
		trace (TRACE_LEVEL_INFO, string("Deleting the standard IP ACL rule#") + sequenceNumber + string(" from the database ..."));
		pExtAclPolicyMO->deleteRule(sequenceNumber);
		deleteFromComposition(ExtIpAclPolicyManagedObject::getClassName(), ExtIpAclRuleManagedObject::getClassName(), "extRules", pExtAclPolicyMO->getObjectId(), pExtAclRuleMO->getObjectId());
		delete pExtAclRuleMO;
		status = commitTransaction();
	}
	else
	{
		if (AAA_ADD_OPER == operationType)
		{
			trace (TRACE_LEVEL_INFO, string("Creating a new standard IP ACL rule#") + sequenceNumber + string(" in the database ..."));

			startTransaction();
			pExtAclRuleMO = dynamic_cast<ExtIpAclRuleManagedObject *>(createManagedObjectInstance(ExtIpAclRuleManagedObject::getClassName()));
			pExtAclRuleMO->setOwnerManagedObjectId(pExtAclPolicyMO->getObjectId());

			addToComposition(ExtIpAclPolicyManagedObject::getClassName(), ExtIpAclRuleManagedObject::getClassName(), "extRules", pExtAclPolicyMO->getObjectId(), pExtAclRuleMO->getObjectId());

			pExtAclRuleMO->setSeqNum(sequenceNumber);
			//pExtAclPolicyMO->addRule(pExtAclRuleMO);
		}
		else if (AAA_CHANGE_OPER == operationType)
		{
			/*  */
			if (action == ACL_ACTION_UNDEFINED)
			{
				action = pExtAclRuleMO->getAction();
			}
			if (protocol == ACL_PROTO_UNDEFINED)
			{
				protocol = pExtAclRuleMO->getProtocol();
			}

			if (srcIpType == IPV4_ADDR_UNDEFINED)
			{
				srcIpType = pExtAclRuleMO->getSrcIpType();
				srcHostAddr = pExtAclRuleMO->getSrcHost();
			} else if (srcIpType == IPV4_ADDR_ANY) {
				srcHostAddr = "";
			}
			destIpType = IPV4_ADDR_ANY;
			if (destPortType == ACL_PORT_UNDEFINED) {
				destPortType = pExtAclRuleMO->getDestPortType();
				if (destPortType == ACL_PORT_RANGE) {
					if (destPort == IP_ACL_NO_PORT_VAL) {
						destPort = pExtAclRuleMO->getDestPort();		
					}
					if (destEndPort == IP_ACL_NO_PORT_VAL) {
						destEndPort = pExtAclRuleMO->getDestEndPort();		
					}
				} else if (destPortType == ACL_PORT_EQ) {
					if (destPort == IP_ACL_NO_PORT_VAL) {
						destPort = pExtAclRuleMO->getDestPort();		
					}
				}
			} else if (destPortType == ACL_PORT_RANGE) {
				if (destPort == IP_ACL_NO_PORT_VAL) {
					destPort = pExtAclRuleMO->getDestPort();
					if (destPort == IP_ACL_NO_PORT_VAL) {
						delete pExtAclPolicyMO;
						delete pExtAclRuleMO;
						status = IP_ACL_INVALID_DESTPORT_RANGE;
						pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
						reply(pGlobalExtIpAclRuleMessage);
						return;
					}
				}
			} else if (destPortType == ACL_PORT_EQ) {
				if (destPort == IP_ACL_NO_PORT_VAL) {
					destPort = pExtAclRuleMO->getDestPort();
					if (destPort == IP_ACL_NO_PORT_VAL) {
						delete pExtAclPolicyMO;
						delete pExtAclRuleMO;
						status = IP_ACL_INVALID_DESTPORT;
						pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
						reply(pGlobalExtIpAclRuleMessage);
						return;
					}
				}
			}
			if (destPortType == ACL_PORT_RANGE) {
				if (destEndPort < destPort) {
					delete pExtAclPolicyMO;
					delete pExtAclRuleMO;
					status = IP_ACL_INVALID_DESTPORT_RANGE;
					pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
					reply(pGlobalExtIpAclRuleMessage);
					return;
				}
			}
			startTransaction();
			updateWaveManagedObject(pExtAclRuleMO);
		}

		trace (TRACE_LEVEL_INFO, string("Creating a destination port ") + destPort + string("destEndPort ") + destEndPort);
		pExtAclRuleMO->setAction((IpAclAction)action);
		pExtAclRuleMO->setSrcHost(srcHostAddr);
		pExtAclRuleMO->setSrcIpType(srcIpType);
		pExtAclRuleMO->setProtocol((AclProtocol)protocol);
		pExtAclRuleMO->setDestIpType(destIpType);
		pExtAclRuleMO->setDestPortType(destPortType);
		//if (destPort != IP_ACL_NO_PORT_VAL) {
			pExtAclRuleMO->setDestPort(destPort);
		//}
		//if (destEndPort != IP_ACL_NO_PORT_VAL) {
			pExtAclRuleMO->setDestEndPort(destEndPort);
		//}
		status = commitTransaction();
		delete pExtAclRuleMO;
	}
	if (status == FRAMEWORK_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "Making changes for the rule in the DB is successful.");
		status = WAVE_MESSAGE_SUCCESS;
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Making changes for the rule in the database FAILED!!.");
	}

	delete pExtAclPolicyMO;
	pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
	reply(pGlobalExtIpAclRuleMessage);
	trace (TRACE_LEVEL_INFO, "GlobalIpAclWorker::processExtAclRules() Exit");
}

void GlobalIpAclWorker::extAclRuleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalIpAclWorker::extAclRuleSTCCallback() Enter");
	AAAExtAclRuleContext *pAAAContext = (AAAExtAclRuleContext *)pWaveSendToClusterContext->getPCallerContext();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalExtIpAclRuleMessage *pGlobalExtIpAclRuleMessage = (AAAGlobalExtIpAclRuleMessage *)pAAAContext->getPPrismMessage();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
	status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Change user operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Change user operation failed on all nodes");

		vector<LocationId> cxLocations;
		FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalIpAclWorker::extAclRuleSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? USER_CHANGE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? USER_CHANGE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		if (pGlobalExtIpAclRuleMessage->getAclVersion() == ACL_IPV4)	 {
			processExtAclRules(pGlobalExtIpAclRuleMessage);
		} else {
			processExtIp6AclRules(pGlobalExtIpAclRuleMessage);
		}
	} else {
		pGlobalExtIpAclRuleMessage->setCompletionStatus(status);
		reply(pGlobalExtIpAclRuleMessage);
	}
	
	trace (TRACE_LEVEL_INFO, "GlobalIpAclWorker::extAclRuleSTCCallback() Exit");
}
}
#endif
