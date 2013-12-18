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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aditya Munjal                                                *
 **************************************************************************/

#include "Firmware/Global/NodeAutoUpgradeObjectManager.h"
#include "Firmware/Global/NodeAutoUpgradeManagedObject.h"
#include "Firmware/Global/NodeAutoUpgradeConfigMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Firmware/Global/FirmwareTypes.h"
#include "AAA/Global/AAACrypto.h"

namespace DcmNs
{

NodeAutoUpgradeObjectManager::NodeAutoUpgradeObjectManager ()
	: WaveObjectManager  (getClassName ())
{
	NodeAutoUpgradeManagedObject NodeAutoUpgradeManagedObject(this);
	NodeAutoUpgradeManagedObject.setupOrm();
	addManagedClass(NodeAutoUpgradeManagedObject::getClassName());
	addOperationMap(NODEAUTOUPGRADECONFIG, reinterpret_cast<PrismMessageHandler> (&NodeAutoUpgradeObjectManager::NodeAutoUpgradeConfigMessageHandler));
}


NodeAutoUpgradeObjectManager::~NodeAutoUpgradeObjectManager ()
{
}


NodeAutoUpgradeObjectManager *NodeAutoUpgradeObjectManager::getInstance()
{
	static NodeAutoUpgradeObjectManager *pNodeAutoUpgradeObjectManager = new NodeAutoUpgradeObjectManager();

	WaveNs::prismAssert (NULL != pNodeAutoUpgradeObjectManager,
				__FILE__, __LINE__);

	return (pNodeAutoUpgradeObjectManager);
}


string NodeAutoUpgradeObjectManager::getClassName()
{
	return ("NodeAutoUpgrade");
}


PrismServiceId NodeAutoUpgradeObjectManager::getPrismServiceId()
{
	return ((getInstance())->getServiceId());
}


PrismMessage *NodeAutoUpgradeObjectManager::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	switch(operationCode) {

		case NODEAUTOUPGRADECONFIG :
			pPrismMessage = new NodeAutoUpgradeConfigMessage();
			break;
		default :
		pPrismMessage = NULL;
	}

	return (pPrismMessage);
}


WaveManagedObject *NodeAutoUpgradeObjectManager::createManagedObjectInstance(const string &managedClassName)
{
	WaveManagedObject *pWaveManagedObject = NULL;

	if ((NodeAutoUpgradeManagedObject::getClassName()) == managedClassName) {

		pWaveManagedObject = new NodeAutoUpgradeManagedObject(this);

	} else {

		trace (TRACE_LEVEL_FATAL, "NodeAutoUpgradeObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);

	}
	return (pWaveManagedObject);
}


void NodeAutoUpgradeObjectManager::install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	trace(TRACE_LEVEL_INFO, "NodeAutoUpgradeObjectManager::install : "
		"Installing NodeAutoUpgrade GMO DB");

	WaveBootReason bootReason =
		pWaveAsynchronousContextForBootPhases->getBootReason();

	WaveNs::trace (TRACE_LEVEL_INFO, string ("NodeAutoUpgradeObjectManager::"
		"install : boot = ") + bootReason);

	if ((WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason) ||
		(WAVE_BOOT_FIRST_TIME_BOOT == bootReason)) {

		startTransaction ();
		NodeAutoUpgradeManagedObject *pAutoUpgradeManagedOgject =
			new NodeAutoUpgradeManagedObject(this);
		pAutoUpgradeManagedOgject->setStatus(false);
		status = commitTransaction ();

		if (FRAMEWORK_SUCCESS == status) {
			trace(TRACE_LEVEL_INFO, "NodeAutoUpgradeObjectManager::install: "
				"Installing NodeAutoUpgrade DB complete");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_FATAL, "NodeAutoUpgradeObjectManager::install: "
				"Failed to install NodeAutoUpgrade DB");
			prismAssert (false, __FILE__, __LINE__);
    	}

		/*if (WAVE_BOOT_FIRST_TIME_BOOT == bootReason) {
			delete pNAUMO;
		}*/

		if (WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason) {
			trace(TRACE_LEVEL_DEBUG, "NodeAutoUpgradeObjectManager::install: "
				"secondary configure boot");
			status = WAVE_MESSAGE_SUCCESS;
		}

		delete pAutoUpgradeManagedOgject;
	}

	pWaveAsynchronousContextForBootPhases->setCompletionStatus(status);
	pWaveAsynchronousContextForBootPhases->callback ();

}


void NodeAutoUpgradeObjectManager::NodeAutoUpgradeConfigMessageHandler(
		NodeAutoUpgradeConfigMessage *pUpgradeConfigMessage)
{

	trace(TRACE_LEVEL_DEBUG, "NodeAutoUpgradeObjectManager::NodeAutoUpgrade"
		"ConfigMessageHandler : Entering...");

	UI32		flags;
	ResourceId	status = WAVE_MESSAGE_ERROR;

	vector<WaveManagedObject *> *pResults = querySynchronously (NodeAutoUpgradeManagedObject::getClassName());

	NodeAutoUpgradeManagedObject *pUpgradeManagedObject =
		dynamic_cast<NodeAutoUpgradeManagedObject *> ((*pResults)[0]);;
	prismAssert (NULL != pUpgradeManagedObject, __FILE__, __LINE__);

	flags = pUpgradeConfigMessage->getFlags();

	if (pUpgradeConfigMessage->getStatus() == 1)
			pUpgradeManagedObject->setStatus(true);
	else if (pUpgradeConfigMessage->getStatus() == 0)
			pUpgradeManagedObject->setStatus(false);

	pUpgradeManagedObject->setFirmwareVersion(
								pUpgradeConfigMessage->getFirmwareVersion());

	if (flags & NA_OPT_HOST)
		pUpgradeManagedObject->setIpAddress(
								pUpgradeConfigMessage->getHost());

	if (flags & NA_OPT_PATH)
		pUpgradeManagedObject->setPath(pUpgradeConfigMessage->getPath());

	if (flags & NA_OPT_PROTOCOL)
		pUpgradeManagedObject->setProtocol(
								pUpgradeConfigMessage->getProtocol());

	if (flags & NA_OPT_USERNAME)
		pUpgradeManagedObject->setUsername(
								pUpgradeConfigMessage->getUsername());
	
	if (flags & NA_OPT_PASSWORD) {
		string password = AAACrypto::encryptPassword(
								pUpgradeConfigMessage->getPassword());
		pUpgradeManagedObject->setPassword(password);
	}

	pUpgradeManagedObject->setMsgVersion(NODEAUTOUPGRADE_MSG_VER);

	pUpgradeManagedObject->setFlags(pUpgradeConfigMessage->getFlags());

	startTransaction();
	updateWaveManagedObject(pUpgradeManagedObject);
	status = commitTransaction();

	if (FRAMEWORK_SUCCESS == status) {
		trace (TRACE_LEVEL_INFO, "Updated NodeAutoUpgrade Managed Object");
		status = WAVE_MESSAGE_SUCCESS;
	} else {
		trace(TRACE_LEVEL_FATAL, string("Failed to update NodeAutoUpgrade "
			"Managed Object") + FrameworkToolKit::localize (status));
	}

	delete pUpgradeManagedObject;

	pUpgradeConfigMessage->setCompletionStatus (status);
	reply(pUpgradeConfigMessage);

}


}
