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

#include "ClientInterface/Rmon/RmonClientDeleteCollectionHistoryMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Rmon/rmon_common.h"

namespace DcmNs
{

    RmonClientDeleteCollectionHistoryMessage::RmonClientDeleteCollectionHistoryMessage ()
        : DcmManagementInterfaceMessage(RMONCLIENTDELETECOLLECTIONHISTORY)
    {
    }

    RmonClientDeleteCollectionHistoryMessage::RmonClientDeleteCollectionHistoryMessage (const string &ifName,const SI32 &historyIndex)
        : DcmManagementInterfaceMessage (RMONCLIENTDELETECOLLECTIONHISTORY),
		m_ifName    (ifName),
        m_historyIndex    (historyIndex)
    {
    }

    RmonClientDeleteCollectionHistoryMessage::~RmonClientDeleteCollectionHistoryMessage ()
    {
    }

    void  RmonClientDeleteCollectionHistoryMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeSI32(&m_historyIndex,"historyIndex"));
    }

	void  RmonClientDeleteCollectionHistoryMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  RmonClientDeleteCollectionHistoryMessage::getIfName() const
    {
        return (m_ifName);
    }


    void  RmonClientDeleteCollectionHistoryMessage::setHistoryIndex(const SI32 &historyIndex)
    {
        m_historyIndex  =  historyIndex;
    }

    SI32  RmonClientDeleteCollectionHistoryMessage::getHistoryIndex() const
    {
        return (m_historyIndex);
    }

	const void *RmonClientDeleteCollectionHistoryMessage::getCStructureForInputs ()
    {
		rmon_history_message_t *pInput = (rmon_history_message_t*)malloc(sizeof(rmon_history_message_t));
		pInput->history_index = m_historyIndex;
		pInput->buckets = 0;
		pInput->interval = 0;
		strncpy(pInput->ifname, m_ifName.c_str(), DCM_IFNAME_LENGTH);
		pInput->ifname[DCM_IFNAME_LENGTH] = '\0';
		strncpy(pInput->owner, DEFAULT_OWNER, RMON_MAX_WORD_LEN);
		pInput->owner[RMON_MAX_WORD_LEN] = '\0';
        return (pInput);
    }

	void RmonClientDeleteCollectionHistoryMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
