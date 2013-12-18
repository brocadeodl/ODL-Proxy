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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Navin                                                       *
 **************************************************************************/

#ifndef SSM_APP_H
#define SSM_APP_H

#include <stdio.h>
#include <unistd.h>
#include "ClientInterface/DceWaveClientMessage.h"
#include "ssm_dcmd.h"
//#include "fabos/src/include/dce/ssm/ssmAcl_dcmd_public.h"
#include "ssmAcl_dcmd_public.h"
//#include "/vobs/projects/springboard/fabos/src/include/dce/ssm/ssmAcl_dcmd_public.h"

#define MAX_NAME_STR 64
#define MAX_APP_TYPE 20
#define MAX_MAC_STR  16

#define NO_RULE_ID	700000
#define SSM_TCAM_STS_CNT_NOT_AVAIL 1
#define SSM_STAT_OK 2

#define  MAC_ADDR_SIZE 6

#define SSM_BPRATELIMIT_DEFAULT_MODE 0
#define SSM_BPRATELIMIT_HEAVY_MODE  1
#define SSM_BPRATELIMIT_ALL_SLOTS  0
#define SSM_BPRATELIMIT_MAX_SLOTS 16 

typedef enum {
	ACL_STATS_POLICY_WITH_INTF = 0,
	ACL_STATS_ONLY_INTF,
	ACL_STATS_ONLY_POLICY
} aclStatsType;

typedef enum {
	MAC_ACL_POLICY_UNKNOWN = 0,
	MAC_ACL_POLICY_STANDARD,
	MAC_ACL_POLICY_EXTENDED
} policyTypeEnum;

typedef enum {
    IP_ACL_POLICY_UNKNOWN = 0,
    IP_ACL_POLICY_STANDARD,
    IP_ACL_POLICY_EXTENDED,
	IP_ACL_POLICY_MAX
} policyTypeIpEnum;

typedef enum {
	ACL_CLEAR_STATS_CMD_ALL = 10,
	ACL_CLEAR_STATS_CMD_GRP,
	ACL_CLEAR_STATS_CMD_GRP_IF,
	ACL_CLEAR_STATS_CMD_MAX
} AclClearStatsCmdCode;
/*
typedef enum {
	ACL_CLEAR_STATS_RC_OK = 0,
	ACL_CLEAR_STATS_RC_ERR,
	ACL_CLEAR_STATS_RC_INVALID_GRP_NAME,
	ACL_CLEAR_STATS_RC_NO_GRP,
	ACL_CLEAR_STATS_RC_MAX
} AclClearStatsRetCode;
*/
typedef enum {
	SSM_DCM_ERR_MAC_ACL_EEXIST = 1,
	SSM_DCM_ERR_IP_ACL_EEXIST,
	SSM_DCM_ERR_ACL_CONFIG_CHANGE,
	SSM_ACL_CLEAR_STATS_RC_NO_IF,
    SSM_DCM_ERR_ACL_CMD_FAIL_RETRY
} AclClearShowStatsRetCode;

typedef enum {
	CREATE_MAC_ACL_POLICY = 0,
	DELETE_MAC_ACL_POLICY,
	ASSOC_MAC_ACL_POLICY_TO_INT,
	DISASSOC_MAC_ACL_POLICY_TO_INT,
	ASSOC_RULE_TO_POLICY,
	DISASSOC_RULE_TO_POLICY,
	SHOW_STATS_ACL_INTF,
	SHOW_MAC_ACCESS_GROUP,
	SHOW_MAC_ACCESS_GROUP_INTF,
	RSEQ_MAC_ACL_POLICY
} aclOpcode;

typedef enum {
	L3ACL_OPCODE_UNKNOWN = 0,
	CREATE_IP_ACL_POLICY,
	DELETE_IP_ACL_POLICY
} l3aclOpcode;

	
typedef enum {
	SHOW_IP_ACCESS_GROUP = 0,
	SHOW_IP_ACCESS_GROUP_INTF,
	SHOW_IP_NAME_ACCESS_GROUP,
	SHOW_IP_NAME_ACCESS_GROUP_INTF
} aclIpOpcode;

typedef enum {
	PHY_INTF_TYPE = 0,
	PO_INTF_TYPE,
	VLAN_INTF_TYPE,
	VE_INTF_TYPE,
	PHY_INTF_TE_TYPE,
	PHY_INTF_GI_TYPE,
	UNKNOWN_TYPE
} intfType;

typedef enum{
	INGRESS_BINDING =0,
	EGRESS_BINDING,
	UNKNOWN_BINDING
} bindingType;

typedef enum{
	DETAIL =0,
	NO_DETAIL
}detailType;

typedef enum {
    SSM_BPRATELIMIT_NO_HEAVY = 0,
    SSM_BPRATELIMIT_HEAVY
}bpRatelimitOpcode;


typedef struct L2aclPolicyMsg_ {
	char policyname[MAX_NAME_STR];
	char ifname[MAX_NAME_STR];
	int policytype;
	aclOpcode opcode;
} L2aclPolicyMsg_t;

typedef struct L2aclRuleMsg_ {
	char policyname[MAX_NAME_STR];
	int policytype;
	unsigned long long ruleid; 
	int action;
	char srcmac[MAX_MAC_STR];
	int srcmask;
	char dstmac[MAX_MAC_STR];
	int dstmask;
	char apptype[MAX_NAME_STR];
	int rulecounter;
	aclOpcode opcode;
} L2aclRuleMsg_t;

typedef struct L2aclPolicyBulkMsg_ {
	char policyname[MAX_NAME_STR];
	int policytype;
	unsigned long long numrules;
	aclOpcode opcode;
	L2aclRuleMsg_t rules[1];
}L2aclPolicyBulkMsg_t;

typedef struct L2aclIntfAssocBulkMsg_ {
	unsigned long long numrecords;
	L2aclPolicyMsg_t intflist[1];
}L2aclIntfAssocBulkMsg_t;


typedef struct L2aclShowMsg_ {
	char policyname[MAX_NAME_STR];
	char ifname[MAX_NAME_STR];
	int bindingType;
	int detail;
	int policytype;
	int iftype;
	int opcode;
	int isset; //used only in response
} L2aclShowMsg_t;

typedef struct L3aclShowMsg_ {
      char policyname[MAX_NAME_STR];
      char ifname[MAX_NAME_STR];
      int bindingType;
      int detail;
      int policytype;
      int iftype;
      int opcode;
      int isset; //used only in response
} L3aclShowMsg_t;

typedef struct L2aclShowAccessGroup_ {
	unsigned long long numrecords;
	L2aclShowMsg_t info[1];
} L2aclShowAccessGroup_t;

typedef struct L3aclShowAccessGroup_ {
	unsigned long long numrecords;
	L3aclShowMsg_t info[1];
} L3aclShowAccessGroup_t;

typedef struct acl_clr_stats_input_msg {
    unsigned int cmdCode;
    char grp_name[MAX_NAME_STR];
    char if_name[MAX_NAME_STR];
} acl_clr_stats_input_msg_t;

typedef struct acl_clr_stats_output_msg {
    unsigned int cmdCode;
    int result;
    int result1;
    int result2;
} acl_clr_stats_output_msg_t;

typedef struct ruleStats_ {
	unsigned long long ruleid;
	unsigned long long countframes;
	int ruleTcamStatus;
} ruleStats_t;

typedef struct L2aclIntfStatsMsg_ {
	char policyname[MAX_NAME_STR];
	int policytype;
	char ifname[MAX_NAME_STR];
	intfType iftype;
	int opcode;
	int status;
	unsigned long long numrecords;
	ruleStats_t stats[1];
} L2aclIntfStatsMsg_t;

typedef struct L3aclIntfStatsMsg_ {
    char policyname[MAX_NAME_STR];
    int policytype;
    char ifname[MAX_NAME_STR];
    intfType iftype;
    int opcode;
    int status;
    unsigned long long numrecords;
    ruleStats_t stats[1];
} L3aclIntfStatsMsg_t;

typedef struct L2aclIntfInfo_ {
	char ifname[MAX_NAME_STR];
	intfType iftype;
} L2aclIntfInfo_t;

typedef struct L3aclIntfInfo_ {
    char ifname[MAX_NAME_STR];
    intfType iftype;
} L3aclIntfInfo_t;

typedef union  ssm_mgmt_buffer_num_ { 
    struct { 
        u_int16_t   mapped_id;
        u_int16_t   tag;     
    } buff;
    u_int32_t   num;   
} ssm_mgmt_buffer_num_t;

typedef struct MacaclReseqMsg_ {
	char policyname[MAX_NAME_STR];
	unsigned int seqId;
	unsigned int incSeqId; 
	aclOpcode opcode;
}MacaclReseqMsg_t;

typedef struct ssmBbpratelimitConfigMsg_
{
    unsigned int opCode;
    unsigned int slotId;
}ssmBpratelimitConfigMsg_t ;

static inline u_int16_t 
ssm_get_buffer_tag (u_int32_t  buff_num) 
{
    return (((ssm_mgmt_buffer_num_t*) &buff_num)->buff.tag);
}

static inline u_int16_t 
ssm_get_buffer_mapped_id (u_int32_t  buff_num) 
{
    return (((ssm_mgmt_buffer_num_t*) &buff_num)->buff.mapped_id);
}

#endif /* SSM_APP_H */
