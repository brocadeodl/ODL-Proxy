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
 *   December, 2012                                                         *
 *   All rights reserved.                                                  *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "Ha/Local/HaOpTypes.h"
#include "Ha/Local/HaShowLocalMessage.h"
#include "Ha/Global/HaShowGlobalObjectManager.h"
#include "Ha/Global/HaShowClusterMessage.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

#include <string>
#include "errno.h"

namespace DcmNs
{

	HaShowGlobalObjectManager::HaShowGlobalObjectManager ()
		: WaveObjectManager  (getClassName ())
	{
		addOperationMap (HASHOWACTION, reinterpret_cast<PrismMessageHandler> (&HaShowGlobalObjectManager::HaShowClusterMessageHandler));
	}

	HaShowGlobalObjectManager::~HaShowGlobalObjectManager ()
	{
	}

	HaShowGlobalObjectManager  *HaShowGlobalObjectManager::getInstance()
	{
		static HaShowGlobalObjectManager *pHaShowGlobalObjectManager = new HaShowGlobalObjectManager ();

		WaveNs::prismAssert (NULL != pHaShowGlobalObjectManager, __FILE__, __LINE__);

		return (pHaShowGlobalObjectManager);
	}

	string  HaShowGlobalObjectManager::getClassName()
	{
		return ("Ha Show Operation Global Manager");
	}

	PrismServiceId  HaShowGlobalObjectManager::getPrismServiceId()
	{
		return ((getInstance ())->getServiceId ());
	}

	PrismMessage  *HaShowGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;

        trace (TRACE_LEVEL_DEBUG, "HaShowGlobalObjectManager::"
			"createMessageInstance : opCode = " + operationCode);

		switch (operationCode)
		{

			case HASHOWACTION :
				trace (TRACE_LEVEL_DEBUG, "HaShowGlobalObjectManager::"
					"createMessageInstance : HASHOWACTION");
				pPrismMessage = new HaShowClusterMessage();
				break;	
			default :
				pPrismMessage = NULL;
		}

		return (pPrismMessage);
	}

	bool HaShowGlobalObjectManager::isOperationAllowedBeforeEnabling (const UI32 &operationCode)
	{
		switch (operationCode) {
			case HASHOWACTION :
				trace (TRACE_LEVEL_DEBUG, "HaShowGlobalObjectManager::"
					"isOperationAllowedBeforeEnabling : HASHOWACTION");
				return (true);
			default:
				return (WaveObjectManager::isOperationAllowedBeforeEnabling (operationCode));
		}
	}

	vector<WaveManagedObject *> *HaShowGlobalObjectManager::DoSynchronousQuery (const string className)
	{
		vector<WaveManagedObject *> *pResults = querySynchronously (className);
		prismAssert (NULL != pResults, __FILE__, __LINE__);
		return pResults;
	}

	void  HaShowGlobalObjectManager::HaShowClusterMessageHandler(HaShowClusterMessage *pHaShowClusterMessage)
    {

		PrismLinearSequencerContext *pHaShowContext = NULL;
		PrismLinearSequencerStep sequencerSteps[] = {
			reinterpret_cast<PrismLinearSequencerStep>(&HaShowGlobalObjectManager::HaShowHandler),
			reinterpret_cast<PrismLinearSequencerStep>(&HaShowGlobalObjectManager::prismLinearSequencerSucceededStep),
			reinterpret_cast<PrismLinearSequencerStep>(&HaShowGlobalObjectManager::prismLinearSequencerFailedStep),
		};

		pHaShowContext = new PrismLinearSequencerContext (pHaShowClusterMessage, this,
			sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
		pHaShowContext->setPPrismMessage (pHaShowClusterMessage);
		pHaShowContext->start ();
    }

	void  HaShowGlobalObjectManager::HaShowHandler(PrismLinearSequencerContext *pHaShowContext)
	{
		ResourceId status = WAVE_MESSAGE_ERROR;
		ResourceId messageStatus = WAVE_MESSAGE_ERROR;
		HaShowLocalMessage *pMessage = NULL;
		UI32 mappedId = 0;
		string data = "";
		// MSG from Actionpoint worker to carry data
		HaShowClusterMessage *pHaShowClusterMessage = dynamic_cast<HaShowClusterMessage *> (pHaShowContext->getPPrismMessage ());
		WaveNs::trace (TRACE_LEVEL_DEBUG, string("HaShowGlobalObjectManager::Entering HaShowHandler"));
		// get cluser info
		vector<WaveManagedObject *> *pResultsClusterInfo = DoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
		prismAssert (NULL != pResultsClusterInfo, __FILE__, __LINE__);
		UI32 nodesInCluster = pResultsClusterInfo->size ();
		WaveNs::trace (TRACE_LEVEL_DEBUG, string("HaShowGlobalObjectManager::getClusterInfo: Getting information for : ")+ nodesInCluster + " switches");
		pHaShowContext->setNumberOfFailures (0);	
		UI32 i;
		for (i = 0; i < nodesInCluster; i++) {
			if (pHaShowClusterMessage->getMappedId() == 0) {
				VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject  =
					(dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i]));
				mappedId = pVcsNodeLocalManagedObject->getMappedId ();
			} else {
				mappedId = pHaShowClusterMessage->getMappedId ();
			}

			LocationId locationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
			tracePrintf(TRACE_LEVEL_DEBUG, "HaShowGlobalObjectManager::HaShowHandler "
					"mappedId=%d locationId=%d", mappedId, locationId);
			pMessage = new HaShowLocalMessage();
			pMessage->setMappedId(mappedId);
				// setup foscli cmd
			pMessage->setCmd(pHaShowClusterMessage->getCmd());
			WaveClientSessionContext waveClientSessionContext = pHaShowClusterMessage->getWaveClientSessionContext ();
			pMessage->setWaveClientOriginatingLocationId(pHaShowClusterMessage->getWaveClientOriginatingLocationId ());
			pMessage->setWaveNativeClientId(pHaShowClusterMessage->getWaveNativeClientId ());
			pMessage->setWaveUserClientId(pHaShowClusterMessage->getWaveUserClientId());

			status = sendSynchronously(pMessage, locationId);
			if (WAVE_MESSAGE_SUCCESS != status) {
				pHaShowContext->incrementNumberOfFailures ();
				trace (TRACE_LEVEL_INFO, string ("HaShowGlobalObjectManager::HaShowHandler:Sending a message to [") +
						PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) +
						" service] failed.");
				trace (TRACE_LEVEL_ERROR, "HaShowGlobalObjectManager::HaShowHandler:Error in sending message to get this node  : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");
				delete pMessage;
			} else {
				ResourceId completionStatus = pMessage->getCompletionStatus();
				if (WAVE_MESSAGE_SUCCESS != completionStatus) {
					trace (TRACE_LEVEL_ERROR, "HaShowGlobalObjectManager::HaShowHandler:getCompletionStatus Failed");
					delete pMessage;
				} else {
					trace (TRACE_LEVEL_INFO, "HaShowGlobalObjectManager::HaShowHandler: Appending data");
					data.append(pMessage->getData());
				}
			}
			if (pHaShowClusterMessage->getMappedId() != 0)
				break;
		}
		pHaShowClusterMessage->setData(data);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsClusterInfo);
		pHaShowContext->executeNextStep (((pHaShowContext->getNumberOfFailures ()) > 0) ?
				WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
		trace (TRACE_LEVEL_DEBUG, "HaShowGlobalObjectManager::HaShowHandler:  Done executing seq step");
	}
}
