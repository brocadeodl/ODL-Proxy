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
 *   Author : Jitendra Singh                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosSlotEvents.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

/*************************************************************************
* Slot Online Event
*************************************************************************/

VcsNodeFabosSlotOnlineEvent::VcsNodeFabosSlotOnlineEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SLOT_ONLINE),
    m_slotNumber (0),
    m_objectType (0),
    m_hwType (0),
    m_hwId (0),
    m_slotState (0),
    m_reason (0),
    m_version (0),
    m_eventId (0)
{
}

VcsNodeFabosSlotOnlineEvent::VcsNodeFabosSlotOnlineEvent (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version, UI32 &eventId)
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SLOT_ONLINE),
    m_slotNumber (slotNumber),
    m_objectType (objectType),
    m_hwType (hwType),
    m_hwId (hwId),
    m_slotState (slotState),
    m_reason (reason),
    m_version (version),
    m_eventId (eventId)
{
}

VcsNodeFabosSlotOnlineEvent::~VcsNodeFabosSlotOnlineEvent ()
{
}

void VcsNodeFabosSlotOnlineEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_slotNumber,  "slotNumber"));
     addSerializableAttribute (new AttributeUI32   (&m_objectType,  "objectType"));
     addSerializableAttribute (new AttributeUI32   (&m_hwType,      "hwType"));
     addSerializableAttribute (new AttributeUI32   (&m_hwId,        "hwId"));
     addSerializableAttribute (new AttributeUI32   (&m_slotState,   "slotState"));
     addSerializableAttribute (new AttributeUI32   (&m_reason,      "reason"));
     addSerializableAttribute (new AttributeUI32   (&m_version,     "version"));
     addSerializableAttribute (new AttributeUI32   (&m_eventId,     "eventId"));
}

UI32 VcsNodeFabosSlotOnlineEvent::getSlotNumber () const
{
    return (m_slotNumber);
}

void VcsNodeFabosSlotOnlineEvent::setSlotNumber (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

UI32 VcsNodeFabosSlotOnlineEvent::getHwType () const
{
    return (m_hwType);
}

void VcsNodeFabosSlotOnlineEvent::setHwType (const UI32 &hwType)
{
    m_hwType = hwType;
}

UI32 VcsNodeFabosSlotOnlineEvent::getObjectType () const
{
    return (m_objectType);
}

void VcsNodeFabosSlotOnlineEvent::setObjectType (const UI32 &objectType)
{
    m_objectType = objectType;
}

UI32 VcsNodeFabosSlotOnlineEvent::getHwId () const
{
    return (m_hwId);
}

void VcsNodeFabosSlotOnlineEvent::setHwId (const UI32 &hwId)
{
    m_hwId = hwId;
}

UI32 VcsNodeFabosSlotOnlineEvent::getSlotState() const
{
    return (m_slotState);
}

void VcsNodeFabosSlotOnlineEvent::setSlotState (const UI32 &slotState)
{
    m_slotState = slotState;
}

UI32 VcsNodeFabosSlotOnlineEvent::getReason() const
{
    return (m_reason);
}

void VcsNodeFabosSlotOnlineEvent::setReason (const UI32 &reason)
{
    m_reason = reason;
}

UI32 VcsNodeFabosSlotOnlineEvent::getVersion() const
{
    return (m_version);
}

void VcsNodeFabosSlotOnlineEvent::setVersion (const UI32 &version)
{
    m_version = version;
}

UI32    VcsNodeFabosSlotOnlineEvent::getEventId       () const
{
    return m_eventId;
}

void    VcsNodeFabosSlotOnlineEvent::setEventId      (const UI32 &eventId)
{
    m_eventId = eventId;
}


VcsNodeFabosSlotOfflineEvent::VcsNodeFabosSlotOfflineEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SLOT_OFFLINE),
    m_slotNumber (0),
    m_objectType (0),
    m_hwType (0),
    m_hwId (0),
    m_slotState (0x50),
    m_reason (0),
    m_version (0),
    m_eventId (0)
{
}

VcsNodeFabosSlotOfflineEvent::VcsNodeFabosSlotOfflineEvent (UI32 &slotNumber)
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SLOT_OFFLINE),
    m_slotNumber (slotNumber),
    m_objectType (0),
    m_hwType (0x50),
    m_hwId (0),
    m_slotState (0x50),
    m_reason (0),
    m_version (0),
    m_eventId (0)
{
}

VcsNodeFabosSlotOfflineEvent::VcsNodeFabosSlotOfflineEvent (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version, UI32 &eventId)
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SLOT_OFFLINE),
    m_slotNumber (slotNumber),
    m_objectType (objectType),
    m_hwType (hwType),
    m_hwId (hwId),
    m_slotState (slotState),
    m_reason (reason),
    m_version (version),
    m_eventId (eventId)
{
}

VcsNodeFabosSlotOfflineEvent::~VcsNodeFabosSlotOfflineEvent ()
{
}

void VcsNodeFabosSlotOfflineEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_slotNumber,  "slotNumber"));
     addSerializableAttribute (new AttributeUI32   (&m_objectType,  "objectType"));
     addSerializableAttribute (new AttributeUI32   (&m_hwType,      "hwType"));
     addSerializableAttribute (new AttributeUI32   (&m_hwId,        "hwId"));
     addSerializableAttribute (new AttributeUI32   (&m_slotState,   "slotState"));
     addSerializableAttribute (new AttributeUI32   (&m_reason,      "reason"));
     addSerializableAttribute (new AttributeUI32   (&m_version,     "version"));
     addSerializableAttribute (new AttributeUI32   (&m_eventId,     "eventId"));
}

UI32 VcsNodeFabosSlotOfflineEvent::getSlotNumber () const
{
    return (m_slotNumber);
}

void VcsNodeFabosSlotOfflineEvent::setSlotNumber (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

UI32 VcsNodeFabosSlotOfflineEvent::getObjectType () const
{
    return (m_objectType);
}

void VcsNodeFabosSlotOfflineEvent::setObjectType (const UI32 &objectType)
{
    m_objectType = objectType;
}

UI32 VcsNodeFabosSlotOfflineEvent::getHwType() const
{
    return (m_hwType);
}

void VcsNodeFabosSlotOfflineEvent::setHwType (const UI32 &hwType)
{
    m_hwType = hwType;
}

UI32 VcsNodeFabosSlotOfflineEvent::getHwId () const
{
    return (m_hwId);
}

void VcsNodeFabosSlotOfflineEvent::setHwId (const UI32 &hwId)
{
    m_hwId = hwId;
}

UI32 VcsNodeFabosSlotOfflineEvent::getSlotState() const
{
    return (m_slotState);
}

void VcsNodeFabosSlotOfflineEvent::setSlotState (const UI32 &slotState)
{
    m_slotState = slotState;
}

UI32 VcsNodeFabosSlotOfflineEvent::getReason() const
{
    return (m_reason);
}

void VcsNodeFabosSlotOfflineEvent::setReason (const UI32 &reason)
{
    m_reason = reason;
}

UI32 VcsNodeFabosSlotOfflineEvent::getVersion() const
{
    return (m_version);
}

void VcsNodeFabosSlotOfflineEvent::setVersion (const UI32 &version)
{
    m_version = version;
}

UI32    VcsNodeFabosSlotOfflineEvent::getEventId       ()   const
{
    return m_eventId;
}

void    VcsNodeFabosSlotOfflineEvent::setEventId      (const UI32 &eventId)
{
    m_eventId = eventId;
}

}
