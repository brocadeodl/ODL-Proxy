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

#include "Cluster/CentralClusterConfigDeleteClusterWorker.h"
#include "Cluster/ClusterTypes.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Cluster/Local/HeartBeat/HeartBeatMessages.h"
#include "Cluster/ClusterDeleteContext.h"
#include "Cluster/ClusterMessages.h"
#include "Cluster/PrismCluster.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Cluster/Local/WaveNode.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

namespace WaveNs
{

CentralClusterConfigDeleteClusterWorker::CentralClusterConfigDeleteClusterWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (CLUSTER_DELETE_CLUSTER, reinterpret_cast<PrismMessageHandler> (&CentralClusterConfigDeleteClusterWorker::deleteClusterMessageHandler));
}

CentralClusterConfigDeleteClusterWorker::~CentralClusterConfigDeleteClusterWorker ()
{
}

PrismMessage *CentralClusterConfigDeleteClusterWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case CLUSTER_DELETE_CLUSTER :
            pPrismMessage = new ClusterObjectManagerDeleteClusterMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void CentralClusterConfigDeleteClusterWorker::deleteClusterMessageHandler (ClusterObjectManagerDeleteClusterMessage *pClusterObjectManagerDeleteClusterMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteClusterWorker::deleteClusterValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteClusterWorker::deleteClusterStopHeartBeatsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteClusterWorker::deleteClusterCommitStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteClusterWorker::deleteClusterRequestFrameworkToDeleteClusterStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteClusterWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteClusterWorker::prismLinearSequencerFailedStep)
    };

    ClusterDeleteContext *pClusterDeleteContext = new ClusterDeleteContext (pClusterObjectManagerDeleteClusterMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pClusterDeleteContext->holdAll ();
    pClusterDeleteContext->start ();
}

void CentralClusterConfigDeleteClusterWorker::deleteClusterValidateStep (ClusterDeleteContext *pClusterDeleteContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigDeleteClusterWorker::deleteClusterValidateStep : Entering ...");

    // Check if cluster is already created and reject if it is not already created.

    vector<WaveManagedObject *> *pResults = querySynchronously (PrismCluster::getClassName ());

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    if (NULL == pResults)
    {
        pClusterDeleteContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    UI32       numberOfResults = pResults->size ();
    ResourceId status          = WAVE_MESSAGE_SUCCESS;

    if (0 == numberOfResults)
    {
        trace (TRACE_LEVEL_WARN, "CentralClusterConfigDeleteClusterWorker::deleteClusterValidateStep : There is no cluster created.");

        status = WAVE_CLUSTER_DELETE_STATUS_CLUSTER_NOT_FOUND;
    }
    else if (1 < numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, "CentralClusterConfigDeleteClusterWorker::deleteClusterValidateStep : Multiple cluster objects found.");
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        pClusterDeleteContext->setPPrismCluster (dynamic_cast<PrismCluster *> ((*pResults)[0]));
    }

    pResults->clear ();
    delete pResults;

    pClusterDeleteContext->executeNextStep (status);
}

void CentralClusterConfigDeleteClusterWorker::deleteClusterStopHeartBeatsStep (ClusterDeleteContext *pClusterDeleteContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigDeleteClusterWorker::deleteClusterStopHeartBeatsStep : Entering ...");

    PrismCluster                                *pPrismCluster          = pClusterDeleteContext->getPPrismCluster ();
    vector<ObjectId>                             secondaryNodes;
    UI32                                         numberOfSecondaryNodes = 0;
    UI32                                         i                      = 0;
    string                                       secondaryNodeName;
    UI32                                         secondaryNodePort;
    WaveNode                                    *pSecondaryNode         = NULL;
    ResourceId                                   status                 = WAVE_MESSAGE_SUCCESS;
    vector<WaveManagedObject *>                 *pSecondaryNodeResults  = NULL;
    UI32                                         numberOfResults        = 0;
    ClusterObjectManagerDeleteClusterMessage    *pMessage               = reinterpret_cast<ClusterObjectManagerDeleteClusterMessage *> (pClusterDeleteContext->getPPrismMessage ());
    WaveNode                                     tempNode               (getPWaveObjectManager (), FrameworkToolKit::getThisLocationId (), FrameworkToolKit::getThisLocationIpAddress (), FrameworkToolKit::getThisLocationPort ());

    prismAssert (NULL != pPrismCluster, __FILE__, __LINE__);

    secondaryNodes         = pPrismCluster->getSecondaryNodes ();
    numberOfSecondaryNodes = secondaryNodes.size ();

    if (numberOfSecondaryNodes)
    {
        pSecondaryNodeResults = querySynchronously (WaveNode::getClassName (), secondaryNodes);
        prismAssert (NULL != pSecondaryNodeResults, __FILE__, __LINE__);

        numberOfResults = pSecondaryNodeResults->size ();
        prismAssert (numberOfSecondaryNodes == numberOfResults, __FILE__, __LINE__);
    }
    else     
    {
        trace (TRACE_LEVEL_INFO, "CentralClusterConfigDeleteClusterWorker::deleteClusterStopHeartBeatsStep: There is no Seoncdary Node Present");     
    }

    pClusterDeleteContext->setPSecondaryNodes (pSecondaryNodeResults);

    // Attempt to stop all heart beats to secondary.  Even if we fail stopping heart beats we will simply proceed.

    for (i = 0; i < numberOfSecondaryNodes; i++)
    {
        pSecondaryNode = dynamic_cast<WaveNode *> ((*pSecondaryNodeResults)[i]);

        prismAssert (NULL != pSecondaryNode, __FILE__, __LINE__);

        if ((*pSecondaryNode) != tempNode)
        {
            secondaryNodeName = pSecondaryNode->getIpAddress ();
            secondaryNodePort = pSecondaryNode->getPort ();

            StopHeartBeatMessage message;

            message.setDstIpAddress (secondaryNodeName);
            message.setDstPortNumber (secondaryNodePort);

            status = sendSynchronously (&message);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "CentralClusterConfigDeleteClusterWorker::deleteClusterStopHeartBeatsStep : Failed to Stop heart beat to the Secondary Node.  Node Name : " + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Status : ") + FrameworkToolKit::localize (status));
            }
            else
            {
                status = message.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR, "CentralClusterConfigDeleteClusterWorker::deleteClusterStopHeartBeatsStep : Failed to Stop heart beat to the Secondary Node.  Node Name : " + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Completion Status : ") + FrameworkToolKit::localize (status));
                }
                else
                {
                    trace (TRACE_LEVEL_INFO, "CentralClusterConfigDeleteClusterWorker::deleteClusterStopHeartBeatsStep : Stopped Heart Beat with " + secondaryNodeName + string (":") + secondaryNodePort);
                }
            }

            if (false == pMessage->getIsRebootRequired ())
            {
                (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (secondaryNodeName, (SI32) secondaryNodePort);
            }
        }
    }

    pClusterDeleteContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void CentralClusterConfigDeleteClusterWorker::deleteClusterCommitStep (ClusterDeleteContext *pClusterDeleteContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigDeleteClusterWorker::deleteClusterCommitStep : Entering ...");

    PrismCluster                 *pPrismCluster          = pClusterDeleteContext->getPPrismCluster ();
    ResourceId                    status                 = WAVE_MESSAGE_SUCCESS;

    startTransaction ();

    delete pPrismCluster;
    pClusterDeleteContext->setPPrismCluster (NULL);

    //Also update the status of local wavenode

  //Update local waveNode
    WaveManagedObjectSynchronousQueryContext queryContextForPrimarywaveNode (WaveNode::getClassName ());
    string  primaryIpAddress = FrameworkToolKit::getThisLocationIpAddress ();
    UI32    primaryPort      = FrameworkToolKit::getThisLocationPort ();

    queryContextForPrimarywaveNode.addAndAttribute (new AttributeString (primaryIpAddress,     "ipAddress"));
    queryContextForPrimarywaveNode.addAndAttribute (new AttributeUI32   (primaryPort, "port"));

    vector<WaveManagedObject *>* pResultsForPrimaryWavenode = querySynchronously (&queryContextForPrimarywaveNode);
    prismAssert (NULL != pResultsForPrimaryWavenode, __FILE__, __LINE__);
    prismAssert (1 == pResultsForPrimaryWavenode->size(), __FILE__, __LINE__);

    WaveNode *pPrimaryWaveNode = dynamic_cast<WaveNode *> ((*pResultsForPrimaryWavenode)[0]);
    prismAssert( NULL != pPrimaryWaveNode, __FILE__, __LINE__);

    pPrimaryWaveNode->setGenericStatus(WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD);
    pPrimaryWaveNode->setSpecificStatus(WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_STAND_ALONE);
    updateWaveManagedObject(pPrimaryWaveNode);

    status = commitTransaction ();
    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "CentralClusterConfigDeleteClusterWorker::deleteClusterCommitStep : Could not commit to repository.  status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }
    
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsForPrimaryWavenode);
    pClusterDeleteContext->executeNextStep (status);
}

void CentralClusterConfigDeleteClusterWorker::deleteClusterRequestFrameworkToDeleteClusterStep (ClusterDeleteContext *pClusterDeleteContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigDeleteClusterWorker::deleteClusterRequestFrameworkToDeleteClusterStep : Entering ...");

    ClusterObjectManagerDeleteClusterMessage    *pClusterObjectManagerDeleteClusterMessage    = reinterpret_cast<ClusterObjectManagerDeleteClusterMessage *> (pClusterDeleteContext->getPPrismMessage ());
    PrismServiceId                               serderServiceId                              = pClusterObjectManagerDeleteClusterMessage->getSenderServiceCode ();

    FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage = new FrameworkObjectManagerDestroyClusterMessage ();
    ResourceId                                   status                                       = WAVE_MESSAGE_SUCCESS;

    pFrameworkObjectManagerDestroyClusterMessage->setOriginalRequester (serderServiceId);
    pFrameworkObjectManagerDestroyClusterMessage->setIsRebootRequired (pClusterObjectManagerDeleteClusterMessage->getIsRebootRequired ());

    status = sendOneWay (pFrameworkObjectManagerDestroyClusterMessage);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigDeleteClusterWorker::deleteClusterRequestFrameworkToDeleteClusterStep : Error in requesting framework to delete cluster.  status : " + FrameworkToolKit::localize (status));

        pClusterDeleteContext->executeNextStep (status);
        return;
    }
    pClusterDeleteContext->executeNextStep (status);
}

void CentralClusterConfigDeleteClusterWorker::deleteClusterRequestFrameworkToDeleteClusterCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigDeleteClusterWorker::deleteClusterRequestFrameworkToDeleteClusterCallback : Entering ...");

    ClusterDeleteContext *pClusterDeleteContext = reinterpret_cast<ClusterDeleteContext *> (pContext);
    ResourceId            status                = WAVE_MESSAGE_SUCCESS;

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        status = frameworkStatus;

        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigDeleteClusterWorker::deleteClusterRequestFrameworkToDeleteClusterCallback : Error in requesting framework to delete cluster.  status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = pFrameworkObjectManagerDestroyClusterMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "CentralClusterConfigDeleteClusterWorker::deleteClusterRequestFrameworkToDeleteClusterCallback : Error in requesting framework to delete cluster.  Completion Status : " + FrameworkToolKit::localize (status));
        }
    }

    delete pFrameworkObjectManagerDestroyClusterMessage;

    pClusterDeleteContext->executeNextStep (status);
    return;
}

}
