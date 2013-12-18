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

#ifndef __ARP_MESSAGE_DEF_H__
#define __ARP_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"

#define ARP_DCM_IF_NAME_LEN	20
#define ARP_MAC_ADDRESS_LEN 16
#define ARP_VRF_NAME_LEN    256

#define ARP_DCMP_ARP_REC_IP_STR_SIZE	16
#define ARP_DCMP_ARP_REC_MAC_STR_SIZE	16
#define ARP_DCMP_ARP_REC_TYPE_STR_SIZE	8
#define ARP_DCMP_ARP_REC_IF_NAME_STR_SIZE 20	
#define ARP_DCMP_ARP_REC_IS_RESOLVED_STR_SIZE	10
#define ARP_DCMP_ARP_REC_TIME_STR_SIZE	20

typedef union arp_mgmt_buffer_num_ {
    struct {
        unsigned short   mapped_id;
        unsigned short   tag;
    } buff;
    unsigned int num;
} arp_mgmt_buffer_num_t;
    
    
static inline unsigned short
arp_get_buffer_mapped_id (unsigned int  buff_num)
{
    return (((arp_mgmt_buffer_num_t*) &buff_num)->buff.mapped_id);
}

static inline unsigned short
arp_get_buffer_tag (unsigned int  buff_num)
{
    return (((arp_mgmt_buffer_num_t*) &buff_num)->buff.tag);
}


typedef enum {
    ARP_MEMORY_ALLOCATION_FAILED = 2000,
    ARP_DCM_API_MSG_ERROR,
    ARP_DCM_LINUX_FILEDOESNOT_EXIST,
    ARP_L3_COMMANDS_NOT_ALLOWED_ON_L2_INTERFACE,
    ARP_L3_COMMANDS_NOT_ALLOWED_ON_PO_INTERFACE,
	ARP_INTERFACE_DOES_NOT_EXIST,
	ARP_INTERFACE_IS_LOOPBACK,
	ARP_UNKNOWN_INTERNAL_ERROR,
    ARP_MAPPED_ID_IFNAME_MISMATCH,
    ARP_INVALID_COMMAND_ERROR,
    ARP_INVALID_IF_TYPE_ERROR,
    ARP_INVALID_RBRIDGE_ERROR
}arp_error_code;

/*  Enums for the interface that are supported for the arp Config command*/
typedef enum {
	CONFIG_ARP_PORTCHANNEL,
	CONFIG_ARP_GIGABIT,
	CONFIG_ARP_TENGIGABIT,
	CONFIG_ARP_FORTYGIGABIT,
	CONFIG_ARP_VLAN
} arp_config_code;

typedef enum {
	CLEAR_ALL_ARPS,
	CLEAR_ALL_ARPS_NOREFRESH,
	CLEAR_ARPS_VLAN_NOREFRESH,
	CLEAR_ARPS_VLAN,
	CLEAR_ARPS_TE_NOREFRESH,
	CLEAR_ARPS_TE,
	CLEAR_ARPS_GI_NOREFRESH,
	CLEAR_ARPS_GI,
	CLEAR_ARPS_FO_NOREFRESH,
	CLEAR_ARPS_FO,
	CLEAR_ARPS_PO_NOREFRESH,
	CLEAR_ARPS_PO,
	CLEAR_ARPS_IP_NOREFRESH,
	CLEAR_ARPS_IP,	
	CLEAR_ARPS_MAC_NOREFRESH,
	CLEAR_ARPS_MAC	
} arp_cmd_code;


typedef enum {
    SHOW_ARP,
    SHOW_ARP_SUMMARY,
    SHOW_ARP_STATIC_CONFIG,
    SHOW_ARP_DYNAMIC_CONFIG,
    SHOW_ARP_VLAN_IF_CONFIG,
	SHOW_ARP_PO_IF_CONFIG,
	SHOW_ARP_TE_IF_CONFIG,
	SHOW_ARP_GI_IF_CONFIG,
	SHOW_ARP_FO_IF_CONFIG,
    SHOW_ARP_IP_CONFIG,
    SHOW_ARP_MAC_CONFIG,
    SHOW_ARP_SKIP_COUNT,
    SHOW_ARP_IP_MASK,
    SHOW_ARP_DEFAULT_CONFIG
} arp_show_cmd_code;


typedef struct {
    char ip[ARP_DCMP_ARP_REC_IP_STR_SIZE];
    char mac[ARP_DCMP_ARP_REC_MAC_STR_SIZE];
    char type[ARP_DCMP_ARP_REC_TYPE_STR_SIZE];
	char if_name[ARP_DCMP_ARP_REC_IF_NAME_STR_SIZE];
	char is_resolved[ARP_DCMP_ARP_REC_IS_RESOLVED_STR_SIZE];
    char time[ARP_DCMP_ARP_REC_TIME_STR_SIZE];
}dcm_arp_rec;

typedef struct {
	int  dynamic_arp_count;
	int  static_arp_count;
	int  size;
	int  rec_count;
	int  last_printed;
	unsigned char  more_elements;
	dcm_arp_rec rec[1];
}arp_show_out_msg_t;

typedef struct {
    int cmd;
    char if_name[ARP_DCM_IF_NAME_LEN];
    char vrf_name[ARP_VRF_NAME_LEN];
    unsigned char static_arps;
    unsigned char summary;
    unsigned char dynamic;
    char ip[16];
    char mac[15];
    char ip_mask[19];
    int skip_count;
    int last_printed;
}arp_show_in_msg_t;

typedef struct {
	int cmd;
	char if_name[ARP_DCM_IF_NAME_LEN];
  char vrf_name[ARP_VRF_NAME_LEN];
	char ip_address[16];
	char mac_address[ARP_MAC_ADDRESS_LEN];
}arp_clear_msg_t;

typedef struct {
	char ip_address[16];
	int noflag;
}arp_debug_msg_t;

typedef struct {
    int system_max;
}arp_config_msg_t;

typedef struct {
	char ip_address[16];
	char mac_address[ARP_MAC_ADDRESS_LEN];
	char if_name[ARP_DCM_IF_NAME_LEN];
	int interface_type;
	int interface_id;
	int negation;
    char vrf_ctxt[ARP_VRF_NAME_LEN];
}arp_static_config_msg_t;

#endif	/* __MSTP_MESSAGE_DEF_H__ */
