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

#include "Rmon/Global/RmonEventManagedObject.h"
#include "Rmon/Global/RmonGlobalObjectManager.h"
#include "Rmon/Global/RmonGlobalTypes.h"
#include "vcs.h"

namespace DcmNs
{

    RmonEventManagedObject::RmonEventManagedObject (RmonGlobalObjectManager *pRmonGlobalObjectManager)
        : PrismElement  (pRmonGlobalObjectManager),
        PrismPersistableObject (RmonEventManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pRmonGlobalObjectManager),
        DcmManagedObject (pRmonGlobalObjectManager)
    {
    }

    RmonEventManagedObject::RmonEventManagedObject (RmonGlobalObjectManager *pRmonGlobalObjectManager,const SI32  &eventIndex,const bool &log,const string &communityName,const string &description,const string &owner)
        : PrismElement  (pRmonGlobalObjectManager),
        PrismPersistableObject (RmonEventManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pRmonGlobalObjectManager),
        DcmManagedObject (pRmonGlobalObjectManager),
        m_eventIndex    (eventIndex),
        m_communityName    (communityName),
        m_description    (description),
        m_owner    (owner),
		m_log   (log)
    {
    }

    RmonEventManagedObject::~RmonEventManagedObject ()
    {
    }

    string  RmonEventManagedObject::getClassName()
    {
        return ("RmonEventManagedObject");
    }

    void  RmonEventManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_eventIndex,"eventIndex"));
        addPersistableAttribute (new AttributeString(&m_communityName,"communityName",rmon_event_community));
        addPersistableAttribute (new AttributeString(&m_description,"description",rmon_event_description));
        addPersistableAttribute (new AttributeString(&m_owner,"owner",rmon_event_owner));
        addPersistableAttribute (new AttributeBool(&m_log,"log",rmon_log));
		setUserTagForAttribute("eventIndex",rmon_event_index);
    }

    void  RmonEventManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_eventIndex,"eventIndex"));
        addPersistableAttributeForCreate  (new AttributeString(&m_communityName,"communityName",rmon_event_community));
        addPersistableAttributeForCreate  (new AttributeString(&m_description,"description",rmon_event_description));
        addPersistableAttributeForCreate  (new AttributeString(&m_owner,"owner",rmon_event_owner));
        addPersistableAttributeForCreate  (new AttributeBool(&m_log,"log",rmon_log));

		vector<string > keyName;
        keyName.push_back ("eventIndex");
        setUserDefinedKeyCombination (keyName);

    }

    void  RmonEventManagedObject::setEventIndex(const SI32 &eventIndex)
    {
        m_eventIndex  =  eventIndex;
    }

    SI32  RmonEventManagedObject::getEventIndex() const
    {
        return (m_eventIndex);
    }

    void  RmonEventManagedObject::setCommunityName(const string &communityName)
    {
        m_communityName  =  communityName;
    }

    string  RmonEventManagedObject::getCommunityName() const
    {
        return (m_communityName);
    }

    void  RmonEventManagedObject::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  RmonEventManagedObject::getDescription() const
    {
        return (m_description);
    }

    void  RmonEventManagedObject::setOwner(const string &owner)
    {
        m_owner  =  owner;
    }

    string  RmonEventManagedObject::getOwner() const
    {
        return (m_owner);
    }

	void RmonEventManagedObject::setLog(const bool &log)
	{
		m_log = log;
	}

    bool RmonEventManagedObject::getLog()  const
	{
        return (m_log);
	}

}
