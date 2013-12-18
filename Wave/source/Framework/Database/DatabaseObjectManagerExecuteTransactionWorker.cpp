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

#include "Framework/Database/DatabaseObjectManagerExecuteTransactionWorker.h"
#include "Framework/Database/DatabaseObjectManagerExecuteTransactionMessage.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Database/DatabaseConnection.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseStatisticsTracker.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForDebugInformation.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Shell/ShellDebug.h"

#include "libpq-fe.h"

namespace WaveNs
{

static UI32 s_totalNumberOfTransactionRequests = 0;
static UI32 s_numberOfCommandsExecuted         = 0;
static UI32 s_numberCommandsBetweenVacuums     = 10000;
static UI32 s_numberOfVacuumsAttempted         = 0;
static UI32 s_numberOfSuccessfulVacuums        = 0;


DatabaseObjectManagerExecuteTransactionWorker::DatabaseObjectManagerExecuteTransactionWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addDebugFunction ((ShellCmdFunction) (&DatabaseObjectManagerExecuteTransactionWorker::printTransactionStats), "printtransactionstats");

    addOperationMap (DATABASE_OBJECT_MANAGER_EXECUTE_TRANSACTION, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerExecuteTransactionWorker::executeTransactionMessageHandler));

    DatabaseStatisticsTracker::setVacuumInterval (s_numberCommandsBetweenVacuums);
}

DatabaseObjectManagerExecuteTransactionWorker::~DatabaseObjectManagerExecuteTransactionWorker ()
{
}

PrismMessage *DatabaseObjectManagerExecuteTransactionWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case DATABASE_OBJECT_MANAGER_EXECUTE_TRANSACTION :
            pPrismMessage = new DatabaseObjectManagerExecuteTransactionMessage ();
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("DatabaseObjectManagerExecuteTransactionWorker::createMessageInstance : Unknown operation code : ") + operationCode);
            prismAssert (false, __FILE__, __LINE__);
    }

    return (pPrismMessage);
}

void DatabaseObjectManagerExecuteTransactionWorker::executeTransactionMessageHandler (DatabaseObjectManagerExecuteTransactionMessage *pDatabaseObjectManagerExecuteTransactionMessage)
{
    s_totalNumberOfTransactionRequests++;
    DatabaseStatisticsTracker::incrementTransactionsRequested ();

    if (false == (DatabaseObjectManager::getIsDatabaseEnabled ()))
    {
        trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerExecuteTransactionWorker::executeTransactionMessageHandler : No support for Persistent Store will be provided.");
        pDatabaseObjectManagerExecuteTransactionMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pDatabaseObjectManagerExecuteTransactionMessage->setSql ("");
        reply (pDatabaseObjectManagerExecuteTransactionMessage);
        return;
    }

    const string &sql = pDatabaseObjectManagerExecuteTransactionMessage->getSql ();

    if ("" == sql)
    {
        pDatabaseObjectManagerExecuteTransactionMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pDatabaseObjectManagerExecuteTransactionMessage->setSql ("");
        reply (pDatabaseObjectManagerExecuteTransactionMessage);
        return;
    }

    ResourceId          status              = WAVE_MESSAGE_ERROR;
    DatabaseConnection *pDatabaseConnection = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    PGresult           *pResult             = NULL;
    ExecStatusType      databaseStatus      = PGRES_FATAL_ERROR;
    PGconn             *pConnection         = pDatabaseConnection->getPConnection ();

    prismAssert (NULL != pConnection, __FILE__, __LINE__);

    pResult = PQexec (pDatabaseConnection->getPConnection (), sql.c_str ());

    if (NULL == pResult)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        // Keep track of how many commands we executed successfully.  We will execute Vacuum regularly.
        if (pDatabaseObjectManagerExecuteTransactionMessage->getIsCommit ())
        {
            s_numberOfCommandsExecuted++;
            DatabaseStatisticsTracker::incrementTransactionsExecuted ();
        }

        databaseStatus = PQresultStatus (pResult);

        if (PGRES_COMMAND_OK == databaseStatus)
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            string  errorString   = PQresultErrorMessage (pResult);
            string  stringToFind  = "HINT:  Please REINDEX it";
            size_t  found         = errorString.find (stringToFind);

            if (found != string::npos)
            {
                trace (TRACE_LEVEL_ERROR, errorString);
                trace (TRACE_LEVEL_ERROR, "Need to REINDEX the WaveDatabase");

                string     stringToReIndex      = "REINDEX DATABASE \"WaveDatabase\";";
                PGresult  *pResultForReIndexing = NULL;

                pResultForReIndexing = PQexec (pDatabaseConnection->getPConnection (), stringToReIndex.c_str ());
                databaseStatus       = PQresultStatus (pResultForReIndexing);

                if (PGRES_COMMAND_OK != databaseStatus)
                {
                    status = WAVE_MESSAGE_ERROR;
                    string errorMessage = string (PQresultErrorMessage (pResultForReIndexing));
                    trace (TRACE_LEVEL_ERROR, errorMessage);
                    DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResultForReIndexing); 
                    prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    PQclear (pResult);
                    trace (TRACE_LEVEL_INFO, "Reindexing Completed");

                    pResult = PQexec (pDatabaseConnection->getPConnection (), sql.c_str ());
                    databaseStatus = PQresultStatus (pResult);

                    if (PGRES_COMMAND_OK != databaseStatus)
                    {
                        status = WAVE_MESSAGE_ERROR;
                        string errorMessage = string (PQresultErrorMessage (pResult));

                        trace (TRACE_LEVEL_ERROR, errorMessage);
                        trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + sql);

                        DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult); 
                    }
                    else
                    {
                        status = WAVE_MESSAGE_SUCCESS;
                    }
                }

                PQclear (pResultForReIndexing);
            }
            else if (true == (pDatabaseObjectManagerExecuteTransactionMessage->getIsPrepare ()))  // If prepare failed
            {
                string     stringToRollback = string ("ROLLBACK");
                PGresult  *pResultForRollback = NULL;

                pResultForRollback = PQexec (pDatabaseConnection->getPConnection (), stringToRollback.c_str ());
                databaseStatus     = PQresultStatus (pResultForRollback);

                if (PGRES_COMMAND_OK != databaseStatus)
                {
                    status = WAVE_MESSAGE_ERROR;
                    string errorMessage = string (PQresultErrorMessage (pResultForRollback));
                    trace (TRACE_LEVEL_ERROR, errorMessage);
                    DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResultForRollback);
                    prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerExecuteTransactionWorker::executeTransactionMessageHandler : failed trasaction at prepare phase is rolled back.");
                    status = WAVE_MESSAGE_ERROR;
                    string errorMessage = string (PQresultErrorMessage (pResult));
                    trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerExecuteTransactionWorker::executeTransactionMessageHandler : ERROR in executing the Database Transaction at prepare phase:");
                    trace (TRACE_LEVEL_ERROR, errorMessage);
                    trace (TRACE_LEVEL_ERROR, "The Errored out SQL was : " + sql);

                    DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult); 
                }

                PQclear (pResultForRollback);
            }
            else 
            {
                status = WAVE_MESSAGE_ERROR;
                string errorMessage = string (PQresultErrorMessage (pResult));
                trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerExecuteTransactionWorker::executeTransactionMessageHandler : ERROR in executing the Database Transaction :");
                trace (TRACE_LEVEL_ERROR, errorMessage);
                trace (TRACE_LEVEL_ERROR, "The Errored out SQL was : " + sql);

                DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult); 
            }
        }

        PQclear (pResult);
    }

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        if (true == pDatabaseObjectManagerExecuteTransactionMessage->getIsCommit ())
        {
            DatabaseObjectManager::executeDatabaseCommitClientSubscriptionFunction (true);
            
            DatabaseStatisticsTracker::incrementTransactionsCommitted ();
        }
    }

    pDatabaseObjectManagerExecuteTransactionMessage->setCompletionStatus (status);
    pDatabaseObjectManagerExecuteTransactionMessage->setSql ("");
    reply (pDatabaseObjectManagerExecuteTransactionMessage);

    // Now vacuum the database if we executed the required number of transactions already.

    if ((0 == (s_numberOfCommandsExecuted % s_numberCommandsBetweenVacuums)) && (0 < s_numberOfCommandsExecuted))
    {
        trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerExecuteTransactionWorker::executeTransactionMessageHandler : VACUUM ing the Database started.");
        pResult = PQexec (pDatabaseConnection->getPConnection (), "VACUUM;");
        s_numberOfVacuumsAttempted++;
        DatabaseStatisticsTracker::incrementVacuumsRequested ();

        if (NULL == pResult)
        {
            trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerExecuteTransactionWorker::executeTransactionMessageHandler : ERROR in VACUUM ing the Database.  NULL Result.");
        }
        else
        {
            databaseStatus = PQresultStatus (pResult);

            if (PGRES_COMMAND_OK != databaseStatus)
            {
                status = WAVE_MESSAGE_ERROR;
                trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerExecuteTransactionWorker::executeTransactionMessageHandler : ERROR in VACUUM ing the Database :");
                trace (TRACE_LEVEL_ERROR, string (PQresultErrorMessage (pResult)));
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerExecuteTransactionWorker::executeTransactionMessageHandler : VACUUM ing the Database completed.");
                s_numberOfSuccessfulVacuums++;
                DatabaseStatisticsTracker::incrementVacuumsExecuted ();
            }

            PQclear (pResult);
        }
    }
}

ResourceId DatabaseObjectManagerExecuteTransactionWorker::printTransactionStats (UI32 argc, vector<string> argv)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "");
    WaveNs::trace (TRACE_LEVEL_INFO, string ("Total Number of Transaction Requests        : ") + s_totalNumberOfTransactionRequests);
    WaveNs::trace (TRACE_LEVEL_INFO, string ("Total Number of Transactions Executed       : ") + s_numberOfCommandsExecuted);
    WaveNs::trace (TRACE_LEVEL_INFO, string ("Vacuuming Interval                          : ") + s_numberCommandsBetweenVacuums);
    WaveNs::trace (TRACE_LEVEL_INFO, string ("Total Number of Vacuums Attempted           : ") + s_numberOfVacuumsAttempted);
    WaveNs::trace (TRACE_LEVEL_INFO, string ("Total Number of Successful Vacuums Requests : ") + s_numberOfSuccessfulVacuums);

    return (0);
}

void DatabaseObjectManagerExecuteTransactionWorker::getDebugInformation (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation)
{
    string debugInformation = DatabaseStatisticsTracker::getAllTransactionStatistics ();

    if (0 < debugInformation.size ())
    {
        debugInformation = "\nDATABASE OBJECT MANAGER EXECUTE TRANSACTION WORKER STATISTICS:\n\n" + debugInformation;

        pWaveAsynchronousContextForDebugInformation->setDebugInformation (debugInformation);
    }

    pWaveAsynchronousContextForDebugInformation->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForDebugInformation->callback ();
}

void DatabaseObjectManagerExecuteTransactionWorker::resetDebugInformation (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation)
{ 
    DatabaseStatisticsTracker::clearAllTransactionStatistics ();

    pWaveAsynchronousContextForDebugInformation->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForDebugInformation->callback ();
}

}
