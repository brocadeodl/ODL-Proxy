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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessagePayload.h"

namespace WaveNs
{

WaveBrokerBasedMessagePayload::WaveBrokerBasedMessagePayload ()
    : m_messageBodySize            (0),
      m_pMessageBody               (NULL),
      m_numberOfBuffers            (0),
      m_isPreparedForPostingOnWire (false),
      m_pIoVector                  (NULL),
      m_ioVectorCount              (0)
{
}

WaveBrokerBasedMessagePayload::~WaveBrokerBasedMessagePayload ()
{
    UI32 numberOfBuffers = m_buffers.size ();
    UI32 i               = 0;

    if (NULL != m_pMessageBody)
    {
        delete[] m_pMessageBody;
    }

    for (i = 0; i < numberOfBuffers; i++)
    {
        delete[] m_buffers[i];
    }

    if (NULL != m_pIoVector)
    {
        delete[] m_pIoVector;
    }
}

UI32 &WaveBrokerBasedMessagePayload::getMessageBodySize ()
{
    return (m_messageBodySize);
}

char *&WaveBrokerBasedMessagePayload::getMessageBody ()
{
    return (m_pMessageBody);
}

vector<UI32> &WaveBrokerBasedMessagePayload::getBufferTags ()
{
    return (m_bufferTags);
}

vector<UI32> &WaveBrokerBasedMessagePayload::getBufferSizes ()
{
    return (m_bufferSizes);
}

vector<UI8 *> &WaveBrokerBasedMessagePayload::getBuffers ()
{
    return (m_buffers);
}

void WaveBrokerBasedMessagePayload::prepareAndGetIovecToPostOnWire (struct iovec **pIoVector, UI32 &ioVectorCount)
{
    if (NULL == m_pIoVector)
    {
        m_messageBodySize = htonl (m_messageBodySize);
        m_numberOfBuffers = htonl (m_buffers.size ());

        m_ioVectorCount = 3;

        m_pIoVector = new struct iovec[m_ioVectorCount];

        m_pIoVector[0].iov_base = &m_messageBodySize;
        m_pIoVector[0].iov_len  = sizeof (m_messageBodySize);

        m_pIoVector[1].iov_base = (void *) (m_pMessageBody);
        m_pIoVector[1].iov_len  = ntohl (m_messageBodySize);

        m_pIoVector[2].iov_base = &m_numberOfBuffers;
        m_pIoVector[2].iov_len  = sizeof (m_numberOfBuffers);
    }

    *pIoVector     = m_pIoVector;
     ioVectorCount = m_ioVectorCount;

    return;
}

}
