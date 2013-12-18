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
 *   Author : kghanta                                                      *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/LocalServerMgmtWorker.h"
#include "AAA/Local/AAALocalAddRadiusHostMessage.h"
#include "AAA/Local/AAALocalChangeRadiusHostMessage.h"
#include "AAA/Local/AAALocalDeleteRadiusHostMessage.h"
#include "AAA/Local/AAALocalAddTacacsHostMessage.h"
#include "AAA/Local/AAALocalChangeTacacsHostMessage.h"
#include "AAA/Local/AAALocalDeleteTacacsHostMessage.h"
#include "AAA/Local/AAALocalAddLDAPHostMessage.h"
#include "AAA/Local/AAALocalChangeLDAPHostMessage.h"
#include "AAA/Local/AAALocalDeleteLDAPHostMessage.h"
#include "AAA/Local/AAALocalChangeAuthLoginModeMessage.h"
#include "AAA/Local/AAALocalChangeAccountingMessage.h"
#include "AAA/Local/AAALocalTypes.h"
#include "AAA/Global/AAAGlobalTypes.h"
#include "ClientInterface/Dot1X/Dot1XRadiusMessage.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"

extern "C" {
  #include "security/public.h"
}

using namespace WaveNs;

namespace DcmNs
{

LocalServerMgmtWorker::LocalServerMgmtWorker ( AAALocalLocalObjectManager *pAAALocalLocalObjectManager)
	: WaveWorker  (pAAALocalLocalObjectManager)
{
	addOperationMap (AAALOCALADDRADIUSHOST, reinterpret_cast<PrismMessageHandler> (&LocalServerMgmtWorker::AAALocalAddRadiusHostMessageHandler));
	addOperationMap (AAALOCALCHANGERADIUSHOST, reinterpret_cast<PrismMessageHandler> (&LocalServerMgmtWorker::AAALocalChangeRadiusHostMessageHandler));
	addOperationMap (AAALOCALDELETERADIUSHOST, reinterpret_cast<PrismMessageHandler> (&LocalServerMgmtWorker::AAALocalDeleteRadiusHostMessageHandler));
	addOperationMap (AAALOCALADDTACACSHOST, reinterpret_cast<PrismMessageHandler> (&LocalServerMgmtWorker::AAALocalAddTacacsHostMessageHandler));
	addOperationMap (AAALOCALCHANGETACACSHOST, reinterpret_cast<PrismMessageHandler> (&LocalServerMgmtWorker::AAALocalChangeTacacsHostMessageHandler));
	addOperationMap (AAALOCALDELETETACACSHOST, reinterpret_cast<PrismMessageHandler> (&LocalServerMgmtWorker::AAALocalDeleteTacacsHostMessageHandler));
	addOperationMap (AAALOCALADDLDAPHOST, reinterpret_cast<PrismMessageHandler> (&LocalServerMgmtWorker::AAALocalAddLDAPHostMessageHandler));
	addOperationMap (AAALOCALCHANGELDAPHOST, reinterpret_cast<PrismMessageHandler> (&LocalServerMgmtWorker::AAALocalChangeLDAPHostMessageHandler));
	addOperationMap (AAALOCALDELETELDAPHOST, reinterpret_cast<PrismMessageHandler> (&LocalServerMgmtWorker::AAALocalDeleteLDAPHostMessageHandler));
	addOperationMap (AAALOCALCHANGEAUTHLOGINMODE, reinterpret_cast<PrismMessageHandler> (&LocalServerMgmtWorker::AAALocalChangeAuthLoginModeMessageHandler));
	addOperationMap (AAALOCALCHANGEACCOUNTING, reinterpret_cast<PrismMessageHandler> (&LocalServerMgmtWorker::AAALocalChangeAccountingMessageHandler));
}

LocalServerMgmtWorker::~LocalServerMgmtWorker ()
{
}

PrismMessage  *LocalServerMgmtWorker::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{
		case AAALOCALADDRADIUSHOST :
			pPrismMessage = new AAALocalAddRadiusHostMessage ();
			break;
		case AAALOCALCHANGERADIUSHOST :
			pPrismMessage = new AAALocalChangeRadiusHostMessage ();
			break;
		case AAALOCALDELETERADIUSHOST :
			pPrismMessage = new AAALocalDeleteRadiusHostMessage ();
			break;
		case AAALOCALADDTACACSHOST :
			pPrismMessage = new AAALocalAddTacacsHostMessage ();
			break;
		case AAALOCALCHANGETACACSHOST :
			pPrismMessage = new AAALocalChangeTacacsHostMessage ();
			break;
		case AAALOCALDELETETACACSHOST :
			pPrismMessage = new AAALocalDeleteTacacsHostMessage ();
			break;
		case AAALOCALADDLDAPHOST :
			pPrismMessage = new AAALocalAddLDAPHostMessage ();
			break;
		case AAALOCALCHANGELDAPHOST :
			pPrismMessage = new AAALocalChangeLDAPHostMessage ();
			break;
		case AAALOCALDELETELDAPHOST :
			pPrismMessage = new AAALocalDeleteLDAPHostMessage ();
			break;
		case AAALOCALCHANGEAUTHLOGINMODE :
			pPrismMessage = new AAALocalChangeAuthLoginModeMessage ();
			break;
		case AAALOCALCHANGEACCOUNTING :
            pPrismMessage = new AAALocalChangeAccountingMessage ();
            break;
		default :
			pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject  *LocalServerMgmtWorker::createManagedObjectInstance(const string &managedClassName)
{
	return NULL;
}

void  LocalServerMgmtWorker::AAALocalAddRadiusHostMessageHandler( AAALocalAddRadiusHostMessage *pAAALocalAddRadiusHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalServerMgmtWorker::AAALocalAddRadiusHostMessageHandler()");
	if (pAAALocalAddRadiusHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status;
	ResourceId dot1xstatus;

	secRadConf_t *pradconf = (secRadConf_t*)calloc(1, sizeof (secRadConf_t));

	pradconf->hostname = strdup((pAAALocalAddRadiusHostMessage->getHost()).c_str());
	pradconf->secret = strdup((pAAALocalAddRadiusHostMessage->getKey()).c_str());
	pradconf->timeout = pAAALocalAddRadiusHostMessage->getTimeout();
	pradconf->retries = pAAALocalAddRadiusHostMessage->getRetransmit();
	pradconf->port = pAAALocalAddRadiusHostMessage->getPort();
	pradconf->proto = pAAALocalAddRadiusHostMessage->getProtocol();

	int ret = secRadiusAdd(pradconf);

	if (ret == 0) {
#ifdef LAST_CMD_SYNC_TEST
	    if ((pAAALocalAddRadiusHostMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0) ) {
	        trace (TRACE_LEVEL_INFO, "pAAALocalAddRadiusHostMessage***NogetIsALastConfigReplay***");
	        int sysRet = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + sysRet);
	    }   
#endif
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "Radius host " + pAAALocalAddRadiusHostMessage->getHost() + string(" is added successfully to the switch"));
	} else {
		if ((pAAALocalAddRadiusHostMessage->getIsALastConfigReplay() == true) &&
					(RADIUS_CONFIG_DUP == ret)) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalAddRadiusHostMessage getIsALastConfigReplay true");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalAddRadiusHostMessage getIsALastConfigReplay false");
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Adding Radius host " + pAAALocalAddRadiusHostMessage->getHost() + string(" failed"));
		}
	}

	 	Dot1XRadiusMessage *m = new Dot1XRadiusMessage();

        m->setHost((pAAALocalAddRadiusHostMessage->getHost()).c_str());
        m->setKey((pAAALocalAddRadiusHostMessage->getKey()).c_str());
        m->setTimeout(pAAALocalAddRadiusHostMessage->getTimeout());
        m->setRetransmit(pAAALocalAddRadiusHostMessage->getRetransmit());
        m->setPort(pAAALocalAddRadiusHostMessage->getPort());
        m->setProtocol(pAAALocalAddRadiusHostMessage->getProtocol());
		m->setCmdCode(DOT1X_RADIUS_CONFIG_ADD);

        dot1xstatus = sendSynchronouslyToWaveClient ("dauthd", m);
		if (dot1xstatus == WAVE_MESSAGE_SUCCESS)
		{
			trace (TRACE_LEVEL_DEBUG, "Radius host " + pAAALocalAddRadiusHostMessage->getHost() + string(" is added successfully to the dot1x daemon"));
		} else {
			trace (TRACE_LEVEL_ERROR, "Adding Radius host " + pAAALocalAddRadiusHostMessage->getHost() + string(" failed"));
		}		
		delete m;
 
	free(pradconf->hostname);
	free(pradconf->secret);
	free(pradconf);

	pAAALocalAddRadiusHostMessage->setCompletionStatus (status);
	reply (pAAALocalAddRadiusHostMessage);
}

void  LocalServerMgmtWorker::AAALocalChangeRadiusHostMessageHandler( AAALocalChangeRadiusHostMessage *pAAALocalChangeRadiusHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalServerMgmtWorker::AAALocalChangeRadiusHostMessageHandler()");
	if (pAAALocalChangeRadiusHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status;
	ResourceId dot1xstatus;
	secRadConf_t *pradconf = (secRadConf_t*)calloc(1, sizeof (secRadConf_t));
	pradconf->hostname = strdup((pAAALocalChangeRadiusHostMessage->getHost()).c_str());
	pradconf->secret = strdup((pAAALocalChangeRadiusHostMessage->getKey()).c_str());
	pradconf->timeout = pAAALocalChangeRadiusHostMessage->getTimeout();
	pradconf->retries = pAAALocalChangeRadiusHostMessage->getRetransmit();
	pradconf->port = pAAALocalChangeRadiusHostMessage->getPort();
	pradconf->proto = pAAALocalChangeRadiusHostMessage->getProtocol();

	int ret = secRadiusChange(pradconf);
	if (ret == 0) {
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalChangeRadiusHostMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalChangeRadiusHostMessage getIsALastConfigReplay false");
            int sysRet = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + sysRet);
        }
#endif
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "Changing radius host " + pAAALocalChangeRadiusHostMessage->getHost() + string(" settings is successfully on the switch"));
	} else {
		if ((pAAALocalChangeRadiusHostMessage->getIsALastConfigReplay() == true) &&
				(RADIUS_CONFIG_NOT_CHANGED == ret)) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalChangeRadiusHostMessage getIsALastConfigReplay ture");
            status = WAVE_MESSAGE_SUCCESS;
        } else {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalChangeRadiusHostMessage getIsALastConfigReplay false");
			status = WAVE_MESSAGE_ERROR;
		}
		trace (TRACE_LEVEL_ERROR, "Changing radius host " + pAAALocalChangeRadiusHostMessage->getHost() + string(" failed"));
	}


	 	Dot1XRadiusMessage *m = new Dot1XRadiusMessage();

        m->setHost((pAAALocalChangeRadiusHostMessage->getHost()).c_str());
        m->setKey((pAAALocalChangeRadiusHostMessage->getKey()).c_str());
        m->setTimeout(pAAALocalChangeRadiusHostMessage->getTimeout());
        m->setRetransmit(pAAALocalChangeRadiusHostMessage->getRetransmit());
        m->setPort(pAAALocalChangeRadiusHostMessage->getPort());
        m->setProtocol(pAAALocalChangeRadiusHostMessage->getProtocol());
		m->setCmdCode(DOT1X_RADIUS_CONFIG_CHNG);

        dot1xstatus = sendSynchronouslyToWaveClient ("dauthd", m);
		if (dot1xstatus == WAVE_MESSAGE_SUCCESS)
		{
			trace (TRACE_LEVEL_DEBUG, "Radius host " + pAAALocalChangeRadiusHostMessage->getHost() + string(" is changed successfully to the dot1x daemon"));
		} else {
			trace (TRACE_LEVEL_ERROR, "Changing Radius host " + pAAALocalChangeRadiusHostMessage->getHost() + string(" failed"));
		}		
		delete m;
 
	free(pradconf->hostname);
	free(pradconf->secret);
	free(pradconf);

	pAAALocalChangeRadiusHostMessage->setCompletionStatus (status);
	reply (pAAALocalChangeRadiusHostMessage);
}

void  LocalServerMgmtWorker::AAALocalDeleteRadiusHostMessageHandler( AAALocalDeleteRadiusHostMessage *pAAALocalDeleteRadiusHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalServerMgmtWorker::AAALocalDeleteRadiusHostMessageHandler()");
	if (pAAALocalDeleteRadiusHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status;
	ResourceId dot1xstatus;
	secRadConf_t *pradconf = (secRadConf_t*)calloc(1, sizeof (secRadConf_t));
	pradconf->hostname = strdup((pAAALocalDeleteRadiusHostMessage->getHost()).c_str());

	int ret = secRadiusRemove(pradconf);
	if (ret == 0) {
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalDeleteRadiusHostMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalRemoveRadiusHostMessage getIsALastConfigReplay false");
            int sysRet = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + sysRet);
        }
#endif
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "Deleting radius host " + pAAALocalDeleteRadiusHostMessage->getHost() + string(" is successfully on the switch"));
	} else {
		if ((pAAALocalDeleteRadiusHostMessage->getIsALastConfigReplay() == true) &&
				(RADIUS_CONFIG_NOT_FOUND == ret)) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalRemoveRadiusHostMessage getIsALastConfigReplay true");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalRemoveRadiusHostMessage getIsALastConfigReplay false");
			status = WAVE_MESSAGE_ERROR;
		}
		trace (TRACE_LEVEL_ERROR, "Deleting radius host " + pAAALocalDeleteRadiusHostMessage->getHost() + string(" failed"));
	}

	 	Dot1XRadiusMessage *m = new Dot1XRadiusMessage();

        m->setHost((pAAALocalDeleteRadiusHostMessage->getHost()).c_str());
		m->setCmdCode(DOT1X_RADIUS_CONFIG_DEL);

        dot1xstatus = sendSynchronouslyToWaveClient ("dauthd", m);
		if (dot1xstatus == WAVE_MESSAGE_SUCCESS)
		{
			trace (TRACE_LEVEL_DEBUG, "Radius host " + pAAALocalDeleteRadiusHostMessage->getHost() + string(" is added successfully to the dot1x daemon"));
		} else {
			trace (TRACE_LEVEL_ERROR, "Adding Radius host " + pAAALocalDeleteRadiusHostMessage->getHost() + string(" failed"));
		}		
		delete m;
 
	free(pradconf->hostname);
	free(pradconf);

	pAAALocalDeleteRadiusHostMessage->setCompletionStatus (status);
	reply (pAAALocalDeleteRadiusHostMessage);
}

void  LocalServerMgmtWorker::AAALocalAddTacacsHostMessageHandler( AAALocalAddTacacsHostMessage *pAAALocalAddTacacsHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalServerMgmtWorker::AAALocalAddTacacsHostMessageHandler()");
	if (pAAALocalAddTacacsHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	secTacacsConf_t *ptacconf = (secTacacsConf_t*)calloc(1, sizeof (secTacacsConf_t));

	ptacconf->hostname = strdup((pAAALocalAddTacacsHostMessage->getHost()).c_str());
	ptacconf->secret = strdup((pAAALocalAddTacacsHostMessage->getKey()).c_str());
	ptacconf->timeout = pAAALocalAddTacacsHostMessage->getTimeout();
	ptacconf->retries = pAAALocalAddTacacsHostMessage->getRetries();
	ptacconf->port = pAAALocalAddTacacsHostMessage->getPort();
	ptacconf->proto = pAAALocalAddTacacsHostMessage->getProtocol();

	int ret = secTacacsAdd(ptacconf);
	if (ret == 0) {
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalAddTacacsHostMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
	        trace (TRACE_LEVEL_DEBUG, "pAAALocalAddTacacsHostMessag getIsALastConfigReplay false");
            int sysRet = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + sysRet);
        }
#endif
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "Tacacs host " + pAAALocalAddTacacsHostMessage->getHost() + string(" is added successfully to the switch"));
	} else {
		if ((pAAALocalAddTacacsHostMessage->getIsALastConfigReplay() == true) &&
				(TACACS_CONFIG_DUP == ret)) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalAddTacacsHostMessage getIsALastConfigReplay true");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalAddTacacsHostMessage getIsALastConfigReplay false");
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Adding Tacacs host " + pAAALocalAddTacacsHostMessage->getHost() + string(" failed"));
		}
	}

	free(ptacconf->hostname);
	free(ptacconf->secret);
	free(ptacconf);

	pAAALocalAddTacacsHostMessage->setCompletionStatus (status);
	reply (pAAALocalAddTacacsHostMessage);
}

void  LocalServerMgmtWorker::AAALocalChangeTacacsHostMessageHandler( AAALocalChangeTacacsHostMessage *pAAALocalChangeTacacsHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalServerMgmtWorker::AAALocalChangeTacacsHostMessageHandler()");
	if (pAAALocalChangeTacacsHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	secTacacsConf_t *ptacconf = (secTacacsConf_t*)calloc(1, sizeof (secTacacsConf_t));
	ptacconf->hostname = strdup((pAAALocalChangeTacacsHostMessage->getHost()).c_str());
	ptacconf->secret = strdup((pAAALocalChangeTacacsHostMessage->getKey()).c_str());
	ptacconf->timeout = pAAALocalChangeTacacsHostMessage->getTimeout();
	ptacconf->retries = pAAALocalChangeTacacsHostMessage->getRetries();
	ptacconf->port = pAAALocalChangeTacacsHostMessage->getPort();
	ptacconf->proto = pAAALocalChangeTacacsHostMessage->getProtocol();

	int ret = secTacacsChange(ptacconf);
	if (ret == 0) {
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalChangeTacacsHostMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalChangeTacacsHostMessage getIsALastConfigReplay falise");
           	int sysRet = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + sysRet);
        }
#endif
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "Changing Tacacs host " + pAAALocalChangeTacacsHostMessage->getHost() + string(" settings is successfully on the switch"));
	} else {
		if ((pAAALocalChangeTacacsHostMessage->getIsALastConfigReplay() == true) &&
					(TACACS_CONFIG_NOT_CHANGED == ret)) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalChangeTacacsHostMessage getIsALastConfigReplay true");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalChangeTacacsHostMessage getIsALastConfigReplay false");
			status = WAVE_MESSAGE_ERROR;
		
			trace (TRACE_LEVEL_ERROR, "Changing Tacacs host " + pAAALocalChangeTacacsHostMessage->getHost() + string(" failed"));
		}
	}

	free(ptacconf->hostname);
	free(ptacconf->secret);
	free(ptacconf);

	pAAALocalChangeTacacsHostMessage->setCompletionStatus (status);
	reply (pAAALocalChangeTacacsHostMessage);
}

void  LocalServerMgmtWorker::AAALocalDeleteTacacsHostMessageHandler( AAALocalDeleteTacacsHostMessage *pAAALocalDeleteTacacsHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalServerMgmtWorker::AAALocalDeleteTacacsHostMessageHandler()");
	if (pAAALocalDeleteTacacsHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	secTacacsConf_t *ptacconf = (secTacacsConf_t*)calloc(1, sizeof (secTacacsConf_t));
	ptacconf->hostname = strdup((pAAALocalDeleteTacacsHostMessage->getHost()).c_str());

	int ret = secTacacsRemove(ptacconf);
	if (ret == 0) {
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalDeleteTacacsHostMessage->getIsALastConfigReplay() ==false) &&
					(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalDeleteTacacsHostMessage getIsALastConfigReplay false");
			int sysRet = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + sysRet);
        }
#endif
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "Deleting Tacacs host " + pAAALocalDeleteTacacsHostMessage->getHost() + string(" is successfully on the switch"));
	} else {
		if ((pAAALocalDeleteTacacsHostMessage->getIsALastConfigReplay() == true) &&
				(TACACS_CONFIG_NOT_FOUND == ret)) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalDeleteTacacsHostMessage getIsALastConfigReplay true");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalDeleteTacacsHostMessage getIsALastConfigReplay false");
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Deleting Tacacs host " + pAAALocalDeleteTacacsHostMessage->getHost() + string(" failed"));
		}
	}

	free(ptacconf->hostname);
	free(ptacconf);

	pAAALocalDeleteTacacsHostMessage->setCompletionStatus (status);
	reply (pAAALocalDeleteTacacsHostMessage);
}

void  LocalServerMgmtWorker::AAALocalAddLDAPHostMessageHandler( AAALocalAddLDAPHostMessage *pAAALocalAddLDAPHostMessage)
{
	trace (TRACE_LEVEL_INFO, "LocalServerMgmtWorker::AAALocalAddLDAPHostMessageHandler()");
	if (pAAALocalAddLDAPHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status;

	secLdapConf_t *pldapconf = (secLdapConf_t*)calloc(1, sizeof (secLdapConf_t));

	pldapconf->hostname = strdup((pAAALocalAddLDAPHostMessage->getHost()).c_str());
	pldapconf->domain = strdup((pAAALocalAddLDAPHostMessage->getBasedn()).c_str());
	pldapconf->timeout = pAAALocalAddLDAPHostMessage->getTimeout();
	//pldapconf->retries = pAAALocalAddLDAPHostMessage->getRetries();
	pldapconf->port = pAAALocalAddLDAPHostMessage->getPort();
	trace (TRACE_LEVEL_INFO, "LocalServerMgmtWorker::AAALocalAddLDAPHostMessageHandler(): Before sending to secLDAP");
	int ret = secLdapAdd(pldapconf);
	if (ret == 0) {
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalAddLDAPHostMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalAddLdapHostMessage getIsALastConfigReplay false");
            int sysRet = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + sysRet);
        }
#endif
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_INFO, "LDAP host " + pAAALocalAddLDAPHostMessage->getHost() + string(" is added successfully to the switch"));
	} else {
		if ((pAAALocalAddLDAPHostMessage->getIsALastConfigReplay() == true) &&
				(LDAP_CONFIG_DUP == ret)) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalAddLdapHostMessage getIsALastConfigReplay true");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalAddLdapHostMessage getIsALastConfigReplay falise");
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Adding LDAP host " + pAAALocalAddLDAPHostMessage->getHost() + string(" failed"));
		}
	}

	trace (TRACE_LEVEL_INFO, "Before freeing");
	free(pldapconf->hostname);
	free(pldapconf->domain);
	free(pldapconf);

	trace (TRACE_LEVEL_INFO, "After freeing");
	pAAALocalAddLDAPHostMessage->setCompletionStatus (status);
	reply (pAAALocalAddLDAPHostMessage);
}

void  LocalServerMgmtWorker::AAALocalChangeLDAPHostMessageHandler( AAALocalChangeLDAPHostMessage *pAAALocalChangeLDAPHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalServerMgmtWorker::AAALocalChangeLDAPHostMessageHandler()");
	if (pAAALocalChangeLDAPHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status;
	secLdapConf_t *pldapconf = (secLdapConf_t*)calloc(1, sizeof (secLdapConf_t));
	pldapconf->hostname = strdup((pAAALocalChangeLDAPHostMessage->getHost()).c_str());
	pldapconf->domain = strdup((pAAALocalChangeLDAPHostMessage->getBasedn()).c_str());
	pldapconf->timeout = pAAALocalChangeLDAPHostMessage->getTimeout();
	//pldapconf->retries = pAAALocalChangeLDAPHostMessage->getRetransmit();
	pldapconf->port = pAAALocalChangeLDAPHostMessage->getPort();

	int ret = secLdapChange(pldapconf);
	if (ret == 0) {
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalChangeLDAPHostMessage->getIsALastConfigReplay() ==false) &&
				(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalChangedapHostMessage getIsALastConfigRepla false*");
            int sysRet = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + sysRet);
        }
#endif
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "Changing ldap host " + pAAALocalChangeLDAPHostMessage->getHost() + string(" settings is successfully on the switch"));
	} else {
		if ((pAAALocalChangeLDAPHostMessage->getIsALastConfigReplay() == true) &&
				(LDAP_CONFIG_NOT_CHANGED == ret)) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalChangedapHostMessage getIsALastConfigReplay true");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalChangedapHostMessage getIsALastConfigReplay false");
			status = WAVE_MESSAGE_ERROR;
		}
		trace (TRACE_LEVEL_ERROR, "Changing ldap host " + pAAALocalChangeLDAPHostMessage->getHost() + string(" failed"));
	}

	free(pldapconf->hostname);
	free(pldapconf->domain);
	free(pldapconf);

	pAAALocalChangeLDAPHostMessage->setCompletionStatus (status);
	reply (pAAALocalChangeLDAPHostMessage);
}

void  LocalServerMgmtWorker::AAALocalDeleteLDAPHostMessageHandler( AAALocalDeleteLDAPHostMessage *pAAALocalDeleteLDAPHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalServerMgmtWorker::AAALocalDeleteLDAPHostMessageHandler()");
	if (pAAALocalDeleteLDAPHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	ResourceId status;
	secLdapConf_t *pldapconf = (secLdapConf_t*)calloc(1, sizeof (secLdapConf_t));
	pldapconf->hostname = strdup((pAAALocalDeleteLDAPHostMessage->getHost()).c_str());

	int ret = secLdapRemove(pldapconf);
	if (ret == 0) {
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalDeleteLDAPHostMessage->getIsALastConfigReplay() ==false) &&
					(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalDeleteLdapHostMessage getIsALastConfigReplay false");
            int sysRet = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + sysRet);
        }
#endif
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "Deleting ldap host " + pAAALocalDeleteLDAPHostMessage->getHost() + string(" is successfully on the switch"));
	} else {
		if ((pAAALocalDeleteLDAPHostMessage->getIsALastConfigReplay() == true) &&
				(LDAP_CONFIG_NOT_FOUND == ret)) {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalDeleteLdapHostMessage ALastConfigReplay true");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_DEBUG, "pAAALocalDeleteLdapHostMessage getIsALastConfigReplay false");
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Deleting ldap host " + pAAALocalDeleteLDAPHostMessage->getHost() + string(" failed"));
		}
	}

	free(pldapconf->hostname);
	free(pldapconf);

	pAAALocalDeleteLDAPHostMessage->setCompletionStatus (status);
	reply (pAAALocalDeleteLDAPHostMessage);
}

void  LocalServerMgmtWorker::AAALocalChangeAuthLoginModeMessageHandler( AAALocalChangeAuthLoginModeMessage *pAAALocalChangeAuthLoginModeMessage)
{
	trace (TRACE_LEVEL_DEBUG, "LocalServerMgmtWorker::AAALocalChangeAuthLoginModeMessageHandler()");
	if (pAAALocalChangeAuthLoginModeMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	/*
	 * Change the netlogin/ttylogin/sshd PAM configuration files in the file system
	 */
	ResourceId status;
	UI8 authMode;
	bool radMode, tacMode, ldapMode, logout;
	int ret = -1;

	authMode = pAAALocalChangeAuthLoginModeMessage->getAuthMode();
	radMode = pAAALocalChangeAuthLoginModeMessage->getRadiusMode();
	tacMode = pAAALocalChangeAuthLoginModeMessage->getTacacsMode();
	ldapMode = pAAALocalChangeAuthLoginModeMessage->getLDAPMode();
	logout = pAAALocalChangeAuthLoginModeMessage->getLogout();

	trace (TRACE_LEVEL_INFO, string("Calling Sec Lib API to make changes to the FS for authMode ") + authMode);
	ret = secAuthConfig((secAuthTypeConf_t)authMode, radMode, ldapMode, tacMode, logout);
	if (ret == 0) {
#ifdef LAST_CMD_SYNC_TEST
        if ((pAAALocalChangeAuthLoginModeMessage->getIsALastConfigReplay() ==false) &&
					(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            trace (TRACE_LEVEL_DEBUG, "pAAALocalChangeAuthLoginModeMessag getIsALastConfigReplay false");
            int sysRet = system ("/sbin/reboot -f");
			trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + sysRet);
        } 
#endif
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_INFO, "AAA Local Service: Changing login auth spec is successful on the switch");
	} else {
		/* No check required for Last config replay since secAuthConfig() does not return error */
		status = WAVE_MESSAGE_ERROR;
		trace (TRACE_LEVEL_ERROR, "AAA Local Service: Changing login auth spec failed on the switch");
	}

	pAAALocalChangeAuthLoginModeMessage->setCompletionStatus (status);
	reply (pAAALocalChangeAuthLoginModeMessage);
}

void LocalServerMgmtWorker::AAALocalChangeAccountingMessageHandler(AAALocalChangeAccountingMessage *pAAALocalChangeAccountingMessage)
{
    trace (TRACE_LEVEL_DEBUG, "LocalServerMgmtWorker::AAALocalChangeAccountingMessageHandler()");

    if (pAAALocalChangeAccountingMessage== NULL) {
        trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
        prismAssert (false, __FILE__, __LINE__);
    }

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    int retVal = -1;    int accType = -1;
    int srvType = -1;
        
    accType = pAAALocalChangeAccountingMessage->getAccType();
    srvType = pAAALocalChangeAccountingMessage->getServerType();
        
    if (EXEC_ACC == accType) {
        if (TACACS_SRV == srvType) {
            retVal = secTacacsExecAccSet(1);
        } else if (NO_SRV == srvType) {
            retVal = secTacacsExecAccSet(0);
        }
        if (0 == retVal) {
            status = WAVE_MESSAGE_SUCCESS;
            trace (TRACE_LEVEL_INFO, "Successfully enabled/disabledLogin Acc at backend\n");
        } else {
	        status = WAVE_MESSAGE_ERROR;
            trace (TRACE_LEVEL_INFO, "Enabling/Disabling Login Acc at backend failed\n");
        }
    } else if (CMD_ACC == accType) {
		if (TACACS_SRV == srvType) {
			retVal = secTacacsCmdAccSet(1);
			if (0 == retVal) {
				tracePrintf(TRACE_LEVEL_INFO, "flag for tacacs+ command accounting is enabled");
			} // TODO: Enable other accounting in future eg: ldap, radius etc...
		} else if (NO_SRV == srvType) {
			retVal = secTacacsCmdAccSet(0);
			if (0 == retVal) {
				//TODO: disable other accounting flag in future 
				tracePrintf(TRACE_LEVEL_INFO, "flag for tacacs+ command accounting is diabled");
			}
		}
		if (0 == retVal) {
#ifdef LAST_CMD_SYNC_TEST
      	  	if ((pAAALocalChangeAccountingMessage->getIsALastConfigReplay() ==false) &&
						(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
            	trace (TRACE_LEVEL_DEBUG, "pAAALocalChangeAccountingMessage getIsALastConfigReplay false");
	            int sysRet = system ("/sbin/reboot -f");
				trace (TRACE_LEVEL_DEBUG, string("/sbin/reboot -f retVal ") + sysRet);
    	    }
#endif
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Successfully enabled/disabled Cmd Acc at backend\n");
		} else {
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_INFO, "Enabling/Disabling Cmd Acc at backend failed\n");
		}
    }

    pAAALocalChangeAccountingMessage->setCompletionStatus (status);
    reply (pAAALocalChangeAccountingMessage);
}
}
