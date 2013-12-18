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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"
#include "Framework/Utils/PrismMutex.h"
#include <map>

namespace WaveNs
{

map<PrismServiceId, PrismServiceId> s_userSpecificServicesMap;
PrismMutex s_userSpecificServicesMapMutex;

WaveLocalObjectManagerForUserSpecificTasks::WaveLocalObjectManagerForUserSpecificTasks (const string &objectManagerName, const UI32 &stackSize, const vector<UI32> *pCpuAffinityVector)
    : WaveLocalObjectManager (objectManagerName, stackSize, pCpuAffinityVector)
{
    addToUserspecificServicesMap (getServiceId ());
}

WaveLocalObjectManagerForUserSpecificTasks::~WaveLocalObjectManagerForUserSpecificTasks ()
{
}

void WaveLocalObjectManagerForUserSpecificTasks::addToUserspecificServicesMap (const PrismServiceId &prismServiceId)
{
    s_userSpecificServicesMapMutex.lock ();
    s_userSpecificServicesMap[prismServiceId] = prismServiceId;
    s_userSpecificServicesMapMutex.unlock ();
}

bool WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (const PrismServiceId &prismServiceId)
{
    s_userSpecificServicesMapMutex.lock ();

    map<PrismServiceId, PrismServiceId>::iterator element = s_userSpecificServicesMap.find (prismServiceId);
    map<PrismServiceId, PrismServiceId>::iterator end     = s_userSpecificServicesMap.end ();
    bool                                          found   = false;

    if (element != end)
    {
        found = true;
    }
    else
    {
        found = false;
    }

    s_userSpecificServicesMapMutex.unlock ();

    return (found);
}

}
