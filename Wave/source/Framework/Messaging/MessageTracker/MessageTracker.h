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

#ifndef MESSAGETRACKER_H
#define MESSAGETRACKER_H

#include "Framework/Types/Types.h"
#include "Framework/MultiThreading/PrismPosixThread.h"
#include "Framework/Utils/PrismMutex.h"

#include <map>
#include <vector>

using namespace std;

namespace WaveNs
{

class PrismMessage;

class MessageTracker
{
    private :
        static bool isAKnownMessage (const PrismMessage *pPrismMessage);
        static bool isAKnownMessageWithBt (const PrismMessage *pPrismMessage);
        static void clearCurrentlyAllocatedMessagesWithBt ();

        static void getMessagesForAThread    (const PrismThreadId &prismThreadId, vector<PrismServiceId> &messageServiceIds, vector<UI32> &messageOperationCodes, vector<WaveMessageType> &messageTypes, vector<string> &btStrings); // no-lock

    protected :
    public :
        static void addToMessageTracker      (const PrismMessage *pPrismMessage);
        static void deleteFromMessageTracker (const PrismMessage *pPrismMessage);
        static void getMessages              (const PrismServiceId &prismServiceId, vector<PrismServiceId> &messageServiceIds, vector<UI32> &messageOperationCodes, vector<WaveMessageType> &messageTypes, vector<string> &btStrings);
        static void changeBtTrackerState     (const bool &btTrackerState);

    // Now the data members

    private :
        static map<PrismThreadId, map<const PrismMessage *, const PrismMessage *> > m_currentlyAllocatedMessagesByThread;
        static map<const PrismMessage *, PrismThreadId>                             m_currentlyAllocatedMessages;

        static bool                                                                 m_trackingMessagesWithBt;
        static map<const PrismMessage *, string>                                    m_currentlyAllocatedMessagesWithBt;

        static PrismMutex                                                           m_messageTrackerMutex;

    protected :
    public :
};

}

#endif // MESSAGETRACKER_H
