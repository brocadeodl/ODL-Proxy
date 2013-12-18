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

#ifndef VCSNODELOCALMESSAGINGCONTEXT_h
#define VCSNODELOCALMESSAGINGCONTEXT_h

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"

namespace DcmNs
{

class VcsNodeLocalMessagingContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
                                    VcsNodeLocalMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                   ~VcsNodeLocalMessagingContext ();
       VcsNodeLocalManagedObject    *getPVcsNodeLocalManagedObject() const;
       UI32                         getMappedId () const;
       UI32                         getDomainId () const;
       UI32                         getVcsId    () const;
       UI32                         getNewRbridgeId () const; 
       UI32                         getNewVcsId () const;
       UI32                         getVcsMode  () const;
       ResourceId                   getNewClusterType () const;
       ResourceId                   getClusterType () const;
       bool                         getPrincipalControlFailoverFlag () const; 
       bool                         getNoVcsEnableSecondaryFlag () const;
       bool                         getRbridgeIdResetFlag () const;
       MacAddress                   getFcfMac   () const;
       WorldWideName                getSwitchWWN () const;
       string                       getSnapshotId () const; 
       void                         setPVcsNodeLocalManagedObject (VcsNodeLocalManagedObject *vcsNodeLocalManagedObject);
       void                         setMappedId (const UI32 &mapped_id);
       void                         setDomainId (const UI32 &domain_id);
       void                         setVcsId    (const UI32 &vcs_id);
       void                         setNewRbridgeId (const UI32 &newRbridgeId ); 
       void                         setNewVcsId (const UI32 &newVcsId);
       void                         setPrincipalControlFailoverFlag ( const bool &noVcsEnablePrincipalControlFailoverFlag ); 
       void                         setNoVcsEnableSecondaryFlag ( const bool &noVcsEnableSecondaryFlag );    
       void                         setRbridgeIdResetFlag ( const bool &rbridgeIdResetFlag );    
       void                         setFcfMac   (const MacAddress &mac_addr);
       void                         setSwitchWWN (const WorldWideName &wwn);
       void                         setSnapshotId (const string &snapshotId); 
       void                         setVcsMode (const UI32 &vcsMode);         
       void                         setClusterType (const ResourceId &clusterType);
       void                         setNewClusterType (const ResourceId &newClusterType);

    // Now the data members

    private :
        UI32                        m_mappedId;
        UI32                        m_domainId;
        UI32                        m_vcsId;
        UI32                        m_newRbridgeId;
        UI32                        m_newVcsId;
        UI32                        m_vcsMode; 
        ResourceId                  m_clusterType;
        ResourceId                  m_newClusterType;
        bool                        m_noVcsEnablePrincipalControlFailoverFlag;
        bool                        m_noVcsEnableSecondaryFlag;
        bool                        m_rbridgeIdResetFlag;
        MacAddress                  m_fcfMac;
        WorldWideName               m_wwn;
        VcsNodeLocalManagedObject   *m_pVcsNodeLocalManagedObject;
        string                      m_snapshotId;

    protected :
    public :
};

}

#endif // VCSNODELOCALMESSAGINGCONTEXT_h
