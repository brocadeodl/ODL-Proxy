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

#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestObjectManager.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestContexts.h"
#include "Framework/Core/Test/FrameworkTestabilityMessages.h"
#include "Framework/Messaging/Local/Test/FrameworkTestabilityEvents.h"
#include "Framework/Core/Test/FrameworkTestability1ObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/ServiceInterface/Test/PrismNativeTest1ServiceMessages.h"

namespace WaveNs
{

FrameworkLocalMessagingTestObjectManager::FrameworkLocalMessagingTestObjectManager ()
    : PrismTestObjectManager ("Framework Local Messaging Test")
{
    addEventType (FRAMEWORK_MESSAGING_LOCAL_TEST_EVENT1);
}

FrameworkLocalMessagingTestObjectManager *FrameworkLocalMessagingTestObjectManager::getInstance ()
{
    static FrameworkLocalMessagingTestObjectManager *pFrameworkLocalMessagingTestObjectManager = NULL;

    if (NULL == pFrameworkLocalMessagingTestObjectManager)
    {
        pFrameworkLocalMessagingTestObjectManager = new FrameworkLocalMessagingTestObjectManager ();
        WaveNs::prismAssert (NULL != pFrameworkLocalMessagingTestObjectManager, __FILE__, __LINE__);
    }

    return (pFrameworkLocalMessagingTestObjectManager);
}

FrameworkLocalMessagingTestObjectManager::~FrameworkLocalMessagingTestObjectManager ()
{
}

PrismServiceId FrameworkLocalMessagingTestObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void FrameworkLocalMessagingTestObjectManager::testRequestHandler (RegressionTestMessage *pMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager::simpleAsynchronousMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager::simpleOneWayMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager::simpleSynchronousMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager::simpleRecallMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager::simpleRecallFromFrontMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager::simpleRecallFromBackMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager::simpleEventTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager::prismLinearSequencerFailedStep),
    };

    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = new FrameworkLocalMessagingTestContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pFrameworkLocalMessagingTestContext->start ();
}

void FrameworkLocalMessagingTestObjectManager::simpleAsynchronousMessageTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Simple Asynchronous Messaging Test.");

    UI32               numberOfMessagesToSend = 100000;
    UI32               i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkLocalMessagingTestObjectManager::frameworkTestabilityMessage1Callback),
                       pFrameworkLocalMessagingTestContext);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_DEBUG, string ("FrameworkLocalMessagingTestObjectManager::simpleAsynchronousMessageTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
            delete pMessage;
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager::frameworkTestabilityMessage1Callback (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage1 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    --(*pFrameworkLocalMessagingTestContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pMessage, __FILE__, __LINE__);

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ()))
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
        }

        delete pMessage;
    }
    else
    {
        pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();

        if (NULL != pMessage)
        {
            delete pMessage;
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager::simpleOneWayMessageTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting One Way      Messaging              Test.");

    UI32               numberOfMessagesToSend = 100000;
    UI32               i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();

        pMessage->setMessage ("This is a test message");

        status = sendOneWay (pMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkLocalMessagingTestObjectManager::simpleAsynchronousMessageTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
            delete pMessage;
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager::simpleSynchronousMessageTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Simple Synchronous Messaging Test.");

    UI32             numberOfMessagesToSend = 100000;
    UI32             i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage1 message;
        //PrismNativeTest1ServiceMessage1 message;

        status = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, string ("FrameworkLocalMessagingTestObjectManager::simpleSynchronousMessageTestStep : Failed sending a message synchronously. status = ") + status + ".");
            pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        }
        else
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_DEBUG, string ("FrameworkLocalMessagingTestObjectManager::simpleSynchronousMessageTestStep : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                return;
            }
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager::simpleRecallMessageTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Simple Recall       Messaging Test.");

    UI32                   numberOfMessagesToSend   = 100000;
    UI32                   i                        = 0;
    WaveMessageStatus     status                   = WAVE_MESSAGE_ERROR;
    vector<PrismMessage *> messagesToCancel;
    UI32                   numberOfMessagesToCancel = 0;
    UI32                   numberOfFailuresToRecall = 0;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    FrameworkTestabilityMessage5 *pFrameworkTestabilityMessage5 = new FrameworkTestabilityMessage5 ();

    status = send (pFrameworkTestabilityMessage5,
                   reinterpret_cast<PrismMessageResponseHandler> (&FrameworkLocalMessagingTestObjectManager::frameworkTestabilityMessage5Callback),
                   pFrameworkLocalMessagingTestContext);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        delete pFrameworkTestabilityMessage5;

        pFrameworkLocalMessagingTestContext->executeNextStep (status);
        return;
    }

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage6 *pMessage = new FrameworkTestabilityMessage6 ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkLocalMessagingTestObjectManager::frameworkTestabilityMessage6Callback),
                       pFrameworkLocalMessagingTestContext);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_DEBUG, string ("FrameworkLocalMessagingTestObjectManager::simpleRecallMessageTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
            delete pMessage;
        }
        else
        {
            messagesToCancel.push_back (pMessage);
        }
    }

    numberOfMessagesToCancel = messagesToCancel.size ();

    if (0 == numberOfMessagesToCancel)
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkLocalMessagingTestObjectManager::simpleRecallMessageTestStep : Cannot continue the test.  We could not send any messages.  So we cannot cancel any.");
        prismAssert (false, __FILE__, __LINE__);
    }

    numberOfFailuresToRecall = 0;

    // Now before proceeding to cancellation, make sure that the receiver at least got a chance to see
    // if there were  any messages in its incoming queues.  Waiting here is important.  Otherwise we may
    // cancel all the messages even before the reciever sees them in its incoming queues, causing the
    // receiver to wait forever expecting to see some number of messages (> 0) in its queues.

    // We read the value and reset it so that the setup will function properly for the next test iteration.

    while (false == (FrameworkTestability1ObjectManager::getAndResetAreMessage6sReceived ()))
    {
        prismSleep (1);
    }

    trace (TRACE_LEVEL_INFO, "FrameworkLocalMessagingTestObjectManager::simpleRecallFromFrontMessageTestStep : The receiver indicated that it saw messages in its incoming queues.  We can proceed to cancel them.");

    for (i = 0; i < numberOfMessagesToCancel; i++)
    {
        status = recall (messagesToCancel[i]);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            numberOfFailuresToRecall++;
        }
        else
        {
            delete messagesToCancel[i];
        }
    }

    messagesToCancel.clear ();

    if (0 != numberOfFailuresToRecall)
    {
        trace (TRACE_LEVEL_ERROR, string ("FrameworkLocalMessagingTestObjectManager::simpleRecallMessageTestStep : Failed to recall ") + numberOfFailuresToRecall + " out of " + numberOfMessagesToCancel + " .");
    }
}

void FrameworkLocalMessagingTestObjectManager::frameworkTestabilityMessage5Callback (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage5 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pMessage, __FILE__, __LINE__);

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ()))
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
        }

        delete pMessage;
    }
    else
    {
        pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();

        if (NULL != pMessage)
        {
            delete pMessage;
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager::frameworkTestabilityMessage6Callback (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage6 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{

    trace (TRACE_LEVEL_FATAL, "FrameworkLocalMessagingTestObjectManager::frameworkTestabilityMessage6Callback : We should never reach this place.");
    prismAssert (false, __FILE__, __LINE__);
}

void FrameworkLocalMessagingTestObjectManager::simpleRecallFromFrontMessageTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Simple Recall Front Messaging Test.");

    UI32                   numberOfMessagesToSend   = 100000;
    UI32                   i                        = 0;
    WaveMessageStatus     status                   = WAVE_MESSAGE_ERROR;
    vector<PrismMessage *> messagesToCancel;
    UI32                   numberOfMessagesToCancel = 0;
    UI32                   numberOfFailuresToRecall = 0;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkLocalMessagingTestObjectManager::frameworkTestabilityMessage1Callback),
                       pFrameworkLocalMessagingTestContext);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_DEBUG, string ("FrameworkLocalMessagingTestObjectManager::simpleRecallFromFrontMessageTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
            delete pMessage;
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
            messagesToCancel.push_back (pMessage);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);

    numberOfMessagesToCancel = messagesToCancel.size ();
    numberOfFailuresToRecall = 0;

    for (i = 0; i < numberOfMessagesToCancel; i++)
    {
        status = recall (messagesToCancel[i]);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            // In this test we do not count this as an actual failure.  We expect the actual message callback to arrive for this message.  That si why we could not recall the message.
            // Most like it would been already processed by the rcipient service.

            numberOfFailuresToRecall++;
        }
        else
        {
            --(*pFrameworkLocalMessagingTestContext);
            delete messagesToCancel[i];
        }
    }

    messagesToCancel.clear ();

    trace (TRACE_LEVEL_INFO, string ("    Failed to recall ") + numberOfFailuresToRecall + " messages out of " + numberOfMessagesToCancel);
    trace (TRACE_LEVEL_INFO, string ("    For all the failed messages we must receive callbacks to proceed further."));

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager::simpleRecallFromBackMessageTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Simple Recall Back  Messaging Test.");

    UI32                   numberOfMessagesToSend   = 100000;
    UI32                   i                        = 0;
    WaveMessageStatus     status                   = WAVE_MESSAGE_ERROR;
    vector<PrismMessage *> messagesToCancel;
    UI32                   numberOfMessagesToCancel = 0;
    UI32                   numberOfFailuresToRecall = 0;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkLocalMessagingTestObjectManager::frameworkTestabilityMessage1Callback),
                       pFrameworkLocalMessagingTestContext);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_DEBUG, string ("FrameworkLocalMessagingTestObjectManager::simpleRecallFromBackMessageTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
            delete pMessage;
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
            messagesToCancel.push_back (pMessage);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);

    numberOfMessagesToCancel = messagesToCancel.size ();
    numberOfFailuresToRecall = 0;

    for (i = 0; i < numberOfMessagesToCancel; i++)
    {
        status = recall (messagesToCancel[numberOfMessagesToCancel - 1 - i]);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            // In this test we do not count this as an actual failure.  We expect the actual message callback to arrive for this message.  That si why we could not recall the message.
            // Most like it would been already processed by the rcipient service.

            numberOfFailuresToRecall++;
        }
        else
        {
            --(*pFrameworkLocalMessagingTestContext);
            delete messagesToCancel[numberOfMessagesToCancel - 1 - i];
        }
    }

    messagesToCancel.clear ();

    trace (TRACE_LEVEL_INFO, string ("    Failed to recall ") + numberOfFailuresToRecall + " messages out of " + numberOfMessagesToCancel);
    trace (TRACE_LEVEL_INFO, string ("    For all the failed messages we must receive callbacks to proceed further."));

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager::simpleEventTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Event broadcasting Test.");

    UI32                   numberOfEventsToSend   = 100000;
    UI32                   i                        = 0;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfEventsToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfEventsToSend + " Events.");

    for (i = 0; i < numberOfEventsToSend; i++)
    {
        FrameworkTestabilityEvent1 *pTestEvent = new FrameworkTestabilityEvent1;

        pTestEvent->setEventUI32 (i + 29);
        pTestEvent->setEventString (string ("PJreasiusse Giosd AGlomdighty.") + (i + 29));

        broadcast (pTestEvent);
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

PrismEvent *FrameworkLocalMessagingTestObjectManager::createEventInstance (const UI32 &eventOperationCode)
{
    PrismEvent *pPrismEvent = NULL;

    switch (eventOperationCode)
    {
        case FRAMEWORK_MESSAGING_LOCAL_TEST_EVENT1:
            pPrismEvent = new FrameworkTestabilityEvent1;
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("FrameworkLocalMessagingTestObjectManager::createEventInstance : Unkown Event Operation Code : ") + eventOperationCode);
            prismAssert (false, __FILE__, __LINE__);
    }

    return (pPrismEvent);
}

}
