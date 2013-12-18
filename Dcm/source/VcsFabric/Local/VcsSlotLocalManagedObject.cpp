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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.                               *
 *   All rights reserved.                                                                                                   *
 *   Author : Jitendra Singh                                                                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsSlotLocalManagedObject.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "vcs.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeEnum.h"

using namespace WaveNs;

namespace DcmNs
{

// Dummy Constructor: This should not be called
VcsSlotLocalManagedObject::VcsSlotLocalManagedObject ()
    : PrismElement               (*this),
      PrismPersistableObject     (VcsSlotLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (*this),
      DcmManagedObject           (*this),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (*this),
      WaveSlotLocalManagedObjectBase (this),
      m_linecardType             (VCS_NODE_SLOT_LINECARD_TYPE_UNKNOWN),
      m_linecardMode             (VCS_NODE_SLOT_LINECARD_MODE_DEFAULT)
{
}

VcsSlotLocalManagedObject::VcsSlotLocalManagedObject (VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager)
    : PrismElement               (pVcsFabricLocalObjectManager),
      PrismPersistableObject     (VcsSlotLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pVcsFabricLocalObjectManager),
      DcmManagedObject           (pVcsFabricLocalObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pVcsFabricLocalObjectManager),
      WaveSlotLocalManagedObjectBase (this),
      m_linecardType             (VCS_NODE_SLOT_LINECARD_TYPE_UNKNOWN),
      m_linecardMode             (VCS_NODE_SLOT_LINECARD_MODE_DEFAULT)
{
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
}

VcsSlotLocalManagedObject::VcsSlotLocalManagedObject (VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager, string &linecardName, UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version)
    : PrismElement               (pVcsFabricLocalObjectManager),
      PrismPersistableObject     (VcsSlotLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pVcsFabricLocalObjectManager),
      DcmManagedObject           (pVcsFabricLocalObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pVcsFabricLocalObjectManager),
      WaveSlotLocalManagedObjectBase (this, slotNumber, objectType, hwType, hwId, slotState, reason, version),
      m_linecardType             (VCS_NODE_SLOT_LINECARD_TYPE_UNKNOWN),
      m_linecardMode             (VCS_NODE_SLOT_LINECARD_MODE_DEFAULT)
{
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
 
    setName (linecardName);
}

VcsSlotLocalManagedObject::~VcsSlotLocalManagedObject ()
{
}

string VcsSlotLocalManagedObject::getClassName ()
{
    return ("VcsSlotLocalManagedObject");
}

void VcsSlotLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    WaveSlotLocalManagedObjectBase::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeEnum((UI32 * )&m_linecardType,"linecardType", linecard_management_linecardType));
//    addPersistableAttribute (new AttributeEnum((UI32 * )&m_linecardMode,"linecardMode", linecard_management_mode));
    addPersistableAttribute (new AttributeEnum((UI32 * )&m_linecardMode,"linecardMode"));

    setUserTagForAttribute("name", linecard_management_linecardName);
}

void VcsSlotLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    WaveSlotLocalManagedObjectBase::setupAttributesForCreate ();

    addPersistableAttributeForCreate  (new AttributeEnum((UI32 * )&m_linecardType,"linecardType", linecard_management_linecardType));
//    addPersistableAttributeForCreate  (new AttributeEnum((UI32 * )&m_linecardMode,"linecardMode", linecard_management_mode));
    addPersistableAttributeForCreate  (new AttributeEnum((UI32 * )&m_linecardMode,"linecardMode"));

    vector<string > keyList;
    keyList.push_back ("name");
    setUserDefinedKeyCombination (keyList);
}

UI32 VcsSlotLocalManagedObject::getLinecardType () const
{
    return (m_linecardType);
}

void VcsSlotLocalManagedObject::setLinecardType (const UI32 &linecardType)
{
    m_linecardType = linecardType;
}

UI32 VcsSlotLocalManagedObject::getLinecardMode () const
{
    return (m_linecardMode);
}

void VcsSlotLocalManagedObject::setLinecardMode (const UI32 &linecardMode)
{
    m_linecardMode = linecardMode;
}

void VcsSlotLocalManagedObject::setWaveNodeObjectId (const ObjectId &waveNodeObjectId)
{
    setOwnerWaveNodeObjectId (waveNodeObjectId);
}

}
