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
 *   Description:                                                          *    
 *       This header file contains RAS-LOG related stub routines           *
 *       and structures                                                    *    
 ***************************************************************************/

#ifndef __RASLOGT_H__
#define __RASLOGT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define raslog_msg_id_t int

#if 0
typedef struct rlmd_msgdef {
    raslog_msg_id_t msg_id; /* message id */
    int     attributes; /* attributes of the message */
    int     msg_class; /* attributes of the message */
    int severity;   /* severity level */
    char    msg_value[MAX_MESSAGE_VALUE_LEN+1]; /* msg string */
    char    message[MAX_MESSAGE_LEN + 1];   /* msg format string */
    int arg_type;   /* argument type */
    int arg_num;    /* number of argument */
    char    cat_name[RAS_CAT_NAME_LEN];
} rlmd_msgdef_t;
#endif
/* message id struct with CAT_NAME */
typedef struct raslog_cat_msg_id {
    raslog_msg_id_t  msg_id;
    char cat_name[RAS_CAT_NAME_LEN];
} raslog_cat_msg_id_t;

static inline void rltl_msgcat_init(void)
{
	return;
}

static inline raslog_msg_id_t rltl_find_cat_msg_id(char *msg_value, char *cat_name)
{
	return (0);
}

static inline rlmd_msgdef_t * rltl_get_msg_def(raslog_cat_msg_id_t catmsg)
{
    return (0);
}

#ifdef __cplusplus
}
#endif

#endif
