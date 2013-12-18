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
 ***************************************************************************/

#ifndef PRISMSERVICEMAP_H
#define PRISMSERVICEMAP_H

#include <map>
#include <string>
#include <vector>
#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/MultiThreading/WaveThreadStatus.h"

using namespace std;

namespace WaveNs
{

class PrismThread;

class PrismServiceMap
{
    private :
    protected :
    public :
                           PrismServiceMap                 ();
                          ~PrismServiceMap                 ();
        void               addServiceMap                   (PrismServiceId id, PrismThread *pPrismThread, const string &serviceName);
        void               removeServiceMap                (PrismServiceId id);
        void               getListOfServiceIds             (vector<PrismServiceId> &serviceIds);
        PrismThread       *getPrismThreadForServiceId      (PrismServiceId id);
        string             getPrismServiceNameForServiceId (const PrismServiceId &id);
        PrismServiceId     getPrismServiceIdForServiceName (const string &serviceName);
        WaveThreadStatus  joinAllThreads                   ();

    // Now the data members

    private :
        map<PrismServiceId, PrismThread *> m_servicesMap;
        map<PrismServiceId, string>        m_servicesNameMap;
        map<string, PrismServiceId>        m_servicesIdMap;
        PrismMutex                         m_mutex;

    protected :
    public:
};

}

#endif //PRISMSERVICEMAP_H
