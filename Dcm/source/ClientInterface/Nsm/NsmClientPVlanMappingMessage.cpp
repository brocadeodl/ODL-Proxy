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

#include "ClientInterface/Nsm/NsmClientPVlanMappingMessage.h"
#include "ClientInterface/Nsm/PVlanCommon.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/InterfaceCommon.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmClientPVlanMappingMessage::NsmClientPVlanMappingMessage ()
        : DcmManagementInterfaceMessage (NSM_CLIENT_PVLAN_MAPPING_MSG)
    {
    	m_primaryVlanId = 0;
		m_cmdCode = 0;
		m_ifType = IF_TYPE_INVALID;
    }

    NsmClientPVlanMappingMessage::NsmClientPVlanMappingMessage (const string &ifName,const UI32 &primaryVlanId,const UI32Range &secVlanRange, const  UI32 &oper)
        : DcmManagementInterfaceMessage (NSM_CLIENT_PVLAN_MAPPING_MSG),
        m_ifName    (ifName),
        m_primaryVlanId    (primaryVlanId),
        m_secVlanRange (secVlanRange),
        m_oper (oper)
    {
    		m_cmdCode = 0;
    		m_ifType = IF_TYPE_INVALID;
        }

    NsmClientPVlanMappingMessage::~NsmClientPVlanMappingMessage ()
    {
    }

    void  NsmClientPVlanMappingMessage::setupAttributesForSerialization()
    {
    	DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_primaryVlanId,"primaryVlanId"));
        addSerializableAttribute (new AttributeUI32Range(&m_secVlanRange,"secVlanRange"));
        addSerializableAttribute (new AttributeUI32(&m_oper,"oper"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
    }

    void  NsmClientPVlanMappingMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmClientPVlanMappingMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmClientPVlanMappingMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmClientPVlanMappingMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmClientPVlanMappingMessage::setPrimaryVlanId(const UI32 &primaryVlanId)
    {
        m_primaryVlanId  =  primaryVlanId;
    }

    UI32  NsmClientPVlanMappingMessage::getPrimaryVlanId() const
    {
        return (m_primaryVlanId);
    }

    void  NsmClientPVlanMappingMessage ::setSecVlanRange(const UI32Range &secVlanRange)
    {
    	m_secVlanRange = secVlanRange;
    }

    UI32Range  NsmClientPVlanMappingMessage ::getSecVlanRange() const
    {
        return (m_secVlanRange);
    }

    void  NsmClientPVlanMappingMessage ::setoper(const UI32  &oper)
    {
        m_oper  =  oper;
    }

    UI32 NsmClientPVlanMappingMessage ::getoper() const
    {
        return (m_oper);
    }


    void  NsmClientPVlanMappingMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmClientPVlanMappingMessage::getOpcode() const
    {
        return (m_opcode);
    }


    void  NsmClientPVlanMappingMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  NsmClientPVlanMappingMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    const void *NsmClientPVlanMappingMessage::getCStructureForInputs ()
    {
	PVlanMappingMsg_t *pInput = (PVlanMappingMsg_t*) malloc(
			sizeof(PVlanMappingMsg_t));

	strncpy(pInput->PortName, m_ifName.c_str(), 32);
	pInput->PortName[31] = '\0';
	pInput->primaryVlanId = m_primaryVlanId;
	pInput->ifType = m_ifType;
	pInput->oper = m_oper;

	char *vlanList = NULL;
	int vlSize = 0;

	vlanList = (char *) (m_secVlanRange.toString()).c_str();
	vlSize = (m_secVlanRange.toString()).size();

	if (vlanList) {
		pInput->secVlanRange = (char *) malloc(vlSize + 1);
		strncpy(pInput->secVlanRange, vlanList, vlSize);
		pInput->secVlanRange[vlSize] = '\0';
	}
	pInput->opcode = m_opcode;
	return (pInput);
}

    void NsmClientPVlanMappingMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
