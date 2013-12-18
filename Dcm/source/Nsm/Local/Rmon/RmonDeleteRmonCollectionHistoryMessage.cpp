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
#include "Nsm/Local/Rmon/RmonDeleteRmonCollectionHistoryMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    RmonDeleteRmonCollectionHistoryMessage::RmonDeleteRmonCollectionHistoryMessage ()
        : PrismMessage(NsmLocalObjectManager::getPrismServiceId(),RMONDELETERMONCOLLECTIONHISTORY)
    {
		m_historyIndex = 0;
		m_ifType = IF_TYPE_INVALID;
    }

    RmonDeleteRmonCollectionHistoryMessage::RmonDeleteRmonCollectionHistoryMessage (const string &ifName,const SI32 &historyIndex)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),RMONDELETERMONCOLLECTIONHISTORY),
		m_ifName    (ifName),
        m_historyIndex    (historyIndex)
    {
		m_ifType = IF_TYPE_INVALID;
    }

    RmonDeleteRmonCollectionHistoryMessage::~RmonDeleteRmonCollectionHistoryMessage ()
    {
    }

    void  RmonDeleteRmonCollectionHistoryMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeSI32(&m_historyIndex,"historyIndex"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
    }

    void  RmonDeleteRmonCollectionHistoryMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  RmonDeleteRmonCollectionHistoryMessage::getIfType() const
    {
        return (m_ifType);
    }
	void  RmonDeleteRmonCollectionHistoryMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  RmonDeleteRmonCollectionHistoryMessage::getIfName() const
    {
        return (m_ifName);
    }


    void  RmonDeleteRmonCollectionHistoryMessage::setHistoryIndex(const SI32 &historyIndex)
    {
        m_historyIndex  =  historyIndex;
    }

    SI32  RmonDeleteRmonCollectionHistoryMessage::getHistoryIndex() const
    {
        return (m_historyIndex);
    }

}
