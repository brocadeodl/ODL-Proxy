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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBroker.h"
#include "Framework/Utils/ClientStreamingSocket.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerClientInformation.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerSubscribeMessage.h"

namespace WaveNs
{

WaveMessagingBroker::WaveMessagingBroker (const string &name, const string &ipAddress, const SI32 &port)
    : m_name                         (name),
      m_ipAddress                    (ipAddress),
      m_port                         (port),
      m_pClientStreamingSocket       (NULL),
      m_numberOfOutstandingCheckouts (0)
{
}

WaveMessagingBroker::~WaveMessagingBroker ()
{
    m_mutex.lock ();

    if (0 != m_numberOfOutstandingCheckouts)
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    indicateClosing ();

    m_mutex.unlock ();

    if (NULL != m_pClientStreamingSocket)
    {
        delete m_pClientStreamingSocket;

        m_pClientStreamingSocket = NULL;
    }
}

string WaveMessagingBroker::getName () const
{
    return (m_name);
}

void WaveMessagingBroker::setName (const string &name)
{
    m_name = name;
}

string WaveMessagingBroker::getIpAddress () const
{
    return (m_ipAddress);
}

void WaveMessagingBroker::setIpAddress (const string &ipAddress)
{
    m_ipAddress = ipAddress;
}

SI32 WaveMessagingBroker::getPort () const
{
    return (m_port);
}

void WaveMessagingBroker::setPort (const SI32 &port)
{
    m_port = port;
}

string WaveMessagingBroker::getUniqueString () const
{
    return (m_ipAddress + string (":") + m_port);
}

ResourceId WaveMessagingBroker::connect (const UI32 &numberOfRetries, const UI32 &maximumNumberOfSecondsToTryFor)
{
    UI32       numberOfTimesWeTriedToConnect = 0;
    UI32       numberOfSecondsElapsed        = 0;
    timeval    startTime;
    timeval    currentTime;
    ResourceId status                        = FRAMEWORK_ERROR;

    invalidateConnection ();

    if (0 != (gettimeofday (&startTime, NULL)))
    {
        trace (TRACE_LEVEL_FATAL, string ("WaveMessagingBroker::connect : Could not obtain start time."));
        prismAssert (false, __FILE__, __LINE__);
        status = FRAMEWORK_ERROR;
    }
    else
    {
        while (true)
        {
            if (!(((0 == numberOfRetries) || (numberOfTimesWeTriedToConnect < numberOfRetries)) && ((0 == maximumNumberOfSecondsToTryFor) || (numberOfSecondsElapsed <= maximumNumberOfSecondsToTryFor))))
            {
                break;
            }

            m_pClientStreamingSocket = new ClientStreamingSocket (m_ipAddress, m_port, 1);

            if (NULL == m_pClientStreamingSocket)
            {
                trace (TRACE_LEVEL_FATAL, string ("WaveMessagingBroker::connect : Could not allocate a ClientSocket for Wave Messaging Broker: ") + m_ipAddress + m_port);
                prismAssert (false, __FILE__, __LINE__);
                break;
            }

            if (false == (m_pClientStreamingSocket->getIsConnected ()))
            {
                delete m_pClientStreamingSocket;

                m_pClientStreamingSocket = NULL;

                tracePrintf (TRACE_LEVEL_WARN, false, true, "+");
                fflush (stdout);
                prismSleep (1);
            }
            else
            {
                bool isSuccessful = false;

                isSuccessful = m_pClientStreamingSocket->setDefaultKeepAliveTimeouts ();

                // Need a trace message to indicate if 'Set custom Keep Alive timeout settings' failed.

                if (false == isSuccessful)
                {
                    trace (TRACE_LEVEL_ERROR, string ("WaveMessagingBroker::connect : \"setDefaultKeepAliveTimeouts ()\" failed. peerIpAddress [") + m_ipAddress + string ("]"));
                }

                // If we are able to connect post them the header.
                // the header currently contains The pass code for authorization, ipaddress and
                // the server port for this location so that the remote locations knows how to contact us back.

                isSuccessful = (*m_pClientStreamingSocket) << FrameworkToolKit::getPrismConnectionPassPhraseForMessageBroker ();

                if (true != isSuccessful)
                {
                    prismAssert (false, __FILE__, __LINE__);
                }

                // now create the Wave Messaging Broker Client Information object and post it to the Wave Messaging Broker.

                if ("127.0.0.1" == m_ipAddress)
                {
                    WaveMessagingBrokerClientInformation waveMessagingBrokerClientInformation ("", m_ipAddress, FrameworkToolKit::getMessageBrokerClientPort ());

                    //isSuccessful = (*m_pClientStreamingSocket) << (&waveManagementClientInformation);
                    string serializedData;
                    waveMessagingBrokerClientInformation.serialize2 (serializedData, SERIALIZE_WITH_ATTRIBUTE_NAME);
                    UI32 size = serializedData.size();
                    isSuccessful = (*m_pClientStreamingSocket) << (size);

                    if (true == isSuccessful)
                    {
                        isSuccessful = (*m_pClientStreamingSocket) << (serializedData);
                    }
                }
                else
                {
                    WaveMessagingBrokerClientInformation waveMessagingBrokerClientInformation ("", FrameworkToolKit::getThisLocationIpAddress (), FrameworkToolKit::getMessageBrokerClientPort ());

                    //isSuccessful = (*m_pClientStreamingSocket) << (&waveManagementClientInformation);
                    string serializedData;
                    waveMessagingBrokerClientInformation.serialize2 (serializedData, SERIALIZE_WITH_ATTRIBUTE_NAME);
                    UI32 size = serializedData.size();
                    isSuccessful = (*m_pClientStreamingSocket) << (size);

                    if (true == isSuccessful)
                    {
                        isSuccessful = (*m_pClientStreamingSocket) << (serializedData);
                    }
                }

                if (true != isSuccessful)
                {
                    trace (TRACE_LEVEL_ERROR, string ("WaveMessagingBroker::connect : Error while sending WaveManagementClientInformation."));

                    status = FRAMEWORK_ERROR;
                }
                else
                {
                    status = FRAMEWORK_SUCCESS;

                    postCurrentSubscriptions ();
                }

                break;
            }

            numberOfTimesWeTriedToConnect++;

            if (0 != (gettimeofday (&currentTime, NULL)))
            {
                trace (TRACE_LEVEL_FATAL, string ("WaveMessagingBroker::connect : Could not obtain current time."));
                prismAssert (false, __FILE__, __LINE__);
                status = FRAMEWORK_ERROR;
                break;
            }

            numberOfSecondsElapsed = (UI32) (currentTime.tv_sec - startTime.tv_sec);
        }
    }

    //tracePrintf (TRACE_LEVEL_SUCCESS, false, true, "\n");

    if (FRAMEWORK_SUCCESS != status)
    {
        requestToTryForConnection (m_name);
    }

    return (status);
}

void WaveMessagingBroker::indicateClosing ()
{
    if (NULL != m_pClientStreamingSocket)
    {
        (*m_pClientStreamingSocket) << 0xFFFFFFFF;
    }
}

void WaveMessagingBroker::checkout ()
{
    m_mutex.lock ();

    m_numberOfOutstandingCheckouts++;

    m_mutex.unlock ();
}

void WaveMessagingBroker::checkin ()
{
    m_mutex.lock ();

    if (0 == m_numberOfOutstandingCheckouts)
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    m_numberOfOutstandingCheckouts--;

    m_mutex.unlock ();
}

UI32 WaveMessagingBroker::getNumberOfOutstandingCheckouts ()
{
    UI32 numberOfOutstandingCheckouts = 0;

    m_mutex.lock ();

    numberOfOutstandingCheckouts = m_numberOfOutstandingCheckouts;

    m_mutex.unlock ();

    return (numberOfOutstandingCheckouts);
}

void WaveMessagingBroker::invalidateConnection ()
{
    m_mutex.lock ();

    if (NULL != m_pClientStreamingSocket)
    {
        indicateClosing ();

        delete m_pClientStreamingSocket;

        m_pClientStreamingSocket = NULL;
    }

    m_mutex.unlock ();
}

bool WaveMessagingBroker::isCurrentlyConnected ()
{
    bool isConnected = false;

    m_mutex.lock ();

    if (NULL != m_pClientStreamingSocket)
    {
        isConnected = true;
    }

    m_mutex.unlock ();

    return (isConnected);
}

void WaveMessagingBroker::requestToTryForConnection (const string &waveMessagingBrokerName)
{
    WaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage *pWaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage = new WaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage (waveMessagingBrokerName);

    prismAssert (NULL != pWaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage, __FILE__, __LINE__);

    WaveObjectManagerToolKit::sendOneWay (pWaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage);
}

bool WaveMessagingBroker::operator << (WaveBrokerSubscribeMessage *pWaveBrokerSubscribeMessage)
{
    bool status = false;

    m_mutex.lock ();

    if (NULL != m_pClientStreamingSocket)
    {
        status = (*m_pClientStreamingSocket) << pWaveBrokerSubscribeMessage;
    }


    vector<string> topicNames         = pWaveBrokerSubscribeMessage->getTopicNames ();
    UI32           numberOfTopicNames = topicNames.size                            ();
    UI32           i                  = 0;

    for (i = 0; i < numberOfTopicNames; i++)
    {
        m_currentSubscritptions[topicNames[i]] = topicNames[i];

        PrismServiceId prismServiceId = PrismThread::getPrismServiceIdForCurrentThread ();

        addServiceSubscription (topicNames[i], prismServiceId);
    }

    m_mutex.unlock ();

    return (status);
}

bool WaveMessagingBroker::operator << (WaveBrokerPublishMessage *pWaveBrokerPublishMessage)
{
    bool status = false;

    m_mutex.lock ();

    if (NULL != m_pClientStreamingSocket)
    {
        status = (*m_pClientStreamingSocket) << pWaveBrokerPublishMessage;
    }

    m_mutex.unlock ();

    return (status);
}

void WaveMessagingBroker::postCurrentSubscriptions ()
{
    m_mutex.lock ();

    vector<string>                      topicNames;
    map<string, string>::const_iterator element     = m_currentSubscritptions.begin ();
    map<string, string>::const_iterator endElement  = m_currentSubscritptions.end   ();

    while (element != endElement)
    {
        topicNames.push_back (element->first);

        element++;
    }

    WaveBrokerSubscribeMessage *pWaveBrokerSubscribeMessage = new WaveBrokerSubscribeMessage (topicNames);

    bool status = (*m_pClientStreamingSocket) << pWaveBrokerSubscribeMessage;

    m_mutex.unlock ();

    if (false == status)
    {
        invalidateConnection ();
        requestToTryForConnection (m_name);
    }
}

void WaveMessagingBroker::addServiceSubscription (const string &topicName, const PrismServiceId &prismServiceId)
{
    m_currentlySubscribedServicesByTopicName[topicName][prismServiceId] = prismServiceId;
}

void WaveMessagingBroker::getCurrentlySubscribedServicesForTopicName (const string &topicName, vector<PrismServiceId> &currentlySubscribedServices)
{
    currentlySubscribedServices.clear ();

    m_mutex.lock ();

    map<string, map<PrismServiceId, PrismServiceId> >::const_iterator element    = m_currentlySubscribedServicesByTopicName.find (topicName);
    map<string, map<PrismServiceId, PrismServiceId> >::const_iterator endElement = m_currentlySubscribedServicesByTopicName.end  ();

    if (endElement != element)
    {
        map<PrismServiceId, PrismServiceId>::const_iterator element1    = (element->second).begin ();
        map<PrismServiceId, PrismServiceId>::const_iterator endElement1 = (element->second).end   ();

        while (endElement1 != element1)
        {
            currentlySubscribedServices.push_back (element1->first);

            element1++;
        }
    }

    m_mutex.unlock ();
}

}
