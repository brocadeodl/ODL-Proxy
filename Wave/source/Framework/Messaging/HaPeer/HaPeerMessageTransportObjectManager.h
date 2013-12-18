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
 *   Author : Vipool Prajapati                                          *
 ***************************************************************************/

#ifndef HAPEERMESSAGETRANSPORTOBJECTMANAGER_H
#define HAPEERMESSAGETRANSPORTOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include <vector>

namespace WaveNs
{

class PrismThread;

class HaPeerMessageTransportObjectManager : public WaveLocalObjectManager
{
    private :
        class MessageMap
        {
            private :
                void lockAccess ();
                void unlockAccess ();
            protected :
            public :
                                           MessageMap                          ();
                                          ~MessageMap                          ();
                       WaveMessageStatus   addMessage                          (PrismMessage *pPrismMessage);
                       PrismMessage       *findMessage                         (UI32 prismMessageId);
                       PrismMessage       *removeMessage                       (UI32 prismMessageId);
                       void                getPendingMessagesForRemoteLocation (LocationId locationId, vector<PrismMessage *> &messagesVector);

            // Now the data members

            private :
                map<UI32, PrismMessage *> m_messagesMap;
                PrismMutex                m_accessMutex;

            protected :
            public:
        };

                            HaPeerMessageTransportObjectManager ();

        void                initialize                                 (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                listenForEvents                            (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                haPeerMessageTransportHandler              (PrismMessage *pPrismMessage);
        void                lockAccess                                 ();
        void                unlockAccess                               ();
        void                lockAccess1                                ();
        void                unlockAccess1                              ();
        WaveMessageStatus   sendToBeUsedByReceiverThreads              (PrismMessage *pPrismMessage);
        void                callbackForSendUsedByReceiverThreads       (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);
        PrismMessage       *getPendingMessage                          (const UI32 &messageId);
        void                replyToBeUsedByReceiverThreads             (PrismMessage *pPrismMessage);
        void                replyToBeUsedByReceiverThreads             (UI32 prismMessageId);
        void                getPendingMessagesForRemoteLocation        (LocationId locationId, vector<PrismMessage *> &messagesVector);
        void                replyToRemoteMessagesPendingOnLocation     (LocationId locationId, ResourceId completionStatus);

        void                haPeerEventTransportHandler                (PrismEvent *pPrismEvent);

    protected :
    public :
        virtual                                            ~HaPeerMessageTransportObjectManager ();
        static  HaPeerMessageTransportObjectManager *getInstance                                ();
        static  PrismServiceId                              getPrismServiceId                          ();

        static  PrismThread                                *getPrismThread                             ();

    // Now the data members

    private :
        MessageMap m_remoteMessagesMap;
        PrismMutex m_accessMutexForReceiverThreads;
        PrismMutex m_accessMutexForMessagesMap;

    protected :
    public :
        friend class HaPeerMessageReceiverThread;
        friend class PrismFrameworkObjectManager;
        friend class PrismFrameworkObjectManagerHaSyncWorker;
};

}

#endif // HAPEERMESSAGETRANSPORTOBJECTMANAGER_H
