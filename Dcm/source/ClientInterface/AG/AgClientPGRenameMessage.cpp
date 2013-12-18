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
 *   Author : Ramachandran Anantharaman                                    *
 **************************************************************************/

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include "ClientInterface/AG/AgClientPGRenameMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "AG/Common/AgPluginTypes.h"
#include "agd/public.h"
#include "AG/Common/AgUtils.h"

namespace DcmNs
{

    AgClientPGRenameMessage::AgClientPGRenameMessage ()
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_PGRENAME)
    {
    }

    AgClientPGRenameMessage::AgClientPGRenameMessage (const UI32 &pgId, const string &Name)
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_PGRENAME),
        m_pgId    (pgId),
        m_Name   (Name)
    {
    }

    AgClientPGRenameMessage::~AgClientPGRenameMessage ()
    {
    }

    void  AgClientPGRenameMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_pgId, "agPgId"));
        addSerializableAttribute (new AttributeString(&m_Name, "agPgName"));
    }

    void AgClientPGRenameMessage::setpgId(const UI32 &pgId)
    {
        m_pgId = pgId;
    }

    UI32 AgClientPGRenameMessage::getpgId() const
    {
        return(m_pgId);
    }

    void AgClientPGRenameMessage::setName(const string &Name)
    {
        m_Name = Name;
    }

    string AgClientPGRenameMessage::getName()  const
    {
        return(m_Name);
    }

    const void * AgClientPGRenameMessage::getCStructureForInputs()
    {
        ag_pgrename_msg_t *msg_p;

        msg_p = (ag_pgrename_msg_t *) calloc (sizeof(ag_pgrename_msg_t), 1);

        msg_p->pgid = m_pgId;
        strncpy(msg_p->name, m_Name.c_str(), 49);
        msg_p->name[49] = '\0';


        return msg_p;
    }

    void
    AgClientPGRenameMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ag_pgrename_msg_resp_t *resp_p = (ag_pgrename_msg_resp_t *)pOutputCStructure;

        if (resp_p)
        	addBuffer(resp_p->cmd_code, resp_p->size,
                  (unsigned char *)resp_p, false);
    }
}
