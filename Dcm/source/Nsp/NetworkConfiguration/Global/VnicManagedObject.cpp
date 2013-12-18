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

#include "Nsp/NetworkConfiguration/Global/VnicManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/VirtualMachineManagedObject.h"

namespace NspNs
{

VnicManagedObject::VnicManagedObject (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager)
    : PrismElement                  (pNetworkConfigurationObjectManager),
      PrismPersistableObject        (VnicManagedObject::getClassName (), NetworkInterfaceManagedObject::getClassName ()),
      NetworkInterfaceManagedObject (pNetworkConfigurationObjectManager)
{
}

VnicManagedObject::~VnicManagedObject ()
{
}

string VnicManagedObject::getClassName ()
{
    return ("VnicManagedObject");
}

void VnicManagedObject::setupAttributesForPersistence ()
{
    NetworkInterfaceManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeObjectIdAssociation (&m_virtualMachine, "virtualMachine", VirtualMachineManagedObject::getClassName (), true));
}

void VnicManagedObject::setupAttributesForCreate ()
{
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_virtualMachine, "virtualMachine", VirtualMachineManagedObject::getClassName (), true));
}

ObjectId VnicManagedObject::getVirtualMachine () const
{
    return (m_virtualMachine);
}

void VnicManagedObject::setVirtualMachine (const ObjectId& virtualMachine)
{
    m_virtualMachine = virtualMachine;
}

}
