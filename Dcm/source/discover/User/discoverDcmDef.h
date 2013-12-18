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
 *   File: discoverDcmDef.h                                                 *
 *   Description:                                                           *
 *                                                                          *
 ***************************************************************************/

#ifndef DISCOVERDCMDEF_H
#define DISCOVERDCMDEF_H


#include <stdio.h>
#include <unistd.h>

#define DISC_PLUGIN_LOCAL_VERSION   0
#define DISC_PLUGIN_GLOBAL_VERSION  0

#define DISC_GLOBAL_PLUGIN_SCRIPTS  ("discover")
#define DISC_LOCAL_PLUGIN_SCRIPTS   ("pp_create copy") 
#define DISC_PPCLEAR_PLUS_LOCAL_PLUGIN_SCRIPTS  ("pp_clear pp_create copy") 
#define DISC_PPCLEAR_SCRIPTS        ("pp_clear") 
#define DISC_PPCONFIG_SCRIPTS       ("pp_create") 
#define DISC_COPY_SCRIPTS           ("copy") 
#define DISC_PERSIST_SCRIPTS        ("persist")


#define	DISCOVER_INTERVAL_DEFAULT	180000
#define	DISCOVER_INTERVAL_ZERO      0
#define	MIN_TO_MILLISEC(x)          ((x)*60*1000)

typedef enum discoverVcenterCmdCode_ {
    DISC_CMD_UNKNOWN = 0,
    DISC_CMD_SHOW_STATUS_VCENTER, 
    DISC_CMD_SHOW_STATUS_ALL_VCENTER,
    DISC_CMD_SHOW_VCENTER_INTERNAL_HIST, 
    DISC_CMD_DISCOVER_VCENTER,
    DISC_CMD_DISCOVER_ALL_VCENTER,
    DISC_CMD_DISCOVER_TIMER,
    DISC_CMD_NODE_ADD,
    DISC_CMD_NODE_COPY_RUNNING_TO_STARTUP,
    DISC_CMD_SYNC_PERSISTED_WITH_RUNNING,
    DISC_CMD_START_CORELATION,
    DISC_CMD_CORELATION_DELETE_VCENTER,
    DISC_CMD_DISCOVER_ALL_VCENTER_ON_NODE_STATRUP,
    DISC_CMD_START_RECONCILE,
    DISC_CMD_NODE_ADD_VCENTER_NAMES,
    DISC_CMD_START_CORRELATION_ON_NODE_REJOIN

    
} discoverVcenterCmdCode_t;

typedef enum discoverUpdateCmdCode_ {
    DISC_CMD_UPDATE_DISCOVER_STATUS = 0,
    DISC_CMD_DELETE_VCENTER_MO


} discoverUpdateCmdCode_t;

typedef enum discoverVcenterMsgType_ {
    DISC_MSG_TYPE_CREATE = 0,
    DISC_MSG_TYPE_UPDATE,
    DISC_MSG_TYPE_DELETE,
    DISC_MSG_TYPE_PERSIST,
    DISC_MSG_TYPE_REMOVE_PERSIST,
    DISC_MSG_TYPE_DEACTIVATE
} discoverVcenterMsgType_t;

typedef enum discoverInternalMsgType_ {
    DISC_INT_TYPE_GET_STATE_MACHINES = 0

} discoverInternalMsgType_t;


typedef enum discoverMgmtErrCode_ { 
    DISC_MGMT_ERR_SUCCESS = 0,
    DISC_MGMT_ERR_FAILURE,
    DISC_MGMT_ERR_UNKNOWN_VCENTER,
    DISC_MGMT_ERR_MAX
} discoverMgmtErrCode_t;

typedef enum discoverStages_ {
    DISC_STAGE_DISCOVERY_BEGIN = 0,
    DISC_STAGE_DISCOVERY_SUCCESS,
    DISC_STAGE_DISCOVERY_FAILURE,
    DISC_STAGE_DISCOVERY_FINISH,
    DISC_STAGE_DISCOVERY_MAX
} discoverStages_t;

typedef enum discoverStateMachineStates_ {
    DISC_STATE_UNKNOWN = 0,
    DISC_STATE_START,
    DISC_STATE_STOP_TIMER,
    DISC_STATE_DISCOVER_START,
    DISC_STATE_DISCOVER_END,
    DISC_STATE_DISTRIBUTION,
    DISC_STATE_PPCONFIG_COPY_START,
    DISC_STATE_PPCONFIG_COPY_END,
    DISC_STATE_START_TIMER,
    DISC_STATE_END,
    DISC_STATE_MAX
} discoverStageMachineStates_t;

typedef enum discoverExitStatus_ {
    DISC_EXIT_STATUS_SUCCESS,
    DISC_EXIT_STATUS_NOT_ACTIVATE,
    DISC_EXIT_STATUS_LOST_CONNECTION,
    DISC_EXIT_STATUS_BAD_CREDENTIAL,
    DISC_EXIT_STATUS_UNSUPPORTED_VERSION,
    DISC_EXIT_STATUS_NO_VCENTER_SERVICE,
	DISC_EXIT_STATUS_INVALID,
    DISC_EXIT_STATUS_MAX
} discoverExitStatus_t;


typedef struct discoverStatus_ {
	time_t sTime;
	time_t eTime;
	discoverStages_t dStage;
} discoverStatus_t;

typedef struct discoverStatusRecord_ {
	string vCenter;
	vector<discoverStatus_t *> status;
} discoverStatusRecord_t;

typedef struct discoverShowStatusRecord_ {
	string vCenter;
	u_int8_t nStatus;
	discoverStatus_t status[5];
} discoverShowStatusRecord_t;

typedef struct discoverShowStatusResp_ { 
    discoverMgmtErrCode_t   retCode;
    u_int32_t               len;
    u_int32_t               nRecords;
    discoverShowStatusRecord_t	records[1];
} discoverShowStatusResp_t;

#define DISC_CLI_TKN_MAX_LEN    32
#define DISC_STATUS_STAGE_STR_MAX_LEN    64
#define DISC_CLI_TKN_DISC   "discover"
#define DISC_CLI_TKN_STATUS   "status"
#define DISC_CLI_TKN_INTERNAL   "internal"
#define DISC_CLI_TKN_HISTORY   "history"

typedef struct disc_status_stage_str_ { 
    char    str[DISC_STATUS_STAGE_STR_MAX_LEN + 1];
} disc_status_stage_str_t;

typedef struct disc_exit_status_str_ { 
    char    str[DISC_STATUS_STAGE_STR_MAX_LEN + 1];
} disc_exit_status_str_t;

static inline char* 
disc_status_stage2str (int  stage_num)
{
    static disc_status_stage_str_t stage_str[DISC_STAGE_DISCOVERY_MAX+1];
    static u_int8_t init_done = 0;
    
    if (!init_done) { 
        strcpy(stage_str[DISC_STAGE_DISCOVERY_BEGIN].str, 
                "Progress.");
        strcpy(stage_str[DISC_STAGE_DISCOVERY_SUCCESS].str, 
                "Success.");
        strcpy(stage_str[DISC_STAGE_DISCOVERY_FAILURE].str, 
                "Failure.");
        strcpy(stage_str[DISC_STAGE_DISCOVERY_MAX].str, "Unknown Error code");
        init_done = 1;
    }

    if ( (stage_num >= DISC_STAGE_DISCOVERY_MAX) ||
         (stage_num < 0 ) ) { 
        return stage_str[DISC_STAGE_DISCOVERY_MAX].str;
    }

    return stage_str[stage_num].str;
}

static inline char* 
disc_exit_status2str (int  state_num)
{
    static disc_exit_status_str_t state_str[DISC_EXIT_STATUS_MAX+1];
    static u_int8_t init_done = 0;
    
    if (!init_done) { 
        strcpy(state_str[DISC_EXIT_STATUS_SUCCESS].str, 
                "Success");
        strcpy(state_str[DISC_EXIT_STATUS_NOT_ACTIVATE].str, 
                "Discover not activated");
        strcpy(state_str[DISC_EXIT_STATUS_LOST_CONNECTION].str, 
                "Lost connection");
        strcpy(state_str[DISC_EXIT_STATUS_BAD_CREDENTIAL].str, 
                "Bad credentials");
        strcpy(state_str[DISC_EXIT_STATUS_UNSUPPORTED_VERSION].str, 
                "Unsupported vCenter version (check logs)");
        strcpy(state_str[DISC_EXIT_STATUS_NO_VCENTER_SERVICE].str, 
                "No vCenter Service at URL");
		strcpy(state_str[DISC_EXIT_STATUS_INVALID].str,"delete-all response from vCenter");
        strcpy(state_str[DISC_EXIT_STATUS_MAX].str, "Script Failure - Unknown");
        init_done = 1;
    }

    if ( (state_num >= DISC_EXIT_STATUS_MAX) ||
         (state_num < 0 ) ) { 
        return state_str[DISC_EXIT_STATUS_MAX].str;
    }

    return state_str[state_num].str;
}

#endif  //DISCOVERDCMDEF_H

