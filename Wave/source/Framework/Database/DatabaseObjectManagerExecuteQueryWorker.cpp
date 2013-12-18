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

#include "Framework/Database/DatabaseObjectManagerExecuteQueryWorker.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryMessage.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryMessageForCount.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Database/DatabaseConnection.h"
#include "Framework/Database/DatabaseMultiConnection.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseStatisticsTracker.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/Database/DatabaseStandaloneTransaction.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForDebugInformation.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

bool    s_useRecursiveQuery = true;
UI32    s_batchSize         = 200;

// Following line is added only for debuging. Not to be checked in as uncommented.
// #define PQexec(a,b) PQexec(a,b);trace (TRACE_LEVEL_SUCCESS, string ("SQL [") + b + string ("]"))

DatabaseObjectManagerExecuteQueryWorker::DatabaseObjectManagerExecuteQueryWorker (WaveObjectManager *pWaveObjectManager, bool isMultiDatabaseMode)
    : WaveWorker            (pWaveObjectManager),
      m_isMultiDatabaseMode (isMultiDatabaseMode)
{
    addOperationMap (DATABASE_OBJECT_MANAGER_EXECUTE_QUERY, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler));
    addOperationMap (DATABASE_OBJECT_MANAGER_EXECUTE_QUERY_FOR_COUNT, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandlerForCount));
    addOperationMap (DATABASE_OBJECT_MANAGER_DEBUG_CHANGE_QUERY_SETTINGS, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerExecuteQueryWorker::debugChangeQuerySettingsMessageHandler));
    addOperationMap (DATABASE_OBJECT_MANAGER_GET_DB_SCHEMA_INFO, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerExecuteQueryWorker::getDBSchemaInfoMessageHandler));
    addOperationMap (DATABASE_OBJECT_MANAGER_DROP_DB_VIEWS, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerExecuteQueryWorker::dropAllDbViewsMessageHandler));
}

DatabaseObjectManagerExecuteQueryWorker::~DatabaseObjectManagerExecuteQueryWorker ()
{
}

PrismMessage *DatabaseObjectManagerExecuteQueryWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case DATABASE_OBJECT_MANAGER_DEBUG_CHANGE_QUERY_SETTINGS :
            pPrismMessage = new DatabaseObjectManagerDebugChangeQuerySettingsMessage ();
            break;

        case DATABASE_OBJECT_MANAGER_GET_DB_SCHEMA_INFO :
            pPrismMessage = new DatabaseObjectManagerGetDBSchemaInfoMessage ();
            break;

        case DATABASE_OBJECT_MANAGER_DROP_DB_VIEWS :
            pPrismMessage = new DatabaseObjectManagerDropAllDBViewsMessage ();
            break;

        default :
            pPrismMessage = NULL;
    }

    return pPrismMessage;
}

ResourceId DatabaseObjectManagerExecuteQueryWorker::getCountFromDatabaseTableForAGivenSql (const string &sqlString, UI32 &count, const string &className)
{

    ResourceId          status              = WAVE_MESSAGE_SUCCESS;
    PGresult           *pResult             = NULL;
    ExecStatusType      databaseStatus      = PGRES_FATAL_ERROR;
    UI32                countValue          = 0;
    PGconn             *pConnection         = NULL;

    if ("" == className)
    {
        DatabaseConnection *pDatabaseConnection    = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
        pConnection = pDatabaseConnection->getPConnection ();
    }
    else
    {    
        pConnection = getDatabaseServerConnection (className);
    }

    prismAssert (NULL != pConnection, __FILE__, __LINE__);

    //trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandlerForCount : Query to execute : \n\r" + pDatabaseObjectManagerExecuteQueryMessageForCount->getSql ());

    pResult = PQexec (pConnection, sqlString.c_str ());

    if (NULL == pResult)
    {
        status = WAVE_MESSAGE_ERROR_QUERY_TO_DATABASE_FAILED;
    }
    else
    {
        databaseStatus = PQresultStatus (pResult);

        if (PGRES_TUPLES_OK == databaseStatus)
        {
                    status           = WAVE_MESSAGE_SUCCESS;
            UI32    numberOfRows     = PQntuples (pResult);
            SI32    column           = PQfnumber (pResult, "count");

            prismAssert (1 == numberOfRows, __FILE__, __LINE__);

            if (-1 != column)
            {
                char *pValue = PQgetvalue (pResult, 0, column);
                countValue = strtoul (pValue, NULL, 10);
            }
            else
            {
                status = WAVE_MESSAGE_ERROR_COLUMN_IN_DATABASE_NOT_FOUND;
            }

            PQclear (pResult);

        }
        else
        {    
            status = WAVE_MESSAGE_ERROR_DATABASE_RESULT_STATUS_FAILED;

            string errorMessage = string (PQresultErrorMessage (pResult));
            DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);

            PQclear (pResult);
        }

    }

    count  = countValue;

    return (status);
}

void DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandlerForCount (DatabaseObjectManagerExecuteQueryMessageForCount *pDatabaseObjectManagerExecuteQueryMessageForCount)
{
    if (true == (DatabaseObjectManager::getIsDatabaseShuttingDownOnReboot ()))
    {
        DatabaseObjectManager::goOnInfiniteLoopBeforeReboot ();
    }

    if ((false == (DatabaseObjectManager::getIsDatabaseEnabled ())) && (false == m_isMultiDatabaseMode))
    {
        trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler : No support for Persistent Store will be provided.");
        pDatabaseObjectManagerExecuteQueryMessageForCount->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pDatabaseObjectManagerExecuteQueryMessageForCount);
        return;
    }

    ResourceId status    = WAVE_MESSAGE_SUCCESS;
    UI32       count     = 0;
    string     className = pDatabaseObjectManagerExecuteQueryMessageForCount->getClassName ();
 
    status = getCountFromDatabaseTableForAGivenSql (pDatabaseObjectManagerExecuteQueryMessageForCount->getSql (), count, className);

    pDatabaseObjectManagerExecuteQueryMessageForCount->setCount (count);

    pDatabaseObjectManagerExecuteQueryMessageForCount->setCompletionStatus (status);
    reply (pDatabaseObjectManagerExecuteQueryMessageForCount);

}

void DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler (DatabaseObjectManagerExecuteQueryMessage *pDatabaseObjectManagerExecuteQueryMessage)
{
    if (true == (DatabaseObjectManager::getIsDatabaseShuttingDownOnReboot ()))
    {
        DatabaseObjectManager::goOnInfiniteLoopBeforeReboot ();
    }
    
    vector<WaveManagedObject *> *pResults = new vector<WaveManagedObject *>;

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    pDatabaseObjectManagerExecuteQueryMessage->setPResults (pResults);

    if ((false == (DatabaseObjectManager::getIsDatabaseEnabled ())) && (false == m_isMultiDatabaseMode))
    {
        trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler : No support for Persistent Store will be provided.");
        pDatabaseObjectManagerExecuteQueryMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pDatabaseObjectManagerExecuteQueryMessage);
        return;
    }

    ResourceId          status              = WAVE_MESSAGE_ERROR;
    PGresult           *pResult             = NULL;
    ExecStatusType      databaseStatus      = PGRES_FATAL_ERROR;
    PGconn             *pConnection         = getDatabaseServerConnection (pDatabaseObjectManagerExecuteQueryMessage->getClassName ());
    string              schema              = pDatabaseObjectManagerExecuteQueryMessage->getSchema ();

    prismAssert (NULL != pConnection, __FILE__, __LINE__);

    //trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler : Query to execute : \n\r" + pDatabaseObjectManagerExecuteQueryMessage->getSql ());

    if (true == s_useRecursiveQuery)
    {
        if (false == m_isMultiDatabaseMode)
        {
            string classNameReceivedFromMessage = pDatabaseObjectManagerExecuteQueryMessage->getClassName ();
            RecursiveQueryContext recursiveQueryContext;
            recursiveQueryContext.m_pWaveObjectManager              = WaveObjectManager::getOwnerForManagedClass (classNameReceivedFromMessage);
            recursiveQueryContext.m_pRequestingWaveObjectManager    = pDatabaseObjectManagerExecuteQueryMessage->getPWaveObjectManager ();
            recursiveQueryContext.m_schemaToQuery                   = schema;
            recursiveQueryContext.m_loadOneToManyRelationShips      = pDatabaseObjectManagerExecuteQueryMessage->getLoadOneToManyRelationships ();
            recursiveQueryContext.m_loadCompositions                = pDatabaseObjectManagerExecuteQueryMessage->getLoadCompositions ();
            string              sqlStringReceived                   = pDatabaseObjectManagerExecuteQueryMessage->getSql ();
            vector<string>      selectFieldsInManagedObject         = pDatabaseObjectManagerExecuteQueryMessage->getSelectFieldsInManagedObject ();
            vector<WaveManagedObject *> waveMOVector;
            loadWaveManagedObjects (recursiveQueryContext, classNameReceivedFromMessage, sqlStringReceived, selectFieldsInManagedObject, waveMOVector);
            (*pResults) = waveMOVector;
            pDatabaseObjectManagerExecuteQueryMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            reply (pDatabaseObjectManagerExecuteQueryMessage);
            return;
        }
    }

    pResult = PQexec (pConnection, (pDatabaseObjectManagerExecuteQueryMessage->getSql ()).c_str ());

    DatabaseStatisticsTracker::incrementDirectQuery (pDatabaseObjectManagerExecuteQueryMessage->getClassName ());

    if (NULL == pResult)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        databaseStatus = PQresultStatus (pResult);

        if (PGRES_TUPLES_OK == databaseStatus)
        {
            status = WAVE_MESSAGE_SUCCESS;

            //trace (TRACE_LEVEL_INFO, string ("DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler : Got ") + PQntuples (pResult) + string (" results."));

            UI32                     numberOfResults              = PQntuples (pResult);
            UI32                     i                            = 0;
            WaveObjectManager       *pRequestingWaveObjectManager = pDatabaseObjectManagerExecuteQueryMessage->getPWaveObjectManager ();
            string                   className                    = pDatabaseObjectManagerExecuteQueryMessage->getClassName ();
            WaveObjectManager       *pWaveObjectManager           = WaveObjectManager::getOwnerForManagedClass (className);
            WaveManagedObject       *pWaveManagedObject           = NULL;
            vector<string>           auxilliaryTableNames;
            vector<OrmRelationUmlType>  auxilliaryTableUmlTypes;
            UI32                     numberOfAuxilliaryTables     = 0;
            ObjectId                 tempObjectId;
            UI32                     tempClassId;
            UI64                     tempInstanceId;
            UI32                     j;
            string                   auxilliarySqlPrefix;
            string                   auxilliarySqlPostfix;
            string                   auxilliarySql;
            PGresult                *pAuxilliaryResult            = NULL;
            map<string, PGresult *>  auxilliaryResultsMap;
            ExecStatusType           auxilliaryDatabaseStatus     = PGRES_FATAL_ERROR;
            vector<string>           selectFieldsInManagedObject  = pDatabaseObjectManagerExecuteQueryMessage->getSelectFieldsInManagedObject ();
            //UI32                     numberOfSelectFields         = selectFieldsInManagedObject.size ();
            bool                     loadOneToManyRelationships   = pDatabaseObjectManagerExecuteQueryMessage->getLoadOneToManyRelationships ();
            bool                     loadCompositions             = pDatabaseObjectManagerExecuteQueryMessage->getLoadCompositions ();


            if (NULL == pWaveObjectManager)
            {
                trace (TRACE_LEVEL_FATAL, "DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler : There is no known Owner for Managed Class : " + className);
                prismAssert (false, __FILE__, __LINE__);

                pDatabaseObjectManagerExecuteQueryMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
                reply (pDatabaseObjectManagerExecuteQueryMessage);
            }

            if (true == loadOneToManyRelationships)
            {
                // Obtain if there are any auxilliary table Names based on the selected fields.  If there are no selected fields, then all auxiliary tables will be given.

                OrmRepository::getAllAuxilliaryTableNames (className, auxilliaryTableNames, auxilliaryTableUmlTypes, selectFieldsInManagedObject);
                numberOfAuxilliaryTables = auxilliaryTableNames.size ();
            }

            DatabaseStatisticsTracker::incrementManagedObjectsLoadedFromDirectQuery (className, numberOfResults);

            for (i = 0; i < numberOfResults; i++)
            {
                pWaveManagedObject = pWaveObjectManager->createManagedObjectInstanceWrapper (className);

                prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

                //trace (TRACE_LEVEL_INFO, string (" OBJECT ID : ") + ((pWaveManagedObject->getObjectId ()).getClassId ()) + string (", ") + (pWaveManagedObject->getObjectId ()).getInstanceId ());

                //trace (TRACE_LEVEL_INFO, string (" OBJECT ID : ") + ((pWaveManagedObject->getObjectId ()).getClassId ()) + string (", ") + (pWaveManagedObject->getObjectId ()).getInstanceId ());

                // Reset the Object Manager If required (This case will arise in case of a local ON delegating ownership tocorresponding global OM)

                //trace (TRACE_LEVEL_INFO, "Requesting OM : " + pRequestingWaveObjectManager->getName ());

                //trace (TRACE_LEVEL_INFO, "IS MO Supported for : " + className + " : " + (true == (pRequestingWaveObjectManager->isManagedClassSupported (className)) ? "True" : "False"));

                if ((NULL != pRequestingWaveObjectManager) && (true == (pRequestingWaveObjectManager->isManagedClassSupported (className))))
                {
                    pWaveManagedObject->setPWaveObjectManager (pRequestingWaveObjectManager);
                }

                pWaveManagedObject->setPCurrentOwnerWaveObjectManager (pRequestingWaveObjectManager);

                //trace (TRACE_LEVEL_DEVEL, string ("DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler : Query for Managed Class : ") + className + ", loadOneToManyRelationships : " + loadOneToManyRelationships + ", loadCompositions : " + loadCompositions);

                pWaveManagedObject->loadFromPostgresQueryResult (pResult, i, schema, selectFieldsInManagedObject, loadOneToManyRelationships, loadCompositions);

                pResults->push_back (pWaveManagedObject);

                if (true == loadOneToManyRelationships)
                {
                    if (0 < numberOfAuxilliaryTables)
                    {
                        // We will process 1 -> * Associations (unconditionally) and Compositions (conditionally).  If no select fields are specified then all 1->* relations will be preocessed.
                        // But, if any select fields are specified, only the 1->* many relationships that are part of the select fields will be processed.
                        // Now process the auxilliary table entries if there are any for this table.
                        
                        tempObjectId   = pWaveManagedObject->getObjectId ();
                        tempClassId    = tempObjectId.getClassId () ;
                        tempInstanceId = tempObjectId.getInstanceId ();

                        auxilliarySqlPrefix  = "SELECT relatedToClassId, relatedToInstanceId from ";
                        auxilliarySqlPostfix = string (" WHERE ownerClassId = ") + tempClassId + string (" AND ownerInstanceId = ") + tempInstanceId + string (";");

                        for (j = 0; j < numberOfAuxilliaryTables; j++)
                        {
                            if (ORM_RELATION_UML_TYPE_COMPOSITION == auxilliaryTableUmlTypes[j])
                            {
                                if (false == loadCompositions)
                                {
                                    //trace (TRACE_LEVEL_DEBUG, "DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler : Compositions detected, skipping auxillary table query.");
                                    continue;
                                }
                            }

                            auxilliarySql = auxilliarySqlPrefix + schema + "." + auxilliaryTableNames[j] + auxilliarySqlPostfix;

                            pAuxilliaryResult = PQexec (pConnection, auxilliarySql.c_str ());

                            DatabaseStatisticsTracker::incrementIndirectQuery (auxilliaryTableNames[j]);

                            if (NULL == pAuxilliaryResult)
                            {
                                trace (TRACE_LEVEL_FATAL, "DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler : Could not obtain results for auxilliary table " + auxilliaryTableNames[j] + " On table " + className + ".");
                                prismAssert (false, __FILE__, __LINE__);
                            }
                            else
                            {
                                auxilliaryDatabaseStatus = PQresultStatus (pAuxilliaryResult);

                                if (PGRES_TUPLES_OK != auxilliaryDatabaseStatus)
                                {
                                    string errorMessage = string (PQresultErrorMessage (pAuxilliaryResult));
                                    trace (TRACE_LEVEL_FATAL, "DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler : Could not obtain results for auxilliary table " + auxilliaryTableNames[j] + " On table " + className + ".");
                                    trace (TRACE_LEVEL_ERROR, errorMessage);
                                    trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + auxilliarySql);

                                    DatabaseObjectManager::handleIfDBCorruption (errorMessage, pAuxilliaryResult);

                                    prismAssert (false, __FILE__, __LINE__);
                                }
                                else
                                {
                                    auxilliaryResultsMap[auxilliaryTableNames[j]] = pAuxilliaryResult;
                                }
                            }
                        }

                        pWaveManagedObject->loadFromPostgresAuxilliaryQueryResult (auxilliaryResultsMap, schema, selectFieldsInManagedObject, loadCompositions);

                        // Now release all of the memory held by all of the auxilliary PGresult data structures.

                        map<string, PGresult *>::iterator element = auxilliaryResultsMap.begin ();
                        map<string, PGresult *>::iterator end     = auxilliaryResultsMap.end ();

                        while (element != end)
                        {
                            PQclear (element->second);
                            element++;
                        }
                    }
                }
            }
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
            string errorMessage = string (PQresultErrorMessage (pResult));
            trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler : ERROR in executing the Database Query :");
            trace (TRACE_LEVEL_ERROR, errorMessage);
            trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + pDatabaseObjectManagerExecuteQueryMessage->getSql ());

            DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);
        }

        PQclear (pResult);
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
    	trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteQueryWorker::executeQueryMessageHandler : Status : " + FrameworkToolKit::localize (status));
    }

    pDatabaseObjectManagerExecuteQueryMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pDatabaseObjectManagerExecuteQueryMessage);
}

WaveManagedObject *DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectFromDatabase (const ObjectId &waveManagedObjectObjectId, const string &schema, WaveObjectManager *pRequestingWaveObjectManager)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((false == (DatabaseObjectManager::getIsDatabaseEnabled ())) && (false == m_isMultiDatabaseMode))
    {
        trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectFromDatabase : No support for Persistent Store will be provided.");
        return (pWaveManagedObject);
    }

    ResourceId          status              = WAVE_MESSAGE_ERROR;
    PGresult           *pResult             = NULL;
    ExecStatusType      databaseStatus      = PGRES_FATAL_ERROR;
    string              tableName           = OrmRepository::getTableNameById (waveManagedObjectObjectId.getClassId ());
    PGconn             *pConnection         = getDatabaseServerConnection (tableName);
    string              sql                 = string ("SELECT * from ") + schema + "." + tableName + string (" WHERE ObjectIdClassId = ") + waveManagedObjectObjectId.getClassId () + string (" AND ObjectIdInstanceId = ") + waveManagedObjectObjectId.getInstanceId ();
    vector<string>      selectFields;

    prismAssert (NULL != pConnection, __FILE__, __LINE__);

    //trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectFromDatabase : Query to execute : \n" + sql);

    pResult = PQexec (pConnection, (sql.c_str ()));

    DatabaseStatisticsTracker::incrementIndirectQuery (tableName);

    if (NULL == pResult)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        databaseStatus = PQresultStatus (pResult);

        if (PGRES_TUPLES_OK == databaseStatus)
        {
            status = WAVE_MESSAGE_SUCCESS;

            //trace (TRACE_LEVEL_INFO, string ("DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectFromDatabase : Got ") + PQntuples (pResult) + string (" results."));

            UI32                     numberOfResults               = PQntuples (pResult);
            string                   className                     = tableName;
            WaveObjectManager       *pWaveObjectManager            = WaveObjectManager::getOwnerForManagedClass (className);
            vector<string>           auxilliaryTableNames;
            UI32                     numberOfAuxilliaryTables;
            ObjectId                 tempObjectId;
            UI32                     tempClassId;
            UI64                     tempInstanceId;
            UI32                     j;
            string                   auxilliarySqlPrefix;
            string                   auxilliarySqlPostfix;
            string                   auxilliarySql;
            PGresult                *pAuxilliaryResult             = NULL;
            map<string, PGresult *>  auxilliaryResultsMap;
            ExecStatusType           auxilliaryDatabaseStatus      = PGRES_FATAL_ERROR;

            if (NULL == pWaveObjectManager)
            {
                trace (TRACE_LEVEL_FATAL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectFromDatabase : There is no known Owner for Managed Class : " + className);
                prismAssert (false, __FILE__, __LINE__);

                return (pWaveManagedObject);
            }

            prismAssert (1 == numberOfResults, __FILE__, __LINE__);

            // Obtain if there are any auxilliary table Names.

            OrmRepository::getAllAuxilliaryTableNames (className, auxilliaryTableNames);
            numberOfAuxilliaryTables = auxilliaryTableNames.size ();

            pWaveManagedObject = pWaveObjectManager->createManagedObjectInstanceWrapper (className);

            prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

            //trace (TRACE_LEVEL_INFO, string (" OBJECT ID : ") + ((pWaveManagedObject->getObjectId ()).getClassId ()) + string (", ") + (pWaveManagedObject->getObjectId ()).getInstanceId ());

            //trace (TRACE_LEVEL_INFO, string (" OBJECT ID : ") + ((pWaveManagedObject->getObjectId ()).getClassId ()) + string (", ") + (pWaveManagedObject->getObjectId ()).getInstanceId ());

            // Reset the Object Manager If required (This case will arise in case of a local ON delegating ownership tocorresponding global OM)

            //trace (TRACE_LEVEL_INFO, "Requesting OM : " + pRequestingWaveObjectManager->getName ());

            //trace (TRACE_LEVEL_INFO, "IS MO Supported for : " + className + " : " + (true == (pRequestingWaveObjectManager->isManagedClassSupported (className)) ? "True" : "False"));

            if ((NULL != pRequestingWaveObjectManager) && (true == (pRequestingWaveObjectManager->isManagedClassSupported (className))))
            {
                pWaveManagedObject->setPWaveObjectManager (pRequestingWaveObjectManager);
            }

            pWaveManagedObject->setPCurrentOwnerWaveObjectManager (pRequestingWaveObjectManager);

            pWaveManagedObject->loadFromPostgresQueryResult (pResult, 0, schema, selectFields);

            DatabaseStatisticsTracker::incrementManagedObjectsLoadedFromIndirectQuery (className);

            if (0 < numberOfAuxilliaryTables)
            {
                // Now process the auxilliary table entries if there are any for this table.

                tempObjectId   = pWaveManagedObject->getObjectId ();
                tempClassId    = tempObjectId.getClassId () ;
                tempInstanceId = tempObjectId.getInstanceId ();

                auxilliarySqlPrefix  = "SELECT relatedToClassId, relatedToInstanceId from ";
                auxilliarySqlPostfix = string (" WHERE ownerClassId = ") + tempClassId + string (" AND ownerInstanceId = ") + tempInstanceId + string (";");

                for (j = 0; j < numberOfAuxilliaryTables; j++)
                {
                    auxilliarySql = auxilliarySqlPrefix + schema + "." + auxilliaryTableNames[j] + auxilliarySqlPostfix;

                    pAuxilliaryResult = PQexec (pConnection, auxilliarySql.c_str ());

                    DatabaseStatisticsTracker::incrementIndirectQuery (auxilliaryTableNames[j]);

                    if (NULL == pAuxilliaryResult)
                    {
                        trace (TRACE_LEVEL_FATAL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectFromDatabase : Could not obtain results for auxilliary table " + auxilliaryTableNames[j] + " On table " + className + ".");
                        prismAssert (false, __FILE__, __LINE__);
                    }
                    else
                    {
                        auxilliaryDatabaseStatus = PQresultStatus (pAuxilliaryResult);

                        if (PGRES_TUPLES_OK != auxilliaryDatabaseStatus)
                        {
                            string errorMessage = string (PQresultErrorMessage (pAuxilliaryResult));
                            trace (TRACE_LEVEL_FATAL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectFromDatabase : Could not obtain results for auxilliary table " + auxilliaryTableNames[j] + " On table " + className + ".");
                            trace (TRACE_LEVEL_ERROR, errorMessage);
                            trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + auxilliarySql);

                            DatabaseObjectManager::handleIfDBCorruption (errorMessage, pAuxilliaryResult);

                            prismAssert (false, __FILE__, __LINE__);
                        }
                        else
                        {
                            auxilliaryResultsMap[auxilliaryTableNames[j]] = pAuxilliaryResult;
                        }
                    }
                }

                pWaveManagedObject->loadFromPostgresAuxilliaryQueryResult (auxilliaryResultsMap, schema, vector<string> ());

                // Now release all of the memory held by all of the auxilliary PGresult data structures.

                map<string, PGresult *>::iterator element = auxilliaryResultsMap.begin ();
                map<string, PGresult *>::iterator end     = auxilliaryResultsMap.end ();

                while (element != end)
                {
                    PQclear (element->second);
                    element++;
                }
            }
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
            string errorMessage = string (PQresultErrorMessage (pResult));
            trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectFromDatabase : ERROR in executing the Database Query :");
            trace (TRACE_LEVEL_ERROR, errorMessage);
            trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + sql);

            DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);
        }

        PQclear (pResult);
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
    	trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectFromDatabase : Status : " + FrameworkToolKit::localize (status));
    }

    return (pWaveManagedObject);
}

vector<WaveManagedObject *> DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase (const vector<ObjectId> &waveManagedObjectObjectIds, const string &schema, WaveObjectManager *pRequestingWaveObjectManager)
{
    vector<WaveManagedObject *>  waveManagedObjects;
    WaveManagedObject           *pWaveManagedObject  = NULL;

    if ((false == (DatabaseObjectManager::getIsDatabaseEnabled ())) && (false == m_isMultiDatabaseMode))
    {
        trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : No support for Persistent Store will be provided.");
        return (waveManagedObjects);
    }

    const UI32 numberOfObjectIds = waveManagedObjectObjectIds.size ();

    if (0 == numberOfObjectIds)
    {
        return (waveManagedObjects);
    }

    ResourceId                                status                  = WAVE_MESSAGE_ERROR;
    PGresult                                 *pResult                 = NULL;
    ExecStatusType                            databaseStatus          = PGRES_FATAL_ERROR;
    string                                    tableName               = OrmRepository::getTableNameById ((waveManagedObjectObjectIds[0]).getClassId ());
    PGconn                                   *pConnection             = getDatabaseServerConnection (tableName);
    WaveManagedObjectSynchronousQueryContext  synchronousQueryContext (tableName);
    string                                    sql;
    UI32                                      i                       = 0;
    vector<string>                            selectFields;

    synchronousQueryContext.setSchemaToQuery (schema);
    synchronousQueryContext.addObjectIds     (waveManagedObjectObjectIds);

    sql = synchronousQueryContext.getSql ();

    prismAssert (NULL != pConnection, __FILE__, __LINE__);

    //trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectFromDatabase : Query to execute : \n" + sql);

    pResult = PQexec (pConnection, (sql.c_str ()));

    DatabaseStatisticsTracker::incrementIndirectQuery (tableName);

    if (NULL == pResult)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        databaseStatus = PQresultStatus (pResult);

        if (PGRES_TUPLES_OK == databaseStatus)
        {
            status = WAVE_MESSAGE_SUCCESS;

            //trace (TRACE_LEVEL_INFO, string ("DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : Got ") + PQntuples (pResult) + string (" results."));

            UI32                     numberOfResults               = PQntuples (pResult);
            string                   className                     = tableName;
            WaveObjectManager       *pWaveObjectManager            = WaveObjectManager::getOwnerForManagedClass (className);
            vector<string>           auxilliaryTableNames;
            UI32                     numberOfAuxilliaryTables;
            ObjectId                 tempObjectId;
            UI32                     tempClassId;
            UI64                     tempInstanceId;
            UI32                     j;
            string                   auxilliarySqlPrefix;
            string                   auxilliarySqlPostfix;
            string                   auxilliarySql;
            PGresult                *pAuxilliaryResult             = NULL;
            map<string, PGresult *>  auxilliaryResultsMap;
            ExecStatusType           auxilliaryDatabaseStatus      = PGRES_FATAL_ERROR;

            if (NULL == pWaveObjectManager)
            {
                trace (TRACE_LEVEL_FATAL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : There is no known Owner for Managed Class : " + className);
                prismAssert (false, __FILE__, __LINE__);

                return (waveManagedObjects);
            }

            prismAssert (numberOfObjectIds == numberOfResults, __FILE__, __LINE__);

            // Obtain if there are any auxilliary table Names.

            OrmRepository::getAllAuxilliaryTableNames (className, auxilliaryTableNames);
            numberOfAuxilliaryTables = auxilliaryTableNames.size ();

            DatabaseStatisticsTracker::incrementManagedObjectsLoadedFromIndirectQuery (className, numberOfResults);

            for (i = 0; i < numberOfResults; i++)
            {
                pWaveManagedObject = pWaveObjectManager->createManagedObjectInstanceWrapper (className);

                prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

                waveManagedObjects.push_back (pWaveManagedObject);

                //trace (TRACE_LEVEL_INFO, string (" OBJECT ID : ") + ((pWaveManagedObject->getObjectId ()).getClassId ()) + string (", ") + (pWaveManagedObject->getObjectId ()).getInstanceId ());

                //trace (TRACE_LEVEL_INFO, string (" OBJECT ID : ") + ((pWaveManagedObject->getObjectId ()).getClassId ()) + string (", ") + (pWaveManagedObject->getObjectId ()).getInstanceId ());

                // Reset the Object Manager If required (This case will arise in case of a local ON delegating ownership tocorresponding global OM)

                //trace (TRACE_LEVEL_INFO, "Requesting OM : " + pRequestingWaveObjectManager->getName ());

                //trace (TRACE_LEVEL_INFO, "IS MO Supported for : " + className + " : " + (true == (pRequestingWaveObjectManager->isManagedClassSupported (className)) ? "True" : "False"));

                if ((NULL != pRequestingWaveObjectManager) && (true == (pRequestingWaveObjectManager->isManagedClassSupported (className))))
                {
                    pWaveManagedObject->setPWaveObjectManager (pRequestingWaveObjectManager);
                }

                pWaveManagedObject->setPCurrentOwnerWaveObjectManager (pRequestingWaveObjectManager);

                pWaveManagedObject->loadFromPostgresQueryResult (pResult, i, schema, selectFields);

                if (0 < numberOfAuxilliaryTables)
                {
                    // Now process the auxilliary table entries if there are any for this table.

                    tempObjectId   = pWaveManagedObject->getObjectId ();
                    tempClassId    = tempObjectId.getClassId () ;
                    tempInstanceId = tempObjectId.getInstanceId ();

                    auxilliarySqlPrefix  = "SELECT relatedToClassId, relatedToInstanceId from ";
                    auxilliarySqlPostfix = string (" WHERE ownerClassId = ") + tempClassId + string (" AND ownerInstanceId = ") + tempInstanceId + string (";");

                    for (j = 0; j < numberOfAuxilliaryTables; j++)
                    {
                        auxilliarySql = auxilliarySqlPrefix + schema + "." + auxilliaryTableNames[j] + auxilliarySqlPostfix;

                        pAuxilliaryResult = PQexec (pConnection, auxilliarySql.c_str ());

                        DatabaseStatisticsTracker::incrementIndirectQuery (auxilliaryTableNames[j]);

                        if (NULL == pAuxilliaryResult)
                        {
                            trace (TRACE_LEVEL_FATAL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : Could not obtain results for auxilliary table " + auxilliaryTableNames[j] + " On table " + className + ".");
                            prismAssert (false, __FILE__, __LINE__);
                        }
                        else
                        {
                            auxilliaryDatabaseStatus = PQresultStatus (pAuxilliaryResult);

                            if (PGRES_TUPLES_OK != auxilliaryDatabaseStatus)
                            {
                                trace (TRACE_LEVEL_FATAL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : Could not obtain results for auxilliary table " + auxilliaryTableNames[j] + " On table " + className + ".");
                                trace (TRACE_LEVEL_ERROR, string (PQresultErrorMessage (pAuxilliaryResult)));
                                trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + auxilliarySql);
                                prismAssert (false, __FILE__, __LINE__);
                            }
                            else
                            {
                                auxilliaryResultsMap[auxilliaryTableNames[j]] = pAuxilliaryResult;
                            }
                        }
                    }

                    pWaveManagedObject->loadFromPostgresAuxilliaryQueryResult (auxilliaryResultsMap, schema, vector<string> ());

                    // Now release all of the memory held by all of the auxilliary PGresult data structures.

                    map<string, PGresult *>::iterator element = auxilliaryResultsMap.begin ();
                    map<string, PGresult *>::iterator end     = auxilliaryResultsMap.end ();

                    while (element != end)
                    {
                        PQclear (element->second);
                        element++;
                    }
                }
            }
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
            string errorMessage = string (PQresultErrorMessage (pResult));
            trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : ERROR in executing the Database Query :");
            trace (TRACE_LEVEL_ERROR, errorMessage);
            trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + sql);

            DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);
        }

        PQclear (pResult);
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
    	trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : Status : " + FrameworkToolKit::localize (status));
    }

    return (waveManagedObjects);
}

vector<WaveManagedObject *> DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase (const string &childClassName, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pRequestingWaveObjectManager)
{
    vector<WaveManagedObject *>  waveManagedObjects;
    WaveManagedObject           *pWaveManagedObject  = NULL;

    if ((false == (DatabaseObjectManager::getIsDatabaseEnabled ())) && (false == m_isMultiDatabaseMode))
    {
        trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : No support for Persistent Store will be provided.");
        return (waveManagedObjects);
    }

    ResourceId                                status                  = WAVE_MESSAGE_ERROR;
    PGresult                                 *pResult                 = NULL;
    ExecStatusType                            databaseStatus          = PGRES_FATAL_ERROR;
    PGconn                                   *pConnection             = getDatabaseServerConnection (childClassName);
    string                                    sql;
    UI32                                      i                       = 0;
    vector<string>                            selectFields;

    sql = string ("SELECT * from ") + schema +  string (".") + childClassName + string (" WHERE ownerManagedObjectIdClassId = ") + parentObjectId.getClassId () + string (" AND ownerManagedObjectIdInstanceId = ") + parentObjectId.getInstanceId ();

    prismAssert (NULL != pConnection, __FILE__, __LINE__);

    //trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectFromDatabase : Query to execute : \n" + sql);

    pResult = PQexec (pConnection, (sql.c_str ()));

    DatabaseStatisticsTracker::incrementIndirectQuery (childClassName);

    if (NULL == pResult)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        databaseStatus = PQresultStatus (pResult);

        if (PGRES_TUPLES_OK == databaseStatus)
        {
            status = WAVE_MESSAGE_SUCCESS;

            //trace (TRACE_LEVEL_INFO, string ("DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : Got ") + PQntuples (pResult) + string (" results."));

            UI32                     numberOfResults               = PQntuples (pResult);
            string                   className                     = childClassName;
            WaveObjectManager       *pWaveObjectManager            = WaveObjectManager::getOwnerForManagedClass (className);
            vector<string>           auxilliaryTableNames;
            UI32                     numberOfAuxilliaryTables;
            ObjectId                 tempObjectId;
            UI32                     tempClassId;
            UI64                     tempInstanceId;
            UI32                     j;
            string                   auxilliarySqlPrefix;
            string                   auxilliarySqlPostfix;
            string                   auxilliarySql;
            PGresult                *pAuxilliaryResult             = NULL;
            map<string, PGresult *>  auxilliaryResultsMap;
            ExecStatusType           auxilliaryDatabaseStatus      = PGRES_FATAL_ERROR;

            if (NULL == pWaveObjectManager)
            {
                trace (TRACE_LEVEL_FATAL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : There is no known Owner for Managed Class : " + className);
                prismAssert (false, __FILE__, __LINE__);

                return (waveManagedObjects);
            }

            // Obtain if there are any auxilliary table Names.

            OrmRepository::getAllAuxilliaryTableNames (className, auxilliaryTableNames);
            numberOfAuxilliaryTables = auxilliaryTableNames.size ();

            DatabaseStatisticsTracker::incrementManagedObjectsLoadedFromIndirectQuery (className, numberOfResults);

            for (i = 0; i < numberOfResults; i++)
            {
                pWaveManagedObject = pWaveObjectManager->createManagedObjectInstanceWrapper (className);

                prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

                waveManagedObjects.push_back (pWaveManagedObject);

                //trace (TRACE_LEVEL_INFO, string (" OBJECT ID : ") + ((pWaveManagedObject->getObjectId ()).getClassId ()) + string (", ") + (pWaveManagedObject->getObjectId ()).getInstanceId ());

                //trace (TRACE_LEVEL_INFO, string (" OBJECT ID : ") + ((pWaveManagedObject->getObjectId ()).getClassId ()) + string (", ") + (pWaveManagedObject->getObjectId ()).getInstanceId ());

                // Reset the Object Manager If required (This case will arise in case of a local ON delegating ownership tocorresponding global OM)

                //trace (TRACE_LEVEL_INFO, "Requesting OM : " + pRequestingWaveObjectManager->getName ());

                //trace (TRACE_LEVEL_INFO, "IS MO Supported for : " + className + " : " + (true == (pRequestingWaveObjectManager->isManagedClassSupported (className)) ? "True" : "False"));

                if ((NULL != pRequestingWaveObjectManager) && (true == (pRequestingWaveObjectManager->isManagedClassSupported (className))))
                {
                    pWaveManagedObject->setPWaveObjectManager (pRequestingWaveObjectManager);
                }

                pWaveManagedObject->setPCurrentOwnerWaveObjectManager (pRequestingWaveObjectManager);

                pWaveManagedObject->loadFromPostgresQueryResult (pResult, i, schema, selectFields);

                if (0 < numberOfAuxilliaryTables)
                {
                    // Now process the auxilliary table entries if there are any for this table.

                    tempObjectId   = pWaveManagedObject->getObjectId ();
                    tempClassId    = tempObjectId.getClassId () ;
                    tempInstanceId = tempObjectId.getInstanceId ();

                    auxilliarySqlPrefix  = "SELECT relatedToClassId, relatedToInstanceId from ";
                    auxilliarySqlPostfix = string (" WHERE ownerClassId = ") + tempClassId + string (" AND ownerInstanceId = ") + tempInstanceId + string (";");

                    for (j = 0; j < numberOfAuxilliaryTables; j++)
                    {
                        auxilliarySql = auxilliarySqlPrefix + schema + "." + auxilliaryTableNames[j] + auxilliarySqlPostfix;

                        pAuxilliaryResult = PQexec (pConnection, auxilliarySql.c_str ());

                        DatabaseStatisticsTracker::incrementIndirectQuery (auxilliaryTableNames[j]);

                        if (NULL == pAuxilliaryResult)
                        {
                            trace (TRACE_LEVEL_FATAL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : Could not obtain results for auxilliary table " + auxilliaryTableNames[j] + " On table " + className + ".");
                            prismAssert (false, __FILE__, __LINE__);
                        }
                        else
                        {
                            auxilliaryDatabaseStatus = PQresultStatus (pAuxilliaryResult);

                            if (PGRES_TUPLES_OK != auxilliaryDatabaseStatus)
                            {
                                trace (TRACE_LEVEL_FATAL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : Could not obtain results for auxilliary table " + auxilliaryTableNames[j] + " On table " + className + ".");
                                trace (TRACE_LEVEL_ERROR, string (PQresultErrorMessage (pAuxilliaryResult)));
                                trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + auxilliarySql);
                                prismAssert (false, __FILE__, __LINE__);
                            }
                            else
                            {
                                auxilliaryResultsMap[auxilliaryTableNames[j]] = pAuxilliaryResult;
                            }
                        }
                    }

                    pWaveManagedObject->loadFromPostgresAuxilliaryQueryResult (auxilliaryResultsMap, schema, vector<string> ());

                    // Now release all of the memory held by all of the auxilliary PGresult data structures.

                    map<string, PGresult *>::iterator element = auxilliaryResultsMap.begin ();
                    map<string, PGresult *>::iterator end     = auxilliaryResultsMap.end ();

                    while (element != end)
                    {
                        PQclear (element->second);
                        element++;
                    }
                }
            }
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
            string errorMessage = string (PQresultErrorMessage (pResult));
            trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : ERROR in executing the Database Query :");
            trace (TRACE_LEVEL_ERROR, errorMessage);
            trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + sql);

            DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);
        }

        PQclear (pResult);
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
    	trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjectsFromDatabase : Status : " + FrameworkToolKit::localize (status));
    }

    return (waveManagedObjects);
}

void DatabaseObjectManagerExecuteQueryWorker::getDebugInformation (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation)
{
    string debugInformation = DatabaseStatisticsTracker::getAllQueryStatistics ();

    if (0 < debugInformation.size ())
    {
        debugInformation = "\nDATABASE OBJECT MANAGER EXECUTE QUERY WORKER STATISTICS:\n\n" + debugInformation;        

        pWaveAsynchronousContextForDebugInformation->setDebugInformation (debugInformation);
    }

    pWaveAsynchronousContextForDebugInformation->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForDebugInformation->callback ();
}

void DatabaseObjectManagerExecuteQueryWorker::resetDebugInformation (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation)
{
    DatabaseStatisticsTracker::clearAllQueryStatistics ();

    pWaveAsynchronousContextForDebugInformation->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForDebugInformation->callback ();
}

bool DatabaseObjectManagerExecuteQueryWorker::getIsMultiDatabaseMode () const
{
    return (m_isMultiDatabaseMode);
}

void DatabaseObjectManagerExecuteQueryWorker::setIsMultiDatabaseMode (const bool &isMultiDatabaseMode)
{
    m_isMultiDatabaseMode = isMultiDatabaseMode;
}

PGconn *DatabaseObjectManagerExecuteQueryWorker::getDatabaseServerConnection (const string &className)
{
    PGconn *pDatabaseServerConnection = NULL;

    if (false == m_isMultiDatabaseMode)
    {
        DatabaseConnection *pDatabaseConnection = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());

        prismAssert (NULL != pDatabaseConnection, __FILE__, __LINE__);

        pDatabaseServerConnection = pDatabaseConnection->getPConnection ();
    }
    else
    {
        DatabaseMultiConnection *pDatabaseMultiConnection = DatabaseMultiConnection::getInstance ();

        prismAssert (NULL != pDatabaseMultiConnection, __FILE__, __LINE__);

        pDatabaseServerConnection = pDatabaseMultiConnection->getServerConnectionForManagedObject (className);
    }

    return (pDatabaseServerConnection);
}


ResourceId DatabaseObjectManagerExecuteQueryWorker::pqExecWrapper (const string &sqlString, PGresult *&pPGresult, UI32 &numberOfResults)
{
    pPGresult           = NULL;
    numberOfResults     = 0;

    ResourceId returnStatus = WAVE_MESSAGE_SUCCESS;

    DatabaseConnection *pDatabaseConnection = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    prismAssert (NULL != pDatabaseConnection, __FILE__, __LINE__);
    PGconn             *pConnection         = pDatabaseConnection->getPConnection ();
    prismAssert (NULL != pConnection, __FILE__, __LINE__);

    pPGresult = PQexec (pConnection, sqlString.c_str ());

    if (NULL == pPGresult)
    {
        returnStatus = WAVE_MESSAGE_ERROR;
    }
    else
    {
        ExecStatusType databaseStatus = PQresultStatus (pPGresult);

        if (PGRES_TUPLES_OK != databaseStatus)
        {
            returnStatus = WAVE_MESSAGE_ERROR;
            freePqResultsWrapper (pPGresult);
        }
        else
        {
            numberOfResults  = PQntuples (pPGresult);

            if (0 == numberOfResults)
            {
                freePqResultsWrapper (pPGresult);
            }
        }
    }

    return returnStatus;
}

void DatabaseObjectManagerExecuteQueryWorker::freePqResultsWrapper (PGresult *&pPGresult)
{
    PQclear (pPGresult);
    pPGresult = NULL;
}

UI64 DatabaseObjectManagerExecuteQueryWorker::getInstanceIdFromResult (PGresult *pPGresult, SI32 instanceIdColumnNumber, UI32 row)
{
    const   char   *pInstanceIdValue = PQgetvalue (pPGresult, row, instanceIdColumnNumber);
            UI64    instanceId       = strtoull (pInstanceIdValue, NULL, 10);

    return instanceId;
}

ObjectId DatabaseObjectManagerExecuteQueryWorker::getOidFromResult (PGresult *pPGresult, SI32 classIdColumnNumber, SI32 instanceIdColumnNumber, UI32 row)
{
    const   char   *pclassIdValue    = PQgetvalue (pPGresult, row, classIdColumnNumber);
            UI32    classId          = strtoul  (pclassIdValue, NULL, 10);
    const   char   *pInstanceIdValue = PQgetvalue (pPGresult, row, instanceIdColumnNumber);
            UI64    instanceId       = strtoull (pInstanceIdValue, NULL, 10);

    ObjectId  oidFromResult (classId, instanceId);

    return oidFromResult;
}

ResourceId DatabaseObjectManagerExecuteQueryWorker::loadWaveManagedObjects (const RecursiveQueryContext &recursiveQueryContext, const string &className, const string &sqlString, const vector<string> &selectFieldsInManagedObject, vector<WaveManagedObject *> &waveManagedObjectPointerResultVector)
{
    // 1. Query given SQL.

    PGresult   *pPGresult           = NULL;
    UI32        numberOfResults     = 0;

    ResourceId queryStatus = pqExecWrapper (sqlString, pPGresult, numberOfResults);

    if (WAVE_MESSAGE_SUCCESS != queryStatus)
    {
        // Return error if this query fails.
        //
        // If this is top level query, then
        //      Error will be returned to the querying service.
        // Else,
        //      This error will be received by the calling function, (i.e. current / other recursive function), and it will fail assert.
        //      Those assert failures are required, since 'an internal query has failed' when, Database Object Manager is running fine.
        //      (because of SQL errors, or because postgresql server problem.)
        //
        // i.e. Whenever, an error is returned to querying service, there should not be any Managed Object memory leaks.

        return queryStatus;
    }

    if (0 == numberOfResults)
    {
        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId returnStatus     = WAVE_MESSAGE_SUCCESS;

    vector<ObjectId>            currentResultObjectIds;
    map<UI32, vector <UI64> >   classIdToVectorOfInstanceIdMapForOneToOneCompositions;

    // for each row :
    //    create MO
    //    load all simple attributes (including one-one association) in MO.
    //    For 1-1 Compositions, add entry to classIdToVectorOfInstanceIdMapForOneToOneCompositions.
    //    store MO in waveManagedObjectPointerMap
    //    add entry to currentResultObjectIds.

    WaveObjectManager *pWaveObjectManager           = recursiveQueryContext.m_pWaveObjectManager;
    WaveObjectManager *pRequestingWaveObjectManager = recursiveQueryContext.m_pRequestingWaveObjectManager;
    string             schemaToQuery                = recursiveQueryContext.m_schemaToQuery;
    UI32               numberOfSelectFields         = selectFieldsInManagedObject.size ();

    for (UI32 i = 0; i < numberOfResults; i++)
    {
        // A. Create Managed Object.

        WaveManagedObject *pWaveManagedObject = pWaveObjectManager->createManagedObjectInstanceWrapper (className);
        prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

        waveManagedObjectPointerResultVector.push_back (pWaveManagedObject);

        if ((NULL != pRequestingWaveObjectManager) && (true == (pRequestingWaveObjectManager->isManagedClassSupported (className))))
        {
            pWaveManagedObject->setPWaveObjectManager (pRequestingWaveObjectManager);
        }

        pWaveManagedObject->setPCurrentOwnerWaveObjectManager (pRequestingWaveObjectManager);

        // B. Load simple fields.

        //pWaveManagedObject->loadFromPostgresQueryResult (pResult, i, schema, selectFieldsInManagedObject, loadOneToManyRelationships, loadCompositions);
        pWaveManagedObject->loadFromPostgresQueryResult2 (pPGresult, i, schemaToQuery, selectFieldsInManagedObject);
        currentResultObjectIds.push_back (pWaveManagedObject->getObjectId ());

        // C. Retrieve ObjectIds for 1-1 compositions.

        vector<ObjectId> vectorOfOneToOneCompositionObjectIds;
        pWaveManagedObject->getOidsOfOneToOneCompositions (vectorOfOneToOneCompositionObjectIds);

        UI32 numberOfOneToOneCompositionObjectIds = vectorOfOneToOneCompositionObjectIds.size ();

        for (UI32 j = 0; j < numberOfOneToOneCompositionObjectIds; j++)
        {
            ObjectId composedObjectObjectId = vectorOfOneToOneCompositionObjectIds[j];
            classIdToVectorOfInstanceIdMapForOneToOneCompositions[composedObjectObjectId.getClassId ()].push_back (composedObjectObjectId.getInstanceId ());
        }
    }

    freePqResultsWrapper (pPGresult);

    // 3. Load 1-1 compositions.

    if ((true == recursiveQueryContext.m_loadCompositions) || (0 != numberOfSelectFields))
    {
        map<ObjectId, WaveManagedObject*>  map1;

        // for each entry in classIdToVectorOfInstanceIdMapForOneToOneComposition - ((i.e. numberOfOOC + (if target is specialized)))
        //    getSQL for RHS (select from lhstable where instanceid = OR OR OR .. and classid = lhs)
        //    call loadFunction. Get/Store results in ObjectIdToWaveManagedObjectMapForOneToOneCompositions map.

        map<UI32, vector <UI64> >::const_iterator itr5 = classIdToVectorOfInstanceIdMapForOneToOneCompositions.begin ();
        map<UI32, vector <UI64> >::const_iterator end5 = classIdToVectorOfInstanceIdMapForOneToOneCompositions.end ();

        for (; end5 != itr5; itr5++)
        {
            vector<WaveManagedObject *> compositionChildObjects;
            returnStatus = loadObjectsForOidsFromATable (recursiveQueryContext, itr5->first, itr5->second, compositionChildObjects);
            prismAssert (WAVE_MESSAGE_SUCCESS == returnStatus, __FILE__, __LINE__);
            UI32 numberOfCompositionChildObjects = compositionChildObjects.size ();

            for (UI32 j = 0; j < numberOfCompositionChildObjects; j++)
            {
                WaveManagedObject* pChildWaveManagedObject = compositionChildObjects[j];
                map1[pChildWaveManagedObject->getObjectId ()] = pChildWaveManagedObject;
            }
        }

        for (UI32 i = 0; i < numberOfResults; i++)
        {
            WaveManagedObject* pParentWaveManagedObject = waveManagedObjectPointerResultVector[i];
            pParentWaveManagedObject->popOneToOneCompositionsFromResults (map1, selectFieldsInManagedObject);
        }
    }

    if ((false == recursiveQueryContext.m_loadOneToManyRelationShips) && (0 == numberOfSelectFields))
    {
        return returnStatus;
    }

    map <UI32, vector<UI64> > currentResultTableIdToVectorOfInstanceIdMap;

    for (UI32 i = 0; i < numberOfResults; i++)
    {
        WaveManagedObject*  pParentWaveManagedObject    = waveManagedObjectPointerResultVector[i];
        ObjectId            parentObjectId              = pParentWaveManagedObject->getObjectId ();
        currentResultTableIdToVectorOfInstanceIdMap[parentObjectId.getClassId ()].push_back (parentObjectId.getInstanceId ());
    }

    // 4. Load 1-N Associations.

    map<string, string> relationNameToAuxilliaryTableNameMapForOneToNAssociations;
    OrmRepository::getRelationNameToAuxilliaryTableNameMapForOneToNAssociations (className, selectFieldsInManagedObject, relationNameToAuxilliaryTableNameMapForOneToNAssociations);
    map<string, map <ObjectId, vector <ObjectId> > > map3;

    // for current class's 1-N Association relations :

    map<string, string>::const_iterator itr6 = relationNameToAuxilliaryTableNameMapForOneToNAssociations.begin ();
    map<string, string>::const_iterator end6 = relationNameToAuxilliaryTableNameMapForOneToNAssociations.end ();
    for (; itr6 != end6; itr6++)
    {
        map <ObjectId, vector <ObjectId> > ownerToRelatedOidsMap;
        map <UI32, vector<UI64> >::const_iterator itr7 = currentResultTableIdToVectorOfInstanceIdMap.begin ();
        map <UI32, vector<UI64> >::const_iterator end7 = currentResultTableIdToVectorOfInstanceIdMap.end ();
        for (; itr7 != end7; itr7++)
        {
            returnStatus = queryAuxiliaryTableRowsForVectorOfOwnerOids (recursiveQueryContext, itr6->second, itr7->first, itr7->second, ownerToRelatedOidsMap);
            prismAssert (WAVE_MESSAGE_SUCCESS == returnStatus, __FILE__, __LINE__);
        }
        map3[itr6->first].swap (ownerToRelatedOidsMap);
    }

    for (UI32 i = 0; i < numberOfResults; i++)
    {
        WaveManagedObject* pParentWaveManagedObject = waveManagedObjectPointerResultVector[i];

        map<string, map<ObjectId, vector <ObjectId> > >::iterator itr8 = map3.begin ();
        map<string, map<ObjectId, vector <ObjectId> > >::iterator end8 = map3.end ();

        for (; end8 != itr8; itr8++)
        {
            ObjectId currentObjectId = pParentWaveManagedObject->getObjectId ();
            string relationName = itr8->first;
            if (itr8->second.end () != itr8->second.find (currentObjectId))
            {
                pParentWaveManagedObject->storeRelatedObjectIdVectorForAOneToNAssociation (relationName, currentObjectId, itr8->second[currentObjectId]);
            }
            else
            {
                vector<ObjectId> emptyVectorOfRelatedObjectIds;
                pParentWaveManagedObject->storeRelatedObjectIdVectorForAOneToNAssociation (relationName, currentObjectId, emptyVectorOfRelatedObjectIds);
            }
        }
    }

    // 5. Load 1-N Compositions

    if ((true == recursiveQueryContext.m_loadCompositions) || (0 != numberOfSelectFields))
    {
        map<string, string> relationNameToAuxilliaryTableNameMapForOneToNCompositions;
        OrmRepository::getRelationNameToAuxilliaryTableNameMapForOneToNCompositions (className, selectFieldsInManagedObject, relationNameToAuxilliaryTableNameMapForOneToNCompositions);
        map<string, map <ObjectId, vector <WaveManagedObject *> > > map4;

        // for current class's 1-N Composition relations :
        map<string, string>::iterator itr10 = relationNameToAuxilliaryTableNameMapForOneToNCompositions.begin ();
        map<string, string>::iterator end10 = relationNameToAuxilliaryTableNameMapForOneToNCompositions.end ();
        for (; itr10 != end10; itr10++)
        {
            map <ObjectId, vector <WaveManagedObject *> > ownerToRelatedObjectsMap;
            map <UI32, vector<UI64> >::const_iterator itr11 = currentResultTableIdToVectorOfInstanceIdMap.begin ();
            map <UI32, vector<UI64> >::const_iterator end11 = currentResultTableIdToVectorOfInstanceIdMap.end ();
            for (; itr11 != end11; itr11++)
            {
                returnStatus = getChildObjectsForACompositionRelation (recursiveQueryContext, itr10->second, itr11->first, itr11->second, ownerToRelatedObjectsMap);
                prismAssert (WAVE_MESSAGE_SUCCESS == returnStatus, __FILE__, __LINE__);
            }
            map4[itr10->first].swap (ownerToRelatedObjectsMap);
        }

        for (UI32 i = 0; i < numberOfResults; i++)
        {
            WaveManagedObject* pParentWaveManagedObject = waveManagedObjectPointerResultVector[i];

            map<string, map<ObjectId, vector <WaveManagedObject *> > >::iterator itr12 = map4.begin ();
            map<string, map<ObjectId, vector <WaveManagedObject *> > >::iterator end12 = map4.end ();

            for (; end12 != itr12; itr12++)
            {
                ObjectId currentObjectId = pParentWaveManagedObject->getObjectId ();
                string relationName = itr12->first;
                if (itr12->second.end () != itr12->second.find (currentObjectId))
                {
                    pParentWaveManagedObject->storeRelatedObjectVectorForAOneToNComposition (relationName, currentObjectId, itr12->second[currentObjectId]);
                }
                else
                {
                    vector<WaveManagedObject *> emptyVectorOfRelatedObjects;
                    pParentWaveManagedObject->storeRelatedObjectVectorForAOneToNComposition (relationName, currentObjectId, emptyVectorOfRelatedObjects);
                }
            }
        }
    }

    return returnStatus;
}

ResourceId DatabaseObjectManagerExecuteQueryWorker::loadObjectsForOidsFromATable (const RecursiveQueryContext &recursiveQueryContext, const UI32 &classId, const vector<UI64> &vectorOfInstanceIds, vector<WaveManagedObject *> &pWaveManagedObjectPointerVector)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    string schemaToQuery    = recursiveQueryContext.m_schemaToQuery;
    string tableName        = OrmRepository::getTableNameById (classId);

    UI32 numberOfStages = vectorOfInstanceIds.size () / s_batchSize;
    UI32 lastStageSize  = (vectorOfInstanceIds.size ()) % s_batchSize;

    if (0 != lastStageSize)
    {
        numberOfStages++;
    }

    for (UI32 i = 0; i < numberOfStages; i++)
    {
        string sql = "SELECT * FROM ONLY " + schemaToQuery + "." + tableName + " where objectIdInstanceId IN (";

        bool separatorRequired = false;

        UI32 beginIndex = i * s_batchSize;
        UI32 endIndex   = (i == (numberOfStages - 1)) ? vectorOfInstanceIds.size () : (i + 1) * s_batchSize;

        for (UI32 j = beginIndex; j < endIndex; j++)
        {
            if (false == separatorRequired)
            {
                separatorRequired = true;
            }
            else
            {
                sql = sql + ", ";
            }

            sql = sql + vectorOfInstanceIds[j];
        }

        sql = sql + ")";

        vector<string> selectFieldsInManagedObject;

        vector<WaveManagedObject *> pWaveManagedObjectPointerVectorStage;

        status = loadWaveManagedObjects (recursiveQueryContext, tableName, sql, selectFieldsInManagedObject, pWaveManagedObjectPointerVectorStage);

        pWaveManagedObjectPointerVector.insert (pWaveManagedObjectPointerVector.end (), pWaveManagedObjectPointerVectorStage.begin (), pWaveManagedObjectPointerVectorStage.end ());

    }

    return status;
}

ResourceId DatabaseObjectManagerExecuteQueryWorker::queryAuxiliaryTableRowsForVectorOfOwnerOids (const RecursiveQueryContext &recursiveQueryContext, const string &auxiliaryTableName, const UI32 &ownerClassId, const vector<UI64> &ownerInstanceIdVector, map <ObjectId, vector <ObjectId> > &ownerOidToVectorOfRelatedOidsMap)
{
    UI32 numberOfStages = ownerInstanceIdVector.size () / s_batchSize;
    UI32 lastStageSize  = (ownerInstanceIdVector.size ()) % s_batchSize;

    if (0 != lastStageSize)
    {
        numberOfStages++;
    }

    // 1. Prepare sql

    string schemaToQuery = recursiveQueryContext.m_schemaToQuery;

    for (UI32 i = 0; i < numberOfStages; i++)
    {
        string sqlString = "SELECT ownerInstanceId, relatedToClassId, relatedToInstanceId from " + schemaToQuery + "." + auxiliaryTableName + " WHERE ownerClassId = " + ownerClassId + " AND ownerInstanceId IN (";

        bool separatorRequired = false;

        UI32 beginIndex = i * s_batchSize;
        UI32 endIndex   = (i == (numberOfStages - 1)) ? ownerInstanceIdVector.size () : (i + 1) * s_batchSize;

        for (UI32 j = beginIndex; j < endIndex; j++)
        {
            if (false == separatorRequired)
            {
                separatorRequired = true;
            }
            else
            {
                sqlString = sqlString + ", ";
            }

            sqlString = sqlString + ownerInstanceIdVector[j];
        }

        sqlString = sqlString + ") ORDER BY ownerInstanceId";

        // 2. Execute sql

        PGresult *pPGresult         = NULL;
        UI32      numberOfResults   = 0;

        ResourceId queryStatus = pqExecWrapper (sqlString, pPGresult, numberOfResults);

        if (WAVE_MESSAGE_SUCCESS != queryStatus)
        {
            return queryStatus;
        }

        if (0 == numberOfResults)
        {
            continue;
        }

        // 3. Load results.

        SI32 ownerInstanceIdColumnNumber    = PQfnumber (pPGresult, "ownerInstanceId");
        SI32 rhsClassIdColumnNumber         = PQfnumber (pPGresult, "relatedToClassId");
        SI32 rhsInstanceIdColumnNumber      = PQfnumber (pPGresult, "relatedToInstanceId");

        ObjectId lastOwnerOid (ownerClassId, getInstanceIdFromResult (pPGresult, ownerInstanceIdColumnNumber, 0));

        vector<ObjectId> vectorOfRelatedOids;

        for (UI32 j = 0; j < numberOfResults; j++)
        {
            ObjectId currentOwnerOid (ownerClassId, getInstanceIdFromResult (pPGresult, ownerInstanceIdColumnNumber, j));

            if (lastOwnerOid != currentOwnerOid)
            {
                ownerOidToVectorOfRelatedOidsMap[lastOwnerOid] = vectorOfRelatedOids;

                vectorOfRelatedOids.clear ();

                lastOwnerOid = currentOwnerOid;
            }

            vectorOfRelatedOids.push_back (getOidFromResult (pPGresult, rhsClassIdColumnNumber, rhsInstanceIdColumnNumber, j));
        }

        ownerOidToVectorOfRelatedOidsMap[lastOwnerOid] = vectorOfRelatedOids;

        freePqResultsWrapper (pPGresult);
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId DatabaseObjectManagerExecuteQueryWorker::getChildObjectsForACompositionRelation (const RecursiveQueryContext &recursiveQueryContext, const string &auxiliaryTableName, const UI32& ownerClassId, const vector<UI64> &ownerInstanceIdVector, map <ObjectId, vector <WaveManagedObject *> > &ownerOidToVectorOfRelatedObjectsMap)
{
    map<ObjectId, vector <ObjectId> > map1;

    ResourceId status = queryAuxiliaryTableRowsForVectorOfOwnerOids (recursiveQueryContext, auxiliaryTableName, ownerClassId, ownerInstanceIdVector, map1);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        return status;
    }

    // prepare map<ClassIdType, vector <InstanceIdType> > using related oids in map1.
    // (Note : single MO instance cannot be composed in two different parents at the same time.)

    map<UI32, vector <UI64> > childIdsMap;

    map<ObjectId, vector <ObjectId> >::const_iterator itr1 = map1.begin ();
    map<ObjectId, vector <ObjectId> >::const_iterator end1 = map1.end ();

    for (; end1 != itr1; itr1++)
    {
        vector <ObjectId> vec1 = itr1->second;

        for (UI32 i = 0; i < vec1.size (); i++)
        {
            UI32    classId     = vec1[i].getClassId ();
            UI64    instanceId  = vec1[i].getInstanceId ();
            childIdsMap[classId].push_back (instanceId);
        }
    }

    UI32 numberOfTypesOfChildObjects = childIdsMap.size ();

    if (0 == numberOfTypesOfChildObjects)
    {
        // Nothing to be done.

        return WAVE_MESSAGE_SUCCESS;
    }

    if (1 == numberOfTypesOfChildObjects)
    {
        map<UI32, vector <UI64> >::iterator itr8            = childIdsMap.begin ();
        UI32                                childClassId    = itr8->first;
        string                              childTableName  = OrmRepository::getTableNameById (childClassId);
        string                              ownerTableName  = OrmRepository::getTableNameById (ownerClassId);

        UI32 numberOfPossibleCompositionRelationsBetweenOwnerClassIdAndChildClassId = OrmRepository::getNumberOfCompositionRelationsBetweenTables (ownerTableName, childTableName);

        if (1 == numberOfPossibleCompositionRelationsBetweenOwnerClassIdAndChildClassId)
        {
            UI32 numberOfStages = ownerInstanceIdVector.size () / s_batchSize;
            UI32 lastStageSize  = (ownerInstanceIdVector.size ()) % s_batchSize;

            if (0 != lastStageSize)
            {
                numberOfStages++;
            }

            for (UI32 i = 0; i < numberOfStages; i++)
            {
                string sqlString = "SELECT * from ONLY " + recursiveQueryContext.m_schemaToQuery + "." + childTableName + " WHERE ownerManagedObjectIdClassId = " + ownerClassId + " AND ownerManagedObjectIdInstanceId IN (";

                bool separatorRequired = false;

                UI32 beginIndex = i * s_batchSize;
                UI32 endIndex   = (i == (numberOfStages - 1)) ? ownerInstanceIdVector.size () : (i + 1) * s_batchSize;

                for (UI32 j = beginIndex; j < endIndex; j++)
                {
                    if (false == separatorRequired)
                    {
                        separatorRequired = true;
                    }
                    else
                    {
                        sqlString = sqlString + ", ";
                    }

                    sqlString = sqlString + ownerInstanceIdVector[j];
                }

                sqlString = sqlString + ")";

                vector<string>              selectFieldsInManagedObject;
                vector<WaveManagedObject *> childObjects;

                ResourceId status2 = loadWaveManagedObjects (recursiveQueryContext, childTableName, sqlString, selectFieldsInManagedObject, childObjects);
                prismAssert (WAVE_MESSAGE_SUCCESS == status2, __FILE__, __LINE__);

                for (UI32 k = 0; k < childObjects.size (); k++)
                {
                    WaveManagedObject* pChildWaveManagedObject = childObjects[k];
                    ownerOidToVectorOfRelatedObjectsMap[pChildWaveManagedObject->getOwnerManagedObjectId ()].push_back (pChildWaveManagedObject);
                }
            }

            // TODO : Following is not part of query/not required for current query. However, it can be added for additional consistency check.
            // i.e. Verify that the "vector of child instance ids from auxiliary table (.. itr8->second)" and  "instance ids from child results" match.
            //      (Maintain a vector for child instance ids from child table results, Sort the vectors and match.)

            return WAVE_MESSAGE_SUCCESS;
        }
    }

    map<UI32, vector <UI64> >::const_iterator itr2 = childIdsMap.begin ();
    map<UI32, vector <UI64> >::const_iterator end2 = childIdsMap.end ();

    for (; end2 != itr2; itr2++)
    {
        UI32 childTableId = itr2->first;

        vector<UI64> childInstanceIds = itr2->second;

        vector<WaveManagedObject *> childObjects;

        ResourceId status3 = loadObjectsForOidsFromATable (recursiveQueryContext, childTableId, childInstanceIds, childObjects);
        prismAssert (WAVE_MESSAGE_SUCCESS == status3, __FILE__, __LINE__);

        for (UI32 i = 0; i < childObjects.size (); i++)
        {
            WaveManagedObject* pChildWaveManagedObject = childObjects[i];
            ownerOidToVectorOfRelatedObjectsMap[pChildWaveManagedObject->getOwnerManagedObjectId ()].push_back (pChildWaveManagedObject);
        }
    }

    return WAVE_MESSAGE_SUCCESS;
}

void DatabaseObjectManagerExecuteQueryWorker::getDBSchemaInfoMessageHandler (DatabaseObjectManagerGetDBSchemaInfoMessage *pDatabaseObjectManagerGetDBSchemaInfoMessage)
{

    if (true == (DatabaseObjectManager::getIsDatabaseShuttingDownOnReboot ()))
    {
        DatabaseObjectManager::goOnInfiniteLoopBeforeReboot ();
    }

    if ((false == (DatabaseObjectManager::getIsDatabaseEnabled ())) && (false == m_isMultiDatabaseMode))
    {
        trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteQueryWorker::getDBSchemaInfoMessageHandler: No support for Persistent Store will be provided.");
        pDatabaseObjectManagerGetDBSchemaInfoMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pDatabaseObjectManagerGetDBSchemaInfoMessage);
        return;
    }

    DatabaseSchema                    databaseSchema;
    DatabaseStandaloneTransaction     databaseObject;

    if (0 == databaseObject.getSchemaChangeInfo (databaseSchema))
    {   
        trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerExecuteQueryWorker::getDBSchemaInfoMessageHandler : Upgrade Mo has zero entries.");
        pDatabaseObjectManagerGetDBSchemaInfoMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
    }
    else
    {
        pDatabaseObjectManagerGetDBSchemaInfoMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pDatabaseObjectManagerGetDBSchemaInfoMessage->setDatabaseSchema (databaseSchema);
    }
    reply (pDatabaseObjectManagerGetDBSchemaInfoMessage);
}

void DatabaseObjectManagerExecuteQueryWorker::dropAllDbViewsMessageHandler (DatabaseObjectManagerDropAllDBViewsMessage *pDatabaseObjectManagerDropAllDBViewsMessage)
{
    if (true == (DatabaseObjectManager::getIsDatabaseShuttingDownOnReboot ()))
    {   
        DatabaseObjectManager::goOnInfiniteLoopBeforeReboot ();
    }

    if ((false == (DatabaseObjectManager::getIsDatabaseEnabled ())) && (false == m_isMultiDatabaseMode))
    {   
        trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteQueryWorker::dropAllDbViewsMessageHandler: No support for Persistent Store will be provided.");
        pDatabaseObjectManagerDropAllDBViewsMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pDatabaseObjectManagerDropAllDBViewsMessage);
        return;
    }

    DatabaseStandaloneTransaction     databaseObject;

    if (FRAMEWORK_SUCCESS != databaseObject.dropAllDBViews ())
    {
        trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerExecuteQueryWorker::dropAllDbViewsMessageHandler: failed to delete all database views.");
        pDatabaseObjectManagerDropAllDBViewsMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerExecuteQueryWorker::dropAllDbViewsMessageHandler: successful.");
        pDatabaseObjectManagerDropAllDBViewsMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }

    reply (pDatabaseObjectManagerDropAllDBViewsMessage);
}

void DatabaseObjectManagerExecuteQueryWorker::debugChangeQuerySettingsMessageHandler (DatabaseObjectManagerDebugChangeQuerySettingsMessage *pDatabaseObjectManagerDebugChangeQuerySettingsMessage)
{
    bool    useRecursiveQuery = pDatabaseObjectManagerDebugChangeQuerySettingsMessage->getUseRecursiveQuery ();

    string recursiveQueryFlagValue = "FALSE";

    if (true == useRecursiveQuery)
    {
        UI32 batchSize = pDatabaseObjectManagerDebugChangeQuerySettingsMessage->getBatchSize ();

        if (0 != batchSize)
        {
            s_batchSize = batchSize;
        }

        recursiveQueryFlagValue = "TRUE";

        s_useRecursiveQuery = true;
    }
    else
    {
        s_useRecursiveQuery = false;
    }

    trace (TRACE_LEVEL_INFO, string ("DatabaseObjectManagerExecuteQueryWorker::debugChangeQuerySettingsMessageHandler : s_useRecursiveQuery [") + recursiveQueryFlagValue + string ("], s_batchSize [") + s_batchSize + string ("]"));

    pDatabaseObjectManagerDebugChangeQuerySettingsMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pDatabaseObjectManagerDebugChangeQuerySettingsMessage);
}

}
