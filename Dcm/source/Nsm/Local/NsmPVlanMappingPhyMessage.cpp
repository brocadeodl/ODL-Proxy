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

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmPVlanMappingPhyMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    NsmPVlanMappingPhyMessage::NsmPVlanMappingPhyMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMPVLANMAPPINGTOPHY)
    {
    	m_primaryVlanId = 0;
    	//m_secVlanRange = "";  + PVLAN todo oper
		m_opcode = 0;
		m_cmdCode = 0;
		m_ifType = IF_TYPE_INVALID;
    }

    NsmPVlanMappingPhyMessage::NsmPVlanMappingPhyMessage (const string &ifName,const UI32 &primaryVlanId,const UI32Range &secVlanRange, const  UI32 &oper, const UI32 &opcode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMPVLANMAPPINGTOPHY),
        m_ifName    (ifName),
        m_primaryVlanId    (primaryVlanId),
        m_secVlanRange (secVlanRange),
        m_oper (oper),
	    m_opcode    (opcode)
    {
    		m_cmdCode = 0;
    		m_ifType = IF_TYPE_INVALID;
        }

    NsmPVlanMappingPhyMessage::~NsmPVlanMappingPhyMessage ()
    {
    }

    void  NsmPVlanMappingPhyMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_primaryVlanId,"primaryVlanId"));
        addSerializableAttribute (new AttributeUI32Range(&m_secVlanRange,"secVlanRange"));
        addSerializableAttribute (new AttributeUI32(&m_oper,"oper"));
        addSerializableAttribute (new AttributeUI32(& m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
    }

    void  NsmPVlanMappingPhyMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmPVlanMappingPhyMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmPVlanMappingPhyMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmPVlanMappingPhyMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmPVlanMappingPhyMessage::setPrimaryVlanId(const UI32 &primaryVlanId)
    {
        m_primaryVlanId  =  primaryVlanId;
    }

    UI32  NsmPVlanMappingPhyMessage::getPrimaryVlanId() const
    {
        return (m_primaryVlanId);
    }

    void  NsmPVlanMappingPhyMessage ::setSecVlanRange(const UI32Range &secVlanRange)
    {
    	m_secVlanRange = secVlanRange;
    }

    UI32Range  NsmPVlanMappingPhyMessage ::getSecVlanRange() const
    {
        return (m_secVlanRange);
    }

    void   NsmPVlanMappingPhyMessage ::setoper(const UI32  &oper)
    {
        m_oper  =  oper;
    }

    UI32 NsmPVlanMappingPhyMessage ::getoper() const
    {
        return (m_oper);
    }


    void  NsmPVlanMappingPhyMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmPVlanMappingPhyMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void  NsmPVlanMappingPhyMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  NsmPVlanMappingPhyMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}
