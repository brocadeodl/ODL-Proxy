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
 *   Author : Bhavin                                                     *
 **************************************************************************/

#include "Ssm/Global/SsmGlobalAddIpAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"

namespace DcmNs
{

    SsmGlobalAddIpAclPolicyMessage::SsmGlobalAddIpAclPolicyMessage ()
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),SSMGLOBALADDIPACLPOLICY)
    {
    }

    SsmGlobalAddIpAclPolicyMessage::SsmGlobalAddIpAclPolicyMessage (const string &policyName,
		const UI32 &policyType)
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),
		SSMGLOBALADDIPACLPOLICY),
        m_policyName    (policyName),
        m_policyType    (policyType)
    {
    }

    SsmGlobalAddIpAclPolicyMessage::~SsmGlobalAddIpAclPolicyMessage ()
    {
    }

    void  SsmGlobalAddIpAclPolicyMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeUI32(&m_policyType,"policyType"));
        addSerializableAttribute (new AttributeObjectId(&m_oldPolicyId,"oldPolicyId"));
        addSerializableAttribute (new AttributeObjectId(&m_newPolicyId,"newPolicyId"));
    }

    void  SsmGlobalAddIpAclPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmGlobalAddIpAclPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmGlobalAddIpAclPolicyMessage::setPolicyType(const UI32 &policyType)
    {
        m_policyType  =  policyType;
    }

    UI32  SsmGlobalAddIpAclPolicyMessage::getPolicyType() const
    {
        return (m_policyType);
    }

	void  SsmGlobalAddIpAclPolicyMessage::setOldPolicyId(const ObjectId &policyId)
    {
        m_oldPolicyId  =  policyId;
    }

    ObjectId  SsmGlobalAddIpAclPolicyMessage::getOldPolicyId() const
    {
        return (m_oldPolicyId);
    }
	void  SsmGlobalAddIpAclPolicyMessage::setNewPolicyId(const ObjectId &policyId)
    {
        m_newPolicyId  =  policyId;
    }

    ObjectId  SsmGlobalAddIpAclPolicyMessage::getNewPolicyId() const
    {
        return (m_newPolicyId);
    }

}
