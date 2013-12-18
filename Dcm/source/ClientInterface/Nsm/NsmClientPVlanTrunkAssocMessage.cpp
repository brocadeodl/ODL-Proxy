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
 *   Author : rprashar                                                     *
 **************************************************************************/

#include "ClientInterface/Nsm/NsmClientPVlanTrunkAssocMessage.h"
#include "ClientInterface/Nsm/PVlanCommon.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    NsmClientPVlanTrunkAssocMessage::NsmClientPVlanTrunkAssocMessage ()
        : DcmManagementInterfaceMessage (NSM_CLIENT_PVLAN_TRUNK_ASSOC_MSG)
    {
    	m_primaryVlanId = 0;
    	m_secondaryVlanId = 0;
		m_cmdCode = 0;
		m_ifType = IF_TYPE_INVALID;
    }

    NsmClientPVlanTrunkAssocMessage::NsmClientPVlanTrunkAssocMessage (const string &ifName,const UI32 &primaryVlanId,const UI32 &secondaryVlanId)
        : DcmManagementInterfaceMessage (NSM_CLIENT_PVLAN_TRUNK_ASSOC_MSG),
        m_ifName    (ifName),
        m_primaryVlanId    (primaryVlanId),
        m_secondaryVlanId    (secondaryVlanId)
    {
    		m_cmdCode = 0;
    		m_ifType = IF_TYPE_INVALID;
        }

    NsmClientPVlanTrunkAssocMessage::~NsmClientPVlanTrunkAssocMessage ()
    {
    }

    void  NsmClientPVlanTrunkAssocMessage::setupAttributesForSerialization()
    {
    	DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_primaryVlanId,"primaryVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_secondaryVlanId,"secondaryVlanIdd"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
    }

    void  NsmClientPVlanTrunkAssocMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmClientPVlanTrunkAssocMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmClientPVlanTrunkAssocMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmClientPVlanTrunkAssocMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmClientPVlanTrunkAssocMessage::setPrimaryVlanId(const UI32 &primaryVlanId)
    {
        m_primaryVlanId  =  primaryVlanId;
    }

    UI32  NsmClientPVlanTrunkAssocMessage::getPrimaryVlanId() const
    {
        return (m_primaryVlanId);
    }

    void  NsmClientPVlanTrunkAssocMessage::setSecondaryVlanId(const UI32 &secondaryVlanId)
    {
        m_secondaryVlanId  =  secondaryVlanId;
    }

    UI32  NsmClientPVlanTrunkAssocMessage::getSecondaryVlanId() const
    {
        return (m_secondaryVlanId);
    }


    void  NsmClientPVlanTrunkAssocMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmClientPVlanTrunkAssocMessage::getOpcode() const
    {
        return (m_opcode);
    }


    void  NsmClientPVlanTrunkAssocMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  NsmClientPVlanTrunkAssocMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    const void *NsmClientPVlanTrunkAssocMessage::getCStructureForInputs ()
    {
    	PVlanTrunkAssocMsg_t  *pInput = (PVlanTrunkAssocMsg_t*)malloc (sizeof (PVlanTrunkAssocMsg_t));

        strncpy (pInput->PortName, m_ifName.c_str(), 32);
        pInput->PortName[31] = '\0';
        pInput->ifType = m_ifType;
        pInput->primaryVlanId = m_primaryVlanId;
        pInput->secondaryVlanId = m_secondaryVlanId;
        pInput->opcode = m_opcode;
        return (pInput);
    }

    void NsmClientPVlanTrunkAssocMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
