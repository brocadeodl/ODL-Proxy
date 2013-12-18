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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Utils/ClientStreamingSocket.h"
#include "Framework/ObjectModel/SerializableObject.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessagePayload.h"

#include <errno.h>

namespace WaveNs
{

const   UI32    ClientStreamingSocket::m_customDefaultKeepAliveIdleTimeoutInMilliSeconds    = 30000;
const   UI32    ClientStreamingSocket::m_customDefaultKeepAliveProbeIntervalInMilliSeconds  = 20000;
const   UI32    ClientStreamingSocket::m_customDefaultKeepAliveProbeCount                   = 3;


ClientStreamingSocket::ClientStreamingSocket (const string &host, const SI32 port, const SI32 maximumNumberOfConnections)
    : StreamingSocket (maximumNumberOfConnections),
      m_host (host),
      m_port (port)
{
    if (true != (connect (host, port)))
    {
        //cout << "*** errno = " << errno << " ***\n";
        m_isConnected = false;
    }
    else
    {
        m_isConnected = true;
    }
}

ClientStreamingSocket::ClientStreamingSocket ()
    : StreamingSocket (),
      m_port   (0),
      m_isConnected (false)
{
}

ClientStreamingSocket::~ClientStreamingSocket ()
{
}

bool ClientStreamingSocket::setTcpNoDelay ()
{
    return (StreamingSocket::setTcpNoDelay());
}

bool ClientStreamingSocket::clearTcpNoDelay ()
{
    return (StreamingSocket::clearTcpNoDelay ());
}

bool ClientStreamingSocket::setTcpCork ()
{
    return (StreamingSocket::setTcpCork ());
}

bool ClientStreamingSocket::clearTcpCork ()
{
    return (StreamingSocket::clearTcpCork ());
}

bool ClientStreamingSocket::send(UI8 *pBuffer, const UI32 maximumBufferLength)
{
    SI32 status = StreamingSocketBase::send (pBuffer, maximumBufferLength);

    if (-1 == status)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool ClientStreamingSocket::operator << (const UI32 &value)
{
    if (-1 == (StreamingSocketBase::send (value)))
    {
        //cout << "*** errno = " << errno << " ***\n";
        return (false);
    }
    else
    {
        return (true);
    }
}

bool ClientStreamingSocket::operator << (const FixedSizeBuffer &fixedSizeBuffer)
{
    if (-1 == (StreamingSocketBase::send (fixedSizeBuffer)))
    {
        //cout << "*** errno = " << errno << " ***\n";
        return (false);
    }
    else
    {
        return (true);
    }
}

bool ClientStreamingSocket::operator << (SerializableObject *pSerializableObject)
{
    if (-1 == (StreamingSocketBase::send (pSerializableObject)))
    {
        //cout << "*** errno = " << errno << " ***\n";
        return (false);
    }
    else
    {
        return (true);
    }
}

bool ClientStreamingSocket::operator << (PrismMessage *pPrismMessage)
{
    string bufferToPost;

    // Make sure that the message has been prepared for serialization.

    //pPrismMessage->serialize2 (bufferToPost, getToMessageVersion ());
    pPrismMessage->serialize2 (bufferToPost, getToSerializationType ());

    UI32   bufferSize      = bufferToPost.size ();
    UI32   numberOfBuffers = pPrismMessage->getNumberOfBuffers ();
    bool   isSuccessful    = false;

    UI32  bufferSizeToBeSentOverNetwork = htonl (bufferSize);
    UI8  *pNetworkBuffer                = (UI8 *) (&bufferSizeToBeSentOverNetwork);

    bufferToPost.insert (bufferToPost.begin (), 1, (char) (pNetworkBuffer[3]));
    bufferToPost.insert (bufferToPost.begin (), 1, (char) (pNetworkBuffer[2]));
    bufferToPost.insert (bufferToPost.begin (), 1, (char) (pNetworkBuffer[1]));
    bufferToPost.insert (bufferToPost.begin (), 1, (char) (pNetworkBuffer[0]));

    UI32 numberOfBuffersToBeSentOverNetwork = htonl (numberOfBuffers);

    pNetworkBuffer = (UI8 *) (&numberOfBuffersToBeSentOverNetwork);

    bufferToPost.insert (bufferToPost.end (), 1, (char) (pNetworkBuffer[0]));
    bufferToPost.insert (bufferToPost.end (), 1, (char) (pNetworkBuffer[1]));
    bufferToPost.insert (bufferToPost.end (), 1, (char) (pNetworkBuffer[2]));
    bufferToPost.insert (bufferToPost.end (), 1, (char) (pNetworkBuffer[3]));

#if 0
    isSuccessful = (*this) << bufferSize;

    if (false == isSuccessful)
    {
        return (false);
    }
#endif

// Following instrumentation is for figuring out how the thernet driver is performing.
// Disable the instrumentation.  Enable ony for ethernet performance testing purposes.

#if 0
    //    pClientStreamingSocket->clearTcpNoDelay ();
    //    pClientStreamingSocket->setTcpCork ();

                        struct timespec timespec1;
                        struct timespec timespec2;
    static unsigned long int             timeSpentInPosting            = 0;
    static unsigned long int             timeSpentInPostingThisMessage = 0;
    static unsigned long int             timeSpentInPostingMin         = 0;
    static unsigned long int             timeSpentInPostingMax         = 0;
    static unsigned      int             numberofMessages              = 0;
                        int             clockReturnValue              = 0;

    numberofMessages++;

    clockReturnValue = clock_gettime (CLOCK_MONOTONIC, &timespec1);

    if (0 != clockReturnValue)
    {
        prismAssert (false, __FILE__, __LINE__);
    }
#endif

    isSuccessful = (*this) << bufferToPost;

// Following instrumentation is for figuring out how the thernet driver is performing.
// Disable the instrumentation.  Enable ony for ethernet performance testing purposes.

#if 0
    clockReturnValue = clock_gettime (CLOCK_MONOTONIC, &timespec2);

    if (0 != clockReturnValue)
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    //printf ("sec = %20ld, nsec = %20ld\n", timespec1.tv_sec, timespec1.tv_nsec);
    //printf ("sec = %20ld, nsec = %20ld\n", timespec2.tv_sec, timespec2.tv_nsec);
    timeSpentInPostingThisMessage = (timespec2.tv_sec * 1000000000 + timespec2.tv_nsec) - (timespec1.tv_sec * 1000000000 + timespec1.tv_nsec);
    timeSpentInPosting += timeSpentInPostingThisMessage;

    if (0 == timeSpentInPostingMin)
    {
        timeSpentInPostingMin = timeSpentInPostingThisMessage;
    }

    if (timeSpentInPostingMin > timeSpentInPostingThisMessage)
    {
        timeSpentInPostingMin = timeSpentInPostingThisMessage;
    }

    if (0 == timeSpentInPostingMax)
    {
        timeSpentInPostingMax = timeSpentInPostingThisMessage;
    }

    if (timeSpentInPostingMax < timeSpentInPostingThisMessage)
    {
        timeSpentInPostingMax = timeSpentInPostingThisMessage;
    }

    if (0 == (numberofMessages % 10000))
    {
        timeSpentInPosting /= 10000;
        printf ("Time spent in posting per message Average = %lu\n", timeSpentInPosting);
        printf ("Time spent in posting per message Minimum = %lu\n", timeSpentInPostingMin);
        printf ("Time spent in posting per message Maximum = %lu\n", timeSpentInPostingMax);

        timeSpentInPosting    = 0;
        numberofMessages      = 0;
        timeSpentInPostingMin = 0;
        timeSpentInPostingMax = 0;
    }
#endif

    // In the following code clearing TcpCork and setting TcpNoDelay are deliberately commented out.

    if (false == isSuccessful)
    {
//        pClientStreamingSocket->clearTcpCork ();
//        pClientStreamingSocket->setTcpNoDelay ();

        return (false);
    }

    UI32         i                 = 0;
    vector<UI32> bufferTagsVector;

    if (numberOfBuffers > 0)
    {
        pPrismMessage->getBufferTags (bufferTagsVector);
    }

#if 0
    isSuccessful = (*this) << numberOfBuffers;

    if (false == isSuccessful)
    {
//        pClientStreamingSocket->clearTcpCork ();
//        pClientStreamingSocket->setTcpNoDelay ();

        return (false);
    }
#endif

    for (i = 0; i < numberOfBuffers; i++)
    {
        UI32  bufferTag  = bufferTagsVector[i];
        void *pBuffer    = NULL;

        pBuffer = pPrismMessage->findBuffer(bufferTag, bufferSize);

        FixedSizeBuffer tempBuffer (bufferSize, pBuffer);

        prismAssert (0 != bufferSize, __FILE__, __LINE__);

        isSuccessful = (*this) << bufferTag;

        if (false == isSuccessful)
        {
//            pClientStreamingSocket->clearTcpCork ();
//            pClientStreamingSocket->setTcpNoDelay ();

            return (false);
        }

        isSuccessful = (*this) << tempBuffer;

        if (false == isSuccessful)
        {
//            pClientStreamingSocket->clearTcpCork ();
//            pClientStreamingSocket->setTcpNoDelay ();

            return (false);
        }
    }

//    pClientStreamingSocket->clearTcpCork ();
//    pClientStreamingSocket->setTcpNoDelay ();

    return (true);
}

bool ClientStreamingSocket::operator << (WaveBrokerBasedMessage *pWaveBrokerBasedMessage)
{
    string bufferToPost;

    // Make sure that the message has been prepared for serialization.

    pWaveBrokerBasedMessage->serialize2 (bufferToPost, getToSerializationType ());

    UI32   bufferSize      = bufferToPost.size ();
    UI32   numberOfBuffers = pWaveBrokerBasedMessage->getNumberOfBuffers ();
    bool   isSuccessful    = false;

    UI32  bufferSizeToBeSentOverNetwork = htonl (bufferSize);
    UI8  *pNetworkBuffer                = (UI8 *) (&bufferSizeToBeSentOverNetwork);

    bufferToPost.insert (bufferToPost.begin (), 1, (char) (pNetworkBuffer[3]));
    bufferToPost.insert (bufferToPost.begin (), 1, (char) (pNetworkBuffer[2]));
    bufferToPost.insert (bufferToPost.begin (), 1, (char) (pNetworkBuffer[1]));
    bufferToPost.insert (bufferToPost.begin (), 1, (char) (pNetworkBuffer[0]));

    UI32 numberOfBuffersToBeSentOverNetwork = htonl (numberOfBuffers);

    pNetworkBuffer = (UI8 *) (&numberOfBuffersToBeSentOverNetwork);

    bufferToPost.insert (bufferToPost.end (), 1, (char) (pNetworkBuffer[0]));
    bufferToPost.insert (bufferToPost.end (), 1, (char) (pNetworkBuffer[1]));
    bufferToPost.insert (bufferToPost.end (), 1, (char) (pNetworkBuffer[2]));
    bufferToPost.insert (bufferToPost.end (), 1, (char) (pNetworkBuffer[3]));

#if 0
    isSuccessful = (*this) << bufferSize;

    if (false == isSuccessful)
    {
        return (false);
    }
#endif

// Following instrumentation is for figuring out how the ethernet driver is performing.
// Disable the instrumentation.  Enable ony for ethernet performance testing purposes.

#if 0
    //    pClientStreamingSocket->clearTcpNoDelay ();
    //    pClientStreamingSocket->setTcpCork ();

                        struct timespec timespec1;
                        struct timespec timespec2;
    static unsigned long int             timeSpentInPosting            = 0;
    static unsigned long int             timeSpentInPostingThisMessage = 0;
    static unsigned long int             timeSpentInPostingMin         = 0;
    static unsigned long int             timeSpentInPostingMax         = 0;
    static unsigned      int             numberofMessages              = 0;
                        int             clockReturnValue              = 0;

    numberofMessages++;

    clockReturnValue = clock_gettime (CLOCK_MONOTONIC, &timespec1);

    if (0 != clockReturnValue)
    {
        prismAssert (false, __FILE__, __LINE__);
    }
#endif

    isSuccessful = (*this) << bufferToPost;

// Following instrumentation is for figuring out how the thernet driver is performing.
// Disable the instrumentation.  Enable ony for ethernet performance testing purposes.

#if 0
    clockReturnValue = clock_gettime (CLOCK_MONOTONIC, &timespec2);

    if (0 != clockReturnValue)
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    //printf ("sec = %20ld, nsec = %20ld\n", timespec1.tv_sec, timespec1.tv_nsec);
    //printf ("sec = %20ld, nsec = %20ld\n", timespec2.tv_sec, timespec2.tv_nsec);
    timeSpentInPostingThisMessage = (timespec2.tv_sec * 1000000000 + timespec2.tv_nsec) - (timespec1.tv_sec * 1000000000 + timespec1.tv_nsec);
    timeSpentInPosting += timeSpentInPostingThisMessage;

    if (0 == timeSpentInPostingMin)
    {
        timeSpentInPostingMin = timeSpentInPostingThisMessage;
    }

    if (timeSpentInPostingMin > timeSpentInPostingThisMessage)
    {
        timeSpentInPostingMin = timeSpentInPostingThisMessage;
    }

    if (0 == timeSpentInPostingMax)
    {
        timeSpentInPostingMax = timeSpentInPostingThisMessage;
    }

    if (timeSpentInPostingMax < timeSpentInPostingThisMessage)
    {
        timeSpentInPostingMax = timeSpentInPostingThisMessage;
    }

    if (0 == (numberofMessages % 10000))
    {
        timeSpentInPosting /= 10000;
        printf ("Time spent in posting per message Average = %lu\n", timeSpentInPosting);
        printf ("Time spent in posting per message Minimum = %lu\n", timeSpentInPostingMin);
        printf ("Time spent in posting per message Maximum = %lu\n", timeSpentInPostingMax);

        timeSpentInPosting    = 0;
        numberofMessages      = 0;
        timeSpentInPostingMin = 0;
        timeSpentInPostingMax = 0;
    }
#endif

    // In the following code clearing TcpCork and setting TcpNoDelay are deliberately commented out.

    if (false == isSuccessful)
    {
//        pClientStreamingSocket->clearTcpCork ();
//        pClientStreamingSocket->setTcpNoDelay ();

        return (false);
    }

    UI32         i                 = 0;
    vector<UI32> bufferTagsVector;

    if (numberOfBuffers > 0)
    {
        pWaveBrokerBasedMessage->getBufferTags (bufferTagsVector);
    }

#if 0
    isSuccessful = (*this) << numberOfBuffers;

    if (false == isSuccessful)
    {
//        pClientStreamingSocket->clearTcpCork ();
//        pClientStreamingSocket->setTcpNoDelay ();

        return (false);
    }
#endif

    for (i = 0; i < numberOfBuffers; i++)
    {
        UI32  bufferTag  = bufferTagsVector[i];
        void *pBuffer    = NULL;

        pBuffer = pWaveBrokerBasedMessage->findBuffer(bufferTag, bufferSize);

        FixedSizeBuffer tempBuffer (bufferSize, pBuffer);

        prismAssert (0 != bufferSize, __FILE__, __LINE__);

        isSuccessful = (*this) << bufferTag;

        if (false == isSuccessful)
        {
//            pClientStreamingSocket->clearTcpCork ();
//            pClientStreamingSocket->setTcpNoDelay ();

            return (false);
        }

        isSuccessful = (*this) << tempBuffer;

        if (false == isSuccessful)
        {
//            pClientStreamingSocket->clearTcpCork ();
//            pClientStreamingSocket->setTcpNoDelay ();

            return (false);
        }
    }

//    pClientStreamingSocket->clearTcpCork ();
//    pClientStreamingSocket->setTcpNoDelay ();

    return (true);
}

bool ClientStreamingSocket::operator << (WaveBrokerBasedMessagePayload *pWaveBrokerBasedMessagePayload)
{
#if 0
    prismAssert (NULL != pWaveBrokerBasedMessagePayload, __FILE__, __LINE__);

    pWaveBrokerBasedMessagePayload->prepareForPostingOnWire ();

    string bufferToPost = pWaveBrokerBasedMessagePayload->getMessageBody ();

    bool   isSuccessful = false;
#endif
#if 0
    isSuccessful = (*this) << bufferSize;

    if (false == isSuccessful)
    {
        return (false);
    }
#endif

// Following instrumentation is for figuring out how the ethernet driver is performing.
// Disable the instrumentation.  Enable ony for ethernet performance testing purposes.

#if 0
    //    pClientStreamingSocket->clearTcpNoDelay ();
    //    pClientStreamingSocket->setTcpCork ();

                        struct timespec timespec1;
                        struct timespec timespec2;
    static unsigned long int             timeSpentInPosting            = 0;
    static unsigned long int             timeSpentInPostingThisMessage = 0;
    static unsigned long int             timeSpentInPostingMin         = 0;
    static unsigned long int             timeSpentInPostingMax         = 0;
    static unsigned      int             numberofMessages              = 0;
                        int             clockReturnValue              = 0;

    numberofMessages++;

    clockReturnValue = clock_gettime (CLOCK_MONOTONIC, &timespec1);

    if (0 != clockReturnValue)
    {
        prismAssert (false, __FILE__, __LINE__);
    }
#endif

//    isSuccessful = (*this) << bufferToPost;

    struct iovec *pIoVector;
    UI32           ioVectorCount;

    pWaveBrokerBasedMessagePayload->prepareAndGetIovecToPostOnWire (&pIoVector, ioVectorCount);

    int status = writev (m_socket, pIoVector, ioVectorCount);
    bool isSuccessful = false;

    if (0 == status)
    {
        isSuccessful = true;
    }

// Following instrumentation is for figuring out how the thernet driver is performing.
// Disable the instrumentation.  Enable ony for ethernet performance testing purposes.

#if 0
    clockReturnValue = clock_gettime (CLOCK_MONOTONIC, &timespec2);

    if (0 != clockReturnValue)
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    //printf ("sec = %20ld, nsec = %20ld\n", timespec1.tv_sec, timespec1.tv_nsec);
    //printf ("sec = %20ld, nsec = %20ld\n", timespec2.tv_sec, timespec2.tv_nsec);
    timeSpentInPostingThisMessage = (timespec2.tv_sec * 1000000000 + timespec2.tv_nsec) - (timespec1.tv_sec * 1000000000 + timespec1.tv_nsec);
    timeSpentInPosting += timeSpentInPostingThisMessage;

    if (0 == timeSpentInPostingMin)
    {
        timeSpentInPostingMin = timeSpentInPostingThisMessage;
    }

    if (timeSpentInPostingMin > timeSpentInPostingThisMessage)
    {
        timeSpentInPostingMin = timeSpentInPostingThisMessage;
    }

    if (0 == timeSpentInPostingMax)
    {
        timeSpentInPostingMax = timeSpentInPostingThisMessage;
    }

    if (timeSpentInPostingMax < timeSpentInPostingThisMessage)
    {
        timeSpentInPostingMax = timeSpentInPostingThisMessage;
    }

    if (0 == (numberofMessages % 10000))
    {
        timeSpentInPosting /= 10000;
        printf ("Time spent in posting per message Average = %lu\n", timeSpentInPosting);
        printf ("Time spent in posting per message Minimum = %lu\n", timeSpentInPostingMin);
        printf ("Time spent in posting per message Maximum = %lu\n", timeSpentInPostingMax);

        timeSpentInPosting    = 0;
        numberofMessages      = 0;
        timeSpentInPostingMin = 0;
        timeSpentInPostingMax = 0;
    }
#endif

    // In the following code clearing TcpCork and setting TcpNoDelay are deliberately commented out.

    if (false == isSuccessful)
    {
//        pClientStreamingSocket->clearTcpCork ();
//        pClientStreamingSocket->setTcpNoDelay ();

        return (false);
    }

    UI32           i                 = 0;
    vector<UI32>  &bufferTagsVector  = pWaveBrokerBasedMessagePayload->getBufferTags ();
    UI32           numberOfBuffers   = bufferTagsVector.size ();
    vector<UI8 *> &buffers           = pWaveBrokerBasedMessagePayload->getBuffers ();
    vector<UI32>  &bufferSizesVector = pWaveBrokerBasedMessagePayload->getBufferSizes ();

#if 0
    isSuccessful = (*this) << numberOfBuffers;

    if (false == isSuccessful)
    {
//        pClientStreamingSocket->clearTcpCork ();
//        pClientStreamingSocket->setTcpNoDelay ();

        return (false);
    }
#endif

    for (i = 0; i < numberOfBuffers; i++)
    {
        UI32  bufferTag  = bufferTagsVector[i];
        void *pBuffer    = buffers[i];
        UI32  bufferSize = bufferSizesVector[i];

        prismAssert (0 != bufferSize, __FILE__, __LINE__);

        FixedSizeBuffer tempBuffer (bufferSize, pBuffer);

        isSuccessful = (*this) << bufferTag;

        if (false == isSuccessful)
        {
//            pClientStreamingSocket->clearTcpCork ();
//            pClientStreamingSocket->setTcpNoDelay ();

            return (false);
        }

        isSuccessful = (*this) << tempBuffer;

        if (false == isSuccessful)
        {
//            pClientStreamingSocket->clearTcpCork ();
//            pClientStreamingSocket->setTcpNoDelay ();

            return (false);
        }
    }

//    pClientStreamingSocket->clearTcpCork ();
//    pClientStreamingSocket->setTcpNoDelay ();

    return (true);
}

bool ClientStreamingSocket::operator << (const string &dataString)
{
    if (-1 == (StreamingSocketBase::send (dataString)))
    {
        cout << "*** errno = " << errno << " ***\n";
        return (false);
    }
    else
    {
        return (true);
    }
}

bool ClientStreamingSocket::operator >> (string &dataString)
{
    if (true != (receive (dataString)))
    {
        cout << "*** errno = " << errno << " ***\n";
        return (false);
    }

    return (true);
}

bool ClientStreamingSocket::operator >> (FixedSizeBuffer &fixedSizeBuffer)
{
    UI32 numberOfBytesRemainingToRead = fixedSizeBuffer.getRemainingSize ();

    while (numberOfBytesRemainingToRead > 0)
    {
        SI32 numberOfBytesRead = receiveAll (fixedSizeBuffer.getPCurrentRawBuffer (), fixedSizeBuffer.getRemainingSize ());

        if (numberOfBytesRead > 0)
        {
            fixedSizeBuffer.incrementCurrentSize (numberOfBytesRead);
            numberOfBytesRemainingToRead -= numberOfBytesRead;
        }
        else
        {
            return (false);
        }
    }

    return (true);
}

bool ClientStreamingSocket::operator >> (UI32 &value)
{
    return (receiveUI32 (value));
}

void ClientStreamingSocket::print ()
{
}

bool ClientStreamingSocket::getIsConnected () const
{
    return (m_isConnected);
}

string ClientStreamingSocket::getHost () const
{
    return (m_host);
}

SI32 ClientStreamingSocket::getPort () const
{
    return (m_port);
}

bool ClientStreamingSocket::tryToConnect ()
{
    if (true == getIsConnected ())
    {
        return (true);
    }
    else
    {
        if (true != (connect (m_host, m_port)))
        {
            //cout << "*** errno = " << errno << " ***\n";
            m_isConnected = false;
        }
        else
        {
            m_isConnected = true;
        }

        return (m_isConnected);
    }
}

string ClientStreamingSocket::getSourceIpAddress ()
{
    return (StreamingSocket::getSourceIpAddress ());
}

string ClientStreamingSocket::getPeerIpAddress ()
{
    return (StreamingSocket::getPeerIpAddress ());
}

SI32 ClientStreamingSocket::getPeerPort ()
{
    return (StreamingSocket::getPeerPort ());
}

string ClientStreamingSocket::getToMessageVersion () const
{
    return (getMessageVersion ());
}

UI8 ClientStreamingSocket::getToSerializationType () const
{
    return (getSerializationType ());
}

void ClientStreamingSocket::setToMessageVersionAndSerializationType (const string& toMsgVersion, const UI8& serializationType)
{
    setMessageVersionAndSerializationType (toMsgVersion, serializationType);
}

bool ClientStreamingSocket::setCustomKeepAliveTimeouts (UI32 keepAliveIdleTimeoutInMilliSeconds, UI32 keepAliveProbeIntervalInMilliSeconds, UI32 keepAliveProbeCount)
{
    bool setSocketOptionStatus = false;

    setSocketOptionStatus = setTcpKeepAliveIdleTimeout (keepAliveIdleTimeoutInMilliSeconds);

    // if (false == setSocketOptionStatus)
    // {
    //     prismAssert (false, __FILE__, __LINE__);
    // }

    if (true == setSocketOptionStatus)
    {
        setSocketOptionStatus = setTcpKeepAliveProbeInterval (keepAliveProbeIntervalInMilliSeconds);

        // if (false == setSocketOptionStatus)
        // {
        //     prismAssert (false, __FILE__, __LINE__);
        // }
    }

    if (true == setSocketOptionStatus)
    {
        setSocketOptionStatus = setTcpKeepAliveProbeCount (keepAliveProbeCount);

        // if (false == setSocketOptionStatus)
        // {
        //     prismAssert (false, __FILE__, __LINE__);
        // }
    }

    return setSocketOptionStatus;
}

bool ClientStreamingSocket::setDefaultKeepAliveTimeouts ()
{
    return (setCustomKeepAliveTimeouts (m_customDefaultKeepAliveIdleTimeoutInMilliSeconds, m_customDefaultKeepAliveProbeIntervalInMilliSeconds, m_customDefaultKeepAliveProbeCount));
}


}
