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

#ifndef CONFIGFILEMANAGEMENTTOOLKIT
#define CONFIGFILEMANAGEMENTTOOLKIT

#include "Framework/Types/Types.h"

#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

class ConfigFileManagementToolKit
{
    private :
    protected :
    public :
        static      string                      getConfigFileManagementClientPath                       ();
        static      void                        setConfigFileManagementClientPath                       (const string &configFileManagementClientPath);

        static      string                      getConfigFileManagementConfigCmdPath                    ();
        static      void                        setConfigFileManagementConfigCmdPath                    (const string &configFileManagementConfigCmdPath);

        static      string                      getConfigFileManagementScriptsDirectory                 ();
        static      void                        setConfigFileManagementScriptsDirectory                 (const string &configFileManagementScriptsDirectory);

        static      string                      getConfigFileManagementDefaultClientEnvironmentSettings ();

        static      string                      getConfigFileManagementDefaultClientOptions             ();

        static      string                      getConfigFileManagementGlobalOnlyFilename               ();
        static      string                      getConfigFileManagementLocalOnlyFilename                ();

        static      SI32                        configFileManagementClientCommandOutput                 (const string &configFileManagementClientCommand, vector<string> &configFileManagementClientCommandOutput);
        static      SI32                        configFileManagementConfigCmdOutput                     (const string &configFileManagementConfigCmd, vector<string> &configFileManagementConfigCmdOutput);

        static      ResourceId                  copyRunningToStartup                                    ();
        static      ResourceId                  copyRunningToStartupFile                                ();

        static      bool                        isFileExisting                                          (const string &fullFilePath);

        static      ResourceId                  replayConfigurationFile                                 (const string &configFilePath, const string &configFilename, const string &preReplayActionCommands);
        static      ResourceId                  renameConfigurationFile                                 (const string &oldConfigFile, const string &newConfigFile);
        static      ResourceId                  deleteConfigurationFile                                 (const string &fullFilePath);
        static      void                        getDifference                                           (const vector<string> &set1, const vector<string> &set2, vector<string> &set1MinusSet2Difference);

       // Now the data members

    private :
    protected :
    public :
};

}

#endif // CONFIGFILEMANAGEMENTTOOLKIT
