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

#include "ClientInterface/Ssm/SsmMacAclIntfAssocBulkMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"

namespace DcmNs
{

    SsmMacAclIntfAssocBulkMessage::SsmMacAclIntfAssocBulkMessage ()
        : DcmManagementInterfaceMessage (SSMMACACLINTFASSOCBULK),
		m_numRecords (0)
    {
    }


    SsmMacAclIntfAssocBulkMessage::~SsmMacAclIntfAssocBulkMessage ()
    {
    }

    void  SsmMacAclIntfAssocBulkMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI64(&m_numRecords,"numRecords"));
		addSerializableAttribute (new AttributeStringVector(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeStringVector(&m_ifname,"ifname"));
		addSerializableAttribute (new AttributeUI32Vector(&m_opcode,"opcode"));
		addSerializableAttribute (new AttributeUI32Vector(&m_direction,"direction"));
    }


    vector<string>  SsmMacAclIntfAssocBulkMessage::getPolicyNameVector() const
    {
        return (m_policyName);
    }

    vector<UI32>  SsmMacAclIntfAssocBulkMessage::getOpCodeVector() const
    {
        return (m_opcode);
    }


    vector<string>  SsmMacAclIntfAssocBulkMessage::getIfNameVector() const
    {
        return (m_ifname);
    }

    void  SsmMacAclIntfAssocBulkMessage::setNumRecords(const UI64 &num)
    {
        m_numRecords  =  num;
    }


    UI64  SsmMacAclIntfAssocBulkMessage::getNumRecords() const
    {
        return (m_numRecords);
    }

	void SsmMacAclIntfAssocBulkMessage::setIntfDetails(const string &ifname, const string &policy, const UI32 &opcode, const UI32 &direction)
	{
		m_policyName.push_back(policy);
		m_ifname.push_back(ifname);
		m_opcode.push_back(opcode);
		m_direction.push_back(direction);
	}


	const void *SsmMacAclIntfAssocBulkMessage::getCStructureForInputs ()
	{
        ssmAcl_dcmd_binding_bulk_t *pInput 	= NULL;
			
       	UI64 num 							= m_numRecords;
        UI64 i								= 0;

		
        if (num > 0) {
        	pInput = (ssmAcl_dcmd_binding_bulk_t *)calloc(1, (sizeof (ssmAcl_dcmd_binding_bulk_t) + (num - 1) * sizeof(ssmAcl_dcmd_acl_t)));
			if (pInput == NULL) {
				return (pInput);
			}

			memset(pInput, 0, (sizeof (ssmAcl_dcmd_binding_bulk_t) + (num - 1) * sizeof(ssmAcl_dcmd_acl_t)));
			
        	pInput->numrecords = num;
        	for (i = 0; i < num; i++) {
        		strncpy (pInput->binding_list[i].acl_name, m_policyName[i].c_str (), MAX_NAME_STR);
        		strncpy (pInput->binding_list[i].if_name, m_ifname[i].c_str (), MAX_NAME_STR);
        		pInput->binding_list[i].direction 	= (ssmAcl_binding_direction) m_direction[i];
				pInput->binding_list[i].opcode 		= (ssmAcl_dcmd_opcode_t)	m_opcode[i];
				/* Policy Type should alwasy be L2 in this Class*/
				pInput->binding_list[i].type        = SSMACL_TYPE_L2_E;
			}

		} else  {
        	pInput = (ssmAcl_dcmd_binding_bulk_t *)calloc(1, (sizeof (ssmAcl_dcmd_binding_bulk_t)));
        	pInput->numrecords = 0;
		}
        
        return (pInput);
	}

	void SsmMacAclIntfAssocBulkMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
