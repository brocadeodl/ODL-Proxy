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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef WAVECONFIGURATIONSEGMENTWORKERMAP_H
#define WAVECONFIGURATIONSEGMENTWORKERMAP_H

#include "Framework/Utils/PrismMutex.h"

#include <map>
#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{
class WaveConfigurationSegmentWorker;

class WaveConfigurationSegmentWorkerMap
{
    private :
    protected :
    public :
        static void     addConfigurationSegmentWorkerIntoMap  (const string &configurationSegmentName, WaveConfigurationSegmentWorker *pWaveConfigurationSegmentWorker);

        static bool     isConfigurationSegmentNameInMap (const string &configurationSegmentName);

    // Now the data members

    private :
        static PrismMutex                                       m_configurationSegmentWorkerMapMutex; 


    protected :
    public :
        static map<string, WaveConfigurationSegmentWorker *>    m_configurationSegmentWorkerMap; // Map from Config Segment Name to Config Segment Worker registered
};

}

#endif // WAVECONFIGURATIONSEGMENTMAP_H
