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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef PERSISTENCELOCALOBJECTMANAGER_H
#define PERSISTENCELOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/StartupSchemaChangeEvent.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerUpdateInstanceIdMesssage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerPushConfigToKernelMessaage.h"

namespace WaveNs
{

class PersistenceLocalObjectManagerSetStartupFileMessage;
class PersistenceLocalObjectManagerSavePrismConfigurationMessage;
class PersistenceLocalObjectManagerCopySchemaMessage;
class PersistenceLocalObjectManagerCopyDefaultClusterMessage;
class PersistenceLocalObjectManagerSetConfigurationValidMessage;
class PersistenceLocalObjectManagerDebugSchemaChangeMessage;
class PersistenceLocalObjectManagerExecuteTransactionMessage;
class PersistenceLocalObjectManagerEnableTransactionMessage;
class PersistenceLocalObjectManagerSetLiveSyncEnableMessage;
class PersistenceLocalObjectManagerGetLiveSyncEnableMessage;
class PersistenceLocalExecuteTransactionContext;
class DatabaseObjectManagerExecuteTransactionMessage;

typedef ResourceId  (*UpdateGlobalConfigInKernelHandlerFunction)       (string &globalConfigs);

class PersistenceLocalObjectManager : public WaveLocalObjectManager
{
    private :
                                                PersistenceLocalObjectManager                   ();
                void                            setStartupFileMessageHandler                    (PersistenceLocalObjectManagerSetStartupFileMessage *pMessage);
                void                            copyDefaultClusterMessageHandler                (PersistenceLocalObjectManagerCopyDefaultClusterMessage *pMessage); 
                void                            savePrismConfigurationMessageHandler            (PersistenceLocalObjectManagerSavePrismConfigurationMessage *pMessage);
                void                            copySchemaMessageHandler                        (PersistenceLocalObjectManagerCopySchemaMessage *pPersistenceLocalObjectManagerCopySchemaMessage);
                void                            setConfigurationValidMessageHandler             (PersistenceLocalObjectManagerSetConfigurationValidMessage *pPersistenceLocalObjectManagerSetConfigurationValidMessage);   

                void                            debugSchemaChangeMessageHandler                 (PersistenceLocalObjectManagerDebugSchemaChangeMessage *pPersistenceLocalObjectManagerDebugSchemaChangeMessage);

        virtual PrismMessage                   *createMessageInstance                           (const UI32 &operationCode);
        virtual PrismEvent                     *createEventInstance                             (const UI32 &eventOperationCode);

        static  bool                            getDisableLocalCommitTransactions               ();
        static  void                            setDisableLocalCommitTransactions               (const bool &disableLocalCommitTransactions);
        static  bool                            getLiveSyncEnabled                              ();
        static  void                            setLiveSyncEnabled                              (const bool &liveSyncEnabled);

    protected :
    public :
        static PersistenceLocalObjectManager   *getInstance                                     ();
        static PrismServiceId                   getPrismServiceId                               ();
        static string                           getServiceName                                  ();
        void                                    broadcastSchemaChangeEvent                      (SchemaType m_fromSchema, string fromSchemaPath = "", SchemaType toSchema = STARTUP_SCHEMA, string toSchemaPath = "", bool operationStatus = true);
        void                                    updateInstanceIdMessageHandler                  (PersistenceLocalObjectManagerUpdateInstanceIdMesssage *pPersistenceLocalObjectManagerUpdateInstanceIdMesssage);
        void                                    updateInstanceId                                ();
        void                                    executeTransactionMessageHandler                (PersistenceLocalObjectManagerExecuteTransactionMessage *pPersistenceLocalObjectManagerExecuteTransactionMessage);
        void                                    sendTransactionToDatabase                       (PersistenceLocalExecuteTransactionContext *pPersistenceLocalExecuteTransactionContext);
        void                                    handleTransactionResult                         (PersistenceLocalExecuteTransactionContext *pPersistenceLocalExecuteTransactionContext);
        void                                    executeTransactionSendRequestCallback           (FrameworkStatus frameworkStatus, DatabaseObjectManagerExecuteTransactionMessage *pDatabaseObjectManagerExecuteTransactionMessage, PersistenceLocalExecuteTransactionContext *pPersistenceLocalExecuteTransactionContext);
        void                                    executeTransactionSendRequestStandbyCallback    (FrameworkStatus frameworkStatus, DatabaseObjectManagerExecuteTransactionMessage *pDatabaseObjectManagerExecuteTransactionMessage, PersistenceLocalExecuteTransactionContext *pPersistenceLocalExecuteTransactionContext);
        void                                    enableTransactionMessageHandler                 (PersistenceLocalObjectManagerEnableTransactionMessage *pPersistenceLocalObjectManagerEnableTransactionMessage);
        void                                    setLiveSyncEnableMessageHandler                 (PersistenceLocalObjectManagerSetLiveSyncEnableMessage *pPersistenceLocalObjectManagerSetLiveSyncEnableMessage);
        void                                    getLiveSyncEnableMessageHandler                 (PersistenceLocalObjectManagerGetLiveSyncEnableMessage *pPersistenceLocalObjectManagerGetLiveSyncEnableMessage);
        void                                    install                                         (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                                    hainstall                                       (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                                    boot                                            (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                                    haboot                                          (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                                    shutdown                                        (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        static UI64                             getAnyConfigurationChangeTrackingNumber         ();
        static void                             setAnyConfigurationChangeTrackingNumber         (UI64 &trackingNumber);
        static void                             setUpdateGlobalConfigInKernelHandlerFunction    (UpdateGlobalConfigInKernelHandlerFunction updateGlobalConfigInKernelHandlerFunction);
        void                                    pushConfigToKernelMessageHandler                (PersistenceLocalObjectManagerPushConfigToKernelMessaage *pPersistenceLocalObjectManagerPushConfigToKernelMessaage);
               ResourceId                       updateGlobalConfigInKernel                      (string &globalConfigs);
    // Now the data members

    private :
        static  bool                            m_liveSyncEnabled;
        static  PrismMutex                      m_liveSyncEnabledMutex;
        static  bool                            m_disableLocalCommitTransactions;
        static  PrismMutex                      m_disableLocalCommitTransactionsMutex;
        static  string                          m_unsyncedPendingPrepareTransaction;
        static  UI64                            m_anyConfiguraitonChangeTrackingNumber;
        static  PrismMutex                      m_anyConfiguraitonChangeTrackingNumberMutex;
        static  UpdateGlobalConfigInKernelHandlerFunction m_updateGlobalConfigInKernelHandlerFunction;

    protected :
    public :

    friend class WaveObjectManager;
    friend class FrameworkToolKit;
    friend class PersistenceObjectManager;
};

}

#endif //PERSISTENCELOCALOBJECTMANAGER_H
