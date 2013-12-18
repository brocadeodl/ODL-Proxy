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

#include "Shell/WaveCliWyserShell.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include <stdlib.h>

namespace WaveNs
{

WaveCliWyserShell::WaveCliWyserShell (WaveClientSynchronousConnection &connection)
    : WaveCliShell ("wyser", connection)
{
    addCommandfunction ("rebuildYangInterfaces", reinterpret_cast<WaveShellCommandFunction> (&WaveCliWyserShell::rebuildYangInterfaces),  "rebuild yang interfaces", NULL);
}

WaveCliWyserShell::~WaveCliWyserShell ()
{
}

void WaveCliWyserShell::briefHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Wyser shell provides basic control to run wyser based internal");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        commands in Wave based application.  For more information on");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        configuring Wyser, please type help in Wyser shell");
}

void WaveCliWyserShell::wyserCreateHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : ");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

ResourceId WaveCliWyserShell::rebuildYangInterfaces (const vector<string> &arguments)
{
    ResourceId                      status;
    WaveClientSynchronousConnection  connection  = getConnection ();

    status = connection.rebuildYangInterfaces ();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "DcmCliDebugShell::rebuildYangInterfaces : Rebuilding Yang Interfaces Failed with error status : " + FrameworkToolKit::localize (status));
    }

    return (WAVE_MESSAGE_ERROR);
}

}
