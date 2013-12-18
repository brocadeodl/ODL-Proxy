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

#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateInterfaceMessage.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManagerTypes.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace NspNs
{

NetworkConfigurationCreateInterfaceMessage::NetworkConfigurationCreateInterfaceMessage ()
    : ManagementInterfaceMessage (NetworkConfigurationObjectManager::getClassName (), NETWORK_CONFIGURATION_CREATE_INTERFACE),
      m_networkId       (0),
      m_networkUuid     (""),
      m_interfaceStatus (NETWORK_INTERFACE_ACTIVE)
{
}

NetworkConfigurationCreateInterfaceMessage::NetworkConfigurationCreateInterfaceMessage (const UI32 &neworkId, const NetworkInterfaceStatus &networkInterfaceStatus)
    : ManagementInterfaceMessage (NetworkConfigurationObjectManager::getClassName (), NETWORK_CONFIGURATION_CREATE_INTERFACE),
      m_networkId       (neworkId),
      m_networkUuid     (""),
      m_interfaceStatus (networkInterfaceStatus)
{
}

NetworkConfigurationCreateInterfaceMessage::NetworkConfigurationCreateInterfaceMessage (const string &neworkUuId, const NetworkInterfaceStatus &networkInterfaceStatus)
    : ManagementInterfaceMessage (NetworkConfigurationObjectManager::getClassName (), NETWORK_CONFIGURATION_CREATE_INTERFACE),
      m_networkId       (0),
      m_networkUuid     (neworkUuId),
      m_interfaceStatus (networkInterfaceStatus)
{
}

NetworkConfigurationCreateInterfaceMessage::~NetworkConfigurationCreateInterfaceMessage()
{
}

void NetworkConfigurationCreateInterfaceMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32                                        (&m_networkId,          "networkId"));
     addSerializableAttribute (new AttributeString                                      (&m_networkUuid,        "networkUuid"));
    addSerializableAttribute  (new AttributeResourceId (reinterpret_cast<ResourceId *>  (&m_interfaceStatus),   "interfaceStatus"));
     addSerializableAttribute (new AttributeString                                      (&m_interfaceUuid,      "interfaceUuid"));
}

UI32 NetworkConfigurationCreateInterfaceMessage::getNetworkId () const
{
    return (m_networkId);
}

void NetworkConfigurationCreateInterfaceMessage::setNetworkId (const UI32 &networkId)
{
    m_networkId = networkId;
}

NetworkInterfaceStatus NetworkConfigurationCreateInterfaceMessage::getInterfaceStatus () const
{
    return (m_interfaceStatus);
}

void NetworkConfigurationCreateInterfaceMessage::setInterfacestatus (const NetworkInterfaceStatus &interfaceStatus)
{
    m_interfaceStatus = interfaceStatus;
}

string NetworkConfigurationCreateInterfaceMessage::getNetworkUuid () const
{
    return (m_networkUuid);
}

void NetworkConfigurationCreateInterfaceMessage::setNetworkUuid (const string &networkUuid)
{
    m_networkUuid = networkUuid;
}

string NetworkConfigurationCreateInterfaceMessage::getInterfaceUuid () const
{
    return (m_interfaceUuid);
}


void NetworkConfigurationCreateInterfaceMessage::setInterfaceUuid (const string &interfaceUuid)
{
    m_interfaceUuid = interfaceUuid;
}

}
