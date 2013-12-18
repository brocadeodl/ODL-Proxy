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
 *   Author : jvenkata                                                     *
 **************************************************************************/

#include "Interface/InterfaceNodeSpecificLocalManagedObject.h"
#include "Interface/InterfaceNodeSpecificTypes.h"

#include "brocade-rbridge.h"

namespace DcmNs
{

    InterfaceNodeSpecificLocalManagedObject::InterfaceNodeSpecificLocalManagedObject (WaveObjectManager *pWaveObjectManager)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (InterfaceNodeSpecificLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pWaveObjectManager)
    {
    }

    InterfaceNodeSpecificLocalManagedObject::~InterfaceNodeSpecificLocalManagedObject ()
    {
    }

    string  InterfaceNodeSpecificLocalManagedObject::getClassName()
    {
        return ("InterfaceNodeSpecificLocalManagedObject");
    }

    void  InterfaceNodeSpecificLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_nsVlan,"nsVlan", rbridge_ns_vlan));
        addPersistableAttribute (new AttributeSI32(&m_nsEthernet,"nsEthernet", rbridge_ns_ethernet));
    }

    void  InterfaceNodeSpecificLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeSI32(&m_nsVlan,"nsVlan", rbridge_ns_vlan));
        addPersistableAttributeForCreate (new AttributeSI32(&m_nsEthernet,"nsEthernet", rbridge_ns_ethernet));
    }

    void  InterfaceNodeSpecificLocalManagedObject::setNsVlan(const SI32 &nsVlan)
    {
        m_nsVlan  =  nsVlan;
    }

    SI32  InterfaceNodeSpecificLocalManagedObject::getNsVlan() const
    {
        return (m_nsVlan);
    }

    void  InterfaceNodeSpecificLocalManagedObject::setNsEthernet(const SI32 &nsEthernet)
    {
        m_nsEthernet  =  nsEthernet;
    }

    SI32  InterfaceNodeSpecificLocalManagedObject::getNsEthernet() const
    {
        return (m_nsEthernet);
    }

}
