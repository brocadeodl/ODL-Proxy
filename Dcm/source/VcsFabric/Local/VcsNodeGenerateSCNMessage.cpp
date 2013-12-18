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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeGenerateSCNMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeGenerateSCNMessage::VcsNodeGenerateSCNMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GENERATE_SCN)
{
    m_domainId = 0;
    m_pDomainId = 0;
    m_scnType = 0;
}

VcsNodeGenerateSCNMessage::VcsNodeGenerateSCNMessage (const UI32 &scnType, const UI32 &domainId, const UI32 &pDomainId)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GENERATE_SCN),
      m_scnType                  (scnType),
      m_domainId                 (domainId),
      m_pDomainId                (pDomainId)
{
}

VcsNodeGenerateSCNMessage::~VcsNodeGenerateSCNMessage ()
{
}

void VcsNodeGenerateSCNMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_scnType,   "scnType"));
     addSerializableAttribute (new AttributeUI32 (&m_domainId,  "domainId"));
     addSerializableAttribute (new AttributeUI32 (&m_pDomainId, "pDomainId"));
}

UI32 VcsNodeGenerateSCNMessage::getSCNType () const
{
    return (m_scnType);
}

void VcsNodeGenerateSCNMessage::setSCNType (const UI32 &type)
{
    m_scnType = type;
}

UI32 VcsNodeGenerateSCNMessage::getDomainId () const
{
    return (m_domainId);
}

void VcsNodeGenerateSCNMessage::setDomainId (const UI32 &domainId)
{
    m_domainId = domainId;
}

UI32 VcsNodeGenerateSCNMessage::getPDomainId () const
{
    return (m_pDomainId);
}

void VcsNodeGenerateSCNMessage::setPDomainId (const UI32 &pDomainId)
{
    m_pDomainId = pDomainId;
}

}
