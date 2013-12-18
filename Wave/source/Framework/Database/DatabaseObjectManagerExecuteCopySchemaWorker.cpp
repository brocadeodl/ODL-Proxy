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

#include "Framework/Database/DatabaseObjectManagerExecuteCopySchemaWorker.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Database/DatabaseObjectManagerExecuteCopySchemaMessage.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Database/DatabaseConnection.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"

namespace WaveNs
{

DatabaseObjectManagerExecuteCopySchemaWorker::DatabaseObjectManagerExecuteCopySchemaWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (DATABASE_OBJECT_MANAGER_EXECUTE_COPY_SCHEMA, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerExecuteCopySchemaWorker::executeCopySchemaMessageHandler));

    // This is the order in which the schema tables needs to be copied
    m_tableType.push_back (ORM_DERIVATIONS_TABLE_TYPE);
    m_tableType.push_back (ORM_TABLE_TYPE);
    m_tableType.push_back (ORM_AUXILLIARY_TABLE_TYPE);
}

DatabaseObjectManagerExecuteCopySchemaWorker::~DatabaseObjectManagerExecuteCopySchemaWorker ()
{
}

PrismMessage *DatabaseObjectManagerExecuteCopySchemaWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case DATABASE_OBJECT_MANAGER_EXECUTE_COPY_SCHEMA :
            pPrismMessage = new DatabaseObjectManagerExecuteCopySchemaMessage ();
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("DatabaseObjectManagerExecuteCopySchemaWorker::createMessageInstance : Unknown operation code : ") + operationCode);
            prismAssert (false, __FILE__, __LINE__);
    }

    return (pPrismMessage);
}

void DatabaseObjectManagerExecuteCopySchemaWorker::executeCopySchemaMessageHandler (DatabaseObjectManagerExecuteCopySchemaMessage *pDatabaseObjectManagerExecuteCopySchemaMessage)
{
    if (false == (DatabaseObjectManager::getIsDatabaseEnabled ()))
    {
        trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteCopySchemaWorker::executeCopySchemaMessageHandler : No support for Persistent Store will be provided.");
        pDatabaseObjectManagerExecuteCopySchemaMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pDatabaseObjectManagerExecuteCopySchemaMessage);
        return;
    }

    ResourceId          status              = WAVE_MESSAGE_SUCCESS;
    DatabaseConnection *pDatabaseConnection = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    PGresult           *pCopyResult	    = NULL;
    ExecStatusType      databaseStatus      = PGRES_FATAL_ERROR;
    PGconn             *pConnection         = pDatabaseConnection->getPConnection ();
    vector<string>      tables;
    vector<string>      derivationsTables;
    vector<string>      auxilliaryTables;
    string              sourceSchema        = pDatabaseObjectManagerExecuteCopySchemaMessage->getSourceSchema ();
    string              destSchema          = pDatabaseObjectManagerExecuteCopySchemaMessage->getDestSchema ();

    prismAssert (NULL != pConnection, __FILE__, __LINE__);

    SI32           i                     = 0;
    string         sqlForCopy		 = "";
    string	   sqlForDelete		 = "";
    UI32           numberOfReferences    = 0;
    UI32            t                    = 0;

    trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerExecuteCopySchemaWorker::executeCopySchemaMessageHandler: Selective copying from " + sourceSchema + " to " + destSchema);

    /////////////////////////////////////////////////////////////////////////////
    // Always perform a selectively copy for both copy start to running and 
    // running to start 
    /////////////////////////////////////////////////////////////////////////////
    bool checkEmptyNeeded = true;

    string copySqlForAllTables = pDatabaseObjectManagerExecuteCopySchemaMessage->getPrepareString ();


    set<string> schemasToBeEmptied;
    schemasToBeEmptied.insert (destSchema);
    UI32 emptyType = DatabaseObjectManager::determineDatabaseEmptyType (schemasToBeEmptied, DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_AUTO_DETECTION);
    if (DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_DELETE == emptyType)
    {
        trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerExecuteCopySchemaWorker::executeCopySchemaMessageHandler: Emptying DB using Delete.");
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerExecuteCopySchemaWorker::executeCopySchemaMessageHandler: Emptying DB using Truncate.");
    }
    /////////////////////////////////////////////////////////////////////////////
    // Iterate through the different table types in order and get the number of 
    // respective tables
    /////////////////////////////////////////////////////////////////////////////

    for (t = 0; t < m_tableType.size (); t++)
    {
        switch (m_tableType[t])
        {
            case ORM_DERIVATIONS_TABLE_TYPE:
            {
                derivationsTables = (OrmRepository::getInstance ())->getDerivationsTableNames (checkEmptyNeeded);
                numberOfReferences = derivationsTables.size ();
                break;
            }
            case ORM_TABLE_TYPE:
            {
                tables = (OrmRepository::getInstance ())->getTableNames (checkEmptyNeeded);
                numberOfReferences = tables.size ();
                break;
            }
            case ORM_AUXILLIARY_TABLE_TYPE:
            {
                auxilliaryTables = (OrmRepository::getInstance ())->getAuxilliaryTableNames (checkEmptyNeeded);
                numberOfReferences = auxilliaryTables.size ();
                break;
            }
            default:
                prismAssert(false, __FILE__, __LINE__);
        }

        /////////////////////////////////////////////////////////////////////////////
        // Now iterate through the tables and construct copy query
        /////////////////////////////////////////////////////////////////////////////

        for (i = 0; i < ((SI32) numberOfReferences); i++)
        {

        switch (m_tableType[t])
        {
            case ORM_DERIVATIONS_TABLE_TYPE:
            {
                if (DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_DELETE == emptyType)
                {
                    sqlForDelete += string (OrmRepository::getSqlToDeleteAllEntriesFromDerivationsTable (derivationsTables[i], destSchema));
                }
                else
                {
                    sqlForDelete += string (OrmRepository::getSqlToTruncateFromDerivationsTable (derivationsTables[i], destSchema));
                }
                sqlForCopy += string (OrmRepository::getSqlToCopyDerivationsTable (derivationsTables[i], sourceSchema, destSchema));

                break;
            }
            case ORM_TABLE_TYPE:
            {
                if (DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_DELETE == emptyType)
                {
                    sqlForDelete += string ((OrmRepository::getInstance ())->getSqlToDeleteAllEntriesFromTable (tables[i], destSchema));
                }
                else
                {
                    sqlForDelete += string ((OrmRepository::getInstance ())->getSqlToTruncateTable (tables[i], destSchema));
                }
                sqlForCopy += string ((OrmRepository::getInstance ())->getSqlToCopyTable (tables[i], sourceSchema, destSchema));

                break;
            }
            case ORM_AUXILLIARY_TABLE_TYPE:
            {
                if (DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_DELETE == emptyType)
                {
		            sqlForDelete += string (OrmRepository::getSqlToDeleteAllEntriesFromAuxilliaryTable (auxilliaryTables[i], destSchema));
                }
                else
                {
		            sqlForDelete += string (OrmRepository::getSqlToTruncateFromAuxilliaryTable (auxilliaryTables[i], destSchema));
                }
		        sqlForCopy += string (OrmRepository::getSqlToCopyAuxilliaryTable (auxilliaryTables[i], sourceSchema, destSchema));

                break;
            }
            default:
                prismAssert(false, __FILE__, __LINE__);
        }
	
	}
    }
    copySqlForAllTables += sqlForDelete + sqlForCopy;

    ////////////////////////////////////////////////////////////////////////////////////////
    // Run the copy query
    ////////////////////////////////////////////////////////////////////////////////////////
    trace (TRACE_LEVEL_DEBUG, string ("copySqlForAllTables to execute : \n") + copySqlForAllTables);

    pCopyResult = PQexec (pDatabaseConnection->getPConnection (), copySqlForAllTables.c_str ());

    databaseStatus = PQresultStatus (pCopyResult);

    if (PGRES_COMMAND_OK != databaseStatus)
    {
        string  errorString   = PQresultErrorMessage (pCopyResult);
/*
        string  stringToFind  = "HINT:  Please REINDEX it";
        size_t  found         = errorString.find (stringToFind);

        if (found != string::npos)
        {
*/
            trace (TRACE_LEVEL_ERROR, errorString);
            trace (TRACE_LEVEL_WARN, "Need to REINDEX the WaveDatabase");

            string     stringToReIndex = "REINDEX DATABASE \"WaveDatabase\";";
            PGresult  *pCopyResultForReIndexing = NULL;
    
            pCopyResultForReIndexing = PQexec (pDatabaseConnection->getPConnection (), stringToReIndex.c_str ());
            databaseStatus           = PQresultStatus (pCopyResultForReIndexing);
  
            if (PGRES_COMMAND_OK != databaseStatus)
            {
                status = WAVE_MESSAGE_ERROR;
                string errorMessage = string (PQresultErrorMessage (pCopyResultForReIndexing));
                trace (TRACE_LEVEL_ERROR, errorMessage);
            
                DatabaseObjectManager::handleIfDBCorruption (errorMessage, pCopyResultForReIndexing);

                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                PQclear (pCopyResult);

                pCopyResult     = PQexec (pDatabaseConnection->getPConnection (), copySqlForAllTables.c_str ());
                databaseStatus  = PQresultStatus (pCopyResult);

                if (PGRES_COMMAND_OK != databaseStatus)
                {
	                status = WAVE_MESSAGE_ERROR;
                    string errorMessage = string (PQresultErrorMessage (pCopyResult));
	                trace (TRACE_LEVEL_ERROR, errorMessage);
	                trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + copySqlForAllTables);

                    DatabaseObjectManager::handleIfDBCorruption (errorMessage, pCopyResult);

	                prismAssert (false, __FILE__, __LINE__);
                }
            }

            PQclear (pCopyResultForReIndexing);
/*        }
        else
        {
            status = WAVE_MESSAGE_ERROR;

	        trace (TRACE_LEVEL_ERROR, errorString);
	        trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + copySqlForAllTables);

            DatabaseObjectManager::handleIfDBCorruption (errorString, pCopyResult);

    	    prismAssert (false, __FILE__, __LINE__);
        }
*/
    }

    PQclear (pCopyResult);

    trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerExecuteCopySchemaWorker::executeCopySchemaMessageHandler: done copying schema\n");

    pDatabaseObjectManagerExecuteCopySchemaMessage->setCompletionStatus (status);
    reply (pDatabaseObjectManagerExecuteCopySchemaMessage);

}

}
