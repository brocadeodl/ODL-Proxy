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

#include "Zone/Global/ZoneOperationInfoMessage.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneTypes.h"

namespace DcmNs
{

    ZoneOperationInfoMessage::ZoneOperationInfoMessage ()
        : PrismMessage (ZoneObjectManager::getPrismServiceId (), ZONEOPERATIONINFO)
    {
    }

    ZoneOperationInfoMessage::ZoneOperationInfoMessage (const UI32 &dbTransToken,const UI32 &dbTransFlag,const UI32 &dbMaxSize,const UI32 &dbAvailableSize,const UI32 &dbCommittedSize,const UI32 &dbTransactionSize,const string &lastZoneChangedTimestamp,const string &lastZoneCommittedTimestamp)
        : PrismMessage (ZoneObjectManager::getPrismServiceId (), ZONEOPERATIONINFO),
        m_dbTransToken    (dbTransToken),
        m_dbTransFlag    (dbTransFlag),
        m_dbMaxSize    (dbMaxSize),
        m_dbAvailableSize    (dbAvailableSize),
        m_dbCommittedSize    (dbCommittedSize),
        m_dbTransactionSize    (dbTransactionSize),
        m_lastZoneChangedTimestamp    (lastZoneChangedTimestamp),
		m_lastZoneCommittedTimestamp    (lastZoneCommittedTimestamp)
    {
    }

    ZoneOperationInfoMessage::~ZoneOperationInfoMessage ()
    {
    }

    void  ZoneOperationInfoMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_dbTransToken,"dbTransToken"));
        addSerializableAttribute (new AttributeUI32(&m_dbTransFlag,"dbTransFlag"));
        addSerializableAttribute (new AttributeUI32(&m_dbMaxSize,"dbMaxSize"));
        addSerializableAttribute (new AttributeUI32(&m_dbAvailableSize,"dbAvailableSize"));
        addSerializableAttribute (new AttributeUI32(&m_dbCommittedSize,"dbCommittedSize"));
        addSerializableAttribute (new AttributeUI32(&m_dbTransactionSize,"dbTransactionSize"));
        addSerializableAttribute (new AttributeString(&m_lastZoneChangedTimestamp,"lastZoneChangedTimestamp"));
        addSerializableAttribute (new AttributeString(&m_lastZoneCommittedTimestamp,"lastZoneCommittedTimestamp"));
    }

    void  ZoneOperationInfoMessage::setDbTransToken(const UI32 &dbTransToken)
    {
        m_dbTransToken  =  dbTransToken;
    }

    UI32  ZoneOperationInfoMessage::getDbTransToken() const
    {
        return (m_dbTransToken);
    }

    void  ZoneOperationInfoMessage::setDbTransFlag(const UI32 &dbTransFlag)
    {
        m_dbTransFlag  =  dbTransFlag;
    }

    UI32  ZoneOperationInfoMessage::getDbTransFlag() const
    {
        return (m_dbTransFlag);
    }

    void  ZoneOperationInfoMessage::setDbMaxSize(const UI32 &dbMaxSize)
    {
        m_dbMaxSize  =  dbMaxSize;
    }

    UI32  ZoneOperationInfoMessage::getDbMaxSize() const
    {
        return (m_dbMaxSize);
    }

    void  ZoneOperationInfoMessage::setDbAvailableSize(const UI32 &dbAvailableSize)
    {
        m_dbAvailableSize  =  dbAvailableSize;
    }

    UI32  ZoneOperationInfoMessage::getDbAvailableSize() const
    {
        return (m_dbAvailableSize);
    }

    void  ZoneOperationInfoMessage::setDbCommittedSize(const UI32 &dbCommittedSize)
    {
        m_dbCommittedSize  =  dbCommittedSize;
    }

    UI32  ZoneOperationInfoMessage::getDbCommittedSize() const
    {
        return (m_dbCommittedSize);
    }

    void  ZoneOperationInfoMessage::setDbTransactionSize(const UI32 &dbTransactionSize)
    {
        m_dbTransactionSize  =  dbTransactionSize;
    }

    UI32  ZoneOperationInfoMessage::getDbTransactionSize() const
    {
        return (m_dbTransactionSize);
    }

    void  ZoneOperationInfoMessage::setLastZoneChangedTimestamp(const string &lastZoneChangedTimestamp)
    {
        m_lastZoneChangedTimestamp  =  lastZoneChangedTimestamp;
    }

    string  ZoneOperationInfoMessage::getLastZoneChangedTimestamp() const
    {
        return (m_lastZoneChangedTimestamp);
    }

    void  ZoneOperationInfoMessage::setLastZoneCommittedTimestamp(const string &lastZoneCommittedTimestamp)
    {
        m_lastZoneCommittedTimestamp  =  lastZoneCommittedTimestamp;
    }

    string  ZoneOperationInfoMessage::getLastZoneCommittedTimestamp() const
    {
        return (m_lastZoneCommittedTimestamp);
    }
}
