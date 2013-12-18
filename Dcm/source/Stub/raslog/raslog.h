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
 *   Copyright (C) 2010-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : James Chen                                                   *
 *   Description:                                                          *    
 *       This header file contains RAS-LOG related stub routines           *
 *       and structures                                                    *    
 ***************************************************************************/

#ifndef __RASLOG_H__
#define __RASLOG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define CHASSIS_SVC_NAME	"chassis"
#define SWITCH_SVC_NAME		"fcsw"
#define ESWITCH_SVC_NAME	"ethsw"
#define CHASSIS_SVC_ID		0
#define SWITCH_SVC_ID		1
#define ESWITCH_SVC_ID		2

#define RASLOG_OID_INVALID   	0
#define RASLOG_NOFLAGS  	0
#define RASLOG_FRCLOG		0 
#define RASLOG_NO_SEEPROM	0
#define RASLOG_FORCE_SEEPROM	0
#define RASLOG_NOTRAP		0
#define RASLOG_NODIAG		0
#define RASLOG_NOBURNIN		0  

#define RASLOG_CRITICAL		1
#define RASLOG_ERROR		2
#define RASLOG_WARNING		3
#define RASLOG_INFO		4
#define RASLOG_SEVERITY_MAX     4

#define RASLOG_OID_INVALID   	0

// #include <switch/switch.h>	/* for OK/ERROR return codes */
#define RASLOG_OK		0

#define raslog_oid_t   int
#define raslog_msg_id_t int
static inline int raslog_init(char *svc, int inst, char *comp)
{
	return (0);
}

static inline void raslog_exit(void)
{
	return;
}

static inline int raslog_ext(__const char *file, __const char *function,
	   unsigned int line,
      raslog_oid_t objid, int flags, raslog_msg_id_t msg_id, ...)
{
	printf ("raslog_ext, file=%s func=%s ln=%d messageID=0x%x\n",
		file, function, line, (int)msg_id);
	return (0);
}

static inline int auditlog_ext(__const char *file, __const char *function,
           unsigned int line,
          char *user, char *role, char *ip, int interface, char *application,
      raslog_oid_t objid, int flags, raslog_msg_id_t msg_id, ...)
{
	return (0);
}

static inline int get_swbd(void )
{
	return (0);
}

#ifdef __cplusplus
}
#endif

#endif
