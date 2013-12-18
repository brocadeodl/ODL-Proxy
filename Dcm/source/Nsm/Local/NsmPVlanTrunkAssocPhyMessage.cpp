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
#include "Nsm/Local/NsmPVlanTrunkAssocPhyMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    NsmPVlanTrunkAssocPhyMessage::NsmPVlanTrunkAssocPhyMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMPVLANTRUNKASSOCTOPHY)
    {
    	m_primaryVlanId = 0;
    	m_secondaryVlanId = 0;
		m_opcode = 0;
		m_cmdCode = 0;
		m_ifType = IF_TYPE_INVALID;
    }

    NsmPVlanTrunkAssocPhyMessage::NsmPVlanTrunkAssocPhyMessage (const string &ifName,const UI32 &primaryVlanId,const UI32 &secondaryVlanId,const UI32 &opcode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMPVLANTRUNKASSOCTOPHY),
        m_ifName    (ifName),
        m_primaryVlanId    (primaryVlanId),
        m_secondaryVlanId    (secondaryVlanId),
	    m_opcode    (opcode)
    {
    		m_cmdCode = 0;
    		m_ifType = IF_TYPE_INVALID;
        }

    NsmPVlanTrunkAssocPhyMessage::~NsmPVlanTrunkAssocPhyMessage ()
    {
    }

    void  NsmPVlanTrunkAssocPhyMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_primaryVlanId,"primaryVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_secondaryVlanId,"secondaryVlanIdd"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
    }

    void  NsmPVlanTrunkAssocPhyMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmPVlanTrunkAssocPhyMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmPVlanTrunkAssocPhyMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmPVlanTrunkAssocPhyMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmPVlanTrunkAssocPhyMessage::setPrimaryVlanId(const UI32 &primaryVlanId)
    {
        m_primaryVlanId  =  primaryVlanId;
    }

    UI32  NsmPVlanTrunkAssocPhyMessage::getPrimaryVlanId() const
    {
        return (m_primaryVlanId);
    }

    void  NsmPVlanTrunkAssocPhyMessage::setSecondaryVlanId(const UI32 &secondaryVlanId)
    {
        m_secondaryVlanId  =  secondaryVlanId;
    }

    UI32  NsmPVlanTrunkAssocPhyMessage::getSecondaryVlanId() const
    {
        return (m_secondaryVlanId);
    }

    void  NsmPVlanTrunkAssocPhyMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmPVlanTrunkAssocPhyMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void  NsmPVlanTrunkAssocPhyMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  NsmPVlanTrunkAssocPhyMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}
