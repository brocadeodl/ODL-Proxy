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

#include "Cluster/ClusterFailoverContext.h"
#include "Cluster/PrismCluster.h"
#include "Cluster/NodeManagedObject.h"
#include "Cluster/Local/WaveNode.h"

namespace WaveNs
{

ClusterFailoverContext::ClusterFailoverContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps, const FrameworkObjectManagerFailoverReason &failoverReason, const vector<LocationId> &failedLocationIds)
    : PrismSynchronousLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_failoverReason (failoverReason),
      m_failedLocationIds (failedLocationIds),
      m_pPrismCluster       (NULL),
      m_pNodeManagedObjects (NULL),
      m_isPrimaryChanged    (false),
      m_isClusterExists     (true),
      m_pThisWaveNode       (NULL)
{
}

ClusterFailoverContext::ClusterFailoverContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps, const FrameworkObjectManagerFailoverReason &failoverReason, const vector<LocationId> &failedLocationIds)
    : PrismSynchronousLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_failoverReason (failoverReason),
      m_failedLocationIds (failedLocationIds),
      m_pPrismCluster       (NULL),
      m_pNodeManagedObjects (NULL),
      m_isPrimaryChanged    (false),
      m_isClusterExists     (true),
      m_pThisWaveNode       (NULL)
{
}

ClusterFailoverContext::~ClusterFailoverContext ()
{
    if (NULL != m_pPrismCluster)
    {
        delete m_pPrismCluster;
            m_pPrismCluster = NULL;
    }

    if (NULL != m_pNodeManagedObjects)
    {
        UI32 numberOfNodeManagedObjects = m_pNodeManagedObjects->size ();
        UI32 i                          = 0;

        for (i = 0; i < numberOfNodeManagedObjects; i++)
        {
            delete ((*m_pNodeManagedObjects)[i]);
        }

        m_pNodeManagedObjects->clear ();
        delete m_pNodeManagedObjects;
        m_pNodeManagedObjects = NULL;
    }
}

FrameworkObjectManagerFailoverReason ClusterFailoverContext::getFailoverReason () const
{
    return (m_failoverReason);
}

vector<LocationId> &ClusterFailoverContext::getFailedLocationIds ()
{
    return (m_failedLocationIds);
}

PrismCluster *ClusterFailoverContext::getPPrismCluster ()
{
    return (m_pPrismCluster);
}

void ClusterFailoverContext::setPPrismCluster (PrismCluster *pPrismCluster)
{
    m_pPrismCluster = pPrismCluster;
}

vector<WaveManagedObject *> *ClusterFailoverContext::getPNodeManagedObjects ()
{
    return (m_pNodeManagedObjects);
}

void ClusterFailoverContext::setPNodeManagedObjects (vector<WaveManagedObject *> *pNodeManagedObjects)
{
    m_pNodeManagedObjects = pNodeManagedObjects;
}

bool ClusterFailoverContext::getIsPrimaryChanged () const
{
    return (m_isPrimaryChanged);
}

void ClusterFailoverContext::setIsPrimaryChanged (const bool &isPrimaryChanged)
{
    m_isPrimaryChanged = isPrimaryChanged;
}

bool ClusterFailoverContext::isAFailedLocationId (const LocationId &locationId)
{
    UI32 numberOfFailedLocationIds = m_failedLocationIds.size ();
    UI32 i                         = 0;

    for (i = 0; i < numberOfFailedLocationIds; i++)
    {
        if (locationId == m_failedLocationIds[i])
        {
            return (true);
        }
    }

    return (false);
}

bool ClusterFailoverContext::getIsClusterExists () const
{
    return (m_isClusterExists);
}

void ClusterFailoverContext::setIsClusterExists (const bool &isClusterExists)
{
    m_isClusterExists = isClusterExists;
}

WaveNode *ClusterFailoverContext::getThisWaveNode () const
{
    return (m_pThisWaveNode);
}

void ClusterFailoverContext::setThisWaveNode (WaveNode *pWaveNode)
{
    m_pThisWaveNode = pWaveNode;
}

}
