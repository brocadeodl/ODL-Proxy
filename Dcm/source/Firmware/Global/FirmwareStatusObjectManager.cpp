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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aditya Munjal                                                *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "errno.h"

#include "Firmware/Global/FirmwareStatusObjectManager.h"
#include "Firmware/Global/FirmwareStatusMessage.h"
#include "Firmware/Global/FirmwareTypes.h"

namespace DcmNs
{

	FirmwareStatusObjectManager::FirmwareStatusObjectManager()
		: WaveLocalObjectManager(getClassName())
	{
		addOperationMap(FIRMWARENETCONFSTATUS, reinterpret_cast<PrismMessageHandler> (&FirmwareStatusObjectManager::FirmwareStatusMessageHandler));
	}

	FirmwareStatusObjectManager::~FirmwareStatusObjectManager()
	{
	}

	FirmwareStatusObjectManager *FirmwareStatusObjectManager::getInstance()
	{
		static FirmwareStatusObjectManager *pFirmwareStatusObjectManager = new FirmwareStatusObjectManager();

		WaveNs::prismAssert(NULL != pFirmwareStatusObjectManager, __FILE__, __LINE__);

		return(pFirmwareStatusObjectManager);
	}

	string  FirmwareStatusObjectManager::getClassName()
	{
		return("GlobalFirmwareStatus");
	}

	PrismServiceId  FirmwareStatusObjectManager::getPrismServiceId()
	{
		return ((getInstance())->getServiceId());
	}

	PrismMessage *FirmwareStatusObjectManager::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;

		switch (operationCode)
		{

			case FIRMWARENETCONFSTATUS :
				pPrismMessage = new FirmwareStatusMessage();
				break;
			default :
				pPrismMessage = NULL;
		}

		return (pPrismMessage);
	}

	WaveManagedObject *FirmwareStatusObjectManager::createManagedObjectInstance(const string &managedClassName)
	{
		return NULL;
	}

	vector<WaveManagedObject *> *FirmwareStatusObjectManager::DoSynchronousQuery (const string className)
	{
		vector<WaveManagedObject *> *pResults = querySynchronously (className);
		prismAssert (NULL != pResults, __FILE__, __LINE__);
		return pResults;
	}

	void FirmwareStatusObjectManager::FirmwareStatusMessageHandler(FirmwareStatusMessage *pFirmwareStatusMessage)
	{

		WaveNs::trace(TRACE_LEVEL_ERROR, "FirmwareStatusOM::FirmwareStatus"
			"MessageHandler: Entry.");
		
		PrismLinearSequencerStep sequencerSteps1[] =
		{
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareStatusObjectManager::NetconfStatusHandler),
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareStatusObjectManager::prismLinearSequencerSucceededStep),
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareStatusObjectManager::prismLinearSequencerFailedStep)
		};

		WaveNs::trace(TRACE_LEVEL_SUCCESS, "FirmwareStatusOM::Firmware"
			"StatusMessageHandler: Netconf status operation initiated.");
		PrismLinearSequencerContext *pNetconfStatusContext = new PrismLinearSequencerContext (pFirmwareStatusMessage, this, sequencerSteps1, sizeof (sequencerSteps1) / sizeof (sequencerSteps1[0]));

		pNetconfStatusContext->setPPrismMessage(pFirmwareStatusMessage);
		pNetconfStatusContext->start();

	}

	void FirmwareStatusObjectManager::NetconfStatusHandler(PrismLinearSequencerContext *pNetconfStatusContext)
	{
		UI32			switchId = 0, entire_cluster = 1, i;
		UI32			nodesInCluster;
		vector<UI32>	rbridgeIdsVector, rbridgeIdsVector_temp;
		ResourceId		status = WAVE_MESSAGE_SUCCESS;

		pNetconfStatusContext->setNumberOfFailures(0);
		++(*pNetconfStatusContext);

		FirmwareStatusMessage *pFirmwareStatusMessage = dynamic_cast<FirmwareStatusMessage *> (pNetconfStatusContext->getPPrismMessage());

		vector<WaveManagedObject *> *pResultsClusterInfo = DoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
		prismAssert (NULL != pResultsClusterInfo, __FILE__, __LINE__);
		nodesInCluster = pResultsClusterInfo->size();

		rbridgeIdsVector = pFirmwareStatusMessage->getRbridgeId();
		if ((rbridgeIdsVector.size() > 0) && (rbridgeIdsVector[0] != 0)) {
			nodesInCluster = rbridgeIdsVector.size();
			entire_cluster = 0;
		}

		rbridgeIdsVector_temp.clear();
		pFirmwareStatusMessage->setRbridgeIdVector(rbridgeIdsVector_temp);

		for (i = 0; i < nodesInCluster; i++) {

			if (entire_cluster == 1) {
				VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject  = (dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i]));
				switchId = pVcsNodeLocalManagedObject->getMappedId();
			} else {
				switchId = rbridgeIdsVector[i];
			}

			LocationId locationId =
							DcmToolKit::getLocationIdFromMappedId(switchId);
			NetconfStatusMessage *pMessage = new NetconfStatusMessage();
			pMessage->setRbridgeId(switchId);

			status = send(pMessage,
						reinterpret_cast<PrismMessageResponseHandler> (&FirmwareStatusObjectManager::NetconfStatusCallback),
						pNetconfStatusContext, 30000, locationId); 

			if (WAVE_MESSAGE_SUCCESS != status) {
				pNetconfStatusContext->incrementNumberOfFailures();
				WaveNs::trace(TRACE_LEVEL_INFO, string("FirmwareStatusOM::"
					"NetconfStatusHandler: Error in sending message to %d") +
					switchId);
				delete pMessage;
			} else {
				WaveNs::trace(TRACE_LEVEL_DEBUG, string("FirmwareStatusOM::"
					"NetconfStatusHandler: Message sent to local node %d") +
					switchId);
				++(*pNetconfStatusContext);
			}
		}

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
												pResultsClusterInfo);

		WaveNs::trace(TRACE_LEVEL_INFO, "FirmwareStatusOM:NetconfStatus"
					"Handler:Exit");

		--(*pNetconfStatusContext);
		pNetconfStatusContext->executeNextStep (((pNetconfStatusContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);

}

	void FirmwareStatusObjectManager::NetconfStatusCallback(FrameworkStatus frameworkStatus, NetconfStatusMessage *pMessage, PrismLinearSequencerContext *pNetconfStatusContext)
{
	--(*pNetconfStatusContext);

	UI32			switchId = 0;
	vector<UI32>	Index, MessageId;
	vector<string>	DateAndTime, Message, BladeName;

	if (FRAMEWORK_SUCCESS == frameworkStatus) {

		switchId = pMessage->getRbridgeId();
		if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus())) {
			WaveNs::trace(TRACE_LEVEL_INFO, string("FirmwareStatusOM:Netconf"
				"StatusCallback: Request failed for ") + switchId);
			pNetconfStatusContext->incrementNumberOfFailures();
		} else {

			WaveNs::trace(TRACE_LEVEL_INFO, string("FirmwareStatusOM:Netconf"
				"Status Response received from ") + switchId);

			FirmwareStatusMessage *pFirmwareStatusMessage = dynamic_cast<FirmwareStatusMessage *> (pNetconfStatusContext->getPPrismMessage());

			Index = pMessage->getIndex();
			MessageId = pMessage->getMessageId();
			DateAndTime = pMessage->getDateAndTime();
			Message = pMessage->getMessage();
			BladeName = pMessage->getBladeName();

			pFirmwareStatusMessage->setRbridgeId(switchId);
			pFirmwareStatusMessage->setFwdlState(pMessage->getFwdlState());
			pFirmwareStatusMessage->setNumOfEntries(pMessage->getNumOfEntries());
			pFirmwareStatusMessage->setIndex(Index);
			pFirmwareStatusMessage->setMessageId(MessageId);
			pFirmwareStatusMessage->setDateAndTime(DateAndTime);
			pFirmwareStatusMessage->setMessage(Message);
			pFirmwareStatusMessage->setBladeName(BladeName);

		}
	}

	if (pMessage)
		delete pMessage;

		WaveNs::trace(TRACE_LEVEL_INFO, "FirmwareStatusOM:NetconfStatus"
					"Callback:Exit");

	pNetconfStatusContext->executeNextStep (((pNetconfStatusContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);

}


}
