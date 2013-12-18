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

#include "ThresholdMonitor/Global/SfpGlobalShowMessage.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalObjectManager.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{
	SfpGlobalShowMessage::SfpGlobalShowMessage ()
		: PrismMessage (ThresholdMonitorGlobalObjectManager::getPrismServiceId (), SFPGLOBALSHOWMESSAGE)
	{
	}

	SfpGlobalShowMessage::SfpGlobalShowMessage (const UI32 &areaName, const string &ifName)
		: PrismMessage (ThresholdMonitorGlobalObjectManager::getPrismServiceId (), SFPGLOBALSHOWMESSAGE),
	m_areaName (areaName),
	m_ifName (ifName)
//	m_locationId (locationId)
//	m_IsGwRequest (IsGwRequest)
	{
	}

	SfpGlobalShowMessage::~SfpGlobalShowMessage ()
	{
	}

	void SfpGlobalShowMessage::setupAttributesForSerialization ()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_areaName,"areaName"));
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeUI32(&m_locationId,"locationId"));
		addSerializableAttribute (new AttributeBool(&m_IsGwRequest,"IsGwRequest"));
	}

	void SfpGlobalShowMessage::setAreaName (const UI32 &areaName)
	{
		m_areaName = areaName;
	}

	UI32 SfpGlobalShowMessage::getAreaName () const
	{
		return m_areaName;
	}

	void SfpGlobalShowMessage::setIfName (const string &ifName)
	{
		m_ifName = ifName;
	}

	string SfpGlobalShowMessage::getIfName () const
	{
		return m_ifName;
	}

	void SfpGlobalShowMessage::setLocationId (const UI32 &locationId)
	{
		m_locationId = locationId;
	}

	UI32 SfpGlobalShowMessage::getLocationId () const
	{
		return m_locationId;
	}

	void SfpGlobalShowMessage::setIsGwRequest (const bool &IsGwRequest)
	{
		m_IsGwRequest = IsGwRequest;
	}

	bool SfpGlobalShowMessage::getIsGwRequest () const
	{
		return m_IsGwRequest;
	}

	void SfpGlobalShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer = findBuffer(bufferNum, size);
	}

	void SfpGlobalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
	{
		this->copyBuffersFrom(prismMessage);
	}

}
