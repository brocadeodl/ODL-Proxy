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

#ifndef __NSMDIPINTF_H
#define __NSMDIPINTF_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "ClientInterface/DceWaveClientMessage.h"

#define NSM_DCM_CLIENT_NAME "nsm"
#define NSM_MTU_DEF_VAL     2500
#define NSM_MTU_PORTMIRROR_DESTINATION_VAL     9216
#define NSM_MINLINKS_DEF_VAL     1
#define NSM_IP_MTU_DEF_VAL  1500
#define NSM_IP_ARP_AGING_TIMEOUT_DEF_VAL  240

#define NSM_OSPF_PASSIVE 	1 << 0
#define NSM_OSPF_IGNORE		1 << 1
#define NSM_NO_OSPF_OPT		0

typedef enum {
    NSM_OPENFLOW_DEF_STATUS,
    NSM_OPENFLOW_ENABLE_STATUS
} nsm_openflow_status;

typedef enum {
    NSM_OPENFLOW_DEF_MODE,
    NSM_OPENFLOW_HYBRID_MODE
} nsm_openflow_mode;

typedef enum {
    /* Sflow configure */
    NSM_MSG_IP_ADDRESS,
   	NSM_MSG_IP_MTU,
	NSM_MSG_NO_IP_ADDRESS,
	NSM_MSG_SEC_IP_ADDRESS,
	NSM_MSG_MGMT_IP_ADDRESS,
	NSM_MSG_NO_SEC_IP_ADDRESS,
	NSM_MSG_NO_SEC_IP_ADDRESS_ALL,
	NSM_MSG_NO_IP_MTU,
	NSM_MSG_IP_SWITCHPORT,
	NSM_MSG_NO_IP_SWITCHPORT,
	NSM_MSG_IP_PROXY_ARP,
	NSM_MSG_NO_IP_PROXY_ARP,
	NSM_MSG_ARP_AGEING_TIMEOUT,
	NSM_MSG_NO_ARP_AGEING_TIMEOUT,
    NSM_MSG_SET_NODE_PO,
    NSM_MSG_UNSET_NODE_PO,
    NSM_MSG_VE_SET,
    NSM_MSG_VE_UNSET,
    NSM_MSG_INTF_LOOPBACK_SET,
    NSM_MSG_INTF_LOOPBACK_UNSET,
    NSM_MSG_INTF_STATUS,
    NSM_MSG_INTF_NO_STATUS,
    NSM_MSG_INTF_SHUTDOWN,
    NSM_MSG_INTF_NO_SHUTDOWN,
    NSM_MSG_IP_DIRECTED_BROADCAST,
    NSM_MSG_NO_IP_DIRECTED_BROADCAST,
    NSM_MSG_L3READY_MO,
    NSM_MSG_NO_L3READY_MO,

    NSM_MSG_CMD_CODE
} nsm_ip_cmd_code;

typedef enum {
	DCM_NSM_ERR_INTERNAL_INTERFACE = 1,
	DCM_NSM_API_ERR_IP_INVLD_HOST,
	DCM_NSM_API_ERR_IP_OVERLAP,
	DCM_NSM_API_ERR_IP_INVLD_MASK,
	DCM_NSM_API_ERR_IP_OVERLAP_MANAGE,
	DCM_NSM_API_ERR_UPDATE_MAC_IN_HW,
	DCM_NSM_API_ERR_ROUTERPORT_CREATE,
	DCM_NSM_API_SET_ERR_ADDRESS_NOT_EXIST,
	DCM_NSM_API_SET_ERR_NOT_SECONDARY_IP_ADDR,
	DCM_NSM_API_SET_ERR_SECONDARY_IP_ADDR_EXISTS,
	DCM_NSM_API_ERR_ROUTERPORT_DELETE,
	DCM_NSM_API_ERR_FCF_ON_IP_VLAN,
	DCM_NSM_API_ERR_IP_INVLD_IF,
	DCM_NSM_ERR_DCM_INTERNAL_INTERFACE,
	DCM_NSM_API_SET_ERR_NO_SUCH_INTERFACE,
	DCM_NSM_API_ERR_IP_OVERLAP_STATIC,
	DCM_NSM_API_ERR_SVI_CREAT,
	DCM_NSM_API_SET_ERR_NOT_PRIMARY_IP_ADDR,
	DCM_NSM_API_SET_ERR_MASTER_NOT_EXIST,
	DCM_NSM_API_SET_ERR_INVALID_IPV4_ADDRESS,
	DCM_NSM_API_SET_ERR_CANT_SET_ADDRESS_ON_P2P,
	DCM_NSM_API_SET_ERR_CANT_SET_SECONDARY_FIRST,
	DCM_NSM_API_SET_ERR_MAX_ADDRESS_LIMIT,
	DCM_NSM_API_SET_ERR_CANT_CHANGE_PRIMARY,
	DCM_NSM_API_SET_ERR_CANT_CHANGE_SECONDARY,
	DCM_NSM_API_SET_ERR_ADDRESS_OVERLAPPED,
	DCM_NSM_API_SET_ERR_CANT_SET_ADDRESS,
	DCM_NSM_API_SET_ERR_MUST_DELETE_SECONDARY_FIRST,
	DCM_NSM_API_SET_ERR_REAL_INTERFACE_ACT,
	DCM_NSM_API_ERR_AGG_L3,
	DCM_NSM_API_ERR_L2_L3,
	DCM_NSM_API_ERR_DEFAULT_VE_NOT_ALLOWED,
	DCM_NSM_API_ERR_SPAN_L3,
	DCM_NSM_API_SET_ERR_INVALID_PORT_ROLE_MODE,
        DCM_NSM_API_VRF_SET_INVLD_CONFIG_FOR_AGG_MEM
}ip_err_codes;

typedef enum{
	PHY_INT_GI,
	PHY_INT_TE,
	PHY_INT_FO,
	PO_INT,
	VLAN_INT,
	PHY_INT,
    LOOPBACK_INT,
	MGMT_INT
}intftype;

typedef enum {
    NSM_IP_ADDR_NO_MATCH = 0,
    NSM_IP_ADDR_PREFIX_MATCH,
    NSM_IP_ADDR_EXACT_MATCH
} NSMIntfIpMatch;

typedef enum{
	IP_FALSE,
	IP_TRUE
}bool_type;

typedef struct ip_global_config_msg_ {
    uint32_t if_id;
	char if_name[63];
    uint32_t ip_mtu;
    uint32_t opcode;
    uint32_t intf_type;
	char 	ip_address_mask[19];
	unsigned char  sec;	 	
	unsigned char  ospf_opt;
	unsigned char  status;	 	
	uint32_t arp_timeout;
	unsigned char proxy_arp;	
    unsigned char switchport;
    unsigned char directed_broadcast;
} nsm_ip_global_config_msg_t;


#endif
