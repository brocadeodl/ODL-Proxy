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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                          *
 ***************************************************************************/

#include "SystemManagement/WaveConfigurationSegmentWorkerMap.h"
#include "SystemManagement/WaveConfigurationSegmentWorker.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

map<string, WaveConfigurationSegmentWorker *> WaveConfigurationSegmentWorkerMap::m_configurationSegmentWorkerMap;
PrismMutex WaveConfigurationSegmentWorkerMap::m_configurationSegmentWorkerMapMutex;
        
void WaveConfigurationSegmentWorkerMap::addConfigurationSegmentWorkerIntoMap  (const string &configurationSegmentName, WaveConfigurationSegmentWorker *pWaveConfigurationSegmentWorker)
{
    trace (TRACE_LEVEL_INFO, "WaveConfigurationSegmentWorkerMap::addConfigurationSegmentWorkerIntoMap : Entering ... ");

    if (false == isConfigurationSegmentNameInMap (configurationSegmentName))
    {
        m_configurationSegmentWorkerMapMutex.lock ();
        m_configurationSegmentWorkerMap[configurationSegmentName] = pWaveConfigurationSegmentWorker;
        m_configurationSegmentWorkerMapMutex.unlock ();
    }
}

bool WaveConfigurationSegmentWorkerMap::isConfigurationSegmentNameInMap (const string &configurationSegmentName)
{
    bool retVal = false;
    m_configurationSegmentWorkerMapMutex.lock ();

    map<string, WaveConfigurationSegmentWorker *>::iterator configSegmentWorkerMapIterator; 

    if (m_configurationSegmentWorkerMap.find (configurationSegmentName) != m_configurationSegmentWorkerMap.end ())
    {
        retVal = true;
    }
    else
    {
        retVal = false;
    }
    
    m_configurationSegmentWorkerMapMutex.unlock ();

    return (retVal);
}

}
