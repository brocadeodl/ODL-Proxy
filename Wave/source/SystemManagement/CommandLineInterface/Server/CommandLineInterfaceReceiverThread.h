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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef COMMANDLINEINTERFACERECEIVERTHREAD_H
#define COMMANDLINEINTERFACERECEIVERTHREAD_H

#include "Framework/MultiThreading/PrismPosixThread.h"

namespace WaveNs
{

class ServerStreamingSocket;
class CommandLineInterfaceReceiverObjectManager;

class CommandLineInterfaceReceiverThread : public PrismPosixThread
{
    private :
        virtual WaveThreadStatus start ();

    protected :
    public :
                 CommandLineInterfaceReceiverThread (CommandLineInterfaceReceiverObjectManager *pCommandLineInterfaceReceiverObjectManager, ServerStreamingSocket *pServerStreamingSocket);
        virtual ~CommandLineInterfaceReceiverThread ();

    // Now the data members

    private :
        CommandLineInterfaceReceiverObjectManager *m_pCommandLineInterfaceReceiverObjectManager;
        ServerStreamingSocket                     *m_pServerSocketForCommandLineInterfaceClients;

    protected :
    public :
};

}

#endif // COMMANDLINEINTERFACERECEIVERTHREAD_H
