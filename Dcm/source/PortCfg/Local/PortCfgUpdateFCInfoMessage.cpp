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
 *   Author : vbagavat                                                     *
 **************************************************************************/

#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgUpdateFCInfoMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"

namespace DcmNs
{

    PortCfgUpdateFCInfoMessage::PortCfgUpdateFCInfoMessage ()
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGUPDATEFCINFO)
    {
    }

    PortCfgUpdateFCInfoMessage::PortCfgUpdateFCInfoMessage (const string &id,const UI32 &speed,const UI32 &fillWord,const UI32 &longDistance,const UI32 &vcLinkInit,const bool &islRRdyMode,const UI32 &distance,const UI32 &dbEntryType, const bool &status, const UI32 &respCode,const UI32 &lock_port)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGUPDATEFCINFO),
        m_id    (id),
        m_speed    (speed),
        m_fillWord    (fillWord),
        m_longDistance    (longDistance),
        m_vcLinkInit    (vcLinkInit),
        m_islRRdyMode    (islRRdyMode),
        m_distance    (distance),
        m_dbEntryType    (dbEntryType),
        m_status    (status),
        m_respCode    (respCode),
        m_lock_port    (lock_port)
    {
    }

    PortCfgUpdateFCInfoMessage::~PortCfgUpdateFCInfoMessage ()
    {
    }

    void  PortCfgUpdateFCInfoMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeUI32(&m_speed,"speed"));
        addSerializableAttribute (new AttributeUI32(&m_fillWord,"fillWord"));
        addSerializableAttribute (new AttributeUI32(&m_longDistance,"longDistance"));
        addSerializableAttribute (new AttributeUI32(&m_vcLinkInit,"vcLinkInit"));
        addSerializableAttribute (new AttributeBool(&m_islRRdyMode,"islRRdyMode"));
        addSerializableAttribute (new AttributeUI32(&m_distance,"distance"));
        addSerializableAttribute (new AttributeUI32(&m_dbEntryType,"dbEntryType"));
        addSerializableAttribute (new AttributeBool(&m_status,"status"));
        addSerializableAttribute (new AttributeUI32(&m_respCode,"respCode"));
        addSerializableAttribute (new AttributeUI32(&m_lock_port,"lock_port"));
    }

    void  PortCfgUpdateFCInfoMessage::setId(const string &id)
    {
        m_id  =  id;
    }

    string  PortCfgUpdateFCInfoMessage::getId() const
    {
        return (m_id);
    }

    void  PortCfgUpdateFCInfoMessage::setSpeed(const UI32 &speed)
    {
        m_speed  =  speed;
    }

    UI32  PortCfgUpdateFCInfoMessage::getSpeed() const
    {
        return (m_speed);
    }

    void  PortCfgUpdateFCInfoMessage::setFillWord(const UI32 &fillWord)
    {
        m_fillWord  =  fillWord;
    }

    UI32  PortCfgUpdateFCInfoMessage::getFillWord() const
    {
        return (m_fillWord);
    }

    void  PortCfgUpdateFCInfoMessage::setLongDistance(const UI32 &longDistance)
    {
        m_longDistance  =  longDistance;
    }

    UI32  PortCfgUpdateFCInfoMessage::getLongDistance() const
    {
        return (m_longDistance);
    }

    void  PortCfgUpdateFCInfoMessage::setVcLinkInit(const UI32 &vcLinkInit)
    {
        m_vcLinkInit  =  vcLinkInit;
    }

    UI32  PortCfgUpdateFCInfoMessage::getVcLinkInit() const
    {
        return (m_vcLinkInit);
    }

    void  PortCfgUpdateFCInfoMessage::setIslRRdyMode(const bool &islRRdyMode)
    {
        m_islRRdyMode  =  islRRdyMode;
    }

    bool  PortCfgUpdateFCInfoMessage::getIslRRdyMode() const
    {
        return (m_islRRdyMode);
    }

    void  PortCfgUpdateFCInfoMessage::setDistance(const UI32 &distance)
    {
        m_distance  =  distance;
    }

    UI32  PortCfgUpdateFCInfoMessage::getDistance() const
    {
        return (m_distance);
    }

    void  PortCfgUpdateFCInfoMessage::setStatus(const bool &status)
    {
        m_status  =  status; 
    }
 
    bool  PortCfgUpdateFCInfoMessage::getStatus() const
    {
        return (m_status);
    }

    void  PortCfgUpdateFCInfoMessage::setDbEntryType(const UI32 &dbEntryType)
    {
        m_dbEntryType  =  dbEntryType;
    }

    UI32  PortCfgUpdateFCInfoMessage::getDbEntryType() const
    {
        return (m_dbEntryType);
    }

    void  PortCfgUpdateFCInfoMessage::setRespCode(const UI32 &respCode)
    {
        m_respCode  =  respCode;
    }

    UI32  PortCfgUpdateFCInfoMessage::getRespCode() const
    {
        return (m_respCode);
    }

    void  PortCfgUpdateFCInfoMessage::setLock_port(const UI32 &lock_port)
    {
        m_lock_port  =  lock_port;
    }

    UI32  PortCfgUpdateFCInfoMessage::getLock_port() const
    {
        return (m_lock_port);
    }
}
