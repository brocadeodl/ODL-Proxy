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
#include "NTP_Clock/Local/NTPClockSetSwitchClockMessage.h"
#include "NTP_Clock/Local/NTPClockTypes.h"

namespace DcmNs
{

    NTPClockSetSwitchClockMessage::NTPClockSetSwitchClockMessage ()
        : PrismMessage (NTPClockLocalObjectManager::getPrismServiceId (), NTPCLOCKSETSWITCHCLOCK)
    {
    }

    NTPClockSetSwitchClockMessage::NTPClockSetSwitchClockMessage (const DateTime &ClockDateTime)
        : PrismMessage (NTPClockLocalObjectManager::getPrismServiceId (), NTPCLOCKSETSWITCHCLOCK),
        m_ClockDateTime    (ClockDateTime)
    {
    }

    NTPClockSetSwitchClockMessage::~NTPClockSetSwitchClockMessage ()
    {
    }

    void  NTPClockSetSwitchClockMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeDateTime(&m_ClockDateTime,"ClockDateTime"));
    }

    void  NTPClockSetSwitchClockMessage::setClockDateTime(const DateTime &ClockDateTime)
    {
        m_ClockDateTime  =  ClockDateTime;
    }

    DateTime  NTPClockSetSwitchClockMessage::getClockDateTime() const
    {
        return (m_ClockDateTime);
    }

}
