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

#include "Framework/Database/DatabaseObjectManagerEmptyWorker.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryWorker.h"
#include "Framework/Database/DatabaseObjectManagerEmptyMessage.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseConnection.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/DistributedLog/DistributedLogEntryManagedObject.h"

namespace WaveNs
{

DatabaseObjectManagerEmptyWorker::DatabaseObjectManagerEmptyWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (DATABASE_OBJECT_MANAGER_EMPTY, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerEmptyWorker::emptyMessageHandler));
    addOperationMap (DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_PARAMETER_CONFIGURE, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerEmptyWorker::emptyDatabaseParameterConfigureMessageHandler));
}

DatabaseObjectManagerEmptyWorker::~DatabaseObjectManagerEmptyWorker ()
{
}

PrismMessage *DatabaseObjectManagerEmptyWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {   
        case DATABASE_OBJECT_MANAGER_EMPTY :
            pPrismMessage = new DatabaseObjectManagerEmptyMessage (); 
            break;
 
        case DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_PARAMETER_CONFIGURE :
            pPrismMessage = new DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage ();
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("DatabaseObjectManagerEmptyWorker::createMessageInstance : Unknown operation code : ") + operationCode);
            prismAssert (false, __FILE__, __LINE__);
    }   

    return (pPrismMessage);
}

void DatabaseObjectManagerEmptyWorker::emptyMessageHandler (DatabaseObjectManagerEmptyMessage *pDatabaseObjectManagerEmptyMessage)
{
    if (false == (DatabaseObjectManager::getIsDatabaseEnabled ()))
    {
        trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerEmptyWorker::emptyMessageHandler : No support for Persistent Store will be provided.");
        pDatabaseObjectManagerEmptyMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pDatabaseObjectManagerEmptyMessage);
        return;
    }

    ResourceId              status                 = WAVE_MESSAGE_ERROR;
    DatabaseConnection     *pDatabaseConnection    = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    PGresult               *pResult                = NULL;
    ExecStatusType          databaseStatus         = PGRES_FATAL_ERROR;
    PGconn                 *pConnection            = pDatabaseConnection->getPConnection ();
    OrmRepository          *pOrmRepository         = OrmRepository::getInstance ();
    string                  waveSchema             = OrmRepository::getWaveCurrentSchema ();
    string                  sqlToEmptyOrmDatabase;
    
    prismAssert (NULL != pConnection, __FILE__, __LINE__);
    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    set<string> schemasToBeEmptied = pDatabaseObjectManagerEmptyMessage->getSchemasToBeEmptied ();
    UI32 emptyType = DatabaseObjectManager::determineDatabaseEmptyType (schemasToBeEmptied, pDatabaseObjectManagerEmptyMessage->getDatabaseEmptyType ());

    if (DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_DELETE == emptyType)
    {
        trace (TRACE_LEVEL_INFO, string ("DatabaseObjectManagerEmptyWorker::emptyMessageHandler is using delete"));

        if (schemasToBeEmptied.empty ())
        {
            if (pDatabaseObjectManagerEmptyMessage->getIsSelectivelyEmptyDatabase ())
            {
                sqlToEmptyOrmDatabase = pOrmRepository->generateSqlToSelectivelyEmptyOrmDatabase (waveSchema);
            }
            else
            {
                string sqlToTruncateDistributedLogEntries = pOrmRepository->getSqlToTruncateTable (DistributedLogEntryManagedObject::getClassName () , waveSchema);

                sqlToEmptyOrmDatabase += sqlToTruncateDistributedLogEntries;

                sqlToEmptyOrmDatabase += pOrmRepository->generateSqlToEmptyOrmDatabase (waveSchema);
            }
        }
        else
        {
            set<string>::iterator it; 

            for (it = schemasToBeEmptied.begin (); it != schemasToBeEmptied.end (); it++)
            {   
                if (pDatabaseObjectManagerEmptyMessage->getIsSelectivelyEmptyDatabase ())
                {
                    sqlToEmptyOrmDatabase += pOrmRepository->generateSqlToSelectivelyEmptyOrmDatabase (*it);
                }
                else
                {
                    string sqlToTruncateDistributedLogEntries = pOrmRepository->getSqlToTruncateTable (DistributedLogEntryManagedObject::getClassName () , *it);

                    sqlToEmptyOrmDatabase += sqlToTruncateDistributedLogEntries;

                    sqlToEmptyOrmDatabase += pOrmRepository->generateSqlToEmptyOrmDatabase (*it);
                }
            }
        }
    }
    else if (DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_TRUNCATE == emptyType)
    {
        trace (TRACE_LEVEL_INFO, string ("DatabaseObjectManagerEmptyWorker::emptyMessageHandler is using truncate"));

        if (schemasToBeEmptied.empty ())
        {
            if (pDatabaseObjectManagerEmptyMessage->getIsSelectivelyEmptyDatabase ())
            {
                sqlToEmptyOrmDatabase = pOrmRepository->generateSqlToSelectivelyEmptyOrmDatabaseUsingTruncate (waveSchema);
            }
            else
            {
                sqlToEmptyOrmDatabase = pOrmRepository->generateSqlToEmptyOrmDatabaseUsingTruncate (waveSchema);
            }
        }
        else
        {
            set<string>::iterator it;

            for (it = schemasToBeEmptied.begin (); it != schemasToBeEmptied.end (); it++)
            {
                if (pDatabaseObjectManagerEmptyMessage->getIsSelectivelyEmptyDatabase ())
                {
                    sqlToEmptyOrmDatabase += pOrmRepository->generateSqlToSelectivelyEmptyOrmDatabaseUsingTruncate (*it);
                }
                else
                {
                    sqlToEmptyOrmDatabase += pOrmRepository->generateSqlToEmptyOrmDatabaseUsingTruncate (*it);
                }
            }
        }
    }

    if (true == pDatabaseObjectManagerEmptyMessage->getIsReturnSqlForEmptyDatabase ())
    {
        pDatabaseObjectManagerEmptyMessage->setSqlForEmpty (sqlToEmptyOrmDatabase);
    }

    pResult = PQexec (pDatabaseConnection->getPConnection (), sqlToEmptyOrmDatabase.c_str ());

    if (NULL == pResult)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        databaseStatus = PQresultStatus (pResult);

        if (PGRES_COMMAND_OK == databaseStatus)
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
            string databaseErrorMessage = PQresultErrorMessage (pResult);
            trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerEmptyWorker::emptyMessageHandler : ERROR in executing the Database Transaction :");
            trace (TRACE_LEVEL_ERROR, databaseErrorMessage);
            trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + sqlToEmptyOrmDatabase);

            DatabaseObjectManager::handleIfDBCorruption (databaseErrorMessage, pResult);
        }

        PQclear (pResult);
    }

    pDatabaseObjectManagerEmptyMessage->setCompletionStatus (status);
    reply (pDatabaseObjectManagerEmptyMessage);
}

void DatabaseObjectManagerEmptyWorker::emptyDatabaseParameterConfigureMessageHandler (DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage *pDatabaseObjectManagerEmptyDatabaseParameterConfigureMessage)
{
    UI32                    thresholdValue            = pDatabaseObjectManagerEmptyDatabaseParameterConfigureMessage->getEmptyDatabaseAutoDetectionThresholdValue ();
    UI32                    numberOfEntriesInDatabase = 0;
    DatabaseObjectManager  *pDatabaseObjectManager    = dynamic_cast<DatabaseObjectManager *> (DatabaseObjectManager::getInstance ());
    string                  waveSchema                = OrmRepository::getWaveCurrentSchema ();

    if (0 == thresholdValue)
    {
        pDatabaseObjectManagerEmptyDatabaseParameterConfigureMessage->setEmptyDatabaseAutoDetectionThresholdValue (DatabaseObjectManager::getEmptyDatabaseAutoDetectionThresholdValue ());
        
        numberOfEntriesInDatabase = pDatabaseObjectManager->getTotalNumberOfEntriesInAllTableFromDatabase (waveSchema); 

        pDatabaseObjectManagerEmptyDatabaseParameterConfigureMessage->setNumberOfEntriesInDatabase (numberOfEntriesInDatabase);

    }
    else
    {
        DatabaseObjectManager::setEmptyDatabaseAutoDetectionThresholdValue (thresholdValue);
    }

    pDatabaseObjectManagerEmptyDatabaseParameterConfigureMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pDatabaseObjectManagerEmptyDatabaseParameterConfigureMessage);
}

}
