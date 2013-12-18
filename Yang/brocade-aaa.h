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

#ifndef BROCADE_AAA_H
#define BROCADE_AAA_H

#ifdef __cplusplus
extern "C"
{
#endif

#define brcd_aaa__ns 1807968882

#define brcd_aaa_0 0
#define brcd_aaa_7 7
#define brcd_aaa_Result 877277942
#define brcd_aaa_aaa 2073600231
#define brcd_aaa_aaa_config 1722112787
#define brcd_aaa_accept 0
#define brcd_aaa_accounting 803748480
#define brcd_aaa_action 571883887
#define brcd_aaa_ad_group 688936092
#define brcd_aaa_admin_lockout_enable 1474009151
#define brcd_aaa_alias 159608788
#define brcd_aaa_alias_config 1321712754
#define brcd_aaa_auth_port 1663739573
#define brcd_aaa_authentication 838318934
#define brcd_aaa_banner 1707654397
#define brcd_aaa_basedn 1695256610
#define brcd_aaa_bp_rate_limit 63
#define brcd_aaa__wysereaoperationid_clear_sessions_wysereaoperation "clear_sessions_wysereaoperation"
#define brcd_aaa__wysereaoperationid_user_unlock_ap "user_unlock_ap"
#define brcd_aaa__wysereaconfigurationid_ConfdAliasUserWyserEaConfiguration "ConfdAliasUserWyserEaConfiguration"
#define brcd_aaa__wysereaconfigurationid_ConfdCliCommandAliasWyserEaConfiguration "ConfdCliCommandAliasWyserEaConfiguration"
#define brcd_aaa__wysereaconfigurationid_ConfdCliCommandGlobalAliasWyserEaConfiguration "ConfdCliCommandGlobalAliasWyserEaConfiguration"
#define brcd_aaa__wysereaconfigurationid_acc_cmd_cp "acc_cmd_cp"
#define brcd_aaa__wysereaconfigurationid_acc_exec_cp "acc_exec_cp"
#define brcd_aaa__wysereaconfigurationid_auth_login_cp "auth_login_cp"
#define brcd_aaa__wysereaconfigurationid_banner_cp "banner_cp"
#define brcd_aaa__wysereaconfigurationid_ldap_host_cp "ldap_host_cp"
#define brcd_aaa__wysereaconfigurationid_maprole_cp "maprole_cp"
#define brcd_aaa__wysereaconfigurationid_password_attributes_cp "password_attributes_cp"
#define brcd_aaa__wysereaconfigurationid_radius_host_cp "radius_host_cp"
#define brcd_aaa__wysereaconfigurationid_role_cp "role_cp"
#define brcd_aaa__wysereaconfigurationid_rule_cp "rule_cp"
#define brcd_aaa__wysereaconfigurationid_service_password_encryption_cp "service_password_encryption_cp"
#define brcd_aaa__wysereaconfigurationid_tacacs_host_cp "tacacs_host_cp"
#define brcd_aaa__wysereaconfigurationid_user_cp "user_cp"
#define brcd_aaa_cee_map 46
#define brcd_aaa_chap 0
#define brcd_aaa_character_restriction 204798507
#define brcd_aaa_cidrecov 3
#define brcd_aaa_clear 634788371
#define brcd_aaa_clear_a 227748976
#define brcd_aaa_clear_b 1953447269
#define brcd_aaa_clear_c 2075730165
#define brcd_aaa_clear_cl 2141733492
#define brcd_aaa_clear_cont 1289171142
#define brcd_aaa_clear_logging 2139546258
#define brcd_aaa_clear_support 182022315
#define brcd_aaa_clock 28
#define brcd_aaa_cmdlist 829097933
#define brcd_aaa_command 36215204
#define brcd_aaa_command_aaa 17
#define brcd_aaa_command_alias_config 26
#define brcd_aaa_command_banner 24
#define brcd_aaa_command_fcoe 16
#define brcd_aaa_command_ldap_server 20
#define brcd_aaa_command_password_attributes 21
#define brcd_aaa_command_radius_server 18
#define brcd_aaa_command_role 23
#define brcd_aaa_command_rule 25
#define brcd_aaa_command_service 62
#define brcd_aaa_command_tacacs_server 19
#define brcd_aaa_command_unlock 60
#define brcd_aaa_command_username 22
#define brcd_aaa_commands 913421552
#define brcd_aaa_configure 33
#define brcd_aaa_container_cmds 382490255
#define brcd_aaa_copy 1887233912
#define brcd_aaa_copy_a 103001367
#define brcd_aaa_copy_b 1842121400
#define brcd_aaa_copy_c 843945278
#define brcd_aaa_copy_cont 2122143303
#define brcd_aaa_copy_cp 803149876
#define brcd_aaa_copy_running 182572886
#define brcd_aaa_copy_support 1786013682
#define brcd_aaa_debug 61
#define brcd_aaa_debug1 4
#define brcd_aaa_default 0
#define brcd_aaa_defaultacc 1499122374
#define brcd_aaa_delete 41
#define brcd_aaa_desc 1535721889
#define brcd_aaa_df 38
#define brcd_aaa_diag 43
#define brcd_aaa_dir 37
#define brcd_aaa_dot1x 0
#define brcd_aaa_enable 448570367
#define brcd_aaa_encryption_level 762992865
#define brcd_aaa_enumList 786526438
#define brcd_aaa_exec 1716494663
#define brcd_aaa_expansion 1432673402
#define brcd_aaa_fabric 30
#define brcd_aaa_fastboot 59
#define brcd_aaa_fcoe_cont 1104176227
#define brcd_aaa_fcoe_leaf 46998250
#define brcd_aaa_filter_change_update_delay 31
#define brcd_aaa_firmware 36
#define brcd_aaa_first 1598348016
#define brcd_aaa_gigabitethernet_cont 1445103196
#define brcd_aaa_gigabitethernet_leaf 161051255
#define brcd_aaa_group 1540685982
#define brcd_aaa_host 1711485895
#define brcd_aaa_hostname 840214230
#define brcd_aaa_incoming 1857744478
#define brcd_aaa_index 827593980
#define brcd_aaa_interface 221572658
#define brcd_aaa_interface_a 506487674
#define brcd_aaa_interface_b 1513769770
#define brcd_aaa_interface_c 1545849228
#define brcd_aaa_interface_cont 1556121356
#define brcd_aaa_interface_d 721729925
#define brcd_aaa_interface_e 1768125679
#define brcd_aaa_interface_f 323846728
#define brcd_aaa_interface_fcoe 259771396
#define brcd_aaa_interface_fcoe_leaf 1516479021
#define brcd_aaa_interface_g 536578711
#define brcd_aaa_interface_ge 6800410
#define brcd_aaa_interface_ge_leaf 771047129
#define brcd_aaa_interface_h 1766207180
#define brcd_aaa_interface_i 1161199504
#define brcd_aaa_interface_int 729688334
#define brcd_aaa_interface_j 1462235739
#define brcd_aaa_interface_k 1638652769
#define brcd_aaa_interface_l 2010693842
#define brcd_aaa_interface_mgmt 1689876584
#define brcd_aaa_interface_pc 1851583490
#define brcd_aaa_interface_pc_leaf 447038669
#define brcd_aaa_interface_te 1382626010
#define brcd_aaa_interface_te_leaf 350203181
#define brcd_aaa_interface_vlan 389906115
#define brcd_aaa_interface_vlan_leaf 893676953
#define brcd_aaa_ip 48
#define brcd_aaa_ipv6 49
#define brcd_aaa_key 1348858669
#define brcd_aaa_lacp 52
#define brcd_aaa_lccli 5
#define brcd_aaa_ldap 3
#define brcd_aaa_ldap_server 1843402550
#define brcd_aaa_license 32
#define brcd_aaa_lldp_cont 1986612606
#define brcd_aaa_local 4
#define brcd_aaa_local_auth_fallback 5
#define brcd_aaa_logging 35
#define brcd_aaa_logging_cont 821885093
#define brcd_aaa_login 1704196980
#define brcd_aaa_lower 141911956
#define brcd_aaa_mac 9
#define brcd_aaa_mac_address_table 1
#define brcd_aaa_mac_rebalance 2
#define brcd_aaa_management 50
#define brcd_aaa_management_cont 1378746581
#define brcd_aaa_maprole 1443654840
#define brcd_aaa_max_retry 949061210
#define brcd_aaa_min_length 1311920431
#define brcd_aaa_monitor 11
#define brcd_aaa_motd 1964962555
#define brcd_aaa_name 1998270519
#define brcd_aaa_no_operation 64
#define brcd_aaa_none 1
#define brcd_aaa_ntp 29
#define brcd_aaa_numeric 1901932170
#define brcd_aaa_operation 1037011282
#define brcd_aaa_oscmd 6
#define brcd_aaa_pap 1
#define brcd_aaa_password_attributes 699852746
#define brcd_aaa_password_encryption 51398110
#define brcd_aaa_peap_mschap 2
#define brcd_aaa_port 10387472
#define brcd_aaa_port_channel_cont 1122184333
#define brcd_aaa_port_channel_leaf 2140510702
#define brcd_aaa_port_profile 15
#define brcd_aaa_prompt1 39
#define brcd_aaa_prompt2 40
#define brcd_aaa_protocol 943257313
#define brcd_aaa_protocol_a 1952487732
#define brcd_aaa_protocol_b 309210635
#define brcd_aaa_protocol_c 991595057
#define brcd_aaa_protocol_cont 199031353
#define brcd_aaa_protocol_lldp 319917776
#define brcd_aaa_protocol_pr 863669969
#define brcd_aaa_protocol_spanning 348046495
#define brcd_aaa_qos 8
#define brcd_aaa_radius 1
#define brcd_aaa_radius_server 1517669916
#define brcd_aaa_rbridge_id 23403572
#define brcd_aaa_read_only 0
#define brcd_aaa_read_write 1
#define brcd_aaa_reject 1
#define brcd_aaa_reload 56
#define brcd_aaa_rename 42
#define brcd_aaa_retries 1723503617
#define brcd_aaa_rmon 58
#define brcd_aaa_role 1297556411
#define brcd_aaa_router 53
#define brcd_aaa_rule 661146188
#define brcd_aaa_running_config_cont 390043736
#define brcd_aaa_second 620489704
#define brcd_aaa_server_type 1232689207
#define brcd_aaa_service 855380710
#define brcd_aaa_sessions 972148830
#define brcd_aaa_sflow 10
#define brcd_aaa_snmp_server 27
#define brcd_aaa_spanning_tree_cont 1620404694
#define brcd_aaa_special_char 2118807737
#define brcd_aaa_ssh 14
#define brcd_aaa_start_stop 323140968
#define brcd_aaa_support 34
#define brcd_aaa_support_cl 1031928426
#define brcd_aaa_support_cp 468861773
#define brcd_aaa_switch_attributes 54
#define brcd_aaa_switch_role 1720813829
#define brcd_aaa_system_monitor 12
#define brcd_aaa_tacacs0x2b 2
#define brcd_aaa_tacacs_server 1602942299
#define brcd_aaa_telnet 13
#define brcd_aaa_tengigabitethernet_cont 2065977989
#define brcd_aaa_tengigabitethernet_leaf 1222034934
#define brcd_aaa_terminal 45
#define brcd_aaa_timeout 614210192
#define brcd_aaa_unlock 1383788173
#define brcd_aaa_upper 387913976
#define brcd_aaa_usb 57
#define brcd_aaa_user 1529217067
#define brcd_aaa_user_password 820414788
#define brcd_aaa_username 870658415
#define brcd_aaa_vCenter 66
#define brcd_aaa_vcs 44
#define brcd_aaa_vlan 7
#define brcd_aaa_vlan_cont 1360548878
#define brcd_aaa_vlan_leaf 1165248196
#define brcd_aaa_vnetwork 65
#define brcd_aaa_vrf 55

#ifdef __cplusplus
}
#endif

#endif

