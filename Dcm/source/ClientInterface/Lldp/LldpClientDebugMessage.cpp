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
 *   Author : Glenn Wenig                                           *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Lldp/LldpClientDebugMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"

namespace DcmNs
{

    LldpClientDebugMessage::LldpClientDebugMessage ()
        : DcmManagementInterfaceMessage (MSG_LLDP_DEBUG)
    {
    }

    LldpClientDebugMessage::LldpClientDebugMessage (const string &ifName, 
                                                    const UI32 &cmdCode,
                                                    const UI32 &dbgFlags,
                                                    const UI32 &mappedId,
                                                    const UI32 &lastIfIndex,
                                                    const UI32 &numRecords)
        : DcmManagementInterfaceMessage (MSG_LLDP_DEBUG),
        m_ifName    (ifName),
        m_cmdCode    (cmdCode),
        m_dbgFlags   (dbgFlags),
        m_mappedId   (mappedId),
        m_lastIfIndex(lastIfIndex),
        m_numRecords (numRecords)
    {
    }

    LldpClientDebugMessage::~LldpClientDebugMessage ()
    {
    }

    void  LldpClientDebugMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute(new AttributeUI32(&m_dbgFlags,"dbgFlags"));
        addSerializableAttribute(new AttributeUI32(&m_mappedId,"mappedId"));
        addSerializableAttribute(new AttributeUI32(&m_lastIfIndex,"lastIfIndex"));
        addSerializableAttribute(new AttributeUI32(&m_numRecords,"numRecords"));
    }


    void 
    LldpClientDebugMessage::setIfName(const string &ifName)
    {
        m_ifName = ifName;
    }

    string 
    LldpClientDebugMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  LldpClientDebugMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  LldpClientDebugMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  LldpClientDebugMessage::setDbgFlags(const UI32 &dbgFlags)
    {
        m_dbgFlags  =  dbgFlags;
    }

    UI32  LldpClientDebugMessage::getDbgFlags() const
    {
        return (m_dbgFlags);
    }

    void  LldpClientDebugMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  LldpClientDebugMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    void LldpClientDebugMessage::getOutputBufferRef (UI32 bufferNum, 
                                                    UI32 &size, 
                                                    void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }
    
    const void *LldpClientDebugMessage::getCStructureForInputs ()
    {
        lldp_debug_cmd_req_t *req_p = new lldp_debug_cmd_req_t;

        req_p->cmd_code = (lldp_debug_cmd_t) m_cmdCode;
        req_p->last_ifindex = m_lastIfIndex;
        req_p->max_records = m_numRecords;

        switch (m_cmdCode) {

            case LLDP_DEBUG_CMD_CODE_DUMP_ALL:
            case LLDP_DEBUG_CMD_CODE_SHOW_STATUS:
            {

            }
            break;

            case LLDP_DEBUG_CMD_CODE_DUMP_INTF:
            {   
                snprintf(req_p->intf.if_name, LLDP_IF_STR_MAX_LEN,
                         "te%s", m_ifName.c_str ()); 
            }
            break;
            case LLDP_DEBUG_CMD_CODE_DUMP_INTF_GI:
            {   
                snprintf(req_p->intf.if_name, LLDP_IF_STR_MAX_LEN,
                         "gi%s", m_ifName.c_str ()); 
				req_p->cmd_code = LLDP_DEBUG_CMD_CODE_DUMP_INTF;
            }
            break;
            case LLDP_DEBUG_CMD_CODE_DUMP_INTF_FO:
            {   
                snprintf(req_p->intf.if_name, LLDP_IF_STR_MAX_LEN,
                         "fo%s", m_ifName.c_str ()); 
				req_p->cmd_code = LLDP_DEBUG_CMD_CODE_DUMP_INTF;
            }
            break;

            case LLDP_DEBUG_CMD_CODE_PACKET_ALL:
            {
                req_p->debug_flags = m_dbgFlags;
            }
            break;

            case LLDP_DEBUG_CMD_CODE_PACKET_INTF:
            {   
                snprintf(req_p->intf.if_name, LLDP_IF_STR_MAX_LEN,
                         "te%s", m_ifName.c_str ()); 
                req_p->debug_flags = m_dbgFlags;
            }
			break;
            case LLDP_DEBUG_CMD_CODE_PACKET_INTF_GI:
            {   
                snprintf(req_p->intf.if_name, LLDP_IF_STR_MAX_LEN,
                         "gi%s", m_ifName.c_str ()); 
                req_p->debug_flags = m_dbgFlags;
				req_p->cmd_code = LLDP_DEBUG_CMD_CODE_PACKET_INTF;
            }
            break;
            case LLDP_DEBUG_CMD_CODE_PACKET_INTF_FO:
            {   
                snprintf(req_p->intf.if_name, LLDP_IF_STR_MAX_LEN,
                         "fo%s", m_ifName.c_str ()); 
                req_p->debug_flags = m_dbgFlags;
				req_p->cmd_code = LLDP_DEBUG_CMD_CODE_PACKET_INTF;
            }
            break;

            default:
                break;
        }

        // m_cmdCode should be set back to modified command code as it will be
        // used in loadOutputsFromCStructure function
        m_cmdCode = (UI32)req_p->cmd_code;

        return req_p;
    }

	void 
    LldpClientDebugMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        lldp_mgmt_buffer_num_t  buff_num;

        memset(&buff_num, 0, sizeof(buff_num));


        switch (m_cmdCode) { 
            case LLDP_DEBUG_CMD_CODE_DUMP_ALL:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_DEBUG_BUF_DUMP_ALL;

                lldp_debug_cmd_resp_t *resp_p =
                    (lldp_debug_cmd_resp_t*) pOutputCStructure;

                addBuffer(buff_num.num, 
                          sizeof(lldp_debug_cmd_resp_t) +
                          ((resp_p->num_records - 1) *
                           sizeof(lldp_debug_dump_resp_entry_t)), 
                          (unsigned char *)pOutputCStructure, 
                          false);
            }
            break;

            case LLDP_DEBUG_CMD_CODE_DUMP_INTF:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_DEBUG_BUF_DUMP_INTF;
                
                addBuffer(buff_num.num, 
                          sizeof(lldp_debug_cmd_resp_t), 
                          (unsigned char *)pOutputCStructure, 
                          false);                
            }
            break;

            case LLDP_DEBUG_CMD_CODE_PACKET_ALL:
            case LLDP_DEBUG_CMD_CODE_PACKET_INTF:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_DEBUG_BUF_PKT;

                addBuffer(buff_num.num, 
                          sizeof(lldp_debug_cmd_resp_t), 
                          (unsigned char *)pOutputCStructure, 
                          false);                
            }
            break;

            case LLDP_DEBUG_CMD_CODE_SHOW_STATUS:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_DEBUG_BUF_SHOW_STATUS;

                lldp_debug_status_resp_t *resp_p =
                    (lldp_debug_status_resp_t*) pOutputCStructure;

                addBuffer(buff_num.num, 
                          sizeof(lldp_debug_status_resp_t) +
                          ((resp_p->num_records - 1) *
                           sizeof(lldp_debug_status_resp_entry_t)), 
                          (unsigned char *)pOutputCStructure, 
                          false);
            }
            break;

            case LLDP_MGMT_CMD_UNKNOWN:
            {
            }
            break;

            default:
            {
            }
        }
	}

	UI32 LldpClientDebugMessage::getLastIfIndex() const
	{
	    return m_lastIfIndex;
	}

	UI32 LldpClientDebugMessage::getNumRecords() const
	{
	    return m_numRecords;
	}

	void LldpClientDebugMessage::setLastIfIndex(const UI32 lastIfIndex)
	{
	    m_lastIfIndex = lastIfIndex;
	}

	void LldpClientDebugMessage::setNumRecords(const UI32 numRecords)
	{
	    m_numRecords = numRecords;
	}
}
