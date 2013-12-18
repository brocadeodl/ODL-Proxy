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

#include <iostream>
#include <strings.h>
#include "App/AppObjectManager.h"

#include "Framework/Core/Wave.h"
#include "Framework/ServiceInterface/ApplicationService.h"
#include "Framework/ServiceInterface/ApplicationLocalService.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/ServiceInterface/Test/PrismNativeTest1Service.h"
#include "Framework/ServiceInterface/ApplicationServiceUtils.h"
#include "Framework/Utils/Integer.h"

using namespace std;
using namespace WaveNs;

extern "C" int testMain1 (int argc, char *argv[], int id);
extern "C" int testMain2 (int argc, char *argv[], int id);
extern "C" int waveSystemManagementMain (int argc, char *argv[]);

int g_commandLineArgumentLength = 512;

void genericMessageHandler (void *pPayLoad, UI32 payLoadLength, void *pPrismContext);

void displayUsage (const char *pProgramName)
{
    cout << "USAGE:" << endl;
    cout << pProgramName << " [-daemon <true/false>] [-type <0/1/2>] [-syslog <true/false>] [-port <port number>] [-enabledb <true/false>] [-dbport <database port number>] [-mgmtport <Management Interface Port Number>] [-chdir <Directory Path>]" << endl;
    cout << endl;
    cout << endl;
    cout << "Command Line Options:" << endl;
    cout << endl;
    cout << "    -daemon     : true                             - run the program in daemon mode" << endl;
    cout << "                : false                            - run the program in shell  mode" << endl;
    cout << endl;
    cout << "    -syslog     : true                             - enable logging trace messages to sys log apart from logging to the .trc file" << endl;
    cout << "                : false                            - log the trace messages only to .trc file" << endl;
    cout << endl;
    cout << "    -port       : port number                      - the TCP/IP port number on which the server socket for this program listens to accept incoming connections" << endl;
    cout << endl;
    cout << "    -enabledb   : true                             - persist application configuration to database" << endl;
    cout << "                : false                            - do not persist application configuration to database" << endl;
    cout << endl;
    cout << "    -dbport     : databse port number              - the TCP/IP port number on which the databse accepts connections" << endl;
    cout << endl;
    cout << "    -mgmtport   : Management Interface port number - the TCP/IP port number for Management Client connections" << endl;
    cout << endl;
    cout << "    -eth        : Ethernet Interface               - the Ethernet Interface to be used" << endl;
    cout << endl;
    cout << "    -clientport : Client Receiver Port             - the TCP/IP Port to be used for Client Receiver side functionality" << endl;
    cout << endl;
    cout << "    -chdir      : Directory Path                   - the directory to which this process will move to.  This will be the Current Working Directory for the process." << endl;
    cout << endl;
}

int main (int argc, char *argv[])
{
    char  *pCommandLineArguments[64];
    char  *pValCommandLineArguments[16];
    int    i;
    int    numberOfCommandLineArguments    = 26;
    int    numberOfValCommandLineArguments = 11;
    string servers;

    for (i = 0; i < numberOfCommandLineArguments; i++)
    {
        pCommandLineArguments[i] = new char[g_commandLineArgumentLength];
    }

    for (i = 0; i < numberOfValCommandLineArguments; i++)
    {
        pValCommandLineArguments[i] = new char[g_commandLineArgumentLength];
    }

    strcpy (pCommandLineArguments[0],  argv[0]);
    strcpy (pCommandLineArguments[1],  "false");
    strcpy (pCommandLineArguments[2],  "0");
    strcpy (pCommandLineArguments[3],  "false");
    strcpy (pCommandLineArguments[4],  "3016");
    strcpy (pCommandLineArguments[5],  "false");
    strcpy (pCommandLineArguments[6],  "9013");
    strcpy (pCommandLineArguments[7],  "9110");
    strcpy (pCommandLineArguments[8],  "");
    strcpy (pCommandLineArguments[9],  "8710");
    strcpy (pCommandLineArguments[10], "");
    strcpy (pCommandLineArguments[11], "false");
    strcpy (pCommandLineArguments[12], "2501");
    strcpy (pCommandLineArguments[13], "3516");                                     // haPort
    strcpy (pCommandLineArguments[14], "3516");                                     // haPeerPort
    strcpy (pCommandLineArguments[15], "true");                                     // enabledblog
    strcpy (pCommandLineArguments[16], "128MB");                                    // dbsharedbuffers
    strcpy (pCommandLineArguments[17], "true");                                     // handledbcorruption
    strcpy (pCommandLineArguments[18], "");                                         // dbbackupfileonfwdl
    strcpy (pCommandLineArguments[19], "yang");                                     // YinPath
    strcpy (pCommandLineArguments[20], "DcmUserTags.wyser");                        // YinPath
    strcpy (pCommandLineArguments[21], "1301");                                     // CLI Receiver Port
    strcpy (pCommandLineArguments[22], "/usr/WyserEa/bin/WyserEaCli");                 // Config file management client path
    strcpy (pCommandLineArguments[23], "/fabos/libexec/ConfigCmd.Linux.powerpc");   // Config file management ConfigCmd path
    strcpy (pCommandLineArguments[24], "/var/config/vcs/scripts");                  // Config file management scripts directory
    strcpy (pCommandLineArguments[25], "");                                         // Multi Database Connections in <daemon>:<IP>:<PORT>; format

    strcpy (pValCommandLineArguments[0],  "");
    strcpy (pValCommandLineArguments[1],  "");
    strcpy (pValCommandLineArguments[2],  "127.0.0.1");
    strcpy (pValCommandLineArguments[3],  ((Integer (4565)).toString ()).c_str ());
    strcpy (pValCommandLineArguments[4],  "");
    strcpy (pValCommandLineArguments[5],  "false");  // ipot
    strcpy (pValCommandLineArguments[6],  "true");   // fcPrincipal
    strcpy (pValCommandLineArguments[7],  "false");  // usefs
    strcpy (pValCommandLineArguments[8],  "DCMCCM");
    strcpy (pValCommandLineArguments[9],  "0");      // swbd
    strcpy (pValCommandLineArguments[10], "false");  // enablecache

    if (2 == argc)
    {
        if (0 == strcasecmp ("-help", argv[1]))
        {
            displayUsage (argv[0]);
            return (0);
        }
    }

    if (1 != (argc % 2))
    {
        displayUsage (argv[0]);
        return (-1);
    }

    for (i = 1; i < argc; i += 2)
    {
        if (0 == strcasecmp ("-daemon", argv[i]))
        {
            strcpy (pCommandLineArguments[1], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-type", argv[i]))
        {
            strcpy (pCommandLineArguments[2], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-syslog", argv[i]))
        {
            strcpy (pCommandLineArguments[3], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-port", argv[i]))
        {
            strcpy (pCommandLineArguments[4], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-enabledb", argv[i]))
        {
            strcpy (pCommandLineArguments[5], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-enabledblog", argv[i]))
        {
            strcpy (pCommandLineArguments[15], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-dbsharedbuffers", argv[i]))
        {
            strcpy (pCommandLineArguments[16], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-dbport", argv[i]))
        {
            strcpy (pCommandLineArguments[6], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-mgmtport", argv[i]))
        {
            strcpy (pCommandLineArguments[7], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-eth", argv[i]))
        {
            strcpy (pCommandLineArguments[8], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-clientport", argv[i]))
        {
            strcpy (pCommandLineArguments[9], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-chdir", argv[i]))
        {
            strcpy (pCommandLineArguments[10], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-detectschemachange", argv[i]))
        {
            strcpy (pCommandLineArguments[11], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-httpport", argv[i]))
        {
            strcpy (pCommandLineArguments[12], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-haport", argv[i]))
        {
            strcpy (pCommandLineArguments[13], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-hapeerport", argv[i]))
        {
            strcpy (pCommandLineArguments[14], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-handledbcorruption", argv[i]))
        {
            strcpy (pCommandLineArguments[17], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-dbdumponfwdl", argv[i]))
        {
            strcpy (pCommandLineArguments[18], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-vcsconfpath", argv[i]))
        {
            strcpy (pValCommandLineArguments[4], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-ipot", argv[i]))
        {
            strcpy (pValCommandLineArguments[5], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-fcprincipal", argv[i]))
        {
            strcpy (pValCommandLineArguments[6], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-usefs", argv[i]))
        {
            strcpy (pValCommandLineArguments[7], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-distributionmode",  argv[i]))
        {
            strcpy (pValCommandLineArguments[8], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-swbd",  argv[i]))
        {
            strcpy (pValCommandLineArguments[9], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-enablecache",  argv[i]))
        {
            strcpy (pValCommandLineArguments[10], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-yinpath",  argv[i]))
        {
            strcpy (pCommandLineArguments[19], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-wysertags",  argv[i]))
        {
            strcpy (pCommandLineArguments[20], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-cliport", argv[i]))
        {
            strcpy (pCommandLineArguments[21], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-configfilemgmtclientpath",  argv[i]))
        {
            strcpy (pCommandLineArguments[22], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-configfilemgmtconfigcmdpath",  argv[i]))
        {
            strcpy (pCommandLineArguments[23], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-configfilemgmtscriptsdir",  argv[i]))
        {
            strcpy (pCommandLineArguments[24], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-server",  argv[i]))
        {
            servers += string (argv[i + 1]) + string (";");
        }
        else
        {
            cerr << endl;
            cerr << "Unknown command line option : \"" << argv[i] << "\"" << endl;
            cerr << endl;
            displayUsage (argv[0]);
            return (-2);
        }
    }

    strcpy (pCommandLineArguments[25], servers.c_str ());

    return (waveSystemManagementMain (numberOfCommandLineArguments, pCommandLineArguments));
}

void genericMessageHandler (void *pPayLoad, UI32 payLoadLength, void *pPrismContext)
{
    cout << "Received Pay Load with Length " << payLoadLength << endl;

    ApplicationServiceUtils::replyToApplicationService (pPayLoad, payLoadLength, pPrismContext);
}
