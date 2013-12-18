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
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/

#ifndef VCSNODEPSMESSAGINGCONTEXT
#define VCSNODEPSMESSAGINGCONTEXT

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

namespace DcmNs
{

class VcsFabricLocalSwitchInfoObject;
class ValClientSynchronousConnection;

class VcsNodeRemoteMessagingContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                    VcsNodeRemoteMessagingContext       (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);

                                   ~VcsNodeRemoteMessagingContext       ();

       vector<UI32>                 getDomainIds                        () const;
       void                         setDomainIds                        (const vector<UI32> &domainIds);
       void                         addToDomainIdVector                 (const UI32 &domainId);

       void                         setPDomainList                      (u_short *fabDomainList);
       u_short                     *getPDomainList                      () const; 

       void                         addToValClientSynchronousConnection (const UI32 &domainId, ValClientSynchronousConnection *pValClientSynchronousConnection);
    // Now the data members
       void                         disconnectFromServer                (const UI32 &domainId);
    private :

        vector<UI32>                                 m_domainId;
        u_short                                     *m_fabDomainList;
        map<UI32, ValClientSynchronousConnection* >  m_mapDomainIdToClientSynchronousConnection;

    protected :
    public :
};

class VcsNodePSMessagingContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                        VcsNodePSMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
									    VcsNodePSMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);

                                       ~VcsNodePSMessagingContext ();
       vector<UI32>                     getMappedId () const;
       vector<UI32>                     getDomainId () const;
	   PrismMessage                    *getPMessage () const;
	   ResourceId                       getCompletionStatus () const;
       UI32                             getEstablishPrincipalCase () const;

       void                             setMappedId (const vector<UI32> &mapped_id);
       void                             setDomainId (const vector<UI32> &domain_id);
	   void							    setPMessage (PrismMessage *pMessage);
	   void							    setCompletionStatus (ResourceId &status);
       void                             setEstablishPrincipalCase (const UI32 &establishPrincipalCase);
       void                             setPDomainList (u_short *fabDomainList);
       u_short                         *getPDomainList () const; 
       // Add domain id in connected status to list if reset message needs to be sent to the node.
       void                             addToDomainIdInConnectedStatusVector (UI32 domainId);
       vector<UI32>                     getDomainIdInConnectedStatusVector () const;    
       void                             setSwitchInfoParametersP (VcsFabricLocalSwitchInfoObject *switchInfo);
       VcsFabricLocalSwitchInfoObject   *getSwitchInfoParametersP () const;
       void                             setPSenderContext (void *pSenderContext);
       void                            *getPSenderContext ();
                

    // Now the data members

    private :
        vector<UI32>                    m_mappedId;
        vector<UI32>                    m_domainId;
        UI32                            m_establishPrincipalCase;
		PrismMessage                   *m_pMessage;
        u_short                        *m_fabDomainList;
        VcsFabricLocalSwitchInfoObject *m_switchInfo;
        vector<UI32>                    m_domainIdsInConnectedStatus;
        void                            *m_pSenderContext;

    protected :
    public :
};

class VcsNodePSSynchronousMessagingContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
                                    VcsNodePSSynchronousMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                   ~VcsNodePSSynchronousMessagingContext ();
       vector<UI32>                 getMappedId () const;
       vector<UI32>                 getDomainId () const;
	   PrismMessage					*getPMessage () const;
	   ResourceId					getCompletionStatus () const;
       UI32                         getEstablishPrincipalCase () const;

       void                         setMappedId (const vector<UI32> &mapped_id);
       void                         setDomainId (const vector<UI32> &domain_id);
	   void							setPMessage (PrismMessage *pMessage);
	   void							setCompletionStatus (ResourceId &status);
       void                         setEstablishPrincipalCase (const UI32 &establishPrincipalCase);
       void                         setPDomainList (u_short *fabDomainList);
       u_short                     *getPDomainList () const; 
       void                         setSwitchInfoParametersP (VcsFabricLocalSwitchInfoObject *switchInfo);
       VcsFabricLocalSwitchInfoObject   *getSwitchInfoParametersP () const;
                

    // Now the data members

    private :
        vector<UI32>                    m_mappedId;
        vector<UI32>                    m_domainId;
        UI32                            m_establishPrincipalCase;
		PrismMessage				   *m_pMessage;
        u_short                        *m_fabDomainList;
        VcsFabricLocalSwitchInfoObject *m_switchInfo;

    protected :
    public :
};

}

#endif // VCSNODEPSMESSAGINGCONTEXT
