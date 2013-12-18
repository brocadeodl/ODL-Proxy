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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/Test/WaveMessagingBrokerClientTestObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/Test/WaveBrokerPublishTest1Message.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessageFactory.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessage.h"

namespace WaveNs
{

WaveMessagingBrokerClientTestObjectManager::WaveMessagingBrokerClientTestObjectManager ()
    : PrismTestObjectManager (getServiceName ())
{
    WaveBrokerPublishMessageFactory::registerWaveBrokerPublishMessageInstantiator ("WaveBrokerPublishTest1Message", &WaveBrokerPublishTest1Message::createMessageInstance);
}

WaveMessagingBrokerClientTestObjectManager::~WaveMessagingBrokerClientTestObjectManager ()
{
}

string WaveMessagingBrokerClientTestObjectManager::getServiceName ()
{
    return ("Wave Messaging Broker Client Test");
}

WaveMessagingBrokerClientTestObjectManager *WaveMessagingBrokerClientTestObjectManager::getInstance ()
{
    static WaveMessagingBrokerClientTestObjectManager *pWaveMessagingBrokerClientTestObjectManager = new WaveMessagingBrokerClientTestObjectManager ();

    WaveNs::prismAssert (NULL != pWaveMessagingBrokerClientTestObjectManager, __FILE__, __LINE__);

    return (pWaveMessagingBrokerClientTestObjectManager);
}

PrismServiceId WaveMessagingBrokerClientTestObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void WaveMessagingBrokerClientTestObjectManager::testRequestHandler (RegressionTestMessage *pRegressionTestMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveMessagingBrokerClientTestObjectManager::simpleWaveBrokerConnectTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveMessagingBrokerClientTestObjectManager::simpleWaveBrokerSubscribeMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveMessagingBrokerClientTestObjectManager::simpleWaveBrokerPublishMessageTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveMessagingBrokerClientTestObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveMessagingBrokerClientTestObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pRegressionTestMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveMessagingBrokerClientTestObjectManager::simpleWaveBrokerConnectTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    string brokerName         = "broker1";
    bool   runConnectingTests = false;
    string tempString;
    string brokerIpAddress;
    SI32   brokerPort;

    brokerName      = getTestParameterValue ("broker");
    brokerIpAddress = getTestParameterValue ("brokerip");
    tempString      = getTestParameterValue ("brokerport");
    brokerPort      = atoi (tempString.c_str ());
    tempString      = getTestParameterValue ("connect");

    if ("true" == tempString)
    {
        runConnectingTests = true;
    }

    if (true == runConnectingTests)
    {
        trace (TRACE_LEVEL_INFO, "WaveMessagingBrokerClientTestObjectManager::simpleWaveBrokerConnectTestStep : Running Connecting Tests ...");

        WaveMessageBrokerStatus status = connectToMessageBroker (brokerName, brokerIpAddress, brokerPort);

        if (WAVE_MESSAGE_BROKER_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveMessagingBrokerClientTestObjectManager::simpleWaveBrokerConnectTestStep : Failed to connect to Broker.  Broker Name : " + brokerName);
        }
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveMessagingBrokerClientTestObjectManager::simpleWaveBrokerPublishMessageTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    string brokerName                = "broker1";
    bool   runPublishingTests        = false;
    string tempString;
    UI32   numberOfMessagesToPublish = 1000;

    brokerName = getTestParameterValue ("broker");
    tempString = getTestParameterValue ("publish");

    if ("true" == tempString)
    {
        runPublishingTests = true;
    }

    tempString = getTestParameterValue ("nmsgs");

    if ("" != tempString)
    {
        numberOfMessagesToPublish = atoi (tempString.c_str ());
    }

    if (true == runPublishingTests)
    {
        trace (TRACE_LEVEL_INFO, "WaveMessagingBrokerClientTestObjectManager::simpleWaveBrokerPublishMessageTestStep : Running Publish Tests ...");

        WaveBrokerPublishTest1Message *pWaveBrokerPublishTest1Message = new WaveBrokerPublishTest1Message ();

        prismAssert (NULL != pWaveBrokerPublishTest1Message, __FILE__, __LINE__);

        UI32 i = 0;

        for (i = 0; i < numberOfMessagesToPublish; i++)
        {
            pWaveBrokerPublishTest1Message->setMessageToBePublished (string ("Published Message - ") + (i + 1));

            WaveMessageBrokerStatus status = publishToMessageBroker (brokerName, pWaveBrokerPublishTest1Message);

            if (WAVE_MESSAGE_BROKER_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "WaveMessagingBrokerClientTestObjectManager::simpleWaveBrokerPublishMessageTestStep : Failed to publish message : Status : " + FrameworkToolKit::localize (status));
            }

            if (0 == ((i + 1) % 10000))
            {
                trace (TRACE_LEVEL_INFO, string ("    Published Message Count : ") + (i + 1));
            }
        }

        delete pWaveBrokerPublishTest1Message;
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveMessagingBrokerClientTestObjectManager::simpleWaveBrokerSubscribeMessageTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    string brokerName        = "broker1";
    bool   runSubscribeTests = false;
    string tempString;

    brokerName = getTestParameterValue ("broker");
    tempString = getTestParameterValue ("subscribe");

    if ("true" == tempString)
    {
        runSubscribeTests = true;
    }

    if (true == runSubscribeTests)
    {
        trace (TRACE_LEVEL_INFO, "WaveMessagingBrokerClientTestObjectManager::simpleWaveBrokerSubscribeMessageTestStep : Running Subscribe Tests ...");

        string topicToSubscribe = "WaveBrokerPublishTest1Message";

        WaveMessageBrokerStatus status = subscribeToMessageBroker (brokerName, topicToSubscribe, reinterpret_cast<WaveBrokerPublishMessageHandler>(&WaveMessagingBrokerClientTestObjectManager::simpleSubscriptionCallback));

        if (WAVE_MESSAGE_BROKER_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveMessagingBrokerClientTestObjectManager::simpleWaveBrokerSubscribeMessageTestStep : Failed to publish message : Status : " + FrameworkToolKit::localize (status));
        }
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveMessagingBrokerClientTestObjectManager::simpleSubscriptionCallback (const WaveBrokerPublishMessage * const pWaveBrokerPublishMessage)
{
    static UI64 totalNumberOfPublishedMessagesReceived  = 0;

    prismAssert (NULL != pWaveBrokerPublishMessage, __FILE__, __LINE__);

    const WaveBrokerPublishTest1Message * const pWaveBrokerPublishTest1Message = dynamic_cast<const WaveBrokerPublishTest1Message * const> (pWaveBrokerPublishMessage);

    prismAssert (NULL != pWaveBrokerPublishTest1Message, __FILE__, __LINE__);

    totalNumberOfPublishedMessagesReceived++;

    if (0 == (totalNumberOfPublishedMessagesReceived % 10000))
    {
        trace (TRACE_LEVEL_INFO, "WaveMessagingBrokerClientTestObjectManager::simpleSubscriptionCallback : " + pWaveBrokerPublishTest1Message->getMessageToBePublished () + string (" : Sequence No. : " ) + pWaveBrokerPublishTest1Message->getSequenceNumber ());
    }

    pWaveBrokerPublishTest1Message->garbageCollect ();
}

}
