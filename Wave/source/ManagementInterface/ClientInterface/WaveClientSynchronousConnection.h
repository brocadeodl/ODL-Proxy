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

#ifndef WAVECLIENTSYNCHRONOUSCONNECTION_H
#define WAVECLIENTSYNCHRONOUSCONNECTION_H

#include "Framework/Types/Types.h"
#include "Framework/Types/IpV4Address.h"
#include "Regression/RegressionTestEntry.h"
#include "Cluster/Local/HeartBeat/HeartBeatTypes.h"

#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

class DistributedDebugParameters;
class ManagementInterfaceMessage;

typedef void (*WaveClientMessageCallbackHandler) (UI32 callbackStatus, const void *pOutput, void *pInputContext);

class WaveClientSynchronousConnection
{
    private :
    protected :
    public :
                              WaveClientSynchronousConnection                         (const string &waveClientName, const string &waveServerIpAddress, const UI32 &waveServerPort);
                              WaveClientSynchronousConnection                         ();
                             ~WaveClientSynchronousConnection                         ();

        bool                  isALocalConnection                                      () const;

        WaveConnectionStatus  connect                                                 (const string &waveClientNameExtension = "", const UI32 &numberOfMaxRetries = 15);
        void                  close                                                   ();

        UI32                  getTimeout                                              () const;
        void                  setTimeout                                              (const UI32 &timeout);

        UI32                  getWaveServerId                                         () const;

        WaveConnectionStatus  getConnectionStatus                                     () const;

        bool                  isCurrentlyConnected                                    () const;

        WaveMessageStatus     sendOneWayToWaveServer                                  (ManagementInterfaceMessage *pManagementInterfaceMessage, const LocationId &locationId = 0);
        WaveMessageStatus     sendSynchronouslyToWaveServer                           (ManagementInterfaceMessage *pManagementInterfaceMessage, const LocationId &locationId = 0);

        WaveMessageStatus     sendToWaveServer                                        (ManagementInterfaceMessage *pManagementInterfaceMessage, WaveClientMessageCallbackHandler messageCallback, void *pInputContext = NULL, UI32 timeOutInMilliSeconds = 0, LocationId locationId = 0, PrismElement *pPrismMessageSender = NULL);
        WaveMessageStatus     sendToWaveServer                                        (ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler messageCallback, PrismElement *pPrismMessageSender, void *pInputContext = NULL, UI32 timeOutInMilliSeconds = 0);
        // Other management interface related member functions.

        string                getServiceNameById                                      (const PrismServiceId &prismServiceId) const;
        PrismServiceId        getServiceIdByName                                      (const string &prismServiceName) const;

        ResourceId            getListOfTestServices                                   (vector<RegressionTestEntry> &testServices);
        ResourceId            prepareTestForAService                                  (const PrismServiceId &prismServiceId, const vector<string> &inputStrings);
        ResourceId            runTestForAService                                      (const PrismServiceId &prismServiceId, const UI32 &numberOfTimesToRunTheTest = 1);

        ResourceId            getServicesInformation                                  (vector<PrismServiceId> &serviceIds, vector<string> &serviceNames, vector<bool> &enabledStates, vector<bool> &localServiceStates, vector<string> &cpuAffinities);

        ResourceId            setCpuAffinityForService                                (const PrismServiceId &prismServiceId, const vector<UI32> &cpuAffinityVector);
        void                  setWaveServerIpAddress                                  (const string &waveServerIpAddress);
        string                getWaveServerIpAddress                                  () const;
        // Clustering related API.

        ResourceId            createCluster                                           (const vector<string> &serverIpAddresses, const vector<SI32> &serverPorts);
        ResourceId            getHeartBeatStats                                       (IpV4Address dstIpAddress, UI16 dstPortNumber, HeartBeatStat &heartBeatStat);
        // Wave Management Interface related API

        ResourceId            testWaveManagementInterface                             ();
        ResourceId            testAsynchronousWaveManagementInterface                 ();
        ResourceId            testAsynchronousMultipleResponseWaveManagementInterface ();
        ResourceId            testSendToWaveClientsManagementInterface                (vector<string> arguments);

        // software Management related API

        ResourceId            getVersion                                              (string &version);

        // Debug Tracing related API

         ResourceId           debugPrint                                              (const string &debugMessage);

        // Trace related API

         ResourceId           getListOfTraceServices                                  (vector<TraceClientId> &traceClientIDs,  vector<TraceLevel> &traceLevel, vector<string> &serviceNames);
         ResourceId           setTraceLevelForAService                                (TraceClientId &traceClientID, TraceLevel &traceLevel);
         ResourceId           setTraceLevelForAllServices                             (TraceLevel &traceLevel);

        // Distributed Log related API

        ResourceId            addLogEntry                                             (const vector<string> &arguments);
        ResourceId            updateMaxLogEntries                                     (const vector<string> &arguments);

        // Show Related API.

        ResourceId            showManagedObjects                                      (const string &managedObjectClassName, const string &schemaName, UI32 &numberOfManagedObjects, vector<string> &managedObjects);
        ResourceId            showConfigurationByQualifiedYangPath                    (const string &qualifiedYangPath, const string &schemaName);

        // DB Compare API

        ResourceId            compareDBWithPrimary                                    ();

        // Message History (debug) related functions-
        ResourceId            dumpMessageHistoryOfAService                            (const PrismServiceId &prismServiceId);
        ResourceId            configMessageHistoryOfAService                          (const PrismServiceId &prismServiceId, bool messageHistoryState, const UI32 &messageHistorySize);

        //Lock Management related functions
        ResourceId            acquireLock                                             (const string &serviceString);
        ResourceId            releaseLock                                             (const string &serviceString);
        // FIPS suspend DCMd
        ResourceId			  FIPSZeroize                                             ();

        // Debug Information related API
        ResourceId            getDebugInformation                                     (const string &serviceName, string &debugInformation);
        ResourceId            resetDebugInformation                                   (const string &serviceName);

        // Change PrismConfiguration Validity
        ResourceId            setPrismConfigurationValid                              (const bool &validity , const string schemaFile = "");

	    // Running Debug Script on all nodes
	    ResourceId            runDebugScript                                          (DistributedDebugParameters &distributedDebugParameters);

        ResourceId            copyFile                                                (const string &sourceFileName, const string &destinationFileName, UI32 &copyFileFlag, const vector<string> &vectorOfDestinationIpAddressesAndPorts);
        ResourceId            copyFileToHaPeer                                        (const string &sourceFileName, const string &destinationFileName);

        // ORM debug functions
        ResourceId            printOrm                                                (const vector<string> &arguments);

        ResourceId            debugSchemaChange                                       (const vector<string> &arguments);

        ResourceId            addXPathStrings                                         (const vector<string> &xPathStrings);
        ResourceId            deleteXPathStrings                                      (const vector<string> &xPathStrings);
        ResourceId            getLastUpdateTimestampsForXPathStrings                  (vector<string> &xPathStrings, vector<UI64> &lastUpdatedTimestamps);
        ResourceId            resetLastUpdateTimestampsForXPathStrings                ();
        ResourceId            rebuildYangInterfaces                                   ();

        ResourceId            getAllManagedObjectNamesAndConfigurationSegmentNames    (set<string> &allManagedObjectNames, vector<string> &configurationSegmentNames, vector<string> &classNamesForConfigurationSegmentNames);
        ResourceId            getDatabaseAccessDetails                                (string &databaseName, UI32 &port);

        ResourceId            debugChangeQuerySettings                                (const bool &useRecursiveQuery, const UI32 &batchSize);

        ResourceId            getClientInformation                                    (vector<string> &serviceNames);

        ResourceId            emptyDatabase                                           (const string &emptyType, string &sqlString);
        ResourceId            getEmptyDatabaseParameter                               (UI32 &thresholdValue, UI32 &numberOfEntriesInDatabase);
        ResourceId            configureEmptyDatabaseParameter                         (const UI32 &thresholdValue);
        ResourceId            setControllerIpAddress                                  (const string &ipAddress, const SI32 &port);
    // Now the data members

    private :
        string               m_waveClientName;
        string               m_waveServerIpAddress;
        UI32                 m_waveServerPort;
        UI32                 m_timeout; // In seconds
        UI32                 m_waveServerId;
        WaveConnectionStatus m_connectionStatus;
        string               m_inputStrings;

    protected :
    public :
};

}

#endif // WAVECLIENTSYNCHRONOUSCONNECTION_H
