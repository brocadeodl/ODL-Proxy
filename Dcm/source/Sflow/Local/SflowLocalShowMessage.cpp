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
 *   Author : Venkat                                                       *
 **************************************************************************/

#include "Sflow/Local/SflowLocalObjectManager.h"
#include "Sflow/Local/SflowTypes.h"
#include "Sflow/Local/SflowLocalShowMessage.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"

namespace DcmNs
{

	SflowLocalShowMessage::SflowLocalShowMessage () : PrismMessage (SflowLocalObjectManager::getPrismServiceId (), SFLOWLOCALSHOW)
	{
        m_ifName = "";
        m_cmdCode = SFLOW_OUT_OF_RANGE_VALUE;
        m_isGwRequest = false;
		m_interface = "tengigabitethernet";
	}

	SflowLocalShowMessage::SflowLocalShowMessage (const string &ifName, const UI32 &cmdCode, bool &isGwRequest)
		: PrismMessage (SflowLocalObjectManager::getPrismServiceId (), SFLOWLOCALSHOW),
		m_ifName       (ifName),
		m_cmdCode      (cmdCode),
		m_isGwRequest  (isGwRequest),
		m_interface   ("tengigabitethernet")
	{

	}

	SflowLocalShowMessage::~SflowLocalShowMessage ()
	{
	}

	void  SflowLocalShowMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
		addSerializableAttribute(new AttributeString(&m_interface,"interface"));
		addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
	}

	void  SflowLocalShowMessage::setIfName(const string &ifName)
	{
		m_ifName  =  ifName;
	}

	string  SflowLocalShowMessage::getIfName() const
	{
		return (m_ifName);
	}

	void  SflowLocalShowMessage::setCmdCode(const UI32 &cmdCode)
	{
		m_cmdCode  =  cmdCode;
	}

	UI32  SflowLocalShowMessage::getCmdCode() const
	{
		return (m_cmdCode);
	}

 	void SflowLocalShowMessage::setIsGwRequest (bool &isGwRequest)
	{
		m_isGwRequest = isGwRequest;
	}

	bool SflowLocalShowMessage::getIsGwRequest () const
	{
		return (m_isGwRequest);
	}

	void SflowLocalShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer = findBuffer(bufferNum, size);
	}

	void SflowLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage) 
	{
		this->copyBuffersFrom(prismMessage);
	}
	void  SflowLocalShowMessage::setInterface (const string &interface)
	{
		m_interface  =  interface;
	}

	string  SflowLocalShowMessage::getInterface() const
	{
		return (m_interface);
	}
	
	void  SflowLocalShowMessage::setMappedId(const UI32 &mappedId)
	{
		m_mappedId  =  mappedId;
	}

	UI32  SflowLocalShowMessage::getMappedId() const
	{
		return (m_mappedId);
	}

}
