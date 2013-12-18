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
 *   Author : Aashish Akhouri                                              *
 ***************************************************************************/

#ifndef CLUSTERLOCALREPORTREMOVEDNODEFROMCLUSTERWORKER_H
#define CLUSTERLOCALREPORTREMOVEDNODEFROMCLUSTERWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class ClusterLocalObjectManager;
class ClusterLocalReportRemovedNodeFromClusterMessage;
class PrismLinearSequencerContext;
class WaveWorker;

///Class:
///ClusterLocalReportRemovedNodeFromClusterWorker
/// 
///Description:
/// This Class is a worker in the ClusterLocalObjectManager
/// It handles the ClusterLocalReportRemovedNodeFromClusterMessage received
/// from the F/w.It updates the specific status of the node to STAND_ALONE

class ClusterLocalReportRemovedNodeFromClusterWorker : public WaveWorker
{
    private :
        //Handler for the ClusterLocalReportRemovedNodeFromClusterMessage received from the F/W
        void nodeRemovedMessageHandler                           (ClusterLocalReportRemovedNodeFromClusterMessage* pClusterLocalReportRemovedNodeFromClusterMessage);

        //Update the node status in the WaveNode MO
        void updateWaveNodeManagedObjectStep                     (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                    
    protected :
    public :
                  ClusterLocalReportRemovedNodeFromClusterWorker(ClusterLocalObjectManager *pClusterLocalObjetManager);
        virtual ~ClusterLocalReportRemovedNodeFromClusterWorker ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif //LUSTERLOCALREPORTREMOVEDNODEFROMCLUSTERWORKER_H 
