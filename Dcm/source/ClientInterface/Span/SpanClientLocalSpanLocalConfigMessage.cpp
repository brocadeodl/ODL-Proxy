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

#include "ClientInterface/Span/SpanClientLocalSpanLocalConfigMessage.h"
#include "ClientInterface/Span/SpanMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    SpanClientLocalSpanLocalConfigMessage::SpanClientLocalSpanLocalConfigMessage ()
        : NsmVlanProvisionMessage (NSMCLIENTCREATESPANLOCALCONFIG)
    {
    }

    SpanClientLocalSpanLocalConfigMessage::SpanClientLocalSpanLocalConfigMessage (const UI32 &session_id,const string &source_ifName,const string &dest_ifName,const UI32 &direction,const UI32 &opCode,const UI32 &messageType,const string &description, const UI32 &src_ethernet, const UI32 &dest_ethernet)
        : NsmVlanProvisionMessage (NSMCLIENTCREATESPANLOCALCONFIG),
        m_session_id    (session_id),
        m_source_ifName    (source_ifName),
        m_dest_ifName    (dest_ifName),
        m_direction    (direction),
        m_opCode    (opCode),
        m_messageType    (messageType),
        m_description    (description),
        m_src_ethernet  (src_ethernet),
        m_dest_ethernet (dest_ethernet),
        m_rspan_vlanid (0)
    {
    }

    SpanClientLocalSpanLocalConfigMessage::~SpanClientLocalSpanLocalConfigMessage ()
    {
    }

    void  SpanClientLocalSpanLocalConfigMessage::setupAttributesForSerialization()
    {
    	NsmVlanProvisionMessage::setupAttributesForSerialization ();
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
        addSerializableAttribute (new AttributeUI32(&m_rspan_vlanid,"rspan_vlanid"));

    }

    void  SpanClientLocalSpanLocalConfigMessage::setSession_id(const UI32 &session_id)
    {
        m_session_id  =  session_id;
    }

    UI32  SpanClientLocalSpanLocalConfigMessage::getSession_id() const
    {
        return (m_session_id);
    }

    void  SpanClientLocalSpanLocalConfigMessage::setSource_ifName(const string &source_ifName)
    {
        m_source_ifName  =  source_ifName;
    }

    string  SpanClientLocalSpanLocalConfigMessage::getSource_ifName() const
    {
        return (m_source_ifName);
    }

    void  SpanClientLocalSpanLocalConfigMessage::setDest_ifName(const string &dest_ifName)
    {
        m_dest_ifName  =  dest_ifName;
    }

    string  SpanClientLocalSpanLocalConfigMessage::getDest_ifName() const
    {
        return (m_dest_ifName);
    }

    void  SpanClientLocalSpanLocalConfigMessage::setDirection(const UI32 &direction)
    {
        m_direction  =  direction;
    }

    UI32  SpanClientLocalSpanLocalConfigMessage::getDirection() const
    {
        return (m_direction);
    }

    void  SpanClientLocalSpanLocalConfigMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SpanClientLocalSpanLocalConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  SpanClientLocalSpanLocalConfigMessage::setMessageType(const UI32 &messageType)
    {
        m_messageType  =  messageType;
    }

    UI32  SpanClientLocalSpanLocalConfigMessage::getMessageType() const
    {
        return (m_messageType);
    }

    void  SpanClientLocalSpanLocalConfigMessage::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  SpanClientLocalSpanLocalConfigMessage::getDescription() const
    {
        return (m_description);
    }

    void  SpanClientLocalSpanLocalConfigMessage::setSrcethernet(const UI32 &src_ethernet)
    {
         m_src_ethernet =  src_ethernet;
    }

    UI32  SpanClientLocalSpanLocalConfigMessage::getSrcethernet() const
    {
         return (m_src_ethernet);
    }

    void  SpanClientLocalSpanLocalConfigMessage::setDestethernet(const UI32 &dest_ethernet)
    {
         m_dest_ethernet =  dest_ethernet;
    }

    UI32  SpanClientLocalSpanLocalConfigMessage::getDestethernet() const
    {
         return (m_dest_ethernet);
    }

    void  SpanClientLocalSpanLocalConfigMessage::setTrillSA(const UI32 &trill_sa)
    {
         m_trill_sa =  trill_sa;
    }

    UI32  SpanClientLocalSpanLocalConfigMessage::getTrillSA() const
    {
         return (m_trill_sa);
    }

    void  SpanClientLocalSpanLocalConfigMessage::setTrillDA(const UI32 &trill_da)
    {
    	m_trill_da =  trill_da;
    }

    UI32  SpanClientLocalSpanLocalConfigMessage::getTrillDA() const
    {
         return (m_trill_da);
    }


    void  SpanClientLocalSpanLocalConfigMessage::setRspanVlanId(const UI32 &vlanId)
    {
    	m_rspan_vlanid =  vlanId;
    }

    UI32  SpanClientLocalSpanLocalConfigMessage::getRspanVlanId() const
    {
         return (m_rspan_vlanid);
    }

    const void *SpanClientLocalSpanLocalConfigMessage::getCStructureForInputs ()
    {
        span_config_msg_t *pInput = (span_config_msg_t *)malloc (sizeof (struct span_config_msg_t));
    
        pInput->m_session_id= m_session_id;
        strncpy(pInput->m_source_ifName, m_source_ifName.c_str(),MAX_TENGIGA_PORT_NUM_LEN);
        strncpy(pInput->m_dest_ifName, m_dest_ifName.c_str(),MAX_TENGIGA_PORT_NUM_LEN);
		pInput->m_direction = m_direction;	
        pInput->m_opCode= m_opCode;
		pInput->m_dest_ethernet = m_dest_ethernet;
		pInput->m_src_ethernet = m_src_ethernet;
		strncpy(pInput->m_description, m_description.c_str(),MAX_DESCRIPTION_LENGTH);
	   	pInput->m_messageType = m_messageType;
	   	pInput->m_vlan_id = m_rspan_vlanid;
	   	pInput->m_trill_da = m_trill_da;
	   	pInput->m_trill_sa = m_trill_sa;
        return (pInput);
    }

    void SpanClientLocalSpanLocalConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
		if(m_messageType == SPAN_LOCAL_MSG_CONFIG &&
			m_opCode == SPAN_MSG_SET)
		{
			span_config_error_t *msg = ((span_config_error_t *)pOutputCStructure);

			if (msg == NULL)
				return;

			if(msg->m_state  == 2)
				addBuffer (SPAN_BUFFER, sizeof(int), msg->error_result, false);
		}
		else
		{
			NsmVlanProvisionMessage::loadOutputsFromCStructure(pOutputCStructure);
		}
     }

     void SpanClientLocalSpanLocalConfigMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,UI32 &size, void * &pBuffer)
      {
             pBuffer   =   findBuffer(bufferNum, size);
      }
}
