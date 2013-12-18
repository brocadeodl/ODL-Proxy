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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef CLUSTERDELETECONTEXT_H
#define CLUSTERDELETECONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"

#include <vector>

using namespace std;

namespace WaveNs
{

class PrismCluster;
class WaveManagedObject;

class ClusterDeleteContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                     ClusterDeleteContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                     ClusterDeleteContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                    ~ClusterDeleteContext ();

        PrismCluster                *getPPrismCluster     () const;
        void                         setPPrismCluster     (PrismCluster *pPrismCluster);
        vector<WaveManagedObject *> *getPSecondaryNodes   () const;
        void                         setPSecondaryNodes   (vector<WaveManagedObject *> *pSecondaryNodes);

    // Now the data members

    private :
        PrismCluster                 *m_pPrismCluster;
        vector<WaveManagedObject *>  *m_pSecondaryNodes;

    protected :
    public :
};

typedef ClusterDeleteContext ClusterBootContext;

}

#endif // CLUSTERDELETECONTEXT_H
