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

#include "Ssm/Global/SsmGlobalGetMacAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"

namespace DcmNs
{

    SsmGlobalGetMacAclPolicyMessage::SsmGlobalGetMacAclPolicyMessage ()
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),SSMGLOBALGETMACACLPOLICY)
    {
    }

    SsmGlobalGetMacAclPolicyMessage::SsmGlobalGetMacAclPolicyMessage (const string &policyName)
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),SSMGLOBALGETMACACLPOLICY),
        m_policyName    (policyName)
    {
    }

    SsmGlobalGetMacAclPolicyMessage::~SsmGlobalGetMacAclPolicyMessage ()
    {
    }

    void  SsmGlobalGetMacAclPolicyMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeObjectId(&m_policyObjId,"policyObjId"));
    }

    void  SsmGlobalGetMacAclPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmGlobalGetMacAclPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmGlobalGetMacAclPolicyMessage::setPolicyObjId(const ObjectId &policyId)
    {
        m_policyObjId  =  policyId;
    }

    ObjectId  SsmGlobalGetMacAclPolicyMessage::getPolicyObjId() const
    {
        return (m_policyObjId);
    }

}
