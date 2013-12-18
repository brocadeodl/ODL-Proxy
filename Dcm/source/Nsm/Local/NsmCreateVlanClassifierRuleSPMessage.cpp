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
 *   Author : rmuthigi                                                     *
 **************************************************************************/
#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmCreateVlanClassifierRuleSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmCreateVlanClassifierRuleSPMessage::NsmCreateVlanClassifierRuleSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMCREATEVLANCLASSIFIERRULESP)
    {
		m_encap = 0;
		m_opcode = 0;
		m_protoVal= 0;
		m_ruleId = 0;
    }

    NsmCreateVlanClassifierRuleSPMessage::NsmCreateVlanClassifierRuleSPMessage (const UI32 &ruleId,const string &mac,const UI32 &protoVal,const UI32 &encap,const  UI32 &opcode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMCREATEVLANCLASSIFIERRULESP),
        m_ruleId    (ruleId),
        m_mac    (mac),
        m_protoVal    (protoVal),
        m_encap    (encap),
	m_opcode   (opcode) 
    {
    }

    NsmCreateVlanClassifierRuleSPMessage::~NsmCreateVlanClassifierRuleSPMessage ()
    {
    }

    void  NsmCreateVlanClassifierRuleSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_ruleId,"ruleId"));
        addSerializableAttribute (new AttributeString(&m_mac,"mac"));
        addSerializableAttribute (new AttributeUI32(&m_protoVal,"protoVal"));
        addSerializableAttribute (new AttributeUI32(&m_encap,"encap"));
	addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
    }

    void  NsmCreateVlanClassifierRuleSPMessage::setRuleId(const UI32 &ruleId)
    {
        m_ruleId  =  ruleId;
    }

    UI32  NsmCreateVlanClassifierRuleSPMessage::getRuleId() const
    {
        return (m_ruleId);
    }

    void  NsmCreateVlanClassifierRuleSPMessage::setMac(const string &mac)
    {
        m_mac  =  mac;
    }

    string  NsmCreateVlanClassifierRuleSPMessage::getMac() const
    {
        return (m_mac);
    }

    void  NsmCreateVlanClassifierRuleSPMessage::setProtoVal(const UI32 &protoVal)
    {
        m_protoVal  =  protoVal;
    }

    UI32  NsmCreateVlanClassifierRuleSPMessage::getProtoVal() const
    {
        return (m_protoVal);
    }

    void  NsmCreateVlanClassifierRuleSPMessage::setEncap(const UI32 &encap)
    {
        m_encap  =  encap;
    }

    UI32  NsmCreateVlanClassifierRuleSPMessage::getEncap() const
    {
        return (m_encap);
    }
    void  NsmCreateVlanClassifierRuleSPMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmCreateVlanClassifierRuleSPMessage::getOpcode() const
    {
        return (m_opcode);
    }

}
