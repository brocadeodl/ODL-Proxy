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

#include "Framework/ObjectModel/WaveObjectManagerStatisticsTracker.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

static  const   UI32                MAX_BUFFER_SIZE                 = 512;
static  const   string              statsFormat                     = "    %-64s  %-10d\n";


WaveObjectManagerStatisticsTracker::WaveObjectManagerStatisticsTracker ()
{
}

WaveObjectManagerStatisticsTracker::~WaveObjectManagerStatisticsTracker ()
{
}

void WaveObjectManagerStatisticsTracker::incrementManagedObjectsCreated (const string &managedObjectName)
{
    ++(m_managedObjectsCreated[managedObjectName]);
}

void WaveObjectManagerStatisticsTracker::incrementManagedObjectsUpdated (const string &managedObjectName)
{
    ++(m_managedObjectsUpdated[managedObjectName]);
}

void WaveObjectManagerStatisticsTracker::incrementManagedObjectsDeleted (const string &managedObjectName)
{
    ++(m_managedObjectsDeleted[managedObjectName]);
}

void WaveObjectManagerStatisticsTracker::incrementCompositionsCreated (const string &compositionName)
{
    ++(m_compositionsCreated[compositionName]);
}

void WaveObjectManagerStatisticsTracker::incrementCompositionsDeleted (const string &compositionName)
{
    ++(m_compositionsDeleted[compositionName]);
}

void WaveObjectManagerStatisticsTracker::incrementAssociationsCreated (const string &associationName)
{
    ++(m_associationsCreated[associationName]);
}

void WaveObjectManagerStatisticsTracker::incrementAssociationsDeleted (const string &associationName)
{
    ++(m_associationsDeleted[associationName]);
}

string WaveObjectManagerStatisticsTracker::getManagedObjectsCreatedStatistics ()
{
    UI32    numberOfElements        = m_managedObjectsCreated.size ();
    string  output                  = "";

    if (0 < numberOfElements)
    {
        map<string, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Managed Objects Created:\n";

        for (it = m_managedObjectsCreated.begin (); it != m_managedObjectsCreated.end (); ++it)
        {
            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), (it->first).c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Managed Objects Created", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string WaveObjectManagerStatisticsTracker::getManagedObjectsUpdatedStatistics ()
{
    UI32    numberOfElements    = m_managedObjectsUpdated.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<string, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Managed Objects Updated:\n";

        for (it = m_managedObjectsUpdated.begin (); it != m_managedObjectsUpdated.end (); ++it)
        {
            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), (it->first).c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Managed Objects Updated", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string WaveObjectManagerStatisticsTracker::getManagedObjectsDeletedStatistics ()
{
    UI32    numberOfElements    = m_managedObjectsDeleted.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<string, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Managed Objects Deleted:\n";

        for (it = m_managedObjectsDeleted.begin (); it != m_managedObjectsDeleted.end (); ++it)
        {
            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), (it->first).c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Managed Objects Deleted", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string WaveObjectManagerStatisticsTracker::getCompositionsCreatedStatistics ()
{
    UI32    numberOfElements    = m_compositionsCreated.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<string, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Optimized Composition Insertion Into Parent:\n";

        for (it = m_compositionsCreated.begin (); it != m_compositionsCreated.end (); ++it)
        {
            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), (it->first).c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Optimized Composition Insertions Into Parent", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string WaveObjectManagerStatisticsTracker::getCompositionsDeletedStatistics ()
{
    UI32    numberOfElements    = m_compositionsDeleted.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<string, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Optimized Composition Deletion From Parent:\n";

        for (it = m_compositionsDeleted.begin (); it != m_compositionsDeleted.end (); ++it)
        {
            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), (it->first).c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Optimized Composition Deletions From Parent", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string WaveObjectManagerStatisticsTracker::getAssociationsCreatedStatistics ()
{
    UI32    numberOfElements    = m_associationsCreated.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<string, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Optimized Association Insertion Into Parent:\n";

        for (it = m_associationsCreated.begin (); it != m_associationsCreated.end (); ++it)
        {
            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), (it->first).c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Optimized Association Insertions Into Parent", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string WaveObjectManagerStatisticsTracker::getAssociationsDeletedStatistics ()
{
    UI32    numberOfElements    = m_associationsDeleted.size ();
    string  output              = "";

    if (0 < numberOfElements)
    {
        map<string, UI32>::iterator it;
        char                        line[MAX_BUFFER_SIZE];
        UI32                        total                   = 0;

        output = "Optimized Association Deletion From Parent:\n";

        for (it = m_associationsDeleted.begin (); it != m_associationsDeleted.end (); ++it)
        {
            snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), (it->first).c_str (), it->second);
            output += string (line);

            total += it->second;
        }

        snprintf (line, MAX_BUFFER_SIZE, statsFormat.c_str (), "Total Optimized Association Deletions From Parent", total);
        output += "\n" + string (line) + "\n";
    }

    return (output);
}

string WaveObjectManagerStatisticsTracker::getAllManagedObjectStatistics ()
{
    string  output = "";

    output += getManagedObjectsCreatedStatistics ();
    output += getManagedObjectsUpdatedStatistics ();
    output += getManagedObjectsDeletedStatistics ();
    output += getCompositionsCreatedStatistics   ();
    output += getCompositionsDeletedStatistics   ();
    output += getAssociationsCreatedStatistics   ();
    output += getAssociationsDeletedStatistics   ();

    return (output);
}

void WaveObjectManagerStatisticsTracker::clearManagedObjectsCreatedMap ()
{
    m_managedObjectsCreated.clear ();
}

void WaveObjectManagerStatisticsTracker::clearManagedObjectsUpdatedMap ()
{
    m_managedObjectsUpdated.clear ();
}

void WaveObjectManagerStatisticsTracker::clearManagedObjectsDeletedMap ()
{
    m_managedObjectsDeleted.clear ();
}

void WaveObjectManagerStatisticsTracker::clearCompositionsCreatedMap ()
{
    m_compositionsCreated.clear ();
}

void WaveObjectManagerStatisticsTracker::clearCompositionsDeletedMap ()
{
    m_compositionsDeleted.clear ();
}

void WaveObjectManagerStatisticsTracker::clearAssociationsCreatedMap ()
{
    m_associationsCreated.clear ();
}

void WaveObjectManagerStatisticsTracker::clearAssociationsDeletedMap ()
{
    m_associationsDeleted.clear ();
}

void WaveObjectManagerStatisticsTracker::clearAllManagedObjectStatistics ()
{
    clearManagedObjectsCreatedMap ();
    clearManagedObjectsUpdatedMap ();
    clearManagedObjectsDeletedMap ();
    clearCompositionsCreatedMap   ();
    clearCompositionsDeletedMap   ();
    clearAssociationsCreatedMap   ();
    clearAssociationsDeletedMap   ();
}

}
