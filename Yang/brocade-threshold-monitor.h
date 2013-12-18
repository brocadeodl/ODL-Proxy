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

#ifndef BROCADE_THRESHOLD_MONITOR_H
#define BROCADE_THRESHOLD_MONITOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#define threshold_monitor__ns 1567508442

#define threshold_monitor_10GLR 3
#define threshold_monitor_10GSR 2
#define threshold_monitor_10GUSR 4
#define threshold_monitor_1GLR 1
#define threshold_monitor_1GSR 0
#define threshold_monitor_CRCAlignErrors 1
#define threshold_monitor_Cpu 62221082
#define threshold_monitor_Current 3
#define threshold_monitor_Ethernet 0
#define threshold_monitor_IFG 3
#define threshold_monitor_Memory 248659231
#define threshold_monitor_MissingTerminationCharacter 0
#define threshold_monitor_QSFP 5
#define threshold_monitor_RXP 1
#define threshold_monitor_SymbolErrors 2
#define threshold_monitor_TXP 2
#define threshold_monitor_Temperature 0
#define threshold_monitor_Voltage 4
#define threshold_monitor_above 148921322
#define threshold_monitor_above_highthresh_action 952788882
#define threshold_monitor_above_lowthresh_action 1707951357
#define threshold_monitor_actions 2034878685
#define threshold_monitor_alert 268671234
#define threshold_monitor_apply 1468797760
#define threshold_monitor_area 1324291201
#define threshold_monitor_area_value 1495171487
#define threshold_monitor_below 22416181
#define threshold_monitor_below_highthresh_action 1293403580
#define threshold_monitor_below_lowthresh_action 241504007
#define threshold_monitor_buffer 1428421018
#define threshold_monitor_custom_monitoring 0
#define threshold_monitor_day 4
#define threshold_monitor_default_monitoring 1
#define threshold_monitor_high_limit 424484445
#define threshold_monitor_high_threshold 1000472314
#define threshold_monitor_hour 3
#define threshold_monitor_interface 221572658
#define threshold_monitor_limit 1553282341
#define threshold_monitor_login_violation 11
#define threshold_monitor_low_limit 1776497526
#define threshold_monitor_low_threshold 854789548
#define threshold_monitor_minute 2
#define threshold_monitor_none 0
#define threshold_monitor_pause 924676795
#define threshold_monitor_policy 1919655803
#define threshold_monitor_policy_name 356058760
#define threshold_monitor_poll 409005595
#define threshold_monitor_raslog 2
#define threshold_monitor_retry 1817854888
#define threshold_monitor_sec_above_highthresh_action 275630173
#define threshold_monitor_sec_below_highthresh_action 424291622
#define threshold_monitor_sec_below_lowthresh_action 1322809948
#define threshold_monitor_sec_buffer 1532892502
#define threshold_monitor_sec_high_threshold 2107166775
#define threshold_monitor_sec_low_threshold 1181957596
#define threshold_monitor_sec_area_value 1598274896
#define threshold_monitor_sec_policy_name 69660714
#define threshold_monitor_security 1612212717
#define threshold_monitor_sfp 1476860116
#define threshold_monitor_telnet_violation 0
#define threshold_monitor_threshold 1161930343
#define threshold_monitor_threshold_monitor 1660549775
#define threshold_monitor_threshold_monitor_hidden 1722564437
#define threshold_monitor__bm_int_supported_actions_all (1 << 1)
#define threshold_monitor__bm_int_supported_actions_email (1 << 2)
#define threshold_monitor__bm_int_supported_actions_fence (1 << 4)
#define threshold_monitor__bm_int_supported_actions_none (1 << 0)
#define threshold_monitor__bm_int_supported_actions_raslog (1 << 3)
#define threshold_monitor__bm_supported_actions_all (1 << 1)
#define threshold_monitor__bm_supported_actions_email (1 << 2)
#define threshold_monitor__bm_supported_actions_none (1 << 0)
#define threshold_monitor__bm_supported_actions_raslog (1 << 3)
#define threshold_monitor__wysereaconfigurationid_CpuMonitor "CpuMonitor"
#define threshold_monitor__wysereaconfigurationid_MemoryMonitor "MemoryMonitor"
#define threshold_monitor__wysereaconfigurationid_interfaceareamonitoring "interfaceareamonitoring"
#define threshold_monitor__wysereaconfigurationid_interfaceconfiguration "interfaceconfiguration"
#define threshold_monitor__wysereaconfigurationid_interfacemonitoring "interfacemonitoring"
#define threshold_monitor__wysereaconfigurationid_securityconfiguration "securityconfiguration"
#define threshold_monitor__wysereaconfigurationid_securitymonitoring "securitymonitoring"
#define threshold_monitor__wysereaconfigurationid_securitypolicymonitoring "securitypolicymonitoring"
#define threshold_monitor__wysereaconfigurationid_sfpareamonitoring "sfpareamonitoring"
#define threshold_monitor__wysereaconfigurationid_sfpconfiguration "sfpconfiguration"
#define threshold_monitor__wysereaconfigurationid_sfpmonitoring "sfpmonitoring"
#define threshold_monitor_timebase 993421222
#define threshold_monitor_timebase_value 68081600
#define threshold_monitor_type 2040883914

#ifdef __cplusplus
}
#endif

#endif

