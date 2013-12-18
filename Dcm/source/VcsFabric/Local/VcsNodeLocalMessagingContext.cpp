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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeLocalMessagingContext.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{

VcsNodeLocalMessagingContext::VcsNodeLocalMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismSynchronousLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_pVcsNodeLocalManagedObject           (NULL)
{
    m_domainId = 0;
    m_mappedId = m_domainId;
    m_newRbridgeId = 0;
    m_vcsId = 0;
    m_noVcsEnablePrincipalControlFailoverFlag = false;
    m_noVcsEnableSecondaryFlag = false;
    m_rbridgeIdResetFlag = true;
    m_newVcsId = 0;   
    m_vcsMode = 0;
    m_clusterType = VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE; 
    m_newClusterType = VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE;
}

VcsNodeLocalMessagingContext::~VcsNodeLocalMessagingContext ()
{
    if (m_pVcsNodeLocalManagedObject != NULL)
    {
        delete (m_pVcsNodeLocalManagedObject);
    }
}

VcsNodeLocalManagedObject* VcsNodeLocalMessagingContext::getPVcsNodeLocalManagedObject () const
{
    return (m_pVcsNodeLocalManagedObject);
}

UI32 VcsNodeLocalMessagingContext::getMappedId () const
{
    return (m_mappedId);
}

UI32 VcsNodeLocalMessagingContext::getDomainId () const
{
    return (m_domainId);
}

MacAddress VcsNodeLocalMessagingContext::getFcfMac () const
{
    return (m_fcfMac);
}

WorldWideName VcsNodeLocalMessagingContext::getSwitchWWN () const
{
    return (m_wwn);
}

UI32 VcsNodeLocalMessagingContext::getNewRbridgeId () const
{
    return (m_newRbridgeId);
}

UI32 VcsNodeLocalMessagingContext::getNewVcsId () const
{
    return (m_newVcsId);
}

UI32 VcsNodeLocalMessagingContext::getVcsId () const
{
    return (m_vcsId);
}

UI32 VcsNodeLocalMessagingContext::getVcsMode () const
{
    return (m_vcsMode);
}

ResourceId VcsNodeLocalMessagingContext::getNewClusterType () const
{
    return (m_newClusterType);
}

ResourceId VcsNodeLocalMessagingContext::getClusterType () const
{
    return (m_clusterType);
}

bool VcsNodeLocalMessagingContext::getPrincipalControlFailoverFlag () const
{
    return (m_noVcsEnablePrincipalControlFailoverFlag);
}

bool VcsNodeLocalMessagingContext::getNoVcsEnableSecondaryFlag  () const
{
    return (m_noVcsEnableSecondaryFlag);
}

void VcsNodeLocalMessagingContext::setPVcsNodeLocalManagedObject (VcsNodeLocalManagedObject *vcsNodeLocalManagedObject)
{
    m_pVcsNodeLocalManagedObject = vcsNodeLocalManagedObject;
}

void VcsNodeLocalMessagingContext::setMappedId (const UI32 &mapped_id)
{
    m_mappedId = mapped_id;
}

void VcsNodeLocalMessagingContext::setDomainId (const UI32 &domain_id)
{
    m_domainId = domain_id;
}

void VcsNodeLocalMessagingContext::setVcsId (const UI32 &vcs_id)
{
    m_vcsId = vcs_id;
}

void VcsNodeLocalMessagingContext::setClusterType (const ResourceId &clusterType)
{
    m_clusterType = clusterType;
}

void VcsNodeLocalMessagingContext::setNewClusterType (const ResourceId &newClusterType)
{
    m_newClusterType = newClusterType;
}

void VcsNodeLocalMessagingContext::setFcfMac (const MacAddress &mac_addr)
{
    m_fcfMac = mac_addr;
}

void VcsNodeLocalMessagingContext::setSwitchWWN (const WorldWideName &wwn)
{
    m_wwn = wwn;
}

void VcsNodeLocalMessagingContext::setNewRbridgeId (const UI32 &newRbridgeId)
{
    m_newRbridgeId = newRbridgeId ;
}

void VcsNodeLocalMessagingContext::setNewVcsId (const UI32 &newVcsId)
{
    m_newVcsId = newVcsId;
}

void VcsNodeLocalMessagingContext::setVcsMode (const UI32 &vcsMode)
{
    m_vcsMode = vcsMode;
}

void VcsNodeLocalMessagingContext::setPrincipalControlFailoverFlag ( const bool &noVcsEnablePrincipalControlFailoverFlag)
{
    m_noVcsEnablePrincipalControlFailoverFlag = noVcsEnablePrincipalControlFailoverFlag;
}

void VcsNodeLocalMessagingContext::setNoVcsEnableSecondaryFlag  ( const bool &noVcsEnableSecondaryFlag)
{
    m_noVcsEnableSecondaryFlag = noVcsEnableSecondaryFlag;
}

void VcsNodeLocalMessagingContext::setRbridgeIdResetFlag  ( const bool &rbridgeIdResetFlag)
{
    m_rbridgeIdResetFlag = rbridgeIdResetFlag;
}

bool VcsNodeLocalMessagingContext::getRbridgeIdResetFlag () const
{
    return m_rbridgeIdResetFlag;
}

string VcsNodeLocalMessagingContext::getSnapshotId () const
{
    return (m_snapshotId);
}

void    VcsNodeLocalMessagingContext::setSnapshotId (const string &snapshotId )
{
    m_snapshotId = snapshotId;
}

}
