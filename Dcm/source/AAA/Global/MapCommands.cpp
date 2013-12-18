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

/***************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : kghanta                                                     *
 **************************************************************************/

#include "MapCommands.h"
#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{
	map< int, vector<string> > MapCommands::cmdRuleCmds;
	map< int, vector<string> > MapCommands::dataRuleCmds;
	//map< int, vector<string> > MapCommands::showCmds;
	vector<string> MapCommands::defaultCmds;
	//TODO: Need to get the commands from the static file.
	
	void MapCommands::setDataRuleCmd(const int &cmdKey, const vector<string> &cmd)
	{
		dataRuleCmds[cmdKey] = cmd;
	}

	vector<string> MapCommands::getDataRuleCmd(int &cmdKey)
	{
		return (dataRuleCmds[cmdKey]);
	}
	
	void MapCommands::setCommandRuleCmd(const int &cmdKey, const vector<string> &cmd) 
	{
		cmdRuleCmds[cmdKey] = cmd;
	}
	
	vector<string> MapCommands::getCommandRuleCmd(int &cmdKey)
	{
		return (cmdRuleCmds[cmdKey]);
	}

	map< int, vector<string> > MapCommands::getCommandRuleMap()
	{
		return (cmdRuleCmds);
	}
#if 0	
	void MapCommands::setShowCmd(const int &cmdKey, const string &cmd) 
	{
		showCmds[cmdKey] = cmd;
	}

	string MapCommands::getShowCmd(int &cmdKey)
	{
		return (showCmds[cmdKey]);
	}
#endif
	void MapCommands::setupDefaultCmds()
	{
		defaultCmds.push_back("autowizard");
		defaultCmds.push_back("complete-on-space");
		defaultCmds.push_back("debug");
		defaultCmds.push_back("display-level");
		defaultCmds.push_back("chassis");
		defaultCmds.push_back("exit");
		defaultCmds.push_back("file");
		defaultCmds.push_back("foscmd");
		defaultCmds.push_back("help");
		defaultCmds.push_back("history");
		defaultCmds.push_back("no history");
		defaultCmds.push_back("id");
		defaultCmds.push_back("ignore-leading-space");
		defaultCmds.push_back("job");
		defaultCmds.push_back("logout");
		defaultCmds.push_back("output-file");
		defaultCmds.push_back("paginate");
		defaultCmds.push_back("quit");
		defaultCmds.push_back("rename");
		defaultCmds.push_back("send");
		defaultCmds.push_back("undebug");
		defaultCmds.push_back("who");
		defaultCmds.push_back("write");
		defaultCmds.push_back("abort");
		defaultCmds.push_back("do");
		defaultCmds.push_back("end");
		defaultCmds.push_back("insert");
		defaultCmds.push_back("load");
		defaultCmds.push_back("move");
		defaultCmds.push_back("pwd");
		defaultCmds.push_back("rollback");
		defaultCmds.push_back("save");
		defaultCmds.push_back("show");
		defaultCmds.push_back("top");
		defaultCmds.push_back("cipherset");
		defaultCmds.push_back("show cipherset");
		defaultCmds.push_back("no cipherset");
		defaultCmds.push_back("fips");
		defaultCmds.push_back("no fips");
		defaultCmds.push_back("capture");
		defaultCmds.push_back("no capture");
		defaultCmds.push_back("show capture");
		defaultCmds.push_back("alias");
		defaultCmds.push_back("no alias");
		defaultCmds.push_back("slot");
		defaultCmds.push_back("logical-chassis");
		defaultCmds.push_back("no route-map");
		defaultCmds.push_back("no config");
	}

	vector<string> MapCommands::getDefaultCmds()
	{
			return defaultCmds;
	}
	void MapCommands::unsetDefaultCmds()
	{
		defaultCmds.erase(defaultCmds.begin(), defaultCmds.end());
	}
	void MapCommands::setupCmdMaps()
	{
		//Setting up Command Rule Maps
		//As the items in the vector are not many, I am not using dynamic memory allocation with "new".
		int i = 0;	
		vector<string> cmds[MAX_NO_CMDS];

        trace(TRACE_LEVEL_INFO, "MapCommands::setupCmdMaps Enter");
        ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType ();
		
		cmds[i].push_back("clock");
		cmds[i].push_back("show clock");
		cmds[i].push_back("no clock");
		setCommandRuleCmd(brcd_aaa_clock, cmds[i]);
		i++;

		cmds[i].push_back("diag");
		cmds[i].push_back("show diag");
		setCommandRuleCmd(brcd_aaa_diag, cmds[i]);
		i++;
		
		cmds[i].push_back("terminal");
		cmds[i].push_back("no terminal");
		setCommandRuleCmd(brcd_aaa_terminal, cmds[i]);
		i++;

		cmds[i].push_back("dir");
		setCommandRuleCmd(brcd_aaa_dir, cmds[i]);
		i++;

		cmds[i].push_back("df");
		setCommandRuleCmd(brcd_aaa_df, cmds[i]);
		i++;

		cmds[i].push_back("prompt1");
		setCommandRuleCmd(brcd_aaa_prompt1, cmds[i]);
		i++;

		cmds[i].push_back("prompt2");
		setCommandRuleCmd(brcd_aaa_prompt2, cmds[i]);
		i++;

		cmds[i].push_back("delete");
		setCommandRuleCmd(brcd_aaa_delete, cmds[i]);
		i++;

		cmds[i].push_back("rename");
		setCommandRuleCmd(brcd_aaa_rename, cmds[i]); 
		i++;

		cmds[i].push_back("configure");
		setCommandRuleCmd(brcd_aaa_configure, cmds[i]);
		i++;

		cmds[i].push_back("vcs");
		cmds[i].push_back("show vcs");
		cmds[i].push_back("no vcs");
		setCommandRuleCmd(brcd_aaa_vcs, cmds[i]);
		i++;

		//Handling show operations for configuration data.
		cmds[i].push_back("dot1x");
		cmds[i].push_back("show dot1x");
		cmds[i].push_back("no dot1x");
		setCommandRuleCmd(brcd_aaa_dot1x, cmds[i]);
		i++;

		cmds[i].push_back("fcoe");
		cmds[i].push_back("show fcoe");
		setCommandRuleCmd(brcd_aaa_command_fcoe, cmds[i]);
		i++;

		cmds[i].push_back("show logging");
		setCommandRuleCmd(brcd_aaa_logging, cmds[i]);
		i++;

		cmds[i].push_back("show mac-address-table");
		cmds[i].push_back("mac-address-table");
		cmds[i].push_back("no mac-address-table");
		setCommandRuleCmd(brcd_aaa_mac_address_table, cmds[i]); 
		i++;

		cmds[i].push_back("mac-rebalance");
		setCommandRuleCmd(brcd_aaa_mac_rebalance, cmds[i]);
		i++;

		cmds[i].push_back("cidrecov");
		setCommandRuleCmd(brcd_aaa_cidrecov, cmds[i]); 
		i++;
		
		cmds[i].push_back("debug1");
		setCommandRuleCmd(brcd_aaa_debug1, cmds[i]); 
		i++;

		cmds[i].push_back("lccli");
		setCommandRuleCmd(brcd_aaa_lccli, cmds[i]); 
		i++;

		cmds[i].push_back("oscmd");
		setCommandRuleCmd(brcd_aaa_oscmd, cmds[i]); 
		i++;
		
		cmds[i].push_back("resequence");
		cmds[i].push_back("show mac");
		setCommandRuleCmd(brcd_aaa_mac, cmds[i]);
		i++;

		cmds[i].push_back("show sflow");
		setCommandRuleCmd(brcd_aaa_sflow, cmds[i]); 
		i++;

		cmds[i].push_back("show ntp");
		setCommandRuleCmd(brcd_aaa_ntp, cmds[i]); 
		i++;

		cmds[i].push_back("show monitor");
		setCommandRuleCmd(brcd_aaa_monitor, cmds[i]); 
		i++;

		cmds[i].push_back("show qos");
		setCommandRuleCmd(brcd_aaa_qos, cmds[i]); 
		i++;

		cmds[i].push_back("show vlan");
		cmds[i].push_back("no vlan");
		setCommandRuleCmd(brcd_aaa_vlan, cmds[i]);
		i++;

		cmds[i].push_back("show fabric");
		setCommandRuleCmd(brcd_aaa_fabric, cmds[i]);
		i++;

		cmds[i].push_back("show support");
		setCommandRuleCmd(brcd_aaa_support, cmds[i]);
		i++;

		cmds[i].push_back("license");
		cmds[i].push_back("show license");
		setCommandRuleCmd(brcd_aaa_license, cmds[i]);
		i++;

		cmds[i].push_back("firmware");
		cmds[i].push_back("show firmwaredownloadstatus");
		setCommandRuleCmd(brcd_aaa_firmware, cmds[i]);
		i++;

		//New commands	
		cmds[i].push_back("show cee");
		setCommandRuleCmd(brcd_aaa_cee_map, cmds[i]);
		i++;

		cmds[i].push_back("show rmon");
		setCommandRuleCmd(brcd_aaa_rmon, cmds[i]);
		i++;

		cmds[i].push_back("show ip");
		cmds[i].push_back("ip");
		cmds[i].push_back("no ip");
		setCommandRuleCmd(brcd_aaa_ip, cmds[i]);
		i++;

		cmds[i].push_back("show lacp");
		setCommandRuleCmd(brcd_aaa_lacp, cmds[i]);
		i++;

		cmds[i].push_back("reload");
		setCommandRuleCmd(brcd_aaa_reload, cmds[i]);
		i++;

		cmds[i].push_back("fastboot");
		setCommandRuleCmd(brcd_aaa_fastboot, cmds[i]);
		i++;

		cmds[i].push_back("usb");
		setCommandRuleCmd(brcd_aaa_usb, cmds[i]);
		i++;

		cmds[i].push_back("unlock");
		setCommandRuleCmd(brcd_aaa_command_unlock, cmds[i]);
		i++;

		cmds[i].push_back("debug");
		cmds[i].push_back("show debug");
		cmds[i].push_back("no debug");
		setCommandRuleCmd(brcd_aaa_debug, cmds[i]);
		i++;

		cmds[i].push_back("show port-profile");
		setCommandRuleCmd(brcd_aaa_port_profile, cmds[i]);
		i++;

		cmds[i].push_back("no-operation");
		setCommandRuleCmd(brcd_aaa_no_operation, cmds[i]);
		i++;

		// Hercules command additions.
		cmds[i].push_back("vnetwork");
		cmds[i].push_back("show vnetwork");
		setCommandRuleCmd(brcd_aaa_vnetwork, cmds[i]);
		i++;

		//Setting up Data Rule Maps
		cmds[i].push_back("/clock-set-datetime/clock");
		cmds[i].push_back("/clock-set-timezone/clock");
		cmds[i].push_back("/no/clock");
		setDataRuleCmd(brcd_aaa_clock, cmds[i]);
		i++;

		cmds[i].push_back("/diag");
		setDataRuleCmd(brcd_aaa_diag, cmds[i]);
		i++;

		cmds[i].push_back("/terminal");
		setDataRuleCmd(brcd_aaa_terminal, cmds[i]);
		i++;

		cmds[i].push_back("/license");
		setDataRuleCmd(brcd_aaa_license, cmds[i]);
		i++;

		cmds[i].push_back("/firmware");
		setDataRuleCmd(brcd_aaa_firmware, cmds[i]);
		i++;

		cmds[i].push_back("/dot1x");
		setDataRuleCmd(brcd_aaa_dot1x, cmds[i]);
		i++;

		cmds[i].push_back("/vcs");
		setDataRuleCmd(brcd_aaa_vcs, cmds[i]);
		i++;

		cmds[i].push_back("/mac-address-table");
		setDataRuleCmd(brcd_aaa_mac_address_table, cmds[i]); 
		i++;

		cmds[i].push_back("/vlan");
		setDataRuleCmd(brcd_aaa_vlan, cmds[i]);
		i++;

		cmds[i].push_back("/qos");
		setDataRuleCmd(brcd_aaa_qos, cmds[i]);
		i++;

		cmds[i].push_back("/mac");
		setDataRuleCmd(brcd_aaa_mac, cmds[i]);
		i++;

		cmds[i].push_back("/sflow");
		setDataRuleCmd(brcd_aaa_sflow, cmds[i]); 
		i++;
		
		cmds[i].push_back("/monitor");
		setDataRuleCmd(brcd_aaa_monitor, cmds[i]); 
		i++;
		
		cmds[i].push_back("/fcoe");
		setDataRuleCmd(brcd_aaa_command_fcoe, cmds[i]);
		i++;

		cmds[i].push_back("/aaa-config/aaa");
		setDataRuleCmd(brcd_aaa_command_aaa,  cmds[i]);
		i++;

		cmds[i].push_back("/radius-server");
		setDataRuleCmd(brcd_aaa_command_radius_server, cmds[i]);
		i++;

		cmds[i].push_back("/tacacs-server");
		setDataRuleCmd(brcd_aaa_command_tacacs_server, cmds[i]);
		i++;

		cmds[i].push_back("/ldap-server");
		setDataRuleCmd(brcd_aaa_command_ldap_server, cmds[i]);
		i++;

		cmds[i].push_back("/snmp-server");
		setDataRuleCmd(brcd_aaa_snmp_server, cmds[i]); 
		i++;

		cmds[i].push_back("/password-attributes");
		setDataRuleCmd(brcd_aaa_command_password_attributes, cmds[i]);
		i++;

		cmds[i].push_back("/alias-config");
		setDataRuleCmd(brcd_aaa_command_alias_config, cmds[i]);
		i++;

		cmds[i].push_back("/username");
		setDataRuleCmd(brcd_aaa_command_username, cmds[i]);
		i++;

		cmds[i].push_back("/role");
		setDataRuleCmd(brcd_aaa_command_role, cmds[i]);
		i++;

		cmds[i].push_back("/rule");
		setDataRuleCmd(brcd_aaa_command_rule, cmds[i]);
		i++;

		cmds[i].push_back("/banner");
		setDataRuleCmd(brcd_aaa_command_banner, cmds[i]);
		i++;

		cmds[i].push_back("/service");
		setDataRuleCmd(brcd_aaa_command_service, cmds[i]);
		i++;

		cmds[i].push_back("/ntp");
		setDataRuleCmd(brcd_aaa_ntp, cmds[i]);
		i++;

		cmds[i].push_back("/fabric");
		setDataRuleCmd(brcd_aaa_fabric, cmds[i]);
		i++;

		cmds[i].push_back("/support");
		setDataRuleCmd(brcd_aaa_support, cmds[i]);
		i++;

		cmds[i].push_back("/logging");
		setDataRuleCmd(brcd_aaa_logging, cmds[i]);
		i++;

		// Adding the new commands
		cmds[i].push_back("/cee-map");
		setDataRuleCmd(brcd_aaa_cee_map, cmds[i]);
		i++;

		cmds[i].push_back("/ip");
		setDataRuleCmd(brcd_aaa_ip, cmds[i]);
		i++;

		cmds[i].push_back("/ipv6");
		setDataRuleCmd(brcd_aaa_ipv6, cmds[i]);
		i++;

		cmds[i].push_back("/lacp");
		setDataRuleCmd(brcd_aaa_lacp, cmds[i]);
		i++;

		cmds[i].push_back("/rmon");
		setDataRuleCmd(brcd_aaa_rmon, cmds[i]);
		i++;

		cmds[i].push_back("/system/switch-attributes");
		setDataRuleCmd(brcd_aaa_switch_attributes, cmds[i]);
		i++;

		cmds[i].push_back("/reboot/fastboot");
		setDataRuleCmd(brcd_aaa_fastboot, cmds[i]);
		i++;

		cmds[i].push_back("/system/usb");
		setDataRuleCmd(brcd_aaa_usb, cmds[i]);
		i++;

		cmds[i].push_back("/user/unlock");
		setDataRuleCmd(brcd_aaa_command_unlock, cmds[i]);
		i++;

		cmds[i].push_back("/port-profile");
		cmds[i].push_back("/port-profile-global");
		setDataRuleCmd(brcd_aaa_port_profile, cmds[i]);
		i++;
		
		cmds[i].push_back("/debug");
		setDataRuleCmd(brcd_aaa_debug, cmds[i]);
		i++;

		// Hercules data rules.
		cmds[i].push_back("/vCenter");
		setDataRuleCmd(brcd_aaa_vCenter, cmds[i]);
		i++;

		cmds[i].push_back("/vnetwork");
		setDataRuleCmd(brcd_aaa_vnetwork, cmds[i]);
		i++;

		if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE == vcsMode) {

            // SA mode - filter-change-update-delay
			cmds[i].push_back("/hide-filter-change-update-delay-holder/filter-change-update-delay");
			setDataRuleCmd(brcd_aaa_filter_change_update_delay, cmds[i]);
			i++;

			// SA mode - interface 
			cmds[i].push_back("/interface");
			setDataRuleCmd(brcd_aaa_interface, cmds[i]);
			i++;

			// SA mode - management
			cmds[i].push_back("/management");
			setDataRuleCmd(brcd_aaa_management, cmds[i]);
			i++;

			// SA mode - protocol
			cmds[i].push_back("/protocol");
			setDataRuleCmd(brcd_aaa_protocol, cmds[i]);
			i++;

			// SA mode - ssh
			cmds[i].push_back("/ssh-sa/ssh");
			setDataRuleCmd(brcd_aaa_ssh, cmds[i]);
			i++;

			// SA mode - switch-attributes
			cmds[i].push_back("/system/switch-attributes");
			setDataRuleCmd(brcd_aaa_switch_attributes, cmds[i]);
			i++;

			// SA mode - system-monitor
			cmds[i].push_back("/system-monitor");
			setDataRuleCmd(brcd_aaa_system_monitor, cmds[i]);
			i++;

			// SA mode - telnet 
			cmds[i].push_back("/telnet-sa/telnet");
			setDataRuleCmd(brcd_aaa_telnet, cmds[i]);
			i++;

		} else if ((VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == vcsMode) ||
					(VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == vcsMode)) {

			// FC/MC mode - system-monitor
			cmds[i].push_back("/rbridge-id/system-monitor");
			setDataRuleCmd(brcd_aaa_system_monitor, cmds[i]);
			i++;

			// FC/MC mode - filter-change-update-delay
			cmds[i].push_back("/rbridge-id/filter-change-update-delay");
			setDataRuleCmd(brcd_aaa_filter_change_update_delay, cmds[i]);
			i++;

			// FC/MC mode - fabric
			cmds[i].push_back("/rbridge-id/fabric");
			setDataRuleCmd(brcd_aaa_fabric, cmds[i]);
			i++;

			// FC/MC mode - interface 
			cmds[i].push_back("/rbridge-id/interface");
			setDataRuleCmd(brcd_aaa_interface, cmds[i]);
			i++;

			// FC/MC mode - management
			cmds[i].push_back("/rbridge-id/management");
			setDataRuleCmd(brcd_aaa_management, cmds[i]);
			i++;
	
			// FC/MC mode - protocol
			cmds[i].push_back("/rbridge-id/protocol");
			setDataRuleCmd(brcd_aaa_protocol, cmds[i]);
			i++;
	
			// FC/MC mode - switch-attributes
			cmds[i].push_back("/rbridge-id/switch-attributes");
			setDataRuleCmd(brcd_aaa_switch_attributes, cmds[i]);
			i++;

			// FC/MC mode - ssh
			cmds[i].push_back("/rbridge-id/ssh");
			setDataRuleCmd(brcd_aaa_ssh, cmds[i]);
			i++;

			// FC/MC mode - telnet 
			cmds[i].push_back("/rbridge-id/telnet");
			setDataRuleCmd(brcd_aaa_telnet, cmds[i]);
			i++;

			// FC/MC mode - vrf
			cmds[i].push_back("/rbridge-id/vrf");
			setDataRuleCmd(brcd_aaa_vrf, cmds[i]);
			i++;
		}

#if 0
		//Setting up show command Maps
		setShowCmd(brcd_aaa_clock, "clock");
		setShowCmd(brcd_aaa_diag, "diag");
		setShowCmd(brcd_aaa_firmware, "firmware");
		setShowCmd(brcd_aaa_license, "license");
		setShowCmd(brcd_aaa_terminal, "terminal");
		setShowCmd(brcd_aaa_vcs, "vcs");
		setShowCmd(brcd_aaa_dir, "dir");
		setShowCmd(brcd_aaa_delete, "delete");
		setShowCmd(brcd_aaa_rename, "rename"); 
		setShowCmd(brcd_aaa_dot1x, "dot1x");
		setShowCmd(brcd_aaa_mac_address_table, "mac-address-table"); 
		setShowCmd(brcd_aaa_vlan, "vlan");
		setShowCmd(brcd_aaa_qos, "qos");
		setShowCmd(brcd_aaa_mac, "mac");
		setShowCmd(brcd_aaa_sflow, "sflow"); 
		setShowCmd(brcd_aaa_monitor, "monitor"); 
		setShowCmd(brcd_aaa_rules_fcoe, "fcoe");
		setShowCmd(brcd_aaa_rules_aaa, "aaa");
		setShowCmd(brcd_aaa_rules_radius_server, "radius-server");
		setShowCmd(brcd_aaa_rules_tacacs_server, "tacacs-server");
		setShowCmd(brcd_aaa_snmp_server, "snmp-server"); 
		setShowCmd(brcd_aaa_rules_password_attributes, "password-attributes");
		setShowCmd(brcd_aaa_rules_username, "username");
		setShowCmd(brcd_aaa_rules_role, "role");
		setShowCmd(brcd_aaa_rules_rule, "rule");
		setShowCmd(brcd_aaa_rules_banner, "banner");
		setShowCmd(brcd_aaa_ntp, "ntp");
		setShowCmd(brcd_aaa_fabric, "fabric");
		setShowCmd(brcd_aaa_support, "support");
		setShowCmd(brcd_aaa_logging, "logging");
		setShowCmd(brcd_aaa_configure, "configure");
#endif
	trace(TRACE_LEVEL_INFO, "MapCommands::setupCmdMaps Exit");
	}
    
}
