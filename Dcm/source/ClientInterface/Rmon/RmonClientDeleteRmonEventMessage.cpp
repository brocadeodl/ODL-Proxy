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

#include "Rmon/Local/RmonLocalObjectManager.h"
#include "ClientInterface/Rmon/RmonClientDeleteRmonEventMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    RmonClientDeleteRmonEventMessage::RmonClientDeleteRmonEventMessage ()
        : DcmManagementInterfaceMessage (RMONCLIENTDELETERMONEVENT)
    {
    }

    RmonClientDeleteRmonEventMessage::RmonClientDeleteRmonEventMessage (const SI32 &eventIndex)
        : DcmManagementInterfaceMessage (RMONCLIENTDELETERMONEVENT),
        m_eventIndex    (eventIndex)
    {
    }

    RmonClientDeleteRmonEventMessage::~RmonClientDeleteRmonEventMessage ()
    {
    }

    void  RmonClientDeleteRmonEventMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_eventIndex,"eventIndex"));
    }

    void  RmonClientDeleteRmonEventMessage::setEventIndex(const SI32 &eventIndex)
    {
        m_eventIndex  =  eventIndex;
    }

    SI32 RmonClientDeleteRmonEventMessage::getEventIndex() const
    {
        return (m_eventIndex);
    }
	 const void *RmonClientDeleteRmonEventMessage::getCStructureForInputs ()
	 {
		 int *pInput = (int*)malloc(sizeof(int));
		 *pInput = m_eventIndex;
		 return (pInput);
	 }

	 void RmonClientDeleteRmonEventMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	 {
	 }

}
