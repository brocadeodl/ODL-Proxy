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

#ifndef __VRRP_MESSAGE_DEF_H__
#define __VRRP_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/InterfaceCommon.h"

#define VRRP_TRUE 1

#define VRRP_DEFAULT_PRIO       100
#define VRRP_DEFAULT_ADVT_INT   1
#define VRRP_DEFAULT_HOLD_TIME  0
#define VRRP_DEFAULT_TRACK_PRIO 2
#define VRRP_DEFAULT_BACKUP_ADVT_INT   60
#define VRRPE_DEFAULT_GARP_TIMER        2

#define VRRP_PLUG_ASSERT_PTR(exp) \
            prismAssert ((exp) != NULL, __FILE__, __LINE__);
#define VRRP_IFNAME_LEN			  64
#define VRRP_DESCRIPTION_MAX_LEN  65
#define MAX_VRRP_VIRTUAL_IPS	  16
#define MAX_BACKUP_ROUTERS        100
#define MAX_TRACKPORT_INTERFACES  16 
#define IPV4_ADDR_STR_SIZE        17
#define HAL_MSG_MAX_RB_IN_MCT     16

#define VRRP_DEBUG_EVENT				(1<<0)
#define VRRP_DEBUG_PACKET_SEND			(1<<1)
#define VRRP_DEBUG_PACKET_RECV			(1<<2)
#define VRRP_DEBUG_SESSION				(1<<3)
#define VRRP_DEBUG_PACKET_INTF         (1<<4)

typedef enum {
    VRRP_SET_VRID,
    VRRP_PRIORITY,
    VRRP_ADVT_INT,
    VRRP_ENABLE,
    VRRP_PREEMPT,
    VRRP_HOLD_TIME,
    VRRP_DESC,
    VRRP_ADVT_BACKUP,
    VRRP_BACKUP_ADVT_INT,
	VRRP_SPF,
	VRRP_GARP_TIMER,
    VRRP_VIP,
    VRRP_TRACK,
	VRRP_REVERT_PRIORITY,
    NO_VRRP_SET_VRID,
    NO_VRRP_PRIORITY,
    NO_VRRP_ADVT_INT,
    NO_VRRP_ENABLE,
    NO_VRRP_PREEMPT,
    NO_VRRP_VIP,
    NO_VRRP_TRACK,
    NO_VRRP_HOLD_TIME,
    NO_VRRP_DESC,
    NO_VRRP_ADVT_BACKUP,
    NO_VRRP_BACKUP_ADVT_INT,
	NO_VRRP_SPF,
	NO_VRRP_GARP_TIMER,
	NO_VRRP_REVERT_PRIORITY,
	VRRP_INTF_CONFIG_REPLAY,
	VRRP_GLOBAL_CMD_ENABLE,
	VRRP_GLOBAL_CMD_DISABLE,
    VRRPE_GLOBAL_CMD_ENABLE,
    VRRPE_GLOBAL_CMD_DISABLE,
    VRRP_ALL_GLOBAL_CMD_ENABLE,
    VRRP_ALL_GLOBAL_CMD_DISABLE,
	NO_INTF_IP_REAL,
	NO_INTF_IP_SEC,
	NO_INTF_IP_ALL
} vrrp_cmd_choice;

typedef enum {
    VRRP_OP_INVALID,
    VRRP_OP_ADD,
    VRRP_OP_DELETE
} VRRPOpType;

typedef enum {
	VRRP_TYPE_NONE,
    VRRP_TYPE_STANDARD,
    VRRP_TYPE_EXTENDED,
    VRRP_TYPE_CLUSTER
} VRRPType;

typedef enum {
	VRRP_DEBUG_CMD_SHOW,
    VRRP_DEBUG_CMD_SESSION,
    VRRP_DEBUG_CMD_PACKET_SENT,
    VRRP_DEBUG_CMD_PACKET_RECV,
    VRRP_DEBUG_CMD_ALL,
    VRRP_DEBUG_CMD_EVENT,
    VRRP_DEBUG_CMD_PACKET,
	VRRP_DEBUG_CMD_INTERFACE,

    VRRP_DEBUG_CMD_MAX
} VRRPDebugCmdCode;

typedef enum {
    VRRP_CLEAR_STATS_CMD_ALL,
    VRRP_CLEAR_STATS_CMD_SESS,
    VRRP_CLEAR_STATS_CMD_INTERFACE,

    VRRP_CLEAR_STATS_CMD_MAX
} VRRPClearStatsCmdCode;

typedef enum {
    VRRP_SHOW_CMD_TYPE_DETAIL,
    VRRP_SHOW_CMD_TYPE_SUMMARY,
    VRRP_SHOW_CMD_TYPE_NONE,
	VRRP_SHOW_CMD_TYPE_NSM_DB,
	VRRP_SHOW_CMD_TYPE_NSM_PENDING_LIST
} VRRPShowCmdType;

typedef enum {
    VRRP_SHOW_CMD,
    VRRP_SHOW_CMD_VRID,
    VRRP_SHOW_CMD_INTERFACE,
	VRRP_SHOW_CMD_NSM_DB,
	VRRP_SHOW_CMD_NSM_PENDING_LIST
} VRRPShowCmdCode;

typedef struct vrrp_global_config_msg_ {
    int  config_disable;
    int  vrrp_config;
    int  vrrpe_config;
} vrrp_global_config_msg_t;

typedef struct vrrp_virtual_ip_ {
	char virtual_ip[IPV4_ADDR_STR_SIZE];
} vrrp_virtual_ip_t;

typedef struct vrrp_track_port_ {
	unsigned int track_ifType;
	char		 track_ifName[VRRP_IFNAME_LEN];
	unsigned int track_prio;
} vrrp_track_port_t;

typedef struct vrrp_config_replay_info_ {
	vrrp_virtual_ip_t vip_list[MAX_VRRP_VIRTUAL_IPS];
	vrrp_track_port_t track_list[MAX_TRACKPORT_INTERFACES];
	unsigned int	  num_virtual_ip;
	unsigned int	  num_track_port;
} vrrp_config_replay_info_t;

typedef struct vrrp_local_config_msg_ {
    char ifName[VRRP_IFNAME_LEN];
    unsigned int ifType;
    unsigned int vrid;
    char virtual_ip[IPV4_ADDR_STR_SIZE];
    unsigned int track_ifType;
    char track_ifName[VRRP_IFNAME_LEN];
    unsigned int track_prio;
    unsigned int advt_int;
    unsigned int prio;
    int  enable;
    int  preempt;
    unsigned int hold_time;
    char description[VRRP_DESCRIPTION_MAX_LEN];
    int  owner;
    int  advt_backup;
    unsigned int garp_timer;
    int  vrrp_type;
	int  spf;
	int  rev_prio;
	unsigned int backup_advt_int;
	unsigned int intf_ip_addr;
	unsigned int intf_ip_mask;
    unsigned int choice;

	char config_replay[1];
} vrrp_local_config_msg_t;

typedef struct vrrp_clear_stats_input_msg {
    unsigned int cmdCode;
    unsigned int vrid;
    char if_name[VRRP_IFNAME_LEN];
} vrrp_clear_stats_input_msg_t;

typedef struct vrrp_clear_stats_output_msg {
    unsigned int cmdCode;
    int result;
    int result1;
    int result2;
} vrrp_clear_stats_output_msg_t;

typedef struct vrrp_debug_input_msg {
    unsigned int cmdCode;
    unsigned int vrid;
    unsigned int negation;
	char if_name[VRRP_IFNAME_LEN];
} vrrp_debug_input_msg_t;

typedef struct vrrp_debug_output_msg {
    unsigned int cmdCode;
	unsigned int vrid;
    char if_name[VRRP_IFNAME_LEN];
    int debug_bitmap;
} vrrp_debug_output_msg_t;

typedef struct vrrp_show_input_msg {
    unsigned int cmdCode;
    unsigned int showType;
    unsigned int vrid;
	unsigned int lastIndex;
    char if_name[VRRP_IFNAME_LEN];
} vrrp_show_input_msg_t;

typedef struct vrrp_show_output_summary_msg {
    unsigned int  vrid;
    unsigned char mode;
    char          ifname[VRRP_IFNAME_LEN];
    unsigned char admin_state;
    unsigned int  ifindex;
    int           curr_prio;
    unsigned char state;
	unsigned char spf;
	unsigned char spf_reverted;
	int           revert_prio;
} vrrp_show_output_summary_msg_t;

typedef enum vrrp_trackport_status_ {
	VRRP_TRACKPORT_ABSENT = 0,
	VRRP_TRACKPORT_UP,
	VRRP_TRACKPORT_DOWN
} vrrp_trackport_status_t;

typedef struct vrrp_trackport_info_ {
	char ifname[VRRP_IFNAME_LEN];
	unsigned char priority;
	vrrp_trackport_status_t port_status;
} vrrp_trackport_info_t;

#define VRRP_TRACKPORT_STATUS_STR(trackport_status) \
	((trackport_status == VRRP_TRACKPORT_ABSENT) ? "Absent" : ((trackport_status == VRRP_TRACKPORT_UP) ? "Up" : "Down"))
	
typedef struct vrrp_show_output_brief_msg {
    unsigned int vrid;
    char ifname[VRRP_IFNAME_LEN];
    unsigned int ifindex;
    unsigned char mode;
    unsigned char admin_state;
    unsigned char master_ip[IPV4_ADDR_STR_SIZE];

    unsigned char af_type;
    unsigned char auth_type;
    unsigned char state;
	int           vip_status;
	unsigned int  num_vips;
    unsigned char vip[MAX_VRRP_VIRTUAL_IPS][IPV4_ADDR_STR_SIZE];
	unsigned int  num_backup_rtrs;
	unsigned char backup_routers[MAX_BACKUP_ROUTERS][IPV4_ADDR_STR_SIZE];
	unsigned char vmac[16];
    int           conf_prio;
    int           curr_prio;
    int           adv_int;
    unsigned char preempt;
	unsigned char backup_advt;
	int           backup_hello_int;
    unsigned int  hold_time;
	unsigned int  num_track_ports;
    vrrp_trackport_info_t track_info[MAX_TRACKPORT_INTERFACES];
	char          desc[VRRP_DESCRIPTION_MAX_LEN];
	unsigned char spf;
	unsigned char spf_reverted;
	int           revert_prio;

    /* Session Statistics */
    unsigned int stats_advt_rcvd;
    unsigned int stats_advt_sent;
    unsigned int stats_arps_rcvd;
    unsigned int stats_arps_sent;
} vrrp_show_output_brief_msg_t;

typedef struct vrrp_show_output_detail_msg {
	vrrp_show_output_brief_msg_t info;

    /* Global statistics */
    unsigned int vrrp_stats_checksum_err;
    unsigned int vrrp_stats_version_err;
    unsigned int vrrp_stats_invalid_vrid;

    unsigned int stats_become_master;
    unsigned int stats_advt_int_errors;
    unsigned int stats_pri_zero_pkts_rcvd;
    unsigned int stats_pri_zero_pkts_sent;
    unsigned int stats_invalid_type_pkts_rcvd;
    unsigned int stats_addr_list_errors;
    unsigned int stats_invalid_auth_type;
    unsigned int stats_ip_ttl_errors;
    unsigned int stats_pkt_len_errors;
	unsigned int vrrpe_bkup_adv_sent;
	unsigned int vrrpe_bkup_adv_recvd;
    unsigned int master_down_int;
	unsigned int vrrpe_garp_timer;
} vrrp_show_output_detail_msg_t;

typedef struct vrrp_db_nsm_show_msg {
    unsigned int vrid;
	unsigned int vlanid;
	unsigned int vrbid;
	unsigned char vmac[7];
	unsigned int phy_rb[HAL_MSG_MAX_RB_IN_MCT];
	unsigned int total_rb;
	unsigned char status[7];
	unsigned int vmac_vrbid_en;
} vrrp_db_nsm_show_msg_t;

typedef struct vrrp_pending_list_nsm_show_msg {
    unsigned int vrid;
	unsigned int vid;
	unsigned int vrbid;
	unsigned char vmac[7];
	unsigned char status[7];
	unsigned int vmac_vrbid_en;
} vrrp_pending_list_nsm_show_msg_t;

typedef struct vrrp_show_output_msg {
    unsigned int cmdCode;
	unsigned int num_records;
	unsigned int record_type;
	unsigned int ret_code;
	unsigned int total_records;
	unsigned int last_index;
	unsigned int master_count;
	unsigned int backup_count;
	unsigned int init_count;
	unsigned char more_elements;
	union {
	    vrrp_show_output_summary_msg_t summary[1];
		vrrp_show_output_brief_msg_t brief[1];
		vrrp_show_output_detail_msg_t detail[1];
		vrrp_db_nsm_show_msg_t vrrpdb[1];
		vrrp_pending_list_nsm_show_msg_t pendinglist[1];
	} records;
} vrrp_show_output_msg_t;


#endif  /* __VRRP_MESSAGE_DEF_H__ */
