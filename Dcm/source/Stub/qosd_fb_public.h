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
 *  Copyright (c) 2008-2011 by Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Module name: qosd_fb_public.h
 *
 *  Description:
 * 		Flowbased QoS definitions shared between flowbased server and client.
 */

#ifndef _QOSD_FB_PUBLIC_H_
#define _QOSD_FB_PUBLIC_H_

#include <stdint.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ssmAcl_public.h>

typedef unsigned int ifindex_t;

typedef bool bool_t;

#define FB_MAX_MAP_NAME_SIZE 65
#define FB_MAX_ACL_NAME_SIZE SSMACL_NAME_SIZE+1
#define MAX_IF_NAME_LEN 64
#define MAX_INTENT_HDR_LEN 200

enum fb_policy_match_criteria {
	FB_POLICY_RULE_MATCH_INVALID,
	FB_POLICY_RULE_MATCH_ACL_E
};

typedef enum {
	FB_QOS_IPC_TCAM_PROG_REQ,
	FB_QOS_IPC_NUM_REC_REQ,
	FB_QOS_IPC_STATS_REQ,
	FB_QOS_IPC_PFC_EXCLUDE_REQ,
	FB_QOS_IPC_CLEAR_COUNTER_REQ,
	FB_QOS_IPC_ACL_PROG_REQ	
}fbIpcReqMessageType_t;

typedef enum {
	FB_QOS_IPC_TCAM_PROG_RESP,
	FB_QOS_IPC_NUM_REC_RESP,
	FB_QOS_IPC_STATS_RESP,
	FB_QOS_IPC_PFC_EXCLUDE_RESP,
	FB_QOS_IPC_CLEAR_COUNTER_RESP,
	FB_QOS_IPC_ACL_PROG_RESP
}fbIpcRespMessageType_t;

typedef enum {
	FB_QOS_MSG_TYPE_INVALID,
	FB_QOS_MSG_TYPE_ADD,
	FB_QOS_MSG_TYPE_DEL_ALL,
	FB_QOS_MSG_TYPE_MAX
}fbTcamProgReqMessageType_t;

typedef enum {
	FB_QOS_ACL_MSG_TYPE_INVALID,
	FB_QOS_ACL_MSG_TYPE_ADD,
	FB_QOS_ACL_MSG_TYPE_DEL
}fbAclProgReqMessageType_t;

typedef enum {
	FB_QOS_MSG_TYPE_CLEAR_ALL,
	FB_QOS_MSG_TYPE_CLEAR_INTF,
	FB_QOS_MSG_TYPE_CLEAR_INTF_CLASS
}fbClearCounterMessageType_t;
	
/* policy direction */
typedef enum {
	FB_POLICY_DIR_IN,
	FB_POLICY_DIR_OUT,
	FB_POLICY_DIR_BOTH
}fbPolicyDirection_t;

typedef enum {
	FB_IPC_RESP_TYPE_INVALID,
	FB_IPC_RESP_TYPE_SUCCESS,
	FB_IPC_RESP_TYPE_FAILED
}fbIpcRespType_t;
	
typedef struct fbPolTcamAttr_t_ {
	uint16_t policer_id;
}fbPolTcamAttr_t;

typedef struct fbCosTcamAttr_t_ {
	uint8_t cos;
	uint8_t tc;
}fbCosTcamAttr_t;

typedef struct fbDscpTcamAttr_t_ {
	uint16_t dscp;
}fbDscpTcamAttr_t;

typedef struct fbSflowTcamAttr_t_ {
	uint8_t sflow_profile_id;
}fbSflowTcamAttr_t;

/* TCAM Add/Del Request Message */
typedef struct fbTcamProgReqMessage_t_ {
	fbTcamProgReqMessageType_t     msg_type;
	ifindex_t                      ifindex;
	uint32_t                       class_id;
	uint16_t                       precedence;
	char                           class_name[FB_MAX_MAP_NAME_SIZE+1];
	char                           acl_name[FB_MAX_ACL_NAME_SIZE];
	fbPolicyDirection_t            dir;
	bool                          is_pol_valid;
	bool                          is_cos_valid;
	bool                          is_dscp_valid;
	bool                          is_sflow_valid;
	fbPolTcamAttr_t                pol_tcam_param;
	fbCosTcamAttr_t                cos_tcam_param;
	fbDscpTcamAttr_t               dscp_tcam_param;
	fbSflowTcamAttr_t              sflow_tcam_param;
}fbTcamProgReqMessage_t;

typedef struct fbNasAclInfo_s {
	fbAclProgReqMessageType_t	msg_type;
	char                        acl_name[FB_MAX_ACL_NAME_SIZE];
	uint32_t 					ipaddr;
	uint32_t 					mask;
	bool 						is_vlan_present;
	uint32_t					vlan_vrf_id;   // it can be either vlan id or vrf id
} fbNasAclInfo_t;


/* ACL Add/Del Request Message */
typedef struct fbAclProgReqMessage_s {
	fbAclProgReqMessageType_t   msg_type;
	fbNasAclInfo_t nas_acl_info;
}fbAclProgReqMessage_t;

typedef struct fbAclProgRespMessage_s {
	fbAclProgReqMessageType_t   msg_type;
	fbNasAclInfo_t nas_acl_info;
}fbAclProgRespMessage_t;

/* TCAM Add/Del Response Message */
typedef struct fbTcamProgRespMessage_t_ {
	fbTcamProgReqMessageType_t     msg_type;
	ifindex_t                      ifindex;
	uint32_t                       class_id;
	uint16_t                       precedence;
	char                           class_name[FB_MAX_MAP_NAME_SIZE+1];
	fbPolicyDirection_t            dir;
}fbTcamProgRespMessage_t;

/* Bulk Stats request message */
typedef struct fbStatsReqMessage_t_ {
	ifindex_t             ifindex;
	fbPolicyDirection_t   dir;
	uint32_t              class_id;
}fbStatsReqMessage_t;

typedef struct fbPerClassStatsRec_t_ {
	uint32_t              class_id;
	char                  class_name[FB_MAX_MAP_NAME_SIZE+1];
	char                  acl_name[FB_MAX_ACL_NAME_SIZE];
	uint64_t              packet_count;
	uint64_t              byte_count;
}fbPerClassStatsRec_t;

/* Bulk Stats response message */
typedef struct fbStatsRespMessage_t_ {
	ifindex_t              ifindex;
	fbPolicyDirection_t    dir;
	uint32_t               num_record;
	fbPerClassStatsRec_t   class_record[1];
}fbStatsRespMessage_t;

/* Number of record request message */
typedef struct fbNumRecordReqMessage_t_ {
	ifindex_t               ifindex;
	fbPolicyDirection_t     dir;
}fbNumRecordReqMessage_t;

/* Number of record response message */
typedef struct fbNumRecordRespMessage_t_ {
	uint32_t       num_rec;
}fbNumRecordRespMessage_t;

/* PFC exclude list request message */
typedef struct fbPfcExcludeListReqMessage_t_ {
	fbTcamProgReqMessageType_t msg_type;
	ifindex_t                  ifindex;
	fbPolTcamAttr_t            pol_tcam_param; /* Infinite Policer Id */
	uint16_t                   precedence;
	bool                      pfc[8];
}fbPfcExcludeListReqMessage_t;

/* PFC exclude list resp message */
typedef struct fbPfcExcludeListRespMessage_t_ {
	fbTcamProgReqMessageType_t msg_type;
	ifindex_t                  ifindex;
}fbPfcExcludeListRespMessage_t;

/* Clear stats request and response message */
typedef struct fbClearCounterReqMessage_t_ {
	fbClearCounterMessageType_t msg_type;
	ifindex_t                   ifindex;
	fbPolicyDirection_t         dir;
	uint32_t                    class_id;
} fbClearCounterReqMessage_t;

/* Generic IPC Request structure */
typedef struct fbIpcReqMessage_t_ {
	char								 intent_hdr[MAX_INTENT_HDR_LEN];
	fbIpcReqMessageType_t                req_type;
	union {
		fbTcamProgReqMessage_t           tcam_req_msg;
		fbNumRecordReqMessage_t          record_count_req_msg;
		fbStatsReqMessage_t              stats_req_msg;
		fbPfcExcludeListReqMessage_t     pfc_excludelist_req_msg;
		fbClearCounterReqMessage_t       clear_counter_req_msg;
		fbAclProgReqMessage_t			 acl_req_msg;
	}req_msg;
}fbIpcReqMessage_t;

/* Generic IPC Response structure */
typedef struct fbIpcRespMessage_t_ {
	char								intent_hdr[MAX_INTENT_HDR_LEN];
	fbIpcRespMessageType_t              resp_type;
	fbIpcRespType_t                     resp_code;
	union {
		fbTcamProgRespMessage_t         tcam_resp_msg;
		fbNumRecordRespMessage_t        record_count_resp_msg;
		fbStatsRespMessage_t            stats_resp_msg;
		fbPfcExcludeListRespMessage_t   pfc_excludelist_resp_msg;
		fbAclProgRespMessage_t			acl_resp_msg;
	}resp_msg;
}fbIpcRespMessage_t;

#define FB_DIR_STR(dir)  ((dir == FB_POLICY_DIR_BOTH)?"BOTH":((dir == FB_POLICY_DIR_IN)?"IN":"OUT"))

#endif

