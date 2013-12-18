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

#include "Framework/Database/DatabaseObjectManagerRestoreWorker.h"
#include "Framework/Database/DatabaseObjectManagerRestoreMessage.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "SystemManagement/WaveSystemManagementGateway.h"

namespace WaveNs
{

DatabaseObjectManagerRestoreWorker::DatabaseObjectManagerRestoreWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (DATABASE_OBJECT_MANAGER_RESTORE, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerRestoreWorker::restoreMessageHandler));
}

DatabaseObjectManagerRestoreWorker::~DatabaseObjectManagerRestoreWorker ()
{
}

void DatabaseObjectManagerRestoreWorker::restoreMessageHandler (DatabaseObjectManagerRestoreMessage *pDatabaseObjectManagerRestoreMessage)
{
    ResourceId restoreStatus = WAVE_MESSAGE_SUCCESS;

    if (false == (DatabaseObjectManager::getIsDatabaseEnabled ()))
    {
        trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerRestoreWorker::restoreMessageHandler : No support for Persistent Store will be provided.");
        pDatabaseObjectManagerRestoreMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pDatabaseObjectManagerRestoreMessage);
        return;
    }

    trace (TRACE_LEVEL_DEBUG, "DatabaseObjectManagerRestoreWorker::restoreMessageHandler : Restoring Database ...");

    string fileToRestoreFrom = pDatabaseObjectManagerRestoreMessage->getFileToRestoreFrom ();

    if ("" == fileToRestoreFrom)
    {
        fileToRestoreFrom = (DatabaseObjectManager::getDatabaseName ()) + ".pgd";
    }

    string commandString;

    if ( true == pDatabaseObjectManagerRestoreMessage->getRestoreSchema() )
    {
        commandString = string (" pg_restore --create --format=c --dbname=")  + (DatabaseObjectManager::getDatabaseName ()) + string (" --disable-triggers  --port=") + DatabaseObjectManager::getDatabasePort () + string (" ") + fileToRestoreFrom;
    }
    else if (pDatabaseObjectManagerRestoreMessage->getDataOnlyRestore ())
    {
        commandString = string (" pg_restore --data-only --single-transaction --format=c --dbname=") + (DatabaseObjectManager::getDatabaseName ()) + string (" --port=") + DatabaseObjectManager::getDatabasePort () + string (" ") + fileToRestoreFrom;
    }
    else
    {
        commandString = string (" pg_restore --clean --single-transaction --format=c --dbname=") + (DatabaseObjectManager::getDatabaseName ()) + string (" --port=") + DatabaseObjectManager::getDatabasePort () + string (" ") + fileToRestoreFrom;
    }

    // set DBRestoreIncomplete to true before starting pg_restore operation
    FrameworkToolKit::setIsDbRestoreIncomplete (true);

    if (pDatabaseObjectManagerRestoreMessage->getSaveConfiguration ())
    {
        ResourceId configSaveStatus = FrameworkToolKit::savePrismConfiguration ();

        if (WAVE_MESSAGE_SUCCESS != configSaveStatus)
        {   
            trace (TRACE_LEVEL_ERROR, string("DatabaseObjectManagerRestoreWorker::restoreMessageHandler : failed save configuration for DbRestoreComplete. Status : ") + FrameworkToolKit::localize(configSaveStatus));
        }
    }

    SI32 status  = system (commandString.c_str ());

    if (0 != status)
    {
        restoreStatus = WAVE_MESSAGE_ERROR_DB_RESTORE_FAILED;
    }
    else
    {
        FrameworkToolKit::setIsDbRestoreIncomplete (false);

        ResourceId configSaveStatus = FrameworkToolKit::savePrismConfiguration ();

        if (WAVE_MESSAGE_SUCCESS != configSaveStatus)
        {   
            trace (TRACE_LEVEL_ERROR, string("DatabaseObjectManagerRestoreWorker::restoreMessageHandler : failed to save configuration for DbRestoreComplete. Status : ") + FrameworkToolKit::localize(configSaveStatus));
        }   
    }

    // Clear the System Management cache contents     
    WaveSystemManagementGatewayWorker::clearTargetNodeNameObjectIdMap ();

    pDatabaseObjectManagerRestoreMessage->setCompletionStatus (restoreStatus);
    reply (pDatabaseObjectManagerRestoreMessage);
}

}
