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
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Shell/WaveCliShell.h"
#include "DcmShell/DcmCliDebugShell.h"
#include "DcmShell/NspCliDebugShell.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/Test/ManagementInterfaceClientTestMessages.h"
#include "ManagementInterface/ClientInterface/Test/ManagementInterfaceClientTestTypes.h"
#include "ManagementInterface/ClientInterface/WaveClientReceiverObjectManager.h"
#include "Framework/../../application/WaveCliReplyThread.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;
using namespace NspNs;

extern "C" int dcmClientMain (const char * const pProgramName, const int clientPort);

void displayUsage (const char *pProgramName)
{
    cout << "USAGE:" << endl;
    cout << pProgramName << " [-serverip <Server IP Address>] [-serverport <Server TCP/IP Port Number>]" << endl;
    cout << endl;
    cout << endl;
    cout << "Command Line Options:" << endl;
    cout << endl;
    cout << "    -serverip   : Server IP Address         - The IP Address where NSPd is running" << endl;
    cout << endl;
    cout << "    -serverport : Server TCP/IP Port Number - Management port number of NSPd instance to connect to" << endl;
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
           UI32 operationCode;

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
    string  serverIpAddress              = FrameworkToolKit::getAPublicIpAddressForThisMachine ();
    UI32    serverPort                   = 8110;
    UI32    i                            = 0;
    char    serverIpCharArray[64];
    string  clientIpAddress;
    char    clientIpCharArray[64];
    UI32    numberOfCommandLineArguments = (UI32) argc;
    SI32    clientPort                   = 9810;
    SI32    maxNumberOfRetries           = 15;
    string  command;
    string  ipAddress;

    for (i = 1; i < numberOfCommandLineArguments; i++)
    {
        if (0 == strcasecmp ("-serverip", argv[i]))
        {
            strcpy (serverIpCharArray, argv[i + 1]);
            serverIpAddress = serverIpCharArray;

            i++;
        }
        else if (0 == strcasecmp ("-serverport", argv[i]))
        {
            serverPort = atoi (argv[i + 1]);

            i++;
        }
        else if (0 == strcasecmp ("-clientip", argv[i]))
        {
            strcpy (clientIpCharArray, argv[i + 1]);
            clientIpAddress = clientIpCharArray;

            i++;
        }
        else if (0 == strcasecmp ("-clientport", argv[i]))
        {
            clientPort = atoi (argv[i + 1]);

            i++;
        }
        else if (0 == strcasecmp ("-connectionretries", argv[i]))
        {
            maxNumberOfRetries = atoi (argv[i + 1]);

            i++;
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

    dcmClientMain (argv[0], clientPort);
    
    if (0 == clientIpAddress.size())
    {
        ipAddress = FrameworkToolKit::getAPublicIpAddressForThisMachine ();
    }
    else
    {
        ipAddress = clientIpAddress; 
    }

    NspClientSynchronousConnection nspConnection                           ("Wave Client", serverIpAddress, serverPort);
    WaveConnectionStatus           connectionstatus                      = WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER;
    UI32                           managementInterfaceClientReceiverPort = FrameworkToolKit::getManagementInterfaceClientReceiverPort ();
    string                         clientNameExtension                   = ipAddress + managementInterfaceClientReceiverPort;
    //ResourceId                     status;

    connectionstatus = nspConnection.connect (clientNameExtension, maxNumberOfRetries);

    if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER == connectionstatus)
    {
        vector<string> tokens;

        WaveCliShell     *pWaveCliShell     = WaveCliShell::getInstance (nspConnection);
        NspCliDebugShell *pNspCliDebugshell = new NspCliDebugShell (nspConnection);

        prismAssert (NULL != pWaveCliShell,     __FILE__, __LINE__);
        prismAssert (NULL != pNspCliDebugshell, __FILE__, __LINE__);

        pWaveCliShell->addSubShell (pNspCliDebugshell);

        tokenize (command, tokens);
        pWaveCliShell->execute (tokens);
        delete pWaveCliShell;

        nspConnection.close ();
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Could not establish connection to NSP Server.");

        return (1);
    }

    return (0);
}
