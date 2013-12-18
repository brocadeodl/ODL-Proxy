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
 *   Author : Vidyasagara Reddy Guntaka, Jayanth Venkataraman              *
 ***************************************************************************/

#include "SystemManagement/CommandLineInterface/Client/WaveCommandLineInterface.h"
#include "Framework/Utils/FrameworkToolKit.h"

using namespace WaveNs;

void displayUsage (const char *pProgramName)
{
    cout << "USAGE:" << endl;
    cout << pProgramName << endl;
    cout << "    -cliport <port number> - Cli port number of DCMd instance to connect to" << endl;
    cout << endl;
}

int main (int argc, char *argv[])
{
    UI32    cliPortNumber   = FrameworkToolKit::FrameworkToolKit::getCommandLineInterfaceReceiverPort ();
    string  cliIpAddress    = "127.0.0.1";

    // Get the command line options
    for (SI32 i=1; i < argc; i++)
    {
        if ((0 == strcasecmp ("-cliport", argv[i])))
        {
            if ((i+1) < argc)
            {
                cliPortNumber = atoi (argv[i+1]);
            }
            i++;
        }
        else
        {
            displayUsage (argv[0]);
            return (1);
        }
    }

    WaveCommandLineInterface waveCommandLineInterface (cliIpAddress, cliPortNumber);

    return (0);
}
