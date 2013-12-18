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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#ifndef FCOE_PLUGIN_TYPES_H
#define FCOE_PLUGIN_TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif    

#include "stdbool.h"
#include "stdint.h"

#define FCOE_ENUM_BIT_POS(num)  (1<<num)

#define FCOE_DCM_CLIENT_NAME "fcoed"
#define NSM_DCM_CLIENT_NAME "nsm"

#define FCOE_PLUGIN_LOCAL_VERSION   0
#define FCOE_PLUGIN_GLOBAL_VERSION  0

#define FCOE_FAB_MAP_DEFAULT_FCMAP_HEX  0x0efc00
#define FCOE_FAB_MAP_DEFAULT_FCMAP      "0E:FC:00"
#define FCOE_FAB_MAP_DEFAULT_VLAN       1002
#define FCOE_FAB_MAP_DEFAULT_VFID       128
#define FCOE_FAB_MAP_DEFAULT_PRIORITY   3
#define FCOE_FAB_MAP_DEFAULT_MTU        2112
#define FCOE_FAB_MAP_DEFAULT_MAX_ENODES 256
#define FCOE_DEFAULT_FCOE_ENODES_WITH_LICENSE 64
#define FCOE_DEFAULT_FCOE_ENODES_WITH_OUT_LICENSE 0

#define FCOE_ENODES_CFG_MODE_LOCAL           "local"
#define FCOE_ENODES_CFG_MODE_GLOBAL          "global"
#define FCOE_FAB_MAP_DEFAULT_ENODES_CFG_MODE FCOE_ENODES_CFG_MODE_LOCAL

#define FCOE_DEFAULT_FKA_INTVL              8000
#define FCOE_DEFAULT_FKA_TIMEOUT            true
#define FCOE_DEFAULT_FKA_TIMEOUT_DISABLED   false

#define FCOE_FAB_MAP_DEFAULT_FKA_INTVL      FCOE_DEFAULT_FKA_INTVL
#define FCOE_FAB_MAP_DEFAULT_TIMEOUT        FCOE_DEFAULT_FKA_TIMEOUT
#define FCOE_FAB_MAP_TIMEOUT_DISABLED       FCOE_DEFAULT_FKA_TIMEOUT_DISABLED

#define FCOE_FIP_DEFAULT_FKA_INTVL  FCOE_DEFAULT_FKA_INTVL
#define FCOE_FIP_DEFAULT_TIMEOUT    FCOE_DEFAULT_FKA_TIMEOUT
#define FCOE_FIP_TIMEOUT_DISABLED   FCOE_FAB_MAP_TIMEOUT_DISABLED

#define FCOE_INTF_STATUS_NO_SHUTDOWN      false
#define FCOE_INTF_STATUS_SHUTDOWN         true
#define FCOE_INTF_STATUS_DEFAULT    FCOE_INTF_STATUS_NO_SHUTDOWN

#define FCOE_ET_INTF_STATUS_ENABLED         true
#define FCOE_ET_INTF_STATUS_DISABLED        false
#define FCOE_ET_INTF_STATUS_DEFAULT         FCOE_ET_INTF_STATUS_ENABLED
#define FCOE_ET_INTF_DMN_ISL_STATUS_ENABLED     1
#define FCOE_ET_INTF_DMN_ISL_STATUS_DISABLED    0
#define FCOE_ET_INTF_DMN_ISL_STATUS_DEFAULT     FCOE_ET_INTF_DMN_ISL_STATUS_ENABLED

#define FCOE_ET_INTF_TRUNK_STATUS_ENABLED   true
#define FCOE_ET_INTF_TRUNK_STATUS_DISABLED  false
#define FCOE_ET_INTF_TRUNK_STATUS_DEFAULT   FCOE_ET_INTF_TRUNK_STATUS_ENABLED
#define FCOE_ET_INTF_DMN_TRUNK_STATUS_ENABLED   1
#define FCOE_ET_INTF_DMN_TRUNK_STATUS_DISABLED  0
#define FCOE_ET_INTF_DMN_TRUNK_STATUS_DEFAULT   FCOE_ET_INTF_DMN_TRUNK_STATUS_ENABLED

#define FCOE_INTF_STATUS_ENABLED          true
#define FCOE_INTF_STATUS_DISABLED         false

#define FCOE_INTF_DMN_STATUS_ENABLED       1
#define FCOE_INTF_DMN_STATUS_DISABLED      0
#define FCOE_INTF_DMN_STATUS_DEFAULT       FCOE_INTF_DMN_STATUS_ENABLED

#define FCOE_INTF_DEFAULT_MODE              "VF"
#define FCOE_INTF_DEFAULT_BINDING           ""
#define FCOE_INTF_UPORT_NUM_INVALID         65535
#define FCOE_INTF_IFINDEX_INVALID           0
#define FCOE_INTF_VN_NUM_INVALID            0
#define FCOE_INTF_DEFAULT_VN_NUM            1
#define FCOE_INTF_PORT_NUM_INVALID          65535
#define FCOE_INTF_DEFAULT_NAME              ""
#define FCOE_INTF_DEFAULT_DESCRIPTION       ""
#define FCOE_INTF_FC_UPORT_SLOT_NUM         256

#define FCOE_CLI_TOKEN_STR_FAB_MAP              "fabric-map"
#define FCOE_CLI_TOKEN_STR_MAP                  "map"
#define FCOE_CLI_TOKEN_STR_FIP                  "fip"
#define FCOE_CLI_TOKEN_STR_INTERFACE            "interface"
#define FCOE_CLI_TOKEN_STR_FCOE                 "fcoe"
#define FCOE_CLI_TOKEN_STR_VLAN                 "vlan"
#define FCOE_CLI_TOKEN_STR_VFID                 "vfid"
#define FCOE_CLI_TOKEN_STR_LOGIN                "login"
#define FCOE_CLI_TOKEN_STR_BRIEF                "brief"
#define FCOE_CLI_TOKEN_INTF_DELIM               "/"
#define FCOE_CLI_TOKEN_STR_TE_NAME              "tengigabitethernet"
#define FCOE_CLI_TOKEN_STR_FO_NAME              "fortygigabitethernet"
#define FCOE_CLI_TOKEN_STR_ENODE_DEVICE         "device"
#define FCOE_CLI_TOKEN_STR_INTERNAL                 "internal"
#define FCOE_CLI_TOKEN_STR_FCF_MAC_ADDRESS      "fcf-mac-address"
#define FCOE_CLI_TOKEN_STR_ETHERNET             "ethernet"
#define FCOE_CLI_TOKEN_STR_RBRIDGEID            "rbridge-id"
#define FCOE_CLI_TOKEN_STR_ALL                  "all"
#define FCOE_CLI_TOKEN_STR_ETH_PORT_TABLE       "eth-port-table"
#define FCOE_CLI_TOKEN_STR_ISL_PORT_TABLE       "isl-port-table"
#define FCOE_CLI_TOKEN_STR_VX_PORT_TABLE        "vx-port-table"
#define FCOE_CLI_TOKEN_STR_BINDING_TABLE        "binding-table"
#define FCOE_CLI_TOKEN_STR_SW_INFO              "sw-info"
#define FCOE_CLI_TOKEN_STR_GLOBAL_INFO          "global-info"
#define FCOE_CLI_TOKEN_STR_DCMD                 "dcmd"
#define FCOE_CLI_TOKEN_STR_PORT_CHANNEL         "port-channel"
#define FCOE_CLI_TOKEN_STR_CEE_STATE            "cee-state"
#define FCOE_CLI_TOKEN_STR_BINDING              "binding"
#define FCOE_CLI_TOKEN_STR_CONV_TO_STATIC       "convert-to-static"
#define FCOE_CLI_TOKEN_STR_MAC_ADDRESS          "mac-address"
#define FCOE_CLI_TOKEN_STR_HA					"ha"
#define FCOE_CLI_TOKEN_STR_STATS				"stats"

#define FCOE_CLI_TOKEN_STR_LEN_MAX          32

#define FCOE_CLI_TOKEN_ID_INVALID               0
#define FCOE_CLI_TOKEN_ID_FAB_MAP               1
#define FCOE_CLI_TOKEN_ID_MAP                   2
#define FCOE_CLI_TOKEN_ID_FIP                   3
#define FCOE_CLI_TOKEN_ID_INTERFACE             4
#define FCOE_CLI_TOKEN_ID_FCOE                  5
#define FCOE_CLI_TOKEN_ID_VLAN                  6
#define FCOE_CLI_TOKEN_ID_VFID                  7
#define FCOE_CLI_TOKEN_ID_LOGIN                 8
#define FCOE_CLI_TOKEN_ID_BRIEF                 9
#define FCOE_CLI_TOKEN_ID_POSSIBLE_INTF_NAME    10
#define FCOE_CLI_TOKEN_ID_TE_NAME               11
#define FCOE_CLI_TOKEN_ID_ENODE_DEVICE          12
#define FCOE_CLI_TOKEN_ID_INTERNAL              13
#define FCOE_CLI_TOKEN_ID_FCF_MAC_ADDRESS       14
#define FCOE_CLI_TOKEN_ID_ETHERNET              15
#define FCOE_CLI_TOKEN_ID_RBRIDGEID             16
#define FCOE_CLI_TOKEN_ID_ALL                   17
#define FCOE_CLI_TOKEN_ID_ETH_PORT_TABLE        18
#define FCOE_CLI_TOKEN_ID_ISL_PORT_TABLE        19
#define FCOE_CLI_TOKEN_ID_VX_PORT_TABLE         20
#define FCOE_CLI_TOKEN_ID_BINDING_TABLE         21
#define FCOE_CLI_TOKEN_ID_SW_INFO               22
#define FCOE_CLI_TOKEN_ID_GLOBAL_INFO           23
#define FCOE_CLI_TOKEN_ID_DCMD                  24
#define FCOE_CLI_TOKEN_ID_PORT_CHANNEL          25
#define FCOE_CLI_TOKEN_ID_CEE_STATE             26
#define FCOE_CLI_TOKEN_ID_BINDING               27
#define FCOE_CLI_TOKEN_ID_CONV_TO_STATIC        28
#define FCOE_CLI_TOKEN_ID_MAC_ADDRESS           29
#define FCOE_CLI_TOKEN_ID_HA					30
#define FCOE_CLI_TOKEN_ID_STATS					31
#define FCOE_CLI_TOKEN_ID_FO_NAME               32
#define FCOE_MO_DEFAULT_NAME            "default"

#define FCOE_WWN_LEN        24
#define FCOE_MAC_ADDR_LEN   20

#define     FCOE_HA_SSIZE        16
#define		FCOE_MAX_KEY_PATHS_PER_INTERFACE 2
#define MAX_NUMBER_OF_FCOE_ENODES_PER_EACH_GO  300  
typedef enum fcoe_interface_bind_val_type_ 
{
    FCOE_INTERFACE_BIND_VAL_TYPE_NONE   = 0,
    FCOE_INTERFACE_BIND_VAL_TYPE_TE     = 1,
    FCOE_INTERFACE_BIND_VAL_TYPE_FO     = 2,
    FCOE_INTERFACE_BIND_VAL_TYPE_PO     = 3,
    FCOE_INTERFACE_BIND_VAL_TYPE_ENODE  = 4
} fcoe_interface_bind_if_t;

typedef enum fcoe_cfg_bind_type_ 
{
    FCOE_CFG_BIND_TYPE_NONE   = 0,
    FCOE_CFG_BIND_TYPE_STATIC = 1,
    FCOE_CFG_BIND_TYPE_DYNAMIC = 2
} fcoe_cfg_bind_type_t;

typedef enum
{
    FCOE_FAB_MAP_OPER_INVALID           = 0,
    // CREATE & REMOVE must be at top.
    FCOE_FAB_MAP_OPER_CREATE            = FCOE_ENUM_BIT_POS(1),
    FCOE_FAB_MAP_OPER_REMOVE            = FCOE_ENUM_BIT_POS(2),
    // VLAN must be next.
    FCOE_FAB_MAP_OPER_UPD_VLAN          = FCOE_ENUM_BIT_POS(3),
    FCOE_FAB_MAP_OPER_DEL_VLAN          = FCOE_ENUM_BIT_POS(4),
    // REST can be in any order.
    FCOE_FAB_MAP_OPER_UPD_FCMAP         = FCOE_ENUM_BIT_POS(5),
    FCOE_FAB_MAP_OPER_UPD_VFID          = FCOE_ENUM_BIT_POS(6),
    FCOE_FAB_MAP_OPER_UPD_PRIORITY      = FCOE_ENUM_BIT_POS(7),
    FCOE_FAB_MAP_OPER_UPD_TIMEOUT       = FCOE_ENUM_BIT_POS(8),
    FCOE_FAB_MAP_OPER_UPD_FKA_INTVL     = FCOE_ENUM_BIT_POS(9),
    FCOE_FAB_MAP_OPER_GET_FCF_MAC_ADDR  = FCOE_ENUM_BIT_POS(10),
    FCOE_FAB_MAP_OPER_UPD_MTU           = FCOE_ENUM_BIT_POS(11),
    FCOE_FAB_MAP_OPER_UPD_MAX_ENODES    = FCOE_ENUM_BIT_POS(12),
    FCOE_FAB_MAP_OPER_UPD_FCOE_ENODES   = FCOE_ENUM_BIT_POS(13),
    FCOE_FAB_MAP_OPER_UPD_ENODES_CFG_MODE   = FCOE_ENUM_BIT_POS(14),
    FCOE_FAB_MAP_OPER_MAX               = FCOE_ENUM_BIT_POS(15)
}FcoeGlobalFabricMapOper;

typedef enum
{
    FCOE_MAP_OPER_INVALID           = 0,
    FCOE_MAP_OPER_ADD_FAB_MAP       = FCOE_ENUM_BIT_POS(1),
    FCOE_MAP_OPER_REM_FAB_MAP       = FCOE_ENUM_BIT_POS(2),
    FCOE_MAP_OPER_ADD_CEE_MAP       = FCOE_ENUM_BIT_POS(3),
    FCOE_MAP_OPER_REM_CEE_MAP       = FCOE_ENUM_BIT_POS(4),
    FCOE_MAP_OPER_CREATE            = FCOE_ENUM_BIT_POS(5),
    FCOE_MAP_OPER_REMOVE            = FCOE_ENUM_BIT_POS(6),
    FCOE_MAP_OPER_MAX               = FCOE_ENUM_BIT_POS(7)
}FcoeGlobalMapOper;

typedef enum
{
    FCOE_FIP_OPER_CREATE            = FCOE_ENUM_BIT_POS(1),
    FCOE_FIP_OPER_REMOVE            = FCOE_ENUM_BIT_POS(2),
    FCOE_FIP_OPER_UPD_TIMEOUT       = FCOE_ENUM_BIT_POS(3),
    FCOE_FIP_OPER_UPD_FKA_INTVL     = FCOE_ENUM_BIT_POS(4),
    FCOE_FIP_OPER_MAX               = FCOE_ENUM_BIT_POS(5)
}FcoeGlobalFipOper;

typedef enum
{
    FCOE_INTF_OPER_CREATE       = FCOE_ENUM_BIT_POS(1),
    FCOE_INTF_OPER_REMOVE       = FCOE_ENUM_BIT_POS(2),
    FCOE_INTF_OPER_UPD_STATUS   = FCOE_ENUM_BIT_POS(3),
    FCOE_INTF_OPER_GET_IF_LIST  = FCOE_ENUM_BIT_POS(4),
    FCOE_INTF_OPER_CONFIG_REPLAY     = FCOE_ENUM_BIT_POS(5),
    FCOE_INTF_OPER_UPD_BIND     = FCOE_ENUM_BIT_POS(6),
    FCOE_INTF_OPER_REM_BIND     = FCOE_ENUM_BIT_POS(7),
    FCOE_INTF_OPER_CREATE_BULK  = FCOE_ENUM_BIT_POS(8),
    FCOE_INTF_OPER_DELETE_BULK  = FCOE_ENUM_BIT_POS(9),
    FCOE_INTF_OPER_MAX          = FCOE_ENUM_BIT_POS(10),
    FCOE_INTF_OPER_GET_IF_BIND_LIST  = FCOE_ENUM_BIT_POS(11)
}FcoeLocalIntfOper;

typedef enum
{
    FCOE_ETH_INTF_OPER_FCOEPORT_ADD = FCOE_ENUM_BIT_POS(1),
    FCOE_ETH_INTF_OPER_FCOEPORT_DEL = FCOE_ENUM_BIT_POS(2),
    FCOE_ETH_INTF_OPER_FCOEPORT_LAG_ADD = FCOE_ENUM_BIT_POS(3),
    FCOE_ETH_INTF_OPER_FCOEPORT_LAG_DEL = FCOE_ENUM_BIT_POS(4),
    FCOE_ETH_INTF_OPER_LAG_ADD_MBR  = FCOE_ENUM_BIT_POS(5),
    FCOE_ETH_INTF_OPER_LAG_DEL_MBR  = FCOE_ENUM_BIT_POS(6),
    FCOE_ETH_INTF_OPER_MAX          = FCOE_ENUM_BIT_POS(7)
}FcoeEthIntfOper;

typedef enum
{
    FCOE_ET_INTF_OPER_CREATE            = FCOE_ENUM_BIT_POS(1),
    FCOE_ET_INTF_OPER_REMOVE            = FCOE_ENUM_BIT_POS(2),
    FCOE_ET_INTF_OPER_UPD_ISL_STATUS    = FCOE_ENUM_BIT_POS(3),
    FCOE_ET_INTF_OPER_GET_IF_LIST       = FCOE_ENUM_BIT_POS(4),
    FCOE_ET_INTF_OPER_UPD_TRUNK_STATUS  = FCOE_ENUM_BIT_POS(5),
    FCOE_ET_INTF_OPER_CONFIG_REPLAY          = FCOE_ENUM_BIT_POS(6),
    FCOE_ET_INTF_OPER_CREATE_BULK       = FCOE_ENUM_BIT_POS(7),
    FCOE_ET_INTF_OPER_SLOT_FAILOVER     = FCOE_ENUM_BIT_POS(8),
    FCOE_ET_INTF_OPER_DEL_UPORT_BULK    = FCOE_ENUM_BIT_POS(9),
    FCOE_ET_INTF_OPER_MAX               = FCOE_ENUM_BIT_POS(10)
}FcoeLocalETIntfOper;

typedef enum
{
    FCOEPORT_ATTR_CREATE  = FCOE_ENUM_BIT_POS(1),
    FCOEPORT_ATTR_REMOVE  = FCOE_ENUM_BIT_POS(2),
    FCOEPORT_ATTR_LAG_CREATE = FCOE_ENUM_BIT_POS(3),
    FCOEPORT_ATTR_LAG_REMOVE = FCOE_ENUM_BIT_POS(4),
    FCOEPORT_ATTR_OPER_MAX = FCOE_ENUM_BIT_POS(5)
}FcoeportAttrOper;

typedef enum
{
    FCOE_OPER_FSB_ENABLE   = FCOE_ENUM_BIT_POS(1),
    FCOE_OPER_FSB_DISABLE  = FCOE_ENUM_BIT_POS(2)
}FcoeFsbOper;

typedef enum
{
    FCOE_ENODE_OPER_UPD_OPER_INVALID = 0,
    FCOE_ENODE_OPER_UPD_TO_GLOBAL = FCOE_FAB_MAP_OPER_UPD_FCOE_ENODES,
    FCOE_ENODE_OPER_UPD_FCOE_ENODES = FCOE_FAB_MAP_OPER_UPD_MAX_ENODES 
}FcoeEnodeOper;



typedef enum
{
    FCOE_SHOW_CMD_CODE_INVALID          = 0,
    FCOE_SHOW_CMD_CODE_FAB_MAP          = 1,
    FCOE_SHOW_CMD_CODE_MAP              = 2,
    FCOE_SHOW_CMD_CODE_FIP              = 3,
    FCOE_SHOW_CMD_CODE_LOGIN            = 4,
    FCOE_SHOW_CMD_CODE_LOGIN_INTF       = 5,
    FCOE_SHOW_CMD_CODE_LOGIN_VLAN       = 6,
    FCOE_SHOW_CMD_CODE_LOGIN_VFID       = 7,
    FCOE_SHOW_CMD_CODE_INTF             = 8,
    FCOE_SHOW_CMD_CODE_INTF_BRIEF       = 9,
    FCOE_SHOW_CMD_CODE_PHY_INTF_FCOE    = 10,
    FCOE_SHOW_CMD_CODE_INTERNAL_FCF_MAC_ADDRESS = 11,
    FCOE_SHOW_CMD_CODE_ETHERNET_INTF    = 12,
    FCOE_ACTION_CMD_CODE_CONV_TO_STATIC_MAC_ADDR_ALL = 13
}FcoeActionCmdCode;

typedef enum
{
    FCOE_CLEAR_LOGIN_CMD_CODE_INVALID         = 0,
    FCOE_CLEAR_LOGIN_CMD_CODE_FCOE_INTF       = 1,
    FCOE_CLEAR_LOGIN_CMD_CODE_TE_INTF         = 2,
    FCOE_CLEAR_LOGIN_CMD_CODE_VLAN            = 3,
    FCOE_CLEAR_LOGIN_CMD_CODE_VFID            = 4,
    FCOE_CLEAR_LOGIN_CMD_CODE_DEVICE          = 5,
    FCOE_CLEAR_COUNTERS_CMD_CODE_ALL          = 6,
    FCOE_CLEAR_COUNTERS_CMD_CODE_UPORT        = 7,
    FCOE_CLEAR_LOGIN_CMD_CODE_ALL             = 8,
    FCOE_CLEAR_COUNTERS_CMD_CODE_SLOT         = 9,
    FCOE_CLEAR_LOGIN_CMD_CODE_PO              = 10,
    FCOE_CLEAR_BINDING_MAC_ADDR_ALL           = 11
}FcoeClearCmdCode;

typedef enum
{
    FCOE_DEBUG_CMD_CODE_INVALID         = 0,
    FCOE_DEBUG_CMD_CODE_ENABLE_LOGFILE  = 1,
    FCOE_DEBUG_CMD_CODE_DISABLE_LOGFILE = 2,
    FCOE_DEBUG_CMD_CODE_TEST_HA         = 3,
	FCOE_DEBUG_CMD_CODE_HA_LOGGING_ENABLE  = 4,
	FCOE_DEBUG_CMD_CODE_HA_LOGGING_DISABLE = 5
}FcoeDebugCmdCode;

typedef enum
{
    FCOE_DEBUG_HA_SEND_INTENT     = 0,
    FCOE_DEBUG_HA_SEND_UPDATE     = 1
}FcoeDebugHaSendType;

typedef enum
{
    FCOE_DEBUG_HA_SERVICE_CHASSIS    = 0,
    FCOE_DEBUG_HA_SERVICE_FCSW       = 1,
    FCOE_DEBUG_HA_SERVICE_ETHSW      = 2
}FcoeDEbugHaServiceType;

typedef enum
{
    FCOE_INTERNAL_CMD_CODE_INVALID                  = 0,
    FCOE_INTERNAL_CMD_CODE_ETH_PORT_TABLE_TE_INTF   = 1,
    FCOE_INTERNAL_CMD_CODE_ETH_PORT_TABLE_ALL       = 2,
    FCOE_INTERNAL_CMD_CODE_ETH_PORT_TABLE_RBRIDGEID = 3,
    FCOE_INTERNAL_CMD_CODE_ISL_PORT_TABLE_TE_INTF   = 4,
    FCOE_INTERNAL_CMD_CODE_ISL_PORT_TABLE_ALL       = 5,
    FCOE_INTERNAL_CMD_CODE_ISL_PORT_TABLE_RBRIDGEID = 6,
    FCOE_INTERNAL_CMD_CODE_VX_PORT_TABLE_FCOE_INTF  = 7,
    FCOE_INTERNAL_CMD_CODE_VX_PORT_TABLE_VLAN       = 8,
    FCOE_INTERNAL_CMD_CODE_VX_PORT_TABLE_VFID       = 9,
    FCOE_INTERNAL_CMD_CODE_VX_PORT_TABLE_ALL        = 10,
    FCOE_INTERNAL_CMD_CODE_VX_PORT_TABLE_RBRIDGEID  = 11,
    FCOE_INTERNAL_CMD_CODE_BINDING_TABLE_TE_INTF    = 12,
    FCOE_INTERNAL_CMD_CODE_BINDING_TABLE_ALL        = 13,
    FCOE_INTERNAL_CMD_CODE_BINDING_TABLE_RBRIDGEID  = 14,
    FCOE_INTERNAL_CMD_CODE_SW_INFO_VLAN             = 15,
    FCOE_INTERNAL_CMD_CODE_SW_INFO_VFID             = 16,
    FCOE_INTERNAL_CMD_CODE_SW_INFO_ALL              = 17,
    FCOE_INTERNAL_CMD_CODE_SW_INFO_RBRIDGEID        = 18,
    FCOE_INTERNAL_CMD_CODE_GLOBAL_INFO_ALL          = 19,
    FCOE_INTERNAL_CMD_CODE_GLOBAL_INFO_RBRIDGEID    = 20,
    FCOE_INTERNAL_CMD_CODE_CEE_STATE                = 21,
    FCOE_INTERNAL_CMD_CODE_BACKEND_MAX                      = 22,
	FCOE_INTERNAL_CMD_CODE_HA_GLOBAL_INFO			= 23,
	FCOE_INTERNAL_CMD_CODE_HA_STATS					= 24,

    /* Internal Commands Codes for DCMd Tables */
    FCOE_INTERNAL_CMD_CODE_DCMD_BEGIN                       = 500,
    FCOE_INTERNAL_CMD_CODE_DCMD_ISL_PORT_TABLE              = 501,
    FCOE_INTERNAL_CMD_CODE_DCMD_ISL_PORT_TABLE_RBRIDGE_ID   = 502,
    FCOE_INTERNAL_CMD_CODE_DCMD_VX_PORT_TABLE               = 503,
    FCOE_INTERNAL_CMD_CODE_DCMD_VX_PORT_TABLE_RBRIDGE_ID    = 504,
    FCOE_INTERNAL_CMD_CODE_DCMD_END                         = 505
}FcoeInternalCmdCode;
typedef enum
{
    FCOE_PLUGIN_SHUT        = 0,
    FCOE_PLUGIN_NO_SHUT     = 1
}FcoePluginCtrlCode;

typedef enum
{
    FCOE_PLUGIN_CMD_CODE_INVALID        = 0,
    FCOE_PLUGIN_CMD_CODE_SHUT_NOTIFY    = 1
}FcoePluginCmdCode;

#define FCOE_FAB_MAP_NAME_LEN_MAX   32
#define FCOE_MAP_NAME_LEN_MAX       32
#define FCOE_CEE_MAP_NAME_LEN_MAX   32
#define FCOE_INTF_NAME_LEN_MAX      32
#define FCOE_FAB_MAP_NUM_MAX        8
#define FCOE_PHY_INTF_NAME_LEN_MAX  48
#define FCOE_TSTAMP_STR_LEN_MAX     32
#define FCOE_INTF_MODE_STR_LEN_MAX  8


typedef struct fcoe_fabric_map_msg_ {
    uint32_t    oper_bmp;
    uint8_t     fab_map_name[FCOE_FAB_MAP_NAME_LEN_MAX];
    uint32_t    vlan_id;
    uint32_t    vfid;
    uint32_t    fcmap;
    uint32_t    priority;
    uint32_t    fka_intvl;
    uint32_t    timeout;
    uint32_t    mtu;
    uint32_t    max_enodes;
    uint32_t    config_replay;
} fcoe_fabric_map_msg_t;

typedef struct fcoe_fabric_map_msg_resp_ {
    uint32_t    cmd_code;
    uint32_t    size;
    uint32_t    num_entries;
    uint32_t    loc_id;
    uint32_t    vlan_id[FCOE_FAB_MAP_NUM_MAX];
    uint8_t     fcf_mac_addr_list[FCOE_FAB_MAP_NUM_MAX][FCOE_MAC_ADDR_LEN];
} fcoe_fabric_map_msg_resp_t;

typedef struct fcoe_map_msg_ {
    uint32_t    oper_bmp;
    uint8_t     map_name[FCOE_MAP_NAME_LEN_MAX];
    uint8_t     fab_map_name[FCOE_FAB_MAP_NAME_LEN_MAX];
    uint8_t     cee_map_name[FCOE_CEE_MAP_NAME_LEN_MAX];
} fcoe_map_msg_t;

typedef struct fcoe_fip_msg_ {
    uint32_t    oper_bmp;
    uint32_t    fka_intvl;
    uint32_t    timeout;
} fcoe_fip_msg_t;

typedef struct fcoe_debug_msg_ {
    uint32_t    oper_bmp;
    uint32_t    send_type;
    uint32_t    service_type;
} fcoe_debug_msg_t;

typedef struct fcoe_internal_msg_ {
    uint32_t    oper_bmp;
    uint32_t    vlan_id;
    uint32_t    vfid;
    uint8_t     if_name[FCOE_INTF_NAME_LEN_MAX];
} fcoe_internal_msg_t;

typedef struct fcoe_plugin_ctrl_msg_ {
    uint32_t    oper_bmp;
    uint32_t    plugin_status;
} fcoe_plugin_ctrl_msg_t;

typedef struct fcoe_fsb_msg_ {
    int     enable;
} fcoe_fsb_msg_t;

typedef struct fcoe_show_cmd_fab_map_req_
{
    uint8_t     fab_map_name[FCOE_FAB_MAP_NAME_LEN_MAX];
} fcoe_show_cmd_fab_map_req_t;

typedef struct fcoe_show_cmd_fab_map_resp_
{
    uint32_t    num_entries;
    struct {
        uint8_t     fab_map_name[FCOE_FAB_MAP_NAME_LEN_MAX];
        uint32_t    vlan_id;
        uint32_t    vfid;
        uint32_t    fcmap;
        uint32_t    priority;
        uint32_t    fka_intvl;
        uint32_t    timeout;
        uint32_t    mtu;
        uint32_t    max_enodes;
    } list[1];
} fcoe_show_cmd_fab_map_resp_t;


typedef struct fcoe_show_cmd_fip_resp_
{
    uint32_t    fka_intvl;
    uint32_t    timeout;
} fcoe_show_cmd_fip_resp_t;

typedef struct fcoe_show_cmd_map_req_
{
    uint8_t     map_name[FCOE_MAP_NAME_LEN_MAX];
} fcoe_show_cmd_map_req_t;

typedef struct fcoe_show_cmd_map_resp_
{
    uint32_t    num_entries;
    struct {
        uint8_t     map_name[FCOE_MAP_NAME_LEN_MAX];
        uint32_t    num_fab_maps;
        uint8_t     fab_map_name[FCOE_FAB_MAP_NUM_MAX][FCOE_FAB_MAP_NAME_LEN_MAX];
        uint8_t     cee_map_name[FCOE_CEE_MAP_NAME_LEN_MAX];
    } list[1];
} fcoe_show_cmd_map_resp_t;

typedef struct fcoe_show_cmd_login_req_
{
    uint32_t    fc_uport;
    uint32_t    vlan_id;
    uint32_t    vfid;
} fcoe_show_cmd_login_req_t;

typedef struct fcoe_show_cmd_login_resp_entry_ {
    uint32_t    fc_uport;
    uint32_t    if_index;
    uint8_t     is_direct_attached;
    uint8_t     connected_peer_type;
    uint8_t     te_intf_name[FCOE_PHY_INTF_NAME_LEN_MAX];
    uint8_t     logical_port_name[FCOE_INTF_NAME_LEN_MAX];
    uint8_t     dev_wwn[FCOE_WWN_LEN];
    uint8_t     dev_mac[FCOE_MAC_ADDR_LEN];
    uint8_t     ssn_mac[FCOE_MAC_ADDR_LEN];
} fcoe_show_cmd_login_resp_entry_t;

typedef struct fcoe_show_cmd_login_resp_
{
    uint32_t    num_entries;
    fcoe_show_cmd_login_resp_entry_t list[1];
} fcoe_show_cmd_login_resp_t;

typedef struct fcoe_show_cmd_intf_req_
{
    uint32_t    fc_uport;
    bool        stats_enable;
} fcoe_show_cmd_intf_req_t;

typedef struct fcoe_show_cmd_intf_resp_entry_ {
        uint32_t    fc_uport;
        uint8_t     logical_port_name[FCOE_INTF_NAME_LEN_MAX];
        uint8_t     cfg_mode[FCOE_INTF_MODE_STR_LEN_MAX];
        uint8_t     curr_mode[FCOE_INTF_MODE_STR_LEN_MAX];
        uint32_t    cfg_status;
        uint32_t    curr_status;
        uint8_t     te_intf_name[FCOE_PHY_INTF_NAME_LEN_MAX];
        uint32_t    num_vn_devices;
        uint32_t    vfid;
        uint8_t     mac[FCOE_MAC_ADDR_LEN];
        uint8_t     enode_mac[FCOE_MAC_ADDR_LEN];
        uint8_t     enode_bind_type;
        uint8_t     port_wwn[FCOE_WWN_LEN];
        uint32_t    if_index;
        uint32_t    is_lag;
        uint32_t    bind_type;
        uint8_t     clear_counter_tstamp[FCOE_TSTAMP_STR_LEN_MAX];
        uint8_t     status_change_tstamp[FCOE_TSTAMP_STR_LEN_MAX];
        uint32_t    rx_vlan_disc_req;
        uint32_t    rx_fcf_disc;
        uint32_t    rx_flogi;
        uint32_t    rx_fdisc;
        uint32_t    rx_logo;
        uint32_t    rx_errors;
        uint32_t    rx_enode_ka;
        uint32_t    rx_vnport_ka;
        uint32_t    tx_vlan_disc_resp;
        uint32_t    tx_fcf_disc_sa;
        uint32_t    tx_fcf_disc_ua;
        uint32_t    tx_flogi_acc;
        uint32_t    tx_fdisc_acc;
        uint32_t    tx_ls_rjt;
        uint32_t    tx_cvls;
        uint32_t    tx_errors;
} fcoe_show_cmd_intf_resp_entry_t;

typedef struct fcoe_show_cmd_intf_resp_ {
    uint32_t                        num_entries;
    bool                            stats_enable;
    fcoe_show_cmd_intf_resp_entry_t list[1];
} fcoe_show_cmd_intf_resp_t;

typedef struct fcoe_show_cmd_phy_intf_fcoe_req_ {
    uint8_t     phy_intf_name[FCOE_PHY_INTF_NAME_LEN_MAX];
} fcoe_show_cmd_phy_intf_fcoe_req_t;
        
typedef struct fcoe_show_cmd_phy_intf_fcoe_resp_ {
    uint8_t     phy_intf_name[FCOE_PHY_INTF_NAME_LEN_MAX];
    uint32_t    priority;
    uint32_t    logical_link_status;
    uint32_t    num_fc_uport_entries;
    uint32_t    fc_uport_list[1];
} fcoe_show_cmd_phy_intf_fcoe_resp_t;

typedef struct fcoe_show_cmd_ethernet_intf_resp_entry_ {
    uint64_t    if_index;
    uint8_t     phy_intf_name[FCOE_PHY_INTF_NAME_LEN_MAX];
    uint8_t     map_name[FCOE_MAP_NAME_LEN_MAX];
} fcoe_show_cmd_ethernet_intf_resp_entry_t;

typedef struct fcoe_show_cmd_ethernet_intf_resp_ {
    uint32_t    num_entries;
    fcoe_show_cmd_ethernet_intf_resp_entry_t    list[1];
} fcoe_show_cmd_ethernet_intf_resp_t;
        
typedef struct fcoe_show_cmd_req_ {
    uint32_t    cmd_code;
    union {
        fcoe_show_cmd_intf_req_t            intf_req;
        fcoe_show_cmd_phy_intf_fcoe_req_t   phy_intf_req;
        fcoe_show_cmd_fab_map_req_t         fab_map_req;
        fcoe_show_cmd_map_req_t             map_req;
        fcoe_show_cmd_login_req_t           login_req;
    }req;
} fcoe_show_cmd_req_t;

typedef struct fcoe_show_cmd_resp_ {
    uint32_t    cmd_code;
    uint32_t    size;
    union {
        fcoe_show_cmd_intf_resp_t            intf_resp;
        fcoe_show_cmd_phy_intf_fcoe_resp_t   phy_intf_resp;
        fcoe_show_cmd_fab_map_resp_t         fab_map_resp;
        fcoe_show_cmd_map_resp_t             map_resp;
        fcoe_show_cmd_login_resp_t           login_resp;
        fcoe_show_cmd_fip_resp_t             fip_resp;
        fcoe_show_cmd_ethernet_intf_resp_t   eth_intf_resp;
    }resp;
} fcoe_show_cmd_resp_t;

typedef struct fcoe_internal_cmd_req_ {
    uint32_t    cmd_code;
    uint32_t    fc_uport;
    uint32_t    vlan_id;
    uint32_t    vfid;
    uint8_t     intf_name[FCOE_INTF_NAME_LEN_MAX];
    uint64_t    if_index;
} fcoe_internal_cmd_req_t;

typedef enum fcoe_db_eth_port_flag_ {
    FCOE_DB_ETH_PORT_FLAG_NONE          = 0x0,
    // PORT_READY recieved from HSL
    FCOE_DB_ETH_PORT_FLAG_PORT_READY    = 0x1,
    FCOE_DB_ETH_PORT_FLAG_CEE_UP        = 0x2,
    FCOE_DB_ETH_PORT_FLAG_LLS_UP        = 0x4,
    FCOE_DB_ETH_PORT_CREATE_DEFAULT     = 0x8,
    FCOE_DB_ETH_PORT_CREATE_PERSISTENT  = 0x10,
    FCOE_DB_ETH_PORT_CONFIG_CACHED      = 0x20,
    FCOE_DB_ETH_PORT_LAG_CFG_PUSHED     = 0x40,
    FCOE_DB_ETH_PORT_LAG_MBR            = 0x80,
    FCOE_DB_ETH_PORT_FCOEPORT_ENABLE    = 0x100
} fcoe_db_eth_port_flag_t;

typedef enum fcoe_db_et_port_flags_ {
    FCOE_DB_ET_PORT_REF_FLAG_NONE       = 0x0,
    FCOE_DB_ET_PORT_REF_FLAG_ET_TREE    = 0x1,
    FCOE_DB_ET_PORT_REF_FLAG_ETH_PORT   = 0x2,
    FCOE_DB_ET_PORT_REF_FLAG_DCMD       = 0x4,
    FCOE_DB_ET_PORT_REF_FLAG_DRIVER     = 0x8,
    FCOE_DB_ET_PORT_FLAG_UPORT_DELETE_IN_PROGRESS = 0x10
} fcoe_db_et_port_flags_t;

typedef enum fcoe_db_vx_port_flags_ {
    FCOE_DB_VX_PORT_FLAG_REF_NONE           = 0x0,
    FCOE_DB_VX_PORT_FLAG_REF_UPORT_TREE     = 0x1,
    FCOE_DB_VX_PORT_FLAG_REF_SW_TREE        = 0x2,
    FCOE_DB_VX_PORT_FLAG_REF_ETH_TREE       = 0x4,
    FCOE_DB_VX_PORT_FLAG_REF_DCMD           = 0x8,
    FCOE_DB_VX_PORT_FLAG_REF_DRIVER         = 0x10,
    FCOE_DB_VX_PORT_FLAG_READY              = 0x20,
    FCOE_DB_VX_PORT_FLAG_UPORT_DELETE_IN_PROGRESS = 0x40,
    FCOE_DB_VX_PORT_FLAG_DYN_BOUND          = 0x80,
    FCOE_DB_VX_PORT_FLAG_ENODE_BIND         = 0x100,
    FCOE_DB_VX_PORT_FLAG_ENODE_BIND_CFG_PUSH_PNDG = 0x200
} fcoe_db_vx_port_flags_t;

typedef enum fcoe_db_fab_map_flag_ {
    FCOE_DB_FAB_MAP_FLAG_NONE               = 0x0,
    FCOE_DB_FAB_MAP_FLAG_VLAN_READY         = 0x1
} fcoe_db_fab_map_flag_t;

typedef enum fcoe_db_boot_flags_ {
    FCOE_DB_BOOT_FLAG_INVALID               = 0x0,
    FCOE_DB_BOOT_FLAG_DEFAULT_BOOT          = 0x1,
    FCOE_DB_BOOT_FLAG_PERSISTENT_BOOT       = 0x2,
    FCOE_DB_BOOT_FLAG_NSM_SERVER_READY      = 0x4,
    FCOE_DB_BOOT_FLAG_FCOE_CHASSIS_READY    = 0x8,
    FCOE_DB_BOOT_FLAG_NO_TRILL_PORTS        = 0x10,
    FCOE_DB_BOOT_FLAG_NO_VX_PORTS           = 0x20
} fcoe_db_boot_flags_t;

typedef enum fcoe_db_dcm_flags_ {
    FCOE_DB_DCM_FLAG_NONE = 0x0,
    FCOE_DB_DCM_FLAG_INIT_COMPLETE = 0x1,
    FCOE_DB_DCM_FLAG_IF_CONFIG_DNLD_COMPLETE = 0x2
} fcoe_db_dcm_flags_t;

typedef struct fcoe_internal_cmd_eth_port_resp_entry_ {
    uint8_t                te_if_name[FCOE_PHY_INTF_NAME_LEN_MAX];
    uint32_t               if_index;
    uint32_t               active_pfc;
    uint32_t               sw_num;
    uint32_t               num_vx_ports;
    uint8_t                et_status;
    fcoe_db_eth_port_flag_t  flags;
} fcoe_internal_cmd_eth_port_resp_entry_t;

typedef struct fcoe_internal_cmd_eth_port_resp_ {
    uint32_t    num_entries;
    fcoe_internal_cmd_eth_port_resp_entry_t    list[1];
} fcoe_internal_cmd_eth_port_resp_t;

typedef struct fcoe_internal_cmd_isl_port_resp_entry_ {
    uint8_t                te_if_name[FCOE_PHY_INTF_NAME_LEN_MAX];
    uint32_t               if_index;
    uint32_t               lif_id;
    uint32_t               sw_num;
    uint32_t               fc_uport;
    uint8_t                isl_status;
    uint8_t                trunk_status;
    fcoe_db_et_port_flags_t  flags;
} fcoe_internal_cmd_isl_port_resp_entry_t;

typedef struct fcoe_internal_cmd_isl_port_resp_ {
    uint32_t    num_entries;
    fcoe_internal_cmd_isl_port_resp_entry_t    list[1];
} fcoe_internal_cmd_isl_port_resp_t;

typedef struct fcoe_internal_cmd_vx_port_resp_entry_ {
    uint8_t               fcoe_if_name[FCOE_INTF_NAME_LEN_MAX];
    uint32_t              fc_uport;
    uint32_t              sw_num;
    uint32_t              lif_id;
    uint8_t               status;
    fcoe_db_vx_port_flags_t  flags;
} fcoe_internal_cmd_vx_port_resp_entry_t;

typedef struct fcoe_internal_cmd_vx_port_resp_ {
    uint32_t    num_entries;
    fcoe_internal_cmd_vx_port_resp_entry_t    list[1];
} fcoe_internal_cmd_vx_port_resp_t;

typedef struct fcoe_internal_binding_info_ {
    uint32_t      vf_num;
    uint32_t      vn_num; 
    uint32_t      sw_num;     
    uint8_t       fcoe_if_name[FCOE_INTF_NAME_LEN_MAX];
} fcoe_internal_binding_info_t;

typedef struct fcoe_internal_cmd_binding_resp_entry_ {
    uint8_t                         te_intf_name[FCOE_PHY_INTF_NAME_LEN_MAX];
    uint32_t                        num_entries;
    fcoe_internal_binding_info_t    binding_list[1];
} fcoe_internal_cmd_binding_resp_entry_t;

typedef struct fcoe_internal_cmd_binding_resp_ {
    uint32_t    num_entries;
    fcoe_internal_cmd_binding_resp_entry_t    list[1];
} fcoe_internal_cmd_binding_resp_t;

typedef struct fcoe_internal_cmd_sw_info_resp_ {
    uint32_t              sw_num;
    uint32_t              vn_num;
    uint8_t               name[FCOE_FAB_MAP_NAME_LEN_MAX];
    uint32_t              vlan;
    uint32_t              ivid;
    uint32_t              ctag;
    uint32_t              vf_id;
    uint32_t              fcmap;
    uint32_t              priority;
    uint32_t              fka_intvl;
    uint32_t              fka_timeout;
    uint32_t              eff_fka_intvl;
    uint32_t              eff_fka_timeout;
    fcoe_db_fab_map_flag_t  flags;
    uint32_t              fsb;
} fcoe_internal_cmd_sw_info_resp_t;

typedef enum fcoe_db_lock_type_ {
	FCOE_DB_LOCK_TYPE_NSM  = 0x0,
	FCOE_DB_LOCK_TYPE_DCM  = 0x1,
	FCOE_DB_LOCK_TYPE_SCN  = 0x2,
	FCOE_DB_LOCK_TYPE_NONE,
	FCOE_DB_LOCK_TYPE_MAX  = FCOE_DB_LOCK_TYPE_NONE
} fcoe_db_lock_type_t;

#define FCOE_DB_LOCK_TYPE_STR_MAX  16

typedef struct fcoe_db_lock_info_ {
	uint8_t     lock_owner[FCOE_DB_LOCK_TYPE_STR_MAX];

	struct {
			uint8_t     type_str[FCOE_DB_LOCK_TYPE_STR_MAX];
			uint32_t    lock_aqu_cnt;
			uint32_t    lock_rel_cnt;
	} type_stats_list[FCOE_DB_LOCK_TYPE_MAX];

} fcoe_db_lock_info_t;

typedef struct fcoe_internal_cmd_global_info_resp_ {
    uint32_t                fka_intvl;
    uint32_t                fka_timeout;
    fcoe_db_dcm_flags_t     dcm_flags;
    fcoe_db_boot_flags_t    boot_flags;
    uint32_t                vcs_mapped_id;
	fcoe_db_lock_info_t		lock_info;
} fcoe_internal_cmd_global_info_resp_t;

typedef struct fcoe_internal_cmd_cee_state_mbr_entry_ {
    uint8_t                if_name[FCOE_PHY_INTF_NAME_LEN_MAX];
    bool                   cee_state;  
} fcoe_internal_cmd_cee_state_mbr_entry_t;

typedef struct fcoe_internal_cmd_cee_state_resp_entry_ {
	uint8_t                if_name[FCOE_PHY_INTF_NAME_LEN_MAX];
    bool                   cee_state;
	bool                   is_lag;
	uint32_t               num_mbrs;
	fcoe_internal_cmd_cee_state_mbr_entry_t mbr_list[1];
} fcoe_internal_cmd_cee_state_resp_entry_t;

typedef struct fcoe_internal_cmd_cee_state_resp_ {
    uint32_t    num_entries;
    fcoe_internal_cmd_cee_state_resp_entry_t    list[1];
} fcoe_internal_cmd_cee_state_resp_t;

// FCOE HA EVENT TYPE
typedef enum fcoe_ha_event_type_ {
    // Reserved for testing/debug
    FCOE_HA_EVENT_TYPE_CHSW_TEST_UPDATE,
    FCOE_HA_EVENT_TYPE_FCSW_TEST_UPDATE,
    FCOE_HA_EVENT_TYPE_ETHSW_TEST_UPDATE,
    FCOE_HA_EVENT_TYPE_CHSW_TEST_INTENT,
    FCOE_HA_EVENT_TYPE_FCSW_TEST_INTENT,
    FCOE_HA_EVENT_TYPE_ETHSW_TEST_INTENT,
	FCOE_HA_EVENT_DEBUG_EN_DIS_REQ,


    // ADD Chassis HA Events here

    // ADD FCSW HA Events here

    // ADD ETHSW HA Events here
    FCOE_HA_EVENT_TYPE_VF_PORT,
    FCOE_HA_EVENT_TYPE_ET_PORT,
    FCOE_HA_EVENT_TYPE_PHY_PORT,
	FCOE_HA_EVENT_TYPE_LAG_PORT,
	FCOE_HA_EVENT_TYPE_VF_PORT_BULK,
	FCOE_HA_EVENT_TYPE_ET_PORT_BULK,
	FCOE_HA_EVENT_TYPE_PHY_PORT_BULK,
	FCOE_HA_EVENT_TYPE_LAG_PORT_BULK,

	FCOE_HA_EVENT_TYPE_GL_INFO,
	FCOE_HA_EVENT_TYPE_GL_FAB_MAP,

	// FCOE-NSM HA EVENTS
	FCOE_HA_EVENT_TYPE_NSM_UPDATE_CEE_UP,
    FCOE_HA_EVENT_TYPE_NSM_UPDATE_CEE_DOWN,
	FCOE_HA_EVENT_TYPE_NSM_UPDATE_VLAN_SET,
    FCOE_HA_EVENT_TYPE_NSM_UPDATE_VLAN_UNSET,
	FCOE_HA_EVENT_TYPE_NSM_UPDATE_LINK_UP,
    FCOE_HA_EVENT_TYPE_NSM_UPDATE_LINK_DOWN,

    // FCOE config events
    FCOE_HA_EVENT_TYPE_FAB_MAP,
    FCOE_HA_EVENT_TYPE_FIP_CFG,
    FCOE_HA_EVENT_TYPE_VF_STATE,
    FCOE_HA_EVENT_TYPE_BIND_UPD,
    FCOE_HA_EVENT_TYPE_REMV_BIND_UPD,
    FCOE_HA_EVENT_TYPE_ET_UPD,
    FCOE_HA_EVENT_TYPE_PHY_FCOEPORT_UPD,
    FCOE_HA_EVENT_TYPE_LAG_FCOEPORT_UPD,
    FCOE_HA_EVENT_TYPE_LAG_MEM_UPD,
	FCOE_HA_EVENT_TYPE_LGCL_PORT_INFO,

	// FCOE Port Model Events
	FCOE_HA_EVENT_TYPE_ETH_PHASE,
	FCOE_HA_EVENT_TYPE_HALC_PHASE,
	FCOE_HA_EVENT_TYPE_LCRT_PHASE,
	FCOE_HA_EVENT_TYPE_LREG_PHASE,
	FCOE_HA_EVENT_TYPE_HXPD_PHASE,
	FCOE_HA_EVENT_TYPE_FPRT_PHASE,
	FCOE_HA_EVENT_TYPE_FCET_DCM_PHASE,
	FCOE_HA_EVENT_TYPE_FCVF_DCM_PHASE,
	FCOE_HA_EVENT_TYPE_DXPD_FPRT_PHASE,
	FCOE_HA_EVENT_TYPE_DXPD_FCET_DCM_PHASE,
	FCOE_HA_EVENT_TYPE_DXPD_FCVF_DCM_PHASE,
	FCOE_HA_EVENT_TYPE_DXPD_FPRT_DIS_PHASE,
	FCOE_HA_EVENT_TYPE_DXPD_HDXPD_PHASE,
	FCOE_HA_EVENT_TYPE_DXPD_LDEL_PHASE,

	FCOE_HA_EVENT_TYPE_SLOTFAILOVER,

    FCOE_HA_EVENT_TYPE_MAX
} fcoe_ha_event_type_t;

typedef enum fcoe_ha_db_serv_type_ {
    FCOE_HA_DB_SERV_CHASSIS = 1,
    FCOE_HA_DB_SERV_FCSW,
    FCOE_HA_DB_SERV_ETHSW
} fcoe_ha_db_serve_type_t;

typedef struct fcoe_internal_ha_info_ {
	/* Versions */
	uint32_t     app_version;                /* Application version */
	uint32_t     ha_version;                 /* Transparent HA API version */
	/* Reg Info */
	uint8_t		fr_service[FCOE_HA_SSIZE];       /* Service name */
	uint8_t		fr_instance[FCOE_HA_SSIZE];      /* Instance name */
	uint8_t		fr_component[FCOE_HA_SSIZE];     /* Component name */
	uint32_t	fr_curver;                  /* Component FSS version */
	uint32_t	fr_minver;                  /* minimum supported */
	/* Handles */
	uint32_t	tha_handle;
	uint32_t	fss_handle;
} fcoe_internal_ha_info_t;


typedef struct fcoe_ha_msg_stats_ {
    uint32_t    num_intents;
    uint32_t    num_commits;
    uint32_t    num_updates;
} fcoe_ha_msg_stats_t;

typedef struct fcoe_ha_stats_ {
    fcoe_ha_event_type_t    event_type;
    fcoe_ha_db_serve_type_t serv_type;
    fcoe_ha_msg_stats_t     stats;
} fcoe_ha_stats_t;

typedef struct fcoe_internal_ha_stats_list_ {
	fcoe_ha_stats_t    rcv_stats[FCOE_HA_EVENT_TYPE_MAX];
    fcoe_ha_stats_t    snd_stats[FCOE_HA_EVENT_TYPE_MAX];
} fcoe_internal_ha_stats_list_t;

typedef struct fcoe_internal_cmd_ha_glinfo_resp_ {
    uint32_t    log_enable;
	fcoe_internal_ha_info_t    chassis_info;
	fcoe_internal_ha_info_t    fcsw_info;
	fcoe_internal_ha_info_t    ethsw_info;
} fcoe_internal_cmd_ha_glinfo_resp_t;

typedef struct fcoe_internal_cmd_ha_stats_resp_ {
    fcoe_internal_ha_stats_list_t		chassis_stats;
    fcoe_internal_ha_stats_list_t		fcsw_stats;
    fcoe_internal_ha_stats_list_t		ethsw_stats; 
} fcoe_internal_cmd_ha_stats_resp_t;

typedef struct fcoe_internal_cmd_resp_ {
    uint32_t    cmd_code;
    uint32_t    size;
    union {
        fcoe_internal_cmd_eth_port_resp_t         eth_port_resp;
        fcoe_internal_cmd_isl_port_resp_t         isl_port_resp;
        fcoe_internal_cmd_vx_port_resp_t          vx_port_resp;
        fcoe_internal_cmd_binding_resp_t          binding_resp;
        fcoe_internal_cmd_sw_info_resp_t          sw_resp;
        fcoe_internal_cmd_global_info_resp_t      global_resp;
        fcoe_internal_cmd_cee_state_resp_t        cee_state_resp;
		fcoe_internal_cmd_ha_glinfo_resp_t        ha_glinfo_resp;
		fcoe_internal_cmd_ha_stats_resp_t         ha_stats_resp;
    }resp;
} fcoe_internal_cmd_resp_t;


typedef struct fcoe_clear_cmd_req_ {
    uint32_t        cmd_code;
    uint64_t        if_index;
    uint8_t         if_name[FCOE_INTF_NAME_LEN_MAX];
    uint32_t        vlan;
    uint32_t        vfid;
    uint32_t        fc_uport_num;
    uint64_t        wwn;
    uint32_t        po;
} fcoe_clear_cmd_req_t;

typedef struct fcoe_intf_msg_ {
    uint32_t    oper_bmp;
    uint8_t     if_name[FCOE_INTF_NAME_LEN_MAX];
    uint64_t    if_index;
    uint32_t    log_if_index;
    uint32_t    status;
} fcoe_intf_msg_t;

typedef struct fcoe_intf_confg_if_list_entry_ {
    uint8_t     if_name[FCOE_PHY_INTF_NAME_LEN_MAX];
    uint8_t     bind_name[FCOE_PHY_INTF_NAME_LEN_MAX];
    uint32_t    bind_type;
    uint32_t    if_index;
    uint32_t    port_num;
    uint32_t    fc_port_num;
    uint32_t    vn_num;
    uint32_t    status;
    uint32_t    trunk_status;
    uint32_t    breakout;
} fcoe_intf_confg_if_list_entry_t;

typedef fcoe_intf_confg_if_list_entry_t fcoe_et_intf_confg_if_list_entry_t;

typedef struct fcoe_intf_config_msg_ {
    uint32_t    cmd_code;
    uint32_t    num_entries;
    uint32_t    size;
    uint32_t    config_replay;
    fcoe_intf_confg_if_list_entry_t list[1];
} fcoe_intf_config_msg_t;

#define FCOE_PLUG_LAG_MEMBERS_MAX 16
typedef struct fcoe_eth_intf_config_msg_ {
    uint32_t    cmd_code;
    uint8_t     if_name[FCOE_PHY_INTF_NAME_LEN_MAX];
    uint64_t    if_index;
    uint8_t     map_name[FCOE_MAP_NAME_LEN_MAX];
    uint8_t     cee_map_name[FCOE_CEE_MAP_NAME_LEN_MAX];
    uint32_t    num_vlans;
    uint32_t    fcoe_vlan_list[FCOE_FAB_MAP_NUM_MAX];
    uint32_t    num_entries;
    uint64_t    mbr_if_index_list[FCOE_PLUG_LAG_MEMBERS_MAX];
} fcoe_eth_intf_config_msg_t;

typedef fcoe_intf_config_msg_t fcoe_et_intf_config_msg_t;

typedef fcoe_intf_config_msg_t fcoe_intf_config_msg_resp_t;
#ifdef __cplusplus
}
#endif

#endif
