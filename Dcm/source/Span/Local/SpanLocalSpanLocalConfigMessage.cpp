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
 *   Author : ugodavar                                                     *
 **************************************************************************/

#include "Span/Local/SpanLocalLocalObjectManager.h"
#include "Span/Local/SpanLocalSpanLocalConfigMessage.h"
#include "Span/Local/SpanLocalTypes.h"
#include "ClientInterface/Span/SpanMessageDef.h"

namespace DcmNs
{

    SpanLocalSpanLocalConfigMessage::SpanLocalSpanLocalConfigMessage ()
        : PrismMessage (SpanLocalLocalObjectManager::getPrismServiceId (),SPANLOCALSPANLOCALCONFIG),
          m_capabilityProbeMsg(false)
    {
    }

    SpanLocalSpanLocalConfigMessage::SpanLocalSpanLocalConfigMessage (const UI32 &session_id,const string &source_ifName,const string &dest_ifName,const UI32 &direction,const UI32 &opCode,const UI32 &messageType,const string &description, const UI32 &src_ethernet, const UI32 &dest_ethernet,const UI32 &trill_sa, const UI32 &trill_da)
        : PrismMessage (SpanLocalLocalObjectManager::getPrismServiceId (),SPANLOCALSPANLOCALCONFIG),
        m_session_id    (session_id),
        m_source_ifName    (source_ifName),
        m_dest_ifName    (dest_ifName),
        m_direction    (direction),
        m_opCode    (opCode),
        m_messageType    (messageType),
        m_description    (description),
		m_src_ethernet  (src_ethernet),
		m_dest_ethernet (dest_ethernet),
		m_trill_sa (trill_sa),
		m_trill_da (trill_da),
		m_capabilityProbeMsg(false)
    {
    }

    SpanLocalSpanLocalConfigMessage::~SpanLocalSpanLocalConfigMessage ()
    {
    }

    void  SpanLocalSpanLocalConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_session_id,"session_id"));
        addSerializableAttribute (new AttributeString(&m_source_ifName,"source_ifName"));
        addSerializableAttribute (new AttributeString(&m_dest_ifName,"dest_ifName"));
        addSerializableAttribute (new AttributeUI32(&m_direction,"direction"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI32(&m_messageType,"messageType"));
        addSerializableAttribute (new AttributeString(&m_description,"description"));
        addSerializableAttribute (new AttributeUI32(&m_src_ethernet,"src_ethernet"));
        addSerializableAttribute (new AttributeUI32(&m_dest_ethernet,"dest_ethernet"));

        addSerializableAttribute (new AttributeUI32(&m_trill_sa,"trill_sa"));
        addSerializableAttribute (new AttributeUI32(&m_trill_da,"trill_da"));
        addSerializableAttribute (new AttributeBool(&m_capabilityProbeMsg,"capabilityProbeMsg"));
    }

    void  SpanLocalSpanLocalConfigMessage::setSession_id(const UI32 &session_id)
    {
        m_session_id  =  session_id;
    }

    UI32  SpanLocalSpanLocalConfigMessage::getSession_id() const
    {
        return (m_session_id);
    }

    void  SpanLocalSpanLocalConfigMessage::setSource_ifName(const string &source_ifName)
    {
        m_source_ifName  =  source_ifName;
    }

    string  SpanLocalSpanLocalConfigMessage::getSource_ifName() const
    {
        return (m_source_ifName);
    }

    void  SpanLocalSpanLocalConfigMessage::setDest_ifName(const string &dest_ifName)
    {
        m_dest_ifName  =  dest_ifName;
    }

    string  SpanLocalSpanLocalConfigMessage::getDest_ifName() const
    {
        return (m_dest_ifName);
    }

    void  SpanLocalSpanLocalConfigMessage::setDirection(const UI32 &direction)
    {
        m_direction  =  direction;
    }

    UI32  SpanLocalSpanLocalConfigMessage::getDirection() const
    {
        return (m_direction);
    }

    void  SpanLocalSpanLocalConfigMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SpanLocalSpanLocalConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  SpanLocalSpanLocalConfigMessage::setMessageType(const UI32 &messageType)
    {
        m_messageType  =  messageType;
    }

    UI32  SpanLocalSpanLocalConfigMessage::getMessageType() const
    {
        return (m_messageType);
    }

    void  SpanLocalSpanLocalConfigMessage::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  SpanLocalSpanLocalConfigMessage::getDescription() const
    {
        return (m_description);
    }

    void  SpanLocalSpanLocalConfigMessage::setSrcethernet(const UI32 &src_ethernet)
    {
         m_src_ethernet =  src_ethernet;
    }

    UI32  SpanLocalSpanLocalConfigMessage::getSrcethernet() const
    {
         return (m_src_ethernet);
    }

    void  SpanLocalSpanLocalConfigMessage::setDestethernet(const UI32 &dest_ethernet)
    {
         m_dest_ethernet =  dest_ethernet;
    }

    UI32  SpanLocalSpanLocalConfigMessage::getDestethernet() const
    {
         return (m_dest_ethernet);
    }

    void  SpanLocalSpanLocalConfigMessage::setTrillSA(const UI32 &trill_sa)
    {
         m_trill_sa =  trill_sa;
    }

    UI32  SpanLocalSpanLocalConfigMessage::getTrillSA() const
    {
         return (m_trill_sa);
    }

    void  SpanLocalSpanLocalConfigMessage::setTrillDA(const UI32 &trill_da)
    {
    	m_trill_da =  trill_da;
    }

    UI32  SpanLocalSpanLocalConfigMessage::getTrillDA() const
    {
         return (m_trill_da);
    }


	void  SpanLocalSpanLocalConfigMessage::setCapabilityProbeMessage(const bool &capabilityProbeMsg)
	{
		m_capabilityProbeMsg  =  capabilityProbeMsg;
	}

	bool  SpanLocalSpanLocalConfigMessage::isCapabilityProbeMessage() const
	{
	     return (m_capabilityProbeMsg);
	}

	void SpanLocalSpanLocalConfigMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,UI32 &size, void * &pBuffer)
	{
		pBuffer   =   findBuffer(bufferNum, size);
	}

	void SpanLocalSpanLocalConfigMessage::copyBufferFromClient(SpanClientLocalSpanLocalConfigMessage *m)
	{
		PrismMessage &span_client_msg = *m;
		copyBuffersFrom(span_client_msg);
		m->removeBuffer(SPAN_BUFFER);
	}
}
