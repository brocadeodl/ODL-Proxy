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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVESHELL_H
#define WAVESHELL_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/WaveLineEditor/WaveLineEditor.h"

#include <string>
#include <map>
#include <vector>

using namespace std;

namespace WaveNs
{

class WaveShell;

typedef ResourceId (WaveShell::* WaveShellCommandFunction)     (const vector<string> &arguments);
typedef void       (WaveShell::* WaveShellCommandHelpFunction) ();

class WaveShell
{
    private :
        virtual void briefHelp   ();
                void overAllHelp (const vector<string> &arguments);
                void help        (const string &command);

    protected :
        void runCommand      (const vector<string> &arguments, const bool &exitWhenDone);
        bool isAKnownCommand (const string &command);

    public :
                        WaveShell          (const string &shellName);

        virtual        ~WaveShell          ();

                string  getShellName       () const;

                void    addCommandfunction (const string &command, WaveShellCommandFunction commandFunction, const string &commandBriefHelp, WaveShellCommandHelpFunction commandHelpFunction = NULL, WaveShell *pCommandFunctionOwner = NULL);
                void    addSubShell        (WaveShell *pSubshell);

                void    execute            (const vector<string> &arguments);

    // Now the data members

    private :
        string                                    m_shellName;
        map<string, WaveShell *>                  m_subShells;
        map<string, WaveShellCommandFunction>     m_commandFunctions;
        map<string, WaveShellCommandHelpFunction> m_commandHelpFunctions;
        map<string, string>                       m_commandFunctionBriefDescriptions;
        map<string, WaveShell *>                  m_commandFunctionOwners;

        WaveLineEditor                            m_waveLineEditor;

    protected :
    public :
};

}

#endif // WAVESHELL_H
