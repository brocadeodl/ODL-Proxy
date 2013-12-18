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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : jvenkata                                                     *
 **************************************************************************/

#include "Interface/InterfacePriorityLocalManagedObject.h"
#include "Interface/InterfaceClassLocalManagedObject.h"
#include "Interface/InterfacePriorityTypes.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "DcmInterface.h"

namespace DcmNs
{

    InterfacePriorityLocalManagedObject::InterfacePriorityLocalManagedObject (WaveObjectManager *pWaveObjectManager)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (InterfacePriorityLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pWaveObjectManager)
    {
    }

    InterfacePriorityLocalManagedObject::~InterfacePriorityLocalManagedObject ()
    {
    }

    string  InterfacePriorityLocalManagedObject::getClassName()
    {
        return ("InterfacePriorityLocalManagedObject");
    }

    void  InterfacePriorityLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_vlanPriority,"vlanPriority",dcminterface_vlanPriority));
        addPersistableAttribute (new AttributeSI32(&m_ethernetPriority,"ethernetPriority",dcminterface_ethernetPriority));
        addPersistableAttribute (new AttributeManagedObjectComposition<InterfaceClassLocalManagedObject> (&m_child, "child", InterfaceClassLocalManagedObject::getClassName ()));
    }

    void  InterfacePriorityLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_vlanPriority,"vlanPriority",dcminterface_vlanPriority));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_ethernetPriority,"ethernetPriority",dcminterface_ethernetPriority));
        addPersistableAttributeForCreate (new AttributeManagedObjectComposition<InterfaceClassLocalManagedObject> (&m_child, "child", InterfaceClassLocalManagedObject::getClassName ()));
    }

    void  InterfacePriorityLocalManagedObject::setVlanPriority(const SI32 &vlanPriority)
    {
        m_vlanPriority  =  vlanPriority;
    }

    SI32  InterfacePriorityLocalManagedObject::getVlanPriority() const
    {
        return (m_vlanPriority);
    }

    void  InterfacePriorityLocalManagedObject::setEthernetPriority(const SI32 &ethernetPriority)
    {
        m_ethernetPriority  =  ethernetPriority;
    }

    SI32  InterfacePriorityLocalManagedObject::getEthernetPriority() const
    {
        return (m_ethernetPriority);
    }

	WaveManagedObjectPointer<InterfaceClassLocalManagedObject> InterfacePriorityLocalManagedObject::getChild () const
    {
        return (m_child);
    }

    void InterfacePriorityLocalManagedObject::setChild (WaveManagedObjectPointer<InterfaceClassLocalManagedObject> child)
    {
        m_child = child;
    }

}
