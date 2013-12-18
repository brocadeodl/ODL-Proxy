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
#include "ClientInterface/Rmon/RmonClientCreateRmonEventMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Rmon/rmon_common.h"

namespace DcmNs
{

    RmonClientCreateRmonEventMessage::RmonClientCreateRmonEventMessage ()
        : DcmManagementInterfaceMessage (RMONCLIENTCREATERMONEVENT)
    {
    }

    RmonClientCreateRmonEventMessage::RmonClientCreateRmonEventMessage (const SI32 &eventIndex,const bool &log,const string &communityName,const string &description,const string &owner)
        : DcmManagementInterfaceMessage (RMONCLIENTCREATERMONEVENT),
        m_eventIndex    (eventIndex),
        m_log    (log),
        m_communityName    (communityName),
        m_description    (description),
        m_owner    (owner)
    {
    }

    RmonClientCreateRmonEventMessage::~RmonClientCreateRmonEventMessage ()
    {
    }

    void  RmonClientCreateRmonEventMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_eventIndex,"eventIndex"));
        addSerializableAttribute (new AttributeBool(&m_log,"log"));
        addSerializableAttribute (new AttributeString(&m_communityName,"communityName"));
        addSerializableAttribute (new AttributeString(&m_description,"description"));
        addSerializableAttribute (new AttributeString(&m_owner,"owner"));
    }

    void  RmonClientCreateRmonEventMessage::setEventIndex(const SI32 &eventIndex)
    {
        m_eventIndex  =  eventIndex;
    }

    SI32  RmonClientCreateRmonEventMessage::getEventIndex() const
    {
        return (m_eventIndex);
    }

    void  RmonClientCreateRmonEventMessage::setLog(const bool &log)
    {
        m_log  =  log;
    }

    bool RmonClientCreateRmonEventMessage::getLog() const
    {
        return (m_log);
    }

    void  RmonClientCreateRmonEventMessage::setCommunityName(const string &communityName)
    {
        m_communityName  =  communityName;
    }

    string  RmonClientCreateRmonEventMessage::getCommunityName() const
    {
        return (m_communityName);
    }

    void  RmonClientCreateRmonEventMessage::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  RmonClientCreateRmonEventMessage::getDescription() const
    {
        return (m_description);
    }

    void  RmonClientCreateRmonEventMessage::setOwner(const string &owner)
    {
        m_owner  =  owner;
    }

    string  RmonClientCreateRmonEventMessage::getOwner() const
    {
        return (m_owner);
    }
	const void *RmonClientCreateRmonEventMessage::getCStructureForInputs ()
	{	
		rmon_event_message_t *pInput = (rmon_event_message_t*)malloc(sizeof(rmon_event_message_t));
	    pInput->eventIndex= m_eventIndex;
		pInput->log= m_log;
		CopyCString(pInput->communityName, m_communityName);
		CopyCString(pInput->description, m_description);
		CopyCString(pInput->owner, m_owner);

		return (pInput);
	}
	const void RmonClientCreateRmonEventMessage::CopyCString(char* dst, string& src)
	{
		strncpy(dst, src.c_str(), RMON_MAX_WORD_LEN);
		dst[RMON_MAX_WORD_LEN - 1] = '\0';
	}	

	void RmonClientCreateRmonEventMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}

}
