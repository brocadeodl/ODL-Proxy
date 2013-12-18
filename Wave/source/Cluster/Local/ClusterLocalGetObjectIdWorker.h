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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef CLUSTERLOCALGETOBJECTIDWORKER_H
#define CLUSTERLOCALGETOBJECTIDWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class ClusterLocalObjectManager;
class ClusterLocalGetSlotObjectIdMessage;
class ClusterLocalGetLocationObjectIdMessage;
class ClusterLocalGetSlotNumberMessage; 
class ClusterLocalGetLocationIdMessage;
class WaveWorker;
class ClusterLocalWaveSlotManagementWorker;

class ClusterLocalGetObjectIdWorker: public WaveWorker
{
    private :
                void 	    getSlotObjectIdMessageHandler       (ClusterLocalGetSlotObjectIdMessage *pClusterLocalGetSlotObjectIdMessage);
                void        getLocationObjectIdMessageHandler   (ClusterLocalGetLocationObjectIdMessage *pClusterLocalGetLocationObjectIdMessage);
                void 	    getSlotNumberMessageHandler         (ClusterLocalGetSlotNumberMessage *pClusterLocalGetSlotNumberMessage);
                void        getLocationIdMessageHandler         (ClusterLocalGetLocationIdMessage *pClusterLocalGetLocationIdMessage);
                ObjectId    getObjectIdForLocation              (LocationId locationId);
                    
    protected :
    public :
                            ClusterLocalGetObjectIdWorker       (ClusterLocalObjectManager *pClusterLocalObjectManager);
        virtual            ~ClusterLocalGetObjectIdWorker       ();

    // Now the data members

    private :
    protected :
    public :
		friend	class ClusterLocalWaveSlotManagementWorker;
		
};

}

#endif //CLUSTERLOCALSETHAROLEWORKER_H 
