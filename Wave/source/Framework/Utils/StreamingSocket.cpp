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

#include "Framework/Utils/StreamingSocket.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include <errno.h>
//#include "Framework/ObjectModel/SerializableObject.h"

namespace WaveNs
{

StreamingSocket::StreamingSocket (const SI32 maximumNumberOfConnections)
    : StreamingSocketBase (maximumNumberOfConnections)
{
    m_socket = ::socket (PF_INET, SOCK_STREAM, 0);

    if (true != isValid ())
    {
        tracePrintf (TRACE_LEVEL_FATAL, "StreamingSocket::StreamingSocket : %d\n", errno);
        prismAssert (false, __FILE__, __LINE__);
    }

    SI32 temp = 1;

    if (-1 == (setsockopt (m_socket, SOL_SOCKET, SO_REUSEADDR, (const char *) &temp, sizeof (temp))))
    {
        tracePrintf (TRACE_LEVEL_FATAL, "StreamingSocket::StreamingSocket : %d\n", errno);
        prismAssert (false, __FILE__, __LINE__);
    }

    if (-1 == (setsockopt (m_socket, SOL_SOCKET, SO_KEEPALIVE, (const char *) &temp, sizeof (temp))))
    {
        tracePrintf (TRACE_LEVEL_FATAL, "StreamingSocket::StreamingSocket : %d\n", errno);
        prismAssert (false, __FILE__, __LINE__);
    }

// Disable the TCP_NODELAY option temporarily.  MPC 8548 ehternet driver performs pporly if TCP_NODELAY or TCP_CORK options are set.
	bool tcpNoDelayStatus = false;

	tcpNoDelayStatus = setTcpNoDelay ();

	if (false == tcpNoDelayStatus)
	{
		trace (TRACE_LEVEL_FATAL, "Not Able to Set No Tcp Delay");
		prismAssert (false, __FILE__, __LINE__);
	}
	else
	{
		trace (TRACE_LEVEL_DEBUG, "Set No TCP DELAY");
	}

    bool sendTimeoutStatus = true;

    // Setting timeout to 40 seconds.
    sendTimeoutStatus = setTcpSendTimeout (40000);
    // Note : as send is retried in a loop, total time until error is received may be two-three times this timeout value.

    if (false == sendTimeoutStatus)
    {
        trace (TRACE_LEVEL_FATAL, "Not Able to set send timout");
        prismAssert (false, __FILE__, __LINE__); 
    }

    memset (&m_socketAddress, 0, sizeof (m_socketAddress));
}

StreamingSocket::StreamingSocket ()
    : StreamingSocketBase ()
{
    m_socket                     = -1;
    m_maximumNumberOfConnections = 1;
    memset (&m_socketAddress, 0, sizeof (m_socketAddress));
}

StreamingSocket::~StreamingSocket ()
{
}

bool StreamingSocket::setTcpNoDelay ()
{
    SI32 flag = 1;

    if (-1 == (setsockopt (m_socket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof (flag))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool StreamingSocket::clearTcpNoDelay ()
{
    SI32 flag = 0;

    if (-1 == (setsockopt (m_socket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof (flag))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool StreamingSocket::setTcpSendTimeout (UI32 timeoutInMilliSeconds)
{
    struct timeval tv = {0};

    tv.tv_sec = timeoutInMilliSeconds/1000;

    if (-1 == (setsockopt (m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *) &tv, sizeof (tv))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool StreamingSocket::clearTcpSendTimeout ()
{
    struct timeval tv = {0};

    tv.tv_sec = 0;

    if (-1 == (setsockopt (m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *) &tv, sizeof (tv))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool StreamingSocket::setTcpCork ()
{
    SI32 flag = 1;

    if (-1 == (setsockopt (m_socket, IPPROTO_TCP, TCP_CORK, (char *) &flag, sizeof (flag))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool StreamingSocket::clearTcpCork ()
{
    SI32 flag = 0;

    if (-1 == (setsockopt (m_socket, IPPROTO_TCP, TCP_CORK, (char *) &flag, sizeof (flag))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool StreamingSocket::setTcpKeepAliveIdleTimeout (UI32 timeoutInMilliSeconds)
{
    // cout<<"bool StreamingSocket::setTcpKeepAliveIdleTimeout (UI32 timeoutInMilliSeconds) ["<<timeoutInMilliSeconds<<"]"<<endl;

    struct timeval tv = {0};

    tv.tv_sec = timeoutInMilliSeconds / 1000;

    if (-1 == (setsockopt (m_socket, IPPROTO_TCP, TCP_KEEPIDLE, (char *) &tv, sizeof (tv))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool StreamingSocket::setTcpKeepAliveProbeInterval (UI32 timeoutInMilliSeconds)
{
    // cout<<"bool StreamingSocket::setTcpKeepAliveProbeInterval (UI32 timeoutInMilliSeconds) ["<<timeoutInMilliSeconds<<"]"<<endl;

    struct timeval tv = {0};

    tv.tv_sec = timeoutInMilliSeconds / 1000;

    if (-1 == (setsockopt (m_socket, IPPROTO_TCP, TCP_KEEPINTVL, (char *) &tv, sizeof (tv))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool StreamingSocket::setTcpKeepAliveProbeCount (UI32 numberOfProbes)
{
    // cout<<"bool StreamingSocket::setTcpKeepAliveProbeCount (UI32 numberOfProbes) ["<<numberOfProbes<<"]"<<endl;

    int numberOfKeepAliveProbes = (int) numberOfProbes;

    if (-1 == (setsockopt (m_socket, IPPROTO_TCP, TCP_KEEPCNT, (char *) &numberOfKeepAliveProbes, sizeof (numberOfKeepAliveProbes))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool StreamingSocket::bind (const SI32 port)
{
    if (true != (isValid ()))
    {
        return (false);
    }

    memset (&m_socketAddress, 0, sizeof (m_socketAddress));

    m_socketAddress.sin_family      = PF_INET;
    m_socketAddress.sin_port        = htons (port);
    m_socketAddress.sin_addr.s_addr = INADDR_ANY;

    SI32 status = ::bind (m_socket, (struct sockaddr *) &m_socketAddress, sizeof (m_socketAddress));

    if (-1 != status)
    {
        return (true);
    }
    else
    {
        tracePrintf (TRACE_LEVEL_DEBUG, "StreamingSocket::bind : %d\n", errno);
        //cout << "*** errno = " << errno << " ***\n";
        return (false);
    }
}

bool StreamingSocket::accept (StreamingSocketBase &newStreamingSocketBase)
{
    if (true != (isValid ()))
    {
        return (false);
    }

    SI32 lengthOfScocketAddress = sizeof (m_socketAddress);

    newStreamingSocketBase.setSocket (::accept (m_socket, (struct sockaddr *) &m_socketAddress, (socklen_t *) &lengthOfScocketAddress));

    if ((newStreamingSocketBase.getSocket ()) > 0)
    {
        return (true);
    }
    else
    {
        tracePrintf (TRACE_LEVEL_ERROR, "StreamingSocket::accept : %d\n", errno);
        //cout << "*** errno = " << errno << " ***\n";
        return (false);
    }
}

bool StreamingSocket::connect (const string &host, const SI32 port)
{
    if (true != (isValid ()))
    {
        return (false);
    }

    m_socketAddress.sin_family = AF_INET;
    m_socketAddress.sin_port   = htons (port);

    SI32 status = inet_pton (AF_INET, host.c_str (), &m_socketAddress.sin_addr);

    if (0 >= status)
    {
        tracePrintf (TRACE_LEVEL_ERROR, "StreamingSocket::connect : %d\n", errno);
        //cout << "*** errno = " << errno << " ***\n";
        return (false);
    }

    status = ::connect (m_socket, (sockaddr *) &m_socketAddress, sizeof (m_socketAddress));

    if (0 == status)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string StreamingSocket::getSourceIpAddress ()
{
    if (true != (isValid ()))
    {
        return (NULL);
    }
    else
    {
        return (inet_ntoa (m_socketAddress.sin_addr));
    }
}

string StreamingSocket::getPeerIpAddress ()
{
    if (true != (isValid ()))
    {
        return (NULL);
    }
    else
    {
        sockaddr_in tempSocketAddress;
        SI32        length = sizeof (tempSocketAddress);

        getpeername (m_socket, (struct sockaddr *) &tempSocketAddress, (socklen_t *) &length);
        return (inet_ntoa (tempSocketAddress.sin_addr));
    }
}

SI32 StreamingSocket::getPeerPort ()
{
    if (true != (isValid ()))
    {
        return (0);
    }
    else
    {
        sockaddr_in tempSocketAddress;
        SI32        length = sizeof (tempSocketAddress);

        getpeername (m_socket, (struct sockaddr *) &tempSocketAddress, (socklen_t *) &length);
        return (tempSocketAddress.sin_port);
    }
}

string StreamingSocket::getAPublicIpAddressForThisMachine ()
{
    string         publicIpAddress           = "";
    vector<string> publicIpAddressesVector;
    UI32           numberOfPublicIpAddresses = 0;

    getPublicIpAddressesForThisMachine (publicIpAddressesVector);
    numberOfPublicIpAddresses = publicIpAddressesVector.size ();

    if (0 < numberOfPublicIpAddresses)
    {
        publicIpAddress = publicIpAddressesVector[0];
    }

    return (publicIpAddress);
}

void StreamingSocket::getPublicIpAddressesForThisMachine (vector<string> &ipAddressesVector)
{
    getIpAddressesForThisHost (ipAddressesVector);
}

string StreamingSocket::getIpAddressForInterface (const string &interfaceName)
{
    return (WaveNs::getIpAddressForInterface (interfaceName));
}

void StreamingSocket::close (void)
{
    prismCloseSocket (getSocket ());
    setSocket (-1);
}

}
