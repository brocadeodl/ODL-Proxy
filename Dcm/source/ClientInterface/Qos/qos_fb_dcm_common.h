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
 *  Module name: qos_fb_dcm_common.h
 *
 *  Description:
 *      Flowbased QoS Shared structures with dcmd
 */


#ifndef _QOS_FB_DCM_COMMON_H_
#define _QOS_FB_DCM_COMMON_H_

#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "WyserEaGateway/GenericClientMessage.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "qosd_fb_public.h"
#include "ssmAcl_public.h"
#include "DcmCStatus.h"

#define STR32 32
#define FB_DEFAULT_COS_VALUE 8
#define FB_DEFAULT_TRAFFIC_CLASS_VALUE 8
#define FB_DEFAULT_DSCP_VALUE 64
#define FB_DEFAULT_SHAPING_RATE_VALUE 0
#define FB_DEFAULT_STRICT_PRIORITY_VALUE 8
#define FB_DEFAULT_QUEUE_DWRR_VALUE 0
#define FB_DEFAULT_QUEUE_SCHEDULER_TYPE 0
#define FB_DWRR_SUM 100
#define FB_INVALID_LOCATION_ID 0


typedef enum
{
	FB_CLASSMAP_CREATE=1005,
	FB_CLASSMAP_DELETE,
	FB_ADD_CLASS_MAP_TO_POLICY_MAP,
	FB_DEL_CLASS_MAP_TO_POLICY_MAP,
	FB_SET_POLICER_ATTR,
	FB_CLEAR_POLICER_ATTR,
	FB_POLICYMAP_CREATE,
	FB_POLICYMAP_DELETE,
    FB_POLICYMAP_BIND_INTERFACE_IN,
    FB_POLICYMAP_BIND_INTERFACE_OUT,
    FB_POLICYMAP_UNBIND_INTERFACE_IN,
    FB_POLICYMAP_UNBIND_INTERFACE_OUT,
	FB_SHOW_POLICYMAP_INTERFACE,
	FB_SHOW_POLICYMAP_ALL,
	FB_ADD_FILTER_TO_CLASS,
	FB_DELETE_FILTER_FROM_CLASS,

	FB_SHOW_POLICY_MAP_DETAIL,

	FB_SET_COS_TC_ACTION_CREATE,
	FB_SET_COS_TC_ACTION_DELETE,

	FB_SET_DSCP_ACTION_CREATE,
	FB_SET_DSCP_ACTION_DELETE,

	FB_COS_MUTATION_MAP_ACTION_CREATE,
	FB_COS_MUTATION_MAP_ACTION_DELETE,
	FB_COS_TRAFFIC_CLASS_MAP_ACTION_CREATE,
	FB_COS_TRAFFIC_CLASS_MAP_ACTION_DELETE,
	FB_DSCP_MUTATION_MAP_ACTION_CREATE,
	FB_DSCP_MUTATION_MAP_ACTION_DELETE,
	FB_DSCP_COS_MAP_ACTION_CREATE,
	FB_DSCP_COS_MAP_ACTION_DELETE,
	FB_DSCP_TRAFFIC_CLASS_MAP_ACTION_CREATE,
	FB_DSCP_TRAFFIC_CLASS_MAP_ACTION_DELETE,
	FB_SFLOW_MAP_ACTION_CREATE,
	FB_SFLOW_MAP_ACTION_DELETE,

	FB_SHAPING_RATE_ACTION_CREATE,
	FB_SHAPING_RATE_ACTION_DELETE,

	FB_QOS_QUEUE_SCHEDULER_ACTION_CREATE,
	FB_QOS_QUEUE_SCHEDULER_ACTION_DELETE,

	FB_CEE_ACTION_CREATE,
	FB_CEE_ACTION_DELETE,

	FB_CLEAR_POLICYMAP_COUNTER_INT,
	FB_CLEAR_POLICYMAP_COUNTER_ALL,

	FB_INVALID_CMD_CODE
}fbDcmCommandCode_t;

/*************************************************************************
                    DCMD Configuration structures 
 ************************************************************************/

/*
 * Dcmd class-map message.
 */
typedef struct fbDcmClassName_t_ {
	char name[FB_MAX_MAP_NAME_SIZE];
}fbDcmClassName_t;

typedef struct fbDcmClassMapMessage_t_ {
	fbDcmCommandCode_t opcode;
	char               name[FB_MAX_MAP_NAME_SIZE];
	enum 			   fb_policy_match_criteria rule;
	char			   accessListName[FB_MAX_ACL_NAME_SIZE];
}fbDcmClassMapMessage_t;

/*
 * Dcmd policy-map message.
 */
typedef struct fbDcmPolicyMapMessage_t_ {
	fbDcmCommandCode_t opcode;
	char               name[FB_MAX_MAP_NAME_SIZE];
}fbDcmPolicyMapMessage_t;

/*
 * Dcmd policy-class message.
 */
typedef struct fbDcmPolicyClassMessage_t_ {
	fbDcmCommandCode_t opcode;
	char               policy_name[FB_MAX_MAP_NAME_SIZE];
	fbDcmClassName_t   class_name;
}fbDcmPolicyClassMessage_t;

/*
 * Dcmd police message.
 */
typedef struct fbDcmPoliceAttr_t_ {
	char     prio_map_name[FB_MAX_MAP_NAME_SIZE];
	uint8_t  conform_ip_tos; // conform IP precedence or dscp value
	uint8_t  conform_traffic_class; // conform traffic class value
	uint8_t  exceed_ip_tos; // exceed IP precedence or dscp value
	uint8_t  exceed_traffic_class; // exceed traffic class value
	uint8_t  conform_action;
	uint8_t  exceed_action;
	uint8_t  violate_action;
	uint64_t cir;
	uint64_t cbs;
	uint64_t eir;
	uint64_t ebs;
	uint32_t set_conform_action:1;
	uint32_t set_exceed_action:1;
	uint32_t set_violate_action:1;
	uint32_t set_conform_ip_prec:1;
	uint32_t set_conform_dscp:1;
	uint32_t set_conform_tc:1;
	uint32_t set_exceed_ip_prec:1;
	uint32_t set_exceed_dscp:1;
	uint32_t set_exceed_tc:1;
	uint32_t reserved:23;
}fbDcmPoliceAttr_t;



typedef struct fbDcmPoliceMessage_t_ {
    fbDcmCommandCode_t  opcode;
    char                policy_name[FB_MAX_MAP_NAME_SIZE];
    fbDcmClassName_t    class_name;
    fbDcmPoliceAttr_t   police_attr;
}fbDcmPoliceMessage_t;


/*
 * Dcmd 'set cos' message.
 */
typedef struct fbDcmCosMarkAttr_t_ {
	uint8_t         cos;
	uint8_t         tc;
}fbDcmCosMarkAttr_t;

typedef struct fbDcmCosMessage_t_ {
	fbDcmCommandCode_t opcode;
	char               policy_name[FB_MAX_MAP_NAME_SIZE];
	fbDcmClassName_t   class_name;
	fbDcmCosMarkAttr_t cos_attr;
}fbDcmCosMessage_t;
	
/*
 * Dcmd 'set dscp' message.
 */
typedef struct fbDcmDscpMarkAttr_t_ {
	uint16_t         dscp;
}fbDcmDscpMarkAttr_t;

typedef struct fbDcmDscpMessage_t_ {
	fbDcmCommandCode_t   opcode;
	char                 policy_name[FB_MAX_MAP_NAME_SIZE];
	fbDcmClassName_t     class_name;
	fbDcmDscpMarkAttr_t  dscp_attr;
}fbDcmDscpMessage_t;

/*
 * Dcmd mutation-map message.
 */
typedef struct fbDcmMapAttr_t_ {
	char  map_name[FB_MAX_MAP_NAME_SIZE];
}fbDcmMapAttr_t;

typedef struct fbDcmMutationMessage_t_ {
	fbDcmCommandCode_t opcode;
	char               policy_name[FB_MAX_MAP_NAME_SIZE];
	fbDcmClassName_t   class_name;
	fbDcmMapAttr_t     mutation_attr;
}fbDcmMutationMessage_t;

/*
 * Dcmd sflow message.
 */
typedef struct fbDcmSflowMessage_t_ {
	fbDcmCommandCode_t opcode;
	char               policy_name[FB_MAX_MAP_NAME_SIZE];
	fbDcmClassName_t   class_name;
	fbDcmMapAttr_t     sflow_attr;
}fbDcmSflowMessage_t;

/*
 * Dcmd scheduler Message.
 */
typedef struct fbDcmSchdAttr_t_ {
	uint8_t   sp_count;
	uint64_t  shape_bw[8];
	uint32_t  dwrr_wt[8];
}fbDcmSchdAttr_t;

typedef struct fbDcmSchdMessage_t_ {
	fbDcmCommandCode_t opcode;
	char               policy_name[FB_MAX_MAP_NAME_SIZE];
	fbDcmClassName_t   class_name;
	fbDcmSchdAttr_t    schd_attr;
}fbDcmSchdMessage_t;

/*
 * Dcmd port-Shape Message.
 */
typedef struct fbDcmPortshapeAttr_t_ {
	uint64_t   shape_rate; /* In Kbps */
}fbDcmPortshapeAttr_t;

typedef struct fbDcmPortshapeMessage_t_ {
	fbDcmCommandCode_t      opcode;
	char                    policy_name[FB_MAX_MAP_NAME_SIZE];
	fbDcmClassName_t        class_name;
	fbDcmPortshapeAttr_t    schd_attr;
}fbDcmPortshapeMessage_t;



typedef struct fbDcmPolicyClassMapActionMessage_t_ {
	fbDcmCommandCode_t  opcode;
	char                policy_name[FB_MAX_MAP_NAME_SIZE];
	fbDcmClassName_t    class_name;

	union {
		fbDcmPoliceAttr_t   	police_attr;
		fbDcmCosMarkAttr_t      cos_attr;
		fbDcmDscpMarkAttr_t     dscp_attr;
		fbDcmMapAttr_t          cosmutation_attr;
		fbDcmMapAttr_t          costrafficclass_attr;
		fbDcmMapAttr_t          dscpcos_attr;
		fbDcmMapAttr_t          dscptrafficclass_attr;
		fbDcmMapAttr_t          dscpmutation_attr;
		fbDcmMapAttr_t          sflow_attr;
		fbDcmPortshapeAttr_t    portshape_attr;
		fbDcmSchdAttr_t         schd_attr;
		fbDcmMapAttr_t          cee_attr;
	} action;

}fbDcmPolicyClassMapActionMessage_t;

/*
 * Dcmd bind/unbind message.
 */
typedef struct fbDcmBindPolicyMapMessage_t_ {
	fbDcmCommandCode_t opcode;
	char if_name[MAX_IF_NAME_LEN+4];
	char policy_name[FB_MAX_MAP_NAME_SIZE];
	fbPolicyDirection_t dir;
}fbDcmBindPolicyMapMessage_t;

/*************************************************************************
                    DCMD statistics structures 
 ************************************************************************/

/* Input structure for the command "show policy-map" */
typedef struct fbDcmShowPmapSummaryInputMessage_t_ {
	fbDcmCommandCode_t opcode;
}fbDcmShowPmapSummaryInputMessage_t;

/* Input structure for the command "show policy-map interface [[te|gi|fo] x/y [in|out]]" */
typedef struct fbDcmShowPmapIntInputMessage_t_ {
	fbDcmCommandCode_t opcode;
	char               if_name[MAX_STR+1];
	uint8_t            direction;
}fbDcmShowPmapIntInputMessage_t;

/* Input structure for the internal command "show policy-map class-map
 * 					interface [[te|gi|fo] x/y [in|out]]" */
typedef struct fbDcmShowPmapCmapIntInputMessage_t_ {
	fbDcmCommandCode_t opcode;
	char               if_name[MAX_STR+1];
	uint8_t            direction;
	char			   class_name[FB_MAX_MAP_NAME_SIZE];
}fbDcmShowPmapCmapIntInputMessage_t;

/* Input structure for the command "show policy-map detail <policyMapName>" */
typedef struct fbDcmShowPmapDetailsInputMessage_t_ {
	fbDcmCommandCode_t opcode;
	char               policy_name[FB_MAX_MAP_NAME_SIZE];
}fbDcmShowPmapDetailsInputMessage_t;

/* Input structure for the command "clear policy-map-counters [interface]" */
typedef struct fbDcmClearPmapCountersInputMessage_t_ {
	fbDcmCommandCode_t  opcode;
	char                if_name[MAX_IF_NAME_LEN+4];
	char                policy_name[FB_MAX_MAP_NAME_SIZE];
	fbPolicyDirection_t direction;
}fbDcmClearPmapCountersInputMessage_t;

typedef struct fbDcmPolStats_t_ {
	uint64_t oper_cir;
	uint64_t oper_cbs;
	uint64_t oper_eir;
	uint64_t oper_ebs;
	uint64_t conform_byte;
	uint64_t exceed_byte;
	uint64_t violate_byte;
}fbDcmPolStats_t;

typedef struct fbDcmPerActionRecord_t_ {
#define SET_COS                   0x1
#define SET_DSCP                  0x2
#define DSCP_DSCP_MUTATION        0x4
#define DSCP_COS_MUTATION         0x8
#define DSCP_TC_MUTATION          0x10
#define COS_COS_MUTATION          0x20
#define COS_TC_MUTATION           0x40
#define PORTSHAPE                 0x80
#define POLICE                    0x100
#define SFLOW                     0x200
#define SCHD                      0x400
#define CEE                       0x800
	uint32_t                    action_id;
	union {
		fbDcmPoliceAttr_t       police_attr;
		fbDcmCosMarkAttr_t      cos_attr;
		fbDcmDscpMarkAttr_t     dscp_attr;
		fbDcmMapAttr_t          mutation_attr;
		fbDcmMapAttr_t          sflow_attr;
		fbDcmPortshapeAttr_t    portshape_attr;
		fbDcmSchdAttr_t         schd_attr;
		fbDcmMapAttr_t          cee_attr;
	}action_attr;
	uint32_t                    is_stats_valid:1;
	uint32_t                    reserve:31;
	union {
		fbDcmPolStats_t         police_stats;
	}action_stats;
}fbDcmPerActionRecord_t;

typedef struct fbDcmPerClassRecord_t_ {
	char                     class_name[FB_MAX_MAP_NAME_SIZE];
	uint32_t                 is_stats_valid:1;
	uint32_t                 reserved:31;
	uint32_t                 num_action_record;
	uint64_t                 packet_count;
	uint64_t                 byte_count;
	fbDcmPerActionRecord_t   action_record[1];
}fbDcmPerClassRecord_t;

//struct to store the policer stats, the struct is unique for a given policy-map and class-map combination
typedef struct fbDcmFlowBasedStats_t_ {
	uint32_t is_class_stats_valid:1;
	uint32_t is_policer_stats_valid:1;
	uint64_t packet_count;
	uint64_t byte_count;
	uint64_t conform_byte;
	uint64_t exceed_byte;
	uint64_t violate_byte;
} fbDcmFlowBasedStats_t;


typedef struct fbDcmShowPmapIntOutputMessage_t_ {
	size_t					data_size;
	char                    pmap[FB_MAX_MAP_NAME_SIZE];
	uint8_t                 direction;
	uint32_t                num_class_record;
	fbDcmPerClassRecord_t   class_record[1];
}fbDcmShowPmapIntOutputMessage_t;

typedef struct fbDcmBindInstance_t_ {
	char                    if_name[STR32];
	fbPolicyDirection_t     dir;
}fbDcmBindInstance_t;

typedef struct fbDcmBindList_t_ {
	uint32_t                num_bind_inst;
	fbDcmBindInstance_t     bind_inst[1];
}fbDcmBindList_t;

typedef struct fbDcmShowPmapDetailOutputMessage_t_ {
	size_t					data_size;
	char                    pmap[FB_MAX_MAP_NAME_SIZE];
	uint32_t                num_class_record;
	fbDcmBindList_t         bind_list;
	fbDcmPerClassRecord_t   class_record[1];
}fbDcmShowPmapDetailOutputMessage_t;

typedef struct fbDcmPmapInst_t_ {
	char                    pmap[FB_MAX_MAP_NAME_SIZE];
	fbDcmBindList_t         bind_list;
}fbDcmPmapInst_t;

typedef struct fbDcmShowPmapOutputMessage_t_ {
	size_t					data_size;
	uint32_t                num_policy_record;
	fbDcmPmapInst_t         pmap_inst[1];
}fbDcmShowPmapOutputMessage_t;

typedef struct fbDcmClearPmapCountersOutputMessage_t_ {
	fbDcmCommandCode_t opcode;
	int status;
}fbDcmClearPmapCountersOutputMessage_t;

#endif
