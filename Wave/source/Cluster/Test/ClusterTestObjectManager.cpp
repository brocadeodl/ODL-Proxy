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

#include "Cluster/Test/ClusterTestContexts.h"
#include "Cluster/Test/ClusterTestObjectManager.h"
#include "Framework/Core/Test/FrameworkTestabilityMessages.h"
#include "Framework/Types/Types.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Cluster/ClusterDebug.h"
#include <time.h>
#include "Framework/Utils/TraceUtils.h"
#include "App/ClusterInterfaceObjectManager.h"

namespace WaveNs
{
static  vector <string>        *commandBuffers = NULL;
static  vector <string>        *resultBuffers = NULL;

ClusterTestObjectManager::ClusterTestObjectManager ()
    : PrismTestObjectManager ("Cluster Test")
{
}

ClusterTestObjectManager *ClusterTestObjectManager::getInstance ()
{
    static ClusterTestObjectManager *pClusterTestObjectManager = NULL;

    if (NULL == pClusterTestObjectManager)
    {
        pClusterTestObjectManager = new ClusterTestObjectManager ();
        WaveNs::prismAssert (NULL != pClusterTestObjectManager, __FILE__, __LINE__);
    }

    return (pClusterTestObjectManager);
}

ClusterTestObjectManager::~ClusterTestObjectManager ()
{
}

PrismServiceId ClusterTestObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void ClusterTestObjectManager::testRequestHandler (RegressionTestMessage *pMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
//        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::ClusterShowStatStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::initData),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::registerCallBacks),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::CreateDeleteClusterStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::clusterCreateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::DuplicateCmdStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::DuplicateCmdOneWayStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::DuplicateCmdMulticastStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::DuplicateCmdMulticastOneWayStep),
//        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::AddDeleteNodeStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::clusterDeleteStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ClusterTestObjectManager::prismLinearSequencerFailedStep),
    };

    ClusterTestContext *pClusterTestContext = new ClusterTestContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pClusterTestContext->start ();
}

void ClusterTestObjectManager::initBuffers ()
{
    if (NULL == commandBuffers)
    {
        commandBuffers = new vector <string> ();
        commandBuffers->push_back ("1st BUFFER");
        commandBuffers->push_back ("2nd BUFFER");
        commandBuffers->push_back ("3rd BUFFER");
        commandBuffers->push_back ("4th BUFFER");
        commandBuffers->push_back ("5th BUFFER");
        commandBuffers->push_back ("6th BUFFER");
        commandBuffers->push_back ("7th BUFFER");
        commandBuffers->push_back ("8th BUFFER");
        commandBuffers->push_back ("9th BUFFER");
        commandBuffers->push_back ("10th BUFFER");
    }

    if (NULL == resultBuffers)
    {
        resultBuffers = new vector <string> ();
        resultBuffers->push_back ("1st RESULT BUFFER");
        resultBuffers->push_back ("2nd RESULT BUFFER");
        resultBuffers->push_back ("3rd RESULT BUFFER");
        resultBuffers->push_back ("4th RESULT BUFFER");
        resultBuffers->push_back ("5th RESULT BUFFER");
        resultBuffers->push_back ("6th RESULT BUFFER");
        resultBuffers->push_back ("7th RESULT BUFFER");
        resultBuffers->push_back ("8th RESULT BUFFER");
        resultBuffers->push_back ("9th RESULT BUFFER");
        resultBuffers->push_back ("10th RESULT BUFFER");
    }

    return;
}


void ClusterTestObjectManager::initData (ClusterTestContext *pClusterTestContext)
{
    initBuffers ();
    pClusterTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

void ClusterTestObjectManager::registerCallBacks (ClusterTestContext *pClusterTestContext)
{
    vector<string> temp;

    ClusterDebug::shellExecuteRegisterCallBackDebug (0, temp);
    pClusterTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

void ClusterTestObjectManager::CreateDeleteClusterStep (ClusterTestContext *pClusterTestContext)
{

    UI32                                        i;
    UI32                                        trial;
    WaveMessageStatus                          status = WAVE_MESSAGE_ERROR;

    trace ( TRACE_LEVEL_INFO, "Entering ClusterTestObjectManager::CreateDeleteClusterStep");

    for (trial = 0; trial < 100; trial++)
    {
        ClusterObjectManagerCreateClusterMessage   *pCreateMessage = new ClusterObjectManagerCreateClusterMessage ();
        ClusterObjectManagerDeleteClusterMessage   *pDeleteMessage = new ClusterObjectManagerDeleteClusterMessage ();


        pCreateMessage->setPrimaryNodeName (FrameworkToolKit::getThisLocationIpAddress ());
        pCreateMessage->setNSecondaryNodes (m_regressionInput.size ()/2);

        i = 0;
        while (i < m_regressionInput.size ())
        {
            pCreateMessage->setSecondaryNodeNameAndPort (m_regressionInput[i], atoi (m_regressionInput[i+1].c_str ()));
            i+=2;
        }


        status = sendSynchronously (pCreateMessage);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "ClusterTestObjectManager:: Failed to send message to createCluster ");
            pClusterTestContext->incrementNumberOfFailures ();
            return;
        }

 //       haNodeStatus_t      *pNodeStatus =   pCreateMessage->getNodeStatusAddr ();
        haNodeStatus_t     *pNodeStatus = new haNodeStatus_t[pCreateMessage->getNSecondaryNodes ()];
        for (i = 0; i < pCreateMessage->getNSecondaryNodes (); i++)
        {
            string nodeName = pCreateMessage->getStatusNodeName (i);
            memcpy (pNodeStatus [i].nodeName, nodeName.c_str (), strlen (nodeName.c_str ()));
            //pNodeStatus [i].nodeName   = pCreateMessage->getStatusNodeName (i);
            pNodeStatus [i].nodePort   = (UI32) pCreateMessage->getStatusNodePort (i);
            pNodeStatus [i].nodeStatus = (SI32) pCreateMessage->getStatusNodeStatus (i);
        }


        status = sendSynchronously (pDeleteMessage);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR,"ClusterTestObjectManager:: Failed to send message to removeCluster ");
            pClusterTestContext->incrementNumberOfFailures ();
			delete[] pNodeStatus;
            return;
        }

        delete (pCreateMessage);
        delete (pDeleteMessage);
		delete[] pNodeStatus;
    }

    trace ( TRACE_LEVEL_DEVEL, "Exit ClusterTestObjectManager::createCluster");

    pClusterTestContext->executeNextStep (((pClusterTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
    return;
}

void ClusterTestObjectManager::DuplicateCmdStep (ClusterTestContext *pClusterTestContext)
{
    trace ( TRACE_LEVEL_INFO, "DuplicateCmdStep: Send random number of buffers to all nodes in cluster");
    DuplicateCmd (pClusterTestContext, 0, 0);
}

void ClusterTestObjectManager::DuplicateCmdOneWayStep (ClusterTestContext *pClusterTestContext)
{
    trace ( TRACE_LEVEL_INFO, "DuplicateCmdOneWayStep: Send one way random number of buffers to all nodes in cluster");
    ClusterTestObjectManager::DuplicateCmd (pClusterTestContext, 1, 0);
}

void ClusterTestObjectManager::DuplicateCmdMulticastStep (ClusterTestContext *pClusterTestContext)
{
    trace ( TRACE_LEVEL_INFO, "DuplicateCmdMulticastStep: Send random number of buffers to some nodes in cluster");
    DuplicateCmd (pClusterTestContext, 0, 1);
}

void ClusterTestObjectManager::DuplicateCmdMulticastOneWayStep (ClusterTestContext *pClusterTestContext)
{
    trace ( TRACE_LEVEL_INFO, "DuplicateCmdMulticastOneWayStep: Send one way random number of buffers to some nodes in cluster");
    ClusterTestObjectManager::DuplicateCmd (pClusterTestContext, 1, 1);
}

void ClusterTestObjectManager::DuplicateCmd (ClusterTestContext *pClusterTestContext, UI32 oneWay, UI32 multicast)
{
    trace ( TRACE_LEVEL_DEBUG, "Entering ClusterTestObjectManager::DuplicateCmdStep");

    sendDuplicateCmd (0, oneWay, multicast, &ClusterTestObjectManager::duplicateCmdTestReply, pClusterTestContext);

    return;
}

void ClusterTestObjectManager::sendDuplicateCmd (UI32 service, UI32 oneWay, UI32 multicast, duplicateCmdResultCallBack_t duplicateCmdResultCB, void *pContext)
{
    UI32                nCmdBuffers;
    commandData_t       commandList [MAX_DUP_COMMAND_TRIALS];
    node_t              nodeList [30];
    vector <Node >     *nodesInCluster       = new vector <Node>();
    UI32                nNodes = 1;
    UI32                trial;
    UI32                i = 0;
    UI32                node = 0;
    UI32                status;

    ClusterTestContext *pClusterTestContext = reinterpret_cast <ClusterTestContext *> (pContext);

    nDupCommandTrials = 0;
    srand (time(NULL));

    initBuffers ();
    ResourceId status1 = (ClusterInterfaceObjectManager::getInstance ())->getClusterNodes (nodesInCluster);
    if (WAVE_MESSAGE_SUCCESS != status1)
    {
        trace (TRACE_LEVEL_WARN, "ClusterTestObjectManager::sendDuplicateCmd : Could not get list of nodesInCluster. returning..");
        delete (nodesInCluster);
        pClusterTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    while (i < nodesInCluster->size ())
    {
        if (((FrameworkToolKit::getThisLocationIpAddress () != (*nodesInCluster)[i].m_nodeName)) ||
             ((UI32)(FrameworkToolKit::getThisLocationPort ()) != (*nodesInCluster)[i].m_nodePort))
        {
            memcpy(nodeList [node].nodeName, (*nodesInCluster)[i].m_nodeName.c_str (), strlen((*nodesInCluster)[i].m_nodeName.c_str ()) + 1);
            nodeList [node].nodePort =  (*nodesInCluster)[i].m_nodePort;
            node++;
        }
        i++;
    }

    if (nodesInCluster->size () > 2)
    {
        nNodes = nodesInCluster->size () - 2;
    }

    delete (nodesInCluster);                    // no use of nodesInCluster further.

    for (i = 0; i < MAX_DUP_COMMAND_TRIALS; i++)
    {
        commandList[i].cmdLen = strlen((*commandBuffers) [i].c_str ()) + 1;
        commandList[i].cmdPtr = (char *) new UI8[strlen((*commandBuffers) [i].c_str ()) + 1];
        memcpy (commandList[i].cmdPtr, (*commandBuffers) [i].c_str (), strlen((*commandBuffers) [i].c_str ()) + 1);
    }

    for (trial = 0; trial < MAX_DUP_COMMAND_TRIALS; trial++)
    {
        nCmdBuffers = rand () % MAX_DUP_COMMAND_TRIALS;

        if (multicast)
        {
            if (0 == oneWay)
            {
                //cout << "###### nNodes: " << nNodes << " NodeList.Ip: " << nodeList [0].nodeName << " Port: " << nodeList [0].nodePort << endl;
                status = duplicateMultiCastCmds (service, nCmdBuffers, commandList, nNodes, nodeList, duplicateCmdResultCB, pContext);
            }
            else
            {
                status = duplicateMultiCastCmds (service, nCmdBuffers, commandList, nNodes, nodeList, NULL, NULL);
            }
        }
        else
        {
            if (0 == oneWay)
            {
                status = duplicateCmdsToClusterNodesWithCallBack (service, nCmdBuffers, commandList, duplicateCmdResultCB, pContext);
            }
            else
            {
                status = duplicateCmdsToClusterNodesWithCallBack (service, nCmdBuffers, commandList, NULL, NULL);
            }
        }
    }

    for (i = 0; i < MAX_DUP_COMMAND_TRIALS; i++)
    {
        delete [] ((char *)commandList[i].cmdPtr);
    }

    if (NULL == pClusterTestContext)
    {
        return;
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        pClusterTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    if (1 == oneWay)
    {
        pClusterTestContext->executeNextStep (((pClusterTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
    }


    trace ( TRACE_LEVEL_DEVEL, "Exit ClusterTestObjectManager::DuplicateCmdStep");
}

void ClusterTestObjectManager::duplicateCmdTestHandler (void *pContext, unsigned int nCommands, commandData_t commandList [])
{
    WaveNs::trace ( TRACE_LEVEL_INFO, "ClusterTestObjectManager::duplicateCmdTestHandler: Duplicate Command Test Handler");
    commandData_t   resultList [MAX_DUP_COMMAND_TRIALS];
    UI32            nResults;
    UI32            i;

    (ClusterTestObjectManager::getInstance ())->initBuffers ();
    nResults = rand () % MAX_DUP_COMMAND_TRIALS;

    for (i = 0; i < MAX_DUP_COMMAND_TRIALS; i++)
    {
        resultList[i].cmdLen = strlen((*resultBuffers) [i].c_str ()) + 1;
        resultList[i].cmdPtr = (char *) new UI8[strlen((*resultBuffers) [i].c_str ()) + 1];
        memcpy (resultList[i].cmdPtr, (*resultBuffers) [i].c_str (), strlen((*resultBuffers) [i].c_str ()) + 1);
    }

    //cout << endl << "...Got Duplicate CMD. nCommands: " << nCommands << endl;

    for (i = 0; i < nCommands; i++)
    {
        if (NULL != commandList [i].cmdPtr)
        {
            if ( 0 != memcmp (commandList[i].cmdPtr, (*commandBuffers) [i].c_str (), strlen((*commandBuffers) [i].c_str ()) + 1))
            {
                WaveNs::trace (TRACE_LEVEL_ERROR,string ("Invalid command buffer received. Expected Buffer = ") + (*commandBuffers) [i] + " received buffer: " + ((char *) commandList[i].cmdPtr));
            }
            //cout << "Received Command Num: " << i << "  Cmd: " <<  string ((char *) commandList [i].cmdPtr) << ", cmdLen: " << commandList [i].cmdLen << endl;
        }

    }

    sendRemoteCommandsReply (0, pContext, nResults, resultList);

    for ( i = 0; i < MAX_DUP_COMMAND_TRIALS; i++)
    {
        delete [] ((char *)resultList[i].cmdPtr);
    }
}

void ClusterTestObjectManager::clusterCreateStep (ClusterTestContext *pClusterTestContext)
{
    trace ( TRACE_LEVEL_DEBUG, "Entering ClusterTestObjectManager::clusterCreateStep");
    UI32                i           = 0;
    WaveMessageStatus  status  = WAVE_MESSAGE_ERROR;

    ClusterObjectManagerCreateClusterMessage   *pCreateMessage = new ClusterObjectManagerCreateClusterMessage ();

    pCreateMessage->setPrimaryNodeName (FrameworkToolKit::getThisLocationIpAddress ());
    pCreateMessage->setNSecondaryNodes (m_regressionInput.size ()/2);

    if (0 != (m_regressionInput.size ()%2))
    {
        trace ( TRACE_LEVEL_ERROR, "Invalid parameters for create cluster");
        pClusterTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    while (i < m_regressionInput.size ())
    {
        pCreateMessage->setSecondaryNodeNameAndPort (m_regressionInput[i], atoi (m_regressionInput[i+1].c_str ()));
        i+=2;
    }

    status = sendSynchronously (pCreateMessage);

    delete (pCreateMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR,"ClusterTestObjectManager:: Failed to send message to create Cluster ");
        pClusterTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }
    pClusterTestContext->executeNextStep (((pClusterTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void ClusterTestObjectManager::clusterDeleteStep (ClusterTestContext *pClusterTestContext)
{
    trace ( TRACE_LEVEL_DEBUG, "Entering ClusterTestObjectManager::clusterDeleteStep");

    WaveMessageStatus                          sendStatus     = WAVE_MESSAGE_ERROR;
    ClusterObjectManagerDeleteClusterMessage   *pDeleteMessage = new ClusterObjectManagerDeleteClusterMessage ();

    sendStatus = (ClusterTestObjectManager::getInstance ())->sendSynchronously (pDeleteMessage);
    delete (pDeleteMessage);

    if (WAVE_MESSAGE_SUCCESS != sendStatus)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR,"ClusterTestObjectManager:: Failed to send message to delete Cluster ");
        pClusterTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }
    pClusterTestContext->executeNextStep (((pClusterTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void ClusterTestObjectManager::duplicateCmdTestReply (int status, void *pContext, int nNodes, remoteCmdResult_t nodeResults [])
{
    WaveNs::trace ( TRACE_LEVEL_DEBUG, "Entering ClusterTestObjectManager::duplicateCmdTestReply");

    ClusterTestContext *pClusterTestContext = reinterpret_cast <ClusterTestContext *> (pContext);

    (ClusterTestObjectManager::getInstance ())->nDupCommandTrials++;

    (ClusterTestObjectManager::getInstance ())->duplicateCmdReply (status, pContext, nNodes, nodeResults);

    if (MAX_DUP_COMMAND_TRIALS == (ClusterTestObjectManager::getInstance ())->nDupCommandTrials)
    {
        pClusterTestContext->executeNextStep (((pClusterTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
    }

}

void ClusterTestObjectManager::duplicateCmdReply (int status, void *pContext, int nNodes, remoteCmdResult_t nodeResults [])
{
    UI32                i;
    UI32                node;

    WaveNs::trace ( TRACE_LEVEL_DEBUG, "Entering ClusterTestObjectManager::duplicateCmdReply");

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace ( TRACE_LEVEL_ERROR, "ClusterTestObjectManager::duplicateCmdReply: Failed to duplicate Command to all nodes");
    }
    else
    {
        WaveNs::trace ( TRACE_LEVEL_DEBUG, "Entering ClusterTestObjectManager::duplicateCmdReply: duplicate Command done successfully");
        if (NULL != nodeResults)
        {
            for (node = 0; node < (UI32) nNodes; node++)
            {
                if ( WAVE_MESSAGE_SUCCESS !=nodeResults [node].status)
                {
                    WaveNs::trace ( TRACE_LEVEL_ERROR, "ClusterTestObjectManager::duplicateCmdReply: Failed to duplicate Command to node");
                }
                else
                {
                    //cout << "Num Commands: " << nodeResults [node].nCommands << endl;

                    for (i = 0; i < nodeResults [node].nCommands; i++)
                    {
                        if (NULL != nodeResults [node].commandData [i].cmdPtr)
                        {
                            if ( 0 != memcmp (nodeResults [node].commandData [i].cmdPtr, (*commandBuffers) [i].c_str (), strlen((*commandBuffers) [i].c_str ()) + 1))
                            {
                                WaveNs::trace (TRACE_LEVEL_ERROR,string ("Invalid command buffer received back. Expected Buffer = ") + (*commandBuffers) [i] + " received buffer: " + ((char *) nodeResults [node].commandData [i].cmdPtr));
                            }

                            //cout << "Received Command Num: " << i << " Cmd: " <<  string ((char *) nodeResults [node].commandData [i].cmdPtr) << ", cmdLen: " << nodeResults [node].commandData [i].cmdLen << endl;
                        }
                    }

                    //cout << "Num Results: " << nodeResults [node].nResults << endl;
                    for (i = 0; i < nodeResults [node].nResults; i++)
                    {
                        if (NULL != nodeResults [node].resultData [i].cmdPtr)
                        {
                            if ( 0 != memcmp (nodeResults [node].resultData [i].cmdPtr, (*resultBuffers) [i].c_str (), strlen((*resultBuffers) [i].c_str ()) + 1))
                            {
                                WaveNs::trace (TRACE_LEVEL_ERROR,string ("Invalid Result buffer received. Expected Buffer = ") + (*resultBuffers) [i] + " received buffer: " + ((char *) nodeResults [node].resultData [i].cmdPtr));
                            }
                            //cout << "Received result Num: " << i << " Cmd: " <<  string ((char *) nodeResults [node].resultData [i].cmdPtr) << ", cmdLen: " << nodeResults [node].resultData [i].cmdLen << endl;
                        }
                    }
                }
            }
        }
    }

}

void ClusterTestObjectManager::AddDeleteNodeStep (ClusterTestContext *pClusterTestContext)
{

    UI32                                        i;
//    UI32                                        trial;
    WaveMessageStatus                          status = WAVE_MESSAGE_ERROR;
    ClusterObjectManagerCreateClusterMessage   *pCreateMessage = new ClusterObjectManagerCreateClusterMessage ();
    ClusterObjectManagerDeleteClusterMessage   *pDeleteMessage = new ClusterObjectManagerDeleteClusterMessage ();
    ClusterObjectManagerAddNodeMessage         *pAddNodeMessage = new ClusterObjectManagerAddNodeMessage ();
    ClusterObjectManagerDeleteNodeMessage      *pDeleteNodeMessage = new ClusterObjectManagerDeleteNodeMessage ();


    trace ( TRACE_LEVEL_INFO, "Entering ClusterTestObjectManager::CreateDeleteClusterStep");

    pCreateMessage->setPrimaryNodeName (FrameworkToolKit::getThisLocationIpAddress ());
    pCreateMessage->setNSecondaryNodes (m_regressionInput.size ());

    for (i = 0; i < m_regressionInput.size (); i++)
    {
        pCreateMessage->setSecondaryNodeNameAndPort (m_regressionInput[i], FrameworkToolKit::getPrismDefaultTcpPort ());
        pAddNodeMessage->setSecondaryNodeNameAndPort (m_regressionInput[i], FrameworkToolKit::getPrismDefaultTcpPort ());
        pDeleteNodeMessage->setSecondaryNodeNameAndPort (m_regressionInput[i], FrameworkToolKit::getPrismDefaultTcpPort ());
    }


    status = sendSynchronously (pCreateMessage);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "ClusterTestObjectManager:: Failed to send message to createCluster ");
        pClusterTestContext->incrementNumberOfFailures ();
        return;
    }

    status = sendSynchronously (pAddNodeMessage);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"Failed to send message to addNodeToCluster ");
        pClusterTestContext->incrementNumberOfFailures ();
        return;
    }

    status = sendSynchronously (pDeleteNodeMessage);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"Failed to send message to removeNodeFromCluster ");
        pClusterTestContext->incrementNumberOfFailures ();
        return;
    }

/*    for (trial = 0; trial < 100; trial++)
    {
        for (i = 0; i < m_regressionInput.size (); i++)
        {

            //pDeleteNodeMessage->setResultCB (NULL);
            //pDeleteNodeMessage->setContext (NULL);


            //pAddNodeMessage->setNodeName (m_regressionInput[i]);
            //pAddNodeMessage->setResultCB (NULL);
            //pAddNodeMessage->setContext (NULL);

        }

    }

*/
    status = sendSynchronously (pDeleteMessage);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,"ClusterTestObjectManager:: Failed to send message to removeCluster ");
        pClusterTestContext->incrementNumberOfFailures ();
        return;
    }

    trace ( TRACE_LEVEL_DEVEL, "Exit ClusterTestObjectManager::createCluster");

    delete (pCreateMessage);
    delete (pDeleteMessage);

    pClusterTestContext->executeNextStep (((pClusterTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
    return;
}

void ClusterTestObjectManager::ClusterShowStatStep (ClusterTestContext *pClusterTestContext)
{
    /*UI32 i;

    cout << "argc: " << m_regressionInput.size () << endl;
    for (i = 0; i < m_regressionInput.size (); i++)
    {
        cout << m_regressionInput [i] << endl;
    }
    */
    pClusterTestContext->executeNextStep (((pClusterTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
    return;
}

ClusterTestData::ClusterTestData ()
{

}

ClusterTestData::~ClusterTestData ()
{

}


}
