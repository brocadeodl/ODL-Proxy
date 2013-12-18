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
 *   Author : Ojas                                                     *
 **************************************************************************/

#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/SsmIpAclShowLocalSPMessage.h"
#include "Ssm/Local/SsmTypes.h"

namespace DcmNs
{
    SsmIpAclShowLocalSPMessage::SsmIpAclShowLocalSPMessage ()
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), SSMIPACLSHOWLOCALSP)
    {
    }
    
    SsmIpAclShowLocalSPMessage::SsmIpAclShowLocalSPMessage (const string &ifname, const UI32 &binding, const UI32 &detail, const UI32 &iftype, const UI32 &opcode, bool isGwRequest)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), SSMIPACLSHOWLOCALSP),
        m_ifname (ifname),
		m_binding (binding),
		m_detail(detail),
        m_ifType (iftype),
        m_opcode (opcode),
        m_isGwRequest (isGwRequest)
	{
	}

    SsmIpAclShowLocalSPMessage::~SsmIpAclShowLocalSPMessage ()
    {
    }

    void  SsmIpAclShowLocalSPMessage::setupAttributesForSerialization()
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

    UI32  SsmIpAclShowLocalSPMessage::getOpCode() const
    {
        return (m_opcode);
    }
    
    UI32  SsmIpAclShowLocalSPMessage::getPolicyType() const
    {
        return (m_policyType);
    }
    
    UI32  SsmIpAclShowLocalSPMessage::getIfType() const
    {
        return (m_ifType);
    }

    string  SsmIpAclShowLocalSPMessage::getIfName() const
    {
        return (m_ifname);
    }

	UI32  SsmIpAclShowLocalSPMessage::getBinding() const
	{
		return (m_binding);
	}

	UI32  SsmIpAclShowLocalSPMessage::getDetail() const
	{
		return(m_detail);
	}

    string  SsmIpAclShowLocalSPMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void SsmIpAclShowLocalSPMessage::setOpcode(const UI32 &opcode)
	{
		m_opcode = opcode;
	}
    
	void SsmIpAclShowLocalSPMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool SsmIpAclShowLocalSPMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void SsmIpAclShowLocalSPMessage::setIfType(const UI32 &type)
	{
		m_ifType = type;
	}
	
    void SsmIpAclShowLocalSPMessage::setPolicyType(const UI32 &type)
	{
		m_policyType = type;
	}

	void  SsmIpAclShowLocalSPMessage::setIfName(const string &name)
	{
		m_ifname = name;
	}

	void  SsmIpAclShowLocalSPMessage::setBinding(const UI32 &binding)
    {
        m_binding = binding;
    }

	void  SsmIpAclShowLocalSPMessage::setDetail(const UI32 &detail)
	{
		m_detail = detail;
	}

	void  SsmIpAclShowLocalSPMessage::setPolicyName(const string &policyname)
	{
		m_policyName = policyname;
	}

	void SsmIpAclShowLocalSPMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer = findBuffer(bufferNum, size);
	}

	void SsmIpAclShowLocalSPMessage::copyAllBuffers (const PrismMessage &prismMessage)
	{
		this->copyBuffersFrom(prismMessage);
	}
}
