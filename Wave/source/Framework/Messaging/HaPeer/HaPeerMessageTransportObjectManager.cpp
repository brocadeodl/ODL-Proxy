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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                    *
 ***************************************************************************/

#include "Framework/Messaging/HaPeer/HaPeerMessageTransportObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManagerHaSyncWorker.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

namespace WaveNs
{

PrismThread *s_pPrismThreadForHaPeerMessageTransportObjectManager = NULL;
PrismMutex   s_mutexForPrismThreadForHaPeerMessageTransportObjectManager;

HaPeerMessageTransportObjectManager::MessageMap::MessageMap ()
{
}

HaPeerMessageTransportObjectManager::MessageMap::~MessageMap ()
{
}

void HaPeerMessageTransportObjectManager::MessageMap::lockAccess ()
{
    m_accessMutex.lock ();
}

void HaPeerMessageTransportObjectManager::MessageMap::unlockAccess ()
{
    m_accessMutex.unlock ();
}

WaveMessageStatus HaPeerMessageTransportObjectManager::MessageMap::addMessage (PrismMessage *pPrismMessage)
{
    lockAccess ();

    UI32              prismMessageId    = pPrismMessage->getMessageId ();
    PrismMessage       *pTempPrismMessage = findMessage (prismMessageId);
    WaveMessageStatus  status          = WAVE_MESSAGE_SUCCESS;

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

PrismMessage *HaPeerMessageTransportObjectManager::MessageMap::findMessage (UI32 prismMessageId)
{
    map<UI32, PrismMessage *>::iterator  element     = m_messagesMap.find (prismMessageId);
    map<UI32, PrismMessage *>::iterator  end         = m_messagesMap.end ();
    PrismMessage                        *pPrismMessage = NULL;

    if (end != element)
    {
        pPrismMessage = element->second;
    }

    return (pPrismMessage);
}

PrismMessage *HaPeerMessageTransportObjectManager::MessageMap::removeMessage (UI32 prismMessageId)
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

void HaPeerMessageTransportObjectManager::MessageMap::getPendingMessagesForRemoteLocation (LocationId locationId, vector<PrismMessage *> &messagesVector)
{
    lockAccess ();

    map<UI32, PrismMessage *>::iterator  element     = m_messagesMap.begin ();
    map<UI32, PrismMessage *>::iterator  end         = m_messagesMap.end ();
    PrismMessage                        *pPrismMessage = NULL;

    while (element != end)
    {
        pPrismMessage = element->second;

        if ((pPrismMessage->getReceiverLocationId ()) == locationId)
        {
            messagesVector.push_back (pPrismMessage);
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

HaPeerMessageTransportObjectManager::HaPeerMessageTransportObjectManager ()
    : WaveLocalObjectManager ("Ha Peer Message Transport")
{
    s_mutexForPrismThreadForHaPeerMessageTransportObjectManager.lock ();

    s_pPrismThreadForHaPeerMessageTransportObjectManager = PrismThread::getPrismThreadForServiceId (getServiceId ());

    s_mutexForPrismThreadForHaPeerMessageTransportObjectManager.unlock ();

    removeOperationMap (WAVE_OBJECT_MANAGER_LOAD_OPERATIONAL_DATA_FOR_MANAGED_OBJECT);

    addOperationMap (WAVE_OBJECT_MANAGER_ANY_OPCODE, reinterpret_cast<PrismMessageHandler> (&HaPeerMessageTransportObjectManager::haPeerMessageTransportHandler));
}

HaPeerMessageTransportObjectManager *HaPeerMessageTransportObjectManager::getInstance ()
{
    static HaPeerMessageTransportObjectManager *pHaPeerMessageTransportObjectManager = NULL;

    if (NULL == pHaPeerMessageTransportObjectManager)
    {
        pHaPeerMessageTransportObjectManager = new HaPeerMessageTransportObjectManager ();
        WaveNs::prismAssert (NULL != pHaPeerMessageTransportObjectManager, __FILE__, __LINE__);
    }

    return (pHaPeerMessageTransportObjectManager);
}

HaPeerMessageTransportObjectManager::~HaPeerMessageTransportObjectManager ()
{
}

PrismServiceId HaPeerMessageTransportObjectManager::getPrismServiceId ()
{
    PrismServiceId prismServiceId = 0;

    s_mutexForPrismThreadForHaPeerMessageTransportObjectManager.lock ();

    if (NULL != s_pPrismThreadForHaPeerMessageTransportObjectManager)
    {
        prismServiceId = (getInstance ())->getServiceId ();
    }
    else
    {
        prismServiceId = 0;
    }

    s_mutexForPrismThreadForHaPeerMessageTransportObjectManager.unlock ();

    return (prismServiceId);
}

PrismThread *HaPeerMessageTransportObjectManager::getPrismThread ()
{
    PrismThread *pPrismThread = NULL;

    s_mutexForPrismThreadForHaPeerMessageTransportObjectManager.lock ();

    pPrismThread = s_pPrismThreadForHaPeerMessageTransportObjectManager;

    s_mutexForPrismThreadForHaPeerMessageTransportObjectManager.unlock ();

    return (pPrismThread);
}

void HaPeerMessageTransportObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void HaPeerMessageTransportObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (0, WAVE_OBJECT_MANAGER_ANY_EVENT, reinterpret_cast<PrismEventHandler> (&HaPeerMessageTransportObjectManager::haPeerEventTransportHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void HaPeerMessageTransportObjectManager::haPeerMessageTransportHandler (PrismMessage *pPrismMessage)
{
    lockAccess1 ();

    trace (TRACE_LEVEL_DEVEL, "HaPeerMessageTransportObjectManager::haPeerMessageTransportHandler : Starting ...");

    bool               isMessageCached = false;
    WaveMessageStatus status;
    bool               isOneWayMessage = pPrismMessage->getIsOneWayMessage ();

    // Cache the message before posting it to remote location if the message is not one way message.
    // for one way messages we do not cache the location because we do not care about the replies.
    // In this case we must not receive the replies to one way messages.

    if (false == isOneWayMessage)
    {
        isMessageCached = true;
        status          = m_remoteMessagesMap.addMessage (pPrismMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "HaPeerMessageTransportObjectManager::haPeerMessageTransportHandler : Could not cache a message after sending out to a remote location.");
            prismAssert (false, __FILE__, __LINE__);

            // FIXME : sagar : If we are not able to cache it.  What to do???
        }
    }

    status = postToHaPeerLocation (pPrismMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_DEVEL, "HaPeerMessageTransportObjectManager::haPeerMessageTransportHandler : Could not post message to remote location.");

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

void HaPeerMessageTransportObjectManager::lockAccess ()
{
    m_accessMutexForReceiverThreads.lock ();
}

void HaPeerMessageTransportObjectManager::unlockAccess ()
{
    m_accessMutexForReceiverThreads.unlock ();
}

void HaPeerMessageTransportObjectManager::lockAccess1 ()
{
    m_accessMutexForMessagesMap.lock ();
}

void HaPeerMessageTransportObjectManager::unlockAccess1 ()
{
    m_accessMutexForMessagesMap.unlock ();
}

WaveMessageStatus HaPeerMessageTransportObjectManager::sendToBeUsedByReceiverThreads (PrismMessage *pPrismMessage)
{
    // This method must be protected with locking mechanism since it can be executed from mutiple receiver threads.

    lockAccess ();

    //trace (TRACE_LEVEL_DEVEL, "HaPeerMessageTransportObjectManager::sendToBeUsedByReceiverThreads : We are delivering a Remote Message that arrived.");

    WaveMessageStatus status = WAVE_MESSAGE_ERROR;

    // We distinguish between normally sent messages verses one way messages.

    if (false == (pPrismMessage->getIsOneWayMessage ()))
    {
        status = send (pPrismMessage,
                    reinterpret_cast<PrismMessageResponseHandler> (&HaPeerMessageTransportObjectManager::callbackForSendUsedByReceiverThreads),
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

void HaPeerMessageTransportObjectManager::callbackForSendUsedByReceiverThreads (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    static PrismMutex callbackMutex;

    callbackMutex.lock ();

    // This method is executed via normal reply mechanism.  So only one reply at a time will be processed.  So for each of the message
    // responses that are there in the queues it will be executed once and in serial order.  But this method can be executed by one of the
    // receiver threads as well in case of a send failure when sending using sendToBeUsedByReceiverThreads.  So we must protect this method
    // with locking mechanism.

    //trace (TRACE_LEVEL_DEVEL, "HaPeerMessageTransportObjectManager::callbackForSendUsedByReceiverThreads : Received response to a remote message we delivered earlier.");

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
        WaveMessageStatus status = postToHaPeerLocation (pPrismMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEVEL, "HaPeerMessageTransportObjectManager::callbackForSendUsedByReceiverThreads : We could not deliver a response destined for remote location.  In that case currently we simply drop the message.");
        }
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "HaPeerMessageTransportObjectManager::callbackForSendUsedByReceiverThreads : We dropped a reply across location boundary.");
    }

    callbackMutex.unlock ();

    // Destroy the message in any case.  This is an intermediate message that we created on this location upon receving a remote message.
    // We created this message so that we can normal framework mechanisms to deliver the remote message locally to the corresponsing
    // service.  Either we succeeded sending the response to the message originating location or not we must destroy this object.

    delete pPrismMessage;
}

PrismMessage *HaPeerMessageTransportObjectManager::getPendingMessage (const UI32 &messageId)
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

void HaPeerMessageTransportObjectManager::replyToBeUsedByReceiverThreads (PrismMessage *pPrismMessage)
{
    // This method Need not be protected with locking mechanism though it can be executed from mutiple receiver threads.

    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    if (NULL == pPrismMessage)
    {
        trace (TRACE_LEVEL_ERROR, "HaPeerMessageTransportObjectManager::replyToBeUsedByReceiverThreads : Someone is trying to forward a NULL remote response.  We are simply droping the response.");
        return;
    }

    reply (pPrismMessage);

    return;
}

void HaPeerMessageTransportObjectManager::replyToBeUsedByReceiverThreads (UI32 prismMessageId)
{
    // This method must be protected with locking mechanism since it can be executed from mutiple receiver threads.

    PrismMessage *pPrismMessage = m_remoteMessagesMap.removeMessage (prismMessageId);

    if (NULL == pPrismMessage)
    {
        trace (TRACE_LEVEL_ERROR, "HaPeerMessageTransportObjectManager::replyToBeUsedByReceiverThreads : Some one is trying to forward a remote response to a message that does not exist.");
        return;
    }

    reply (pPrismMessage);

    return;
}

void HaPeerMessageTransportObjectManager::getPendingMessagesForRemoteLocation (LocationId locationId, vector<PrismMessage *> &messagesVector)
{
    // This method must be protected with lockAccess1 since this will be executed on the remote message receiver thread.  This member function
    // must not be executed before the corresponding haPeerMessageTransportHandler is completed executing.

    lockAccess1 ();

    m_remoteMessagesMap.getPendingMessagesForRemoteLocation (locationId, messagesVector);

    unlockAccess1 ();
}

void HaPeerMessageTransportObjectManager::replyToRemoteMessagesPendingOnLocation (LocationId locationId, ResourceId completionStatus)
{
    vector<PrismMessage*> messagesPendingOnRemoteLocation;
    UI32                numberOfPendingMessages          = 0;
    UI32                i                                = 0;

    // We lock this so that we find all the pending messages and also reply to those messages in one go.

    getPendingMessagesForRemoteLocation (locationId, messagesPendingOnRemoteLocation);
    numberOfPendingMessages = messagesPendingOnRemoteLocation.size ();

    trace (TRACE_LEVEL_INFO, string ("HaPeerMessageTransportObjectManager::replyToRemoteMessagesPendingOnLocation : Replying to all ") + numberOfPendingMessages + string (" pending messages for remote location : ") + locationId);

    for (i = 0; i < numberOfPendingMessages; i++)
    {
        (messagesPendingOnRemoteLocation[i])->setCompletionStatus (completionStatus);
        replyToBeUsedByReceiverThreads (messagesPendingOnRemoteLocation[i]);
    }
}

void HaPeerMessageTransportObjectManager::haPeerEventTransportHandler (PrismEvent *pPrismEvent)
{
}

}
