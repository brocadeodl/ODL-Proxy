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

/*
 ******************************************************************************
 * Copyright (c) 1996-2011 Brocade Communications Systems, Inc.
 * All rights reserved.
 *
 * Module name:L3 
 *
 * Description:
 *  This header file contains defines and structure definition of IPC messages
 *  used between RTM and other modules (IFM, OSPF etc).
 *****************************************************************************
 */
#ifndef _L3_IPC_H_
#define _L3_IPC_H_

#include "dce_ipc_common.h"
#include "rip_ipc.h"

#define DCE_NULL_INTERFACE	0xFFFFFFFF
/* SBFIXIT - move to dce_l3_vrf.h */
#define VRF_NAME_MAX_LENGTH 256

/* server names and instance please add here */
#define OSPF_SERVER_NAME  "IPCOSPF"
#define OSPF_INSTANCE	  MYSWITCH
#define OSPF_IPC_INSTANCE DCE_IPC_INSTANCE
#define OSPF_NODE		  MYNODE

#define PIM_SERVER_NAME    "IPCPIM"
#define PIM_INSTANCE       MYSWITCH
#define PIM_IPC_INSTANCE   DCE_IPC_INSTANCE
#define PIM_NODE           MYNODE

#define RTM_SERVER_NAME "IPCRTM"
#define RTM_INSTANCE	 MYSWITCH
#define RTM_IPC_INSTANCE DCE_IPC_INSTANCE
#define RTM_NODE		 MYNODE

#define BGP_SERVER_NAME  "IPCBGP"
#define BGP_INSTANCE	 MYSWITCH
#define BGP_IPC_INSTANCE DCE_IPC_INSTANCE
#define BGP_NODE		 MYNODE

#define ARP_SERVER_NAME "IPCARP"
#define ARP_INSTANCE MYSWITCH
#define ARP_IPC_INSTANCE DCE_IPC_INSTANCE
#define ARP_NODE MYNODE

#define SSM_IPC_NAME "SSMIPC"
#define SSM_IPC_INSTANCE DCE_IPC_INSTANCE
#define SSM_IPC_NODE MYNODE

/* common code please add here */

#define L3_IPC_TIMEOUT	5
#define IP_NH_INVALID_NEXTHOP_ID NO_SUCH_PORT

/* Attention: added protocols should add their values */
typedef enum {
	CB_L3_MIN_IPC = 0,
    CB_OSPF_GENERIC_IPC = CB_L3_MIN_IPC,
	CB_RTM_GENERIC_IPC,
	CB_RIP_GENERIC_IPC,
	CB_PIM_GENERIC_IPC,
	CB_PIM_EVT_IPC,
	CB_POLICY_IPC,
    CB_IGMP_IPC,
    CB_RTM_FIB_IPC,
    CB_BGP_GENERIC_IPC,
	CB_ARP_GENERIC_IPC,
	CB_L3_MAX_IPC
}L3IpcRegister;


#define L3_SETIPC_DEST(_dest, peer, protocol_name, protocol_instance)          \
    do {                                        \
	    strcpy((_dest).name, protocol_name);       \
		(_dest).instance = protocol_instance;          \
		(_dest).node = (peer) ? PEERNODE : MYNODE;  \
	} while (0)

/* Initilization using basic timeout value */
#define L3_SET_IPC_TIMEOUT(timeout)		\
do {										\
	(timeout).tv_sec = L3_IPC_TIMEOUT;	\
	(timeout).tv_usec = 0;					\
} while (0)

#define IP_MAX_MAX_EQUAL_COST_PATHS 8

extern int l3_ipc_send(char *src_proto_name, int src_type, char *dst_proto_name, int dst_type, int type, void *data,
					   size_t dsz, char **resp, size_t *rsz, int peer);

extern int l3_client_ipc_send(char *dst_proto_name, int dst_type, int ipc_type, void *data, size_t dsz, int peer);

/* BGP related stuff */
typedef enum {
	BGP_MIN_IPC = 0,
	BGP_CLEAR_ROUTE_IPC,
	BGP_CLEAR_ROUTE_ALL_IPC,
	BGP_OSPF_REQ_ROUTE_CONVERGENCE_IPC,
	BGP_MAX_IPC
}BgpIpcCbCode;

/* OSPF related stuff */
typedef enum {
	OSPF_MIN_IPC = 0,
	OSPF_CLEAR_ROUTE_IPC,
	OSPF_CLEAR_ROUTE_ALL_IPC,
	OSPF_BGP_ROUTE_CONVERGENCE_COMPELETED_IPC,
	OSPF_MAX_IPC
}OspfIpcCbCode;

/* RIP related stuff */
typedef enum {
	RIP_MIN_IPC = 3,
	RIP_CLEAR_ROUTE_IPC,
	RIP_CLEAR_ROUTE_ALL_IPC,
	RIP_MAX_IPC
}RipIpcCbCode;

/* RTM related stuff */
typedef enum {
	RTM_MIN_IPC = 0,
	RTM_ROUTE_DNLD_IPC,
	RTM_OTHER_PROTO_ENABLE_IPC,
	RTM_OTHER_PROTO_DISABLE_IPC,
	RTM_CLEAR_ROUTE_IPC,
	RTM_CLEAR_ROUTE_ALL_IPC,
	RTM_IP_LOAD_SHARE_IPC,
	RTM_IP_ROUTER_ID_IPC,
	RTM_ENABLE_PROTO_REDIS_IPC,
	RTM_DISABLE_PROTO_REDIS_IPC,
	RTM_REDIS_READD_IPC,
	RTM_REDIS_PREFIX_IPC,
	RTM_REDIS_ENABLE_DEFAULT_RT_IPC,
	RTM_REDIS_DISABLE_DEFAULT_RT_IPC,
	RTM_ROUTE_EVT_NOTIFY_IPC,
	RTM_DY_SYNC_IPC,
	RTM_STATIC_ROUTE_DEL_IPC,
        RTM_ROUTE_REG_IPC,
	RTM_ROUTE_DEREG_IPC,
	/* VRF Related IPC */
	RTM_VRF_RD_IPC,
	RTM_VRF_RT_IPC,
	RTM_VRF_AF_IPV4_IPC,
	RTM_VRF_AF_IPV4_RT_IPC,
	RTM_VRF_MAX_ROUTE_IPC,
	RTM_VRF_NH_VRF_IPC,
    RTM_PBR_ROUTE_REGISTER_IPC,
    RTM_PBR_ROUTE_DEREGISTER_IPC,

	RTM_ENABLE_REDIS_IBGP_IPC,
	RTM_DISABLE_REDIS_IBGP_IPC,
	RTM_ENABLE_RIB_COUNT_BGP_IPC,

	RTM_MAX_IPC
} RtmIpcCbCode;

typedef enum {
	RTM_ROUTE_ADD = 0,
	RTM_ROUTE_MOD,
	RTM_ROUTE_DEL
} RtmRouteDnld;


/* used for clearing one route */
typedef struct clear_ipv4_route {
    unsigned int ip_address;
    unsigned int prefix_len;
}clearip4route_t;

#if 0 // SBMERGE (lives in dp_trie_ipv4_sync_m.h)
/* used for rtm-l3ss */
#define DP_TRIE_INSERT_SYNC		0
#define DP_TRIE_DELETE_SYNC	1
#define DP_TRIE_CLEAR_SYNC		2
#define DP_TRIE_COMPRESS_SYNC	3
#define DP_TRIE_INSERT_IP_NH	4
#define DP_TRIE_DELETE_IP_NH	5
#define DP_TRIE_SYNC_ALL_IP_NH	6
#endif

/* defined to sync interface from rtm to fib */
#define RTM_FIB_IF_SYNC			7


#if 0 // SBMERGE (lives in dp_trie_ipv4_sync_m.h)
typedef struct dy_sync_header
{
	unsigned long serial_number; 
	unsigned long route_number;
	unsigned long magic_number;
	unsigned short sync_type; // We just need 8 bits
	unsigned char  all_done;
	unsigned char  fib_loc;
} dy_sync_header;

typedef struct tree_veri_header // for verification
{
	unsigned long num_of_route_all;
	unsigned long num_of_route_info_all;
	unsigned long start_ip;
	unsigned char  start_bitlen;
	unsigned long last_ip;
	unsigned char  last_bitlen;
	unsigned long num_of_route;
	unsigned long num_of_route_info;
	unsigned long sum_of_ip;
	unsigned long sum_of_mask_len;
	unsigned long sum_of_next_hop;
	unsigned long sum_of_outgoing_interface;
	unsigned long display_mode; // What the BP do after receive BP
} tree_veri_header;


typedef struct IP_TREE_VERIFY_STAT
{
	unsigned long Veri_packet_num;
	unsigned long Veri_packet_num_checksum;
	unsigned long Veri_full_tree;
	unsigned long Veri_packet_route_num_all_err;
	unsigned long Veri_packet_route_info_all_err;
	unsigned long Veri_packet_route_num_err;
	unsigned long Veri_packet_route_info_err;
	unsigned long Veri_packet_ip_sum_err;
	unsigned long Veri_packet_bitlen_sum_err;
	unsigned long Veri_packet_nexthop_sum_err;
	unsigned long Veri_packet_outgoing_sum_err;
	unsigned long Veri_packet_last_ip_err;
	unsigned long Veri_full_tree_last_time;
	unsigned long Veri_packet_err_lasttime;
	unsigned long Veri_reload_last_time;
	unsigned long restart_due_to_veri_error;
	unsigned long last_display_mdoe;
	unsigned long error_start_ip;
	unsigned long error_last_ip;
	unsigned long error_start_bitlen;
	unsigned long error_last_bitlen;
} IP_TREE_VERIFY_STAT;

#define LP_VRF_NAME_MAX_LENGTH	16 // so that record below is 20bytes
typedef struct VRF_UPDATE_ENTRY
{
	unsigned long 	vrf_idx:12;			//4K
	unsigned long	add:1;
	unsigned long 	max_v4_route:19;	//500K
	unsigned long	max_v6_route:18;	
	unsigned long	ipv4:1;
	unsigned long	ipv6:1;
	unsigned long	spare:12;
	unsigned long	vpn_id;
	char	vrf_name[LP_VRF_NAME_MAX_LENGTH];
} VRF_UPDATE_ENTRY;

// 4.0

typedef struct IP_TREE_DOWNLOAD_REQUEST
{
	unsigned short 	tree_sync_current_vrf_index;
	unsigned long 	tree_sync_current_nh_id;
	unsigned long	tree_sync_current_ip;
	unsigned long	tree_sync_current_mgmt_ip;
	unsigned long	tree_sync_current_if_ip;
	unsigned long	tree_sync_current_if_index;
	unsigned char	tree_sync_bitlen;
} IP_TREE_DOWNLOAD_REQUEST;

typedef struct IP_TREE_UPDATE_TLV_ENTRY
{
	unsigned short type:4;
	unsigned short length:12;
	unsigned short vrf_index;
	char   data[1];
} IP_TREE_UPDATE_TLV_ENTRY;


typedef struct IP_ROUTE_INFO_UPDATE
{
	unsigned long					next_hop_router_ip_address;
	unsigned long					outgoing_interface;
	unsigned char					direct_connected; 
	unsigned char                   loopback:1;
	unsigned char                   mgmt_if:1;
	unsigned char                   unused:6;
	unsigned char					spare[2];
} IP_ROUTE_INFO_UPDATE;

typedef struct IP_NH_ENTRY_UPDATE
{
	unsigned long nh_id;			// ID for this nexthop entry, used for NH reference
	unsigned char num_of_paths;	// determines the size of this nexthop entries, number of ECMP paths.
	IP_ROUTE_INFO_UPDATE  path[1];
} IP_NH_ENTRY_UPDATE;

typedef struct IP_TREE_UPDATE_NH_ENTRY
{
	unsigned short type:4;		// nh add/change, delete
	unsigned short length:12;		// total size 
	unsigned short vrf_index;
	IP_NH_ENTRY_UPDATE nh[1];	// nh entries to follow
} IP_TREE_UPDATE_NH_ENTRY;

typedef struct IP_PREFIX_ENTRY_UPDATE
{
	unsigned long target;
	unsigned char mask_len;
	unsigned char type:4;
	unsigned char by_host:1; 
	unsigned char default_net:1;
	unsigned char spare:2;
	unsigned long as_magic;	// do we need full 32 bits - Perhaps 24 bits and 8 bits Unused for later?
	unsigned char  distance;
} IP_PREFIX_ENTRY_UPDATE;

typedef struct IP_TREE_UPDATE_RT_ENTRY
{
	unsigned short type:4;			//route add/modify, delete
	unsigned short length:12;		// size determines number of IP_PREFIX to follow
	unsigned short vrf_index;
	unsigned long nh_id;
	// variable number of the following
	IP_PREFIX_ENTRY_UPDATE	prefix[1];
} IP_TREE_UPDATE_RT_ENTRY;
#endif // SBMERGE

/* interface sync */

typedef enum{
    RTM_FIB_IP_ADDRESS_ADD = 1,
    RTM_FIB_IP_ADDRESS_DELETE,
    RTM_FIB_STATUS_UPDATE,
    RTM_FIB_INTERFACE_ADD,
    RTM_FIB_INTERFACE_DELETE,
    RTM_FIB_STATUS_UP,
    RTM_FIB_STATUS_DOWN,
    RTM_FIB_ADMIN_STATUS_UP,
    RTM_FIB_ADMIN_STATUS_DOWN,
    RTM_FIB_MAX_MSGS
}RTM_IF_UPDATE_MSG_TYPE;

typedef struct RTM_IF_UPDATE_
{
    unsigned short type:4;
    unsigned short length:12;
    unsigned short vrf_index;

	RTM_IF_UPDATE_MSG_TYPE sub_type;

	unsigned long ifindex;
    unsigned char ifinfo[1];
} RTM_IF_UPDATE;

typedef struct RTM_IF_STATUS_UPDATE_
{
	unsigned char type;
	unsigned char capability;
        unsigned char flag2;
	unsigned long vrf_id;
	unsigned long cindex;
	unsigned long flags;
	unsigned long mtu;
} RTM_IF_STATUS_UPDATE;

#define RTM_IF_NAME_LEN			20
typedef struct RTM_IF_INFO_
{
	unsigned char ifname[RTM_IF_NAME_LEN + 1];
	unsigned char mac_addr[6];
	unsigned long ivid;
	unsigned short port_number;
	unsigned char up;
	unsigned long if_index;
	RTM_IF_STATUS_UPDATE if_status;
} RTM_IF_INFO;

typedef struct RTM_IF_IP_ADDR_
{
	unsigned long ip_addr;
	unsigned char prefix_len;
	unsigned short port_number;
	unsigned char secondary;
} RTM_IF_IP_ADDR;

#define MAX_DY_IF_INFO_SIZE	(sizeof(RTM_IF_UPDATE) + sizeof(RTM_IF_INFO))
#define MAX_DY_IF_IP_ADDR_SIZE	(sizeof(RTM_IF_UPDATE) + sizeof(RTM_IF_IP_ADDR))

/* interface sync */

#if 0 // SBMERGE: reverting to NI data structures
typedef struct RTM_IPV4_NEXTHOP_PATH
{ 
	unsigned long	next_hop_router_ip_address; 
	unsigned short	outgoing_interface; 
	unsigned char	direct_connected; 
	unsigned char	vip_address:1; 
	unsigned char	spare:7; 
#ifdef MPLS
	MPLS_VRF_LABEL_ROUTE		label;
#endif 
} RTM_IPV4_NEXTHOP_PATH;

typedef struct RTM_IPV4_PATH_PARAM
{ 
	unsigned long as_magic;
	unsigned short num_as;
	unsigned char spare;
	unsigned char number_of_paths;
	unsigned int igp_cost;
	RTM_IPV4_NEXTHOP_PATH	nexthops[IP_MAX_MAX_EQUAL_COST_PATHS]; 
} RTM_IPV4_PATH_PARAM;
#endif // SBMERGE

#define RTM_ROUTE_CMD_HDR 		\
			unsigned char	cmd;		\
			unsigned char	af;			\
			unsigned char	proto;		\
			unsigned char	flags;		\
			unsigned short	vrf_idx;	\
			unsigned short	len;		\
			unsigned long	ip_addr;	\
			unsigned long	ip_mask;

typedef struct {
	RTM_ROUTE_CMD_HDR
} rtm_route_cmd_hdr_t;

typedef struct {
	RTM_ROUTE_CMD_HDR
	unsigned long		path_type;
	unsigned long		distance;
	unsigned long		metric;
	unsigned long		tag;
//	RTM_IPV4_PATH_PARAM	nexthops;
} rtm_add_route_msg_t;

typedef struct {
	RTM_ROUTE_CMD_HDR
	unsigned long		path_type;
	unsigned long		distance;
	unsigned long		metric;
	unsigned long		tag;
//	RTM_IPV4_PATH_PARAM	nexthops;
} rtm_mod_route_msg_t;

typedef struct {
	RTM_ROUTE_CMD_HDR
} rtm_del_route_msg_t;

typedef struct {
	unsigned short		src_proto;
	unsigned short		route_type;
	unsigned short		safi;
	unsigned short		vrf_idx;
} l3_redis_ipc_t;

typedef struct {
	unsigned char		src_proto;
	unsigned char		prefix_len;
	unsigned long		prefix;
	unsigned short		safi;
	unsigned short		vrf_idx;
} l3_redis_prefix_ipc_t;

typedef struct {
	unsigned short		safi;
	unsigned short		vrf_idx;
} rtm_route_evt_t;

typedef struct {
	unsigned short		af;
	unsigned short		vrf_idx;
	unsigned short		clnt_proto;
    int                 ipcMsgType;
    unsigned char       vrf_name[VRF_NAME_MAX_LENGTH];
} rtm_proto_reg_msg_t;

typedef struct {
	unsigned short		af;
	unsigned short		vrf_idx;
	unsigned long		prefix;
	unsigned char       prefix_len;
	unsigned long       ifindex;
} rtm_static_route_del_t;

/* ip address/prefix reg/dereg support */
typedef struct {
	unsigned short		af;
	unsigned short		vrf_idx;
	unsigned int        ip_address;
	unsigned short		clnt_proto;
    unsigned char       vrf_name[VRF_NAME_MAX_LENGTH];
} rtm_rt_reg_msg_t;

typedef struct {
    unsigned int        rd_rt[2];
    int                 rd_rt_type;
#if 0    
	unsigned short  rd_rt_type;
	union  {
	    struct {
	        unsigned short  rd_rt_asf_admin;
	        unsigned long   rd_rt_asf_assigned;
	    } rd_rt_as2_value;    
	    struct {
	        unsigned long   rd_rt_asf_admin;
	        unsigned short  rd_rt_asf_assigned;
	    } rd_rt_as4_value;    
        } rd_rt_value;
#endif        
} rtm_rd_rt_t;

typedef struct {
	unsigned short		vrf_idx;
	unsigned char		af;
	unsigned long		router_id;
	rtm_rd_rt_t		    rd_rt;
	unsigned char		rt_type;
	unsigned char		add_del_flag;
    unsigned long       max_route;
} rtm_vrf_ipc_t;	

typedef struct {
	unsigned int 		rib_lim;
	unsigned short		safi;
	unsigned short		vrf_idx;
} l3_rib_count_ipc_t;

typedef struct {
	unsigned int of_vrf_index;
	unsigned int  safi;
} bgp_ospf_ipc_t;

#endif /* _L3_IPC_H_ */
