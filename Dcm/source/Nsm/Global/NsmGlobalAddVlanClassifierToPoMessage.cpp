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

#include "Nsm/Global/NsmGlobalAddVlanClassifierToPoMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalAddVlanClassifierToPoMessage::NsmGlobalAddVlanClassifierToPoMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALADDVLANCLASSIFIERTOPO)
    {
		m_groupId = 0;
		m_id = 0;
		m_ifIndex = 0;
		m_opcode = 0;
		m_vlanId = 1;
    }

    NsmGlobalAddVlanClassifierToPoMessage::NsmGlobalAddVlanClassifierToPoMessage (const UI32 &id,const string &ifName,const UI64 &ifIndex,const UI32 &groupId,const UI32 &vlanId,const UI32 &opcode)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALADDVLANCLASSIFIERTOPO),
        m_id    (id),
        m_ifName    (ifName),
        m_ifIndex    (ifIndex),
        m_groupId    (groupId),
        m_vlanId    (vlanId),
	m_opcode    (opcode)
    {
    }

    NsmGlobalAddVlanClassifierToPoMessage::~NsmGlobalAddVlanClassifierToPoMessage ()
    {
    }

    void  NsmGlobalAddVlanClassifierToPoMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeUI32(&m_groupId,"groupId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
    }

    void  NsmGlobalAddVlanClassifierToPoMessage::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  NsmGlobalAddVlanClassifierToPoMessage::getId() const
    {
        return (m_id);
    }

    void  NsmGlobalAddVlanClassifierToPoMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmGlobalAddVlanClassifierToPoMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmGlobalAddVlanClassifierToPoMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  NsmGlobalAddVlanClassifierToPoMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  NsmGlobalAddVlanClassifierToPoMessage::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }

    UI32  NsmGlobalAddVlanClassifierToPoMessage::getGroupId() const
    {
        return (m_groupId);
    }

    void  NsmGlobalAddVlanClassifierToPoMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmGlobalAddVlanClassifierToPoMessage::getVlanId() const
    {
        return (m_vlanId);
    }
    void  NsmGlobalAddVlanClassifierToPoMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmGlobalAddVlanClassifierToPoMessage::getOpcode() const
    {
        return (m_opcode);
    }

}
