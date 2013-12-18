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
 *   Author : pasu                                                     *
 **************************************************************************/

#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/SsmMacAclShowLocalSPMessage.h"
#include "Ssm/Local/SsmTypes.h"

namespace DcmNs
{
    SsmMacAclShowLocalSPMessage::SsmMacAclShowLocalSPMessage ()
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), SSMMACACLSHOWLOCALSP)
    {
    }
    
    SsmMacAclShowLocalSPMessage::SsmMacAclShowLocalSPMessage (const string &ifname, const UI32 &binding, const UI32 &detail, const UI32 &iftype, const UI32 &opcode, bool isGwRequest)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), SSMMACACLSHOWLOCALSP),
        m_ifname (ifname),
        m_binding (binding),
        m_detail(detail),
        m_ifType (iftype),
        m_opcode (opcode),
        m_isGwRequest (isGwRequest)
	{
	}

    SsmMacAclShowLocalSPMessage::~SsmMacAclShowLocalSPMessage ()
    {
    }

    void  SsmMacAclShowLocalSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyname"));
        addSerializableAttribute (new AttributeUI32(&m_policyType,"policytype"));
		addSerializableAttribute (new AttributeString(&m_ifname,"ifname"));
        addSerializableAttribute (new AttributeUI32(&m_binding,"binding"));
        addSerializableAttribute (new AttributeUI32(&m_detail,"detail"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"iftype"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeBool(&m_isGwRequest,"isGwRequest"));
    }


    UI32  SsmMacAclShowLocalSPMessage::getOpCode() const
    {
        return (m_opcode);
    }
    
    UI32  SsmMacAclShowLocalSPMessage::getPolicyType() const
    {
        return (m_policyType);
    }
    
    UI32  SsmMacAclShowLocalSPMessage::getIfType() const
    {
        return (m_ifType);
    }


    string  SsmMacAclShowLocalSPMessage::getIfName() const
    {
        return (m_ifname);
    }

    UI32  SsmMacAclShowLocalSPMessage::getBinding() const
    {
        return (m_binding);
    }

    UI32  SsmMacAclShowLocalSPMessage::getDetail() const
    {
        return(m_detail);
    }

    string  SsmMacAclShowLocalSPMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void SsmMacAclShowLocalSPMessage::setOpcode(const UI32 &opcode)
	{
		m_opcode = opcode;
	}
    
	void SsmMacAclShowLocalSPMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool SsmMacAclShowLocalSPMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }


    void SsmMacAclShowLocalSPMessage::setIfType(const UI32 &type)
	{
		m_ifType = type;
	}
	
    void SsmMacAclShowLocalSPMessage::setPolicyType(const UI32 &type)
	{
		m_policyType = type;
	}

	void  SsmMacAclShowLocalSPMessage::setIfName(const string &name)
	{
		m_ifname = name;
	}
	
    void  SsmMacAclShowLocalSPMessage::setBinding(const UI32 &binding)
    {
        m_binding = binding;
    }

    void  SsmMacAclShowLocalSPMessage::setDetail(const UI32 &detail)
    {
        m_detail = detail;
    }

	void  SsmMacAclShowLocalSPMessage::setPolicyName(const string &policyname)
	{
		m_policyName = policyname;
	}

	void SsmMacAclShowLocalSPMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer = findBuffer(bufferNum, size);
	}

	void SsmMacAclShowLocalSPMessage::copyAllBuffers (const PrismMessage &prismMessage)
	{
		this->copyBuffersFrom(prismMessage);
	}


}
