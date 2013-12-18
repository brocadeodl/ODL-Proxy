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

#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "DcmCore/DcmToolKit.h"

#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/GlobalAuthenticationWorker.h"
#include "AAA/Global/GlobalAuthorizationWorker.h"
#include "AAA/Global/GlobalServerMgmtWorker.h"
#include "AAA/Global/GlobalDiscreteObjsWorker.h"
#include "AAA/Global/GlobalSessionWorker.h"
//#include "AAA/Global/GlobalIpAclWorker.h"
#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Global/UserManagedObject.h"
#include "AAA/Global/RoleManagedObject.h"
#include "AAA/Global/RuleManagedObject.h"
#include "AAA/Global/CmdRuleManagedObject.h"
#include "AAA/Global/DataRuleManagedObject.h"
#include "AAA/Global/PasswordAttributesManagedObject.h"
#include "AAA/Global/BannerManagedObject.h"
#include "AAA/Global/AuthLoginModeManagedObject.h"
#include "AAA/Global/ExecAccountingManagedObject.h"
#include "AAA/Global/CmdAccountingManagedObject.h"
#include "AAA/Global/PasswordEncryptionManagedObject.h"
#include "AAA/Global/SecServicesGlobalManagedObject.h"
#include "AAA/Global/MapCommands.h"
#include "AAA/Local/AAALocalCleanCacheMessage.h"


#include "brocade-aaa.h"

extern "C" {
  #include "security/public.h"
}

using namespace WaveNs;

namespace DcmNs
{

#define USR_ACNT_PASS	"4/WzpaAcD4IXw"

AAAGlobalObjectManager::AAAGlobalObjectManager ()
	: WaveObjectManager  (getClassName ())
{
	m_pGlobalAuthenticationWorker = new GlobalAuthenticationWorker (this);
	prismAssert (NULL != m_pGlobalAuthenticationWorker, __FILE__, __LINE__);
	m_pGlobalAuthorizationWorker = new GlobalAuthorizationWorker (this);
	prismAssert (NULL != m_pGlobalAuthorizationWorker, __FILE__, __LINE__);
	m_pGlobalServerMgmtWorker = new GlobalServerMgmtWorker (this);
	prismAssert (NULL != m_pGlobalServerMgmtWorker, __FILE__, __LINE__);
	m_pGlobalDiscreteObjsWorker = new GlobalDiscreteObjsWorker (this);
	prismAssert (NULL != m_pGlobalDiscreteObjsWorker, __FILE__, __LINE__);
	m_pGlobalSessionWorker = new GlobalSessionWorker (this);
	prismAssert (NULL != m_pGlobalSessionWorker, __FILE__, __LINE__);
	//m_pGlobalIpAclWorker = new GlobalIpAclWorker (this);
	// prismAssert (NULL != m_pGlobalIpAclWorker, __FILE__, __LINE__);
	/* RBAC Initialization */
	MapCommands::setupCmdMaps();
}

AAAGlobalObjectManager::~AAAGlobalObjectManager ()
{
	delete m_pGlobalAuthenticationWorker;
	delete m_pGlobalAuthorizationWorker;
	delete m_pGlobalServerMgmtWorker;
	delete m_pGlobalDiscreteObjsWorker;
	delete m_pGlobalSessionWorker;
	// delete m_pGlobalIpAclWorker;
}

AAAGlobalObjectManager  *AAAGlobalObjectManager::getInstance()
{
	static AAAGlobalObjectManager *pAAAGlobalObjectManager = new AAAGlobalObjectManager ();

	WaveNs::prismAssert (NULL != pAAAGlobalObjectManager, __FILE__, __LINE__);

	return (pAAAGlobalObjectManager);
}

string  AAAGlobalObjectManager::getClassName()
{
	return ("AAA Global Service");
}

PrismServiceId  AAAGlobalObjectManager::getPrismServiceId()
{
	return ((getInstance ())->getServiceId ());
}

PrismMessage  *AAAGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{

		default :
			pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject  *AAAGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
{
	return NULL;
}

void AAAGlobalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
	trace (TRACE_LEVEL_INFO, "AAAGlobalObjectManager::install()");

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	ResourceId bootReason = pWaveAsynchronousContextForBootPhases->getBootReason();

	/* Node is out-of-the-box or is rebooting after net-install is performed */
	if ((bootReason == WAVE_BOOT_FIRST_TIME_BOOT) ||
		(bootReason == WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT))
	{
		trace(TRACE_LEVEL_INFO, string("AAA Global 'install' Service called with reason - ") + FrameworkToolKit::localize(bootReason));

		startTransaction ();

		/* Install default Roles and Users*/
		RoleManagedObject *pAdmRoleObj = new RoleManagedObject(this, ACCT_ADMIN_ROLE, ACCT_ADMIN_DESC, "", ACCT_ADMIN_GID);
		RoleManagedObject *pUsrRoleObj = new RoleManagedObject(this, ACCT_USER_ROLE, ACCT_USER_DESC, "", ACCT_USER_GID);

		UserManagedObject *pAdminObj = new UserManagedObject(this, ACCT_ADMIN_ROLE, USER_ACCT_DEFAULT_ENCRYPTED_PASSWORD, ENCRYPTION_HASH, pAdmRoleObj->getObjectId(), ACCT_ADMIN_DESC, true, USER_ACCT_DEFAULT_MD5_PASSWORD, ACCT_SYS_DEF_PW_UID, pAdmRoleObj->getGID(), ACCT_SYS_DEF_PW_ADMIN_SSH_DIR, ACCT_SYS_DEF_PW_ADMIN_DIR);
		UserManagedObject *pUserObj = new UserManagedObject(this, ACCT_USER_ROLE, USER_ACCT_DEFAULT_ENCRYPTED_PASSWORD, ENCRYPTION_HASH, pUsrRoleObj->getObjectId(), ACCT_USER_DESC, true, USER_ACCT_DEFAULT_MD5_PASSWORD, ACCT_SYS_DEF_PW_UID, pUsrRoleObj->getGID(), ACCT_SYS_DEF_PW_ADMIN_SSH_DIR, ACCT_SYS_DEF_PW_ADMIN_DIR);

		string admUsers = pAdminObj->getName() + ROLE_USERS_DELIMITER + string(ACCT_ROOT_ROLE);
		string usrUsers = pUserObj->getName() + ROLE_USERS_DELIMITER + pAdminObj->getName() + ROLE_USERS_DELIMITER + string(ACCT_ROOT_ROLE);

		pAdmRoleObj->setUsers(admUsers);
		pUsrRoleObj->setUsers(usrUsers);

		//Setting the Data and command rules for new role, user and admin.
 		DataRuleManagedObject *pDataRuleAdm = new DataRuleManagedObject(this, 1, "*", "*", "*", "admin", READ_WRITE_EXECUTE, ACCEPT);
 		CmdRuleManagedObject *pCmdRuleAdm = new CmdRuleManagedObject(this, 1, "*", "*", "admin", READ_EXECUTE, ACCEPT);
 		DataRuleManagedObject *pDataRuleShow = new DataRuleManagedObject(this, 2, "*", "*", "/show", "*", READ_WRITE_EXECUTE, ACCEPT);
		/* All these pre defined rules are important for RBAC. */
 		CmdRuleManagedObject *pCmdRuleAll = new CmdRuleManagedObject(this, MAX_RULES, "*", "*", "*", READ_EXECUTE, ACCEPT);
		map< int, vector <string> > cmdMapObject = MapCommands::getCommandRuleMap();
		map< int, vector <string> >::iterator mapIt;
		vector <string>::iterator vecIt;
		int index = MAX_RULES - 1;
		vector<CmdRuleManagedObject  *> pCmdRuleObjs;
		CmdRuleManagedObject *pCmdRuleOperCmds;
		for(mapIt = cmdMapObject.begin(); mapIt != cmdMapObject.end(); mapIt++) {
			for(vecIt = (mapIt->second).begin(); vecIt != (mapIt->second).end(); vecIt++) {
				pCmdRuleOperCmds = new CmdRuleManagedObject(this, index--, "*", *vecIt, "*", READ_EXECUTE, REJECT) ;
				pCmdRuleObjs.push_back(pCmdRuleOperCmds);
			}
	    }
		pCmdRuleOperCmds = new CmdRuleManagedObject(this, index--, "*", "l2ping", "*", READ_EXECUTE, REJECT) ;
		pCmdRuleObjs.push_back(pCmdRuleOperCmds);
		pCmdRuleOperCmds = new CmdRuleManagedObject(this, index--, "*", "l2traceroute", "*", READ_EXECUTE, REJECT) ;
		pCmdRuleObjs.push_back(pCmdRuleOperCmds);
		//copy, clear commands are not part of the CommandRuleMap. They are case statements.
		pCmdRuleOperCmds = new CmdRuleManagedObject(this, index--, "*", "copy", "*", READ_EXECUTE, REJECT) ;
		pCmdRuleObjs.push_back(pCmdRuleOperCmds);

		pCmdRuleOperCmds = new CmdRuleManagedObject(this, index--, "*", "clear", "*", READ_EXECUTE, REJECT) ;
		pCmdRuleObjs.push_back(pCmdRuleOperCmds);

		//"no protocol" and "no spanning-tree" are not part of CommandRuleMap. They are case statements. These 2 commands need to be separately handled.
		pCmdRuleOperCmds = new CmdRuleManagedObject(this, index--, "*", "no protocol", "*", READ_EXECUTE, REJECT) ;
		pCmdRuleObjs.push_back(pCmdRuleOperCmds);
		pCmdRuleOperCmds = new CmdRuleManagedObject(this, index--, "*", "no spanning-tree", "*", READ_EXECUTE, REJECT) ;
		pCmdRuleObjs.push_back(pCmdRuleOperCmds);
		pCmdRuleOperCmds = new CmdRuleManagedObject(this, index--, "*", "no instance", "*", READ_EXECUTE, REJECT) ;
		pCmdRuleObjs.push_back(pCmdRuleOperCmds);

		MapCommands::setupDefaultCmds();
		vector<string> defaultCmdList = MapCommands::getDefaultCmds();
		for(unsigned int k = 0; k < defaultCmdList.size(); k++) {
			CmdRuleManagedObject *pCmdRuleDefaultCmds = new CmdRuleManagedObject(this, index--, "*", defaultCmdList[k], "*", READ_EXECUTE, REJECT);
			pCmdRuleObjs.push_back(pCmdRuleDefaultCmds);
		}
		MapCommands::unsetDefaultCmds();
 		DataRuleManagedObject *pDataRuleReadOnly = new DataRuleManagedObject(this, index--, "*", "*", "*", "*", READ_ONLY, ACCEPT);
 		//CmdRuleManagedObject *pCmdRuleReadOnly = new CmdRuleManagedObject(this, index--, "*", "*", "*", READ_ONLY, ACCEPT);
 		CmdRuleManagedObject *pCmdRuleShow = new CmdRuleManagedObject(this, index--, "*", "show", "*", READ_EXECUTE, ACCEPT);
 		CmdRuleManagedObject *pCmdRuleShowRunning = new CmdRuleManagedObject(this, index--, "*", "show running", "*", READ_EXECUTE, ACCEPT);

		/* Install Authentication Login Mode singleton managed object */
		AuthLoginModeManagedObject *pAuthLoginModeObj = new AuthLoginModeManagedObject(this, LOCAL_DB, NO_AAA);

		/* Install Login and Cmd Accounting MO */
		ExecAccountingManagedObject *pExecAccountingMO = new ExecAccountingManagedObject(this, NO_SRV);
		CmdAccountingManagedObject *pCmdAccountingMO = new CmdAccountingManagedObject(this, NO_SRV);

		/* Install Password Properties singleton managed object */
		PasswordAttributesManagedObject *pPasswordAttributesObj = new PasswordAttributesManagedObject(this, PASSWD_MIN_LEN_DEFAULT, PASSWD_MAX_RETRY_DEFAULT, PASSWD_UPPER_DEFAULT, PASSWD_LOWER_DEFAULT, PASSWD_NUMERICS_DEFAULT, PASSWD_SPL_CHAR_DEFAULT, PASSWD_ADMIN_LOCKOUT_DEFAULT);

		/* Install Banner singleton managed object */
		BannerManagedObject *pBannerManagedObj = new BannerManagedObject(this, "", "", "");

#if 0
		/* Install Telnet Server singleton managed object */
		TelnetServerGlobalManagedObject *pTelnetServerGlobalMO = new TelnetServerGlobalManagedObject(this, false);

		/* Install SSH Server singleton managed object */
		SSHServerGlobalManagedObject *pSSHServerGlobalMO = new SSHServerGlobalManagedObject(this, false);
#endif
		/* Install Password-Encryption Service managed object */
		PasswordEncryptionManagedObject *pPasswordEncryptionObj = new PasswordEncryptionManagedObject(this, true);
		
		// Let's not store the Default Policy and rules, If we need to store them we should support port range option too.
#if 0	
		
		vector<ExtIpAclRuleManagedObject  *> pExtIpAclRuleObjs;
		ExtIpAclRuleManagedObject *pExtIpAclRule;
		pExtIpAclRule = new ExtIpAclRuleManagedObject (this, 1, IPF_ACCEPT, ANY_HOST, NULL, NULL, NULL, IPPROTO_TCP, NO_WILD_CARD, 0, 22);
		pExtIpAclRule = new ExtIpAclRuleManagedObject (this, 2, IPF_ACCEPT, ANY_HOST, NULL, NULL, NULL, IPPROTO_TCP, NO_WILD_CARD, 0, 22);
		pExtIpAclRule = new ExtIpAclRuleManagedObject (this, 3, IPF_ACCEPT, ANY_HOST, NULL, NULL, NULL, IPPROTO_TCP, NO_WILD_CARD, 0, 22);
		pExtIpAclRule = new ExtIpAclRuleManagedObject (this, 4, IPF_ACCEPT, ANY_HOST, NULL, NULL, NULL, IPPROTO_TCP, NO_WILD_CARD, 0, 22);
		pExtIpAclRule = new ExtIpAclRuleManagedObject (this, 5, IPF_ACCEPT, ANY_HOST, NULL, NULL, NULL, IPPROTO_TCP, NO_WILD_CARD, 0, 22);
		pExtIpAclRule = new ExtIpAclRuleManagedObject (this, 6, IPF_ACCEPT, ANY_HOST, NULL, NULL, NULL, IPPROTO_TCP, NO_WILD_CARD, 0, 22);
		pExtIpAclRule = new ExtIpAclRuleManagedObject (this, 7, IPF_ACCEPT, ANY_HOST, NULL, NULL, NULL, IPPROTO_TCP, NO_WILD_CARD, 0, 22);
		pExtIpAclRule = new ExtIpAclRuleManagedObject (this, 8, IPF_ACCEPT, ANY_HOST, NULL, NULL, NULL, IPPROTO_TCP, NO_WILD_CARD, 0, 22);
		pExtIpAclRule = new ExtIpAclRuleManagedObject (this, 9, IPF_ACCEPT, ANY_HOST, NULL, NULL, NULL, IPPROTO_TCP, NO_WILD_CARD, 0, 22);
		pExtIpAclRule = new ExtIpAclRuleManagedObject (this, 10, IPF_ACCEPT, ANY_HOST, NULL, NULL, NULL, IPPROTO_TCP, NO_WILD_CARD, 0, 22);
		pExtIpAclRule = new ExtIpAclRuleManagedObject (this, 11, IPF_ACCEPT, ANY_HOST, NULL, NULL, NULL, IPPROTO_TCP, NO_WILD_CARD, 0, 22);
		pExtIpAclRule = new ExtIpAclRuleManagedObject (this, 12, IPF_ACCEPT, ANY_HOST, NULL, NULL, NULL, IPPROTO_TCP, NO_WILD_CARD, 0, 22);
#endif	
		/* Commit to the Wave Database */
		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status) {
			trace (TRACE_LEVEL_INFO, "Installed AAA Managed Objects");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_FATAL, "Failed to install AAA Managed Objects");
			prismAssert (false, __FILE__, __LINE__);
		}
		delete pAdmRoleObj;
		delete pUsrRoleObj;
		for (UI32 j = 0; j < pCmdRuleObjs.size(); j++) {
			delete pCmdRuleObjs[j];
		}
		delete pDataRuleAdm;
		delete pCmdRuleAdm;
		delete pDataRuleShow;
		delete pCmdRuleAll;
		delete pDataRuleReadOnly;
		delete pCmdRuleShow;
		delete pCmdRuleShowRunning;
		delete pAdminObj;
		delete pUserObj;
		delete pAuthLoginModeObj;
		delete pExecAccountingMO;
		delete pCmdAccountingMO;
		delete pPasswordAttributesObj;
		delete pBannerManagedObj;
		delete pPasswordEncryptionObj;
#if 0
        delete pTelnetServerGlobalMO;
        delete pSSHServerGlobalMO;
#endif

		/*
		 * Don't need to install in the FS in case of FIRST_TIME_BOOT. Default
		 * configuration is part of the Firmware.
		 */
	}

	pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
	pWaveAsynchronousContextForBootPhases->callback ();
}

void AAAGlobalObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{

	listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIG_REPLAY_COMPLETED_EVENT, reinterpret_cast<PrismEventHandler> (&AAAGlobalObjectManager::configReplayCompletedEventHandler));

	pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
	pWaveAsynchronousContextForBootPhases->callback ();
}

void AAAGlobalObjectManager::configReplayCompletedEventHandler (const ConfigReplayCompletedEvent* &pEvent)
{
	trace (TRACE_LEVEL_INFO, "AAAGlobalObjectManager::configReplayCompletedEventHandler()");

	WaveManagedObjectSynchronousQueryContext *syncQueryCtx = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	bool isAdminPassDefault = false;
	bool isUserPassDefault = false;
	int defaultPasswdSync = NONE_HAS_DEFAULT_PASSWD;

	// Retrieve users from wave database
	syncQueryCtx = new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());
	syncQueryCtx->addOrAttribute (new AttributeString ("admin", "username"));
	syncQueryCtx->addOrAttribute (new AttributeString ("user", "username"));
	pResults = querySynchronously (syncQueryCtx);
	if (NULL != pResults) {
		for (UI32 i = 0; i < pResults->size(); i++)
		{
			UserManagedObject *pUser = (dynamic_cast<UserManagedObject *> ((*pResults)[i]));
			if (pUser->getName().compare("admin") == 0) {
				if (pUser->getPassword().compare(USER_ACCT_DEFAULT_ENCRYPTED_PASSWORD) == 0) {
					isAdminPassDefault = true;
				}
			} else if (pUser->getName().compare("user") == 0)  {
				if (pUser->getPassword().compare(USER_ACCT_DEFAULT_ENCRYPTED_PASSWORD) == 0) {
					isUserPassDefault = true;
				}
			}
			delete pUser;
		}
		delete pResults;
	} else {
		trace (TRACE_LEVEL_FATAL, "User does not exist");
	}
	delete syncQueryCtx;
	if (isUserPassDefault && isAdminPassDefault) {
		defaultPasswdSync = BOTH_HAS_DEFAULT_PASSWD;	
	} else if(isAdminPassDefault) {
		defaultPasswdSync = ADMIN_HAS_DEFAULT_PASSWD;	
	} else if(isUserPassDefault) {
		defaultPasswdSync = USER_HAS_DEFAULT_PASSWD;	
	}


	vector<string> aaaElementsPath;
	AAALocalCleanCacheMessage *pSetCacheMessage = new AAALocalCleanCacheMessage ("Config-Replay-Complete-Event", defaultPasswdSync, aaaElementsPath);
	WaveSendToClusterContext *pWaveSendToClusterContextCache = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&AAAGlobalObjectManager::configReplayCompletedCallback), NULL);
	pWaveSendToClusterContextCache->setPPrismMessageForPhase1(pSetCacheMessage);
	sendToWaveCluster (pWaveSendToClusterContextCache);
	reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void AAAGlobalObjectManager::configReplayCompletedCallback(WaveSendToClusterContext *pWaveSendToClusterContextCache)
{
	trace (TRACE_LEVEL_INFO, "GlobalAuthenticationWorker:configReplayCompletedCallback()");
	AAALocalCleanCacheMessage *pSetCacheMessage = (AAALocalCleanCacheMessage *)pWaveSendToClusterContextCache->getPPrismMessageForPhase1();
	string operation = pSetCacheMessage->getCaller();
	ResourceId status = pWaveSendToClusterContextCache->getCompletionStatus();

	if (status == WAVE_MESSAGE_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "Config Replay Completed successfully for " + pSetCacheMessage->getCaller());
	}
	else
	{
		vector<LocationId> cxLocations;
		FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			string errStr;
			status = CLUSTER_ERROR_1001;
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "AAA Clean Cache operation for " + operation + " failed - " + FrameworkToolKit::localize(status));
		}
	}
	trace (TRACE_LEVEL_INFO, "configReplayCompletedCallback():deleting...");
	
	delete pSetCacheMessage;
	delete pWaveSendToClusterContextCache;

	/* Set the Command Accounting Flag */
	setCmdAccFlag();
}

void AAAGlobalObjectManager::setCmdAccFlag()
{
	trace (TRACE_LEVEL_INFO, "AAAGlobalObjectManager:setCmdAccFlag()");

	vector<WaveManagedObject *> *pResults = NULL;
    bool isTacacsAccEnabled = false;

    WaveManagedObjectSynchronousQueryContext    synchronousQueryContext (CmdAccountingManagedObject::getClassName());
    pResults = querySynchronously (&synchronousQueryContext);
    if (((NULL != pResults) && (pResults->size() == 1))) {
        CmdAccountingManagedObject *pCmdAccountingMO = dynamic_cast<CmdAccountingManagedObject *> ((*pResults)[0]);
        if (NULL == pCmdAccountingMO) {
   	        isTacacsAccEnabled = false;
        } else if (TACACS_SRV == (SI8)pCmdAccountingMO->getAccSrvType()) {
            /* Cmd accounting MO is found and the type of server is tacacs+ */
            isTacacsAccEnabled = true;
        }
        if (pCmdAccountingMO) {
   	        delete pCmdAccountingMO;
           pCmdAccountingMO = NULL;
       }
    } else {
   	    /* Issue with DB */
        isTacacsAccEnabled = false;
    }
    if (isTacacsAccEnabled) {
        tracePrintf(TRACE_LEVEL_INFO, "AAAGlobalObjectManager::configReplayCompletedEventHandler flag for tacacs+ command accounting is enabled");
    }
    delete pResults;

}
}
