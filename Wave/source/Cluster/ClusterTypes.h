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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#ifndef CLUSTERTYPES_H
#define CLUSTERTYPES_H

namespace WaveNs
{

#define                 CENTERAL_CLUSTER_HEART_BEAT_PERIOD          10000
#define                 MAX_LOST_HEARTBEAT                          3

typedef enum
{
    STAND_ALONE_NODE = 0,
    PRIMARY_NODE,
    SECONDARY_NODE
} NodeRole;

typedef enum
{
    CLUSTER_CREATE_CLUSTER         = 0,
    CLUSTER_DELETE_CLUSTER,
    CLUSTER_ADD_NODE,
    CLUSTER_JOIN_NODE,
    CLUSTER_DELETE_NODE,
    CLUSTER_DUPLICATE_COMMAND,
//    CLUSTER_REMOTE_COMMAND,
//    CLUSTER_REGISTER_HEART_BEAT_LOSS_CB,
//    CLUSTER_REGISTER_MEMBER_JOINED_CB,
//    CLUSTER_REGISTER_DUPLICTAE_COMMAND_HANDLER,
//    CLUSTER_REGISTER_VALIDATE_CLUSTER_HANDLER,
    CLUSTER_PRIMARY_NODE_CHANGED,
    CLUSTER_REPORT_REMOVED_NODE_FROM_CLUSTER,
//    CLUSTER_REPORT_LOCAL_HEART_BEAT,
//    CLUSTER_REPORT_CENTERAL_HEART_BEAT,
    CLUSTER_GET_NODE_ROLE,
    CLUSTER_GET_CLUSTER_CONFIG,
//    CLUSTER_SET_HEARTBEAT_CONFIG,
    CLUSTER_HEARTBEAT_LOST,
    CLUSTER_HEARTBEAT_RESUMED,
    CLUSTER_GET_DEBUGINFO,
    CENTRAL_CLUSTER_CONFIG_UPDATE_HARDWARE_SYNCHRONIZATION_STATE
} ClusterOpCodes;

typedef enum
{
    MANAGMENT_OPERATION_IDLE,
    MANAGMENT_OPERATION_IN_PROGRESS
} ManagmentOperationStatus;

}

#endif //CLUSTERTYPES_H
