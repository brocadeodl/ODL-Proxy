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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "Interface/NDeepComposition/InterfaceNDeepCompositionGChildLocalManagedObject.h"
//#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"

#include "interface_with_ndeepcomposition.h"

namespace DcmNs
{

////////////////////////////////////////////////////////////////////////////////////
/// NDeep Composition Grand Child
///////////////////////////////////////////////////////////////////

InterfaceNDeepCompositionGChildLocalManagedObject::InterfaceNDeepCompositionGChildLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (InterfaceNDeepCompositionGChildLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
}

InterfaceNDeepCompositionGChildLocalManagedObject::InterfaceNDeepCompositionGChildLocalManagedObject (WaveObjectManager *pWaveObjectManager, SI32 &id, bool allow, SI32 &xid)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (InterfaceNDeepCompositionGChildLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager),
      m_vlan_id                  (id),
      m_x_id                     (xid)
{
}

InterfaceNDeepCompositionGChildLocalManagedObject::~InterfaceNDeepCompositionGChildLocalManagedObject ()
{
}

string InterfaceNDeepCompositionGChildLocalManagedObject::getClassName ()
{
    return ("InterfaceNDeepCompositionGChildLocalManagedObject");
}

void InterfaceNDeepCompositionGChildLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeSI32          (&m_vlan_id,          "vlan_id",         interface_ndeepcomposition_vlan_id));
    addPersistableAttribute (new AttributeSI32          (&m_x_id,          "x_id",         interface_ndeepcomposition_x_id));
}

void InterfaceNDeepCompositionGChildLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeSI32          (&m_vlan_id,           "vlan_id",           interface_ndeepcomposition_vlan_id));
    addPersistableAttributeForCreate (new AttributeSI32          (&m_x_id,          "x_id",         interface_ndeepcomposition_x_id));

    vector<string > keyName;
    keyName.push_back ("vlan_id");
    setUserDefinedKeyCombination (keyName);
}

SI32 InterfaceNDeepCompositionGChildLocalManagedObject::getId () const
{
    return (m_vlan_id);
}

void InterfaceNDeepCompositionGChildLocalManagedObject::setId (const SI32 &vlan_id)
{
    m_vlan_id = vlan_id;
}

SI32 InterfaceNDeepCompositionGChildLocalManagedObject::getXId () const
{
    return (m_x_id);
}

void InterfaceNDeepCompositionGChildLocalManagedObject::setXId (const SI32 &x_id)
{
    m_x_id = x_id;
}


}
