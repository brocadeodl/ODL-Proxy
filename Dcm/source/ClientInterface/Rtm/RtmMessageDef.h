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

#ifndef __RTM_MESSAGE_DEF_H__
#define __RTM_MESSAGE_DEF_H__

#define RTM_DCM_CLIENT_NAME     "rtm"
#define RTM_DEFAULT_MAX_ROUTES 1200
#define RTM_SHOW_VRF_MAX_OUTPUT_SIZE 10000
#define RTM_SHOW_VRF_NUM_RECORDS  3

#include <stdbool.h>

typedef enum 
{
    RTM_MSG_CONFIG_RT_TYPE_EXPORT = 0,
    RTM_MSG_CONFIG_RT_TYPE_IMPORT,
    RTM_MSG_CONFIG_RT_TYPE_BOTH,
    RTM_MSG_CONFIG_RT_TYPE_UNKNOWN
}RtmMessageConfigRtType;

typedef enum
{
    RTM_MSG_ALL_CONFIG_SET,
    RTM_MSG_CONFIG_ROUTER_ID_SET,
    RTM_MSG_CONFIG_LOADSHARING_SET,
    RTM_MSG_CONFIG_NEXTHOP_SET,
    RTM_MSG_CONFIG_ENABLEDEFAULT_SET,
    RTM_MSG_CONFIG_NEXTHOPRECURSION_SET,
    RTM_MSG_CONFIG_ROUTER_ID_UNSET,
    RTM_MSG_CONFIG_LOADSHARING_UNSET,
    RTM_MSG_CONFIG_NEXTHOP_UNSET,
    RTM_MSG_CONFIG_ENABLEDEFAULT_UNSET,
    RTM_MSG_CONFIG_NEXTHOPRECURSION_UNSET,
    RTM_MSG_NH_STATIC_ROUTE_SET,
    RTM_MSG_IF_STATIC_ROUTE_SET,
    RTM_MSG_NH_STATIC_ROUTE_UNSET,
    RTM_MSG_IF_STATIC_ROUTE_UNSET,
    RTM_MSG_NH_VRF_SET,
    RTM_MSG_NH_VRF_UNSET,
    RTM_MSG_RD_SET,
    RTM_MSG_RD_UNSET,
    RTM_MSG_AF_IPV4_SET,
    RTM_MSG_AF_IPV4_UNSET,
    RTM_MSG_CONFIG_VRF_MODE_RT_SET,
    RTM_MSG_CONFIG_VRF_MODE_RT_UNSET,
    RTM_MSG_CONFIG_VRF_AF_IPV4_MODE_RT_SET,
    RTM_MSG_CONFIG_VRF_AF_IPV4_MODE_RT_UNSET,
    RTM_MSG_CONFIG_MAX_ROUTES_SET,
    RTM_MSG_CONFIG_MAX_ROUTES_UNSET

}RtmTransationTypes;

/* enum defines to mirror enumeration from brocade-rtm.yang
 * If you need to change this, please do so in both places. */
typedef enum
{
    RTM_IF_TYPE_TENGIGABIT   = 1,
    RTM_IF_TYPE_GIGABIT      = 2,
    RTM_IF_TYPE_PORTCHANNEL  = 3,
    RTM_IF_TYPE_VE           = 4,
    RTM_IF_TYPE_NULL         = 5,
    RTM_IF_TYPE_FORTYGIGABIT = 6
}RtmIfTypes;
    
#define RTM_CONFIG_DEFAULT_NH_RECUR         3
#define RTM_CONFIG_DEFAULT_LOADSHARING      8
#define RTM_STATIC_ROUTE_DEFAULT_METRIC     1
#define RTM_STATIC_ROUTE_DEFAULT_DISTANCE   1
#define RTM_STATIC_ROUTE_DEFAULT_TAG        0

#define RTM_MSG_CONFIG_ROUTER_ID_BIT        0x01
#define RTM_MSG_CONFIG_LOADSHARING_BIT      0x02
#define RTM_MSG_CONFIG_NEXTHOP_BIT          0x04
#define RTM_MSG_CONFIG_ENABLEDEFAULT_BIT    0x08
#define RTM_MSG_CONFIG_NEXTHOPRECURSION_BIT 0x10

/*RT related opcodes*/
#define RTM_MSG_CONFIG_VRF_MODE_RT_BIT      0x12

/*max route opcode*/
#define RTM_MSG_CONFIG_MAX_ROUTES_BIT       0x14
#define RTM_MSG_CONFIG_RD_BIT               0x18

#define RTM_IP_STRING_LEN                   32
#define RTM_IF_TYPE_STRING_LEN              32
#define RTM_IF_NAME_STRING_LEN              32
#define RTM_VRF_NAME_STRING_LEN             255 
#define RTM_RD_RT_MAX_LEN                   255

typedef enum vrfRtType_ {
    RTM_RT_TYPE_IMPORT = 0,
    RTM_RT_TYPE_EXPORT,
    RTM_RT_TYPE_BOTH,
    RTM_RT_TYPE_UNKNOWN
}vrfRtType_t;

typedef struct rtm_config_msg_ {
    char            vrfName[RTM_VRF_NAME_STRING_LEN];
    unsigned int    opCode;
    unsigned int    cfgBitMask;
    unsigned int    nhProto;
    unsigned int    nhDefault;
    unsigned int    nhRecur;
    unsigned int    loadSharing;
	char            routerID[RTM_IP_STRING_LEN+1];
    char            vrfRdRt[RTM_RD_RT_MAX_LEN];
    char            vrfRt[RTM_RD_RT_MAX_LEN];
    unsigned int    vrfRtType;
    bool            afIpv4Mode;
    unsigned int    maxRoutes;

} rtm_config_msg_t;

typedef struct rtm_static_route_msg_ {
    unsigned int    opCode;
    char            dest[RTM_IP_STRING_LEN+1];
    char            nhIp[RTM_IP_STRING_LEN+1];
    char            vrfCtxt[RTM_VRF_NAME_STRING_LEN+1];
    char            nextHopVrf[RTM_VRF_NAME_STRING_LEN+1];
    const char      *ifType;
    const char      *ifName;
    unsigned int    cost;
    unsigned int    distance;
    unsigned int    tag;
} rtm_static_route_msg_t;

typedef enum
{
	RTM_SHOW_ROUTE_MSG,
	RTM_SHOW_NH_MSG,
	RTM_SHOW_STATIC_ROUTE_MSG,
	RTM_SHOW_MSG_OUTPUT,

	RTM_SHOW_VRF_NAME,
	RTM_SHOW_VRF,
	RTM_SHOW_VRF_DETAIL
        
}RtmShowCmdCode_t;


#define RTM_SHOW_OUT_MSG_PRINT_NUM_ENTRIES		0x00000001
typedef struct rtm_show_out_msg_t_ {
	RtmShowCmdCode_t cmdCode;
	unsigned int size;
  unsigned int size_of_record;
	unsigned int num_records;
	unsigned int num_active;
	unsigned int num_del_entry;
	unsigned int flags;
	int buf_is_string;
	int data_pending;
	int handle;
  int retCode;  
  unsigned char       more_data;
  char last_vrf_name[RTM_VRF_NAME_STRING_LEN+1];
	// Specifies which data struct has been sent back
	void *contents;
	char *buf;
} rtm_show_out_msg_t;

/*
 * Show command message for backend
*/
typedef struct rtm_show_in_msg_ {
    RtmShowCmdCode_t cmdCode;
	// parsing CLI commands in tranditional argc, argv
	unsigned int argc;
	const char *argv[24];
	char vrf_name[RTM_VRF_NAME_STRING_LEN+1];/*This is used only for vrf show cmds*/
} rtm_show_in_msg_t;


#define RTM_IF_SHORT_NAME_LEN		15
typedef struct rtm_show_ip_route_info_t_ {
	unsigned char ifname[RTM_IF_SHORT_NAME_LEN];
	unsigned int outgoing_interface;
	unsigned int next_hop_router_ip_address;
}rtm_show_ip_route_info_t;

typedef struct rtm_show_ip_rib_entry_t_ {
	unsigned int len;
	unsigned int metric;
	unsigned char distance;
	char route_type[4];
	rtm_show_ip_route_info_t route_info[1];
}rtm_show_ip_rib_entry_t;

typedef struct rtm_show_ip_route_entry_t_ {
	unsigned int len;
	unsigned int route_number;
	unsigned int timestamp;
	unsigned int ip_address;
	unsigned char prefix_len;
	rtm_show_ip_rib_entry_t rib[1];
}rtm_show_ip_route_entry_t;

typedef struct rtm_show_ip_nh_entry_t_ {
	unsigned int len;
	unsigned int next_hop_id;
	unsigned int timestamp;
	unsigned int delete_timer;
	unsigned int ref_cnt;	
	unsigned int fwd_ref_cnt;
	rtm_show_ip_route_info_t route_info[1];
}rtm_show_ip_nh_entry_t;

typedef struct rtm_show_ip_static_route_entry_t_ {
	unsigned int ip_address;
	unsigned char prefix_len;
	unsigned char installed;
	unsigned int next_hop_router_ip_address;
	unsigned char ifname[RTM_IF_SHORT_NAME_LEN];
	unsigned char distance;
	unsigned int metric;
	unsigned int tag;
}rtm_show_ip_static_route_entry_t;

#endif /*__RTM_MESSAGE_DEF_H__*/
