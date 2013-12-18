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

#include "ClientInterface/Nsm/NsmClientAddVlanClassifierToPoSPMessage.h"
#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include "ClientInterface/DceWaveClientMessage.h"


namespace DcmNs
{

    NsmClientAddVlanClassifierToPoSPMessage::NsmClientAddVlanClassifierToPoSPMessage ()
        : NsmVlanProvisionMessage (NSMCLIENTADDVLANCLASSIFIERTOPOSP)
    {
    }

    NsmClientAddVlanClassifierToPoSPMessage::NsmClientAddVlanClassifierToPoSPMessage (const UI32 &id,const string &ifName,const UI64 &ifIndex,const UI32 &groupId,const UI32 &vlanId)
        : NsmVlanProvisionMessage (NSMCLIENTADDVLANCLASSIFIERTOPOSP),
        m_id    (id),
        m_ifName    (ifName),
        m_ifIndex    (ifIndex),
        m_groupId    (groupId),
        m_vlanId    (vlanId)
    {
    }

    NsmClientAddVlanClassifierToPoSPMessage::~NsmClientAddVlanClassifierToPoSPMessage ()
    {
    }

    void  NsmClientAddVlanClassifierToPoSPMessage::setupAttributesForSerialization()
    {
        NsmVlanProvisionMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeUI32(&m_groupId,"groupId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
    }

    void  NsmClientAddVlanClassifierToPoSPMessage::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  NsmClientAddVlanClassifierToPoSPMessage::getId() const
    {
        return (m_id);
    }

    void  NsmClientAddVlanClassifierToPoSPMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmClientAddVlanClassifierToPoSPMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmClientAddVlanClassifierToPoSPMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  NsmClientAddVlanClassifierToPoSPMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  NsmClientAddVlanClassifierToPoSPMessage::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }

    UI32  NsmClientAddVlanClassifierToPoSPMessage::getGroupId() const
    {
        return (m_groupId);
    }

    void  NsmClientAddVlanClassifierToPoSPMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmClientAddVlanClassifierToPoSPMessage::getVlanId() const
    {
        return (m_vlanId);
    }
    void  NsmClientAddVlanClassifierToPoSPMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode=  opcode;
    }

    UI32  NsmClientAddVlanClassifierToPoSPMessage::getOpcode() const
    {
        return (m_opcode);
    }
    const void *NsmClientAddVlanClassifierToPoSPMessage::getCStructureForInputs ()
    {
        VlanClassifierActivateGroupMsg_t  *pInput = (VlanClassifierActivateGroupMsg_t*)malloc (sizeof (VlanClassifierActivateGroupMsg_t));

        strncpy (pInput->PortName, m_ifName.c_str(), 32);
	pInput->PortName[31] = '\0';
        pInput->ifIndex = m_ifIndex;
        pInput->groupid = m_groupId;
        pInput->vlanid = m_vlanId;
	pInput->opcode = m_opcode;
        return (pInput);
    }

}