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

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "DcmResourceIdEnums.h"

#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/LocalDiscreteObjsWorker.h"
#include "AAA/Local/AAALocalSetPasswordAttributesMessage.h"
#include "AAA/Local/AAALocalSetBannerMessage.h"
#include "AAA/Local/AAALocalAddMapRoleMessage.h"
#include "AAA/Local/AAALocalChangeMapRoleMessage.h"
#include "AAA/Local/AAALocalDeleteMapRoleMessage.h"
#include "AAA/Local/CertutilOpActionMessage.h"
#include "AAA/Local/CertutilOpShowMessage.h"
#include "AAA/Local/SecServicesLocalConfigMessage.h"
#include "AAA/Local/SecServicesLocalShowMessage.h"
#include "AAA/Local/AAALocalTypes.h"
#include "AAA/Global/SecServicesGlobalManagedObject.h"
#include "AAA/Local/SecServicesLocalManagedObject.h"
#include "AAA/Local/ClearSessionsLocalMessage.h"

extern "C" {
	#include "security/public.h"
}

namespace DcmNs
{

LocalDiscreteObjsWorker::LocalDiscreteObjsWorker ( AAALocalLocalObjectManager *pAAALocalLocalObjectManager)
	: WaveWorker  (pAAALocalLocalObjectManager)
{
	TelnetServerLocalManagedObject telnetServerLocalMO (pAAALocalLocalObjectManager);
    telnetServerLocalMO.setupOrm ();
    addManagedClass (TelnetServerLocalManagedObject::getClassName (), this);

    SSHServerLocalManagedObject sshServerLocalMO (pAAALocalLocalObjectManager);
    sshServerLocalMO.setupOrm ();
    addManagedClass (SSHServerLocalManagedObject::getClassName (), this);

    SSHServerListLocalManagedObject sshServerListLocalMO (pAAALocalLocalObjectManager);
    sshServerListLocalMO.setupOrm ();
    addManagedClass (SSHServerListLocalManagedObject::getClassName (), this);

	addOperationMap (AAALOCALSETPASSWORDATTRIBUTES, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::AAALocalSetPasswordAttributesMessageHandler));
	addOperationMap (AAALOCALSETBANNER, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::AAALocalSetBannerMessageHandler));
	addOperationMap (AAALOCALADDMAPROLE, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::AAALocalAddMapRoleMessageHandler));
	addOperationMap (AAALOCALCHANGEMAPROLE, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::AAALocalChangeMapRoleMessageHandler));
	addOperationMap (AAALOCALDELETEMAPROLE, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::AAALocalDeleteMapRoleMessageHandler));
	addOperationMap (CERTUTILOPACTION, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::AAALocalCertutilOpActionMessageHandler));
	addOperationMap (CERTUTILOPSHOW, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::AAALocalCertutilOpShowMessageHandler));
	addOperationMap (TELNETSERVERLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::TelnetServerLocalConfigMessageHandler));
	addOperationMap (TELNETSERVERLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::TelnetServerLocalShowMessageHandler));
	addOperationMap (SSHSERVERLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::SSHServerLocalConfigMessageHandler));
	addOperationMap (SSHSERVERLISTLOCALADD, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::SSHServerListLocalAddMessageHandler));
	addOperationMap (SSHSERVERLISTLOCALDELETE, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::SSHServerListLocalDeleteMessageHandler));
	addOperationMap (SSHSERVERLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::SSHServerLocalShowMessageHandler));
	addOperationMap (SSHSERVERLISTLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::SSHServerListLocalShowMessageHandler));
	addOperationMap (CLEARSESSIONSLOCAL, reinterpret_cast<PrismMessageHandler> (&LocalDiscreteObjsWorker::ClearSessionsLocalMessageHandler));
}

LocalDiscreteObjsWorker::~LocalDiscreteObjsWorker ()
{
}

PrismMessage  *LocalDiscreteObjsWorker::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{
		case AAALOCALSETPASSWORDATTRIBUTES :
			pPrismMessage = new AAALocalSetPasswordAttributesMessage ();
			break;
		case AAALOCALSETBANNER :
			pPrismMessage = new AAALocalSetBannerMessage ();
			break;
		case AAALOCALADDMAPROLE :
			pPrismMessage = new AAALocalAddMapRoleMessage ();
			break;
		case AAALOCALCHANGEMAPROLE :
			pPrismMessage = new AAALocalChangeMapRoleMessage ();
			break;
		case AAALOCALDELETEMAPROLE :
			pPrismMessage = new AAALocalDeleteMapRoleMessage ();
			break;
		case CERTUTILOPACTION :
			pPrismMessage = new CertutilOpActionMessage ();
			break;
		case CERTUTILOPSHOW :
			pPrismMessage = new CertutilOpShowMessage ();
			break;
		case TELNETSERVERLOCALCONFIG :
			pPrismMessage = new TelnetServerLocalConfigMessage ();
			break;
		case TELNETSERVERLOCALSHOW :
			pPrismMessage = new TelnetServerLocalShowMessage ();
			break;
		case SSHSERVERLOCALCONFIG :
			pPrismMessage = new SSHServerLocalConfigMessage ();
			break;
		case SSHSERVERLISTLOCALADD :
			pPrismMessage = new SSHServerListLocalAddMessage ();
			break;
		case SSHSERVERLISTLOCALDELETE :
			pPrismMessage = new SSHServerListLocalDeleteMessage ();
			break;
		case SSHSERVERLOCALSHOW :
			pPrismMessage = new SSHServerLocalShowMessage ();
			break;
		case SSHSERVERLISTLOCALSHOW :
			pPrismMessage = new SSHServerListLocalShowMessage();
			break;
		case CLEARSESSIONSLOCAL :
			pPrismMessage = new ClearSessionsLocalMessage ();
			break;
		default :
			pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject  *LocalDiscreteObjsWorker::createManagedObjectInstance(const string &managedClassName)
{
	WaveManagedObject *pWaveManagedObject = NULL;
	if ((TelnetServerLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new TelnetServerLocalManagedObject(dynamic_cast<AAALocalLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((SSHServerLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new SSHServerLocalManagedObject(dynamic_cast<AAALocalLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((SSHServerListLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new SSHServerListLocalManagedObject(dynamic_cast<AAALocalLocalObjectManager *>(getPWaveObjectManager()));
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "LocalDiscreteObjsWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }

    return (pWaveManagedObject);
}

void  LocalDiscreteObjsWorker::AAALocalSetBannerMessageHandler( AAALocalSetBannerMessage *pAAALocalSetBannerMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalDiscreteObjsWorker::AAALocalSetBannerMessageHandler()");
	if (pAAALocalSetBannerMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;

        int ret = -1;
        UI8 bannerType = pAAALocalSetBannerMessage->getBannerType();

        if ( bannerType == LOGIN_BANNER)
        {
                ret = secBannerSet((pAAALocalSetBannerMessage->getBanner()).c_str(), MYSWITCH, LOGIN_BANNER);
        }
        else if ( bannerType == MOTD_BANNER)
        {
                ret = secBannerSet((pAAALocalSetBannerMessage->getBanner()).c_str(), MYSWITCH, MOTD_BANNER);
        }
        else if ( bannerType == INCOMING_BANNER)
        {
                ret = secBannerSet((pAAALocalSetBannerMessage->getBanner()).c_str(), MYSWITCH, INCOMING_BANNER);
        }

	/* No specific handlign required for Last config replay(hafailover) */
	if (ret != 0) {
		status = BANNER_SET_FS_ERROR;
	}

	pAAALocalSetBannerMessage->setCompletionStatus (status);
	reply (pAAALocalSetBannerMessage);
}

void  LocalDiscreteObjsWorker::AAALocalSetPasswordAttributesMessageHandler( AAALocalSetPasswordAttributesMessage *pAAALocalSetPasswordAttributesMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalDiscreteObjsWorker::AAALocalSetPasswordAttributesMessageHandler()");
	if (pAAALocalSetPasswordAttributesMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	secPwdCfg_t     passwd_cfg;

	/* Setting default to be compatible with FOS Security library */
	passwd_cfg.min_len = PW_CFG_NO_CHANGE;
	passwd_cfg.lower = PW_CFG_NO_CHANGE;
	passwd_cfg.upper = PW_CFG_NO_CHANGE;
	passwd_cfg.digit = PW_CFG_NO_CHANGE;
	passwd_cfg.punct = PW_CFG_NO_CHANGE;
	passwd_cfg.history = PW_CFG_NO_CHANGE;
	passwd_cfg.minpassage = PW_CFG_NO_CHANGE;
	passwd_cfg.maxpassage = PW_CFG_NO_CHANGE;
	passwd_cfg.warn = PW_CFG_NO_CHANGE;
	passwd_cfg.lockthresh = PW_CFG_NO_CHANGE;
	passwd_cfg.lockdur = PW_CFG_NO_CHANGE;
	passwd_cfg.adminlockout = PW_CFG_NO_CHANGE;
	passwd_cfg.repeat = PW_CFG_NO_CHANGE;
	passwd_cfg.sequence = PW_CFG_NO_CHANGE;
	passwd_cfg.status = PW_CFG_NO_CHANGE;

	/* Setting the object with the user input */
	passwd_cfg.min_len = pAAALocalSetPasswordAttributesMessage->getMinlength();
	passwd_cfg.lockthresh = pAAALocalSetPasswordAttributesMessage->getMaxretry();
	passwd_cfg.upper = pAAALocalSetPasswordAttributesMessage->getUppercase();
	passwd_cfg.lower = pAAALocalSetPasswordAttributesMessage->getLowercase();
	passwd_cfg.digit = pAAALocalSetPasswordAttributesMessage->getNumerics();
	passwd_cfg.punct = pAAALocalSetPasswordAttributesMessage->getSplchars();
	passwd_cfg.adminlockout = pAAALocalSetPasswordAttributesMessage->getAdminlockout();

	int ret = secPwdCfgSet(&passwd_cfg, NULL, 0);
	/* No specific check requreied for last config replay*/
	if (ret != 0) {
		status = PASSWORD_ATTRIBS_SET_FS_ERROR;
	}

	pAAALocalSetPasswordAttributesMessage->setCompletionStatus (status);
	reply (pAAALocalSetPasswordAttributesMessage);
}

void  LocalDiscreteObjsWorker::AAALocalAddMapRoleMessageHandler( AAALocalAddMapRoleMessage *pAAALocalAddMapRoleMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalDiscreteObjsWorker::AAALocalAddMapRoleMessageHandler()");
	if (pAAALocalAddMapRoleMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	int ret = -1;
	//string cmd = "ldapcfg --maprole " + pAAALocalAddMapRoleMessage->getGroup() + " " + pAAALocalAddMapRoleMessage->getRole();
	ret = secLdapMapRole((pAAALocalAddMapRoleMessage->getGroup()).c_str(), (pAAALocalAddMapRoleMessage->getRole()).c_str());
	if (ret) {
		if ( pAAALocalAddMapRoleMessage->getIsALastConfigReplay() == true &&
				-9 == ret) {
            //LDAPROLE_DUP == ret) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalAddMapRoleMessage getIsALastConfigReplay true");
            status = WAVE_MESSAGE_SUCCESS;
        } else {
			status = MAPROLE_ADD_FS_ERROR;
		}
		trace (TRACE_LEVEL_ERROR, string("Mapping AD groups to switch role(s) in backend FS failed.") + ret);
	} else {
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalAddMapRoleMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalAddMapRoleMessage getIsALastConfigReplay fakse");
			int retVal = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + retVal);
        }
#endif
		trace (TRACE_LEVEL_INFO, string("Mapping AD groups to switch role(s) in backend FS successful."));
	}
	pAAALocalAddMapRoleMessage->setCompletionStatus (status);
	reply (pAAALocalAddMapRoleMessage);
}

void  LocalDiscreteObjsWorker::AAALocalChangeMapRoleMessageHandler( AAALocalChangeMapRoleMessage *pAAALocalChangeMapRoleMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalDiscreteObjsWorker::AAALocalChangeMapRoleMessageHandler()");
	if (pAAALocalChangeMapRoleMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	int ret = -1;
	//string cmd = "ldapcfg --maprole " + pAAALocalChangeMapRoleMessage->getGroup() + " " + pAAALocalChangeMapRoleMessage->getRole();
	ret = secLdapMapRole((pAAALocalChangeMapRoleMessage->getGroup()).c_str(), (pAAALocalChangeMapRoleMessage->getRole()).c_str());
	if (ret) {
		status = MAPROLE_CHANGE_FS_ERROR;
		trace (TRACE_LEVEL_ERROR, string("Changing Mapped AD groups to switch role(s) in backend FS failed.") + ret);
	} else {
		trace (TRACE_LEVEL_INFO, string("Changing Mapped AD groups to switch role(s) in backend FS successful."));
	}
	pAAALocalChangeMapRoleMessage->setCompletionStatus (status);
	reply (pAAALocalChangeMapRoleMessage);
}

void  LocalDiscreteObjsWorker::AAALocalDeleteMapRoleMessageHandler( AAALocalDeleteMapRoleMessage *pAAALocalDeleteMapRoleMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalDiscreteObjsWorker::AAALocalDeleteMapRoleMessageHandler()");
	if (pAAALocalDeleteMapRoleMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	int ret = -1;
	ret = secLdapUnmapRole((pAAALocalDeleteMapRoleMessage->getGroup()).c_str());
	if (ret) {
		if ( pAAALocalDeleteMapRoleMessage->getIsALastConfigReplay() == true &&
				-14 == ret) {
				//LDAPROLE_NOT_FOUND == ret) {
			trace (TRACE_LEVEL_DEBUG, string(" pAAALocalDeleteMapRoleMessage getIsALastConfigReplay true"));
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_ERROR, string("Unmapping AD group from backend FS failed.") + ret);
			status = MAPROLE_DELETE_FS_ERROR;
		}
	}
	else
	{
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalDeleteMapRoleMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalDeleteMapRoleMessage getIsALastConfigReplay");
            int retVal = system ("/sbin/reboot -f"); 
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + retVal);
        }
#endif
		trace (TRACE_LEVEL_INFO, "Unmapping AD Group from backend FS is successful.");
	}

	pAAALocalDeleteMapRoleMessage->setCompletionStatus (status);
	reply (pAAALocalDeleteMapRoleMessage);
}

void  LocalDiscreteObjsWorker::AAALocalCertutilOpActionMessageHandler(CertutilOpActionMessage *pCertutilOpActionMessage)
{
	trace (TRACE_LEVEL_INFO, "LocalDiscreteObjsWorker::AAALocalCertutilOpActionMessageHandler()");
	string fosCmd = pCertutilOpActionMessage->getFosCmd();
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	char                  authfile[100];
	int ret = -1;

	if (pCertutilOpActionMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	if (CERT_LDAPCA_DELETE == pCertutilOpActionMessage->getCertType()) {
		if (access(LDAP_CERT_FILE, F_OK) != 0) {
			pCertutilOpActionMessage->setCompletionStatus (CERTUTIL_LDAPCA_NO_CERT);
			reply (pCertutilOpActionMessage);
			return;
		}
	}

	if (CERT_SYSLOGCA_DELETE == pCertutilOpActionMessage->getCertType()) {
		if (access(SYSLOGCA_CERT_FILE, F_OK) != 0) {
			pCertutilOpActionMessage->setCompletionStatus (CERTUTIL_SYSLOGCA_NO_CERT);
			reply (pCertutilOpActionMessage);
			return;
		}
	}

	if (CERT_SSHKEY_DELETE == pCertutilOpActionMessage->getCertType()) {
		int     firstIndex = -1, secondIndex = -1;
		firstIndex = fosCmd.find("-user");
		secondIndex = fosCmd.find(' ', firstIndex);
		string user = fosCmd.substr(secondIndex + 1);

		trace (TRACE_LEVEL_DEBUG,
            string("CERT_SSHKEY_DELETE user:") + string(user));

		snprintf(authfile, sizeof(authfile), "%s.%s", SSHKEY_AUTH_FILE, user.c_str());
		trace (TRACE_LEVEL_DEBUG, string("authfile: ") + string(authfile));
		if (access(authfile, F_OK) != 0) {
			pCertutilOpActionMessage->setCompletionStatus (CERTUTIL_SSHKEY_FILE_NOT_EXIST);
			reply (pCertutilOpActionMessage);
			return;
		}
	}

	if (fosCmd.size() == 0)
	{
		trace (TRACE_LEVEL_ERROR, "Empty command is arrived in the handler");
		//TODO: Need to have proper error messages
        switch (pCertutilOpActionMessage->getCertType())
        {
            case CERT_LDAPCA_IMPORT:
			    status = CERTUTIL_LDAPCA_IMPORT_INTERNAL_ERROR;
                break;
            case CERT_LDAPCA_DELETE:
			    status = CERTUTIL_LDAPCA_DELETE_INTERNAL_ERROR;
                break;
            case CERT_SYSLOGCA_IMPORT:
			    status = CERTUTIL_SYSLOGCA_IMPORT_INTERNAL_ERROR;
                break;
            case CERT_SYSLOGCA_DELETE:
			    status = CERTUTIL_SYSLOGCA_DELETE_INTERNAL_ERROR;
                break;
            case CERT_SSHKEY_IMPORT:
			    status = CERTUTIL_SSHKEY_IMPORT_INTERNAL_ERROR;
                break;
            case CERT_SSHKEY_DELETE:
			    status = CERTUTIL_SSHKEY_DELETE_INTERNAL_ERROR;
                break;
            case CERT_GENERATE_FCAPKEYCSR:
			    status = CERTUTIL_GENERATE_FCAPKEYCSR_INTERNAL_ERROR;
                break;
            case CERT_EXPORT_FCAP:
			    status = CERTUTIL_EXPORT_FCAP_INTERNAL_ERROR;
                break;
            case CERT_IMPORT_FCAP:
			    status = CERTUTIL_IMPORT_FCAP_INTERNAL_ERROR;
                break;
            case CERT_DELETE_FCAPALL:
			    status = CERTUTIL_DELETE_FCAPALL_INTERNAL_ERROR;
                break;
            case CERT_DELETE_FCAPSWCERT:
			    status = CERTUTIL_DELETE_FCAPSWCERT_INTERNAL_ERROR;
                break;
            case CERT_DELETE_FCAPCACERT:
			    status = CERTUTIL_DELETE_FCAPCACERT_INTERNAL_ERROR;
                break;
            default:
                status = CERTUTIL_UNKNOWN_COMMAND;
                break;
		}
		pCertutilOpActionMessage->setCompletionStatus (status);
		reply (pCertutilOpActionMessage);
		return;
	}

	ret = system(fosCmd.c_str());
	if (ret || (ret = WEXITSTATUS(ret))) {
		trace (TRACE_LEVEL_ERROR,
            string("certificate operation failed in the backend.") + ret);
		//TODO: Need to have proper error messages based on the return value
        switch (pCertutilOpActionMessage->getCertType())
        {
            case CERT_LDAPCA_IMPORT:
			status = CERTUTIL_LDAPCA_IMPORT_FAILURE;
                break;
            case CERT_LDAPCA_DELETE:
			status = CERTUTIL_LDAPCA_DELETE_FAILURE;
                break;
            case CERT_SYSLOGCA_IMPORT:
                if (2 == WEXITSTATUS(ret))
                    status = CERTUTIL_SYSLOGCA_IMPORT_CERT_EXIST;
                else if (3 == WEXITSTATUS(ret))
                    status = CERTUTIL_SYSLOGCA_IMPORT_INVALID_FILE;
                else 
			        status = CERTUTIL_SYSLOGCA_IMPORT_FAILURE;
                break;
            case CERT_SYSLOGCA_DELETE:
			    status = CERTUTIL_SYSLOGCA_DELETE_FAILURE;
                break;
            case CERT_SSHKEY_IMPORT:
                if (2 == WEXITSTATUS(ret)) {
                    status = CERTUTIL_SSHKEY_IMPORT_INVALID_FILE;
                } else if (3 == WEXITSTATUS(ret)) {
                    status = CERTUTIL_SSHKEY_IMPORT_DOWNLOAD_FAILURE;
                } else if (1 == WEXITSTATUS(ret)) {
                    status = CERTUTIL_SSHKEY_IMPORT_FAILURE;
                }
                break;
            case CERT_SSHKEY_DELETE:
                if (2 == WEXITSTATUS(ret)) {
                    status = CERTUTIL_SSHKEY_FILE_NOT_EXIST;
                } else if (1 == WEXITSTATUS(ret)) {
                    status = CERTUTIL_SSHKEY_DELETE_FAILURE;
                }
                break;
            case CERT_GENERATE_FCAPKEYCSR:
			    status = CERTUTIL_GENERATE_FCAPKEYCSR_FAILURE;
                break;
            case CERT_EXPORT_FCAP:
			    status = CERTUTIL_EXPORT_FCAP_FAILURE;
                break;
            case CERT_IMPORT_FCAP:
			    status = CERTUTIL_IMPORT_FCAP_FAILURE;
                break;
            case CERT_DELETE_FCAPALL:
			    status = CERTUTIL_DELETE_FCAPALL_FAILURE;
                break;
            case CERT_DELETE_FCAPSWCERT:
			    status = CERTUTIL_DELETE_FCAPSWCERT_FAILURE;
                break;
            case CERT_DELETE_FCAPCACERT:
			    status = CERTUTIL_DELETE_FCAPCACERT_FAILURE;
                break;
            default:
                status = CERTUTIL_UNKNOWN_COMMAND;
                break;
		}
	}
	else
	{
		trace (TRACE_LEVEL_INFO, "certificate operation from backend is successful.");
        switch (pCertutilOpActionMessage->getCertType())
        {
            case CERT_LDAPCA_IMPORT:
                // gets translated to correct status in global worker
			    status = CERTUTIL_LDAPCA_IMPORT_SUCCESSFUL;
			    // status = WAVE_MESSAGE_SUCCESS;
                break;
            case CERT_LDAPCA_DELETE:
                // gets translated to correct status in global worker
			    status = CERTUTIL_LDAPCA_DELETE_SUCCESSFUL;
			    // status = WAVE_MESSAGE_SUCCESS;
                break;
            case CERT_SYSLOGCA_IMPORT:
				status = CERTUTIL_SYSLOGCA_IMPORT_SUCCESSFUL;
			    //status = WAVE_MESSAGE_SUCCESS;
                break;
            case CERT_SYSLOGCA_DELETE:
				status = CERTUTIL_SYSLOGCA_DELETE_SUCCESSFUL;
			    // status = WAVE_MESSAGE_SUCCESS;
                break;
            case CERT_SSHKEY_IMPORT:
				status = CERTUTIL_SSHKEY_IMPORT_SUCCESSFUL;
			    // status = WAVE_MESSAGE_SUCCESS;
                break;
            case CERT_SSHKEY_DELETE:
				status = CERTUTIL_SSHKEY_DELETE_SUCCESSFUL;
			    // status = WAVE_MESSAGE_SUCCESS;
                break;
            case CERT_GENERATE_FCAPKEYCSR:
			    status = CERTUTIL_GENERATE_FCAPKEYCSR_SUCCESSFUL;
                break;
            case CERT_EXPORT_FCAP:
			    status = CERTUTIL_EXPORT_FCAP_SUCCESSFUL;
                break;
            case CERT_IMPORT_FCAP:
			    status = CERTUTIL_IMPORT_FCAP_SUCCESSFUL;
                break;
            case CERT_DELETE_FCAPALL:
			    status = CERTUTIL_DELETE_FCAPALL_SUCCESSFUL;
                break;
            case CERT_DELETE_FCAPSWCERT:
			    status = CERTUTIL_DELETE_FCAPSWCERT_SUCCESSFUL;
                break;
            case CERT_DELETE_FCAPCACERT:
			    status = CERTUTIL_DELETE_FCAPCACERT_SUCCESSFUL;
                break;
            default:
                status = CERTUTIL_UNKNOWN_COMMAND;
                break;
        }
	}

	pCertutilOpActionMessage->setCompletionStatus (status);
	reply (pCertutilOpActionMessage);
}

void  LocalDiscreteObjsWorker::AAALocalCertutilOpShowMessageHandler(CertutilOpShowMessage *pCertutilOpShowMessage)
{
	trace (TRACE_LEVEL_INFO, "LocalDiscreteObjsWorker::AAALocalCertutilOpShowMessageHandler()");
	string fosCmd = pCertutilOpShowMessage->getFosCmd();
	char authfile[100];
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	if (pCertutilOpShowMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	if (CERT_LDAPCA_SHOWCERT == pCertutilOpShowMessage->getCertType()) {
		if (access(LDAP_CERT_FILE, F_OK) != 0) {
			pCertutilOpShowMessage->setCompletionStatus (CERTUTIL_LDAPCA_NO_CERT);
			reply (pCertutilOpShowMessage);
			return;
		}
	}
	if (CERT_SYSLOGCA_SHOWCERT == pCertutilOpShowMessage->getCertType()) {
		if (access(SYSLOGCA_CERT_FILE, F_OK) != 0) {
			pCertutilOpShowMessage->setCompletionStatus (CERTUTIL_SYSLOGCA_NO_CERT);
			reply (pCertutilOpShowMessage);
			return;
		}
	}

	if (CERT_SSHKEY_SHOW == pCertutilOpShowMessage->getCertType()) {
		int     firstIndex = -1, secondIndex = -1;
		firstIndex = fosCmd.find("-user");
		secondIndex = fosCmd.find(' ', firstIndex);
		string user = fosCmd.substr(secondIndex + 1);

		trace (TRACE_LEVEL_DEBUG,
			string("CERT_SSHKEY_SHOW user:") + string(user));
		snprintf(authfile, sizeof(authfile), "%s.%s", SSHKEY_AUTH_FILE, user.c_str());
		trace (TRACE_LEVEL_DEBUG, string("authfile: ") + string(authfile));
		if (access(authfile, F_OK) != 0) {
			pCertutilOpShowMessage->setCompletionStatus (CERTUTIL_SSHKEY_FILE_NOT_EXIST);
			reply (pCertutilOpShowMessage);
			return;
		}
	}
	if (fosCmd.size() == 0)
	{
		trace (TRACE_LEVEL_ERROR, "Empty command is arrived in the handler");
		//TODO: Need to have proper error messages
        switch (pCertutilOpShowMessage->getCertType())
        {
            case CERT_LDAPCA_SHOWCERT:
			    status = CERTUTIL_LDAPCA_SHOW_INTERNAL_ERROR;
                break;
            case CERT_SYSLOGCA_SHOWCERT:
			    status = CERTUTIL_SYSLOGCA_SHOW_INTERNAL_ERROR;
                break;
            case CERT_SSHKEY_SHOW:
			    status = CERTUTIL_SYSLOGCA_SHOW_INTERNAL_ERROR;
                break;
            case CERT_SHOW_FCAPALL:
			    status = CERTUTIL_SHOW_FCAPALL_INTERNAL_ERROR;
                break;
            case CERT_SHOW_FCAPCACERT:
			    status = CERTUTIL_SHOW_FCAPCACERT_INTERNAL_ERROR;
                break;
            case CERT_SHOW_FCAPSWCERT:
			    status = CERTUTIL_SHOW_FCAPSWCERT_INTERNAL_ERROR;
                break;
            default:
                status = CERTUTIL_UNKNOWN_COMMAND;
                break;
		}
		pCertutilOpShowMessage->setCompletionStatus (status);
		reply (pCertutilOpShowMessage);
		return;
	}

    vector<string> output;
	SI32 ret = FrameworkToolKit::systemCommandOutput(fosCmd.c_str(), output);
	if (0 != ret) {
		trace (TRACE_LEVEL_ERROR,
            string("certificate operation failed in the backend.") + ret);
		//TODO: Need to have proper error messages based on the return value
        switch (pCertutilOpShowMessage->getCertType())
        {
            case CERT_LDAPCA_SHOWCERT:
			    status = CERTUTIL_LDAPCA_SHOW_FAILURE;
                break;
            case CERT_SYSLOGCA_SHOWCERT:
			    status = CERTUTIL_SYSLOGCA_SHOW_FAILURE;
                break;
            case CERT_SSHKEY_SHOW:
                if (2 == WEXITSTATUS(ret)) {
			        status = CERTUTIL_SSHKEY_FILE_NOT_EXIST;
                } else if (1 == WEXITSTATUS(ret)) {
			        status = CERTUTIL_SSHKEY_SHOW_FAILURE;
                }
                break;
            case CERT_SHOW_FCAPALL:
			    status = CERTUTIL_SHOW_FCAPALL_FAILURE;
                break;
            case CERT_SHOW_FCAPSWCERT:
			    status = CERTUTIL_SHOW_FCAPSWCERT_FAILURE;
                break;
            case CERT_SHOW_FCAPCACERT:
			    status = CERTUTIL_SHOW_FCAPCACERT_FAILURE;
                break;
            default:
                status = CERTUTIL_UNKNOWN_COMMAND;
                break;
		}
	}
	else
	{
		trace (TRACE_LEVEL_INFO, "show certificate operation from backend is successful.");
        switch (pCertutilOpShowMessage->getCertType())
        {
            case CERT_LDAPCA_SHOWCERT:
			    status = CERTUTIL_LDAPCA_SHOW_SUCCESSFUL;
                break;
            case CERT_SYSLOGCA_SHOWCERT:
			    status = CERTUTIL_SYSLOGCA_SHOW_SUCCESSFUL;
                break;
            case CERT_SSHKEY_SHOW:
			    status = CERTUTIL_SSHKEY_SHOW_SUCCESSFUL;
                break;
            case CERT_SHOW_FCAPALL:
			    status = CERTUTIL_SHOW_FCAPALL_SUCCESSFUL;
                break;
            case CERT_SHOW_FCAPSWCERT:
			    status = CERTUTIL_SHOW_FCAPSWCERT_SUCCESSFUL;
                break;
            case CERT_SHOW_FCAPCACERT:
			    status = CERTUTIL_SHOW_FCAPCACERT_SUCCESSFUL;
                break;
            default:
                status = CERTUTIL_UNKNOWN_COMMAND;
                break;
        }
    }

    // loop through output and add to msg response
    for (UI32 ii = 0; ii < output.size(); ii++)
    {
        pCertutilOpShowMessage->setShowOutput(output[ii]);
    }

	pCertutilOpShowMessage->setCompletionStatus (status);
	reply (pCertutilOpShowMessage);
}

void  LocalDiscreteObjsWorker::TelnetServerLocalConfigMessageHandler(TelnetServerLocalConfigMessage *pTelnetServerLocalConfigMessage)
{
    int ret = 0;
	bool disableFlag;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	trace (TRACE_LEVEL_INFO, "LocalDiscreteObjsWorker::TelnetServerLocalConfigMessageHandler Entered");

	if (pTelnetServerLocalConfigMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	disableFlag = pTelnetServerLocalConfigMessage->getDisableFlag();

    if (true == disableFlag) {
        ret = system("/usr/bin/killall -q in.telnetd; /fabos/libexec/inetdUpdate 0");
    } else {
        ret = system("/fabos/libexec/inetdUpdate 1");
    }

	if (0 != ret) {
		trace (TRACE_LEVEL_ERROR,
            string("Telnet server config operation failed in the backend.") + ret);
        status = TELNET_SERVER_CONFIG_ERROR;
	}
	else
	{
		trace (TRACE_LEVEL_INFO, "Telnet server operation from backend is successful.");
		TelnetServerLocalManagedObject *pTelnetServerLocalMO = NULL;
		/* Retrieve the Telnet Server Local Managed Object */
#if 0
    WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(TelnetServerLocalManagedObject::getClassName());
    vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
#endif
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, TelnetServerLocalManagedObject::getClassName());

		if ((NULL == pResults) || (pResults->size() != 1))    {
			trace (TRACE_LEVEL_INFO, "creating new entry in db");
			if ((pResults == NULL) || (pResults->size() == 0))
                trace (TRACE_LEVEL_INFO, "Zero Telnet server config instances found.");
	        else if (pResults->size() > 1)
                trace (TRACE_LEVEL_INFO, "More than one Telnet server config instances found.");

    	    startTransaction();
			pTelnetServerLocalMO =
				new TelnetServerLocalManagedObject(dynamic_cast<AAALocalLocalObjectManager *>(getPWaveObjectManager()));
			pTelnetServerLocalMO->setDisableFlag(pTelnetServerLocalConfigMessage->getDisableFlag());
			status = commitTransaction();
			delete  pTelnetServerLocalMO;

			if (pResults != NULL) {
                for (unsigned int j = 0; j < pResults->size(); j++)
                    delete (*pResults)[j];

                delete pResults;
            }
			if (FRAMEWORK_SUCCESS == status) {
				trace (TRACE_LEVEL_INFO, "Created Telnet MO");
				status = WAVE_MESSAGE_SUCCESS;
			}
			else
			{
				status = TELNET_SERVER_CONFIG_ERROR;
				trace (TRACE_LEVEL_ERROR, "Commiting Telnet server config failed. Framework error - "
						+ FrameworkToolKit::localize(status));
			}
	    } else if (1 == pResults->size()) {
			trace (TRACE_LEVEL_INFO, "setting telnet flag");

    	    pTelnetServerLocalMO = dynamic_cast<TelnetServerLocalManagedObject *> ((*pResults)[0]);

			startTransaction ();
			updateWaveManagedObject (pTelnetServerLocalMO);
			pTelnetServerLocalMO->setDisableFlag(pTelnetServerLocalConfigMessage->getDisableFlag());
			status = commitTransaction ();

			if (FRAMEWORK_SUCCESS == status) {
				status = WAVE_MESSAGE_SUCCESS;
	            trace (TRACE_LEVEL_INFO, "Commiting Telnet server to Wave database is successful.");
	        }
	        else
	        {
	            status = TELNET_SERVER_CONFIG_ERROR;
	            trace (TRACE_LEVEL_ERROR, "Commiting Telnet server config failed. Framework error - "
	                    + FrameworkToolKit::localize(status));
	        }
			if (pResults)
				delete pResults;
	    }
			
    }
	pTelnetServerLocalConfigMessage->setCompletionStatus (status);
	reply (pTelnetServerLocalConfigMessage);
}

void  LocalDiscreteObjsWorker::TelnetServerLocalShowMessageHandler(TelnetServerLocalShowMessage *pTelnetServerLocalShowMessage)
{
    int ret = 0;
	bool disableFlag = false;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	trace (TRACE_LEVEL_INFO, "LocalDiscreteObjsWorker::TelnetServerLocalShowMessageHandler Entered");

	if (pTelnetServerLocalShowMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

    ret = system("/bin/grep ^telnet /etc/inetd.conf");

    if (ret != 0)
        disableFlag = true;    

    pTelnetServerLocalShowMessage->setDisableFlag(disableFlag);

	pTelnetServerLocalShowMessage->setCompletionStatus (status);
	reply (pTelnetServerLocalShowMessage);
}

void  LocalDiscreteObjsWorker::SSHServerLocalConfigMessageHandler(SSHServerLocalConfigMessage *pSSHServerLocalConfigMessage)
{
    int ret = 0;
	bool disableFlag;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	trace (TRACE_LEVEL_INFO, "LocalDiscreteObjsWorker::SSHServerLocalConfigMessageHandler Entered");

	if (pSSHServerLocalConfigMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	disableFlag = pSSHServerLocalConfigMessage->getDisableFlag();

    if (true == disableFlag) {
        ret = system("/usr/bin/killall -q sshd");
    } else {
        ret = system("/usr/sbin/sshd");
    }

	if (0 != ret) {
		trace (TRACE_LEVEL_ERROR,
            string("SSH server operation failed in the backend.") + ret);
	}
	else
	{
		trace (TRACE_LEVEL_INFO, "SSH server operation from backend is successful.");
		SSHServerLocalManagedObject *pSSHServerLocalMO = NULL;
	    /* Retrieve the SSH Server Local Managed Object */
#if 0
	    WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(SSHServerLocalManagedObject::getClassName());
	    vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
#endif
	    LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
	    vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SSHServerLocalManagedObject::getClassName());
	    if ((NULL == pResults) || (pResults->size() != 1))    {
	        if ((pResults == NULL) || (pResults->size() == 0)) trace (TRACE_LEVEL_FATAL, "Zero ssh server config instances found.");
	        else if (pResults->size() > 1) trace (TRACE_LEVEL_FATAL, "More than one ssh server config instances found.");

			/* create MO only if ssh server is disabled */
			// if (true == disableFlag) {
			startTransaction();
			pSSHServerLocalMO =
				new SSHServerLocalManagedObject(dynamic_cast<AAALocalLocalObjectManager *>(getPWaveObjectManager()));
			pSSHServerLocalMO->setDisableFlag(pSSHServerLocalConfigMessage->getDisableFlag());
			status = commitTransaction();

			delete  pSSHServerLocalMO;
			/*} else {
				status = FRAMEWORK_SUCCESS;
			}*/
	        if (pResults != NULL) {
    	        for (unsigned int j = 0; j < pResults->size(); j++)
	                delete (*pResults)[j];
		 
	            delete pResults;
	       	}
			if (FRAMEWORK_SUCCESS == status) {
	        	trace (TRACE_LEVEL_INFO, "Created SSH MO");
				status = WAVE_MESSAGE_SUCCESS;
			}
            else
            {
                status = SSH_SERVER_CONFIG_ERROR;
                trace (TRACE_LEVEL_ERROR, "Commiting ssh server config failed. Framework error - "
                        + FrameworkToolKit::localize(status));
            }
	    }
		else if (1 == pResults->size()) {    
			trace (TRACE_LEVEL_INFO, "setting ssh flag");
			pSSHServerLocalMO = dynamic_cast<SSHServerLocalManagedObject *> ((*pResults)[0]);

			/* update MO only if telnet server is disabled */
			// if (true == disableFlag) {
			startTransaction ();
			updateWaveManagedObject (pSSHServerLocalMO);
			pSSHServerLocalMO->setDisableFlag(pSSHServerLocalConfigMessage->getDisableFlag());
			status = commitTransaction ();
			/*} else {// "no ssh server .." should remove the entry
				startTransaction ();
				delete pSSHServerLocalMO;
				status = commitTransaction ();
			}*/

			if (FRAMEWORK_SUCCESS == status) {
                status = WAVE_MESSAGE_SUCCESS;
                trace (TRACE_LEVEL_INFO, "Commiting ssh server to Wave database is successful.");
            }
            else
            {
                status = SSH_SERVER_CONFIG_ERROR;
				 trace (TRACE_LEVEL_ERROR, "Commiting SSH server config failed. Framework error - " + FrameworkToolKit::localize(status));
        	}
			if (pResults)
				delete pResults;
		}
#if 0
		trace (TRACE_LEVEL_INFO, "SSH server operation from backend is successful.");
		startTransaction ();
		updateWaveManagedObject (pSSHServerLocalMO);
		pSSHServerLocalMO->setDisableFlag(pSSHServerLocalConfigMessage->getDisableFlag());
		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
            status = WAVE_MESSAGE_SUCCESS;
            trace (TRACE_LEVEL_INFO, "Commiting SSH server to Wave database is successful.");
        }
        else
        {
            status = SSH_SERVER_CONFIG_ERROR;
            trace (TRACE_LEVEL_ERROR, "Commiting SSH server config failed. Framework error - " + FrameworkToolKit::localize(status));
		}
#endif
    }
	pSSHServerLocalConfigMessage->setCompletionStatus (status);
	reply (pSSHServerLocalConfigMessage);
}

void  LocalDiscreteObjsWorker::SSHServerListLocalAddMessageHandler(SSHServerListLocalAddMessage *pSSHServerListLocalAddMessage)
{
    int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	SSHServerListLocalManagedObject *pSSHServerListMO = NULL;

	trace (TRACE_LEVEL_DEBUG, "LocalDiscreteObjsWorker::SSHServerListLocalAddMessageHandler Entered");

	if (pSSHServerListLocalAddMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	/* Retrieve the SSH Server List Managed Object */
#if 0
    WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(SSHServerListLocalManagedObject::getClassName());
    vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
#endif
	LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
	vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SSHServerListLocalManagedObject::getClassName());

    if ((NULL == pResults) || (0 != pResults->size()))    {
        if (NULL == pResults)
            trace (TRACE_LEVEL_FATAL, "No DB Table for SSH Server List"); 
        else if (1 == pResults->size())
            trace (TRACE_LEVEL_FATAL, "SSH Server List is already present");
        if (NULL != pResults)
            delete pResults;
        pSSHServerListLocalAddMessage->setCompletionStatus (SSH_SERVER_LIST_ADD_ERROR);
        reply (pSSHServerListLocalAddMessage);
        return;
    }

    SSHLocalKeyExchangeProtocol protocol = pSSHServerListLocalAddMessage->getKeyExchangeProtocol();
	trace (TRACE_LEVEL_DEBUG, string("protocol: ") + protocol);

    if (SSH_KEX_DH_GROUP_14 == protocol) {
	    trace (TRACE_LEVEL_DEBUG, "Uncommenting KexAlgorithms in sshd_config");
        ret = system("/fabos/sbin/sshdconfigupdate KexAlgorithms 1");
    } else {
	    trace (TRACE_LEVEL_DEBUG, "No change in sshd_config");
    }

	if (0 != ret) {
		trace (TRACE_LEVEL_ERROR,
            string("SSH server operation failed in the backend.") + ret);
        status = WAVE_MESSAGE_ERROR;
	}
	else
	{
		trace (TRACE_LEVEL_DEBUG, "SSH server list operation from backend is successful.");
		startTransaction ();

        pSSHServerListMO = dynamic_cast<SSHServerListLocalManagedObject *>(createManagedObjectInstance(SSHServerListLocalManagedObject::getClassName()));

        pSSHServerListMO->setKeyExchangeProtocol(pSSHServerListLocalAddMessage->getKeyExchangeProtocol());
        
        status = commitTransaction ();
        if (FRAMEWORK_SUCCESS == status)
        {               status = WAVE_MESSAGE_SUCCESS;
            trace (TRACE_LEVEL_DEBUG, "Commiting SSH Server List MO to Wave database is successful.");
        }
        else
        {   
            status = SSH_SERVER_LIST_ADD_ERROR;
            trace (TRACE_LEVEL_ERROR, "Commiting SSH server list addd failed. Framework error - " + FrameworkToolKit::localize(status));
		}
    	if (pSSHServerListMO) {
			delete pSSHServerListMO;    
		}
    }

	delete pResults;
	pSSHServerListLocalAddMessage->setCompletionStatus (status);
	reply (pSSHServerListLocalAddMessage);
}

void  LocalDiscreteObjsWorker::SSHServerListLocalDeleteMessageHandler(SSHServerListLocalDeleteMessage *pSSHServerListLocalDeleteMessage)
{
    int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	trace (TRACE_LEVEL_DEBUG, "LocalDiscreteObjsWorker::SSHServerListLocalDeleteMessageHandler Entered");

	if (pSSHServerListLocalDeleteMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

#if 0
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
    vector<WaveManagedObject *> *pResults = NULL;

	/* Get the SSH Server List Managed Object instance */
    synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(SSHServerListLocalManagedObject::getClassName());
    //synchronousQueryContext->addAndAttribute (new AttributeEnum (protocol, "protocol"));
    pResults = querySynchronously (synchronousQueryContext);
#endif

	LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
	vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SSHServerListLocalManagedObject::getClassName());

    if (NULL == pResults) {
        trace (TRACE_LEVEL_FATAL, "No DB Table for SSH Server List");
        pSSHServerListLocalDeleteMessage->setCompletionStatus (SSH_SERVER_LIST_DELETE_ERROR);
        reply (pSSHServerListLocalDeleteMessage);
        return;
	}

	SSHServerListLocalManagedObject *pSSHServerListMO = dynamic_cast<SSHServerListLocalManagedObject *> ((*pResults)[0]);

    if (NULL == pSSHServerListMO) {
        trace (TRACE_LEVEL_FATAL, "SSH Server List MO is NULL");
        pSSHServerListLocalDeleteMessage->setCompletionStatus (SSH_SERVER_LIST_DELETE_ERROR);
        if (NULL != pResults) {
            delete pResults;
        }
    }

    SSHLocalKeyExchangeProtocol protocol = pSSHServerListLocalDeleteMessage->getKeyExchangeProtocol();
	trace (TRACE_LEVEL_DEBUG, string("protocol: ") + protocol);

    if (SSH_KEX_DH_GROUP_14 == protocol) {
	    trace (TRACE_LEVEL_DEBUG, "Commenting KexAlgorithms in sshd_config");
        ret = system("/fabos/sbin/sshdconfigupdate KexAlgorithms 0");
    } else {
	    trace (TRACE_LEVEL_DEBUG, "No change in sshd_config");
    }

	if (0 != ret) {
		trace (TRACE_LEVEL_ERROR,
            string("SSH server operation failed in the backend.") + ret);
        status = WAVE_MESSAGE_ERROR;
	}
	else
	{
		trace (TRACE_LEVEL_DEBUG, "SSH server list operation from backend is successful.");
		startTransaction ();

        // Remove record operation 
        delete pSSHServerListMO;

        status = commitTransaction ();
        if (FRAMEWORK_SUCCESS == status)
        {
            status = WAVE_MESSAGE_SUCCESS;
            trace (TRACE_LEVEL_DEBUG, "Commiting SSH Server List MO to Wave database is successful.");
        }
        else
        {
            status = SSH_SERVER_LIST_DELETE_ERROR;
            trace (TRACE_LEVEL_ERROR, "Commiting SSH server config failed. Framework error - " + FrameworkToolKit::localize(status));
		}
        
    }

	delete pResults;
	pSSHServerListLocalDeleteMessage->setCompletionStatus (status);
	reply (pSSHServerListLocalDeleteMessage);
}

void  LocalDiscreteObjsWorker::SSHServerLocalShowMessageHandler(SSHServerLocalShowMessage *pSSHServerLocalShowMessage)
{
    int ret = 0;
	bool disableFlag = false;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	trace (TRACE_LEVEL_INFO, "LocalDiscreteObjsWorker::SSHServerLocalShowMessageHandler Entered");

	if (pSSHServerLocalShowMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

    ret = system("/bin/ps -ef | /bin/grep /usr/sbin/sshd | /bin/grep -v /bin/grep");
	trace (TRACE_LEVEL_DEBUG, string("LocalDiscreteObjsWorker::SSHServerLocalShowMessageHandler: ret: ") + ret);

    if (ret != 0) {
        disableFlag = true;    
	    trace (TRACE_LEVEL_INFO, "LocalDiscreteObjsWorker::SSHServerLocalShowMessageHandler: disableFlag set to true");
    } else {
	    trace (TRACE_LEVEL_INFO, "LocalDiscreteObjsWorker::SSHServerLocalShowMessageHandler: disableFlag set to false");
    }

    pSSHServerLocalShowMessage->setDisableFlag(disableFlag);

	pSSHServerLocalShowMessage->setCompletionStatus (status);
	reply (pSSHServerLocalShowMessage);
}

void  LocalDiscreteObjsWorker::SSHServerListLocalShowMessageHandler(SSHServerListLocalShowMessage *pSSHServerListLocalShowMessage)
{
    SSHLocalKeyExchangeProtocol protocol = SSH_KEX_DEFAULT;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	trace (TRACE_LEVEL_DEBUG, "LocalDiscreteObjsWorker::SSHServerListLocalShowMessageHandler Entered");

	if (pSSHServerListLocalShowMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

#if 0
    WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
    vector<WaveManagedObject *> *pResults = NULL;

    /* Get the SSH Server List Managed Object instance */
    synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(SSHServerListLocalManagedObject::getClassName());
    pResults = querySynchronously (synchronousQueryContext);
#endif
	/* Get the node specific SSH Server List Managed Object instance */
	LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
	vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SSHServerListLocalManagedObject::getClassName());

    if ((NULL == pResults) || (0 == pResults->size())) {
        if (NULL == pResults) {
		    trace (TRACE_LEVEL_FATAL, "No DB Table for SSH Server List");
		    pSSHServerListLocalShowMessage->setCompletionStatus (SSH_SERVER_LIST_SHOW_ERROR);
        }
        else if (0 == pResults->size()) {
		    trace (TRACE_LEVEL_FATAL, "SSH Server List MO is not present");
            pSSHServerListLocalShowMessage->setKeyExchangeProtocol(protocol);
		    pSSHServerListLocalShowMessage->setCompletionStatus (status);
        }
		reply (pSSHServerListLocalShowMessage);
		return;
    }

    SSHServerListLocalManagedObject *pSSHServerListMO = dynamic_cast<SSHServerListLocalManagedObject *> ((*pResults)[0]);

    if (NULL == pSSHServerListMO) {
		trace (TRACE_LEVEL_FATAL, "SSH Server List MO is NULL");
        pSSHServerListLocalShowMessage->setCompletionStatus (SSH_SERVER_LIST_SHOW_ERROR);
        return;
    }

    if (1 == pResults->size()) {
        protocol = pSSHServerListMO->getKeyExchangeProtocol();
		trace (TRACE_LEVEL_FATAL, "protocol is SSH_KEX_DH_GROUP_14");
    } else {
		trace (TRACE_LEVEL_FATAL, "no protocol value");
    }
    
	trace (TRACE_LEVEL_DEBUG, string("protocol: ") + protocol);
    pSSHServerListLocalShowMessage->setKeyExchangeProtocol(protocol);

	pSSHServerListLocalShowMessage->setCompletionStatus (status);
    delete pSSHServerListMO;
	reply (pSSHServerListLocalShowMessage);
}

void  LocalDiscreteObjsWorker::ClearSessionsLocalMessageHandler(ClearSessionsLocalMessage *pClearSessionsLocalMessage)
{
    int ret = -1;
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEBUG, "LocalDiscreteObjsWorker::ClearSessionsLocalMessageHandler Entered");

    if (pClearSessionsLocalMessage == NULL)
    {
        trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
        prismAssert (false, __FILE__, __LINE__);
    }

    ret = system("/fabos/libexec/usrlogout");

    if (ret || (ret = WEXITSTATUS(ret))) {
        trace (TRACE_LEVEL_ERROR,
            string("Clear sessions operation failed in the backend.") + ret);
        status = CLEAR_SESSIONS_FAILED;
    } else {
        trace (TRACE_LEVEL_DEBUG, "Clear sesions operation from the backend is successful.");
        status = CLEAR_SESSIONS_SUCCESSFUL;
    }

    pClearSessionsLocalMessage->setCompletionStatus (status);
    reply (pClearSessionsLocalMessage);
}

}
