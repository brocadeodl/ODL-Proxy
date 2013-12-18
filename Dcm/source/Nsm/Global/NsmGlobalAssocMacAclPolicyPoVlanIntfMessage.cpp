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
 *   Author : pasu														   *
 **************************************************************************/

#include "Nsm/Global/NsmGlobalAssocMacAclPolicyPoVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::NsmGlobalAssocMacAclPolicyPoVlanIntfMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (), NSMGLOBALASSOCMACACLPOLICYPOVLANINTF)
    {
		m_cmdCode = 0;
		m_ifId = 0;
		m_ifType = 0;
    }

    NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::NsmGlobalAssocMacAclPolicyPoVlanIntfMessage (const UI32 &ifId, const UI8 &ifType, const string &aclName, const UI32 &cmdCode, const ssmAcl_binding_direction &direction)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALASSOCMACACLPOLICYPOVLANINTF),
        m_ifId      (ifId),
        m_ifType    (ifType),
        m_aclName   (aclName),
        m_cmdCode   (cmdCode),
		m_direction	(direction)
    {
    }

    NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::~NsmGlobalAssocMacAclPolicyPoVlanIntfMessage ()
    {
    }

    void  NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_ifId,"ifId"));
        addSerializableAttribute (new AttributeUI8(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_aclName,"aclName"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeEnum((UI32 *)&m_direction,"direction"));
    }

    void  NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::setAclName(const string &aclName)
    {
        m_aclName  =  aclName;
    }

    string  NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::getAclName() const
    {
        return (m_aclName);
    }

    void  NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::setIfId(const UI32 &ifId)
    {
        m_ifId  =  ifId;
    }

    UI32  NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::getIfId() const
    {
        return (m_ifId);
    }

    void  NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::setIfType(const UI8 &iftype)
    {
        m_ifType  =  iftype;
    }

    UI8  NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    
	void NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::setDirection(ssmAcl_binding_direction &direction)
    {
        m_direction = direction;
    }

    ssmAcl_binding_direction NsmGlobalAssocMacAclPolicyPoVlanIntfMessage::getDirection () const
    {
        return ((ssmAcl_binding_direction)(m_direction));
    }

}
