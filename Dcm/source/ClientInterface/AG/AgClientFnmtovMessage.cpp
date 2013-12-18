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
#include "ClientInterface/AG/AgClientFnmtovMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "AG/Common/AgPluginTypes.h"
#include "agd/public.h"
#include "AG/Common/AgUtils.h"

namespace DcmNs
{

    AgClientFnmtovMessage::AgClientFnmtovMessage ()
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_FNMTOV)
    {
    }

    AgClientFnmtovMessage::AgClientFnmtovMessage (const UI32 &FnmtoValue)
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_FNMTOV),
        m_FnmtoValue    (FnmtoValue)
    {
    }

    AgClientFnmtovMessage::~AgClientFnmtovMessage ()
    {
    }

    void  AgClientFnmtovMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_FnmtoValue, "fnmtovalue"));
    }

    void  AgClientFnmtovMessage::setFnmtoValue(const UI32 &FnmtoValue)
    {
        m_FnmtoValue  =  FnmtoValue;
    }

    UI32 AgClientFnmtovMessage::getFnmtoValue() const
    {
        return (m_FnmtoValue);
    }

    const void * AgClientFnmtovMessage::getCStructureForInputs()
    {
        ag_fnmtov_msg_t *msg_p = new ag_fnmtov_msg_t;

        msg_p->value = m_FnmtoValue;

        return msg_p;
    }

    void
    AgClientFnmtovMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ag_fnmtov_msg_resp_t *resp_p = (ag_fnmtov_msg_resp_t *)pOutputCStructure;

        if (resp_p)
        	addBuffer(resp_p->cmd_code, resp_p->size,
                  (unsigned char *)resp_p, false);
    }
}
