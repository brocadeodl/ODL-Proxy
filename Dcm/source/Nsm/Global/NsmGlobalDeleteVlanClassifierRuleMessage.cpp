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

#include "Nsm/Global/NsmGlobalDeleteVlanClassifierRuleMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalDeleteVlanClassifierRuleMessage::NsmGlobalDeleteVlanClassifierRuleMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALDELETEVLANCLASSIFIERRULE)
    {
		m_ruleId = 0;
    }

    NsmGlobalDeleteVlanClassifierRuleMessage::NsmGlobalDeleteVlanClassifierRuleMessage (const UI32 &ruleId)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALDELETEVLANCLASSIFIERRULE),
        m_ruleId    (ruleId)
    {
    }

    NsmGlobalDeleteVlanClassifierRuleMessage::~NsmGlobalDeleteVlanClassifierRuleMessage ()
    {
    }

    void  NsmGlobalDeleteVlanClassifierRuleMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_ruleId,"ruleId"));
    }

    void  NsmGlobalDeleteVlanClassifierRuleMessage::setRuleId(const UI32 &ruleId)
    {
        m_ruleId  =  ruleId;
    }

    UI32  NsmGlobalDeleteVlanClassifierRuleMessage::getRuleId() const
    {
        return (m_ruleId);
    }

}
