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

/**
 *@file VcsFabConfigSnapshotContext.h 
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc. All
 * rights reserved.
 * Description: This file declares the
 *              asynchronous context used during
 *              Create and Restore Snapshot request
 *              
 * Author :     Anil ChannaveeraSetty 
 *  
 */
#ifndef VCSFABCONFIGSNAPSHOTCONTEXT_H
#define VCSFABCONFIGSNAPSHOTCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Types/DateTime.h"

using namespace WaveNs;

namespace DcmNs
{
class VcsFabConfigSnapshotContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                        VcsFabConfigSnapshotContext             (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                        VcsFabConfigSnapshotContext             (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfStepsi, const string &snapshotId, const UI32 &rbridgeId, const UI32 &opcode );
        virtual                        ~VcsFabConfigSnapshotContext             ();

                    void                setSnapshotId                           (const string& snapshotId );
                    string              getSnapshotId                           () const;

                    void                setRbridgeId                            (const UI32 &rbridgeId );
                    UI32                getRbridgeId                            () const;

                    void                setSWBDModel                            ( const SI32 &SWBDModel );
                    SI32                getSWBDModel                            () const;

                    void                setIsNodePrimary                        (const bool &isNodePrimary);
                    bool                getIsNodePrimary                        () const;
            
                    void                setNodeLocationId                       ( const LocationId &nodeLocationId );
                    LocationId          getNodeLocationId                       () const;

                    void                setSnapshotCreatedTime                  ( const DateTime &snapshotCreatedTime );
                    DateTime            getSnapshotCreatedTime                  () const;
         
                    void                setRestoreOnDisconnectedNode            (const bool &restoreOnDisconnectedNode);
                    bool                getRestoreOnDisconnectedNode            () const;            
                    
     // Now the data members
    private :
                    string              m_snapshotId;
                    UI32                m_rbridgeId;
                    SI32                m_SWBDModel;
                    bool                m_isNodePrimary;
                    LocationId          m_nodeLocationId;
                    DateTime            m_snapshotCreatedTime;
                    bool                m_restoreOnDisconnectedNode;

    protected :
    public :                                                    
};

}
#endif // VCSFABCONFIGSNAPSHOTCONTEXT_H
