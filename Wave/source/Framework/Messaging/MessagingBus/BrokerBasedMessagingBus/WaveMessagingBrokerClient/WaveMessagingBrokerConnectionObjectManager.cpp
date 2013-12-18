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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerConnectionObjectManager.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerConnectionObjectManagerTypes.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerRepository.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBroker.h"

namespace WaveNs
{

WaveMessagingBrokerConnectionObjectManager::WaveMessagingBrokerConnectionObjectManager ()
    : WaveLocalObjectManager      (getServiceName ()),
      m_tryIntervalInMilliSeconds (5000)
{
    addOperationMap (WAVE_MESSAGING_BROKER_CONNECTION_OBJECT_MANAGER_REQUEST_TO_TRY_FOR_CONNECTION,        reinterpret_cast<PrismMessageHandler> (&WaveMessagingBrokerConnectionObjectManager::requestToTryForConnectionMessageHandler));
    addOperationMap (WAVE_MESSAGING_BROKER_CONNECTION_OBJECT_MANAGER_CANCEL_REQUEST_TO_TRY_FOR_CONNECTION, reinterpret_cast<PrismMessageHandler> (&WaveMessagingBrokerConnectionObjectManager::cancelRequestToTryForConnectionMessageHandler));
}

WaveMessagingBrokerConnectionObjectManager::~WaveMessagingBrokerConnectionObjectManager ()
{
}

string WaveMessagingBrokerConnectionObjectManager::getServiceName ()
{
    return ("Wave Messaging Broker Connection Management");
}

WaveMessagingBrokerConnectionObjectManager *WaveMessagingBrokerConnectionObjectManager::getInstance ()
{
    static WaveMessagingBrokerConnectionObjectManager *pWaveMessagingBrokerConnectionObjectManager = new WaveMessagingBrokerConnectionObjectManager ();

    WaveNs::prismAssert (NULL != pWaveMessagingBrokerConnectionObjectManager, __FILE__, __LINE__);

    return (pWaveMessagingBrokerConnectionObjectManager);
}

PrismServiceId WaveMessagingBrokerConnectionObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

bool WaveMessagingBrokerConnectionObjectManager::isAKnownWaveMessagingBroker (const string &waveMessagingBrokerName)
{
    map<string, string>::const_iterator element    = m_pendingWaveMessagingBrokerNamesToTryForConnection.find (waveMessagingBrokerName);
    map<string, string>::const_iterator endElement = m_pendingWaveMessagingBrokerNamesToTryForConnection.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void WaveMessagingBrokerConnectionObjectManager::requestToTryForConnectionMessageHandler (WaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage *pWaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage)
{
    vector<string>                 waveMessagingBrokerNames         = pWaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage->getBrokerNames ();
    UI32                           numberOfWaveMessagingBrokerNames = waveMessagingBrokerNames.size ();
    UI32                           i                                = 0;
    WaveMessagingBrokerRepository *pWaveMessagingBrokerRepository   = WaveMessagingBrokerRepository::getInstance ();

    prismAssert (NULL != pWaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage, __FILE__, __LINE__);

    for (i = 0; i < numberOfWaveMessagingBrokerNames; i++)
    {
        const string waveMessagingBrokerName                = waveMessagingBrokerNames[i];
              bool   isKnown                                = isAKnownWaveMessagingBroker (waveMessagingBrokerName);
              bool   isKnownByWaveMessagingBrokerRepository = pWaveMessagingBrokerRepository->isAKnownBrokerByName (waveMessagingBrokerName);

        if (false == isKnown)
        {
            if (true == isKnownByWaveMessagingBrokerRepository)
            {
                m_pendingWaveMessagingBrokerNamesToTryForConnection[waveMessagingBrokerName] = waveMessagingBrokerName;
            }
        }
    }

    startTimerIfNeeded ();

    pWaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pWaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage);
}

void WaveMessagingBrokerConnectionObjectManager::cancelRequestToTryForConnectionMessageHandler (WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage *pWaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage)
{
    vector<string>                 waveMessagingBrokerNames         = pWaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage->getBrokerNames ();
    UI32                           numberOfWaveMessagingBrokerNames = waveMessagingBrokerNames.size ();
    UI32                           i                                = 0;
    WaveMessagingBrokerRepository *pWaveMessagingBrokerRepository   = pWaveMessagingBrokerRepository->getInstance ();

    prismAssert (NULL != pWaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage, __FILE__, __LINE__);

    for (i = 0; i < numberOfWaveMessagingBrokerNames; i++)
    {
        const string waveMessagingBrokerName = waveMessagingBrokerNames[i];

        removeWaveMessagingBrokerNameFromPendingList (waveMessagingBrokerName);
    }

    pWaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pWaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage);
}

void WaveMessagingBrokerConnectionObjectManager::startTimerIfNeeded ()
{
    if (0 != m_timerHandle)
    {
        return;
    }

    UI32 numberOfPendingWaveMessagingBrokerNamesToTryForConnection = m_pendingWaveMessagingBrokerNamesToTryForConnection.size ();

    if (0 < numberOfPendingWaveMessagingBrokerNamesToTryForConnection)
    {
        ResourceId status = startTimer (m_timerHandle, m_tryIntervalInMilliSeconds, reinterpret_cast<PrismTimerExpirationHandler> (&WaveMessagingBrokerConnectionObjectManager::timerCallback));

        if (FRAMEWORK_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "WaveMessagingBrokerConnectionObjectManager::startTimerIfNeeded : Could not start a timer.  Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void WaveMessagingBrokerConnectionObjectManager::timerCallback (TimerHandle timerHandle, void *pContext)
{
    trace (TRACE_LEVEL_DEBUG, "WaveMessagingBrokerConnectionObjectManager::timerCallback : Entering ...");

    m_timerHandle = 0;

    prismAssert (NULL == pContext, __FILE__, __LINE__);

    map<string, string>::iterator element                                    = m_pendingWaveMessagingBrokerNamesToTryForConnection.begin ();
    map<string, string>::iterator endElement                                 = m_pendingWaveMessagingBrokerNamesToTryForConnection.end   ();

    vector<string>                brokerNamesTopBeRemovedFromThePendingList;

    while (endElement != element)
    {
        WaveMessagingBrokerRepository *pWaveMessagingBrokerRepository = WaveMessagingBrokerRepository::getInstance ();

        prismAssert (NULL != pWaveMessagingBrokerRepository, __FILE__, __LINE__);

        const string               waveMessagingBrokerName = element->first;

              WaveMessagingBroker *pWaveMessagingBroker    = pWaveMessagingBrokerRepository->checkoutBroker (waveMessagingBrokerName);

        if (NULL != pWaveMessagingBroker)
        {
            bool isConnected = pWaveMessagingBroker->isCurrentlyConnected ();

            if (false == isConnected)
            {
                ResourceId status = pWaveMessagingBroker->connect (1, 10);

                if (FRAMEWORK_SUCCESS == status)
                {
                    brokerNamesTopBeRemovedFromThePendingList.push_back (waveMessagingBrokerName);
                }
            }
            else
            {
                brokerNamesTopBeRemovedFromThePendingList.push_back (waveMessagingBrokerName);
            }

            pWaveMessagingBrokerRepository->checkinBroker (pWaveMessagingBroker);
        }
        else
        {
            brokerNamesTopBeRemovedFromThePendingList.push_back (waveMessagingBrokerName);
        }

        element++;
    }

    UI32 numberOfBrokerNamesToBeRemovedFromThePendingList = brokerNamesTopBeRemovedFromThePendingList.size ();
    UI32 i                                                = 0;

    for (i = 0; i < numberOfBrokerNamesToBeRemovedFromThePendingList; i++)
    {
        removeWaveMessagingBrokerNameFromPendingList (brokerNamesTopBeRemovedFromThePendingList[i]);
    }

    startTimerIfNeeded ();
}

void WaveMessagingBrokerConnectionObjectManager::removeWaveMessagingBrokerNameFromPendingList  (const string &waveMessagingBrokerName)
{
    map<string, string>::iterator element    = m_pendingWaveMessagingBrokerNamesToTryForConnection.find (waveMessagingBrokerName);
    map<string, string>::iterator endElement = m_pendingWaveMessagingBrokerNamesToTryForConnection.end  ();

    if (endElement != element)
    {
        m_pendingWaveMessagingBrokerNamesToTryForConnection.erase (waveMessagingBrokerName);
    }
}

}
