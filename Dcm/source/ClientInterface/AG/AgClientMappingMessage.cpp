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
#include "ClientInterface/AG/AgClientMappingMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "AG/Common/AgPluginTypes.h"
#include "agd/public.h"
#include "AG/Common/AgUtils.h"

namespace DcmNs
{

    AgClientMappingMessage::AgClientMappingMessage ()
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_MAPPING)
    {
    }

    AgClientMappingMessage::AgClientMappingMessage (const string &nPort, const vector<string> &fPorts)
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_MAPPING),
        m_nPort    (nPort),
        m_fPorts   (fPorts)
    {
    }

    AgClientMappingMessage::~AgClientMappingMessage ()
    {
    }

    void  AgClientMappingMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_nPort, "agNPortNb"));
        addSerializableAttribute (new AttributeStringVector(&m_fPorts, "agNFPortTopo"));
    }

    void  AgClientMappingMessage::setnPort(const string &nPort)
    {
        m_nPort  =  nPort;
    }

    string AgClientMappingMessage::getnPort() const
    {
        return (m_nPort);
    }

    void  AgClientMappingMessage::setfPorts(const vector<string> &fPorts)
    {
        m_fPorts  =  fPorts;
    }

    vector<string> AgClientMappingMessage::getfPorts() const
    {
        return (m_fPorts);
    }

    const void * AgClientMappingMessage::getCStructureForInputs()
    {
        ag_mapping_msg_t *msg_p;
        UI32 msgSize = sizeof(ag_mapping_msg_t) + SIZEOF_BM(MAX_AG_PORT);
        vector<string>::iterator iter;
        int fportinteger; 
        string fport_str;

        msg_p = (ag_mapping_msg_t *) calloc (msgSize, 1);
        msg_p->fports = (BitMap_t)((char *)msg_p + sizeof(ag_mapping_msg_t));
        BM_Clear(msg_p->fports, MAX_AG_PORT);

        msg_p->nport = atoi(m_nPort.c_str());
        for(iter = m_fPorts.begin(); iter != m_fPorts.end(); iter++) {
            fport_str = *iter;
            fportinteger = atoi(fport_str.c_str());
            if(fportinteger < MAX_AG_PORT) {
                BM_SetBitN(fportinteger, msg_p->fports);
            }
        }

        return msg_p;
    }

    void
    AgClientMappingMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ag_mapping_msg_resp_t *resp_p = (ag_mapping_msg_resp_t *)pOutputCStructure;

        if (resp_p)
        	addBuffer(resp_p->cmd_code, resp_p->size,
                  (unsigned char *)resp_p, false);
    }
}
