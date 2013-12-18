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

#include "IgmpsTime.h"
#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include <time.h>
#include <sys/time.h>
#include <asm/param.h>
#include "memory.h"


#define IGMPS_WRAPAROUND_VALUE (0xffffffffUL / HZ + 1)

int
system_uptime (struct pal_timeval *tv, struct pal_tzval *tz)
{
#if 0		// disable due to 4.3.74 toolchain not allow including <linux/kernel.h> in user space
  struct sysinfo info;
  static unsigned long prev = 0;
  static unsigned long wraparound_count = 0;
  unsigned long uptime;
  static long base = 0;
  static long offset = 0;
  long leap;
  long diff;
  
  /* Get sysinfo.  */
  if (sysinfo (&info) < 0)
    return -1;

  /* Check for wraparound. */
  if (prev > (unsigned long)info.uptime)
    wraparound_count++;
  
  /* System uptime.  */
  uptime = wraparound_count * IGMPS_WRAPAROUND_VALUE + info.uptime;
  prev = info.uptime;      
  
  /* Get tv_sec and tv_usec.  */
  gettimeofday (tv, (struct timezone*)tz);

  /* Deffernce between gettimeofday sec and uptime.  */
  leap = tv->tv_sec - uptime;

  /* Remember base diff for adjustment.  */
  if (! base)
    base = leap;

  /* Basically we use gettimeofday's return value because it is the
     only way to get required granularity.  But when diff is very
     different we adjust the value using base value.  */
  diff = (leap - base) + offset;

  /* When system time go forward than 2 sec.  */
  if (diff > 2 || diff < -2)
    offset -= diff;

  /* Adjust second.  */
  tv->tv_sec += offset;
#endif
  return 0;
}

time_t
pal_time_current (time_t *tp)
{
  struct pal_timeval tv;
  int ret;

  /* Get current time i.e. time since reboot. */
  ret = system_uptime (&tv, NULL);
  if (ret != 0)
    return -1;

  /* When argument is specified copy value.  */
  if (tp)
    *tp = (time_t) tv.tv_sec;

  return tv.tv_sec;
}

/*!
** Take a local time and convert it to GMT (UTC), in expanded form.
**
** replaces gmtime()
**
** Parameters
**   IN  time_t *tp         : A pointer to the time to convert
**   OUT struct pal_tm *gmt     : A pointer to where to put the expanded GMT
**
** Results
**   RESULT_OK for success, else the error which occurred
*/
int
pal_time_gmt (time_t * tp, struct pal_tm *gmt)
{
  struct pal_tm *tmp;

  tmp = (struct pal_tm*)gmtime (tp);

  if (tmp)
	  memcpy (gmt, tmp, sizeof (struct pal_tm));

  return (tmp ? 0: EAGAIN);
}

char *
timeutil_uptime (char *timebuf,
                 u_int32_t bufsiz,
                 time_t uptime)
{
  struct pal_tm tm;

  /* Get current time. */
  pal_time_gmt (&uptime, &tm);

  /* Making formatted timer string. */
  if (uptime < ONE_DAY_SECOND)
    snprintf (timebuf, bufsiz, "%02d:%02d:%02d",
                  tm.tm_hour, tm.tm_min, tm.tm_sec);
  else if (uptime < ONE_WEEK_SECOND)
    snprintf (timebuf, bufsiz, "%02dd%02dh%02dm",
                  tm.tm_yday, tm.tm_hour, tm.tm_min);
  else
    snprintf (timebuf, bufsiz, "%02dw%02dd%02dh",
                  tm.tm_yday/7, tm.tm_yday - ((tm.tm_yday/7) * 7), tm.tm_hour);

  return timebuf;
}

char *
timeval_uptime (char *timebuf,
                u_int32_t bufsiz,
                struct pal_timeval uptime)
{
  /* Making formatted timer string. */
  if (uptime.tv_sec < ONE_DAY_SECOND)
    snprintf (timebuf, bufsiz, "%02ld:%02ld:%02ld",
                  uptime.tv_sec / ONE_HOUR_SECOND,
                  (uptime.tv_sec % ONE_HOUR_SECOND) / ONE_MIN_SECOND,
                  uptime.tv_sec % ONE_MIN_SECOND);
  else if (uptime.tv_sec < ONE_WEEK_SECOND)
    snprintf (timebuf, bufsiz, "%02ldd%02ldh%02ldm",
                  uptime.tv_sec / ONE_DAY_SECOND,
                  (uptime.tv_sec % ONE_DAY_SECOND) / ONE_HOUR_SECOND,
                  (uptime.tv_sec % ONE_HOUR_SECOND) / ONE_MIN_SECOND);
  else
    snprintf (timebuf, bufsiz, "%02ldw%02ldd%02ldh",
                  uptime.tv_sec / ONE_WEEK_SECOND,
                  (uptime.tv_sec % ONE_WEEK_SECOND) / ONE_DAY_SECOND,
                  (uptime.tv_sec % ONE_DAY_SECOND) / ONE_HOUR_SECOND);

  return timebuf;
}

time_t
igmp_get_current_monotime_for_cli(void)
{
    struct timespec abs_time;

    clock_gettime(CLOCK_MONOTONIC, &abs_time);

    return abs_time.tv_sec;
}
