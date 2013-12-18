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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : rhkumar                                                      *
 **************************************************************************/

#include "ClientInterface/AG/AgClientShowMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "AG/Common/AgPluginTypes.h"

namespace DcmNs
{

    AgClientShowMessage::AgClientShowMessage ()
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_SHOW_AG)
    {
    }

    AgClientShowMessage::AgClientShowMessage (const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_SHOW_AG),
        m_cmdCode    (cmdCode)
    {
    }

    AgClientShowMessage::~AgClientShowMessage ()
    {
    }

    void  AgClientShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32Vector(&m_elements,"elements"));
    }

    void  AgClientShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  AgClientShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  AgClientShowMessage::setElements(const vector<UI32> &elements)
    {
        m_elements  =  elements;
    }

    vector<UI32>  AgClientShowMessage::getElements() const
    {
        return (m_elements);
    }

    const void * AgClientShowMessage::getCStructureForInputs()
    {
        vector<UI32>::iterator iter;
        ag_show_msg_t *msg_p = (ag_show_msg_t *) calloc (1, sizeof(ag_show_msg_t) + (m_elements.size() * sizeof(UI32)));
        UI32 count = 0;

        msg_p->cmd_code = m_cmdCode;
        msg_p->nb_elem = m_elements.size();
        msg_p->elements = (UI32 *)((char *) msg_p + sizeof(ag_show_msg_t));
        for(iter = m_elements.begin(); iter != m_elements.end(); iter++) {
            msg_p->elements[count++] = *iter;
        }
        
        return msg_p;
    }

    void
    AgClientShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ag_show_msg_resp_t *resp_p = (ag_show_msg_resp_t *)pOutputCStructure;

	if (resp_p)
        	addBuffer(resp_p->cmd_code, resp_p->size,
                  (unsigned char *)resp_p, false);
    }

}
