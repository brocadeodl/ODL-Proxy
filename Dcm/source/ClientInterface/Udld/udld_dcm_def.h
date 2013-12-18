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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.                *
 *   All rights reserved.                                                   *
 *   Author : Eugene Gee                                                    *
 *   File: udld_dcm_def.h                                                   *
 *   Description:                                                           *
 *                                                                          *
 *      The common C structure definition for back-end to interface with    *
 *       DCMd                                                               *
 ***************************************************************************/

#ifndef UDLDDCMDEF_H
#define UDLDDCMDEF_H


#include <stdio.h>
#include <unistd.h>
#include "ClientInterface/InterfaceCommon.h"

#define PACKED                   __attribute__((packed)) 

#define TRUE_NTOH                1
#define FALSE_NTOH               0

#define UDLD_IF_STR_MAX_LEN             64
#define UDLD_NAME_MAX_LEN               50
#define UDLD_MGMT_SMALL_BUF             128
#define UDLD_MGMT_ERR_STR_MAX_LEN       256
#define UDLD_MGMT_COMPOSITE_MAX_SIZE    50
#define UDLD_MGMT_ETHER_ADDR_LEN        6


#define UDLD_CLI_TKN_MAX_LEN    128
#define UDLD_CLI_TKN_INTF   "interface"
#define UDLD_CLI_TKN_STATS "statistics"

#define UDLD_DCM_CLIENT_NAME    "UdldDCMClient"

#define CHECK_FLAG(V,F)      ((V) & (F))

#define UDLD_NO_PROTOCOL                          (1 << 0)  /* default or "no protocol udld" executed in config mode */
#define UDLD_GLOBAL_DISABLED                      (1 << 1)  /* "no enable" executed in protocol udld mode */
#define UDLD_INTERFACE_DISABLED                   (1 << 2)  /* default or "no udld enable" executed in interface mode */
#define UDLD_DISABLED  (UDLD_NO_PROTOCOL | UDLD_GLOBAL_DISABLED | UDLD_INTERFACE_DISABLED) /* UDLD disabled on interface */

#define UDLD_UNKNOWN_PORT_ID   0xffff
#define UDLD_UNKNOWN_PORT_ID_STR   "Unknown"
#define UDLD_UNKNOWN_LINK_ID   0xffff
#define UDLD_UNKNOWN_SYS_ID    0xffffffff
#define UDLD_UNKNOWN_SEQ_NUM   0xffffffff


#define COMPARENLEN(A,B)  strncmp(A, B, strlen(A))

/* Tag Names */
typedef enum udld_mgmt_show_buf_ { 
    UDLD_MGMT_SHOW_BUF_UNKNOWN = 0,
    UDLD_MGMT_SHOW_BUF_GBL, 
    UDLD_MGMT_SHOW_BUF_INTF, 
    UDLD_MGMT_SHOW_BUF_STATS,
    UDLD_MGMT_SHOW_BUF_STATS_INTF
} udld_mgmt_show_buf_t;

/* Tag Names */
typedef enum udld_mgmt_clear_buf_ {
    UDLD_MGMT_CLEAR_BUF_UNKNOWN = 0,
    UDLD_MGMT_CLEAR_BUF_STATS,
    UDLD_MGMT_CLEAR_BUF_STATS_INTF
} udld_mgmt_clear_buf_t;


/* Executive mode command code */
typedef enum {
    UDLD_MGMT_CMD_UNKNOWN = 0,
    UDLD_MGMT_CMD_SHOW_GBL,

    UDLD_MGMT_CMD_SHOW_INTF_ALL,
    UDLD_MGMT_CMD_SHOW_INTF,
    UDLD_MGMT_CMD_SHOW_INTF_GI,
    UDLD_MGMT_CMD_SHOW_INTF_FO,

    UDLD_MGMT_CMD_SHOW_STATS,
    UDLD_MGMT_CMD_SHOW_STATS_INTF,
    UDLD_MGMT_CMD_SHOW_STATS_INTF_GI,
    UDLD_MGMT_CMD_SHOW_STATS_INTF_FO,

    UDLD_MGMT_CMD_CLEAR_STATS,
    UDLD_MGMT_CMD_CLEAR_STATS_INTF,
    UDLD_MGMT_CMD_CLEAR_STATS_INTF_GI,
    UDLD_MGMT_CMD_CLEAR_STATS_INTF_FO
} udld_mgmt_cmd_code_t;

typedef enum { 
    UDLD_MGMT_SUCCESS                     = 0,
    UDLD_MGMT_ERR_FAILURE                 = 1,
    UDLD_MGMT_ERR_UDLD_NOT_EXIST          = 2,
    UDLD_MGMT_ERR_MASTER_NOT_EXIST        = 3,
    UDLD_MGMT_ERR_NO_SUCH_INTF            = 4,
    UDLD_MGMT_ERR_CMD_ALLOWED_ON_PHY_INTF = 5,
    UDLD_MGMT_ERR_UDLD_INTF_NOT_ACTIVE    = 6,
    UDLD_MGMT_ERR_INTERFACE_NOT_ETHERNET  = 7,
    UDLD_MGMT_ERR_MAX
} udld_mgmt_err_code_t;

typedef struct udld_mgmt_err_str_ { 
    char    str[UDLD_MGMT_ERR_STR_MAX_LEN + 1];
} udld_mgmt_err_str_t;

static inline char* 
udld_mgmt_err2str (int  err_num)
{
    static udld_mgmt_err_str_t err_str[UDLD_MGMT_ERR_MAX + 1];
    static u_int8_t init_done = 0;
    
    if (!init_done) { 
        strncpy(err_str[UDLD_MGMT_SUCCESS].str,"Success.",UDLD_MGMT_ERR_STR_MAX_LEN);
        err_str[UDLD_MGMT_SUCCESS].str[UDLD_MGMT_ERR_STR_MAX_LEN-1] = '\0';
        strncpy(err_str[UDLD_MGMT_ERR_FAILURE].str,"Failure.",UDLD_MGMT_ERR_STR_MAX_LEN);
        err_str[UDLD_MGMT_ERR_FAILURE].str[UDLD_MGMT_ERR_STR_MAX_LEN-1] = '\0';
        strncpy(err_str[UDLD_MGMT_ERR_UDLD_NOT_EXIST].str,"UDLD not active on interface.",UDLD_MGMT_ERR_STR_MAX_LEN);
        err_str[UDLD_MGMT_ERR_UDLD_NOT_EXIST].str[UDLD_MGMT_ERR_STR_MAX_LEN-1] = '\0';
        strncpy(err_str[UDLD_MGMT_ERR_MASTER_NOT_EXIST].str,"The UDLD master does not exist.",UDLD_MGMT_ERR_STR_MAX_LEN);
        err_str[UDLD_MGMT_ERR_MASTER_NOT_EXIST].str[UDLD_MGMT_ERR_STR_MAX_LEN-1] = '\0';
        strncpy(err_str[UDLD_MGMT_ERR_NO_SUCH_INTF].str,"No such interface.",UDLD_MGMT_ERR_STR_MAX_LEN);
        err_str[UDLD_MGMT_ERR_NO_SUCH_INTF].str[UDLD_MGMT_ERR_STR_MAX_LEN-1]  ='\0';
        strncpy(err_str[UDLD_MGMT_ERR_UDLD_INTF_NOT_ACTIVE].str,"UDLD interface Not Active.",UDLD_MGMT_ERR_STR_MAX_LEN);
        err_str[UDLD_MGMT_ERR_UDLD_INTF_NOT_ACTIVE].str[UDLD_MGMT_ERR_STR_MAX_LEN-1]  = '\0';
        strncpy(err_str[UDLD_MGMT_ERR_INTERFACE_NOT_ETHERNET].str,"Command allowed only on Physical Interface.",UDLD_MGMT_ERR_STR_MAX_LEN);
        err_str[UDLD_MGMT_ERR_INTERFACE_NOT_ETHERNET].str[UDLD_MGMT_ERR_STR_MAX_LEN-1]  = '\0';
        strncpy(err_str[UDLD_MGMT_ERR_MAX].str, "Unknown Error code",UDLD_MGMT_ERR_STR_MAX_LEN);
        err_str[UDLD_MGMT_ERR_MAX].str[UDLD_MGMT_ERR_STR_MAX_LEN-1] = '\0';
        init_done = 1;
    }

    if ( (err_num >= UDLD_MGMT_ERR_MAX) ||
         (err_num < 0 ) ) { 
        return err_str[UDLD_MGMT_ERR_MAX].str;
    }

    return err_str[err_num].str;
}

typedef struct udld_mgmt_int_req_ { 
    char    if_name[UDLD_IF_STR_MAX_LEN + 1];
    char    interface[UDLD_IF_STR_MAX_LEN + 1];
} udld_mgmt_int_req_t, udld_mgmt_stats_int_req_t, udld_debug_int_req_t;

typedef struct udld_mgmt_req_ { 
    udld_mgmt_cmd_code_t  cmd_code;

    union { 
        udld_mgmt_int_req_t         intf;
        udld_mgmt_stats_int_req_t   stats_intf;
    } entry;
} udld_mgmt_req_t;


#define UDLD_MGMT_HELLO_CONFIGURED                       (1 << 7)
#define UDLD_MGMT_MULTIPLIER_CONFIGURED                  (1 << 9)

#define UDLD_MGMT_CONF_DISABLED        (1 << 0)
#define UDLD_MGMT_NO_PROTOCOL          (1 << 5)
#define UDLD_MGMT_DISABLED  (UDLD_MGMT_NO_PROTOCOL | UDLD_MGMT_CONF_DISABLED)


#define UDLD_MGMT_INTERFACE_DISABLED                   (1 << 4)
#define UDLD_MGMT_MULTIPLIER_DEFAULT                   5
#define UDLD_MGMT_HELLO_DEFAULT                        5

#define UDLD_DEFAULT_HELLO                             UDLD_MGMT_HELLO_DEFAULT
#define UDLD_DEFAULT_MULTIPLIER                        UDLD_MGMT_MULTIPLIER_DEFAULT
#define UDLD_DEFAULT_DISABLE                           true

typedef struct  udld_mgmt_master_ { 
    u_int16_t  admin_status;
    u_int16_t  resvd1;
    u_int32_t  timeout_multiplier;
    u_int32_t  hello_time;

    u_int8_t   system_cfg;
    u_int8_t   resvd2[3];
} PACKED udld_mgmt_master_t;



typedef struct udld_mgmt_gbl_resp_ {
    udld_mgmt_err_code_t  ret_code;
    udld_mgmt_master_t entry;
} PACKED udld_mgmt_gbl_resp_t;

typedef struct udld_mgmt_show_intf_ { 
    char        if_name[UDLD_IF_STR_MAX_LEN + 4];
    u_int16_t   globalAdminStatus;
    u_int16_t   interfaceAdminStatus;
    u_int16_t   interfaceOperationalStatus;
    u_int16_t   remoteHelloStatus;
    u_int32_t   localSystemId;
    u_int32_t   remoteSystemId;
    char		localPortId[UDLD_IF_STR_MAX_LEN + 4];
    char		remotePortId[UDLD_IF_STR_MAX_LEN + 4];
    u_int16_t   localLinkId;
    u_int16_t   remoteLinkId;
    u_int32_t   lastTxSeqNumber;
    u_int32_t   lastRxSeqNumber;
} PACKED udld_mgmt_show_intf_t;

typedef struct udld_mgmt_show_intf_resp_ {
    udld_mgmt_err_code_t    ret_code;
    udld_mgmt_show_intf_t   entry;
} PACKED udld_mgmt_show_intf_resp_t;

typedef struct udld_mgmt_show_all_intf_resp_ {
	udld_mgmt_err_code_t			ret_code;
	u_int32_t 						num_records;
	udld_mgmt_show_intf_resp_t		entry[1];
} PACKED udld_mgmt_show_all_intf_resp_t;


typedef struct udld_mgmt_clear_resp_ {
    udld_mgmt_err_code_t    ret_code;
} PACKED udld_mgmt_clear_resp_t;

typedef struct udld_mgmt_show_stats_intf_ { 
    char       if_name[UDLD_IF_STR_MAX_LEN + 4];
    u_int32_t  numberOfTxFrames;
    u_int32_t  numberOfRxFrames;
    u_int32_t  numberOfDiscardedFrames;
    u_int32_t  numberOfErrorFrames;
    u_int32_t  numberOfRportIdsChanged;
    u_int32_t  numberOfRMacAddressChanged;
} PACKED udld_mgmt_show_stats_intf_t;

typedef struct udld_mgmt_show_stats_resp_ {
    udld_mgmt_err_code_t    ret_code;
    u_int32_t               num_records;
    udld_mgmt_show_stats_intf_t  stats[1];
} PACKED udld_mgmt_show_stats_resp_t;

typedef struct udld_mgmt_composite_data_
{
    u_int8_t type;
    u_int8_t size;
    char     value[UDLD_MGMT_COMPOSITE_MAX_SIZE];
} PACKED udld_mgmt_composite_data_t;


#define UDLD_PLUGIN_LOCAL_VERSION   0
#define UDLD_PLUGIN_GLOBAL_VERSION  0

#define IS_CMDCODE_SET(cmdcode, flag) (cmdcode & flag)

/* UDLD Global Command Codes */
#define        UDLD_GLOBAL_CONF_HELLO          1 << 0
#define        UDLD_GLOBAL_CONF_MULTIPLIER     1 << 1
#define        UDLD_GLOBAL_CONF_DISABLE        1 << 2
#define        UDLD_GLOBAL_CONF_UDLD           1 << 3
#define        UDLD_GLOBAL_CONF_NO_UDLD        1 << 4
#define        UDLD_GLOBAL_CONF                (UDLD_GLOBAL_CONF_HELLO | UDLD_GLOBAL_CONF_MULTIPLIER)

// Intf Level cmds
#define		   UDLD_INTF_CONF_DISABLE							 1<<0
#define        UDLD_INTF_CONF_DESCRIPTION                        1<<4
// Internel cmds
#define		   UDLD_NSM_INTERNAL_NO_UDLD						 1<<1

/* UDLD Message Types */
typedef enum
    {
        UDLD_CREATE_CONF,
        UDLD_UPDATE_CONF,
        UDLD_DELETE_CONF
    } UdldMsgType;

typedef struct udld_global_conf_ {
        unsigned int mgmt_flags;
        unsigned int cmd_code;
        unsigned int msg_type;
        unsigned int hello;
        unsigned int multiplier;
        unsigned int disable;
} udld_mgmt_global_conf_t;

typedef struct udld_intf_conf_ {
        unsigned int mgmt_flags;
        unsigned int cmd_code;
        unsigned int msg_type;
        char id[64];
        char if_name[64];
        unsigned int if_index;
        u_int16_t disable;
        char description[64];
        unsigned long if_type;
}udld_mgmt_intf_conf_t;

typedef struct udld_mgmt_conf_req_ {
    u_int32_t mgmt_flags;
    u_int32_t cmd_code;
    u_int32_t msg_type;

    union {
        udld_mgmt_global_conf_t     global_conf;
        udld_mgmt_intf_conf_t       intf_conf;
    } entry;
} udld_mgmt_conf_req_t;

typedef union udld_mgmt_buffer_num_ { 
    struct { 
        u_int16_t   mapped_id;
        //This is typically udld_mgmt_show_buf_t for show commands
        u_int16_t   tag;     
    } buff;
    u_int32_t   num;   
} PACKED udld_mgmt_buffer_num_t;

static inline u_int16_t 
udld_get_buffer_tag (u_int32_t  buff_num) 
{
    return (((udld_mgmt_buffer_num_t*) &buff_num)->buff.tag);
}


static inline u_int16_t 
udld_get_buffer_mapped_id (u_int32_t  buff_num) 
{
    return (((udld_mgmt_buffer_num_t*) &buff_num)->buff.mapped_id);
}

#define UDLD_DEBUG_CLI_TKN_STR_LEN_MAX        32

#define UDLD_DEBUG_CLI_TKN_STR_DEBUG          "debug"
#define UDLD_DEBUG_CLI_TKN_STR_UDLD           "udld"
#define UDLD_DEBUG_CLI_TKN_STR_DUMP           "dump"
#define UDLD_DEBUG_CLI_TKN_STR_PACKET         "packet"
#define UDLD_DEBUG_CLI_TKN_STR_ALL            "all"
#define UDLD_DEBUG_CLI_TKN_STR_DETAIL         "detail"
#define UDLD_DEBUG_CLI_TKN_STR_BOTH           "both"
#define UDLD_DEBUG_CLI_TKN_STR_TX             "tx"
#define UDLD_DEBUG_CLI_TKN_STR_RX             "rx"
#define UDLD_DEBUG_CLI_TKN_STR_NO             "no"
#define UDLD_DEBUG_CLI_TKN_STR_TE_NAME        "tengigabitethernet"
#define UDLD_DEBUG_CLI_TKN_STR_GI_NAME        "gigabitethernet"
#define UDLD_DEBUG_CLI_TKN_STR_FO_NAME        "fortygigabitethernet"
#define UDLD_DEBUG_CLI_TKN_INTF_DELIM         "/"

#define UDLD_DEBUG_REQ_FLAG_NONE		   (0 << 1)
#define UDLD_DEBUG_REQ_FLAG_RX             (1 << 0)
#define UDLD_DEBUG_REQ_FLAG_TX             (1 << 1)
#define UDLD_DEBUG_REQ_FLAG_TXRX		   (UDLD_DEBUG_REQ_FLAG_RX|UDLD_DEBUG_REQ_FLAG_TX)
#define UDLD_DEBUG_REQ_FLAG_DUMP		   (1 << 2)
#define UDLD_DEBUG_REQ_FLAG_DETAIL         (1 << 3)

typedef enum {
    UDLD_DEBUG_CMD_CODE_INVALID = 0,
    UDLD_DEBUG_CMD_CODE_DUMP_ALL,
    UDLD_DEBUG_CMD_CODE_DUMP_INTF,
    UDLD_DEBUG_CMD_CODE_DUMP_INTF_GI,
    UDLD_DEBUG_CMD_CODE_PACKET_ALL,
    UDLD_DEBUG_CMD_CODE_PACKET_INTF,
    UDLD_DEBUG_CMD_CODE_PACKET_INTF_GI,
    UDLD_DEBUG_CMD_CODE_SHOW_STATUS,
    UDLD_DEBUG_CMD_CODE_DUMP_INTF_FO,
    UDLD_DEBUG_CMD_CODE_PACKET_INTF_FO,
    UDLD_NO_DEBUG_CMD_CODE_PACKET_ALL,
    UDLD_NO_DEBUG_CMD_CODE_PACKET_INTF
} udld_debug_cmd_t;

typedef enum udld_debug_buf_ { 
    UDLD_DEBUG_BUF_UNKNOWN = 0,
    UDLD_DEBUG_BUF_DUMP_ALL, 
    UDLD_DEBUG_BUF_DUMP_INTF,
    UDLD_DEBUG_BUF_PKT,
    UDLD_DEBUG_BUF_SHOW_STATUS
} udld_debug_buf_t;

typedef struct udld_debug_cmd_req_ {
    udld_debug_cmd_t cmd_code;
    udld_debug_int_req_t intf;
    u_int32_t   debug_flags;
} udld_debug_cmd_req_t;

typedef struct udld_debug_dump_resp_entry_ {
    u_int32_t   timeout_multiplier;
    u_int32_t   hello_time;
    u_int16_t   admin_status;
    u_int8_t    udld_if_mac_addr[UDLD_MGMT_ETHER_ADDR_LEN];
    char        if_name[UDLD_IF_STR_MAX_LEN + 1];
} udld_debug_dump_resp_entry_t;

typedef struct udld_debug_cmd_resp_ {
    udld_mgmt_err_code_t    ret_code;
    u_int32_t               num_records;
    udld_debug_dump_resp_entry_t dump_entry[1];
} udld_debug_cmd_resp_t;

typedef struct udld_debug_status_resp_entry_ {
    char        if_name[UDLD_IF_STR_MAX_LEN + 1];
    u_int32_t   debug_flags;
} udld_debug_status_resp_entry_t;

typedef struct udld_debug_status_resp_ {
    udld_mgmt_err_code_t    ret_code;
    u_int32_t               num_records;
    udld_debug_status_resp_entry_t status_entry[1];
} udld_debug_status_resp_t;


#endif  //UDLDDCMDEF_H

