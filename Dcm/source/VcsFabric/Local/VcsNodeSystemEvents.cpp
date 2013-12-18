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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou, Leifang Hu                                        *
 ***************************************************************************/

#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeSystemEvents.h"

namespace DcmNs
{

/*************************************************************************
* Slot online Event
*************************************************************************/

VcsNodeSlotOnlineEvent::VcsNodeSlotOnlineEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SLOT_ONLINE),
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

VcsNodeSlotOnlineEvent::VcsNodeSlotOnlineEvent (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version)
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SLOT_ONLINE),
    m_slotNumber (slotNumber),
    m_objectType (objectType),
    m_hwType (hwType),
    m_hwId (hwId),
    m_slotState (slotState),
    m_reason (reason),
    m_version (version),
    m_eventId (0)
{
}

VcsNodeSlotOnlineEvent::~VcsNodeSlotOnlineEvent ()
{
}

void VcsNodeSlotOnlineEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32   (&m_slotNumber,   "slotNumber"));
    addSerializableAttribute (new AttributeUI32   (&m_objectType,   "objectType"));
    addSerializableAttribute (new AttributeUI32   (&m_hwType,       "hwType"));
    addSerializableAttribute (new AttributeUI32   (&m_hwId,         "hwId"));
    addSerializableAttribute (new AttributeUI32   (&m_slotState,    "slotState"));
    addSerializableAttribute (new AttributeUI32   (&m_reason,       "reason"));
    addSerializableAttribute (new AttributeUI32   (&m_version,      "version"));
    addSerializableAttribute (new AttributeUI32   (&m_eventId,      "eventId"));
    
    
}

UI32 VcsNodeSlotOnlineEvent::getSlotNumber () const
{
    return (m_slotNumber);
}

void VcsNodeSlotOnlineEvent::setSlotNumber (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

UI32 VcsNodeSlotOnlineEvent::getHwType () const
{
    return (m_hwType);
}

void VcsNodeSlotOnlineEvent::setHwType (const UI32 &hwType)
{
    m_hwType = hwType;
}

UI32 VcsNodeSlotOnlineEvent::getObjectType () const
{
    return (m_objectType);
}

void VcsNodeSlotOnlineEvent::setObjectType (const UI32 &objectType)
{
    m_objectType = objectType;
}

UI32 VcsNodeSlotOnlineEvent::getHwId () const
{
    return (m_hwId);
}

void VcsNodeSlotOnlineEvent::setHwId (const UI32 &hwId)
{
    m_hwId = hwId;
}

UI32 VcsNodeSlotOnlineEvent::getSlotState() const
{
    return (m_slotState);
}

void VcsNodeSlotOnlineEvent::setSlotState (const UI32 &slotState)
{
    m_slotState = slotState;
}

UI32 VcsNodeSlotOnlineEvent::getReason() const
{
    return (m_reason);
}

void VcsNodeSlotOnlineEvent::setReason (const UI32 &reason)
{
    m_reason = reason;
}

UI32 VcsNodeSlotOnlineEvent::getVersion() const
{
    return (m_version);
}

void VcsNodeSlotOnlineEvent::setVersion (const UI32 &version)
{
    m_version = version;
}

UI32    VcsNodeSlotOnlineEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeSlotOnlineEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}


/*************************************************************************
* Slot offline Event
*************************************************************************/

VcsNodeSlotOfflineEvent::VcsNodeSlotOfflineEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SLOT_OFFLINE),
    m_slotNumber (0),
    m_hwType (0),
    m_slotState (0),
    m_eventId (0)
{
}

VcsNodeSlotOfflineEvent::VcsNodeSlotOfflineEvent (UI32 &slotNumber, UI32 &hwType, UI32 &slotState)
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SLOT_OFFLINE),
    m_slotNumber (slotNumber),
    m_hwType (hwType),
    m_slotState (slotState),
    m_eventId (0)
{
}

VcsNodeSlotOfflineEvent::~VcsNodeSlotOfflineEvent ()
{
}

void VcsNodeSlotOfflineEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32   (&m_slotNumber,   "slotNumber"));
    addSerializableAttribute (new AttributeUI32   (&m_hwType,       "hwType"));
    addSerializableAttribute (new AttributeUI32   (&m_slotState,    "slotState"));
    addSerializableAttribute (new AttributeUI32   (&m_eventId,      "eventId"));
}

UI32 VcsNodeSlotOfflineEvent::getSlotNumber () const
{
    return (m_slotNumber);
}

void VcsNodeSlotOfflineEvent::setSlotNumber (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

UI32 VcsNodeSlotOfflineEvent::getHwType() const
{
    return (m_hwType);
}

void VcsNodeSlotOfflineEvent::setHwType (const UI32 &hwType)
{
    m_hwType = hwType;
}

UI32 VcsNodeSlotOfflineEvent::getSlotState() const
{
    return (m_slotState);
}

void VcsNodeSlotOfflineEvent::setSlotState (const UI32 &slotState)
{
    m_slotState = slotState;
}

UI32    VcsNodeSlotOfflineEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeSlotOfflineEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

/*************************************************************
* SwitchReadyToAcceptCommands Event
*************************************************************************/

VcsNodeSwitchReadyToAcceptCommandsEvent::VcsNodeSwitchReadyToAcceptCommandsEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SWITCH_READY_TO_ACCEPT_COMMANDS),
    m_eventId (0)
{
}

VcsNodeSwitchReadyToAcceptCommandsEvent::~VcsNodeSwitchReadyToAcceptCommandsEvent ()
{
}

void VcsNodeSwitchReadyToAcceptCommandsEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32   (&m_eventId, "eventId"));
}

UI32    VcsNodeSwitchReadyToAcceptCommandsEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeSwitchReadyToAcceptCommandsEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

VcsNodeSwitchActivationCompleteEvent::VcsNodeSwitchActivationCompleteEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SWITCH_ACTIVATION_COMPLETE),
	m_eventId (0)
{
}

VcsNodeSwitchActivationCompleteEvent::~VcsNodeSwitchActivationCompleteEvent ()
{
}

void VcsNodeSwitchActivationCompleteEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32   (&m_eventId, "eventId"));
}

UI32    VcsNodeSwitchActivationCompleteEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeSwitchActivationCompleteEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

}

