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

#include "UidService/Global/UidServiceQueryMessage.h"
#include "UidService/Global/UidServiceObjectManager.h"
#include "UidService/Global/UidServiceTypes.h"

namespace DcmNs
{

    UidServiceQueryMessage::UidServiceQueryMessage (const UI32 opcode)
        : UidServiceMessage (opcode)
    {
    }

    UidServiceQueryMessage::~UidServiceQueryMessage ()
    {
    }

    void  UidServiceQueryMessage::setupAttributesForSerialization ()
    {
        UidServiceMessage::setupAttributesForSerialization();

        addSerializableAttribute(new AttributeStringVector(&entityNameList, "entityNameList"));
        addSerializableAttribute(new AttributeUI32Vector(&idList, "idList"));
        addSerializableAttribute(new AttributeBoolVector(&activeList, "activeList"));
    }


    UI32 UidServiceQueryMessage::getCount () const
    {
        return entityNameList.size();
    }

    UI32 UidServiceQueryMessage::getIdAt(const UI32 index) const
    {
        return idList[index];
    }

    bool UidServiceQueryMessage::getActiveAt (const UI32 index) const
    {
        return activeList[index];
    }

    string  UidServiceQueryMessage::getEntityNameAt (const UI32 index) const
    {
        return entityNameList[index];
    }

    void UidServiceQueryMessage::addEntityName (const string &entityName)
    {
        entityNameList.push_back(entityName);
    }

    void UidServiceQueryMessage::addIdInfo (const string &entityName, const UI32 id, const bool active)
    {
        idList.push_back(id);
        activeList.push_back(active);
        entityNameList.push_back(entityName);
    }

    void UidServiceQueryMessage::pack()
    {
    }

}
