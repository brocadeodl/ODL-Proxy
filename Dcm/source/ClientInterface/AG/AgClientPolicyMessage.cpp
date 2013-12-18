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
#include "ClientInterface/AG/AgClientPolicyMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "AG/Common/AgPluginTypes.h"
#include "agd/public.h"
#include "AG/Common/AgUtils.h"

namespace DcmNs
{

    AgClientPolicyMessage::AgClientPolicyMessage ()
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_POLICY)
    {
    }

    AgClientPolicyMessage::AgClientPolicyMessage (const string &Policy, const bool &Enable)
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_POLICY),
        m_Policy    (Policy),
        m_Enable   (Enable)
    {
    }

    AgClientPolicyMessage::~AgClientPolicyMessage ()
    {
    }

    void  AgClientPolicyMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_Policy, "policy"));
        addSerializableAttribute (new AttributeBool(&m_Enable, "enable"));
    }

    void  AgClientPolicyMessage::setPolicy(const string &Policy)
    {
        m_Policy  =  Policy;
    }

    string AgClientPolicyMessage::getPolicy() const
    {
        return (m_Policy);
    }

    void  AgClientPolicyMessage::setEnable(const bool &Enable)
    {
        m_Enable  =  Enable;
    }

    bool AgClientPolicyMessage::getEnable() const
    {
        return (m_Enable);
    }

    const void * AgClientPolicyMessage::getCStructureForInputs()
    {
        ag_policy_msg_t *msg_p = new ag_policy_msg_t;
        char *policy_send;

        policy_send = (char *)m_Policy.c_str();

        if(strcasecmp(policy_send, "pg") == 0)
            msg_p->policy = AG_POLICY_PG;
        else if (strcasecmp(policy_send, "ads") == 0)
               msg_p->policy = AG_POLICY_ADS;

        if(m_Enable) {
               msg_p->flag = 1;
        } else {
               msg_p->flag = 0;
        }

        return msg_p;
    }

    void
    AgClientPolicyMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ag_policy_msg_resp_t *resp_p = (ag_policy_msg_resp_t *)pOutputCStructure;

        if (resp_p)
        	addBuffer(resp_p->cmd_code, resp_p->size,
                  (unsigned char *)resp_p, false);
    }
}
