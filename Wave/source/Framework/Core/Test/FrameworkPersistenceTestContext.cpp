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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/Test/FrameworkPersistenceTestContext.h"
#include "Framework/Core/Test/PrismTestManagedObject1.h"
#include "Framework/Core/Test/PrismTestManagedObject2.h"
#include "Framework/Core/Test/PrismTestManagedObject3.h"
#include "Framework/Core/Test/PrismTestManagedObject4.h"
#include "Framework/Core/Test/PrismTestManagedObject5.h"
#include "Framework/Core/Test/PrismTestManagedObject5.h"
#include "Framework/Core/Test/WaveTestManagedObject6.h"
#include "Framework/Core/Test/WaveTestManagedObject7.h"
#include "Framework/Core/Test/WaveTestManagedObject10.h"
#include "Framework/Core/Test/WaveTestManagedObject11.h"
#include "Framework/Core/Test/WaveTestManagedObject12.h"

namespace WaveNs
{

FrameworkPersistenceTestContext::FrameworkPersistenceTestContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_numberOfObjectsToBeCreated (0),
      m_numberOfObjectsForPartialQuery (0),
    m_numberOfExpectedResults (0)
{
}

FrameworkPersistenceTestContext::~FrameworkPersistenceTestContext ()
{
    UI32 numberOfCreatedObjects = m_createdObjects.size ();
    UI32 i                      = 0;

    for (i = 0; i < numberOfCreatedObjects; i++)
    {
        delete m_createdObjects[i];
    }

    m_createdObjects.clear ();
}

vector<PrismTestManagedObject1 *> &FrameworkPersistenceTestContext::getCreatedObjects ()
{
    return (m_createdObjects);
}

map<ObjectId, PrismTestManagedObject1 *> &FrameworkPersistenceTestContext::getCreatedObjectsMap ()
{
    return (m_createdObjectsMap);
}

vector<PrismTestManagedObject2 *> &FrameworkPersistenceTestContext::getCreatedObjects2 ()
{
    return (m_createdObjects2);
}

map<ObjectId, PrismTestManagedObject2 *> &FrameworkPersistenceTestContext::getCreatedObjects2Map ()
{
    return (m_createdObjects2Map);
}

vector<PrismTestManagedObject3 *> &FrameworkPersistenceTestContext::getCreatedObjects3 ()
{
    return (m_createdObjects3);
}

map<ObjectId, PrismTestManagedObject3 *> &FrameworkPersistenceTestContext::getCreatedObjects3Map ()
{
    return (m_createdObjects3Map);
}

vector<PrismTestManagedObject4 *> &FrameworkPersistenceTestContext::getCreatedObjects4 ()
{
    return (m_createdObjects4);
}

map<ObjectId, PrismTestManagedObject4 *> &FrameworkPersistenceTestContext::getCreatedObjects4Map ()
{
    return (m_createdObjects4Map);
}

vector<PrismTestManagedObject5 *> &FrameworkPersistenceTestContext::getCreatedObjects5 ()
{
    return (m_createdObjects5);
}

map<ObjectId, PrismTestManagedObject5 *> &FrameworkPersistenceTestContext::getCreatedObjects5Map ()
{
    return (m_createdObjects5Map);
}

vector<WaveTestManagedObject6 *> &FrameworkPersistenceTestContext::getCreatedObjects6 ()
{
    return (m_createdObjects6);
}

map<ObjectId, WaveTestManagedObject6 *> &FrameworkPersistenceTestContext::getCreatedObjects6Map ()
{
    return (m_createdObjects6Map);
}

vector<WaveTestManagedObject7 *> &FrameworkPersistenceTestContext::getCreatedObjects7 ()
{
    return (m_createdObjects7);
}

map<ObjectId, WaveTestManagedObject7 *> &FrameworkPersistenceTestContext::getCreatedObjects7Map ()
{
    return (m_createdObjects7Map);
}

vector<WaveTestManagedObject10 *> &FrameworkPersistenceTestContext::getCreatedObjects10 ()
{
    return (m_createdObjects10);
}

map<ObjectId, WaveTestManagedObject10 *> &FrameworkPersistenceTestContext::getCreatedObjects10Map ()
{
    return (m_createdObjects10Map);
}

vector<WaveTestManagedObject11 *> &FrameworkPersistenceTestContext::getCreatedObjects11 ()
{
    return (m_createdObjects11);
}

map<ObjectId, WaveTestManagedObject11 *> &FrameworkPersistenceTestContext::getCreatedObjects11Map ()
{
    return (m_createdObjects11Map);
}

vector<WaveTestManagedObject12 *> &FrameworkPersistenceTestContext::getCreatedObjects12 ()
{
    return (m_createdObjects12);
}

map<ObjectId, WaveTestManagedObject12 *> &FrameworkPersistenceTestContext::getCreatedObjects12Map ()
{
    return (m_createdObjects12Map);
}

UI32 FrameworkPersistenceTestContext::getNumberOfExpectedResults () const
{
    return (m_numberOfExpectedResults);
}

void FrameworkPersistenceTestContext::setNumberOfExpectedResults (const UI32 &numberOfExpectedResults)
{
    m_numberOfExpectedResults = numberOfExpectedResults;
}

UI32 FrameworkPersistenceTestContext::getNumberOfObjectsToBeCreated () const
{
    return (m_numberOfObjectsToBeCreated);
}

void FrameworkPersistenceTestContext::setNumberOfObjectsToBeCreated (const UI32 &numberOfObjectsToBeCreated)
{
    m_numberOfObjectsToBeCreated = numberOfObjectsToBeCreated;
}

UI32 FrameworkPersistenceTestContext::getNumberOfObjectsForPartialQuery () const
{
    return (m_numberOfObjectsForPartialQuery);
}

void FrameworkPersistenceTestContext::setNumberOfObjectsForPartialQuery (const UI32 &numberOfObjectsForPartialQuery)
{
    m_numberOfObjectsForPartialQuery = numberOfObjectsForPartialQuery;
}

}
