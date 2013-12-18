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

#ifndef BROCADE_QOS_H
#define BROCADE_QOS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define qos__ns 1002900239

#define qos_auto_qos 1467931084
#define qos_burst 1259488663
#define qos_cos 762246620
#define qos_cos_mutation 267997825
#define qos_cos_threshold 1102841510
#define qos_cos_traffic_class 1645992366
#define qos_cos0 1864263392
#define qos_cos0_threshold 1816403011
#define qos_cos1 584655999
#define qos_cos1_threshold 967503908
#define qos_cos2 739232254
#define qos_cos2_threshold 483586641
#define qos_cos3 1626852221
#define qos_cos3_threshold 1236902798
#define qos_cos4 624645456
#define qos_cos4_threshold 1376577109
#define qos_cos5 2084727040
#define qos_cos5_threshold 1846184341
#define qos_cos6 1876894153
#define qos_cos6_threshold 1059455270
#define qos_cos7 311832713
#define qos_cos7_threshold 1296045783
#define qos_default_cos 1751827346
#define qos_drop_probability 1211774092
#define qos_dscp 1118112132
#define qos_dscp_cos 473850896
#define qos_dscp_cos_map_name 1234367750
#define qos_dscp_in_values 516636505
#define qos_dscp_mutation 893259192
#define qos_dscp_mutation_map_name 8765951
#define qos_dscp_traffic_class 2018517111
#define qos_dscp_traffic_class_map_name 1088637174
#define qos_dwrr 307165511
#define qos_dwrr_traffic_class_last 1741021851
#define qos_dwrr_traffic_class0 1699029728
#define qos_dwrr_traffic_class1 1265613945
#define qos_dwrr_traffic_class2 1214587564
#define qos_dwrr_traffic_class3 1772416598
#define qos_dwrr_traffic_class4 892829479
#define qos_dwrr_traffic_class5 820810639
#define qos_dwrr_traffic_class6 985535564
#define qos_dwrr_traffic_class7 1603844551
#define qos_flowcontrol 1165835935
#define qos_flowcontrol_rx 1280436953
#define qos_flowcontrol_tx 440268204
#define qos_limit 1553282341
#define qos_link_level_flowcontrol 1999011338
#define qos_map 426659489
#define qos_mark 915493668
#define qos_max_threshold 1138579592
#define qos_min_threshold 769054133
#define qos_multicast 1213561005
#define qos_name 1998270519
#define qos_nas 371020630
#define qos_off 0
#define qos_on 1
#define qos_pfc 1671005254
#define qos_pfc_cos 368658507
#define qos_pfc_flowcontrol_rx 92142236
#define qos_pfc_flowcontrol_tx 1624050797
#define qos_priority_number 326147360
#define qos_priority_tag_enable 1816757478
#define qos_profile_id 1309383196
#define qos_qos 2021583496
#define qos__wysereaconfigurationid_cos_mutation "cos_mutation"
#define qos__wysereaconfigurationid_cos_profile_po "cos_profile_po"
#define qos__wysereaconfigurationid_cos_profile_te "cos_profile_te"
#define qos__wysereaconfigurationid_cos_traffic_class "cos_traffic_class"
#define qos__wysereaconfigurationid_dscp_cos "dscp_cos"
#define qos__wysereaconfigurationid_dscp_mark_list_cos "dscp_mark_list_cos"
#define qos__wysereaconfigurationid_dscp_mark_list_mutation "dscp_mark_list_mutation"
#define qos__wysereaconfigurationid_dscp_mark_list_traffic_class "dscp_mark_list_traffic_class"
#define qos__wysereaconfigurationid_dscp_mutation "dscp_mutation"
#define qos__wysereaconfigurationid_dscp_traffic_class "dscp_traffic_class"
#define qos__wysereaconfigurationid_interface_fortygigabit "interface_fortygigabit"
#define qos__wysereaconfigurationid_interface_onegigabit "interface_onegigabit"
#define qos__wysereaconfigurationid_interface_po "interface_po"
#define qos__wysereaconfigurationid_interface_tengigabite "interface_tengigabite"
#define qos__wysereaconfigurationid_multicast_queue_scheduler "multicast_queue_scheduler"
#define qos__wysereaconfigurationid_pfc_flowcontrol_po "pfc_flowcontrol_po"
#define qos__wysereaconfigurationid_pfc_flowcontrol_te "pfc_flowcontrol_te"
#define qos__wysereaconfigurationid_qos_cos_threshold "qos_cos_threshold"
#define qos__wysereaconfigurationid_qos_nas_config "qos_nas_config"
#define qos__wysereaconfigurationid_qos_nas_serverip "qos_nas_serverip"
#define qos__wysereaconfigurationid_qos_nas_serverip_vlan "qos_nas_serverip_vlan"
#define qos__wysereaconfigurationid_qos_nas_serverip_vrf "qos_nas_serverip_vrf"
#define qos__wysereaconfigurationid_qos_red_profile "qos_red_profile"
#define qos__wysereaconfigurationid_rcv_queue_multicast "rcv_queue_multicast"
#define qos__wysereaconfigurationid_unicast_scheduler "unicast_scheduler"
#define qos_queue 1207615863
#define qos_random_detect 1626324771
#define qos_rate_limit 1980303890
#define qos_rcv_queue 1201132142
#define qos_red_cos_value 1417437382
#define qos_red_profile 546458683
#define qos_red_profile_id 429970613
#define qos_scheduler 930765920
#define qos_scheduler_type 1993344529
#define qos_scheduler_type_dwrr 0
#define qos_server_ip 1434589413
#define qos_set 1786000827
#define qos_strict_priority 1881065958
#define qos_threshold 1161930343
#define qos_to 809436769
#define qos_traffic_class0 768090353
#define qos_traffic_class1 636414221
#define qos_traffic_class2 1794986295
#define qos_traffic_class3 1808406284
#define qos_traffic_class4 1198050916
#define qos_traffic_class5 1899936055
#define qos_traffic_class6 1571323377
#define qos_traffic_class7 738155357
#define qos_trust 2130484176
#define qos_trust_cos 1941680285
#define qos_trust_dscp 1620108962
#define qos_vlan 71481437
#define qos_vlan_number 661637713
#define qos_vrf 1281310328
#define qos_vrf_name 2140916904

#ifdef __cplusplus
}
#endif

#endif

