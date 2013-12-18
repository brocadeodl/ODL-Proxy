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
#include "NTP_Clock/Local/NTPClockGetSwitchClockMessage.h"
#include "NTP_Clock/Local/NTPClockTypes.h"

namespace DcmNs
{

    NTPClockGetSwitchClockMessage::NTPClockGetSwitchClockMessage ()
        : PrismMessage (NTPClockLocalObjectManager::getPrismServiceId (), NTPCLOCKGETSWITCHCLOCK)
    {
    }

    NTPClockGetSwitchClockMessage::NTPClockGetSwitchClockMessage (const DateTime &ClockDateTime, const string &ClockTimeZone)
        : PrismMessage (NTPClockLocalObjectManager::getPrismServiceId (), NTPCLOCKGETSWITCHCLOCK),
        m_ClockDateTime    (ClockDateTime), m_ClockTimeZone    (ClockTimeZone)
    {
    }

    NTPClockGetSwitchClockMessage::~NTPClockGetSwitchClockMessage ()
    {
    }

    void  NTPClockGetSwitchClockMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeDateTime(&m_ClockDateTime,"ClockDateTime"));
        addSerializableAttribute (new AttributeString(&m_ClockTimeZone,"ClockTimeZone"));
    }

    void  NTPClockGetSwitchClockMessage::setClockDateTime(const DateTime &ClockDateTime)
    {
        m_ClockDateTime  =  ClockDateTime;
    }

    DateTime  NTPClockGetSwitchClockMessage::getClockDateTime() const
    {
        return (m_ClockDateTime);
    }

    void  NTPClockGetSwitchClockMessage::setClockTimeZone(const string &ClockTimeZone)
    {
        m_ClockTimeZone  =  ClockTimeZone;
    }

    string  NTPClockGetSwitchClockMessage::getClockTimeZone() const
    {
        return (m_ClockTimeZone);
    }

}
