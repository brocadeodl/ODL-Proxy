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

#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "ManagementInterface/ManagementInterfaceTypes.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/ClientInterface/WaveManagementServer.h"
#include "ManagementInterface/ClientInterface/WaveClientReceiverObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/Types.h"


namespace WaveNs
{

WaveClientTransportObjectManager::MessageMap::MessageMap ()
{
}

WaveClientTransportObjectManager::MessageMap::~MessageMap ()
{
}

void WaveClientTransportObjectManager::MessageMap::lockAccess ()
{
    m_accessMutex.lock ();
}

void WaveClientTransportObjectManager::MessageMap::unlockAccess ()
{
    m_accessMutex.unlock ();
}

WaveMessageStatus WaveClientTransportObjectManager::MessageMap::addMessage (ManagementInterfaceMessage *pManagementInterfaceMessage)
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

ManagementInterfaceMessage *WaveClientTransportObjectManager::MessageMap::findMessage (UI32 prismMessageId)
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

ManagementInterfaceMessage *WaveClientTransportObjectManager::MessageMap::removeMessage (UI32 prismMessageId)
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

void WaveClientTransportObjectManager::MessageMap::getPendingMessagesForServer (const UI32 &serverId, vector<ManagementInterfaceMessage *> &messagesVector)
{
    lockAccess ();

    map<UI32, ManagementInterfaceMessage *>::iterator  element                     = m_messagesMap.begin ();
    map<UI32, ManagementInterfaceMessage *>::iterator  end                         = m_messagesMap.end ();
    ManagementInterfaceMessage                        *pManagementInterfaceMessage = NULL;

    while (element != end)
    {
        pManagementInterfaceMessage = element->second;

        if ((pManagementInterfaceMessage->getServerId ()) == serverId)
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

WaveClientTransportObjectManager::WaveClientTransportObjectManager ()
    : WaveLocalObjectManager ("Wave Client Transport")
{
    setTraceLevel (TRACE_LEVEL_ERROR);

    // Explicitly removing the following generic opcodes so that the generic opcodes will fall into the "ANY_OPCODE" category and get posted to the wave server.
    removeOperationMap (WAVE_OBJECT_MANAGER_GET_DEBUG_INFORMATION);
    removeOperationMap (WAVE_OBJECT_MANAGER_RESET_DEBUG_INFORMATION);

    addOperationMap (WAVE_OBJECT_MANAGER_ANY_OPCODE, reinterpret_cast<PrismMessageHandler> (&WaveClientTransportObjectManager::managementInterfaceMessageHandler));

    // restrictMessageHistoryLogging                (bool messageHistoryLogInsideSend, bool messageHistoryLogInsideReply, bool messageHistoryLogInsideHandleMessage);
    restrictMessageHistoryLogging                (false, false, false);
}

WaveClientTransportObjectManager::~WaveClientTransportObjectManager ()
{
}

WaveClientTransportObjectManager *WaveClientTransportObjectManager:: getInstance ()
{
    static WaveClientTransportObjectManager *pWaveClientTransportObjectManager = new WaveClientTransportObjectManager ();

    WaveNs::prismAssert (NULL != pWaveClientTransportObjectManager, __FILE__, __LINE__);

    return (pWaveClientTransportObjectManager);
}

PrismServiceId WaveClientTransportObjectManager:: getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void WaveClientTransportObjectManager::managementInterfaceMessageHandler (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveClientTransportObjectManager::managementInterfaceMessagePostToServerStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveClientTransportObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveClientTransportObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pManagementInterfaceMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveClientTransportObjectManager::managementInterfaceMessagePostToServerStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    lockAccess1 ();

    trace (TRACE_LEVEL_DEVEL, "WaveClientTransportObjectManager::managementInterfaceMessagePostToServerStep : Starting ...");

    ManagementInterfaceMessage *pManagementInterfaceMessage = dynamic_cast<ManagementInterfaceMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert( NULL != pManagementInterfaceMessage , __FILE__ , __LINE__ );

    bool                        isMessageCached = false;
    ResourceId                  status;
    bool                        isOneWayMessage = pManagementInterfaceMessage->getIsOneWayMessage ();

    string                      serviceName     = pManagementInterfaceMessage->getServiceName ();
    UI32                        serverId        = pManagementInterfaceMessage->getServerId ();
    PrismServiceId              serviceId       = getPrismServiceIdForServiceName (serverId, serviceName);

    if (0 != serviceId)
    {
        pManagementInterfaceMessage->setServiceCode (serviceId);

        trace (TRACE_LEVEL_INFO, string ("Sending  Message with Service Code : ") + serviceId + string (", ServiceName : ") + serviceName + string (" to server id : ") + serverId);

        // Cache the message before posting it to server if the message is not one way message.
        // for one way messages we do not cache the server id because we do not care about the replies.
        // In this case we must not receive the replies to one way messages.

        if (false == isOneWayMessage)
        {
            isMessageCached = true;
            status          = m_remoteMessagesMap.addMessage (pManagementInterfaceMessage);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "WaveClientTransportObjectManager::managementInterfaceMessagePostToServerStep : Could not cache a message after sending out to a server.");
                prismAssert (false, __FILE__, __LINE__);

                // FIXME : sagar : If we are not able to cache it.  What to do???
            }
        }

        status = postToServer (pManagementInterfaceMessage);
    }
    else
    {
        status = WAVE_MGMT_CLIENT_INTF_SERVICE_NOT_AVAILABLE;
    }

    if (WAVE_MGMT_CLIENT_INTF_POST_TO_SERVER_SUCCEEDED != status)
    {
        trace (TRACE_LEVEL_DEVEL, "WaveClientTransportObjectManager::managementInterfaceMessagePostToServerStep : Could not post message to server.");

        // Since we have already cached this message remove it from the remote messages map.

        UI32                        messageId                       = pManagementInterfaceMessage->getMessageId ();
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
    }

    unlockAccess1 ();

    pPrismLinearSequencerContext->setPPrismMessage (NULL);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveClientTransportObjectManager::lockAccess ()
{
    m_accessMutexForReceiverThreads.lock ();
}

void WaveClientTransportObjectManager::unlockAccess ()
{
    m_accessMutexForReceiverThreads.unlock ();
}

void WaveClientTransportObjectManager::lockAccess1 ()
{
    m_accessMutexForMessagesMap.lock ();
}

void WaveClientTransportObjectManager::unlockAccess1 ()
{
    m_accessMutexForMessagesMap.unlock ();
}

WaveMessageStatus WaveClientTransportObjectManager::sendToBeUsedByReceiverThreads (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    // This method must be protected with locking mechanism since it can be executed from mutiple receiver threads.

    lockAccess ();

    //trace (TRACE_LEVEL_DEVEL, "WaveClientTransportObjectManager::sendToBeUsedByReceiverThreads : We are delivering a Remote Message that arrived.");

    WaveMessageStatus status = WAVE_MESSAGE_ERROR;

    // We distinguish between normally sent messages verses one way messages.

    if (false == (pManagementInterfaceMessage->getIsOneWayMessage ()))
    {
        status = send (pManagementInterfaceMessage,
                    reinterpret_cast<PrismMessageResponseHandler> (&WaveClientTransportObjectManager::callbackForSendUsedByReceiverThreads),
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

void WaveClientTransportObjectManager::callbackForSendUsedByReceiverThreads (FrameworkStatus frameworkStatus, ManagementInterfaceMessage *pManagementInterfaceMessage, void *pContext)
{
    static PrismMutex callbackMutex;

    callbackMutex.lock ();

    // This method is executed via normal reply mechanism.  So only one reply at a time will be processed.  So for each of the message
    // responses that are there in the queues it will be executed once and in serial order.  But this method can be executed by one of the
    // receiver threads as well in case of a send failure when sending using sendToBeUsedByReceiverThreads.  So we must protect this method
    // with locking mechanism.

    //trace (TRACE_LEVEL_DEVEL, "WaveClientTransportObjectManager::callbackForSendUsedByReceiverThreads : Received response to a remote message we delivered earlier.");

    // Ensure that we do have a valid message.  In some cases framework can callback with a NULL message pointer (in case of a message timeout).
    // but we know that we never requested a timeout period for this message while sending this message using send.  So make sure that the message
    // that we got back is a non NULL message.

    prismAssert (NULL != pManagementInterfaceMessage, __FILE__, __LINE__);

    // Ensure that there is no context associated with this reposnse.

    prismAssert (NULL == pContext, __FILE__, __LINE__);

    // The response we recieved is destined for a Management Interface Server.  So deliver it to that server.
    // We must not cache this since it is a response.  We assume that we do not have to track this any more.  We are simply
    // acting as a remote transport agent.

    if (false == (pManagementInterfaceMessage->getDropReplyAcrossLocations ()))
    {
        ResourceId status = postToServer (pManagementInterfaceMessage);

        if (WAVE_MGMT_INTF_POST_TO_CLIENT_SUCCEEDED != status)
        {
            trace (TRACE_LEVEL_DEVEL, "WaveClientTransportObjectManager::callbackForSendUsedByReceiverThreads : We could not deliver a response destined for remote location.  In that case currently we simply drop the message.");
        }
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "WaveClientTransportObjectManager::callbackForSendUsedByReceiverThreads : We dropped a reply across location boundary.");
    }

    callbackMutex.unlock ();

    // Destroy the message in any case.  This is an intermediate message that we created on this location upon receving a remote message.
    // We created this message so that we can normal framework mechanisms to deliver the remote message locally to the corresponsing
    // service.  Either we succeeded sending the response to the message originating location or not we must destroy this object.

    delete pManagementInterfaceMessage;
}

ManagementInterfaceMessage *WaveClientTransportObjectManager::getPendingMessage (const UI32 &messageId)
{
    // WARNING !!!!!  WARNING !!!!!  WARNING !!!!!

    // Care must be taken in this member function that this function should not interfere with the posting of messages to Servers.
    // Otherwise dead locks can occur while the sendig thread is waiting to post messages after it acquires lock and the received thread waits to
    // acquire the same lock so that it can receive data.  In extreme cases where the kernel buffers for the underlying sockets are full it can
    // cause deadlock conditions.

    ManagementInterfaceMessage *pManagementInterfaceMessage = NULL;

    pManagementInterfaceMessage = m_remoteMessagesMap.removeMessage (messageId);

    return (pManagementInterfaceMessage);
}

void WaveClientTransportObjectManager::replyToBeUsedByReceiverThreads (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    // This method Need not be protected with locking mechanism though it can be executed from mutiple receiver threads.

    prismAssert (NULL != pManagementInterfaceMessage, __FILE__, __LINE__);

    if (NULL == pManagementInterfaceMessage)
    {
        trace (TRACE_LEVEL_ERROR, "WaveClientTransportObjectManager::replyToBeUsedByReceiverThreads : Some Management Interface Server is trying to forward a NULL Server response.  We are simply droping the response.");
        return;
    }

    reply (pManagementInterfaceMessage);

    return;
}

void WaveClientTransportObjectManager::replyToBeUsedByReceiverThreads (UI32 prismMessageId)
{
    // This method must be protected with locking mechanism since it can be executed from mutiple receiver threads.

    PrismMessage *pPrismMessage = m_remoteMessagesMap.removeMessage (prismMessageId);

    if (NULL == pPrismMessage)
    {
        trace (TRACE_LEVEL_ERROR, "WaveClientTransportObjectManager::replyToBeUsedByReceiverThreads : Some one is trying to forward a remote response to a message that does not exist.");
        return;
    }

    reply (pPrismMessage);

    return;
}

void WaveClientTransportObjectManager::getPendingMessagesForServer (const UI32 &serverId, vector<ManagementInterfaceMessage *> &messagesVector)
{
    // This method must be protected with lockAccess1 since this will be executed on the remote message receiver thread.  This member function
    // must not be executed before the corresponding interLocationMessageTransportHandler is completed executing.

    lockAccess1 ();

    m_remoteMessagesMap.getPendingMessagesForServer (serverId, messagesVector);

    unlockAccess1 ();
}

void WaveClientTransportObjectManager::replyToRemoteMessagesPendingForServer (const UI32 &serverId, ResourceId completionStatus)
{
    vector<ManagementInterfaceMessage*> messagesPendingOnServer;
    UI32                                numberOfPendingMessages          = 0;
    UI32                                i                                = 0;

    // We lock this so that we find all the pending messages and also reply to those messages in one go.

    getPendingMessagesForServer (serverId, messagesPendingOnServer);
    numberOfPendingMessages = messagesPendingOnServer.size ();

    trace (TRACE_LEVEL_INFO, string ("WaveClientTransportObjectManager::replyToRemoteMessagesPendingForServer : Replying to all ") + numberOfPendingMessages + string (" pending messages for Server : ") + serverId);

    for (i = 0; i < numberOfPendingMessages; i++)
    {
        (messagesPendingOnServer[i])->setCompletionStatus (completionStatus);
        replyToBeUsedByReceiverThreads (messagesPendingOnServer[i]);
    }
}

ResourceId WaveClientTransportObjectManager::connectToNewManagementInterfaceServer (const string &waveClientName, const string &waveClientNameWithoutExtension, const string &serverIpAddress, const SI32 &serverPort, UI32 &serverId)
{
    WaveManagementServer *pWaveManagementServer = new WaveManagementServer (waveClientName, waveClientNameWithoutExtension, serverIpAddress, serverPort);
    ResourceId            status                = FRAMEWORK_ERROR;

    // We first add to the cache and then connect so that the returning connectioncan find us in the cache if thew connect call succeeds

    m_serversMap.addServer (pWaveManagementServer);

    addServerId (pWaveManagementServer->getId ());

    status = pWaveManagementServer->connect (420, 420);

    removeServerId (pWaveManagementServer->getId ());

    if (FRAMEWORK_SUCCESS == status)
    {
        serverId = pWaveManagementServer->getId ();
    }
    else
    {
        m_serversMap.removeServer (serverIpAddress, serverPort);
        delete pWaveManagementServer;
    }

    return (status);
}

WaveManagementServer *WaveClientTransportObjectManager::getServer (const string &serverIpAddress, const SI32 &serverPort)
{
    return (m_serversMap.getServer (serverIpAddress, serverPort));
}

UI32 WaveClientTransportObjectManager::getServerId (const string &serverIpAddress, const SI32 &serverPort)
{
    return (m_serversMap.getServerId (serverIpAddress, serverPort));
}

void WaveClientTransportObjectManager::addServiceCache (const UI32 &serverId, const vector<string> &serviceNames, const vector<UI32> &serviceIds)
{
    m_serversMap.addServiceCache (serverId, serviceNames, serviceIds);
}

ResourceId WaveClientTransportObjectManager::postToServer (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    status = m_serversMap.post (pManagementInterfaceMessage);

    if (WAVE_MGMT_CLIENT_INTF_POST_TO_SERVER_SUCCEEDED != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveClientTransportObjectManager::postToServer : Could not post the message to server : Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveClientTransportObjectManager::disconnectFromServer (const UI32 &serverId, const bool removeFromCache)
{
    WaveManagementServer *pWaveManagementServer = m_serversMap.removeServer (serverId);

    if (NULL != pWaveManagementServer)
    {
        // Remove the WaveClientReceiverThread from the cache
        string serverIpAddress = pWaveManagementServer->getIpAddress ();
        SI32   serverPort      = pWaveManagementServer->getPort ();

        if (removeFromCache == true)
        {
            WaveClientReceiverObjectManager::terminateAndRemoveWaveClientReceiverThreadFromCache (serverIpAddress, serverPort);
        }

        // Delete will indicate and close the socket as well.

        if (false == validateIfInTheMiddleOfConnectingToServer (serverId))
        {
            delete pWaveManagementServer;
        }

        replyToRemoteMessagesPendingForServer (serverId, WAVE_MGMT_CLIENT_INTF_SERVER_NOT_AVAILABLE);
    }
    else
    {
        trace (TRACE_LEVEL_WARN, string ("WaveClientTransportObjectManager::disconnectFromClient : Already disconnected from server with id : ") + serverId);
    }
}

void WaveClientTransportObjectManager::disconnectFromServer (const string &serverIpAddress, const SI32 &serverPort)
{
    WaveManagementServer *pWaveManagementServer = m_serversMap.removeServer (serverIpAddress, serverPort);

    if (NULL != pWaveManagementServer)
    {
        // Remove the WaveClientReceiverThread from the cache
        WaveClientReceiverObjectManager::terminateAndRemoveWaveClientReceiverThreadFromCache (serverIpAddress, serverPort);

        UI32 serverId = pWaveManagementServer->getId ();

        // Delete will indicate and close the socket as well.

        delete pWaveManagementServer;

        replyToRemoteMessagesPendingForServer (serverId, WAVE_MGMT_CLIENT_INTF_SERVER_NOT_AVAILABLE);
    }
    else
    {
        trace (TRACE_LEVEL_WARN, string ("WaveClientTransportObjectManager::disconnectFromClient : Already disconnected from server : ") + serverIpAddress + string (":") + serverPort);
    }
}

PrismServiceId WaveClientTransportObjectManager::getPrismServiceIdForServiceName (const UI32 &serverId, const string &serviceName)
{
    return (m_serversMap.getPrismServiceIdForServiceName (serverId, serviceName));
}

string WaveClientTransportObjectManager::getPrismServiceNameForServiceId (const UI32 &serverId, const PrismServiceId &serviceId)
{
    return (m_serversMap.getPrismServiceNameForServiceId (serverId, serviceId));
}

UI32 WaveClientTransportObjectManager::getServerIdIfKnown (const string &serverIpAddress, const SI32 &serverPort)
{
    return (m_serversMap.getServerIdIfKnown (serverIpAddress, serverPort));
}

UI32 WaveClientTransportObjectManager::getServerIdIfKnownForUniqueServerIdentifier (const string &serverIpAddress, const SI32 &serverPort, const UI32 &uniqueServerIdentifierFromServerPerspective)
{
    return (m_serversMap.getServerIdIfKnownForUniqueServerIdentifier (serverIpAddress, serverPort, uniqueServerIdentifierFromServerPerspective));
}

void WaveClientTransportObjectManager::replyToPendingMessagesForServer  (const string &serverIpAddress, const SI32 &serverPort)
{
    UI32 serverId = getServerIdIfKnown (serverIpAddress, serverPort);
    
    replyToRemoteMessagesPendingForServer (serverId, WAVE_MGMT_CLIENT_INTF_SERVER_NOT_AVAILABLE);
}

void WaveClientTransportObjectManager::lockAccessForServerIdConnectionStatus ()
{
    m_accessServerIdConnectionStatus.lock ();
}

void WaveClientTransportObjectManager::unlockAccessForServerIdConnectionStatus ()
{
    m_accessServerIdConnectionStatus.unlock ();
}

void WaveClientTransportObjectManager::addServerId (const UI32 &serverId)
{
    lockAccessForServerIdConnectionStatus ();

    m_serverIdSet.insert (serverId);

    unlockAccessForServerIdConnectionStatus ();
}

bool WaveClientTransportObjectManager::validateIfInTheMiddleOfConnectingToServer (const UI32 &serverId)
{
    lockAccessForServerIdConnectionStatus ();
    
    bool connectingStatus = true;

    set<UI32>::iterator  it;

    it = m_serverIdSet.find (serverId);

    if (m_serverIdSet.end () == it)
    {
        connectingStatus = false;
    }

    unlockAccessForServerIdConnectionStatus ();

    return (connectingStatus);
}

void WaveClientTransportObjectManager::removeServerId (const UI32 &serverId)
{
    lockAccessForServerIdConnectionStatus ();

    m_serverIdSet.erase (serverId);

    unlockAccessForServerIdConnectionStatus ();
}

void WaveClientTransportObjectManager::updateMessageVersionAndSerializationTypeForServer (const string &serverIpAddress, const SI32 &serverPort, const string &version) 
{
    UI32 serverid = getServerIdIfKnown (serverIpAddress, serverPort);

    m_serversMap.updateMessageVersionAndSerializationTypeForServer (serverid, version);
}

}
