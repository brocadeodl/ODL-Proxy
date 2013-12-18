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

/* Copyright (C) 2001-2005 IP Infusion, Inc. All Rights Reserved. */

#ifndef _ZEBOS_IGMP_DEFINES_H
#define _ZEBOS_IGMP_DEFINES_H

/* Default configuration settings for IGMP */
#define IGMP_DBG_FN_DESC_MAX                    (32)
#define HAVE_IGMP_DEBUG
#undef HAVE_IGMP_DEV_DEBUG

/* IGMP Module Miscellaneous defines */
#define IGMP_SPACE_STR                          " "

/* IGMP Module-wide Error Codes */
#define IGMP_SUCCESS                            (0)
#define IGMP_FAILURE                            (-1)
#define IGMP_ERR_NONE                           (0)
#define IGMP_ERR_GENERIC                        (-1)
#define IGMP_ERR_INVALID_COMMAND                (-2)
#define IGMP_ERR_INVALID_VALUE                  (-3)
#define IGMP_ERR_INVALID_FLAG                   (-4)
#define IGMP_ERR_INVALID_AF                     (-5)
#define IGMP_ERR_NO_SUCH_IFF                    (-6)
#define IGMP_ERR_NO_SUCH_GROUP_REC              (-7)
#define IGMP_ERR_NO_SUCH_SOURCE_REC             (-8)
#define IGMP_ERR_NO_SUCH_SVC_REG                (-9)
#define IGMP_ERR_NO_CONTEXT_INFO                (-10)
#define IGMP_ERR_NO_VALID_CONFIG                (-11)
#define IGMP_ERR_NO_SUCH_VALUE                  (-12)
#define IGMP_ERR_DOOM                           (-13)
#define IGMP_ERR_OOM                            (-14)
#define IGMP_ERR_SOCK_JOIN_FAIL                 (-15)
#define IGMP_ERR_MALFORMED_ARG                  (-16)
#define IGMP_ERR_QI_LE_QRI                      (-17)
#define IGMP_ERR_QRI_GT_QI                      (-18)
#define IGMP_ERR_MALFORMED_MSG                  (-19)
#define IGMP_ERR_TEMP_INTERNAL                  (-20)
#define IGMP_ERR_INTERNAL                       (-20)
#define IGMP_ERR_CFG_WITH_MROUTE_PROXY          (-21)
#define IGMP_ERR_CFG_FOR_PROXY_SERVICE          (-22)
#define IGMP_ERR_UNINIT_WITHOUT_DEREG           (-23)
#define IGMP_ERR_IF_GREC_LIMIT_REACHED          (-24)
#define IGMP_ERR_BUF_TOO_SHORT                  (-25)
#define IGMP_ERR_L2_PHYSICAL_IF                 (-26)
#define IGMP_ERR_L3_NON_VLAN_IF                 (-27)
#define IGMP_ERR_UNKNOWN_MSG                    (-28)
#define IGMP_ERR_L2_SOCK_FAIL                   (-29)
#define IGMP_ERR_L3_SOCK_FAIL                   (-30)
#define IGMP_ERR_API_GET                        (-31)
#define IGMP_ERR_SNOOP_ENABLED                  (-32)
#define IGMP_ERR_IGMP_ENABLED                   (-33)
#define IGMP_ERR_NOT_VLAN_MEMBER                (-34)
#define IGMP_ERR_DUP_MRTR_VLAN                  (-35)
#define IGMP_ERR_SNOOPQ_ENABLED                 (-36)
#define IGMP_ERR_MRTR_ENABLED                   (-37)
#define IGMP_ERR_INVALID_SOURCE_IP              (-38)
#define IGMP_ERR_PIM_SOCK_FAIL                  (-39)
#define IGMP_ERR_MEM_ALLOC_FAILURE              (-40)
#define IGMP_ERR_ILL_ADD                        (-41)
#define IGMP_ERR_NO_NOT_ALLOWED_ON_SEC_VLAN     (-42)
#define IGMP_ERR_MAX_LIMIT_REACHED              (-43)
#define IGMP_ERR_GROUP_LIMIT_REACHED            (-44)
#define IGMP_ERR_NOT_VLAG_PRIMARY               (-45)
#define IGMP_ERR_NULL_ARG_RCVD 		            (-46)

/* IGMP Show Commands Dispaly Formatting Flags */
#define IGMP_SHOW_FLAG_DISP_HEADER              (1 << 0)
#define IGMP_SHOW_FLAG_DISP_DETAIL              (1 << 1)
#define IGMP_SHOW_FLAG_DISP_INTF_AS_VLAN        (1 << 2)

/* IGMP IF Default Svc User ID (Used for L3 Svc IFS) */
#define IGMP_IF_SVC_USER_ID_DEF                 (0)

/* IGMP Input/Output Buffer Size */
#define IGMP_IO_BUFFER_SIZE                     (2048)

/* IGMP Interface Hash Table Size */
#define IGMP_IFHASH_TAB_SZ                      (47)

/* IGMP Invalid Socket-FD */
#define IGMP_SOCK_FD_INVALID                    (-1)

/* IGMP Message-Types */
#define IGMP_MSG_MEMBERSHIP_QUERY               (0x11)
#define IGMP_MSG_V1_MEMBERSHIP_REPORT           (0x12)
#define IGMP_MSG_V2_MEMBERSHIP_REPORT           (0x16)
#define IGMP_MSG_V3_MEMBERSHIP_REPORT           (0x22)
#define IGMP_MSG_V2_LEAVE_GROUP                 (0x17)
#define IGMP_MSG_DVMRP                          (0x13)

/* IGMP Messages related defines */
#ifndef IPOPT_RA
#define IPOPT_RA                                (148)
#endif /* IPOPT_RA */
#define IGMP_IPOPT_RA                           IPOPT_RA
#define IGMP_MFC_MSG_NOCACHE                    PAL_MCAST_NOCACHE
#define IGMP_MFC_MSG_WRONGVIF                   PAL_MCAST_WRONGVIF
#define IGMP_MFC_MSG_WHOLEPKT                   PAL_MCAST_WHOLEPKT
#define IGMP_RA_SIZE                            (4)
#define IGMP_MSG_TTL_DEF                        (1)
#define IGMP_MSG_MIN_LEN                        (8)
#define IGMP_MSG_HEADER_SIZE                    (4) /* agidwani: This is not the full IGMP header size but the size from the beginning of the packet uptil the beginning of the group address. */
#define IGMP_MSG_TIME_INTERVAL_EXPONENT         (0x80)
#define IGMP_MSG_TIME_INTERVAL_EXPONENT_MASK    (0x70)
#define IGMP_MSG_TIME_INTERVAL_BASE_EXPONENT    (3)
#define IGMP_MSG_TIME_INTERVAL_MANTISSA_BITSIZE (4)
#define IGMP_MSG_TIME_INTERVAL_MANTISSA_MASK    (0x0F)
#define IGMP_MSG_QUERY_MIN_SIZE                 (4)
#define IGMP_MSG_QUERY_V3_ADDITIONAL_MIN_SIZE   (4)
#define IGMP_MSG_QUERY_SUPPRESS_FLAG_MASK       (0x08)
#define IGMP_MSG_QUERY_QRV_MASK                 (0x07)
#define IGMP_MSG_V3_REPORT_MIN_SIZE             (8)
#define IGMP_MSG_V3_REPORT_GRP_REC_MIN_SIZE     (8)
#define IGMP_MSG_V3_REPORT_SRC_REC_MIN_SIZE     (4)
#define IGMP_HAVE_IP_OPT_RA                     (1)

/* Declaration of 32 bit Floating-point Bit mask */
#define IGMP_FLOAT32_EXPONENT_MASK              (0x7F800000UL)
#define IGMP_FLOAT32_SIGNIFICANT_MASK           (0x007FFFFFUL)
#define IGMP_FLOAT32_1SCOMPLEMENT_EXPONENT      (0x7F)
#define IGMP_FLOAT32_SIGNIFICANT_BIT_SIZE       (23)

/* Last Member Query Count */
#define IGMP_LAST_MEMBER_QUERY_COUNT_MIN        2
#define IGMP_LAST_MEMBER_QUERY_COUNT_DEF        2
#define IGMP_LAST_MEMBER_QUERY_COUNT_MAX        7

/* Last Member Query Interval in msec */
#define IGMP_LAST_MEMBER_QUERY_INTERVAL_MIN     100
#define IGMP_LAST_MEMBER_QUERY_INTERVAL_DEF     1000
#define IGMP_LAST_MEMBER_QUERY_INTERVAL_MAX     25500

/* Limit Group-Records */
#define IGMP_LIMIT_MIN                          1
#define IGMP_LIMIT_DEF                          0
#define IGMP_LIMIT_MAX                          2097152

/* IGMP Interface Robustness Variable value */
#define IGMP_ROBUSTNESS_VAR_MIN                 2
#define IGMP_ROBUSTNESS_VAR_DEF                 2
#define IGMP_ROBUSTNESS_VAR_MAX                 7

/* Query Interval in sec */
#define IGMP_QUERY_INTERVAL_MIN                 1
#define IGMP_QUERY_INTERVAL_DEF                 125
#define IGMP_QUERY_INTERVAL_MAX                 18000

/* Query Max-Response Timeout in sec */
#define IGMP_QUERY_RESPONSE_INTERVAL_MIN        1
#define IGMP_QUERY_RESPONSE_INTERVAL_DEF        10
#define IGMP_QUERY_RESPONSE_INTERVAL_MAX        25

#define IGMP_SNOOP_MROUTER_TIMEOUT_DEF          300 
#define IGMP_SNOOP_MROUTER_TIMEOUT_MIN          1
#define IGMP_SNOOP_MROUTER_TIMEOUT_MAX          60000

/* Other-Querier Timeout in sec */
#define IGMP_QUERIER_TIMEOUT_MIN                60
#define IGMP_QUERIER_TIMEOUT_DEF                                      \
  (IGMP_ROBUSTNESS_VAR_DEF * IGMP_QUERY_INTERVAL_DEF                  \
   + (IGMP_QUERY_RESPONSE_INTERVAL_DEF / 2))
#define IGMP_QUERIER_TIMEOUT_MAX                300

/* IGMP Interface Version value */
#define IGMP_VERSION_1                          1
#define IGMP_VERSION_2                          2
#define IGMP_VERSION_3                          3
#define IGMP_VERSION_MIN                        IGMP_VERSION_1
#ifdef HAVE_IGMP_V3
#define IGMP_VERSION_DEF                        IGMP_VERSION_3
#define IGMP_VERSION_MAX                        IGMP_VERSION_3
#else /* HAVE_IGMP_V3 */
#define IGMP_VERSION_DEF                        IGMP_VERSION_2
#define IGMP_VERSION_MAX                        IGMP_VERSION_2
#endif /* HAVE_IGMP_V3 */

/* IGMP Interface Miscellaneous values (sec) */
#define IGMP_WARN_RATE_LIMIT_INTERVAL           (15)
#define IGMP_GROUP_MEMBERSHIP_INTERVAL_DEF                            \
  (IGMP_ROBUSTNESS_VAR_DEF * IGMP_QUERY_INTERVAL_DEF                  \
   + IGMP_QUERY_RESPONSE_INTERVAL_DEF)

/*
 * Well-known Multicast Groups
 */
/* 224.0.0.0/8 */
#ifndef INADDR_MULTICAST_ADDRESS
#define INADDR_MULTICAST_ADDRESS                (0xE0000000UL)
#endif /* INADDR_MULTICAST_ADDRESS */
/* 224.0.0.1 */
#ifndef INADDR_ALLHOSTS_GROUP
#define INADDR_ALLHOSTS_GROUP                   (0xE0000001UL)
#endif /* INADDR_ALLHOSTS_GROUP */
/* 224.0.0.2 */
#ifndef INADDR_ALLRTRS_GROUP
#define INADDR_ALLRTRS_GROUP                    (0xE0000002UL)
#endif /* INADDR_ALLRTRS_GROUP */
/* 224.0.0.13 */
#ifndef INADDR_PIM_ALLRTRS_GROUP
#define INADDR_PIM_ALLRTRS_GROUP                (0xE000000DUL)
#endif /* INADDR_PIM_ALLRTRS_GROUP */
/* 224.0.0.22 */
#ifndef INADDR_IGMPv3_RTRS_GROUP
#define INADDR_IGMPv3_RTRS_GROUP                (0xE0000016UL)
#endif /* INADDR_IGMPv3_RTRS_GROUP */
/* 224.0.0.255 */
#ifndef INADDR_MAX_LOCAL_GROUP
#define INADDR_MAX_LOCAL_GROUP                  (0xE00000FFUL)
#endif /* INADDR_MAX_LOCAL_GROUP */
/* 232.0.0.0/8 */
#ifndef INADDR_SSM_RANGE_ADDRESS
#define INADDR_SSM_RANGE_ADDRESS                (0xE8000000UL)
#endif /* INADDR_MAX_LOCAL_ADDRESS */

/* IGMP SNMP Definitions */
#ifdef HAVE_SNMP

/* IGMP-MIB */
#define IGMPMIB 1,3,6,1,2,1,85

/* ZebOS enterprise IGMP MIB.  This variable is used for registering
   IGMP MIB to SNMP agent under SMUX protocol.  */
#define IGMPDOID 1,3,6,1,4,1,3317,1,2,25

/* IGMP MIB igmp_snmp_rtr_if_tab */
#define IGMPROUTERIFINDEX                    1
#define IGMPROUTERIFQTYPE                    2
#define IGMPROUTERIFQUERIER                  3
#define IGMPROUTERIFQINTERVAL                4
#define IGMPROUTERIFSTATUS                   5
#define IGMPROUTERIFVERSION                  6
#define IGMPROUTERIFQMAXRESTIME              7
#define IGMPROUTERIFQUPTIME                  8
#define IGMPROUTERIFQEXPIRYTIME              9
#define IGMPROUTERIFQWRONGVERQUERY           10
#define IGMPROUTERIFJOINS                    11
#define IGMPROUTERIFPROXYIFINDEX             12
#define IGMPROUTERIFGROUPS                   13
#define IGMPROUTERIFROBUSTNESS               14
#define IGMPROUTERIFLASTMEMQINTVL            15
#define IGMPROUTERIFLASTMEMQCOUNT            16
#define IGMPROUTERIFSTARTUPQCOUNT            17
#define IGMPROUTERIFSTARTUPQINTVL            18
/* IGMP MIB igmp_snmp_rtr_cache_tab */
#define IGMPROUTERCACHEADDRESSTYPE           19
#define IGMPROUTERCACHEADDRESS               20
#define IGMPROUTERCACHEIFINDEX               21
#define IGMPROUTERCACHELASTREPORTER          22
#define IGMPROUTERCACHEUPTIME                23
#define IGMPROUTERCACHEEXPIRYTIME            24
#define IGMPROUTERCACHEEXCLMODEEXPTIMER      25
#define IGMPROUTERCACHEVER1HOSTTIMER         26
#define IGMPROUTERCACHEVER2HOSTTIMER         27
#define IGMPROUTERCACHESRCFILTERMODE         28
/* IGMP MIB igmp_snmp_inv_rtr_cache_tab */
#define IGMPINVERSEROUTERCACHEIFINDEX        29
#define IGMPINVERSEROUTERCACHEADDRTYPE       30
#define IGMPINVERSEROUTERCACHEADDR           31
/* IGMP MIB igmp_snmp_rtr_src_list_tab */
#define IGMPROUTERSRCLISTADDRTYPE            32
#define IGMPROUTERSRCLISTADDR                33
#define IGMPROUTERSRCLISTIFINDEX             34
#define IGMPROUTERSRCLISTHOSTADDRESS         35
#define IGMPROUTERSRCLISTEXPIRE              36

#define IGMP_SNMP_OIDIDX_LEN_HSTIFTABENT           (2)
#define IGMP_SNMP_OIDIDX_LEN_RTRIFTABENT           (2)
#define IGMP_SNMP_OIDIDX_LEN_HSTCACHETABENT        (6)
#define IGMP_SNMP_OIDIDX_LEN_RTRCACHETABENT        (6)
#define IGMP_SNMP_OIDIDX_LEN_INVHSTCACHETABENT     (6)
#define IGMP_SNMP_OIDIDX_LEN_INVRTRCACHETABENT     (6)
#define IGMP_SNMP_OIDIDX_LEN_HSTSRCLSTTABENT       (10)
#define IGMP_SNMP_OIDIDX_LEN_RTRSRCLSTTABENT       (10)

#define IGMP_SNMP_INETADDRTYPE_IPV4           AFI_IP
#define IGMP_SNMP_INETADDRTYPE_IPV6           AFI_IP6
#define IGMP_SNMP_INETADDRTYPE_IPV4z          (3)
#define IGMP_SNMP_INETADDRTYPE_IPV6z          (4)
#define IGMP_SNMP_INETADDRTYPE_DNS            (16)

/* RowStatus. */
#define IGMP_SNMP_ROW_STATUS_ACTIVE           1
#define IGMP_SNMP_ROW_STATUS_NOTINSERVICE     2
#define IGMP_SNMP_ROW_STATUS_NOTREADY         3
#define IGMP_SNMP_ROW_STATUS_CREATEANDGO      4
#define IGMP_SNMP_ROW_STATUS_CREATEANDWAIT    5
#define IGMP_SNMP_ROW_STATUS_DESTROY          6

#define IGMP_MAX_GROUPS_IN_CALLISTO 2000
#define IGMP_MAX_GROUPS_IN_MERCURY  6000

#define IGMP_MAX_VLAN_LIMIT_CALLISTO 128
#define IGMP_MAX_VLAN_LIMIT_MERCURY 256

#endif /* HAVE_SNMP */
#endif /* _ZEBOS_IGMP_DEFINES_H */
