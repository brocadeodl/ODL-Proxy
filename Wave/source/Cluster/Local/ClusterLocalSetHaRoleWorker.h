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
 *   Author : Leifang Hu                                                   *
 ***************************************************************************/

#ifndef CLUSTERLOCALSETHAROLEWORKER_H
#define CLUSTERLOCALSETHAROLEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class ClusterLocalObjectManager;
class ClusterLocalSetHaRoleMessage;
class PrismLinearSequencerContext;
class WaveWorker;

class ClusterLocalSetHaRoleWorker : public WaveWorker
{
    private :
        void setHaRoleMessageHandler                           (ClusterLocalSetHaRoleMessage* pClusterLocalSetHaRoleMessage);

        void updateWaveHaNodeManagedObjectStep                     (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                    
    protected :
    public :
                  ClusterLocalSetHaRoleWorker(ClusterLocalObjectManager *pClusterLocalObjetManager);
        virtual ~ClusterLocalSetHaRoleWorker ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif //CLUSTERLOCALSETHAROLEWORKER_H 
