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

#include "Framework/ObjectModel/LoadOperationalDataSynchronousContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

LoadOperationalDataSynchronousContext::LoadOperationalDataSynchronousContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismSynchronousLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_pWaveManagedObject                   (NULL)
{
}

LoadOperationalDataSynchronousContext::LoadOperationalDataSynchronousContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismSynchronousLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_pWaveManagedObject                   (NULL)
{
}

LoadOperationalDataSynchronousContext::~LoadOperationalDataSynchronousContext ()
{
}

ObjectId LoadOperationalDataSynchronousContext::getWaveManagedObjectId () const
{
    return (m_waveManagedObjctId);
}

void LoadOperationalDataSynchronousContext::setWaveManagedObjectId (const ObjectId &waveManagedObjectId)
{
    m_waveManagedObjctId = waveManagedObjectId;
}

vector<string> LoadOperationalDataSynchronousContext::getOperationalDataFields () const
{
    return (m_operationalDataFields);
}

void LoadOperationalDataSynchronousContext::setOperationalDataFields (const vector<string> &operationalDataFields)
{
    m_operationalDataFields = operationalDataFields;
}

WaveManagedObject *LoadOperationalDataSynchronousContext::getPWaveManagedObject () const
{
    return (m_pWaveManagedObject);
}

void LoadOperationalDataSynchronousContext::setPWaveManagedObject (WaveManagedObject *pWaveManagedObject)
{
    m_pWaveManagedObject = pWaveManagedObject;
}

}