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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Database/DatabaseMultiConnection.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

PrismMutex               DatabaseMultiConnection::m_mutex;
DatabaseMultiConnection *DatabaseMultiConnection::m_pDatabaseMultiConnection      = NULL;
const UI32               DatabaseMultiConnection::m_maximumNumOfRetriesWhenDBIsBusy = 50;

DatabaseMultiConnection::DatabaseMultiConnection ()
{
}

DatabaseMultiConnection::~DatabaseMultiConnection ()
{
    close ();
}

DatabaseMultiConnection *DatabaseMultiConnection::getInstance ()
{
    lock ();

    if (NULL == m_pDatabaseMultiConnection)
    {
        m_pDatabaseMultiConnection = new DatabaseMultiConnection ();

        prismAssert (NULL != m_pDatabaseMultiConnection, __FILE__, __LINE__);
    }

    unlock ();

    return (m_pDatabaseMultiConnection);
}

void DatabaseMultiConnection::deleteInstance ()
{
    lock ();

    if (NULL != m_pDatabaseMultiConnection)
    {
        delete m_pDatabaseMultiConnection;

        m_pDatabaseMultiConnection = NULL;
    }

    unlock ();
}

void DatabaseMultiConnection::lock ()
{
    m_mutex.lock ();
}

void DatabaseMultiConnection::unlock ()
{
    m_mutex.unlock ();
}

bool DatabaseMultiConnection::isAKnownServer (const string &serverName)
{
    lock ();

    set<string>::iterator element    = m_serverSet.find (serverName);
    set<string>::iterator endElement = m_serverSet.end  ();

    if (endElement != element)
    {
        unlock ();

        return (true);
    }
    else
    {
        unlock ();

        return (false);
    }

    unlock ();

    return (false);
}

bool DatabaseMultiConnection::isAConnectedServer (const string &serverName)
{
    lock ();

    map<string, PGconn *>::const_iterator element    = m_serverConnectionMap.find (serverName);
    map<string, PGconn *>::const_iterator endElement = m_serverConnectionMap.end  ();

    if (endElement != element)
    {
        if (NULL != (element->second))
        {
            unlock ();

            return (true);
        }
        else
        {
            unlock ();

            return (false);
        }
    }
    else
    {
        unlock ();

        return (false);
    }

    unlock ();

    return (false);
}

PGconn *DatabaseMultiConnection::getServerConnection (const string &serverName)
{
    lock ();

    map<string, PGconn *>::iterator element    = m_serverConnectionMap.find (serverName);
    map<string, PGconn *>::iterator endElement = m_serverConnectionMap.end  ();

    if (endElement != element)
    {
        unlock ();

        return (element->second);
    }
    else
    {
        unlock ();

        return (NULL);
    }

    unlock ();

    return (NULL);
}

void DatabaseMultiConnection::connect (const UI32 &maximumNumberOfRetries)
{
    lock ();

    vector<string> serverNames         = m_serverNames;

    unlock ();

    UI32           numberOfServerNames = serverNames.size ();
    UI32           i                   = 0;
    bool           connectionStatus    = false;

    for (i = 0; i < numberOfServerNames; i++)
    {
        trace (TRACE_LEVEL_DEBUG, "DatabaseMultiConnection::connect Connecting to Server " + serverNames[i]);

        connectionStatus = connect (serverNames[i], maximumNumberOfRetries);

        if (false == connectionStatus)
        {
            trace (TRACE_LEVEL_ERROR, "DatabaseMultiConnection::connect : Connection to server failed : " + serverNames[i]);
        }
    }
}

bool DatabaseMultiConnection::connect (const string &serverName, const UI32 &maximumNumberOfRetries)
{
    if (true == (isAConnectedServer (serverName)))
    {
        return (true);
    }

    UI32    numberOfTries        = 0;
    string  databaseErrorMessage = "";
    string  serverDatabaseName;
    UI32    serverPort;
    bool    isKnown              = isAKnownServer (serverName);
    PGconn *pConnection          = NULL;

    if (false == isKnown)
    {
        trace (TRACE_LEVEL_ERROR, "DatabaseMultiConnection::connect : This Database Server is NOT Known : " + serverName);

        return (false);
    }

    getServerDetails (serverName, serverDatabaseName, serverPort);

    string connectionString = string ("dbname=") + serverDatabaseName + string (" port=") + serverPort;
    string errorString2     = string ("FATAL:  the database system is starting up\n");
    string errorString3     = string ("FATAL:  the database system is shutting down\n");

    lock ();

    while (numberOfTries <= maximumNumberOfRetries)
    {
        pConnection = PQconnectdb (connectionString.c_str ());

        if (CONNECTION_OK != (PQstatus (pConnection)))
        {
            databaseErrorMessage = PQerrorMessage (pConnection);
            trace (TRACE_LEVEL_ERROR, "DatabaseMultiConnection::connect : error connecting with Database :\n" + databaseErrorMessage);

            PQfinish (pConnection);

            pConnection = NULL;
        }
        else
        {
            m_serverConnectionMap[serverName] = pConnection;

            unlock ();

            return (true);
        }

        numberOfTries++;

        cout << "Still Waiting to connect ..." << numberOfTries << "\r";
        fflush (stdout);

        prismSleep (1);
    }

    int numberOfTriesInfiniteLoop       = 0;

    for (UI32 i = 0; i < m_maximumNumOfRetriesWhenDBIsBusy; i++)
    {
        pConnection = PQconnectdb (connectionString.c_str ());

        if (CONNECTION_OK != (PQstatus (pConnection)))
        {
            databaseErrorMessage = PQerrorMessage (pConnection);
            trace (TRACE_LEVEL_ERROR, "DatabaseMultiConnection::connect : error connecting with Database :\n" + databaseErrorMessage);

            PQfinish (pConnection);

            if (0 == databaseErrorMessage.compare (errorString2))
            {
                if (!(numberOfTriesInfiniteLoop%10))
                {
                    trace (TRACE_LEVEL_INFO, "DatabaseMultiConnection::connect: Trying to connect again since database system is starting up");
                }
                prismSleep (1);

                numberOfTriesInfiniteLoop++;

                continue;
            }
            else if (0 == databaseErrorMessage.compare (errorString3))
            {
                if (!(numberOfTriesInfiniteLoop%10))
                {
                    trace (TRACE_LEVEL_INFO, "DatabaseMultiConnection::connect: Trying to connect again since database system, previous shutdown is still in progress.");
                }
                prismSleep (1);

                numberOfTriesInfiniteLoop++;

                continue;
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "DatabaseMultiConnection::connect: Stopping to try connecting to database :\n" + databaseErrorMessage);
                break;
            }
        }
        else
        {
            m_serverConnectionMap[serverName] = pConnection;

            unlock ();

            return (true);
        }
    }

    unlock ();

    return (false);
}

void DatabaseMultiConnection::close ()
{
    lock ();

    vector<string> serverNames         = m_serverNames;

    unlock ();

    UI32           numberOfServerNames = serverNames.size ();
    UI32           i                   = 0;

    for (i = 0; i < numberOfServerNames; i++)
    {
        close (serverNames[i]);
    }
}

void DatabaseMultiConnection::close (const string &serverName)
{
    lock ();

    PGconn *pConnection = NULL;

    if (true == (isAConnectedServer (serverName)))
    {
        pConnection = m_serverConnectionMap[serverName];

        if (NULL != pConnection)
        {
            PQfinish (pConnection);

            m_serverConnectionMap[serverName] = NULL;
        }
    }

    unlock ();
}

void DatabaseMultiConnection::addServer (const string &serverName, const string &serverDatabaseName, const UI32 &serverPort)
{
    bool isKnown = isAKnownServer (serverName);

    if (true == isKnown)
    {
        trace (TRACE_LEVEL_FATAL, "DatabaseMultiConnection::addServer : This server is already known : " + serverName);
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    m_serverNames.push_back (serverName);
    m_serverSet.insert (serverName);
    m_serverDatabaseNameMap[serverName] = serverDatabaseName;
    m_serverPortMap[serverName]         = serverPort;
}

void DatabaseMultiConnection::getServerDetails (const string &serverName, string &serverDatabaseName, UI32 &serverPort)
{
    bool isKnown = isAKnownServer (serverName);

    if (true == isKnown)
    {
        serverDatabaseName = m_serverDatabaseNameMap[serverName];
        serverPort         = m_serverPortMap[serverName];
    }
}

bool DatabaseMultiConnection::isAKnownManagedObjectName (const string &managedObject) const
{
    map<string, string>::const_iterator element    = m_managedObjectToServerNameMap.find (managedObject);
    map<string, string>::const_iterator endElement = m_managedObjectToServerNameMap.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void DatabaseMultiConnection::addManagedObjectToServerNameMapEntry (const string &managedObject, const string &serverName)
{
    bool isKnown = isAKnownManagedObjectName (managedObject);

    if (false == isKnown)
    {
        m_managedObjectToServerNameMap[managedObject] = serverName;
    }
    else if (serverName != m_managedObjectToServerNameMap[managedObject])
    {
        tracePrintf (TRACE_LEVEL_INFO, false, false, "DatabaseMultiConnection::addManagedObjectToServerNameMapEntry : Overlapping Managed Object : %s\n", managedObject.c_str ());
        m_managedObjectToServerNameMap[managedObject] = serverName;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "DatabaseMultiConnection::addManagedObjectToServerNameMapEntry : " + managedObject + " is already added");
        prismAssert (false, __FILE__, __LINE__);
    }
}

string DatabaseMultiConnection::getServerNameForManagedObject (const string &managedObject) const
{
    map<string, string>::const_iterator element    = m_managedObjectToServerNameMap.find (managedObject);
    map<string, string>::const_iterator endElement = m_managedObjectToServerNameMap.end  ();

    if (endElement != element)
    {
        return (element->second);
    }
    else
    {
        return ("");
    }
}

PGconn *DatabaseMultiConnection::getServerConnectionForManagedObject (const string &managedObject)
{
    string serverName = getServerNameForManagedObject (managedObject);

    prismAssert ("" != serverName, __FILE__, __LINE__);

    return (getServerConnection (serverName));
}

}
