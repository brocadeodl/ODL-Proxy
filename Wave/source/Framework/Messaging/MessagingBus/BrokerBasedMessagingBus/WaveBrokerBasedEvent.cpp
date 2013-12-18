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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedEvent.h"

namespace WaveNs
{

WaveBrokerBasedEvent::WaveBrokerBasedEvent (const string &topic)
    : SerializableObject (),
      m_topic            (topic)
{
}

WaveBrokerBasedEvent::~WaveBrokerBasedEvent ()
{
}

string WaveBrokerBasedEvent::getTopic () const
{
    return (m_topic);
}

void WaveBrokerBasedEvent::setTopic (const string &topic)
{
    m_topic = topic;
}

void WaveBrokerBasedEvent::setupAttributesForSerialization ()
{
    SerializableObject::setupAttributesForSerialization ();
}

}
