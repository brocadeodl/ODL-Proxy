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
 *   .Author : pasu                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmEventMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmEventMessage::NsmEventMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMEVENT)
    {
		m_eventId = 0;
		m_parameter = 0;
    }

    NsmEventMessage::NsmEventMessage (const UI32 &event, const UI32 &parameter)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMEVENT),
        m_eventId    (event),
        m_parameter	 (parameter)
    {
    }

    NsmEventMessage::~NsmEventMessage ()
    {
    }

    void  NsmEventMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_eventId,"eventid"));
        addSerializableAttribute (new AttributeUI32(&m_parameter,"parameter"));
    }


    void  NsmEventMessage::setEventId(const UI32 &eventId)
    {
        m_eventId  =  eventId;
    }

    UI32  NsmEventMessage::getEventId() const
    {
        return (m_eventId);
    }

    void  NsmEventMessage::setParameter(const UI32 &parameter)
    {
        m_parameter  =  parameter;
    }

    UI32  NsmEventMessage::getParameter() const
    {
        return (m_parameter);
    }
}
