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

#ifndef BROCADE_IP_POLICY_H
#define BROCADE_IP_POLICY_H

#ifdef __cplusplus
extern "C"
{
#endif

#define ip_policy__ns 934413215

#define ip_policy_AF11 10
#define ip_policy_AF12 12
#define ip_policy_AF13 14
#define ip_policy_AF21 18
#define ip_policy_AF22 20
#define ip_policy_AF23 22
#define ip_policy_AF31 26
#define ip_policy_AF32 28
#define ip_policy_AF33 30
#define ip_policy_AF41 34
#define ip_policy_AF42 36
#define ip_policy_AF43 38
#define ip_policy_CS0 0
#define ip_policy_CS1 8
#define ip_policy_CS2 16
#define ip_policy_CS3 24
#define ip_policy_CS4 32
#define ip_policy_CS5 40
#define ip_policy_CS6 48
#define ip_policy_CS7 56
#define ip_policy_EF 46
#define ip_policy_access_list 1900334689
#define ip_policy_acl_rmm 932869675
#define ip_policy_action_ipp 1020873955
#define ip_policy_action_rm 583868912
#define ip_policy_add 1
#define ip_policy_address 1266954393
#define ip_policy_as_path 192495808
#define ip_policy_as_path_access_list_name 1362159846
#define ip_policy_aspath_tag 1359492348
#define ip_policy_assign 3
#define ip_policy_automatic_tag 2030769946
#define ip_policy_bgp_route_type 679629208
#define ip_policy_comm_list 500681673
#define ip_policy_comm_list_name 212729614
#define ip_policy_community 81681536
#define ip_policy_community_access_list_name 901301444
#define ip_policy_community_list 628403157
#define ip_policy_content 1400276169
#define ip_policy_continue 1852005625
#define ip_policy_continue_holder 1974174264
#define ip_policy_continue_val 1616807642
#define ip_policy_dampening 1049243107
#define ip_policy_default 99
#define ip_policy_delta_rms 1688462041
#define ip_policy_deny 2
#define ip_policy_dist_rms 1397055285
#define ip_policy_distance 436769799
#define ip_policy_dscp 1118112132
#define ip_policy_dscp_rms 2085048180
#define ip_policy_extended 1097797017
#define ip_policy_external 1055777754
#define ip_policy_filter_change_update_delay 1303114355
#define ip_policy_filter_delay_value 329697748
#define ip_policy_fortygigabitethernet_rmm 491086848
#define ip_policy_ge_ipp 1160840094
#define ip_policy_gigabitethernet_rmm 191716828
#define ip_policy_global 1112611080
#define ip_policy_half_life 722675026
#define ip_policy_hide_as_path_holder 1593357658
#define ip_policy_hide_community_list_holder 1485991072
#define ip_policy_hide_filter_change_update_delay_holder 1659287968
#define ip_policy_hide_prefix_holder 1610395095
#define ip_policy_hide_routemap_holder 338469414
#define ip_policy_instance 136214150
#define ip_policy_interface 221572658
#define ip_policy_internal 1286562807
#define ip_policy_ip 1643785435
#define ip_policy_ip_action 182639591
#define ip_policy_ip_community_reg_expr 2052060735
#define ip_policy_ip_pbr_interface 1513571956
#define ip_policy_ip_reg_expr 869015574
#define ip_policy__wysereaconfigurationid_PBRIntFoCP "PBRIntFoCP"
#define ip_policy__wysereaconfigurationid_PBRIntGigCP "PBRIntGigCP"
#define ip_policy__wysereaconfigurationid_PBRIntPoCP "PBRIntPoCP"
#define ip_policy__wysereaconfigurationid_PBRIntTeCP "PBRIntTeCP"
#define ip_policy__wysereaconfigurationid_PBRIntVlanCP "PBRIntVlanCP"
#define ip_policy__wysereaconfigurationid_filterChangeUpdateDelay "filterChangeUpdateDelay"
#define ip_policy__wysereaconfigurationid_ipAsPathAccessList "ipAsPathAccessList"
#define ip_policy__wysereaconfigurationid_ipCommunityExtAccessList "ipCommunityExtAccessList"
#define ip_policy__wysereaconfigurationid_ipCommunityStdAccessList "ipCommunityStdAccessList"
#define ip_policy__wysereaconfigurationid_ipprefix_cp "ipprefix-cp"
#define ip_policy__wysereaconfigurationid_pbrglobalnexthop_cp "pbrglobalnexthop-cp"
#define ip_policy__wysereaconfigurationid_pbrnexthop_cp "pbrnexthop-cp"
#define ip_policy__wysereaconfigurationid_pbrvrf_cp "pbrvrf-cp"
#define ip_policy__wysereaconfigurationid_routemap_cp "routemap-cp"
#define ip_policy_le_ipp 380324890
#define ip_policy_local_preference 1044932322
#define ip_policy_local_preference_value 693692243
#define ip_policy_loopback 343067885
#define ip_policy_match 1501964518
#define ip_policy_match_comm_delete 1825743383
#define ip_policy_max_suppress_time 1395095106
#define ip_policy_metric 235154778
#define ip_policy_metric_rmm 99839598
#define ip_policy_metric_rms 211338415
#define ip_policy_metric_type 855418505
#define ip_policy_name 1998270519
#define ip_policy_next_global_hop 908299500
#define ip_policy_next_hop 1889650088
#define ip_policy_next_hop_filter_val 2076624629
#define ip_policy_next_ip 349794657
#define ip_policy_next_vrf 134504585
#define ip_policy_next_vrf_list 404224613
#define ip_policy_none 4
#define ip_policy_null0 1593719863
#define ip_policy_origin 2002579816
#define ip_policy_origin_igp 15391958
#define ip_policy_origin_incomplete 1302861595
#define ip_policy_peer_address 2130238605
#define ip_policy_permit 1
#define ip_policy_policy 1919655803
#define ip_policy_port_channel_rmm 1570558824
#define ip_policy_prefix_ipp 1269736256
#define ip_policy_prefix_list 1573095890
#define ip_policy_prefix_list_rmm 1265230057
#define ip_policy_prefix_list_rmm_n 596655625
#define ip_policy_prefix_list_rmrs 1971360820
#define ip_policy_prepend 344992584
#define ip_policy_protocol 943257313
#define ip_policy_protocol_bgp 1333675015
#define ip_policy_reuse 1223581657
#define ip_policy_route_map 100226878
#define ip_policy_route_map_name 1467882625
#define ip_policy_route_source 992480222
#define ip_policy_route_type 695663619
#define ip_policy_route_type_rmm 119431184
#define ip_policy_route_type_rms 1352297874
#define ip_policy_seq 1
#define ip_policy_seq_keyword 1576901242
#define ip_policy_set 1786000827
#define ip_policy_set_community_expr 151069163
#define ip_policy_standard 1881611236
#define ip_policy_static_network 2
#define ip_policy_std_community_expr 2011535680
#define ip_policy_sub 2
#define ip_policy_suppress 1746378947
#define ip_policy_tag 21925559
#define ip_policy_tag_empty 1078625924
#define ip_policy_tag_rmm 756255899
#define ip_policy_tag_rms 1800431237
#define ip_policy_tengigabitethernet_rmm 1925840353
#define ip_policy_type_1 1489976290
#define ip_policy_type_2 550461344
#define ip_policy_ve 290909456
#define ip_policy_vlan_rmm 127121424
#define ip_policy_vrf 1281310328
#define ip_policy_weight 306158986
#define ip_policy_weight_value 988545633

#ifdef __cplusplus
}
#endif

#endif

