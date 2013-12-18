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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Utils/UnixDomainStreamingSocket.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include <errno.h>

namespace WaveNs
{

const string UnixDomainStreamingSocket::serverPathPrefix = "/tmp/PRISM.";

UnixDomainStreamingSocket::UnixDomainStreamingSocket (const SI32 maximumNumberOfConnections)
    : StreamingSocketBase (maximumNumberOfConnections)
{
    m_socket = ::socket (PF_LOCAL, SOCK_STREAM, 0);

    if (true != isValid ())
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    SI32 temp = 1;

    if (-1 == (setsockopt (m_socket, SOL_SOCKET, SO_REUSEADDR, (const char *) &temp, sizeof (temp))))
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    if (-1 == (setsockopt (m_socket, SOL_SOCKET, SO_KEEPALIVE, (const char *) &temp, sizeof (temp))))
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    memset (&m_socketAddress, 0, sizeof (m_socketAddress));
}

UnixDomainStreamingSocket::UnixDomainStreamingSocket ()
    : StreamingSocketBase ()
{
    m_socket                     = -1;
    m_maximumNumberOfConnections = 1;
    memset (&m_socketAddress, 0, sizeof (m_socketAddress));
}

UnixDomainStreamingSocket::~UnixDomainStreamingSocket ()
{
}

bool UnixDomainStreamingSocket::bind (const SI32 port)
{
    if (true != (isValid ()))
    {
        return (false);
    }

    string serverUdsPath = serverPathPrefix + port;

    memset (&m_socketAddress, 0, sizeof (m_socketAddress));

    m_socketAddress.sun_family = PF_LOCAL;

    if (serverUdsPath.length() > sizeof(m_socketAddress.sun_path))
    {
        return (false);
    }
    
    strncpy (m_socketAddress.sun_path, serverUdsPath.c_str (), serverUdsPath.length());

    SI32 status = ::bind (m_socket, (struct sockaddr *) &m_socketAddress, SUN_LEN (&m_socketAddress));

    if (-1 != status)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool UnixDomainStreamingSocket::accept (StreamingSocketBase &newStreamingSocketBase)
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
        //cout << "*** errno = " << errno << " ***\n";
        return (false);
    }
}

bool UnixDomainStreamingSocket::connect (const string &host, const SI32 port)
{
    if (true != (isValid ()))
    {
        return (false);
    }

    string serverUdsPath = serverPathPrefix + port;

    m_socketAddress.sun_family  = PF_LOCAL;

    if (serverUdsPath.length() > sizeof(m_socketAddress.sun_path))
    {
        return (false);
    }

    strncpy (m_socketAddress.sun_path, serverUdsPath.c_str (), serverUdsPath.length());

    int status = ::connect (m_socket, (sockaddr *) &m_socketAddress, SUN_LEN (&m_socketAddress));

    if (0 == status)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

}

