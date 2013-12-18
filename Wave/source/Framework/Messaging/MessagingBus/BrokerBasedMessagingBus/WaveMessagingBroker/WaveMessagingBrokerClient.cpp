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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerClient.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerTopicRepository.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/ClientStreamingSocket.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerInformation.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessagePayload.h"

namespace WaveNs
{

WaveMessagingBrokerClient::WaveMessagingBrokerClient (const string &name, const string &ipAddress, const SI32 &port)
    : m_name                         (name),
      m_ipAddress                    (ipAddress),
      m_port                         (port),
      m_pClientStreamingSocket       (NULL),
      m_numberOfOutstandingCheckouts (0)

{
}

WaveMessagingBrokerClient::~WaveMessagingBrokerClient ()
{
    indicateClosing ();

    if (NULL != m_pClientStreamingSocket)
    {
        delete m_pClientStreamingSocket;

        m_pClientStreamingSocket = NULL;
    }

    removeAllSubscriptionTopics ();
}

string WaveMessagingBrokerClient::getName () const
{
    return (m_name);
}

void WaveMessagingBrokerClient::setName (const string &name)
{
    m_name = name;
}

string WaveMessagingBrokerClient::getIpAddress () const
{
    return (m_ipAddress);
}

void WaveMessagingBrokerClient::setIpAddress (const string &ipAddress)
{
    m_ipAddress = ipAddress;
}

SI32 WaveMessagingBrokerClient::getPort () const
{
    return (m_port);
}

void WaveMessagingBrokerClient::setPort (const SI32 &port)
{
    m_port = port;
}

bool WaveMessagingBrokerClient::isAKnownSubscriptionTopic (const string &subscriptionTopic) const
{
    map<string, string>::const_iterator element    = m_subscriptionTopics.find (subscriptionTopic);
    map<string, string>::const_iterator endElement = m_subscriptionTopics.end  ();

    if (endElement == element)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

void WaveMessagingBrokerClient::addSubscriptionTopic (const string &subscriptionTopic)
{
    bool isKnown = isAKnownSubscriptionTopic (subscriptionTopic);

    if (true == isKnown)
    {
        // Nothing To Do.
    }
    else
    {
        m_subscriptionTopics[subscriptionTopic] = subscriptionTopic;

        WaveMessagingBrokerTopicRepository *pWaveMessagingBrokerTopicRepository = WaveMessagingBrokerTopicRepository::getInstance ();

        prismAssert (NULL != pWaveMessagingBrokerTopicRepository, __FILE__, __LINE__);

        pWaveMessagingBrokerTopicRepository->addWaveMessagingBrokerTopicIfNotKnown (subscriptionTopic, getUniqueString ());
    }
}

void WaveMessagingBrokerClient::removeSubscriptionTopic  (const string &subscriptionTopic)
{
    map<string, string>::iterator element    = m_subscriptionTopics.find (subscriptionTopic);
    map<string, string>::iterator endElement = m_subscriptionTopics.end  ();

    if (endElement == element)
    {
        // Nothing to do.
    }
    else
    {
        m_subscriptionTopics.erase (element);

        WaveMessagingBrokerTopicRepository *pWaveMessagingBrokerTopicRepository = WaveMessagingBrokerTopicRepository::getInstance ();

        prismAssert (NULL != pWaveMessagingBrokerTopicRepository, __FILE__, __LINE__);

        pWaveMessagingBrokerTopicRepository->removeWaveMessagingBrokerTopicIfKnown (subscriptionTopic, getUniqueString ());
    }
}

void WaveMessagingBrokerClient::removeAllSubscriptionTopics  ()
{
    map<string, string>::iterator element    = m_subscriptionTopics.begin ();
    map<string, string>::iterator endElement = m_subscriptionTopics.end   ();

    while (endElement != element)
    {
        WaveMessagingBrokerTopicRepository *pWaveMessagingBrokerTopicRepository = WaveMessagingBrokerTopicRepository::getInstance ();

        prismAssert (NULL != pWaveMessagingBrokerTopicRepository, __FILE__, __LINE__);

        pWaveMessagingBrokerTopicRepository->removeWaveMessagingBrokerTopicIfKnown (element->first, getUniqueString ());

        element++;
    }

    m_subscriptionTopics.clear ();
}

string WaveMessagingBrokerClient::getUniqueString () const
{
    return (m_ipAddress + string (":") + m_port);
}

ResourceId WaveMessagingBrokerClient::connect (const UI32 &numberOfRetries, const UI32 &maximumNumberOfSecondsToTryFor)
{
    UI32       numberOfTimesWeTriedToConnect = 0;
    UI32       numberOfSecondsElapsed        = 0;
    timeval    startTime;
    timeval    currentTime;
    ResourceId status                        = FRAMEWORK_ERROR;

    if (0 != (gettimeofday (&startTime, NULL)))
    {
        trace (TRACE_LEVEL_FATAL, string ("WaveMessagingBrokerClient::connect : Could not obtain start time."));
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
                trace (TRACE_LEVEL_FATAL, string ("WaveMessagingBrokerClient::connect : Could not allocate a ClientSocket for Management Interface Client : ") + m_ipAddress + m_port);
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
                    trace (TRACE_LEVEL_ERROR, string ("WaveMessagingBrokerClient::connect : \"setDefaultKeepAliveTimeouts ()\" failed. peerIpAddress [") + m_ipAddress + string ("]"));
                }

                //m_pClientStreamingSocket->clearTcpNoDelay ();

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
                    WaveMessagingBrokerInformation waveMessagingBrokerInformation ("", m_ipAddress, FrameworkToolKit::getMessageBrokerPort ());

                    //isSuccessful = (*m_pClientStreamingSocket) << (&waveManagementClientInformation);
                    string serializedData;
                    waveMessagingBrokerInformation.serialize2 (serializedData, SERIALIZE_WITH_ATTRIBUTE_NAME);
                    UI32 size = serializedData.size();
                    isSuccessful = (*m_pClientStreamingSocket) << (size);

                    if (true == isSuccessful)
                    {
                        isSuccessful = (*m_pClientStreamingSocket) << (serializedData);
                    }
                }
                else
                {
                    WaveMessagingBrokerInformation waveMessagingBrokerInformation ("", FrameworkToolKit::getThisLocationIpAddress (), FrameworkToolKit::getMessageBrokerPort ());

                    //isSuccessful = (*m_pClientStreamingSocket) << (&waveManagementClientInformation);
                    string serializedData;
                    waveMessagingBrokerInformation.serialize2 (serializedData, SERIALIZE_WITH_ATTRIBUTE_NAME);
                    UI32 size = serializedData.size();
                    isSuccessful = (*m_pClientStreamingSocket) << (size);

                    if (true == isSuccessful)
                    {
                        isSuccessful = (*m_pClientStreamingSocket) << (serializedData);
                    }
                }

                if (true != isSuccessful)
                {
                    trace (TRACE_LEVEL_ERROR, string ("WaveMessagingBrokerClient::connect : Error while sending WaveManagementClientInformation."));

                    status = FRAMEWORK_ERROR;
                }
                else
                {
                    status = FRAMEWORK_SUCCESS;
                }

                break;
            }

            numberOfTimesWeTriedToConnect++;

            if (0 != (gettimeofday (&currentTime, NULL)))
            {
                trace (TRACE_LEVEL_FATAL, string ("WaveMessagingBrokerClient::connect : Could not obtain current time."));
                prismAssert (false, __FILE__, __LINE__);
                status = FRAMEWORK_ERROR;
                break;
            }

            numberOfSecondsElapsed = (UI32) (currentTime.tv_sec - startTime.tv_sec);
        }
    }

    //tracePrintf (TRACE_LEVEL_SUCCESS, false, true, "\n");

    return (status);
}

void WaveMessagingBrokerClient::indicateClosing ()
{
    if (NULL != m_pClientStreamingSocket)
    {
        (*m_pClientStreamingSocket) << 0xFFFFFFFF;
    }
}

void WaveMessagingBrokerClient::checkout ()
{
    m_mutex.lock ();

    m_numberOfOutstandingCheckouts++;

    m_mutex.unlock ();
}

void WaveMessagingBrokerClient::checkin ()
{
    m_mutex.lock ();

    if (0 == m_numberOfOutstandingCheckouts)
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    m_numberOfOutstandingCheckouts--;

    m_mutex.unlock ();
}

UI32 WaveMessagingBrokerClient::getNumberOfOutstandingCheckouts ()
{
    UI32 numberOfOutstandingCheckouts = 0;

    m_mutex.lock ();

    numberOfOutstandingCheckouts = m_numberOfOutstandingCheckouts;

    m_mutex.unlock ();

    return (numberOfOutstandingCheckouts);
}

bool WaveMessagingBrokerClient::operator << (WaveBrokerPublishMessage *pWaveBrokerPublishMessage)
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

bool WaveMessagingBrokerClient::operator << (WaveBrokerBasedMessagePayload *pWaveBrokerBasedMessagePayload)
{
    bool status = false;

    m_mutex.lock ();

    if (NULL != m_pClientStreamingSocket)
    {
        status = (*m_pClientStreamingSocket) << pWaveBrokerBasedMessagePayload;
    }

    m_mutex.unlock ();

    return (status);
}

void WaveMessagingBrokerClient::getAllSubscribedTopics (vector<string> &allsubscribedTopics)
{
    map<string, string>::const_iterator element    = m_subscriptionTopics.begin ();
    map<string, string>::const_iterator endElement = m_subscriptionTopics.end   ();

    allsubscribedTopics.clear ();

    while (endElement != element)
    {
        allsubscribedTopics.push_back (element->first);

        element++;
    }
}

}
