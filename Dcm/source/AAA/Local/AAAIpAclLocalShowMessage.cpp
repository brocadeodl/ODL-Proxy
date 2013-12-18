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
 *   Author : Yugant                                                     *
 **************************************************************************/

#include "AAA/Local/LocalIpAclWorker.h"
#include "AAA/Local/AAALocalTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "AAA/Local/AAAIpAclLocalShowMessage.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "ClientInterface/Ssm/SsmApp.h"

namespace DcmNs
{

    AAAIpAclLocalShowMessage::AAAIpAclLocalShowMessage () : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId(), AAALOCALSHOWIPACL)
    {
    }

    AAAIpAclLocalShowMessage::AAAIpAclLocalShowMessage(const string &PolicyName, const string &ifName, const UI32 &opCode,
		                  bool isGwRequest, const UI32 &aclType, const UI32 &ifType, const UI32 &direction)
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALSHOWIPACL),
        m_PolicyName   (PolicyName),
        m_ifName       (ifName),
        m_opCode       (opCode),
        m_isGwRequest  (isGwRequest),
		m_aclType	   (aclType),
        m_ifType       (ifType),
		m_direction    (direction)
    {
    }

    AAAIpAclLocalShowMessage::~AAAIpAclLocalShowMessage ()
    {
    }

    void  AAAIpAclLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute(new AttributeString(&m_PolicyName,"PolicyName"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_ifType,"ifType"));
		addSerializableAttribute(new AttributeUI32(&m_aclType,"aclType"));
		addSerializableAttribute(new AttributeUI32(&m_direction,"direction"));
    }

    void  AAAIpAclLocalShowMessage::setPolicyName(const string &PolicyName)
    {
        m_PolicyName  =  PolicyName;
    }

    string  AAAIpAclLocalShowMessage::getPolicyName() const
    {
        return (m_PolicyName);
    }

    void  AAAIpAclLocalShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  AAAIpAclLocalShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  AAAIpAclLocalShowMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  AAAIpAclLocalShowMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void AAAIpAclLocalShowMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool AAAIpAclLocalShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

	void  AAAIpAclLocalShowMessage::setAclType(const UI32 &aclType)
	{
		m_aclType = aclType;
	}

	UI32  AAAIpAclLocalShowMessage::getAclType() const
	{
		return (m_aclType);
	}

    UI32  AAAIpAclLocalShowMessage::getIfType() const
    {
        return (m_ifType);
    }

    void AAAIpAclLocalShowMessage::setIfType(const UI32 &ifType)
    {
        m_ifType = ifType;
    }

	void AAAIpAclLocalShowMessage::setDirection(const UI32 &direction)
	{
		m_direction = direction;
	}

	UI32  AAAIpAclLocalShowMessage::getDirection() const
	{
		return (m_direction);
	}

    void AAAIpAclLocalShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void AAAIpAclLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

}

