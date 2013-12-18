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

#ifndef SHELLBASE_H
#define SHELLBASE_H

#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Utils/WaveLineEditor/WaveLineEditor.h"
#include "ShellTypes.h"
#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

class ShellBase;
class ShellCommandHandler;

typedef UI32 (*ShellCmdFunction)   (UI32 argc, vector<string> argv);
typedef UI32 (*ShellUsageFunction) ();

class ShellBase : public WaveWorker
{
    private :
        void    getArguments (string *cmdString, vector<string> &argv, UI32 *argc);

    protected :
    public :
        void    shellExecuteHandler    (void);
                ShellBase              (WaveObjectManager *pWaveObjectManager, string shellName);
               ~ShellBase              ();
        void    setShellCommandHandler (vector<ShellCommandHandler>    *shellCommandHandler);

        string  getShellName           () const;

        void    addShellCommandHandler (const ShellCommandHandler &shellCommandHandler);

    // Now the data members

    private :
        string                      m_shellName;
        vector<ShellCommandHandler> m_shellCommandHandlers;
        WaveLineEditor              m_waveLineEditor;

    protected :
    public :

};

class ShellCommandHandler
{

    private :
    protected :
    public :
                                ShellCommandHandler (string action, UI32 minArgc, ShellCmdFunction cmdFunction, UI32 minCmdLength, ShellUsageFunction usageFunction);
                                ShellCommandHandler ();
                               ~ShellCommandHandler ();
        string                  getAction           () const;
        UI32                    getMinArgc          () const;
        ShellCmdFunction        getCmdFunction      () const;
        ShellUsageFunction      getUsageFunction    () const;
        UI32                    getMinCmdLength     () const;
        ShellCommandHandler     &operator =         (const ShellCommandHandler &rhs);

    // Now the data members

    private :
        string                  m_action;
        UI32                    m_minArgc;
        ShellCmdFunction        m_cmdFunction;
        UI32                    m_minCmdLength;
        ShellUsageFunction      m_usageFunction;

    protected :
    public :

};


}
#endif //SHELLBASE_H
