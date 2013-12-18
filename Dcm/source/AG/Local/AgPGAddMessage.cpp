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
#include "AG/Local/AgPGAddMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
    AgPGAddMessage::AgPGAddMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_PGADD_MESSAGE)
    {
    }

    AgPGAddMessage::AgPGAddMessage (const UI32 &pgId, const vector<string> &nPorts)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_PGADD_MESSAGE),
        m_pgId(pgId),
        m_nPorts(nPorts)
    {
    }

    AgPGAddMessage::~AgPGAddMessage ()
    {
    }

    void  AgPGAddMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_pgId, "agPgId"));
        addSerializableAttribute (new AttributeStringVector(&m_nPorts, "agPgNPortTopo"));
    }
	
    void  AgPGAddMessage::setnPorts(const vector<string> &nPorts)
    {
        m_nPorts =  nPorts;
    }

    vector<string>  AgPGAddMessage::getnPorts() const
    {
        return (m_nPorts);
    }

    void  AgPGAddMessage::setpgId(const UI32 &pgId)
    {
        m_pgId =  pgId;
    }

    UI32  AgPGAddMessage::getpgId() const
    {
        return (m_pgId);
    }


}
