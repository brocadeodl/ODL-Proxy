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
 *   Author : dchakrab                                                     *
 **************************************************************************/

#include "Rmon/Local/RmonLocalObjectManager.h"
#include "Rmon/Local/RmonClearMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Rmon/Local/RmonTypes.h"

namespace DcmNs
{

    RmonClearMessage::RmonClearMessage() : PrismMessage (RmonLocalObjectManager::getPrismServiceId(),RMON_LOCAL_CLEAR_MSG)
    {
    }

    RmonClearMessage::RmonClearMessage(const SI32 &statsIndex, const SI32 &cmdCode, const bool isGwRequest) : PrismMessage (RmonLocalObjectManager::getPrismServiceId(), RMON_LOCAL_CLEAR_MSG),
		m_statsIndex (statsIndex),
		m_cmdCode   (cmdCode),
		m_isGwRequest  (isGwRequest)
    {
    }

    RmonClearMessage::~RmonClearMessage()
    {
    }
    void  RmonClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_statsIndex,"statsIndex"));
		addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
    }

    void  RmonClearMessage::setStatsId(const SI32 &statsId)
    {
        m_statsIndex =  statsId;
    }

    SI32  RmonClearMessage::getStatsId() const
    {
        return (m_statsIndex);
    }

    void  RmonClearMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode =  cmdCode;
    }

    SI32  RmonClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    void RmonClearMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool RmonClearMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }
}
