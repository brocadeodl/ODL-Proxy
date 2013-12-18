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

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Arp/ArpShowMessage.h"
#include "ClientInterface/Arp/ArpMessageDef.h"

namespace DcmNs
{
    ArpShowMessage::ArpShowMessage ()
        : DcmManagementInterfaceMessage (ARPSHOW)
    {
    }

    ArpShowMessage::~ArpShowMessage ()
    {
    }


    void  ArpShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmd,"cmd"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeBool(&m_summary, "summary"));	
		addSerializableAttribute (new AttributeBool(&m_dynamic, "dynamic"));
		addSerializableAttribute (new AttributeBool(&m_static_arp, "static_arp"));
		addSerializableAttribute (new AttributeString(&m_mac, "mac"));
		addSerializableAttribute (new AttributeIpV4Address(&m_ip_addr, "ip_addr"));
		addSerializableAttribute (new AttributeSI32(&m_skip_count,"skip_count"));
		addSerializableAttribute (new AttributeString(&m_ip_mask, "ip_mask"));
		addSerializableAttribute (new AttributeSI32(&m_last_printed, "last_printed"));
		addSerializableAttribute (new AttributeSI32(&m_mappedId,"mappedId"));
    }


    void  ArpShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  ArpShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  ArpShowMessage::setCmdCode(const UI32 &cmd)
    {
        m_cmd  =  cmd;
    }

    UI32  ArpShowMessage::getCmdCode() const
    {
        return (m_cmd);
    }
    void  ArpShowMessage::setIp_addr(const IpV4Address &ip_addr)
    {
        m_ip_addr  =  ip_addr;
    }

    IpV4Address  ArpShowMessage::getIp_addr() const
    {
        return (m_ip_addr);
    }
	void ArpShowMessage::setMac(const string &mac)
	{
		m_mac = mac;
	}
	string ArpShowMessage::getMac() const
	{
		return (m_mac);
	}
	void ArpShowMessage::setSummary (const bool &summary)
	{
		m_summary = summary;
	}
	bool ArpShowMessage::getSummary	() const
	{
		return (m_summary);
	}
	void ArpShowMessage::setStatic_arp(const bool &static_arp)
	{
		m_static_arp = static_arp;
	}
	bool ArpShowMessage::getStatic_arp() const
	{
		return (m_static_arp);
	}
	void ArpShowMessage::setDynamic(const bool &dynamic)
	{
		m_dynamic = dynamic;
	}
	bool ArpShowMessage::getDynamic( ) const
	{
		return (m_dynamic);
	}
	void ArpShowMessage::setSkipCount(const int &skip_count)
	{
		m_skip_count = skip_count;
	}
	int ArpShowMessage::getSkipCount( ) const
	{
		return (m_skip_count);
	}
	void ArpShowMessage::setIpMask(const string &ip_mask)
	{
		m_ip_mask = ip_mask;
	}
	string ArpShowMessage::getIpMask() const
	{
		return (m_ip_mask);
	}
    void ArpShowMessage::setLastPrinted(const int &last_printed)
    {
        m_last_printed = last_printed;
    }
    int ArpShowMessage::getLastPrinted( ) const
    {
        return (m_last_printed);
    }
    void ArpShowMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }

    const void *ArpShowMessage::getCStructureForInputs ()
    {
        arp_show_in_msg_t  *msg = new arp_show_in_msg_t;
		string str_ipaddr;
		msg->cmd = m_cmd;
	  strncpy((char *)(msg->vrf_name), (const char*)(getPartitionName().c_str()), ARP_VRF_NAME_LEN);
		msg->static_arps = m_static_arp;
		msg->dynamic = m_dynamic;
		msg->summary = m_summary;	
		msg->skip_count = m_skip_count;
		msg->last_printed = m_last_printed;
		if(msg->cmd == SHOW_ARP_TE_IF_CONFIG ||
			msg->cmd == SHOW_ARP_PO_IF_CONFIG || msg->cmd == SHOW_ARP_VLAN_IF_CONFIG ||
			msg->cmd == SHOW_ARP_GI_IF_CONFIG || msg->cmd == SHOW_ARP_FO_IF_CONFIG 
		)
			strncpy((char *)(msg->if_name), (const char*)(m_ifName.c_str()), ARP_DCM_IF_NAME_LEN);
		if(msg->cmd == SHOW_ARP_IP_CONFIG) {
				str_ipaddr = m_ip_addr.toString();
				strncpy(msg->ip, str_ipaddr.c_str(), 15);
				msg->ip[15] = '\0';
		}
		if(msg->cmd == SHOW_ARP_MAC_CONFIG) {
			str_ipaddr = m_mac;
			strncpy(msg->mac, str_ipaddr.c_str(), 15);
			msg->mac[14] = '\0';	
		}

		if(msg->cmd == SHOW_ARP_IP_MASK) {
		
			str_ipaddr = m_ip_mask;
			strcpy(msg->ip_mask, str_ipaddr.c_str());
		}


        return msg;
    }

	void 
    ArpShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		arp_show_out_msg_t *msg = ((arp_show_out_msg_t *)pOutputCStructure);

        if (msg == 0) {
            return;
        }
        arp_mgmt_buffer_num_t  buff_num = {{0}};
        buff_num.buff.mapped_id = m_mappedId;
        buff_num.buff.tag = ARPSHOW;

		addBuffer (buff_num.num, msg->size, pOutputCStructure, false);

		return;
	}

}
