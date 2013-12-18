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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/Test/FrameworkTestability1ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestabilityMessages.h"
#include "Framework/Core/Test/FrameworkTestabilityManagementInterfaceMessages.h"
#include "Framework/Messaging/Local/Test/FrameworkTestabilityEvents.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestTypes.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestObjectManager.h"
#include "ManagementInterface/ClientInterface/Test/ManagementInterfaceClientTestMessages.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "Framework/Messaging/MessageTracker/MessageTracker.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/ObjectTracker/ObjectTracker.h"
#include "Framework/ObjectModel/WaveSendToClientsContext.h"
#include <stdio.h>

namespace WaveNs
{

static UI32       s_numberOfProcessedMessage2s         = 0;
static PrismMutex s_numberOfProcessedMessage2sMutex;
static bool       s_canProceedToProcessMessage2s       = false;
static PrismMutex s_canProceedToProcessMessage2sMutex;

static UI32       s_numberOfProcessedMessage7s         = 0;
static PrismMutex s_numberOfProcessedMessage7sMutex;

static bool       s_areMessage6sReceived               = false;
static PrismMutex s_areMessage6sReceivedMutex;

FrameworkTestability1ObjectManager::FrameworkTestability1ObjectManager ()
    : WaveLocalObjectManager (getPrismServiceName ())
{
    WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient ("Wave Client", &FrameworkTestability1ObjectManager::createManagementInterfaceMessageInstance);
    addOperationMap (FRAMEWORK_TESTABILITY_MESSAGE1,                      reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityMessage1RequestHandler));
    addOperationMap (FRAMEWORK_TESTABILITY_MESSAGE2,                      reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityMessage2RequestHandler));
    addOperationMap (FRAMEWORK_TESTABILITY_MESSAGE3,                      reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityMessage3RequestHandler));
    addOperationMap (FRAMEWORK_TESTABILITY_MESSAGE4,                      reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityMessage4RequestHandler));
    addOperationMap (FRAMEWORK_TESTABILITY_MESSAGE5,                      reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityMessage5RequestHandler));
    addOperationMap (FRAMEWORK_TESTABILITY_MESSAGE6,                      reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityMessage6RequestHandler));
    addOperationMap (FRAMEWORK_TESTABILITY_MESSAGE7,                      reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityMessage7RequestHandler));

    addOperationMap (FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE1, reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage1RequestHandler));
    addOperationMap (FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE2, reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage2RequestHandler));
    addOperationMap (FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE3, reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage3RequestHandler));
    addOperationMap (FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE4, reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage4RequestHandler));
    addOperationMap (FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE_FOR_MEMORY_LEAKS, reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessageLeakMessageRequestHandler)); 
    addOperationMap (FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE_FOR_OBJECT_LEAKS, reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceObjectLeakMessageRequestHandler)); 
    addOperationMap (FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_FILE_SERVICE_USAGE, reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfacFileServiceUsageRequestHandler)); 
    addOperationMap (FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_TRACK_LEAKS_WITH_BT, reinterpret_cast<PrismMessageHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceTrackLeaksWithBtMessageRequestHandler)); 

}

FrameworkTestability1ObjectManager *FrameworkTestability1ObjectManager::getInstance ()
{
    static FrameworkTestability1ObjectManager *pFrameworkTestability1ObjectManager = NULL;

    if (NULL == pFrameworkTestability1ObjectManager)
    {
        pFrameworkTestability1ObjectManager = new FrameworkTestability1ObjectManager ();
        WaveNs::prismAssert (NULL != pFrameworkTestability1ObjectManager, __FILE__, __LINE__);
    }

    return (pFrameworkTestability1ObjectManager);
}

FrameworkTestability1ObjectManager::~FrameworkTestability1ObjectManager ()
{
}

PrismServiceId FrameworkTestability1ObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string FrameworkTestability1ObjectManager::getPrismServiceName ()
{
    return ("Framework Testability1");
}

void FrameworkTestability1ObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (FrameworkLocalMessagingTestObjectManager::getPrismServiceId (), FRAMEWORK_MESSAGING_LOCAL_TEST_EVENT1, reinterpret_cast<PrismEventHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityEvent1EventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

PrismMessage *FrameworkTestability1ObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case FRAMEWORK_TESTABILITY_MESSAGE1 :
            pPrismMessage = new FrameworkTestabilityMessage1;
            break;

        case FRAMEWORK_TESTABILITY_MESSAGE2 :
            pPrismMessage = new FrameworkTestabilityMessage2;
            break;

        case FRAMEWORK_TESTABILITY_MESSAGE3 :
            pPrismMessage = new FrameworkTestabilityMessage3;
            break;

        case FRAMEWORK_TESTABILITY_MESSAGE4 :
            pPrismMessage = new FrameworkTestabilityMessage4;
            break;

        case FRAMEWORK_TESTABILITY_MESSAGE5 :
            pPrismMessage = new FrameworkTestabilityMessage5;
            break;

        case FRAMEWORK_TESTABILITY_MESSAGE6 :
            pPrismMessage = new FrameworkTestabilityMessage6;
            break;

        case FRAMEWORK_TESTABILITY_MESSAGE7 :
            pPrismMessage = new FrameworkTestabilityMessage7;
            break;

        case FRAMEWORK_TESTABILITY_MESSAGE8 :
            pPrismMessage = new FrameworkTestabilityMessage8;
            break;

        case FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE1:
            pPrismMessage = new FrameworkTestabilityManagementInterfaceMessage1;
            break;

        case FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE2:
            pPrismMessage = new FrameworkTestabilityManagementInterfaceMessage2;
            break;

        case FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE3:
            pPrismMessage = new FrameworkTestabilityManagementInterfaceMessage3;
            break;

        case FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE4:
            pPrismMessage = new FrameworkTestabilityManagementInterfaceMessage4;
            break;

        case FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE_FOR_MEMORY_LEAKS:
            pPrismMessage = new FrameworkTestabilityManagementInterfaceMessageLeakMessage;
            break;

        case FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE_FOR_OBJECT_LEAKS:
            pPrismMessage = new FrameworkTestabilityManagementInterfaceObjectLeakMessage;
            break;

        case FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_FILE_SERVICE_USAGE:
            pPrismMessage = new FrameworkTestabilityManagementInterfaceFileServiceUsageMessage;
            break;
            
        case FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_TRACK_LEAKS_WITH_BT:
            pPrismMessage = new FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage;
            break;
            
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

ManagementInterfaceMessage *FrameworkTestability1ObjectManager::createManagementInterfaceMessageInstance (const UI32 &operationCode)
{
    ManagementInterfaceMessage *pManagementInterfaceMessage = NULL;

    switch (operationCode)
    {
        case 0:
            pManagementInterfaceMessage = new ManagementInterfaceClientTestMessage1 ();
            break;

        case 1:
            pManagementInterfaceMessage = new ManagementInterfaceClientTestMessage2 ();
            break;

        case 2:
            pManagementInterfaceMessage = new ManagementInterfaceClientTestMessage3 ();
            break;
        case 3:
            pManagementInterfaceMessage = new ManagementInterfaceClientTestMessage4 ();
            break;
        default:

            WaveNs::trace (TRACE_LEVEL_ERROR, string ("clientManagementInterfaceMessageInstantiator : Unknown Operation Code : ") + operationCode);
            pManagementInterfaceMessage = NULL;
    }

    return (pManagementInterfaceMessage);
}

void FrameworkTestability1ObjectManager::frameworkTestabilityMessage1RequestHandler (FrameworkTestabilityMessage1 *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityMessage1RequestHandler : Serving up a FrameworkTestabilityMessage1.");
    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

void FrameworkTestability1ObjectManager::frameworkTestabilityMessage2RequestHandler (FrameworkTestabilityMessage2 *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityMessage2RequestHandler : Serving up a FrameworkTestabilityMessage2.");

    if (0 == (getNumberOfProcessedMessage2s ()))
    {
        while (1)
        {
            if (true == (getCanProceedToProcessMessage2s ()))
            {
                break;
            }

            prismSleep (1);
        }
    }

    trace (TRACE_LEVEL_INFO, "FrameworkTestability1ObjectManager::frameworkTestabilityMessage2RequestHandler : Now starting to serve FrameworkTestabilityMessage2s.");

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    //incrementNumberOfProcessedMessage2s ();

    reply (pMessage);
}

// message to test sendToWaveCluster
void FrameworkTestability1ObjectManager::frameworkTestabilityMessage3RequestHandler (FrameworkTestabilityMessage3 *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityMessage3RequestHandler : Serving up a FrameworkTestabilityMessage3.");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability1ObjectManager::frameworkTestabilityMessage3ValidateInputBuffersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability1ObjectManager::frameworkTestabilityMessage3AttachOutputBuffersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability1ObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability1ObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void FrameworkTestability1ObjectManager::frameworkTestabilityMessage3ValidateInputBuffersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    FrameworkTestabilityMessage3 *pMessage = reinterpret_cast<FrameworkTestabilityMessage3 *> (pPrismLinearSequencerContext->getPPrismMessage ());
    ResourceId                    status;

    if (true == (pMessage->validateInputBuffer1 ()))
    {
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        status = WAVE_MESSAGE_ERROR;
    }

    if (true == (pMessage->validateInputLargeBuffer1 ()))
    {
        status = WAVE_MESSAGE_SUCCESS;

        status = pMessage->getRequestedCompletionStatus ();
    }
    else
    {
        status = WAVE_MESSAGE_ERROR;
    }

    trace (TRACE_LEVEL_DEBUG, string("FrameworkTestability1ObjectManager::frameworkTestabilityMessage3AttachOutputBuffersStep : isBeingSurrogated returned :") + isBeingSurrogated ()); 
    pPrismLinearSequencerContext->executeNextStep (status);
}

void FrameworkTestability1ObjectManager::frameworkTestabilityMessage3AttachOutputBuffersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    FrameworkTestabilityMessage3 *pMessage = reinterpret_cast<FrameworkTestabilityMessage3 *> (pPrismLinearSequencerContext->getPPrismMessage ());

    pMessage->setupOutputBuffer1 ();

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void FrameworkTestability1ObjectManager::frameworkTestabilityMessage4RequestHandler (FrameworkTestabilityMessage4 *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityMessage4RequestHandler : Serving up a FrameworkTestabilityMessage4.");

    bool isInputIntact = pMessage->validateContent ();

    if (true == isInputIntact)
    {
        pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        pMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
    }

    reply (pMessage);
}

void FrameworkTestability1ObjectManager::setAreMessage6sReceived (const bool &areMessage6sReceived)
{
    s_areMessage6sReceivedMutex.lock ();
    s_areMessage6sReceived = areMessage6sReceived;
    s_areMessage6sReceivedMutex.unlock ();
}

bool FrameworkTestability1ObjectManager::getAndResetAreMessage6sReceived ()
{
    bool areMessage6sReceived = false;

    s_areMessage6sReceivedMutex.lock ();
    areMessage6sReceived = s_areMessage6sReceived;
    s_areMessage6sReceived = false;
    s_areMessage6sReceivedMutex.unlock ();

    return (areMessage6sReceived);
}

void FrameworkTestability1ObjectManager::frameworkTestabilityMessage5RequestHandler (FrameworkTestabilityMessage5 *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityMessage5RequestHandler : Serving up a FrameworkTestabilityMessage5.");

    UI32 numberOfPendingMessages = 0;

    while (0 == (numberOfPendingMessages = getNumberOfPendingNormalMessages ()))
    {
        prismSleep (1);
    }

    trace (TRACE_LEVEL_INFO, "FrameworkTestability1ObjectManager::frameworkTestabilityMessage5RequestHandler : Received more messages in the incoming queues.");

    setAreMessage6sReceived (true);

    while (0 != (numberOfPendingMessages = getNumberOfPendingNormalMessages ()))
    {
        prismSleep (1);
    }

    trace (TRACE_LEVEL_INFO, "FrameworkTestability1ObjectManager::frameworkTestabilityMessage5RequestHandler : All messages from the incoming queues are gone.");

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

void FrameworkTestability1ObjectManager::frameworkTestabilityMessage6RequestHandler (FrameworkTestabilityMessage6 *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityMessage6RequestHandler : Serving up a FrameworkTestabilityMessage6.");

    trace (TRACE_LEVEL_FATAL, "FrameworkTestability1ObjectManager::frameworkTestabilityMessage6RequestHandler : We must never reach this place.  FrameworkTestabilityMessage6 is used message cancellation test mechanisms.");
    prismAssert (false, __FILE__, __LINE__);

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

void FrameworkTestability1ObjectManager::frameworkTestabilityMessage7RequestHandler (FrameworkTestabilityMessage7 *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityMessage7RequestHandler : Serving up a FrameworkTestabilityMessage1.");
    prismSleep (1);
    incrementNumberOfProcessedMessage7s ();
    // Use cout for the following, not trace.
    cout << "Number of Processed Message7s : " << s_numberOfProcessedMessage7s << " \r";
    fflush (stdout);
    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

UI32 FrameworkTestability1ObjectManager::getNumberOfProcessedMessage2s ()
{
    UI32 count = 0;

    s_numberOfProcessedMessage2sMutex.lock ();
    count = s_numberOfProcessedMessage2s;
    s_numberOfProcessedMessage2sMutex.unlock ();

    return (count);
}

void FrameworkTestability1ObjectManager::setNumberOfProcessedMessage2s (const UI32 numberOfProcessedMessage2s)
{
    s_numberOfProcessedMessage2sMutex.lock ();
    s_numberOfProcessedMessage2s = numberOfProcessedMessage2s;
    s_numberOfProcessedMessage2sMutex.unlock ();
}

void FrameworkTestability1ObjectManager::incrementNumberOfProcessedMessage2s ()
{
    s_numberOfProcessedMessage2sMutex.lock ();
    s_numberOfProcessedMessage2s++;
    s_numberOfProcessedMessage2sMutex.unlock ();
}

bool FrameworkTestability1ObjectManager::getCanProceedToProcessMessage2s ()
{
    bool canProceedToProcessMessage2s = false;

    s_canProceedToProcessMessage2sMutex.lock ();
    canProceedToProcessMessage2s = s_canProceedToProcessMessage2s;
    s_canProceedToProcessMessage2sMutex.unlock ();

    return (canProceedToProcessMessage2s);
}

void FrameworkTestability1ObjectManager::setCanProceedToProcessMessage2s (const bool &canProceedToProcessMessage2s)
{
    s_canProceedToProcessMessage2sMutex.lock ();
    s_canProceedToProcessMessage2s = canProceedToProcessMessage2s;
    s_canProceedToProcessMessage2sMutex.unlock ();
}

UI32 FrameworkTestability1ObjectManager::getNumberOfProcessedMessage7s ()
{
    UI32 count = 0;

    s_numberOfProcessedMessage7sMutex.lock ();
    count = s_numberOfProcessedMessage7s;
    s_numberOfProcessedMessage7sMutex.unlock ();

    return (count);
}

void FrameworkTestability1ObjectManager::setNumberOfProcessedMessage7s (const UI32 numberOfProcessedMessage7s)
{
    s_numberOfProcessedMessage7sMutex.lock ();
    s_numberOfProcessedMessage7s = numberOfProcessedMessage7s;
    s_numberOfProcessedMessage7sMutex.unlock ();
}

void FrameworkTestability1ObjectManager::incrementNumberOfProcessedMessage7s ()
{
    s_numberOfProcessedMessage7sMutex.lock ();
    s_numberOfProcessedMessage7s++;
    s_numberOfProcessedMessage7sMutex.unlock ();
}

void FrameworkTestability1ObjectManager::frameworkTestabilityEvent1EventHandler (const FrameworkTestabilityEvent1 *&pEvent)
{
    static UI32 numberOfEventsRceivedSoFar = 0;

    numberOfEventsRceivedSoFar++;

    if (0 == (numberOfEventsRceivedSoFar % 10000))
    {
        trace (TRACE_LEVEL_INFO, string ("FrameworkTestability1ObjectManager::frameworkTestabilityEvent1EventHandler : Received Events ... ") + numberOfEventsRceivedSoFar);
        trace (TRACE_LEVEL_DEBUG, string ("m_eventUI32   = ") + pEvent->getEventUI32 ());
        trace (TRACE_LEVEL_DEBUG, string ("m_eventString = ") + pEvent->getEventString ());
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage1RequestHandler (FrameworkTestabilityManagementInterfaceMessage1 *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage1RequestHandler : ");

    const UI32 numberOfMessagesToBeSentToClient = 100000;
          UI32 i                                = 0;

    for (i = 0; i < numberOfMessagesToBeSentToClient; i++)
    {
        ResourceId status;

        ManagementInterfaceClientTestMessage1 message;

        trace (TRACE_LEVEL_INFO, string ("FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage1RequestHandler : Sending Message No : ") + i);

        status = sendSynchronouslyToWaveClient (pMessage->getClientName (), &message);

        trace (TRACE_LEVEL_INFO, string ("FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage1RequestHandler : Sent    Message No : ") + i);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            status = message.getCompletionStatus ();
        }

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, string ("FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage1RequestHandler : Total Number of messages to Client attempted : ") + i);

            pMessage->setCompletionStatus (status);
            reply (pMessage);
            return;
        }
    }

    trace (TRACE_LEVEL_INFO, string ("FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage1RequestHandler : Total Number of messages to Client attempted : ") + i);

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

void FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage2RequestHandler (FrameworkTestabilityManagementInterfaceMessage2 *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage2RequestHandler : ");

    ResourceId status;
    const UI32 numberOfMessagesToBeSentToClient = 100000;
          UI32 i                                = 0;

    m_messageSent     = numberOfMessagesToBeSentToClient;
    m_isStatusFailure = false;

    for (i = 0; i < numberOfMessagesToBeSentToClient; i++)
    {
        ManagementInterfaceClientTestMessage2 *message = new ManagementInterfaceClientTestMessage2;

        status = sendToWaveClient (pMessage->getClientName (), message, reinterpret_cast<PrismMessageResponseHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage2RequestCallBack), pMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, string ("FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage2RequestHandler: Failed to send message number :") + i);
            delete message;
            m_messageSent--;
        }
    }

    if (0 == m_messageSent)
    {
        pMessage->setCompletionStatus (status);
        reply (pMessage);
    }
}

void FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage2RequestCallBack (FrameworkStatus frameworkStatus, ManagementInterfaceClientTestMessage2 *message, void *pContext)
{

    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage2RequestCallBack : ");

    FrameworkTestabilityManagementInterfaceMessage2 *pMessage = reinterpret_cast<FrameworkTestabilityManagementInterfaceMessage2 *> (pContext);

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage2RequestCallBack " + frameworkStatus);
        if (false == m_isStatusFailure)
        {
            pMessage->setCompletionStatus(frameworkStatus);
            m_isStatusFailure = true;
        }
    }

    m_messageSent--;

    if (0 == m_messageSent)
    {
        if (false == m_isStatusFailure)
        {
            pMessage->setCompletionStatus (message->getCompletionStatus ());
        }

        reply (pMessage);
    }

    delete message;
}

void FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage3RequestHandler (FrameworkTestabilityManagementInterfaceMessage3 *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage3RequestHandler : ");

    ResourceId status;

    ManagementInterfaceClientTestMessage3 *message = new ManagementInterfaceClientTestMessage3;

    status = sendToWaveClient (pMessage->getClientName (), message, reinterpret_cast<PrismMessageResponseHandler> (&FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage3RequestCallBack), pMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, string ("FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage3RequestHandler: Failed to send message number :"));
        pMessage->setCompletionStatus (status);
        reply (pMessage);
        delete message;
    }

}

void FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage3RequestCallBack (FrameworkStatus frameworkStatus, ManagementInterfaceClientTestMessage3 *message, void *pContext)
{

    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage3RequestCallBack : ");

    FrameworkTestabilityManagementInterfaceMessage3 *pMessage    = reinterpret_cast<FrameworkTestabilityManagementInterfaceMessage3 *> (pContext);
    bool                                             isLastReply = false;

    isLastReply = message->getIsLastReply ();

    if (true == isLastReply)
    {
        if (FRAMEWORK_SUCCESS != frameworkStatus)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage3RequestCallBack " + frameworkStatus);
            pMessage->setCompletionStatus(frameworkStatus);
        }

        pMessage->setCompletionStatus (message->getCompletionStatus ());

        reply (pMessage);
   }

   delete message;
}

void FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage4RequestHandler (FrameworkTestabilityManagementInterfaceMessage4 *pMessage)
{

 trace (TRACE_LEVEL_DEVEL, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage4RequestHandler : Enter");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllSuccessTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllSucesssButPhase2MessageSetTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllFailureTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllFailureButPhase2MessageSetTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1SomeSuccessTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1SomeSucesssButPhase2MessageSetTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability1ObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability1ObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}


void FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllSuccessTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Send To Client with Buffers Async All Success Test.");
    FrameworkTestabilityManagementInterfaceMessage4 *pMessage = reinterpret_cast<FrameworkTestabilityManagementInterfaceMessage4 *> (pPrismLinearSequencerContext->getPPrismMessage ());

    ManagementInterfaceClientTestMessage4 *pManagementInterfaceClientTestMessage4 = new ManagementInterfaceClientTestMessage4 ();

    WaveSendToClientsContext *pWaveSendToClientsContext = new WaveSendToClientsContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllSuccessTestCallback), pPrismLinearSequencerContext, pMessage->getClientName ());

    pManagementInterfaceClientTestMessage4->setupInputBuffer1 ();
    pManagementInterfaceClientTestMessage4->setCompletionStatusType (0);

    pWaveSendToClientsContext->setPManagementInterfaceMessageForPhase1 (pManagementInterfaceClientTestMessage4);

    sendToWaveClients (pWaveSendToClientsContext);
}

void FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllSuccessTestCallback (WaveSendToClientsContext *pWaveSendToClientsContext)
{
    ResourceId                          sendToClientsCompletionStatus       = pWaveSendToClientsContext->getCompletionStatus ();
    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClientsContext->getPCallerContext ());

    prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);

    delete (pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase1 ());
    delete pWaveSendToClientsContext;

    pPrismLinearSequencerContext->executeNextStep (sendToClientsCompletionStatus);
}

void FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllSucesssButPhase2MessageSetTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Send To Clients with Buffers Async All Success But Phase 2 Set Test.");
    FrameworkTestabilityManagementInterfaceMessage4 *pMessage = reinterpret_cast<FrameworkTestabilityManagementInterfaceMessage4 *> (pPrismLinearSequencerContext->getPPrismMessage ());

    ManagementInterfaceClientTestMessage4 *pManagementInterfaceClientTestMessage4  = new ManagementInterfaceClientTestMessage4 ();
    ManagementInterfaceClientTestMessage4 *pManagementInterfaceClientTestMessage42 = new ManagementInterfaceClientTestMessage4 ();
    WaveSendToClientsContext     *pWaveSendToClientsContext      = new WaveSendToClientsContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllSucesssButPhase2MessageSetTestCallback), pPrismLinearSequencerContext, pMessage->getClientName ());

    pManagementInterfaceClientTestMessage4->setupInputBuffer1 ();
    pManagementInterfaceClientTestMessage4->setCompletionStatusType (0);

    pWaveSendToClientsContext->setPManagementInterfaceMessageForPhase1 (pManagementInterfaceClientTestMessage4);

    pManagementInterfaceClientTestMessage42->setupInputBuffer1 ();
    pManagementInterfaceClientTestMessage42->setCompletionStatusType (0);

    pWaveSendToClientsContext->setPManagementInterfaceMessageForPhase2 (pManagementInterfaceClientTestMessage42);

    sendToWaveClients (pWaveSendToClientsContext);
}

void FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllSucesssButPhase2MessageSetTestCallback (WaveSendToClientsContext *pWaveSendToClientsContext)
{
    ResourceId                          sendToClientsCompletionStatus       = pWaveSendToClientsContext->getCompletionStatus ();
    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClientsContext->getPCallerContext ());

    prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);

    delete (pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase1 ());
    delete (pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase2 ());
    delete pWaveSendToClientsContext;

    pPrismLinearSequencerContext->executeNextStep (sendToClientsCompletionStatus);
}

void FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllFailureTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Send To Clients with Buffers Async All Failure Test.");

    FrameworkTestabilityManagementInterfaceMessage4 *pMessage = reinterpret_cast<FrameworkTestabilityManagementInterfaceMessage4 *> (pPrismLinearSequencerContext->getPPrismMessage ());

    ManagementInterfaceClientTestMessage4 *pManagementInterfaceClientTestMessage4  = new ManagementInterfaceClientTestMessage4 ();
    WaveSendToClientsContext     *pWaveSendToClientsContext      = new WaveSendToClientsContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllFailureTestCallback), pPrismLinearSequencerContext, pMessage->getClientName ());

    pManagementInterfaceClientTestMessage4->setupInputBuffer1 ();
    pManagementInterfaceClientTestMessage4->setCompletionStatusType (1);

    pWaveSendToClientsContext->setPManagementInterfaceMessageForPhase1 (pManagementInterfaceClientTestMessage4);

    sendToWaveClients (pWaveSendToClientsContext);
}

void FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllFailureTestCallback (WaveSendToClientsContext *pWaveSendToClientsContext)
{
    ResourceId                          sendToClientsCompletionStatus       = pWaveSendToClientsContext->getCompletionStatus ();
    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClientsContext->getPCallerContext ());

    prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);

    delete (pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase1 ());
    delete pWaveSendToClientsContext;

    if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == sendToClientsCompletionStatus)
    {
        sendToClientsCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }

    pPrismLinearSequencerContext->executeNextStep (sendToClientsCompletionStatus);
}

void FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllFailureButPhase2MessageSetTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Send To Clients with Buffers Async All Failure But Phase 2 Set Test.");

    FrameworkTestabilityManagementInterfaceMessage4 *pMessage = reinterpret_cast<FrameworkTestabilityManagementInterfaceMessage4 *> (pPrismLinearSequencerContext->getPPrismMessage ());

    ManagementInterfaceClientTestMessage4 *pManagementInterfaceClientTestMessage4  = new ManagementInterfaceClientTestMessage4 ();
    ManagementInterfaceClientTestMessage4 *pManagementInterfaceClientTestMessage42 = new ManagementInterfaceClientTestMessage4 ();
    WaveSendToClientsContext     *pWaveSendToClientsContext      = new WaveSendToClientsContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllFailureButPhase2MessageSetTestCallback), pPrismLinearSequencerContext, pMessage->getClientName ());

    pManagementInterfaceClientTestMessage4->setupInputBuffer1 ();
    pManagementInterfaceClientTestMessage4->setCompletionStatusType (1);

    pWaveSendToClientsContext->setPManagementInterfaceMessageForPhase1 (pManagementInterfaceClientTestMessage4);

    pManagementInterfaceClientTestMessage42->setupInputBuffer1 ();
    pManagementInterfaceClientTestMessage42->setCompletionStatusType (0);

    pWaveSendToClientsContext->setPManagementInterfaceMessageForPhase2 (pManagementInterfaceClientTestMessage42);

    sendToWaveClients (pWaveSendToClientsContext);
}

void FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1AllFailureButPhase2MessageSetTestCallback (WaveSendToClientsContext *pWaveSendToClientsContext)
{
    ResourceId                          sendToClientsCompletionStatus       = pWaveSendToClientsContext->getCompletionStatus ();
    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClientsContext->getPCallerContext ());

    prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);

    delete (pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase1 ());
    delete (pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase2 ());
    delete pWaveSendToClientsContext;

    if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == sendToClientsCompletionStatus)
    {
        sendToClientsCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }

    pPrismLinearSequencerContext->executeNextStep (sendToClientsCompletionStatus);
}

void FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1SomeSuccessTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Send To Clients with Buffers Async Some Success Test.");

    FrameworkTestabilityManagementInterfaceMessage4 *pMessage = reinterpret_cast<FrameworkTestabilityManagementInterfaceMessage4 *> (pPrismLinearSequencerContext->getPPrismMessage ());

    ManagementInterfaceClientTestMessage4 *pManagementInterfaceClientTestMessage4  = new ManagementInterfaceClientTestMessage4 ();
    WaveSendToClientsContext     *pWaveSendToClientsContext      = new WaveSendToClientsContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1SomeSuccessTestCallback), pPrismLinearSequencerContext, pMessage->getClientName ());

    pManagementInterfaceClientTestMessage4->setupInputBuffer1 ();
    pManagementInterfaceClientTestMessage4->setCompletionStatusType (2);

    pWaveSendToClientsContext->setPManagementInterfaceMessageForPhase1 (pManagementInterfaceClientTestMessage4);

    sendToWaveClients (pWaveSendToClientsContext);
}

void FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1SomeSuccessTestCallback (WaveSendToClientsContext *pWaveSendToClientsContext)
{
    ResourceId                          sendToClientsCompletionStatus       = pWaveSendToClientsContext->getCompletionStatus ();
    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClientsContext->getPCallerContext ());

    prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);

    delete (pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase1 ());
    delete pWaveSendToClientsContext;

    if (WAVE_MESSAGE_ERROR_FAILED_ON_SOME_NODES == sendToClientsCompletionStatus)
    {
        sendToClientsCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == sendToClientsCompletionStatus)
    {
        trace (TRACE_LEVEL_WARN, "FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1SomeSuccessTestCallback : BYPASSING test... Instances all returned error status");       
        sendToClientsCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1SomeSuccessTestCallback : Status " + FrameworkToolKit::localize (sendToClientsCompletionStatus));
    }

    pPrismLinearSequencerContext->executeNextStep (sendToClientsCompletionStatus);
}

void FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1SomeSucesssButPhase2MessageSetTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Send To Clients with Buffers Async Some Success But Phase 2 Set Test.");

    FrameworkTestabilityManagementInterfaceMessage4 *pMessage = reinterpret_cast<FrameworkTestabilityManagementInterfaceMessage4 *> (pPrismLinearSequencerContext->getPPrismMessage ());

    ManagementInterfaceClientTestMessage4 *pManagementInterfaceClientTestMessage4  = new ManagementInterfaceClientTestMessage4 ();
    ManagementInterfaceClientTestMessage4 *pManagementInterfaceClientTestMessage42 = new ManagementInterfaceClientTestMessage4 ();
    WaveSendToClientsContext     *pWaveSendToClientsContext      = new WaveSendToClientsContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1SomeSucesssButPhase2MessageSetTestCallback), pPrismLinearSequencerContext, pMessage->getClientName ());

    pManagementInterfaceClientTestMessage4->setupInputBuffer1 ();
    pManagementInterfaceClientTestMessage4->setCompletionStatusType (2);

    pWaveSendToClientsContext->setPManagementInterfaceMessageForPhase1 (pManagementInterfaceClientTestMessage4);


    pManagementInterfaceClientTestMessage42->setupInputBuffer1 ();
    pManagementInterfaceClientTestMessage42->setCompletionStatusType (0);

    pWaveSendToClientsContext->setPManagementInterfaceMessageForPhase2 (pManagementInterfaceClientTestMessage42);

    sendToWaveClients (pWaveSendToClientsContext);
}

void FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1SomeSucesssButPhase2MessageSetTestCallback (WaveSendToClientsContext *pWaveSendToClientsContext)
{
    ResourceId                          sendToClientsCompletionStatus       = pWaveSendToClientsContext->getCompletionStatus ();
    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClientsContext->getPCallerContext ());

    prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);

    delete (pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase1 ());
    delete (pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase2 ());
    delete pWaveSendToClientsContext;

    if (WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED == sendToClientsCompletionStatus)
    {
        sendToClientsCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == sendToClientsCompletionStatus)
    {
        trace (TRACE_LEVEL_WARN, "FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1SomeSuccessButPhase2MessageSetTestCallback : BYPASSING test... Instnaces all returned error status");       
        sendToClientsCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability1ObjectManager::sendToClientsWithBuffersPhase1SomeSuccessButPhase2MessageSetTestCallback : Status " + FrameworkToolKit::localize (sendToClientsCompletionStatus));
    }

    pPrismLinearSequencerContext->executeNextStep (sendToClientsCompletionStatus);
}

void FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessageLeakMessageRequestHandler (FrameworkTestabilityManagementInterfaceMessageLeakMessage *pMessage)
{
    trace (TRACE_LEVEL_DEBUG, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessage1RequestHandler : ");

    PrismServiceId prismServiceId = pMessage->getPrismServiceId ();
    vector<PrismServiceId>  messageServiceIds;
    vector<UI32>            messageOperationCodes;
    vector<WaveMessageType> messageTypes;
    vector<string>          btStrings;

    tracePrintf (TRACE_LEVEL_INFO, true, false, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessageLeakMessageRequestHandler : Getting Message Leaks For Service : %u, \"%s\"", prismServiceId, (FrameworkToolKit::getServiceNameById (prismServiceId)).c_str ());

    MessageTracker::getMessages (prismServiceId, messageServiceIds, messageOperationCodes, messageTypes, btStrings);

    tracePrintf (TRACE_LEVEL_INFO, true, false, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceMessageLeakMessageRequestHandler : Got %u Message Leaks For Service : %u, \"%s\"", messageServiceIds.size (), prismServiceId, (FrameworkToolKit::getServiceNameById (prismServiceId)).c_str ());

    pMessage->setMessageArguments (messageServiceIds, messageOperationCodes, messageTypes, btStrings);

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pMessage);
}

void FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceObjectLeakMessageRequestHandler (FrameworkTestabilityManagementInterfaceObjectLeakMessage *pMessage)
{
    trace (TRACE_LEVEL_DEBUG, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceObjectLeakMessageRequestHandler ");

    PrismServiceId prismServiceId = pMessage->getPrismServiceId ();

    vector<string>  managedObjectClassNames;
    vector<string>  managedObjectNames;
    vector<bool>    queryResults;
    vector<string>  btStrings;

    ObjectTracker::getObjects (prismServiceId, managedObjectClassNames, managedObjectNames, queryResults, btStrings);

    pMessage->setMessageArguments (managedObjectClassNames, managedObjectNames, queryResults, btStrings);

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pMessage);
}

void FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfacFileServiceUsageRequestHandler(FrameworkTestabilityManagementInterfaceFileServiceUsageMessage  *pMessage)
{
    trace (TRACE_LEVEL_DEBUG, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceObjectLeakMessageRequestHandler ");

    // PrismServiceId prismServiceId = pMessage->getPrismServiceId ();
    bool bFileServiceUsageFlag = pMessage->getFileServiceUseforFileTransferFlag();

    if (true == bFileServiceUsageFlag)
    {
         trace(TRACE_LEVEL_DEBUG, string(" Enabling Usage of File Service call being made from  FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfacFileServiceUsageRequestHandler.\n"));
         FrameworkToolKit::enableFileServiceUse();
    }
    else {
         trace(TRACE_LEVEL_DEBUG, string(" Disabling Usage of File Service call being made from  FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfacFileServiceUsageRequestHandler.\n"));    
         FrameworkToolKit::disableFileServiceUse();
    }

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply(pMessage);
}

void FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceTrackLeaksWithBtMessageRequestHandler (FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage *pMessage)
{
    trace (TRACE_LEVEL_DEBUG, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceTrackLeaksWithBtMessageRequestHandler");

    UI32 leakType       = pMessage->getLeakType ();
    bool btTrackerState = pMessage->getBtTrackerState ();

    if (OBJECT_LEAK == leakType)
    {
        ObjectTracker::changeBtTrackerState (btTrackerState);
    }
    else if (MESSAGE_LEAK == leakType)
    {
        MessageTracker::changeBtTrackerState (btTrackerState);
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "FrameworkTestability1ObjectManager::frameworkTestabilityManagementInterfaceTrackLeaksWithBtMessageRequestHandler : unknown leak type requested.");
    }
    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply(pMessage);
}


}
