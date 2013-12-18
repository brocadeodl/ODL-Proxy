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

#include "ClientInterface/Rmon/RmonClientDeleteCollectionStatisticsMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Rmon/rmon_common.h"



namespace DcmNs
{

    RmonClientDeleteCollectionStatisticsMessage::RmonClientDeleteCollectionStatisticsMessage ()
        : DcmManagementInterfaceMessage (RMONCLIENTDELETECOLLECTIONSTATISTICS)
    {
    }

    RmonClientDeleteCollectionStatisticsMessage::RmonClientDeleteCollectionStatisticsMessage
	  (const string &ifName,const SI32 &statsIndex)
        : DcmManagementInterfaceMessage (RMONCLIENTDELETECOLLECTIONSTATISTICS),
        m_statsIndex    (statsIndex)
    {
    }

    RmonClientDeleteCollectionStatisticsMessage::~RmonClientDeleteCollectionStatisticsMessage ()
    {
    }

    void  RmonClientDeleteCollectionStatisticsMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeSI32(&m_statsIndex,"statsIndex"));
    }
	
	void  RmonClientDeleteCollectionStatisticsMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string RmonClientDeleteCollectionStatisticsMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  RmonClientDeleteCollectionStatisticsMessage::setStatsIndex(const SI32 &statsIndex)
    {
        m_statsIndex  =  statsIndex;
    }

    SI32  RmonClientDeleteCollectionStatisticsMessage::getStatsIndex() const
    {
        return (m_statsIndex);
    }

	const void *RmonClientDeleteCollectionStatisticsMessage::getCStructureForInputs ()
    {
		rmon_stats_message_t *pInput = (rmon_stats_message_t*)malloc(sizeof(rmon_stats_message_t));
		pInput->stats_index = m_statsIndex;
		strncpy(pInput->ifname, m_ifName.c_str(), DCM_IFNAME_LENGTH);
		pInput->ifname[DCM_IFNAME_LENGTH] = '\0';
		strncpy(pInput->owner, DEFAULT_OWNER, RMON_MAX_WORD_LEN);
		pInput->owner[RMON_MAX_WORD_LEN] = '\0';
        return (pInput);
    }

    void RmonClientDeleteCollectionStatisticsMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
