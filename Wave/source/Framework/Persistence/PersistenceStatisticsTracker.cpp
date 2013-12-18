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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#include "Framework/Persistence/PersistenceStatisticsTracker.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/StringUtils.h"

#include <map>

namespace WaveNs
{

static          map <UI32, UI32>    s_prepareTransactionsRequestedMap;
static          map <UI32, UI32>    s_prepareTransactionsExecutedMap;
static          map <UI32, UI32>    s_prepareTransactionsMarkedExecutedOnPrimaryMap;
static          map <UI32, UI32>    s_prepareTransactionsMarkedDisconnectedOnSecondaryMap;
static          map <UI32, UI32>    s_commitTransactionsRequestedMap;
static          map <UI32, UI32>    s_commitTransactionsExecutedMap;
static          map <UI32, UI32>    s_rollbackTransactionsRequestedMap;
static          map <UI32, UI32>    s_rollbackTransactionsExecutedMap;

static  const   UI32                MAX_BUFFER_SIZE                 = 512;
static  const   string              statsFormat                     = "    %-64s  %-10d\n";


void PersistenceStatisticsTracker::incrementPrepareTransactionsRequested (UI32 serviceId)
{
    ++(s_prepareTransactionsRequestedMap[serviceId]);
}

void PersistenceStatisticsTracker::incrementPrepareTransactionsRequested (UI32 serviceId, UI32 numberOfTransactions)
{
    s_prepareTransactionsRequestedMap[serviceId] += numberOfTransactions;
}

void PersistenceStatisticsTracker::incrementPrepareTransactionsExecuted (UI32 serviceId)
{
    ++(s_prepareTransactionsExecutedMap[serviceId]);
}

void PersistenceStatisticsTracker::incrementPrepareTransactionsMarkedExecutedOnPrimary (UI32 serviceId)
{
    ++(s_prepareTransactionsMarkedExecutedOnPrimaryMap[serviceId]);
}

void PersistenceStatisticsTracker::incrementPrepareTransactionsMarkedDisconnectedOnSecondary (UI32 serviceId)
{
    ++(s_prepareTransactionsMarkedDisconnectedOnSecondaryMap[serviceId]);
}

void PersistenceStatisticsTracker::incrementCommitTransactionsRequested (UI32 serviceId)
{
    ++(s_commitTransactionsRequestedMap[serviceId]);
}

void PersistenceStatisticsTracker::incrementCommitTransactionsRequested (UI32 serviceId, UI32 numberOfTransactions)
{
    s_commitTransactionsRequestedMap[serviceId] += numberOfTransactions;
}

void PersistenceStatisticsTracker::incrementCommitTransactionsExecuted (UI32 serviceId)
{
    ++(s_commitTransactionsExecutedMap[serviceId]);
}

void PersistenceStatisticsTracker::incrementRollbackTransactionsRequested (UI32 serviceId)
{
    ++(s_rollbackTransactionsRequestedMap[serviceId]);
}

void PersistenceStatisticsTracker::incrementRollbackTransactionsRequested (UI32 serviceId, UI32 numberOfTransactions)
{
    s_rollbackTransactionsRequestedMap[serviceId] += numberOfTransactions;
}

void PersistenceStatisticsTracker::incrementRollbackTransactionsExecuted (UI32 serviceId)
{
    ++(s_rollbackTransactionsExecutedMap[serviceId]);
}

string PersistenceStatisticsTracker::getPrepareTransactionsRequestedStatistics ()
{
    UI32    numberOfElements    = s_prepareTransactionsRequestedMap.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<UI32, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;
    
        output = "Prepare Transactions Requested:\n";

        for (it = s_prepareTransactionsRequestedMap.begin (); it != s_prepareTransactionsRequestedMap.end (); ++it)
        {
            string serviceName = FrameworkToolKit::getServiceNameById (it->first);

            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), serviceName.c_str (), it->second);
            output += string (line);
    
            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Prepare Transactions Requested", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string PersistenceStatisticsTracker::getPrepareTransactionsExecutedStatistics ()
{
    UI32    numberOfElements    = s_prepareTransactionsExecutedMap.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<UI32, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;
    
        output = "Prepare Transactions Executed:\n";

        for (it = s_prepareTransactionsExecutedMap.begin (); it != s_prepareTransactionsExecutedMap.end (); ++it)
        {
            string serviceName = FrameworkToolKit::getServiceNameById (it->first);

            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), serviceName.c_str (), it->second);
            output += string (line);
    
            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Prepare Transactions Executed", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string PersistenceStatisticsTracker::getPrepareTransactionsMarkedExecutedOnPrimaryStatistics ()
{
    UI32    numberOfElements    = s_prepareTransactionsMarkedExecutedOnPrimaryMap.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<UI32, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Prepare Transactions Marked Executed On Primary:\n";

        for (it = s_prepareTransactionsMarkedExecutedOnPrimaryMap.begin (); it != s_prepareTransactionsMarkedExecutedOnPrimaryMap.end (); ++it)
        {
            string serviceName = FrameworkToolKit::getServiceNameById (it->first);

            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), serviceName.c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Prepare Transactions Marked Executed On Primary", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string PersistenceStatisticsTracker::getPrepareTransactionsMarkedDisconnectedOnSecondaryStatistics ()
{
    UI32    numberOfElements    = s_prepareTransactionsMarkedDisconnectedOnSecondaryMap.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<UI32, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Prepare Transactions Marked Disconnected On Secondary:\n";

        for (it = s_prepareTransactionsMarkedDisconnectedOnSecondaryMap.begin (); it != s_prepareTransactionsMarkedDisconnectedOnSecondaryMap.end (); ++it)
        {
            string serviceName = FrameworkToolKit::getServiceNameById (it->first);

            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), serviceName.c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Prepare Transactions Marked Disconnected On Secondary", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string PersistenceStatisticsTracker::getCommitTransactionsRequestedStatistics ()
{
    UI32    numberOfElements    = s_commitTransactionsRequestedMap.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<UI32, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Commit Transactions Requested:\n";

        for (it = s_commitTransactionsRequestedMap.begin (); it != s_commitTransactionsRequestedMap.end (); ++it)
        {
            string serviceName = FrameworkToolKit::getServiceNameById (it->first);

            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), serviceName.c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Commit Transactions Requested", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string PersistenceStatisticsTracker::getCommitTransactionsExecutedStatistics ()
{
    UI32    numberOfElements    = s_commitTransactionsExecutedMap.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<UI32, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Commit Transactions Executed:\n";

        for (it = s_commitTransactionsExecutedMap.begin (); it != s_commitTransactionsExecutedMap.end (); ++it)
        {
            string serviceName = FrameworkToolKit::getServiceNameById (it->first);

            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), serviceName.c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Commit Transactions Executed", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string PersistenceStatisticsTracker::getRollbackTransactionsRequestedStatistics ()
{
    UI32    numberOfElements    = s_rollbackTransactionsRequestedMap.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<UI32, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Rollback Transactions Requested:\n";

        for (it = s_rollbackTransactionsRequestedMap.begin (); it != s_rollbackTransactionsRequestedMap.end (); ++it)
        {
            string serviceName = FrameworkToolKit::getServiceNameById (it->first);

            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), serviceName.c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Rollback Transactions Requested", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string PersistenceStatisticsTracker::getRollbackTransactionsExecutedStatistics ()
{
    UI32    numberOfElements    = s_rollbackTransactionsExecutedMap.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<UI32, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Rollback Transactions Executed:\n";

        for (it = s_rollbackTransactionsExecutedMap.begin (); it != s_rollbackTransactionsExecutedMap.end (); ++it)
        {
            string serviceName = FrameworkToolKit::getServiceNameById (it->first);

            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), serviceName.c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Rollback Transactions Executed", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string PersistenceStatisticsTracker::getAllTransactionStatistics ()
{
    string output = "";

    output += getPrepareTransactionsRequestedStatistics ();
    output += getPrepareTransactionsExecutedStatistics ();
    output += getPrepareTransactionsMarkedExecutedOnPrimaryStatistics ();
    output += getPrepareTransactionsMarkedDisconnectedOnSecondaryStatistics ();
    output += getCommitTransactionsRequestedStatistics ();
    output += getCommitTransactionsExecutedStatistics ();
    output += getRollbackTransactionsRequestedStatistics ();
    output += getRollbackTransactionsExecutedStatistics ();

    return (output);
}

void PersistenceStatisticsTracker::clearPrepareTransactionsRequestedMap ()
{
    s_prepareTransactionsRequestedMap.clear ();
}

void PersistenceStatisticsTracker::clearPrepareTransactionsExecutedMap ()
{
    s_prepareTransactionsExecutedMap.clear ();
}

void PersistenceStatisticsTracker::clearPrepareTransactionsMarkedExecutedOnPrimaryMap ()
{
    s_prepareTransactionsMarkedExecutedOnPrimaryMap.clear ();
}

void PersistenceStatisticsTracker::clearPrepareTransactionsMarkedDisconnectedOnSecondaryMap ()
{
    s_prepareTransactionsMarkedDisconnectedOnSecondaryMap.clear ();
}

void PersistenceStatisticsTracker::clearCommitTransactionsRequestedMap ()
{
    s_commitTransactionsRequestedMap.clear ();
}

void PersistenceStatisticsTracker::clearCommitTransactionsExecutedMap ()
{
    s_commitTransactionsExecutedMap.clear ();
}

void PersistenceStatisticsTracker::clearRollbackTransactionsRequestedMap ()
{
    s_rollbackTransactionsRequestedMap.clear ();
}

void PersistenceStatisticsTracker::clearRollbackTransactionsExecutedMap ()
{
    s_rollbackTransactionsExecutedMap.clear ();
}

void PersistenceStatisticsTracker::clearAllTransactionStatistics ()
{
    clearPrepareTransactionsRequestedMap ();
    clearPrepareTransactionsExecutedMap ();
    clearPrepareTransactionsMarkedExecutedOnPrimaryMap ();
    clearPrepareTransactionsMarkedDisconnectedOnSecondaryMap ();
    clearCommitTransactionsRequestedMap ();
    clearCommitTransactionsExecutedMap ();
    clearRollbackTransactionsRequestedMap ();
    clearRollbackTransactionsExecutedMap ();
}

}
