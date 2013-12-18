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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include <iostream>
#include "Framework/Core/WaveClient.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/WaveClientConfiguration.h"

using namespace WaveNs;

namespace WaveNs
{
}

extern "C" int waveClientMain (int argc, char *argv[], bool wait)
{
    bool                  isADaemon            = true;
    FrameworkSequenceType type                 = FRAMEWORK_SEQUENCE_TYPE0;
    bool                  isSysLogRequired     = true;
    SI32                  port                 = FrameworkToolKit::getPrismDefaultTcpPort ();
    bool                  isDatabaseEnabled    = false;
    SI32                  databasePort         = FrameworkToolKit::getPrismDefaultDatabaseTcpPort ();
    SI32                  managementClientPort = FrameworkToolKit::getManagementInterfaceClientReceiverPort ();

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
        managementClientPort = atoi (argv[7]);
    }

    FrameworkToolKit::setManagementInterfaceClientReceiverPort (managementClientPort);


    initializePrismSocketLayer ();

#if 0
    char hostName[257];

    if (0 == (gethostname (hostName, 257)))
    {
        cout << "Host Name           : " << hostName << endl;
    }
    else
    {
        cout << "Host Name           : " << "U K N O W N" << endl;
    }
#endif

    string ipAddress = FrameworkToolKit::getAPublicIpAddressForThisMachine ();

#if 0
    cout << "Host IP Address     : " << ipAddress << endl;
    cout << "     Port           : " << port << endl;
#endif

    PrismFrameworkObjectManager::setIpAddressForThisLocation (ipAddress);

    PrismFrameworkObjectManager::setFrameworkSequenceType (type);
    PrismFrameworkObjectManager::configureAsLocation (port);

    WaveClientConfiguration waveClientConfiguration;

    waveClientConfiguration.setApplicationName   (argv[0]);
    waveClientConfiguration.setIsADaemon         (isADaemon);
    waveClientConfiguration.setIsSysLogRequired  (isSysLogRequired);
    waveClientConfiguration.setIsDatabaseEnabled (isDatabaseEnabled);
    waveClientConfiguration.setDatabasePort      (databasePort);
    waveClientConfiguration.setNeedShell         (wait);

    WaveClient::initialize (waveClientConfiguration);

    PrismFrameworkObjectManager::bootPrism ();

    PrismFrameworkObjectManager::acquireBootSynchronizationMutex ();
    PrismFrameworkObjectManager::releaseBootSynchronizationMutex ();

    if (true == wait)
    {
        PrismFrameworkObjectManager::waitForPrismServicesToFinish ();
    }

    return EXIT_SUCCESS;
}
