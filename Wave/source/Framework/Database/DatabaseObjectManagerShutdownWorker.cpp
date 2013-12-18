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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "Framework/Database/DatabaseObjectManagerShutdownWorker.h"
#include "Framework/Database/DatabaseObjectManagerShutdownMessage.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseConnection.h"
#include "libpq-fe.h"

#include <string>

namespace WaveNs
{

DatabaseObjectManagerShutdownWorker::DatabaseObjectManagerShutdownWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (DATABASE_OBJECT_MANAGER_SHUTDOWN, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerShutdownWorker::shutdownMessageHandler));
    addOperationMap (DATABASE_OBJECT_MANAGER_STOP, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerShutdownWorker::stopMessageHandler));
}

DatabaseObjectManagerShutdownWorker::~DatabaseObjectManagerShutdownWorker ()
{
}

PrismMessage *DatabaseObjectManagerShutdownWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case DATABASE_OBJECT_MANAGER_SHUTDOWN :
            pPrismMessage = new DatabaseObjectManagerShutdownMessage ();
            break;

        case DATABASE_OBJECT_MANAGER_STOP :
            pPrismMessage = new DatabaseObjectManagerShutdownMessage (false);
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("DatabaseObjectManagerShutdownMessage ::createMessageInstance : Unknown operation code : ") + operationCode);
            prismAssert (false, __FILE__, __LINE__);
    }

    return (pPrismMessage);
}

void DatabaseObjectManagerShutdownWorker::shutdownMessageHandler (DatabaseObjectManagerShutdownMessage *pDatabaseObjectManagerShutdownMessage)
{
        string  databaseDirectory = pDatabaseObjectManagerShutdownMessage->getDatabaseDirectory ();
        databaseDirectory =  databaseDirectory + "/" + DatabaseObjectManager::getDatabaseDirectory ();

		trace (TRACE_LEVEL_INFO, string ("DB shutdown Handling started: ") + databaseDirectory);
        // Hold All messages in the Database OM from being processed 
        holdAll ();    

        // set DatabaseShuttingDownOnReboot to be true... this will make all subsequent queries to be in an infinite loop 
        DatabaseObjectManager::setIsDatabaseShuttingDownOnReboot (true);

        // Shutdown postgresql itself using system call
        int rc = system ((string ("pg_ctl -D ") + databaseDirectory + string (" stop -m fast")).c_str ());

		trace (TRACE_LEVEL_INFO, string ("DB shutdown Handling completed: ") + rc );

        if (rc == 0)
		{
        	pDatabaseObjectManagerShutdownMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		}
		else
		{
			pDatabaseObjectManagerShutdownMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
		}

        reply (pDatabaseObjectManagerShutdownMessage);
}

void DatabaseObjectManagerShutdownWorker::stopMessageHandler (DatabaseObjectManagerShutdownMessage *pDatabaseObjectManagerShutdownMessage)
{
    string  databaseDirectory = FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + DatabaseObjectManager::getDatabaseDirectory ();

    trace (TRACE_LEVEL_INFO, string ("DB shutdown Handling started: ") + databaseDirectory + ": shutdownOnReboot" + pDatabaseObjectManagerShutdownMessage->getIsShutdownOnReboot ());

    //First close the connections/sessions
    DatabaseConnection::deleteInstance ();

    // Shutdown postgresql itself using system call
    int rc = system ((string ("pg_ctl -D ") + databaseDirectory + string (" stop -m fast")).c_str ());

    trace (TRACE_LEVEL_INFO, string ("DB shutdown Handling completed: ") + rc );

    if (rc == 0)
    {
        pDatabaseObjectManagerShutdownMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        pDatabaseObjectManagerShutdownMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
    }

    reply (pDatabaseObjectManagerShutdownMessage);
}

}
