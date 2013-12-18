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
#include "AG/Local/AgPGModesMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
    AgPGModesMessage::AgPGModesMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_PGMODES_MESSAGE)
    {
    }

    AgPGModesMessage::AgPGModesMessage (const UI32 &pgId, const vector<string> &Modes, const UI32 &action)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_PGMODES_MESSAGE),
        m_pgId(pgId),
        m_Modes(Modes),
        m_action(action)
    {
    }

    AgPGModesMessage::~AgPGModesMessage ()
    {
    }

    void  AgPGModesMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_pgId, "agPgId"));
        addSerializableAttribute (new AttributeStringVector(&m_Modes, "agPgFlags"));
        addSerializableAttribute (new AttributeUI32(&m_action, "action"));
    }
	
    void  AgPGModesMessage::setModes(const vector<string> &Modes)
    {
        m_Modes =  Modes;
    }

    vector<string>  AgPGModesMessage::getModes() const
    {
        return (m_Modes);
    }

    void  AgPGModesMessage::setpgId(const UI32 &pgId)
    {
        m_pgId =  pgId;
    }

    UI32  AgPGModesMessage::getpgId() const
    {
        return (m_pgId);
    }

    void  AgPGModesMessage::setAction(const UI32 &action)
    {
        m_action =  action;
    }

    UI32  AgPGModesMessage::getAction() const
    {
        return (m_action);
    }


}
