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

#include "Nsp/NetworkConfiguration/Global/NetworkInterfaceManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace NspNs
{

NetworkInterfaceManagedObject::NetworkInterfaceManagedObject (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager)
    : PrismElement             (pNetworkConfigurationObjectManager),
      PrismPersistableObject   (NetworkInterfaceManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject        (pNetworkConfigurationObjectManager),
      m_networkInterfaceStatus (NETWORK_INTERFACE_ACTIVE)
{
}

NetworkInterfaceManagedObject::~NetworkInterfaceManagedObject ()
{
}

string NetworkInterfaceManagedObject::getClassName ()
{
    return ("NetworkInterfaceManagedObject");
}

void NetworkInterfaceManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString     (                                &m_uuid,                    "uuid"));
    addPersistableAttribute (new AttributeString     (                                &m_networkUuid,             "networkUuid"));
    addPersistableAttribute (new AttributeResourceId (reinterpret_cast<ResourceId *> (&m_networkInterfaceStatus), "networkInterfaceStatus"));
}

void NetworkInterfaceManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString     (                                &m_uuid,                    "uuid"));
    addPersistableAttributeForCreate (new AttributeString     (                                &m_networkUuid,             "networkUuid"));
    addPersistableAttributeForCreate (new AttributeResourceId (reinterpret_cast<ResourceId *> (&m_networkInterfaceStatus), "networkInterfaceStatus"));
}
string NetworkInterfaceManagedObject::getUuid () const
{
    return (m_uuid);
}

void NetworkInterfaceManagedObject::setUuid (const string &uuid)
{
    m_uuid = uuid;
}

string NetworkInterfaceManagedObject::getNetworkUuid () const
{
    return (m_networkUuid);
}

void NetworkInterfaceManagedObject::setNetworkUuid (const string &networkUuid)
{
    m_networkUuid = networkUuid;
}

NetworkInterfaceStatus NetworkInterfaceManagedObject::getNetworkInterfaceStatus () const
{
    return (m_networkInterfaceStatus);
}

void NetworkInterfaceManagedObject::setNetworkInterfaceStatus (const NetworkInterfaceStatus &networkInterfaceStatus)
{
    m_networkInterfaceStatus = networkInterfaceStatus;
}

void NetworkInterfaceManagedObject::getAttributeNamesForHtmlTable (vector<string> &attributeNamesForHtmlTable, const WaveManagedObjectShowType &showType)
{
    attributeNamesForHtmlTable.push_back ("uuid");
    attributeNamesForHtmlTable.push_back ("networkInterfaceStatus");
    attributeNamesForHtmlTable.push_back ("createdTimeStamp");
    attributeNamesForHtmlTable.push_back ("lastModifiedTimeStamp");
}

void NetworkInterfaceManagedObject::getHtmlTableHeaderNames (vector<string> &htmlTableHeaderNames, const WaveManagedObjectShowType &showType)
{
    htmlTableHeaderNames.push_back ("Port UUID");
    htmlTableHeaderNames.push_back ("Interface Status");
    htmlTableHeaderNames.push_back ("Created");
    htmlTableHeaderNames.push_back ("Last Modified");
}

void NetworkInterfaceManagedObject::getRestHeaderNames (vector<string> &restHeaderNames, const WaveManagedObjectShowType &showType)
{
    restHeaderNames.push_back ("id");
    restHeaderNames.push_back ("status");
    restHeaderNames.push_back ("created");
    restHeaderNames.push_back ("lastModified");
}

void NetworkInterfaceManagedObject::getManagedObjectClassNameForRest (string &managedObjectClassNameForRest) const
{
    managedObjectClassNameForRest = "port";
}

}

