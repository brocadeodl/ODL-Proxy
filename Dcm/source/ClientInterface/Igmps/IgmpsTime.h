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

#ifndef _IGMP_TIME_H
#define _IGMP_TIME_H

#include "time.h"
#if defined(__KERNEL__)
#include "sys/sysinfo.h"
#endif

#define ONE_WEEK_SECOND     (60*60*24*7)
#define ONE_DAY_SECOND      (60*60*24)
#define ONE_HOUR_SECOND     (60*60)
#define ONE_MIN_SECOND      (60)
#define ONE_SEC_DECISECOND  (10)
#define ONE_SEC_CENTISECOND (100)
#define ONE_SEC_MILLISECOND (1000)
#define ONE_SEC_MICROSECOND (1000000)
#define MILLISEC_TO_DECISEC (100)
#define DECISEC_TO_MILLISEC (100)
#define HALFSEC_MILLISEC    (500)

/*
** A time in seconds and microseconds
*/
#define pal_timeval timeval

/*
 ** A structure with the elements of time disassembled
 */
#define pal_tm tm


time_t
pal_time_current (time_t *tp);

int
system_uptime (struct pal_timeval *tv, struct pal_tzval *tz);

int
pal_time_gmt (time_t * tp, struct pal_tm *gmt);

char *
timeutil_uptime (char *timebuf,
                 unsigned int bufsiz,
                 time_t uptime);

char *
timeval_uptime (char *timebuf,
                unsigned int bufsiz,
                struct pal_timeval uptime);

time_t
igmp_get_current_monotime_for_cli(void);

#endif
