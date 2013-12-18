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

#include "Cluster/MultiPartition/Global/MultiPartitionSequencerContext.h"

namespace WaveNs
{

MultiPartitionSynchronousLinearSequencerContext::MultiPartitionSynchronousLinearSequencerContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismSynchronousLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_pWavePartitionManagedObject(NULL)
{
        // Do nothing.
}

MultiPartitionSynchronousLinearSequencerContext::~MultiPartitionSynchronousLinearSequencerContext ()
{
        // Do nothing.
}

WavePartitionManagedObject  *MultiPartitionSynchronousLinearSequencerContext::getWavePartitionManagedObject(void)
{
    return m_pWavePartitionManagedObject;
}


void MultiPartitionSynchronousLinearSequencerContext::setWavePartitionManagedObject(WavePartitionManagedObject *pWavePartitionManagedObject)
{
    m_pWavePartitionManagedObject = pWavePartitionManagedObject;
}

ObjectId  MultiPartitionSynchronousLinearSequencerContext::getWaveNodeObjectId(void)
{
    return m_waveNodeObjectId; 
}

void MultiPartitionSynchronousLinearSequencerContext::setWaveNodeObjectId(const ObjectId &objId)
{
    m_waveNodeObjectId = objId;
}



}
