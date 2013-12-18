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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerConnectionObjectManagerTypes.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerConnectionObjectManager.h"

namespace WaveNs
{

WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage::WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage (const vector<string> &brokerNames)
    : PrismMessage  (WaveMessagingBrokerConnectionObjectManager::getPrismServiceId (), WAVE_MESSAGING_BROKER_CONNECTION_OBJECT_MANAGER_CANCEL_REQUEST_TO_TRY_FOR_CONNECTION),
      m_brokerNames (brokerNames)
{
}

WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage::WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage (const string &brokerName)
    : PrismMessage  (WaveMessagingBrokerConnectionObjectManager::getPrismServiceId (), WAVE_MESSAGING_BROKER_CONNECTION_OBJECT_MANAGER_CANCEL_REQUEST_TO_TRY_FOR_CONNECTION)
{
    m_brokerNames.push_back (brokerName);
}

WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage::~WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage ()
{
}

vector<string> WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage::getBrokerNames () const
{
    return (m_brokerNames);
}

void WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage::setBrokerNames (const vector<string> &brokerNames)
{
    m_brokerNames = brokerNames;
}

void WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage::addBrokerNames (const vector<string> &brokerNames)
{
    m_brokerNames.insert (m_brokerNames.end (), brokerNames.begin (), brokerNames.end ());
}

void WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage::addBrokerName (const string &brokerName)
{
    m_brokerNames.push_back (brokerName);
}

}
