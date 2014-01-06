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

#include "Zone/Global/ZoneDbDisableMessage.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneTypes.h"

namespace DcmNs
{

    ZoneDbDisableMessage::ZoneDbDisableMessage ()
        : PrismMessage (ZoneObjectManager::getPrismServiceId (), ZONEDBDISABLE)
    {
    }

    ZoneDbDisableMessage::ZoneDbDisableMessage (const UI32 &dbEntryType,const UI32 &respCode)
        : PrismMessage (ZoneObjectManager::getPrismServiceId (), ZONEDBDISABLE),
        m_dbEntryType    (dbEntryType),
        m_respCode    (respCode)
    {
    }

    ZoneDbDisableMessage::~ZoneDbDisableMessage ()
    {
    }

    void  ZoneDbDisableMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_dbEntryType,"dbEntryType"));
        addSerializableAttribute (new AttributeUI32(&m_respCode,"respCode"));
    }

    void  ZoneDbDisableMessage::setDbEntryType(const UI32 &dbEntryType)
    {
        m_dbEntryType  =  dbEntryType;
    }

    UI32  ZoneDbDisableMessage::getDbEntryType() const
    {
        return (m_dbEntryType);
    }

    void  ZoneDbDisableMessage::setRespCode(const UI32 &respCode)
    {
        m_respCode  =  respCode;
    }

    UI32  ZoneDbDisableMessage::getRespCode() const
    {
        return (m_respCode);
    }

}