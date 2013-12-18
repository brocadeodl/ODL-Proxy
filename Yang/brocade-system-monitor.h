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

#ifndef BROCADE_SYSTEM_MONITOR_H
#define BROCADE_SYSTEM_MONITOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#define system_monitor__ns 1036448278

#define system_monitor_LineCard 2100557176
#define system_monitor_MM 1028291256
#define system_monitor_SFM 412712490
#define system_monitor_action 571883887
#define system_monitor_alert 268671234
#define system_monitor_cid_card 1094540810
#define system_monitor_compact_flash 376037425
#define system_monitor_domain_name 1601118986
#define system_monitor_down_threshold 175820628
#define system_monitor_email 971836852
#define system_monitor_email_list 561179076
#define system_monitor_enable 448570367
#define system_monitor_fan 1957768428
#define system_monitor_fru 1919016017
#define system_monitor_host_ip 1659215920
#define system_monitor_interface 221572658
#define system_monitor_marginal_threshold 1594771794
#define system_monitor_power 2065308511
#define system_monitor_relay 1978853656
#define system_monitor_security 1612212717
#define system_monitor_sfp 1476860116
#define system_monitor_state 630973766
#define system_monitor_system_monitor 1948952383
#define system_monitor_system_monitor_mail 249806039
#define system_monitor__bm_sfp_supported_state_all (1 << 6)
#define system_monitor__bm_sfp_supported_state_faulty (1 << 4)
#define system_monitor__bm_sfp_supported_state_inserted (1 << 1)
#define system_monitor__bm_sfp_supported_state_none (1 << 5)
#define system_monitor__bm_sfp_supported_state_removed (1 << 0)
#define system_monitor__bm_supported_actions_all (1 << 1)
#define system_monitor__bm_supported_actions_email (1 << 2)
#define system_monitor__bm_supported_actions_none (1 << 0)
#define system_monitor__bm_supported_actions_raslog (1 << 3)
#define system_monitor__bm_supported_state_all (1 << 6)
#define system_monitor__bm_supported_state_faulty (1 << 4)
#define system_monitor__bm_supported_state_inserted (1 << 1)
#define system_monitor__bm_supported_state_none (1 << 5)
#define system_monitor__bm_supported_state_on (1 << 2)
#define system_monitor__bm_supported_state_removed (1 << 0)
#define system_monitor__wysereaconfigurationid_relay_ip_server "relay-ip-server"
#define system_monitor__wysereaconfigurationid_smsetCompactFlashThreshold "smsetCompactFlashThreshold"
#define system_monitor__wysereaconfigurationid_smsetFanAlert "smsetFanAlert"
#define system_monitor__wysereaconfigurationid_smsetFanThreshold "smsetFanThreshold"
#define system_monitor__wysereaconfigurationid_smsetLineCardAlert "smsetLineCardAlert"
#define system_monitor__wysereaconfigurationid_smsetLineCardThreshold "smsetLineCardThreshold"
#define system_monitor__wysereaconfigurationid_smsetMMThreshold "smsetMMThreshold"
#define system_monitor__wysereaconfigurationid_smsetPowerAlert "smsetPowerAlert"
#define system_monitor__wysereaconfigurationid_smsetPowerThreshold "smsetPowerThreshold"
#define system_monitor__wysereaconfigurationid_smsetSfmThreshold "smsetSfmThreshold"
#define system_monitor__wysereaconfigurationid_smsetSfpAlert "smsetSfpAlert"
#define system_monitor__wysereaconfigurationid_smsetTempThreshold "smsetTempThreshold"
#define system_monitor__wysereaconfigurationid_smsetWwnCardAlert "smsetWwnCardAlert"
#define system_monitor__wysereaconfigurationid_smsetWwnCardThreshold "smsetWwnCardThreshold"
#define system_monitor__wysereaconfigurationid_system_monitor_email "system-monitor-email"
#define system_monitor__wysereaconfigurationid_system_monitor_int_email "system-monitor-int-email"
#define system_monitor__wysereaconfigurationid_system_monitor_mail_enableflag "system-monitor-mail-enableflag"
#define system_monitor__wysereaconfigurationid_system_monitor_mail_int_enableflag "system-monitor-mail-int-enableflag"
#define system_monitor__wysereaconfigurationid_system_monitor_mail_sec_enableflag "system-monitor-mail-sec-enableflag"
#define system_monitor__wysereaconfigurationid_system_monitor_mail_sfp_enableflag "system-monitor-mail-sfp-enableflag"
#define system_monitor__wysereaconfigurationid_system_monitor_sec_email "system-monitor-sec-email"
#define system_monitor__wysereaconfigurationid_system_monitor_sfp_email "system-monitor-sfp-email"
#define system_monitor_temp 72088356
#define system_monitor_threshold 1161930343

#ifdef __cplusplus
}
#endif

#endif

