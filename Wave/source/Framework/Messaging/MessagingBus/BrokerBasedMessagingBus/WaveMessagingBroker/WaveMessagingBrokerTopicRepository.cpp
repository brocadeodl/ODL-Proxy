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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerTopicRepository.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerTopic.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

WaveMessagingBrokerTopicRepository::WaveMessagingBrokerTopicRepository ()
{
}

WaveMessagingBrokerTopicRepository::~WaveMessagingBrokerTopicRepository ()
{
}

bool WaveMessagingBrokerTopicRepository::isAKnownWaveMessagingBrokerTopic (const string &waveMessagingBrokerTopicName)
{
    bool isKnown = false;

    m_waveMesasgingBrokerTopicsMutex.lock ();

    map<string, WaveMessagingBrokerTopic *>::const_iterator element    = m_waveMesasgingBrokerTopics.find (waveMessagingBrokerTopicName);
    map<string, WaveMessagingBrokerTopic *>::const_iterator endElement = m_waveMesasgingBrokerTopics.end  ();

    if (element != endElement)
    {
        isKnown = true;
    }
    else
    {
        isKnown = false;
    }

    m_waveMesasgingBrokerTopicsMutex.unlock ();

    return (isKnown);
}

void WaveMessagingBrokerTopicRepository::addWaveMessagingBrokerTopic (const string &waveMessagingBrokerTopicName, const string &clientUniqueString)
{
    m_waveMesasgingBrokerTopicsMutex.lock ();

    map<string, WaveMessagingBrokerTopic *>::iterator element    = m_waveMesasgingBrokerTopics.find (waveMessagingBrokerTopicName);
    map<string, WaveMessagingBrokerTopic *>::iterator endElement = m_waveMesasgingBrokerTopics.end  ();

    if (element != endElement)
    {
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        WaveMessagingBrokerTopic *pWaveMessagingBrokerTopic = new WaveMessagingBrokerTopic (waveMessagingBrokerTopicName);

        prismAssert (NULL != pWaveMessagingBrokerTopic, __FILE__, __LINE__);

        m_waveMesasgingBrokerTopics[waveMessagingBrokerTopicName] = pWaveMessagingBrokerTopic;

        pWaveMessagingBrokerTopic->addSubscription (clientUniqueString);
    }

    m_waveMesasgingBrokerTopicsMutex.unlock ();
}

void WaveMessagingBrokerTopicRepository::addWaveMessagingBrokerTopicIfNotKnown (const string &waveMessagingBrokerTopicName, const string &clientUniqueString)
{
    WaveMessagingBrokerTopic *pWaveMessagingBrokerTopic = NULL;

    m_waveMesasgingBrokerTopicsMutex.lock ();

    map<string, WaveMessagingBrokerTopic *>::iterator element    = m_waveMesasgingBrokerTopics.find (waveMessagingBrokerTopicName);
    map<string, WaveMessagingBrokerTopic *>::iterator endElement = m_waveMesasgingBrokerTopics.end  ();

    if (element != endElement)
    {
        pWaveMessagingBrokerTopic = element->second;
    }
    else
    {
        pWaveMessagingBrokerTopic = new WaveMessagingBrokerTopic (waveMessagingBrokerTopicName);

        prismAssert (NULL != pWaveMessagingBrokerTopic, __FILE__, __LINE__);

        m_waveMesasgingBrokerTopics[waveMessagingBrokerTopicName] = pWaveMessagingBrokerTopic;
    }

    prismAssert (NULL != pWaveMessagingBrokerTopic, __FILE__, __LINE__);

    pWaveMessagingBrokerTopic->addSubscription (clientUniqueString);

    m_waveMesasgingBrokerTopicsMutex.unlock ();
}

void WaveMessagingBrokerTopicRepository::removeWaveMessagingBrokerTopic (const string &waveMessagingBrokerTopicName, const string &clientUniqueString)
{
    m_waveMesasgingBrokerTopicsMutex.lock ();

    map<string, WaveMessagingBrokerTopic *>::iterator element    = m_waveMesasgingBrokerTopics.find (waveMessagingBrokerTopicName);
    map<string, WaveMessagingBrokerTopic *>::iterator endElement = m_waveMesasgingBrokerTopics.end  ();

    if (element != endElement)
    {
        WaveMessagingBrokerTopic *pWaveMessagingBrokerTopic = element->second;

        prismAssert (NULL != pWaveMessagingBrokerTopic, __FILE__, __LINE__);

        pWaveMessagingBrokerTopic->removeSubscription (clientUniqueString);

        if (0 == (pWaveMessagingBrokerTopic->getNumberOfSubscribedClients ()))
        {
            m_waveMesasgingBrokerTopics.erase (element);

            delete pWaveMessagingBrokerTopic;
        }
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    m_waveMesasgingBrokerTopicsMutex.unlock ();
}

void WaveMessagingBrokerTopicRepository::removeWaveMessagingBrokerTopicIfKnown (const string &waveMessagingBrokerTopicName, const string &clientUniqueString)
{
    m_waveMesasgingBrokerTopicsMutex.lock ();

    map<string, WaveMessagingBrokerTopic *>::iterator element    = m_waveMesasgingBrokerTopics.find (waveMessagingBrokerTopicName);
    map<string, WaveMessagingBrokerTopic *>::iterator endElement = m_waveMesasgingBrokerTopics.end  ();

    if (element != endElement)
    {
        WaveMessagingBrokerTopic *pWaveMessagingBrokerTopic = element->second;

        prismAssert (NULL != pWaveMessagingBrokerTopic, __FILE__, __LINE__);

        pWaveMessagingBrokerTopic->removeSubscription (clientUniqueString);

        if (0 == (pWaveMessagingBrokerTopic->getNumberOfSubscribedClients ()))
        {
            m_waveMesasgingBrokerTopics.erase (element);

            delete pWaveMessagingBrokerTopic;
        }
    }
    else
    {
        // Nothing to do
    }

    m_waveMesasgingBrokerTopicsMutex.unlock ();
}

void WaveMessagingBrokerTopicRepository::getCurrentlySubscribedClientsForTopic (const string &waveMessagingBrokerTopicName, vector<string> &currentlySubscribedClients)
{
    currentlySubscribedClients.clear ();

    m_waveMesasgingBrokerTopicsMutex.lock ();

    map<string, WaveMessagingBrokerTopic *>::iterator element    = m_waveMesasgingBrokerTopics.find (waveMessagingBrokerTopicName);
    map<string, WaveMessagingBrokerTopic *>::iterator endElement = m_waveMesasgingBrokerTopics.end  ();

    if (element != endElement)
    {
        WaveMessagingBrokerTopic *pWaveMessagingBrokerTopic = element->second;

        prismAssert (NULL != pWaveMessagingBrokerTopic, __FILE__, __LINE__);

        pWaveMessagingBrokerTopic->getSubscribedClients (currentlySubscribedClients);
    }
    else
    {
        // Nothing to do
    }

    m_waveMesasgingBrokerTopicsMutex.unlock ();
}

WaveMessagingBrokerTopicRepository *WaveMessagingBrokerTopicRepository::getInstance ()
{
    static PrismMutex                          waveMessagingBrokerTopicRepositoryMutex;
    static WaveMessagingBrokerTopicRepository *pWaveMessagingBrokerTopicRepository      = NULL;

    waveMessagingBrokerTopicRepositoryMutex.lock ();

    if (NULL == pWaveMessagingBrokerTopicRepository)
    {
        pWaveMessagingBrokerTopicRepository = new WaveMessagingBrokerTopicRepository ();

        prismAssert (NULL != pWaveMessagingBrokerTopicRepository, __FILE__, __LINE__);
    }

    waveMessagingBrokerTopicRepositoryMutex.unlock ();

    return (pWaveMessagingBrokerTopicRepository);
}

}
