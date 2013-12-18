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

#include "Shell/WaveCliShowShell.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include <stdlib.h>

namespace WaveNs
{

WaveCliShowShell::WaveCliShowShell (WaveClientSynchronousConnection &connection)
    : WaveCliShell ("show", connection)
{
    addCommandfunction ("running-config", reinterpret_cast<WaveShellCommandFunction> (&WaveCliShowShell::showRunningConfiguration), "Displays Running Configuration.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliShowShell::showRunningConfigurationHelp));
    addCommandfunction ("qyprunning-config", reinterpret_cast<WaveShellCommandFunction> (&WaveCliShowShell::showRunningConfigurationByQualifiedYangPath), "Displays Running Configuration using Qualified YANG Path.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliShowShell::showRunningConfigurationByQualifiedYangPathHelp));
}

WaveCliShowShell::~WaveCliShowShell ()
{
}

void WaveCliShowShell::briefHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Displays Currently active configuration.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        You can specify the name of a ManagedObject Class as input.");
}

ResourceId WaveCliShowShell::showRunningConfiguration (const vector<string> &arguments)
{
    ResourceId                      status                 = WAVE_MESSAGE_ERROR;
    WaveClientSynchronousConnection connection             = getConnection ();
    UI32                            numberOfManagedObjects = 0;
    vector<string>                  managedObjects;

    if (1 > (arguments.size ()))
    {
        showRunningConfigurationHelp ();

        return (status);
    }

    status = connection.showManagedObjects (arguments[1], "WaveCurrent", numberOfManagedObjects, managedObjects);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, string ("WaveCliShowShell::showRunningConfiguration : Number Of Managed Objects : ") + numberOfManagedObjects );

        UI32 i = 0;

        for (i = 0; i < numberOfManagedObjects; i++)
        {
            //tracePrintf (TRACE_LEVEL_INFO, true, true, "%s", (managedObjects[i]).c_str ());
            cout << managedObjects[i] << endl;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Obtain the Managed Objects. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliShowShell::showRunningConfigurationHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : list");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Dsiplays the Managed Objects.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    ");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    The following fields are listed:");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

ResourceId WaveCliShowShell::showRunningConfigurationByQualifiedYangPath (const vector<string> &arguments)
{
    ResourceId                      status                 = WAVE_MESSAGE_ERROR;
    WaveClientSynchronousConnection connection             = getConnection ();

    if (1 > (arguments.size ()))
    {
        showRunningConfigurationHelp ();

        return (status);
    }

    status = connection.showConfigurationByQualifiedYangPath (arguments[1], "WaveCurrent");

    if (WAVE_MESSAGE_SUCCESS == status)
    {
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Obtain the Managed Objects. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliShowShell::showRunningConfigurationByQualifiedYangPathHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : list");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Dsiplays the Managed Objects.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    ");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    The following fields are listed:");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

}
