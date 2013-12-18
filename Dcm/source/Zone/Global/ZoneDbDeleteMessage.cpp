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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : dchung                                                     *
 **************************************************************************/

#include "Zone/Global/ZoneDbDeleteMessage.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneTypes.h"

namespace DcmNs
{

    ZoneDbDeleteMessage::ZoneDbDeleteMessage ()
        : ManagementInterfaceMessage (ZoneObjectManager::getClassName (), ZONEDBDELETE)
    {
    }

    ZoneDbDeleteMessage::ZoneDbDeleteMessage (const UI32 &dbEntryType,const string &dbEntryName,const UI32 &respCode)
        : ManagementInterfaceMessage (ZoneObjectManager::getClassName (), ZONEDBDELETE),
        m_dbEntryType    (dbEntryType),
        m_dbEntryName    (dbEntryName),
        m_respCode    (respCode),
        m_parentMoObjectId  (ObjectId::NullObjectId)
    {
    }

    ZoneDbDeleteMessage::~ZoneDbDeleteMessage ()
    {
    }

    void  ZoneDbDeleteMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_dbEntryType,"dbEntryType"));
        addSerializableAttribute (new AttributeString(&m_dbEntryName,"dbEntryName"));
        addSerializableAttribute (new AttributeUI32(&m_respCode,"respCode"));
        addSerializableAttribute (new AttributeObjectId (&m_parentMoObjectId,"parentMoObjectId"));
    }

    void  ZoneDbDeleteMessage::setDbEntryType(const UI32 &dbEntryType)
    {
        m_dbEntryType  =  dbEntryType;
    }

    UI32  ZoneDbDeleteMessage::getDbEntryType() const
    {
        return (m_dbEntryType);
    }

    void  ZoneDbDeleteMessage::setDbEntryName(const string &dbEntryName)
    {
        m_dbEntryName  =  dbEntryName;
    }

    string  ZoneDbDeleteMessage::getDbEntryName() const
    {
        return (m_dbEntryName);
    }

    void  ZoneDbDeleteMessage::setRespCode(const UI32 &respCode)
    {
        m_respCode  =  respCode;
    }

    UI32  ZoneDbDeleteMessage::getRespCode() const
    {
        return (m_respCode);
    }

    void ZoneDbDeleteMessage::setParentMoObjectId (const ObjectId parentMoObjectId)
    {
        m_parentMoObjectId = parentMoObjectId;
    }

    ObjectId ZoneDbDeleteMessage::getParentMoObjectId () const
    {
        return (m_parentMoObjectId);
    }

}
