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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.		  *
 *   All rights reserved.												  *
 *   Author : ckunduru													 *
 **************************************************************************/

#include <sstream>

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"

#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/GlobalAuthenticationWorker.h"
#include "AAA/Global/UserManagedObject.h"
#include "AAA/Global/RoleManagedObject.h"
#include "AAA/Global/PasswordEncryptionManagedObject.h"
#include "AAA/Global/PasswordAttributesManagedObject.h"
#include "AAA/Global/AAAGlobalAddUserMessage.h"
#include "AAA/Global/AAAGlobalChangeUserMessage.h"
#include "AAA/Global/AAAGlobalDeleteUserMessage.h"
#include "AAA/Global/AAAGlobalUserActionMessage.h"
#include "AAA/Global/AAAGlobalAddRoleMessage.h"
#include "AAA/Global/AAAGlobalChangeRoleMessage.h"
#include "AAA/Global/AAAGlobalDeleteRoleMessage.h"
#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Global/AAACrypto.h"
#include "AAA/Global/AAASequencerContexts.h"
#include "AAA/Local/AAALocalAddUserMessage.h"
#include "AAA/Local/AAALocalChangeUserMessage.h"
#include "AAA/Local/AAALocalDeleteUserMessage.h"
#include "AAA/Local/AAALocalUserActionMessage.h"
#include "AAA/Local/AAALocalAddRoleMessage.h"
#include "AAA/Local/AAALocalChangeRoleMessage.h"
#include "AAA/Local/AAALocalDeleteRoleMessage.h"
#include "AAA/Global/RuleManagedObject.h"
#include "AAA/Local/AAALocalCleanCacheMessage.h"

extern "C" {
	#include "security/public.h"
    #include "geps/push.h"
}

#define SEC_MAX_PASS_LEN    40

namespace DcmNs
{

GlobalAuthenticationWorker::GlobalAuthenticationWorker ( AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: WaveWorker  (pAAAGlobalObjectManager)
{
	UserManagedObject	userMO	(pAAAGlobalObjectManager);
	userMO.setupOrm ();
	addManagedClass (UserManagedObject::getClassName (), this);

	RoleManagedObject	roleMO	(pAAAGlobalObjectManager);
	roleMO.setupOrm ();
	addManagedClass (RoleManagedObject::getClassName (), this);

	addOperationMap (AAAGLOBALADDUSER, reinterpret_cast<PrismMessageHandler> (&GlobalAuthenticationWorker::AAAGlobalAddUserMessageHandler));
	addOperationMap (AAAGLOBALCHANGEUSER, reinterpret_cast<PrismMessageHandler> (&GlobalAuthenticationWorker::AAAGlobalChangeUserMessageHandler));
	addOperationMap (AAAGLOBALDELETEUSER, reinterpret_cast<PrismMessageHandler> (&GlobalAuthenticationWorker::AAAGlobalDeleteUserMessageHandler));
	addOperationMap (AAAGLOBALUSERACTION, reinterpret_cast<PrismMessageHandler> (&GlobalAuthenticationWorker::AAAGlobalUserActionMessageHandler));
	addOperationMap (AAAGLOBALADDROLE, reinterpret_cast<PrismMessageHandler> (&GlobalAuthenticationWorker::AAAGlobalAddRoleMessageHandler));
	addOperationMap (AAAGLOBALCHANGEROLE, reinterpret_cast<PrismMessageHandler> (&GlobalAuthenticationWorker::AAAGlobalChangeRoleMessageHandler));
	addOperationMap (AAAGLOBALDELETEROLE, reinterpret_cast<PrismMessageHandler> (&GlobalAuthenticationWorker::AAAGlobalDeleteRoleMessageHandler));
}

GlobalAuthenticationWorker::~GlobalAuthenticationWorker ()
{
}

PrismMessage  *GlobalAuthenticationWorker::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{
		case AAAGLOBALADDUSER :
			pPrismMessage = new AAAGlobalAddUserMessage ();
			break;
		case AAAGLOBALCHANGEUSER :
			pPrismMessage = new AAAGlobalChangeUserMessage ();
			break;
		case AAAGLOBALDELETEUSER :
			pPrismMessage = new AAAGlobalDeleteUserMessage ();
			break;
		case AAAGLOBALUSERACTION :
			pPrismMessage = new AAAGlobalUserActionMessage ();
			break;
		case AAAGLOBALADDROLE :
			pPrismMessage = new AAAGlobalAddRoleMessage ();
			break;
		case AAAGLOBALCHANGEROLE :
			pPrismMessage = new AAAGlobalChangeRoleMessage ();
			break;
		case AAAGLOBALDELETEROLE :
			pPrismMessage = new AAAGlobalDeleteRoleMessage ();
			break;
		default :
			pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject  *GlobalAuthenticationWorker::createManagedObjectInstance(const string &managedClassName)
{
	WaveManagedObject *pWaveManagedObject = NULL;

	if ((UserManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new UserManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((RoleManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new RoleManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "GlobalAuthenticationWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
	}

	return (pWaveManagedObject);
}

void  GlobalAuthenticationWorker::AAAGlobalAddUserMessageHandler( AAAGlobalAddUserMessage *pAAAGlobalAddUserMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalAuthenticationWorker::AAAGlobalAddUserMessageHandler()");
	if (pAAAGlobalAddUserMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	/* coverity - false positive
	 * prismAssert() is called if pAAAGlobalAddUserMessage is NULL
	 * Hence flow does not come here.
	 */
	string name = pAAAGlobalAddUserMessage->getName();
	string password = pAAAGlobalAddUserMessage->getPassword();
	SI8 encryptionLevel = pAAAGlobalAddUserMessage->getEncryptionLevel();
	string role = pAAAGlobalAddUserMessage->getRole();
	string description = pAAAGlobalAddUserMessage->getDescription();
	bool enabled = pAAAGlobalAddUserMessage->getEnabled();
	vector<UI32> locations;
	string loginUser = pAAAGlobalAddUserMessage->getLoginUser();
	string loginRole = pAAAGlobalAddUserMessage->getLoginRole();
	string loginIp = pAAAGlobalAddUserMessage->getLoginIp();
	string loginProto = pAAAGlobalAddUserMessage->getLoginProto();
	string loginAppName = pAAAGlobalAddUserMessage->getLoginAppName();

	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	string md5password = "";

	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	RoleManagedObject *pRoleObj = NULL;
	ObjectId roleObjId;
	string usersOfRole;

	/* Validate user parameters */
	if ((ret = paramValidate(name, USER_NAME)) != 0)
	{
		status = USER_INVALID_NAME;
		ret = -1;
	}
	else if (name.compare(ACCT_ROOT_ROLE) == 0)
	{
		trace (TRACE_LEVEL_ERROR, "'root' cannot be added");
		status = USER_NAME_RESERVED;
		ret = -1;
	}

	/* during getIsALastConfigReplay, Check if the user with same details exists in DB,
     * if so, it is duplicate command, just ignore.     */
    if (pAAAGlobalAddUserMessage->getIsALastConfigReplay() == true) {
		trace (TRACE_LEVEL_DEBUG, "pAAAGlobalAddUserMessage getIsALastConfigReplay true");
        synchronousQueryContext =
                new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());
        synchronousQueryContext->addOrAttribute(new AttributeString(name, "username"));
        pResults = querySynchronously (synchronousQueryContext);
        delete synchronousQueryContext;
        if ((NULL == pResults) || (pResults->size() != 1)) {
            trace (TRACE_LEVEL_DEBUG, "Last Cfg replay: User does not exist");
        } else if (pResults->size() == 1) { /* user has unique name */
            trace (TRACE_LEVEL_DEBUG, "Last Cfg replay: User already exist");
			for (UI32 j = 0; j < pResults->size(); j++)
                delete (*pResults)[j];
            delete pResults;
            pAAAGlobalAddUserMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pAAAGlobalAddUserMessage);
            return;
        }
    }

	if (0 == ret) {
		if ((encryptionLevel != -1) && (password.length() == 0))
		{
			status = USER_ENCLEVEL_NO_PASS;
			ret = -1;
		}
	}

	if (ret == 0)
	{
		/* Check if a role is assigned to the user */
		if (role.length() == 0)
		{
			ret = -1;
			status = USER_MANDATORY_ROLE;
		}
		else
		{
		/* Get the MO instance of the role */
		synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RoleManagedObject::getClassName());
		synchronousQueryContext->addOrAttribute(new AttributeString(role, "rolename"));
		/* Check if a role exists with the same user name */
		synchronousQueryContext->addOrAttribute(new AttributeString(name, "rolename"));
		pResults = querySynchronously (synchronousQueryContext);
		if ((NULL == pResults) || (pResults->size() != 1))
		{
			trace (TRACE_LEVEL_ERROR, "Role does not exist");
			status = ROLE_DOESNT_EXIST;
			if (pResults != NULL)
			{
				for (UI32 j = 0; j < pResults->size(); j++)
				{
					RoleManagedObject *tmpRole = dynamic_cast<RoleManagedObject *> ((*pResults)[j]);
					/* Coverity */
					if (NULL == tmpRole) {
						/* continue to the next entry */
						continue;
					}
					if (tmpRole->getName() == name)
					{
						status = USER_NAME_ROLE_MATCH;
					}
					delete (*pResults)[j];
				}
			}
			ret = -1;
		}
		else
		{
			pRoleObj = dynamic_cast<RoleManagedObject *> ((*pResults)[0]);
			/* Coverity  */
			if(NULL == pRoleObj)
			{
				status = ROLE_DOESNT_EXIST;
				ret = -1;
			}
			else if (pRoleObj->getName() == name)
			{
				status = USER_NAME_ROLE_MATCH;
				ret = -1;
			}
			else
			{
				roleObjId = pRoleObj->getObjectId();
			}
		}
		if (pResults) delete pResults;
		delete synchronousQueryContext;
		} // role.length > 0
	}

	if (ret == 0)
	{
		if (encryptionLevel < 0)
		{
			encryptionLevel = DEFAULT_ENCRYPTION_LEVEL;
		}
		else if ((encryptionLevel != ENCRYPTION_CLEAR_TEXT) && (encryptionLevel != ENCRYPTION_HASH))
		{
			status = USER_INVALID_ENCRYPTION_LEVEL;
			ret = -1;
		}
	}

	if (ret == 0)
	{
		password = AAACrypto::decryptPassword(password);
		if (password.length() == 0)
		{
			trace (TRACE_LEVEL_ERROR, "Password Decryption failed");
			status = USER_PASSWD_ENCDEC_ERROR;
			ret = -1;
		}

		/* Get the cleartext password for MD5 if it is already encrypted */
		string inputPswd = password;
		if (ret == 0 && encryptionLevel == ENCRYPTION_HASH)
		{
			password = AAACrypto::decryptPassword(password);
			if (password.length() == 0)
			{
				trace (TRACE_LEVEL_ERROR, "Invalid encrypted password");
				status = USER_INVALID_ENCRYPTED_PASSWORD;
				ret = -1;
			}
		}

		if (ret == 0) {
			/* See if it is a 'show' password such as '*******' or 'xxxxxx' etc i.e having just the same character */
			char ch = password[0];
			for (UI32 i = 1; i < password.length(); i++)
			{
				if (ch != password[i])
				{
					ch = 0;
					break;
				}
			}
			if (ch != 0 && password.length() != 1)
			{
				trace (TRACE_LEVEL_ERROR, string("Password contains only character '") + ch + string("'"));
				status = USER_INVALID_PASSWORD;
				ret = -1;
			}

			/* Check if the password exceeds maximum limit */
			if (password.length() > SEC_MAX_PASS_LEN)
			{
				trace (TRACE_LEVEL_ERROR, string("Password is too long with length=") + password.length());
				status = USER_PASSWORD_ERROR;
				pAAAGlobalAddUserMessage->setMessageString(SEC_MAX_PASS_LEN + string(" characters of maximum length"));
				ret = -1;
			}
		}

		// Convert the plain text to md5sum hash
		if (ret == 0) {
			char *md5hash = NULL;
			ret = this->setGEPS(pAAAGlobalAddUserMessage->getLoginUser());
			if (ret == 0) {
				// FOS PAM Password Module enforces password rules as part of encrypting the string
				ret = secEncryptedPasswdGet(name.c_str(), PAM_1ST_PASSWD_SERVICE, NULL, password.c_str(),
   	    	    	                    NULL, NULL, &md5hash);
				if (ret != 0)
				{
					if ((ret <= SEC_PWERR_LOWER)
							  && (ret >= SEC_PWERR_INVALID_CHAR )) {
						string errSt = this->setErrorMessage(status, "add" );
						pAAAGlobalAddUserMessage->setMessageString(errSt);
						trace (TRACE_LEVEL_ERROR, "AdduserMessageHandler::secEncryptedPasswdGet returned - " + errSt);
                        status = USER_PASSWORD_ERROR;
					}
					else
					{
						trace (TRACE_LEVEL_ERROR, string("Producing md5hash of the password failed. - ") + ret);
						status = USER_PASSWORD_PAM_ERROR;
					}
				}
				else
				{
					md5password.assign(md5hash);
					password = inputPswd; // In case of Encrypted password input, assign the original back.
				}

				if (md5hash != NULL)
					free (md5hash);
			}
			else
			{
					trace (TRACE_LEVEL_ERROR, "Setting GEPS failed for " + pAAAGlobalAddUserMessage->getLoginUser());
					status = USER_ADD_ERROR;
			}
		}
	}

	if (ret == 0)
	{
		if (description.length() > 0)
		{
			if ((ret = paramValidate(description, USER_DESCRIPTION)) != 0)
				status = USER_INVALID_DESC;
		}
	}

	if ((ret == 0) && (pAAAGlobalAddUserMessage->getEnabledFlag() == false))
	{
		enabled = true; // default
	}

	if (ret != 0)
	{
		if (pRoleObj != NULL) delete pRoleObj;
		pAAAGlobalAddUserMessage->setCompletionStatus(status);
		reply (pAAAGlobalAddUserMessage);
		return;
	}
	else
	{
		trace (TRACE_LEVEL_DEBUG, "User input validation OK.");
	}

	/* Encrypt the password if 'service password-encryption' is set */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(PasswordEncryptionManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	if ((pResults == NULL) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_ERROR, "Received Null/Zero PasswordEncryptionManagedObject from the DB");
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];
			delete pResults;
		}
		delete synchronousQueryContext;
		pAAAGlobalAddUserMessage->setCompletionStatus(USER_ADD_ERROR);
		reply (pAAAGlobalAddUserMessage);
		delete pRoleObj;
		return;
	}

	PasswordEncryptionManagedObject *pPasswordEncryptionObj = dynamic_cast<PasswordEncryptionManagedObject *>((*pResults)[0]);

	delete pResults;
	delete synchronousQueryContext;

	/* Coverity */
	if (NULL == pPasswordEncryptionObj)
	{
		trace (TRACE_LEVEL_ERROR, "Failed to retrive Password encription object");
		pAAAGlobalAddUserMessage->setCompletionStatus(USER_ADD_ERROR);
		reply (pAAAGlobalAddUserMessage);
		delete pRoleObj;
		return;
	}
	if ((pPasswordEncryptionObj->getPasswordEncryption() == true) && (encryptionLevel != ENCRYPTION_HASH))
	{
		encryptionLevel = ENCRYPTION_HASH;
		password = AAACrypto::encryptPassword(password);
		if (password.length() == 0)
		{
			trace (TRACE_LEVEL_ERROR, "Encrypting the password failed");
			delete pPasswordEncryptionObj;
			pAAAGlobalAddUserMessage->setCompletionStatus(USER_ADD_ERROR);
			reply (pAAAGlobalAddUserMessage);
			delete pRoleObj;
			return;
		}
		trace (TRACE_LEVEL_INFO, "Password Encryption is ON. Encrypted the clear-text password for storing.");
	}
	delete pPasswordEncryptionObj;

	/* Check if the max limit for users on local db has reached */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	delete synchronousQueryContext;
	if (pResults != NULL)
	{
		UI32 userNum = pResults->size();

		for (UI32 j = 0; j < pResults->size(); j++)
			delete (*pResults)[j];
		delete pResults;

		if (userNum == ACCT_MAX_NUM)
		{
			pAAAGlobalAddUserMessage->setCompletionStatus(USER_MAX_LIMIT);
			reply (pAAAGlobalAddUserMessage);
			delete pRoleObj;
			return;
		}
	}

	/* Update each role with the new user */
	usersOfRole = pRoleObj->getUsers();
	if (usersOfRole.length() != 0)
		usersOfRole.append(ROLE_USERS_DELIMITER);
	usersOfRole.append(name);
	trace (TRACE_LEVEL_INFO, "Appended users : *" + usersOfRole + "* to role : *" + pRoleObj->getName());

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	// Send message to all nodes of the cluster
	AAALocalAddUserMessage *pAddMessage = new AAALocalAddUserMessage (name, md5password, role, usersOfRole, description, enabled, loginUser, loginRole, loginIp, loginProto, loginAppName);

	AAAAddUserContext *pAddUserContext = new AAAAddUserContext (pAAAGlobalAddUserMessage, pRoleObj, usersOfRole, password, encryptionLevel);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::addUserSTCCallback), pAddUserContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pAddMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalAuthenticationWorker::addUserSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalAuthenticationWorker::addUserSTCCallback()");

	AAAAddUserContext *pAAAContext = (AAAAddUserContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalAddUserMessage *pLocalAddMsg = (AAALocalAddUserMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;
	vector<UI32> locations;

	if (FrameworkToolKit::isConfigReplayInProgress())
		trace (TRACE_LEVEL_INFO, "User is added as part of Config Replay.");
	else
		trace (TRACE_LEVEL_INFO, "User is added from command line.");

	AAAGlobalAddUserMessage *pAAAGlobalAddUserMessage = (AAAGlobalAddUserMessage *)pAAAContext->getPPrismMessage();

	RoleManagedObject *pRoleObj = pAAAContext->getRoleObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Add user operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Add user operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalAuthenticationWorker::addUserSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? USER_ADD_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? USER_ADD_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (status == WAVE_MESSAGE_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "addUserSTCCallback : SUCCESS on local plugins. Commiting to DB ...");

		startTransaction ();

		UserManagedObject *pUserObj = dynamic_cast<UserManagedObject *>(createManagedObjectInstance(UserManagedObject::getClassName()));

		/* Coverity : false positive*/
		pUserObj->setName(pLocalAddMsg->getName());
		pUserObj->setPassword(pAAAContext->getPassword());
		pUserObj->setEncryptionLevel((AAAEncryptionLevel)pAAAContext->getEncryptionLevel());
		pUserObj->setRole(pRoleObj->getObjectId());
		pUserObj->setDescription(pLocalAddMsg->getDescription());
		pUserObj->setEnabled(pLocalAddMsg->getEnabled());

		pUserObj->setMD5Password(pLocalAddMsg->getPassword());
		pUserObj->setUid(ACCT_SYS_DEF_PW_UID);
		pUserObj->setGid(pRoleObj->getGID()); //Assign the GID of the first role in the list
		pUserObj->setSsh_keydir(ACCT_SYS_DEF_PW_SSH_DIR);
		pUserObj->setHomedir(ACCT_SYS_DEF_PW_DIR);

		/* Update each role with the new user */
		updateWaveManagedObject (pRoleObj);
		pRoleObj->setUsers(pAAAContext->getUsrsOfRole());

		/* Commit the changes in MO instances to the DB */
		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = USER_ADD_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting the User configuration to Wave database is successful.");
		}
		else
		{
			status = USER_ADD_DB_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting the User configuration failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
		delete pUserObj;
	}
	pAAAGlobalAddUserMessage->setMessageString(errorMessage);

	delete pLocalAddMsg;

	delete pAAAContext; // deletes pRoleObj internally
	delete pWaveSendToClusterContext;

	if ((status != WAVE_MESSAGE_SUCCESS) || FrameworkToolKit::isConfigReplayInProgress())
	{
		/* we are not invoking cache reload, we'll reply here */
		pAAAGlobalAddUserMessage->setCompletionStatus(status);
		reply(pAAAGlobalAddUserMessage);
		trace (TRACE_LEVEL_INFO, "Exiting without aaa-clear-cache.");
	}
	else
	{
		vector<string> pathVec;
		string path = string("/aaa:aaa/authentication/users/user{") + pLocalAddMsg->getName() + "}" ;
		pathVec.push_back(path);
		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

		AAALocalCleanCacheMessage *pSetCacheMessage = new AAALocalCleanCacheMessage ("Add-User-Operation", NONE_HAS_DEFAULT_PASSWD, pathVec);
		AAACleanCacheContext *pCleanCacheContext = new AAACleanCacheContext (pAAAGlobalAddUserMessage);
		WaveSendToClusterContext *pWaveSendToClusterContextCache = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::cleanAAACacheSTCCallback), pCleanCacheContext);
		pWaveSendToClusterContextCache->setPPrismMessageForPhase1(pSetCacheMessage);
		if(!locations.empty()) {
			pWaveSendToClusterContextCache->setLocationsToSendToForPhase1(locations);
		}
		sendToWaveCluster (pWaveSendToClusterContextCache);
	}
	trace (TRACE_LEVEL_INFO, "AAAGlobalAddUserMessageHandler() - EXIT.");
}

void  GlobalAuthenticationWorker::AAAGlobalChangeUserMessageHandler( AAAGlobalChangeUserMessage *pAAAGlobalChangeUserMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalAuthenticationWorker::AAAGlobalChangeUserMessageHandler()");
	if (pAAAGlobalChangeUserMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	/* coverity - false positive
	 * prismAssert() is called if pAAAGlobalAddUserMessage is NULL
	 * Hence flow does not come here.
	 */
	string name = pAAAGlobalChangeUserMessage->getName();
	string password = pAAAGlobalChangeUserMessage->getPassword();
	SI8 encryptionLevel = pAAAGlobalChangeUserMessage->getEncryptionLevel();
	string role = pAAAGlobalChangeUserMessage->getRole();
	string description = pAAAGlobalChangeUserMessage->getDescription();
	bool enabled = pAAAGlobalChangeUserMessage->getEnabled();
	string loginUser = pAAAGlobalChangeUserMessage->getLoginUser();
	string loginRole = pAAAGlobalChangeUserMessage->getLoginRole();
	string loginIp = pAAAGlobalChangeUserMessage->getLoginIp();
	string loginProto = pAAAGlobalChangeUserMessage->getLoginProto();
	string loginAppName = pAAAGlobalChangeUserMessage->getLoginAppName();
	int gid = -1;
	ObjectId newRoleObjId;
	vector<UI32> locations;

	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	string md5password("");

	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	UserManagedObject *pUserObj = NULL;

	RoleManagedObject *pRoleObj = NULL;
	ObjectId roleObjId;
	string usersOfRole, oldUsersOfRole;

	RoleManagedObject *pOldRoleObj = NULL;
	ObjectId oldRoleObjId;
	string usersOfOldRole;

	string oldRoleName;
	string users;

	/* Check if the user being changed is a default user */
	if (name.compare("user") == 0 || name.compare("admin") == 0)
	{
		// Any attribute except password & enable cannot be changed
		if ((role.length() > 0) || (pAAAGlobalChangeUserMessage->getDescFlag() == true))
		{
			trace (TRACE_LEVEL_ERROR, "Except password & enable, attribute(s) of a default account cannot be changed.");
			pAAAGlobalChangeUserMessage->setCompletionStatus (USER_CHANGE_DEFAULT);
			reply (pAAAGlobalChangeUserMessage);
			return;
		}
	}

	/* Get the User Managed Object for optional parameters */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());
	synchronousQueryContext->addAndAttribute(new AttributeString(name, "username"));
	pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_FATAL, "User does not exist");
		pAAAGlobalChangeUserMessage->setCompletionStatus (USER_DOESNT_EXIST);
		reply (pAAAGlobalChangeUserMessage);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		return;
	}
	pUserObj = dynamic_cast<UserManagedObject *> ((*pResults)[0]);

	delete synchronousQueryContext;
	delete pResults;

	/* Coverity */
	if (NULL == pUserObj)
	{
		trace (TRACE_LEVEL_FATAL, "Failed to get the user details");
		pAAAGlobalChangeUserMessage->setCompletionStatus (USER_DOESNT_EXIST);
		reply (pAAAGlobalChangeUserMessage);
		return;
	}
/*
      else
        {                   
		// Logic to keep at least one [the last] admin privilege acc available by not allowing to disable it.     

                ObjectId tmpRoleObjId;
                RoleManagedObject *pTmpRoleObj = NULL;
                string tmpRoleName,tmpUsersOfRole;
                UI32 count = 0;

                if ( (pAAAGlobalChangeUserMessage->getEnabledFlag() == true) && (enabled == false) )
  	        {
		    if ( enabled != pUserObj->getEnabled() )
		    {
	                synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());

        	        // Get role object-id from user object
                	tmpRoleObjId = pUserObj->getRole();

	                synchronousQueryContext->addAndAttribute (new AttributeObjectId (tmpRoleObjId, "role"));
        	        synchronousQueryContext->addAndAttribute (new AttributeBool(true, "enabled"));
                	querySynchronouslyForCount (synchronousQueryContext, count);
	                delete synchronousQueryContext;

        	        // Get role-name from user's current role object-id
                	pTmpRoleObj = dynamic_cast<RoleManagedObject *> (queryManagedObject(tmpRoleObjId));
	                tmpRoleName = pTmpRoleObj->getName();
        	        delete pTmpRoleObj;

                	if( count < 2 && (tmpRoleName.compare("admin") == 0))
	                {   //OO this is the last admin previledge account.
     	                    trace (TRACE_LEVEL_ERROR, "Except password, attribute(s) of last admin account cannot be changed.");
			    // Uncomment USER_LAST_ADMIN_CHANGE	from AAA.xml 
                            pAAAGlobalChangeUserMessage->setCompletionStatus(USER_LAST_ADMIN_CHANGE);
	                    reply (pAAAGlobalChangeUserMessage);
        	            return;                	        
	                }            
	            }
	        }
	}
*/
	/* Retrieve and validate the input parameters */
	if ((ret == 0) && (password.length() > 0))
	{
		password = AAACrypto::decryptPassword(password);
		if (password.length() == 0)
		{
			trace (TRACE_LEVEL_ERROR, "Password Decryption failed");
			status = USER_PASSWD_ENCDEC_ERROR;
			ret = -1;
		}

		// Get the cleartext password for MD5 if it is already encrypted
		string inputPswd = password;
		if (ret == 0 && encryptionLevel == -1)
		{
			// Check if encryption-level is not received from the command line
			if (pUserObj->getEncryptionLevel() == ENCRYPTION_HASH)
			{
				// See if the received password is encrypted word
				if ((AAACrypto::decryptPassword(inputPswd)).length() > 0)
				{
					encryptionLevel = ENCRYPTION_HASH;
				}
				else
				{
					encryptionLevel = ENCRYPTION_CLEAR_TEXT;
				}
			}
		}

		if (ret == 0 && encryptionLevel == ENCRYPTION_HASH)
		{
			password = AAACrypto::decryptPassword(password);
			if (password.length() == 0)
			{
				trace (TRACE_LEVEL_ERROR, "Invalid encrypted password");
				status = USER_INVALID_ENCRYPTED_PASSWORD;
				ret = -1;
			}
		}

		if (ret == 0) {
			// See if it is '*******' or 'xxxxxx' etc i.e having just the same character
			char ch = password[0];
			for (UI32 i = 1; i < password.length(); i++)
			{
				if (ch != password[i])
				{
					ch = 0;
					break;
				}
			}
			if (ch != 0 && password.length() != 1)
			{
				trace (TRACE_LEVEL_ERROR, string("Password contains only character '") + ch + string("'"));
				status = USER_INVALID_PASSWORD;
				ret = -1;
			}

			// Check if the password exceeds maximum limit
			if (password.length() > SEC_MAX_PASS_LEN)
			{
				trace (TRACE_LEVEL_ERROR, string("Password is too long with length=") + password.length());
				status = USER_PASSWORD_ERROR;
				pAAAGlobalChangeUserMessage->setMessageString(SEC_MAX_PASS_LEN + string(" character maximum length"));
				ret = -1;
			}
		}

		// Computer MD5 Hash of the clear text password
		if (ret == 0) {
			char *md5hash = NULL;
			ret = this->setGEPS(pAAAGlobalChangeUserMessage->getLoginUser());
			if (ret == 0) {
				// FOS PAM Password Module enforces password rules as part of encrypting the string
				ret = secEncryptedPasswdGet(name.c_str(), PAM_1ST_PASSWD_SERVICE, NULL, password.c_str(),
   	    	    	                    NULL, NULL, &md5hash);
				if (ret != 0)
				{
					if ((ret <= SEC_PWERR_LOWER) && (ret >= SEC_PWERR_INVALID_CHAR ))
					{
						string errSt = this->setErrorMessage(status, "change" );
						pAAAGlobalChangeUserMessage->setMessageString(errSt);
						trace (TRACE_LEVEL_ERROR, "ChangeuserMessageHandler::secEncryptedPasswdGet returned - " + errSt);
					}
					else
					{
						trace (TRACE_LEVEL_ERROR, string("Producing md5hash of the password failed. - ") + ret);
						status = USER_PASSWORD_PAM_ERROR;
					}
				}
				else
				{
					md5password.assign(md5hash);
					password = inputPswd; // In case of Encrypted password input, assign the original back.
				}

				if (md5hash != NULL)
					free (md5hash);
			}
			else
			{
					trace (TRACE_LEVEL_ERROR, "Setting GEPS failed for " + pAAAGlobalChangeUserMessage->getLoginUser());
					status = USER_CHANGE_ERROR;
			}
		}

		/* Validate the encryption level, if given */
		if (ret == 0)
		{
			if (encryptionLevel < 0)
			{
				encryptionLevel = DEFAULT_ENCRYPTION_LEVEL;
			}
			else if ((encryptionLevel != ENCRYPTION_CLEAR_TEXT) && (encryptionLevel != ENCRYPTION_HASH))
			{
				status = USER_INVALID_ENCRYPTION_LEVEL;
				ret = -1;
			}
		}
	}
	else
	{
		password = pUserObj->getPassword();
		encryptionLevel = pUserObj->getEncryptionLevel();
		md5password = pUserObj->getMD5Password();
	}

    if (ret == 0)
    {
		if (role.length() > 0)
		{
			/* Get the MO instances of the new role */
			synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RoleManagedObject::getClassName());
			synchronousQueryContext->addAndAttribute(new AttributeString(role, "rolename"));
			pResults = querySynchronously (synchronousQueryContext);
			if ((NULL == pResults) || (pResults->size() != 1))
			{
				trace (TRACE_LEVEL_FATAL, "Role does not exist");
				status = ROLE_DOESNT_EXIST;
				ret = -1;
				if (pResults != NULL) {
					for (UI32 j = 0; j < pResults->size(); j++)
						delete (*pResults)[j];
				}
			}
			else
			{
				pRoleObj = dynamic_cast<RoleManagedObject *> ((*pResults)[0]);
				/* Coverity */
				if (NULL == pRoleObj)
				{
					trace (TRACE_LEVEL_FATAL, "Failed to retrive role");
					status = ROLE_DOESNT_EXIST;
					ret = -1;
					if (pResults != NULL) {
						for (UI32 j = 0; j < pResults->size(); j++)
							delete (*pResults)[j];
					}
				}
				else
				{
				roleObjId = pRoleObj->getObjectId();
				oldUsersOfRole = pRoleObj->getUsers();
			}
			}
			if (pResults) delete pResults;
			delete synchronousQueryContext;

			if (ret == 0)
				newRoleObjId = roleObjId;
		}

		if (ret == 0)
		{
			/* Get role-name from user's current role object-id */
			oldRoleObjId = pUserObj->getRole();
			pOldRoleObj = dynamic_cast<RoleManagedObject *> (queryManagedObject(oldRoleObjId));
			oldRoleName = pOldRoleObj->getName();
		}
    }

	if (ret == 0)
	{
		if (description.length() > 0)
		{
			if ((ret = paramValidate(description, USER_DESCRIPTION)) != 0)
				status = USER_INVALID_DESC;
		}
		else if (pAAAGlobalChangeUserMessage->getDescFlag() == false) // desc is not defaulted
		{
			description = pUserObj->getDescription();
		}
	}

	if ((ret == 0) && (pAAAGlobalChangeUserMessage->getEnabledFlag() == false))
	{
		enabled = pUserObj->getEnabled();
	}

	if (ret != 0)
	{
		delete pUserObj;
		if (pRoleObj) delete pRoleObj;
		if (pOldRoleObj) delete pOldRoleObj;
		pAAAGlobalChangeUserMessage->setCompletionStatus(status);
		reply (pAAAGlobalChangeUserMessage);
		return;
	}
	else
	{
		trace (TRACE_LEVEL_DEBUG, "User input validation OK.");
	}

	/* Encrypt the password if 'service password-encryption' is set */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(PasswordEncryptionManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	delete synchronousQueryContext;
	if ((pResults == NULL) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_ERROR, "Retrieved Null/Zero PasswordEncryptionManagedObject from the DB.");
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];
			delete pResults;
		}
		delete pUserObj;
		if (pRoleObj) delete pRoleObj;
		if (pOldRoleObj) delete pOldRoleObj;
		pAAAGlobalChangeUserMessage->setCompletionStatus(USER_CHANGE_ERROR);
		reply (pAAAGlobalChangeUserMessage);
		return;
	}

	PasswordEncryptionManagedObject *pPasswordEncryptionObj = dynamic_cast<PasswordEncryptionManagedObject *>((*pResults)[0]);
	delete pResults;

	/* coverity */
	if (NULL == pPasswordEncryptionObj)
	{
		trace (TRACE_LEVEL_ERROR, "FAiled to get the password encription object");
		delete pUserObj;
		if (pRoleObj) delete pRoleObj;
		if (pOldRoleObj) delete pOldRoleObj;
		pAAAGlobalChangeUserMessage->setCompletionStatus(USER_CHANGE_ERROR);
		reply (pAAAGlobalChangeUserMessage);
		return;
	}
	if ((pPasswordEncryptionObj->getPasswordEncryption() == true) && (encryptionLevel != ENCRYPTION_HASH))
	{
		encryptionLevel = ENCRYPTION_HASH;
		password = AAACrypto::encryptPassword(password);
		if (password.length() == 0)
		{
			trace (TRACE_LEVEL_ERROR, "Encrypting the password failed");
			delete pPasswordEncryptionObj;
			delete pUserObj;
			if (pRoleObj) delete pRoleObj;
			if (pOldRoleObj) delete pOldRoleObj;
			pAAAGlobalChangeUserMessage->setCompletionStatus(USER_CHANGE_ERROR);
			reply (pAAAGlobalChangeUserMessage);
			return;
		}
		trace (TRACE_LEVEL_INFO, "Password Encryption is ON. Encrypted the clear-text password for storing.");
	}

	delete pPasswordEncryptionObj;

	/* Role change related functionality */
	if (pRoleObj != NULL)
	{
		/* Get GID of the new role */
		gid = pRoleObj->getGID();
		trace (TRACE_LEVEL_INFO, string("The user's gid is now ") + gid);

		/* Update the new role to include the user */
		users = pRoleObj->getUsers();
		if (users.length() != 0) users.append(ROLE_USERS_DELIMITER);
		users.append(pUserObj->getName());
		usersOfRole = users;
		trace (TRACE_LEVEL_DEBUG, "Appended users : *" + users + "* to role : " + pRoleObj->getName());

		/* Update the old role to exclude the user */
		users = pOldRoleObj->getUsers();
		UI32 usrlen = (pUserObj->getName()).length();
		UI32 len = users.length();
		size_t pos = users.find(pUserObj->getName());
		if (pos != string::npos) {
			if (pos == 0) {
				if (usrlen == len) {
					users.erase();
				} else {
					users = users.substr(usrlen + 1, len - 1);
				}
			} else {
				if (len == pos + usrlen) {
					users = users.substr(0, pos - 1);
				} else {
					users = users.substr(0, pos - 1) + users.substr(pos + usrlen, len - 1);
				}
			}
		}
		usersOfOldRole = users;
		trace (TRACE_LEVEL_DEBUG, "Excluded user to *" + users + "* from old role *" + pOldRoleObj->getName());
	}

	/* during getIsALastConfigReplay, Check if the user with same details exists in DB,
	 * if so, it is duplicate command, just ignore.
	 */
	if (pAAAGlobalChangeUserMessage->getIsALastConfigReplay() == true) {
		trace (TRACE_LEVEL_DEBUG, "pAAAGlobalChangeUserMessage getIsALastConfigReplay true");
		synchronousQueryContext =
			new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());
		synchronousQueryContext->addOrAttribute(new AttributeString(name, "username"));
		if (role.length() > 0) {
			synchronousQueryContext->addOrAttribute(new AttributeString(role, "role"));
		}
		if (md5password.length() > 0) {
			synchronousQueryContext->addOrAttribute(new  AttributeString(md5password, "md5password"));
		}
		if (description.length() > 0) {
			synchronousQueryContext->addOrAttribute(new AttributeString(description, "description"));
		}
		if (encryptionLevel > 0) {
			synchronousQueryContext->addOrAttribute(new AttributeUI32(encryptionLevel, "encryption_level"));
		}
		if (pAAAGlobalChangeUserMessage->getEnabledFlag() == true) {
			synchronousQueryContext->addOrAttribute(new AttributeBool(enabled, "enabled"));
		}
		pResults = querySynchronously (synchronousQueryContext);
		delete synchronousQueryContext;
		if ((NULL == pResults) || (pResults->size() != 1)) {
			/*No entru in DB, proceed with change Handler */
			trace (TRACE_LEVEL_DEBUG, "Last Cfg replay: similar user does not exist");
		} else if (pResults->size() == 1) { /* user has unique entry in DB */
			trace (TRACE_LEVEL_DEBUG, "Last Cfg replay: User already exist");
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];
			delete pResults;
			pAAAGlobalChangeUserMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			reply(pAAAGlobalChangeUserMessage);
			if (pUserObj) delete pUserObj;
            if (pRoleObj) delete pRoleObj;
            if (pOldRoleObj) delete pOldRoleObj;
			return;
		}
	}

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	// Send messages to all local services
	AAALocalChangeUserMessage *pMessage = new AAALocalChangeUserMessage (name, md5password, role, usersOfRole, oldRoleName, usersOfOldRole, description, pAAAGlobalChangeUserMessage->getDescFlag(), enabled, loginUser, loginRole, loginIp, loginProto, loginAppName);

	AAAChangeUserContext *pChangeUserContext = new AAAChangeUserContext (pAAAGlobalChangeUserMessage, pUserObj, pRoleObj, pOldRoleObj, password, encryptionLevel);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::changeUserSTCCallback), pChangeUserContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalAuthenticationWorker::changeUserSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalAuthenticationWorker::changeUserSTCCallback()");

    AAAChangeUserContext *pAAAContext = (AAAChangeUserContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalChangeUserMessage *pSetMessage = (AAALocalChangeUserMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;
	vector<UI32> locations;

	AAAGlobalChangeUserMessage *pAAAGlobalChangeUserMessage = (AAAGlobalChangeUserMessage *)pAAAContext->getPPrismMessage();
	UserManagedObject *pUserObj = pAAAContext->getUserObj();
	RoleManagedObject *pRoleObj = pAAAContext->getRoleObj();
	RoleManagedObject *pOldRoleObj = pAAAContext->getOldRoleObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Change user operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Change user operation failed on all nodes");

		vector<LocationId> cxLocations =  pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		//FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalAuthenticationWorker::changeUserSTCCallback", errorMessage);
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
		startTransaction ();

		/* Update the user */
		updateWaveManagedObject (pUserObj);

		pUserObj->setPassword(pAAAContext->getPassword());
		pUserObj->setEncryptionLevel((AAAEncryptionLevel)pAAAContext->getEncryptionLevel());
		pUserObj->setMD5Password(pSetMessage->getPassword());
		pUserObj->setDescription(pSetMessage->getDescription());
		pUserObj->setEnabled(pSetMessage->getEnabled());

		if (pRoleObj != NULL)
		{
			pUserObj->setRole(pRoleObj->getObjectId());
			pUserObj->setGid(pRoleObj->getGID());

			/* Update the new role */
			updateWaveManagedObject (pRoleObj);
			pRoleObj->setUsers(pSetMessage->getUsersOfRole());

			/* Update the old role */
			updateWaveManagedObject (pOldRoleObj);
			pOldRoleObj->setUsers(pSetMessage->getUsersOfOldRole());
		}

		/* Commit the changes in MO instances to the DB */
		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = USER_CHANGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Wave database commit is successful.");
		}
		else
		{
			status = USER_CHANGE_DB_ERROR;
			trace (TRACE_LEVEL_ERROR, "Wave DB commit failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}
	pAAAGlobalChangeUserMessage->setMessageString(errorMessage);

	delete pUserObj;
	if (pRoleObj) delete pRoleObj;
	if (pOldRoleObj) delete pOldRoleObj;

	delete pSetMessage;

	delete pAAAContext;
	delete pWaveSendToClusterContext;

	if ((status != WAVE_MESSAGE_SUCCESS) || FrameworkToolKit::isConfigReplayInProgress())
	{
		pAAAGlobalChangeUserMessage->setCompletionStatus(status);
		reply (pAAAGlobalChangeUserMessage);
	}
	else
	{
		vector<string> pathVec;
		string path = string("/aaa:aaa/authentication/users/user{") + pAAAGlobalChangeUserMessage->getName() + "}" ;
		pathVec.push_back(path);
		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

		AAALocalCleanCacheMessage *pSetCacheMessage = new AAALocalCleanCacheMessage ("Change-User-Operation", NONE_HAS_DEFAULT_PASSWD, pathVec);
		AAACleanCacheContext *pCleanCacheContext = new AAACleanCacheContext (pAAAGlobalChangeUserMessage);
		WaveSendToClusterContext *pWaveSendToClusterContextCache = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::cleanAAACacheSTCCallback), pCleanCacheContext);
		pWaveSendToClusterContextCache->setPPrismMessageForPhase1(pSetCacheMessage);
		if(!locations.empty()) {
			pWaveSendToClusterContextCache->setLocationsToSendToForPhase1(locations);
		}
		sendToWaveCluster (pWaveSendToClusterContextCache);
	}

	trace (TRACE_LEVEL_DEBUG, "AAAGlobalChangeUserMessageHandler() - EXIT.");
}

void  GlobalAuthenticationWorker::AAAGlobalDeleteUserMessageHandler( AAAGlobalDeleteUserMessage *pAAAGlobalDeleteUserMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalAuthenticationWorker::AAAGlobalDeleteUserMessageHandler()");
	if (pAAAGlobalDeleteUserMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	/* Coverity false positive
	 * Flow does not come here, since prismAssert is called in case of NULL.
	 */
	string name = pAAAGlobalDeleteUserMessage->getName();
	 vector<UI32> locations;
	string loginUser = pAAAGlobalDeleteUserMessage->getLoginUser();
	string loginRole = pAAAGlobalDeleteUserMessage->getLoginRole();
	string loginIp = pAAAGlobalDeleteUserMessage->getLoginIp();
	string loginProto = pAAAGlobalDeleteUserMessage->getLoginProto();
	string loginAppName = pAAAGlobalDeleteUserMessage->getLoginAppName();

	/* Check if the user being deleted is a default user */
	if (name.compare("user") == 0 || name.compare("admin") == 0)
	{
		pAAAGlobalDeleteUserMessage->setCompletionStatus (USER_DELETE_DEFAULT);
		reply (pAAAGlobalDeleteUserMessage);
		return;
	}

	/* Get the User Managed Object */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());

	synchronousQueryContext->addAndAttribute(new AttributeString(name, "username"));
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_ERROR, "User does not exist");
		if (pAAAGlobalDeleteUserMessage->getIsALastConfigReplay() == true) {
			trace (TRACE_LEVEL_DEBUG, "pAAAGlobalDeleteUserMessage getIsALastConfigReplay true");
			pAAAGlobalDeleteUserMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		}
		else {
			pAAAGlobalDeleteUserMessage->setCompletionStatus (USER_DOESNT_EXIST);
		}
		reply (pAAAGlobalDeleteUserMessage);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		return;
	}
	UserManagedObject *pUserObj = dynamic_cast<UserManagedObject *> ((*pResults)[0]);

	delete pResults;
	delete synchronousQueryContext;

	/* Coverity */
	if (NULL == pUserObj)
	{
		trace (TRACE_LEVEL_ERROR, "FAiled to retrive the user obj");
		pAAAGlobalDeleteUserMessage->setCompletionStatus (USER_DOESNT_EXIST);
        reply (pAAAGlobalDeleteUserMessage);
		return;
	}
	/* Get the managed object of role of the user */
	ObjectId roleObjId = pUserObj->getRole();
	RoleManagedObject *pRoleObj = dynamic_cast<RoleManagedObject *> (queryManagedObject(roleObjId));
	/* Coverity */
	if (NULL == pRoleObj)
	{
		trace (TRACE_LEVEL_ERROR, "FAiled to retrive the role obj");
		pAAAGlobalDeleteUserMessage->setCompletionStatus (ROLE_DOESNT_EXIST);
		reply (pAAAGlobalDeleteUserMessage);
		return;
	}
	string role = pRoleObj->getName();
	trace (TRACE_LEVEL_DEBUG, "To rollback for error, retreived role from DB - " + pRoleObj->getName());

	/* Update role to exclude the user */
	string usersOfRole;
	string users = pRoleObj->getUsers();
	UI32 usrlen = (pUserObj->getName()).length();
	UI32 len = users.length();
	size_t pos = users.find(pUserObj->getName());
	if (pos != string::npos) {
		if (pos == 0) {
			if (usrlen == len) {
				users.erase();
			} else {
				users = users.substr(usrlen + 1, len - 1);
			}
		} else {
			if (len == pos + usrlen) {
				users = users.substr(0, pos - 1);
			} else {
				users = users.substr(0, pos - 1) + users.substr(pos + usrlen, len - 1);
			}
		}
	}
	usersOfRole = users;

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	// Send message to all local services
	AAALocalDeleteUserMessage *pMessage = new AAALocalDeleteUserMessage (name, role, usersOfRole, loginUser, loginRole, loginIp, loginProto, loginAppName);

	AAADeleteUserContext *pDeleteUserContext = new AAADeleteUserContext (pAAAGlobalDeleteUserMessage, pUserObj, pRoleObj);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::deleteUserSTCCallback), pDeleteUserContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalAuthenticationWorker::deleteUserSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalAuthenticationWorker::deleteUserSTCCallback()");

	AAADeleteUserContext *pAAAContext = (AAADeleteUserContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalDeleteUserMessage *pMessage = (AAALocalDeleteUserMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;
	vector<UI32> locations; 

	AAAGlobalDeleteUserMessage *pAAAGlobalDeleteUserMessage = (AAAGlobalDeleteUserMessage *)pAAAContext->getPPrismMessage();
	UserManagedObject *pUserObj = pAAAContext->getUserObj();
	RoleManagedObject *pRoleObj = pAAAContext->getRoleObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Delete user operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Delete user operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalAuthenticationWorker::deleteUserSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? USER_DELETE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? USER_DELETE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
    }

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		trace (TRACE_LEVEL_INFO, "Removing the user in FS of all nodes is successful.");
		startTransaction ();

		/* Update the role to exclude the user */
		updateWaveManagedObject (pRoleObj);
		pRoleObj->setUsers(pMessage->getUsersOfRole());

		/* Delete the user */
		delete pUserObj;

		/* Commit the changes in MO instances to the DB */
		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = USER_DELETE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting the User configuration to Wave database is successful.");
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "Commiting the User configuration failed. Framework error - " + FrameworkToolKit::localize (status));
			status = USER_DELETE_DB_ERROR;
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}
	pAAAGlobalDeleteUserMessage->setMessageString(errorMessage);


	delete pRoleObj;

	delete pMessage;

	delete pAAAContext;
	delete pWaveSendToClusterContext;

	if ((status != WAVE_MESSAGE_SUCCESS) || FrameworkToolKit::isConfigReplayInProgress())
	{
		pAAAGlobalDeleteUserMessage->setCompletionStatus(status);
		reply (pAAAGlobalDeleteUserMessage);
	}
	else
	{
		vector<string> pathVec;
		string path = string("/aaa:aaa/authentication/users/user{") + pAAAGlobalDeleteUserMessage->getName() + "}" ;
		pathVec.push_back(path);
		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

		AAALocalCleanCacheMessage *pSetCacheMessage = new AAALocalCleanCacheMessage ("Delete-User-Operation", NONE_HAS_DEFAULT_PASSWD, pathVec);
		AAACleanCacheContext *pCleanCacheContext = new AAACleanCacheContext (pAAAGlobalDeleteUserMessage);
		WaveSendToClusterContext *pWaveSendToClusterContextCache = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::cleanAAACacheSTCCallback), pCleanCacheContext);
		pWaveSendToClusterContextCache->setPPrismMessageForPhase1(pSetCacheMessage);
		if(!locations.empty()) {
			pWaveSendToClusterContextCache->setLocationsToSendToForPhase1(locations);
		}
		sendToWaveCluster (pWaveSendToClusterContextCache);
	}

	trace (TRACE_LEVEL_WARN, "AAAGlobalDeleteUserMessageHandler() - EXIT");
}

void  GlobalAuthenticationWorker::AAAGlobalUserActionMessageHandler( AAAGlobalUserActionMessage *pAAAGlobalUserActionMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalAuthenticationWorker::AAAGlobalUserActionMessageHandler()");
	if (pAAAGlobalUserActionMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string name = pAAAGlobalUserActionMessage->getName();
	vector<UI32> locations;

	/* Check if the user being unlocked is 'admin' */
	if (name.compare("admin") == 0)
	{
		pAAAGlobalUserActionMessage->setCompletionStatus (USER_NOT_LOCKED_ERROR);
		reply (pAAAGlobalUserActionMessage);
		return;
	}

	/* Check if the User exists */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());

	synchronousQueryContext->addAndAttribute(new AttributeString(name, "username"));
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	delete synchronousQueryContext;
	if ((NULL == pResults) || (pResults->size() == 0))
	{
		trace (TRACE_LEVEL_ERROR, "User does not exist");
		if (pResults) delete pResults;
		pAAAGlobalUserActionMessage->setCompletionStatus (USER_DOESNT_EXIST);
		reply (pAAAGlobalUserActionMessage);
		return;
	}
	else
	{
		for (UI32 j = 0; j < pResults->size(); j++)
			delete (*pResults)[j];

		delete pResults;
	}

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	/* Send message to all local services */
	AAALocalUserActionMessage *pMessage = new AAALocalUserActionMessage(name, pAAAGlobalUserActionMessage->getActionType());

	AAASequencerContext *pUserActionContext = new AAASequencerContext(pAAAGlobalUserActionMessage);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::userActionSTCCallback), pUserActionContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalAuthenticationWorker::userActionSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalAuthenticationWorker::userActionSTCCallback()");

	AAASequencerContext *pUserActionContext = (AAASequencerContext *)pWaveSendToClusterContext->getPCallerContext();
	AAAGlobalUserActionMessage *pAAAGlobalUserActionMessage = (AAAGlobalUserActionMessage *)pUserActionContext->getPPrismMessage();
	AAALocalUserActionMessage *pMessage = (AAALocalUserActionMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		trace (TRACE_LEVEL_INFO, "GlobalAuthenticationWorker : Unlocking the user in FS of all nodes is successful.");
	}
	else
	{
		trace (TRACE_LEVEL_ERROR, string("GlobalAuthenticationWorker : Unlocking the user in FS of node(s) failed with Framework error - ") + FrameworkToolKit::localize (status));
		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();

		status = pWaveSendToClusterContext->getCompletionStatusForPhase1(FrameworkToolKit::getThisLocationId());

		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			string errStr;
			bool notLocked = true;
			ResourceId cxNodeStatus;

			// Check if all nodes returned USER_NOT_LOCKED_ERROR/WAVE_MESSAGE_SUCCESS to know
			// if the user is not-locked/unlocked
			if (status != WAVE_MESSAGE_SUCCESS && status != USER_NOT_LOCKED_ERROR)
			{
				notLocked = false;
			}
			else if (status == WAVE_MESSAGE_SUCCESS)
			{ // return WAVE_MESSAGE_SUCCESS if the user is locked on some nodes and not-locked on the remaining
				for (vector<LocationId>::iterator element = cxLocations.begin(); element != cxLocations.end(); element++)
				{
					cxNodeStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(*element);
					if (cxNodeStatus != WAVE_MESSAGE_SUCCESS && cxNodeStatus != USER_NOT_LOCKED_ERROR)
					{
						notLocked = false;
						break;
					}
				}
			}
			else
			{ // return USER_NOT_LOCKED_ERROR if the user is not-locked on all nodes
				bool unlocked = false;
				for (vector<LocationId>::iterator element = cxLocations.begin(); element != cxLocations.end(); element++)
				{
					cxNodeStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(*element);
					if (cxNodeStatus == WAVE_MESSAGE_SUCCESS)
					{
						unlocked = true;
					}
					else if (cxNodeStatus != USER_NOT_LOCKED_ERROR)
					{
						notLocked = false;
						break;
					}
				}

				if (unlocked) status = WAVE_MESSAGE_SUCCESS;
			}

			if (notLocked == false)
			{
				// Unlocking failed in one or more nodes
				pUserActionContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalAuthenticationWorker::userActionSTCCallback", errStr);
				pAAAGlobalUserActionMessage->setMessageString(errStr);
				status = CLUSTER_ERROR_1001;
			}
		}
	}

	pAAAGlobalUserActionMessage->setCompletionStatus(status);
	reply (pAAAGlobalUserActionMessage);

	delete pMessage;
	delete pUserActionContext;
	delete pWaveSendToClusterContext;

	trace (TRACE_LEVEL_INFO, "AAAGlobalUserActionMessageHandler() - EXIT from STC Callback");
}

void  GlobalAuthenticationWorker::AAAGlobalAddRoleMessageHandler (AAAGlobalAddRoleMessage *pAAAGlobalAddRoleMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalAuthenticationWorker::AAAGlobalAddRoleMessageHandler()");
	if (pAAAGlobalAddRoleMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string name = pAAAGlobalAddRoleMessage->getName();
	string description = pAAAGlobalAddRoleMessage->getDescription();
	UI32 gid;
	vector<UI32> locations;
	string loginUser = pAAAGlobalAddRoleMessage->getLoginUser();
	string loginRole = pAAAGlobalAddRoleMessage->getLoginRole();
	string loginIp = pAAAGlobalAddRoleMessage->getLoginIp();
	string loginProto = pAAAGlobalAddRoleMessage->getLoginProto();
	string loginAppName = pAAAGlobalAddRoleMessage->getLoginAppName();

	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	/* Validate user parameters */
	if ((ret = paramValidate(name, ROLE_NAME)) != 0)
	{
		status = ROLE_INVALID_NAME;
	}
	else if (name.compare(ACCT_ROOT_ROLE) == 0)
	{
		trace (TRACE_LEVEL_ERROR, "'root' cannot be added");
		status = ROLE_NAME_RESERVED;
		ret = -1;
	}

	if (ret == 0)
	{
		if (description.length() > 0)
		{
			if ((ret = paramValidate(description, USER_DESCRIPTION)) != 0)
				status = ROLE_INVALID_DESC;
		}
	}

	if (ret != 0)
	{
		pAAAGlobalAddRoleMessage->setCompletionStatus(status);
		reply (pAAAGlobalAddRoleMessage);
		return;
	}
	else
	{
		trace (TRACE_LEVEL_DEBUG, "User input validation OK.");
	}

	if (pAAAGlobalAddRoleMessage->getIsALastConfigReplay() == true) {
		trace (TRACE_LEVEL_DEBUG, "pAAAGlobalAddRoleMessage getIsALastConfigReplay true");
        synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RoleManagedObject::getClassName());     
        
        synchronousQueryContext->addAndAttribute(new AttributeString(name, "rolename"));
        pResults = querySynchronously (synchronousQueryContext);
        if ((NULL == pResults) || (pResults->size() != 1)) {
            trace (TRACE_LEVEL_DEBUG, "Role does not exist");
        } else if (pResults->size() == 1) {
            trace (TRACE_LEVEL_DEBUG, "Role exist");
            pAAAGlobalAddRoleMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            reply (pAAAGlobalAddRoleMessage);
            delete synchronousQueryContext;
            if (pResults != NULL) {
                for (UI32 j = 0; j < pResults->size(); j++)
                    delete (*pResults)[j];
                delete pResults;
            }
            return;
        }

    }

	/* Check if the max limit for roles has reached */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RoleManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	if (pResults != NULL && pResults->size() == ROLE_MAX_NUM)
	{
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];
		}
		delete pResults;
		delete synchronousQueryContext;
		pAAAGlobalAddRoleMessage->setCompletionStatus(ROLE_MAX_LIMIT);
		reply (pAAAGlobalAddRoleMessage);
		return;
	}
	if (pResults != NULL) {
		for (UI32 j = 0; j < pResults->size(); j++)
			delete (*pResults)[j];
		delete pResults;
	}
	delete synchronousQueryContext;

	/* Check if an existing user has the same name */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());
	synchronousQueryContext->addOrAttribute(new AttributeString(name, "username"));
	pResults = querySynchronously (synchronousQueryContext);
	if ((pResults != NULL) && (pResults->size() > 0))
	{
		for (UI32 j = 0; j < pResults->size(); j++)
			delete (*pResults)[j];
		delete pResults;
		delete synchronousQueryContext;
		pAAAGlobalAddRoleMessage->setCompletionStatus(ROLE_NAME_USER_MATCH);
		reply (pAAAGlobalAddRoleMessage);
		return;
	}
	if (pResults != NULL) delete pResults;
	delete synchronousQueryContext;

	/* Find least available GID to assign to the new role */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RoleManagedObject::getClassName());
	synchronousQueryContext->addOrderField("gid", true);
	pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() == 0))
	{
		trace (TRACE_LEVEL_FATAL, "No Role exists on the switch");
		pAAAGlobalAddRoleMessage->setCompletionStatus (ROLE_ADD_ERROR);
		reply (pAAAGlobalAddRoleMessage);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		return;
	}

	RoleManagedObject *pRoleObj = NULL;
	bool found = false;

	gid = (UDROLE_BASE_GID + 1);
	for (UI32 i = 0; i < pResults->size(); i++)
	{
		pRoleObj = dynamic_cast<RoleManagedObject *> ((*pResults)[i]);
		/* Coverity */
		if (NULL == pRoleObj)
		{
			continue; // Continue to next role
		}
		if (!found)
		{
			if (gid > pRoleObj->getGID())
			{
				delete pRoleObj;
				continue; // Skip default roles
			}
			else if (gid != pRoleObj->getGID())
			{
				found = true; // least available GID is found
			}
			else
			{
				gid++; // Check if the next GID is available
			}
		}
		delete pRoleObj; // All results will be deleted even if a GID is in hand
	}
	delete pResults;
	delete synchronousQueryContext;

	trace (TRACE_LEVEL_WARN, string("Assigning new role with GID ") + gid);

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));
	// Send message to all local services

	AAALocalAddRoleMessage *pAddMessage = new AAALocalAddRoleMessage (name, description, gid, loginUser, loginRole, loginIp, loginProto, loginAppName);

	AAAAddRoleContext *pAddRoleContext = new AAAAddRoleContext (pAAAGlobalAddRoleMessage);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::addRoleSTCCallback), pAddRoleContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pAddMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalAuthenticationWorker::addRoleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalAuthenticationWorker::addRoleSTCCallback()");

    AAAAddRoleContext *pAAAContext = (AAAAddRoleContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalAddRoleMessage *pAddMessage = (AAALocalAddRoleMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;
	vector<UI32> locations;

	AAAGlobalAddRoleMessage *pAAAGlobalAddRoleMessage = (AAAGlobalAddRoleMessage *)pAAAContext->getPPrismMessage();

	if (FrameworkToolKit::isConfigReplayInProgress())
		trace (TRACE_LEVEL_INFO, "Role is added as part of Config Replay.");
	else
		trace (TRACE_LEVEL_INFO, "Role is added from command line.");

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Add role operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Add role operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalAuthenticationWorker::addRoleSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? ROLE_ADD_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? ROLE_ADD_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
    }

	if (status == WAVE_MESSAGE_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "Role add operation is successful in the FS.");

		startTransaction ();

		RoleManagedObject *pRoleMO = dynamic_cast<RoleManagedObject *>(createManagedObjectInstance(RoleManagedObject::getClassName()));

		pRoleMO->setName(pAddMessage->getName());
		pRoleMO->setDescription(pAddMessage->getDescription());
		pRoleMO->setGID(pAddMessage->getGid());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = ROLE_ADD_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting the Role configuration to Wave database is successful.");
		}
		else
		{
			status = ROLE_ADD_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting the Role configuration failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
		delete pRoleMO;
	}
	pAAAGlobalAddRoleMessage->setMessageString(errorMessage);

	delete pAddMessage;

	delete pAAAContext;
	delete pWaveSendToClusterContext;

	if ((status != WAVE_MESSAGE_SUCCESS) || FrameworkToolKit::isConfigReplayInProgress())
	{
		pAAAGlobalAddRoleMessage->setCompletionStatus(status);
		reply(pAAAGlobalAddRoleMessage);
	}
	else
	{
		vector<string> pathVec;
		string path = string("/aaa:aaa/authentication/groups/group{") + pAAAGlobalAddRoleMessage->getName() + "}" ;
		pathVec.push_back(path);
		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

		AAALocalCleanCacheMessage *pSetCacheMessage = new AAALocalCleanCacheMessage ("Add-Role-Operation", NONE_HAS_DEFAULT_PASSWD, pathVec);
		AAACleanCacheContext *pCleanCacheContext = new AAACleanCacheContext (pAAAGlobalAddRoleMessage);
		WaveSendToClusterContext *pWaveSendToClusterContextCache = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::cleanAAACacheSTCCallback), pCleanCacheContext);
		pWaveSendToClusterContextCache->setPPrismMessageForPhase1(pSetCacheMessage);
		if(!locations.empty()) {
			pWaveSendToClusterContextCache->setLocationsToSendToForPhase1(locations);
		}
		sendToWaveCluster (pWaveSendToClusterContextCache);
	}

	trace (TRACE_LEVEL_WARN, "AAAGlobalAddRoleMessageHandler() - EXIT");
}

void  GlobalAuthenticationWorker::AAAGlobalChangeRoleMessageHandler( AAAGlobalChangeRoleMessage *pAAAGlobalChangeRoleMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalAuthenticationWorker::AAAGlobalChangeRoleMessageHandler()");
	if (pAAAGlobalChangeRoleMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	vector<UI32> locations;
	string name = pAAAGlobalChangeRoleMessage->getName();
	string description = pAAAGlobalChangeRoleMessage->getDescription();
	string loginUser = pAAAGlobalChangeRoleMessage->getLoginUser();
	string loginRole = pAAAGlobalChangeRoleMessage->getLoginRole();
	string loginIp = pAAAGlobalChangeRoleMessage->getLoginIp();
	string loginProto = pAAAGlobalChangeRoleMessage->getLoginProto();
	string loginAppName = pAAAGlobalChangeRoleMessage->getLoginAppName();

	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	/* Check if the role being changed is a default role */
	if (name.compare("user") == 0 || name.compare("admin") == 0)
	{
		pAAAGlobalChangeRoleMessage->setCompletionStatus (ROLE_CHANGE_DEFAULT);
		reply (pAAAGlobalChangeRoleMessage);
		return;
	}

	/* Get the Role Managed Object for optional parameters */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RoleManagedObject::getClassName());

	synchronousQueryContext->addAndAttribute(new AttributeString(name, "rolename"));
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_FATAL, "Role does not exist");
		pAAAGlobalChangeRoleMessage->setCompletionStatus (ROLE_DOESNT_EXIST);
		reply (pAAAGlobalChangeRoleMessage);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		return;
	}
	RoleManagedObject *pRoleObj = dynamic_cast<RoleManagedObject *> ((*pResults)[0]);

	delete pResults;
	delete synchronousQueryContext;
	/* Coverity */
	if (NULL == pRoleObj)
	{
		trace (TRACE_LEVEL_FATAL, "Role does not exist");
		pAAAGlobalChangeRoleMessage->setCompletionStatus (ROLE_DOESNT_EXIST);
		reply (pAAAGlobalChangeRoleMessage);
		return;
	}

	/* Validate the input parameters */
	if (description.length() > 0)
	{
		if ((ret = paramValidate(description, USER_DESCRIPTION)) != 0)
			status = ROLE_INVALID_DESC;
	}
	else if (pAAAGlobalChangeRoleMessage->getDescFlag() == false)
	{
		description = pRoleObj->getDescription();
	}

	if (pAAAGlobalChangeRoleMessage->getIsALastConfigReplay() == true) {
		trace (TRACE_LEVEL_DEBUG, "pAAAGlobalChangeRoleMessage getIsALastConfigReplay true");
		if (description.compare(pRoleObj->getDescription()) == 0) {
			trace (TRACE_LEVEL_DEBUG, "pAAAGlobalChangeRoleMessage:Role already chagned");
			if (pRoleObj != NULL) delete pRoleObj;
			pAAAGlobalChangeRoleMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			reply (pAAAGlobalChangeRoleMessage);
			return;
		}	
	}
	if (ret != 0)
	{
		delete pRoleObj;
		pAAAGlobalChangeRoleMessage->setCompletionStatus(status);
		reply (pAAAGlobalChangeRoleMessage);
		return;
	}
	else
	{
		trace (TRACE_LEVEL_DEBUG, "User input validation OK.");
	}
	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	// Send message to all local services
	AAALocalChangeRoleMessage *pSetMessage = new AAALocalChangeRoleMessage (name, description, pAAAGlobalChangeRoleMessage->getDescFlag(), pRoleObj->getUsers(), loginUser, loginRole, loginIp, loginProto, loginAppName);

	AAAChangeRoleContext *pChangeRoleContext = new AAAChangeRoleContext (pAAAGlobalChangeRoleMessage, pRoleObj);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::changeRoleSTCCallback), pChangeRoleContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pSetMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalAuthenticationWorker::changeRoleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalAuthenticationWorker::changeRoleSTCCallback()");

    AAAChangeRoleContext *pAAAContext = (AAAChangeRoleContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalChangeRoleMessage *pSetMessage = (AAALocalChangeRoleMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;
	vector<UI32> locations;

	AAAGlobalChangeRoleMessage *pAAAGlobalChangeRoleMessage = (AAAGlobalChangeRoleMessage *)pAAAContext->getPPrismMessage();
	RoleManagedObject *pRoleObj = (RoleManagedObject *)pAAAContext->getRoleObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Change role operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Change role operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalAuthenticationWorker::changeRoleSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? ROLE_CHANGE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? ROLE_CHANGE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
    }

	if (status == WAVE_MESSAGE_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "Role change operation is successful in the FS.");

		startTransaction ();

		updateWaveManagedObject(pRoleObj);

		pRoleObj->setDescription(pSetMessage->getDescription());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = ROLE_CHANGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting the Role configuration to Wave database is successful.");
		}
		else
		{
			status = ROLE_CHANGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting the Role configuration failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}
	pAAAGlobalChangeRoleMessage->setMessageString(errorMessage);

	delete pRoleObj;
	delete pSetMessage;
	delete pAAAContext;
	delete pWaveSendToClusterContext;

	if ((status != WAVE_MESSAGE_SUCCESS) || FrameworkToolKit::isConfigReplayInProgress())
	{
		pAAAGlobalChangeRoleMessage->setCompletionStatus(status);
		reply(pAAAGlobalChangeRoleMessage);
	}
	else
	{
		vector<string> pathVec;
		string path = string("/aaa:aaa/authentication/groups/group{") + pAAAGlobalChangeRoleMessage->getName() + "}" ;
		pathVec.push_back(path);
		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

		AAALocalCleanCacheMessage *pSetCacheMessage = new AAALocalCleanCacheMessage ("Change-Role-Operation", NONE_HAS_DEFAULT_PASSWD, pathVec);
		AAACleanCacheContext *pCleanCacheContext = new AAACleanCacheContext (pAAAGlobalChangeRoleMessage);
		WaveSendToClusterContext *pWaveSendToClusterContextCache = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::cleanAAACacheSTCCallback), pCleanCacheContext);
		pWaveSendToClusterContextCache->setPPrismMessageForPhase1(pSetCacheMessage);
		if(!locations.empty()) {
			pWaveSendToClusterContextCache->setLocationsToSendToForPhase1(locations);
		}
		sendToWaveCluster (pWaveSendToClusterContextCache);
	}

	trace (TRACE_LEVEL_INFO, "AAAGlobalChangeRoleMessageHandler() - EXIT");
}

void  GlobalAuthenticationWorker::AAAGlobalDeleteRoleMessageHandler( AAAGlobalDeleteRoleMessage *pAAAGlobalDeleteRoleMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalAuthenticationWorker::AAAGlobalDeleteRoleMessageHandler()");
	if (pAAAGlobalDeleteRoleMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}
	vector<UI32> locations;

	string name = pAAAGlobalDeleteRoleMessage->getName();
	string loginUser = pAAAGlobalDeleteRoleMessage->getLoginUser();
	string loginRole = pAAAGlobalDeleteRoleMessage->getLoginRole();
	string loginIp = pAAAGlobalDeleteRoleMessage->getLoginIp();
	string loginProto = pAAAGlobalDeleteRoleMessage->getLoginProto();
	string loginAppName = pAAAGlobalDeleteRoleMessage->getLoginAppName();

	/* Check if the role being deleted is a default role */
	if (name.compare("user") == 0 || name.compare("admin") == 0)
	{
		pAAAGlobalDeleteRoleMessage->setCompletionStatus (ROLE_DELETE_DEFAULT);
		reply (pAAAGlobalDeleteRoleMessage);
		return;
	}

	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(MapRoleManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	string role("");
	if (pResults != NULL)
	{
		for (UI32 i = 0; i < pResults->size(); i++)
		{
			MapRoleManagedObject *mapRoleObj = dynamic_cast<MapRoleManagedObject *> ((*pResults)[i]);
			role = mapRoleObj->getRole();
			if (strcmp(role.c_str(), name.c_str()) == 0)
			{
				trace(TRACE_LEVEL_INFO, string("Role ") + name.c_str() + string("is mapped to LDAP Role"));
				pAAAGlobalDeleteRoleMessage->setCompletionStatus (ROLE_ASSOCIATED_TO_LDAP);
				reply (pAAAGlobalDeleteRoleMessage);
				delete synchronousQueryContext;
				if (pResults != NULL)
				{
					for (UI32 j = 0; j < pResults->size(); j++)
						delete (*pResults)[j];

					delete pResults;
				}
				return;
			}
		}
	}
	delete synchronousQueryContext;
	if (pResults != NULL) {
		for (UI32 j = 0; j < pResults->size(); j++)
			delete (*pResults)[j];

		delete pResults;
	}

	/* Get the Role Managed Object for rollback purpose */
	// WaveManagedObjectSynchronousQueryContext 
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RoleManagedObject::getClassName());
	synchronousQueryContext->addAndAttribute(new AttributeString(name, "rolename"));
	// vector<WaveManagedObject *>
	pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_FATAL, "Role does not exist");
		if (pAAAGlobalDeleteRoleMessage->getIsALastConfigReplay() == true) {
			trace (TRACE_LEVEL_DEBUG, "pAAAGlobalDeleteRoleMessage getIsALastConfigReplay true");
			pAAAGlobalDeleteRoleMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		} else {
			pAAAGlobalDeleteRoleMessage->setCompletionStatus (ROLE_DOESNT_EXIST);
		}
		reply (pAAAGlobalDeleteRoleMessage);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		return;
	}
	RoleManagedObject *pRoleObj = dynamic_cast<RoleManagedObject *> ((*pResults)[0]);

	delete pResults;
	delete synchronousQueryContext;

	/* Coverity */
	if (NULL == pRoleObj)
	{
		trace (TRACE_LEVEL_FATAL, "Role does not exist");
		pAAAGlobalDeleteRoleMessage->setCompletionStatus (ROLE_DOESNT_EXIST);
		reply (pAAAGlobalDeleteRoleMessage);
		return;
	}
	/* Return error if any of the users have this role as their primary role */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());
	synchronousQueryContext->addAndAttribute(new AttributeUI32(pRoleObj->getGID(), "gid"));
	pResults = querySynchronously (synchronousQueryContext);
	if ((pResults != NULL) && (pResults->size() > 0))
	{
		/* Coverity */
		UserManagedObject *pUsrObj = dynamic_cast<UserManagedObject *> ((*pResults)[0]);
		if (NULL != pUsrObj)
		{
			trace (TRACE_LEVEL_INFO, "Role cannot be deleted. It is primary role for " + pUsrObj->getName());
		}
		else
		{
			trace (TRACE_LEVEL_INFO, "Role cannot be deleted. It is primary role for Unknown user managed object");
		}

		pAAAGlobalDeleteRoleMessage->setCompletionStatus (ROLE_PRIMARY_USER_DELETE);
		reply (pAAAGlobalDeleteRoleMessage);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		delete pRoleObj;
		return;
	}
	if (pResults) delete pResults;
	delete synchronousQueryContext;

	/* Check if the role is assigned to any existing rule */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RuleManagedObject::getClassName());
	synchronousQueryContext->addAndAttribute(new AttributeObjectId(pRoleObj->getObjectId(), "role"));
	pResults = querySynchronously (synchronousQueryContext);

	if ((pResults != NULL) && (pResults->size() > 0))
	{
		RuleManagedObject *pRuleObj = dynamic_cast<RuleManagedObject *> ((*pResults)[0]);
		if (NULL != pRuleObj)
		{
			trace (TRACE_LEVEL_INFO, string("Role cannot be deleted. It's associated to the rule ") + pRuleObj->getIndex());
		}
		else
		{
			trace (TRACE_LEVEL_INFO, string("Role cannot be deleted. It's associated to the rule: Unknown Rule index"));
		}
		pAAAGlobalDeleteRoleMessage->setCompletionStatus (ROLE_ASSOCIATED_TO_RULE);
		reply (pAAAGlobalDeleteRoleMessage);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		delete pRoleObj;
		return;
	}

	if (pResults) delete pResults;
	delete synchronousQueryContext;

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	// Send message to all local services
	AAALocalDeleteRoleMessage *pMessage = new AAALocalDeleteRoleMessage (name, loginUser, loginRole, loginIp, loginProto, loginAppName);

	AAADeleteRoleContext *pDeleteRoleContext = new AAADeleteRoleContext (pAAAGlobalDeleteRoleMessage, pRoleObj);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::deleteRoleSTCCallback), pDeleteRoleContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalAuthenticationWorker::deleteRoleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalAuthenticationWorker::deleteRoleSTCCallback()");

    AAADeleteRoleContext *pAAAContext = (AAADeleteRoleContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalDeleteRoleMessage *pMessage = (AAALocalDeleteRoleMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;
	vector<UI32> locations;

	AAAGlobalDeleteRoleMessage *pAAAGlobalDeleteRoleMessage = (AAAGlobalDeleteRoleMessage *)pAAAContext->getPPrismMessage();
	RoleManagedObject *pRoleObj = (RoleManagedObject *)pAAAContext->getRoleObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Delete role operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Delete role operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalAuthenticationWorker::deleteRoleSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? ROLE_DELETE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? ROLE_DELETE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
    }

	if (status == WAVE_MESSAGE_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "Role delete operation is successful in the FS.");

		startTransaction ();

		delete pRoleObj;

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = ROLE_DELETE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting the Role configuration to Wave database is successful.");
		}
		else
		{
			status = ROLE_DELETE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting the Role configuration failed. Framework error - " + FrameworkToolKit::localize (status));
		}
	}
	pAAAGlobalDeleteRoleMessage->setMessageString(errorMessage);

	delete pMessage;
	delete pAAAContext;
	delete pWaveSendToClusterContext;

	if ((status != WAVE_MESSAGE_SUCCESS) || FrameworkToolKit::isConfigReplayInProgress())
	{
		pAAAGlobalDeleteRoleMessage->setCompletionStatus(status);
		reply(pAAAGlobalDeleteRoleMessage);
	}
	else
	{
		vector<string> pathVec;
		string path = string("/aaa:aaa/authentication/groups/group{") + pAAAGlobalDeleteRoleMessage->getName() + "}" ;
		pathVec.push_back(path);
		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

		AAALocalCleanCacheMessage *pSetCacheMessage = new AAALocalCleanCacheMessage ("Delete-Role-Operation", NONE_HAS_DEFAULT_PASSWD, pathVec);
		AAACleanCacheContext *pCleanCacheContext = new AAACleanCacheContext (pAAAGlobalDeleteRoleMessage);
		WaveSendToClusterContext *pWaveSendToClusterContextCache = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalAuthenticationWorker::cleanAAACacheSTCCallback), pCleanCacheContext);
		pWaveSendToClusterContextCache->setPPrismMessageForPhase1(pSetCacheMessage);
		if(!locations.empty()) {
			pWaveSendToClusterContextCache->setLocationsToSendToForPhase1(locations);
		}
		sendToWaveCluster (pWaveSendToClusterContextCache);
	}

	trace (TRACE_LEVEL_INFO, "AAAGlobalDeleteRoleMessageHandler() - EXIT");
}

int GlobalAuthenticationWorker::paramValidate(string str, AuthParamType type)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalAuthenticationWorker::paramValidate()");
	char debugInfo[10];
	int ret = 0;

	/* Coverity */
	snprintf(debugInfo, 10, "%d", type);
	trace (TRACE_LEVEL_INFO, "Validating the parameter '" + str + "' of type " + string(debugInfo));

	switch (type)
	{
		case USER_NAME:
			if (str.length() < ACCT_MIN_NAME_LEN || str.length() > ACCT_MAX_NAME_LEN) {
				/* Length of the user name should be between 1 and 40 */
				trace (TRACE_LEVEL_ERROR, "Name length < 1 or > 40");
				ret = -1;
			} else if (!isalpha(str[0])) {
				trace (TRACE_LEVEL_ERROR, "Name first letter is not alphabet");
				/* First letter should be alphabet */
				ret = -1;
			} else {
				/* Remaining letters should contain only alphabet & underscore & dot */
				char c;
				for (UI32 i = 1; i < str.length(); i++)
				{
					c = str[i];
					if (!(isalnum(c) || c == '_' || c == '.')) {
						trace (TRACE_LEVEL_ERROR, string("Name contains letter ") + c + string(" which is not alpha-numeric or '_' or '.'"));
						ret = -1;
						break;
					}
				}
			}
			break;

		case USER_DESCRIPTION:
			/* Description cannot exceed 64 chars */
			if (str.length() > ACCT_MAX_DESC_LEN) {
				trace (TRACE_LEVEL_ERROR, "Description length more than > 40");
				ret = -1;
			} else {
				/* Can contain any printable characters except ':'. It's the delimiter in /etc/password file */
				char c;
				for (UI32 i = 0; i < str.length(); i++)
				{
					c = str[i];
					if (!isprint(c) || c == '\'' || c == '\"' || c == '!' || c == ';' || c == ':') {
						trace (TRACE_LEVEL_ERROR, "Description characters can be any from the printable ASCII character set, except single quote, double quote, exclamation, colon and semi-colon");
						ret = -1;
						break;
					}
				}
			}
			break;

		case ROLE_NAME:
			if ((str.length() < ROLE_MIN_NAME_LEN) || (str.length() > ROLE_MAX_NAME_LEN)) {
				trace (TRACE_LEVEL_ERROR, "Name length < 4 or > 32");
				ret = -1;
			} else if (!isalpha(str[0])) {
				trace (TRACE_LEVEL_ERROR, "First letter is not an alphabet");
				ret = -1;
			} else {
				for (UI32 i = 1; i < str.length(); i++)
				{
					if (!(isalnum(str[i]) || str[i] == '_')) {
						trace (TRACE_LEVEL_ERROR, "Name should contain only alphabet, numbers and underscore");
						ret = -1;
						break;
					}
				}
			}
			break;

		default:
			trace (TRACE_LEVEL_FATAL, "Invalid parameter for validation");
			prismAssert (false, __FILE__, __LINE__);
			break;
	}

	return (ret);
}

string GlobalAuthenticationWorker::setErrorMessage(ResourceId &status,string operation)
{
	int ret = 0;
	string outputString("");
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(PasswordAttributesManagedObject::getClassName());
	vector<WaveManagedObject *> *pResultsPassword = querySynchronously (synchronousQueryContext);
	if ((NULL == pResultsPassword) || (pResultsPassword->size() != 1)) 		{
		trace (TRACE_LEVEL_ERROR, "Failed to obtain the PasswordAttribute's Managed Object");

		if (operation.compare("add") == 0) {
			status = USER_ADD_ERROR;
		}

		if (operation.compare("change") == 0) {
			status = USER_CHANGE_ERROR;
		}
		ret = -1;
	}
	if (ret == 0) {
		PasswordAttributesManagedObject *pPasswordObj =dynamic_cast<PasswordAttributesManagedObject *> ((*pResultsPassword)[0]);
		/* Coverity */
		if (NULL == pPasswordObj) {
			 trace (TRACE_LEVEL_ERROR, "Failed to derive the PasswordAttribute's Managed Object");
			if (operation.compare("add") == 0) {
				status = USER_ADD_ERROR;
			}
			if (operation.compare("change") == 0) {
				status = USER_CHANGE_ERROR;
			}
		}
		else {
		vector<int> passwordVal;
		vector<string>passwordStr;
		if (pPasswordObj->getLowercase() != 0) {
			passwordVal.push_back(pPasswordObj->getLowercase());
			passwordStr.push_back("lower-case characters,");
		}
		if (pPasswordObj->getUppercase() != 0) {
			passwordVal.push_back(pPasswordObj->getUppercase());
			passwordStr.push_back("upper-case characters,");
		}
		if (pPasswordObj->getNumerics() != 0) {
			passwordVal.push_back(pPasswordObj->getNumerics());
			passwordStr.push_back("numeric characters,");
		}
		if (pPasswordObj->getSplchars() != 0) {
			passwordVal.push_back(pPasswordObj->getSplchars());
			passwordStr.push_back("special characters,");
		}
		passwordVal.push_back(pPasswordObj->getMinlength());
		passwordStr.push_back("characters of minimum length");
		outputString = this->formatString(passwordVal,passwordStr);
		status = USER_PASSWORD_ERROR;
		}
		delete synchronousQueryContext;
		if (pResultsPassword) delete pResultsPassword;
		if (pPasswordObj) delete pPasswordObj;
	}
	return(outputString);
}

string GlobalAuthenticationWorker::formatString(vector<int> value, vector<string> strValue)
{
	string outputstr("");
	if (value.size() == strValue.size())
	{
		ostringstream oss;
		int len = value.size();
		for (int i = 0; i < len; ++i)
		{
			if (i != 0) oss << " ";
			oss << value[i];
			oss << " ";
			oss << strValue[i];
		}
		outputstr = oss.str();
	}
	return outputstr;
}

int GlobalAuthenticationWorker::setGEPS(const string &loginUser)
{

	int ret = 0;
	if (gset_name(loginUser.c_str()) != 0 ) {
		trace (TRACE_LEVEL_FATAL, "Setting the user's name GEPS variable failed");
		ret = -1;
	}
	if (gset_role("user") != 0) {
		trace (TRACE_LEVEL_FATAL, "Setting the user's role GEPS variable failed");
		ret = -1;
	}
	return (ret);
}

void GlobalAuthenticationWorker::cleanAAACacheSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalAuthenticationWorker::cleanAAACacheSTCCallback()");
	AAACleanCacheContext *pAAAContext = (AAACleanCacheContext *)pWaveSendToClusterContext->getPCallerContext();
	PrismMessage *pCleanCacheContextMessage = (PrismMessage *)pAAAContext->getPPrismMessage();
	AAALocalCleanCacheMessage *pCleanCacheMessage = (AAALocalCleanCacheMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	string operation = pCleanCacheMessage->getCaller();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();

	if (status == WAVE_MESSAGE_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "AAA Clean Cache operation is successful for " + pCleanCacheMessage->getCaller());
	}
	else
	{
		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			string errStr;
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalAuthenticationWorker::cleanAAACacheSTCCallback()", errStr);
			pCleanCacheContextMessage->setMessageString(errStr);
			// status = CLUSTER_ERROR_1001;
			status = WAVE_MESSAGE_ERROR;
		}
		else
		{
			status = AAA_CLEAN_CACHE_ERROR;
			trace (TRACE_LEVEL_ERROR, "AAA Clean Cache operation for " + operation + " failed - " + FrameworkToolKit::localize(status));
		}
	}

    delete pCleanCacheMessage;
	delete pAAAContext;
	delete pWaveSendToClusterContext;

	pCleanCacheContextMessage->setCompletionStatus(status);
	reply(pCleanCacheContextMessage);
	trace (TRACE_LEVEL_INFO, "GlobalAuthenticationWorker::cleanAAACacheSTCCallback() called from " + operation + " EXIT");
}

}
