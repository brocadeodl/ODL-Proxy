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

#include "Interface/InterfaceClassLocalManagedObject.h"
#include "Interface/InterfaceClassTypes.h"

#include "DcmInterface.h"

namespace DcmNs
{

    InterfaceClassLocalManagedObject::InterfaceClassLocalManagedObject (WaveObjectManager *pWaveObjectManager)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (InterfaceClassLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pWaveObjectManager)
    {
    }

    InterfaceClassLocalManagedObject::~InterfaceClassLocalManagedObject ()
    {
    }

    string  InterfaceClassLocalManagedObject::getClassName()
    {
        return ("InterfaceClassLocalManagedObject");
    }

    void  InterfaceClassLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_vlanClass,"vlanClass",dcminterface_vlanClass));
        addPersistableAttribute (new AttributeSI32(&m_ethernetClass,"ethernetClass",dcminterface_ethernetClass));
    }

    void  InterfaceClassLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_vlanClass,"vlanClass",dcminterface_vlanClass));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_ethernetClass,"ethernetClass",dcminterface_ethernetClass));
    }

    void  InterfaceClassLocalManagedObject::setVlanClass(const SI32 &vlanClass)
    {
        m_vlanClass  =  vlanClass;
    }

    SI32  InterfaceClassLocalManagedObject::getVlanClass() const
    {
        return (m_vlanClass);
    }

    void  InterfaceClassLocalManagedObject::setEthernetClass(const SI32 &ethernetClass)
    {
        m_ethernetClass  =  ethernetClass;
    }

    SI32  InterfaceClassLocalManagedObject::getEthernetClass() const
    {
        return (m_ethernetClass);
    }

}
