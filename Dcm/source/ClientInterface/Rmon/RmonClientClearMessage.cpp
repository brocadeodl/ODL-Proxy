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
 *   Author : Venkat                                                       *
 **************************************************************************/

#include "ClientInterface/Rmon/RmonClientClearMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Rmon/rmon_common.h"

namespace DcmNs
{

    RmonClientClearMessage::RmonClientClearMessage ()
        : DcmManagementInterfaceMessage (RMONCLIENTCLEAR)
    {
    }

    RmonClientClearMessage::RmonClientClearMessage (const SI32 &statsId, const SI32 &cmdCode)
        : DcmManagementInterfaceMessage (RMONCLIENTCLEAR),
        m_statsId	(statsId),
        m_cmdCode	(cmdCode)
    {
    }

    RmonClientClearMessage::~RmonClientClearMessage ()
    {
    }

    void  RmonClientClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_statsId,"statsId"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
    }

    void  RmonClientClearMessage::setStatsId(const SI32 &statsId)
    {
        m_statsId =  statsId;
    }

    SI32  RmonClientClearMessage::getStatsId() const
    {
        return (m_statsId);
    }

    void  RmonClientClearMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode =  cmdCode;
    }

    SI32  RmonClientClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	const void *RmonClientClearMessage::getCStructureForInputs ()
    {
        RmonClearInputMsg_t *pInput = (RmonClearInputMsg_t *)malloc(sizeof(RmonClearInputMsg_t));
        pInput->statsId = m_statsId;
        pInput->cmdCode = m_cmdCode;
        return (pInput);
    }

    void RmonClientClearMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
