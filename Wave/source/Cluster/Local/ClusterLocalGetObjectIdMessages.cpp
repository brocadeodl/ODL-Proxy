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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "Cluster/Local/ClusterLocalGetObjectIdMessages.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"

namespace WaveNs
{

ClusterLocalGetSlotObjectIdMessage::ClusterLocalGetSlotObjectIdMessage (const UI32 &slotNumber, const LocationId &locationId)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_GET_SLOT_OBJECTID),
        m_slotNumber (slotNumber),
        m_locationId (locationId)
{
}

ClusterLocalGetSlotObjectIdMessage::~ClusterLocalGetSlotObjectIdMessage ()
{
}

UI32 ClusterLocalGetSlotObjectIdMessage::getSlotNumber () const
{
    return (m_slotNumber);
}

void ClusterLocalGetSlotObjectIdMessage::setSlotNumber (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

LocationId ClusterLocalGetSlotObjectIdMessage::getLocationId () const
{
    return (m_locationId);
}

void ClusterLocalGetSlotObjectIdMessage::setLocationId (const LocationId &locationId)
{
    m_locationId = locationId;
}

ObjectId ClusterLocalGetSlotObjectIdMessage::getSlotObjectId () const
{
    return (m_slotObjectId);
}

void ClusterLocalGetSlotObjectIdMessage::setSlotObjectId (const ObjectId slotObjectId)
{
    m_slotObjectId = slotObjectId;
}

ClusterLocalGetLocationObjectIdMessage::ClusterLocalGetLocationObjectIdMessage (const LocationId &locationId)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_GET_LOCATION_OBJECTID),
        m_locationId (locationId)
{
}

ClusterLocalGetLocationObjectIdMessage::~ClusterLocalGetLocationObjectIdMessage ()
{
}

LocationId ClusterLocalGetLocationObjectIdMessage::getLocationId () const
{
    return (m_locationId);
}

void ClusterLocalGetLocationObjectIdMessage::setLocationId (const LocationId &locationId)
{
    m_locationId = locationId;
}

ObjectId ClusterLocalGetLocationObjectIdMessage::getLocationObjectId () const
{
    return (m_locationObjectId);
}

void ClusterLocalGetLocationObjectIdMessage::setLocationObjectId (const ObjectId locationObjectId)
{
    m_locationObjectId = locationObjectId;
}

ClusterLocalGetLocationIdMessage::ClusterLocalGetLocationIdMessage (const ObjectId locationObjectId)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_GET_LOCATIONID),
        m_locationObjectId (locationObjectId)
{
}

ClusterLocalGetLocationIdMessage::~ClusterLocalGetLocationIdMessage ()
{
}

LocationId ClusterLocalGetLocationIdMessage::getLocationId () const
{
    return (m_locationId);
}

void ClusterLocalGetLocationIdMessage::setLocationId (const LocationId &locationId)
{
    m_locationId = locationId;
}

ObjectId ClusterLocalGetLocationIdMessage::getLocationObjectId () const
{
    return (m_locationObjectId);
}

void ClusterLocalGetLocationIdMessage::setLocationObjectId (const ObjectId locationObjectId)
{
    m_locationObjectId = locationObjectId;
}

ClusterLocalGetSlotNumberMessage::ClusterLocalGetSlotNumberMessage (const ObjectId slotObjectId)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_GET_SLOTNUMBER),
        m_slotObjectId (slotObjectId)
{
}

ClusterLocalGetSlotNumberMessage::~ClusterLocalGetSlotNumberMessage ()
{
}

UI32 ClusterLocalGetSlotNumberMessage::getSlotNumber () const
{
    return (m_slotNumber);
}

void ClusterLocalGetSlotNumberMessage::setSlotNumber (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

ObjectId ClusterLocalGetSlotNumberMessage::getSlotObjectId () const
{
    return (m_slotObjectId);
}

void ClusterLocalGetSlotNumberMessage::setSlotObjectId (const ObjectId slotObjectId)
{
    m_slotObjectId = slotObjectId;
}

}
