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

/************************************8***************************************
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.                *
 *   All rights reserved.                                                   *
 *   Author : Mobin Mohan                                                   *
 *   File: configmgmt_dcm_def.h                                             *
 *   Description:                                                           *
 *                                                                          *
 *      The common C structure definition for back-end to interface with    *
 *       DCMd                                                               *
 ***************************************************************************/

#ifndef CONFIGMGMTDCMDEF_H
#define CONFIGMGMTDCMDEF_H


#include <stdio.h>
#include <unistd.h>

#define PACKED                   __attribute__((packed)) 


/* Tag Names */
typedef enum config_mgmt_show_buf_ { 
    CONFIGMGMT_SHOW_BUF_UNKNOWN = 0,
    CONFIGMGMT_SHOW_BUF_SYSTEM 
} config_mgmt_show_buf_t;


/* show config mgmt cmd code */
typedef enum config_mgmt_cmd_code_ {
    CONFIGMGMT_CMD_UNKNOWN = 0,
    CONFIGMGMT_CMD_SHOW_SYSTEM
} config_mgmt_cmd_code_t;

typedef struct config_mgmt_req_ { 
    config_mgmt_cmd_code_t  cmd_code;
} config_mgmt_req_t;

typedef union config_mgmt_buffer_num_ { 
    struct { 
        u_int16_t   mapped_id;
        //This is typically lldp_mgmt_show_buf_t for show commands
        u_int16_t   tag;     
    } buff;
    u_int32_t   num;   
} PACKED config_mgmt_buffer_num_t;

static inline u_int16_t 
config_get_buffer_tag (u_int32_t  buff_num) 
{
    return (((config_mgmt_buffer_num_t*) &buff_num)->buff.tag);
}


static inline u_int16_t 
config_get_buffer_mapped_id (u_int32_t  buff_num) 
{
    return (((config_mgmt_buffer_num_t*) &buff_num)->buff.mapped_id);
}

typedef struct config_mgmt_show_system_resp_ {
	u_int32_t				teport;
	u_int32_t				fcport;
	u_int32_t				giport;
	u_int32_t				foport;
} PACKED config_mgmt_show_system_resp_t;

#endif  //CONFIGMGMTDCMDEF_H

