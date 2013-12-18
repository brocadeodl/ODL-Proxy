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

#ifndef BROCADE_VCS_H
#define BROCADE_VCS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define vcs__ns 699392245

#define vcs_Awaiting_Rejoin 7
#define vcs_Coming_Online 5
#define vcs_Offline 3
#define vcs_Online 2
#define vcs_Rejoining 6
#define vcs_Replacing 4
#define vcs_Unknown 1
#define vcs_address 1266954393
#define vcs_cluster_generic_status 1007237946
#define vcs_cluster_specific_status 1935454150
#define vcs_co_ordinator 501287396
#define vcs_co_ordinator_wwn 1710347238
#define vcs_firmware_version 1588326193
#define vcs_get_last_config_update_time 498985744
#define vcs_get_last_config_update_time_for_xpaths 2030937129
#define vcs_get_vcs_details 1228416875
#define vcs_ip 1643785435
#define vcs_last_config_update_time 1814042955
#define vcs_last_config_update_time_for_xpaths 895896421
#define vcs_local_node 1170793894
#define vcs_local_switch_wwn 1505834823
#define vcs_no_vcs_rbridge_context 1552165176
#define vcs_node_condition 1850804848
#define vcs_node_hw_sync_state 322930091
#define vcs_node_in_sync 4
#define vcs_node_internal_ip_address 2130662087
#define vcs_node_is_principal 1545806082
#define vcs_node_num 1610822670
#define vcs_node_out_of_sync 5
#define vcs_node_public_ip_address 65557384
#define vcs_node_public_ip_addresses 1724127552
#define vcs_node_public_ipv6_address 1864166847
#define vcs_node_public_ipv6_addresses 655672240
#define vcs_node_rbridge_id 2031818019
#define vcs_node_serial_num 629016829
#define vcs_node_state 1253769162
#define vcs_node_status 261919490
#define vcs_node_swbd_number 1884843680
#define vcs_node_switch_mac 413603018
#define vcs_node_switch_wwn 663184657
#define vcs_node_switchname 409722106
#define vcs_node_synchronizing 3
#define vcs_node_uninitialized 2
#define vcs_node_unknown 1
#define vcs_node_vcs_id 944596906
#define vcs_node_vcs_mode 1504587145
#define vcs_node_vcs_type 2104101311
#define vcs_nodes_disconnected_from_cluster 866338921
#define vcs_principal_switch_wwn 1945743502
#define vcs_rbridge_id 23403572
#define vcs_show_vcs 313179455
#define vcs_swbd_number 346007495
#define vcs_switch_fcf_mac 1080146947
#define vcs_total_nodes_in_cluster 724938969
#define vcs_vcs 1276246689
#define vcs_vcs_cluster_mode 627074736
#define vcs_vcs_cluster_type_info 208788598
#define vcs_vcs_details 952520288
#define vcs_vcs_fabric_cluster 3
#define vcs_vcs_guid 1789983806
#define vcs_vcs_management_cluster 4
#define vcs_vcs_mode 201220030
#define vcs_vcs_node_info 1938051019
#define vcs_vcs_nodes 609266997
#define vcs_vcs_rbridge_context 1094131099
#define vcs_vcs_stand_alone 2
#define vcs_vcs_unknown_cluster 1
#define vcs__wysereaoperationid_getclusterinfo_action_point "getclusterinfo-action-point"
#define vcs__wysereaoperationid_getvcsdetails_action_point "getvcsdetails-action-point"
#define vcs__wysereaoperationid_last_config_update_time_action_point "last-config-update-time-action-point"
#define vcs__wysereaoperationid_vcscontextrbridgeid_action_point "vcscontextrbridgeid-action-point"
#define vcs__wysereaconfigurationid_VcsModeWyserEaConfiguration "VcsModeWyserEaConfiguration"
#define vcs__wysereaconfigurationid_virtualipcpt "virtualipcpt"
#define vcs_vcsmode 345616749
#define vcs_virtual 487377987
#define vcs_virtual_ip_address 1021441954
#define vcs_xpath_string 663822712
#define vcs_xpath_strings 6841

#ifdef __cplusplus
}
#endif

#endif

