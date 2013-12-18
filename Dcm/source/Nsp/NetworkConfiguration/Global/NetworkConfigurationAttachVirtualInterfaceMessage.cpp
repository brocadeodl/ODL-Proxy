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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationAttachVirtualInterfaceMessage.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManagerTypes.h"

namespace NspNs
{

NetworkConfigurationAttachVirtualInterfaceMessage::NetworkConfigurationAttachVirtualInterfaceMessage ()
    : ManagementInterfaceMessage (NetworkConfigurationObjectManager::getClassName (), NETWORK_CONFIGURATION_ATTACH_INTERFACE)
{
}

NetworkConfigurationAttachVirtualInterfaceMessage::NetworkConfigurationAttachVirtualInterfaceMessage (const string &networkUuid, const string &interfaceUuid, const MacAddress &macAddress, const string &targetHost)
    : ManagementInterfaceMessage (NetworkConfigurationObjectManager::getClassName (), NETWORK_CONFIGURATION_ATTACH_INTERFACE),
      m_networkUuid              (networkUuid),
      m_interfaceUuid            (interfaceUuid),
      m_macAddress               (macAddress),
      m_targetHost               (targetHost)
{
}

NetworkConfigurationAttachVirtualInterfaceMessage::~NetworkConfigurationAttachVirtualInterfaceMessage ()
{
}

void NetworkConfigurationAttachVirtualInterfaceMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString     (&m_networkUuid,     "networkUuid"));
     addSerializableAttribute (new AttributeString     (&m_interfaceUuid,   "interfaceUuid"));
     addSerializableAttribute (new AttributeMacAddress (&m_macAddress,      "macAddress"));
     addSerializableAttribute (new AttributeString     (&m_targetHost,      "targetHost"));
}

string NetworkConfigurationAttachVirtualInterfaceMessage::getNetworkUuid () const
{
    return (m_networkUuid);
}

void NetworkConfigurationAttachVirtualInterfaceMessage::setNetworkUuid (const string &networkUuid)
{
    m_networkUuid = networkUuid;
}

string NetworkConfigurationAttachVirtualInterfaceMessage::getInterfaceUuid () const
{
    return (m_interfaceUuid);
}

void NetworkConfigurationAttachVirtualInterfaceMessage::setInterfaceUuid (const string &interfaceUuid)
{
    m_interfaceUuid = interfaceUuid;
}

MacAddress NetworkConfigurationAttachVirtualInterfaceMessage::getMacAddress () const
{
    return (m_macAddress);
}

void NetworkConfigurationAttachVirtualInterfaceMessage::setMacAddress (const MacAddress &macAddress)
{
    m_macAddress = macAddress;
}

string NetworkConfigurationAttachVirtualInterfaceMessage::getTargetHost () const
{
    return (m_targetHost);
}

void NetworkConfigurationAttachVirtualInterfaceMessage::setTargetHost (const string &targetHost)
{
    m_targetHost = targetHost;
}

}
