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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Navin
 **************************************************************************/

#ifndef DCE_DBG_MSG_DEF_H
#define DCE_DBG_MSG_DEF_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define DCE_DBG_ERR_BUF_LEN_MAX			-2048
#define DCE_DBG_ERR_BUF_REALLOC_FAIL	-2047

#define COOKIE_REQ_TAG  2
#define COOKIE_RESP_TAG 3

typedef struct cookie_t_{
    unsigned long size; // size of data that is sent
    void *data; //Ptr to cookie structure
} cookie_t;

typedef struct dce_dbg_in_msg {
    uint32_t    dbg_cli_code;
    char        dbg_argv[200];
    uint8_t     cookie_flag;
    cookie_t    cookie_data;
}dce_dbg_in_msg_t;

typedef struct dce_dbg_out_msg {
    uint32_t	out_buf_len;
    uint8_t     cookie_flag; // Flag value 1 means cookie_data.data is not null
    cookie_t    cookie_data; 
    char 		out_buf[1];
}dce_dbg_out_msg_t;

#endif /* DCE_DBG_MSG_DEF_H */

