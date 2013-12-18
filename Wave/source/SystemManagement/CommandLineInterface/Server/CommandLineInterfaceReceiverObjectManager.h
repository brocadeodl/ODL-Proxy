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

#ifndef COMMANDLINEINTERFACERECEIVEROBJECTMANAGER_H
#define COMMANDLINEINTERFACERECEIVEROBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceReceiverWorkerThread.h"

namespace WaveNs
{

class ServerStreamingSocket;
class BootCompleteForThisLocationEvent;
class WaveAsynchronousContextForBootPhases;
class CommandLineInterfaceReceiverThread;

class CommandLineInterfaceReceiverObjectManager : public WaveLocalObjectManagerForUserSpecificTasks
{
    private :
                                  CommandLineInterfaceReceiverObjectManager           ();

        virtual void              initialize                                          (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void              boot                                                (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        virtual void              listenForEvents                                     (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void              bootCompleteForThisLocationEventHandler             (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);

        virtual WaveMessageStatus send                                                (PrismMessage *pPrismMessage, PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, void *pPrismMessageContext,  CommandLineInterfaceReceiverWorkerThread *pCommandLineInterfaceReceiverWorkerThread, UI32 timeOutInMilliSeconds = 0, LocationId locationId = 0);
                void              sendCallback                                        (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);

                bool              isAMessageSentByThisObjectManager                   (PrismMessage *pPrismMessage);
                void              addToCommandLineInterfaceReceiverMapsByMessage      (PrismMessage *pPrismMessage, CommandLineInterfaceReceiverWorkerThread *pCommandLineInterfaceReceiverWorkerThread, PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, void *pPrismMessageContext);
                void              removeFromCommandLineInterfaceReceiverMapsByMessage (PrismMessage *pPrismMessage);
                void              getCommandLineInterfaceReceiverDetailsForMessage    (PrismMessage *pPrismMessage, CommandLineInterfaceReceiverWorkerThread *&pCommandLineInterfaceReceiverWorkerThread, PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread &prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, void *&pPrismMessageContext);

    protected :
    public :
        virtual                                          ~CommandLineInterfaceReceiverObjectManager ();

        static string                                     getServiceName                            ();
        static CommandLineInterfaceReceiverObjectManager *getInstance                               ();
        static PrismServiceId                             getPrismServiceId                         ();

    // Now the data members

    private :
        ServerStreamingSocket                                                             *m_pServerSocketForCommandLineInterfaceClients;
        CommandLineInterfaceReceiverThread                                                *m_pCommandLineInterfaceReceiverThread;
        map<UI32, CommandLineInterfaceReceiverWorkerThread *>                              m_commandLineInterfaceReceiverWorkerThreadMapByMessageId;
        map<UI32, PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread>  m_commandLineInterfaceReceiverCallbackMapByMessageId;
        map<UI32, void *>                                                                  m_commandLineInterfaceReceiverContextMapByMessageId;

    protected :
    public:

    friend class CommandLineInterfaceReceiverWorkerThread;
};

}

#endif // COMMANDLINEINTERFACERECEIVEROBJECTMANAGER_H
