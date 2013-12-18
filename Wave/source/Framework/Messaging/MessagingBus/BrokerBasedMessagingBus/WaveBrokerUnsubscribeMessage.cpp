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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerUnsubscribeMessage.h"

namespace WaveNs
{

WaveBrokerUnsubscribeMessage::WaveBrokerUnsubscribeMessage ()
    : WaveBrokerBasedMessage (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_UNSUBSCRIBE)
{
}

WaveBrokerUnsubscribeMessage::WaveBrokerUnsubscribeMessage (const string &topicName)
    : WaveBrokerBasedMessage (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_UNSUBSCRIBE),
      m_topicName            (topicName)
{
}

WaveBrokerUnsubscribeMessage::~WaveBrokerUnsubscribeMessage ()
{
}

void WaveBrokerUnsubscribeMessage::setupAttributesForSerialization ()
{
    WaveBrokerBasedMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_topicName, "topicName"));
}

string WaveBrokerUnsubscribeMessage::getTopicName () const
{
    return (m_topicName);
}

WaveBrokerBasedMessage *WaveBrokerUnsubscribeMessage::createMessageInstance ()
{
    return (new WaveBrokerUnsubscribeMessage ());
}

}
