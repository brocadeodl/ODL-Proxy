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

#ifndef COMMANDLINEINTERFACEWORKERMESSAGE_H
#define COMMANDLINEINTERFACEWORKERMESSAGE_H

#include "Framework/Types/Types.h"

namespace WaveNs
{

class CommandLineInterfaceMessage;
class CommandLineInterfaceWorkerSendContext;

class CommandLineInterfaceWorkerMessage
{
    private :
    protected :
    public :
                                                 CommandLineInterfaceWorkerMessage           (CommandLineInterfaceMessage           *pCommandLineInterfaceMessage);
                                                 CommandLineInterfaceWorkerMessage           (CommandLineInterfaceWorkerSendContext *pCommandLineInterfaceWorkerSendContext);
                                                ~CommandLineInterfaceWorkerMessage           ();

        WaveCommandLineInterfaceWorkerOperation  getWaveCommandLineInterfaceWorkerOperation  ();
        CommandLineInterfaceMessage             *getPCommandLineInterfaceMessage             ();
        void                                     setPCommandLineInterfaceMessage             (CommandLineInterfaceMessage *pCommandLineInterfaceMessage);
        CommandLineInterfaceWorkerSendContext   *getPCommandLineInterfaceWorkerSenderContext ();
        void                                     setPCommandLineInterfaceWorkerSenderContext (CommandLineInterfaceWorkerSendContext *pCommandLineInterfaceWorkerSendContext);

        // Now the data members

    private :
        WaveCommandLineInterfaceWorkerOperation  m_commandLineInterfaceWorkerOperation;
        CommandLineInterfaceMessage             *m_pcommandLineInterfaceMessage;
        CommandLineInterfaceWorkerSendContext   *m_pCommandLineInterfaceWorkerSendContext;

    protected :
    public :
};

}

#endif // COMMANDLINEINTERFACEWORKERMESSAGE_H
