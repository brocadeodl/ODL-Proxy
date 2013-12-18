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
#include "ClientInterface/AG/AgClientPGDelMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "AG/Common/AgPluginTypes.h"
#include "agd/public.h"
#include "AG/Common/AgUtils.h"

namespace DcmNs
{

    AgClientPGDelMessage::AgClientPGDelMessage ()
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_PGDEL)
    {
    }

    AgClientPGDelMessage::AgClientPGDelMessage (const UI32 &pgId, const vector<string> &nPorts)
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_PGDEL),
        m_pgId    (pgId),
        m_nPorts   (nPorts)
    {
    }

    AgClientPGDelMessage::~AgClientPGDelMessage ()
    {
    }

    void  AgClientPGDelMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_pgId, "agPgId"));
        addSerializableAttribute (new AttributeStringVector(&m_nPorts, "agPgNPortTopo"));
    }

    void AgClientPGDelMessage::setpgId(const UI32 &pgId)
    {
        m_pgId = pgId;
    }

    UI32 AgClientPGDelMessage::getpgId() const
    {
        return(m_pgId);
    }

    void AgClientPGDelMessage::setnPorts(const vector<string> &nPorts)
    {
        m_nPorts = nPorts;
    }

    vector<string> AgClientPGDelMessage::getnPorts()  const
    {
        return(m_nPorts);
    }

    const void * AgClientPGDelMessage::getCStructureForInputs()
    {
        ag_pgdel_msg_t *msg_p;
        string mode;
        vector<string>::iterator iter;
        int nportinteger=0; 
        string fport_str, nport_str;
        UI32 msgSize = sizeof(ag_pgdel_msg_t) + SIZEOF_BM(MAX_AG_PORT);
//                                                   for nports         

        msg_p = (ag_pgdel_msg_t *) calloc (msgSize, 1);
        msg_p->nports = (BitMap_t)((char *)msg_p + sizeof(ag_pgdel_msg_t));

        BM_Clear(msg_p->nports, MAX_AG_PORT);

        for(iter = m_nPorts.begin(); iter != m_nPorts.end(); iter++) {
            nport_str = *iter;
            nportinteger = atoi(nport_str.c_str());
            if(nportinteger < MAX_AG_PORT) {
                BM_SetBitN(nportinteger, msg_p->nports);
            }
        }

        msg_p->pgid = m_pgId;

        return msg_p;
    }

    void
    AgClientPGDelMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ag_pgdel_msg_resp_t *resp_p = (ag_pgdel_msg_resp_t *)pOutputCStructure;

        if (resp_p)
        	addBuffer(resp_p->cmd_code, resp_p->size,
                  (unsigned char *)resp_p, false);
    }
}
