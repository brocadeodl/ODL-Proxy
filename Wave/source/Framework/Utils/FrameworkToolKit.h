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

#ifndef FRAMEWORKTOOLKIT
#define FRAMEWORKTOOLKIT

#include "Framework/Types/Types.h"
#include "Framework/LocationManagement/LocationBase.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Cluster/Local/WaveHaNodeTypes.h"
#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

class FrameworkToolKit
{
    private :
        static void       initialize                  ();
        static void       setIsRunningAsADaemon       (const bool &isRunningAsADaemon);
        static UI32       printNumberOfCpus           (UI32 argc, vector<string> argv);
        static UI32       debugResourceId             (UI32 argc, vector<string> argv);
        static UI32       debugAllResourceEnums       (UI32 argc, vector<string> argv);
        static ResourceId debugSavePrismConfiguration (UI32 argc, vector<string> argv);
        static ResourceId printServices               (UI32 argc, vector<string> argv);
        static ResourceId printRemoteLocations        (UI32 argc, vector<string> argv);
        static ResourceId assertTest                  (UI32 argc, vector<string> argv);
        static ResourceId sigSegvTest                 (UI32 argc, vector<string> argv);
        static ResourceId debugMessageLeak            (UI32 argc, vector<string> argv);
        static ResourceId debugObjectLeak             (UI32 argc, vector<string> argv);
        static ResourceId debugObjectLeakAll          (UI32 argc, vector<string> argv);
        static ResourceId analyzeUri                  (UI32 argc, vector<string> argv);
        static void       registerDebugShellEntries   ();

        static void       setManagementInterfaceRole  (const WaveManagementInterfaceRole &managementInterfaceRole);

        static void       setNumberOfCpus             (const UI32 &numberOfCpus);

    protected :
    public :
        static       string                      getPrismConnectionPassPhrase               ();
        static       string                      getPrismHaConnectionPassPhrase             ();
        static const SI32                        getPrismDefaultTcpPort                     ();
        static const SI32                        getPrismDefaultDatabaseTcpPort             ();
        static const bool                        getIsRunningAsADaemon                      ();
        static const bool                        isFrameworkReadyToBoot                     ();
        static const string                      getAPublicIpAddressForThisMachine          ();
        static const string                      getLoopBackIpAddressForThisMachine         ();
        static const string                      getIpAddressForInterface                   (const string &interfaceName);
        static       LocationBase               *getPThisLocation                           ();
        static const UI32                        getMyLocationId                            ();
        static const UI32                        getThisLocationId                          ();
        static const string                      getThisLocationIpAddress                   ();
        static const SI32                        getThisLocationPort                        ();
        static const LocationRole                getThisLocationRole                        ();
        static const LocationId                  getClusterPrimaryLocationId                ();
        static const string                      getClusterPrimaryIpAddress                 ();
        static const SI32                        getClusterPrimaryPort                      ();
        static const string                      getServiceNameById                         (const PrismServiceId &id);
        static const PrismServiceId              getServiceIdByName                         (const string &serviceName);
        static       LocationId                  getLocationIdForIpAddressAndPort           (const string &ipAddress, const SI32 &port);
        static       string                      getIpAddressForLocationId                  (const LocationId &locationId);
        static       SI32                        getPortForLocationId                       (const LocationId &locationId);
        static const bool                        isThisALocation                            ();
        static const bool                        isThisASubLocation                         ();
        static const bool                        isAKnownLocation                           (const string &ipAddress, const SI32 &port);
        static const bool                        isAKnownLocation                           (const LocationId &locationId);
        static const bool                        isAConnectedLocation                       (const LocationId &locationId);
        static       void                        getKnownRemoteLocations                    (vector<LocationId> &remoteLocationsVector);
        static       void                        getKnownLocations                          (vector<LocationId> &knownLocationsVector);
        static       void                        getConnectedLocations                      (vector<LocationId> &connectedLocationsVector);
        static       void                        getFullyConnectedLocations                 (vector<LocationId> &connectedLocationsVector);
        static       void                        getKnownSubLocations                       (vector<LocationId> &kownSubLocationsVector);
        static const bool                        isAnIpAddressForThisMachine                (const string &ipAddress);
        static const string                     &getProcessInitialWorkingDirectory          ();
        static const string						 getProcessCurrentWorkingDirectory		    ();
        static const string                      getTraceClientNameById                     (const TraceClientId traceClientId);
        static const bool                        isAResourceId                              (ResourceId resourceId);
        static const bool                        isAResourceEnum                            (ResourceEnum resourceEnum);
        static const string                      localize                                   (const ResourceId &resourceId);
        static const string                      localize                                   (const ResourceId &resourceId, const vector <Attribute *> &arguments);
        static const string                      localizeToSourceCodeEnum                   (ResourceId resourceId);
        static       void                        getResourceIdsForResourceEnum              (const ResourceEnum &resourceEnum, vector<ResourceId> &resourcesVector);
        static const ResourceId                  localizeToSourceCodeResourceId             (const string &sourceCodeEnum);
        static const ResourceId                  savePrismConfiguration                     (const bool &syncToStandby = false);
        static const ResourceId                  getPrismConfigurationValidity              (bool &validity);
        static const ResourceId                  changePrismConfigurationValidity           (const bool &validity);
        static const bool                        isALocalService                            (const PrismServiceId &prismServiceId);
        static const bool                        isPrimaryLocation                          ();
        static const bool                        isStandAloneLocation                       ();
        static const SI32                        getManagementInterfaceReceiverPort         ();
        static       void                        setManagementInterfaceReceiverPort         (const SI32 &managementInterfaceReceiverPort);
        static const SI32                        getManagementInterfaceClientReceiverPort   ();
        static       void                        setManagementInterfaceClientReceiverPort   (const SI32 &managementInterfaceClientReceiverPort);
        static const string                      getDatabaseBackupFileName                  ();
        static const string                      getDatabaseBackupFileName2                 ();
        static const string                      getDatabaseBackupFileName3                 ();
        static const string                      getDatabaseBackupFileForFwdl               ();
        static       void                        setDatabaseBackupFileForFwdl               (const string &dbDumpFile);
        static       ObjectId                    getObjectIdForSlotNumber                   (const UI32 slotNumber, const LocationId locationId = 0);
        static       ObjectId                    createDummySlot                            (const UI32 slotNumber);
        static       ObjectId                    getObjectIdForLocationId                   (LocationId locationId = 0);

        static const string                      getUniqueString                            (const string &ipAddress, const UI32 &port);

        static       void                        getServices                                (vector<string> &serviceNames, vector<UI32> &serviceIds);

        static       WaveManagementInterfaceRole getManagementInterfaceRole                 ();

        static       TraceLevel                  getDefaultTraceLevel                       ();
        static       void                        setDefaultTraceLevel                       (const TraceLevel &defaultTraceLevel);
        static       void                        setDefaultTraceLevelForClient              (const TraceLevel &defaultTraceLevelForClient);

        static       UI32                        getNumberOfCpus                            ();

        static       ObjectId                    getThisWaveNodeObjectId                    ();

        static       bool                        isIpAddressValid                           (const string &ipAddress);
        static       void                        setIsStartupValid                          (const bool &isValid);
        static       bool                        getIsStartupValid                          ();
        static       void                        setStartupFileName                         (const string &startupFileName);
        static       string                      getStartupFileName                         ();
        static       string                      getStartupBackupFileName                   ();
        static       bool                        isStartupConfigBackupFileExist             ();
        static       void                        setStartupFileType                         (const ResourceId &startupFileType);
        static       ResourceId                  getStartupFileType                         ();
        static       SI32                        systemCommandOutput                        (const string &command, vector<string> &output);

        static       LocationId                  getLastUsedLocationId                      ();
        static       void                        setLastUsedLocationId                      (const LocationId &locationId);
        static const bool                        getSecondaryClusterFormationFlag           ();

        static       void                        incrementLineCardPostBootCount             ();
        static       UI32                        getNumberOfLineCardPostBootCurrentlyRunning();
        static       void                        decrementLineCardPostBootCount             ();

        static       void                        disableFileServiceUse                      (void);
        static       void                        enableFileServiceUse                       (void);
        static const bool                        isFileServiceEnabled                       (void);

        static       bool                        isActivationCompleted                      ();
        static     string                        getActivationCompletedTimeStamp            ();

        static       bool                        isWarmHaRecoveryPreparationInProgress      ();

        static       bool                        isPostBootStarted                          ();
        static     string                        getPostBootStartTimeStamp                  ();

        static       bool                        isPostBootCompleted                        ();
        static     string                        getPostBootCompleteTimeStamp               ();

        static       bool                        isConfigReplayStarted                      ();
        static     string                        getConfigReplayStartedTimeStamp            ();

        static       bool                        isConfigReplayCompleted                    ();
        static     string                        getConfigReplayCompletedTimeStamp          ();

        static       bool                        isConfigReplayInProgress                   ();
        static       bool                        isNodeReadyForAllCommands                  ();
        static     string                        getNodeReadyForAllCommandsTimeStamp        ();

        static      void                         obtainBtString                             (const int &startFrame, const int &numberOfFrames, string &btString);
        static      string                       getSymbolicLinkToStartupFile               ();

        static      void                         setFileReplayStarted                       (bool fileReplayStarted);
        static      void                         setFileReplayCompleted                     (bool fileReplayCompleted);
        static      bool                         isFileReplayInProgress                     ();

        static      void                         setDetectSchemaChange                      (const string &setSchemaChange);
        static      bool                         getDetectSchemaChange                      ();
        static 		bool						 isNodeZeroized 	                        ();
        static 		ResourceId                   getDbConversionStatus 	                    ();
        static		void						 secureDeleteDir	                        (const string &dirName);
        static		void						 secureClearFile	                        (const string &fileName, bool removeOnClear = false);
        static	    void 	                     disconnectFromAllInstanceClients           (const string &clientIpAddress);
        static      void                         getConnectedInstances                      (vector<SI32> &connectedInstancesVector);
        static      void                         getConnectedInstancesAndSubInstances       (vector<SI32> &connectedInstanceVector, vector<SI32> &connectedSubInstanceVector);
        static      SI32                         getThisSlotInstance                        ();
        static      SI32                         getHttpInterfaceReceiverPort               ();
        static      void                         setHttpInterfaceReceiverPort               (const SI32 &httpInterfaceReceiverPort);
        static      SI32                         getHaInterfaceReceiverPort                 ();
        static      void                         setHaInterfaceReceiverPort                 (const SI32 &haInterfaceReceiverPort);

		static 		SI32 						 getHaPeerInterfaceReceiverPort 		    (); // ha receiver port of remote receiver interface
		static		void 						 setHaPeerInterfaceReceiverPort             (const WaveNs::SI32& haPeerInterfaceReceiverPort);

        static     WaveHaNodeRole                getThisWaveHaNodeRole                      ();

        static      void                         setIsDbRestoreIncomplete                   (bool isRestoreIncomplete);
        static      bool                         getIsDbRestoreIncomplete                   ();
        static      void                         rollbackCfgFile                            ();
        static      void                         setIsConfigFileReplayGoingToBeDone         (const bool &isConfigFileReplayGoingToBeDone);
        static      bool                         getIsConfigFileReplayGoingToBeDone         ();
        static      void                         broadcastStartupSchemaChangeEvent          (SchemaType m_fromSchema, string fromSchemaPath = "", SchemaType                                                                                              toSchema = STARTUP_SCHEMA, string toSchemaPath = "", bool operationStatus = true);
        static      void                         setHandleDBCorruptionFlag                  (const string &handleDbCorruption);
        static      bool                         getHandleDBCorruptionFlag                  ();

        static      ResourceId                   addXPathStrings                            (const vector<string> &xPathStrings);
        static      ResourceId                   getLastUpdateTimestampsForXPathStrings     (vector<string> &xPathStrings, vector<UI64> &lastUpdatedTimestamps);
        static      void                         initializeConsolePrintFunction             ();
        static      void                         consolePrint                               (const string &stringToBePrinted);
        static      void                         uninitializeConsolePrintFunction           ();
        static      bool                         getIsSyncDumpCompleted                     ();

        static      void                         getAllFilesInTheDirectory                  (const string &directoryPath, vector<string> &filePaths, const string &extensionFilter = "");

        static      SI32                         getCommandLineInterfaceReceiverPort        ();
        static      void                         setCommandLineInterfaceReceiverPort        (const SI32 &commandLineInterfaceReceiverPort);

        static      string                       getHostNameForThisMachine                  ();

        static      string                       getCurrentTtyName                          ();

        static      void                         initializeEnvironment                      ();

        static      ResourceId                   isConnectedToHaPeerLocation                ();
        static      LocationId                   getHaPeerLocationId                        ();

        static      bool                         getIsLiveSyncEnabled                       ();
        static      void                         disableLiveSync                            ();
        static      UI32                         getSyncState                               ();
        static      void                         setSyncState                               (const UI32 &syncState);
        static      void                         notifySyncUpdateFailure                    (const ResourceId &reason);
        static      void                         notifySyncUpdateFailAndStopSync            (const ResourceId &reason);
        static      SI32                         getFirmwareDownloadCase                    ();
        static      void                         setFirmwareDownloadCase                    (const SI32 &fwdlcase);

        static      void                         updateSchemaConversionDisabledByUser       ();
        static      bool                         isSchemaConversionDisabledByUser           ();

        static      ObjectId                     getObjectIdForPartition                    (const string &partitionName);

        static      bool                         isConfigurationCompatibilityCheckRequired  ();
        static      void                         setConfigurationCompatibilityCheckRequired (const bool &checkRequired);

        static      ResourceId                   pushFilesToAllNodes                        (const vector<string> &sourceFilenames, const vector<string> &destFilenames, const UI32 &transferFlag, const bool &disconnectFromFailedNode = true, const vector<LocationId> &locationIds = vector<LocationId>() );
        static      ResourceId                   deleteFilesOnAllNodes                      (const vector<string> &filenames, const UI32 &transferFlag, const bool &disconnectFromFailedNode = true, const vector<LocationId> &locationIds = vector<LocationId>() );
        static      void                         removeDisconnectedLocationFromVector       ( vector<LocationId> &connectedLocationsVector );
        static      void                         setIsAbruptReboot                          (const bool &isAbruptReboot);
        static      bool                         getIsAbruptReboot                          ();

        static      ResourceId                   checkForExistenceOfOneMO                   (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext, const string &classToQuery);
        static      ResourceId                   getHaPeerIpAddressAndPort                  (string& ip, SI32& port);
        static      string                       getMessageVersionForHaPeer                 (const string &ipAddress, const SI32 &port);
        static      UI8                          getSerializationTypeForHaPeer              (const string &ipAddress, const SI32 &port);

        static      string                       getMessageVersionForRemoteNode             (const string &ipAddress, const SI32 &port);
        static      UI8                          getSerializationTypeForRemoteNode          (const string &ipAddress, const SI32 &port);

        static      bool                         getSyncFailureNotified                     ();
        static      void                         setSyncFailureNotified                     (const bool &syncFailureNotified);
        static      ClientStreamingSocket*       getClientStreamingSocketForRemoteLocation  (LocationId locationId);
        static const SI32                        getThisLocationSlotId                      ();
        static       UI32                        getHaId                                    ();
        static       void                        setHaId                                    (const UI32 &haId);
        static       AttributeType               getFieldTypeInATable                       (const string& tableName, const string& fieldName);
        static      string                       getSqlTypeForAFieldInATable                (const string& tableName, const string& fieldName);
        static       void                        setStartMode                               (const string &mode);
        static       ResourceId                  getStartMode                               ();

        static      void                         setRewriteStartupSchemaOnNodeReady         ();
        static      bool                         getRewriteStartupSchemaOnNodeReady         ();

        static      SI32                         getMessageBrokerPort                       ();
        static      void                         setMessageBrokerPort                       (const SI32 &messageBrokerPort);

        static      SI32                         getMessageBrokerClientPort                 ();
        static      void                         setMessageBrokerClientPort                 (const SI32 &messageBrokerPort);

        static       string                      getPrismConnectionPassPhraseForMessageBroker ();

       // Now the data members

    private :
    protected :
    public :

    friend class Wave;
    friend class WaveClient;
    friend class WaveCommandLineInterface;
    friend class WaveSystemManagement;
    friend class WaveMessageBroker;
    friend class WaveMessageBrokerClient;
};

}

#endif // FRAMEWORKTOOLKIT
