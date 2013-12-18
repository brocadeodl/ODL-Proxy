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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "PhysicalInterface/Local/PhysicalInterfaceLocalManagedObject.h"

namespace DcmNs
{

PhysicalInterfaceLocalManagedObject::PhysicalInterfaceLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (PhysicalInterfaceLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
}

PhysicalInterfaceLocalManagedObject::PhysicalInterfaceLocalManagedObject (WaveObjectManager *pWaveObjectManager, const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (PhysicalInterfaceLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager),
      m_ifIndex                  (ifIndex),
      m_mtu                      (mtu),
      m_description              (description)
{
    setName (name);
}

PhysicalInterfaceLocalManagedObject::~PhysicalInterfaceLocalManagedObject ()
{
}

string PhysicalInterfaceLocalManagedObject::getClassName ()
{
    return ("PhysicalInterfaceLocalManagedObject");
}

void PhysicalInterfaceLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32   (&m_ifIndex,     "ifIndex"));
    addPersistableAttribute (new AttributeUI32   (&m_mtu,         "mtu"));
    addPersistableAttribute (new AttributeString (&m_description, "description"));
}

void PhysicalInterfaceLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI32   (&m_ifIndex,     "ifIndex"));
    addPersistableAttributeForCreate (new AttributeUI32   (&m_mtu,         "mtu"));
    addPersistableAttributeForCreate (new AttributeString (&m_description, "description"));
}

UI32 PhysicalInterfaceLocalManagedObject::getIfIndex () const
{
    return (m_ifIndex);
}

void PhysicalInterfaceLocalManagedObject::setIfIndex (const UI32 &ifIndex)
{
    m_ifIndex = ifIndex;
}

UI32 PhysicalInterfaceLocalManagedObject::getMtu () const
{
    return (m_mtu);
}

void PhysicalInterfaceLocalManagedObject::setMtu (const UI32 &mtu)
{
    m_mtu = mtu;
}

string PhysicalInterfaceLocalManagedObject::getDescription () const
{
    return (m_description);
}

void PhysicalInterfaceLocalManagedObject::setDescription (const string &desc)
{
    m_description = desc;
}

}
