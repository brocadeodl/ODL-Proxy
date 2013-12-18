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

#ifndef TRACECLIENTMAP_H
#define TRACECLIENTMAP_H

#include "Framework/Types/Types.h"
#include "Framework/Trace/TraceTypes.h"
#include "Framework/Utils/PrismMutex.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

namespace WaveNs
{

class TraceClientMap
{
    private :
    protected :
    public :
                      TraceClientMap      ();

        bool          isAKnownClient      (TraceClientId traceClientId);
        bool          isAKnownClient      (const string &traceClientName);
        TraceClientId addClient           (TraceLevel tracelevel, const string &traceClientName);
        void          removeClient        (const string &traceClientName);
        bool          setClientLevel      (TraceClientId traceClientId, TraceLevel tracelevel);
        TraceLevel    getClientLevel      (TraceClientId traceClientId);
        void          getClientsAndLevels (vector<TraceClientId> &traceClientIdsVector, vector<TraceLevel> &traceLevelsVector);
        string        getTraceClientName  (const TraceClientId &traceClientId);

    // Now the data members

    private :
        map<TraceClientId, TraceLevel> m_traceClientsAndLevels;
        map<TraceClientId, string>     m_traceClientsAndNames;
        TraceClientId                  m_nextAvailableTraceClientId;

        PrismMutex                     m_traceClientMapMutex;

    protected :
    public :
};

}

#endif // TRACECLIENTMAP_H
