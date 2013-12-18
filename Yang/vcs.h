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


#ifndef _VCS_H_
#define _VCS_H_


#include "brocade-zone.h"
#include "brocade-vswitch.h"
#include "brocade-vcs.h"
#include "brocade-aaa.h"
#include "brocade-interface.h"
#include "brocade-mac-access-list.h" 
#include "brocade-aaa.h"
#include "brocade-aaa-ext.h"
#include "brocade-cee-map.h"
#include "brocade-clock.h"
#include "brocade-chassis.h"
#include "brocade-common-def.h"
#include "brocade-diagnostics.h"
#include "brocade-distributedlog.h"
#include "brocade-dot1x.h"
#include "brocade-fabric-service.h"
#include "brocade-fc-auth.h"
#include "brocade-fcoe-ext.h"
#include "brocade-fcoe.h"
#include "brocade-firmware-ext.h"
#include "brocade-firmware.h"
#include "brocade-igmp-snooping.h"
#include "brocade-interface-ext.h"
#include "brocade-ip-access-list.h"
#include "brocade-ipv6-access-list.h"
#include "brocade-ip-administration.h"
#include "brocade-ip-forward.h"
#include "brocade-lacp.h"
#include "brocade-lag.h"
#include "brocade-license.h"
#include "brocade-lldp.h"
#include "brocade-cdp.h"
#include "brocade-mac-address-table.h"
#include "brocade-nameserver.h"
#include "brocade-ntp.h"
#include "brocade-port-profile-ext.h"
#include "brocade-port-profile.h"
#include "brocade-qos.h"
#include "brocade-ras.h"
#include "brocade-ras-ext.h"
#include "brocade-rbridge.h"
#include "brocade-rmon.h"
#include "brocade-sflow.h"
#include "brocade-snmp.h"
#include "brocade-span.h"
#include "brocade-system-monitor.h"
#include "brocade-system-monitor-ext.h"
#include "brocade-terminal.h"
#include "brocade-trilloam.h"
#include "brocade-vlan.h"
#include "brocade-vswitch.h"
#include "brocade-xstp-ext.h"
#include "brocade-xstp.h"
#include "brocade-hidden-cli.h"
#include "brocade-system.h"
#include "brocade-eld.h"
#include "brocade-ag.h"
#include "brocade-arp.h"
#include "brocade-igmp.h"
#include "brocade-intf-loopback.h"
#include "brocade-ip-config.h"
#include "brocade-ip-policy.h"
#include "brocade-ospf.h"
#include "brocade-pim.h"
#include "brocade-policer.h"
#include "brocade-rtm.h"
#include "brocade-vrrp.h"
#include "brocade-bprate-limit.h"
#include "brocade-beacon.h"
#include "brocade-linecard-management.h"
#include "brocade-ha.h"
#include "brocade-threshold-monitor.h"
#include "brocade-threshold-monitor-ext.h"
#include "brocade-bum-storm-control.h"
#include "brocade-vrf.h"
#include "brocade-bgp.h"
#include "brocade-udld.h"
#include "brocade-hardware.h"
#include "brocade-dhcp.h"
#endif

