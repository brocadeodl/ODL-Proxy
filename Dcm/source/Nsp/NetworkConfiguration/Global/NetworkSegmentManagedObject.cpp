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

#include "Nsp/NetworkConfiguration/Global/NetworkSegmentManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"

namespace NspNs
{

NetworkSegmentManagedObject::NetworkSegmentManagedObject (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager)
    : PrismElement           (pNetworkConfigurationObjectManager),
      PrismPersistableObject (NetworkSegmentManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject      (pNetworkConfigurationObjectManager)
{
}

NetworkSegmentManagedObject::~NetworkSegmentManagedObject ()
{
}

string NetworkSegmentManagedObject::getClassName ()
{
    return ("NetworkSegmentManagedObject");
}

void NetworkSegmentManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeManagedObjectVectorComposition<NetworkInterfaceManagedObject> (&m_networkInterfaces, "networkInterfaces", getClassName (), getObjectId (), NetworkInterfaceManagedObject::getClassName ()));
}

void NetworkSegmentManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<NetworkInterfaceManagedObject> (&m_networkInterfaces, "networkInterfaces", getClassName (), getObjectId (), NetworkInterfaceManagedObject::getClassName ()));
}

UI32 NetworkSegmentManagedObject::getNumberOfNetworkInterfaces () const
{
    return (m_networkInterfaces.size ());
}

NetworkInterfaceManagedObject *NetworkSegmentManagedObject::getNetworkInterfaceAtIndex (const UI32 &index)
{
          NetworkInterfaceManagedObject *pNetworkInterfaceManagedObject = NULL;
    const UI32                           numberOfNetworkInterfaces      = getNumberOfNetworkInterfaces ();

    if (index < numberOfNetworkInterfaces)
    {
        pNetworkInterfaceManagedObject = (m_networkInterfaces[index]).operator -> ();
    }

    return (pNetworkInterfaceManagedObject);
}

void NetworkSegmentManagedObject::addNetworkInterface (NetworkInterfaceManagedObject *pNetworkInterfaceManagedObject)
{
    m_networkInterfaces.push_back (WaveManagedObjectPointer<NetworkInterfaceManagedObject> (pNetworkInterfaceManagedObject));
}

void NetworkSegmentManagedObject::getAttributeNamesForHtmlTable (vector<string> &attributeNamesForHtmlTable, const WaveManagedObjectShowType &showType)
{
    attributeNamesForHtmlTable.push_back ("networkInterfaces");
}

void NetworkSegmentManagedObject::getHtmlTableHeaderNames (vector<string> &htmlTableHeaderNames, const WaveManagedObjectShowType &showType)
{
    htmlTableHeaderNames.push_back ("Network Interfaces");
}

void NetworkSegmentManagedObject::getRestHeaderNames (vector<string> &restHeaderNames, const WaveManagedObjectShowType &showType)
{
    restHeaderNames.push_back ("ports");
}

void NetworkSegmentManagedObject::getManagedObjectClassNameForRest (string &managedObjectClassNameForRest) const
{
    managedObjectClassNameForRest = "";
}


}

