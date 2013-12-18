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
 *   Author : Amulya Makam                                                 *
 **************************************************************************/

#include "Nsm/Local/NsmLocalShowEcmpMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

	NsmLocalShowEcmpMessage::NsmLocalShowEcmpMessage ()
		: PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALSHOWECMP)
		{
			m_cmdCode = 0;
			m_isGwRequest = true;
		}

	NsmLocalShowEcmpMessage::NsmLocalShowEcmpMessage (const UI32 &cmdCode,const UI32 &rbridgeId,const bool &isGwRequest)
		: PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALSHOWECMP),
		m_cmdCode   (cmdCode),
		m_rbridgeId (rbridgeId),
		m_isGwRequest(isGwRequest)
		{
		}

	NsmLocalShowEcmpMessage::~NsmLocalShowEcmpMessage ()
	{
	}

	void  NsmLocalShowEcmpMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_rbridgeId,"rbridgeId"));
		addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
	}

	void  NsmLocalShowEcmpMessage::setCmdCode(const UI32 &cmdCode)
	{
		m_cmdCode  =  cmdCode;
	}
	UI32  NsmLocalShowEcmpMessage::getCmdCode() const
	{
		return (m_cmdCode);
	}

	void  NsmLocalShowEcmpMessage::setRbridgeId(const UI32 &rbridgeId)
	{
		m_rbridgeId  =  rbridgeId;
	}

	UI32  NsmLocalShowEcmpMessage::getRbridgeId() const
	{
		return (m_rbridgeId);
	}

	void NsmLocalShowEcmpMessage::setIsGwRequest(const bool &isGwRequest)
	{
		m_isGwRequest = isGwRequest;
	}

	bool NsmLocalShowEcmpMessage::getIsGwRequest() const
	{
		return (m_isGwRequest);
	}

	void NsmLocalShowEcmpMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size,void *&pBuffer)
	{
		pBuffer = findBuffer(bufferNum, size);
	}

	void NsmLocalShowEcmpMessage::copyAllBuffers (const PrismMessage &prismMessage)
	{
		this->copyBuffersFrom(prismMessage);
	}
}

