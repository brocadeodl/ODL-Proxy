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

#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgPGCreateMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
    AgPGCreateMessage::AgPGCreateMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_PGCREATE_MESSAGE)
    {
    }

    AgPGCreateMessage::AgPGCreateMessage (const UI32 &pgId, const vector<string> &nPorts, const vector<string> &fPorts, const string &pgName,
                                          const vector<string> &pgModes)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_PGCREATE_MESSAGE),
        m_pgId(pgId),
        m_nPorts(nPorts),
        m_fPorts(fPorts),
        m_pgName(pgName),
        m_pgModes(pgModes)
    {
    }

    AgPGCreateMessage::AgPGCreateMessage (const UI32 &pgId)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_PGCREATE_MESSAGE),
        m_pgId(pgId) 
    {
    }

    AgPGCreateMessage::~AgPGCreateMessage ()
    {
    }

    void  AgPGCreateMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_pgId, "agPgId"));
        addSerializableAttribute (new AttributeStringVector(&m_nPorts, "agPgNPortTopo"));
        addSerializableAttribute (new AttributeStringVector(&m_fPorts, "agPgFPortTopo"));
        addSerializableAttribute (new AttributeString(&m_pgName, "agPgName"));
        addSerializableAttribute (new AttributeStringVector(&m_pgModes, "agPgFlags"));
    }
	
    void  AgPGCreateMessage::setnPorts(const vector<string> &nPorts)
    {
        m_nPorts =  nPorts;
    }

    vector<string>  AgPGCreateMessage::getnPorts() const
    {
        return (m_nPorts);
    }

    void  AgPGCreateMessage::setfPorts(const vector<string> &fPorts)
    {
        m_fPorts =  fPorts;
    }

    vector<string>  AgPGCreateMessage::getfPorts() const
    {
        return (m_fPorts);
    }

    void  AgPGCreateMessage::setpgModes(const vector<string> &pgModes)
    {
        m_pgModes =  pgModes;
    }

    vector<string>  AgPGCreateMessage::getpgModes() const
    {
        return (m_pgModes);
    }

    void  AgPGCreateMessage::setpgName(const string &pgName)
    {
        m_pgName =  pgName;
    }

    string  AgPGCreateMessage::getpgName() const
    {
        return (m_pgName);
    }

    void  AgPGCreateMessage::setpgId(const UI32 &pgId)
    {
        m_pgId =  pgId;
    }

    UI32  AgPGCreateMessage::getpgId() const
    {
        return (m_pgId);
    }


}
