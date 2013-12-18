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

#include "Nsm/Global/NsmGlobalPVlanTrunkAssocPoMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalPVlanTrunkAssocPoMessage::NsmGlobalPVlanTrunkAssocPoMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName(),NSMGLOBALPVLANTRUNKASSOCTOPO)
    {
    	m_id = 0;
    	m_primaryVlanId = 0;
    	m_secondaryVlanId = 0;
		m_opcode = 0;
		m_cmdCode = 0;
    }

    NsmGlobalPVlanTrunkAssocPoMessage::NsmGlobalPVlanTrunkAssocPoMessage (const UI32 &id, const string &ifName,const UI32 &primaryVlanId,const UI32 &secondaryVlanId,const UI32 &opcode)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName(),NSMGLOBALPVLANTRUNKASSOCTOPO),
        m_id    (id),
        m_ifName    (ifName),
        m_primaryVlanId    (primaryVlanId),
        m_secondaryVlanId    (secondaryVlanId),
	    m_opcode    (opcode)
    {
    		m_cmdCode = 0;
        }

    NsmGlobalPVlanTrunkAssocPoMessage::~NsmGlobalPVlanTrunkAssocPoMessage ()
    {
    }

    void  NsmGlobalPVlanTrunkAssocPoMessage::setupAttributesForSerialization()
    {
    	ManagementInterfaceMessage::setupAttributesForSerialization ();
    	addSerializableAttribute (new AttributeUI32(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_primaryVlanId,"primaryVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_secondaryVlanId,"secondaryVlanIdd"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
    }

    void  NsmGlobalPVlanTrunkAssocPoMessage::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  NsmGlobalPVlanTrunkAssocPoMessage::getId() const
    {
        return (m_id);
    }

    void  NsmGlobalPVlanTrunkAssocPoMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmGlobalPVlanTrunkAssocPoMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmGlobalPVlanTrunkAssocPoMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmGlobalPVlanTrunkAssocPoMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmGlobalPVlanTrunkAssocPoMessage::setPrimaryVlanId(const UI32 &primaryVlanId)
    {
        m_primaryVlanId  =  primaryVlanId;
    }

    UI32  NsmGlobalPVlanTrunkAssocPoMessage::getPrimaryVlanId() const
    {
        return (m_primaryVlanId);
    }

    void  NsmGlobalPVlanTrunkAssocPoMessage::setSecondaryVlanId(const UI32 &secondaryVlanId)
    {
        m_secondaryVlanId  =  secondaryVlanId;
    }

    UI32  NsmGlobalPVlanTrunkAssocPoMessage::getSecondaryVlanId() const
    {
        return (m_secondaryVlanId);
    }

    void  NsmGlobalPVlanTrunkAssocPoMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmGlobalPVlanTrunkAssocPoMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void  NsmGlobalPVlanTrunkAssocPoMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  NsmGlobalPVlanTrunkAssocPoMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}
