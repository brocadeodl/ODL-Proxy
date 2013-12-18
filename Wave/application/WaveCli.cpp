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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include <iostream>
#include <strings.h>

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Shell/WaveCliShell.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/Test/ManagementInterfaceClientTestMessages.h"
#include "ManagementInterface/ClientInterface/Test/ManagementInterfaceClientTestTypes.h"
#include "ManagementInterface/ClientInterface/WaveClientReceiverObjectManager.h"
#include "WaveCliReplyThread.h"

using namespace std;
using namespace WaveNs;

extern "C" int waveClientMain (int argc, char *argv[], bool wait);

int                              g_commandLineArgumentLength = 512;
WaveClientSynchronousConnection *g_pConnection               = NULL;

void genericMessageHandler (void *pPayLoad, UI32 payLoadLength, void *pPrismContext);

void displayUsage (const char *pProgramName)
{
    cout << "USAGE:" << endl;
    cout << pProgramName << "[-serverip <Server IP Address>] [-serverport <Server TCP/IP Port>] [Command Options]" << endl;
    cout << endl;
    cout << endl;
    cout << "Command Line Options:" << endl;
    cout << endl;
    cout << "    -serverip       : Server IP Address     - IP Address of the Wave Server to which the CLI connects to the Server." << endl;
    cout << endl;
    cout << "    -serverport     : Server TCP/IP Address - The TCP/IP Port Number at which the CLI connects to the Server." << endl;
    cout << endl;
    cout << "    Command Options : CLI Commands          - The CLI commands to be interpreted by the CLI Shell." << endl;
    cout << endl;
}

ManagementInterfaceMessage *clientManagementInterfaceMessageInstantiator (const UI32 &operationCode)
{
    ManagementInterfaceMessage *pManagementInterfaceMessage = NULL;

    switch (operationCode)
    {
        case 0:
            pManagementInterfaceMessage = new ManagementInterfaceClientTestMessage1 ();
            break;

        case 1:
            pManagementInterfaceMessage = new ManagementInterfaceClientTestMessage2 ();
            break;

        case 2:
            pManagementInterfaceMessage = new ManagementInterfaceClientTestMessage3 ();
            break;

        default:
            trace (TRACE_LEVEL_ERROR, string ("clientManagementInterfaceMessageInstantiator : Unknown Operation Code : ") + operationCode);
            pManagementInterfaceMessage = NULL;
    }

    return (pManagementInterfaceMessage);
}

ResourceId clientManagementInterfaceMessageHandler (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    static UI32 managementInterfaceMessageCounter = 0;
    UI32        operationCode;

    managementInterfaceMessageCounter++;

    if (0 == managementInterfaceMessageCounter % 1000)
    {
        trace (TRACE_LEVEL_INFO, string ("Number of Management Interface Messages Received so far from the server : ") + managementInterfaceMessageCounter);
    }

    operationCode = pManagementInterfaceMessage->getOperationCode ();

    if (2 == operationCode)
    {
        WaveCliReplyThread *pWaveCliReplyThread = new WaveCliReplyThread (pManagementInterfaceMessage);

        WaveThreadStatus status = pWaveCliReplyThread->run ();

        if (WAVE_THREAD_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "clientManagementInterfaceMessageHandler : A new thread COULD NOT BE CREATED to start the action");
            delete pWaveCliReplyThread;
            return (WAVE_MESSAGE_ERROR);
        }
 
        return (WAVE_MGMT_CLIENT_INTF_CLIENT_ASYNC_REPSONSE);
    } 
    else 
    {
        return (WAVE_MESSAGE_SUCCESS);
    }
}

int main (int argc, char *argv[])
{
    char   *pCommandLineArguments[8];
    int     i;
    int     numberOfCommandLineArguments = 8;
    string  serverIpAddress              = FrameworkToolKit::getAPublicIpAddressForThisMachine ();
    UI32    serverPort                   = 9110;
    const int     serverIpCharArray_Length = 64;
    char    serverIpCharArray[serverIpCharArray_Length];
    string  command;

    for (i = 0; i < numberOfCommandLineArguments; i++)
    {
        pCommandLineArguments[i] = new char[g_commandLineArgumentLength+1];
        memset(pCommandLineArguments[i], '\0', (g_commandLineArgumentLength+1));
    }

    strncpy (pCommandLineArguments[0], argv[0], g_commandLineArgumentLength );
    strncpy (pCommandLineArguments[1], "false", g_commandLineArgumentLength );
    strncpy (pCommandLineArguments[2], "0", g_commandLineArgumentLength );
    strncpy (pCommandLineArguments[3], "false", g_commandLineArgumentLength );
    strncpy (pCommandLineArguments[4], "3016", g_commandLineArgumentLength );
    strncpy (pCommandLineArguments[5], "false", g_commandLineArgumentLength );
    strncpy (pCommandLineArguments[6], "9013", g_commandLineArgumentLength );
    strncpy (pCommandLineArguments[7], "9810", g_commandLineArgumentLength  );

    for (i = 1; i < argc; i += 1)
    {
        if (0 == strcasecmp ("-serverip", argv[i]))
        {
            if (argc > (i + 1))
            {
                strncpy (serverIpCharArray, argv[i + 1], serverIpCharArray_Length);
                serverIpAddress = serverIpCharArray;
                i++;
            }
        }
        else if (0 == strcasecmp ("-serverport", argv[i]))
        {
            if (argc > (i + 1))
            {
                serverPort = atoi (argv[i + 1]);
                i++;
            }
        }
        else if (0 == strcasecmp ("-clientport", argv[i]))
        {
            if (argc > (i + 1))
            {
                strncpy (pCommandLineArguments[7], argv[i + 1], g_commandLineArgumentLength);
                i++;
            }
        }
        else
        {
            command += argv[i];
            command += " ";
        }
    }

    WaveClientReceiverObjectManager::setAutoScanForAvailablePort (true);
    WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (clientManagementInterfaceMessageInstantiator);
    WaveUserInterfaceObjectManager::setManagementInterfaceMessageHandlerForClient      (clientManagementInterfaceMessageHandler);

    waveClientMain (numberOfCommandLineArguments, pCommandLineArguments, false);

    g_pConnection = new WaveClientSynchronousConnection ("Wave Client", serverIpAddress, serverPort);

    WaveConnectionStatus           connectionstatus                      = WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER;
    vector<string>                 tokens;
    string                         ipAddress                             = FrameworkToolKit::getAPublicIpAddressForThisMachine ();
    UI32                           managementInterfaceClientReceiverPort = FrameworkToolKit::getManagementInterfaceClientReceiverPort ();
    string                         clientNameExtension                   = ipAddress + managementInterfaceClientReceiverPort;

    connectionstatus = g_pConnection->connect (clientNameExtension);

    if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER == connectionstatus)
    {
        WaveCliShell *pWaveCliShell = WaveCliShell::getInstance (*g_pConnection);

        tokenize (command, tokens);
        pWaveCliShell->execute (tokens);
        delete pWaveCliShell;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Could not establish connection to Wave Server");
    }

    g_pConnection->close ();
    delete g_pConnection;
    g_pConnection = NULL;

    return (0);
}
