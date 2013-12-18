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
 *   Copyright (C) 2006 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef TRACETESTCONTEXT_H
#define TRACETESTCONTEXT_H


#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Trace/TraceTypes.h"
#include <vector>

namespace WaveNs
{

class TraceTestContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                               TraceTestContext             (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual               ~TraceTestContext             ();
                void           clearTraceClientsInformation ();
                UI32           getNumberOfTraceClients      ();
                TraceClientId  getTraceClientIdAt           (const UI32 &i);
                TraceLevel     getTraceLevelAt              (const UI32 &i);
                void           addTraceClientInformation    (const TraceClientId &traceClientId, const TraceLevel &traceLevel);

                void           setTraceLevelForAClient      (const TraceLevel &traceLevelForAClient);
                TraceLevel     getTraceLevelForAClient      ();

    // Now the data members

    private :
        vector<TraceClientId> m_traceClientIdsVector;
        vector<TraceLevel>    m_traceLevelsVector;

        TraceLevel            m_traceLevelForAClient;

    protected :
    public :
};

}

#endif // TRACETESTCONTEXT_H
