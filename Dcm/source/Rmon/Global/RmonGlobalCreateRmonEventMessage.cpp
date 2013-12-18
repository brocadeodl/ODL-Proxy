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

#include "Rmon/Global/RmonGlobalCreateRmonEventMessage.h"
#include "Rmon/Global/RmonGlobalObjectManager.h"
#include "Rmon/Global/RmonGlobalTypes.h"

namespace DcmNs
{

    RmonGlobalCreateRmonEventMessage::RmonGlobalCreateRmonEventMessage ()
        : ManagementInterfaceMessage (RmonGlobalObjectManager::getClassName (),RMONGLOBALCREATERMONEVENT)
    {
    }

    RmonGlobalCreateRmonEventMessage::RmonGlobalCreateRmonEventMessage (const SI32 &eventIndex,const bool &log,const string &communityName,const string &description,const string &owner)
        : ManagementInterfaceMessage (RmonGlobalObjectManager::getClassName (),RMONGLOBALCREATERMONEVENT),
        m_eventIndex    (eventIndex),
        m_log    (log),
        m_communityName    (communityName),
        m_description    (description),
        m_owner    (owner)
    {
    }

    RmonGlobalCreateRmonEventMessage::~RmonGlobalCreateRmonEventMessage ()
    {
    }

    void  RmonGlobalCreateRmonEventMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_eventIndex,"eventIndex"));
        addSerializableAttribute (new AttributeBool(&m_log,"log"));
        addSerializableAttribute (new AttributeString(&m_communityName,"communityName"));
        addSerializableAttribute (new AttributeString(&m_description,"description"));
        addSerializableAttribute (new AttributeString(&m_owner,"owner"));
    }

    void  RmonGlobalCreateRmonEventMessage::setEventIndex(const SI32 &eventIndex)
    {
        m_eventIndex  =  eventIndex;
    }

    SI32  RmonGlobalCreateRmonEventMessage::getEventIndex() const
    {
        return (m_eventIndex);
    }

    void  RmonGlobalCreateRmonEventMessage::setLog(const bool &log)
    {
        m_log  =  log;
    }

    bool RmonGlobalCreateRmonEventMessage::getLog() const
    {
        return (m_log);
    }

    void  RmonGlobalCreateRmonEventMessage::setCommunityName(const string &communityName)
    {
        m_communityName  =  communityName;
    }

    string  RmonGlobalCreateRmonEventMessage::getCommunityName() const
    {
        return (m_communityName);
    }

    void  RmonGlobalCreateRmonEventMessage::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  RmonGlobalCreateRmonEventMessage::getDescription() const
    {
        return (m_description);
    }

    void  RmonGlobalCreateRmonEventMessage::setOwner(const string &owner)
    {
        m_owner  =  owner;
    }

    string  RmonGlobalCreateRmonEventMessage::getOwner() const
    {
        return (m_owner);
    }

}
