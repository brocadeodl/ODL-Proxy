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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef __WWN_H__
#define __WWN_H__

#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct wwn_ieee48 {
    u_char      ww_naaNnib;
    u_char      ww_ext;
    u_char      ww_ula_hi[2];   /* top 16 bits - vendor */
    u_char      ww_ula_lo[4];   /* bottom 32 bits - vendor assign */
} wwn_ieee48_t;

typedef struct wwn_local {
    u_char      ww_naaNnib;
    u_char      ww_rest[7];
} wwn_local_t;

typedef struct wwn_ip {
    u_char      ww_naaNnib;
    u_char      ww_resvbytes[3];
    u_char      ww_ip32[4];
} wwn_ip_t;

typedef union wwn {
    char            chars[8];
    char            wwn[8];
    u_int           words[2];
    wwn_ieee48_t    ieee;
    wwn_local_t     local;
    wwn_ip_t        ip;
} wwn_t;

static inline char *
wwnfmt_r(wwn_t *w, char *buffer, size_t buflen)
{
    wwn_t swn;
    u_char *p;

    *(u_int *)&swn = htonl(*(u_int *)w);
    *((u_int *)&swn + 1) = htonl(*((u_int *)w + 1));

    p = (u_char *)swn.chars;

    sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
        p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);

    return (buffer);
}

static inline char *
wwnfmt(wwn_t *w)
{
    static char buf[24];

    return (wwnfmt_r(w, buf, 24));
}

static inline int
wwnscanf(char *s, wwn_t *w)
{
	int  i, n;
	uint p[8];

	if(sscanf(s, "%x:%x:%x:%x:%x:%x:%x:%x%n",
	          p, p+1, p+2, p+3, p+4, p+5, p+6, p+7, &n) != 8)
		return (-1);

	for (i = 0; i < 8; i++) {
		if (p[i] < 0 || p[i] > 0xff)
			return (-1);
		if (w)
			w->chars[i] = p[i];
	}

	return (n);
}

#ifdef __cplusplus
}
#endif

#endif // __WWN_H__
