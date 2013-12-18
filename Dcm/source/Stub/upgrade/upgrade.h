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

#ifndef __UPGRADE_H_
#define __UPGRADE_H_

#include <stdio.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

static inline int soft_upgrade_state(void)
{
    return (1);
}

static inline int soft_upgrade_sanity(int fwdl_type, char *protocol, char *host,
                        char *file, char *user, char *passwd, long swbd,
                        char *app_name, int *app_swbd, int bitmap)
{
    return (0);
}

static inline int fup_sanity_status_get(char *buffer, int maxSize)
{
    strcpy(buffer, "Error during preinstall");
    return (0);
}

static inline int fup_status_count(int *count)
{
    *count = 10;
    return (0);
}

static inline int fup_status_get(int seq, int *slot, char *bld_name, int *code, time_t *t)
{
    *slot = 1;
    strcpy(bld_name, "mybuild");
    *code = seq;
    *t = time(0);
    return (0);
}

static inline int fup_status_msg(int code, char *msg)
{
    sprintf(msg, "message %d", code);
    return (0);
}

#ifdef __cplusplus
}
#endif

#endif
