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

#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceReceiverObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceReceiverThread.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceReceiverWorkerThread.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceWorkerMessage.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceWorkerSendContext.h"

namespace WaveNs
{

CommandLineInterfaceReceiverObjectManager::CommandLineInterfaceReceiverObjectManager ()
    : WaveLocalObjectManagerForUserSpecificTasks    (getServiceName ()),
      m_pServerSocketForCommandLineInterfaceClients (NULL),
      m_pCommandLineInterfaceReceiverThread         (NULL)
{
    setAllowAutomaticallyUnlistenForEvents (false);

    restrictMessageHistoryLogging (false, false, false);
}

CommandLineInterfaceReceiverObjectManager::~CommandLineInterfaceReceiverObjectManager ()
{
}

string CommandLineInterfaceReceiverObjectManager::getServiceName ()
{
    return ("Command Line Interface Receiver");
}

CommandLineInterfaceReceiverObjectManager *CommandLineInterfaceReceiverObjectManager::getInstance ()
{
    static CommandLineInterfaceReceiverObjectManager *pCommandLineInterfaceReceiverObjectManager = new CommandLineInterfaceReceiverObjectManager ();

    WaveNs::prismAssert (NULL != pCommandLineInterfaceReceiverObjectManager, __FILE__, __LINE__);

    return (pCommandLineInterfaceReceiverObjectManager);
}

PrismServiceId CommandLineInterfaceReceiverObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void CommandLineInterfaceReceiverObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    m_pServerSocketForCommandLineInterfaceClients = new ServerStreamingSocket (FrameworkToolKit::getCommandLineInterfaceReceiverPort (), 1);

    prismAssert (NULL != m_pServerSocketForCommandLineInterfaceClients, __FILE__, __LINE__);

    status = m_pServerSocketForCommandLineInterfaceClients->getStatus ();

    if (SERVER_STREAMING_SOCKET_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "CommandLineInterfaceReceiverObjectManager::initialize : Could not bind to the Command Line Interface Receiver Port.  Please make sure that this port is not used by any other application.  Otherwise specify a different port for this purpose.");

        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "CommandLineInterfaceReceiverObjectManager::initialize : Successfully bound to the Management Interface Receiver Port");

        status = WAVE_MESSAGE_SUCCESS;
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void CommandLineInterfaceReceiverObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void CommandLineInterfaceReceiverObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&CommandLineInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void CommandLineInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
{
    // Since we told framework not to unlistenEvents, we must explicitly unlisten for events since we are going to go into an infinite while loop.

    unlistenEvents ();

    reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));

    m_pCommandLineInterfaceReceiverThread = new CommandLineInterfaceReceiverThread (this, m_pServerSocketForCommandLineInterfaceClients);

    prismAssert (NULL != m_pCommandLineInterfaceReceiverThread, __FILE__, __LINE__);

    m_pCommandLineInterfaceReceiverThread->run ();
}

WaveMessageStatus CommandLineInterfaceReceiverObjectManager::send (PrismMessage *pPrismMessage, PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, void *pPrismMessageContext, CommandLineInterfaceReceiverWorkerThread *pCommandLineInterfaceReceiverWorkerThread, UI32 timeOutInMilliSeconds, LocationId locationId)
{
    if (NULL != pCommandLineInterfaceReceiverWorkerThread)
    {
        addToCommandLineInterfaceReceiverMapsByMessage (pPrismMessage, pCommandLineInterfaceReceiverWorkerThread, prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, pPrismMessageContext);

        return (WaveObjectManager::send (pPrismMessage, reinterpret_cast<PrismMessageResponseHandler> (&CommandLineInterfaceReceiverObjectManager::sendCallback), pPrismMessageContext, timeOutInMilliSeconds, locationId, this));
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    return (WAVE_MESSAGE_ERROR);
}

void CommandLineInterfaceReceiverObjectManager::sendCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    CommandLineInterfaceReceiverWorkerThread                               *pCommandLineInterfaceReceiverWorkerThread                              = NULL;
    PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread  prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread = NULL;
    void                                                                   *pPrismMessageContext                                                   = NULL;

    getCommandLineInterfaceReceiverDetailsForMessage (pPrismMessage, pCommandLineInterfaceReceiverWorkerThread, prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, pPrismMessageContext);

    prismAssert (NULL != pCommandLineInterfaceReceiverWorkerThread,                              __FILE__, __LINE__);
    prismAssert (NULL != prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, __FILE__, __LINE__);
    prismAssert (NULL != pPrismMessageContext,                                                   __FILE__, __LINE__);

    removeFromCommandLineInterfaceReceiverMapsByMessage (pPrismMessage);

    CommandLineInterfaceWorkerSendContext *pCommandLineInterfaceWorkerSendContext = new CommandLineInterfaceWorkerSendContext (frameworkStatus, pPrismMessage, prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, pPrismMessageContext);

    prismAssert (NULL != pCommandLineInterfaceWorkerSendContext, __FILE__, __LINE__);

    CommandLineInterfaceWorkerMessage *pCommandLineInterfaceWorkerMessage = new CommandLineInterfaceWorkerMessage (pCommandLineInterfaceWorkerSendContext);

    prismAssert (NULL != pCommandLineInterfaceWorkerMessage, __FILE__, __LINE__);

    pCommandLineInterfaceReceiverWorkerThread->submitCommandLineInterfaceWorkerMessage (pCommandLineInterfaceWorkerMessage);
}

bool CommandLineInterfaceReceiverObjectManager::isAMessageSentByThisObjectManager (PrismMessage *pPrismMessage)
{
    UI32                                                                  messageId  = pPrismMessage->getMessageId                                   ();
    map<UI32, CommandLineInterfaceReceiverWorkerThread *>::const_iterator element    = m_commandLineInterfaceReceiverWorkerThreadMapByMessageId.find (messageId);
    map<UI32, CommandLineInterfaceReceiverWorkerThread *>::const_iterator endElement = m_commandLineInterfaceReceiverWorkerThreadMapByMessageId.end  ();
    bool                                                                  isKnown    = false;

    if (endElement == element)
    {
        isKnown = false;
    }
    else
    {
        isKnown = true;
    }

    return (isKnown);
}

void CommandLineInterfaceReceiverObjectManager::addToCommandLineInterfaceReceiverMapsByMessage (PrismMessage *pPrismMessage, CommandLineInterfaceReceiverWorkerThread *pCommandLineInterfaceReceiverWorkerThread, PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, void *pPrismMessageContext)
{
    prismAssert (NULL != pPrismMessage,                             __FILE__, __LINE__);
    prismAssert (NULL != pCommandLineInterfaceReceiverWorkerThread, __FILE__, __LINE__);

    bool isKnown = isAMessageSentByThisObjectManager (pPrismMessage);

    if (true == isKnown)
    {
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        UI32 messageId = pPrismMessage->getMessageId ();

        m_commandLineInterfaceReceiverWorkerThreadMapByMessageId[messageId] = pCommandLineInterfaceReceiverWorkerThread;
        m_commandLineInterfaceReceiverCallbackMapByMessageId[messageId]     = prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread;
        m_commandLineInterfaceReceiverContextMapByMessageId[messageId]      = pPrismMessageContext;
    }
}

void CommandLineInterfaceReceiverObjectManager::getCommandLineInterfaceReceiverDetailsForMessage    (PrismMessage *pPrismMessage, CommandLineInterfaceReceiverWorkerThread *&pCommandLineInterfaceReceiverWorkerThread, PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread &prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, void *&pPrismMessageContext)
{
    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    bool isKnown = isAMessageSentByThisObjectManager (pPrismMessage);

    pCommandLineInterfaceReceiverWorkerThread                              = NULL;
    prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread = NULL;
    pPrismMessageContext                                                   = NULL;

    if (false == isKnown)
    {
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        UI32 messageId = pPrismMessage->getMessageId ();

        pCommandLineInterfaceReceiverWorkerThread                              = m_commandLineInterfaceReceiverWorkerThreadMapByMessageId[messageId];
        prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread = m_commandLineInterfaceReceiverCallbackMapByMessageId[messageId];
        pPrismMessageContext                                                   = m_commandLineInterfaceReceiverContextMapByMessageId[messageId];

        prismAssert (NULL != pCommandLineInterfaceReceiverWorkerThread,                              __FILE__, __LINE__);
        prismAssert (NULL != prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, __FILE__, __LINE__);
        prismAssert (NULL != pPrismMessageContext,                                                   __FILE__, __LINE__);
    }
}

void CommandLineInterfaceReceiverObjectManager::removeFromCommandLineInterfaceReceiverMapsByMessage (PrismMessage *pPrismMessage)
{
    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    bool isKnown = isAMessageSentByThisObjectManager (pPrismMessage);

    if (false == isKnown)
    {
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        bool isLastReply = pPrismMessage->getIsLastReply ();

        if (true == isLastReply)
        {
            m_commandLineInterfaceReceiverWorkerThreadMapByMessageId.erase (pPrismMessage->getMessageId ());

            UI32 messageId = pPrismMessage->getMessageId ();

            m_commandLineInterfaceReceiverWorkerThreadMapByMessageId.erase (messageId);
            m_commandLineInterfaceReceiverCallbackMapByMessageId.erase     (messageId);
            m_commandLineInterfaceReceiverContextMapByMessageId.erase      (messageId);
        }
    }
}

}
