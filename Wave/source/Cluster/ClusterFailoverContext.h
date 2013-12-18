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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef CLUSTERFAILOVERCONTEXT_H
#define CLUSTERFAILOVERCONTEXT_H

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"

#include <vector>

using namespace std;

namespace WaveNs
{

class PrismCluster;
class NodeManagedObject;
class WaveManagedObject;
class WaveNode;

class ClusterFailoverContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
                                                      ClusterFailoverContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps, const FrameworkObjectManagerFailoverReason &failoverReason, const vector<LocationId> &failedLocationIds);
                                                      ClusterFailoverContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps, const FrameworkObjectManagerFailoverReason &failoverReason, const vector<LocationId> &failedLocationIds);
        virtual                                      ~ClusterFailoverContext ();

                FrameworkObjectManagerFailoverReason  getFailoverReason      () const;
                vector<LocationId>                   &getFailedLocationIds   ();

                PrismCluster                         *getPPrismCluster       ();
                void                                  setPPrismCluster       (PrismCluster *pPrismCluster);

                vector<WaveManagedObject *>          *getPNodeManagedObjects ();
                void                                  setPNodeManagedObjects (vector<WaveManagedObject *> *pNodeManagedObjects);

                bool                                  getIsPrimaryChanged    () const;
                void                                  setIsPrimaryChanged    (const bool &isPrimaryChanged);

                bool                                  isAFailedLocationId    (const LocationId &locationId);
                bool                                  getIsClusterExists     () const;
                void                                  setIsClusterExists     (const bool &isClusterExists);
                WaveNode                             *getThisWaveNode        () const;
                void                                  setThisWaveNode        (WaveNode *pWaveNode);

    // Now the data members

    private :
        FrameworkObjectManagerFailoverReason  m_failoverReason;
        vector<LocationId>                    m_failedLocationIds;

        PrismCluster                         *m_pPrismCluster;
        vector<WaveManagedObject *>          *m_pNodeManagedObjects;
        bool                                  m_isPrimaryChanged;
        bool                                  m_isClusterExists;
        WaveNode                             *m_pThisWaveNode;

    protected :
    public :
};

}

#endif // CLUSTERFAILOVERCONTEXT_H
