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

#include "Nsm/Global/NsmGlobalDeleteVlanClassifierToPoMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalDeleteVlanClassifierToPoMessage::NsmGlobalDeleteVlanClassifierToPoMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALDELETEVLANCLASSIFIERTOPO)
    {
		m_groupId = 0;
		m_id = 0;
		m_ifIndex = 0;
		m_opcode = 0;
    }

    NsmGlobalDeleteVlanClassifierToPoMessage::NsmGlobalDeleteVlanClassifierToPoMessage (const UI32 &id,const string &ifName,const UI64 &ifIndex,const UI32 &groupId,const UI32 &opcode)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALDELETEVLANCLASSIFIERTOPO),
        m_id    (id),
        m_ifName    (ifName),
        m_ifIndex    (ifIndex),
        m_groupId    (groupId),
	m_opcode     (opcode)	
    {
    }

    NsmGlobalDeleteVlanClassifierToPoMessage::~NsmGlobalDeleteVlanClassifierToPoMessage ()
    {
    }

    void  NsmGlobalDeleteVlanClassifierToPoMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeUI32(&m_groupId,"groupId"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
    }

    void  NsmGlobalDeleteVlanClassifierToPoMessage::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  NsmGlobalDeleteVlanClassifierToPoMessage::getId() const
    {
        return (m_id);
    }

    void  NsmGlobalDeleteVlanClassifierToPoMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmGlobalDeleteVlanClassifierToPoMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmGlobalDeleteVlanClassifierToPoMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  NsmGlobalDeleteVlanClassifierToPoMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  NsmGlobalDeleteVlanClassifierToPoMessage::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }

    UI32  NsmGlobalDeleteVlanClassifierToPoMessage::getGroupId() const
    {
        return (m_groupId);
    }
    void  NsmGlobalDeleteVlanClassifierToPoMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode=  opcode;
    }

    UI32  NsmGlobalDeleteVlanClassifierToPoMessage::getOpcode() const
    {
        return (m_opcode);
    }

}
