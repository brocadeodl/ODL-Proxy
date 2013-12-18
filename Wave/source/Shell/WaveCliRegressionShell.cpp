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

#include "Shell/WaveCliRegressionShell.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

WaveCliRegressionShell::WaveCliRegressionShell (WaveClientSynchronousConnection &connection)
    : WaveCliShell ("regression", connection)
{
    addCommandfunction ("list", reinterpret_cast<WaveShellCommandFunction> (&WaveCliRegressionShell::regressionList),           "Lists the available Services.",                                                    reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliRegressionShell::regressionListHelp));
    addCommandfunction ("prepare2", reinterpret_cast<WaveShellCommandFunction> (&WaveCliRegressionShell::regressionPrepare2),   "Prepares a Regression Service by allowing test parameters to be set.",             reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliRegressionShell::regressionPrepare2Help));
    addCommandfunction ("run",  reinterpret_cast<WaveShellCommandFunction> (&WaveCliRegressionShell::regressionRunForAService), "Runs a particular Regression Service irrespective of its enabled/disabled state.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliRegressionShell::regressionRunForAServiceHelp));
}

WaveCliRegressionShell::~WaveCliRegressionShell ()
{
}

void WaveCliRegressionShell::briefHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Regression shell provides basic control to execute BIST");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        in Wave based application.  For more information on");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        controlling BIST services please type help in regression shell");
}

ResourceId WaveCliRegressionShell::regressionList (const vector<string> &arguments)
{
    vector<RegressionTestEntry>     testServiceEntries;
    ResourceId                      status;
    char                            indexArray[64];
    WaveClientSynchronousConnection connection          = getConnection ();

    status = connection.getListOfTestServices (testServiceEntries);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        UI32                numberOfTestServices = testServiceEntries.size ();
        UI32                j                    = 0;
        RegressionTestEntry tempEntry;

        tracePrintf (TRACE_LEVEL_INFO, true, true, "Total Number Of Test Services : %u", numberOfTestServices);
        tracePrintf (TRACE_LEVEL_INFO, true, true, "");
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Index    State    Service Name");
        tracePrintf (TRACE_LEVEL_INFO, true, true, "_______  _______  ____________");

        for (j = 0; j < numberOfTestServices; j++)
        {
            tempEntry = testServiceEntries[j];

            snprintf (indexArray, 64, "    %03d", j + 1);

            tracePrintf (TRACE_LEVEL_INFO, true, true, tempEntry.getIsTestEnabled () ? "%s  Enabled  %s" : "%s Disabled %s", indexArray, (connection.getServiceNameById (tempEntry.getTestServiceId ())).c_str ());
        }

        tracePrintf (TRACE_LEVEL_INFO, true, true, "______________________________");
        tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Obtain the list of Test Services. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliRegressionShell::regressionListHelp ()
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

ResourceId WaveCliRegressionShell::regressionPrepare2 (const vector<string> &arguments)
{
    UI32                            numberOfArguments   = arguments.size ();
    vector<RegressionTestEntry>     testServiceEntries;
    ResourceId                      status;
    PrismServiceId                  serviceId           = 0;
    UI32                            serviceIndex        = 0;
    WaveClientSynchronousConnection connection          = getConnection ();
    vector<string>                  inputStrings        = arguments;

    if (numberOfArguments < 1)
    {
        return (WAVE_MESSAGE_ERROR);
    }
    else
    {
        if (1 <= numberOfArguments)
        {
            serviceIndex = strtoul (arguments[0].c_str (), NULL, 10);
    
            // Remove service index from string vector
            inputStrings.erase(inputStrings.begin());
        }
    }

    status = connection.getListOfTestServices (testServiceEntries);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_DEBUG, "Successfully Obtained the list of Test Services.");

        UI32                numberOfTestServices = testServiceEntries.size ();
        RegressionTestEntry tempEntry;

        if (serviceIndex <= numberOfTestServices)
        {
            tempEntry = testServiceEntries[serviceIndex - 1];
            serviceId = tempEntry.getTestServiceId ();

            status = connection.prepareTestForAService (serviceId, inputStrings);

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_SUCCESS, "Prepare Regression Test Succeeded.");
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "Prepare Regression Test Failed.  Status : " + FrameworkToolKit::localize (status));
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "Service Index Out of range.");
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Obtain the list of Test Services. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliRegressionShell::regressionPrepare2Help ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : prepare2 <index> [-parameterKey1 <parameterValue1] ...");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    index       - The index of the BIST service to be run.");
}

ResourceId WaveCliRegressionShell::regressionRunForAService (const vector<string> &arguments)
{
    UI32                            numberOfArguments   = arguments.size ();
    vector<RegressionTestEntry>     testServiceEntries;
    ResourceId                      status;
    PrismServiceId                  serviceId           = 0;
    UI32                            serviceIndex        = 0;
    UI32                            numberOfIterations  = 1;
    WaveClientSynchronousConnection connection          = getConnection ();

    if (numberOfArguments < 1)
    {
        return (WAVE_MESSAGE_ERROR);
    }
    else
    {
        if (1 <= numberOfArguments)
        {
            serviceIndex = strtoul (arguments[0].c_str (), NULL, 10);
        }

        if (2 <= numberOfArguments)
        {
            numberOfIterations = strtoul (arguments[1].c_str (), NULL, 10);
        }
    }

    status = connection.getListOfTestServices (testServiceEntries);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_DEBUG, "Successfully Obtained the list of Test Services.");

        UI32                numberOfTestServices = testServiceEntries.size ();
        RegressionTestEntry tempEntry;

        if (serviceIndex <= numberOfTestServices)
        {
            tempEntry = testServiceEntries[serviceIndex - 1];
            serviceId = tempEntry.getTestServiceId ();

            status = connection.runTestForAService (serviceId, numberOfIterations);

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_SUCCESS, "Regression Test Succeeded.");
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "Regression Test Failed.  Status : " + FrameworkToolKit::localize (status));
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "Service Index Out of range.");
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Obtain the list of Test Services. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliRegressionShell::regressionRunForAServiceHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : run <index> [nIterations]");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    index       - The index of the BIST service to be run.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                  This value can be obtained via list command.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                  Please note that this value is different from coresponding Wave Service Id.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    nIterations - Number of iterations for which the BIST is to be run.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                  This is in an optional argument.  The default value is 1.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Runs the particual BIST Service identified by the index for nIterations times.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

}
