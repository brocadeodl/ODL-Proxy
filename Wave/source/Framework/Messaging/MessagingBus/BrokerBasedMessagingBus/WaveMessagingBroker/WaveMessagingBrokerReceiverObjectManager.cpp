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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerReceiverObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerReceiverThread.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessageFactory.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerSubscribeMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerUnsubscribeMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessage.h"
#include "Shell/ShellDebug.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerClientRepository.h"

namespace WaveNs
{

WaveMessagingBrokerReceiverObjectManager::WaveMessagingBrokerReceiverObjectManager ()
    : WaveLocalObjectManagerForUserSpecificTasks (getServiceName ())
{
    setTraceLevel                          (TRACE_LEVEL_ERROR);
    setAllowAutomaticallyUnlistenForEvents (false);

    registerWaveBrokerBasedMessageInstantiators ();

    registerDebugShellEntries ();
}

WaveMessagingBrokerReceiverObjectManager::~WaveMessagingBrokerReceiverObjectManager ()
{
}

WaveMessagingBrokerReceiverObjectManager *WaveMessagingBrokerReceiverObjectManager::getInstance ()
{
    static WaveMessagingBrokerReceiverObjectManager *pWaveMessagingBrokerReceiverObjectManager = new WaveMessagingBrokerReceiverObjectManager ();

    WaveNs::prismAssert (NULL != pWaveMessagingBrokerReceiverObjectManager, __FILE__, __LINE__);

    return (pWaveMessagingBrokerReceiverObjectManager);
}

PrismServiceId WaveMessagingBrokerReceiverObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string WaveMessagingBrokerReceiverObjectManager::getServiceName ()
{
    return ("Wave Message Broker Receiver");
}

void WaveMessagingBrokerReceiverObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    m_pServerSocketForWaveMessagingBrokerClients = new ServerStreamingSocket (FrameworkToolKit::getMessageBrokerPort(), 1);

    prismAssert (NULL != m_pServerSocketForWaveMessagingBrokerClients, __FILE__, __LINE__);

    status = m_pServerSocketForWaveMessagingBrokerClients->getStatus ();

    if (SERVER_STREAMING_SOCKET_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveMessagingBrokerReceiverObjectManager::initialize : Could not bind to the Wave Messaging Broker Receiver Port.  Please make sure that this port is not used by any other application.  Otherwise specify a different port for this purpose.");

        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "WaveMessagingBrokerReceiverObjectManager::initialize : Successfully bound to the Management Interface Receiver Port");

        status = WAVE_MESSAGE_SUCCESS;
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveMessagingBrokerReceiverObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveMessagingBrokerReceiverObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&WaveMessagingBrokerReceiverObjectManager::bootCompleteForThisLocationEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveMessagingBrokerReceiverObjectManager::bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
{
    // Since we told framework not to unlistenEvents, we must explicitly unlisten for evnets since we asre going to go into an infinite while loop.

    unlistenEvents ();

    reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));

    trace (TRACE_LEVEL_INFO, "WaveMessagingBrokerReceiverObjectManager::bootCompleteForThisLocationEventHandler : Now accepting connections from Wave Clients.");

    for (;;)
    {
        ServerStreamingSocket *pNewServerStreamingSocket         = new ServerStreamingSocket;
        bool                   successfullyAcceptedNewConnection = false;

        prismAssert (NULL != pNewServerStreamingSocket, __FILE__, __LINE__);

        trace (TRACE_LEVEL_DEBUG, "WaveMessagingBrokerReceiverObjectManager::bootCompleteForThisLocationEventHandler : Awaiting NEW Management Interface Client Connections...");

        successfullyAcceptedNewConnection = m_pServerSocketForWaveMessagingBrokerClients->accept (*pNewServerStreamingSocket);

        if (true != successfullyAcceptedNewConnection)
        {
            trace (TRACE_LEVEL_INFO, "WaveMessagingBrokerReceiverObjectManager::bootCompleteForThisLocationEventHandler : Could not accept connection from a new Management Interface Client.  This could be because of an unwanted Client arbitrarily trying to connect to us.");

            delete pNewServerStreamingSocket;
            continue;
        }

        trace (TRACE_LEVEL_DEBUG, string ("WaveMessagingBrokerReceiverObjectManager::bootCompleteForThisLocationEventHandler : Accepted A NEW connection From : ") + pNewServerStreamingSocket->getPeerIpAddress () + string (" Port:") +  pNewServerStreamingSocket->getPeerPort());

        WaveMessagingBrokerReceiverThread *pWaveMessagingBrokerReceiverThread = new WaveMessagingBrokerReceiverThread (pNewServerStreamingSocket);

        WaveThreadStatus status = pWaveMessagingBrokerReceiverThread->run ();

        if (WAVE_THREAD_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveMessagingBrokerReceiverObjectManager::boot : A new thread COULD NOT BE CREATED to monitor messages from this newly connected Client.  Status : " + FrameworkToolKit::localize (status));

            delete pWaveMessagingBrokerReceiverThread;
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "WaveMessagingBrokerReceiverObjectManager::bootCompleteForThisLocationEventHandler : Now a new thread will monitor messages from this newly connected Client.");
        }
    }
}

void WaveMessagingBrokerReceiverObjectManager::registerWaveBrokerBasedMessageInstantiators ()
{
    WaveBrokerBasedMessageFactory::registerWaveBrokerBasedMessageInstantiator (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_SUBSCRIBE,   WaveBrokerSubscribeMessage::createMessageInstance);
    WaveBrokerBasedMessageFactory::registerWaveBrokerBasedMessageInstantiator (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_UNSUBSCRIBE, WaveBrokerUnsubscribeMessage::createMessageInstance);
    WaveBrokerBasedMessageFactory::registerWaveBrokerBasedMessageInstantiator (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_PUBLISH,     WaveBrokerPublishMessage::createMessageInstance);
}

void WaveMessagingBrokerReceiverObjectManager::debugPrintBrokerClientRepository (UI32 argc, vector<string> argv)
{
    WaveMessagingBrokerClientRepository *pWaveMessagingBrokerClientRepository  = WaveMessagingBrokerClientRepository::getInstance ();

    WaveNs::prismAssert (NULL != pWaveMessagingBrokerClientRepository, __FILE__, __LINE__);

    pWaveMessagingBrokerClientRepository->print ();
}

void WaveMessagingBrokerReceiverObjectManager::registerDebugShellEntries ()
{
    addDebugFunction ((ShellCmdFunction) (&WaveMessagingBrokerReceiverObjectManager::debugPrintBrokerClientRepository), "printBrokerClientRepository");
}

}
