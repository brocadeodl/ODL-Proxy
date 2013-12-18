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
#include "Ha/Local/HaOpActionMessage.h"
#include "Ha/Local/HaOpTypes.h"
#include "Ha/Global/HaOpGlobalObjectManager.h"
#include "Ha/Global/HaOpGlobalActionMessage.h"
#include "Ha/Global/HaOpGlobalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

#include <string>
#include "errno.h"

namespace DcmNs
{

	HaOpGlobalObjectManager::HaOpGlobalObjectManager ()
		: WaveObjectManager  (getClassName ())
	{
		HaOpGlobalManagedObject	HaOpGlobalManagedObject (this);
		HaOpGlobalManagedObject.setupOrm ();
		addManagedClass (HaOpGlobalManagedObject::getClassName ());
		addOperationMap (HAOPACTION, reinterpret_cast<PrismMessageHandler> (&HaOpGlobalObjectManager::HaOpActionMessageHandler));
	}

	HaOpGlobalObjectManager::~HaOpGlobalObjectManager ()
	{
	}

	HaOpGlobalObjectManager  *HaOpGlobalObjectManager::getInstance()
	{
		static HaOpGlobalObjectManager *pHaOpGlobalObjectManager = new HaOpGlobalObjectManager ();

		WaveNs::prismAssert (NULL != pHaOpGlobalObjectManager, __FILE__, __LINE__);

		return (pHaOpGlobalObjectManager);
	}

	string  HaOpGlobalObjectManager::getClassName()
	{
		return ("Ha Operation Global Manager");
	}

	PrismServiceId  HaOpGlobalObjectManager::getPrismServiceId()
	{
		return ((getInstance ())->getServiceId ());
	}

	PrismMessage  *HaOpGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;

        trace (TRACE_LEVEL_DEBUG, "HaOpGlobalObjectManager::"
			"createMessageInstance : opCode = " + operationCode);

		switch (operationCode)
		{

			case HAOPACTION :
        		trace (TRACE_LEVEL_DEBUG, "HaOpGlobalObjectManager::"
					"createMessageInstance : HAOPACTION");
				pPrismMessage = new HaOpGlobalActionMessage ();
				break;
			default :
				pPrismMessage = NULL;
		}

		return (pPrismMessage);
	}

	bool HaOpGlobalObjectManager::isOperationAllowedBeforeEnabling (const UI32 &operationCode)
	{
		switch (operationCode) {
				case HAOPACTION :
					return (true);
			default:
				return (WaveObjectManager::isOperationAllowedBeforeEnabling (operationCode));
		}
	}

	WaveManagedObject  *HaOpGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
	{
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((HaOpGlobalManagedObject::getClassName ()) == managedClassName) {
            pWaveManagedObject = new HaOpGlobalManagedObject(this);
        } else {
            trace (TRACE_LEVEL_FATAL, "HaOpGlobalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
	}

	vector<WaveManagedObject *> *HaOpGlobalObjectManager::DoSynchronousQuery (const string className)
	{
		vector<WaveManagedObject *> *pResults = querySynchronously (className);
		prismAssert (NULL != pResults, __FILE__, __LINE__);
		return pResults;
	}


	void  HaOpGlobalObjectManager::HaOpActionMessageHandler(HaOpGlobalActionMessage *pHaOpGlobalActionMessage)
    {

		PrismLinearSequencerContext *pHaOpContext = NULL;
		PrismLinearSequencerStep sequencerSteps[] = {
			reinterpret_cast<PrismLinearSequencerStep>(&HaOpGlobalObjectManager::HaReloadHandler),
			reinterpret_cast<PrismLinearSequencerStep>(&HaOpGlobalObjectManager::prismLinearSequencerSucceededStep),
			reinterpret_cast<PrismLinearSequencerStep>(&HaOpGlobalObjectManager::prismLinearSequencerFailedStep)
		};

		pHaOpContext = new PrismLinearSequencerContext (pHaOpGlobalActionMessage, this, 
			sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
		pHaOpContext->setPPrismMessage (pHaOpGlobalActionMessage);
		pHaOpContext->start ();

    }

#define LOCAL_MESSAGE 0
#define CLUSTER_MESSAGE 1
    void  HaOpGlobalObjectManager::HaReloadHandler(PrismLinearSequencerContext *pHaOpContext)
{
	UI32			switchId = 0;
	UI32			nodesInCluster, i, entire_cluster = 1;
	vector<UI32>	rbridgeIdsVector;
	ResourceId		status = WAVE_MESSAGE_ERROR;
	HaOpActionMessage *pMessage = NULL;
	string fosCommand;
	UI32 mappedId = 0;
	LocationId locationId;

	pHaOpContext->setNumberOfFailures(0);
	++(*pHaOpContext);

	HaOpGlobalActionMessage *pHaOpGlobalActionMessage = dynamic_cast<HaOpGlobalActionMessage *> (pHaOpContext->getPPrismMessage());

	// get cluser info vector
	vector<LocationId> fullyConnectedLocationIdVector;
	FrameworkToolKit::getFullyConnectedLocations (fullyConnectedLocationIdVector);
	fullyConnectedLocationIdVector.push_back(FrameworkToolKit::getThisLocationId());

	nodesInCluster = fullyConnectedLocationIdVector.size ();

    WaveNs::trace(TRACE_LEVEL_INFO, string("HaOM::HaReloadHandler: "
		"# of switches in cluster = ") + nodesInCluster);

	fosCommand = pHaOpGlobalActionMessage->getCommand();
	mappedId = pHaOpGlobalActionMessage->getMappedId();
	if (mappedId != 0) {
		entire_cluster = 0;
		nodesInCluster = 1;
		rbridgeIdsVector.push_back(mappedId);
    	WaveNs::trace(TRACE_LEVEL_INFO, string("HaOM::HaReloadHandler: "
			"mappidId = ") + mappedId + string(", send to one switch : ") +
			rbridgeIdsVector[0]);
	}

    for (i = 0; i < nodesInCluster; i++) {

        if (entire_cluster == 1) {
			locationId = fullyConnectedLocationIdVector[i];
			switchId = DcmToolKit::getMappedIdFromLocationId(locationId);
		} else {
			switchId = rbridgeIdsVector[i];
			locationId = DcmToolKit::getLocationIdFromMappedId(switchId);
		}

		WaveNs::trace(TRACE_LEVEL_INFO, string("HaOM::HaReloadHandler: "
			"Mapped ID = ") + switchId);

		startTransaction();
		HaOpGlobalManagedObject *pHaOpGlobalManagedObject = new HaOpGlobalManagedObject(this);
		updateWaveManagedObject(pHaOpGlobalManagedObject);
		pHaOpGlobalManagedObject->setRbridge_Id(switchId);
		status = commitTransaction();

		if (status == FRAMEWORK_SUCCESS) {
			trace (TRACE_LEVEL_INFO, string("HaOM::HaReloadHandler: "
				"Created new Managed Object."));

			pMessage = new HaOpActionMessage(fosCommand);
			if (nodesInCluster == 1) {
				pMessage->setFlag(LOCAL_MESSAGE);
			} else {
				pMessage->setFlag(CLUSTER_MESSAGE);
			}
			pMessage->setRbridgeId(switchId);
			status = send (pMessage,
				reinterpret_cast<PrismMessageResponseHandler> (&HaOpGlobalObjectManager::HaReloadCallback), pHaOpContext, 30000, locationId);

			if (WAVE_MESSAGE_SUCCESS != status) {
				pHaOpContext->incrementNumberOfFailures();
				trace(TRACE_LEVEL_ERROR, string("HaOM::HaReloadHandler: "
					"Error in sending message to node: ") + switchId);
				delete pMessage;
			} else {
				trace (TRACE_LEVEL_INFO, string("HaOM::HaReloadHandler: "
					"Message sent to local node: ") + switchId);
				++(*pHaOpContext);
			}
		} else {
			pHaOpContext->incrementNumberOfFailures ();
			trace(TRACE_LEVEL_ERROR, string("HaOM::HaReloadHandler: "
				"Managed Object creation failed."));
		}
	}

	trace(TRACE_LEVEL_INFO, string("HaObjectManager::HaReloadHandler: Exiting."));

	--(*pHaOpContext);
	pHaOpContext->executeNextStep (((pHaOpContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

	void  HaOpGlobalObjectManager::HaReloadCallback(FrameworkStatus frameworkStatus, HaOpActionMessage *pMessage, PrismLinearSequencerContext *pHaOpContext)
{
	--(*pHaOpContext);
	UI32 switchId = 0;

	if (FRAMEWORK_SUCCESS == frameworkStatus) {
		WaveNs::trace (TRACE_LEVEL_INFO, string ("Global HaOpOM::"
			"HaReloadCallback: FRAMEWORK_SUCCESS."));
		if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ())) {
			WaveNs::trace (TRACE_LEVEL_INFO, string ("Global HaOpOM::"
				"HaReloadCallback: Message receive Failure."));
			pHaOpContext->incrementNumberOfFailures ();
		} else {
			switchId = pMessage->getRbridgeId();
			WaveNs::trace(TRACE_LEVEL_INFO, string("Global HaOpOM::HaReload"
				"Callback: Message received successfully for : ") + switchId);
		}
	}

	if (NULL != pMessage)
		delete pMessage;

	trace (TRACE_LEVEL_INFO, string("HaOpGlobalObjectManager::"
		"HaReloadCallback: Exiting."));

	pHaOpContext->executeNextStep (((pHaOpContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void HaOpGlobalObjectManager::prismLinearSequencerFailedStep (PrismLinearSequencerContext *pHaOpContext)
{

	UI32 nodesInCluster, i, rbridge_id;
	//UI32 current_state;
	ResourceId status = WAVE_MESSAGE_ERROR;
	HaOpGlobalManagedObject *pHaOpGlobalManagedObject = NULL;

	WaveNs::trace (TRACE_LEVEL_INFO, string("HaOpOM::"
		"PrismLinearSequencerFailedStep: Entering ..."));
	vector<WaveManagedObject *> *pResults = querySynchronously (HaOpGlobalManagedObject::getClassName());
    prismAssert (NULL != pResults, __FILE__, __LINE__);
	nodesInCluster = pResults->size();
	for (i = 0; i < nodesInCluster; i++) {

		pHaOpGlobalManagedObject = dynamic_cast<HaOpGlobalManagedObject *> ((*pResults)[i]);
        prismAssert (NULL != pHaOpGlobalManagedObject, __FILE__, __LINE__);
		//current_state = pHaOpGlobalManagedObject->getState();
		rbridge_id = pHaOpGlobalManagedObject->getRbridge_Id();

		if (pHaOpGlobalManagedObject) {

			trace (TRACE_LEVEL_INFO, string("HaOpOM::PrismLSFailedStep"
				"Managed Object deleted for Rbridge-id # .") + rbridge_id);

			startTransaction();
			delete pHaOpGlobalManagedObject;
			status = commitTransaction ();

			if (status == FRAMEWORK_SUCCESS) {
				WaveNs::trace (TRACE_LEVEL_INFO, string("HaOpOM::Prism"
					"LSFailedStep: Commit transaction completed."));
			} else {
				WaveNs::trace (TRACE_LEVEL_INFO, string("HaOpOM::Prism"
					"LSFailedStep: Commit transaction failed."));
			}
		}
	}

	if (pResults)
		pResults->clear();

	PrismMessage             *pPrismMessage             = pHaOpContext->getPPrismMessage ();
	PrismAsynchronousContext *pPrismAsynchronousContext = pHaOpContext->getPPrismAsynchronousContext ();

	if (true == (pHaOpContext->getIsHoldAllRequested ())) {
		pHaOpContext->unholdAll ();
	}

	if (true == (pHaOpContext->getIsTransactionStartedByMe ())) {
		pHaOpContext->setIsTransactionStartedByMe (false);
		rollbackTransaction ();
	}

	if (NULL != pPrismMessage) {
		pPrismMessage->setCompletionStatus (pHaOpContext->getCompletionStatus ());
		reply (pPrismMessage);
	} else if (NULL != pPrismAsynchronousContext) {
		pPrismAsynchronousContext->setCompletionStatus (pHaOpContext->getCompletionStatus ());
		pPrismAsynchronousContext->callback ();
	}

	WaveNs::trace (TRACE_LEVEL_INFO, string("HaOpOM::"
		"PrismLinearSequencerFailedStep: Exiting ..."));
	delete pHaOpContext;
}


}
