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

#include "Nsp/NetworkConfiguration/Global/NetworkManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"

namespace NspNs
{

NetworkManagedObject::NetworkManagedObject (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager)
    : PrismElement             (pNetworkConfigurationObjectManager),
      PrismPersistableObject   (NetworkManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject        (pNetworkConfigurationObjectManager),
      m_networkId              (0),
      m_numberOfAvailablePorts (32 * 1024),
      m_numberOfAttachedPorts  (0)
{
}

NetworkManagedObject::~NetworkManagedObject ()
{
}

string NetworkManagedObject::getClassName ()
{
    return ("NetworkManagedObject");
}

void NetworkManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<NetworkSegmentManagedObject> (&m_networkSegments,        "networkSegments", getClassName (), getObjectId (), NetworkSegmentManagedObject::getClassName ()));
    addPersistableAttributeForCreate (new AttributeUI32                                                        (&m_networkId,              "networkId"));
    addPersistableAttributeForCreate (new AttributeUI32                                                        (&m_numberOfAvailablePorts, "numberOfAvailablePorts"));
    addPersistableAttributeForCreate (new AttributeUI32                                                        (&m_numberOfAttachedPorts,  "numberOfAttachedPorts"));
    addPersistableAttributeForCreate (new AttributeString                                                      (&m_uuid,                   "uuid"));
}

void NetworkManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence();

    addPersistableAttribute (new AttributeManagedObjectVectorComposition<NetworkSegmentManagedObject> (&m_networkSegments,        "networkSegments", getClassName (), getObjectId (), NetworkSegmentManagedObject::getClassName ()));
    addPersistableAttribute (new AttributeUI32                                                        (&m_networkId,              "networkId"));
    addPersistableAttribute (new AttributeUI32                                                        (&m_numberOfAvailablePorts, "numberOfAvailablePorts"));
    addPersistableAttribute (new AttributeUI32                                                        (&m_numberOfAttachedPorts,  "numberOfAttachedPorts"));
    addPersistableAttribute (new AttributeString                                                      (&m_uuid,                   "uuid"));
}

UI32 NetworkManagedObject::getNetworkId () const
{
    return (m_networkId);
}

void NetworkManagedObject::setNetworkId (const UI32 &networkId)
{
    m_networkId = networkId;
}

void NetworkManagedObject::getAttributeNamesForHtmlTable (vector<string> &attributeNamesForHtmlTable, const WaveManagedObjectShowType &showType)
{
    attributeNamesForHtmlTable.push_back ("uuid");
    attributeNamesForHtmlTable.push_back ("networkId");
    attributeNamesForHtmlTable.push_back ("name");
    attributeNamesForHtmlTable.push_back ("createdTimeStamp");
    attributeNamesForHtmlTable.push_back ("lastModifiedTimeStamp");
    attributeNamesForHtmlTable.push_back ("numberOfAvailablePorts");
    attributeNamesForHtmlTable.push_back ("numberOfAttachedPorts");
    attributeNamesForHtmlTable.push_back ("networkSegments");
}

void NetworkManagedObject::getHtmlTableHeaderNames (vector<string> &htmlTableHeaderNames, const WaveManagedObjectShowType &showType)
{
    htmlTableHeaderNames.push_back ("Network UUID");
    htmlTableHeaderNames.push_back ("VLAN ID");
    htmlTableHeaderNames.push_back ("Network Name");
    htmlTableHeaderNames.push_back ("Created");
    htmlTableHeaderNames.push_back ("Last Modified");
    htmlTableHeaderNames.push_back ("Available Port Count");
    htmlTableHeaderNames.push_back ("Attached Port Count");
    htmlTableHeaderNames.push_back ("Network Segments");
}

void NetworkManagedObject::getRestHeaderNames (vector<string> &restHeaderNames, const WaveManagedObjectShowType &showType)
{
    restHeaderNames.push_back ("id");
    restHeaderNames.push_back ("vlan");
    restHeaderNames.push_back ("name");
    restHeaderNames.push_back ("created");
    restHeaderNames.push_back ("lastModified");
    restHeaderNames.push_back ("numberOfAvailablePorts");
    restHeaderNames.push_back ("numberOfAttachedPorts");
    restHeaderNames.push_back ("");
}

void NetworkManagedObject::getManagedObjectClassNameForRest (string &managedObjectClassNameForRest) const
{
    managedObjectClassNameForRest = "network";
}

void NetworkManagedObject::addNetworkSegment (NetworkSegmentManagedObject *pNetworkSegmentManagedObject)
{
    prismAssert (NULL != pNetworkSegmentManagedObject, __FILE__, __LINE__);

    m_networkSegments.push_back (WaveManagedObjectPointer<NetworkSegmentManagedObject> (pNetworkSegmentManagedObject));
}

NetworkSegmentManagedObject *NetworkManagedObject::getNetworkSegementAtIndex (const UI32 &index)
{
    NetworkSegmentManagedObject *pNetworkSegmentManagedObject = NULL;
    UI32                         numberOfNetworkSegments      = m_networkSegments.size ();

    if (index < numberOfNetworkSegments)
    {
        pNetworkSegmentManagedObject = (m_networkSegments[index]).operator -> ();
    }

    return (pNetworkSegmentManagedObject);
}

string NetworkManagedObject::getUuid () const
{
    return (m_uuid);
}

void NetworkManagedObject::setUuid (const string &uuid)
{
    m_uuid = uuid;
}

}
