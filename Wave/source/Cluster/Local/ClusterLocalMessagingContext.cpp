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

#include "Cluster/Local/ClusterLocalMessagingContext.h"

namespace WaveNs
{

ClusterLocalMessagingContext::ClusterLocalMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismSynchronousLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
    m_WaveSlotLocalManagedObjectPointer (NULL),
    m_waveNodePointer (NULL),
    m_slotNumber (0),
    m_hwType (0),
    m_slotCreatedFlag (false)
{
}

ClusterLocalMessagingContext::~ClusterLocalMessagingContext ()
{
    if (NULL != m_WaveSlotLocalManagedObjectPointer)
    {
        if (NULL != m_waveNodePointer)
        {
            m_waveNodePointer->removeWaveSlotLocalManagedObject (m_WaveSlotLocalManagedObjectPointer);
            delete (m_waveNodePointer);
        }
        else
        {
            delete (m_WaveSlotLocalManagedObjectPointer);
        }
    }
    else if (NULL != m_waveNodePointer)
    {
        delete (m_waveNodePointer);
    }
}

WaveSlotLocalManagedObject *ClusterLocalMessagingContext::getWaveSlotManagedObjectPointer ()
{
    return m_WaveSlotLocalManagedObjectPointer;
}

void    ClusterLocalMessagingContext::setWaveSlotManagedObjectPointer (WaveSlotLocalManagedObject *pWaveSlotManagedObjectPointer)
{
    m_WaveSlotLocalManagedObjectPointer = pWaveSlotManagedObjectPointer;
}
UI32  ClusterLocalMessagingContext::getSlotNumber ()
{
    return m_slotNumber;
}

void  ClusterLocalMessagingContext::setSlotNumber (UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

UI32  ClusterLocalMessagingContext::getHwType ()
{
    return m_hwType;
}

void  ClusterLocalMessagingContext::setHwType (UI32 &hwType)
{
    m_hwType = hwType;
}

LocationId ClusterLocalMessagingContext::getLocationId ()
{
    return m_locationId;
}

void ClusterLocalMessagingContext::setLocationId (LocationId &locationId)
{
    m_locationId = locationId;
}

bool  ClusterLocalMessagingContext::getSlotCreatedFlag ()
{
    return m_slotCreatedFlag;
}

void  ClusterLocalMessagingContext::setSlotCreatedFlag (bool slotCreatedFlag)
{
    m_slotCreatedFlag = slotCreatedFlag;
}

WaveNode  *ClusterLocalMessagingContext::getWaveNodePointer ()
{
    return m_waveNodePointer;
}

void  ClusterLocalMessagingContext::setWaveNodePointer (WaveNode *pWaveNode)
{
    m_waveNodePointer = pWaveNode;   
}


}
