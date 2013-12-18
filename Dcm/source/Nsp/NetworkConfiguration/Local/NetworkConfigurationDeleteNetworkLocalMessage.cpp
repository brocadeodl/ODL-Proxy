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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationDeleteNetworkLocalMessage.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationLocalObjectManager.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationLocalObjectManagerTypes.h"

namespace NspNs
{

NetworkConfigurationDeleteNetworkLocalMessage::NetworkConfigurationDeleteNetworkLocalMessage ()
    : PrismMessage  (NetworkConfigurationLocalObjectManager::getPrismServiceId (), NETWORK_CONFIGURATION_LOCAL_DELETE_NETWORK)
{
}

NetworkConfigurationDeleteNetworkLocalMessage::NetworkConfigurationDeleteNetworkLocalMessage (const string& networkName, const UI32 &networkId)
    : PrismMessage  (NetworkConfigurationLocalObjectManager::getPrismServiceId (), NETWORK_CONFIGURATION_LOCAL_DELETE_NETWORK),
      m_networkName (networkName),
      m_networkId   (networkId)
{
}

NetworkConfigurationDeleteNetworkLocalMessage::~NetworkConfigurationDeleteNetworkLocalMessage()
{
}

void NetworkConfigurationDeleteNetworkLocalMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_networkName, "networkName"));
     addSerializableAttribute (new AttributeUI32   (&m_networkId,   "networkId"));
}

string NetworkConfigurationDeleteNetworkLocalMessage::getNetworkName () const
{
    return (m_networkName);
}

void NetworkConfigurationDeleteNetworkLocalMessage::setNetworkName (const string& networkName)
{
    m_networkName = networkName;
}

UI32 NetworkConfigurationDeleteNetworkLocalMessage::getNetworkId () const
{
    return (m_networkId);
}

void NetworkConfigurationDeleteNetworkLocalMessage::setNetworkId (const UI32& networkId)
{
    m_networkId = networkId;
}

}
