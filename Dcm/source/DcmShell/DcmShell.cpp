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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "DcmShell/DcmShell.h"
#include "Shell/ShellPrism.h"
#include "Shell/ShellObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmShell/DcmDebugShell.h"

#include <unistd.h>
#include <stdlib.h>

using namespace std;

namespace DcmNs
{

DcmShell *s_pDcmShell = NULL;

DcmShell::DcmShell (WaveObjectManager *pWaveObjectManager)
    : ShellBase (pWaveObjectManager, string ("DCM"))
{
    s_pDcmShell = this;

    addShellCommandHandler (ShellCommandHandler (string("quit"),  1, (ShellCmdFunction) (&DcmShell::quit),                 1, (ShellUsageFunction) (NULL)));

    addShellCommandHandler (ShellCommandHandler (string("debug"), 1, (ShellCmdFunction) (&DcmShell::executeDcmDebugShell), 1, (ShellUsageFunction) (&DcmShell::usageDcmDebugShell)));
}

DcmShell::~DcmShell ()
{
}

UI32 DcmShell::executeThisShell (UI32 argc, string argv[])
{
    static DcmShell *pDcmShell = new DcmShell (ShellObjectManager::getInstance ());

    pDcmShell->shellExecuteHandler ();

    return (SHELL_OK);
}

void DcmShell::usageExecuteThisShell ()
{
}

void DcmShell::addToWaveShell ()
{
    (ShellObjectManager::getInstance ())->m_pPrismShell->addShellCommandHandler (ShellCommandHandler (string ("DCM"), 1, (ShellCmdFunction) (&DcmShell::executeThisShell), 1, (ShellUsageFunction) (&DcmShell::usageExecuteThisShell)));
}

UI32 DcmShell::executeDcmDebugShell (UI32 argc, string argv[])
{
    DcmDebugShell::executeThisShell (argc, argv);

    return (SHELL_OK);
}

void DcmShell::usageDcmDebugShell ()
{
}

UI32 DcmShell::quit (UI32 argc, string argv[])
{
    return (SHELL_EXIT);
}


}
