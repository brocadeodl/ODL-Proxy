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
 *   Author : Bhavin                                                     *
 **************************************************************************/

#include "Ssm/Local/SsmLocalIpAclWorker.h"
#include "Ssm/Local/SsmTypes.h"
#include "Ssm/Local/SsmLocalServiceResqIpAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalObjectManager.h"

namespace DcmNs
{

    SsmLocalServiceResqIpAclPolicyMessage::SsmLocalServiceResqIpAclPolicyMessage ()
        :  PrismMessage (SsmLocalObjectManager::getPrismServiceId (),SSMLOCALSERVICERESEQIPACLPOLICY)
    {
    }


    SsmLocalServiceResqIpAclPolicyMessage::SsmLocalServiceResqIpAclPolicyMessage (const string &policyName,const UI64 &SeqId,const UI64 &IncSeqId)
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (),SSMLOCALSERVICERESEQIPACLPOLICY),
        m_policyName    (policyName),
        m_seqId         (SeqId), 
        m_incSeqId      (IncSeqId) 
    {
    }
    SsmLocalServiceResqIpAclPolicyMessage::~SsmLocalServiceResqIpAclPolicyMessage ()
    {
    }

    void  SsmLocalServiceResqIpAclPolicyMessage::setupAttributesForSerialization()
    {
         PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeUI64(&m_seqId,"sequenceId"));
        addSerializableAttribute (new AttributeUI64(&m_incSeqId,"incsequenceId"));
    }

    void  SsmLocalServiceResqIpAclPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmLocalServiceResqIpAclPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }


    void SsmLocalServiceResqIpAclPolicyMessage::setSeqId(const UI64 &seqId)
    {
      m_seqId   =  seqId;
    }

    UI64 SsmLocalServiceResqIpAclPolicyMessage::getSeqId() const
    {
        return (m_seqId);
    }
    void SsmLocalServiceResqIpAclPolicyMessage::setIncSeqId(const UI64 &IncSeqId)
    {
      m_incSeqId   =  IncSeqId;
    }

    UI64 SsmLocalServiceResqIpAclPolicyMessage::getIncSeqId() const
    {
        return (m_incSeqId);
    }

}
