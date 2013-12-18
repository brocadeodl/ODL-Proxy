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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Rmon/RmonShowMessage.h"
#include "ClientInterface/Rmon/rmon_common.h"


namespace DcmNs
{

    RmonShowMessage::RmonShowMessage ()
        : DcmManagementInterfaceMessage (RMONSHOW)
    {
    }

    RmonShowMessage::RmonShowMessage (const SI32 &index, const bool &showAll, const bool &showBrief, const UI32 &cmdCode, 
                                      const UI32 &mappedId, const SI32 & startIndex, const SI32 & endIndex)
        : DcmManagementInterfaceMessage (RMONSHOW),
        m_index    (index),
		m_showAll (showAll),
        m_showBrief (showBrief),
        m_cmdCode    (cmdCode),
        m_mappedId   (mappedId),
    	m_startIndex (startIndex),
    	m_endIndex (endIndex)
    {
    }

    RmonShowMessage::~RmonShowMessage ()
    {
    }

    void  RmonShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization();
        addSerializableAttribute(new AttributeSI32(&m_index,"index"));
		addSerializableAttribute(new AttributeBool(&m_showBrief,"showBrief"));
        addSerializableAttribute(new AttributeBool (&m_showAll,"showAll"));
        addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute(new AttributeUI32(&m_mappedId,"mappedId"));
        addSerializableAttribute(new AttributeSI32(&m_startIndex,"startIndex"));
		addSerializableAttribute(new AttributeSI32(&m_endIndex,"endIndex"));
		addSerializableAttribute(new AttributeSI32(&m_startSampleIndex,"startSampleIndex"));
		addSerializableAttribute(new AttributeSI32(&m_endSampleIndex,"endSampleIndex"));
    }

    void  RmonShowMessage::setIndex(const SI32 &index)
    {
        m_index  =  index;
    }

    SI32  RmonShowMessage::getIndex() const
    {
        return (m_index);
    }

	void  RmonShowMessage::setShowAll(const bool &showAll)
    {
        m_showAll =  showAll;
    }

    bool RmonShowMessage::getShowAll() const
    {
        return (m_showAll);
    }

    void  RmonShowMessage::setShowBrief(const bool &showBrief)
    {
        m_showBrief =  showBrief;
    }

    bool RmonShowMessage::getShowBrief() const
    {
        return (m_showBrief);
    }


    void  RmonShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  RmonShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  RmonShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  RmonShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    SI32 RmonShowMessage::getEndIndex() const
	{
		return m_endIndex;
	}

	SI32 RmonShowMessage::getStartIndex() const
	{
		return m_startIndex;
	}

	void RmonShowMessage::setEndIndex(SI32 endIndex)
	{
		m_endIndex = endIndex;
	}

	SI32 RmonShowMessage::getEndSampleIndex() const
	{
		return m_endSampleIndex;
	}

	SI32 RmonShowMessage::getStartSampleIndex() const
	{
		return m_startSampleIndex;
	}

	void RmonShowMessage::setEndSampleIndex(SI32 endSampleIndex)
	{
		m_endSampleIndex = endSampleIndex;
	}

	void RmonShowMessage::setStartSampleIndex(SI32 startSampleIndex)
	{
		m_startSampleIndex = startSampleIndex;
	}

	void RmonShowMessage::setStartIndex(SI32 startIndex)
	{
		m_startIndex = startIndex;
	}

    void RmonShowMessage::getOutputBufferRef (UI32 bufferNum, 
                                              UI32 &size, 
                                              void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }
    
    const void *RmonShowMessage::getCStructureForInputs ()
    {
        rmon_show_input_msg_t *req_p = (rmon_show_input_msg_t*)(((calloc(1, sizeof (rmon_show_input_msg_t)))));
        req_p->index = m_index;
        req_p->show_all = m_showAll;
        req_p->show_brief = m_showBrief;
        req_p->command_code = m_cmdCode;
        req_p->rmonPaginationAttr.startIndex = m_startIndex;
        req_p->rmonPaginationAttr.endIndex = m_endIndex;
        req_p->rmonPaginationAttr.startSampleIndex = m_startSampleIndex;
		req_p->rmonPaginationAttr.endSampleIndex = m_endSampleIndex;
        return req_p;
    }

    void RmonShowMessage::loadOutputsFromCStructure(const void *pOutputCStructure)
    {
        rmon_mgmt_buffer_num_t buffNum;
        memset(&buffNum, 0, sizeof (buffNum));
        rmon_show_output_msg_t *msg = ((rmon_show_output_msg_t*)(((pOutputCStructure))));
        int num_records = msg->num_records;
        switch (msg->record_type){
            case SHOW_RMON_LOGS:
                {
                    buffNum.buff.mapped_id = this->getMappedId();
                    buffNum.buff.tag = SHOW_RMON_LOGS;
                    addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_show_logs_message_t))), pOutputCStructure, false);
                    break;
            }case SHOW_RMON_STATISTICS:
            {
                buffNum.buff.mapped_id = this->getMappedId();
                buffNum.buff.tag = SHOW_RMON_STATISTICS;
                addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_show_statistics_message_t))), pOutputCStructure, false);
                break;
        }
    case SHOW_RMON_EVENTS:
        {
            buffNum.buff.mapped_id = this->getMappedId();
            buffNum.buff.tag = SHOW_RMON_EVENTS;
            addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_show_events_message_t))), pOutputCStructure, false);
            break;
    }
case SHOW_RMON_EVENTS_BRIEF:
    {
        buffNum.buff.mapped_id = this->getMappedId();
        buffNum.buff.tag = SHOW_RMON_EVENTS_BRIEF;
        addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_show_events_message_t))), pOutputCStructure, false);
        break;
}
case SHOW_RMON_ALARMS:
    {
        buffNum.buff.mapped_id = this->getMappedId();
        buffNum.buff.tag = SHOW_RMON_ALARMS;
        addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_show_alarms_message_t))), pOutputCStructure, false);
        break;
}
case SHOW_RMON_ALARMS_BRIEF:
    {
        buffNum.buff.mapped_id = this->getMappedId();
        buffNum.buff.tag = SHOW_RMON_ALARMS_BRIEF;
        addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_show_alarms_message_t))), pOutputCStructure, false);
        break;
}
case SHOW_RMON_STATISTICS_BRIEF:
    {
        buffNum.buff.mapped_id = this->getMappedId();
        buffNum.buff.tag = SHOW_RMON_STATISTICS_BRIEF;
        addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_show_statistics_brief_message_t))), pOutputCStructure, false);
        break;
}
case SHOW_RMON_HISTORY:
    {
        buffNum.buff.mapped_id = this->getMappedId();
        buffNum.buff.tag = SHOW_RMON_HISTORY;
        addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_show_history_message_t))), pOutputCStructure, false);
        break;
}
case SHOW_RMON_HISTORY_STATISTICS:
    {
        buffNum.buff.mapped_id = this->getMappedId();
        buffNum.buff.tag = SHOW_RMON_HISTORY_STATISTICS;
        addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_show_history_statistics_message_t))), pOutputCStructure, false);
        break;
}
case GET_ALL_RMON_STATS_ID:
    {
        buffNum.buff.mapped_id = this->getMappedId();
        buffNum.buff.tag = GET_ALL_RMON_STATS_ID;
        addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_get_stats_id_output_t))), pOutputCStructure, false);
        break;
}
case GET_ALL_RMON_HISTORY_ID:
    {
        buffNum.buff.mapped_id = this->getMappedId();
        buffNum.buff.tag = GET_ALL_RMON_HISTORY_ID;
        addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_get_history_id_output_t))), pOutputCStructure, false);
        break;
}
case GET_ALL_RMON_EVENTS_ID:
    {
        buffNum.buff.mapped_id = this->getMappedId();
        buffNum.buff.tag = GET_ALL_RMON_EVENTS_ID;
        addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_get_event_id_output_t))), pOutputCStructure, false);
        break;
}
case GET_ALL_RMON_ALARMS_ID:
    {
        buffNum.buff.mapped_id = this->getMappedId();
        buffNum.buff.tag = GET_ALL_RMON_ALARMS_ID;
        addBuffer(buffNum.num, (sizeof (rmon_show_output_msg_t) + ((num_records - 1) * sizeof (rmon_get_alarm_id_output_t))), pOutputCStructure, false);
        break;
}
default:
    break;
}

}

	/*RmonPaginationAttributes RmonShowMessage::getRmonPaginationAttr() const
	{
		return m_rmonPaginationAttr;
	}

	void RmonShowMessage::setRmonPaginationAttr(RmonPaginationAttributes rmonPaginationAttr)
	{
		m_rmonPaginationAttr = rmonPaginationAttr;
	}*/
}
