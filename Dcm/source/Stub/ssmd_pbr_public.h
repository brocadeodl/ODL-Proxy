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
 *  Copyright (c) 2008-2009 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Module name: ssmd_public.h
 *
 *  Description:
 *      DCE System Services Manager (SSM) IPC services definitions.
 *
 */

#ifndef _SSMD_PBR_PUBLIC_H_
#define _SSMD_PBR_PUBLIC_H_

typedef enum {
/* 0 - 100 Generic NSSM Errors */
    SSMD_PBR_PUBLIC_SUCCESS = 0,
    SSMD_PBR_PUBLIC_IPC_SEND_ERR,
    SSMD_PBR_PUBLIC_ERR_LAST = 0xFFFF
} ssmd_pbr_public_err_t;

typedef enum ssmd_pbr_public_action
{
    SSMD_PBR_PUBLIC_ACTION_INVALID = 0,
    SSMD_PBR_PUBLIC_ACTION_REGISTER,
    SSMD_PBR_PUBLIC_ACTION_DERGISTER,
    SSMD_PBR_PUBLIC_ACTION_BIND,
    SSMD_PBR_PUBLIC_ACTION_UNBIND,
    SSMD_PBR_PUBLIC_ACTION_MAX
}ssmd_pbr_public_action_t;

typedef enum ssmd_pbr_public_arp_notify_action
{
    SSMD_PBR_PUBLIC_ARP_NOTIFY_ACTION_INVALID = 0,
    SSMD_PBR_PUBLIC_ARP_NOTIFY_ACTION_REACHABLE,
    SSMD_PBR_PUBLIC_ARP_NOTIFY_ACTION_UNREACHABLE

}ssmd_pbr_public_arp_notify_action_t;

/*
 * This function sends an IPC to the SSM daemon indicating RTM Route change
 *
 * Input:
 *   vrf_index and safi
 *
 * Output:
 *   NA
 *
 * Return values:
 *   0: OK.
 *   SSMD_PBR_PUBLIC_IPC_SEND_ERR: IPC send failed.
 */
ssmd_pbr_public_err_t
ssm_pbr_rtm_notification(char *vrf_name, unsigned short safi);

/*
 * This function sends an IPC to the SSM daemon indicating ARP reachability change
 *
 * Input:
 *   action, macAddr, ipAddr
 *
 * Output:
 *   NA
 *
 * Return values:
 *   0: OK.
 *   SSMD_PBR_PUBLIC_IPC_SEND_ERR: IPC send failed.
 */
ssmd_pbr_public_err_t
ssm_pbr_arp_notification(enum ssmd_pbr_public_arp_notify_action arp_action,
                         unsigned char *mac,
                         unsigned int ipAddress,
                         unsigned char *vrf_name,
                         unsigned int ifindex);

#endif /* _SSMD_PBR_PUBLIC_H_ */
