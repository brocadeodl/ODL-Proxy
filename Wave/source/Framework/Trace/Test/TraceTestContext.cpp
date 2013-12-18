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

#include "Framework/Trace/Test/TraceTestContext.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

TraceTestContext::TraceTestContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
    m_traceLevelForAClient (TRACE_LEVEL_INFO)
{
}

TraceTestContext::~TraceTestContext ()
{
}

void TraceTestContext::clearTraceClientsInformation ()
{
    m_traceClientIdsVector.clear ();
    m_traceLevelsVector.clear ();
}

UI32 TraceTestContext::getNumberOfTraceClients ()
{
    UI32 numberOfClientIds = m_traceClientIdsVector.size ();
    UI32 numberOfLevels    = m_traceLevelsVector.size ();

    prismAssert (numberOfClientIds == numberOfLevels, __FILE__, __LINE__);

    return (numberOfClientIds);
}

TraceClientId TraceTestContext::getTraceClientIdAt (const UI32 &i)
{
    UI32 numberOfTraceClients = getNumberOfTraceClients ();

    prismAssert (i < numberOfTraceClients, __FILE__, __LINE__);

    return (m_traceClientIdsVector[i]);
}

TraceLevel TraceTestContext::getTraceLevelAt (const UI32 &i)
{
    UI32 numberOfTraceClients = getNumberOfTraceClients ();

    prismAssert (i < numberOfTraceClients, __FILE__, __LINE__);

    return (m_traceLevelsVector[i]);
}

void TraceTestContext::addTraceClientInformation (const TraceClientId &traceClientId, const TraceLevel &traceLevel)
{
    m_traceClientIdsVector.push_back (traceClientId);
    m_traceLevelsVector.push_back (traceLevel);
}

void TraceTestContext::setTraceLevelForAClient (const TraceLevel &traceLevelForAClient)
{
    m_traceLevelForAClient = traceLevelForAClient;
}

TraceLevel TraceTestContext::getTraceLevelForAClient ()
{
    return (m_traceLevelForAClient);
}

}
