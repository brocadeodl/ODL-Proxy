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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Mayur Mahajan                                                *
 ***************************************************************************/

#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Database/DatabaseStandaloneTransaction.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Cluster/Local/WaveNode.h"
#include "Framework/Types/Types.h" 

namespace WaveNs
{

PrismMutex DatabaseStandaloneTransaction::s_standaloneTransactionMutex;

DatabaseStandaloneTransaction::DatabaseStandaloneTransaction ()
    : m_needConnectionTeardown (true),
      m_bootCount (0)
{
}

DatabaseStandaloneTransaction::~DatabaseStandaloneTransaction ()
{
}

bool DatabaseStandaloneTransaction::execute (const string &sql)
{

    s_standaloneTransactionMutex.lock ();

    bootDatabaseStep ();
    bool status = executeStep (sql);

    shutdownDatabase ();
    s_standaloneTransactionMutex.unlock ();

    return (status);
}

void DatabaseStandaloneTransaction::bootDatabaseStep ()
{
    trace (TRACE_LEVEL_INFO, "DatabaseStandaloneTransaction::bootDatabaseStep : Booting the persistent repository for standalone transaction [" + DatabaseObjectManager::getDatabaseName () + ":" + DatabaseObjectManager::getDatabasePort () + "]...");

    DatabaseConnection *pDatabaseConnection = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());

    prismAssert (NULL != pDatabaseConnection, __FILE__, __LINE__);

    if (NULL != (pDatabaseConnection->getPConnection ()))
    {
        trace (TRACE_LEVEL_INFO, "DatabaseStandaloneTransaction::bootDatabaseStep : Already connected to Wave Database.  No booting required.");

        if (0 == m_bootCount)
        {
            m_needConnectionTeardown = false;
        }
    }
    else
    {
        system ((string ("pg_ctl -D ") + DatabaseObjectManager::getDatabaseDirectory () + string (" stop")).c_str ());
        system ((string ("chmod 777 ") + DatabaseObjectManager::getDatabaseDirectory () + string ("/postmaster.pid")).c_str ());
        system ((string ("rm -rf ") + DatabaseObjectManager::getDatabaseDirectory () + string ("/postmaster.pid")).c_str ());

        UI8     retries       = 10;
        SI32    status        = -1;

        // option -w in pg_ctl command returns a non-zero status for command failure. 
        string commandString = string ("pg_ctl -D ") + DatabaseObjectManager::getDatabaseDirectory () + DatabaseObjectManager::getDatabaseLogStringForPgctl () + string (" -w -o \" -F --shared_buffers=") + DatabaseObjectManager::getDatabaseSharedBuffers () + string (" --max_stack_depth=7680kB --checkpoint_segments=1 --max_prepared_transactions='8' --max_locks_per_transaction='200' --log_line_prefix='%t %a [%e] ' -p ") + DatabaseObjectManager::getDatabasePort () + "\" start > /dev/null";

        trace (TRACE_LEVEL_DEVEL, "DatabaseStandaloneTransaction::bootDatabaseStep : Starting psql- conmmand string " + commandString);

        for (UI8 i = 0; i < retries; i++)
        {   
            vector<string>  output;
                            status = FrameworkToolKit::systemCommandOutput (commandString.c_str (), output);

            if (0 == status)
            {   
                trace (TRACE_LEVEL_INFO, "DatabaseStandaloneTransaction::bootDatabaseStep : Done");
                break;
            }

            /* 
             * Sometimes pg_ctl command returns non-zero but postgres has actually started.
             * So, lets try connecting to DB and confirm if we need to retry 'pg_ctl start' command.
             */

            prismSleep (4);

            bool                isConnectedToDatabase = false;
            pDatabaseConnection  = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());

            prismAssert (NULL != pDatabaseConnection, __FILE__, __LINE__);

            if (NULL != (pDatabaseConnection->getPConnection ()))
            {   
                trace (TRACE_LEVEL_ERROR, "DatabaseStandaloneTransaction::bootDatabaseStep : Already connected to Wave Database. This should not happen as it is just started.");
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {   
                isConnectedToDatabase = pDatabaseConnection->connect (120);

                if (false == isConnectedToDatabase)
                {   
                    trace (TRACE_LEVEL_ERROR, "DatabaseStandaloneTransaction::bootDatabaseStep : Could not connect to database.");
                }
                else
                {   
                    trace (TRACE_LEVEL_INFO, "DatabaseStandaloneTransaction::bootDatabaseStep : Connection succeeded even if pg_ctl errored out. Treating postgres has started successfully.");
                    DatabaseConnection::deleteInstance ();
                    status = 0;
                    break;
                }
            }

            trace (TRACE_LEVEL_WARN, "DatabaseStandaloneTransaction::bootDatabaseStep : failed to start postgres server. -");
            if (0 < output.size())
            {   
                tracePrintf (TRACE_LEVEL_WARN, false, true, output[0].c_str());
            }
            prismSleep (4);
        }

        if (0 != status)
        {   
            tracePrintf (TRACE_LEVEL_WARN, true, false, string("DatabaseStandaloneTransaction::bootDatabaseStep : failed to start postgres server after [%s] retries also. Can't move forward. Asserting.").c_str(), retries);
            prismAssert (false, __FILE__, __LINE__);
        }
        
    }

    m_bootCount++;

}

bool DatabaseStandaloneTransaction::executeStep (const string &sql)
{
    DatabaseConnection *pDatabaseConnection  = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());

    prismAssert (NULL != pDatabaseConnection, __FILE__, __LINE__);

    PGconn *pConnection = pDatabaseConnection->getPConnection ();
    if (NULL != pConnection)
    {
        trace (TRACE_LEVEL_DEVEL, "DatabaseStandaloneTransaction::executeStep : Already connected to Wave Database.");
    }
    else
    {
        bool isConnectedToDatabse = pDatabaseConnection->connect (30);

        if (false == isConnectedToDatabse)
        {
            trace (TRACE_LEVEL_ERROR, "DatabaseStandaloneTransaction::executeStep : Could not connect to database.");
        return (false);
        }
    pConnection = pDatabaseConnection->getPConnection ();
    prismAssert (pConnection != NULL, __FILE__, __LINE__);
    }

    trace (TRACE_LEVEL_INFO, "DatabaseStandaloneTransaction::executeStep : Succesfully connected to Wave Database.");

    PGresult *pResult = PQexec (pConnection, sql.c_str ());
    if (NULL == pResult)
    {
        trace (TRACE_LEVEL_ERROR, "DatabaseStandaloneTransaction::executeStep : SQL execution returned NULL result");
    return (false);
    }
    else if (PGRES_COMMAND_OK != PQresultStatus (pResult))
    {
        string errorMessage = string(PQresultErrorMessage (pResult));
        trace (TRACE_LEVEL_ERROR, string("DatabaseStandaloneTransaction::executeStep : SQL execution returned failure, type :") + PQresultStatus (pResult) + ", Error out: " + errorMessage);
    
        DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);
        PQclear (pResult);
        return (false);
    }

    PQclear (pResult);
    return (true);
}

PGconn *DatabaseStandaloneTransaction::getDatabaseConnection ()
{
    DatabaseConnection *pDatabaseConnection  = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());

    prismAssert (NULL != pDatabaseConnection, __FILE__, __LINE__);

    PGconn *pConnection = pDatabaseConnection->getPConnection ();
    if (NULL != pConnection)
    {   
        trace (TRACE_LEVEL_DEVEL, "DatabaseStandaloneTransaction::getDerivationsInstancesForTable : Already connected to Wave Database.");
    }
    else
    {   
        bool isConnectedToDatabse = pDatabaseConnection->connect (30);

        if (false == isConnectedToDatabse)
        {   
            trace (TRACE_LEVEL_ERROR, "DatabaseStandaloneTransaction::getDerivationsInstancesForTable : Could not connect to database.");
            return (NULL);
        }
        pConnection = pDatabaseConnection->getPConnection ();
        prismAssert (pConnection != NULL, __FILE__, __LINE__);
    }

    return (pConnection);
}

bool DatabaseStandaloneTransaction::getObjectInstanceIdsForTable (const string &tableName, vector<UI64>& objectInstanceIds)
{
    s_standaloneTransactionMutex.lock ();
    bootDatabaseStep ();

    PGconn *pConnection  = getDatabaseConnection ();
    bool status = false;

    string sql = OrmRepository::getSqlForAllEntriesInTable (tableName, OrmRepository::getWaveCurrentSchema ());
    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "DatabaseStandaloneTransaction::getDerivationsInstancesForTable : sql = %s", sql.c_str());

    PGresult *pResult = PQexec (pConnection, sql.c_str());
    if (NULL == pResult)
    {
        trace (TRACE_LEVEL_DEBUG, "DatabaseStandaloneTransaction::getDerivationsInstancesForTable : SQL execution returned NULL result");
    }
    else if (PGRES_TUPLES_OK != PQresultStatus (pResult))
    {
        string errorMessage = string (PQresultErrorMessage (pResult));
        trace (TRACE_LEVEL_DEBUG, string("DatabaseStandaloneTransaction::getDerivationsInstancesForTable : SQL execution returned failure, type :") + PQresultStatus (pResult) + ", Error out: " + errorMessage);

        DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);

        PQclear (pResult);
    }
    else
    {
        UI32 numberOfResults = PQntuples (pResult);
        UI32  column    = PQfnumber (pResult, "objectIdInstanceId");
        for (UI32 i = 0; i < numberOfResults; i++)
        {
            char *pValue    = PQgetvalue (pResult, i, column);
            UI64 instanceId = strtoull (pValue, NULL, 10);
            objectInstanceIds.push_back (instanceId);
        }
        PQclear (pResult);
        status = true;
    }
    shutdownDatabase ();
    s_standaloneTransactionMutex.unlock ();

    return (status);
}

bool DatabaseStandaloneTransaction::checkIfTableHasEntriesInDb (const string &tableName, const string &schema, bool &exists)
{
    s_standaloneTransactionMutex.lock ();
    bootDatabaseStep ();

    PGconn *pConnection  = getDatabaseConnection ();
    string sql           = string ("SELECT * FROM " + schema + "." + tableName + ";");
           exists        = false;
    bool   status        = false;
    
    PGresult *pResult = PQexec (pConnection, sql.c_str());
    if (NULL == pResult)
    {
        trace (TRACE_LEVEL_WARN, "DatabaseStandaloneTransaction::checkIfTableHasEntriesInDb : SQL execution returned NULL result");
    }
    else if (PGRES_TUPLES_OK != PQresultStatus (pResult))
    {
        string errorMessage = string (PQresultErrorMessage (pResult));
        trace (TRACE_LEVEL_WARN, string("DatabaseStandaloneTransaction::checkIfTableHaveEntriesInDb : SQL execution returned failure, type :") + PQresultStatus (pResult) + ", Error out: " + errorMessage);

        DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);
        trace (TRACE_LEVEL_WARN, string("DatabaseStandaloneTransaction::checkIfTableHasEntriesInDb : SQL execution returned failure, type :") + PQresultStatus (pResult) + ", Error out: " + PQresultErrorMessage (pResult));

        PQclear (pResult);
    }
    else
    {
        status = true;
        UI32 numberOfResults = PQntuples (pResult);
        if ( 0 < numberOfResults )
        {
            exists = true;
        }
        PQclear (pResult);
    }

    shutdownDatabase ();
    s_standaloneTransactionMutex.unlock ();

    return (status);
}


UI32 DatabaseStandaloneTransaction::getSchemaChangeInfo (DatabaseSchema &databaseSchema)
{                                                                                                                                                                                
    s_standaloneTransactionMutex.lock ();                                                                                                                                        

    vector<string>  managedObjectNames;
    vector<string>  managedObjectFieldNamesStrings;
    vector<string>  managedObjectFieldTypesStrings;
    vector<UI32>    classIds;
    vector<UI32>    parentClassIds;
    vector<string>  userDefinedKeyCombinationsFieldNamesForSchemaChange;
    vector<string>  userDefinedKeyCombinationsFieldTypesForSchemaChange;
    vector<string>  managedObjectExtendedFieldTypes;
    vector<bool>    isLocalMOInfo;
    vector<string>  derivedFromClassNames;

    bootDatabaseStep ();                                                                                                                                                         
                                                                                                                                                                                 
    PGconn *pConnection        = getDatabaseConnection ();                                                                                                                       
    string sql                 = string ("SELECT * FROM " + OrmRepository::getWaveCurrentSchema () + "." + OrmRepository::getSchemaUpgradeTableName () + " ORDER BY classname;");
                                                                                                                                                                                 
    PGresult *pResult          = PQexec (pConnection, sql.c_str());                                                                                                              
    UI32      numberOfResults  = 0;                                                                                                                                              
                                                                                                                                                                                 
    if (NULL == pResult)                                                                                                                                                         
    {                                                                                                                                                                            
        trace (TRACE_LEVEL_WARN, "DatabaseStandaloneTransaction::getSchemaChangeInfo : SQL execution returned NULL result");                                                     
        prismAssert (false, __FILE__, __LINE__);
    }                                                                                                                                                                            
    else if (PGRES_TUPLES_OK != PQresultStatus (pResult))                                                                                                                        
    {                                                                                                                                                                            
        string errorMessage = string(PQresultErrorMessage (pResult));
        trace (TRACE_LEVEL_WARN, string("DatabaseStandaloneTransaction::getSchemaChangeInfo : SQL execution returned failure, type :") + PQresultStatus (pResult) + ", Error out: " + errorMessage);
        DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);

        PQclear (pResult);
        prismAssert (false, __FILE__, __LINE__);
    }                                                                                                                                                                            
    else                                                                                                                                                                         
    {                                                                                                                                                                            
        numberOfResults = PQntuples (pResult);                                                                                                                                   
        for (UI32 i = 0; i < numberOfResults; ++i)                                                                                                                               
        {                                                                                                                                                                        
            managedObjectNames.push_back (PQgetvalue (pResult, i, PQfnumber (pResult, "classname")));                                                                            
            managedObjectFieldNamesStrings.push_back (PQgetvalue (pResult, i, PQfnumber (pResult, "fieldnames")));                                                               
            managedObjectFieldTypesStrings.push_back (PQgetvalue (pResult, i, PQfnumber (pResult, "fieldtypes")));                                                               
            userDefinedKeyCombinationsFieldNamesForSchemaChange.push_back (PQgetvalue (pResult, i, PQfnumber (pResult, "userdefinedfieldnames")));                                                               
            userDefinedKeyCombinationsFieldTypesForSchemaChange.push_back (PQgetvalue (pResult, i, PQfnumber (pResult, "userdefinedfieldtypes")));                                                               
            managedObjectExtendedFieldTypes.push_back (PQgetvalue (pResult, i, PQfnumber (pResult, "expandedfieldtypes")));                                                               
                                                                                                                                                                                 
            char *pValue    = PQgetvalue (pResult, i, PQfnumber (pResult, "classid"));                                                                                           
            UI32 classId    = strtoul (pValue, NULL, 10);                                                                                                                        
            classIds.push_back (classId);                                                                                                                                        
                                                                                                                                                                                 
            pValue          = PQgetvalue (pResult, i, PQfnumber (pResult, "parentclassid"));                                                                                     
            UI32 parentId   = strtoul (pValue, NULL, 10);                                                                                                                        
            parentClassIds.push_back (parentId);                                                                                                                                 

            bool isALocalMo = false;
            SI8  column     = PQfnumber (pResult, "isalocalmanagedobject");

            if (-1 != column)
            {
                pValue      = PQgetvalue (pResult, i, column);
            
                if ((0 == strncmp (pValue, ("t"), 1)) || (0 == strncmp (pValue, ("true"), 4)))
                {
                    isALocalMo = true;
                }
            }

            isLocalMOInfo.push_back (isALocalMo);

            string derivedFrom = "";
            column = PQfnumber (pResult, "derivedfromclassname");
            if (-1 != column)
            {
                derivedFrom = (PQgetvalue (pResult, i, column));
            }

            derivedFromClassNames.push_back (derivedFrom);
        }                                 
                                                                                                                                                                                 
        databaseSchema.setManagedObjectNames (managedObjectNames);
        databaseSchema.setFieldNamesStrings (managedObjectFieldNamesStrings);
        databaseSchema.setFieldNamesTypes (managedObjectFieldTypesStrings);
        databaseSchema.setClassIds (classIds);
        databaseSchema.setParentTableIds (parentClassIds);
        databaseSchema.setUserDefinedKeyCombinationsFieldNames (userDefinedKeyCombinationsFieldNamesForSchemaChange);
        databaseSchema.setUserDefinedKeyCombinationsFieldTypes (userDefinedKeyCombinationsFieldTypesForSchemaChange);
        databaseSchema.setFieldNamesExpandedTypes (managedObjectExtendedFieldTypes);
        databaseSchema.setIsLocalManagedObjectInfo (isLocalMOInfo);
        databaseSchema.setDerivedFromClassNames (derivedFromClassNames);

        PQclear (pResult);                                                                                                                                                       
    }                                                                                                                                                                            
                                                                                                                                                                                   
                                                                                                                                                                                   
    shutdownDatabase ();                                                                                                                                                         
    s_standaloneTransactionMutex.unlock ();                                                                                                                                      
                                                                                                                                                                                 
    return (numberOfResults);                                                                                                                                                    
}   

ResourceId DatabaseStandaloneTransaction::getTableNamesForWaveSchema (vector<string>& tableNames)
{
   /*s_standaloneTransactionMutex.lock ();
    bootDatabaseStep ();*/

    string      schema1             = "wavecurrent";
    PGconn      *pConnection        = getDatabaseConnection ();
    string      sqlForTableNames    = string ("SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema='" + schema1 + "';");
    //string sqlForTableNames       = string ("SELECT table_name FROM INFORMATION_SCHEMA.VIEWS WHERE table_schema1='wavecurrent';");
    ResourceId  status              = FRAMEWORK_ERROR;

    PGresult *pResult = PQexec (pConnection, sqlForTableNames.c_str());
    if (NULL == pResult)
    {
        trace (TRACE_LEVEL_WARN, "DatabaseStandaloneTransaction::getTableNamesForWaveSchema : SQL execution returned NULL result");
    }
    else if (PGRES_TUPLES_OK != PQresultStatus (pResult))
    {
        string errorMessage = string (PQresultErrorMessage (pResult));
        trace (TRACE_LEVEL_WARN, string("DatabaseStandaloneTransaction::getTableNamesForWaveSchema : SQL execution returned failure, type :") + PQresultStatus (pResult) + ", Error out: " + errorMessage);

        DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);
        trace (TRACE_LEVEL_WARN, string("DatabaseStandaloneTransaction::getTableNamesForWaveSchema : SQL execution returned failure, type :") + PQresultStatus (pResult) + ", Error out: " + PQresultErrorMessage (pResult));

        PQclear (pResult);
    }
    else
    {
        status = FRAMEWORK_SUCCESS;

        string  tableName       = "";
        UI32    numberOfResults = PQntuples (pResult);
        SI8     column          = PQfnumber (pResult, "table_name");

        tracePrintf (TRACE_LEVEL_WARN, true, false, "DatabaseStandaloneTransaction::getTableNamesForWaveSchema : numberOfResults: %d, column : %d, sqlForTableNames: %s", numberOfResults, column, sqlForTableNames.c_str());

        for (UI32 i = 0; i < numberOfResults; i++)
        {
            tableName = (PQgetvalue (pResult, i, column));

            trace (TRACE_LEVEL_INFO, string("DatabaseStandaloneTransaction::getTableNamesForWaveSchema : tableName - ") + tableName);
            
            tableNames.push_back(tableName);
        }
        PQclear (pResult);
    }

/*
    shutdownDatabase ();                                                                                                                                                         
    s_standaloneTransactionMutex.unlock ();                                                                                                                                      
*/

    return (status);
}

ResourceId DatabaseStandaloneTransaction::rollbackDatabase ()
{
    ResourceId status = FRAMEWORK_ERROR;

    s_standaloneTransactionMutex.lock ();                                                                                                                                        

    bootDatabaseStep ();

    PGconn *pConnection        = getDatabaseConnection ();
    
    prismAssert (NULL != pConnection, __FILE__, __LINE__);

    // 1. empty the database first.
    OrmRepository      *pOrmRepository  = OrmRepository::getInstance ();
    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    /*string commandString     = pOrmRepository->generateSqlToEmptyOrmDatabaseUsingTruncate (OrmRepository::getWaveCurrentSchema ());
           commandString    += pOrmRepository->generateSqlToEmptyOrmDatabaseUsingTruncate (OrmRepository::getWaveStartSchema   ());  */

    vector<string>  tableNames;
    getTableNamesForWaveSchema (tableNames);
    
    string commandString     = pOrmRepository->generateSqlToTruncateDatabaseSchemaForGivenTables (OrmRepository::getWaveCurrentSchema (), tableNames);
           commandString    += pOrmRepository->generateSqlToTruncateDatabaseSchemaForGivenTables (OrmRepository::getWaveStartSchema   (), tableNames);  

    trace (TRACE_LEVEL_DEBUG, string("DatabaseStandaloneTransaction::rollbackDatabase: sqlToEmpty : ") + commandString);

    PGresult *pResult   = PQexec (pConnection, commandString.c_str());

    if (NULL == pResult)
    {
        trace (TRACE_LEVEL_DEBUG, "DatabaseStandaloneTransaction::rollbackDatabase : SQL execution returned NULL result");
    }
    else if (PGRES_COMMAND_OK != PQresultStatus (pResult))
    {
        string errorMessage = string (PQresultErrorMessage (pResult));
        trace (TRACE_LEVEL_WARN, string("DatabaseStandaloneTransaction::rollbackDatabase : SQL execution returned failure, type :") + PQresultStatus (pResult) + ", Error out: " + errorMessage);

        DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);

        PQclear (pResult);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string("DatabaseStandaloneTransaction::rollbackDatabase : emptied the database successfully."));
        PQclear (pResult);
        status = FRAMEWORK_SUCCESS;
    }

    if (FRAMEWORK_SUCCESS != status)
    {
        shutdownDatabase ();
        s_standaloneTransactionMutex.unlock ();
        return (status);
    }

    // 2. rollback the database to its previous backup.
    string          backupFileName  = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName2 ());
                    commandString   = string (" pg_restore --data-only --single-transaction --format=c --dbname=") + (DatabaseObjectManager::getDatabaseName ()) + string (" --port=") + DatabaseObjectManager::getDatabasePort () + string (" ") + backupFileName;
    vector<string>  commandOutput;
    SI32            ret             = 0;
    
    ret = FrameworkToolKit::systemCommandOutput (commandString, commandOutput );

    if (0 != ret)
    {   
        tracePrintf(TRACE_LEVEL_ERROR, ("DatabaseStandaloneTransaction::rollbackDatabase : db restore failed. Error: [%d ]"), ret);
        status = FRAMEWORK_ERROR;
    }   
    else
    {   
        trace (TRACE_LEVEL_INFO, string("DatabaseStandaloneTransaction::rollbackDatabase : rollback of a database is successfully."));
        status = FRAMEWORK_SUCCESS;
    }   
 
    shutdownDatabase ();
    s_standaloneTransactionMutex.unlock ();

    return (status);
}

ResourceId DatabaseStandaloneTransaction::rollbackConfigFile (PrismFrameworkConfiguration& prismFrameworkConfiguration)
{
    ResourceId status = FRAMEWORK_ERROR;

    s_standaloneTransactionMutex.lock ();

    bootDatabaseStep ();

    PGconn *pConnection        = getDatabaseConnection ();

    prismAssert (NULL != pConnection, __FILE__, __LINE__);

    // 1. empty the database first.
    OrmRepository      *pOrmRepository  = OrmRepository::getInstance ();
    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    string      thisLocationIpAddress   = prismFrameworkConfiguration.getThisLocationIpAddress ();
    SI32        thisNodePort            = prismFrameworkConfiguration.getThisLocationPort ();
    LocationId  thisnodelocationid      = 0;

    string commandString = string ("SELECT * FROM " + OrmRepository::getWaveCurrentSchema () + "." + WaveNode::getClassName () + " WHERE (ipaddress = '" + thisLocationIpAddress + "') AND (port = '" + thisNodePort + "')");

    trace (TRACE_LEVEL_INFO, string("DatabaseStandaloneTransaction::rollbackDatabase: commandString : ") + commandString);

    PGresult *pResult   = PQexec (pConnection, commandString.c_str());

    if (NULL == pResult)
    {
        trace (TRACE_LEVEL_DEBUG, "DatabaseStandaloneTransaction::rollbackDatabase : SQL execution returned NULL result");
    }
    else 
    {
        if (PGRES_TUPLES_OK != PQresultStatus (pResult))
        {
            string errorMessage = string(PQresultErrorMessage (pResult));
            trace (TRACE_LEVEL_WARN, string("DatabaseStandaloneTransaction::getSchemaChangeInfo : SQL execution returned failure, type :") + PQresultStatus (pResult) + ", Error out: " + errorMessage);

            DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);
        }
        else
        {
            UI32 numberOfResults = PQntuples (pResult);                

            if (1 != numberOfResults)
            {
                prismAssert (false, __FILE__, __LINE__);
            }
            
            char* pValue = PQgetvalue (pResult, 0, PQfnumber (pResult, "locationid"));

            thisnodelocationid = strtoul (pValue, NULL, 10);

            status = FRAMEWORK_SUCCESS;
        }

       PQclear (pResult); 
    }

    shutdownDatabase ();
    s_standaloneTransactionMutex.unlock ();

    if (FRAMEWORK_SUCCESS != status)
    {
        return (status);
    }

    LocationRole locRole = prismFrameworkConfiguration.getThisLocationRole ();

    prismFrameworkConfiguration.setThisLocationLocationId   (thisnodelocationid);
    prismFrameworkConfiguration.setClusterPrimaryLocationId (thisnodelocationid);
    prismFrameworkConfiguration.setClusterPrimaryPort       (thisNodePort);

    if ((LOCATION_SECONDARY_CLUSTER_PHASE_0 == locRole) || (LOCATION_SECONDARY_CLUSTER_PHASE_1 == locRole) || (LOCATION_SECONDARY_CLUSTER_PHASE_2 == locRole) || (LOCATION_SECONDARY_CLUSTER_PHASE_3 == locRole))
    { 
        prismFrameworkConfiguration.setThisLocationRole         (LOCATION_STAND_ALONE);
        prismFrameworkConfiguration.setLastUsedLocationId       (thisnodelocationid);

        vector<string>      knownLocIpAddresses;
        vector<SI32>        knownLocPorts;
        vector<LocationId>  knownLocLocationIds;

        prismFrameworkConfiguration.setKnownLocationIpAddresses (knownLocIpAddresses); 
        prismFrameworkConfiguration.setKnownLocationPorts       (knownLocPorts);
        prismFrameworkConfiguration.setKnownLocationLocationIds (knownLocLocationIds);
    }     
    else if ((LOCATION_SECONDARY_REJOIN_PHASE_0 == locRole) || (LOCATION_SECONDARY_REJOIN_PHASE_1 == locRole) || (LOCATION_SECONDARY_REJOIN_PHASE_2 == locRole) || (LOCATION_SECONDARY_REJOIN_PHASE_3 == locRole))
    {   
        prismFrameworkConfiguration.setThisLocationRole         (LOCATION_SECONDARY_UNCONFIRMED);
    }
    
    prismFrameworkConfiguration.setDBRestoreIncomplete(false);
    
    string prismConfigurationfileName = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();

    status = prismFrameworkConfiguration.saveConfiguration (prismConfigurationfileName);

    return ((WAVE_MESSAGE_SUCCESS == status) ? FRAMEWORK_SUCCESS : FRAMEWORK_ERROR);
}

/* 
 Function Name: populateSchemaUpgradeTableInDb                                                                                                                                    
 Achieves     : 
    1. query columns classname and classid from the upgradeMO (moschemainfomanagedobject)
    2. form a map of <string, UI32> for <table-name, tbale-id.
    3. return number of tables queried from the upgrade MO.
*/

/*
UI32 DatabaseStandaloneTransaction::getTableIdsFromUpgradeMO (vector<pair<string, UI32> > &tableIdsFromUpgradeMO)
{
    tableIdsFromUpgradeMO.clear();

    s_standaloneTransactionMutex.lock ();                                                                                                                                        
    bootDatabaseStep ();                                                                                                                                                         

    string sqlToGetTableIds    = string ("SELECT t1.classname,t1.classid FROM " + OrmRepository::getWaveCurrentSchema () + "." + OrmRepository::getSchemaUpgradeTableName () + " t1;");
    UI32   numberOfResults     = 0;

    PGconn *pConnection = getDatabaseConnection ();                                                                                                                              
    PGresult *pResult   = PQexec (pConnection, sqlToGetTableIds.c_str());                                                                                        

    if (NULL == pResult)
    {
        trace (TRACE_LEVEL_WARN, "DatabaseStandaloneTransaction::getSchemaChangeInfo : SQL execution returned NULL result");
    }
    else if (PGRES_TUPLES_OK != PQresultStatus (pResult))
    {
        trace (TRACE_LEVEL_WARN, string("DatabaseStandaloneTransaction::getSchemaChangeInfo : SQL execution returned failure, type :") + PQresultStatus (pResult) + ", Error out: " + PQresultErrorMessage (pResult));
        PQclear (pResult);
    }
    else
    {
        numberOfResults = PQntuples (pResult);
        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "DatabaseStandaloneTransaction::getTableIdsFromUpgradeMO: #tables in UpgradeMo : %u", numberOfResults);

        for (UI32 i = 0; i < numberOfResults; ++i)
        {
            string table    = (PQgetvalue (pResult, i, PQfnumber (pResult, "classname")));

            char *pValue    = PQgetvalue (pResult, i, PQfnumber (pResult, "classid"));
            UI32 tableId    = strtoul (pValue, NULL, 10);

            tableIdsFromUpgradeMO.push_back (pair<string, UI32>(table, tableId));

            tracePrintf (TRACE_LEVEL_DEBUG, true, false, "DatabaseStandaloneTransaction::getTableIdsFromUpgradeMO: table : %s , table-id : %u", table.c_str(), tableId);
        }

        PQclear (pResult);
    }

    shutdownDatabase ();
    s_standaloneTransactionMutex.unlock ();

    return (numberOfResults);
}
*/
/*                                                                                                                                                                               
 Function Name: populateSchemaUpgradeTableInDb                                                                                                                                    
 Achieves     :                                                                                                                                                                   
     // 1. Delete all the rows from the table for MoSchemaInfoManagedObject/DROP and CREATE a table.                                                                              
     // 2. Get sql for populating all rows in the table from OrmRepository.                                                                                                       
     // 3. apply this sql.                                                                                                                                                        
*/                                                                                                                                                                               
/*
void DatabaseStandaloneTransaction::populateSchemaUpgradeTableInDb ()                                                                                                            
{                                                                                                                                                                                
    s_standaloneTransactionMutex.lock ();                                                                                                                                        
    bootDatabaseStep ();                                                                                                                                                         
                                                                                                                                                                                 
    string sqlToDropAndCreateSchemaUpgradeTable = string ("DELETE FROM " + OrmRepository::getWaveCurrentSchema () + "." + OrmRepository::getSchemaUpgradeTableName () + ";\n" 
                                                        + "DELETE FROM " + OrmRepository::getWaveStartSchema () + "." + OrmRepository::getSchemaUpgradeTableName () + ";\n");

    string sqlToInsertUpgradeTableInCurrentSchema = "";
    OrmRepository *pOrmRepository  = OrmRepository::getInstance ();                                                                                                              
    pOrmRepository->generateSqlToPopulateUpgradeTable (sqlToInsertUpgradeTableInCurrentSchema, OrmRepository::getWaveCurrentSchema ());                                          
                                                                                                                                                                                 
    string sqlToInsertUpgradeTableInStartSchema = "";                                                                                                                            
    pOrmRepository->generateSqlToPopulateUpgradeTable (sqlToInsertUpgradeTableInStartSchema, OrmRepository::getWaveStartSchema ());                                              
                                                                                                                                                                                 
    string sqlToDropAndRecreateUpgradeTable = sqlToDropAndCreateSchemaUpgradeTable + sqlToInsertUpgradeTableInCurrentSchema + sqlToInsertUpgradeTableInStartSchema;              

    trace (TRACE_LEVEL_DEBUG, "populateSchemaUpgradeTableInDb : sqlToDropAndRecreateUpgradeTable = " + sqlToDropAndRecreateUpgradeTable + "\n");                                  
                                                                                                                                                                                 
    PGconn *pConnection = getDatabaseConnection ();                                                                                                                              
    PGresult *pResult   = PQexec (pConnection, sqlToDropAndRecreateUpgradeTable.c_str());                                                                                        
                                                                                                                                                                                 
    if (NULL == pResult)                                                                                                                                                         
    {                                                                                                                                                                            
        trace (TRACE_LEVEL_WARN, "DatabaseStandaloneTransaction::getSchemaChangeInfo : SQL execution returned NULL result");                                                     
    } 
    else if (PGRES_COMMAND_OK != PQresultStatus (pResult))                                                                                                                        
    {                                                                                                                                                                            
        trace (TRACE_LEVEL_WARN, string("DatabaseStandaloneTransaction::getSchemaChangeInfo : SQL execution returned failure, type :") + PQresultStatus (pResult) + ", Error out: " + PQresultErrorMessage (pResult));
        PQclear (pResult);                                                                                                                                                       
    }                                                                                                                                                                            
    else                                                                                                                                                                         
    {                                                                                                                                                                            
        trace (TRACE_LEVEL_INFO, "DatabaseStandaloneTransaction::populateSchemaUpgradeTableInDb : successful.");                                                                 
    }                                                                                                                                                                            
                                                                                                                                                                                 
    shutdownDatabase ();                                                                                                                                                         
    s_standaloneTransactionMutex.unlock ();                                                                                                                                      
}  
*/
void DatabaseStandaloneTransaction::shutdownDatabase ()
{
    m_bootCount--;

    if (0 != m_bootCount)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, false, "DatabaseStandaloneTransaction::shutdownDatabase : m_bootCount : %d", m_bootCount);
        return;
    }

    if (m_needConnectionTeardown == false)
        return;

    DatabaseConnection *pDatabaseConnection = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    prismAssert (NULL != pDatabaseConnection, __FILE__, __LINE__);
    DatabaseConnection::deleteInstance ();

    trace (TRACE_LEVEL_INFO, "DatabaseStandaloneTransaction::shutdownDatabase : Shutting down database after standalone transaction");

    system ((string ("pg_ctl -D ") + DatabaseObjectManager::getDatabaseDirectory () + string (" stop")).c_str ());
    system ((string ("chmod 777 ") + DatabaseObjectManager::getDatabaseDirectory () + string ("/postmaster.pid")).c_str ());
    system ((string ("rm -rf ") + DatabaseObjectManager::getDatabaseDirectory () + string ("/postmaster.pid")).c_str ());

    trace (TRACE_LEVEL_DEVEL, "DatabaseStandaloneTransaction::shutdownDatabase : Done");
}

void DatabaseStandaloneTransaction::establishDbConnection ()
{
    s_standaloneTransactionMutex.lock ();

    bootDatabaseStep ();
    getDatabaseConnection ();

    s_standaloneTransactionMutex.unlock ();
}


ResourceId DatabaseStandaloneTransaction::dropAllDBViews ()
{
    s_standaloneTransactionMutex.lock ();
    bootDatabaseStep ();

    string      schema1             = "wavecurrent";
    PGconn      *pConnection        = getDatabaseConnection ();
    string      sqlTofindViews      = string ("SELECT table_name FROM INFORMATION_SCHEMA.VIEWS WHERE table_schema='" + schema1 + "';");
    string      sqlToDeleteViews    = "";
    ResourceId  status              = FRAMEWORK_ERROR;

    PGresult *pResult = PQexec (pConnection, sqlTofindViews.c_str());
    if (NULL == pResult)
    {
        trace (TRACE_LEVEL_WARN, "DatabaseStandaloneTransaction::dropAllDBViews : SQL execution returned NULL result");
    }
    else if (PGRES_TUPLES_OK != PQresultStatus (pResult))
    {
        string errorMessage = string (PQresultErrorMessage (pResult));
        trace (TRACE_LEVEL_WARN, string("DatabaseStandaloneTransaction::dropAllDBViews : SQL execution returned failure, sqlTofindViews :\n") + sqlTofindViews.c_str() + "\ntype :" + PQresultStatus (pResult) + ", Error out: " + errorMessage);

        DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);

        PQclear (pResult);
    }
    else
    {   
        string  viewName        = "";
        UI32    numberOfResults = PQntuples (pResult);
        SI8     column          = PQfnumber (pResult, "table_name");

        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "DatabaseStandaloneTransaction::dropAllDBViews : numberOfResults: %d, column : %d, sqlTofindViews: %s", numberOfResults, column, sqlTofindViews.c_str());

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_INFO, string("DatabaseStandaloneTransaction::dropAllDBViews : no views available in the database schema to drop."));
            status = FRAMEWORK_SUCCESS;
        }
        else
        {
            for (UI32 i = 0; i < numberOfResults; i++)
            {
                viewName = (PQgetvalue (pResult, i, column));

                trace (TRACE_LEVEL_DEBUG, string("DatabaseStandaloneTransaction::dropAllDBViews : viewName - ") + viewName);
                if (false == viewName.empty ())
                {
                    sqlToDeleteViews += "DROP VIEW IF EXISTS " + schema1 + "." + viewName + " CASCADE;";
                }
            }
            PQclear (pResult);

            pResult = PQexec (pConnection, sqlToDeleteViews.c_str());
            if (NULL == pResult)
            {   
                trace (TRACE_LEVEL_WARN, "DatabaseStandaloneTransaction::dropAllDBViews : SQL execution returned NULL result");
            }   
            else if (PGRES_COMMAND_OK != PQresultStatus (pResult))
            {   
                string errorMessage = string (PQresultErrorMessage (pResult));
                trace (TRACE_LEVEL_WARN, string("DatabaseStandaloneTransaction::dropAllDBViews : SQL execution returned failure, sqlToDeleteViews - \n") + sqlToDeleteViews + "\ntype :" + PQresultStatus (pResult) + ", Error out: " + errorMessage);

                DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);

                PQclear (pResult);
            }   
            else
            {   
                trace (TRACE_LEVEL_INFO, string("DatabaseStandaloneTransaction::dropAllDBViews : Successfully dropped all DB views."));
                status = FRAMEWORK_SUCCESS;
            }
        }
    }          
    
    shutdownDatabase ();    
    s_standaloneTransactionMutex.unlock ();
        
    return (status);
}

}
