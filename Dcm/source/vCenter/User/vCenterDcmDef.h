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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.                *
 *   All rights reserved.                                                   *
 *   Author : Mobin Mohan                                                   *
 *   File: vCenterDcmDef.h                                                 *
 *   Description:                                                           *
 *                                                                          *
 ***************************************************************************/

#ifndef VCENTERDCMDEF_H
#define VCENTERDCMDEF_H


#include <stdio.h>
#include <unistd.h>

#define	DISCOVER_INTERVAL_DEFAULT_MIN	30
#define DISCOVER_IGNORE_COUNT           0

#define VN_DEBUG_CLI_TKN_MAX_LEN    32
#define VN_DEBUG_CLI_TKN_LOG_SIZE   "log-size"
#define VN_DEBUG_CLI_TKN_LOG_COUNT  "log-count"
#define VN_DEBUG_CLI_TKN_LOG_LEVEL  "log-level"
#define VN_SHOW_DEBUG_CLI_TKN       "vnetwork"

typedef enum VnetworkDebugCmdCode_ {
    VN_DEBUG_CMD_UNKNOWN = 0,
    VN_DEBUG_CMD_LOG_SIZE,
    VN_DEBUG_CMD_LOG_COUNT,
    VN_DEBUG_CMD_LOG_LEVEL,
    VN_SHOW_DEBUG_CMD
} VnetworkDebugCmdCode_t;

#define	VN_DEFAULT_LOG_SIZE     (1024*1024)
#define	VN_DEFAULT_LOG_COUNT    5
#define	VN_DEFAULT_LOG_LEVEL    "INFO"

#endif  //VCENTERDCMDEF_H

