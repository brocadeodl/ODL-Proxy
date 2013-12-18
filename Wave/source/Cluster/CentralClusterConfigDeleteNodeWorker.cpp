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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Cluster/CentralClusterConfigDeleteNodeWorker.h"
#include "Cluster/CentralClusterConfigObjectManager.h"
#include "Cluster/ClusterMessages.h"
#include "Cluster/ClusterTypes.h"
#include "Cluster/Local/WaveNode.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Cluster/PrismCluster.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

CentralClusterConfigDeleteNodeWorker::CentralClusterConfigDeleteNodeWorker (CentralClusterConfigObjectManager *pCentralClusterConfigObjectManager)
    : WaveWorker (pCentralClusterConfigObjectManager)
{
    addOperationMap (CLUSTER_DELETE_NODE, reinterpret_cast<PrismMessageHandler> (&CentralClusterConfigDeleteNodeWorker::deleteNodeMessageHandler));
}

CentralClusterConfigDeleteNodeWorker::~CentralClusterConfigDeleteNodeWorker ()
{
}

PrismMessage *CentralClusterConfigDeleteNodeWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case CLUSTER_DELETE_NODE :
            pPrismMessage = new ClusterObjectManagerDeleteNodeMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void CentralClusterConfigDeleteNodeWorker::deleteNodeMessageHandler (ClusterObjectManagerDeleteNodeMessage *pClusterObjectManagerDeleteNodeMessage)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigAddNodeWorker::deleteNodeMessageHandler : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext;

    // In case of deletion of a disconnected node, we do not need to explicitly stop HB as it would have been
    // done during HB failure.    
    if (true == pClusterObjectManagerDeleteNodeMessage->getIsDisconnected())
    {
        PrismLinearSequencerStep sequencerForDisconnectedSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteNodeWorker::deleteNodeValidateStep),
            reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteNodeWorker::deleteNodeRequestFrameworkToDeleteNodeStep),
            reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteNodeWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteNodeWorker::prismLinearSequencerFailedStep)
        };

        pPrismLinearSequencerContext = new PrismLinearSequencerContext (pClusterObjectManagerDeleteNodeMessage, this, sequencerForDisconnectedSteps, sizeof (sequencerForDisconnectedSteps) / sizeof (sequencerForDisconnectedSteps[0]));
    }
    else  
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteNodeWorker::deleteNodeValidateStep),
            reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteNodeWorker::deleteNodeStopHeartBeatsStep),
            reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteNodeWorker::deleteNodeRequestFrameworkToDeleteNodeStep),
            reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteNodeWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigDeleteNodeWorker::prismLinearSequencerFailedStep)
        };

        pPrismLinearSequencerContext = new PrismLinearSequencerContext (pClusterObjectManagerDeleteNodeMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    }
    //  pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();

}

void CentralClusterConfigDeleteNodeWorker::deleteNodeValidateStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "CentralClusterConfigdeleteNodeWorker::deleteNodeValidateStep : starting ...");

    ClusterObjectManagerDeleteNodeMessage *pClusterObjectManagerDeleteNodeMessage = reinterpret_cast<ClusterObjectManagerDeleteNodeMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    string                               nodeName;
    SI32                                 nodePort;
    UI32                                 i, j;
    vector<WaveManagedObject *>         *pResults = NULL;
    WaveNode                            *pNode    = NULL;
    UI32                                 noOfSecondaryNodes;
    UI32                                 validSecondaryNode = 0;
    ResourceId                           status   = WAVE_MESSAGE_SUCCESS;
    UI32                                 noOfNode = pClusterObjectManagerDeleteNodeMessage->getNNodesToDelete ();
    vector<string>                       NodeNames;
    vector<UI32>                         NodePorts;
    UI32                                 NodeSize;
    string                               primaryNodeName = FrameworkToolKit::getClusterPrimaryIpAddress ();
    SI32                                 primaryNodePort = FrameworkToolKit::getClusterPrimaryPort ();

    //Check if node is a part of cluster 

    pResults = querySynchronously (WaveNode::getClassName ());

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    noOfSecondaryNodes = pResults->size ();

    for (j = 0; j < noOfNode; j++)
    {
        nodeName = pClusterObjectManagerDeleteNodeMessage->getNodeName (j);
        nodePort = pClusterObjectManagerDeleteNodeMessage->getNodePort (j);

        NodeNames.push_back (nodeName);
        NodePorts.push_back (nodePort);

        for (i = 0; i < noOfSecondaryNodes; i++)
        {
            pNode = dynamic_cast<WaveNode *> ((*pResults)[i]);

            prismAssert (NULL != pNode, __FILE__, __LINE__);
        
            if (((pNode->getIpAddress ()) ==  nodeName) && ((pNode->getPort ()) ==  (UI32) nodePort))
            {
                validSecondaryNode = 1;
                break;
            }
        }
        
        if ((nodeName == primaryNodeName) && (nodePort == primaryNodePort))
        {
            trace (TRACE_LEVEL_ERROR, "CentralClusterConfigAddNodeWorker::deleteNodeValidateStep: Can Node Delete Primary Node");
            validSecondaryNode = 0;
            break;
        }
        //check if deleting any node twice

        NodeSize = NodeNames.size ();

        for (i = 0; i < NodeSize - 1 ; i++)
        {
            if ((nodeName == NodeNames[i]) && ((UI32) nodePort == NodePorts[i]))
            {
                trace (TRACE_LEVEL_ERROR, "CentralClusterConfigAddNodeWorker::deleteNodeValidateStep: please check for Duplicate Node in delete nodes");
                validSecondaryNode = 0;
                break;
            }
    
        }

        // In case of deletion of disconnected node, check for disconnected status.
        if (true == pClusterObjectManagerDeleteNodeMessage->getIsDisconnected())
        {
            if (WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_SECONDARY_NOT_CONNECTED_TO_CLUSTER != pNode->getSpecificStatus())
            {
                trace (TRACE_LEVEL_ERROR, "CentralClusterConfigAddNodeWorker::deleteNodeValidateStep: Not a disconnected node");
                validSecondaryNode = 0;
                break; 
            }   
        }
    }

    if (!validSecondaryNode) 
    {
       trace (TRACE_LEVEL_ERROR, "CentralClusterConfigAddNodeWorker::deleteNodeValidateStep: Invalid Node Name/ port" + nodeName + string(" ")+nodePort);
       status = WAVE_MESSAGE_ERROR;
    }

    for (i = 0; i < noOfSecondaryNodes; i++)
    {
        delete (*pResults)[i];
    }

    pResults->clear ();
    delete pResults;

    pPrismLinearSequencerContext->executeNextStep (status);
}

void CentralClusterConfigDeleteNodeWorker::deleteNodeStopHeartBeatsStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "CentralClusterConfigDeleteNodeWorker::deleteNodeStopHeartBeatStep : starting ...");

    ClusterObjectManagerDeleteNodeMessage *pClusterObjectManagerDeleteNodeMessage = reinterpret_cast<ClusterObjectManagerDeleteNodeMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    string                               nodeName;
    UI32                                 nodePort;
    ResourceId                           status   = WAVE_MESSAGE_SUCCESS;
    StopHeartBeatMessage                 message;
    UI32                                 noOfNode = pClusterObjectManagerDeleteNodeMessage->getNNodesToDelete ();
    UI32                                 i;

    for (i = 0; i < noOfNode; i++)
    {
        nodeName = pClusterObjectManagerDeleteNodeMessage->getNodeName (i);
        nodePort = pClusterObjectManagerDeleteNodeMessage->getNodePort (i);
    
        message.setDstIpAddress (nodeName);
        message.setDstPortNumber (nodePort);

        status = sendSynchronously (&message);
    
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "CentralClusterConfigDeleteNodeWorker::deleteNodeStopHeartBeatsStep : Failed to Stop heart beat to the Secondary Node.  Node Name : " + nodeName + string (", Port : ") + nodePort + string (", Status : ") + FrameworkToolKit::localize (status));
        }
        else
        {
            status = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "CentralClusterConfigDeleteNodeWorker::deleteNodeStopHeartBeatsStep : Failed to Stop heart beat to the Secondary Node.  Node Name : " + nodeName + string (", Port : ") + nodePort + string (", Completion Status : ") + FrameworkToolKit::localize (status));
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "CentralClusterConfigDeleteNodeWorker::deleteNodeStopHeartBeatsStep : Stopped Heart Beat with " + nodeName + string (":") + nodePort);
            }
        }
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);

}

void CentralClusterConfigDeleteNodeWorker::deleteNodeRequestFrameworkToDeleteNodeStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "CentralClusterConfigDeleteClusterWorker::deleteNodeRequestFrameworkToDeleteNodeStep : Entering ...");

        ClusterObjectManagerDeleteNodeMessage               *pClusterObjectManagerDeleteNodeMessage               = reinterpret_cast<ClusterObjectManagerDeleteNodeMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
        FrameworkObjectManagerDeleteNodesFromClusterMessage *pFrameworkObjectManagerDeleteNodesFromClusterMessage = new FrameworkObjectManagerDeleteNodesFromClusterMessage ();
        ResourceId                                           status                                               = WAVE_MESSAGE_SUCCESS;
        UI32                                                 i                                                    = 0;
        UI32                                                 noNodes                                              = pClusterObjectManagerDeleteNodeMessage->getNNodesToDelete ();
        string                                               nodeName;
        SI32                                                 nodePort;


        for (i = 0; i < noNodes; i++)
        {
            nodeName = pClusterObjectManagerDeleteNodeMessage->getNodeName (i);
            nodePort = pClusterObjectManagerDeleteNodeMessage->getNodePort (i);

            pFrameworkObjectManagerDeleteNodesFromClusterMessage->addNodeIpAddressAndPort (nodeName, nodePort);

        }

        // Set flag to indicate deletion of a disconnected node.  
        if (true == pClusterObjectManagerDeleteNodeMessage->getIsDisconnected()) 
        {
            pFrameworkObjectManagerDeleteNodesFromClusterMessage->setIsDisconnected(true);
        }

        status = send (pFrameworkObjectManagerDeleteNodesFromClusterMessage, reinterpret_cast<PrismMessageResponseHandler> (&CentralClusterConfigDeleteNodeWorker::deleteNodeRequestFrameworkToDeleteNodeCallback), pPrismLinearSequencerContext);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "CentralClusterConfigDeleteNodeWorker::deleteNodeRequestFrameworkToDeleteNodeStep : Error in requesting framework to delete cluster.  status : " + FrameworkToolKit::localize (status));

            pPrismLinearSequencerContext->executeNextStep (status);         
        
            return;
        } else {
            trace (TRACE_LEVEL_INFO, "CentralClusterConfigDeleteClusterWorker::deleteNodeRequestFrameworkToDeleteNodeStep : Sent ...");
        }

//    pPrismLinearSequencerContext->executeNextStep (status);  
}

void CentralClusterConfigDeleteNodeWorker::deleteNodeRequestFrameworkToDeleteNodeCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerDeleteNodesFromClusterMessage *pFrameworkObjectManagerDeleteNodesFromClusterMessage, void *pContext)
{
    trace (TRACE_LEVEL_INFO, "CentralClusterConfigDeleteNodeWorker::deleteNodeRequestFrameworkToDeleteNodeCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext  = reinterpret_cast<PrismLinearSequencerContext *> (pContext);
    ClusterObjectManagerDeleteNodeMessage *pClusterObjectManagerDeleteNodeMessage = reinterpret_cast<ClusterObjectManagerDeleteNodeMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    ResourceId                                status                                    = WAVE_MESSAGE_SUCCESS;
    string                                               nodeName;
    SI32                                                 nodePort;
    UI32                                                 noNodes = pClusterObjectManagerDeleteNodeMessage->getNNodesToDelete ();
    UI32 i;

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        status = frameworkStatus;

        for (i = 0; i < noNodes; i++)
        {
            nodeName = pClusterObjectManagerDeleteNodeMessage->getNodeName (i);
            nodePort = pClusterObjectManagerDeleteNodeMessage->getNodePort (i);

            pClusterObjectManagerDeleteNodeMessage->setNodeStatus (nodeName, nodePort, status);
        }

        if(WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE == status) {
            status = WAVE_MESSAGE_SUCCESS;
        }
    }
    else
    {
        for (i = 0; i < noNodes; i++)
        {
            nodeName = pClusterObjectManagerDeleteNodeMessage->getNodeName (i);
            nodePort = pClusterObjectManagerDeleteNodeMessage->getNodePort (i);

            status = pFrameworkObjectManagerDeleteNodesFromClusterMessage->getNodeStatus (nodeName, nodePort);

            pClusterObjectManagerDeleteNodeMessage->setNodeStatus (nodeName, nodePort, status);
        }
    }

    delete (pFrameworkObjectManagerDeleteNodesFromClusterMessage);

    pPrismLinearSequencerContext->executeNextStep (status);
}

}

