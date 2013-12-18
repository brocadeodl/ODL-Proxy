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

#include "Cluster/Local/ClusterLocalSetThisNodeIpAddressWorker.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Cluster/Local/ClusterLocalSetThisNodeIpAddressMessage.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Cluster/Local/WaveNode.h"

namespace WaveNs
{

ClusterLocalSetThisNodeIpAddressWorker::ClusterLocalSetThisNodeIpAddressWorker (ClusterLocalObjectManager *pClusterLocalObjectManager)
    : WaveWorker (pClusterLocalObjectManager)
{
    addOperationMap (CLUSTER_LOCAL_SET_THIS_NODE_IP_ADDRESS, reinterpret_cast<PrismMessageHandler> (&ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressMessageHandler));
}

ClusterLocalSetThisNodeIpAddressWorker::~ClusterLocalSetThisNodeIpAddressWorker ()
{
}

void ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressMessageHandler (ClusterLocalSetThisNodeIpAddressMessage *pClusterLocalSetThisNodeIpAddressMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressUpdatePrismFrameworkObjectManagerStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetThisNodeIpAddressWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressUpdateThisWaveNodeStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetThisNodeIpAddressWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetThisNodeIpAddressWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetThisNodeIpAddressWorker::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pClusterLocalSetThisNodeIpAddressMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressValidateStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressValidateStep : Entering ...");

    ClusterLocalSetThisNodeIpAddressMessage *pClusterLocalSetThisNodeIpAddressMessage = reinterpret_cast<ClusterLocalSetThisNodeIpAddressMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    string                                   ipAddress                                = pClusterLocalSetThisNodeIpAddressMessage->getIpAddress ();
    ResourceId                               status                                   = WAVE_MESSAGE_SUCCESS;

    if (true != (FrameworkToolKit::isIpAddressValid (ipAddress)))
    {
        status = CLUSTER_LOCAL_SET_THIS_NODE_IP_ADDRESS_STATUS_INVALID_IP_ADDRESS;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressUpdatePrismFrameworkObjectManagerStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressUpdatePrismFrameworkObjectManagerStep : Entering ...");

    ClusterLocalSetThisNodeIpAddressMessage *pClusterLocalSetThisNodeIpAddressMessage = reinterpret_cast<ClusterLocalSetThisNodeIpAddressMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    string                                   ipAddress                                = pClusterLocalSetThisNodeIpAddressMessage->getIpAddress ();

    PrismFrameworkObjectManager::updateIpAddressForThisLocation (ipAddress);

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressUpdateThisWaveNodeStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressUpdateThisWaveNodeStep : Entering ...");

    ClusterLocalSetThisNodeIpAddressMessage *pClusterLocalSetThisNodeIpAddressMessage = reinterpret_cast<ClusterLocalSetThisNodeIpAddressMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    string                                   ipAddress                                = pClusterLocalSetThisNodeIpAddressMessage->getIpAddress ();
    WaveManagedObject                       *pWaveManagedObject                       = NULL;
    ObjectId                                 thisWaveNodeObjectId                     = FrameworkToolKit::getThisWaveNodeObjectId ();
    WaveNode                                *pWaveNode                                = NULL;
    ResourceId                               status                                   = WAVE_MESSAGE_SUCCESS;

    pWaveManagedObject = queryManagedObject (thisWaveNodeObjectId);

    if (NULL == pWaveManagedObject)
    {
        trace (TRACE_LEVEL_FATAL, "ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressUpdateThisWaveNodeStep : Cannot find the Local Wave Node.  Something went terribly wrong.");
        prismAssert (false, __FILE__, __LINE__);

        status = CLUSTER_LOCAL_SET_THIS_NODE_IP_ADDRESS_STATUS_CANNOT_FIND_LOCAL_NODE;
    }
    else
    {
        pWaveNode = dynamic_cast<WaveNode *> (pWaveManagedObject);

        if (NULL == pWaveNode)
        {
            trace (TRACE_LEVEL_FATAL, "ClusterLocalSetThisNodeIpAddressWorker::setThisNodeIpAddressUpdateThisWaveNodeStep : Cannot cast the Local Wave Node.  Something went terribly wrong.");
            prismAssert (false, __FILE__, __LINE__);

            status = CLUSTER_LOCAL_SET_THIS_NODE_IP_ADDRESS_STATUS_CANNOT_CAST_LOCAL_NODE;
        }
        else
        {
            updateWaveManagedObject (pWaveNode);

            pWaveNode->setIpAddress (ipAddress);
        }
        pPrismLinearSequencerContext->addManagedObjectForGarbageCollection (pWaveManagedObject);
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

}
