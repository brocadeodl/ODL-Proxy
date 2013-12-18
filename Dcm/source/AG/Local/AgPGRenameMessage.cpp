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
#include "AG/Local/AgPGRenameMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
    AgPGRenameMessage::AgPGRenameMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_PGRENAME_MESSAGE)
    {
    }

    AgPGRenameMessage::AgPGRenameMessage (const UI32 &pgId, const string &Name)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_PGRENAME_MESSAGE),
        m_pgId(pgId),
        m_Name(Name)
    {
    }

    AgPGRenameMessage::~AgPGRenameMessage ()
    {
    }

    void  AgPGRenameMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_pgId, "agPgId"));
        addSerializableAttribute (new AttributeString(&m_Name, "agPgName"));
    }
	
    void  AgPGRenameMessage::setName(const string &Name)
    {
        m_Name =  Name;
    }

    string  AgPGRenameMessage::getName() const
    {
        return (m_Name);
    }

    void  AgPGRenameMessage::setpgId(const UI32 &pgId)
    {
        m_pgId =  pgId;
    }

    UI32  AgPGRenameMessage::getpgId() const
    {
        return (m_pgId);
    }


}
