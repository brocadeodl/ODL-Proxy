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

#include "Framework/Database/MultiDatabaseObjectManagerSetupServersWorker.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Database/MultiDatabaseToolKit.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "Framework/Utils/Integer.h"
#include "Framework/Database/DatabaseMultiConnection.h"
#include "SystemManagement/WaveConfigurationSegmentMap.h"

namespace WaveNs
{

MultiDatabaseObjectManagerSetupServersWorker::MultiDatabaseObjectManagerSetupServersWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
}

MultiDatabaseObjectManagerSetupServersWorker::~MultiDatabaseObjectManagerSetupServersWorker ()
{
}

void MultiDatabaseObjectManagerSetupServersWorker::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    UI32                     numberOfServerConnections = MultiDatabaseToolKit::getNumberOfServerConnections ();
    UI32                     i                         = 0;
    UI32                     j                         = 0;
    DatabaseMultiConnection *pDatabaseMultiConnection  = DatabaseMultiConnection::getInstance ();
    ResourceId               status                    = WAVE_MESSAGE_SUCCESS;

    prismAssert (NULL != pDatabaseMultiConnection, __FILE__, __LINE__);

    tracePrintf (TRACE_LEVEL_DEBUG, false, false, "MultiDatabaseObjectManagerSetupServersWorker::boot : The number of server connections is %d \n", numberOfServerConnections);

    for (i = 0; i < numberOfServerConnections; i++)
    {
        string serverName;
        string serverIpAddress;
        UI32   serverPort       = 0;

        MultiDatabaseToolKit::getServerInformationAtIndex (i, serverName, serverIpAddress, serverPort);

        WaveClientSynchronousConnection *pServerConnection = new WaveClientSynchronousConnection ("WSMD", serverIpAddress, serverPort);
        

        for (int retry = 0 ; retry < 5 ; retry++)   // try for connection 5 times each with an interval of 5 seconds 
        {                                           // this is required for first time boot when DCMd or CCMd will take little longer time to boot 

            pServerConnection->connect ("", 30);    // Try 30 times to connect

            if (true == (pServerConnection->isCurrentlyConnected ()))
            {
                break;
            }
            else 
            {
                trace (TRACE_LEVEL_INFO, "MultiDatabaseObjectManagerSetupServersWorker::boot : No connection established sleeping for 5 seconds before retry");
                sleep(5);
            }
        }    

        if (true == (pServerConnection->isCurrentlyConnected ()))
        {
            string     databaseName;
            UI32       databasePort     = 0;
            ResourceId connectionStatus = WAVE_MESSAGE_SUCCESS;

            connectionStatus = pServerConnection->getDatabaseAccessDetails (databaseName, databasePort);

            if (WAVE_MESSAGE_SUCCESS == connectionStatus)
            {
                trace (TRACE_LEVEL_INFO, "MultiDatabaseObjectManagerSetupServersWorker::boot : " + serverName + ": Database Connection Details : " + databaseName + string (":") + databasePort);

                pDatabaseMultiConnection->addServer (serverName, databaseName, databasePort);

                pDatabaseMultiConnection->connect (serverName, 30);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "MultiDatabaseObjectManagerSetupServersWorker::boot : Could not get Database Connection Details for Server " + serverName);
            }

            set<string>    allManagedObjectNames;
            vector<string> configurationSegmentNames;
            vector<string> classNamesForConfigurationSegmentNames;

            connectionStatus = pServerConnection->getAllManagedObjectNamesAndConfigurationSegmentNames (allManagedObjectNames, configurationSegmentNames, classNamesForConfigurationSegmentNames);

            if (WAVE_MESSAGE_SUCCESS == connectionStatus)
            {

                UI32 numberOfManagedObjectNames                     = allManagedObjectNames.size ();
                UI32 numberOfConfigurationSegmentnames              = configurationSegmentNames.size ();
                UI32 numberOfClassNamesForConfigurationSegmentNames = classNamesForConfigurationSegmentNames.size ();

                prismAssert (numberOfConfigurationSegmentnames == numberOfClassNamesForConfigurationSegmentNames, __FILE__, __LINE__);

                trace (TRACE_LEVEL_INFO, "MultiDatabaseObjectManagerSetupServersWorker::boot : " + serverName + ": Successfully obtained Managed Object Details.");
                trace (TRACE_LEVEL_INFO, "MultiDatabaseObjectManagerSetupServersWorker::boot :     Number Of Managed Object Names                    : " + (Integer (numberOfManagedObjectNames)).toString ());
                trace (TRACE_LEVEL_INFO, "MultiDatabaseObjectManagerSetupServersWorker::boot :     Number Of Configuration Segment Names             : " + (Integer (numberOfConfigurationSegmentnames)).toString ());
                trace (TRACE_LEVEL_INFO, "MultiDatabaseObjectManagerSetupServersWorker::boot :     Number Of Class Names Configuration Segment Names : " + (Integer (numberOfClassNamesForConfigurationSegmentNames)).toString ());

                // Currently only 2 servers are allowed and the first server registers all the MO's whereas the second server replaces the registration only for the Config Segment accessed MO's
                if (i == 0)
                {
                    set<string>::const_iterator element    = allManagedObjectNames.begin ();
                    set<string>::const_iterator endElement = allManagedObjectNames.end   ();

                    while (endElement != element)
                    {
                        pDatabaseMultiConnection->addManagedObjectToServerNameMapEntry (*element, serverName);

                        element++;
                    }
                }
                else if (i == 1)
                {
                    vector<string>::const_iterator element    = classNamesForConfigurationSegmentNames.begin ();
                    vector<string>::const_iterator endElement = classNamesForConfigurationSegmentNames.end   ();

                    while (endElement != element)
                    {
                        pDatabaseMultiConnection->addManagedObjectToServerNameMapEntry (*element, serverName);

                        element++;
                    }
                }

                // This is not required during the Compatibility mode and when Wyser takes over the shell we need to enable this code
                for (j = 0; j < numberOfClassNamesForConfigurationSegmentNames; j++)
                {
                    tracePrintf (TRACE_LEVEL_INFO, false, false, "MultiDatabaseObjectManagerSetupServersWorker::boot : %s .. %s \n", configurationSegmentNames[j].c_str (), classNamesForConfigurationSegmentNames[j].c_str ());
                    WaveConfigurationSegmentMap::addToConfigSegmentImportantInfoMap (configurationSegmentNames[j], classNamesForConfigurationSegmentNames[j], serverName, pServerConnection);
                }
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "MultiDatabaseObjectManagerSetupServersWorker::boot : Could not get Managed Object Details for Server " + serverName);
            }
        }
        else 
        {
            trace (TRACE_LEVEL_ERROR, "MultiDatabaseObjectManagerSetupServersWorker::boot : No connection established");
            delete pServerConnection;
            status = WAVE_MESSAGE_ERROR;
    	}
    
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

}
