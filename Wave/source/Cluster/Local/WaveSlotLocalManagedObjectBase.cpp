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
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "Cluster/Local/WaveSlotLocalManagedObjectBase.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

// Dummy Constructor: This should not be called
WaveSlotLocalManagedObjectBase::WaveSlotLocalManagedObjectBase ()
{
} 

WaveSlotLocalManagedObjectBase::WaveSlotLocalManagedObjectBase (WaveManagedObject *pWaveManagedObject)
    : m_pWaveManagedObject (pWaveManagedObject),
      m_slotNumber         (0),
      m_objectType         (0),
      m_hwType             (0),
      m_hwId               (0),
      m_slotState          (0),
      m_reason             (0),
      m_version            (0)
{
}

WaveSlotLocalManagedObjectBase::WaveSlotLocalManagedObjectBase (WaveManagedObject *pWaveManagedObject, UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version)
    : m_pWaveManagedObject (pWaveManagedObject),
      m_slotNumber         (slotNumber),
      m_objectType         (objectType),
      m_hwType             (hwType),
      m_hwId               (hwId),
      m_slotState          (slotState),
      m_reason             (reason),
      m_version            (version)
{
}

WaveSlotLocalManagedObjectBase::~WaveSlotLocalManagedObjectBase ()
{
}

void WaveSlotLocalManagedObjectBase::setupAttributesForPersistence ()
{
    m_pWaveManagedObject->addPersistableAttribute (new AttributeUI32 (&m_slotNumber, "slotNumber"));
    m_pWaveManagedObject->addPersistableAttribute (new AttributeUI32 (&m_objectType, "objectType"));
    m_pWaveManagedObject->addPersistableAttribute (new AttributeUI32 (&m_hwType, "hwType"));
    m_pWaveManagedObject->addPersistableAttribute (new AttributeUI32 (&m_hwId, "hwId"));
    m_pWaveManagedObject->addPersistableAttribute (new AttributeUI32 (&m_slotState, "slotState"));
    m_pWaveManagedObject->addPersistableAttribute (new AttributeUI32 (&m_reason, "reason"));
    m_pWaveManagedObject->addPersistableAttribute (new AttributeUI32 (&m_version, "version"));
}

void WaveSlotLocalManagedObjectBase::setupAttributesForCreate ()
{
    m_pWaveManagedObject->addPersistableAttributeForCreate (new AttributeUI32 (&m_slotNumber, "slotNumber"));
    m_pWaveManagedObject->addPersistableAttributeForCreate (new AttributeUI32 (&m_objectType, "objectType"));
    m_pWaveManagedObject->addPersistableAttributeForCreate (new AttributeUI32 (&m_hwType, "hwType"));
    m_pWaveManagedObject->addPersistableAttributeForCreate (new AttributeUI32 (&m_hwId, "hwId"));
    m_pWaveManagedObject->addPersistableAttributeForCreate (new AttributeUI32 (&m_slotState, "slotState"));
    m_pWaveManagedObject->addPersistableAttributeForCreate (new AttributeUI32 (&m_reason, "reason"));
    m_pWaveManagedObject->addPersistableAttributeForCreate (new AttributeUI32 (&m_version, "version"));
}

string WaveSlotLocalManagedObjectBase::getClassName ()
{
    return ("WaveSlotLocalManagedObjectBase");
}

UI32 WaveSlotLocalManagedObjectBase::getSlotNumber ()
{
    return (m_slotNumber);
}

void WaveSlotLocalManagedObjectBase::setSlotNumber (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

UI32 WaveSlotLocalManagedObjectBase::getObjectType ()
{
    return (m_objectType);
}

void WaveSlotLocalManagedObjectBase::setObjectType (const UI32 &objectType)
{
    m_objectType = objectType;
}

UI32 WaveSlotLocalManagedObjectBase::getReason ()
{
    return (m_reason);
}

void WaveSlotLocalManagedObjectBase::setReason (const UI32 &reason)
{
    m_reason = reason;
}

UI32 WaveSlotLocalManagedObjectBase::getHwType ()
{
    return (m_hwType);
}

void WaveSlotLocalManagedObjectBase::setHwType (const UI32 &hwType)
{
    m_hwType = hwType;
}

UI32 WaveSlotLocalManagedObjectBase::getHwId ()
{
    return (m_hwId);
}

void WaveSlotLocalManagedObjectBase::setHwId (const UI32 &hwId)
{
    m_hwId = hwId;
}

UI32 WaveSlotLocalManagedObjectBase::getVersion ()
{
    return (m_version);
}

void WaveSlotLocalManagedObjectBase::setVersion (const UI32 &version)
{
    m_version = version;
}

UI32 WaveSlotLocalManagedObjectBase::getSlotState ()
{
    return (m_slotState);
}

void WaveSlotLocalManagedObjectBase::setSlotState (const UI32 &slotState)
{
    m_slotState = slotState;
}

}
