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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessageFactory.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

map<string, WaveBrokerPublishMessageInstantiator> WaveBrokerPublishMessageFactory::m_waveBrokerPublishMessageInstatiators;
PrismMutex                                        WaveBrokerPublishMessageFactory::m_waveBrokerPublishMessageInstatiatorsMutex;

WaveBrokerPublishMessageFactory::WaveBrokerPublishMessageFactory ()
{
}

WaveBrokerPublishMessageFactory::~WaveBrokerPublishMessageFactory ()
{
}

void WaveBrokerPublishMessageFactory::registerWaveBrokerPublishMessageInstantiator (const string &topicName, WaveBrokerPublishMessageInstantiator waveBrokerPublishMessageInstantiator)
{
    m_waveBrokerPublishMessageInstatiatorsMutex.lock ();

    map<string, WaveBrokerPublishMessageInstantiator>::iterator element    = m_waveBrokerPublishMessageInstatiators.find (topicName);
    map<string, WaveBrokerPublishMessageInstantiator>::iterator endElement = m_waveBrokerPublishMessageInstatiators.end  ();

    if (endElement == element)
    {
        m_waveBrokerPublishMessageInstatiators[topicName] = waveBrokerPublishMessageInstantiator;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "WaveBrokerPublishMessageFactory::registerWaveBrokerPublishMessageInstantiator : Duplicate Instantiator Registration for " + topicName);
        prismAssert (false, __FILE__, __LINE__);
    }

    m_waveBrokerPublishMessageInstatiatorsMutex.unlock ();
}

WaveBrokerPublishMessage *WaveBrokerPublishMessageFactory::createWaveBrokerPublishMessageInstance (const string &topicName)
{
    WaveBrokerPublishMessage *pWaveBrokerPublishMessage = NULL;

    m_waveBrokerPublishMessageInstatiatorsMutex.lock ();

    map<string, WaveBrokerPublishMessageInstantiator>::iterator element    = m_waveBrokerPublishMessageInstatiators.find (topicName);
    map<string, WaveBrokerPublishMessageInstantiator>::iterator endElement = m_waveBrokerPublishMessageInstatiators.end  ();

    if (endElement != element)
    {
        WaveBrokerPublishMessageInstantiator waveBrokerPublishMessageInstantiator = m_waveBrokerPublishMessageInstatiators[topicName];

        pWaveBrokerPublishMessage = (*waveBrokerPublishMessageInstantiator) ();

        prismAssert (NULL != pWaveBrokerPublishMessage, __FILE__, __LINE__);
    }

    m_waveBrokerPublishMessageInstatiatorsMutex.unlock ();

    return (pWaveBrokerPublishMessage);
}

}
