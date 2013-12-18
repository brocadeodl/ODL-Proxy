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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Leifang Hu                                            *
 ***************************************************************************/

#ifndef PRISMFRAMEWORKOBJECTMANAGERHASYNCWORKER_H
#define PRISMFRAMEWORKOBJECTMANAGERHASYNCWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Core/HaSyncWorkerSequencerContext.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/Utils/PrismMutex.h"

namespace WaveNs
{

class FrameworkObjectManagerStartHaSyncMessage;
class FrameworkObjectManagerInitHaIpAddressMessage;
class PrismFrameworkConfiguration;
class VcsClusterConfiguration;
class StartupFileConfiguration;

class PrismFrameworkObjectManagerHaSyncWorker : public WaveWorker
{
    private :
                void initHaIpAddressHandler (FrameworkObjectManagerInitHaIpAddressMessage *pFrameworkObjectManagerInitHaIpAddressMessage);
                void initHaIpAddressStep    (PrismLinearSequencerContext *pPrismLinearSequencerContext);

                void startHaSyncDumpHandler (FrameworkObjectManagerStartHaSyncMessage *pFrameworkObjectManagerStartHaSyncMessage);
                void connectToHaPeerStep    (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void ccmdConnectToHaPeerStep          (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void haSyncCollectValidationDataStep  (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void haSyncValidateVersionStep        (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void haSyncValidateVersionCallback    (FrameworkStatus frameworkStatus, FrameworkObjectManagerGetFirmwareVersionMessage *pFrameworkObjectManagerGetFirmwareVersionMessage, void *pContext);
                void haSyncCreateStandbyMessageStep   (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void haSyncGetStartupDataStep         (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void haSyncGetVcsClusterDataStep      (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void haSyncGetConfigurationFileStep   (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void haSyncCreateDatabaseDumpStep     (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void haSyncGetValidationDetailsStep   (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void haSyncSendDatabaseDumpStep       (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void haSyncSendDatabaseDumpCallback   (FrameworkStatus frameworkStatus, PrismHaSyncConfigureStandbyMessage *pPrismHaSyncConfigureStandbyMessage, void *pContext);
                bool isServiceToBeExcludedInHaSyncCommunications (const PrismServiceId &prismServiceId);
                void createDatabaseBackupBuffer       (char * &pDatabaseBackupBuffer, SI32 &sizeOfBackupFile);
                void resumeDatabase                   ();
                void haSyncGetValidationResultsStep   (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void setSyncCompletionStatusStep      (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void haSyncFailedStep                 (StartHaSyncDumpContext *pPrismLinearSequencerContext);
                void configureStandbyHandler               (PrismHaSyncConfigureStandbyMessage *pPrismHaSyncConfigureStandbyMessage);
                void removePreviousDatabaseBackupFile      (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void configureStandbyValidateVersionStep   (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void configureStandbyValidateServicesStep  (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void configureStandbyShutdownServicesStep  (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void standbyConnectToHaPeerStep            (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void configureStandbyBootServicesPrePhaseStep   (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void configureStandbyEmptyDatabaseStep     (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void configureStandbyLoadDatabaseFromActiveDatabaseStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void configureStandbyConvertDatabaseStep   (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void loadPrismConfigurationStep            (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void savePrismConfigurationStep            (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void configureStandbyUpdatePrismConfigurationFileStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void configureStandbyServicesPostPhaseStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
               	void configureStandbyUpdateVcsConfigStep   (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
               	void configureStandbyUpdateStartupFileStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
               	void copyStartupSchemaStep                 (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
               	void updateInstanceIdStep                  (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
               	void copyRunningStartupStep                (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void configureStandbySetInSyncStep         (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);

                void startHaSyncUpdateHandler              (FrameworkObjectManagerHaSyncUpdateMessage *pFrameworkObjectManagerHaSyncUpdateMessage);
                void haSyncUpdateValidationStep            (StartHaSyncDumpContext *pPrismLinearSequencerContext);

                void getFirmwareVersionHandler             (FrameworkObjectManagerGetFirmwareVersionMessage *pFrameworkObjectManagerGetFirmwareVersionMessage);
               	void sendFirmwareVersionStep               (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext);
                void sendFirmwareVersionCallback           (FrameworkStatus frameworkStatus, FrameworkObjectManagerGetFirmwareVersionMessage *pFrameworkObjectManagerGetFirmwareVersionMessage, void *pContext);

                void startCcmdHaSyncDumpHandler            (FrameworkObjectManagerStartCcmdHaSyncMessage *pFrameworkObjectManagerStartCcmdHaSyncMessage);
                void startCcmdHaSyncUpdateHandler          (FrameworkObjectManagerCcmdHaSyncUpdateMessage *pFrameworkObjectManagerCcmdHaSyncUpdateMessage);
                void ccmdHaSyncUpdateValidationStep        (StartHaSyncDumpContext *pPrismLinearSequencerContext);

               	string getFirmwareVersion                  ();
               	void   setFirmwareVersion                  (const string &firmwareVersion);
               	void   setSyncDumpComplete                 (const bool &syncDumpComplete);
                bool   getFirmwareVersionMatch             ();
                void   setFirmwareVersionMatch             (const bool &firmwareVersionMatch);
               	void   enableLiveSync                      ();
               	void   setSyncState                        (const UI32 &syncState);
                void   setStandbySyncState                 (const UI32 &syncState);
                void   setSyncServicedAsStandby            (const bool &syncServicedAsStandby);           
                void   resumeLocalPersistence              ();
                void   pauseLocalPersistence              ();
    protected :
    public :
                PrismFrameworkObjectManagerHaSyncWorker (WaveObjectManager *pWaveObjectManager);
        virtual ~PrismFrameworkObjectManagerHaSyncWorker ();
        static  PrismFrameworkObjectManagerHaSyncWorker *getInstance ();
                PrismMessage *createMessageInstance (const UI32 &operationCode);

               	bool getSyncDumpComplete                  ();
                void initializeHaPeerServerCommunications ();
                bool acceptHaPeerConnection (ServerStreamingSocket &newSocket);
                ResourceId connectToHaPeer (const string &ipAddress, const SI32 &port);
                void disconnectFromHaPeer (const string &ipAddress, const SI32 &port, const bool &closePeerServerSocket = true);
                ResourceId setFrameworkConfigurationFromConfigurationFile (const PrismFrameworkConfiguration &prismFrameworkConfiguration);
                void endHaSyncHandler (FrameworkObjectManagerEndHaSyncMessage *pFrameworkObjectManagerEndHaSyncMessage);
                void setSyncStateHandler (FrameworkObjectManagerSetSyncStateMessage *pFrameworkObjectManagerSetSyncStateMessage);
                void getSyncStateHandler (FrameworkObjectManagerGetSyncStateMessage *pFrameworkObjectManagerGetSyncStateMessage);
                UI32 getStandbySyncState                 ();
                bool getSyncServicedAsStandby            ();
                UI32 getSyncState                        ();
    // Now the Data Members
    private :
        string     m_firmwareVersion;
        bool       m_syncDumpComplete;
        bool       m_firmwareVersionMatch;
        PrismMutex m_syncDumpCompleteMutex;
        UI32       m_syncState;
        PrismMutex m_syncStateMutex;
        UI32       m_standbySyncState;
        PrismMutex m_standbySyncStateMutex;
        bool       m_syncServicedAsStandby;
        PrismMutex m_syncServicedAsStandbyMutex;
        bool       m_isDbDropNeeded;
        UI32       m_dbSchemaType;
        UI32       m_myHaVersion;
        UI32       m_peerHaVersion;

    protected :
    public :
               friend class PrismFrameworkObjectManager;
               friend class PrismFrameworkConfigurationWorker;

};

}

#endif // PRISMFRAMEWORKOBJECTMANAGERHASYNCWORKER_H
