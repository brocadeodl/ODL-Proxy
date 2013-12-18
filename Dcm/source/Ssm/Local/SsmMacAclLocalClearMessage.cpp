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
 *   Author : Bhavin                                                  *
 **************************************************************************/

#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/SsmTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Ssm/Local/SsmMacAclLocalClearMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
namespace DcmNs
{

    SsmMacAclLocalClearMessage::SsmMacAclLocalClearMessage ()
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (),
                SSMMACACLLOCALCLEAR)
    {
	}

    SsmMacAclLocalClearMessage::SsmMacAclLocalClearMessage (const string &l2aclPolicyName, 
            const string &ifName, const UI32 &cmdCode, bool isGwRequest, const UI32 &type)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), SSMMACACLLOCALCLEAR),
        m_l2aclPolicyName    (l2aclPolicyName),
		m_if_name       (ifName),
		m_cmdCode    (cmdCode), 
        m_isGwRequest (isGwRequest),
		m_ifType (type)
    {
    }

    SsmMacAclLocalClearMessage::~SsmMacAclLocalClearMessage ()
    {
    }

    void  SsmMacAclLocalClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute(new AttributeString(&m_l2aclPolicyName,"l2aclPolicyName"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, 
                                "isGwRequest"));
		addSerializableAttribute (new AttributeString(&m_if_name,"ifName"));
		addSerializableAttribute(new AttributeUI32(&m_ifType,"ifType"));
    }

    void  SsmMacAclLocalClearMessage::setL2aclPolicyName(const string &l2aclPolicyName)
    {
        m_l2aclPolicyName  =  l2aclPolicyName;
    }

    string  SsmMacAclLocalClearMessage::getL2aclPolicyName() const
    {
        return (m_l2aclPolicyName);
    }

    void  SsmMacAclLocalClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  SsmMacAclLocalClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void SsmMacAclLocalClearMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool SsmMacAclLocalClearMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void  SsmMacAclLocalClearMessage::setIfName(const string &ifName)
    {
        m_if_name  =  ifName;
    }

    string  SsmMacAclLocalClearMessage::getIfName() const
    {
        return (m_if_name);
    }

    UI32  SsmMacAclLocalClearMessage::getIfType() const
    {
        return (m_ifType);
    }

    void SsmMacAclLocalClearMessage::setIfType(const UI32 &type)
    {
        m_ifType = type;
    }


}
