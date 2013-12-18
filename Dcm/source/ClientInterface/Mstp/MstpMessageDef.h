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

#ifndef __MSTP_MESSAGE_DEF_H__
#define __MSTP_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"

#define MSTP_DCM_MAX_STR		64
#define MSTP_DCM_IF_NAME_LEN	20

#define MSTP_DCM_BRIDGE_STP			0
#define MSTP_DCM_BRIDGE_RSTP		1
#define MSTP_DCM_BRIDGE_MSTP		2
#define MSTP_DCM_BRIDGE_PVSTP		3
#define MSTP_DCM_BRIDGE_RPVSTP		4
#define MSTP_DCM_BRIDGE_NONE		1000

#define MSTP_DCM_CLEAR_COUNTER		"counter"
#define MSTP_DCM_CLEAR_PROTOCOL		"detected-protocols"

#define MSTP_DCM_CLIENT_NAME		"mstp"

#define MSP_DCM_BRIDGE_PRIORITY_MULTIPLIER		4096
#define MSTP_DCM_PORT_PRIORITY_MULTIPLIER		16

#define REQ_TYPE_GET        0
#define REQ_TYPE_GETNEXT    1


typedef	enum
{
	MSTPMACREDUCTIONENABLE,
	MSTPMACREDUCTIONSTANDARD
}MstpMacReduction;

typedef	enum
{
	MSTPPOPATHCOSTCUSTOM,
	MSTPPOPATHCOSTSTANDARD
}MstpPoPathcost;

typedef	enum
{
	MSTPCISCOINTEROPENABLE,
	MSTPCISCOINTEROPDISABLE
}MstpCiscoInterOp;

typedef	enum
{
	MSTPPORTNOGUARDROOT = 0,
	MSTPPORTGUARDROOT
}MstpPortGuardRootEnum;

typedef	enum
{
	MSTPPORTLINKTYPEP2P = 0,
	MSTPPORTLINKTYPESHARED 
}MstpPortLinkTypeEnum;

typedef enum
{
	MSTPPORTPVSTBPDUMACBRCD = 0,
	MSTPPORTPVSTBPDUMACCISCO
} MstpPortPvstBpDuMac;

typedef enum
{
	MSTPNETCONFMSTPTYPESTP = 1,
	MSTPNETCONFMSTPTYPERSTP,
	MSTPNETCONFMSTPTYPEMSTP,
	MSTPNETCONFMSTPTYPEPVSTP,
	MSTPNETCONFMSTPTYPERPVSTP,
	MSTPNETCONFMSTPTYPENONE
}MstpNetconfMstpType;

typedef enum
{
	MSTPNETCONFIFROLEERROR = 1,
	MSTPNETCONFIFROLEDESIGNATED,
	MSTPNETCONFIFROLEROOT,
	MSTPNETCONFIFROLEALTERNATE,
	MSTPNETCONFIFROLEBACKUP,
	MSTPNETCONFIFROLEDISABLED,
	MSTPNETCONFIFROLEMASTER
}MstpNetconfIfRole;

typedef enum
{
	MSTPNETCONFIFSTATUSERROR = 1,
	MSTPNETCONFIFSTATUSDISABLED,
	MSTPNETCONFIFSTATUSBLOCKING,
	MSTPNETCONFIFSTATUSLISTENING,
	MSTPNETCONFIFSTATUSLEARNING,
	MSTPNETCONFIFSTATUSFORWARDING,
	MSTPNETCONFIFSTATUSDISCARDING
}MstpNetconfIfStatus;

typedef enum
{
	MSTPNETCONFIFLINKTYPEP2P = 1,
	MSTPNETCONFIFLINKTYPESHARED
}MstpNetconfIfLinkType;

typedef enum
{
	MSTPNETCONFIFISNOTBOUNDARY = 1,
	MSTPNETCONFIFISBOUNDARY
}MstpNetconfIfBoundary;

typedef enum
{
	MSTPNETCONFIFISNOTEDGE = 1,
	MSTPNETCONFIFISEDGE
}MstpNetconfIfEdge;

typedef enum
{
	MSTP_TAG_ID_SHOW_BPDU = 1
} MstpShowTagId;


#define MSTP_DCM_DEFAULT_HELLO_TIME			2
#define MSTP_DCM_DEFAULT_TX_HOLDCOUNT		6		
#define MSTP_DCM_DEFAULT_PRIORITY			32768
#define MSTP_DCM_DEFAULT_DESCRIPTION		""
#define MSTP_DCM_DEFAULT_REGION				"\0"
#define MSTP_DCM_DEFAULT_REVISION			0
#define MSTP_DCM_DEFAULT_CISCO_INTEROP		MSTPCISCOINTEROPDISABLE
#define MSTP_DCM_DEFAULT_MAX_HOP			20
#define MSTP_DCM_DEFAULT_ERR_INTERVAL		300
#define MSTP_DCM_DEFAULT_FWD_DELAY			15
#define MSTP_DCM_DEFAULT_MAX_AGE			20
#define MSTP_DCM_DEFAULT_AGING_TIME			300
#define MSTP_DCM_DEFAULT_PATHCOST			MSTPPOPATHCOSTSTANDARD
#define MSTP_DCM_DEFAULT_ERR_TIMEOUT		false
#define MSTP_DCM_DEFUALT_PROTO_STATUS		false
#define MSTP_DCM_DEFAULT_BPDU_MAC			false
#define MSTP_DCM_MAC_REDUCTION				MSTPMACREDUCTIONENABLE
#define MSTP_DCM_DEFAULT_PORT_PRIORITY			128

/* 0 means to use default cost based on bandwidth */
#define MSTP_DCM_INVALID_COST				0
#define MSTP_DCM_INVALID_FWD_DELAY			65536
#define MSTP_DCM_INVALID_MAX_AGE			65536
#define MSTP_DCM_INVALID_HELLO_TIME			65536
#define MSTP_DCM_INVALID_PRIORITY			65536
#define MSTP_DCM_INVALID_AGING_TIME			65536
#define MSTP_DCM_INVALID_GUARDROOT			65536
#define MSTP_DCM_INVALID_RES_TCN			65536
#define MSTP_DCM_INVALID_RES_ROLE			65536

#define MSTP_DCM_DEFAULT_PORT_GUARDROOT     false
#define MSTP_DCM_DEFAULT_PORT_RES_TCN       false
#define MSTP_DCM_DEFAULT_PORT_RES_ROLE      false
#define MSTP_DCM_DEFAULT_PORT_COST			2000


#define MSTP_DCM_VLAN_ALL					0

#define MSTP_DCM_MAX_VLAN_BRIDGE	128
#define MSTP_DCM_MIN_MSTP_INST_ID	1
#define MSTP_DCM_MAX_MSTP_INST_ID	32

#define MSTP_PORT_ID_COUNT_BUFTAG   10000
#define MSTP_PORT_ID_ARRAY_BUFTAG   10001

// Default Bridge Xstp Value

typedef enum {
	/* Bridge configure */
	MSTP_MSG_BR_STP,
	MSTP_MSG_BR_RSTP,
	MSTP_MSG_BR_MSTP,
	MSTP_MSG_BR_PVSTP,
	MSTP_MSG_BR_RPVSTP,
	MSTP_MSG_BR_CREATE,
	MSTP_MSG_BR_NONE,

    MSTP_MSG_SET_PORT_ID = 80,
	MSTP_MSG_SET_BR_MAC,    // Manually set the VCS bridge mac

	MSTP_MSG_BR_PROTO_ENABLE = 100,
	MSTP_MSG_BR_PRIORITY,
	MSTP_MSG_BR_CISCO_INTEROP,
	MSTP_MSG_BR_DESCRITION,
	MSTP_MSG_BR_ERR_TIMEOUT,
	MSTP_MSG_BR_ERR_INTERVAL,
	MSTP_MSG_BR_FWD_DELAY,
	MSTP_MSG_BR_HELLO_TIME,
	MSTP_MSG_BR_MAX_AGE,
	MSTP_MSG_BR_MAX_HOP,
	MSTP_MSG_BR_REGION,
	MSTP_MSG_BR_REVISION,
	MSTP_MSG_BR_PATHCOST_STDANDARD,
	MSTP_MSG_BR_TRANS_HOLDCOUNT,
	MSTP_MSG_BR_MAC_REDUCTION,
	MSTP_MSG_BR_BPDU_MAC,
	MSTP_MSG_BR_MAC_AGING_TIME,

	MSTP_MSG_BR_NO_PROTO_ENABLE = 200,
	MSTP_MSG_BR_NO_PRIORITY,
	MSTP_MSG_BR_NO_CISCO_INTEROP,
	MSTP_MSG_BR_NO_DESCRITION,
	MSTP_MSG_BR_NO_ERR_TIMEOUT,
	MSTP_MSG_BR_NO_ERR_INTERVAL,
	MSTP_MSG_BR_NO_FWD_DELAY,
	MSTP_MSG_BR_NO_HELLO_TIME,
	MSTP_MSG_BR_NO_MAX_AGE,
	MSTP_MSG_BR_NO_MAX_HOP,
	MSTP_MSG_BR_NO_REGION,
	MSTP_MSG_BR_NO_REVISION,
	MSTP_MSG_BR_NO_PATHCOST_STDANDARD,
	MSTP_MSG_BR_NO_TRANS_HOLDCOUNT,
	MSTP_MSG_BR_NO_MAC_REDUCTION,
	MSTP_MSG_BR_NO_BPDU_MAC,

	/* Bridge vlan configure */
	MSTP_MSG_BR_VLAN_HELLO_TIME = 300,
	MSTP_MSG_BR_VLAN_MAX_AGE,
	MSTP_MSG_BR_VLAN_PRIORITY,
	MSTP_MSG_BR_VLAN_FWD_DELAY,
	MSTP_MSG_BR_VLAN_AGING_TIME,

    MSTP_MSG_BR_VLAN_NO_HELLO_TIME = 400,
    MSTP_MSG_BR_VLAN_NO_MAX_AGE,
    MSTP_MSG_BR_VLAN_NO_PRIORITY,
    MSTP_MSG_BR_VLAN_NO_FWD_DELAY,
    MSTP_MSG_BR_VLAN_NO_AGING_TIME,

	/* Bridge instance  configure */
	MSTP_MSG_BR_INST_ADD = 500,
	MSTP_MSG_BR_INST_ADD_VLAN,
	MSTP_MSG_BR_INST_REMOVE_VLAN,
	MSTP_MSG_BR_INST_PRIORITY,
	MSTP_MSG_BR_INST_NO_PRIORITY,
	MSTP_MSG_BR_NO_INST,

	/* Port  configure */
    MSTP_MSG_IF_AUTO_EDGE = 600,
    MSTP_MSG_IF_PRIORITY,
    MSTP_MSG_IF_COST,
    MSTP_MSG_IF_EDGE,
    MSTP_MSG_IF_EDGE_FILTER,
    MSTP_MSG_IF_EDGE_GUARD,
    MSTP_MSG_IF_PORTFAST,
    MSTP_MSG_IF_PORTFAST_FILTER,
    MSTP_MSG_IF_PORTFAST_GUARD,
    MSTP_MSG_IF_ROOT_GUARD,
    MSTP_MSG_IF_HELLO_TIME,
    MSTP_MSG_IF_LINK_SHARE,
    MSTP_MSG_IF_RESTRICTED_ROLE,
    MSTP_MSG_IF_RESTRICTED_TCN,
    MSTP_MSG_IF_PROTO_ENABLE,
    MSTP_MSG_IF_PVST_MAC_CHANGE,

	MSTP_MSG_IF_NO_AUTO_EDGE = 700,
	MSTP_MSG_IF_NO_PRIORITY,
	MSTP_MSG_IF_NO_COST,
	MSTP_MSG_IF_NO_EDGE,
	MSTP_MSG_IF_NO_EDGE_FILTER,
	MSTP_MSG_IF_NO_EDGE_GUARD,
	MSTP_MSG_IF_NO_ROOT_GUARD,
	MSTP_MSG_IF_NO_PORTFAST,
	MSTP_MSG_IF_NO_PORTFAST_FILTER,
	MSTP_MSG_IF_NO_PORTFAST_GUARD,
	MSTP_MSG_IF_NO_HELLO_TIME,
	MSTP_MSG_IF_NO_LINK_SHARE,
	MSTP_MSG_IF_NO_RESTRICTED_ROLE,
	MSTP_MSG_IF_NO_RESTRICTED_TCN,
	MSTP_MSG_IF_NO_PROTO_ENABLE,
    MSTP_MSG_IF_NO_PVST_MAC_CHANGE,

	/* Port vlan configure */
    MSTP_MSG_IF_VLAN_COST = 800,
    MSTP_MSG_IF_VLAN_PRIORITY,
    MSTP_MSG_IF_VLAN_RESTRICTED_ROLE,
    MSTP_MSG_IF_VLAN_RESTRICTED_TCN,
	MSTP_MSG_IF_VLAN_GUARDROOT,
    MSTP_MSG_VLAN_STP_DISABLE,

    MSTP_MSG_IF_VLAN_NO_COST = 900,
    MSTP_MSG_IF_VLAN_NO_PRIORITY,
    MSTP_MSG_IF_VLAN_NO_RESTRICTED_ROLE,
    MSTP_MSG_IF_VLAN_NO_RESTRICTED_TCN,
    MSTP_MSG_IF_VLAN_NO_GUARDROOT,
    MSTP_MSG_VLAN_NO_STP_DISABLE,


	/* Port instance configure */
    MSTP_MSG_IF_INST_COST = 1000,
    MSTP_MSG_IF_INST_PRIORITY,
    MSTP_MSG_IF_INST_RESTRICTED_ROLE,
    MSTP_MSG_IF_INST_RESTRICTED_TCN,
    MSTP_MSG_IF_INST_GUARDROOT,

	MSTP_MSG_IF_INST_NO_COST = 1100,
	MSTP_MSG_IF_INST_NO_PRIORITY,
	MSTP_MSG_IF_INST_NO_RESTRICTED_ROLE,
	MSTP_MSG_IF_INST_NO_RESTRICTED_TCN,
	MSTP_MSG_IF_INST_NO_GUARDROOT,

    /* xSTP SHOW */
    MSTP_SHOW_SPT = 1200,
    MSTP_SHOW_SPT_BRIEF,
    MSTP_SHOW_SPT_IF,
    MSTP_SHOW_MST_BRIEF,
    MSTP_SHOW_MST_DETAIL,
    MSTP_SHOW_MST_DETAIL_IF,
    MSTP_SHOW_MST_INST,
    MSTP_SHOW_MST_INST_BRIEF,
    MSTP_SHOW_MST_INST_IF,
    MSTP_SHOW_MST_IF,
    MSTP_SHOW_PVST,
    MSTP_SHOW_PVST_BRIEF,
    MSTP_SHOW_MST_CFG,
    MSTP_SHOW_AGING_TIME,

    /* xSTP debug */
    MSTP_DEBUG_SPT_ALL = 1300,
    MSTP_NO_DEBUG_SPT_ALL,
    MSTP_DEBUG_BPDU_TX_ALL,
    MSTP_NO_DEBUG_BPDU_TX_ALL,
    MSTP_DEBUG_BPDU_RX_ALL,
    MSTP_NO_DEBUG_BPDU_RX_ALL,
    MSTP_DEBUG_BPDU_TX_IF,
    MSTP_NO_DEBUG_BPDU_TX_IF,
    MSTP_DEBUG_BPDU_RX_IF,
    MSTP_NO_DEBUG_BPDU_RX_IF,
    MSTP_DEBUG_NSM_IF,
    MSTP_NO_DEBUG_NSM_IF,
    MSTP_DEBUG_API_IF,
    MSTP_NO_DEBUG_API_IF,
    MSTP_DEBUG_TMR_IF,
    MSTP_NO_DEBUG_TMR_IF,
    MSTP_DEBUG_CRIT_IF,
    MSTP_NO_DEBUG_CRIT_IF,
    MSTP_DEBUG_KAP_IF,
    MSTP_NO_DEBUG_KAP_IF,
    MSTP_DEBUG_EVENT_IF,
    MSTP_NO_DEBUG_EVENT_IF,
    MSTP_DEBUG_THA_IF,
    MSTP_NO_DEBUG_THA_IF,
    MSTP_DEBUG_PORT_IF,
    MSTP_NO_DEBUG_PORT_IF,
    MSTP_DEBUG_BRG_IF,
    MSTP_NO_DEBUG_BRG_IF,
    MSTP_DEBUG_DIST_IF,
    MSTP_NO_DEBUG_DIST_IF,
    MSTP_DEBUG_MGMT_IF,
    MSTP_NO_DEBUG_MGMT_IF,
    MSTP_DEBUG_PROTO_IF,
    MSTP_NO_DEBUG_PROTO_IF,


	/* Clear command */
	MSTP_MSG_CLEAR_COUNTER = 1400,
	MSTP_MSG_CLEAR_COUNTER_ALL,
	MSTP_MSG_CLEAR_PROTOCOL,
	MSTP_MSG_CLEAR_PROTOCOL_ALL,


	MSTP_MSG_POST_BOOT_BR_XSTP_CONFIG = 1500,
	MSTP_MSG_POST_BOOT_BR_VLAN_CONFIG,
	MSTP_MSG_POST_BOOT_BR_INST_MSTP_CONFIG,
	MSTP_MSG_POST_BOOT_IF_XSTP_CONFIG,
	MSTP_MSG_POST_BOOT_IF_VLAN_CONFIG,
	MSTP_MSG_POST_BOOT_IF_INST_MSTP_CONFIG,
    MSTP_MSG_BR_PORTFAST_BPDUGUARD,
    MSTP_MSG_BR_NO_PORTFAST_BPDUGUARD,

	/* STP Show cmd Local */
	MSTP_SSM_SHOW_STP

} mstp_cmd_code;

typedef enum
{
	/*config bpdu-guard*/
	DCM_SSM_BPDUGUARD_ENABLE = 1600,
	DCM_SSM_BPDUGUARD_NO_ENABLE,
	/*show bpdu-guard*/
	DCM_SSM_SHOW_BPDUGUARD,
	DCM_SSM_SHOW_BPDUGUARD_IF,

    DCM_SSM_SHOW_TAGGED_BPDU,
    DCM_SSM_SHOW_TAGGED_BPDU_IF,

    DCM_SSM_TAGGED_BPDU_ENABLE,
    DCM_SSM_TAGGED_BPDU_NO_ENABLE,
    DCM_SSM_XSTP_POSTBOOT

} SsmLocalUpdatePhyIntfCmdCode;

typedef enum
{
    BPDU_DROP_DISABLED,
    BPDU_DROP_DIRECTION_RX,
    BPDU_DROP_DIRECTION_TX,
    BPDU_DROP_DIRECTION_ALL
} bpduDropDirection;

typedef enum
{
    DEBUG_SEVERITY_LEVEL_NONE,
    DEBUG_SEVERITY_LEVEL_INFO,
    DEBUG_SEVERITY_LEVEL_ALERT,
    DEBUG_SEVERITY_LEVEL_NOTIFY,
    DEBUG_SEVERITY_LEVEL_DEBUG1,
    DEBUG_SEVERITY_LEVEL_DEBUG2,
    DEBUG_SEVERITY_LEVEL_WARNING,
    DEBUG_SEVERITY_LEVEL_ERROR,
    DEBUG_SEVERITY_LEVEL_CRITICAL,
    DEBUG_SEVERITY_LEVEL_EMERGENCY
} debugSeverity;

typedef struct mstp_msg_br_config_ {
	unsigned int	cmd;
	unsigned int	mode;
	unsigned int	priority;
	unsigned int	fwd_delay;
	unsigned int	hello_time;
	unsigned int	max_age;
	unsigned int	max_hop;
	unsigned int	err_timer_interval;
	unsigned int	revision;
	unsigned int	aging_time;
	int			cisco_interop;
	int			mac_reduction;
	int			stp_enable;
	int			err_disable;
	int			po_cost_standard;
	unsigned char	hold_count;
	char		desc[MSTP_DCM_MAX_STR];
	char		region[MSTP_DCM_MAX_STR];
	unsigned char   br_mac[6];
} mstp_msg_br_config;


/* MSTP instance configure */
typedef struct mstp_msg_br_inst_config_ {
	unsigned int	cmd;
	unsigned int	inst;
	unsigned int	priority;
	unsigned int	vlan_num;
	unsigned int	*vid;
} mstp_msg_br_inst_config;

/* MSTP vlan bridge configure */
typedef struct mstp_msg_br_vlan_config_ {
	unsigned int	cmd;
	unsigned int	vid;
	unsigned int	priority;
	unsigned int	fwd_delay;
	unsigned int	hello_time;
	unsigned int	max_age;
	unsigned int	aging_time;
} mstp_msg_br_vlan_config;

/* MSTP port configure */
typedef struct mstp_msg_port_config_ {
	unsigned int	cmd;
	unsigned int	priority;
	unsigned int	hello_time;
	unsigned int	cost;
	unsigned int	if_index;
	unsigned int	inst;
    unsigned int    port_id;
	int			restricted_role;
	int			restricted_tcn;
	int			auto_edge;
	int			port_fast;
	int			edge_port;
	int			bpdu_guard_enable;
	int			bpdu_guard;
	int			bpdu_filter;
	int			guard_root;
	int			link_shared;
	int			bpdu_mac;
    int         tagged_bpdu_enable;
	int			stp_disable;
	unsigned int    if_type;
	unsigned char	name[MSTP_DCM_IF_NAME_LEN];
} mstp_msg_port_config;

/* MSTP port instance/vlan  configure */
typedef struct mstp_msg_port_inst_config_ {
	unsigned int	cmd;
	unsigned int	id;
	unsigned int	cost;
	unsigned int	priority;
	int			guard_root;
	int			restricted_tcn;
	int			restricted_role;
	unsigned int    if_type;
	unsigned char	name[MSTP_DCM_IF_NAME_LEN];
} mstp_msg_port_inst_config, mstp_msg_port_vlan_cfg;

typedef struct mstp_msg_vlan_config_ {
	unsigned int	cmd;
	unsigned int	vid;
} mstp_msg_vlan_config;


typedef struct mstp_msg_port_id_info_ {
    unsigned int    port_id;
    unsigned int    if_type;
    unsigned char   if_name[MSTP_DCM_IF_NAME_LEN];
} mstp_msg_port_id_info;

// Input for MSTP_FPID_SET message..
typedef struct mstp_msg_port_id_config_ {
    unsigned int            count;
    mstp_msg_port_id_info   pid_info[1];
} mstp_msg_port_id_config;


/* MSTP show inputs */
typedef struct mstp_show_input_msg {
	unsigned int	cmd;
	unsigned int	severity;
	unsigned int	type;
	unsigned int	inst;
	char	        if_name[MSTP_DCM_IF_NAME_LEN];
	unsigned int    last_index; // last vlan id or last instance id
} mstp_show_input_msg_t;

typedef union dcm_mstp_bridge_id {
        struct {
	        unsigned char	prio[2];
	        unsigned char	addr[6];
        }struct_prio_addr;
	unsigned char id[8];
} dcm_mstp_bridge_id_t;

/* MSTP show output */
typedef struct mstp_show_if_output_msg {
	unsigned int	inst;
	unsigned char	if_name[MSTP_DCM_IF_NAME_LEN];
	unsigned char	spanning_tree_disable;
	unsigned int	ifindex;
	unsigned int	id;
	unsigned char	role[16];
	unsigned char	state[18];
	unsigned int	external_rpc;
	unsigned int	internal_rpc;
	unsigned int	path_cost;
	unsigned int	priority;
	unsigned int	designated_port_id;
	unsigned int	msti_path_cost;
	unsigned int	msti_priority;
	unsigned int	cist_path_cost;
	unsigned int	designated_port_priority;
	dcm_mstp_bridge_id_t designated_bridge_id;
	unsigned int	forward_transitions;
	unsigned int	hello_time;
	unsigned char	peer_stp_status[32];
	unsigned char	recv_disputed;
	unsigned char	info_internal;
	unsigned char	tx_mstp_type[32];
	unsigned char	rx_mstp_type[32];
	unsigned char	version[40];
	unsigned char	oper_edge;
	unsigned char	auto_edge;
	unsigned char	admin_edge;
	unsigned char	edge_delay;
	unsigned char	admin_rootguard;
	unsigned char	oper_rootguard;
	unsigned char   restricted_role;
	unsigned char	restricted_tcn;	
	unsigned char	oper_bpduguard;
	unsigned char	oper_bpdufilter;
	unsigned char	link_type[16];
	unsigned int	tx_bpdu_cnt;
	unsigned int	rx_bpdu_cnt;
	unsigned int    vlag_master; // Rbridge id of vlag master
} mstp_show_if_output_msg_t;

typedef struct mstp_show_mst_reg_output_msg {
    char            br_cfg_name[64];
    unsigned int    cfg_revision_lvl;
    char            cfg_digest[32];
} mstp_show_mst_reg_output_msg_t;

typedef struct mstp_show_inst_vlan_output_msg {
	unsigned int	inst;
    char            vlan[512];
    unsigned int    port_list_cnt;
    unsigned int    num_inst;
} mstp_show_inst_vlan_output_msg_t;

typedef struct mstp_show_msti_output_msg {
	unsigned int	inst;
	unsigned char	br_type;
	unsigned char	br_mstp_enabled;
	dcm_mstp_bridge_id_t designated_root_bridge_id;
	dcm_mstp_bridge_id_t bridge_id;
	dcm_mstp_bridge_id_t reg_root_bridge_id;
	unsigned char	self;
	unsigned char	self2;
    unsigned int    msti_bridge_priority;
    unsigned int    msti_priority;
    unsigned int    sys_id_ext;
	unsigned int	root_bridge_fwd_delay;
	unsigned int	root_bridge_hello;
	unsigned int	root_bridge_max_age;
	unsigned int	root_max_hop;
	unsigned char	root_port_ifname[MSTP_DCM_IF_NAME_LEN];
	unsigned int	bridge_fwd_delay;
	unsigned int	bridge_hello;
	unsigned int	bridge_max_age;
	unsigned int	bridge_max_hop;
	unsigned int	transmit_hold_count;
    unsigned int    external_root_path_cost;
    unsigned int     internal_root_path_cost;
	unsigned int	num_topo_changes;
	unsigned char	last_topo_change_ago[48];
	unsigned char	last_topo_change_if_name[64];
	unsigned char	errdisable_timeout_enable;
	unsigned int	errdisable_timeout_interval;
	unsigned int	migrate_time;
    unsigned int    port_list_cnt;
    unsigned int    num_inst;
	unsigned int    root_port_rbId;
} mstp_show_msti_output_msg_t;

#define DCM_MSTP_SHOW_RECORD_TYPE_MSTI 	        0x1
#define DCM_MSTP_SHOW_RECORD_TYPE_IF 	        0x2
#define DCM_MSTP_SHOW_RECORD_TYPE_INST_VLAN 	0x3
#define DCM_MSTP_SHOW_RECORD_TYPE_MST_REG 	    0x4
#define DCM_SSM_SHOW_RECORD_TYPE_SSM_IF 	    0x5
#define DCM_SSM_SHOW_RECORD_TYPE_BPDUGUARD 	    DCM_SSM_SHOW_RECORD_TYPE_SSM_IF

/* SSM interface information structure*/
typedef struct ssm_show_xstp_if_info {
                //unsgined int   if_index;
                char            if_name[MSTP_DCM_MAX_STR];
                unsigned char   bpduguard_type;
                unsigned char   tagged_bpdu_enable;
} ssm_show_xstp_if_info_t, ssm_show_bpduguard_all_t;

typedef struct mstp_show_msg {
        unsigned int 	cmd;
        unsigned int    rc;
        unsigned int 	type;
        unsigned int 	num_records;
        unsigned int 	ifindex;
        unsigned int 	record_type;
        unsigned int 	aging_time; // "show mac-address-table aging-time"

        unsigned char   more_rec_flag;

        union {
            mstp_show_msti_output_msg_t inst;
            mstp_show_if_output_msg_t port;
            mstp_show_inst_vlan_output_msg_t vlan;
            mstp_show_mst_reg_output_msg_t mstreg;
       		ssm_show_bpduguard_all_t bpdu_port; // Deprecated
            ssm_show_xstp_if_info_t  ssm_port;
        } records;
} mstp_show_msg_t;

typedef struct {
	unsigned int	cmd;
	unsigned int	ifType;
	unsigned char	name[MSTP_DCM_IF_NAME_LEN];
} mstp_msg_clear_t;

/* Copied from IPI Code */
#define NSM_BRIDGE_TYPE_STP                     1
#define NSM_BRIDGE_TYPE_STP_VLANAWARE           2
#define NSM_BRIDGE_TYPE_RSTP                    3
#define NSM_BRIDGE_TYPE_RSTP_VLANAWARE          4
#define NSM_BRIDGE_TYPE_MSTP                    5
#define NSM_BRIDGE_TYPE_PROVIDER_RSTP           6
#define NSM_BRIDGE_TYPE_PROVIDER_MSTP           7
#define NSM_BRIDGE_TYPE_CE                      8
#define NSM_BRIDGE_TYPE_PVST                    9
#define NSM_BRIDGE_TYPE_RPVST                   10
#define NSM_BRIDGE_NOT_CONFIGURED               0

/* Return codes */
#define DCM_MSTP_SHOW_RC_OK                     0
#define DCM_MSTP_SHOW_RC_NO_SPT                 100
#define DCM_MSTP_SHOW_RC_NO_MSTI                101
#define DCM_MSTP_SHOW_RC_NO_BR                  102
#define DCM_MSTP_SHOW_RC_NO_ENABLE              103
#define DCM_MSTP_SHOW_RC_MAX_INSTANCE           104

#endif	/* __MSTP_MESSAGE_DEF_H__ */
