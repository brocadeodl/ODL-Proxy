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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Cluster/CentralClusterConfigObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/LocationManagement/LocationBase.h"
#include "Shell/ShellObjectManager.h"
#include "Shell/ShellDebug.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Cluster/NodeManagedObject.h"
#include "Cluster/PrismCluster.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Cluster/ClusterDeleteContext.h"
#include "Cluster/ClusterFailoverContext.h"
#include "Cluster/CentralClusterConfigCreateClusterWorker.h"
#include "Cluster/CentralClusterConfigDeleteClusterWorker.h"
#include "Cluster/CentralClusterConfigAddNodeWorker.h"
#include "Cluster/CentralClusterConfigDeleteNodeWorker.h"
#include "Cluster/CentralClusterConfigGetDebugInfoWorker.h"
#include "Cluster/CentralClusterConfigRejoinNodeWorker.h"
#include "Cluster/CentralClusterConfigUpdateHardwareSynchronizationStateWorker.h"
#include "Cluster/Local/WaveNode.h"
#include "Framework/CliBlockManagement/CliBlockWorker.h"

namespace WaveNs
{

CentralClusterConfigObjectManager::CentralClusterConfigObjectManager ()
    : WaveObjectManager (getServiceName ()),
      m_cluster (this)
{
    NodeManagedObject nodeManagedObject (this);
    PrismCluster      prismCluster      (this);

    nodeManagedObject.setupOrm ();
    addManagedClass (NodeManagedObject::getClassName ());

    prismCluster.setupOrm ();
    addManagedClass (PrismCluster::getClassName ());

//    addOperationMap (CLUSTER_CREATE_CLUSTER,                    reinterpret_cast<PrismMessageHandler> (&CentralClusterConfigObjectManager::createClusterConfig));
//    addOperationMap (CLUSTER_DELETE_CLUSTER,                    reinterpret_cast<PrismMessageHandler> (&CentralClusterConfigObjectManager::deleteCluster));
    //addOperationMap (CLUSTER_ADD_NODE,                          reinterpret_cast<PrismMessageHandler> (&CentralClusterConfigObjectManager::addNode));
//    addOperationMap (CLUSTER_DELETE_NODE,                       reinterpret_cast<PrismMessageHandler> (&CentralClusterConfigObjectManager::deleteNode));
  //  addOperationMap (CLUSTER_JOIN_NODE,                         reinterpret_cast<PrismMessageHandler> (&CentralClusterConfigObjectManager::joinNode));
//    addOperationMap (CLUSTER_HEARTBEAT_LOST,                    reinterpret_cast<PrismMessageHandler> (&CentralClusterConfigObjectManager::reportLostHeartBeat));

    m_managmentOperationInProgress = MANAGMENT_OPERATION_IDLE;

    m_pCentralClusterConfigCreateClusterWorker = new CentralClusterConfigCreateClusterWorker (this);

    prismAssert (NULL != m_pCentralClusterConfigCreateClusterWorker, __FILE__, __LINE__);

    m_pCentralClusterConfigDeleteClusterWorker = new CentralClusterConfigDeleteClusterWorker (this);

    prismAssert (NULL != m_pCentralClusterConfigDeleteClusterWorker, __FILE__, __LINE__);

    m_pCentralClusterConfigAddNodeWorker = new CentralClusterConfigAddNodeWorker (this);

    prismAssert (NULL != m_pCentralClusterConfigAddNodeWorker, __FILE__, __LINE__);

    m_pCentralClusterConfigDeleteNodeWorker = new CentralClusterConfigDeleteNodeWorker (this);

    prismAssert (NULL != m_pCentralClusterConfigDeleteNodeWorker, __FILE__, __LINE__);

    m_pCentralClusterConfigGetDebugInfoWorker = new CentralClusterConfigGetDebugInfoWorker(this);

    prismAssert (NULL != m_pCentralClusterConfigGetDebugInfoWorker, __FILE__, __LINE__);

    m_pCentralClusterConfigRejoinNodeWorker = new CentralClusterConfigRejoinNodeWorker(this);

    prismAssert (NULL != m_pCentralClusterConfigRejoinNodeWorker, __FILE__, __LINE__);
    
    m_pCentralClusterConfigUpdateHardwareSynchronizationStateWorker = new CentralClusterConfigUpdateHardwareSynchronizationStateWorker (this);

    prismAssert (NULL != m_pCentralClusterConfigUpdateHardwareSynchronizationStateWorker, __FILE__, __LINE__);

    m_pCliBlockWorker  = new CliBlockWorker (this);

    prismAssert (NULL != m_pCliBlockWorker, __FILE__, __LINE__);
}

CentralClusterConfigObjectManager *CentralClusterConfigObjectManager::getInstance ()
{
    static CentralClusterConfigObjectManager *pCentralClusterConfigObjectManager = NULL;

    if (NULL == pCentralClusterConfigObjectManager)
    {
        pCentralClusterConfigObjectManager = new CentralClusterConfigObjectManager ();
        WaveNs::prismAssert (NULL != pCentralClusterConfigObjectManager, __FILE__, __LINE__);
    }

    return (pCentralClusterConfigObjectManager);
}

CentralClusterConfigObjectManager::~CentralClusterConfigObjectManager ()
{
    if (NULL != m_pCentralClusterConfigCreateClusterWorker)
    {
        delete m_pCentralClusterConfigCreateClusterWorker;
    }

    if (NULL != m_pCentralClusterConfigDeleteClusterWorker)
    {
        delete m_pCentralClusterConfigDeleteClusterWorker;
    }

    if (NULL != m_pCentralClusterConfigAddNodeWorker)
    {
        delete m_pCentralClusterConfigAddNodeWorker;
    }

    if (NULL != m_pCentralClusterConfigDeleteNodeWorker)
    {
        delete m_pCentralClusterConfigDeleteNodeWorker;
    }

    if(NULL != m_pCentralClusterConfigGetDebugInfoWorker)
    {
        delete m_pCentralClusterConfigGetDebugInfoWorker;
    }

    if(NULL != m_pCentralClusterConfigRejoinNodeWorker)
    {
        delete m_pCentralClusterConfigRejoinNodeWorker;
    }

    if (NULL != m_pCentralClusterConfigUpdateHardwareSynchronizationStateWorker) 
    {
        delete m_pCentralClusterConfigUpdateHardwareSynchronizationStateWorker;
    }

    if (NULL != m_pCliBlockWorker)
    {
        delete m_pCliBlockWorker;
    }
}

PrismServiceId CentralClusterConfigObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string CentralClusterConfigObjectManager::getServiceName ()
{
    return ("Centeral Cluster Configuration");
}

PrismMessage *CentralClusterConfigObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case CLUSTER_ADD_NODE :
            pPrismMessage = new ClusterObjectManagerAddNodeMessage;
            break;
        case CLUSTER_DELETE_NODE :
            pPrismMessage = new ClusterObjectManagerDeleteNodeMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

WaveManagedObject *CentralClusterConfigObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((NodeManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new NodeManagedObject (this);
    }
    else if ((PrismCluster::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new PrismCluster (this);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "CentralClusterConfigObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        prismAssert (false, __FILE__, __LINE__);
    }

    return (pWaveManagedObject);
}

void CentralClusterConfigObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    // init cluster configuration as stand alone.
    // The primary IP is set as this node ip address.

    m_cluster.m_selfNode.m_nodeName       = FrameworkToolKit::getThisLocationIpAddress ();
    m_cluster.m_selfNode.m_nodePort       = FrameworkToolKit::getThisLocationPort ();
    m_cluster.m_primaryNode.m_nodeName    = FrameworkToolKit::getThisLocationIpAddress ();
    m_cluster.m_primaryNode.m_nodePort    = FrameworkToolKit::getThisLocationPort ();

    m_clusterDebug.initialize ();

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void CentralClusterConfigObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    // During installation persist NodeManagedObject corresponding to this node.
    if (pWaveAsynchronousContextForBootPhases->getBootReason () != WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT)
    {
        startTransaction ();

        NodeManagedObject *pNodeManagedObject = new NodeManagedObject (this, FrameworkToolKit::getThisLocationId (), FrameworkToolKit::getThisLocationIpAddress (), FrameworkToolKit::getThisLocationPort ());

        pNodeManagedObject->getNodeGenericStatus ();

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "CentralClusterConfigObjectManager::install: Could not install CentralClusterConfigObjectManager.");
            //prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }

        delete (pNodeManagedObject);
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void CentralClusterConfigObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigObjectManager::bootHandleSecondaryNodeUnconfigureBootStep),

#if 0
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigObjectManager::bootQueryNodesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigObjectManager::bootStartHeartBeatsStep),
#endif
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&CentralClusterConfigObjectManager::prismLinearSequencerFailedStep)
    };

    ClusterBootContext *pClusterBootContext = new ClusterBootContext (pWaveAsynchronousContextForBootPhases, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pClusterBootContext->holdAll ();
    pClusterBootContext->start ();
}

void CentralClusterConfigObjectManager::bootHandleSecondaryNodeUnconfigureBootStep (ClusterBootContext *pClusterBootContext)
{
    trace (TRACE_LEVEL_INFO, "CentralClusterConfigObjectManager::bootHandleSecondaryNodeUnconfigureBootStep : Entering ...");

    PrismAsynchronousContext             *pPrismAsynchronousContext             = pClusterBootContext->getPPrismAsynchronousContext ();
    LocationRole                          locationRole                          = FrameworkToolKit::getThisLocationRole ();
    vector<WaveManagedObject *>          *pResults                              = querySynchronously (PrismCluster::getClassName ());

    WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = dynamic_cast<WaveAsynchronousContextForBootPhases *> (pPrismAsynchronousContext);
    prismAssert (NULL != pWaveAsynchronousContextForBootPhases, __FILE__, __LINE__);

    WaveBootReason                        bootReason                            = pWaveAsynchronousContextForBootPhases->getBootReason ();

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_INFO, "CentralClusterConfigObjectManager:: bootHandleSecondaryNodeUnconfigureBootStep: There is no cluster created.");
        }

        else if (1 == numberOfResults)
        {
            if (WAVE_BOOT_SECONDARY_NODE_UNCONFIGURE_BOOT == bootReason && ( locationRole == LOCATION_STAND_ALONE))
            {
                startTransaction ();

                delete ((*pResults)[0]);
                (*pResults)[0] = NULL;

                ResourceId status = commitTransaction ();

                if (FRAMEWORK_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_FATAL, "CentralClusterConfigObjectManager::bootHandleSecondaryNodeUnconfigureBootStep");
                    prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    status = WAVE_MESSAGE_SUCCESS;
                }
            }
            else
            {

               pClusterBootContext->setPPrismCluster (reinterpret_cast<PrismCluster *> ((*pResults)[0]));
            }
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "CentralClusterConfigObjectManager::bootHandleSecondaryNodeUnconfigureBootStep : There can be only one cluster.");
            prismAssert (false, __FILE__, __LINE__);
        }

        pResults->clear ();
        delete pResults;
    }

    pClusterBootContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void CentralClusterConfigObjectManager::bootQueryNodesStep (ClusterBootContext *pClusterBootContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::bootQueryNodesStep : Entering ...");

    PrismCluster                 *pPrismCluster          = pClusterBootContext->getPPrismCluster ();
    vector<ObjectId>              secondaryNodes;
    UI32                          numberOfSecondaryNodes = 0;
    vector<WaveManagedObject *> *pSecondaryNodeResults  = NULL;
    UI32                          numberOfResults        = 0;

    if (NULL == pPrismCluster)
    {
        pClusterBootContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    secondaryNodes         = pPrismCluster->getSecondaryNodes ();
    numberOfSecondaryNodes = secondaryNodes.size ();

    pSecondaryNodeResults = querySynchronously (WaveNode::getClassName (), secondaryNodes);

    prismAssert (NULL != pSecondaryNodeResults, __FILE__, __LINE__);

    numberOfResults = pSecondaryNodeResults->size ();

    prismAssert (numberOfSecondaryNodes == numberOfResults, __FILE__, __LINE__);

    pClusterBootContext->setPSecondaryNodes (pSecondaryNodeResults);

    pClusterBootContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

void CentralClusterConfigObjectManager::bootStartHeartBeatsStep (ClusterBootContext *pClusterBootContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::bootQueryNodesStep : Entering ...");

    vector<WaveManagedObject *> *pSecondaryNodeResults  = pClusterBootContext->getPSecondaryNodes ();
    UI32                          numberOfSecondaryNodes = 0;
    UI32                          i                      = 0;
    NodeManagedObject            *pSecondaryNode         = NULL;
    string                        secondaryNodeName;
    UI32                          secondaryNodePort;
    string                        primaryNodeName;
    UI32                          primaryNodePort;
    ResourceId                    status                 = WAVE_MESSAGE_SUCCESS;
    NodeManagedObject             tempNode               (this, FrameworkToolKit::getThisLocationId (), FrameworkToolKit::getThisLocationIpAddress (), FrameworkToolKit::getThisLocationPort ());
    LocationId                    secondaryLocationId    = 0;

    if (NULL == pSecondaryNodeResults)
    {
        pClusterBootContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    numberOfSecondaryNodes = pSecondaryNodeResults->size ();

    for (i = 0; i < numberOfSecondaryNodes; i++)
    {
        pSecondaryNode = reinterpret_cast<NodeManagedObject *> ((*pSecondaryNodeResults)[i]);

        prismAssert (NULL != pSecondaryNode, __FILE__, __LINE__);

        if ((*pSecondaryNode) != tempNode)
        {
            secondaryNodeName = pSecondaryNode->getNodeName ();
            secondaryNodePort = pSecondaryNode->getNodePort ();
            primaryNodeName   = tempNode.getNodeName ();
            primaryNodePort   = tempNode.getNodePort ();

            // First send a message to the remote node indicating that it shall start heartbeating with us.

            secondaryLocationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (secondaryNodeName, secondaryNodePort);

            StartHeartBeatMessage messageToSecondaryLocation;

            messageToSecondaryLocation.setDstIpAddress           (primaryNodeName);
            messageToSecondaryLocation.setDstPortNumber                (primaryNodePort);
            messageToSecondaryLocation.setHeartBeatInterval      (CENTERAL_CLUSTER_HEART_BEAT_PERIOD);
            messageToSecondaryLocation.setHeartBeatFailureThreshold (MAX_LOST_HEARTBEAT);

            status = sendSynchronously (&messageToSecondaryLocation, secondaryLocationId);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::bootStartHeartBeatsStep : Failed to Start heart beat on the Secondary Node.  Node Name : " + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Status : ") + FrameworkToolKit::localize (status));
            }
            else
            {
                status = messageToSecondaryLocation.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::bootStartHeartBeatsStep : Failed to Start heart beat on the Secondary Node.  Node Name : " + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Completion Status : ") + FrameworkToolKit::localize (status));
                }
                else
                {
                    trace (TRACE_LEVEL_INFO, "CentralClusterConfigObjectManager::bootStartHeartBeatsStep : Started Heart Beat from " + secondaryNodeName + string (":") + secondaryNodePort);
                }
            }

            // Then start the heart to that location for full duplex heart beat.

            StartHeartBeatMessage message;

            message.setDstIpAddress           (secondaryNodeName);
            message.setDstPortNumber                (secondaryNodePort);
            message.setHeartBeatInterval      (CENTERAL_CLUSTER_HEART_BEAT_PERIOD);
            message.setHeartBeatFailureThreshold (MAX_LOST_HEARTBEAT);

            status = sendSynchronously (&message);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::bootStartHeartBeatsStep : Failed to Start heart beat to the Secondary Node.  Node Name : " + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Status : ") + FrameworkToolKit::localize (status));
            }
            else
            {
                status = message.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::bootStartHeartBeatsStep : Failed to Start heart beat to the Secondary Node.  Node Name : " + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Completion Status : ") + FrameworkToolKit::localize (status));
                }
                else
                {
                    trace (TRACE_LEVEL_INFO, "CentralClusterConfigObjectManager::bootStartHeartBeatsStep : Started Heart Beat with " + secondaryNodeName + string (":") + secondaryNodePort);
                }
            }
        }
    }

    pClusterBootContext->executeNextStep (status);
}

void CentralClusterConfigObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
    vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::failoverQueryPrismClusterStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::failoverDetermineIfPrimaryChangedStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::failoverQueryAllWaveNodeObjectsStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::failoverUpdateWaveNodeObjectsStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::failoverUpdatePrismClusterStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
#if 0
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep),
#endif
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::prismSynchronousLinearSequencerFailedStep)
    };
    WaveNs::PrismSynchronousLinearSequencerStep sequencerUncontrolledSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::failoverQueryPrismClusterStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::failoverDetermineIfPrimaryChangedStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::failoverQueryAllWaveNodeObjectsStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::failoverUncontrolledUpdateWaveNodeObjectsStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::failoverUpdatePrismClusterStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
#if 0
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::failoverStartHeartBeatsIfUncontrolledPrimaryChangedStep),
#endif
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&CentralClusterConfigObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    ClusterFailoverContext *pClusterFailoverContext;

    if(FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED == failoverReason) 
    {
        pClusterFailoverContext = new ClusterFailoverContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerUncontrolledSteps, sizeof (sequencerUncontrolledSteps) / sizeof (sequencerSteps[0]), failoverReason, failedLocationIds);
    } 
    else 
    {
        pClusterFailoverContext = new ClusterFailoverContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]), failoverReason, failedLocationIds);
    }

    ResourceId status = pClusterFailoverContext->execute ();

    pFailoverAsynchronousContext->setCompletionStatus (status);
    pFailoverAsynchronousContext->callback ();
}

ResourceId CentralClusterConfigObjectManager::failoverQueryPrismClusterStep (ClusterFailoverContext *pClusterFailoverContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::failoverQueryPrismClusterStep : Entering ...");

    vector<WaveManagedObject *> *pResults       = querySynchronously (PrismCluster::getClassName ());
    ResourceId                   status         = WAVE_MESSAGE_ERROR;

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_INFO, "CentralClusterConfigObjectManager::failoverQueryPrismClusterStep : There is no cluster created. Cluster is Deleted");
            pClusterFailoverContext->setIsClusterExists(false);
            
            status = WAVE_MESSAGE_SUCCESS;
        }
        else if (1 == numberOfResults)
        {
            pClusterFailoverContext->setPPrismCluster (reinterpret_cast<PrismCluster *> ((*pResults)[0]));
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "CentralClusterConfigObjectManager::failoverQueryPrismClusterStep : There can be only one cluster.");
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }

        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "CentralClusterConfigObjectManager::failoverQueryPrismClusterStep : Could not query for clusters in the system");
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

ResourceId CentralClusterConfigObjectManager::failoverDetermineIfPrimaryChangedStep (ClusterFailoverContext *pClusterFailoverContext)
{
    if (pClusterFailoverContext->getIsClusterExists ())
    {
        PrismCluster *pPrismCluster = pClusterFailoverContext->getPPrismCluster ();

        prismAssert (NULL != pPrismCluster, __FILE__, __LINE__);

        const LocationId previousClusterPrimaryLocationId = pPrismCluster->getPrimaryLocationId ();
        const LocationId thisLocationId                   = FrameworkToolKit::getThisLocationId ();

        if (previousClusterPrimaryLocationId != thisLocationId)
        {
            trace (TRACE_LEVEL_DEBUG, "CentralClusterConfigObjectManager::failoverDetermineIfPrimaryChangedStep : PRIMARY CHANGED");
            pClusterFailoverContext->setIsPrimaryChanged (true);
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "CentralClusterConfigObjectManager::failoverDetermineIfPrimaryChangedStep : PRIMARY DID NOT CHANGE");
            pClusterFailoverContext->setIsPrimaryChanged (false);
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId CentralClusterConfigObjectManager::failoverQueryAllWaveNodeObjectsStep (ClusterFailoverContext *pClusterFailoverContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::failoverQueryAllNodeManagedObjectsStep : Entering ...");
    
    vector<WaveManagedObject *> *pNodeResults    = NULL;

    pNodeResults = querySynchronously (WaveNode::getClassName ());

    prismAssert (NULL != pNodeResults, __FILE__, __LINE__);

    pClusterFailoverContext->setPNodeManagedObjects (pNodeResults);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId CentralClusterConfigObjectManager::failoverUpdatePrismClusterStep (ClusterFailoverContext *pClusterFailoverContext)
{
    if (pClusterFailoverContext->getIsClusterExists ())
    {
        PrismCluster *pPrismCluster         = pClusterFailoverContext->getPPrismCluster ();

        prismAssert (NULL != pPrismCluster, __FILE__, __LINE__);

        if (LOCATION_STAND_ALONE == FrameworkToolKit::getThisLocationRole())
        {
            delete pPrismCluster;
            pPrismCluster = NULL;
            pClusterFailoverContext->setPPrismCluster (NULL);                    
            pClusterFailoverContext->setIsClusterExists(false);

            return (WAVE_MESSAGE_SUCCESS);
        }

        bool          isPrimaryChanged      = pClusterFailoverContext->getIsPrimaryChanged ();
        LocationId    primaryLocationId     = FrameworkToolKit::getThisLocationId ();
        string        primaryIpAddress      = FrameworkToolKit::getThisLocationIpAddress ();
        UI32          primaryPort           = FrameworkToolKit::getThisLocationPort ();
        vector<WaveManagedObject *> *pNodes = pClusterFailoverContext->getPNodeManagedObjects ();
        WaveNode     *pWaveNode             = NULL;
        UI32          i                     = 0; 
        bool          isClusterDegraded     = false;

        trace (TRACE_LEVEL_INFO, string("CentralClusterConfigObjectManager::failoverUpdatePrismClusterStep : Primary Changed:") + isPrimaryChanged + string(" port: ") + primaryPort);


        updateWaveManagedObject (pPrismCluster);

        // Update the status of the cluster to degraded if any node in the
        // cluster is disconnected, otherwise to good.
        for (i = 0; i < (pNodes->size ()); i++)
        {
            pWaveNode = dynamic_cast<WaveNode *> ((*pNodes)[i]);
            prismAssert( NULL != pWaveNode, __FILE__, __LINE__);
 
            if (WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_SECONDARY_NOT_CONNECTED_TO_CLUSTER == pWaveNode->getSpecificStatus()) 
            {
                isClusterDegraded = true;
                break;
            }
    
        }

        if (true == isClusterDegraded) 
        {
            pPrismCluster->setGenericStatus (WAVE_MANAGED_OBJECT_GENERIC_STATUS_DEGRADED);
            trace (TRACE_LEVEL_INFO, "CentralClusterConfigObjectManager::failoverUpdatePrismClusterStep : Updating cluster status to degraded as node in the cluster: " + pWaveNode->getIpAddress ()  + string (" is disconnected"));
        }
        else 
        {
            pPrismCluster->setGenericStatus (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD);
            trace (TRACE_LEVEL_INFO, "CentralClusterConfigObjectManager::failoverUpdatePrismClusterStep : Updating cluster status to good as all nodes in the cluster are connected");
        } 

        if (true == isPrimaryChanged)
        {
            pPrismCluster->setPrimaryLocationId (primaryLocationId);
            pPrismCluster->setPrimaryIpAddress  (primaryIpAddress);
            pPrismCluster->setPrimaryPort       (primaryPort);
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId CentralClusterConfigObjectManager::failoverUncontrolledUpdateWaveNodeObjectsStep (ClusterFailoverContext *pClusterFailoverContext)
{
    LocationId                   thisLocationId  = FrameworkToolKit::getThisLocationId ();
    vector<WaveManagedObject *> *pNodes          = pClusterFailoverContext->getPNodeManagedObjects ();
    UI32                         i               = 0;
    LocationId                   nodeLocationId  = 0;
    WaveNode                    *pWaveNode       = NULL;

    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::failoverUncontrolledUpdateWaveNodeObjectsStep : Entering ...");

    if (pClusterFailoverContext->getIsClusterExists ())
    {
        PrismCluster       *pPrismCluster          = pClusterFailoverContext->getPPrismCluster ();

        prismAssert (NULL != pPrismCluster, __FILE__, __LINE__);
        prismAssert (NULL != pNodes,        __FILE__, __LINE__);

        trace (TRACE_LEVEL_INFO, string("failoverUncontrolledUpdateWaveNodeObjectsStep : Primary ") + pPrismCluster->getPrimaryLocationId() + string(" Node ") + pPrismCluster->getPrimaryPort());


        for (i = 0; i < (pNodes->size ()); i++)
        {
            pWaveNode = dynamic_cast<WaveNode *> ((*pNodes)[i]);
            prismAssert (NULL != pWaveNode, __FILE__, __LINE__);

            nodeLocationId = pWaveNode->getLocationId ();
            trace (TRACE_LEVEL_INFO, string("CentralClusterConfigObjectManager::failoverUncontrolledUpdateWaveNodeObjectsStep : Location: ") + nodeLocationId + string(" Port:") +  pWaveNode->getPort());

            updateWaveManagedObject (pWaveNode);

            if (true == (pClusterFailoverContext->isAFailedLocationId (nodeLocationId)))
            {
                trace (TRACE_LEVEL_INFO, string("failoverUncontrolledUpdateWaveNodeObjectsStep Set Failed state: Location: ") + nodeLocationId + string(" Port:") +  pWaveNode->getPort());
                pWaveNode->setGenericStatus(WAVE_MANAGED_OBJECT_GENERIC_STATUS_FAILED);
                pWaveNode->setSpecificStatus(WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_SECONDARY_NOT_CONNECTED_TO_CLUSTER);
                pWaveNode->setHardwareSynchronizationState (WAVE_NODE_HARDWARE_SYNCHRONIZATION_STATE_UNKNOWN);

            } else {
                if (true == (pClusterFailoverContext->getIsPrimaryChanged ())) {
                    if(nodeLocationId == thisLocationId) {
                        trace (TRACE_LEVEL_FATAL, string("failoverUncontrolledUpdateWaveNodeObjectsStep Set PRIMARY state: Location: ") + nodeLocationId + string(" Port:") +  pWaveNode->getPort());
                        pWaveNode->setSpecificStatus(WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_PRIMARY);
    
                    }
                }
            }

        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}


ResourceId CentralClusterConfigObjectManager::failoverUpdateWaveNodeObjectsStep (ClusterFailoverContext *pClusterFailoverContext)
{
    vector<WaveManagedObject *> *pNodes          = pClusterFailoverContext->getPNodeManagedObjects ();
    UI32                         i               = 0;
    LocationId                   nodeLocationId  = 0;
    WaveNode                    *pWaveNode       = NULL;
    vector<WaveManagedObject *>  nodeLeft;
    

    if (pClusterFailoverContext->getIsClusterExists ())
    {
        PrismCluster       *pPrismCluster          = pClusterFailoverContext->getPPrismCluster ();

        prismAssert (NULL != pPrismCluster, __FILE__, __LINE__);
        prismAssert (NULL != pNodes,        __FILE__, __LINE__);

 
        if (LOCATION_STAND_ALONE != FrameworkToolKit::getThisLocationRole())
        {
            updateWaveManagedObject (pPrismCluster);
        }

        for (i = 0; i < (pNodes->size ()); i++)
        {
            pWaveNode = dynamic_cast<WaveNode *> ((*pNodes)[i]);
            prismAssert (NULL != pWaveNode, __FILE__, __LINE__);

            nodeLocationId = pWaveNode->getLocationId ();

            if (true == (pClusterFailoverContext->isAFailedLocationId (nodeLocationId)) /* || ((primaryLocationId == thisLocationId) &&(nodeLocationId == thisLocationId)) */)
            {
                if (LOCATION_STAND_ALONE != FrameworkToolKit::getThisLocationRole())
                {
                    pPrismCluster->removeSecondaryNode (pWaveNode->getObjectId()); 
                }

                delete pWaveNode;
                pWaveNode = NULL;
            }
            else
            {
                nodeLeft.push_back(pWaveNode);
            }
        }

        *pNodes = nodeLeft;
    }
    else
    {
        for (i = 0; i < (pNodes->size ()); i++)
        {
            pWaveNode = dynamic_cast<WaveNode *> ((*pNodes)[i]);
            prismAssert (NULL != pWaveNode, __FILE__, __LINE__);

            nodeLocationId = pWaveNode->getLocationId ();

            if (true == pClusterFailoverContext->isAFailedLocationId (nodeLocationId))
            {
                delete pWaveNode;
                pWaveNode = NULL;
            }
            else
            {   
                nodeLeft.push_back(pWaveNode);
            }
        }

        *pNodes = nodeLeft;
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep (ClusterFailoverContext *pClusterFailoverContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Starting ...");

    if (pClusterFailoverContext->getIsClusterExists ())
    {
        bool isPrimaryChanged = pClusterFailoverContext->getIsPrimaryChanged ();

        if (true != isPrimaryChanged)
        {
            return (WAVE_MESSAGE_SUCCESS);
        }

        PrismCluster                 *pPrismCluster         = pClusterFailoverContext->getPPrismCluster ();
        vector<WaveManagedObject *> *pSecondaryNodeResults = pClusterFailoverContext->getPNodeManagedObjects ();

        prismAssert (NULL != pPrismCluster,         __FILE__, __LINE__);
        prismAssert (NULL != pSecondaryNodeResults, __FILE__, __LINE__);

        string                        secondaryNodeName;
        UI32                          secondaryNodePort;
        string                        primaryNodeName;
        UI32                          primaryNodePort;
        ResourceId                    status                 = WAVE_MESSAGE_SUCCESS;
        WaveNode                      tempNode               (this, FrameworkToolKit::getThisLocationId (), FrameworkToolKit::getThisLocationIpAddress (), FrameworkToolKit::getThisLocationPort ());
        LocationId                    secondaryLocationId    = 0;


        UI32 numberOfSecondaryNodes = pSecondaryNodeResults->size ();

        for (UI32 i = 0; i < numberOfSecondaryNodes; i++)
        {
            WaveNode* pSecondaryNode = dynamic_cast<WaveNode *> ((*pSecondaryNodeResults)[i]);

            prismAssert (NULL != pSecondaryNode, __FILE__, __LINE__);

            secondaryNodeName   = pSecondaryNode->getIpAddress ();
            secondaryNodePort   = pSecondaryNode->getPort ();
            primaryNodeName     = tempNode.getIpAddress ();
            primaryNodePort     = tempNode.getPort ();
            secondaryLocationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (secondaryNodeName, secondaryNodePort);

            if (((*pSecondaryNode) != tempNode) && (WAVE_MANAGED_OBJECT_GENERIC_STATUS_FAILED != (pSecondaryNode->getGenericStatus ())))
            {

            // First send a message to the remote node indicating that it shall start heartbeating with us.

                StartHeartBeatMessage messageToSecondaryLocation;

                messageToSecondaryLocation.setDstIpAddress           (primaryNodeName);
                messageToSecondaryLocation.setDstPortNumber                (primaryNodePort);
                messageToSecondaryLocation.setHeartBeatInterval      (CENTERAL_CLUSTER_HEART_BEAT_PERIOD);
                messageToSecondaryLocation.setHeartBeatFailureThreshold (MAX_LOST_HEARTBEAT);

                status = sendSynchronously (&messageToSecondaryLocation, secondaryLocationId);

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Failed to Start heart beat on the Secondary Node.  Node Name : " + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Status : ") + FrameworkToolKit::localize (status));
                }
                else
                {
                    status = messageToSecondaryLocation.getCompletionStatus ();

                    if (WAVE_MESSAGE_SUCCESS != status)
                    {
                        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Failed to Start heart beat on the Secondary Node.  Node Name : " + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Completion Status : ") + FrameworkToolKit::localize (status));
                    }
                    else
                    {
                        trace (TRACE_LEVEL_INFO, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Started Heart Beat from " + secondaryNodeName + string (":") + secondaryNodePort);
                    }
                }

                // Then start the heart to that location for full duplex heart beat.

                StartHeartBeatMessage message;

                message.setDstIpAddress           (secondaryNodeName);
                message.setDstPortNumber                (secondaryNodePort);
                message.setHeartBeatInterval      (CENTERAL_CLUSTER_HEART_BEAT_PERIOD);
                message.setHeartBeatFailureThreshold (MAX_LOST_HEARTBEAT);

                status = sendSynchronously (&message);

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Failed to Start heart beat to the Secondary Node.  Node Name : " + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Status : ") + FrameworkToolKit::localize (status));
                }
                else
                {
                    status = message.getCompletionStatus ();

                    if (WAVE_MESSAGE_SUCCESS != status)
                    {
                        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Failed to Start heart beat to the Secondary Node.  Node Name : " + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Completion Status : ") + FrameworkToolKit::localize (status));
                    }
                    else
                    {
                        trace (TRACE_LEVEL_INFO, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Started Heart Beat with " + secondaryNodeName + string (":") + secondaryNodePort);
                    }
                }
            }
        }
    }
    else
    {

        WaveNode *pNode = pClusterFailoverContext->getThisWaveNode();
        delete pNode;
        pNode = NULL;
    }

    // FIXME : Sagar : Investigate to find out if we need to so any special handling if we are not able to send heart beats.

    return (WAVE_MESSAGE_SUCCESS);
}


ResourceId CentralClusterConfigObjectManager::failoverStartHeartBeatsIfUncontrolledPrimaryChangedStep (ClusterFailoverContext *pClusterFailoverContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfUncontrolledPrimaryChangedStep : Starting ...");
    if (pClusterFailoverContext->getIsClusterExists ())
    {   
        bool isPrimaryChanged = pClusterFailoverContext->getIsPrimaryChanged ();

        // If primary is not changed, we do not need to start heartbeat. Heartbeat should be running for alive nodes.
        if (true != isPrimaryChanged)
        {
            trace (TRACE_LEVEL_INFO, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Primary not chnaged...");
            return (WAVE_MESSAGE_SUCCESS);
        }

        PrismCluster                 *pPrismCluster         = pClusterFailoverContext->getPPrismCluster ();
        vector<WaveManagedObject *> *pSecondaryNodeResults = pClusterFailoverContext->getPNodeManagedObjects ();

        prismAssert (NULL != pPrismCluster,         __FILE__, __LINE__);
        prismAssert (NULL != pSecondaryNodeResults, __FILE__, __LINE__);

        UI32                          numberOfSecondaryNodes = 0;
        UI32                          i                      = 0;
        WaveNode                     *pSecondaryNode         = NULL;
        string                        secondaryNodeName;
        UI32                          secondaryNodePort;
        string                        primaryNodeName;
        UI32                          primaryNodePort;
        ResourceId                    status                 = WAVE_MESSAGE_SUCCESS;
        WaveNode                      tempNode               (this, FrameworkToolKit::getThisLocationId (), FrameworkToolKit::getThisLocationIpAddress (), FrameworkToolKit::getThisLocationPort ());
        LocationId                    secondaryLocationId    = 0;

        numberOfSecondaryNodes = pSecondaryNodeResults->size ();

        for (i = 0; i < numberOfSecondaryNodes; i++)
        {
            pSecondaryNode = dynamic_cast<WaveNode *> ((*pSecondaryNodeResults)[i]);            

            prismAssert (NULL != pSecondaryNode, __FILE__, __LINE__);

            secondaryNodeName   = pSecondaryNode->getIpAddress();
            secondaryNodePort   = pSecondaryNode->getPort ();
            primaryNodeName     = tempNode.getIpAddress ();
            primaryNodePort     = tempNode.getPort ();
            secondaryLocationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (secondaryNodeName, secondaryNodePort);
            trace (TRACE_LEVEL_INFO, string("failoverStartHeartBeatsIfPrimaryChangedStep No Sec: ")+ numberOfSecondaryNodes + string("Primary Node: ") + primaryNodeName + string (", Prim Node Port : ") + primaryNodePort);
            trace (TRACE_LEVEL_INFO, string("ChangedStep: Sec Node Name : ") + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Prim Node Name : ") + primaryNodeName + string (", Prim Node Port : ") + primaryNodePort + string ("Secondary Locaion Id : ") + secondaryLocationId);

            if (((*pSecondaryNode) != tempNode) && (WAVE_MANAGED_OBJECT_GENERIC_STATUS_FAILED != (pSecondaryNode->getGenericStatus ())))
            {
                trace (TRACE_LEVEL_INFO, string("ChangedStep: Sec Node Name : ") + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Prim Node Name : ") + primaryNodeName + string (", Prim Node Port : ") + primaryNodePort);

                StartHeartBeatMessage message;

                message.setDstIpAddress              (secondaryNodeName);
                message.setDstPortNumber             (secondaryNodePort);
                message.setHeartBeatInterval         (CENTERAL_CLUSTER_HEART_BEAT_PERIOD);
                message.setHeartBeatFailureThreshold (MAX_LOST_HEARTBEAT);

                status = sendSynchronously (&message);
                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Failed to Start heart beat to the Secondary Node.  Node Name : " + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Status : ") + FrameworkToolKit::localize (status));
                }
                else
                {
                    status = message.getCompletionStatus ();

                    if (WAVE_MESSAGE_SUCCESS != status)
                    {
                        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Failed to Start heart beat to the Secondary Node.  Node Name : " + secondaryNodeName + string (", Port : ") + secondaryNodePort + string (", Completion Status : ") + FrameworkToolKit::localize (status));
                    }
                    else
                    {
                        trace (TRACE_LEVEL_INFO, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Started Heart Beat with " + secondaryNodeName + string (":") + secondaryNodePort);
                    }
                }
            }
        }
    }
    else
    {   
        trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Cluster Does Not exist ...");
        WaveNode *pNode = pClusterFailoverContext->getThisWaveNode();
        delete pNode;
        pNode = NULL;
    }

   trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::failoverStartHeartBeatsIfPrimaryChangedStep : Exiting ...");

    return (WAVE_MESSAGE_SUCCESS);
}



void CentralClusterConfigObjectManager::createClusterConfig(ClusterObjectManagerCreateClusterMessage *pMessage)
{
    UI32        index;
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::createClusterConfig : Entering ...");
    m_mutex.lock ();

    // Check if we can accept the request for creating the cluster.
    UI32 status = validateClusterCreation (pMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        // if we can not process the request we should reset the management busy
        // unless we did not accept the request becuase there is another management
        // operation in progress. In this case do not reset the managment flag
        // because we are not the one who set it.

        for (index = 0; index < pMessage->getNSecondaryNodes(); index++)
        {
            // Set the status for the creation of all the nodes in the clusters as failed.
            string  nodeName = pMessage->getSecondaryNodeName (index);
            UI32    nodePort = pMessage->getSecondaryNodePort (index);

            char    name [MAX_IP_STRING_LENGTH];

            memcpy (name, nodeName.c_str (), strlen (nodeName.c_str ()) + 1);
            pMessage->setNodeStaus (name, nodePort, status);
        }

        pMessage->setCompletionStatus (status);
        reply (pMessage);
        if (APP_CLUSTER_MANAGMENT_OPERATION_IN_PROGRESS != status)
        {
            resetManagmentOperationStatus ();
        }
        m_mutex.unlock ();
        return;
    }

//    preCreateClusterUpdateConfig (pMessage);

    // Inofrm the framework about the cluster creating cluster. We will continue
    // the cluster creating after the framework reply back to us.

    status = informFrameworkForClusterCreation (pMessage);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("informFrameworkForClusterCreation, status: ") + status);
        pMessage->setCompletionStatus (APP_CLUSTER_REMOTE_CONFIG_FAILED);
        reply (pMessage);
        resetManagmentOperationStatus ();
        m_mutex.unlock ();
        return;
    }

    m_mutex.unlock ();
    return;
}



UI32 CentralClusterConfigObjectManager::validateClusterCreation (ClusterObjectManagerCreateClusterMessage *pMessage)
{
    UI32                                    index;

    // Lock is taken before calling this function.
    //m_mutex.lock ();

    if ( MANAGMENT_OPERATION_IN_PROGRESS == getManagmentOperationStatus () )
    {
        //m_mutex.unlock ();
        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::createClusterConfig: Thread Busy process other mgm command!");
        return (APP_CLUSTER_MANAGMENT_OPERATION_IN_PROGRESS);
    }

    setManagmentOperationStatus ();

    if (m_cluster.m_clusterCreated != 0)
    {
        // cluster already created.
        trace (TRACE_LEVEL_ERROR, "Cluster is already created for this node");
        return (APP_CLUSTER_ALREADY_CREATED);
    }

    /*  Make sure that createCluster is processed in the primary. For DMM, this is for sure
        and we do not need this check but we left it for future use. */
    if (m_cluster.m_selfNode !=  m_cluster.m_primaryNode)
    {
        // node is already in a cluster.
        trace (TRACE_LEVEL_ERROR, "This node is already part of another cluster");
        return (APP_CLUSTER_ALREADY_PART_OF_ANOTHER_CLUSTER);
    }

    for (index = 0; index < pMessage->getNSecondaryNodes (); index++)
    {
        if (((m_cluster.m_selfNode.m_nodeName ==  pMessage->getSecondaryNodeName (index)) && (m_cluster.m_selfNode.m_nodePort ==  ((UI32) pMessage->getSecondaryNodePort (index)))) ||
             (pMessage->getSecondaryNodeName (index) ==  string ("127.0.0.1")) ||
             (pMessage->getSecondaryNodeName (index).c_str() == NULL) ||
             (pMessage->getSecondaryNodePort (index) < 1024))
        {
            // invalid secondary name.
            trace (TRACE_LEVEL_ERROR, "Invalid secondary node name/port");
            return (APP_CLUSTER_INVALID_SECONDARY_ADDRESS);
        }
    }
    return  (WAVE_MESSAGE_SUCCESS);
}

// This function is not used. We decided to create the cluster after framework reply to us.
// If we create it before sending to the framework we will have to update the configuration
// after the framework reply to us as it may fail to communicate to send nodes.
UI32 CentralClusterConfigObjectManager::preCreateClusterUpdateConfig (ClusterObjectManagerCreateClusterMessage *pMessage)
{
    UI32                                    index;
    m_mutex.lock ();

    m_cluster.m_primaryNode      =  NodeManagedObject (this, FrameworkToolKit::getThisLocationId (), FrameworkToolKit::getThisLocationIpAddress (), FrameworkToolKit::getThisLocationPort ());
    m_cluster.m_nSecondaryNodes  =  pMessage->getNSecondaryNodes ();

    for (index = 0; index < m_cluster.m_nSecondaryNodes; index++)
    {
        m_cluster.m_nodeList.push_back (NodeManagedObject (this, 0, pMessage->getSecondaryNodeName (index), pMessage->getSecondaryNodePort (index)));
    }
    m_mutex.unlock ();
    return  (WAVE_MESSAGE_SUCCESS);
}

// Tell the framework that we would like to create a cluster and provide it with
// List of secondary nodes.

UI32 CentralClusterConfigObjectManager::informFrameworkForClusterCreation (ClusterObjectManagerCreateClusterMessage *pMessage)
{
    PrismCreateClusterWithNodesMessage     *pCreateClusterMessage = new PrismCreateClusterWithNodesMessage ();
    UI32                                    index;
    UI32                                    status;

    /* Inform framework that we got a request to create a cluster. */

    for (index = 0; index < pMessage->getNSecondaryNodes(); index++)
    {
        // Provide a list of all the secondary nodes in this cluster.
        string  secondaryNode     =   pMessage->getSecondaryNodeName (index);
        UI32    secondaryNodePort =   pMessage->getSecondaryNodePort (index);
        pCreateClusterMessage->addNewNodeIpAddressAndPort (secondaryNode, secondaryNodePort);
    }

    status = send (pCreateClusterMessage, reinterpret_cast<PrismMessageResponseHandler> (&CentralClusterConfigObjectManager::processFrameworkCreateClusterReply), pMessage);

    return (status);
}

void CentralClusterConfigObjectManager::processFrameworkCreateClusterReply (FrameworkStatus frameworkStatus, PrismCreateClusterWithNodesMessage *pCreateClusterMessage, void *pContext)
{
    ClusterObjectManagerCreateClusterMessage *pMessage =  (ClusterObjectManagerCreateClusterMessage *) pContext;
    UI32                                      status;

    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::processFrameworkCreateClusterReply : Entering ...");


    if ((FRAMEWORK_SUCCESS != frameworkStatus) || (WAVE_MESSAGE_SUCCESS != pCreateClusterMessage->getCompletionStatus()))
    {
        // Cluster creataion failed.
        trace (TRACE_LEVEL_ERROR, string ("CentralClusterConfigObjectManager::processFrameworkCreateClusterReply : FrameworkStatus : ") + frameworkStatus + string (", Message Status : ") + pCreateClusterMessage->getCompletionStatus ());
        pMessage->setCompletionStatus (APP_CLUSTER_REMOTE_CONFIG_FAILED);

        UI32                                      index;
        for (index = 0; index < pMessage->getNSecondaryNodes(); index++)
        {
            // Set the status for the creation of all the nodes in the clusters as failed.
            string  nodeName = pMessage->getSecondaryNodeName (index);
            UI32    nodePort = pMessage->getSecondaryNodePort (index);

            char    name [MAX_IP_STRING_LENGTH];

            memcpy (name, nodeName.c_str (), strlen (nodeName.c_str ()) + 1);
            pMessage->setNodeStaus (name, nodePort, APP_CLUSTER_REMOTE_CONFIG_FAILED);
        }

        reply (pMessage);
        delete (pCreateClusterMessage);
        m_mutex.lock ();
        resetManagmentOperationStatus ();
        m_mutex.unlock ();
        return;
    }

    // Cluster is created succesfully but it may not include all the nodes.
    // Nodes that we could not communicate with will not be part of the cluster.
    // set the node in the cluster database based on the return node status.

    status = postCreateClusterUpdateConfig (pMessage, pCreateClusterMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string("postCreateClusterUpdateConfig, status: ") + status);
        pMessage->setCompletionStatus (status);
        m_mutex.lock ();
        resetManagmentOperationStatus ();
        m_mutex.unlock ();
        reply (pMessage);
        return;
    }

    replyToCreateClusterMessage (pMessage, pCreateClusterMessage);
    delete (pCreateClusterMessage);

    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::processFrameworkCreateClusterReply : call sendCenteralHeartBeat ...");
    sendCenteralHeartBeat ();
    m_mutex.lock ();
    resetManagmentOperationStatus ();
    m_mutex.unlock ();

    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::processFrameworkCreateClusterReply : Exiting ...");
    return;
}

UI32 CentralClusterConfigObjectManager::postCreateClusterUpdateConfig (ClusterObjectManagerCreateClusterMessage *pMessage, PrismCreateClusterWithNodesMessage *pCreateClusterMessage)
{
    UI32                                    status;
    UI32                                    index = 0;
    UI32                                    nodeIndex = 0;
    vector<NodeManagedObject>::iterator     iterator;

    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::postCreateClusterUpdateConfig : Entering ...");
    m_mutex.lock ();

    // Remove whatever is there before since we are creating a new cluster.

    m_cluster.m_nodeList.clear ();

    while (nodeIndex < pMessage->getNSecondaryNodes ())
    {
        // Get the framework status for all the nodes in the cluster.
        // Only the nodes with SUCCESS status will be added to the cluster.
        ResourceId    nodeStatusForCreate;
        nodeStatusForCreate = pCreateClusterMessage->getNewNodeStatus (pMessage->getSecondaryNodeName (nodeIndex), pMessage->getSecondaryNodePort (nodeIndex));
        if (WAVE_MESSAGE_SUCCESS == nodeStatusForCreate)
        {
            m_cluster.m_nodeList.push_back (NodeManagedObject (this, 0, pMessage->getSecondaryNodeName (nodeIndex), pMessage->getSecondaryNodePort (nodeIndex)));
            m_cluster.m_nodeList[index].setNodeGenericStatus (NODE_COMMUNICATED);
            index++;
        }
        nodeIndex++;
    }

    m_cluster.m_nSecondaryNodes  =  index;

    // If there is no node with SUCESS states (we failed to communicate to all the nodes)
    //  we set the message status to reflect that.
    if (0 == m_cluster.m_nSecondaryNodes)
    {
        status = APP_CLUSTER_FAILED_TO_COMMUNICATE_TO_ALL_NODES;
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
        m_cluster.m_clusterCreated            = 1;

    }
    m_mutex.unlock ();

    return  (status);
}

void CentralClusterConfigObjectManager::replyToCreateClusterMessage (ClusterObjectManagerCreateClusterMessage *pMessage, PrismCreateClusterWithNodesMessage *pCreateClusterMessage)
{
    UI32     index;

    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::replyToCreateClusterMessage : Entering ...");
    m_mutex.lock ();

    // Prepare the reply message for create cluster request.
    // Set the validation result for each node in the cluster.

    for (index = 0; index < pMessage->getNSecondaryNodes(); index++)
    {
        string  nodeName = pMessage->getSecondaryNodeName (index);
        UI32    nodePort = pMessage->getSecondaryNodePort (index);
        char    name [MAX_IP_STRING_LENGTH];

        memcpy (name, nodeName.c_str (), strlen (nodeName.c_str ()) + 1);
        pMessage->setNodeStaus (name, nodePort, pCreateClusterMessage->getNewNodeStatus (nodeName, nodePort));
    }

    if (m_cluster.m_nSecondaryNodes > 0)
    {
        pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        pMessage->setCompletionStatus (APP_CLUSTER_FAILED_TO_COMMUNICATE_TO_ALL_NODES);
    }

    m_mutex.unlock ();
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::replyToCreateClusterMessage : Exiting ...");

    reply (pMessage);
}

void CentralClusterConfigObjectManager::deleteCluster(ClusterObjectManagerDeleteClusterMessage *pMessage)
{
    UI32 status;

    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::deleteCluster : Enter ...");
    m_mutex.lock ();

    // Check if we can accept the delete request.
    status = validateDeleteCluster (pMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        // if we can not process the request we should reset the management busy
        // unless we did not accept the request becuase there is another management
        // operation in progress. In this case do not reset the managment flag
        // because we are not the one who set it.

        if (APP_CLUSTER_MANAGMENT_OPERATION_IN_PROGRESS != status)
        {
            resetManagmentOperationStatus ();
        }
        pMessage->setCompletionStatus (status);
        reply (pMessage);
        m_mutex.unlock ();
        return;
    }


    // Inofrm the framework about the delete creating cluster. We will continue
    // the cluster creating after the framework reply back to us.

    status = informFrameworkForDeleteCluster (pMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        pMessage->setCompletionStatus (APP_CLUSTER_REMOTE_CONFIG_FAILED);
        reply (pMessage);
        resetManagmentOperationStatus ();
        m_mutex.unlock ();
        return;
    }


    m_mutex.unlock ();
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::deleteCluster : Exit ...");
    return;

}

UI32 CentralClusterConfigObjectManager::validateDeleteCluster (ClusterObjectManagerDeleteClusterMessage *pMessage)
{
    // Lock is taken beore calling this function.
    if (MANAGMENT_OPERATION_IN_PROGRESS == getManagmentOperationStatus () )
    {
       trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::validateDeleteCluster: Thread Busy processing other mgm command!");
        return (APP_CLUSTER_MANAGMENT_OPERATION_IN_PROGRESS);
    }
    setManagmentOperationStatus ();

    /*  This switch is not the primary node for the cluster. Fail the request */
    if (m_cluster.m_selfNode !=  m_cluster.m_primaryNode)
    {
            // node is already in a cluster.
        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::validateDeleteCluster: Not the primary node. Del cluster failed");
        return (APP_CLUSTER_NOT_PRIMARY_NODE);
    }

    if (m_cluster.m_clusterCreated == 0)
    {
            // No Cluster created yet.
        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::validateDeleteCluster: No Cluster created for this switch. Can not del cluster.");
        return (APP_CLUSTER_NO_CLUSTER_CREATED);
    }

    return (WAVE_MESSAGE_SUCCESS);

}

UI32 CentralClusterConfigObjectManager::informFrameworkForDeleteCluster (ClusterObjectManagerDeleteClusterMessage *pMessage)
{
    UI32 status;

    FrameworkObjectManagerDestroyClusterMessage       *pFrameworkDestroyClusterMessage = new FrameworkObjectManagerDestroyClusterMessage ();

    status = send (pFrameworkDestroyClusterMessage, reinterpret_cast<PrismMessageResponseHandler> (&CentralClusterConfigObjectManager::processFrameworkDeleteClusterReply), pMessage);

    return (status);

}

void CentralClusterConfigObjectManager::processFrameworkDeleteClusterReply (FrameworkStatus frameworkStatus, FrameworkObjectManagerDestroyClusterMessage *pFrameworkDestroyClusterMessage, void *pContext)
{
    ClusterObjectManagerDeleteClusterMessage *pMessage =  (ClusterObjectManagerDeleteClusterMessage *) pContext;
    UI32                                      i;

    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::processFrameworkDeleteClusterReply : Entering ...");

    if ((FRAMEWORK_SUCCESS != frameworkStatus) || (WAVE_MESSAGE_SUCCESS != pFrameworkDestroyClusterMessage->getCompletionStatus()))
    {
        // This should never happen. Delete cluster should never fail.
        // Code here is just in case there is a problem we will not stuck and we will
        // report the failure to the application.

        trace (TRACE_LEVEL_ERROR, string ("CentralClusterConfigObjectManager::processFrameworkDeleteClusterReply : FrameworkStatus : ") + frameworkStatus + string (", Message Status : ") + pFrameworkDestroyClusterMessage->getCompletionStatus ());
        pMessage->setCompletionStatus (APP_CLUSTER_REMOTE_CONFIG_FAILED);
        reply (pMessage);
        delete (pFrameworkDestroyClusterMessage);
        m_mutex.lock ();
        resetManagmentOperationStatus ();
        m_mutex.unlock ();
        return;
    }

    m_mutex.lock ();

    StopHeartBeatMessage   *pStopHeartBeatMessage;

    for (i=0; i < m_cluster.m_nSecondaryNodes; i++)
    {
        pStopHeartBeatMessage = new StopHeartBeatMessage ();
        pStopHeartBeatMessage->setDstIpAddress (m_cluster.m_nodeList[i].m_nodeName);
        pStopHeartBeatMessage->setDstPortNumber (m_cluster.m_nodeList[i].m_nodePort);
        ResourceId status = sendSynchronously (pStopHeartBeatMessage);

        if ((WAVE_MESSAGE_SUCCESS != status) || (WAVE_MESSAGE_SUCCESS != pStopHeartBeatMessage->getCompletionStatus())) 
        {
            trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::processFrameworkDeleteClusterReply : Failed to Start heart beat on the Secondary Node.  Node Name : " + m_cluster.m_nodeList[i].m_nodeName + string (", Port : ") + m_cluster.m_nodeList[i].m_nodePort + string (", Status : ") + FrameworkToolKit::localize (status));
        }

        delete (pStopHeartBeatMessage);
    }
#if 0
    // DMM wanted to be notified when the cluster is deleted.
    if (NULL != m_heartBeatLostCallBack)
    {
            // Call register lost heartbeat function with the appropriate reason to tell the application that
            // the cluster is delete.

        char    nodeName [MAX_IP_STRING_LENGTH];
        for (i = 0; i < m_cluster.m_nSecondaryNodes; i++)
        {
            strncpy (nodeName, m_cluster.m_nodeList[i].m_nodeName.c_str (), MAX_IP_STRING_LENGTH);
            m_heartBeatLostCallBack (APP_CLUSTER_CLUSTER_DELETED, nodeName, m_cluster.m_nodeList[i].m_nodePort);
        }
    }
#endif

    m_mutex.unlock ();

    removeClusterConfig ();
    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);

    delete (pFrameworkDestroyClusterMessage);
    m_mutex.lock ();
    resetManagmentOperationStatus ();
    m_mutex.unlock ();
    return;
}

void CentralClusterConfigObjectManager::removeClusterConfig ()
{
    UI32                                        i;
    vector<NodeManagedObject>::iterator         iterator;

    trace (TRACE_LEVEL_DEBUG, "CentralClusterConfigObjectManager::deleteCluster : Entering ...");
    m_mutex.lock ();

    iterator   = m_cluster.m_nodeList.begin ();

    for (i = 0; i < m_cluster.m_nSecondaryNodes; i++)
    {
        /* Inform platform that node has been deleted from the cluster. */
        m_cluster.m_nodeList.erase (iterator + i);
    }
    m_cluster.m_nSecondaryNodes = 0;

    m_cluster.m_primaryNode.m_nodeName  =   FrameworkToolKit::getThisLocationIpAddress ();
    m_cluster.m_primaryNode.m_nodePort  =   FrameworkToolKit::getThisLocationPort ();
    m_cluster.m_primaryNode.setNodeGenericStatus (NODE_CREATED);
    m_cluster.m_clusterCreated          = 0;

    m_mutex.unlock ();
    return;
}

void CentralClusterConfigObjectManager::joinNode(ClusterObjectManagerJoinNodeMessage *pMessage)
{
    UI32    status;

    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::joinNode : Entering ...");

    m_mutex.lock ();
    status = validateJoinNode (pMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        if (APP_CLUSTER_MANAGMENT_OPERATION_IN_PROGRESS != status)
        {
            resetManagmentOperationStatus ();
        }

        pMessage->setCompletionStatus (status);
        reply (pMessage);
        m_mutex.unlock ();
        return;
    }

    status = informFrameworkForJoinNode (pMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        pMessage->setCompletionStatus (APP_CLUSTER_REMOTE_CONFIG_FAILED);
        reply (pMessage);
        resetManagmentOperationStatus ();
        m_mutex.unlock ();
        return;
    }

    m_mutex.unlock ();
    return;
}

UI32 CentralClusterConfigObjectManager::validateJoinNode (ClusterObjectManagerJoinNodeMessage *pMessage)
{
    UI32    index;

    if (MANAGMENT_OPERATION_IN_PROGRESS == getManagmentOperationStatus () )
    {
        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::validateJoinNode: Thread Busy processing other mgm command!");
        return (APP_CLUSTER_MANAGMENT_OPERATION_IN_PROGRESS);
    }

    setManagmentOperationStatus ();

    /*  This switch is not the primary node for the cluster. Fail the request */
    if (m_cluster.m_selfNode != m_cluster.m_primaryNode)
    {
       trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::validateJoinNode: Not the primary node. Join node failed.");
        return (APP_CLUSTER_NOT_PRIMARY_NODE);
    }

    if (m_cluster.m_clusterCreated == 0)
    {
            // No Cluster created yet.
        trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::validateJoinNode: No Cluster created for this switch. Join node failed.");
        return (APP_CLUSTER_NO_CLUSTER_CREATED);
    }

        // check if the node is already part of the cluster. Fail the opration.
    for (index = 0; index < m_cluster.m_nodeList.size (); index++)
    {
        if ((m_cluster.m_nodeList[index].m_nodeName == pMessage->getNodeName()) && (m_cluster.m_nodeList[index].m_nodePort == ((UI32) pMessage->getNodePort())))
        {
            if (m_cluster.m_nodeList[index].getNodeGenericStatus () == NODE_COMMUNICATED)
            {
                trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::validateJoinNode: Node alreay active in cluster. Join node failed.");
                return (APP_CLUSTER_NODE_ALREADY_IN_CLUSTER);
            }
            else
            {
                return (WAVE_MESSAGE_SUCCESS);
            }
        }
    }

    trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::validateJoinNode: Node was not part of this cluster. Join node failed.");
    return (APP_CLUSTER_NODE_NOT_IN_CLUSTER);
}

UI32 CentralClusterConfigObjectManager::informFrameworkForJoinNode (ClusterObjectManagerJoinNodeMessage *pMessage)
{
    FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkJoinNodeMessage = new FrameworkObjectManagerRejoinNodesToClusterMessage ();
    UI32                                               status;

    /* Inform framework to add node. */
    string  secondaryNode     =   pMessage->getNodeName ();
    SI32    secondaryNodePort =   pMessage->getNodePort ();
    pFrameworkJoinNodeMessage->addNodeIpAddressAndPort (secondaryNode, secondaryNodePort);

    status = send (pFrameworkJoinNodeMessage, reinterpret_cast<PrismMessageResponseHandler> (&CentralClusterConfigObjectManager::processFrameworkJoinNodeReply), pMessage);

    return (status);
}

void CentralClusterConfigObjectManager::processFrameworkJoinNodeReply (FrameworkStatus frameworkStatus, FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkJoinNodeMessage, void *pContext)
{
    ClusterObjectManagerJoinNodeMessage *pMessage =  (ClusterObjectManagerJoinNodeMessage *) pContext;
    UI32                                 index;

    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::processFrameworkJoinNodeReply : Entering ...");

    // If message failed do not do anything. Consider the message lost. Will deal with it within the timer process.
    if ((FRAMEWORK_SUCCESS != frameworkStatus) || (WAVE_MESSAGE_SUCCESS != pFrameworkJoinNodeMessage->getCompletionStatus()))
    {
        trace (TRACE_LEVEL_ERROR, string ("CentralClusterConfigObjectManager::processFrameworkJoinNodeReply : FrameworkStatus : ") + frameworkStatus + string (", Message Status : ") + pFrameworkJoinNodeMessage->getCompletionStatus ());
        pMessage->setCompletionStatus (APP_CLUSTER_REMOTE_CONFIG_FAILED);
        reply (pMessage);
        delete (pFrameworkJoinNodeMessage);
        m_mutex.lock ();
        resetManagmentOperationStatus ();
        m_mutex.unlock ();
        return;
    }

    if (WAVE_MESSAGE_SUCCESS != pFrameworkJoinNodeMessage->getNewNodeStatus (pMessage->getNodeName(), pMessage->getNodePort ()))
    {
        trace (TRACE_LEVEL_ERROR, string ("CentralClusterConfigObjectManager::processFrameworkJoinNodeReply: Failed to add Node, Node Status : ") + pFrameworkJoinNodeMessage->getNewNodeStatus (pMessage->getNodeName(), pMessage->getNodePort ()));
        pMessage->setCompletionStatus (APP_CLUSTER_REMOTE_CONFIG_FAILED);
        reply (pMessage);
        delete (pFrameworkJoinNodeMessage);
        m_mutex.lock ();
        resetManagmentOperationStatus ();
        m_mutex.unlock ();
        return;
    }

    m_mutex.lock ();

    // check if the node is already part of the cluster. Init the node.
    for (index = 0; index < m_cluster.m_nodeList.size (); index++)
    {
        if ((m_cluster.m_nodeList[index].m_nodeName == pMessage->getNodeName()) && (m_cluster.m_nodeList[index].m_nodePort == ((UI32) pMessage->getNodePort())) )
        {
            m_cluster.m_nodeList[index].setNodeGenericStatus (NODE_COMMUNICATED);
            pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            reply (pMessage);
            resetManagmentOperationStatus ();
            m_mutex.unlock ();
            return;
        }
    }

    pMessage->setCompletionStatus (APP_CLUSTER_NODE_NOT_IN_CLUSTER);
    reply (pMessage);
    delete (pFrameworkJoinNodeMessage);
    resetManagmentOperationStatus ();
    m_mutex.unlock ();
}

void CentralClusterConfigObjectManager::sendCenteralHeartBeat ()
{
StartHeartBeatMessage   *pMessage;
UI32                     i;

    trace (TRACE_LEVEL_DEVEL, string ("CentralClusterConfigObjectManager::sendCenteralHeartBeat : Enter ... nSecondary: ") + m_cluster.m_nSecondaryNodes);
    m_mutex.lock ();

    for (i=0; i < m_cluster.m_nSecondaryNodes; i++)
    {
        pMessage = new StartHeartBeatMessage ();
        pMessage->setDstIpAddress (m_cluster.m_nodeList[i].m_nodeName);
        pMessage->setDstPortNumber (m_cluster.m_nodeList[i].m_nodePort);
        pMessage->setHeartBeatInterval (CENTERAL_CLUSTER_HEART_BEAT_PERIOD);
        pMessage->setHeartBeatFailureThreshold (MAX_LOST_HEARTBEAT);
        ResourceId status = sendSynchronously (pMessage);
        if ((WAVE_MESSAGE_SUCCESS != status) || (WAVE_MESSAGE_SUCCESS != pMessage->getCompletionStatus()))
        {
            trace (TRACE_LEVEL_WARN, string ("CentralClusterConfigObjectManager::processFrameworkDeleteClusterReply : failed to process StopHeartBeatMessage to : ") + m_cluster.m_nodeList[i].m_nodeName + string(":") + m_cluster.m_nodeList[i].m_nodePort + string (", status :") + FrameworkToolKit::localize (status));
        }

        delete (pMessage);
    }
    m_mutex.unlock ();
}


UI32 CentralClusterConfigObjectManager::informFrameworkLostHeartBeat(string ipAddress, UI32 port)
{
    FrameworkObjectManagerLostHeartBeatMessage *pFrameworkLostHeartBeatMessage = new FrameworkObjectManagerLostHeartBeatMessage ();
    UI32    status;

    /* Inform framework to add node. */

    pFrameworkLostHeartBeatMessage->addNodeIpAddressAndPort (ipAddress, port);

    status = send (pFrameworkLostHeartBeatMessage, reinterpret_cast<PrismMessageResponseHandler> (&CentralClusterConfigObjectManager::processFrameworkHeartBeatLostReply), NULL);

    return (status);

}

void CentralClusterConfigObjectManager::processFrameworkHeartBeatLostReply (FrameworkStatus frameworkStatus, FrameworkObjectManagerLostHeartBeatMessage *pFrameworkLostHeartBeatMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::processFrameworkHeartBeatLostReply : Entering ...");

    if ((FRAMEWORK_SUCCESS != frameworkStatus) || (WAVE_MESSAGE_SUCCESS != pFrameworkLostHeartBeatMessage->getCompletionStatus()))
    {
        trace (TRACE_LEVEL_ERROR, string ("CentralClusterConfigObjectManager::processFrameworkHeartBeatLostReply : FrameworkStatus : ") + frameworkStatus + string (", Message Status : ") + pFrameworkLostHeartBeatMessage->getCompletionStatus ());
        delete (pFrameworkLostHeartBeatMessage);
        return;
    }

    delete (pFrameworkLostHeartBeatMessage);
    return;
}



UI32 CentralClusterConfigObjectManager::getNodeRole ()
{
    UI32            status;

    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.lock ();

    if (((CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_selfNode.m_nodeName == (CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_primaryNode.m_nodeName) &&
        ((CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_selfNode.m_nodePort == (CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_primaryNode.m_nodePort))
    {
        status = CLUSTER_DMM_PRIMARY_NODE;
    }
    else
    {
        status = CLUSTER_DMM_SECONDARY_NODE;
    }

    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();
    return (status);
}

void CentralClusterConfigObjectManager::lockClusterMutex ()
{
    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.lock ();
}

void CentralClusterConfigObjectManager::unlockClusterMutex ()
{
    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();
}

void CentralClusterConfigObjectManager::setManagmentOperationStatus ()
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::setManagmentOperationStatus");
    m_managmentOperationInProgress = MANAGMENT_OPERATION_IN_PROGRESS;
}

void CentralClusterConfigObjectManager::resetManagmentOperationStatus ()
{
    trace (TRACE_LEVEL_DEVEL, "CentralClusterConfigObjectManager::resetManagmentOperationStatus");
    m_managmentOperationInProgress = MANAGMENT_OPERATION_IDLE;
}

UI32 CentralClusterConfigObjectManager::getManagmentOperationStatus ()
{
    return (m_managmentOperationInProgress);
}
#if 0
void CentralClusterConfigObjectManager::reportLostHeartBeat (HeartBeatLostMessage *pMessage)
{
    trace (TRACE_LEVEL_ERROR, "CentralClusterConfigObjectManager::reportLostHeartBeat : Entering ...");
    string dstNode    =   pMessage->getDstIpAddress ();
    UI32   dstPort    =   pMessage->getDstPort ();
    m_mutex.lock ();

    PauseHeartBeatMessage   *pPauseHeartBeatMessage;

    pPauseHeartBeatMessage = new PauseHeartBeatMessage ();
    pPauseHeartBeatMessage->setDstIpAddress (pMessage->getDstIpAddress ());
    pPauseHeartBeatMessage->setDstPortNumber (pMessage->getDstPort ());
    sendSynchronously (pPauseHeartBeatMessage);
    delete (pPauseHeartBeatMessage);

#if 0
    if (NULL != m_heartBeatLostCallBack)
    {
        char    nodeName [MAX_IP_STRING_LENGTH];
        strncpy (nodeName, dstNode.c_str (), MAX_IP_STRING_LENGTH);
        m_heartBeatLostCallBack (APP_CLUSTER_HEARTBEAT_LOST, nodeName, dstPort);
    }
#endif


        // DMM Specific
        // For DMM take down the cluster instead of just mark the node down.
    m_mutex.unlock ();
    //removeClusterConfig();
    informFrameworkLostHeartBeat (dstNode, dstPort);



    return;
        // END DMM Specific.
}
#endif
}
