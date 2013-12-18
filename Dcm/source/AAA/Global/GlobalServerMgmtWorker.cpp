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

#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "DcmResourceIdEnums.h"

#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/GlobalServerMgmtWorker.h"
#include "AAA/Global/AAAGlobalAddRadiusHostMessage.h"
#include "AAA/Global/AAAGlobalChangeRadiusHostMessage.h"
#include "AAA/Global/AAAGlobalDeleteRadiusHostMessage.h"
#include "AAA/Global/AAAGlobalAddTacacsHostMessage.h"
#include "AAA/Global/AAAGlobalChangeTacacsHostMessage.h"
#include "AAA/Global/AAAGlobalDeleteTacacsHostMessage.h"
#include "AAA/Global/AAAGlobalAddLDAPHostMessage.h"
#include "AAA/Global/AAAGlobalChangeLDAPHostMessage.h"
#include "AAA/Global/AAAGlobalDeleteLDAPHostMessage.h"
#include "AAA/Global/AAAGlobalChangeAuthLoginModeMessage.h"
#include "AAA/Global/AAAGlobalChangeAccountingMessage.h"
#include "AAA/Global/RadiusHostManagedObject.h"
#include "AAA/Global/TacacsHostManagedObject.h"
#include "AAA/Global/LDAPHostManagedObject.h"
#include "AAA/Global/AuthLoginModeManagedObject.h"
#include "AAA/Global/ExecAccountingManagedObject.h"
#include "AAA/Global/CmdAccountingManagedObject.h"
#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Global/AAASequencerContexts.h"
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

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include "brocade-aaa.h"

extern "C" {
  #include "security/public.h"
}

using namespace WaveNs;

namespace DcmNs
{

GlobalServerMgmtWorker::GlobalServerMgmtWorker ( AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: WaveWorker  (pAAAGlobalObjectManager)
{
	RadiusHostManagedObject    radiusHostMO    (pAAAGlobalObjectManager);
	radiusHostMO.setupOrm ();
	addManagedClass (RadiusHostManagedObject::getClassName (), this);

	TacacsHostManagedObject    tacacsHostMO    (pAAAGlobalObjectManager);
	tacacsHostMO.setupOrm ();
	addManagedClass (TacacsHostManagedObject::getClassName (), this);

	LDAPHostManagedObject    ldapHostMO    (pAAAGlobalObjectManager);
	ldapHostMO.setupOrm ();
	addManagedClass (LDAPHostManagedObject::getClassName (), this);

	AuthLoginModeManagedObject    authLoginModeMO    (pAAAGlobalObjectManager);
	authLoginModeMO.setupOrm ();
	addManagedClass (AuthLoginModeManagedObject::getClassName (), this);

	ExecAccountingManagedObject execAccountingMO    (pAAAGlobalObjectManager);
    execAccountingMO.setupOrm ();
    addManagedClass (ExecAccountingManagedObject::getClassName (), this);

    CmdAccountingManagedObject cmdAccountingMO    (pAAAGlobalObjectManager);
    cmdAccountingMO.setupOrm ();
    addManagedClass (CmdAccountingManagedObject::getClassName (), this);

	addOperationMap (AAAGLOBALADDRADIUSHOST, reinterpret_cast<PrismMessageHandler> (&GlobalServerMgmtWorker::AAAGlobalAddRadiusHostMessageHandler));
	addOperationMap (AAAGLOBALCHANGERADIUSHOST, reinterpret_cast<PrismMessageHandler> (&GlobalServerMgmtWorker::AAAGlobalChangeRadiusHostMessageHandler));
	addOperationMap (AAAGLOBALDELETERADIUSHOST, reinterpret_cast<PrismMessageHandler> (&GlobalServerMgmtWorker::AAAGlobalDeleteRadiusHostMessageHandler));

	addOperationMap (AAAGLOBALADDTACACSHOST, reinterpret_cast<PrismMessageHandler> (&GlobalServerMgmtWorker::AAAGlobalAddTacacsHostMessageHandler));
	addOperationMap (AAAGLOBALCHANGETACACSHOST, reinterpret_cast<PrismMessageHandler> (&GlobalServerMgmtWorker::AAAGlobalChangeTacacsHostMessageHandler));
	addOperationMap (AAAGLOBALDELETETACACSHOST, reinterpret_cast<PrismMessageHandler> (&GlobalServerMgmtWorker::AAAGlobalDeleteTacacsHostMessageHandler));

	addOperationMap (AAAGLOBALADDLDAPHOST, reinterpret_cast<PrismMessageHandler> (&GlobalServerMgmtWorker::AAAGlobalAddLDAPHostMessageHandler));
	addOperationMap (AAAGLOBALCHANGELDAPHOST, reinterpret_cast<PrismMessageHandler> (&GlobalServerMgmtWorker::AAAGlobalChangeLDAPHostMessageHandler));
	addOperationMap (AAAGLOBALDELETELDAPHOST, reinterpret_cast<PrismMessageHandler> (&GlobalServerMgmtWorker::AAAGlobalDeleteLDAPHostMessageHandler));

	addOperationMap (AAAGLOBALCHANGEAUTHLOGINMODE, reinterpret_cast<PrismMessageHandler> (&GlobalServerMgmtWorker::AAAGlobalChangeAuthLoginModeMessageHandler));

	addOperationMap (AAAGLOBALCHANGEACCOUNTING, reinterpret_cast<PrismMessageHandler> (&GlobalServerMgmtWorker::AAAGlobalChangeAccountingMessageHandler));
}

GlobalServerMgmtWorker::~GlobalServerMgmtWorker ()
{
}

PrismMessage  *GlobalServerMgmtWorker::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{
		case AAAGLOBALADDRADIUSHOST :
			pPrismMessage = new AAAGlobalAddRadiusHostMessage ();
			break;
		case AAAGLOBALCHANGERADIUSHOST :
			pPrismMessage = new AAAGlobalChangeRadiusHostMessage ();
			break;
		case AAAGLOBALDELETERADIUSHOST :
			pPrismMessage = new AAAGlobalDeleteRadiusHostMessage ();
			break;
		case AAAGLOBALADDTACACSHOST :
			pPrismMessage = new AAAGlobalAddTacacsHostMessage ();
			break;
		case AAAGLOBALCHANGETACACSHOST :
			pPrismMessage = new AAAGlobalChangeTacacsHostMessage ();
			break;
		case AAAGLOBALDELETETACACSHOST :
			pPrismMessage = new AAAGlobalDeleteTacacsHostMessage ();
			break;
		case AAAGLOBALADDLDAPHOST :
			pPrismMessage = new AAAGlobalAddLDAPHostMessage ();
			break;
		case AAAGLOBALCHANGELDAPHOST :
			pPrismMessage = new AAAGlobalChangeLDAPHostMessage ();
			break;
		case AAAGLOBALDELETELDAPHOST :
			pPrismMessage = new AAAGlobalDeleteLDAPHostMessage ();
			break;
		case AAAGLOBALCHANGEAUTHLOGINMODE :
			pPrismMessage = new AAAGlobalChangeAuthLoginModeMessage ();
			break;
		case AAAGLOBALCHANGEACCOUNTING:
            pPrismMessage = new AAAGlobalChangeAccountingMessage ();
            break;
		default :
			pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject  *GlobalServerMgmtWorker::createManagedObjectInstance(const string &managedClassName)
{
	WaveManagedObject *pWaveManagedObject = NULL;

	if ((RadiusHostManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new RadiusHostManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((TacacsHostManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new TacacsHostManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((LDAPHostManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new LDAPHostManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((AuthLoginModeManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new AuthLoginModeManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((ExecAccountingManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new ExecAccountingManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((CmdAccountingManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new CmdAccountingManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
    }
	else
	{
		trace (TRACE_LEVEL_FATAL, "GlobalServerMgmtWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
	}

	return (pWaveManagedObject);
}

void  GlobalServerMgmtWorker::AAAGlobalAddRadiusHostMessageHandler( AAAGlobalAddRadiusHostMessage *pAAAGlobalAddRadiusHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalServerMgmtWorker::AAAGlobalAddRadiusHostMessageHandler()");
	if (pAAAGlobalAddRadiusHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string host = pAAAGlobalAddRadiusHostMessage->getHost();
	SI32 port = pAAAGlobalAddRadiusHostMessage->getPort();
	AuthProtocol protocol = pAAAGlobalAddRadiusHostMessage->getProtocol();
	string key = pAAAGlobalAddRadiusHostMessage->getKey();
	SI32 retransmit = pAAAGlobalAddRadiusHostMessage->getRetransmit();
	SI32 timeout = pAAAGlobalAddRadiusHostMessage->getTimeout();
	vector<UI32> locations;

	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	/* Retrieve and validate the input parameters */
	// Host
	ret = radiusParamValidate(host, 0, RADIUS_HOST);
	if (ret != 0)
	{
		status = RADIUS_INVALID_HOST;
	}

	// Port
	if ((ret == 0) && (pAAAGlobalAddRadiusHostMessage->getPortFlag() == true))
	{
		ret = radiusParamValidate(string(""), port, RADIUS_PORT);
		if (ret != 0) {
			status = RADIUS_INVALID_PORT_NUMBER;
		}
	} else if (-1 == port) {
		port = RADIUS_DEF_AUTH_PORT;
	}

	// Protocol
	if ((ret == 0) && (pAAAGlobalAddRadiusHostMessage->getProtocolFlag() == true))
	{
		ret = radiusParamValidate(string(""), protocol, RADIUS_PROTOCOL);
		if (ret != 0)
		{
			status = RADIUS_INVALID_PROTOCOL;
		}
		else
		{
			/* MS-CHAP-PEAP is not supported for IPv6 Address */
			char addr[16];
			if (inet_pton(AF_INET6, host.c_str(), (void *) &addr) && (protocol == RADIUS_PEAP_PROTO))
			{
				status = RADIUS_MSCHAP_V6_ERROR;
				ret = -1;
			}
		}
	} else if (NO_AUTHPROTO == protocol) {
		protocol = RADIUS_DEF_PROTO;
	}

	// Key
	if ((ret == 0) && (key.length() > 0))
	{
		ret = radiusParamValidate(key, 0, RADIUS_KEY);
		if (ret != 0)
		{
			status = RADIUS_INVALID_KEY;
		}
	} else if (key.length() == 0) {
		key = RADIUS_DEF_KEY;
	}

	// Retransmit
	if ((ret == 0) && (pAAAGlobalAddRadiusHostMessage->getRetransmitFlag() == true))
	{
		ret = radiusParamValidate(string(""), retransmit, RADIUS_RETRANSMIT);
		if (ret != 0)
		{
			status = RADIUS_INVALID_RETRANSMIT;
		}
	} else if (-1 == retransmit) {
		retransmit = RADIUS_DEF_RETRANSMIT;
	}

	// Timeout
	if ((ret == 0) && (timeout != -1))
	{
		ret = radiusParamValidate(string(""), timeout, RADIUS_TIMEOUT);
		if (ret != 0)
		{
			status = RADIUS_INVALID_TIMEOUT;
		}
	} else {
		timeout = RADIUS_DEF_TMOUT;
	}

	if (ret == 0)
	{
		trace (TRACE_LEVEL_DEBUG, "User input validation is successful.");
	}
	else
	{
		pAAAGlobalAddRadiusHostMessage->setCompletionStatus(status);
		reply (pAAAGlobalAddRadiusHostMessage);
		return;
	}

	/* Compute the index for new radius host */
	int index = 1;
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RadiusHostManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if (NULL == pResults)
	{
		trace (TRACE_LEVEL_FATAL, "No DB Table for Radius Host.");
		pAAAGlobalAddRadiusHostMessage->setCompletionStatus (RADIUS_HOST_ADD_ERROR);
		reply (pAAAGlobalAddRadiusHostMessage);
		delete synchronousQueryContext;
		return;
	}
	else
	{
		if (pResults->size() == RADIUS_MAX_SERVERS)
		{
			trace (TRACE_LEVEL_ERROR, "Maximum limit of five servers reached.");
			pAAAGlobalAddRadiusHostMessage->setCompletionStatus (RADIUS_HOST_MAX_LIMIT);
			reply (pAAAGlobalAddRadiusHostMessage);
			delete synchronousQueryContext;
			for (UI32 i = 0; i < pResults->size();i++)
				delete (*pResults)[i];
			delete pResults;
			return;
		}
			index = pResults->size() + 1; // Next index
		for (UI32 j = 0; j < pResults->size(); j++)
			delete (*pResults)[j];

		delete synchronousQueryContext;
		delete pResults;
	}

	if (pAAAGlobalAddRadiusHostMessage->getIsALastConfigReplay() == true) {
		trace (TRACE_LEVEL_DEBUG, "pAAAGlobalAddRadiusHostMessage getIsALastConfigReplay false");
		synchronousQueryContext =
				new WaveManagedObjectSynchronousQueryContext(RadiusHostManagedObject::getClassName());
	
	    synchronousQueryContext->addAndAttribute(new AttributeString(host, "host"));
	    pResults = querySynchronously (synchronousQueryContext);
	    if ((NULL == pResults) || (pResults->size() != 1))
	    {
	        trace (TRACE_LEVEL_DEBUG, "Host does not exist");
			delete synchronousQueryContext;
		} else {
			trace (TRACE_LEVEL_INFO, "pAAAGlobalAddRadiusHostMessage->getIsALastConfigReplay()");
			pAAAGlobalAddRadiusHostMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
			reply (pAAAGlobalAddRadiusHostMessage);
			delete synchronousQueryContext;
			if (pResults != NULL) {
				for (UI32 j = 0; j < pResults->size(); j++)
					delete (*pResults)[j];
				delete pResults;
			}
			return;
	    }
	}
		

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	/* Send updates to all nodes of the cluster for local FS sync */
	AAALocalAddRadiusHostMessage *pAddMessage = new AAALocalAddRadiusHostMessage (host, port, protocol, key, retransmit, timeout);

	AAAAddRadiusHostContext *pRadiusHostCtx = new AAAAddRadiusHostContext (pAAAGlobalAddRadiusHostMessage, index);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalServerMgmtWorker::addRadiusHostSTCCallback), pRadiusHostCtx);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pAddMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);

	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}
	sendToWaveCluster (pWaveSendToClusterContext);
}

void  GlobalServerMgmtWorker::AAAGlobalAddLDAPHostMessageHandler(AAAGlobalAddLDAPHostMessage *pAAAGlobalAddLDAPHostMessage)
{
	trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::AAAGlobalAddLDAPHostMessageHandler()");
	if (pAAAGlobalAddLDAPHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string host = pAAAGlobalAddLDAPHostMessage->getHost();
	SI16 port = pAAAGlobalAddLDAPHostMessage->getPort();
	string domain = pAAAGlobalAddLDAPHostMessage->getBasedn();
	SI32 retries = pAAAGlobalAddLDAPHostMessage->getRetries();
	SI32 timeout = pAAAGlobalAddLDAPHostMessage->getTimeout();
	vector<UI32> locations;
	
	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	/* Validate the user input parameters */
	ret = radiusParamValidate(host, 0, RADIUS_HOST);
	if (ret != 0)
	{
		status = LDAP_INVALID_HOST;
	}

	if (ret == 0)
	{
		if (port >= 0)
		{
			ret = radiusParamValidate(string(""), port, RADIUS_PORT);
			if (ret != 0) {
				status = LDAP_INVALID_PORT_NUMBER;
			}
		}
		else if (-1 == port) 
		{
			port = LDAP_DEF_PORT;
		}
    }

	if (ret == 0)
	{
		if (retries >= 0)
		{
			ret = radiusParamValidate(string(""), retries, RADIUS_RETRANSMIT);
			if (ret != 0)
			{
				status = LDAP_INVALID_RETRIES;
			}
		}
		else if (-1 == retries)
		{
			retries = LDAP_DEF_RETRY;
		}
	}

	if (ret == 0)
	{
		if (timeout >= 0)
		{
			ret = radiusParamValidate(string(""), timeout, RADIUS_TIMEOUT);
			if (ret != 0)
			{
				status = LDAP_INVALID_TIMEOUT;
			}
		}
		else if (-1 == timeout)
		{
			timeout = LDAP_DEF_TMOUT;
		}
	}
	if (ret == 0)
	{
		if (domain.length() == 0)
		{
			domain = LDAP_DEF_BASEDN;
		}
	}

	/* If the input parameter validation fails, return here with the proper errors. */
	if (ret != 0)
	{
		trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::AAAGlobalAddLDAPHostMessageHandler() validation failed");
		pAAAGlobalAddLDAPHostMessage->setCompletionStatus(status);
		reply (pAAAGlobalAddLDAPHostMessage);
		return;
	}
	else
	{
		trace (TRACE_LEVEL_INFO, "User input validation is successful.");
	}

	/* Compute the index for new LDAP host */
	int index = 1;
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(LDAPHostManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if (NULL == pResults)
	{
		trace (TRACE_LEVEL_FATAL, "No DB Table for LDAP Host.");
		pAAAGlobalAddLDAPHostMessage->setCompletionStatus (LDAP_HOST_ADD_ERROR);
		reply (pAAAGlobalAddLDAPHostMessage);
		delete synchronousQueryContext;
		return;
	}
	else
	{
		if (pResults->size() == LDAP_MAX_SERVERS)
		{
			trace (TRACE_LEVEL_ERROR, "Maximum limit of 5 servers reached.");
			pAAAGlobalAddLDAPHostMessage->setCompletionStatus (LDAP_HOST_MAX_LIMIT);
			reply (pAAAGlobalAddLDAPHostMessage);
			delete synchronousQueryContext;
			for (UI32 i = 0; i < pResults->size(); i++)
				delete (*pResults)[i];
			delete pResults;
			return;
		}
		index = pResults->size() + 1; // Next index
		for (UI32 j = 0; j < pResults->size(); j++)
			delete (*pResults)[j];

		delete synchronousQueryContext;
		delete pResults;
	}

	if (pAAAGlobalAddLDAPHostMessage->getIsALastConfigReplay() == true) {
		trace (TRACE_LEVEL_DEBUG, "pAAAGlobalAddLdapHostMessage getIsALastConfigReplay");
		synchronousQueryContext =
			new WaveManagedObjectSynchronousQueryContext(LDAPHostManagedObject::getClassName());

		synchronousQueryContext->addAndAttribute(new AttributeString(host, "host"));
		pResults = querySynchronously (synchronousQueryContext);
		if ((NULL == pResults) || (pResults->size() != 1))
		{
			trace (TRACE_LEVEL_DEBUG, "Host does not exist");
			delete synchronousQueryContext;
		} else {
			trace (TRACE_LEVEL_DEBUG, "pAAAGlobalAddLdapHostMessage->getIsALastConfigReplay()");
			pAAAGlobalAddLDAPHostMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
			reply (pAAAGlobalAddLDAPHostMessage);
			delete synchronousQueryContext;
			if (pResults != NULL) {
				for (UI32 j = 0; j < pResults->size(); j++)
					delete (*pResults)[j];
				delete pResults;
			}
			return;
		}
	}

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));
	trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::AAAGlobalAddLDAPHostMessageHandler() Before constructing the message");
	/* Send updates to all nodes of the cluster for local FS sync */
	AAALocalAddLDAPHostMessage *pAddMessage = new AAALocalAddLDAPHostMessage (host, port, domain, retries, timeout);

	AAAAddLDAPHostContext *pLDAPHostCtx = new AAAAddLDAPHostContext (pAAAGlobalAddLDAPHostMessage, index);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalServerMgmtWorker::addLDAPHostSTCCallback), pLDAPHostCtx);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pAddMessage);

	pWaveSendToClusterContext->setPartialSuccessFlag(true);

	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
	trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::AAAGlobalAddLDAPHostMessageHandler() just sent it to Local");
}
void GlobalServerMgmtWorker::addLDAPHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
    trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::addLDAPHostSTCCallback()");

	AAAAddLDAPHostContext *pLDAPHostCtx = (AAAAddLDAPHostContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalAddLDAPHostMessage *pAddMessage = (AAALocalAddLDAPHostMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalAddLDAPHostMessage *pAAAGlobalAddLDAPHostMessage = (AAAGlobalAddLDAPHostMessage *)pLDAPHostCtx->getPPrismMessage();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Adding LDAP host operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Adding LDAP host operation failed on all nodes");

		vector<LocationId> cxLocations =  pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pLDAPHostCtx->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalServerMgmtWorker::addLDAPHostSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? LDAP_HOST_ADD_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? LDAP_HOST_ADD_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		LDAPHostManagedObject *pLDAPHostObj = dynamic_cast<LDAPHostManagedObject *>(createManagedObjectInstance(LDAPHostManagedObject::getClassName()));

		pLDAPHostObj->setHost(pAddMessage->getHost());
		pLDAPHostObj->setPort(pAddMessage->getPort());
		pLDAPHostObj->setBasedn(pAddMessage->getBasedn());
		pLDAPHostObj->setRetries(pAddMessage->getRetries());
		pLDAPHostObj->setTimeout(pAddMessage->getTimeout());

		pLDAPHostObj->setIndex(pLDAPHostCtx->getIndex());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Wave database commit is successful.");
		}
		else
		{
			status = LDAP_HOST_ADD_ERROR;
			trace (TRACE_LEVEL_ERROR, "Wave DB commit failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
		delete pLDAPHostObj;
	}

	delete pAddMessage;
	delete pLDAPHostCtx;
	delete pWaveSendToClusterContext;

	pAAAGlobalAddLDAPHostMessage->setMessageString(errorMessage);
	pAAAGlobalAddLDAPHostMessage->setCompletionStatus(status);
	reply (pAAAGlobalAddLDAPHostMessage);

	trace (TRACE_LEVEL_DEBUG, "AAAGlobalAddLDAPHostMessageHandler() - EXIT.");
}

void GlobalServerMgmtWorker::AAAGlobalChangeLDAPHostMessageHandler (AAAGlobalChangeLDAPHostMessage *pAAAGlobalChangeLDAPHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalServerMgmtWorker::AAAGlobalChangeLDAPHostMessageHandler()");
	if (pAAAGlobalChangeLDAPHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string host = pAAAGlobalChangeLDAPHostMessage->getHost();
	SI32 port = pAAAGlobalChangeLDAPHostMessage->getPort();
	string basedn = pAAAGlobalChangeLDAPHostMessage->getBasedn();
	SI8 retries = pAAAGlobalChangeLDAPHostMessage->getRetries();
	SI8 timeout = pAAAGlobalChangeLDAPHostMessage->getTimeout();
	vector<UI32> locations;

	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	/* Get the LDAP Host Managed Object for optional parameters */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(LDAPHostManagedObject::getClassName());
	synchronousQueryContext->addAndAttribute(new AttributeString(host, "host"));
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_ERROR, "Tac+ host does not exist");
		if (pResults != NULL) {
			for (UI32 i = 0; i < pResults->size(); i++)
				delete (*pResults)[i];

			delete pResults;
		}
		delete synchronousQueryContext;
		pAAAGlobalChangeLDAPHostMessage->setCompletionStatus (LDAP_HOST_DOESNT_EXIST);
		reply (pAAAGlobalChangeLDAPHostMessage);
		return;
	}
	LDAPHostManagedObject *pLDAPHostObj = dynamic_cast<LDAPHostManagedObject *> ((*pResults)[0]);

	if (NULL == pLDAPHostObj) {
		trace (TRACE_LEVEL_ERROR, "LDAP host does not exist");
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		delete synchronousQueryContext;
		pAAAGlobalChangeLDAPHostMessage->setCompletionStatus (LDAP_HOST_DOESNT_EXIST);
		reply (pAAAGlobalChangeLDAPHostMessage);
		return;
	}
	delete pResults;
	delete synchronousQueryContext;

	if (((port == -1) || (port == pLDAPHostObj->getPort())) &&
		((retries == -1) || (retries == pLDAPHostObj->getRetries())) &&
		((timeout == -1) || (timeout == pLDAPHostObj->getTimeout())) &&
		((basedn.length() <= 0) || (basedn.compare(pLDAPHostObj->getBasedn()) == 0)))
	{
		delete pLDAPHostObj;
		/* Last config replay also taken care here */
		pAAAGlobalChangeLDAPHostMessage->setCompletionStatus(status);
		reply (pAAAGlobalChangeLDAPHostMessage);
		return;
	}

	/* Validate the user input parameters */
	if (port >= 0)
	{
		ret = radiusParamValidate(string(""), port, RADIUS_PORT);
		if (ret != 0) {
			status = LDAP_INVALID_PORT_NUMBER;
		}
	}
    else
    {
        port = pLDAPHostObj->getPort();
    }
	
	/* Kumar Ghanta: As we are not validating the basedn format in the FOS, I am not doing it now.
	 * But I will try to do it in the future after reading the Active directory spec.
	 */

	if (ret == 0)
	{
		if (basedn.length() <= 0)
		{
			basedn = pLDAPHostObj->getBasedn();
 		}
	}
	if (ret == 0)
	{
		if (retries >= 0)
		{
			ret = radiusParamValidate(string(""), retries, RADIUS_RETRANSMIT);
			if (ret != 0)
			{
				status = LDAP_INVALID_RETRIES;
			}
		}
		else
		{
			retries = pLDAPHostObj->getRetries();
		}
	}

	if (ret == 0)
	{
		if (timeout >= 0)
		{
			ret = radiusParamValidate(string(""), timeout, RADIUS_TIMEOUT);
			if (ret != 0)
			{
				status = LDAP_INVALID_TIMEOUT;
			}
		}
		else
		{
			timeout = pLDAPHostObj->getTimeout();
		}
	}

	if (ret != 0)
	{
		delete pLDAPHostObj;
		pAAAGlobalChangeLDAPHostMessage->setCompletionStatus(status);
		reply (pAAAGlobalChangeLDAPHostMessage);
		return;
	}
	else
	{
		trace (TRACE_LEVEL_DEBUG, "User input validation is successful.");
	}

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	/* Send updates to all nodes of the cluster for local FS sync */
	AAALocalChangeLDAPHostMessage *pSetMessage = new AAALocalChangeLDAPHostMessage (host, port, basedn, retries, timeout);

	AAAChangeLDAPHostContext *pLDAPHostCtx = new AAAChangeLDAPHostContext (pAAAGlobalChangeLDAPHostMessage, pLDAPHostObj);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalServerMgmtWorker::changeLDAPHostSTCCallback), pLDAPHostCtx);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pSetMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalServerMgmtWorker::changeLDAPHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
    trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::changeLDAPHostSTCCallback()");

	AAAChangeLDAPHostContext *pLDAPHostCtx = (AAAChangeLDAPHostContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalChangeLDAPHostMessage *pSetMessage = (AAALocalChangeLDAPHostMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalChangeLDAPHostMessage *pAAAGlobalChangeLDAPHostMessage = (AAAGlobalChangeLDAPHostMessage *)pLDAPHostCtx->getPPrismMessage();
	LDAPHostManagedObject *pLDAPHostObj = pLDAPHostCtx->getLDAPHostObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Change LDAP host operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Change LDAP host operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pLDAPHostCtx->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalServerMgmtWorker::changeLDAPHostSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? LDAP_HOST_CHANGE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? LDAP_HOST_CHANGE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		updateWaveManagedObject (pLDAPHostObj);

		pLDAPHostObj->setPort(pSetMessage->getPort());
		pLDAPHostObj->setBasedn(pSetMessage->getBasedn());
		pLDAPHostObj->setRetries(pSetMessage->getRetries());
		pLDAPHostObj->setTimeout(pSetMessage->getTimeout());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Wave database commit is successful.");
		}
		else
		{
			status = LDAP_HOST_CHANGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Wave DB commit failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}

	delete pLDAPHostObj;
	delete pSetMessage;
	delete pLDAPHostCtx;
	delete pWaveSendToClusterContext;

	pAAAGlobalChangeLDAPHostMessage->setMessageString(errorMessage);
	pAAAGlobalChangeLDAPHostMessage->setCompletionStatus(status);
	reply (pAAAGlobalChangeLDAPHostMessage);

	trace (TRACE_LEVEL_DEBUG, "AAAGlobalChangeLDAPHostMessageHandler() - EXIT.");
}

void GlobalServerMgmtWorker::AAAGlobalDeleteLDAPHostMessageHandler (AAAGlobalDeleteLDAPHostMessage *pAAAGlobalDeleteLDAPHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalServerMgmtWorker::AAAGlobalDeleteLDAPHostMessageHandler()");
	if (pAAAGlobalDeleteLDAPHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string host = pAAAGlobalDeleteLDAPHostMessage->getHost();
	vector<LDAPHostManagedObject *> *pLDAPHostsForUpdatingIndex = NULL;

	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	vector<UI32> locations;

	/* Get the host Managed Object instance */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(LDAPHostManagedObject::getClassName());
	synchronousQueryContext->addAndAttribute (new AttributeString (host, "host"));
	pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_ERROR, "LDAP host does not exist");
		if (pResults != NULL) {
			for (UI32 i = 0; i < pResults->size(); i++)
				delete (*pResults)[i];

			delete pResults;
		}
		delete synchronousQueryContext;
		if (pAAAGlobalDeleteLDAPHostMessage->getIsALastConfigReplay() == true) {
			trace (TRACE_LEVEL_DEBUG, "pAAAGlobalDeleteLDAPHostMessag getIsALastConfigReplay false");
			pAAAGlobalDeleteLDAPHostMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		} else {
			pAAAGlobalDeleteLDAPHostMessage->setCompletionStatus (LDAP_HOST_DOESNT_EXIST);
		}
		reply (pAAAGlobalDeleteLDAPHostMessage);
		return;
	}
	LDAPHostManagedObject *pLDAPHostObj = dynamic_cast<LDAPHostManagedObject *> ((*pResults)[0]);

	if (NULL == pLDAPHostObj) {
		pAAAGlobalDeleteLDAPHostMessage->setCompletionStatus (LDAP_HOST_DOESNT_EXIST);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		reply (pAAAGlobalDeleteLDAPHostMessage);
		return;
	}
	delete pResults;
	delete synchronousQueryContext;

	trace (TRACE_LEVEL_INFO, "LDAP host MO instance found in the database.");

	/* Check if it is the last LDAP server in the config database */
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(LDAPHostManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	if (pResults->size() == 1)
	{ /* Check if auth login is set to LDAP */
		WaveManagedObjectSynchronousQueryContext *syncQueryCtxtAuthL = new WaveManagedObjectSynchronousQueryContext(AuthLoginModeManagedObject::getClassName());
		vector<WaveManagedObject *> *pResultsAuthL = querySynchronously (syncQueryCtxtAuthL);
		if (pResultsAuthL == NULL || pResultsAuthL->size() != 1)
		{
			trace (TRACE_LEVEL_FATAL, "Auth login instance not found in DB");
			status = LDAP_HOST_DELETE_ERROR;
		}
		else
		{
			AuthLoginModeManagedObject *pAuthLoginObj = dynamic_cast<AuthLoginModeManagedObject *>((*pResultsAuthL)[0]);
			if (NULL == pAuthLoginObj) {
				pAAAGlobalDeleteLDAPHostMessage->setCompletionStatus (LDAP_HOST_DELETE_ERROR);
				delete synchronousQueryContext;
				if (pResultsAuthL != NULL) {
					for (UI32 j = 0; j < pResultsAuthL->size(); j++)
						delete (*pResultsAuthL)[j];

					delete pResultsAuthL;
				}
				reply (pAAAGlobalDeleteLDAPHostMessage);
				return;
			}
			if (pAuthLoginObj->getFirst() == LDAP_DB ||
				pAuthLoginObj->getSecond() == LDAP_DB)
			{
				status = LDAP_HOST_LAST_AUTH_SERVER;
			}
		}

		if (pResultsAuthL)
		{
			for (UI32 i = 0; i < pResultsAuthL->size(); i++)
				delete (*pResultsAuthL)[i];

			delete pResultsAuthL;
		}
		delete syncQueryCtxtAuthL;

		delete (*pResults)[0];
	}
	else if (pResults->size() > 1)
	{ /* Save them for updating their 'index' after deletion of this host */
		pLDAPHostsForUpdatingIndex = new vector<LDAPHostManagedObject *>;
		LDAPHostManagedObject *pLHost = NULL;
		for (UI8 i = 0; i < pResults->size(); i++)
		{
			pLHost = dynamic_cast<LDAPHostManagedObject *>((*pResults)[i]);
			if (pLHost) {
			if (pLHost->getHost() != host)
			{
				pLDAPHostsForUpdatingIndex->push_back(pLHost);
			}
			else
			{
				delete pLHost;
			}
		}
	}
	}
	delete pResults;
	delete synchronousQueryContext;

	if (status != WAVE_MESSAGE_SUCCESS)
	{
		delete pLDAPHostObj;
		pAAAGlobalDeleteLDAPHostMessage->setCompletionStatus (status);
		reply (pAAAGlobalDeleteLDAPHostMessage);
		return;
	}

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	/* Send updates to all nodes of the cluster for local FS sync */
	AAALocalDeleteLDAPHostMessage *pDelMessage = new AAALocalDeleteLDAPHostMessage (host);

	AAADeleteLDAPHostContext *pLDAPHostCtx = new AAADeleteLDAPHostContext (pAAAGlobalDeleteLDAPHostMessage, pLDAPHostObj, pLDAPHostsForUpdatingIndex);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalServerMgmtWorker::deleteLDAPHostSTCCallback), pLDAPHostCtx);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pDelMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);

	trace (TRACE_LEVEL_DEBUG, "Sending message to all AAA local services.");
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}
	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalServerMgmtWorker::deleteLDAPHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
    trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::deleteLDAPHostSTCCallback()");

	AAADeleteLDAPHostContext *pLDAPHostCtx = (AAADeleteLDAPHostContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalDeleteLDAPHostMessage *pDelMessage = (AAALocalDeleteLDAPHostMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalDeleteLDAPHostMessage *pAAAGlobalDeleteLDAPHostMessage = (AAAGlobalDeleteLDAPHostMessage *)pLDAPHostCtx->getPPrismMessage();
	LDAPHostManagedObject *pLDAPHostObj = pLDAPHostCtx->getLDAPHostObj();
	vector<LDAPHostManagedObject *> *pLDAPHostsForUpdatingIndex = pLDAPHostCtx->getLDAPHostsForUpdatingIndex();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Delete tacacs host operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Delete tacacs host operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pLDAPHostCtx->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalServerMgmtWorker::deleteLDAPHostSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? LDAP_HOST_DELETE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? LDAP_HOST_DELETE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		/* Decrease the index for the hosts added later than the host being deleted now */
		if (NULL != pLDAPHostsForUpdatingIndex)
		{
			LDAPHostManagedObject *pLDAPHostForIndexChange = NULL;
			UI8 index = 0;
			for (UI8 i = 0; i < pLDAPHostsForUpdatingIndex->size(); i++)
			{
				pLDAPHostForIndexChange = (*pLDAPHostsForUpdatingIndex)[i];
				index = pLDAPHostForIndexChange->getIndex();
				if (index > pLDAPHostObj->getIndex())
				{
					updateWaveManagedObject (pLDAPHostForIndexChange);
					pLDAPHostForIndexChange->setIndex(--index);
				}
			}
		}

		/* Delete the specified LDAP Host */
		delete pLDAPHostObj;

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Wave database commit is successful.");
		}
		else
		{
			status = LDAP_HOST_DELETE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Wave DB commit failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}

	if (NULL != pLDAPHostsForUpdatingIndex)
	{
		for (UI8 i = 0; i < pLDAPHostsForUpdatingIndex->size(); i++)
		{
			delete (*pLDAPHostsForUpdatingIndex)[i];
		}
		delete pLDAPHostsForUpdatingIndex;
	}
	delete pDelMessage;
	delete pLDAPHostCtx;
	delete pWaveSendToClusterContext;

	pAAAGlobalDeleteLDAPHostMessage->setMessageString(errorMessage);
	pAAAGlobalDeleteLDAPHostMessage->setCompletionStatus(status);
	reply (pAAAGlobalDeleteLDAPHostMessage);

	trace (TRACE_LEVEL_DEBUG, "AAAGlobalDeleteLDAPHostMessageHandler() - EXIT.");
}
void GlobalServerMgmtWorker::addRadiusHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
    trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::addRadiusHostSTCCallback()");

	AAAAddRadiusHostContext *pRadiusHostCtx = (AAAAddRadiusHostContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalAddRadiusHostMessage *pAddMessage = (AAALocalAddRadiusHostMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalAddRadiusHostMessage *pAAAGlobalAddRadiusHostMessage = (AAAGlobalAddRadiusHostMessage *)pRadiusHostCtx->getPPrismMessage();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Adding radius host operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Adding radius host operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pRadiusHostCtx->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalServerMgmtWorker::addRadiusHostSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? RADIUS_HOST_ADD_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? RADIUS_HOST_ADD_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		RadiusHostManagedObject *pRadiusHostObj = dynamic_cast<RadiusHostManagedObject *>(createManagedObjectInstance(RadiusHostManagedObject::getClassName()));

		pRadiusHostObj->setHost(pAddMessage->getHost());
		pRadiusHostObj->setPort(pAddMessage->getPort());
		pRadiusHostObj->setProtocol((AuthProtocol)pAddMessage->getProtocol());
		pRadiusHostObj->setKey(pAddMessage->getKey());
		pRadiusHostObj->setRetransmit(pAddMessage->getRetransmit());
		pRadiusHostObj->setTimeout(pAddMessage->getTimeout());

		pRadiusHostObj->setIndex(pRadiusHostCtx->getIndex());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = RADIUS_HOST_ADD_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Wave database commit is successful.");
		}
		else
		{
			status = RADIUS_HOST_ADD_ERROR;
			trace (TRACE_LEVEL_ERROR, "Wave DB commit failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
		delete pRadiusHostObj;
	}

	delete pAddMessage;
	delete pRadiusHostCtx;
	delete pWaveSendToClusterContext;

	pAAAGlobalAddRadiusHostMessage->setMessageString(errorMessage);
	pAAAGlobalAddRadiusHostMessage->setCompletionStatus(status);
	reply (pAAAGlobalAddRadiusHostMessage);

	trace (TRACE_LEVEL_DEBUG, "AAAGlobalAddRadiusHostMessageHandler() - EXIT.");
}

void  GlobalServerMgmtWorker::AAAGlobalChangeRadiusHostMessageHandler (AAAGlobalChangeRadiusHostMessage *pAAAGlobalChangeRadiusHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalServerMgmtWorker::AAAGlobalChangeRadiusHostMessageHandler()");
	if (pAAAGlobalChangeRadiusHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string host = pAAAGlobalChangeRadiusHostMessage->getHost();
	SI32 port = pAAAGlobalChangeRadiusHostMessage->getPort();
	AuthProtocol protocol = pAAAGlobalChangeRadiusHostMessage->getProtocol();
	string key = pAAAGlobalChangeRadiusHostMessage->getKey();
	SI32 retransmit = pAAAGlobalChangeRadiusHostMessage->getRetransmit();
	SI32 timeout = pAAAGlobalChangeRadiusHostMessage->getTimeout();
	vector<UI32> locations;

	RadiusHostManagedObject *pRadiusHostObj = NULL;
	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	/* Get the Radius Global Host Managed Object for optional parameters */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RadiusHostManagedObject::getClassName());

	synchronousQueryContext->addAndAttribute(new AttributeString(host, "host"));
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_FATAL, "Host does not exist");
		pAAAGlobalChangeRadiusHostMessage->setCompletionStatus (RADIUS_HOST_DOESNT_EXIST);
		reply (pAAAGlobalChangeRadiusHostMessage);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		return;
	}
	pRadiusHostObj = dynamic_cast<RadiusHostManagedObject *> ((*pResults)[0]);

	if (NULL == pRadiusHostObj) {
		trace (TRACE_LEVEL_FATAL, "Radius host does not exist");
		pAAAGlobalChangeRadiusHostMessage->setCompletionStatus (RADIUS_HOST_DOESNT_EXIST);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		reply (pAAAGlobalChangeRadiusHostMessage);
		return;
	}
	delete pResults;
	delete synchronousQueryContext;

	if (((pAAAGlobalChangeRadiusHostMessage->getPortFlag() == false) || (port == pRadiusHostObj->getPort())) &&
		((pAAAGlobalChangeRadiusHostMessage->getProtocolFlag() == false) ||
			(protocol == pRadiusHostObj->getProtocol())) &&
		((pAAAGlobalChangeRadiusHostMessage->getRetransmitFlag() == false) ||
			(retransmit == (int)pRadiusHostObj->getRetransmit())) &&
		((timeout == -1) || (timeout == (int)pRadiusHostObj->getTimeout())) &&
		((key.length() <= 0) || (key.compare(pRadiusHostObj->getKey()) == 0)))
	{
		delete pRadiusHostObj;
		/* Last config replay is also taken care */
		pAAAGlobalChangeRadiusHostMessage->setCompletionStatus(status);
		reply (pAAAGlobalChangeRadiusHostMessage);
		return;
	}
	/* Retrieve and validate the input parameters */
	// Port
	if (pAAAGlobalChangeRadiusHostMessage->getPortFlag() == true)
	{
		port = pAAAGlobalChangeRadiusHostMessage->getPort();
		ret = radiusParamValidate(string(""), port, RADIUS_PORT);
		if (ret != 0) {
			status = RADIUS_INVALID_PORT_NUMBER;
		}
	}
	else
	{
		port = pRadiusHostObj->getPort();
	}

	// Protocol
	if (ret == 0)
	{
	if (pAAAGlobalChangeRadiusHostMessage->getProtocolFlag() == true)
	{
		protocol = pAAAGlobalChangeRadiusHostMessage->getProtocol();
		ret = radiusParamValidate(string(""), protocol, RADIUS_PROTOCOL);
		if (ret != 0)
		{
			status = RADIUS_INVALID_PROTOCOL;
		}
		else
		{
			/* MS-CHAP-PEAP is not supported for IPv6 Address */
			char addr[16];
			if (inet_pton(AF_INET6, host.c_str(), (void *) &addr) && (protocol == RADIUS_PEAP_PROTO))
			{
				status = RADIUS_MSCHAP_V6_ERROR;
				ret = -1;
			}
		}
	}
	else
	{
		protocol = pRadiusHostObj->getProtocol();
	}
	}

	// Key
	if (ret == 0)
	{
	key = pAAAGlobalChangeRadiusHostMessage->getKey();
	if (key.length() != 0)
	{
		ret = radiusParamValidate(key, 0, RADIUS_KEY);
		if (ret != 0)
		{
			status = RADIUS_INVALID_KEY;
		}
	}
	else
	{
		key = pRadiusHostObj->getKey();
	}
	}

	// Retransmit
	if (ret == 0)
	{
	if (pAAAGlobalChangeRadiusHostMessage->getRetransmitFlag() == true)
	{
		retransmit = pAAAGlobalChangeRadiusHostMessage->getRetransmit();
		ret = radiusParamValidate(string(""), retransmit, RADIUS_RETRANSMIT);
		if (ret != 0)
		{
			status = RADIUS_INVALID_RETRANSMIT;
		}
	}
	else
	{
		retransmit = pRadiusHostObj->getRetransmit();
	}
	}

	// Timeout
	if (ret == 0)
	{
	timeout = pAAAGlobalChangeRadiusHostMessage->getTimeout();
	if (timeout != -1)
	{
		ret = radiusParamValidate(string(""), timeout, RADIUS_TIMEOUT);
		if (ret != 0)
		{
			status = RADIUS_INVALID_TIMEOUT;
		}
	}
	else
	{
		timeout = pRadiusHostObj->getTimeout();
	}
	}

	if (ret == 0)
	{
		trace (TRACE_LEVEL_DEBUG, "User input validation is successful.");
	}
	else
	{
		delete pRadiusHostObj;
		pAAAGlobalChangeRadiusHostMessage->setCompletionStatus(status);
		reply (pAAAGlobalChangeRadiusHostMessage);
		return;
	}
	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	/* Send updates to all nodes of the cluster for local FS sync */
	AAALocalChangeRadiusHostMessage *pSetMessage = new AAALocalChangeRadiusHostMessage (host, port, protocol, key, retransmit, timeout);

	AAAChangeRadiusHostContext *pRadiusHostCtx = new AAAChangeRadiusHostContext (pAAAGlobalChangeRadiusHostMessage, pRadiusHostObj);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalServerMgmtWorker::changeRadiusHostSTCCallback), pRadiusHostCtx);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pSetMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);

	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}
	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalServerMgmtWorker::changeRadiusHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
    trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::changeRadiusHostSTCCallback()");

	AAAChangeRadiusHostContext *pRadiusHostCtx = (AAAChangeRadiusHostContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalChangeRadiusHostMessage *pSetMessage = (AAALocalChangeRadiusHostMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalChangeRadiusHostMessage *pAAAGlobalChangeRadiusHostMessage = (AAAGlobalChangeRadiusHostMessage *)pRadiusHostCtx->getPPrismMessage();
	RadiusHostManagedObject *pRadiusHostObj = pRadiusHostCtx->getRadiusHostObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Changing radius host operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Changing radius host operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pRadiusHostCtx->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalServerMgmtWorker::changeRadiusHostSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? RADIUS_HOST_CHANGE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? RADIUS_HOST_CHANGE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		updateWaveManagedObject (pRadiusHostObj);

		pRadiusHostObj->setPort(pSetMessage->getPort());
		pRadiusHostObj->setProtocol((AuthProtocol)pSetMessage->getProtocol());
		pRadiusHostObj->setKey(pSetMessage->getKey());
		pRadiusHostObj->setRetransmit(pSetMessage->getRetransmit());
		pRadiusHostObj->setTimeout(pSetMessage->getTimeout());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = RADIUS_HOST_CHANGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Wave database commit is successful.");
		}
		else
		{
			status = RADIUS_HOST_CHANGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Wave DB commit failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}

	delete pRadiusHostObj;
	delete pSetMessage;
	delete pRadiusHostCtx;
	delete pWaveSendToClusterContext;

	pAAAGlobalChangeRadiusHostMessage->setMessageString(errorMessage);
	pAAAGlobalChangeRadiusHostMessage->setCompletionStatus(status);
	reply (pAAAGlobalChangeRadiusHostMessage);

	trace (TRACE_LEVEL_DEBUG, "AAAGlobalChangeRadiusHostMessageHandler() - EXIT.");
}

void  GlobalServerMgmtWorker::AAAGlobalDeleteRadiusHostMessageHandler( AAAGlobalDeleteRadiusHostMessage *pAAAGlobalDeleteRadiusHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalServerMgmtWorker::AAAGlobalDeleteRadiusHostMessageHandler()");
	if (pAAAGlobalDeleteRadiusHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string host = pAAAGlobalDeleteRadiusHostMessage->getHost();
	vector<RadiusHostManagedObject *> *pRadiusHostsForUpdatingIndex = NULL;
	vector<UI32> locations;

	/* Get the Managed Object instance of the host to be deleted */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RadiusHostManagedObject::getClassName());
	synchronousQueryContext->addAndAttribute (new AttributeString (host, "host"));
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() == 0))
	{
		trace (TRACE_LEVEL_FATAL, "Radius Host not found");
		if (pAAAGlobalDeleteRadiusHostMessage->getIsALastConfigReplay() == true) {
			trace (TRACE_LEVEL_DEBUG, "pAAAGlobalDeleteRadiusHostMessage getIsALastConfigReplay true");
			pAAAGlobalDeleteRadiusHostMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		} else {
			pAAAGlobalDeleteRadiusHostMessage->setCompletionStatus (RADIUS_HOST_DOESNT_EXIST);
		}
		reply (pAAAGlobalDeleteRadiusHostMessage);
		if (pResults != NULL) delete pResults;
		delete synchronousQueryContext;
		return;
	}
	trace (TRACE_LEVEL_DEBUG, "User input validation is successful.");

	RadiusHostManagedObject *pRadiusHostObj = dynamic_cast<RadiusHostManagedObject *> ((*pResults)[0]);

	if (NULL == pRadiusHostObj) {
		pAAAGlobalDeleteRadiusHostMessage->setCompletionStatus (RADIUS_HOST_DOESNT_EXIST);
		delete synchronousQueryContext;
		if (pResults != NULL) delete pResults;
		reply (pAAAGlobalDeleteRadiusHostMessage);
		return;
	}
	delete pResults;
	delete synchronousQueryContext;

	/* Check if it is the last RADIUS server in the config database */
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RadiusHostManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	if (pResults->size() == 1)
	{ /* Check if auth login is set to RADIUS */
		WaveManagedObjectSynchronousQueryContext *syncQueryCtxtAuthL = new WaveManagedObjectSynchronousQueryContext(AuthLoginModeManagedObject::getClassName());
		vector<WaveManagedObject *> *pResultsAuthL = querySynchronously (syncQueryCtxtAuthL);
		if (pResultsAuthL == NULL || pResultsAuthL->size() != 1)
		{
			trace (TRACE_LEVEL_FATAL, "Auth login instance not found in DB");
			status = RADIUS_HOST_DEL_ERROR;
		}
		else
		{
			AuthLoginModeManagedObject *pAuthLoginObj = dynamic_cast<AuthLoginModeManagedObject *>((*pResultsAuthL)[0]);
			if (NULL == pAuthLoginObj) {
				pAAAGlobalDeleteRadiusHostMessage->setCompletionStatus (RADIUS_HOST_DOESNT_EXIST);
				delete synchronousQueryContext;
				if (pResults != NULL) delete pResults;
				reply (pAAAGlobalDeleteRadiusHostMessage);
				return;
			}
			if (pAuthLoginObj->getFirst() == RADIUS_DB ||
				pAuthLoginObj->getSecond() == RADIUS_DB)
			{
				status = RADIUS_HOST_LAST_AUTH_SERVER;
			}
		}

		if (pResultsAuthL)
		{
			for (UI32 i = 0; i < pResultsAuthL->size(); i++)
				delete (*pResultsAuthL)[i];
			delete pResultsAuthL;
		}
		delete syncQueryCtxtAuthL;

		delete (*pResults)[0];
	}
	else if (pResults->size() > 1)
	{ /* Save them for updating their 'index' after deletion of this host */
		pRadiusHostsForUpdatingIndex = new vector<RadiusHostManagedObject *>;
		RadiusHostManagedObject *pRadHost = NULL;
		for (UI8 i = 0; i < pResults->size(); i++)
		{
			pRadHost = dynamic_cast<RadiusHostManagedObject *>((*pResults)[i]);
			if (pRadHost) {
			if (pRadHost->getHost() != host)
			{
				pRadiusHostsForUpdatingIndex->push_back(pRadHost);
			}
			else
			{
				delete pRadHost;
			}
		}
	}
	}
	delete pResults;
	delete synchronousQueryContext;

	if (status != WAVE_MESSAGE_SUCCESS)
	{
		delete pRadiusHostObj;
		pAAAGlobalDeleteRadiusHostMessage->setCompletionStatus (status);
		reply (pAAAGlobalDeleteRadiusHostMessage);
		return;
	}

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	/* Send updates to all nodes of the cluster for local FS sync */
	AAALocalDeleteRadiusHostMessage *pDelMessage = new AAALocalDeleteRadiusHostMessage (host);

	AAADeleteRadiusHostContext *pRadiusHostCtx = new AAADeleteRadiusHostContext (pAAAGlobalDeleteRadiusHostMessage, pRadiusHostObj, pRadiusHostsForUpdatingIndex);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalServerMgmtWorker::deleteRadiusHostSTCCallback), pRadiusHostCtx);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pDelMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);

	trace (TRACE_LEVEL_DEBUG, "Sending message to all AAA local services.");
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}
	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalServerMgmtWorker::deleteRadiusHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::deleteRadiusHostSTCCallback()");

	AAADeleteRadiusHostContext *pRadiusHostCtx = (AAADeleteRadiusHostContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalDeleteRadiusHostMessage *pDelMessage = (AAALocalDeleteRadiusHostMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalDeleteRadiusHostMessage *pAAAGlobalDeleteRadiusHostMessage = (AAAGlobalDeleteRadiusHostMessage *)pRadiusHostCtx->getPPrismMessage();
	RadiusHostManagedObject *pRadiusHostObj = pRadiusHostCtx->getRadiusHostObj();
	vector<RadiusHostManagedObject *> *pRadiusHostsForUpdatingIndex = pRadiusHostCtx->getRadiusHostsForUpdatingIndex();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Deleting radius host operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Deleting radius host operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pRadiusHostCtx->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalServerMgmtWorker::deleteRadiusHostSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? RADIUS_HOST_DEL_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? RADIUS_HOST_DEL_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		/* Decrease the index for the hosts added later than the host being deleted now */
		if (NULL != pRadiusHostsForUpdatingIndex)
		{
			RadiusHostManagedObject *pRadiusHostForIndexChange = NULL;
			UI8 index = 0;
			for (UI8 i = 0; i < pRadiusHostsForUpdatingIndex->size(); i++)
			{
				pRadiusHostForIndexChange = (*pRadiusHostsForUpdatingIndex)[i];
				index = pRadiusHostForIndexChange->getIndex();
				if (index > pRadiusHostObj->getIndex())
				{
					updateWaveManagedObject (pRadiusHostForIndexChange);
					pRadiusHostForIndexChange->setIndex(--index);
				}
			}
		}

		/* Delete the specified Radius Host */
		delete pRadiusHostObj;

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = RADIUS_HOST_DELETE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Wave database commit is successful.");
		}
		else
		{
			status = RADIUS_HOST_DEL_ERROR;
			trace (TRACE_LEVEL_ERROR, "Wave DB commit failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}

	if (NULL != pRadiusHostsForUpdatingIndex)
	{
		for (UI8 i = 0; i < pRadiusHostsForUpdatingIndex->size(); i++)
		{
			delete (*pRadiusHostsForUpdatingIndex)[i];
		}
		delete pRadiusHostsForUpdatingIndex;
	}

	delete pDelMessage;
	delete pRadiusHostCtx;
	delete pWaveSendToClusterContext;

	pAAAGlobalDeleteRadiusHostMessage->setMessageString(errorMessage);
	pAAAGlobalDeleteRadiusHostMessage->setCompletionStatus(status);
	reply (pAAAGlobalDeleteRadiusHostMessage);

	trace (TRACE_LEVEL_DEBUG, "AAAGlobalDeleteRadiusHostMessageHandler() - EXIT.");
}

void GlobalServerMgmtWorker::AAAGlobalAddTacacsHostMessageHandler (AAAGlobalAddTacacsHostMessage *pAAAGlobalAddTacacsHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalServerMgmtWorker::AAAGlobalAddTacacsHostMessageHandler()");
	if (pAAAGlobalAddTacacsHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string host = pAAAGlobalAddTacacsHostMessage->getHost();
	SI32 port = pAAAGlobalAddTacacsHostMessage->getPort();
	SI8 protocol = pAAAGlobalAddTacacsHostMessage->getProtocol();
	string key = pAAAGlobalAddTacacsHostMessage->getKey();
	SI8 retries = pAAAGlobalAddTacacsHostMessage->getRetries();
	SI8 timeout = pAAAGlobalAddTacacsHostMessage->getTimeout();
	vector<UI32> locations;

	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	/* Validate the user input parameters */
	ret = radiusParamValidate(host, 0, RADIUS_HOST);
	if (ret != 0)
	{
		status = TACACS_INVALID_HOST;
	}

	if (ret == 0)
	{
		if (port >= 0)
		{
			ret = radiusParamValidate(string(""), port, RADIUS_PORT);
			if (ret != 0) {
				status = TACACS_INVALID_PORT_NUMBER;
			}
		}
		else if (-1 == port)
		{
			port = TAC_DEF_PORT;
		}
    }

	if (ret == 0)
	{
		if (protocol >= 0)
		{
			ret = radiusParamValidate(string(""), protocol, RADIUS_PROTOCOL);
			if (ret != 0)
			{
				status = TACACS_INVALID_PROTOCOL;
			}
		}
		else if (protocol == -1)
		{
			protocol = TAC_DEF_PROTO;
		}
	}

	if (ret == 0)
	{
		if (key.length() > 0)
		{
			ret = radiusParamValidate(key, 0, TACACS_KEY);
			if (ret != 0)
			{
				status = TACACS_INVALID_KEY;
			}
		}
		else if (key.length() == 0)
		{
			key = TAC_DEF_KEY;
		}
	}

	if (ret == 0)
	{
		if (retries >= 0)
		{
			ret = radiusParamValidate(string(""), retries, RADIUS_RETRANSMIT);
			if (ret != 0)
			{
				status = TACACS_INVALID_RETRIES;
			}
		}
		else if (retries == -1)
		{
			retries = TAC_DEF_RETRY;
		}
	}

	if (ret == 0)
	{
		if (timeout >= 0)
		{
			ret = radiusParamValidate(string(""), timeout, RADIUS_TIMEOUT);
			if (ret != 0)
			{
				status = TACACS_INVALID_TIMEOUT;
			}
		}
		else if (timeout == -1)
		{
			timeout = TAC_DEF_TMOUT;
		}
	}

	if (ret != 0)
	{
		pAAAGlobalAddTacacsHostMessage->setCompletionStatus(status);
		reply (pAAAGlobalAddTacacsHostMessage);
		return;
	}
	else
	{
		trace (TRACE_LEVEL_DEBUG, "User input validation is successful.");
	}

	/* Compute the index for new tacacs+ host */
	int index = 1;
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(TacacsHostManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if (NULL == pResults)
	{
		trace (TRACE_LEVEL_FATAL, "No DB Table for Tacacs Host.");
		pAAAGlobalAddTacacsHostMessage->setCompletionStatus (TACACS_HOST_ADD_ERROR);
		reply (pAAAGlobalAddTacacsHostMessage);
		delete synchronousQueryContext;
		return;
	}
	else
	{
		if (pResults->size() == TACACS_MAX_SERVERS)
		{
			trace (TRACE_LEVEL_ERROR, "Maximum limit of 5 servers reached.");
			pAAAGlobalAddTacacsHostMessage->setCompletionStatus (TACACS_HOST_MAX_LIMIT);
			reply (pAAAGlobalAddTacacsHostMessage);
			delete synchronousQueryContext;
			for (UI32 i = 0; i < pResults->size(); i++)
				delete (*pResults)[i];
			delete pResults;
			return;
		}
		index = pResults->size() + 1; // Next index
		for (UI32 j = 0; j < pResults->size(); j++)
			delete (*pResults)[j];

		delete synchronousQueryContext;
		delete pResults;
	}

	if (pAAAGlobalAddTacacsHostMessage->getIsALastConfigReplay() == true) {
		trace (TRACE_LEVEL_DEBUG, "pAAAGlobalAddTacacsHostMessage getIsALastConfigReplay true");
		synchronousQueryContext =
			new WaveManagedObjectSynchronousQueryContext(TacacsHostManagedObject::getClassName());

		synchronousQueryContext->addAndAttribute(new AttributeString(host, "host"));
		pResults = querySynchronously (synchronousQueryContext);
		if ((NULL == pResults) || (pResults->size() != 1))
		{
			trace (TRACE_LEVEL_DEBUG, "Host does not exist");
			delete synchronousQueryContext;
		} else {
			trace (TRACE_LEVEL_DEBUG, "pAAAGlobalAddTacacsHostMessage->getIsALastConfigReplay()");
			pAAAGlobalAddTacacsHostMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
			reply (pAAAGlobalAddTacacsHostMessage);
			delete synchronousQueryContext;
			if (pResults != NULL) {
				for (UI32 j = 0; j < pResults->size(); j++)
					delete (*pResults)[j];
				delete pResults;
			}
			return;
		}
	}

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	/* Send updates to all nodes of the cluster for local FS sync */
	AAALocalAddTacacsHostMessage *pAddMessage = new AAALocalAddTacacsHostMessage (host, port, protocol, key, retries, timeout);

	AAAAddTacacsHostContext *pTacacsHostCtx = new AAAAddTacacsHostContext (pAAAGlobalAddTacacsHostMessage, index);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalServerMgmtWorker::addTacacsHostSTCCallback), pTacacsHostCtx);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pAddMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);

	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}
	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalServerMgmtWorker::addTacacsHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
    trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::addTacacsHostSTCCallback()");

	AAAAddTacacsHostContext *pTacacsHostCtx = (AAAAddTacacsHostContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalAddTacacsHostMessage *pAddMessage = (AAALocalAddTacacsHostMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalAddTacacsHostMessage *pAAAGlobalAddTacacsHostMessage = (AAAGlobalAddTacacsHostMessage *)pTacacsHostCtx->getPPrismMessage();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Adding tacacs host operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Adding tacacs host operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pTacacsHostCtx->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalServerMgmtWorker::addTacacsHostSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? TACACS_HOST_ADD_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? TACACS_HOST_ADD_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		TacacsHostManagedObject *pTacacsHostObj = dynamic_cast<TacacsHostManagedObject *>(createManagedObjectInstance(TacacsHostManagedObject::getClassName()));

		pTacacsHostObj->setHost(pAddMessage->getHost());
		pTacacsHostObj->setPort(pAddMessage->getPort());
		pTacacsHostObj->setProtocol((AuthProtocol)pAddMessage->getProtocol());
		pTacacsHostObj->setKey(pAddMessage->getKey());
		pTacacsHostObj->setRetries(pAddMessage->getRetries());
		pTacacsHostObj->setTimeout(pAddMessage->getTimeout());

		pTacacsHostObj->setIndex(pTacacsHostCtx->getIndex());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = TACACS_HOST_ADD_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Wave database commit is successful.");
		}
		else
		{
			status = TACACS_HOST_ADD_ERROR;
			trace (TRACE_LEVEL_ERROR, "Wave DB commit failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
		delete pTacacsHostObj;
	}

	delete pAddMessage;
	delete pTacacsHostCtx;
	delete pWaveSendToClusterContext;

	pAAAGlobalAddTacacsHostMessage->setMessageString(errorMessage);
	pAAAGlobalAddTacacsHostMessage->setCompletionStatus(status);
	reply (pAAAGlobalAddTacacsHostMessage);

	trace (TRACE_LEVEL_DEBUG, "AAAGlobalAddTacacsHostMessageHandler() - EXIT.");
}

void GlobalServerMgmtWorker::AAAGlobalChangeTacacsHostMessageHandler (AAAGlobalChangeTacacsHostMessage *pAAAGlobalChangeTacacsHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalServerMgmtWorker::AAAGlobalChangeTacacsHostMessageHandler()");
	if (pAAAGlobalChangeTacacsHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string host = pAAAGlobalChangeTacacsHostMessage->getHost();
	SI32 port = pAAAGlobalChangeTacacsHostMessage->getPort();
	SI8 protocol = pAAAGlobalChangeTacacsHostMessage->getProtocol();
	string key = pAAAGlobalChangeTacacsHostMessage->getKey();
	SI8 retries = pAAAGlobalChangeTacacsHostMessage->getRetries();
	SI8 timeout = pAAAGlobalChangeTacacsHostMessage->getTimeout();
	vector<UI32> locations;

	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	/* Get the Tacacs+ Host Managed Object for optional parameters */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(TacacsHostManagedObject::getClassName());
	synchronousQueryContext->addAndAttribute(new AttributeString(host, "host"));
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_ERROR, "Tac+ host does not exist");
		if (pResults != NULL) {
			for (UI32 i = 0; i < pResults->size(); i++)
				delete (*pResults)[i];

			delete pResults;
		}
		delete synchronousQueryContext;
		pAAAGlobalChangeTacacsHostMessage->setCompletionStatus (TACACS_HOST_DOESNT_EXIST);
		reply (pAAAGlobalChangeTacacsHostMessage);
		return;
	}
	TacacsHostManagedObject *pTacacsHostObj = dynamic_cast<TacacsHostManagedObject *> ((*pResults)[0]);
	if (NULL == pTacacsHostObj) {
		trace (TRACE_LEVEL_ERROR, "TACACS host does not exist");
		if (pResults != NULL) {
			for (UI32 i = 0; i < pResults->size(); i++)
				delete (*pResults)[i];

			delete pResults;
		}

		delete synchronousQueryContext;
		pAAAGlobalChangeTacacsHostMessage->setCompletionStatus (TACACS_HOST_DOESNT_EXIST);
		reply (pAAAGlobalChangeTacacsHostMessage);
		return;
	}

	delete pResults;
	delete synchronousQueryContext;

	if (((port == -1) || (port == pTacacsHostObj->getPort())) &&
		((protocol == -1) || (protocol == pTacacsHostObj->getProtocol())) &&
		((retries == -1) || (retries == pTacacsHostObj->getRetries())) &&
		((timeout == -1) || (timeout == pTacacsHostObj->getTimeout())) &&
		((key.length() <= 0) || (key.compare(pTacacsHostObj->getKey()) == 0)))
	{
		delete pTacacsHostObj;
		/* Last config replay also taken care */
		pAAAGlobalChangeTacacsHostMessage->setCompletionStatus(status);
		reply (pAAAGlobalChangeTacacsHostMessage);
		return;
	}

	/* Validate the user input parameters */
	if (port >= 0)
	{
		ret = radiusParamValidate(string(""), port, RADIUS_PORT);
		if (ret != 0) {
			status = TACACS_INVALID_PORT_NUMBER;
		}
	}
    else
    {
        port = pTacacsHostObj->getPort();
    }

	if (ret == 0)
	{
		if (protocol >= 0)
		{
			ret = radiusParamValidate(string(""), protocol, RADIUS_PROTOCOL);
			if (ret != 0)
			{
				status = TACACS_INVALID_PROTOCOL;
			}
		}
		else
		{
			protocol = pTacacsHostObj->getProtocol();
		}
	}

	if (ret == 0)
	{
		if (key.length() > 0)
		{
			ret = radiusParamValidate(key, 0, TACACS_KEY);
			if (ret != 0)
			{
				status = TACACS_INVALID_KEY;
			}
		}
		else
		{
			key = pTacacsHostObj->getKey();
 		}
	}

	if (ret == 0)
	{
		if (retries >= 0)
		{
			ret = radiusParamValidate(string(""), retries, RADIUS_RETRANSMIT);
			if (ret != 0)
			{
				status = TACACS_INVALID_RETRIES;
			}
		}
		else
		{
			retries = pTacacsHostObj->getRetries();
		}
	}

	if (ret == 0)
	{
		if (timeout >= 0)
		{
			ret = radiusParamValidate(string(""), timeout, RADIUS_TIMEOUT);
			if (ret != 0)
			{
				status = TACACS_INVALID_TIMEOUT;
			}
		}
		else
		{
			timeout = pTacacsHostObj->getTimeout();
		}
	}

	if (ret != 0)
	{
		delete pTacacsHostObj;
		pAAAGlobalChangeTacacsHostMessage->setCompletionStatus(status);
		reply (pAAAGlobalChangeTacacsHostMessage);
		return;
	}
	else
	{
		trace (TRACE_LEVEL_DEBUG, "User input validation is successful.");
	}
	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	/* Send updates to all nodes of the cluster for local FS sync */
	AAALocalChangeTacacsHostMessage *pSetMessage = new AAALocalChangeTacacsHostMessage (host, port, protocol, key, retries, timeout);

	AAAChangeTacacsHostContext *pTacacsHostCtx = new AAAChangeTacacsHostContext (pAAAGlobalChangeTacacsHostMessage, pTacacsHostObj);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalServerMgmtWorker::changeTacacsHostSTCCallback), pTacacsHostCtx);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pSetMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalServerMgmtWorker::changeTacacsHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
    trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::changeTacacsHostSTCCallback()");

	AAAChangeTacacsHostContext *pTacacsHostCtx = (AAAChangeTacacsHostContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalChangeTacacsHostMessage *pSetMessage = (AAALocalChangeTacacsHostMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalChangeTacacsHostMessage *pAAAGlobalChangeTacacsHostMessage = (AAAGlobalChangeTacacsHostMessage *)pTacacsHostCtx->getPPrismMessage();
	TacacsHostManagedObject *pTacacsHostObj = pTacacsHostCtx->getTacacsHostObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Change tacacs host operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Change tacacs host operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pTacacsHostCtx->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalServerMgmtWorker::changeTacacsHostSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? TACACS_HOST_CHANGE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? TACACS_HOST_CHANGE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		updateWaveManagedObject (pTacacsHostObj);

		pTacacsHostObj->setPort(pSetMessage->getPort());
		pTacacsHostObj->setProtocol((AuthProtocol)pSetMessage->getProtocol());
		pTacacsHostObj->setKey(pSetMessage->getKey());
		pTacacsHostObj->setRetries(pSetMessage->getRetries());
		pTacacsHostObj->setTimeout(pSetMessage->getTimeout());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = TACACS_HOST_CHANGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Wave database commit is successful.");
		}
		else
		{
			status = TACACS_HOST_CHANGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Wave DB commit failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}

	delete pTacacsHostObj;
	delete pSetMessage;
	delete pTacacsHostCtx;
	delete pWaveSendToClusterContext;

	pAAAGlobalChangeTacacsHostMessage->setMessageString(errorMessage);
	pAAAGlobalChangeTacacsHostMessage->setCompletionStatus(status);
	reply (pAAAGlobalChangeTacacsHostMessage);

	trace (TRACE_LEVEL_DEBUG, "AAAGlobalChangeTacacsHostMessageHandler() - EXIT.");
}

void GlobalServerMgmtWorker::AAAGlobalDeleteTacacsHostMessageHandler (AAAGlobalDeleteTacacsHostMessage *pAAAGlobalDeleteTacacsHostMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalServerMgmtWorker::AAAGlobalDeleteTacacsHostMessageHandler()");
	if (pAAAGlobalDeleteTacacsHostMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

    /* coverity[var_deref_model] */
    /* As pAAAGlobalDeleteTacacsModeMessage is NULL control will enter the
        if statment and will not come out due to prismAssert  */

	string host = pAAAGlobalDeleteTacacsHostMessage->getHost();
	vector<TacacsHostManagedObject *> *pTacacsHostsForUpdatingIndex = NULL;

	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	vector<UI32> locations;
	/* Get the host Managed Object instance */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(TacacsHostManagedObject::getClassName());
	synchronousQueryContext->addAndAttribute (new AttributeString (host, "host"));
	pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		trace (TRACE_LEVEL_ERROR, "Tac+ host does not exist");
		if (pResults != NULL) {
			for (UI32 i = 0; i < pResults->size(); i++)
				delete (*pResults)[i];

			delete pResults;
		}
		delete synchronousQueryContext;
		if (pAAAGlobalDeleteTacacsHostMessage->getIsALastConfigReplay() == true) {
			trace (TRACE_LEVEL_DEBUG, "pAAAGlobalDeleteTacacsHostMessage getIsALastConfigReplaytrue");
			pAAAGlobalDeleteTacacsHostMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		} else {
			pAAAGlobalDeleteTacacsHostMessage->setCompletionStatus (TACACS_HOST_DOESNT_EXIST);
		}
		reply (pAAAGlobalDeleteTacacsHostMessage);
		return;
	}
	TacacsHostManagedObject *pTacacsHostObj = dynamic_cast<TacacsHostManagedObject *> ((*pResults)[0]);

	if (NULL == pTacacsHostObj) {
		pAAAGlobalDeleteTacacsHostMessage->setCompletionStatus (TACACS_HOST_DOESNT_EXIST);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		reply (pAAAGlobalDeleteTacacsHostMessage);
		return;
	}
	delete pResults;
	delete synchronousQueryContext;

	trace (TRACE_LEVEL_INFO, "Tac+ host MO instance found in the database.");

	/* Check if it is the last TACACS+ server in the config database */
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(TacacsHostManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	if (pResults->size() == 1)
	{ /* Check if auth login is set to TACACS+ */
		WaveManagedObjectSynchronousQueryContext *syncQueryCtxtAuthL = new WaveManagedObjectSynchronousQueryContext(AuthLoginModeManagedObject::getClassName());
		vector<WaveManagedObject *> *pResultsAuthL = querySynchronously (syncQueryCtxtAuthL);
		if (pResultsAuthL == NULL || pResultsAuthL->size() != 1)
		{
			trace (TRACE_LEVEL_FATAL, "Auth login instance not found in DB");
			status = TACACS_HOST_DELETE_ERROR;
		}
		else
		{
			AuthLoginModeManagedObject *pAuthLoginObj = dynamic_cast<AuthLoginModeManagedObject *>((*pResultsAuthL)[0]);
			if (pAuthLoginObj == NULL) {
				status = TACACS_HOST_DELETE_ERROR;
			}
			else 
			{
			if (pAuthLoginObj->getFirst() == TACACS_DB ||
				pAuthLoginObj->getSecond() == TACACS_DB)
			{
				status = TACACS_HOST_LAST_AUTH_SERVER;
			}
		}
		}

		if (pResultsAuthL)
		{
			for (UI32 i = 0; i < pResultsAuthL->size(); i++)
				delete (*pResultsAuthL)[i];

			delete pResultsAuthL;
		}
		delete syncQueryCtxtAuthL;

		/* Check if Login Accounting is enbaled with tacacs+ server */
		WaveManagedObjectSynchronousQueryContext *syncQueryCtxtAccE = new WaveManagedObjectSynchronousQueryContext(ExecAccountingManagedObject::getClassName());
        vector<WaveManagedObject *> *pResultsAccE = querySynchronously (syncQueryCtxtAccE);
        if (pResultsAccE == NULL || pResultsAccE->size() != 1)
        {
            trace (TRACE_LEVEL_FATAL, "Exec Accounting instance not found in DB");
            status = TACACS_HOST_DELETE_ERROR;
        }
        else
        {
            ExecAccountingManagedObject *pExecAccObj = dynamic_cast<ExecAccountingManagedObject *>((*pResultsAccE)[0]);   
            if (pExecAccObj == NULL) {
                status = TACACS_HOST_DELETE_ERROR;
            }   
            else    
            {
                if (pExecAccObj->getAccSrvType() == TACACS_SRV)
                {
                    status = TACACS_HOST_LAST_ACC_SERVER;
                }
            }
        }

        if (pResultsAccE)
        {
            for (UI32 i = 0; i < pResultsAccE->size(); i++)
                delete (*pResultsAccE)[i];

            delete pResultsAccE;
        }
		delete syncQueryCtxtAccE;

		/* Check if Command accounting is enabled with tacacs+ server*/
        WaveManagedObjectSynchronousQueryContext *syncQueryCtxtAccC = new WaveManagedObjectSynchronousQueryContext(CmdAccountingManagedObject::getClassName());
        vector<WaveManagedObject *> *pResultsAccC = querySynchronously (syncQueryCtxtAccC);
        if (pResultsAccC == NULL || pResultsAccC->size() != 1)
        {
            trace (TRACE_LEVEL_FATAL, "Command Accounting instance not found in DB");
            status = TACACS_HOST_DELETE_ERROR;
        }
        else
        {
            CmdAccountingManagedObject *pCmdAccObj = dynamic_cast<CmdAccountingManagedObject *>((*pResultsAccC)[0]);
            if (pCmdAccObj == NULL) {
                status = TACACS_HOST_DELETE_ERROR;
            }
            else
            {
                if (pCmdAccObj->getAccSrvType() == TACACS_SRV)
                {
                    status = TACACS_HOST_LAST_ACC_SERVER;
                }
            }
        }

        if (pResultsAccC)
        {
            for (UI32 i = 0; i < pResultsAccC->size(); i++)
                delete (*pResultsAccC)[i];

            delete pResultsAccC;
        }
        delete syncQueryCtxtAccC;

        delete (*pResults)[0];
	}
	else if (pResults->size() > 1)
	{ /* Save them for updating their 'index' after deletion of this host */
		pTacacsHostsForUpdatingIndex = new vector<TacacsHostManagedObject *>;
		TacacsHostManagedObject *pTacHost = NULL;
		for (UI8 i = 0; i < pResults->size(); i++)
		{
			pTacHost = dynamic_cast<TacacsHostManagedObject *>((*pResults)[i]);
			if (pTacHost) {
			if (pTacHost->getHost() != host)
			{
				pTacacsHostsForUpdatingIndex->push_back(pTacHost);
			}
			else
			{
				delete pTacHost;
			}
		}
	}
	}
	delete pResults;
	delete synchronousQueryContext;

	if (status != WAVE_MESSAGE_SUCCESS)
	{
		delete pTacacsHostObj;
		pAAAGlobalDeleteTacacsHostMessage->setCompletionStatus (status);
		reply (pAAAGlobalDeleteTacacsHostMessage);
		return;
	}
	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	/* Send updates to all nodes of the cluster for local FS sync */
	AAALocalDeleteTacacsHostMessage *pDelMessage = new AAALocalDeleteTacacsHostMessage (host);

	AAADeleteTacacsHostContext *pTacacsHostCtx = new AAADeleteTacacsHostContext (pAAAGlobalDeleteTacacsHostMessage, pTacacsHostObj, pTacacsHostsForUpdatingIndex);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalServerMgmtWorker::deleteTacacsHostSTCCallback), pTacacsHostCtx);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pDelMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);

	trace (TRACE_LEVEL_DEBUG, "Sending message to all AAA local services.");
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}
	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalServerMgmtWorker::deleteTacacsHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
    trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::deleteTacacsHostSTCCallback()");

	AAADeleteTacacsHostContext *pTacacsHostCtx = (AAADeleteTacacsHostContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalDeleteTacacsHostMessage *pDelMessage = (AAALocalDeleteTacacsHostMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalDeleteTacacsHostMessage *pAAAGlobalDeleteTacacsHostMessage = (AAAGlobalDeleteTacacsHostMessage *)pTacacsHostCtx->getPPrismMessage();
	TacacsHostManagedObject *pTacacsHostObj = pTacacsHostCtx->getTacacsHostObj();
	vector<TacacsHostManagedObject *> *pTacacsHostsForUpdatingIndex = pTacacsHostCtx->getTacacsHostsForUpdatingIndex();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Delete tacacs host operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Delete tacacs host operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pTacacsHostCtx->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalServerMgmtWorker::deleteTacacsHostSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? TACACS_HOST_DELETE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? TACACS_HOST_DELETE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		/* Decrease the index for the hosts added later than the host being deleted now */
		if (NULL != pTacacsHostsForUpdatingIndex)
		{
			TacacsHostManagedObject *pTacacsHostForIndexChange = NULL;
			UI8 index = 0;
			for (UI8 i = 0; i < pTacacsHostsForUpdatingIndex->size(); i++)
			{
				pTacacsHostForIndexChange = (*pTacacsHostsForUpdatingIndex)[i];
				index = pTacacsHostForIndexChange->getIndex();
				if (index > pTacacsHostObj->getIndex())
				{
					updateWaveManagedObject (pTacacsHostForIndexChange);
					pTacacsHostForIndexChange->setIndex(--index);
				}
			}
		}

		/* Delete the specified Tacacs Host */
		delete pTacacsHostObj;

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = TACACS_HOST_DELETE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Wave database commit is successful.");
		}
		else
		{
			status = TACACS_HOST_DELETE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Wave DB commit failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}

	if (NULL != pTacacsHostsForUpdatingIndex)
	{
		for (UI8 i = 0; i < pTacacsHostsForUpdatingIndex->size(); i++)
		{
			delete (*pTacacsHostsForUpdatingIndex)[i];
		}
		delete pTacacsHostsForUpdatingIndex;
	}
	delete pDelMessage;
	delete pTacacsHostCtx;
	delete pWaveSendToClusterContext;

	pAAAGlobalDeleteTacacsHostMessage->setMessageString(errorMessage);
	pAAAGlobalDeleteTacacsHostMessage->setCompletionStatus(status);
	reply (pAAAGlobalDeleteTacacsHostMessage);

	trace (TRACE_LEVEL_DEBUG, "AAAGlobalDeleteTacacsHostMessageHandler() - EXIT.");
}
void  GlobalServerMgmtWorker::AAAGlobalChangeAccountingMessageHandler( AAAGlobalChangeAccountingMessage *pAAAGlobalChangeAccountingMessage)
{       
    trace (TRACE_LEVEL_DEBUG, "GlobalServerMgmtWorker::AAAGlobalChangeAccountingMessageHandler()");
    if (pAAAGlobalChangeAccountingMessage == NULL)
    {   
        trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
        prismAssert (false, __FILE__, __LINE__);
    }
    WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
    vector<WaveManagedObject *> *pResults = NULL;
    SI8 accType = -1;
    SI8 srvType = -1;
	vector<UI32> locations;

	srvType = pAAAGlobalChangeAccountingMessage->getServerType();
    if (EXEC_ACC == (accType = pAAAGlobalChangeAccountingMessage->getAccType())) {
        synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(ExecAccountingManagedObject::getClassName());
        pResults = querySynchronously (synchronousQueryContext);
		if (((NULL != pResults) && (pResults->size() == 1))) {            
			ExecAccountingManagedObject *pExecAccountingMO = dynamic_cast<ExecAccountingManagedObject *> ((*pResults)[0]);
			if (NULL == pExecAccountingMO) {
				trace (TRACE_LEVEL_INFO, "Faliled to retrive exec MO");
				pAAAGlobalChangeAccountingMessage->setCompletionStatus (ACC_UNKNOWN_ERROR);
				reply (pAAAGlobalChangeAccountingMessage);
				if (synchronousQueryContext) {
        		    delete synchronousQueryContext;
		            synchronousQueryContext = NULL;
		        }
		        if (pResults) {
		            delete pResults;
		            pResults = NULL;
		        }
				return;
			}
			if (pAAAGlobalChangeAccountingMessage->getIsALastConfigReplay() == true) {
				trace (TRACE_LEVEL_DEBUG, "pAAAGlobalChangeAccountingMessage getIsALastConfigReplay true");
				if (srvType == pExecAccountingMO->getAccSrvType()) {
					trace (TRACE_LEVEL_DEBUG, "pAAAGlobalChangeAccountingMessage::already configured");
					pAAAGlobalChangeAccountingMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
					reply (pAAAGlobalChangeAccountingMessage);
					if (pExecAccountingMO) {
						delete pExecAccountingMO;
						pExecAccountingMO = NULL;
					}
					if (synchronousQueryContext) {
	                    delete synchronousQueryContext;
	                    synchronousQueryContext = NULL;
	                }
	                if (pResults) {
	                    delete pResults;
	                    pResults = NULL;
	                }
	                return;
				}
			}
			if (pExecAccountingMO) {
                delete pExecAccountingMO;
                pExecAccountingMO = NULL;
            }
        }
    } else if (CMD_ACC == accType) {
        synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(CmdAccountingManagedObject::getClassName());
        pResults = querySynchronously (synchronousQueryContext);
		if (((NULL != pResults) && (pResults->size() == 1))) {
            CmdAccountingManagedObject *pCmdAccountingMO = dynamic_cast<CmdAccountingManagedObject *> ((*pResults)[0]);
			if (NULL == pCmdAccountingMO) {
                trace (TRACE_LEVEL_INFO, "Faliled to retrive command MO");
                pAAAGlobalChangeAccountingMessage->setCompletionStatus (ACC_UNKNOWN_ERROR);
                reply (pAAAGlobalChangeAccountingMessage);
				if (synchronousQueryContext) {
		            delete synchronousQueryContext;
		            synchronousQueryContext = NULL;
		        }
		        if (pResults) {
		            delete pResults;
		        	    pResults = NULL;
    		    }
				return;
			}
			if (pAAAGlobalChangeAccountingMessage->getIsALastConfigReplay() == true) {
				trace (TRACE_LEVEL_DEBUG, "pAAAGlobalChangeAccountingMessage getIsALastConfigReplay true");
				if (srvType == pCmdAccountingMO->getAccSrvType()) {
					pAAAGlobalChangeAccountingMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
					reply (pAAAGlobalChangeAccountingMessage);
					if (pCmdAccountingMO) {
						delete pCmdAccountingMO;
						pCmdAccountingMO = NULL;
		            }
					if (synchronousQueryContext) {
						delete synchronousQueryContext;
						synchronousQueryContext = NULL;
					}
					if (pResults) {
						delete pResults;
						pResults = NULL;
					}
					return;
                }
            }
			if (pCmdAccountingMO) {
                delete pCmdAccountingMO;
                pCmdAccountingMO = NULL;
            }
        }
    } else {
        trace (TRACE_LEVEL_FATAL, "Unknown Accounting type");
        accType = -1;
    }

    if ((NULL != pResults) && (pResults->size() == 0)) {
		trace (TRACE_LEVEL_INFO, "Accounting entry missing in DB");
        pAAAGlobalChangeAccountingMessage->setCompletionStatus (ACC_UNKNOWN_ERROR);
        reply (pAAAGlobalChangeAccountingMessage);
        if (synchronousQueryContext) {
            delete synchronousQueryContext;
            synchronousQueryContext = NULL;
        }
        if (pResults) {
            delete pResults;
            pResults = NULL;
        }
        return;
    }

    if (pResults) {
        delete pResults;
        pResults = NULL;
    }
    if (synchronousQueryContext) {
        delete synchronousQueryContext;
        synchronousQueryContext = NULL;
    }

    /* Chekc the existance of the server */
    if (TACACS_SRV == srvType) {
        string serverType;
        serverType = TacacsHostManagedObject::getClassName();
        synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(serverType);
        pResults = querySynchronously (synchronousQueryContext);
        if ((NULL == pResults) || (pResults->size() == 0)) {
             trace (TRACE_LEVEL_ERROR, string("No active ") + serverType + string(" server exists to support accounting."));
            if (synchronousQueryContext) {
                delete synchronousQueryContext;
                synchronousQueryContext = NULL;
            }
            if (pResults != NULL) {
                delete pResults;
                pResults = NULL;
            }
            pAAAGlobalChangeAccountingMessage->setCompletionStatus (ACC_NO_TACACS_SERVER);
            reply (pAAAGlobalChangeAccountingMessage);
            return;
        } else {
			if (synchronousQueryContext) {
                delete synchronousQueryContext;
                synchronousQueryContext = NULL;
            }
            for (UI32 i = 0; i < pResults->size(); i++)
                delete (*pResults)[i];
            delete pResults;
        }
    }

    /* Send message to all the nodes of the cluster */
    AAALocalChangeAccountingMessage *pChangeMessage = new AAALocalChangeAccountingMessage(accType, srvType);
	FrameworkToolKit::getFullyConnectedLocations(locations);
    locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

    if (EXEC_ACC == accType) {
		synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(ExecAccountingManagedObject::getClassName());
        pResults = querySynchronously (synchronousQueryContext);
        if ((NULL != pResults) && (pResults->size() == 0)) {
            pAAAGlobalChangeAccountingMessage->setCompletionStatus (ACC_UNKNOWN_ERROR);
            reply (pAAAGlobalChangeAccountingMessage);
            if (synchronousQueryContext) {
                delete synchronousQueryContext;
                synchronousQueryContext = NULL;
            }
            if (pResults) {
                delete pResults;
                pResults = NULL;
            }
            return;
        }
        ExecAccountingManagedObject *pExecAccountingMO = dynamic_cast<ExecAccountingManagedObject *> ((*pResults)[0]); 
        if (NULL == pExecAccountingMO) {
            trace (TRACE_LEVEL_DEBUG, "AAAGlobalChangeAccountingMessageHandler:Failed to get ExecAccountingManagedObject");
            pAAAGlobalChangeAccountingMessage->setCompletionStatus (ACC_LOGIN_ENABLE_ERROR);
            reply (pAAAGlobalChangeAccountingMessage);
			if (synchronousQueryContext) {
                delete synchronousQueryContext;
                synchronousQueryContext = NULL;
            }
            if (pResults) {
                delete pResults;
                pResults = NULL;
            }
            return;
        }
        AAAChangeExecAccountingContext* pChangeExecAccountingCtx = new AAAChangeExecAccountingContext(pAAAGlobalChangeAccountingMessage, pExecAccountingMO);
        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalServerMgmtWorker::changeExecAccountingSTCCallback), pChangeExecAccountingCtx);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pChangeMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}

        sendToWaveCluster (pWaveSendToClusterContext);
    } else if (CMD_ACC == accType) {
		synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(CmdAccountingManagedObject::getClassName());
            pResults = querySynchronously (synchronousQueryContext);
            if ((NULL != pResults) && (pResults->size() == 0)) {
                trace (TRACE_LEVEL_INFO, "Accounting is not enabled");
                pAAAGlobalChangeAccountingMessage->setCompletionStatus (ACC_UNKNOWN_ERROR);
                reply (pAAAGlobalChangeAccountingMessage);
                if (synchronousQueryContext) {
                    delete synchronousQueryContext;
                    synchronousQueryContext = NULL;
                }
                if (pResults) {
                    delete pResults;
                    pResults = NULL;
                }
                return;
            }
        CmdAccountingManagedObject *pCmdAccountingMO = dynamic_cast<CmdAccountingManagedObject*> ((*pResults)[0]);
        if (NULL == pCmdAccountingMO) {
            trace (TRACE_LEVEL_DEBUG, "AAAGlobalChangeAccountingMessageHandler:Failed to get CmdAccountingManagedObject");
            pAAAGlobalChangeAccountingMessage->setCompletionStatus (ACC_CMD_ENABLE_ERROR);
            reply (pAAAGlobalChangeAccountingMessage);
			if (synchronousQueryContext) {
                delete synchronousQueryContext;
                synchronousQueryContext = NULL;
            }
            if (pResults) {
                delete pResults;
                pResults = NULL;
            }
            return;
        }
        AAAChangeCmdAccountingContext* pChangeCmdAccountingCtx = new AAAChangeCmdAccountingContext(pAAAGlobalChangeAccountingMessage, pCmdAccountingMO);
        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalServerMgmtWorker::changeCmdAccountingSTCCallback), pChangeCmdAccountingCtx);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pChangeMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
            pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
        }

        sendToWaveCluster (pWaveSendToClusterContext);
    }
    
}
void GlobalServerMgmtWorker::changeExecAccountingSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)          
{      
    trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::changeExecAccountingSTCCallback()");
    AAAChangeExecAccountingContext *pAAAContext = (AAAChangeExecAccountingContext *)pWaveSendToClusterContext->getPCallerContext();
    AAALocalChangeAccountingMessage *pChangeMessage= (AAALocalChangeAccountingMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
    ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
    string errorMessage;
        
    AAAGlobalChangeAccountingMessage *pAAAGlobalChangeAccountingMessage = (AAAGlobalChangeAccountingMessage *)pAAAContext->getPPrismMessage();
        
    ExecAccountingManagedObject *pExecAccMO = pAAAContext->getExecAccObj();
        
    if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
            status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES) {
        if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
            trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - ChangingLogin Accounting failed on one or more of the connected nodes.");
        else
            trace (TRACE_LEVEL_ERROR, "FAILURE - Changing Login Accounting operation failed on all nodes");

        vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
        // FrameworkToolKit::getFullyConnectedLocations(cxLocations);
        if (cxLocations.size() > 0) {
            pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalServerMgmtWorker::changeExecAccountingSTCCallback", errorMessage);
        } else {
            trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
            LocationId thisNode = FrameworkToolKit::getThisLocationId();
            status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? ACC_LOGIN_ENABLE_ERROR :
                (pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? ACC_LOGIN_ENABLE_ERROR :
                pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
        }
    }

    if (WAVE_MESSAGE_SUCCESS == status) {
		trace (TRACE_LEVEL_INFO, string("Callback MO Login accounting :") +(AccServerType)pExecAccMO->getAccSrvType());
        startTransaction ();

		updateWaveManagedObject (pExecAccMO);
        pExecAccMO->setAccSrvType((AccServerType)pChangeMessage->getServerType());

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS == status) {
            status = WAVE_MESSAGE_SUCCESS;  //status =
            trace (TRACE_LEVEL_INFO, "Commiting Login Acc configuration to Wave database is successful.");
        } else {
            status = ACC_LOGIN_ENABLE_ERROR;
            trace (TRACE_LEVEL_ERROR, "Commiting ogin Acc configuration failed. Framework error - " + FrameworkToolKit::localize (status));
            if (errorMessage.length() > 0)
                errorMessage += string("\n") + FrameworkToolKit::localize (status);
        }
    }

	delete pExecAccMO;
    delete pChangeMessage;
    delete pAAAContext;
    delete pWaveSendToClusterContext;

    pAAAGlobalChangeAccountingMessage->setMessageString(errorMessage);
    pAAAGlobalChangeAccountingMessage->setCompletionStatus(status);
    reply (pAAAGlobalChangeAccountingMessage);
}

void GlobalServerMgmtWorker::changeCmdAccountingSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
    trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::changeCmdAccountingSTCCallback()");

    AAAChangeCmdAccountingContext *pAAAContext = (AAAChangeCmdAccountingContext *)pWaveSendToClusterContext->getPCallerContext();
    AAALocalChangeAccountingMessage *pChangeMessage= (AAALocalChangeAccountingMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
    ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
    string errorMessage;

    AAAGlobalChangeAccountingMessage *pAAAGlobalChangeAccountingMessage = (AAAGlobalChangeAccountingMessage *)pAAAContext->getPPrismMessage();

    CmdAccountingManagedObject *pCmdAccMO = pAAAContext->getCmdAccObj();

    if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
            status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES) {
        if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
            trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Changing Cmd Accounting failed on one or more of the connected nodes.");
        else
            trace (TRACE_LEVEL_ERROR, "FAILURE - Changing Cmd Accounting operation failed on all nodes");

        vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
        // FrameworkToolKit::getFullyConnectedLocations(cxLocations);
        if (cxLocations.size() > 0) {
            pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalServerMgmtWorker::changeCmdAccountingSTCCallback", errorMessage);
        } else {            trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
            LocationId thisNode = FrameworkToolKit::getThisLocationId();
            status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? ACC_CMD_ENABLE_ERROR :
                    (pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? ACC_CMD_ENABLE_ERROR :
                pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
        }
    }

    if (WAVE_MESSAGE_SUCCESS == status) {
        trace (TRACE_LEVEL_INFO, string("Callback MO Cmd accounting :") +(AccServerType)pCmdAccMO->getAccSrvType());
        startTransaction ();

		updateWaveManagedObject (pCmdAccMO);
        pCmdAccMO->setAccSrvType((AccServerType)pChangeMessage->getServerType());

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS == status) {
            status = WAVE_MESSAGE_SUCCESS;  //status =
            trace (TRACE_LEVEL_INFO, "Commiting Cmd Acc configuration to Wave database is successful.");
        } else {
            status = ACC_CMD_ENABLE_ERROR;
            trace (TRACE_LEVEL_ERROR, "Commiting ogin Acc configuration failed. Framework error - " + FrameworkToolKit::localize (status));
            if (errorMessage.length() > 0)
                errorMessage += string("\n") + FrameworkToolKit::localize (status);
        }
    }

	delete pCmdAccMO;
    delete pChangeMessage;
    delete pAAAContext;
    delete pWaveSendToClusterContext;

    pAAAGlobalChangeAccountingMessage->setMessageString(errorMessage);
    pAAAGlobalChangeAccountingMessage->setCompletionStatus(status);
    reply (pAAAGlobalChangeAccountingMessage);
}
void  GlobalServerMgmtWorker::AAAGlobalChangeAuthLoginModeMessageHandler( AAAGlobalChangeAuthLoginModeMessage *pAAAGlobalChangeAuthLoginModeMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalServerMgmtWorker::AAAGlobalChangeAuthLoginModeMessageHandler()");
	if (pAAAGlobalChangeAuthLoginModeMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	vector<UI32> locations;

	/* Retrieve the Auth Login Mode Managed Object */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(AuthLoginModeManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		if ((pResults == NULL) || (pResults->size() == 0)) trace (TRACE_LEVEL_FATAL, "Zero AuthLoginMode instances found.");
		else if (pResults->size() != 1) trace (TRACE_LEVEL_FATAL, "More than one AuthLoginMode instances found.");
		trace (TRACE_LEVEL_FATAL, "Unexpected number of AuthLoginMode instances");
		pAAAGlobalChangeAuthLoginModeMessage->setCompletionStatus (AUTH_LOGIN_MODE_SET_ERROR);
		reply (pAAAGlobalChangeAuthLoginModeMessage);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		return;
	}

	AuthLoginModeManagedObject *pAuthLoginModeObj = dynamic_cast<AuthLoginModeManagedObject *> ((*pResults)[0]);
	if (NULL == pAuthLoginModeObj) {
		pAAAGlobalChangeAuthLoginModeMessage->setCompletionStatus (AUTH_LOGIN_MODE_SET_ERROR);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		reply (pAAAGlobalChangeAuthLoginModeMessage);
		return;
	}

	delete pResults;
	delete synchronousQueryContext;

	/* Validate user input */
	/* coverity[var_deref_model] */
	/* As pAAAGlobalChangeAuthLoginModeMessage is NULL control will enter the
		if statment and will not come out due to prismAssert  */
	SI8 primary = pAAAGlobalChangeAuthLoginModeMessage->getFirst();
	SI8 secondary = pAAAGlobalChangeAuthLoginModeMessage->getSecond();

	AAAServerType first = NO_AAA, second = NO_AAA;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	if (primary == brcd_aaa_default)
	{
		if (secondary >= 0)
		{
			status = AUTH_LOGIN_DEFAULT_SECONDARY;
		}
		else
		{
			first = LOCAL_DB;
		}
	}
	else
	{
		if (primary == -1) // Only secondary mode is changed
		{
			first = pAuthLoginModeObj->getFirst();
		}
		else if (primary != brcd_aaa_local &&
			primary != brcd_aaa_radius &&
			primary != brcd_aaa_tacacs0x2b &&
			primary != brcd_aaa_ldap)
		{
			status = AUTH_LOGIN_INVALID_MODE;
		}
		else
		{
			first = (AAAServerType)primary;
		}
	}

	if (status == WAVE_MESSAGE_SUCCESS)
	{
		if (secondary == -1)
		{
			second = NO_AAA; // Secondary is optional and will be none if unspecified
		}
		else if (secondary == brcd_aaa_local)
		{
			second = LOCAL_DB;
		}
		else if (secondary == brcd_aaa_local_auth_fallback)
		{
			second = LOCALBKUP_DB;
		}
		else
		{
			status = AUTH_LOGIN_INVALID_MODE;
		}
	}

	if (status != WAVE_MESSAGE_SUCCESS)
	{
		delete pAuthLoginModeObj;
		pAAAGlobalChangeAuthLoginModeMessage->setCompletionStatus (status);
		reply (pAAAGlobalChangeAuthLoginModeMessage);
		return;
	}

	trace (TRACE_LEVEL_DEBUG, "User input validation is successful.");

	/* Check if the values are unchanged */
	AAAServerType oldFirst = pAuthLoginModeObj->getFirst();
	AAAServerType oldSecond = pAuthLoginModeObj->getSecond();
	if ((first == oldFirst) && (second == oldSecond))
	{
		trace (TRACE_LEVEL_INFO, "Auth mode is unchanged.");
		delete pAuthLoginModeObj;
		/* Last cofnig replay also handled here */
		pAAAGlobalChangeAuthLoginModeMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		reply (pAAAGlobalChangeAuthLoginModeMessage);
		return;
	}

	/* Check if Server configuration exists for the respective mode */
	if (first == RADIUS_DB || first == TACACS_DB || first == LDAP_DB)
	{
		string serverType;	
		if (first == RADIUS_DB) {
			serverType = RadiusHostManagedObject::getClassName();
		} else if (first == TACACS_DB) {
			serverType = TacacsHostManagedObject::getClassName();
		} else {
			serverType = LDAPHostManagedObject::getClassName();
		}
		synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(serverType);
		pResults = querySynchronously (synchronousQueryContext);
		delete synchronousQueryContext;
		if ((NULL == pResults) || (pResults->size() == 0))
		{
			trace (TRACE_LEVEL_ERROR, string("No active ") + serverType + string(" server exists to support the auth mode."));
			delete pAuthLoginModeObj;
			if (pResults != NULL) delete pResults;
			if (first == RADIUS_DB) {
				pAAAGlobalChangeAuthLoginModeMessage->setCompletionStatus (AUTH_LOGIN_NO_RADIUS);
			} else if (first == TACACS_DB) {
				pAAAGlobalChangeAuthLoginModeMessage->setCompletionStatus (AUTH_LOGIN_NO_TACACS);
			} else {
				pAAAGlobalChangeAuthLoginModeMessage->setCompletionStatus (AUTH_LOGIN_NO_LDAP);
			}
			reply (pAAAGlobalChangeAuthLoginModeMessage);
			return;
		}
		else
		{
			for (UI32 i = 0; i < pResults->size(); i++)
				delete (*pResults)[i];
			delete pResults;
		}
        /* Commented the check condition below during local auth fallback checkin
           to Leo as second was getting reset to LOCAL_DB */
#if 0
		if (oldSecond == LOCAL_DB) {
			second = LOCAL_DB;
		}
#endif
	}

	/* Calculate the backend modes from the primary and secondary source values */
	UI8 authMode;
	bool radMode, tacMode, logout, ldapMode;

	logout = pAAAGlobalChangeAuthLoginModeMessage->getIsLogout();
    int ret = getAuthModes(first, second, &authMode, (void *)&radMode, (void *)&tacMode, (void *)&ldapMode);

	if (ret != 0)
	{
		trace (TRACE_LEVEL_ERROR, "Obtaining the auth-mode from backend failed.");
		delete pAuthLoginModeObj;
		pAAAGlobalChangeAuthLoginModeMessage->setCompletionStatus (AUTH_LOGIN_INVALID_MODE); // Charan - define a new error code
		reply (pAAAGlobalChangeAuthLoginModeMessage);
		return;
	}

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	/* Send updates to all nodes of the cluster for local FS sync */
	AAALocalChangeAuthLoginModeMessage *pSetMessage = new AAALocalChangeAuthLoginModeMessage (authMode, radMode, tacMode, ldapMode, logout);
	AAAAuthLoginModeContext *pAuthLoginModeCtx = new AAAAuthLoginModeContext (pAAAGlobalChangeAuthLoginModeMessage, pAuthLoginModeObj, first, second);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalServerMgmtWorker::changeAuthLoginModeSTCCallback), pAuthLoginModeCtx);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pSetMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::AAAGlobalChangeAuthLoginModeMessageHandler() Exit");
	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalServerMgmtWorker::changeAuthLoginModeSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
    trace (TRACE_LEVEL_INFO, "GlobalServerMgmtWorker::changeAuthLoginModeSTCCallback()");

	AAAAuthLoginModeContext *pAuthLoginModeCtx = (AAAAuthLoginModeContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalChangeAuthLoginModeMessage *pSetMessage = (AAALocalChangeAuthLoginModeMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalChangeAuthLoginModeMessage *pAAAGlobalChangeAuthLoginModeMessage = (AAAGlobalChangeAuthLoginModeMessage *)pAuthLoginModeCtx->getPPrismMessage();
	AuthLoginModeManagedObject *pAuthLoginModeObj = pAuthLoginModeCtx->getAuthLoginModeObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Adding radius host operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Adding radius host operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAuthLoginModeCtx->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalServerMgmtWorker::addRadiusHostSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? AUTH_LOGIN_MODE_SET_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? AUTH_LOGIN_MODE_SET_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		updateWaveManagedObject (pAuthLoginModeObj);

		pAuthLoginModeObj->setFirst(pAuthLoginModeCtx->getFirst());
		pAuthLoginModeObj->setSecond(pAuthLoginModeCtx->getSecond());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = AUTH_LOGIN_MODE_CHANGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting Auth Login mode to Wave database is successful.");
		}
		else
		{
			status = AUTH_LOGIN_MODE_SET_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting Auth Login mode failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
    }

	delete pAuthLoginModeObj;
	delete pSetMessage;
	delete pAuthLoginModeCtx;
	delete pWaveSendToClusterContext;

	pAAAGlobalChangeAuthLoginModeMessage->setMessageString(errorMessage);
	pAAAGlobalChangeAuthLoginModeMessage->setCompletionStatus(status);
	reply (pAAAGlobalChangeAuthLoginModeMessage);

	trace (TRACE_LEVEL_DEBUG, "AAAGlobalChangeAuthLoginModeMessageHandler() - EXIT.");
}

int GlobalServerMgmtWorker::radiusParamValidate(const string &strParam, const UI32 &intParam, const radiusParamType &type)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalServerMgmtWorker::radiusParamValidate()");
	char debugInfo[10];
	int ret = 0;

	snprintf(debugInfo, sizeof(debugInfo), "%d", type);
	trace (TRACE_LEVEL_DEBUG, "Validating the parameter of type " + string(debugInfo));

	switch (type)
	{
		case RADIUS_HOST:
		{
			int len = strParam.length();
			char *hname = const_cast<char*>(strParam.c_str());

			if ((strchr(hname, ':')) || ((strchr(hname, '.')) && isdigit(hname[0]))) {
				/* check for valid IP v4/v6 Addr */
				char addr[16];
				if (!(inet_pton(AF_INET, hname, (void *) &addr) || inet_pton(AF_INET6, hname, (void *)&addr))) {
					ret = -1;
					trace (TRACE_LEVEL_ERROR, "Not a valid IP Address.");
				}
			} else if ((strspn(hname,"0123456789") == strlen(hname)) || strchr(hname, ' ')) {
				/* Throw error if it's neither hostname/ipaddr or if it contains space */
				ret = -1;
				trace (TRACE_LEVEL_ERROR, "The host name is neither an IP address nor a valid domain name");
			} else {
				/* check for valid DNS name */
				// struct addrinfo hints, *res;

				for (int i = 0; i < len; i++) {
					if (!(isalnum(hname[i]))) {
						if (((hname[i] != '.') && (hname[i] != '-') && (hname[i] != '_'))) {
							ret = -1;
							trace (TRACE_LEVEL_ERROR, "Invalid host domain name.");
						}
					}
				}
#if 0
				if (ret != -1) {
					hints.ai_family = PF_UNSPEC;
					hints.ai_socktype = SOCK_DGRAM;
					hints.ai_flags = AI_PASSIVE;
					hints.ai_protocol = 0;
					//ret = getaddrinfo(hname, NULL, &hints, &res); // Charan
					ret = getaddrinfo(hname, NULL, NULL, &res);
					if (ret == 0)
						freeaddrinfo(res);
					else
						trace (TRACE_LEVEL_ERROR, "Unable to resolve host domain name");
                }
#endif
            }
		}
			break;

		case RADIUS_PORT:
			if (!(intParam >= 0 && intParam <= 65535)) {
				ret = -1;
				trace (TRACE_LEVEL_ERROR, "Invalid port number.");
			}
			break;

		case RADIUS_PROTOCOL:
			if ((intParam != RADIUS_CHAP_PROTO) &&
				(intParam != RADIUS_PAP_PROTO) &&
				(intParam != RADIUS_PEAP_PROTO))
			{
				ret = -1;
				trace (TRACE_LEVEL_ERROR, "Invalid authentication protocol.");
			}

			/* if (secFipsModeGet() && (pradconf->proto != RADIUS_PEAP_PROTO)) {
				printf("Protocol %s not supported in FIPS mode\n");
				return (RADIUS_INVALID_PROTO_FIPS);
			} */

			break;

		case RADIUS_KEY:
		{
			int len = strParam.length();
			int index = 0;

			for (index = 0; (index < len) && isprint(strParam.at(index)); index++);

			if (len < MIN_RADIUS_KEY_LEN || len > MAX_RADIUS_KEY_LEN || index < len) {
				ret = -1;
				trace (TRACE_LEVEL_ERROR, "Invalid key. !(>=8 and <=40).");
			}
			else if (strParam.find(" ") != string::npos)
			{
				ret = -1;
				trace (TRACE_LEVEL_ERROR, "Key cannot contain spaces.");
			}
		}
			break;

		
		case TACACS_KEY:
		{
			int len = strParam.length();
			int index = 0;

			for (index = 0; (index < len) && isprint(strParam.at(index)); index++);

			if (len < MIN_TACACS_KEY_LEN || len > MAX_RADIUS_KEY_LEN || index < len) {
				ret = -1;
				trace (TRACE_LEVEL_ERROR, "Invalid key. !(>=1 and <=40).");
			}
			else if (strParam.find(" ") != string::npos)
			{
				ret = -1;
				trace (TRACE_LEVEL_ERROR, "Key cannot contain spaces.");
			}
			break;
		}

		case RADIUS_RETRANSMIT:
			if (!(intParam >= MIN_RADIUS_RETRANS && intParam <= MAX_RADIUS_RETRANS))
			{
				ret = -1;
				trace (TRACE_LEVEL_ERROR, "Invalid retransmit times. !(>=0 and <=100).");
			}
			break;

		case RADIUS_TIMEOUT:
			if (intParam < RADIUS_MIN_TIMEOUT || intParam > RADIUS_MAX_TIMEOUT)
			{
				ret = -1;
				trace (TRACE_LEVEL_ERROR, "Invalid timeout. !(>=1 and <=60).");
			}
			break;

		case RADIUS_DEADTIME:
			if (intParam < MIN_RADIUS_DEADTIME || intParam > MAX_RADIUS_DEADTIME)
			{
				ret = -1;
				trace (TRACE_LEVEL_ERROR, "Invalid deadtime. !(>=1 and <=2880).");
			}
			break;

		default:
			trace (TRACE_LEVEL_ERROR, "Unknown radius parameter type");
			ret = -1;
			break;
	}

	return (ret);
}

int GlobalServerMgmtWorker::getAuthModes(const AAAServerType &first, const AAAServerType &second, void *authMode, void *radMode, void *tacMode, void *ldapMode)
{
	if (!authMode || !radMode || !tacMode || !ldapMode) {
		return (-1);
	}

	/* logout is always true. Not removing itall over the code to make minimal changes */
	// *((bool *)logout) = true;

	/* Now computer the first and second sources for the backend */
	if (first == LOCAL_DB) {
		if (second != NO_AAA) {
			return (-1);
		} else {
			*((UI8 *)authMode) = LOCAL_ONLY;
			*((bool *)radMode) = false;
			*((bool *)tacMode) = false;
			*((bool *)ldapMode) = false;
		}
	} else if (first == RADIUS_DB) {
		if (second == NO_AAA) {
			*((UI8 *)authMode) = RADIUS_ONLY;
		} else if (second == LOCAL_DB) {
			*((UI8 *)authMode) = RADIUS_LOCLBKUP;
		} else if (second == LOCALBKUP_DB) {
			*((UI8 *)authMode) = RADIUS_LOCAL;
		} else {
			return (-1);
		}
		*((bool *)radMode) = true;
		*((bool *)tacMode) = false;
		*((bool *)ldapMode) = false;
	} else if (first == TACACS_DB) {
		if (second == NO_AAA) {
			*((UI8 *)authMode) = TACACS_ONLY;
		} else if (second == LOCAL_DB) {
			*((UI8 *)authMode) = TACACS_LOCLBKUP;
		} else if (second == LOCALBKUP_DB) {
			*((UI8 *)authMode) = TACACS_LOCAL;
		} else {
			return (-1);
		}
		*((bool *)radMode) = false;
		*((bool *)tacMode) = true;
		*((bool *)ldapMode) = false;
	} else if (first == LDAP_DB) {
		if (second == NO_AAA) {
			*((UI8 *)authMode) = LDAP_ONLY;
		} else if (second == LOCAL_DB) {
			*((UI8 *)authMode) = LDAP_LOCLBKUP;
		} else if (second == LOCALBKUP_DB) {
			*((UI8 *)authMode) = LDAP_LOCAL;
		} else {
			return (-1);
		}
		*((bool *)radMode) = false;
		*((bool *)tacMode) = false;
		*((bool *)ldapMode) = true;
	} else {
		return (-1);
	}

	return (0);
}

}
