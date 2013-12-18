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
 * Copyright (c) 2012 by Brocade Communications Systems, Inc.
 * All rights reserved.
 *
 * @file
 *
 * @brief
 *
 * @author
 *
 * @date
 */
#ifndef _SSMPBR_PUBLIC_H_
#define _SSMPBR_PUBLIC_H_

#include "ssmd_pbr_public.h"
#include "ssmAcl_public.h"

/*
 * FIXME we need to find a way to include ssm_sysdef.h to have a common
 * definition ... or someway to share this define in a truly generic system
 * header file
 */
#define SSMPBR_MAX_VRF_NAME_SIZE 256

#define SSMPBR_ROUTE_MAP_NAME_SIZE    64
#define SSMPBR_INTF_LONG_NAME         64
#define SSMPBR_MAX_NHOPS_PER_STANZA   3

enum ssmPbr_stanza_action
{
   SSMPBR_STANZA_ACTION_PERMIT_E,
   SSMPBR_STANZA_ACTION_DENY_E
};

enum ssmPbr_stanza_status
{
   SSMPBR_STANZA_INVALID_E,
   SSMPBR_STANZA_INPROGRESS_E,
   SSMPBR_STANZA_ACTIVE_E,
   SSMPBR_STANZA_PART_ACTIVE_E,
   SSMPBR_STANZA_INACTIVE_E
};

enum ssmPbr_stanza_counter_status
{
   SSMPBR_COUNTER_UNAVAILABLE_E,
   SSMPBR_COUNTER_PARTIAL_E,
   SSMPBR_COUNTER_ACTIVE
};

struct ssmPbr_stanza_status_record
{
   enum ssmPbr_stanza_action action;
   enum ssmPbr_stanza_status status;
   enum ssmPbr_stanza_counter_status cnt_status;
   uint16_t seq_num;
   uint8_t ip_dscp;
   uint8_t ip_dscp_set:1;
   uint8_t is_null_if_cfg:1;
   uint16_t reserved:6;
   char ip_acl_name[SSMACL_NAME_SIZE];
   uint8_t num_next_hops_cfg; // Number of next hops valid
   int8_t active_next_hop_idx; //-1 means none are active
   uint16_t padding;
   char vrf_name[SSMPBR_MAX_NHOPS_PER_STANZA][SSMPBR_MAX_VRF_NAME_SIZE];
   uint32_t next_hops[SSMPBR_MAX_NHOPS_PER_STANZA];
   uint64_t frame_cnt;
   uint64_t byte_cnt;
};

struct ssmPbr_route_map_status_record
{
   char route_map[SSMPBR_ROUTE_MAP_NAME_SIZE];
   char if_name[SSMPBR_INTF_LONG_NAME];
   uint16_t num_stanzas;
   uint16_t reserved;
   struct ssmPbr_stanza_status_record stanza_rec[1];
};

struct ssmPbr_status_resp_data
{
   uint16_t num_records;
   struct ssmPbr_route_map_status_record rmap_rec[1];
};

struct ssmPbr_route_map_bind_entry
{
   char route_map[SSMPBR_ROUTE_MAP_NAME_SIZE];
   char if_name[SSMPBR_INTF_LONG_NAME];
};

struct ssmPbr_summary_status_resp_data
{
   uint16_t num_records;
   struct ssmPbr_route_map_bind_entry rmap_rec[1];
};

#endif //_SSMPBR_PUBLIC_H_

/* vim:set ts=3 expandtab tw=79: */
