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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef __FABSYS_H__
#define __FABSYS_H__

//#include "Stub/sysmod/sysctrl.h"
#include "Stub/sysmod/syscomm.h"
#include "Stub/sys/fabos/fabctrl.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define FABSYS_POD_PORT_UNASSIGNED  0
#define FABSYS_POD_PORT_IN_BASE     0xB
#define FABSYS_POD_PORT_IN_SINGLE   1
#define FABSYS_POD_PORT_IN_DOUBLE   2

/* This value can be used to handle gaps in the port numbering of a product */
#define FABSYS_POD_PORT_DISCONNECTED 0xD
#define FABSYS_MAX_POD_PORT          160
#define FABSYS_POD_ARRAY_SIZE        (FABSYS_MAX_POD_PORT+1)

#define VCS_2_NODE_LICENSE           1
#define VCS_MULTI_NODE_LICENSE       1
#define SWBD97                       97
#define SWBD95                       95
#define SWBD131                      131
#define SWBD137                      137
#define swbd_model                   int

/* sysCtrlHwRecord_t field access */
#define fabSysStateHwType(sP)   (sP)->record.type
#define fabSysStateHwId(sP) (sP)->record.id
#define fabSysStateHwVer(sP)    (sP)->record.version

/* sysCtrlData_t field access */
#define fabSysStateObject(sP)   (sP)->object
#define fabSysStateUnit(sP) (sP)->unitNumber
#define fabSysStateRecord(sP)   (sP)->record
#define fabSysStateReason(sP)   (sP)->reason
#define fabSysStateStatus(sP)   (sP)->state

typedef struct podlicense {
    u_int   pod1_license;       /* a pod license */
    u_int   pod2_license;       /* a second equivalent pod license */
    u_int   port_10G_upgradelicense;    /* 10G port upgrade license install state */
    u_int   port_40G_upgradelicense;    /* 40G port upgrade license install state */
} podlicense_t;
 
typedef struct {
    unsigned char bytes[FABSYS_POD_ARRAY_SIZE];
    unsigned char protocol[FABSYS_POD_ARRAY_SIZE];
    uint32_t      port_id[FABSYS_POD_ARRAY_SIZE];
} pod_set_t;

typedef struct pod {
    u_short         is_pod_supported;       /* whether the platform supports POD */
    u_short         is_epod_supported;      /* whether the platform supports two POD pools */
    u_int           pod_total_count;        /* num of 10GE ports under POD management */
    u_int           pod_base_set_size;      /* num of 10GE ports in base POD set */
    u_int           pod_single_set_size;    /* num of 10GE ports in single POD set */
    u_int           pod_double_set_size;    /* num of 10GE ports in double POD set */
    pod_set_t       pod_assignment;         /* all port type assignments */
    u_int           pod_40G_total_count;    /* num of 40GE ports under POD management */
    u_int           pod_40G_base_set_size;  /* num of 40G ports in base POD set */
    u_int           pod_40G_single_set_size; /* num of 40G ports in single POD set */
    u_int           pod_40G_first_port_num; /* first 40G port number */
    u_int           pod_40G_last_port_num;  /* last 40G port number */
    podlicense_t    pod_license;            /* pod license information */
} pod_t;

static inline int  sysModGetSwModel(sysModHandle_t *handle, swbd_model *model)
{
    // assume x86 is Elara2F (SWBD95)
    *model = 95;
    return (0);
}

static inline int sysCtrlGetSwState(sysModHandle_t *handleP, sysCtrlState_t *stateP)
{
    return (1);
}


#ifdef __cplusplus
}
#endif

#endif // __FABSYS_H__
