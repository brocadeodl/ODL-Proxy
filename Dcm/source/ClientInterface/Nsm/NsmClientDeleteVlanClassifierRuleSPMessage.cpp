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

#include "ClientInterface/Nsm/NsmClientDeleteVlanClassifierRuleSPMessage.h"
#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    NsmClientDeleteVlanClassifierRuleSPMessage::NsmClientDeleteVlanClassifierRuleSPMessage ()
        : NsmVlanProvisionMessage (NSMCLIENTDELETEVLANCLASSIFIERRULESP)
    {
    }

    NsmClientDeleteVlanClassifierRuleSPMessage::NsmClientDeleteVlanClassifierRuleSPMessage (const UI32 &ruleId)
        : NsmVlanProvisionMessage (NSMCLIENTDELETEVLANCLASSIFIERRULESP),
        m_ruleId    (ruleId)
    {
    }

    NsmClientDeleteVlanClassifierRuleSPMessage::~NsmClientDeleteVlanClassifierRuleSPMessage ()
    {
    }

    void  NsmClientDeleteVlanClassifierRuleSPMessage::setupAttributesForSerialization()
    {
        NsmVlanProvisionMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_ruleId,"ruleId"));
    }

    void  NsmClientDeleteVlanClassifierRuleSPMessage::setRuleId(const UI32 &ruleId)
    {
        m_ruleId  =  ruleId;
    }

    UI32  NsmClientDeleteVlanClassifierRuleSPMessage::getRuleId() const
    {
        return (m_ruleId);
    }
    const void *NsmClientDeleteVlanClassifierRuleSPMessage::getCStructureForInputs ()
    {
        unsigned int  *pInput = (unsigned int*)malloc(sizeof(unsigned int));
	*pInput = m_ruleId;
        return (pInput);
    }

}
