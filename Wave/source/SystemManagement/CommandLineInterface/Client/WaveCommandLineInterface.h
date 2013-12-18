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

#ifndef WAVECOMMANDLINEINTERFACE_H
#define WAVECOMMANDLINEINTERFACE_H

#include "Framework/Types/Types.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceMessage.h"
#include "SystemManagement/CommandLineInterface/Client/CommandLineInterfaceEntry.h"

#include <signal.h>

namespace WaveNs
{

class ClientStreamingSocket;
class YangUserInterface;
class YangElement;

class WaveCommandLineInterface
{
    private :
    protected :
    public :
                                          WaveCommandLineInterface                                (const string &serverIpAddress, const SI32 &serverPort);
                                         ~WaveCommandLineInterface                                ();

               void                       connectToServer                                         ();
               void                       reconnectToServer                                       ();

               void                       sendHelloSecretToServer                                 ();

               void                       post                                                    (CommandLineInterfaceMessage &commandLineInterfaceMessage);
               bool                       receive                                                 (CommandLineInterfaceMessage &commandLineInterfaceMessage);
               ResourceId                 processCommand                                          (CommandLineInterfaceMessage &commandLineInterfaceMessage);

               void                       acceptCommands                                          ();

               void                       displayTopLevelPrompt                                   ();

               void                       initializeCommandLineEntries                            ();
               CommandLineInterfaceEntry *getCommandLineInterfaceEntryForShowRunningConfiguration (const string &commandLine);

        static void                       sigIntHandler                                           (int signal);
        static void                       sigSegvHandler                                          (int signal);
               void                       installSigIntHandlers                                   () const;

        static WaveCommandLineInterface   *getInstance                                            ();
        static vector<YangElement *>       getYangElementsByTargetNodeName                        (const string &targetNodeName);

               CommandLineInterfaceEntry  *getPCurrentModecommandLineInterfaceEntry               ();

               vector<string>             &getCommandHistory                                      ();
               UI32                       &getCommandHistoryCurrentIndex                          ();
               string                     &getCurrentlyTypingCommand                              ();

        static CommandLineInterfaceEntry  *getPCurrentModeCommandLineInterfaceEntry               ();

        // Now the data members

    private :
               string                                    m_serverIpAddress;
               SI32                                      m_serverPort;
               WaveCommandLineInterfaceConnectionStatus  m_connectionStatus;
               ClientStreamingSocket                    *m_pClientStreamingSocket;
               string                                    m_thisMachineIpAddress;
               string                                    m_thisMachineHostName;

               YangUserInterface                        *m_pYangUserInterface;

               CommandLineInterfaceEntry                 m_commandLineinterfaceEntries;
               CommandLineInterfaceEntry                *m_pCurrentModeCommandLineInterfacEntry;

        static WaveCommandLineInterface                 *m_pWaveCommandLineInterface;

    protected :
    public :
};

}

#endif // WAVECOMMANDLINEINTERFACE_H
