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
 * All rights reserved
 *
 * @file ssmAcl_public.h
 *
 * @brief
 *
 * @author Alan Bolton
 *
 * @date 2/08/2011
 *
 */
#ifndef _SSMACL_PUBLIC_H_
#define _SSMACL_PUBLIC_H_
/*
* Included Files
*/
#include "linux/types.h"
#include "stdint.h"

/*
* Typedef, enumeration, structure definitions
*/

/// Macro to define the valid bit within a "key's" valid field
#define SSMACL_KEY_ELEMENT_VALID     0x80000000
#define SSMACL_KEY_ELEMENT_INVALID   0

// Macros for Policer field in the ACL rule
#define SSMACL_NO_POLICING           0x40000000

// Macro to enable L3inL2. ie overrride L2 results with L3 results
#define SSMACL_RESULT_L3_OVERRIDE_EN       0x00000001

// Macros for the keys valid field for IP And MAC
#define SSMACL_KEY_ADDR_MASK_HOST            0x01
#define SSMACL_KEY_ADDR_MASK_USER_SPECIFIED  0x02
#define SSMACL_KEY_ELEMENT_ANY               0x04

// Macros for the keys valid field for IP TOS and Prec
#define SSMACL_KEY_TOS_VALID         0x10
#define SSMACL_KEY_PRECEDENCE_VALID  0x20
#define SSMACL_KEY_DSCP_VALID        0x40

// Macros for the keys valid field for VLAN
#define SSMACL_KEY_ELEMENT_VLAN_ADD   0x01 // Update Key - VLAN Added
#define SSMACL_KEY_ELEMENT_VLAN_DEL   0x02 // Update Key - VLAN Deleted
#define SSMACL_KEY_ELEMENT_VLAN_ALL   0x04 // Key - All VLANs for the interface



/// Macro to define the maximum allowable ACL name size
#define SSMACL_NAME_SIZE        64
#define SSMACL_BIND_STR_SIZE    80
#define SSMACL_INTF_LONG_NAME   64
#define SSMACL_DESC_LEN         120
#define SSMACL_VRF_NAME_SIZE    64

/**
 * Enumeration to define the specific ACL type
 *
 */
enum ssmAcl_type
{
   SSMACL_TYPE_UNKNOWN_E = 0,
   SSMACL_TYPE_L2_E,
   SSMACL_TYPE_L2_STD_E,
   SSMACL_TYPE_L2_EXT_E,
   SSMACL_TYPE_L3_IPV4_E,
   SSMACL_TYPE_L3_IPV4_STD_E,
   SSMACL_TYPE_L3_IPV4_EXT_E,
   SSMACL_TYPE_L3_IPV4_FLEX_E,
   SSMACL_TYPE_L3_IPV6_E
};

enum ssmAcl_binding_direction
{
   SSMACL_BINDING_DIRECTION_UNKNOWN_E = 0,
   SSMACL_BINDING_DIRECTION_IN_E,
   SSMACL_BINDING_DIRECTION_OUT_E,
   SSMACL_BINDING_DIRECTION_BOTH_E
};

enum ssmAcl_binding_status
{
   SSMACL_BINDING_STATUS_INVALID = 0,
   SSMACL_BINDING_STATUS_SUCCESS_E,
   SSMACL_BINDING_STATUS_FAIL_E,
   SSMACL_BINDING_STATUS_PARTIAL_E,
   SSMACL_BINDING_STATUS_INPROGRESS_E,
   SSMACL_BINDING_STATUS_UNCOUNT_E /* Counter requested but not allocated*/
};

/**
 * Enumeration to define the ACL Forwarding result
 * This enumeration is used to decide the forwarding result set in TCAM result.
 */
enum ssmAcl_forwarding_result
{
   SSMACL_FORWARDING_RESULT_INVALID_E = 0,
   SSMACL_FORWARDING_RESULT_PERMIT_E,
   SSMACL_FORWARDING_RESULT_SOFT_DROP_E,
   SSMACL_FORWARDING_RESULT_HARD_DROP_E,
   SSMACL_FORWARDING_RESULT_TRAP_E,
   SSMACL_FORWARDING_RESULT_FORWARD_E
};

/**
 * Enumeration to define the supplimentary result
 *
 * To allow combination of supplementary actions to be specified the
 * enumerations for this field are bit mask values. TCAM library will program
 * supplementary actions only if the underlying h/w supports the capability
 */
enum ssmAcl_supplementary_result
{
   SSMACL_SUPPLEMENTAL_RESULT_INVALID_E      = 0x00,
   SSMACL_SUPPLEMENTAL_RESULT_COUNT_E        = 0x01, /**< Counter per rule */
   SSMACL_SUPPLEMENTAL_RESULT_LOG_E          = 0x02,
   SSMACL_SUPPLEMENTAL_RESULT_SHARED_COUNT_E = 0x04, /**< Counter per ACL */
   SSMACL_SUPPLEMENTAL_RESULT_TRAP_E         = 0x08  /**< Trap the frame also */
};

/**
 * Enumeration to define the allowable protocol port range operations
 *
 * When configuring a user L3 extended ACL the user is allowed to specify or
 * filter on specific source or destination protocol port ranges. When
 * specifying port ranges the user must provide the type of operation desired
 * on the port values entered.
 *
 * Begining it from 1 as in MO default value given to port operator is 0.
 * 0 is obvious choice for operator as port value can not be 0 ideally.
 * If we do not assign default value to non mandatory leafs then show running 
 * config will not work properly.
 *
 * These enum values should always match the enum values defined in 
 * brocade-l3-access-list.yang for source and destination Port operators
 */
enum ssmAcl_port_range_operators
{
   SSMACL_PORT_RANGE_OPERATOR_EQUAL_E = 1,
   SSMACL_PORT_RANGE_OPERATOR_GREATER_THAN_E,
   SSMACL_PORT_RANGE_OPERATOR_LESS_THAN_E,
   SSMACL_PORT_RANGE_OPERATOR_NOT_EQUAL_E,
   SSMACL_PORT_RANGE_OPERATOR_RANGE_E
};

/**
 * Enumeration to define the allowable states of flags in an ACL rule
 *
 * When configuring a user L3 extended ACL the user is allowed to filter on
 * specific flag values like TCP flags (when the protocol field of the rule is set to
 * TCP). The key element tcp_flag uses these values to indicate the specific
 * state the flag must be set to in order for the rule to be considered a match
 */
enum ssmAcl_flag_state
{
   SSMACL_FLAG_STATE_DONTCARE_E = 0,
   SSMACL_FLAG_STATE_SET_E,
   SSMACL_FLAG_STATE_CLEARED_E
};

enum ssmAcl_l2_addr_lookup_result
{
   SSMACL_L2_ADDR_LOOKUP_DONT_CARE_E,
   SSMACL_L2_ADDR_LOOKUP_HIT_E,
   SSMACL_L2_ADDR_LOOKUP_MISS_E
};

/**
 * Data Type for representing a mac address as an element of the ACL key
 *
 * The valid element is a concatenation of a "valid" bit (defined by
 * SSMACL_KEY_ELEMENT_VALID) and a value indicating the user entered address
 * mask format. The mask is always carried as a 48 bit value. The LSBs have the
 * following meaning
 *  1 = mask entered as "host"
 *  2 = mask was entered xxxx.xxxx.xxxx notation
 */
struct ssmAcl_key_element_mac_addr
{
   uint32_t valid; /**< equal to SSMACL_KEY_ELEMENT_VALID when valid */
   uint8_t  addr[6];
   uint8_t  mask[6];
};

/**
 * Data Type for representing an IP address as an element of the ACL key
 *
 * The valid element is a concatenation of a "valid" bit (defined by
 * SSMACL_KEY_ELEMENT_VALID) and a value indicating the user entered address
 * mask format. The mask is always carried as a 32 bit value. The LSBs have the
 * following meaning
 *  1 = mask entered as "host"
 *  2 = mask was entered in a.b.c.d notation
 *  3 = mask was entered as /x notation
 */
struct ssmAcl_key_element_ip_addr
{

   uint32_t valid; /**< equal to SSMACL_KEY_ELEMENT_VALID when valid */
   uint32_t addr;
   uint32_t mask;
};

/**
 * Data Type for representing any element of the ACL key requiring 8 bits
 */
struct ssmAcl_key_element_octet
{
   uint32_t valid; /**< equal to SSMACL_KEY_ELEMENT_VALID when valid */
   uint8_t  value;
   uint8_t  mask;
};

/**
 * Data Type for representing any element of the ACL key requiring 16 bits
 */
struct ssmAcl_key_element_uint16
{
   uint32_t valid; /**< equal to SSMACL_KEY_ELEMENT_VALID when valid */
   uint16_t value;
   uint16_t mask;
};

/**
 * Data Type for representing any element of the ACL key requiring 32 bits
 */
struct ssmAcl_key_element_uint32
{
   uint32_t valid; /**< equal to SSMACL_KEY_ELEMENT_VALID when valid */
   uint32_t value;
   uint32_t mask;
};

/**
 * Data type for represent any element of ACL key requiring 1 bit
 */
struct ssmAcl_key_element_flag
{
    uint32_t valid; /**< equal to SSMACL_KEY_ELEMENT_VALID when valid */
    enum ssmAcl_flag_state flag;
};

/**
 * Data Type for representing protocol port ranges within the ACL key
 */
struct ssmAcl_key_element_protocol_port
{
   uint32_t valid; /**< equal to SSMACL_KEY_ELEMENT_VALID when valid */
   enum ssmAcl_port_range_operators oper;
   uint16_t starting_port_number;
   uint16_t ending_port_number;
};

/**
 * Data Type for representing the TCP protocol flags within an ACL key
 */
struct ssmAcl_key_element_tcp_flags
{
   uint32_t valid; /**< equal to SSMACL_KEY_ELEMENT_VALID when valid */
   enum ssmAcl_flag_state ack;
   enum ssmAcl_flag_state fin;
   enum ssmAcl_flag_state rst;
   enum ssmAcl_flag_state sync;
   enum ssmAcl_flag_state push;
   enum ssmAcl_flag_state urg;
};


struct ssmAcl_key_element_ip_frag_flags
{
    uint32_t valid; /**< equal to SSMACL_KEY_ELEMENT_VALID when valid */
    enum ssmAcl_flag_state reserved;
    enum ssmAcl_flag_state dont_frag;
    enum ssmAcl_flag_state more_frag;
};

struct ssmAcl_key_element_vrf_name
{
    uint32_t valid; /**< equal to SSMACL_KEY_ELEMENT_VALID when valid */
    uint16_t vrf_id;
    uint16_t reserved;
    char vrf_name[SSMACL_VRF_NAME_SIZE];
};

/**
 * Data Type for representing the actions to take when the rule is matched
 *
 * there is only one forwarding action which is considered the primary action
 * this can be deny, permit, or trap. There can be a number of combination of
 * supplemental actions. Supplemental actions are logging, counting, metering
 * etc.
 */
struct ssmAcl_result
{
   enum ssmAcl_forwarding_result    forwarding;
   // Use enum ssmAcl_supplementary_result to set supplemental result
   uint16_t supplemental;
   uint8_t trap_queue;
   uint8_t reserved;
};

/**
 * Information that all ACLs contain regardless of their type
 *
 * Every ACL rule will contain a sequence number and some type of action(s)
 * regardless of it type. These elements are referred to as generic elements of
 * the rule. All APIs and abstracted functions will take pointer to this object
 * when requiring a "reference". The called function will know the type of ACL
 * and rule it is operating on and will cast the referenced passed in into the
 * specific rule it is acting on (l2 or l3)
 */
struct ssmAcl_rule_common
{
   uint32_t seq_num; /* ACL Rule seq number */
   uint16_t rule_id; /* Backend Internal Rule ID */
   uint16_t reserved;
   char rule_desc[SSMACL_DESC_LEN+4];
   struct ssmAcl_result results;
};

/**
 * Data Type used for both L2 Standard and L2 extended ACLs.
 *
 * the same data type is used as standard ACLs are simply a subset of extended
 * ACLs
 */
struct ssmAcl_rule_l2_obj
{
   struct ssmAcl_rule_common           generic;
   struct ssmAcl_key_element_mac_addr  src_mac;
   struct ssmAcl_key_element_mac_addr  dst_mac;
   struct ssmAcl_key_element_uint16    ether_type;
   struct ssmAcl_key_element_octet     cos;
   struct ssmAcl_key_element_octet     tos;
   enum ssmAcl_l2_addr_lookup_result   sa_res; /* L2 Lookup on Src Addr */
   enum ssmAcl_l2_addr_lookup_result   da_res; /* L2 Lookup on Dst Addr */
   struct ssmAcl_key_element_uint16    vlan_id; /* VLAN ID */
};

/**
 * Data Type used for both L3 Standard and L3 extended ACLs.
 *
 * the same data type is used as standard ACLs are simply a subset of extended
 * ACLs
 */
struct ssmAcl_rule_l3_ipv4_obj
{
   struct ssmAcl_rule_common               generic;
   struct ssmAcl_key_element_octet         ip_protocol;
   struct ssmAcl_key_element_ip_addr       src_ip;
   struct ssmAcl_key_element_protocol_port src_protocol_port;
   struct ssmAcl_key_element_ip_addr       dst_ip;
   struct ssmAcl_key_element_protocol_port dst_protocol_port;
   struct ssmAcl_key_element_octet         cos;
   struct ssmAcl_key_element_octet         tos;
   struct ssmAcl_key_element_tcp_flags     tcp_flags;
   struct ssmAcl_key_element_uint16 vlan_id; /* VLAN ID */
   struct ssmAcl_key_element_vrf_name vrf_name;
};

enum ssmAcl_l3_ipv4_flex_key_profile
{
    SSMACL_L3_IPV4_FLEX_KEY_PROFILE_1_E, /* Key profile 1 */
    SSMACL_L3_IPV4_FLEX_KEY_PROFILE_MAX_E
};

struct ssmAcl_rule_l3_ipv4_flex_rule_profile_1
{
   struct ssmAcl_key_element_octet cos;
   struct ssmAcl_key_element_uint16 frag_offset;
   struct ssmAcl_key_element_ip_frag_flags frag_flags;
   struct ssmAcl_key_element_uint16 ip_tlen;
   struct ssmAcl_key_element_octet tos;
   struct ssmAcl_key_element_octet ip_ttl;
   struct ssmAcl_key_element_octet ip_protocol;
   struct ssmAcl_key_element_ip_addr src_ip;
   struct ssmAcl_key_element_ip_addr dst_ip;
   struct ssmAcl_key_element_protocol_port src_protocol_port;
   struct ssmAcl_key_element_protocol_port dst_protocol_port;
   struct ssmAcl_key_element_tcp_flags tcp_flags;
};

struct ssmAcl_rule_l3_ipv4_flex_obj
{
   struct ssmAcl_rule_common               generic;

   struct ssmAcl_key_element_flag is_fragment;
   struct ssmAcl_key_element_flag is_my_ip_da;
   struct ssmAcl_key_element_octet apptype;

   enum ssmAcl_l3_ipv4_flex_key_profile profile;
   union
   {
      struct ssmAcl_rule_l3_ipv4_flex_rule_profile_1 profile1;
   } flex_key;
};

struct ssmAcl_rule_status
{
   uint64_t hit_count;
   uint64_t byte_count;
   enum ssmAcl_binding_status hw_programming;

   union
   {
      struct ssmAcl_rule_l2_obj l2_rule;
      struct ssmAcl_rule_l3_ipv4_obj l3_rule;
      struct ssmAcl_rule_l3_ipv4_flex_obj l3_flex_rule;
   };
};

struct ssmAcl_acl_status
{
   uint32_t num_of_rules;
   enum ssmAcl_binding_status acl_status;
   struct ssmAcl_rule_status rule[1];
};

struct ssmAcl_status_acl_record
{
   char name[SSMACL_NAME_SIZE];
   enum ssmAcl_type type;
   enum ssmAcl_binding_direction direction;
   char bind_src[SSMACL_BIND_STR_SIZE];
   struct ssmAcl_acl_status data;
};

struct ssmAcl_status_intf_record
{
   char if_name[SSMACL_INTF_LONG_NAME];
   uint16_t  num_acls;
   uint16_t  reserved;
   struct ssmAcl_status_acl_record acl_rec[1];
};

struct ssmAcl_status_resp_data
{
   uint16_t num_intf;
   uint16_t reserved;
   struct ssmAcl_status_intf_record intf_rec[1];
};

/*
* Exported Functions
*/

#endif /*_SSMACL_PUBLIC_H_*/
