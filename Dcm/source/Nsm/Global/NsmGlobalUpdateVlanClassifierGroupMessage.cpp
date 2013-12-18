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

#include "Nsm/Global/NsmGlobalUpdateVlanClassifierGroupMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalUpdateVlanClassifierGroupMessage::NsmGlobalUpdateVlanClassifierGroupMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEVLANCLASSIFIERGROUP)
    {
		m_dummy = 0;
		m_groupId = 0;
		m_operation = 0;
		m_ruleId = 0;
    }

    NsmGlobalUpdateVlanClassifierGroupMessage::NsmGlobalUpdateVlanClassifierGroupMessage (const UI32 &groupId,const UI32 &operation,const UI32 &ruleId, const UI32 &dummy)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEVLANCLASSIFIERGROUP),
        m_groupId    (groupId),
        m_operation    (operation),
        m_ruleId    (ruleId),
	m_dummy     (dummy)
    {
    }

    NsmGlobalUpdateVlanClassifierGroupMessage::~NsmGlobalUpdateVlanClassifierGroupMessage ()
    {
    }

    void  NsmGlobalUpdateVlanClassifierGroupMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_groupId,"groupId"));
        addSerializableAttribute (new AttributeUI32(&m_operation,"operation"));
        addSerializableAttribute (new AttributeUI32(&m_ruleId,"ruleId"));
        addSerializableAttribute (new AttributeUI32(&m_dummy,"m_dummy"));
    }

    void  NsmGlobalUpdateVlanClassifierGroupMessage::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }

    UI32  NsmGlobalUpdateVlanClassifierGroupMessage::getGroupId() const
    {
        return (m_groupId);
    }

    void  NsmGlobalUpdateVlanClassifierGroupMessage::setOperation(const UI32 &operation)
    {
        m_operation  =  operation;
    }

    UI32  NsmGlobalUpdateVlanClassifierGroupMessage::getOperation() const
    {
        return (m_operation);
    }

    void  NsmGlobalUpdateVlanClassifierGroupMessage::setRuleId(const UI32 &ruleId)
    {
        m_ruleId  =  ruleId;
    }

    UI32  NsmGlobalUpdateVlanClassifierGroupMessage::getRuleId() const
    {
        return (m_ruleId);
    }
    void  NsmGlobalUpdateVlanClassifierGroupMessage::setdummy(const UI32 &dummy)
    {
        m_dummy  =  dummy;
    }

    UI32  NsmGlobalUpdateVlanClassifierGroupMessage::getdummy() const
    {
        return (m_dummy);
    }

}
