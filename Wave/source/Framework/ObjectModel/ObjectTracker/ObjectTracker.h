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

#ifndef OBJECTTRACKER_H
#define OBJECTTRACKER_H

#include "Framework/Types/Types.h"
#include "Framework/MultiThreading/PrismPosixThread.h"
#include "Framework/Utils/PrismMutex.h"

#include <map>
#include <vector>

using namespace std;

namespace WaveNs
{

class WaveManagedObject;
class WaveObjectManager;

class ObjectTracker
{
    private :
        static bool isAKnownMessage (const WaveManagedObject *pWaveManagedObject);
        static bool isAKnownManagedObjectWithBt (const WaveManagedObject *pWaveManagedObject);
        static void clearCurrentlyAllocatedObjectsWithBt ();

    protected :
    public :
        static void addToObjectTracker           (const WaveManagedObject *pWaveManagedObject, const bool &queryResults = false, WaveObjectManager *pWaveObjectManager = NULL);
        static void deleteFromObjectTracker      (const WaveManagedObject *pWaveManagedObject);
        static void getObjects                   (const PrismServiceId &prismServiceId, vector<string> &objectClassNames, vector<string> &objectNames, vector<bool> &queryResults, vector<string> &btStrings);

        static void addQueriedObjectsToBtTracker (vector<WaveManagedObject *> *queryResults);
        static void changeBtTrackerState         (const bool &btTrackerState);

    // Now the data members

    private :
        static map<PrismThreadId, map<const WaveManagedObject *, const WaveManagedObject *> > m_currentlyAllocatedObjectsByThread;
        static map<const WaveManagedObject *, PrismThreadId>                                  m_currentlyAllocatedObjects;
        static map<const WaveManagedObject *, bool>                                           m_currentlyAllocatedObjectsForQueyResults;

        static bool                                                                           m_trackingObjectsWithBt;
        static map<const WaveManagedObject *, string>                                         m_currentlyAllocatedObjectsWithBt;

        static PrismMutex                                                                     m_objectTrackerMutex;

    protected :
    public :
};

}

#endif // OBJECTTRACKER_H
