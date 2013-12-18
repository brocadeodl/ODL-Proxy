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

#include "NTP_Clock/Global/NTPClockGlobalGetClockMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalObjectManager.h"
#include "NTP_Clock/Global/NTPClockGlobalTypes.h"

namespace DcmNs
{

    NTPClockGlobalGetClockMessage::NTPClockGlobalGetClockMessage ()
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALGETCLOCK)
    {
    }

    NTPClockGlobalGetClockMessage::NTPClockGlobalGetClockMessage (const DateTime &ClockDateTime,const UI32 &MappedId, const string &ClockTimeZone)
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALGETCLOCK),
        m_ClockDateTime    (ClockDateTime),
        m_MappedId    (MappedId),
        m_ClockTimeZone    (ClockTimeZone)
    {
        m_NumVectors = 1;
    }

    NTPClockGlobalGetClockMessage::NTPClockGlobalGetClockMessage (const vector<DateTime> &ClockDateTime,const vector<UI32> &MappedId, const vector<string> &ClockTimeZone)
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALGETCLOCK),
        m_vClockDateTime    (ClockDateTime),
        m_vMappedId    (MappedId),
        m_vClockTimeZone    (ClockTimeZone)
    {
        m_NumVectors = 0;
    }

    NTPClockGlobalGetClockMessage::~NTPClockGlobalGetClockMessage ()
    {
    }

    void  NTPClockGlobalGetClockMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeDateTime(&m_ClockDateTime,"ClockDateTime"));
        addSerializableAttribute (new AttributeUI32(&m_MappedId,"SwitchId"));
        addSerializableAttribute (new AttributeString(&m_ClockTimeZone,"ClockTimeZone"));
        addSerializableAttribute (new AttributeUI32(&m_NumVectors,"NumVectors"));
        addSerializableAttribute (new AttributeDateTimeVector(&m_vClockDateTime,"vClockDateTime"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vMappedId,"vSwitchId"));
        addSerializableAttribute (new AttributeStringVector(&m_vClockTimeZone,"vClockTimeZone"));
    }

    void  NTPClockGlobalGetClockMessage::setClockDateTime(const DateTime &ClockDateTime)
    {
        m_ClockDateTime  =  ClockDateTime;

        m_vClockDateTime.push_back(ClockDateTime);
        m_NumVectors = 1;
    }

    DateTime  NTPClockGlobalGetClockMessage::getClockDateTime() const
    {
        return (m_ClockDateTime);
    }

    void  NTPClockGlobalGetClockMessage::setMappedId(const UI32 &MappedId)
    {
        m_MappedId  =  MappedId;

        m_vMappedId.push_back(MappedId);
        m_NumVectors = 1;
    }

    UI32  NTPClockGlobalGetClockMessage::getMappedId() const
    {
        return (m_MappedId);
    }

    void  NTPClockGlobalGetClockMessage::setClockTimeZone(const string &ClockTimeZone)
    {
        m_ClockTimeZone  =  ClockTimeZone;

        m_vClockTimeZone.push_back(ClockTimeZone);
        m_NumVectors = 1;
    }

    string  NTPClockGlobalGetClockMessage::getClockTimeZone() const
    {
        return (m_ClockTimeZone);
    }

    UI32  NTPClockGlobalGetClockMessage::getNumVectors() const
    {
        return (m_NumVectors);
    }

	void NTPClockGlobalGetClockMessage::clearNumVectors()
	{
		m_NumVectors = 0;
        m_vClockDateTime.clear();
        m_vMappedId.clear();
		m_vClockTimeZone.clear();
	}

    void  NTPClockGlobalGetClockMessage::setVectorClockDateTime(const DateTime &ClockDateTime, const UI32 &MappedId, const string &ClockTimeZone)
    {
        m_vClockDateTime.push_back(ClockDateTime);
        m_vMappedId.push_back(MappedId);
        m_vClockTimeZone.push_back(ClockTimeZone);
        m_NumVectors++;
    }

    bool NTPClockGlobalGetClockMessage::getVectorClockDateTime(const UI32 &index, DateTime &ClockDateTime, UI32 &MappedId, string &ClockTimeZone)
    {
        if (index >= m_NumVectors)
        {
            return false;
        }

        ClockDateTime = m_vClockDateTime[index];
        MappedId = m_vMappedId[index];
        ClockTimeZone = m_vClockTimeZone[index];

        return true;
    }
}
