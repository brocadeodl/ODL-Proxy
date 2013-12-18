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

#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestObjectManager2.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestContexts.h"
#include "Framework/Core/Test/FrameworkTestabilityMessages.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/Test/FrameworkTestability1ObjectManager.h"

namespace WaveNs
{

FrameworkLocalMessagingTestObjectManager2::FrameworkLocalMessagingTestObjectManager2 ()
    : PrismTestObjectManager ("Framework Local Messaging Test 2")
{
}

FrameworkLocalMessagingTestObjectManager2::~FrameworkLocalMessagingTestObjectManager2 ()
{
}

PrismServiceId FrameworkLocalMessagingTestObjectManager2::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

FrameworkLocalMessagingTestObjectManager2 *FrameworkLocalMessagingTestObjectManager2::getInstance ()
{
    static FrameworkLocalMessagingTestObjectManager2 *pFrameworkLocalMessagingTestObjectManager2 = NULL;

    if (NULL == pFrameworkLocalMessagingTestObjectManager2)
    {
        pFrameworkLocalMessagingTestObjectManager2 = new FrameworkLocalMessagingTestObjectManager2 ();
        WaveNs::prismAssert (NULL != pFrameworkLocalMessagingTestObjectManager2, __FILE__, __LINE__);
    }

    return (pFrameworkLocalMessagingTestObjectManager2);
}

void FrameworkLocalMessagingTestObjectManager2::testRequestHandler (RegressionTestMessage *pRegressionTestMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageWithTimeoutTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageWithBuffersWithTimeoutTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageResponseDeliveryAfterTimeoutTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageWithTimeout2TestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageWithBuffersWithTimeout2TestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager2::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkLocalMessagingTestObjectManager2::prismLinearSequencerFailedStep),
    };

    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = new FrameworkLocalMessagingTestContext (pRegressionTestMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pFrameworkLocalMessagingTestContext->start ();
}

void FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageWithTimeoutTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Simple Asynchronous Messaging with TimeOut Test.");

    UI32               numberOfMessagesToSend = 100000;
    UI32               i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    LocationId         thisLocationId         = FrameworkToolKit::getThisLocationId ();

    if ("" != getTestParameterValue ("nummsg"))
    {
        numberOfMessagesToSend = atoi (getTestParameterValue ("nummsg").c_str ());
    }

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkLocalMessagingTestObjectManager2::frameworkTestabilityMessage1Callback),
                       pFrameworkLocalMessagingTestContext,
                       60000,
                       thisLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageWithTimeoutTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager2::frameworkTestabilityMessage1Callback (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage1 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
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

void FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageWithBuffersWithTimeoutTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Simple Asynchronous Messaging with Buffers with TimeOut Test.");

    UI32               numberOfMessagesToSend = 100000;
    UI32               i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    LocationId         thisLocationId         = FrameworkToolKit::getThisLocationId ();
    
    if ("" != getTestParameterValue ("nummsg"))
    {
        numberOfMessagesToSend = atoi (getTestParameterValue ("nummsg").c_str ());
    }

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();
        char                         *pTemp    = new char[256];

        pMessage->addBuffer (0, 256, pTemp, true);

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkLocalMessagingTestObjectManager2::frameworkTestabilityMessage1Callback),
                       pFrameworkLocalMessagingTestContext,
                       60000,
                       thisLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageWithTimeoutTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageResponseDeliveryAfterTimeoutTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Simple Asynchronous Messaging with Timeout Test - Response Delivery after timeout");

    UI32               numberOfMessagesToSend = 100000;
    UI32               i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    LocationId         thisLocationId         = FrameworkToolKit::getThisLocationId ();

    if ("" != getTestParameterValue ("nummsg"))
    {
        numberOfMessagesToSend = atoi (getTestParameterValue ("nummsg").c_str ());
    }

    FrameworkTestability1ObjectManager::setCanProceedToProcessMessage2s (false);

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage2 *pMessage = new FrameworkTestabilityMessage2 ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkLocalMessagingTestObjectManager2::frameworkTestabilityMessage2Callback),
                       pFrameworkLocalMessagingTestContext,
                       10000,
                       thisLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageResponseDeliveryAfterTimeoutTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager2::frameworkTestabilityMessage2Callback (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage2 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    static UI32 numberOfTimeOuts             = 0;
    static UI32 numberOfRecallsDueToTimeOuts = 0;
    static UI32 numberOfSuccessfulReplies    = 0;

    --(*pFrameworkLocalMessagingTestContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pMessage, __FILE__, __LINE__);

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ()))
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
        }
        else
        {
            numberOfSuccessfulReplies++;
        }

        delete pMessage;
    }
    else
    {
        if (FRAMEWORK_TIME_OUT != frameworkStatus)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
        }
        else
        {
            numberOfTimeOuts++;

            if (NULL != pMessage)
            {
                numberOfRecallsDueToTimeOuts++;
            }
        }

        if (NULL != pMessage)
        {
            delete pMessage;
        }
    }

    if (0 == (pFrameworkLocalMessagingTestContext->getNumberOfCallbacksBeforeAdvancingToNextStep()))
    {
        trace (TRACE_LEVEL_INFO, string ("    Number of Timeouts received        : ") + numberOfTimeOuts);
        trace (TRACE_LEVEL_INFO, string ("    Number of Recalls due to Timeouts  : ") + numberOfRecallsDueToTimeOuts);
        trace (TRACE_LEVEL_INFO, string ("    Number of Successful Replies       : ") + numberOfSuccessfulReplies);

        numberOfTimeOuts             = 0;
        numberOfRecallsDueToTimeOuts = 0;
        numberOfSuccessfulReplies    = 0;

        FrameworkTestability1ObjectManager::setCanProceedToProcessMessage2s (true);
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageWithTimeout2TestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Simple Asynchronous Messaging with TimeOut Test - With some guaranteed TimeOuts.");

    UI32               numberOfMessagesToSend = 100000;
    UI32               i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    LocationId         thisLocationId         = FrameworkToolKit::getThisLocationId ();

    if ("" != getTestParameterValue ("nummsg"))
    {
        numberOfMessagesToSend = atoi (getTestParameterValue ("nummsg").c_str ());
    }

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);
    FrameworkTestability1ObjectManager::setNumberOfProcessedMessage7s (0);

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage7 *pMessage = new FrameworkTestabilityMessage7 ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkLocalMessagingTestObjectManager2::frameworkTestabilityMessage7Callback),
                       pFrameworkLocalMessagingTestContext,
                       10000,
                       thisLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageWithTimeoutTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager2::frameworkTestabilityMessage7Callback (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage7 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    static UI32 numberOfTimeOuts             = 0;
    static UI32 numberOfRecallsDueToTimeOuts = 0;
    static UI32 numberOfSuccessfulReplies    = 0;

    --(*pFrameworkLocalMessagingTestContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pMessage, __FILE__, __LINE__);

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ()))
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
        }
        else
        {
            numberOfSuccessfulReplies++;
        }

        delete pMessage;
    }
    else
    {
        if (FRAMEWORK_TIME_OUT != frameworkStatus)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
        }
        else
        {
            numberOfTimeOuts++;

            if (NULL != pMessage)
            {
                numberOfRecallsDueToTimeOuts++;
            }
        }

        if (NULL != pMessage)
        {
            delete pMessage;
        }
    }

    if (0 == (pFrameworkLocalMessagingTestContext->getNumberOfCallbacksBeforeAdvancingToNextStep()))
    {
        trace (TRACE_LEVEL_INFO, string ("    Number of Messages Processed       : ") + FrameworkTestability1ObjectManager::getNumberOfProcessedMessage7s ());
        trace (TRACE_LEVEL_INFO, string ("    Number of Timeouts received        : ") + numberOfTimeOuts);
        trace (TRACE_LEVEL_INFO, string ("    Number of Recalls due to Timeouts  : ") + numberOfRecallsDueToTimeOuts);
        trace (TRACE_LEVEL_INFO, string ("    Number of Successful Replies       : ") + numberOfSuccessfulReplies);

        numberOfTimeOuts             = 0;
        numberOfRecallsDueToTimeOuts = 0;
        numberOfSuccessfulReplies    = 0;

        // Wait for a second so that even if the recipient service picked up a message from processing and
        // then we got a timed out delivery, waiting for a second makes sure that the last picked up
        // message will be processed completely before we start the next test.

        prismSleep (1);

        trace (TRACE_LEVEL_INFO, string ("    Number of Messages Processed       : ") + FrameworkTestability1ObjectManager::getNumberOfProcessedMessage7s ());
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageWithBuffersWithTimeout2TestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Simple Asynchronous Messaging with buffers with TimeOut Test - With some guaranteed TimeOuts.");

    UI32               numberOfMessagesToSend = 100000;
    UI32               i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;
    LocationId         thisLocationId         = FrameworkToolKit::getThisLocationId ();

    if ("" != getTestParameterValue ("nummsg"))
    {
        numberOfMessagesToSend = atoi (getTestParameterValue ("nummsg").c_str ());
    }

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);
    FrameworkTestability1ObjectManager::setNumberOfProcessedMessage7s (0);

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage7 *pMessage = new FrameworkTestabilityMessage7 ();
        char                         *pTemp    = new char[256];

        pMessage->addBuffer (0, 256, pTemp, true);

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkLocalMessagingTestObjectManager2::frameworkTestabilityMessage7Callback2),
                       pFrameworkLocalMessagingTestContext,
                       10000,
                       thisLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkLocalMessagingTestObjectManager2::simpleAsynchronousMessageWithBuffersWithTimeout2TestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkLocalMessagingTestObjectManager2::frameworkTestabilityMessage7Callback2 (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage7 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    static UI32 numberOfTimeOuts             = 0;
    static UI32 numberOfRecallsDueToTimeOuts = 0;
    static UI32 numberOfSuccessfulReplies    = 0;

    --(*pFrameworkLocalMessagingTestContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pMessage, __FILE__, __LINE__);

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ()))
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
        }
        else
        {
            numberOfSuccessfulReplies++;
        }

        delete pMessage;
    }
    else
    {
        if (FRAMEWORK_TIME_OUT != frameworkStatus)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
        }
        else
        {
            numberOfTimeOuts++;

            if (NULL != pMessage)
            {
                numberOfRecallsDueToTimeOuts++;
            }
        }

        if (NULL != pMessage)
        {
            delete pMessage;
        }
    }

    if (0 == (pFrameworkLocalMessagingTestContext->getNumberOfCallbacksBeforeAdvancingToNextStep()))
    {
        trace (TRACE_LEVEL_INFO, string ("    Number of Messages Processed       : ") + FrameworkTestability1ObjectManager::getNumberOfProcessedMessage7s ());
        trace (TRACE_LEVEL_INFO, string ("    Number of Timeouts received        : ") + numberOfTimeOuts);
        trace (TRACE_LEVEL_INFO, string ("    Number of Recalls due to Timeouts  : ") + numberOfRecallsDueToTimeOuts);
        trace (TRACE_LEVEL_INFO, string ("    Number of Successful Replies       : ") + numberOfSuccessfulReplies);

        numberOfTimeOuts             = 0;
        numberOfRecallsDueToTimeOuts = 0;
        numberOfSuccessfulReplies    = 0;

        // Wait for a second so that even if the recipient service picked up a message from processing and
        // then we got a timed out delivery, waiting for a second makes sure that the last picked up
        // message will be processed completely before we start the next test.

        prismSleep (1);

        trace (TRACE_LEVEL_INFO, string ("    Number of Messages Processed       : ") + FrameworkTestability1ObjectManager::getNumberOfProcessedMessage7s ());
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

}
