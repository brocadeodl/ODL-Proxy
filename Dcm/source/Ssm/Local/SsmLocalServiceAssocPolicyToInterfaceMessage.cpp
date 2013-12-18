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

#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/SsmLocalServiceAssocPolicyToInterfaceMessage.h"
#include "Ssm/Local/SsmTypes.h"

namespace DcmNs
{

    SsmLocalServiceAssocPolicyToInterfaceMessage::SsmLocalServiceAssocPolicyToInterfaceMessage ()
        : ManagementInterfaceMessage (SsmLocalObjectManager::getClassName (),SSMLOCALSERVICEASSOCPOLICYTOINTERFACE)
    {
    }

    SsmLocalServiceAssocPolicyToInterfaceMessage::SsmLocalServiceAssocPolicyToInterfaceMessage (const string &ifName,const string &policyName,const UI32 &opcode)
        : ManagementInterfaceMessage (SsmLocalObjectManager::getClassName (),SSMLOCALSERVICEASSOCPOLICYTOINTERFACE),
        m_ifName    (ifName),
        m_policyName    (policyName),
        m_opcode    (opcode)
    {
    }

    SsmLocalServiceAssocPolicyToInterfaceMessage::~SsmLocalServiceAssocPolicyToInterfaceMessage ()
    {
    }

    void  SsmLocalServiceAssocPolicyToInterfaceMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
    }

    void  SsmLocalServiceAssocPolicyToInterfaceMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  SsmLocalServiceAssocPolicyToInterfaceMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  SsmLocalServiceAssocPolicyToInterfaceMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmLocalServiceAssocPolicyToInterfaceMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmLocalServiceAssocPolicyToInterfaceMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  SsmLocalServiceAssocPolicyToInterfaceMessage::getOpcode() const
    {
        return (m_opcode);
    }

}
