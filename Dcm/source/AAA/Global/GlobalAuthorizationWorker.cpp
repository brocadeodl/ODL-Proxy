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
 *   Author : kghanta                                                     *
 **************************************************************************/


#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "DcmResourceIdEnums.h"


#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/GlobalAuthenticationWorker.h"
#include "AAA/Global/UserManagedObject.h"
#include "AAA/Global/RoleManagedObject.h"
#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Global/AAAGlobalAddUserMessage.h"
#include "AAA/Global/AAAGlobalChangeUserMessage.h"
#include "AAA/Global/AAAGlobalDeleteUserMessage.h"
#include "AAA/Global/AAAGlobalAddRoleMessage.h"
#include "AAA/Global/AAAGlobalChangeRoleMessage.h"
#include "AAA/Global/AAAGlobalDeleteRoleMessage.h"
#include "AAA/Local/AAALocalAddUserMessage.h"
#include "AAA/Local/AAALocalChangeUserMessage.h"
#include "AAA/Local/AAALocalDeleteUserMessage.h"
#include "AAA/Local/AAALocalAddRoleMessage.h"
#include "AAA/Local/AAALocalChangeRoleMessage.h"
#include "AAA/Local/AAALocalDeleteRoleMessage.h"
#include "AAA/Local/AAALocalCleanCacheMessage.h"
#include "AAA/Global/AAASequencerContexts.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "AAA/Global/GlobalAuthorizationWorker.h"
#include "AAA/Global/AAAGlobalAddRuleMessage.h"
#include "AAA/Global/AAAGlobalChangeRuleMessage.h"
#include "AAA/Global/AAAGlobalDeleteRuleMessage.h"
#include "AAA/Global/RuleManagedObject.h"
#include "AAA/Global/DataRuleManagedObject.h"
#include "AAA/Global/CmdRuleManagedObject.h"
#include "DcmCore/DcmToolKit.h"
extern "C" {
	#include "security/public.h"
}

namespace DcmNs
{

	GlobalAuthorizationWorker::GlobalAuthorizationWorker ( AAAGlobalObjectManager *pAAAGlobalObjectManager)
		: WaveWorker  (pAAAGlobalObjectManager)
	{
		RuleManagedObject    ruleMO    (pAAAGlobalObjectManager);
        ruleMO.setupOrm ();
        addManagedClass (RuleManagedObject::getClassName (), this);

        CmdRuleManagedObject    cmdRuleMO    (pAAAGlobalObjectManager);
        cmdRuleMO.setupOrm ();
        addManagedClass (CmdRuleManagedObject::getClassName (), this);

		DataRuleManagedObject	dataRuleMO    (pAAAGlobalObjectManager);
        dataRuleMO.setupOrm ();
        addManagedClass (DataRuleManagedObject::getClassName (), this);

        addOperationMap (AAAGLOBALADDRULE, reinterpret_cast<PrismMessageHandler> (&GlobalAuthorizationWorker::AAAGlobalAddRuleMessageHandler));
        addOperationMap (AAAGLOBALCHANGERULE, reinterpret_cast<PrismMessageHandler> (&GlobalAuthorizationWorker::AAAGlobalChangeRuleMessageHandler));
        addOperationMap (AAAGLOBALDELETERULE, reinterpret_cast<PrismMessageHandler> (&GlobalAuthorizationWorker::AAAGlobalDeleteRuleMessageHandler));
    }

    GlobalAuthorizationWorker::~GlobalAuthorizationWorker ()
    {
    }

    PrismMessage  *GlobalAuthorizationWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case AAAGLOBALADDRULE :
                pPrismMessage = new AAAGlobalAddRuleMessage ();
                break;
            case AAAGLOBALCHANGERULE :
                pPrismMessage = new AAAGlobalChangeRuleMessage ();
                break;
            case AAAGLOBALDELETERULE :
                pPrismMessage = new AAAGlobalDeleteRuleMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *GlobalAuthorizationWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((RuleManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RuleManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
        }
		else if ((CmdRuleManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new CmdRuleManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
		}
		else if ((DataRuleManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new DataRuleManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
		}
        else
        {
            trace (TRACE_LEVEL_FATAL, "GlobalAuthorizationWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  GlobalAuthorizationWorker::AAAGlobalAddRuleMessageHandler( AAAGlobalAddRuleMessage *pAAAGlobalAddRuleMessage)
    {
		ResourceId status;
		DataRuleManagedObject *pDataRuleObj = NULL;
		CmdRuleManagedObject *pCmdRuleObj = NULL;
		if (pAAAGlobalAddRuleMessage == NULL)
		{
			trace (TRACE_LEVEL_ERROR, "Null message arrived in the handler.");
			prismAssert (false, __FILE__, __LINE__);
		}
		UI32 index = pAAAGlobalAddRuleMessage->getIndex();
		RBACOperation operation = pAAAGlobalAddRuleMessage->getOperation();
		vector<string> command = pAAAGlobalAddRuleMessage->getCommand();
		vector<string> keypath = pAAAGlobalAddRuleMessage->getKeypath();
		string context = pAAAGlobalAddRuleMessage->getContext();
		RBACAction action = pAAAGlobalAddRuleMessage->getAction();
		vector<UI32> locations;

		if (index < MIN_RULE_INDEX || index > MAX_RULE_INDEX) {
			trace(TRACE_LEVEL_ERROR, "Rule index is not in the  valid range.");
			pAAAGlobalAddRuleMessage->setCompletionStatus(RULE_INDEX_INVALID);
			reply (pAAAGlobalAddRuleMessage);
			return;
		}

		if ((pAAAGlobalAddRuleMessage->getRole().compare(DEFAULT_ADMIN_ROLE) == 0) || (pAAAGlobalAddRuleMessage->getRole().compare(DEFAULT_USER_ROLE) == 0)) {
			trace(TRACE_LEVEL_ERROR, "Rules cannot be set for predefined roles");
			pAAAGlobalAddRuleMessage->setCompletionStatus(RULE_SET_PREDEFINED_ROLE);
			reply (pAAAGlobalAddRuleMessage);
			return;
		}
		WaveManagedObjectSynchronousQueryContext *synchronousQueryContextRule = new WaveManagedObjectSynchronousQueryContext(RuleManagedObject::getClassName());
		vector<WaveManagedObject *> *pResultsRule = querySynchronously (synchronousQueryContextRule);
		if (NULL == pResultsRule)
		{
			trace (TRACE_LEVEL_ERROR, "No DB Table for Rule ManagedObject");

			pAAAGlobalAddRuleMessage->setCompletionStatus(RULE_ADD_ERROR);
			reply (pAAAGlobalAddRuleMessage);
			delete synchronousQueryContextRule;
			return;
		}
		else
		{
			unsigned int ruleSize = pResultsRule->size();
			delete synchronousQueryContextRule;
			for (UI32 i = 0; i < ruleSize; i++)
				delete (*pResultsRule)[i];
			delete pResultsRule;
			if (ruleSize == RULE_MAX_NUM)
			{
			trace (TRACE_LEVEL_ERROR, "Maximum Rules configured");
			pAAAGlobalAddRuleMessage->setCompletionStatus(RULE_MAX_LIMIT);
			reply (pAAAGlobalAddRuleMessage);
			return;
			}
		}
		WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RoleManagedObject::getClassName());
		synchronousQueryContext->addAndAttribute(new AttributeString(pAAAGlobalAddRuleMessage->getRole(), "rolename"));
		vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);

		if ((NULL == pResults) || (pResults->size() != 1))
		{
			trace(TRACE_LEVEL_ERROR, "Role does not exist in the database\n");
			pAAAGlobalAddRuleMessage->setCompletionStatus (ROLE_DOESNT_EXIST);
			reply (pAAAGlobalAddRuleMessage);
			delete synchronousQueryContext;
			if (pResults != NULL) {
				for (UI32 j = 0; j < pResults->size(); j++)
					delete (*pResults)[j];

				delete pResults;
			}
			return;
		}
		RoleManagedObject *pRoleObj = dynamic_cast<RoleManagedObject *> ((*pResults)[0]);
		if (NULL == pRoleObj) 
		{
			trace(TRACE_LEVEL_ERROR, "Role does not exist\n");
			pAAAGlobalAddRuleMessage->setCompletionStatus (ROLE_DOESNT_EXIST);
			reply (pAAAGlobalAddRuleMessage);
			delete synchronousQueryContext;
			if (NULL != pResults) {
				for (UI32 j = 0; j < pResults->size(); j++)
					delete (*pResults)[j];

				delete pResults;
			}
			return;
		}
		
		RBACCommandType cmdType = pAAAGlobalAddRuleMessage->getCommandType();
		// Validations such as predefined rules should come here
		// Save it in the database.
		startTransaction ();
		RuleManagedObject *pRuleObj = dynamic_cast<RuleManagedObject *>(createManagedObjectInstance(RuleManagedObject::getClassName()));
		pRuleObj->setIndex(index);
		pRuleObj->setInterfaceInt(false);
		pRuleObj->setInterfaceFcoe(false);
		pRuleObj->setInterfaceTe(false);
		pRuleObj->setInterfaceGe(false);
		pRuleObj->setInterfacePc(false);
		pRuleObj->setInterfaceVlan(false);
		pRuleObj->setInterfaceMgmt(false);
		pRuleObj->setCommand(INVALID_ENUM_COMMAND);
		pRuleObj->setInterfaceFCOELeaf(INVALID_INTERFACE_COMMAND);
		pRuleObj->setInterfaceTELeaf(INVALID_INTERFACE_COMMAND);
		pRuleObj->setInterfaceGELeaf(INVALID_INTERFACE_COMMAND);
		pRuleObj->setInterfacePCLeaf(INVALID_INTERFACE_COMMAND);
		pRuleObj->setInterfaceVLANLeaf(INVALID_INTERFACE_COMMAND);

		pRuleObj->setCopyCp(false);
		pRuleObj->setCopyRunning(false);
		pRuleObj->setCopySupport(false);

		pRuleObj->setClearCl(false);
		pRuleObj->setClearLogging(false);
		pRuleObj->setClearSupport(false);

		pRuleObj->setProtocolPr(false);
		pRuleObj->setProtocolSpanning(false);
		pRuleObj->setProtocolLldp(false);
		switch (cmdType) {
			case CMD_TYPE_ENUM_CMDS:
				pRuleObj->setCommand(pAAAGlobalAddRuleMessage->getCommandEnum());
			break;

			case CMD_TYPE_INTERFACE:
				pRuleObj->setInterfaceInt(true);
			break;

			case CMD_TYPE_INTERFACE_FCOE:
				pRuleObj->setInterfaceFcoe(true);
			break;

			case CMD_TYPE_INTERFACE_TE:
				pRuleObj->setInterfaceTe(true);
			break;

			case CMD_TYPE_INTERFACE_GE:
                pRuleObj->setInterfaceGe(true);
            break;

			case CMD_TYPE_INTERFACE_PC:
				pRuleObj->setInterfacePc(true);
			break;

			case CMD_TYPE_INTERFACE_VLAN:
				pRuleObj->setInterfaceVlan(true);
			break;

			case CMD_TYPE_INTERFACE_FCOE_LEAF:
				pRuleObj->setInterfaceFCOELeaf(pAAAGlobalAddRuleMessage->getInstanceCommand());
			break;

			case CMD_TYPE_INTERFACE_TE_LEAF:
				pRuleObj->setInterfaceTELeaf(pAAAGlobalAddRuleMessage->getInstanceCommand());
			break;

			case CMD_TYPE_INTERFACE_GE_LEAF:
                pRuleObj->setInterfaceGELeaf(pAAAGlobalAddRuleMessage->getInstanceCommand());
            break;

			case CMD_TYPE_INTERFACE_PC_LEAF:
				pRuleObj->setInterfacePCLeaf(pAAAGlobalAddRuleMessage->getInstanceCommand());
			break;

			case CMD_TYPE_INTERFACE_VLAN_LEAF:
				pRuleObj->setInterfaceVLANLeaf(pAAAGlobalAddRuleMessage->getInstanceCommand());
			break;

			case CMD_TYPE_INTERFACE_MGMT:
				pRuleObj->setInterfaceMgmt(true);
			break;

			case CMD_TYPE_COPY:
				pRuleObj->setCopyCp(true);
			break;

			case CMD_TYPE_COPY_RUNNING:
				pRuleObj->setCopyRunning(true);
			break;

			case CMD_TYPE_COPY_SUPPORT:
				pRuleObj->setCopySupport(true);
			break;

			case CMD_TYPE_CLEAR:
				pRuleObj->setClearCl(true);
			break;

			case CMD_TYPE_CLEAR_LOGGING:
				pRuleObj->setClearLogging(true);
			break;

			case CMD_TYPE_CLEAR_SUPPORT:
				pRuleObj->setClearSupport(true);
			break;

			case CMD_TYPE_PROTOCOL:
				pRuleObj->setProtocolPr(true);
			break;

			case CMD_TYPE_PROTOCOL_SPANNING_TREE:
				pRuleObj->setProtocolSpanning(true);
			break;

			case CMD_TYPE_PROTOCOL_LLDP:
				pRuleObj->setProtocolLldp(true);
			break;

			default:
				trace (TRACE_LEVEL_ERROR, "Invalid command arrived.");
			break;
		}
		vector <string> paths;
		pRuleObj->setCmdTypeChoice(pAAAGlobalAddRuleMessage->getCommandType());
		pRuleObj->setRole(pRoleObj->getObjectId());
		pRuleObj->setOperation(operation);
		pRuleObj->setAction(action);
		pRuleObj->setGroup(pRoleObj->getName());
		pRuleObj->setIsPredefined(false);
		pRuleObj->setCmdRuleCounter(0);
		pRuleObj->setDataRuleCounter(0);
		vector <DataRuleManagedObject *> pDataRuleObjs;
		vector <CmdRuleManagedObject *> pCmdRuleObjs;
		if (keypath.size()) {
			for (UI32 i = 0; i < keypath.size(); i++) {
				pDataRuleObj = dynamic_cast<DataRuleManagedObject *>(createManagedObjectInstance(DataRuleManagedObject::getClassName()));
				pDataRuleObj->setIndex(MAP_INDEX(index) + i);
				pDataRuleObj->setNamespace(ALL_NAMESPACES); //TODO: Need to get it from message for flexibiilty
				pDataRuleObj->setContext(context);
				pDataRuleObj->setKeypath(keypath[i]);
				pDataRuleObj->setGroup(pRoleObj->getName());
				if (operation == READ_WRITE) {
					pDataRuleObj->setOps(READ_WRITE_EXECUTE);
				} else {
					pDataRuleObj->setOps(operation);
				}
				pDataRuleObj->setAction(action);
				pDataRuleObjs.push_back(pDataRuleObj);
				string path = string("/aaa:aaa/authorization/datarules/datarule{") + pDataRuleObj->getIndex() + "}"; 
				paths.push_back(path);
			}
			pRuleObj->setDataRuleCounter(keypath.size());
		}
		if (command.size()) {
			for (UI32 i = 0; i < command.size(); i++) {
				pCmdRuleObj = dynamic_cast<CmdRuleManagedObject *>(createManagedObjectInstance(CmdRuleManagedObject::getClassName()));
				pCmdRuleObj->setIndex(MAP_INDEX(index) + i);
				pCmdRuleObj->setContext(context);
				pCmdRuleObj->setCommand(command[i]);
				pCmdRuleObj->setGroup(pRoleObj->getName());
				size_t loc = command[i].find("show ", 0);
				// size_t noCmd = command[i].find("no ", 0);
				if ((operation == READ_WRITE) || (loc == 0)) {
					pCmdRuleObj->setOps(READ_EXECUTE);
				} else {
					pCmdRuleObj->setOps(operation);
				}
				/* Update the action in the command rules.
				 * For "no" commands it is always REJECT except for the
				 * combination of "READ_WRITE" and "ACCEPT".
				 */
#if 0
				if (noCmd == 0) {
					if ((operation == READ_WRITE) && (action == ACCEPT)) {
						action = ACCEPT;
					} else {
						action = REJECT;
					}
				}
#endif
				pCmdRuleObj->setAction(action);
				pCmdRuleObjs.push_back(pCmdRuleObj);
				string path = string("/aaa:aaa/authorization/cmdrules/cmdrule{") + pCmdRuleObj->getIndex() + "}"; 
				paths.push_back(path);
				action = pAAAGlobalAddRuleMessage->getAction();
			}
			pRuleObj->setCmdRuleCounter(command.size());
		}
		/* Commit the changes in MO instances to the DB */
		status = commitTransaction ();

		delete synchronousQueryContext;
		delete pRuleObj;
		for (UI32 j = 0; j < pDataRuleObjs.size(); j++) {
			delete pDataRuleObjs[j];
		}
		for (UI32 j = 0; j < pCmdRuleObjs.size(); j++) {
			delete pCmdRuleObjs[j];
		}
		delete pRoleObj;
		delete pResults;

		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = RULE_ADD_SUCCESS;
		}
		else
		{
			status = RULE_ADD_ERROR;
		}

	    if ((status != WAVE_MESSAGE_SUCCESS) || FrameworkToolKit::isConfigReplayInProgress())
		{
			pAAAGlobalAddRuleMessage->setCompletionStatus(status);
			reply (pAAAGlobalAddRuleMessage);
		}
		else
		{
			FrameworkToolKit::getFullyConnectedLocations(locations);
			locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

			// Send message to all local services
			AAALocalCleanCacheMessage *pSetMessage = new AAALocalCleanCacheMessage ("Add-Rule-Operation", NONE_HAS_DEFAULT_PASSWD, paths);
			AAACleanCacheContext *pCleanCacheContext = new AAACleanCacheContext (pAAAGlobalAddRuleMessage);
			WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthorizationWorker::addRuleCleanCacheSTCCallback), pCleanCacheContext);
			pWaveSendToClusterContext->setPPrismMessageForPhase1(pSetMessage);
			pWaveSendToClusterContext->setPartialSuccessFlag(true);
			if(!locations.empty()) {
				pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
			}
			sendToWaveCluster (pWaveSendToClusterContext);
		}
	}

    void  GlobalAuthorizationWorker::AAAGlobalChangeRuleMessageHandler( AAAGlobalChangeRuleMessage *pAAAGlobalChangeRuleMessage)
    {
// Validations and Queries should go here
		ResourceId status;
		if (pAAAGlobalChangeRuleMessage == NULL)
		{
			trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
			prismAssert (false, __FILE__, __LINE__);
		}
		UI32 index = pAAAGlobalChangeRuleMessage->getIndex();
		string role =  pAAAGlobalChangeRuleMessage->getRole();
		RBACOperation operation = pAAAGlobalChangeRuleMessage->getOperation();
		RBACAction action =	pAAAGlobalChangeRuleMessage->getAction();
		RBACCommandType changingCmdType = pAAAGlobalChangeRuleMessage->getCommandType();
		RoleManagedObject *pRoleObj = NULL;
		RuleManagedObject *pRuleObj = NULL;
		string command;
		vector<UI32> locations;
		
		trace (TRACE_LEVEL_DEBUG, "GlobalAuthorizationWorker::AAAGlobalChangeRuleMessageHandler()");

		if ((pAAAGlobalChangeRuleMessage->getRole().compare(DEFAULT_ADMIN_ROLE) == 0) || (pAAAGlobalChangeRuleMessage->getRole().compare(DEFAULT_USER_ROLE) == 0)) {
			trace(TRACE_LEVEL_ERROR, "Rules cannot be set for predefined roles");
			pAAAGlobalChangeRuleMessage->setCompletionStatus(RULE_SET_PREDEFINED_ROLE);
			reply (pAAAGlobalChangeRuleMessage);
			return;
		}

		/* Get the Rule Managed Object for optional parameters */
		WaveManagedObjectSynchronousQueryContext *syncRuleContext = new WaveManagedObjectSynchronousQueryContext(RuleManagedObject::getClassName());

		syncRuleContext->addAndAttribute(new AttributeUI32(index, "index"));
		vector<WaveManagedObject *> *ruleResults = querySynchronously (syncRuleContext);
		if ((NULL == ruleResults) || (ruleResults->size() != 1))
		{
			trace (TRACE_LEVEL_ERROR, "Rule does not exist");
			pAAAGlobalChangeRuleMessage->setCompletionStatus (RULE_DOESNT_EXIST);
			reply (pAAAGlobalChangeRuleMessage);
			delete syncRuleContext;
			if (ruleResults != NULL) {
				for (UI32 j = 0; j < ruleResults->size(); j++)
					delete (*ruleResults)[j];

				delete ruleResults;
			}
			return;
		}
		pRuleObj = dynamic_cast<RuleManagedObject *> ((*ruleResults)[0]);
		if (NULL == pRuleObj)
		{
			trace (TRACE_LEVEL_ERROR, "Rule does not exist");
			pAAAGlobalChangeRuleMessage->setCompletionStatus (RULE_DOESNT_EXIST);
			reply (pAAAGlobalChangeRuleMessage);
			delete syncRuleContext;
			if (NULL != ruleResults) {
				for (UI32 j = 0; j < ruleResults->size(); j++)
					delete (*ruleResults)[j];

				delete ruleResults;
			}
			return;
		}
		delete syncRuleContext;
		delete ruleResults;

		/*Predefined factory rules cannot be changed by customer. */
		if (pRuleObj->getIsPredefined()) {
			trace (TRACE_LEVEL_ERROR, "Pre-defined rules cannot be changed");
			pAAAGlobalChangeRuleMessage->setCompletionStatus(RULE_CHANGE_PREDEFINED);
			reply (pAAAGlobalChangeRuleMessage);
			return;
		}
		/* Finds if the existing rule is data or command rule. */
		DataRuleManagedObject *pDataRuleObj = NULL;
		CmdRuleManagedObject *pCmdRuleObj = NULL;
		vector <DataRuleManagedObject *> pExistDataRuleObjs;
		vector <CmdRuleManagedObject *> pExistCmdRuleObjs;
		vector <DataRuleManagedObject *> pChangeDataRuleObjs;
		vector <CmdRuleManagedObject *> pChangeCmdRuleObjs;
		UI32 existingCmdRuleCount = pRuleObj->getCmdRuleCounter();
		UI32 existingDataRuleCount = pRuleObj->getDataRuleCounter();
		UI32 changingCmdRuleCount = (pAAAGlobalChangeRuleMessage->getCommand()).size();
		UI32 changingDataRuleCount = (pAAAGlobalChangeRuleMessage->getKeypath()).size();

		// If we don't get both command and keypath, command is not being changed. So setting back
		// the changingCmdRuleCount and changingDataRuleCount to existingCmdRuleCount and existingDataRuleCount.
		if ((changingCmdRuleCount == 0) && (changingDataRuleCount == 0)) {
			changingCmdRuleCount = existingCmdRuleCount;
			changingDataRuleCount = existingDataRuleCount;
		}

		if (existingDataRuleCount > 0) {
			syncRuleContext = new WaveManagedObjectSynchronousQueryContext(DataRuleManagedObject::getClassName());
			for (UI32 i = 0; i < existingDataRuleCount; i++) {
				syncRuleContext->addOrAttribute(new AttributeUI32(MAP_INDEX(index) + i, "index"));
			}
			syncRuleContext->addOrderField("index", true);
			ruleResults = querySynchronously(syncRuleContext);
			if ((NULL == ruleResults) || (ruleResults->size() != existingDataRuleCount))
			{
				trace (TRACE_LEVEL_ERROR, "Data Rule does not exist");
				pAAAGlobalChangeRuleMessage->setCompletionStatus (RULE_CHANGE_ERROR);
				reply (pAAAGlobalChangeRuleMessage);
				delete syncRuleContext;
				if (ruleResults != NULL) {
					for (UI32 j = 0; j < ruleResults->size(); j++)
						delete (*ruleResults)[j];
					delete ruleResults;
				}
				return;
			}
			for (UI32 i = 0; i < existingDataRuleCount; i++) {
				pDataRuleObj = dynamic_cast<DataRuleManagedObject *> ((*ruleResults)[i]);
				pExistDataRuleObjs.push_back(pDataRuleObj);
			}
			delete ruleResults;
			delete syncRuleContext;
		}

		if (existingCmdRuleCount > 0) {
			syncRuleContext = new WaveManagedObjectSynchronousQueryContext(CmdRuleManagedObject::getClassName());
			for (UI32 i = 0; i < existingCmdRuleCount; i++) {
				syncRuleContext->addOrAttribute(new AttributeUI32(MAP_INDEX(index) + i, "index1"));
			}
			syncRuleContext->addOrderField("index1", true);
			ruleResults = querySynchronously(syncRuleContext);
			if ((NULL == ruleResults) || (ruleResults->size() != existingCmdRuleCount))
			{
				trace (TRACE_LEVEL_ERROR, "Cmd rule does not exist");
				pAAAGlobalChangeRuleMessage->setCompletionStatus (RULE_CHANGE_ERROR);
				reply (pAAAGlobalChangeRuleMessage);
				delete syncRuleContext;
				if (ruleResults != NULL) {
					for (UI32 j = 0; j < ruleResults->size(); j++)
						delete (*ruleResults)[j];
					delete ruleResults;
				}
				return;
			}
			for (UI32 i = 0; i < existingCmdRuleCount; i++) {
				pCmdRuleObj = dynamic_cast<CmdRuleManagedObject *> ((*ruleResults)[i]);
				pExistCmdRuleObjs.push_back(pCmdRuleObj);
			}
			delete ruleResults;
			delete syncRuleContext;
		}

		vector<WaveManagedObject *> *pResults = NULL;
		if (role.length()) {
			WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RoleManagedObject::getClassName());

			synchronousQueryContext->addAndAttribute(new AttributeString(pAAAGlobalChangeRuleMessage->getRole(), "rolename"));
			pResults = querySynchronously (synchronousQueryContext);
			if ((NULL == pResults) || (pResults->size() != 1))
			{
				trace (TRACE_LEVEL_ERROR, "Role does not exist");
				pAAAGlobalChangeRuleMessage->setCompletionStatus (ROLE_DOESNT_EXIST);
				reply (pAAAGlobalChangeRuleMessage);
				delete synchronousQueryContext;
				if (pResults != NULL) {
					for (UI32 j = 0; j < pResults->size(); j++)
						delete (*pResults)[j];
					delete pResults;
				}
				return;
			}
			pRoleObj = dynamic_cast<RoleManagedObject *> ((*pResults)[0]);

			if (NULL == pRoleObj)
			{
				trace (TRACE_LEVEL_ERROR, "Role does not exist");
				pAAAGlobalChangeRuleMessage->setCompletionStatus (ROLE_DOESNT_EXIST);
				reply (pAAAGlobalChangeRuleMessage);
				delete synchronousQueryContext;
				if (NULL != pResults) {
					for (UI32 j = 0; j < pResults->size(); j++)
						delete (*pResults)[j];
					delete pResults;
				}
				return;
			}
			delete synchronousQueryContext;
		}

		//If the user enters the command, finds whether the user entered data rule command
		//or command rule command.
		// Finds if the new command belongs to the different type of rule than the earlier.
		DataRuleManagedObject *pDataRuleObjNew = NULL;
		CmdRuleManagedObject *pCmdRuleObjNew = NULL;
		// Save it in the database.
		startTransaction ();
		//First update the rule object.
		updateWaveManagedObject(pRuleObj);
		if (operation != INVALID_OPERATION) {
			pRuleObj->setOperation(operation);
		}

		if (action != INVALID_ACTION) {
			pRuleObj->setAction(action);
		}

		if (role.length()){
			pRuleObj->setRole(pRoleObj->getObjectId());
			pRuleObj->setGroup(pRoleObj->getName());
		}

		if (pAAAGlobalChangeRuleMessage->getCommand().size() ||
			pAAAGlobalChangeRuleMessage->getKeypath().size()) {
			RBACCommandType existingCmdType =	(RBACCommandType)pRuleObj->getCmdTypeChoice();
			// It is better to have 2 switch statements rather than setting everything else once again
			switch (existingCmdType) {
				case CMD_TYPE_ENUM_CMDS:
					pRuleObj->setCommand(pAAAGlobalChangeRuleMessage->getCommandEnum());
				break;
				case CMD_TYPE_INTERFACE:
					pRuleObj->setInterfaceInt(false);
				break;
				case CMD_TYPE_INTERFACE_FCOE:
					pRuleObj->setInterfaceFcoe(false);
				break;
				case CMD_TYPE_INTERFACE_TE:
					pRuleObj->setInterfaceTe(false);
				break;
				case CMD_TYPE_INTERFACE_GE:
                    pRuleObj->setInterfaceGe(false);
				break;
				case CMD_TYPE_INTERFACE_PC:
					pRuleObj->setInterfacePc(false);
				break;
				case CMD_TYPE_INTERFACE_VLAN:
					pRuleObj->setInterfaceVlan(false);
				break;
				case CMD_TYPE_INTERFACE_FCOE_LEAF:
					pRuleObj->setInterfaceFCOELeaf(pAAAGlobalChangeRuleMessage->getInstanceCommand());
				break;
				case CMD_TYPE_INTERFACE_TE_LEAF:
					pRuleObj->setInterfaceTELeaf(pAAAGlobalChangeRuleMessage->getInstanceCommand());
				break;
				case CMD_TYPE_INTERFACE_GE_LEAF:
                    pRuleObj->setInterfaceGELeaf(pAAAGlobalChangeRuleMessage->getInstanceCommand());
                break;
				case CMD_TYPE_INTERFACE_PC_LEAF:
					pRuleObj->setInterfacePCLeaf(pAAAGlobalChangeRuleMessage->getInstanceCommand());
				break;
				case CMD_TYPE_INTERFACE_VLAN_LEAF:
					pRuleObj->setInterfaceVLANLeaf(pAAAGlobalChangeRuleMessage->getInstanceCommand());
				break;
				case CMD_TYPE_INTERFACE_MGMT:
					pRuleObj->setInterfaceMgmt(false);
				break;
				case CMD_TYPE_COPY:
					pRuleObj->setCopyCp(false);
				break;
				case CMD_TYPE_COPY_RUNNING:
					pRuleObj->setCopyRunning(false);
				break;
				case CMD_TYPE_COPY_SUPPORT:
					pRuleObj->setCopySupport(false);
				break;
				case CMD_TYPE_CLEAR:
					pRuleObj->setClearCl(false);
				break;
				case CMD_TYPE_CLEAR_LOGGING:
					pRuleObj->setClearLogging(false);
				break;
				case CMD_TYPE_CLEAR_SUPPORT:
					pRuleObj->setClearSupport(false);
				break;
				case CMD_TYPE_PROTOCOL:
					pRuleObj->setProtocolPr(false);
				break;
				case CMD_TYPE_PROTOCOL_SPANNING_TREE:
					pRuleObj->setProtocolSpanning(false);
				break;
				case CMD_TYPE_PROTOCOL_LLDP:
					pRuleObj->setProtocolLldp(false);
				break;
				default:
					trace (TRACE_LEVEL_ERROR, "Invalid command existing.");
				break;
			}

			switch (changingCmdType) {
				case CMD_TYPE_ENUM_CMDS:
					pRuleObj->setCommand(pAAAGlobalChangeRuleMessage->getCommandEnum());
				break;
				case CMD_TYPE_INTERFACE:
					pRuleObj->setInterfaceInt(true);
				break;
				case CMD_TYPE_INTERFACE_FCOE:
					pRuleObj->setInterfaceFcoe(true);
				break;
				case CMD_TYPE_INTERFACE_TE:
					pRuleObj->setInterfaceTe(true);
				break;
				case CMD_TYPE_INTERFACE_GE:
                    pRuleObj->setInterfaceGe(true);
				break;
				case CMD_TYPE_INTERFACE_PC:
					pRuleObj->setInterfacePc(true);
				break;
				case CMD_TYPE_INTERFACE_VLAN:
					pRuleObj->setInterfaceVlan(true);
				break;
				case CMD_TYPE_INTERFACE_FCOE_LEAF:
					pRuleObj->setInterfaceFCOELeaf(pAAAGlobalChangeRuleMessage->getInstanceCommand());
				break;
				case CMD_TYPE_INTERFACE_TE_LEAF:
					pRuleObj->setInterfaceTELeaf(pAAAGlobalChangeRuleMessage->getInstanceCommand());
				break;
				case CMD_TYPE_INTERFACE_GE_LEAF:
                    pRuleObj->setInterfaceGELeaf(pAAAGlobalChangeRuleMessage->getInstanceCommand());
                break;
				case CMD_TYPE_INTERFACE_PC_LEAF:
					pRuleObj->setInterfacePCLeaf(pAAAGlobalChangeRuleMessage->getInstanceCommand());
				break;
				case CMD_TYPE_INTERFACE_VLAN_LEAF:
					pRuleObj->setInterfaceVLANLeaf(pAAAGlobalChangeRuleMessage->getInstanceCommand());
				break;
				case CMD_TYPE_INTERFACE_MGMT:
					pRuleObj->setInterfaceMgmt(true);
				break;
				case CMD_TYPE_COPY:
					pRuleObj->setCopyCp(true);
				break;
				case CMD_TYPE_COPY_RUNNING:
					pRuleObj->setCopyRunning(true);
				break;
				case CMD_TYPE_COPY_SUPPORT:
					pRuleObj->setCopySupport(true);
				break;
				case CMD_TYPE_CLEAR:
					pRuleObj->setClearCl(true);
				break;
				case CMD_TYPE_CLEAR_LOGGING:
					pRuleObj->setClearLogging(true);
				break;
				case CMD_TYPE_CLEAR_SUPPORT:
					pRuleObj->setClearSupport(true);
				break;
				case CMD_TYPE_PROTOCOL:
					pRuleObj->setProtocolPr(true);
				break;
				case CMD_TYPE_PROTOCOL_SPANNING_TREE:
					pRuleObj->setProtocolSpanning(true);
				break;
				case CMD_TYPE_PROTOCOL_LLDP:
					pRuleObj->setProtocolLldp(true);
				break;
				default:
					trace (TRACE_LEVEL_ERROR, "Invalid command arrived.");
				break;
			}
			pRuleObj->setCmdTypeChoice(changingCmdType);
			pRuleObj->setCmdRuleCounter(changingCmdRuleCount);
			pRuleObj->setDataRuleCounter(changingDataRuleCount);
		}

		/* Changing command and Data Rule attributes. */
		vector<string> changingCmdList = pAAAGlobalChangeRuleMessage->getCommand();
		vector<string> paths;
		/* Possible cases of changing existing command rule objects. */
		if ((existingCmdRuleCount > 0) && (changingCmdRuleCount > 0))
		{
			UI32 updateRulesSize = existingCmdRuleCount;
			if (changingCmdRuleCount < existingCmdRuleCount) {
				updateRulesSize = changingCmdRuleCount;
			}
			for (UI32 i = 0; i < updateRulesSize;i++) {
				pCmdRuleObj = pExistCmdRuleObjs[i];
				updateWaveManagedObject(pCmdRuleObj);
				/* Update the operation in the command rules.
				 * For "show" commands it is always READ_EXECUTE.
				 */
				
				string path = string("/aaa:aaa/authorization/cmdrules/cmdrule{") + pCmdRuleObj->getIndex() + "}"; 
				paths.push_back(path);
				if (changingCmdList.size()) {
					command = changingCmdList[i];
				} else {
					command = pCmdRuleObj->getCommand();
				}
				if (operation == INVALID_OPERATION) {
					operation = pRuleObj->getOperation();
				}
				if (action == INVALID_ACTION) {
					action = pRuleObj->getAction();
				}
				size_t showCmd = command.find("show ", 0);
	//			size_t noCmd = command.find("no ", 0);
				if (showCmd == 0) {
					pCmdRuleObj->setOps(READ_EXECUTE);
				} else {
					if (operation == READ_WRITE) {
						pCmdRuleObj->setOps(READ_EXECUTE);
					} else {
						pCmdRuleObj->setOps(operation);
					}
				}

				/* Update the action in the command rules.
				 * For "no" commands it is always REJECT except for the
				 * combination of "READ_WRITE" and "ACCEPT".
				 */
#if 0
				if (noCmd == 0) {
					if ((operation == READ_WRITE) && (action == ACCEPT)) {
						action = ACCEPT;
					} else {
						action = REJECT;
					}
				}
#endif
				pCmdRuleObj->setAction(action);
				if (role.length()){
					pCmdRuleObj->setGroup(role);
				}
				if (changingCmdList.size()) {
					pCmdRuleObj->setCommand(changingCmdList[i]);
				}
				/* Setting the old "action" value back which could be different than the original due to "no" command.*/
				action = pAAAGlobalChangeRuleMessage->getAction();
			}
		}

		/* Possible cases of adding new command rule objects. */
		if (changingCmdRuleCount > existingCmdRuleCount)
		{
			for (UI32 i = existingCmdRuleCount; i < changingCmdRuleCount; i++) {
				pCmdRuleObjNew = dynamic_cast<CmdRuleManagedObject *>(createManagedObjectInstance(CmdRuleManagedObject::getClassName()));
				pCmdRuleObjNew->setIndex(MAP_INDEX(index) + i);
				string path = string("/aaa:aaa/authorization/cmdrules/cmdrule{") + pCmdRuleObjNew->getIndex() + "}"; 
				paths.push_back(path);
				if (operation == INVALID_OPERATION) {
					operation = pRuleObj->getOperation();
				}
				size_t loc= changingCmdList[i].find("show ", 0);
//				size_t noCmd = changingCmdList[i].find("no ", 0);
				if ((operation == READ_WRITE) || (loc == 0)) {
					pCmdRuleObjNew->setOps(READ_EXECUTE);
				} else {
					pCmdRuleObjNew->setOps(operation);
				}
				if (action == INVALID_ACTION) {
					action = pRuleObj->getAction();
				}

				/* Update the action in the command rules.
				 * For "no" commands it is always REJECT except for the
				 * combination of "READ_WRITE" and "ACCEPT".
				 */
#if 0
				if (noCmd == 0) {
					if ((operation == READ_WRITE) && (action == ACCEPT)) {
						action = ACCEPT;
					} else {
						action = REJECT;
					}
				}
#endif
				pCmdRuleObjNew->setAction(action);

				if (role.length() == 0) {
					role = pRuleObj->getGroup();
				}
				pCmdRuleObjNew->setGroup(role);
				// It comes here only if the command exists.
				pCmdRuleObjNew->setCommand(changingCmdList[i]);

				//Set off the isDataRule flag
				pCmdRuleObjNew->setContext(ALL_CONTEXTS);
				pChangeCmdRuleObjs.push_back(pCmdRuleObjNew);
				/* Setting the old "action" value back which could be different than the original due to "no" command.*/
				action = pAAAGlobalChangeRuleMessage->getAction();
			}
		}
		/* Possible cases for deleting existing cmd rule objects. */
		if (changingCmdRuleCount < existingCmdRuleCount) {
			for (UI32 i = changingCmdRuleCount; i < existingCmdRuleCount;i++) {
				string path = string("/aaa:aaa/authorization/cmdrules/cmdrule{") + pExistCmdRuleObjs[i]->getIndex() + "}"; 
				paths.push_back(path);
				delete pExistCmdRuleObjs[i];
			}
			pExistCmdRuleObjs.erase(pExistCmdRuleObjs.begin() + changingCmdRuleCount,
									pExistCmdRuleObjs.begin() + existingCmdRuleCount);
		}

		vector<string> changingDataList = pAAAGlobalChangeRuleMessage->getKeypath();
		/* Possible cases of changing existing data rule objects. */
		if ((existingDataRuleCount > 0) && (changingDataRuleCount > 0))
		{
			UI32 updateRulesSize = existingDataRuleCount;
			if (changingDataRuleCount < existingDataRuleCount) {
				updateRulesSize = changingDataRuleCount;
			}
			for (UI32 i = 0; i < updateRulesSize;i++) {
				pDataRuleObj = pExistDataRuleObjs[i];
				updateWaveManagedObject(pDataRuleObj);
				string path = string("/aaa:aaa/authorization/datarules/datarule{") + pDataRuleObj->getIndex() + "}"; 
				paths.push_back(path);
				if (operation != INVALID_OPERATION) {
					if (operation == READ_WRITE) {
						pDataRuleObj->setOps(READ_WRITE_EXECUTE);
					} else {
						pDataRuleObj->setOps(operation);
					}
				}
				if (action != INVALID_ACTION) {
					pDataRuleObj->setAction(action);
				}

				if (role.length()){
					pDataRuleObj->setGroup(role);
				}

				if (changingDataList.size()) {
					pDataRuleObj->setKeypath(changingDataList[i]);
				}
			}
		}

		/* Possible cases of adding new data rule objects. */
		if ((changingDataRuleCount > existingDataRuleCount))
		{
			for (UI32 i = existingDataRuleCount; i < changingDataRuleCount;i++) {
				// Create a new instance of data rule.
				pDataRuleObjNew = dynamic_cast<DataRuleManagedObject *>(createManagedObjectInstance(DataRuleManagedObject::getClassName()));
				pDataRuleObjNew->setIndex(MAP_INDEX(index) + i);
				string path = string("/aaa:aaa/authorization/datarules/datarule{") + pDataRuleObjNew->getIndex() + "}"; 
				paths.push_back(path);
				if (operation == INVALID_OPERATION) {
					operation = pRuleObj->getOperation();
				}
				if (operation == READ_WRITE) {
					pDataRuleObjNew->setOps(READ_WRITE_EXECUTE);
				} else {
					pDataRuleObjNew->setOps(operation);
				}

				if (action == INVALID_ACTION) {
					action = pRuleObj->getAction();
				}
				pDataRuleObjNew->setAction(action);

				if (role.length() == 0) {
					role = pRuleObj->getGroup();
				}
				pDataRuleObjNew->setGroup(role);

				// It comes here only if the command exists.
				pDataRuleObjNew->setKeypath(changingDataList[i]);
				pDataRuleObjNew->setNamespace(ALL_NAMESPACES);
				pDataRuleObjNew->setContext(ALL_CONTEXTS);
				pChangeDataRuleObjs.push_back(pDataRuleObjNew);
			}
		}

		/* Possible cases for deleting existing data rule objects. */
		if (changingDataRuleCount < existingDataRuleCount) {
			for (UI32 i = changingDataRuleCount; i < existingDataRuleCount;i++) {
				delete pExistDataRuleObjs[i];
				string path = string("/aaa:aaa/authorization/datarules/datarule{") + pExistDataRuleObjs[i]->getIndex() + "}"; 
				paths.push_back(path);
			}
			pExistDataRuleObjs.erase(pExistDataRuleObjs.begin() + changingDataRuleCount,
								pExistDataRuleObjs.begin() + existingDataRuleCount);
		}

		/* Commit the changes in MO instances to the DB */
		status = commitTransaction ();
		if (pRuleObj) {
			delete pRuleObj;
		}

		for (UI32 j = 0; j < pExistDataRuleObjs.size(); j++) {
			delete pExistDataRuleObjs[j];
		}

		for (UI32 j = 0; j < pChangeDataRuleObjs.size(); j++) {
			delete pChangeDataRuleObjs[j];
		}

		for (UI32 j = 0; j < pExistCmdRuleObjs.size(); j++) {
			delete pExistCmdRuleObjs[j];
		}

		for (UI32 j = 0; j < pChangeCmdRuleObjs.size(); j++) {
			delete pChangeCmdRuleObjs[j];
		}

		if (role.length()) {
			delete pRoleObj;
			delete pResults;
		}
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = RULE_ADD_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting the Rule configuration to Wave database is successful.");
		}
		else
		{
			status = RULE_CHANGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting the Rule configuration failed. Framework error - " + FrameworkToolKit::localize (status));
		}
		if ((status != WAVE_MESSAGE_SUCCESS) || FrameworkToolKit::isConfigReplayInProgress())
		{
			pAAAGlobalChangeRuleMessage->setCompletionStatus(status);
			reply (pAAAGlobalChangeRuleMessage);
		}
		else
		{
			FrameworkToolKit::getFullyConnectedLocations(locations);
			locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

			// Send message to all local services to reload the AAA Cache.
			AAALocalCleanCacheMessage *pSetMessage = new AAALocalCleanCacheMessage ("Change-Rule-Operation", NONE_HAS_DEFAULT_PASSWD, paths);
			AAACleanCacheContext *pCleanCacheContext = new AAACleanCacheContext (pAAAGlobalChangeRuleMessage);
			WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthorizationWorker::changeRuleCleanCacheSTCCallback), pCleanCacheContext);
			pWaveSendToClusterContext->setPPrismMessageForPhase1(pSetMessage);
			pWaveSendToClusterContext->setPartialSuccessFlag(true);
			if(!locations.empty()) {
				pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
			}
			sendToWaveCluster (pWaveSendToClusterContext);
		}
    }

    void  GlobalAuthorizationWorker::AAAGlobalDeleteRuleMessageHandler( AAAGlobalDeleteRuleMessage *pAAAGlobalDeleteRuleMessage)
    {
	// Validations and Queries should go here
		ResourceId status;

		trace (TRACE_LEVEL_DEBUG, "GlobalAuthorizationWorker::AAAGlobalDeleteRuleMessageHandler()");
		if (pAAAGlobalDeleteRuleMessage == NULL)
		{
			trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
			prismAssert (false, __FILE__, __LINE__);
		}

		vector<UI32> locations;
		/* Get the Rule Managed Object for optional parameters */
		WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RuleManagedObject::getClassName());

		synchronousQueryContext->addAndAttribute(new AttributeUI32(pAAAGlobalDeleteRuleMessage->getIndex(), "index"));
		vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
		if ((NULL == pResults) || (pResults->size() != 1))
		{
			trace (TRACE_LEVEL_FATAL, "Rule does not exist");
			pAAAGlobalDeleteRuleMessage->setCompletionStatus (RULE_DOESNT_EXIST);
			reply (pAAAGlobalDeleteRuleMessage);
			delete synchronousQueryContext;
			if (pResults != NULL) {
				for (UI32 j = 0; j < pResults->size(); j++)
					delete (*pResults)[j];

				delete pResults;
			}
			return;
		}
		RuleManagedObject *pRuleObj = dynamic_cast<RuleManagedObject *> ((*pResults)[0]);
		if (NULL == pRuleObj)
		{
			trace (TRACE_LEVEL_FATAL, "Rule does not exist");
			pAAAGlobalDeleteRuleMessage->setCompletionStatus (RULE_DOESNT_EXIST);
			reply (pAAAGlobalDeleteRuleMessage);
			delete synchronousQueryContext;
			if (NULL != pResults) {
				for (UI32 j = 0; j < pResults->size(); j++)
					delete (*pResults)[j];

				delete pResults;
			}
			return;
		}	

		delete pResults;
		delete synchronousQueryContext;

		/*Predefined factory rules cannot be changed by customer. */
		if (pRuleObj->getIsPredefined()) {
			trace (TRACE_LEVEL_ERROR, "Pre-defined rules cannot be deleted");
			pAAAGlobalDeleteRuleMessage->setCompletionStatus(RULE_DELETE_PREDEFINED);
			reply (pAAAGlobalDeleteRuleMessage);
			return;
		}

		DataRuleManagedObject *pDataRuleObj = NULL;
		CmdRuleManagedObject *pCmdRuleObj = NULL;
		UI32 cmdRuleCounter = pRuleObj->getCmdRuleCounter();
		UI32 dataRuleCounter = pRuleObj->getDataRuleCounter();
		vector <DataRuleManagedObject *> pDataRuleObjs;
		vector <CmdRuleManagedObject *> pCmdRuleObjs;
		if (dataRuleCounter > 0) {
			synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(DataRuleManagedObject::getClassName());
			for (UI32 i = 0; i < dataRuleCounter; i++) {
				synchronousQueryContext->addOrAttribute(new AttributeUI32(MAP_INDEX(pAAAGlobalDeleteRuleMessage->getIndex()) + i, "index"));
			}
			pResults = querySynchronously (synchronousQueryContext);
			if ((NULL == pResults) || (pResults->size() != dataRuleCounter))
			{
				trace (TRACE_LEVEL_FATAL, "Data Rule does not exist");
				pAAAGlobalDeleteRuleMessage->setCompletionStatus (RULE_DEL_ERROR);
				reply (pAAAGlobalDeleteRuleMessage);
				delete synchronousQueryContext;
				if (pResults != NULL) {
					for (UI32 j = 0; j < pResults->size(); j++)
						delete (*pResults)[j];
					delete pResults;
				}
				return;
			}
			for (UI32 i = 0; i < dataRuleCounter; i++) {
				pDataRuleObj = dynamic_cast<DataRuleManagedObject *> ((*pResults)[i]);
				pDataRuleObjs.push_back(pDataRuleObj);
			}
			delete pResults;
			delete synchronousQueryContext;
		}

		if (cmdRuleCounter > 0) {
			synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(CmdRuleManagedObject::getClassName());
			for (UI32 i = 0; i < cmdRuleCounter; i++) {
				synchronousQueryContext->addOrAttribute(new AttributeUI32(MAP_INDEX(pAAAGlobalDeleteRuleMessage->getIndex()) + i, "index1"));
			}
			pResults = querySynchronously (synchronousQueryContext);
			if ((NULL == pResults) || (pResults->size() != cmdRuleCounter))
			{
				trace (TRACE_LEVEL_FATAL, "Cmd Rule does not exist");
				pAAAGlobalDeleteRuleMessage->setCompletionStatus (RULE_DEL_ERROR);
				reply (pAAAGlobalDeleteRuleMessage);
				delete synchronousQueryContext;
				if (pResults != NULL) {
					for (UI32 j = 0; j < pResults->size(); j++)
						delete (*pResults)[j];
					delete pResults;
				}
				return;
			}
			for (UI32 i = 0; i < cmdRuleCounter; i++) {
				pCmdRuleObj = dynamic_cast<CmdRuleManagedObject *> ((*pResults)[i]);
				pCmdRuleObjs.push_back(pCmdRuleObj);
			}
			delete pResults;
			delete synchronousQueryContext;
		}
		vector <string> paths; 
		// Save it in the database.
		startTransaction ();

		if (dataRuleCounter > 0)
		{
			for (UI32 j = 0; j < pDataRuleObjs.size(); j++) {
				string path = string("/aaa:aaa/authorization/datarules/datarule{") + pDataRuleObjs[j]->getIndex() + "}"; 
				paths.push_back(path);
				delete pDataRuleObjs[j];
			}
		}
		if (cmdRuleCounter > 0)
		{
			for (UI32 j = 0; j < pCmdRuleObjs.size(); j++) {
				string path = string("/aaa:aaa/authorization/cmdrules/cmdrule{") + pCmdRuleObjs[j]->getIndex() + "}"; 
				paths.push_back(path);
				delete pCmdRuleObjs[j];
			}
		}
		delete pRuleObj;

		/* Commit the changes in MO instances to the DB */
		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = RULE_ADD_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting the Rule configuration to Wave database is successful.");
		}
		else
		{
			status = RULE_DEL_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting the Rule configuration failed. Framework error - " + FrameworkToolKit::localize (status));
		}

		if ((status != WAVE_MESSAGE_SUCCESS) || FrameworkToolKit::isConfigReplayInProgress())
		{
			pAAAGlobalDeleteRuleMessage->setCompletionStatus(status);
			reply (pAAAGlobalDeleteRuleMessage);
		}
		else
		{
			FrameworkToolKit::getFullyConnectedLocations(locations);
			locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

			// Send message to all local services to reload the AAA Cache.
			AAALocalCleanCacheMessage *pSetMessage = new AAALocalCleanCacheMessage ("Delete-Rule-Operation", NONE_HAS_DEFAULT_PASSWD, paths);
			AAACleanCacheContext *pCleanCacheContext = new AAACleanCacheContext (pAAAGlobalDeleteRuleMessage);
			WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthorizationWorker::deleteRuleCleanCacheSTCCallback), pCleanCacheContext);
			pWaveSendToClusterContext->setPPrismMessageForPhase1(pSetMessage);
			pWaveSendToClusterContext->setPartialSuccessFlag(true);
			if(!locations.empty()) {
				pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
			}
			sendToWaveCluster (pWaveSendToClusterContext);
			//pAAAGlobalDeleteRuleMessage->setCompletionStatus(status);
			//reply (pAAAGlobalDeleteRuleMessage);
			//trace (TRACE_LEVEL_WARN, "AAAGlobaleDeleteRuleMessageHandler() - EXIT.");
		}
    }

	// TODO: Need to support Roll back functionality
	void GlobalAuthorizationWorker::addRuleCleanCacheSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		trace (TRACE_LEVEL_DEBUG, "GlobalAuthorizationWorker::addRuleCleanCacheSTCCallback()");
		AAACleanCacheContext *pAAAContext = (AAACleanCacheContext *)pWaveSendToClusterContext->getPCallerContext();
		AAALocalCleanCacheMessage *pCleanCacheMessage = (AAALocalCleanCacheMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
		ResourceId status = pWaveSendToClusterContext->getCompletionStatus();

		AAAGlobalAddRuleMessage *pAddRuleMessage = (AAAGlobalAddRuleMessage *)pAAAContext->getPPrismMessage();

		if (status == WAVE_MESSAGE_SUCCESS)
		{
			trace (TRACE_LEVEL_DEBUG, "AAA Clean Cache operation is successful.");
		}
		else if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
            status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
		{
			vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
			//FrameworkToolKit::getFullyConnectedLocations(cxLocations);
			if (cxLocations.size() > 0)
			{
				string errStr;
				pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalAuthorizationWorker::addRulecleanCacheSTCCallback", errStr);
				pAddRuleMessage->setMessageString(errStr);
				status = CLUSTER_ERROR_1001;
			}
			else
			{
				status = RULE_ADD_ERROR;
				trace (TRACE_LEVEL_ERROR, "Adding rule failed with error - " + FrameworkToolKit::localize(status));
			}
		}

		delete pCleanCacheMessage;

		delete pAAAContext;
		delete pWaveSendToClusterContext;

		pAddRuleMessage->setCompletionStatus(status);
		reply(pAddRuleMessage);
		trace (TRACE_LEVEL_DEBUG, "GlobalAuthorizationWorker::addRuleCleanCacheSTCCallback() EXIT");
}

	void GlobalAuthorizationWorker::changeRuleCleanCacheSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		trace (TRACE_LEVEL_DEBUG, "GlobalAuthorizationWorker::changeRuleCleanCacheSTCCallback()");
		AAACleanCacheContext *pAAAContext = (AAACleanCacheContext *)pWaveSendToClusterContext->getPCallerContext();
		AAALocalCleanCacheMessage *pCleanCacheMessage = (AAALocalCleanCacheMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
		ResourceId status = pWaveSendToClusterContext->getCompletionStatus();

		AAAGlobalChangeRuleMessage *pChangeRuleMessage = (AAAGlobalChangeRuleMessage *)pAAAContext->getPPrismMessage();

		if (status == WAVE_MESSAGE_SUCCESS)
		{
			trace (TRACE_LEVEL_DEBUG, "AAA Rule Change Clean Cache operation is successful.");
		}
		else if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
            status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
		{
			vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
			//FrameworkToolKit::getFullyConnectedLocations(cxLocations);
			if (cxLocations.size() > 0)
			{
				string errStr;
				pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalAuthorizationWorker::changeRulecleanCacheSTCCallback", errStr);
				pChangeRuleMessage->setMessageString(errStr);
				status = CLUSTER_ERROR_1001;
			}
			else
			{
				status = RULE_CHANGE_ERROR;
				trace (TRACE_LEVEL_ERROR, "Changing rule failed with error - " + FrameworkToolKit::localize(status));
			}
		}

		delete pCleanCacheMessage;
		delete pAAAContext;
		delete pWaveSendToClusterContext;
		pChangeRuleMessage->setCompletionStatus(status);
		reply(pChangeRuleMessage);

		trace (TRACE_LEVEL_DEBUG, "GlobalAuthorizationWorker::changeRuleCleanCacheSTCCallback() EXIT");
	}

	void GlobalAuthorizationWorker::deleteRuleCleanCacheSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		trace (TRACE_LEVEL_DEBUG, "GlobalAuthorizationWorker::deleteRuleCleanCacheSTCCallback()");
		AAACleanCacheContext *pAAAContext = (AAACleanCacheContext *)pWaveSendToClusterContext->getPCallerContext();
		AAALocalCleanCacheMessage *pCleanCacheMessage = (AAALocalCleanCacheMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
		ResourceId status = pWaveSendToClusterContext->getCompletionStatus();

		AAAGlobalDeleteRuleMessage *pDeleteRuleMessage = (AAAGlobalDeleteRuleMessage *)pAAAContext->getPPrismMessage();

		if (status == WAVE_MESSAGE_SUCCESS)
		{
			trace (TRACE_LEVEL_DEBUG, "AAA Rule Delete Clean Cache operation is successful.");
		}
		else if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
            status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
		{
			vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
			//FrameworkToolKit::getFullyConnectedLocations(cxLocations);
			if (cxLocations.size() > 0)
			{
				string errStr;
				pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalAuthorizationWorker::deleteRuleCleanCacheSTCCallback", errStr);
				pDeleteRuleMessage->setMessageString(errStr);
				status = CLUSTER_ERROR_1001;
			}
			else
			{
				status = RULE_DEL_ERROR;
				trace (TRACE_LEVEL_ERROR, "Deleting rule failed with error - " + FrameworkToolKit::localize(status));
			}
		}

		delete pCleanCacheMessage;
		delete pAAAContext;
		delete pWaveSendToClusterContext;

		pDeleteRuleMessage->setCompletionStatus(status);
		reply(pDeleteRuleMessage);
		trace (TRACE_LEVEL_DEBUG, "GlobalAuthorizationWorker::deleteRuleCleanCacheSTCCallback() EXIT");
	}
}
