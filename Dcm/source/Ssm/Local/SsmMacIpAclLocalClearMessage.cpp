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
 *   Author : Ojasvita                                                     *
 **************************************************************************/

#include "Ssm/Local/SsmLocalIpAclWorker.h"
#include "Ssm/Local/SsmTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Ssm/Local/SsmMacIpAclLocalClearMessage.h"
#include "Ssm/Local/SsmLocalObjectManager.h"
#include "ClientInterface/Ssm/SsmApp.h"

namespace DcmNs
{

    SsmMacIpAclLocalClearMessage::SsmMacIpAclLocalClearMessage () : PrismMessage (SsmLocalObjectManager::getPrismServiceId(), SSMMACIPACLLOCALCLEAR)
    {
    }

    SsmMacIpAclLocalClearMessage::SsmMacIpAclLocalClearMessage(const string &PolicyName, const string &ifName, const UI32 &opCode,
		                  bool isGwRequest, const UI32 &aclType, const UI32 &ifType, const UI32 &direction)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), SSMMACIPACLLOCALCLEAR),
        m_PolicyName   (PolicyName),
        m_ifName       (ifName),
        m_opCode       (opCode),
        m_isGwRequest  (isGwRequest),
		m_aclType	   (aclType),
        m_ifType       (ifType),
		m_direction    (direction)
    {
    }

    SsmMacIpAclLocalClearMessage::SsmMacIpAclLocalClearMessage(const string &PolicyName, const string &ifName, const UI32 &opCode,
		                  bool isGwRequest, const UI32 &aclType, const UI32 &ifType, const UI32 &direction, const string &rbridgeId)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), SSMMACIPACLLOCALCLEAR),
        m_PolicyName   (PolicyName),
        m_ifName       (ifName),
        m_opCode       (opCode),
        m_isGwRequest  (isGwRequest),
		m_aclType	   (aclType),
        m_ifType       (ifType),
		m_direction    (direction),
		m_rbridgeId	   (rbridgeId)
    {
    }

    SsmMacIpAclLocalClearMessage::~SsmMacIpAclLocalClearMessage ()
    {
    }

    void  SsmMacIpAclLocalClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute(new AttributeString(&m_PolicyName,"PolicyName"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_ifType,"ifType"));
		addSerializableAttribute(new AttributeUI32(&m_aclType,"aclType"));
		addSerializableAttribute(new AttributeUI32(&m_direction,"direction"));
		addSerializableAttribute(new AttributeString(&m_rbridgeId,"rbridgeId"));
    }

    void  SsmMacIpAclLocalClearMessage::setPolicyName(const string &PolicyName)
    {
        m_PolicyName  =  PolicyName;
    }

    string  SsmMacIpAclLocalClearMessage::getPolicyName() const
    {
        return (m_PolicyName);
    }

    void  SsmMacIpAclLocalClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  SsmMacIpAclLocalClearMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  SsmMacIpAclLocalClearMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SsmMacIpAclLocalClearMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void SsmMacIpAclLocalClearMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool SsmMacIpAclLocalClearMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

	void  SsmMacIpAclLocalClearMessage::setAclType(const UI32 &aclType)
	{
		m_aclType = aclType;
	}

	UI32  SsmMacIpAclLocalClearMessage::getAclType() const
	{
		return (m_aclType);
	}

    UI32  SsmMacIpAclLocalClearMessage::getIfType() const
    {
        return (m_ifType);
    }

    void SsmMacIpAclLocalClearMessage::setIfType(const UI32 &ifType)
    {
        m_ifType = ifType;
    }

	void SsmMacIpAclLocalClearMessage::setDirection(const UI32 &direction)
	{
		m_direction = direction;
	}

	UI32  SsmMacIpAclLocalClearMessage::getDirection() const
	{
		return (m_direction);
	}

    void SsmMacIpAclLocalClearMessage::setRbridgeId(const string &rbridgeId)
    {
    	m_rbridgeId = rbridgeId;
    }

    string SsmMacIpAclLocalClearMessage::getRbridgeId() const
    {
    	return m_rbridgeId;
    }
}

