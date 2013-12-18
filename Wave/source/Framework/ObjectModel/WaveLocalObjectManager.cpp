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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include <map>

namespace WaveNs
{

static map<PrismServiceId, PrismServiceId> s_localPrismServices;

WaveLocalObjectManager::WaveLocalObjectManager (const string &objectManagerName, const UI32 &stackSize, const vector<UI32> *pCpuAffinityVector)
    : WaveObjectManager (objectManagerName, stackSize, pCpuAffinityVector)
{
    s_localPrismServices[getServiceId ()] = getServiceId ();
}

WaveLocalObjectManager::~WaveLocalObjectManager ()
{
}

bool WaveLocalObjectManager::isALocalPrismService ()
{
    return (true);
}

bool WaveLocalObjectManager::isALocalService (const PrismServiceId &prismServiceId)
{
    map<PrismServiceId, PrismServiceId>::iterator element = s_localPrismServices.find (prismServiceId);
    map<PrismServiceId, PrismServiceId>::iterator end     = s_localPrismServices.end ();

    if (element == end)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

}
