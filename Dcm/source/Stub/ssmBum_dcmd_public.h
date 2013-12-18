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
 * @file ssmAcl.h
 * @brief defintions used in the exchange of info from frontend CLI module
 * @author Alan Bolton
 * @date 11/22/2011
 *
 */
#ifndef _SSMBUM_DCMD_PUBLIC_H_
#define _SSMBUM_DCMD_PUBLIC_H_
/*
 * Included Files
 */

#include "ssmBum_public.h"

/*
 * Typedef, enumeration, structure definitions
 */

/**
 * Enumeration of opcodes used by front-end cli to backend module
 */
typedef enum
{
   SSMBUM_DCMD_ENABLE = 0,
   SSMBUM_DCMD_DISABLE,
   SSMBUM_DCMD_SHOW_SYSTEM_IF,
   SSMBUM_DCMD_SHOW_SYSTEM_BY_PROTOCOL,
   SSMBUM_DCMD_SHOW_IF,
   SSMBUM_DCMD_SHOW_IF_BY_PROTOCOL,
   SSMBUM_DCMD_CLEAR_SYSTEM,
   SSMBUM_DCMD_CLEAR_SYSTEM_BY_PROTOCOL,
   SSMBUM_DCMD_CLEAR_IF,
   SSMBUM_DCMD_CLEAR_IF_BY_PROTOCOL
} ssmBum_dcmd_opcode_t;

/**
 * Data Type configuration message for enabling/disabling BUM storm control
 */
typedef struct ssmBum_dcmd_config
{
   ssmBum_dcmd_opcode_t opcode;
   char if_name[64]; //FIXME what #define do we use here?
   enum ssmBum_protocol protocol;
   enum ssmBum_action action;
   enum ssmBum_rate_format rate_format;
   unsigned long long rate;
} ssmBum_dcmd_config_t;

/**
 * Data Type for requesting showing or clearing BUM storm control status
 */
typedef struct ssmBum_dcmd_show_clear_
{
   ssmBum_dcmd_opcode_t opcode;
   char if_name[64]; //FIXME what #define do we use here?
   enum ssmBum_protocol protocol;
} ssmBum_dcmd_show_clear_t;

/**
 * Data Type used by backend to report status info to the frontend CLI
 */
struct ssmBum_dcmd_status_response
{
   ssmBum_dcmd_opcode_t opcode; //!< Opcode is used to interpret resp buffer
   unsigned char more_elements;
   struct ssmBum_status_resp_data data;
};

/*
 * Exported Functions
 */

#endif /*_SSMBUM_DCMD_PUBLIC_H_*/
