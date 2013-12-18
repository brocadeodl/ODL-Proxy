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
 *   Author : Navin
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/NsmClearMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    NsmClearMessage::NsmClearMessage ()
        : DcmManagementInterfaceMessage (NSM_CLEAR_MSG)
    {
    }

    NsmClearMessage::NsmClearMessage (const string &ifName, const UI32 &slot, const UI32 &poId, const UI32 &vlan, const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (NSM_CLEAR_MSG),
        m_ifName    (ifName),
        m_slot    (slot),
        m_vlan    (vlan),
        m_po_id    (poId),
        m_cmdCode   (cmdCode)
    {
    }

    NsmClearMessage::~NsmClearMessage ()
    {
    }

    void  NsmClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_slot,"slot"));
        addSerializableAttribute (new AttributeUI32(&m_vlan,"vlan"));
        addSerializableAttribute (new AttributeUI32(&m_po_id,"poid"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
    }

    void  NsmClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmClearMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  NsmClearMessage::setVlanId(const UI32 &vlan)
    {
        m_vlan  =  vlan;
    }

    UI32  NsmClearMessage::getVlanId() const
    {
        return (m_vlan);
    }

    void  NsmClearMessage::setPortChannelId(const UI32 &po_id)
    {
        m_po_id  =  po_id;
    }

    UI32  NsmClearMessage::getPortChannelId() const
    {
        return (m_po_id);
    }

    void  NsmClearMessage::setSlotId(const UI32 &slot)
    {
        m_slot  =  slot;
    }

    UI32  NsmClearMessage::getSlotId() const
    {
        return (m_slot);
    }

    void  NsmClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void NsmClearMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
							UI32 &size, void *&pBuffer)
    {
        pBuffer   =   findBuffer(bufferNum, size);
	printf("getDuplicateInterfaceOutputBuffer::Receive size :: %d \n", size);
	printf("getDuplicateInterfaceOutputBuffer::BufferNo:: %d\n", bufferNum);
    }
    
    const void *NsmClearMessage::getCStructureForInputs ()
    {
        nsm_clear_input_msg_t *pInput = new nsm_clear_input_msg_t;

        strncpy (pInput->if_name, m_ifName.c_str (), MAX_STR);
		pInput->if_name[MAX_STR-1] = '\0';
        pInput->vlan_id     = m_vlan;
        pInput->po_id       = m_po_id;
        pInput->slot_id     = m_slot;
        pInput->cmd_code    = m_cmdCode;

        return (pInput);
    }

    void NsmClearMessage::loadOutputsFromCStructure (const void 
						*pOutputCStructure)
    {
    }
}
