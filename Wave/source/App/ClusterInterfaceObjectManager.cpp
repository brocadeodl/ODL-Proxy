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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#include "App/ClusterInterfaceObjectManager.h"
#include "Cluster/LocalClusterConfigObjectManager.h"
#include "Cluster/ClusterMessages.h"
#include "Cluster/Local/HeartBeat/HeartBeatMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "App/AppInterfaceObjectManager.h"
#include "App/AppObjectManager.h"
#include "App/AppMessages.h"
#include <string>
#include <vector>

namespace WaveNs
{

ClusterInterfaceObjectManager::ClusterInterfaceObjectManager ()
    : WaveLocalObjectManager ("Cluster Interface")
{
}


ClusterInterfaceObjectManager *ClusterInterfaceObjectManager::getInstance ()
{
    static ClusterInterfaceObjectManager *pClusterInterfaceObjectManager = NULL;

    if (NULL == pClusterInterfaceObjectManager)
    {
        pClusterInterfaceObjectManager = new ClusterInterfaceObjectManager ();
        WaveNs::prismAssert (NULL != pClusterInterfaceObjectManager, __FILE__, __LINE__);
    }

    return (pClusterInterfaceObjectManager);
}

ClusterInterfaceObjectManager::~ClusterInterfaceObjectManager ()
{

}

PrismServiceId ClusterInterfaceObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

int ClusterInterfaceObjectManager::createCluster (SI32 appId, int nSecondaryNodes, node_t secondaryNodes [], createClusterCallBack_t  createClusterResultCB, void *pContext)
{
    int                                         i;
    WaveMessageStatus                          status = WAVE_MESSAGE_ERROR;
    ClusterObjectManagerCreateClusterMessage   *pMessage = new ClusterObjectManagerCreateClusterMessage ();
    ClusterInterafceData                       *pMessageContext = new ClusterInterafceData ();;

    trace ( TRACE_LEVEL_DEBUG, "Entering ClusterInterfaceObjectManager::createCluster");

    pMessage->setPrimaryNodeName (FrameworkToolKit::getThisLocationIpAddress ());
    pMessage->setNSecondaryNodes (nSecondaryNodes);
    pMessageContext->createClusterResultCB = createClusterResultCB;
    pMessageContext->pContext              = pContext;
    pMessageContext->appId                 = appId;

    //pMessage->setResultCB (createClusterResultCB);
    //pMessage->setContext (pContext);
    for (i = 0; i < nSecondaryNodes; i++)
    {
        pMessage->setSecondaryNodeNameAndPort (string (secondaryNodes[i].nodeName), secondaryNodes[i].nodePort);
    }

    status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&ClusterInterfaceObjectManager::createClusterCallBack), pMessageContext);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "ClusterInterfaceObjectManager::createCluster: Failed to send message to createCluster ");
        return (WAVE_MESSAGE_ERROR);
    }

    trace ( TRACE_LEVEL_DEBUG, "Exit ClusterInterfaceObjectManager::createCluster");
    return (WAVE_MESSAGE_SUCCESS);
}

void ClusterInterfaceObjectManager::createClusterCallBack (FrameworkStatus frameworkStatus, ClusterObjectManagerCreateClusterMessage *pMessage, void *pContext)
{
    trace ( TRACE_LEVEL_INFO, string ("ClusterInterfaceObjectManager::createClusterCallBack ClusterCreation Status: ") + pMessage->getCompletionStatus () + string (" frameworkStatus: ") + frameworkStatus);
    UI32                       i;
    ClusterInterafceData      *pMessageContext = reinterpret_cast<ClusterInterafceData *> (pContext);
    UI32                       status = WAVE_MESSAGE_ERROR;

    if (-1 == pMessageContext->appId)
    {
        NodeValidationResult_t     *pNodeStatus = new NodeValidationResult_t[pMessage->getNSecondaryNodes ()];
      if ((FRAMEWORK_SUCCESS == frameworkStatus) && (WAVE_MESSAGE_SUCCESS == pMessage->getCompletionStatus()))
        {
            status =  pMessage->getCompletionStatus();
            for (i = 0; i < pMessage->getNSecondaryNodes (); i++)
            {
                string nodeName = pMessage->getStatusNodeName (i);
                memcpy (pNodeStatus [i].nodeName, nodeName.c_str (), strlen (nodeName.c_str ()));
                pNodeStatus [i].nodePort   = (UI32) pMessage->getStatusNodePort (i);
                pNodeStatus [i].nodeStatus = (SI32) pMessage->getStatusNodeStatus (i);
                pNodeStatus [i].validationResultPtr = NULL;
                pNodeStatus [i].validationResultLen = 0;
                //pNodeStatus [i] =   *(pMessage->getNodeStatusAddr (i));
            }
        }
        else
        {
            status = APP_CLUSTER_FAILED_TO_COMMUNICATE_TO_ALL_NODES;
            for (i = 0; i < pMessage->getNSecondaryNodes (); i++)
            {
                string nodeName = pMessage->getSecondaryNodeName (i);
                memcpy (pNodeStatus [i].nodeName, nodeName.c_str (), strlen (nodeName.c_str ()));
                pNodeStatus [i].nodePort   = (UI32) pMessage->getSecondaryNodePort (i);
                pNodeStatus [i].nodeStatus = APP_CLUSTER_FAILED_COMMUNICATE_TO_NODE;
                pNodeStatus [i].validationResultPtr = NULL;
                pNodeStatus [i].validationResultLen = 0;
            }
        }
        (pMessageContext->createClusterResultCB ) (pMessageContext->pContext, status,  pMessage->getNSecondaryNodes (), pNodeStatus);
        delete[] pNodeStatus;
    }
    else
    {
        trace ( TRACE_LEVEL_DEBUG, "ClusterInterfaceObjectManager::createClusterCallBack ");
        ClusterInterfaceObjectManagerCreateClusterDoneMessage  *pCreateClusterDoneMessage = new ClusterInterfaceObjectManagerCreateClusterDoneMessage ((AppObjectManager::getAppInterfaceObjectManager (pMessageContext->appId))->getPrismServiceId ());
        pCreateClusterDoneMessage->setOperationStatus (pMessage->getCompletionStatus ());
        pCreateClusterDoneMessage->setNSecondaryNodes (pMessage->getNSecondaryNodes ());
        for (i = 0; i < pMessage->getNSecondaryNodes (); i++)
        {
            pCreateClusterDoneMessage->setNodeStaus(pMessage->getStatusNodeName (i), pMessage->getStatusNodePort (i), pMessage->getStatusNodeStatus (i));
        }
        pCreateClusterDoneMessage->setCreateClusterResultCallBack (pMessageContext->createClusterResultCB);
        pCreateClusterDoneMessage->setContext (pMessageContext->pContext);
        if ( WAVE_MESSAGE_SUCCESS != sendSynchronously (pCreateClusterDoneMessage) )
        {
            trace ( TRACE_LEVEL_DEBUG, "ClusterInterfaceObjectManager::createClusterCallBack failed to send createCluster Done Message.");
        }
        delete (pCreateClusterDoneMessage);
    }
    delete (pMessageContext);
    delete (pMessage);
}


int ClusterInterfaceObjectManager::addNodeToCluster (SI32 appId, node_t node, addNodeCallBack_t  addNodeResultCB, void *pContext)
{
    WaveMessageStatus                   status = WAVE_MESSAGE_ERROR;
    ClusterObjectManagerAddNodeMessage  *pMessage = new ClusterObjectManagerAddNodeMessage ();
    ClusterInterafceData                *pMessageContext = new ClusterInterafceData ();;
    UI32                                 noNewNode = 1;

    pMessage->setPrimaryNodeName (FrameworkToolKit::getThisLocationIpAddress ());
    pMessage->setNSecondaryNodes (noNewNode);
    pMessage->setSecondaryNodeNameAndPort (string (node.nodeName), node.nodePort);
    pMessageContext->addNodeResultCB = addNodeResultCB;
    pMessageContext->pContext        = pContext;
    pMessageContext->appId           = appId;

//    pMessage->setResultCB (addNodeResultCB);
//    pMessage->setContext (pContext);

    status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&ClusterInterfaceObjectManager::addNodeCallBack), pMessageContext);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"ClusterInterfaceObjectManager::addNodeCallBack: Failed to send message to addNodeToCluster ");
        return (WAVE_MESSAGE_ERROR);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

void ClusterInterfaceObjectManager::addNodeCallBack (FrameworkStatus frameworkStatus, ClusterObjectManagerAddNodeMessage *pMessage, void *pContext)
{
    UI32 status = WAVE_MESSAGE_SUCCESS;
    ClusterInterafceData      *pMessageContext = reinterpret_cast<ClusterInterafceData *> (pContext);

    if (-1 == pMessageContext->appId)
    {

        if (FRAMEWORK_SUCCESS != frameworkStatus || (WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION == pMessage->getCompletionStatus()))
        {
            trace (TRACE_LEVEL_ERROR,"ClusterInterfaceObjectManager::addNodeCallBack Failed to add node");
            status = APP_CLUSTER_FAILED_COMMUNICATE_TO_NODE;
        }
        else
        {
            status = pMessage->getCompletionStatus();
        }

        (pMessageContext->addNodeResultCB) (pMessageContext->pContext, status);
    }
    else
    {
        ClusterInterfaceObjectManagerAddNodeDoneMessage  *pAddNodeDoneMessage = new ClusterInterfaceObjectManagerAddNodeDoneMessage ((AppObjectManager::getAppInterfaceObjectManager (pMessageContext->appId))->getPrismServiceId ());
        pAddNodeDoneMessage->setOperationStatus (pMessage->getCompletionStatus ());

        pAddNodeDoneMessage->setAddNodeResultCallBack (pMessageContext->addNodeResultCB);
        pAddNodeDoneMessage->setContext (pMessageContext->pContext);
        if( WAVE_MESSAGE_SUCCESS != sendSynchronously (pAddNodeDoneMessage) )
        {
            trace (TRACE_LEVEL_DEBUG,"ClusterInterfaceObjectManager::addNodeCallBack Failed to send addNode Done Message.");
        }
        delete (pAddNodeDoneMessage);
    }

    delete (pMessageContext);
    delete (pMessage);
}

int ClusterInterfaceObjectManager::joinNodeToCluster (SI32 appId, node_t node, joinNodeCallBack_t  joinNodeResultCB, void *pContext)
{
    WaveMessageStatus                     status          = WAVE_MESSAGE_ERROR;
    ClusterObjectManagerJoinNodeMessage   *pMessage        = new ClusterObjectManagerJoinNodeMessage ();
    ClusterInterafceData                  *pMessageContext = new ClusterInterafceData ();;

    pMessage->setNodeName (string (node.nodeName));
    pMessage->setNodePort (node.nodePort);
    pMessageContext->joinNodeResultCB = joinNodeResultCB;
    pMessageContext->pContext         = pContext;
    pMessageContext->appId            = appId;
    //pMessage->setResultCB (joinNodeResultCB);
    //pMessage->setContext (pContext);

    status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&ClusterInterfaceObjectManager::joinNodeCallBack), pMessageContext);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"ClusterInterfaceObjectManager::joinNodeToCluster Failed to send message to joinNodeToCluster ");
        return (WAVE_MESSAGE_ERROR);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

void ClusterInterfaceObjectManager::joinNodeCallBack (FrameworkStatus frameworkStatus, ClusterObjectManagerJoinNodeMessage *pMessage, void *pContext)
{
    UI32 status = WAVE_MESSAGE_SUCCESS;
    ClusterInterafceData      *pMessageContext = reinterpret_cast<ClusterInterafceData *> (pContext);

    if (-1 == pMessageContext->appId)
    {
        if (FRAMEWORK_SUCCESS != frameworkStatus || (WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION == pMessage->getCompletionStatus()))
        {
            trace (TRACE_LEVEL_ERROR,"ClusterInterfaceObjectManager::joinNodeCallBack Failed to join node");
            status = APP_CLUSTER_FAILED_COMMUNICATE_TO_NODE;
        }
        else
        {
            status = pMessage->getCompletionStatus();
        }

        (pMessageContext->joinNodeResultCB) (pMessageContext->pContext, status);
    }
    else
    {
        ClusterInterfaceObjectManagerJoinNodeDoneMessage  *pJoinNodeDoneMessage = new ClusterInterfaceObjectManagerJoinNodeDoneMessage ((AppObjectManager::getAppInterfaceObjectManager (pMessageContext->appId))->getPrismServiceId ());
        pJoinNodeDoneMessage->setOperationStatus (pMessage->getCompletionStatus ());

        pJoinNodeDoneMessage->setJoinNodeResultCallBack (pMessageContext->joinNodeResultCB);
        pJoinNodeDoneMessage->setContext (pMessageContext->pContext);
        if(WAVE_MESSAGE_SUCCESS != sendSynchronously (pJoinNodeDoneMessage))
        {
            trace (TRACE_LEVEL_DEBUG,"ClusterInterfaceObjectManager::joinNodeCallBack Failed to send Join node done Message.");
        }
        delete (pJoinNodeDoneMessage);
    }

    delete (pMessageContext);
    delete (pMessage);
}


int ClusterInterfaceObjectManager::removeNodeFromCluster (SI32 appId, node_t node, removeNodeCallBack_t  removeNodeResultCB, void *pContext)
{
    WaveMessageStatus                        status          = WAVE_MESSAGE_ERROR;
    ClusterObjectManagerDeleteNodeMessage    *pMessage        = new ClusterObjectManagerDeleteNodeMessage ();
    ClusterInterafceData                     *pMessageContext = new ClusterInterafceData ();;

    pMessage->setSecondaryNodeNameAndPort (string (node.nodeName), node.nodePort);
    pMessage->setNNodesToDelete (1);
    pMessageContext->removeNodeResultCB = removeNodeResultCB;
    pMessageContext->pContext           = pContext;
    pMessageContext->appId              = appId;
    //pMessage->setResultCB (removeNodeResultCB);
    //pMessage->setContext (pContext);

    status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&ClusterInterfaceObjectManager::removeNodeCallBack), pMessageContext);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"ClusterInterfaceObjectManager::removeNodeFromCluster Failed to send message to removeNodeFromCluster ");
        return (WAVE_MESSAGE_ERROR);
    }

    return (WAVE_MESSAGE_SUCCESS);
}


void ClusterInterfaceObjectManager::removeNodeCallBack (FrameworkStatus frameworkStatus, ClusterObjectManagerDeleteNodeMessage *pMessage, void *pContext)
{
    ClusterInterafceData   *pMessageContext = reinterpret_cast<ClusterInterafceData *> (pContext);
    UI32                    status = WAVE_MESSAGE_SUCCESS;

    if (-1 == pMessageContext->appId)
    {
        if (FRAMEWORK_SUCCESS != frameworkStatus || (WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION == pMessage->getCompletionStatus()))
        {
            trace (TRACE_LEVEL_ERROR,"ClusterInterfaceObjectManager::removeNodeCallBack Failed to delete node");
            status = APP_CLUSTER_FAILED_COMMUNICATE_TO_NODE;
        }
        else
        {
            status = pMessage->getCompletionStatus();
        }
        (pMessageContext->removeNodeResultCB) (pMessageContext->pContext, status);
    }
    else
    {
        ClusterInterfaceObjectManagerDeleteNodeDoneMessage  *pDeleteNodeDoneMessage = new ClusterInterfaceObjectManagerDeleteNodeDoneMessage ((AppObjectManager::getAppInterfaceObjectManager (pMessageContext->appId))->getPrismServiceId ());
        pDeleteNodeDoneMessage->setOperationStatus (pMessage->getCompletionStatus ());

        pDeleteNodeDoneMessage->setRemoveNodeResultCallBack (pMessageContext->removeNodeResultCB);
        pDeleteNodeDoneMessage->setContext (pMessageContext->pContext);
        if ( WAVE_MESSAGE_SUCCESS != sendSynchronously (pDeleteNodeDoneMessage))
        {
            trace (TRACE_LEVEL_DEBUG,"ClusterInterfaceObjectManager::removeNodeCallBack failed to send Delete node Done message.");
        }
        delete (pDeleteNodeDoneMessage);
    }

    delete (pMessageContext);
    delete (pMessage);
}



int ClusterInterfaceObjectManager::removeCluster (SI32 appId, removeClusterCallBack_t  removeClusterResultCB, void *pContext)
{
    WaveMessageStatus                         status          = WAVE_MESSAGE_ERROR;
    ClusterObjectManagerDeleteClusterMessage  *pMessage        = new ClusterObjectManagerDeleteClusterMessage ();
    ClusterInterafceData                      *pMessageContext = new ClusterInterafceData ();;

    pMessageContext->removeClusterResultCB = removeClusterResultCB;
    pMessageContext->pContext              = pContext;
    pMessageContext->appId                 = appId;
    //pMessage->setResultCB (removeClusterResultCB);
    //pMessage->setContext (pContext);

    status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&ClusterInterfaceObjectManager::removeClusterCallBack), pMessageContext);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"ClusterInterfaceObjectManager::removeCluster: Failed to send message to removeCluster ");
        return (WAVE_MESSAGE_ERROR);
    }

    return (WAVE_MESSAGE_SUCCESS);
}


void ClusterInterfaceObjectManager::removeClusterCallBack (FrameworkStatus frameworkStatus, ClusterObjectManagerDeleteClusterMessage *pMessage, void *pContext)
{
    ClusterInterafceData   *pMessageContext = reinterpret_cast<ClusterInterafceData *> (pContext);
    UI32                    status = WAVE_MESSAGE_SUCCESS;

    if (-1 == pMessageContext->appId)
    {
        if (FRAMEWORK_SUCCESS != frameworkStatus || (WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION == pMessage->getCompletionStatus()))
        {
            trace (TRACE_LEVEL_ERROR,"ClusterInterfaceObjectManager::removeClusterCallBack Failed to delete node");
            status = APP_CLUSTER_FAILED_COMMUNICATE_TO_NODE;
        }
        else
        {
            status = pMessage->getCompletionStatus();
        }
        (pMessageContext->removeClusterResultCB) (pMessageContext->pContext, status);
    }
    else
    {
        ClusterInterfaceObjectManagerDeleteClusterDoneMessage  *pDeleteClusterDoneMessage = new ClusterInterfaceObjectManagerDeleteClusterDoneMessage ((AppObjectManager::getAppInterfaceObjectManager (pMessageContext->appId))->getPrismServiceId ());
        pDeleteClusterDoneMessage->setOperationStatus (pMessage->getCompletionStatus ());

        pDeleteClusterDoneMessage->setRemoveClusterResultCallBack (pMessageContext->removeClusterResultCB);
        pDeleteClusterDoneMessage->setContext (pMessageContext->pContext);
        if(WAVE_MESSAGE_SUCCESS != sendSynchronously (pDeleteClusterDoneMessage) ) 
        {
            trace (TRACE_LEVEL_DEBUG,"ClusterInterfaceObjectManager::removeClusterCallBack Failed to send DeleteClusterDone message.");
        }
        delete (pDeleteClusterDoneMessage);
    }

    delete (pMessageContext);
    delete (pMessage);
}


#if 0
int ClusterInterfaceObjectManager::registerHeartBeatCallBack (heartBeatCallBack_t    heartBeatCB)
{

    WaveMessageStatus                                                  status = WAVE_MESSAGE_ERROR;
    LocalClusterConfigObjectManagerRegisterHeartBeatLossCBMessage       message;

    // FIXME Store the call back here and send a message from clutser service when heartbeat loss
    // is detected. Call the callback funcation from the interface thread.

    message.setHeartBeatLostCallBack (heartBeatCB);
    status = sendSynchronously (&message);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"Failed to send message to removeCluster ");
        return WAVE_MESSAGE_ERROR;
    }

    return (message.getCompletionStatus ());
}

int ClusterInterfaceObjectManager::registerMemberJoinedCallBack (memberJoinedCallBack_t memberJoinedCB)
{
    WaveMessageStatus                                                    status = WAVE_MESSAGE_ERROR;
    LocalClusterConfigObjectManagerRegisterMemberJoinedCBMessage        message;

    // FIXME Store the call back here and send a message from clutser service when memeber join the
    // cluster. Call the callback funcation from the interface thread.

    message.setMemberJoinedCallBack (memberJoinedCB);
    status = sendSynchronously (&message);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"Failed to send message to removeCluster ");
        return WAVE_MESSAGE_ERROR;
    }

    return (message.getCompletionStatus ());
}
#endif

int ClusterInterfaceObjectManager::setHeartBeatConfig (int heartBeatInterval, int nLostHeartBeatsToNotify)
{
    WaveMessageStatus        status = WAVE_MESSAGE_ERROR;
    ConfigHeartBeatMessage    message;

    message.setHeartBeatInterval (heartBeatInterval);
    message.setHeartBeatFailureThreshold (nLostHeartBeatsToNotify);

    status = sendSynchronously (&message);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"ClusterInterfaceObjectManager::setHeartBeatConfig Failed to send message to setHeartBeatConfig ");
        return WAVE_MESSAGE_ERROR;
    }

    return (message.getCompletionStatus ());
}

int ClusterInterfaceObjectManager::getSwitchHaRole (int *nodeRole)
{
    WaveMessageStatus                                    status = WAVE_MESSAGE_ERROR;
    LocalClusterConfigObjectManagerGetNodeRoleMessage   message;

    status = sendSynchronously (&message);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"ClusterInterfaceObjectManager::getSwitchHaRole Failed to send message to getSwitchHaRole ");
        return WAVE_MESSAGE_ERROR;
    }

    *nodeRole   =   message.getNodeRole ();
    return (message.getCompletionStatus ());
}

int ClusterInterfaceObjectManager::getSwitchHaConfig (haNodeStatus_t *primaryNodes, int *nSecondaryNodes, haNodeStatus_t *secondaryNodes)
{
    UI32                                                   index;
    WaveMessageStatus                                     status = WAVE_MESSAGE_ERROR;
    LocalClusterConfigObjectManagerGetClusterConfigMessage message;
    UI32                                                   nNodes = *nSecondaryNodes;

    status = sendSynchronously (&message);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"ClusterInterfaceObjectManager::getSwitchHaConfig Failed to send message to getSwitchHaRole ");
        return WAVE_MESSAGE_SUCCESS;
    }

    string nodeName  =  message.getPrimaryNodeName ();
    memcpy (primaryNodes->nodeName, nodeName.c_str (), MAX_IP_STRING_LENGTH);
    primaryNodes->nodePort = message.getPrimaryNodePort ();
    primaryNodes->nodeStatus =  message.getPrimaryNodeStatus ();

    if (message.getNSecondaryNodes () < nNodes)
    {
        nNodes  = message.getNSecondaryNodes ();
    }
    *nSecondaryNodes =  message.getNSecondaryNodes ();

    for (index = 0; index < nNodes; index++)
    {
        nodeName  =  message.getSecondaryNodeName (index);
        memcpy (&(secondaryNodes ->nodeName), nodeName.c_str (), MAX_IP_STRING_LENGTH);
        secondaryNodes ->nodePort = message.getSecondaryNodePort (index);
        secondaryNodes ->nodeStatus = message.getSecondaryNodeStatus (index);
        secondaryNodes++;
    }
    return (message.getCompletionStatus ());
}


int  ClusterInterfaceObjectManager::getLocalIpAddr (ipString localIpAddr)
{
    string ipAddress     = FrameworkToolKit::getThisLocationIpAddress ();
    UI32   ipAddressSize = ipAddress.size ();

    prismAssert (MAX_IP_STRING_LENGTH >= ipAddressSize + 1, __FILE__, __LINE__);

    memcpy (localIpAddr, ipAddress.c_str (), ipAddressSize);
    localIpAddr[ipAddressSize] = '\0';

    return (WAVE_MESSAGE_SUCCESS);
}


UI32 ClusterInterfaceObjectManager::getClusterNodes (vector <Node> *nodeList)
{
    UI32                                                   index;
    WaveMessageStatus                                     status = WAVE_MESSAGE_ERROR;
    LocalClusterConfigObjectManagerGetClusterConfigMessage message;

    trace (TRACE_LEVEL_DEBUG, "ClusterInterfaceObjectManager::getClusterNodes: Enter");
    status = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"ClusterInterfaceObjectManager::getClusterNodes Failed to send message to getSwitchHaRole ");
        return APP_CLUSTER_ERROR;
    }

    Node node;
    node.m_nodeName  =  message.getPrimaryNodeName ();
    node.m_nodePort  =  message.getPrimaryNodePort ();

    nodeList->push_back (node);
    //cout << "nSecondaryNodes: " << message.getNSecondaryNodes () << endl;

    for (index = 0; index < message.getNSecondaryNodes (); index++)
    {
        node.m_nodeName  =  message.getSecondaryNodeName (index);
        node.m_nodePort  =  message.getSecondaryNodePort (index);
        nodeList->push_back (node);
    }
    return (message.getCompletionStatus());
}

}
