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

#ifndef INTERLOCATIONMESSAGERECEIVERTHREAD_H
#define INTERLOCATIONMESSAGERECEIVERTHREAD_H

#include "Framework/MultiThreading/PrismPosixThread.h"
#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"
#include <string>

namespace WaveNs
{

class ServerStreamingSocket;

class InterLocationMessageReceiverThread : public PrismPosixThread
{
    private :
        bool authorizeClient ();

    protected :
    public :
                                   InterLocationMessageReceiverThread (ServerStreamingSocket *pServerStreamingSocket);
        virtual                   ~InterLocationMessageReceiverThread ();
        virtual WaveThreadStatus   start                              ();
                void               setPeerServerDetails               (const string &peerServerIpAddress, const SI32 &peerServerPort, const string& messageVersion = "");
                string             getPeerServerMessageVersion        ()const;
                UI8                getPeerServerSerializationType     () const;
                void               closePeerServerStreamingSocket     (void);

    // Now the data members

    private :
        ServerStreamingSocket *m_pServerStreamingSocket;
        string                 m_peerServerIpAddress;
        SI32                   m_peerServerPort;
        PrismMutex             m_peerServerMutex;
        string                 m_peerServerMessageVersion;
        UI8                    m_peerServerSerializationType;

    protected :
    public :
};

}

#endif // INTERLOCATIONMESSAGERECEIVERTHREAD_H