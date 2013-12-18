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
 *   Author : Srikanth                                                     *
 **************************************************************************/

#include "ClientInterface/Ssm/SsmMacAclReseqMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"

namespace DcmNs
{

    SsmMacAclReseqMessage::SsmMacAclReseqMessage ()
        : DcmManagementInterfaceMessage (SSMMACACLRESEQUENCE)
    {
    }

    SsmMacAclReseqMessage:: SsmMacAclReseqMessage (const string &policyName,const UI64 &seqId, const UI64 &incSeqId)
        : DcmManagementInterfaceMessage (SSMMACACLRESEQUENCE),
        m_policyName    (policyName),
        m_seqId         (seqId),
        m_incSeqId      (incSeqId)
    {
    }

    SsmMacAclReseqMessage::~SsmMacAclReseqMessage ()
    {
    }

    void  SsmMacAclReseqMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeUI64(& m_seqId,"sequenceId"));
        addSerializableAttribute (new AttributeUI64(&m_incSeqId,"incsequenceId"));
    }

	const void *SsmMacAclReseqMessage::getCStructureForInputs ()
	{
        ssmAcl_dcmd_resequence_t *pInput = (ssmAcl_dcmd_resequence_t *)calloc (1, sizeof(ssmAcl_dcmd_resequence_t));
        
		strncpy (pInput->acl_name, m_policyName.c_str (), SSMACL_NAME_SIZE);
        pInput->seqId  		= m_seqId;
        pInput->incSeqId  	= m_incSeqId;
        pInput->opcode    	= SSMACL_DCMD_RESEQUENCE ;
		/* Should always be below value as it is L2 acl msg*/
		pInput->type        = SSMACL_TYPE_L2_E; 
        return (pInput);
	}

	void SsmMacAclReseqMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{

	}

    void  SsmMacAclReseqMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmMacAclReseqMessage::getPolicyName() const
    {
        return (m_policyName);
    }


    void SsmMacAclReseqMessage::setSeqId(const UI64 &seqId)
    {
      m_seqId   =  seqId;
    }

    UI64 SsmMacAclReseqMessage::getSeqId() const
    {
        return (m_seqId);
    }
    void SsmMacAclReseqMessage::setIncSeqId(const UI64 &IncSeqId)
    {
      m_incSeqId   =  IncSeqId;
    }

    UI64 SsmMacAclReseqMessage::getIncSeqId() const
    {
        return (m_incSeqId);
    }

}
