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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Messaging/MessageTracker/MessageTracker.h"
#include <Framework/MultiThreading/PrismThread.h>
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/ReservedWaveLocalObjectManager.h"

#include <algorithm>
// #include <typeinfo> // To print ClassName of message.

namespace WaveNs
{

map<PrismThreadId, map<const PrismMessage *, const PrismMessage *> > MessageTracker::m_currentlyAllocatedMessagesByThread;
map<const PrismMessage *,PrismThreadId>                              MessageTracker::m_currentlyAllocatedMessages;
bool                                                                 MessageTracker::m_trackingMessagesWithBt = false;
map<const PrismMessage *, string>                                    MessageTracker::m_currentlyAllocatedMessagesWithBt;
PrismMutex                                                           MessageTracker::m_messageTrackerMutex;

bool MessageTracker::isAKnownMessage (const PrismMessage *pPrismMessage)
{
    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    map<const PrismMessage *, PrismThreadId>::iterator element    = m_currentlyAllocatedMessages.find (pPrismMessage);
    map<const PrismMessage *, PrismThreadId>::iterator endElement = m_currentlyAllocatedMessages.end ();
    bool                                               found      = false;

    if (endElement != element)
    {
        found = true;
    }

    return (found);
}

void MessageTracker::addToMessageTracker(const PrismMessage* pPrismMessage)
{
    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    m_messageTrackerMutex.lock ();

    if (true == (isAKnownMessage (pPrismMessage)))
    {
        trace (TRACE_LEVEL_FATAL, "MessageTracker::addToMessageTracker : Duplicate Tracking of a message detected.");
        prismAssert (false, __FILE__, __LINE__);

        m_messageTrackerMutex.unlock ();

        return;
    }

    if (true == MessageTracker::m_trackingMessagesWithBt)
    {
        if (true == (isAKnownMessageWithBt (pPrismMessage)))
        {
            trace (TRACE_LEVEL_FATAL, "MessageTracker::addToMessageTracker : Duplicate Tracking of a Message with Bt detected.");
            prismAssert (false, __FILE__, __LINE__);

            m_messageTrackerMutex.unlock ();

            return;
        }
    }

    PrismThreadId thisThread = PrismThread::getSelf ();

    m_currentlyAllocatedMessagesByThread[thisThread][pPrismMessage] = pPrismMessage;
    m_currentlyAllocatedMessages[pPrismMessage]                     = thisThread;

    if (true == MessageTracker::m_trackingMessagesWithBt)
    {
        string btstring;
        FrameworkToolKit::obtainBtString (2, 10, btstring);
        m_currentlyAllocatedMessagesWithBt[pPrismMessage]           = btstring;
    }

    m_messageTrackerMutex.unlock ();
}

void MessageTracker::deleteFromMessageTracker(const PrismMessage* pPrismMessage)
{
    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    m_messageTrackerMutex.lock ();

    if (false == (isAKnownMessage (pPrismMessage)))
    {
        // When bt tracker is turned off, we clear "Message pointer" --> "bt string" map too.
        trace (TRACE_LEVEL_FATAL, "MessageTracker::deleteFromMessageTracker :Deleting of an unknown message. Possible DOUBLE DELETION / MEMORY CORRUPTION Detected.");
        prismAssert (false, __FILE__, __LINE__);

        m_messageTrackerMutex.unlock ();

        return;
    }

    if (true == MessageTracker::m_trackingMessagesWithBt)
    {
        if (true == (isAKnownMessageWithBt (pPrismMessage)))
        {
            m_currentlyAllocatedMessagesWithBt.erase(pPrismMessage);
        }
    }

    m_currentlyAllocatedMessagesByThread[m_currentlyAllocatedMessages[pPrismMessage]].erase (pPrismMessage);
    m_currentlyAllocatedMessages.erase (pPrismMessage);

    m_messageTrackerMutex.unlock ();
}

// private, no-lock.
void MessageTracker::getMessagesForAThread (const PrismThreadId &prismThreadId, vector<PrismServiceId> &messageServiceIds, vector<UI32> &messageOperationCodes, vector<WaveMessageType> &messageTypes, vector<string> &btStrings)
{
    map<PrismThreadId, map<const PrismMessage *, const PrismMessage *> >::const_iterator itr1 = m_currentlyAllocatedMessagesByThread.find (prismThreadId);
    map<PrismThreadId, map<const PrismMessage *, const PrismMessage *> >::const_iterator end1 = m_currentlyAllocatedMessagesByThread.end ();

    if (end1 == itr1)
    {
        return;
    }

    const map<const PrismMessage *, const PrismMessage *> &messageMap = itr1->second;

    map<const PrismMessage *, const PrismMessage *>::const_iterator itr2 = messageMap.begin ();
    map<const PrismMessage *, const PrismMessage *>::const_iterator end2 = messageMap.end ();

    for (; end2 != itr2; itr2++)
    {
        const PrismMessage *pPrismMessage = itr2->second;
        prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

        messageServiceIds.push_back     (pPrismMessage->getServiceCode ());
        messageOperationCodes.push_back (pPrismMessage->getOperationCode ());
        messageTypes.push_back          (pPrismMessage->getType ());

        string btstring; // = string (typeid (*pPrismMessage).name ()) + "\r\n"; // To print ClassName of message.

        if (true == MessageTracker::m_trackingMessagesWithBt)
        {
            if (isAKnownMessageWithBt (pPrismMessage))
            {
                btstring += m_currentlyAllocatedMessagesWithBt[pPrismMessage];
            }
        }

        btStrings.push_back (btstring);
    }
}

void MessageTracker::getMessages (const PrismServiceId &prismServiceId, vector<PrismServiceId> &messageServiceIds, vector<UI32> &messageOperationCodes, vector<WaveMessageType> &messageTypes, vector<string> &btStrings)
{
    m_messageTrackerMutex.lock ();

    PrismThread *pPrismThread = PrismThread::getPrismThreadForServiceId (prismServiceId);

    if (NULL == pPrismThread)
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, false, "MessageTracker::getMessages : Could not find a Prism Thread that corresponds to Prism Service ID : %u", prismServiceId);

        m_messageTrackerMutex.unlock ();
        return;
    }

    PrismThreadId prismThreadId = pPrismThread->getId ();

    getMessagesForAThread (prismThreadId, messageServiceIds, messageOperationCodes, messageTypes, btStrings);

    PrismServiceId serviceIdForReservedWaveLocalObjectManager = ReservedWaveLocalObjectManager::getPrismServiceId ();

    if (prismServiceId == serviceIdForReservedWaveLocalObjectManager)
    {
        // 1. Get set of all thread ids tracked.

        set<PrismThreadId> setOfAllPrismThreadIds;

        map<PrismThreadId, map<const PrismMessage *, const PrismMessage *> >::const_iterator itr1 = m_currentlyAllocatedMessagesByThread.begin ();
        map<PrismThreadId, map<const PrismMessage *, const PrismMessage *> >::const_iterator end1 = m_currentlyAllocatedMessagesByThread.end ();

        for (; end1 != itr1; itr1++)
        {
            setOfAllPrismThreadIds.insert (itr1->first);
        }


        // 2. Get set of thread ids corresponding to all services.

        set<PrismThreadId> setOfPrismThreadIdsForAllServices;

        vector<PrismServiceId> allPrismServiceIds;

        PrismThread::getListOfServiceIds (allPrismServiceIds);

        vector<PrismServiceId>::const_iterator itr2 = allPrismServiceIds.begin ();
        vector<PrismServiceId>::const_iterator end2 = allPrismServiceIds.end ();

        for (; end2 != itr2; itr2++)
        {
            PrismServiceId prismServiceIdForAService = *itr2;

            PrismThread *pPrismThreadForAService = PrismThread::getPrismThreadForServiceId (prismServiceIdForAService);

            if (NULL != pPrismThreadForAService)
            {
                setOfPrismThreadIdsForAllServices.insert (pPrismThreadForAService->getId ());
            }
        }


        // 3. Prepare a vector of service ids corresponding to thread ids, each of which is 'not related to any service.'
        //    i.e. collect all the thread ids which are in '1.', but not in '2.'

        vector<PrismThreadId> vectorOfNonServicePrismThreads (setOfAllPrismThreadIds.size ());

        vector<PrismThreadId>::iterator endOfDifference = std::set_difference (setOfAllPrismThreadIds.begin (), setOfAllPrismThreadIds.end (), setOfPrismThreadIdsForAllServices.begin (), setOfPrismThreadIdsForAllServices.end (), vectorOfNonServicePrismThreads.begin ());

        vectorOfNonServicePrismThreads.resize (endOfDifference - vectorOfNonServicePrismThreads.begin ());


        // 4. Get Message tracker information for thread ids obtained in '3.' above.

        vector<PrismThreadId>::const_iterator itr3 = vectorOfNonServicePrismThreads.begin ();
        vector<PrismThreadId>::const_iterator end3 = vectorOfNonServicePrismThreads.end ();

        for (; end3 != itr3; itr3++)
        {
            getMessagesForAThread ((*itr3), messageServiceIds, messageOperationCodes, messageTypes, btStrings);
        }
    }

#if 0
    PrismThreadId                                                                  prismThreadId    = (PrismThread::getPrismThreadForServiceId (prismServiceId))->getId ();
    map<PrismThreadId, map<const PrismMessage *, const PrismMessage *> >::iterator threadElement    = m_currentlyAllocatedMessagesByThread.find (prismThreadId);
    map<PrismThreadId, map<const PrismMessage *, const PrismMessage *> >::iterator threadEndElement = m_currentlyAllocatedMessagesByThread.end  ();

    string btstring = "";
    if (threadEndElement != threadElement)
    {
        map<const PrismMessage *, const PrismMessage *>::iterator messageElement    = m_currentlyAllocatedMessagesByThread[prismThreadId].begin ();
        map<const PrismMessage *, const PrismMessage *>::iterator messageEndElement = m_currentlyAllocatedMessagesByThread[prismThreadId].end   ();

        while (messageEndElement != messageElement)
        {
            const PrismMessage *pPrismMessage = messageElement->second;

            prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

            messageServiceIds.push_back (pPrismMessage->getServiceCode ());
            messageOperationCodes.push_back (pPrismMessage->getOperationCode ());
            messageTypes.push_back (pPrismMessage->getType ());

            if (true == MessageTracker::m_trackingMessagesWithBt)
            {
                if (isAKnownMessageWithBt (pPrismMessage))
                {
                    btstring = m_currentlyAllocatedMessagesWithBt[pPrismMessage];
                }
                else
                {
                    btstring = "";
                }
            }
            else
            {
                btstring = "";
            }
            btStrings.push_back (btstring);

            messageElement++;
        }
    }
#endif

    m_messageTrackerMutex.unlock ();
}

bool MessageTracker::isAKnownMessageWithBt (const PrismMessage *pPrismMessage)
{
    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    map<const PrismMessage *, string>::iterator element    = m_currentlyAllocatedMessagesWithBt.find (pPrismMessage);
    map<const PrismMessage *, string>::iterator endElement = m_currentlyAllocatedMessagesWithBt.end ();
    bool                                        found      = false;

    if (endElement != element)
    {
        found = true;
    }

    return (found);
}

void MessageTracker::changeBtTrackerState (const bool &btTrackerState)
{
    m_messageTrackerMutex.lock ();

    if (true == btTrackerState)
    {
        MessageTracker::m_trackingMessagesWithBt = true;
        trace (TRACE_LEVEL_INFO, "MessageTracker::changeBtTrackerState : Tracking with Bt is enabled.");
    }
    else
    {
        MessageTracker::m_trackingMessagesWithBt = false;
        MessageTracker::clearCurrentlyAllocatedMessagesWithBt ();
        trace (TRACE_LEVEL_INFO, "MessageTracker::changeBtTrackerState : Disabled tracking with Bt and cleared history.");
    }

    m_messageTrackerMutex.unlock ();
}

void MessageTracker::clearCurrentlyAllocatedMessagesWithBt ()
{
    m_currentlyAllocatedMessagesWithBt.erase (m_currentlyAllocatedMessagesWithBt.begin (), m_currentlyAllocatedMessagesWithBt.end ());
}

}
