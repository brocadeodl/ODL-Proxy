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

#include "Shell/ShellObjectManager.h"
#include "Shell/ShellTrace.h"
#include "ShellTypes.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/Integer.h"
#include <string>

namespace WaveNs
{

ShellTrace::ShellTrace (WaveObjectManager *pWaveObjectManager)
    :ShellBase (pWaveObjectManager, string("Trace")), m_nServices (0)
    {
    addShellCommandHandler (ShellCommandHandler (string("regression"), 1, (ShellCmdFunction) (&ShellPrism::shellExecuteRegressionService),       1, (ShellUsageFunction) (&ShellPrism::usageShellPrismRegressionService)));
    addShellCommandHandler (ShellCommandHandler (string("debug"),      1, (ShellCmdFunction) (&ShellPrism::shellExecuteDebugService),             1, (ShellUsageFunction) (&ShellPrism::usageShellPrismDebugService)));
    addShellCommandHandler (ShellCommandHandler (string("list"),       1, (ShellCmdFunction) (&ShellTrace::shellExecuteListServices),             1, (ShellUsageFunction) (&ShellTrace::usageShellTraceList)));
    addShellCommandHandler (ShellCommandHandler (string("setlevel"),   3, (ShellCmdFunction) (&ShellTrace::shellExecuteSetTraceLevel),            4, (ShellUsageFunction) (&ShellTrace::usageShellTraceSetLevel)));
    addShellCommandHandler (ShellCommandHandler (string("setall"),     2, (ShellCmdFunction) (&ShellTrace::shellExecuteSetAllServicesTraceLevel), 4, (ShellUsageFunction) (&ShellTrace::usageShellTraceSetAllServicesTraceLevel)));
    addShellCommandHandler (ShellCommandHandler (string("setdefault"), 1, (ShellCmdFunction) (&ShellTrace::shellExecuteSetDefaultLevels),         4, (ShellUsageFunction) (&ShellTrace::usageShellTraceSetDefaultLevels)));
    addShellCommandHandler (ShellCommandHandler (string("legend"),     1, (ShellCmdFunction) (&ShellTrace::shellExecuteLegend),                   4, (ShellUsageFunction) (&ShellTrace::usageShellLegend)));
    addShellCommandHandler (ShellCommandHandler (string("help"),       1, (ShellCmdFunction) (&ShellTrace::shellExecuteHelpTrace),                1, (ShellUsageFunction) (&ShellTrace::usageShellTraceHelp)));
    addShellCommandHandler (ShellCommandHandler (string("quit"),       1, (ShellCmdFunction) (&ShellTrace::shellExecuteQuitTrace),                1, (ShellUsageFunction) (&ShellTrace::usageShellTraceQuit)));

}

ShellTrace::~ShellTrace ()
{
}

UI32 ShellTrace::shellExecuteListServices (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getTraceShell ()->listServices (argc, argv));
}

UI32 ShellTrace::listServices (UI32 argc, vector<string> argv)
{
    vector<TraceClientId> traceClientIdsVector;
    vector<TraceLevel>    traceLevelsVector;
    UI32                  nServices;
    UI32                  index;
    string                serviceName;
    string                traceLevelName;

    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellTrace::shellExecuteListServices : Entering ...");

    TraceObjectManager::getClientsInformationDirectly (traceClientIdsVector, traceLevelsVector);

    nServices = traceClientIdsVector.size ();

    cout << "Service #: " << nServices << endl;

    for (index = 0; index < nServices; index++)
    {
        TraceClientId traceClientId = traceClientIdsVector[index];
        TraceLevel    traceLevel    = traceLevelsVector[index];

        serviceName    = FrameworkToolKit::getTraceClientNameById (traceClientId);
        traceLevelName = FrameworkToolKit::localize (traceLevel);

        printf ("%3d - %-40s: %d : %s\n", index, serviceName.c_str (),  traceLevel, traceLevelName.c_str ());
    }

    return SHELL_OK;
}

void ShellTrace::shellGetServicesList (void)
{
    vector<TraceClientId> traceClientIdsVector;
    vector<TraceLevel>    traceLevelsVector;
    UI32                  index;

    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellTrace::shellGetServicesList : Entering ...");

    TraceObjectManager::getClientsInformationDirectly (traceClientIdsVector, traceLevelsVector);

    m_nServices = traceClientIdsVector.size ();

    for (index = 0; index < m_nServices ;index++)
    {
        TraceClientId traceClientId = traceClientIdsVector[index];
        m_serviceMap.push_back (traceClientId);
    }
}

UI32 ShellTrace::shellExecuteSetTraceLevel (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getTraceShell ()->setServiceLevel (argc, argv));
}

UI32 ShellTrace::setServiceLevel (UI32 argc, vector<string> argv)
{
    TraceLevel    traceLevel;
    TraceClientId id;
    UI32          serviceIndex;
    ResourceId    status        = WAVE_MESSAGE_ERROR;

    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellTrace::shellExecuteSetTraceLevel : Entering ...");

    serviceIndex    =  atoi (argv[1].c_str ());
    if (serviceIndex > m_nServices)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR,  "invalid service id");
        return SHELL_OK;
    }

    id  =   m_serviceMap [serviceIndex];

    traceLevel  =  (TraceLevel) atoi (argv[2].c_str ());

    status = TraceObjectManager::setClientTraceLevelDirectly (id, traceLevel);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR, string ("Failed to set service level. status: ") + status);
    }
    else
    {
        ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, string ("Succeeded to set service level for service ") + serviceIndex);
    }

    return SHELL_OK;
}

UI32 ShellTrace::shellExecuteSetAllServicesTraceLevel (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getTraceShell ()->setAllServicesLevel (argc, argv));
}

UI32 ShellTrace::setAllServicesLevel (UI32 argc, vector<string> argv)
{
    TraceLevel    traceLevel;
    TraceClientId id;
    UI32          serviceIndex;
    ResourceId    status        = WAVE_MESSAGE_ERROR;

    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellTrace::shellExecuteSetAllServicesTraceLevel : Entering ...");

    traceLevel =  (TraceLevel) atoi (argv[1].c_str ());

    for (serviceIndex = 0; serviceIndex < m_nServices; serviceIndex++)
    {
        id     = m_serviceMap [serviceIndex];
        status = TraceObjectManager::setClientTraceLevelDirectly (id, traceLevel);;

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            ShellPrism::shellTrace (TRACE_LEVEL_ERROR, string ("Failed to set level (service, status): ") + serviceIndex + status);
        }
        else
        {
            ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, string ("Succeeded to set level for service ") + serviceIndex);
        }

    }

    return SHELL_OK;
}


UI32 ShellTrace::shellExecuteSetDefaultLevels (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getTraceShell ()->setDefaultLevels (argc, argv));
}


UI32 ShellTrace::setDefaultLevels (UI32 argc, vector<string> argv)
{
    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellTrace::shellExecuteSetDefaultLevels : Not Supported yet ...");

    return SHELL_OK;
}

UI32 ShellTrace::shellExecuteHelpTrace (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getTraceShell ()->helpTrace (argc, argv));
}


UI32 ShellTrace::helpTrace (UI32 argc, vector<string> argv)
{
    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellTrace::shellExecuteHelpTrace : Entering ...");

    usageShellTraceHelp ();

    return SHELL_OK;
}

UI32 ShellTrace::shellExecuteQuitTrace (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getTraceShell ()->quitTrace (argc, argv));
}


UI32 ShellTrace::quitTrace (UI32 argc, vector<string> argv)
{
//    cout << "shellExecuteQuitTrace" << endl;
    m_serviceMap.clear ();
    return SHELL_EXIT;
}

UI32 ShellTrace::shellExecuteLegend (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getTraceShell ()->legend (argc, argv));
}

UI32 ShellTrace::legend (UI32 argc, vector<string> argv)
{
    TraceObjectManagerSetClientTraceLevelMessage message (getTraceClientId (), TRACE_LEVEL_UNKNOWN);

    WaveMessageStatus status = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    trace (TRACE_LEVEL_UNKNOWN, (Integer (TRACE_LEVEL_UNKNOWN)).toString () + " : This is a message traced at TRACE_LEVEL_UNKNOWN");
    trace (TRACE_LEVEL_DEVEL,   (Integer (TRACE_LEVEL_DEVEL)).toString ()   + " : This is a message traced at TRACE_LEVEL_DEVEL");
    trace (TRACE_LEVEL_DEBUG,   (Integer (TRACE_LEVEL_DEBUG)).toString ()   + " : This is a message traced at TRACE_LEVEL_DEBUG");
    trace (TRACE_LEVEL_INFO,    (Integer (TRACE_LEVEL_INFO)).toString ()    + " : This is a message traced at TRACE_LEVEL_INFO");
    trace (TRACE_LEVEL_WARN,    (Integer (TRACE_LEVEL_WARN)).toString ()    + " : This is a message traced at TRACE_LEVEL_WARN");
    trace (TRACE_LEVEL_ERROR,   (Integer (TRACE_LEVEL_ERROR)).toString ()   + " : This is a message traced at TRACE_LEVEL_ERROR");
    trace (TRACE_LEVEL_FATAL,   (Integer (TRACE_LEVEL_FATAL)).toString ()   + " : This is a message traced at TRACE_LEVEL_FATAL");

    TraceObjectManagerSetClientTraceLevelMessage message1 (getTraceClientId (), TRACE_LEVEL_INFO);

    status = sendSynchronously (&message1);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    return (SHELL_OK);
}

void ShellTrace::usageShellTraceList (void)
{
    cout << "list                                           <List all services and their Trace Levels>" << endl;
}

void ShellTrace::usageShellTraceSetLevel (void)
{
    cout << "setlevel [serviceId] [enable/disable]          <Set service trace level>" << endl;
}

void ShellTrace::usageShellTraceSetAllServicesTraceLevel (void)
{
    cout << "setall [enable/disable]                        <Set the trace level for all services>" << endl;
}

void ShellTrace::usageShellTraceSetDefaultLevels (void)
{
    cout << "setdefault                                     <Set the default trace level for all services>" << endl;
}


void ShellTrace::usageShellTraceQuit (void)
{
    cout << "quit                                           <Quit trace shell>" << endl;
}

void ShellTrace::usageShellLegend (void)
{
    cout << "legend                                         <Display Trace Legend, Useful for identifying  values for trace levels>" << endl;
}

void ShellTrace::usageShellTraceHelp (void)
{
    usageShellTraceList ();
    usageShellTraceSetLevel ();
    usageShellTraceSetAllServicesTraceLevel ();
    usageShellTraceSetDefaultLevels ();
    usageShellLegend ();
    usageShellTraceQuit ();
    cout << "help                                           <Help for trace shell>" << endl;
}

}
