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

#include "ClientInterface/Rmon/RmonClientCreateCollectionStatisticsMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Rmon/rmon_common.h"

namespace DcmNs
{

    RmonClientCreateCollectionStatisticsMessage::RmonClientCreateCollectionStatisticsMessage ()
        : DcmManagementInterfaceMessage (RMONCLIENTCREATECOLLECTIONSTATISTICS)
    {
    }

    RmonClientCreateCollectionStatisticsMessage::RmonClientCreateCollectionStatisticsMessage
	  (const string &ifName,const SI32 &statsIndex,const string &owner)
        : DcmManagementInterfaceMessage (RMONCLIENTCREATECOLLECTIONSTATISTICS),
	  	m_ifName    (ifName),
        m_statsIndex (statsIndex),
        m_owner    (owner)
    {
    }

    RmonClientCreateCollectionStatisticsMessage::~RmonClientCreateCollectionStatisticsMessage ()
    {
    }

    void  RmonClientCreateCollectionStatisticsMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeSI32(&m_statsIndex,"statsIndex"));
        addSerializableAttribute (new AttributeString(&m_owner,"owner"));
    }

	void  RmonClientCreateCollectionStatisticsMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  RmonClientCreateCollectionStatisticsMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  RmonClientCreateCollectionStatisticsMessage::setStatsIndex(const SI32 &statsIndex)
    {
        m_statsIndex =  statsIndex;
    }

    SI32 RmonClientCreateCollectionStatisticsMessage::getStatsIndex() const
    {
        return (m_statsIndex);
    }

    void  RmonClientCreateCollectionStatisticsMessage::setOwner(const string &owner)
    {
        m_owner  =  owner;
    }

    string  RmonClientCreateCollectionStatisticsMessage::getOwner() const
    {
        return (m_owner);
    }

	const void *RmonClientCreateCollectionStatisticsMessage::getCStructureForInputs ()
    {
		rmon_stats_message_t *pInput = (rmon_stats_message_t*)malloc(sizeof(rmon_stats_message_t));
		pInput->stats_index = m_statsIndex;
		strncpy(pInput->ifname, m_ifName.c_str(), DCM_IFNAME_LENGTH);
		pInput->ifname[DCM_IFNAME_LENGTH] = '\0';
		strncpy(pInput->owner, m_owner.c_str(), RMON_MAX_WORD_LEN);
		pInput->owner[RMON_MAX_WORD_LEN] = '\0';
        return (pInput);
    }

	void RmonClientCreateCollectionStatisticsMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }


}
