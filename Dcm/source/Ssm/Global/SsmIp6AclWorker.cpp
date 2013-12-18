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

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmIp6AclWorker.h"
#include "Ssm/Global/SsmGlobalIp6AclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalRemIpAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalAssocIp6RuleToPolicyMessage.h"
#include "Ssm/Global/SsmGlobalReseqIpAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalGetIpAclPolicyMessage.h"
#include "Ssm/Global/Ip6AclPolicyManagedObject.h"
#include "Ssm/Global/StdIp6AclPolicyManagedObject.h"
#include "Ssm/Global/ExtIp6AclPolicyManagedObject.h"
#include "Ssm/Global/StdIp6AclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Ssm/Local/SsmTypes.h"
#include "Ssm/Local/SsmLocalServiceAddIpAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceRemIpAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceAssocIpRuleToPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceResqIpAclPolicyMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "DcmCStatus.h"
#include "Nsm/Global/IntfManagedObject.h"
#include "Nsm/Global/NsmGlobalReAssocIpAclPolicyMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"

#include "Ssm/Global/MacAclPolicyManagedObject.h"
#include "Nsm/Local/Ssm/NsmLocalIngressEgressIpAclPolicyManagedObject.h"
#include <string>
#include <sstream>
#include <algorithm>
#include "Ssm/Global/SsmPlugUtils.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "AAA/Global/AAASequencerContexts.h"
#include "AAA/Local/IpAclLocalManagedObjects.h"
#include "AAA/Local/AAALocalIpAclMessages.h"
#include "AAA/Local/AAALocalAssocIp6RuleToPolicyMessage.h"
#include "AAA/Global/AAASequencerContexts.h"


using namespace WaveNs;

namespace DcmNs
{
SsmIp6AclWorker::SsmIp6AclWorker (SsmGlobalObjectManager *pSsmGlobalObjectManager)
	: WaveWorker (pSsmGlobalObjectManager)
{
	Ip6AclPolicyManagedObject ip6AclPolicyMO (pSsmGlobalObjectManager);
	ip6AclPolicyMO.setupOrm ();
	addManagedClass (Ip6AclPolicyManagedObject::getClassName (), this);

	StdIp6AclPolicyManagedObject stdIp6AclPolicyMO (pSsmGlobalObjectManager);
	stdIp6AclPolicyMO.setupOrm ();
	addManagedClass (StdIp6AclPolicyManagedObject::getClassName (), this);

	ExtIp6AclPolicyManagedObject extIp6AclPolicyMO (pSsmGlobalObjectManager);
	extIp6AclPolicyMO.setupOrm ();
	addManagedClass (ExtIp6AclPolicyManagedObject::getClassName (), this);

	Ip6AclRuleManagedObject ip6AclRuleMO (pSsmGlobalObjectManager);
	ip6AclRuleMO.setupOrm ();
	addManagedClass (Ip6AclRuleManagedObject::getClassName (), this);

	StdIp6AclRuleManagedObject stdIp6AclRuleMO (pSsmGlobalObjectManager);
	stdIp6AclRuleMO.setupOrm ();
	addManagedClass (StdIp6AclRuleManagedObject::getClassName (), this);

	ExtIp6AclRuleManagedObject extIp6AclRuleMO (pSsmGlobalObjectManager);
	extIp6AclRuleMO.setupOrm ();
	addManagedClass (ExtIp6AclRuleManagedObject::getClassName (), this);
	
	addOperationMap (SSMGLOBALIP6ACLPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmIp6AclWorker::SsmGlobalIp6AclPolicyMessageHandler));
	addOperationMap (SSMGLOBALASSOCIP6RULETOPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmIp6AclWorker::SsmGlobalAssocIp6RuleToPolicyMessageHandler));
}

SsmIp6AclWorker::~SsmIp6AclWorker ()
{
}

PrismMessage *SsmIp6AclWorker::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{
		case SSMGLOBALIP6ACLPOLICY :
			pPrismMessage = new SsmGlobalIp6AclPolicyMessage ();
			break;
		case SSMGLOBALASSOCIP6RULETOPOLICY :
			pPrismMessage = new SsmGlobalAssocIp6RuleToPolicyMessage();
			break;
		default :
			pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject *SsmIp6AclWorker::createManagedObjectInstance(const string &managedClassName)
{
	WaveManagedObject *pWaveManagedObject = NULL;

	if ((StdIp6AclRuleManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new StdIp6AclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((ExtIp6AclRuleManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new ExtIp6AclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((Ip6AclPolicyManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new Ip6AclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((StdIp6AclPolicyManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new StdIp6AclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((ExtIp6AclPolicyManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new ExtIp6AclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "SsmIp6AclWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
	}

	return (pWaveManagedObject);
}

void SsmIp6AclWorker::SsmGlobalIp6AclPolicyMessageHandler (SsmGlobalIp6AclPolicyMessage *pGlobalIpAclPolicyMessage)
{
	trace (TRACE_LEVEL_INFO, "Enter SsmIp6AclWorker::AAAGlobalIp6AclPolicyMessageHandler()");
	prismAssert(pGlobalIpAclPolicyMessage != NULL, __FILE__, __LINE__);

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	string name = pGlobalIpAclPolicyMessage->getName();
	if (IP6_ADD_OPER == pGlobalIpAclPolicyMessage->getOpType())
	{
		// We will check if IP/MAC ACL with the same name exists
		status = verifyIPMACPolicyexists(name);
		if (status != WAVE_MESSAGE_SUCCESS)
		{
			pGlobalIpAclPolicyMessage->setCompletionStatus(status);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		//Same policy name shouldn't exist in IPv6 standard and extended ACL list.	
		WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
		vector<WaveManagedObject *> *pResults = NULL;
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute(new AttributeString(name, "policyName"));
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
			trace (TRACE_LEVEL_ERROR, "IPv6 Policy Already Exists with the same name.");
			delete pSyncQueryCtx;
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			pGlobalIpAclPolicyMessage->setCompletionStatus(WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		delete pSyncQueryCtx;
		delete pResults;

		Ip6AclPolicyManagedObject *pAclPolicyObj = NULL;
		startTransaction();

		if (STANDARD_IP6_ACL == pGlobalIpAclPolicyMessage->getAclType())
		{
			pAclPolicyObj = dynamic_cast<StdIp6AclPolicyManagedObject *>(createManagedObjectInstance(StdIp6AclPolicyManagedObject::getClassName()));
		}
		else if (EXTENDED_IP6_ACL == pGlobalIpAclPolicyMessage->getAclType())
		{
			pAclPolicyObj = dynamic_cast<ExtIp6AclPolicyManagedObject *>(createManagedObjectInstance(ExtIp6AclPolicyManagedObject::getClassName()));
		}
		pAclPolicyObj->setPolicyName(pGlobalIpAclPolicyMessage->getName());
		status = commitTransaction();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Adding ACL policy to the DB is successful.");
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "DB Commit transaction failed." + FrameworkToolKit::localize (status));
			status = WAVE_MESSAGE_ERROR;
		}
		delete pAclPolicyObj;
	}
	else if (IP6_DELETE_OPER == pGlobalIpAclPolicyMessage->getOpType())
	{
		WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
		vector<WaveManagedObject *> *pResults = NULL;

		// Verify that this policy is not enforced, currently verify Mgmt intf.
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyLocalManagedObject::getClassName());
		pSyncQueryCtx->addAndAttribute (new AttributeString(name, "ip6aclpolicy"));
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults)
		{
			trace (TRACE_LEVEL_FATAL, "Ip6AclPolicyLocalManagedObject database failed");
			delete pSyncQueryCtx;
			pGlobalIpAclPolicyMessage->setCompletionStatus(FRAMEWORK_ERROR);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		else if (pResults->size() > 0)
		{
			trace (TRACE_LEVEL_INFO, string ("some of the switches are enforced with this policy") + name);
			delete pSyncQueryCtx;
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			pGlobalIpAclPolicyMessage->setCompletionStatus(WRC_SSM_DCM_ERR_IP_ACL_APPLIED);
			reply(pGlobalIpAclPolicyMessage);
			return;
		}
		delete pSyncQueryCtx;
		delete pResults;

		if (STANDARD_IP6_ACL == pGlobalIpAclPolicyMessage->getAclType())
		{
			pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIp6AclPolicyManagedObject::getClassName());
			pSyncQueryCtx->addAndAttribute (new AttributeString (name, "policyName"));
			pResults = querySynchronously (pSyncQueryCtx);
			delete pSyncQueryCtx;
			if (!pResults)
			{
				trace (TRACE_LEVEL_FATAL, "StdIp6AclPolicyManagedObject database failed");
				pGlobalIpAclPolicyMessage->setCompletionStatus(FRAMEWORK_ERROR);
				reply(pGlobalIpAclPolicyMessage);
				return;
			}
			else if (pResults->size () == 0)
			{
				trace (TRACE_LEVEL_FATAL, "StdIp6AclPolicyManagedObject - policy doesn't exist");
				delete pResults;
				pGlobalIpAclPolicyMessage->setCompletionStatus(FRAMEWORK_ERROR);
				reply(pGlobalIpAclPolicyMessage);
				return;
			}
			trace (TRACE_LEVEL_INFO, "its an ACL_TYPE_L3_IPV4_STD_E");
		}
		if (EXTENDED_IP6_ACL == pGlobalIpAclPolicyMessage->getAclType())
		{
			pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIp6AclPolicyManagedObject::getClassName());
			pSyncQueryCtx->addAndAttribute (new AttributeString (name, "policyName"));
			pResults = querySynchronously (pSyncQueryCtx);
			delete pSyncQueryCtx;
			if (!pResults)
			{
				trace (TRACE_LEVEL_FATAL, "ExtIp6AclPolicyManagedObject database failed");
				delete pSyncQueryCtx;
				pGlobalIpAclPolicyMessage->setCompletionStatus(FRAMEWORK_ERROR);
				reply(pGlobalIpAclPolicyMessage);
				return;
			}
			else if (pResults->size () == 0)
			{
				trace (TRACE_LEVEL_FATAL, "ExtIp6AclPolicyManagedObject- policy doesn't exist");
				delete pResults;
				pGlobalIpAclPolicyMessage->setCompletionStatus(FRAMEWORK_ERROR);
				reply(pGlobalIpAclPolicyMessage);
				return;
			}
			trace (TRACE_LEVEL_INFO, "its an ACL_TYPE_L3_IPV4_EXT_E");
		}

		StdIp6AclPolicyManagedObject *stdPolicyMO = NULL;
		ExtIp6AclPolicyManagedObject *extPolicyMO = NULL;
		UI64 iter = 0;
		UI64 seqnum = 0;
		if (STANDARD_IP6_ACL == pGlobalIpAclPolicyMessage->getAclType())
		{
//			pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIp6AclRuleManagedObject::getClassName());
			StdIp6AclRuleManagedObject *pStdAclRuleMO = NULL;

			stdPolicyMO = dynamic_cast<StdIp6AclPolicyManagedObject *>((*pResults)[0]);
			delete pResults;

			vector<WaveManagedObjectPointer<StdIp6AclRuleManagedObject> > stdRuleMoVector;
			stdPolicyMO->sortRules();
			stdRuleMoVector = stdPolicyMO->getRules();

			for (iter = 0; iter < stdRuleMoVector.size(); iter++)
			{
				seqnum = stdRuleMoVector[iter]->getRuleid();
				pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIp6AclRuleManagedObject::getClassName());
				pSyncQueryCtx->addAndAttribute (new AttributeUI64 (seqnum, "ruleid"));
				pSyncQueryCtx->addAndAttribute (new AttributeObjectId (stdPolicyMO->getObjectId(), "ownerManagedObjectId"));
				pResults = querySynchronously (pSyncQueryCtx);
				delete pSyncQueryCtx;
				if (!pResults || pResults->size() != 1)
				{
					trace (TRACE_LEVEL_ERROR, "Rule does not exist");
					if (pResults != NULL)
						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

					delete stdPolicyMO;
					pGlobalIpAclPolicyMessage->setCompletionStatus(IP_ACL_RULE_DOESNT_EXIST);
					reply(pGlobalIpAclPolicyMessage);
					return;
				}
				pStdAclRuleMO = dynamic_cast<StdIp6AclRuleManagedObject *>((*pResults)[0]);

				startTransaction();
				trace (TRACE_LEVEL_INFO, string("Deleting the standard IP ACL rule#") + seqnum + string(" from the database ..."));
				stdPolicyMO->deleteRule(seqnum);
				deleteFromComposition(StdIp6AclPolicyManagedObject::getClassName(), StdIp6AclRuleManagedObject::getClassName(), "rules", stdPolicyMO->getObjectId(), pStdAclRuleMO->getObjectId());
				delete pStdAclRuleMO;
				commitTransaction();
				delete pResults;
			}
		}
		else if (EXTENDED_IP6_ACL == pGlobalIpAclPolicyMessage->getAclType())
		{
//			pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIp6AclRuleManagedObject::getClassName());
			ExtIp6AclRuleManagedObject *pExtAclRuleMO = NULL;

			extPolicyMO = dynamic_cast<ExtIp6AclPolicyManagedObject *>((*pResults)[0]);
			delete pResults;

			vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > extRuleMoVector;
			extPolicyMO->sortRules();
			extRuleMoVector = extPolicyMO->getRules();

			for (iter = 0; iter < extRuleMoVector.size(); iter++)
			{
				seqnum = extRuleMoVector[iter]->getRuleid();
				pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIp6AclRuleManagedObject::getClassName());
				pSyncQueryCtx->addAndAttribute (new AttributeUI64 (seqnum, "ruleid"));
				pSyncQueryCtx->addAndAttribute (new AttributeObjectId (extPolicyMO->getObjectId(), "ownerManagedObjectId"));
				pResults = querySynchronously (pSyncQueryCtx);
				delete pSyncQueryCtx;
				if (!pResults || pResults->size() != 1)
				{
					trace (TRACE_LEVEL_ERROR, "Ext Rule does not exist");
					if (pResults != NULL)
						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

					delete extPolicyMO;
					pGlobalIpAclPolicyMessage->setCompletionStatus(IP_ACL_RULE_DOESNT_EXIST);
					reply(pGlobalIpAclPolicyMessage);
					return;
				}
				pExtAclRuleMO = dynamic_cast<ExtIp6AclRuleManagedObject *>((*pResults)[0]);

				startTransaction();
				trace (TRACE_LEVEL_INFO, string("Deleting the EXTENDED IP ACL rule#") + seqnum + string(" from the database ..."));
				extPolicyMO->deleteRule(seqnum);
				deleteFromComposition(ExtIp6AclPolicyManagedObject::getClassName(), ExtIp6AclRuleManagedObject::getClassName(), "extRules", extPolicyMO->getObjectId(), pExtAclRuleMO->getObjectId());
				delete pExtAclRuleMO;
				commitTransaction();
				delete pResults;
			}
		}

		startTransaction();
		if (stdPolicyMO)
		{
			delete stdPolicyMO;
		}
		else
		{
			delete extPolicyMO;
		}
		status = commitTransaction();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Removing ACL policy from DB is successful.");
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "DB Commit transaction failed." + FrameworkToolKit::localize (status));
			status = WAVE_MESSAGE_ERROR;
		}

	}

	pGlobalIpAclPolicyMessage->setCompletionStatus(status);
	reply(pGlobalIpAclPolicyMessage);
	trace (TRACE_LEVEL_DEBUG, "Exit SsmIp6AclWorker::AAAGlobalIp6AclPolicyMessageHandler()");
}

int SsmIp6AclWorker::getPolicyEnforcedSwitches (string &policyName, vector<LocationId> &locVector) 
{
//	IpAclPolicyLocalManagedObject *pIpAclPolicyLocalMO = NULL;
	Ip6AclPolicyLocalManagedObject *pIp6AclPolicyLocalMO = NULL;
	//IpAclPolicyManagedObject *pIpAclPolicyMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	vector <UI32> mappedIdVector;

	/* Check if the ipAclPolicy exists  */
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyManagedObject::getClassName());

	pSyncQueryCtx->addAndAttribute (new AttributeString (policyName, "policyName"));
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
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyLocalManagedObject::getClassName());
	pSyncQueryCtx->addAndAttribute (new AttributeString(policyName, "ip6aclpolicy"));
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
		pIp6AclPolicyLocalMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>((*pResults)[j]);
		if (pIp6AclPolicyLocalMO) {
			switchId = pIp6AclPolicyLocalMO->getSwitchId();
			delete pIp6AclPolicyLocalMO;
		}

		if (switchId.length()) {
			UI32 mappedId = atoi(switchId.c_str());
			/* Check for duplicate entry:: M chassis*/
			vector<UI32>::iterator found;
			found = std::find(mappedIdVector.begin(), mappedIdVector.end(), mappedId);
			if (found == mappedIdVector.end()) {
				locVector.push_back(DcmToolKit::getLocationIdFromMappedId(mappedId));
				mappedIdVector.push_back(mappedId);
			}
		}
	}
	delete pResults;
	delete pSyncQueryCtx;

	return (FRAMEWORK_SUCCESS);
}

void SsmIp6AclWorker::SsmGlobalAssocIp6RuleToPolicyMessageHandler (SsmGlobalAssocIp6RuleToPolicyMessage *pGlobalAssocIp6RuleToPolicyMessage)
{
	trace (TRACE_LEVEL_INFO, "Enter SsmIp6AclWorker::AAAGlobalExtIp6AclRuleMessageHandler()");
	prismAssert(pGlobalAssocIp6RuleToPolicyMessage != NULL, __FILE__, __LINE__);

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	string className = "";

	if ((pGlobalAssocIp6RuleToPolicyMessage->getPolicyType()) == EXTENDED_IP6_ACL)
	{
		className = ExtIp6AclPolicyManagedObject::getClassName();
		trace (TRACE_LEVEL_INFO, string ("EXTENDED_IP6_ACL className ") + className);
	}
	else if ((pGlobalAssocIp6RuleToPolicyMessage->getPolicyType()) == STANDARD_IP6_ACL)
	{
		className = StdIp6AclPolicyManagedObject::getClassName();
		trace (TRACE_LEVEL_INFO, string ("STANDARD_IP6_ACL className ") + className);
	}

	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(className);
	pSyncQueryCtx->addAndAttribute (new AttributeString (pGlobalAssocIp6RuleToPolicyMessage->getPolicyName(), "policyName"));
	pSyncQueryCtx->setLoadOneToManyRelationships(false);
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults || pResults->size() != 1)
	{
		trace (TRACE_LEVEL_ERROR, "Policy does not exist");
		if (pResults != NULL)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		status = IP_ACL_DOESNT_EXIST;
		pGlobalAssocIp6RuleToPolicyMessage->setCompletionStatus(IP_ACL_DOESNT_EXIST);
		reply(pGlobalAssocIp6RuleToPolicyMessage);
		return;
	}
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	delete pSyncQueryCtx;

	status = processExtIp6AclRules(pGlobalAssocIp6RuleToPolicyMessage);
	if (status == WAVE_MESSAGE_SUCCESS)
	{
		// Send messages to all local services
		vector <LocationId> locVector;
		string policyName = pGlobalAssocIp6RuleToPolicyMessage->getPolicyName();
		int ret = getPolicyEnforcedSwitches(policyName, locVector);
		if (ret == FRAMEWORK_ERROR) {
			trace (TRACE_LEVEL_ERROR, "Framework error while getting policy enforced switches");
			pGlobalAssocIp6RuleToPolicyMessage->setCompletionStatus(IP_ACL_INTERNAL_ERROR);
			reply(pGlobalAssocIp6RuleToPolicyMessage);
			return;
		} else if (locVector.size() != 0) {
			updateIpAclRuleToAAA(pGlobalAssocIp6RuleToPolicyMessage, locVector);
		}
//			 else if (locVector.size() == 0) {
		pGlobalAssocIp6RuleToPolicyMessage->setCompletionStatus(status);
		reply (pGlobalAssocIp6RuleToPolicyMessage);
//		}
		
	}
	else
	{
		pGlobalAssocIp6RuleToPolicyMessage->setCompletionStatus(IP_ACL_INTERNAL_ERROR);
		reply (pGlobalAssocIp6RuleToPolicyMessage);
	}
	
	trace (TRACE_LEVEL_INFO, "Exit SsmIp6AclWorker::AAAGlobalExtIp6AclRuleMessageHandler()");
}

int SsmIp6AclWorker::processExtIp6AclRules(SsmGlobalAssocIp6RuleToPolicyMessage *pGlobalAssocIp6RuleToPolicyMessage)
{
	trace (TRACE_LEVEL_INFO, "SsmIp6AclWorker::processExtIp6AclRules() Enter");
	ExtIp6AclPolicyManagedObject *pExtAclPolicyMO = NULL;
	ExtIp6AclRuleManagedObject *pExtAclRuleMO = NULL;
	StdIp6AclPolicyManagedObject  *pStdAclPolicyMO = NULL;
	StdIp6AclRuleManagedObject *pStdAclRuleMO = NULL;
	string className = "";
	UI32 policyType = pGlobalAssocIp6RuleToPolicyMessage->getPolicyType();

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	const string policyName = pGlobalAssocIp6RuleToPolicyMessage->getPolicyName();
	UI64 sequenceNumber =	pGlobalAssocIp6RuleToPolicyMessage->getRuleid();
	if (policyType == STANDARD_IP6_ACL)
	{
		className = StdIp6AclPolicyManagedObject::getClassName();
		trace (TRACE_LEVEL_INFO, string("STANDARD_IP6_ACL ") + className + sequenceNumber);
	}
	else if (policyType == EXTENDED_IP6_ACL)
	{
		className = ExtIp6AclPolicyManagedObject::getClassName();
		trace (TRACE_LEVEL_INFO, string("EXTENDED_IP6_ACL ") + className + sequenceNumber);
	}

	trace (TRACE_LEVEL_INFO, string("debug: ") + policyName + policyType + className + sequenceNumber);
//	return (IP_ACL_DOESNT_EXIST);
	/* Get the MO instance of the Ext Acl Policy */
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(className);
	pSyncQueryCtx->addAndAttribute (new AttributeString (policyName, "policyName"));
	pSyncQueryCtx->setLoadOneToManyRelationships(false);
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults || pResults->size() != 1)
	{
		trace (TRACE_LEVEL_ERROR, "Policy does not exist");
		if (pResults != NULL)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		delete pSyncQueryCtx;
		pGlobalAssocIp6RuleToPolicyMessage->setCompletionStatus(IP_ACL_DOESNT_EXIST);
//		reply(pGlobalAssocIp6RuleToPolicyMessage);
		return (IP_ACL_DOESNT_EXIST);
	}
	else
	{
		if (policyType == STANDARD_IP6_ACL)
		{
			pStdAclPolicyMO = dynamic_cast<StdIp6AclPolicyManagedObject *>((*pResults)[0]);
		}
		else if (policyType == EXTENDED_IP6_ACL)
		{
			pExtAclPolicyMO = dynamic_cast<ExtIp6AclPolicyManagedObject *>((*pResults)[0]);
		}
	}
	delete pResults;
	delete pSyncQueryCtx;

	vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > extRuleMoVector;
	/* For DELETE operation, get the MO instance of the rule */
	if (pGlobalAssocIp6RuleToPolicyMessage->getUserEnteredSeqid() == 0)
	{
		/* Get the RuleId*/
		trace (TRACE_LEVEL_INFO, string("New DELETE option "));
		status = validateRule(pGlobalAssocIp6RuleToPolicyMessage);
		trace (TRACE_LEVEL_INFO, string("validateRule status:")+status);
		if(status != WAVE_MESSAGE_SUCCESS) {
			pGlobalAssocIp6RuleToPolicyMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			return(WAVE_MESSAGE_SUCCESS);
		}
		sequenceNumber = pGlobalAssocIp6RuleToPolicyMessage->getRuleid();
		pGlobalAssocIp6RuleToPolicyMessage->setUserEnteredSeqid(1);
	}
	if (IP6_DELETE_OPER == pGlobalAssocIp6RuleToPolicyMessage->getOpcode())
	{
		if (policyType == EXTENDED_IP6_ACL)
		{
			pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(ExtIp6AclRuleManagedObject::getClassName());
			pSyncQueryCtx->addAndAttribute (new AttributeUI64 (sequenceNumber, "ruleid"));
			pSyncQueryCtx->addAndAttribute (new AttributeObjectId (pExtAclPolicyMO->getObjectId(), "ownerManagedObjectId"));
		}
		else if (policyType == STANDARD_IP6_ACL)
		{
			pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(StdIp6AclRuleManagedObject::getClassName());
			pSyncQueryCtx->addAndAttribute (new AttributeUI64 (sequenceNumber, "ruleid"));
			pSyncQueryCtx->addAndAttribute (new AttributeObjectId (pStdAclPolicyMO->getObjectId(), "ownerManagedObjectId"));
		}
		pResults = querySynchronously (pSyncQueryCtx);
		if (!pResults || pResults->size() != 1)
		{
			trace (TRACE_LEVEL_ERROR, "Rule does not exist");
			if (pResults != NULL)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			delete pSyncQueryCtx;
			if (policyType == STANDARD_IP6_ACL)
			{
				delete pExtAclPolicyMO;
			}
			else if (policyType == EXTENDED_IP6_ACL)
			{
				delete pStdAclPolicyMO;
			}
			status = IP_ACL_RULE_DOESNT_EXIST;
			pGlobalAssocIp6RuleToPolicyMessage->setCompletionStatus(status);
//			reply(pGlobalAssocIp6RuleToPolicyMessage);
			return (IP_ACL_RULE_DOESNT_EXIST);
		}
		else
		{
			if (policyType == EXTENDED_IP6_ACL)
			{
				pExtAclRuleMO = dynamic_cast<ExtIp6AclRuleManagedObject *>((*pResults)[0]);
			}
			else if (policyType == STANDARD_IP6_ACL)
			{
				pStdAclRuleMO = dynamic_cast<StdIp6AclRuleManagedObject *>((*pResults)[0]);
			} 
		}
		delete pResults;
		delete pSyncQueryCtx;
	}

	/* Make changes to DCM database */
	if (IP6_DELETE_OPER == pGlobalAssocIp6RuleToPolicyMessage->getOpcode())
	{
		startTransaction();
		if (policyType == STANDARD_IP6_ACL)
		{
			trace (TRACE_LEVEL_INFO, string("Deleting the standard IP ACL rule#") + sequenceNumber + string(" from the database ..."));
			pStdAclPolicyMO->deleteRule(sequenceNumber);
			deleteFromComposition(StdIp6AclPolicyManagedObject::getClassName(), StdIp6AclRuleManagedObject::getClassName(), "rules", pStdAclPolicyMO->getObjectId(), pStdAclRuleMO->getObjectId());
			delete pStdAclRuleMO;
		}
		else if (policyType == EXTENDED_IP6_ACL)
		{
			trace (TRACE_LEVEL_INFO, string("Deleting the EXTENDED IP ACL rule#") + sequenceNumber + string(" from the database ..."));
			pExtAclPolicyMO->deleteRule(sequenceNumber);
			deleteFromComposition(ExtIp6AclPolicyManagedObject::getClassName(), ExtIp6AclRuleManagedObject::getClassName(), "extRules", pExtAclPolicyMO->getObjectId(), pExtAclRuleMO->getObjectId());
			delete pExtAclRuleMO;
		}
		commitTransaction();
	}
	else if (IP6_ADD_OPER == pGlobalAssocIp6RuleToPolicyMessage->getOpcode())
	{
		if (policyType == STANDARD_IP6_ACL)
		{
			trace (TRACE_LEVEL_INFO, string("IP6_ADD_OPER the standard IP ACL rule#") + sequenceNumber + string(" to the database ..."));
			assocIpAclRuleToPolicy(pGlobalAssocIp6RuleToPolicyMessage, pStdAclPolicyMO);
		}
		else if (policyType == EXTENDED_IP6_ACL)
		{
			trace (TRACE_LEVEL_INFO, string("IP6_ADD_OPER theEXTENDED IP ACL rule#") + sequenceNumber + string(" to the database ..."));
			assocIpAclRuleToPolicy(pGlobalAssocIp6RuleToPolicyMessage, pExtAclPolicyMO);
		}
	}

	if (policyType == STANDARD_IP6_ACL)
	{
		delete pStdAclPolicyMO;
	}
	else if (policyType == EXTENDED_IP6_ACL)
	{
		delete pExtAclPolicyMO;
	}
	pGlobalAssocIp6RuleToPolicyMessage->setCompletionStatus(status);
//	reply(pGlobalAssocIp6RuleToPolicyMessage);
	trace (TRACE_LEVEL_INFO, "SsmIp6AclWorker::processExtIp6AclRules() Exit");
	return (status);
}

ResourceId SsmIp6AclWorker::validateRule(SsmGlobalAssocIp6RuleToPolicyMessage *pGlobalAssocIp6RuleToPolicyMessage)
{
        trace(TRACE_LEVEL_INFO,string("SsmIp6AclWorker::validateRule Enter..."));

        UI32 policyType                             = pGlobalAssocIp6RuleToPolicyMessage->getPolicyType();
        const string policyName                     = pGlobalAssocIp6RuleToPolicyMessage->getPolicyName();
        IpV6Address srcHostIpAddress                = pGlobalAssocIp6RuleToPolicyMessage->getSrcHostIpAddress();
        IpV6AddressNetworkMask srcIpAddress                    = pGlobalAssocIp6RuleToPolicyMessage->getSrcIpAddress();
//        IpV4Address srcIpAddressMask                = pGlobalAssocIp6RuleToPolicyMessage->getSrcIpAddressMask();
        IpAddrType srcType                          = pGlobalAssocIp6RuleToPolicyMessage->getSrcType();
        Ip6AclPolicyManagedObject *policyMO          = NULL;
        UI64 seqid                                  = 0;
        string className                            = "";
        string childClassName                       = "";
        ResourceId status                           = WAVE_MESSAGE_SUCCESS;
        bool seqFound                               = false;
        bool ruleFound                              = false;
        UI32 opcode                                 = (UI32) pGlobalAssocIp6RuleToPolicyMessage->getOpcode();
        UI32 UserEnteredSeqid                       = pGlobalAssocIp6RuleToPolicyMessage->getUserEnteredSeqid();

//      trace(TRACE_LEVEL_INFO,string("SsmIp6AclWorker::validateRule"));
        prismAssert(opcode == IP6_ADD_OPER ||
                opcode == IP6_DELETE_OPER, __FILE__, __LINE__);
        if (policyType == STANDARD_IP6_ACL) {
            className = StdIp6AclPolicyManagedObject::getClassName();
            childClassName = StdIp6AclRuleManagedObject::getClassName();
        } else if (policyType == EXTENDED_IP6_ACL) {
            className = ExtIp6AclPolicyManagedObject::getClassName();
            childClassName = ExtIp6AclRuleManagedObject::getClassName();
        } else {
            // This case should not appear and it is error case
            className = Ip6AclPolicyManagedObject::getClassName();
            status = WAVE_MESSAGE_ERROR;
        }
//       trace(TRACE_LEVEL_INFO,string("SsmIp6AclWorker::validateRule className=")+className+" childClassName="+childClassName);

        /* Query the MO for a given policy*/

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
        // set the objectId as addSelectedField in the query
        syncQueryCtxt.addSelectFieldForObjectId ("objectId");
        syncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
        syncQueryCtxt.setLoadOneToManyRelationships(false);
        vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);

//      trace(TRACE_LEVEL_INFO,string("SsmIp6AclWorker::validateRule querySynchronously called"));
        if (NULL != pResultsPMO) {
            UI32 numberOfResults = pResultsPMO->size ();
//          trace(TRACE_LEVEL_INFO,string("SsmIp6AclWorker::validateRule() numberOfResults=")+numberOfResults);
            if (0 == numberOfResults) {
                trace(TRACE_LEVEL_INFO,string("Can't find the policy Managed Object"));
                status = WRC_SSM_DCM_ERR_ACL_EEXIST; //Acl does not exist
            } else if (1 == numberOfResults) {
                trace(TRACE_LEVEL_INFO,string("Found Policy Managed Object"));
                policyMO = dynamic_cast<Ip6AclPolicyManagedObject *>((*pResultsPMO)[0]);
                ObjectId policyId = policyMO->getObjectId();
                // set the policyId in the sequencer context
//                seqContext->setWaveManagedObjectId(policyId);
                WaveManagedObjectSynchronousQueryContext queryCtxtForChildWithseqNum(childClassName);
                WaveManagedObjectSynchronousQueryContext queryCtxtForChild(childClassName);

                queryCtxtForChildWithseqNum.addAndAttribute (new AttributeObjectId (policyId, "ownerManagedObjectId"));
                queryCtxtForChild.addAndAttribute (new AttributeObjectId (policyId, "ownerManagedObjectId"));

                /**
                 * added the selectField value as ruleid, and set the
                 * LoadOneToManyRelationships to false to improve the
                 * select query performance.
                 */
                queryCtxtForChild.addSelectField("ruleid");
                queryCtxtForChild.setLoadOneToManyRelationships(false);

                if (UserEnteredSeqid) {
                    seqid = pGlobalAssocIp6RuleToPolicyMessage->getRuleid();
                    queryCtxtForChildWithseqNum.addAndAttribute (new AttributeUI64 (seqid, "ruleid"));
                    /**
                     * added the selectField value as ruleid, and set the
                     * LoadOneToManyRelationships to false to improve the
                     * select query performance.
                     */
                    queryCtxtForChildWithseqNum.addSelectField("ruleid");
                    queryCtxtForChildWithseqNum.setLoadOneToManyRelationships(false);
                    QuerychildValidation(queryCtxtForChildWithseqNum, status, ruleFound, seqFound, opcode, UserEnteredSeqid, seqid, policyType);
                }
                if ((status == WAVE_MESSAGE_SUCCESS) && (((opcode == IP6_DELETE_OPER) && (!UserEnteredSeqid))
                            || ((opcode == IP6_ADD_OPER)))) {

                    queryCtxtForChild.addAndAttribute (new AttributeEnum(pGlobalAssocIp6RuleToPolicyMessage->getAction(), "action"));

                    if (srcType == IP_ADDR_TYPE_ANY) {
                        queryCtxtForChild.addAndAttribute (new AttributeEnum(srcType, "srcType"));
                    } else if (srcType == IP_ADDR_TYPE_HOST) {
                        queryCtxtForChild.addAndAttribute (new AttributeEnum(srcType, "srcType"));
                        queryCtxtForChild.addAndAttribute (new AttributeIpV6Address(srcHostIpAddress, "srcHostIpAddress"));
                    } else if (srcType == IP_ADDR_TYPE_NONE) {
                        queryCtxtForChild.addAndAttribute (new AttributeEnum(srcType, "srcType"));
                        queryCtxtForChild.addAndAttribute (new AttributeIpV6AddressNetworkMask(srcIpAddress, "srcIpAddress"));
//                        queryCtxtForChild.addAndAttribute (new AttributeIpV4Address(srcIpAddressMask, "srcIpAddressMask"));
                    }
                    if (policyType == EXTENDED_IP6_ACL) {

                        IpAddrType  dstType                                     = pGlobalAssocIp6RuleToPolicyMessage->getDstType();
                        IpV6AddressNetworkMask dstIpAddress                                = pGlobalAssocIp6RuleToPolicyMessage->getDstIpAddress();
//                        IpV4Address dstIpAddressMask                            = pGlobalAssocIp6RuleToPolicyMessage->getDstIpAddressMask();
                        IpV6Address dstHostIpAddress                            = pGlobalAssocIp6RuleToPolicyMessage->getDstHostIpAddress();
                        WaveUnion dstUnion                                      = pGlobalAssocIp6RuleToPolicyMessage->getDstUnion();
                        WaveUnion ProtocolTypeUnion                             = pGlobalAssocIp6RuleToPolicyMessage->getProtocolTypeUnion();
                        UI32 l3l4CustomProtoType                                = pGlobalAssocIp6RuleToPolicyMessage->getL3l4CustomProtoType();
                        L3L4wellknownProtoType l3l4WellknownProtoType           = pGlobalAssocIp6RuleToPolicyMessage->getL3l4WellknownProtoType();
                        PortOperator srcPortOperator                            = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortOperator();
                        PortOperator dstPortOperator                            = pGlobalAssocIp6RuleToPolicyMessage->getDstPortOperator();
                        WaveUnion srcPortNumberTcpUnion                         = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortNumberTcpUnion();
                        WaveUnion srcPortltNumberTcpUnion                       = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortltNumberTcpUnion();
                        WaveUnion srcPortgtNumberTcpUnion                       = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortgtNumberTcpUnion();
                        WaveUnion dstPortNumberTcpUnion                         = pGlobalAssocIp6RuleToPolicyMessage->getDstPortNumberTcpUnion();
                        WaveUnion dstPortltNumberTcpUnion                       = pGlobalAssocIp6RuleToPolicyMessage->getDstPortltNumberTcpUnion();
                        WaveUnion dstPortgtNumberTcpUnion                       = pGlobalAssocIp6RuleToPolicyMessage->getDstPortgtNumberTcpUnion();
                        WaveUnion srcPortNumberUdpUnion                         = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortNumberUdpUnion();
                        WaveUnion srcPortltNumberUdpUnion                       = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortltNumberUdpUnion();
                        WaveUnion srcPortgtNumberUdpUnion                       = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortgtNumberUdpUnion();
                        WaveUnion dstPortNumberUdpUnion                         = pGlobalAssocIp6RuleToPolicyMessage->getDstPortNumberUdpUnion();
                        WaveUnion dstPortltNumberUdpUnion                       = pGlobalAssocIp6RuleToPolicyMessage->getDstPortltNumberUdpUnion();
                        WaveUnion dstPortgtNumberUdpUnion                       = pGlobalAssocIp6RuleToPolicyMessage->getDstPortgtNumberUdpUnion();
                        WaveUnion srcPortrangeLowerTcpUnion                     = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortrangeLowerTcpUnion();
                        WaveUnion dstPortrangeLowerTcpUnion                     = pGlobalAssocIp6RuleToPolicyMessage->getDstPortrangeLowerTcpUnion();
                        WaveUnion srcPortrangeHigherTcpUnion                    = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortrangeHigherTcpUnion();
                        WaveUnion dstPortrangeHigherTcpUnion                    = pGlobalAssocIp6RuleToPolicyMessage->getDstPortrangeHigherTcpUnion();
                        WaveUnion srcPortrangeLowerUdpUnion                     = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortrangeLowerUdpUnion();
                        WaveUnion dstPortrangeLowerUdpUnion                     = pGlobalAssocIp6RuleToPolicyMessage->getDstPortrangeLowerUdpUnion();
                        WaveUnion srcPortrangeHigherUdpUnion                    = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortrangeHigherUdpUnion();
                        WaveUnion dstPortrangeHigherUdpUnion                    = pGlobalAssocIp6RuleToPolicyMessage->getDstPortrangeHigherUdpUnion();
                        UI32UC srcPortCustomNumberTcp                           = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortCustomNumberTcp();
                        UI32UC srcPortltCustomNumberTcp                         = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortltCustomNumberTcp();
                        UI32UC srcPortgtCustomNumberTcp                         = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortgtCustomNumberTcp();
                        UI32UC dstPortCustomNumberTcp                           = pGlobalAssocIp6RuleToPolicyMessage->getDstPortCustomNumberTcp();
                        UI32UC dstPortltCustomNumberTcp                         = pGlobalAssocIp6RuleToPolicyMessage->getDstPortltCustomNumberTcp();
                        UI32UC dstPortgtCustomNumberTcp                         = pGlobalAssocIp6RuleToPolicyMessage->getDstPortgtCustomNumberTcp();
                        UI32UC srcPortCustomNumberUdp                           = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortCustomNumberUdp();
                        UI32UC srcPortltCustomNumberUdp                         = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortltCustomNumberUdp();
                        UI32UC srcPortgtCustomNumberUdp                         = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortgtCustomNumberUdp();
                        UI32UC dstPortCustomNumberUdp                           = pGlobalAssocIp6RuleToPolicyMessage->getDstPortCustomNumberUdp();
                        UI32UC dstPortltCustomNumberUdp                         = pGlobalAssocIp6RuleToPolicyMessage->getDstPortltCustomNumberUdp();
                        UI32UC dstPortgtCustomNumberUdp                         = pGlobalAssocIp6RuleToPolicyMessage->getDstPortgtCustomNumberUdp();
                        EnumUC srcPortWellknownNumberTcp                        = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortWellknownNumberTcp();
                        EnumUC srcPortltWellknownNumberTcp                      = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortltWellknownNumberTcp();
                        EnumUC srcPortgtWellknownNumberTcp                      = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortgtWellknownNumberTcp();
                        EnumUC dstPortWellknownNumberTcp                        = pGlobalAssocIp6RuleToPolicyMessage->getDstPortWellknownNumberTcp();
                        EnumUC dstPortltWellknownNumberTcp                      = pGlobalAssocIp6RuleToPolicyMessage->getDstPortltWellknownNumberTcp();
                        EnumUC dstPortgtWellknownNumberTcp                      = pGlobalAssocIp6RuleToPolicyMessage->getDstPortgtWellknownNumberTcp();
                        EnumUC srcPortWellknownNumberUdp                        = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortWellknownNumberUdp();
                        EnumUC srcPortltWellknownNumberUdp                      = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortltWellknownNumberUdp();
                        EnumUC srcPortgtWellknownNumberUdp                      = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortgtWellknownNumberUdp();
                        EnumUC dstPortWellknownNumberUdp                        = pGlobalAssocIp6RuleToPolicyMessage->getDstPortWellknownNumberUdp();
                        EnumUC dstPortltWellknownNumberUdp                      = pGlobalAssocIp6RuleToPolicyMessage->getDstPortltWellknownNumberUdp();
                        EnumUC dstPortgtWellknownNumberUdp                      = pGlobalAssocIp6RuleToPolicyMessage->getDstPortgtWellknownNumberUdp();
                        UI32UC srcPortCustomrangeLowerTcp                       = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortCustomrangeLowerTcp();
                        UI32UC dstPortCustomrangeLowerTcp                       = pGlobalAssocIp6RuleToPolicyMessage->getDstPortCustomrangeLowerTcp();
                        UI32UC srcPortCustomrangeLowerUdp                       = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortCustomrangeLowerUdp();
                        UI32UC dstPortCustomrangeLowerUdp                       = pGlobalAssocIp6RuleToPolicyMessage->getDstPortCustomrangeLowerUdp();
                        UI32UC srcPortCustomrangeHigherTcp                      = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortCustomrangeHigherTcp();
                        UI32UC dstPortCustomrangeHigherTcp                      = pGlobalAssocIp6RuleToPolicyMessage->getDstPortCustomrangeHigherTcp();
                        UI32UC srcPortCustomrangeHigherUdp                      = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortCustomrangeHigherUdp();
                        UI32UC dstPortCustomrangeHigherUdp                      = pGlobalAssocIp6RuleToPolicyMessage->getDstPortCustomrangeHigherUdp();
                        EnumUC srcPortWellknownrangeLowerUdp                    = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortWellknownrangeLowerUdp();
                        EnumUC dstPortWellknownrangeLowerUdp                    = pGlobalAssocIp6RuleToPolicyMessage->getDstPortWellknownrangeLowerUdp();
                        EnumUC srcPortWellknownrangeLowerTcp                    = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortWellknownrangeLowerTcp();
                        EnumUC dstPortWellknownrangeLowerTcp                    = pGlobalAssocIp6RuleToPolicyMessage->getDstPortWellknownrangeLowerTcp();
                        EnumUC srcPortWellknownrangeHigherUdp                   = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortWellknownrangeHigherUdp();
                        EnumUC dstPortWellknownrangeHigherUdp                   = pGlobalAssocIp6RuleToPolicyMessage->getDstPortWellknownrangeHigherUdp();
                        EnumUC srcPortWellknownrangeHigherTcp                   = pGlobalAssocIp6RuleToPolicyMessage->getSrcPortWellknownrangeHigherTcp();
                        EnumUC dstPortWellknownrangeHigherTcp                   = pGlobalAssocIp6RuleToPolicyMessage->getDstPortWellknownrangeHigherTcp();
                        EnumUC wellknownPrecedence                              = pGlobalAssocIp6RuleToPolicyMessage->getWellknownPrecedence();
                        EnumUC wellknownTos                                     = pGlobalAssocIp6RuleToPolicyMessage->getWellknownTos();
                        EnumUC wellknownDscp                                    = pGlobalAssocIp6RuleToPolicyMessage->getWellknownDscp();
                        UI32UC customPrecedence                                 = pGlobalAssocIp6RuleToPolicyMessage->getCustomPrecedence();
                        UI32UC customTos                                        = pGlobalAssocIp6RuleToPolicyMessage->getCustomTos();
                        UI32UC customDscp                                       = pGlobalAssocIp6RuleToPolicyMessage->getCustomDscp();
                        WaveUnion precedenceUnion                               = pGlobalAssocIp6RuleToPolicyMessage->getPrecedenceUnion();
                        WaveUnion dscpUnion                                     = pGlobalAssocIp6RuleToPolicyMessage->getDscpUnion();
                        WaveUnion tosUnion                                      = pGlobalAssocIp6RuleToPolicyMessage->getTosUnion();
                        bool urg                                                = pGlobalAssocIp6RuleToPolicyMessage->getUrg();
                        bool ack                                                = pGlobalAssocIp6RuleToPolicyMessage->getAck();
                        bool push                                               = pGlobalAssocIp6RuleToPolicyMessage->getPush();
                        bool fin                                                = pGlobalAssocIp6RuleToPolicyMessage->getFin();
                        bool sync                                               = pGlobalAssocIp6RuleToPolicyMessage->getSync();
                        bool rst                                                = pGlobalAssocIp6RuleToPolicyMessage->getRst();

                        if (dstType == IP_ADDR_TYPE_ANY) {
                            queryCtxtForChild.addAndAttribute (new AttributeEnum (dstType, "dstType"));
                        } else if (dstType == IP_ADDR_TYPE_HOST) {
                            queryCtxtForChild.addAndAttribute (new AttributeEnum (dstType, "dstType"));
                            queryCtxtForChild.addAndAttribute (new AttributeIpV6Address(dstHostIpAddress, "dstHostIpAddress"));
                        } else if (dstType == IP_ADDR_TYPE_NONE) {
                            queryCtxtForChild.addAndAttribute (new AttributeEnum (dstType, "dstType"));
                            queryCtxtForChild.addAndAttribute (new AttributeIpV6AddressNetworkMask(dstIpAddress, "dstIpAddress"));
//                            queryCtxtForChild.addAndAttribute (new AttributeIpV4Address(dstIpAddressMask, "dstIpAddressMask"));
                        }

                        if (ProtocolTypeUnion.compare("l3l4WellknownProtoType") == 0) {
                            queryCtxtForChild.addAndAttribute (new AttributeEnum(l3l4WellknownProtoType, "l3l4WellknownProtoType"));
                        } else if (ProtocolTypeUnion.compare("l3l4CustomProtoType") == 0) {
                            queryCtxtForChild.addAndAttribute (new AttributeUI32(l3l4CustomProtoType, "l3l4CustomProtoType"));
                        }

                        queryCtxtForChild.addAndAttribute (new AttributeEnum(srcPortOperator, "srcPortOperator"));
                        if (srcPortOperator) {
                            /* TCP */
                            if (l3l4WellknownProtoType == L3L4_PROTOTYPE_TCP) {
                                if (srcPortOperator == PORTOPERATOR_LT) {
                                    if ((srcPortltNumberTcpUnion.compare ("srcPortltCustomNumberTcp") == 0) && (srcPortltCustomNumberTcp != DEFAULT_PORTNUM)) {
                                        srcPortltCustomNumberTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(srcPortltCustomNumberTcp, true, "srcPortltCustomNumberTcp"));
                                    } else if ((srcPortltNumberTcpUnion.compare ("srcPortltWellknownNumberTcp") == 0)&& (srcPortltWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
                                        srcPortltWellknownNumberTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(srcPortltWellknownNumberTcp, true, "srcPortltWellknownNumberTcp"));
                                    }
                                } else if (srcPortOperator == PORTOPERATOR_GT) {
                                    if ((srcPortgtNumberTcpUnion.compare ("srcPortgtCustomNumberTcp") == 0) && (srcPortgtCustomNumberTcp != DEFAULT_PORTNUM)) {
                                        srcPortgtCustomNumberTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(srcPortgtCustomNumberTcp, true, "srcPortgtCustomNumberTcp"));
                                    } else if ((srcPortgtNumberTcpUnion.compare ("srcPortgtWellknownNumberTcp") == 0) && (srcPortgtWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
                                        srcPortgtWellknownNumberTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(srcPortgtWellknownNumberTcp, true, "srcPortgtWellknownNumberTcp"));
                                    }
                                }  else if ((srcPortOperator == PORTOPERATOR_EQ) || (srcPortOperator == PORTOPERATOR_NEQ)) {
                                    if ((srcPortNumberTcpUnion.compare ("srcPortWellknownNumberTcp") == 0) && (srcPortWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
                                        srcPortWellknownNumberTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(srcPortWellknownNumberTcp, true, "srcPortWellknownNumberTcp"));
                                    } else if ((srcPortNumberTcpUnion.compare ("srcPortCustomNumberTcp") == 0) && (srcPortCustomNumberTcp != DEFAULT_PORTNUM)) {
                                        srcPortCustomNumberTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(srcPortCustomNumberTcp, true, "srcPortCustomNumberTcp"));
                                    }
                                } else if (srcPortOperator == PORTOPERATOR_RANGE) {
                                    if ((srcPortrangeLowerTcpUnion.compare("srcPortWellknownrangeLowerTcp") == 0) && (srcPortWellknownrangeLowerTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
                                        srcPortWellknownrangeLowerTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(srcPortWellknownrangeLowerTcp, true, "srcPortWellknownrangeLowerTcp"));
                                    } else if ((srcPortrangeLowerTcpUnion.compare("srcPortCustomrangeLowerTcp") == 0) && (srcPortCustomrangeLowerTcp != DEFAULT_PORTNUM)){
                                        srcPortCustomrangeLowerTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(srcPortCustomrangeLowerTcp, true, "srcPortCustomrangeLowerTcp"));
                                    }
                                    if ((srcPortrangeHigherTcpUnion.compare("srcPortWellknownrangeHigherTcp") == 0) && (srcPortWellknownrangeHigherTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
                                        srcPortWellknownrangeHigherTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(srcPortWellknownrangeHigherTcp, true, "srcPortWellknownrangeHigherTcp"));
                                    } else if ((srcPortrangeHigherTcpUnion.compare("srcPortCustomrangeHigherTcp") == 0) && (srcPortCustomrangeHigherTcp != DEFAULT_PORTNUM)){
                                        srcPortCustomrangeHigherTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(srcPortCustomrangeHigherTcp, true, "srcPortCustomrangeHigherTcp"));
                                    }
                                }
                            } else if (l3l4WellknownProtoType == L3L4_PROTOTYPE_UDP) {
                                if (srcPortOperator == PORTOPERATOR_LT) {
                                    if ((srcPortltNumberUdpUnion.compare ("srcPortltCustomNumberUdp") == 0) && (srcPortltCustomNumberUdp != DEFAULT_PORTNUM)) {
                                        srcPortltCustomNumberUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(srcPortltCustomNumberUdp, true, "srcPortltCustomNumberUdp"));
                                    } else if ((srcPortltNumberUdpUnion.compare ("srcPortltWellknownNumberUdp") == 0)&& (srcPortltWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
                                        srcPortltWellknownNumberUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(srcPortltWellknownNumberUdp, true, "srcPortltWellknownNumberUdp"));
                                    }
                                } else if (srcPortOperator == PORTOPERATOR_GT) {
                                    if ((srcPortgtNumberUdpUnion.compare ("srcPortgtCustomNumberUdp") == 0) && (srcPortgtCustomNumberUdp != DEFAULT_PORTNUM)) {
                                        srcPortgtCustomNumberUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(srcPortgtCustomNumberUdp, true, "srcPortgtCustomNumberUdp"));
                                    } else if ((srcPortgtNumberUdpUnion.compare ("srcPortgtWellknownNumberUdp") == 0) && (srcPortgtWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
                                        srcPortgtWellknownNumberUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(srcPortgtWellknownNumberUdp, true, "srcPortgtWellknownNumberUdp"));
                                    }
                                }  else if ((srcPortOperator == PORTOPERATOR_EQ) || (srcPortOperator == PORTOPERATOR_NEQ)) {
                                    if ((srcPortNumberUdpUnion.compare ("srcPortWellknownNumberUdp") == 0) && (srcPortWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
                                        srcPortWellknownNumberUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(srcPortWellknownNumberUdp, true, "srcPortWellknownNumberUdp"));
                                    } else if ((srcPortNumberUdpUnion.compare ("srcPortCustomNumberUdp") == 0) && (srcPortCustomNumberUdp != DEFAULT_PORTNUM)) {
                                        srcPortCustomNumberUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(srcPortCustomNumberUdp, true ,"srcPortCustomNumberUdp"));
                                    }
                                } else if (srcPortOperator == PORTOPERATOR_RANGE) {
                                    if ((srcPortrangeLowerUdpUnion.compare("srcPortWellknownrangeLowerUdp") == 0) && (srcPortWellknownrangeLowerUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
                                        srcPortWellknownrangeLowerUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(srcPortWellknownrangeLowerUdp, true , "srcPortWellknownrangeLowerUdp"));
                                    } else if ((srcPortrangeLowerUdpUnion.compare("srcPortCustomrangeLowerUdp") == 0) && (srcPortCustomrangeLowerUdp != DEFAULT_PORTNUM)){
                                        srcPortCustomrangeLowerUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(srcPortCustomrangeLowerUdp, true, "srcPortCustomrangeLowerUdp"));
                                    }
                                    if ((srcPortrangeHigherUdpUnion.compare("srcPortWellknownrangeHigherUdp") == 0) && (srcPortWellknownrangeHigherUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
                                        srcPortWellknownrangeHigherUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(srcPortWellknownrangeHigherUdp, true, "srcPortWellknownrangeHigherUdp"));
                                    } else if ((srcPortrangeHigherUdpUnion.compare("srcPortCustomrangeHigherUdp") == 0) && (srcPortCustomrangeHigherUdp != DEFAULT_PORTNUM)){
                                        srcPortCustomrangeHigherUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(srcPortCustomrangeHigherUdp, true, "srcPortCustomrangeHigherUdp"));
                                    }
                                }

                            }
                        }

                        queryCtxtForChild.addAndAttribute (new AttributeEnum(dstPortOperator, "dstPortOperator"));

                        if (dstPortOperator) {
                            /* TCP */
                            if (l3l4WellknownProtoType == L3L4_PROTOTYPE_TCP) {
                                if (dstPortOperator == PORTOPERATOR_LT) {
                                    if ((dstPortltNumberTcpUnion.compare ("dstPortltCustomNumberTcp") == 0) && (dstPortltCustomNumberTcp != DEFAULT_PORTNUM)) {
                                        dstPortltCustomNumberTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(dstPortltCustomNumberTcp, true, "dstPortltCustomNumberTcp"));
                                    } else if ((dstPortltNumberTcpUnion.compare ("dstPortltWellknownNumberTcp") == 0)&& (dstPortltWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
                                        dstPortltWellknownNumberTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(dstPortltWellknownNumberTcp, true, "dstPortltWellknownNumberTcp"));
                                    }
                                } else if (dstPortOperator == PORTOPERATOR_GT) {
                                    if ((dstPortgtNumberTcpUnion.compare ("dstPortgtCustomNumberTcp") == 0) && (dstPortgtCustomNumberTcp != DEFAULT_PORTNUM)) {
                                        dstPortgtCustomNumberTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(dstPortgtCustomNumberTcp, true, "dstPortgtCustomNumberTcp"));
                                    } else if ((dstPortgtNumberTcpUnion.compare ("dstPortgtWellknownNumberTcp") == 0) && (dstPortgtWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
                                        dstPortgtWellknownNumberTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(dstPortgtWellknownNumberTcp, true, "dstPortgtWellknownNumberTcp"));
                                    }
                                }  else if ((dstPortOperator == PORTOPERATOR_EQ) || (dstPortOperator == PORTOPERATOR_NEQ)) {
                                    if ((dstPortNumberTcpUnion.compare ("dstPortWellknownNumberTcp") == 0) && (dstPortWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
                                        dstPortWellknownNumberTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(dstPortWellknownNumberTcp, true, "dstPortWellknownNumberTcp"));
                                    } else if ((dstPortNumberTcpUnion.compare ("dstPortCustomNumberTcp") == 0) && (dstPortCustomNumberTcp != DEFAULT_PORTNUM)) {
                                        dstPortCustomNumberTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(dstPortCustomNumberTcp, true, "dstPortCustomNumberTcp"));
                                    }
                                } else if (dstPortOperator == PORTOPERATOR_RANGE) {
                                    if ((dstPortrangeLowerTcpUnion.compare("dstPortWellknownrangeLowerTcp") == 0) && (dstPortWellknownrangeLowerTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
                                        dstPortWellknownrangeLowerTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(dstPortWellknownrangeLowerTcp, true, "dstPortWellknownrangeLowerTcp"));
                                    } else if ((dstPortrangeLowerTcpUnion.compare("dstPortCustomrangeLowerTcp") == 0) && (dstPortCustomrangeLowerTcp != DEFAULT_PORTNUM)){
                                        dstPortCustomrangeLowerTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(dstPortCustomrangeLowerTcp, true, "dstPortCustomrangeLowerTcp"));
                                    }
                                    if ((dstPortrangeHigherTcpUnion.compare("dstPortWellknownrangeHigherTcp") == 0) && (dstPortWellknownrangeHigherTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
                                        dstPortWellknownrangeHigherTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(dstPortWellknownrangeHigherTcp, true, "dstPortWellknownrangeHigherTcp"));
                                    } else if ((dstPortrangeHigherTcpUnion.compare("dstPortCustomrangeHigherTcp") == 0) && (dstPortCustomrangeHigherTcp != DEFAULT_PORTNUM)){
                                        dstPortCustomrangeHigherTcp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(dstPortCustomrangeHigherTcp, true, "dstPortCustomrangeHigherTcp"));
                                    }
                                }
                            } else if (l3l4WellknownProtoType == L3L4_PROTOTYPE_UDP) {
                                if (dstPortOperator == PORTOPERATOR_LT) {
                                    if ((dstPortltNumberUdpUnion.compare ("dstPortltCustomNumberUdp") == 0) && (dstPortltCustomNumberUdp != DEFAULT_PORTNUM)) {
                                        dstPortltCustomNumberUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(dstPortltCustomNumberUdp, true, "dstPortltCustomNumberUdp"));
                                    } else if ((dstPortltNumberUdpUnion.compare ("dstPortltWellknownNumberUdp") == 0)&& (dstPortltWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
                                        dstPortltWellknownNumberUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(dstPortltWellknownNumberUdp, true, "dstPortltWellknownNumberUdp"));
                                    }
                                } else if (dstPortOperator == PORTOPERATOR_GT) {
                                    if ((dstPortgtNumberUdpUnion.compare ("dstPortgtCustomNumberUdp") == 0) && (dstPortgtCustomNumberUdp != DEFAULT_PORTNUM)) {
                                        dstPortgtCustomNumberUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(dstPortgtCustomNumberUdp, true, "dstPortgtCustomNumberUdp"));
                                    } else if ((dstPortgtNumberUdpUnion.compare ("dstPortgtWellknownNumberUdp") == 0) && (dstPortgtWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
                                        dstPortgtWellknownNumberUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(dstPortgtWellknownNumberUdp, true, "dstPortgtWellknownNumberUdp"));
                                    }
                                }  else if ((dstPortOperator == PORTOPERATOR_EQ) || (dstPortOperator == PORTOPERATOR_NEQ)) {
                                    if ((dstPortNumberUdpUnion.compare ("dstPortWellknownNumberUdp") == 0) && (dstPortWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
                                        dstPortWellknownNumberUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(dstPortWellknownNumberUdp, true, "dstPortWellknownNumberUdp"));
                                    } else if ((dstPortNumberUdpUnion.compare ("dstPortCustomNumberUdp") == 0) && (dstPortCustomNumberUdp != DEFAULT_PORTNUM)) {
                                        dstPortCustomNumberUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(dstPortCustomNumberUdp, true ,"dstPortCustomNumberUdp"));
                                    }
                                } else if (dstPortOperator == PORTOPERATOR_RANGE) {
                                    if ((dstPortrangeLowerUdpUnion.compare("dstPortWellknownrangeLowerUdp") == 0) && (dstPortWellknownrangeLowerUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
                                        dstPortWellknownrangeLowerUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(dstPortWellknownrangeLowerUdp, true , "dstPortWellknownrangeLowerUdp"));
                                    } else if ((dstPortrangeLowerUdpUnion.compare("dstPortCustomrangeLowerUdp") == 0) && (dstPortCustomrangeLowerUdp != DEFAULT_PORTNUM)){
                                        dstPortCustomrangeLowerUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(dstPortCustomrangeLowerUdp, true, "dstPortCustomrangeLowerUdp"));
                                    }
                                    if ((dstPortrangeHigherUdpUnion.compare("dstPortWellknownrangeHigherUdp") == 0) && (dstPortWellknownrangeHigherUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
                                        dstPortWellknownrangeHigherUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(dstPortWellknownrangeHigherUdp, true, "dstPortWellknownrangeHigherUdp"));
                                    } else if ((dstPortrangeHigherUdpUnion.compare("dstPortCustomrangeHigherUdp") == 0) && (dstPortCustomrangeHigherUdp != DEFAULT_PORTNUM)){
                                        dstPortCustomrangeHigherUdp.setIsUserConfigured(true);
                                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(dstPortCustomrangeHigherUdp, true, "dstPortCustomrangeHigherUdp"));
                                    }
                                }

                            }
                        }

                        if (customPrecedence != DEFAULT_PREC_DSCP_TOS_VAL)
                            customPrecedence.setIsUserConfigured(true);
                        if (wellknownPrecedence != WELLKNOWN_PRECEDENCE_UNKNOWN)
                            wellknownPrecedence.setIsUserConfigured(true);
                        if (customDscp != DEFAULT_PREC_DSCP_TOS_VAL)
                            customDscp.setIsUserConfigured(true);
                        if (wellknownDscp != WELLKNOWN_DSCP_UNKNOWN)
                            wellknownDscp.setIsUserConfigured(true);
                        if (customTos != DEFAULT_PREC_DSCP_TOS_VAL)
                            customTos.setIsUserConfigured(true);
                        if (wellknownTos != WELLKNOWN_TOS_UNKNOWN)
                            wellknownTos.setIsUserConfigured(true);

                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(customPrecedence, true, "customPrecedence"));
                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(wellknownPrecedence, true, "wellknownPrecedence"));
                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(customDscp, true, "customDscp"));
                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(wellknownDscp, true, "wellknownDscp"));
                        queryCtxtForChild.addAndAttribute (new AttributeUI32UC(customTos, true, "customTos"));
                        queryCtxtForChild.addAndAttribute (new AttributeEnumUC(wellknownTos, true, "wellknownTos"));
                        if (l3l4WellknownProtoType == L3L4_PROTOTYPE_TCP) {
                            /* TCP Flags */
                            queryCtxtForChild.addAndAttribute (new AttributeBool(urg, "urg"));
                            queryCtxtForChild.addAndAttribute (new AttributeBool(ack, "ack"));
                            queryCtxtForChild.addAndAttribute (new AttributeBool(push, "push"));
                            queryCtxtForChild.addAndAttribute (new AttributeBool(fin, "fin"));
                            queryCtxtForChild.addAndAttribute (new AttributeBool(rst, "rst"));
                            queryCtxtForChild.addAndAttribute (new AttributeBool(sync, "sync"));
                        }
                    }

                    /* Passing User EnteredSeqid zero */
                    QuerychildValidation(queryCtxtForChild, status, ruleFound, seqFound, opcode, 0, seqid, policyType);
                }
            } else {
                trace(TRACE_LEVEL_INFO,string("Two or more Objects exists"));
                status = WAVE_MESSAGE_ERROR;
            }

//            seqContext->addManagedObjectsForGarbageCollection(*pResultsPMO);
            if (pResultsPMO)
                delete pResultsPMO;
        } else {
                trace(TRACE_LEVEL_INFO,string("Querying Policy Managed Object Failed"));
                /* TODO: Return specific error */
                status = WAVE_MESSAGE_ERROR;
        }

        trace(TRACE_LEVEL_INFO,string("seqid=")+seqid);
        pGlobalAssocIp6RuleToPolicyMessage->setRuleid(seqid);

return (status);
}

    void SsmIp6AclWorker::QuerychildValidation(WaveManagedObjectSynchronousQueryContext &queryCtxtForChild, ResourceId &status, bool &ruleFound, bool &seqFound, UI32 opcode, UI32 UserEnteredSeqid, UI64 &seqid, UI32 policyType) {

        vector<WaveManagedObject *> *pResultsChildMO = querySynchronously (&queryCtxtForChild);
        UI64 tempSeqid = seqid;
        UI32 numberOfResultsChild = pResultsChildMO->size ();

        if (NULL != pResultsChildMO) {
            if (0 == numberOfResultsChild) {
                trace(TRACE_LEVEL_INFO,string("Can't find the Rule Managed Object"));

            } else if (1 == numberOfResultsChild) {
                trace(TRACE_LEVEL_INFO,"Rule already exists");
                if (UserEnteredSeqid) {
                    seqFound = true;
                    trace(TRACE_LEVEL_INFO,"seqFound is true");
                } else {
                    ruleFound = true;
                    trace(TRACE_LEVEL_INFO,"ruleFound is true");
                }

                if (policyType == EXTENDED_IP6_ACL) {
                    ExtIp6AclRuleManagedObject *ExtruleMO = dynamic_cast<ExtIp6AclRuleManagedObject *>((*pResultsChildMO)[0]);
                    tempSeqid = ExtruleMO->getRuleid();
                } else if (policyType == STANDARD_IP6_ACL) {
                    StdIp6AclRuleManagedObject *StdruleMO = dynamic_cast<StdIp6AclRuleManagedObject *>((*pResultsChildMO)[0]);
                    tempSeqid = StdruleMO->getRuleid();
                }
            } else {
                trace(TRACE_LEVEL_INFO,string("Two or more Rules exists"));
                status = WAVE_MESSAGE_ERROR;
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsChildMO);
        }

        if (status == WAVE_MESSAGE_SUCCESS) {
            if (opcode == IP6_ADD_OPER) {
                /* Fix Me : Change Below Logic As NO_RULE_ID is Valid */
                if ((ruleFound) || (seqFound)) {
                    if (seqFound) {
                        status = WRC_SSM_DCM_ERR_ACL_RULE_SEQ_ALREADY_EXISTS;
                    } else {
                        status = WRC_SSM_DCM_ERR_ACL_RULE_ALREADY_EXISTS;
                    }
                    seqid = tempSeqid;
                }
            }
            else {// SSMACL_DCMD_DEL_RULE
                if ((ruleFound) || (seqFound))
                    seqid = tempSeqid;
                if (!((ruleFound) || (seqFound)))
                    status = WRC_SSM_DCM_ERR_ACL_RULE_EXISTS;
            }
        }
        return;
    }


#if 0
// This callback will not be required as AAA will recieve a oneway message
void SsmIp6AclWorker::updateAclRuleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "SsmIp6AclWorker::updateAclRuleSTCCallback() Enter");
	AAAExtAclRuleContext *pAAAContext = (AAAExtAclRuleContext *)pWaveSendToClusterContext->getPCallerContext();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	SsmGlobalAssocIp6RuleToPolicyMessage *pGlobalAssocIp6RuleToPolicyMessage = (SsmGlobalAssocIp6RuleToPolicyMessage*)pAAAContext->getPPrismMessage();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
	status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - ACL operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - ACL operation failed on all nodes");

		vector<LocationId> cxLocations;
		FrameworkToolKit::getConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "SsmIp6AclWorker::updateAclRuleSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? WAVE_MESSAGE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? WAVE_MESSAGE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		processExtIp6AclRules(pGlobalAssocIp6RuleToPolicyMessage);
	} else {
		pGlobalAssocIp6RuleToPolicyMessage->setCompletionStatus(status);
		reply(pGlobalAssocIp6RuleToPolicyMessage);
	}
	
	trace (TRACE_LEVEL_INFO, "SsmIp6AclWorker::updateAclRuleSTCCallback() Exit");
}
#endif

    void SsmIp6AclWorker::updateIpAclRuleToAAA(SsmGlobalAssocIp6RuleToPolicyMessage *pGlobalAssocIp6RuleToPolicyMessage, vector <LocationId> &locVector)
	{
		SsmGlobalAssocIp6RuleToPolicyMessage *pMsg 				= pGlobalAssocIp6RuleToPolicyMessage;
		
		/* Getting Data From Message to Send to Local Plugin */
		UI64 ruleid												= pMsg->getRuleid();
		UI32 action												= pMsg->getAction();
		UI32 policyType											= pMsg->getPolicyType();
		const string policyName									= pMsg->getPolicyName();
		IpAddrType  srcType										= pMsg->getSrcType();
		IpV6AddressNetworkMask srcIpAddress						= pMsg->getSrcIpAddress();
//		string srcIpAddress										= pMsg->getSrcIpAddress();
//		IpV6Address srcIpAddressMask							= pMsg->getSrcIpAddressMask();
		IpV6Address srcHostIpAddress							= pMsg->getSrcHostIpAddress();
//		string srcHostIpAddress									= pMsg->getSrcHostIpAddress();
		WaveUnion srcUnion										= pMsg->getSrcUnion();
		bool isCount											= pMsg->getIsCount();
		bool isLog												= pMsg->getIsLog();
		UI32 opcode												= (UI32) pMsg->getOpcode();
//		ResourceId status										= WAVE_MESSAGE_ERROR;

		trace (TRACE_LEVEL_INFO, string(" SsmIp6AclWorker::updateIpAclRuleToAAA Rule id: ") + ruleid + string ("Policy: ") + policyName);
	
		IpAddrType  dstType 									= pMsg->getDstType();	
		IpV6AddressNetworkMask dstIpAddress 								= pMsg->getDstIpAddress();
//		IpV6Address dstIpAddressMask						 	= pMsg->getDstIpAddressMask();
		IpV6Address dstHostIpAddress						 	= pMsg->getDstHostIpAddress();
		WaveUnion dstUnion 										= pMsg->getDstUnion();
		WaveUnion ProtocolTypeUnion 							= pMsg->getProtocolTypeUnion();
		UI32 l3l4CustomProtoType 								= pMsg->getL3l4CustomProtoType();
		L3L4wellknownProtoType l3l4WellknownProtoType 			= pMsg->getL3l4WellknownProtoType();
		PortOperator srcPortOperator						 	= pMsg->getSrcPortOperator();
		PortOperator dstPortOperator						 	= pMsg->getDstPortOperator();
		WaveUnion srcPortNumberTcpUnion							= pMsg->getSrcPortNumberTcpUnion();
		WaveUnion srcPortltNumberTcpUnion						= pMsg->getSrcPortltNumberTcpUnion();
		WaveUnion srcPortgtNumberTcpUnion						= pMsg->getSrcPortgtNumberTcpUnion();
		WaveUnion dstPortNumberTcpUnion							= pMsg->getDstPortNumberTcpUnion();
		WaveUnion dstPortltNumberTcpUnion						= pMsg->getDstPortltNumberTcpUnion();
		WaveUnion dstPortgtNumberTcpUnion						= pMsg->getDstPortgtNumberTcpUnion();
		WaveUnion srcPortNumberUdpUnion							= pMsg->getSrcPortNumberUdpUnion();
		WaveUnion srcPortltNumberUdpUnion						= pMsg->getSrcPortltNumberUdpUnion();
		WaveUnion srcPortgtNumberUdpUnion						= pMsg->getSrcPortgtNumberUdpUnion();
		WaveUnion dstPortNumberUdpUnion							= pMsg->getDstPortNumberUdpUnion();
		WaveUnion dstPortltNumberUdpUnion						= pMsg->getDstPortltNumberUdpUnion();
		WaveUnion dstPortgtNumberUdpUnion						= pMsg->getDstPortgtNumberUdpUnion();
		WaveUnion srcPortrangeLowerTcpUnion						= pMsg->getSrcPortrangeLowerTcpUnion();
		WaveUnion dstPortrangeLowerTcpUnion						= pMsg->getDstPortrangeLowerTcpUnion();
		WaveUnion srcPortrangeHigherTcpUnion				 	= pMsg->getSrcPortrangeHigherTcpUnion();
		WaveUnion dstPortrangeHigherTcpUnion				 	= pMsg->getDstPortrangeHigherTcpUnion();
		WaveUnion srcPortrangeLowerUdpUnion						= pMsg->getSrcPortrangeLowerUdpUnion();
		WaveUnion dstPortrangeLowerUdpUnion						= pMsg->getDstPortrangeLowerUdpUnion();
		WaveUnion srcPortrangeHigherUdpUnion					= pMsg->getSrcPortrangeHigherUdpUnion();
		WaveUnion dstPortrangeHigherUdpUnion					= pMsg->getDstPortrangeHigherUdpUnion();
		UI32 srcPortCustomNumberTcp								= pMsg->getSrcPortCustomNumberTcp();
		UI32 srcPortltCustomNumberTcp							= pMsg->getSrcPortltCustomNumberTcp();
		UI32 srcPortgtCustomNumberTcp							= pMsg->getSrcPortgtCustomNumberTcp();
		UI32 dstPortCustomNumberTcp								= pMsg->getDstPortCustomNumberTcp();
		UI32 dstPortltCustomNumberTcp							= pMsg->getDstPortltCustomNumberTcp();
		UI32 dstPortgtCustomNumberTcp							= pMsg->getDstPortgtCustomNumberTcp();
		UI32 srcPortCustomNumberUdp								= pMsg->getSrcPortCustomNumberUdp();
		UI32 srcPortltCustomNumberUdp							= pMsg->getSrcPortltCustomNumberUdp();
		UI32 srcPortgtCustomNumberUdp							= pMsg->getSrcPortgtCustomNumberUdp();
		UI32 dstPortCustomNumberUdp								= pMsg->getDstPortCustomNumberUdp();
		UI32 dstPortltCustomNumberUdp							= pMsg->getDstPortltCustomNumberUdp();
		UI32 dstPortgtCustomNumberUdp							= pMsg->getDstPortgtCustomNumberUdp();
		PortWellknownNumberTcp srcPortWellknownNumberTcp		= pMsg->getSrcPortWellknownNumberTcp();
		PortWellknownNumberTcp srcPortltWellknownNumberTcp		= pMsg->getSrcPortltWellknownNumberTcp();
		PortWellknownNumberTcp srcPortgtWellknownNumberTcp		= pMsg->getSrcPortgtWellknownNumberTcp();
		PortWellknownNumberTcp dstPortWellknownNumberTcp		= pMsg->getDstPortWellknownNumberTcp();
		PortWellknownNumberTcp dstPortltWellknownNumberTcp		= pMsg->getDstPortltWellknownNumberTcp();
		PortWellknownNumberTcp dstPortgtWellknownNumberTcp		= pMsg->getDstPortgtWellknownNumberTcp();
		PortWellknownNumberUdp srcPortWellknownNumberUdp		= pMsg->getSrcPortWellknownNumberUdp();
		PortWellknownNumberUdp srcPortltWellknownNumberUdp		= pMsg->getSrcPortltWellknownNumberUdp();
		PortWellknownNumberUdp srcPortgtWellknownNumberUdp		= pMsg->getSrcPortgtWellknownNumberUdp();
		PortWellknownNumberUdp dstPortWellknownNumberUdp		= pMsg->getDstPortWellknownNumberUdp();
		PortWellknownNumberUdp dstPortltWellknownNumberUdp		= pMsg->getDstPortltWellknownNumberUdp();
		PortWellknownNumberUdp dstPortgtWellknownNumberUdp		= pMsg->getDstPortgtWellknownNumberUdp();
		UI32 srcPortCustomrangeLowerTcp							= pMsg->getSrcPortCustomrangeLowerTcp();
		UI32 dstPortCustomrangeLowerTcp							= pMsg->getDstPortCustomrangeLowerTcp();
		UI32 srcPortCustomrangeLowerUdp							= pMsg->getSrcPortCustomrangeLowerUdp();
		UI32 dstPortCustomrangeLowerUdp							= pMsg->getDstPortCustomrangeLowerUdp();
		UI32 srcPortCustomrangeHigherTcp 						= pMsg->getSrcPortCustomrangeHigherTcp();
		UI32 dstPortCustomrangeHigherTcp 						= pMsg->getDstPortCustomrangeHigherTcp();
		UI32 srcPortCustomrangeHigherUdp 						= pMsg->getSrcPortCustomrangeHigherUdp();
		UI32 dstPortCustomrangeHigherUdp 						= pMsg->getDstPortCustomrangeHigherUdp();
		PortWellknownNumberUdp srcPortWellknownrangeLowerUdp 	= pMsg->getSrcPortWellknownrangeLowerUdp();
		PortWellknownNumberUdp dstPortWellknownrangeLowerUdp 	= pMsg->getDstPortWellknownrangeLowerUdp();
		PortWellknownNumberTcp srcPortWellknownrangeLowerTcp 	= pMsg->getSrcPortWellknownrangeLowerTcp();
		PortWellknownNumberTcp dstPortWellknownrangeLowerTcp 	= pMsg->getDstPortWellknownrangeLowerTcp();
		PortWellknownNumberUdp srcPortWellknownrangeHigherUdp	= pMsg->getSrcPortWellknownrangeHigherUdp();
		PortWellknownNumberUdp dstPortWellknownrangeHigherUdp	= pMsg->getDstPortWellknownrangeHigherUdp();
		PortWellknownNumberTcp srcPortWellknownrangeHigherTcp	= pMsg->getSrcPortWellknownrangeHigherTcp();
		PortWellknownNumberTcp dstPortWellknownrangeHigherTcp	= pMsg->getDstPortWellknownrangeHigherTcp();
		WellknownPrecedence wellknownPrecedence                 = pMsg->getWellknownPrecedence();
		WellknownTos wellknownTos                               = pMsg->getWellknownTos();
		WellknownDscp wellknownDscp                             = pMsg->getWellknownDscp();
		UI32  customPrecedence                                  = pMsg->getCustomPrecedence();
		UI32  customTos                                         = pMsg->getCustomTos();
		UI32  customDscp                                        = pMsg->getCustomDscp();
		WaveUnion precedenceUnion								= pMsg->getPrecedenceUnion();
		WaveUnion tosUnion										= pMsg->getTosUnion();
		WaveUnion dscpUnion										= pMsg->getDscpUnion();	
		bool urg											 	= pMsg->getUrg();
		bool ack											 	= pMsg->getAck();
		bool push											 	= pMsg->getPush();
		bool fin											 	= pMsg->getFin();
		bool rst											 	= pMsg->getRst();
		bool sync								  				= pMsg->getSync();
		UI8	 userEnteredSeqid									= pMsg->getUserEnteredSeqid();
			
        AAALocalAssocIp6RuleToPolicyMessage *newMsg =
			new AAALocalAssocIp6RuleToPolicyMessage(policyName, policyType, ruleid, action, 
				srcUnion, srcType, srcHostIpAddress, srcIpAddress,
// srcIpAddressMask,
				 isCount, 
				isLog, dstHostIpAddress, dstIpAddress, 
//dstIpAddressMask
				dstUnion, dstType, 
				ProtocolTypeUnion, l3l4CustomProtoType, l3l4WellknownProtoType, srcPortOperator, 
				srcPortNumberTcpUnion, srcPortltNumberTcpUnion, srcPortgtNumberTcpUnion, 
				srcPortNumberUdpUnion, srcPortltNumberUdpUnion, srcPortgtNumberUdpUnion, 
				srcPortrangeLowerTcpUnion, srcPortrangeLowerUdpUnion, srcPortrangeHigherTcpUnion, 
				srcPortrangeHigherUdpUnion, srcPortCustomNumberTcp, srcPortltCustomNumberTcp, 
				srcPortgtCustomNumberTcp, srcPortWellknownNumberTcp, srcPortltWellknownNumberTcp, 
				srcPortgtWellknownNumberTcp, srcPortCustomNumberUdp, srcPortltCustomNumberUdp,
				srcPortgtCustomNumberUdp, srcPortWellknownNumberUdp, 
				srcPortltWellknownNumberUdp, srcPortgtWellknownNumberUdp,
				srcPortCustomrangeLowerTcp, srcPortWellknownrangeLowerTcp, 
				srcPortCustomrangeHigherTcp, srcPortWellknownrangeHigherTcp, srcPortCustomrangeLowerUdp, 
				srcPortWellknownrangeLowerUdp, srcPortCustomrangeHigherUdp, srcPortWellknownrangeHigherUdp, 
				dstPortNumberTcpUnion, dstPortltNumberTcpUnion, dstPortgtNumberTcpUnion,
				dstPortNumberUdpUnion, dstPortltNumberUdpUnion, dstPortgtNumberUdpUnion,
				dstPortrangeLowerTcpUnion, dstPortrangeLowerUdpUnion, 
				dstPortrangeHigherTcpUnion, dstPortrangeHigherUdpUnion, dstPortOperator, 
				dstPortCustomNumberTcp, dstPortltCustomNumberTcp, dstPortgtCustomNumberTcp,
				dstPortWellknownNumberTcp, dstPortltWellknownNumberTcp, dstPortgtWellknownNumberTcp,
				dstPortCustomNumberUdp, dstPortltCustomNumberUdp, dstPortgtCustomNumberUdp,
				dstPortWellknownNumberUdp, dstPortltWellknownNumberUdp, dstPortgtWellknownNumberUdp, 
				dstPortCustomrangeLowerTcp, dstPortWellknownrangeLowerTcp, dstPortCustomrangeHigherTcp, 
				dstPortWellknownrangeHigherTcp, dstPortCustomrangeLowerUdp, dstPortWellknownrangeLowerUdp, 
				dstPortCustomrangeHigherUdp, dstPortWellknownrangeHigherUdp, wellknownPrecedence, wellknownTos, 
				wellknownDscp, customPrecedence, customTos, customDscp, precedenceUnion, tosUnion, dscpUnion,
				urg, ack, push, fin, rst, sync, opcode, userEnteredSeqid);
        
		if (newMsg != NULL) {
			trace (TRACE_LEVEL_INFO, string("message being sent to Local - Policyname: ") + policyName + string("policy Type: ") + policyType + string("Ruleid: ") + ruleid + string ("action: ") + action + srcPortltCustomNumberTcp + srcPortgtCustomNumberTcp + dstPortgtCustomNumberTcp + srcPortltCustomNumberTcp + action + string ("customTos: ")+ customTos + string ("wellknownTos: ") + wellknownTos + string ("customDscp: ") + customDscp + string ("wellknownDscp: ") + wellknownDscp + string ("customPrecedence :") + customPrecedence + string ("wellknownPrecedence :") + wellknownPrecedence);

			/*
			 * As was done in ipv4 ACL, SSM should not wait for AAA plugins due to timing issues, so sendoneway, and let AAA handle its errors
			 * sendoneway dsnt accept vector of location, so snding in a loop where this ACL is applied on Mgmt interface.
			 */
			int duplicate = 0;
			for (UI32 i = 0; i < locVector.size(); i++)
			{
				duplicate = 0;
				for(UI32 k = 0; k < i; k++)
				{
					if (locVector[k] == locVector[i])
						{duplicate = 1;}
				}
				if (duplicate != 1) {
					ResourceId aaastatus = sendOneWay(newMsg, locVector[i]);
					trace (TRACE_LEVEL_INFO, string("SSM notified AAA of rule change Oneway") + aaastatus);
				}
			}		

/*
			AAAExtAclRuleContext *pAclRuleContext = new AAAExtAclRuleContext (newMsg);

			WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&SsmIp6AclWorker::updateAclRuleSTCCallback), pAclRuleContext);
			pWaveSendToClusterContext->setPPrismMessageForPhase1(newMsg);
			pWaveSendToClusterContext->setPartialSuccessFlag(true);
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locVector);
			sendToWaveCluster (pWaveSendToClusterContext);
*/
		}

	}
    

    
	void SsmIp6AclWorker::assocIpAclRuleToPolicy(SsmGlobalAssocIp6RuleToPolicyMessage *pGlobalAssocIp6RuleToPolicyMessage, Ip6AclPolicyManagedObject *pIp6AclPolicyManagedObject)
	{

		SsmGlobalAssocIp6RuleToPolicyMessage *pMsg = pGlobalAssocIp6RuleToPolicyMessage;
		UI64 ruleid 								= pMsg->getRuleid();
		UI32 action 								= pMsg->getAction();
		UI32 policyType 							= pMsg->getPolicyType();
		const string policyName 					= pMsg->getPolicyName();
		const IpV6Address HostIpAddress 			= pMsg->getSrcHostIpAddress();
		const IpV6AddressNetworkMask srcIpAddress 	= pMsg->getSrcIpAddress();
		const IpV6Address srcHostIpAddress			= pMsg->getSrcHostIpAddress();
/*
		const string HostIpAddress             = pMsg->getSrcHostIpAddress();
		const string srcIpAddress              = pMsg->getSrcIpAddress();
		const string srcHostIpAddress          = pMsg->getSrcHostIpAddress();
*/
//		const IpV6Address srcIpAddressMask			= pMsg->getSrcIpAddressMask();
		IpAddrType srcType						   	= pMsg->getSrcType();
		WaveUnion srcUnion						   	= pMsg->getSrcUnion();
		bool isCount 								= pMsg->getIsCount();
		bool isLog 									= pMsg->getIsLog();
		ResourceId status 							= WAVE_MESSAGE_ERROR;
       	//IpAclPolicyManagedObject *policyMO 			= NULL; 
		Ip6AclRuleManagedObject *ruleMO 				= NULL;
		string className;
		string childClassName;
		string compositionName;

		trace(TRACE_LEVEL_INFO, "SsmIp6AclWorker::assocIpAclRuleToPolicy" + string("Rule id: ") + ruleid + string ("Policy: ") + policyName + policyType);;
		
        WaveNs::trace(TRACE_LEVEL_INFO, string("SsmIp6AclWorker::assocIpAclRuleToPolicy srcHostIpAddress = ") + srcHostIpAddress.toString() + string(", srcIpAddress = ") + srcIpAddress.toString());

		startTransaction ();

		/* Update Policy MO with the respective Std/Ext Rule Composition  */
		if (policyType == STANDARD_IP6_ACL) {
			className = StdIp6AclPolicyManagedObject::getClassName();
			ruleMO 	= new StdIp6AclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager*>(getPWaveObjectManager()));
			childClassName 	= StdIp6AclRuleManagedObject::getClassName();
			compositionName = "rules";
		} else if (policyType == EXTENDED_IP6_ACL) {
			className = ExtIp6AclPolicyManagedObject::getClassName();
			ruleMO = new ExtIp6AclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager*>(getPWaveObjectManager()));
			childClassName = ExtIp6AclRuleManagedObject::getClassName();
			compositionName = "extRules";
		} else {
			className = Ip6AclPolicyManagedObject::getClassName();
			status = WAVE_MESSAGE_ERROR;
		}

		if (ruleMO == NULL) {
			status = WAVE_MESSAGE_ERROR;
		} else {
			ruleMO->setOwnerManagedObjectId(pIp6AclPolicyManagedObject->getObjectId());
			addToComposition(className, childClassName,
						compositionName, pIp6AclPolicyManagedObject->getObjectId(), ruleMO->getObjectId());
			trace(TRACE_LEVEL_INFO, "Successfully updated the IPv4Policy MO with the rule");
			status = WAVE_MESSAGE_SUCCESS;
		}

		if (status == WAVE_MESSAGE_SUCCESS) {
			ruleMO->setRuleid(ruleid);
			ruleMO->setAction(action);
			ruleMO->setSrcType(srcType);
			ruleMO->setSrcHostIpAddress(srcHostIpAddress);
			ruleMO->setSrcIpAddress(srcIpAddress);
//			ruleMO->setSrcIpAddressMask(srcIpAddressMask);
			ruleMO->setSrcUnion(srcUnion); 
			ruleMO->setIsCount(isCount);
			ruleMO->setIsLog(isLog);
			
			if (policyType == EXTENDED_IP6_ACL) {
				IpAddrType  dstType 										= pMsg->getDstType();	
				IpV6AddressNetworkMask dstIpAddress 									= pMsg->getDstIpAddress();
//				IpV6Address dstIpAddressMask 								= pMsg->getDstIpAddressMask();
				IpV6Address dstHostIpAddress 								= pMsg->getDstHostIpAddress();
				WaveUnion dstUnion 											= pMsg->getDstUnion();
				WaveUnion ProtocolTypeUnion 								= pMsg->getProtocolTypeUnion();
				UI32 l3l4CustomProtoType 									= pMsg->getL3l4CustomProtoType();
				L3L4wellknownProtoType l3l4WellknownProtoType 				= pMsg->getL3l4WellknownProtoType();
				PortOperator srcPortOperator 								= pMsg->getSrcPortOperator();
				PortOperator dstPortOperator 								= pMsg->getDstPortOperator();
				WaveUnion srcPortNumberTcpUnion								= pMsg->getSrcPortNumberTcpUnion();
				WaveUnion srcPortltNumberTcpUnion                      		= pMsg->getSrcPortltNumberTcpUnion();
				WaveUnion srcPortgtNumberTcpUnion                       	= pMsg->getSrcPortgtNumberTcpUnion();

	
				WaveUnion dstPortNumberTcpUnion								= pMsg->getDstPortNumberTcpUnion();
				WaveUnion dstPortltNumberTcpUnion							= pMsg->getDstPortltNumberTcpUnion();
				WaveUnion dstPortgtNumberTcpUnion							= pMsg->getDstPortgtNumberTcpUnion();
				WaveUnion srcPortNumberUdpUnion								= pMsg->getSrcPortNumberUdpUnion();

				WaveUnion srcPortltNumberUdpUnion							= pMsg->getSrcPortltNumberUdpUnion();
				WaveUnion srcPortgtNumberUdpUnion							= pMsg->getSrcPortgtNumberUdpUnion();
				

				WaveUnion dstPortNumberUdpUnion								= pMsg->getDstPortNumberUdpUnion();

        		WaveUnion dstPortltNumberUdpUnion							= pMsg->getDstPortltNumberUdpUnion();
        		WaveUnion dstPortgtNumberUdpUnion							= pMsg->getDstPortgtNumberUdpUnion();

				WaveUnion srcPortrangeLowerTcpUnion							= pMsg->getSrcPortrangeLowerTcpUnion();
				WaveUnion dstPortrangeLowerTcpUnion							= pMsg->getDstPortrangeLowerTcpUnion();
				WaveUnion srcPortrangeHigherTcpUnion					 	= pMsg->getSrcPortrangeHigherTcpUnion();
				WaveUnion dstPortrangeHigherTcpUnion					 	= pMsg->getDstPortrangeHigherTcpUnion();
				WaveUnion srcPortrangeLowerUdpUnion							= pMsg->getSrcPortrangeLowerUdpUnion();
				WaveUnion dstPortrangeLowerUdpUnion							= pMsg->getDstPortrangeLowerUdpUnion();
				WaveUnion srcPortrangeHigherUdpUnion					 	= pMsg->getSrcPortrangeHigherUdpUnion();
				WaveUnion dstPortrangeHigherUdpUnion					 	= pMsg->getDstPortrangeHigherUdpUnion();


				UI32 srcPortCustomNumberTcp									= pMsg->getSrcPortCustomNumberTcp();
				UI32 srcPortltCustomNumberTcp								= pMsg->getSrcPortltCustomNumberTcp();
				UI32 srcPortgtCustomNumberTcp								= pMsg->getSrcPortgtCustomNumberTcp();
				UI32 dstPortCustomNumberTcp									= pMsg->getDstPortCustomNumberTcp();
				UI32 dstPortltCustomNumberTcp								= pMsg->getDstPortltCustomNumberTcp();
				UI32 dstPortgtCustomNumberTcp								= pMsg->getDstPortgtCustomNumberTcp();
				UI32 srcPortCustomNumberUdp									= pMsg->getSrcPortCustomNumberUdp();
				UI32 srcPortltCustomNumberUdp								= pMsg->getSrcPortltCustomNumberUdp();
				UI32 srcPortgtCustomNumberUdp								= pMsg->getSrcPortgtCustomNumberUdp();
				UI32 dstPortCustomNumberUdp									= pMsg->getDstPortCustomNumberUdp();
				UI32 dstPortltCustomNumberUdp								= pMsg->getDstPortltCustomNumberUdp();
				UI32 dstPortgtCustomNumberUdp								= pMsg->getDstPortgtCustomNumberUdp();
				PortWellknownNumberTcp srcPortWellknownNumberTcp			= pMsg->getSrcPortWellknownNumberTcp();
				PortWellknownNumberTcp srcPortltWellknownNumberTcp			= pMsg->getSrcPortltWellknownNumberTcp();
				PortWellknownNumberTcp srcPortgtWellknownNumberTcp			= pMsg->getSrcPortgtWellknownNumberTcp();
				PortWellknownNumberTcp dstPortWellknownNumberTcp			= pMsg->getDstPortWellknownNumberTcp();
				PortWellknownNumberTcp dstPortltWellknownNumberTcp			= pMsg->getDstPortltWellknownNumberTcp();
				PortWellknownNumberTcp dstPortgtWellknownNumberTcp			= pMsg->getDstPortgtWellknownNumberTcp();
				PortWellknownNumberUdp srcPortWellknownNumberUdp			= pMsg->getSrcPortWellknownNumberUdp();
				PortWellknownNumberUdp srcPortltWellknownNumberUdp			= pMsg->getSrcPortltWellknownNumberUdp();
				PortWellknownNumberUdp srcPortgtWellknownNumberUdp			= pMsg->getSrcPortgtWellknownNumberUdp();
				PortWellknownNumberUdp dstPortWellknownNumberUdp			= pMsg->getDstPortWellknownNumberUdp();
				PortWellknownNumberUdp dstPortltWellknownNumberUdp			= pMsg->getDstPortltWellknownNumberUdp();
				PortWellknownNumberUdp dstPortgtWellknownNumberUdp			= pMsg->getDstPortgtWellknownNumberUdp();

				UI32 srcPortCustomrangeLowerTcp 							= pMsg->getSrcPortCustomrangeLowerTcp();
				UI32 dstPortCustomrangeLowerTcp								= pMsg->getDstPortCustomrangeLowerTcp();
				UI32 srcPortCustomrangeLowerUdp								= pMsg->getSrcPortCustomrangeLowerUdp();
				UI32 dstPortCustomrangeLowerUdp								= pMsg->getDstPortCustomrangeLowerUdp();
				UI32 srcPortCustomrangeHigherTcp							= pMsg->getSrcPortCustomrangeHigherTcp();
				UI32 dstPortCustomrangeHigherTcp							= pMsg->getDstPortCustomrangeHigherTcp();
				UI32 srcPortCustomrangeHigherUdp							= pMsg->getSrcPortCustomrangeHigherUdp();
				UI32 dstPortCustomrangeHigherUdp							= pMsg->getDstPortCustomrangeHigherUdp();
				PortWellknownNumberUdp srcPortWellknownrangeLowerUdp	 	= pMsg->getSrcPortWellknownrangeLowerUdp();
				PortWellknownNumberUdp dstPortWellknownrangeLowerUdp	 	= pMsg->getDstPortWellknownrangeLowerUdp();
				PortWellknownNumberTcp srcPortWellknownrangeLowerTcp	 	= pMsg->getSrcPortWellknownrangeLowerTcp();
				PortWellknownNumberTcp dstPortWellknownrangeLowerTcp	 	= pMsg->getDstPortWellknownrangeLowerTcp();
				PortWellknownNumberUdp srcPortWellknownrangeHigherUdp	 	= pMsg->getSrcPortWellknownrangeHigherUdp();
				PortWellknownNumberUdp dstPortWellknownrangeHigherUdp	 	= pMsg->getDstPortWellknownrangeHigherUdp();
				PortWellknownNumberTcp srcPortWellknownrangeHigherTcp	 	= pMsg->getSrcPortWellknownrangeHigherTcp();
				PortWellknownNumberTcp dstPortWellknownrangeHigherTcp	 	= pMsg->getDstPortWellknownrangeHigherTcp();
                WellknownPrecedence wellknownPrecedence                 	= pMsg->getWellknownPrecedence();
                WellknownTos wellknownTos                               	= pMsg->getWellknownTos();
                WellknownDscp wellknownDscp                             	= pMsg->getWellknownDscp();
                UI32  customPrecedence                                  	= pMsg->getCustomPrecedence();
                UI32  customTos                                         	= pMsg->getCustomTos();
                UI32  customDscp                                        	= pMsg->getCustomDscp();
				WaveUnion precedenceUnion									= pMsg->getPrecedenceUnion();
				WaveUnion dscpUnion											= pMsg->getDscpUnion();
				WaveUnion tosUnion											= pMsg->getTosUnion();
				bool urg												 	= pMsg->getUrg();
				bool ack												 	= pMsg->getAck();
				bool push												 	= pMsg->getPush();
				bool fin												 	= pMsg->getFin();
				bool rst												 	= pMsg->getRst();
				bool sync												 	= pMsg->getSync();
				
				/* Assigning  Values to MO fields */	
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstType(dstType);
				if (dstType == IP_ADDR_TYPE_HOST) {
					dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstHostIpAddress(dstHostIpAddress);
				} else if (dstType == IP_ADDR_TYPE_NONE) {
					dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstIpAddress(dstIpAddress);
//					dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstIpAddressMask(dstIpAddressMask);
				}	
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstUnion(dstUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setProtocolTypeUnion(ProtocolTypeUnion);
				if (ProtocolTypeUnion.compare("l3l4CustomProtoType") == 0) {
					dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setL3l4CustomProtoType(l3l4CustomProtoType);
				} else if (ProtocolTypeUnion.compare("l3l4WellknownProtoType") == 0){
					dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setL3l4WellknownProtoType(l3l4WellknownProtoType);
				}
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortOperator(srcPortOperator);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortOperator(dstPortOperator);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortNumberTcpUnion(srcPortNumberTcpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortltNumberTcpUnion(srcPortltNumberTcpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortgtNumberTcpUnion(srcPortgtNumberTcpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortNumberTcpUnion(dstPortNumberTcpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortltNumberTcpUnion(dstPortltNumberTcpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortgtNumberTcpUnion(dstPortgtNumberTcpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortNumberUdpUnion(srcPortNumberUdpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortltNumberUdpUnion(srcPortltNumberUdpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortgtNumberUdpUnion(srcPortgtNumberUdpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortNumberUdpUnion(dstPortNumberUdpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortltNumberUdpUnion(dstPortltNumberUdpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortgtNumberUdpUnion(dstPortgtNumberUdpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortrangeLowerTcpUnion(srcPortrangeLowerTcpUnion);	 
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortrangeLowerTcpUnion(dstPortrangeLowerTcpUnion);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortrangeHigherTcpUnion(srcPortrangeHigherTcpUnion);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortrangeHigherTcpUnion(dstPortrangeHigherTcpUnion);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortrangeLowerUdpUnion(srcPortrangeLowerUdpUnion);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortrangeLowerUdpUnion(dstPortrangeLowerUdpUnion);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortrangeHigherUdpUnion(srcPortrangeHigherUdpUnion);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortrangeHigherUdpUnion(dstPortrangeHigherUdpUnion);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortCustomNumberTcp(srcPortCustomNumberTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortltCustomNumberTcp(srcPortltCustomNumberTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortgtCustomNumberTcp(srcPortgtCustomNumberTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortCustomNumberTcp(dstPortCustomNumberTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortltCustomNumberTcp(dstPortltCustomNumberTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortgtCustomNumberTcp(dstPortgtCustomNumberTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortCustomNumberUdp(srcPortCustomNumberUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortltCustomNumberUdp(srcPortltCustomNumberUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortgtCustomNumberUdp(srcPortgtCustomNumberUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortCustomNumberUdp(dstPortCustomNumberUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortltCustomNumberUdp(dstPortltCustomNumberUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortgtCustomNumberUdp(dstPortgtCustomNumberUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortWellknownNumberTcp(srcPortWellknownNumberTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortltWellknownNumberTcp(srcPortltWellknownNumberTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortgtWellknownNumberTcp(srcPortgtWellknownNumberTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortWellknownNumberTcp(dstPortWellknownNumberTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortltWellknownNumberTcp(dstPortltWellknownNumberTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortgtWellknownNumberTcp(dstPortgtWellknownNumberTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortWellknownNumberUdp(srcPortWellknownNumberUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortltWellknownNumberUdp(srcPortltWellknownNumberUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortgtWellknownNumberUdp(srcPortgtWellknownNumberUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortWellknownNumberUdp(dstPortWellknownNumberUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortltWellknownNumberUdp(dstPortltWellknownNumberUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortgtWellknownNumberUdp(dstPortgtWellknownNumberUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortCustomrangeLowerTcp(srcPortCustomrangeLowerTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortCustomrangeLowerTcp(dstPortCustomrangeLowerTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortCustomrangeLowerUdp(srcPortCustomrangeLowerUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortCustomrangeLowerUdp(dstPortCustomrangeLowerUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortCustomrangeHigherTcp(srcPortCustomrangeHigherTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortCustomrangeHigherTcp(dstPortCustomrangeHigherTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortCustomrangeHigherUdp(srcPortCustomrangeHigherUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortCustomrangeHigherUdp(dstPortCustomrangeHigherUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortWellknownrangeLowerUdp(srcPortWellknownrangeLowerUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortWellknownrangeLowerUdp(dstPortWellknownrangeLowerUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortWellknownrangeLowerTcp(srcPortWellknownrangeLowerTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortWellknownrangeLowerTcp(dstPortWellknownrangeLowerTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortWellknownrangeHigherUdp(srcPortWellknownrangeHigherUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortWellknownrangeHigherUdp(dstPortWellknownrangeHigherUdp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSrcPortWellknownrangeHigherTcp(srcPortWellknownrangeHigherTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDstPortWellknownrangeHigherTcp(dstPortWellknownrangeHigherTcp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setCustomTos(customTos);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setCustomPrecedence(customPrecedence);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setCustomDscp(customDscp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setWellknownPrecedence(wellknownPrecedence);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setWellknownDscp(wellknownDscp);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setWellknownTos(wellknownTos);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setPrecedenceUnion(precedenceUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setDscpUnion(dscpUnion);
				dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setTosUnion(tosUnion);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setUrg(urg);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setAck(ack);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setPush(push);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setFin(fin);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setRst(rst);
                dynamic_cast <ExtIp6AclRuleManagedObject *>(ruleMO)->setSync(sync);
			}
			
        	status = commitTransaction ();
        	if (status != FRAMEWORK_SUCCESS) {
        		trace(TRACE_LEVEL_INFO, string("Commit transaction failed: ") + status);
       			status = WAVE_MESSAGE_ERROR;
        	} else {
        		trace(TRACE_LEVEL_INFO, "Commit Succeeded");
       			status = WAVE_MESSAGE_SUCCESS;
			}
		}

		delete (ruleMO);

	}

	ResourceId SsmIp6AclWorker::verifyIPMACPolicyexists(string policyName)
	{
		trace (TRACE_LEVEL_INFO, "Enter SsmIp6AclWorker::verifyIPMACexists check");
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IpAclPolicyManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults)
		{
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults)
			{
				trace (TRACE_LEVEL_INFO, "No IP policy with same name exists");
			}
			else
			{
				trace (TRACE_LEVEL_INFO, "Two or more object Exists");
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				return (WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS);
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}

		// check for the presence of any MAC ACL
		WaveManagedObjectSynchronousQueryContext syncQueryCtxtMac(MacAclPolicyManagedObject::getClassName());
		syncQueryCtxtMac.addAndAttribute (new AttributeString (policyName, "policyName"));
		pResults = querySynchronously (&syncQueryCtxtMac);
		if ((NULL != pResults) && (pResults->size () > 0))
		{
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			trace (TRACE_LEVEL_INFO, "MAC ACL with same name exists");
			return (WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS_MAC);
		}
		trace (TRACE_LEVEL_INFO, "SsmIp6AclWorker::verifyIPMACexists check -Exit");
		return (WAVE_MESSAGE_SUCCESS);
	}

}
