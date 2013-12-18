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

#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceWorkerMessage.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceMessage.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceWorkerSendContext.h"

namespace WaveNs
{

CommandLineInterfaceWorkerMessage::CommandLineInterfaceWorkerMessage (CommandLineInterfaceMessage *pCommandLineInterfaceMessage)
    : m_commandLineInterfaceWorkerOperation    (WAVE_CLI_WORKER_OPERATION_USER_REQUEST),
      m_pcommandLineInterfaceMessage           (pCommandLineInterfaceMessage),
      m_pCommandLineInterfaceWorkerSendContext (NULL)
{
}

CommandLineInterfaceWorkerMessage::CommandLineInterfaceWorkerMessage (CommandLineInterfaceWorkerSendContext *pCommandLineInterfaceWorkerSendContext)
    : m_commandLineInterfaceWorkerOperation    (WAVE_CLI_WORKER_OPERATION_SEND_CALLBACK),
      m_pcommandLineInterfaceMessage           (NULL),
      m_pCommandLineInterfaceWorkerSendContext (pCommandLineInterfaceWorkerSendContext)
{
}

CommandLineInterfaceWorkerMessage::~CommandLineInterfaceWorkerMessage ()
{
    if (NULL != m_pcommandLineInterfaceMessage)
    {
        delete m_pcommandLineInterfaceMessage;
    }

    if (NULL != m_pCommandLineInterfaceWorkerSendContext)
    {
        delete m_pCommandLineInterfaceWorkerSendContext;
    }
}

WaveCommandLineInterfaceWorkerOperation CommandLineInterfaceWorkerMessage::getWaveCommandLineInterfaceWorkerOperation ()
{
    return (m_commandLineInterfaceWorkerOperation);
}

CommandLineInterfaceMessage *CommandLineInterfaceWorkerMessage::getPCommandLineInterfaceMessage ()
{
    return (m_pcommandLineInterfaceMessage);
}

void CommandLineInterfaceWorkerMessage::setPCommandLineInterfaceMessage (CommandLineInterfaceMessage *pCommandLineInterfaceMessage)
{
    m_pcommandLineInterfaceMessage = pCommandLineInterfaceMessage;
}

CommandLineInterfaceWorkerSendContext *CommandLineInterfaceWorkerMessage::getPCommandLineInterfaceWorkerSenderContext ()
{
    return (m_pCommandLineInterfaceWorkerSendContext);
}

void CommandLineInterfaceWorkerMessage::setPCommandLineInterfaceWorkerSenderContext (CommandLineInterfaceWorkerSendContext *pCommandLineInterfaceWorkerSendContext)
{
    m_pCommandLineInterfaceWorkerSendContext = pCommandLineInterfaceWorkerSendContext;
}

}
