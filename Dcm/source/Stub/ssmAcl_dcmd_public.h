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
 * Copyright (c) 2009 by Brocade Communications Systems, Inc.
 * @file ssmAcl_dcmd_public.h
 * @brief This header f
 * @author Alan Bolton
 * @date 2/08/2011
 *
 */
#ifndef _SSMACL_DCMD_PUBLIC_H_
#define _SSMACL_DCMD_PUBLIC_H_
/*
 * Included Files
 */

#include "ssmAcl_public.h"


/*
 * Typedef, enumeration, structure definitions
 */

typedef enum
{
   SSMACL_DCMD_CREATE_L2_ACL = 0,
   SSMACL_DCMD_DELETE_L2_ACL,
   SSMACL_DCMD_BIND_L2_ACL,
   SSMACL_DCMD_UNBIND_L2_ACL,
   SSMACL_DCMD_CREATE_L3_IPV4_ACL,
   SSMACL_DCMD_DELETE_L3_IPV4_ACL,
   SSMACL_DCMD_BIND_L3_IPV4_ACL,
   SSMACL_DCMD_UNBIND_L3_IPV4_ACL,
   SSMACL_DCMD_ADD_RULE,
   SSMACL_DCMD_DEL_RULE,
   SSMACL_DCMD_GET_HIT_COUNTS,
   SSMACL_DCMD_GET_STATUS,
   SSMACL_DCMD_CLEAR_STATS_ALL,
   SSMACL_DCMD_CLEAR_STATS_ON_INTERFACE,
   SSMACL_DCMD_CLEAR_STATS_ACL,
   SSMACL_DCMD_CLEAR_STATS_ACL_ON_INTERFACE,
   /* Ip + Mac Show Access-list opcodes */
   SSMACL_DCMD_SHOW_ACCESSLIST_ALL,
   SSMACL_DCMD_SHOW_ACCESSLIST_ACL,
   SSMACL_DCMD_SHOW_ACCESSLIST_ON_INTERFACE,
   SSMACL_DCMD_SHOW_ACCESSLIST_ACL_ON_INTERFACE,
   /* Ip + Mac Show statistics opcodes */
   SSMACL_DCMD_SHOW_STATS_ACL,
   SSMACL_DCMD_SHOW_STATS_ON_INTERFACE,
   SSMACL_DCMD_SHOW_STATS_ACL_ON_INTERFACE,
   SSMACL_DCMD_SHOW_MAC_ACCESS_GROUP,
   SSMACL_DCMD_SHOW_MAC_ACCESS_GROUP_INTF,
   SSMACL_DCMD_RESEQUENCE
} ssmAcl_dcmd_opcode_t;

typedef struct ssmAcl_dcmd_acl_show_request_
{
   ssmAcl_dcmd_opcode_t opcode;
   enum ssmAcl_type type;
   char acl_name[SSMACL_NAME_SIZE];
   char if_name[SSMACL_INTF_LONG_NAME];
   enum ssmAcl_binding_direction direction;
   uint32_t detail;
   uint32_t bind_oid;
} ssmAcl_dcmd_acl_show_request_t;

typedef struct ssmAcl_dcmd_acl_
{
   ssmAcl_dcmd_opcode_t opcode;
   enum ssmAcl_type type;
   char acl_name[SSMACL_NAME_SIZE];
   char if_name[SSMACL_INTF_LONG_NAME];
   enum ssmAcl_binding_direction direction;
} ssmAcl_dcmd_acl_t;

typedef struct ssmAcl_dcmd_l2_rule_
{
   ssmAcl_dcmd_opcode_t opcode;
   char acl_name[SSMACL_NAME_SIZE];
   struct ssmAcl_rule_l2_obj rule;
   uint8_t user_entered_sequence_number; /*1 = user entered 0 = seq# not valid*/
   uint8_t padding[3];
} ssmAcl_dcmd_l2_rule_t;

typedef struct ssmAcl_dcmd_l3_rule_
{
   ssmAcl_dcmd_opcode_t opcode;
   char acl_name[SSMACL_NAME_SIZE];
   struct ssmAcl_rule_l3_ipv4_obj rule;
   uint8_t user_entered_sequence_number; /*1 = user entered 0 = seq# not valid*/
   uint8_t padding[3];
} ssmAcl_dcmd_l3_rule_t;

typedef struct ssmAcl_dcmd_resequence_
{
   ssmAcl_dcmd_opcode_t opcode;
   char acl_name[SSMACL_NAME_SIZE];
   uint32_t seqId;
   uint32_t incSeqId;
   enum ssmAcl_type type;
} ssmAcl_dcmd_resequence_t;

typedef struct ssmAcl_dcmd_l2_rule_bulk_ 
{
   ssmAcl_dcmd_opcode_t opcode;
   char acl_name[SSMACL_NAME_SIZE];
   enum ssmAcl_type type;
   uint64_t numrules;
   ssmAcl_dcmd_l2_rule_t rules[1];
}ssmAcl_dcmd_l2_rule_bulk_t;

typedef struct ssmAcl_dcmd_l3_rule_bulk_ 
{
   ssmAcl_dcmd_opcode_t opcode;
   char acl_name[SSMACL_NAME_SIZE];
   enum ssmAcl_type type;
   uint64_t numrules;
   ssmAcl_dcmd_l3_rule_t rules[1];
}ssmAcl_dcmd_l3_rule_bulk_t;

typedef struct ssmAcl_dcmd_binding_bulk_ 
{
   uint64_t numrecords;
   ssmAcl_dcmd_acl_t binding_list[1];
}ssmAcl_dcmd_binding_bulk_t;

struct ssmAcl_dcmd_status_response
{
   ssmAcl_dcmd_opcode_t opcode; //!< Opcode is used to interpret resp buffer
   uint8_t more_elements;
   uint8_t reserved[3];
   uint32_t bind_oid;
   struct ssmAcl_status_resp_data resp_data;
};

/*
 * Exported Functions
 */

#endif /*_SSMACL_DCMD_PUBLIC_H_*/

/* vim:set ts=3 expandtab tw=79: */
