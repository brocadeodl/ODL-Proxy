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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Bhavin                                                     *
 **************************************************************************/

#include "Ssm/Global/SsmGlobalGetIpAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"

namespace DcmNs
{

    SsmGlobalGetIpAclPolicyMessage::SsmGlobalGetIpAclPolicyMessage ()
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),SSMGLOBALGETIPACLPOLICY)
    {
    }

    SsmGlobalGetIpAclPolicyMessage::SsmGlobalGetIpAclPolicyMessage (const string &policyName)
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),SSMGLOBALGETIPACLPOLICY),
        m_policyName    (policyName)
    {
    }

    SsmGlobalGetIpAclPolicyMessage::~SsmGlobalGetIpAclPolicyMessage ()
    {
    }

    void  SsmGlobalGetIpAclPolicyMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeObjectId(&m_policyObjId,"policyObjId"));
    }

    void  SsmGlobalGetIpAclPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmGlobalGetIpAclPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmGlobalGetIpAclPolicyMessage::setPolicyObjId(const ObjectId &policyId)
    {
        m_policyObjId  =  policyId;
    }

    ObjectId  SsmGlobalGetIpAclPolicyMessage::getPolicyObjId() const
    {
        return (m_policyObjId);
    }

}
