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

#ifndef WAVEBROKERPUBLISHTEST1MESSAGE_H
#define WAVEBROKERPUBLISHTEST1MESSAGE_H

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessage.h"

namespace WaveNs
{

class WaveBrokerPublishTest1Message : public WaveBrokerPublishMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                                          WaveBrokerPublishTest1Message ();
        virtual                          ~WaveBrokerPublishTest1Message ();

        static  WaveBrokerPublishMessage *createMessageInstance         ();

                string                    getMessageToBePublished       () const;
                void                      setMessageToBePublished       (const string &messagetoBePublished);

                UI32                      getSequenceNumber             () const;
                void                      setSequenceNumber             (const UI32 &sequenceNumber);

    // now the data members

    private :
        string m_messageToBePublished;
        UI32   m_sequenceNumber;

    protected :
    public :
};

}

#endif // WAVEBROKERPUBLISHTEST1MESSAGE_H
