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

#ifndef __SYSCTRL_H__
#define __SYSCTRL_H__

#include "Stub/fabos/fabioctl.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* module handle data structure */
typedef struct sysModHandle {
    int     fd;     /* system module device file descriptor */
    int     wwn_fd; /* system module device file descriptor */
} sysModHandle_t;

static sysModHandle_t sysModHandle;

int sysModInit(sysModHandle_t **hdl);
int sysModClose(sysModHandle_t **hdl);
int sysCtrlGetSystemParameters(sysModHandle_t *pHandle, void *pSystemParam);

int sysModInit(sysModHandle_t **hdl)
{
    *hdl = &sysModHandle;
    return (0);
}

int sysModClose(sysModHandle_t **hdl)
{
    return (0);
}

int sysCtrlGetSystemParameters(sysModHandle_t *pHandle, void *pSystemParam)
{
    fabSysParam_t *fabSys = (fabSysParam_t *)pSystemParam;
    fabSys->mac_pool.MACaddressMS = 0x11223344;
    fabSys->mac_pool.MACaddressLS = 0x5566;
    fabSys->max_ge_ports_per_system = 64;

    return (0);
}

#ifdef __cplusplus
}
#endif

#endif // __SYSCTRL_H__
