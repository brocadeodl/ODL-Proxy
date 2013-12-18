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
 *   Author : vkarnati                                                     *
 **************************************************************************/

#include "Arp/Local/ArpConfigLocalManagedObject.h"
#include "Arp/Local/ArpLocalObjectManager.h"
#include "Arp/Local/ArpTypes.h"
#include "vcs.h"

namespace DcmNs
{

    ArpConfigLocalManagedObject::ArpConfigLocalManagedObject (ArpLocalObjectManager *pArpLocalObjectManager)
        : PrismElement  (pArpLocalObjectManager),
        PrismPersistableObject (ArpConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pArpLocalObjectManager),
        DcmManagedObject (pArpLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pArpLocalObjectManager)
    {
    }

    ArpConfigLocalManagedObject::ArpConfigLocalManagedObject (ArpLocalObjectManager *pArpLocalObjectManager,const UI32 &SystemMaxArp)
        : PrismElement  (pArpLocalObjectManager),
        PrismPersistableObject (ArpConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pArpLocalObjectManager),
        DcmManagedObject (pArpLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pArpLocalObjectManager),
        m_SystemMaxArp    (SystemMaxArp)
    {
    }

    ArpConfigLocalManagedObject::~ArpConfigLocalManagedObject ()
    {
    }

    string  ArpConfigLocalManagedObject::getClassName()
    {
        return ("ArpConfigLocalManagedObject");
    }

    void  ArpConfigLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_SystemMaxArp,"SystemMaxArp", arp_arp));
    }

    void  ArpConfigLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_SystemMaxArp,"SystemMaxArp", arp_arp));
    }

    void  ArpConfigLocalManagedObject::setSystemMaxArp(const UI32 &SystemMaxArp)
    {
        m_SystemMaxArp  =  SystemMaxArp;
    }

    UI32  ArpConfigLocalManagedObject::getSystemMaxArp() const
    {
        return (m_SystemMaxArp);
    }

}
