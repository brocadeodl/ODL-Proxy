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
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/L3sys/L3ssDebugLpmMessage.h"
#include "ClientInterface/L3sys/L3ssMessageDef.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"

namespace DcmNs
{

    L3ssDebugLpmMessage::L3ssDebugLpmMessage ()
        : DcmManagementInterfaceMessage (L3SS_DEBUG_LPM)
    {
    }

    L3ssDebugLpmMessage::~L3ssDebugLpmMessage ()
    {
    }

    void  L3ssDebugLpmMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_slot, "slot"));
		addSerializableAttribute (new AttributeUI32(&m_chip, "chip"));
		addSerializableAttribute (new AttributeUI32(&m_last_printed, "last_printed"));
    }


    void  L3ssDebugLpmMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  L3ssDebugLpmMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    void  L3ssDebugLpmMessage::setSlot(const UI32 &slotno)
    {
        m_slot  =  slotno;
    }
    UI32  L3ssDebugLpmMessage::getSlot() const
    {
        return (m_slot);
    }
    void  L3ssDebugLpmMessage::setChip(const UI32 &chipno)
    {
        m_chip  =  chipno;
    }

    UI32  L3ssDebugLpmMessage::getChip() const
    {
        return (m_chip);
    }

    void  L3ssDebugLpmMessage::setLastPrinted(const UI32 &last_printed)
    {
        m_last_printed  =  last_printed;
    }
    UI32  L3ssDebugLpmMessage::getLastPrinted() const
    {
        return (m_last_printed);
    }
    const void *L3ssDebugLpmMessage::getCStructureForInputs ()
    {
        l3ss_debug_lpm_in_msg_t  *msg = new l3ss_debug_lpm_in_msg_t;
		string str_ipaddr;
		msg->cmd = m_cmdCode;
        msg->slot = m_slot;
        msg->chip = m_chip;
        msg->last_printed = m_last_printed;
        return msg;
    }
    void L3ssDebugLpmMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    void L3ssDebugLpmMessage::copyAllBuffers (const PrismMessage
            &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }
	void 
    L3ssDebugLpmMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		l3ss_debug_lpm_out_msg_t *msg = ((l3ss_debug_lpm_out_msg_t *)pOutputCStructure);

		if (msg == 0) {
			return;
		}

		int num_records = msg->num_records;
		addBuffer (L3SS_DEBUG_LPM, (((num_records - 1) > 0 )? (num_records - 1) : 0) * sizeof (debug_lpm_rec_t) +  sizeof (l3ss_debug_lpm_out_msg_t), pOutputCStructure, false);

		return;
	}
}
