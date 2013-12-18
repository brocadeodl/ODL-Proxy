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

#ifndef PERSISTENCEOBJECTMANAGERTYPES_H
#define PERSISTENCEOBJECTMANAGERTYPES_H

namespace WaveNs
{

typedef enum
{
    PERSISTENCE_OBNJECT_MANAGER_EXECUTE_TRANSACTION,
    PERSISTENCE_OBNJECT_MANAGER_COPY_SCHEMA,
    PERSISTENCE_OBNJECT_MANAGER_COPY_FILE,
    PERSISTENCE_OBNJECT_MANAGER_COMPARE_DB,
    PERSISTENCE_OBJECT_MANAGER_INITIALIZE_BEFORE_BOOT_COMPLETE,
    PERSISTENCE_OBJECT_MANAGER_UPDATE_INSTANCE_ID,
    PERSISTENCE_OBJECT_MANAGER_PRINT_ORM,
    PERSISTENCE_OBJECT_MANAGER_ADD_XPATH_STRINGS_MESSAGE,
    PERSISTENCE_OBJECT_MANAGER_DELETE_XPATH_STRINGS_MESSAGE,
    PERSISTENCE_OBJECT_MANAGER_GET_LAST_UPDATE_TIMESTAMPS_FOR_XPATH_STRINGS_MESSAGE,
    PERSISTENCE_OBJECT_MANAGER_RESET_XPATH_STRINGS_TIMESTAMPS,
    PERSISTENCE_OBJECT_MANAGER_GET_ALL_CLASS_NAMES,
    PERSISTENCE_OBJECT_MANAGER_ADD_DELAYED_TRANSACTION
} PersistenceObjectManagerTypes;

typedef enum
{
    PERSISTENCE_SET_STARTUP_FILE,
    PERSISTENCE_SAVE_PRISM_CONFIGURATION,
    PERSISTENCE_COPY_SCHEMA,
    PERSISTENCE_COPY_DEFAULT_CLUSTER,
    PERSISTENCE_SET_CONFIGURATION_VALID,
    PERSISTENCE_DEBUG_SCHEMA_CHANGE,
    STARTUP_SCHEMA_CHANGE_EVENT,
    PERSISTENCE_LOCAL_OBJECT_MANAGER_UPDATE_INSTANCE_ID, 
    PERSISTENCE_LOCAL_OBJECT_MANAGER_EXECUTE_TRANSACTION,
    PERSISTENCE_LOCAL_OBJECT_MANAGER_ENABLE_TRANSACTION,
    PERSISTENCE_SET_LIVE_SYNC_ENABLE,
    PERSISTENCE_GET_LIVE_SYNC_ENABLE,
    PERSISTENCE_PUSH_CONFIG_TO_KERNEL
} PersistenceLocalObjectManagerTypes;

}

#endif // PERSISTENCEOBJECTMANAGERTYPES_H
