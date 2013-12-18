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
 *	Copyright(C) 2010 Brocade Communications Systems, Inc.
 *	All rights reserved.
 *	Author : James Chen
 *	Description:
 *		This header file contains RAS-LOG related stub routines
 *		and structures
 ***************************************************************************/

#ifndef __RASLOGD_H__
#define __RASLOGD_H__

#include "Stub/sysmod/syscomm.h"

using namespace WaveNs;

namespace DcmNs
{

}

#ifdef __cplusplus
extern "C"
{
#endif

/* syslog defines and typedef */
#define	SYSLOG_MAX_IP_STRING	256		
#define RAS_MAX_SYSLOGD		6
#define SWNAME_SIZE		32

/* syslogdIpAdd error */
typedef enum ras_syslog_err {
    RAS_SYSLOG_SUCESS,
    RAS_SYSLOG_IP_DUPLICATE,
    RAS_SYSLOG_IP_MAX,
    RAS_SYSLOG_IP_NOT_VALID,
    RAS_SYSLOG_NOT_CONFIGURED,
    RAS_SYSLOG_NONAME_RESOLUTION
} ras_syslog_err_t;

typedef struct syslog_ipaddr {
	int count;
	char addr[RAS_MAX_SYSLOGD][SYSLOG_MAX_IP_STRING];
} syslog_ipaddr_t;

typedef char	swname_t[SWNAME_SIZE];

static inline int syslogdIpGetNew(syslog_ipaddr_t *addrList)
{
	return (0);
}	    
static  inline void syslogdIpClear(void)
{
}
static inline int syslogdIpAdd(char *ipAddr)
{
	return (0);
}
static inline int syslogdIpRemove(char *ipAddr)
{
        return (0);
}
static inline int getSwitchName(swname_t *switchName)
{
    strncpy ((char * const) switchName, "X86-SW0", SWNAME_SIZE);
	return (0);
}

#ifdef __cplusplus
}
#endif

#endif
