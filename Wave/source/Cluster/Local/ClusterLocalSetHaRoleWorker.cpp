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
#include "Cluster/Local/ClusterLocalSetHaRoleMessage.h"
#include "Cluster/Local/ClusterLocalSetHaRoleWorker.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include  "Framework/Types/Types.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Cluster/Local/WaveNode.h"
#include "Cluster/Local/WaveHaNode.h"
#include "Cluster/PrismCluster.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace WaveNs
{

/// Name
/// ClusterLocalSetHaRoleWorker
/// Description
/// Constructor: Registers the handler to handle the HA role notofication message 
/// notification from the framework.
/// Constructor
/// Input
/// ClusterLocalObjectManager *: Registered with the PrismElement
/// Output
/// None
/// Return
/// None

ClusterLocalSetHaRoleWorker::ClusterLocalSetHaRoleWorker (ClusterLocalObjectManager *pClusterLocalObjectManager)
    : WaveWorker (pClusterLocalObjectManager)
{
    addOperationMap (CLUSTER_LOCAL_SET_HA_ROLE, reinterpret_cast<PrismMessageHandler> (&ClusterLocalSetHaRoleWorker::setHaRoleMessageHandler));
}
/// Name
/// ClusterLocalSetHaRoleWorker
/// Description
/// Destructor
/// Input
/// None
/// Output
/// None
/// Return
/// None

ClusterLocalSetHaRoleWorker::~ClusterLocalSetHaRoleWorker ()
{
}

/// Name
/// ClusterLocalSetHaRoleWorker
/// Description
/// On receiving the message from the framework it constructs the steps of processing
/// and lodges them in a context.Then it kick starts the processing
/// Input
/// ClusterLocalSetHaRoleMessage *: pointer to message to received from f/w
/// Output
/// None
/// Return
/// None

void ClusterLocalSetHaRoleWorker::setHaRoleMessageHandler (ClusterLocalSetHaRoleMessage *pClusterLocalSetHaRoleMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetHaRoleWorker::updateWaveHaNodeManagedObjectStep),

        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetHaRoleWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalSetHaRoleWorker::prismLinearSequencerFailedStep)
    };

    //Note: Memory is freed inside the framework in the Succeeeded or Failure step
    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pClusterLocalSetHaRoleMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0])); 
    tracePrintf(TRACE_LEVEL_INFO, "Number of Steps = %d",sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

/// Name
/// updateWaveHaNodeManagedObjectStep
/// Description
/// In this step the local WaveNode MO's node status is updated 
/// and committed back to the DB
/// Input
/// PrismLinearSequencerContext *
/// Output
/// None
/// Return
/// None
void ClusterLocalSetHaRoleWorker::updateWaveHaNodeManagedObjectStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{

    trace (TRACE_LEVEL_INFO, "ClusterLocalSetHaRoleWorker::updateWaveHaNodeManagedObjectStep: Entering ...");

#if 0
    //Message from the context
    ClusterLocalSetHaRoleMessage* pClusterLocalSetHaRoleMessage = dynamic_cast<ClusterLocalSetHaRoleMessage* >(pPrismLinearSequencerContext->getPPrismMessage());

    prismAssert(NULL != pClusterLocalSetHaRoleMessage, __FILE__, __LINE__);

    //Obtain the wavenode and update the status and then committ
    WaveManagedObjectSynchronousQueryContext queryContext (WaveHaNode::getClassName ());

	WaveHaNodeRole  haRole    =    (WaveHaNodeRole)(pClusterLocalSetHaRoleMessage->getHaRole());

    vector<WaveManagedObject *>              *pWaveHaNodeMOs = querySynchronously (&queryContext);
    prismAssert(NULL != pWaveHaNodeMOs, __FILE__, __LINE__);

    UI32 numberOfResults = pWaveHaNodeMOs->size ();
    prismAssert (1 == numberOfResults, __FILE__, __LINE__);

	startTransaction();
    WaveHaNode *pWaveHaNode = dynamic_cast<WaveHaNode *> ((*pWaveHaNodeMOs)[0]);
    //Set HaRole 
    pWaveHaNode->setHaRole(haRole);
    updateWaveManagedObject(pWaveHaNode);
    commitTransaction();

	trace (TRACE_LEVEL_INFO, string("ClusterLocalSetHaRoleWorker:set haNode to ") + haRole);
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pWaveHaNodeMOs);
#endif

    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS); 

    return;
}

}
