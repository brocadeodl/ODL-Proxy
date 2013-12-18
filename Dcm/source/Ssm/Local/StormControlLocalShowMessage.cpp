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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : ojasvita                                                     *
 **************************************************************************/

#include "Ssm/Local/StormControlLocalWorker.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Ssm/Local/StormControlLocalShowMessage.h"
#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/SsmTypes.h"

namespace DcmNs
{
    StormControlLocalShowMessage::StormControlLocalShowMessage () : 
		PrismMessage (SsmLocalObjectManager::getPrismServiceId(), STORMCONTROLLOCALSHOW)
    {
    }

    StormControlLocalShowMessage::StormControlLocalShowMessage(const string &ifName, const UI32 &opCode,
		                  bool isGwRequest, const UI32 &ifType, const UI32 &protocol, const UI32 &locationId)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), STORMCONTROLLOCALSHOW),
        m_ifName       (ifName),
        m_opCode       (opCode),
        m_isGwRequest  (isGwRequest),
        m_ifType       (ifType),
		m_protocol     (protocol),
		m_locationId   (locationId)
    {
    }

    StormControlLocalShowMessage::~StormControlLocalShowMessage ()
    {
    }

    void  StormControlLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_ifType,"ifType"));
		addSerializableAttribute(new AttributeUI32(&m_protocol,"protocol"));
		addSerializableAttribute(new AttributeUI32(&m_locationId,"locationId"));
    }

    void  StormControlLocalShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  StormControlLocalShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  StormControlLocalShowMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  StormControlLocalShowMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void StormControlLocalShowMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool StormControlLocalShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    UI32  StormControlLocalShowMessage::getIfType() const
    {
        return (m_ifType);
    }

    void StormControlLocalShowMessage::setIfType(const UI32 &ifType)
    {
        m_ifType = ifType;
    }

	void StormControlLocalShowMessage::setProtocol(const UI32 &protocol)
	{
		m_protocol = protocol;
	}

	UI32  StormControlLocalShowMessage::getProtocol() const
	{
		return (m_protocol);
	}

	void StormControlLocalShowMessage::setLocationId(const UI32 &locationId) {
		m_locationId = locationId;
	}

	UI32  StormControlLocalShowMessage::getLocationId() const {
		return (m_locationId);
	}

    void StormControlLocalShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void StormControlLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }
}

