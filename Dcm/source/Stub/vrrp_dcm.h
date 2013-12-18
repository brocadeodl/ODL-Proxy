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
 *  Copyright (c) 2007-2010 by Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  File name: vrrp_dcm.h
 *
 *  Description:
 *  This file contains declarations and definitions required for
 *  integrating VRRP module with DCMD.
 *              
 */

#ifndef _VRRP_DCMD_H_
#define _VRRP_DCMD_H_

#include "ClientInterface/VRRP/VRRPMessageDef.h"

#define         MAX_VRIDS                       255
#define         VRRP_PROTOCOL_IP                1
#define         MAX_VRRPE_VIRTUAL_IPS            1
#define         MAX_TRACKPORT_INTERFACES        16 

#define         VRRP_ADMIN_STATE_UP             1
#define         VRRP_ADMIN_STATE_DOWN           2

#define         VRRP_SET                        0
#define         VRRP_UNSET                      -1

#define 		MAX_RECORDS_PER_PAGE			10

/* VRRP State. */
typedef enum vrrp_state {
  VRRP_STATE_INIT=1,
  VRRP_STATE_BACKUP,
  VRRP_STATE_MASTER
} vrrp_state_t;

#define VRRP_STATE_STR(_state)  ((_state==VRRP_STATE_INIT)?"Init":(_state==VRRP_STATE_MASTER?"Master":"Backup"))

int vrrp_dcm_return (int ret);

#endif /* _VRRP_DCMD_H_ */

