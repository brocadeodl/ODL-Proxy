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

/**************************************************************************
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Messaging/Remote/Test/FrameworkRemoteMessagingTestObjectManager2.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestContexts.h"
#include "Framework/Core/Test/FrameworkTestabilityMessages.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

FrameworkRemoteMessagingTestObjectManager2::FrameworkRemoteMessagingTestObjectManager2 ()
    : PrismTestObjectManager ("Framework Remote Messaging Test 2")
{
}

FrameworkRemoteMessagingTestObjectManager2 *FrameworkRemoteMessagingTestObjectManager2::getInstance ()
{
    static FrameworkRemoteMessagingTestObjectManager2 *pFrameworkRemoteMessagingTestObjectManager2 = NULL;

    if (NULL == pFrameworkRemoteMessagingTestObjectManager2)
    {
        pFrameworkRemoteMessagingTestObjectManager2 = new FrameworkRemoteMessagingTestObjectManager2 ();
        WaveNs::prismAssert (NULL != pFrameworkRemoteMessagingTestObjectManager2, __FILE__, __LINE__);
    }

    return (pFrameworkRemoteMessagingTestObjectManager2);
}

FrameworkRemoteMessagingTestObjectManager2::~FrameworkRemoteMessagingTestObjectManager2 ()
{
}

PrismServiceId FrameworkRemoteMessagingTestObjectManager2::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void FrameworkRemoteMessagingTestObjectManager2::testRequestHandler (RegressionTestMessage *pMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::selectARemoteLocationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::simpleAsynchronousMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::simpleOneWayMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::simpleSynchronousMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithBuffersTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithBuffersTestStep),

        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithLargeBuffersTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithLargeBuffersTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageAttributeTest),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::synchronousMessageAttributeTest),
        //reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::messageCloningTestStep),
        //reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::messageCloningWithBuffersTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager2::prismLinearSequencerFailedStep),
    };

    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = new FrameworkLocalMessagingTestContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pFrameworkLocalMessagingTestContext->start ();
}

void FrameworkRemoteMessagingTestObjectManager2::selectARemoteLocationStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    string      ipAddress   =   "";
    SI32        port        =   0;
    LocationId  locationId  =   0;

    ipAddress = getTestParameterValue("ip");
    port = (SI32) atoi( getTestParameterValue("port").c_str() );

    trace (TRACE_LEVEL_WARN, string("FrameworkRemoteMessagingTestObjectManager2::selectARemoteLocationStep : ipAddress => ") + ipAddress + string(" Port => ") + port);

    if ( 0 >= ipAddress.length() || 0 == port )
    {
        trace (TRACE_LEVEL_WARN, "FrameworkRemoteMessagingTestObjectManager2::selectARemoteLocationStep : No known remote locations at this time.  Cannot run the remote messaging test.");
        pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }
    else
    {
        locationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress, port);
        trace (TRACE_LEVEL_INFO, string ("Selected Remote Location : ") + locationId );
        pFrameworkLocalMessagingTestContext->setRemoteLocationId ( locationId );
        pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void FrameworkRemoteMessagingTestObjectManager2::simpleAsynchronousMessageTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging              Test.");

    m_startTime = time (NULL);
    ctime (&m_startTime);

    m_numberOfMessagesToSend = 10000;

    UI32             i                        = 0;
    WaveMessageStatus status                  = WAVE_MESSAGE_ERROR;

    if ("" != getTestParameterValue("numberOfMessages"))
    {
        m_numberOfMessagesToSend = atoi (getTestParameterValue("numberOfMessages").c_str());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + m_numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < m_numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();

        pMessage->setMessage ("This is a test message");

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkRemoteMessagingTestObjectManager2::frameworkTestabilityMessage1Callback),
                       pFrameworkLocalMessagingTestContext,
                       0,
                       pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::simpleAsynchronousMessageTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
            delete pMessage;
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    trace (TRACE_LEVEL_INFO, string ("    Finished Sending ") + m_numberOfMessagesToSend + " messages.");

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::frameworkTestabilityMessage1Callback (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage1 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    --(*pFrameworkLocalMessagingTestContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pMessage, __FILE__, __LINE__);

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

    if (0 == (pFrameworkLocalMessagingTestContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        if (0 != (pFrameworkLocalMessagingTestContext->getNumberOfFailures ()))
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::frameworkTestabilityMessage1Callback : Number of failed messages : ") + pFrameworkLocalMessagingTestContext->getNumberOfFailures ());
        }
        else
        {
            UI64 timeTaken = 0;
            m_endTime = time (NULL);
            ctime (&m_endTime);
            timeTaken = (UI64) difftime(m_endTime, m_startTime);
            trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::frameworkTestabilityMessage1Callback : Time taken:") + timeTaken + string ("s"));
            trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::frameworkTestabilityMessage1Callback : No. of messages:") + m_numberOfMessagesToSend);
        }
    }

    

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::simpleOneWayMessageTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting One Way      Messaging              Test.");

    m_startTime = time (NULL);
    ctime (&m_startTime);

    m_numberOfMessagesToSend = 10000;

    UI32             i                        = 0;
    WaveMessageStatus status                  = WAVE_MESSAGE_ERROR;

    if ("" != getTestParameterValue("numberOfMessages"))
    {
        m_numberOfMessagesToSend = atoi (getTestParameterValue("numberOfMessages").c_str());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + m_numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < m_numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();

        pMessage->setMessage ("This is a test message");

        status = sendOneWay (pMessage, pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::simpleAsynchronousMessageTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
            delete pMessage;
        }
    }

    --(*pFrameworkLocalMessagingTestContext);

    if (0 == pFrameworkLocalMessagingTestContext->getNumberOfFailures ())
    {
        UI64 timeTaken = 0;

        m_endTime = time (NULL);
        ctime (&m_endTime);
        timeTaken = (UI64) difftime(m_endTime, m_startTime);

        trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2:simpleOneWayMessageTestStep: Time taken:") + timeTaken + string ("s"));
        trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::simpleOneWayMessageTestStep: No. of messages:") + m_numberOfMessagesToSend);
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::simpleSynchronousMessageTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Synchronous  Messaging              Test.");

    m_startTime = time (NULL);
    ctime (&m_startTime);

    m_numberOfMessagesToSend = 10000;

    UI32             i                        = 0;
    WaveMessageStatus status                  = WAVE_MESSAGE_ERROR;

    if ("" != getTestParameterValue("numberOfMessages"))
    {
        m_numberOfMessagesToSend = atoi (getTestParameterValue("numberOfMessages").c_str());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + m_numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    for (i = 0; i < m_numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage1 message;

        message.setMessage ("This is a test message");

        status = sendSynchronously (&message, pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::simpleSynchronousMessageTestStep : Failed sending a message synchronously. status = ") + status + ".");
            pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        }
        else
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::simpleSynchronousMessageTestStep : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                return;
            }
            else
            {
                trace (TRACE_LEVEL_DEVEL, "FrameworkRemoteMessagingTestObjectManager2::simpleSynchronousMessageTestStep : Synchronously sent message Succeeded.");
            }
        }
    }

    UI64 timeTaken = 0;

    m_endTime = time (NULL);
    ctime (&m_endTime);
    timeTaken = (UI64) difftime(m_endTime, m_startTime);

    trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::simpleSynchronousMessageTestStep: Time taken:") + timeTaken + string ("s"));
    trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::simpleSynchronousMessageTestStep: No. of messages:") + m_numberOfMessagesToSend);

    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithBuffersTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging with Buffers Test.");

    m_startTime = time (NULL);
    ctime (&m_startTime);

    m_numberOfMessagesToSend = 10000;

    UI32             i                        = 0;
    WaveMessageStatus status                  = WAVE_MESSAGE_ERROR;

    if ("" != getTestParameterValue("numberOfMessages"))
    {
        m_numberOfMessagesToSend = atoi (getTestParameterValue("numberOfMessages").c_str());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + m_numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < m_numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage3 *pMessage = new FrameworkTestabilityMessage3 ();

        pMessage->setupInputBuffer1 ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithBuffersCallback),
                       pFrameworkLocalMessagingTestContext,
                       0,
                       pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithBuffersTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithBuffersCallback (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage3 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    --(*pFrameworkLocalMessagingTestContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pMessage, __FILE__, __LINE__);

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ()))
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
        }
        else
        {
            if (true != (pMessage->validateOutputBuffer1 ()))
            {
                pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            }
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

    if (0 == (pFrameworkLocalMessagingTestContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        if (0 != (pFrameworkLocalMessagingTestContext->getNumberOfFailures ()))
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithBuffersCallback : Number of failed messages : ") + pFrameworkLocalMessagingTestContext->getNumberOfFailures ());
        }
        else
        {
            UI64 timeTaken = 0;

            m_endTime = time (NULL);
            ctime (&m_endTime);
            timeTaken = (UI64) difftime(m_endTime, m_startTime);

            trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithBuffersCallback: Time taken:") + timeTaken + string ("s"));
            trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithBuffersCallback: No. of messages:") + m_numberOfMessagesToSend);
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithBuffersTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Synchronous  Messaging with Buffers Test.");

    m_startTime = time (NULL);
    ctime (&m_startTime);

    m_numberOfMessagesToSend = 10000;

    UI32             i                        = 0;
    WaveMessageStatus status                  = WAVE_MESSAGE_ERROR;

    if ("" != getTestParameterValue("numberOfMessages"))
    {
        m_numberOfMessagesToSend = atoi (getTestParameterValue("numberOfMessages").c_str());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + m_numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    for (i = 0; i < m_numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage3 message;

        message.setupInputBuffer1 ();

        status = sendSynchronously (&message, pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithBuffersTestStep : Failed sending a message synchronously. status = ") + status + ".");
            pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
            return;
        }
        else
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithBuffersTestStep : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                return;
            }
            else
            {
                if (true != (message.validateOutputBuffer1 ()))
                {
                    trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithBuffersTestStep : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                    return;
                }

                trace (TRACE_LEVEL_DEVEL, "FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithBuffersTestStep : Synchronously sent message Succeeded.");
            }
        }
    }

    UI64 timeTaken = 0;

    m_endTime = time (NULL);
    ctime (&m_endTime);
    timeTaken = (UI64) difftime(m_endTime, m_startTime);

    trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithBuffersTestStep: Time taken:") + timeTaken + string ("s"));
    trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithBuffersTestStep: No. of messages:") + m_numberOfMessagesToSend);

    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithLargeBuffersTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging with Large Buffers Test.");

    m_startTime = time (NULL);
    ctime (&m_startTime);

    m_numberOfMessagesToSend = 10000;

    UI32             i                        = 0;
    WaveMessageStatus status                  = WAVE_MESSAGE_ERROR;

    if ("" != getTestParameterValue("numberOfMessages"))
    {
        m_numberOfMessagesToSend = atoi (getTestParameterValue("numberOfMessages").c_str());
    }

    // Limit the number of messages to send to 5 since each message will take up a 5 MB buffer.

    if (m_numberOfMessagesToSend > 5)
    {
        m_numberOfMessagesToSend = 5;
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + m_numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < m_numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage3 *pMessage = new FrameworkTestabilityMessage3 ();

        pMessage->setupInputBuffer1 ();
        pMessage->setInputLargeBuffer1 ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithBuffersCallback),
                       pFrameworkLocalMessagingTestContext,
                       0,
                       pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithLargeBuffersTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithLargeBuffersTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Synchronous  Messaging with Large Buffers Test.");

    m_startTime = time (NULL);
    ctime (&m_startTime);

    m_numberOfMessagesToSend = 10000;

    UI32             i                        = 0;
    WaveMessageStatus status                  = WAVE_MESSAGE_ERROR;

    if ("" != getTestParameterValue("numberOfMessages"))
    {
        m_numberOfMessagesToSend = atoi (getTestParameterValue("numberOfMessages").c_str());
    }

    // Limit the number of messages since we will be sending huge chunks of data (5 MB+ in each message).

    if (m_numberOfMessagesToSend > 5)
    {
        m_numberOfMessagesToSend = 5;
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + m_numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    for (i = 0; i < m_numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage3 message;

        message.setupInputBuffer1 ();
        message.setInputLargeBuffer1 ();

        status = sendSynchronously (&message, pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithLargeBuffersTestStep : Failed sending a message synchronously. status = ") + status + ".");
            pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
            return;
        }
        else
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithLargeBuffersTestStep : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                return;
            }
            else
            {
                if (true != (message.validateOutputBuffer1 ()))
                {
                    trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithLargeBuffersTestStep : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                    return;
                }

                trace (TRACE_LEVEL_DEVEL, "FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithLargeBuffersTestStep : Synchronously sent message Succeeded.");
            }
        }
    }

    UI64 timeTaken = 0;

    m_endTime = time (NULL);
    ctime (&m_endTime);
    timeTaken = (UI64) difftime(m_endTime, m_startTime);

    trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithLargeBuffersTestStep: Time taken:") + timeTaken + string ("s"));
    trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageWithLargeBuffersTestStep: No. of messages:") + m_numberOfMessagesToSend);

    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageAttributeTest (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging Attribute    Test.");
    trace (TRACE_LEVEL_INFO, "    Tests the following attribute types:");
    trace (TRACE_LEVEL_INFO, "        SI8, SI16, SI32");
    trace (TRACE_LEVEL_INFO, "        UI8, UI16, UI32");
    trace (TRACE_LEVEL_INFO, "        UI64, SI64");
    trace (TRACE_LEVEL_INFO, "        char, string, vector<string>");
    trace (TRACE_LEVEL_INFO, "        bool");
    trace (TRACE_LEVEL_INFO, "        LocationId, ResourceId");
    trace (TRACE_LEVEL_INFO, "        WaveMessageType, WaveMessagePriority");
    trace (TRACE_LEVEL_INFO, "        PrismServiceId");
    trace (TRACE_LEVEL_INFO, "        SerializableObjectType");
    trace (TRACE_LEVEL_INFO, "        TraceClientId, TraceLevel");
    trace (TRACE_LEVEL_INFO, "        WorldWideName, vector<WorldWideName>");
    trace (TRACE_LEVEL_INFO, "        MacAddress, vector<MacAddress>");

    m_startTime = time (NULL);
    ctime (&m_startTime);

    m_numberOfMessagesToSend = 10000;

    UI32             i                        = 0;
    WaveMessageStatus status                  = WAVE_MESSAGE_ERROR;

    if ("" != getTestParameterValue("numberOfMessages"))
    {
        m_numberOfMessagesToSend = atoi (getTestParameterValue("numberOfMessages").c_str());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + m_numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < m_numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage4 *pMessage = new FrameworkTestabilityMessage4 ();

        pMessage->setupInput ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageAttributeTestCallback),
                       pFrameworkLocalMessagingTestContext,
                       0,
                       pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageAttributeTest : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageAttributeTestCallback (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage4 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    --(*pFrameworkLocalMessagingTestContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pMessage, __FILE__, __LINE__);

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ()))
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
        }
        else
        {
            if (true != (pMessage->validateContent ()))
            {
                pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            }
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

    if (0 == (pFrameworkLocalMessagingTestContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        if (0 != (pFrameworkLocalMessagingTestContext->getNumberOfFailures ()))
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageAttributeTestCallback : Number of failed messages : ") + pFrameworkLocalMessagingTestContext->getNumberOfFailures ());
        }
        else
        {
            UI64 timeTaken = 0;

            m_endTime = time (NULL);
            ctime (&m_endTime);
            timeTaken = (UI64) difftime(m_endTime, m_startTime);

            trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageAttributeTestCallback: Time taken:") + timeTaken + string ("s"));
            trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageAttributeTestCallback: No. of messages:") + m_numberOfMessagesToSend);

        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::synchronousMessageAttributeTest (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Synchronous  Messaging Attribute    Test.");
    trace (TRACE_LEVEL_INFO, "    Tests the following attribute types:");
    trace (TRACE_LEVEL_INFO, "        SI8, SI16, SI32");
    trace (TRACE_LEVEL_INFO, "        UI8, UI16, UI32");
    trace (TRACE_LEVEL_INFO, "        UI64, SI64");
    trace (TRACE_LEVEL_INFO, "        char, string, vector<string>");
    trace (TRACE_LEVEL_INFO, "        bool");
    trace (TRACE_LEVEL_INFO, "        LocationId, ResourceId");
    trace (TRACE_LEVEL_INFO, "        WaveMessageType, WaveMessagePriority");
    trace (TRACE_LEVEL_INFO, "        PrismServiceId");
    trace (TRACE_LEVEL_INFO, "        SerializableObjectType");
    trace (TRACE_LEVEL_INFO, "        TraceClientId, TraceLevel");
    trace (TRACE_LEVEL_INFO, "        WorldWideName, vector<WorldWideName>");
    trace (TRACE_LEVEL_INFO, "        MacAddress, vector<MacAddress>");

    m_startTime = time (NULL);
    ctime (&m_startTime);

    m_numberOfMessagesToSend = 10000;

    UI32             i                        = 0;
    WaveMessageStatus status                  = WAVE_MESSAGE_ERROR;

    if ("" != getTestParameterValue("numberOfMessages"))
    {
        m_numberOfMessagesToSend = atoi (getTestParameterValue("numberOfMessages").c_str());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + m_numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    for (i = 0; i < m_numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage4 message;

        message.setupInput ();

        status = sendSynchronously (&message, pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageAttributeTest : Failed sending a message synchronously. status = ") + status + ".");
            pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
            return;
        }
        else
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageAttributeTest : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                return;
            }
            else
            {
                if (true != (message.validateContent ()))
                {
                    trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageAttributeTest : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                    return;
                }

                trace (TRACE_LEVEL_DEVEL, "FrameworkRemoteMessagingTestObjectManager2::synchronousMessageAttributeTest : Synchronously sent message Succeeded.");
            }
        }
    }

    UI64 timeTaken = 0;

    m_endTime = time (NULL);
    ctime (&m_endTime);
    timeTaken = (UI64) difftime(m_endTime, m_startTime);

    trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageAttributeTest: Time taken:") + timeTaken + string ("s"));
    trace (TRACE_LEVEL_INFO, string ("FrameworkRemoteMessagingTestObjectManager2::synchronousMessageAttributeTest: No. of messages:") + m_numberOfMessagesToSend);

    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::messageCloningTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Message Cloning and Sending Aync    Test.");
    trace (TRACE_LEVEL_INFO, "    Tests the following attribute types:");
    trace (TRACE_LEVEL_INFO, "        SI8, SI16, SI32");
    trace (TRACE_LEVEL_INFO, "        UI8, UI16, UI32");
    trace (TRACE_LEVEL_INFO, "        UI64, SI64");
    trace (TRACE_LEVEL_INFO, "        char, string, vector<string>");
    trace (TRACE_LEVEL_INFO, "        bool");
    trace (TRACE_LEVEL_INFO, "        LocationId, ResourceId");
    trace (TRACE_LEVEL_INFO, "        WaveMessageType, WaveMessagePriority");
    trace (TRACE_LEVEL_INFO, "        PrismServiceId");
    trace (TRACE_LEVEL_INFO, "        SerializableObjectType");
    trace (TRACE_LEVEL_INFO, "        TraceClientId, TraceLevel");
    trace (TRACE_LEVEL_INFO, "        WorldWideName, vector<WorldWideName>");
    trace (TRACE_LEVEL_INFO, "        MacAddress, vector<MacAddress>");

    m_numberOfMessagesToSend = 10000;

    UI32                         i                        = 0;
    WaveMessageStatus            status                   = WAVE_MESSAGE_ERROR;
    FrameworkTestabilityMessage4 message;

    if ("" != getTestParameterValue("numberOfMessages"))
    {
        m_numberOfMessagesToSend = atoi (getTestParameterValue("numberOfMessages").c_str());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + m_numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < m_numberOfMessagesToSend; i++)
    {
        PrismMessage                 *pTempPrismMessage = message.clone ();
        FrameworkTestabilityMessage4 *pMessage          = dynamic_cast<FrameworkTestabilityMessage4 *> (pTempPrismMessage);

        prismAssert( NULL != pMessage , __FILE__ , __LINE__ );

        pMessage->setupInput ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageAttributeTestCallback),
                       pFrameworkLocalMessagingTestContext,
                       0,
                       pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageAttributeTest : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager2::messageCloningWithBuffersTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Cloning Messages With Buffers Async Test.");

    m_numberOfMessagesToSend = 10000;

    UI32             i                        = 0;
    WaveMessageStatus status                  = WAVE_MESSAGE_ERROR;

    if (0 != (m_regressionInput.size ()))
    {
        m_numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + m_numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    FrameworkTestabilityMessage3 message;

    message.setupInputBuffer1 ();

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < m_numberOfMessagesToSend; i++)
    {
        PrismMessage                 *pTempPrismMessage = message.clone ();
        FrameworkTestabilityMessage3 *pMessage          = dynamic_cast<FrameworkTestabilityMessage3 *> (pTempPrismMessage);

        prismAssert( NULL != pMessage , __FILE__ , __LINE__ );

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkRemoteMessagingTestObjectManager2::asynchronousMessageWithBuffersCallback),
                       pFrameworkLocalMessagingTestContext,
                       0,
                       pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager2::messageCloningWithBuffersTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

}
