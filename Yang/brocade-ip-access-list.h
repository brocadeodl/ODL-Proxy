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

#ifndef BROCADE_IP_ACCESS_LIST_H
#define BROCADE_IP_ACCESS_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#define ip_access_list__ns 71180821

#define ip_access_list_access_group 1842585928
#define ip_access_list_access_list 1900334689
#define ip_access_list_ack 2014788115
#define ip_access_list_action 571883887
#define ip_access_list_af11 10
#define ip_access_list_af12 12
#define ip_access_list_af13 14
#define ip_access_list_af21 18
#define ip_access_list_af22 20
#define ip_access_list_af23 22
#define ip_access_list_af31 26
#define ip_access_list_af32 28
#define ip_access_list_af33 30
#define ip_access_list_af41 34
#define ip_access_list_af42 36
#define ip_access_list_af43 38
#define ip_access_list_any 1
#define ip_access_list_bgp 179
#define ip_access_list_bootpc 68
#define ip_access_list_bootps 67
#define ip_access_list_count 180894398
#define ip_access_list_critical 5
#define ip_access_list_cs1 8
#define ip_access_list_cs2 16
#define ip_access_list_cs3 24
#define ip_access_list_cs4 32
#define ip_access_list_cs5 40
#define ip_access_list_cs6 48
#define ip_access_list_cs7 56
#define ip_access_list_daytime 13
#define ip_access_list_default 0
#define ip_access_list_deny 2
#define ip_access_list_domain 53
#define ip_access_list_dport 2064863781
#define ip_access_list_dport_number_eq_neq_tcp 1895459895
#define ip_access_list_dport_number_eq_neq_udp 454121496
#define ip_access_list_dport_number_gt_tcp 2136660235
#define ip_access_list_dport_number_gt_udp 1946569132
#define ip_access_list_dport_number_lt_tcp 1608511513
#define ip_access_list_dport_number_lt_udp 1885260796
#define ip_access_list_dport_number_range_higher_tcp 1595731884
#define ip_access_list_dport_number_range_higher_udp 1453357508
#define ip_access_list_dport_number_range_lower_tcp 1404452995
#define ip_access_list_dport_number_range_lower_udp 547079369
#define ip_access_list_dscp 1118112132
#define ip_access_list_dst_host_any_dip 448712306
#define ip_access_list_dst_host_ip 746149290
#define ip_access_list_dst_mask 587857443
#define ip_access_list_echo 7
#define ip_access_list_ef 46
#define ip_access_list_eq 1
#define ip_access_list_extended 1097797017
#define ip_access_list_fin 1491610228
#define ip_access_list_flash 3
#define ip_access_list_flash_override 4
#define ip_access_list_ftp 21
#define ip_access_list_ftp_data 20
#define ip_access_list_gt 2
#define ip_access_list_hard_drop 3
#define ip_access_list_hide_ip_acl_ext 2063720235
#define ip_access_list_hide_ip_acl_std 406407952
#define ip_access_list_host 2
#define ip_access_list_hostname 101
#define ip_access_list_icmp 1
#define ip_access_list_immediate 2
#define ip_access_list_in 1
#define ip_access_list_internet 6
#define ip_access_list_ip 1643785435
#define ip_access_list_ip_access_list 953614387
#define ip_access_list_ip_acl 705722760
#define ip_access_list_ip_acl_interface 441437017
#define ip_access_list_ip_direction 226979192
#define ip_access_list__wysereaconfigurationid_IpaclAccessgroupIntFoCP "IpaclAccessgroupIntFoCP"
#define ip_access_list__wysereaconfigurationid_IpaclAccessgroupIntGiCP "IpaclAccessgroupIntGiCP"
#define ip_access_list__wysereaconfigurationid_IpaclAccessgroupIntPoCP "IpaclAccessgroupIntPoCP"
#define ip_access_list__wysereaconfigurationid_IpaclAccessgroupIntTeCP "IpaclAccessgroupIntTeCP"
#define ip_access_list__wysereaconfigurationid_IpaclAccessgroupIntVlanCP "IpaclAccessgroupIntVlanCP"
#define ip_access_list__wysereaconfigurationid_extIpAclCP "extIpAclCP"
#define ip_access_list__wysereaconfigurationid_extIpaclRuleCP "extIpaclRuleCP"
#define ip_access_list__wysereaconfigurationid_ip_acl_config_cp "ip_acl_config_cp"
#define ip_access_list__wysereaconfigurationid_stdIpAclCP "stdIpAclCP"
#define ip_access_list__wysereaconfigurationid_stdIpaclRuleCP "stdIpaclRuleCP"
#define ip_access_list_log 1285913844
#define ip_access_list_login 513
#define ip_access_list_lt 3
#define ip_access_list_max_reliability 1
#define ip_access_list_max_throughput 2
#define ip_access_list_mgmt_ip_access_list 847432086
#define ip_access_list_mgmt_ip_direction 1023982013
#define ip_access_list_min_delay 4
#define ip_access_list_name 1998270519
#define ip_access_list_neq 4
#define ip_access_list_network 7
#define ip_access_list_normal 0
#define ip_access_list_ntp 123
#define ip_access_list_out 2
#define ip_access_list_permit 1
#define ip_access_list_pim_auto_rp 496
#define ip_access_list_priority 1
#define ip_access_list_protocol_type 138627217
#define ip_access_list_push 632681101
#define ip_access_list_range 5
#define ip_access_list_rip 520
#define ip_access_list_routine 0
#define ip_access_list_rst 1445656776
#define ip_access_list_seq 1774006065
#define ip_access_list_seq_id 1180263038
#define ip_access_list_smtp 25
#define ip_access_list_snmp 161
#define ip_access_list_sport 789223547
#define ip_access_list_sport_number_eq_neq_tcp 1529576516
#define ip_access_list_sport_number_eq_neq_udp 64221816
#define ip_access_list_sport_number_gt_tcp 816594394
#define ip_access_list_sport_number_gt_udp 866193961
#define ip_access_list_sport_number_lt_tcp 1637983780
#define ip_access_list_sport_number_lt_udp 1703207656
#define ip_access_list_sport_number_range_higher_tcp 1938292561
#define ip_access_list_sport_number_range_higher_udp 1750390686
#define ip_access_list_sport_number_range_lower_tcp 190795161
#define ip_access_list_sport_number_range_lower_udp 1423095989
#define ip_access_list_src_host_any_sip 807876249
#define ip_access_list_src_host_ip 1531111487
#define ip_access_list_src_mask 1298192472
#define ip_access_list_standard 1881611236
#define ip_access_list_sync 2047869128
#define ip_access_list_syslog 514
#define ip_access_list_tacacs 49
#define ip_access_list_talk 517
#define ip_access_list_tcp 6
#define ip_access_list_telnet 23
#define ip_access_list_tftp 69
#define ip_access_list_time 37
#define ip_access_list_udp 17
#define ip_access_list_urg 843650196
#define ip_access_list_www 80

#ifdef __cplusplus
}
#endif

#endif

