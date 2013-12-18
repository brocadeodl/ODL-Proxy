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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : suw                                                     *
 **************************************************************************/

#ifndef IPPOLICYLOCALTYPES_H
#define IPPOLICYLOCALTYPES_H

namespace DcmNs
{

    typedef enum
    {
        IPPOLICYROUTEMAP = 0,
        IPPOLICYIPPREFIX,
        IPPOLICYIPASPATHACCESSLIST,
        IPPOLICYIPCOMMUNITYSTDACCESSLIST,
        IPPOLICYIPCOMMUNITYEXTACCESSLIST,
        IPPOLICYFILTERCHANGEUPDATEDELAY,
		IPPOLICYPBRVRF,
		IPPOLICYGETPBR,
		PBR_SHOW_ROUTEMAP
    } IpPolicy;

    typedef enum
    {
        pa_unknown = 0,
        permit = 1,
        deny = 2
    } PolicyAction;

    typedef enum
    {
        IPPOLICY_MO_CREATE = 0,
        IPPOLICY_MO_DEL,
        IPPOLICY_MO_SET,
        IPPOLICY_MO_UNSET
    } IpPolicyOpCode;

	typedef enum 
	{
		PBR_VRF_NEXTHOP_MO_CREATE = 0,
		PBR_VRF_NEXTHOP_MO_DEL,
		PBR_VRF_NEXTHOP_MO_SET,
		PBR_VRF_NEXTHOP_MO_UNSET,
		PBR_NEXTHOP_MO_CREATE,
		PBR_NEXTHOP_MO_DEL,
		PBR_NEXTHOP_MO_SET,
		PBR_NEXTHOP_MO_UNSET,
		PBR_GLOBAL_NEXTHOP_MO_CREATE,
		PBR_GLOBAL_NEXTHOP_MO_DEL,
		PBR_GLOBAL_NEXTHOP_MO_SET,
		PBR_GLOBAL_NEXTHOP_MO_UNSET
	} PbrVrfNexthopOpCode; 

    typedef enum
    {
        IPPOLICY_FILTERCHANGEUPDATE_CREATE = 0,
        IPPOLICY_FILTERCHANGEUPDATE_DEL,
        IPPOLICY_FILTERCHANGEUPDATE_CLEAR
    } FilterChangeUpdateOpCode;

    typedef enum
    {
        pr_unknown = 0,
        internal = 1,
        type_1 = 2,
        type_2 = 3,
        level_1 = 4,
        level_2 = 5
    } PolicyRouteType;

    typedef enum
    {
        bprt_unknown = 0,
        bgp_proto_external = 1,
        bgp_proto_static_network = 2,
        bgp_proto_internal = 3
    } BgpProtocolRouteType;

    typedef enum
    {
        pm_unknown = 0,
        add = 1,
        subtract = 2,
        assign = 3,
        none = 4
    } PolicyMetricAction;
/*
    typedef enum {
        routine = 0,
        priority = 1,
        immediate = 2,
        flash = 3,
        flash_override = 4,
        critical = 5,
        internet = 6,
        network = 7,
        defaultVal = 99
    } WellKnownPrecedence;
	*/

    typedef enum {
        CS0 = 0,
        CS1 = 8,
        CS2 = 16,
        CS3 = 24,
        CS4 = 32,
        CS5 = 40,
        CS6 = 48,
        CS7 = 56,
        AF11 = 10,
        AF12 = 12,
        AF13 = 14,
        AF21 = 18,
        AF22 = 20,
        AF23 = 22,
        AF31 = 26,
        AF32 = 28,
        AF33 = 30,
        AF41 = 34,
        AF42 = 36,
        AF43 = 38,
        EF = 46,
        defaultValDscp = 99
    } DscpPrecedence;

    typedef enum ipCommunityAclType
    {
       IPCOMMUNITY_UNKNOWN_ACL_TYPE = 0,
       IPCOMMUNITY_STD_ACL_TYPE,
       IPCOMMUNITY_EXT_ACL_TYPE
    } IpCommunityAclType;

    #define IP_POLICY_NAME_POS      2
    #define IP_POLICY_ACTION_POS    3
    #define IP_POLICY_NETMASK_POS   4
    #define IP_POLICY_LIMIT1_POS    5
    #define IP_POLICY_LIMIT2_POS    7

    /*Note: When changing this macro, also change the corresponding macro define for backend (MAX_BGP_MATCH_INTERFACE_PER_CLI) */
    #define IP_POLICY_MATCH_MAX_INTERFACES  7

	#define IP_POLICY_PBBR_IPV4_VAL_UINT32(x) (((uint8_t *)&x)[0]), (((uint8_t *)&x)[1]), \
                                            (((uint8_t *)&x)[2]), (((uint8_t *)&x)[3])

}
#endif                                            // IPPOLICYLOCALTYPES_H
