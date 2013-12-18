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

#include "Zone/Global/ZoneDbTransAbortMessage.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneTypes.h"

namespace DcmNs
{

    ZoneDbTransAbortMessage::ZoneDbTransAbortMessage ()
        : PrismMessage (ZoneObjectManager::getPrismServiceId (), ZONEDBTRANSABORT)
    {
    }

    ZoneDbTransAbortMessage::ZoneDbTransAbortMessage (const UI32 &dbEntryType,const UI32 &dbTransToken,const UI32 &respCode)
        : PrismMessage (ZoneObjectManager::getPrismServiceId (), ZONEDBTRANSABORT),
        m_dbEntryType    (dbEntryType),
        m_dbTransToken    (dbTransToken),
        m_respCode    (respCode)
    {
    }

    ZoneDbTransAbortMessage::~ZoneDbTransAbortMessage ()
    {
    }

    void  ZoneDbTransAbortMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_dbEntryType,"dbEntryType"));
        addSerializableAttribute (new AttributeUI32(&m_dbTransToken,"dbTransToken"));
        addSerializableAttribute (new AttributeUI32(&m_respCode,"respCode"));
    }

    void  ZoneDbTransAbortMessage::setDbEntryType(const UI32 &dbEntryType)
    {
        m_dbEntryType  =  dbEntryType;
    }

    UI32  ZoneDbTransAbortMessage::getDbEntryType() const
    {
        return (m_dbEntryType);
    }

    void  ZoneDbTransAbortMessage::setDbTransToken(const UI32 &dbTransToken)
    {
        m_dbTransToken  =  dbTransToken;
    }

    UI32  ZoneDbTransAbortMessage::getDbTransToken() const
    {
        return (m_dbTransToken);
    }

    void  ZoneDbTransAbortMessage::setRespCode(const UI32 &respCode)
    {
        m_respCode  =  respCode;
    }

    UI32  ZoneDbTransAbortMessage::getRespCode() const
    {
        return (m_respCode);
    }

}
