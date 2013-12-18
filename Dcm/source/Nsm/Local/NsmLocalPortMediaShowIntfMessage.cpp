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
 *   .Author : bvreddy	                                                   *
 **************************************************************************/

#include "Nsm/Local/NsmLocalPortMediaShowIntfMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    NsmLocalPortMediaShowIntfMessage::NsmLocalPortMediaShowIntfMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALPORTMEDIASHOWINTERFACE)
    {
		m_cmdCode = 0;
		m_isGwRequest = true;
		m_slotId = 0;
    }

    NsmLocalPortMediaShowIntfMessage::NsmLocalPortMediaShowIntfMessage (const string &ifTypeName, const string &ifName,const UI32 &slotId, const UI32 &cmdCode,const bool isGwRequest)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALPORTMEDIASHOWINTERFACE),
        m_ifTypeName    (ifTypeName),
        m_ifName    (ifName),
        m_slotId   (slotId),
        m_cmdCode   (cmdCode),
		m_isGwRequest  (isGwRequest)
    {
    }

    NsmLocalPortMediaShowIntfMessage::~NsmLocalPortMediaShowIntfMessage ()
    {
    }

    void  NsmLocalPortMediaShowIntfMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_slotId,"slotId"));
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeString(&m_ifTypeName,"ifTypeName"));
		addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
    }

	void  NsmLocalPortMediaShowIntfMessage::setIfTypeName(const string &ifTypeName)
	{
	     m_ifTypeName  =  ifTypeName;
	}

	string  NsmLocalPortMediaShowIntfMessage::getIfTypeName() const
	{
	    return (m_ifTypeName);
	}


	void  NsmLocalPortMediaShowIntfMessage::setIfName(const string &ifName)
	{
	     m_ifName  =  ifName;
	}

	string  NsmLocalPortMediaShowIntfMessage::getIfName() const
	{
	    return (m_ifName);
	}


    void  NsmLocalPortMediaShowIntfMessage::setCmdSlotId(const UI32 &slotId)
    {
        m_slotId  =  slotId;
    }

    UI32  NsmLocalPortMediaShowIntfMessage::getCmdSlotId() const
    {
        return (m_slotId);
    }

	
    void  NsmLocalPortMediaShowIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmLocalPortMediaShowIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void NsmLocalPortMediaShowIntfMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool NsmLocalPortMediaShowIntfMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void NsmLocalPortMediaShowIntfMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size,void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void NsmLocalPortMediaShowIntfMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

}
