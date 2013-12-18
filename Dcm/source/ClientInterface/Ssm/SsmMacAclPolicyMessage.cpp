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

#include "ClientInterface/Ssm/SsmMacAclPolicyMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    SsmMacAclPolicyMessage::SsmMacAclPolicyMessage ()
        : DcmManagementInterfaceMessage (SSMMACACLPOLICY)
    {
    }

    SsmMacAclPolicyMessage::SsmMacAclPolicyMessage (const string &policyName,const UI32 &policyType, const UI32 opCode)
        : DcmManagementInterfaceMessage (SSMMACACLPOLICY),
        m_policyName    (policyName),
        m_policyType    (policyType),
        m_opCode		(opCode)
    {
    }

    SsmMacAclPolicyMessage::SsmMacAclPolicyMessage (const string &policyName,const string &ifname, const UI32 opCode)
        : DcmManagementInterfaceMessage (SSMMACACLPOLICY),
        m_policyName    (policyName),
        m_ifname		(ifname),
        m_opCode		(opCode)
    {
    }

    SsmMacAclPolicyMessage::SsmMacAclPolicyMessage (const string &policyName, const string &ifname, const UI32 &policyType, const UI32 opCode, const ssmAcl_binding_direction direction)
        : DcmManagementInterfaceMessage (SSMIPACLPOLICY),
        m_policyName    (policyName),
        m_ifname        (ifname),
        m_policyType    (policyType),
        m_opCode        (opCode),
        m_direction     (direction)
    {
    }

 
	SsmMacAclPolicyMessage::~SsmMacAclPolicyMessage ()
    {
    }

    void  SsmMacAclPolicyMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeString(&m_ifname,"ifname"));
        addSerializableAttribute (new AttributeUI32(&m_policyType,"policyType"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
		addSerializableAttribute (new AttributeEnum((UI32 *)&m_direction,"direction"));
    }

    void  SsmMacAclPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmMacAclPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }
    void  SsmMacAclPolicyMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SsmMacAclPolicyMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  SsmMacAclPolicyMessage::setPolicyType(const UI32 &policyType)
    {
        m_policyType  =  policyType;
    }

    UI32  SsmMacAclPolicyMessage::getPolicyType() const
    {
        return (m_policyType);
    }

    void  SsmMacAclPolicyMessage::setIfname(const string &ifname)
    {
        m_ifname =  ifname;
    }

    string SsmMacAclPolicyMessage::getIfname() const
    {
        return (m_ifname);
    }

    void SsmMacAclPolicyMessage::setDirection(const ssmAcl_binding_direction &direction)
    {
        m_direction = direction;
    }

    ssmAcl_binding_direction SsmMacAclPolicyMessage::getDirection () const
    {
        return ((ssmAcl_binding_direction)(m_direction));
    }

	const void *SsmMacAclPolicyMessage::getCStructureForInputs ()
	{

        ssmAcl_dcmd_acl_t *pInput = (ssmAcl_dcmd_acl_t *) calloc (1, sizeof(ssmAcl_dcmd_acl_t));

        pInput->opcode 	= (ssmAcl_dcmd_opcode_t) m_opCode;
		pInput->type	= (ssmAcl_type) m_policyType;
        strncpy (pInput->acl_name, m_policyName.c_str (), SSMACL_NAME_SIZE);
        if (m_policyType == SSMACL_TYPE_L2_E) {
			strncpy (pInput->if_name, m_ifname.c_str (), MAX_NAME_STR);
			pInput->direction = m_direction;
		} else { 
			pInput->direction = SSMACL_BINDING_DIRECTION_UNKNOWN_E;
			m_ifname = "";
			strncpy (pInput->if_name, m_ifname.c_str (), MAX_NAME_STR);
		}

        return (pInput);
	}

	void SsmMacAclPolicyMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
