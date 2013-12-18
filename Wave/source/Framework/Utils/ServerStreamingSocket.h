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

#ifndef SERVERSTREAMINGSOCKET_H
#define SERVERSTREAMINGSOCKET_H

#include "Framework/Utils/StreamingSocket.h"
#include "Framework/Utils/FixedSizeBuffer.h"

namespace WaveNs
{

typedef enum
{
    SERVER_STREAMING_SOCKET_SUCCESS = 0,
    SERVER_STREAMING_SOCKET_ERROR_COULD_NOT_BIND,
    SERVER_STREAMING_SOCKET_ERROR_COULD_NOT_LISTEN
} ServerStreamingSocketStatus;

class ServerStreamingSocket : protected StreamingSocket
{
    private :
    protected :
    public :
                                                   ServerStreamingSocket (SI32 port, SI32 maximumNumberOfConnections);
                                                   ServerStreamingSocket ();
        virtual                                   ~ServerStreamingSocket ();
                const ServerStreamingSocket       &operator <<           (const string &dataString);
                      bool                         operator <<           (SerializableObject *pSerializableObject);
                      bool                         operator >>           (string &dataString);
                      bool                         operator >>           (FixedSizeBuffer &fizedSizeBuffer);
                      bool                         operator >>           (UI32 &value);
                      bool                         accept                (ServerStreamingSocket &newSocket);
        virtual       string                       getSourceIpAddress    ();
        virtual       string                       getPeerIpAddress      ();
        virtual       SI32                         getPeerPort           ();
                const ServerStreamingSocketStatus  getStatus             ();

                      // Keep Alive settings -
                      bool                         setCustomKeepAliveTimeouts (UI32 keepAliveIdleTimeoutInMilliSeconds, UI32 keepAliveProbeIntervalInMilliSeconds, UI32 keepAliveProbeCount);
                      // sets custom timeouts to WaveNs::ServerStreamingSocket::* values.
                      bool                         setCustomKeepAliveTimeouts ();

                      void                         close                 (void);

    // Now the data members

    private :
        ServerStreamingSocketStatus m_status;
        static const  UI32          m_customDefaultKeepAliveIdleTimeoutInMilliSeconds;
        static const  UI32          m_customDefaultKeepAliveProbeIntervalInMilliSeconds;
        static const  UI32          m_customDefaultKeepAliveProbeCount;
    protected :
    public :
};

}

#endif // SERVERSTREAMINGSOCKET_H
