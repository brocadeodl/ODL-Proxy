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

#include "Nsm/Local/Layer3/NsmLocalNodeIpMessage.h"

namespace DcmNs
{

    NsmLocalNodeIpMessage::NsmLocalNodeIpMessage()
       : PrismMessage(NsmLocalObjectManager::getPrismServiceId(),
       NSMLOCALNODEIPMSG)
    {
        m_opCode = (UI32)NSM_MSG_OP_CREATE;
		m_cmdCode = 0;
        m_ifId = 0;
        m_ifType = 0;
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
        m_skipValidation_BackendStep = false;
    }

    NsmLocalNodeIpMessage::~NsmLocalNodeIpMessage ()
    {
    }

    void  NsmLocalNodeIpMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeUI32(&m_opCode, "opCode"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_ifId, "ifId"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
        addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
        addSerializableAttribute (new AttributeString(&m_ipv4Addr, "ipv4Address"));
        addSerializableAttribute (new AttributeBool(&m_switchport, "switchport"));
        addSerializableAttribute (new AttributeBool(&m_secondary, "secondary"));
        addSerializableAttribute (new AttributeBool(&m_ospfPassive, "ospfPassive"));
        addSerializableAttribute (new AttributeBool(&m_ospfIgnore, "ospfIgnore"));

        addSerializableAttribute (new AttributeBool(&m_status, "status"));
        addSerializableAttribute (new AttributeUI32(&m_mtu, "mtu"));
        addSerializableAttribute (new AttributeBool(&m_directed_broadcast,
            "directed_broadcast"));
        addSerializableAttribute (new AttributeBool(&m_proxy_arp, "proxy_arp"));
        addSerializableAttribute (new AttributeUI32(&m_arp_aging_timeout,
            "arp_aging_timeout"));
        addSerializableAttribute (new AttributeBool(&m_skipValidation_BackendStep, "skipValBeSteps"));
    }

    void NsmLocalNodeIpMessage::setOpCode(const UI32 &op)
    {
        m_opCode  = op;
    }

    UI32 NsmLocalNodeIpMessage::getOpCode() const
    {
        return (m_opCode);
    }
    
    void  NsmLocalNodeIpMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmLocalNodeIpMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    UI32  NsmLocalNodeIpMessage::getIfType() const
    {
        return m_ifType;
    }

    void  NsmLocalNodeIpMessage::setIfType(const UI32 &type)
    {
        m_ifType  =  type;
    }

    UI32  NsmLocalNodeIpMessage::getIfId() const
    {
        return m_ifId;
    }

    void  NsmLocalNodeIpMessage::setIfId(const UI32 &if_id)
    {
        m_ifId  =  if_id;
    }

    string  NsmLocalNodeIpMessage::getIfName() const
    {
        return m_ifName;
    }

    void  NsmLocalNodeIpMessage::setIfName(const string &if_name)
    {
        m_ifName  =  if_name;
    }

    void  NsmLocalNodeIpMessage::setIpv4Address(const string &ip_address)
    {
        m_ipv4Addr  =  ip_address;
    }

    string  NsmLocalNodeIpMessage::getIpv4Address() const
    {
        return (m_ipv4Addr);
    }

    bool  NsmLocalNodeIpMessage::getSecond() const
    {
        return m_secondary;
    }

    void  NsmLocalNodeIpMessage::setSecond(const bool &flag)
    {
        m_secondary  =  flag;
    }

    bool NsmLocalNodeIpMessage::getOspfPassive() const
    {
        return m_ospfPassive;
    }
        
    void NsmLocalNodeIpMessage::setOspfPassive(const bool &flag)
    {
        m_ospfPassive  =  flag;
    }
        
    bool NsmLocalNodeIpMessage::getOspfIgnore() const
    {
        return m_ospfIgnore;
    }
 
    void NsmLocalNodeIpMessage::setOspfIgnore(const bool &flag)
    {
        m_ospfIgnore  =  flag;
    }

    bool  NsmLocalNodeIpMessage::getSwitchport() const
    {
        return m_switchport;
    }

    void  NsmLocalNodeIpMessage::setSwitchport(const bool &flag)
    {
        m_switchport  =  flag;
    }

    void  NsmLocalNodeIpMessage::setStatus(const bool &flag)
    {
        m_status  =  flag;
    }

    bool  NsmLocalNodeIpMessage::getStatus() const
    {
        return (m_status);
    }

    void  NsmLocalNodeIpMessage::setMtu(const UI32 &mtu)
    {
        m_mtu = mtu;
    }

    UI32  NsmLocalNodeIpMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  NsmLocalNodeIpMessage::setDirectedBroadcast(const bool &flag)
    {
        m_directed_broadcast = flag;
    }

    bool  NsmLocalNodeIpMessage::getDirectedBroadcast() const
    {
        return (m_directed_broadcast);
    }

    void  NsmLocalNodeIpMessage::setProxyArp(const bool &flag)
    {
        m_proxy_arp = flag;
    }

    bool  NsmLocalNodeIpMessage::getProxyArp() const
    {
        return (m_proxy_arp);
    }

    void  NsmLocalNodeIpMessage::setArpAgingTimeout(const UI32 &timeout)
    {
        m_arp_aging_timeout = timeout;
    }

    UI32  NsmLocalNodeIpMessage::getArpAgingTimeout() const
    {
        return (m_arp_aging_timeout);
    }

    void  NsmLocalNodeIpMessage :: setValidationBackendStepFlag(bool skipFlag) {
        m_skipValidation_BackendStep = skipFlag;
    }

    bool  NsmLocalNodeIpMessage :: getValidationBackendStepFlag() {
        return m_skipValidation_BackendStep;
    }

} //DcmNs
