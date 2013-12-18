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
 *   Author : Ramachandran Anantharaman                                    *
 **************************************************************************/

#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgMappingsMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
    AgMappingsMessage::AgMappingsMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_MAPPINGS_MESSAGE)
    {
    }

    AgMappingsMessage::AgMappingsMessage (const string &nPort, const vector<string> &fPorts)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_MAPPINGS_MESSAGE),
        m_nPort(nPort),
        m_fPorts(fPorts)
    {
    }

    AgMappingsMessage::~AgMappingsMessage ()
    {
    }

    void  AgMappingsMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_nPort, "agNPortNb"));
        addSerializableAttribute (new AttributeStringVector(&m_fPorts, "agNFPortTopo"));
    }
	
    void  AgMappingsMessage::setnPort(const string &nPort)
    {
        m_nPort =  nPort;
    }

    string  AgMappingsMessage::getnPort() const
    {
        return (m_nPort);
    }

    void  AgMappingsMessage::setfPorts(const vector<string> &fPorts)
    {
        m_fPorts =  fPorts;
    }

    vector<string>  AgMappingsMessage::getfPorts() const
    {
        return (m_fPorts);
    }
}
