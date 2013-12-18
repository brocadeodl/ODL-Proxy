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

#ifndef __IGMP_MESSAGE_DEF_H__
#define __IGMP_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"

#define IS_CMDCODE_SET(cmdcode, flag) (cmdcode & flag)

#define IGMP_MAX_STR 31

/* IGMP Interface Commands */
#define IGMP_LMQI				1<<0
#define IGMP_QMRT				1<<1
#define IGMP_QUERY_INTERVAL	    1<<2
#define IGMP_IMMEDIATE_LEAVE    1<<3

#define IGMP_INTERFACE_NAMESIZE  32 

#define IGMP_DEFAULT_CMD_CODE 0

#define IGMP_MAX_STR 31

#define IGMP_TRUE 1
#define IGMP_FALSE 0
#define IGMP_PLUGIN_LOCAL_VERSION 0

typedef struct igmp_intf_cfg_msg_ {
	int	        lmqi;
	int	        qmrt;
	int	        immediate_leave;
	int	        query_interval;
	char        if_name[IGMP_MAX_STR + 1];
	unsigned int if_index;
	unsigned int if_type;
    int     opcode;
} igmp_intf_cfg_msg_t;

typedef struct igmp_sg_intf_cfg_msg_ {
    char            group[IGMP_MAX_STR + 1]; 
	unsigned int    if_type;
	char            if_name[IGMP_MAX_STR + 1];
	unsigned char   negation;
} igmp_sg_intf_cfg_msg_t;

typedef enum {
    INTERFACE_INVALID_IGMP = -1,
    INTERFACE_ONEGIG_IGMP,
    INTERFACE_TENGIG_IGMP,
    INTERFACE_FORTYGIG_IGMP,
    INTERFACE_PORT_CHANNEL_IGMP,
    INTERFACE_VLAN_IGMP
} igmp_intf_type;

#endif	/* __IGMP_MESSAGE_DEF_H__ */
