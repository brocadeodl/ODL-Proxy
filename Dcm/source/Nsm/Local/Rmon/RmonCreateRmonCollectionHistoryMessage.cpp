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
 *   .Author : pbalacha                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Rmon/RmonCreateRmonCollectionHistoryMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    RmonCreateRmonCollectionHistoryMessage::RmonCreateRmonCollectionHistoryMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),RMONCREATERMONCOLLECTIONHISTORY)
    {
		m_buckets = 0;
		m_historyIndex = 0;
		m_interval = 0;
		m_ifType = IF_TYPE_INVALID;
    }

    RmonCreateRmonCollectionHistoryMessage::RmonCreateRmonCollectionHistoryMessage (const string &ifName,const SI32 &historyIndex,const SI32 &buckets,
		const SI32 &interval,const string &owner)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),RMONCREATERMONCOLLECTIONHISTORY),
	  	m_ifName    (ifName),
        m_historyIndex    (historyIndex),
        m_buckets    (buckets),
        m_interval    (interval),
        m_owner    (owner)
    {
		m_ifType = IF_TYPE_INVALID;
    }

    RmonCreateRmonCollectionHistoryMessage::~RmonCreateRmonCollectionHistoryMessage ()
    {
    }

    void  RmonCreateRmonCollectionHistoryMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeSI32(&m_historyIndex,"historyIndex"));
        addSerializableAttribute (new AttributeSI32(&m_buckets,"buckets"));
        addSerializableAttribute (new AttributeSI32(&m_interval,"interval"));
        addSerializableAttribute (new AttributeString(&m_owner,"owner"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
    }

    void  RmonCreateRmonCollectionHistoryMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  RmonCreateRmonCollectionHistoryMessage::getIfType() const
    {
        return (m_ifType);
    }

	void  RmonCreateRmonCollectionHistoryMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  RmonCreateRmonCollectionHistoryMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  RmonCreateRmonCollectionHistoryMessage::setHistoryIndex(const SI32 &historyIndex)
    {
        m_historyIndex  =  historyIndex;
    }

    SI32  RmonCreateRmonCollectionHistoryMessage::getHistoryIndex() const
    {
        return (m_historyIndex);
    }

    void  RmonCreateRmonCollectionHistoryMessage::setBuckets(const SI32 &buckets)
    {
        m_buckets  =  buckets;
    }

    SI32  RmonCreateRmonCollectionHistoryMessage::getBuckets() const
    {
        return (m_buckets);
    }

    void  RmonCreateRmonCollectionHistoryMessage::setInterval(const SI32 &interval)
    {
        m_interval  =  interval;
    }

    SI32  RmonCreateRmonCollectionHistoryMessage::getInterval() const
    {
        return (m_interval);
    }

    void  RmonCreateRmonCollectionHistoryMessage::setOwner(const string &owner)
    {
        m_owner  =  owner;
    }

    string  RmonCreateRmonCollectionHistoryMessage::getOwner() const
    {
        return (m_owner);
    }

}
