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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"
#include "Nsm/Local/Ssm/NsmLocalAssocIpAclPolicyVeIntfMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmLocalAssocIpAclPolicyVeIntfMessage::NsmLocalAssocIpAclPolicyVeIntfMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (), NSMLOCALASSOCIPACLPOLICYVEINTF)
    {
		m_cmdCode = 0;
		m_ifId = 0;
		m_ifType = 0;
    }

    NsmLocalAssocIpAclPolicyVeIntfMessage::NsmLocalAssocIpAclPolicyVeIntfMessage (const UI32 &ifId, const UI8 &ifType, const string &aclName, const UI32 &cmdCode, const ssmAcl_binding_direction &direction)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMLOCALASSOCIPACLPOLICYVEINTF),
        m_ifId      (ifId),
        m_ifType    (ifType),
        m_aclName   (aclName),
        m_cmdCode   (cmdCode),
		m_direction	(direction)
    {
    }

    NsmLocalAssocIpAclPolicyVeIntfMessage::~NsmLocalAssocIpAclPolicyVeIntfMessage ()
    {
    }

    void  NsmLocalAssocIpAclPolicyVeIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_ifId,"ifId"));
        addSerializableAttribute (new AttributeUI8(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_aclName,"aclName"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_direction,"direction"));
    }

    void  NsmLocalAssocIpAclPolicyVeIntfMessage::setAclName(const string &aclName)
    {
        m_aclName  =  aclName;
    }

    string  NsmLocalAssocIpAclPolicyVeIntfMessage::getAclName() const
    {
        return (m_aclName);
    }

    void  NsmLocalAssocIpAclPolicyVeIntfMessage::setIfId(const UI32 &ifId)
    {
        m_ifId  =  ifId;
    }

    UI32  NsmLocalAssocIpAclPolicyVeIntfMessage::getIfId() const
    {
        return (m_ifId);
    }

    void  NsmLocalAssocIpAclPolicyVeIntfMessage::setIfType(const UI8 &iftype)
    {
        m_ifType  =  iftype;
    }

    UI8  NsmLocalAssocIpAclPolicyVeIntfMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmLocalAssocIpAclPolicyVeIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmLocalAssocIpAclPolicyVeIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	void NsmLocalAssocIpAclPolicyVeIntfMessage::setDirection(ssmAcl_binding_direction &direction)
    {
        m_direction = direction;
    }

    ssmAcl_binding_direction NsmLocalAssocIpAclPolicyVeIntfMessage::getDirection () const
    {
        return ((ssmAcl_binding_direction)(m_direction));
    }

}
