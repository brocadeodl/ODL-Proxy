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
 *
 *	  Copyright (c) 1996-2009 Brocade Communications Systems, Inc.
 *	  All rights reserved.
 *
 *	  Module name: rtm_util.h
 *
 *	  Description:
 *    This file contains utility and prototypes of glueing functions 
 *    and definitions.
 *    
 */
#ifndef __RTM_UTIL_H__
#define __RTM_UTIL_H__

#ifndef DCE_RTM_PROTO_HACK
#define RTM_PROTO_ID 254
#else
#include "dce_l3_util.h"
#endif /* DCE_RTM_PROTO_HACK */
#include "l3_ipc.h"

#define DCE_MAX_PORTS  (DCE_MAX_PHY_PORTS + DCE_MAX_LAG + DCE_MAX_VLAN)

typedef enum {
	RTM_MIN_TP = 0,								/* RTM Min TP # */
	RTM_BEF_SEND_NETDEV_CREATE_HSL_TP,
	RTM_AFT_SEND_NETDEV_CREATE_HSL_TP,
	RTM_BEF_SEND_NETDEV_DELETE_HSL_TP,
	RTM_AFT_SEND_NETDEV_DELETE_HSL_TP,
	RTM_BEF_SEND_IPADDR_ADD_KERN_TP,
	RTM_AFT_SEND_IPADDR_ADD_KERN_TP,
	RTM_BEF_SEND_IPADDR_DEL_KERN_TP,
	RTM_AFT_SEND_IPADDR_DEL_KERN_TP,
	RTM_BEF_SEND_ROUTERID_SET_OSPF_TP,
	RTM_AFT_SEND_ROUTERID_SET_OSPF_TP,
	RTM_BEF_SEND_ROUTERID_SET_BGP_TP,
	RTM_AFT_SEND_ROUTERID_SET_BGP_TP,
	RTM_BEF_SEND_ROUTERID_UNSET_OSPF_TP,
	RTM_AFT_SEND_ROUTERID_UNSET_OSPF_TP,
	RTM_BEF_SEND_ROUTERID_UNSET_BGP_TP,
	RTM_AFT_SEND_ROUTERID_UNSET_BGP_TP,
	RTM_BEF_SEND_RD_SET_OSPF_TP,
	RTM_AFT_SEND_RD_SET_OSPF_TP,
	RTM_BEF_SEND_RD_SET_BGP_TP,
	RTM_AFT_SEND_RD_SET_BGP_TP,
	RTM_BEF_SEND_RT_SET_OSPF_TP,
	RTM_AFT_SEND_RT_SET_OSPF_TP,
	RTM_BEF_SEND_RT_SET_BGP_TP,
	RTM_AFT_SEND_RT_SET_BGP_TP,

	RTM_MAX_TP									/* RTM Max TP # */
} rtm_tp_type;

/* Macro to add Testpoints - TP based testing */
/*
#define RTM_TP_HOOK(_state) \
    nsm_lib_tp_hook(rtm_root.lib_global->fss_hndl, _state, 0)
*/
/*
typedef enum {
	BEF_SEND_ROUTERID_SET_OSPF,
	AFT_SEND_ROUTERID_SET_OSPF,
	BEF_SEND_ROUTERID_SET_BGP,
	AFT_SEND_ROUTERID_SET_BGP,
	BEF_SEND_ROUTERID_UNSET_OSPF,
	AFT_SEND_ROUTERID_UNSET_OSPF,
	BEF_SEND_ROUTERID_UNSET_BGP,
	AFT_SEND_ROUTERID_UNSET_BGP,
	BEF_SEND_NETDEV_CREATE_HSL,
	AFT_SEND_NETDEV_CREATE_HSL,
	BEF_SEND_NETDEV_DELETE_HSL,
	AFT_SEND_NETDEV_DELETE_HSL,
	BEF_SEND_IPADDR_ADD_KERN,
	AFT_SEND_IPADDR_ADD_KERN,
	BEF_SEND_IPADDR_DEL_KERN,
	AFT_SEND_IPADDR_DEL_KERN,
	BEF_SEND_RD_SET_OSPF,
	AFT_SEND_RD_SET_OSPF,
	BEF_SEND_RD_SET_BGP,
	AFT_SEND_RD_SET_BGP,
	BEF_SEND_RT_SET_OSPF,
	AFT_SEND_RT_SET_OSPF,
	BEF_SEND_RT_SET_BGP,
	AFT_SEND_RT_SET_BGP
} rtm_tp;
*/
typedef enum {
	CLI_RTM_SUCCESS	= 0,
	CLI_STATIC_NON_CONT_SUBNETMASK,
	CLI_STATIC_INCONSISTENT_SUBNETMASK,
	CLI_STATIC_MULTICAST_ADDRESS,
	CLI_STATIC_INVLD_PORT,
	CLI_STATIC_DUPLICATE_ROUTE,
	CLI_STATIC_INVLD_ROUTE,
	CLI_STATIC_ONE_PATH_ALLOWED,
	CLI_STATIC_INT_DEF_NOT_ALLOWED,
	CLI_STATIC_INVLD_VRF,
	CLI_STATIC_INVLD_NHOP,
	CLI_STATIC_ZERO_NHOP,
	CLI_ROUTE_NOT_FOUND,
	CLI_STATIC_BAD_PREFIX,
	CLI_STATIC_BAD_NHOP_IP,
	CLI_STATIC_OIF_NOT_EXIST,
	CLI_STATIC_MAX_ROUTES_REACHED,
	CLI_RTM_INVALID_ROUTER_ID,
	CLI_RTM_VRF_INVLD_RD,
	CLI_RTM_VRF_RD_AF_NOTSET,
	CLI_ERR_MAX
} CLI_ERR_NO;

typedef enum {
	RTM_VRF_RD_THA_MSG = 1,
	RTM_VRF_AF_THA_MSG = 2,
    RTM_MGMT_RT_THA_MSG
} rtm_tha_msg_types;


extern void		*g_cli;

int rtm_map_port_state(int ifindex, int up);
int rtm_map_ip_addr_add(int ifindex, unsigned long ip_addr, int mask_len, int secondary, int virt);
int rtm_map_ip_addr_del(int ifindex, unsigned long ip_addr, int mask_len, int secondary, int virt);
int rtm_map_port_add(int vrfid, int ifindex);
int rtm_map_port_del(int vrfid, int ifindex);
int	rtm_debug(void *cli, char *debug_opt, int debug_val, unsigned long);
int rtm_show_protocol_routes(void *cli, char *vrf_name, char *debug_opt);
int rtm_clear_routes_all(void *cli, char *vrf_name);
extern int rtm_cli_out(void *, const char *, ...);

int rtm_timer_init(int timeout, void *arg1, void *arg2);
int rtm_ipv4_init(void);

void rtm_map_timer_callback(void *arg1, void *arg2);

int rtm_static_route_add(char *vrf_name, unsigned long prefix, int prefix_len, char *,
			char *nh_vrf_name, int intf, unsigned long nh, 
			int distance, int metric, unsigned long tag);
int rtm_static_route_del(char *vrf_name, unsigned long prefix, int prefix_len, char *, 
			 char *nh_vrf_name, int intf, unsigned long nh, 
			 int distance, int metric, unsigned long tag);
char *ip_static_get_err_msg(int err_no);

char* rtm_ifg_get_name_by_ifindex(int ifindex, char* if_name);
int rtm_ip_route_config_write(void *);
void rtm_show_specific_net_route(char *vrf_name, unsigned long, int, char *);
void rtm_show_specific_route(char *vrf_name, unsigned long prefix, char *opt);

/* RTM 'show ip rtm dump' APIs */
void rtm_show_debug_info(void *ptr);
void rtm_show_port_info(void *cli);
void rtm_show_all_interfaces(void *cli);
void rtm_show_vrf_id_mapping (void *ptr, const char *vrf_name);

int rtm_rt_type_convert(int rtm_rt_type);
int	rtm_set_load_share(int);
int rtm_reset_load_share(void);
int rtm_change_nhop_res_type(int, char *, int);
int rtm_change_default_nhop_res(int, int);
int rtm_change_recur_levels(int, int, int);
void rtm_show_nhop_route_table(char *, int, int);
int rtm_set_router_id(unsigned long, unsigned long);
int rtm_unset_router_id(unsigned long, unsigned long);

int rtm_clear_route(char *vrf_name, unsigned int ip_address, unsigned int prefix_len);
int	rtm_shared_mem_init(void);
int rtm_create_rib_lock(void);

void rtm_add_del_mgmt_rt_from_nsm(unsigned long prefix, unsigned char prefix_len, int add);
void rtm_reprogram_mgmt_rt(int broadcast);

void rtm_process_redis_enable_msg(void *buffer, int enable);
void rtm_process_redis_ibgp_msg(void *buffer, int enable);
void rtm_process_redis_readd_msg(void *buffer);
void rtm_process_redis_default_rt_msg(void *buffer, int enable);
void rtm_process_redis_prefix_msg(void *buffer);
void rtm_process_client_reg_msg(void *buffer, int enable);
void rtm_process_rib_count_msg(void *buffer, int enable);

int rtm_get_static_rt_def_distance(void);
int rtm_get_static_rt_def_metric(void);
int rtm_get_default_load_share(void);
int rtm_get_default_recur_level(void);
int	rtm_mask_is_contiguous(unsigned long);
int	rtm_show_static_routes(void *, char *vrf_name);
int rtm_show_specific_net_static_route(char *vrf_name, unsigned long prefix, int prefix_len);
int	rtm_create_rib_mutex(void);
int	ipc_send_to_l3ss(char *, int);
void *rtm_zg(void);
void rtm_process_route_updates(void *);

void *rtm_shared_rib_alloc_mem(int size);

void rtm_ipv4_init_timer(void);

int rtm_reinstall_route(int ifindex);
void *rtm_get_msgq(void);

void rtm_remove_static_route_on_slot(int slot_id);
int rtm_get_slot_id_by_ifindex(int ifindex);

extern unsigned int rtm_counter_dy_route_add;
extern unsigned int rtm_counter_dy_route_del;
extern unsigned int rtm_counter_dy_route_update;

extern unsigned int rtm_counter_static_route_add;
extern unsigned int rtm_counter_static_route_del;

extern unsigned int rtm_counter_static_route;

extern unsigned int rtm_counter_fib_route_download;
extern unsigned int rtm_counter_fib_nexthop_download;

extern unsigned int rtm_counter_route_download_msg;
extern unsigned int rtm_counter_fib_download_msg;

extern unsigned int rtm_counter_fib_download_req_msg;

extern unsigned int rtm_counter_proto_reg;
extern unsigned int rtm_counter_proto_unreg;

extern unsigned int rtm_counter_if_num;
extern unsigned int rtm_counter_ip_num;
extern unsigned int rtm_counter_ip_addr_not_found;

#define RTM_COUNTER_DY_ROUTE_ADD				(rtm_counter_dy_route_add++)
#define RTM_COUNTER_DY_ROUTE_DEL				(rtm_counter_dy_route_del++)
#define RTM_COUNTER_DY_ROUTE_UPDATE				(rtm_counter_dy_route_update++)
#define RTM_COUNTER_STATIC_ROUTE_ADD			(rtm_counter_static_route_add++)
#define RTM_COUNTER_STATIC_ROUTE_DEL			(rtm_counter_static_route_del++)

#define RTM_COUNTER_STATIC_ROUTE_INC			(rtm_counter_static_route++)
#define RTM_COUNTER_STATIC_ROUTE_DEC			(rtm_counter_static_route--)
#define RTM_COUNTER_STATIC_ROUTE_LIMIT_REACHED	(rtm_counter_static_route >= 25000)

#define RTM_COUNTER_FIB_ROUTE_DOWNLOAD_INC		(rtm_counter_fib_route_download++)
#define RTM_COUNTER_FIB_ROUTE_DOWNLOAD_DEC		(rtm_counter_fib_route_download--)
#define RTM_COUNTER_FIB_NEXTHOP_DOWNLOAD_INC		(rtm_counter_fib_nexthop_download++)
#define RTM_COUNTER_FIB_NEXTHOP_DOWNLOAD_DEC		(rtm_counter_fib_nexthop_download--)

#define RTM_COUNTER_ROUTE_DOWNLOAD_MSG			(rtm_counter_route_download_msg++) 
#define RTM_COUNTER_FIB_DOWNLOAD_MSG			(rtm_counter_fib_download_msg++)

#define RTM_COUNTER_FIB_DOWNLOAD_REQ_MSG			(rtm_counter_fib_download_req_msg++)

#define RTM_COUNTER_PROTO_ENABLE_MSG				(rtm_counter_proto_reg++)
#define RTM_COUNTER_PROTO_DISABLE_MSG				(rtm_counter_proto_unreg++)

#define RTM_COUNTER_FIB_DOWNLOAD_REQ_MSG			(rtm_counter_fib_download_req_msg++)

void rtm_counter_show_all(void *cli);
void rtm_counter_clear_all(void);

void rtm_convert_ifp_to_if_status(void *ptr, RTM_IF_STATUS_UPDATE *if_status);
void rtm_convert_ifp_to_if_info(void *ptr, RTM_IF_INFO *if_info);
unsigned long rtm_get_ifindex_from_ifp(void *ptr);
unsigned short rtm_get_vrfindex_from_ifp(void *ptr);

int rtm_if_state_to_fib (unsigned char mode, unsigned long safi, unsigned long vrf_id, void *ifp);
int rtm_ipaddr_to_fib (unsigned char mode, unsigned long safi, unsigned long vrf_id, 
                        unsigned long ifindex, unsigned long ip_addr, 
                        unsigned char bitlen, unsigned char secondary);

long rtm_add_if_entry_to_tree(unsigned long vrf_id, unsigned long ifindex, void *ifp);
void rtm_remove_if_entry_from_tree(unsigned long ifindex);
int rtm_ipaddr_tree_add(unsigned long safi, unsigned long vrf_id, 
						unsigned long ifindex, unsigned long ipaddr, 
                        unsigned char prefix_len, unsigned char secondary);
int rtm_ipaddr_tree_del(unsigned long safi, unsigned long vrf_id, 
						unsigned long ifindex, unsigned long ipaddr, unsigned char prefix_len);

int rtm_if_debug_enabled();
void rtm_debug_uprintf(char const *cptr_fmt, ...);
void rtm_debug_init();

int rtm_get_local_mapped_id(void);
void rtm_local_mapped_id_init(void);
void rtm_cli_arg_set(int cmdCode, void *buf, 
					 unsigned int num_entries, unsigned int num_active, unsigned int num_del_entry,
					 unsigned int size, unsigned int flags);

int rtm_unicast_ip_addr_check(unsigned long ip_addr);
char *rtm_convert_ip_address_to_dot_format(char *to_store_dot_format_address,  unsigned long ip_address);


#define RTM_GET_IPADM_GW		0
#define RTM_SET_IPADM_GW	 	1
#define RTM_DEL_IPADM_GW	 	2   
void initIPadmGw(void);
void getSetIPadmGw(unsigned long *ip_gateway, int set);
int rtm_check_default_gw(void *rt);
unsigned int rtm_mgmt_get_rt_metric(void);
void rtm_mgmt_set_rt_metric(unsigned int metric);

int rtm_client_route_register(void* req);
int rtm_client_route_deregister(void* req);
void rtm_rt_reg_init(void);
int rtm_rt_reg(void* req);
int rtm_rt_dereg(void* req);
void schedule_rt_reg_walk();
int rtm_rt_reg_walk(void);


#define RTM_DEF_RT_OWNER_DHCP   0x01
#define RTM_DEF_RT_OWNER_STATIC 0x02
#define RTM_DHCP_RT_NOT_UPDATED 0
#define RTM_DHCP_RT_UPDATED     1
#define RTM_DHCP_RT_DUPLICATE   2
#define RTM_NSM_DEL_GW          0
#define RTM_NSM_ADD_GW          1
//rtm_dhcp_flag has bit0: current dhcp state, bit1: previous dhcp state (from NSM)
#define RTM_NSM_PREV_DHCP_STATUS_BIT    0x02
#define RTM_NSM_CURRENT_DHCP_STATUS_BIT 0x01
#define RTM_NSM_DHCP_STATUS_MASK        (RTM_NSM_PREV_DHCP_STATUS_BIT | RTM_NSM_CURRENT_DHCP_STATUS_BIT)
#define RTM_IS_DHCP_TRANSIT_TO_DISABLE  (((rtm_dhcp_flag & RTM_NSM_DHCP_STATUS_MASK) == 0x02))
#define RTM_IS_DHCP_TRANSIT_TO_ENABLE   (((rtm_dhcp_flag & RTM_NSM_DHCP_STATUS_MASK) == 0x01))
#define RTM_IS_DHCP_ENABLE              (((rtm_dhcp_flag & RTM_NSM_DHCP_STATUS_MASK) == 0x03))
#define RTM_IS_DHCP_DISABLE             (((rtm_dhcp_flag & RTM_NSM_DHCP_STATUS_MASK) == 0x00))
extern unsigned char rtm_dhcp_flag;
void rtm_mgmt_update_gateway(unsigned long *ip_gateway, int set, unsigned char dhcp_flag);
int rtm_update_default_route(unsigned long *ip_gateway, int set);
void *rtm_is_gw_default_route_nexthop(unsigned long gateway);

void rtm_raslog_wrapper (int msg_id, char const *cptr_fmt, ...);
int l3_is_license_enabled(void);
void rtm_testpoint(rtm_tp_type state);

#endif /*__OSPF_UTIL_H__*/
