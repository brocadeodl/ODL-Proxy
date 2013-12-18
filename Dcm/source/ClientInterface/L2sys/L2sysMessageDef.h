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

#ifndef __L2SYS_MESSAGE_DEF_H__
#define __L2SYS_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"

#define L2SYS_VCS_MIN_MAC_AGING		60
#define L2SYS_VCS_MAX_MAC_AGING		100000



/* Tag Names */
/* These enums defined the L2sys payload record types that are passed from the L2sys daemon */
/* to the L2sys local plug-in.  The local plug-in will copy the data to a DCM buffer */
/* when it calls function addBuffer. */
typedef enum
	{
	  L2SYSSHOWMAC,
	  L2SYSSHOWAGINGTIME,
	  L2SYSSHOWCOUNT,
	  L2SYSSHOWLINECARD,
	  L2SYSSHOWMACPORTPROFILE,
	  L2SYSSHOWRBRIDGEMACTABLE
	} L2sysMessageBufferId;

typedef enum
	{
	  L2SYS_PLATFORM_INDEPENDENT,
	  L2SYS_EANV_PLATFORM,
	  L2SYS_WLV_PLATFORM
	} L2sysPlatformInfo;

typedef enum
	{
	  L2SYS_SET_OP,
	  L2SYS_DELETE_OP,
	  L2SYS_UNPDATE_OP,
      L2SYS_NO_LINECARD
	}L2sysTransationTypes;

/* show l2sys cmd code */
typedef enum
    {
        L2SYSSHOW_MAC_STATIC_ALL,
        L2SYSSHOW_MAC_STATIC_ADDRESS,
        L2SYSSHOW_MAC_STATIC_ONEGIG,
        L2SYSSHOW_MAC_STATIC_TENGIG,
        L2SYSSHOW_MAC_STATIC_FORTYGIG,
        L2SYSSHOW_MAC_STATIC_PO,
        L2SYSSHOW_MAC_STATIC_VLAN,
        L2SYSSHOW_MAC_DYNAMIC_ALL,
        L2SYSSHOW_MAC_DYNAMIC_ADDRESS,
        L2SYSSHOW_MAC_DYNAMIC_ONEGIG,
        L2SYSSHOW_MAC_DYNAMIC_TENGIG,
        L2SYSSHOW_MAC_DYNAMIC_FORTYGIG,
        L2SYSSHOW_MAC_DYNAMIC_PO,
        L2SYSSHOW_MAC_DYNAMIC_VLAN,
		L2SYSSHOW_MAC_AGING_TIME,
        L2SYSSHOW_MAC_VLAN,
        L2SYSSHOW_MAC_ADDRESS,
        L2SYSSHOW_MAC_NONE,
        L2SYSSHOW_MAC_INTERFACE_ONEGIG,
        L2SYSSHOW_MAC_INTERFACE_TENGIG,
        L2SYSSHOW_MAC_INTERFACE_FORTYGIG,
        L2SYSSHOW_MAC_INTERFACE_PO,
        L2SYSSHOW_MAC_COUNT,
        L2SYSSHOW_MAC_COUNT_ADDRESS,
        L2SYSSHOW_MAC_COUNT_DYNAMIC_ALL,
        L2SYSSHOW_MAC_COUNT_DYNAMIC_ADDRESS,
        L2SYSSHOW_MAC_COUNT_DYNAMIC_ONEGIG,
        L2SYSSHOW_MAC_COUNT_DYNAMIC_TENGIG,
        L2SYSSHOW_MAC_COUNT_DYNAMIC_FORTYGIG,
        L2SYSSHOW_MAC_COUNT_DYNAMIC_PO,
        L2SYSSHOW_MAC_COUNT_STATIC,
        L2SYSSHOW_MAC_COUNT_VLAN,
        L2SYSSHOW_MAC_COUNT_INTERFACE_ONEGIG,
        L2SYSSHOW_MAC_COUNT_INTERFACE_TENGIG,
        L2SYSSHOW_MAC_COUNT_INTERFACE_FORTYGIG,
        L2SYSSHOW_MAC_COUNT_INTERFACE_PO,
        L2SYSSHOW_MAC_LINECARD,
        L2SYS_CLEAR_DYNAMIC,
        L2SYS_CLEAR_DYNAMIC_ADDRESS,
        L2SYS_CLEAR_DYNAMIC_INTERFACE_PO,
        L2SYS_CLEAR_DYNAMIC_INTERFACE_ONEGIG,
        L2SYS_CLEAR_DYNAMIC_INTERFACE_TENGIG,
        L2SYS_CLEAR_DYNAMIC_INTERFACE_FORTYGIG,
		L2SYS_CLEAR_DYNAMIC_VLAN,
        L2SYS_CLEAR_FABRIC,
        L2SYS_CLEAR_FABRIC_VLAN,
		L2SYSSHOW_MAC_PORT_PROFILE,
		L2SYSSHOW_MAC_PORT_PROFILE_DYNAMIC,
		L2SYSSHOW_MAC_PORT_PROFILE_STATIC,
		L2SYSSHOW_MAC_PORT_PROFILE_VLAN,
		L2SYSSHOW_MAC_PORT_PROFILE_ADDRESS,
		L2SYSSHOW_MAC_PORT_PROFILE_COUNT,
		L2SYSSHOW_MAC_DYNAMIC_FROM_RBRIDGE,
		L2SYSSHOW_MAC_DYNAMIC_FROM_RBRIDGE_FOR_RBRIDGE
    } L2sysShowIntfCmdCode;

typedef enum
	{
		L2SYSSHOW_PORT_SECURITY,
		L2SYSSHOW_PORT_SECURITY_ADDRESSES,
		L2SYSSHOW_PORT_SECURITY_INTERFACE,
		L2SYSSHOW_PORT_SECURITY_OUI
	} L2sysShowPortSecCmdCode;

#define L2SYS_PHYSICAL_INTERFACE 1
#define L2SYS_GIGAETHERNET         1  /* same as INTERFACE_TYPE_GIGAETHERNET*/
/*To be changed according to yang values */
#define L2SYS_TENGIGAETHERNET      2  /* same as INTERFACE_TYPE_TENGIGAETHERNET*/
#define L2SYS_FORTYGIGAETHERNET      3  /* same as INTERFACE_TYPE_FORTYGIGAETHERNET*/
#define L2SYS_PORT_CHANNEL         4  /* same as INTERFACE_TYPE_PORTCHANNEL */

typedef struct L2sysConfig_InputMsg_t_ {

    char if_name[64+1];
    unsigned int if_index;
    char Mac[16+1];
    unsigned int forward;
    unsigned int vlan;
    unsigned int vlan_id;
    unsigned int slotid;
    unsigned int bridge;
    unsigned int interface_type;
    unsigned int cmdCode;
    unsigned int port_channel_id;
} L2sysConfig_InputMsg_t;

/* L2sys show mac output Msg */
typedef struct l2sys_show_mac_output_msg_ {
	unsigned int rbridge_id;
    char if_name[256];
    char Mac[16];
    char type[64];
    char state[64];
    int vlan_id;
    int appmFlag;
	unsigned int is_last;
	char isprofiled[20];
} l2sys_show_mac_output_msg_t;

typedef struct L2sysShow_InputMsg_t_ {

    char if_name[64+1];
    char interface[64+1];
    char Mac[16+1];
    unsigned int vlan_id;
    unsigned int bridge;
    unsigned int type;
    unsigned int cmdCode;
    unsigned int interface_type;
	unsigned int ifindex;
	unsigned int port_channel_id;
	unsigned int port_channel_type;
	unsigned int isprofiledflag;
	int page_idx;
	/* Input fields related to pagination */
	unsigned int  is_first_request;
	l2sys_show_mac_output_msg_t last_record;
} L2sysShow_InputMsg_t;

typedef struct l2sys_port_security_output_msg_ {
    char if_name[64+1];
    unsigned int max_secure_count;
    unsigned int current_dynamic_count;
    unsigned int current_static_count;
	unsigned char is_violated;
	char action[10];
	unsigned int cfgd_ouis;
	unsigned char is_sticky;
} l2sys_port_security_output_msg_t;

typedef struct l2sys_port_security_interface_output_msg_ {
    unsigned char is_enabled;
    char port_status[30];	/* up/down (security violation) */
	char violation_mode[10];
	unsigned char is_violated; /* 0 or 1 */
	unsigned int max_macs;
	unsigned char is_sticky;
	unsigned int total_mac_addr;
	unsigned int cfgd_mac_addr;
	char violation_time[40];
	unsigned int shutdown_time;
	unsigned int ouis;
} l2sys_port_security_interface_output_msg_t;

typedef struct l2sys_port_security_oui_output_msg_ {
    unsigned int cfgd_ouis;	/* Number of configured ouis */
    char ouis[4][20];	/* configured ouis */
} l2sys_port_security_oui_output_msg_t;

typedef struct L2sysShow_PortSecurity_InputMsg_t_ {
    char if_name[64+1];
    unsigned int cmdCode;
    unsigned int interface_type;
	/* Input fields related to pagination */
	unsigned int  is_first_request;
	l2sys_show_mac_output_msg_t last_record;
} L2sysShow_PortSecurity_InputMsg_t;

typedef struct l2sys_timeout_global_config_msg_ {
	int	timeout;
} l2sys_global_timeout_config_msg_t;

typedef struct l2sys_global_count_config_msg_ {
	int	dynamic_fdb_count;
	int	static_fdb_count;
	int	fpma_fdb_count;
	int internal_fdb_count;
	int	fdb_count;
} l2sys_global_count_config_msg_t;

typedef struct l2sys_global_linecard_config_msg_ {
	unsigned int rbridge_id;
	unsigned int	fid;
	char mac[16];
	char lrn_port[10];
	char is_mgid;
	char interface[64];
	int	xgid;
	int grp_id;
	char age[3];
	char trill_fields[10];
} l2sys_global_linecard_config_msg_t;

typedef struct l2sys_show_msg_ {
#define L2SYS_NO_MORE_ELEMENTS 0
#define L2SYS_MORE_ELEMENTS 1
	unsigned int system_fdb_count;
    unsigned int fdb_count;
    unsigned int num_records;
    unsigned int record_type;
	int more_elements;
	L2sysPlatformInfo platform_type;
	int page_idx;
    union {
        l2sys_show_mac_output_msg_t info[1];
		l2sys_global_timeout_config_msg_t timeout[1];
		l2sys_global_linecard_config_msg_t linecard[1];
		l2sys_global_count_config_msg_t count[1];
		l2sys_port_security_output_msg_t port_security[1];	/* show port-security */
		l2sys_port_security_interface_output_msg_t port_sec_intf[1];	/* show port-security interface */
		l2sys_port_security_oui_output_msg_t port_sec_oui[1];	/* show port-security oui */
    } records;
} l2sys_show_msg_t;

#if 0
typedef struct l2sys_show_port_security_msg_ {
#define L2SYS_PORT_SECURITY_NO_MORE_ELEMENTS 0
#define L2SYS_PORT_SECURITY_MORE_ELEMENTS 1
	//unsigned int system_fdb_count;
    unsigned int fdb_count;
    unsigned int num_records;
    unsigned char record_type;
	unsigned char more_elements;
    union {
        l2sys_show_mac_output_msg_t info[1];	/* show port-security addresses */
		l2sys_port_security_output_msg_t port_security[1];	/* show port-security */
		l2sys_port_security_interface_output_msg_t port_sec_intf[1];	/* show port-security interface */
		l2sys_port_security_oui_output_msg_t port_sec_oui[1];	/* show port-security oui */
    } records;
} l2sys_show_port_security_msg_t;
#endif

typedef struct L2sysClear_InputMsg_
{
    char if_name[64+1];
    char Mac[16+1];
    unsigned int vlan_id;
    unsigned int bridge;
    unsigned int type;
    unsigned int cmdCode;
    unsigned int ifindex;
    unsigned int interface_type;
    unsigned int port_channel_id;
} L2sysClear_InputMsg_t;

typedef union l2sys_buffer_num_u {
	struct {
		uint16_t   mapped_id;
		uint16_t   tag;
	} buff;
	uint32_t   num;
} l2sys_buffer_num_t;

	static inline uint16_t
l2sys_get_buffer_tag (uint32_t  buff_num)
{
	return (((l2sys_buffer_num_t*) &buff_num)->buff.tag);
}

	static inline u_int16_t
l2sys_get_buffer_mapped_id(uint32_t  buff_num)
{
	return (((l2sys_buffer_num_t*) &buff_num)->buff.mapped_id);
}

 typedef enum  {
   
   L2_APPM_NO_FLAGS = 0,
   L2_APPM_MAC_PROFILED,
   L2_APPM_MAC_APPLY_TAGGED,
   L2_APPM_MAC_APPLY_UNTAGGED,
   L2_APPM_PORT_NOT_PROFILED,
   L2_APPM_MAC_NOT_PROFILED,
   L2_APPM_MAC_NO_FORWARD,
   L2_APPM_MAC_CONFLICT,
   L2_APPM_MAC_NOT_ACTIVE,
   L2_APPM_MAC_MAX_FLAGS
} L2AppmEnum;

#endif
