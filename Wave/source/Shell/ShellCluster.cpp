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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Aashish Akhouri                                                    *
 ***************************************************************************/

#include "Shell/ShellCluster.h"
#include "ShellTypes.h"
#include <string>
#include "Shell/ShellObjectManager.h"


namespace WaveNs
{

/// Name
/// ShellCluster
/// Description
/// Constructor: adds the commandhandler for the basic 
/// facilities: regeression,help,trace,quit
/// Input
/// pWaveObjectManager
/// Output
/// None
/// Return
/// None


ShellCluster::ShellCluster (WaveObjectManager *pWaveObjectManager)
    :ShellBase (pWaveObjectManager, string("Cluster"))
{
    addShellCommandHandler (ShellCommandHandler (string("regression"), 1,  (ShellCmdFunction) (&ShellPrism::shellExecuteRegressionService), 1, (ShellUsageFunction) (&ShellPrism::usageShellPrismRegressionService)));
    addShellCommandHandler (ShellCommandHandler (string("trace"),      1,  (ShellCmdFunction) (&ShellPrism::shellExecuteTraceService),      1, (ShellUsageFunction) (&ShellPrism::usageShellPrismTraceService)));
    addShellCommandHandler (ShellCommandHandler (string("help"),       1,  (ShellCmdFunction) (&ShellCluster::shellExecuteHelpCluster),         1, (ShellUsageFunction) (&ShellCluster::usageShellClusterHelp)));
    addShellCommandHandler (ShellCommandHandler (string("quit"),       1,  (ShellCmdFunction) (&ShellCluster::shellExecuteQuitCluster),         1, (ShellUsageFunction) (&ShellCluster::usageShellClusterQuit)));
}

/// Name
/// ~ShellCluster
/// Description
/// Destructor
/// Input
/// None
///  Output
/// None
/// Return
/// 

ShellCluster::~ShellCluster ()
{
}


/// Name
/// shellExecuteHelpCluster
/// Description
/// Shows the usage syntax of the 
/// CLI commands in the subshell
/// Input:
/// argc : will be 1 here
/// argv: string "help" or its substring
/// Output
/// None
/// Return
/// SHELL_OK

UI32 ShellCluster::shellExecuteHelpCluster (UI32 argc, vector<string> argv)
{
    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellCluster::shellExecuteHelpCluster : Entering ...");

    usageShellClusterHelp ();

    return SHELL_OK;
}

/// Name
/// shellExecuteQuitCluster
/// Description
/// Implements Quit which is used to go back to Wave Shell
/// Input
/// argc : will be 1 here
/// argv: string "quit" or its substring
/// Output
/// None
/// Return
/// EXIT

UI32 ShellCluster::shellExecuteQuitCluster (UI32 argc, vector<string> argv)
{
    return SHELL_EXIT;
}

///Name :
/// usageShellClusterHelp
/// Description :
/// Displays the help menu.

void ShellCluster::usageShellClusterHelp (void)
{
    cout<<"cc secondaryIpAddr1 secondaryPort1 [IpAddr2] [Port2]  <Create Cluster>"<<endl;
    cout<<"addnode nodeIpAddr nodePort                           <Add Node to cluster>"<<endl;
    cout<<"rmnode nodeIpAddr nodePort                            <Remove node from cluster>"<<endl;
    cout<<"rmcluster                                             <Remove Cluster>"<<endl;
    cout<<"getClusterDbgInfo                                     <Get Debug Info for the cluster>"<<endl;
    cout<<"setThisNodeAsPrincipal                                <Set this node as cluster primary>"<<endl;
    cout<<"secFailed                                             <Inform Primary of a lost secondary node>"<<endl;
    cout<<"quit                                                  <Quit Cluster shell>" << endl;
    cout<<"help                                                  <Help for Cluster shell>"<<endl;
}
///Name :
/// usageShellClusterQuit
/// Description :
//

void ShellCluster::usageShellClusterQuit (void)
{
 cout << "quit                                           <Quit Cluster shell>" << endl;
}

/// Name
/// registerClusterFunction
/// Description
/// Adds the command handler for a cli 
/// to the shellcmdhandler in ShellBase(Shellprism)
/// Input
/// clusterFunction: function ptr to the handler
/// cliName: command string
/// Output
/// None
/// Return
/// None

void ShellCluster::registerClusterFunction (ShellCmdFunction clusterFunction, const char *cliName)
{
    static  PrismMutex mutex;

    mutex.lock();
    ((ShellObjectManager::getInstance ())->m_pPrismShell->getClusterShell ())->addShellCommandHandler (ShellCommandHandler (string(cliName), 1,  clusterFunction, strlen (cliName), NULL));
    mutex.unlock();
}

/// Name
/// addClusterFunction
/// Description
///Calls registartion.Used as an API by 
/// other classes to create a new cli 
/// in the Cluster subshell.
/// Input
/// clusterFunction: function ptr to the handler
/// cliName: command string
/// Output
/// None
/// Return
/// None
extern "C" void addClusterFunction (ShellCmdFunction clusterFunction, const char *cliName)
{
    ShellCluster::registerClusterFunction (clusterFunction, cliName);
}

}
