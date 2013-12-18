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

#include "Cluster/ClusterDeleteContext.h"
#include "Cluster/PrismCluster.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

ClusterDeleteContext::ClusterDeleteContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_pPrismCluster             (NULL),
      m_pSecondaryNodes           (NULL)
{
}

ClusterDeleteContext::ClusterDeleteContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_pPrismCluster             (NULL),
      m_pSecondaryNodes           (NULL)
{
}

ClusterDeleteContext::~ClusterDeleteContext ()
{
    if (NULL != m_pPrismCluster)
    {
        delete m_pPrismCluster;
        m_pPrismCluster = NULL;
    }

    if (NULL != m_pSecondaryNodes)
    {
        UI32 numberOfSecondaryNodes = m_pSecondaryNodes->size ();
        UI32 i                      = 0;

        for (i = 0; i < numberOfSecondaryNodes; i++)
        {
            delete (*m_pSecondaryNodes)[i];
        }

        m_pSecondaryNodes->clear ();

        delete m_pSecondaryNodes;
        m_pSecondaryNodes = NULL;
    }
}

PrismCluster *ClusterDeleteContext::getPPrismCluster () const
{
    return (m_pPrismCluster);
}

void ClusterDeleteContext::setPPrismCluster (PrismCluster *pPrismCluster)
{
    m_pPrismCluster = pPrismCluster;
}

vector<WaveManagedObject *> *ClusterDeleteContext::getPSecondaryNodes () const
{
    return (m_pSecondaryNodes);
}

void ClusterDeleteContext::setPSecondaryNodes (vector<WaveManagedObject *> *pSecondaryNodes)
{
    m_pSecondaryNodes = pSecondaryNodes;
}

}
