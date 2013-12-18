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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "VcsFabric/Local/Test/VcsNodeLocalMessagingTestObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "VcsFabric/Local/VcsNodeGenerateSCNMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"

namespace DcmNs
{

#define NUM_MSGS  100 

VcsNodeLocalMessagingTestObjectManager::VcsNodeLocalMessagingTestObjectManager ()
    : PrismTestObjectManager ("Vcs Node Local Messaging Test")
{
}

VcsNodeLocalMessagingTestObjectManager *VcsNodeLocalMessagingTestObjectManager::getInstance ()
{
    static VcsNodeLocalMessagingTestObjectManager *pVcsNodeLocalMessagingTestObjectManager = NULL;

    if (NULL == pVcsNodeLocalMessagingTestObjectManager)
    {
        pVcsNodeLocalMessagingTestObjectManager = new VcsNodeLocalMessagingTestObjectManager ();
        WaveNs::prismAssert (NULL != pVcsNodeLocalMessagingTestObjectManager, __FILE__, __LINE__);
    }

    return (pVcsNodeLocalMessagingTestObjectManager);
}

VcsNodeLocalMessagingTestObjectManager::~VcsNodeLocalMessagingTestObjectManager ()
{
}

PrismServiceId VcsNodeLocalMessagingTestObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void VcsNodeLocalMessagingTestObjectManager::testRequestHandler (RegressionTestMessage *pMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleAsyncSetVcsIdTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleSyncSetVcsIdTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleAsyncGetVcsIdTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleSyncGetVcsIdTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleAsyncSetIsPrincipalTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleSyncSetIsPrincipalTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleAsyncGetIsPrincipalTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleSyncGetIsPrincipalTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleAsyncGetSwitchMacTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleSyncGetSwitchMacTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleAsyncGetLocalMappedIdTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleSyncGetLocalMappedIdTest),
		// TBD - disabling for now as setting vcs mode reboots switch
        //reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleAsyncSetVcsModeTest),
        //reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleSyncSetVcsModeTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleAsyncGetVcsModeTest),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleSyncGetVcsModeTest),
        // The following test sends events for its own domain id. Appropriate domain id should be set or else we will assert when we get them.
        //reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleEventTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::simpleDcmToolKitTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeLocalMessagingTestObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void VcsNodeLocalMessagingTestObjectManager::simpleAsyncSetVcsIdTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging for Set VCSID.");

    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    UI32                i = 0;
   
    vector<PrismMessage *> messagesVector;
	UI32 numberOfMessagesToSend = 0;

    for (i = 0; i < NUM_MSGS; i++)
    {
        VcsNodeSetVcsIdMessage *pMessage;
        pMessage = new VcsNodeSetVcsIdMessage (5);    
        messagesVector.push_back (pMessage);
    }

	numberOfMessagesToSend = messagesVector.size ();

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

	trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

	++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        status = send (messagesVector[i],
                       reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeLocalMessagingTestObjectManager::setVcsIdMessageCallback),
                       pPrismLinearSequencerContext);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_DEBUG, string ("VcsNodeLocalMessagingTestObjectManager::simpleAsyncSetVcsIdTest : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId ((messagesVector[i])->getSenderServiceCode ()) + " service] failed.");
            delete messagesVector[i];
			break;
        }
        else
        {
            ++(*pPrismLinearSequencerContext);
        }
    }

	--(*pPrismLinearSequencerContext);

	status = (((pPrismLinearSequencerContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
	
    pPrismLinearSequencerContext->executeNextStep(status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleSyncSetVcsIdTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
	UI32 i = 0;

    trace (TRACE_LEVEL_INFO, "Starting Synchronous Messaging for Set VCSID.");

    WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;

    VcsNodeSetVcsIdMessage message (5);

	trace (TRACE_LEVEL_INFO, string ("    Sending ") + NUM_MSGS + " messages.");

    for (i = 0; i < NUM_MSGS; i++)
    {
        status = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, string ("VcsNodeLocalMessagingTestObjectManager::simpleSyncSetVcsIdTest : Failed sending a message synchronously. status = ") + status + ".");
			status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
				if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_DISABLED) {
		            trace (TRACE_LEVEL_INFO, string ("VcsNodeLocalMessagingTestObjectManager::simpleSyncSetVcsIdTest : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
					status = WAVE_MESSAGE_ERROR;
				} else {
					trace (TRACE_LEVEL_DEBUG, "VcsNodeLocalMessagingTestObjectManager::simpleSyncSetVcsIdTest : Return success when vcs mode is enabled.");
				}
            }
        }
		if (WAVE_MESSAGE_SUCCESS != status) {
			break;
		}
    }	

    pPrismLinearSequencerContext->executeNextStep (status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleAsyncGetVcsIdTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging for Get VCSID.");
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    UI32                i = 0;
   
    vector<PrismMessage *> messagesVector;

    for (i = 0; i < NUM_MSGS; i++)
    {
        VcsNodeGetVcsIdMessage *pMessage;
        pMessage = new VcsNodeGetVcsIdMessage();    
        messagesVector.push_back (pMessage);
    }

    status = simpleAsynchronousMessageTest (pPrismLinearSequencerContext, messagesVector);
    pPrismLinearSequencerContext->executeNextStep(status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleSyncGetVcsIdTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    trace (TRACE_LEVEL_INFO, "Starting Synchronous Messaging for Get VCSID.");

    VcsNodeGetVcsIdMessage message;
    status = simpleSynchronousMessageTest (&message);
    pPrismLinearSequencerContext->executeNextStep (status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleAsyncSetIsPrincipalTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging for Set IsPrincipal.");

    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    UI32                i = 0;
   
    vector<PrismMessage *> messagesVector;

    for (i = 0; i < NUM_MSGS; i++)
    {
        VcsNodeSetIsPrincipalMessage *pMessage;
        pMessage = new VcsNodeSetIsPrincipalMessage (false);    
        messagesVector.push_back (pMessage);
    }

    status = simpleAsynchronousMessageTest (pPrismLinearSequencerContext, messagesVector);
    pPrismLinearSequencerContext->executeNextStep(status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleSyncSetIsPrincipalTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Synchronous Messaging for Set isPrincipal.");

    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    VcsNodeSetIsPrincipalMessage message (false);
    status = simpleSynchronousMessageTest (&message);
    pPrismLinearSequencerContext->executeNextStep (status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleAsyncGetIsPrincipalTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging for Get isPrincipal.");
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    UI32                i = 0;
   
    vector<PrismMessage *> messagesVector;

    for (i = 0; i < NUM_MSGS; i++)
    {
        VcsNodeGetIsPrincipalMessage *pMessage;
        pMessage = new VcsNodeGetIsPrincipalMessage();    
        messagesVector.push_back (pMessage);
    }

    status = simpleAsynchronousMessageTest (pPrismLinearSequencerContext, messagesVector);
    pPrismLinearSequencerContext->executeNextStep(status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleSyncGetIsPrincipalTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    trace (TRACE_LEVEL_INFO, "Starting Synchronous Messaging for Get isPrincipal.");

    VcsNodeGetIsPrincipalMessage message;
    status = simpleSynchronousMessageTest (&message);
    pPrismLinearSequencerContext->executeNextStep (status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleAsyncGetSwitchMacTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging for Get SwitchMac.");
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    UI32                i = 0;
   
    vector<PrismMessage *> messagesVector;

    for (i = 0; i < NUM_MSGS; i++)
    {
        VcsNodeGetSwitchMacMessage *pMessage;
        pMessage = new VcsNodeGetSwitchMacMessage();    
        messagesVector.push_back (pMessage);
    }

    status = simpleAsynchronousMessageTest (pPrismLinearSequencerContext, messagesVector);
    pPrismLinearSequencerContext->executeNextStep(status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleSyncGetSwitchMacTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    trace (TRACE_LEVEL_INFO, "Starting Synchronous Messaging for Get SwitchMac.");

    VcsNodeGetSwitchMacMessage message;
    status = simpleSynchronousMessageTest (&message);
    pPrismLinearSequencerContext->executeNextStep (status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleAsyncGetLocalMappedIdTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging for Get LocalMappedId.");
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    UI32                i = 0;
   
    vector<PrismMessage *> messagesVector;

    for (i = 0; i < NUM_MSGS; i++)
    {
        VcsNodeGetLocalMappedIdMessage *pMessage;
        pMessage = new VcsNodeGetLocalMappedIdMessage();    
        messagesVector.push_back (pMessage);
    }

    status = simpleAsynchronousMessageTest (pPrismLinearSequencerContext, messagesVector);
    pPrismLinearSequencerContext->executeNextStep(status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleSyncGetLocalMappedIdTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    trace (TRACE_LEVEL_INFO, "Starting Synchronous Messaging for Get LocalMappedId.");

    VcsNodeGetLocalMappedIdMessage message;
    status = simpleSynchronousMessageTest (&message);
    pPrismLinearSequencerContext->executeNextStep (status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleAsyncSetVcsModeTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging for Set VCSMODE.");

    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    UI32                i = 0;
   
    vector<PrismMessage *> messagesVector;

    for (i = 0; i < NUM_MSGS; i++)
    {
        // TBD - disabling for now as setting vcs mode reboots switch
        //VcsNodeSetVcsModeMessage *pMessage;
        //pMessage = new VcsNodeSetVcsModeMessage (false);    
        VcsNodeSetVcsIdMessage *pMessage;
        pMessage = new VcsNodeSetVcsIdMessage (5);    
        messagesVector.push_back (pMessage);
    }

    status = simpleAsynchronousMessageTest (pPrismLinearSequencerContext, messagesVector);
    pPrismLinearSequencerContext->executeNextStep(status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleSyncSetVcsModeTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Synchronous Messaging for Set VCSMODE.");

    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    //VcsNodeSetVcsModeMessage message (false);
    VcsNodeSetVcsIdMessage message (5);
    status = simpleSynchronousMessageTest (&message);
    pPrismLinearSequencerContext->executeNextStep (status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleAsyncGetVcsModeTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging for Get VCSMODE.");
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    UI32                i = 0;
   
    vector<PrismMessage *> messagesVector;

    for (i = 0; i < NUM_MSGS; i++)
    {
        VcsNodeGetVcsModeMessage *pMessage;
        pMessage = new VcsNodeGetVcsModeMessage();    
        messagesVector.push_back (pMessage);
    }

    status = simpleAsynchronousMessageTest (pPrismLinearSequencerContext, messagesVector);
    pPrismLinearSequencerContext->executeNextStep(status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleSyncGetVcsModeTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    trace (TRACE_LEVEL_INFO, "Starting Synchronous Messaging for Get VCSMODE.");

    VcsNodeGetVcsModeMessage message;
    status = simpleSynchronousMessageTest (&message);
    pPrismLinearSequencerContext->executeNextStep (status);
}

void VcsNodeLocalMessagingTestObjectManager::simpleEventTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Event broadcasting Test.");

    ResourceId            status           = WAVE_MESSAGE_SUCCESS;
    ResourceId            completionStatus = WAVE_MESSAGE_SUCCESS;

    UI32                   numberOfEventsToSend   = 100;
    UI32                   i                        = 0;
    UI32                   j                        = 0;

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfEventsToSend + " Events.");

    for (j = 0; j < 5; j++)
    {
    for (i = 0; i < numberOfEventsToSend; i++)
    {
        VcsNodeGenerateSCNMessage   *message = new VcsNodeGenerateSCNMessage ();
        message->setSCNType (j);

        // First get the local domainId
        const ObjectId  localSwitchObjectId = (VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeObjectId ();
        VcsNodeLocalManagedObject * pVcsNodeLocalManagedObject = getSwitchObject(localSwitchObjectId);
        prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);

        message->setDomainId (pVcsNodeLocalManagedObject->getDomainId ());

        status = sendSynchronously (message);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            completionStatus = message->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                status = WAVE_MESSAGE_ERROR;
            }
        }
    }
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}


void VcsNodeLocalMessagingTestObjectManager::simpleDcmToolKitTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    UI32              i                      = 0;              
    WaveMessageStatus status                 = WAVE_MESSAGE_SUCCESS;
    trace (TRACE_LEVEL_INFO, "Starting Messaging for DcmToolKit methods.");

    for (i = 0; i < NUM_MSGS; i++)
    {
        if (-1 == DcmToolKit::getVcsId ())
        {
            status = WAVE_MESSAGE_ERROR;
            break;
        }

        // First get the local domainId
        const ObjectId  localSwitchObjectId = (VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeObjectId ();
        VcsNodeLocalManagedObject * pVcsNodeLocalManagedObject = getSwitchObject(localSwitchObjectId);
        prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);

        if (-1 == DcmToolKit::getMappedIdFromDomainId (pVcsNodeLocalManagedObject->getDomainId ()))
        {
            status = WAVE_MESSAGE_ERROR;
            break;
        }

        if (-1 == DcmToolKit::getDomainIdFromMappedId (pVcsNodeLocalManagedObject->getMappedId ()))
        {
            status = WAVE_MESSAGE_ERROR;
            break;
        }
        if (-1 == DcmToolKit::getLocalMappedId ())
        {
            status = WAVE_MESSAGE_ERROR;
            break;
        }
        if (-1 == DcmToolKit::getVcsMode ())
        {
            status = WAVE_MESSAGE_ERROR;
            break;
        }
        if (0 == DcmToolKit::getLocationIdFromMappedId (pVcsNodeLocalManagedObject->getMappedId ()))
        {
            status = WAVE_MESSAGE_ERROR;
            break;
        }
        if (0 == DcmToolKit::getMappedIdFromLocationId (pVcsNodeLocalManagedObject->getOwnerLocationId ()))
        {
            status = WAVE_MESSAGE_ERROR;
            break;
        }
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

/**********************************************
* General functions
*************************************************/

WaveMessageStatus VcsNodeLocalMessagingTestObjectManager::simpleAsynchronousMessageTest (PrismLinearSequencerContext *&pPrismLinearSequencerContext, vector<PrismMessage *> &messagesVector)
{
    UI32               i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    UI32 numberOfMessagesToSend = messagesVector.size ();

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        status = send (messagesVector[i],
                       reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeLocalMessagingTestObjectManager::setVcsNodeMessageCallback),
                       pPrismLinearSequencerContext);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_DEBUG, string ("VcsNodeLocalMessagingTestObjectManager::simpleAsyncSetVcsIdTest : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId ((messagesVector[i])->getSenderServiceCode ()) + " service] failed.");
            delete messagesVector[i];
            return (status);
        }
        else
        {
            ++(*pPrismLinearSequencerContext);
        }
    }

    --(*pPrismLinearSequencerContext);
    //pPrismLinearSequencerContext->executeNextStep (((pPrismLinearSequencerContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
    status = (((pPrismLinearSequencerContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
    return (status);
}

WaveMessageStatus VcsNodeLocalMessagingTestObjectManager::simpleSynchronousMessageTest (PrismMessage *message)
{
    UI32             i                      = 0;
    WaveMessageStatus status = WAVE_MESSAGE_ERROR;

    //if (0 != (m_regressionInput.size ()))
    //{
     //   numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    //}

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + NUM_MSGS + " messages.");

    for (i = 0; i < NUM_MSGS; i++)
    {
        status = sendSynchronously (message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, string ("VcsNodeLocalMessagingTestObjectManager::simpleSyncSetVcsIdTest : Failed sending a message synchronously. status = ") + status + ".");
            return (WAVE_MESSAGE_ERROR);
        }
        else
        {
            ResourceId completionStatus = message->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_DEBUG, string ("VcsNodeLocalMessagingTestObjectManager::simpleSyncSetVcsIdTest : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                return (WAVE_MESSAGE_ERROR);
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeLocalMessagingTestObjectManager::setVcsNodeMessageCallback (FrameworkStatus frameworkStatus, ManagementInterfaceMessage *pMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    --(*pPrismLinearSequencerContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pMessage, __FILE__, __LINE__);

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ()))
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
        }

        delete pMessage;
    }
    else
    {
        pPrismLinearSequencerContext->incrementNumberOfFailures ();

        if (NULL != pMessage)
        {
            delete pMessage;
        }
    }

    pPrismLinearSequencerContext->executeNextStep (((pPrismLinearSequencerContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

VcsNodeLocalManagedObject * VcsNodeLocalMessagingTestObjectManager::getSwitchObject(const ObjectId &switchObjectId)
{
	vector<ObjectId> switchObjectIds;
	switchObjectIds.insert(switchObjectIds.begin(), 1, switchObjectId);
	VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;

	vector<WaveManagedObject *> *pResults = querySynchronously (VcsNodeLocalManagedObject::getClassName (), switchObjectIds);
	pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *>((*pResults)[0]);
	delete pResults;
   	return pVcsNodeLocalManagedObject;
}

void VcsNodeLocalMessagingTestObjectManager::setVcsIdMessageCallback (FrameworkStatus frameworkStatus, ManagementInterfaceMessage *pMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    --(*pPrismLinearSequencerContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pMessage, __FILE__, __LINE__);

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ()))
        {
			// Set VCSID test will return WAVE_MESSAGE_ERROR if vcs mode is enabled,
			// so consider as success if we get error.

			if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_DISABLED)
	        {    	       
            	pPrismLinearSequencerContext->incrementNumberOfFailures ();
	        } else {
				trace (TRACE_LEVEL_DEBUG, "VcsNodeLocalMessagingTestObjectManager::setVcsIdMessageCallback : Return success when vcs mode is enabled.");
			}			
        }

        delete pMessage;
    }
    else
    {
        pPrismLinearSequencerContext->incrementNumberOfFailures ();

        if (NULL != pMessage)
        {
            delete pMessage;
        }
    }

    pPrismLinearSequencerContext->executeNextStep (((pPrismLinearSequencerContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}
}
