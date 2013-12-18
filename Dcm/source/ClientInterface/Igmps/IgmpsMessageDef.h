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

#ifndef __IGMPS_MESSAGE_DEF_H__
#define __IGMPS_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"

#define IS_CMDCODE_SET(cmdcode, flag) (cmdcode & flag)

/* Duplicate of macro defined in igmp_vcs.h */
#define  IGMP_MAX_RBRIDGE 256

#define IGMPS_MAX_STR 31

#define IGMPS_IF_TYPE_GIGABIT       0 /*igmp_snooping_gigabitethernet*/
#define IGMPS_IF_TYPE_TENGIGABIT    1 /*igmp_snooping_tengigabitethernet*/
#define IGMPS_IF_TYPE_FORTYGIGABIT  2 /*igmp_snooping_fortygigabitethernet*/
#define IGMPS_IF_TYPE_PORTCHANNEL   3 /*igmp_snooping_port_channel*/
#define IGMPS_IF_TYPE_VLAN          0xef
#define IGMPS_IF_TYPE_NONE          0xff

/* IGMPS Vlan Interface Commands */
#define IGMPS_VLAN_SNOOP_ENABLE     1<<0
#define IGMPS_VLAN_QUERIER_ENABLE   1<<1
#define IGMPS_VLAN_FAST_LEAVE       1<<2
#define IGMPS_VLAN_MROUTER_TIMEOUT  1<<3
#define IGMPS_VLAN_LMQI             1<<4
#define IGMPS_VLAN_QMRT             1<<5
#define IGMPS_VLAN_QUERY_INTERVAL   1<<6
#define IGMPS_GLOBAL_SNOOP_ENABLE   1<<7
#define IGMPS_GLOBAL_RESTRICT_UNKNOWN_MCAST_ENABLE  1<<8
#define IGMPS_GLOBAL_CONFIG_REPLAY      1<<9
#define IGMP_INTERFACE_NAMESIZE  32 

/* IGMPS Vlan Interface default configuration */
#define LMQI_DFLT_VALUE             1000
#define QMRT_DFLT_VALUE             10
#define QUERY_INTRVL_DFLT_VALUE     125
#define MRTR_TIMEOUT_DFLT_VALUE     300

#define IGMP_MULTICAST_BEGIN_IP        "224.0.0.0"
#define IGMP_MULTICAST_END_IP          "239.255.255.255"

#define GENERATE_IF_NAME(_name, _sz, _arg1, _arg2)                      \
    do {                                                                \
        if (! strncmp (_arg1, "v", 1)) {                                \
            snprintf (_name, (_sz), "vlan0.%s", _arg2);                 \
        } else if (! strncmp (_arg1, "p", 1)) {                         \
            snprintf (_name, (_sz), "po%s", _arg2);                     \
        } else if (! strncmp (_arg1, "f", 1)) {                         \
            snprintf (_name, (_sz), "fo%s", _arg2);                     \
        } else if (! strncmp (_arg1, "t", 1)) {                         \
            snprintf (_name, (_sz), "te%s", _arg2);                     \
        } else if (! strncmp (_arg1, "g", 1)) {                         \
            snprintf (_name, (_sz), "gi%s", _arg2);                     \
        }else {                                                     \
            snprintf (_name, (_sz), "%s%s", _arg1, _arg2);              \
        }                                                               \
    } while (0)

#define GENERATE_IF_TYPE(_ifType,_arg1)                                 \
    do {                                                                \
        _ifType = IGMPS_IF_TYPE_NONE;                                   \
        if (! strncmp (_arg1, "v", 1))                                  \
        {                                                               \
            _ifType = IGMPS_IF_TYPE_VLAN;                               \
        }                                                               \
        else if (! strncmp (_arg1, "p", 1))                             \
        {                                                               \
            _ifType = IGMPS_IF_TYPE_PORTCHANNEL;                        \
        }                                                               \
        else if (! strncmp (_arg1, "t", 1))                             \
        {                                                               \
            _ifType = IGMPS_IF_TYPE_TENGIGABIT;                         \
        } else if (! strncmp (_arg1, "g", 1))                           \
        {                                                               \
            _ifType = IGMPS_IF_TYPE_GIGABIT;                            \
        }else                                                           \
        {                                                               \
            _ifType = IGMPS_IF_TYPE_NONE;                               \
        }                                                               \
    } while (0)
#define IGMPS_DEFAULT_CMD_CODE 0

typedef enum {
    IGMPS_CLEAR_GROUPS_ALL_CLI = 100,
    IGMPS_CLEAR_GROUPS_CLI,
    IGMPS_CLEAR_GROUPS_ALL_INTF_CLI,
    IGMPS_CLEAR_GROUPS_INTF_CLI,
    IGMPS_CLEAR_STATS_CLI    
} igmps_clear_cmd_code;

typedef enum {
    IGMPS_SHOW_GROUPS_ALL_CLI = 200,
    IGMPS_SHOW_GROUPS_ALL_DETAIL_CLI,
    IGMPS_SHOW_GROUPS_ADDR_CLI,
    IGMPS_SHOW_GROUPS_ADDR_DETAIL_CLI,
    IGMPS_SHOW_GROUPS_INTERFACE_CLI,
    IGMPS_SHOW_GROUPS_INTERFACE_DETAIL_CLI,
    IGMPS_SHOW_GROUPS_INTERFACE_ADDR_CLI,
    IGMPS_SHOW_GROUPS_INTERFACE_ADDR_DETAIL_CLI,
    IGMPS_SHOW_INTERFACE_CLI,
    IGMPS_SHOW_INTERFACE_VLAN_CLI,
    IGMPS_SHOW_STATISTICS_CLI,
    IGMPS_SHOW_SNOOPING_CLI,
    IGMPS_SHOW_SNOOPING_INTERFACE_CLI,
    IGMPS_SHOW_MROUTER_CLI,
    IGMPS_SHOW_MROUTER_VLAN_CLI,
    IGMPS_SHOW_DEBUG_CLI,
    IGMP_SHOW_INTERFACE_VE_CLI,
    IGMP_SHOW_INTERFACE_PHY_CLI,
    IGMP_SHOW_INTERFACE_PO_CLI,
    IGMP_SHOW_STATISTICS_INTERFACE_PHY_CLI,
    IGMP_SHOW_STATISTICS_INTERFACE_PO_CLI

} igmps_show_cmd_code;

typedef enum {
    IGMPS_DEBUG_ALL_CLI = 300,
    IGMPS_DEBUG_GROUP_CLI,
    IGMPS_DEBUG_INTERFACE_CLI
} igmps_debug_cmd_code;

typedef struct igmps_global_config_msg_ {
    int enable;
        int     restrict_unknown_mcast;
        int     opcode;
} igmps_global_config_msg_t;

typedef struct igmps_vlan_config_msg_ {
    int opcode;
    int enable;
    int qenable;
    int fast_leave;
    int qmrt;
    int lmqi;
    int query_interval;
    int mrouter_timeout;
    unsigned int vlan_id;
    char vlan_if_name[IGMPS_MAX_STR + 1];
    unsigned int vlan_if_index;
} igmps_vlan_config_msg_t;

typedef struct igmps_sg_vlan_config_msg_ {
    char group[IGMPS_MAX_STR + 1]; 
    char if_type[IGMPS_MAX_STR + 1];
    char if_name[IGMPS_MAX_STR + 1];
    unsigned int vlan_id;
    char vlan_if_name[IGMPS_MAX_STR + 1];
    unsigned int vlan_if_index;
    unsigned char negation;
} igmps_sg_vlan_config_msg_t;

typedef struct igmps_mrtr_vlan_config_msg_ {
    char if_type[IGMPS_MAX_STR + 1];
    char if_name[IGMPS_MAX_STR + 1];
    unsigned int vlan_id;
    char vlan_if_name[IGMPS_MAX_STR + 1];
    unsigned int vlan_if_index;
    unsigned char negation;
} igmps_mrtr_vlan_config_msg_t;

typedef struct igmps_clear_input_msg {
    unsigned int cmdCode;
    char grp_name[IGMPS_MAX_STR + 1];
    char if_name[IGMPS_MAX_STR + 1];
} igmps_clear_input_msg_t;

typedef struct igmps_clear_output_msg {
    unsigned int cmdCode;
    int result;
    int result1;
    int result2;
} igmps_clear_output_msg_t;

typedef union igmp_mgmt_buffer_num_ {
    struct {
        u_int16_t   mapped_id;
        u_int16_t   tag;
    } buff;
    u_int32_t   num;
} igmp_mgmt_buffer_num_t;

typedef struct igmps_show_debug_rec {
    unsigned int type;
#define IGMPS_SHOW_DEBUG_TYPE_GROUP 1
#define IGMPS_SHOW_DEBUG_TYPE_IF    2
    char name[IGMPS_MAX_STR + 1];
} igmps_show_debug_rec_t;

typedef struct igmps_debug_input_msg {
    unsigned int cmdCode;
    unsigned int negation;
    char grp_name[IGMPS_MAX_STR + 1];
    char if_name[IGMPS_MAX_STR + 1];
} igmps_debug_input_msg_t;

typedef struct igmps_debug_output_msg {
    unsigned int global;
    unsigned int num_recs;
    unsigned int buf_len;
    igmps_show_debug_rec_t records[1]; 
} igmps_debug_output_msg_t;

typedef struct igmps_show_input_msg {
    unsigned int cmdCode;
    char grp_name[IGMPS_MAX_STR + 1];
    char if_name[IGMPS_MAX_STR + 1];
    unsigned int  bIsMgmtClusterEnabled;
} igmps_show_input_msg_t;

typedef struct igmps_show_hdr {
    unsigned int cmdCode;
    /* length of the buffer including the
     * header 
     */
    uint32_t buflen;
}igmps_show_hdr_t;

typedef struct igmps_show_grp_output_msg_hdr {
    /* hdr: this field should always be first */
    igmps_show_hdr_t hdr;

#define IGMPS_SHOW_GRP_QUERY_NONE 1
#define IGMPS_SHOW_GRP_QUERY_PHY  2
#define IGMPS_SHOW_GRP_QUERY_VLAN 3
    uint32_t query_type;

    /* no. of igmps_show_grp_output_msg_t nodes */
    uint32_t  ngrps;
    /* no. of igmps_remote_show_grp_output_msg_t nodes */
    uint16_t  nremgrps;
}igmps_show_grp_output_msg_hdr_t;

typedef struct igmps_show_grp_output_msg {
    uint16_t vid;
    uint32_t grp_addr;
    uint32_t expiry;
    uint32_t last_reporter;
    uint32_t last_reporter_mode;
    uint32_t uptime;
    uint32_t filt_mode;
    uint32_t igr_sflags;
    char p_ifname[IGMP_INTERFACE_NAMESIZE];
    char c_ifname[IGMP_INTERFACE_NAMESIZE];
    uint32_t grp_is_on_L3_port;
    uint32_t igmp_snooping_enabled_on_vlan;
}igmps_show_grp_output_msg_t;

typedef struct igmps_show_remote_grp_output_msg {
    uint16_t vid;
    uint32_t grp_addr;
    uint32_t uptime;
    uint16_t rbcount;
    char p_ifname[IGMP_INTERFACE_NAMESIZE];
    uint16_t rbarray[IGMP_MAX_RBRIDGE];
}igmps_show_remote_grp_output_msg_t;

typedef struct igmps_show_intf_output_msg_hdr {
    igmps_show_hdr_t hdr;
    uint32_t nifs;
}igmps_show_intf_output_msg_hdr_t;

/* This message was originally meant to contain info from VLAN igifs but now
 * it also has info from router port and PO igifs. And since a VE uses the
 * igif of the underlying VLAN, this message also effectively contains VE info
 */
typedef struct igmps_show_intf_output_msg {
    uint32_t igif_vid; /* Originally used for sorting the output based on VID. */
    uint32_t igif_ifindex; /* Since we're now using the even for router ports and POs, we 
                              will sort the output based on ifindex, the only unique attribute that 
                              VLANs, router-ports and POs have in common */
    uint32_t igif_cflags;
    uint32_t igif_sflags;
    uint32_t igif_paddr; /* This holds the actual address (primary address) of this igif.*/
    uint32_t igif_num_joins;
    uint32_t igif_num_leaves;
    uint32_t igif_other_querier_addr;/* This is the address of the remote querier. */
    uint32_t igifc_qi;
    uint32_t igif_qi_rm;
    uint32_t igif_oqi; /*  This is the operational value of OQI. It is derived from QI and QRI provided it is not explicitly configured on the CLI. */
    uint32_t igif_oqi_rm;
    uint32_t igifc_lmqi;
    uint32_t igifc_qri;
    uint32_t igif_gmi;
    uint32_t igifc_mrti;
    uint16_t igifc_version;
    uint32_t igif_mrti;
    uint32_t igif_mrtr_count;
    uint32_t igif_remote_mrouter_ports_present;
    /* L3 interface specific stuff from this point onwards */
    uint32_t igif_l3_igmp_enabled;
    uint32_t igif_is_router_port_or_po;
    char     igif_l3_ifname[IGMP_INTERFACE_NAMESIZE];
    uint32_t igif_display_l3_info; /* Both 'show ip igmp interface' and 'show ip igmp interface vlan'have the same backend handler. 
                                      For the former command, we need to display both the VLAN(i.e. snooping) and VE(i.e. L3 IGMP) info 
                                      associated with the igif whereas for the latter, we should display only the VLAN info. This flag
                                      stores the decision. */
}igmps_show_intf_output_msg_t;

typedef struct igmps_show_intf_output_mrtr_msg {
    char mrtr_ifname[IGMP_INTERFACE_NAMESIZE];
}igmps_show_intf_output_mrtr_msg_t;

typedef struct igmps_show_snp_stats_msg_hdr {
    igmps_show_hdr_t hdr;
}igmps_show_snp_stats_msg_hdr_t;

typedef struct igmps_show_snp_stats_msg {
    uint32_t vid;
    uint32_t membership_query_rcvd;
    uint32_t membership_query_sent;
    uint32_t membership_query_error;
    uint32_t v1_membership_report_rcvd;
    uint32_t v1_membership_report_sent;
    uint32_t v1_membership_report_error;
    uint32_t v2_membership_report_rcvd;
    uint32_t v2_membership_report_sent;
    uint32_t v2_membership_report_error;
    uint32_t v3_membership_report_rcvd;
    uint32_t v3_membership_report_sent;
    uint32_t v3_membership_report_error;
    uint32_t v2_group_leave_rcvd;
    uint32_t v2_group_leave_sent;
    uint32_t v2_group_leave_error;
    uint32_t rx_unknown;
    uint32_t rx_bad_length;
    uint32_t rx_bad_checksum;
    uint32_t igif_is_router_port_or_po;
    char     ifname[IGMP_INTERFACE_NAMESIZE];
    u_int32_t isl_membership_query_rcvd;
    u_int32_t isl_v1_membership_report_rcvd;
    u_int32_t isl_v2_membership_report_rcvd;
    u_int32_t isl_v3_membership_report_rcvd;
    u_int32_t isl_v2_group_leave_rcvd;

}igmps_show_snp_stats_msg_t;

typedef struct igmps_show_mrouter_msg_hdr {
    igmps_show_hdr_t hdr;
    uint32_t nmrtrs;
}igmps_show_mrouter_msg_hdr_t;

typedef struct igmps_show_mrouter_msg {
    uint32_t vid;
    char if_name[IGMP_INTERFACE_NAMESIZE];
    int32_t expiry;
}igmps_show_mrouter_msg_t;

/* data structs for carrying the output of
 * show ip igmp snooping 
 */
typedef struct igmps_show_ip_igmp_snp_hdr {
    igmps_show_hdr_t hdr;
    uint32_t nvids;
}igmps_show_ip_igmp_snp_hdr_t;

typedef struct igmps_show_ip_igmp_snp {
    uint32_t vid;
    uint32_t igifc_version;
    uint32_t querier_status;
    uint32_t nmrtrs;
    uint32_t ngrps;
}igmps_show_ip_igmp_snp_t;

typedef struct igmps_show_ip_igmp_snp_grp_info {
    uint32_t gaddr;
    char ifname[IGMP_INTERFACE_NAMESIZE];
}igmps_show_ip_igmp_snp_grp_info_t;

typedef struct igmps_show_ip_igmp_snp_mrtr_info {
    char ifname[IGMP_INTERFACE_NAMESIZE];
}igmps_show_ip_igmp_snp_mrtr_info_t;


#endif  /* __IGMPS_MESSAGE_DEF_H__ */
