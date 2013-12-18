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

#include "NTP_Clock/Global/NTPClockGlobalSetZoneMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalObjectManager.h"
#include "NTP_Clock/Global/NTPClockGlobalTypes.h"
#include "NTP_Clock/Global/timezone-list.h"

namespace DcmNs
{

    NTPClockGlobalSetZoneMessage::NTPClockGlobalSetZoneMessage ()
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALSETZONE)
    {
    }

    NTPClockGlobalSetZoneMessage::NTPClockGlobalSetZoneMessage (const UI32 &Timezone)
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALSETZONE),
        m_Timezone    (Timezone)
    {
    }

    NTPClockGlobalSetZoneMessage::~NTPClockGlobalSetZoneMessage ()
    {
    }

    void  NTPClockGlobalSetZoneMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_Timezone,"Timezone"));
    }

	string NTPClockGlobalSetZoneMessage::convertZoneEnumToStr(const UI32 &Timezone)
	{
		return getTimezoneStr(Timezone);
	}

    void  NTPClockGlobalSetZoneMessage::setTimezone(const UI32 &Timezone)
    {
        m_Timezone  =  Timezone;
		m_Timestr	=  convertZoneEnumToStr(Timezone);
    }

    UI32  NTPClockGlobalSetZoneMessage::getTimezone() const
    {
        return (m_Timezone);
    }

    string  NTPClockGlobalSetZoneMessage::getTimeStr() const
    {
        return (m_Timestr);
    }

}
