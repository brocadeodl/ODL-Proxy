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
#include "AG/Local/AgStaticMappingsMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
    AgStaticMappingsMessage::AgStaticMappingsMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_STATIC_MAPPINGS_MESSAGE)
    {
    }

    AgStaticMappingsMessage::AgStaticMappingsMessage (const string &nPort, const vector<string> &staticFPorts, const UI32 &action)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_STATIC_MAPPINGS_MESSAGE),
        m_nPort(nPort),
        m_staticFPorts(staticFPorts),
        m_action(action)
    {
    }

    AgStaticMappingsMessage::~AgStaticMappingsMessage ()
    {
    }

    void  AgStaticMappingsMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_nPort, "agNPortNb"));
        addSerializableAttribute (new AttributeStringVector(&m_staticFPorts, "agStaticNFPortTopo"));
        addSerializableAttribute (new AttributeUI32(&m_action, "action"));
    }
	
    void  AgStaticMappingsMessage::setnPort(const string &nPort)
    {
        m_nPort =  nPort;
    }

    string  AgStaticMappingsMessage::getnPort() const
    {
        return (m_nPort);
    }

    void  AgStaticMappingsMessage::setStaticFPorts(const vector<string> &staticFPorts)
    {
        m_staticFPorts =  staticFPorts;
    }

    vector<string>  AgStaticMappingsMessage::getStaticFPorts() const
    {
        return (m_staticFPorts);
    }

    void  AgStaticMappingsMessage::setAction(const UI32 &action)
    {
        m_action =  action;
    }

    UI32  AgStaticMappingsMessage::getAction() const
    {
        return (m_action);
    }

}
