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

#include "Cluster/CentralClusterConfigObjectManager.h"
#include "Cluster/LocalClusterConfigObjectManager.h"
#include "Cluster/Local/HeartBeat/HeartBeatObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

LocalClusterConfigObjectManager::LocalClusterConfigObjectManager ()
    : WaveLocalObjectManager ("Local Cluster Configuration") /*, m_primaryNode (this) */
{

    addOperationMap (CLUSTER_PRIMARY_NODE_CHANGED,               reinterpret_cast<PrismMessageHandler> (&LocalClusterConfigObjectManager::reportPrimaryNodeChanged));
    addOperationMap (CLUSTER_REPORT_REMOVED_NODE_FROM_CLUSTER,   reinterpret_cast<PrismMessageHandler> (&LocalClusterConfigObjectManager::reportRemovedNodeFromCluster));
    addOperationMap (CLUSTER_GET_NODE_ROLE,                      reinterpret_cast<PrismMessageHandler> (&LocalClusterConfigObjectManager::getNodeRoleHandler));
    addOperationMap (CLUSTER_GET_CLUSTER_CONFIG,                 reinterpret_cast<PrismMessageHandler> (&LocalClusterConfigObjectManager::getClusterConfigHandler));
    addOperationMap (CLUSTER_HEARTBEAT_LOST,                     reinterpret_cast<PrismMessageHandler> (&LocalClusterConfigObjectManager::reportLostHeartBeat));

}

LocalClusterConfigObjectManager *LocalClusterConfigObjectManager::getInstance ()
{
    static LocalClusterConfigObjectManager *pLocalClusterConfigObjectManager = NULL;

    if (NULL == pLocalClusterConfigObjectManager)
    {
        pLocalClusterConfigObjectManager = new LocalClusterConfigObjectManager ();
        WaveNs::prismAssert (NULL != pLocalClusterConfigObjectManager, __FILE__, __LINE__);
    }

    return (pLocalClusterConfigObjectManager);
}

LocalClusterConfigObjectManager::~LocalClusterConfigObjectManager ()
{

}

PrismServiceId LocalClusterConfigObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

// This function is called when a request from the primary node to join
// a cluster is received. The request will be received by framework which will send this message
// to Local cluster.
// The communication between Centeral Cluster and Local cluster is done through the framework.
// Centeral Cluster send a request to create cluster to framework. The framework will send
// the request to the framework on secondary nodes. Framework in secondary node will send
// request to Local cluster service.

void LocalClusterConfigObjectManager::reportPrimaryNodeChanged (LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage *pMessage)
{
    UI32        i;

    trace (TRACE_LEVEL_DEVEL, "LocalClusterConfigObjectManager::reportPrimaryNodeChanged: Entering ...");
    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.lock ();

    if (MANAGMENT_OPERATION_IN_PROGRESS == (CentralClusterConfigObjectManager::getInstance ())->getManagmentOperationStatus () )
    {
        trace (TRACE_LEVEL_ERROR, "LocalClusterConfigObjectManager::reportPrimaryNodeChanged: Busy process other mgm command!");
        pMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
        reply (pMessage);
        (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();
        return;
    }

    (CentralClusterConfigObjectManager::getInstance ())->setManagmentOperationStatus ();

    ClusterManagedObject *pCluster = &(CentralClusterConfigObjectManager::getInstance ())->m_cluster;
    NodeManagedObject *pPrimaryNode = &(CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_primaryNode;

    pPrimaryNode->m_nodeName            =    pMessage->getPrimaryNodeName();
    pPrimaryNode->m_nodePort            =    pMessage->getPrimaryNodePort();

    pCluster->m_clusterCreated  = 1;
    /* Update primary and local list in centeral cluster config. */

    (CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_primaryNode = NodeManagedObject (this, 0, pMessage->getPrimaryNodeName (), pMessage->getPrimaryNodePort());

    // Update the self node details as well.  The ip address might have changed if we have multiple eth interfaces, depending on what ip was used in the cluster creation request.

    (CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_selfNode = NodeManagedObject (this, 0, pMessage->getThisNodeName (), pMessage->getThisNodePort ());

    // Set the information about the secondary nodes in the cluster.

    (CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_nSecondaryNodes  =  pMessage->getNSecondaryNodes ();

    // remove all secondaries and add the new ones.
    (CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_nodeList.clear ();
    for (i = 0; i < pMessage->getNSecondaryNodes (); i++)
    {
        (CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_nodeList.push_back (NodeManagedObject (this, 0, pMessage->getSecondaryNodeName (i), pMessage->getSecondaryNodePort(i)));
    }

    pPrimaryNode->setNodeGenericStatus (NODE_COMMUNICATED);

    // Update the ipaddress information for the HeartBeatObjectManager.  The ip address might have changed if we have multiple eth interfaces, depending on what ip was used in the cluster creation request.

    (HeartBeatObjectManager::getInstance ())->setThisLocationIpAddr (pMessage->getThisNodeName ());

    /* start heart beat */
//    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();
    sendLocalHeartBeat ();

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);

//    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.lock ();
    (CentralClusterConfigObjectManager::getInstance ())->resetManagmentOperationStatus ();
    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();

    trace (TRACE_LEVEL_DEVEL, "LocalClusterConfigObjectManager::reportPrimaryNodeChanged : Exit");
    return;
}

void LocalClusterConfigObjectManager::reportRemovedNodeFromCluster (LocalClusterConfigObjectManagerReportRemovedNodeFromClusterMessage *pMessage)
{
    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.lock ();

    ClusterManagedObject *pCluster  = &(CentralClusterConfigObjectManager::getInstance ())->m_cluster;
    NodeManagedObject *pPrimaryNode = &(CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_primaryNode;

    if (MANAGMENT_OPERATION_IN_PROGRESS == (CentralClusterConfigObjectManager::getInstance ())->getManagmentOperationStatus () )
    {
        trace (TRACE_LEVEL_ERROR, "LocalClusterConfigObjectManager::reportRemovedNodeFromCluster: Busy process other mgm command!");
        pMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
        reply (pMessage);
        (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();
        return;
    }

    (CentralClusterConfigObjectManager::getInstance ())->setManagmentOperationStatus ();

    StopHeartBeatMessage   *pStopHeartBeatMessage;

    pStopHeartBeatMessage = new StopHeartBeatMessage ();
    pStopHeartBeatMessage->setDstIpAddress (pPrimaryNode->m_nodeName);
    pStopHeartBeatMessage->setDstPortNumber (pPrimaryNode->m_nodePort);
    ResourceId status = sendSynchronously (pStopHeartBeatMessage);

    if ((WAVE_MESSAGE_SUCCESS != status) || (WAVE_MESSAGE_SUCCESS != pStopHeartBeatMessage->getCompletionStatus ()))
    {
        trace (TRACE_LEVEL_WARN, string ("LocalClusterConfigObjectManager::reportLostHeartBeat : failed to process StopHeartBeatMessage with Primary : ") + pPrimaryNode->m_nodeName + string (":") + pPrimaryNode->m_nodePort + string (", status : ") + FrameworkToolKit::localize(status));
    }

    delete (pStopHeartBeatMessage);
#if 0
    if (NULL != (CentralClusterConfigObjectManager::getInstance ())->m_heartBeatLostCallBack)
    {
        char    nodeName [MAX_IP_STRING_LENGTH];
        strncpy (nodeName, pPrimaryNode->m_nodeName.c_str (), MAX_IP_STRING_LENGTH);
        (CentralClusterConfigObjectManager::getInstance ())->m_heartBeatLostCallBack (APP_CLUSTER_CLUSTER_DELETED, nodeName, pPrimaryNode->m_nodePort);
    }
#endif

    pCluster->m_primaryNode.m_nodeName    = FrameworkToolKit::getThisLocationIpAddress ();
    pCluster->m_primaryNode.m_nodePort    = FrameworkToolKit::getThisLocationPort ();
    pPrimaryNode->setNodeGenericStatus (NODE_CREATED);
    pCluster->m_clusterCreated  = 0;

    (CentralClusterConfigObjectManager::getInstance ())->resetManagmentOperationStatus ();
    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);

}

void LocalClusterConfigObjectManager::sendLocalHeartBeat ()
{
    trace (TRACE_LEVEL_DEVEL, "LocalClusterConfigObjectManager::sendLocalHeartBeat : Entering ...");
//    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.lock ();
    NodeManagedObject      *pPrimaryNode = &(CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_primaryNode;
    StartHeartBeatMessage  *pMessage;

    pMessage = new StartHeartBeatMessage ();
    pMessage->setDstIpAddress (pPrimaryNode->m_nodeName);
    pMessage->setDstPortNumber (pPrimaryNode->m_nodePort);
    pMessage->setHeartBeatInterval (CENTERAL_CLUSTER_HEART_BEAT_PERIOD);
    pMessage->setHeartBeatFailureThreshold (MAX_LOST_HEARTBEAT);
//    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();
    ResourceId status = sendSynchronously (pMessage);

    if ((WAVE_MESSAGE_SUCCESS != status) || (WAVE_MESSAGE_SUCCESS != pMessage->getCompletionStatus ()))
    {
        trace (TRACE_LEVEL_WARN, string ("LocalClusterConfigObjectManager::sendLocalHeartBeat: failed to process StopHeartBeatMessage to Primary : ") + pPrimaryNode->m_nodeName + string (":") + pPrimaryNode->m_nodePort + string ("status : ") + FrameworkToolKit::localize(status));
    }

    delete (pMessage);
    trace (TRACE_LEVEL_DEVEL, "LocalClusterConfigObjectManager::sendLocalHeartBeat : Exit ...");

}


UI32 LocalClusterConfigObjectManager::informFrameworkLostHearBeat(string ipAddress, UI32 port)
{
    FrameworkObjectManagerLostHeartBeatMessage *pFrameworkLostHeartBeatMessage = new FrameworkObjectManagerLostHeartBeatMessage ();
    UI32    status;

    /* Inform framework to add node. */

    pFrameworkLostHeartBeatMessage->addNodeIpAddressAndPort (ipAddress, port);

    status = send (pFrameworkLostHeartBeatMessage, reinterpret_cast<PrismMessageResponseHandler> (&CentralClusterConfigObjectManager::processFrameworkHeartBeatLostReply), NULL);

    return (status);

}

void LocalClusterConfigObjectManager::processFrameworkHeartBeatLostReply (FrameworkStatus frameworkStatus, FrameworkObjectManagerLostHeartBeatMessage *pFrameworkLostHeartBeatMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "LocalClusterConfigObjectManager::processFrameworkHeartBeatLostReply : Entering ...");

    if ((FRAMEWORK_SUCCESS != frameworkStatus) || (WAVE_MESSAGE_SUCCESS != pFrameworkLostHeartBeatMessage->getCompletionStatus()))
    {
        trace (TRACE_LEVEL_ERROR, string ("LocalClusterConfigObjectManager::processFrameworkHeartBeatLostReply : FrameworkStatus : ") + frameworkStatus + string (", Message Status : ") + pFrameworkLostHeartBeatMessage->getCompletionStatus ());
        delete (pFrameworkLostHeartBeatMessage);
        return;
    }
    delete (pFrameworkLostHeartBeatMessage);
}



void LocalClusterConfigObjectManager::getNodeRoleHandler (LocalClusterConfigObjectManagerGetNodeRoleMessage *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, "LocalClusterConfigObjectManager::getNodeRoleHandler : Entering ...");
    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.lock ();

    ClusterManagedObject    *m_cluster = &(CentralClusterConfigObjectManager::getInstance ())->m_cluster;


    pMessage->setNodeRole (m_cluster->getThisNodeRole ());

    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
    trace (TRACE_LEVEL_DEVEL, "LocalClusterConfigObjectManager::getNodeRoleHandler : Exit ...");
}

void LocalClusterConfigObjectManager::getClusterConfigHandler (LocalClusterConfigObjectManagerGetClusterConfigMessage *pMessage)
{
    UI32                     index;
    trace (TRACE_LEVEL_DEVEL, "LocalClusterConfigObjectManager::getClusterConfigHandler : Entering ...");
    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.lock ();

    ClusterManagedObject    *m_cluster = &(CentralClusterConfigObjectManager::getInstance ())->m_cluster;

    SI32  nodePort = m_cluster->m_primaryNode.getNodePort ();
    pMessage->setPrimaryNodeName (m_cluster->m_primaryNode.getNodeName ());
    pMessage->setPrimaryNodePort (nodePort);

    if (m_cluster->m_primaryNode.getNodeGenericStatus () == NODE_FAILED)
    {
        pMessage->setPrimaryNodeStatus (APP_CLUSTER_NODE_FAILED);
    }
    else
    {
        pMessage->setPrimaryNodeStatus (APP_CLUSTER_NODE_ACTIVE);
    }

//    cout << "getClusterConfigHandler: nSecondaryNodes" << m_cluster->m_nSecondaryNodes << endl;
    pMessage->setNSecondaryNodes (m_cluster->m_nSecondaryNodes);

    for (index = 0; index < m_cluster->m_nSecondaryNodes; index++)
    {
        pMessage->setSecondaryNodeStatus (m_cluster->m_nodeList[index].getNodeName (),
                                          m_cluster->m_nodeList[index].getNodePort (),
                                          m_cluster->m_nodeList[index].getNodeGenericStatus ());
    }

    //cout << "role: " << m_cluster->getThisNodeRole () << endl;
    if (m_cluster->getThisNodeRole () ==  SECONDARY_NODE)
    {
//          cout <<  "getClusterConfigHandler: This is Secondary Node" << endl;
          pMessage->setNSecondaryNodes (m_cluster->m_nSecondaryNodes + 1);
          pMessage->setSecondaryNodeStatus (m_cluster->m_selfNode.getNodeName (),
                                            m_cluster->m_selfNode.getNodePort (),
                                            m_cluster->m_selfNode.getNodeGenericStatus ());
    }


    if (m_cluster->m_clusterCreated == 0)
    {
        pMessage->setCompletionStatus (APP_CLUSTER_NO_CLUSTER_CREATED);
    }
    else
    {
        pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }

    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();
    reply (pMessage);
    trace (TRACE_LEVEL_DEVEL, "LocalClusterConfigObjectManager::getClusterConfigHandler : Exit ...");
}


int LocalClusterConfigObjectManager::getSwitchHaConfig (haNodeStatus_t *primaryNodes, int *nSecondaryNodes, haNodeStatus_t *secondaryNodes)
{
    UI32                     index;
    (CentralClusterConfigObjectManager::getInstance ())->trace (TRACE_LEVEL_DEVEL, "LocalClusterConfigObjectManager::getSwitchHaConfig : Entering ...");
    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.lock ();

    ClusterManagedObject    *m_cluster = &(CentralClusterConfigObjectManager::getInstance ())->m_cluster;

    string nodeName     =  m_cluster->m_primaryNode.getNodeName ();
    UI32   nodeNameSize = nodeName.size ();

    WaveNs::prismAssert (MAX_IP_STRING_LENGTH >= nodeNameSize + 1, __FILE__, __LINE__); // Make sure that the trailing '\0' can also be put into the string.

    memcpy (primaryNodes->nodeName, nodeName.c_str (), nodeNameSize);
    primaryNodes->nodeName[nodeNameSize] = '\0';

    primaryNodes->nodePort = m_cluster->m_primaryNode.getNodePort ();

    if (m_cluster->m_primaryNode.getNodeGenericStatus () == NODE_FAILED)
    {
        primaryNodes->nodeStatus =  APP_CLUSTER_NODE_FAILED;
    }
    else
    {
        primaryNodes->nodeStatus =  APP_CLUSTER_NODE_ACTIVE;
    }

    if ( ((UI32) *nSecondaryNodes) > m_cluster->m_nSecondaryNodes)
    {
        *nSecondaryNodes =  (SI32) m_cluster->m_nSecondaryNodes;
    }

    for (index = 0; index < ((UI32)*nSecondaryNodes); index++)
    {
        nodeName     = m_cluster->m_nodeList[index].getNodeName ();
        nodeNameSize = nodeName.size ();

        WaveNs::prismAssert (MAX_IP_STRING_LENGTH >= nodeNameSize + 1, __FILE__, __LINE__); // Make sure that the trailing '\0' can also be put into the string.

        memcpy (&(secondaryNodes ->nodeName), nodeName.c_str (), nodeNameSize);
        secondaryNodes ->nodeName[nodeNameSize] = '\0';
        secondaryNodes ->nodePort = m_cluster->m_nodeList[index].getNodePort ();
        secondaryNodes ->nodeStatus = m_cluster->m_nodeList[index].getNodeGenericStatus ();
    }

    //cout << "role: " << m_cluster->getThisNodeRole () << endl;
    if (m_cluster->getThisNodeRole () ==  SECONDARY_NODE)
    {
        *nSecondaryNodes =  m_cluster->m_nSecondaryNodes + 1;
        nodeName     =  m_cluster->m_selfNode.getNodeName ();
        nodeNameSize = nodeName.size ();

        WaveNs::prismAssert (MAX_IP_STRING_LENGTH >= nodeNameSize + 1, __FILE__, __LINE__); // Make sure that the trailing '\0' can also be put into the string.

        memcpy (&(secondaryNodes ->nodeName), nodeName.c_str (), nodeNameSize);
        secondaryNodes ->nodeName[nodeNameSize] = '\0';
        secondaryNodes ->nodePort = m_cluster->m_selfNode.getNodePort ();
        secondaryNodes ->nodeStatus = m_cluster->m_selfNode.getNodeGenericStatus ();
    }

    if (m_cluster->m_clusterCreated == 0)
    {
        (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();
        return (APP_CLUSTER_NO_CLUSTER_CREATED);
    }
    else
    {
        (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();
        return (WAVE_MESSAGE_SUCCESS);
    }
}


void LocalClusterConfigObjectManager::showStatistic()
{
//ShowHeartBeatStatMessage    *pMessage = new ShowHeartBeatStatMessage ();

//    sendSynchronously (pMessage);
    (CentralClusterConfigObjectManager::getInstance ())->m_cluster.showStatistic();
}

void LocalClusterConfigObjectManager::reportLostHeartBeat (HeartBeatLostMessage *pMessage)
{
    trace (TRACE_LEVEL_ERROR, "LocalClusterConfigObjectManager::reportLostHeartBeat : Entering ...");
    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.lock ();

    NodeManagedObject     *pPrimaryNode = &(CentralClusterConfigObjectManager::getInstance ())->m_cluster.m_primaryNode;
    ClusterManagedObject  *pCluster     = &(CentralClusterConfigObjectManager::getInstance ())->m_cluster;
    string                 nodeName      = pMessage->getDstIpAddress ();
    UI32                   nodePort      = pMessage->getDstPort ();

    StopHeartBeatMessage   *pStopHeartBeatMessage;

    pStopHeartBeatMessage = new StopHeartBeatMessage ();
    pStopHeartBeatMessage->setDstIpAddress (nodeName);
    pStopHeartBeatMessage->setDstPortNumber (nodePort);
    ResourceId status = sendSynchronously (pStopHeartBeatMessage);

    if ((WAVE_MESSAGE_SUCCESS != status) || (WAVE_MESSAGE_SUCCESS != pStopHeartBeatMessage->getCompletionStatus ()))
    {
        trace (TRACE_LEVEL_WARN, string ("LocalClusterConfigObjectManager::reportLostHeartBeat : failed to send StopHeartBeatMessage to ") + nodeName + string (":") + nodePort + string ("status : ") + FrameworkToolKit::localize(status));
    }

    delete pStopHeartBeatMessage;

#if 0
    if (NULL != (CentralClusterConfigObjectManager::getInstance ())->m_heartBeatLostCallBack)
    {
        char    nodeName [MAX_IP_STRING_LENGTH];
        strncpy (nodeName, dstNode.c_str (), MAX_IP_STRING_LENGTH);
        (CentralClusterConfigObjectManager::getInstance ())->m_heartBeatLostCallBack (APP_CLUSTER_HEARTBEAT_LOST, nodeName, nodePort);
    }
#endif
           // DMM Specific
           // Delete node from Cluster
           // inform Framework
    informFrameworkLostHearBeat (nodeName, nodePort);
#if 1
    /* reset node configuration */
    pCluster->m_nSecondaryNodes = 0;
    pCluster->m_clusterCreated  = 0;

    /* Threre is no hearbeat running now. We will not start any other timers.*/

    pPrimaryNode->m_nodeName    = FrameworkToolKit::getThisLocationIpAddress ();
    pPrimaryNode->m_nodePort    = FrameworkToolKit::getThisLocationPort ();
    pPrimaryNode->setNodeGenericStatus (NODE_CREATED);
#endif


    (CentralClusterConfigObjectManager::getInstance ())->m_mutex.unlock ();

    return;
          // END DMM Specific.
}

}
