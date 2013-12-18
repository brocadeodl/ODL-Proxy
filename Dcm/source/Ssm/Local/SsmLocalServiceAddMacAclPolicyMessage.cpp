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
 *   Author : pasu                                                     *
 **************************************************************************/

#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/SsmLocalServiceAddMacAclPolicyMessage.h"
#include "Ssm/Local/SsmTypes.h"

namespace DcmNs
{

    SsmLocalServiceAddMacAclPolicyMessage::SsmLocalServiceAddMacAclPolicyMessage ()
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (),SSMLOCALSERVICEADDMACACLPOLICY)
    {
    }

    SsmLocalServiceAddMacAclPolicyMessage::SsmLocalServiceAddMacAclPolicyMessage (const string &policyName,const UI32 &policyType)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (),SSMLOCALSERVICEADDMACACLPOLICY),
        m_policyName    (policyName),
        m_policyType    (policyType)
    {
    }

    SsmLocalServiceAddMacAclPolicyMessage::~SsmLocalServiceAddMacAclPolicyMessage ()
    {
    }

    void  SsmLocalServiceAddMacAclPolicyMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeUI32(&m_policyType,"policyType"));
    }

    void  SsmLocalServiceAddMacAclPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmLocalServiceAddMacAclPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmLocalServiceAddMacAclPolicyMessage::setPolicyType(const UI32 &policyType)
    {
        m_policyType  =  policyType;
    }

    UI32  SsmLocalServiceAddMacAclPolicyMessage::getPolicyType() const
    {
        return (m_policyType);
    }

}
