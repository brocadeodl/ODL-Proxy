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

#include "Cluster/Local/ClusterLocalSlotOnlineMessage.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalTypes.h"


namespace WaveNs
{

ClusterLocalSlotOnlineMessage::ClusterLocalSlotOnlineMessage ()
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_SLOT_ONLINE_MESSAGE),
    m_slotNumber (0),
    m_objectType (0),
    m_hwType (0),
    m_hwId (0),
    m_slotState (0),
    m_reason (0),
    m_version (0), 
    m_waveNodeObjectId (ObjectId::NullObjectId),
    m_waveSlotObjectId (ObjectId::NullObjectId),
    m_isDummySlotUpdation (false)
{
}

ClusterLocalSlotOnlineMessage::ClusterLocalSlotOnlineMessage (const UI32 &slotNumber, const UI32 &objectType, const UI32 &hwType, const UI32 &hwId, const UI32 &slotState, const UI32 &reason, const UI32 &version)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_SLOT_ONLINE_MESSAGE),
    m_slotNumber (slotNumber),
    m_objectType (objectType),
    m_hwType (hwType),
    m_hwId (hwId),
    m_slotState (slotState),
    m_reason (reason),
    m_version (version), 
    m_waveNodeObjectId (ObjectId::NullObjectId),
    m_waveSlotObjectId (ObjectId::NullObjectId),
    m_isDummySlotUpdation (false)
{
}

ClusterLocalSlotOnlineMessage::~ClusterLocalSlotOnlineMessage ()
{
}

void ClusterLocalSlotOnlineMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_slotNumber,              "slotNumber"));
     addSerializableAttribute (new AttributeUI32   (&m_objectType,              "objectType"));
     addSerializableAttribute (new AttributeUI32   (&m_hwType,                  "hwType"));
     addSerializableAttribute (new AttributeUI32   (&m_hwId,                    "hwId"));
     addSerializableAttribute (new AttributeUI32   (&m_slotState,               "slotState"));
     addSerializableAttribute (new AttributeUI32   (&m_reason,                  "reason"));
     addSerializableAttribute (new AttributeUI32   (&m_version,                 "version"));
     addSerializableAttribute (new AttributeObjectId (&m_waveNodeObjectId,      "waveNodeObjectId"));
     addSerializableAttribute (new AttributeObjectId (&m_waveSlotObjectId,      "waveSlotObjectId"));
     addSerializableAttribute (new AttributeBool   (&m_isDummySlotUpdation,     "isDummySlotUpdation"));
}

UI32 ClusterLocalSlotOnlineMessage::getSlotNumber () const
{
    return (m_slotNumber);
}

void ClusterLocalSlotOnlineMessage::setSlotNumber (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

UI32 ClusterLocalSlotOnlineMessage::getObjectType () const
{
    return (m_objectType);
}

void ClusterLocalSlotOnlineMessage::setObjectType (const UI32 &objectType)
{
    m_objectType = objectType;
}

UI32 ClusterLocalSlotOnlineMessage::getHwType () const
{
    return (m_hwType);
}

void ClusterLocalSlotOnlineMessage::setHwType (const UI32 &hwType)
{
    m_hwType = hwType;
}

UI32 ClusterLocalSlotOnlineMessage::getHwId () const
{
    return (m_hwId);
}

void ClusterLocalSlotOnlineMessage::setHwId (const UI32 &hwId)
{
    m_hwId = hwId;
}

UI32 ClusterLocalSlotOnlineMessage::getSlotState() const
{
    return (m_slotState);
}

void ClusterLocalSlotOnlineMessage::setSlotState (const UI32 &slotState)
{
    m_slotState = slotState;
}

UI32 ClusterLocalSlotOnlineMessage::getReason() const
{
    return (m_reason);
}

void ClusterLocalSlotOnlineMessage::setReason (const UI32 &reason)
{
    m_reason = reason;
}

UI32 ClusterLocalSlotOnlineMessage::getVersion() const
{
    return (m_version);
}

void ClusterLocalSlotOnlineMessage::setVersion (const UI32 &version)
{
    m_version = version;
}

ObjectId ClusterLocalSlotOnlineMessage::getWaveNodeObjectId() const
{
    return (m_waveNodeObjectId);
}

void ClusterLocalSlotOnlineMessage::setWaveNodeObjectId (const ObjectId &waveNodeObjectId)
{
    m_waveNodeObjectId = waveNodeObjectId;
}

ObjectId ClusterLocalSlotOnlineMessage::getWaveSlotObjectId() const
{
    return (m_waveSlotObjectId);
}

void ClusterLocalSlotOnlineMessage::setWaveSlotObjectId (const ObjectId &waveSlotObjectId)
{
    m_waveSlotObjectId = waveSlotObjectId;
}

UI32 ClusterLocalSlotOnlineMessage::getIsDummySlotUpdation () const
{
    return (m_isDummySlotUpdation);
}

void ClusterLocalSlotOnlineMessage::setIsDummySlotUpdation (const bool isDummySlotUpdation)
{
    m_isDummySlotUpdation = isDummySlotUpdation;
}

}

