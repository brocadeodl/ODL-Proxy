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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               
 *   All rights reserved.  
 *   Description: This file implements the Rejoin processing at the
 *                level of Cluster Service                                 
 *   Author : Aashish Akhouri
 *   Date   : 05/26/2010                                             
 ***************************************************************************/

#include "Cluster/CentralClusterConfigRejoinNodeWorker.h"
#include "Cluster/CentralClusterConfigObjectManager.h"
#include "Cluster/ClusterMessages.h"
#include "Cluster/ClusterTypes.h"
#include "Cluster/ClusterRejoinContext.h"
#include "Cluster/Local/WaveNode.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Cluster/PrismCluster.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

namespace WaveNs
{

/// Name
/// CentralClusterConfigRejoinNodeWorker()
/// Description
/// Constructor
/// Input
/// pCentralClusterConfigObjectManager : pointer to owner configManager
/// Output
/// None
/// Return
/// None
CentralClusterConfigRejoinNodeWorker::CentralClusterConfigRejoinNodeWorker (CentralClusterConfigObjectManager *pCentralClusterConfigObjectManager)
    : WaveWorker (pCentralClusterConfigObjectManager)
{
    addOperationMap (CLUSTER_JOIN_NODE, reinterpret_cast<PrismMessageHandler> (&CentralClusterConfigRejoinNodeWorker::rejoinNodeMessageHandler));
}

/// Name
/// ~CentralClusterConfigRejoinNodeWorker()
/// Description
/// Destructor
/// Input
/// 
/// Output
/// 
/// Return
/// 

CentralClusterConfigRejoinNodeWorker::~CentralClusterConfigRejoinNodeWorker ()
{
}
/// Name
/// createMessageInstance
/// Description
/// Creates the ClusterObjectManagerRejoinNodeMessage
/// Input
/// opcode
/// Output
/// pointer to message
/// Return
/// none

PrismMessage *CentralClusterConfigRejoinNodeWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case CLUSTER_JOIN_NODE :
            pPrismMessage = new ClusterObjectManagerRejoinNodeMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}
/// Name
/// rejoinNodeMessageHandler
/// Description
/// Called by the framework when it receives a ClusterObjectManagerRejoinNodeMessage.
/// It composes the steps to process the message and then starts processing by creating 
/// and invoking the context.
/// Input
/// ClusterObjectManagerRejoinNodeMessage
/// Output
/// none
/// Return
/// none

void CentralClusterConfigRejoinNodeWorker::rejoinNodeMessageHandler (ClusterObjectManagerRejoinNodeMessage *pClusterObjectManagerRejoinNodeMessage)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigRejoinNodeWorker::rejoinNodeMessageHandler : Entering ...");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigRejoinNodeWorker::rejoinNodeValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigRejoinNodeWorker::rejoinNodeRequestFrameworkToRejoinNodeStep),
#if 0
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigRejoinNodeWorker::rejoinNodeCommitStep),
#endif
//        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigRejoinNodeWorker::rejoinNodeStartHeartBeatsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigRejoinNodeWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigRejoinNodeWorker::prismLinearSequencerFailedStep)
    };

    //Note that this is freed in SequencerSucceeded step inside the framework code
    ClusterRejoinContext *pClusterRejoinContext = new ClusterRejoinContext (pClusterObjectManagerRejoinNodeMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0])); 

    pClusterRejoinContext->setIsReplaceRejoin (pClusterObjectManagerRejoinNodeMessage->getIsReplaceRejoin ());
//    pClusterRejoinContext->holdAll ();
    pClusterRejoinContext->start ();

}

/// Name
/// rejoinNodeValidateStep
/// Description
/// This function runs a validation check on the nodes in the message
/// It checks whether
/// 1. The input nodes are valid
/// 2. A cluster is available
/// 3. The nodes in the rejoin message are all secondary and
///    already in the database
/// Input
/// ClusterRejoinContext : Used by the f/w
/// Output
/// none
/// Return
/// none
void CentralClusterConfigRejoinNodeWorker::rejoinNodeValidateStep (ClusterRejoinContext *pClusterRejoinContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigRejoinNodeWorker::rejoinNodeValidateStep : starting ...");

    // Check if cluster is already created and reject if it is not created
    vector<WaveManagedObject *> *pResults = querySynchronously (PrismCluster::getClassName ());
    
    //Check whether Cluster MO obtained from DB
    if (NULL == pResults)
    {
        trace (TRACE_LEVEL_FATAL, string ("CentralClusterConfigRejoinNodeWorker::RejoinNodeValidateStep : Unable to obtain Cluster Managed Object from the database"));
        prismAssert (false, __FILE__, __LINE__);
        pClusterRejoinContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    
    UI32 numberOfResults = pResults->size ();
    if (numberOfResults>1)
    {
        trace (TRACE_LEVEL_FATAL, string ("CentralClusterConfigRejoinNodeWorker::RejoinNodeValidateStep : There can only be one cluster in the system.We obtained ") + numberOfResults + string (" of clusters"));
        prismAssert (false, __FILE__, __LINE__);
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        pClusterRejoinContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    //If cluster is not created
    if (0 == numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, "CentralClusterConfigRejoinNodeWorker::RejoinNodeValidateStep : There is no Cluster Created");
        prismAssert (false, __FILE__, __LINE__);
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        pClusterRejoinContext->executeNextStep (WAVE_MESSAGE_ERROR_CLUSTER_VALIDATION_FAILED);
        return;
    }

    //Released the memory for the Cluster MO
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);


    //Valid case: only one cluster 
    if (1 == numberOfResults)
    {
        // Validate the node name and port number, it should be same as any of current node;
        pResults = querySynchronously (WaveNode::getClassName ());

        prismAssert (NULL != pResults, __FILE__, __LINE__);

        ClusterObjectManagerRejoinNodeMessage *pClusterObjectManagerRejoinNodeMessage = dynamic_cast<ClusterObjectManagerRejoinNodeMessage *> (pClusterRejoinContext->getPPrismMessage ());

	prismAssert(NULL != pClusterObjectManagerRejoinNodeMessage, __FILE__, __LINE__);
        UI32 numberOfNodes = pClusterObjectManagerRejoinNodeMessage->getNNodes ();
        UI32 numberOfNodesInTheDB = pResults->size ();

	//All the Nodes received in the request should already be in the Wave Database
	//and none of them should be primary
	vector<string>                       nodeIpAddresses;
        vector<UI32>                         nodePorts;
        for (UI32 i = 0; i < numberOfNodes; ++i)
        {
            string nodeIpAddress = pClusterObjectManagerRejoinNodeMessage->getNodeIpAddress (i);
            UI32 nodePort = pClusterObjectManagerRejoinNodeMessage->getNodePort (i);

	    //Check for duplicates
	    bool isAlreadyPresentInTheVector = false;
	    for(UI32 k=0;k<nodeIpAddresses.size();++k) {
		if((nodeIpAddresses[k] == nodeIpAddress) && (nodePorts[k]==nodePort)) {
                    isAlreadyPresentInTheVector = true;
		    break;
	        }
	    }

	    if(isAlreadyPresentInTheVector) {

		tracePrintf (TRACE_LEVEL_ERROR, "CentralClusterConfigRejoinNodeWorker::RejoinNodeValidateStep : Ip: %s Port: %d : duplicated in  the RejoinMessage",nodeIpAddress.c_str(),nodePort);
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                pClusterRejoinContext->executeNextStep (WAVE_MESSAGE_ERROR);
		return;
	    }

        bool found = false;

	    //Make sure that the node that is there is already in the database
        //Skip the check if this is a case of replace rejoin
        
        if (false == pClusterRejoinContext->getIsReplaceRejoin ())
        {
            for (UI32 j = 0; j < numberOfNodesInTheDB; ++j)
            {
                //Have to use a dynamic cast through a virtual base
                WaveNode* pNode = dynamic_cast<WaveNode *> ((*pResults)[j]);
                prismAssert(NULL!=pNode,__FILE__,__LINE__);
                if(pNode->getIpAddress()== nodeIpAddress && pNode->getPort()==nodePort){
                    found = true;
                    break;
                }
            }
        }
        else
        {
            found = true;
        }

	    if(!found) 
	    {
		tracePrintf (TRACE_LEVEL_ERROR, "CentralClusterConfigRejoinNodeWorker::RejoinNodeValidateStep : Ip: %s Port: %d : not found in the WaveNodes obtained from the DB",nodeIpAddress.c_str(),nodePort);
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                pClusterRejoinContext->executeNextStep (WAVE_MESSAGE_ERROR);
		return;
	    }
        }
    } 
   
    //Validated the ip and port of the  node
    //delete the vector and node and proceed to next step
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

    trace(TRACE_LEVEL_DEVEL, "CentarlClusterConfigRejoinNodeWorker::RejoinNodeValidateStep: Validation Succeeded");
    pClusterRejoinContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

/// Name
/// rejoinNodeRequestFrameworkToRejoinNodeStep
/// Description
/// This function constructs the message to be sent to the F/w
/// to start processing rejoin
/// Input
/// ClusterRejoinContext : Used by the f/w
/// Output
/// none
/// Return
/// none
void CentralClusterConfigRejoinNodeWorker::rejoinNodeRequestFrameworkToRejoinNodeStep (ClusterRejoinContext *pClusterRejoinContext)
{

    //Message pointer of the mesage received from VCS obtained from ctxt
    ClusterObjectManagerRejoinNodeMessage  *pClusterObjectManagerRejoinNodeMessage = dynamic_cast<ClusterObjectManagerRejoinNodeMessage *> (pClusterRejoinContext->getPPrismMessage ());

    prismAssert(NULL != pClusterObjectManagerRejoinNodeMessage, __FILE__, __LINE__);

    //This message will be sent to the Wave Framework running underneath and will start 
    //the process of rejoin processing at the F/W level on the primary
    FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage = new FrameworkObjectManagerRejoinNodesToClusterMessage ();

    //Fill in if replaceRejoin
    pFrameworkObjectManagerRejoinNodesToClusterMessage->setIsReplaceRejoin (pClusterRejoinContext->getIsReplaceRejoin ());

    //Filling in the ip/port pair of all rejoining nodes in the msg being sent to 
    //the framework
    UI32 nNodes = pClusterObjectManagerRejoinNodeMessage->getNNodes();
    for(UI32 i=0; i<nNodes;++i) {
        string nodeIpAddress     = pClusterObjectManagerRejoinNodeMessage->getNodeIpAddress (i);
        UI32   nodePort = pClusterObjectManagerRejoinNodeMessage->getNodePort (i);
        pFrameworkObjectManagerRejoinNodesToClusterMessage->addNodeIpAddressAndPort (nodeIpAddress,
										     nodePort);
        pFrameworkObjectManagerRejoinNodesToClusterMessage->setNodeLocationId (pClusterObjectManagerRejoinNodeMessage->getNodeLocationId (i));
    }

    //Call into the framework. Notice that this is lodging a callback function in the framework.
    ResourceId status = send (pFrameworkObjectManagerRejoinNodesToClusterMessage, reinterpret_cast<PrismMessageResponseHandler> (&CentralClusterConfigRejoinNodeWorker::rejoinNodeRequestFrameworkToRejoinNodeCallBack), pClusterRejoinContext);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigRejoinNodeWorker::rejoinNodeRequestFrameworkToRejoinNodeStep : Could not request Framework For Rejoining Nodes.  Status " + FrameworkToolKit::localize (status));
        pClusterRejoinContext->executeNextStep (status);
    }

    return;
}


/// Name
/// rejoinNodeRequestFrameworkToRejoinNodeCallBack
/// Description
/// This function handles the callback for rejoin received from the framework
/// Input
/// FrameowrkStatus : Used by f/w to inform the worker if there was an error 
/// in processing within the cluster
/// FrameworkObjectManagerRejoinNodesToClusterMessage: The actual message with the 
/// compeletion status set
/// Context: The sequencer context to be used to transfer to next step
/// Output
/// none
/// Return
/// none
void CentralClusterConfigRejoinNodeWorker::rejoinNodeRequestFrameworkToRejoinNodeCallBack (FrameworkStatus frameworkStatus, FrameworkObjectManagerRejoinNodesToClusterMessage *  pFrameworkObjectManagerRejoinNodesToClusterMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigRejoinNodeWorker::createClusterRequestFrameworkToRejoinNodeCallback : Entering ...");

    ClusterRejoinContext *pClusterRejoinContext = reinterpret_cast<ClusterRejoinContext *> (pContext);
    ClusterObjectManagerRejoinNodeMessage *pClusterObjectManagerRejoinNodeMessage  = reinterpret_cast<ClusterObjectManagerRejoinNodeMessage *> (pClusterRejoinContext->getPPrismMessage ());

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;
    UI32 i = 0;
    string nodeIp;
    UI32 nodePort;
    UI32 noNodes = pClusterObjectManagerRejoinNodeMessage->getNNodes ();

    //If the processing failed in the F/W
    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        status = frameworkStatus;
    }

    //If there was no problem with the processing in the F/W 
    // but on the secondary receiver
    else
    {
        completionStatus = pFrameworkObjectManagerRejoinNodesToClusterMessage->getCompletionStatus();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            status = completionStatus;
        }
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("CentralClusterConfigRejoinNodeWorker::rejoinNodeRequestFrameworkToRejoinNodeCallBack : FrameworkStatus : ") + frameworkStatus + string (", Message Status : ") + completionStatus);

        for (i = 0; i < noNodes; i++)
        {
            nodeIp = pClusterObjectManagerRejoinNodeMessage->getNodeIpAddress (i);
            nodePort = pClusterObjectManagerRejoinNodeMessage->getNodePort (i);
            pClusterObjectManagerRejoinNodeMessage->setNodeStatus (nodeIp, nodePort, pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodeStatus (nodeIp, nodePort));
        }
    }
    else
    {
        for (i = 0; i < noNodes; i++)
        {
            nodeIp = pClusterObjectManagerRejoinNodeMessage->getNodeIpAddress (i);
            nodePort = pClusterObjectManagerRejoinNodeMessage->getNodePort (i);
            pClusterObjectManagerRejoinNodeMessage->setNodeStatus (nodeIp, nodePort, pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodeStatus (nodeIp, nodePort));
        }
    }

    delete pFrameworkObjectManagerRejoinNodesToClusterMessage;

    pFrameworkObjectManagerRejoinNodesToClusterMessage = 0;

    pClusterRejoinContext->executeNextStep (status);
}

/// Name
/// rejoinNodeCommitStep
/// Description
/// This function 
/// Input
/// ClusterRejoinContext : Used by the f/w
/// Output
/// none
/// Return
/// none

void CentralClusterConfigRejoinNodeWorker::rejoinNodeCommitStep (ClusterRejoinContext *pClusterRejoinContext)
{


    //TODO: AASHISH:  Discuss this function with Sagar
    trace(TRACE_LEVEL_DEVEL,"CentralClusterConfigRejoinNodeWorker::rejoinNodeCommitStep: Entering..");
    startTransaction();

    vector<WaveManagedObject *>  *pResultsCluster = querySynchronously (PrismCluster::getClassName ());

    prismAssert (NULL != pResultsCluster, __FILE__, __LINE__);

    UI32                          numberOfResults = pResultsCluster->size ();  
    if (numberOfResults>1)
    {
        trace (TRACE_LEVEL_FATAL, string ("CentralClusterConfigRejoinNodeWorker::RejoinNodeCommitStep : There can only be one cluster in the system.We obtained ") + numberOfResults + string (" of clusters"));
        prismAssert (false, __FILE__, __LINE__);

	    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsCluster);
        pClusterRejoinContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    if (numberOfResults!=1)
    {
        trace (TRACE_LEVEL_FATAL,"CentralClusterConfigRejoinNodeWorker::RejoinNodeValidateStep:There is no Cluster Created");
        prismAssert (false, __FILE__, __LINE__);
	    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsCluster);
        pClusterRejoinContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    ClusterObjectManagerRejoinNodeMessage *pClusterObjectManagerRejoinNodeMessage = dynamic_cast<ClusterObjectManagerRejoinNodeMessage *> (pClusterRejoinContext->getPPrismMessage ());

    prismAssert(NULL != pClusterObjectManagerRejoinNodeMessage, __FILE__, __LINE__);

    UI32 noNewNode = pClusterObjectManagerRejoinNodeMessage->getNNodes ();
    PrismCluster* pPrismCluster = (static_cast<PrismCluster *> ((*pResultsCluster)[0]));

    
    vector<vector<WaveManagedObject *> *>  waveNodeVectors;

    for (UI32 i = 0; i < noNewNode; i++)
    {

            WaveManagedObjectSynchronousQueryContext queryContext (WaveNode::getClassName ());
	    string newNode;
            UI32 newNodePort;
            queryContext.addAndAttribute (new AttributeString (newNode,     "ipAddress"));
            queryContext.addAndAttribute (new AttributeUI32   (newNodePort, "port"));
            
            vector<WaveManagedObject *>  *pResults = querySynchronously (&queryContext);
            prismAssert (NULL != pResults, __FILE__, __LINE__);

            numberOfResults = pResults->size ();
            prismAssert (1 == numberOfResults, __FILE__, __LINE__);

	    //No static cast through virtual base
            WaveNode *pWaveNode = dynamic_cast<WaveNode *> ((*pResults)[0]);
            prismAssert(NULL != pWaveNode, __FILE__, __LINE__);
            updateWaveManagedObject (pPrismCluster);
            waveNodeVectors.push_back (pResults);
    }

    ResourceId status = commitTransaction ();
    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "CentralClusterConfigRejoinNodeWorker::rejoinNodeCommitStep : Failed to Persist the configuration.  Cannot Continue.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        // Reset the status so that it can be used to pass on to the pClusterRejoinContext.
        status = WAVE_MESSAGE_SUCCESS;
    }

    UI32 numVectors = waveNodeVectors.size();
    for (UI32 i = 0; i < numVectors; ++i)
    {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(waveNodeVectors[i]);
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsCluster);
    pClusterRejoinContext->executeNextStep (status);
}


/// Name
/// rejoinNodesStartHearBeatStep
/// Description
/// This function constructs and sends the hearbeat message from
/// the primary to each secondary
/// Input
/// ClusterRejoinContext : Used by the f/w
/// Output
/// none
/// Return
/// none

void CentralClusterConfigRejoinNodeWorker::rejoinNodeStartHeartBeatsStep (ClusterRejoinContext *pClusterRejoinContext)
{

    ClusterObjectManagerRejoinNodeMessage *pClusterObjectManagerRejoinNodeMessage = dynamic_cast<ClusterObjectManagerRejoinNodeMessage *> (pClusterRejoinContext->getPPrismMessage ());

    prismAssert(NULL != pClusterObjectManagerRejoinNodeMessage, __FILE__ , __LINE__);

    UI32 noNewNode = pClusterObjectManagerRejoinNodeMessage->getNNodes ();

    for (UI32 i = 0; i < noNewNode; ++i)
    {
        string  newNodeIpAddress        = pClusterObjectManagerRejoinNodeMessage->getNodeIpAddress (i);
        UI32    newNodePort             = pClusterObjectManagerRejoinNodeMessage->getNodePort   (i);

        StartHeartBeatMessage *pMessage = new StartHeartBeatMessage ();
        pMessage->setDstIpAddress                 (newNodeIpAddress);
        pMessage->setDstPortNumber                (newNodePort);
        pMessage->setHeartBeatInterval            (CENTERAL_CLUSTER_HEART_BEAT_PERIOD);
        pMessage->setHeartBeatFailureThreshold    (MAX_LOST_HEARTBEAT);

        ResourceId status = sendSynchronously (pMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
                trace (TRACE_LEVEL_FATAL, "CentralClusterConfigRejoinNodeWorker::rejoinNodeClusterStartHeartBeatsStep : Could not start heart beating with Node : " + newNodeIpAddress + string (", Port : ") + newNodePort + string (", Status = ") + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
                status = pMessage->getCompletionStatus ();
                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_FATAL, "CentralClusterConfigRejoinNodeWorker::rejoinNodeStartHeartBeatsStep : Could not start heart beating with Node : " + newNodeIpAddress + string (", Port : ") + newNodePort + string (", Completion Status = ") + FrameworkToolKit::localize (status));                     
		    prismAssert (false, __FILE__, __LINE__);
                }
        }

        delete pMessage;
      }
    

    pClusterRejoinContext->executeNextStep (WAVE_MESSAGE_SUCCESS);

}

}

