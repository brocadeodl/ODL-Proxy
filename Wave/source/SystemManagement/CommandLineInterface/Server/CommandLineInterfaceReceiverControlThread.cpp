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

#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceReceiverControlThread.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceReceiverObjectManager.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceReceiverWorkerThread.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FixedSizeBuffer.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/Base64Utils.h"
#include "Framework/Utils/PamUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "SystemManagement/SystemManagementToolKit.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceMessage.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceWorkerMessage.h"

namespace WaveNs
{

CommandLineInterfaceReceiverControlThread::CommandLineInterfaceReceiverControlThread (CommandLineInterfaceReceiverObjectManager *pCommandLineInterfaceReceiverObjectManager, ServerStreamingSocket *pServerStreamingSocket)
    : PrismPosixThread                             (),
      WaveWorker                                   (pCommandLineInterfaceReceiverObjectManager),
      m_pServerStreamingSocket                     (pServerStreamingSocket),
      m_transferredSocketcontrol                   (false),
      m_pCommandLineInterfaceReceiverObjectManager (pCommandLineInterfaceReceiverObjectManager),
      m_pCommandLineInterfaceReceiverWorkerThread  (NULL)
{
}

CommandLineInterfaceReceiverControlThread::~CommandLineInterfaceReceiverControlThread ()
{
    if (false == m_transferredSocketcontrol)
    {
        delete m_pServerStreamingSocket;
    }
}

WaveThreadStatus CommandLineInterfaceReceiverControlThread::start()
{
    trace (TRACE_LEVEL_DEVEL, "CommandLineInterfaceReceiverControlThread::start : Starting ...");

    prismAssert (NULL != m_pServerStreamingSocket, __FILE__, __LINE__);

    bool isAuthorizationSuccessful = authorize ();

    if (false == isAuthorizationSuccessful)
    {
        trace (TRACE_LEVEL_WARN, "CommandLineInterfaceReceiverControlThread::start : A rogue client is trying to connect with us.");
    }
    else
    {
        m_transferredSocketcontrol = true;

        m_pCommandLineInterfaceReceiverWorkerThread = new CommandLineInterfaceReceiverWorkerThread (m_pCommandLineInterfaceReceiverObjectManager, m_pServerStreamingSocket);

        if (NULL == m_pCommandLineInterfaceReceiverWorkerThread)
        {
            m_transferredSocketcontrol = false;
        }
        else
        {
            m_pCommandLineInterfaceReceiverWorkerThread->run ();

            while (true)
            {
                bool isSuccessful = false;
                UI32 messageSize  = 0;

                isSuccessful = (*m_pServerStreamingSocket) >> messageSize;

                if (true == isSuccessful)
                {
                    FixedSizeBuffer messageBuffer (messageSize);

                    isSuccessful = (*m_pServerStreamingSocket) >> messageBuffer;

                    if (true == isSuccessful)
                    {
                        string                       messageString;
                        CommandLineInterfaceMessage *pCommandLineInterfaceMessage = new CommandLineInterfaceMessage;

                        prismAssert (NULL != pCommandLineInterfaceMessage, __FILE__, __LINE__);

                        CommandLineInterfaceWorkerMessage *pCommandLineInterfaceWorkerMessage = new CommandLineInterfaceWorkerMessage (pCommandLineInterfaceMessage);

                        prismAssert (NULL != pCommandLineInterfaceWorkerMessage, __FILE__, __LINE__);

                        messageBuffer.toString (messageString);
                        pCommandLineInterfaceMessage->loadFromSerializedData2 (messageString);

                        m_pCommandLineInterfaceReceiverWorkerThread->submitCommandLineInterfaceWorkerMessage (pCommandLineInterfaceWorkerMessage);
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    delete this;

    return (WAVE_THREAD_SUCCESS);
}

bool CommandLineInterfaceReceiverControlThread::authorize ()
{
    // Try to read the Prism connection Pass Phrase.  If the connected client does not supply the
    // proper pass phrase, then we know it is a rogue client.  We must drop the client.

    bool            isSuccessful          = false;
    string          wavesecretHelloString = SystemManagementToolKit::getHelloSecret ();
    FixedSizeBuffer secretBuffer          (wavesecretHelloString.size ());

    isSuccessful = (*m_pServerStreamingSocket) >> (secretBuffer);

    if ((false == isSuccessful) || (0 == (secretBuffer.getCurrentSize ())))
    {
        trace (TRACE_LEVEL_DEBUG, "CommandLineInterfaceReceiverControlThread::authorize : A client could not supply proper credentials.  Ignoring client.");
    }
    else
    {
        string helloSecretGivenByClient;

        secretBuffer.toString (helloSecretGivenByClient);

        if (helloSecretGivenByClient != wavesecretHelloString)
        {
            isSuccessful = false;

            trace (TRACE_LEVEL_DEBUG, "CommandLineInterfaceReceiverControlThread::authorize : A client supplied improper credentials.  Ignoring client.");
        }
    }

    return (isSuccessful);
}

}
