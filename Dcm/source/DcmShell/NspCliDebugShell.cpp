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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "DcmShell/NspCliDebugShell.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include <stdlib.h>
#include <string.h>

namespace NspNs
{

NspCliDebugShell::NspCliDebugShell (NspClientSynchronousConnection &connection)
    : WaveCliShell                     ("nspdebug", connection),
      m_nspClientSynchronousConnection (connection)
{
    addCommandfunction ("createnetwork", reinterpret_cast<WaveShellCommandFunction> (&NspCliDebugShell::createNetwork), "Creates a given network in VCS",       reinterpret_cast<WaveShellCommandHelpFunction> (&NspCliDebugShell::createNetworkHelp));
}

NspCliDebugShell::~NspCliDebugShell ()
{
}

NspClientSynchronousConnection &NspCliDebugShell::getNspClientSynchronousConnection ()
{
    return (m_nspClientSynchronousConnection);
}

void NspCliDebugShell::briefHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Debug shell provides a way to execute debug actions.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Developer can execute pre-registered actions from this shell.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Debug actions that are not particularly part of any other shells");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        typically land here.");
}

ResourceId NspCliDebugShell::createNetwork (const vector<string> &arguments)
{
    ResourceId                     status;
    NspClientSynchronousConnection connection   = getNspClientSynchronousConnection ();
    string                         networkName;

    if (1 != arguments.size ())
    {
        createNetworkHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    networkName = arguments[0];

    status = connection.createNetwork (networkName);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Create Network : " + networkName);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Network. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void NspCliDebugShell::createNetworkHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : createnetwork");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Creates a Network in VCS with the given name.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    name - Name of the new network.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

}
