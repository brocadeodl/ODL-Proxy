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
 *   Author : rhkumar/ranantha                                             *
 **************************************************************************/

#include "ClientInterface/AG/AgClientEnableMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "AG/Common/AgPluginTypes.h"

namespace DcmNs
{

    AgClientEnableMessage::AgClientEnableMessage ()
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_MODE_ENABLE)
    {
    }

    AgClientEnableMessage::AgClientEnableMessage (const bool &agEnable, const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_MODE_ENABLE),
        m_agEnable    (agEnable),
        m_cmdCode    (cmdCode)
    {
    }

    AgClientEnableMessage::~AgClientEnableMessage ()
    {
    }

    void  AgClientEnableMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_agEnable,"agEnable"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    }

    void  AgClientEnableMessage::setEnable(const bool &agEnable)
    {
        m_agEnable  =  agEnable;
    }

    bool  AgClientEnableMessage::getEnable() const
    {
        return (m_agEnable);
    }

    void  AgClientEnableMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  AgClientEnableMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    const void * AgClientEnableMessage::getCStructureForInputs()
    {
        ag_config_msg_t *msg_p = new ag_config_msg_t;

        msg_p->cmd_code = m_cmdCode;
        msg_p->enable = m_agEnable;
        
        return msg_p;
    }

    void
    AgClientEnableMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ag_config_msg_resp_t *resp_p = (ag_config_msg_resp_t *)pOutputCStructure;

		if (resp_p->cmd_code == AG_CONFIG_GET_ENABLE) {
			m_agEnable = resp_p->status;
		}
		

	if (resp_p)
        	addBuffer(resp_p->cmd_code, resp_p->size,
                  (unsigned char *)resp_p, false);
    }

}
