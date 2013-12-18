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

#include "NTP_Clock/Global/NTPClockGlobalAddKeyMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalObjectManager.h"
#include "NTP_Clock/Global/NTPClockGlobalTypes.h"

namespace DcmNs
{

    NTPClockGlobalAddKeyMessage::NTPClockGlobalAddKeyMessage ()
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALADDKEY)
    {
    }

    NTPClockGlobalAddKeyMessage::NTPClockGlobalAddKeyMessage (const UI32 &Key_ID,const string &Key_Value)
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALADDKEY),
        m_Key_ID    (Key_ID),
        m_Key_Value    (Key_Value)
    {
    }

    NTPClockGlobalAddKeyMessage::~NTPClockGlobalAddKeyMessage ()
    {
    }

    void  NTPClockGlobalAddKeyMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_index,"index"));
        addSerializableAttribute (new AttributeUI32(&m_Key_ID,"Key_ID"));
        addSerializableAttribute (new AttributeString(&m_Key_Value,"Key_Value"));
    }

	void  NTPClockGlobalAddKeyMessage::setIndex(const UI32 &index)
    {   
        m_index  =  index;
    }   

    UI32  NTPClockGlobalAddKeyMessage::getIndex() const
    {   
        return (m_index);
    }

    void  NTPClockGlobalAddKeyMessage::setKey_ID(const UI32 &Key_ID)
    {
        m_Key_ID  =  Key_ID;
    }

    UI32  NTPClockGlobalAddKeyMessage::getKey_ID() const
    {
        return (m_Key_ID);
    }

    void  NTPClockGlobalAddKeyMessage::setKey_Value(const string &Key_Value)
    {
        m_Key_Value  =  Key_Value;
    }

    string  NTPClockGlobalAddKeyMessage::getKey_Value() const
    {
        return (m_Key_Value);
    }

}
