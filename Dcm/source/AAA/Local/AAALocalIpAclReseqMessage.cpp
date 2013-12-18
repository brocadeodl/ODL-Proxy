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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Hari Om                                                      *
 **************************************************************************/

#include "AAA/Local/AAALocalIpAclReseqMessage.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

    AAALocalIpAclReseqMessage::AAALocalIpAclReseqMessage ()
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALIPACLRESEQ)
    {
    }

    AAALocalIpAclReseqMessage:: AAALocalIpAclReseqMessage (const string &policyName,const UI64 &seqId, const UI64 &incSeqId)
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALIPACLRESEQ),
        m_policyName    (policyName),
        m_seqId         (seqId),
        m_incSeqId      (incSeqId)
    {
    }

    AAALocalIpAclReseqMessage::~AAALocalIpAclReseqMessage ()
    {
    }

    void  AAALocalIpAclReseqMessage::setupAttributesForSerialization()
    {
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeUI64(& m_seqId,"sequenceId"));
        addSerializableAttribute (new AttributeUI64(&m_incSeqId,"incsequenceId"));
    }

    void  AAALocalIpAclReseqMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  AAALocalIpAclReseqMessage::getPolicyName() const
    {
        return (m_policyName);
    }


    void AAALocalIpAclReseqMessage::setSeqId(const UI64 &seqId)
    {
      m_seqId   =  seqId;
    }

    UI64 AAALocalIpAclReseqMessage::getSeqId() const
    {
        return (m_seqId);
    }
    void AAALocalIpAclReseqMessage::setIncSeqId(const UI64 &IncSeqId)
    {
      m_incSeqId   =  IncSeqId;
    }

    UI64 AAALocalIpAclReseqMessage::getIncSeqId() const
    {
        return (m_incSeqId);
    }

}
