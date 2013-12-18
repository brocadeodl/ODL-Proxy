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

#ifndef SHELLWYSER_H
#define SHELLWYSER_H

#include "Shell/ShellBase.h"
#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{
extern "C" void addWyserFunction (ShellCmdFunction wyserFunction, const char *cliName);

class ShellWyser : public ShellBase
{
    private :
    protected :
    public :
                     ShellWyser             (WaveObjectManager *pWaveObjectManager);
                    ~ShellWyser             ();
        static UI32  shellExecuteHelpWyser  (UI32 argc, vector<string> argv);
        static UI32  shellExecuteQuitWyser  (UI32 argc, vector<string> argv);

        static void  usageShellWyserHelp   (void);
        static void  usageShellWyserQuit   (void);
        static void  registerWyserFunction (ShellCmdFunction wyserFunction, const char *cliName);

    private :
    protected :
    public :

};

}

#endif //SHELLDEBUG_H
