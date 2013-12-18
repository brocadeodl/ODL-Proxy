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

#include "Framework/Messaging/Remote/InterLocationMessageTransportObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Messaging/Remote/InterLocationMulticastMessage.h"
#include "Framework/Messaging/Remote/InterLocationTypes.h"

namespace WaveNs
{

PrismThread *s_pPrismThreadForInterLocationMessageTransportObjectManager = NULL;
PrismMutex   s_mutexForPrismThreadForInterLocationMessageTransportObjectManager;

InterLocationMessageTransportObjectManager::MessageMap::MessageMap ()
{
}

InterLocationMessageTransportObjectManager::MessageMap::~MessageMap ()
{
}

void InterLocationMessageTransportObjectManager::MessageMap::lockAccess ()
{
    m_accessMutex.lock ();
}

void InterLocationMessageTransportObjectManager::MessageMap::unlockAccess ()
{
    m_accessMutex.unlock ();
}

WaveMessageStatus InterLocationMessageTransportObjectManager::MessageMap::addMessage (PrismMessage *pPrismMessage, UI32 originalMessageId)
{
    lockAccess ();

    UI32               prismMessageId = pPrismMessage->getMessageId ();
    PrismMessage      *pTempPrismMessage = findMessage (prismMessageId);
    WaveMessageStatus  status            = WAVE_MESSAGE_SUCCESS;

    if (0 != originalMessageId)
    {
        prismMessageId = originalMessageId;
    }

    if (NULL == pTempPrismMessage)
    {
        m_messagesMap[prismMessageId] = pPrismMessage;
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        status = WAVE_MESSAGE_ERROR_MESSAGE_ALREADY_EXISTS_IN_MAP;
    }

    unlockAccess ();

    return (status);
}

PrismMessage *InterLocationMessageTransportObjectManager::MessageMap::findMessage (UI32 prismMessageId)
{
    map<UI32, PrismMessage *>::iterator  element       = m_messagesMap.find (prismMessageId);
    map<UI32, PrismMessage *>::iterator  end           = m_messagesMap.end ();
    PrismMessage                        *pPrismMessage = NULL;

    if (end != element)
    {
        pPrismMessage = element->second;
    }

    return (pPrismMessage);
}

PrismMessage *InterLocationMessageTransportObjectManager::MessageMap::findMessageWithLock (UI32 prismMessageId)
{
    lockAccess ();

    map<UI32, PrismMessage *>::iterator  element       = m_messagesMap.find (prismMessageId);
    map<UI32, PrismMessage *>::iterator  end           = m_messagesMap.end (); 
    PrismMessage                        *pPrismMessage = NULL;

    if (end != element)
    {   
        pPrismMessage = element->second;
    }   

    unlockAccess ();

    return (pPrismMessage);
}

PrismMessage *InterLocationMessageTransportObjectManager::MessageMap::removeMessage (UI32 prismMessageId)
{
    lockAccess ();

    map<UI32, PrismMessage *>::iterator element      = m_messagesMap.find (prismMessageId);
    map<UI32, PrismMessage *>::iterator end          = m_messagesMap.end ();
    PrismMessage                        *pPrismMessage = NULL;

    if (end != element)
    {
        pPrismMessage = element->second;
        m_messagesMap.erase (element);
    }

    unlockAccess ();

    return (pPrismMessage);
}

void InterLocationMessageTransportObjectManager::MessageMap::getPendingMessagesForRemoteLocationForReplying (LocationId locationId, vector<PrismMessage *> &messagesVector)
{
    lockAccess ();

    map<UI32, PrismMessage *>::iterator  element     = m_messagesMap.begin ();
    map<UI32, PrismMessage *>::iterator  end         = m_messagesMap.end ();
    PrismMessage                        *pPrismMessage = NULL;

    while (element != end)
    {
        pPrismMessage = element->second;

        InterLocationMulticastMessage *pInterLocationMulticastMessage = dynamic_cast<InterLocationMulticastMessage *>(pPrismMessage);

        if (NULL != pInterLocationMulticastMessage)
        {
            pInterLocationMulticastMessage->messageOperationAccess ();

            if (true == pInterLocationMulticastMessage->isMessageSentToThisLocation (locationId))
            {
                if (true == pInterLocationMulticastMessage->isMessageSentToThisLocationNotReplied (locationId))
                {
                    messagesVector.push_back (pPrismMessage);

                    pInterLocationMulticastMessage->setMessageRepliedToThisLocation (locationId);                        
                }
            }

            pInterLocationMulticastMessage->messageOperationReleaseAccess ();
        }
        else
        {
            if ((pPrismMessage->getReceiverLocationId ()) == locationId)
            {
                messagesVector.push_back (pPrismMessage);
            }
        }

        element++;
    }

    UI32 numberOfPendingMessages = messagesVector.size ();
    UI32 i;

    for (i = 0; i < numberOfPendingMessages; i++)
    {
        // Multicast Message are not removed from the map here. Multicast Message are saved in Map with orignial Message Id. These Messages will be removed while replying to pending messages if this is the last reply.
        m_messagesMap.erase ((messagesVector[i])->getMessageId ());
    }

    unlockAccess ();
}

InterLocationMessageTransportObjectManager::InterLocationMessageTransportObjectManager ()
    : WaveLocalObjectManager ("Remote Message Transport")
{
    s_mutexForPrismThreadForInterLocationMessageTransportObjectManager.lock ();

    s_pPrismThreadForInterLocationMessageTransportObjectManager = PrismThread::getPrismThreadForServiceId (getServiceId ());

    s_mutexForPrismThreadForInterLocationMessageTransportObjectManager.unlock ();

    removeOperationMap (WAVE_OBJECT_MANAGER_LOAD_OPERATIONAL_DATA_FOR_MANAGED_OBJECT);

    // These are Framework Message opcodes which can be handled by any OM, so removing these opcodes from operationMap so that Remote service should not handle these message and send these messages to repective location.                                                   
    removeOperationMap (WAVE_OBJECT_MANAGER_CREATE_MANAGED_OBJECT);
    removeOperationMap (WAVE_OBJECT_MANAGER_DELETE_MANAGED_OBJECT);
    removeOperationMap (WAVE_OBJECT_MANAGER_UPDATE_MANAGED_OBJECT);
    removeOperationMap (WAVE_OBJECT_MANAGER_GET_CLIENT_DATA);
    removeOperationMap (WAVE_OBJECT_MANAGER_DELETE_MANAGED_OBJECTS);

    addOperationMap (INTERLOCATION_MESSAGE_FOR_MULTICAST_SEND, reinterpret_cast<PrismMessageHandler> (&InterLocationMessageTransportObjectManager::interLocationMulticastMessageTransportHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_ANY_OPCODE, reinterpret_cast<PrismMessageHandler> (&InterLocationMessageTransportObjectManager::interLocationMessageTransportHandler));

    // restrictMessageHistoryLogging                (bool messageHistoryLogInsideSend, bool messageHistoryLogInsideReply, bool messageHistoryLogInsideHandleMessage);
    restrictMessageHistoryLogging                (false, false, false);
}

InterLocationMessageTransportObjectManager *InterLocationMessageTransportObjectManager::getInstance ()
{
    static InterLocationMessageTransportObjectManager *pInterLocationMessageTransportObjectManager = NULL;

    if (NULL == pInterLocationMessageTransportObjectManager)
    {
        pInterLocationMessageTransportObjectManager = new InterLocationMessageTransportObjectManager ();
        WaveNs::prismAssert (NULL != pInterLocationMessageTransportObjectManager, __FILE__, __LINE__);
    }

    return (pInterLocationMessageTransportObjectManager);
}

InterLocationMessageTransportObjectManager::~InterLocationMessageTransportObjectManager ()
{
}

PrismServiceId InterLocationMessageTransportObjectManager::getPrismServiceId ()
{
    PrismServiceId prismServiceId = 0;

    s_mutexForPrismThreadForInterLocationMessageTransportObjectManager.lock ();

    if (NULL != s_pPrismThreadForInterLocationMessageTransportObjectManager)
    {
        prismServiceId = (getInstance ())->getServiceId ();
    }
    else
    {
        prismServiceId = 0;
    }

    s_mutexForPrismThreadForInterLocationMessageTransportObjectManager.unlock ();

    return (prismServiceId);
}

PrismThread *InterLocationMessageTransportObjectManager::getPrismThread ()
{
    PrismThread *pPrismThread = NULL;

    s_mutexForPrismThreadForInterLocationMessageTransportObjectManager.lock ();

    pPrismThread = s_pPrismThreadForInterLocationMessageTransportObjectManager;

    s_mutexForPrismThreadForInterLocationMessageTransportObjectManager.unlock ();

    return (pPrismThread);
}

void InterLocationMessageTransportObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    (PrismFrameworkObjectManager::getInstance ())->initializeClientCommunications ();

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void InterLocationMessageTransportObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (0, WAVE_OBJECT_MANAGER_ANY_EVENT, reinterpret_cast<PrismEventHandler> (&InterLocationMessageTransportObjectManager::interLocationEventTransportHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void InterLocationMessageTransportObjectManager::interLocationMulticastMessageTransportHandler (InterLocationMulticastMessage *pInterLocationMulticastMessage)
{
    lockAccess1 ();

    trace (TRACE_LEVEL_DEVEL, "InterLocationMessageTransportObjectManager::interLocationMulticastMessageTransportHandler : Starting ...");

    WaveMessageStatus status;

    // Cache the message before posting it to remote location
    // This message is for multicase so add the message based on original message id of multicast message.
   
    UI32            originalMessageId = pInterLocationMulticastMessage->getMessageIdForMessageToMulticast ();
    set<LocationId> remoteLocations;

    pInterLocationMulticastMessage->getLocationsToSend (remoteLocations);

    status = m_remoteMessagesMap.addMessage (pInterLocationMulticastMessage, originalMessageId);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "InterLocationMessageTransportObjectManager::interLocationMulticastMessageTransportHandler : Could not cache a message after sending out to a remote location.");
        prismAssert (false, __FILE__, __LINE__);
    }
    
    status = postToRemoteLocation (pInterLocationMulticastMessage, remoteLocations);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, "InterLocationMessageTransportObjectManager::interLocationMulticastMessageTransportHandler multicast failed for all location");

        // Since we have already cached this message remove it from the remote messages map.

        PrismMessage *pTempPrismMessage = NULL;

        pTempPrismMessage = m_remoteMessagesMap.removeMessage (originalMessageId);

        prismAssert (pTempPrismMessage == pInterLocationMulticastMessage, __FILE__, __LINE__);

        pInterLocationMulticastMessage->setCompletionStatus (status);
        reply (pInterLocationMulticastMessage);
    }

    unlockAccess1 ();

}

void InterLocationMessageTransportObjectManager::interLocationMessageTransportHandler (PrismMessage *pPrismMessage)
{
    lockAccess1 ();

    trace (TRACE_LEVEL_DEVEL, "InterLocationMessageTransportObjectManager::interLocationMessageTransportHandler : Starting ...");

    bool               isMessageCached                  = false;
    WaveMessageStatus  status                           = WAVE_MESSAGE_SUCCESS;
    bool               isOneWayMessage                  = pPrismMessage->getIsOneWayMessage ();
    bool               isSendAllowedForOneWayConnection = pPrismMessage->checkToSendForOneWayCommunication ();
 
    // Cache the message before posting it to remote location if the message is not one way message.
    // for one way messages we do not cache the location because we do not care about the replies.
    // In this case we must not receive the replies to one way messages.

    if (false == isOneWayMessage)
    {
        isMessageCached = true;
        status          = m_remoteMessagesMap.addMessage (pPrismMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "InterLocationMessageTransportObjectManager::interLocationMessageTransportHandler : Could not cache a message after sending out to a remote location.");
            prismAssert (false, __FILE__, __LINE__);

            // FIXME : sagar : If we are not able to cache it.  What to do???
        }
    }

    if (false  == isSendAllowedForOneWayConnection)
    {
        LocationId locationId = pPrismMessage->getReceiverLocationId ();

        if (false == FrameworkToolKit::isAConnectedLocation (locationId))
        {
            status = WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE;
        }
    }

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        status = postToRemoteLocation (pPrismMessage);
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_DEVEL, "InterLocationMessageTransportObjectManager::interLocationMessageTransportHandler : Could not post message to remote location.");

        // Since we have already cached this message remove it from the remote messages map.

        UI32          messageId         = pPrismMessage->getMessageId ();
        PrismMessage *pTempPrismMessage = NULL;

        if (true == isMessageCached)
        {
            pTempPrismMessage = m_remoteMessagesMap.removeMessage (messageId);
            prismAssert (pTempPrismMessage == pPrismMessage, __FILE__, __LINE__);
        }

        pPrismMessage->setCompletionStatus (status);
        reply (pPrismMessage);
    }
    else
    {
        // We are able to successfully send the message across network.

        // Now we must delete if the message that we sent across is a one way message.  For oneway messages, we will never receive replies across boundaries.
        // We did not cache it (in case of oneway messages) so we can simply delete the message.

        // WARNING !!!!!  WARNING !!!!!  WARNING !!!!!

        // We must not access the pPrismMessages in this member function beyond this comment since it can be concurrently accesed on a receiver thread
        // if the reply to the posted message is already arrived.  However there is one exception:  If we are sure of deleting the message due to the
        // message being a one way message it can be safely deleted since there will no replies to the message that will be received on the receiver
        // thread.  Again, however the pPrismMessage must not beaccessed for any other reason.  that is why we cache the value in isOneWayMessage and test
        // it against that variable.

        if (true == isOneWayMessage)
        {
            delete pPrismMessage;
        }
    }

    unlockAccess1 ();
}

void InterLocationMessageTransportObjectManager::lockAccess ()
{
    m_accessMutexForReceiverThreads.lock ();
}

void InterLocationMessageTransportObjectManager::unlockAccess ()
{
    m_accessMutexForReceiverThreads.unlock ();
}

void InterLocationMessageTransportObjectManager::lockAccess1 ()
{
    m_accessMutexForMessagesMap.lock ();
}

void InterLocationMessageTransportObjectManager::unlockAccess1 ()
{
    m_accessMutexForMessagesMap.unlock ();
}

WaveMessageStatus InterLocationMessageTransportObjectManager::sendToBeUsedByReceiverThreads (PrismMessage *pPrismMessage)
{
    // This method must be protected with locking mechanism since it can be executed from mutiple receiver threads.

    lockAccess ();

    //trace (TRACE_LEVEL_DEVEL, "InterLocationMessageTransportObjectManager::sendToBeUsedByReceiverThreads : We are delivering a Remote Message that arrived.");

    WaveMessageStatus status = WAVE_MESSAGE_ERROR;

    // We distinguish between normally sent messages verses one way messages.

    if (false == (pPrismMessage->getIsOneWayMessage ()))
    {
        status = send (pPrismMessage,
                    reinterpret_cast<PrismMessageResponseHandler> (&InterLocationMessageTransportObjectManager::callbackForSendUsedByReceiverThreads),
                    NULL);
    }
    else
    {
        // If the original message was sent as a one way message send it as one way.  We will never receive a response for this message.

        status = sendOneWay (pPrismMessage);
    }

    unlockAccess ();


    if (WAVE_MESSAGE_SUCCESS != status)
    {
        if (false == (pPrismMessage->getIsOneWayMessage ()))
        {
            pPrismMessage->setCompletionStatus (status);

            // We indicate that the FrameworkStatus is success and the actual failure is stored in the completion status of the message.

            callbackForSendUsedByReceiverThreads (FRAMEWORK_SUCCESS, pPrismMessage, NULL);
        }
        else
        {
            // Delete the one way message

            delete pPrismMessage;
        }
    }

    return (status);
}

void InterLocationMessageTransportObjectManager::callbackForSendUsedByReceiverThreads (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    static PrismMutex callbackMutex;

    callbackMutex.lock ();

    // This method is executed via normal reply mechanism.  So only one reply at a time will be processed.  So for each of the message
    // responses that are there in the queues it will be executed once and in serial order.  But this method can be executed by one of the
    // receiver threads as well in case of a send failure when sending using sendToBeUsedByReceiverThreads.  So we must protect this method
    // with locking mechanism.

    //trace (TRACE_LEVEL_DEVEL, "InterLocationMessageTransportObjectManager::callbackForSendUsedByReceiverThreads : Received response to a remote message we delivered earlier.");

    // Ensure that we do have a valid message.  In some cases framework can callback with a NULL message pointer (in case of a message timeout).
    // but we know that we never requested a timeout period for this message while sending this message using send.  So make sure that the message
    // that we got back is a non NULL message.

    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    pPrismMessage->setType (WAVE_MESSAGE_TYPE_RESPONSE);

    // Ensure that there is no context associated with this reposnse.

    prismAssert (NULL == pContext, __FILE__, __LINE__);

    // The response we recieved is destined for a remote location.  So deliver it to that remote location.
    // We must not cache this since it is a response.  We assume that we do not have to track this any more.  We are simply
    // acting as a remote transport agent.

    if (false == (pPrismMessage->getDropReplyAcrossLocations ()))
    {
        WaveMessageStatus status = postToRemoteLocation (pPrismMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEVEL, "InterLocationMessageTransportObjectManager::callbackForSendUsedByReceiverThreads : We could not deliver a response destined for remote location.  In that case currently we simply drop the message.");
        }
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "InterLocationMessageTransportObjectManager::callbackForSendUsedByReceiverThreads : We dropped a reply across location boundary.");
    }

    //Check if we need to disconnect and remove location after posting the message.

    bool        disconnectFromNodeAfterReply  = pPrismMessage->checkToDisconnectNodeFromLocationAfterReply ();
    bool        removeKnownLocationAfterReply = pPrismMessage->checkToRemoveNodeFromKnownLocationAfterReply ();
    LocationId  locationId                    = pPrismMessage->getSenderLocationId ();

    if (true == disconnectFromNodeAfterReply)
    {
        (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId, true);
    }

    if (true == removeKnownLocationAfterReply)
    {
        (PrismFrameworkObjectManager::getInstance ())->removeKnownLocation  (locationId);
    }

    callbackMutex.unlock ();

    // Destroy the message in any case.  This is an intermediate message that we created on this location upon receving a remote message.
    // We created this message so that we can normal framework mechanisms to deliver the remote message locally to the corresponsing
    // service.  Either we succeeded sending the response to the message originating location or not we must destroy this object.

    delete pPrismMessage;
}

PrismMessage *InterLocationMessageTransportObjectManager::getPendingMessage (const UI32 &messageId)
{
    // WARNING !!!!!  WARNING !!!!!  WARNING !!!!!

    // Care must be taken in this member function that this function should not interfere with the posting of messages to remote locations.
    // Otherwise dead locks can occur while the sendig thread is waiting to post messages after it acquires lock and the received thread waits to
    // acquire the same lock so that it can receive data.  In extreme cases where the kernel buffers for the underlying sockets are full it can
    // cause deadlock conditions.

    PrismMessage *pPrismMessage = NULL;

    pPrismMessage = m_remoteMessagesMap.removeMessage (messageId);

    return (pPrismMessage);
}

PrismMessage *InterLocationMessageTransportObjectManager::getPendingMessageWithoutRemove (const UI32 &messageId)
{
    // WARNING !!!!!  WARNING !!!!!  WARNING !!!!!

    // Care must be taken in this member function that this function should not interfere with the posting of messages to remote locations.
    // Otherwise dead locks can occur while the sendig thread is waiting to post messages after it acquires lock and the received thread waits to
    // acquire the same lock so that it can receive data.  In extreme cases where the kernel buffers for the underlying sockets are full it can
    // cause deadlock conditions.

    PrismMessage *pPrismMessage = NULL;

    pPrismMessage = m_remoteMessagesMap.findMessageWithLock (messageId);

    return (pPrismMessage);
}

void InterLocationMessageTransportObjectManager::replyToBeUsedByReceiverThreads (PrismMessage *pPrismMessage)
{
    // This method Need not be protected with locking mechanism though it can be executed from mutiple receiver threads.

    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    if (NULL == pPrismMessage)
    {
        trace (TRACE_LEVEL_ERROR, "InterLocationMessageTransportObjectManager::replyToBeUsedByReceiverThreads : Someone is trying to forward a NULL remote response.  We are simply droping the response.");
        return;
    }

    reply (pPrismMessage);

    return;
}

void InterLocationMessageTransportObjectManager::replyToBeUsedByReceiverThreads (UI32 prismMessageId)
{
    // This method must be protected with locking mechanism since it can be executed from mutiple receiver threads.

    PrismMessage *pPrismMessage = m_remoteMessagesMap.removeMessage (prismMessageId);

    if (NULL == pPrismMessage)
    {
        trace (TRACE_LEVEL_ERROR, "InterLocationMessageTransportObjectManager::replyToBeUsedByReceiverThreads : Some one is trying to forward a remote response to a message that does not exist.");
        return;
    }

    reply (pPrismMessage);

    return;
}

void InterLocationMessageTransportObjectManager::getPendingMessagesForRemoteLocationForReplying (LocationId locationId, vector<PrismMessage *> &messagesVector)
{
    // This method must be protected with lockAccess1 since this will be executed on the remote message receiver thread.  This member function
    // must not be executed before the corresponding interLocationMessageTransportHandler is completed executing.

    lockAccess1 ();

    m_remoteMessagesMap.getPendingMessagesForRemoteLocationForReplying (locationId, messagesVector);

    unlockAccess1 ();
}

void InterLocationMessageTransportObjectManager::replyToRemoteMessagesPendingOnLocation (LocationId locationId, ResourceId completionStatus)
{
    vector<PrismMessage*> messagesPendingOnRemoteLocation;
    UI32                numberOfPendingMessages          = 0;
    UI32                i                                = 0;

    // this lock is applied for Multicast message being accessed from multiple receiver threads and this thread at same time,
    lockGlobalAccessMutexForMulticastMessaging ();

    // We lock this so that we find all the pending messages and also reply to those messages in one go.

    getPendingMessagesForRemoteLocationForReplying (locationId, messagesPendingOnRemoteLocation);
    numberOfPendingMessages = messagesPendingOnRemoteLocation.size ();

    trace (TRACE_LEVEL_INFO, string ("InterLocationMessageTransportObjectManager::replyToRemoteMessagesPendingOnLocation : Replying to all ") + numberOfPendingMessages + string (" pending messages for remote location : ") + locationId);

    for (i = 0; i < numberOfPendingMessages; i++)
    {
        InterLocationMulticastMessage *pInterLocationMulticastMessage = dynamic_cast<InterLocationMulticastMessage *>(messagesPendingOnRemoteLocation[i]);

        if (NULL != pInterLocationMulticastMessage)
        {
            pInterLocationMulticastMessage->messageOperationAccess ();

            pInterLocationMulticastMessage->setStatusForALocation (locationId, completionStatus);

            --(*pInterLocationMulticastMessage);

            if (true == pInterLocationMulticastMessage->areAllMessageReplyReceived ())
            {
                PrismMessage *pTempPrismMessage = getPendingMessage (pInterLocationMulticastMessage->getMessageIdForMessageToMulticast ());

                prismAssert (pTempPrismMessage == pInterLocationMulticastMessage, __FILE__, __LINE__);

                pInterLocationMulticastMessage->messageOperationReleaseAccess ();

                replyToBeUsedByReceiverThreads (pInterLocationMulticastMessage);                
            }
            else
            {
                pInterLocationMulticastMessage->messageOperationReleaseAccess ();
            }
        }
        else
        {
            (messagesPendingOnRemoteLocation[i])->setCompletionStatus (completionStatus);
            replyToBeUsedByReceiverThreads (messagesPendingOnRemoteLocation[i]);
        }
    }

    unlockGlobalAccessMutexForMulticastMessaging ();
}

void InterLocationMessageTransportObjectManager::interLocationEventTransportHandler (const PrismEvent *&pPrismEvent)
{
    reply (reinterpret_cast<const PrismEvent *&> (pPrismEvent));
}

void InterLocationMessageTransportObjectManager::lockGlobalAccessMutexForMulticastMessaging ()
{
    m_globalAccessMutexForMulticastMessaging.lock ();
}

void InterLocationMessageTransportObjectManager::unlockGlobalAccessMutexForMulticastMessaging ()
{
    m_globalAccessMutexForMulticastMessaging.unlock ();
}

}
