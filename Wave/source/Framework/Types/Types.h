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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <limits.h>
#include <assert.h>
#include "Framework/OsLayer/PrismOsLayer.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

typedef unsigned long long  int  UI64;
typedef signed   long long  int  SI64;
typedef unsigned            int  UI32;
typedef unsigned short      int  UI16;
typedef signed              int  SI32;
typedef signed   short      int  SI16;
typedef unsigned            char UI8;
typedef signed              char SI8;
typedef string                   WaveUnion;
typedef UI32                     PrismServiceId;
typedef UI32                     ResourceId;
typedef ResourceId               ResourceEnum;
typedef UI32                     TraceClientId;
typedef UI32                     SerializableObjectType;
typedef UI32                     SerializableObjectAttributeId;
typedef UI32                     LocationId;
typedef UI32                     TimerHandle;

typedef unsigned long       int  ULI;  // On 32 bit machines it is same as UI32 and on 64 bit machines it is UI64

extern const SI64 WAVE_LLONG_MAX;
extern const SI64 WAVE_LLONG_MIN;
extern const UI64 WAVE_ULLONG_MAX;
extern const UI32 WAVE_INVALID_SLOT;

typedef enum
{
    // There cannot be more than 1000 entries here

    WAVE_OBJECT_MANAGER_INITIALIZE = UINT_MAX - 1001,
    WAVE_OBJECT_MANAGER_LISTEN_FOR_EVENTS,
    WAVE_OBJECT_MANAGER_ENABLE,
    WAVE_OBJECT_MANAGER_INSTALL,
    WAVE_OBJECT_MANAGER_BOOT,
    WAVE_OBJECT_MANAGER_POSTBOOT,
    WAVE_OBJECT_MANAGER_HEARTBEAT_FAILURE,
    WAVE_OBJECT_MANAGER_CONFIG_REPLAY_END,
    WAVE_OBJECT_MANAGER_SLOT_FAILOVER,
    WAVE_OBJECT_MANAGER_EXTERNAL_STATE_SYNCHRONIZATION,
    WAVE_OBJECT_MANAGER_DISABLE,
    WAVE_OBJECT_MANAGER_SHUTDOWN,
    WAVE_OBJECT_MANAGER_UNINSTALL,
    WAVE_OBJECT_MANAGER_UNINITIALIZE,
    WAVE_OBJECT_MANAGER_DESTRUCT,
    WAVE_OBJECT_MANAGER_PING,
    WAVE_OBJECT_MANAGER_PREPARE_FOR_REGRESSION,
    WAVE_OBJECT_MANAGER_PREPARE_FOR_REGRESSION2,
    WAVE_OBJECT_MANAGER_TEST,
    WAVE_OBJECT_MANAGER_TIMER_EXPIRED,
    WAVE_OBJECT_MANAGER_CREATE_CLUSTER_COLLECT_VALIDATION_DATA,
    WAVE_OBJECT_MANAGER_CREATE_CLUSTER_VALIDATE,
    WAVE_OBJECT_MANAGER_CREATE_CLUSTER_SEND_VALIDATION_RESULTS,
    WAVE_OBJECT_MANAGER_FAILOVER,

    WAVE_OBJECT_MANAGER_REGISTER_EVENT_LISTENER,

    WAVE_OBJECT_MANAGER_PAUSE,
    WAVE_OBJECT_MANAGER_RESUME,

    WAVE_OBJECT_MANAGER_SET_CPU_AFFINITY,

    WAVE_OBJECT_MANAGER_LOAD_OPERATIONAL_DATA_FOR_MANAGED_OBJECT,

    WAVE_OBJECT_MANAGER_BACKEND_SYNC_UP,

    WAVE_OBJECT_MANAGER_MESSAGE_HISTORY_CONFIG,
    WAVE_OBJECT_MANAGER_MESSAGE_HISTORY_DUMP,

    WAVE_OBJECT_MANAGER_UPDATE_MANAGED_OBJECT,
    WAVE_OBJECT_MANAGER_CREATE_MANAGED_OBJECT,

    WAVE_OBJECT_MANAGER_DELETE_MANAGED_OBJECT,
    WAVE_OBJECT_MANAGER_FIPS_ZEROIZE,
    WAVE_OBJECT_MANAGER_UPGRADE,

    WAVE_OBJECT_MANAGER_HA_SYNC_COLLECT_VALIDATION_DATA,
    WAVE_OBJECT_MANAGER_HA_SYNC_VALIDATE_DATA,
    WAVE_OBJECT_MANAGER_HA_SYNC_SEND_VALIDATION_RESULTS,

    WAVE_OBJECT_MANAGER_HAINSTALL,
    WAVE_OBJECT_MANAGER_HABOOT,

    WAVE_OBJECT_MANAGER_GET_DEBUG_INFORMATION,
    WAVE_OBJECT_MANAGER_RESET_DEBUG_INFORMATION,

    WAVE_OBJECT_MANAGER_UPDATE_RELATIONSHIP,

    WAVE_OBJECT_MANAGER_FILE_REPLAY_END,

    // Multi-Parttion cleanup.
    WAVE_OBJECT_MANAGER_PARTITION_CLEANUP,

    // Get Data From client
    WAVE_OBJECT_MANAGER_GET_CLIENT_DATA,

    //Bulk delete in Unified
    WAVE_OBJECT_MANAGER_DELETE_MANAGED_OBJECTS,

    WAVE_OBJECT_MANAGER_DATABASE_SANITY_CHECK,

    // Wave Broker Publish Message delivery

    WAVE_OBJECT_MANAGER_DELIVER_WAVE_BROKER_PUBLISH_MESSAGE,

    // The following must be the last two.
    // These opcodes are currently limited to be used by only
    // the remote transport service.

    WAVE_OBJECT_MANAGER_ANY_OPCODE = UINT_MAX - 1,
    WAVE_OBJECT_MANAGER_ANY_EVENT  = UINT_MAX
} FrameworkOpcodes;

typedef enum {
    WAVE_MESSAGE_SEND_TYPE_ONEWAY = 1,
    WAVE_MESSAGE_SEND_TYPE_SYNCHRONOUS,
    WAVE_MESSAGE_SEND_TYPE_ASYNCHRONOUS
} WaveMessageSendType;

typedef enum {
    WAVE_MESSAGE_HISTORY_LOG_AT_SEND = 1,
    WAVE_MESSAGE_HISTORY_LOG_AT_REPLY,
    WAVE_MESSAGE_HISTORY_LOG_AT_HANDLE
} WaveMessageHistoryLogType;

}

#endif //TYPES_H

