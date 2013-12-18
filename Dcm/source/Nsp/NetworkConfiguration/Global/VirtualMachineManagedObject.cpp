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

#include "Nsp/NetworkConfiguration/Global/VirtualMachineManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/VnicManagedObject.h"

namespace NspNs
{

VirtualMachineManagedObject::VirtualMachineManagedObject (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager)
    : PrismElement           (pNetworkConfigurationObjectManager),
      PrismPersistableObject (VirtualMachineManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject      (pNetworkConfigurationObjectManager)
{
}

VirtualMachineManagedObject::~VirtualMachineManagedObject ()
{
}

string VirtualMachineManagedObject::getClassName ()
{
    return ("VirtualMachineManagedObject");
}

void VirtualMachineManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeObjectIdAssociation (&m_virtualNetworkInterface, "virtualNetworkInterface", VnicManagedObject::getClassName (), true));
    addPersistableAttribute (new AttributeMacAddress          (&m_macAddress,              "macAddress"));
    addPersistableAttribute (new AttributeString              (&m_targetHost,              "targetHost"));
}

void VirtualMachineManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_virtualNetworkInterface, "virtualNetworkInterface", VnicManagedObject::getClassName (), true));
    addPersistableAttributeForCreate (new AttributeMacAddress          (&m_macAddress,              "macAddress"));
    addPersistableAttributeForCreate (new AttributeString              (&m_targetHost,              "targetHost"));
}

ObjectId VirtualMachineManagedObject::getVirtualNetowrkInterface () const
{
    return (m_virtualNetworkInterface);
}

void VirtualMachineManagedObject::setVirtualNetowrkInterface (const ObjectId& virtualNetworkInterface)
{
    m_virtualNetworkInterface = virtualNetworkInterface;
}

MacAddress VirtualMachineManagedObject::getMacAddress () const
{
    return (m_macAddress);
}

void VirtualMachineManagedObject::setMacAddress (const MacAddress &macAddress)
{
    m_macAddress = macAddress;
}

string VirtualMachineManagedObject::getTargetHost () const
{
    return (m_targetHost);
}

void VirtualMachineManagedObject::setTargetHost (const string &targetHost)
{
    m_targetHost = targetHost;
}

}
