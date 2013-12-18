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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include <iostream>
#include "Framework/Core/WaveSystemManagement.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Core/WaveMainConfiguration.h"
#include "SystemManagement/SystemManagementToolKit.h"
#include "Framework/Database/MultiDatabaseToolKit.h"

using namespace WaveNs;

namespace WaveNs
{
}

extern "C" int waveSystemManagementMain (int argc, char *argv[])
{
    bool                   isADaemon                            = true;
    FrameworkSequenceType  type                                 = FRAMEWORK_SEQUENCE_TYPE0;
    bool                   isSysLogRequired                     = true;
    SI32                   port                                 = FrameworkToolKit::getPrismDefaultTcpPort ();
    bool                   isDatabaseEnabled                    = false;
    SI32                   databasePort                         = FrameworkToolKit::getPrismDefaultDatabaseTcpPort ();
    string                 databaseLogFileName                  = DatabaseObjectManager::getDatabaseLogFileName ();
    bool                   isDatabaseLogEnabled                 = true;
    string                 databaseSharedBuffers                = DatabaseObjectManager::getDatabaseSharedBuffers ();
    SI32                   managementPort                       = FrameworkToolKit::getManagementInterfaceReceiverPort ();
    string                 ethernetInterface                    = "";
    SI32                   managementClientPort                 = FrameworkToolKit::getManagementInterfaceClientReceiverPort ();
    string                 changeDirectoryTo                    = "";
    char                  *pChangeDirectoryToArray              = new char[257];
    string                 detectSchemaChange                   = "true";
    SI32                   httpPort                             = 2301;
    SI32                   haPort                               = 3516;
    SI32                   haPeerPort                           = haPort;
    string                 handleDBCorruption                   = "true";
    string                 dbDumpOnFwdlFile                     = "";
    string                 yinPath                              = "";
    string                 wyserTagsFilePath                    = "";
    SI32                   commandLineInterfaceReceiverPort     = FrameworkToolKit::getCommandLineInterfaceReceiverPort ();
    string                 configFileManagementClientPath       = "";
    string                 configFileManagementConfigCmdPath    = "";
    string                 configFileManagementScriptsDirectory = "";
    string                 serverConnectionsString;

    prismAssert (NULL != pChangeDirectoryToArray, __FILE__, __LINE__);

    if (argc >= 2)
    {
        if (0 == (strcmp (argv[1], "false")))
        {
            isADaemon = false;
        }
    }

    if (argc >= 3)
    {
        if (0 == (strcmp (argv[2], "0")))
        {
            type = FRAMEWORK_SEQUENCE_TYPE0;
        }
        else if (0 == (strcmp (argv[2], "1")))
        {
            type = FRAMEWORK_SEQUENCE_TYPE1;
        }
    }

    if (argc >= 4)
    {
        if (0 == (strcmp (argv[3], "true")))
        {
            isSysLogRequired = true;
        }
        else
        {
            isSysLogRequired = false;
        }
    }

    if (argc >= 5)
    {
        port = atoi (argv[4]);
    }

    if (argc >= 6)
    {
        if (0 == (strcmp (argv[5], "true")))
        {
            isDatabaseEnabled = true;
        }
        else
        {
            isDatabaseEnabled = false;
        }
    }

    if (argc >= 7)
    {
        databasePort = atoi (argv[6]);
    }

    if (argc >= 8)
    {
        managementPort = atoi (argv[7]);
    }

    FrameworkToolKit::setManagementInterfaceReceiverPort (managementPort);

    if (argc >= 9)
    {
        ethernetInterface = argv[8];
    }

    if (argc >= 10)
    {
        managementClientPort = atoi (argv[9]);
    }

    if (argc >= 11)
    {
        strncpy (pChangeDirectoryToArray, argv[10], 256);
        changeDirectoryTo = pChangeDirectoryToArray;
    }

    if (argc >= 12)
    {
        detectSchemaChange = argv[11];
    }

    if (argc >= 13)
    {
        httpPort = atoi (argv[12]);
    }

    if (argc >= 14)
    {
        haPort = atoi (argv[13]);
    }

    if (argc >= 15)
    {
        haPeerPort = atoi (argv[14]);
    }

    if (argc >= 16)
    {
        if (0 == (strcmp (argv[15], "true")))
        {
            isDatabaseLogEnabled = true;
        }
        else
        {
            isDatabaseLogEnabled = false;
        }
    }

    if (argc >= 17)
    {
        databaseSharedBuffers = argv[16];
    }

    if (argc >= 18)
    {
        handleDBCorruption = argv[17];
    }

    if (argc >= 19)
    {
        dbDumpOnFwdlFile = argv[18];
    }

    if (argc >= 20)
    {
        yinPath = argv[19];
    }

    if (argc >= 21)
    {
        wyserTagsFilePath = argv[20];
    }

    if (argc >= 22)
    {
        commandLineInterfaceReceiverPort = atoi (argv[21]);
    }

    if (argc >= 23)
    {
        configFileManagementClientPath = argv[22];
    }

    if (argc >= 24)
    {
        configFileManagementConfigCmdPath = argv[23];
    }

    if (argc >= 25)
    {
        configFileManagementScriptsDirectory = argv[24];
    }

    if (argc >= 26)
    {
        serverConnectionsString = argv[25];
    }

    FrameworkToolKit::setDetectSchemaChange (detectSchemaChange);

    FrameworkToolKit::setHandleDBCorruptionFlag (handleDBCorruption);

    FrameworkToolKit::setDatabaseBackupFileForFwdl (dbDumpOnFwdlFile);

    FrameworkToolKit::setManagementInterfaceClientReceiverPort (managementClientPort);

    FrameworkToolKit::setHttpInterfaceReceiverPort (httpPort);

    FrameworkToolKit::setHaInterfaceReceiverPort (haPort);

    FrameworkToolKit::setHaPeerInterfaceReceiverPort (haPeerPort);

    FrameworkToolKit::setCommandLineInterfaceReceiverPort (commandLineInterfaceReceiverPort);

    ConfigFileManagementToolKit::setConfigFileManagementClientPath (configFileManagementClientPath);

    ConfigFileManagementToolKit::setConfigFileManagementConfigCmdPath (configFileManagementConfigCmdPath);

    ConfigFileManagementToolKit::setConfigFileManagementScriptsDirectory (configFileManagementScriptsDirectory);

    MultiDatabaseToolKit::initializeServerConnections (serverConnectionsString);

    initializePrismSocketLayer ();

    string ipAddress;

    if ("" == ethernetInterface)
    {
        ipAddress = FrameworkToolKit::getAPublicIpAddressForThisMachine ();
    }
    else
    {
        ipAddress = FrameworkToolKit::getIpAddressForInterface (ethernetInterface);
    }

    if (false == isADaemon)
    {
        cout << "Running as a Daemon : " << isADaemon << endl;
        cout << "Type                : " << type << endl;

        char hostName[257];

        if (0 == (gethostname (hostName, 257)))
        {
            cout << "Host Name           : " << hostName << endl;
        }
        else
        {
            cout << "Host Name           : " << "U K N O W N" << endl;
        }

        cout << "Host IP Address     : " << ipAddress << endl;
        cout << "     Port           : " << port << endl;

        cout << "Starting Services now..." << endl;
    }

    PrismFrameworkObjectManager::setIpAddressForThisLocation         (ipAddress);
    PrismFrameworkObjectManager::setEthernetInterfaceForThisLocation (ethernetInterface);

    PrismFrameworkObjectManager::setFrameworkSequenceType            (type);
    PrismFrameworkObjectManager::configureAsLocation                 (port);

    WaveMainConfiguration waveMainConfiguration;
    vector<string>        yinPaths;

    StringUtils::tokenize (yinPath, yinPaths, ':');

    waveMainConfiguration.setApplicationName            (argv[0]);
    waveMainConfiguration.setIsADaemon                  (isADaemon);
    waveMainConfiguration.setIsSysLogRequired           (isSysLogRequired);
    waveMainConfiguration.setIsDatabaseEnabled          (isDatabaseEnabled);
    waveMainConfiguration.setPort                       (port);
    waveMainConfiguration.setDatabasePort               (databasePort);
    waveMainConfiguration.setDatabaseLogFileName        (databaseLogFileName);
    waveMainConfiguration.setIsDatabaseLogEnabled       (isDatabaseLogEnabled);
    waveMainConfiguration.setDatabaseSharedBuffers      (databaseSharedBuffers);
    waveMainConfiguration.setManagementPort             (managementPort);
    waveMainConfiguration.setEthernetInterface          (ethernetInterface);
    waveMainConfiguration.setChangeDirectoryTo          (changeDirectoryTo);
    waveMainConfiguration.setYinPaths                   (yinPaths);
    waveMainConfiguration.setWyserTagsFilePath          (wyserTagsFilePath);

    WaveSystemManagement::initialize (waveMainConfiguration);

    PrismFrameworkObjectManager::bootPrism ();

    PrismFrameworkObjectManager::waitForPrismServicesToFinish ();

    delete [] pChangeDirectoryToArray;

    return EXIT_SUCCESS;
}
