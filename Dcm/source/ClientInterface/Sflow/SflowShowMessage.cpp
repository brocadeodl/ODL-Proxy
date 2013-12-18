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

#include "ClientInterface/Sflow/SflowShowMessage.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"

namespace DcmNs
{

	SflowShowMessage::SflowShowMessage()
		: DcmManagementInterfaceMessage (SFLOWSHOWMESSAGE),
		m_ifName	(),
		m_cmdCode	(SFLOW_OUT_OF_RANGE_VALUE),
		m_mappedId	(0),
        m_interface ("tengigabitethernet")
    {
    }

	SflowShowMessage::SflowShowMessage(const string &ifName, const UI32 &cmdCode, const UI32 &mappedId)
		: DcmManagementInterfaceMessage (SFLOWSHOWMESSAGE),
		m_ifName	(ifName),
		m_cmdCode	(cmdCode),
		m_mappedId	(mappedId),
        m_interface ("tengigabitethernet")
    {
    }

    SflowShowMessage::~SflowShowMessage ()
    {
    }

    void  SflowShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeUI8(&m_mappedId,"mappedId"));
        addSerializableAttribute (new AttributeString(&m_interface,"interface"));
    }

    void  SflowShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  SflowShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  SflowShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  SflowShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	void  SflowShowMessage::setMappedId(const UI8 &mappedId)
	{
		m_mappedId  =  mappedId;
	}

	UI8  SflowShowMessage::getMappedId() const
	{
		return (m_mappedId);
	}

    void SflowShowMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer   =   findBuffer(bufferNum, size);
    }

    void  SflowShowMessage::setInterface (const string &interface)
    {
        m_interface  =  interface;
    }

    string  SflowShowMessage::getInterface() const
    {
        return (m_interface);
    }

    
 	void SflowShowMessage::setIsGwRequest (bool &isGwRequest)
	{
		m_isGwRequest = isGwRequest;
	}

	bool SflowShowMessage::getIsGwRequest () const
	{
		return (m_isGwRequest);
	}

	void SflowShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer = findBuffer(bufferNum, size);
	}
    const void *SflowShowMessage::getCStructureForInputs ()
    {
        sflow_show_input_msg_t *pInput = new sflow_show_input_msg_t;

        strncpy (pInput->if_name, m_ifName.c_str (), 64);
        strncpy (pInput->interface, m_interface.c_str (), 64);
        pInput->cmdCode     = m_cmdCode;

        return (pInput);
    }

    void SflowShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
		SflowBufferNum_t  buffNum;
		memset(&buffNum, 0, sizeof(buffNum));
        sflow_show_msg_t *msg = ((sflow_show_msg_t *)pOutputCStructure);

        if (!msg) return;

		int num_records = msg->num_records;
		switch (msg->record_type)
		{
			case SFLOWSHOWSFLOW:
				buffNum.buff.mappedId = m_mappedId;
				buffNum.buff.slotId = msg->slot_id;
				buffNum.buff.tag = SFLOWSHOWSFLOW;
				addBuffer (buffNum.num, (sizeof(sflow_show_msg_t)+ ((num_records - 1) * sizeof (sflow_show_interface_msg_t))), pOutputCStructure, false);
				break;
			case SFLOWSHOWSFLOWALL:
				buffNum.buff.mappedId = m_mappedId;
				buffNum.buff.slotId = msg->slot_id;
				buffNum.buff.tag = SFLOWSHOWSFLOWALL;
				addBuffer (buffNum.num, (sizeof(sflow_show_msg_t)+ ((num_records - 1) * sizeof (sflow_show_interface_msg_t))), pOutputCStructure, false);
				break;
			case SFLOWSHOWSFLOWINTERFACE:
				buffNum.buff.mappedId = m_mappedId;
				buffNum.buff.slotId = msg->slot_id;
				buffNum.buff.tag = SFLOWSHOWSFLOWINTERFACE;
				addBuffer (buffNum.num, (sizeof(sflow_show_msg_t)+ ((num_records - 1) * sizeof (sflow_show_interface_msg_t))), pOutputCStructure, false);
				break;
			default:
				break;
		}	
    }
}
