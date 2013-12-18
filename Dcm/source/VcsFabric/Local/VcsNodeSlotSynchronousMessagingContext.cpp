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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeSlotSynchronousMessagingContext.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"

namespace DcmNs
{

VcsNodeSlotSynchronousMessagingContext::VcsNodeSlotSynchronousMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismSynchronousLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
    m_hwType (0),
    m_locationId (0),
    m_pVcsSlotLocalManagedObject (NULL),
    m_pVcsNodeLocalManagedObject (NULL)
{
}

VcsNodeSlotSynchronousMessagingContext::~VcsNodeSlotSynchronousMessagingContext ()
{
    if (NULL != m_pVcsNodeLocalManagedObject)
    {
        delete (m_pVcsNodeLocalManagedObject);
    }
}
           
UI32 VcsNodeSlotSynchronousMessagingContext::getHwType () const
{
    return (m_hwType);
}

void VcsNodeSlotSynchronousMessagingContext::setHwType (const UI32 hwType)
{
    m_hwType = hwType;
}

LocationId VcsNodeSlotSynchronousMessagingContext::getLocationId ()
{
    return m_locationId;
}

void VcsNodeSlotSynchronousMessagingContext::setLocationId   (const LocationId &locationId)
{
    m_locationId = locationId;
}

VcsSlotLocalManagedObject  *VcsNodeSlotSynchronousMessagingContext::getVcsSlotManagedObjectPointer () const
{
    return m_pVcsSlotLocalManagedObject;
}

void  VcsNodeSlotSynchronousMessagingContext::setVcsSlotManagedObjectPointer (VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject)
{
    m_pVcsSlotLocalManagedObject =  pVcsSlotLocalManagedObject;   
}

VcsNodeLocalManagedObject  *VcsNodeSlotSynchronousMessagingContext::getVcsNodeManagedObjectPointer () const
{
    return m_pVcsNodeLocalManagedObject;
}

void  VcsNodeSlotSynchronousMessagingContext::setVcsNodeManagedObjectPointer (VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject)
{
    m_pVcsNodeLocalManagedObject =  pVcsNodeLocalManagedObject;   
}

}
