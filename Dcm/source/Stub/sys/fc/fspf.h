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
 *    Copyright (c) 2011 Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Description:
 *      Chandrika - Added all the structures required for showlinkinfo.
 */

#ifndef __SYS_FC_FSPF_H__
#define __SYS_FC_FSPF_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned char u_char;
typedef unsigned short u_short;

/*
 * Link State info - Protocol Version 3.
 */
typedef struct ls_info_v3 {
	int linkId;     /* Link ID */
	int outPort;    /* Output port for this link */
	int remPort;    /* Remote port */
#if __BYTE_ORDER == __BIG_ENDIAN
	u_char  type;       /* Link type */
	u_char  costCnt;    /* Number of Class costs */
	u_short cost;       /* Cost of sending a frame over this ISL */
	u_char  dispType;   /* Port display type */
	u_char  blade;      /* Blade ID for physical/external port */
	u_short phyPort;    /* Physical/external port number */
	u_short reserved1;  /* Reserved for future use */
	u_short flags:10;   /* Reserved for later flags */
	u_short trunked:1;  /* Indicates if the local/src port is a T_Port */
        u_short qsfp_idx_valid:1 /* If the QSFP index is valid or not. */
        u_short qsfp_idx:4  /* The qsfp breakout index. */
#elif __BYTE_ORDER == __LITTLE_ENDIAN
	u_short cost;       /* Cost of sending a frame over this ISL */
	u_char  costCnt;    /* Number of Class costs */
	u_char  type;       /* Link type */
	u_short phyPort;    /* Physical/external port number */
	u_char  blade;      /* Blade ID for physical/external port */
	u_char  dispType;   /* Port display type */
        u_short qsfp_idx_valid:1; /* If the QSFP index is valid or not. */
        u_short qsfp_idx:4;  /* The qsfp breakout index. */
	u_short trunked:1;  /* Indicates if the local/src port is a T_Port */
	u_short flags:10;   /* Reserved for later flags */
	u_short reserved1;  /* Reserved for future use */
#else
#error "Endianness undefined!"
#endif
	int reserved2;      /* Reserved for future growth */
	int reserved3;      /* Reserved for future growth */
} lsi_v3_t;

/* 
 * Generic Link State Info
 *  This should always be pointing to the
 *  version used for the local switch.
 */
typedef lsi_v3_t lsi_t;

/*
 * LinkInfo Structure
 */

typedef struct links_info {
	int domainid;
	int domainReachable;
	int version;
	char domain_wwn[24];
	int numberOfLinks;
	lsi_t *links;
} links_info_t;

#ifdef __cplusplus
}
#endif

#endif /* __SYS_FC_FSPF_H__ */
