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

#ifndef __L3SS_MESSAGE_DEF_H__
#define __L3SS_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"

#define L3SS_DCM_IF_NAME_LEN	20
#define MAX_CHAR_PER_LINE       80
#define MAX_IOCTLS_PER_MESSAGE  1000
#define MAX_IP_PREFIX_LENGTH	19
#define MAX_IP_ADDRESS_LENGTH   16
#define MAC_ADDRESS_LENGTH      16
#define MAX_VRF_NAME_LENGTH     256

#define IP_ADDRESS_PREFIX_MASK  0xFF

typedef enum {
	L3SS_CLEAR_IN_PROGRESS = 3000,    
	L3SS_UNKNOWN_INTERNAL_ERROR
}l3ss_error_code;

typedef enum {
    L3SS_SHOW_FIB,
    L3SS_SHOW_FIB_IP,
    L3SS_SHOW_FIB_IP_MASK,
    L3SS_SHOW_FIB_NEXTHOP,
    L3SS_SHOW_FIB_NEXTHOP_IP,
    L3SS_SHOW_FIB_NEXTHOP_IP_MASK,
    L3SS_CLEAR_FIB,
    L3SS_CLEAR_FIB_IP,
    L3SS_CLEAR_FIB_IP_MASK,
    L3SS_CLEAR_FIB_ARP,
    L3SS_CLEAR_FIB_ARP_IP,
    L3SS_ARP_SHOW,
    L3SS_ARP_SHOW_IP,
    L3SS_ARP_SHOW_VLAN_IF_CONFIG,
	L3SS_ARP_SHOW_PO_IF_CONFIG,
	L3SS_ARP_SHOW_TE_IF_CONFIG,
	L3SS_ARP_SHOW_GI_IF_CONFIG,
	L3SS_ARP_SHOW_FO_IF_CONFIG,
    L3SS_ARP_SHOW_MAC_CONFIG
} l3ss_cmd_code;

typedef  enum {
    SHOW_IP_FIB_TYPE,
    SHOW_IP_FIB_NH_TYPE,
    CLEAR_IP_FIB_TYPE,
    L3SS_ARP_SHOW_TYPE
}l3ss_record_type;

typedef struct {
    int cmd;
    char ip_address[MAX_IP_ADDRESS_LENGTH];
    char ip_address_mask[MAX_IP_PREFIX_LENGTH];
    char if_name[L3SS_DCM_IF_NAME_LEN];
    char mac[MAC_ADDRESS_LENGTH];
    char vrf_name[MAX_VRF_NAME_LENGTH];
    int last_printed;
}l3ss_show_in_msg_t;

typedef struct {
    int cmd;
    int slot;
    int chip;
    int last_printed;
}l3ss_debug_lpm_in_msg_t;

typedef struct {
    int cmd;
    char ip_address[MAX_IP_ADDRESS_LENGTH];
    char ip_address_mask[MAX_IP_PREFIX_LENGTH];
    char vrf_name[MAX_VRF_NAME_LENGTH];
}l3ss_clear_in_msg_t;

typedef struct {
    char  ip_address[MAX_IP_ADDRESS_LENGTH];
    char  mac_address[MAC_ADDRESS_LENGTH];
    char  l3_ifindex[L3SS_DCM_IF_NAME_LEN];
    char  is_resolved[10];
    uint8_t type:1;
}route_arp_rec_t;

typedef struct {
    char dest_pref[MAX_IP_PREFIX_LENGTH];
    char next_hop[MAX_IP_ADDRESS_LENGTH];
    char if_name[L3SS_DCM_IF_NAME_LEN];
    char type[15];
    int is_resolved;
    int   ref_cnt;
    int   rtm_nh_id;
    uint32_t ip_address;
	uint16_t next_hop_flag;
}route_rec_t;

typedef struct {
    char  ip_address[MAX_IP_ADDRESS_LENGTH];
    uint32_t  route_id;
    uint32_t  nh_id;
    char      prefix_len;
}debug_lpm_rec_t;

typedef struct {
    int cmd;
    int num_arps;
    int num_routes;
    int num_records;
    int record_type;
    route_rec_t rec[1];
}l3ss_clear_out_msg_t;

typedef struct {
    int cmd;
    int num_arps;
    int num_records;
    int record_type;
    unsigned char more_elements;
    uint16_t last_printed;
    route_arp_rec_t rec[1];
}l3ss_show_out_arp_msg_t;

typedef struct {
    int cmd;
    int num_arps;
    int num_routes;
    int num_records;
    int record_type;
    unsigned char more_elements;
    uint16_t last_printed;
    route_rec_t rec[1];
}l3ss_show_out_msg_t;

typedef struct {
    int cmd;
    int num_records;
    unsigned char more_elements;
    uint16_t  last_printed;
    debug_lpm_rec_t rec[1];
}l3ss_debug_lpm_out_msg_t;

typedef struct {
    int cmd;
    int num_arps;
    int num_routes;
    int num_records;
    int record_type;
    int num_lines;
    char nh_hw_rec[MAX_IOCTLS_PER_MESSAGE][MAX_CHAR_PER_LINE];
    route_rec_t rec[1];
}l3ss_show_out_msg_nh_t;

#endif	/* __MSTP_MESSAGE_DEF_H__ */
