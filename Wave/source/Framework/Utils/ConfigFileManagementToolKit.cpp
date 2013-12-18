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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#include "Framework/Utils/ConfigFileManagementToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerCopySchemaMessage.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "WaveResourceIdEnums.h"
#include <fstream>
#include <algorithm>


namespace WaveNs
{

static string                       s_configFileManagementClientPath                        ("");
static string                       s_configFileManagementConfigCmdPath                     ("");
static string                       s_configFileManagementScriptsDirectory                  ("");
static string                       s_configFileManagementDefaultClientEnvironmentSettings  ("export ROLE_ID=root\nexport HOME=/root\nexport LOGNAME=root\nexport PWD=/root\n");
static string                       s_configFileManagementDefaultClientOptions              ("");
static string                       s_configFileManagementGlobalOnlyFilename                ("VcsConfigFile.GlobalOnly");
static string                       s_configFileManagementLocalOnlyFilename                 ("VcsConfigFile.LocalOnly");

string ConfigFileManagementToolKit::getConfigFileManagementClientPath ()
{
    return (s_configFileManagementClientPath);
}

void ConfigFileManagementToolKit::setConfigFileManagementClientPath (const string &configFileManagementClientPath)
{
    s_configFileManagementClientPath = configFileManagementClientPath;
}

string ConfigFileManagementToolKit::getConfigFileManagementConfigCmdPath ()
{
    return (s_configFileManagementConfigCmdPath);
}

void ConfigFileManagementToolKit::setConfigFileManagementConfigCmdPath (const string &configFileManagementConfigCmdPath)
{
    s_configFileManagementConfigCmdPath = configFileManagementConfigCmdPath;
}

string ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory ()
{
    return (s_configFileManagementScriptsDirectory);
}

void ConfigFileManagementToolKit::setConfigFileManagementScriptsDirectory (const string &configFileManagementScriptsDirectory)
{
    s_configFileManagementScriptsDirectory = configFileManagementScriptsDirectory;
}

string ConfigFileManagementToolKit::getConfigFileManagementDefaultClientEnvironmentSettings ()
{
    return (s_configFileManagementDefaultClientEnvironmentSettings);
}

string ConfigFileManagementToolKit::getConfigFileManagementDefaultClientOptions ()
{
    s_configFileManagementDefaultClientOptions = " --cwd " + getConfigFileManagementScriptsDirectory () + " -C << EOF\n";

    return (s_configFileManagementDefaultClientOptions);
}

string ConfigFileManagementToolKit::getConfigFileManagementGlobalOnlyFilename ()
{
    return (s_configFileManagementGlobalOnlyFilename);
}

string ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename ()
{
    return (s_configFileManagementLocalOnlyFilename);
}

SI32 ConfigFileManagementToolKit::configFileManagementClientCommandOutput (const string &configFileManagementClientCommand, vector<string> &configFileManagementClientCommandOutput)
{
    string clientCommand (getConfigFileManagementDefaultClientEnvironmentSettings () + getConfigFileManagementClientPath () + getConfigFileManagementDefaultClientOptions () + configFileManagementClientCommand + "\nEOF\n");

    trace (TRACE_LEVEL_INFO, string ("ConfigFileManagementToolKit::configFileManagementClientCommandOutput : Client Command :\n") + clientCommand);

    return (FrameworkToolKit::systemCommandOutput (clientCommand, configFileManagementClientCommandOutput));
}

SI32 ConfigFileManagementToolKit::configFileManagementConfigCmdOutput (const string &configFileManagementConfigCmd, vector<string> &configFileManagementConfigCmdOutput)
{
    string configCmd (getConfigFileManagementDefaultClientEnvironmentSettings () + getConfigFileManagementConfigCmdPath () + " " + configFileManagementConfigCmd + "\nEOF\n");

    trace (TRACE_LEVEL_INFO, string ("ConfigFileManagementToolKit::configFileManagementConfigCmdOutput : Config Command :\n") + configCmd);

    return (FrameworkToolKit::systemCommandOutput (configCmd, configFileManagementConfigCmdOutput));
}

bool ConfigFileManagementToolKit::isFileExisting (const string &fullFilePath)
{
    ifstream inputFile (fullFilePath.c_str ());

    return (inputFile.good ());
}

ResourceId ConfigFileManagementToolKit::replayConfigurationFile (const string &configFilePath, const string &configFilename, const string &preReplayActionCommands)
{
    string      fullConfigFilename  (configFilePath + "/" + configFilename);
    ResourceId  status              = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, string ("ConfigFileManagementToolKit::replayConfigurationFile : Replaying config file \"") + fullConfigFilename + "\" ...");

    string          clientCommand       (preReplayActionCommands + "\nterminal timeout 0\nconfigReplayInProgressSet\nccmdConfigReplayInProgressSet\nfileReplayInProgress\nccmdfileReplayInProgress\nconfig terminal\nload merge " + configFilename+"\nexit config\nfileReplayEnd\nccmdfileReplayEnd");
    vector<string>  clientCommandOutput;

    SI32 clientCommandReturnStatus = configFileManagementClientCommandOutput (clientCommand, clientCommandOutput);

    string  clientCommandOutputString   = "";

    for (UI32 i = 0; i < clientCommandOutput.size (); i++)
    {
        clientCommandOutputString += clientCommandOutput[i];
    }

    trace (TRACE_LEVEL_INFO, "ConfigFileManagementToolKit::replayConfigurationFile : Replay Command Output:\n" + clientCommandOutputString);

    if (0 == clientCommandReturnStatus)
    {
        trace (TRACE_LEVEL_SUCCESS, "ConfigFileManagementToolKit::replayConfigurationFile : Replaying config file completed successfully.");

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("ConfigFileManagementToolKit::replayConfigurationFile : Replaying config file failed with return code: ") + clientCommandReturnStatus);

        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

ResourceId ConfigFileManagementToolKit::renameConfigurationFile (const string &oldConfigFile, const string &newConfigFile)
{
    string          backupCommand ("/bin/mv -f " + oldConfigFile + " " + newConfigFile);
    vector<string>  backupOutput;
    ResourceId      status          = WAVE_MESSAGE_SUCCESS;

    SI32 systemCommandReturnStatus = FrameworkToolKit::systemCommandOutput (backupCommand, backupOutput);

    if (0 != systemCommandReturnStatus)
    {
        string backupCommandOutputString = "";

        for (UI32 i = 0; i < backupOutput.size (); i++)
        {
            backupCommandOutputString += backupOutput[i];
        }

        trace (TRACE_LEVEL_ERROR, "ConfigFileManagementToolKit::renameConfigurationFile : Failed to rename " + oldConfigFile + " to " + newConfigFile  + ":\n" + backupCommandOutputString);
        
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        trace (TRACE_LEVEL_SUCCESS, "ConfigFileManagementToolKit::renameConfigurationFile : Renamed " + oldConfigFile + " to " + newConfigFile);

        status = WAVE_MESSAGE_SUCCESS;
    }

    return (status);
}

ResourceId ConfigFileManagementToolKit::copyRunningToStartup ()
{

    WaveMessageStatus status           = WAVE_MESSAGE_ERROR;
    ResourceId        completionStatus = WAVE_MESSAGE_ERROR;

    PersistenceLocalObjectManagerCopySchemaMessage *pPersistenceLocalObjectManagerCopySchemaMessage;
    string sourceSchema = OrmRepository::getWaveCurrentSchema ();
    string destSchema   = OrmRepository::getWaveStartSchema ();

    WaveNs::trace (TRACE_LEVEL_INFO, "ConfigFileManagementToolKit::copyRunningToStartup: copying from source schema " + sourceSchema + " to destination schema " + destSchema);
 
    pPersistenceLocalObjectManagerCopySchemaMessage = new PersistenceLocalObjectManagerCopySchemaMessage (sourceSchema, destSchema);

    status = WaveObjectManagerToolKit::sendSynchronously (pPersistenceLocalObjectManagerCopySchemaMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "ConfigFileManagementToolKit::copyRunningToStartup: send to Persistence service failed");
        //raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_FRCLOG, DCM_1102);

        delete pPersistenceLocalObjectManagerCopySchemaMessage;
        
        return (WAVE_CONFIGURATION_COPY_RUNNING_TO_STARTUP_FAILED);
    }
    else
    {
        completionStatus = pPersistenceLocalObjectManagerCopySchemaMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("ConfigFileManagementToolKit::copyRunningToStartup: Message completed with error") + FrameworkToolKit::localize (completionStatus));

            delete pPersistenceLocalObjectManagerCopySchemaMessage;

            return (WAVE_CONFIGURATION_COPY_RUNNING_TO_STARTUP_FAILED);
        }
    }

    delete pPersistenceLocalObjectManagerCopySchemaMessage;

    return completionStatus;
}

ResourceId ConfigFileManagementToolKit::copyRunningToStartupFile ()
{

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    WaveNs::trace (TRACE_LEVEL_INFO, "ConfigFileManagementToolKit::copyRunningToStartupFile: copying current startup-schema to the startup config file");

    // On success copy current startup-schema to the startup config file.
    // copy startup-db to a file

    string startupConfigFileName = getConfigFileManagementScriptsDirectory () + "/startup-config";

    string command = getConfigFileManagementClientPath () + " -C --user=_copy_schema_ --group=admin << EOF > " + startupConfigFileName + "\nshow running-config\nEOF\n"; 

    vector<string> output;

    SI32 rc = FrameworkToolKit::systemCommandOutput (command, output);
    if (rc) {
        WaveNs::trace (TRACE_LEVEL_WARN, "VcsCopyToStartupWorker::copyStartupToStartupFileStep: Failed to create startup-config file");
    }
    else
    {
        FrameworkToolKit::setStartupFileName (startupConfigFileName);
        FrameworkToolKit::setStartupFileType (WAVE_PERSISTENCE_REGULAR_FILE);
        status = FrameworkToolKit::savePrismConfiguration (false);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
             WaveNs::trace (TRACE_LEVEL_WARN, "VcsCopyToStartupWorker::copyStartupToStartupFileStep: savePrismConfiguration failed.");
        }
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        return (WAVE_CONFIGURATION_COPY_RUNNING_TO_STARTUP_FILE_FAILED);
    }
    else
    {
       return status; 
    }

}

void ConfigFileManagementToolKit::getDifference (const vector<string> &set1, const vector<string> &set2, vector<string> &set1MinusSet2Difference)
{
    set_difference (set1.begin (), set1.end (), set2.begin (), set2.end (), inserter (set1MinusSet2Difference, set1MinusSet2Difference.begin ()));
}

ResourceId ConfigFileManagementToolKit::deleteConfigurationFile (const string &fullFilePath)
{
    string          deleteCommand ("/bin/rm -f " + fullFilePath);
    vector<string>  deleteOutput;
    ResourceId      status          = WAVE_MESSAGE_SUCCESS;

    SI32 systemCommandReturnStatus = FrameworkToolKit::systemCommandOutput (deleteCommand, deleteOutput);

    if (0 != systemCommandReturnStatus)
    {
        string deleteCommandOutputString = "";

        for (UI32 i = 0; i < deleteOutput.size (); i++)
        {
            deleteCommandOutputString += deleteOutput[i];
        }

        trace (TRACE_LEVEL_ERROR, "ConfigFileManagementToolKit::deleteConfigurationFile : Failed to delete " + fullFilePath + ":\n" + deleteCommandOutputString);

        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        trace (TRACE_LEVEL_SUCCESS, "ConfigFileManagementToolKit::deleteConfigurationFile : deleted " + fullFilePath);

        status = WAVE_MESSAGE_SUCCESS;
    }

    return (status);
}

}

