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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Justin Huang                                                 *
 **************************************************************************/

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

#include "Nsm/Local/Layer3/NsmLocalPhyIpMessage.h"

namespace DcmNs
{

    NsmLocalPhyIpMessage::NsmLocalPhyIpMessage()
       : PrismMessage(NsmLocalObjectManager::getPrismServiceId(),
       NSMLOCALPHYIPMSG)
    {
        m_opCode = (UI32)NSM_MSG_OP_CREATE;
		m_cmdCode = 0;
        m_ifId = 0;
        m_ifName = "";
        m_ipv4Addr = "";
        m_secondary = false;
        m_ospfPassive = false;
        m_ospfIgnore = false;
        m_switchport = false;

        m_mtu = NSM_IP_MTU_DEF_VAL;
        m_directed_broadcast = false;
        m_proxy_arp = false;
        m_arp_aging_timeout = NSM_IP_ARP_AGING_TIMEOUT_DEF_VAL;
    }

    NsmLocalPhyIpMessage::~NsmLocalPhyIpMessage ()
    {
    }

    void  NsmLocalPhyIpMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeUI32(&m_opCode, "opCode"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_ifId, "ifId"));
        addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
        addSerializableAttribute (new AttributeString(&m_ipv4Addr, "ipv4Address"));
        addSerializableAttribute (new AttributeBool(&m_switchport, "switchport"));
        addSerializableAttribute (new AttributeBool(&m_secondary, "secondary"));
        addSerializableAttribute (new AttributeBool(&m_ospfPassive, "ospfPassive"));
        addSerializableAttribute (new AttributeBool(&m_ospfIgnore, "ospfIgnore"));

        addSerializableAttribute (new AttributeUI32(&m_mtu, "mtu"));
        addSerializableAttribute (new AttributeBool(&m_directed_broadcast, 
            "directed_broadcast"));
        addSerializableAttribute (new AttributeBool(&m_proxy_arp, "proxy_arp"));
        addSerializableAttribute (new AttributeUI32(&m_arp_aging_timeout, 
            "arp_aging_timeout"));
    }

    void NsmLocalPhyIpMessage::setOpCode(const UI32 &op)
    {
        m_opCode  = op;
    }

    UI32 NsmLocalPhyIpMessage::getOpCode() const
    {
        return (m_opCode);
    }
    
    void  NsmLocalPhyIpMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmLocalPhyIpMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    UI32  NsmLocalPhyIpMessage::getIfType() const
    {
        return m_ifType;
    }

    void  NsmLocalPhyIpMessage::setIfType(const UI32 &type)
    {
        m_ifType  =  type;
    }

    UI32  NsmLocalPhyIpMessage::getIfId() const
    {
        return m_ifId;
    }

    void  NsmLocalPhyIpMessage::setIfId(const UI32 &if_id)
    {
        m_ifId  =  if_id;
    }

    string  NsmLocalPhyIpMessage::getIfName() const
    {
        return m_ifName;
    }

    void  NsmLocalPhyIpMessage::setIfName(const string &if_name)
    {
        m_ifName  =  if_name;
    }

    void  NsmLocalPhyIpMessage::setIpv4Address(const string &ip_address)
    {
        m_ipv4Addr  =  ip_address;
    }

    string  NsmLocalPhyIpMessage::getIpv4Address() const
    {
        return (m_ipv4Addr);
    }

    bool  NsmLocalPhyIpMessage::getSecond() const
    {
        return m_secondary;
    }

    void  NsmLocalPhyIpMessage::setSecond(const bool &flag)
    {
        m_secondary  =  flag;
    }

    bool NsmLocalPhyIpMessage::getOspfPassive() const
    {
        return m_ospfPassive;
    }

    void NsmLocalPhyIpMessage::setOspfPassive(const bool &flag) 
    {
        m_ospfPassive  =  flag;
    }	

    bool NsmLocalPhyIpMessage::getOspfIgnore() const
    {
        return m_ospfIgnore;
    }

    void NsmLocalPhyIpMessage::setOspfIgnore(const bool &flag) 
    {
        m_ospfIgnore  =  flag;
    }	
    
	bool  NsmLocalPhyIpMessage::getSwitchport() const
    {
        return m_switchport;
    }

    void  NsmLocalPhyIpMessage::setSwitchport(const bool &flag)
    {
        m_switchport  =  flag;
    }

    void  NsmLocalPhyIpMessage::setDirectedBroadcast(const bool &flag)
    {
        m_directed_broadcast = flag;
    }

    bool  NsmLocalPhyIpMessage::getDirectedBroadcast() const
    {
        return (m_directed_broadcast);
    }

    void  NsmLocalPhyIpMessage::setMtu(const UI32 &mtu)
    {
        m_mtu = mtu;
    }

    UI32  NsmLocalPhyIpMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  NsmLocalPhyIpMessage::setProxyArp(const bool &flag)
    {
        m_proxy_arp = flag;
    }

    bool  NsmLocalPhyIpMessage::getProxyArp() const
    {
        return (m_proxy_arp);
    }

    void  NsmLocalPhyIpMessage::setArpAgingTimeout(const UI32 &timeout)
    {
        m_arp_aging_timeout = timeout;
    }

    UI32  NsmLocalPhyIpMessage::getArpAgingTimeout() const
    {
        return (m_arp_aging_timeout);
    }


} //DcmNs
