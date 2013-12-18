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
 *   Author : Bhavin                                                       *
 **************************************************************************/

#include "ClientInterface/Ssm/SsmMacIpAclShowMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "ssmAcl_dcmd_public.h"

namespace DcmNs
{

    SsmMacIpAclShowMessage::SsmMacIpAclShowMessage() : DcmManagementInterfaceMessage (SSMMACIPACLSHOW)
    {
    }

    SsmMacIpAclShowMessage::SsmMacIpAclShowMessage(const string &PolicyName, const string &ifName, const UI32 &opCode, const UI32 &aclType, const UI32 &direction , const UI32 &mappedId)
        : DcmManagementInterfaceMessage (SSMMACIPACLSHOW),
        m_PolicyName   (PolicyName),
        m_ifName       (ifName),
        m_opCode       (opCode),
        m_aclType      (aclType),
        m_direction    (direction),
		m_mappedId     (mappedId)
    {
    }

    SsmMacIpAclShowMessage::SsmMacIpAclShowMessage(const string &PolicyName, const string &ifName, const UI32 &opCode, const UI32 &aclType, const UI32 &direction , const UI32 &mappedId, const UI64 &bindOid)
        : DcmManagementInterfaceMessage (SSMMACIPACLSHOW),
        m_PolicyName   (PolicyName),
        m_ifName       (ifName),
        m_opCode       (opCode),
        m_aclType      (aclType),
        m_direction    (direction),
		m_mappedId     (mappedId),
    	m_bindOid	   (bindOid)
    {
    }

    SsmMacIpAclShowMessage::~SsmMacIpAclShowMessage ()
    {
    }

    void  SsmMacIpAclShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute(new AttributeString(&m_PolicyName,"PolicyName"));
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_aclType,"aclType"));
        addSerializableAttribute(new AttributeUI32(&m_direction,"direction"));
		addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
		addSerializableAttribute (new AttributeUI64(&m_bindOid,"bindOid"));
    }

    void  SsmMacIpAclShowMessage::setPolicyName(const string &PolicyName)
    {
        m_PolicyName  =  PolicyName;
    }

    string  SsmMacIpAclShowMessage::getPolicyName() const
    {
        return (m_PolicyName);
    }

    void  SsmMacIpAclShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  SsmMacIpAclShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  SsmMacIpAclShowMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SsmMacIpAclShowMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  SsmMacIpAclShowMessage::setAclType(const UI32 &aclType)
    {
        m_aclType = aclType;
    }

    UI32  SsmMacIpAclShowMessage::getAclType() const
    {
        return (m_aclType);
    }

    void SsmMacIpAclShowMessage::setDirection(const UI32 &direction)
    {
        m_direction = direction;
    }

    UI32  SsmMacIpAclShowMessage::getDirection() const
    {
        return (m_direction);
    }

    void  SsmMacIpAclShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  SsmMacIpAclShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    void  SsmMacIpAclShowMessage::setBindOid(const UI64 &bindOid)
	{
		m_bindOid  =  bindOid;
	}

	UI64  SsmMacIpAclShowMessage::getBindOid() const
	{
		return (m_bindOid);
	}

    const void *SsmMacIpAclShowMessage::getCStructureForInputs ()
    {
        ssmAcl_dcmd_acl_show_request_t *pInput = (ssmAcl_dcmd_acl_show_request_t *) calloc (1, sizeof(ssmAcl_dcmd_acl_show_request_t));
        memset(pInput, 0, sizeof(ssmAcl_dcmd_acl_show_request_t));

        strncpy (pInput->acl_name, m_PolicyName.c_str(), SSMACL_NAME_SIZE);
        strncpy (pInput->if_name, m_ifName.c_str(), 64);
        pInput->opcode     = (ssmAcl_dcmd_opcode_t)m_opCode;
		pInput->direction  = (ssmAcl_binding_direction)m_direction;
		pInput->type	   = (ssmAcl_type)m_aclType;
		pInput->bind_oid   = m_bindOid;
        return (pInput);
    }

	void SsmMacIpAclShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        ssm_mgmt_buffer_num_t  buff_num;
        memset(&buff_num, 0, sizeof(buff_num));

		ssmAcl_dcmd_status_response  *resp_t = (ssmAcl_dcmd_status_response *)pOutputCStructure;

		if (!resp_t)
			return;
				
		size_t numOfRules 	= (resp_t->resp_data).intf_rec[0].acl_rec[0].data.num_of_rules;
		size_t numOfRec 	= ((resp_t->resp_data).num_intf == 1) ? 1 : (resp_t->resp_data).num_intf;
			
		buff_num.buff.mapped_id = this->getMappedId();
		
		if (resp_t->opcode == SSMACL_DCMD_SHOW_ACCESSLIST_ALL)
		{
			struct 			ssmAcl_status_resp_data 	*resp_data_status 	= NULL;
			struct 			ssmAcl_status_intf_record 	*intf_rec 			= NULL;
			unsigned int 	intfAclRecords 									= 0;
			
			resp_data_status 	= 	(struct ssmAcl_status_resp_data *)(&(resp_t->resp_data));
			resp_data_status 	+= 	1;
			intf_rec 			= 	(struct ssmAcl_status_intf_record *) (resp_data_status);

			for (size_t j = 0; j < numOfRec; j++) 
			{
				unsigned int 	intfAcls 		= 0;
				char 			*status_buff 	= NULL;
				
				if ((intf_rec->num_acls) > 1)
					intfAcls = ((intf_rec->num_acls) -1);
				else 
					intfAcls = 0;
					
				intfAclRecords += intfAcls;
				
				status_buff = (char *)(intf_rec) + sizeof (struct ssmAcl_status_intf_record) + 
					((intfAcls) * sizeof (struct ssmAcl_status_acl_record));	
				
				intf_rec = (struct ssmAcl_status_intf_record *)(status_buff);
			}      
			
			unsigned Totalbuffsize = sizeof(ssmAcl_dcmd_status_response) + ((numOfRec) * sizeof (ssmAcl_status_intf_record)) 
				+ ((intfAclRecords) * sizeof (struct ssmAcl_status_acl_record));
			
			addBuffer (buff_num.num, Totalbuffsize, (unsigned char *)pOutputCStructure, false);
		}
		else
		{
			addBuffer(buff_num.num, (sizeof(ssmAcl_dcmd_status_response)
				+ ((numOfRules -1) *sizeof (ssmAcl_rule_status))), (unsigned char *)pOutputCStructure, false);
		}
	}
}
