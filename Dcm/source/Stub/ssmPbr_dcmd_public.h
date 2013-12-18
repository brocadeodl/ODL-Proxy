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
#ifndef _SSMPBR_DCMD_PUBLIC_H_
#define _SSMPBR_DCMD_PUBLIC_H_

#include "ssmPbr_public.h"

typedef enum ssmPbr_dcmd_opcode
{
   SSMPBR_DCM_INVALID_OP_E = 0,

   SSMPBR_DCM_SHOW_BIND_SUMMARY_E,
   SSMPBR_DCM_SHOW_STATUS_BY_ROUTE_MAP_E,
   SSMPBR_DCM_SHOW_STATUS_BY_INTERFACE_E,

   SSMPBR_DCM_MAX_OP_E
} ssmPbr_dcmd_opcode_t;


typedef struct ssmPbr_dcmd_show_request
{
   ssmPbr_dcmd_opcode_t opcode;
   char rmap_name[SSMPBR_ROUTE_MAP_NAME_SIZE];
   char if_name[SSMPBR_INTF_LONG_NAME];
   uint16_t last_stanza_num; /**< For Get request it will be 0. For Get Next
         requests it will be filled from the last response */
} ssmPbr_dcmd_show_request_t;

typedef struct ssmPbr_dcmd_show_response
{
   ssmPbr_dcmd_opcode_t opcode;
   uint8_t more_elements; /**< Will be set if a subsequent request is needed */
   uint8_t reserved[3];
   struct ssmPbr_status_resp_data resp_data;
} ssmPbr_dcmd_show_response_t;

typedef struct ssmPbr_dcmd_show_summary_response
{
   ssmPbr_dcmd_opcode_t opcode;
   struct ssmPbr_summary_status_resp_data resp_data;
} ssmPbr_dcmd_show_summary_response_t;


#endif //_SSMPBR_DCMD_PUBLIC_H_

/* vim:set ts=3 expandtab tw=79: */
