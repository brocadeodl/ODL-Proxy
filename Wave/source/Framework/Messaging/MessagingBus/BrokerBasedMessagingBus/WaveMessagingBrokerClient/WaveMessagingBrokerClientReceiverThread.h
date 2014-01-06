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

#ifndef WAVEMESSAGINGBROKERCLIENTRECEIVERTHREAD_H
#define WAVEMESSAGINGBROKERCLIENTRECEIVERTHREAD_H

#include "Framework/MultiThreading/PrismPosixThread.h"
#include "Framework/Utils/PrismMutex.h"

namespace WaveNs
{

class ServerStreamingSocket;
class WaveBrokerBasedMessage;
class WaveBrokerPublishMessage;

class WaveMessagingBrokerClientReceiverThread : public PrismPosixThread
{
    private :
                                 WaveMessagingBrokerClientReceiverThread (ServerStreamingSocket *pServerStreamingSocket);

        virtual WaveThreadStatus start                                   ();

                bool             authorizeClient                         ();

                void             setPeerServerDetails                    (const string &peerServerIpAddress, const SI32 &peerServerPort);

                void             setPeerClientName                       (const string &peerClientName);
                string           getPeerClientName                       () const;

                void             processWaveBrokerBasedMessage           (WaveBrokerBasedMessage   *pWaveBrokerBasedMessage, const char *pMessageBuffer);
                void             processWaveBrokerBasedMessage           (WaveBrokerPublishMessage *pWaveBrokerPublishMessage);

                void             reconnectToPeerServer                   ();

    protected :
    public :
        virtual ~WaveMessagingBrokerClientReceiverThread ();

    // Now the data members

    private :
        ServerStreamingSocket *m_pServerStreamingSocket;
        string                 m_peerServerIpAddress;
        SI32                   m_peerServerPort;
        PrismMutex             m_peerServerMutex;

        string                 m_peerClientName;

        string                 m_brokerNameFromClientPerspective;

    protected :
    public :

    friend class WaveMessagingBrokerClientReceiverObjectManager;
};

}

#endif // WAVEMESSAGINGBROKERCLIENTRECEIVERTHREAD_H