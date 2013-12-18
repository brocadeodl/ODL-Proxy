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

#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateNetworkMessage.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManagerTypes.h"

namespace NspNs
{

NetworkConfigurationCreateNetworkMessage::NetworkConfigurationCreateNetworkMessage ()
    : ManagementInterfaceMessage (NetworkConfigurationObjectManager::getClassName (), NETWORK_CONFIGURATION_CREATE_NETWORK),
      m_networkId                (0)
{
}

NetworkConfigurationCreateNetworkMessage::NetworkConfigurationCreateNetworkMessage (const string& networkName)
    : ManagementInterfaceMessage (NetworkConfigurationObjectManager::getClassName (), NETWORK_CONFIGURATION_CREATE_NETWORK),
      m_networkName              (networkName),
      m_networkId                (0)
{
}

NetworkConfigurationCreateNetworkMessage::NetworkConfigurationCreateNetworkMessage (const string& networkName, const UI32 &networkId)
    : ManagementInterfaceMessage (NetworkConfigurationObjectManager::getClassName (), NETWORK_CONFIGURATION_CREATE_NETWORK),
      m_networkName              (networkName),
      m_networkId                (networkId)
{
}

NetworkConfigurationCreateNetworkMessage::~NetworkConfigurationCreateNetworkMessage()
{
}

void NetworkConfigurationCreateNetworkMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_networkName,     "networkName"));
     addSerializableAttribute (new AttributeUI32   (&m_networkId,       "networkId"));
     addSerializableAttribute (new AttributeString (&m_networkUuid,     "networkUuid"));
}

string NetworkConfigurationCreateNetworkMessage::getNetworkName () const
{
    return (m_networkName);
}

void NetworkConfigurationCreateNetworkMessage::setNetworkName (const string& networkName)
{
    m_networkName = networkName;
}

UI32 NetworkConfigurationCreateNetworkMessage::getNetworkId () const
{
    return (m_networkId);
}

void NetworkConfigurationCreateNetworkMessage::setNetworkId (const UI32 &networkId)
{
    m_networkId = networkId;
}

string NetworkConfigurationCreateNetworkMessage::getNetworkUuid () const
{
    return (m_networkUuid);
}

void NetworkConfigurationCreateNetworkMessage::setNetworkUuid (const string &networkUuid)
{
    m_networkUuid = networkUuid;
}

}
