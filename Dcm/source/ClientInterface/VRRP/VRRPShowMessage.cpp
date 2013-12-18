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
 *   Author : Karthik A                                                    *
 **************************************************************************/

#include "ClientInterface/VRRP/VRRPShowMessage.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

    VRRPShowMessage::VRRPShowMessage()
        : DcmManagementInterfaceMessage (VRRP_SHOW_MSG)
    {
        m_mappedId = 0;
    }

    VRRPShowMessage::VRRPShowMessage(const UI32 &vrid, const string &ifName, const UI32 &cmdCode, const UI32 &showType, const UI32 &lastIndex)
        : DcmManagementInterfaceMessage (VRRP_SHOW_MSG),
        m_vrid          (vrid),
        m_if_name       (ifName),
        m_cmdCode       (cmdCode),
        m_showType       (showType),
		m_lastIndex     (lastIndex)
    {
        m_mappedId = 0;
    }

    VRRPShowMessage::~VRRPShowMessage ()
    {
    }

    void  VRRPShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32 (&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeUI32 (&m_showType, "showType"));
        addSerializableAttribute (new AttributeUI32 (&m_vrid, "vrid"));
        addSerializableAttribute (new AttributeString (&m_if_name, "ifName"));
        addSerializableAttribute (new AttributeUI32 (&m_lastIndex, "lastIndex"));
        addSerializableAttribute (new AttributeUI32 (&m_mappedId, "mappedId"));
    }

    void  VRRPShowMessage::setVrid(const UI32 &vrid)
    {
        m_vrid = vrid;
    }

    UI32 VRRPShowMessage::getVrid() const
    {
        return (m_vrid);
    }
    
    void  VRRPShowMessage::setIfName(const string &ifName)
    {
        m_if_name  =  ifName;
    }

    string  VRRPShowMessage::getIfName() const
    {
        return (m_if_name);
    }
    
    void  VRRPShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  VRRPShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  VRRPShowMessage::setShowType(const UI32 &showType)
    {
        m_showType  =  showType;
    }

    UI32  VRRPShowMessage::getShowType() const
    {
        return (m_showType);
    }

    void  VRRPShowMessage::setLastIndex(const UI32 &lastIndex)
    {
        m_lastIndex  =  lastIndex;
    }

    UI32  VRRPShowMessage::getLastIndex() const
    {
        return (m_lastIndex);
    }

    void  VRRPShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  VRRPShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    void VRRPShowMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *VRRPShowMessage::getCStructureForInputs ()
    {
        vrrp_show_input_msg_t *pInput = new vrrp_show_input_msg_t;

        pInput->vrid = m_vrid;
        strncpy (pInput->if_name, m_if_name.c_str (), 64);
        pInput->cmdCode = m_cmdCode;
        pInput->showType = m_showType;
		pInput->lastIndex = m_lastIndex;

        return (pInput);
    }

	void VRRPShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		nsm_mgmt_buffer_num_t buffNum;
		memset(&buffNum, 0, sizeof(buffNum));
		buffNum.buff.mapped_id = getMappedId();
		buffNum.buff.tag = VRRP_SHOW_MSG;
		vrrp_show_output_msg_t *msg = ((vrrp_show_output_msg_t *)pOutputCStructure);

        if (!msg) return;

		int num_records = msg->num_records;

		switch (msg->record_type) {
			case VRRP_SHOW_CMD_TYPE_DETAIL:
				addBuffer (buffNum.num, (sizeof(vrrp_show_output_msg_t) + ((num_records -1) * sizeof(vrrp_show_output_detail_msg_t))), pOutputCStructure, false);
				break;
			case VRRP_SHOW_CMD_TYPE_SUMMARY:
				addBuffer (buffNum.num, (sizeof(vrrp_show_output_msg_t) + ((num_records -1) * sizeof(vrrp_show_output_summary_msg_t))), pOutputCStructure, false);
				break;
			case VRRP_SHOW_CMD_TYPE_NONE:
				addBuffer (buffNum.num, (sizeof(vrrp_show_output_msg_t) + ((num_records -1) * sizeof(vrrp_show_output_brief_msg_t))), pOutputCStructure, false);
				break;
			case VRRP_SHOW_CMD_TYPE_NSM_DB:
				addBuffer (buffNum.num, (sizeof(vrrp_show_output_msg_t) + ((num_records -1) * sizeof(vrrp_db_nsm_show_msg_t))), pOutputCStructure, false);
				break;
			case VRRP_SHOW_CMD_TYPE_NSM_PENDING_LIST:
				addBuffer (buffNum.num, (sizeof(vrrp_show_output_msg_t) + ((num_records -1) * sizeof(vrrp_pending_list_nsm_show_msg_t))), pOutputCStructure, false);
				break;
			default:
				break;
		}
	}

	void VRRPShowMessage::dumpInputMessage()
	{
		trace(TRACE_LEVEL_DEBUG, "VRRPShowMessage::dumpInputMessage");
		trace(TRACE_LEVEL_DEBUG, string("m_vrid =") + m_vrid);
		trace(TRACE_LEVEL_DEBUG, string("m_if_name=") + m_if_name);
		trace(TRACE_LEVEL_DEBUG, string("m_cmdCode=") + m_cmdCode);
		trace(TRACE_LEVEL_DEBUG, string("m_showType=") + m_showType);
		trace(TRACE_LEVEL_DEBUG, string("m_lastIndex=") + m_lastIndex);
	}
}
