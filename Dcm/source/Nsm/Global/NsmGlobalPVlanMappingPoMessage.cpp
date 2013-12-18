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

#include "Nsm/Global/NsmGlobalPVlanMappingPoMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalPVlanMappingPoMessage::NsmGlobalPVlanMappingPoMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName(),NSMGLOBALPVLANMAPPINGTOPO)
    {
    	m_id = 0;
    	m_primaryVlanId = 0;
    	//m_secVlanRange = 0;
		m_opcode = 0;
		m_cmdCode = 0;
		m_ifType = IF_TYPE_INVALID;
    }

    NsmGlobalPVlanMappingPoMessage::NsmGlobalPVlanMappingPoMessage (const UI32 &id, const string &ifName,const UI32 &primaryVlanId,const UI32Range &secVlanRange, const  UI32 &oper, const UI32 &opcode)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName(),NSMGLOBALPVLANMAPPINGTOPO),
        m_id    (id),
        m_ifName    (ifName),
        m_primaryVlanId    (primaryVlanId),
        m_secVlanRange (secVlanRange),
        m_oper (oper),
	    m_opcode    (opcode)
    {
    		m_cmdCode = 0;
    		m_ifType = IF_TYPE_INVALID;
        }

    NsmGlobalPVlanMappingPoMessage::~NsmGlobalPVlanMappingPoMessage ()
    {
    }

    void  NsmGlobalPVlanMappingPoMessage::setupAttributesForSerialization()
    {
    	ManagementInterfaceMessage::setupAttributesForSerialization ();
    	addSerializableAttribute (new AttributeUI32(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_primaryVlanId,"primaryVlanId"));
        addSerializableAttribute (new AttributeUI32Range(&m_secVlanRange,"secVlanRange"));
        addSerializableAttribute (new AttributeUI32(&m_oper,"oper"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
    }

    void  NsmGlobalPVlanMappingPoMessage::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  NsmGlobalPVlanMappingPoMessage::getId() const
    {
        return (m_id);
    }

    void  NsmGlobalPVlanMappingPoMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmGlobalPVlanMappingPoMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmGlobalPVlanMappingPoMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmGlobalPVlanMappingPoMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmGlobalPVlanMappingPoMessage::setPrimaryVlanId(const UI32 &primaryVlanId)
    {
        m_primaryVlanId  =  primaryVlanId;
    }

    UI32  NsmGlobalPVlanMappingPoMessage::getPrimaryVlanId() const
    {
        return (m_primaryVlanId);
    }

    void  NsmGlobalPVlanMappingPoMessage ::setSecVlanRange(const UI32Range &secVlanRange)
    {
    	m_secVlanRange = secVlanRange;
    }

    UI32Range  NsmGlobalPVlanMappingPoMessage ::getSecVlanRange() const
    {
        return (m_secVlanRange);
    }

    void  NsmGlobalPVlanMappingPoMessage ::setoper(const UI32  &oper)
    {
        m_oper  =  oper;
    }

    UI32 NsmGlobalPVlanMappingPoMessage ::getoper() const
    {
        return (m_oper);
    }

    void  NsmGlobalPVlanMappingPoMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmGlobalPVlanMappingPoMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void  NsmGlobalPVlanMappingPoMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  NsmGlobalPVlanMappingPoMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}
