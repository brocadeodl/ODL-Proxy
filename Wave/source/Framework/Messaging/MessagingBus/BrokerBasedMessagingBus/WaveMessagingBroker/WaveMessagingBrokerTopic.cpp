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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerTopic.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerClientRepository.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerTopicProcessorObjectManager.h"

namespace WaveNs
{

WaveMessagingBrokerTopic::WaveMessagingBrokerTopic (const string &name)
    : m_name (name)
{
    WaveMessagingBrokerTopicProcessorObjectManager *pWaveMessagingBrokerTopicProcessorObjectManager = new WaveMessagingBrokerTopicProcessorObjectManager (name);

    prismAssert (NULL != pWaveMessagingBrokerTopicProcessorObjectManager, __FILE__, __LINE__);
}

WaveMessagingBrokerTopic::~WaveMessagingBrokerTopic ()
{
    PrismServiceId prismServiceId = WaveMessagingBrokerTopicProcessorObjectManager::getPrismServiceIdByTopicName (m_name);

    WaveObjectManager::endOfLifeService (prismServiceId);
}

bool WaveMessagingBrokerTopic::isAKnownSubscription (const string &ipAddress, const SI32 &port) const
{
    string uniqueString                            = WaveMessagingBrokerClientRepository::getWaveMessagingBrokerClientUniqueString (ipAddress, port);
    map<string, string>::const_iterator element    = m_subscribedClients.find (uniqueString);
    map<string, string>::const_iterator endElement = m_subscribedClients.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void WaveMessagingBrokerTopic::addSubscription (const string &ipAddress, const SI32 &port)
{
    string uniqueString                      = WaveMessagingBrokerClientRepository::getWaveMessagingBrokerClientUniqueString (ipAddress, port);
    map<string, string>::iterator element    = m_subscribedClients.find (uniqueString);
    map<string, string>::iterator endElement = m_subscribedClients.end  ();

    if (endElement != element)
    {
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        m_subscribedClients[uniqueString] = uniqueString;
    }
}

void WaveMessagingBrokerTopic::addSubscription (const string &uniqueString)
{
    map<string, string>::iterator element    = m_subscribedClients.find (uniqueString);
    map<string, string>::iterator endElement = m_subscribedClients.end  ();

    if (endElement != element)
    {
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        m_subscribedClients[uniqueString] = uniqueString;
    }
}

void WaveMessagingBrokerTopic::removeSubscription (const string &ipAddress, const SI32 &port)
{
    string uniqueString                      = WaveMessagingBrokerClientRepository::getWaveMessagingBrokerClientUniqueString (ipAddress, port);
    map<string, string>::iterator element    = m_subscribedClients.find (uniqueString);
    map<string, string>::iterator endElement = m_subscribedClients.end  ();

    if (endElement != element)
    {
        m_subscribedClients.erase (element);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveMessagingBrokerTopic::removeSubscription (const string &uniqueString)
{
    map<string, string>::iterator element    = m_subscribedClients.find (uniqueString);
    map<string, string>::iterator endElement = m_subscribedClients.end  ();

    if (endElement != element)
    {
        m_subscribedClients.erase (element);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveMessagingBrokerTopic::getSubscribedClients (vector<string> &subscribedClients) const
{
    map<string, string>::const_iterator element    = m_subscribedClients.begin ();
    map<string, string>::const_iterator endElement = m_subscribedClients.end   ();

    while (endElement != element)
    {
        subscribedClients.push_back (element->first);

        element++;
    }
}

UI32 WaveMessagingBrokerTopic::getNumberOfSubscribedClients () const
{
    return (m_subscribedClients.size ());
}

}
