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
 *   Author : Balachandar Mani                                             *
 ***************************************************************************/

#include "DceLinearSeqContext.h"

namespace DcmNs
{

DceLinearSeqContext::DceLinearSeqContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_pWaveManagedObject (NULL)
{
}

DceLinearSeqContext::DceLinearSeqContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_pWaveManagedObject (NULL)
{
}

DceLinearSeqContext::~DceLinearSeqContext ()
{
}


WaveManagedObject *DceLinearSeqContext::getPWaveManagedObject () const
{
    return (m_pWaveManagedObject);
}

void DceLinearSeqContext::setPWaveManagedObject (WaveManagedObject *pWaveManagedObject)
{
    m_pWaveManagedObject = pWaveManagedObject;
}

void DceLinearSeqContext::addManagedObjectsForGarbageCollection (const vector<WaveManagedObject *> &mos)
{
    PrismLinearSequencerContext::addManagedObjectsForGarbageCollection(mos);
}

void DceLinearSeqContext::addManagedObjectForGarbageCollection  (WaveManagedObject *mo)
{
    if (mo) PrismLinearSequencerContext::addManagedObjectForGarbageCollection(mo);
}

}
