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

#include "Ssm/Global/SsmGlobalRemMacAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"

namespace DcmNs
{

    SsmGlobalRemMacAclPolicyMessage::SsmGlobalRemMacAclPolicyMessage ()
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),SSMGLOBALREMMACACLPOLICY)
    {
    }

    SsmGlobalRemMacAclPolicyMessage::SsmGlobalRemMacAclPolicyMessage (const string &policyName,const UI32 &policyType)
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),SSMGLOBALREMMACACLPOLICY),
        m_policyName    (policyName),
        m_policyType    (policyType)
    {
    }

    SsmGlobalRemMacAclPolicyMessage::~SsmGlobalRemMacAclPolicyMessage ()
    {
    }

    void  SsmGlobalRemMacAclPolicyMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeUI32(&m_policyType,"policyType"));
		addSerializableAttribute (new AttributeObjectId(&m_policyId,"policyId"));
    }

    void  SsmGlobalRemMacAclPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmGlobalRemMacAclPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmGlobalRemMacAclPolicyMessage::setPolicyType(const UI32 &policyType)
    {
        m_policyType  =  policyType;
    }

    UI32  SsmGlobalRemMacAclPolicyMessage::getPolicyType() const
    {
        return (m_policyType);
    }
	
	void  SsmGlobalRemMacAclPolicyMessage::setPolicyId(const ObjectId &policyId)
	{
		m_policyId = policyId;
	}

	ObjectId  SsmGlobalRemMacAclPolicyMessage::getPolicyId() const
	{
		return (m_policyId);
	}

}
