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
#include "ClientInterface/Nsm/NsmClientUpdateVlanClassifierGroupSPMessage.h"
#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include "ClientInterface/DceWaveClientMessage.h"


namespace DcmNs
{

    NsmClientUpdateVlanClassifierGroupSPMessage::NsmClientUpdateVlanClassifierGroupSPMessage ()
        : DcmManagementInterfaceMessage (NSMCLIENTUPDATEVLANCLASSIFIERGROUPSP)
    {
    }

    NsmClientUpdateVlanClassifierGroupSPMessage::NsmClientUpdateVlanClassifierGroupSPMessage (const UI32 &groupId,const UI32 &operation,const UI32 &ruleId)
        : DcmManagementInterfaceMessage (NSMCLIENTUPDATEVLANCLASSIFIERGROUPSP),
        m_groupId    (groupId),
        m_operation    (operation),
        m_ruleId    (ruleId)
    {
    }

    NsmClientUpdateVlanClassifierGroupSPMessage::~NsmClientUpdateVlanClassifierGroupSPMessage ()
    {
    }

    void  NsmClientUpdateVlanClassifierGroupSPMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_groupId,"groupId"));
        addSerializableAttribute (new AttributeUI32(&m_operation,"operation"));
        addSerializableAttribute (new AttributeUI32(&m_ruleId,"ruleId"));
    }

    void  NsmClientUpdateVlanClassifierGroupSPMessage::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }

    UI32  NsmClientUpdateVlanClassifierGroupSPMessage::getGroupId() const
    {
        return (m_groupId);
    }

    void  NsmClientUpdateVlanClassifierGroupSPMessage::setOperation(const UI32 &operation)
    {
        m_operation  =  operation;
    }

    UI32  NsmClientUpdateVlanClassifierGroupSPMessage::getOperation() const
    {
        return (m_operation);
    }

    void  NsmClientUpdateVlanClassifierGroupSPMessage::setRuleId(const UI32 &ruleId)
    {
        m_ruleId  =  ruleId;
    }

    UI32  NsmClientUpdateVlanClassifierGroupSPMessage::getRuleId() const
    {
        return (m_ruleId);
    }
    

    const void *NsmClientUpdateVlanClassifierGroupSPMessage::getCStructureForInputs ()
    {
        VlanClassifierCreateGroupMsg_t *pInput = (VlanClassifierCreateGroupMsg_t *)malloc (sizeof (VlanClassifierCreateGroupMsg_t));

        pInput->ruleId = m_ruleId;
        pInput->groupId = m_groupId;
        pInput->operation = m_operation;
        
        return (pInput);
    }

    void NsmClientUpdateVlanClassifierGroupSPMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }


}
