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

#ifndef _LCCLICLIENT_H_
#define _LCCLICLIENT_H_

#include <stdio.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#include <utils/mqueue.h>
#endif

#define LCCLI_INPUT_LEN 128
#define LCCLI_CLIENT_NAME "Lcclid"

#define MSG_LCCLI_CB_HANDLER    0xa0

typedef struct lccli_input_msg {
    uint32_t    cli_code;
    char        argv[LCCLI_INPUT_LEN + 1];
} lccli_input_msg_t;

#define LCCLI_OUTPUT_LEN 2048

typedef struct lccli_output_msg {
    uint32_t    out_buf_len;
} lccli_output_msg_t;


/* Client initialization related API */
int setLccliMsgHandlerForClient(void);
int lccli_cb_on_main_q(msg_t *msg);
void setLccliManagementInterfaceReceiverPort (int serverPort);


#ifdef __cplusplus
}
#endif

#endif
