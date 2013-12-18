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
 *   Author : Bhavin                                                     *
 **************************************************************************/

#include "Ssm/Local/SsmLocalIpAclWorker.h"
#include "Ssm/Local/SsmTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Ssm/Local/SsmMacIpAclLocalShowMessage.h"
#include "Ssm/Local/SsmLocalObjectManager.h"
#include "ClientInterface/Ssm/SsmApp.h"

namespace DcmNs
{

    SsmMacIpAclLocalShowMessage::SsmMacIpAclLocalShowMessage () : PrismMessage (SsmLocalObjectManager::getPrismServiceId(), SSMMACIPACLLOCALSHOW)
    {
    }

    SsmMacIpAclLocalShowMessage::SsmMacIpAclLocalShowMessage(const string &PolicyName, const string &ifName, const UI32 &opCode,
		                  bool isGwRequest, const UI32 &aclType, const UI32 &ifType, const UI32 &direction)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), SSMMACIPACLLOCALSHOW),
        m_PolicyName   (PolicyName),
        m_ifName       (ifName),
        m_opCode       (opCode),
        m_isGwRequest  (isGwRequest),
		m_aclType	   (aclType),
        m_ifType       (ifType),
		m_direction    (direction)
    {
    }

    SsmMacIpAclLocalShowMessage::SsmMacIpAclLocalShowMessage(const string &PolicyName, const string &ifName, const UI32 &opCode,
		                  bool isGwRequest, const UI32 &aclType, const UI32 &ifType, const UI32 &direction, const UI64 &bindOid)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), SSMMACIPACLLOCALSHOW),
        m_PolicyName   (PolicyName),
        m_ifName       (ifName),
        m_opCode       (opCode),
        m_isGwRequest  (isGwRequest),
		m_aclType	   (aclType),
        m_ifType       (ifType),
		m_direction    (direction),
    	m_bindOid	   (bindOid)
    {
    }

    SsmMacIpAclLocalShowMessage::SsmMacIpAclLocalShowMessage(const string &PolicyName, const string &ifName, const UI32 &opCode,
		                  bool isGwRequest, const UI32 &aclType, const UI32 &ifType, const UI32 &direction, const UI64 &bindOid, const string &rbridgeId)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), SSMMACIPACLLOCALSHOW),
        m_PolicyName   (PolicyName),
        m_ifName       (ifName),
        m_opCode       (opCode),
        m_isGwRequest  (isGwRequest),
		m_aclType	   (aclType),
        m_ifType       (ifType),
		m_direction    (direction),
    	m_bindOid	   (bindOid),
		m_rbridgeId    (rbridgeId)
    {
    }

    SsmMacIpAclLocalShowMessage::~SsmMacIpAclLocalShowMessage ()
    {
    }

    void  SsmMacIpAclLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute(new AttributeString(&m_PolicyName,"PolicyName"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_ifType,"ifType"));
		addSerializableAttribute(new AttributeUI32(&m_aclType,"aclType"));
		addSerializableAttribute(new AttributeUI32(&m_direction,"direction"));
		addSerializableAttribute(new AttributeUI64(&m_bindOid,"bindOid"));
		addSerializableAttribute(new AttributeString(&m_rbridgeId,"rbridgeId"));
    }

    void  SsmMacIpAclLocalShowMessage::setPolicyName(const string &PolicyName)
    {
        m_PolicyName  =  PolicyName;
    }

    string  SsmMacIpAclLocalShowMessage::getPolicyName() const
    {
        return (m_PolicyName);
    }

    void  SsmMacIpAclLocalShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  SsmMacIpAclLocalShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  SsmMacIpAclLocalShowMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SsmMacIpAclLocalShowMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void SsmMacIpAclLocalShowMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool SsmMacIpAclLocalShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

	void  SsmMacIpAclLocalShowMessage::setAclType(const UI32 &aclType)
	{
		m_aclType = aclType;
	}

	UI32  SsmMacIpAclLocalShowMessage::getAclType() const
	{
		return (m_aclType);
	}

    UI32  SsmMacIpAclLocalShowMessage::getIfType() const
    {
        return (m_ifType);
    }

    void SsmMacIpAclLocalShowMessage::setIfType(const UI32 &ifType)
    {
        m_ifType = ifType;
    }

	void SsmMacIpAclLocalShowMessage::setDirection(const UI32 &direction)
	{
		m_direction = direction;
	}

	UI32  SsmMacIpAclLocalShowMessage::getDirection() const
	{
		return (m_direction);
	}

	void SsmMacIpAclLocalShowMessage::setBindOid(const UI64 &bindOid)
	{
		m_bindOid = bindOid;
	}

	UI64  SsmMacIpAclLocalShowMessage::getBindOid() const
	{
		return (m_bindOid);
	}

    void SsmMacIpAclLocalShowMessage::setRbridgeId(const string &rbridgeId)
    {
    	m_rbridgeId = rbridgeId;
    }

    string SsmMacIpAclLocalShowMessage::getRbridgeId() const
    {
    	return m_rbridgeId;
    }

    void SsmMacIpAclLocalShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void SsmMacIpAclLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

}

