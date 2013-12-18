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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMTHREAD_H
#define PRISMTHREAD_H

#include "Framework/MultiThreading/PrismPosixThread.h"
#include "Framework/MultiThreading/PrismMessageQueue.cpp"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Utils/PrismCondition.h"
#include "Framework/Core/PrismServiceMap.h"

namespace WaveNs
{

class WaveObjectManager;
class TimerWorker;

typedef enum
{
    WAVE_THREAD_MESSAGE_QUEUE_SUCCESS = 0,
    WAVE_THREAD_MESSAGE_QUEUE_NO_MESSAGES_TO_PROCESS
} PrismThreadMessageQueueStatus;

class PrismThread : public PrismPosixThread
{
    private :
        PrismMessage       *getNextMessageToProcess                                ();
        WaveMessageStatus   submitMessage                                          (PrismMessage *pPrismMessage);
        WaveMessageStatus   submitMessageAtFront                                   (PrismMessage *pPrismMessage);
        WaveMessageStatus   submitMessageAtBack                                    (PrismMessage *pPrismMessage);
        WaveMessageStatus   recallMessage                                          (PrismMessage *pPrismMessage);
        UI32                recallTimerExpirationMessagesForTimer                  (const TimerHandle &timerHandle);
        WaveMessageStatus   submitReplyMessage                                     (PrismMessage *pPrismMessage);
        WaveMessageStatus   submitEvent                                            (PrismEvent *pPrismEvent);
        WaveObjectManager *getWaveObjectManagerForOperationCode                  (UI32 operationCode);
        WaveObjectManager *getWaveObjectManagerForEventOperationCode             (UI32 eventOperationCode);
        WaveObjectManager *getWaveObjectManagerForEventOperationCodeForListening (const LocationId &eventSourceLocationId, const PrismServiceId &eventSourceServiceId, const UI32 &eventOperationCode);
        WaveObjectManager *getWaveObjectManagerForPrismMessageId                 (UI32 prismMessageId);
        WaveObjectManager *getWaveObjectManagerForManagedClass                   (const string &managedClass);
        PrismServiceId      getPrismServiceId                                      () const;
        bool                hasWaveObjectManagers                                 ();
        void                initializeHoldCounts                                   ();
        void                holdMessages                                           ();
        void                holdHighPriorityMessages                               ();
        void                holdEvents                                             ();
        void                holdFrameworkMessages                                  ();
        void                holdAll                                                ();
        void                unholdMessages                                         ();
        void                unholdHighPriorityMessages                             ();
        void                unholdEvents                                           ();
        void                unholdFrameworkMessages                                ();
        void                unholdAll                                              ();
        UI32                getNumberOfPendingTimerExpirationMessages              ();
        UI32                getNumberOfPendingNormalMessages                       ();
        UI32                getNumberOfPendingHighPriorityMessages                 ();

        void                emptyIncomingMessageQueuesForDisable                   (vector<PrismMessage *> &incomingMessages);

        void                setCpuAffinity                                         (const vector<UI32> &cpuAffinityVector);

        void                requestForThreadTermination                            ();

    protected :
                                  PrismThread            (PrismServiceId id, const string &serviceName, const UI32 &stackSize = 0, const vector<UI32> *pCpuAffinityVector = NULL);
                                  PrismThread            (PrismServiceId id, WaveObjectManager *pWaveObjectManager, const string &serviceName, const UI32 &stackSize = 0, const vector<UI32> *pCpuAffinityVector = NULL);

        virtual                   ~PrismThread           ();
        virtual WaveThreadStatus  start                  ();
        virtual WaveThreadStatus  consumePendingMessages ();
                void              addWaveObjectManager   (WaveObjectManager *pWaveObjectManager);

    public :

        static void               getListOfServiceIds                     (vector<PrismServiceId> &serviceIds);
        static PrismThread       *getPrismThreadForServiceId              (PrismServiceId id);
        static vector<UI32>       getCpuAffinityVectorForServiceId        (PrismServiceId id);
        static PrismThreadId      getSelf                                 ();
        static string             getPrismServiceNameForServiceId         (const PrismServiceId &id);
        static PrismServiceId     getPrismServiceIdForServiceName         (const string &prismServiceName);
        static PrismThread       *getPrismThreadForMessageRemoteTransport ();
        static PrismThread       *getPrismThreadForMessageHaPeerTransport ();
        static WaveThreadStatus   joinAllThreads                          ();

               vector<UI32>       getCpuAffinityVector                    () const;

        static WaveObjectManager *getWaveObjectManagerForCurrentThread    ();
        static PrismServiceId     getPrismServiceIdForCurrentThread       ();


    // Now the member variables

    private :
               PrismServiceId                          m_prismServiceId;

               PrismMessageQueue<PrismMessage>         m_messages;
               PrismMessageQueue<PrismMessage>         m_messageResponses;
               PrismMessageQueue<PrismMessage>         m_highPriorityMessages;
               PrismMessageQueue<PrismMessage>         m_highPriorityMessageResponses;
               PrismMessageQueue<PrismMessage>         m_events;
               PrismMessageQueue<PrismMessage>         m_timerExpirations;
               PrismMessageQueue<PrismMessage>         m_timerExpirationResponses;
               PrismMessageQueue<PrismMessage>         m_frameworkMessages;
               PrismMessageQueue<PrismMessage>         m_frameworkMessageResponses;
               PrismMessageQueue<PrismMessage>         m_frameworkResumeMessages;

               PrismMutex                              m_gateKeeper;
               PrismMutex                              m_wakeupCaller;
               PrismCondition                          m_wakeupCondition;

               PrismMutex                              m_messagesMutex;
               UI32                                    m_messagesHoldCount;
               PrismMutex                              m_highPriorityMessagesMutex;
               UI32                                    m_highPriorityMessagesHoldCount;
               PrismMutex                              m_eventsMutex;
               UI32                                    m_eventsHoldCount;
               PrismMutex                              m_frameworkMessagesMutex;
               UI32                                    m_frameworkMessagesHoldCount;
               PrismMutex                              m_frameworkResumeMessagesMutex;
               UI32                                    m_frameworkResumeMessagesHoldCount;

               vector<WaveObjectManager *>             m_pWaveObjectManagers;

               vector<UI32>                            m_cpuAffinityVector;

        static map<PrismThreadId, WaveObjectManager *> m_prismThreadIdToWaveObjectManagerMap;
        static PrismMutex                              m_prismThreadIdToWaveObjectManagerMapMutex;

               bool                                    m_terminateThread;

    protected :
    public :

        friend class WaveObjectManager;
        friend class PrismFrameworkObjectManager;
        friend class TimerWorker;
        friend class PrismMessageFactory;
        friend class WaveManagedObjectFactory;
};

}

#endif //PRISMTHREAD_H
