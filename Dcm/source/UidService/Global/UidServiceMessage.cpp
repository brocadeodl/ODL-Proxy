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
 *   Author : sholla                                                     *
 **************************************************************************/

#include "UidService/Global/UidServiceMessage.h"
#include "UidService/Global/UidServiceObjectManager.h"
#include "UidService/Global/UidServiceTypes.h"

namespace DcmNs
{

    UidServiceMessage::UidServiceMessage (const UI32 opcode)
        : ManagementInterfaceMessage (UidServiceObjectManager::getClassName (), opcode),
        m_idType    (IDTYPE_INVALID_ID),
        m_id        (0) 
    {
    }

    UidServiceMessage::UidServiceMessage (const UI32 opcode, const string &entityName,const UI32 &idType,const UI32 &id)
        : ManagementInterfaceMessage (UidServiceObjectManager::getClassName (), opcode),
        m_entityName    (entityName),
        m_idType    (idType),
        m_id    (id)
    {
    }

    UidServiceMessage::~UidServiceMessage ()
    {
    }

    void  UidServiceMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_entityName,"entityName"));
        addSerializableAttribute (new AttributeUI32(&m_idType,"idType"));
        addSerializableAttribute (new AttributeUI32(&m_id,"id"));
    }

    void  UidServiceMessage::setEntityName(const string &entityName)
    {
        m_entityName  =  entityName;
    }

    string  UidServiceMessage::getEntityName() const
    {
        return (m_entityName);
    }

    void  UidServiceMessage::setIdType(const UI32 &idType)
    {
        m_idType  =  idType;
    }

    UI32  UidServiceMessage::getIdType() const
    {
        return (m_idType);
    }

    void  UidServiceMessage::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  UidServiceMessage::getId() const
    {
        return (m_id);
    }

    bool UidServiceMessage::isEntityNamePresent()
    {
        return !m_entityName.empty();
    }

    bool UidServiceMessage::isIdTypePresent()
    {
        // TODO check for valid types
        return m_idType != 0;
    }

    bool UidServiceMessage::isIdPresent()
    {
        return m_id != 0;
    }

}
