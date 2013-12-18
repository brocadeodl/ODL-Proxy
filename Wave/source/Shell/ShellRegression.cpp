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
#include "Shell/ShellRegression.h"
#include "Regression/RegressionServiceMessages.h"
#include "Regression/RegressionPrepareMessage.h"
#include "Regression/RegressionPrepareMessage2.h"
#include "ShellTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include <string>


namespace WaveNs
{

ShellRegression::ShellRegression (WaveObjectManager *pWaveObjectManager)
    : ShellBase (pWaveObjectManager, string("Regression")),
      m_nServices (0)
{
    addShellCommandHandler (ShellCommandHandler (string("trace"),       1,  (ShellCmdFunction) (&ShellPrism::shellExecuteTraceService),             1, (ShellUsageFunction) (&ShellPrism::usageShellPrismTraceService)));
    addShellCommandHandler (ShellCommandHandler (string("debug"),       1,  (ShellCmdFunction) (&ShellPrism::shellExecuteDebugService),             1, (ShellUsageFunction) (&ShellPrism::usageShellPrismDebugService)));

    addShellCommandHandler (ShellCommandHandler (string("list"),        1,  (ShellCmdFunction) (&ShellRegression::shellExecuteListServices),        1, (ShellUsageFunction) (&ShellRegression::usageShellRegressionList)));
    addShellCommandHandler (ShellCommandHandler (string("setstate"),    3,  (ShellCmdFunction) (&ShellRegression::shellExecuteSetServiceState),     4, (ShellUsageFunction) (&ShellRegression::usageShellRegressionSetState)));
    addShellCommandHandler (ShellCommandHandler (string("setallstate"), 2,  (ShellCmdFunction) (&ShellRegression::shellExecuteSetAllServicesState), 4, (ShellUsageFunction) (&ShellRegression::usageShellRegressionSetAllStates)));
    addShellCommandHandler (ShellCommandHandler (string("setdefault"),  1,  (ShellCmdFunction) (&ShellRegression::shellExecuteSetDefaultState),     4, (ShellUsageFunction) (&ShellRegression::usageShellRegressionSetDefaultState)));
    addShellCommandHandler (ShellCommandHandler (string("prepare"),     3,  (ShellCmdFunction) (&ShellRegression::shellExecutePrepareRegression),   1, (ShellUsageFunction) (&ShellRegression::usageShellRegressionPrepare)));
    addShellCommandHandler (ShellCommandHandler (string("prepare2"),    3,  (ShellCmdFunction) (&ShellRegression::shellExecutePrepareRegression2),  1, (ShellUsageFunction) (&ShellRegression::usageShellRegressionPrepare2)));
    addShellCommandHandler (ShellCommandHandler (string("start"),       1,  (ShellCmdFunction) (&ShellRegression::shellExecuteStartRegression),     2, (ShellUsageFunction) (&ShellRegression::usageShellRegressionStart)));
    addShellCommandHandler (ShellCommandHandler (string("run"),         2,  (ShellCmdFunction) (&ShellRegression::shellExecuteRunTestService),      1, (ShellUsageFunction) (&ShellRegression::usageShellRegressionRun)));
    addShellCommandHandler (ShellCommandHandler (string("help"),        1,  (ShellCmdFunction) (&ShellRegression::shellExecuteHelpRegression),      1, (ShellUsageFunction) (&ShellRegression::usageShellRegressionHelp)));
    addShellCommandHandler (ShellCommandHandler (string("quit"),        1,  (ShellCmdFunction) (&ShellRegression::shellExecuteQuitRegression),      1, (ShellUsageFunction) (&ShellRegression::usageShellRegressionQuit)));

}

ShellRegression::~ShellRegression ()
{
}

UI32 ShellRegression::shellExecuteListServices (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getRegressionShell ()->listServices (argc, argv));
}

UI32 ShellRegression::listServices (UI32 argc, vector<string> argv)
{
    RegressionTestObjectManagerGetTestServiceEntriesMessage message;
    UI32                                                    nServices;
    PrismServiceId                                          id;
    bool                                                    testServiceState;
    UI32                                                    index;
    const char                                             *serviceState;
    string                                                  serviceName;
    WaveMessageStatus                                      status = WAVE_MESSAGE_ERROR;

    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellRegression::shellExecuteListServices : Entering ...");

    status = ShellPrism::shellSendSynchronously (&message);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR, "Failed to send GetTestService message");
    }
    else
    {
        if (WAVE_MESSAGE_SUCCESS != message.getCompletionStatus ())
        {
            ShellPrism::shellTrace (TRACE_LEVEL_ERROR, "Failed to get service list. Status: " + message.getCompletionStatus ());
        }
        else
        {
            nServices =   message.getNumberOfTestServiceEntries();
            cout << "Service #: " << nServices << endl;
            for (index=0; index<nServices ;index++)
            {
                RegressionTestEntry regressionTestEntry (message.getTestServiceEntryAt (index));
                id                  = regressionTestEntry.getTestServiceId ();
                testServiceState    = regressionTestEntry.getIsTestEnabled ();
                serviceState        = ((1 == testServiceState)? "enable" : "disble");
                serviceName         = FrameworkToolKit::getServiceNameById (id);
                printf ("%3d- %-40s: %s\n", index, serviceName.c_str (),  serviceState);
            }
        }
    }

    return SHELL_OK;
}

void ShellRegression::shellGetServicesList (void)
{
    RegressionTestObjectManagerGetTestServiceEntriesMessage message;
    PrismServiceId                id;
    UI32                        index;
    WaveMessageStatus            status = WAVE_MESSAGE_ERROR;

    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellRegression::shellGetServicesList : Entering ...");

    status = ShellPrism::shellSendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR, "Failed to send GetTestService message");
    }
    else
    {
        if (WAVE_MESSAGE_SUCCESS != message.getCompletionStatus ())
        {
            ShellPrism::shellTrace (TRACE_LEVEL_ERROR, string ("Failed to get service list. Status: ") + message.getCompletionStatus ());
        }
        else
        {
            m_nServices =   message.getNumberOfTestServiceEntries();
            for (index = 0; index < m_nServices ;index++)
            {
                RegressionTestEntry regressionTestEntry (message.getTestServiceEntryAt (index));
                id                  = regressionTestEntry.getTestServiceId ();
                m_serviceMap.push_back (id);
            }
        }
    }
}

UI32 ShellRegression::shellExecuteSetServiceState (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getRegressionShell ()->setServiceState (argc, argv));
}

UI32 ShellRegression::setServiceState (UI32 argc, vector<string> argv)
{
    bool                        state;
    PrismServiceId                id;
    UI32                        serviceIndex;
    WaveMessageStatus            status = WAVE_MESSAGE_ERROR;

    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellRegression::shellExecuteSetServiceState : Entering ...");

    serviceIndex    =  atoi(argv[1].c_str ());
    if (serviceIndex > m_nServices)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR,  "invalid service id");
        return SHELL_OK;
    }

    id  =   m_serviceMap [serviceIndex];

    if ("enable" == argv[2])
    {
        state = true;
    }
    else
    {
        if ("disable" == argv[2])
        {
            state = false;
        }
        else
        {
            ShellPrism::shellTrace (TRACE_LEVEL_ERROR, "invalid state: state should be enable/disable");
            return SHELL_OK;
        }
    }

    RegressionTestObjectManagerSetTestServiceStateMessage message (id, state);
    status = ShellPrism::shellSendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR, string ("Failed to send message to set state for service ") + serviceIndex);
    }
    else
    {
        if (WAVE_MESSAGE_SUCCESS != message.getCompletionStatus ())
        {
            ShellPrism::shellTrace (TRACE_LEVEL_ERROR, string ("Failed to set service state. status: ") + message.getCompletionStatus ());
        }
        else
        {
            ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, string ("Succeeded to set service state for service ") + serviceIndex);
        }
    }
    return SHELL_OK;
}

UI32 ShellRegression::shellExecuteSetAllServicesState (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getRegressionShell ()->setAllServicesState (argc, argv));
}

UI32 ShellRegression::setAllServicesState (UI32 argc, vector<string> argv)
{
    bool                        state;
    PrismServiceId                id;
    UI32                        serviceIndex;
    WaveMessageStatus            status = WAVE_MESSAGE_ERROR;

    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellRegression::shellExecuteSetAllServicesState : Entering ...");

    if ("enable" == argv[1])
    {
        state = true;
    }
    else
    {
        if ("disable" == argv[1])
        {
            state = false;
        }
        else
        {
            ShellPrism::shellTrace (TRACE_LEVEL_ERROR, "invalid state: state should be enable/disable");
            return SHELL_OK;
        }
    }

    for (serviceIndex = 0; serviceIndex < m_nServices; serviceIndex++)
    {
        id  =   m_serviceMap [serviceIndex];
        RegressionTestObjectManagerSetTestServiceStateMessage message (id, state);
        status = ShellPrism::shellSendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            ShellPrism::shellTrace (TRACE_LEVEL_ERROR, string ("Failed to send message to set state for service ") + serviceIndex);
        }
        else
        {
            if (WAVE_MESSAGE_SUCCESS != message.getCompletionStatus ())
            {
                ShellPrism::shellTrace (TRACE_LEVEL_ERROR, string ("Failed to set state (service, status): ") + serviceIndex + message.getCompletionStatus ());
            }
            else
            {
                ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, string ("Succeeded to set state for service ") + serviceIndex);
            }
        }
    }

    return SHELL_OK;
}

UI32 ShellRegression::shellExecuteRunTestService  (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getRegressionShell ()->runTestService (argc, argv));
}


UI32 ShellRegression::runTestService (UI32 argc, vector<string> argv)
{
    PrismServiceId                id;
    UI32                        serviceIndex;
    UI32                        nTimes = 1;
    WaveMessageStatus            status = WAVE_MESSAGE_ERROR;

    ShellPrism::shellTrace (TRACE_LEVEL_INFO, "ShellRegression::shellExecuteRunTestService : Entering ...");

    if (0 == m_nServices)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR,  "invalid service id - No Services Exist for testing ...");
        return SHELL_OK;
    }

    serviceIndex    =  atoi(argv[1].c_str ());
    if (serviceIndex > m_nServices)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR,  "invalid service id");
        return SHELL_OK;
    }

    id      =   m_serviceMap [serviceIndex];
    if (argc > 2)
    {
        nTimes  =   atoi(argv[2].c_str ());
    }
    cout << "ID: " << id << " nTimes: " << nTimes << endl;

    RegressionTestObjectManagerRunTestForAServiceMessage message (id, nTimes);
    status = ShellPrism::shellSendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR, string ("Failed to send message to run test for a service ") + serviceIndex);
    }
    else
    {
        if (WAVE_MESSAGE_SUCCESS != message.getCompletionStatus ())
        {
            ShellPrism::shellTrace (TRACE_LEVEL_ERROR, string ("Failed to run service test. status: ") + message.getCompletionStatus ());
        }
        else
        {
            ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, string ("Succeeded to run service test ") + serviceIndex);
        }
    }
    return SHELL_OK;
}

UI32 ShellRegression::shellExecuteSetDefaultState (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getRegressionShell ()->setDefaultState (argc, argv));
}

UI32 ShellRegression::setDefaultState (UI32 argc, vector<string> argv)
{
    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellRegression::shellExecuteSetDefaultState : Entering ...");

    return SHELL_OK;
}

UI32 ShellRegression::shellExecuteStartRegression (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getRegressionShell ()->startRegression (argc, argv));
}


UI32 ShellRegression::startRegression (UI32 argc, vector<string> argv)
{
    WaveMessageStatus                        status = WAVE_MESSAGE_ERROR;
    UI32                                    nTimes = 1;

    if ( argc > 1)
    {
        nTimes  =   atoi(argv[1].c_str ());
    }

    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellRegression::shellExecuteStartRegression : Entering ...");

    RegressionTestObjectManagerStartRegressionMessage           message (nTimes);

    status = ShellPrism::shellSendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR, "Failed to send startRegession message ");
    }
    else
    {
        if (WAVE_MESSAGE_SUCCESS != message.getCompletionStatus ())
        {
            ShellPrism::shellTrace (TRACE_LEVEL_ERROR, string ("Regession test failed: Status: ") + message.getCompletionStatus ());
        }
        else
        {
            ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "Regession test successed");
        }
    }


    return SHELL_OK;
}

UI32 ShellRegression::shellExecutePrepareRegression (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getRegressionShell ()->prepareRegression (argc, argv));
}


UI32 ShellRegression::prepareRegression (UI32 argc, vector<string> argv)
{
    ShellPrism::shellTrace (TRACE_LEVEL_INFO, "ShellRegression::shellExecuteStartRegression : Entering ...");

    WaveMessageStatus            status = WAVE_MESSAGE_ERROR;
    PrismServiceId                id;
    UI32                        serviceIndex;
    UI32                        argIndex;

    serviceIndex    =  atoi(argv[1].c_str ());
    if (serviceIndex > m_nServices)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR,  "invalid service id");
        return SHELL_OK;
    }

    id  =   m_serviceMap [serviceIndex];

    RegressionPrepareMessage           message (id);

//    cout << "argc: " << argc << " adding "<<  argc-2 << " arguments to Message " << endl;

    for (argIndex = 2; argIndex < argc; argIndex++)
    {
        message.addInputString (argv[argIndex]);
    }

    status = ShellPrism::shellSendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR, "Failed to send prepareRegession message ");
    }
    else
    {
        if (WAVE_MESSAGE_SUCCESS != message.getCompletionStatus ())
        {
            ShellPrism::shellTrace (TRACE_LEVEL_ERROR, string ("Prepare Regession failed: Status: ") + message.getCompletionStatus ());
        }
        else
        {
            ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "Prepare Regession successed");
        }
    }


    return SHELL_OK;
}

UI32 ShellRegression::shellExecutePrepareRegression2 (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getRegressionShell ()->prepareRegression2 (argc, argv));
}

UI32 ShellRegression::prepareRegression2 (UI32 argc, vector<string> argv)
{
    ShellPrism::shellTrace (TRACE_LEVEL_INFO, "ShellRegression::prepareRegression2 : Entering ...");

    WaveMessageStatus           status = WAVE_MESSAGE_ERROR;
    PrismServiceId              id;
    UI32                        serviceIndex;
    UI32                        argIndex;

    serviceIndex    =  atoi(argv[1].c_str ());
    if (serviceIndex > m_nServices)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR,  "invalid service id");
        return SHELL_OK;
    }

    id  =   m_serviceMap [serviceIndex];

    RegressionPrepareMessage2           message (id);

    for (argIndex = 2; argIndex < argc; argIndex++)
    {
        message.addInputString (argv[argIndex]);
    }

    status = ShellPrism::shellSendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        ShellPrism::shellTrace (TRACE_LEVEL_ERROR, "Failed to send prepareRegession2 message ");
    }
    else
    {
        if (WAVE_MESSAGE_SUCCESS != message.getCompletionStatus ())
        {
            ShellPrism::shellTrace (TRACE_LEVEL_ERROR, string ("Prepare Regession 2 failed: Status: ") + message.getCompletionStatus ());
        }
        else
        {
            ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "Prepare Regession 2 succeeded");
        }
    }


    return SHELL_OK;
}

UI32 ShellRegression::shellExecuteHelpRegression (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getRegressionShell ()->helpRegression (argc, argv));
}


UI32 ShellRegression::helpRegression (UI32 argc, vector<string> argv)
{
    ShellPrism::shellTrace (TRACE_LEVEL_DEVEL, "ShellRegression::shellExecuteHelpRegression : Entering ...");

    usageShellRegressionHelp ();

    return SHELL_OK;
}

UI32 ShellRegression::shellExecuteQuitRegression (UI32 argc, vector<string> argv)
{
    return ((ShellObjectManager::getInstance ())->m_pPrismShell->getRegressionShell ()->quitRegression (argc, argv));
}


UI32 ShellRegression::quitRegression (UI32 argc, vector<string> argv)
{
    m_serviceMap.clear ();
    return SHELL_EXIT;
}

void ShellRegression::usageShellRegressionList (void)
{
    cout << "list                                           <List all services and their states>" << endl;
}

void ShellRegression::usageShellRegressionSetState (void)
{
    cout << "setstate [serviceId] [enable/disable]          <Set service test state>" << endl;
}

void ShellRegression::usageShellRegressionSetAllStates (void)
{
    cout << "setallstate [enable/disable]                   <Set the test state for all services>" << endl;
}

void ShellRegression::usageShellRegressionSetDefaultState (void)
{
    cout << "setdefault                                     <Set the default test state for all services>" << endl;
}

void ShellRegression::usageShellRegressionPrepare (void)
{
    cout << "Prepare [serviceId] [arg1] [arg2] ...          <Prepare regression for a service>" << endl;
}

void ShellRegression::usageShellRegressionPrepare2 (void)
{
    cout << "prepare2 <serviceId> [-argKey argValue] ...    <Prepare regression for a service using a map>" << endl;
}

void ShellRegression::usageShellRegressionStart (void)
{
    cout << "start                                          <Start regression for all enabled services>" << endl;
}

void ShellRegression::usageShellRegressionRun (void)
{
    cout << "run [serviceId]                                <Run regression for certain service>" << endl;
}

void ShellRegression::usageShellRegressionQuit (void)
{
    cout << "quit                                           <Quit regression shell>" << endl;
}

void ShellRegression::usageShellRegressionHelp (void)
{
    usageShellRegressionList ();
    usageShellRegressionSetState ();
    usageShellRegressionSetAllStates ();
    usageShellRegressionSetDefaultState ();
    usageShellRegressionPrepare ();
    usageShellRegressionPrepare2 ();
    usageShellRegressionStart ();
    usageShellRegressionRun ();
    usageShellRegressionQuit ();
    cout << "help                                           <Help for regression shell>" << endl;
}

}
