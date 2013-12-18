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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : James Chen                                    *
 ***************************************************************************/

#ifndef _RASCLIENT_H_
#define _RASCLIENT_H_

#include <stdio.h>
#include <unistd.h>
#include "raslog/raslogm.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define	uint32_t	int

typedef struct vcslog_t {
    rls_msg_t vcsmsg;
} vcslog_t;

#define LCCLI_INPUT__MAX 128
typedef struct lccli_in_msg {
    uint32_t    cli_code;
    char        argv[LCCLI_INPUT__MAX+1];
} lccli_in_msg_t;

#define LCCLI_OUTPUT__MAX 2048

typedef struct lccli_out_msg {
    uint32_t    out_buf_len;
	//  char        out_buf[LCCLI_OUTPUT__MAX+1];
} lccli_out_msg_t;


/* Client initialization related API */
int setRASMsgHandlerForClient(void);
int send_vcslog (vcslog_t *p_vcslog);
void setManagementInterfaceReceiverPort (int serverPort);

#ifdef __cplusplus
}
#endif

#endif
