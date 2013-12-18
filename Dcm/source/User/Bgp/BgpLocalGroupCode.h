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
 *   Copyright (C) 2013-2018 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/


#ifndef BGPLOCALGROUPCODE_H
#define BGPLOCALGROUPCODE_H




typedef enum
{
    BGP_GROUP_CODE_DEFAULT                                      = 0,
    BGP_GROUP_CODE_LOCALS                                       = 50000,
    BGP_GROUP_CODE_VRF_NAME                                     = 50001,
    BGP_GROUP_CODE_LOCAL_AS                                     = 50002,
    BGP_GROUP_CODE_ALWAYS_COMPARE_MED                           = 50003,
    BGP_GROUP_CODE_AS_PATH_IGNORE                               = 50004,
    BGP_GROUP_CODE_CAPABILITY_AS4                               = 50005,
    BGP_GROUP_CODE_CHOICE_CLUSTER_ID                            = 50006,
    BGP_GROUP_CODE_CLUSTER_ID                                   = 50007,
    BGP_GROUP_CODE_CLUSTER_IP_ADDR                              = 50008,
    BGP_GROUP_CODE_COMPARE_MED_ASPATH                           = 50009,
    BGP_GROUP_CODE_COMPARE_ROUTERID                             = 50010,
    BGP_GROUP_CODE_DEFAULT_LOCAL_PREFERENCE                     = 50011,
    BGP_GROUP_CODE_DISTANCE                                     = 50012,
    BGP_GROUP_CODE_ENFORCE_FIRST_AS                             = 50013,
    BGP_GROUP_CODE_FAST_EXTERNAL_FALLOVER                       = 50014,
    BGP_GROUP_CODE_INSTALL_IGP_COST                             = 50015,
    BGP_GROUP_CODE_LOG_DAMPENING_DEBUG                          = 50016,
    BGP_GROUP_CODE_MAX_AS_LIMIT                                 = 50017,
    BGP_GROUP_CODE_MED_MISSING_AS_WORST                         = 50018,
    BGP_GROUP_CODE_TIMERS                                       = 50019,
    BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_PEER_GRP                 = 50020,
    BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_IPX_ADDR                 = 50021,
    BGP_GROUP_CODE_ROUTER_BGP_IPV4_UCAST                        = 50022,
    BGP_GROUP_CODE_NEI_REMOTE_AS                                = 6,
    BGP_GROUP_CODE_NEI_DESCRIPTION                              = 50024,
    BGP_GROUP_CODE_NEI_SHUTDOWN                                 = 50025,
    BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT                = 50026,
    BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND                      = 50027,
    BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF                      = 50028,
    BGP_GROUP_CODE_NEI_NEXT_HOP_SELF                            = 50029,
    BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS                     = 50030,
    BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL                   = 50031,
    BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE               = 50032,
    BGP_GROUP_CODE_NEI_EBGP_MULTIHOP                            = 50033,
    BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT                      = 50034,
    BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT                       = 50035,
    BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH                           = 50036,
    BGP_GROUP_CODE_NEI_MAX_AS_DISABLE                           = 50037,
    BGP_GROUP_CODE_NEI_PASSWORD                                 = 50038,
    BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS                         = 50039,
    BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE                 = 50040,
    BGP_GROUP_CODE_NEI_AS4_ENABLE                               = 50041,
    BGP_GROUP_CODE_NEI_AS4_DISABLE                              = 50042,
    BGP_GROUP_CODE_NEI_TIMERS                                   = 50043,
    BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE                     = 50044,
    BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4                       = 50045,
    BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH                        = 50046,
    BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP                       = 50047,
    BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI                         = 50048,
    BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS                        = 50049,
    BGP_GROUP_CODE_NEI_AS_OVERRIDE                              = 50050,
    BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND             = 50051,
    BGP_GROUP_CODE_NEI_IPVX_ADDRESS                             = 50052,
    BGP_GROUP_CODE_NEI_IP_PEER_GROUP                            = 7,
    BGP_GROUP_CODE_NEI_PEER_GROUP_NAME_KEY                      = 50054,
    BGP_GROUP_CODE_NEI_PEER_GROUP_NAME                          = 2,
    BGP_GROUP_CODE_AF_IPV4_AGGREGATE_ADDRESS                    = 50056,
    BGP_GROUP_CODE_AF_IPV4_NETWORK                              = 50057,
    BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED                    = 50058,
    BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_METRIC             = 50059,
    BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_ROUTE_MAP          = 50060,
    BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF                         = 50061,
    BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_INTERNAL                = 50062,
    BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALONE             = 50063,
    BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALTWO             = 50064,
    BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_METRIC                  = 50065,
    BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_ROUTE_MAP               = 50066,
    BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC                       = 50067,
    BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_METRIC                = 50068,
    BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_ROUTE_MAP             = 50069,
    BGP_GROUP_CODE_AF_IPV4_REDISTRIBUTE_INTERNAL                = 50070,
    BGP_GROUP_CODE_AF_IPV4_STATIC_NETWORK                       = 50071,
    BGP_GROUP_CODE_AF_NEIGHBOR_IPADDR                           = 50072,
    BGP_GROUP_CODE_AF_NEIGHBOR_PEER_GRP                         = 50073,
    BGP_GROUP_CODE_AF_MAX_PATH_LOAD_SHARING_VALUE               = 50074,
    BGP_GROUP_CODE_AF_MAX_PATH_EBGP                             = 50075,
    BGP_GROUP_CODE_AF_MAX_PATH_IBGP                             = 50076,
    BGP_GROUP_CODE_AF_MAX_PATH_USE_LOAD_SHARING                 = 50077,
    BGP_GROUP_CODE_AF_MULTI_PATH_EBGP                           = 50078,
    BGP_GROUP_CODE_AF_MULTI_PATH_IBGP                           = 50079,
    BGP_GROUP_CODE_AF_MULTI_PATH_AS                             = 50080,
    BGP_GROUP_CODE_AF_ALWAYS_PROPAGATE                          = 50081,
    BGP_GROUP_CODE_AF_DEFAULT_INFO_ORIGINATE                    = 50082,
    BGP_GROUP_CODE_AF_RIB_ROUTE_LIMIT                           = 50083,
    BGP_GROUP_CODE_AF_CLIENT_TO_CLIENT_REFLECTION               = 50084,
    BGP_GROUP_CODE_AF_CHOICE_DAMPENING                          = 50085,
    BGP_GROUP_CODE_AF_DAMPENING                                 = 50086,
    BGP_GROUP_CODE_AF_DAMPENING_TIME                            = 50087,
    BGP_GROUP_CODE_AF_DAMPENING_ROUTE_MAP                       = 50088,
    BGP_GROUP_CODE_AF_DEFAULT_METRIC                            = 50089,
    BGP_GROUP_CODE_AF_NEXT_HOP_ENABLE_DEFAULT                   = 50090,
    BGP_GROUP_CODE_AF_TABLE_MAP                                 = 50091,
    BGP_GROUP_CODE_AF_UPDATE_TIME                               = 50092,
    BGP_GROUP_CODE_AF_IPV4_UCAST_PRESENCE                       = 1,
    BGP_GROUP_CODE_AF_IPV4_UCAST_NEXT_HOP_RECURSION             = 50094,
    BGP_GROUP_CODE_AF_NEI_ACTIVATE                              = 50095,
    BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME               = 50096,
    BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME              = 50097,
    BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT                      = 50098,
    BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE                     = 50099,
    BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP           = 50100,
    BGP_GROUP_CODE_AF_PREFIX_LIST_IN                            = 50101,
    BGP_GROUP_CODE_AF_PREFIX_LIST_OUT                           = 50102,
    BGP_GROUP_CODE_AF_ROUTE_MAP_IN                              = 50103,
    BGP_GROUP_CODE_AF_ROUTE_MAP_OUT                             = 50104,
    BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT                    = 50105,
    BGP_GROUP_CODE_AF_SEND_COMMUNITY                            = 50106,
    BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP                      = 50107,
    BGP_GROUP_CODE_AF_WEIGHT                                    = 50108,
    BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_IP_ADDRESS                  = 50109,
    BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_PEER_GRP_NAME               = 50110,
    BGP_GROUP_CODE_AF_NETWORK_ADDR_PREFIX                       = 4,
    BGP_GROUP_CODE_AF_NETWORK_WEIGHT                            = 50112,
    BGP_GROUP_CODE_AF_NETWORK_BACKDOOR                          = 50113,
    BGP_GROUP_CODE_AF_NETWORK_ROUTE_MAP                         = 50114,
    BGP_GROUP_CODE_AGGR_ADDR_ADVERTISE_MAP                      = 50115,
    BGP_GROUP_CODE_AGGR_ADDR_AS_SET                             = 50116,
    BGP_GROUP_CODE_AGGR_ADDR_ATTRIBUTE_MAP                      = 50117,
    BGP_GROUP_CODE_AGGR_ADDR_SUMMARY_ONLY                       = 50118,
    BGP_GROUP_CODE_AGGR_ADDR_SUPPRESS_MAP                       = 50119,
    BGP_GROUP_CODE_AGGR_ADDR_PREFIX                             = 3,
    BGP_GROUP_CODE_STATIC_NETWORK_PREFIX                        = 5,
    BGP_GROUP_CODE_STATIC_NETWORK_DISTANCE                      = 50122
}BGP_GROUP_CODE_TYPES;

#endif

