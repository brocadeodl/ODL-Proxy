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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/Test/WaveBrokerPublishTest1Message.h"

namespace WaveNs
{

WaveBrokerPublishTest1Message::WaveBrokerPublishTest1Message ()
    : WaveBrokerPublishMessage ("WaveBrokerPublishTest1Message"),
      m_sequenceNumber         (0)
{
}

WaveBrokerPublishTest1Message::~WaveBrokerPublishTest1Message ()
{
}

void WaveBrokerPublishTest1Message::setupAttributesForSerialization ()
{
    WaveBrokerPublishMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_messageToBePublished, "messageToBePublished"));
    addSerializableAttribute (new AttributeUI32   (&m_sequenceNumber,       "sequenceNumber"));
}

WaveBrokerPublishMessage *WaveBrokerPublishTest1Message::createMessageInstance ()
{
    return (new WaveBrokerPublishTest1Message ());
}

string WaveBrokerPublishTest1Message::getMessageToBePublished () const
{
    return (m_messageToBePublished);
}

void WaveBrokerPublishTest1Message::setMessageToBePublished (const string &messageToBePublished)
{
    m_messageToBePublished = messageToBePublished;
}

UI32 WaveBrokerPublishTest1Message::getSequenceNumber () const
{
    return (m_sequenceNumber);
}

void WaveBrokerPublishTest1Message::setSequenceNumber (const UI32 &sequenceNumber)
{
    m_sequenceNumber = sequenceNumber;
}

}
