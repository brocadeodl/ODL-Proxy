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

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include "ClientInterface/AG/AgClientFailoverMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "AG/Common/AgPluginTypes.h"
#include "agd/public.h"
#include "AG/Common/AgUtils.h"

namespace DcmNs
{

    AgClientFailoverMessage::AgClientFailoverMessage ()
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_FAILOVER)
    {
    }

    AgClientFailoverMessage::AgClientFailoverMessage (const string &Port, const bool &Enable)
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_FAILOVER),
        m_Port    (Port),
        m_Enable   (Enable)
    {
    }

    AgClientFailoverMessage::~AgClientFailoverMessage ()
    {
    }

    void  AgClientFailoverMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_Enable, "agFailover"));
        addSerializableAttribute (new AttributeString(&m_Port, "Port"));
    }

    void  AgClientFailoverMessage::setPort(const string &Port)
    {
        m_Port  =  Port;
    }

    string AgClientFailoverMessage::getPort() const
    {
        return (m_Port);
    }

    void  AgClientFailoverMessage::setEnable(const bool &Enable)
    {
        m_Enable  =  Enable;
    }

    bool AgClientFailoverMessage::getEnable() const
    {
        return (m_Enable);
    }

    const void * AgClientFailoverMessage::getCStructureForInputs()
    {
        ag_failover_msg_t *msg_p = new ag_failover_msg_t;

        msg_p->port = atoi(m_Port.c_str());

        if(m_Enable) {
            msg_p->enable = 'y';
        } else {
            msg_p->enable = 'n';
        }
        
        return msg_p;
    }

    void
    AgClientFailoverMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ag_failover_msg_resp_t *resp_p = (ag_failover_msg_resp_t *)pOutputCStructure;

        if (resp_p)
        	addBuffer(resp_p->cmd_code, resp_p->size,
                  (unsigned char *)resp_p, false);
    }
}
