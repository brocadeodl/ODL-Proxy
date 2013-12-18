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

#include "ClientInterface/Ssm/BpratelimitClientConfigMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"

namespace DcmNs
{

    BpratelimitClientConfigMessage::BpratelimitClientConfigMessage ()
        : DcmManagementInterfaceMessage(BPRATELIMITCLIENTCONFIG)
    {

    }

    BpratelimitClientConfigMessage::BpratelimitClientConfigMessage (const UI32 &slotId,const UI32 &opCode)
        : DcmManagementInterfaceMessage (BPRATELIMITCLIENTCONFIG),
        m_slotId    (slotId),
        m_opCode    (opCode)
    {
    }

    BpratelimitClientConfigMessage::~BpratelimitClientConfigMessage ()
    {
    }

    void  BpratelimitClientConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_slotId,"slotId"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }


    void  BpratelimitClientConfigMessage::setSlotId(const UI32 &slotId)
    {
        m_slotId  =  slotId;
    }

    UI32  BpratelimitClientConfigMessage::getSlotId() const
    {
        return (m_slotId);
    }
    void  BpratelimitClientConfigMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  BpratelimitClientConfigMessage::getMode() const
    {
        return (m_mode);
    }

    void  BpratelimitClientConfigMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  BpratelimitClientConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

    const void *BpratelimitClientConfigMessage::getCStructureForInputs ()
    {
    	ssmBpratelimitConfigMsg_t *pInput = (ssmBpratelimitConfigMsg_t *)malloc (sizeof (ssmBpratelimitConfigMsg_t));
        pInput->slotId = m_slotId;
        pInput->opCode  = m_opCode;

        return (pInput);
    }

    void BpratelimitClientConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
