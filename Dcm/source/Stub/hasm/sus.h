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

#ifndef __HASM_SUS_H__
#define __HASM_SUS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <fabos/version.h>

typedef struct  {
    version_array_t sv_pvers;   /* primary partition versions */
    version_array_t sv_svers;   /* secondary partition versions */
} sus_vers_t;

typedef struct  {
    int sn_hamst;       /* node ham state */
    int sn_runst;       /* node run state */
    int sn_envok;       /* node env ok for fwdl? */
    int sn_pad[1];      /* pad bytes */
} sus_node_t;

/* Structure returned to the applications */
typedef struct	{
	int ss_node;	/* node id */
	int ss_state;	/* node ham state, not used */
	int ss_scode;	/* sus status code in sus.h */
	int ss_fcode;	/* fwdl status code in fwdl.h */
	int ss_count;	/* number of status items */
	time_t  ss_time;	/* timestamp */
	int ss_pad[2];	/* pad bytes */
} sus_status_t;

static inline int sus_version_get(int nid, sus_vers_t *ver)
{
    memset(ver, 0, sizeof (sus_vers_t));
    version_get(NULL, (char *) "base", &ver->sv_pvers[0]);
    version_get(NULL, (char *) "base", &ver->sv_svers[0]);

    return(0);
}

static inline int sus_node_get(int nid, sus_node_t *nin)
{
    memset(nin, 0, sizeof (sus_node_t));
    return (0);
}

static inline int sus_fwdl_state(void)
{
	return (0);
}

static inline int sus_status_get(int seq, sus_status_t *status)
{
	memset(status, 0, sizeof (sus_status_t));
	return (0);
}

static inline int sus_status_map(int scode, int fcode, char *msg)
{
	memset(msg, 0, sizeof(msg));
	return (0);
}

static inline int sus_sanity_status_get(char *buffer, int size)
{
	memset(buffer, 0, sizeof (buffer));
	return (0);
}

static inline int sus_sanity_activate(void)
{
	return (0);
}

static inline int sus_progress_get(void)
{
	return (0);
}

static inline int sus_state_get(int)
{
	return (0);
}

#define SUS_VERS_PRI(vs) (&((vs)->sv_pvers))
#define SUS_VERS_SEC(vs) (&((vs)->sv_svers))

#ifdef  __cplusplus
}
#endif

#endif
