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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh		                                       *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeGenerateSlotScnMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"


namespace DcmNs
{

VcsNodeGenerateSlotScnMessage::VcsNodeGenerateSlotScnMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GENERATE_SLOT_SCN),
    m_slotNumber (0),
    m_objectType (0),
    m_hwType (0),
    m_hwId (0),
    m_slotState (0),
    m_reason (0),
    m_version (0)
{
}

VcsNodeGenerateSlotScnMessage::VcsNodeGenerateSlotScnMessage (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GENERATE_SLOT_SCN),
    m_slotNumber (slotNumber),
    m_objectType (objectType),
    m_hwType (hwType),
    m_hwId (hwId),
    m_slotState (slotState),
    m_reason (reason),
    m_version (version)
{
}

VcsNodeGenerateSlotScnMessage::~VcsNodeGenerateSlotScnMessage ()
{
}

void VcsNodeGenerateSlotScnMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_slotNumber,  "slotNumber"));
     addSerializableAttribute (new AttributeUI32   (&m_objectType,  "objectType"));
     addSerializableAttribute (new AttributeUI32   (&m_hwType,      "hwType"));
     addSerializableAttribute (new AttributeUI32   (&m_hwId,        "hwId"));
     addSerializableAttribute (new AttributeUI32   (&m_slotState,   "slotState"));
     addSerializableAttribute (new AttributeUI32   (&m_reason,      "reason"));
     addSerializableAttribute (new AttributeUI32   (&m_version,     "version"));
}

UI32 VcsNodeGenerateSlotScnMessage::getSlotNumber () const
{
    return (m_slotNumber);
}

void VcsNodeGenerateSlotScnMessage::setSlotNumber (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

UI32 VcsNodeGenerateSlotScnMessage::getObjectType () const
{
    return (m_objectType);
}

void VcsNodeGenerateSlotScnMessage::setObjectType (const UI32 &objectType)
{
    m_objectType = objectType;
}

UI32 VcsNodeGenerateSlotScnMessage::getHwType () const
{
    return (m_hwType);
}

void VcsNodeGenerateSlotScnMessage::setHwType (const UI32 &hwType)
{
    m_hwType = hwType;
}

UI32 VcsNodeGenerateSlotScnMessage::getHwId () const
{
    return (m_hwId);
}

void VcsNodeGenerateSlotScnMessage::setHwId (const UI32 &hwId)
{
    m_hwId = hwId;
}

UI32 VcsNodeGenerateSlotScnMessage::getSlotState() const
{
    return (m_slotState);
}

void VcsNodeGenerateSlotScnMessage::setSlotState (const UI32 &slotState)
{
    m_slotState = slotState;
}

UI32 VcsNodeGenerateSlotScnMessage::getReason() const
{
    return (m_reason);
}

void VcsNodeGenerateSlotScnMessage::setReason (const UI32 &reason)
{
    m_reason = reason;
}

UI32 VcsNodeGenerateSlotScnMessage::getVersion() const
{
    return (m_version);
}

void VcsNodeGenerateSlotScnMessage::setVersion (const UI32 &version)
{
    m_version = version;
}


}

