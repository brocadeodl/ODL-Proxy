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
 *   Copyright (c) 1996-2011 by Brocade Communications Systems, Inc.
 *   All rights reserved.
 *
 *   Utility routines for ftp to remote hosts
 *
 */

#ifndef SUPPORT_H__
#define SUPPORT_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum    {
    SS_STATUS_UNKNOWN   = 0,
    SS_STATUS_COMPLETED,
    SS_STATUS_INPROGRESS,
    SS_STATUS_ERROR
} ss_status_e;

typedef struct ss_progress_s {
        int     percentage;
        int     modules_uploaded;
        int     modules_left;
        ss_status_e     status;
        char    msg[256];
} ss_progress_t;

inline int ss_get_progress(ss_progress_t *ss_status)
{
    return 0;
}

inline void ss_get_status(int sid, ss_progress_t *p_ss_status)
{
    if (!p_ss_status)
        return;

    bzero(p_ss_status, sizeof (ss_progress_t));
    strncpy(p_ss_status->msg, "The Support Save status is Unknown....",
            sizeof (p_ss_status->msg));

    return;
}

inline int set_ss_module_tout()
{
	return 1;
}

inline int get_ss_total_tout()
{
	return 1;
}

#ifdef __cplusplus
}
#endif

#endif // SUPPORT_H__
