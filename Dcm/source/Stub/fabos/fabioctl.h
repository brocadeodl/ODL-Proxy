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


/***************************************************************************
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef __FABIOCTL_H__
#define __FABIOCTL_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Max # slots in chassis */
#define DCE_MAX_SLOTS               12

typedef struct _mac_addr
{
    unsigned char addr[6];
} mac_addr_t;

typedef struct _mac_pool {
    unsigned int size;
    unsigned int MACaddressMS;
    unsigned int MACaddressLS;
} mac_pool_t;

typedef struct _mac_pool_list  {
    unsigned int  count;
    //mac_pool_t entries[0];
    mac_pool_t *entries;
} mac_pool_list_t;

typedef struct fabSysParam {
    int max_chips_per_system;
    int max_ge_ports_per_anvil;
    int max_lags_per_system;
    int max_ge_ports_per_system;
    unsigned char slot_chip_info[DCE_MAX_SLOTS + 1];
    mac_pool_t mac_pool;
} fabSysParam_t;

#ifdef __cplusplus
}
#endif

#endif // __FABIOCTL_H__
