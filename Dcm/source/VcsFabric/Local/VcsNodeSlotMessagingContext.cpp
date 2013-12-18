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
 *   Author : Jitendra Singh                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeSlotMessagingContext.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"
#include "dcm/ipc.h"

namespace DcmNs
{

VcsNodeSlotMessagingContext::VcsNodeSlotMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
    m_slotMessage (NULL),
    m_slotNumber (0),
    m_hwType (0),
    m_rbridgeId (0),
    m_locationId (0),
    m_slotState (0),
    m_hwId (0),
    m_linecardMode (VCS_NODE_SLOT_LINECARD_MODE_DEFAULT),
    m_objectType (0),
    m_reason (0),
    m_version (0),
    m_pWaveSlotLocalManagedObject (NULL), 
    m_pVcsSlotLocalManagedObject (NULL),
    m_pVcsNodeLocalManagedObject (NULL),
    m_slotCreatedFlag (false),
    m_msgType (0),
    m_iscDestination (0),    
    m_waveNodeObjectId (ObjectId::NullObjectId),
    m_waveSlotObjectId (ObjectId::NullObjectId)
{
}

VcsNodeSlotMessagingContext::~VcsNodeSlotMessagingContext ()
{
    if (NULL != m_pWaveSlotLocalManagedObject)
    {
        delete (m_pWaveSlotLocalManagedObject);
    }

    if (NULL != m_pVcsNodeLocalManagedObject)
    {
        delete (m_pVcsNodeLocalManagedObject);
    }
    else if ((NULL != m_pVcsSlotLocalManagedObject) && (false == m_slotCreatedFlag))
    {
        delete (m_pVcsSlotLocalManagedObject);
    }
}
           
UI32 VcsNodeSlotMessagingContext::getSlotNumber   () const
{
    return (m_slotNumber);
}

void VcsNodeSlotMessagingContext::setSlotNumber   (const UI32 slotNumber)
{
    m_slotNumber = slotNumber;
}
           
UI32 VcsNodeSlotMessagingContext::getHwType () const
{
    return (m_hwType);
}

void VcsNodeSlotMessagingContext::setHwType (const UI32 hwType)
{
    m_hwType = hwType;
}

UI32 VcsNodeSlotMessagingContext::getRbridgeId    () const
{
    return   m_rbridgeId;
}

void VcsNodeSlotMessagingContext::setRbridgeId    (const UI32 rbridgeId)
{
    m_rbridgeId = rbridgeId;
}

LocationId VcsNodeSlotMessagingContext::getLocationId ()
{
    return m_locationId;
}

void VcsNodeSlotMessagingContext::setLocationId   (const LocationId &locationId)
{
    m_locationId = locationId;
}

UI32 VcsNodeSlotMessagingContext::getSlotState   () const
{
    return (m_slotState);
}

void VcsNodeSlotMessagingContext::setSlotState   (const UI32 slotState)
{
    m_slotState = slotState;
}
           
UI32 VcsNodeSlotMessagingContext::getHwId () const
{
    return (m_hwId);
}

void VcsNodeSlotMessagingContext::setHwId (const UI32 hwId)
{
    m_hwId = hwId;
}

UI32 VcsNodeSlotMessagingContext::getLinecardMode () const
{
    return (m_linecardMode);
}

void VcsNodeSlotMessagingContext::setLinecardMode (const UI32 linecardMode)
{
    m_linecardMode = linecardMode;
}

UI32 VcsNodeSlotMessagingContext::getObjectType () const
{
    return (m_objectType);
}

void VcsNodeSlotMessagingContext::setObjectType (const UI32 objectType)
{
    m_objectType = objectType;
}

UI32 VcsNodeSlotMessagingContext::getReason () const
{
    return (m_reason);
}

void VcsNodeSlotMessagingContext::setReason (const UI32 reason)
{
    m_reason = reason;
}

UI32 VcsNodeSlotMessagingContext::getVersion () const
{
    return (m_version);
}

void VcsNodeSlotMessagingContext::setVersion (const UI32 version)
{
    m_version = version;
}

PrismMessage *VcsNodeSlotMessagingContext::getSlotMessage ()
{
    return (m_slotMessage);
}

void VcsNodeSlotMessagingContext::setSlotMessage (PrismMessage *slotMessage)
{
    m_slotMessage = slotMessage;
}

WaveSlotLocalManagedObject  *VcsNodeSlotMessagingContext::getWaveSlotManagedObjectPointer () const
{
    return m_pWaveSlotLocalManagedObject;
}

void  VcsNodeSlotMessagingContext::setWaveSlotManagedObjectPointer (WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject)
{
    m_pWaveSlotLocalManagedObject =  pWaveSlotLocalManagedObject;   
}

VcsSlotLocalManagedObject  *VcsNodeSlotMessagingContext::getVcsSlotManagedObjectPointer () const
{
    return m_pVcsSlotLocalManagedObject;
}

void  VcsNodeSlotMessagingContext::setVcsSlotManagedObjectPointer (VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject)
{
    m_pVcsSlotLocalManagedObject =  pVcsSlotLocalManagedObject;   
}

VcsNodeLocalManagedObject  *VcsNodeSlotMessagingContext::getVcsNodeManagedObjectPointer () const
{
    return m_pVcsNodeLocalManagedObject;
}

void  VcsNodeSlotMessagingContext::setVcsNodeManagedObjectPointer (VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject)
{
    m_pVcsNodeLocalManagedObject =  pVcsNodeLocalManagedObject;   
}

bool VcsNodeSlotMessagingContext::getSlotCreatedFlag ()
{
    return m_slotCreatedFlag;
}

void VcsNodeSlotMessagingContext::setSlotCreatedFlag (bool slotCreatedFlag)
{
    m_slotCreatedFlag = slotCreatedFlag;
}

int VcsNodeSlotMessagingContext::getMessageType ()
{
    return m_msgType;
}

void VcsNodeSlotMessagingContext::setMessageType (const  int &messageType)
{
    m_msgType = messageType;
}

int VcsNodeSlotMessagingContext::getIscDestination ()
{
    return m_iscDestination;
}

void VcsNodeSlotMessagingContext::setIscDestination (int reqh)
{
    m_iscDestination = reqh;
}

ObjectId VcsNodeSlotMessagingContext::getWaveNodeObjectId () const
{
    return (m_waveNodeObjectId);
}

void VcsNodeSlotMessagingContext::setWaveNodeObjectId (const ObjectId &waveNodeObjectId)
{
    m_waveNodeObjectId = waveNodeObjectId;
}

ObjectId VcsNodeSlotMessagingContext::getWaveSlotObjectId () const
{
    return (m_waveSlotObjectId);
}

void VcsNodeSlotMessagingContext::setWaveSlotObjectId (const ObjectId &waveSlotObjectId)
{
    m_waveSlotObjectId = waveSlotObjectId;
}

}
