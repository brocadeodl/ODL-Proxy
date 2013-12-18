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

#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationAttachVirtualInterfaceLocalMessage.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationLocalObjectManager.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationLocalObjectManagerTypes.h"

namespace NspNs
{

NetworkConfigurationAttachVirtualInterfaceLocalMessage::NetworkConfigurationAttachVirtualInterfaceLocalMessage ()
    : PrismMessage  (NetworkConfigurationLocalObjectManager::getPrismServiceId (), NETWORK_CONFIGURATION_LOCAL_ATTACH_VIRTUAL_INTERFACE)
{
}

NetworkConfigurationAttachVirtualInterfaceLocalMessage::NetworkConfigurationAttachVirtualInterfaceLocalMessage (const string& networkName, const UI32 &networkId, const MacAddress &macAddress, const string &targetHost)
    : PrismMessage  (NetworkConfigurationLocalObjectManager::getPrismServiceId (), NETWORK_CONFIGURATION_LOCAL_ATTACH_VIRTUAL_INTERFACE),
      m_networkName (networkName),
      m_networkId   (networkId),
      m_macAddress  (macAddress),
      m_targetHost  (targetHost)
{
}

NetworkConfigurationAttachVirtualInterfaceLocalMessage::~NetworkConfigurationAttachVirtualInterfaceLocalMessage()
{
}

void NetworkConfigurationAttachVirtualInterfaceLocalMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString     (&m_networkName,     "networkName"));
     addSerializableAttribute (new AttributeUI32       (&m_networkId,       "networkId"));
     addSerializableAttribute (new AttributeMacAddress (&m_macAddress,      "macAddress"));
     addSerializableAttribute (new AttributeString     (&m_targetHost,      "targetHost"));
}

string NetworkConfigurationAttachVirtualInterfaceLocalMessage::getNetworkName () const
{
    return (m_networkName);
}

void NetworkConfigurationAttachVirtualInterfaceLocalMessage::setNetworkName (const string& networkName)
{
    m_networkName = networkName;
}

UI32 NetworkConfigurationAttachVirtualInterfaceLocalMessage::getNetworkId () const
{
    return (m_networkId);
}

void NetworkConfigurationAttachVirtualInterfaceLocalMessage::setNetworkId (const UI32& networkId)
{
    m_networkId = networkId;
}

MacAddress NetworkConfigurationAttachVirtualInterfaceLocalMessage::getMacAddress () const
{
    return (m_macAddress);
}

void NetworkConfigurationAttachVirtualInterfaceLocalMessage::setMacAddress (const MacAddress &macAddress)
{
    m_macAddress = macAddress;
}

string NetworkConfigurationAttachVirtualInterfaceLocalMessage::getTargetHost () const
{
    return (m_targetHost);
}

void NetworkConfigurationAttachVirtualInterfaceLocalMessage::setTargetHost (const string &targetHost)
{
    m_targetHost = targetHost;
}

}
