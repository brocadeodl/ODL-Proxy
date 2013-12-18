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

#include "Ssm/Global/SsmGlobalAssocPolicyToInterfaceMessage.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"

namespace DcmNs
{

    SsmGlobalAssocPolicyToInterfaceMessage::SsmGlobalAssocPolicyToInterfaceMessage ()
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),SSMGLOBALASSOCPOLICYTOINTERFACE)
    {
    }

    SsmGlobalAssocPolicyToInterfaceMessage::SsmGlobalAssocPolicyToInterfaceMessage (const string &ifName,const string &policyName,const UI32 &opcode)
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),SSMGLOBALASSOCPOLICYTOINTERFACE),
        m_ifName    (ifName),
        m_policyName    (policyName),
        m_opcode    (opcode)
    {
    }

    SsmGlobalAssocPolicyToInterfaceMessage::~SsmGlobalAssocPolicyToInterfaceMessage ()
    {
    }

    void  SsmGlobalAssocPolicyToInterfaceMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
    }

    void  SsmGlobalAssocPolicyToInterfaceMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  SsmGlobalAssocPolicyToInterfaceMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  SsmGlobalAssocPolicyToInterfaceMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmGlobalAssocPolicyToInterfaceMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmGlobalAssocPolicyToInterfaceMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  SsmGlobalAssocPolicyToInterfaceMessage::getOpcode() const
    {
        return (m_opcode);
    }

}
