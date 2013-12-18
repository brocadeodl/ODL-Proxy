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

#include "ClientInterface/Nsm/NsmClientCreateVlanClassifierRuleSPMessage.h"
#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    NsmClientCreateVlanClassifierRuleSPMessage::NsmClientCreateVlanClassifierRuleSPMessage ()
        : DcmManagementInterfaceMessage (NSMCLIENTCREATEVLANCLASSIFIERRULESP)
    {
    }

    NsmClientCreateVlanClassifierRuleSPMessage::NsmClientCreateVlanClassifierRuleSPMessage (const UI32 &ruleId,const string &mac,const UI32 &protoVal,const UI32 &encap,const  UI32 &opcode)
        : DcmManagementInterfaceMessage (NSMCLIENTCREATEVLANCLASSIFIERRULESP),
        m_ruleId    (ruleId),
        m_mac    (mac),
        m_protoVal    (protoVal),
        m_encap    (encap),
	m_opcode   (opcode)
    {
    }

    NsmClientCreateVlanClassifierRuleSPMessage::~NsmClientCreateVlanClassifierRuleSPMessage ()
    {
    }

    void  NsmClientCreateVlanClassifierRuleSPMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_ruleId,"ruleId"));
        addSerializableAttribute (new AttributeString(&m_mac,"mac"));
        addSerializableAttribute (new AttributeUI32(&m_protoVal,"protoVal"));
        addSerializableAttribute (new AttributeUI32(&m_encap,"encap"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
    }

    void  NsmClientCreateVlanClassifierRuleSPMessage::setRuleId(const UI32 &ruleId)
    {
        m_ruleId  =  ruleId;
    }

    UI32  NsmClientCreateVlanClassifierRuleSPMessage::getRuleId() const
    {
        return (m_ruleId);
    }

    void  NsmClientCreateVlanClassifierRuleSPMessage::setMac(const string &mac)
    {
        m_mac  =  mac;
    }

    string  NsmClientCreateVlanClassifierRuleSPMessage::getMac() const
    {
        return (m_mac);
    }

    void  NsmClientCreateVlanClassifierRuleSPMessage::setProtoVal(const UI32 &protoVal)
    {
        m_protoVal  =  protoVal;
    }

    UI32  NsmClientCreateVlanClassifierRuleSPMessage::getProtoVal() const
    {
        return (m_protoVal);
    }

    void  NsmClientCreateVlanClassifierRuleSPMessage::setEncap(const UI32 &encap)
    {
        m_encap  =  encap;
    }

    UI32  NsmClientCreateVlanClassifierRuleSPMessage::getEncap() const
    {
        return (m_encap);
    }
    
    const void *NsmClientCreateVlanClassifierRuleSPMessage::getCStructureForInputs ()
    {
        VlanClassifierCreateRuleMsg_t *pInput = (VlanClassifierCreateRuleMsg_t *)malloc (sizeof (VlanClassifierCreateRuleMsg_t));

        strncpy (pInput->mac, m_mac.c_str(), 15);
	pInput->mac[14] = '\0';
        pInput->ruleId = m_ruleId;
        pInput->protoVal = m_protoVal;
        pInput->encap = m_encap;
        pInput->opcode = m_opcode;

        return (pInput);
    }

    void NsmClientCreateVlanClassifierRuleSPMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
    
    void  NsmClientCreateVlanClassifierRuleSPMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmClientCreateVlanClassifierRuleSPMessage::getOpcode() const
    {
        return (m_opcode);
    }

}

