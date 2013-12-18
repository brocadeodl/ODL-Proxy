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

#ifndef CLIENTSTREAMINGSOCKET_H
#define CLIENTSTREAMINGSOCKET_H

#include "Framework/Utils/StreamingSocket.h"
#include "Framework/Utils/FixedSizeBuffer.h"
#include <string>

namespace WaveNs
{

class SerializableObject;
class PrismMessage;
class WaveBrokerBasedMessage;
class WaveBrokerBasedMessagePayload;

class ClientStreamingSocket : private StreamingSocket
{
    private :
    protected :
    public :
                                             ClientStreamingSocket (const string &host, const SI32 port, const SI32 maximumNumberOfConnections);
                                             ClientStreamingSocket ();
        virtual                             ~ClientStreamingSocket ();

                      bool                   setTcpNoDelay         ();
                      bool                   clearTcpNoDelay       ();
                      bool                   setTcpCork            ();
                      bool                   clearTcpCork          ();

                      bool                   send                  (UI8 *pBuffer, const UI32 maximumBufferLength);
                      bool                   operator <<           (const UI32 &value);
                      bool                   operator <<           (const string &dataString);
                      bool                   operator <<           (const FixedSizeBuffer &fixedsizeBuffer);
                      bool                   operator <<           (SerializableObject *pSerializableObject);
                      bool                   operator <<           (PrismMessage *pPrismMessge);
                      bool                   operator <<           (WaveBrokerBasedMessage *pWaveBrokerBasedMessage);
                      bool                   operator <<           (WaveBrokerBasedMessagePayload *pWaveBrokerBasedMessagePayload);
                      bool                   operator >>           (string &dataString);
                      bool                   operator >>           (FixedSizeBuffer &fixedsizeBuffer);
                      bool                   operator >>           (UI32 &value);
                      void                   print                 ();
                      bool                   getIsConnected        () const;
                      string                 getHost               () const;
                      SI32                   getPort               () const;
                      bool                   tryToConnect          ();
        virtual       string                 getSourceIpAddress    ();
        virtual       string                 getPeerIpAddress      ();
        virtual       SI32                   getPeerPort           ();

                      void                   setToMessageVersionAndSerializationType   (const string& toMessageVersion = "", const UI8& serializationType = SERIALIZE_WITH_ATTRIBUTE_NAME);
                      string                 getToMessageVersion                       () const;
                      UI8                    getToSerializationType                    () const;

                      // Keep Alive settings -
                      bool                   setCustomKeepAliveTimeouts                (UI32 keepAliveIdleTimeoutInMilliSeconds, UI32 keepAliveProbeIntervalInMilliSeconds, UI32 keepAliveProbeCount);
                      // sets custom timeouts to WaveNs::ServerStreamingSocket::* values.
                      bool                   setDefaultKeepAliveTimeouts               ();

    // Now the data members

    private :
        string m_host;
        SI32   m_port;
        bool   m_isConnected;
        static const  UI32          m_customDefaultKeepAliveIdleTimeoutInMilliSeconds;
        static const  UI32          m_customDefaultKeepAliveProbeIntervalInMilliSeconds;
        static const  UI32          m_customDefaultKeepAliveProbeCount;

    protected :
    public :
};

}

#endif // CLIENTSTREAMINGSOCKET_H
