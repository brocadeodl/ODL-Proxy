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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectCreateContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

WaveManagedObjectCreateContext::WaveManagedObjectCreateContext (PrismMessage* pPrismMessage, PrismElement* pPrismElement, PrismLinearSequencerStep* pSteps, UI32 numberOfSteps)
    : WaveManagedObjectUpdateContext   (pPrismMessage, pPrismElement, pSteps, numberOfSteps)
{
      m_pInputWaveManagedObject     = NULL;
      m_pOperateOnWaveManagedObject = NULL;

      m_needToCreateManagedObject = false;
}

WaveManagedObjectCreateContext::WaveManagedObjectCreateContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement* pPrismElement, PrismLinearSequencerStep* pSteps, UI32 numberOfSteps)
    : WaveManagedObjectUpdateContext   (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps)
{
      m_pInputWaveManagedObject     = NULL;
      m_pOperateOnWaveManagedObject = NULL;

      m_needToCreateManagedObject = false;
}

WaveManagedObjectCreateContext::~WaveManagedObjectCreateContext ()
{

}

string WaveManagedObjectCreateContext::getManagedObjectClassNameNeedToBeCreated () const
{
   return (m_managedObjectClassName);
}

void WaveManagedObjectCreateContext::setManagedObjectClassNameNeedToBeCreated (const string &managedObjectClassName)
{
    m_managedObjectClassName = managedObjectClassName;
}

bool WaveManagedObjectCreateContext::isNeedToCreateManagedObject () const
{
    return (m_needToCreateManagedObject);
}

void WaveManagedObjectCreateContext::setNeedToCreateManagedObject (const bool &needToCreateManagedObject)
{
    m_needToCreateManagedObject = needToCreateManagedObject;
}

vector<Attribute *> WaveManagedObjectCreateContext::getChildAttributeVector () const
{
    return (m_childAttributes);
}

void WaveManagedObjectCreateContext::addToChildAttributeVector (Attribute *pAttribute)
{
    prismAssert (NULL != pAttribute, __FILE__, __LINE__);

    m_childAttributes.push_back (pAttribute);
}

}

