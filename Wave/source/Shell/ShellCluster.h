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

#ifndef SHELLCLUSTER_H
#define SHELLCLUSTER_H

#include "Shell/ShellBase.h"
#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

//
// registers the cli command handler against the cliname 
//
extern "C" void addClusterFunction (ShellCmdFunction clusterFunction, const char *cliName);


///Class:
///ShellCluster
///Description:
/// This Class represents the Cluster Subshell
/// It provides the interface fro cluster CLIs
class ShellCluster : public ShellBase
{
    private :
    protected :
    public :
                     ShellCluster            (WaveObjectManager *pWaveObjectManager);
                    ~ShellCluster            ();
        static UI32  shellExecuteHelpCluster (UI32 argc, vector<string> argv);
        static UI32  shellExecuteQuitCluster (UI32 argc, vector<string> argv);

        static void  usageShellClusterHelp   (void);
        static void  usageShellClusterQuit   (void);
        static void  registerClusterFunction (ShellCmdFunction cliFunction, const char *cliName);

    private :
    protected :
    public :

};

}

#endif //SHELLCLUSTER_H
