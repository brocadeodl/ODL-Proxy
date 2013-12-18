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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectDeleteContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

WaveManagedObjectDeleteContext::WaveManagedObjectDeleteContext (PrismMessage* pPrismMessage, PrismElement* pPrismElement, PrismLinearSequencerStep* pSteps, UI32 numberOfSteps)
    : WaveManagedObjectUpdateContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps)
{
      m_pInputWaveManagedObject     = NULL;
      m_pOperateOnWaveManagedObject = NULL;
}

WaveManagedObjectDeleteContext::~WaveManagedObjectDeleteContext ()
{

}

bool WaveManagedObjectDeleteContext::isNeedToDeleteManagedObject () const
{
    return (m_needToDeleteManagedObject);
}

void WaveManagedObjectDeleteContext::setNeedToDeleteManagedObject (const bool &needToDeleteManagedObject)
{
    m_needToDeleteManagedObject = needToDeleteManagedObject;
}

}

