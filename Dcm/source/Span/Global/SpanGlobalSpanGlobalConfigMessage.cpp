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


#include "Span/Global/SpanGlobalObjectManager.h"
#include "Span/Global/SpanGlobalSpanGlobalConfigMessage.h"
#include "Span/Global/SpanGlobalTypes.h"
#include "ClientInterface/Span/SpanMessageDef.h"

namespace DcmNs
{
    SpanGlobalSpanGlobalConfigMessage::SpanGlobalSpanGlobalConfigMessage ()
        : ManagementInterfaceMessage (SpanGlobalObjectManager::getClassName (),SPANGLOBALSPANGLOBALCONFIG),
          m_capabilityProbeMsg(false)
    {
    }

	SpanGlobalSpanGlobalConfigMessage::SpanGlobalSpanGlobalConfigMessage (const UI32 &session_id,const string &source_ifName,const string &dest_ifName,const UI32 &direction,const UI32 &opCode,const UI32 &messageType,const string &description, const UI32 &src_ethernet, const UI32 &dest_ethernet,const UI32 &dstVlanID)
	    : ManagementInterfaceMessage (SpanGlobalObjectManager::getClassName (),SPANGLOBALSPANGLOBALCONFIG),
		 m_session_id    (session_id),
		 m_source_ifName    (source_ifName),
		 m_dest_ifName    (dest_ifName),
		 m_direction    (direction),
		 m_opCode    (opCode),
		 m_messageType    (messageType),
		 m_description    (description),
		 m_src_ethernet  (src_ethernet),
		 m_dest_ethernet (dest_ethernet),
		 m_destVlanId (dstVlanID),
		 m_capabilityProbeMsg(false)
	{
	}

	SpanGlobalSpanGlobalConfigMessage::~SpanGlobalSpanGlobalConfigMessage ()
	{
	}

	void  SpanGlobalSpanGlobalConfigMessage::setupAttributesForSerialization()
	{
	     ManagementInterfaceMessage::setupAttributesForSerialization ();
	     addSerializableAttribute (new AttributeUI32(&m_session_id,"session_id"));
	     addSerializableAttribute (new AttributeString(&m_source_ifName,"source_ifName"));
	     addSerializableAttribute (new AttributeString(&m_dest_ifName,"dest_ifName"));
	     addSerializableAttribute (new AttributeUI32(&m_direction,"direction"));
	     addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
	     addSerializableAttribute (new AttributeUI32(&m_messageType,"messageType"));
	     addSerializableAttribute (new AttributeString(&m_description,"description"));
		 addSerializableAttribute (new AttributeUI32(&m_src_ethernet,"src_ethernet"));
		 addSerializableAttribute (new AttributeUI32(&m_dest_ethernet,"dest_ethernet"));
		 addSerializableAttribute (new AttributeUI32(&m_destVlanId,"destVlanId"));
		 addSerializableAttribute (new AttributeBool(&m_capabilityProbeMsg,"capabilityProbeMsg"));
	}

	void  SpanGlobalSpanGlobalConfigMessage::setSession_id(const UI32 &session_id)
	{
	     m_session_id  =  session_id;
	}

	UI32  SpanGlobalSpanGlobalConfigMessage::getSession_id() const
	{
	     return (m_session_id);
	}

	void  SpanGlobalSpanGlobalConfigMessage::setSource_ifName(const string &source_ifName)
	{
	     m_source_ifName  =  source_ifName;
	}

	string  SpanGlobalSpanGlobalConfigMessage::getSource_ifName() const
	{
	     return (m_source_ifName);
	}

	void  SpanGlobalSpanGlobalConfigMessage::setDest_ifName(const string &dest_ifName)
	{
	     m_dest_ifName  =  dest_ifName;
	}

    string  SpanGlobalSpanGlobalConfigMessage::getDest_ifName() const
    {
         return (m_dest_ifName);
    }

	void  SpanGlobalSpanGlobalConfigMessage::setDirection(const UI32 &direction)
	{
	     m_direction  =  direction;
	}

	UI32  SpanGlobalSpanGlobalConfigMessage::getDirection() const
	{
	     return (m_direction);
	}

	void  SpanGlobalSpanGlobalConfigMessage::setOpCode(const UI32 &opCode)
	{
	     m_opCode  =  opCode;
	}

	UI32  SpanGlobalSpanGlobalConfigMessage::getOpCode() const
	{
	     return (m_opCode);
	}

	void  SpanGlobalSpanGlobalConfigMessage::setMessageType(const UI32 &messageType)
	{
	     m_messageType  =  messageType;
	}

	UI32  SpanGlobalSpanGlobalConfigMessage::getMessageType() const
	{
	     return (m_messageType);
	}

	void  SpanGlobalSpanGlobalConfigMessage::setDescription(const string &description)
	{
	     m_description  =  description;
	}

	string  SpanGlobalSpanGlobalConfigMessage::getDescription() const
	{
	     return (m_description);
	}

    void  SpanGlobalSpanGlobalConfigMessage::setSrcethernet(const UI32 &src_ethernet)
    {
         m_src_ethernet =  src_ethernet;
    }

    UI32  SpanGlobalSpanGlobalConfigMessage::getSrcethernet() const
    {
         return (m_src_ethernet);
    }

    void  SpanGlobalSpanGlobalConfigMessage::setDestethernet(const UI32 &dest_ethernet)
    {
         m_dest_ethernet =  dest_ethernet;
    }

    UI32  SpanGlobalSpanGlobalConfigMessage::getDestethernet() const
    {
         return (m_dest_ethernet);
    }

	void  SpanGlobalSpanGlobalConfigMessage::setDestVlanId(const UI32 &vlanId)
	{
		m_destVlanId  =  vlanId;
	}

	UI32  SpanGlobalSpanGlobalConfigMessage::getDestVlanId() const
	{
	     return (m_destVlanId);
	}

	void  SpanGlobalSpanGlobalConfigMessage::setCapabilityProbeMessage(const bool &capabilityProbeMsg)
	{
		m_capabilityProbeMsg  =  capabilityProbeMsg;
	}

	bool  SpanGlobalSpanGlobalConfigMessage::isCapabilityProbeMessage() const
	{
	     return (m_capabilityProbeMsg);
	}

    void SpanGlobalSpanGlobalConfigMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,UI32 &size, void * &pBuffer)
    {
        pBuffer   =   findBuffer(bufferNum, size);
    }

	void SpanGlobalSpanGlobalConfigMessage::copyAllBuffers (const PrismMessage &prismMessage)
	{
	    this->copyBuffersFrom(prismMessage);
	}
}



