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
 *   Author : vivekk                                                     *
 **************************************************************************/

#include "Rmon/Global/RmonGlobalDeleteRmonEventMessage.h"
#include "Rmon/Global/RmonGlobalObjectManager.h"
#include "Rmon/Global/RmonGlobalTypes.h"

namespace DcmNs
{

    RmonGlobalDeleteRmonEventMessage::RmonGlobalDeleteRmonEventMessage ()
        : ManagementInterfaceMessage (RmonGlobalObjectManager::getClassName (),RMONGLOBALDELETERMONEVENT)
    {
    }

    RmonGlobalDeleteRmonEventMessage::RmonGlobalDeleteRmonEventMessage (const SI32 &eventIndex)
        : ManagementInterfaceMessage (RmonGlobalObjectManager::getClassName (),RMONGLOBALDELETERMONEVENT),
        m_eventIndex    (eventIndex)
    {
    }

    RmonGlobalDeleteRmonEventMessage::~RmonGlobalDeleteRmonEventMessage ()
    {
    }

    void  RmonGlobalDeleteRmonEventMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_eventIndex,"eventIndex"));
    }

    void  RmonGlobalDeleteRmonEventMessage::setEventIndex(const SI32 &eventIndex)
    {
        m_eventIndex  =  eventIndex;
    }

    SI32  RmonGlobalDeleteRmonEventMessage::getEventIndex() const
    {
        return (m_eventIndex);
    }

}
