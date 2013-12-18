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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "ClientInterface/Rmon/RmonClientCreateCollectionHistoryMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Rmon/rmon_common.h"

namespace DcmNs
{

    RmonClientCreateCollectionHistoryMessage::RmonClientCreateCollectionHistoryMessage ()
        : DcmManagementInterfaceMessage (RMONCLIENTCREATECOLLECTIONHISTORY)
    {
    }

    RmonClientCreateCollectionHistoryMessage::RmonClientCreateCollectionHistoryMessage (const string &ifName,
		const SI32 &historyIndex,const SI32 &buckets, const SI32 &interval,const string &owner)
        : DcmManagementInterfaceMessage (RMONCLIENTCREATECOLLECTIONHISTORY),
	  	m_ifName    (ifName),
        m_historyIndex    (historyIndex),
        m_buckets    (buckets),
        m_interval    (interval),
        m_owner    (owner)
    {
    }

    RmonClientCreateCollectionHistoryMessage::~RmonClientCreateCollectionHistoryMessage ()
    {
    }

    void  RmonClientCreateCollectionHistoryMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeSI32(&m_historyIndex,"historyIndex"));
        addSerializableAttribute (new AttributeSI32(&m_buckets,"buckets"));
        addSerializableAttribute (new AttributeSI32(&m_interval,"interval"));
        addSerializableAttribute (new AttributeString(&m_owner,"owner"));
    }

	void  RmonClientCreateCollectionHistoryMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  RmonClientCreateCollectionHistoryMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  RmonClientCreateCollectionHistoryMessage::setHistoryIndex(const SI32 &historyIndex)
    {
        m_historyIndex  =  historyIndex;
    }

    SI32  RmonClientCreateCollectionHistoryMessage::getHistoryIndex() const
    {
        return (m_historyIndex);
    }

    void  RmonClientCreateCollectionHistoryMessage::setBuckets(const SI32 &buckets)
    {
        m_buckets  =  buckets;
    }

    SI32  RmonClientCreateCollectionHistoryMessage::getBuckets() const
    {
        return (m_buckets);
    }

    void  RmonClientCreateCollectionHistoryMessage::setInterval(const SI32 &interval)
    {
        m_interval  =  interval;
    }

    SI32  RmonClientCreateCollectionHistoryMessage::getInterval() const
    {
        return (m_interval);
    }

    void  RmonClientCreateCollectionHistoryMessage::setOwner(const string &owner)
    {
        m_owner  =  owner;
    }

    string  RmonClientCreateCollectionHistoryMessage::getOwner() const
    {
        return (m_owner);
    }
	const void *RmonClientCreateCollectionHistoryMessage::getCStructureForInputs ()
    {
		rmon_history_message_t *pInput = (rmon_history_message_t*)malloc(sizeof(rmon_history_message_t));
		pInput->history_index = m_historyIndex;
		pInput->buckets = m_buckets;
		pInput->interval = m_interval;
		strncpy(pInput->ifname, m_ifName.c_str(), DCM_IFNAME_LENGTH);
		pInput->ifname[DCM_IFNAME_LENGTH] = '\0';
		strncpy(pInput->owner, m_owner.c_str(), RMON_MAX_WORD_LEN);
		pInput->owner[RMON_MAX_WORD_LEN] = '\0';
        return (pInput);
    }

    void RmonClientCreateCollectionHistoryMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }


}
