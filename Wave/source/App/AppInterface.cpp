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

#include "Cluster/LocalClusterConfigObjectManager.h"
#include "App/AppInterfaceObjectManager.h"
#include "App/ClusterInterfaceObjectManager.h"
#include "App/AppObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

extern "C" int createCluster (int appId, int nSecondaryNodes, node_t secondaryNodes [], createClusterCallBack_t  createClusterResultCB, void *pContext)
{
    return ((ClusterInterfaceObjectManager::getInstance ())->createCluster (appId, nSecondaryNodes, secondaryNodes, createClusterResultCB, pContext));
}

extern "C" int createClusterWithCallBack (int appId, int nSecondaryNodes, ipString secondaryNodes [], createClusterCallBack_t  createClusterResultCB, void *pContext)
{
    node_t       *nodes = new node_t[nSecondaryNodes];
    int          i;

    for (i = 0; i < nSecondaryNodes; i++)
    {
        memcpy (nodes [i].nodeName, secondaryNodes [i], strlen (secondaryNodes [i]) + 1);
        nodes [i].nodePort = FrameworkToolKit::getPrismDefaultTcpPort ();
    }
    int result = (ClusterInterfaceObjectManager::getInstance ())->createCluster (appId, nSecondaryNodes, nodes, createClusterResultCB, pContext);

    delete[] nodes;

	return (result);
}

extern "C" int addNodeToClusterWithCallBack (int appId, ipString ipAddr, addNodeCallBack_t  addNodeResultCB, void *pContext, void *pValidationData, unsigned int validationDataLen)
{
    node_t       node;

    memcpy (node.nodeName, ipAddr, strlen (ipAddr) + 1);
    node.nodePort = FrameworkToolKit::getPrismDefaultTcpPort ();

    return ((ClusterInterfaceObjectManager::getInstance ())->addNodeToCluster (appId, node, addNodeResultCB, pContext));
}

extern "C" int addNodeToCluster (int appId, node_t node, addNodeCallBack_t  addNodeResultCB, void *pContext, void *pValidationData, unsigned int validationDataLen)
{
    return ((ClusterInterfaceObjectManager::getInstance ())->addNodeToCluster (appId, node, addNodeResultCB, pContext));
}

extern "C" int joinNodeToClusterWithCallBack (int appId, node_t node, joinNodeCallBack_t  joinNodeResultCB, void *pContext, void *pValidationData, unsigned int validationDataLen)
{
    return ((ClusterInterfaceObjectManager::getInstance ())->joinNodeToCluster (appId, node, joinNodeResultCB, pContext));
}

extern "C" int removeNodeFromClusterWithCallBack (int appId, ipString ipAddr, removeNodeCallBack_t  removeNodeResultCB, void *pContext)
{
    node_t       node;

    memcpy (node.nodeName, ipAddr, strlen (ipAddr) + 1);
    node.nodePort = FrameworkToolKit::getPrismDefaultTcpPort ();

    return ((ClusterInterfaceObjectManager::getInstance ())->removeNodeFromCluster (appId, node, removeNodeResultCB, pContext));
}

extern "C" int removeNodeFromCluster (int appId, node_t node, removeNodeCallBack_t  removeNodeResultCB, void *pContext)
{
    return ((ClusterInterfaceObjectManager::getInstance ())->removeNodeFromCluster (appId, node, removeNodeResultCB, pContext));
}

extern "C" int removeClusterWithCallBack (int appId, removeClusterCallBack_t  removeClusterResultCB, void *pContext)
{
    return ((ClusterInterfaceObjectManager::getInstance ())->removeCluster (appId, removeClusterResultCB, pContext));
}

extern "C" int registerClusterValidationHandler (int appId, clusterValidationHandler_t  clusterValidationHandler)
{
    if (true == AppObjectManager::validateAppId (appId))
    {
        (AppObjectManager::getAppInterfaceObjectManager (appId))->registerValidationHandler (clusterValidationHandler);
        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        return (APP_CLUSTER_INVALID_APP_ID);
    }
}

extern "C" int registerClusterCollectValidationHandler (int appId, clusterCollectValidationHandler_t  clusterCollectValidationHandler)
{
    if (true == AppObjectManager::validateAppId (appId))
    {
        (AppObjectManager::getAppInterfaceObjectManager (appId))->registerCollectValidationHandler (clusterCollectValidationHandler);
        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        return (APP_CLUSTER_INVALID_APP_ID);
    }
}

extern "C" int registerClusterReportValidationHandler (int appId, clusterReportValidationHandler_t  clusterReportValidationHandler)
{
    if (true == AppObjectManager::validateAppId (appId))
    {
        (AppObjectManager::getAppInterfaceObjectManager (appId))->registerReportValidationHandler (clusterReportValidationHandler);
        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        return (APP_CLUSTER_INVALID_APP_ID);
    }
}

extern "C" int registerNodeJoinedHandler (int appId, memberJoinedCallBack_t nodeJoinedCallBack)
{
    if (true == AppObjectManager::validateAppId (appId))
    {
        (AppObjectManager::getAppInterfaceObjectManager (appId))->registerNodeJoinedHandler (nodeJoinedCallBack);
        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        return (APP_CLUSTER_INVALID_APP_ID);
    }
}

extern "C" int registerHeartbeatLossHandler (int appId, heartBeatCallBack_t heartBeatLossCallBack)
{
    if (true == AppObjectManager::validateAppId (appId))
    {
        (AppObjectManager::getAppInterfaceObjectManager (appId))->registerHeartbeatLossHandler (heartBeatLossCallBack);
        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        return (APP_CLUSTER_INVALID_APP_ID);
    }
}
extern "C" int setHeartBeatConfig (int heartBeatInterval, int nLostHeartBeatsToNotify)
{
    return ((ClusterInterfaceObjectManager::getInstance ())->setHeartBeatConfig (heartBeatInterval, nLostHeartBeatsToNotify));
}

extern "C" int getSwitchHaRole (int *nodeRole)
{
    return ((ClusterInterfaceObjectManager::getInstance ())->getSwitchHaRole (nodeRole));
}

extern "C" int reportValidationDetails (void *pContext, UI32 validationStatus, UI32 validationLen, void *validationPtr)
{
    MessageAndContext           *pMessageAndContext        = reinterpret_cast <MessageAndContext *> (pContext);
    AppInterfaceObjectManager   *appInterfaceObjectManager = AppObjectManager::getAppInterfaceObjectManagerForServiceId (pMessageAndContext->m_appId);

    if (NULL != appInterfaceObjectManager)
    {
        appInterfaceObjectManager->clusterCollectValidationReply (pContext, validationStatus, validationLen, validationPtr);
        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string ("reportValidationDetails: Could find Interface object Manager. pContext: ") + (unsigned long int) pContext + string (" App ID: ") + pMessageAndContext->m_appId) ;
        return (APP_CLUSTER_INVALID_APP_ID);
    }
}

extern "C" int reportValidationResults (void *pContext, UI32 validationStatus, UI32 resultLen, void *resultPtr)
{
    MessageAndContext           *pMessageAndContext        = reinterpret_cast <MessageAndContext *> (pContext);
    AppInterfaceObjectManager   *appInterfaceObjectManager = AppObjectManager::getAppInterfaceObjectManagerForServiceId (pMessageAndContext->m_appId);
    if (NULL != appInterfaceObjectManager)
    {
        appInterfaceObjectManager->clusterValidationReply (pContext, validationStatus, resultLen, resultPtr);
        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string ("reportValidationResults: Could find Interface object Manager. pContext: ") + (unsigned long int) pContext + string (" App ID: ") + pMessageAndContext->m_appId) ;
        return (APP_CLUSTER_INVALID_APP_ID);
    }
}

extern "C" int  getLocalIpAddr (ipString localIpAddr)
{
    return ((ClusterInterfaceObjectManager::getInstance ())->getLocalIpAddr (localIpAddr));
}

extern "C" int getSwitchHaConfig (haNodeStatus_t *primaryNodes, int *nSecondaryNodes, haNodeStatus_t *secondaryNodes)
{
    return ((LocalClusterConfigObjectManager::getInstance ())->getSwitchHaConfig (primaryNodes, nSecondaryNodes, secondaryNodes));
}

extern "C" int duplicateCmdsToClusterNodesWithCallBack (int appId, unsigned int nCommands, commandData_t commandList [], duplicateCmdResultCallBack_t duplicateCmdResultCB, void *pContext)
{
    if (true == AppObjectManager::validateAppId (appId))
    {
        return ((AppObjectManager::getAppInterfaceObjectManager (appId))->duplicateCmdsToClusterNodes (nCommands, commandList, duplicateCmdResultCB, pContext));
    }
    else
    {
        return (APP_CLUSTER_INVALID_APP_ID);
    }
}

extern "C" int registerDuplicateCmdsHandler (int appId, duplicateCmdsHandler_t  duplicateCmdHandler)
{
    if (true == AppObjectManager::validateAppId (appId))
    {
        return ((AppObjectManager::getAppInterfaceObjectManager (appId))->registerDuplicateCmdsHandler (duplicateCmdHandler));
    }
    else
    {
        return (APP_CLUSTER_INVALID_APP_ID);
    }
}

extern "C" int sendRemoteCommandsReply (int appId, void *pContext, unsigned int nResults, commandData_t resultList [])
{
    if (true == AppObjectManager::validateAppId (appId))
    {
        return ((AppObjectManager::getAppInterfaceObjectManager (appId))->sendRemoteCommandsReply (pContext, nResults, resultList));
    }
    else
    {
        return (APP_CLUSTER_INVALID_APP_ID);
    }
}

extern "C" int duplicateMultiCastCmds (int appId, unsigned int nCommands, commandData_t commandList [], unsigned int nNodes, node_t nodeList [], duplicateCmdResultCallBack_t duplicateCmdResultCB, void *pContext)
{
    if (true == AppObjectManager::validateAppId (appId))
    {
        return ((AppObjectManager::getAppInterfaceObjectManager (appId))->duplicateMultiCastCmds (nCommands, commandList, nNodes, nodeList, duplicateCmdResultCB, pContext));
    }
    else
    {
        return (APP_CLUSTER_INVALID_APP_ID);
    }
}

extern "C" int prismRegisterApp  (char * applicationName, applicationMainFunction applicationMain)
{
    return (AppObjectManager::registerApp (applicationName, applicationMain));
}

extern "C" int prismRegisterAppWithArg  (char * applicationName, applicationMainFunction applicationMain, UI32 argc, char *argv[])
{
    return (AppObjectManager::registerApp (applicationName, applicationMain, argc, argv));
}

#if 0
extern "C" int registerHeartBeatCallBack (heartBeatCallBack_t    heartBeatCB)
{
    return ((ClusterInterfaceObjectManager::getInstance ())->registerHeartBeatCallBack (heartBeatCB));
}

extern "C" int registerMemberJoinedCallBack (memberJoinedCallBack_t memberJoinedCB)
{
    return ((ClusterInterfaceObjectManager::getInstance ())->registerMemberJoinedCallBack (memberJoinedCB));
}
#endif

}
