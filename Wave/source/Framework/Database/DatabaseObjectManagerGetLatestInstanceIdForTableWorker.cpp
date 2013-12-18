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

#include "Framework/Database/DatabaseObjectManagerGetLatestInstanceIdForTableWorker.h"
#include "Framework/Database/DatabaseObjectManagerGetLatestInstanceIdForTableMessage.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Database/DatabaseConnection.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"

namespace WaveNs
{

DatabaseObjectManagerGetLatestInstanceIdForTableWorker::DatabaseObjectManagerGetLatestInstanceIdForTableWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (DATABASE_OBJECT_MANAGER_GET_LATEST_INSTANCE_ID_FOR_TABLE, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerGetLatestInstanceIdForTableWorker::getLatestInstanceIdForTableMessageHandler));
}

DatabaseObjectManagerGetLatestInstanceIdForTableWorker::~DatabaseObjectManagerGetLatestInstanceIdForTableWorker ()
{
}

void DatabaseObjectManagerGetLatestInstanceIdForTableWorker::getLatestInstanceIdForTableMessageHandler (DatabaseObjectManagerGetLatestInstanceIdForTableMessage *pDatabaseObjectManagerGetLatestInstanceIdForTableMessage)
{
    trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerGetLatestInstanceIdForTableWorker::getLatestInstanceIdForTableMessageHandler : Starting ...");

    if (false == (DatabaseObjectManager::getIsDatabaseEnabled ()))
    {
        trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManagerGetLatestInstanceIdForTableWorker::getLatestInstanceIdForTableMessageHandler : No support for Persistent Store will be provided.");
        pDatabaseObjectManagerGetLatestInstanceIdForTableMessage->setLatestInstabceId (0);
        pDatabaseObjectManagerGetLatestInstanceIdForTableMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pDatabaseObjectManagerGetLatestInstanceIdForTableMessage);
        return;
    }

    string              schema              = OrmRepository::getWaveCurrentSchema ();
    string              tableName           = pDatabaseObjectManagerGetLatestInstanceIdForTableMessage->getTableName ();
    string              sql                 = "SELECT objectIdInstanceId FROM " + schema + "." + tableName + " WHERE objectIdInstanceId > " + (((UI64) (FrameworkToolKit::getThisLocationId ()) + (FrameworkToolKit::getHaId ())) << 32) + " AND objectIdInstanceId <= " + ((((UI64) (FrameworkToolKit::getThisLocationId ()) + (FrameworkToolKit::getHaId ())) << 32) + ((UI64) 0xFFFFFFFF)) + " ORDER BY objectIdInstanceId DESC LIMIT 1;";
    DatabaseConnection *pDatabaseConnection = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    PGresult           *pResult             = NULL;
    ExecStatusType      databaseStatus      = PGRES_FATAL_ERROR;
    PGconn             *pConnection         = pDatabaseConnection->getPConnection ();
    ResourceId          status              = WAVE_MESSAGE_SUCCESS;
    UI64                latestInstanceId    = 0;

    prismAssert (NULL != pConnection, __FILE__, __LINE__);

    trace (TRACE_LEVEL_DEBUG, string ("This Location ID : ") + FrameworkToolKit::getThisLocationId ());
    trace (TRACE_LEVEL_DEBUG, sql);

    pResult = PQexec (pDatabaseConnection->getPConnection (), sql.c_str ());

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

            UI32 numberOfResults  = PQntuples (pResult);

            if (numberOfResults < 2)
            {
                if (1 == numberOfResults)
                {
                    UI32  row    = 0;
                    UI32  column = PQfnumber (pResult, "objectIdInstanceId");
                    char *pValue = PQgetvalue (pResult, row, column);

                    latestInstanceId = strtoull (pValue, NULL, 10);
                }
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string ("DatabaseObjectManagerGetLatestInstanceIdForTableWorker::getLatestInstanceIdForTableMessageHandler : Unexpected Results for Database Query : ") + numberOfResults);
            }

        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
            string errorMessage = string (PQresultErrorMessage (pResult));
            trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerGetLatestInstanceIdForTableWorker::getLatestInstanceIdForTableMessageHandler : ERROR in executing the Database Query :");
            trace (TRACE_LEVEL_ERROR, errorMessage);
            trace (TRACE_LEVEL_ERROR, "The Errored out SQL was :\n" + sql);
            DatabaseObjectManager::handleIfDBCorruption (errorMessage, pResult);
        }

        PQclear (pResult);
    }

    pDatabaseObjectManagerGetLatestInstanceIdForTableMessage->setLatestInstabceId (latestInstanceId & ((UI64) 0xFFFFFFFF));
    pDatabaseObjectManagerGetLatestInstanceIdForTableMessage->setCompletionStatus (status);

    reply (pDatabaseObjectManagerGetLatestInstanceIdForTableMessage);
}

}
