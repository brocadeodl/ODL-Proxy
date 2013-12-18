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
 *   Author : rmuthigi                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmAddVlanClassifierToPhyMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    NsmAddVlanClassifierToPhyMessage::NsmAddVlanClassifierToPhyMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMADDVLANCLASSIFIERTOPHY)
    {
		m_groupId = 0;
		m_ifIndex = 0;
		m_opcode = 0;
		m_vlanId = 1;
		m_ifType = IF_TYPE_INVALID;
    }

    NsmAddVlanClassifierToPhyMessage::NsmAddVlanClassifierToPhyMessage (const string &id,const string &ifName,const UI64 &ifIndex,const UI32 &groupId,const UI32 &vlanId,const UI32 &opcode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMADDVLANCLASSIFIERTOPHY),
        m_id    (id),
        m_ifName    (ifName),
        m_ifIndex    (ifIndex),
        m_groupId    (groupId),
        m_vlanId    (vlanId),
	    m_opcode    (opcode)	
    {
		m_ifType = IF_TYPE_INVALID;
    }

    NsmAddVlanClassifierToPhyMessage::~NsmAddVlanClassifierToPhyMessage ()
    {
    }

    void  NsmAddVlanClassifierToPhyMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeUI32(&m_groupId,"groupId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    }

    void  NsmAddVlanClassifierToPhyMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmAddVlanClassifierToPhyMessage::getIfType() const
    {
        return (m_ifType);
    }
    void  NsmAddVlanClassifierToPhyMessage::setId(const string &id)
    {
        m_id  =  id;
    }

    string  NsmAddVlanClassifierToPhyMessage::getId() const
    {
        return (m_id);
    }

    void  NsmAddVlanClassifierToPhyMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmAddVlanClassifierToPhyMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmAddVlanClassifierToPhyMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  NsmAddVlanClassifierToPhyMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  NsmAddVlanClassifierToPhyMessage::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }

    UI32  NsmAddVlanClassifierToPhyMessage::getGroupId() const
    {
        return (m_groupId);
    }

    void  NsmAddVlanClassifierToPhyMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmAddVlanClassifierToPhyMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmAddVlanClassifierToPhyMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmAddVlanClassifierToPhyMessage::getOpcode() const
    {
        return (m_opcode);
    }

}
