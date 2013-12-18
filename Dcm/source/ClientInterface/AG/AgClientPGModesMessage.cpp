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
#include "ClientInterface/AG/AgClientPGModesMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "AG/Common/AgPluginTypes.h"
#include "agd/public.h"
#include "AG/Common/AgUtils.h"

#define LB_MODE     0
#define MFNM_MODE   1
#define NUNR_MODE   2

namespace DcmNs
{

    AgClientPGModesMessage::AgClientPGModesMessage ()
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_PGMODES)
    {
    }

    AgClientPGModesMessage::AgClientPGModesMessage (const UI32 &pgId, const vector<string> &Modes, const UI32 &action)
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_PGMODES),
        m_pgId    (pgId),
        m_Modes   (Modes),
        m_action(action)
    {
    }

    AgClientPGModesMessage::~AgClientPGModesMessage ()
    {
    }

    void  AgClientPGModesMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_pgId, "agPgId"));
        addSerializableAttribute (new AttributeStringVector(&m_Modes, "agPgFlags"));
        addSerializableAttribute (new AttributeUI32(&m_action, "action"));
    }

    void AgClientPGModesMessage::setAction(const UI32 &action)
    {
        m_action = action;
    }

    UI32 AgClientPGModesMessage::getAction() const
    {
        return(m_action);
    }

    void AgClientPGModesMessage::setpgId(const UI32 &pgId)
    {
        m_pgId = pgId;
    }

    UI32 AgClientPGModesMessage::getpgId() const
    {
        return(m_pgId);
    }

    void AgClientPGModesMessage::setModes(const vector<string> &modes)
    {
        m_Modes = modes;
    }

    vector<string> AgClientPGModesMessage::getModes()  const
    {
        return(m_Modes);
    }

    const void * AgClientPGModesMessage::getCStructureForInputs()
    {
        ag_pgmodes_msg_t *msg_p;
        string mode;
        vector<string>::iterator iter;
        UI32 msgSize = sizeof(ag_pgmodes_msg_t) + SIZEOF_BM(10);
//                                                   for modes         

        msg_p = (ag_pgmodes_msg_t *) calloc (msgSize, 1);
        msg_p->modes = (BitMap_t)((char *)msg_p + sizeof(ag_pgmodes_msg_t));

        BM_Clear(msg_p->modes, 10);

        for(iter = m_Modes.begin(); iter != m_Modes.end(); iter++) {
            mode = *iter;
            if (strcmp(mode.c_str(), AG_PGFLG_LB_NAME) == 0) {
                BM_SetBitN(LB_MODE, msg_p->modes);
            } else if (strcmp(mode.c_str(), AG_PGFLG_MFNM_NAME) == 0) {
                BM_SetBitN(MFNM_MODE, msg_p->modes);
            } else if (strcmp(mode.c_str(),AG_PGFLG_NPORT_UP_NO_REBAL_NAME) == 0) {
                BM_SetBitN(NUNR_MODE, msg_p->modes);
            }
        }


        msg_p->pgid = m_pgId;

        msg_p->action = m_action;

        return msg_p;
    }

    void
    AgClientPGModesMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ag_pgmodes_msg_resp_t *resp_p = (ag_pgmodes_msg_resp_t *)pOutputCStructure;

        if (resp_p)
        	addBuffer(resp_p->cmd_code, resp_p->size,
                  (unsigned char *)resp_p, false);
    }
}
