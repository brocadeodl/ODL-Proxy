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

#ifndef WAVEMESSAGINGBROKERCLIENTRECEIVEROBJECTMANAGER_H
#define WAVEMESSAGINGBROKERCLIENTRECEIVEROBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

namespace WaveNs
{

class WaveAsynchronousContextForBootPhases;
class ServerStreamingSocket;
class BootCompleteForThisLocationEvent;

class WaveMessagingBrokerClientReceiverObjectManager : public WaveLocalObjectManagerForUserSpecificTasks
{
    private :
        virtual void boot                                         (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void initialize                                   (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        virtual void listenForEvents                              (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void bootCompleteForThisLocationEventHandler      (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);

                void  registerWaveBrokerBasedMessageInstantiators ();

        static  void debugConnectToWaveMessageBroker              (UI32 argc, vector<string> argv);
        static  void debugSubscribeToWaveMessageBroker            (UI32 argc, vector<string> argv);
        static  void registerDebugShellEntries                    ();


    protected :
    public :
                                                                WaveMessagingBrokerClientReceiverObjectManager ();
        virtual                                                ~WaveMessagingBrokerClientReceiverObjectManager ();

        static  WaveMessagingBrokerClientReceiverObjectManager *getInstance                                    ();
        static  PrismServiceId                                  getPrismServiceId                              ();
        static  string                                          getServiceName                                 ();

    // Now the data members

    private :
        ServerStreamingSocket *m_pServerSocketForWaveMessagingBrokerClients;

    protected :
    public :
};

}

#endif // WAVEMESSAGINGBROKERCLIENTRECEIVEROBJECTMANAGER_H
