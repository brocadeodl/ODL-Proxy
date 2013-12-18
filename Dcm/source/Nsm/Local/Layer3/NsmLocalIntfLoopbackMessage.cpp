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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : justinh                                                      *
 **************************************************************************/

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
//#include "Framework/Attributes/AttributeEnum.h"

#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/NsmUtils.h"

#include "Nsm/Local/Layer3/NsmLocalIntfLoopbackMessage.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

namespace DcmNs
{
    NsmLocalIntfLoopbackMessage::NsmLocalIntfLoopbackMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), 
        NSMLOCALINTFLOOPBACKMSG)
    {
        m_opCode = (UI32)NSM_MSG_OP_CREATE;
		m_cmdCode = 0;
        m_ifId = 0;
        m_ifName = "";
        m_rbridge = "";
		m_status = false;
        m_ipv4Addr = "";
        m_vrfName = "";
    }

    NsmLocalIntfLoopbackMessage::NsmLocalIntfLoopbackMessage (UI32 id)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), 
        NSMLOCALINTFLOOPBACKMSG),
        m_ifId (id)
    {
        m_opCode = (UI32)NSM_MSG_OP_CREATE;
		m_cmdCode = 0;
        m_ifName = "";
		m_status = false;
        m_ipv4Addr = "";
        m_vrfName = "";
    }

    NsmLocalIntfLoopbackMessage::~NsmLocalIntfLoopbackMessage ()
    {
    }

    void  NsmLocalIntfLoopbackMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeUI32(&m_opCode, "opCode"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_ifId, "ifId"));
        addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
        addSerializableAttribute (new AttributeBool(&m_status, "status"));
        addSerializableAttribute (new AttributeString(&m_ipv4Addr, "ipv4Addr"));
        addSerializableAttribute (new AttributeString(&m_vrfName, "vrfName"));
        //addSerializableAttribute (new AttributeUI32(&m_ipMtu, "mtu"));
        //addSerializableAttribute (new AttributeBool(&m_proxy_arp, "proxy_arp"));
        //addSerializableAttribute (new AttributeUI32(&m_arp_aging_timeout,"arp_aging_timeout"));
    }

    void  NsmLocalIntfLoopbackMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  NsmLocalIntfLoopbackMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  NsmLocalIntfLoopbackMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmLocalIntfLoopbackMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmLocalIntfLoopbackMessage::setIfId(const UI32 &id)
    {
        m_ifId = id;
    }

    UI32  NsmLocalIntfLoopbackMessage::getIfId() const
    {
        return (m_ifId);
    }

    void  NsmLocalIntfLoopbackMessage::setIfName(const string &name)
    {
        m_ifName = name;
    }

    string  NsmLocalIntfLoopbackMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmLocalIntfLoopbackMessage::setRbridge(const string &name)
    {
        m_rbridge = name;
    }

    string  NsmLocalIntfLoopbackMessage::getRbridge() const
    {
        return (m_rbridge);
    }

    void  NsmLocalIntfLoopbackMessage::setStatus(const bool &flag)
    {
        m_status  =  flag;
    }

    bool  NsmLocalIntfLoopbackMessage::getStatus() const
    {
        return (m_status);
    }

    void  NsmLocalIntfLoopbackMessage::setIpv4Addr(const string &addr)
    {
        m_ipv4Addr = addr;
    }

    string  NsmLocalIntfLoopbackMessage::getIpv4Addr() const
    {
        return (m_ipv4Addr);
    }

    void  NsmLocalIntfLoopbackMessage::setVrfName(const string &name)
    {
        m_vrfName = name;
    }

    string  NsmLocalIntfLoopbackMessage::getVrfName() const
    {
        return (m_vrfName);
    }
}

