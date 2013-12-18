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
 *   Author : payyappa                                                     *
 **************************************************************************/

#include "ClientInterface/Nsm/NsmClientCreateIntIpConfigSPMessage.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include <arpa/inet.h>

#define IP_ADDR_MASK_LEN 18
namespace DcmNs
{

    NsmClientCreateIntIpConfigSPMessage::NsmClientCreateIntIpConfigSPMessage ()
        : NsmVlanProvisionMessage (MSG_IP_INT)
    {
        m_ifId = 0;
        m_status = false;
        m_directed_broadcast = false;
    }

    NsmClientCreateIntIpConfigSPMessage::NsmClientCreateIntIpConfigSPMessage (const string &ifName,const UI32 &mtu,const string &ip_addr_mask,const UI32 &opcode, const UI32 &intf_type, const bool &sec, const UI32 &arp_ageing_timeout, const bool &proxy_arp)
        : NsmVlanProvisionMessage (MSG_IP_INT),
        m_ifName    (ifName),
        m_mtu    (mtu),
		m_ip_addr_mask  (ip_addr_mask),
		m_opcode	(opcode),
		m_intf_type	(intf_type),
		m_sec		(sec),
		m_arp_ageing_timeout	(arp_ageing_timeout),
		m_proxy_arp	(proxy_arp)
    {
        m_ifId = 0;
        m_status = false;
        m_directed_broadcast = false;
    }

    NsmClientCreateIntIpConfigSPMessage::~NsmClientCreateIntIpConfigSPMessage ()
    {
    }

    void  NsmClientCreateIntIpConfigSPMessage::setupAttributesForSerialization()
    {
        NsmVlanProvisionMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_ifId,"ifId"));
        addSerializableAttribute (new AttributeUI32(&m_mtu,"mtu"));
		addSerializableAttribute (new AttributeString(&m_ip_addr_mask,"ip_addr_mask"));
		addSerializableAttribute (new AttributeUI32(&m_opcode, "opcode"));
		addSerializableAttribute (new AttributeUI32(&m_intf_type, "intf_type"));
		addSerializableAttribute (new AttributeBool(&m_sec, "sec"));
		addSerializableAttribute (new AttributeBool(&m_ospfPassive, "ospf_passive"));
		addSerializableAttribute (new AttributeBool(&m_ospfIgnore, "ospf_ignore"));
		addSerializableAttribute (new AttributeBool(&m_status, "status"));
		addSerializableAttribute (new AttributeBool(&m_directed_broadcast, "directed_broadcast"));
		addSerializableAttribute (new AttributeUI32(&m_arp_ageing_timeout, "arp_ageing_timeout"));
		addSerializableAttribute (new AttributeBool(&m_proxy_arp, "proxy_arp"));
		addSerializableAttribute (new AttributeBool(&m_switchport, "switchport"));
    }

    void  NsmClientCreateIntIpConfigSPMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmClientCreateIntIpConfigSPMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmClientCreateIntIpConfigSPMessage::setIfId(const UI32 &id)
    {
        m_ifId  =  id;
    }

    UI32  NsmClientCreateIntIpConfigSPMessage::getIfId() const
    {
        return (m_ifId);
    }

    void  NsmClientCreateIntIpConfigSPMessage::setMtu(const UI32 &mtu)
    {
        m_mtu  =  mtu;
    }

    UI32  NsmClientCreateIntIpConfigSPMessage::getMtu() const
    {
        return (m_mtu);
    }

    void NsmClientCreateIntIpConfigSPMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode = opcode;
    }

    UI32  NsmClientCreateIntIpConfigSPMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void NsmClientCreateIntIpConfigSPMessage::setIntfType(const UI32 &intf_type)
    {
        m_intf_type = intf_type;
    }

    UI32  NsmClientCreateIntIpConfigSPMessage::getIntfType() const
    {
        return (m_intf_type);
    }

    void  NsmClientCreateIntIpConfigSPMessage::setIp_addr_mask(const string &ip_addr_mask)
    {
        m_ip_addr_mask  =  ip_addr_mask;
    }

    string  NsmClientCreateIntIpConfigSPMessage::getIp_addr_mask() const
    {
        return (m_ip_addr_mask);
    }

	void NsmClientCreateIntIpConfigSPMessage::setSec(const bool &sec)
    {
        m_sec = sec;
    }

    bool NsmClientCreateIntIpConfigSPMessage::getSec() const
    {
        return (m_sec);
    }
    void NsmClientCreateIntIpConfigSPMessage::setOspfPassive(const bool &ospf_passive)
    {
        m_ospfPassive = ospf_passive;
    }

    bool NsmClientCreateIntIpConfigSPMessage::getOspfPassive() const 
    {
        return (m_ospfPassive);
    }
	
    void NsmClientCreateIntIpConfigSPMessage::setOspfIgnore(const bool &ospf_ignore)
    {
        m_ospfIgnore = ospf_ignore;
    }

    bool NsmClientCreateIntIpConfigSPMessage::getOspfIgnore() const 
    {
        return (m_ospfIgnore);
    }

	void NsmClientCreateIntIpConfigSPMessage::setStatus(const bool &flag)
    {
        m_status = flag;
    }

    bool NsmClientCreateIntIpConfigSPMessage::getStatus() const
    {
        return (m_status);
    }

    void NsmClientCreateIntIpConfigSPMessage::setDirectedBroadcast(const bool &flag)
    {
        m_directed_broadcast = flag;
    }

    bool NsmClientCreateIntIpConfigSPMessage::getDirectedBroadcast() const
    {
        return (m_directed_broadcast);
    }

    void  NsmClientCreateIntIpConfigSPMessage::setProxy_arp(const bool &proxy_arp)
    {
        m_proxy_arp  =  proxy_arp;
    }

    bool  NsmClientCreateIntIpConfigSPMessage::getProxy_arp() const
    {
        return (m_proxy_arp);
    }

    void  NsmClientCreateIntIpConfigSPMessage::setArp_ageing_timeout(const UI32 &arp_ageing_timeout)
    {
        m_arp_ageing_timeout  =  arp_ageing_timeout;
    }

    UI32  NsmClientCreateIntIpConfigSPMessage::getArp_ageing_timeout() const
    {
        return (m_arp_ageing_timeout);
    }

	
    const void *NsmClientCreateIntIpConfigSPMessage::getCStructureForInputs ()
    {
       	nsm_ip_global_config_msg_t            *msg_p;
	    string str_ipaddr;

        msg_p = (nsm_ip_global_config_msg_t *)calloc(1, sizeof(nsm_ip_global_config_msg_t));

        msg_p->if_id = m_ifId;
        msg_p->opcode = m_opcode;
		strncpy(msg_p->if_name, m_ifName.c_str (), 63);
		msg_p->intf_type = m_intf_type;
        msg_p->status = (unsigned char)m_status;
        msg_p->directed_broadcast = (unsigned char)m_directed_broadcast;

		if(NSM_MSG_IP_MTU == m_opcode)
				msg_p->ip_mtu = m_mtu;
		else if( NSM_MSG_SEC_IP_ADDRESS == m_opcode || NSM_MSG_NO_SEC_IP_ADDRESS == m_opcode) {
				strncpy(msg_p->ip_address_mask, m_ip_addr_mask.c_str(), 18);
				msg_p->ip_address_mask[18] = '\0';
				msg_p->sec = m_sec ? 1 : 0;
				if (m_ospfPassive) {
					msg_p->ospf_opt = NSM_OSPF_PASSIVE;
				}
				else if (m_ospfIgnore) {
					msg_p->ospf_opt = NSM_OSPF_IGNORE;
				}
				else {
					msg_p->ospf_opt = NSM_NO_OSPF_OPT;
				}	
		} else if (m_opcode== NSM_MSG_ARP_AGEING_TIMEOUT) {
				msg_p->arp_timeout = m_arp_ageing_timeout;
		} 
        else if (m_opcode == NSM_MSG_IP_PROXY_ARP || 
            m_opcode == NSM_MSG_NO_IP_PROXY_ARP) 
        {
            msg_p->proxy_arp = (unsigned char)m_proxy_arp;
		}
		else if (m_opcode== NSM_MSG_IP_SWITCHPORT || 
            m_opcode== NSM_MSG_NO_IP_SWITCHPORT) 
        {
            msg_p->switchport = (unsigned char)m_switchport;
		}
	else if (m_opcode == NSM_MSG_MGMT_IP_ADDRESS) {
		strncpy(msg_p->ip_address_mask,m_ip_addr_mask.c_str(), IP_ADDR_MASK_LEN);
		 msg_p->ip_address_mask[IP_ADDR_MASK_LEN] = '\0';
	}

        return msg_p;
    	
	}

}
