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

#include "Nsm/Global/NsmGlobalReAssocIpAclPolicyMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"

namespace DcmNs
{

    NsmGlobalReAssocIpAclPolicyMessage::NsmGlobalReAssocIpAclPolicyMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (), NSMGLOBALREASSOCIPACLPOLICY)
    {
    }

    NsmGlobalReAssocIpAclPolicyMessage::NsmGlobalReAssocIpAclPolicyMessage (const ObjectId &oldPolicy, const ObjectId &newPolicy)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (), NSMGLOBALREASSOCIPACLPOLICY),
        m_oldPolicyId   (oldPolicy),
        m_newPolicyId   (newPolicy)
    {
    }

    NsmGlobalReAssocIpAclPolicyMessage::~NsmGlobalReAssocIpAclPolicyMessage ()
    {
    }

    void  NsmGlobalReAssocIpAclPolicyMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeObjectId(&m_oldPolicyId,"oldPolicyId"));
        addSerializableAttribute (new AttributeObjectId(&m_newPolicyId,"newPolicyId"));
    }

    void  NsmGlobalReAssocIpAclPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  NsmGlobalReAssocIpAclPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }

	void  NsmGlobalReAssocIpAclPolicyMessage::setOldPolicyId(const ObjectId &policyId)
    {
        m_oldPolicyId  =  policyId;
    }

    ObjectId  NsmGlobalReAssocIpAclPolicyMessage::getOldPolicyId() const
    {
        return (m_oldPolicyId);
    }
	void  NsmGlobalReAssocIpAclPolicyMessage::setNewPolicyId(const ObjectId &policyId)
    {
        m_newPolicyId  =  policyId;
    }

    ObjectId  NsmGlobalReAssocIpAclPolicyMessage::getNewPolicyId() const
    {
        return (m_newPolicyId);
    }

}
