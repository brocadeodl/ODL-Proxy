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

#include "Framework/Core/Test/Upgrade/UpgradePersistenceTestContext.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject1.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject2.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject3.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject4.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject5.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject5.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject6.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject7.h"

namespace WaveNs
{

UpgradePersistenceTestContext::UpgradePersistenceTestContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
        : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
        m_numberOfObjectsToBeCreated (0),
        m_numberOfObjectsForPartialQuery (0),
        m_numberOfExpectedResults (0)
{
}

UpgradePersistenceTestContext::~UpgradePersistenceTestContext ()
{
    UI32 numberOfCreatedObjects = m_createdObjects.size ();
    UI32 i                      = 0;

    for (i = 0; i < numberOfCreatedObjects; i++)
    {
        delete m_createdObjects[i];
    }

    m_createdObjects.clear ();
}

vector<UpgradeTestManagedObject1 *> &UpgradePersistenceTestContext::getCreatedObjects ()
{
    return (m_createdObjects);
}

map<ObjectId, UpgradeTestManagedObject1 *> &UpgradePersistenceTestContext::getCreatedObjectsMap ()
{
    return (m_createdObjectsMap);
}

vector<UpgradeTestManagedObject2 *> &UpgradePersistenceTestContext::getCreatedObjects2 ()
{
    return (m_createdObjects2);
}

map<ObjectId, UpgradeTestManagedObject2 *> &UpgradePersistenceTestContext::getCreatedObjects2Map ()
{
    return (m_createdObjects2Map);
}

vector<UpgradeTestManagedObject3 *> &UpgradePersistenceTestContext::getCreatedObjects3 ()
{
    return (m_createdObjects3);
}

map<ObjectId, UpgradeTestManagedObject3 *> &UpgradePersistenceTestContext::getCreatedObjects3Map ()
{
    return (m_createdObjects3Map);
}

vector<UpgradeTestManagedObject4 *> &UpgradePersistenceTestContext::getCreatedObjects4 ()
{
    return (m_createdObjects4);
}

map<ObjectId, UpgradeTestManagedObject4 *> &UpgradePersistenceTestContext::getCreatedObjects4Map ()
{
    return (m_createdObjects4Map);
}

vector<UpgradeTestManagedObject5 *> &UpgradePersistenceTestContext::getCreatedObjects5 ()
{
    return (m_createdObjects5);
}

map<ObjectId, UpgradeTestManagedObject5 *> &UpgradePersistenceTestContext::getCreatedObjects5Map ()
{
    return (m_createdObjects5Map);
}

vector<UpgradeTestManagedObject6 *> &UpgradePersistenceTestContext::getCreatedObjects6 ()
{
    return (m_createdObjects6);
}

map<ObjectId, UpgradeTestManagedObject6 *> &UpgradePersistenceTestContext::getCreatedObjects6Map ()
{
    return (m_createdObjects6Map);
}

vector<UpgradeTestManagedObject7 *> &UpgradePersistenceTestContext::getCreatedObjects7 ()
{
    return (m_createdObjects7);
}

map<ObjectId, UpgradeTestManagedObject7 *> &UpgradePersistenceTestContext::getCreatedObjects7Map ()
{
    return (m_createdObjects7Map);
}

UI32 UpgradePersistenceTestContext::getNumberOfExpectedResults () const
{
    return (m_numberOfExpectedResults);
}

void UpgradePersistenceTestContext::setNumberOfExpectedResults (const UI32 &numberOfExpectedResults)
{
    m_numberOfExpectedResults = numberOfExpectedResults;
}

UI32 UpgradePersistenceTestContext::getNumberOfObjectsToBeCreated () const
{
    return (m_numberOfObjectsToBeCreated);
}

void UpgradePersistenceTestContext::setNumberOfObjectsToBeCreated (const UI32 &numberOfObjectsToBeCreated)
{
    m_numberOfObjectsToBeCreated = numberOfObjectsToBeCreated;
}

UI32 UpgradePersistenceTestContext::getNumberOfObjectsForPartialQuery () const
{
    return (m_numberOfObjectsForPartialQuery);
}

void UpgradePersistenceTestContext::setNumberOfObjectsForPartialQuery (const UI32 &numberOfObjectsForPartialQuery)
{
    m_numberOfObjectsForPartialQuery = numberOfObjectsForPartialQuery;
}

}
