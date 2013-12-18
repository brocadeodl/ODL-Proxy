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

#include "ClientInterface/Ssm/SsmMacAclShowMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"

namespace DcmNs
{
    SsmMacAclShowMessage::SsmMacAclShowMessage () : DcmManagementInterfaceMessage (SSMMACACLSHOW)
    {
    }

    SsmMacAclShowMessage::~SsmMacAclShowMessage ()
    {
    }

    void  SsmMacAclShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyname"));
        addSerializableAttribute (new AttributeUI32(&m_policyType,"policytype"));
        addSerializableAttribute (new AttributeString(&m_ifname,"ifname"));
		addSerializableAttribute (new AttributeUI32(&m_binding,"binding"));
		addSerializableAttribute (new AttributeUI32(&m_detail,"detail"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"iftype"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute(new AttributeUI32(&m_mappedId,"mappedId"));
    }

    UI32  SsmMacAclShowMessage::getOpCode() const
    {
        return (m_opcode);
    }
    
    UI32  SsmMacAclShowMessage::getPolicyType() const
    {
        return (m_policyType);
    }
    
    UI32  SsmMacAclShowMessage::getIfType() const
    {
        return (m_ifType);
    }


    string  SsmMacAclShowMessage::getIfName() const
    {
        return (m_ifname);
    }

	UI32 SsmMacAclShowMessage::getBinding() const
	{
		return (m_binding);
	}

	UI32 SsmMacAclShowMessage::getDetail() const
	{
		return (m_detail);
	}
	
    string  SsmMacAclShowMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void SsmMacAclShowMessage::setOpcode(const UI32 &opcode)
	{
		m_opcode = opcode;
	}

    void SsmMacAclShowMessage::setIfType(const UI32 &type)
	{
		m_ifType = type;
	}
	
    void SsmMacAclShowMessage::setPolicyType(const UI32 &type)
	{
		m_policyType = type;
	}

	void  SsmMacAclShowMessage::setIfName(const string &name)
	{
		m_ifname = name;
	}
	
	void  SsmMacAclShowMessage::setBinding(const UI32 &binding)
	{
		m_binding = binding;
	}

	void  SsmMacAclShowMessage::setDetail(const UI32 &detail)
	{
		m_detail =detail;
	}
	
	void  SsmMacAclShowMessage::setPolicyName(const string &policyname)
	{
		m_policyName = policyname;
	}
    
	void  SsmMacAclShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  SsmMacAclShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }


	void  SsmMacAclShowMessage::getOutputBufferRef(UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer = findBuffer(bufferNum, size);
	}

	const void *SsmMacAclShowMessage::getCStructureForInputs ()
	{

		if (m_opcode == ACL_STATS_POLICY_WITH_INTF || m_opcode == ACL_STATS_ONLY_INTF || m_opcode == ACL_STATS_ONLY_POLICY) 
		{
        	L2aclShowMsg_t *pInput = NULL;

        	pInput = (L2aclShowMsg_t*)calloc(1, sizeof (L2aclShowMsg_t));
			if (pInput == NULL) 
			{
				return (pInput);
			}
        	strncpy (pInput->ifname, m_ifname.c_str(), MAX_NAME_STR);
        	if (pInput->opcode != ACL_STATS_ONLY_INTF) 
			{
        		strncpy (pInput->policyname, m_policyName.c_str(), MAX_NAME_STR);
			}
        	pInput->iftype = (intfType)m_ifType;
        	pInput->opcode = m_opcode;
        	return (pInput);
		} 
		else if (m_opcode == SHOW_MAC_ACCESS_GROUP || m_opcode == SHOW_MAC_ACCESS_GROUP_INTF) 
		{
			L2aclShowMsg_t *pInput = NULL;

			pInput = (L2aclShowMsg_t *) calloc (1, sizeof (L2aclShowMsg_t));
			if (pInput == NULL) 
			{
				return (pInput);
			}
			strncpy (pInput->ifname, m_ifname.c_str(), MAX_NAME_STR);
			if (m_opcode == SHOW_MAC_ACCESS_GROUP_INTF) 
			{
				pInput->iftype = (intfType)m_ifType;
			}
			pInput->detail = m_detail;
			pInput->bindingType = m_binding;
			pInput->opcode = m_opcode;
			return (pInput);
		} 
		else 
		{
			return (NULL);
		}
	}

	void SsmMacAclShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		ssm_mgmt_buffer_num_t  buff_num;
        memset(&buff_num, 0, sizeof(buff_num));

		if (m_opcode == ACL_STATS_POLICY_WITH_INTF || m_opcode == ACL_STATS_ONLY_INTF || m_opcode == ACL_STATS_ONLY_POLICY) 
		{
			L2aclIntfStatsMsg_t *resp_t = (L2aclIntfStatsMsg_t *)pOutputCStructure;
			int numrec;
			if (resp_t) 
			{
				numrec = (resp_t->numrecords == 0) ? 1 : resp_t->numrecords;
				buff_num.buff.mapped_id = this->getMappedId();
				buff_num.buff.tag = SHOW_STATS_ACL_INTF;

				addBuffer(buff_num.num, sizeof(L2aclIntfStatsMsg_t) + ((numrec - 1) * sizeof (ruleStats_t)),(unsigned char *)pOutputCStructure, false);
			}
		} 
		else if (m_opcode == SHOW_MAC_ACCESS_GROUP || m_opcode == SHOW_MAC_ACCESS_GROUP_INTF) 
		{
			L2aclShowAccessGroup_t *resp_t = (L2aclShowAccessGroup_t *)pOutputCStructure;
			int numrec;
			if (resp_t) 
			{
				numrec = (resp_t->numrecords == 0) ? 1 : resp_t->numrecords;
				buff_num.buff.mapped_id = this->getMappedId();
				buff_num.buff.tag = m_opcode;

				addBuffer(buff_num.num, sizeof(L2aclShowAccessGroup_t) + ((numrec - 1) * sizeof (L2aclShowMsg_t)), (unsigned char *)pOutputCStructure, false);
			}
		}
	}
}
