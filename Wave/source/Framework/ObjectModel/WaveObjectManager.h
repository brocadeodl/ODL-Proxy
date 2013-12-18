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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMOBJECTMANAGER_H
#define PRISMOBJECTMANAGER_H

#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/MultiThreading/PrismThread.h"
#include <map>
#include <vector>
#include <set>
#include <string>
#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/Utils/DatabaseQueryCursor.h"
#include "Framework/Messaging/Local/PrismEvent.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectRelationalMapping/CompositionEntry.h"
#include "Framework/ObjectRelationalMapping/RelationshipEntry.h"
#include "Framework/ObjectModel/WaveManagedObjectAsynchronousPluginContext.h"
#include "Framework/ObjectRelationalMapping/ModifiedManagedObjectSchemaDifference.h"
#include "Framework/ObjectModel/WaveManagedObjectQueryContextBase.h"
//#include "Framework/Persistence/WaveConfigurationAttributes.h"

using namespace std;

namespace WaveNs
{

class PrismThread;
class WaveWorker;
class PrismEvent;
class PrismInitializeObjectManagerMessage;
class PrismListenForEventsObjectManagerMessage;
class PrismInstallObjectManagerMessage;
class PrismEnableObjectManagerMessage;
class PrismBootObjectManagerMessage;
class PrismUpgradeObjectManagerMessage;
class PrismHaInstallObjectManagerMessage;
class PrismHaBootObjectManagerMessage;
class PrismShutdownObjectManagerMessage;
class PrismUninstallObjectManagerMessage;
class PrismDisableObjectManagerMessage;
class PrismUninitializeObjectManagerMessage;
class PrismPingObjectManagerMessage;
class PrismLinearSequencerContext;
class PrismLinearSequencerContextForUpgradePhase;
class PrismSynchronousLinearSequencerContext;
class TimerObjectManagerAddTimerMessage;
class PrismTimerExpiredObjectManagerMessage;
class WaveObjectManagerCollectValidationDataMessage;
class WaveObjectManagerValidateClusterCreationMessage;
class WaveObjectManagerSendValidationResultsMessage;
class DatabaseObjectManagerExecuteQueryMessage;
class PrismPauseObjectManagerMessage;
class PrismResumeObjectManagerMessage;
class PrismSetCpuAffinityObjectManagerMessage;
class WaveAsynchronousContextForBootPhases;
class WaveAsynchronousContextForPostbootPhase;
class WaveAsynchronousContextForShutDownPhases;
class WaveAsynchronousContextForExternalStateSynchronization;
class WaveAsynchronousContextForConfigReplayEnd;
class WaveAsynchronousContextForFileReplayEnd;
class WaveAsynchronousContextForSlotFailover;
class WaveAsynchronousContextForMultiPartitionCleanup;
class WaveAsynchronousContextForUpgradePhase;
class PrismHeartbeatFailureObjectManagerMessage;
class PrismExternalStateSynchronizationObjectManagerMessage;
class PrismConfigReplayEndObjectManagerMessage;
class PrismFileReplayEndObjectManagerMessage;
class PrismSlotFailoverObjectManagerMessage;
class PrismMultiPartitionCleanupObjectManagerMessage;
class WaveObjectManagerBackendSyncUpMessage;
class WaveObjectManagerHaSyncCollectValidationDataMessage;
class WaveObjectManagerHaSyncValidateDataMessage;
class WaveObjectManagerHaSyncSendValidationResultsMessage;

class WaveManagedObject;
class WaveManagedObjectQueryContext;
class WaveManagedObjectSynchronousQueryContext;
class WaveManagedObjectSynchronousQueryContextForDeletion;
class WaveManagedObjectSynchronousQueryContextForUpdate;
class DatabaseQueryCursor;

class WaveManagedObjectLoadOperationalDataWorker;
class PrismPostbootWorker;
class WaveSendToClusterContext;
class WaveSendToClientsContext;

class MessageHistory;
class WaveObjectManagerMessageHistoryConfigMessage;
class WaveObjectManagerMessageHistoryDumpMessage;

class WaveManagedObjectUpdateWorker;
class WaveManagedObjectCreateWorker;
class WaveManagedObjectDeleteWorker;
class WaveClientDataObjectGetWorker;
class FailoverAsynchronousContext;

class WaveDebugInformationWorker;
class WaveAsynchronousContextForDebugInformation;
class WaveObjectManagerStatisticsTracker;

class UnifiedClientBackendDetails;
class PrismLinearSequencerContextForShutdownPhase;
class WaveSendMulticastContext;
class InterLocationMulticastMessage;

class WaveObjectManagerCommitTransactionContext;
class CliBlockContext;
class WaveConfigurationAttributes;

class WaveDeliverBrokerPublishMessageWorker;

class WaveObjectManager : public PrismElement
{
    private :
        class PrismMessageResponseContext
        {
            private :
            protected :
            public :
                              PrismMessageResponseContext       (PrismMessage *pPrismMessage, PrismElement *pPrismMessageSender, PrismMessageResponseHandler pPrismMessageSenderCallback, void *pPrismMessageSenderContext);
                void          executeResponseCallback           (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, bool isMessageRecalled = false);
                void          executeResponseCallback           (FrameworkStatus frameworkStatus);
                void          setIsMessageTimedOut              (bool isMessageTimedOut);
                bool          getIsMessageTimedOut              ();
                PrismMessage *getPPrismMessage                  ();
                PrismMessage *getPInputMessageInResponseContext ();
                void          setPInputMessageInResponseContext (PrismMessage *pPrismMessage);
                bool          getIsTimerStarted                 () const;
                void          setIsTimerStarted                 (const bool &isTimerStarted);
                TimerHandle   getTimerHandle                    () const;
                void          setTimerHandle                    (const TimerHandle &timerHandle);

            // Now the data members

            private :
                PrismMessage                *m_pPrismMessage;
                PrismElement                *m_pPrismMessageSender;
                PrismMessageResponseHandler  m_pPrismMessageSenderCallback;
                void                        *m_pPrismMessageSenderContext;
                bool                         m_isMessageTimedOut;
                PrismMessage                *m_pInputMessageInResponseContext;
                bool                         m_isTimerStarted;
                TimerHandle                  m_timerHandle;

            protected :
            public :
        };

        class PrismOperationMapContext
        {
            private :
            protected :
            public :
                     PrismOperationMapContext (PrismElement *pPrismElement, PrismMessageHandler pPrismMessageHandler);
                void executeMessageHandler    (PrismMessage *&pPrismMessage);

            // Now the data members

            private :
                PrismElement        *m_pPrismElementThatHandlesTheMessage;
                PrismMessageHandler  m_pPrismMessageHandler;

            protected :
            public :
        };

        class PrismEventMapContext
        {
            private :
            protected :
            public :
                     PrismEventMapContext (PrismElement *pPrismElement, PrismEventHandler pPrismEventHandler);
                void executeEventHandler  (const PrismEvent *&pPrismEvent);

            // Now the data members

            private :
                PrismElement      *m_pPrismElementThatHandlesTheEvent;
                PrismEventHandler  m_pPrismEventHandler;

            protected :
            public :
        };

        class PrismEventListenerMapContext
        {
            private :
            protected :
            public :
                 PrismEventListenerMapContext (const PrismServiceId &eventListenerServiceId, const LocationId &eventListenerLocationId);
                ~PrismEventListenerMapContext ();

                PrismServiceId getEventListenerServiceId  () const;
                LocationId     getEventListenerLocationId () const;

            // Now the data members

            private :
                PrismServiceId m_eventListenerSericeId;
                LocationId     m_eventListenerLocationId;

            protected :
            public :
        };

        class WaveManagedObjectForUpdate
        {
            public:
                virtual         ~WaveManagedObjectForUpdate ();
                virtual void    getSqlForUpdate             (string &sql) = 0;
        };

        class WaveManagedObjectUpdateSingle: public WaveManagedObjectForUpdate
        {
            private :
            protected :
            public :
                                WaveManagedObjectUpdateSingle   (WaveManagedObject *updateObject);
                virtual         ~WaveManagedObjectUpdateSingle  ();
                        void    getSqlForUpdate                 (string &sql);
            // Now the data members
            private :
                        WaveManagedObject*  m_pUpdateWaveManagedObject;
            protected :
            public :

        };

        class WaveManagedObjectUpdateMultiple: public WaveManagedObjectForUpdate
        {
            private :
            protected :
            public :
                                WaveManagedObjectUpdateMultiple     (WaveManagedObjectSynchronousQueryContextForUpdate * pWaveManagedObjectSynchronousQueryContextForUpdate);
                virtual         ~WaveManagedObjectUpdateMultiple    ();
                        void    getSqlForUpdate                     (string &sql);
            // Now the data members
            private :
                        WaveManagedObjectSynchronousQueryContextForUpdate*  m_pWaveManagedObjectSynchronousQueryContextForUpdate;
            protected :
            public :

        };

        class WaveManagedObjectForDelete
        {
            public:
                virtual         ~WaveManagedObjectForDelete ();
                virtual void    getSqlForDelete             (string &sql) = 0;
        };

        class WaveManagedObjectDeleteSingle: public WaveManagedObjectForDelete
        {
            private :
            protected :
            public :
                                WaveManagedObjectDeleteSingle   (ObjectId deleteObjectId);
                virtual         ~WaveManagedObjectDeleteSingle  ();
                        void    getSqlForDelete                 (string &sql);
            // Now the data members
            private :
                        ObjectId m_deleteObjectId;
            protected :
            public :

        };

        class WaveManagedObjectDeleteMultiple: public WaveManagedObjectForDelete
        {
            private :
            protected :
            public :
                                WaveManagedObjectDeleteMultiple     (WaveManagedObjectSynchronousQueryContextForDeletion* pWaveManagedObjectSynchronousQueryContextForDeletion);
                                WaveManagedObjectDeleteMultiple     (WaveManagedObjectSynchronousQueryContextForDeletion* pWaveManagedObjectSynchronousQueryContextForDeletion, bool deleteOnlyCompositions, set<string> relationsSet);
                                WaveManagedObjectDeleteMultiple     ( WaveManagedObjectSynchronousQueryContextForDeletion* pWaveManagedObjectSynchronousQueryContextForDeletion,
                                                bool partialDeleteFromVectorComposition, string parentClassName, string compositionName);
                virtual         ~WaveManagedObjectDeleteMultiple    ();
                        void    getSqlForDelete                     (string &sql);
            // Now the data members
            private :
                        WaveManagedObjectSynchronousQueryContextForDeletion*  m_pWaveManagedObjectSynchronousQueryContextForDeletion;
                        bool        m_deleteOnlyCompositions;
                        set<string> m_relationsSet;
                        bool        m_partialDeleteFromVectorComposition;
                        string      m_parentClassName;
                        string      m_compositionName;
            protected :
            public :

        };

        class WaveBrokerPublishMessageHandlerContext
        {
            private :
            protected :
            public :
                                                 WaveBrokerPublishMessageHandlerContext (WaveBrokerPublishMessageHandler waveBrokerPublishMessageHandler, PrismElement *pSubscriber);
                                                ~WaveBrokerPublishMessageHandlerContext ();

                WaveBrokerPublishMessageHandler  getWaveBrokerPublishMessageHandler     ();
                PrismElement                    *getPSubscriber                         ();

            // Now the data members

            private :
                WaveBrokerPublishMessageHandler  m_waveBrokerPublishMessageHandler;
                PrismElement                    *m_pSubscriber;
            protected :
            public :
        };

                void                initializeHandler                            (PrismInitializeObjectManagerMessage *pInitializeMessage);
                void                initializeInitializeWorkersStep              (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                initializeInitializeWorkersStepCallback      (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                initializeInitializeSelfStep                 (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                initializeInitializeSelfStepCallback         (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void                initialize                                   (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

                void                listenForEventsHandler                       (PrismListenForEventsObjectManagerMessage *pListenForEventsMessage);
                void                listenForEventsWorkersStep                   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                listenForEventsWorkersStepCallback           (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                listenForEventsSelfStep                      (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                listenForEventsSelfStepCallback              (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void                listenForEvents                              (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

                void                installHandler                               (PrismInstallObjectManagerMessage *pInstallMessage);
                void                installInstallWorkersStep                    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                installInstallWorkersStepCallback            (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                installInstallSelfStep                       (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                installInstallSelfStepCallback               (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void                install                                      (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        static  void                addServiceToEnabledServicesList              (const PrismServiceId &prismServiceId);
        static  void                removeServiceFromEnabledServicesList         (const PrismServiceId &prismServiceId);

                void                enableHandler                                (PrismEnableObjectManagerMessage *pEnableMessage);
                void                enableEnableWorkersStep                      (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                enableEnableWorkersStepCallback              (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                enableEnableSelfStep                         (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                enableEnableSelfStepCallback                 (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void                enable                                       (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

                void                upgradeHandler                               (PrismUpgradeObjectManagerMessage *pUpgradeMessage);
                void                upgradeDefaultValueStep                      (PrismLinearSequencerContextForUpgradePhase *pPrismLinearSequencerContextForUpgradePhase);
                void                upgradeUpgradeWorkersStep                    (PrismLinearSequencerContextForUpgradePhase *pPrismLinearSequencerContextForUpgradePhase);
                void                upgradeUpgradeWorkersStepCallback            (WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase);
                void                upgradeUpgradeSelfStep                       (PrismLinearSequencerContextForUpgradePhase *pPrismLinearSequencerContextForUpgradePhase);
                void                upgradeUpgradeSelfStepCallback               (WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase);
        virtual void                upgrade                                      (WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase);

                void                bootHandler                                  (PrismBootObjectManagerMessage *pBootMessage);
                void                bootBootWorkersStep                          (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                bootBootWorkersStepCallback                  (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                bootBootSelfStep                             (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                bootBootSelfStepCallback                     (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void                boot                                         (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

                void                hainstallHandler                             (PrismHaInstallObjectManagerMessage *pHaInstallMessage);
                void                hainstallInstallWorkersStep                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                hainstallInstallWorkersStepCallback          (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                hainstallInstallSelfStep                     (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                hainstallInstallSelfStepCallback             (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void                hainstall                                    (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

                void                habootHandler                                (PrismHaBootObjectManagerMessage *pHaBootMessage);
                void                habootBootWorkersStep                        (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                habootBootWorkersStepCallback                (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                habootBootSelfStep                           (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                habootBootSelfStepCallback                   (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void                haboot                                       (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        virtual void                postboot                                     (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);
        virtual void                postbootCallback                             (GetHardwareConfigurationDetailsForPostbootContext *pGetHardwareConfigurationDetailsForPostbootContext);
        virtual ResourceId          postbootValidateAllOwnedManagedClassNames    (const string passName, vector<string> &managedObjectNamesInPass);

                void                shutdownHandler                              (PrismShutdownObjectManagerMessage *pShutdownMessage);
                void                shutdownShutdownWorkersStep                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                shutdownShutdownWorkersStepCallback          (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
                void                shutdownShutdownSelfStep                     (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                shutdownShutdownSelfStepCallback             (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        virtual void                shutdown                                     (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
                void                notifyStep                                   (PrismLinearSequencerContextForShutdownPhase *pPrismLinearSequencerContext);
                void                uninstallHandler                             (PrismUninstallObjectManagerMessage *pUninstallMessage);
                void                uninstallUninstallWorkersStep                (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                uninstallUninstallWorkersStepCallback        (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
                void                uninstallUninstallSelfStep                   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                uninstallUninstallSelfStepCallback           (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        virtual void                uninstall                                    (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);

                void                disableHandler                               (PrismDisableObjectManagerMessage *pDisableMessage);
                void                disableUnlistenEventsStep                    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                disableDisableWorkersStep                    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                disableDisableWorkersStepCallback            (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
                void                disableDisableSelfStep                       (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                disableDisableSelfStepCallback               (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        virtual void                disable                                      (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);

                void                uninitializeHandler                          (PrismUninitializeObjectManagerMessage *pUninitializeMessage);
                void                uninitializeUninitializeWorkersStep          (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                uninitializeUninitializeWorkersStepCallback  (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
                void                uninitializeUninitializeSelfStep             (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                uninitializeUninitializeSelfStepCallback     (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        virtual void                uninitialize                                 (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);

                void                destructHandler                              (PrismDestructObjectManagerMessage *pDestructMessage);
                void                destructDestructWorkersStep                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                destructDestructWorkersStepCallback          (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
                void                destructDestructSelfStep                     (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                destructDestructSelfStepCallback             (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        virtual void                destruct                                     (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);

                void                heartbeatFailureHandler                      (PrismHeartbeatFailureObjectManagerMessage *pHearbeatFailureMessage);
                void                heartbeatFailureWorkersStep                  (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                heartbeatFailureWorkersStepCallback          (PrismAsynchronousContext     *pPrismAsynchronousContext);
                void                heartbeatFailureSelfStep                     (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                heartbeatFailureSelfStepCallback             (PrismAsynchronousContext     *pPrismAsynchronousContext);
        virtual void                heartbeatFailure                             (PrismAsynchronousContext     *pPrismAsynchronousContext);

                void                configReplayEndHandler                       (PrismConfigReplayEndObjectManagerMessage *pConfigReplayEndMessage);
                void                configReplayEndWorkersStep                   (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                configReplayEndWorkersStepCallback           (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd);
                void                configReplayEndSelfStep                      (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                configReplayEndSelfStepCallback              (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd);
        virtual void                configReplayEnd                              (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd);

                void                fileReplayEndHandler                         (PrismFileReplayEndObjectManagerMessage *pFileReplayEndMessage);
                void                fileReplayEndWorkersStep                     (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                fileReplayEndWorkersStepCallback             (WaveAsynchronousContextForFileReplayEnd *pWaveAsynchronousContextForFileReplayEnd);
                void                fileReplayEndSelfStep                        (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                fileReplayEndSelfStepCallback                (WaveAsynchronousContextForFileReplayEnd *pWaveAsynchronousContextForFileReplayEnd);
        virtual void                fileReplayEnd                                (WaveAsynchronousContextForFileReplayEnd *pWaveAsynchronousContextForFileReplayEnd);

                void                slotFailoverHandler                          (PrismSlotFailoverObjectManagerMessage *pSlotFailoverMessage);
                void                slotFailoverWorkersStep                      (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                slotFailoverWorkersStepCallback              (WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover);
                void                slotFailoverSelfStep                         (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                slotFailoverSelfStepCallback                 (WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover);
        virtual void                slotFailover                                 (WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover);

                void                multiPartitionCleanupHandler                   (PrismMultiPartitionCleanupObjectManagerMessage *pMultiPartitionCleanupMessage);
                void                multiPartitionCleanupWorkersStep               (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                multiPartitionCleanupWorkersStepCallback       (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup);
                void                multiPartitionCleanupSelfStep                  (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                multiPartitionCleanupSelfStepCallback          (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup);
        virtual void                multiPartitionCleanup                          (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup);
        virtual void                multiPartitionPartialCleanup                   (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup);

                void                externalStateSynchronizationHandler                      (PrismExternalStateSynchronizationObjectManagerMessage *pExternalStateSynchronizationMessage);
                void                externalStateSynchronizationWorkersStep                  (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                externalStateSynchronizationWorkersStepCallback          (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);
                void                externalStateSynchronizationSelfStep                     (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                externalStateSynchronizationSelfStepCallback             (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);
        virtual void                externalStateSynchronization                             (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);
                void                backendSyncUpHandler                         (WaveObjectManagerBackendSyncUpMessage *pPrismHeartbeatFailureObjectManagerMessage);
                void                backendSyncUpWorkersStep                     (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                backendSyncUpWorkersStepCallback             (PrismAsynchronousContext     *pPrismAsynchronousContext);
                void                backendSyncUpSelfStep                        (PrismLinearSequencerContext  *pPrismLinearSequencerContext);
                void                backendSyncUpSelfStepCallback                (PrismAsynchronousContext     *pPrismAsynchronousContext);
        virtual void                backendSyncUp                                (PrismAsynchronousContext     *pPrismAsynchronousContext);
                void                pauseHandler                                 (PrismPauseObjectManagerMessage *pPrismPauseObjectManagerMessage);
                void                resumeHandler                                (PrismResumeObjectManagerMessage *pPrismResumeObjectManagerMessage);

                void                pingHandler                                  (PrismPingObjectManagerMessage *pPingMessage);

                void                setCpuAffinityHandler                        (PrismSetCpuAffinityObjectManagerMessage *pPrismSetCpuAffinityObjectManagerMessage);

                void                clusterCreateCollectValidationDataHandler             (WaveObjectManagerCollectValidationDataMessage *pWaveObjectManagerCollectValidationDataMessage);
                void                clusterCreateCollectValidationDataWorkersStep         (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                clusterCreateCollectValidationDataWorkersStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext);
                void                clusterCreateCollectValidationDataSelfStep            (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                clusterCreateCollectValidationDataSelfStepCallback    (PrismAsynchronousContext *pPrismAsynchronousContext);
        virtual void                clusterCreateCollectValidationData                    (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);

                void                clusterCreateValidateHandler                          (WaveObjectManagerValidateClusterCreationMessage *pWaveObjectManagerValidateClusterCreationMessage);
                void                clusterCreateValidateWorkersStep                      (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                clusterCreateValidateWorkersStepCallback              (PrismAsynchronousContext *pPrismAsynchronousContext);
                void                clusterCreateValidateSelfStep                         (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                clusterCreateValidateSelfStepCallback                 (PrismAsynchronousContext *pPrismAsynchronousContext);
        virtual void                clusterCreateValidate                                 (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);

                void                clusterCreateSendValidationResultsHandler             (WaveObjectManagerSendValidationResultsMessage *pWaveObjectManagerSendValidationResultsMessage);
                void                clusterCreateSendValidationResultsWorkersStep         (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                clusterCreateSendValidationResultsWorkersStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext);
                void                clusterCreateSendValidationResultsSelfStep            (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                clusterCreateSendValidationResultsSelfStepCallback    (PrismAsynchronousContext *pPrismAsynchronousContext);
        virtual void                clusterCreateSendValidationResults                    (WaveObjectManagerSendValidationResultsMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);

                void                failoverHandler                                       (PrismFailoverObjectManagerMessage *pPrismFailvoerObjectManagerMessage);
                void                failoverWorkersStep                                   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                failoverWorkersStepCallback                           (FailoverAsynchronousContext *pFailoverAsynchronousContext);
                void                failoverSelfStep                                      (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                failoverSelfStepCallback                              (FailoverAsynchronousContext *pFailoverAsynchronousContext);
        // virtual void                failover                                              (FrameworkObjectManagerFailoverReason failoverReason, vector<LocationId> failedLocationIds, PrismAsynchronousContext *pPrismAsynchronousContext);
        virtual void                failover                                              (FailoverAsynchronousContext *pFailoverAsynchronousContext);

                void                haSyncCollectValidationDataHandler                    (WaveObjectManagerHaSyncCollectValidationDataMessage *pWaveObjectManagerHaSyncCollectValidationDataMessage);
                void                haSyncCollectValidationDataWorkersStep                (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                haSyncCollectValidationDataSelfStep                   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                haSyncCollectValidationDataSelfStepCallback           (PrismAsynchronousContext *pPrismAsynchronousContext);
        virtual void                haSyncCollectValidationData                           (WaveObjectManagerHaSyncCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);

                void                haSyncValidateDataHandler                             (WaveObjectManagerHaSyncValidateDataMessage *pWaveObjectManagerHaSyncValidateDataMessage);
                void                haSyncValidateDataWorkersStep                         (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                haSyncValidateDataSelfStep                            (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                haSyncValidateDataSelfStepCallback                    (PrismAsynchronousContext *pPrismAsynchronousContext);
        virtual void                haSyncValidateData                                    (WaveObjectManagerHaSyncValidateDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);

                void                haSyncSendValidationResultsHandler                    (WaveObjectManagerHaSyncSendValidationResultsMessage *pWaveObjectManagerHaSyncSendValidationResultsMessage);
                void                haSyncSendValidationResultsWorkersStep                (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                haSyncSendValidationResultsSelfStep                   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                haSyncSendValidationResultsSelfStepCallback           (PrismAsynchronousContext *pPrismAsynchronousContext);
        virtual void                haSyncSendValidationResults                           (WaveObjectManagerHaSyncSendValidationResultsMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);

        virtual void                getDebugInformation                                   (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation);
        virtual void                resetDebugInformation                                 (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation);

                void                setAssociatedPrismThread                     (PrismThread *pAssociatedPrismThread);
                bool                isOperationCodeSupported                     (UI32 operationCode);
                bool                isEventOperationCodeSupported                (UI32 eventOperationCode);
                bool                isEventOperationCodeSupportedForListening    (const LocationId &eventSourceLocationId, const PrismServiceId &eventSourceServiceId, const UI32 &eventOperationCode);
                bool                isAKnownMessage                              (UI32 prismMessageId);
                void                handlePrismMessage                           (PrismMessage *pPrismMessage);
                void                handlePrismEvent                             (const PrismEvent *&pPrismEvent);
                void                handlePrismMessageResponse                   (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, bool isMessageRecalled = false);
                void                handlePrismMessageResponseWhenTimeOutExpired (FrameworkStatus frameworkStatus, UI32 prismMessageId);
                void                addWorker                                    (WaveWorker *pWorker);
                void                removeWorker                                 (WaveWorker *pWorker);
        static  bool                isEventAllowedBeforeEnabling                 (const UI32 &eventOperationCode);
        static  bool                canInstantiateServiceAtThisTime              (const string &prismServiceName);

        virtual PrismMessage       *createMessageInstance                        (const UI32 &operationCode);
                PrismMessage       *createMessageInstanceWrapper                 (const UI32 &operationCode);
        virtual PrismEvent         *createEventInstance                          (const UI32 &eventOperationCode);
        virtual WaveManagedObject  *createManagedObjectInstance                  (const string &managedClassName);
                WaveManagedObject  *createManagedObjectInstanceWrapper           (const string &managedClassName);

                void                addEventListener                             (const UI32 &eventOperationCode, const PrismServiceId &listenerPrismServiceId, const LocationId &listenerLocationId);
                void                removeEventListener                          (const UI32 &eventOperationCode, const PrismServiceId &listenerPrismServiceId, const LocationId &listenerLocationId);
                void                getEventListeners                            (const UI32 &eventOperationCode, vector<PrismEventListenerMapContext *> &eventListeners);

                void                trackObjectCreatedDuringCurrentTransaction   (WaveManagedObject *pWaveManagedObject);
                void                trackObjectDeletedDuringCurrentTransaction   (WaveManagedObject *pWaveManagedObject);

        virtual void                tracePrintf                                  (TraceLevel traceLevel, const bool &addNewLine, const bool &suppressPrefix, const char * const pFormat, va_list &variableArguments);
        virtual void                tracePrintf                                  (TraceLevel traceLevel, const char * const pFormat, va_list &variableArguments);

        virtual void                printfToAllWaveClientSessions                (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, va_list &variableArguments);

                void                computeDisconnectedNodesIfSurrogateStep      (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                sendPhase1MessageToAllNodesStep              (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                sendPhase1MessageToAllNodesCallback          (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                computeFailedNodesForPhase1Step              (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                sendPhase2MessageToAllNodesIfApplicableStep  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                sendPhase2MessageToAllNodesIfApplicableCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                computeOverallStatusStep                     (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                waveObjectManagerMessageHistoryConfigMessageHandler (WaveObjectManagerMessageHistoryConfigMessage *pWaveObjectManagerMessageHistoryConfigMessage);
                void                waveObjectManagerMessageHistoryDumpMessageHandler   (WaveObjectManagerMessageHistoryDumpMessage   *pWaveObjectManagerMessageHistoryDumpMessage);
                void                addMessageToMessageHistory                   (PrismMessage *pPrismMessage, WaveMessageHistoryLogType messageHistoryLogType);
                void                addMessageToMessageHistoryCalledFromSend     (PrismMessage *pPrismMessage);
                void                addMessageToMessageHistoryCalledFromReply    (PrismMessage *pPrismMessage);
                void                addMessageToMessageHistoryCalledFromHandle   (PrismMessage *pPrismMessage);
                bool                isServiceStringRegisteredWithService         (const string &serviceString);
                ResourceId          executeMessageReplyDuringSurrogacy           (ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler pManagementInterfaceMessageCallback, void *pManagementInterfaceMessageContext = NULL);
                void                sendPhase1MessageToAllInstancesStep          (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                sendPhase1MessageToAllInstancesCallback      (FrameworkStatus frameworkStatus, ManagementInterfaceMessage *pPrismMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                sendPhase2MessageToAllInstancesIfApplicableStep  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                sendPhase2MessageToAllInstancesIfApplicableCallback (FrameworkStatus frameworkStatus, ManagementInterfaceMessage *pPrismMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                computeFailedInstancesForPhase1Step           (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                computeOverallInstancesStatusStep            (PrismLinearSequencerContext *pPrismLinearSequencerContext);

        virtual void                updateTimeConsumedInThisThread                        (const UI32 &operationCode, const UI32 &sequencerStepIndex, const UI32 &numberOfSeconds, const SI32 &numberOfNanoSeconds);
        virtual void                updateRealTimeConsumedInThisThread                    (const UI32 &operationCode, const UI32 &sequencerStepIndex, const UI32 &numberOfSeconds, const SI32 &numberOfNanoSeconds);

                void                clearAllTimingsAccumulatedForSequencerSteps           ();

                void                getAllTimingsAccumulatedForSequencerSteps             (string &allTimingsAccumulatedForSequencerSteps);

        static  bool                isManagedObjectBeingTrackedForCreations               (const string &managedObject);
                WaveWorker         *getPWaveManagedObjectCreateWorker                     ();


               void                 addPartitionNameToSetOfPartitionNamesReferencedInCurrentTransaction (const string &partitionName);
               set<string>         &getSetOfPartitionNamesReferencedInCurrentTransaction                (void);

                void                checkIfUpgradeIsNecessary                                           (const map<string, ModifiedManagedObjectSchemaDifference*>& modifiedMoInSchema);

               ResourceId           sendOneWayForStoringConfigurationIntent                                   (PrismMessage *pPrismMessage);
               ResourceId           sendOneWayForRemovingConfigurationIntent                                  (const UI32 &configurationIntentMessageId);

               void                 deliverWaveBrokerPublishedEvent                                     (const string &brokerName, const string &topicName, WaveBrokerPublishMessage *pWaveBrokerPublishMessage);

    protected :
                bool                getIsEnabled                                 ();
                                                                 WaveObjectManager                            (const string &objectManagerName, const UI32 &stackSize = 0, const vector<UI32> *pCpuAffinityVector = NULL);
                                                                 WaveObjectManager                            (const WaveObjectManager &prismObjectManager);
        virtual                                                 ~WaveObjectManager                            ();
                string                                           getName                                      () const;
                WaveObjectManager                              &operator =                                    (const WaveObjectManager &prismObjectManager);
                void                                             addOperationMap                              (UI32 operationCode, PrismMessageHandler pMessageHandler, PrismElement *pPrismElement = NULL);
                void                                             addServiceIndependentOperationMap            (UI32 operationCode, WaveServiceIndependentMessageHandler pWaveServiceIndependentMessageHandler);
                void                                             addServiceIndependentOperationMap            (UI32 operationCode, ManagementInterfaceServiceIndependentMessageHandler pManagementInterfaceServiceIndependentMessageHandler);
                void                                             removeServiceIndependentOperationMap         (UI32 operationCode);
                void                                             removeOperationMap                           (const UI32 &operationCode);
        virtual void                                             addEventType                                 (const UI32 &eventOperationCode);
        virtual void                                             listenForEvent                               (PrismServiceId prismServiceId, UI32 sourceOperationCode, PrismEventHandler pPrismEventHandler, PrismElement *pPrismElement = NULL, const LocationId &sourceLocationId = 0);
        virtual void                                             unlistenEvents                               ();
                void                                             addResponseMap                               (UI32 prismMessageId, PrismMessageResponseContext *pPrismMessageResponseContext);
                PrismMessageResponseContext                     *removeResponseMap                            (UI32 prismMessageId);
                PrismOperationMapContext                        *getPrismMessageHandler                       (UI32 operationCode, UI32 messageHandlerServiceCode = 0, UI32 thisServiceId = 0);
                PrismEventMapContext                            *getPrismEventHandler                         (const LocationId &eventSourceLocationId, const PrismServiceId &eventSourceServiceId, const UI32 &eventOperationCode);
                WaveObjectManager::PrismMessageResponseContext *getResponseContext                            (UI32 prismMessageId);
                void                                             setIsEnabled                                 (bool isEnabled);
        virtual TraceClientId                                    getTraceClientId                             ();

                void                                             addManagedClass                              (const string &managedClassName, PrismElement *pOwnerForInstantiation = NULL);
                void                                             addManagedClassWithoutOwnership              (const string &managedClassName);
        static  void                                             addOwnerForManagedClass                      (const string &managedClassName, WaveObjectManager *pWaveObjectManager);
        static  WaveObjectManager                               *getOwnerForManagedClass                      (const string &managedClassName);
                ResourceId                                       ValidateAllOwnedManagedClassNames            ( const vector<string> &managedObjectNames );


        virtual WaveMessageStatus                                postToRemoteLocation                         (PrismMessage *pPrismMessage);
        virtual WaveMessageStatus                                postToRemoteLocation                         (InterLocationMulticastMessage *pPrismMessage, set<LocationId> locationsToSent);
        virtual WaveMessageStatus                                postToHaPeerLocation                         (PrismMessage *pPrismMessage);
        virtual WaveMessageStatus                                send                                         (PrismMessage *pPrismMessage, PrismMessageResponseHandler pPrismMessageCallback, void *pPrismMessageContext, UI32 timeOutInMilliSeconds = 0, LocationId locationId = 0, PrismElement *pPrismMessageSender = NULL);
                void                                             sendTimerExpiredCallback                     (TimerHandle timerHandle, void *pContext);
        virtual WaveMessageStatus                                sendOneWay                                   (PrismMessage *pPrismMessage, const LocationId &locationId = 0);
        virtual WaveMessageStatus                                sendOneWayToFront                            (PrismMessage *pPrismMessage, const LocationId &locationId = 0);
        virtual WaveMessageStatus                                sendSynchronously                            (PrismMessage *pPrismMessage, const LocationId &locationId = 0);
                ResourceId                                       sendSynchronouslyForBackEndAttributesMap     ( GetHardwareConfigurationDetailsForPostbootContext *pContext );
                ResourceId                                       sendSynchronouslyForAGroup                   ( GetHardwareConfigurationDetailsForPostbootContext *pContext );

        virtual bool                                             isBeingSurrogated                            ();

		/**
		 @addtogroup PartialSuccess
         @{
		 */

		/**
		 @brief sendToWaveCluster sends the message to all the nodes in the cluster. The message to be sent can be set
				in WaveSendToClusterContext. Similarly partialSuccess flag can be set to true in WaveSendToClusterContext
				before calling sendToWaveCluster.\n

         <b>Update</b>: Return status is now enhanced based on new flag m_returnSuccessForPartialSuccessFlag see below
			for more details.\n

		 @return if m_returnSuccessForPartialSuccessFlag is set then\n\n
				 WAVE_MESSAGE_SUCCES is returned if all or any node succeeds.\n
				 WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES is returned if all nodes fail.\n\n
				 If m_returnSuccessForPartialSuccessFlag is not set then\n
				 WAVE_MESSAGE_SUCCESS is returned only if all nodes succeed else returns WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES.\n

		 @see WaveSendToClusterContext
		 */
        virtual void                                             sendToWaveCluster                                  (WaveSendToClusterContext *pWaveSendToClusterContext);

        virtual void                                             sendMulticast                                      (WaveSendMulticastContext *pWaveSendMulticastContext);
                void                                             performSendMulticast                               (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                             performSendMulticastLocalCallback                  (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);
                void                                             performSendMulticastRemoteCallback                 (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);

		/**
		 @}
		 */
        virtual void                                             sendOneWayToWaveCluster                            (WaveSendToClusterContext *pWaveSendToClusterContext);

        virtual WaveMessageStatus                                recall                                             (PrismMessage *pPrismMessage);
                WaveMessageStatus                                recallButDoNotDeleteResponseMap                    (PrismMessage *pPrismMessage);
        virtual WaveMessageStatus                                reply                                              (PrismMessage *pPrismMessage);
        virtual WaveMessageStatus                                broadcast                                          (PrismEvent *pPrismEvent);
        virtual WaveMessageStatus                                reply                                              (const PrismEvent *&pPrismEvent);

        virtual void                                             trace                                              (TraceLevel traceLevel, const string &stringToTrace);
        virtual void                                             tracePrintf                                        (TraceLevel traceLevel, const bool &addNewLine, const bool &suppressPrefix, const char * const pFormat, ...);
        virtual void                                             tracePrintf                                        (TraceLevel traceLevel, const char * const pFormat, ...);
        virtual void                                             prismAssert                                        (bool isAssertNotRequired, const char *pFileName, UI32 lineNumber);
        virtual ResourceId                                       startTimer                                         (TimerHandle &timerHandle, timeval &startInterval, timeval &periodicInterval, PrismTimerExpirationHandler pPrismTimerExpirationCallback, void *pPrismTimerExpirationContext = NULL, PrismElement *pPrismTimerSender = NULL);
        virtual ResourceId                                       startTimer                                         (TimerHandle &timerHandle,UI32 timeInMilliSeconds, PrismTimerExpirationHandler pPrismTimerExpirationCallback, void *pPrismTimerExpirationContext = NULL, PrismElement *pPrismTimerSender = NULL);
        void                                                     timerExpiredHandler                                (PrismTimerExpiredObjectManagerMessage *pTimerExpiredMessage);
        virtual ResourceId                                       deleteTimer                                        (TimerHandle timerHandle);
        virtual ResourceId                                       deleteAllTimersForService                          ();

        virtual void                                             holdMessages                                       ();
        virtual void                                             holdHighPriorityMessages                           ();
        virtual void                                             holdEvents                                         ();
        virtual void                                             holdAll                                            ();
        virtual void                                             unholdMessages                                     ();
        virtual void                                             unholdHighPriorityMessages                         ();
        virtual void                                             unholdEvents                                       ();
        virtual void                                             unholdAll                                          ();

        virtual UI32                                             getNumberOfPendingTimerExpirationMessages          ();
        virtual UI32                                             getNumberOfPendingNormalMessages                   ();
        virtual UI32                                             getNumberOfPendingHighPriorityMessages             ();

        virtual bool                                             isALocalPrismService                               ();

                void                                             registerEventListenerHandler                       (WaveObjectManagerRegisterEventListenerMessage *pWaveObjectManagerRegisterEventListenerMessage);

                void                                             setupPartitionForMOsBeingCommitted                 (vector<WaveManagedObject *> *&partitionManagedObjectsForGarbageCollection);

        virtual void                                             startTransaction                                   ();
        virtual bool                                             isTransactionInProgress                            ();
        virtual ResourceId                                       commitTransaction                                  (WaveObjectManagerCommitTransactionContext *pWaveObjectManagerCommitTransactionContext = NULL);
        virtual void                                             rollbackTransaction                                ();
        virtual void                                             addWaveConfigEntry                                 (Attribute *attribute);
        virtual void                                             updateWaveConfigEntry                              (Attribute *attribute);
        virtual void                                             getWaveConfigEntry                                 (string &configName, Attribute *attribute, bool &configFound);
        virtual void                                             updateWaveManagedObject                            (WaveManagedObject *pWaveManagedObject);
        virtual void                                             updateMultipleWaveManagedObjects                   (WaveManagedObjectSynchronousQueryContextForUpdate *pWaveManagedObjectSynchronousQueryContextForUpdate);
        virtual void                                             deleteWaveManagedObject                            (const ObjectId &objectId);
        virtual void                                             deleteWaveManagedObject                            (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion);
        virtual void                                             deleteMultipleWaveManagedObjects                   (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion);

        /**
        * @brief Deletes the compositions of MO's matching the passed query context. The objects themselves are
        *        not deleted.
        *        The compositions passed for deletions must support canBeEmpty. Otherwise this API will assert.
        * @param pWaveManagedObjectSynchronousQueryContextForDeletion - query context of the MO's whose compositions
        *        need to be deleted.
        * @param relationsSet - set of compositions to be deleted.
        *        For deleting all compositions pass empty set, or don't pass this parameter at all.
        * @retval void - Void or Empty.
        */
        virtual void                                             deleteManagedObjectCompositions                    (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion, const set<string> relationsSet = set<string>());

        /**
        * @brief Deletes the compositions of the MO with passed objectId. The object itself is not deleted.
        *        The compositions passed for deletions must support canBeEmpty. Otherwise this API will assert.
        * @param objectId - Object Id MO's whose compositions need to be deleted.
        * @param relationsSet - set of compositions to be deleted.
        *        For deleting all compositions pass empty set, or don't pass this parameter at all.
        * @retval void - Void or Empty.
        */
        virtual void                                             deleteManagedObjectCompositions                    (const ObjectId &objectId, const set<string> relationsSet = set<string>());

        virtual void                                             deletePartialManagedObjectCompositions             (const string &parentClassName, const string &compositionName, WaveManagedObjectSynchronousQueryContextForDeletion *pQueryContextForDeletionOnChildMO);

        virtual bool                                             isManagedClassSupported                            (const string &managedClass);
        virtual void                                             addRelationship                                    (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                                             deleteRelationship                                 (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                                             addToComposition                                   (const string &parentClassName, const string &childCalssName, const string &compositionName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                                             deleteFromComposition                              (const string &parentClassName, const string &childCalssName, const string &compositionName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                                             addToAssociation                                   (const string &parentClassName, const string &childCalssName, const string &associationName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                                             deleteFromAssociation                              (const string &parentClassName, const string &childCalssName, const string &associationName, const ObjectId &parentObjectId, const ObjectId &childObjectId);

        virtual void                                             query                                              (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext);
                void                                             queryCallback                                      (FrameworkStatus frameworkStatus, DatabaseObjectManagerExecuteQueryMessage *pDatabaseObjectManagerExecuteQueryMessage, void *pContext);
        virtual vector<WaveManagedObject *>                     *querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, const string &className);
        virtual vector<WaveManagedObject *>                     *querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, const string &className, const vector<string> &selectFields);
        virtual vector<WaveManagedObject *>                     *querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext);
        virtual vector<WaveManagedObject *>                     *querySynchronously                                 (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext);
        virtual vector<WaveManagedObject *>                     *querySynchronously                                 (const string &managedClassName, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual vector<WaveManagedObject *>                     *querySynchronously                                 (const string &managedClassName, vector<ObjectId> &objectIds, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual vector<WaveManagedObject *>                     *querySynchronouslyByName                           (const string &managedClassName, const string &managedObjectName, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual WaveManagedObject                               *queryManagedObject                                 (const ObjectId &managedObjectId, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual ResourceId                                       querySynchronouslyForCount                         (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext, UI32 &count);
        virtual ResourceId                                       querySynchronouslyForCount                         (const string &managedClassName, UI32 &count, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual ResourceId                                       querySynchronouslyForCount                         (const string &managedClassName, const string &fieldName, const string &range, UI32 &count, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual vector<WaveManagedObject *>                     *queryManagedObjectAssociatedWithSlot               (const string &managedClassName, UI32 slotNumber, LocationId locationId = 0, const string &schema = OrmRepository::getWaveCurrentSchema ());

        virtual pthread_t                                        getPthreadId                                       ();

                void                                             setTraceLevel                                      (const TraceLevel &traceLevel);

                void                                             setCpuAffinity                                     (const vector<UI32> &cpuAffinityVector);

                void                                             associateWithVirtualWaveObjectManager              (WaveObjectManager *pAssociatedVirtualWaveObjectManager);

        virtual ResourceId                                       sendSynchronouslyToWaveClient                      (const string &waveClientName, ManagementInterfaceMessage *pManagementInterfaceMessage, const SI32 &Instnace = 0);
        virtual WaveMessageStatus                                sendToWaveServer                                   (const UI32 &waveServerId, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler messageCallback, PrismElement *pPrismMessageSender, void *pInputContext, UI32 timeOutInMilliSeconds);
        virtual ResourceId                                       sendToWaveClient                                   (const string &waveClientName, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler pPrismMessageCallback, void *pPrismMessageContext = NULL, UI32 timeOutInMilliSeconds = 0, const SI32 &Instnace = 0);
        virtual void                                             sendToWaveClients                                  (WaveSendToClientsContext *pWaveSendToClientsContext);
        virtual ResourceId                                       sendOneWayToAllWaveClients                         (ManagementInterfaceMessage *pManagementInterfaceMessage);
                bool                                             getAllowAutomaticallyUnlistenForEvents             () const;
                void                                             setAllowAutomaticallyUnlistenForEvents             (const bool &allowAutomaticallyUnlistenForEvents);

                vector<WaveWorker *>                             getWorkers                                         ();   // postboot requires the list of workers on which the function will be invoked
                                                                                                                          // since postboot is derived from WaveWorker & it is friend of WaveObjectManager
                                                                                                                          // we can return the list of worker

        virtual void                                             printfToWaveClientSession                          (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, ...);
        virtual void                                             printfToWaveClientSession                          (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, va_list &variableArguments);
        virtual void                                             printfToAllWaveClientSessions                      (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, ...);
        virtual void                                             printfToWaveClientSessionOver                      (const WaveClientSessionContext &waveClientSessionContext);
        virtual ResourceId                                       addLog                                             (ResourceId logType, ResourceId logDescriptionType, const vector<Attribute *> &logDescriptionArguments);
                void                                             updateMessageHistoryConfig                         (bool requestedMessageHistoryState, UI32 requestedMessageHistoryMaxSize);
                void                                             getMessageHistoryDumpStringVector                  (vector<string> &messageHistoryDumpStringVector);
                void                                             restrictMessageHistoryLogging                      (bool messageHistoryLogInsideSend, bool messageHistoryLogInsideReply, bool messageHistoryLogInsideHandleMessage);
        virtual bool                                             isOperationAllowedBeforeEnabling                   (const UI32 &operationCode);

                void                                             registerLock                                       (const string &serviceString);
                ResourceId                                       acquireLock                                        (const string &serviceString);
                ResourceId                                       releaseLock                                        (const string &serviceString);

                void                                             zeroizeWorkersStep                                 (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                             zeroizeWorkersStepCallback                         (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases) ;
                void                                             zeroizeSelfStep                                    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                             zeroizeSelfStepCallback                            (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
                void                                             zeroizeHandler                                     (PrismZeroizeObjectManagerMessage *pMessage);

        virtual void                                             zeroize                                            (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);

                void                                             databaseSanityCheckHandler                         (WaveObjectManagerDatabaseSanityCheckMessage *pMessage);

                void                                             checkBasicDatabaseSanityWorkersStep                (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                             checkBasicDatabaseSanityWorkersStepCallback        (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                                             checkBasicDatabaseSanitySelfStep                   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                             checkBasicDatabaseSanitySelfStepCallback           (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void                                             dbBasicSanityCheck                                 (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

                void                                             checkIncorrectEntriesWorkersStep                   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                             checkIncorrectEntriesWorkersStepCallback           (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                                             checkIncorrectEntriesSelfStep                      (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                             checkIncorrectEntriesSelfStepCallback              (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void                                             dbInconsistencyCheck                               (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);



        virtual ResourceId                                       updateHardwareSynchronizationState                 (ResourceId hardwareSynchronizationState, const vector<LocationId> &locationIds);
        virtual ResourceId                                       updateHardwareSynchronizationState                 (ResourceId hardwareSynchronizationState, LocationId locationId = 0);
        virtual void                                             postponeMessageHandling                            (PrismMessage *pPrismMessage);
        virtual void                                             resumeAllPostponedMessages                         ();
        virtual ResourceId                                       blockCli                                           (const CliBlockContext &cliBlockContext, const bool &clusterWide);
        virtual ResourceId                                       unblockCli                                         (const CliBlockContext &cliBlockContext, const bool &clusterWide);


        /**
         * This API has few limitations.
         * 1. The MO/class 'className' should not be part of any c++ inheritance hierarchy.
         * 2. The table/MO should have a relation pointing to it or the MO should have 1-m relation point from it.
         *    This means the MO should have its own DerivationsInstances table.
         * So, Use deleteMultipleWaveManagedObjects API to avoid these limitations.
         */
        virtual void                                             deleteAllManagedObjectInstances                     (const string &className);

                WaveObjectManagerStatisticsTracker              *getPWaveObjectManagerStatisticsTracker              () const;

        virtual void                                             populatePostbootMap                                 ();
                void                                             setPostbootMap                                      (const map<string, vector<string> > &postbootManagedObjectNames);
                void                                             getPostbootMap                                      (map<string, vector<string> > &postbootManagedObjectNames) const;

                void                                             getManageObjectNamesFromPostbootMap                 (const string passName, vector<string> &managedObjectNamesInPass );

                bool                                             checkForDuplicationInBackEndAttributesMap           (GetHardwareConfigurationDetailsForPostbootContext *pGetHardwareConfigurationDetailsForPostbootContext );

                void                                             getAssociatedAttributeClone                         (string parentClassName, Attribute *pAttribute, Attribute *&pAssociatedAttribute );

                void                                             postbootForCompositeChild                           (Attribute *pAttribute, GetHardwareConfigurationDetailsForPostbootContext *pGetHardwareConfigurationDetailsForPostbootContext);

                void                                             postbootForChoiceGroup                              (GetHardwareConfigurationDetailsForPostbootContext *pGetHardwareConfigurationDetailsForPostbootContext );

        virtual void                                             applyPartitionFiltersWithPropagatedPartitionContext (WaveManagedObjectQueryContextBase *pWaveManagedObjectQueryContextBase);

        virtual void                                             bootStrapSelf                                       ();

        virtual WaveMessageBrokerStatus                          connectToMessageBroker                              (const string &brokerName, const string &brokerIpAddress, const SI32 &brokerPort);
        virtual WaveMessageBrokerStatus                          subscribeToMessageBroker                            (const string &brokerName, const vector<string> &topicNames, const vector<WaveBrokerPublishMessageHandler> publishMessageHandlers, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus                          subscribeToMessageBroker                            (const string &brokerName, const vector<string> &topicNames, WaveBrokerPublishMessageHandler publishMessageHandler, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus                          subscribeToMessageBroker                            (const string &brokerName, const string &topicName, WaveBrokerPublishMessageHandler publishMessageHandler, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus                          unsubscribeToMessageBroker                          (const string &brokerName, const vector<string> &topicNames, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus                          unsubscribeToMessageBroker                          (const string &brokerName, const string &topicName, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus                          publishToMessageBroker                              (const string &brokerName, WaveBrokerPublishMessage *pWaveBrokerPublishMessage, PrismElement *pSubscriber = NULL);

                bool                                             isAKnownWaveBrokerBasedMessageSubscription          (const string &brokerName, const string &topicName) const;

    public :
        static void                getListOfEnabledServices                    (vector<PrismServiceId> &enabledServices);
        static bool                isServiceEnabled                            (const PrismServiceId &prismServiceId);

        // The following is a special function for consuming all pending messages.  This is not to be used in general.
        // This is being implemented so that the CSCN based application can consume Wave messages as well.

               void                consumeAllPendingMessages                   ();
               PrismMessage       *getPInputMesage                             () const;

               PrismServiceId      getServiceId                                ();

        static void                addClientListeningForManagedObjectCreate    (const string &waveManagedObjectName, const string &waveClientName);
        static void                removeClientListeningForManagedObjectCreate (const string &waveManagedObjectName, const string &waveClientName);

                void               setBackendAttributeMap                      (const map<string, map<string,UnifiedClientBackendDetails*> > &backendAttributeMap);
                void               getBackendAttributeMap                      (map<string, map<string,UnifiedClientBackendDetails*> > &backendAttributeMap) const;
        virtual ResourceId         postbootValidate                            (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);
        virtual WaveManagedObject *createInMemoryManagedObject                 (const  string &className);
                void               getAllOwnedManagedObjectClassNames          (vector<string> &ownedManagedObjectClassNames) const ;
                void               checkMessageAttributesInSerialization       ();

        static  void               endOfLifeService                            (PrismServiceId prismServiceId);
        static  void               bootStrapService                            (PrismServiceId prismServiceId);

    // Now the data members

    private :
               string                                                               m_name;
               PrismThread                                                         *m_pAssociatedPrismThread;
               map<UI32, PrismOperationMapContext *>                                m_operationsMap;
               map<UI32, UI32>                                                      m_supportedEvents;
               map<LocationId, map<UI32, map<UI32, PrismEventMapContext *> *> *>    m_eventsMap;
               map<UI32, PrismMessageResponseContext *>                             m_responsesMap;
               map<UI32, vector<PrismEventListenerMapContext *> *>                  m_eventListenersMap;
               map<string, vector<string> >                                         m_postbootManagedObjectNames;
               PrismMutex                                                           m_responsesMapMutex;
               PrismMutex                                                           m_sendReplyMutexForResponseMap;
               vector<WaveWorker *>                                                 m_workers;
               bool                                                                 m_isEnabled;
               PrismMutex                                                           m_isEnabledMutex;
               TraceClientId                                                        m_traceClientId;

               map<string, string>                                                  m_managedClasses;
               map<string, string>                                                  m_managedViews;
               bool                                                                 m_isTransactionOn;
               vector<WaveManagedObject *>                                          m_objectsCreatedDuringCurrentTransaction;
               vector<CompositionEntry>                                             m_compositionEntriesCreatedDuringCurrentTrasnaction;
               vector<AssociationEntry>                                             m_associationEntriesCreatedDuringCurrentTrasnaction;
               vector<RelationshipEntry>                                            m_relationshipEntriesCreatedDuringCurrentTransaction;
               vector<WaveManagedObject *>                                          m_objectsUpdatedIndividuallyDuringCurrentTransaction;
               vector<WaveManagedObjectForUpdate *>                                 m_objectsUpdatedDuringCurrentTransaction;
               vector<ObjectId>                                                     m_objectsDeletedIndividuallyDuringCurrentTransaction;
               vector<WaveManagedObjectForDelete *>                                 m_objectsDeletedDuringCurrentTransaction;
               vector<string>                                                       m_managedObjectsDeletedAtTheBeginingOfTransaction;
               vector<CompositionEntry>                                             m_compositionEntriesDeletedDuringCurrentTransaction;
               vector<AssociationEntry>                                             m_associationEntriesDeletedDuringCurrentTrasnaction;
               vector<RelationshipEntry>                                            m_relationshipEntriesDeletedDuringCurrentTransaction;

               vector<WaveConfigurationAttributes>                                  m_configurationsCreatedDuringCurrentTransaction;
               vector<WaveConfigurationAttributes>                                  m_configurationsUpdatedDuringCurrentTransaction;


               vector<UI32>                                                         m_cpuAffinityVector;

               WaveObjectManager                                                   *m_pAssociatedVirtualWaveObjectManager; // This filed will be used only by the derived classes of type WaveLocalObjectManager to represent virtual/global service corresponding to that locla service.

        static map<string, PrismElement *>                                          m_ownersForCreatingManagedObjectInstances;
        static PrismMutex                                                           m_createManagedObjectInstanceWrapperMutex;
               map<UI32, PrismElement *>                                            m_ownersForCreatingMessageInstances;
               PrismMutex                                                           m_createMessageInstanceWrapperMutex;

               bool                                                                 m_allowAutomaticallyUnlistenForEvents;

               WaveManagedObjectLoadOperationalDataWorker                          *m_pWaveManagedObjectLoadOperationalDataWorker;
               PrismPostbootWorker                                                 *m_pPrismPostbootWorker;
               PrismMessage                                                        *m_pInputMessage;

               vector<MessageHistory *>                                             m_messageHistoryVector;
               bool                                                                 m_messageHistoryState;
               UI32                                                                 m_messageHistoryMaxSize;
               UI32                                                                 m_messageHistoryNextSlotInVector;
               bool                                                                 m_messageHistoryLogInsideSend;
               bool                                                                 m_messageHistoryLogInsideReply;
               bool                                                                 m_messageHistoryLogInsideHandleMessage;
        static map<string, PrismServiceId>                                          m_serviceStringServiceIdMap;
        static PrismMutex                                                           m_serviceStringServiceIdMapMutex;
               set<string>                                                          m_serviceStrings;

               WaveManagedObjectUpdateWorker                                       *m_pWaveManagedObjectUpdateWorker;
               WaveManagedObjectCreateWorker                                       *m_pWaveManagedObjectCreateWorker;
               WaveDebugInformationWorker                                          *m_pWaveDebugInformationWorker;
               WaveObjectManagerStatisticsTracker                                  *m_pWaveObjectManagerStatisticsTracker;
                WaveManagedObjectDeleteWorker                                      *m_pWaveManagedObjectDeleteWorker;
                WaveClientDataObjectGetWorker                                      *m_pWaveClientDataObjectGetWorker;

               map<UI32, map<UI32, UI64> >                                          m_secondsForMessageHandlerSequencerSteps;
               map<UI32, map<UI32, SI64> >                                          m_nanoSecondsForMessageHandlerSequencerSteps;

               map<UI32, map<UI32, UI64> >                                          m_realSecondsForMessageHandlerSequencerSteps;
               map<UI32, map<UI32, SI64> >                                          m_realNanoSecondsForMessageHandlerSequencerSteps;
               map<string, map<string, UnifiedClientBackendDetails*> >              m_backendAttributeMap; // map of boot or shutdown phase, <taskname, backendDetails>
               set<string>                                                          m_setOfPartitionNamesReferencedInCurrentTransaction;
        static PrismMutex                                                           m_postponedMessageQueueMutex;
               PrismMessageQueue<PrismMessage>                                      m_postponedMessageQueue;

        static map<string, map<string, string> >                                    m_clientsListeningForCreateByManagedObject;
        static map<string, map<string, string> >                                    m_clientsListeningForUpdateByManagedObject;
        static map<string, map<string, string> >                                    m_clientsListeningForDeleteByManagedObject;
        static map<string, map<string, string> >                                    m_managedObjectsForCreateByClient;
        static map<string, map<string, string> >                                    m_managedObjectsForUpdateByClient;
        static map<string, map<string, string> >                                    m_managedObjectsForDeleteByClient;
        static PrismMutex                                                           m_clientsListeningMutex;
               PrismMutex                                                           m_workersMutex;
        static WaveServiceMode                                                      m_waveServiceLaunchMode;
               WaveServiceMode                                                      m_waveServiceMode;
               UI32                                                                 m_serviceId;

               map<string, map<string, WaveBrokerPublishMessageHandlerContext *> >  m_waveBrokerBasedMessageSubscriberInformationMap;

               WaveDeliverBrokerPublishMessageWorker                               *m_pWaveDeliverBrokerPublishMessageWorker;

    protected :
    public :

    friend class Wave;
    friend class WaveClient;
    friend class WaveSystemManagement;
    friend class PrismThread;
    friend class WaveWorker;
    friend class WaveManagedObject;
    friend class PrismMessageFactory;
    friend class ApplicationServiceRepository;
    friend class ApplicationServiceUtils;
    friend class WaveManagedObjectFactory;
    friend class DatabaseObjectManagerExecuteQueryWorker;
	friend class DatabaseQueryCursor;
    friend class PrismPostbootWorker;
    friend class WaveDebugInformationWorker;
    friend class WaveCliTraceShell;
    friend class ObjectTracker;
    friend class WaveObjectManagerToolKit;
    friend class WaveClientSynchronousConnection;
    friend class WaveManagedObjectCreateWorker;
    friend class WaveManagedObjectUpdateWorker;
    friend class WaveManagedObjectDeleteWorker;
    friend class WaveClientDataObjectGetWorker;
    friend class PrismSynchronousLinearSequencerContext;
    friend class ManagementInterfaceObjectManager;
    friend class WaveMessageBroker;
    friend class WaveMessageBrokerClient;

    friend void tracePrintf (TraceLevel traceLevel, const bool &addNewLine, const bool &suppressPrefix, const char * const pFormat, ...);
    friend void tracePrintf (TraceLevel traceLevel, const char * const pFormat, ...);

};

}

#endif //PRISMOBJECTMANAGER_H
