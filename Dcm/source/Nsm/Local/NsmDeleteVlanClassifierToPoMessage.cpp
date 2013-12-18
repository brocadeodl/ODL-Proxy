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
 *   .Author : rmuthigi                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmDeleteVlanClassifierToPoMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    NsmDeleteVlanClassifierToPhyMessage::NsmDeleteVlanClassifierToPhyMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), NSMDELETEVLANCLASSIFIERTOPO)
    {
		m_groupId = 0;
		m_ifIndex = 0;
		m_opcode = 0;
		m_ifType = IF_TYPE_INVALID;
    }

    NsmDeleteVlanClassifierToPhyMessage::NsmDeleteVlanClassifierToPhyMessage (const string &id,const string &ifName,const UI64 &ifIndex,const UI32 &groupId,const  UI32 &opcode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), NSMDELETEVLANCLASSIFIERTOPO),
        m_id    (id),
        m_ifName    (ifName),
        m_ifIndex    (ifIndex),
        m_groupId    (groupId),
		m_opcode     (opcode)
    {
		m_ifType = IF_TYPE_INVALID;
    }

    NsmDeleteVlanClassifierToPhyMessage::~NsmDeleteVlanClassifierToPhyMessage ()
    {
    }

    void  NsmDeleteVlanClassifierToPhyMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeUI32(&m_groupId,"groupId"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    }

    void  NsmDeleteVlanClassifierToPhyMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmDeleteVlanClassifierToPhyMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmDeleteVlanClassifierToPhyMessage::setId(const string &id)
    {
        m_id  =  id;
    }

    string  NsmDeleteVlanClassifierToPhyMessage::getId() const
    {
        return (m_id);
    }

    void  NsmDeleteVlanClassifierToPhyMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmDeleteVlanClassifierToPhyMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmDeleteVlanClassifierToPhyMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  NsmDeleteVlanClassifierToPhyMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  NsmDeleteVlanClassifierToPhyMessage::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }

    UI32  NsmDeleteVlanClassifierToPhyMessage::getGroupId() const
    {
        return (m_groupId);
    }

    void  NsmDeleteVlanClassifierToPhyMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmDeleteVlanClassifierToPhyMessage::getOpcode() const
    {
        return (m_opcode);
    }


}
