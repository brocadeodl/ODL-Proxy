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

#include "Framework/Database/DatabaseStatisticsTracker.h"
#include "Framework/Utils/StringUtils.h"

#include <map>

namespace WaveNs
{

static          map <string, UI32>  s_directQuery;
static          map <string, UI32>  s_indirectQuery;
static          map <string, UI32>  s_managedObjectDirectQuery;
static          map <string, UI32>  s_managedObjectIndirectQuery;

static          UI32                s_transactionsRequested         = 0;
static          UI32                s_transactionsExecuted          = 0;
static          UI32                s_transactionsCommitted         = 0;
static          UI32                s_vacuumsRequested              = 0;
static          UI32                s_vacuumsExecuted               = 0;
static          UI32                s_vacuumInterval                = 0;

static  const   UI32                MAX_BUFFER_SIZE                 = 512;
static  const   string              statsFormat                     = "    %-64s  %-10d\n";



void DatabaseStatisticsTracker::incrementDirectQuery (const string &managedObjectName)
{
    ++(s_directQuery[managedObjectName]);
}

void DatabaseStatisticsTracker::incrementIndirectQuery (const string &managedObjectName)
{
    ++(s_indirectQuery[managedObjectName]);
}

void DatabaseStatisticsTracker::incrementManagedObjectsLoadedFromDirectQuery (const string &managedObjectName)
{
    ++(s_managedObjectDirectQuery[managedObjectName]);
}

void DatabaseStatisticsTracker::incrementManagedObjectsLoadedFromDirectQuery (const string &managedObjectName, UI32 numberOfManagedObjectsLoaded)
{
    if (0 != numberOfManagedObjectsLoaded)
    {
        s_managedObjectDirectQuery[managedObjectName] += numberOfManagedObjectsLoaded;
    }
}

void DatabaseStatisticsTracker::incrementManagedObjectsLoadedFromIndirectQuery (const string &managedObjectName)
{
    ++(s_managedObjectIndirectQuery[managedObjectName]);
}

void DatabaseStatisticsTracker::incrementManagedObjectsLoadedFromIndirectQuery (const string &managedObjectName, UI32 numberOfManagedObjectsLoaded)
{
    if (0 != numberOfManagedObjectsLoaded)
    {
        s_managedObjectIndirectQuery[managedObjectName] += numberOfManagedObjectsLoaded;
    }
}

string DatabaseStatisticsTracker::getDirectQueryStatistics ()
{
    UI32    numberOfElements        = s_directQuery.size ();
    string  output                  = "";

    if (0 < numberOfElements)
    {
        map<string, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Direct Queries:\n";

        for (it = s_directQuery.begin (); it != s_directQuery.end (); ++it)
        {
            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), (it->first).c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Direct Queries", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string DatabaseStatisticsTracker::getIndirectQueryStatistics ()
{
    UI32    numberOfElements    = s_indirectQuery.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<string, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Indirect Queries:\n";

        for (it = s_indirectQuery.begin (); it != s_indirectQuery.end (); ++it)
        {
            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), (it->first).c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Indirect Queries", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string DatabaseStatisticsTracker::getManagedObjectsLoadedFromDirectQueryStatistics ()
{
    UI32    numberOfElements    = s_managedObjectDirectQuery.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<string, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Managed Objects Loaded From Direct Queries:\n";

        for (it = s_managedObjectDirectQuery.begin (); it != s_managedObjectDirectQuery.end (); ++it)
        {
            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), (it->first).c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Managed Objects Loaded From Direct Queries", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string DatabaseStatisticsTracker::getManagedObjectsLoadedFromIndirectQueryStatistics ()
{
    UI32    numberOfElements    = s_managedObjectIndirectQuery.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<string, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Managed Objects Loaded from Indirect Queries:\n";

        for (it = s_managedObjectIndirectQuery.begin (); it != s_managedObjectIndirectQuery.end (); ++it)
        {
            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), (it->first).c_str (), it->second);
            output += string (line);
    
            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Managed Objects Loaded From Indirect Queries", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string DatabaseStatisticsTracker::getAllQueryStatistics ()
{
    string  output = "";

    output += getDirectQueryStatistics ();
    output += getIndirectQueryStatistics ();
    output += getManagedObjectsLoadedFromDirectQueryStatistics ();
    output += getManagedObjectsLoadedFromIndirectQueryStatistics ();

    return (output);
}

void DatabaseStatisticsTracker::clearDirectQueryMap ()
{
    s_directQuery.clear ();
}

void DatabaseStatisticsTracker::clearIndirectQueryMap ()
{
    s_indirectQuery.clear ();
}

void DatabaseStatisticsTracker::clearManagedObjectsLoadedFromDirectQueryMap ()
{
    s_managedObjectDirectQuery.clear ();
}

void DatabaseStatisticsTracker::clearManagedObjectsLoadedFromIndirectQueryMap ()
{
    s_managedObjectIndirectQuery.clear ();
}

void DatabaseStatisticsTracker::clearAllQueryStatistics ()
{
    clearDirectQueryMap ();
    clearIndirectQueryMap ();
    clearManagedObjectsLoadedFromDirectQueryMap ();
    clearManagedObjectsLoadedFromIndirectQueryMap ();
}


void DatabaseStatisticsTracker::incrementTransactionsRequested ()
{
    ++s_transactionsRequested;
}

void DatabaseStatisticsTracker::incrementTransactionsExecuted ()
{
    ++s_transactionsExecuted;
}

void DatabaseStatisticsTracker::incrementTransactionsCommitted ()
{
    ++s_transactionsCommitted;
}

void DatabaseStatisticsTracker::incrementVacuumsRequested ()
{
    ++s_vacuumsRequested;
}

void DatabaseStatisticsTracker::incrementVacuumsExecuted ()
{
    ++s_vacuumsExecuted;
}

void DatabaseStatisticsTracker::setVacuumInterval (UI32 vacuumInterval)
{
    s_vacuumInterval = vacuumInterval;
}

string DatabaseStatisticsTracker::getTransactionsRequestedStatistics ()
{
    char    line[MAX_BUFFER_SIZE];

    snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Number of Transactions Requested", s_transactionsRequested);
    string output = string (line);

    return (output);
}

string DatabaseStatisticsTracker::getTransactionsExecutedStatistics ()
{
    char    line[MAX_BUFFER_SIZE];

    snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Number of Transactions Executed", s_transactionsExecuted);
    string output = string (line);

    return (output);
}

string DatabaseStatisticsTracker::getTransactionsCommittedStatistics ()
{
    char    line[MAX_BUFFER_SIZE];
    
    snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Number of Commit Transactions", s_transactionsCommitted);
    string output = string (line);

    return (output);
}

string DatabaseStatisticsTracker::getVacuumsRequestedStatistics ()
{
    char    line[MAX_BUFFER_SIZE];

    snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Number of Vacuums Requested", s_vacuumsRequested);
    string output = string (line);

    return (output);
}

string DatabaseStatisticsTracker::getVacuumsExecutedStatistics ()
{
    char    line[MAX_BUFFER_SIZE];

    snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Number of Vacuums Executed", s_vacuumsExecuted);
    string output = string (line);

    return (output);
}

string DatabaseStatisticsTracker::getVacuumInterval ()
{
    char    line[MAX_BUFFER_SIZE];

    snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Vacuum Interval", s_vacuumInterval);
    string output = string (line);

    return (output);
}

string DatabaseStatisticsTracker::getAllTransactionStatistics ()
{
    string output = "Database Transactions:\n";

    output += getTransactionsRequestedStatistics ();
    output += getTransactionsExecutedStatistics ();
    output += getTransactionsCommittedStatistics ();

    output += "\nDatabase Vacuums:\n";
    output += getVacuumsRequestedStatistics ();
    output += getVacuumsExecutedStatistics ();
    output += getVacuumInterval ();

    return (output);
}

void DatabaseStatisticsTracker::clearTransactionsRequestedCounter ()
{
    s_transactionsRequested = 0;
}

void DatabaseStatisticsTracker::clearTransactionsExecutedCounter ()
{
    s_transactionsExecuted = 0;
}

void DatabaseStatisticsTracker::clearTransactionsCommittedCounter ()
{
    s_transactionsCommitted = 0;
}

void DatabaseStatisticsTracker::clearVacuumsRequestedCounter ()
{
    s_vacuumsRequested = 0;
}

void DatabaseStatisticsTracker::clearVacuumsExecutedCounter ()
{
    s_vacuumsExecuted = 0;
}

void DatabaseStatisticsTracker::clearAllTransactionStatistics ()
{
    clearTransactionsRequestedCounter ();
    clearTransactionsExecutedCounter ();
    clearTransactionsCommittedCounter ();
    clearVacuumsRequestedCounter ();
    clearVacuumsExecutedCounter ();
}

}
