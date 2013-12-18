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
//#include "Nsm/ClientInterface/VlanClassifierCommon.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Global/NsmGlobalCreateVlanClassifierRuleMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    NsmGlobalCreateVlanClassifierRuleMessage::NsmGlobalCreateVlanClassifierRuleMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALCREATEVLANCLASSIFIERRULE)
    {
		m_configTypeChoice = 0;
		m_encap = ENCAP_TYPE_UNKNOWN;
		m_opcode = 0;
		m_protoValEnum = PROTO_TYPE_UNKNOWN;
		m_ruleId = 0; 
    }

    NsmGlobalCreateVlanClassifierRuleMessage::NsmGlobalCreateVlanClassifierRuleMessage (const UI32 &ruleId,const string &mac,const ProtoType &protoValEnum,const string &protoValStr,const string &appType,const EncapType &encap,const UI32 &opcode, const UI32 &choice)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALCREATEVLANCLASSIFIERRULE),
        m_ruleId    (ruleId),
        m_mac    (mac),
        m_protoValEnum    (protoValEnum),
        m_protoValStr     (protoValStr),
	m_appType     (appType),
        m_encap    (encap),
	m_opcode   (opcode),
	m_configTypeChoice (choice) 
    {
    }

    NsmGlobalCreateVlanClassifierRuleMessage::~NsmGlobalCreateVlanClassifierRuleMessage ()
    {
    }

    void  NsmGlobalCreateVlanClassifierRuleMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_ruleId,"ruleId"));
        addSerializableAttribute (new AttributeString(&m_mac,"mac"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_protoValEnum),"protoVal"));
	addSerializableAttribute (new AttributeString(&m_protoValStr,"protoValStr"));
	addSerializableAttribute (new AttributeString(&m_appType,"appType"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_encap),"encap"));
	addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
	addSerializableAttribute (new AttributeUI32(&m_configTypeChoice,"configTypeChoice"));
    }

    void  NsmGlobalCreateVlanClassifierRuleMessage::setRuleId(const UI32 &ruleId)
    {
        m_ruleId  =  ruleId;
    }

    UI32  NsmGlobalCreateVlanClassifierRuleMessage::getRuleId() const
    {
        return (m_ruleId);
    }

    void  NsmGlobalCreateVlanClassifierRuleMessage::setMac(const string &mac)
    {
        m_mac  =  mac;
    }

    string  NsmGlobalCreateVlanClassifierRuleMessage::getMac() const
    {
        return (m_mac);
    }

    void  NsmGlobalCreateVlanClassifierRuleMessage::setProtoValEnum(const ProtoType &protoValEnum)
    {
        m_protoValEnum  =  protoValEnum;
    }

    ProtoType  NsmGlobalCreateVlanClassifierRuleMessage::getProtoValEnum() const
    {
        return (m_protoValEnum);
    }
     
     void  NsmGlobalCreateVlanClassifierRuleMessage::setProtoValStr(const string &protoValStr)
    {
        m_protoValStr =  protoValStr;
    }

    string  NsmGlobalCreateVlanClassifierRuleMessage::getProtoValStr() const
    {
        return (m_protoValStr);
    }

    void  NsmGlobalCreateVlanClassifierRuleMessage::setAppType(const string &appType)
    {
        m_appType  =  appType;
    }

    string  NsmGlobalCreateVlanClassifierRuleMessage::getAppType() const
    {
        return (m_appType);
    }

    void  NsmGlobalCreateVlanClassifierRuleMessage::setEncap(const EncapType &encap)
    {
        m_encap  =  encap;
    }

    EncapType  NsmGlobalCreateVlanClassifierRuleMessage::getEncap() const
    {
        return (m_encap);
    }
    void  NsmGlobalCreateVlanClassifierRuleMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmGlobalCreateVlanClassifierRuleMessage::getOpcode() const
    {
        return (m_opcode);
    }
     UI32 NsmGlobalCreateVlanClassifierRuleMessage::getConfigTypeChoice () const
    {
        return (m_configTypeChoice);
    }

    void NsmGlobalCreateVlanClassifierRuleMessage::setConfigTypeChoice (const UI32 &configTypeChoice)
    {
        m_configTypeChoice = configTypeChoice;
    }


}
