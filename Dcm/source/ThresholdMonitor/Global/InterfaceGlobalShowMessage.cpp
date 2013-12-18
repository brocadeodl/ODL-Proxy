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
 *   Author : Yugant Chandrakar                                            *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"

#include "ThresholdMonitor/Global/InterfaceGlobalShowMessage.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalObjectManager.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{
	InterfaceGlobalShowMessage::InterfaceGlobalShowMessage ()
		: PrismMessage (ThresholdMonitorGlobalObjectManager::getPrismServiceId (), INTERFACEGLOBALSHOWMESSAGE)
	{
	}

	InterfaceGlobalShowMessage::InterfaceGlobalShowMessage (const UI32 &areaName, const string &ifName)
		: PrismMessage (ThresholdMonitorGlobalObjectManager::getPrismServiceId (), INTERFACEGLOBALSHOWMESSAGE),
	m_areaName (areaName),
	m_ifName (ifName)
	{
	}

	InterfaceGlobalShowMessage::~InterfaceGlobalShowMessage ()
	{
	}

	void InterfaceGlobalShowMessage::setupAttributesForSerialization ()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_areaName,"areaName"));
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeUI32(&m_locationId,"locationId"));
		addSerializableAttribute (new AttributeBool(&m_IsGwRequest,"IsGwRequest"));
	}

	void InterfaceGlobalShowMessage::setAreaName (const UI32 &areaName)
	{
		m_areaName = areaName;
	}

	UI32 InterfaceGlobalShowMessage::getAreaName () const
	{
		return m_areaName;
	}

	void InterfaceGlobalShowMessage::setIfName (const string &ifName)
	{
		m_ifName = ifName;
	}

	string InterfaceGlobalShowMessage::getIfName () const
	{
		return m_ifName;
	}

	void InterfaceGlobalShowMessage::setLocationId (const UI32 &locationId)
	{
		m_locationId = locationId;
	}

	UI32 InterfaceGlobalShowMessage::getLocationId () const
	{
		return m_locationId;
	}

	void InterfaceGlobalShowMessage::setIsGwRequest (const bool &IsGwRequest)
	{
		m_IsGwRequest = IsGwRequest;
	}

	bool InterfaceGlobalShowMessage::getIsGwRequest () const
	{
		return m_IsGwRequest;
	}

	void InterfaceGlobalShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer = findBuffer(bufferNum, size);
	}

	void InterfaceGlobalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
	{
		this->copyBuffersFrom(prismMessage);
	}

}
