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

#ifndef VCSFABMESSAGECONTEXT
#define VCSFABMESSAGECONTEXT

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "VcsFabric/Global/VcsClusterInfoMessage.h"

namespace DcmNs
{

class VcsFabricLocalSwitchInfoObject;
class VcsFabAddRejoinNodeWorker;

class VcsFabMessagingContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                                    VcsFabMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                                   ~VcsFabMessagingContext ();
       vector<UI32>                                 getMappedId () const;
       vector<UI32>                                 getDomainId () const;
       PrismMessage                                *getPMessage () const;
       ResourceId                                   getCompletionStatus () const;

       void                                         setMappedId (const vector<UI32> &mapped_id);
       void                                         setDomainId (const vector<UI32> &domain_id);
       void                                         setPMessage (PrismMessage *pMessage);
       void                                         setCompletionStatus (ResourceId &status);
       void                                         setClusterReboot (bool isClusterReboot);
                

       void                                         addSwitchInfoToAddList (const VcsFabricLocalSwitchInfoObject &switchInfo);
       void                                         addSwitchInfoToRejoinList(const VcsFabricLocalSwitchInfoObject &switchInfo);
       void                                         addToUncontrolledFailoverList (LocationId failedLocationId);
       void                                         addToControlledFailoverList (LocationId failedLocationId);

       void                                         addSwitchInfoToReplaceRejoinList(const VcsFabricLocalSwitchInfoObject &switchInfo);
       vector<VcsFabricLocalSwitchInfoObject>       getSwitchesToBeAdded ();
       vector<VcsFabricLocalSwitchInfoObject>       getSwitchesToBeRejoined ();
       vector<LocationId>                           getSwitchesForUncontrolledFailover ();
       vector<LocationId>                           getSwitchesForControlledFailover ();
       vector<VcsFabricLocalSwitchInfoObject>       getSwitchesToBeReplaceRejoined ();
       void                                         setRejoinNodeStatus (ResourceId rejoinStatus);
       ResourceId                                   getRejoinNodeStatus () const;
       void                                         setAddNodeStatus (ResourceId addStatus);
       ResourceId                                   getAddNodeStatus () const;
       bool                                         getClusterReboot () const;

       void                                         setReplaceRejoinNodeStatus (ResourceId rejoinStatus);
       ResourceId                                   getReplaceRejoinNodeStatus () const;

       void                                         getFilenamesToSync ( vector<string> &filenamesToSync ) ;
       void                                         setFilenamesToSync ( const vector<string> &filenamesToSync ); 
       // Now the data members

    private :
        vector<UI32>                                m_mappedId;
        vector<UI32>                                m_domainId;
        PrismMessage                               *m_pMessage;

        //Vectors for storing the switches to be added or rejoined 
        vector<VcsFabricLocalSwitchInfoObject>      m_switchesToBeAdded;
        vector<VcsFabricLocalSwitchInfoObject>      m_switchesToBeRejoined;
        vector<LocationId>                          m_uncontrolledFailoverSwitches;
        vector<LocationId>                          m_controlledFailoverSwitches;

        //fields for storing status of operations
        ResourceId                                  m_rejoinNodeStatus;
        ResourceId                                  m_addNodeStatus;
        bool                                        m_isClusterReboot; 
	    vector<VcsFabricLocalSwitchInfoObject>      m_switchesToBeReplaceRejoined;
        ResourceId                                  m_replaceRejoinNodeStatus;
        vector<string>                              m_filenamesToSync;

    protected :
    public :
};

}

#endif // VCSFABMESSAGECONTEXT
