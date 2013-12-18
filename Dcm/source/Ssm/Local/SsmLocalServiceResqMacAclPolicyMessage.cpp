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
#include "Ssm/Local/SsmTypes.h"
#include "Ssm/Local/SsmLocalServiceResqMacAclPolicyMessage.h"

namespace DcmNs
{

    SsmLocalServiceResqMacAclPolicyMessage::SsmLocalServiceResqMacAclPolicyMessage ()
        :  PrismMessage (SsmLocalObjectManager::getPrismServiceId (),SSMLOCALSERVICERESEQMACACLPOLICY)
    {
    }


    SsmLocalServiceResqMacAclPolicyMessage::SsmLocalServiceResqMacAclPolicyMessage (const string &policyName,const UI64 &SeqId,const UI64 &IncSeqId)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (),SSMLOCALSERVICERESEQMACACLPOLICY),
        m_policyName    (policyName),
        m_seqId         (SeqId), 
        m_incSeqId      (IncSeqId) 
    {
    }
    SsmLocalServiceResqMacAclPolicyMessage::~SsmLocalServiceResqMacAclPolicyMessage ()
    {
    }

    void  SsmLocalServiceResqMacAclPolicyMessage::setupAttributesForSerialization()
    {
         PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeUI64(&m_seqId,"sequenceId"));
        addSerializableAttribute (new AttributeUI64(&m_incSeqId,"incsequenceId"));
    }

    void  SsmLocalServiceResqMacAclPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmLocalServiceResqMacAclPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }


    void SsmLocalServiceResqMacAclPolicyMessage::setSeqId(const UI64 &seqId)
    {
      m_seqId   =  seqId;
    }

    UI64 SsmLocalServiceResqMacAclPolicyMessage::getSeqId() const
    {
        return (m_seqId);
    }
    void SsmLocalServiceResqMacAclPolicyMessage::setIncSeqId(const UI64 &IncSeqId)
    {
      m_incSeqId   =  IncSeqId;
    }

    UI64 SsmLocalServiceResqMacAclPolicyMessage::getIncSeqId() const
    {
        return (m_incSeqId);
    }

}
