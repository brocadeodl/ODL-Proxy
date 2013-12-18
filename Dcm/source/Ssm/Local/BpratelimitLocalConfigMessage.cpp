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
 *   Author : vsatyana                                                     *
 **************************************************************************/

#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/SsmTypes.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "Ssm/Local/BpratelimitLocalConfigMessage.h"

namespace DcmNs
{

    BpratelimitLocalConfigMessage::BpratelimitLocalConfigMessage ()
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (),BPRATELIMITLOCALCONFIG)
    {
    }

    BpratelimitLocalConfigMessage::BpratelimitLocalConfigMessage (const UI32 &slotId,const UI32 &opCode)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (),BPRATELIMITLOCALCONFIG),
        m_slotId    (slotId),
        m_opCode    (opCode)
    {
    }

    BpratelimitLocalConfigMessage::~BpratelimitLocalConfigMessage ()
    {
    }

    void  BpratelimitLocalConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_slotId,"slotId"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }


    void  BpratelimitLocalConfigMessage::setSlotId(const UI32 &slotId)
    {
        m_slotId  =  slotId;
    }

    UI32  BpratelimitLocalConfigMessage::getSlotId() const
    {
        return (m_slotId);
    }
    void  BpratelimitLocalConfigMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  BpratelimitLocalConfigMessage::getMode() const
    {
        return (m_mode);
    }

    void  BpratelimitLocalConfigMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  BpratelimitLocalConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
