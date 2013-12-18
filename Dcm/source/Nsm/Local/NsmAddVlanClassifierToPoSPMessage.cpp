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
#include "Nsm/Local/NsmAddVlanClassifierToPoSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmAddVlanClassifierToPoSPMessage::NsmAddVlanClassifierToPoSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMADDVLANCLASSIFIERTOPOSP)
    {
		m_groupId = 0;
		m_id = 0;
		m_ifIndex = 0;
		m_opcode = 0;
		m_vlanId = 1;
    }

    NsmAddVlanClassifierToPoSPMessage::NsmAddVlanClassifierToPoSPMessage (const UI32 &id,const string &ifName,const UI64 &ifIndex,const UI32 &groupId,const UI32 &vlanId,const UI32 &opcode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMADDVLANCLASSIFIERTOPOSP),
        m_id    (id),
        m_ifName    (ifName),
        m_ifIndex    (ifIndex),
        m_groupId    (groupId),
        m_vlanId    (vlanId),
	m_opcode    (opcode) 
    {
    }

    NsmAddVlanClassifierToPoSPMessage::~NsmAddVlanClassifierToPoSPMessage ()
    {
    }

    void  NsmAddVlanClassifierToPoSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeUI32(&m_groupId,"groupId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
    }

    void  NsmAddVlanClassifierToPoSPMessage::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  NsmAddVlanClassifierToPoSPMessage::getId() const
    {
        return (m_id);
    }

    void  NsmAddVlanClassifierToPoSPMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmAddVlanClassifierToPoSPMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmAddVlanClassifierToPoSPMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  NsmAddVlanClassifierToPoSPMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  NsmAddVlanClassifierToPoSPMessage::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }

    UI32  NsmAddVlanClassifierToPoSPMessage::getGroupId() const
    {
        return (m_groupId);
    }

    void  NsmAddVlanClassifierToPoSPMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmAddVlanClassifierToPoSPMessage::getVlanId() const
    {
        return (m_vlanId);
    }
    void  NsmAddVlanClassifierToPoSPMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  = opcode;
    }

    UI32  NsmAddVlanClassifierToPoSPMessage::getOpcode() const
    {
        return (m_opcode);
    }

}
