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

#include "Shell/WaveCliServiceShell.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

WaveCliServiceShell::WaveCliServiceShell (WaveClientSynchronousConnection &connection)
    : WaveCliShell ("service", connection)
{
    addCommandfunction ("list",           reinterpret_cast<WaveShellCommandFunction> (&WaveCliServiceShell::serviceList),           "Lists the available Services.",   reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliServiceShell::serviceListHelp));
    addCommandfunction ("setcpuaffinity", reinterpret_cast<WaveShellCommandFunction> (&WaveCliServiceShell::serviceSetCpuAffinity), "Set CPU Affinity for a service.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliServiceShell::serviceSetCpuAffinityHelp));
    addCommandfunction ("listClient", reinterpret_cast<WaveShellCommandFunction> (&WaveCliServiceShell::clientList), "Lists the available Clients.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliServiceShell::clientListHelp));
}

WaveCliServiceShell::~WaveCliServiceShell ()
{
}

void WaveCliServiceShell::briefHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Service shell provides basic control to get information about the Wave based Services running as part of the process.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        It also allows to manipulate certain parameters for the Wave Based Services at run time.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        To get more information on available commands in Service shell, please type help in Service shell.");
}

ResourceId WaveCliServiceShell::serviceList (const vector<string> &arguments)
{
    vector<RegressionTestEntry>     testServiceEntries;
    ResourceId                      status;
    WaveClientSynchronousConnection connection           = getConnection ();
    vector<PrismServiceId>          serviceIds;
    vector<string>                  serviceNames;
    vector<bool>                    enabledStates;
    vector<bool>                    localServiceStates;
    vector<string>                  cpuAffinities;
    UI32                            numberOfServices     = 0;
    UI32                            i                    = 0;
    char                            bufferToTrace[1024];

    status = connection.getServicesInformation (serviceIds, serviceNames, enabledStates, localServiceStates, cpuAffinities);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        numberOfServices = serviceIds.size ();

        snprintf (bufferToTrace, 1024, "%5s   %50s   %10s   %10s   %s", "Index", "Service Name", "State", "Presence", "CPU Affinity");
        tracePrintf (TRACE_LEVEL_SUCCESS, true, true, "%s", bufferToTrace);
        snprintf (bufferToTrace, 1024, "%5s   %50s   %10s   %10s   %s", "_____", "__________________________________________________", "__________", "__________", "____________");
        tracePrintf (TRACE_LEVEL_SUCCESS, true, true, "%s", bufferToTrace);
        
        if (0 == arguments.size())
        {
            for (i = 0; i < numberOfServices; i++)
            {
                snprintf (bufferToTrace, 1024,"%5u   %50s   %10s   %10s   %s", serviceIds[i], (serviceNames[i]).c_str (), enabledStates[i] ? "Enabled" : "Disabled", localServiceStates[i] ? "Local" : "Global", (cpuAffinities[i]).c_str ());
                tracePrintf (TRACE_LEVEL_INFO, true, true, "%s", bufferToTrace);
            }
        }
        else if (1 == arguments.size())
        {
            for (i = 0; i < numberOfServices; i++)
            {
                UI32 serviceId = atoi((arguments[0]).c_str ());
 
                if (serviceIds[i] == serviceId)
                {
                    snprintf (bufferToTrace, 1024,"%5u   %50s   %10s   %10s   %s", serviceIds[i], (serviceNames[i]).c_str (), enabledStates[i] ? "Enabled" : "Disabled", localServiceStates[i] ? "Local" : "Global", (cpuAffinities[i]).c_str ());
                    tracePrintf (TRACE_LEVEL_INFO, true, true, "%s", bufferToTrace);
 
                    break;
                }
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Obtain the list of Test Services. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliServiceShell::serviceListHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : list");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Lists the BIST services present in the Wave based application.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Listing is done in the order that the BIST services are registered with WAVe Framework.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    None.  All arguments are ignored.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    The following fields are listed:");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Index        - A monotonically increasing number to identify a BIST service in the CLI.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                       This index number is different from the Wave Service Id.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        State        - Enabled / Disabled based on the BIST service State");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                       This will make a differnce only when the entire regression is run using start.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                       All of the BIST services in Disabled state are ignored when start CLI is issued.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                       The state of the BIST service will not effect run CLI.  run CLI runs the particular");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                       BIST irrespective of the state.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Service Name - The Name of the Wave BIST Service.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}


ResourceId WaveCliServiceShell::clientList (const vector<string> &arguments)
{       
    ResourceId                      status;
    WaveClientSynchronousConnection connection           = getConnection ();
    vector<string>                  clientNames;
    char                            bufferToTrace[100];
 
    status = connection.getClientInformation (clientNames);
 
    if (WAVE_MESSAGE_SUCCESS == status)
    {
        if (0 != clientNames.size())
        {
            snprintf (bufferToTrace, 100, "%s", "Client Name");
            tracePrintf (TRACE_LEVEL_SUCCESS, true, true, "%s", bufferToTrace);
            snprintf (bufferToTrace, 100, "%s" ,"------------");
            tracePrintf (TRACE_LEVEL_SUCCESS, true, true, "%s", bufferToTrace);

            for (UI32 i = 0; i < clientNames.size(); i++)
            {
                snprintf (bufferToTrace, 100,"%s ", (clientNames[i]).c_str ());
                tracePrintf (TRACE_LEVEL_INFO, true, true, "%s", bufferToTrace);
            }
        }
        else
        {
            snprintf (bufferToTrace, 100, "%s", "No Client Connected");
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Obtain the list of Client. Status : " + FrameworkToolKit::localize (status));
    }
 
    return (status);
}

void WaveCliServiceShell::clientListHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : list");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Lists the backend Clients attached ");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    None.  All arguments are ignored.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Client Name - The Name of the Client attached.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

ResourceId WaveCliServiceShell::serviceSetCpuAffinity (const vector<string> &arguments)
{
    UI32                            numberOfArguments   = arguments.size ();
    PrismServiceId                  prismServiceId      = 0;
    vector<string>                  cpuAffinityStrings;
    ResourceId                      status              = WAVE_MESSAGE_ERROR;
    UI32                            numberOfCpuValues   = 0;
    vector<UI32>                    cpuAffinityVector;
    UI32                            i                   = 0;
    WaveClientSynchronousConnection connection          = getConnection ();

    if (2 > numberOfArguments)
    {
        serviceSetCpuAffinityHelp ();

        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        prismServiceId = strtoul (arguments[0].c_str (), NULL, 10);

        tokenize (arguments[1], cpuAffinityStrings, ':');

        numberOfCpuValues = cpuAffinityStrings.size ();

        for (i = 0; i < numberOfCpuValues; i++)
        {
            cpuAffinityVector.push_back (strtoul (cpuAffinityStrings[i].c_str (), NULL, 10));
        }

        numberOfCpuValues = cpuAffinityVector.size ();

        if (0 != numberOfCpuValues)
        {
            status = connection.setCpuAffinityForService (prismServiceId, cpuAffinityVector);
        }
        else
        {
            tracePrintf (TRACE_LEVEL_ERROR, "WaveCliServiceShell::serviceSetCpuAffinity : No valid CPU Values were found in input.");

            status = WAVE_MESSAGE_ERROR;
        }
    }

    return (status);
}

void WaveCliServiceShell::serviceSetCpuAffinityHelp ()
{
}

}
