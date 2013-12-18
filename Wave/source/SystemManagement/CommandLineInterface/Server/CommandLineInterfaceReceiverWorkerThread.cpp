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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceReceiverWorkerThread.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceReceiverObjectManager.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FixedSizeBuffer.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/Base64Utils.h"
#include "Framework/Utils/PamUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceWorkerMessage.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceMessage.h"
#include "SystemManagement/WaveSystemManagementGetYangUserInterfaceMessage.h"
#include "SystemManagement/WaveSystemManagementDisplayConfigurationByTargetNodeNameMessage.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceWorkerSendContext.h"
#include "SystemManagement/WaveSystemManagementObjectManager.h"
#include "Modeling/YANG/ObjectModel/YangElement.h"

namespace WaveNs
{

CommandLineInterfaceReceiverWorkerThread::CommandLineInterfaceReceiverWorkerThread (CommandLineInterfaceReceiverObjectManager *pCommandLineInterfaceReceiverObjectManager, ServerStreamingSocket *pServerStreamingSocket)
    : PrismPosixThread                             (),
      WaveWorker                                   (pCommandLineInterfaceReceiverObjectManager),
      m_pCommandLineInterfaceReceiverObjectManager (pCommandLineInterfaceReceiverObjectManager),
      m_pServerStreamingSocket                     (pServerStreamingSocket),
      m_pPrismMutex                                (NULL),
      m_pPrismCondition                            (NULL)
{
    m_pPrismMutex = new PrismMutex;

    prismAssert (NULL != m_pPrismMutex, __FILE__, __LINE__);

    m_pPrismCondition = new PrismCondition (m_pPrismMutex);

    prismAssert (NULL != m_pPrismCondition, __FILE__, __LINE__);
}

CommandLineInterfaceReceiverWorkerThread::~CommandLineInterfaceReceiverWorkerThread ()
{
    if (NULL != m_pServerStreamingSocket)
    {
        delete m_pServerStreamingSocket;
    }

    if (NULL != m_pPrismCondition)
    {
        delete m_pPrismCondition;
    }

    if (NULL != m_pPrismMutex)
    {
        delete m_pPrismMutex;
    }
}

void CommandLineInterfaceReceiverWorkerThread::submitCommandLineInterfaceWorkerMessage (CommandLineInterfaceWorkerMessage *pCommandLineInterfaceWorkerMessage)
{
    prismAssert (NULL != pCommandLineInterfaceWorkerMessage, __FILE__, __LINE__);

    m_pPrismMutex->lock ();

    m_commandLineInterfaceWorkerMessages.push_back (pCommandLineInterfaceWorkerMessage);

    m_pPrismCondition->resume ();

    m_pPrismMutex->unlock ();
}

CommandLineInterfaceWorkerMessage *CommandLineInterfaceReceiverWorkerThread::getNextCommandLineInterfaceWorkerMessage ()
{
    if (true == (m_commandLineInterfaceWorkerMessages.empty ()))
    {
        return (NULL);
    }
    else
    {
        CommandLineInterfaceWorkerMessage *pCommandLineInterfaceWorkerMessage = m_commandLineInterfaceWorkerMessages.front ();

        m_commandLineInterfaceWorkerMessages.pop_front ();

        return (pCommandLineInterfaceWorkerMessage);
    }
}

void CommandLineInterfaceReceiverWorkerThread::processMessages ()
{
    while (true)
    {
        m_pPrismMutex->lock ();

        CommandLineInterfaceWorkerMessage *pCommandLineInterfaceWorkerMessage = getNextCommandLineInterfaceWorkerMessage ();

        if (NULL == pCommandLineInterfaceWorkerMessage)
        {
            m_pPrismCondition->wait ();

            m_pPrismMutex->unlock ();
        }
        else
        {
            m_pPrismMutex->unlock ();

            WaveCommandLineInterfaceWorkerOperation waveCommandLineInterfaceWorkerOperation = pCommandLineInterfaceWorkerMessage->getWaveCommandLineInterfaceWorkerOperation ();

            if (WAVE_CLI_WORKER_OPERATION_USER_REQUEST == waveCommandLineInterfaceWorkerOperation)
            {
                CommandLineInterfaceMessage *pCommandLineInterfaceMessage = pCommandLineInterfaceWorkerMessage->getPCommandLineInterfaceMessage ();

                pCommandLineInterfaceWorkerMessage->setPCommandLineInterfaceMessage (NULL);

                if (NULL != pCommandLineInterfaceMessage)
                {
                    // Process the message first.

                    processCommandLineInterfaceMessage (pCommandLineInterfaceMessage);
                }
            }
            else if (WAVE_CLIWORKER__OPERATION_ADMINISTRATIVE_REQUEST == waveCommandLineInterfaceWorkerOperation)
            {
            }
            else if (WAVE_CLI_WORKER_OPERATION_SEND_CALLBACK == waveCommandLineInterfaceWorkerOperation)
            {
                CommandLineInterfaceWorkerSendContext *pCommandLineInterfaceWorkerSendContext = pCommandLineInterfaceWorkerMessage->getPCommandLineInterfaceWorkerSenderContext ();

                pCommandLineInterfaceWorkerMessage->setPCommandLineInterfaceWorkerSenderContext (NULL);

                if (NULL != pCommandLineInterfaceWorkerSendContext)
                {
                    FrameworkStatus                                                         frameworkStatus                                                        = pCommandLineInterfaceWorkerSendContext->getFrameworkStatus             ();
                    PrismMessage                                                           *pPrismMessage                                                          = pCommandLineInterfaceWorkerSendContext->getPPrismMessage               ();
                    PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread  prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread = pCommandLineInterfaceWorkerSendContext->getPrismMessageResponseHandler ();
                    void                                                                   *pPrismMessageContext                                                   = pCommandLineInterfaceWorkerSendContext->getPPrismMessageContext        ();

                    prismAssert (NULL != pPrismMessage,                                                          __FILE__, __LINE__);
                    prismAssert (NULL != prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, __FILE__, __LINE__);
                    prismAssert (NULL != pPrismMessageContext,                                                   __FILE__, __LINE__);

                    delete pCommandLineInterfaceWorkerSendContext;

                    (this->*prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread) (frameworkStatus, pPrismMessage, pPrismMessageContext);
                }
            }

            delete pCommandLineInterfaceWorkerMessage;
        }
    }
}

void CommandLineInterfaceReceiverWorkerThread::processCommandLineInterfaceMessage (CommandLineInterfaceMessage *pCommandLineInterfaceMessage)
{
    ResourceId operationCode = pCommandLineInterfaceMessage->getOperationcode ();

    switch (operationCode)
    {
        case WAVE_CLI_CLIENT_OPERATION_GET_YANG_USER_INTERFACE:
            processCommandLineInterfaceMessageToGetYangUserInterface (pCommandLineInterfaceMessage);
            break;

        case WAVE_CLI_CLIENT_OPERATION_SHOW_RUNNING_CONFIGURATION:
            processCommandLineInterfaceMessageToShowRunningConfiguration (pCommandLineInterfaceMessage);
            break;

        case WAVE_CLI_CLIENT_OPERATION_WRITE_REQUEST:
            processCommandLineInterfaceMessageToHandleWriteRequest (pCommandLineInterfaceMessage);
            break;

        default :
            prismAssert (false, __FILE__, __LINE__);
            break;
    }
}

void CommandLineInterfaceReceiverWorkerThread::processCommandLineInterfaceMessageToHandleWriteRequest (CommandLineInterfaceMessage *pCommandLineInterfaceMessage) 
{
    ResourceId operationCode = pCommandLineInterfaceMessage->getOperationcode ();

    prismAssert (WAVE_CLI_CLIENT_OPERATION_WRITE_REQUEST == operationCode, __FILE__, __LINE__);

    vector<string> inputString = pCommandLineInterfaceMessage->getInput ();

    string errorString("");

    WaveCommandLineInterfaceOperationStatus status = processWriteRequests (inputString, errorString, pCommandLineInterfaceMessage->getIsWyserEaCompatibilityMode());

    pCommandLineInterfaceMessage->setErrorString(errorString);

    pCommandLineInterfaceMessage->setCommandLineInterfaceOperationStatus (status);

    pCommandLineInterfaceMessage->setCommandLineInterfaceOperation (WAVE_CLI_OPERATION_RESPONSE);

    (*m_pServerStreamingSocket) << pCommandLineInterfaceMessage;

    delete pCommandLineInterfaceMessage;
}

void CommandLineInterfaceReceiverWorkerThread::processCommandLineInterfaceMessageToGetYangUserInterface (CommandLineInterfaceMessage *pCommandLineInterfaceMessage)
{
    ResourceId operationCode = pCommandLineInterfaceMessage->getOperationcode ();

    prismAssert (WAVE_CLI_CLIENT_OPERATION_GET_YANG_USER_INTERFACE == operationCode, __FILE__, __LINE__);

    WaveSystemManagementGetYangUserInterfaceMessage *pWaveSystemManagementGetYangUserInterfaceMessage = new WaveSystemManagementGetYangUserInterfaceMessage ();

    prismAssert (NULL != pWaveSystemManagementGetYangUserInterfaceMessage, __FILE__, __LINE__);

    ResourceId status = send (pWaveSystemManagementGetYangUserInterfaceMessage, reinterpret_cast<PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread> (&CommandLineInterfaceReceiverWorkerThread::getYangUserInterfaceCallback), pCommandLineInterfaceMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        getYangUserInterfaceCallback (FRAMEWORK_ERROR, pWaveSystemManagementGetYangUserInterfaceMessage, pCommandLineInterfaceMessage);
    }
}

void CommandLineInterfaceReceiverWorkerThread::getYangUserInterfaceCallback (FrameworkStatus frameworkStatus, WaveSystemManagementGetYangUserInterfaceMessage *pWaveSystemManagementGetYangUserInterfaceMessage, void *pContext)
{
    CommandLineInterfaceMessage *pCommandLineInterfaceMessage = reinterpret_cast<CommandLineInterfaceMessage *> (pContext);
    ResourceId                   status                       = WAVE_MESSAGE_ERROR;

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        status = pWaveSystemManagementGetYangUserInterfaceMessage->getCompletionStatus ();
    }
    else
    {
        status = frameworkStatus;
    }

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        pCommandLineInterfaceMessage->addToOutput (pWaveSystemManagementGetYangUserInterfaceMessage->getYangUserInterface ());

        pCommandLineInterfaceMessage->setCommandLineInterfaceOperationStatus (WAVE_CLI_OPERATION_SUCCESS);
    }
    else
    {
        pCommandLineInterfaceMessage->setCommandLineInterfaceOperationStatus (WAVE_CLI_OPERATION_ERROR);
    }

    pCommandLineInterfaceMessage->setCommandLineInterfaceOperation (WAVE_CLI_OPERATION_RESPONSE);

    (*m_pServerStreamingSocket) << pCommandLineInterfaceMessage;

    delete pWaveSystemManagementGetYangUserInterfaceMessage;
    delete pCommandLineInterfaceMessage;
}

void CommandLineInterfaceReceiverWorkerThread::processCommandLineInterfaceMessageToShowRunningConfiguration (CommandLineInterfaceMessage *pCommandLineInterfaceMessage)
{
    ResourceId operationCode = pCommandLineInterfaceMessage->getOperationcode ();

    prismAssert (WAVE_CLI_CLIENT_OPERATION_SHOW_RUNNING_CONFIGURATION == operationCode, __FILE__, __LINE__);

    string targetNodeName = (pCommandLineInterfaceMessage->getInput ())[0];
    string ttyName        = pCommandLineInterfaceMessage->getTtyName ();

    cout << "\"" << targetNodeName << "\"\r\n";

    WaveSystemManagementDisplayConfigurationByTargetNodeNameMessage *pWaveSystemManagementDisplayConfigurationByTargetNodeNameMessage = new WaveSystemManagementDisplayConfigurationByTargetNodeNameMessage (targetNodeName);

    prismAssert (NULL != pWaveSystemManagementDisplayConfigurationByTargetNodeNameMessage, __FILE__, __LINE__);
    
    pWaveSystemManagementDisplayConfigurationByTargetNodeNameMessage->setTtyName (ttyName);

    ResourceId status = send (pWaveSystemManagementDisplayConfigurationByTargetNodeNameMessage, reinterpret_cast<PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread> (&CommandLineInterfaceReceiverWorkerThread::showRunningConfigurationCallback), pCommandLineInterfaceMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        showRunningConfigurationCallback (FRAMEWORK_ERROR, pWaveSystemManagementDisplayConfigurationByTargetNodeNameMessage, pCommandLineInterfaceMessage);
    }
}

void CommandLineInterfaceReceiverWorkerThread::showRunningConfigurationCallback (FrameworkStatus frameworkStatus, WaveSystemManagementDisplayConfigurationByTargetNodeNameMessage *pWaveSystemManagementDisplayConfigurationByTargetNodeNameMessage, void *pContext)
{
    CommandLineInterfaceMessage *pCommandLineInterfaceMessage = reinterpret_cast<CommandLineInterfaceMessage *> (pContext);
    ResourceId                   status                       = WAVE_MESSAGE_ERROR;

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        status = pWaveSystemManagementDisplayConfigurationByTargetNodeNameMessage->getCompletionStatus ();
    }
    else
    {
        status = frameworkStatus;
    }

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        pCommandLineInterfaceMessage->setCommandLineInterfaceOperationStatus (WAVE_CLI_OPERATION_SUCCESS);
    }
    else
    {
        pCommandLineInterfaceMessage->setCommandLineInterfaceOperationStatus (WAVE_CLI_OPERATION_ERROR);
    }

    pCommandLineInterfaceMessage->setCommandLineInterfaceOperation (WAVE_CLI_OPERATION_RESPONSE);

    (*m_pServerStreamingSocket) << pCommandLineInterfaceMessage;

    delete pWaveSystemManagementDisplayConfigurationByTargetNodeNameMessage;
    delete pCommandLineInterfaceMessage;
}


WaveThreadStatus CommandLineInterfaceReceiverWorkerThread::start()
{
    trace (TRACE_LEVEL_DEVEL, "CommandLineInterfaceReceiverWorkerThread::start : Starting ...");

    processMessages ();

    delete this;

    return (WAVE_THREAD_SUCCESS);
}

WaveMessageStatus CommandLineInterfaceReceiverWorkerThread::send (PrismMessage *pPrismMessage, PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, void *pPrismMessageContext, UI32 timeOutInMilliSeconds, LocationId locationId)
{
    return (m_pCommandLineInterfaceReceiverObjectManager->send (pPrismMessage, prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, pPrismMessageContext, this, timeOutInMilliSeconds, locationId));
}

}
