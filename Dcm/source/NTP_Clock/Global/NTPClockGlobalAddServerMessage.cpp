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
 *   Author : noku                                                     *
 **************************************************************************/

#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "NTP_Clock/Global/NTPClockGlobalAddServerMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalObjectManager.h"
#include "NTP_Clock/Global/NTPClockGlobalTypes.h"

namespace DcmNs
{

    NTPClockGlobalAddServerMessage::NTPClockGlobalAddServerMessage ()
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALADDSERVER)
    {
    }

    NTPClockGlobalAddServerMessage::NTPClockGlobalAddServerMessage (const IpVxAddress &IpAddr, const UI32 &Key)
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALADDSERVER),
        m_IpAddr    (IpAddr),
        m_Key       (Key)
    {
    }

    NTPClockGlobalAddServerMessage::~NTPClockGlobalAddServerMessage ()
    {
    }

    void  NTPClockGlobalAddServerMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_index,"index"));
        addSerializableAttribute (new AttributeIpVxAddress(&m_IpAddr,"IpAddr"));
        addSerializableAttribute (new AttributeUI32(&m_Key, "Key"));
    }

    void  NTPClockGlobalAddServerMessage::setIndex(const UI32 &index)
    {
        m_index  =  index;
    }

    UI32  NTPClockGlobalAddServerMessage::getIndex() const
    {
        return (m_index);
    }

    void  NTPClockGlobalAddServerMessage::setIpAddr(const IpVxAddress &IpAddr)
    {
        m_IpAddr  =  IpAddr;
    }

    IpVxAddress  NTPClockGlobalAddServerMessage::getIpAddr() const
    {
        return (m_IpAddr);
    }

    void NTPClockGlobalAddServerMessage::setKey(const UI32 &Key)
    {   
        m_Key = Key;
    }
    
    UI32 NTPClockGlobalAddServerMessage::getKey() const
    {   
        return (m_Key);
    }

}

