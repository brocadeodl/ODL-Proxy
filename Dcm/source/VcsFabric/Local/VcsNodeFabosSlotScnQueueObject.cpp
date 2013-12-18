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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh                                            *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeFabosSlotScnQueueObject.h"
namespace DcmNs
{

VcsNodeFabosSlotScnQueueObject::VcsNodeFabosSlotScnQueueObject () :
    m_slotNumber (0),
    m_objectType (0),
    m_hwType (0),
    m_hwId (0),
    m_slotState (0),
    m_reason (0),
    m_version (0)
{
}

VcsNodeFabosSlotScnQueueObject::VcsNodeFabosSlotScnQueueObject (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version) :
    m_slotNumber (slotNumber),
    m_objectType (objectType),
    m_hwType (hwType),
    m_hwId (hwId),
    m_slotState (slotState),
    m_reason (reason),
    m_version (version)
{
}

VcsNodeFabosSlotScnQueueObject::VcsNodeFabosSlotScnQueueObject (UI32 &slotNumber, UI32 &hwType, UI32 &slotState) :
    m_slotNumber (slotNumber),
    m_objectType (0),
    m_hwType (hwType),
    m_hwId (0),
    m_slotState (slotState),
    m_reason (0),
    m_version (0)
{
}

VcsNodeFabosSlotScnQueueObject::~VcsNodeFabosSlotScnQueueObject ()
{
}

VcsNodeFabosSlotScnQueueObject & VcsNodeFabosSlotScnQueueObject::operator=(const VcsNodeFabosSlotScnQueueObject &rhs)
{
    m_slotNumber = rhs.m_slotNumber;
    m_hwType = rhs.m_hwType;
    m_objectType = rhs.m_objectType;
    m_hwId = rhs.m_hwId;
    m_slotState = rhs.m_slotState;
    m_reason = rhs.m_reason;
    m_version = rhs.m_version;
    return (*this);
}

UI32 VcsNodeFabosSlotScnQueueObject::getSlotNumber () const
{
    return (m_slotNumber);
}

void VcsNodeFabosSlotScnQueueObject::setSlotNumber (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

UI32 VcsNodeFabosSlotScnQueueObject::getHwType () const
{
    return (m_hwType);
}

void VcsNodeFabosSlotScnQueueObject::setHwType (const UI32 &hwType)
{
    m_hwType = hwType;
}

UI32 VcsNodeFabosSlotScnQueueObject::getObjectType () const
{
    return (m_objectType);
}

void VcsNodeFabosSlotScnQueueObject::setObjectType (const UI32 &objectType)
{
    m_objectType = objectType;
}

UI32 VcsNodeFabosSlotScnQueueObject::getHwId () const
{
    return (m_hwId);
}

void VcsNodeFabosSlotScnQueueObject::setHwId (const UI32 &hwId)
{
    m_hwId = hwId;
}

UI32 VcsNodeFabosSlotScnQueueObject::getSlotState() const
{
    return (m_slotState);
}

void VcsNodeFabosSlotScnQueueObject::setSlotState (const UI32 &slotState)
{
    m_slotState = slotState;
}

UI32 VcsNodeFabosSlotScnQueueObject::getReason() const
{
    return (m_reason);
}

void VcsNodeFabosSlotScnQueueObject::setReason (const UI32 &reason)
{
    m_reason = reason;
}

UI32 VcsNodeFabosSlotScnQueueObject::getVersion() const
{
    return (m_version);
}

void VcsNodeFabosSlotScnQueueObject::setVersion (const UI32 &version)
{
    m_version = version;
}

UI32 VcsNodeFabosSlotScnQueueObject::getSlotScnObjectId   () const
{
    return m_slotScnObjectId;
}

void VcsNodeFabosSlotScnQueueObject::setSlotScnObjectId (const UI32 &slotScnObjectId)
{
    m_slotScnObjectId = slotScnObjectId;
}


}


