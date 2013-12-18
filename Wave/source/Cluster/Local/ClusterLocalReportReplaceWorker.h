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

#ifndef CLUSTERLOCALREPORTREPLACEWORKER_H
#define CLUSTERLOCALREPORTREPLACEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class ClusterLocalObjectManager;
class ClusterLocalReportReplaceMessage;
class PrismLinearSequencerContext;
class WaveWorker;

///Class:
///ClusterLocalReportReplaceWorker
/// 
///Description:
/// This Class is a worker in the ClusterLocalObjectManager
/// It handles the ClusterLocalReportPrimaryNodeChangedMessage received

class ClusterLocalReportReplaceWorker : public WaveWorker
{
    private :
        //Handler for the ClusterLocalReportPrimaryNodeChnagedMessage received from the F/W
        void replaceMessageHandler                    (ClusterLocalReportReplaceMessage *pClusterLocalReportReplaceMessage);
        //Update the node status in the WaveNode MO
        void updateWaveNodeManagedObjectStep                     (PrismLinearSequencerContext *pPrismLinearSequencerContext);
   
    protected :
    public :
                 ClusterLocalReportReplaceWorker (ClusterLocalObjectManager *pClusterLocalObjetManager);
        virtual ~ClusterLocalReportReplaceWorker ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // CLUSTERLOCALREPORTREPLACEWORKER_H
