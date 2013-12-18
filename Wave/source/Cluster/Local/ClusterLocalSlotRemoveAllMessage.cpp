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

#include "Cluster/Local/ClusterLocalSlotRemoveAllMessage.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"


namespace WaveNs
{

ClusterLocalSlotRemoveAllMessage::ClusterLocalSlotRemoveAllMessage ()
: PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_SLOT_REMOVE_ALL_MESSAGE),
  m_hwType (0x10) // hardcoded for SYS_SLOT_LC
{
    m_locationId = FrameworkToolKit::getThisLocationId ();
}
    
ClusterLocalSlotRemoveAllMessage::ClusterLocalSlotRemoveAllMessage (const LocationId &locationId)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_SLOT_REMOVE_ALL_MESSAGE),
      m_locationId (locationId), 
      m_hwType (0x10) // hardcoded for SYS_SLOT_LC
{
}

ClusterLocalSlotRemoveAllMessage::ClusterLocalSlotRemoveAllMessage (const LocationId &locationId, const UI32 &hwType)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_SLOT_REMOVE_ALL_MESSAGE),
      m_locationId (locationId), 
      m_hwType (hwType)
{
}

void ClusterLocalSlotRemoveAllMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeLocationId  (&m_locationId, "locationId"));
     addSerializableAttribute (new AttributeUI32        (&m_hwType,     "hwType"));
}

ClusterLocalSlotRemoveAllMessage::~ClusterLocalSlotRemoveAllMessage ()
{
}

LocationId ClusterLocalSlotRemoveAllMessage::getLocationId ()
{
    return m_locationId;
}

void ClusterLocalSlotRemoveAllMessage::setLocationId (const LocationId &locationId)
{
    m_locationId = locationId;
}

UI32 ClusterLocalSlotRemoveAllMessage::getHwType ()
{
    return m_hwType;
}

void ClusterLocalSlotRemoveAllMessage::setHwType (const  UI32 &hwType)
{
    m_hwType = hwType;
}

}

