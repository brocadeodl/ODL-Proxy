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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Ayush Jaiswal                                                *
 **************************************************************************/

#include "ThresholdMonitor/Global/SecGlobalShowMessage.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalObjectManager.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalTypes.h"

namespace DcmNs
{

    SecGlobalShowMessage::SecGlobalShowMessage ()
        : PrismMessage (ThresholdMonitorGlobalObjectManager::getPrismServiceId (), SECURITYGLOBALSHOW)
    {
    }

    SecGlobalShowMessage::SecGlobalShowMessage (const UI32 &areaName)
        : PrismMessage (ThresholdMonitorGlobalObjectManager::getPrismServiceId (), SECURITYGLOBALSHOW),
		m_areaName (areaName)
    {
    }

    SecGlobalShowMessage::~SecGlobalShowMessage ()
    {
    }

    void  SecGlobalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_areaName,"areaName"));
        addSerializableAttribute (new AttributeBool(&m_sendToAllNodes,"sendToAllNodes"));
        addSerializableAttribute (new AttributeUI32(&m_locationId,"locationId"));
    }

    void SecGlobalShowMessage::setAreaName(const UI32 &areaName)
    {
        m_areaName = areaName;
    }

    UI32 SecGlobalShowMessage::getAreaName() const
    {
        return m_areaName;
    }

	void SecGlobalShowMessage::setSendToAllNodes(const bool &sendToAllNodes)
	{
		m_sendToAllNodes = sendToAllNodes;
	}

	bool SecGlobalShowMessage::getSendToAllNodes() const
	{
		return(m_sendToAllNodes);
	}

	void SecGlobalShowMessage::setLocationId(const UI32 &locationId)
	{
		m_locationId = locationId;
	}

	UI32 SecGlobalShowMessage::getLocationId() const
	{
		return(m_locationId);
	}

	void SecGlobalShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer = findBuffer(bufferNum, size);
	}

	void SecGlobalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
	{
		this->copyBuffersFrom(prismMessage);
	}
}
