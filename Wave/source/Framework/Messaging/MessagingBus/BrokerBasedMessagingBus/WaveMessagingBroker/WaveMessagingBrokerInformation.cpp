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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerInformation.h"

namespace WaveNs
{

WaveMessagingBrokerInformation::WaveMessagingBrokerInformation ()
    : SerializableObject (),
      m_port             (0)
{
}

WaveMessagingBrokerInformation::WaveMessagingBrokerInformation (const string &name, const string &ipAddress, const SI32 &port)
    : SerializableObject (),
      m_name             (name),
      m_ipAddress        (ipAddress),
      m_port             (port)
{
}

WaveMessagingBrokerInformation::~WaveMessagingBrokerInformation ()
{
}

void WaveMessagingBrokerInformation::setupAttributesForSerialization ()
{
    SerializableObject::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_name,      "name"));
    addSerializableAttribute (new AttributeString (&m_ipAddress, "ipAddress"));
    addSerializableAttribute (new AttributeSI32   (&m_port,      "port"));
}

string WaveMessagingBrokerInformation::getName () const
{
    return (m_name);
}

void WaveMessagingBrokerInformation::setName (const string &name)
{
    m_name = name;
}

string WaveMessagingBrokerInformation::getIpAddress () const
{
    return (m_ipAddress);
}

void WaveMessagingBrokerInformation::setIpAddress (const string &ipAddress)
{
    m_ipAddress = ipAddress;
}

SI32 WaveMessagingBrokerInformation::getPort () const
{
    return (m_port);
}

void WaveMessagingBrokerInformation::setPort (const SI32 &port)
{
    m_port = port;
}

}
