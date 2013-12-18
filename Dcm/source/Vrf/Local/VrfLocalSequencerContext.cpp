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
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#include "Vrf/Local/VrfLocalSequencerContext.h"

namespace DcmNs
{

VrfLocalSynchronousLinearSequencerContext::VrfLocalSynchronousLinearSequencerContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismSynchronousLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_pVrfLocalManagedObject(NULL)
{
        // Do nothing.
}

VrfLocalSynchronousLinearSequencerContext::~VrfLocalSynchronousLinearSequencerContext ()
{
        // Do nothing.
}

VrfLocalManagedObject  *VrfLocalSynchronousLinearSequencerContext::getVrfLocalManagedObjectPointer (void)
{
    return m_pVrfLocalManagedObject;
}

void VrfLocalSynchronousLinearSequencerContext::setVrfLocalManagedObjectPointer (VrfLocalManagedObject *pVrfLocalManagedObject)
{
    m_pVrfLocalManagedObject = pVrfLocalManagedObject;
}

void     VrfLocalSynchronousLinearSequencerContext::setReceivedWavePartitionManagedObjectId   (const ObjectId &receivedWavePartitionManagedObjectId) 
{
    m_receivedWavePartitionManagedObjectId = receivedWavePartitionManagedObjectId;
}

ObjectId VrfLocalSynchronousLinearSequencerContext::getReceivedWavePartitionManagedObjectId   (void) const 
{
    return (m_receivedWavePartitionManagedObjectId);
}

}
