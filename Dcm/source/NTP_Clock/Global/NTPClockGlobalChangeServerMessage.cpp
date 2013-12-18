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

#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "NTP_Clock/Global/NTPClockGlobalChangeServerMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalObjectManager.h"
#include "NTP_Clock/Global/NTPClockGlobalTypes.h"

namespace DcmNs
{

    NTPClockGlobalChangeServerMessage::NTPClockGlobalChangeServerMessage ()
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALCHANGESERVER)
    {
    }

    NTPClockGlobalChangeServerMessage::NTPClockGlobalChangeServerMessage (const IpVxAddress &IpAddr,const UI32 &Key)
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALCHANGESERVER),
        m_IpAddr    (IpAddr),
        m_Key    (Key)
    {
    }

    NTPClockGlobalChangeServerMessage::~NTPClockGlobalChangeServerMessage ()
    {
    }

    void  NTPClockGlobalChangeServerMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_index,"index"));
        addSerializableAttribute (new AttributeIpVxAddress(&m_IpAddr,"IpAddr"));
        addSerializableAttribute (new AttributeUI32(&m_Key,"Key"));
    }

    void  NTPClockGlobalChangeServerMessage::setIndex(const UI32 &index)
	{
		m_index  =  index;
	}

    UI32  NTPClockGlobalChangeServerMessage::getIndex() const
	{
        return (m_index);
	}

    void  NTPClockGlobalChangeServerMessage::setIpAddr(const IpVxAddress &IpAddr)
    {
        m_IpAddr  =  IpAddr;
    }

    IpVxAddress  NTPClockGlobalChangeServerMessage::getIpAddr() const
    {
        return (m_IpAddr);
    }

    void  NTPClockGlobalChangeServerMessage::setKey(const UI32 &Key)
    {
        m_Key  =  Key;
    }

    UI32  NTPClockGlobalChangeServerMessage::getKey() const
    {
        return (m_Key);
    }

}
