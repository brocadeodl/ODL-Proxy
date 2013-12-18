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

#include "NTP_Clock/Local/NTPClockLocalObjectManager.h"
#include "NTP_Clock/Local/NTPClockSetServersMessage.h"
#include "NTP_Clock/Local/NTPClockTypes.h"

namespace DcmNs
{

    NTPClockSetServersMessage::NTPClockSetServersMessage ()
        : PrismMessage (NTPClockLocalObjectManager::getPrismServiceId (), NTPCLOCKSETSERVERS)
    {
    }

    NTPClockSetServersMessage::NTPClockSetServersMessage (const string &IpAddrList)
        : PrismMessage (NTPClockLocalObjectManager::getPrismServiceId (), NTPCLOCKSETSERVERS),
        m_IpAddrList    (IpAddrList)
    {
    }

    NTPClockSetServersMessage::~NTPClockSetServersMessage ()
    {
    }

    void  NTPClockSetServersMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_IpAddrList,"IpAddrList"));
    }

    void  NTPClockSetServersMessage::setIpAddrList(const string &IpAddrList)
    {
        m_IpAddrList  =  IpAddrList;
    }

    string  NTPClockSetServersMessage::getIpAddrList() const
    {
        return (m_IpAddrList);
    }

}
