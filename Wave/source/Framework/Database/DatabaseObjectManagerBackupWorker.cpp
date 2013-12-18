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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Database/DatabaseObjectManagerBackupWorker.h"
#include "Framework/Database/DatabaseObjectManagerBackupMessage.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Database/DatabaseConnection.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

DatabaseObjectManagerBackupWorker::DatabaseObjectManagerBackupWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (DATABASE_OBJECT_MANAGER_BACKUP, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerBackupWorker::backupMessageHandler));
}

DatabaseObjectManagerBackupWorker::~DatabaseObjectManagerBackupWorker ()
{
}

void DatabaseObjectManagerBackupWorker::backupMessageHandler (DatabaseObjectManagerBackupMessage *pDatabaseObjectManagerBackupMessage)
{
    // IMPORTANT :
    // This message will automically executes holdAll and takes a backup if requested to pause the service.  Another message to resume this service must arrive after to resume the database service.

    bool pauseTheService = pDatabaseObjectManagerBackupMessage->getPauseTheService ();

    if (true == pauseTheService)
    {
        holdAll ();
    }

    if (false == (DatabaseObjectManager::getIsDatabaseEnabled ()))
    {
        trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerBackupWorker::backupMessageHandler : No support for Persistent Store will be provided.");
        pDatabaseObjectManagerBackupMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pDatabaseObjectManagerBackupMessage);
        return;
    }

    trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerBackupWorker::backupMessageHandler : Backing up Database ...");

    string backupFileName = pDatabaseObjectManagerBackupMessage->getBackupFileName ();

    if ("" == backupFileName)
    {
        backupFileName = (DatabaseObjectManager::getDatabaseName ()) + ".pgd";
    }

    string commandString1 = string ("rm -rf ") + backupFileName;
    string addSchemaWaveStartInPgDump = "";

    if ( true ==  pDatabaseObjectManagerBackupMessage->getBackupAllSchema() )
    {
        addSchemaWaveStartInPgDump = string (" --schema=WaveStart ");
    }

    string dataOnlyBackup = "";

    if (pDatabaseObjectManagerBackupMessage->getDataOnlyBackup ())
    {
        dataOnlyBackup = string (" --data-only "); 
    }

    string commandString2 = string (" pg_dump --schema=WaveCurrent ") + addSchemaWaveStartInPgDump + dataOnlyBackup + string(" --format=c --file=") + backupFileName + string (" --port=") + DatabaseObjectManager::getDatabasePort () + string (" ") + DatabaseObjectManager::getDatabaseName ();

    SI32            returnStatus = -1;
    vector <string> output;

    returnStatus = FrameworkToolKit::systemCommandOutput (commandString1, output);

    if (0 != returnStatus)
    {
        if (0 < (output.size ()))
        {
            trace (TRACE_LEVEL_WARN, string ("DatabaseObjectManagerBackupWorker::backupMessageHandler :Failed to remove the old backup file with err no ") + returnStatus + string (" failed error message") + output[0]);
            output.clear ();
        }
        else
        {
            trace (TRACE_LEVEL_WARN, string ("DatabaseObjectManagerBackupWorker::backupMessageHandler :Failed to remove the old backup file with err no ") + returnStatus);
        }
    }

    returnStatus = FrameworkToolKit::systemCommandOutput (commandString2, output);

    if (0 != returnStatus)
    {
        if (0 < (output.size ()))
        {
            trace (TRACE_LEVEL_WARN, string ("DatabaseObjectManagerBackupWorker::backupMessageHandler :Failed to backup database with err no ") + returnStatus + string (" failed error message") + output[0]);
        }
        else
        {
            trace (TRACE_LEVEL_WARN, string ("DatabaseObjectManagerBackupWorker::backupMessageHandler :Failed to backup database with err no ") + returnStatus);
        }
    }

    // IMPORTANT :
    // If not returning success, and pausing was requested then unholdAll and return.

    pDatabaseObjectManagerBackupMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pDatabaseObjectManagerBackupMessage);
}

}
