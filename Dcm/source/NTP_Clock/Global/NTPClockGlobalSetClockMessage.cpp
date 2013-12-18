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

#include "NTP_Clock/Global/NTPClockGlobalSetClockMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalObjectManager.h"
#include "NTP_Clock/Global/NTPClockGlobalTypes.h"

namespace DcmNs
{

    NTPClockGlobalSetClockMessage::NTPClockGlobalSetClockMessage ()
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALSETCLOCK)
    {
    }

    NTPClockGlobalSetClockMessage::NTPClockGlobalSetClockMessage (const DateTime &ClockDateTime)
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALSETCLOCK),
        m_ClockDateTime    (ClockDateTime)
    {
    }

    NTPClockGlobalSetClockMessage::~NTPClockGlobalSetClockMessage ()
    {
    }

    void  NTPClockGlobalSetClockMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeDateTime(&m_ClockDateTime,"ClockDateTime"));
    }

    void  NTPClockGlobalSetClockMessage::setClockDateTime(const DateTime &ClockDateTime)
    {
        m_ClockDateTime  =  ClockDateTime;
    }

    DateTime  NTPClockGlobalSetClockMessage::getClockDateTime() const
    {
        return (m_ClockDateTime);
    }

}
