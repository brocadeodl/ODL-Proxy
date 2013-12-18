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

#include "ClientInterface/Nsm/LacpShowMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    LacpShowMessage::LacpShowMessage ()
        : PoPaginationMessage (LACP_SHOW)
    {
		m_ifType = IF_TYPE_INVALID;
    }

    LacpShowMessage::LacpShowMessage (const string &ifName, const UI32 &poNum, const UI32 &cmdCode, const UI32 &mappedId)
        : PoPaginationMessage (LACP_SHOW),
        m_ifName    (ifName),
        m_poNum    	(poNum),
        m_cmdCode    (cmdCode),
        m_mappedId    (mappedId)
    {
		m_ifType = IF_TYPE_INVALID;
    }

    LacpShowMessage::~LacpShowMessage ()
    {
    }

    void  LacpShowMessage::setupAttributesForSerialization()
    {
    	PoPaginationMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_poNum,"poNum"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    }

    void  LacpShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  LacpShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
	void  LacpShowMessage::setPoNum(const UI32 &poNum)
    {
        m_poNum  =  poNum;
    }

    UI32  LacpShowMessage::getPoNum() const
    {
        return (m_poNum);
    }

    void  LacpShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  LacpShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    
	void  LacpShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  LacpShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    void  LacpShowMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  LacpShowMessage::getIfType() const
    {
        return (m_ifType);
    }
    
    void LacpShowMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
	
	void LacpShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    
    const void *LacpShowMessage::getCStructureForInputs ()
    {
        LacpShow_InputMsg_t *pInput = new LacpShow_InputMsg_t;
        
		trace (TRACE_LEVEL_DEBUG, "Entering LacpShowMessage::getCStructureForInputs\n");

        strncpy (pInput->if_name, m_ifName.c_str (), 64);
		pInput->if_name[63] = '\0';
        pInput->po_num      = m_poNum;
        pInput->cmdCode     = m_cmdCode;
		pInput->ifType      = m_ifType;
		pInput->startId		= m_startPoId;
		pInput->endId		= m_endPoId;

        return (pInput);
    }

	void LacpShowMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
        lacp_show_msg_t *msg = ((lacp_show_msg_t *)pOutputCStructure);
		int num_records = msg->num_records;
        lacp_mgmt_buffer_num_t  buff_num;
        
		trace (TRACE_LEVEL_DEBUG, "Entering LacpShowMessage::loadOutputsFromCStructure\n");
        
		memset(&buff_num, 0, sizeof(buff_num));
        buff_num.buff.mapped_id = this->getMappedId();
        buff_num.buff.tag = LACP_SHOW;

		/* Set LacpMessageBufferId */
		switch (msg->record_type) {
			case LACPSHOWCOUNTER: {
        		addBuffer (buff_num.num, (sizeof(lacp_show_msg_t)+ ((num_records - 1) * sizeof (lacp_show_counter_output_msg_t))), pOutputCStructure, false);
				break;
			}
			case LACPSHOWSYSID: {
        		addBuffer (buff_num.num, (sizeof(lacp_show_msg_t)+ ((num_records - 1) * sizeof (lacp_show_sysid_output_msg_t))), pOutputCStructure, false);
				break;
			}
			case LACPSHOWPORTPORTCHANNEL: {
        		addBuffer (buff_num.num, (sizeof(lacp_show_msg_t)+ ((num_records - 1) * sizeof (lacp_show_portpo_output_msg_t))), pOutputCStructure, false);
				break;
			}
			case LACPSHOWPORTCHANNELDETAILS: {
        		addBuffer (buff_num.num, (sizeof(lacp_show_msg_t)+ ((num_records - 1) * sizeof (lacp_show_podetails_output_msg_t))), pOutputCStructure, false);
				break;
			}
			case LACPSHOWDEBUG: {
				addBuffer(buff_num.num, (sizeof(lacp_show_msg_t) + ((num_records - 1) * sizeof (lacp_show_debug_output_msg_t))), pOutputCStructure , false);
				break;
			}
			default :
				break;
		}
	}
}
