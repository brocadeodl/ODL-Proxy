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

/**
 *@file VcsFabConfigSnapshotWorker.h 
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc. All
 * rights reserved.
 * Description: This file declares the
 *              Worker class which is responsible for processing
 *              Create and Restore Snapshot request
 *         
 *              
 * Author :     Anil ChannaveeraSetty 
 *  
 */
#ifndef VCSFABCONFIGSNAPSHOTWORKER_H
#define VCSFABCONFIGSNAPSHOTWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Core/WaveNodesAdditionToClusterCompletedEvent.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsFabConfigSnapshotMessage;
class VcsCopySnapshotFromFtpMessage;
class VcsFabConfigSnapshotContext;

class VcsFabConfigSnapshotWorker : public WaveWorker
{
    private :
                                        VcsFabConfigSnapshotWorker                  ( WaveObjectManager* pWaveObjectManager );

                    void                listenForEvents                             (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

                    PrismMessage       *createMessageInstance                       ( const UI32 &operationCode);

                    void                configSnapshotCreateMessageHandler          ( VcsFabConfigSnapshotMessage *pVcsFabConfigSnapshotMessage );
                    void                configSnapshotCreateValidationStep          ( VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext );
                    void                configSnapshotCreateConfigFilePushToAllStep ( VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext );
                    
                    void                configSnapshotRestoreMessageHandler         ( VcsFabConfigSnapshotMessage *pVcsFabConfigSnapshotMessage );
                    void                configSnapshotRestoreValidationStep         ( VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext );
                    void                configSnapshotRestoreSendMessageToNodeStep  ( VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext );
                    void                configSnapshotRestoreCleanupNodeConfig      ( VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext );
                    void                configSnapshotDeleteMessageHandler          ( VcsFabConfigSnapshotMessage *pVcsFabConfigSnapshotMessage );
                    void                configSnapshotDeleteConfigFileFromAllStep   ( VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext );    

                    void                configSnapshotCopyFromFtpMessageHandler     ( VcsCopySnapshotFromFtpMessage *pVcsCopySnapshotFromFtpMessage );
                    void                configSnapshotCopyFtpFilePushToAllStep      ( VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext );
     
                    void                configSnapshotDeleteValidationStep          ( VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext );   
                    void                WaveNodesAdditionToClusterCompletedEventHandler         (const WaveNodesAdditionToClusterCompletedEvent* &pEvent);    
     
        static      void                createSnapshotDirectory                     ();
        static      ResourceId          setConfigSnapshotReplayFile                 ( const string &snapshotLocalFileName, const string &snapshotGlobalFilename);
        static      void                deleteAllConfigSnapshotFiles                ( const bool &includeBackupFile = true );   
        static      void                deleteAllConfigSnapshotFilesForDomainId     ( const UI32 &selectedRbridgeId, const bool &syncOnAllNodes = false );
                    ResourceId          retrieveNodeSpecificConfig                  ( const UI32 &rbridgeId, string &configData, DateTime &configCreatedTime, bool isLocalOnlyConfig=true );
      
    protected :

    public :
        virtual                        ~VcsFabConfigSnapshotWorker                  ();

        static      string              getConfigSnapshotGlobalFilenameSuffix       ();
        static      string              getConfigSnapshotLocalFilenameSuffix        ();
        static      string              getConfigSnapshotDirectoryname              ();
        static      string              getConfigSnapshotFilenameExtension          ();
        static      string              getConfigSnapshotBackupFilename             ();
        static      char                getConfigSnapshotFilenameSeparator          ();
        static      void                getConfigSnapshotCompleteFileNames          ( const string &snapshotID, const UI32 &rbridgeId, string &localFilename, string &globalFilename );
        static      void                getConfigSnapshotFileNames                  ( const string &snapshotID, const UI32 &rbridgeId, string &localFilename, string &globalFilename );
        static      void                getAllConfigSnapshotFilenames               ( vector<string> &configSnapshotFilenames, const bool &completePath = true );
        static      void                getAllConfigSnapshotFilenamesForDomainId    ( const UI32 &selectedRbridgeId, set<string> &configSnapshotFilenames, vector<string> &snapshotIds );
        static      ResourceId          checkIfSnapshotFilesPresent                 ( const string &snapshotId, const UI32 &rbridgeId, UI32 &countOfSnapshotsForSelectedRbridgeId, vector<string> &localFilenames, vector<string> &globalFilenames);
        static      ResourceId          checkIfSnapshotBackupFilePresent            ();
        static      UI32                getMaxSnapshotsPerRbridgeId                 (); 
        static      void                parseConfigSnapshotFilenames                ( const vector<string> &configSnapshotFilenames, map <UI32, set<string> > &rbridgeIdSnapshotIdsMap );
        static      void                getRestoreTriggeredConfigSnapshotFilenameForDomainId    ( const UI32 &bridgeId, string &configSnapshotFilename );

    // Now the data memebers

    private :
        static      string              s_configSnapshotFilenameExtension;
        static      string              s_configSnapshotLocalFilenameSuffix;
        static      string              s_configSnapshotGlobalFilenameSuffix;
        static      string              s_configSnapshotDirectoryname;
        static      string              s_configSnapshotBackupFilename;
        static      char                s_configSnapshotFilenameTokenSeparator;
        static      UI32                s_maxSnapshotsPerRbridgeId;    
                    PrismMutex          m_configSnapshotMutex;
    protected :
    public :
                
        friend class VcsFabObjectManager;
        friend class VcsNodeConfigSnapshotWorker;
};

}
 
#endif // VCSFABCONFIGSNAPSHOTWORKER_H
