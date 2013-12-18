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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#include <iostream>
#include <strings.h>
#include <stdlib.h>
#include <fstream>
#include <string>

#include "DcmCore/DcmToolKit.h"
#include "sysmod/syscomm.h"
#include "sysmod/syscfg.h"
#include "WaveResourceIds.h"

using namespace std;

int readConfigFile (string &configFileName, string &contents)
{
    ifstream in(configFileName.c_str (), std::ios::in);
    if (in)
    {
        string tmp;
        in.seekg(0, std::ios::end);
        tmp.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&tmp[0], tmp.size());
        in.close();
        contents = tmp;
        return 0;
    }
    else
    {
        cout << "Failed to read the file : "<<configFileName<<endl;
        return -1;
    }
}
void displayUsage (const char *pProgramName)
{
    cout << "USAGE:" << endl;
    cout << pProgramName << "<-cfgfile <File Path> >";
    cout << endl;
    cout << endl;
    cout << "Command Line Options:" << endl;
    cout << endl;
}

int main (int argc, char *argv[])
{
    string              configurationFileName;
    
    if (3 != argc)
    {
        displayUsage (argv[0]);
        return (1);
    }
    if (0 == (strncmp ("-cfgfile", argv[1], sizeof ("-cfgfile"))))
    {
         configurationFileName = argv[2];
    }
    else
    {
        displayUsage (argv[0]);
        return (1);
    }

    string configs;
    int    rc = readConfigFile (configurationFileName, configs);
    if (-1 == rc)
    {
        cout<<"readConfigFile error"<<endl;
        return (1);
    }

    WaveNs::initializeWaveResourceIds ();
    ResourceId status = DcmNs::DcmToolKit::pushConfigToKernelOnLocalMM (configs);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        cout << " pushConfigToKernel error with ResourceId : "<<status;
        return (1);
    }
    
    return (0);
}
