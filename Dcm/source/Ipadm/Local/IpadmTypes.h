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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : cshah                                                     *
 **************************************************************************/

#ifndef IPADMTYPESH_H
#define IPADMTYPESH_H

namespace DcmNs
{

    typedef enum
    {
        IPADMIPADDRESSCONFIG = 0,
        IPADMMASKCONFIG,
        IPADMGATEWAYCONFIG,
        IPADMICMPUNREACHABLECONFIG,
        IPADMICMPECHOREPLYCONFIG,
        IPADMICMPRATELIMITINGCONFIG,
        IPADMICMPV6UNREACHABLECONFIG,
        IPADMICMPV6ECHOREPLYCONFIG,
        IPADMICMPV6RATELIMITINGCONFIG,
        IPADMDHCPCONFIG,
		IPADMIPV6ADDRESSCONFIG,
		IPADMIPV6PREFIXCONFIG,
		IPADMIPV6AUTOCONFIG,
		IPADMLINKSPEEDCONFIG,
		IPADMREMOVECONFIG,
		IPADMCHASSISCONFIG,
		IPADMCHASSISDELETE,
		IPADMDNSCONFIG,		/* for global msg */
		IPADMNSCFG,		/* for Name server ip */
		IPADMDNCFG,		/* for Domain Name */
		IPADMIPV6ROUTECONFIG,	/* for IPv6 Route add */
		IPADMIPV6DHCPCONFIG	/* for IPv6 dhcp */
    } Ipadm;

    typedef enum
    {
		M_LINK_SPEED_AUTO = 0,
		M_LINK_SPEED_10,
		M_LINK_SPEED_100
		// M_LINK_SPEED_1000,  ethmode says GigE modes can not be forced.
    } LinkSpeed;

//addressType 0 implies IPv4 and 1 as IPv6
typedef enum
    {
        IPADM_ADDR_IPV4 = 0,
        IPADM_ADDR_IPV6 
    } IpadmAddressType;

     typedef enum
        {
                MGMNT_ROUTE_NO_CHOICE = 0,
                MGMNT_ROUTE_NEXTHOP,
                MGMNT_ROUTE_INTERFACE
        } MgmntRouteChoice;


}
#endif                                            //IPADMTYPESH_H
