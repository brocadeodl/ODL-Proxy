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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "App/AppInterfaceObjectManager.h"
#include "App/ClusterInterfaceObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "App/AppDebug.h"
#include <string>
#include <vector>

namespace WaveNs
{

AppInterfaceObjectManager::AppInterfaceObjectManager (string appInterfaceName, const UI32 &stackSize)
    : WaveLocalObjectManager (appInterfaceName, stackSize)
{
    addOperationMap (APP_REMOTE_COMMANDS,               reinterpret_cast<PrismMessageHandler> (&AppInterfaceObjectManager::processRemoteCommands));
    addOperationMap (APP_INTERFACE_CREATE_CLUSTER_DONE, reinterpret_cast<PrismMessageHandler> (&AppInterfaceObjectManager::processCreateClusterDone));
    addOperationMap (APP_INTERFACE_DELETE_CLUSTER_DONE, reinterpret_cast<PrismMessageHandler> (&AppInterfaceObjectManager::processDeleteClusterDone));
    addOperationMap (APP_INTERFACE_ADD_NODE_DONE,       reinterpret_cast<PrismMessageHandler> (&AppInterfaceObjectManager::processAddNodeDone));
    addOperationMap (APP_INTERFACE_JOIN_NODE_DONE,      reinterpret_cast<PrismMessageHandler> (&AppInterfaceObjectManager::processJoinNodeDone));
    addOperationMap (APP_INTERFACE_DELETE_NODE_DONE,    reinterpret_cast<PrismMessageHandler> (&AppInterfaceObjectManager::processDeleteNodeDone));

    m_duplicateCommandHandler           = NULL;
    m_clusterCollectValidationHandler   = NULL;
    m_clusterValidationHandler          = NULL;
    m_clusterReportValidationHandler    = NULL;
    m_nodeJoinedCallBack                = NULL;
    m_heartBeatLossCallBack             = NULL;

    m_clusterCreationResults             = NULL;
    m_nRepliedSecondary = 0;
}

    // This is not a singleton. Each time we call createInstance we will get
    // a different ObjectManager.

AppInterfaceObjectManager *AppInterfaceObjectManager::createInstance (string appInterfaceName)
{
    static int appDebugInit = 0;

    if (0 == appDebugInit)
    {
        AppDebug::initialize ();
        appDebugInit = 1;
    }

    WaveNs::trace (TRACE_LEVEL_DEVEL, "AppInterfaceObjectManager::createInstance : Entering ...");

    AppInterfaceObjectManager *pAppInterfaceObjectManager = new AppInterfaceObjectManager (appInterfaceName);

    WaveNs::prismAssert (NULL != pAppInterfaceObjectManager, __FILE__, __LINE__);

    return (pAppInterfaceObjectManager);

}

AppInterfaceObjectManager::~AppInterfaceObjectManager ()
{

}

PrismServiceId AppInterfaceObjectManager::getPrismServiceId ()
{
    return (getServiceId ());
}

PrismMessage *AppInterfaceObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case APP_REMOTE_COMMANDS :
            pPrismMessage = new AppInterfaceObjectManagerRemoteCommandsMessage;
            break;

        case APP_INTERFACE_CREATE_CLUSTER_DONE:
            pPrismMessage = new ClusterInterfaceObjectManagerCreateClusterDoneMessage;
            break;

        case APP_INTERFACE_DELETE_CLUSTER_DONE:
            pPrismMessage = new ClusterInterfaceObjectManagerDeleteClusterDoneMessage;
            break;

        case APP_INTERFACE_ADD_NODE_DONE:
            pPrismMessage = new ClusterInterfaceObjectManagerAddNodeDoneMessage;
            break;

        case APP_INTERFACE_JOIN_NODE_DONE:
            pPrismMessage = new ClusterInterfaceObjectManagerJoinNodeDoneMessage;
            break;

        case APP_INTERFACE_DELETE_NODE_DONE:
            pPrismMessage = new ClusterInterfaceObjectManagerDeleteNodeDoneMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}


// This function is called in a primary node after the framework in this node
// receives a message to create a clutser. This node should be the primary node.
// Framework will collect data from all the services in the node and will send
// This data to the appropriate services in the secondary nodes.

void AppInterfaceObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, string ("AppInterfaceObjectManager::clusterCreateCollectValidationData : Entering ...service ") + getServiceId ());

    MessageAndContext                  *pMessageAndContext = new MessageAndContext (reinterpret_cast<PrismMessageHandler *> (pMessage), pPrismAsynchronousContext, getServiceId ());
    clusterCollectValidationHandler_t   clusterCollectValidationHandler;

    m_mutex.lock ();
    clusterCollectValidationHandler = m_clusterCollectValidationHandler;
    m_mutex.unlock ();

    if (NULL != clusterCollectValidationHandler)
    {
        // clusterCollectValidationHandler is async interface. The reply will send by calling
        // clusterCollectValidationReply (which is exposed as extern "C" for Apps).
        clusterCollectValidationHandler (pMessageAndContext);
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "No Collect Validation handler");
        pMessageAndContext->m_pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pMessageAndContext->m_pPrismAsynchronousContext->callback ();
    }
    delete (pMessageAndContext);

}

// This function is called from App thread to send reply for collectValidation Handler.
// The application will provide the data it would like to send to secondary nodes as
// part of clutser creation.

void AppInterfaceObjectManager::clusterCollectValidationReply (void *pContext, UI32 validationStatus, unsigned int validationResultsLen, void *pValidationResultsData)
{
    trace (TRACE_LEVEL_DEVEL, "AppInterfaceObjectManager::clusterCollectValidationReply : Entering ...");

    MessageAndContext                                *pMessageAndContext = reinterpret_cast <MessageAndContext *> (pContext);
    WaveObjectManagerCollectValidationDataMessage *pMessage = reinterpret_cast<WaveObjectManagerCollectValidationDataMessage *> (pMessageAndContext->m_pMessage);
    pMessage->setValidationDetails (pValidationResultsData, validationResultsLen);

    //cout << "clusterCollectValidationReply: Validation Data = " << (char *) pValidationResultsData << " Len:" << validationResultsLen << endl;

    if (WAVE_MESSAGE_SUCCESS == validationStatus)
    {
        pMessageAndContext->m_pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        pMessageAndContext->m_pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_ERROR);
    }

    // Send data to framework.
    pMessageAndContext->m_pPrismAsynchronousContext->callback ();

    delete (pMessageAndContext);
}

// This function is called in a secondary node when the framework in this node
// receives a request to join a clutser. Framework will deliver the data provided
// by different services in the primary switch to the corresponding services in the
// secondary switches. The App in the secondary switch will verify that it can join
// the cluster based on the data it receives.

void AppInterfaceObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "AppInterfaceObjectManager::clusterCreateValidate : Entering ...");

    clusterValidationHandler_t          clusterValidationHandler;
    void                               *pValidationData;
    UI32                                Validationlen;
    MessageAndContext                  *pMessageAndContext = new MessageAndContext (reinterpret_cast<PrismMessageHandler *> (pMessage), pPrismAsynchronousContext, getServiceId ());

    m_mutex.lock ();
    clusterValidationHandler = m_clusterValidationHandler;
    m_mutex.unlock ();


    pMessage->getValidationDetails(pValidationData, Validationlen);
    if (NULL != clusterValidationHandler)
    {
        clusterValidationHandler (Validationlen, pValidationData, pMessageAndContext);
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "No Validation handler");
        pMessageAndContext->m_pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pMessageAndContext->m_pPrismAsynchronousContext->callback ();
    }
    delete (pMessageAndContext);

}

// This function is called from App thread to send reply for clusterCreateValidate.
// This code will be running on the secondary node. The secondary node indicates
// if it can join the cluster or not. The results is sent to framework which will
// send it to cluster service.

void AppInterfaceObjectManager::clusterValidationReply (void *pContext, UI32 validationStatus, unsigned int validationResultDataLen, void *pValidationResultData)
{
    trace (TRACE_LEVEL_DEVEL, "AppInterfaceObjectManager::clusterValidationReply : Entering ...");

    MessageAndContext                                *pMessageAndContext = reinterpret_cast <MessageAndContext *> (pContext);
    WaveObjectManagerValidateClusterCreationMessage *pMessage = reinterpret_cast<WaveObjectManagerValidateClusterCreationMessage *> (pMessageAndContext->m_pMessage);
    //cout << "&&&&&&&&& clusterValidationReply: Len: " << validationResultDataLen << " Data: " << pValidationResultData <<  endl;
    pMessage->setValidationResults (pValidationResultData, validationResultDataLen);

    if (WAVE_MESSAGE_SUCCESS == validationStatus)
    {
        pMessageAndContext->m_pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        pMessageAndContext->m_pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_ERROR);
    }

    // Send results to framework.
    pMessageAndContext->m_pPrismAsynchronousContext->callback ();

    delete (pMessageAndContext);
}

// This function is called from framework to report the data it got from service in the secondary nodes.
// This code will be running in the primay node and this function will be called after cluster creation
// is complete (successed or failed).

void AppInterfaceObjectManager::clusterCreateSendValidationResults (WaveObjectManagerSendValidationResultsMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "AppInterfaceObjectManager::clusterCreateSendValidationResults : Entering ...");

    clusterReportValidationHandler_t    clusterReportValidationHandler;
    UI32                                nNodes;
    UI32                                i;
    string                              nodeName;
    SI32                                portName;

    nNodes = pMessage->getNumberOfLocationsThatSentValidationResults ();

    m_mutex.lock ();
    clusterReportValidationHandler = m_clusterReportValidationHandler;
    m_mutex.unlock ();

    // This can happen if we get clusterCreateSendValidationResults more than
    // once without getting createClusterDone Message.
    if (NULL != m_clusterCreationResults)
    {
        delete[] m_clusterCreationResults;
        m_nRepliedSecondary = 0;
    }

    m_nRepliedSecondary = nNodes;
    m_clusterCreationResults = new NodeValidationResult_t[nNodes];

    UI32    index = 0;
    for (i = 0; i < nNodes; i++)
    {
        pMessage->getLocationDetailsAtIndex (i, nodeName, portName);
        if ((nodeName != FrameworkToolKit::getThisLocationIpAddress ()) ||
            (portName != FrameworkToolKit::getThisLocationPort ()))
        {
            memcpy (m_clusterCreationResults [index].nodeName, nodeName.c_str (), strlen (nodeName.c_str()) + 1);
            m_clusterCreationResults [index].nodePort = (UI32) portName;
            //m_clusterCreationResults [i].nodeStatus = WAVE_MESSAGE_SUCCESS;
            pMessage->getValidationResults(m_clusterCreationResults [index].nodeName, m_clusterCreationResults [index].nodePort, m_clusterCreationResults [index].validationResultPtr, m_clusterCreationResults [index].validationResultLen);
            //cout << "^^^^^^^^^^ clusterValidationReply: Node: " << (*nodeList)[i].m_nodeName << " Port: ";
            //cout << m_clusterCreationResults [index].nodePort << " Len: " << m_clusterCreationResults [index].validationResultLen << " Data:"  << endl;
            index ++;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "AppInterfaceObjectManager::clusterCreateSendValidationResults: Got Result for primary node.");
        }
    }


    if (NULL != clusterReportValidationHandler)
    {
        // This is sync call. The pointers are valid until this function returns.
        clusterReportValidationHandler (nNodes, m_clusterCreationResults);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "AppInterfaceObjectManager::clusterCreateSendValidationResults : clusterReportValidationHandler == NULL");
    }

    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();

}

/*

  This Duplicate Command handler sends a list of buffers to other nodes.
  It also allow sending a list of buffers in the reply.

*/

// Send command to a list of nodes.

int AppInterfaceObjectManager::duplicateMultiCastCmds (unsigned int nCommands, commandData_t commandList [], unsigned int nNodes, node_t nodeList [], duplicateCmdResultCallBack_t duplicateCmdResultCB, void *pContext)
{
    UI32                    i;
    int                     status;
    vector <Node>          *multicastNodes       = new vector <Node>();
    Node                    node;

    for (i = 0; i < nNodes; i++)
    {
        node.m_nodeName = nodeList [i].nodeName;
        node.m_nodePort = nodeList [i].nodePort;

        (*multicastNodes).push_back (node);
    }
    status = duplicateCmds (nCommands, commandList, multicastNodes, duplicateCmdResultCB, pContext);
    delete (multicastNodes);
    return (status);
}

// Duplicate command to all nodes in the cluster.

int AppInterfaceObjectManager::duplicateCmdsToClusterNodes (UI32 nCommands, commandData_t commandList [], duplicateCmdResultCallBack_t  duplicateCmdResultCB, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL," AppInterfaceObjectManager::duplicateCmdsToClusterNodes: Entery ... ");

    int                     status;
    vector <Node  >        *nodeList       = new vector <Node>();
    (ClusterInterfaceObjectManager::getInstance ())->getClusterNodes (nodeList);

    status = duplicateCmds (nCommands, commandList, nodeList, duplicateCmdResultCB, pContext);
    delete (nodeList);
    return (status);
}

int AppInterfaceObjectManager::duplicateCmds (unsigned int nCommands, commandData_t commandList [], vector <Node> *nodeList, duplicateCmdResultCallBack_t duplicateCmdResultCB, void *pContext)
{
    CommandInfo            *pCommandDetail  = new CommandInfo ();
//    vector <string>        *nodeNames       = new vector <string>();
    WaveMessageStatus      status          = WAVE_MESSAGE_ERROR;
    UI32                    expectedReply   = 0;
    DataInfo                dataInfo;
    UI32                    i;
    UI32                    oneWayMessage   = 0;

    trace (TRACE_LEVEL_DEVEL," ##### AppInterfaceObjectManager::duplicateCmds: Entery ... ");

    prismAssert (NULL != pCommandDetail, __FILE__, __LINE__);

    if (NULL == duplicateCmdResultCB)
    {
        oneWayMessage = 1;
    }

    for (i = 0; i < nCommands; i++)
    {
        dataInfo.pCommand    =   commandList [i].cmdPtr;
        dataInfo.commandLen  =   commandList [i].cmdLen;
        pCommandDetail->dataDetail.push_back (dataInfo);
    }

    pCommandDetail->duplicateCmdResultCB     =   duplicateCmdResultCB;
    pCommandDetail->pContext                 =   pContext;

    // Init number of replies that we should receive for this command.
    pCommandDetail->nNodes       = 0;
    pCommandDetail->nReplies     = 0;
    pCommandDetail->pNodeResults = new remoteCmdResult_t [nodeList->size ()];
    pCommandDetail->version      = 2;
    pCommandDetail->nCommands    = nCommands;



    // Duplicate command to other nodes in the cluster.
    for (i = 0; i < nodeList->size (); i++)
    {
        // Make sure that the node does not send to itself.

        if (( FrameworkToolKit::getThisLocationIpAddress () != (*nodeList)[i].m_nodeName) || (((UI32)(FrameworkToolKit::getThisLocationPort ()) != (*nodeList)[i].m_nodePort)))
        {
            // sendCommand should never fails. If it fails something really bad happened.
            trace (TRACE_LEVEL_DEBUG, string ("Send Cmd to Node: ") + (*nodeList)[i].m_nodeName + string (" Port: ") + (*nodeList)[i].m_nodePort);
            status = sendCommands (pCommandDetail, (*nodeList)[i], &expectedReply, oneWayMessage);
            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, string ("AppInterfaceObjectManager::duplicateCmdToClusterNodes: sendCommand failed for ") + (*nodeList)[i].m_nodeName + string (" Port: ") + (*nodeList)[i].m_nodePort);
#if 0
                if (NULL != pCommandDetail->pNodeResults)
                {
                    delete [] ( (char *) pCommandDetail->pNodeResults);
                }

                if (NULL != pCommandDetail)
                {

                    //pCommandDetail->dataDetail.erase ();
                    delete (pCommandDetail);
                }

                return (APP_CLUSTER_ERROR);
#endif
            }
        }
    }
    // Set number of expected replies.
    pCommandDetail->nNodes = expectedReply;

    if (0 == expectedReply)
    {
        trace (TRACE_LEVEL_DEBUG, "AppInterfaceObjectManager::duplicateCmdToClusterNodes: No Node to duplicate command to");
        if (NULL != pCommandDetail->pNodeResults)
        {
            delete [] ( (char *) pCommandDetail->pNodeResults);
        }

        if (NULL != pCommandDetail)
        {

           //pCommandDetail->dataDetail.erase ();
            delete (pCommandDetail);
        }
    }

    // If no one way messaging is involved then check if we succeeded in sending messages to all nodes or not.

    if ((0 == oneWayMessage) && ((nodeList->size ()) != expectedReply))
    {
        return (APP_CLUSTER_ERROR);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

// This function sends command to certain node.

WaveMessageStatus AppInterfaceObjectManager::sendCommands (CommandInfo *pCommandDetail, Node &node, UI32 *expectedReply, UI32 oneWayMessage)
{
    UI32                    locationId;
    WaveMessageStatus      status    = WAVE_MESSAGE_SUCCESS;
    UI32                    i;

    trace (TRACE_LEVEL_DEVEL, "AppInterfaceObjectManager::sendCommands : Entering ...");

    locationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (node.m_nodeName, node.m_nodePort);

    if (0 == locationId)
    {
        return (WAVE_MESSAGE_ERROR);
    }

    prismAssert (NULL != pCommandDetail, __FILE__, __LINE__);
    prismAssert (NULL != pCommandDetail->pNodeResults, __FILE__, __LINE__);
    prismAssert (NULL != expectedReply, __FILE__, __LINE__);

    AppInterfaceObjectManagerRemoteCommandsMessage *pRemoteCmdMessage = new AppInterfaceObjectManagerRemoteCommandsMessage (getServiceId ());
    pRemoteCmdMessage->setNodeName (node.m_nodeName);
    pRemoteCmdMessage->setNodePort (node.m_nodePort);

    for (i = 0; i < pCommandDetail->dataDetail.size (); i++)
    {
        if ( (NULL != pCommandDetail->dataDetail [i].pCommand) && (0 != pCommandDetail->dataDetail [i].commandLen))
        {
            pRemoteCmdMessage->setDuplicateCmdInputBuffer (i, pCommandDetail->dataDetail [i].commandLen, pCommandDetail->dataDetail [i].pCommand);
        }
    }
    pRemoteCmdMessage->setNCommands (pCommandDetail->nCommands);

    if (1 == oneWayMessage)
    {
#if 0
        if (NULL != pCommandDetail->pNodeResults)
        {
            delete [] ( (char *) pCommandDetail->pNodeResults);
        }

        if (NULL != pCommandDetail)
        {
            delete (pCommandDetail);
        }
#endif
        status = sendOneWay (pRemoteCmdMessage, locationId);
    }
    else
    {
        status = send (pRemoteCmdMessage, reinterpret_cast<PrismMessageResponseHandler> (&AppInterfaceObjectManager::processDuplicateCmdsReply), pCommandDetail, 0 /* timer */, locationId);

        (*expectedReply)++;
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"AppInterfaceObjectManager::sendCommands :Failed to send remoteCmdMsg to remote location.");

        if (0 == oneWayMessage)
        {
            (*expectedReply)--;
        }
    }

    return (status);
}

// This function is executed in the remote node when it receive duplicate command.

void AppInterfaceObjectManager::processRemoteCommands (AppInterfaceObjectManagerRemoteCommandsMessage *pMessage)
{
    commandData_t               *commandList = new commandData_t[pMessage->getNCommands ()];
    UI32                        i;
    duplicateCmdsHandler_t      duplicateCommandHandler;

    trace (TRACE_LEVEL_DEVEL, "AppInterfaceObjectManager::processRemoteCommands : Entering ...");

    m_mutex.lock ();
    duplicateCommandHandler =  m_duplicateCommandHandler;
    m_mutex.unlock ();

    if (NULL != duplicateCommandHandler)
    {
        trace (TRACE_LEVEL_DEBUG, "AppInterfaceObjectManager::processRemoteCommands :  ...");
        for (i = 0; i < pMessage->getNCommands (); i++)
        {
            pMessage->getDuplicateCmdInputBuffer (i, commandList [i].cmdLen, commandList [i].cmdPtr);
        }

        duplicateCommandHandler ((void *) pMessage, pMessage->getNCommands (), commandList);

		delete[] commandList;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "AppInterfaceObjectManager::processRemoteCommands: No duplicateCommandHandler");
        pMessage->setCompletionStatus (APP_CLUSTER_NO_DUPLICATE_HANDLER_REGISTERED);

		delete[] commandList;

        reply (pMessage);
    }

}

// This function is called to send the result for duplicate command.
// It can send data back to the send. The data is sent as pointer and length.

UI32 AppInterfaceObjectManager::sendRemoteCommandsReply (void *pContext, UI32 nResults, commandData_t resultList [])
{
    UI32                                            i;
    AppInterfaceObjectManagerRemoteCommandsMessage *pRemoteCommandMessage = (AppInterfaceObjectManagerRemoteCommandsMessage *) pContext;

    trace (TRACE_LEVEL_DEVEL, "AppInterfaceObjectManager::sendRemoteCommandsReply: Enter");

    // Set number of buffers that will be sent back.

    pRemoteCommandMessage->setNResults (nResults);

    for (i = 0; i < nResults; i++)
    {
        if ((NULL != resultList [i].cmdPtr) && (0 != resultList [i].cmdLen))
        {
            pRemoteCommandMessage->setDuplicateCmdOuputBuffer (i, resultList [i].cmdLen, resultList [i].cmdPtr);
        }
    }

    trace (TRACE_LEVEL_DEBUG, string ("AppInterfaceObjectManager::sendRemoteCommandsReply: Status = ") + WAVE_MESSAGE_SUCCESS);
    pRemoteCommandMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    return (reply (pRemoteCommandMessage));

}

// This fucntion is executed in the node that iniatiated duplicate command
// when the reply is received.

void AppInterfaceObjectManager::processDuplicateCmdsReply (FrameworkStatus frameworkStatus, AppInterfaceObjectManagerRemoteCommandsMessage *pRemoteCmdMessage, void *pContext)
{
    UI32                 i;
    string               nodeName ("0.0.0.0");
    UI32                 status         = WAVE_MESSAGE_SUCCESS;
    CommandInfo         *pCommandDetail = reinterpret_cast<CommandInfo *> (pContext);
    commandData_t       *pCommandData   = NULL;
    commandData_t       *pResultData    = NULL;

    trace (TRACE_LEVEL_DEVEL, "AppInterfaceObjectManager::processDuplicateCmdsReply : Entering ...");

    prismAssert (NULL != pCommandDetail, __FILE__, __LINE__);

#if 0
    if ((NULL == pCommandDetail) || (NULL == pCommandDetail->pNodeResults))
    {
        // this should only happen if sendCommand failed after sending the message to node (s)
        // in the cluster. (This scenario should never happen but just in case!)

        trace (TRACE_LEVEL_ERROR, "AppInterfaceObjectManager::processDuplicateCmdsReply : NULL = pCommandDetail->pNodeResults");

        if (NULL != pCommandDetail->pNodeResults)
        {
            delete [] ( (char *) pCommandDetail->pNodeResults);
        }

        if (NULL != pCommandDetail)
        {
            delete (pCommandDetail);
        }

        return;
    }
#endif

    remoteCmdResult_t   *pNodeResult    = pCommandDetail->pNodeResults + pCommandDetail->nReplies;

    if ((FRAMEWORK_SUCCESS != frameworkStatus) || (WAVE_MESSAGE_SUCCESS != pRemoteCmdMessage->getCompletionStatus()))
    {
        trace (TRACE_LEVEL_ERROR, string ("AppInterfaceObjectManager::processDuplicateCmdsReply :ERROR, framework status = ") + frameworkStatus + " message status: " + pRemoteCmdMessage->getCompletionStatus());
        status = APP_CLUSTER_FAILED_COMMUNICATE_TO_NODE;
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, string ("NResults: ") + pRemoteCmdMessage->getNResults ());
        // Is there any buffer that is sent back

        if (0 != pRemoteCmdMessage->getNResults ())
        {
            // iniatialize the buffer list for the results of that node.
            // Each node has its own list.

            pResultData = new commandData_t [pRemoteCmdMessage->getNResults ()];

            for (i = 0; i < pRemoteCmdMessage->getNResults (); i++)
            {
                pRemoteCmdMessage->transferOutputBufferToUser (i, pResultData [i].cmdLen, pResultData [i].cmdPtr);
            }
        }

        pNodeResult->nResults       =   pRemoteCmdMessage->getNResults ();
        pNodeResult->resultData     =   pResultData;
    }

    trace (TRACE_LEVEL_DEBUG, string ("Node Name: ") + pRemoteCmdMessage->getNodeName () + string (" Port: ") + pRemoteCmdMessage->getNodePort ());

    // Set the name (IP) of the node that sent the result.
    pNodeResult->nodePort    =   pRemoteCmdMessage->getNodePort ();

    nodeName    =   pRemoteCmdMessage->getNodeName ();
    memcpy (pNodeResult->nodeName, nodeName.c_str (), strlen (nodeName.c_str ()) + 1);

    // If there was a command list sent with the request. We will provide them back.
    // NOTE: if the remote node changed any of these data, these changes will be propagated
    // back to the sender.

    trace (TRACE_LEVEL_DEBUG, string ("NCommands: ") + pRemoteCmdMessage->getNCommands ());

    if (0 != pRemoteCmdMessage->getNCommands ())
    {
        pCommandData = new commandData_t [pRemoteCmdMessage->getNCommands ()];
        for (i = 0; i < pRemoteCmdMessage->getNCommands (); i++)
        {
            pRemoteCmdMessage->transferCmdInputBufferToUser (i, pCommandData [i].cmdLen, pCommandData [i].cmdPtr);
        }
    }

    pNodeResult->nCommands      =   pRemoteCmdMessage->getNCommands ();
    pNodeResult->commandData    =   pCommandData;
    pNodeResult->status         =   status;

    pCommandDetail->nReplies++;

    trace (TRACE_LEVEL_DEBUG, string ("pCommandDetail->nNodes: ") + pRemoteCmdMessage->getNCommands () + string (" pCommandDetail->nReplies: ") + pCommandDetail->nReplies);

    if (pCommandDetail->nNodes == pCommandDetail->nReplies)
    {
        // Set the general status for the message.
        // If the status is failure none of the node status should be examined.

        status = APP_CLUSTER_FAILED_TO_COMMUNICATE_TO_ALL_NODES;

        for (i = 0; i < pCommandDetail->nNodes; i++ )
        {
            pNodeResult =  pCommandDetail->pNodeResults + i;
            if (pNodeResult->status == WAVE_MESSAGE_SUCCESS)
            {
                status = WAVE_MESSAGE_SUCCESS;
                break;
            }
        }

        // Call the application register call back.
        if (NULL != pCommandDetail->duplicateCmdResultCB)
        {
            trace (TRACE_LEVEL_DEBUG, string ("Calling CB function"));
            (pCommandDetail->duplicateCmdResultCB) (status, pCommandDetail->pContext, pCommandDetail->nNodes, pCommandDetail->pNodeResults);
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "Invalid duplicate command call back function");
        }

        trace (TRACE_LEVEL_DEBUG, string ("pCommandDetail->nNodes: ") + pCommandDetail->nNodes);
        for (i = 0; i < pCommandDetail->nNodes; i++ )
        {
            UI32    j;

            pNodeResult =  pCommandDetail->pNodeResults + i;

            if (pNodeResult->status == WAVE_MESSAGE_SUCCESS)
            {
                // Free the result buffer sent from the other nodes.
                // Try to free these buffer only if duplicte command
                // was successful other the pointer is invalid.

                trace (TRACE_LEVEL_DEBUG, string ("pNodeResult->nResults: ") + pNodeResult->nResults);
                for (j = 0; j < pNodeResult->nResults; j++)
                {
                    if (NULL != pNodeResult->resultData [j].cmdPtr)
                    {
                        delete [] ( (char *) pNodeResult->resultData [j].cmdPtr);
                    }
                }
            }

            // Free the buffer for the sent commands that we deliver back to the sender.

            trace (TRACE_LEVEL_DEBUG, string ("pNodeResult->nCommands: ") + pNodeResult->nCommands);
            for (j = 0; j < pNodeResult->nCommands; j++)
            {
                if (NULL != pNodeResult->commandData)
                {
                    if (NULL != pNodeResult->commandData [j].cmdPtr)
                    {
                        delete [] ( (char *) pNodeResult->commandData [j].cmdPtr);
                    }
                }
            }
            if (NULL != pNodeResult->commandData)
            {
                delete [] pNodeResult->commandData;
            }
        }

        delete [] (pCommandDetail->pNodeResults);
        delete (pCommandDetail);
    }

    delete (pRemoteCmdMessage);

}

int AppInterfaceObjectManager::registerDuplicateCmdsHandler (duplicateCmdsHandler_t  duplicateCmdHandler)
{
    trace (TRACE_LEVEL_DEVEL," AppInterfaceObjectManager::registerDuplicateCmdHandler: Entery ... ");
    m_mutex.lock ();
    m_duplicateCommandHandler = duplicateCmdHandler;
    m_mutex.unlock ();

    return (WAVE_MESSAGE_SUCCESS);
}

void AppInterfaceObjectManager::registerValidationHandler (clusterValidationHandler_t clusterValidationHandler)
{
    trace (TRACE_LEVEL_DEVEL, string ("registerValidationHandler: Service ") + getServiceId ());
    m_mutex.lock ();
    m_clusterValidationHandler = clusterValidationHandler;
    m_mutex.unlock ();
}

void AppInterfaceObjectManager::registerCollectValidationHandler (clusterCollectValidationHandler_t  clusterCollectValidationHandler)
{
    trace (TRACE_LEVEL_DEVEL, string ("registerCollectValidationHandler: Service ") + getServiceId ());
    m_mutex.lock ();
    m_clusterCollectValidationHandler = clusterCollectValidationHandler;
    m_mutex.unlock ();
}

void AppInterfaceObjectManager::registerReportValidationHandler (clusterReportValidationHandler_t    clusterReportValidationHandler)
{
    trace (TRACE_LEVEL_DEVEL, string ("registerReportValidationHandler: Service ") + getServiceId ());
    m_mutex.lock ();
    m_clusterReportValidationHandler = clusterReportValidationHandler;
    m_mutex.unlock ();
}

void AppInterfaceObjectManager::registerNodeJoinedHandler (memberJoinedCallBack_t nodeJoinedCallBack)
{
    trace (TRACE_LEVEL_DEVEL, string ("registerNodeJoinedHandler: Service ") + getServiceId ());
    m_mutex.lock ();
    m_nodeJoinedCallBack = nodeJoinedCallBack;
    m_mutex.unlock ();
}

void AppInterfaceObjectManager::registerHeartbeatLossHandler (heartBeatCallBack_t heartBeatLossCallBack)
{
    trace (TRACE_LEVEL_DEVEL, string ("registerHeartbeatLossHandler: Service ") + getServiceId ());
    m_mutex.lock ();
    m_heartBeatLossCallBack = heartBeatLossCallBack;
    m_mutex.unlock ();
}

DataInfo::DataInfo ()
{
    commandLen  = 0;
    pCommand    = NULL;
}

DataInfo::~DataInfo ()
{
}

CommandInfo::CommandInfo ()
{
    duplicateCmdResultCB    = NULL;
    nCommands               = 0;
    nNodes                  = 0;
    nReplies                = 0;
    pContext                = NULL;
    pNodeResults            = NULL;
    status                  = WAVE_MESSAGE_SUCCESS;
    version                 = 0;
}

CommandInfo::~CommandInfo ()
{
}

MessageAndContext::MessageAndContext (PrismMessageHandler *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext, unsigned int appId)
{
    m_pMessage                  = pMessage;
    m_pPrismAsynchronousContext = pPrismAsynchronousContext;
    m_appId                     = appId;
}

MessageAndContext::MessageAndContext ()
{
    m_appId                     = 0; 
    m_pMessage                  = NULL;
    m_pPrismAsynchronousContext = NULL ;
}

MessageAndContext::~MessageAndContext ()
{
}


void AppInterfaceObjectManager::processCreateClusterDone (ClusterInterfaceObjectManagerCreateClusterDoneMessage *pMessage)
{
    UI32                    nNodes;
    UI32                    i;
    UI32                    index;

    NodeValidationResult_t *currClusterCreationResults;

    trace (TRACE_LEVEL_DEVEL, string (" AppInterfaceObjectManager::processCreateClusterDone: Entery ...Cluster Creation Status:") + pMessage->getOperationStatus ());

    nNodes = pMessage->getNSecondaryNodes ();
    currClusterCreationResults = new NodeValidationResult_t[nNodes];

    for (i = 0; i < nNodes; i++)
    {
        memcpy (currClusterCreationResults [i].nodeName, pMessage->getStatusNodeName (i).c_str (), strlen (pMessage->getStatusNodeName (i).c_str()));
        currClusterCreationResults [i].nodePort   = (UI32) pMessage->getStatusNodePort (i);
        currClusterCreationResults [i].nodeStatus = pMessage->getStatusNodeStatus (i);
        currClusterCreationResults [i].validationResultPtr = NULL;
        currClusterCreationResults [i].validationResultLen = 0;
    }

    if (NULL != m_clusterCreationResults)
    {
        for (i = 0; i < nNodes; i++)
        {
            // Find this node in the message.
            for (index = 0; index < m_nRepliedSecondary; index++)
            {
                if ( (0 ==memcmp (currClusterCreationResults [i].nodeName, m_clusterCreationResults [index].nodeName, strlen (m_clusterCreationResults [index].nodeName))) &&
                     (currClusterCreationResults [i].nodePort == (UI32) m_clusterCreationResults [index].nodePort))
                {
                    currClusterCreationResults [i].validationResultPtr = m_clusterCreationResults [index].validationResultPtr;
                    currClusterCreationResults [i].validationResultLen = m_clusterCreationResults [index].validationResultLen;

                }
            }

        }
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "AppInterfaceObjectManager::processCreateClusterDone: NULL == m_clusterCreationResults");
    }

    if (NULL != pMessage->getCreateClusterResultCallBack ())
    {
        // This is sync call. The pointers are valid until this function returns.
        (pMessage->getCreateClusterResultCallBack ()) (pMessage->getContext (), pMessage->getOperationStatus (), pMessage->getNSecondaryNodes (), currClusterCreationResults);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "AppInterfaceObjectManager::processCreateClusterDone: pMessage->m_createClusterResultCb== NULL");
    }

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);

    if (NULL != m_clusterCreationResults)
    {
        delete[] m_clusterCreationResults;
    }
    m_clusterCreationResults = NULL;

    if (NULL != currClusterCreationResults)
    {
        delete[] currClusterCreationResults;
    }

}

void AppInterfaceObjectManager::processDeleteClusterDone (ClusterInterfaceObjectManagerDeleteClusterDoneMessage *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, " AppInterfaceObjectManager::processDeleteClusterDone: Entery ... ");
    if (NULL != pMessage->getRemoveClusterResultCallBack ())
    {
        // This is sync call. The pointers are valid until this function returns.
        (pMessage->getRemoveClusterResultCallBack ()) (pMessage->getContext (), pMessage->getOperationStatus ());
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "AppInterfaceObjectManager::processDeleteClusterDone: pMessage->m_removeClusterResultCb== NULL");
    }

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

void AppInterfaceObjectManager::processAddNodeDone (ClusterInterfaceObjectManagerAddNodeDoneMessage *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, " AppInterfaceObjectManager::processAddNodeDone: Entery ... ");
    if (NULL != pMessage->getAddNodeResultCallBack ())
    {
        // This is sync call. The pointers are valid until this function returns.
        (pMessage->getAddNodeResultCallBack ()) (pMessage->getContext (), pMessage->getOperationStatus ());
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "AppInterfaceObjectManager::processAddNodeDone: pMessage->m_addNodeResultCb== NULL");
    }

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

void AppInterfaceObjectManager::processJoinNodeDone (ClusterInterfaceObjectManagerJoinNodeDoneMessage *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, " AppInterfaceObjectManager::processJoinNodeDone: Entery ... ");
    if (NULL != pMessage->getJoinNodeResultCallBack ())
    {
        // This is sync call. The pointers are valid until this function returns.
        (pMessage->getJoinNodeResultCallBack ()) (pMessage->getContext (), pMessage->getOperationStatus ());
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "AppInterfaceObjectManager::processJoinNodeDone: pMessage->m_joinNodeResultCb== NULL");
    }

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

void AppInterfaceObjectManager::processDeleteNodeDone (ClusterInterfaceObjectManagerDeleteNodeDoneMessage *pMessage)
{
    trace (TRACE_LEVEL_DEVEL, " AppInterfaceObjectManager::processDeleteNodeDone: Entery ... ");
    if (NULL != pMessage->getRemoveNodeResultCallBack ())
    {
        // This is sync call. The pointers are valid until this function returns.
        (pMessage->getRemoveNodeResultCallBack ()) (pMessage->getContext (), pMessage->getOperationStatus ());
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "AppInterfaceObjectManager::processDeleteNodeDone: pMessage->m_removeNodeResultCb== NULL");
    }

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

void AppInterfaceObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();

    ipString ipAddress;

    // FIXME : sagar :
    //         For now we simply give the null ipAddres.

    ipAddress[0] = '\0';

    if (NULL != m_heartBeatLossCallBack)
    {
        trace (TRACE_LEVEL_DEBUG, "AppInterfaceObjectManager::failover : Executing Heartbeat Lost Callback ...");

        switch (failoverReason)
        {
            case FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED :
                (*(m_heartBeatLossCallBack)) (APP_CLUSTER_CLUSTER_DELETED, ipAddress, 0);
                break;

            case FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED :
                (*(m_heartBeatLossCallBack)) (APP_CLUSTER_HEARTBEAT_LOST, ipAddress, 0);
                break;

            default :
                prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "AppInterfaceObjectManager::failover : Heartbeat Lost Callback is not set");
    }

    pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pFailoverAsynchronousContext->callback ();
}

}
