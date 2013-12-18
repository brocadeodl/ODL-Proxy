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

#include "Framework/Trace/TraceClientMap.h"

namespace WaveNs
{

TraceClientMap::TraceClientMap ()
    : m_nextAvailableTraceClientId (0)
{
}

bool TraceClientMap::isAKnownClient (TraceClientId traceClientId)
{
    map<TraceClientId, TraceLevel>::iterator element         = m_traceClientsAndLevels.find (traceClientId);
    map<TraceClientId, TraceLevel>::iterator limitingElement = m_traceClientsAndLevels.end ();

    if (limitingElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool TraceClientMap::isAKnownClient (const string &traceClientName)
{
    map<TraceClientId, string>::iterator element = m_traceClientsAndNames.begin ();
    map<TraceClientId, string>::iterator end     = m_traceClientsAndNames.end ();

    while (element != end)
    {
        if (traceClientName == element->second)
        {
            return (true);
        }

        element++;
    }

    return (false);
}

TraceClientId TraceClientMap::addClient (TraceLevel traceLevel, const string &traceClientName)
{
    TraceClientId currentTraceClientId = 0;

    m_traceClientMapMutex.lock ();

    if (false == (isAKnownClient (traceClientName)))
    {
        m_nextAvailableTraceClientId++;
        currentTraceClientId = m_nextAvailableTraceClientId;

        m_traceClientsAndLevels[currentTraceClientId] = traceLevel;
        m_traceClientsAndNames[currentTraceClientId]  = traceClientName;
    }

    m_traceClientMapMutex.unlock ();

    return (currentTraceClientId);
}

void TraceClientMap::removeClient (const string &traceClientName)
{
    m_traceClientMapMutex.lock ();

    map<TraceClientId, string>::iterator element = m_traceClientsAndNames.begin ();
    map<TraceClientId, string>::iterator end     = m_traceClientsAndNames.end ();

    while (element != end)
    {
        if (traceClientName == element->second)
        {
            TraceClientId traceClientId = element->first;

            m_traceClientsAndNames.erase (element);

            map<TraceClientId, TraceLevel>::iterator element1    = m_traceClientsAndLevels.find (traceClientId);
            map<TraceClientId, TraceLevel>::iterator endElement1 = m_traceClientsAndLevels.end  ();

            if (endElement1 != element1)
            {
                m_traceClientsAndLevels.erase (element1);
            }

            break;
        }

        element++;
    }

    m_traceClientMapMutex.unlock ();
}

bool TraceClientMap::setClientLevel (TraceClientId traceClientId, TraceLevel traceLevel)
{
    bool isClientLevelSet = false;

    m_traceClientMapMutex.lock ();

    if (true == (isAKnownClient (traceClientId)))
    {
        m_traceClientsAndLevels[traceClientId] = traceLevel;

        isClientLevelSet = true;
    }
    else
    {
        isClientLevelSet = false;
    }

    m_traceClientMapMutex.unlock ();

    return (isClientLevelSet);
}

TraceLevel TraceClientMap::getClientLevel (TraceClientId traceClientId)
{
    TraceLevel traceLevel = TRACE_LEVEL_UNKNOWN;

    m_traceClientMapMutex.lock ();

    if (true == isAKnownClient (traceClientId))
    {
        traceLevel = m_traceClientsAndLevels[traceClientId];
    }

    m_traceClientMapMutex.unlock ();

    return (traceLevel);
}

void TraceClientMap::getClientsAndLevels (vector<TraceClientId> &traceClientIdsVector, vector<TraceLevel> &traceLevelsVector)
{
    m_traceClientMapMutex.lock ();

    map<TraceClientId, TraceLevel>::iterator element        = m_traceClientsAndLevels.begin ();
    map<TraceClientId, TraceLevel>::iterator end            = m_traceClientsAndLevels.end ();
    TraceClientId                            traceClientId;
    TraceLevel                               traceLevel;

    traceClientIdsVector.clear ();
    traceLevelsVector.clear ();

    while (element != end)
    {
        traceClientId = element->first;
        traceLevel    = element->second;

        traceClientIdsVector.push_back (traceClientId);
        traceLevelsVector.push_back (traceLevel);

        element++;
    }

    m_traceClientMapMutex.unlock ();
}

string TraceClientMap::getTraceClientName (const TraceClientId &traceClientId)
{
    string traceClientName = "";

    m_traceClientMapMutex.lock ();

    if (true == isAKnownClient (traceClientId))
    {
        traceClientName = m_traceClientsAndNames[traceClientId];
    }

    m_traceClientMapMutex.unlock ();

    return (traceClientName);
}

}
