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
 *    Copyright (c) 1996-2011 Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Description:
 *      System-level FC-SW header; including definitions and data structures
 *      used by inter-switch fabric code. Currently compliant with Revision
 *      3.3 of the ANSI specification.
 *      Mark Walden - Add McDATA "Native" Fabric Mode definitions and data structures.
 */

#ifndef __SYS_FC_FCSW_H__
#define	__SYS_FC_FCSW_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Inter-switch Inquiry Payload
 */
typedef struct {
	u_int	if_flags;	/* interface flags */
	char	if_name[16];	/* interface name */
	u_int	if_ipaddr[4];	/* IP address - enough space for IPv6 */
} iface_t;

typedef struct {
	u_int	ie_code;	/* code - 0x1c000000 */
	u_int	ie_vendor_id;	/* vendor ID */
	char	ie_name[32];	/* element name */
	u_int	ie_model;	/* model information */
	u_int	ie_fw_id;	/* firmware version */
	char	ie_info[60];	/* additional information */
	u_int	ie_gw;		/* gateway address */
	u_int	ie_ifaces;	/* number of network interfaces */
	iface_t	ie_iface[2];	/* network interface descriptions */
} ieINQ_t;

#ifdef __cplusplus
}
#endif

#endif /* __SYS_FC_FCSW_H__ */
