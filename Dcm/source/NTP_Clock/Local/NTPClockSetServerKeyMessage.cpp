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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : bperuma                                                     *
 **************************************************************************/

#include "NTP_Clock/Local/NTPClockLocalObjectManager.h"
#include "NTP_Clock/Local/NTPClockSetServerKeyMessage.h"
#include "NTP_Clock/Local/NTPClockTypes.h"

namespace DcmNs
{

    NTPClockSetServerKeyMessage::NTPClockSetServerKeyMessage ()
        : PrismMessage (NTPClockLocalObjectManager::getPrismServiceId (), NTPCLOCKSETSERVERKEY)
    {
    }

    NTPClockSetServerKeyMessage::NTPClockSetServerKeyMessage (const string &IpAddrList,const string &server,const UI32 &Key_ID,const string &Key_Value)
        : PrismMessage (NTPClockLocalObjectManager::getPrismServiceId (), NTPCLOCKSETSERVERKEY),
        m_IpAddrList    (IpAddrList),
        m_server    (server),
        m_Key_ID    (Key_ID),
        m_Key_Value    (Key_Value)
    {
    }

    NTPClockSetServerKeyMessage::~NTPClockSetServerKeyMessage ()
    {
    }

    void  NTPClockSetServerKeyMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_IpAddrList,"IpAddrList"));
        addSerializableAttribute (new AttributeString(&m_server,"server"));
        addSerializableAttribute (new AttributeUI32(&m_Key_ID,"Key_ID"));
        addSerializableAttribute (new AttributeString(&m_Key_Value,"Key_Value"));
    }

    void  NTPClockSetServerKeyMessage::setIpAddrList(const string &IpAddrList)
    {
        m_IpAddrList  =  IpAddrList;
    }

    string  NTPClockSetServerKeyMessage::getIpAddrList() const
    {
        return (m_IpAddrList);
    }

    void  NTPClockSetServerKeyMessage::setServer(const string &server)
    {
        m_server  =  server;
    }

    string  NTPClockSetServerKeyMessage::getServer() const
    {
        return (m_server);
    }

    void  NTPClockSetServerKeyMessage::setKey_ID(const UI32 &Key_ID)
    {
        m_Key_ID  =  Key_ID;
    }

    UI32  NTPClockSetServerKeyMessage::getKey_ID() const
    {
        return (m_Key_ID);
    }

    void  NTPClockSetServerKeyMessage::setKey_Value(const string &Key_Value)
    {
        m_Key_Value  =  Key_Value;
    }

    string  NTPClockSetServerKeyMessage::getKey_Value() const
    {
        return (m_Key_Value);
    }

}
