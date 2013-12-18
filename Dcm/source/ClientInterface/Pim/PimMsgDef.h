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

/***************************************************************************
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Rahul Jain                                                   *
 ***************************************************************************/

#ifndef __PIM_MSG_DEF_H__
#define __PIM_MSG_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"

/* Global length values */
#define PIM_IP_ADDR_LEN 39
#define IPV4_ADDR_LEN 16
#define MAX_PREFIX_LIST_LEN 32
#define PIM_INTF_NAME_LEN_MAX 32
#define PIM_DFLT_MAP_ID 0
#define IPV6_ADDRESS_DCM IP6_IPV6_ADDRESS_DCM
#define IF_LONG_NAMESIZE_DCM 64
#define PIM_PRINT_BUF_MAX_DCM  10
#define PIM_PRINT_BUF_SIZE_DCM 64
#define PIM_PRINT_AFI_BUF_SIZE_DCM 5
#define MAX_PIM_NEIGHBORS 10
#define PIM_SPT_THRESHOLD_INFINITY_DCM (0)

typedef unsigned char UINT8;
typedef unsigned short int UINT16;
typedef unsigned int UINT32;

/* PIM related error codes */
#define PIM_CLEAR_PRESULTS 1

/* Global define variables */
#define PIM_NOT_USR_CFGED_VAL 0
#define PIM_PREFIX_ADD 1
#define PIM_PREFIX_DELETE 2

/* PIM default values 
 * These values should be in sync with the corresponding values defined in 
 * l3/mcast/include/mcast_cu.h
 */
#define PIM_DFLT_MCACHE 2048
#define PIM_DFLT_HELLO_TIMER 30
#define PIM_DFLT_NBR_TO 105
#define PIM_DFLT_INACT_TO 180
#define PIM_DFLT_MSG_INTVL 60
#define PIM_DFLT_SPT 1
#define PIM_DFLT_DR_PRIO 1
#define PIM_DFLT_TTL_THRESH 1

/* Macro for single VRF */
#define PIM_SINGLE_VRF
/* Supported interface types 
 * The order of these interface types should be the same as that in
 * brocade-pim.yang file. Else wrong interface type value will be 
 * sent to the PIM daemon */
typedef enum {
    INTERFACE_ONEGIG,
    INTERFACE_FORTYGIG,
    INTERFACE_TENGIG,
    INTERFACE_PORT_CHANNEL,
    INTERFACE_VLAN,
    INVALID_INTERFACE = 255
} pim_intf_type;

/* PIM command definitions */
typedef enum {
    /* PIM-SM router config commands */ 
    PIM_CFG_IP_MULT_CMD,
    PIM_CFG_RTR_PIM_CMD,
    PIM_CFG_MAX_MCACHE_CMD,
    PIM_CFG_HELLO_TIMER_CMD,
    PIM_CFG_NBR_TO_CMD,
    PIM_CFG_INACT_TIMER_CMD,
    PIM_CFG_MSG_INTV_CMD,
    PIM_CFG_SPT_THOLD_CMD,
	PIM_CFG_BSR_CAND_CMD,
    PIM_CFG_STATIC_RP_CMD,
    PIM_CFG_CAND_RP_CMD,
	PIM_CFG_CAND_RP_PREFIX_CMD,
	NO_PIM_CFG_BSR_CAND_CMD,

	/* PIM-SM Show commands */
	PIM_SHOW_NEIGHBOR_CMD,
	PIM_SHOW_SPARSE_CMD,
	PIM_SHOW_SPARSE_CMD_VE,
	PIM_SHOW_SPARSE_CMD_PHY,
	PIM_SHOW_SPARSE_CMD_PO,
    PIM_SHOW_MCACHE_CMD,
	PIM_SHOW_NONE_CMD,

    /* PIM-SM interface config commands */ 
    PIM_INTF_CFG_PIM_SP_CMD,
    PIM_INTF_CFG_DR_PRIO_CMD,
    PIM_INTF_CFG_PIM_BRDR_CMD,
    PIM_INTF_CFG_MULT_ACL_CMD,
    PIM_INTF_CFG_TTL_THOLD_CMD,
    
    /* PIM PostBoot message */
    PIM_GLBL_CFG_REPLAY_CMD,
    PIM_LOCAL_CFG_REPLAY_CMD
}pim_cfg_cmd_code;

typedef enum {
    PIM_MSG_ADD = 1,
    PIM_MSG_UPD,
    PIM_MSG_DEL
}pim_wyserea_msg_type;

/* Message definition for PIM Neighbor Show CLI */
typedef struct {
    int cmd_code;
	char if_name[IF_LONG_NAMESIZE_DCM];
}pim_show_in_msg;

/* Message definition for commands under router PIM mode */
typedef struct {
    int msg_type;
    int cmd_code;
    char *vrf_id;
    int enable;
    int max_mcache;
    int hello_timer;
    int nbr_to;
    int inact_timer;
    int msg_intv;
    unsigned int spt_thold;
} pim_rtr_cfg_msg_t;

/* Message definition for ip multicast-routing command */
typedef struct {
    int enable;
} pim_ip_mult_cfg_msg_t;

/* Message definition for BSR Candidate CLI */
typedef struct {
    int msg_type;
    int intf_type;
    char intf_name[PIM_INTF_NAME_LEN_MAX];
	int hash_mask;
	int priority;
} pim_bsr_candidate_msg_t;

/* Message definition for commands under the interface mode */
typedef struct {
    int cmd_code;
    int enable;
    int intf_type;
    char intf_name[PIM_INTF_NAME_LEN_MAX];
    int dr_prio;
    //char mult_bdry[MAX_PREFIX_LIST_LEN];
	int mult_bdry;
    int brdr_rtr;
    int ttl_thold;
} pim_phy_intf_cfg_msg_t;

/* Message definition for Static RP command */
typedef struct {
    int msg_type;
    char rp_addr[IPV4_ADDR_LEN];
    char prefix_list[MAX_PREFIX_LIST_LEN];
} pim_static_rp_msg_t;

/* Message definition for Candidate RP command */
typedef struct {
    int msg_type;
    int intf_type;
    char intf_name[PIM_INTF_NAME_LEN_MAX];
    char prefix_list[MAX_PREFIX_LIST_LEN];
} pim_cand_rp_cfg_msg_t;

/* Message definition for Candidate RP Prefix command */
typedef struct {
    int msg_type;
	int prefixAction;
	char prefixIP[IPV4_ADDR_LEN];
	int prefixLen;
} pim_cand_rp_prefix_msg_t;

typedef struct PIM_NBR_LIST_DCM
{
    // AF Independent
	int 		size;
	char 		port_str[IF_LONG_NAMESIZE_DCM + 1];
	char 		phy_port_str[IF_LONG_NAMESIZE_DCM + 1];
	char 		vrf_name[20];
    struct 		PIM_NBR_LIST_DCM *next;
    UINT32      nbr_timer;
	char 		nbr_uptime[PIM_PRINT_BUF_SIZE_DCM];
	char  		rtr_address[PIM_PRINT_BUF_SIZE_DCM];
    UINT32      dr_priority_present:1;
    UINT32      dr_priority;
    UINT32      gen_id;
    UINT16      hold_down_time;      /* time since last refresh */
    UINT16      physical_port;     /* for a virtual interface, it is the
                                         physical port on which the nbr resides */
    // v6 specific
#ifndef BRCD_DCE_L3_PIM_PORT
    //UINT16      addr_list_option_len;
    //PIM6_V2_HELLO_ADDRESS_LIST_DCM  *secondary_addr_list;
#endif
} PIM_NBR_LIST_DCM;

typedef struct {
	int 				nbr_count;
	//PIM_NBR_LIST_DCM 	pim_nbr_list[MAX_PIM_NEIGHBORS];
	PIM_NBR_LIST_DCM 	pim_nbr_list[1];
} pim_show_out_msg;

typedef struct {
	int 				size;
    int 				port_count;
	int					total_neighbors;
    pim_show_out_msg 	out_msg[1];
} pim_show_out_msg_all_ports;

typedef struct PIM_CLASS_DCM
{
  UINT16          max_mcache;
  UINT32		  mcache_index;
  UINT16          hello_interval;                   /* hello timer interval */
  UINT16          nbr_timeout;                      /* nbr hold time */
  UINT16          join_prune_interval;
  UINT16          timer_interval;                   /* inactivity timer interval */
  UINT16          hw_drop:1;
  UINT16          prune_wait_interval;          /* time to wait for Prune-Overrides after a Prune */
  UINT16          bootstrap_period;
  UINT16          c_rp_adv_interval;
  UINT16          register_suppression_timer;       /* timer to suppress registers */
  UINT16          null_probe_interval;              /* interval after which registers are */
  char            spt_threshold[9];
  unsigned char   number_of_subnet_mask_bits;
  UINT16          ssm_enable:1;
  char 			  ssm_range[PIM_PRINT_BUF_SIZE_DCM];
  } PIM_CLASS_DCM;
  
typedef struct PIM_VIF_DCM
{
  char 		if_name[IF_LONG_NAMESIZE_DCM + 1];
  char 		lcl_address[PIM_PRINT_BUF_SIZE_DCM];
  char      version[2];
  char      mode[2];
  UINT8   	vifp_mode; 
  int 		pim_mode;
  char 		designated_router_address[PIM_PRINT_BUF_SIZE_DCM];
  UINT8     ttl_threshold;
  char      acl_name[50];
  UINT32    dr_priority;
  int 		vrf_index;
  int 		ip_port_vrf_index;
  int 		ipvrf_invalid_vrf_idx;
} PIM_VIF_DCM;

typedef struct {
	int size;
    int port_count;
	int pim_enabled;
	PIM_CLASS_DCM pim_class_dcm;
	PIM_VIF_DCM pim_vif_dcm[1];
} pim_show_sparse_out_msg;

/* Structure to store the flags to be displayed for a mcache entry */
typedef struct {
    int rpt;
    int spt;
    int reg;
    int l2_reg;
    int reg_sup;
    int reg_probe;
    int lcl;
    int lcl_rcv;
    int sp_mode;
    int ssm_mode;
    int prune;
    int need_rte;
    int msdp_adv;
}mc_flags;

/* Structure containing data that will be sent to PIM daemon on executing
 * show ip pim mcache cmd */
typedef struct {
    int cmd_code;
    int first_pg;
    char ip_addr1[IPV4_ADDR_LEN];
    char ip_addr2[IPV4_ADDR_LEN];
    UINT32 nxt_src;
    UINT32 nxt_grp;
}pim_show_in_mcache_msg;

typedef struct{
    char intf_id[IF_LONG_NAMESIZE_DCM];
    char phy_port[IF_LONG_NAMESIZE_DCM];
    char age[128];
    int expire_time;
}pim_show_oif_info;

/* Structure containing data returned by PIM daemon on executing 
 * show ip pim mcache cmd */
typedef struct {
    char parent[IF_LONG_NAMESIZE_DCM];
    char parent_phy_port[IF_LONG_NAMESIZE_DCM];
    char src_addr[PIM_IP_ADDR_LEN];
    char grp_addr[PIM_IP_ADDR_LEN];
    char rp_addr[PIM_IP_ADDR_LEN];
    char sw_uptime[128];
    UINT32 flag_bmp;
    mc_flags flags;
    int wg_entry;
    int rpf_info_param;
    char rpf_info_buf[80];
    int num_oifs;
    pim_show_oif_info oif_info[1];
}pim_show_out_mcache_info;

typedef struct {
    int	size;
    int tot_ent;
    int	ent_cnt;
    int	match_ent_cnt;
    int more_ents;
    UINT32 nxt_src;
    UINT32 nxt_grp;
    pim_show_out_mcache_info mcache_info[1];
} pim_show_out_mcache_msg;

#endif /* __PIM_MSG_DEF_H__ */
