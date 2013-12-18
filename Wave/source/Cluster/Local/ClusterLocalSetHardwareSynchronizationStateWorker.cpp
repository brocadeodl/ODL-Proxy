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
 *@file ClusterLocalSetHardwareSynchronizationStateWorker.cpp 
 * Copyright (C) 2011 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 * 
 * Description: This file has the implementation of the
 *              Worker class which is responsible for updating 
 *              local WaveNodeManagedObject's hardware sync state. 
 * Author :     Brian Adaniya
 */

#include "Cluster/Local/ClusterLocalSetHardwareSynchronizationStateWorker.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalSetHardwareSynchronizationStateMessage.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Cluster/Local/WaveNode.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

/** 
 *Name:         ClusterLocalSetHardwareSynchronizationStateWorker 
 *Description:  Constructor 
 * 
 * @param       ClusterLocalObjectManager* : Object Manager that owns this worker 
 */
ClusterLocalSetHardwareSynchronizationStateWorker::ClusterLocalSetHardwareSynchronizationStateWorker (ClusterLocalObjectManager *pClusterLocalObjectManager)
    : WaveWorker (pClusterLocalObjectManager)
{
    addOperationMap (CLUSTER_LOCAL_SET_HARDWARE_SYNCHRONIZATION_STATE, reinterpret_cast<PrismMessageHandler> (&ClusterLocalSetHardwareSynchronizationStateWorker::setHardwareSynchronizationStateMessageHandler));
}

/** 
 *Name:         ~ClusterLocalSetHardwareSynchronizationStateWorker 
 *Description:  Destructor 
 */
ClusterLocalSetHardwareSynchronizationStateWorker::~ClusterLocalSetHardwareSynchronizationStateWorker ()
{
}

PrismMessage *ClusterLocalSetHardwareSynchronizationStateWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case CLUSTER_LOCAL_SET_HARDWARE_SYNCHRONIZATION_STATE :
            pPrismMessage = new ClusterLocalSetHardwareSynchronizationStateMessage ();
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

/** 
 *Name:         setHardwareSynchronizationStateMessageHandler
 *Description:  Message handler to set the hardware synchronization state
 *              for the WaveNode Managed Object
 * 
 * @param       ClusterLocalSetHardwareSynchronizationStateMessage* : Message to be handled
 */
void ClusterLocalSetHardwareSynchronizationStateWorker::setHardwareSynchronizationStateMessageHandler (ClusterLocalSetHardwareSynchronizationStateMessage *pClusterLocalSetHardwareSynchronizationStateMessage)
{
     PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetHardwareSynchronizationStateWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetHardwareSynchronizationStateWorker::updateWaveNodeManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetHardwareSynchronizationStateWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetHardwareSynchronizationStateWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetHardwareSynchronizationStateWorker::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pClusterLocalSetHardwareSynchronizationStateMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    // serialize the handling of this message
    pPrismLinearSequencerContext->holdAll ();

    pPrismLinearSequencerContext->start ();
}

/** 
 *Name:         updateWaveNodeManagedObjectStep
 *Description:  Asynchronous linear sequencer step to update the WaveNodeMO
 * 
 * @param       PrismLinearSequencerContext* : Asynchronous linear sequencer context
 */ 
void ClusterLocalSetHardwareSynchronizationStateWorker::updateWaveNodeManagedObjectStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "ClusterLocalSetHardwareSynchronizationStateWorker::updateWaveNodeManagedObjectStep : Entering ...");

    ClusterLocalSetHardwareSynchronizationStateMessage* pClusterLocalSetHardwareSynchronizationStateMessage = dynamic_cast<ClusterLocalSetHardwareSynchronizationStateMessage* >(pPrismLinearSequencerContext->getPPrismMessage());

    if (NULL == pClusterLocalSetHardwareSynchronizationStateMessage)
    {
        trace (TRACE_LEVEL_FATAL, "ClusterLocalSetHardwareSynchronizationStateWorker::updateWaveNodeManagedObjectStep : Dynamic cast to ClusterLocalSetHardwareSynchronizationStateMessage failed.");
        prismAssert (false, __FILE__, __LINE__);
    }

    ResourceId hardwareSynchronizationState = pClusterLocalSetHardwareSynchronizationStateMessage->getHardwareSynchronizationState ();

    // Query for the WaveNode MO that we need to update

    WaveNode *pWaveNode = ClusterLocalObjectManager::getInstance ()->getThisWaveNodeManagedObject ();
    prismAssert (NULL != pWaveNode, __FILE__, __LINE__);

    updateWaveManagedObject(pWaveNode);

    pWaveNode->setHardwareSynchronizationState (hardwareSynchronizationState);

    // Clean up of queried Managed Objects are marked now and properly deleted later during sequencer's destructor

    pPrismLinearSequencerContext->addManagedObjectForGarbageCollection (pWaveNode);

    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS); 
}

}
