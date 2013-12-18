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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerSubscribeMessage.h"
#include "Framework/Attributes/Attributes.h"

namespace WaveNs
{

WaveBrokerSubscribeMessage::WaveBrokerSubscribeMessage ()
    : WaveBrokerBasedMessage (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_SUBSCRIBE)
{
}

WaveBrokerSubscribeMessage::WaveBrokerSubscribeMessage (const vector<string> &topicNames)
    : WaveBrokerBasedMessage (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_SUBSCRIBE),
      m_topicNames           (topicNames)
{
}

WaveBrokerSubscribeMessage::WaveBrokerSubscribeMessage (const string &topicName)
    : WaveBrokerBasedMessage (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_SUBSCRIBE)
{
    m_topicNames.push_back (topicName);
}

WaveBrokerSubscribeMessage::~WaveBrokerSubscribeMessage ()
{
}

void WaveBrokerSubscribeMessage::setupAttributesForSerialization ()
{
    WaveBrokerBasedMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeStringVector (&m_topicNames, "topicNames"));
}

vector<string> WaveBrokerSubscribeMessage::getTopicNames () const
{
    return (m_topicNames);
}

void WaveBrokerSubscribeMessage::setTopicNames (const vector<string> &topicNames)
{
    m_topicNames = topicNames;
}

void WaveBrokerSubscribeMessage::addTopicNames (const vector<string> &topicNames)
{
    m_topicNames.insert (m_topicNames.end (), topicNames.begin (), topicNames.end ());
}

WaveBrokerBasedMessage *WaveBrokerSubscribeMessage::createMessageInstance ()
{
    return (new WaveBrokerSubscribeMessage ());
}

}
