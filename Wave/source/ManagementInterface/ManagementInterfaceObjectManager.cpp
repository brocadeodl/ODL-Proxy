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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "ManagementInterface/ManagementInterfaceObjectManager.h"
#include "ManagementInterface/ManagementInterfaceReceiverObjectManager.h"
#include "ManagementInterface/ManagementInterfaceTypes.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/WaveManagementClient.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Shell/ShellObjectManager.h"
#include "Shell/ShellDebug.h"
#include "Shell/ShellPrism.h"
#include "ManagementInterface/ClientInterface/ManagementInterfaceClientListMessage.h"


namespace WaveNs
{

ManagementInterfaceObjectManager::MessageMap::MessageMap ()
{
}

ManagementInterfaceObjectManager::MessageMap::~MessageMap ()
{
}

void ManagementInterfaceObjectManager::MessageMap::lockAccess ()
{
    m_accessMutex.lock ();
}

void ManagementInterfaceObjectManager::MessageMap::unlockAccess ()
{
    m_accessMutex.unlock ();
}

WaveMessageStatus ManagementInterfaceObjectManager::MessageMap::addMessage (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    lockAccess ();

    UI32                        prismMessageId                  = pManagementInterfaceMessage->getMessageId ();
    ManagementInterfaceMessage *pTempManagementInterfaceMessage = findMessage (prismMessageId);
    WaveMessageStatus           status                          = WAVE_MESSAGE_SUCCESS;

    if (NULL == pTempManagementInterfaceMessage)
    {
        m_messagesMap[prismMessageId] = pManagementInterfaceMessage;
        status                        = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        status = WAVE_MESSAGE_ERROR_MESSAGE_ALREADY_EXISTS_IN_MAP;
    }

    unlockAccess ();

    return (status);
}

ManagementInterfaceMessage *ManagementInterfaceObjectManager::MessageMap::findMessage (UI32 prismMessageId)
{
    map<UI32, ManagementInterfaceMessage *>::iterator  element                     = m_messagesMap.find (prismMessageId);
    map<UI32, ManagementInterfaceMessage *>::iterator  end                         = m_messagesMap.end ();
    ManagementInterfaceMessage                        *pManagementInterfaceMessage = NULL;

    if (end != element)
    {
        pManagementInterfaceMessage = element->second;
    }

    return (pManagementInterfaceMessage);
}

ManagementInterfaceMessage *ManagementInterfaceObjectManager::MessageMap::removeMessage (UI32 prismMessageId)
{
    lockAccess ();

    map<UI32, ManagementInterfaceMessage *>::iterator element                      = m_messagesMap.find (prismMessageId);
    map<UI32, ManagementInterfaceMessage *>::iterator end                          = m_messagesMap.end ();
    ManagementInterfaceMessage                        *pManagementInterfaceMessage = NULL;

    if (end != element)
    {
        pManagementInterfaceMessage = element->second;
        m_messagesMap.erase (element);
    }

    unlockAccess ();

    return (pManagementInterfaceMessage);
}

void ManagementInterfaceObjectManager::MessageMap::getPendingMessagesForClient (const UI32 &clientId, vector<ManagementInterfaceMessage *> &messagesVector)
{
    lockAccess ();

    map<UI32, ManagementInterfaceMessage *>::iterator  element                     = m_messagesMap.begin ();
    map<UI32, ManagementInterfaceMessage *>::iterator  end                         = m_messagesMap.end ();
    ManagementInterfaceMessage                        *pManagementInterfaceMessage = NULL;

    while (element != end)
    {
        pManagementInterfaceMessage = element->second;

        if ((pManagementInterfaceMessage->getClientId ()) == clientId)
        {
            messagesVector.push_back (pManagementInterfaceMessage);
        }

        element++;
    }

    UI32 numberOfPendingMessages = messagesVector.size ();
    UI32 i;

    for (i = 0; i < numberOfPendingMessages; i++)
    {
        m_messagesMap.erase ((messagesVector[i])->getMessageId ());
    }

    unlockAccess ();
}

ManagementInterfaceObjectManager::ManagementInterfaceObjectManager ()
    : WaveLocalObjectManager ("Management Interface")
{
    setTraceLevel (TRACE_LEVEL_ERROR);

    addOperationMap (WAVE_OBJECT_MANAGER_ANY_OPCODE, reinterpret_cast<PrismMessageHandler> (&ManagementInterfaceObjectManager::managementInterfaceMessageHandler));

    addOperationMap (MANAGEMENTINTERFACE_CLIENT_LIST, reinterpret_cast<PrismMessageHandler> (&ManagementInterfaceObjectManager::managementInterfaceClientListHandler));

    // restrictMessageHistoryLogging                (bool messageHistoryLogInsideSend, bool messageHistoryLogInsideReply, bool messageHistoryLogInsideHandleMessage);
    restrictMessageHistoryLogging                (false, false, false);
}

ManagementInterfaceObjectManager::~ManagementInterfaceObjectManager ()
{
}

ManagementInterfaceObjectManager *ManagementInterfaceObjectManager::getInstance ()
{
    static ManagementInterfaceObjectManager *pManagementInterfaceObjectManager = NULL;

    if (NULL == pManagementInterfaceObjectManager)
    {
        pManagementInterfaceObjectManager = new ManagementInterfaceObjectManager ();
        WaveNs::prismAssert (NULL != pManagementInterfaceObjectManager, __FILE__, __LINE__);
    }

    return (pManagementInterfaceObjectManager);
}

PrismServiceId ManagementInterfaceObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string ManagementInterfaceObjectManager::getServiceName ()
{
    return ("Management Interface");
}


PrismMessage *ManagementInterfaceObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
 
    switch (operationCode)
    {
        case  MANAGEMENTINTERFACE_CLIENT_LIST:
            pPrismMessage = new ManagementInterfaceClientListMessage();
            break;
        default :
           pPrismMessage = NULL;
    }
 
    return (pPrismMessage);
}


void ManagementInterfaceObjectManager::managementInterfaceMessageHandler (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    m_pInputMessage = NULL;

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&ManagementInterfaceObjectManager::managementInterfaceMessagePostToClientStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ManagementInterfaceObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ManagementInterfaceObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pManagementInterfaceMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void ManagementInterfaceObjectManager::managementInterfaceClientListHandler (ManagementInterfaceClientListMessage *pManagementInterfaceClientListMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&ManagementInterfaceObjectManager::getClientsInformation),
        reinterpret_cast<PrismLinearSequencerStep> (&ManagementInterfaceObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ManagementInterfaceObjectManager::prismLinearSequencerFailedStep)
    };
 
    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pManagementInterfaceClientListMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
 
    pPrismLinearSequencerContext->start ();
}    

void ManagementInterfaceObjectManager::getClientsInformation (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    ManagementInterfaceClientListMessage *pManagementInterfaceClientListMessage = dynamic_cast<ManagementInterfaceClientListMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    
    prismAssert( NULL != pManagementInterfaceClientListMessage , __FILE__ , __LINE__);

    vector<string> clientsConnected = m_clientsMap.getClientsConnected ();

    pManagementInterfaceClientListMessage->setConnectedClients (clientsConnected);
     
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void ManagementInterfaceObjectManager::managementInterfaceMessagePostToClientStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    lockAccess1 ();

    trace (TRACE_LEVEL_DEVEL, "ManagementInterfaceObjectManager::managementInterfaceMessagePostToClientStep : Starting ...");

    ManagementInterfaceMessage *pManagementInterfaceMessage = dynamic_cast<ManagementInterfaceMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert( NULL != pManagementInterfaceMessage , __FILE__ , __LINE__); 

    bool                        isMessageCached       = false;
    ResourceId                  status;
    bool                        isOneWayMessage       = pManagementInterfaceMessage->getIsOneWayMessage ();
    UI32                        timeOutInMilliSeconds = pManagementInterfaceMessage->getTimeOutInMilliSeconds ();
    UI32                        messageId             = pManagementInterfaceMessage->getMessageId ();

    // Cache the message before posting it to client if the message is not one way message.
    // for one way messages we do not cache the clinet id because we do not care about the replies.
    // In this case we must not receive the replies to one way messages.

    if (false == isOneWayMessage)
    {
        isMessageCached = true;
        status          = m_remoteMessagesMap.addMessage (pManagementInterfaceMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "ManagementInterfaceObjectManager::managementInterfaceMessagePostToClientStep : Could not cache a message after sending out to a client.");
            prismAssert (false, __FILE__, __LINE__);

            // FIXME : sagar : If we are not able to cache it.  What to do???
        }
    }

    status = postToClient (pManagementInterfaceMessage);

    if (WAVE_MGMT_INTF_POST_TO_CLIENT_SUCCEEDED != status)
    {
        trace (TRACE_LEVEL_DEVEL, "ManagementInterfaceObjectManager::managementInterfaceMessagePostToClientStep : Could not post message to client.");

        // Since we have already cached this message remove it from the remote messages map.

        ManagementInterfaceMessage *pTempManagementInterfaceMessage = NULL;

        if (true == isMessageCached)
        {
            pTempManagementInterfaceMessage = m_remoteMessagesMap.removeMessage (messageId);
            prismAssert (pTempManagementInterfaceMessage == pManagementInterfaceMessage, __FILE__, __LINE__);
        }

        pManagementInterfaceMessage->setCompletionStatus (status);
        reply (pManagementInterfaceMessage);
    }
    else
    {
        // We are able to successfully send the message across network.

        // Now we must delete if the message that we sent across is a one way message.  For oneway messages, we will never receive replies across boundaries.
        // We did not cache it (in case of oneway messages) so we can simply delete the message.

        // WARNING !!!!!  WARNING !!!!!  WARNING !!!!!

        // We must not access the pManagementInterfaceMessage in this member function beyond this comment since it can be concurrently accesed on a receiver thread
        // if the reply to the posted message is already arrived.  However there is one exception:  If we are sure of deleting the message due to the
        // message being a one way message it can be safely deleted since there will no replies to the message that will be received on the receiver
        // thread.  Again, however the pManagementInterfaceMessage must not be accessed for any other reason.  that is why we cache the value in isOneWayMessage and test
        // it against that variable.

        if (true == isOneWayMessage)
        {
            delete pManagementInterfaceMessage;
        }

        if (0 != timeOutInMilliSeconds)
        {
           TimerHandle timerHandle;
 
            ResourceId timeStatus = startTimer (timerHandle, timeOutInMilliSeconds, reinterpret_cast<PrismTimerExpirationHandler> (&ManagementInterfaceObjectManager::sendTimerExpiredCallback), reinterpret_cast<void *> (messageId));
 
            if (FRAMEWORK_SUCCESS != timeStatus)
            {
                prismAssert (false, __FILE__, __LINE__);
            }
        }
    }

    unlockAccess1 ();

    pPrismLinearSequencerContext->setPPrismMessage (NULL);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void ManagementInterfaceObjectManager::sendTimerExpiredCallback (TimerHandle timerHandle, void *pContext)
{
    trace(TRACE_LEVEL_DEVEL,"ManagementInterfaceObjectManager::sendTimerExpiredCallback entering ..");
 
    UI32 messageId = reinterpret_cast<ULI> (pContext);
    
    PrismMessage *pPrismMessage = getPendingMessage (messageId);

    if (NULL != pPrismMessage)
    {
        trace(TRACE_LEVEL_INFO,"ManagementInterfaceObjectManager::sendTimerExpiredCallback Message is not NULL");

        pPrismMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_SEND_TIMEDOUT);
        reply (pPrismMessage);
    }
}

void ManagementInterfaceObjectManager::lockAccess ()
{
    m_accessMutexForReceiverThreads.lock ();
}

void ManagementInterfaceObjectManager::unlockAccess ()
{
    m_accessMutexForReceiverThreads.unlock ();
}

void ManagementInterfaceObjectManager::lockAccess1 ()
{
    m_accessMutexForMessagesMap.lock ();
}

void ManagementInterfaceObjectManager::unlockAccess1 ()
{
    m_accessMutexForMessagesMap.unlock ();
}

WaveMessageStatus ManagementInterfaceObjectManager::sendToBeUsedByReceiverThreads (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    // This method must be protected with locking mechanism since it can be executed from mutiple receiver threads.

    lockAccess ();

    //trace (TRACE_LEVEL_DEVEL, "ManagementInterfaceObjectManager::sendToBeUsedByReceiverThreads : We are delivering a Remote Message that arrived.");

    WaveMessageStatus status = WAVE_MESSAGE_ERROR;

    // We distinguish between normally sent messages verses one way messages.

    if (false == (pManagementInterfaceMessage->getIsOneWayMessage ()))
    {
        status = send (pManagementInterfaceMessage,
                    reinterpret_cast<PrismMessageResponseHandler> (&ManagementInterfaceObjectManager::callbackForSendUsedByReceiverThreads),
                    NULL);
    }
    else
    {
        // If the original message was sent as a one way message send it as one way.  We will never receive a response for this message.

        status = sendOneWay (pManagementInterfaceMessage);
    }

    unlockAccess ();


    if (WAVE_MESSAGE_SUCCESS != status)
    {
        if (false == (pManagementInterfaceMessage->getIsOneWayMessage ()))
        {
            pManagementInterfaceMessage->setCompletionStatus (status);

            // We indicate that the FrameworkStatus is success and the actual failure is stored in the completion status of the message.

            callbackForSendUsedByReceiverThreads (FRAMEWORK_SUCCESS, pManagementInterfaceMessage, NULL);
        }
        else
        {
            // Delete the one way message

            delete pManagementInterfaceMessage;
        }
    }

    return (status);
}

void ManagementInterfaceObjectManager::callbackForSendUsedByReceiverThreads (FrameworkStatus frameworkStatus, ManagementInterfaceMessage *pManagementInterfaceMessage, void *pContext)
{
    static PrismMutex callbackMutex;

    callbackMutex.lock ();

    // This method is executed via normal reply mechanism.  So only one reply at a time will be processed.  So for each of the message
    // responses that are there in the queues it will be executed once and in serial order.  But this method can be executed by one of the
    // receiver threads as well in case of a send failure when sending using sendToBeUsedByReceiverThreads.  So we must protect this method
    // with locking mechanism.

    //trace (TRACE_LEVEL_DEVEL, "ManagementInterfaceObjectManager::callbackForSendUsedByReceiverThreads : Received response to a remote message we delivered earlier.");

    // Ensure that we do have a valid message.  In some cases framework can callback with a NULL message pointer (in case of a message timeout).
    // but we know that we never requested a timeout period for this message while sending this message using send.  So make sure that the message
    // that we got back is a non NULL message.

    prismAssert (NULL != pManagementInterfaceMessage, __FILE__, __LINE__);

    pManagementInterfaceMessage->setType (WAVE_MESSAGE_TYPE_RESPONSE);

    // Ensure that there is no context associated with this reposnse.

    prismAssert (NULL == pContext, __FILE__, __LINE__);

    // The response we recieved is destined for a Management Interface Client.  So deliver it to that client.
    // We must not cache this since it is a response.  We assume that we do not have to track this any more.  We are simply
    // acting as a remote transport agent.

    if (false == (pManagementInterfaceMessage->getDropReplyAcrossLocations ()))
    {
        ResourceId status = postToClient (pManagementInterfaceMessage);

        if (WAVE_MGMT_INTF_POST_TO_CLIENT_SUCCEEDED != status)
        {
            trace (TRACE_LEVEL_DEVEL, "ManagementInterfaceObjectManager::callbackForSendUsedByReceiverThreads : We could not deliver a response destined for remote location.  In that case currently we simply drop the message.");
        }
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "ManagementInterfaceObjectManager::callbackForSendUsedByReceiverThreads : We dropped a reply across location boundary.");
    }

    callbackMutex.unlock ();

    // Destroy the message in any case.  This is an intermediate message that we created on this location upon receving a remote message.
    // We created this message so that we can normal framework mechanisms to deliver the remote message locally to the corresponsing
    // service.  Either we succeeded sending the response to the message originating location or not we must destroy this object.

    delete pManagementInterfaceMessage;
}

ManagementInterfaceMessage *ManagementInterfaceObjectManager::getPendingMessage (const UI32 &messageId)
{
    // WARNING !!!!!  WARNING !!!!!  WARNING !!!!!

    // Care must be taken in this member function that this function should not interfere with the posting of messages to Clients.
    // Otherwise dead locks can occur while the sendig thread is waiting to post messages after it acquires lock and the received thread waits to
    // acquire the same lock so that it can receive data.  In extreme cases where the kernel buffers for the underlying sockets are full it can
    // cause deadlock conditions.

    ManagementInterfaceMessage *pManagementInterfaceMessage = NULL;

    pManagementInterfaceMessage = m_remoteMessagesMap.removeMessage (messageId);

    return (pManagementInterfaceMessage);
}

ManagementInterfaceMessage *ManagementInterfaceObjectManager::getPendingMessageWithOutRemove (const UI32 &messageId) {
    // WARNING !!!!!  WARNING !!!!!  WARNING !!!!!

    // Care must be taken in this member function that this function should not interfere with the posting of messages to Clients.     // Otherwise dead locks can occur while the sendig thread is waiting to post messages after it acquires lock and the received thread waits to
    // acquire the same lock so that it can receive data.  In extreme cases where the kernel buffers for the underlying sockets are full it can
    // cause deadlock conditions.

    ManagementInterfaceMessage *pManagementInterfaceMessage = NULL;

    pManagementInterfaceMessage = m_remoteMessagesMap.findMessage (messageId);

    return (pManagementInterfaceMessage);
} 

void ManagementInterfaceObjectManager::replyToBeUsedByReceiverThreads (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    // This method Need not be protected with locking mechanism though it can be executed from mutiple receiver threads.

    prismAssert (NULL != pManagementInterfaceMessage, __FILE__, __LINE__);

    if (NULL == pManagementInterfaceMessage)
    {
        trace (TRACE_LEVEL_ERROR, "ManagementInterfaceObjectManager::replyToBeUsedByReceiverThreads : Some Management Interface Client is trying to forward a NULL Client response.  We are simply droping the response.");
        return;
    }

    reply (pManagementInterfaceMessage);

    return;
}

void ManagementInterfaceObjectManager::replyToBeUsedByReceiverThreads (UI32 prismMessageId)
{
    // This method must be protected with locking mechanism since it can be executed from mutiple receiver threads.

    PrismMessage *pPrismMessage = m_remoteMessagesMap.removeMessage (prismMessageId);

    if (NULL == pPrismMessage)
    {
        trace (TRACE_LEVEL_ERROR, "ManagementInterfaceObjectManager::replyToBeUsedByReceiverThreads : Some one is trying to forward a remote response to a message that does not exist.");
        return;
    }

    reply (pPrismMessage);

    return;
}

void ManagementInterfaceObjectManager::getPendingMessagesForClient (const UI32 &clientId, vector<ManagementInterfaceMessage *> &messagesVector)
{
    // This method must be protected with lockAccess1 since this will be executed on the remote message receiver thread.  This member function
    // must not be executed before the corresponding interLocationMessageTransportHandler is completed executing.

    lockAccess1 ();

    m_remoteMessagesMap.getPendingMessagesForClient (clientId, messagesVector);

    unlockAccess1 ();
}

void ManagementInterfaceObjectManager::replyToRemoteMessagesPendingForClient (const UI32 &clientId, ResourceId completionStatus)
{
    vector<ManagementInterfaceMessage*> messagesPendingOnClient;
    UI32                                numberOfPendingMessages          = 0;
    UI32                                i                                = 0;

    // We lock this so that we find all the pending messages and also reply to those messages in one go.

    getPendingMessagesForClient (clientId, messagesPendingOnClient);
    numberOfPendingMessages = messagesPendingOnClient.size ();

    trace (TRACE_LEVEL_DEBUG, string ("ManagementInterfaceObjectManager::replyToRemoteMessagesPendingForClient : Replying to all ") + numberOfPendingMessages + string (" pending messages for Client : ") + clientId);

    for (i = 0; i < numberOfPendingMessages; i++)
    {
        (messagesPendingOnClient[i])->setCompletionStatus (completionStatus);

	// reply to clients msg sender
        replyToBeUsedByReceiverThreads (messagesPendingOnClient[i]);
    }
}

ResourceId ManagementInterfaceObjectManager::connectToNewManagementInterfaceClient (const string &clientName, const string &clientIpAddress, const SI32 &clientPort, const PrismServiceId &userInterfaceServiceId, const string &serverIpAddressFromClientPerspective, const string &messageVersion, const UI32 &serverUniqueIdentifierFromServerPerspective)
{
    WaveManagementClient *pWaveManagementClient = new WaveManagementClient (clientName, clientIpAddress, clientPort, userInterfaceServiceId);
    ResourceId            status                = FRAMEWORK_ERROR;

    status = pWaveManagementClient->connect (serverIpAddressFromClientPerspective, 15, 15, serverUniqueIdentifierFromServerPerspective);

    if (FRAMEWORK_SUCCESS == status)
    {
        pWaveManagementClient->updateMessageVersionAndSerializationTypeForSocket (messageVersion);

        m_clientsMap.addClient (pWaveManagementClient);
    }
    else
    {
        delete pWaveManagementClient;
    }

    return (status);
}

WaveManagementClient *ManagementInterfaceObjectManager::getClient (const string &clientIpAddress, const SI32 &clientPort)
{
    return (m_clientsMap.getClient (clientIpAddress, clientPort));
}

bool ManagementInterfaceObjectManager::isAKnownClient (const string &clientName)
{
    return (m_clientsMap.isAKnownClient (clientName));
}

bool ManagementInterfaceObjectManager::isAKnownClient (const string &ipAddress, const UI32 &port)
{
    return (m_clientsMap.isAKnownClient (ipAddress, port));
}

UI32 ManagementInterfaceObjectManager::getClientId (const string &clientIpAddress, const SI32 &clientPort)
{
    return (m_clientsMap.getClientId (clientIpAddress, clientPort));
}

UI32 ManagementInterfaceObjectManager::getClientId (const string &clientName)
{
    return (m_clientsMap.getClientId (clientName));
}

ResourceId ManagementInterfaceObjectManager::postToClient (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    status = m_clientsMap.post (pManagementInterfaceMessage);

    if (WAVE_MGMT_INTF_POST_TO_CLIENT_SUCCEEDED != status)
    {
        trace (TRACE_LEVEL_ERROR, "ManagementInterfaceObjectManager::postToClient : Could not post the message to client : Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId ManagementInterfaceObjectManager::postToClientWrapper (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    return ((ManagementInterfaceObjectManager::getInstance ())->postToClient (pManagementInterfaceMessage));
}

void ManagementInterfaceObjectManager::disconnectFromClient (const UI32 &clientId)
{
    WaveManagementClient *pWaveManagementClient = m_clientsMap.removeClient (clientId);

    if (NULL != pWaveManagementClient)
    {
        // Delete will indicate and close the socket as well.

        delete pWaveManagementClient;

        replyToRemoteMessagesPendingForClient (clientId, WAVE_MGMT_INTF_CLIENT_NOT_AVAILABLE);
    }
    else
    {
        trace (TRACE_LEVEL_WARN, string ("ManagementInterfaceObjectManager::disconnectFromClient : Already disconnected from client with id : ") + clientId);
    }
}

void ManagementInterfaceObjectManager::disconnectFromClient (const string &clientIpAddress, const SI32 &clientPort)
{
    WaveManagementClient *pWaveManagementClient = m_clientsMap.removeClient (clientIpAddress, clientPort);

    if (NULL != pWaveManagementClient)
    {
        UI32 clientId = pWaveManagementClient->getId ();

        // Delete will indicate and close the socket as well.

        delete pWaveManagementClient;

        replyToRemoteMessagesPendingForClient (clientId, WAVE_MGMT_INTF_CLIENT_NOT_AVAILABLE);
    }
    else
    {
        trace (TRACE_LEVEL_WARN, string ("ManagementInterfaceObjectManager::disconnectFromClient : Already disconnected from client : ") + clientIpAddress + string (":") + clientPort);
    }
}


void ManagementInterfaceObjectManager::disconnectFromClient (const string &clientIpAddress)
{
    lockAccess ();
    vector <WaveManagementClient *>*pWaveManagementClients = m_clientsMap.removeAllClient (clientIpAddress);
	prismAssert (NULL != pWaveManagementClients, __FILE__, __LINE__);
	
    UI32 numberofClients = pWaveManagementClients->size ();
    UI32 i = 0;
    for (i = 0; i < numberofClients; i++)
    {
	WaveManagementClient *pWaveManagementClient =
		 (dynamic_cast<WaveManagementClient *> ((*pWaveManagementClients)[i]));

	if (NULL != pWaveManagementClient)
	{
	    UI32 clientId = pWaveManagementClient->getId ();
            SI32 clientPort = pWaveManagementClient->getPort ();
            trace (TRACE_LEVEL_INFO, string("ManagementInterfaceObjectManager::disconnectFromClient ") +
		    string("IPAddress:") + clientIpAddress + string(" port:") + clientPort +
		   string(" name:") + pWaveManagementClient->getName ());

            m_clientsMap.removeClient (clientId);

	    // Delete will indicate and close the socket as well.
	    delete pWaveManagementClient;

	    replyToRemoteMessagesPendingForClient (clientId, WAVE_MGMT_INTF_CLIENT_NOT_AVAILABLE);

            (ManagementInterfaceReceiverObjectManager::getInstance ())->terminateAndRemoveManagemntInterfaceReceiverThreadFromCache (clientIpAddress, clientPort);

	}
	else
	{
	    trace (TRACE_LEVEL_WARN, string ("ManagementInterfaceObjectManager::disconnectFromClient : Already disconnected from client with IP: ") + clientIpAddress);
	}
    }

    if (pWaveManagementClients != NULL)
	    delete pWaveManagementClients;

    unlockAccess ();
}

void ManagementInterfaceObjectManager::removeInstanceClient (UI32 argc, vector<string> argv)
{
    WaveMessageStatus status = WAVE_MESSAGE_ERROR;
    FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage; 

    if (argc <= 1) {
	 WaveNs:: trace (TRACE_LEVEL_ERROR, string("ManagementInterfaceObjectManager::removeInstanceClient: ipaddress missing"));
	  return;
    }

    WaveNs::trace (TRACE_LEVEL_INFO, string ("ManagementInterfaceObjectManager::removeInstanceClient: ") + 
	   string("clientIpAddress:") + argv[1]);

    FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage.setClientIpAddress (argv[1]);

    status = ShellPrism::shellSendSynchronously (&FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
    	WaveNs::trace (TRACE_LEVEL_ERROR, "ManagementInterfaceObjectManager::removeInstanceClient : Status : " + FrameworkToolKit::localize (status));
    }
}

void ManagementInterfaceObjectManager::registerDebugShellEntries ()
{
    addDebugFunction ((ShellCmdFunction) (&ManagementInterfaceObjectManager::removeInstanceClient), "removeInstanceClient");
}

}
