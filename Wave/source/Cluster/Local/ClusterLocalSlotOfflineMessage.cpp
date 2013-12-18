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

#include "Cluster/Local/ClusterLocalSlotOfflineMessage.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalTypes.h"


namespace WaveNs
{

ClusterLocalSlotOfflineMessage::ClusterLocalSlotOfflineMessage ()
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_SLOT_OFFLINE_MESSAGE),
    m_slotNumber (0),
    m_hwType (0x10),
    m_slotState (0)
{
}

ClusterLocalSlotOfflineMessage::ClusterLocalSlotOfflineMessage (const UI32 &slotNumber, const UI32 &hwType, const UI32 &slotState)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_SLOT_OFFLINE_MESSAGE),
    m_slotNumber (slotNumber),
    m_hwType (hwType),
    m_slotState (slotState)
{
}

ClusterLocalSlotOfflineMessage::~ClusterLocalSlotOfflineMessage ()
{
}

void ClusterLocalSlotOfflineMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_slotNumber,  "slotNumber"));
     addSerializableAttribute (new AttributeUI32   (&m_hwType,      "hwType"));
     addSerializableAttribute (new AttributeUI32   (&m_slotState,   "slotState"));
}

UI32 ClusterLocalSlotOfflineMessage::getSlotNumber () const
{
    return (m_slotNumber);
}

void ClusterLocalSlotOfflineMessage::setSlotNumber (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

UI32 ClusterLocalSlotOfflineMessage::getSlotState() const
{
    return (m_slotState);
}

void ClusterLocalSlotOfflineMessage::setSlotState (const UI32 &slotState)
{
    m_slotState = slotState;
}

UI32 ClusterLocalSlotOfflineMessage::getHwType () const
{
    return (m_hwType);
}

void ClusterLocalSlotOfflineMessage::setHwType (const UI32 &hwType)
{
    m_hwType = hwType;
}


}

