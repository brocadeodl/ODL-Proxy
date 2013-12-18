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
 *   Author : James Chen                                               *
 ***************************************************************************/
#include "DcmShell/DcmShell.h"
#include "DcmCore/DcmToolKit.h"
#include "Shell/ShellDebug.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "RAS/Local/LoggingLocalManagedObject.h"
#include "RAS/Global/RASVCSObjectManager.h"
#include "RAS/Local/RASLoggingMessage.h"
#include "RAS/Global/RASManagedObject.h"
#include "ClientInterface/RAS/RASClientMessage.h"
#include "Shell/ShellPrism.h"
#include "vcs.h"		// Module name of RAS.yang
// * #include "RAS.h"   // this file may not require in hydra		// Module name of RAS.yang

namespace DcmNs
{

RASVCSObjectManager::RASVCSObjectManager () : WaveObjectManager (getClassName ())
{
	PrismFrameworkObjectManager::excludeServiceForClusterValidationPhase (getServiceId ());

	addOperationMap (RAS_VCSLOGGING, reinterpret_cast<PrismMessageHandler>
			(&RASVCSObjectManager::RASClientMessageHandler));
}

RASVCSObjectManager::~RASVCSObjectManager ()
{
}

RASVCSObjectManager *RASVCSObjectManager::getInstance ()
{
	static RASVCSObjectManager *pRASVCSObjectManager = new RASVCSObjectManager ();

	WaveNs::prismAssert (NULL != pRASVCSObjectManager, __FILE__, __LINE__);

	return (pRASVCSObjectManager);
}

PrismServiceId RASVCSObjectManager::getPrismServiceId ()
{
	return ((getInstance ())->getServiceId ());
}

void RASVCSObjectManager::failover (FrameworkObjectManagerFailoverReason failoverReason, vector<LocationId> failedLocationIds, PrismAsynchronousContext *pPrismAsynchronousContext)
{
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	trace (TRACE_LEVEL_DEBUG, string ("RASVCSOM::failover failover reason") +
			FrameworkToolKit::localize(failoverReason));

	if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == failoverReason) {
		UI32 noOfFailedLocations = failedLocationIds.size ();

		for (UI32 i = 0; i < noOfFailedLocations; i++) {
			vector<WaveManagedObject *> *pWaveManagedObjects =
				querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i],
						LoggingLocalManagedObject::getClassName ());

			trace (TRACE_LEVEL_INFO, string ("RASVCSOM::failover for Location Id ") +
					failedLocationIds[i]);

			startTransaction ();

			UI32 sizeOfManagedObjects = pWaveManagedObjects->size ();

			for (UI32 j = 0; j < sizeOfManagedObjects; j++) {
				delete (*pWaveManagedObjects)[j];
			}   

			status = commitTransaction ();

			if (FRAMEWORK_SUCCESS != status)
			{
				trace (TRACE_LEVEL_FATAL, "RASVCSOM::failover can not commit local managed object delete to database ");
				prismAssert (false, __FILE__, __LINE__);
			}

			pWaveManagedObjects->clear ();

			delete pWaveManagedObjects;
		}
	}
	pPrismAsynchronousContext->setCompletionStatus (status);
	pPrismAsynchronousContext->callback ();
}



vector<WaveManagedObject *> *RASVCSObjectManager::DoSynchronousQuery (const string className)
{
	vector<WaveManagedObject *> *pResults = querySynchronously (className);
	prismAssert (NULL != pResults, __FILE__, __LINE__);
	return pResults;
}

WaveManagedObject *RASVCSObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    return NULL;
}

PrismMessage *RASVCSObjectManager::createMessageInstance (const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;

	tracePrintf(TRACE_LEVEL_DEBUG,
			"RASVCSOM::createMessageInstance opcode=%d", operationCode);

	switch (operationCode)
	{
		case RAS_VCSLOGGING :
			trace (TRACE_LEVEL_DEBUG, "RASVCSObjectManager::createMessageInstance:RAS_VCSLOGGING");
			pPrismMessage = new RASClientMessage ();
			break;
		default :
			tracePrintf(TRACE_LEVEL_INFO,
					"RASVCSObjectManager::createMessageInstance Invalid opcode=%d", operationCode);
			pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

/************************************************************************
  * DCM Client Message Handler
  ***********************************************************************/
void RASVCSObjectManager::RASClientMessageHandler (RASClientMessage *pRASClientMessage)
{
	tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSOM:RASClientMessageHandler VER= 0x%x MAGIC=0x%x ",
			pRASClientMessage->m_hdr_ver, pRASClientMessage->m_hdr_magic);

	PrismLinearSequencerStep sequencerSteps[] = {
		reinterpret_cast<PrismLinearSequencerStep> (&RASVCSObjectManager::RASVCSMessage),
		reinterpret_cast<PrismLinearSequencerStep> (&RASVCSObjectManager::prismLinearSequencerSucceededStep),
		reinterpret_cast<PrismLinearSequencerStep> (&RASVCSObjectManager::prismLinearSequencerFailedStep),
	};

	PrismLinearSequencerContext *pRASMessagingContext = new PrismLinearSequencerContext (pRASClientMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

	pRASMessagingContext->start ();
	tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSObjectM:RASClientMessageHandler exit");
}


void RASVCSObjectManager::RASVCSMessage(PrismLinearSequencerContext *pRASMessagingContext)
{
	ResourceId status = WAVE_MESSAGE_ERROR;
	const ObjectId	pOID;   

	tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSOM:RASVCSMessage");

	RASClientMessage *pRASClientMessage = dynamic_cast<RASClientMessage *> (pRASMessagingContext->getPPrismMessage ());

#define RASEVT_MAGIC	   0x12
#define	RASEVT_VER_STORAGE  5
	if ((pRASClientMessage->m_hdr_ver != RASEVT_VER_STORAGE) || 
			(pRASClientMessage->m_hdr_magic != RASEVT_MAGIC)) {
		tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSOM:RASClientMessageH fail VER=0x%x 0x%x MAGIC=0x%x 0x%x",
				RASEVT_VER_STORAGE, pRASClientMessage->m_hdr_ver,
				RASEVT_MAGIC, pRASClientMessage->m_hdr_magic);
		pRASMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
		return;
	}

	// get cluser info vector
	vector<WaveManagedObject *> *pResultsClusterInfo = DoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
	prismAssert (NULL != pResultsClusterInfo, __FILE__, __LINE__);
	UI32 nodesInCluster = pResultsClusterInfo->size ();
	tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSObjectM:RASVCSMessage: %d nodes SMID=%d",
			nodesInCluster, pRASClientMessage->getMappedId());

	pRASMessagingContext->setNumberOfFailures (0);
	++(*pRASMessagingContext);
	UI32 i;
	for (i = 0; i < nodesInCluster; i++) {
		VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject  =
			(dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i]));

		LocationId locationId = DcmToolKit::getLocationIdFromMappedId (pVcsNodeLocalManagedObject->getMappedId ());
		tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSObjectM:VCSMSG:LocationId=0x%x", locationId);

		// skip the switch which initiate the logging
		if (pVcsNodeLocalManagedObject->getMappedId () == pRASClientMessage->getMappedId() || (locationId == 0)) {
			continue;
		}
		const UI32	opcode = RAS_VCSLOGGING_LOCAL;
		RASClientMessage *pMessage = new RASClientMessage (opcode);
		prismAssert (pMessage, __FILE__, __LINE__);

		pMessage->setAll(*pRASClientMessage);

		// no timeout
		status = send (pMessage,
				reinterpret_cast<PrismMessageResponseHandler> (&RASVCSObjectManager::RASVCSMessageCallback),
				pRASMessagingContext, 0, locationId);
		if (WAVE_MESSAGE_SUCCESS != status) {
			pRASMessagingContext->incrementNumberOfFailures ();
			tracePrintf(TRACE_LEVEL_ERROR, "RASVCSObjectM:VCSMSG:Error in sending message to LocationId=0x%x",
					locationId);
			delete pMessage;	  
		} else {
			++(*pRASMessagingContext);
		}
	}

	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsClusterInfo);
	--(*pRASMessagingContext);

	tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSObjectM:VCSMSG:Pending=0x%x",
			pRASMessagingContext->getNumberOfCallbacksBeforeAdvancingToNextStep());

	pRASMessagingContext->executeNextStep (((pRASMessagingContext->getNumberOfFailures ()) > 0) ?
			WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void  RASVCSObjectManager::RASVCSMessageCallback (FrameworkStatus frameworkStatus,
   RASClientMessage *pMessage, PrismLinearSequencerContext *pRASMessagingContext)
{
	--(*pRASMessagingContext);

	// get Original request msg
	RASClientMessage *pRASClientMessage = dynamic_cast<RASClientMessage *> (pRASMessagingContext->getPPrismMessage ());

	if (FRAMEWORK_SUCCESS == frameworkStatus) {

		prismAssert (pMessage, __FILE__, __LINE__);

		tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSObjectManager::RASVCSMessageCB: SMID=%d LocalMID=%d",
				pMessage->getMappedId(), DcmToolKit::getLocalMappedId());

		if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ())) {
			tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSObjectManager::RASVCSMessageCallback:"
					" Completion Failure");
			pRASClientMessage->setData(string ("switchid:") +
					DcmToolKit::getLocalMappedId() + string (" Fail\n"));
			pRASMessagingContext->incrementNumberOfFailures ();

		} else {
			// add returned result
			tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSObjectM:RASVCSMessageCB: LMID=%d Success",
					DcmToolKit::getLocalMappedId());

			pRASClientMessage->setData(string ("switchid:") + DcmToolKit::getLocalMappedId()
					+ string (" Success\n"));
			pRASClientMessage->setData(pMessage->getData());
		}

	} else {

		WaveNs::trace (TRACE_LEVEL_INFO, string ("RASVCSOM::copySupportAsyncMSGCB:FRAMEWROK Error") + frameworkStatus);

		pRASClientMessage->setData(string ("switchid:") +
				DcmToolKit::getLocalMappedId() + string (" Fail\n"));
		pRASMessagingContext->incrementNumberOfFailures ();
	}

	if (NULL != pMessage)
		delete pMessage;

	tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSObjectM:VCSMSG:CB PendingCB=0x%x",
			pRASMessagingContext->getNumberOfCallbacksBeforeAdvancingToNextStep());

	pRASMessagingContext->executeNextStep (((pRASMessagingContext->getNumberOfFailures ()) > 0) ?
			WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}
}

