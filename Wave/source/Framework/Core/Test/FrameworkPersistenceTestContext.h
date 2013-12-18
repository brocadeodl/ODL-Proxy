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

#ifndef FRAMEWORKPERSISTENCETESTCONTEXT_H
#define FRAMEWORKPERSISTENCETESTCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/ObjectId.h"

#include <vector>
#include <map>

using namespace std;

namespace WaveNs
{

class PrismLinearSequencerContext;
class PrismTestManagedObject1;
class PrismTestManagedObject2;
class PrismTestManagedObject3;
class PrismTestManagedObject4;
class PrismTestManagedObject5;
class WaveTestManagedObject6;
class WaveTestManagedObject7;
class WaveTestManagedObject10;
class WaveTestManagedObject11;
class WaveTestManagedObject12;

class FrameworkPersistenceTestContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                                      FrameworkPersistenceTestContext  (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                                     ~FrameworkPersistenceTestContext  ();

            vector<PrismTestManagedObject1 *>        &getCreatedObjects                 ();
            map<ObjectId, PrismTestManagedObject1 *> &getCreatedObjectsMap              ();
            vector<PrismTestManagedObject2 *>        &getCreatedObjects2                ();
            map<ObjectId, PrismTestManagedObject2 *> &getCreatedObjects2Map             ();
            vector<PrismTestManagedObject3 *>        &getCreatedObjects3                ();
            map<ObjectId, PrismTestManagedObject3 *> &getCreatedObjects3Map             ();
            vector<PrismTestManagedObject4 *>        &getCreatedObjects4                ();
            map<ObjectId, PrismTestManagedObject4 *> &getCreatedObjects4Map             ();
            vector<PrismTestManagedObject5 *>        &getCreatedObjects5                ();
            map<ObjectId, PrismTestManagedObject5 *> &getCreatedObjects5Map             ();
            vector<WaveTestManagedObject6 *>         &getCreatedObjects6                ();
            map<ObjectId, WaveTestManagedObject6 *>  &getCreatedObjects6Map             ();
            vector<WaveTestManagedObject7 *>         &getCreatedObjects7                ();
            map<ObjectId, WaveTestManagedObject7 *>  &getCreatedObjects7Map             ();
            vector<WaveTestManagedObject10 *>        &getCreatedObjects10               ();
            map<ObjectId, WaveTestManagedObject10 *> &getCreatedObjects10Map            ();
            vector<WaveTestManagedObject11 *>        &getCreatedObjects11               ();
            map<ObjectId, WaveTestManagedObject11 *> &getCreatedObjects11Map            ();
            vector<WaveTestManagedObject12 *>        &getCreatedObjects12               ();
            map<ObjectId, WaveTestManagedObject12 *> &getCreatedObjects12Map            ();

            UI32                                      getNumberOfObjectsToBeCreated     () const;
            void                                      setNumberOfObjectsToBeCreated     (const UI32 &numberOfObjectsToBeCtreated);

            UI32                                      getNumberOfObjectsForPartialQuery () const;
            void                                      setNumberOfObjectsForPartialQuery (const UI32 &numberOfObjectsForPartialQuery);

            UI32                                      getNumberOfExpectedResults        () const;
            void                                      setNumberOfExpectedResults        (const UI32 &numberOfExpectedResults);

    // Now the data members

    private :
        vector<PrismTestManagedObject1 *>        m_createdObjects;
        map<ObjectId, PrismTestManagedObject1 *> m_createdObjectsMap;
        vector<PrismTestManagedObject2 *>        m_createdObjects2;
        map<ObjectId, PrismTestManagedObject2 *> m_createdObjects2Map;
        vector<PrismTestManagedObject3 *>        m_createdObjects3;
        map<ObjectId, PrismTestManagedObject3 *> m_createdObjects3Map;
        vector<PrismTestManagedObject4 *>        m_createdObjects4;
        map<ObjectId, PrismTestManagedObject4 *> m_createdObjects4Map;
        vector<PrismTestManagedObject5 *>        m_createdObjects5;
        map<ObjectId, PrismTestManagedObject5 *> m_createdObjects5Map;
        vector<WaveTestManagedObject6 *>         m_createdObjects6;
        map<ObjectId, WaveTestManagedObject6 *>  m_createdObjects6Map;
        vector<WaveTestManagedObject7 *>         m_createdObjects7;
        map<ObjectId, WaveTestManagedObject7 *>  m_createdObjects7Map;
        vector<WaveTestManagedObject10 *>        m_createdObjects10;
        map<ObjectId, WaveTestManagedObject10 *> m_createdObjects10Map;
        vector<WaveTestManagedObject11 *>        m_createdObjects11;
        map<ObjectId, WaveTestManagedObject11 *> m_createdObjects11Map;
        vector<WaveTestManagedObject12 *>        m_createdObjects12;
        map<ObjectId, WaveTestManagedObject12 *> m_createdObjects12Map;

        UI32                                     m_numberOfObjectsToBeCreated;
        UI32                                     m_numberOfObjectsForPartialQuery;
        UI32                                     m_numberOfExpectedResults;

    protected :
    public :
};

}

#endif // FRAMEWORKPERSISTENCETESTCONTEXT_H
