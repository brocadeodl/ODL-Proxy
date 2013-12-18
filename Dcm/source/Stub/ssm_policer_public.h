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
 *  Module name: ssmPolicer_public.h
 *
 *  Description:
 *  declares  policer structure.
 */

#ifndef _SSMPOLICER_PUBLIC_H_
#define _SSMPOLICER_PUBLIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#include "ssmAcl_public.h"
//#include "utils/utils.h"

#define MAX_IF_NAME_LEN 64
#define SSM_POL_MAX_NAME_SIZE 64
#define SSM_POL_NUM_COS 8
#define POLICER_NOT_USR_CFGED_VAL 64
#define POLICER_NOT_USR_CFGED_CIR_CBS 0xFFFFFFFFFFFFLLU
#define SSM_POL_DEFAULT_PRECEDENCE 10

enum ssm_pol_policy_match_criteria {
    SSM_POL_POLICY_RULE_MATCH_ANY_E, // Rule criteria match any
    SSM_POL_POLICY_RULE_MATCH_ACL_E // Rule criteria match acl
};

enum ssm_pol_intf_direction {
    SSM_POL_INTF_INGRESS_E = 0, // Ingress direction
    SSM_POL_INTF_EGRESS_E,  // Egress direction
	SSM_POL_INTF_BOTH_E //Both Ingress and Egress
};

/**
 * Priority map object supplied by the user
 *
 * This object is used to do color based priority mapping. If priority map
 * is not defined for particular color, then default priority map is used
 */

typedef struct ssm_pol_prio_map_s {
    char name[SSM_POL_MAX_NAME_SIZE+1];
    uint16_t flags;
    /**< Flags are used to identify whether priority map for particular color
     * set or not */
    uint8_t conform_cos[SSM_POL_NUM_COS];
    /**< Priority map for conformant(green color) packets */
    uint8_t exceed_cos[SSM_POL_NUM_COS];
    /**< Priority map for Exceeded(yellow color) packets */
    uint8_t violate_cos[SSM_POL_NUM_COS];
    /**< Priority map for violated (red color) packets */
	uint16_t set_conform_cos:1;
    uint16_t set_exceed_cos:1;
    uint16_t set_violate_cos:1;
    uint16_t reserved:13;
    /**< Flags are used to identify whether priority map for particular color
     * set or not */
}ssm_pol_prio_map_t;

/**
 * Policy match rule for classifying flows
 *
 * This object is used to maintain match rule criteria and corresponding
 * match rule value. In case of match rule any criteria, policy rule value
 * is not used
*/
typedef struct ssm_pol_policy_match_rule_s {
    enum ssm_pol_policy_match_criteria  match_criteria;
	union {
		char acl_name[SSMACL_NAME_SIZE];
		/**< acl name is used for match rule criteria match acl */
	}u;
}ssm_pol_policy_match_rule_t;

/**
 * Class map object is supplied by the user.
 *
 * This map is used to classify the flows.
 */
typedef struct ssm_pol_class_map_s {
    char name[SSM_POL_MAX_NAME_SIZE+1];
}ssm_pol_class_map_t;


/**
 * Policy map for applying policing actions on packets
 *
 * Policy map is associated with interface on both ingress and
 * egress directions
 */
typedef struct ssm_pol_policy_map_s {
    char name[SSM_POL_MAX_NAME_SIZE+1];
}ssm_pol_policy_map_t;

/**
 * Policer attributes are performed based on the classification nof packet
 */
typedef struct ssm_pol_policer_attr_s {
    char    prio_map_name[SSM_POL_MAX_NAME_SIZE+1];
    uint8_t conform_ip_tos; // conform IP precedence or dscp value
    uint8_t conform_traffic_class; // conform traffic class value
    uint8_t exceed_ip_tos; // exceed IP precedence or dscp value
    uint8_t exceed_traffic_class; // exceed traffic class value
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
}ssm_pol_policer_attr_t;

 // revisit this structure
/**
 * Policy class map contains clas map for classifying flow and policer
 * atrributes to apply policer actions after classification. policy class
 * maps are maintained per policy map.
 */
typedef struct ssm_pol_policy_class_map_s {
    ssm_pol_class_map_t *class_map;
    ssm_pol_policer_attr_t police_attr;
}ssm_pol_policy_class_map_t;

#endif

