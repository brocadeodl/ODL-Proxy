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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Database/DatabaseConnection.h"
#include "Framework/Database/DatabaseMultiConnection.h"
#include "Framework/Database/MultiDatabaseToolKit.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

const UI32 maximumNumOfRetriesWhenDBIsBusy = 50; 
DatabaseConnection *DatabaseConnection::s_pDatabaseConnection = NULL;

DatabaseConnection::DatabaseConnection (const string &name, const UI32 &port)
    : m_name        (name),
      m_port        (port),
      m_pConnection (NULL)
{
}

DatabaseConnection::~DatabaseConnection ()
{
    if (NULL  != m_pConnection)
    {
        close ();
    }
}

void DatabaseConnection::lock ()
{
    m_mutex.lock ();
}

void DatabaseConnection::unlock ()
{
    m_mutex.unlock ();
}

DatabaseConnection *DatabaseConnection::getInstance (const string &name, const UI32 &port)
{
    if (s_pDatabaseConnection)
        return (s_pDatabaseConnection);
    
    s_pDatabaseConnection = new DatabaseConnection (name, port);
    prismAssert (NULL != s_pDatabaseConnection, __FILE__, __LINE__);

    return (s_pDatabaseConnection);
}

void DatabaseConnection::deleteInstance ()
{
    if (s_pDatabaseConnection)
    {
        s_pDatabaseConnection->close ();
        delete s_pDatabaseConnection;
        s_pDatabaseConnection = NULL;
    }
}

PGconn *DatabaseConnection::getPConnection ()
{
    PGconn *pTempConnection = NULL;

    if (true == (MultiDatabaseToolKit::getIsMultiModeEnabled ()))
    {
        string serverName, serverIpAddress; 
        UI32 serverPort;
        UI32 index = 0;
        MultiDatabaseToolKit::getServerInformationAtIndex (index, serverName, serverIpAddress, serverPort);
        DatabaseMultiConnection *pDatabaseMultiConnection = DatabaseMultiConnection::getInstance ();
        lock ();
        pTempConnection = pDatabaseMultiConnection->getServerConnection (serverName);
        unlock ();
    }
    else
    {
        lock ();

        pTempConnection = m_pConnection;

        unlock ();
    }

    return (pTempConnection);
}

bool DatabaseConnection::connect (const UI32 &maximumNumberOfRetries)
{
    UI32   numberOfTries        = 0;
    string databaseErrorMessage = "";

    string connectionString = string ("dbname=") + m_name + string (" port=") + m_port;
    string createString     = string ("createdb") + string (" -p ") + m_port + string (" ") + m_name;
    string errorString      = string ("FATAL:  database ") + string("\"") + m_name + string("\"") + string(" does not exist\n");
    string errorString2     = string ("FATAL:  the database system is starting up\n");
    string errorString3     = string ("FATAL:  the database system is shutting down\n"); 

    lock ();

    while (numberOfTries <= maximumNumberOfRetries)
    {
        m_pConnection = PQconnectdb (connectionString.c_str ());

        if (CONNECTION_OK != (PQstatus (m_pConnection)))
        {
            databaseErrorMessage = PQerrorMessage (m_pConnection);
            trace (TRACE_LEVEL_ERROR, "DatabaseConnection::connect : error connecting with Database :\n" + databaseErrorMessage);

            PQfinish (m_pConnection);

            if (0 == databaseErrorMessage.compare (errorString))
            {
                trace (TRACE_LEVEL_INFO, "DatabaseConnection::connect : Creating Database...");
                trace (TRACE_LEVEL_INFO, "DatabaseConnection::connect : " + createString);
                system (createString.c_str ());
                continue;
            }

            m_pConnection = NULL;
        }
        else
        {
            unlock ();
            return (true);
        }

        numberOfTries++;

        cout << "Still Waiting to connect ..." << numberOfTries << "\r";
        fflush (stdout);

        prismSleep (1);
    }

    int numberOfTriesInfiniteLoop       = 0;

    //while (1)
    for (UI32 i = 0; i < maximumNumOfRetriesWhenDBIsBusy; i++)
    {
        m_pConnection = PQconnectdb (connectionString.c_str ());

        if (CONNECTION_OK != (PQstatus (m_pConnection)))
        {
            databaseErrorMessage = PQerrorMessage (m_pConnection);
            trace (TRACE_LEVEL_ERROR, "DatabaseConnection::connect : error connecting with Database :\n" + databaseErrorMessage);

            PQfinish (m_pConnection);

            if (0 == databaseErrorMessage.compare (errorString2))
            {
                if (!(numberOfTriesInfiniteLoop%10))
                {
                    trace (TRACE_LEVEL_INFO, "DatabaseConnection::connect: Trying to connect again since database system is starting up");
                }
                prismSleep (1);

                numberOfTriesInfiniteLoop++;

                continue;
            }
            else if (0 == databaseErrorMessage.compare (errorString3))
            {
                if (!(numberOfTriesInfiniteLoop%10)) 
                {
                    trace (TRACE_LEVEL_INFO, "DatabaseConnection::connect: Trying to connect again since database system, previous shutdown is still in progress.");
                }
                prismSleep (1);

                numberOfTriesInfiniteLoop++;

                continue; 
            }

            else
            {
                trace (TRACE_LEVEL_INFO, "DatabaseConnection::connect: Stopping to try connecting to database :\n" + databaseErrorMessage);
                break;
            }
        }
        else
        {
            unlock ();
            return (true);
        }
    }

    unlock ();

    DatabaseObjectManager::handleIfDBCorruption (databaseErrorMessage);

    return (false);
}

void DatabaseConnection::close ()
{
    lock ();

    if (NULL != m_pConnection)
    {
        PQfinish (m_pConnection);

        m_pConnection = NULL;
    }

    unlock ();
}

}
