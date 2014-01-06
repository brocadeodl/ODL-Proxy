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

#ifndef SHELLDEBUG_H
#define SHELLDEBUG_H

#include "Shell/ShellBase.h"
#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{
extern "C" void addDebugFunction (ShellCmdFunction debugFunction, const char *debugName);

class ShellDebug : public ShellBase
{
    private :
    protected :
    public :
                     ShellDebug            (WaveObjectManager *pWaveObjectManager);
                    ~ShellDebug            ();
        static UI32  shellExecuteHelpDebug (UI32 argc, vector<string> argv);
        static UI32  shellExecuteQuitDebug (UI32 argc, vector<string> argv);

        static void  usageShellDebugHelp   (void);
        static void  usageShellDebugQuit   (void);
        static void  registerDebugFunction (ShellCmdFunction debugFunction, const char *debugName);

    private :
    protected :
    public :

};

}

#endif //SHELLDEBUG_H