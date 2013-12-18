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

/************************************8***************************************
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.                *
 *   All rights reserved.                                                   *
 *   Author : Himanshu Varshney                                             *
 *   Description:                                                           *
 *                 This file define the common c structure used by          *
 *                 VcsFabric Service at Client to communicate with Dcmd     *   
 ***************************************************************************/

#ifndef VCS_FABRIC_C_STRUCT_H
#define VCS_FABRIC_C_STRUCT_H

#include <stdio.h>
#include <stdint.h>

typedef struct vcs_fabric_debug_client_struct_ 
{ 
    uint32_t *domains_in_cluster;
    uint32_t  number_of_domain_in_cluster;
} vcs_fabric_debug_client_struct_t;

#endif //VCS_FABRIC_C_STRUCT_H
