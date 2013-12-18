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
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmLocalClearMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    NsmLocalClearMessage::NsmLocalClearMessage ()
        : PrismMessage(NsmLocalObjectManager::getPrismServiceId (), NSM_LOCAL_CLEAR_MSG)
	{
		m_ifName = "";
		m_cmdCode = NSM_CLEAR_COUNTERS_ALL;
		m_isGwRequest = true;
		m_slot = 0;
		m_po_id = 1;
		m_vlan = 1;
		m_interface = "tengigabitethernet";
		
	}

    NsmLocalClearMessage::NsmLocalClearMessage (const string &ifName, const UI32 &slot, const UI32 &poId, const UI32 &vlan, const UI32 &cmdCode, const bool isGwRequest)
        : PrismMessage(NsmLocalObjectManager::getPrismServiceId (), NSM_LOCAL_CLEAR_MSG),
        m_ifName    (ifName),
        m_slot    (slot),
        m_vlan    (vlan),
        m_po_id    (poId),
        m_cmdCode   (cmdCode),
		m_isGwRequest  (isGwRequest),
		m_interface ("tengigabitethernet")
    {
    }

    NsmLocalClearMessage::~NsmLocalClearMessage ()
    {
    }

    void  NsmLocalClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_slot,"slot"));
        addSerializableAttribute (new AttributeUI32(&m_vlan,"vlan"));
        addSerializableAttribute (new AttributeUI32(&m_po_id,"poid"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
        addSerializableAttribute (new AttributeString(&m_interface,"interface"));
    }

    void  NsmLocalClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmLocalClearMessage::getIfName() const
    {
        return (m_ifName);
    }
    void  NsmLocalClearMessage::setInterface(const string &interface)
    {
        m_interface  =  interface;
    }

    string  NsmLocalClearMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  NsmLocalClearMessage::setVlanId(const UI32 &vlan)
    {
        m_vlan  =  vlan;
    }

    UI32  NsmLocalClearMessage::getVlanId() const
    {
        return (m_vlan);
    }

    void  NsmLocalClearMessage::setPortChannelId(const UI32 &po_id)
    {
        m_po_id  =  po_id;
    }

    UI32  NsmLocalClearMessage::getPortChannelId() const
    {
        return (m_po_id);
    }

    void  NsmLocalClearMessage::setSlotId(const UI32 &slot)
    {
        m_slot  =  slot;
    }

    UI32  NsmLocalClearMessage::getSlotId() const
    {
        return (m_slot);
    }

    void  NsmLocalClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmLocalClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
	void NsmLocalClearMessage::setIsGwRequest (bool isGwRequest)
	{
		m_isGwRequest = isGwRequest;
	}

	bool NsmLocalClearMessage::getIsGwRequest () const
	{
		return (m_isGwRequest);
	}


}
