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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVEBROKERPUBLISHMESSAGE_H
#define WAVEBROKERPUBLISHMESSAGE_H

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessage.h"
#include "Framework/Utils/PrismMutex.h"

namespace WaveNs
{

class WaveBrokerPublishMessage : public WaveBrokerBasedMessage
{
    private :
    protected :
        virtual void        setupAttributesForSerialization ();

    public :
                                        WaveBrokerPublishMessage  ();
                                        WaveBrokerPublishMessage  (const string &topicName);
        virtual                        ~WaveBrokerPublishMessage  ();

                string                  getTopicName              () const;

        static  WaveBrokerBasedMessage *createMessageInstance     ();

                string                   getBrokerName            () const;
                void                     setBrokerName            (const string &brokerName);

                void                     incrementReferenceCount  ();
                void                     setReferenceCount        (const UI32 &referenceCount);
                void                     decrementReferenceCount  ();
                void                     garbageCollect           () const;

                const char              *getPMessageBuffer        ();
                void                     setPMessageBuffer        (const char *pMessageBuffer);

                void                     loadFromSerializedData2 ();

    // now the data members

    private :
        string m_topicName;

        // Non serialized members

        string     m_brokerName;

        PrismMutex m_mutex;
        UI32       m_referenceCount;

        const char *m_pMessageBuffer;

    protected :
    public :
};

}

#endif // WAVEBROKERPUBLISHMESSAGE_H
