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
 *   Author : ranantha                                             *
 **************************************************************************/

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include "ClientInterface/AG/AgClientRelCounterMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "AG/Common/AgPluginTypes.h"
#include "agd/public.h"
#include "AG/Common/AgUtils.h"

namespace DcmNs
{

    AgClientRelCounterMessage::AgClientRelCounterMessage ()
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_RELCOUNTER)
    {
    }

    AgClientRelCounterMessage::AgClientRelCounterMessage (const UI32 &RelCounterValue)
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_RELCOUNTER),
        m_RelCounterValue    (RelCounterValue)
    {
    }

    AgClientRelCounterMessage::~AgClientRelCounterMessage ()
    {
    }

    void  AgClientRelCounterMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_RelCounterValue, "reliabilityvalue"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode, "commandcode"));
    }

    void  AgClientRelCounterMessage::setRelCounterValue(const UI32 &RelCounterValue)
    {
        m_RelCounterValue  =  RelCounterValue;
    }

    UI32 AgClientRelCounterMessage::getRelCounterValue() const
    {
        return (m_RelCounterValue);
    }

    void  AgClientRelCounterMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode =  cmdCode;
    }

    UI32 AgClientRelCounterMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    const void * AgClientRelCounterMessage::getCStructureForInputs()
    {
        ag_relcounter_msg_t *msg_p = new ag_relcounter_msg_t;

        msg_p->value = m_RelCounterValue;
        msg_p->cmd_code = m_cmdCode;

        return msg_p;
    }

    void
    AgClientRelCounterMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ag_relcounter_msg_resp_t *resp_p = (ag_relcounter_msg_resp_t *)pOutputCStructure;

        if (resp_p)
        	addBuffer(resp_p->cmd_code, resp_p->size,
                  (unsigned char *)resp_p, false);
    }
}
