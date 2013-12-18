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
 *   Author : Bhavin														   *
 **************************************************************************/

#include "Nsm/Local/Ssm/NsmAssocIpAclPolicyPhyIntfMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmAssocIpAclPolicyPhyIntfMessage::NsmAssocIpAclPolicyPhyIntfMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (), NSMASSOCIPACLPOLICYPHYINTF)
    {
		m_cmdCode = 0;
    }

    NsmAssocIpAclPolicyPhyIntfMessage::NsmAssocIpAclPolicyPhyIntfMessage (const string &ifName, const string &aclName, const UI8 &ifType, const UI32 &cmdCode, const ssmAcl_binding_direction &direction)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMASSOCIPACLPOLICYPHYINTF),
        m_ifName    (ifName),
        m_aclName   (aclName),
		m_ifType	(ifType),
        m_cmdCode   (cmdCode),
		m_direction	(direction)
    {
    }

    NsmAssocIpAclPolicyPhyIntfMessage::~NsmAssocIpAclPolicyPhyIntfMessage ()
    {
    }

    void  NsmAssocIpAclPolicyPhyIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI8(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_aclName,"aclName"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_direction,"direction"));

    }
    
    void  NsmAssocIpAclPolicyPhyIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmAssocIpAclPolicyPhyIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmAssocIpAclPolicyPhyIntfMessage::setAclName(const string &aclName)
    {
        m_aclName  =  aclName;
    }

    string  NsmAssocIpAclPolicyPhyIntfMessage::getAclName() const
    {
        return (m_aclName);
    }

    void  NsmAssocIpAclPolicyPhyIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmAssocIpAclPolicyPhyIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmAssocIpAclPolicyPhyIntfMessage::setIfType(const UI8 &iftype)
    {
        m_ifType  =  iftype;
    }

    UI8  NsmAssocIpAclPolicyPhyIntfMessage::getIfType() const
    {
        return (m_ifType);
    }
	
    void NsmAssocIpAclPolicyPhyIntfMessage::setDirection(ssmAcl_binding_direction &direction)
    {
        m_direction = direction;
    }

    ssmAcl_binding_direction NsmAssocIpAclPolicyPhyIntfMessage::getDirection () const
    {
        return ((ssmAcl_binding_direction)(m_direction));
    }

}
