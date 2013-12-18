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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessage.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

WaveBrokerPublishMessage::WaveBrokerPublishMessage ()
    : WaveBrokerBasedMessage (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_PUBLISH),
      m_referenceCount       (0),
      m_pMessageBuffer       (NULL)
{
}

WaveBrokerPublishMessage::WaveBrokerPublishMessage (const string &topicName)
    : WaveBrokerBasedMessage (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_PUBLISH),
      m_topicName            (topicName),
      m_referenceCount       (0),
      m_pMessageBuffer       (NULL)
{
}

WaveBrokerPublishMessage::~WaveBrokerPublishMessage ()
{
    if (NULL != m_pMessageBuffer)
    {
        delete[] (UI8 *) (m_pMessageBuffer);
    }
}

void WaveBrokerPublishMessage::setupAttributesForSerialization ()
{
    WaveBrokerBasedMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_topicName, "topicName"));
}

string WaveBrokerPublishMessage::getTopicName () const
{
    return (m_topicName);
}

WaveBrokerBasedMessage *WaveBrokerPublishMessage::createMessageInstance ()
{
    return (NULL);
}

string WaveBrokerPublishMessage::getBrokerName () const
{
    return (m_brokerName);
}

void WaveBrokerPublishMessage::setBrokerName ( const string &brokerName)
{
    m_brokerName = brokerName;
}

void WaveBrokerPublishMessage::incrementReferenceCount ()
{
    m_mutex.lock ();

    m_referenceCount++;

    m_mutex.unlock ();
}

void WaveBrokerPublishMessage::setReferenceCount (const UI32 &referenceCount)
{
    m_mutex.lock ();

    m_referenceCount = referenceCount;

    m_mutex.unlock ();
}

void WaveBrokerPublishMessage::decrementReferenceCount ()
{
    m_mutex.lock ();

    prismAssert (0 < m_referenceCount, __FILE__, __LINE__);

    m_referenceCount--;

    m_mutex.unlock ();
}

void WaveBrokerPublishMessage::garbageCollect () const
{
    WaveBrokerPublishMessage *pWaveBrokerPublishMessage = const_cast<WaveBrokerPublishMessage *> (this);

    prismAssert (NULL != pWaveBrokerPublishMessage, __FILE__, __LINE__);

    pWaveBrokerPublishMessage->decrementReferenceCount ();

    pWaveBrokerPublishMessage->m_mutex.lock ();

    if (0 == m_referenceCount)
    {
        pWaveBrokerPublishMessage->m_mutex.unlock ();

        delete pWaveBrokerPublishMessage;

        return;
    }

    pWaveBrokerPublishMessage->m_mutex.unlock ();
}

const char *WaveBrokerPublishMessage::getPMessageBuffer ()
{
    return (m_pMessageBuffer);
}

void WaveBrokerPublishMessage::setPMessageBuffer (const char *pMessageBuffer)
{
    m_pMessageBuffer = pMessageBuffer;
}

void WaveBrokerPublishMessage::loadFromSerializedData2 ()
{
    m_mutex.lock ();

    prismAssert (NULL != m_pMessageBuffer, __FILE__, __LINE__);

    SerializableObject::loadFromSerializedData2 (m_pMessageBuffer);

    m_mutex.unlock ();
}

}
