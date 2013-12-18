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
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/L3sys/L3ssArpShowMessage.h"
#include "ClientInterface/L3sys/L3ssMessageDef.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "WyserEaGateway/GenericClientMessage.h"

namespace DcmNs
{

    L3ssArpShowMessage::L3ssArpShowMessage ()
        : DcmManagementInterfaceMessage (L3SSARPSHOW)
    {
    }

    L3ssArpShowMessage::~L3ssArpShowMessage ()
    {
    }

    void  L3ssArpShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeIpV4Address(&m_ip_addr, "ip_addr"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeString(&m_mac, "mac"));
        addSerializableAttribute (new AttributeUI32(&m_lastPrinted, "lastPrinted"));
		addSerializableAttribute (new AttributeString(&m_vrf_name, "vrf_name"));
    }

    void  L3ssArpShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  L3ssArpShowMessage::getIfName() const
    {
        return (m_ifName);
    }

	void L3ssArpShowMessage::setMac(const string &mac)
	{
		m_mac = mac;
	}
	string L3ssArpShowMessage::getMac() const
	{
		return (m_mac);
	}

    void  L3ssArpShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  L3ssArpShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  L3ssArpShowMessage::setIp_addr(const IpV4Address &ip_addr)
    {
        m_ip_addr  =  ip_addr;
    }

    IpV4Address  L3ssArpShowMessage::getIp_addr() const
    {
        return (m_ip_addr);
   	}

	void L3ssArpShowMessage::setVrf_name(const string &vrf_name)
	{
		m_vrf_name = vrf_name;
	}
	string L3ssArpShowMessage::getVrf_name() const
	{
		return (m_vrf_name);
	}

    const void *L3ssArpShowMessage::getCStructureForInputs ()
    {
        l3ss_show_in_msg_t  *msg = new l3ss_show_in_msg_t;
        string str_ipaddr;
        msg->cmd = m_cmdCode;

        strncpy(msg->vrf_name, m_vrf_name.c_str(), 256);
        msg->vrf_name[255] = '\0';

		if(msg->cmd == L3SS_ARP_SHOW_TE_IF_CONFIG ||
			msg->cmd == L3SS_ARP_SHOW_PO_IF_CONFIG || msg->cmd == L3SS_ARP_SHOW_VLAN_IF_CONFIG ||
			msg->cmd == L3SS_ARP_SHOW_GI_IF_CONFIG || msg->cmd == L3SS_ARP_SHOW_FO_IF_CONFIG 
		)
			strncpy((char *)(msg->if_name), (const char*)(m_ifName.c_str()), L3SS_DCM_IF_NAME_LEN);
		if(msg->cmd == L3SS_ARP_SHOW_MAC_CONFIG) {
			str_ipaddr = m_mac;
			strncpy(msg->mac, str_ipaddr.c_str(), 15);
			msg->mac[14] = '\0';	
		}
		if(msg->cmd == L3SS_ARP_SHOW_IP) {
            str_ipaddr = m_ip_addr.toString();
            strncpy(msg->ip_address, str_ipaddr.c_str(), 15);
            msg->ip_address[15] = '\0';
        }

        msg->last_printed = m_lastPrinted;

        return msg;
    }
    void L3ssArpShowMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }

    void L3ssArpShowMessage::copyAllBuffers (const PrismMessage 
            &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

    void
    L3ssArpShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        l3ss_show_out_arp_msg_t *msg = ((l3ss_show_out_arp_msg_t *)pOutputCStructure);

        if (msg == 0) {
            return;
        }

        int num_records = msg->num_records;
        switch (msg->record_type) {
            case L3SS_ARP_SHOW_TYPE: {
                addBuffer (L3SSARPSHOW, (((num_records - 1) > 0 )? (num_records - 1) : 0) * sizeof (route_arp_rec_t) +  sizeof (l3ss_show_out_arp_msg_t), pOutputCStructure, false);
                break;
            }
            default :
                break;
        }

        return;
    }

    void L3ssArpShowMessage::setLastPrinted(const UI32 &lastPrinted)
    {
        m_lastPrinted = lastPrinted;
    }



}


