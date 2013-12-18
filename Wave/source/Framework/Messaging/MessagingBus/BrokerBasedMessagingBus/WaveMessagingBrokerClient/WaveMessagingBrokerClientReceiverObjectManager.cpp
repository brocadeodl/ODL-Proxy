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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerClientReceiverObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerClientReceiverThread.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerRepository.h"
#include "Shell/ShellDebug.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBroker.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessageFactory.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerSubscribeMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerUnsubscribeMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessage.h"

namespace WaveNs
{

WaveMessagingBrokerClientReceiverObjectManager::WaveMessagingBrokerClientReceiverObjectManager ()
    : WaveLocalObjectManagerForUserSpecificTasks (getServiceName ())
{
    setTraceLevel                          (TRACE_LEVEL_ERROR);
    setAllowAutomaticallyUnlistenForEvents (false);

    registerWaveBrokerBasedMessageInstantiators ();

    registerDebugShellEntries ();
}

WaveMessagingBrokerClientReceiverObjectManager::~WaveMessagingBrokerClientReceiverObjectManager ()
{
}

WaveMessagingBrokerClientReceiverObjectManager *WaveMessagingBrokerClientReceiverObjectManager::getInstance ()
{
    static WaveMessagingBrokerClientReceiverObjectManager *pWaveMessagingBrokerReceiverObjectManager = new WaveMessagingBrokerClientReceiverObjectManager ();

    WaveNs::prismAssert (NULL != pWaveMessagingBrokerReceiverObjectManager, __FILE__, __LINE__);

    return (pWaveMessagingBrokerReceiverObjectManager);
}

PrismServiceId WaveMessagingBrokerClientReceiverObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string WaveMessagingBrokerClientReceiverObjectManager::getServiceName ()
{
    return ("Wave Message Broker Client Receiver");
}

void WaveMessagingBrokerClientReceiverObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    m_pServerSocketForWaveMessagingBrokerClients = new ServerStreamingSocket (FrameworkToolKit::getMessageBrokerClientPort(), 1);

    prismAssert (NULL != m_pServerSocketForWaveMessagingBrokerClients, __FILE__, __LINE__);

    status = m_pServerSocketForWaveMessagingBrokerClients->getStatus ();

    if (SERVER_STREAMING_SOCKET_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveMessagingBrokerClientReceiverObjectManager::initialize : Could not bind to the Wave Messaging Broker Receiver Port.  Please make sure that this port is not used by any other application.  Otherwise specify a different port for this purpose.");

        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "WaveMessagingBrokerClientReceiverObjectManager::initialize : Successfully bound to the Management Interface Receiver Port");

        status = WAVE_MESSAGE_SUCCESS;
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveMessagingBrokerClientReceiverObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveMessagingBrokerClientReceiverObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&WaveMessagingBrokerClientReceiverObjectManager::bootCompleteForThisLocationEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveMessagingBrokerClientReceiverObjectManager::bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
{
    // Since we told framework not to unlistenEvents, we must explicitly unlisten for events since we are going to go into an infinite while loop.

    unlistenEvents ();

    reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));

    trace (TRACE_LEVEL_INFO, "WaveMessagingBrokerClientReceiverObjectManager::bootCompleteForThisLocationEventHandler : Now accepting connections from Wave Clients.");

    for (;;)
    {
        ServerStreamingSocket *pNewServerStreamingSocket         = new ServerStreamingSocket;
        bool                   successfullyAcceptedNewConnection = false;

        prismAssert (NULL != pNewServerStreamingSocket, __FILE__, __LINE__);

        trace (TRACE_LEVEL_DEBUG, "WaveMessagingBrokerClientReceiverObjectManager::bootCompleteForThisLocationEventHandler : Awaiting NEW Management Interface Client Connections...");

        successfullyAcceptedNewConnection = m_pServerSocketForWaveMessagingBrokerClients->accept (*pNewServerStreamingSocket);

        if (true != successfullyAcceptedNewConnection)
        {
            trace (TRACE_LEVEL_INFO, "WaveMessagingBrokerClientReceiverObjectManager::bootCompleteForThisLocationEventHandler : Could not accept connection from a new Management Interface Client.  This could be because of an unwanted Client arbitrarily trying to connect to us.");

            delete pNewServerStreamingSocket;
            continue;
        }

        trace (TRACE_LEVEL_DEBUG, string ("WaveMessagingBrokerClientReceiverObjectManager::bootCompleteForThisLocationEventHandler : Accepted A NEW connection From : ") + pNewServerStreamingSocket->getPeerIpAddress () + string (" Port:") +  pNewServerStreamingSocket->getPeerPort());

        WaveMessagingBrokerClientReceiverThread *pWaveMessagingBrokerClientReceiverThread = new WaveMessagingBrokerClientReceiverThread (pNewServerStreamingSocket);

        WaveThreadStatus status = pWaveMessagingBrokerClientReceiverThread->run ();

        if (WAVE_THREAD_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveMessagingBrokerClientReceiverObjectManager::boot : A new thread COULD NOT BE CREATED to monitor messages from this newly connected Client.  Status : " + FrameworkToolKit::localize (status));

            delete pWaveMessagingBrokerClientReceiverThread;
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "WaveMessagingBrokerClientReceiverObjectManager::bootCompleteForThisLocationEventHandler : Now a new thread will monitor messages from this newly connected Client.");
        }
    }
}

void WaveMessagingBrokerClientReceiverObjectManager::registerWaveBrokerBasedMessageInstantiators ()
{
    WaveBrokerBasedMessageFactory::registerWaveBrokerBasedMessageInstantiator (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_SUBSCRIBE,   WaveBrokerSubscribeMessage::createMessageInstance);
    WaveBrokerBasedMessageFactory::registerWaveBrokerBasedMessageInstantiator (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_UNSUBSCRIBE, WaveBrokerUnsubscribeMessage::createMessageInstance);
    WaveBrokerBasedMessageFactory::registerWaveBrokerBasedMessageInstantiator (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_PUBLISH,     WaveBrokerPublishMessage::createMessageInstance);
}

void WaveMessagingBrokerClientReceiverObjectManager::registerDebugShellEntries ()
{
    addDebugFunction ((ShellCmdFunction) (&WaveMessagingBrokerClientReceiverObjectManager::debugConnectToWaveMessageBroker), "connectToWaveMessageBroker");
    addDebugFunction ((ShellCmdFunction) (&WaveMessagingBrokerClientReceiverObjectManager::debugSubscribeToWaveMessageBroker), "subscribeToWaveMessageBroker");
}

void WaveMessagingBrokerClientReceiverObjectManager::debugConnectToWaveMessageBroker (UI32 argc, vector<string> argv)
{
    string brokerName;
    string brokerIpAddress;
    SI32   brokerPort       = 0;

    if (argc < 4)
    {
        return;
    }

    brokerName      = argv[1];
    brokerIpAddress = argv[2];
    brokerPort      = atoi ((argv[3]).c_str ());

    WaveMessagingBrokerRepository *pWaveMessagingBrokerRepository = WaveMessagingBrokerRepository::getInstance ();
    bool                           isNewBrokerAdded               = false;
    WaveMessagingBroker           *pWaveMessagingBroker           = NULL;

    WaveNs::prismAssert (NULL != pWaveMessagingBrokerRepository, __FILE__, __LINE__);

    isNewBrokerAdded = pWaveMessagingBrokerRepository->addBrokerIfNotAlreadyKnown (brokerName, brokerIpAddress, brokerPort);

    if (true == isNewBrokerAdded)
    {
        pWaveMessagingBroker = pWaveMessagingBrokerRepository->checkoutBroker (brokerName);

        WaveNs::prismAssert (NULL != pWaveMessagingBroker, __FILE__, __LINE__);

        ResourceId status = pWaveMessagingBroker->connect(1, 10);

        pWaveMessagingBrokerRepository->checkinBroker (pWaveMessagingBroker);

        if (FRAMEWORK_SUCCESS == status)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "Successfully connected to the Wave Messaging Broker : " + brokerName + " : " + brokerIpAddress + string (" : ") + brokerPort);
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_WARN, "Failed to connect to the Wave Messaging Broker : " + brokerName + " : " + brokerIpAddress + string (" : ") + brokerPort);
            WaveNs::trace (TRACE_LEVEL_WARN, "    Will continue to try connecting to the Wave Messaging Broker : " + brokerName + " : " + brokerIpAddress + string (" : ") + brokerPort);
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "Failed to Add the new Wave Messaging Broker : " + brokerName + " : " + brokerIpAddress + string (" : ") + brokerPort);
    }

    return;
}

void WaveMessagingBrokerClientReceiverObjectManager::debugSubscribeToWaveMessageBroker (UI32 argc, vector<string> argv)
{
    string brokerName;
    string brokerTopic;

    if (argc < 3)
    {
        return;
    }

    brokerName      = argv[1];
    brokerTopic = argv[2];

    WaveMessagingBrokerRepository *pWaveMessagingBrokerRepository = WaveMessagingBrokerRepository::getInstance ();
    WaveMessageBrokerStatus        status                         = WAVE_MESSAGE_BROKER_SUCCESS;

    WaveNs::prismAssert (NULL != pWaveMessagingBrokerRepository, __FILE__, __LINE__);

    status = pWaveMessagingBrokerRepository->subscribeToMessageBroker (brokerName, brokerTopic);

    if (WAVE_MESSAGE_BROKER_SUCCESS == status)
    {
        WaveNs::trace (TRACE_LEVEL_SUCCESS, "Succeeded to Subscribe to the Messaging Broker Topic : " + brokerName + " : " + brokerTopic);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "Failed to Add the new Wave Messaging Broker : " + brokerName + " : " + brokerTopic + ".  Status : " + FrameworkToolKit::localize (status));
    }

    return;
}

}
