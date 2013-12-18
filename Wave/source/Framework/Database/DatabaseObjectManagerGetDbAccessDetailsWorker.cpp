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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Database/DatabaseObjectManagerGetDbAccessDetailsWorker.h"
#include "Framework/Database/DatabaseObjectManagerGetDbAccessDetailsMessage.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Database/DatabaseObjectManager.h"

namespace WaveNs
{

DatabaseObjectManagerGetDbAccessDetailsWorker::DatabaseObjectManagerGetDbAccessDetailsWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (DATABASE_OBJECT_MANAGER_GET_DB_ACCESS_DETAILS, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManagerGetDbAccessDetailsWorker::getDbAccessDetailsMessageHandler));
    addServiceIndependentOperationMap (DATABASE_OBJECT_MANAGER_GET_DB_ACCESS_DETAILS, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&DatabaseObjectManagerGetDbAccessDetailsWorker::getDbAccessDetailsMessageStaticHandler));
}

DatabaseObjectManagerGetDbAccessDetailsWorker::~DatabaseObjectManagerGetDbAccessDetailsWorker ()
{
}

PrismMessage *DatabaseObjectManagerGetDbAccessDetailsWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case DATABASE_OBJECT_MANAGER_GET_DB_ACCESS_DETAILS :
            pPrismMessage = new DatabaseObjectManagerGetDbAccessDetailsMessage ();
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("DatabaseObjectManagerGetDbAccessDetailsWorker::createMessageInstance : Unknown operation code : ") + operationCode);
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (pPrismMessage);
}

void DatabaseObjectManagerGetDbAccessDetailsWorker::getDbAccessDetailsMessageHandler (DatabaseObjectManagerGetDbAccessDetailsMessage *pDatabaseObjectManagerGetDbAccessDetailsMessage)
{
    DatabaseObjectManagerGetDbAccessDetailsWorker::getDbAccessDetailsMessageStaticHandler (pDatabaseObjectManagerGetDbAccessDetailsMessage);
    reply (pDatabaseObjectManagerGetDbAccessDetailsMessage);
}

void DatabaseObjectManagerGetDbAccessDetailsWorker::getDbAccessDetailsMessageStaticHandler (DatabaseObjectManagerGetDbAccessDetailsMessage *pDatabaseObjectManagerGetDbAccessDetailsMessage)
{
    pDatabaseObjectManagerGetDbAccessDetailsMessage->setDatabaseName (DatabaseObjectManager::getDatabaseName ());
    pDatabaseObjectManagerGetDbAccessDetailsMessage->setPort         (DatabaseObjectManager::getDatabasePort ());

    pDatabaseObjectManagerGetDbAccessDetailsMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
}

}
