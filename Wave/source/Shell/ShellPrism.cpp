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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/
#include "Shell/ShellObjectManager.h"
#include "Shell/ShellBase.h"
#include "Shell/ShellPrism.h"

#include <string>

using namespace std;

namespace WaveNs
{

ShellPrism::ShellPrism (WaveObjectManager *pWaveObjectManager)
    :ShellBase (pWaveObjectManager, string("Wave"))
{
    addShellCommandHandler (ShellCommandHandler (string("regression"), 1,  (ShellCmdFunction) (&ShellPrism::shellExecuteRegressionService), 1, (ShellUsageFunction) (&ShellPrism::usageShellPrismRegressionService)));
    addShellCommandHandler (ShellCommandHandler (string("trace"),      1,  (ShellCmdFunction) (&ShellPrism::shellExecuteTraceService),      1, (ShellUsageFunction) (&ShellPrism::usageShellPrismTraceService)));
    addShellCommandHandler (ShellCommandHandler (string("debug"),      1,  (ShellCmdFunction) (&ShellPrism::shellExecuteDebugService),      1, (ShellUsageFunction) (&ShellPrism::usageShellPrismDebugService)));
    addShellCommandHandler (ShellCommandHandler (string("cluster"),    1,  (ShellCmdFunction) (&ShellPrism::shellExecuteClusterService),    1, (ShellUsageFunction) (&ShellPrism::usageShellPrismClusterService)));
    addShellCommandHandler (ShellCommandHandler (string("wyser"),      1,  (ShellCmdFunction) (&ShellPrism::shellExecuteWyserService),      1, (ShellUsageFunction) (&ShellPrism::usageShellPrismWyserService)));
    addShellCommandHandler (ShellCommandHandler (string("load"),       2,  (ShellCmdFunction) (&ShellPrism::shellExecuteLoadScript),        1, (ShellUsageFunction) (&ShellPrism::usageShellPrismLoadScript)));
    addShellCommandHandler (ShellCommandHandler (string("help"),       1,  (ShellCmdFunction) (&ShellPrism::shellExecuteHelp),              1, (ShellUsageFunction) (&ShellPrism::usageShellPrismHelp)));
    addShellCommandHandler (ShellCommandHandler (string("QUIT"),       1,  (ShellCmdFunction) (&ShellPrism::shellExecuteQuit),              1, NULL));

    m_pRegressionShell  =   new ShellRegression    (pWaveObjectManager);
    m_pDebugShell       =   new ShellDebug         (pWaveObjectManager);
    m_pTraceShell       =   new ShellTrace         (pWaveObjectManager);
    m_pClusterShell     =   new ShellCluster       (pWaveObjectManager);
    m_pWyserShell       =   new ShellWyser         (pWaveObjectManager);
}


ShellPrism::~ShellPrism ()
{

    if (NULL != m_pRegressionShell)
    {
        delete m_pRegressionShell;
    }

    if (NULL != m_pDebugShell)
    {
        delete m_pDebugShell;
    }

    if (NULL != m_pTraceShell)
    {
        delete m_pTraceShell;
    }

    if (NULL != m_pClusterShell) 
    {
	    delete m_pClusterShell;
    }
    
    if (NULL != m_pWyserShell) 
    {
	    delete m_pWyserShell;
    }
}

ShellRegression *ShellPrism::getRegressionShell ()
{
    return (m_pRegressionShell);
}

ShellDebug *ShellPrism::getDebugShell ()
{
    return (m_pDebugShell);
}

ShellTrace *ShellPrism::getTraceShell ()
{
    return (m_pTraceShell);
}

ShellCluster *ShellPrism::getClusterShell()
{
    return (m_pClusterShell);
}

ShellWyser *ShellPrism::getWyserShell()
{
    return (m_pWyserShell);
}

UI32 ShellPrism::shellExecuteRegressionService (UI32 argc, vector<string> argv)
{
    shellTrace (TRACE_LEVEL_DEVEL, "ShellPrism::shellExecuteRegressionService : Entering ...");

    (ShellObjectManager::getInstance ())->m_pPrismShell->m_pRegressionShell->shellGetServicesList ();
    (ShellObjectManager::getInstance ())->m_pPrismShell->m_pRegressionShell->shellExecuteHandler ();
    return SHELL_OK;
}

UI32 ShellPrism::shellExecuteTraceService (UI32 argc, vector<string> argv)
{
    shellTrace (TRACE_LEVEL_DEVEL, "ShellPrism::shellExecuteTraceService : Entering ...");

    (ShellObjectManager::getInstance ())->m_pPrismShell->m_pTraceShell->shellGetServicesList ();
    (ShellObjectManager::getInstance ())->m_pPrismShell->m_pTraceShell->shellExecuteHandler ();
    return SHELL_OK;
}

UI32 ShellPrism::shellExecuteDebugService (UI32 argc, vector<string> argv)
{
    shellTrace (TRACE_LEVEL_DEVEL, "ShellPrism::shellExecuteDebugService : Entering ...");

    (ShellObjectManager::getInstance ())->m_pPrismShell->m_pDebugShell->shellExecuteHandler ();
    return SHELL_OK;
}

UI32 ShellPrism::shellExecuteClusterService (UI32 argc, vector<string> argv)
{
    shellTrace (TRACE_LEVEL_DEVEL, "ShellPrism::shellExecuteClusterService : Entering ...");

    (ShellObjectManager::getInstance ())->m_pPrismShell->m_pClusterShell->shellExecuteHandler ();
    return SHELL_OK;
}

UI32 ShellPrism::shellExecuteWyserService (UI32 argc, vector<string> argv)
{
    shellTrace (TRACE_LEVEL_DEVEL, "ShellPrism::shellExecuteWyserService : Entering ...");

    (ShellObjectManager::getInstance ())->m_pPrismShell->m_pWyserShell->shellExecuteHandler ();
    return SHELL_OK;
}

UI32 ShellPrism::shellExecuteLoadScript (UI32 argc, vector<string> argv)
{
    (ShellObjectManager::getInstance ())->m_inputSource.open (argv [1].c_str (), ifstream::in);
    return SHELL_OK;
}

UI32 ShellPrism::shellExecuteHelp (UI32 argc, vector<string> argv)
{
    shellTrace (TRACE_LEVEL_DEVEL, "ShellPrism::shellExecuteHelp : Entering ...");

    cout << endl << "Size of Prism Msg: " << sizeof(PrismMessage) << endl;
    usageShellPrismHelp ();
    return SHELL_OK;
}

UI32 ShellPrism::shellExecuteQuit (UI32 argc, vector<string> argv)
{
    shellTrace (TRACE_LEVEL_DEVEL, "ShellPrism::shellExecuteQuit : Entering ...");
    exit (0);
}

void ShellPrism::usageShellPrismRegressionService ()
{
    cout << "regression                 <Start regression shell>" << endl;
}

void ShellPrism::usageShellPrismTraceService ()
{
    cout << "trace                      <Start trace shell>" << endl;
}

void ShellPrism::usageShellPrismDebugService ()
{
    cout << "debug                      <Start debug shell>" << endl;
}

void ShellPrism::usageShellPrismClusterService ()
{
    cout<<"cluster                     <Start cluster shell>"<< endl;
}

void ShellPrism::usageShellPrismWyserService ()
{
    cout<<"wyser                       <Start wyser shell>"<< endl;
}

void ShellPrism::usageShellPrismLoadScript ()
{
    cout << "load <filename>            <Load Script>" << endl;
}

void ShellPrism::usageShellPrismHelp ()
{
    usageShellPrismRegressionService ();
    usageShellPrismTraceService ();
    usageShellPrismDebugService ();
    usageShellPrismLoadScript ();
    usageShellPrismClusterService ();
    usageShellPrismWyserService ();
    cout << "help                       <Help for prism shell>" << endl;
}


WaveMessageStatus ShellPrism::shellSendSynchronously (PrismMessage *pPrismMessage)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->sendSynchronously (pPrismMessage));
}

void  ShellPrism::shellTrace (TraceLevel shellTraceLevel, const string &stringToTrace)
{
    return (ShellObjectManager::getInstance ())->m_pPrismShell->trace (shellTraceLevel, stringToTrace);
}

}
