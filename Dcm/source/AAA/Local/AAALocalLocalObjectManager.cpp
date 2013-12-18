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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : ckunduru                                                     *
 **************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"

#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Global/RadiusHostManagedObject.h"
#include "AAA/Global/TacacsHostManagedObject.h"
#include "AAA/Global/LDAPHostManagedObject.h"
#include "AAA/Global/MapRoleManagedObject.h"
#include "AAA/Global/ExecAccountingManagedObject.h"
#include "AAA/Global/CmdAccountingManagedObject.h"
#include "AAA/Global/AuthLoginModeManagedObject.h"
#include "AAA/Global/RoleManagedObject.h"
#include "AAA/Global/UserManagedObject.h"
#include "AAA/Global/PasswordAttributesManagedObject.h"
#include "AAA/Global/BannerManagedObject.h"
#include "AAA/Global/IpAclManagedObjects.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/LocalAuthenticationWorker.h"
#include "AAA/Local/LocalDiscreteObjsWorker.h"
#include "AAA/Local/LocalServerMgmtWorker.h"
#include "AAA/Local/LocalSessionWorker.h"
#include "AAA/Local/LocalIpAclWorker.h"
#include "AAA/Local/AAALocalTypes.h"
#include "AAA/Local/IpAclLocalManagedObjects.h"
#include "AAA/Local/AAALocalIpAclConfigMessage.h"
#include "AAA/Local/AAALocalIpAclDeleteMessage.h"
#include "AAA/Local/AAALocalIpAclReseqMessage.h"
#include "AAA/Global/IpAclManagedObjects.h"
#include "AAA/Local/SecServicesLocalManagedObject.h"

extern "C" {
	#include <grp.h>
	#include "security/public.h"
	#include "security/ipfilter_public.h"

	#undef CHASSIS
}

using namespace WaveNs;

namespace DcmNs
{
#ifndef STRDUP
#define STRDUP(s)      (s != NULL) ? strdup(s) : NULL
#endif

AAALocalLocalObjectManager::AAALocalLocalObjectManager ()
	: WaveLocalObjectManager  (getClassName ()),
	m_isPersistentRejoinBoot (false),
	m_isPersistentDefaultBoot (false)
{
	m_pLocalAuthenticationWorker = new LocalAuthenticationWorker (this);
	prismAssert (NULL != m_pLocalAuthenticationWorker, __FILE__, __LINE__);

	m_pLocalDiscreteObjsWorker = new LocalDiscreteObjsWorker (this);
	prismAssert (NULL != m_pLocalDiscreteObjsWorker, __FILE__, __LINE__);

	m_pLocalServerMgmtWorker = new LocalServerMgmtWorker (this);
	prismAssert (NULL != m_pLocalServerMgmtWorker, __FILE__, __LINE__);

	m_pLocalIpAclWorker = new LocalIpAclWorker (this);
	prismAssert (NULL != m_pLocalIpAclWorker, __FILE__, __LINE__);

	m_pLocalSessionWorker = new LocalSessionWorker (this);
	prismAssert (NULL != m_pLocalSessionWorker, __FILE__, __LINE__);
    PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (getServiceId ());
}

AAALocalLocalObjectManager::~AAALocalLocalObjectManager ()
{
	delete m_pLocalAuthenticationWorker;
	delete m_pLocalDiscreteObjsWorker;
	delete m_pLocalServerMgmtWorker;
	delete m_pLocalSessionWorker;
	delete m_pLocalIpAclWorker;
}

AAALocalLocalObjectManager  *AAALocalLocalObjectManager::getInstance()
{
	static AAALocalLocalObjectManager *pAAALocalLocalObjectManager = new AAALocalLocalObjectManager ();

	WaveNs::prismAssert (NULL != pAAALocalLocalObjectManager, __FILE__, __LINE__);

	return (pAAALocalLocalObjectManager);
}

string  AAALocalLocalObjectManager::getClassName()
{
	return ("AAA Local Service");
}

PrismServiceId  AAALocalLocalObjectManager::getPrismServiceId()
{
	return ((getInstance ())->getServiceId ());
}

PrismMessage  *AAALocalLocalObjectManager::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{
	default :
		pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject  *AAALocalLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
{
	return NULL;
}

int AAALocalLocalObjectManager::syncRolesAndUsers(bool isDefaultDBSync)
{
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	int ret = 0;

	/* Check integrity of Roles */
	vector<RoleManagedObject *> pRoleObjs;
	secRole_t *roles = NULL;
	UI32 roleNum = 0;
	vector<UI32> addRoleIndxs, delRoleIndxs;

	// Retrieve roles from database
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RoleManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	for (UI32 i = 0; i < pResults->size(); i++)
	{
		pRoleObjs.push_back(dynamic_cast<RoleManagedObject *> ((*pResults)[i]));
	}
	delete synchronousQueryContext;
	delete pResults;

	// Retrive roles from the Filesystem
	ret = secRoleGetAll(&roles, &roleNum);
	if (ret != 0) {
		trace (TRACE_LEVEL_ERROR, "Retrieving all roles from password database failed.");
	    for (UI32 i = 0; i < pRoleObjs.size(); i++) delete pRoleObjs[i];
		return (-1);
	}

	for (UI32 i = 0; i < roleNum; i++) {
		// Validate all the roles from the backend.
		if (roles[i].name == NULL) {
			trace (TRACE_LEVEL_ERROR, "Retrieved INVALID roles from password database.");
	    	for (UI32 j = 0; j < pRoleObjs.size(); j++) delete pRoleObjs[j];
			return (-1);
		}
		delRoleIndxs.push_back(i);
	}
	for (UI32 j = 0; j < pRoleObjs.size(); j++) {
		// Validate all the roles from the Dcmd.
		if (((pRoleObjs[j]->getName()).length() == 0)) {
			trace (TRACE_LEVEL_ERROR, "Retrieved INVALID roles from DCMD database.");
	    	for (UI32 i = 0; i < pRoleObjs.size(); i++) delete pRoleObjs[i];
			return (-1);
		}
		addRoleIndxs.push_back(j);
	}

	// Remove duplicates from the lists
	for (UI32 i = 0; i < roleNum; i++)
	{
		UI32 j = 0;
		for (; j < pRoleObjs.size(); j++)
		{
			if (pRoleObjs[j]->getGID() == roles[i].gid &&
				pRoleObjs[j]->getName() == roles[i].name)
			{
				if (roles[i].desc == NULL)
				{
					if ((pRoleObjs[j]->getDescription()).length() > 0)
					{
						continue; // mismatch
					}
				}
				else if (strcmp(roles[i].desc, (pRoleObjs[j]->getDescription()).c_str()) != 0)
				{
					continue; // mismatch
				}
				if (roles[i].users == NULL && (pRoleObjs[j]->getUsers()).length() == 0)
				{
					break;
				}
				if (roles[i].users != NULL)
				{
					string userNames;
					UI32 userNum = 0;
					while (userNum < roles[i].userNum)
					{
						if (userNum) userNames.append(ROLE_USERS_DELIMITER);
						userNames.append(roles[i].users[userNum]);
						userNum++;
					}
					if (userNames.compare(pRoleObjs[j]->getUsers()) == 0)
					{
						break;
					}
				}
			}
		}
		if (j != pRoleObjs.size())
		{
			vector<UI32>::iterator iter;
			for (iter = addRoleIndxs.begin(); iter < addRoleIndxs.end(); iter++)
			{
				if (*iter == j)
				{
					iter = addRoleIndxs.erase(iter);
					break;
				}
			}
			for (iter = delRoleIndxs.begin(); iter < delRoleIndxs.end(); iter++)
			{
				if (*iter == i)
				{
					iter = delRoleIndxs.erase(iter);
					break;
				}
			}
		}
	}

	/* Check integrity of Users */
	vector<UserManagedObject *> pUserObjs;
	secAcct_t *users = NULL;
	SI32 userNum = 0;
	vector<UI32> addUserIndxs, delUserIndxs;

	// Retrieve users from wave database
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	for (UI32 i = 0; i < pResults->size(); i++)
	{
		pUserObjs.push_back(dynamic_cast<UserManagedObject *> ((*pResults)[i]));
	}
	delete synchronousQueryContext;
	delete pResults;

	// Retrieve users from the password database
	ret = secAcctGet(NULL, &users, &userNum, false);
	if (ret != 0) {
		trace (TRACE_LEVEL_ERROR, "Retrieving users from the password database failed.");
	    for (UI32 j = 0; j < pRoleObjs.size(); j++) delete pRoleObjs[j];
	    for (UI32 j = 0; j < pUserObjs.size(); j++) delete pUserObjs[j];
		return (-1);
	}

	for (UI32 i = 0; i < pUserObjs.size(); i++) {
		/* 
		   Defect 440386 - Removed the third check condition that checked if the group ID of an
		   user present in the User MO is also present in the backend becaused it didn't succeed and 
		   returning error from this function
		 */
		if (((pUserObjs[i]->getName()).length() == 0) ||
			((pUserObjs[i]->getMD5Password()).length() == 0)) {
			trace (TRACE_LEVEL_ERROR, "Retrieved invalid users from the dcmd database.");
	        for (UI32 j = 0; j < pRoleObjs.size(); j++) delete pRoleObjs[j];
	        for (UI32 j = 0; j < pUserObjs.size(); j++) delete pUserObjs[j];
			return (-1);
		}
		addUserIndxs.push_back(i);
	}
	for (SI32 i = 0; i < userNum; i++) {
		if ((users[i].name == NULL) || (users[i].passwd == NULL) ||
			(users[i].role == NULL) || (getgrnam(users[i].role) == NULL)) {
			trace (TRACE_LEVEL_ERROR, "Retrieved invalid users from the password database.");
	        for (UI32 j = 0; j < pRoleObjs.size(); j++) delete pRoleObjs[j];
	        for (UI32 j = 0; j < pUserObjs.size(); j++) delete pUserObjs[j];
			return (-1);
		}
		delUserIndxs.push_back(i);
	}

	// Remove duplicates from the lists
	for (SI32 i = 0; i < userNum; i++)
	{
		UI32 j = 0;

		if (!strcmp(users[i].name, ACCT_ROOT_ROLE)) {
			vector<UI32>::iterator iter;
			for (iter = delUserIndxs.begin(); iter < delUserIndxs.end(); iter++)
			{
				if (*iter == (UI32)i)
					iter = delUserIndxs.erase(iter);
			}
			continue;
		}
		for (; j < pUserObjs.size(); j++)
		{
			if (!strcmp(users[i].name, (pUserObjs[j]->getName()).c_str()) &&
				!strcmp(users[i].passwd, (pUserObjs[j]->getMD5Password()).c_str()) &&
				!strcmp(users[i].desc, (pUserObjs[j]->getDescription()).c_str()) &&
				(getgrnam(users[i].role))->gr_gid == (gid_t)pUserObjs[j]->getGid() &&
				users[i].enabled == pUserObjs[j]->getEnabled()) {
				break;
			}
		}

		if (j != pUserObjs.size())
		{
			vector<UI32>::iterator iter;
			for (iter = addUserIndxs.begin(); iter < addUserIndxs.end(); iter++)
			{
				if (*iter == j) {
				 	iter = addUserIndxs.erase(iter);
				}
			}
			for (iter = delUserIndxs.begin(); iter < delUserIndxs.end(); iter++)
			{
				if (*iter == (UI32)i) {
				 	iter = delUserIndxs.erase(iter);
				}
			}
		}
	}

	/* Delete the non-existing users from the password DB */
	for (UI32 i = 0; i < delUserIndxs.size(); i++)
	{
		if (strcmp(users[delUserIndxs[i]].name, ACCT_ADMIN_ROLE) == 0 ||
			strcmp(users[delUserIndxs[i]].name, ACCT_USER_ROLE) == 0)
		{
			continue;
		}

		ret = secAcctDelete(users[delUserIndxs[i]].name);
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("Deleting the user '") + users[delUserIndxs[i]].name + string("' in the password database failed."));
	        for (UI32 j = 0; j < pRoleObjs.size(); j++) delete pRoleObjs[j];
	        for (UI32 j = 0; j < pUserObjs.size(); j++) delete pUserObjs[j];
			FREE(users);
			return (-1);
		}
	}

	/* Delete the non-exists roles from the password DB */
	for (UI32 i = 0; i < delRoleIndxs.size(); i++)
	{
		if (strcmp(roles[delRoleIndxs[i]].name, ACCT_ADMIN_ROLE) == 0 ||
			strcmp(roles[delRoleIndxs[i]].name, ACCT_USER_ROLE) == 0)
		{
			continue;
		}

		ret = secRoleDelete(roles[delRoleIndxs[i]].name);
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("Deleting the role '") + roles[delRoleIndxs[i]].name + string("' in the backend FS failed."));
	        for (UI32 j = 0; j < pRoleObjs.size(); j++) delete pRoleObjs[j];
	        for (UI32 j = 0; j < pUserObjs.size(); j++) delete pUserObjs[j];
			FREE(roles);
			return (-1);
		}
	}

	/* Add the new roles from wave DB to FS */
	for (UI32 i = 0; i < addRoleIndxs.size(); i++)
	{
		RoleManagedObject *pRole = pRoleObjs[addRoleIndxs[i]];
		secRole_t role;

		memset(&role, 0, sizeof(secRole_t));
		role.name = STRDUP((pRole->getName()).c_str());
		role.desc = STRDUP((pRole->getDescription()).c_str());
		role.gid = pRole->getGID();
		role.userNum = 0;

		char *user_list = NULL;
		string userNames = pRole->getUsers();
		if (userNames.length() > 0)
		{
			int j = 0;
			char *re_entry = NULL, *token = NULL;

			role.users = (char **)calloc(ACCT_MAX_NUM, sizeof (char *));

			user_list = STRDUP(userNames.c_str());
			token = strtok_r(user_list, ROLE_USERS_DELIMITER, &re_entry);
			while (token != NULL)
			{
				role.users[j++] = STRDUP(token);
				token = strtok_r(NULL, ROLE_USERS_DELIMITER, &re_entry);
			}
			role.users[j] = NULL;
			role.userNum = j;
			if (user_list)
				FREE(user_list);
		}

		if (strcmp(role.name, ACCT_ADMIN_ROLE) == 0 ||
			strcmp(role.name, ACCT_USER_ROLE) == 0)
		{
			ret = secRoleChange(&role);
		}
		else
		{
			ret = secRoleAdd(&role);
			FREE(role.desc);
		}

		if (userNames.length() > 0)
		{
			FREE(user_list);
			//role.users and it's elements are freed by secRoleAdd.
		}

		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("Adding the role '") + role.name + string("' in the backend FS failed."));
	        for (UI32 j = 0; j < pRoleObjs.size(); j++) delete pRoleObjs[j];
	        for (UI32 j = 0; j < pUserObjs.size(); j++) delete pUserObjs[j];
			FREE(roles);
			return (-1);
		}

		FREE(role.name);
	}
	FREE(roles);

	for (UI32 i = 0; i < pRoleObjs.size(); i++) delete pRoleObjs[i];

	/* Add the new users from wave DB to password DB */
	for (UI32 i = 0; i < addUserIndxs.size(); i++)
	{
		UserManagedObject *pUser = pUserObjs[addUserIndxs[i]];
		secAcct_t user;

		memset(&user, 0, sizeof(secAcct_t));
		if (isDefaultDBSync) {
		/* 393577: No need to sync the admin and user accounts for default and First time boot reason codes. */ 
			if ((pUser->getName()).compare("user") == 0 ||
				(pUser->getName()).compare("admin") == 0) {
				continue;
			}
		}
		user.name = STRDUP((pUser->getName()).c_str());
		user.passwd = STRDUP((pUser->getMD5Password()).c_str());
        struct group *pTempGroup = getgrgid(pUser->getGid());
        if (NULL != pTempGroup)
        {
		    user.role = STRDUP(pTempGroup->gr_name);
        }
		if ((pUser->getDescription()).length() > 0)
			user.desc = STRDUP((pUser->getDescription()).c_str());
		user.enabled = pUser->getEnabled();
		user.locked = -1;

		if (strcmp(user.name, ACCT_ADMIN_ROLE) == 0 ||
			strcmp(user.name, ACCT_USER_ROLE) == 0)
		{
			ret = secAcctChange(&user);
		}
		else
		{
			ret = secAcctAdd(&user);
		}
		FREE(user.passwd);
		FREE(user.role);
		FREE(user.desc);
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("Adding the user '") + user.name + string("' in the password db failed."));
	        for (UI32 j = 0; j < pUserObjs.size(); j++) delete pUserObjs[j];
			FREE(user.name);
			FREE(users);
			return (-1);
		}
		FREE(user.name);
	}
	FREE(users);

	for (UI32 i = 0; i < pUserObjs.size(); i++) delete pUserObjs[i];

	return (ret);
}

int AAALocalLocalObjectManager::syncRadiusHosts()
{
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	int ret = 0;
	/* Check integrity of Radius Servers */
	vector<RadiusHostManagedObject *> pRadiusObjs;
	secRadConf_t *pradconf = NULL;
	SI32 radNum = 0;
	vector<UI32> addRadiusIndxs, delRadiusIndxs;

	// Get all radius servers from the FS
	ret = secRadiusGet(&pradconf, &radNum);
	if (ret != 0)
	{
		if (ret == RADIUS_CONFIG_NOT_FOUND)
		{
			// OK if no radius configuration exists on the file system
			ret = 0;
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "Retreving all radius server configurations from backend FS failed.");
			return (-1);
		}
	}

	// Retrieve radius hosts from Wave DB
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RadiusHostManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	for (UI32 i = 0; i < pResults->size(); i++)
	{
		pRadiusObjs.push_back(dynamic_cast<RadiusHostManagedObject *> ((*pResults)[i]));
	}
	delete synchronousQueryContext;
	delete pResults;

	for (UI32 i = 0; i < pRadiusObjs.size(); i++) addRadiusIndxs.push_back(i);
	for (SI32 i = 0; i < radNum; i++) delRadiusIndxs.push_back(i);

	// Remove duplicates from the lists
	for (SI32 i = 0; i < radNum; i++)
	{
		UI32 j = 0;
		for (; j < pRadiusObjs.size(); j++)
		{
			if (strcmp(pradconf[i].hostname, (pRadiusObjs[j]->getHost()).c_str()) == 0 &&
				pradconf[i].timeout == (int)pRadiusObjs[j]->getTimeout() &&
				pradconf[i].retries == (int)pRadiusObjs[j]->getRetransmit() &&
				pradconf[i].port == pRadiusObjs[j]->getPort() &&
				pradconf[i].proto == pRadiusObjs[j]->getProtocol())
			{
				if (pradconf[i].secret == NULL)
				{
					if ((pRadiusObjs[j]->getKey()).length() == 0)
						break;
				}
				else if ((pRadiusObjs[j]->getKey()).length() == 0)
				{
				}
				else if (!strcmp(pradconf[i].secret, (pRadiusObjs[j]->getKey()).c_str()))
				{
					break;
				}
			}
		}
		if (j != pRadiusObjs.size())
		{
			vector<UI32>::iterator iter;
			for (iter = addRadiusIndxs.begin(); iter < addRadiusIndxs.end(); iter++)
			{
				if (*iter == j)
					iter = addRadiusIndxs.erase(iter);
			}
			for (iter = delRadiusIndxs.begin(); iter < delRadiusIndxs.end(); iter++)
			{
				if (*iter == (UI32)i)
					iter = delRadiusIndxs.erase(iter);
			}
		}
	}

	// Delete the radius configuration that does not exist
	for (UI32 i = 0; i < delRadiusIndxs.size(); i++)
	{
		ret = secRadiusRemove(&pradconf[delRadiusIndxs[i]]);
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("Deleting radius server '") + pradconf[delRadiusIndxs[i]].hostname + string("' from backend FS failed."));
			for (SI32 j = 0; j < radNum; j++) {
				FREE(pradconf[j].hostname);
				FREE(pradconf[j].secret);
			}
			FREE(pradconf);
			return (-1);
		}
	}

	// Add the radius hosts from wave DB to FS
    for (UI32 i = 0; i < addRadiusIndxs.size(); i++)
    {
		RadiusHostManagedObject *pRadiusObj = pRadiusObjs[addRadiusIndxs[i]];
		secRadConf_t radiusConf;

		memset(&radiusConf, 0, sizeof (secRadConf_t));

		radiusConf.hostname = STRDUP((pRadiusObj->getHost()).c_str());
		if ((pRadiusObj->getKey()).length())
			radiusConf.secret = STRDUP((pRadiusObj->getKey()).c_str());
		radiusConf.timeout = pRadiusObj->getTimeout();
		radiusConf.retries = pRadiusObj->getRetransmit();
		radiusConf.port = pRadiusObj->getPort();
		radiusConf.proto = pRadiusObj->getProtocol();

		ret = secRadiusAdd(&radiusConf);
		FREE(radiusConf.secret);
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("Adding radius server '") + radiusConf.hostname + string("' to backend FS failed."));
			FREE(radiusConf.hostname);
			for (SI32 j = 0; j < radNum; j++) {
				FREE(pradconf[j].hostname);
				FREE(pradconf[j].secret);
			}
			FREE(pradconf);
			return (-1);
		}
		FREE(radiusConf.hostname);
	}
	for (SI32 j = 0; j < radNum; j++) {
		FREE(pradconf[j].hostname);
		FREE(pradconf[j].secret);
	}
	FREE(pradconf);

	for (UI32 i = 0; i < pRadiusObjs.size(); i++) delete pRadiusObjs[i];

	return (ret);
}

int AAALocalLocalObjectManager::syncTacacsHosts()
{
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	int ret = 0;
	/* Check integrity of Tacacs+ Servers */
	vector<TacacsHostManagedObject *> pTacacsObjs;
	secTacacsConf_t *ptacconf = NULL;
	SI32 tacNum = 0;
	vector<UI32> addTacacsIndxs, delTacacsIndxs;

	// Get all tacacs+ servers from the FS
	ret = secTacacsGet(&ptacconf, &tacNum);
	if (ret != 0)
	{
		if (ret == TACACS_CONFIG_NOT_FOUND)
		{
			// OK if no tacacs+ configuration exists on the file system
			ret = 0;
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "Retreving all tacacs+ server configurations from backend FS failed.");
			return (-1);
		}
	}

	// Retrieve tacacs+ hosts from Wave DB
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(TacacsHostManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	for (UI32 i = 0; i < pResults->size(); i++)
	{
		pTacacsObjs.push_back(dynamic_cast<TacacsHostManagedObject *> ((*pResults)[i]));
	}
	delete synchronousQueryContext;
	delete pResults;

	for (UI32 i = 0; i < pTacacsObjs.size(); i++) addTacacsIndxs.push_back(i);
	for (SI32 i = 0; i < tacNum; i++) delTacacsIndxs.push_back(i);

	// Remove duplicates from the lists
	for (SI32 i = 0; i < tacNum; i++)
	{
		UI32 j = 0;
		for (; j < pTacacsObjs.size(); j++)
		{
			if (strcmp(ptacconf[i].hostname, (pTacacsObjs[j]->getHost()).c_str()) == 0 &&
				ptacconf[i].timeout == (int)pTacacsObjs[j]->getTimeout() &&
				ptacconf[i].retries == (int)pTacacsObjs[j]->getRetries() &&
				ptacconf[i].port == pTacacsObjs[j]->getPort() &&
				ptacconf[i].proto == pTacacsObjs[j]->getProtocol())
			{
				if (ptacconf[i].secret == NULL)
				{
					if ((pTacacsObjs[j]->getKey()).length() == 0)
						break;
				}
				else if ((pTacacsObjs[j]->getKey()).length() == 0)
				{
				}
				else if (!strcmp(ptacconf[i].secret, (pTacacsObjs[j]->getKey()).c_str()))
				{
					break;
				}
			}
		}
		if (j != pTacacsObjs.size())
		{
			vector<UI32>::iterator iter;
			for (iter = addTacacsIndxs.begin(); iter < addTacacsIndxs.end(); iter++)
			{
				if (*iter == j)
					iter = addTacacsIndxs.erase(iter);
			}
			for (iter = delTacacsIndxs.begin(); iter < delTacacsIndxs.end(); iter++)
			{
				if (*iter == (UI32)i)
					iter = delTacacsIndxs.erase(iter);
			}
		}
	}

	// Delete the tacacs+ configuration that does not exist
	for (UI32 i = 0; i < delTacacsIndxs.size(); i++)
	{
		ret = secTacacsRemove(&ptacconf[delTacacsIndxs[i]]);
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("Deleting tacacs+ server '") + ptacconf[delTacacsIndxs[i]].hostname + string("' from backend FS failed."));
			for (SI32 j = 0; j < tacNum; j++) {
				FREE(ptacconf[j].hostname);
				FREE(ptacconf[j].secret);
			}
			FREE(ptacconf);
			return (-1);
		}
	}

	// Add the tacacs+ hosts from wave DB to FS
    for (UI32 i = 0; i < addTacacsIndxs.size(); i++)
    {
		TacacsHostManagedObject *pTacacsObj = pTacacsObjs[addTacacsIndxs[i]];
		secTacacsConf_t tacacsConf;

		memset(&tacacsConf, 0, sizeof (secTacacsConf_t));

		tacacsConf.hostname = STRDUP((pTacacsObj->getHost()).c_str());
		if ((pTacacsObj->getKey()).length())
			tacacsConf.secret = STRDUP((pTacacsObj->getKey()).c_str());
		tacacsConf.timeout = pTacacsObj->getTimeout();
		tacacsConf.retries = pTacacsObj->getRetries();
		tacacsConf.port = pTacacsObj->getPort();
		tacacsConf.proto = pTacacsObj->getProtocol();

		ret = secTacacsAdd(&tacacsConf);
		FREE(tacacsConf.secret);
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("Adding tacacs+ server '") + tacacsConf.hostname + string("' to backend FS failed."));
			FREE(tacacsConf.hostname);
			for (SI32 j = 0; j < tacNum; j++) {
				FREE(ptacconf[j].hostname);
				FREE(ptacconf[j].secret);
			}
			FREE(ptacconf);
			return (-1);
		} else {
			trace (TRACE_LEVEL_ERROR, string("Adding tacacs+ server '") + tacacsConf.hostname + string("' to backend FS successful."));
		}
		FREE(tacacsConf.hostname);
	}

	for (SI32 j = 0; j < tacNum; j++) {
		FREE(ptacconf[j].hostname);
		FREE(ptacconf[j].secret);
	}
	FREE(ptacconf);
	for (UI32 i = 0; i < pTacacsObjs.size(); i++) delete pTacacsObjs[i];

	return (ret);
}

int AAALocalLocalObjectManager::syncLDAPHosts()
{
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	int ret = 0;
	/* Check integrity of LDAP Servers */
	vector<LDAPHostManagedObject *> pLDAPObjs;
	secLdapConf_t *pldapconf = NULL;
	SI32 ldapNum = 0;
	vector<UI32> addLDAPIndxs, delLDAPIndxs;

	// Get all LDAP servers from the FS
	ret = secLdapGet(&pldapconf, &ldapNum);
	if (ret != 0)
	{
		if (ret == LDAP_CONFIG_NOT_FOUND)
		{
			// OK if no LDAP configuration exists on the file system
			ret = 0;
			trace (TRACE_LEVEL_INFO, string("No configuration exists in the backend."));
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "Retreving all LDAP server configurations from backend FS failed.");
			return (-1);
		}
	}

	// Retrieve LDAP hosts from Wave DB
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(LDAPHostManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	for (UI32 i = 0; i < pResults->size(); i++)
	{
		pLDAPObjs.push_back(dynamic_cast<LDAPHostManagedObject *> ((*pResults)[i]));
	}
	delete synchronousQueryContext;
	delete pResults;

	for (UI32 i = 0; i < pLDAPObjs.size(); i++) addLDAPIndxs.push_back(i);
	for (SI32 i = 0; i < ldapNum; i++) delLDAPIndxs.push_back(i);

	// Remove duplicates from the lists
	for (SI32 i = 0; i < ldapNum; i++)
	{
		UI32 j = 0;
		for (; j < pLDAPObjs.size(); j++)
		{
			if (strcmp(pldapconf[i].hostname, (pLDAPObjs[j]->getHost()).c_str()) == 0 &&
				pldapconf[i].timeout == (int)pLDAPObjs[j]->getTimeout() &&
				pldapconf[i].port == pLDAPObjs[j]->getPort())
			{
				if (pldapconf[i].domain == NULL)
				{
					if ((pLDAPObjs[j]->getBasedn()).length() == 0)
						break;
				}
				else if ((pLDAPObjs[j]->getBasedn()).length() == 0)
				{
				}
				else if (!strcmp(pldapconf[i].domain, (pLDAPObjs[j]->getBasedn()).c_str()))
				{
					break;
				}
			}
		}
		if (j != pLDAPObjs.size())
		{
			vector<UI32>::iterator iter;
			for (iter = addLDAPIndxs.begin(); iter < addLDAPIndxs.end(); iter++)
			{
				if (*iter == j)
					iter = addLDAPIndxs.erase(iter);
			}
			for (iter = delLDAPIndxs.begin(); iter < delLDAPIndxs.end(); iter++)
			{
				if (*iter == (UI32)i)
					iter = delLDAPIndxs.erase(iter);
			}
		}
	}

	// Delete the LDAP configuration that does not exist
	for (UI32 i = 0; i < delLDAPIndxs.size(); i++)
	{
		ret = secLdapRemove(&pldapconf[delLDAPIndxs[i]]);
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("Deleting LDAP server '") + pldapconf[delLDAPIndxs[i]].hostname + string("' from backend FS failed."));
			for (SI32 j = 0; j < ldapNum; j++) {
				FREE(pldapconf[j].hostname);
				FREE(pldapconf[j].domain);
			}
			FREE(pldapconf);
			return (-1);
		}
	}

	// Add the LDAP hosts from wave DB to FS
    for (UI32 i = 0; i < addLDAPIndxs.size(); i++)
    {
		LDAPHostManagedObject *pLDAPObj = pLDAPObjs[addLDAPIndxs[i]];
		secLdapConf_t ldapConf;

		memset(&ldapConf, 0, sizeof (secLdapConf_t));

		ldapConf.hostname = STRDUP((pLDAPObj->getHost()).c_str());
		if ((pLDAPObj->getBasedn()).length())
			ldapConf.domain = STRDUP((pLDAPObj->getBasedn()).c_str());
		ldapConf.timeout = pLDAPObj->getTimeout();
		ldapConf.port = pLDAPObj->getPort();

		ret = secLdapAdd(&ldapConf);
		FREE(ldapConf.domain);
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("Adding ldap server '") + ldapConf.hostname + string("' to backend FS failed."));
			for (SI32 j = 0; j < ldapNum; j++) {
				FREE(pldapconf[j].hostname);
				FREE(pldapconf[j].domain);
			}
			FREE(pldapconf);
			FREE(ldapConf.hostname);
			return (-1);
		}
		FREE(ldapConf.hostname);
	}
	for (SI32 j = 0; j < ldapNum; j++) {
		FREE(pldapconf[j].hostname);
		FREE(pldapconf[j].domain);
	}
	FREE(pldapconf);

	for (UI32 i = 0; i < pLDAPObjs.size(); i++) delete pLDAPObjs[i];

	return (ret);
}

int AAALocalLocalObjectManager::syncMapRoles()
{
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	int ret = -1;
	ret = secLdapMapClean();
	if (ret != 0) {
		trace (TRACE_LEVEL_ERROR, string("Cleaning LDAP Map roles in the backend FS failed."));
		return (-1);
	}
	
	// Retrieve LDAP hosts from Wave DB
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(MapRoleManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);

	if (pResults == NULL) {
		trace (TRACE_LEVEL_ERROR, "presults is null");
		delete synchronousQueryContext;
		return (-1);
	}

	for (UI32 i = 0; i < pResults->size(); i++)
	{
		MapRoleManagedObject *mapRoleObj = dynamic_cast<MapRoleManagedObject *> ((*pResults)[i]);
		if (mapRoleObj == NULL) {
			continue;
		}
			
		//string cmd = "ldapcfg --maprole " + mapRoleObj->getGroup() + " " + mapRoleObj->getRole();
		ret = secLdapMapRole((mapRoleObj->getGroup()).c_str(), (mapRoleObj->getRole()).c_str());
		if (ret) {
			trace (TRACE_LEVEL_ERROR, string("Mapping AD group ") + mapRoleObj->getGroup() + " to switch role " + mapRoleObj->getRole() + " in backend FS is failed. Error code " + ret);
		} else {
			trace (TRACE_LEVEL_INFO, string("Mapping AD group ") + mapRoleObj->getGroup() + " to switch role " + mapRoleObj->getRole() + " in backend FS is successful.");
		}
		delete mapRoleObj;
	}
	delete synchronousQueryContext;
	delete pResults;

	return (ret);
}

int AAALocalLocalObjectManager::syncExecAccounting()
{
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	int retVal = 0;
	int srvType = -1;
	trace (TRACE_LEVEL_DEBUG, "AAALocalLocalObjectManager::syncExecAccounting()");

	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(ExecAccountingManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);

	if ((NULL == pResults) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_FATAL, "Unexpected number of login acc instances");
		delete synchronousQueryContext;
		if (pResults) {
			delete pResults;
		}
		return (-1);
	}

	ExecAccountingManagedObject *pExecAccountingMO = dynamic_cast<ExecAccountingManagedObject *> ((*pResults)[0]);
	if (pExecAccountingMO == NULL) {
		trace (TRACE_LEVEL_ERROR, "pExecAccountingMO is null");
		if (pResults) {
			 delete pResults;
		}
		delete synchronousQueryContext;
		return (-1);
	}

	delete pResults;
	delete synchronousQueryContext;

	srvType = pExecAccountingMO->getAccSrvType();

	if (TACACS_SRV == srvType) {
		retVal = secTacacsExecAccSet(1);
	} else if (NO_SRV == srvType) {
		retVal = secTacacsExecAccSet(0);
	}

	if (pExecAccountingMO)
		delete(pExecAccountingMO);

	return (retVal);
}

int AAALocalLocalObjectManager::syncCmdAccounting()
{       
    WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
    vector<WaveManagedObject *> *pResults = NULL;
    int retVal = 0;
    int srvType = -1;
    trace (TRACE_LEVEL_DEBUG, "AAALocalLocalObjectManager::syncCmdAccounting()");

    synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(CmdAccountingManagedObject::getClassName());
    pResults = querySynchronously (synchronousQueryContext);
        
    if ((NULL == pResults) || (pResults->size() != 1))
    {   
        trace (TRACE_LEVEL_FATAL, "Unexpected number of Command acc instances");
        delete synchronousQueryContext;
        if (pResults) {
            delete pResults;
        }
        return (-1);
    }

    CmdAccountingManagedObject *pCmdAccountingMO = dynamic_cast<CmdAccountingManagedObject *> ((*pResults)[0]);
    if (pCmdAccountingMO == NULL) {
        trace (TRACE_LEVEL_ERROR, "pCmdAccountingMO is null");
        if (pResults) {
             delete pResults;
        }
        delete synchronousQueryContext;
        return (-1);
    }

    delete pResults;
    delete synchronousQueryContext;

    srvType = pCmdAccountingMO->getAccSrvType();

    if (TACACS_SRV == srvType) {
        retVal = secTacacsCmdAccSet(1);
    } else if (NO_SRV == srvType) {
        retVal = secTacacsCmdAccSet(0);
    }

	if (pCmdAccountingMO)
		delete(pCmdAccountingMO);

    return (retVal);
}

int AAALocalLocalObjectManager::sendAccMessage(int srvType, int startStop, char *user,
								char *userPort, char *userHost, char *cmd, char *time, char *status)
{
	int retVal = -1;
	if (TACACS_SRV == srvType) {
		retVal = secSendTacacsAcctMsg(startStop, user, userPort, userHost, cmd,
							time, status);
	}
	return (retVal);
}

int AAALocalLocalObjectManager::syncAuthMode()
{
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	int ret = 0;
	/* Check integrity of AAA Server Mode */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(AuthLoginModeManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_FATAL, "Unexpected number of AuthLoginMode instances");
		delete synchronousQueryContext;
		if (pResults) {
			delete pResults;
		}
		return (-1);
	}
	AuthLoginModeManagedObject *pAuthLoginModeObj = dynamic_cast<AuthLoginModeManagedObject *> ((*pResults)[0]);
	if (pAuthLoginModeObj == NULL) {
		trace (TRACE_LEVEL_ERROR, "pAuthLoginModeObj is null");
		if (pResults) {
			delete pResults;
		}
		delete synchronousQueryContext;
		return (-1);
	}
	delete pResults;
	delete synchronousQueryContext;

	/*
	 * Change the netlogin/ttylogin/sshd PAM configuration files in the file system
	 */
	AAAServerType first = pAuthLoginModeObj->getFirst();
	AAAServerType second = pAuthLoginModeObj->getSecond();

	switch (first)
	{
	case LOCAL_DB:
		if (second == NO_AAA)
		{
			ret = secAuthConfig(LOCAL_ONLY, false, false, false, false);
		}
		else
		{
			ret = -1;
			trace (TRACE_LEVEL_FATAL, "AAA Server type combination specified is invalid.");
		}
		break;

	case RADIUS_DB:
		if (second == NO_AAA)
		{
			ret = secAuthConfig(RADIUS_ONLY, true, false, false, false);
		}
		else if (second == LOCAL_DB)
		{
			ret = secAuthConfig(RADIUS_LOCLBKUP, true, false, false, false);
		}
		else if (second == LOCALBKUP_DB)
		{
			ret = secAuthConfig(RADIUS_LOCAL, true, false, false, false);
		}
		else
		{
			ret = -1;
			trace (TRACE_LEVEL_FATAL, "AAA Server type combination specified is invalid");
		}
		break;

	case TACACS_DB:
		if (second == NO_AAA)
		{
			ret = secAuthConfig(TACACS_ONLY, false, false, true, false);
		}
		else if (second == LOCAL_DB)
		{
			ret = secAuthConfig(TACACS_LOCLBKUP, false, false, true, false);
		}
		else if (second == LOCALBKUP_DB)
		{
			ret = secAuthConfig(TACACS_LOCAL, false, false, true, false);
		}
		else
		{
			ret = -1;
			trace (TRACE_LEVEL_FATAL, "AAA Server type combination specified is invalid");
		}
		break;

	case LDAP_DB:
		if (second == NO_AAA)
		{
			ret = secAuthConfig(LDAP_ONLY, false, true, false, false);
		}
		else if (second == LOCAL_DB)
		{
			ret = secAuthConfig(LDAP_LOCLBKUP, false, true, false, false);
		}
		else if (second == LOCALBKUP_DB)
		{
			ret = secAuthConfig(LDAP_LOCAL, false, true, false, false);
		}
		else
		{
			ret = -1;
			trace (TRACE_LEVEL_FATAL, "AAA Server type combination specified is invalid");
		}
		break;

	default:
		ret = -1;
		trace (TRACE_LEVEL_FATAL, "Invalid AAA Server type is specified");
		break;
	}

	delete pAuthLoginModeObj;

	/* Return OK if previous mode in the File-System is same the value in DCM DB */
	if (ret == LDAP_CONFIG_ENABLED)
	{
		trace (TRACE_LEVEL_INFO, "AAA Server authentication sequence is already same");
		ret = 0;
	}

	return (ret);
}

int AAALocalLocalObjectManager::syncPasswdOptions()
{
	secPwdCfg_t     passwd_dbcfg;
	secPwdCfg_t     passwd_fscfg;
	int				isChanged = 0, ret = 0;

	/* Retrieve the Password Attributes Managed Object */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(PasswordAttributesManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		if ((pResults == NULL) || (pResults->size() == 0)) trace (TRACE_LEVEL_FATAL, "Zero PasswordAttribs instances found.");
		else if (pResults->size() != 1) trace (TRACE_LEVEL_FATAL, "More than one PasswordAttribs instances found.");
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (unsigned int j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		return (-1);
	}

	PasswordAttributesManagedObject *pPasswordAttributesManagedObj = dynamic_cast<PasswordAttributesManagedObject *> ((*pResults)[0]);
	if (pPasswordAttributesManagedObj == NULL) {
		trace (TRACE_LEVEL_ERROR, "pPasswordAttributesManagedObj is null");
		delete pResults;
		delete synchronousQueryContext;
		return (-1);
	}

	/* Setting default to be compatible with FOS Security library */
	passwd_fscfg.min_len = PW_CFG_NO_CHANGE;
	passwd_fscfg.lower = PW_CFG_NO_CHANGE;
	passwd_fscfg.upper = PW_CFG_NO_CHANGE;
	passwd_fscfg.digit = PW_CFG_NO_CHANGE;
	passwd_fscfg.punct = PW_CFG_NO_CHANGE;
	passwd_fscfg.history = PW_CFG_NO_CHANGE;
	passwd_fscfg.minpassage = PW_CFG_NO_CHANGE;
	passwd_fscfg.maxpassage = PW_CFG_NO_CHANGE;
	passwd_fscfg.warn = PW_CFG_NO_CHANGE;
	passwd_fscfg.lockthresh = PW_CFG_NO_CHANGE;
	passwd_fscfg.lockdur = PW_CFG_NO_CHANGE;
	passwd_fscfg.adminlockout = PW_CFG_NO_CHANGE;
	passwd_fscfg.repeat = PW_CFG_NO_CHANGE;
	passwd_fscfg.sequence = PW_CFG_NO_CHANGE;
	passwd_fscfg.status = PW_CFG_NO_CHANGE;

	/* Setting the object with the user input */
	passwd_dbcfg.min_len = pPasswordAttributesManagedObj->getMinlength();
	passwd_dbcfg.lockthresh = pPasswordAttributesManagedObj->getMaxretry();
	passwd_dbcfg.upper = pPasswordAttributesManagedObj->getUppercase();
	passwd_dbcfg.lower = pPasswordAttributesManagedObj->getLowercase();
	passwd_dbcfg.digit = pPasswordAttributesManagedObj->getNumerics();
	passwd_dbcfg.punct = pPasswordAttributesManagedObj->getSplchars();
	passwd_dbcfg.adminlockout = pPasswordAttributesManagedObj->getAdminlockout();

	if (secPwdCfgGetAll(&passwd_fscfg) == PW_CFG_SUCCESS) {
		if (passwd_dbcfg.min_len != passwd_fscfg.min_len) {
			passwd_fscfg.min_len = passwd_dbcfg.min_len;
			isChanged = 1;
		}
		if (passwd_dbcfg.lockthresh != passwd_fscfg.lockthresh) {
			passwd_fscfg.lockthresh = passwd_dbcfg.lockthresh;
			isChanged = 1;
		}
		if (passwd_dbcfg.upper != passwd_fscfg.upper) {
			passwd_fscfg.upper = passwd_dbcfg.upper;
			isChanged = 1;
		}
		if (passwd_dbcfg.lower != passwd_fscfg.lower) {
			passwd_fscfg.lower = passwd_dbcfg.lower;
			isChanged = 1;
		}
		if (passwd_dbcfg.digit != passwd_fscfg.digit) {
			passwd_fscfg.digit = passwd_dbcfg.digit;
			isChanged = 1;
		}
		if (passwd_dbcfg.punct != passwd_fscfg.punct) {
			passwd_fscfg.punct = passwd_dbcfg.punct;
			isChanged = 1;
		}
		if (passwd_dbcfg.adminlockout != passwd_fscfg.adminlockout) {
			passwd_fscfg.adminlockout = passwd_dbcfg.adminlockout;
			isChanged = 1;
		}
	} else {
		ret = -1;
	}

	if (isChanged) {
		ret = secPwdCfgSet(&passwd_fscfg, NULL, 0);
	}

	delete pResults;
	delete synchronousQueryContext;
	delete pPasswordAttributesManagedObj;

	return (ret);
}

int AAALocalLocalObjectManager::syncBanner()
{
	char    buf[SEC_MAX_BANNER_LEN+1];
        char    buf_motd[SEC_MAX_BANNER_LEN+1];
        char    buf_incoming[SEC_MAX_BANNER_LEN+1];
	BannerManagedObject *pBannerManagedObj = NULL;
	int ret = 0;

        ret = secBannerGet(buf, sizeof (buf), MYSWITCH, LOGIN_BANNER);
        if (ret != SEC_OK) {
                trace (TRACE_LEVEL_FATAL, "syncBannerGet failed, Login Banner");
                return (ret);
        }

        ret = secBannerGet(buf_motd, sizeof (buf_motd), MYSWITCH, MOTD_BANNER);
        if (ret != SEC_OK) {
                trace (TRACE_LEVEL_FATAL, "syncBannerGet failed, MOTD Banner");
                return (ret);
        }

        ret = secBannerGet(buf_incoming, sizeof (buf_incoming), MYSWITCH, INCOMING_BANNER);
        if (ret != SEC_OK) {
                trace (TRACE_LEVEL_FATAL, "syncBannerGet failed, Incoming Banner");
                return (ret);
        }

	/* Retrieve the Banner Managed Object */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(BannerManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		if ((pResults == NULL) || (pResults->size() == 0)) trace (TRACE_LEVEL_FATAL, "Zero Banner instances found.");
		else if (pResults->size() > 1) trace (TRACE_LEVEL_FATAL, "More than one Banner instances found.");
		ret = -1;
	}

	if (ret == 0) {
		pBannerManagedObj = dynamic_cast<BannerManagedObject *> ((*pResults)[0]);
		if (pBannerManagedObj == NULL) {
			trace (TRACE_LEVEL_ERROR, "pBannerManagedObj is null");
			delete synchronousQueryContext;
			delete pResults;
			return (-1);
		}
		string s = pBannerManagedObj->getLogin();
		const char *dbBanner = s.c_str();
		if (strcmp(buf,  dbBanner)) {
                        ret = secBannerSet(dbBanner, MYSWITCH, LOGIN_BANNER);
                }

                string s_motd = pBannerManagedObj->getMotd();
                const char *dbBanner_motd = s_motd.c_str();
                if (strcmp(buf_motd,  dbBanner_motd)) {
                        ret = secBannerSet(dbBanner_motd, MYSWITCH, MOTD_BANNER);
                }

                string s_incoming = pBannerManagedObj->getIncoming();
                const char *dbBanner_incoming = s_incoming.c_str();
                if (strcmp(buf_incoming,  dbBanner_incoming)) {
                        ret = secBannerSet(dbBanner_incoming, MYSWITCH, INCOMING_BANNER);
                }

		delete pBannerManagedObj;
	}

	return (ret);
}

/* Sync the IpAcls. */
int AAALocalLocalObjectManager::syncIp6Acls()
{
	trace (TRACE_LEVEL_INFO, "Enter AAALocalLocalObjectManager::syncIp6Acls");
	ipfExtnPolicy_t pol;
	int ret = -1;
	Ip6AclPolicyLocalManagedObject *pIpAclPolicyLocalMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	ipf_txn_hdl_t txnHdl;
	string ipAclPolicy;
	//ResourceId status = WAVE_MESSAGE_SUCCESS;
	vector <LocationId> locVector;

	
	// 1. Get the current active policy from the backend(Old policy). No transaction is required for get  policy.
	ret = ipfActivePolicyGet(IPF_IPV6, &pol);
	if (ret) {
		trace (TRACE_LEVEL_ERROR, "Couldn't get the active policy from the switch");
		return (ret);
	}

	// Get the enforced policy from the Local Manged Object.
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(Ip6AclPolicyLocalManagedObject::getClassName());
	int localId = DcmToolKit::getLocalMappedId();
	trace (TRACE_LEVEL_INFO, string ("Local id is ") + localId);
	int itf = -1;
	string switchId;
	itf = findActiveInterface();
	if (itf != -1)
	{
		trace (TRACE_LEVEL_INFO, "getting the switch ids");
		switchId = localId + string ("/") + itf;
		trace (TRACE_LEVEL_INFO, string ("getting the switch id is ") + switchId);
	}
	else
	{
		delete pSyncQueryCtx;
		return (-1);
	}
	pSyncQueryCtx->addAndAttribute (new AttributeString (switchId, "switchid"));
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults)
	{
		trace (TRACE_LEVEL_FATAL, "IP Acl Local Policy database doesn't exist");
		delete pSyncQueryCtx;
		return (-1);
	} else if (pResults->size() == 0) {
		trace (TRACE_LEVEL_INFO, "IP Acl pResults Size is zero");
		//Verify whether default policy is activated or not.
		//If the default policy is not activated activate it now.
		delete pSyncQueryCtx;
		delete pResults;

		if (strcmp(pol.name, IPF_DEFAULT6_POLICY_NAME) == 0) {
			// Expected case if policy is not enforced, default policy should be activated in the back end.	
			return (0);	
		}
		txnHdl = ipfTransBegin(); // Transaction starts here.
		if (txnHdl <= 0) {
			trace (TRACE_LEVEL_ERROR, "Transaction is locked for IP Acls");
			return (-1);
		}
		// Activate the default policy and delete the existing policy.	
		ret = ipfPolicyActivate(IPF_DEFAULT6_POLICY_NAME);
		if (ret) {
			ipfTransAbort(txnHdl);
			return (ret);	
		}

		ret = ipfPolicyDelete(pol.name);
		if (ret) {
			ipfTransAbort(txnHdl);
			return (ret);	
		}
		ipfPolicySave(NULL);
		ipfTransEnd(txnHdl); // Transaction ends here.
		return (ret);
	} else {
		pIpAclPolicyLocalMO = dynamic_cast<Ip6AclPolicyLocalManagedObject *>((*pResults)[0]);
	}
	delete pResults;
	delete pSyncQueryCtx;

	// Activate the policy.
	ipAclPolicy = pIpAclPolicyLocalMO->getIpv6AclPolicy();
	trace (TRACE_LEVEL_DEBUG, string("IP6 Acl policy") + ipAclPolicy);

		//  Delete the old policy if it is not a default
		if (strcmp(pol.name, IPF_DEFAULT6_POLICY_NAME) != 0) {
			txnHdl = ipfTransBegin(); // Transaction starts here.
			if (txnHdl <= 0) {
				delete pIpAclPolicyLocalMO;
				trace (TRACE_LEVEL_ERROR, "Transaction is locked for IP Acls");
				return (-1);
			}
		// Activate the default policy and delete the existing policy.	
		ret = ipfPolicyActivate(IPF_DEFAULT6_POLICY_NAME);
		if (ret) {
			ipfTransAbort(txnHdl);
			delete pIpAclPolicyLocalMO;
			return (ret);	
		}
			ret = ipfPolicyDelete(pol.name);
			if (ret) {
				ipfTransAbort(txnHdl);
				delete pIpAclPolicyLocalMO;
				return (ret);
			}
			ipfPolicySave(NULL);
			ipfTransEnd(txnHdl);
		}
	m_pLocalIpAclWorker->enforceIp6AclPolicy(ipAclPolicy, switchId, FALSE, ACL_IPV6);

	delete pIpAclPolicyLocalMO;
	return (0);
}

/* Sync the IpAcls. */
int AAALocalLocalObjectManager::syncIpAcls()
{
	trace (TRACE_LEVEL_INFO, "Enter AAALocalLocalObjectManager::syncIpAcls()");
	ipfExtnPolicy_t pol;
	int ret = -1;
	IpAclPolicyLocalManagedObject *pIpAclPolicyLocalMO = NULL;

	WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	ipf_txn_hdl_t txnHdl;
	string ipAclPolicy;
	//ResourceId status = WAVE_MESSAGE_SUCCESS;
	vector <LocationId> locVector;
	
	// 1. Get the current active policy from the backend(Old policy). No transaction is required for get  policy.
	ret = ipfActivePolicyGet(IPF_IPV4, &pol);
	if (ret) {
		trace (TRACE_LEVEL_ERROR, "Couldn't get the active policy from the switch");
		return (ret);
	}

	// Get the enforced policy from the Local Manged Object.
	pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(IpAclPolicyLocalManagedObject::getClassName());
	int localId = DcmToolKit::getLocalMappedId();
	trace (TRACE_LEVEL_INFO, string ("Local id is ") + localId);
	int itf = -1;
	string switchId;
	itf = findActiveInterface();
	if (itf != -1)
	{
		trace (TRACE_LEVEL_INFO, string ("getting the switch id"));
		switchId = localId + string ("/") + itf;
		trace (TRACE_LEVEL_INFO, string ("getting the switch id is ") + switchId);
	}
	else
	{
		delete pSyncQueryCtx;
		return (-1);
	}
	
	pSyncQueryCtx->addAndAttribute (new AttributeString (switchId, "switchid"));
	pResults = querySynchronously (pSyncQueryCtx);
	if (!pResults)
	{
		trace (TRACE_LEVEL_FATAL, "IP Acl Local Policy database doesn't exist");
		delete pSyncQueryCtx;
		return (-1);
	} else if (pResults->size() == 0) {
		trace (TRACE_LEVEL_INFO, "IP Acl pResults Size is zero" + switchId);
		//Verify whether default policy is activated or not.
		//If the default policy is not activated activate it now.
		delete pSyncQueryCtx;
		delete pResults;

		if (strcmp(pol.name, IPF_DEFAULT_POLICY_NAME) == 0) {
			// Expected case if policy is not enforced, default policy should be activated in the back end.	
			return (0);	
		}
		txnHdl = ipfTransBegin(); // Transaction starts here.
		if (txnHdl <= 0) {
			trace (TRACE_LEVEL_ERROR, "Transaction is locked for IP Acls");
			return (-1);
		}
		// Activate the default policy and delete the existing policy.	
		ret = ipfPolicyActivate(IPF_DEFAULT_POLICY_NAME);
		if (ret) {
			ipfTransAbort(txnHdl);
			return (ret);	
		}

		ret = ipfPolicyDelete(pol.name);
		if (ret) {
			ipfTransAbort(txnHdl);
			return (ret);	
		}
		ipfPolicySave(NULL);
		ipfTransEnd(txnHdl); // Transaction ends here.
		return (ret);
	} else {
		pIpAclPolicyLocalMO = dynamic_cast<IpAclPolicyLocalManagedObject *>((*pResults)[0]);
	}
	delete pResults;
	delete pSyncQueryCtx;

	// Get the policy from DB.
	ipAclPolicy = pIpAclPolicyLocalMO->getIpAclPolicy();

	trace (TRACE_LEVEL_DEBUG, string("IP Acl policy") + ipAclPolicy);
		//  Delete the old policy if it is not a default
		if (strcmp(pol.name, IPF_DEFAULT_POLICY_NAME) != 0) {
			txnHdl = ipfTransBegin(); // Transaction starts here.
			if (txnHdl <= 0) {
				delete pIpAclPolicyLocalMO;
				trace (TRACE_LEVEL_ERROR, "Transaction is locked for IP Acls");
				return (-1);
			}
		// Activate the default policy and delete the existing policy.	
		ret = ipfPolicyActivate(IPF_DEFAULT_POLICY_NAME);
		if (ret) {
			ipfTransAbort(txnHdl);
			delete pIpAclPolicyLocalMO;
			return (ret);	
		}
			ret = ipfPolicyDelete(pol.name);
			if (ret) {
				ipfTransAbort(txnHdl);
				delete pIpAclPolicyLocalMO;
				return (ret);
			}
			ipfPolicySave(NULL);
			ipfTransEnd(txnHdl);
	}
	m_pLocalIpAclWorker->enforceIpAclPolicy(ipAclPolicy, switchId, FALSE, ACL_IPV4);
	delete pIpAclPolicyLocalMO;
	return (0);
}

int AAALocalLocalObjectManager::syncTelnetServerConfig()
{
	TelnetServerLocalManagedObject *pTelnetServerLocalMO = NULL;
	int ret = 0;
    bool disableFlag = false;

    trace (TRACE_LEVEL_INFO, "AAALocalLocalObjectManager:syncTelnetServerConfig: Entered");

#if 0
	/* Retrieve the Telnet Server Global Managed Object */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(TelnetServerLocalManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
#endif
	LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
	vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, TelnetServerLocalManagedObject::getClassName());
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		if ((pResults == NULL) || (pResults->size() == 0)) trace (TRACE_LEVEL_INFO, "Telnet Server configuration not found.");
		else if (pResults->size() > 1) trace (TRACE_LEVEL_INFO, "More than one Telnet server configuration instances found.");
		/* No need to sync to backend: sync only if the MO exists, MO will be created only when the command is executed for the first time */
		return(ret);
		// ret = -1;
	}

	if (ret == 0) {
		pTelnetServerLocalMO = dynamic_cast<TelnetServerLocalManagedObject *> ((*pResults)[0]);
		if (pTelnetServerLocalMO == NULL) {
            ret = system("/bin/grep ^telnet /etc/inetd.conf");
            if (ret == 0)
            {
			    trace (TRACE_LEVEL_ERROR, "pTelnetServerGlobalMO is null");
			    delete pResults;
			    return (-1);
            }
            else
            {
                ret = system("/fabos/libexec/inetdUpdate 1");
			    delete pResults;
                return (ret);
            }
		}
		disableFlag = pTelnetServerLocalMO->getDisableFlag();

        if (disableFlag == true) {
            ret = system("/usr/bin/killall -q in.telnetd; /fabos/libexec/inetdUpdate 0");
        } else {
            ret = system("/fabos/libexec/inetdUpdate 1");
        }

		delete pTelnetServerLocalMO;
	}

	return (ret);
}

int AAALocalLocalObjectManager::syncSSHServerConfig()
{
	SSHServerLocalManagedObject *pSSHServerLocalMO = NULL;
	int ret = 0;
    trace (TRACE_LEVEL_INFO, "AAALocalLocalObjectManager:syncSSHServerConfig: Entered");

#if 0
	/* Retrieve the SSH Server Global Managed Object */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(SSHServerLocalManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
#endif
	LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
	vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SSHServerLocalManagedObject::getClassName());
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		if ((pResults == NULL) || (pResults->size() == 0)) trace (TRACE_LEVEL_INFO, "SSH Server configuration not found.");
		else if (pResults->size() > 1) trace (TRACE_LEVEL_INFO, "More than one SSH server configuration instances found.");
		/* No need to sync to backend: sync only if the MO exists, MO will be created only when the command is executed for the first time */
		return(ret);
		// ret = -1;
	}

	if (ret == 0) {
		pSSHServerLocalMO = dynamic_cast<SSHServerLocalManagedObject *> ((*pResults)[0]);
		if (pSSHServerLocalMO == NULL) {
			trace (TRACE_LEVEL_ERROR, "pSSHServerGlobalMO is null");
			delete pResults;
			return (-1);
		}
		bool disableFlag = pSSHServerLocalMO->getDisableFlag();

        if (disableFlag == true) {
            ret = system("/usr/bin/killall -q sshd");
        } else {
            ret = system("/usr/sbin/sshd");
        }

		delete pSSHServerLocalMO;
	}

	return (ret);
}

int AAALocalLocalObjectManager::syncSSHServerListConfig()
{
	SSHServerListLocalManagedObject *pSSHServerListMO = NULL;
	int ret = 0;
    trace (TRACE_LEVEL_DEBUG, "AAALocalLocalObjectManager:syncSSHServerListConfig: Entered");

#if 0
	/* Retrieve the SSH Server Global Managed Object */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(SSHServerListLocalManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
#endif
	LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
	vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SSHServerListLocalManagedObject::getClassName());
	if (NULL == pResults)
	{
		if (pResults == NULL) trace (TRACE_LEVEL_FATAL, "SSH Server configuration not found.");
		return (-1);
	} else if (0 == pResults->size()) {
        trace (TRACE_LEVEL_DEBUG, "Commenting KexAlgorithms in sshd_config");
        ret = system("/fabos/sbin/sshdconfigupdate KexAlgorithms 0");
        if (pResults) {
            delete pResults;
        }
        return (WEXITSTATUS(ret));
    }

    pSSHServerListMO = dynamic_cast<SSHServerListLocalManagedObject *> ((*pResults)[0]);
	if (pSSHServerListMO == NULL) {
		trace (TRACE_LEVEL_ERROR, "pSSHServerListMO is null");
		delete pResults;
		return (-1);
	}
	SSHLocalKeyExchangeProtocol protocol = pSSHServerListMO->getKeyExchangeProtocol();

    if (SSH_KEX_DH_GROUP_14 == protocol) {
        trace (TRACE_LEVEL_DEBUG, "Uncommenting KexAlgorithms in sshd_config");
        ret = system("/fabos/sbin/sshdconfigupdate KexAlgorithms 1");
    }

	delete pSSHServerListMO;
    delete pResults;

	return (ret);
}

void AAALocalLocalObjectManager::haboot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "AAALocalLocalObjectManager::haboot : Entering ...");
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason();
	
	trace(TRACE_LEVEL_INFO, string("AAA Local 'haboot' Service called with reason - ") + FrameworkToolKit::localize(bootReason));
	
	/*
	 * When copy running or copy <file> on active CP happens standby CP invokes this function with
	 * below reason codes.
	 */
	if (bootReason == WAVE_BOOT_HASTANDBY_BOOT)
	{
		m_isPersistentRejoinBoot = true;
	}
	pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
	pWaveAsynchronousContextForBootPhases->callback ();
}

void AAALocalLocalObjectManager::boot(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
	trace (TRACE_LEVEL_INFO, "AAALocalLocalObjectManager::boot()----ipacltrack");
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason();

	trace(TRACE_LEVEL_INFO, string("AAA Local 'boot' Service called with reason - ") + FrameworkToolKit::localize(bootReason));
	if ((bootReason == WAVE_BOOT_PERSISTENT_BOOT) ||
		(bootReason == WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT))
	{
		m_isPersistentRejoinBoot = true;

		// Defect 440386 - Invoke syncAAAPlugins here for handing node replacement
		// scenario
		if (bootReason == WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT) 
		{
			syncAAAPlugins();
		}
	} else if ((bootReason == WAVE_BOOT_FIRST_TIME_BOOT) ||
		(bootReason == WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT)) 
	{
		m_isPersistentRejoinBoot = false;
	}


	pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
	pWaveAsynchronousContextForBootPhases->callback ();
}

void AAALocalLocalObjectManager::syncAAAPlugins()
{
	int ret = 0;
	//ResourceId status = WAVE_MESSAGE_SUCCESS;

	trace (TRACE_LEVEL_DEBUG, "AAALocalLocalObjectManager::syncAAAPlugins() Enter");

	if (ret == 0)
	{
		ret = syncRolesAndUsers(false);
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing User/Role database failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing User/Role database succeeded.");
		}
	}

	if (ret == 0)
	{
		ret = syncPasswdOptions();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing password attributes with chassis config db failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing password attributes with chassis config db succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncRadiusHosts();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing radius hosts' configuration to the file-system failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing radius hosts' configuration to the file-system succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncTacacsHosts();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing tacacs+ hosts' configuration to the file-system failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing tacacs+ hosts' configuration to the file-system succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncLDAPHosts();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing LDAP hosts' configuration to the file-system failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing LDAP hosts' configuration to the file-system succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncMapRoles();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing Map roles configuration to the file-system failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing Map roles configuration to the file-system succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncExecAccounting();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing AAA Login Accounting configuration to the file-system failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing AAA Login Accounting configuration to the file-system succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncCmdAccounting();
		if (ret  != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing AAA Command Accounting configuration to the file-system failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing AAA Command Accounting configuration to the file-system succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncAuthMode();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing AAA Authentication mode to the chassis config db failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing AAA Authentication mode to the chassis config db succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncBanner();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing the login banner to the file-system failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing the login banner to the file-system succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncIp6Acls();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing the Ip6 Acl to the file-system failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing the Ip6 Acl to the file-system succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncIpAcls();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing the Ip4 Acl to the file-system failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing the Ip4 Acl to the file-system succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncTelnetServerConfig();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing the Telnet server configuration to the file-system failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing the Telnet server configuration to the file-system succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncSSHServerConfig();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing the SSH server configuration to the file-system failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing the SSH server configuration to the file-system succeeded");
		}
	}

	if (ret == 0)
	{
		ret = syncSSHServerListConfig();
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, "Synchronizing the SSH server list configuration to the file-system failed.");
		} else {
			trace (TRACE_LEVEL_INFO, "Synchronizing the SSH server list configuration to the file-system succeeded");
		}
	}

	if (ret != 0)
	{
		//status = FRAMEWORK_ERROR;
	}
	else
	{
		trace (TRACE_LEVEL_INFO, "Synchronization of AAA plugins is successful.");
	}
	trace (TRACE_LEVEL_DEBUG, "AAALocalLocalObjectManager::syncAAAPlugins() Exit");
}

void AAALocalLocalObjectManager::externalStateSynchronization(WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization)
{
	trace (TRACE_LEVEL_INFO, "AAALocalLocalObjectManager::externalStateSynchronization()----ipacltrack");

	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	UI32 fssStage = pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber();
	ResourceId serviceType = pWaveAsynchronousContextForExternalStateSynchronization->getServiceType();

	if ((fssStage == AAA_CFG_STAGE) &&
		(serviceType == FCSW))
	{
		if (m_isPersistentRejoinBoot)
		{
		trace (TRACE_LEVEL_INFO, "Syncing AAA Plugins in FCSW FSS stage ... for persisent/secondary-rejoin boot");

		// Defect 440386 - Invoking syncAAAPlugins that contains the sync APIs
		// commented below
		syncAAAPlugins ();
#if 0
		if (ret == 0)
		{
			if ((ret = syncRolesAndUsers(false)) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing User/Role database failed.");
			}
		}

		if (ret == 0)
		{
			if ((ret = syncPasswdOptions()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing password attributes with chassis config db failed.");
			}
		}

		if (ret == 0)
		{
			if ((ret = syncRadiusHosts()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing radius hosts' configuration to the file-system failed.");
			}
		}

		if (ret == 0)
		{
			if ((ret = syncTacacsHosts()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing tacacs+ hosts' configuration to the file-system failed.");
			}
		}

		if (ret == 0)
		{
			if ((ret = syncLDAPHosts()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing LDAP hosts' configuration to the file-system failed.");
			}
		}

		if (ret == 0)
		{
			if ((ret = syncMapRoles()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing Map roles configuration to the file-system failed.");
			}
		}

		if (ret == 0)
		{
			if ((ret = syncExecAccounting()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing AAA Login Accounting configuration to the file-system failed.");
			}
		}

		if (ret == 0)
		{
			if ((ret = syncCmdAccounting()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing AAA Command Accounting configuration to the file-system failed.");
			}
		}

		if (ret == 0)
		{
			if ((ret = syncAuthMode()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing AAA Authentication mode to the chassis config db failed.");
			}
		}

		if (ret == 0)
		{
			if ((ret = syncBanner()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing the login banner to the file-system failed.");
			}
		}

		if (ret == 0)
		{
			if ((ret = syncIp6Acls()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing the Ip6 Acl to the file-system failed.");
			}
		}
		if (ret == 0)
		{
			if ((ret = syncIpAcls()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing the Ip4 Acl to the file-system failed.");
			}
		}
		if (ret == 0)
		{
			if ((ret = syncTelnetServerConfig()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing the Telnet server configuration to the file-system failed.");
			}
		}
		if (ret == 0)
		{
			if ((ret = syncSSHServerConfig()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing the SSH server configuration to the file-system failed.");
			}
		}
		if (ret == 0)
		{
			if ((ret = syncSSHServerListConfig()) != 0) {
				trace (TRACE_LEVEL_ERROR, "Synchronizing the SSH server list configuration to the file-system failed.");
			}
		}

		if (ret != 0)
		{
			status = FRAMEWORK_ERROR;
		}
		else
		{
			trace (TRACE_LEVEL_INFO, "Synchronization of AAA plugins is successful.");
		}
#endif
		} else {
		trace (TRACE_LEVEL_INFO, "Syncing AAA Plugins in FCSW FSS stage ... for persisent-with-default-config boot");

		ret = syncRolesAndUsers(true);
		if (ret) trace (TRACE_LEVEL_ERROR, string("Defaulting password database failed. ret=") + ret);

		ret = secRadiusConfigClean();
		if (ret) trace (TRACE_LEVEL_ERROR, string("Removing Radius config failed. ret=") + ret);

		ret = secTacacsConfigClean();
		if (ret) trace (TRACE_LEVEL_ERROR, string("Removing Tacacs+ config failed. ret=") + ret);

		ret = secLdapConfigClean();
		if (ret) trace (TRACE_LEVEL_ERROR, string("Removing LDAP config failed. ret=") + ret);

		ret = secLdapMapClean();
		if (ret) trace (TRACE_LEVEL_ERROR, string("Removing LDAP  map roles config failed. ret=") + ret);

		ret = secTacacsExecAccSet(0);
		if (ret) trace (TRACE_LEVEL_ERROR, string("Defaulting AAA Login Accounting failed. ret=") + ret);

		ret = secTacacsCmdAccSet(0);
		if (ret) trace (TRACE_LEVEL_ERROR, string("Defaulting AAA Command Accounting failed. ret=") + ret);

		ret = secAuthConfig(LOCAL_ONLY, false, false, false, false);
		if (ret) trace (TRACE_LEVEL_ERROR, string("Defaulting AAA Auth Login mode failed. ret=") + ret);

		ret = secPwdCfgSetDefault();
		if (ret) trace (TRACE_LEVEL_ERROR, string("Defaulting password attributes failed. ret=") + ret);

		ret = secBannerSet("", 0, LOGIN_BANNER);
		if (ret) trace (TRACE_LEVEL_ERROR, string("Removing Login banner failed. ret=") + ret);

		ret = secBannerSet("", 0, MOTD_BANNER);
		if (ret) trace (TRACE_LEVEL_ERROR, string("Removing MOTD banner failed. ret=") + ret);

		ret = secBannerSet("", 0, INCOMING_BANNER);
		if (ret) trace (TRACE_LEVEL_ERROR, string("Removing Incoming banner failed. ret=") + ret);

		// In this case if there is any non-default active policy, it will be deleted.
		ret = syncIpAcls();
		if (ret) trace (TRACE_LEVEL_ERROR, string("Deleting IpAcls failed. ret=") + ret);
		
		ret = syncIp6Acls();
		if (ret) trace (TRACE_LEVEL_ERROR, string("Deleting Ip6Acls failed. ret=") + ret);

		} // End of if (m_isPersistentRejoinBoot)

		trace (TRACE_LEVEL_INFO, "AAALocalLocalObjectManager::externalStateSynchronization() - exit");
	}

    pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (status);
    pWaveAsynchronousContextForExternalStateSynchronization->callback ();
}

void AAALocalLocalObjectManager::install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
	trace (TRACE_LEVEL_INFO, "AAALocalLocalObjectManager::install()---ipacltrack");

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason();

#if 0
	/* Node is out-of-the-box or is rebooting after net-install is performed */
    if ((bootReason == WAVE_BOOT_FIRST_TIME_BOOT) ||
			(bootReason == WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT)) {
		startTransaction ();
		/* Install Telnet Server singleton managed object */
        TelnetServerLocalManagedObject *pTelnetServerGlobalMO = new TelnetServerLocalManagedObject(this, false);
		/* Install SSH Server singleton managed object */
        SSHServerLocalManagedObject *pSSHServerGlobalMO = new SSHServerLocalManagedObject(this, false);
		status = commitTransaction ();

		if (FRAMEWORK_SUCCESS == status) {
			trace (TRACE_LEVEL_INFO, "Installed AAA Managed Objects");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_FATAL, "Failed to install AAA Managed Objects");
			prismAssert (false, __FILE__, __LINE__);
        }

		delete pTelnetServerGlobalMO;
		delete pSSHServerGlobalMO;
	}
#endif

	/* This node has joined the cluster (for the first time) and synced its Wave DB */
	if (bootReason == WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT)
	{
	trace(TRACE_LEVEL_INFO, string("AAA Local install received - ") + FrameworkToolKit::localize(bootReason));

	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	int ret = 0;

	/* Push Role and User accounts */
	RoleManagedObject *pRoleObj = NULL;
	UserManagedObject *pUserObj = NULL;

	secAcct_t user;
	secRole_t role;

	ret = system("/sbin/passwddefault");
	//ret = syncRolesAndUsers();
	if (ret != 0) {
		trace (TRACE_LEVEL_INFO, "Defaulting the role and user account database in the FS failed.");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}

	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RoleManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	for (UI32 i = 0; i < pResults->size(); i++)
	{
		pRoleObj = dynamic_cast<RoleManagedObject *> ((*pResults)[i]);
		if (pRoleObj == NULL) {
			trace (TRACE_LEVEL_ERROR, "pRoleObj is null");
			continue;
		}

		/* Skip default roles */
		if ((pRoleObj->getName()).compare("user") == 0 ||
			(pRoleObj->getName()).compare("admin") == 0)
		{
			delete pRoleObj;
			continue;
		}
		memset(&role, 0, sizeof(secRole_t));

		role.name = STRDUP((pRoleObj->getName()).c_str());

		string description = pRoleObj->getDescription();
		if (description.length() > 0)
			role.desc = STRDUP(description.c_str());

		role.gid = pRoleObj->getGID();

		string users = pRoleObj->getUsers();
		if (users.length() > 0)
		{
			char *re_entry = NULL, *token = NULL;
			char *user_list = NULL;

			user_list = STRDUP(users.c_str());
			int j = 0;
			role.users = (char **)calloc(ACCT_MAX_NUM, sizeof (char *));
			token = strtok_r(user_list, ROLE_USERS_DELIMITER, &re_entry);
			while (token != NULL)
			{
				role.users[j++] = STRDUP(token);

				trace (TRACE_LEVEL_INFO, "role users " + string(role.users[j -1 ]));
				token = strtok_r(NULL, ROLE_USERS_DELIMITER, &re_entry);
			}
			role.users[j] = NULL;
			role.userNum = j;
			if (user_list)
				FREE(user_list);
		}

		trace (TRACE_LEVEL_INFO, string("Roles usernum") + role.userNum);
		ret = secRoleAdd(&role);

		FREE(role.name);
		FREE(role.desc);

		if (ret != 0)
		{
			trace (TRACE_LEVEL_FATAL, "Installing roles failed.");
			pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
			pWaveAsynchronousContextForBootPhases->callback ();
			return;
		}

		delete pRoleObj;
	}
	delete pResults;
	delete synchronousQueryContext;

	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	for (UI32 i = 0; i < pResults->size(); i++)
	{
		pUserObj = dynamic_cast<UserManagedObject *> ((*pResults)[i]);
		if (pUserObj == NULL) {
			trace (TRACE_LEVEL_ERROR, "pUserObj is null");
			continue;
		}

		/* Skip default users */
		if ((pUserObj->getName()).compare("user") == 0 ||
			(pUserObj->getName()).compare("admin") == 0)
		{
			delete pUserObj;
			continue;
		}

		memset(&user, 0, sizeof(secAcct_t));
		user.name = (char*)(pUserObj->getName()).c_str();
		user.passwd = (char*)(pUserObj->getMD5Password()).c_str();
		if (pUserObj->getDescription().length() > 0)
			user.desc = (char*)pUserObj->getDescription().c_str();
		user.enabled = pUserObj->getEnabled();
		user.locked = -1;

		RoleManagedObject *pRoleMO = dynamic_cast<RoleManagedObject *> (queryManagedObject(pUserObj->getRole()));
		if (pRoleMO == NULL) {
			delete pUserObj;
			continue;
		}
		user.role = (char*)(pRoleMO->getName()).c_str();

		ret = secAcctAdd(&user);
		if (ret != 0)
		{
			trace (TRACE_LEVEL_FATAL, "Installing user failed.");
			pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
			pWaveAsynchronousContextForBootPhases->callback ();
			return;
		}

		delete pUserObj;
		delete pRoleMO;
	}
	delete pResults;
	delete synchronousQueryContext;

	/* Push Radius Hosts */
	RadiusHostManagedObject *pRadiusHostObj = NULL;
	secRadConf_t *pradconf = NULL;

	// Cleanup
	ret = secRadiusConfigClean();
	if (ret != 0) {
		trace (TRACE_LEVEL_ERROR, "Cleaning the radius server configuration failed.");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}
	
	// Retrieve the Radius Hosts from Wave DB and push them on Linux FS
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RadiusHostManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);

	if (pResults == NULL) {
		trace (TRACE_LEVEL_ERROR, "presults is null");
		delete synchronousQueryContext;
		return;
	}

	for (UI32 i = 0; i < pResults->size(); i++)
	{
		pRadiusHostObj = dynamic_cast<RadiusHostManagedObject *> ((*pResults)[i]);
		if (pRadiusHostObj == NULL) {
			trace (TRACE_LEVEL_ERROR, "pRadiusHostObj is null");
			continue;
		}

		pradconf = (secRadConf_t*)calloc(1, sizeof (secRadConf_t));
		if (pradconf == NULL) {
			trace (TRACE_LEVEL_ERROR, "pradconf is null");
			delete pRadiusHostObj;
			delete pResults;
			delete synchronousQueryContext;
			return;
		}
	    pradconf->hostname = STRDUP((pRadiusHostObj->getHost()).c_str());
		pradconf->secret = STRDUP((pRadiusHostObj->getKey()).c_str());
		pradconf->timeout = pRadiusHostObj->getTimeout();
		pradconf->retries = pRadiusHostObj->getRetransmit();
		pradconf->port = pRadiusHostObj->getPort();
		pradconf->proto = pRadiusHostObj->getProtocol();

		ret = secRadiusAdd(pradconf);

		free(pradconf->hostname);
		free(pradconf->secret);
		free(pradconf);

		if (ret != 0) {
			trace (TRACE_LEVEL_FATAL, "Adding Radius host " + pRadiusHostObj->getHost() + string(" failed"));
			pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
			pWaveAsynchronousContextForBootPhases->callback ();
			delete pRadiusHostObj;
			delete pResults;
			delete synchronousQueryContext;
			return;
		} else {
			trace (TRACE_LEVEL_INFO, "Radius host " + pRadiusHostObj->getHost() + string(" is added successfully to the switch"));
		}
		delete pRadiusHostObj;
	}
	delete pResults;
	delete synchronousQueryContext;

	/* Push Tacacs Hosts */
	TacacsHostManagedObject *pTacacsHostObj = NULL;
	secTacacsConf_t *ptacconf = NULL;

	// Cleanup
	ret = secTacacsConfigClean();
	if (ret != 0) {
		trace (TRACE_LEVEL_ERROR, "Cleaning the tacacs+ server configuration failed.");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}

	// Retrieve the Tacacs+ Hosts from Wave DB and push them on Linux FS
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(TacacsHostManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);

	if (pResults == NULL) {
		trace (TRACE_LEVEL_ERROR, "presults is null");
		delete synchronousQueryContext;
		return;
	}

	for (UI32 i = 0; i < pResults->size(); i++)
	{
		pTacacsHostObj = dynamic_cast<TacacsHostManagedObject *> ((*pResults)[i]);
		if (pTacacsHostObj == NULL) {
			trace (TRACE_LEVEL_ERROR, "pTacacsHostObj is null");
			continue;
		}

		ptacconf = (secTacacsConf_t*)calloc(1, sizeof (secTacacsConf_t));
		if (ptacconf == NULL) {
			trace (TRACE_LEVEL_ERROR, "ptacconf is null");
			delete pTacacsHostObj;
			delete pResults;
			delete synchronousQueryContext;
			return;
		}
	    ptacconf->hostname = STRDUP((pTacacsHostObj->getHost()).c_str());
		ptacconf->secret = STRDUP((pTacacsHostObj->getKey()).c_str());
		ptacconf->timeout = pTacacsHostObj->getTimeout();
		ptacconf->retries = pTacacsHostObj->getRetries();
		ptacconf->port = pTacacsHostObj->getPort();
		ptacconf->proto = pTacacsHostObj->getProtocol();

		ret = secTacacsAdd(ptacconf);

		free(ptacconf->hostname);
		free(ptacconf->secret);
		free(ptacconf);

		if (ret != 0) {
			trace (TRACE_LEVEL_FATAL, "Adding Tacacs+ host " + pTacacsHostObj->getHost() + string(" failed"));
			pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
			pWaveAsynchronousContextForBootPhases->callback ();
			delete pTacacsHostObj;
			delete pResults;
			delete synchronousQueryContext;
			return;
		} else {
			trace (TRACE_LEVEL_INFO, "Tacacs+ host " + pTacacsHostObj->getHost() + string(" is added successfully to the switch"));
		}
		delete pTacacsHostObj;
	}
	delete pResults;
	delete synchronousQueryContext;

	/* Push LDAP Hosts */
	LDAPHostManagedObject *pLDAPHostObj = NULL;
	secLdapConf_t *pldapconf = NULL;

	// Cleanup
	ret = secLdapConfigClean();
	if (ret != 0) {
		trace (TRACE_LEVEL_ERROR, "Cleaning the LDAP server configuration failed.");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}

	// Retrieve the LDAP Hosts from Wave DB and push them on Linux FS
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(LDAPHostManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);

	if (pResults == NULL) {
		trace (TRACE_LEVEL_ERROR, "presults is null");
		delete synchronousQueryContext;
		return;
	}
	for (UI32 i = 0; i < pResults->size(); i++)
	{
		pLDAPHostObj = dynamic_cast<LDAPHostManagedObject *> ((*pResults)[i]);
		if (pLDAPHostObj == NULL) {
			trace (TRACE_LEVEL_ERROR, "pLDAPHostObj is null");
			continue;
		}

		pldapconf = (secLdapConf_t*)calloc(1, sizeof (secLdapConf_t));
		if (pldapconf == NULL) {
			trace (TRACE_LEVEL_ERROR, "pldapconf is null");
			delete pLDAPHostObj;
			delete pResults;
			delete synchronousQueryContext;
			return;
		}
	    pldapconf->hostname = STRDUP((pLDAPHostObj->getHost()).c_str());
		pldapconf->domain = STRDUP((pLDAPHostObj->getBasedn()).c_str());
		pldapconf->timeout = pLDAPHostObj->getTimeout();
		pldapconf->port = pLDAPHostObj->getPort();

		ret = secLdapAdd(pldapconf);
	
		free(pldapconf->hostname);
		free(pldapconf->domain);
		free(pldapconf);

		if (ret != 0) {
			trace (TRACE_LEVEL_FATAL, "Adding LDAP host " + pLDAPHostObj->getHost() + string(" failed"));
			pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
			pWaveAsynchronousContextForBootPhases->callback ();
			delete pResults;
			delete synchronousQueryContext;
			return;
		} else {
			trace (TRACE_LEVEL_INFO, "LDAP host " + pLDAPHostObj->getHost() + string(" is added successfully to the switch"));
		}
		delete pLDAPHostObj;
	}
	delete pResults;
	delete synchronousQueryContext;

	/* Push Authentication Login mode */
	ret = syncMapRoles();
	if (ret == 0)
	{
		trace (TRACE_LEVEL_INFO, "Changing login sync map roles is successful on the switch");
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Changing sync map roles failed on the switch");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}

	ret = syncExecAccounting();
	if (ret == 0)
	{
		trace (TRACE_LEVEL_INFO, "Changing login accounting is successful on the switch");
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Changing login accounting failed on the switch");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
	}

	ret = syncCmdAccounting();
	if (ret == 0)
	{
		trace (TRACE_LEVEL_INFO, "Changing Command accounting is successful on the switch");
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Changing Command accounting failed on the switch");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
	}
	/* Push Authentication Login mode */
	ret = syncAuthMode();
	if (ret == 0)
	{
		trace (TRACE_LEVEL_INFO, "Changing login auth spec is successful on the switch");
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Changing login auth spec failed on the switch");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}

	/* Push Password attributes */
	ret = syncPasswdOptions();
	if (ret == 0)
	{
		trace (TRACE_LEVEL_INFO, "Changing password-attributes is successful on the switch");
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Changing password-attributes failed on the switch");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}

	/* Push Banner */
	ret = syncBanner();
	if (ret == 0)
	{
		trace (TRACE_LEVEL_INFO, "Changing banner is successful on the switch");
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Changing banner failed on the switch");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}

	/* Push Banner */
	ret = syncIpAcls();
	if (ret == 0)
	{
		trace (TRACE_LEVEL_INFO, "Syncing IP Acls  is successful on the switch");
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Syncing IP Acls failed on the switch");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}

	ret = syncIp6Acls();
	if (ret == 0)
	{
		trace (TRACE_LEVEL_INFO, "Syncing IP6 Acls  is successful on the switch");
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Syncing IP6 Acls failed on the switch");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}

	/* Push Telnet server configuration */
	ret = syncTelnetServerConfig();
	if (ret == 0)
	{
		trace (TRACE_LEVEL_INFO, "Changing Telnet server configuration is successful on the switch or MO does not exist");
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Changing Telnet server configuration failed on the switch");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}
    
    /* Push SSH server configuration */
	ret = syncSSHServerConfig();
	if (ret == 0)
	{
		trace (TRACE_LEVEL_INFO, "Changing SSH server configuration is successful on the switch or MO does not exist");
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Changing SSH server configuration failed on the switch");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}
	
    /* Push SSH server list configuration */
	ret = syncSSHServerListConfig();
	if (ret == 0)
	{
		trace (TRACE_LEVEL_INFO, "Changing SSH server list configuration is successful on the switch");
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "Changing SSH server list configuration failed on the switch");
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (FRAMEWORK_ERROR);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;
	}

	} //if WAVE_BOOT_CLUSTER_JOIN

	pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
	pWaveAsynchronousContextForBootPhases->callback ();
}

}
