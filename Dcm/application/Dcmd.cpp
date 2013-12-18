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

#include "Framework/Core/Wave.h"
#include "Framework/Utils/Integer.h"

using namespace std;
using namespace WaveNs;

extern "C" int dcmMain (int argc, char *argv[], int argc1, char *argv1[]);

int g_commandLineArgumentLength = 1024;

unsigned long int g_numberOfCpus = 12;

void displayUsage (const char *pProgramName)
{
    int    programNameLength = strlen (pProgramName);
    int    i                 = 0;
    string blankProgramName;

    for (i = 0; i < programNameLength; i++)
    {
        blankProgramName += ' ';
    }

    cout << endl;
    cout << "USAGE:" << endl;
    cout << endl;
    cout << pProgramName     << " [-daemon <true/false>]" << endl;
    cout << blankProgramName << " [-type <0/1/2>]" << endl;
    cout << blankProgramName << " [-syslog <true/false>]" << endl;
    cout << blankProgramName << " [-port <port number>]" << endl;
    cout << blankProgramName << " [-enabledb <true/false>]" << endl;
    cout << blankProgramName << " [-enabledblog <true/false>]" << endl;
    cout << blankProgramName << " [-dbsharedbuffers <buffer size>]" << endl;
    cout << blankProgramName << " [-dbport <database port number>]" << endl;
    cout << blankProgramName << " [-bpipaddress <bp backplane ipaddress>]" << endl;
    cout << blankProgramName << " [-cpipaddress <cp backplane ipaddress>]" << endl;
    cout << blankProgramName << " [-fgwiport <FOS Gateway Interface Port Number>]" << endl;
    cout << endl;
    cout << endl;
    cout << "Command Line Options:" << endl;
    cout << endl;
    cout << "    -daemon            : true                              - run the program in daemon mode" << endl;
    cout << "                       : false                             - run the program in shell  mode" << endl;
    cout << endl;
    cout << "    -syslog            : true                              - enable logging trace messages to sys log apart from logging to the .trc file" << endl;
    cout << "                       : false                             - log the trace messages only to .trc file" << endl;
    cout << endl;
    cout << "    -port              : port number                       - the TCP/IP port number on which the server socket for this program listens to accept incoming connections" << endl;
    cout << endl;
    cout << "    -enabledb          : true                              - persist application configuration to database" << endl;
    cout << "                       : false                             - do not persist application configuration to database" << endl;
    cout << endl;
    cout << "    -enabledblog       : true                              - enable database server logging." << endl;
    cout << "                       : false                             - do not enable database server logging." << endl;
    cout << endl;
    cout << "    -dbsharedbuffers   : database shared buffer size       - the database shared buffer size" << endl;
    cout << endl;
    cout << "    -dbport            : databse port number               - the TCP/IP port number on which the databse accepts connections" << endl;
    cout << endl;
    cout << "    -mgmtport          : Management Interface port number  - the TCP/IP port number for Management Client connections" << endl;
    cout << endl;
    cout << "    -eth               : Ethernet Interface                - the Ethernet Interface to be used" << endl;
    cout << endl;
    cout << "    -clientport        : Client Receiver Port              - the TCP/IP Port to be used for Client Receiver side functionality" << endl;
    cout << endl;
    cout << "    -chdir             : Directory Path                    - the directory to which this process will move to.  This will be the Current Working Directory for the process." << endl;
    cout << endl;
}

int main (int argc, char *argv[])
{
    char *pCommandLineArguments[64];
    char *pValCommandLineArguments[16];
    int   i;
    int   numberOfCommandLineArguments    = 31;
    int   numberOfValCommandLineArguments = 10;

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
    strcpy (pCommandLineArguments[9],  "9710");
    strcpy (pCommandLineArguments[10], "");
    strcpy (pCommandLineArguments[11], "true");
    strcpy (pCommandLineArguments[12], "2301");
    strcpy (pCommandLineArguments[13], "3516");                                     // haPort
    strcpy (pCommandLineArguments[14], "3516");                                     // haPeerPort
    strcpy (pCommandLineArguments[15], "true");                                     // enabledblog
    strcpy (pCommandLineArguments[16], "128MB");                                    // dbsharedbuffers
    strcpy (pCommandLineArguments[17], "true");                                     // handledbcorruption
    strcpy (pCommandLineArguments[18], "");                                         // dbbackupfileonfwdl
    strcpy (pCommandLineArguments[19], "yang");                                     // YinPath
    strcpy (pCommandLineArguments[20], "DcmUserTags.wyser");                        // YinPath
    strcpy (pCommandLineArguments[21], "1101");                                     // CLI Receiver Port
    strcpy (pCommandLineArguments[22], "wyserea_cli");                 // Config file management client path
    strcpy (pCommandLineArguments[23], "/fabos/libexec/ConfigCmd.Linux.powerpc");   // Config file management ConfigCmd path
    strcpy (pCommandLineArguments[24], "/var/config/vcs/scripts");                  // Config file management scripts directory
    strcpy (pCommandLineArguments[25], "0");                                        // Firmware download case detail           
    strcpy (pCommandLineArguments[26], "false");                                    // isabruptreboot
    strcpy (pCommandLineArguments[27], "auto");                                     //DB empty Type 
    strcpy (pCommandLineArguments[28], "75000");                                    //DB empty type threshold
    strcpy (pCommandLineArguments[29], "0");                                        // Ha id
    strcpy (pCommandLineArguments[30], "normal");

    strcpy (pValCommandLineArguments[0],  "");
    strcpy (pValCommandLineArguments[1],  "");
    strcpy (pValCommandLineArguments[2],  "127.0.0.1");
    strcpy (pValCommandLineArguments[3],  ((Integer (4567)).toString ()).c_str ());
    strcpy (pValCommandLineArguments[4],  "");
    strcpy (pValCommandLineArguments[5],  "false");  // ipot
    strcpy (pValCommandLineArguments[6],  "true");   // fcPrincipal
    strcpy (pValCommandLineArguments[7],  "false");  // usefs
    strcpy (pValCommandLineArguments[8],  "DCMCCM");
    strcpy (pValCommandLineArguments[9],  "0");      // swbd

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
        else if (0 == strcasecmp ("-fwdlcase",  argv[i]))
        {
            strcpy (pCommandLineArguments[25], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-isabruptreboot",  argv[i]))
        {
            strcpy (pCommandLineArguments[26], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-databaseemptytype",  argv[i]))
        {
            strcpy (pCommandLineArguments[27], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-databaseemptyautodetectionthresholdvalue",  argv[i]))
        {
            strcpy (pCommandLineArguments[28], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-haid",  argv[i]))
        {
            strcpy (pCommandLineArguments[29], argv[i + 1]);
        }
        else if (0 == strcasecmp ("-startmode",  argv[i]))
        {
            strcpy (pCommandLineArguments[30], argv[i + 1]);
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

    Wave::enableAllFeatures ();

    return (dcmMain (numberOfCommandLineArguments, pCommandLineArguments, numberOfValCommandLineArguments, pValCommandLineArguments));
}
