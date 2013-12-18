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
 *  .Author : pbalacha                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Rmon/RmonCreateRmonCollectionStatisticsMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    RmonCreateRmonCollectionStatisticsMessage::RmonCreateRmonCollectionStatisticsMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),RMONCREATERMONCOLLECTIONSTATISTICS)
    {
		m_statsIndex = 0;
		m_ifType = IF_TYPE_INVALID;
    }

    RmonCreateRmonCollectionStatisticsMessage::RmonCreateRmonCollectionStatisticsMessage
	  (const string &ifName,const SI32 &statsIndex,const string &owner)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),RMONCREATERMONCOLLECTIONSTATISTICS),
	  	m_ifName    (ifName),
        m_statsIndex (statsIndex),
        m_owner    (owner)
    {
    	m_ifType = IF_TYPE_INVALID;
    }

    RmonCreateRmonCollectionStatisticsMessage::~RmonCreateRmonCollectionStatisticsMessage ()
    {
    }

    void  RmonCreateRmonCollectionStatisticsMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeSI32(&m_statsIndex,"statsIndex"));
        addSerializableAttribute (new AttributeString(&m_owner,"owner"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
    }

    void  RmonCreateRmonCollectionStatisticsMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  RmonCreateRmonCollectionStatisticsMessage::getIfType() const
    {
        return (m_ifType);
    }

	void  RmonCreateRmonCollectionStatisticsMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  RmonCreateRmonCollectionStatisticsMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  RmonCreateRmonCollectionStatisticsMessage::setStatsIndex(const SI32 &statsIndex)
    {
        m_statsIndex =  statsIndex;
    }

    SI32 RmonCreateRmonCollectionStatisticsMessage::getStatsIndex() const
    {
        return (m_statsIndex);
    }

    void  RmonCreateRmonCollectionStatisticsMessage::setOwner(const string &owner)
    {
        m_owner  =  owner;
    }

    string  RmonCreateRmonCollectionStatisticsMessage::getOwner() const
    {
        return (m_owner);
    }
}
