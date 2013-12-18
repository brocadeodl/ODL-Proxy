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

#include "Nsm/Global/Ssm/SsmDeleteAssociationwithDisconnectedMessage.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Nsm/Global/NsmGlobalDeletePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    SsmDeleteAssociationwithDisconnectedMessage::SsmDeleteAssociationwithDisconnectedMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),SSMDELETEASSOCIATIONWITHDISCONNECTEDMESSAGE)
    {
    }

    SsmDeleteAssociationwithDisconnectedMessage::SsmDeleteAssociationwithDisconnectedMessage (const string &policyName)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),SSMDELETEASSOCIATIONWITHDISCONNECTEDMESSAGE),
        m_policyName    (policyName)
    {
    }

    SsmDeleteAssociationwithDisconnectedMessage::~SsmDeleteAssociationwithDisconnectedMessage ()
    {
    }

    void  SsmDeleteAssociationwithDisconnectedMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeObjectId(&m_policyId,"PolicyId"));
    }

    void  SsmDeleteAssociationwithDisconnectedMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmDeleteAssociationwithDisconnectedMessage::getPolicyName() const
    {
        return (m_policyName);
    }


    void  SsmDeleteAssociationwithDisconnectedMessage::setPolicyId(const ObjectId &policyId)
    {
        m_policyId  =  policyId;
    }

    ObjectId  SsmDeleteAssociationwithDisconnectedMessage::getPolicyId() const
    {
        return (m_policyId);
    }

    void  SsmDeleteAssociationwithDisconnectedMessage::setAclType(const UI32 &aclType)
    {
    	m_aclType  =  aclType;
    }

    UI32  SsmDeleteAssociationwithDisconnectedMessage::getAclType() const
    {
        return (m_aclType);
    }

}

