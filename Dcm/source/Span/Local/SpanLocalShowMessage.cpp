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

#include "Span/Local/SpanLocalLocalObjectManager.h"
#include "Span/Local/SpanLocalShowMessage.h"
#include "Span/Local/SpanLocalTypes.h"
#include "ClientInterface/Span/SpanMessageDef.h"

namespace DcmNs
{
	SpanLocalShowMessage::SpanLocalShowMessage ()
    	  : PrismMessage (SpanLocalLocalObjectManager::getPrismServiceId (),SPANLOCALSHOW)
		  {
		  }

	SpanLocalShowMessage::SpanLocalShowMessage (const UI32 &session_id,const UI32 &messageType,const UI32 &cmdCode, bool &isGwRequest)
	      : PrismMessage (SpanLocalLocalObjectManager::getPrismServiceId (),SPANLOCALSHOW),
	      m_session_id    (session_id),
	      m_messageType    (messageType),
		  m_cmdCode    (cmdCode),
	      m_isGwRequest    (isGwRequest)
		  {
		  }

	SpanLocalShowMessage::~SpanLocalShowMessage ()
	{
	}

    void  SpanLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_session_id,"session_id"));
		addSerializableAttribute (new AttributeUI32(&m_messageType,"messageType"));
		addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_isGwRequest,"isGwRequest"));
    }

	void  SpanLocalShowMessage::setSession_id(const UI32 &session_id)
	{
	    m_session_id  =  session_id;
	}

	UI32  SpanLocalShowMessage::getSession_id() const
	{
		return (m_session_id);
	}

	void  SpanLocalShowMessage::setCmdCode(const UI32 &cmdCode)
	{
	    m_cmdCode  =  cmdCode;
	}

	UI32  SpanLocalShowMessage::getCmdCode() const
	{
	    return (m_cmdCode);
	}

	void  SpanLocalShowMessage::setMessageType(const UI32 &messageType)
	{
	    m_messageType  =  messageType;
	}

	UI32  SpanLocalShowMessage::getMessageType() const
	{
	    return (m_messageType);
	}
	
	void  SpanLocalShowMessage::setIsGwRequest(bool &isGwRequest)
    {
        m_isGwRequest  =  isGwRequest;
    }

    UI32  SpanLocalShowMessage::getIsGwRequest() const
    {
        return (m_isGwRequest);
    }
	
	void SpanLocalShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
	    pBuffer = findBuffer(bufferNum, size);
	}

	void SpanLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
	{
	    this->copyBuffersFrom(prismMessage);
	}
		
}

