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

#include "Shell/WaveCliDebugShell.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Utils/MemoryUtils.h"
#include "Framework/Types/UI32Range.h"
#include "Framework/DistributedDebugInfrastructure/DistributedDebugParameters.h"

#include <stdlib.h>
#include <vector>

namespace WaveNs
{

WaveCliDebugShell::WaveCliDebugShell (WaveClientSynchronousConnection &connection)
    : WaveCliShell ("debug", connection)
{
    addCommandfunction ("testasyncmgmtintf",   reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::testAsynchronousWaveManagementInterface), "Test Asynch Wave Management Interface", NULL);
    addCommandfunction ("testasyncmultipleresponsemgmtintf",   reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::testAsynchronousMultipleResponseWaveManagementInterface), "Test Asynchronous Multiple Response Wave Management Interface", NULL);
    addCommandfunction ("testsendtowaveclientsmgmtintf",   reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::testSendToWaveClientsManagementInterface), "Test Asynch Insntance Wave Clients Management Interface", NULL);
    addCommandfunction ("testmgmtintf",   reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::testWaveManagementInterface), "Tests Wave Management Interface.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::testWaveManagementInterfaceHelp));
    addCommandfunction ("testdebugprint", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::testDebugPrint),              "Tests Debug Print Mechanism.",     reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::testDebugPrintHelp));
    addCommandfunction ("addlogentry", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::addLogEntry),            "Tests Adding Entries to Distributed Log.", NULL);
    addCommandfunction ("updatemaxlogentries", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::updateMaxLogEntries), "Tests Updating Max Log Entries in Distributed Log.", NULL);
    addCommandfunction ("psql", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::debugPsql), "Execute Psql commands.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::debugPsqlHelp));
    addCommandfunction ("releaseunusedmemory", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::debugReleaseUnusedMemory), "Release Unused Memory back to system.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::debugReleaseUnusedMemoryHelp));
    addCommandfunction ("comparedbincluster", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::compareDBInCluster), "Compare dabatabases on each nodes that on Primary in cluster.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::compareDBInClusterHelp));
    addCommandfunction ("messagehistorydump", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::messageHistoryDump), "Prints Message History of a service.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::messageHistoryDumpHelp));
    addCommandfunction ("messagehistoryconfig", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::messageHistoryConfig), "Configure Message History state, size.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::messageHistoryConfigHelp));
    addCommandfunction ("acquirelock", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::acquireLock),  "Acquires a cluster wide lock with the given service string", NULL);
    addCommandfunction ("releaselock", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::releaseLock),  "Releases a cluster wide lock with the given service string", NULL);
    addCommandfunction ("zeroize", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::FIPSZeroize),  "Reset Dcmd to factory settings.", NULL);
    addCommandfunction ("getdebuginformation", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::getDebugInformation),  "Get debug information for a service.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::getDebugInformationHelp));
    addCommandfunction ("resetdebuginformation", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::resetDebugInformation),  "Reset debug information for a service.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::resetDebugInformationHelp));
    addCommandfunction ("setPrismConfigurationValid", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::setPrismConfigurationValid), "Set/Reset the validity of PrismConfiguration.", NULL ); 
    addCommandfunction ("runDebugScript", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::runDebugScript),  "Run debug script on all nodes in the cluster.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::runDebugScriptHelp));
    addCommandfunction ("copyFile", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::copyFile),  "Copy a file from this node to specified other nodes in the cluster.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::copyFileHelp));
    addCommandfunction ("copyFileToHaPeer", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::copyFileToHaPeer),  "Copy a file from this node to Ha Peer (standby location) if standby is present.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::copyFileToHaPeerHelp));
    addCommandfunction ("printOrm", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::printOrm),  "Prints object relational mapping repository information.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::printOrmHelp));
    addCommandfunction ("debugSchemaChange", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::debugSchemaChange),  "Debugging facility for DB converion.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::debugSchemaChangeHelp));
//    addCommandfunction ("getAllInstanceIdsForTable", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::debugSchemaChange),  "Prints object relational mapping repository information.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::debugSchemaChangeHelp));

    addCommandfunction ("addXPathStrings", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::addXPathStrings),  "Add XPath Strings", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::addXPathStringsHelp));
    addCommandfunction ("deleteXPathStrings", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::deleteXPathStrings),  "Delete XPath Strings", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::deleteXPathStringsHelp));
    addCommandfunction ("getLastUpdateTimestampsForXPathStrings", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::getLastUpdateTimestampsForXPathStrings),  "Get Last Update Timestamps For XPath Strings", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::getLastUpdateTimestampsForXPathStringsHelp));
    addCommandfunction ("resetLastUpdateTimestampsForXPathStrings", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::resetLastUpdateTimestampsForXPathStrings),  "Reset LastUpdateTimestamps for XPath Strings", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::resetLastUpdateTimestampsForXPathStringsHelp));
    addCommandfunction ("debugChangeQuerySettings", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::debugChangeQuerySettings),  "Change Database Query Settings", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::debugChangeQuerySettingsHelp));

    addCommandfunction ("emptyDatabase", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::emptyDatabase),  "Empty Database and get SQL to empty Database in return", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::emptyDatabaseHelp));

    addCommandfunction ("changeEmptyDatabaseParameter", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::changeEmptyDatabaseParameter),  "This CLI can change the threshold Value to decide between delete and truncate", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliDebugShell::changeEmptyDatabaseParameterHelp));

    addCommandfunction ("getEmptyDatabaseParameter", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::getEmptyDatabaseParameter),  "This CLI will give threshold valye to decide between delete and truncate. Also this will give number entries in Database Tables.", NULL);

    addCommandfunction ("setControllerIpAddress", reinterpret_cast<WaveShellCommandFunction> (&WaveCliDebugShell::setControllerIpAddress),  "setControllerIpADdress", NULL);
}

WaveCliDebugShell::~WaveCliDebugShell ()
{
}

void WaveCliDebugShell::briefHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Debug shell provides a way to execute debug actions.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Developer can execute pre-registered actions from this shell.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Debug actions that are not particularly part of any other shells");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        typically land here.");
}

ResourceId WaveCliDebugShell::testAsynchronousWaveManagementInterface (const vector<string> &arguments)
{
    ResourceId                      status;
    WaveClientSynchronousConnection connection = getConnection ();

    status = connection.testAsynchronousWaveManagementInterface ();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED Testing Wave Management Interface.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED Testing Wave Management Interface. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId WaveCliDebugShell::testAsynchronousMultipleResponseWaveManagementInterface (const vector<string> &arguments)
{
    ResourceId                      status;
    WaveClientSynchronousConnection connection = getConnection ();

    status = connection.testAsynchronousMultipleResponseWaveManagementInterface ();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED Testing Wave Management Interface.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED Testing Wave Management Interface. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId WaveCliDebugShell::testWaveManagementInterface (const vector<string> &arguments)
{
    ResourceId                      status;
    WaveClientSynchronousConnection connection = getConnection ();

    status = connection.testWaveManagementInterface ();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED Testing Wave Management Interface.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED Testing Wave Management Interface. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId WaveCliDebugShell::testSendToWaveClientsManagementInterface (const vector<string> &arguments)
{
    ResourceId                      status;
    WaveClientSynchronousConnection connection = getConnection ();

    status = connection.testSendToWaveClientsManagementInterface (arguments);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED Testing Wave Management Interface.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED Testing Wave Management Interface. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::testWaveManagementInterfaceHelp ()
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

ResourceId WaveCliDebugShell::testDebugPrint (const vector<string> &arguments)
{
    ResourceId                      status     = WAVE_MESSAGE_SUCCESS;
    WaveClientSynchronousConnection connection = getConnection ();
    string                          message    = "This is a Test Message: Test 1. Test 2. Test 3.\n";
    UI32                            count      = 1;
    UI32                            i          = 0;

    if (1 <= (arguments.size ()))
    {
        message  = arguments[0];
        message += "\n";
    }

    if (2 <= (arguments.size ()))
    {
        count = strtoul (arguments[1].c_str (), NULL, 10);
    }

    if (0 == count)
    {
        count = 1;
    }

    for (i = 0; i < count; i++)
    {
        status = connection.debugPrint (message);
    }

    // We only check the status of thelast iteration for now.

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED Testing Debug Print Mechanism.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED Testing Debug Print Mechanism. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::testDebugPrintHelp ()
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

ResourceId WaveCliDebugShell::addLogEntry (const vector<string> &arguments)
{
    ResourceId                      status;
    WaveClientSynchronousConnection connection = getConnection ();
    UI32                            numberOfArguments = arguments.size();

    if (2 < numberOfArguments || 1 > numberOfArguments)
    {
        trace (TRACE_LEVEL_ERROR, "Usage: addLogEntry <description> [username]");
        return (WAVE_MESSAGE_ERROR);
    }

    status = connection.addLogEntry (arguments);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED Adding log entry.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED Adding log entry. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId WaveCliDebugShell::updateMaxLogEntries (const vector<string> &arguments)
{
    ResourceId                      status;
    WaveClientSynchronousConnection connection = getConnection ();
    UI32                            numberOfArguments = arguments.size();

    if (1 != numberOfArguments)
    {
        trace (TRACE_LEVEL_ERROR, "Usage: updateMaxLogEntries <number of entries>");
        return (WAVE_MESSAGE_ERROR);
    }

    status = connection.updateMaxLogEntries (arguments);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED Updating max log entries.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED Updating max log entries. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::debugPsql (UI32 argc, vector<string> argv)
{
    // PSQL Lib and Bin path are exported as the user login shell is different from the Dcmd environment
    // Do not remove the ';' at the end of psql_path string

    string psql_path = string ("export PATH=$PATH:/usr/local/postgresql/9.1.3/bin ; export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/postgresql/9.1.3/lib ;");

    string commandString = psql_path + string ("psql -p ") + (DatabaseObjectManager::getDatabasePort ()) + string (" ") + DatabaseObjectManager::getDatabaseName ();

     system (commandString.c_str ());

}

void WaveCliDebugShell::debugPsqlHelp()
{
     tracePrintf (TRACE_LEVEL_INFO, true, true, "        This shell provides a way to execute Psql commands.");
}

void WaveCliDebugShell::debugReleaseUnusedMemory (UI32 argc, vector<string> argv)
{
    MemoryUtils::releaseUnusedMemoryToSystem ();
}

void WaveCliDebugShell::debugReleaseUnusedMemoryHelp ()
{
     tracePrintf (TRACE_LEVEL_INFO, true, true, "        Releases ununsed memory back to the system.");
}

void WaveCliDebugShell::compareDBInCluster (UI32 argc, vector<string> argv)
{
    ResourceId                      status;
    WaveClientSynchronousConnection connection = getConnection ();

    status = connection.compareDBWithPrimary ();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED Comparing databases.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to compare databases with Primary. Status : " + FrameworkToolKit::localize (status));
    }

}

void WaveCliDebugShell::compareDBInClusterHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : ");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Compares the databases on each node in cluster with Primary's database.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    None.  All arguments are ignored.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Lists the nodes having matching database with that of Primary shown as IP#Port of a node.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Primary will always be in list of matching databases node's list with * appended in IP#Port string.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Also in the same way, lists nodes with non-matching database that with Primary and nodes for which");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    framework failed to get/compare the database if any.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}



ResourceId WaveCliDebugShell::messageHistoryDump (const vector<string> &arguments)
{
    ResourceId                      status = WAVE_MESSAGE_ERROR;
    UI32                            numberOfArguments   = arguments.size();

    if (1 != numberOfArguments)
    {
        trace (TRACE_LEVEL_ERROR, "messagehistorydump <serviceId>");
        return (status);
    }

    WaveClientSynchronousConnection connection = getConnection ();

    PrismServiceId prismServiceId = (PrismServiceId) atoi(arguments[0].c_str ());

    status = connection.dumpMessageHistoryOfAService (prismServiceId);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "WaveCliDebugShell::messageHistoryDump : success.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveCliDebugShell::messageHistoryDump : Failed. Status : " + FrameworkToolKit::localize (status));
    }
    return (status);
}

void WaveCliDebugShell::messageHistoryDumpHelp (void)
{
     tracePrintf (TRACE_LEVEL_INFO, true, true, "        Prints Message History of a service.");
}

ResourceId WaveCliDebugShell::messageHistoryConfig (const vector<string> &arguments)
{
    ResourceId      status                = WAVE_MESSAGE_ERROR;
    UI32            numberOfArguments     = arguments.size();

    if ((2 > numberOfArguments) || (3 < numberOfArguments))
    {
        trace (TRACE_LEVEL_ERROR, "messagehistoryconfig <serviceid> <on/off> <size(optional, default=30)>");
        return (status);
    }

    // Argument validation.

    PrismServiceId  prismServiceId         = (PrismServiceId) atoi(arguments[0].c_str ());

    bool            messageHistoryState    = false;
    UI32            messageHistoryMaxSize  = 0;

    if ("on" == arguments[1])
    {
        messageHistoryState = true;
    }
    else if ("off" == arguments[1])
    {
        messageHistoryState = false;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "messagehistoryconfig <serviceid> <on/off> <size(optional, default=30)>");
        return (status);
    }

    if (3 == numberOfArguments)
    {
        messageHistoryMaxSize = (UI32) atoi(arguments[2].c_str ());
    }

    WaveClientSynchronousConnection connection = getConnection ();

    status = connection.configMessageHistoryOfAService (prismServiceId, messageHistoryState, messageHistoryMaxSize);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "WaveCliDebugShell::messageHistoryConfig : success.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveCliDebugShell::messageHistoryConfig : Failed. Status : " + FrameworkToolKit::localize (status));
    }
    return (status);
}

void WaveCliDebugShell::messageHistoryConfigHelp (void)
{
     tracePrintf (TRACE_LEVEL_INFO, true, true, "        Prints Message History of a service.");
}

ResourceId WaveCliDebugShell::acquireLock (const vector<string> &arguments)
{
    ResourceId                     status = WAVE_MESSAGE_SUCCESS;
    string                         serviceString;

    if (1 != arguments.size ())
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : acquirelock <serviceString>");
        return (WAVE_MESSAGE_ERROR);
    }

    serviceString = arguments[0];

    WaveClientSynchronousConnection connection = getConnection ();

    status = connection.acquireLock (serviceString);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Successfully acquired lock for serviceString = %s",  serviceString.c_str ());
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to acquire the lock");
    }

    return (status);
}

ResourceId WaveCliDebugShell::releaseLock (const vector<string> &arguments)
{
    ResourceId                     status = WAVE_MESSAGE_SUCCESS;
    string                         serviceString;

    if (1 != arguments.size ())
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : releaselock <serviceString>");
        return (WAVE_MESSAGE_ERROR);
    }

    serviceString = arguments[0];

    WaveClientSynchronousConnection connection = getConnection ();

    status = connection.releaseLock (serviceString);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Successfully release lock for serviceString = %s",  serviceString.c_str ());
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to release the lock");
    }

    return (status);
}


ResourceId WaveCliDebugShell::FIPSZeroize (const vector<string> &arguments)
{
    ResourceId                     status = WAVE_MESSAGE_SUCCESS;

    WaveClientSynchronousConnection connection = getConnection ();

    status = connection.FIPSZeroize();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Successfully completed FIPS Zeroize");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Zeroize did not complete");
    }

    // Following trace is processed by the fips-zeroize script.

    tracePrintf (TRACE_LEVEL_INFO, true, true, "%s", (FrameworkToolKit::localizeToSourceCodeEnum (status)).c_str ());

    return (status);
}

ResourceId WaveCliDebugShell::getDebugInformation (const vector<string> &arguments)
{
    ResourceId          status          = WAVE_MESSAGE_ERROR;

    if (1 != arguments.size ())
    {
        getDebugInformationHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    if (false == UI32Range::isAValidString (arguments[0]))
    {
        trace (TRACE_LEVEL_ERROR, "WaveCliDebugShell::getDebugInformation : Service id range is invalid.  Please enter an ascending numeric range.");
        getDebugInformationHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    WaveClientSynchronousConnection connection = getConnection ();

    UI32Range serviceIdRange (arguments[0]);

    vector<UI32> serviceIds;
    serviceIdRange.getUI32RangeVector (serviceIds);

    UI32 numberOfServiceIds = serviceIds.size ();

    for (UI32 i = 0; i < numberOfServiceIds; ++i)
    {
        string      serviceName             = connection.getServiceNameById (serviceIds[i]);
        string  debugInformation        = "";
    
        status = connection.getDebugInformation (serviceName, debugInformation);
    
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            if (0 < debugInformation.size ())
            {
                trace (TRACE_LEVEL_INFO, string ("Debug information for Service Id : ") + serviceIds[i] + " (" + serviceName + ")\n" + debugInformation);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string ("No debug information available for Service Id : ") + serviceIds[i] + " (" + serviceName + ")\n" + debugInformation);
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "WaveCliDebugShell::getDebugInformation : Failed.  Status : " + FrameworkToolKit::localize (status));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

void WaveCliDebugShell::getDebugInformationHelp (void)
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : getdebuginformation <service id range>");
}

ResourceId WaveCliDebugShell::resetDebugInformation (const vector<string> &arguments)
{
    ResourceId          status          = WAVE_MESSAGE_ERROR;

    if (1 != arguments.size ())
    {
        resetDebugInformationHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    if (false == UI32Range::isAValidString (arguments[0]))
    {
        trace (TRACE_LEVEL_ERROR, "WaveCliDebugShell::resetDebugInformation : Service id range is invalid.  Please enter an ascending numeric range.");
        resetDebugInformationHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    WaveClientSynchronousConnection connection = getConnection ();

    UI32Range serviceIdRange (arguments[0]);

    vector<UI32> serviceIds;
    serviceIdRange.getUI32RangeVector (serviceIds);

    UI32 numberOfServiceIds = serviceIds.size ();

    for (UI32 i = 0; i < numberOfServiceIds; ++i)
    {
        string  serviceName = connection.getServiceNameById (serviceIds[i]);

    status = connection.resetDebugInformation (serviceName);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
            trace (TRACE_LEVEL_INFO, string ("Debug information has been reset for Service Id : ") + serviceIds[i] + " (" + serviceName + ")");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveCliDebugShell::resetDebugInformation : Failed.  Status : " + FrameworkToolKit::localize (status));
    }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

void WaveCliDebugShell::resetDebugInformationHelp (void)
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : resetdebuginformation <service id range>");
}

ResourceId WaveCliDebugShell::setPrismConfigurationValid ( const vector<string> &arguments )
{
    ResourceId  status = WAVE_MESSAGE_SUCCESS;

    bool    validity = false;
    string  schemaFile = "";

    if (1 > arguments.size () || 2 < arguments.size ())
    {   
        tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : setPrismConfigurationValid <true/false> [schemaFile]");
        return (WAVE_MESSAGE_ERROR);
    }
    else
    {   
        if ( !( arguments[0].compare("true")) )
        {   
            validity = true;
        }
        else if ( !(arguments[0].compare("false")) )
        {   
            validity = false;
        }
        else
        {   
            tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : setPrismConfigurationValid <true/false> [schemaFile]");
            return (WAVE_MESSAGE_ERROR);
        }
        if (2 == arguments.size ())
        {
            schemaFile = (arguments[1]);
        }
        trace (TRACE_LEVEL_INFO, "schemaFile : "+ schemaFile);
    }

    WaveClientSynchronousConnection connection = getConnection ();

    status = connection.setPrismConfigurationValid ( validity, schemaFile );

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "WaveCliDebugShell::setPrismConfigurationValid : Command succeeded");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveCliDebugShell::setPrismConfigurationValid : Command Failed with status  " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::runDebugScriptHelp ()
{

    tracePrintf (TRACE_LEVEL_INFO, true, true, "Runs the Debug Script on all or specified locations.");

}
ResourceId WaveCliDebugShell::runDebugScript(const vector<string> &arguments)
{
    ResourceId                     status = WAVE_MESSAGE_SUCCESS;


    WaveClientSynchronousConnection connection = getConnection ();

    string inputStringFromScript = arguments[0];
    string nosOrBash = arguments [1];
    string locations = arguments[3];

    DistributedDebugParameters distributedDebugParameters (inputStringFromScript, nosOrBash, locations);

    tracePrintf (TRACE_LEVEL_INFO, true, true, "WaveCliDebugShell::runDebugScript: source %s, nosOrBash: %s: locations: %s",inputStringFromScript.c_str(),nosOrBash.c_str(),locations.c_str());


    status = connection.runDebugScript (distributedDebugParameters);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Successfully ran debug script");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Successfully Run Debug Script");
    }

    return (status);
}

ResourceId WaveCliDebugShell::copyFile (const vector<string> &arguments)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    // Argument type-value validation is not done.

    WaveClientSynchronousConnection connection = getConnection ();

    if (3 >= arguments.size ())
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Usage : copyFile sourceFileName destinationFileName copyFileFlag ipAddress1:port1 ipAddress2:port2 . . .");
        return (status);
    }

    string          sourceFileName      = arguments[0];
    string          destinationFileName = arguments[1];
    UI32            copyFileFlag        = (UI32) atoi(arguments[2].c_str ());

    vector<string>  vectorOfDestinationIpAddressesAndPorts (arguments.begin () + 3, arguments.end ());

    status = connection.copyFile (sourceFileName, destinationFileName, copyFileFlag, vectorOfDestinationIpAddressesAndPorts);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "Success in copyFile operation.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failure in copyFile operation. Status : ") + FrameworkToolKit::localize (status));
    }
    
    return (status);
}

void WaveCliDebugShell::copyFileHelp (void)
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, " copyFile sourceFileName destinationFileName copyFileFlag ipAddress1:port1 ipAddress2:port2 . . .");
    tracePrintf (TRACE_LEVEL_INFO, true, true, " sourceFileName and destinationFileName should be fully qualified file name each.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, " Values for copyFileFlag are - ");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "   0 : Overwrite dest file if exist");
    // tracePrintf (TRACE_LEVEL_INFO, true, true, "   1 : Fail if dest file exist");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "  16 : At destination, also send the file to Ha peer location");
}

ResourceId WaveCliDebugShell::copyFileToHaPeer (const vector<string> &arguments)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    // Argument type-value validation is not done.

    WaveClientSynchronousConnection connection = getConnection ();

    if (2 != arguments.size ())
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Usage : copyFileToHaPeer sourceFileName destinationFileName");
        return (status);
    }

    string          sourceFileName      = arguments[0];
    string          destinationFileName = arguments[1];

    status = connection.copyFileToHaPeer (sourceFileName, destinationFileName);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "Success in copyFileToHaPeer operation.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failure in copyFileToHaPeer operation. Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::copyFileToHaPeerHelp (void)
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, " copyFileToHaPeer sourceFileName destinationFileName");
    tracePrintf (TRACE_LEVEL_INFO, true, true, " sourceFileName and destinationFileName should be fully qualified file name each.");
}

ResourceId WaveCliDebugShell::printOrm (const vector<string> &arguments)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    WaveClientSynchronousConnection connection = getConnection ();

    status = connection.printOrm (arguments);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "Print ORM finished successfully.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Print ORM Failed.  Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::printOrmHelp ()
{
     tracePrintf (TRACE_LEVEL_INFO, true, true, "        USAGE: printOrm [--dot|-d] [--filter|-f <Comma delimited list of Managed Object Names>]");
}

ResourceId WaveCliDebugShell::debugSchemaChange (const vector<string> &arguments)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    
    if ((arguments.size () == 1 || arguments.size () == 2)) 
    {
        string command = arguments[0];
        if (((0 == command.compare ("printMORepository")) && (arguments.size ()== 1)) 
            || ((0 == command.compare ("getAllInstanceIds")) && (arguments.size () == 2)))
        {
            trace (TRACE_LEVEL_SUCCESS, string ("WaveCliDebugShell::debugSchemaChange: called for \"") + command + string (" ") + arguments [1] + string ("\""));
        }
        else
        {
            debugSchemaChangeHelp ();
            return (status);
        }
    }
    else
    {
        debugSchemaChangeHelp (); 
        return (status);
    }

    WaveClientSynchronousConnection connection = getConnection ();
    status = connection.debugSchemaChange (arguments);

    if (WAVE_MESSAGE_SUCCESS == status)
    {   
        trace (TRACE_LEVEL_SUCCESS, "debugSchemaChange finished successfully.");
    }
    else
    {   
        trace (TRACE_LEVEL_ERROR, string ("debugSchemaChange Failed.  Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::debugSchemaChangeHelp ()
{
     tracePrintf (TRACE_LEVEL_INFO, true, true, "        USAGE: debugSchemaChange printMORepository/getAllInstanceIds <MOName>");
}

ResourceId WaveCliDebugShell::addXPathStrings (const vector<string> &arguments)
{
    WaveClientSynchronousConnection connection = getConnection ();

    ResourceId status = connection.addXPathStrings (arguments);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "Success in addXPathStrings operation.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failure in addXPathStrings operation. Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::addXPathStringsHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "addXPathStrings xPathString1 xPathString2 ....");
}

ResourceId WaveCliDebugShell::deleteXPathStrings (const vector<string> &arguments)
{
    WaveClientSynchronousConnection connection = getConnection ();

    ResourceId status = connection.deleteXPathStrings (arguments);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "Success in deleteXPathStrings operation.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failure in deleteXPathStrings operation. Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::deleteXPathStringsHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "deleteXPathStrings xPathString1 xPathString2 ....");
}

ResourceId WaveCliDebugShell::getLastUpdateTimestampsForXPathStrings (const vector<string> &arguments)
{
    vector<string>  xPathStrings            = arguments;
    vector<UI64>    lastUpdatedTimestamps;

    WaveClientSynchronousConnection connection  = getConnection ();
    ResourceId                      status      = connection.getLastUpdateTimestampsForXPathStrings (xPathStrings, lastUpdatedTimestamps);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        UI32 numberOfXPathStrings    = xPathStrings.size ();
        UI32 numberOfTimestamps      = lastUpdatedTimestamps.size ();

        if (numberOfXPathStrings == numberOfTimestamps)
        {
            for (UI32 i = 0; i < numberOfXPathStrings; i++)
            {
                tracePrintf (TRACE_LEVEL_INFO, true, true, "%20llu [%s]", lastUpdatedTimestamps[i], (xPathStrings[i]).c_str ());
            }

            trace (TRACE_LEVEL_SUCCESS, "Success in getLastUpdateTimestampsForXPathStrings operation.");
        }
        else
        {
            tracePrintf (TRACE_LEVEL_ERROR, "WaveCliDebugShell::getLastUpdateTimestampsForXPathStrings numberOfXPathStrings [%u], numberOfTimestamps [%u]", numberOfXPathStrings, numberOfTimestamps);
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failure in getLastUpdateTimestampsForXPathStrings operation. Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::getLastUpdateTimestampsForXPathStringsHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "getLastUpdateTimestampsForXPathStrings xPathString1 xPathString2 ....");
}

ResourceId WaveCliDebugShell::resetLastUpdateTimestampsForXPathStrings (const vector<string> &arguments)
{
    WaveClientSynchronousConnection connection = getConnection ();

    ResourceId status = connection.resetLastUpdateTimestampsForXPathStrings ();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "Success in resetLastUpdateTimestampsForXPathStrings operation.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failure in resetLastUpdateTimestampsForXPathStrings operation. Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::resetLastUpdateTimestampsForXPathStringsHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "resetLastUpdateTimestampsForXPathStrings");
}

ResourceId WaveCliDebugShell::debugChangeQuerySettings (const vector<string> &arguments)
{
    UI32 numberOfArguments = arguments.size ();

    if ((0 == numberOfArguments) || (2 < numberOfArguments))
    {
        debugChangeQuerySettingsHelp ();

        return WAVE_MESSAGE_ERROR;
    }

    bool    useRecursiveQuery   = false;
    UI32    batchSize           = 0;

    if ("TRUE" == arguments[0])
    {
        useRecursiveQuery = true;
    }
    else if ("FALSE" == arguments[0])
    {
        useRecursiveQuery = false;
    }
    else
    {
        debugChangeQuerySettingsHelp ();

        return WAVE_MESSAGE_ERROR;
    }

    if (2 == numberOfArguments)
    {
        batchSize = atoi ((arguments[1]).c_str ());

        if (0 == batchSize)
        {
            debugChangeQuerySettingsHelp ();

            return WAVE_MESSAGE_ERROR;
        }
    }

    WaveClientSynchronousConnection connection = getConnection ();

    ResourceId status = connection.debugChangeQuerySettings (useRecursiveQuery, batchSize);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, string ("Successfully set useRecursiveQuery [") + arguments[0] + string ("], batchSize [") + batchSize + string ("]"));
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failure in debugChangeQuerySettings operation. Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);

}

void WaveCliDebugShell::debugChangeQuerySettingsHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "usage : debugChangeQuerySettings useRecursiveQuery(TRUE/FALSE) batchSize(UI32)");
}

ResourceId WaveCliDebugShell::getEmptyDatabaseParameter (const vector<string> &arguments)
{
    UI32    numberOfArguments         = arguments.size ();
    UI32    thresholdValue            = 0;
    UI32    numberOfEntriesInDatabase = 0;

    if (0 < numberOfArguments)
    {
        return (WAVE_MESSAGE_ERROR);
    }

    WaveClientSynchronousConnection connection = getConnection ();
    ResourceId                      status = WAVE_MESSAGE_SUCCESS;

    status = connection.getEmptyDatabaseParameter (thresholdValue, numberOfEntriesInDatabase);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, string ("Current Threshold Value configured to decide between delete and truncate is : ") + thresholdValue);
        trace (TRACE_LEVEL_SUCCESS, string ("Total number of entries in Database are                                     : ") + numberOfEntriesInDatabase);
    }
    else
    {    
        trace (TRACE_LEVEL_ERROR, string ("Failed to get the value Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::changeEmptyDatabaseParameterHelp ()
{
    trace (TRACE_LEVEL_INFO, string ("usage: changeEmptyDatabaseParameter -thresholdValue <number>"));
}

ResourceId WaveCliDebugShell::changeEmptyDatabaseParameter (const vector<string> &arguments)
{
    UI32    numberOfArguments = arguments.size ();
    UI32    thresholdValue    = 0;

    if ((2 != numberOfArguments))
    {
        changeEmptyDatabaseParameterHelp ();
        
        return (WAVE_MESSAGE_ERROR);
    }

    thresholdValue = atoi ((arguments[1]).c_str ());

    WaveClientSynchronousConnection connection = getConnection ();
    string                          sqlString;
    ResourceId                      status = WAVE_MESSAGE_SUCCESS;

    status = connection.configureEmptyDatabaseParameter (thresholdValue);

    if (WAVE_MESSAGE_SUCCESS == status)
    {    
        trace (TRACE_LEVEL_SUCCESS, string ("Successfully configured the empty database threshold value to decide between delete and truncate to: \n") + thresholdValue);
    }    
    else 
    {    
        trace (TRACE_LEVEL_ERROR, string ("Failure in configured the empty database threshold value to decide between delete and truncate Status : ") + FrameworkToolKit::localize (status));
    }    

    return (status);
}

ResourceId WaveCliDebugShell::setControllerIpAddress (const vector<string> &arguments)
{
    UI32    numberOfArguments = arguments.size ();
    string  emptyType;

    if ((2 != numberOfArguments))
    {
        return WAVE_MESSAGE_ERROR;
    }

    WaveClientSynchronousConnection connection = getConnection ();
    string                          ipAddress  = arguments [0];
    SI32                            port       = atoi(arguments[1].c_str ());
    ResourceId                      status = WAVE_MESSAGE_SUCCESS;

    status = connection.setControllerIpAddress (ipAddress, port);

    if (WAVE_MESSAGE_SUCCESS != status)
    {    
        trace (TRACE_LEVEL_ERROR, string ("Failed To Set IpAddress. Status : ") + FrameworkToolKit::localize (status));
    }    

    return (status);
}

ResourceId WaveCliDebugShell::emptyDatabase (const vector<string> &arguments)
{
    UI32    numberOfArguments = arguments.size ();
    string  emptyType;

    if ((1 < numberOfArguments))
    {    
        emptyDatabaseHelp ();

        return WAVE_MESSAGE_ERROR;
    }

    if (1 == numberOfArguments)
    {
        if ("-delete" == arguments [0] || "-truncate" == arguments [0] || "-auto" == arguments [0])
        {
            emptyType = arguments [0];
        }
        else
        {
            emptyDatabaseHelp ();

            return WAVE_MESSAGE_ERROR;
        }
    }

    WaveClientSynchronousConnection connection = getConnection ();
    string                          sqlString;
    ResourceId                      status = WAVE_MESSAGE_SUCCESS;

    status = connection.emptyDatabase (emptyType, sqlString);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, string ("Sql String To Empty Database is: \n") + sqlString);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failure in Empty Database Operation operation. Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliDebugShell::emptyDatabaseHelp ()
{
    trace (TRACE_LEVEL_INFO, string ("usage: emptyDatabase -delete/truncate/auto"));
    trace (TRACE_LEVEL_INFO, string ("      Parameter is optional, if it is not given current configured type will be used to Empty Database"));
}

}
