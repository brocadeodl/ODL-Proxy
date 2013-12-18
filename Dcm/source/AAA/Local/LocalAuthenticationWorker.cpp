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

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "DcmResourceIdEnums.h"

#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Global/UserManagedObject.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/LocalAuthenticationWorker.h"
#include "AAA/Local/AAALocalTypes.h"
#include "AAA/Local/AAALocalAddUserMessage.h"
#include "AAA/Local/AAALocalChangeUserMessage.h"
#include "AAA/Local/AAALocalDeleteUserMessage.h"
#include "AAA/Local/AAALocalAddRoleMessage.h"
#include "AAA/Local/AAALocalChangeRoleMessage.h"
#include "AAA/Local/AAALocalDeleteRoleMessage.h"
#include "AAA/Local/AAALocalCleanCacheMessage.h"
#include "AAA/Local/AAALocalUserActionMessage.h"
#include "AAA/Utils/AAAErrorUtils.h"

extern "C" {
	#include <grp.h>
}

using namespace WaveNs;

namespace DcmNs
{
#ifndef STRDUP
#define STRDUP(s)      (s != NULL) ? strdup(s) : NULL
#endif
LocalAuthenticationWorker::LocalAuthenticationWorker (AAALocalLocalObjectManager *pAAALocalObjectManager)
	: WaveWorker  (pAAALocalObjectManager)
{
	addOperationMap (AAALOCALADDUSER, reinterpret_cast<PrismMessageHandler> (&LocalAuthenticationWorker::AAALocalAddUserMessageHandler));
	addOperationMap (AAALOCALCHANGEUSER, reinterpret_cast<PrismMessageHandler> (&LocalAuthenticationWorker::AAALocalChangeUserMessageHandler));
	addOperationMap (AAALOCALDELETEUSER, reinterpret_cast<PrismMessageHandler> (&LocalAuthenticationWorker::AAALocalDeleteUserMessageHandler));
	addOperationMap (AAALOCALADDROLE, reinterpret_cast<PrismMessageHandler> (&LocalAuthenticationWorker::AAALocalAddRoleMessageHandler));
	addOperationMap (AAALOCALCHANGEROLE, reinterpret_cast<PrismMessageHandler> (&LocalAuthenticationWorker::AAALocalChangeRoleMessageHandler));
	addOperationMap (AAALOCALDELETEROLE, reinterpret_cast<PrismMessageHandler> (&LocalAuthenticationWorker::AAALocalDeleteRoleMessageHandler));
	addOperationMap (AAALOCALCLEANCACHE, reinterpret_cast<PrismMessageHandler> (&LocalAuthenticationWorker::AAALocalCleanCacheMessageHandler));
	addOperationMap (AAALOCALUSERACTION, reinterpret_cast<PrismMessageHandler> (&LocalAuthenticationWorker::AAALocalUserActionMessageHandler));
	pAAALocalCleanCacheTimer = AAALocalCleanCacheTimer::getInstance();
}

LocalAuthenticationWorker::~LocalAuthenticationWorker ()
{
}

PrismMessage  *LocalAuthenticationWorker::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{
		case AAALOCALADDUSER :
			pPrismMessage = new AAALocalAddUserMessage ();
			break;
		case AAALOCALCHANGEUSER :
			pPrismMessage = new AAALocalChangeUserMessage ();
			break;
		case AAALOCALDELETEUSER :
			pPrismMessage = new AAALocalDeleteUserMessage ();
			break;
		case AAALOCALADDROLE :
			pPrismMessage = new AAALocalAddRoleMessage ();
			break;
		case AAALOCALCHANGEROLE :
			pPrismMessage = new AAALocalChangeRoleMessage ();
			break;
		case AAALOCALDELETEROLE :
			pPrismMessage = new AAALocalDeleteRoleMessage ();
			break;
		case AAALOCALCLEANCACHE :
			pPrismMessage = new AAALocalCleanCacheMessage ();
			break;
		case AAALOCALUSERACTION :
			pPrismMessage = new AAALocalUserActionMessage ();
			break;
		default :
			pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject  *LocalAuthenticationWorker::createManagedObjectInstance(const string &managedClassName)
{
	WaveManagedObject *pWaveManagedObject = NULL;

	return (pWaveManagedObject);
}

void  LocalAuthenticationWorker::AAALocalAddUserMessageHandler( AAALocalAddUserMessage *pAAALocalAddUserMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalAuthenticationWorker::AAALocalAddUserMessageHandler()");
	if (pAAALocalAddUserMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	secAcct_t user;
	memset(&user, 0, sizeof(secAcct_t));

	string description = pAAALocalAddUserMessage->getDescription();

	user.name = strdup((pAAALocalAddUserMessage->getName()).c_str());

	user.passwd = strdup((pAAALocalAddUserMessage->getPassword()).c_str());
	user.role = strdup((pAAALocalAddUserMessage->getRole()).c_str());

	if (description.length() > 0)
		user.desc = strdup(description.c_str());

	user.enabled = pAAALocalAddUserMessage->getEnabled();
	user.locked = -1; // User account unlocking is taken care by a different module of AAA

	string loginUser = pAAALocalAddUserMessage->getLoginUser();
	string loginRole = pAAALocalAddUserMessage->getLoginRole();
	string loginIp = pAAALocalAddUserMessage->getLoginIp();
	string loginProto = pAAALocalAddUserMessage->getLoginProto();
	string loginAppName = pAAALocalAddUserMessage->getLoginAppName();
	this->setGEPS(loginUser, loginRole, loginIp, loginProto, loginAppName);
	int ret = secAcctAdd(&user);
	if (ret != ACCT_OK)
	{
		if (pAAALocalAddUserMessage->getIsALastConfigReplay() == true &&
				ACCT_NAME_EXIST == ret) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalAddUserMessage getIsALastConfigReplay true");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_ERROR, string("Adding user account to backend FS failed. ret=") + ret);
			status = m_utils.getUserDcmError((secAcctErrCode)ret);
		}
	}
	else
	{
		trace (TRACE_LEVEL_INFO, "Adding user account to backend FS is successful.");

		ret = this->changeRoleInFileSystem(pAAALocalAddUserMessage->getRole(), "", false, pAAALocalAddUserMessage->getUsersOfRole());
		if (ret != DRBAC_OK)
		{
			trace (TRACE_LEVEL_ERROR, string("Changing the role of the user in the FS failed. ret=") + ret);
			status = m_utils.getRoleDcmError((drbac_err_t)ret);
		}
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalAddUserMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_INFO, "pAAALocalAddUserMessage getIsALastConfigReplay false");
            int retVal = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + retVal);
        }
#endif
	}

	FREE(user.name);
	FREE(user.passwd);
	FREE(user.role);
	FREE(user.desc);

	pAAALocalAddUserMessage->setCompletionStatus (status);
	reply (pAAALocalAddUserMessage);
}

void  LocalAuthenticationWorker::AAALocalChangeUserMessageHandler( AAALocalChangeUserMessage *pAAALocalChangeUserMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalAuthenticationWorker::AAALocalChangeUserMessageHandler()");
	if (pAAALocalChangeUserMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	string password = pAAALocalChangeUserMessage->getPassword();
	string role = pAAALocalChangeUserMessage->getRole();
	string description = pAAALocalChangeUserMessage->getDescription();

	secAcct_t user;
	memset(&user, 0, sizeof (secAcct_t));

	user.name = strdup(pAAALocalChangeUserMessage->getName().c_str());

	if (password.length() > 0)
		user.passwd = strdup(password.c_str());

	if (role.length() > 0)
		user.role = strdup(role.c_str()); // Principal role

	if (description.length() > 0)
	{
		user.desc = strdup(description.c_str());
	}
	else if (pAAALocalChangeUserMessage->getDescFlag())
	{
		user.desc = strdup(""); // Reset the description
	}

	user.enabled = pAAALocalChangeUserMessage->getEnabled();
	user.locked = -1; // User account unlocking is taken care by a different module of AAA

	string loginUser = pAAALocalChangeUserMessage->getLoginUser();
	string loginRole = pAAALocalChangeUserMessage->getLoginRole();
	string loginIp = pAAALocalChangeUserMessage->getLoginIp();
	string loginProto = pAAALocalChangeUserMessage->getLoginProto();
	string loginAppName = pAAALocalChangeUserMessage->getLoginAppName();
	this->setGEPS(loginUser, loginRole, loginIp, loginProto, loginAppName);
	int ret = secAcctChange(&user);
	if (ret != ACCT_OK)
	{
		if (pAAALocalChangeUserMessage->getIsALastConfigReplay() == true &&
				ACCT_ATTR_NO_CHANGE == ret) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalChangeUserMessage getIsALastConfigReplay no change");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_ERROR, string("Changing user account in the backend FS failed.") + ret);
			status = m_utils.getUserDcmError((secAcctErrCode)ret);
		}
	}
	else if (role.length() > 0)
	{
		trace (TRACE_LEVEL_INFO, "Changing user account in the backend FS is successful.");
		ret = this->changeRoleInFileSystem(pAAALocalChangeUserMessage->getRole(), "", false, pAAALocalChangeUserMessage->getUsersOfRole());
		if (ret != DRBAC_OK)
		{
			trace (TRACE_LEVEL_ERROR, string("Changing the new role of the user account in FS failed.") + ret);
			status = m_utils.getRoleDcmError((drbac_err_t)ret);
		}
		else
		{
			ret = this->changeRoleInFileSystem(pAAALocalChangeUserMessage->getOldRole(), "", false, pAAALocalChangeUserMessage->getUsersOfOldRole());
			if (ret != DRBAC_OK)
			{
				trace (TRACE_LEVEL_ERROR, string("Changing the old role of the user account in FS failed.") + ret);
				status = m_utils.getRoleDcmError((drbac_err_t)ret);
			}
		}
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalChangeUserMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalChangeUserMessage getIsALastConfigReplay false");
            int retVal = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + retVal);
        }   
#endif
	}

	FREE(user.name);
	FREE(user.passwd);
	FREE(user.role);
	FREE(user.desc);

	pAAALocalChangeUserMessage->setCompletionStatus (status);
	reply (pAAALocalChangeUserMessage);
}

void  LocalAuthenticationWorker::AAALocalDeleteUserMessageHandler( AAALocalDeleteUserMessage *pAAALocalDeleteUserMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalAuthenticationWorker::AAALocalDeleteUserMessageHandler()");
	if (pAAALocalDeleteUserMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	string loginUser = pAAALocalDeleteUserMessage->getLoginUser();
	string loginRole = pAAALocalDeleteUserMessage->getLoginRole();
	string loginIp = pAAALocalDeleteUserMessage->getLoginIp();
	string loginProto = pAAALocalDeleteUserMessage->getLoginProto();
	string loginAppName = pAAALocalDeleteUserMessage->getLoginAppName();
	this->setGEPS(loginUser, loginRole, loginIp, loginProto, loginAppName);

	int ret = secAcctDelete((pAAALocalDeleteUserMessage->getName()).c_str());

	if (ret != ACCT_OK)
	{
		if (pAAALocalDeleteUserMessage->getIsALastConfigReplay() == true &&
				ACCT_NAME_NOT_EXIST == ret) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalDeleteUserMessage getIsALastConfigReplay Not exist");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_ERROR, string("Deleting user account from backend FS failed.") + ret);
			status = m_utils.getUserDcmError((secAcctErrCode)ret);
		}
	}
	else
	{
		trace (TRACE_LEVEL_INFO, "Deleting user account from backend FS is successful.");
		ret = this->changeRoleInFileSystem(pAAALocalDeleteUserMessage->getRole(), "", false, pAAALocalDeleteUserMessage->getUsersOfRole());
		if (ret != DRBAC_OK)
		{
			trace (TRACE_LEVEL_ERROR, string("Changing the role of the user account in FS failed.") + ret);
			status = m_utils.getRoleDcmError((drbac_err_t)ret);
		}
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalDeleteUserMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalDeleteUserMessage getIsALastConfigReplay false");
            int retVal = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + retVal);
        }
#endif
	}

	pAAALocalDeleteUserMessage->setCompletionStatus (status);
	reply (pAAALocalDeleteUserMessage);
}

void  LocalAuthenticationWorker::AAALocalAddRoleMessageHandler( AAALocalAddRoleMessage *pAAALocalAddRoleMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalAuthenticationWorker::AAALocalAddRoleMessageHandler()");
	if (pAAALocalAddRoleMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	secRole_t role;
	memset(&role, 0, sizeof(secRole_t));

	role.name = strdup((pAAALocalAddRoleMessage->getName()).c_str());

	string description = pAAALocalAddRoleMessage->getDescription();
	if (description.length() > 0)
		role.desc = strdup(description.c_str());

	role.gid = pAAALocalAddRoleMessage->getGid();
	string loginUser = pAAALocalAddRoleMessage->getLoginUser();
	string loginRole = pAAALocalAddRoleMessage->getLoginRole();
	string loginIp = pAAALocalAddRoleMessage->getLoginIp();
	string loginProto = pAAALocalAddRoleMessage->getLoginProto();
	string loginAppName = pAAALocalAddRoleMessage->getLoginAppName();
	this->setGEPS(loginUser, loginRole, loginIp, loginProto, loginAppName);

	int ret = secRoleAdd(&role);
	if (ret != DRBAC_OK)
	{
		if ((pAAALocalAddRoleMessage->getIsALastConfigReplay() == true) &&
				(UDROLE_EXISTS_ERR == ret)) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalAddRoleMessage getIsALastConfigReplay UDROLE_EXISTS_ERR");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_ERROR, string("Adding role to backend FS failed.") + ret);
			status = m_utils.getRoleDcmError((drbac_err_t)ret);
		}
	}
	else
	{
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalAddRoleMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_INFO, "pAAALocalAddRoleMessage getIsALastConfigReplay false");
            int retVal = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + retVal);
        }
#endif
		trace (TRACE_LEVEL_INFO, "Adding role to backend FS is successful.");
	}

	FREE(role.name);
	FREE(role.desc);

	pAAALocalAddRoleMessage->setCompletionStatus (status);
	reply (pAAALocalAddRoleMessage);
}

void  LocalAuthenticationWorker::AAALocalChangeRoleMessageHandler( AAALocalChangeRoleMessage *pAAALocalChangeRoleMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalAuthenticationWorker::AAALocalChangeRoleMessageHandler()");
	if (pAAALocalChangeRoleMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string name = pAAALocalChangeRoleMessage->getName();
	string description = pAAALocalChangeRoleMessage->getDescription();
	bool descFlag = pAAALocalChangeRoleMessage->getDescFlag();
	string users = pAAALocalChangeRoleMessage->getUsers();
	string loginUser = pAAALocalChangeRoleMessage->getLoginUser();
	string loginRole = pAAALocalChangeRoleMessage->getLoginRole();
	string loginIp = pAAALocalChangeRoleMessage->getLoginIp();
	string loginProto = pAAALocalChangeRoleMessage->getLoginProto();
	string loginAppName = pAAALocalChangeRoleMessage->getLoginAppName();
	this->setGEPS(loginUser, loginRole, loginIp, loginProto, loginAppName);

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	int ret = this->changeRoleInFileSystem(name, description, descFlag, users);
	/* Change role alwasy return success so no check required for last config replay */
	if (ret != DRBAC_OK)
	{
		trace (TRACE_LEVEL_ERROR, string("Changing role(s) in backend FS failed.") + ret);
		status = m_utils.getRoleDcmError((drbac_err_t)ret);
	}
	else
	{
		trace (TRACE_LEVEL_INFO, "Changing role(s) in backend FS is successful.");
	}

	pAAALocalChangeRoleMessage->setCompletionStatus (status);
	reply (pAAALocalChangeRoleMessage);
}

void  LocalAuthenticationWorker::AAALocalDeleteRoleMessageHandler( AAALocalDeleteRoleMessage *pAAALocalDeleteRoleMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalAuthenticationWorker::AAALocalDeleteRoleMessageHandler()");
	if (pAAALocalDeleteRoleMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	string loginUser = pAAALocalDeleteRoleMessage->getLoginUser();
	string loginRole = pAAALocalDeleteRoleMessage->getLoginRole();
	string loginIp = pAAALocalDeleteRoleMessage->getLoginIp();
	string loginProto = pAAALocalDeleteRoleMessage->getLoginProto();
	string loginAppName = pAAALocalDeleteRoleMessage->getLoginAppName();
	this->setGEPS(loginUser, loginRole, loginIp, loginProto, loginAppName);

	int ret = secRoleDelete((pAAALocalDeleteRoleMessage->getName()).c_str());
	if (ret != DRBAC_OK)
	{
		if ((pAAALocalDeleteRoleMessage->getIsALastConfigReplay() == true) &&
				(UDROLE_DOESNT_EXIST_ERR == ret)) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalDeleteRoleMessage getIsALastConfigReplay UDROLE_DOESNT_EXIST_ERR");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_ERROR, string("Deleting role from backend FS failed.") + ret);
			status = m_utils.getRoleDcmError((drbac_err_t)ret);
		}
	}
	else
	{
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalDeleteRoleMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalDeleteRoleMessag getIsALastConfigReplay false");
            int retVal = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + retVal);
        }
#endif
		trace (TRACE_LEVEL_INFO, "Deleting role from backend FS is successful.");
	}

	pAAALocalDeleteRoleMessage->setCompletionStatus (status);
	reply (pAAALocalDeleteRoleMessage);
}

void LocalAuthenticationWorker::AAALocalUserActionMessageHandler (AAALocalUserActionMessage *pAAALocalUserActionMessage)
{
	trace (TRACE_LEVEL_INFO, "LocalAuthenticationWorker::AAALocalUserActionMessageHandler()");
	if (pAAALocalUserActionMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string name = pAAALocalUserActionMessage->getName();
	/* Check if the user being unlocked is 'admin' */
    if (name.compare("admin") == 0)
    {
        pAAALocalUserActionMessage->setCompletionStatus (USER_NOT_LOCKED_ERROR);
        reply (pAAALocalUserActionMessage);
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
        pAAALocalUserActionMessage->setCompletionStatus (USER_DOESNT_EXIST);
        reply (pAAALocalUserActionMessage);
        return;
    } else {
        for (UI32 j = 0; j < pResults->size(); j++)
            delete (*pResults)[j];
        delete pResults;
    }

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	secAcct_t user;
	memset(&user, 0, sizeof (secAcct_t));
	// Set all other attributes as 'no-change'
	user.enabled = -1;
	user.homeAD = -1;
	user.promptchg = -1;
	user.locked = -1;

	// Set user name and set unlock the attribute
	user.name = strdup(name.c_str());

	if (pAAALocalUserActionMessage->getActionType() == USER_UNLOCK)
	{
		trace (TRACE_LEVEL_INFO, "LocalAuthenticationWorker : User unlock request received for " + name);
		user.locked = 0;
	}

	int ret = secAcctChange(&user);
	if (ret == ACCT_ATTR_NO_CHANGE)
	{
		trace (TRACE_LEVEL_WARN, "LocalAuthenticationWorker : No change in user attributes.");
		switch (pAAALocalUserActionMessage->getActionType())
		{
		case USER_UNLOCK:
			/*
			 * During Last cfg replay, If backend is already updated before hafailover
			 * "ACCT_ATTR_NO_CHANGE" is returned
			 */
			if (pAAALocalUserActionMessage->getIsALastConfigReplay() == true) {
				trace (TRACE_LEVEL_DEBUG, "pAAALocalUserActionMessage getIsALastConfigReplay");
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				status = USER_NOT_LOCKED_ERROR;
			}
			break;
		default:
			break;
		}
	}
	else if (ret != ACCT_OK)
	{
		trace (TRACE_LEVEL_ERROR, string("LocalAuthenticationWorker : Change action on the user account failed.") + ret);
		switch (pAAALocalUserActionMessage->getActionType())
		{
		case USER_UNLOCK:
			status = USER_UNLOCK_ERROR;
			break;
		default:
			break;
		}
	}

#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalUserActionMessage->getIsALastConfigReplay() ==false) &&
					(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalUserActionMessage getIsALastConfigReplay false");
            int retVal = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + retVal);
        }
#endif
	FREE(user.name);
	pAAALocalUserActionMessage->setCompletionStatus (status);
	reply (pAAALocalUserActionMessage);
}

int LocalAuthenticationWorker::changeRoleInFileSystem(const string &name, const string &description, const bool &descFlag, const string &users)
{
	secRole_t *role = (secRole_t *)calloc(1, sizeof (secRole_t));

	role->name = strdup(name.c_str());

	if (description.length() > 0)
	{
		role->desc = strdup(description.c_str());
	}
	else if (descFlag == true)
	{
		role->desc = strdup("");
	}

	char *user_list = NULL;
	if (users.length() > 0)
	{
		/* Tokenize the users */
		int i = 0;
		char *re_entry = NULL, *token = NULL;
		// Allocate 2 extra entries to include the "root" and "NULL" entries.
		role->users = (char **)calloc(ACCT_MAX_NUM + 2, sizeof (char *));

		user_list = strdup(users.c_str());
		token = strtok_r(user_list, " ", &re_entry);
		while (token != NULL)
		{
			role->users[i++] = strdup(token);
			token = strtok_r(NULL, " ", &re_entry);
		}
		role->userNum = i;
		if (user_list) {
			free(user_list);
			user_list = NULL;
		}	
	}

	int ret = secRoleChange(role);

	FREE(role->name);
	/* Defect 466205 - role->desc need not be freed here as it is already freed in 
	   the backend function secRoleChange -> secPwdListClean */ 
	// FREE(role->desc); 
	FREE(role);
	/* other role attributes are cleaned by the security library */

	return (ret);
}

int LocalAuthenticationWorker::syncDefaultAccts(int passwdSync)
{
	WaveManagedObjectSynchronousQueryContext *syncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	int ret = 0;
	bool isAdminSync = false;
	bool isUserSync = false;

	if (passwdSync == ADMIN_HAS_DEFAULT_PASSWD) {
		isAdminSync = true;
	} else if (passwdSync == USER_HAS_DEFAULT_PASSWD) {
		isUserSync = true;
	} else if (passwdSync == BOTH_HAS_DEFAULT_PASSWD) {
		isAdminSync = true;
		isUserSync = true;
	}
	// Retrieve users from wave database
	syncQueryCtx = new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());
	syncQueryCtx->addOrAttribute (new AttributeString ("admin", "username"));
	syncQueryCtx->addOrAttribute (new AttributeString ("user", "username"));
	pResults = querySynchronously (syncQueryCtx);
	if (NULL != pResults) {
		for (UI32 i = 0; i < pResults->size(); i++)
		{
			UserManagedObject *pUser = (dynamic_cast<UserManagedObject *> ((*pResults)[i]));
			// It comes here 
			if (((pUser->getName().compare("admin") == 0) && isAdminSync) ||
				((pUser->getName().compare("user") == 0) && isUserSync))
			{
				secAcct_t user;
				memset(&user, 0, sizeof (secAcct_t));
				user.name = STRDUP((pUser->getName()).c_str());
				user.passwd = STRDUP((pUser->getMD5Password()).c_str());
				struct group *pGroup = getgrgid(pUser->getGid());
                if (NULL != pGroup)
                {
				    user.role = STRDUP(getgrgid(pUser->getGid())->gr_name);
                }
                else
                {
                    user.role = NULL;
                }
				if ((pUser->getDescription()).length() > 0)
					user.desc = STRDUP((pUser->getDescription()).c_str());
				user.enabled = pUser->getEnabled();
				user.locked = -1;
				ret = secAcctChange(&user);
				if (ret != 0) {
					trace (TRACE_LEVEL_INFO, string("Changing the user '") + user.name + string("' in the password db failed."));
				}	
				FREE(user.name);
				FREE(user.passwd);
                if (NULL != user.role)
                {
				    FREE(user.role);
                }
				FREE(user.desc);
			}
			delete pUser;
		}
		delete pResults;
	} else {
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("Getting the  user DB failed."));
		}	
	}
	delete syncQueryCtx;

	return (ret);
}

void  LocalAuthenticationWorker::AAALocalCleanCacheMessageHandler( AAALocalCleanCacheMessage *pAAALocalCleanCacheMessage)
{
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	int ret = -1;
	vector<string> path = pAAALocalCleanCacheMessage->getPath();
	trace (TRACE_LEVEL_INFO, "LocalAuthenticationWorker::AAALocalCleanCacheMessageHandler()");
	if (pAAALocalCleanCacheMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	if (pAAALocalCleanCacheMessage->getDefaultPasswdSync() != NONE_HAS_DEFAULT_PASSWD) {
		ret = syncDefaultAccts(pAAALocalCleanCacheMessage->getDefaultPasswdSync());
		if (ret != 0) {
			trace (TRACE_LEVEL_INFO, string("Sync default Accts failed ") + ret);
		}
	}

	pAAALocalCleanCacheMessage->setCompletionStatus (status);
	reply (pAAALocalCleanCacheMessage);
	trace (TRACE_LEVEL_DEBUG, "LocalAuthenticationWorker::AAALocalCleanCacheMessageHandler() EXIT");
}

void LocalAuthenticationWorker::setGEPS(const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName)
{
    trace (TRACE_LEVEL_DEBUG, "LocalAuthenticationWorker:setGEPS Entered");

    trace (TRACE_LEVEL_DEBUG, "LocalAuthenticationWorker:setGEPS Exit");
}

}
