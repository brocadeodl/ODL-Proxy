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

#ifndef BROCADE_BGP_H
#define BROCADE_BGP_H

#ifdef __cplusplus
extern "C"
{
#endif

#define bgp__ns 1946463051

#define bgp_activate 2085232289
#define bgp_address_family 203507431
#define bgp_advertise_map 845648412
#define bgp_advertisement_interval 71113777
#define bgp_af_common_cmds_holder 142129084
#define bgp_af_ipv4_neighbor_address 683681435
#define bgp_af_ipv4_neighbor_address_holder 483990885
#define bgp_af_ipv4_neighbor_peergroup 782150615
#define bgp_af_ipv4_neighbor_peergroup_holder 87115983
#define bgp_af_ipv4_neighbor_peergroup_name 1944532477
#define bgp_af_ipv4_uc_and_vrf_cmds_call_point_holder 236928889
#define bgp_af_nei_weight 2125804763
#define bgp_aggregate_address 1005553390
#define bgp_aggregate_ip_prefix 202659831
#define bgp_always_compare_med 179211405
#define bgp_always_propagate 558570275
#define bgp_as_override 913433881
#define bgp_as_path_ignore 727867265
#define bgp_as_set 33490264
#define bgp_as4 2009670240
#define bgp_as4_enable 1475422795
#define bgp_associate_peer_group 1921840358
#define bgp_attribute_map 796237447
#define bgp_backdoor 511757065
#define bgp_bgp 555337807
#define bgp_bgp_redistribute_internal 70084746
#define bgp__wysereaconfigurationid_AfIpv4NeighborIpxAddr "AfIpv4NeighborIpxAddr"
#define bgp__wysereaconfigurationid_AfIpv4NeighborPeerGrp "AfIpv4NeighborPeerGrp"
#define bgp__wysereaconfigurationid_AfIpv4Network "AfIpv4Network"
#define bgp__wysereaconfigurationid_AfIpv4Ucast "AfIpv4Ucast"
#define bgp__wysereaconfigurationid_AfStaticNetwork "AfStaticNetwork"
#define bgp__wysereaconfigurationid_AggregateIpv4Address "AggregateIpv4Address"
#define bgp__wysereaconfigurationid_BgpBasic "BgpBasic"
#define bgp__wysereaconfigurationid_NeighborIpxAddress "NeighborIpxAddress"
#define bgp__wysereaconfigurationid_NeighborPeerGrp "NeighborPeerGrp"
#define bgp_both 1577301616
#define bgp_ca_cluster_id 1529857733
#define bgp_ca_cluster_id_ipv4_address 151424404
#define bgp_ca_dampening_flag 1909777285
#define bgp_ca_dampening_route_map 544400153
#define bgp_ca_dampening_specify_values 397086911
#define bgp_ca_ebgp_multihop_count 1794478041
#define bgp_ca_ebgp_multihop_flag 2089257057
#define bgp_ca_eth 1916637448
#define bgp_ca_ipv4 2062806762
#define bgp_ca_loopback 254770707
#define bgp_ca_maxas_limit_disable 1294446190
#define bgp_ca_maxas_limit_enable 2103186401
#define bgp_ca_next_hop_self_always 2045384178
#define bgp_ca_next_hop_self_status 888243678
#define bgp_ca_ve 1740855084
#define bgp_capability 1251322185
#define bgp_ch_cluster_id 185670578
#define bgp_ch_dampening_source 1492189876
#define bgp_ch_ebgp_multihop_type 1352914721
#define bgp_ch_maxas_limit 986478089
#define bgp_ch_next_hop_self_type 1509411094
#define bgp_ch_update_source 979499309
#define bgp_client_to_client_reflection 375152299
#define bgp_cluster_id 1203424155
#define bgp_cluster_id_ipv4_address 1422676967
#define bgp_cluster_id_value 1838134740
#define bgp_compare_med_empty_aspath 1441201966
#define bgp_compare_routerid 549561829
#define bgp_connected 1024392365
#define bgp_dampening 1049243107
#define bgp_dampening_flag 303202870
#define bgp_dampening_route_map 992818226
#define bgp_default_information_originate 1311514607
#define bgp_default_local_preference 1678013303
#define bgp_default_metric 1330470564
#define bgp_default_originate 988926361
#define bgp_default_originate_route_map 2028485153
#define bgp_default_originate_status 354801429
#define bgp_description 202559156
#define bgp_direction_in 1305317870
#define bgp_direction_out 1735612755
#define bgp_distance 436769799
#define bgp_ebgp 2120040446
#define bgp_ebgp_multihop 467962677
#define bgp_ebgp_multihop_count 441243982
#define bgp_ebgp_multihop_flag 2146233735
#define bgp_enforce_first_as 1600792329
#define bgp_ethernet 766975154
#define bgp_ethernet_interface 1422836389
#define bgp_ext_route_distance 1886199736
#define bgp_extended 1097797017
#define bgp_fast_external_fallover 1979072199
#define bgp_filter_list 1382624983
#define bgp_filter_list_direction_in_acl_name 1953559674
#define bgp_filter_list_direction_out_acl_name 2068116305
#define bgp_filter_list_name_direction_in 1830270703
#define bgp_filter_list_name_direction_out 1958912020
#define bgp_fortygigabitethernet 3
#define bgp_generate_rib_out 589633308
#define bgp_gigabitethernet 1
#define bgp_half_time 681121183
#define bgp_hold_time 204730640
#define bgp_ibgp 745466335
#define bgp_in 1143034163
#define bgp_inbound 1227481211
#define bgp_install_igp_cost 1212179544
#define bgp_int_route_distance 1574612545
#define bgp_interface_type 1566485963
#define bgp_ipv4 268057062
#define bgp_ipv4_unicast 2018585830
#define bgp_keep_alive 649483949
#define bgp_lcl_route_distance 801732145
#define bgp_load_sharing_value 1498416621
#define bgp_local_as 1447589626
#define bgp_local_as_value 1751025839
#define bgp_log_dampening_debug 1356499846
#define bgp_loopback 343067885
#define bgp_map_name 1579281984
#define bgp_match 1501964518
#define bgp_max_prefix_limit 341847231
#define bgp_max_suppress_time 1395095106
#define bgp_maxas_limit 1936216630
#define bgp_maxas_limit_disable 472463254
#define bgp_maximum_paths 1500334387
#define bgp_maximum_prefix 890264051
#define bgp_med_missing_as_worst 473867146
#define bgp_multi_as 1760646970
#define bgp_multipath 422046958
#define bgp_multipath_ebgp 699075655
#define bgp_multipath_ibgp 1393994241
#define bgp_nei_enforce_first_as 1938826518
#define bgp_nei_enforce_first_as_disable 664474285
#define bgp_nei_hold_time 1253175531
#define bgp_nei_keep_alive 212195286
#define bgp_neighbor_addr 735986661
#define bgp_neighbor_as4_disable 904537591
#define bgp_neighbor_as4_enable 498215381
#define bgp_neighbor_capability 1244578984
#define bgp_neighbor_ips 1453716008
#define bgp_neighbor_peer_grp 1247322438
#define bgp_neighbor_route_map 1955487125
#define bgp_neighbor_route_map_direction_in 1201967632
#define bgp_neighbor_route_map_direction_out 11174546
#define bgp_neighbor_route_map_name_direction_in 1572745301
#define bgp_neighbor_route_map_name_direction_out 1582826886
#define bgp_network 873253992
#define bgp_network_ipv4_address 610445144
#define bgp_network_route_map 2094031700
#define bgp_network_weight 1604817604
#define bgp_next_hop_enable_default 1765575907
#define bgp_next_hop_recursion 1031109626
#define bgp_next_hop_self 1630999918
#define bgp_next_hop_self_always 109236894
#define bgp_next_hop_self_status 650929983
#define bgp_no_prepend 1611967343
#define bgp_num_as_in_as_path 604043956
#define bgp_num_as_in_path 1250356992
#define bgp_ospf 1794178062
#define bgp_ospf_external1 1266512086
#define bgp_ospf_external2 490083645
#define bgp_ospf_internal 1157075385
#define bgp_ospf_metric 873357508
#define bgp_ospf_route_map 1646375391
#define bgp_password 880727685
#define bgp_peer_group_name 1377619148
#define bgp_peer_grps 567379453
#define bgp_prefix_list 1573095890
#define bgp_prefix_list_direction_in 1314537401
#define bgp_prefix_list_direction_in_prefix_name 1790475191
#define bgp_prefix_list_direction_out 490754833
#define bgp_prefix_list_direction_out_prefix_name 622883320
#define bgp_receive 2
#define bgp_redistribute 1418501979
#define bgp_redistribute_connected 22445203
#define bgp_redistribute_ospf 1725549633
#define bgp_redistribute_route_map 1378764560
#define bgp_redistribute_static 832436357
#define bgp_remote_as 81986873
#define bgp_remove_private_as 965066671
#define bgp_reuse_value 300276883
#define bgp_rib_route_limit 1837503029
#define bgp_route_reflector_client 1028338377
#define bgp_router_bgp_attributes 379616463
#define bgp_router_bgp_cmds_holder 2073282765
#define bgp_router_bgp_neighbor_address 713278112
#define bgp_router_bgp_neighbor_peer_grp 1720912256
#define bgp_send 1
#define bgp_send_community 223183420
#define bgp_send_community_status 1842173546
#define bgp_shutdown 460723287
#define bgp_shutdown_status 584724498
#define bgp_sip_ipv4_address 643914099
#define bgp_soft_reconfiguration 669996521
#define bgp_standard 1881611236
#define bgp_start_suppress_time 943188690
#define bgp_static 1504445999
#define bgp_static_network 1963910285
#define bgp_static_network_address 1636665334
#define bgp_static_network_distance 2028541312
#define bgp_static_route_map 544725961
#define bgp_summary_only 488813929
#define bgp_suppress_map 1498656077
#define bgp_table_map 1253712760
#define bgp_table_map_route_map 1682511200
#define bgp_teardown 2048169649
#define bgp_tengigabitethernet 2
#define bgp_threshold 1161930343
#define bgp_threshold_holder 1611993032
#define bgp_timers 1742815315
#define bgp_unicast_metric 675195371
#define bgp_unicast_static_metric 1873641231
#define bgp_unsuppress_map 935724778
#define bgp_update_source 893206494
#define bgp_update_time 1719725355
#define bgp_use_load_sharing 1751180954
#define bgp_value 864367181
#define bgp_values 1583768736
#define bgp_ve_interface 658311338
#define bgp_vrf_name 2140916904

#ifdef __cplusplus
}
#endif

#endif

