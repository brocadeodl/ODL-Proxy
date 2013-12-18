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
#include "Nsm/Local/NsmUpdateVlanClassifierGroupSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmUpdateVlanClassifierGroupSPMessage::NsmUpdateVlanClassifierGroupSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMUPDATEVLANCLASSIFIERGROUPSP)
    {
		m_groupId = 0;
		m_operation = 0;
		m_ruleId = 0;
    }

    NsmUpdateVlanClassifierGroupSPMessage::NsmUpdateVlanClassifierGroupSPMessage (const UI32 &groupId,const UI32 &operation,const UI32 &ruleId)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMUPDATEVLANCLASSIFIERGROUPSP),
        m_groupId    (groupId),
        m_operation    (operation),
        m_ruleId    (ruleId)
    {
    }

    NsmUpdateVlanClassifierGroupSPMessage::~NsmUpdateVlanClassifierGroupSPMessage ()
    {
    }

    void  NsmUpdateVlanClassifierGroupSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_groupId,"groupId"));
        addSerializableAttribute (new AttributeUI32(&m_operation,"operation"));
        addSerializableAttribute (new AttributeUI32(&m_ruleId,"ruleId"));
    }

    void  NsmUpdateVlanClassifierGroupSPMessage::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }

    UI32  NsmUpdateVlanClassifierGroupSPMessage::getGroupId() const
    {
        return (m_groupId);
    }

    void  NsmUpdateVlanClassifierGroupSPMessage::setOperation(const UI32 &operation)
    {
        m_operation  =  operation;
    }

    UI32  NsmUpdateVlanClassifierGroupSPMessage::getOperation() const
    {
        return (m_operation);
    }

    void  NsmUpdateVlanClassifierGroupSPMessage::setRuleId(const UI32 &ruleId)
    {
        m_ruleId  =  ruleId;
    }

    UI32  NsmUpdateVlanClassifierGroupSPMessage::getRuleId() const
    {
        return (m_ruleId);
    }

}
