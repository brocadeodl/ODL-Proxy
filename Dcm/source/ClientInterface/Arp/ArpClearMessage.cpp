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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Arp/ArpClearMessage.h"
#include "ClientInterface/Arp/ArpMessageDef.h"


namespace DcmNs
{

    ArpClearMessage::ArpClearMessage ()
        : DcmManagementInterfaceMessage (ARPCLEAR)
    {
    }

    ArpClearMessage::ArpClearMessage (const string &ifName,const UI32 &cmdCode, const IpV4Address &ip_addr, const string &mac_addr)
    : DcmManagementInterfaceMessage (ARPCLEAR),
		m_ifName    (ifName),
		m_cmdCode   (cmdCode),
		m_ip_addr	(ip_addr),
		m_mac_addr  (mac_addr)
    {
    }

    ArpClearMessage::~ArpClearMessage ()
    {
    }

    void  ArpClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeIpV4Address(&m_ip_addr, "ip_addr"));
		addSerializableAttribute (new AttributeString(&m_mac_addr,"mac_addr"));
    }

    void  ArpClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  ArpClearMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  ArpClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  ArpClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    void  ArpClearMessage::setIp_addr(const IpV4Address &ip_addr)
    {
        m_ip_addr  =  ip_addr;
    }

    IpV4Address  ArpClearMessage::getIp_addr() const
    {
        return (m_ip_addr);
    }

    void  ArpClearMessage::setMac_addr(const string &mac_addr)
    {
        m_mac_addr  =  mac_addr;
    }

    string  ArpClearMessage::getMac_addr() const
    {
        return (m_mac_addr);
    }

    const void *ArpClearMessage::getCStructureForInputs ()
    {
        arp_clear_msg_t  *msg = new arp_clear_msg_t;
		string str_ipaddr;
		msg->cmd = m_cmdCode;
    strncpy((char *)(msg->vrf_name), (const char*)(getPartitionName().c_str()), ARP_VRF_NAME_LEN);
		if(CLEAR_ARPS_VLAN_NOREFRESH || CLEAR_ARPS_VLAN || CLEAR_ARPS_TE_NOREFRESH ||
			CLEAR_ARPS_TE || CLEAR_ARPS_GI || CLEAR_ARPS_GI_NOREFRESH || CLEAR_ARPS_FO || CLEAR_ARPS_FO_NOREFRESH || CLEAR_ARPS_PO_NOREFRESH || CLEAR_ARPS_PO) {
			strncpy((char *)(msg->if_name), (const char*)(m_ifName.c_str()), ARP_DCM_IF_NAME_LEN);
			msg->if_name[ARP_DCM_IF_NAME_LEN - 1] = '\0';
		}
		if(CLEAR_ARPS_IP_NOREFRESH || CLEAR_ARPS_IP) {
			str_ipaddr = m_ip_addr.toString();
			strncpy(msg->ip_address, str_ipaddr.c_str(), 15);
			msg->ip_address[15] = '\0';
		}
		if(CLEAR_ARPS_MAC_NOREFRESH || CLEAR_ARPS_MAC) {
			strncpy((char *)(msg->mac_address), (const char*)(m_mac_addr.c_str()), ARP_MAC_ADDRESS_LEN);
			msg->mac_address[ARP_MAC_ADDRESS_LEN-1] = '\0';
		}
        return msg;
    }

	void 
    ArpClearMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		return;
	}
}
