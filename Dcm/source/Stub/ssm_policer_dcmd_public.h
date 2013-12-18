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

/**
 *  Copyright (c) 2011 by Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Module name: ssm_policer_common.h
 *
 *  Description:
 *  declares APIs to acces policer structure.
 */

#ifndef __SSM_POLICER_DCMD_PUBLIC_H_
#define __SSM_POLICER_DCMD_PUBLIC_H_

#include "ssm_policer_public.h"



typedef enum
{
	SSM_POL_UNKNOWN = 0,
	SSM_POL_PRIORITYMAP_CREATE,
	SSM_POL_PRIORITYMAP_COS_SET,
	SSM_POL_PRIORITYMAP_COS_CLEAR,
	SSM_POL_PRIORITYMAP_DELETE,
	SSM_POL_CLASSMAP_CREATE,
	SSM_POL_CLASSMAP_DELETE,
    SSM_POL_ADD_CLASS_MAP_TO_POLICY_MAP,
    SSM_POL_DEL_CLASS_MAP_TO_POLICY_MAP,
    SSM_POL_SET_POLICER_ATTR,
    SSM_POL_CLEAR_POLICER_ATTR,
	SSM_POL_POLICYMAP_CREATE,
	SSM_POL_POLICYMAP_DELETE,
	SSM_POL_POLICYMAP_BIND_INTERFACE,
    SSM_POL_POLICYMAP_UNBIND_INTERFACE,
	SSM_POL_SHOW_POLICYMAP_INTERFACE,
    SSM_POL_SHOW_POLICYMAP_ALL,
    SSM_POL_CLEAR_STATS_INTERFACE,
    SSM_POL_CLEAR_STATS_ALL,
	SSM_POL_CLEAR_STATS_POLICY
}ssm_pol_cmd_code;


/**
 * Used for
 * SSM_POL_PRIORITYMAP_CREATE
 * SSM_POL_PRIORITYMAP_DELETE
 */
typedef struct ssm_pol_prio_map_msg_
{
    ssm_pol_cmd_code opcode;
    ssm_pol_prio_map_t prio_map;
}ssm_pol_prio_map_msg_t;


typedef struct ssm_pol_class_map_msg_
{
    ssm_pol_cmd_code opcode;
    ssm_pol_class_map_t class_map;
    enum ssm_pol_policy_match_criteria rule;
}ssm_pol_class_map_msg_t;

typedef struct ssm_pol_policy_map_msg_
{
    ssm_pol_cmd_code opcode;
    ssm_pol_policy_map_t policy_map_name;
}ssm_pol_policy_map_create_msg_t;
/*
 * SSM_POL_ADD_CLASS_MAP_TO_POLICY_MAP
 * SSM_POL_DEL_CLASS_MAP_TO_POLICY_MAP
 * Clear Attrib
*/ 
typedef struct ssm_pol_policy_class_map_create_msg_
{
    ssm_pol_cmd_code opcode;
    ssm_pol_policy_map_t policy_map_name;
    ssm_pol_class_map_t class_map_name;
}ssm_pol_policy_class_map_create_msg_t;

/**
 * Set Attributes
 */
typedef struct ssm_pol_policy_class_map_msg_
{
    ssm_pol_cmd_code opcode;
    ssm_pol_policy_map_t policy_map_name;
    ssm_pol_class_map_t class_map_name;
    ssm_pol_policer_attr_t policer_attr;
}ssm_pol_policy_class_map_msg_t;

/*this structure is sent when policymap is added to an interface,
opcode :- to add or delete the policy from interface.
cmdcode:- this is an input map or output map values will be POLICER_INPUT_MAP, POLICER_OUTPUT_MAP,*/

typedef struct ssm_pol_bind_policymap_msg_
{
    ssm_pol_cmd_code opcode;
    char if_name[MAX_IF_NAME_LEN+4];
    ssm_pol_policy_map_t policy_map_name;
    enum ssm_pol_intf_direction cmdcode;
}ssm_pol_bind_policymap_msg_t;

/*this structure is sent during postboot to add policymap to an interface*/
typedef struct ssm_pol_bind_policymap_bulk_msg_
{
	char if_name[MAX_IF_NAME_LEN+4];
	ssm_pol_policy_map_t input_policy_map_name;
	ssm_pol_policy_map_t output_policy_map_name;
	enum ssm_pol_intf_direction direction;
}ssm_pol_bind_policymap_bulk_msg_t;

/*this is sent as an input to the show cmds.
direction :- inpur or output map POLICER_INPUT_MAP, POLICER_OUTPUT_MAP.
cmdCode :- codes mentioned in PolicerShow.
*/
typedef struct ssm_pol_show_or_clr_input_msg_{
    ssm_pol_cmd_code cmdcode;
    char interface[MAX_IF_NAME_LEN+4];
    ssm_pol_policy_map_t map_name;
	enum ssm_pol_intf_direction direction;
} ssm_pol_show_or_clr_input_msg_t;


typedef struct ssm_pol_class_map_stats_data_ {
    ssm_pol_class_map_t class_map_name;
    enum ssm_pol_policy_match_criteria match_crit;
    ssm_pol_policer_attr_t policer_attr;
    uint32_t dnld_status:1;
    uint32_t reserved:1;
    uint32_t padding:30;
    uint64_t conform_byte_count;
    uint64_t exceed_byte_count;
    uint64_t violate_byte_count;
} ssm_pol_class_map_stats_data_t;

/*policyMap contains all class maps attached to the policy*/
typedef struct ssm_pol_show_stats_data_ {
    char interface[MAX_IF_NAME_LEN+4];
    ssm_pol_policy_map_t policy_name;
    enum ssm_pol_intf_direction direction;
    uint32_t num_class_maps;
	uint8_t lossless_prio[8];
    ssm_pol_class_map_stats_data_t stats_data[1];
} ssm_pol_show_stats_data_t;

/*this structure has to be sent by backend to plugin
No of records contains number of interfaces whose details has been sent
num_of_class_maps contains total number of class maps in all these interface maps*/
typedef struct ssm_pol_show_stats_detail_msg_ {
    ssm_pol_cmd_code cmdcode;
    unsigned int num_records;
    size_t data_size;
    ssm_pol_show_stats_data_t records[1];
}ssm_pol_show_stats_detail_msg_t;

typedef struct ssm_pol_show_data_ {
   char interface[MAX_IF_NAME_LEN+4];
   ssm_pol_policy_map_t policy_map_name_ingress;
   ssm_pol_policy_map_t policy_map_name_egress;
}ssm_pol_show_data_t;

typedef struct ssm_pol_show_stats_summary_msg_ {
    ssm_pol_cmd_code cmdcode;
    size_t data_size;
    unsigned int num_records;
    ssm_pol_show_data_t record[1];
}ssm_pol_show_stats_summary_msg_t;


typedef struct ssm_pol_clear_stats_msg_ {
    ssm_pol_cmd_code cmdcode;
    int status;
}ssm_pol_clear_stats_msg_t;

#endif

