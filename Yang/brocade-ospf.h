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

#ifndef BROCADE_OSPF_H
#define BROCADE_OSPF_H

#ifdef __cplusplus
extern "C"
{
#endif

#define ospf__ns 1108202360

#define ospf_active 124277386
#define ospf_address 1266954393
#define ospf_adjacency 2009536835
#define ospf_advertise 1
#define ospf_all 2031982792
#define ospf_all_external 2058648714
#define ospf_all_link_onstartup 3272720
#define ospf_all_lsas 1761062102
#define ospf_all_lsas_onstartup 928913591
#define ospf_all_out 169775611
#define ospf_all_summary_external 333274537
#define ospf_all_vrfs 1689396748
#define ospf_allow_default_and_type4_out 2
#define ospf_allow_default_out 1
#define ospf_always 1980885055
#define ospf_area 1324291201
#define ospf_area_id 1827097903
#define ospf_auth_change_wait_time 64942206
#define ospf_auth_key 428333927
#define ospf_auth_key_table 264432710
#define ospf_authentication_key 249645056
#define ospf_auto_cost 977896865
#define ospf_bad_packet 1517365922
#define ospf_bgp 555337807
#define ospf_bgp_route_map 913744751
#define ospf_broadcast 1
#define ospf_connected 1024392365
#define ospf_connected_route_map 425349754
#define ospf_cost 1323324755
#define ospf_database_filter 23897588
#define ospf_database_overflow_interval 930217981
#define ospf_dead_interval 1055525143
#define ospf_def_orig_metric 1559987368
#define ospf_def_orig_metric_type 740367755
#define ospf_def_orig_route_map 1691459587
#define ospf_default_information_originate 1311514607
#define ospf_default_metric 1330470564
#define ospf_default_passive_interface 13085306
#define ospf_deny 1028777810
#define ospf_dist_value 1990941891
#define ospf_distance 436769799
#define ospf_distribute_list 984757319
#define ospf_dr_only 1986888070
#define ospf_encrypttype 1158586176
#define ospf_external 1
#define ospf_external_lsa 1575072770
#define ospf_external_lsa_onstartup 1646010723
#define ospf_external_lsa_val 918716406
#define ospf_external_lsa_val_onstartup 1072109994
#define ospf_external_lsdb_limit 180719858
#define ospf_hello_interval 208691019
#define ospf_hold_time 204730640
#define ospf_in 1143034163
#define ospf_init_delay 1733884909
#define ospf_inter_area 2
#define ospf_interface_fo_ospf_conf 529751910
#define ospf_interface_gi_ospf_conf 29873290
#define ospf_interface_loopback_ospf_conf 1516689255
#define ospf_interface_te_ospf_conf 656346150
#define ospf_interface_vlan_ospf_conf 1982489087
#define ospf_intra_area 3
#define ospf_key 1348858669
#define ospf_key_activation_wait_time 142302861
#define ospf_key_id 1459848464
#define ospf_key_table 1597491031
#define ospf_link 481374722
#define ospf_link_onstartup 289366756
#define ospf_log 1285913844
#define ospf_log_all 1005110306
#define ospf_log_checksum 507427901
#define ospf_log_database 594282705
#define ospf_log_retransmit 473127169
#define ospf_lsa_group_pacing 1718658710
#define ospf_mask 1167181041
#define ospf_match_metric 1
#define ospf_match_metric_val 1044680913
#define ospf_max_hold_time 896684571
#define ospf_max_metric 2130472127
#define ospf_md5_authentication 701221541
#define ospf_md5_authentication_key 136048919
#define ospf_metric 235154778
#define ospf_mtu_ignore 1397525384
#define ospf_neighbor 81789621
#define ospf_neighbor_addr 735986661
#define ospf_network 873253992
#define ospf_no_encrypt_auth_key 835354195
#define ospf_no_encrypt_auth_key_table 162739236
#define ospf_no_encrypt_key 1259821480
#define ospf_no_encrypt_key_id 1507781377
#define ospf_no_encrypt_key_table 1231981439
#define ospf_no_summary 654723629
#define ospf_no_summary1 901148372
#define ospf_non_broadcast 2
#define ospf_normal 1759395901
#define ospf_not_advertise 2
#define ospf_nssa 1248296090
#define ospf_nssa_translator 73014140
#define ospf_nssa_translator_flag 307714247
#define ospf_nssa_value 407808041
#define ospf_on_startup 819091937
#define ospf_ospf 1794178062
#define ospf_ospf_metric_type 938157796
#define ospf_ospf_route_option_all 1
#define ospf_ospf_route_option_connected 2
#define ospf_ospf_route_option_static 4
#define ospf_ospf1 653866048
#define ospf__wysereaconfigurationid_OSPFAreaWyserEaConfiguration "OSPFAreaWyserEaConfiguration"
#define ospf__wysereaconfigurationid_OSPFAreaRangeWyserEaConfiguration "OSPFAreaRangeWyserEaConfiguration"
#define ospf__wysereaconfigurationid_OSPFConfigWyserEaConfiguration "OSPFConfigWyserEaConfiguration"
#define ospf__wysereaconfigurationid_OSPFDenyRedistributeWyserEaConfiguration "OSPFDenyRedistributeWyserEaConfiguration"
#define ospf__wysereaconfigurationid_OSPFDistanceWyserEaConfiguration "OSPFDistanceWyserEaConfiguration"
#define ospf__wysereaconfigurationid_OSPFLoopbackInterfaceWyserEaConfiguration "OSPFLoopbackInterfaceWyserEaConfiguration"
#define ospf__wysereaconfigurationid_OSPFNeighborIPAddressWyserEaConfiguration "OSPFNeighborIPAddressWyserEaConfiguration"
#define ospf__wysereaconfigurationid_OSPFPermitRedistributeWyserEaConfiguration "OSPFPermitRedistributeWyserEaConfiguration"
#define ospf__wysereaconfigurationid_OSPFSummaryAddressWyserEaConfiguration "OSPFSummaryAddressWyserEaConfiguration"
#define ospf__wysereaconfigurationid_OSPFTeInterfaceWyserEaConfiguration "OSPFTeInterfaceWyserEaConfiguration"
#define ospf__wysereaconfigurationid_OSPFVirtualLinkWyserEaConfiguration "OSPFVirtualLinkWyserEaConfiguration"
#define ospf__wysereaconfigurationid_OSPFVlanInterfaceWyserEaConfiguration "OSPFVlanInterfaceWyserEaConfiguration"
#define ospf_out 3
#define ospf_passive 413335475
#define ospf_permit 1901628419
#define ospf_point_to_point 3
#define ospf_priority 1346277862
#define ospf_ptp 1376234806
#define ospf_ptp_link_onstartup 1850570329
#define ospf_range 28853268
#define ospf_range_address 142984014
#define ospf_range_cost 1864731786
#define ospf_range_effect 1039236571
#define ospf_range_mask 928204609
#define ospf_redist_value 266030761
#define ospf_redistribute 1418501979
#define ospf_ref_bandwidth 1517390201
#define ospf_reference_bandwidth 607719387
#define ospf_retransmit_interval 1593920370
#define ospf_rfc1583_compatibility 987295492
#define ospf_rfc1583_compatibility_flag 1015925306
#define ospf_route_map 100226878
#define ospf_route_option 1649534197
#define ospf_route_type 695663619
#define ospf_router_lsa 581144582
#define ospf_set_metric 2
#define ospf_set_metric_val 1866024653
#define ospf_spf 890445360
#define ospf_static 1504445999
#define ospf_static_route_map 544725961
#define ospf_stub 1231906719
#define ospf_stub_link_onstartup 1332448864
#define ospf_stub_value 621249864
#define ospf_sum_address 1400945201
#define ospf_sum_address_mask 1138427844
#define ospf_summary_address 1505023806
#define ospf_summary_lsa 349216404
#define ospf_summary_lsa_onstartup 1633939239
#define ospf_summary_lsa_val 1270782086
#define ospf_summary_lsa_val_onstartup 1104318501
#define ospf_throttle 714657076
#define ospf_time 190171445
#define ospf_timers 1742815315
#define ospf_transit 1761365771
#define ospf_transit_link_onstartup 995322675
#define ospf_transmit_delay 1385610005
#define ospf_type1 1
#define ospf_type2 2
#define ospf_use_active_ports 1251529324
#define ospf_virt_link_neighbor 718455749
#define ospf_virtual_link 1128308147
#define ospf_vrf 1281310328
#define ospf_vrf_lite_capability 2019772493
#define ospf_wait_for_bgp 1275728462

#ifdef __cplusplus
}
#endif

#endif

