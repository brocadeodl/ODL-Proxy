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
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "DcmResourceIdEnums.h"

#include "AAA/Global/AAACrypto.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/GlobalDiscreteObjsWorker.h"
#include "AAA/Global/GlobalAuthenticationWorker.h"
#include "AAA/Global/AAAGlobalSetPasswordAttributesMessage.h"
#include "AAA/Global/AAAGlobalSetBannerMessage.h"
#include "AAA/Global/AAAGlobalSetPasswordEncryptionMessage.h"
#include "AAA/Global/PasswordAttributesManagedObject.h"
#include "AAA/Global/BannerManagedObject.h"
#include "AAA/Global/PasswordEncryptionManagedObject.h"
#include "AAA/Global/UserManagedObject.h"
#include "AAA/Global/MapRoleManagedObject.h"
#include "AAA/Global/SecServicesGlobalManagedObject.h"
#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Local/AAALocalSetPasswordAttributesMessage.h"
#include "AAA/Local/AAALocalSetBannerMessage.h"
#include "AAA/Global/AAASequencerContexts.h"
#include "AAA/Global/AAAGlobalAddMapRoleMessage.h"
#include "AAA/Global/AAAGlobalChangeMapRoleMessage.h"
#include "AAA/Global/AAAGlobalDeleteMapRoleMessage.h"
#include "AAA/Global/AAAGlobalCertutilOpActionMessage.h"
#include "AAA/Local/AAALocalAddMapRoleMessage.h"
#include "AAA/Local/AAALocalChangeMapRoleMessage.h"
#include "AAA/Local/AAALocalDeleteMapRoleMessage.h"
#include "AAA/Local/CertutilOpActionMessage.h"
#include "AAA/Global/SecServicesGlobalConfigMessage.h"
#include "AAA/Local/SecServicesLocalConfigMessage.h"

extern "C" {
	#include "security/public.h"
}

namespace DcmNs
{

GlobalDiscreteObjsWorker::GlobalDiscreteObjsWorker ( AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: WaveWorker  (pAAAGlobalObjectManager)
{
	PasswordAttributesManagedObject    passwordAttributesObj    (pAAAGlobalObjectManager);
	passwordAttributesObj.setupOrm ();
	addManagedClass (PasswordAttributesManagedObject::getClassName (), this);

	BannerManagedObject bannerMO (pAAAGlobalObjectManager);
	bannerMO.setupOrm();
	addManagedClass (BannerManagedObject::getClassName (), this);

	PasswordEncryptionManagedObject paswdEncrptnMO (pAAAGlobalObjectManager);
	paswdEncrptnMO.setupOrm ();
	addManagedClass (PasswordEncryptionManagedObject::getClassName (), this);
	
	MapRoleManagedObject mapRoleMO (pAAAGlobalObjectManager);
	mapRoleMO.setupOrm ();
	addManagedClass (MapRoleManagedObject::getClassName (), this);

#if 0
	TelnetServerGlobalManagedObject telnetServerGlobalMO (pAAAGlobalObjectManager);
	telnetServerGlobalMO.setupOrm ();
	addManagedClass (TelnetServerGlobalManagedObject::getClassName (), this);
	
	SSHServerGlobalManagedObject sshServerGlobalMO (pAAAGlobalObjectManager);
	sshServerGlobalMO.setupOrm ();
	addManagedClass (SSHServerGlobalManagedObject::getClassName (), this);

	SSHServerListManagedObject sshServerListMO (pAAAGlobalObjectManager);
	sshServerListMO.setupOrm ();
	addManagedClass (SSHServerListManagedObject::getClassName (), this);

#endif
	addOperationMap (AAAGLOBALSETPASSWORDATTRIBUTES, reinterpret_cast<PrismMessageHandler> (&GlobalDiscreteObjsWorker::AAAGlobalSetPasswordAttributesMessageHandler));
	addOperationMap (AAAGLOBALSETBANNER, reinterpret_cast<PrismMessageHandler> (&GlobalDiscreteObjsWorker::AAAGlobalSetBannerMessageHandler));
	addOperationMap (AAAGLOBALSETPASSWORDENCRYPTION, reinterpret_cast<PrismMessageHandler> (&GlobalDiscreteObjsWorker::AAAGlobalSetPasswordEncryptionMessageHandler));
	addOperationMap (AAAGLOBALADDMAPROLE, reinterpret_cast<PrismMessageHandler> (&GlobalDiscreteObjsWorker::AAAGlobalAddMapRoleMessageHandler));
	addOperationMap (AAAGLOBALCHANGEMAPROLE, reinterpret_cast<PrismMessageHandler> (&GlobalDiscreteObjsWorker::AAAGlobalChangeMapRoleMessageHandler));
	addOperationMap (AAAGLOBALDELETEMAPROLE, reinterpret_cast<PrismMessageHandler> (&GlobalDiscreteObjsWorker::AAAGlobalDeleteMapRoleMessageHandler));
	addOperationMap (AAAGLOBALCERTUTILOPACTION, reinterpret_cast<PrismMessageHandler> (&GlobalDiscreteObjsWorker::AAAGlobalCertutilOpActionMessageHandler));
#if 0
	addOperationMap (TELNETSERVERGLOBALCONFIG, reinterpret_cast<PrismMessageHandler> (&GlobalDiscreteObjsWorker::TelnetServerGlobalConfigMessageHandler));
	addOperationMap (SSHSERVERGLOBALCONFIG, reinterpret_cast<PrismMessageHandler> (&GlobalDiscreteObjsWorker::SSHServerGlobalConfigMessageHandler));
	addOperationMap (SSHSERVERLISTGLOBALADD, reinterpret_cast<PrismMessageHandler> (&GlobalDiscreteObjsWorker::SSHServerListGlobalAddMessageHandler));
	addOperationMap (SSHSERVERLISTGLOBALDELETE, reinterpret_cast<PrismMessageHandler> (&GlobalDiscreteObjsWorker::SSHServerListGlobalDeleteMessageHandler));
#endif
}

GlobalDiscreteObjsWorker::~GlobalDiscreteObjsWorker ()
{
}

PrismMessage  *GlobalDiscreteObjsWorker::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{
		case AAAGLOBALSETPASSWORDATTRIBUTES :
			pPrismMessage = new AAAGlobalSetPasswordAttributesMessage ();
			break;
		case AAAGLOBALSETBANNER :
			pPrismMessage = new AAAGlobalSetBannerMessage ();
			break;
		case AAAGLOBALSETPASSWORDENCRYPTION :
			pPrismMessage = new AAAGlobalSetPasswordEncryptionMessage ();
			break;
		case AAAGLOBALADDMAPROLE :
			pPrismMessage = new AAAGlobalAddMapRoleMessage ();
			break;
		case AAAGLOBALCHANGEMAPROLE :
			pPrismMessage = new AAAGlobalChangeMapRoleMessage ();
			break;
		case AAAGLOBALDELETEMAPROLE :
			pPrismMessage = new AAAGlobalDeleteMapRoleMessage ();
			break;
		case AAAGLOBALCERTUTILOPACTION :
			pPrismMessage = new AAAGlobalCertutilOpActionMessage ();
			break;
#if 0
		case TELNETSERVERGLOBALCONFIG :
			pPrismMessage = new TelnetServerGlobalConfigMessage ();
			break;
		case SSHSERVERGLOBALCONFIG :
			pPrismMessage = new SSHServerGlobalConfigMessage ();
			break;
		case SSHSERVERLISTGLOBALADD :
			pPrismMessage = new SSHServerListGlobalAddMessage ();
			break;
		case SSHSERVERLISTGLOBALDELETE :
			pPrismMessage = new SSHServerListGlobalDeleteMessage ();
			break;
#endif
		default :
			pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject  *GlobalDiscreteObjsWorker::createManagedObjectInstance(const string &managedClassName)
{
	WaveManagedObject *pWaveManagedObject = NULL;

	if ((PasswordAttributesManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new PasswordAttributesManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((BannerManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new BannerManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((PasswordEncryptionManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new PasswordEncryptionManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((MapRoleManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new MapRoleManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
#if 0
	else if ((TelnetServerGlobalManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new TelnetServerGlobalManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((SSHServerGlobalManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new SSHServerGlobalManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((SSHServerListManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new SSHServerListManagedObject(dynamic_cast<AAAGlobalObjectManager *>(getPWaveObjectManager()));
	}
#endif
	else
	{
		trace (TRACE_LEVEL_FATAL, "GlobalDiscreteObjsWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
	}

	return (pWaveManagedObject);
}

void  GlobalDiscreteObjsWorker::AAAGlobalSetBannerMessageHandler( AAAGlobalSetBannerMessage *pAAAGlobalSetBannerMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::AAAGlobalSetBannerMessageHandler()");
	if (pAAAGlobalSetBannerMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}
	vector<UI32> locations;

	/* Retrieve the Banner Managed Object */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(BannerManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		if ((pResults == NULL) || (pResults->size() == 0)) trace (TRACE_LEVEL_FATAL, "Zero Banner instances found.");
		else if (pResults->size() > 1) trace (TRACE_LEVEL_FATAL, "More than one Banner instances found.");
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (unsigned int j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		pAAAGlobalSetBannerMessage->setCompletionStatus (BANNER_SET_ERROR);
		reply (pAAAGlobalSetBannerMessage);
		return;
	}

	BannerManagedObject *pBannerManagedObj = dynamic_cast<BannerManagedObject *> ((*pResults)[0]);

	// Read the input parameters
	string banner;
	UI8 bannerType = pAAAGlobalSetBannerMessage->getBannerType();

	if (pAAAGlobalSetBannerMessage->getReset() == true)
	{
		banner = "";
	}
	else
	{
		banner = pAAAGlobalSetBannerMessage->getBanner();
	}

	if (banner.length() > (SEC_MAX_BANNER_LEN - 2))
	{
		delete pBannerManagedObj;
		delete pResults;
		delete synchronousQueryContext;
		pAAAGlobalSetBannerMessage->setCompletionStatus (BANNER_INVALID_LENGTH);
		reply (pAAAGlobalSetBannerMessage);
		return;
	}

	if (pAAAGlobalSetBannerMessage->getIsALastConfigReplay() == true) {
		/* If it is last cfg replay and same as DB banner return success */
		if (banner.compare(pBannerManagedObj->getLogin()) == 0) {
			trace (TRACE_LEVEL_INFO, "AAAGlobalSetBannerMessage::Banner already set");
			if (pBannerManagedObj) delete pBannerManagedObj;
			if (pResults) delete pResults;
			if (synchronousQueryContext) delete synchronousQueryContext;
			pAAAGlobalSetBannerMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
			reply (pAAAGlobalSetBannerMessage);
			return;
		}
	}

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	AAALocalSetBannerMessage *pMessage = new AAALocalSetBannerMessage (banner, bannerType);
	AAASetBannerContext *pSetBannerContext = new AAASetBannerContext(pAAAGlobalSetBannerMessage, pBannerManagedObj);
	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalDiscreteObjsWorker::setBannerSTCCallback), pSetBannerContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
	delete pResults;
	delete synchronousQueryContext;
}

void GlobalDiscreteObjsWorker::setBannerSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::setBannerSTCCallback()");

	AAASetBannerContext *pAAAContext = (AAASetBannerContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalSetBannerMessage *pSetMessage = (AAALocalSetBannerMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalSetBannerMessage *pAAAGlobalSetBannerMessage = (AAAGlobalSetBannerMessage *)pAAAContext->getPPrismMessage();
	BannerManagedObject *pBannerManagedObj = (BannerManagedObject *)pAAAContext->getBannerObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Setting the Banner failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Setting the Banner failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalDiscreteObjsWorker::setBannerSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? BANNER_SET_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? BANNER_SET_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	// Update and commit to the database
	if (WAVE_MESSAGE_SUCCESS == status)
	{
		UI8 bannerType = pSetMessage->getBannerType();

		startTransaction ();

		updateWaveManagedObject (pBannerManagedObj);

 
                 if ( bannerType == LOGIN_BANNER )
                 {
                         pBannerManagedObj->setLogin(pSetMessage->getBanner());
                 }
                 else if ( bannerType == MOTD_BANNER )
                 {
                         pBannerManagedObj->setMotd(pSetMessage->getBanner());
                 }
                 else if ( bannerType == INCOMING_BANNER )
                 {
                         pBannerManagedObj->setIncoming(pSetMessage->getBanner());
                 }

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting Banner to Wave database is successful.");
		}
		else
		{
			status = BANNER_SET_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting Banner failed. Framework error - " + FrameworkToolKit::localize(status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}

	delete pBannerManagedObj;
	delete pSetMessage;
	delete pAAAContext;
	delete pWaveSendToClusterContext;

	pAAAGlobalSetBannerMessage->setMessageString (errorMessage);
	pAAAGlobalSetBannerMessage->setCompletionStatus (status);
	reply (pAAAGlobalSetBannerMessage);
}

#if 0
void  GlobalDiscreteObjsWorker::TelnetServerGlobalConfigMessageHandler( TelnetServerGlobalConfigMessage *pTelnetServerGlobalConfigMessage)
{
	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::TelnetServerGlobalMessageHandler() Entered");
	if (pTelnetServerGlobalConfigMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	/* Retrieve the Telnet Server Global Managed Object */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(TelnetServerGlobalManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);

	if ((NULL == pResults) || (pResults->size() != 1))
	{
		if ((pResults == NULL) || (pResults->size() == 0)) trace (TRACE_LEVEL_FATAL, "Zero Telnet server config instances found.");
		else if (pResults->size() > 1) trace (TRACE_LEVEL_FATAL, "More than one Telnet server config instances found.");
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (unsigned int j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		pTelnetServerGlobalConfigMessage->setCompletionStatus (TELNET_SERVER_CONFIG_ERROR);
		reply (pTelnetServerGlobalConfigMessage);
		return;
	}

	TelnetServerGlobalManagedObject *pTelnetServerGlobalMO = dynamic_cast<TelnetServerGlobalManagedObject *> ((*pResults)[0]);

	// Read the input parameters
	bool disableFlag = false;

	if (pTelnetServerGlobalConfigMessage->getDisableFlag() == true)
	{
		disableFlag = true;
	}

	TelnetServerLocalConfigMessage *pTelnetServerLocalConfigMessage = new TelnetServerLocalConfigMessage(disableFlag);
	TelnetServerConfigContext *pTelnetServerConfigContext = new TelnetServerConfigContext(pTelnetServerGlobalConfigMessage, pTelnetServerGlobalMO);
	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalDiscreteObjsWorker::TelnetServerConfigSTCCallback), pTelnetServerConfigContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pTelnetServerLocalConfigMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);

	sendToWaveCluster (pWaveSendToClusterContext);
	delete pResults;
	delete synchronousQueryContext;
}

void GlobalDiscreteObjsWorker::TelnetServerConfigSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::TelnetServerConfigSTCCallback() Entered");

	TelnetServerConfigContext *pTelnetServerConfigContext = (TelnetServerConfigContext *)pWaveSendToClusterContext->getPCallerContext();
	TelnetServerLocalConfigMessage *pTelnetServerLocalConfigMessage = (TelnetServerLocalConfigMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	TelnetServerGlobalConfigMessage *pTelnetServerGlobalConfigMessage = (TelnetServerGlobalConfigMessage *)pTelnetServerConfigContext->getPPrismMessage();
	TelnetServerGlobalManagedObject *pTelnetServerGlobalMO = (TelnetServerGlobalManagedObject *)pTelnetServerConfigContext->getTelnetServerGlobalMO();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Setting the Telnet server config failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Setting the Telnet server config failed on all nodes");

		vector<LocationId> cxLocations;
		FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pTelnetServerConfigContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalDiscreteObjsWorker::TelnetServerConfigSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? TELNET_SERVER_CONFIG_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? TELNET_SERVER_CONFIG_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	// Update and commit to the database
	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		updateWaveManagedObject (pTelnetServerGlobalMO);

		pTelnetServerGlobalMO->setDisableFlag(pTelnetServerLocalConfigMessage->getDisableFlag());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting Telnet server to Wave database is successful.");
		}
		else
		{
			status = TELNET_SERVER_CONFIG_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting Telnet server config failed. Framework error - " + FrameworkToolKit::localize(status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}

	delete pTelnetServerGlobalMO;
	delete pTelnetServerLocalConfigMessage;
	delete pTelnetServerConfigContext;
	delete pWaveSendToClusterContext;

	pTelnetServerGlobalConfigMessage->setMessageString (errorMessage);
	pTelnetServerGlobalConfigMessage->setCompletionStatus (status);
	reply (pTelnetServerGlobalConfigMessage);
}

void  GlobalDiscreteObjsWorker::SSHServerGlobalConfigMessageHandler( SSHServerGlobalConfigMessage *pSSHServerGlobalConfigMessage)
{
	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::SSHServerGlobalConfigMessageHandler() Entered");
	if (pSSHServerGlobalConfigMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	/* Retrieve the SSH Server Global Managed Object */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(SSHServerGlobalManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		if ((pResults == NULL) || (pResults->size() == 0)) trace (TRACE_LEVEL_FATAL, "Zero Telnet server config instances found.");
		else if (pResults->size() > 1) trace (TRACE_LEVEL_FATAL, "More than one Telnet server config instances found.");
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (unsigned int j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		pSSHServerGlobalConfigMessage->setCompletionStatus (SSH_SERVER_CONFIG_ERROR);
		reply (pSSHServerGlobalConfigMessage);
		return;
	}

	SSHServerGlobalManagedObject *pSSHServerGlobalMO = dynamic_cast<SSHServerGlobalManagedObject *> ((*pResults)[0]);

	// Read the input parameters
	bool disableFlag = false;

	disableFlag = pSSHServerGlobalConfigMessage->getDisableFlag();
	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::SSHServerGlobalConfigMessageHandler() disableFlag: " + disableFlag);

	SSHServerLocalConfigMessage *pSSHServerLocalConfigMessage = new SSHServerLocalConfigMessage(disableFlag);
	SSHServerConfigContext *pSSHServerConfigContext = new SSHServerConfigContext(pSSHServerGlobalConfigMessage, pSSHServerGlobalMO);
	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this,
            reinterpret_cast<PrismAsynchronousCallback> (&GlobalDiscreteObjsWorker::SSHServerConfigSTCCallback), pSSHServerConfigContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pSSHServerLocalConfigMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);

	sendToWaveCluster (pWaveSendToClusterContext);
	delete pResults;
	delete synchronousQueryContext;
}

void GlobalDiscreteObjsWorker::SSHServerConfigSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	SSHServerConfigContext *pSSHServerConfigContext = (SSHServerConfigContext *)pWaveSendToClusterContext->getPCallerContext();
	SSHServerLocalConfigMessage *pSSHServerLocalConfigMessage = (SSHServerLocalConfigMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	SSHServerGlobalConfigMessage *pSSHServerGlobalConfigMessage = (SSHServerGlobalConfigMessage *)pSSHServerConfigContext->getPPrismMessage();
	SSHServerGlobalManagedObject *pSSHServerGlobalMO = (SSHServerGlobalManagedObject *)pSSHServerConfigContext->getSSHServerGlobalMO();

	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::SSHServerConfigSTCCallback() Entered");

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Setting the SSH server config failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Setting the SSH server config failed on all nodes");

		vector<LocationId> cxLocations;
		FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pSSHServerConfigContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalDiscreteObjsWorker::SSHServerConfigSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? SSH_SERVER_CONFIG_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? SSH_SERVER_CONFIG_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	// Update and commit to the database
	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		updateWaveManagedObject (pSSHServerGlobalMO);

		pSSHServerGlobalMO->setDisableFlag(pSSHServerLocalConfigMessage->getDisableFlag());

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
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}

	delete pSSHServerGlobalMO;
	delete pSSHServerLocalConfigMessage;
	delete pSSHServerConfigContext;
	delete pWaveSendToClusterContext;

	pSSHServerGlobalConfigMessage->setMessageString (errorMessage);
	pSSHServerGlobalConfigMessage->setCompletionStatus (status);
	reply (pSSHServerGlobalConfigMessage);
}

void  GlobalDiscreteObjsWorker::SSHServerListGlobalAddMessageHandler (SSHServerListGlobalAddMessage *pSSHServerListGlobalAddMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::SSHServerListGlobalAddMessageHandler() Entered");
	if (pSSHServerListGlobalAddMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	/* Retrieve the SSH Server List Managed Object */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(SSHServerListManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (0 != pResults->size()))
	{
        if (NULL == pResults)
		    trace (TRACE_LEVEL_FATAL, "No DB Table for SSH Server List");
        else if (1 == pResults->size()) 
		    trace (TRACE_LEVEL_FATAL, "SSH Server List is already present");
        if (NULL != pResults)
            delete pResults;
		delete synchronousQueryContext;
		pSSHServerListGlobalAddMessage->setCompletionStatus (SSH_SERVER_LIST_ADD_ERROR);
		reply (pSSHServerListGlobalAddMessage);
		return;
	}

	// Read the input parameters
    SSHKeyExchangeProtocol protocol;

	protocol = pSSHServerListGlobalAddMessage->getKeyExchangeProtocol();
	trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::SSHServerListGlobalAddMessageHandler() protocol: " + protocol);

	SSHServerListLocalAddMessage *pSSHServerListLocalAddMessage = new SSHServerListLocalAddMessage(protocol);
	SSHServerListAddContext *pSSHServerListAddContext = new SSHServerListAddContext(pSSHServerListGlobalAddMessage);
	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this,
            reinterpret_cast<PrismAsynchronousCallback> (&GlobalDiscreteObjsWorker::SSHServerListAddSTCCallback), pSSHServerListAddContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pSSHServerListLocalAddMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);

	sendToWaveCluster (pWaveSendToClusterContext);
	delete pResults;
	delete synchronousQueryContext;
}

void GlobalDiscreteObjsWorker::SSHServerListAddSTCCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    SSHServerListManagedObject *pSSHServerListMO = NULL;
	SSHServerListAddContext *pSSHServerListAddContext = (SSHServerListAddContext *)pWaveSendToClusterContext->getPCallerContext();
	SSHServerListLocalAddMessage *pSSHServerListLocalAddMessage = (SSHServerListLocalAddMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	SSHServerListGlobalAddMessage *pSSHServerListGlobalAddMessage = (SSHServerListGlobalAddMessage *)pSSHServerListAddContext->getPPrismMessage();

	trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::SSHServerListAddSTCCallback() Entered");

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Setting the SSH server list add failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Setting the SSH server list add failed on all nodes");

		vector<LocationId> cxLocations;
		FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pSSHServerListAddContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalDiscreteObjsWorker::SSHServerListAddSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? SSH_SERVER_LIST_ADD_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? SSH_SERVER_LIST_ADD_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	// Update and commit to the database
	if (WAVE_MESSAGE_SUCCESS == status)
	{
        SSHKeyExchangeProtocol protocol = pSSHServerListLocalAddMessage->getKeyExchangeProtocol();
	    trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::SSHServerListAddSTCCallback protocol: " + protocol);
        trace (TRACE_LEVEL_DEBUG, "SUCCESS on local plugins. Commiting to DB ...");
		startTransaction ();

        pSSHServerListMO = dynamic_cast<SSHServerListManagedObject *>(createManagedObjectInstance(SSHServerListManagedObject::getClassName()));

		pSSHServerListMO->setKeyExchangeProtocol(pSSHServerListLocalAddMessage->getKeyExchangeProtocol());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_DEBUG, "Commiting SSH Server List MO to Wave database is successful.");
		}
		else
		{
			status = SSH_SERVER_LIST_ADD_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting SSH server list addd failed. Framework error - " + FrameworkToolKit::localize(status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}

	delete pSSHServerListMO;
	delete pSSHServerListLocalAddMessage;
	delete pSSHServerListAddContext;
	delete pWaveSendToClusterContext;

	pSSHServerListGlobalAddMessage->setMessageString (errorMessage);
	pSSHServerListGlobalAddMessage->setCompletionStatus (status);
	reply (pSSHServerListGlobalAddMessage);
}

void  GlobalDiscreteObjsWorker::SSHServerListGlobalDeleteMessageHandler (SSHServerListGlobalDeleteMessage *pSSHServerListGlobalDeleteMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::SSHServerListGlobalDeleteMessageHandler() Entered");
	if (pSSHServerListGlobalDeleteMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	// Read the input parameters
    SSHKeyExchangeProtocol protocol;

	protocol = pSSHServerListGlobalDeleteMessage->getKeyExchangeProtocol();
	trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::SSHServerListGlobalDeleteMessageHandler() protocol: " + protocol);

    WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
    vector<WaveManagedObject *> *pResults = NULL;

    /* Get the SSH Server List Managed Object instance */
    synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(SSHServerListManagedObject::getClassName());
    //synchronousQueryContext->addAndAttribute (new AttributeEnum (protocol, "protocol"));
    pResults = querySynchronously (synchronousQueryContext);

    if (NULL == pResults) {
		trace (TRACE_LEVEL_FATAL, "No DB Table for SSH Server List");
        delete synchronousQueryContext;
		pSSHServerListGlobalDeleteMessage->setCompletionStatus (SSH_SERVER_LIST_DELETE_ERROR);
		reply (pSSHServerListGlobalDeleteMessage);
		return;
    }

    SSHServerListManagedObject *pSSHServerListMO = dynamic_cast<SSHServerListManagedObject *> ((*pResults)[0]);

    if (NULL == pSSHServerListMO) {
		trace (TRACE_LEVEL_FATAL, "SSH Server List MO is NULL");
        pSSHServerListGlobalDeleteMessage->setCompletionStatus (SSH_SERVER_LIST_DELETE_ERROR);
        if (NULL != pResults) {
            delete pResults;
        }
        delete synchronousQueryContext;
    }

	SSHServerListLocalDeleteMessage *pSSHServerListLocalDeleteMessage = new SSHServerListLocalDeleteMessage(protocol);
	SSHServerListDeleteContext *pSSHServerListDeleteContext = new SSHServerListDeleteContext(pSSHServerListGlobalDeleteMessage, pSSHServerListMO);
	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this,
            reinterpret_cast<PrismAsynchronousCallback> (&GlobalDiscreteObjsWorker::SSHServerListDeleteSTCCallback), pSSHServerListDeleteContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pSSHServerListLocalDeleteMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);

	sendToWaveCluster (pWaveSendToClusterContext);
    delete pResults;
	delete synchronousQueryContext;
}

void GlobalDiscreteObjsWorker::SSHServerListDeleteSTCCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
	SSHServerListDeleteContext *pSSHServerListDeleteContext = (SSHServerListDeleteContext *)pWaveSendToClusterContext->getPCallerContext();
	SSHServerListLocalDeleteMessage *pSSHServerListLocalDeleteMessage = (SSHServerListLocalDeleteMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	SSHServerListGlobalDeleteMessage *pSSHServerListGlobalDeleteMessage = (SSHServerListGlobalDeleteMessage *)pSSHServerListDeleteContext->getPPrismMessage();
	SSHServerListManagedObject *pSSHServerListMO = (SSHServerListManagedObject *)pSSHServerListDeleteContext->getSSHServerListMO();

	trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::SSHServerListDeleteSTCCallback() Entered");

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Setting the SSH server list delete failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Setting the SSH server list delete failed on all nodes");

		vector<LocationId> cxLocations;
		FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pSSHServerListDeleteContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalDiscreteObjsWorker::SSHServerListConfigSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? SSH_SERVER_LIST_DELETE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? SSH_SERVER_LIST_DELETE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	// Update and commit to the database
	if (WAVE_MESSAGE_SUCCESS == status)
	{
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
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}

	delete pSSHServerListLocalDeleteMessage;
	delete pSSHServerListDeleteContext;
	delete pWaveSendToClusterContext;

	pSSHServerListGlobalDeleteMessage->setMessageString (errorMessage);
	pSSHServerListGlobalDeleteMessage->setCompletionStatus (status);
	trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::SSHServerListDeleteSTCCallback() Exiting");
	reply (pSSHServerListGlobalDeleteMessage);
}
#endif

void  GlobalDiscreteObjsWorker::AAAGlobalSetPasswordAttributesMessageHandler( AAAGlobalSetPasswordAttributesMessage *pAAAGlobalSetPasswordAttributesMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::AAAGlobalSetPasswordAttributesMessageHandler()");
	if (pAAAGlobalSetPasswordAttributesMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	vector<UI32> locations;
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	/* coverity[var_deref_model]*/
	/*	As pAAAGlobalSetPasswordAttributesMessage is NULL, control will enter 
	the if statement and will not come out due to prismAssert*/

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
		pAAAGlobalSetPasswordAttributesMessage->setCompletionStatus (PASSWORD_ATTRIBS_SET_ERROR);
		reply (pAAAGlobalSetPasswordAttributesMessage);
		return;
	}

	PasswordAttributesManagedObject *pPasswordAttributesManagedObj = dynamic_cast<PasswordAttributesManagedObject *> ((*pResults)[0]);
	if (NULL == pPasswordAttributesManagedObj)
	{
		pAAAGlobalSetPasswordAttributesMessage->setCompletionStatus (PASSWORD_ATTRIBS_SET_ERROR);
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
			delete synchronousQueryContext;
		}
		reply (pAAAGlobalSetPasswordAttributesMessage);
		return;
	}


	// Read the input parameters
	SI32 minlen, maxretry, upper, lower, numerics, splchars, adminlockout;

	if (pAAAGlobalSetPasswordAttributesMessage->getReset() == true)
	{
		minlen = PASSWD_MIN_LEN_DEFAULT;
		maxretry = PASSWD_MAX_RETRY_DEFAULT;
		upper = PASSWD_UPPER_DEFAULT;
		lower = PASSWD_LOWER_DEFAULT;
		numerics = PASSWD_NUMERICS_DEFAULT;
		splchars = PASSWD_SPL_CHAR_DEFAULT;
		adminlockout = PASSWD_ADMIN_LOCKOUT_DEFAULT;
	}
	else
	{
		minlen = pAAAGlobalSetPasswordAttributesMessage->getMinlength();
		maxretry = pAAAGlobalSetPasswordAttributesMessage->getMaxretry();
		upper = pAAAGlobalSetPasswordAttributesMessage->getUppercase();
		lower = pAAAGlobalSetPasswordAttributesMessage->getLowercase();
		numerics = pAAAGlobalSetPasswordAttributesMessage->getNumerics();
		splchars = pAAAGlobalSetPasswordAttributesMessage->getSplchars();
		adminlockout = pAAAGlobalSetPasswordAttributesMessage->getAdminlockout();
	}

	char buf[50] = {0};
	snprintf(buf, sizeof(buf), "%d.%d.%d.%d.%d.%d.%d\n", minlen, maxretry, upper, lower, numerics, splchars, adminlockout);
	trace (TRACE_LEVEL_INFO, string("Ready to send Local message with : ") + string(buf));

	// Validate input
	UI32 vMinLen, vUpper, vLower, vNums, vSplChars;
	//UI32 vMaxRetry;
	vMinLen = (minlen < 0) ? pPasswordAttributesManagedObj->getMinlength() : minlen;
	//vMaxRetry = (maxretry < 0) ? pPasswordAttributesManagedObj->getMaxretry() : maxretry;
	vUpper = (upper < 0) ? pPasswordAttributesManagedObj->getUppercase() : upper;
	vLower = (lower < 0) ? pPasswordAttributesManagedObj->getLowercase() : lower;
	vNums = (numerics < 0) ? pPasswordAttributesManagedObj->getNumerics() : numerics;
	vSplChars = (splchars < 0) ? pPasswordAttributesManagedObj->getSplchars() : splchars;

	if (vUpper > vMinLen)
	{
		/* Number of upper-case characters must be less than or equal to min-length */
		status = PASSWORD_ATTRIBS_UPPER_CASE_RULE_ERR;
	}
	else if (vLower > vMinLen)
	{
		/* Number of lower-case characters must be less than or equal to min-length */
		status = PASSWORD_ATTRIBS_LOWER_CASE_RULE_ERR;
	}
	else if (vNums > vMinLen)
	{
		/* Number of numeric characters must be less than or equal to min-length */
		status = PASSWORD_ATTRIBS_NUMERICS_RULE_ERR;
	}
	else if (vSplChars > vMinLen)
	{
		/* Number of special characters must be less than or equal to min-length */
		status = PASSWORD_ATTRIBS_SPL_CHARS_RULE_ERR;
	}
	else if ((vUpper + vLower + vNums + vSplChars) > vMinLen)
	{
		/* The total of the lower, upper, numeric and special characters parameter value
			must be less than or equal to min-length */
		status = PASSWORD_ATTRIBS_MIN_LEN_RULE_ERR;
	}

	if (status != WAVE_MESSAGE_SUCCESS)
	{
		delete pPasswordAttributesManagedObj;
		delete synchronousQueryContext;
		delete pResults;
		pAAAGlobalSetPasswordAttributesMessage->setCompletionStatus (status);
		reply (pAAAGlobalSetPasswordAttributesMessage);
		return;
	}

	if (pAAAGlobalSetPasswordAttributesMessage->getIsALastConfigReplay() == true) {
		if (minlen == (SI32)pPasswordAttributesManagedObj->getMinlength() && 
			maxretry == (SI32)pPasswordAttributesManagedObj->getMaxretry() &&
			upper == (SI32)pPasswordAttributesManagedObj->getUppercase() &&
			lower == (SI32)pPasswordAttributesManagedObj->getLowercase() &&
			numerics == (SI32)pPasswordAttributesManagedObj->getNumerics() &&
			splchars == (SI32)pPasswordAttributesManagedObj->getSplchars() &&
			adminlockout == (SI32)pPasswordAttributesManagedObj->getAdminlockout()) {
			trace (TRACE_LEVEL_INFO, "pAAAGlobalSetPasswordAttributesMessage:: already configured");
			delete pPasswordAttributesManagedObj;
			delete synchronousQueryContext;
			delete pResults;
			pAAAGlobalSetPasswordAttributesMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
			return;
		}
	}

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	AAALocalSetPasswordAttributesMessage *pMessage = new AAALocalSetPasswordAttributesMessage (minlen, maxretry, upper, lower, numerics, splchars, adminlockout);
	AAASetPasswordAttributesContext *pSetPasswordAttributesContext = new AAASetPasswordAttributesContext(pAAAGlobalSetPasswordAttributesMessage, pPasswordAttributesManagedObj);
	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalDiscreteObjsWorker::setPasswordAttributesSTCCallback), pSetPasswordAttributesContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	delete pResults;
	delete synchronousQueryContext;
	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalDiscreteObjsWorker::setPasswordAttributesSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::setPasswordAttributesSTCCallback()");

	AAASetPasswordAttributesContext *pAAAContext = (AAASetPasswordAttributesContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalSetPasswordAttributesMessage *pSetMessage = (AAALocalSetPasswordAttributesMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalSetPasswordAttributesMessage *pAAAGlobalSetPasswordAttributesMessage = (AAAGlobalSetPasswordAttributesMessage *)pAAAContext->getPPrismMessage();
	PasswordAttributesManagedObject *pPasswordAttributesManagedObj = (PasswordAttributesManagedObject *)pAAAContext->getPasswordAttributesObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Setting the password attributes failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Setting the password attributes failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		//FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalDiscreteObjsWorker::setPasswordAttributesSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? PASSWORD_ATTRIBS_SET_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? PASSWORD_ATTRIBS_SET_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
	}

	// Update and commit to the database
	if (WAVE_MESSAGE_SUCCESS == status)
	{
		startTransaction ();

		updateWaveManagedObject (pPasswordAttributesManagedObj);
		int minlen = pSetMessage->getMinlength();
		int maxretry = pSetMessage->getMaxretry();
		int upper = pSetMessage->getUppercase();
		int lower = pSetMessage->getLowercase();
		int numerics = pSetMessage->getNumerics();
		int splchars = pSetMessage->getSplchars();
		int adminlockout = pSetMessage->getAdminlockout();
		if (minlen != PASSWD_VALUE_NOT_SET) pPasswordAttributesManagedObj->setMinlength(minlen);
		if (maxretry != PASSWD_VALUE_NOT_SET) pPasswordAttributesManagedObj->setMaxretry(maxretry);
		if (upper != PASSWD_VALUE_NOT_SET) pPasswordAttributesManagedObj->setUppercase(upper);
		if (lower != PASSWD_VALUE_NOT_SET) pPasswordAttributesManagedObj->setLowercase(lower);
		if (numerics != PASSWD_VALUE_NOT_SET) pPasswordAttributesManagedObj->setNumerics(numerics);
		if (splchars != PASSWD_VALUE_NOT_SET) pPasswordAttributesManagedObj->setSplchars(splchars);
		if (adminlockout != PASSWD_VALUE_NOT_SET){
			if (adminlockout) {
				pPasswordAttributesManagedObj->setAdminlockout(true);
			} else {
				pPasswordAttributesManagedObj->setAdminlockout(false);
			}
		}

		status = commitTransaction ();

		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS;
			//status = PASSWORD_ATTRIBS_SET_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting password attributes to Wave database is successful.");
		}
		else
		{
			status = PASSWORD_ATTRIBS_SET_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting password attributes failed. Framework error - " + FrameworkToolKit::localize(status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}

	delete pPasswordAttributesManagedObj;
	delete pSetMessage;
	delete pAAAContext;
	delete pWaveSendToClusterContext;

	pAAAGlobalSetPasswordAttributesMessage->setMessageString (errorMessage);
	pAAAGlobalSetPasswordAttributesMessage->setCompletionStatus (status);
	reply (pAAAGlobalSetPasswordAttributesMessage);
}

void GlobalDiscreteObjsWorker::AAAGlobalSetPasswordEncryptionMessageHandler (AAAGlobalSetPasswordEncryptionMessage *pAAAGlobalSetPasswordEncryptionMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::AAAGlobalSetPasswordEncryptionMessageHandler()");
	if (pAAAGlobalSetPasswordEncryptionMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	bool passwordEncryption = pAAAGlobalSetPasswordEncryptionMessage->getPasswordEncryption();
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	/* Get the Managed Object for Service password-encryption */
	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(PasswordEncryptionManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	if ((NULL == pResults) || (pResults->size() != 1))
	{
		if (NULL == pResults)
			trace (TRACE_LEVEL_FATAL, "PasswordEncryption DB Table doesn't exist.");
		else
			trace (TRACE_LEVEL_ERROR, "More than one instance of PasswordEncryption exists in the DB");

		pAAAGlobalSetPasswordEncryptionMessage->setCompletionStatus (SERVICE_PASSWORD_ENCRYPTION_CHANGE_ERROR);
		reply (pAAAGlobalSetPasswordEncryptionMessage);
		delete synchronousQueryContext;
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
		}
		return;
	}
	PasswordEncryptionManagedObject *pPasswordEncryptionObj = dynamic_cast<PasswordEncryptionManagedObject *> ((*pResults)[0]);

	if (NULL == pPasswordEncryptionObj)
	{
		pAAAGlobalSetPasswordEncryptionMessage->setCompletionStatus (SERVICE_PASSWORD_ENCRYPTION_CHANGE_ERROR);
		if (pResults != NULL) {
			for (UI32 j = 0; j < pResults->size(); j++)
				delete (*pResults)[j];

			delete pResults;
			delete synchronousQueryContext;
		}
		reply (pAAAGlobalSetPasswordEncryptionMessage);
		return;
	}

	delete pResults;
	delete synchronousQueryContext;

	if (pAAAGlobalSetPasswordEncryptionMessage->getIsALastConfigReplay() == true) {
		if (passwordEncryption == pPasswordEncryptionObj->getPasswordEncryption()) {
			trace (TRACE_LEVEL_INFO, "pAAAGlobalSetPasswordEncryptionMessage already set");
			pAAAGlobalSetPasswordEncryptionMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			reply(pAAAGlobalSetPasswordEncryptionMessage);
			return;
		}		
	}

	/* Get all existing users from the DB for updating password encryption level */
	vector<UserManagedObject *> pUserObjs;
	if (passwordEncryption == true)
	{
		synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(UserManagedObject::getClassName());
		pResults = querySynchronously (synchronousQueryContext);
		if (pResults != NULL)
		{
			for (UI32 i = 0; i < pResults->size(); i++)
			{
				pUserObjs.push_back(dynamic_cast<UserManagedObject *>((*pResults)[i]));
			}
		}

		delete pResults;
		delete synchronousQueryContext;
    }

	/* Commit the value to the database */
	startTransaction ();

	updateWaveManagedObject(pPasswordEncryptionObj);
	pPasswordEncryptionObj->setPasswordEncryption(passwordEncryption);

	if (passwordEncryption == true)
	{
		UserManagedObject *pUserMO = NULL;
		for (UI32 i = 0; i < pUserObjs.size(); i++)
		{
			pUserMO = pUserObjs[i];
			if (pUserMO->getEncryptionLevel() != ENCRYPTION_HASH)
			{
				updateWaveManagedObject(pUserMO);
   				pUserMO->setEncryptionLevel(ENCRYPTION_HASH);
				string userPassword = pUserMO->getPassword();
				userPassword = AAACrypto::encryptPassword(userPassword);
				pUserMO->setPassword(userPassword);
			}
		}
		trace (TRACE_LEVEL_INFO, "Encrypted all existing clear-text passwords.");
	}

	status = commitTransaction ();
	if (FRAMEWORK_SUCCESS == status)
	{
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_INFO, "Commiting the password-encryption service configuration changes to Wave database is successful.");
	}
	else
	{
		status = SERVICE_PASSWORD_ENCRYPTION_CHANGE_ERROR;
		trace (TRACE_LEVEL_ERROR, "Commiting the password-encryption changes failed. Framework error - " + FrameworkToolKit::localize (status));
	}

	/* Reply */
	pAAAGlobalSetPasswordEncryptionMessage->setCompletionStatus(status);
	reply(pAAAGlobalSetPasswordEncryptionMessage);
}

bool  GlobalDiscreteObjsWorker::isValidRole (string role)
{
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;
	bool isValid = false;

	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RoleManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	if (pResults != NULL) {
		for (UI32 j = 0; j < pResults->size(); j++) {
			RoleManagedObject *pRoleObj = dynamic_cast<RoleManagedObject *> ((*pResults)[j]);
			if (NULL == pRoleObj)
			{
				delete synchronousQueryContext;
				if (pResults != NULL) delete pResults;
				return false;
			}

			if (role.compare(pRoleObj->getName()) == 0)	{
				isValid = true;
			}
			delete pRoleObj;
		}
		delete pResults;
	}
	delete synchronousQueryContext;

	return (isValid);
}
void  GlobalDiscreteObjsWorker::AAAGlobalAddMapRoleMessageHandler (AAAGlobalAddMapRoleMessage *pAAAGlobalAddMapRoleMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalAuthenticationWorker::AAAGlobalAddMapRoleMessageHandler()");
	if (pAAAGlobalAddMapRoleMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}
	vector<UI32> locations;

	string group = pAAAGlobalAddMapRoleMessage->getGroup();
	string role = pAAAGlobalAddMapRoleMessage->getRole();

	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	/*Kumar Ghanta: Should be one of the existing roles.Need to validate */
	if (isValidRole(role) == false) 
	{
		trace (TRACE_LEVEL_ERROR, "Not a valid role to map the AD group");
		status = MAPROLE_INVALID_ROLE;
		ret = -1;
	}
	if (role.compare(ACCT_ROOT_ROLE) == 0)
	{
		trace (TRACE_LEVEL_ERROR, "'root' cannot be added");
		status = MAPROLE_ROOT_CANT_MAPPED;
		ret = -1;
	}

	if (ret == 0)
	{
		/*Kumar Ghanta: Need to validate the AD group futher. */	
		if (group.length() <= 0)
		{
			ret = -1;	
			status = MAPROLE_INVALID_GROUP;
		}
	}

	synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(MapRoleManagedObject::getClassName());
	pResults = querySynchronously (synchronousQueryContext);
	if (NULL == pResults)
	{
		trace (TRACE_LEVEL_ERROR, "No DB Table for MapRoleManagedObject");
		pAAAGlobalAddMapRoleMessage->setCompletionStatus(MAPROLE_ADD_ERROR);
		reply (pAAAGlobalAddMapRoleMessage);
		delete synchronousQueryContext;
		return;
	}

	// Check for the maximum map role limit
	if ((0 == ret) && (pResults->size() >= LDAP_MAX_MAPPING))
	{
		ret = -1;
		status = MAPROLE_ADD_FS_MAX_ERROR;
	}

	if (ret != 0)
    {
        pAAAGlobalAddMapRoleMessage->setCompletionStatus(status);
        reply (pAAAGlobalAddMapRoleMessage);
		delete synchronousQueryContext;
		for (UI32 i = 0; i < pResults->size();i++)
			delete (*pResults)[i];
		delete pResults;
        return;
    }   
    else
    {
        trace (TRACE_LEVEL_DEBUG, "MapRole input validation OK.");
		delete synchronousQueryContext;
		for (UI32 i = 0; i < pResults->size();i++)
			delete (*pResults)[i];
		delete pResults;
    }

	if (pAAAGlobalAddMapRoleMessage->getIsALastConfigReplay() == true) {
		synchronousQueryContext =
			new WaveManagedObjectSynchronousQueryContext(MapRoleManagedObject::getClassName());
		if (role.length() > 0) {
			synchronousQueryContext->addOrAttribute(new AttributeString(role, "switchrole"));
		}	
		if (group.length() > 0) {
			synchronousQueryContext->addOrAttribute(new AttributeString(group, "adgroup"));
		}
		pResults = querySynchronously (synchronousQueryContext);
		delete synchronousQueryContext;
		if ((NULL == pResults) || (pResults->size() != 1)) {
			trace (TRACE_LEVEL_DEBUG, "Last Cfg replay: similar maprole does not exist");
		} else if (pResults->size() == 1) { /* maprole has unique entry in DB */
            trace (TRACE_LEVEL_DEBUG, "Last Cfg replay: maprole already exist");
            for (UI32 j = 0; j < pResults->size(); j++)
                delete (*pResults)[j];
            delete pResults;
            pAAAGlobalAddMapRoleMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply(pAAAGlobalAddMapRoleMessage);
			return;
		}
	}

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	// Send message to all local services
	AAALocalAddMapRoleMessage *pAddMessage = new AAALocalAddMapRoleMessage (group, role);

	AAAAddMapRoleContext *pAddMapRoleContext = new AAAAddMapRoleContext (pAAAGlobalAddMapRoleMessage);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalDiscreteObjsWorker::addMapRoleSTCCallback), pAddMapRoleContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pAddMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalDiscreteObjsWorker::addMapRoleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::addMapRoleSTCCallback()");

    AAAAddMapRoleContext *pAAAContext = (AAAAddMapRoleContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalAddMapRoleMessage *pAddMessage = (AAALocalAddMapRoleMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalAddMapRoleMessage *pAAAGlobalAddMapRoleMessage = (AAAGlobalAddMapRoleMessage *)pAAAContext->getPPrismMessage();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Add Map role operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Add Map role operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalAuthenticationWorker::addMapRoleSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? MAPROLE_ADD_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? MAPROLE_ADD_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
    }

	if (status == WAVE_MESSAGE_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "MapRole add operation is successful in the FS.");

		startTransaction ();

		MapRoleManagedObject *pMapRoleMO = dynamic_cast<MapRoleManagedObject *>(createManagedObjectInstance(MapRoleManagedObject::getClassName()));
		pMapRoleMO->setGroup(pAddMessage->getGroup());
		pMapRoleMO->setRole(pAddMessage->getRole());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = ROLE_ADD_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting the Role configuration to Wave database is successful.");
		}
		else
		{
			status = MAPROLE_ADD_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting the Role configuration failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
		delete pMapRoleMO;
	}
	pAAAGlobalAddMapRoleMessage->setMessageString(errorMessage);

	delete pAddMessage;

	delete pAAAContext;
	delete pWaveSendToClusterContext;

	pAAAGlobalAddMapRoleMessage->setCompletionStatus(status);
	reply(pAAAGlobalAddMapRoleMessage);

	trace (TRACE_LEVEL_WARN, "AAAGlobalAddMapRoleMessageHandler() - EXIT");
}

void  GlobalDiscreteObjsWorker::AAAGlobalChangeMapRoleMessageHandler( AAAGlobalChangeMapRoleMessage *pAAAGlobalChangeMapRoleMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::AAAGlobalChangeMapRoleMessageHandler()");
	if (pAAAGlobalChangeMapRoleMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	int ret = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	vector<UI32> locations;

	string group = pAAAGlobalChangeMapRoleMessage->getGroup();
	string role = pAAAGlobalChangeMapRoleMessage->getRole();

	if (isValidRole(role) == false) 
	{
		trace (TRACE_LEVEL_ERROR, "Not a valid role to map the AD group");
		status = MAPROLE_INVALID_ROLE;
		ret = -1;
	}
	if (role.compare(ACCT_ROOT_ROLE) == 0)
	{
		trace (TRACE_LEVEL_ERROR, "'root' cannot be added");
		status = MAPROLE_ROOT_CANT_MAPPED;
		ret = -1;
	}

	if (ret == 0)
	{
		/*Kumar Ghanta: Need to validate the AD group futher. */	
		if (group.length() <= 0)
		{
			ret = -1;	
			status = MAPROLE_INVALID_GROUP;
		}
	}

	if (ret != 0)
	{
		pAAAGlobalChangeMapRoleMessage->setCompletionStatus(status);
		reply (pAAAGlobalChangeMapRoleMessage);
		return;
	}
	else
	{
		trace (TRACE_LEVEL_DEBUG, "MapRole input validation OK.");
	}

	/* Get the Role Managed Object for optional parameters */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(MapRoleManagedObject::getClassName());

	synchronousQueryContext->addAndAttribute(new AttributeString(group, "adgroup"));
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if (NULL == pResults)
	{
		trace (TRACE_LEVEL_FATAL, "AD Group does not exist");
		pAAAGlobalChangeMapRoleMessage->setCompletionStatus (MAPROLE_ADGROUP_DOESNT_EXIST);
		reply (pAAAGlobalChangeMapRoleMessage);
		delete synchronousQueryContext;
		return;
	}
	MapRoleManagedObject *pMapRoleObj = dynamic_cast<MapRoleManagedObject *> ((*pResults)[0]);

	delete pResults;
	delete synchronousQueryContext;

	if (pAAAGlobalChangeMapRoleMessage->getIsALastConfigReplay() == true) {
        if ((role.length() > 0) &&
			(role.compare(pMapRoleObj->getRole()) == 0)) {
				trace (TRACE_LEVEL_INFO, "pAAAGlobalChangeMapRoleMessage already set");
    	        pAAAGlobalChangeMapRoleMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
	   	        reply(pAAAGlobalChangeMapRoleMessage);
				if (pMapRoleObj) delete pMapRoleObj;
	            return;
		} 
    }
	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	// Send message to all local services
	AAALocalChangeMapRoleMessage *pSetMessage = new AAALocalChangeMapRoleMessage (group, role);

	AAAChangeMapRoleContext *pChangeMapRoleContext = new AAAChangeMapRoleContext (pAAAGlobalChangeMapRoleMessage, pMapRoleObj);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalDiscreteObjsWorker::changeMapRoleSTCCallback), pChangeMapRoleContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pSetMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalDiscreteObjsWorker::changeMapRoleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::changeMapRoleSTCCallback()");

    AAAChangeMapRoleContext *pAAAContext = (AAAChangeMapRoleContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalChangeMapRoleMessage *pSetMessage = (AAALocalChangeMapRoleMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalChangeMapRoleMessage *pAAAGlobalChangeMapRoleMessage = (AAAGlobalChangeMapRoleMessage *)pAAAContext->getPPrismMessage();
	MapRoleManagedObject *pMapRoleObj = (MapRoleManagedObject *)pAAAContext->getMapRoleObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Change Map role operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Change Map role operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		// FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalDiscreteObjsWorker::changeiMapRoleSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? MAPROLE_CHANGE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? MAPROLE_CHANGE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
    }

	if (status == WAVE_MESSAGE_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "Map Role change operation is successful in the FS.");

		startTransaction ();

		updateWaveManagedObject(pMapRoleObj);

		pMapRoleObj->setRole(pSetMessage->getRole());

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = MAPROLE_CHANGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting the MAP Role configuration to Wave database is successful.");
		}
		else
		{
			status = MAPROLE_CHANGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting the Map Role configuration failed. Framework error - " + FrameworkToolKit::localize (status));
			if (errorMessage.length() > 0)
				errorMessage += string("\n") + FrameworkToolKit::localize (status);
		}
	}
	pAAAGlobalChangeMapRoleMessage->setMessageString(errorMessage);

	delete pMapRoleObj;
	delete pSetMessage;
	delete pAAAContext;
	delete pWaveSendToClusterContext;

	pAAAGlobalChangeMapRoleMessage->setCompletionStatus(status);
	reply(pAAAGlobalChangeMapRoleMessage);
	trace (TRACE_LEVEL_INFO, "AAAGlobalChangeMapRoleMessageHandler() - EXIT");
}

void  GlobalDiscreteObjsWorker::AAAGlobalDeleteMapRoleMessageHandler( AAAGlobalDeleteMapRoleMessage *pAAAGlobalDeleteMapRoleMessage)
{
	trace (TRACE_LEVEL_DEBUG, "GlobalDiscreteObjsWorker::AAAGlobalDeleteMapRoleMessageHandler()");
	if (pAAAGlobalDeleteMapRoleMessage == NULL)
	{
		trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
		prismAssert (false, __FILE__, __LINE__);
	}

	string group = pAAAGlobalDeleteMapRoleMessage->getGroup();
	vector<UI32> locations;

	/* Get the MapRole Managed Object for deleting */
	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(MapRoleManagedObject::getClassName());

	synchronousQueryContext->addAndAttribute(new AttributeString(group, "adgroup"));
	vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
	if (NULL == pResults)
	{
		trace (TRACE_LEVEL_FATAL, "AD group does not exist");
		if (pAAAGlobalDeleteMapRoleMessage->getIsALastConfigReplay() == true) {
			trace (TRACE_LEVEL_DEBUG, "pAAAGlobalDeleteMapRoleMessage getIsALastConfigReplay true");
			pAAAGlobalDeleteMapRoleMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		} else {
			pAAAGlobalDeleteMapRoleMessage->setCompletionStatus (MAPROLE_ADGROUP_DOESNT_EXIST);
		}
		reply (pAAAGlobalDeleteMapRoleMessage);
		delete synchronousQueryContext;
		return;
	}
	MapRoleManagedObject *pMapRoleObj = dynamic_cast<MapRoleManagedObject *> ((*pResults)[0]);

	delete pResults;
	delete synchronousQueryContext;

	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	// Send message to all local services
	AAALocalDeleteMapRoleMessage *pMessage = new AAALocalDeleteMapRoleMessage (group);

	AAADeleteMapRoleContext *pDeleteMapRoleContext = new AAADeleteMapRoleContext (pAAAGlobalDeleteMapRoleMessage, pMapRoleObj);

	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalDiscreteObjsWorker::deleteMapRoleSTCCallback), pDeleteMapRoleContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
}

void GlobalDiscreteObjsWorker::deleteMapRoleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::deleteMapRoleSTCCallback()");

    AAADeleteMapRoleContext *pAAAContext = (AAADeleteMapRoleContext *)pWaveSendToClusterContext->getPCallerContext();
	AAALocalDeleteMapRoleMessage *pMessage = (AAALocalDeleteMapRoleMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	AAAGlobalDeleteMapRoleMessage *pAAAGlobalDeleteMapRoleMessage = (AAAGlobalDeleteMapRoleMessage *)pAAAContext->getPPrismMessage();
	MapRoleManagedObject *pMapRoleObj = (MapRoleManagedObject *)pAAAContext->getMapRoleObj();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Delete Map role operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Delete Map role operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		//FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "GlobalDiscreteObjsWorker::deleteMapRoleSTCCallback", errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? MAPROLE_DELETE_ERROR :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? MAPROLE_DELETE_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
		}
    }

	if (status == WAVE_MESSAGE_SUCCESS)
	{
		trace (TRACE_LEVEL_INFO, "Map Role delete operation is successful in the FS.");

		startTransaction ();

		delete pMapRoleObj;

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			status = WAVE_MESSAGE_SUCCESS; //status = ROLE_DELETE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "Commiting the Map Role configuration to Wave database is successful.");
		}
		else
		{
			status = MAPROLE_DELETE_ERROR;
			trace (TRACE_LEVEL_ERROR, "Commiting the Map Role configuration failed. Framework error - " + FrameworkToolKit::localize (status));
		}
	}
	pAAAGlobalDeleteMapRoleMessage->setMessageString(errorMessage);

	delete pMessage;
	delete pAAAContext;
	delete pWaveSendToClusterContext;

	pAAAGlobalDeleteMapRoleMessage->setCompletionStatus(status);
	reply(pAAAGlobalDeleteMapRoleMessage);

	trace (TRACE_LEVEL_INFO, "AAAGlobalDeleteMapRoleMessageHandler() - EXIT");
}

void  GlobalDiscreteObjsWorker::AAAGlobalCertutilOpActionMessageHandler(AAAGlobalCertutilOpActionMessage *pAAAGlobalCertutilOpActionMessage)
{
	vector<UI32> locations;
	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

	AAACertutilContext *pCertutilContext = new AAACertutilContext (pAAAGlobalCertutilOpActionMessage);

	CertutilOpActionMessage *pMessage = new CertutilOpActionMessage (pAAAGlobalCertutilOpActionMessage->getFosCmd(), pAAAGlobalCertutilOpActionMessage->getCertType());
	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalDiscreteObjsWorker::doCertutilSTCCallback), pCertutilContext);
	pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
	pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}

	sendToWaveCluster (pWaveSendToClusterContext);
	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::actionHandler : Received response..");
}

void GlobalDiscreteObjsWorker::doCertutilSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
{
	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::doCertutilSTCCallback()");
	AAACertutilContext *pAAAContext = (AAACertutilContext *)pWaveSendToClusterContext->getPCallerContext();
	ResourceId status = pWaveSendToClusterContext->getCompletionStatus();
	string errorMessage;

	CertutilOpActionMessage *pCertutilOpActionMessage = (CertutilOpActionMessage *)pAAAContext->getPPrismMessage();

	if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
		status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
	{
		if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
			trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Importing/Deleting certificate operation failed on one or more of the connected nodes.");
		else
			trace (TRACE_LEVEL_ERROR, "FAILURE - Importing/Deleting certificate operation failed on all nodes");

		vector<LocationId> cxLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		//FrameworkToolKit::getFullyConnectedLocations(cxLocations);
		if (cxLocations.size() > 0)
		{
			pAAAContext->traceSendToClusterErrors(pWaveSendToClusterContext, "CertutilActionpointWorker::doCertutilSTCCallback", errorMessage);
			pCertutilOpActionMessage->setCompletionStatus(status);
			/* If the operation is being performed in more than 1 nodes, error message will be set to the message string. */
         	pCertutilOpActionMessage->setMessageString(errorMessage);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
			LocationId thisNode = FrameworkToolKit::getThisLocationId();
			/* If the operation is being performed in only one node, status will be set based on the result. */
			if (pCertutilOpActionMessage->getCertType() == CERT_LDAPCA_IMPORT) {
				status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? CERTUTIL_LDAPCA_IMPORT_FAILURE :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? CERTUTIL_LDAPCA_IMPORT_INTERNAL_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
			} else if (pCertutilOpActionMessage->getCertType() == CERT_LDAPCA_DELETE) {
	            trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::doCertutilSTCCallback()");
				status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? CERTUTIL_LDAPCA_DELETE_FAILURE :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? CERTUTIL_LDAPCA_DELETE_INTERNAL_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
			} else if (pCertutilOpActionMessage->getCertType() == CERT_SYSLOGCA_IMPORT) {
	            trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::doCertutilSTCCallback: CERT_SYSLOGCA_IMPORT");
				status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? CERTUTIL_SYSLOGCA_IMPORT_FAILURE :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? CERTUTIL_SYSLOGCA_IMPORT_INTERNAL_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
			} else if (pCertutilOpActionMessage->getCertType() == CERT_SYSLOGCA_DELETE) {
	            trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::doCertutilSTCCallback: CERT_SYSLOGCA_DELETE");
				status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? CERTUTIL_SYSLOGCA_DELETE_FAILURE :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? CERTUTIL_SYSLOGCA_DELETE_INTERNAL_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
			} else if (pCertutilOpActionMessage->getCertType() == CERT_SSHKEY_IMPORT) {
	            trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::doCertutilSTCCallback: CERT_SSHKEY_IMPORT");
				status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? CERTUTIL_SSHKEY_IMPORT_FAILURE :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? CERTUTIL_SSHKEY_IMPORT_INTERNAL_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
			} else if (pCertutilOpActionMessage->getCertType() == CERT_SSHKEY_DELETE) {
	            trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker::doCertutilSTCCallback: CERT_SSHKEY_DELETE");
				status = (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS) ? CERTUTIL_SSHKEY_DELETE_FAILURE :
					(pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS) ? CERTUTIL_SSHKEY_DELETE_INTERNAL_ERROR :
					pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
			}
		}
	}

	if ((status == WAVE_MESSAGE_SUCCESS) || ((status == FRAMEWORK_SUCCESS)))
	{
		if (pCertutilOpActionMessage->getCertType() == CERT_LDAPCA_IMPORT) {
			status = CERTUTIL_LDAPCA_IMPORT_SUCCESSFUL;
		} else if (pCertutilOpActionMessage->getCertType() == CERT_LDAPCA_DELETE) {
            status = CERTUTIL_LDAPCA_DELETE_SUCCESSFUL;
		} else if (pCertutilOpActionMessage->getCertType() == CERT_SYSLOGCA_IMPORT) {
			status = CERTUTIL_SYSLOGCA_IMPORT_SUCCESSFUL;
		} else if (pCertutilOpActionMessage->getCertType() == CERT_SYSLOGCA_DELETE) {
			status = CERTUTIL_SYSLOGCA_DELETE_SUCCESSFUL;
		} else if (pCertutilOpActionMessage->getCertType() == CERT_SSHKEY_IMPORT) {
			status = CERTUTIL_SSHKEY_IMPORT_SUCCESSFUL;
		} else if (pCertutilOpActionMessage->getCertType() == CERT_SSHKEY_DELETE) {
			status = CERTUTIL_SSHKEY_DELETE_SUCCESSFUL;
        }
	}
	delete pAAAContext;
	delete pWaveSendToClusterContext;
	pCertutilOpActionMessage->setCompletionStatus(status);
	reply(pCertutilOpActionMessage);

	trace (TRACE_LEVEL_INFO, "GlobalDiscreteObjsWorker() - EXIT.");
}
}
