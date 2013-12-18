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

#include "ClientInterface/Span/SpanShowMessage.h"
#include "ClientInterface/Span/SpanMessageDef.h"

namespace DcmNs
{

    SpanShowMessage::SpanShowMessage ()
        : DcmManagementInterfaceMessage (SPANSHOWMESSAGE)
    {
    }

    SpanShowMessage::SpanShowMessage (const UI32 &session_id,const UI32 &messageType, const UI32 &cmdCode , const UI32 &mappedId )
        : DcmManagementInterfaceMessage (SPANSHOWMESSAGE),
        m_session_id    (session_id),
        m_messageType (messageType),
        m_cmdCode   (cmdCode),
		m_mappedId  (mappedId)
    {
    }

    SpanShowMessage::~SpanShowMessage ()
    {
    }

    void  SpanShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_session_id,"session_id"));
        addSerializableAttribute (new AttributeUI32(&m_messageType,"messageType"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
    }

    void  SpanShowMessage::setSession_id(const UI32 &session_id)
    {
        m_session_id  =  session_id;
    }

    UI32  SpanShowMessage::getSession_id() const
    {
        return (m_session_id);
    }

    void  SpanShowMessage::setMessageType(const UI32 &messageType)
    {
        m_messageType = messageType;
    }

    UI32  SpanShowMessage::getMessageType() const
    {
        return (m_messageType);
    }

    void  SpanShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  SpanShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  SpanShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  SpanShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    void SpanShowMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer   =   findBuffer(bufferNum, size);
    }

    const void *SpanShowMessage::getCStructureForInputs ()
    {
        span_show_config_msg_ *pInput = (span_show_config_msg_ *)malloc (sizeof (struct span_show_config_msg_));

        pInput->m_session_id= m_session_id;
        pInput->m_messageType = m_messageType;
        pInput->cmdCode     = m_cmdCode;
        return (pInput);
    }

    void SpanShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        SpanBufferNum_t  buffNum;
        memset(&buffNum, 0, sizeof(buffNum));
        span_nsm_show_msg_info_t *msg = ((span_nsm_show_msg_info_t *)pOutputCStructure);

        if (!msg) return;

        int num_records = msg->num_records;

        switch (msg->record_type)
        {
            case SPANSHOWSPAN:
                    buffNum.buff.mappedId = this->getMappedId();
                    buffNum.buff.tag = SPANSHOWSPAN;
                    addBuffer (buffNum.num, (sizeof(span_nsm_show_msg_info_t)+((num_records - 1) * sizeof (span_nsm_show_msg_t))), pOutputCStructure, false);
	                break;
            case SPANSHOWSPANALL:
                    buffNum.buff.mappedId = this->getMappedId();
                    buffNum.buff.tag = SPANSHOWSPANALL;
                    addBuffer (buffNum.num, (sizeof(span_nsm_show_msg_info_t)+((num_records - 1) * sizeof (span_nsm_show_msg_t))), pOutputCStructure, false);
    	            break;
            case SPANSHOWPATH:
                    buffNum.buff.mappedId = this->getMappedId();
                    buffNum.buff.tag = SPANSHOWPATH;
                    addBuffer (buffNum.num, (sizeof(span_nsm_show_msg_info_t)+((num_records - 1) * sizeof (span_nsm_show_msg_t))), pOutputCStructure, false);
    	            break;
            default:
                break;
        }
    }
}

