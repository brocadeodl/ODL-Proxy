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

#ifndef __NSMVRFINTF_H
#define __NSMVRFINTF_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"

#define NSM_INTF_NAME_SIZE 63
#define NSM_DCM_CLIENT_NAME "nsm"
#define RTM_DCM_CLIENT_NAME "rtm"
#define RTM_RD_MAX_LEN  64
#define DEFAULT_VRF_NAME "default-vrf"

typedef enum {
    /* VRF flow configure */
    /* Need to fix this enum values. Currently it is scattered */
    NSM_MSG_VRF = 50,
    NSM_MSG_NO_VRF,
   	NSM_MSG_VRF_BIND_INTF,
   	NSM_MSG_VRF_UNBIND_INTF,
    NSM_MSG_VRF_CMD_CODE
} nsm_vrf_cmd_code;


typedef struct vrf_global_config_msg_ {
	char vrf_name[NSM_INTF_NAME_SIZE];
	char if_name[NSM_INTF_NAME_SIZE];
    char rd[RTM_RD_MAX_LEN]; /*Actually this is RTM related related
                               symatics dectate it to be in 
                               VRF message*/
    uint32_t opcode;
    uint32_t if_type;
} nsm_vrf_global_config_msg_t;


#endif
