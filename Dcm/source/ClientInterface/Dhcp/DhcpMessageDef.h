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

#ifndef __DHCP_MESSAGE_DEF_H__
#define __DHCP_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"

#define DHCPREL_PLUGIN_LOCAL_VERSION 0

#define DHCP_MAX_IFNAME_STR 20
#define DHCP_MAX_IPV4ADDR_STR 16
#define DHCP_MAX_VRFNAME_STR 256

/* Input structure for dhcp relay address configure/unconfigure */
typedef struct {
	int cmd; /* possible values: IPHELP_CONFIG_DRADDR (enum iphelper_cmd_t) */
	char ip_address[DHCP_MAX_IPV4ADDR_STR]; /* DHCP server IP address */
	char if_name[DHCP_MAX_IFNAME_STR];  /* valid if interface_type indicates a phy i/f */
	int interface_type;  /* uses enum InterfaceType defined in ClientInterface/InterfaceCommon.h */
	int interface_id;    /* Vlan Id - valid if interface_type is IF_TYPE_VE */
	int negation;  /* indicates whether command is for configure or unconfigure */     
} iphelp_dhcprelay_config_in_msg_t;


/* Input structure for show relay address for specific interface */
typedef struct {
	int cmd;
	char if_name[DHCP_MAX_IFNAME_STR];
	int interface_type;
	int interface_id;
} iphelp_dhcprelay_show_intf_in_msg_t;


/* Input structure for clear relay statistics [ip-address] command */
typedef struct {
	int cmd;
	char ip_address[DHCP_MAX_IPV4ADDR_STR];
} iphelp_dhcprelay_clear_stats_in_msg_t;


/* Input structure for show all relay addresses command */
typedef struct {
	int cmd;
} iphelp_dhcprelay_show_in_msg_t;


typedef struct draddr {
	char if_name[DHCP_MAX_IFNAME_STR];  /* This field is sent by backend as combination of ifType and ifName (or VlanId as applicable). So we print it as is. */
	char vrf_name[DHCP_MAX_VRFNAME_STR];
	char ip_address[DHCP_MAX_IPV4ADDR_STR];
} draddr_t;


/* Output structure for both 'show ip dhcp relay address' and 'show ip dhcp relay address interface' commands */
typedef struct {
	int cmd;
	int num_records;
	draddr_t rec[1];
} iphelp_dhcprelay_show_out_msg_t;

typedef struct {
	int cmd;
} iphelp_dhcprelay_show_stats_in_msg_t;


typedef struct {
	ulong ip_address;
	ulong disc;
	ulong offer;
	ulong request;
	ulong ack;
	ulong nak;
	ulong decline;
	ulong release;
	ulong inform;
} iphelp_dhcprelay_stats_rec_t;


typedef struct{
	int cmd;
	ulong active_clients;
	ulong clients_to_restore;
	ulong num_of_client_pkts;
	ulong num_of_server_pkts;
	ulong timed_out;
	ulong no_offers;
	ulong modify_attempts_failed;
	int num_of_recs;
	iphelp_dhcprelay_stats_rec_t rec[1];
}iphelp_dhcprelay_show_stats_out_msg_t;


typedef enum iphelper_cmd {
     IPHELP_CONFIG_DRADDR=1,
     IPHELP_CLEAR_DHCPRELAY_STATS,
	 IPHELP_CLEAR_IP_DHCPRELAY_STATS,
     IPHELP_SHOW_IF_DRADDR,
     IPHELP_SHOW_DRADDR,
     IPHELP_SHOW_DCHP_RELAY_STATS
} iphelper_cmd_t;

#ifndef __IPHELPER_RC_T__
/* Error codes from iphelper.h */
#define __IPHELPER_RC_T__
/*
 * Possible return error codes from iphelpd functions.
 */
typedef enum iphelper_rc_t {
    IPHELPER_SUCCESS = 0, /* this MUST be 0 */
    IPHELPER_DUP_ERR,
    IPHELPER_MEM_ERR,
    IPHELPER_ARG_ERR,
    IPHELPER_PTR_ERR,
    IPHELPER_INTERNAL_ERR,
    IPHELPER_DATA_ERR,
    IPHELPER_LIB_ERR,
    IPHELPER_IPC_ERR,
    IPHELPER_DCM_MSG_ERR,
    IPHELPER_SET_TO_ERR,
    IPHELPER_ENTRY_NOT_FOUND,
    IPHELPER_MAX_ERR      /* add all errors before this */
} iphelper_rc_t;
#endif /* __IPHELPER_RC_T__ */

#endif  /* __DHCP_MESSAGE_DEF_H__ */
