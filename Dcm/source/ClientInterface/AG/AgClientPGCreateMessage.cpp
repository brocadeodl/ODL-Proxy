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
#include "ClientInterface/AG/AgClientPGCreateMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "AG/Common/AgPluginTypes.h"
#include "agd/public.h"
#include "AG/Common/AgUtils.h"

namespace DcmNs
{

    AgClientPGCreateMessage::AgClientPGCreateMessage ()
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_PGCREATE)
    {
    }

    AgClientPGCreateMessage::AgClientPGCreateMessage (const UI32 &pgId, const vector<string> &nPorts, const vector<string> &fPorts, const string &pgName, const vector<string> &pgModes)
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_PGCREATE),
        m_pgId    (pgId),
        m_nPorts   (nPorts),
        m_fPorts(fPorts),
        m_pgName(pgName),
        m_pgModes(pgModes)
    {
    }

    AgClientPGCreateMessage::~AgClientPGCreateMessage ()
    {
    }

    void  AgClientPGCreateMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_pgId, "agPgId"));
    }

    void AgClientPGCreateMessage::setpgId(const UI32 &pgId)
    {
        m_pgId = pgId;
    }

    UI32 AgClientPGCreateMessage::getpgId() const
    {
        return(m_pgId);
    }

    void AgClientPGCreateMessage::setnPorts(const vector<string> &nPorts)
    {
        m_nPorts = nPorts;
    }

    vector<string> AgClientPGCreateMessage::getnPorts()  const
    {
        return(m_nPorts);
    }

    void AgClientPGCreateMessage::setfPorts(const vector<string> &fPorts)
    {
	    m_fPorts = fPorts;
    }

    vector<string> AgClientPGCreateMessage::getfPorts()  const
    {    
    	return(m_fPorts);
    }


    void AgClientPGCreateMessage::setpgName(const string &pgName)
    {
        m_pgName = pgName;
    }

    string AgClientPGCreateMessage::getpgName()  const
    {
        return(m_pgName);
    }


    void AgClientPGCreateMessage::setpgModes(const vector<string> &pgModes)
    {
        m_pgModes = pgModes;
    }

    vector<string> AgClientPGCreateMessage::getpgModes()  const
    {
        return(m_pgModes);
    }

    const void * AgClientPGCreateMessage::getCStructureForInputs()
    {
        ag_pgcreate_msg_t *msg_p;
        string mode;
        vector<string>::iterator iter;
        UI32 msgSize = sizeof(ag_pgcreate_msg_t) + SIZEOF_BM(MAX_AG_PORT)+ SIZEOF_BM(MAX_AG_PORT) + SIZEOF_BM(10);
//                                                   for nports             for fports              for modes

        msg_p = (ag_pgcreate_msg_t *) calloc (msgSize, 1);

        msg_p->nports = (BitMap_t)((char *)msg_p + sizeof(ag_pgcreate_msg_t));
        msg_p->fports = (BitMap_t)((char *)msg_p + sizeof(ag_pgcreate_msg_t) + SIZEOF_BM(MAX_AG_PORT));
        msg_p->pgmodes = (BitMap_t)((char *)msg_p + sizeof(ag_pgcreate_msg_t) + SIZEOF_BM(MAX_AG_PORT) + SIZEOF_BM(MAX_AG_PORT));
        BM_Clear(msg_p->nports, MAX_AG_PORT);
        BM_Clear(msg_p->fports, MAX_AG_PORT);
        BM_Clear(msg_p->pgmodes, 10);

        msg_p->pgid = m_pgId;

        return msg_p;
    }

    void
    AgClientPGCreateMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ag_pgcreate_msg_resp_t *resp_p = (ag_pgcreate_msg_resp_t *)pOutputCStructure;

        if (resp_p)
        	addBuffer(resp_p->cmd_code, resp_p->size,
                  (unsigned char *)resp_p, false);
    }
}
