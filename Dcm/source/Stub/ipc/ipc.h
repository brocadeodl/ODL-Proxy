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

#ifndef IPC_IPC_H
#define IPC_IPC_H

#include <giot/ioss.h>      /* ioss_t */

#ifdef __cplusplus
extern "C" {
#endif

typedef ushort NODE_TYPE;
typedef ushort INST_TYPE;
typedef uint MSG_TYPE;

#define MAX_NAME_SIZE       20

/* *** types *** */
typedef struct _IPC_ADDR {
    NODE_TYPE node;
    INST_TYPE instance;
    char name[MAX_NAME_SIZE];
} IPC_ADDR;

typedef int (*CB_TYPE)(IPC_ADDR *replyDest, MSG_TYPE replyType,
        char *buf, size_t size, void *myStuff);

static inline int ipc_ioss_create(ioss_t **handle, const char *name, INST_TYPE inst,
        size_t maxMsgSize, ushort maxTypeValue)
{
    return (0);
}

static inline int ipc_ioss_free(ioss_t **handle)
{
    return (0);
}

/*
 * ipcRegisterCBlw(MSG_TYPE type, CB_TYPE cb, void *myStuff);
 *
 *    Description:
 *   This function registers a 'light-weight' callback for IPC. This is to
 *   be used when the callback function is GUARANTEED not to block and
 *   returns as quickly as possible (e.g to queue an event in the event
 *   queue and return).
 *
 *   The callback happens in the context of the receive thread as opposed to
 *   transfering control to an IPC worker thread. Note that the receive
 *   thread is still different from other threads that the application may
 *   have started.
 *
 * Input(s):
 *   type   : An integer the enum of IPC functions exported
 *   cb     : The corresponding callback function
 *   myStuff: Context for the corresponding callback function
 *        (See last argument in CB_TYPE above)
 *
 * Output(s):
 *   N/A
 *
 * Returns:
 *   0 if no error, non-zero otherwise.
 *
 */
static inline int ipcRegisterCBlw(MSG_TYPE type, CB_TYPE cb, void *myStuff)
{
    return (0);
}

static inline int ipcSend(const IPC_ADDR *dest, MSG_TYPE type,
        const char *buf, size_t size, struct timeval *timeout)
{
    return (0);
}

static inline int ipcExit(void)
{
    return (0);
}

#ifdef __cplusplus
}
#endif

#endif

