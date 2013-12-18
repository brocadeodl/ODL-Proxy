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

#include "Framework/ObjectModel/LoadOperationalDataContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

LoadOperationalDataContext::LoadOperationalDataContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_pWaveManagedObject        (NULL),
      m_pOwnerWaveManagedObject   (NULL)
{
}

LoadOperationalDataContext::LoadOperationalDataContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_pWaveManagedObject        (NULL),
      m_pOwnerWaveManagedObject   (NULL)
{
}

LoadOperationalDataContext::~LoadOperationalDataContext ()
{
    if (NULL != m_pOwnerWaveManagedObject)
    {
        delete m_pOwnerWaveManagedObject;
    }
    else if (NULL != m_pWaveManagedObject)
    {
        delete m_pWaveManagedObject;
    }
}

ObjectId LoadOperationalDataContext::getWaveManagedObjectId () const
{
    return (m_waveManagedObjctId);
}

void LoadOperationalDataContext::setWaveManagedObjectId (const ObjectId &waveManagedObjectId)
{
    m_waveManagedObjctId = waveManagedObjectId;
}

vector<string> LoadOperationalDataContext::getOperationalDataFields () const
{
    return (m_operationalDataFields);
}

void LoadOperationalDataContext::setOperationalDataFields (const vector<string> &operationalDataFields)
{
    m_operationalDataFields = operationalDataFields;
}

WaveManagedObject *LoadOperationalDataContext::getPWaveManagedObject () const
{
    return (m_pWaveManagedObject);
}

void LoadOperationalDataContext::setPWaveManagedObject (WaveManagedObject *pWaveManagedObject)
{
    m_pWaveManagedObject = pWaveManagedObject;
}

WaveManagedObject *LoadOperationalDataContext::getPOwnerWaveManagedObject () const
{
    return (m_pOwnerWaveManagedObject);
}

void LoadOperationalDataContext::setPOwnerWaveManagedObject (WaveManagedObject *pOwnerWaveManagedObject)
{
    m_pOwnerWaveManagedObject = pOwnerWaveManagedObject;
}

}
