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

#include "DcmShell/DcmDebugShell.h"
#include "Shell/ShellPrism.h"
#include "Shell/ShellObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include <unistd.h>
#include <stdlib.h>

using namespace std;

namespace DcmNs
{

DcmDebugShell::DcmDebugShell (WaveObjectManager *pWaveObjectManager)
    : ShellBase (pWaveObjectManager, string ("DCM-Debug"))
{
    addShellCommandHandler (ShellCommandHandler (string ("quit"),                1, (ShellCmdFunction) (&DcmDebugShell::quit),                1, (ShellUsageFunction) (NULL)));

    addShellCommandHandler (ShellCommandHandler (string ("testoperationaldata"), 1, (ShellCmdFunction) (&DcmDebugShell::testOperationalData), 1, (ShellUsageFunction) (usageTestOperationalData)));
}

DcmDebugShell::~DcmDebugShell ()
{
}

UI32 DcmDebugShell::executeThisShell (UI32 argc, string argv[])
{
    static DcmDebugShell *pDcmDebugShell = new DcmDebugShell (ShellObjectManager::getInstance ());

    pDcmDebugShell->shellExecuteHandler ();

    return (SHELL_OK);
}

UI32 DcmDebugShell::quit (UI32 argc, string argv[])
{
    return (SHELL_EXIT);
}

UI32 DcmDebugShell::testOperationalData (UI32 argc, string argv[])
{
    return (SHELL_OK);
}

void DcmDebugShell::usageTestOperationalData ()
{
}

}
