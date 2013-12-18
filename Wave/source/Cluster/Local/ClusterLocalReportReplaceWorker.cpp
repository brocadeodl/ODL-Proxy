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

#include "Cluster/Local/ClusterLocalReportReplaceMessage.h"
#include "Cluster/Local/ClusterLocalReportReplaceWorker.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Cluster/Local/WaveNode.h"
#include "Cluster/PrismCluster.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Cluster/Local/HeartBeat/HeartBeatMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

/// Name
/// ClusterLocalReportReplaceWorker
/// Description
/// Constructor: Registers the handler to handle the primaryNodeChange
/// notification from the framework.
/// Constructor
/// Input
/// ClusterLocalObjectManager *: Registered with the PrismElement
/// Output
/// None
/// Return
/// None

ClusterLocalReportReplaceWorker::ClusterLocalReportReplaceWorker (ClusterLocalObjectManager *pClusterLocalObjectManager)
    : WaveWorker (pClusterLocalObjectManager)
{
    addOperationMap (CLUSTER_LOCAL_REPORT_REPLACE, reinterpret_cast<PrismMessageHandler> (&ClusterLocalReportReplaceWorker::replaceMessageHandler));
}
/// Name
/// ClusterLocalReportReplaceWorker
/// Description
/// Destructor
/// Input
/// None
/// Output
/// None
/// Return
/// None

ClusterLocalReportReplaceWorker::~ClusterLocalReportReplaceWorker ()
{
}

/// Name
/// replaceMessageHandler
/// Description
/// On receiving the message from the framework it constructs the steps of processing
/// and lodges them in a context.Then it kick starts the processing
/// Input
/// ClusterLocalReportReplaceMessage *: pointer to message to received from f/w
/// Output
/// None
/// Return
/// None

void ClusterLocalReportReplaceWorker::replaceMessageHandler (ClusterLocalReportReplaceMessage *pClusterLocalReportReplaceMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalReportReplaceWorker::updateWaveNodeManagedObjectStep),

        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalReportReplaceWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterLocalReportReplaceWorker::prismLinearSequencerFailedStep)
    };

    //Note: Memory is freed inside the framework in the Succeeeded or Failure step
    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pClusterLocalReportReplaceMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0])); 

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

/// Name
/// updateWaveNodeManagedObjectStep
/// Description
/// In this step the local WaveNode MO's node status is updated 
/// and committed back to the DB
/// Input
/// PrismLinearSequencerContext *
/// Output
/// None
/// Return
/// None
void ClusterLocalReportReplaceWorker::updateWaveNodeManagedObjectStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{

    trace (TRACE_LEVEL_DEVEL, "ClusterLocalReportReplaceWorker::updateWaveNodeManagedObjectStep: Entering ...");

    //Message from the context
    ClusterLocalReportReplaceMessage* pClusterLocalReportReplaceMessage = dynamic_cast<ClusterLocalReportReplaceMessage* >(pPrismLinearSequencerContext->getPPrismMessage());

    prismAssert(NULL != pClusterLocalReportReplaceMessage, __FILE__, __LINE__);

    //Obtain the wavenode and set the ip address and port 
    startTransaction();
    WaveManagedObjectSynchronousQueryContext queryContext (WaveNode::getClassName ());

    LocationId thisNodeLocationId = pClusterLocalReportReplaceMessage->getThisNodeLocationId();
    queryContext.addAndAttribute (new AttributeLocationId (thisNodeLocationId,"locationid"));

    vector<WaveManagedObject *>              *pWaveNodeMOs = querySynchronously (&queryContext);
    prismAssert(NULL != pWaveNodeMOs, __FILE__, __LINE__);

    UI32 numberOfResults = pWaveNodeMOs->size ();
    prismAssert (1 == numberOfResults, __FILE__, __LINE__);
            
    WaveNode *pWaveNode = dynamic_cast<WaveNode *> ((*pWaveNodeMOs)[0]);
    prismAssert (NULL != pWaveNode, __FILE__, __LINE__);

    //Set new IP address and port 
    pWaveNode->setIpAddress (pClusterLocalReportReplaceMessage->getThisNodeIpAddress ());
    pWaveNode->setPort (pClusterLocalReportReplaceMessage->getThisNodePort ());

    updateWaveManagedObject(pWaveNode);
    ResourceId status = commitTransaction();

    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_WARN, "ClusterLocalReportReplaceWorker::updateWaveNodeManagedObjectStep : failed for commitTransaction for WaveNode"); 
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pWaveNodeMOs);

    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS); 

    return;
}

}
