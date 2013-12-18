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
 * Copyright (c) 1996-2011 by Brocade Communications Systems, Inc.
 * All rights reserved.
 *
 * Description:
 *   Module for defining Dcm IPC messages.
 *
 */

#ifndef _DCM_IPC_H
#define _DCM_IPC_H

#ifdef  __cplusplus
extern  "C" {
#endif

#include <ipc/ipc.h>

/*
 * blade config data
 */
typedef struct  {
    int         bc_id;      /* blade type id */
    int         bc_mode;    /* blade mode */
    int         bc_profile; /* blade profile id */
    int         bc_pad0;    /* for alignment */
    uint64_t    bc_brkout;  /* break out bitmap, port 0-63 */
    int         bc_pad1[6]; /* space for more config data */
} blcd_t;

/*
 * for dcm interaction with ISC_TYPE_QUERY_SLOT
 */
typedef struct {
        int     sq_slotno;      /* logical slot# */
        int     sq_hwtype;      /* slot type */
        blcd_t  sq_cdata;       /* blade config data */
} sqry_t;

typedef struct {
    u_int slot_number;
    u_int hw_type;
    blcd_t  bl_cdata;
} slot_type_data_t;

typedef struct {
    int type;
    int sub_type;
    IPC_ADDR dest;
    MSG_TYPE msg_type;
    union {
        slot_type_data_t slot_type_data;
    } ipc_data_t;
} dcm_ipc_t;

/* Dcm IPC messages */
#define MSG_DCM_IPC_DEST_NAME(data)     ((data)->dest.name)
#define MSG_DCM_IPC_DEST_INSTANCE(data) ((data)->dest.instance)
#define MSG_DCM_IPC_DEST_NODE(data)     ((data)->dest.node)
#define MSG_DCM_IPC_MSG_TYPE(data)      ((data)->msg_type)
#define MSG_DCM_IPC_SLOT_NUMBER(data)   ((data)->ipc_data_t.slot_type_data.slot_number)
#define MSG_DCM_IPC_HW_TYPE(data)       ((data)->ipc_data_t.slot_type_data.hw_type)

/* IPC message types */
typedef enum {
    IPC_QUERY_SLOT      /* slot type query message */
} dcm_ipc_msg_type_e;

/* DCMD IPC constants */
#define DCMD_IPC_DEST_NAME "DCMD_VCS_LOCAL"
#define DCMD_IPC_DEST_INSTANCE 0
#define CCMD_IPC_DEST_NAME "CCMD_VCS_LOCAL"
#define CCMD_IPC_DEST_INSTANCE 0
#define NSP_IPC_DEST_NAME "NSP_VCS_LOCAL"
#define NSP_IPC_DEST_INSTANCE 0
#define MYNODE 0

#ifdef  __cplusplus
}
#endif

#endif  /* _DCM_IPC_H_ */

