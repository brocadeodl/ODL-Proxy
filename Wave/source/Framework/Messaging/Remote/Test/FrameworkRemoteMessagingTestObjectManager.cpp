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

#include "Framework/Messaging/Remote/Test/FrameworkRemoteMessagingTestObjectManager.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestContexts.h"
#include "Framework/Core/Test/FrameworkTestabilityMessages.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveSendMulticastContext.h"

namespace WaveNs
{

FrameworkRemoteMessagingTestObjectManager::FrameworkRemoteMessagingTestObjectManager ()
    : PrismTestObjectManager ("Framework Remote Messaging Test")
{
}

FrameworkRemoteMessagingTestObjectManager *FrameworkRemoteMessagingTestObjectManager::getInstance ()
{
    static FrameworkRemoteMessagingTestObjectManager *pFrameworkRemoteMessagingTestObjectManager = NULL;

    if (NULL == pFrameworkRemoteMessagingTestObjectManager)
    {
        pFrameworkRemoteMessagingTestObjectManager = new FrameworkRemoteMessagingTestObjectManager ();
        WaveNs::prismAssert (NULL != pFrameworkRemoteMessagingTestObjectManager, __FILE__, __LINE__);
    }

    return (pFrameworkRemoteMessagingTestObjectManager);
}

FrameworkRemoteMessagingTestObjectManager::~FrameworkRemoteMessagingTestObjectManager ()
{
}

PrismServiceId FrameworkRemoteMessagingTestObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void FrameworkRemoteMessagingTestObjectManager::testRequestHandler (RegressionTestMessage *pMessage)
{

    string numberOfMessages = getTestParameterValue("nummsg");

    if ("" != numberOfMessages)
    {
        m_regressionInput.clear();
        m_regressionInput.push_back(numberOfMessages);
    }
    
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::selectARemoteLocationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::simpleAsynchronousMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::simpleOneWayMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::simpleSynchronousMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::asynchronousMessageWithBuffersTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::synchronousMessageWithBuffersTestStep),


        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::asynchronousMessageWithLargeBuffersTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::synchronousMessageWithLargeBuffersTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::asynchronousMessageAttributeTest),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::synchronousMessageAttributeTest),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::messageCloningTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::messageCloningWithBuffersTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllSuccessTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllSucesssButPhase2MessageSetTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllFailureTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllFailureButPhase2MessageSetTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1SomeSuccessTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1SomeSucesssButPhase2MessageSetTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewSuccessDisconnectnFailWithFlagTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewSuccessDisconnectnFailButPhase2MessageWithFlagSetTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewDisconnectAndFailWithFlagTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewDisconnectAndFailButPhase2MessageWithFlagSetTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1SomeDisconnectButPhase2MessageAndFlagSetTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithPartialSuccessFlagSetAndMessage1TestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithPartialSuccessAndFailingOverFlagSetTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateAllSuccessTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithPartialSuccessAndFailingOverFlagSetTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithLocationsSetByUserStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::sendOneWayToWaveClusterTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::remoteMessagingPerformanceMeasurementForSentToWaveCluster),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::remoteMessagingPerformanceMeasurementForSentMulticast),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FrameworkRemoteMessagingTestObjectManager::prismLinearSequencerFailedStep),
    };

    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = new FrameworkLocalMessagingTestContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pFrameworkLocalMessagingTestContext->start ();
}

void FrameworkRemoteMessagingTestObjectManager::selectARemoteLocationStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    vector<LocationId> remoteLocationsVector;

    FrameworkToolKit::getConnectedLocations (remoteLocationsVector);

    if (0 == (remoteLocationsVector.size ()))
    {
        trace (TRACE_LEVEL_WARN, "FrameworkRemoteMessagingTestObjectManager::selectARemoteLocationStep : No known remote locations at this time.  Cannot run the remote messaging test.");
        pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("Selected Remote Location : ") + remoteLocationsVector[0]);
        pFrameworkLocalMessagingTestContext->setRemoteLocationId (remoteLocationsVector[0]);
        pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void FrameworkRemoteMessagingTestObjectManager::simpleAsynchronousMessageTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging              Test.");

    UI32             numberOfMessagesToSend = 10000;
    UI32             i                      = 0;
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

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkRemoteMessagingTestObjectManager::frameworkTestabilityMessage1Callback),
                       pFrameworkLocalMessagingTestContext,
                       0,
                       pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::simpleAsynchronousMessageTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
            delete pMessage;
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    trace (TRACE_LEVEL_INFO, string ("    Finished Sending ") + numberOfMessagesToSend + " messages.");

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::frameworkTestabilityMessage1Callback (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage1 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
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
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::frameworkTestabilityMessage1Callback : Number of failed messages : ") + pFrameworkLocalMessagingTestContext->getNumberOfFailures ());
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::simpleOneWayMessageTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting One Way      Messaging              Test.");

    UI32             numberOfMessagesToSend = 10000;
    UI32             i                      = 0;
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

        status = sendOneWay (pMessage, pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::simpleAsynchronousMessageTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
            delete pMessage;
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::simpleSynchronousMessageTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Synchronous  Messaging              Test.");

    UI32             numberOfMessagesToSend = 10000;
    UI32             i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage1 message;

        message.setMessage ("This is a test message");

        status = sendSynchronously (&message, pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::simpleSynchronousMessageTestStep : Failed sending a message synchronously. status = ") + status + ".");
            pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        }
        else
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::simpleSynchronousMessageTestStep : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                return;
            }
            else
            {
                trace (TRACE_LEVEL_DEVEL, "FrameworkRemoteMessagingTestObjectManager::simpleSynchronousMessageTestStep : Synchronously sent message Succeeded.");
            }
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::asynchronousMessageWithBuffersTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging with Buffers Test.");

    UI32             numberOfMessagesToSend = 10000;
    UI32             i                      = 0;
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
        FrameworkTestabilityMessage3 *pMessage = new FrameworkTestabilityMessage3 ();

        pMessage->setupInputBuffer1 ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkRemoteMessagingTestObjectManager::asynchronousMessageWithBuffersCallback),
                       pFrameworkLocalMessagingTestContext,
                       0,
                       pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::asynchronousMessageWithBuffersTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::asynchronousMessageWithBuffersCallback (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage3 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
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
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::asynchronousMessageWithBuffersCallback : Number of failed messages : ") + pFrameworkLocalMessagingTestContext->getNumberOfFailures ());
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::synchronousMessageWithBuffersTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Synchronous  Messaging with Buffers Test.");

    UI32             numberOfMessagesToSend = 10000;
    UI32             i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage3 message;

        message.setupInputBuffer1 ();

        status = sendSynchronously (&message, pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::synchronousMessageWithBuffersTestStep : Failed sending a message synchronously. status = ") + status + ".");
            pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
            return;
        }
        else
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::synchronousMessageWithBuffersTestStep : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                return;
            }
            else
            {
                if (true != (message.validateOutputBuffer1 ()))
                {
                    trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::synchronousMessageWithBuffersTestStep : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                    return;
                }

                trace (TRACE_LEVEL_DEVEL, "FrameworkRemoteMessagingTestObjectManager::synchronousMessageWithBuffersTestStep : Synchronously sent message Succeeded.");
            }
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::asynchronousMessageWithLargeBuffersTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Asynchronous Messaging with Large Buffers Test.");

    UI32             numberOfMessagesToSend = 10000;
    UI32             i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    // Limit the number of messages to send to 5 since each message will take up a 5 MB buffer.

    if (numberOfMessagesToSend > 5)
    {
        numberOfMessagesToSend = 5;
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage3 *pMessage = new FrameworkTestabilityMessage3 ();

        pMessage->setupInputBuffer1 ();
        pMessage->setInputLargeBuffer1 ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkRemoteMessagingTestObjectManager::asynchronousMessageWithBuffersCallback),
                       pFrameworkLocalMessagingTestContext,
                       0,
                       pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::asynchronousMessageWithLargeBuffersTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::synchronousMessageWithLargeBuffersTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Synchronous  Messaging with Large Buffers Test.");

    UI32             numberOfMessagesToSend = 10000;
    UI32             i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    // Limit the number of messages since we will be sending huge chunks of data (5 MB+ in each message).

    if (numberOfMessagesToSend > 100)
    {
        numberOfMessagesToSend = 100;
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage3 message;

        message.setupInputBuffer1 ();
        message.setInputLargeBuffer1 ();

        status = sendSynchronously (&message, pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::synchronousMessageWithLargeBuffersTestStep : Failed sending a message synchronously. status = ") + status + ".");
            pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
            return;
        }
        else
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::synchronousMessageWithLargeBuffersTestStep : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                return;
            }
            else
            {
                if (true != (message.validateOutputBuffer1 ()))
                {
                    trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::synchronousMessageWithLargeBuffersTestStep : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                    return;
                }

                trace (TRACE_LEVEL_DEVEL, "FrameworkRemoteMessagingTestObjectManager::synchronousMessageWithLargeBuffersTestStep : Synchronously sent message Succeeded.");
            }
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::asynchronousMessageAttributeTest (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
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
    trace (TRACE_LEVEL_INFO, "        Date, vector<Date>");
    trace (TRACE_LEVEL_INFO, "        Time, vector<Time>");
    trace (TRACE_LEVEL_INFO, "        DateTime, vector<DateTime>");
    trace (TRACE_LEVEL_INFO, "        Decimal64, vector<Decimal64>");

    UI32             numberOfMessagesToSend = 10000;
    UI32             i                      = 0;
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
        FrameworkTestabilityMessage4 *pMessage = new FrameworkTestabilityMessage4 ();

        pMessage->setupInput ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkRemoteMessagingTestObjectManager::asynchronousMessageAttributeTestCallback),
                       pFrameworkLocalMessagingTestContext,
                       0,
                       pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::asynchronousMessageAttributeTest : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::asynchronousMessageAttributeTestCallback (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage4 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
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
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::asynchronousMessageAttributeTestCallback : Number of failed messages : ") + pFrameworkLocalMessagingTestContext->getNumberOfFailures ());
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::synchronousMessageAttributeTest (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
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
    trace (TRACE_LEVEL_INFO, "        Date, vector<Date>");
    trace (TRACE_LEVEL_INFO, "        Time, vector<Time>");
    trace (TRACE_LEVEL_INFO, "        DateTime, vector<DateTime>");
    trace (TRACE_LEVEL_INFO, "        Decimal64, vector<Decimal64>");

    UI32             numberOfMessagesToSend = 10000;
    UI32             i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        FrameworkTestabilityMessage4 message;

        message.setupInput ();

        status = sendSynchronously (&message, pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::synchronousMessageAttributeTest : Failed sending a message synchronously. status = ") + status + ".");
            pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
            return;
        }
        else
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::synchronousMessageAttributeTest : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                return;
            }
            else
            {
                if (true != (message.validateContent ()))
                {
                    trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::synchronousMessageAttributeTest : Synchronously sent message could not be processed successfully. status = ") + completionStatus + ".");
                    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
                    return;
                }

                trace (TRACE_LEVEL_DEVEL, "FrameworkRemoteMessagingTestObjectManager::synchronousMessageAttributeTest : Synchronously sent message Succeeded.");
            }
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::messageCloningTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
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

    UI32                         numberOfMessagesToSend = 10000;
    UI32                         i                      = 0;
    WaveMessageStatus            status                 = WAVE_MESSAGE_ERROR;
    FrameworkTestabilityMessage4 message;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        PrismMessage                 *pTempPrismMessage = message.clone ();
        FrameworkTestabilityMessage4 *pMessage          = dynamic_cast<FrameworkTestabilityMessage4 *> (pTempPrismMessage);

        prismAssert( NULL != pMessage , __FILE__, __LINE__);

        pMessage->setupInput ();

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkRemoteMessagingTestObjectManager::asynchronousMessageAttributeTestCallback),
                       pFrameworkLocalMessagingTestContext,
                       0,
                       pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::asynchronousMessageAttributeTest : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::messageCloningWithBuffersTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Cloning Messages With Buffers Async Test.");

    UI32             numberOfMessagesToSend = 10000;
    UI32             i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    pFrameworkLocalMessagingTestContext->setNumberOfFailures (0);

    FrameworkTestabilityMessage3 message;

    message.setupInputBuffer1 ();

    ++(*pFrameworkLocalMessagingTestContext);

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        PrismMessage                 *pTempPrismMessage = message.clone ();
        FrameworkTestabilityMessage3 *pMessage          = dynamic_cast<FrameworkTestabilityMessage3 *> (pTempPrismMessage);

        prismAssert( NULL != pMessage , __FILE__, __LINE__);

        status = send (pMessage,
                       reinterpret_cast<PrismMessageResponseHandler> (&FrameworkRemoteMessagingTestObjectManager::asynchronousMessageWithBuffersCallback),
                       pFrameworkLocalMessagingTestContext,
                       0,
                       pFrameworkLocalMessagingTestContext->getRemoteLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pFrameworkLocalMessagingTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("FrameworkRemoteMessagingTestObjectManager::messageCloningWithBuffersTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getServiceCode ()) + " service] failed.");
        }
        else
        {
            ++(*pFrameworkLocalMessagingTestContext);
        }
    }

    --(*pFrameworkLocalMessagingTestContext);
    pFrameworkLocalMessagingTestContext->executeNextStep (((pFrameworkLocalMessagingTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllSuccessTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster with Buffers Async All Success Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllSuccessTestCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (0);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllSuccessTestCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllSucesssButPhase2MessageSetTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster with Buffers Async All Success But Phase 2 Set Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage32 = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllSucesssButPhase2MessageSetTestCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (0);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);

    pFrameworkTestabilityMessage32->setupInputBuffer1 ();
    pFrameworkTestabilityMessage32->setCompletionStatusType (0);

    pWaveSendToClusterContext->setPPrismMessageForPhase2 (pFrameworkTestabilityMessage32);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllSucesssButPhase2MessageSetTestCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete (pWaveSendToClusterContext->getPPrismMessageForPhase2 ());
    delete pWaveSendToClusterContext;

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllFailureTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster with Buffers Async All Failure Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllFailureTestCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (1);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllFailureTestCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == sendToClusterCompletionStatus)
    {
        sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllFailureButPhase2MessageSetTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster with Buffers Async All Failure But Phase 2 Set Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage32 = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllFailureButPhase2MessageSetTestCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (1);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);

    pFrameworkTestabilityMessage32->setupInputBuffer1 ();
    pFrameworkTestabilityMessage32->setCompletionStatusType (0);

    pWaveSendToClusterContext->setPPrismMessageForPhase2 (pFrameworkTestabilityMessage32);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1AllFailureButPhase2MessageSetTestCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete (pWaveSendToClusterContext->getPPrismMessageForPhase2 ());
    delete pWaveSendToClusterContext;

    if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == sendToClusterCompletionStatus)
    {
        sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewSuccessDisconnectnFailWithFlagTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster with Buffers Async All few success, Disconnect and failed with noRollBack flag set Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewSuccessDisconnectnFailWithFlagTestStepCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (3);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);
    pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag (true);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewSuccessDisconnectnFailWithFlagTestStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    vector<LocationId>                  locationsToSendToForPhase1          = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();
    UI32                                numberOfLocationsToSendToForPhase1  = locationsToSendToForPhase1.size ();
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    if (3 > numberOfLocationsToSendToForPhase1)
    {
        trace (TRACE_LEVEL_WARN, "FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewSuccessDisconnectnFailWithFlagTestStepCallback : Validity of this test requires at least 3 locations. One each to return Succeeded, Remote Location is not available, and Message Generic Error.  BYPASSING test.");

        sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        if (WAVE_MESSAGE_ERROR_FAILED_ON_SOME_NODES == sendToClusterCompletionStatus)
        {
            sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewSuccessDisconnectnFailWithFlagTestStepCallback : Status " + FrameworkToolKit::localize (sendToClusterCompletionStatus));
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewSuccessDisconnectnFailButPhase2MessageWithFlagSetTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster with Buffers Async few success, Disconnect and fail But Phase 2 Set with noRollBack flag set Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage32 = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewSuccessDisconnectnFailButPhase2MessageWithFlagSetTestStepCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (3);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);

    pFrameworkTestabilityMessage32->setupInputBuffer1 ();
    pFrameworkTestabilityMessage32->setCompletionStatusType (0);

    pWaveSendToClusterContext->setPPrismMessageForPhase2 (pFrameworkTestabilityMessage32);

    pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewSuccessDisconnectnFailButPhase2MessageWithFlagSetTestStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    vector<LocationId>                  locationsToSendToForPhase1          = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();
    UI32                                numberOfLocationsToSendToForPhase1  = locationsToSendToForPhase1.size ();
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete (pWaveSendToClusterContext->getPPrismMessageForPhase2 ());
    delete pWaveSendToClusterContext;

    if (3 > numberOfLocationsToSendToForPhase1)
    {
        trace (TRACE_LEVEL_WARN, "FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewSuccessDisconnectnFailButPhase2MeassageWithFlagTestStepCallback : Validity of this test requires at least 3 locations. One each to return Succeeded, Remote Location is not available, and Message Generic Error.  BYPASSING test.");

        sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        if (WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED == sendToClusterCompletionStatus)
        {
            sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewSuccessDisconnectnFailButPhase2MessageWithFlagSetTestStepCallback : Status " + FrameworkToolKit::localize (sendToClusterCompletionStatus));
        }
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewDisconnectAndFailWithFlagTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster with Buffers Async few failed and other Disconnected with noRollBack flag set Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewDisconnectAndFailWithFlagTestStepCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (4);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);
    pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag (true);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewDisconnectAndFailWithFlagTestStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == sendToClusterCompletionStatus)
    {
        sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewDisconnectAndFailButPhase2MessageWithFlagSetTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster with Buffers Async few failed and other Disconnected But Phase 2 Set with noRollBack flag set Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage32 = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewDisconnectAndFailButPhase2MessageWithFlagSetTestStepCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (4);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);

    pFrameworkTestabilityMessage32->setupInputBuffer1 ();
    pFrameworkTestabilityMessage32->setCompletionStatusType (0);

    pWaveSendToClusterContext->setPPrismMessageForPhase2 (pFrameworkTestabilityMessage32);

    pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1FewDisconnectAndFailButPhase2MessageWithFlagSetTestStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete (pWaveSendToClusterContext->getPPrismMessageForPhase2 ());
    delete pWaveSendToClusterContext;

    if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == sendToClusterCompletionStatus)
    {
        sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}


void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1SomeSuccessTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster with Buffers Async Some Success Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1SomeSuccessTestCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (2);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1SomeSuccessTestCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    if (WAVE_MESSAGE_ERROR_FAILED_ON_SOME_NODES == sendToClusterCompletionStatus)
    {
        sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1SomeSuccessTestCallback : Status " + FrameworkToolKit::localize (sendToClusterCompletionStatus));
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1SomeSucesssButPhase2MessageSetTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster with Buffers Async Some Success But Phase 2 Set Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage32 = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1SomeSucesssButPhase2MessageSetTestCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (2);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);


    pFrameworkTestabilityMessage32->setupInputBuffer1 ();
    pFrameworkTestabilityMessage32->setCompletionStatusType (0);

    pWaveSendToClusterContext->setPPrismMessageForPhase2 (pFrameworkTestabilityMessage32);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1SomeSucesssButPhase2MessageSetTestCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete (pWaveSendToClusterContext->getPPrismMessageForPhase2 ());
    delete pWaveSendToClusterContext;

    if (WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED == sendToClusterCompletionStatus)
    {
        sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1SomeSuccessButPhase2MessageSetTestCallback : Status " + FrameworkToolKit::localize (sendToClusterCompletionStatus));
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1SomeDisconnectButPhase2MessageAndFlagSetTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster with Buffers Async Some disconnects, noRollBack set But Phase 2 Set Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage32 = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1SomeDisconnectButPhase2MessageAndFlagSetTestStepCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (5);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);


    pFrameworkTestabilityMessage32->setupInputBuffer1 ();
    pFrameworkTestabilityMessage32->setCompletionStatusType (0);
        
    pWaveSendToClusterContext->setPPrismMessageForPhase2 (pFrameworkTestabilityMessage32);
    pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag (true);
    
    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterPhase1SomeDisconnectButPhase2MessageAndFlagSetTestStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete (pWaveSendToClusterContext->getPPrismMessageForPhase2 ());
    delete pWaveSendToClusterContext;

    if (WAVE_MESSAGE_SUCCESS != sendToClusterCompletionStatus)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkRemoteMessagingTestObjectManager::sendToClusterWithBuffersPhase1SomeSuccessButPhase2MessageSetTestCallback : Status " + FrameworkToolKit::localize (sendToClusterCompletionStatus));
    }

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithPartialSuccessFlagSetAndMessage1TestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster With Partial Success Flag and Message 1 Set Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithPartialSuccessFlagSetAndMessage1TestStepCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (2);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);

    pWaveSendToClusterContext->setPartialSuccessFlag (true);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithPartialSuccessFlagSetAndMessage1TestStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);


    if ((WAVE_MESSAGE_SUCCESS != sendToClusterCompletionStatus) || false == pWaveSendToClusterContext->getIsPartialSuccessCaseFlag ())
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkRemoteMessagingTestObjectManager::sendToClusterWithPartialSuccessAndFailingOverFlagSetTestStepCallback: Status " + FrameworkToolKit::localize (sendToClusterCompletionStatus));
    }
    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithPartialSuccessAndFailingOverFlagSetTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster With Partial Success And Failing Over Flag Set Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterWithPartialSuccessAndFailingOverFlagSetTestStepCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (3);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);

    pWaveSendToClusterContext->setPartialSuccessFlag (true);
    pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag (true);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterWithPartialSuccessAndFailingOverFlagSetTestStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    vector<LocationId>                  locationsToSendToForPhase1          = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();
    UI32                                numberOfLocationsToSendToForPhase1  = locationsToSendToForPhase1.size ();
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);


    if (3 > numberOfLocationsToSendToForPhase1)
    {
        trace (TRACE_LEVEL_WARN, "FrameworkRemoteMessagingTestObjectManager::sendToClusterWithPartialSuccessAndFailingOverFlagSetTestStepCallback : Validity of this test requires at least 3 locations. One each to return Succeeded, Remote Location is not available, and Message Generic Error.  BYPASSING test.");

        sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        if ((WAVE_MESSAGE_SUCCESS != sendToClusterCompletionStatus) || false == pWaveSendToClusterContext->getIsPartialSuccessCaseFlag ())
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkRemoteMessagingTestObjectManager::sendToClusterWithPartialSuccessAndFailingOverFlagSetTestStepCallback: Status " + FrameworkToolKit::localize (sendToClusterCompletionStatus));
        }
    }

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;
    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateAllSuccessTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster For Surrogate All Success Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateAllSuccessTestStepCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (0);
    pFrameworkTestabilityMessage3->setNeedSurrogateSupportFlag (true);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateAllSuccessTestStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithPartialSuccessAndFailingOverFlagSetTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster For Surrogate With Partial Success And Failing Over Flag Set Test.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithPartialSuccessAndFailingOverFlagSetTestStepCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (3);
    pFrameworkTestabilityMessage3->setNeedSurrogateSupportFlag (true);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);

    pWaveSendToClusterContext->setPartialSuccessFlag (true);
    pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag (true);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithPartialSuccessAndFailingOverFlagSetTestStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    vector<LocationId>                  connectedLocationsForPhase1         = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();;
    UI32                                numberOfLocationsToSendToForPhase1  = connectedLocationsForPhase1.size ();
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);


    if (3 > numberOfLocationsToSendToForPhase1)
    {
        trace (TRACE_LEVEL_WARN, "FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithPartialSuccessAndFailingOverFlagSetTestStepCallback : Validity of this test requires at least 3 locations. One each to return Succeeded, Remote Location is not available, and Message Generic Error.  BYPASSING test.");

        sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        if ((WAVE_MESSAGE_SUCCESS != sendToClusterCompletionStatus) || false == pWaveSendToClusterContext->getIsPartialSuccessCaseFlag ())
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithPartialSuccessAndFailingOverFlagSetTestStepCallback: Status " + FrameworkToolKit::localize (sendToClusterCompletionStatus));
        }
    }

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;
    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithLocationsSetByUserStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send To Cluster For Surrogate With Locations For phase1 by User.");

    FrameworkTestabilityMessage3 *pFrameworkTestabilityMessage3  = new FrameworkTestabilityMessage3 ();
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithLocationsSetByUserStepCallback), pFrameworkLocalMessagingTestContext);

    pFrameworkTestabilityMessage3->setupInputBuffer1 ();
    pFrameworkTestabilityMessage3->setCompletionStatusType (0);
    pFrameworkTestabilityMessage3->setNeedSurrogateSupportFlag (true);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pFrameworkTestabilityMessage3);

    pWaveSendToClusterContext->setPartialSuccessFlag (true);
    pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag (true);

    // Getting a disconnected and a connected location if any and  set to locationToSendToForPhase1 vector.
    vector <LocationId> knownLocations; 
    FrameworkToolKit::getKnownLocations (knownLocations);
    vector <LocationId> locationsForPhase1;
    bool                connectedNodeSetFlag                = false;
    UI8                 disconnectedNodeSetFlag             = false;

    for (unsigned int i = 0; i < knownLocations.size(); i++)
    {
        if (false == FrameworkToolKit::isAConnectedLocation (knownLocations [i]))
        {
            if (disconnectedNodeSetFlag)
                continue;
              
            trace (TRACE_LEVEL_DEBUG, string("FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithLocationsSetByUserStep : Disconnected location : ") + knownLocations [i]);
            locationsForPhase1.push_back(knownLocations [i]);
            disconnectedNodeSetFlag = true;     
        }
        else
        {
            if (connectedNodeSetFlag)
                continue;
            
            trace (TRACE_LEVEL_DEBUG, string("FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithLocationsSetByUserStep : Connected Location : ") + knownLocations [i]);
            locationsForPhase1.push_back(knownLocations [i]);
            connectedNodeSetFlag = true;
        }
        if (2 == locationsForPhase1.size())
        break;
    }

    trace (TRACE_LEVEL_DEBUG, string("FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithLocationsSetByUserStep : locations set = ") + locationsForPhase1.size());
    pWaveSendToClusterContext->setLocationsToSendToForPhase1 (locationsForPhase1);
    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::sendToClusterForSurrogateWithLocationsSetByUserStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    pFrameworkLocalMessagingTestContext->executeNextStep (sendToClusterCompletionStatus);
}

void FrameworkRemoteMessagingTestObjectManager::sendOneWayToWaveClusterTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Send One Way To Wave Cluster Test.");

    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::sendOneWayToWaveClusterTestStepCallback), pFrameworkLocalMessagingTestContext);

    FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();
    pMessage->setMessage ("This is a send one way to wave cluster test message");

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pMessage);

    sendOneWayToWaveCluster (pWaveSendToClusterContext);
}


void FrameworkRemoteMessagingTestObjectManager::sendOneWayToWaveClusterTestStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    // When using SendOneWayToWaveCluster we don't care about the completion status.  We just need to clean up here in the callback.

    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());
    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void FrameworkRemoteMessagingTestObjectManager::remoteMessagingPerformanceMeasurementForSentToWaveCluster (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting remoteMessagingPerformanceTestForSentToWaveCluster");

    UI32 numberOfMessagesToSend = 10000;

    if (0 != (m_regressionInput.size ())) 
    {    
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ()); 
    }    

    pFrameworkLocalMessagingTestContext->setNumberOfMessagesToSend (numberOfMessagesToSend);

    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::remoteMessagingPerformanceMeasurementForSentToWaveClusterCallback), pFrameworkLocalMessagingTestContext);

    FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();

//    pMessage->setupInput ();

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pMessage);

    pFrameworkLocalMessagingTestContext->incrementNumberOfMessagesSend ();

    sendToWaveCluster (pWaveSendToClusterContext);
}

void FrameworkRemoteMessagingTestObjectManager::remoteMessagingPerformanceMeasurementForSentToWaveClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendWaveToClusterCompletionStatus   = pWaveSendToClusterContext->getCompletionStatus ();
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkLocalMessagingTestContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    if (WAVE_MESSAGE_SUCCESS != sendWaveToClusterCompletionStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("Test Staut Failed for remoteMessagingPerformanceMeasurementForSentToWaveClusterCallback ") + FrameworkToolKit::localize (sendWaveToClusterCompletionStatus));

        pFrameworkLocalMessagingTestContext->executeNextStep (sendWaveToClusterCompletionStatus);
        return;
    }

    if (false == pFrameworkLocalMessagingTestContext->areAllMessagesSent ())
    {
        WaveSendToClusterContext     *pNewWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::remoteMessagingPerformanceMeasurementForSentToWaveClusterCallback), pFrameworkLocalMessagingTestContext);

        FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();

//        pMessage->setupInput ();

        pNewWaveSendToClusterContext->setPPrismMessageForPhase1 (pMessage);

        pFrameworkLocalMessagingTestContext->incrementNumberOfMessagesSend ();

        sendToWaveCluster (pNewWaveSendToClusterContext);
    }
    else
    {
        pFrameworkLocalMessagingTestContext->resetNumberOfMessagesSend ();

        pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void FrameworkRemoteMessagingTestObjectManager::remoteMessagingPerformanceMeasurementForSentMulticast (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext)
{
    trace (TRACE_LEVEL_INFO, "Starting remoteMessagingPerformanceMeasurementForSentMulticast");

    UI32 numberOfMessagesToSend = 10000;

    if (0 != (m_regressionInput.size ())) 
    {    
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ()); 
    }    

    pFrameworkLocalMessagingTestContext->setNumberOfMessagesToSend (numberOfMessagesToSend);

    pFrameworkLocalMessagingTestContext->resetNumberOfMessagesSend ();

    WaveSendMulticastContext *pWaveSendMulticastContext = new WaveSendMulticastContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::remoteMessagingPerformanceMeasurementForSentMulticastCallback), pFrameworkLocalMessagingTestContext);

    FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();

//    pMessage->setupInput ();

    pWaveSendMulticastContext->setPrismMessage (pMessage);

    pFrameworkLocalMessagingTestContext->incrementNumberOfMessagesSend ();

    sendMulticast (pWaveSendMulticastContext);
}

void FrameworkRemoteMessagingTestObjectManager::remoteMessagingPerformanceMeasurementForSentMulticastCallback (WaveSendMulticastContext *pWaveSendMulticastContext)
{
    FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext = reinterpret_cast<FrameworkLocalMessagingTestContext *> (pWaveSendMulticastContext->getPCallerContext ());

    if (WAVE_MESSAGE_SUCCESS != pWaveSendMulticastContext->getCompletionStatus ())
    {
        pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    delete (pWaveSendMulticastContext);

    if (false == pFrameworkLocalMessagingTestContext->areAllMessagesSent ())
    {
        WaveSendMulticastContext *pNewWaveSendMulticastContext = new WaveSendMulticastContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkRemoteMessagingTestObjectManager::remoteMessagingPerformanceMeasurementForSentMulticastCallback), pFrameworkLocalMessagingTestContext);

        FrameworkTestabilityMessage1 *pMessage = new FrameworkTestabilityMessage1 ();

//        pMessage->setupInput ();

        pNewWaveSendMulticastContext->setPrismMessage (pMessage);

        pFrameworkLocalMessagingTestContext->incrementNumberOfMessagesSend ();

        sendMulticast (pNewWaveSendMulticastContext);
    }
    else
    {
        pFrameworkLocalMessagingTestContext->resetNumberOfMessagesSend ();

        pFrameworkLocalMessagingTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

}

