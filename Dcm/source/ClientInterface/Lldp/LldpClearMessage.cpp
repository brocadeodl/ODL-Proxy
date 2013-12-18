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
#include "ClientInterface/Lldp/LldpClearMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/DcmMemUtil.h"
namespace DcmNs
{

    LldpClearMessage::LldpClearMessage ()
        : DcmManagementInterfaceMessage (MSG_LLDP_CLEAR)
    {
		m_interface = "tengigabitethernet";
    }

    LldpClearMessage::LldpClearMessage (const string &ifName,const UI32 &cmdCode,const UI32 &mappedId)
        : DcmManagementInterfaceMessage (MSG_LLDP_CLEAR),
        m_ifName    (ifName),
        m_cmdCode    (cmdCode),
        m_mappedId   (mappedId),
		m_interface  ("tengigabitethernet")
    {
    }

    LldpClearMessage::~LldpClearMessage ()
    {
    }

    void  LldpClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_mappedId,"mappedId"));
        addSerializableAttribute (new AttributeString(&m_interface,"interface"));
    }

    void  LldpClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  LldpClearMessage::getIfName() const
    {
        return (m_ifName);
    }
    void  LldpClearMessage::setInterface(const string &interface)
    {
        m_interface =  interface;
    }

    string  LldpClearMessage::getInterface() const
    {
        return (m_interface);
    }

    void  LldpClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  LldpClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    
	void  LldpClearMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  LldpClearMessage::getMappedId() const
    {
        return (m_mappedId);
    }


    void LldpClearMessage::getOutputBufferRef (UI32 bufferNum, 
                                              UI32 &size, 
                                              void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }
    
    const void *LldpClearMessage::getCStructureForInputs ()
    {
		string twoTupleName = "";

        lldp_mgmt_req_t *req_p = (lldp_mgmt_req_t*) 
                                  calloc(1, sizeof(lldp_mgmt_req_t));
	
		if (NsmUtils::isValidThreeTuple(m_ifName)) {
            NsmUtils::getTwoTupleIfName(m_ifName, twoTupleName);
        } else {
            twoTupleName = m_ifName;
        }

        switch(m_cmdCode) { 

            case LLDP_MGMT_CMD_CLEAR_NBR:
            {
                //nothing much to copy here
            }
            break;

            case LLDP_MGMT_CMD_CLEAR_NBR_INTF:
            {   
				strncpy(req_p->entry.intf.interface, m_interface.c_str(), LLDP_IF_STR_MAX_LEN);
                strncpy(req_p->entry.intf.if_name, 
                        twoTupleName.c_str (), LLDP_IF_STR_MAX_LEN); 
            }
            break;

            case LLDP_MGMT_CMD_CLEAR_STATS:
            {
                //nothing much to copy here
            }
            break;

            case LLDP_MGMT_CMD_CLEAR_STATS_INTF:
            {   
				strncpy(req_p->entry.intf.interface, m_interface.c_str(), LLDP_IF_STR_MAX_LEN);
                strncpy(req_p->entry.intf.if_name, 
                        twoTupleName.c_str (), LLDP_IF_STR_MAX_LEN); 
            }
            break;

            default: 
            {
            }
        }
        req_p->cmd_code = (lldp_mgmt_cmd_code_t) m_cmdCode;

        return req_p;
    }

	void 
    LldpClearMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        lldp_mgmt_buffer_num_t  buff_num;

        memset(&buff_num, 0, sizeof(buff_num));

		switch (m_cmdCode) { 
            case LLDP_MGMT_CMD_CLEAR_NBR:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_MGMT_CLEAR_BUF_NBR;

                addBuffer(buff_num.num, 
                          sizeof(lldp_mgmt_clear_resp_t), 
                          (unsigned char *)pOutputCStructure, 
                           false);                
            }
            break;
            
			case LLDP_MGMT_CMD_CLEAR_NBR_INTF:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_MGMT_CLEAR_BUF_NBR_INTF;

                addBuffer(buff_num.num, 
                          sizeof(lldp_mgmt_clear_resp_t), 
                          (unsigned char *)pOutputCStructure, 
                           false);                
            }
            break;
            case LLDP_MGMT_CMD_CLEAR_STATS:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_MGMT_CLEAR_BUF_STATS;

                addBuffer(buff_num.num, 
                          sizeof(lldp_mgmt_clear_resp_t), 
                          (unsigned char *)pOutputCStructure, 
                           false);                
            }
            break;

            case LLDP_MGMT_CMD_CLEAR_STATS_INTF:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_MGMT_CLEAR_BUF_STATS_INTF;

                addBuffer(buff_num.num, 
                          sizeof(lldp_mgmt_clear_resp_t), 
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
}
