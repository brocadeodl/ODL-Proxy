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

#include "Framework/Utils/ServerStreamingSocket.h"
#include "errno.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

const   UI32    ServerStreamingSocket::m_customDefaultKeepAliveIdleTimeoutInMilliSeconds    = 30000;
const   UI32    ServerStreamingSocket::m_customDefaultKeepAliveProbeIntervalInMilliSeconds  = 20000;
const   UI32    ServerStreamingSocket::m_customDefaultKeepAliveProbeCount                   = 3;

ServerStreamingSocket::ServerStreamingSocket (SI32 port, SI32 maximumNumberOfConnections)
    : StreamingSocket (maximumNumberOfConnections),
      m_status (SERVER_STREAMING_SOCKET_SUCCESS)
{
    if (true != (StreamingSocket::bind (port)))
    {
//        cout << "*** errno = " << errno << " ***\n";
        m_status = SERVER_STREAMING_SOCKET_ERROR_COULD_NOT_BIND;
//        prismAssert (false, __FILE__,__LINE__);
    }

    if (true != (StreamingSocket::listen ()))
    {
//        cout << "*** errno = " << errno << " ***\n";
        m_status = SERVER_STREAMING_SOCKET_ERROR_COULD_NOT_LISTEN;
//        prismAssert (false, __FILE__,__LINE__);
    }
}

ServerStreamingSocket::ServerStreamingSocket ()
    : StreamingSocket (),
      m_status (SERVER_STREAMING_SOCKET_SUCCESS)
{
}

ServerStreamingSocket::~ServerStreamingSocket ()
{
}

const ServerStreamingSocket &ServerStreamingSocket::operator << (const string &dataString)
{
    if (-1 == (send (dataString)))
    {
//        cout << "*** errno = " << errno << " ***\n";
        //prismAssert (false, __FILE__,__LINE__);
    }

    return (*this);
}

bool ServerStreamingSocket::operator << (SerializableObject *pSerializableObject)
{
    if (-1 == (send (pSerializableObject)))
    {
        //cout << "*** errno = " << errno << " ***\n";
        return (false);
    }
    else
    {
        return (true);
    }
}


bool ServerStreamingSocket::operator >> (string &dataString)
{
    if (true != (receive (dataString)))
    {
//        cout << "*** errno = " << errno << " ***\n";
        return (false);
    }

    return (true);
}

bool ServerStreamingSocket::operator >> (FixedSizeBuffer &fixedSizeBuffer)
{
    UI32 numberOfBytesRemainingToRead = fixedSizeBuffer.getMaximumSize ();

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

bool ServerStreamingSocket::operator >> (UI32 &value)
{
    return (receiveUI32 (value));
}

bool ServerStreamingSocket::accept (ServerStreamingSocket &newSocket)
{
    bool successfullyAcceptedNewConnection = StreamingSocket::accept (newSocket);

    if (true != successfullyAcceptedNewConnection)
    {
//        cout << "*** errno = " << errno << " ***\n";
    }

    return (successfullyAcceptedNewConnection);
}
string ServerStreamingSocket::getSourceIpAddress ()
{
    return (StreamingSocket::getSourceIpAddress ());
}

string ServerStreamingSocket::getPeerIpAddress ()
{
    return (StreamingSocket::getPeerIpAddress ());
}

const ServerStreamingSocketStatus ServerStreamingSocket::getStatus ()
{
    return (m_status);
}

SI32 ServerStreamingSocket::getPeerPort ()
{
    return (StreamingSocket::getPeerPort ());
}

void ServerStreamingSocket::close (void)
{
    StreamingSocket::close ();
}

bool ServerStreamingSocket::setCustomKeepAliveTimeouts (UI32 keepAliveIdleTimeoutInMilliSeconds, UI32 keepAliveProbeIntervalInMilliSeconds, UI32 keepAliveProbeCount)
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

bool ServerStreamingSocket::setCustomKeepAliveTimeouts ()
{
    return (setCustomKeepAliveTimeouts (m_customDefaultKeepAliveIdleTimeoutInMilliSeconds, m_customDefaultKeepAliveProbeIntervalInMilliSeconds, m_customDefaultKeepAliveProbeCount));
}

}

