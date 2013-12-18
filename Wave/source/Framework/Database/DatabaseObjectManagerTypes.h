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

#ifndef DATABASEOBJECTMANAGERTYPES_H
#define DATABASEOBJECTMANAGERTYPES_H

namespace WaveNs
{

typedef enum
{
    DATABASE_OBJECT_MANAGER_EXECUTE_TRANSACTION,
    DATABASE_OBJECT_MANAGER_EXECUTE_QUERY,
    DATABASE_OBJECT_MANAGER_EXECUTE_QUERY_FOR_COUNT,
    DATABASE_OBJECT_MANAGER_GET_LATEST_INSTANCE_ID_FOR_TABLE,
    DATABASE_OBJECT_MANAGER_BACKUP,
    DATABASE_OBJECT_MANAGER_RESTORE,
    DATABASE_OBJECT_MANAGER_EMPTY,
    DATABASE_OBJECT_MANAGER_CLEAN_PREPARED_TRANSACTIONS,
    DATABASE_OBJECT_MANAGER_EXECUTE_COPY_SCHEMA,
	DATABASE_OBJECT_MANAGER_EXECUTE_CURSOR_CFG_COMMAND,
	DATABASE_OBJECT_MANAGER_EXECUTE_CURSOR_QUERY_COMMAND,
    DATABASE_OBJECT_MANAGER_CALCULATE_DB_CKSUM,
    DATABASE_OBJECT_MANAGER_SHUTDOWN,
    DATABASE_OBJECT_MANAGER_CREATE_INMEMORY_MO,
    DATABASE_OBJECT_MANAGER_GET_DB_ACCESS_DETAILS,
    DATABASE_OBJECT_MANAGER_DEBUG_CHANGE_QUERY_SETTINGS,
    DATABASE_OBJECT_MANAGER_GET_DB_SCHEMA_INFO,
    DATABASE_OBJECT_MANAGER_STOP,
    DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_PARAMETER_CONFIGURE,
    DATABASE_OBJECT_MANAGER_DROP_DB_VIEWS
} DatabaseObjectManagerTypes;

typedef enum
{
    DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_NOT_DEFINED,
    DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_DELETE,
    DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_TRUNCATE,
    DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_AUTO_DETECTION
} DatabaseObjectManagerEmptyDatabaseTypes;

}

#endif // DATABASEOBJECTMANAGERTYPES_H
