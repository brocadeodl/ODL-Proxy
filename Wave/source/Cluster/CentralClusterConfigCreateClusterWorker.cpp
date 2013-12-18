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

#include "Cluster/CentralClusterConfigCreateClusterWorker.h"
#include "Cluster/ClusterTypes.h"
#include "Cluster/ClusterMessages.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Cluster/Local/WaveNode.h"
#include "Cluster/PrismCluster.h"
#include "Cluster/Local/HeartBeat/HeartBeatMessages.h"
#include "Cluster/Local/ClusterLocalSetThisNodeIpAddressMessage.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

namespace WaveNs
{

CentralClusterConfigCreateClusterWorker::CentralClusterConfigCreateClusterWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (CLUSTER_CREATE_CLUSTER, reinterpret_cast<PrismMessageHandler> (&CentralClusterConfigCreateClusterWorker::createClusterMessageHandler));
}

CentralClusterConfigCreateClusterWorker::~CentralClusterConfigCreateClusterWorker ()
{
}

PrismMessage *CentralClusterConfigCreateClusterWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case CLUSTER_CREATE_CLUSTER :
            pPrismMessage = new ClusterObjectManagerCreateClusterMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void CentralClusterConfigCreateClusterWorker::createClusterMessageHandler (ClusterObjectManagerCreateClusterMessage *pClusterObjectManagerCreateClusterMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigCreateClusterWorker::createClusterResetThisNodeIpAddressIfRequiredStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigCreateClusterWorker::createClusterValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigCreateClusterWorker::createClusterRequestFrameworkToCreateClusterStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigCreateClusterWorker::createClusterCommitStep),
//        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigCreateClusterWorker::createClusterStartHeartBeatsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigCreateClusterWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigCreateClusterWorker::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pClusterObjectManagerCreateClusterMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

//    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void CentralClusterConfigCreateClusterWorker::createClusterValidateStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigCreateClusterWorker::createClusterValidateStep : starting ...");

    ClusterObjectManagerCreateClusterMessage *pClusterObjectManagerCreateClusterMessage = reinterpret_cast<ClusterObjectManagerCreateClusterMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    UI32                                      i                                         = 0;
    string                                    nodeName;
    UI32                                      nodePort;
    UI32                                      numberOfNewSecondaryNodes                 = pClusterObjectManagerCreateClusterMessage->getNSecondaryNodes ();

    // Set the status for the creation of all the nodes in the clusters as failed initiallly

    for (i = 0; i < numberOfNewSecondaryNodes; i++)
    {
        nodeName = pClusterObjectManagerCreateClusterMessage->getSecondaryNodeName (i);
        nodePort = pClusterObjectManagerCreateClusterMessage->getSecondaryNodePort (i);

        pClusterObjectManagerCreateClusterMessage->setNodeStaus (nodeName, nodePort, WAVE_MESSAGE_ERROR);
    }

    // Check if cluster is already created and reject if it is already created.

    vector<WaveManagedObject *> *pResults = querySynchronously (PrismCluster::getClassName ());

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    if (NULL == pResults)
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    UI32 numberOfResults = pResults->size ();

    // Delete the results and the vector.  We are done with it.

    for (i = 0; i < numberOfResults; i++)
    {
        delete (*pResults)[i];
    }

    pResults->clear ();
    delete pResults;

    if (1 < numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, string ("CentralClusterConfigCreateClusterWorker::createClusterValidateStep : There can only be one cluster in the system.  Some thing went wrong.  We obtained ") + numberOfResults + string (" of clusters"));
        prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    if (1 == numberOfResults)
    {
        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigCreateClusterWorker::createClusterValidateStep : Cluster is already created for this Node.");

        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    // validate the input that is passed to us.

    // First query this node and get details for it.  Since there is no cluster created at this point, there should only be one node in the system.  That is self node.

    pResults = querySynchronously (WaveNode::getClassName ());

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32 numberOfNodes = pResults->size ();

    if (1 != numberOfNodes)
    {
        trace (TRACE_LEVEL_FATAL, string ("CentralClusterConfigCreateClusterWorker::createClusterValidateStep : When there is no cluster created, there shall be only the self node in the system configuration.  We obtained ") + numberOfNodes + string (" nodes in the system."));
        prismAssert (false, __FILE__, __LINE__);

        // Delete all of the nodes before erroring out.

        for (i = 0; i < numberOfNodes; i++)
        {
            delete (*pResults)[i];
        }

        pResults->clear ();
        delete pResults;

        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    // Since we got only node in the system proceed with it and validate the input.

    WaveNode *pThisNode = dynamic_cast<WaveNode *> ((*pResults)[0]);

    prismAssert (NULL != pThisNode, __FILE__, __LINE__);

    trace (TRACE_LEVEL_INFO, "CentralClusterConfigCreateClusterWorker::createClusterValidateStep : Current Node Name : " + pThisNode->getIpAddress () + string (" Current Node Port : ") + pThisNode->getPort ());

    prismAssert (NULL != pThisNode, __FILE__, __LINE__);

    for (i = 0; i < numberOfNewSecondaryNodes; i++)
    {
        nodeName = pClusterObjectManagerCreateClusterMessage->getSecondaryNodeName (i);
        nodePort = pClusterObjectManagerCreateClusterMessage->getSecondaryNodePort (i);

        if ((((pThisNode->getIpAddress ()) ==  nodeName) && ((pThisNode->getPort ()) ==  nodePort)) ||
             ("127.0.0.1" == nodeName) ||
             ("" == nodeName) ||
             (1024 > nodePort))
        {
            // invalid secondary name/port.

            trace (TRACE_LEVEL_ERROR, "CentralClusterConfigCreateClusterWorker::createClusterValidateStep : Invalid Node Name/Port.  Node Name : " + nodeName + string (", Port : ") + nodePort);

            pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR);
            return;
        }
    }

    // Delete all of the nodes before proceeding, we do not ned them any more.

    for (i = 0; i < numberOfNodes; i++)
    {
        delete (*pResults)[i];
    }

    pResults->clear ();
    delete pResults;

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void CentralClusterConfigCreateClusterWorker::createClusterResetThisNodeIpAddressIfRequiredStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigCreateClusterWorker::createClusterResetThisNodeIpAddressIfRequiredStep : Entering ...");

    string     ethernetInterface = PrismFrameworkObjectManager::getEthernetInterfaceForThisLocation ();
    ResourceId status            = WAVE_MESSAGE_SUCCESS;
    string     ipAddress;

    if ("" == ethernetInterface)
    {
        trace (TRACE_LEVEL_INFO, "CentralClusterConfigCreateClusterWorker::createClusterResetThisNodeIpAddressIfRequiredStep : No specific Ethernet Interface Assigned and hence no IP Address resetting is required.");
    }
    else
    {
        ipAddress = FrameworkToolKit::getIpAddressForInterface (ethernetInterface);

        if ("" == ipAddress)
        {
            trace (TRACE_LEVEL_ERROR, "CentralClusterConfigCreateClusterWorker::createClusterResetThisNodeIpAddressIfRequiredStep : No IP Address assigned to user assigned Ethernet Interface \"" + ethernetInterface + "\"");

            status = WAVE_CLUSTER_CREATE_STATUS_NO_IP_ADDRESS_ASSIGNED_TO_ETHERNET_INTERFACE;
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "CentralClusterConfigCreateClusterWorker::createClusterResetThisNodeIpAddressIfRequiredStep : Resetting IP Address to :\"" + ipAddress + "\" for this node.");

            ClusterLocalSetThisNodeIpAddressMessage message         (ipAddress);
            ResourceId                              messageStatus = WAVE_MESSAGE_ERROR;

            messageStatus = sendSynchronously (&message, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != messageStatus)
            {
                trace (TRACE_LEVEL_ERROR, "CentralClusterConfigCreateClusterWorker::createClusterResetThisNodeIpAddressIfRequiredStep : Error in sending message to set this node IP Address : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");

                status = messageStatus;
            }
            else
            {
                messageStatus = message.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != messageStatus)
                {
                    trace (TRACE_LEVEL_ERROR, "CentralClusterConfigCreateClusterWorker::createClusterResetThisNodeIpAddressIfRequiredStep : Failed to set this node IP Address : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");

                    status = messageStatus;
                }
                else
                {
                    status = WAVE_MESSAGE_SUCCESS;
                }
            }
        }
    }

    // NOTICE: The following code will go away

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        vector<WaveManagedObject *> *pResults = querySynchronously (NodeManagedObject::getClassName ());

        prismAssert (NULL != pResults, __FILE__, __LINE__);

        UI32 numberOfNodes = pResults->size ();

        prismAssert (1 == numberOfNodes, __FILE__, __LINE__);

        NodeManagedObject *pNodeManagedObject = dynamic_cast<NodeManagedObject *> ((*pResults)[0]);
        prismAssert (NULL != pNodeManagedObject, __FILE__, __LINE__);

        startTransaction ();

        updateWaveManagedObject (pNodeManagedObject);

        pNodeManagedObject->setNodeName (ipAddress);

        ResourceId commitStatus = commitTransaction ();
        if (FRAMEWORK_SUCCESS != commitStatus)
        {
            trace (TRACE_LEVEL_ERROR, "CentralClusterConfigCreateClusterWorker::createClusterResetThisNodeIpAddressIfRequiredStep : Failed for commitTransaction for NodeManagedObject : Status : \"" + FrameworkToolKit::localize (commitStatus) + "\"");    
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void CentralClusterConfigCreateClusterWorker::createClusterRequestFrameworkToCreateClusterStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    ClusterObjectManagerCreateClusterMessage *pClusterObjectManagerCreateClusterMessage = reinterpret_cast<ClusterObjectManagerCreateClusterMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    UI32                                      i                                         = 0;
    string                                    secondaryNode;
    UI32                                      secondaryNodePort;
    UI32                                      numberOfNewSecondaryNodes                 = pClusterObjectManagerCreateClusterMessage->getNSecondaryNodes ();
    PrismCreateClusterWithNodesMessage       *pPrismCreateClusterWithNodesMessage       = new PrismCreateClusterWithNodesMessage ();
    ResourceId                                status                                    = WAVE_MESSAGE_SUCCESS;

    /* Inform framework that we got a request to create a cluster. */

    for (i = 0; i < numberOfNewSecondaryNodes; i++)
    {
        // Provide a list of all the secondary nodes in this cluster.

        secondaryNode     = pClusterObjectManagerCreateClusterMessage->getSecondaryNodeName (i);
        secondaryNodePort = pClusterObjectManagerCreateClusterMessage->getSecondaryNodePort (i);

        pPrismCreateClusterWithNodesMessage->addNewNodeIpAddressAndPort (secondaryNode, secondaryNodePort);
    }
    
    pPrismCreateClusterWithNodesMessage->setIsAddNodeForSpecialCaseFlag (pClusterObjectManagerCreateClusterMessage->getIsAddNodeForSpecialCaseFlag ());

    status = send (pPrismCreateClusterWithNodesMessage, reinterpret_cast<PrismMessageResponseHandler> (&CentralClusterConfigCreateClusterWorker::createClusterRequestFrameworkToCreateClusterCallback), pPrismLinearSequencerContext);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigCreateClusterWorker::createClusterRequestFrameworkToCreateClusterStep : Could not request Framework For creating cluster.  Status " + FrameworkToolKit::localize (status));

        pPrismLinearSequencerContext->executeNextStep (status);
    }
}

void CentralClusterConfigCreateClusterWorker::createClusterRequestFrameworkToCreateClusterCallback (FrameworkStatus frameworkStatus, PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigCreateClusterWorker::createClusterRequestFrameworkToCreateClusterCallback : Entering ...");

    PrismLinearSequencerContext              *pPrismLinearSequencerContext              = reinterpret_cast<PrismLinearSequencerContext *> (pContext);
    ClusterObjectManagerCreateClusterMessage *pClusterObjectManagerCreateClusterMessage = reinterpret_cast<ClusterObjectManagerCreateClusterMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    ResourceId                                status                                    = WAVE_MESSAGE_SUCCESS;
    ResourceId                                completionStatus                          = WAVE_MESSAGE_SUCCESS;
    UI32                                      i                                         = 0;
    string                                    secondaryNode;
    UI32                                      secondaryNodePort;
    UI32                                      numberOfNewSecondaryNodes                 = pClusterObjectManagerCreateClusterMessage->getNSecondaryNodes ();

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        status = frameworkStatus;
    }
    else
    {
        completionStatus = pPrismCreateClusterWithNodesMessage->getCompletionStatus();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            status = completionStatus;
        }
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("CentralClusterConfigCreateClusterWorker::processFrameworkCreateClusterReply : FrameworkStatus : ") + frameworkStatus + string (", Message Status : ") + completionStatus);

        for (i = 0; i < numberOfNewSecondaryNodes; i++)
        {
            secondaryNode     = pClusterObjectManagerCreateClusterMessage->getSecondaryNodeName (i);
            secondaryNodePort = pClusterObjectManagerCreateClusterMessage->getSecondaryNodePort (i);

            pClusterObjectManagerCreateClusterMessage->setNodeStaus (secondaryNode, secondaryNodePort, pPrismCreateClusterWithNodesMessage->getNewNodeStatus (secondaryNode, secondaryNodePort));
        }
    }
    else
    {
        for (i = 0; i < numberOfNewSecondaryNodes; i++)
        {
            secondaryNode     = pClusterObjectManagerCreateClusterMessage->getSecondaryNodeName (i);
            secondaryNodePort = pClusterObjectManagerCreateClusterMessage->getSecondaryNodePort (i);

            pClusterObjectManagerCreateClusterMessage->setNodeStaus (secondaryNode, secondaryNodePort, pPrismCreateClusterWithNodesMessage->getNewNodeStatus (secondaryNode, secondaryNodePort));
        }
    }

    delete (pPrismCreateClusterWithNodesMessage);

    pPrismLinearSequencerContext->executeNextStep (status);
}

void CentralClusterConfigCreateClusterWorker::createClusterCommitStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    ClusterObjectManagerCreateClusterMessage *pClusterObjectManagerCreateClusterMessage = reinterpret_cast<ClusterObjectManagerCreateClusterMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    UI32                                      i                                         = 0;
    string                                    secondaryNode;
    UI32                                      secondaryNodePort;
    ResourceId                                secondaryNodeStatus;
    UI32                                      numberOfNewSecondaryNodes                 = pClusterObjectManagerCreateClusterMessage->getNSecondaryNodes ();
    ResourceId                                status                                    = WAVE_MESSAGE_SUCCESS;
    vector<WaveManagedObject *>              *pResults                                  = NULL;
    UI32                                      numberOfResults                           = 0;
    LocationId                                primaryLocationId                         = FrameworkToolKit::getThisLocationId ();
    string                                    primaryIpAddress                          = FrameworkToolKit::getThisLocationIpAddress ();
    UI32                                      primaryPort                               = FrameworkToolKit::getThisLocationPort ();

    startTransaction ();

    PrismCluster *pPrismCluster = new PrismCluster (getPWaveObjectManager ());

    pPrismCluster->setGenericStatus     (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD);
    pPrismCluster->setPrimaryLocationId (primaryLocationId);
    pPrismCluster->setPrimaryIpAddress  (primaryIpAddress);
    pPrismCluster->setPrimaryPort       (primaryPort);

    vector<vector <WaveManagedObject *> *>                        waveNodeVectors;
    for (i = 0; i < numberOfNewSecondaryNodes; i++)
    {
        secondaryNode       = pClusterObjectManagerCreateClusterMessage->getStatusNodeName   (i);
        secondaryNodePort   = pClusterObjectManagerCreateClusterMessage->getStatusNodePort   (i);
        secondaryNodeStatus = pClusterObjectManagerCreateClusterMessage->getStatusNodeStatus (i);

        if (WAVE_MESSAGE_SUCCESS == secondaryNodeStatus)
        {
            WaveManagedObjectSynchronousQueryContext queryContext (WaveNode::getClassName ());

            queryContext.addAndAttribute (new AttributeString (secondaryNode,     "ipAddress"));
            queryContext.addAndAttribute (new AttributeUI32   (secondaryNodePort, "port"));

            pResults = querySynchronously (&queryContext);

            prismAssert (NULL != pResults, __FILE__, __LINE__);

            numberOfResults = pResults->size ();

            prismAssert (1 == numberOfResults, __FILE__, __LINE__);

            WaveNode *pWaveNode = dynamic_cast<WaveNode *> ((*pResults)[0]);
	    prismAssert (NULL != pWaveNode, __FILE__, __LINE__);
            waveNodeVectors.push_back(pResults);
            pPrismCluster->addSecondaryNode (pWaveNode->getObjectId ());
        }
    }

    //Update local waveNode
    WaveManagedObjectSynchronousQueryContext queryContextForPrimarywaveNode (WaveNode::getClassName ());

    queryContextForPrimarywaveNode.addAndAttribute (new AttributeString (primaryIpAddress,     "ipAddress"));
    queryContextForPrimarywaveNode.addAndAttribute (new AttributeUI32   (primaryPort, "port"));

    vector<WaveManagedObject *>* pResultsForPrimaryWavenode = querySynchronously (&queryContextForPrimarywaveNode);
    prismAssert (NULL != pResultsForPrimaryWavenode, __FILE__, __LINE__);
    prismAssert (1 == pResultsForPrimaryWavenode->size(), __FILE__, __LINE__);

    WaveNode *pPrimaryWaveNode = dynamic_cast<WaveNode *> ((*pResultsForPrimaryWavenode)[0]);
    prismAssert( NULL != pPrimaryWaveNode, __FILE__, __LINE__);

    pPrimaryWaveNode->setGenericStatus(WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD);
    pPrimaryWaveNode->setSpecificStatus(WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_PRIMARY);
    updateWaveManagedObject(pPrimaryWaveNode);

    waveNodeVectors.push_back(pResultsForPrimaryWavenode);
    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "CentralClusterConfigCreateClusterWorker::createClusterCommitStep : Failed to Persist the configuration.  Cannot Continue.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        // Reset the status so that it can be used to pass on to the pPrismLinearSequencerContext.

        status = WAVE_MESSAGE_SUCCESS;
    }

    UI32 numberOfWaveNodeVectors = waveNodeVectors.size();
    for(i = 0;i<numberOfWaveNodeVectors;++i) 
    {
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(waveNodeVectors[i]);
    }

    delete pPrismCluster;

    pPrismLinearSequencerContext->executeNextStep (status);
}

void CentralClusterConfigCreateClusterWorker::createClusterStartHeartBeatsStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    ClusterObjectManagerCreateClusterMessage *pClusterObjectManagerCreateClusterMessage = reinterpret_cast<ClusterObjectManagerCreateClusterMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    UI32                                      i                                         = 0;
    string                                    secondaryNode;
    UI32                                      secondaryNodePort;
    ResourceId                                secondaryNodeStatus;
    UI32                                      numberOfNewSecondaryNodes                 = pClusterObjectManagerCreateClusterMessage->getNSecondaryNodes ();
    ResourceId                                status                                    = WAVE_MESSAGE_SUCCESS;

    for (i = 0; i < numberOfNewSecondaryNodes; i++)
    {
        secondaryNode       = pClusterObjectManagerCreateClusterMessage->getStatusNodeName   (i);
        secondaryNodePort   = pClusterObjectManagerCreateClusterMessage->getStatusNodePort   (i);
        secondaryNodeStatus = pClusterObjectManagerCreateClusterMessage->getStatusNodeStatus (i);

        if (WAVE_MESSAGE_SUCCESS == secondaryNodeStatus)
        {
            StartHeartBeatMessage *pMessage = new StartHeartBeatMessage ();
			IpV4Address            secondaryIp (secondaryNode);

            pMessage->setDstIpAddress           (secondaryIp);
            pMessage->setDstPortNumber          (secondaryNodePort);
            pMessage->setHeartBeatInterval      (CENTERAL_CLUSTER_HEART_BEAT_PERIOD);
            pMessage->setHeartBeatFailureThreshold (MAX_LOST_HEARTBEAT);

            status = sendSynchronously (pMessage);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "CentralClusterConfigCreateClusterWorker::createClusterStartHeartBeatsStep : Could not start heart beating with Node : " + secondaryNode + string (", Port : ") + secondaryNodePort + string (", Status = ") + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                status = pMessage->getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_FATAL, "CentralClusterConfigCreateClusterWorker::createClusterStartHeartBeatsStep : Could not start heart beating with Node : " + secondaryNode + string (", Port : ") + secondaryNodePort + string (", Completion Status = ") + FrameworkToolKit::localize (status));
                    prismAssert (false, __FILE__, __LINE__);
                }
            }

            delete pMessage;
        }
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

}
