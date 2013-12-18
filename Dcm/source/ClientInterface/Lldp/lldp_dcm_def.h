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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.                *
 *   All rights reserved.                                                   *
 *   Author : Sanjeev Joshi                                                 *
 *   File: lldp_dcm_def.h                                                   *
 *   Description:                                                           *
 *                                                                          *
 *      The common C structure definition for back-end to interface with    *
 *       DCMd                                                               *
 ***************************************************************************/

#ifndef LLDPDCMDEF_H
#define LLDPDCMDEF_H


#include <stdio.h>
#include <unistd.h>
#include "ClientInterface/InterfaceCommon.h"

#define PACKED                   __attribute__((packed)) 

#define TRUE_NTOH                1
#define FALSE_NTOH               0

#define LLDP_IF_STR_MAX_LEN             64
#define LLDP_PROFILE_MAX_LEN            32
#define LLDP_DESCR_MAX_LEN              50
#define LLDP_NAME_MAX_LEN               50
#define LLDP_MGMT_SMALL_BUF             128
#define LLDP_MGMT_ERR_STR_MAX_LEN       256
#define LLDP_MGMT_COMPOSITE_MAX_SIZE    50
#define LLDP_MGMT_ETHER_ADDR_LEN        6
#define LLDP_VLAN_NAME_MAX_LEN          16
#define LLDP_MGMT_PROTO_STR_MAX_LEN     128
#define LLDP_MGMT_MAX_OID_LEN           128


#define LLDP_CLI_TKN_MAX_LEN    128
#define LLDP_CLI_TKN_INTF   "interface"
#define LLDP_CLI_TKN_NBR   "neighbors"
#define LLDP_CLI_TKN_STATS "statistics"
#define LLDP_CLI_TKN_DETAIL "detail"

#define LLDP_DCM_CLIENT_NAME    "LldpDCMClient"


#define LLDP_MGMT_CHECK_FLAG(V,F)      ((V) & (F))

/* Tag Names */
typedef enum lldp_mgmt_show_buf_ { 
    LLDP_MGMT_SHOW_BUF_UNKNOWN = 0,
    LLDP_MGMT_SHOW_BUF_GBL, 
    LLDP_MGMT_SHOW_BUF_INTF, 
    LLDP_MGMT_SHOW_BUF_NBR,
    LLDP_MGMT_SHOW_BUF_NBR_DETL,
    LLDP_MGMT_SHOW_BUF_NBR_INTF,
    LLDP_MGMT_SHOW_BUF_NBR_INTF_DETL,
    LLDP_MGMT_SHOW_BUF_STATS,
    LLDP_MGMT_SHOW_BUF_STATS_INTF
} lldp_mgmt_show_buf_t;

/* Tag Names */
typedef enum lldp_mgmt_clear_buf_ { 
    LLDP_MGMT_CLEAR_BUF_UNKNOWN = 0,
    LLDP_MGMT_CLEAR_BUF_NBR,
    LLDP_MGMT_CLEAR_BUF_NBR_INTF,
    LLDP_MGMT_CLEAR_BUF_STATS,
    LLDP_MGMT_CLEAR_BUF_STATS_INTF
} lldp_mgmt_clear_buf_t;


/* show nsm cmd code */
typedef enum lldp_mgmt_cmd_code_ {
    LLDP_MGMT_CMD_UNKNOWN = 0,
    LLDP_MGMT_CMD_SHOW_GBL,
    LLDP_MGMT_CMD_SHOW_INTF, 
    LLDP_MGMT_CMD_SHOW_INTF_GI, 
    LLDP_MGMT_CMD_SHOW_NBR,
    LLDP_MGMT_CMD_SHOW_NBR_DETL,
    LLDP_MGMT_CMD_SHOW_NBR_INTF,
    LLDP_MGMT_CMD_SHOW_NBR_INTF_GI,
    LLDP_MGMT_CMD_SHOW_NBR_INTF_DETL,
    LLDP_MGMT_CMD_SHOW_NBR_INTF_DETL_GI,
    LLDP_MGMT_CMD_SHOW_STATS,
    LLDP_MGMT_CMD_SHOW_STATS_INTF,
    LLDP_MGMT_CMD_SHOW_STATS_INTF_GI,
    LLDP_MGMT_CMD_CLEAR_NBR,
    LLDP_MGMT_CMD_CLEAR_NBR_INTF,
    LLDP_MGMT_CMD_CLEAR_NBR_INTF_GI,
    LLDP_MGMT_CMD_CLEAR_STATS,
    LLDP_MGMT_CMD_CLEAR_STATS_INTF,
    LLDP_MGMT_CMD_CLEAR_STATS_INTF_GI,
    LLDP_MGMT_CMD_SHOW_INTF_FO, 
    LLDP_MGMT_CMD_SHOW_NBR_INTF_FO,
    LLDP_MGMT_CMD_SHOW_NBR_INTF_DETL_FO,
    LLDP_MGMT_CMD_SHOW_STATS_INTF_FO
} lldp_mgmt_cmd_code_t;

typedef enum lldp_mgmt_err_code_ { 
    LLDP_MGMT_ERR_SUCCESS = 0,
    LLDP_MGMT_ERR_FAILURE = 1,
    LLDP_MGMT_ERR_ONMD_NOT_EXIST = 2,
    LLDP_MGMT_ERR_MASTER_NOT_EXIST = 3,
    LLDP_MGMT_ERR_NO_SUCH_INTF = 4, 
    LLDP_MGMT_ERR_CMD_ALLOWED_ON_PHY_INTF = 5,
    LLDP_MGMT_ERR_ONMD_INTF_NOT_ACTIVE = 6,
    LLDP_MGMT_ERR_LLDP_INTF_NOT_ACTIVE = 7,
    LLDP_MGMT_ERR_INTERFACE_NOT_ETHERNET = 8,
    LLDP_MGMT_ERR_MAX
} lldp_mgmt_err_code_t;

typedef enum lldp_cee_on_off_ {
	    LLDP_CEE_OFF = 0,
		    LLDP_CEE_ON
}lldp_cee_on_off_t;

typedef struct lldp_mgmt_err_str_ { 
    char    str[LLDP_MGMT_ERR_STR_MAX_LEN + 1];
} lldp_mgmt_err_str_t;

static inline char* 
lldp_mgmt_err2str (int  err_num)
{
    static lldp_mgmt_err_str_t err_str[LLDP_MGMT_ERR_MAX + 1];
    static u_int8_t init_done = 0;
    
    if (!init_done) { 
        strncpy(err_str[LLDP_MGMT_ERR_SUCCESS].str,"Success.",LLDP_MGMT_ERR_STR_MAX_LEN);
		err_str[LLDP_MGMT_ERR_SUCCESS].str[LLDP_MGMT_ERR_STR_MAX_LEN-1] = '\0';
        strncpy(err_str[LLDP_MGMT_ERR_FAILURE].str,"Failure.",LLDP_MGMT_ERR_STR_MAX_LEN);
		err_str[LLDP_MGMT_ERR_FAILURE].str[LLDP_MGMT_ERR_STR_MAX_LEN-1] = '\0';
        strncpy(err_str[LLDP_MGMT_ERR_ONMD_NOT_EXIST].str,"ONMD does not exit.",LLDP_MGMT_ERR_STR_MAX_LEN);
		err_str[LLDP_MGMT_ERR_ONMD_NOT_EXIST].str[LLDP_MGMT_ERR_STR_MAX_LEN-1] = '\0';
        strncpy(err_str[LLDP_MGMT_ERR_MASTER_NOT_EXIST].str,"The LLDP master does not exist.",LLDP_MGMT_ERR_STR_MAX_LEN);
		err_str[LLDP_MGMT_ERR_MASTER_NOT_EXIST].str[LLDP_MGMT_ERR_STR_MAX_LEN-1] = '\0';
        strncpy(err_str[LLDP_MGMT_ERR_NO_SUCH_INTF].str,"No such interface.",LLDP_MGMT_ERR_STR_MAX_LEN);
		err_str[LLDP_MGMT_ERR_NO_SUCH_INTF].str[LLDP_MGMT_ERR_STR_MAX_LEN-1]  ='\0';
        strncpy(err_str[LLDP_MGMT_ERR_ONMD_INTF_NOT_ACTIVE].str,"ONMD interface Not Active.",LLDP_MGMT_ERR_STR_MAX_LEN);
		err_str[LLDP_MGMT_ERR_ONMD_INTF_NOT_ACTIVE].str[LLDP_MGMT_ERR_STR_MAX_LEN-1]  = '\0';
        strncpy(err_str[LLDP_MGMT_ERR_LLDP_INTF_NOT_ACTIVE].str,"LLDP interface Not Active.",LLDP_MGMT_ERR_STR_MAX_LEN);
		err_str[LLDP_MGMT_ERR_ONMD_INTF_NOT_ACTIVE].str[LLDP_MGMT_ERR_STR_MAX_LEN-1]  = '\0';
        strncpy(err_str[LLDP_MGMT_ERR_INTERFACE_NOT_ETHERNET].str,"Command allowed only on Physical Interface.",LLDP_MGMT_ERR_STR_MAX_LEN);
		err_str[LLDP_MGMT_ERR_INTERFACE_NOT_ETHERNET].str[LLDP_MGMT_ERR_STR_MAX_LEN-1]  = '\0';
        strncpy(err_str[LLDP_MGMT_ERR_MAX].str, "Unknown Error code",LLDP_MGMT_ERR_STR_MAX_LEN);
		err_str[LLDP_MGMT_ERR_MAX].str[LLDP_MGMT_ERR_STR_MAX_LEN-1] = '\0';
        init_done = 1;
    }

    if ( (err_num >= LLDP_MGMT_ERR_MAX) ||
         (err_num < 0 ) ) { 
        return err_str[LLDP_MGMT_ERR_MAX].str;
    }

    return err_str[err_num].str;
}

typedef struct lldp_mgmt_int_req_ { 
    char       if_name[LLDP_IF_STR_MAX_LEN + 1];
    char       interface[LLDP_IF_STR_MAX_LEN + 1];
} lldp_mgmt_int_req_t;

typedef struct lldp_mgmt_stats_int_req_ { 
    char       if_name[LLDP_IF_STR_MAX_LEN + 1];
    char       interface[LLDP_IF_STR_MAX_LEN + 1];
    u_int32_t  last_ifindex;  /* Return entries with ifindex greater than this */
    u_int32_t  max_records;   /* Max number of entries to return */
} lldp_mgmt_stats_int_req_t;

typedef struct lldp_debug_int_req_ { 
    char       if_name[LLDP_IF_STR_MAX_LEN + 1];
    char       interface[LLDP_IF_STR_MAX_LEN + 1];
    u_int32_t  last_ifindex;  /* Return entries with ifindex greater than this */
    u_int32_t  max_records;   /* Max number of entries to return */
} lldp_debug_int_req_t;

typedef struct lldp_mgmt_nbr_req_ { 
    char       if_name[LLDP_IF_STR_MAX_LEN + 1];
    u_int32_t  last_ifindex;  /* Return entries with ifindex greater than this */
    u_int32_t  max_records;   /* Max number of entries to return */
    u_int8_t   detail;
} lldp_mgmt_nbr_req_t;


typedef struct lldp_mgmt_req_ { 
    lldp_mgmt_cmd_code_t  cmd_code;

    union { 
        lldp_mgmt_int_req_t         intf;
        lldp_mgmt_nbr_req_t         nbr;
        lldp_mgmt_stats_int_req_t   stats_intf;
    } entry;
} lldp_mgmt_req_t;


#define LLDP_MGMT_ADV_DOT1_CONFIGURED                    (1 << 0)
#define LLDP_MGMT_ADV_DOT3_CONFIGURED                    (1 << 1)
#define LLDP_MGMT_ADV_DCBX_CONFIGURED                    (1 << 2)
#define LLDP_MGMT_ADV_DCBX_FCOE_APP_CONFIGURED           (1 << 3)
#define LLDP_MGMT_ADV_DCBX_FCOE_LOGICAL_LINK_CONFIGURED  (1 << 4)
#define LLDP_MGMT_ADV_OPTIONAL_CONFIGURED                (1 << 5)
#define LLDP_MGMT_DESC_CONFIGURED                        (1 << 6)
#define LLDP_MGMT_HELLO_CONFIGURED                       (1 << 7)
#define LLDP_MGMT_MODE_CONFIGURED                        (1 << 8)
#define LLDP_MGMT_MULTIPLIER_CONFIGURED                  (1 << 9)
#define LLDP_MGMT_ADV_OPTIONAL_CLEAR                     (1 << 10)
#define LLDP_MGMT_ADV_DCBX_ISCSI_APP_CONFIGURED          (1 << 11)

/* LLDP Profile structure */
typedef struct lldp_mgmt_profile_ {
    /* Profile name. */
    char      name[LLDP_PROFILE_MAX_LEN + 4];
    char      description[LLDP_DESCR_MAX_LEN + 2];
    u_int16_t flags;
    u_int16_t  adv_dot1;
    u_int16_t  adv_dot3;
    u_int16_t  adv_dcbx;
    u_int16_t  mode;
    u_int16_t  adv_optional;
    u_int32_t  adv_dcbx_fcoe_app;
    u_int32_t  adv_dcbx_fcoe_logical_link;
    u_int32_t  adv_dcbx_iscsi_app;
    u_int32_t multiplier;
    u_int32_t hello;
} PACKED lldp_mgmt_profile_t;


#define LLDP_MGMT_SYSTEM_NAME_CFG        (1 << 0)
#define LLDP_MGMT_SYSTEM_DESC_CFG        (1 << 1)

#define LLDP_MGMT_CONF_DISABLED        (1 << 0)
#define LLDP_MGMT_NO_PROTOCOL          (1 << 5)
#define LLDP_MGMT_DISABLED  (LLDP_MGMT_NO_PROTOCOL | LLDP_MGMT_CONF_DISABLED)


#define LLDP_MGMT_ENABLED_RX_ONLY                      (1 << 1)
#define LLDP_MGMT_ENABLED_TX_ONLY                      (1 << 2)
#define LLDP_MGMT_ENABLED_RX_TX                        (1 << 3)
#define LLDP_MGMT_INTERFACE_DISABLED                   (1 << 4)

#define LLDP_MGMT_CHASSIS_ID_TLV_TX_ENABLE                  (1 << 0)
#define LLDP_MGMT_PORT_ID_TLV_TX_ENABLE                     (1 << 1)
#define LLDP_MGMT_TTL_TLV_TX_ENABLE                         (1 << 2)
#define LLDP_MGMT_PORT_DESCRIPTION_TLV_TX_ENABLE            (1 << 3)
#define LLDP_MGMT_SYSTEM_NAME_TLV_TX_ENABLE                 (1 << 4)
#define LLDP_MGMT_SYSTEM_DESCRIPTION_TLV_TX_ENABLE          (1 << 5)
#define LLDP_MGMT_SYSTEM_CAPABILITIES_TLV_TX_ENABLE         (1 << 6)
#define LLDP_MGMT_MANAGEMENT_ADDRESS_TLV_TX_ENABLE          (1 << 7)
#define LLDP_MGMT_IEEE_8021_ORG_SPECIFIC_TLV_TX_ENABLE      (1 << 8)
#define LLDP_MGMT_IEEE_8023_ORG_SPECIFIC_TLV_TX_ENABLE      (1 << 9)
#define LLDP_MGMT_IEEE_DCBX_ORG_SPECIFIC_TLV_TX_ENABLE      (1 << 10)
#define LLDP_MGMT_IEEE_DCBX_FCOE_APP_SPECIFIC_TLV_TX_ENABLE (1 << 11)
#define LLDP_MGMT_IEEE_DCBX_FCOE_LOGICAL_LINK_SPECIFIC_TLV_TX_ENABLE (1 << 12)
#define LLDP_MGMT_LINK_PRIM_TLV_TX_ENABLE                   (1 << 13)
#define LLDP_MGMT_BRCD_LINK_TLV_TX_ENABLE                   (1 << 14)
#define LLDP_MGMT_IEEE_DCBX_ISCSI_APP_SPECIFIC_TLV_TX_ENABLE (1 << 15)


#define LLDP_MGMT_MSG_TX_HOLD_DEFAULT                  4
#define LLDP_MGMT_MSG_TX_INTERVAL_DEFAULT              30
#define LLDP_MGMT_REINIT_DELAY_DEFAULT                 2
#define LLDP_MGMT_TX_DELAY_DEFAULT                     1

#define LLDP_DEFAULT_PROTOCOL_FLAG                     true
#define LLDP_DEFAULT_DESCRIPTION                       ""
#define LLDP_DEFAULT_HELLO							   30
#define LLDP_DEFAULT_MODE							   LLDP_MODE_RXTX
#define LLDP_DEFAULT_MULTIPLIER						   4
#define LLDP_DEFAULT_DCBXFCOEAPPTLV					   true
#define LLDP_DEFAULT_DCBXFCOELOGICALLINKTLV			   true
#define LLDP_DEFAULT_DCBXISCSIAPPTLV				   false
#define DEFAULT_ISCSI_PRIORITY                         4
#define LLDP_DEFAULT_DCBXTLV						   true
#define LLDP_DEFAULT_DOT1XTLV						   false
#define LLDP_DEFAULT_DOT3XTLV						   false
#define LLDP_DEFAULT_MGMTADDRTLV					   false
#define LLDP_DEFAULT_PORTDESCRTLV					   false
#define LLDP_DEFAULT_SYSCAPTLV						   false
#define LLDP_DEFAULT_SYSDESCRTLV					   false
#define LLDP_DEFAULT_SYSNAMETLV						   false
#define LLDP_DEFAULT_SYSTEM_NAME					   ""
#define LLDP_DEFAULT_SYSTEM_DESCRIPTION                "Brocade VDX Switch"
#define LLDP_DEFAULT_DISABLE			               false

#define LLDP_MGMT_LINK_CEE_CAPABLE			(1 << 0)
#define LLDP_MGMT_LINK_FCOE_LLS_READY		(1 << 1)
#define LLDP_MGMT_LINK_BRCD_NEIGHBOR    	(1 << 2)
#define LLDP_MGMT_LINK_FCOE_INCAPABLE		(1 << 3)
#define LLDP_MGMT_LINK_FCOE_CAPABLE			(1 << 4)
#define LLDP_MGMT_LINK_PFC_ON               (1 << 5)

#define LLDP_MGMT_PORT_VLAN_ID_TX_ENABLE                    (1 << 0)
#define LLDP_MGMT_PORT_VLAN_NAME_TX_ENABLE                  (1 << 1)
#define LLDP_MGMT_PROTO_VLAN_ID_TX_ENABLE                   (1 << 2)
#define LLDP_MGMT_PROTOCOL_ID_TX_ENABLE                     (1 << 3)
#define LLDP_MGMT_MAC_PHY_CONFIG_STATUS_TX_ENABLE           (1 << 4)
#define LLDP_MGMT_LINK_AGG_TX_ENABLE                        (1 << 5)
#define LLDP_MGMT_MAX_FRAME_SIZE_TX_ENABLE                  (1 << 6)
#define LLDP_MGMT_DCBX_CTRL_TX_ENABLE						(1 << 7)
#define LLDP_MGMT_DCBX_ETS_TX_ENABLE                        (1 << 8)
#define LLDP_MGMT_DCBX_PFC_TX_ENABLE                        (1 << 9)
#define LLDP_MGMT_DCBX_APP_TX_ENABLE                        (1 << 10)
#define LLDP_MGMT_DCBX_FCOE_APP_TX_ENABLE                   (1 << 11)
#define LLDP_MGMT_DCBX_LOGIC_LINK_TX_ENABLE                 (1 << 12)
#define LLDP_MGMT_DCBX_FCOE_LOGIC_LINK_TX_ENABLE            (1 << 13)
#define LLDP_MGMT_DCBX_LAN_LOGIC_LINK_TX_ENABLE             (1 << 14)
#define LLDP_MGMT_DCBX_ISCSI_APP_TX_ENABLE                  (1 << 15)

typedef struct  lldp_mgmt_profile_master_ { 
    u_int8_t  admin_status;
    u_int8_t  fcoe_priority_bits;
    u_int8_t  iscsi_priority_bits;
    u_int8_t  resvd1;

    u_int32_t basic_tlvs_tx_enable;

    u_int16_t sub_tlvs_tx_enable;
    u_int16_t reinit_delay;

    u_int32_t msg_tx_hold;
    u_int32_t msg_tx_interval;
    u_int32_t tx_delay;

    char      system_desc[LLDP_DESCR_MAX_LEN + 2];
    char      system_name[LLDP_NAME_MAX_LEN + 2];
    char      description[LLDP_DESCR_MAX_LEN + 2];

    u_int8_t  system_cfg;
    u_int8_t  resvd2[3];
} PACKED lldp_mgmt_profile_master_t;



typedef struct lldp_mgmt_gbl_resp_ {
    lldp_mgmt_err_code_t  ret_code;
    lldp_mgmt_profile_master_t entry;
} PACKED lldp_mgmt_gbl_resp_t;




#define LLDP_MGMT_PRE_CEE_DCBX		1  /* Pre CEE Author Group standard */ 
#define LLDP_MGMT_CEE_DCBX			2  /* Modification by CEE Author Group */



typedef struct lldp_mgmt_show_intf_ { 
    char        if_name[LLDP_IF_STR_MAX_LEN + 4];
    u_int8_t    admin_status;
    u_int8_t    mode;
    u_int8_t    dcbx_subtype;
    u_int8_t    auto_sense;


    u_int32_t   msg_tx_hold;
    u_int32_t   msg_tx_interval;
    u_int32_t   reinit_delay;
    u_int32_t   tx_delay;

    char        profile_name[LLDP_PROFILE_MAX_LEN + 4];

    u_int32_t   basic_tlvs_tx_enable;
    u_int16_t   sub_tlvs_tx_enable;

    u_int8_t	fcoe_priority_bits;
    u_int8_t	fcoe_priority_configured;
    u_int8_t	iscsi_priority_bits;
    u_int8_t	iscsi_priority_configured;

    u_int32_t   resvd[2];
} PACKED lldp_mgmt_show_intf_t;

typedef struct lldp_mgmt_show_intf_resp_ {
    lldp_mgmt_err_code_t    ret_code;
    lldp_mgmt_show_intf_t   entry;
} PACKED lldp_mgmt_show_intf_resp_t;


typedef struct lldp_mgmt_clear_resp_ {
    lldp_mgmt_err_code_t    ret_code;
} PACKED lldp_mgmt_clear_resp_t;

typedef struct lldp_mgmt_show_stats_intf_ { 
    char      if_name[LLDP_IF_STR_MAX_LEN + 4];
    u_int32_t ifindex;
    u_int32_t frames_out_total;
    u_int32_t ageouts_total;
    u_int32_t frames_discarded_total;
    u_int32_t frames_in_errors_total;
    u_int32_t frames_in_total;
    u_int32_t tlvs_discarded_total;
    u_int32_t tlvs_unrecognized_total;
    u_int32_t  resvd[2];
} PACKED lldp_mgmt_show_stats_intf_t;

typedef struct lldp_mgmt_show_stats_resp_ {
    lldp_mgmt_err_code_t    ret_code;
    u_int8_t                ismore;
    u_int32_t               num_records;
    lldp_mgmt_show_stats_intf_t  stats[1];
} PACKED lldp_mgmt_show_stats_resp_t;




#define LLDP_MGMT_REMOTE_LINK_PRIM_VALID			0x00000001
#define LLDP_MGMT_REMOTE_LINK_BRCD_VALID			0x00000002
#define LLDP_MGMT_REMOTE_DESCR_VALID				0x00000004
#define LLDP_MGMT_REMOTE_SYS_NAME_VALID			    0x00000008
#define LLDP_MGMT_REMOTE_SYS_DESCR_VALID			0x00000010

#define LLDP_MGMT_REMOTE_SYS_CAP_VALID			    0x00000020
#define LLDP_MGMT_REMOTE_SYS_CAP_ENABLED_VALID	    0x00000040
#define LLDP_MGMT_REMOTE_MGMT_ADDR_VALID			0x00000080
#define LLDP_MGMT_REMOTE_IF_NUMBERING_VALID		    0x00000100

#define LLDP_MGMT_REMOTE_IF_NUMBER_VALID			0x00000200
#define LLDP_MGMT_REMOTE_OID_VALID				    0x00000400
#define LLDP_MGMT_REMOTE_PORT_VLAN_ID_VALID		    0x00000800
#define LLDP_MGMT_REMOTE_PPVID_FLAG_VALID			0x00001000

#define LLDP_MGMT_REMOTE_PP_VLAN_ID_VALID			0x00002000
#define LLDP_MGMT_REMOTE_PROTOCOL_VALID			    0x00004000
#define LLDP_MGMT_REMOTE_AUTONEGO_SUPPORT_VALID	    0x00008000
#define LLDP_MGMT_REMOTE_AUTONEGO_CAP_VALID		    0x00010000

#define LLDP_MGMT_REMOTE_OPER_MAU_TYPE_VALID		0x00020000
#define LLDP_MGMT_REMOTE_LINK_AGGR_STATUS_VALID	    0x00040000
#define LLDP_MGMT_REMOTE_LINK_AGGR_ID_VALID		    0x00080000
#define LLDP_MGMT_REMOTE_MAX_FRAME_SIZE_VALID		0x00100000

#define LLDP_MGMT_REMOTE_DCBX_TLV_VALID			    0x00200000


#define LLDP_MGMT_REMOTE_DCBX_CTRL_VALID			0x00000001
#define LLDP_MGMT_REMOTE_DCBX_ETS_VALID			    0x00000002
#define LLDP_MGMT_REMOTE_DCBX_PFC_VALID			    0x00000004
#define LLDP_MGMT_REMOTE_DCBX_FCOE_APP_VALID		0x00000008
#define LLDP_MGMT_REMOTE_DCBX_FCOE_LOGIC_LINK_VALID 0x00000010
#define LLDP_MGMT_REMOTE_DCBX_LAN_LOGIC_LINK_VALID  0x00000020
#define LLDP_MGMT_REMOTE_DCBX_ISCSI_APP_VALID		0x00000040


#define ID_TYPE_MAC				0x0001
#define ID_TYPE_IPV4			0x0002
#define ID_TYPE_IPV6			0x0004
#define ID_TYPE_IF_NAME			0x0008
#define ID_TYPE_LOCAL			0x0010
#define ID_TYPE_CHASSIS_COMP	0x0020
#define ID_TYPE_PORT_COMP		0x0040
#define ID_TYPE_UNKNOWN			0x0080


typedef struct lldp_mgmt_composite_data_
{
	u_int8_t type;
	u_int8_t size;
	char     value[LLDP_MGMT_COMPOSITE_MAX_SIZE];
} PACKED lldp_mgmt_composite_data_t;

typedef struct lldp_mgmt_dcbx_ctrl_tlv_ {
	u_int16_t	type;
	u_int16_t	length;
	u_int32_t	seq_no;
	u_int32_t	ack_no;
	u_int8_t	oper_ver;
	u_int8_t	max_ver;
    u_int16_t   resvd;
} PACKED lldp_mgmt_dcbx_ctrl_tlv_t;


typedef struct lldp_mgmt_dcbx_tlv_hdr_ {
	u_int16_t 	type;
	u_int16_t	length;

	u_int32_t	featureSyncNo;
   
	u_int8_t	oper_ver;
	u_int8_t	max_ver;
	u_int8_t	enable;
	u_int8_t	willing;
	u_int8_t	error;
	u_int8_t	reserved;
	u_int8_t	sub_type;

	/* Feature State Variables           */
	/* Not applicable on the remote lldp */
	u_int8_t		advertise;
	u_int8_t		operMode;
	u_int8_t		syncd;

#define LLDP_MGMT_OPERATION_AS_DESIRED	(1 << 0)
#define LLDP_MGMT_OPERATION_AS_PEER		(1 << 1)
	u_int8_t	operCfg;
    u_int8_t    resvd;

} PACKED lldp_mgmt_dcbx_tlv_hdr_t;


typedef struct lldp_mgmt_ets_cfg_ {
    /* this array maps each priority (0-7) traffic to */
    /* a bandwidth group                              */
    uint8_t pri_map[8];

    /* Bandwidth group definition, how much percentage   */
    /* is allocated for each group. Group 15 is reserved */
    /* for Strict Priority traffic to use.               */
    struct {
        uint8_t   bwg_id;
        uint8_t   percentage;
    } bwg[16];
    uint8_t   grpCnt;

    /* Number of TCs supported by the device */
    uint8_t numTCSupported;
    u_int16_t   resvd;
} PACKED lldp_mgmt_ets_cfg_t;


typedef struct lldp_mgmt_dcbx_ets_ {
	lldp_mgmt_dcbx_tlv_hdr_t	   hdr;
	lldp_mgmt_ets_cfg_t		       cfg;
} PACKED lldp_mgmt_dcbx_ets_t;

typedef struct lldp_mgmt_dcbx_pfc_ {
	lldp_mgmt_dcbx_tlv_hdr_t    hdr;
	u_int8_t                    admin_map;
	u_int8_t	                numTCPFCSupported;
    u_int16_t   resvd;
} PACKED lldp_mgmt_dcbx_pfc_t;

typedef struct lldp_mgmt_dcbx_app_ {
	lldp_mgmt_dcbx_tlv_hdr_t    hdr;
	u_int8_t		user_priority_map;
    u_int8_t        resvd[3];
} PACKED lldp_mgmt_dcbx_app_t;

typedef struct lldp_mgmt_dcbx_logic_link_ {
	lldp_mgmt_dcbx_tlv_hdr_t    hdr;
	u_int8_t   		link_status;
	u_int8_t		reserved;
	u_int8_t		resvd[2];
} PACKED lldp_mgmt_dcbx_logic_link_t;

typedef struct lldp_mgmt_dcbx_ {
    /* Control State Variable */
	u_int32_t		rcvAckNo;

	lldp_mgmt_dcbx_ctrl_tlv_t	ctrl;

	/* BASE */
	lldp_mgmt_dcbx_ets_t    ets;
	lldp_mgmt_dcbx_pfc_t	pfc;

	/* APPLICATION */
    lldp_mgmt_dcbx_app_t            iscsi_app;
	lldp_mgmt_dcbx_app_t            fcoe_app;
	lldp_mgmt_dcbx_logic_link_t     fcoe_logic_link;
	lldp_mgmt_dcbx_logic_link_t	    lan_logic_link;
} PACKED lldp_mgmt_dcbx_t ;

typedef struct lldp_mgmt_vlan_ { 
    char        vlan_name[LLDP_VLAN_NAME_MAX_LEN + 4];
    u_int16_t   vlan_id;
    u_int16_t   resvd;
} PACKED lldp_mgmt_vlan_t;


typedef struct lldp_mgmt_vlan_list_ { 
    u_int32_t           num_vlans;
    lldp_mgmt_vlan_t    vlans[1];
} PACKED lldp_mgmt_vlan_list_t;



#define LLDP_MGMT_SYS_CAP_OTHERS                              (1 << 0)
#define LLDP_MGMT_SYS_CAP_REPEATER                            (1 << 1)
#define LLDP_MGMT_SYS_CAP_L2_SWITCHING                        (1 << 2)
#define LLDP_MGMT_SYS_CAP_WLAN_ACCESS_POINT                   (1 << 3)
#define LLDP_MGMT_SYS_CAP_ROUTING                             (1 << 4)
#define LLDP_MGMT_SYS_CAP_TELEPHONE                           (1 << 5)
#define LLDP_MGMT_SYS_CAP_DOCSIS                              (1 << 6)
#define LLDP_MGMT_SYS_CAP_STATION_ONLY                        (1 << 7)

#define LLDP_MGMT_SYSTEM_CAPABILITIES_DEFAULT  \
    (LLDP_MGMT_SYS_CAP_L2_SWITCHING| LLDP_MGMT_SYS_CAP_ROUTING)

typedef enum lldp_mgmt_protocol_ {
    LLDP_MGMT_PROTO_STP,
    LLDP_MGMT_PROTO_RSTP,
    LLDP_MGMT_PROTO_MSTP,
    LLDP_MGMT_PROTO_GMRP,
    LLDP_MGMT_PROTO_MMRP,
    LLDP_MGMT_PROTO_GVRP,
    LLDP_MGMT_PROTO_MVRP,
    LLDP_MGMT_PROTO_LACP,
    LLDP_MGMT_PROTO_DOT1X,
    LLDP_MGMT_PROTO_EFM_OAM,
    LLDP_MGMT_PROTO_UNKNOWN,
    LLDP_MGMT_PROTO_MAX
} lldp_mgmt_protocol_t;

typedef struct lldp_mgmt_protocol_identity_
{
    char    str[LLDP_MGMT_PROTO_STR_MAX_LEN + 1];
} lldp_mgmt_protocol_identity_t;


static inline char* 
lldp_mgmt_proto2str (int  proto_num)
{
    static lldp_mgmt_protocol_identity_t proto_str[LLDP_MGMT_PROTO_MAX + 1];
    static u_int8_t init_done = 0;
    
    if (!init_done) { 
        strncpy(proto_str[LLDP_MGMT_PROTO_STP].str, "Spanning Tree Protocol",LLDP_MGMT_PROTO_STR_MAX_LEN);
		proto_str[LLDP_MGMT_PROTO_STP].str[LLDP_MGMT_PROTO_STR_MAX_LEN-1] =  '\0';

        strncpy(proto_str[LLDP_MGMT_PROTO_RSTP].str,"Rapid Spanning Tree Protocol",LLDP_MGMT_PROTO_STR_MAX_LEN);
		proto_str[LLDP_MGMT_PROTO_RSTP].str[LLDP_MGMT_PROTO_STR_MAX_LEN-1] = '\0';

        strncpy(proto_str[LLDP_MGMT_PROTO_MSTP].str,"Multiple Spanning Tree Protocol",LLDP_MGMT_PROTO_STR_MAX_LEN);
		proto_str[LLDP_MGMT_PROTO_MSTP].str[LLDP_MGMT_PROTO_STR_MAX_LEN-1] =  '\0';

        strncpy(proto_str[LLDP_MGMT_PROTO_GMRP].str,"GARP Multicast	Registration Protocol",LLDP_MGMT_PROTO_STR_MAX_LEN);
		proto_str[LLDP_MGMT_PROTO_GMRP].str[LLDP_MGMT_PROTO_STR_MAX_LEN-1] =  '\0';

        strncpy(proto_str[LLDP_MGMT_PROTO_MMRP].str, "MRP Multicast Registration Protocol",LLDP_MGMT_PROTO_STR_MAX_LEN);
		proto_str[LLDP_MGMT_PROTO_MMRP].str[LLDP_MGMT_PROTO_STR_MAX_LEN-1] =  '\0';

        strncpy(proto_str[LLDP_MGMT_PROTO_GVRP].str, "GARP VLAN Registration Protocol",LLDP_MGMT_PROTO_STR_MAX_LEN);
		proto_str[LLDP_MGMT_PROTO_GVRP].str[LLDP_MGMT_PROTO_STR_MAX_LEN-1] =  '\0';

        strncpy(proto_str[LLDP_MGMT_PROTO_MVRP].str, "MRP VLAN Registration Protocol",LLDP_MGMT_PROTO_STR_MAX_LEN);
		proto_str[LLDP_MGMT_PROTO_MVRP].str[LLDP_MGMT_PROTO_STR_MAX_LEN-1] =  '\0';

        strncpy(proto_str[LLDP_MGMT_PROTO_LACP].str, "Link Aggregation",LLDP_MGMT_PROTO_STR_MAX_LEN);
		proto_str[LLDP_MGMT_PROTO_LACP].str[LLDP_MGMT_PROTO_STR_MAX_LEN-1] =  '\0';
        
        strncpy(proto_str[LLDP_MGMT_PROTO_DOT1X].str,"Port Authentication",LLDP_MGMT_PROTO_STR_MAX_LEN);
		proto_str[LLDP_MGMT_PROTO_DOT1X].str[LLDP_MGMT_PROTO_STR_MAX_LEN-1] =  '\0';

        strncpy(proto_str[LLDP_MGMT_PROTO_EFM_OAM].str, "Ethernet OAM",LLDP_MGMT_PROTO_STR_MAX_LEN);
		proto_str[LLDP_MGMT_PROTO_EFM_OAM].str[LLDP_MGMT_PROTO_STR_MAX_LEN-1] =  '\0';

        strncpy(proto_str[LLDP_MGMT_PROTO_UNKNOWN].str,"Unknown",LLDP_MGMT_PROTO_STR_MAX_LEN);
		proto_str[LLDP_MGMT_PROTO_UNKNOWN].str[LLDP_MGMT_PROTO_STR_MAX_LEN-1] =  '\0';

        strncpy(proto_str[LLDP_MGMT_PROTO_MAX].str, "Max proto exceeded",LLDP_MGMT_PROTO_STR_MAX_LEN);
		proto_str[LLDP_MGMT_PROTO_MAX].str[LLDP_MGMT_PROTO_STR_MAX_LEN-1] =     '\0';

        init_done = 1;
    }

    if ( (proto_num >= LLDP_MGMT_PROTO_MAX) ||
         (proto_num < 0 ) ) { 
        return proto_str[LLDP_MGMT_PROTO_MAX].str;
    }

    return proto_str[proto_num].str;
}

#define LLDP_MGMT_IF_MAU_AUTONEG_SUPPORTED                  (1 << 0)
#define LLDP_MGMT_IF_MAU_AUTONEG_ENABLED                    (1 << 1)

#define LLDP_MGMT_ONM_AGGREGATION_CAPABLE                       (1 << 0)
#define LLDP_MGMT_ONM_AGGREGATION_ENABLE                        (1 << 1)

#define LLDP_MGMT_PPVID_SUPPORTED                           (1 << 1)
#define LLDP_MGMT_PPVID_ENABLED                             (1 << 2)


#define LLDP_MGMT_REMOTE_PP_VLAN_ID_VALID         0x00002000
#define LLDP_MGMT_REMOTE_PROTOCOL_VALID           0x00004000
#define LLDP_MGMT_REMOTE_AUTONEGO_SUPPORT_VALID   0x00008000
#define LLDP_MGMT_REMOTE_AUTONEGO_CAP_VALID       0x00010000

#define LLDP_PLUGIN_LOCAL_VERSION   0
#define LLDP_PLUGIN_GLOBAL_VERSION  0

typedef struct lldp_mgmt_show_nbr_detail_intf_ { 
    char        if_name[LLDP_IF_STR_MAX_LEN + 4];    
    u_char      mac_addr [LLDP_MGMT_ETHER_ADDR_LEN + 2];
    u_int32_t   ifindex;
    u_int32_t   frames_out_total;
    u_int32_t   frames_in_total;
    
    u_int32_t	remote_info_valid;
    u_int32_t   remote_dcbx_valid;
    u_char      remote_mac_addr [LLDP_MGMT_ETHER_ADDR_LEN + 2];
    lldp_mgmt_composite_data_t remote_chassis_id;
    lldp_mgmt_composite_data_t remote_port_id;
    u_int16_t   rx_ttl;
    u_int16_t   remaining_life;
    u_char      remote_descr[LLDP_DESCR_MAX_LEN + 2];
    u_int32_t   remote_if_number;
	
    u_int16_t   remote_port_vlan_id;
    u_int8_t    remote_if_numbering;
    u_int8_t    remote_ppvid_flag;
	
    u_int16_t   remote_pp_vlan_id;
    u_int16_t   remote_protocol;

    u_int8_t    remote_link_aggr_status;
    u_int8_t    remote_autonego_support;
    u_int16_t   remote_sys_cap;

    u_int16_t   remote_sys_cap_enabled;
    u_int16_t   remote_autonego_cap;

    u_int16_t   remote_oper_mau_type;
    u_int16_t   remote_max_frame_size;

    u_int32_t   remote_link_aggr_id;
    u_char      remote_sys_name[LLDP_NAME_MAX_LEN + 2];
    u_char      remote_sys_descr[LLDP_DESCR_MAX_LEN + 2];

    lldp_mgmt_composite_data_t remote_mgmt_addr;

    int                 remote_oid_length;
    u_char              remote_oid[LLDP_MGMT_MAX_OID_LEN];

    /* The following may be required in future     
    u_int32_t           remote_index;
    u_int32_t           remote_unknown_tlv_type;
    u_char              remote_unknown_tlv_info[UNKNOWN_RMT_TLV_LEN + 1];
    */

    /* Brocade Extension */
    u_int16_t           remote_link_prim;
    u_int16_t           remote_link_brcd;

    /* DCB eXchange */
    u_int8_t            remote_dcbx_subtype;
    u_int8_t            dcbx_subtype;
    u_int16_t           resvd1;
    lldp_mgmt_dcbx_t    remote_dcbx;
    

    u_int16_t           features_present;
    u_int16_t           features_changed;
    lldp_mgmt_vlan_list_t   remote_vlan_list;
    u_int32_t           resvd2[2];
} PACKED lldp_mgmt_show_nbr_detail_intf_t;

typedef struct lldp_mgmt_show_nbr_deatil_resp_ { 
    lldp_mgmt_err_code_t    ret_code;
    u_int8_t                ismore;
    u_int32_t               len;
    u_int32_t               num_records;
    lldp_mgmt_show_nbr_detail_intf_t  intf_entries[1];
} PACKED lldp_mgmt_show_nbr_detail_resp_t;



#define IS_CMDCODE_SET(cmdcode, flag) (cmdcode & flag)

/* LLDP Global Command Codes */
#define        LLDP_GLOBAL_CONF_DESC                     1<<0
#define        LLDP_GLOBAL_CONF_HELLO                    1<<1    
#define        LLDP_GLOBAL_CONF_MODE                     1<<2    
#define        LLDP_GLOBAL_CONF_MULTIPLIER               1<<3
#define        LLDP_GLOBAL_CONF_DCBXFCOEAPPTLV           1<<4
#define        LLDP_GLOBAL_CONF_DCBXFCOELOGICALLINKTLV   1<<5
#define        LLDP_GLOBAL_CONF_DCBXTLV					 1<<6
#define        LLDP_GLOBAL_CONF_DOT1XTLV                 1<<7
#define        LLDP_GLOBAL_CONF_DOT3XTLV                 1<<8
#define        LLDP_GLOBAL_CONF_MGMTADDRTLV              1<<9
#define        LLDP_GLOBAL_CONF_PORTDESCRTLV             1<<10
#define        LLDP_GLOBAL_CONF_SYSCAPTLV                1<<11
#define        LLDP_GLOBAL_CONF_SYSDESCRTLV              1<<12
#define        LLDP_GLOBAL_CONF_SYSNAMETLV               1<<13
#define        LLDP_GLOBAL_CONF_SYSNAME                  1<<14
#define        LLDP_GLOBAL_CONF_SYSDESCR                 1<<15 
#define        LLDP_GLOBAL_CONF_DISABLE                  1<<16
#define        LLDP_GLOBAL_CONF_LLDP					 1<<17
#define        LLDP_GLOBAL_CONF_NO_LLDP					 1<<18
#define        LLDP_GLOBAL_CONF_DCBXISCSIAPPTLV          1<<19
#define        LLDP_GLOBAL_CONF_ISCSIPRIORITY            1<<20
#define        LLDP_GLOBAL_AND_PROFILE_COMMON_CONF 							\
				(LLDP_GLOBAL_CONF_DESC| LLDP_GLOBAL_CONF_HELLO|				\
				 LLDP_GLOBAL_CONF_MODE| LLDP_GLOBAL_CONF_MULTIPLIER|		\
				 LLDP_GLOBAL_CONF_DCBXFCOEAPPTLV|							\
				 LLDP_GLOBAL_CONF_DCBXFCOELOGICALLINKTLV|					\
				 LLDP_GLOBAL_CONF_DCBXTLV| LLDP_GLOBAL_CONF_DOT1XTLV|		\
				 LLDP_GLOBAL_CONF_DOT3XTLV| LLDP_GLOBAL_CONF_MGMTADDRTLV|	\
		         LLDP_GLOBAL_CONF_PORTDESCRTLV| LLDP_GLOBAL_CONF_SYSCAPTLV| \
				 LLDP_GLOBAL_CONF_SYSDESCRTLV| LLDP_GLOBAL_CONF_SYSNAMETLV|	\
				 LLDP_GLOBAL_CONF_DCBXISCSIAPPTLV)

// Profile level cmds

#define        LLDP_GLOBAL_CONF_PROFILE                  		 1<<0
#define        LLDP_GLOBAL_PROFILE_CONF_DESC                     1<<1
#define        LLDP_GLOBAL_PROFILE_CONF_HELLO                    1<<2    
#define        LLDP_GLOBAL_PROFILE_CONF_MODE                     1<<3    
#define        LLDP_GLOBAL_PROFILE_CONF_MULTIPLIER               1<<4
#define        LLDP_GLOBAL_PROFILE_CONF_DCBXFCOEAPPTLV           1<<5
#define        LLDP_GLOBAL_PROFILE_CONF_DCBXFCOELOGICALLINKTLV   1<<6
#define        LLDP_GLOBAL_PROFILE_CONF_DCBXTLV					 1<<7
#define        LLDP_GLOBAL_PROFILE_CONF_DOT1XTLV                 1<<8
#define        LLDP_GLOBAL_PROFILE_CONF_DOT3XTLV                 1<<9
#define        LLDP_GLOBAL_PROFILE_CONF_MGMTADDRTLV              1<<10
#define        LLDP_GLOBAL_PROFILE_CONF_PORTDESCRTLV             1<<11
#define        LLDP_GLOBAL_PROFILE_CONF_SYSCAPTLV                1<<12
#define        LLDP_GLOBAL_PROFILE_CONF_SYSDESCRTLV              1<<13
#define        LLDP_GLOBAL_PROFILE_CONF_SYSNAMETLV               1<<14
#define        LLDP_GLOBAL_PROFILE_CONF_OBJID               	 1<<15
#define        LLDP_GLOBAL_PROFILE_CONF_DCBXISCSIAPPTLV          1<<16
#define        LLDP_GLOBAL_PROFILE_CONF_ISCSIPRIORITY            1<<17

// Intf Level cmds
#define		   LLDP_INTF_CONF_DISABLE							 1<<0
#define		   LLDP_INTF_CONF_DCBXVERSION						 1<<1
#define		   LLDP_INTF_CONF_PROFILE							 1<<2
#define        LLDP_INTF_CONF_ISCSIPRIORITY                      1<<3
#define        LLDP_INTF_CONF_DESCRIPTION                        1<<4
#define        LLDP_INTF_CONF_CEE_ON_OFF                         1<<5
// Internel cmds
#define		   LLDP_NSM_INTERNAL_DEL_PROFILE					 1<<0
#define		   LLDP_NSM_INTERNAL_NO_LLDP						 1<<1

/* LLDP Message Types */
typedef enum
	{
		LLDP_CREATE_CONF,
		LLDP_UPDATE_CONF,
		LLDP_DELETE_CONF
	}LldpMsgType;

/* LLDP Mode Types */
typedef enum
	{
		LLDP_MODE_RXTX,
		LLDP_MODE_RX,
		LLDP_MODE_TX
	}LldpMode;

/* LLDP DCBX Version Types */
typedef enum
	{
		LLDP_DCBX_VERSION_AUTO,
		LLDP_MODE_CEE,
		LLDP_MODE_PRE_CEE
	} LldpDcbxVersion;

typedef struct lldp_global_conf_ {
		unsigned int mgmt_flags;
		unsigned int cmd_code;
		unsigned int msg_type;
		char description[64];
		unsigned int hello;
		unsigned int mode;    
		unsigned int multiplier;
        unsigned int iscsi_priority_bits;
        u_int16_t dcbx_iscsiapp_tlv;
		u_int16_t dcbx_fcoeapp_tlv;
		u_int16_t dcbx_fcoelogicallink_tlv;
		u_int16_t dcbx_tlv;
		u_int16_t dot1x_tlv;
		u_int16_t dot3x_tlv;
		u_int16_t mgmtaddr_tlv;
		u_int16_t portdescr_tlv;
		u_int16_t syscap_tlv;
		u_int16_t sysdescr_tlv;
		u_int16_t sysname_tlv;
		char sysname[64];
		char sysdescr[64];
		unsigned int disable;
} lldp_mgmt_global_conf_t;

typedef struct lldp_global_profile_conf_ {
		unsigned int mgmt_flags;
		unsigned int cmd_code;
		unsigned int msg_type;
		char profile_name[64];
		char description[64];
		unsigned int hello;
		unsigned int mode;    
		unsigned int multiplier;
        u_int16_t dcbx_iscsiapp_tlv;
		u_int16_t dcbx_fcoeapp_tlv;
		u_int16_t dcbx_fcoelogicallink_tlv;
		u_int16_t dcbx_tlv;
		u_int16_t dot1x_tlv;
		u_int16_t dot3x_tlv;
		u_int16_t mgmtaddr_tlv;
		u_int16_t portdescr_tlv;
		u_int16_t syscap_tlv;
		u_int16_t sysdescr_tlv;
		u_int16_t sysname_tlv;
}lldp_mgmt_global_profile_conf_t;

typedef struct lldp_intf_conf_ {
		unsigned int mgmt_flags;
		unsigned int cmd_code;
		unsigned int msg_type;
        unsigned int iscsi_priority_bits;
		char id[64];
		char if_name[64];
		unsigned int if_index;
		u_int16_t disable;
		u_int16_t dcbx_version;
		char profile_name[64];
		char description[64];
		unsigned long if_type;
		lldp_cee_on_off_t lldp_cee_type;
}lldp_mgmt_intf_conf_t;

typedef struct lldp_mgmt_conf_req_ { 
	u_int32_t mgmt_flags;
    u_int32_t cmd_code;
	u_int32_t msg_type;

    union { 
        lldp_mgmt_global_conf_t		global_conf;
        lldp_mgmt_global_profile_conf_t profile_conf;
		lldp_mgmt_intf_conf_t intf_conf;
    } entry;
} lldp_mgmt_conf_req_t;

typedef struct lldp_fcoe_priority_conf_ {
    u_int32_t   cmdcode;
    u_int32_t   priority_bitmap;
} lldp_fcoe_priority_conf_t;

typedef struct lldp_fsb_mode_ {
    int     fsb_enable;
} lldp_fsb_mode_t;

typedef union lldp_mgmt_buffer_num_ { 
    struct { 
        u_int16_t   mapped_id;
        //This is typically lldp_mgmt_show_buf_t for show commands
        u_int16_t   tag;     
    } buff;
    u_int32_t   num;   
} PACKED lldp_mgmt_buffer_num_t;

static inline u_int16_t 
lldp_get_buffer_tag (u_int32_t  buff_num) 
{
    return (((lldp_mgmt_buffer_num_t*) &buff_num)->buff.tag);
}


static inline u_int16_t 
lldp_get_buffer_mapped_id (u_int32_t  buff_num) 
{
    return (((lldp_mgmt_buffer_num_t*) &buff_num)->buff.mapped_id);
}

#define LLDP_DEBUG_CLI_TKN_STR_LEN_MAX        32

#define LLDP_DEBUG_CLI_TKN_STR_DEBUG          "debug"
#define LLDP_DEBUG_CLI_TKN_STR_LLDP           "lldp"
#define LLDP_DEBUG_CLI_TKN_STR_DUMP           "dump"
#define LLDP_DEBUG_CLI_TKN_STR_PACKET         "packet"
#define LLDP_DEBUG_CLI_TKN_STR_ALL            "all"
#define LLDP_DEBUG_CLI_TKN_STR_DETAIL         "detail"
#define LLDP_DEBUG_CLI_TKN_STR_BOTH           "both"
#define LLDP_DEBUG_CLI_TKN_STR_TX             "tx"
#define LLDP_DEBUG_CLI_TKN_STR_RX             "rx"
#define LLDP_DEBUG_CLI_TKN_STR_NO             "no"
#define LLDP_DEBUG_CLI_TKN_STR_TE_NAME        "tengigabitethernet"
#define LLDP_DEBUG_CLI_TKN_STR_GI_NAME        "gigabitethernet"
#define LLDP_DEBUG_CLI_TKN_STR_FO_NAME        "fortygigabitethernet"
#define LLDP_DEBUG_CLI_TKN_INTF_DELIM         "/"

#define LLDP_DEBUG_REQ_FLAG_RX             (1 << 0)
#define LLDP_DEBUG_REQ_FLAG_TX             (1 << 1)
#define LLDP_DEBUG_REQ_FLAG_DUMP		   (1 << 2)
#define LLDP_DEBUG_REQ_FLAG_DETAIL         (1 << 3)

typedef enum {
    LLDP_DEBUG_CMD_CODE_INVALID = 0,
    LLDP_DEBUG_CMD_CODE_DUMP_ALL,
    LLDP_DEBUG_CMD_CODE_DUMP_INTF,
    LLDP_DEBUG_CMD_CODE_DUMP_INTF_GI,
    LLDP_DEBUG_CMD_CODE_PACKET_ALL,
    LLDP_DEBUG_CMD_CODE_PACKET_INTF,
    LLDP_DEBUG_CMD_CODE_PACKET_INTF_GI,
    LLDP_DEBUG_CMD_CODE_SHOW_STATUS,
    LLDP_DEBUG_CMD_CODE_DUMP_INTF_FO,
    LLDP_DEBUG_CMD_CODE_PACKET_INTF_FO
} lldp_debug_cmd_t;

typedef enum lldp_debug_buf_ { 
    LLDP_DEBUG_BUF_UNKNOWN = 0,
    LLDP_DEBUG_BUF_DUMP_ALL, 
    LLDP_DEBUG_BUF_DUMP_INTF,
    LLDP_DEBUG_BUF_PKT,
    LLDP_DEBUG_BUF_SHOW_STATUS
} lldp_debug_buf_t;

typedef struct lldp_debug_cmd_req_ {
    lldp_debug_cmd_t cmd_code;
    lldp_debug_int_req_t intf;
    u_int32_t   debug_flags;
    u_int32_t   last_ifindex;
    u_int32_t   max_records;
} lldp_debug_cmd_req_t;

typedef struct lldp_debug_dump_resp_entry_ {
    char        if_name[LLDP_IF_STR_MAX_LEN + 1];
    char        profile_name[LLDP_PROFILE_MAX_LEN +1];
    u_int32_t   ifindex;
    u_int8_t    admin_status;
    u_int8_t	fcoe_priority_bits;
    u_int8_t	fcoe_priority_configured;
    u_int8_t	iscsi_priority_bits;
    u_int8_t	iscsi_priority_configured;
    u_int8_t    link_property;
    u_int16_t   sub_tlvs_tx_enable;
    u_int32_t   basic_tlvs_tx_enable;
    u_int8_t    dcbx_subtype;
    u_int8_t    auto_sense;
    u_int8_t    ets_numTCSupported;
    u_int8_t    ets_percentage[8];
    u_int8_t    ets_pri_map[8];
    u_int8_t    dcbx_pfc_admin_map;
    u_int8_t	dcbx_pfc_numTCPFCSupported;
    u_int8_t	dcbx_user_priority_map;
    u_int8_t    dcbx_iscsi_priority_map;
    u_int8_t   	lan_logical_link_status;
    u_int8_t   	fcoe_logical_link_status;
    u_int32_t   msg_tx_hold;
    u_int32_t   msg_tx_interval;
    u_int8_t    onm_if_mac_addr [LLDP_MGMT_ETHER_ADDR_LEN];
    u_int8_t    mgmt_port_mac [LLDP_MGMT_ETHER_ADDR_LEN];
} lldp_debug_dump_resp_entry_t;

typedef struct lldp_debug_cmd_resp_ {
    lldp_mgmt_err_code_t    ret_code;
    u_int8_t                ismore;
    u_int32_t               num_records;
    lldp_debug_dump_resp_entry_t dump_entry[1];
} lldp_debug_cmd_resp_t;

typedef struct lldp_debug_status_resp_entry_ {
    char        if_name[LLDP_IF_STR_MAX_LEN + 1];
    u_int32_t   ifindex;
    u_int32_t   debug_flags;
} lldp_debug_status_resp_entry_t;

typedef struct lldp_debug_status_resp_ {
    lldp_mgmt_err_code_t    ret_code;
    u_int8_t                ismore;
    u_int32_t               num_records;
    lldp_debug_status_resp_entry_t status_entry[1];
} lldp_debug_status_resp_t;

// CDP

#define CDP_DEFAULT_PROTOCOL_STATUS                         false

// CDP Global cmds
#define CDP_GLOBAL_CONF_PROTO_STATUS                        1<<0

typedef struct cdp_global_conf_ {
		unsigned int mgmt_flags;
		unsigned int cmd_code;
		unsigned int msg_type;
		unsigned int protoEnable;
} cdp_mgmt_global_conf_t;

typedef struct cdp_mgmt_conf_req_ { 
	u_int32_t mgmt_flags;
    u_int32_t cmd_code;
	u_int32_t msg_type;

    union { 
        cdp_mgmt_global_conf_t		global_conf;
    } entry;
} cdp_mgmt_conf_req_t;


#define LLDP_ADVERTISE_CLI_TKN_STR_ADVERTISE                    "advertise"
#define LLDP_ADVERTISE_CLI_TKN_STR_DCBX_FCOE_APP_TLV            "dcbx-fcoe-app-tlv"
#define LLDP_ADVERTISE_CLI_TKN_STR_DCBX_FCOE_LOGICAL_LINK_TLV   "dcbx-fcoe-logical-link-tlv"
#define LLDP_ADVERTISE_CLI_TKN_STR_DCBX_ISCSI_APP_TLV           "dcbx-iscsi-app-tlv"
#define LLDP_ADVERTISE_CLI_TKN_STR_DCBX_TLV                     "dcbx-tlv"
#define LLDP_ADVERTISE_CLI_TKN_STR_DOT1_TLV                     "dot1-tlv"
#define LLDP_ADVERTISE_CLI_TKN_STR_DOT3_TLV                     "dot3-tlv"
#define LLDP_ADVERTISE_CLI_TKN_STR_OPTIONAL_TLV                 "optional-tlv"
#define LLDP_ADVERTISE_CLI_TKN_STR_OPTIONAL_MGMT_ADDR_TLV       "management-address"
#define LLDP_ADVERTISE_CLI_TKN_STR_OPTIONAL_PORT_DESC_TLV       "port-description"
#define LLDP_ADVERTISE_CLI_TKN_STR_OPTIONAL_SYS_CAPB_TLV        "system-capabilities"
#define LLDP_ADVERTISE_CLI_TKN_STR_OPTIONAL_SYS_DESC_TLV        "system-description"
#define LLDP_ADVERTISE_CLI_TKN_STR_OPTIONAL_SYS_NAME_TLV        "system-name"
#define LLDP_CLI_TKN_STR_HELLO                                  "hello"
#define LLDP_CLI_TKN_STR_MODE                                   "mode"
#define LLDP_CLI_TKN_STR_MULTIPLIER                             "multiplier"
#define LLDP_CLI_TKN_STR_DESCRIPTION                            "description"

#endif  //LLDPDCMDEF_H

