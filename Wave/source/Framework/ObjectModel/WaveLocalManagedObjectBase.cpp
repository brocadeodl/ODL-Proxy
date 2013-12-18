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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveLocalManagedObjectBase.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Cluster/Local/WaveNode.h"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"
#include "Cluster/Local/ClusterLocalGetObjectIdMessages.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

namespace WaveNs
{

WaveLocalManagedObjectBase::WaveLocalManagedObjectBase (WaveManagedObject *pWaveManagedObject)
    : m_pWaveManagedObject (pWaveManagedObject)
{
    setOwnerWaveNodeObjectId (FrameworkToolKit::getThisWaveNodeObjectId ());
}

WaveLocalManagedObjectBase::~WaveLocalManagedObjectBase ()
{
}

string WaveLocalManagedObjectBase::getClassName ()
{
    return ("WaveLocalManagedObjectBase");
}

void WaveLocalManagedObjectBase::setupAttributesForPersistence ()
{
    //m_pWaveManagedObject->addPersistableAttribute (new AttributeObjectIdAssociation (&m_ownerWaveNodeObjectId, "ownerWaveNodeObjectId", WaveNode::getClassName ()));
    m_pWaveManagedObject->addPersistableAttribute (new AttributeObjectId (&m_ownerWaveNodeObjectId, "ownerWaveNodeObjectId"));
    m_pWaveManagedObject->addPersistableAttribute (new AttributeObjectId(&m_waveSlotLocalManagedObjectObjectId, "waveSlotLocalManagedObjectObjectId"));
}

void WaveLocalManagedObjectBase::setupAttributesForCreate ()
{
    //m_pWaveManagedObject->addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_ownerWaveNodeObjectId, "ownerWaveNodeObjectId", WaveNode::getClassName ()));
    m_pWaveManagedObject->addPersistableAttributeForCreate (new AttributeObjectId (&m_ownerWaveNodeObjectId, "ownerWaveNodeObjectId"));
    m_pWaveManagedObject->addPersistableAttributeForCreate (new AttributeObjectId(&m_waveSlotLocalManagedObjectObjectId, "waveSlotLocalManagedObjectObjectId"));
}

void WaveLocalManagedObjectBase::setOwnerWaveNodeObjectId (const ObjectId &ownerWaveNodeObjectId)
{
    m_ownerWaveNodeObjectId = ownerWaveNodeObjectId;
}

ObjectId WaveLocalManagedObjectBase::getOwnerWaveNodeObjectId() const 
{
    return (m_ownerWaveNodeObjectId);
}

LocationId WaveLocalManagedObjectBase::getOwnerLocationId () const
{
    LocationId  ownerLocationId     = 0;
    ResourceId  status              = WAVE_MESSAGE_SUCCESS;          

    if (ObjectId::NullObjectId != m_ownerWaveNodeObjectId)
    {
        ClusterLocalGetLocationIdMessage *pClusterLocalGetLocationIdMessage = new ClusterLocalGetLocationIdMessage (m_ownerWaveNodeObjectId);

        status = m_pWaveManagedObject->sendSynchronously (pClusterLocalGetLocationIdMessage); 

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            m_pWaveManagedObject->trace (TRACE_LEVEL_ERROR, "WaveLocalManagedObjectBase::getOwnerLocationId : Message to get LocationId from framework failed ");
        }
        else 
        {
            status = pClusterLocalGetLocationIdMessage->getCompletionStatus (); 
            if (WAVE_MESSAGE_SUCCESS != status)
            {
                m_pWaveManagedObject->trace (TRACE_LEVEL_ERROR, "WaveLocalManagedObjectBase::getOwnerLocationId : LocationId not found ");
            }
            else
            {
                ownerLocationId = pClusterLocalGetLocationIdMessage->getLocationId ();
            }
        }
    
        delete pClusterLocalGetLocationIdMessage;
    }

    return (ownerLocationId);
}
                
ObjectId WaveLocalManagedObjectBase::getWaveSlotObjectId () const
{
    return m_waveSlotLocalManagedObjectObjectId;
}

void WaveLocalManagedObjectBase::setWaveSlotObjectId (const ObjectId &waveSlotObjectId)
{
    m_waveSlotLocalManagedObjectObjectId = waveSlotObjectId;
}

UI32 WaveLocalManagedObjectBase::getAssociatedSlot () const
{
    UI32        slotNumber     = WAVE_INVALID_SLOT;
    ResourceId  status;
    
    if (ObjectId::NullObjectId != m_waveSlotLocalManagedObjectObjectId)
    {
        ClusterLocalGetSlotNumberMessage *pClusterLocalGetSlotNumberMessage = new ClusterLocalGetSlotNumberMessage (m_waveSlotLocalManagedObjectObjectId);

        status = m_pWaveManagedObject->sendSynchronously (pClusterLocalGetSlotNumberMessage); 

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            m_pWaveManagedObject->trace (TRACE_LEVEL_ERROR, "WaveLocalManagedObjectBase::getAssociatedSlot: Message to get slotNumber from framework failed ");
        }
        else 
        {
            status = pClusterLocalGetSlotNumberMessage->getCompletionStatus (); 
            if (WAVE_MESSAGE_SUCCESS != status)
            {
                m_pWaveManagedObject->trace (TRACE_LEVEL_ERROR, "WaveLocalManagedObjectBase::getAssociatedSlot: slotNumber not found ");
            }
            else
            {
                slotNumber = pClusterLocalGetSlotNumberMessage->getSlotNumber ();
            }
        }
    
        delete pClusterLocalGetSlotNumberMessage;
    }
    
    return (slotNumber);
}

void WaveLocalManagedObjectBase::associateWithSlot (const UI32 &slotNumber, const LocationId &locationId)
{
    ObjectId slotObjectId = FrameworkToolKit::getObjectIdForSlotNumber (slotNumber, locationId);

    if ((ObjectId::NullObjectId != slotObjectId) || (WAVE_INVALID_SLOT == slotNumber))
    {
        setWaveSlotObjectId (slotObjectId);
    }
    else if ((locationId == 0) || (locationId == FrameworkToolKit::getThisLocationId ()))
    {
        m_pWaveManagedObject->trace (TRACE_LEVEL_INFO, "WaveLocalManagedObjectBase::associateWithSlot : The slot number to be associated with does not exist yet. Creating a dummy slot for association");

        //Create a dummy slot with the given slot id
        slotObjectId = FrameworkToolKit::createDummySlot (slotNumber);

        if (ObjectId::NullObjectId != slotObjectId)
        {
            setWaveSlotObjectId (slotObjectId);
        }
        else
        {
            m_pWaveManagedObject->trace (TRACE_LEVEL_FATAL, "WaveLocalManagedObjectBase::associateWithSlot : The dummy slot creation logic failed and hence asserting");
            prismAssert (false, __FILE__, __LINE__);
        }
                
    }
    else
    {
         m_pWaveManagedObject->trace (TRACE_LEVEL_FATAL, string ("WaveLocalManagedObjectBase::associateWithSlot : Could not find the slot corresponding to slotNumber = ") + slotNumber + string (" and LocationId = ") + locationId);
         prismAssert (false, __FILE__, __LINE__);
    }
    
    ObjectId locationObjectId = FrameworkToolKit::getObjectIdForLocationId (locationId);

    if (ObjectId::NullObjectId != locationObjectId)
    {
        m_ownerWaveNodeObjectId = locationObjectId; 
    }
    else
    {
        m_pWaveManagedObject->trace (TRACE_LEVEL_FATAL, string ("WaveLocalManagedObjectBase::associateWithSlot : The locationId to be associated with does not exist. Please check the location id provided. Asserting now ... location id is ") +  locationId);
        prismAssert (false, __FILE__, __LINE__);
    }
}

LocationId WaveLocalManagedObjectBase::getAssociatedLocationId () const
{
    return (getOwnerLocationId ());
}

void WaveLocalManagedObjectBase::associateWithLocationId (const LocationId &locationId, const UI32 &slotNumber)
{
    associateWithSlot (slotNumber, locationId);
}

}
