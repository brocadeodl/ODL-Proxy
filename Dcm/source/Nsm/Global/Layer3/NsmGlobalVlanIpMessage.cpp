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

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

#include "Nsm/Global/Layer3/NsmGlobalVlanIpMessage.h"

namespace DcmNs
{
    NsmGlobalVlanIpMessage::NsmGlobalVlanIpMessage()
        : ManagementInterfaceMessage(NsmGlobalObjectManager::getClassName(),
        NSMGLOBALVLANIPMSG)
    {
        m_opCode = (UI32)NSM_MSG_OP_CREATE;
		m_cmdCode = 0;
        m_rbridge = "";
        m_ifType = 0;
        m_ifId = 0;
        m_ifName = "";
        m_ipv4Addr = "";
        m_secondary = false;
        m_switchport = false;
        m_directed_broadcast = false;
        m_status = false;

        m_mtu = NSM_IP_MTU_DEF_VAL;
        m_proxy_arp = false;
        m_arp_aging_timeout = NSM_IP_ARP_AGING_TIMEOUT_DEF_VAL;
    }

    NsmGlobalVlanIpMessage::~NsmGlobalVlanIpMessage ()
    {
    }

    void  NsmGlobalVlanIpMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeUI32(&m_opCode, "opCode"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeString(&m_rbridge, "rbridge"));
        addSerializableAttribute (new AttributeUI32(&m_ifId, "ifId"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
        addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
        addSerializableAttribute (new AttributeString(&m_ipv4Addr, "ipv4Address"));
        addSerializableAttribute (new AttributeBool(&m_switchport, "switchport"));
        addSerializableAttribute (new AttributeBool(&m_secondary, "secondary"));
        addSerializableAttribute (new AttributeBool(&m_status, "status"));
        addSerializableAttribute (new AttributeUI32(&m_mtu, "mtu"));
        addSerializableAttribute (new AttributeBool(&m_directed_broadcast,
            "directed_broadcast"));
        addSerializableAttribute (new AttributeBool(&m_proxy_arp, "proxy_arp"));
        addSerializableAttribute (new AttributeUI32(&m_arp_aging_timeout,
            "arp_aging_timeout"));
    }

    void NsmGlobalVlanIpMessage::setOpCode(const UI32 &op)
    {
        m_opCode  = op;
    }

    UI32 NsmGlobalVlanIpMessage::getOpCode() const
    {
        return (m_opCode);
    }
    
    void  NsmGlobalVlanIpMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmGlobalVlanIpMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    string  NsmGlobalVlanIpMessage::getRbridge() const
    {
        return m_rbridge;
    }

    void  NsmGlobalVlanIpMessage::setRbridge(const string &name)
    {
        m_rbridge  =  name;
    }

    UI32  NsmGlobalVlanIpMessage::getIfType() const
    {
        return m_ifType;
    }

    void  NsmGlobalVlanIpMessage::setIfType(const UI32 &type)
    {
        m_ifType  =  type;
    }

    UI32  NsmGlobalVlanIpMessage::getIfId() const
    {
        return m_ifId;
    }

    void  NsmGlobalVlanIpMessage::setIfId(const UI32 &if_id)
    {
        m_ifId  =  if_id;
    }

    string  NsmGlobalVlanIpMessage::getIfName() const
    {
        return m_ifName;
    }

    void  NsmGlobalVlanIpMessage::setIfName(const string &if_name)
    {
        m_ifName  =  if_name;
    }

    void  NsmGlobalVlanIpMessage::setIpv4Address(const string &ip_address)
    {
        m_ipv4Addr  =  ip_address;
    }

    string  NsmGlobalVlanIpMessage::getIpv4Address() const
    {
        return (m_ipv4Addr);
    }

    bool  NsmGlobalVlanIpMessage::getSecond() const
    {
        return m_secondary;
    }

    void  NsmGlobalVlanIpMessage::setSecond(const bool &flag)
    {
        m_secondary  =  flag;
    }

    void  NsmGlobalVlanIpMessage::setStatus(const bool &flag)
    {
        m_status  =  flag;
    }

    bool  NsmGlobalVlanIpMessage::getStatus() const
    {
        return (m_status);
    }

    void  NsmGlobalVlanIpMessage::setDirectedBroadcast(const bool &flag)
    {
        m_directed_broadcast = flag;
    }

    bool  NsmGlobalVlanIpMessage::getDirectedBroadcast() const
    {
        return (m_directed_broadcast);
    }

    bool  NsmGlobalVlanIpMessage::getSwitchport() const
    {
        return m_switchport;
    }

    void  NsmGlobalVlanIpMessage::setSwitchport(const bool &flag)
    {
        m_switchport  =  flag;
    }

    void  NsmGlobalVlanIpMessage::setMtu(const UI32 &mtu)
    {
        m_mtu = mtu;
    }

    UI32  NsmGlobalVlanIpMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  NsmGlobalVlanIpMessage::setProxyArp(const bool &flag)
    {
        m_proxy_arp = flag;
    }

    bool  NsmGlobalVlanIpMessage::getProxyArp() const
    {
        return (m_proxy_arp);
    }

    void  NsmGlobalVlanIpMessage::setArpAgingTimeout(const UI32 &timeout)
    {
        m_arp_aging_timeout = timeout;
    }

    UI32  NsmGlobalVlanIpMessage::getArpAgingTimeout() const
    {
        return (m_arp_aging_timeout);
    }


} //DcmNs
