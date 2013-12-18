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

#ifndef __IGMP_FLAGS_
#define __IGMP_FLAGS_

/* flags for struct igmp_if.igif_sflags */
#define IGMP_IF_SFLAG_ACTIVE                    (1 << 0) /* agidwani: Means whether igmp_if_start() or 
                                                            igmp_if_stop() have done their thing on it. */
#define IGMP_IF_SFLAG_HOST_COMPAT_V1            (1 << 1)
#define IGMP_IF_SFLAG_HOST_COMPAT_V2            (1 << 2)
#define IGMP_IF_SFLAG_HOST_COMPAT_V3            (1 << 3)
#define IGMP_IF_SFLAG_IF_CONF_INHERITED         (1 << 4)
#define IGMP_IF_SFLAG_L2_MCAST_ENABLED          (1 << 5)
#define IGMP_IF_SFLAG_MCAST_ENABLED             (1 << 6)
#define IGMP_IF_SFLAG_MFC_MSG_IN_IF             (1 << 7)
#define IGMP_IF_SFLAG_QUERIER                   (1 << 8)
#define IGMP_IF_SFLAG_SNOOPING                  (1 << 9)
#define IGMP_IF_SFLAG_SNOOP_MROUTER_IF          (1 << 10) /* agidwani: Gets set for dynamically learnt mrouters. */
#define IGMP_IF_SFLAG_SNOOP_MROUTER_IF_CONFIG   (1 << 11) /* agidwani: I think this is supposed to indicate that
                                                             a port has been configured as an mrouter port in the vlan
                                                             AND the port is active (i.e. IGMP_IF_SFLAG_ACTIVE is set on it.) */

#define IGMP_IF_SFLAG_SVC_TYPE_L2               (1 << 12)
#define IGMP_IF_SFLAG_SVC_TYPE_L3               (1 << 13)

  /* this flag will be set while the snooping querier is waiting on startup 
     query timer to time-out. should unset the flag up on sending of first query or 
     querier-disable.
   */
#define IGMP_IF_SFLAG_QUERIER_WAITING           (1 << 14)
#define IGMP_IF_SFLAG_TCN_QUERY_PENDING         (1 << 15)

/* should the above igif_sflags change, pls change the 
 * following the following dump function as well.
 */
#define IGMP_DUMP_IGIF_SFLAGS(cli, sflags)\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_ACTIVE))\
   cli_out(cli, "    IGMP_IF_SFLAG_ACTIVE\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_HOST_COMPAT_V1))\
  cli_out(cli, "    IGMP_IF_SFLAG_HOST_COMPAT_V1\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_HOST_COMPAT_V2))\
  cli_out(cli, "    IGMP_IF_SFLAG_HOST_COMPAT_V2\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_HOST_COMPAT_V3))\
  cli_out(cli, "    IGMP_IF_SFLAG_HOST_COMPAT_V3\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_IF_CONF_INHERITED))\
  cli_out(cli, "    IGMP_IF_SFLAG_IF_CONF_INHERITED\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_L2_MCAST_ENABLED))\
  cli_out(cli, "    IGMP_IF_SFLAG_L2_MCAST_ENABLED\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_MCAST_ENABLED))\
  cli_out(cli, "    IGMP_IF_SFLAG_MCAST_ENABLED\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_MFC_MSG_IN_IF))\
  cli_out(cli, "    IGMP_IF_SFLAG_MFC_MSG_IN_IF\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_QUERIER))\
  cli_out(cli, "    IGMP_IF_SFLAG_QUERIER\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_SNOOPING))\
  cli_out(cli, "    IGMP_IF_SFLAG_SNOOPING\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_SNOOP_MROUTER_IF))\
  cli_out(cli, "    IGMP_IF_SFLAG_SNOOP_MROUTER_IF\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_SNOOP_MROUTER_IF_CONFIG))\
  cli_out(cli, "    IGMP_IF_SFLAG_SNOOP_MROUTER_IF_CONFIG\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_SVC_TYPE_L2))\
  cli_out(cli, "    IGMP_IF_SFLAG_SVC_TYPE_L2\n");\
if(CHECK_FLAG(sflags,  IGMP_IF_SFLAG_SVC_TYPE_L3))\
  cli_out(cli,"    IGMP_IF_SFLAG_SVC_TYPE_L3\n");\
if(CHECK_FLAG(sflags, IGMP_IF_SFLAG_QUERIER_WAITING))\
  cli_out(cli,"    IGMP_IF_SFLAG_QUERIER_WAITING\n");\
if(CHECK_FLAG(sflags, IGMP_IF_SFLAG_TCN_QUERY_PENDING))\
  cli_out(cli,"    IGMP_IF_SFLAG_TCN_QUERY_PENDING\n");

/* flags for struct igmp_if_conf.igifc_cflags */
#define IGMP_IF_CFLAG_CONFIG_ENABLED                (1 << 0) // agidwani: means L3 IGMP enabled via CLI
#define IGMP_IF_CFLAG_ACCESS_LIST                   (1 << 1)
#define IGMP_IF_CFLAG_IMMEDIATE_LEAVE               (1 << 2)
#define IGMP_IF_CFLAG_LAST_MEMBER_QUERY_COUNT       (1 << 3)
#define IGMP_IF_CFLAG_LAST_MEMBER_QUERY_INTERVAL    (1 << 4)
#define IGMP_IF_CFLAG_LIMIT_GREC                    (1 << 5)
#define IGMP_IF_CFLAG_MROUTE_PROXY                  (1 << 6)
#define IGMP_IF_CFLAG_PROXY_SERVICE                 (1 << 7)
#define IGMP_IF_CFLAG_QUERIER_TIMEOUT               (1 << 8)
#define IGMP_IF_CFLAG_QUERY_INTERVAL                (1 << 9)
#define IGMP_IF_CFLAG_QUERY_RESPONSE_INTERVAL       (1 << 10)
#define IGMP_IF_CFLAG_ROBUSTNESS_VAR                (1 << 11)
#define IGMP_IF_CFLAG_SNOOP_ENABLED                 (1 << 12) /* agidwani: Orig. code only sets this 
                                                                 on vlan intfs. It means that IGMP snooping 
                                                                 is explicitly enabled on the interface. */
#define IGMP_IF_CFLAG_SNOOP_DISABLED                (1 << 13) /* agidwani: It means that IGMP snooping has been 
                                                                 explicitly disabled on the interface. If IGMP snooping
                                                                 is explicitly globally disabled (on the instance) then we don't
                                                                 need to store the explicit disablement of it on the interface but
                                                                 if not, then we do need to store this explicit interface disablement
                                                                 since its is overriding the global setting. */
#define IGMP_IF_CFLAG_SNOOP_FAST_LEAVE              (1 << 14)
#define IGMP_IF_CFLAG_SNOOP_MROUTER_IF              (1 << 15) /* agidwani: On a vlan, this flag indicates that there is at least one port in 
                                                                 the vlan for which IGMP_MRTR_IF_ACTIVE is still set. 
                                                                 (as per igmp_if_update_mrtr_stat_grp_config()) And note that 
                                                                 IGMP_MRTR_IF_ACTIVE is set for an igif when an mrouter port is statically 
                                                                 configured on it. */

#define IGMP_IF_CFLAG_SNOOP_QUERIER                 (1 << 16) 
#define IGMP_IF_CFLAG_SNOOP_NO_REPORT_SUPPRESS      (1 << 17)
#define IGMP_IF_CFLAG_VERSION                       (1 << 18)
#define IGMP_IF_CFLAG_RA_OPT                        (1 << 19)
#define IGMP_IF_CFLAG_SNOOPQ_SOURCE_IP              (1 << 20) /* This is set when one explicitly configures an IP for the snooping querier 
                                                                 using the CLI 'ip igmp snooping querier source-ip'. However,while that 
                                                                 command is present in the original IPI stack, we don't support it so we 
                                                                 should never find this flag set anywhere. */
#define IGMP_IF_CFLAG_SNOOP_QUERY_INTERVAL          (1 << 21)
#define IGMP_IF_CFLAG_SNOOP_MROUTER_TIMEOUT         (1 << 22)

#define IGMP_IF_CFLAG_SNOOP_NO_FLOOD_ENABLE         (1 << 23) /* This flag indicates that IGMP snooping no flood is enabled on a vlan.
                                                                 This flag should only be set if snooping is enabled on the vlan. if it
                                                                 is not set, it means "no flood" is disabled on that vlan. 
																 When no flood is enabled, we do not flood the unknown multicast traffic
																 in the vlan, rather we drop it, or forward it to mrouter ports. */

/* should the above igifc_cflags change, pls change the 
 * following the following dump function as well.
 */
#define IGMP_DUMP_IGIFC_CFLAGS(cli,__igifc_cflags__) \
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_CONFIG_ENABLED))\
   cli_out(cli, "    IGMP_IF_CFLAG_CONFIG_ENABLED\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_ACCESS_LIST))\
   cli_out(cli, "    IGMP_IF_CFLAG_ACCESS_LIST\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_IMMEDIATE_LEAVE))\
  cli_out(cli, "    IGMP_IF_CFLAG_IMMEDIATE_LEAVE\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_LAST_MEMBER_QUERY_COUNT))\
  cli_out(cli, "    IGMP_IF_CFLAG_LAST_MEMBER_QUERY_COUNT\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_LAST_MEMBER_QUERY_INTERVAL))\
  cli_out(cli, "    IGMP_IF_CFLAG_LAST_MEMBER_QUERY_INTERVAL\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_LIMIT_GREC))\
  cli_out(cli, "    IGMP_IF_CFLAG_LIMIT_GREC\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_MROUTE_PROXY))\
  cli_out(cli, "    IGMP_IF_CFLAG_MROUTE_PROXY\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_PROXY_SERVICE))\
  cli_out(cli, "    IGMP_IF_CFLAG_PROXY_SERVICE\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_QUERIER_TIMEOUT))\
  cli_out(cli, "    IGMP_IF_CFLAG_QUERIER_TIMEOUT\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_QUERY_INTERVAL))\
  cli_out(cli, "    IGMP_IF_CFLAG_QUERY_INTERVAL\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_QUERY_RESPONSE_INTERVAL))\
  cli_out(cli, "    IGMP_IF_CFLAG_QUERY_RESPONSE_INTERVAL\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_ROBUSTNESS_VAR))\
 cli_out(cli, "    IGMP_IF_CFLAG_ROBUSTNESS_VAR\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_SNOOP_ENABLED))\
 cli_out(cli, "    IGMP_IF_CFLAG_SNOOP_ENABLED\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_SNOOP_DISABLED))\
 cli_out(cli, "    IGMP_IF_CFLAG_SNOOP_DISABLED\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_SNOOP_FAST_LEAVE))\
 cli_out(cli, "    IGMP_IF_CFLAG_SNOOP_FAST_LEAVE\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_SNOOP_MROUTER_IF))\
 cli_out(cli, "    IGMP_IF_CFLAG_SNOOP_MROUTER_IF\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_SNOOP_QUERIER))\
 cli_out(cli, "    IGMP_IF_CFLAG_SNOOP_QUERIER\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_SNOOP_NO_REPORT_SUPPRESS))\
 cli_out(cli, "    IGMP_IF_CFLAG_SNOOP_NO_REPORT_SUPPRESS\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_VERSION))\
 cli_out(cli, "    IGMP_IF_CFLAG_VERSION\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_RA_OPT))\
 cli_out(cli, "    IGMP_IF_CFLAG_RA_OPT\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_SNOOPQ_SOURCE_IP))\
 cli_out(cli, "    IGMP_IF_CFLAG_SNOOPQ_SOURCE_IP\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_SNOOP_MROUTER_TIMEOUT))\
 cli_out(cli, "    IGMP_IF_CFLAG_SNOOP_MROUTER_TIMEOUT\n");\
if(CHECK_FLAG(__igifc_cflags__, IGMP_IF_CFLAG_SNOOP_NO_FLOOD_ENABLE))\
 cli_out(cli, "    IGMP_IF_CFLAG_SNOOP_NO_FLOOD_ENABLE\n");

/* struct igmp_group_rec->igr_sflags */
#define IGMP_IGR_SFLAG_COMPAT_V1                (1 << 0)
#define IGMP_IGR_SFLAG_COMPAT_V2                (1 << 1)
#define IGMP_IGR_SFLAG_COMPAT_V3                (1 << 2)
#define IGMP_IGR_SFLAG_REPORT_PENDING           (1 << 3)
#define IGMP_IGR_SFLAG_MFC_PROGMED              (1 << 4)
#define IGMP_IGR_SFLAG_STATE_REFRESH            (1 << 5)
#define IGMP_IGR_SFLAG_STATIC                   (1 << 6)
#define IGMP_IGR_SFLAG_DYNAMIC                  (1 << 7)
#define IGMP_IGR_SFLAG_RBRIDGE                  (1 << 8)
#endif
