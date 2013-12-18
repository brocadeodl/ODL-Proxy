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
#include "ClientInterface/Nsm/ConfigMgmtShowMessage.h"
#include "ClientInterface/Nsm/configmgmt_dcm_def.h"


namespace DcmNs
{

    ConfigMgmtShowMessage::ConfigMgmtShowMessage ()
        : DcmManagementInterfaceMessage (MSG_CONFIGMGMT_SHOW)
    {
    }

    ConfigMgmtShowMessage::ConfigMgmtShowMessage (const UI32 &cmdCode, 
                                      const UI32 &mappedId)
        : DcmManagementInterfaceMessage (MSG_CONFIGMGMT_SHOW),
        m_cmdCode    (cmdCode),
        m_mappedId   (mappedId)

    {
    }

    ConfigMgmtShowMessage::~ConfigMgmtShowMessage ()
    {
    }

    void  ConfigMgmtShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization();
        addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute(new AttributeUI32(&m_mappedId,"mappedId"));
    }

    void  ConfigMgmtShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  ConfigMgmtShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  ConfigMgmtShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  ConfigMgmtShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }


    void ConfigMgmtShowMessage::getOutputBufferRef (UI32 bufferNum, 
                                              UI32 &size, 
                                              void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }
    
    const void *ConfigMgmtShowMessage::getCStructureForInputs ()
    {
        config_mgmt_req_t *req_p = (config_mgmt_req_t*) 
                                  calloc(1, sizeof(config_mgmt_req_t));
        switch(m_cmdCode) { 
            case CONFIGMGMT_CMD_SHOW_SYSTEM: 
            {
                //nothing much to collect here
            }
            break;

            default: 
            {
            }
        }
        req_p->cmd_code = (config_mgmt_cmd_code_t) m_cmdCode;

        return req_p;
    }

	void 
    ConfigMgmtShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        config_mgmt_buffer_num_t  buff_num;

        memset(&buff_num, 0, sizeof(buff_num));


        switch (m_cmdCode) { 
            case CONFIGMGMT_CMD_SHOW_SYSTEM:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = CONFIGMGMT_SHOW_BUF_SYSTEM;
                
				addBuffer(buff_num.num, 
                          sizeof(config_mgmt_show_system_resp_t), 
                          (unsigned char *)pOutputCStructure, 
                           false);                

            }
            break;

            default:
            {
            }
        }
           
	}
}
