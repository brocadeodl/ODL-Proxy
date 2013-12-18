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

#ifndef __TRILLOAM_PLUGIN_TYPES_H__
#define __TRILLOAM_PLUGIN_TYPES_H__

#include "ClientInterface/DceWaveClientMessage.h"

#define TRILLOAM_DCM_MAX_STR		64
#define TRILLOAM_DCM_IF_NAME_LEN	20

#define TRILLOAM_MAX_NAME_STR 64
#define TRILLOAM_MAX_APP_TYPE 20
#define TRILLOAM_MAX_MAC_STR  18
#define TRILLOAM_MAX_PROTO_STR  8
#define TRILLOAM_MAX_IP_STR  16

#define TRILLOAM_INVALID_RBRIDGE 17
#define TRILLOAM_SMAC_NOT_LEARNT 18
#define TRILLOAM_DMAC_NOT_LEARNT 19

typedef enum {
	TRILLOAM_MSG_PING,
	TRILLOAM_MSG_TRACEROUTE
} trilloam_cmd_code;


typedef struct trilloam_msg_ping_ {
	unsigned int    cmd;
	char src_mac[TRILLOAM_MAX_MAC_STR+1];
	char dst_mac[TRILLOAM_MAX_MAC_STR+1];
	unsigned int	vlanid;
} trilloam_msg_ping;


/* TRILLOAM instance configure */
typedef struct trilloam_msg_traceroute_ {
	unsigned int    cmd;
	char src_mac[TRILLOAM_MAX_MAC_STR];
	char dst_mac[TRILLOAM_MAX_MAC_STR];
	unsigned int	vlanid;
	char proto_type[TRILLOAM_MAX_PROTO_STR];
	char proto[TRILLOAM_MAX_PROTO_STR];
	char src_ip[TRILLOAM_MAX_IP_STR];
	char dst_ip[TRILLOAM_MAX_IP_STR];
	unsigned int	src_port;
	unsigned int	dst_port;
    unsigned int   sessionid;
	unsigned int   begin;
	unsigned int   last;
} trilloam_msg_traceroute;

typedef struct trilloam_msg_traceroute_resp_row_ {
    unsigned short  egid;           /* Egress logical port */
    unsigned short  trill_da;       /* Trill nick name  */
    unsigned char   ingress[64];    /* ingress interface */
    unsigned char   egress[64];     /* egress interface */
    unsigned int    rttime;         /* round trip time */
} trilloam_msg_traceroute_resp_row;

typedef struct trilloam_msg_traceroute_resp_ {
	unsigned int   cmd;
	unsigned int   size;
    unsigned int   sessionid;
	unsigned int   begin;
	unsigned int   last;
    unsigned int   row_count;
    trilloam_msg_traceroute_resp_row rows[1];
} trilloam_msg_traceroute_resp;

#endif	/* __TRILLOAM_MESSAGE_DEF_H__ */
