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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "Shell/ShellWyser.h"
#include "ShellTypes.h"
#include <string>
#include "Shell/ShellObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

ShellWyser::ShellWyser (WaveObjectManager *pWaveObjectManager)
    :ShellBase (pWaveObjectManager, string("Wyser"))
{
    addShellCommandHandler (ShellCommandHandler (string("regression"), 1,  (ShellCmdFunction) (&ShellPrism::shellExecuteRegressionService), 1, (ShellUsageFunction) (&ShellPrism::usageShellPrismRegressionService)));
    addShellCommandHandler (ShellCommandHandler (string("trace"),      1,  (ShellCmdFunction) (&ShellPrism::shellExecuteTraceService),      1, (ShellUsageFunction) (&ShellPrism::usageShellPrismTraceService)));
    addShellCommandHandler (ShellCommandHandler (string("help"),       1,  (ShellCmdFunction) (&ShellWyser::shellExecuteHelpWyser),         1, (ShellUsageFunction) (&ShellWyser::usageShellWyserHelp)));
    addShellCommandHandler (ShellCommandHandler (string("quit"),       1,  (ShellCmdFunction) (&ShellWyser::shellExecuteQuitWyser),         1, (ShellUsageFunction) (&ShellWyser::usageShellWyserQuit)));
}

ShellWyser::~ShellWyser ()
{
}

UI32 ShellWyser::shellExecuteHelpWyser (UI32 argc, vector<string> argv)
{
    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellWyser::shellExecuteHelpWyser : Entering ...");

    usageShellWyserHelp ();

    return SHELL_OK;
}

UI32 ShellWyser::shellExecuteQuitWyser (UI32 argc, vector<string> argv)
{
    return SHELL_EXIT;
}

void ShellWyser::usageShellWyserQuit (void)
{
    cout << "quit  <Quit debug shell>" << endl;
}


void ShellWyser::usageShellWyserHelp (void)
{
    cout<<"quit  <Quit Cluster shell>" << endl;
    cout<<"help  <Help for Cluster shell>"<<endl;
}


void ShellWyser::registerWyserFunction (ShellCmdFunction wyserFunction, const char *cliName)
{
    static  PrismMutex mutex;

    mutex.lock();
    ((ShellObjectManager::getInstance ())->m_pPrismShell->getWyserShell ())->addShellCommandHandler (ShellCommandHandler (string(cliName), 1,  wyserFunction, strlen (cliName), NULL));
    mutex.unlock();
}

extern "C" void addWyserFunction (ShellCmdFunction wyserFunction, const char *cliName)
{
        ShellWyser::registerWyserFunction (wyserFunction, cliName);
}

}
