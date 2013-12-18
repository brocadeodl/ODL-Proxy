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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka, Vipool Prajapati                  *
 ***************************************************************************/

#include "Framework/Core/Wave.h"
#include "Framework/Persistence/PersistenceObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceObjectManagerExecuteTransactionMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerCopySchemaMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerCopyFileMessage.h"
#include "Framework/Persistence/PersistenceExecuteTransactionContext.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/Database/DatabaseObjectManagerExecuteTransactionMessage.h"
#include "Framework/Database/DatabaseObjectManagerExecuteCopySchemaMessage.h"
#include "Framework/Database/DatabaseObjectManagerCleanPreparedTransactionsMessage.h"
#include "Framework/Database/DatabaseObjectManagerCalculateDbCksumMessage.h"
#include "Framework/Database/DatabaseObjectManagerRestoreMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerSetStartupFileMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerSavePrismConfigurationMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerCopyDefaultClusterMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerExecuteTransactionMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerCompareDBMessage.h"
#include "Framework/Persistence/MoSchemaInfoManagedObject.h"
#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaInfoRepository.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Shell/ShellDebug.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Database/DatabaseObjectManagerGetLatestInstanceIdForTableMessage.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseObjectManagerEmptyMessage.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForDebugInformation.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Persistence/ConfigurationManagedObject.h"
#include "Framework/Persistence/WaveConfigManagedObject.h"
#include "Framework/Persistence/PersistenceObjectManagerInitializeBeforeBootCompleteMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerUpdateInstanceIdMesssage.h"
#include "Framework/Persistence/PersistenceObjectManagerPrintOrmMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerMessages.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Persistence/PersistenceStatisticsTracker.h"
#include "Framework/Persistence/PersistenceObjectManagerGetAllClassNamesMessage.h"
#include "Framework/ObjectModel/WaveSendMulticastContext.h"
#include "Framework/Persistence/PersistenceObjectManagerDelayedTransactionMessages.h"
#include "Framework/Persistence/PersistenceObjectManagerExecuteTransactionAsynchronousContext.h"
#include "File/Local/FileLocalTypes.h"
#include <algorithm>

namespace WaveNs
{

static bool s_schemaCreationRequiredDuringBoot              = false;
//static bool s_schemaDropRequiredDuringBoot                  = false;
static UI32 s_currentTransactionId                          = 0;

UI64       PersistenceObjectManager::m_anyConfiguraitonChangeTrackingNumber   = 0;
PrismMutex PersistenceObjectManager::m_anyConfiguraitonChangeTrackingNumberMutex;

/*
void PersistenceObjectManager::setSchemaDropRequiredDuringBoot (const bool &schemaDropRequriedDuringBoot)
{
    s_schemaDropRequiredDuringBoot = schemaDropRequriedDuringBoot;
}*/

PersistenceObjectManager::PersistenceObjectManager ()
    : WaveObjectManager (getPrismServiceName ()),
      m_pConfigurationManagedObject (NULL),
      m_delayedCommitTransactions   ("")
{
    ConfigurationManagedObject configurationManagedObject (this);

    configurationManagedObject.setupOrm ();
    addManagedClass (ConfigurationManagedObject::getClassName ());
    
    MoSchemaInfoManagedObject schemaInfoMO(this);
    schemaInfoMO.setupOrm ();
    addManagedClass (MoSchemaInfoManagedObject::getClassName ());

    WaveConfigManagedObject waveConfigManagedObject (this);

    waveConfigManagedObject.setupOrm ();
    addManagedClass (WaveConfigManagedObject::getClassName ());

    XPathStringManagedObject xPathStringManagedObject (this);
    xPathStringManagedObject.setupOrm ();
    addManagedClass (XPathStringManagedObject::getClassName ());

    addDebugFunction ((ShellCmdFunction) (&OrmRepository::printOrmDebugShellWrapper), "printorm");

    addOperationMap (PERSISTENCE_OBNJECT_MANAGER_EXECUTE_TRANSACTION, reinterpret_cast<PrismMessageHandler> (&PersistenceObjectManager::executeTransactionMessageHandler));
    addOperationMap (PERSISTENCE_OBNJECT_MANAGER_COPY_FILE, reinterpret_cast<PrismMessageHandler> (&PersistenceObjectManager::copyFileMessageHandler));
    addOperationMap (PERSISTENCE_OBNJECT_MANAGER_COMPARE_DB, reinterpret_cast<PrismMessageHandler> (&PersistenceObjectManager::compareDBMessageHandler));
    addOperationMap (PERSISTENCE_OBJECT_MANAGER_INITIALIZE_BEFORE_BOOT_COMPLETE, reinterpret_cast<PrismMessageHandler> (&PersistenceObjectManager::initializeConfigurationManagedObjectBeforeBootCompleteMessageHandler));
    addOperationMap (PERSISTENCE_OBJECT_MANAGER_UPDATE_INSTANCE_ID, reinterpret_cast<PrismMessageHandler> (&PersistenceObjectManager::updateInstanceIdMessageHandler));
    addOperationMap (PERSISTENCE_OBJECT_MANAGER_PRINT_ORM, reinterpret_cast<PrismMessageHandler> (&PersistenceObjectManager::printOrmMessageHandler));

    addOperationMap (PERSISTENCE_OBJECT_MANAGER_ADD_XPATH_STRINGS_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PersistenceObjectManager::addXPathStringsMessageHandler));
    addOperationMap (PERSISTENCE_OBJECT_MANAGER_DELETE_XPATH_STRINGS_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PersistenceObjectManager::deleteXPathStringsMessageHandler));
    addOperationMap (PERSISTENCE_OBJECT_MANAGER_GET_LAST_UPDATE_TIMESTAMPS_FOR_XPATH_STRINGS_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PersistenceObjectManager::getLastUpdateTimestampsForXPathStringsMessageHandler));
    addOperationMap (PERSISTENCE_OBJECT_MANAGER_RESET_XPATH_STRINGS_TIMESTAMPS, reinterpret_cast<PrismMessageHandler> (&PersistenceObjectManager::resetXPathStringsTimestampsMessageHandler));

    addOperationMap (PERSISTENCE_OBJECT_MANAGER_GET_ALL_CLASS_NAMES, reinterpret_cast<PrismMessageHandler> (&PersistenceObjectManager::getAllClassNamesMessageHandler));
    addServiceIndependentOperationMap (PERSISTENCE_OBJECT_MANAGER_GET_ALL_CLASS_NAMES, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&PersistenceObjectManager::getAllClassNamesMessageStaticHandler));

    addOperationMap (PERSISTENCE_OBJECT_MANAGER_ADD_DELAYED_TRANSACTION, reinterpret_cast<PrismMessageHandler> (&PersistenceObjectManager::addDelayedTransactionMessageHandler));
}

PersistenceObjectManager::~PersistenceObjectManager ()
{
}

PersistenceObjectManager *PersistenceObjectManager::getInstance ()
{
    static PersistenceObjectManager *pPersistenceObjectManager = new PersistenceObjectManager ();

    WaveNs::prismAssert (NULL != pPersistenceObjectManager, __FILE__, __LINE__);

    return (pPersistenceObjectManager);
}

PrismServiceId PersistenceObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string PersistenceObjectManager::getPrismServiceName ()
{
    return ("Persistence");
}

PrismMessage *PersistenceObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case PERSISTENCE_OBNJECT_MANAGER_EXECUTE_TRANSACTION :
            pPrismMessage = new PersistenceObjectManagerExecuteTransactionMessage ();
            break;

        case PERSISTENCE_OBNJECT_MANAGER_COPY_FILE :
            pPrismMessage = new PersistenceObjectManagerCopyFileMessage ();
            break;

        case PERSISTENCE_OBNJECT_MANAGER_COMPARE_DB :
            pPrismMessage = new PersistenceObjectManagerCompareDBMessage ();
            break;

        case PERSISTENCE_OBJECT_MANAGER_INITIALIZE_BEFORE_BOOT_COMPLETE :
            pPrismMessage = new PersistenceObjectManagerInitializeBeforeBootCompleteMessage ();
            break;

        case PERSISTENCE_OBJECT_MANAGER_UPDATE_INSTANCE_ID :
            pPrismMessage = new PersistenceObjectManagerUpdateInstanceIdMesssage ();
            break;

        case PERSISTENCE_OBJECT_MANAGER_PRINT_ORM :
            pPrismMessage = new PersistenceObjectManagerPrintOrmMessage ();
            break;

        case PERSISTENCE_OBJECT_MANAGER_ADD_XPATH_STRINGS_MESSAGE :
            pPrismMessage = new PersistenceObjectManagerAddXPathStringsMessage ();
            break;

        case PERSISTENCE_OBJECT_MANAGER_DELETE_XPATH_STRINGS_MESSAGE :
            pPrismMessage = new PersistenceObjectManagerDeleteXPathStringsMessage ();
            break;

        case PERSISTENCE_OBJECT_MANAGER_GET_LAST_UPDATE_TIMESTAMPS_FOR_XPATH_STRINGS_MESSAGE :
            pPrismMessage = new PersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage ();
            break;

        case PERSISTENCE_OBJECT_MANAGER_RESET_XPATH_STRINGS_TIMESTAMPS :
            pPrismMessage = new PersistenceObjectManagerResetXPathStringsTimestampsMessage ();
            break;

        case PERSISTENCE_OBJECT_MANAGER_GET_ALL_CLASS_NAMES :
            pPrismMessage = new PersistenceObjectManagerGetAllClassNamesMessage ();
            break;

        case PERSISTENCE_OBJECT_MANAGER_ADD_DELAYED_TRANSACTION :
            pPrismMessage = new PersistenceObjectManagerAddDelayedTransactionMessage ();
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("PersistenceObjectManager::createMessageInstance : Unknown operation code : ") + operationCode);
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (pPrismMessage);
}

WaveManagedObject *PersistenceObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((ConfigurationManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new ConfigurationManagedObject (this);
    }
    else if ((MoSchemaInfoManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new MoSchemaInfoManagedObject (this);
    }
    else if ((XPathStringManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new XPathStringManagedObject (this);
    }
    else if ((WaveConfigManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new WaveConfigManagedObject (this);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        prismAssert (false, __FILE__, __LINE__);
    }

    return (pWaveManagedObject);
}

void PersistenceObjectManager::shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::shutdown Entering");

    delete m_pConfigurationManagedObject;
    m_pConfigurationManagedObject = NULL;    

    flushXPathStringToXPathStringManagedObjectMap ();

    flushWaveConfigManagedObjectMap ();

    clearDelayedTransactions ();

    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void PersistenceObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::initialize : Creating Object Relational Mapping ...");

    // At this point indicate that ORM is enabled.

    PrismPersistableObject::setIsObjectRelationalMappingEnabled (true);

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void PersistenceObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    // Indicate that we need to create schema during boot.
    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    if (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT != bootReason)
    {
        s_schemaCreationRequiredDuringBoot = true;
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
    return;
}

void PersistenceObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    // First check if there are any prepared transactions that are not applied and remvoe them.
    // This case will arise only if the system crashed in the middle of doing some transactions in database.

    DatabaseObjectManagerCleanPreparedTransactionsMessage databaseObjectManagerCleanPreparedTransactionsMessage;
    ResourceId                                            cleanPreparedTransactionsStatus                        = WAVE_MESSAGE_SUCCESS;
    WaveBootReason                                        bootReason                                             = pWaveAsynchronousContextForBootPhases->getBootReason ();

    cleanPreparedTransactionsStatus = sendSynchronously (&databaseObjectManagerCleanPreparedTransactionsMessage);

    if (WAVE_MESSAGE_SUCCESS != cleanPreparedTransactionsStatus)
    {
        trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot : Could not send message to Clean the Prepared Transactions. Status : " + FrameworkToolKit::localize (cleanPreparedTransactionsStatus));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        cleanPreparedTransactionsStatus = databaseObjectManagerCleanPreparedTransactionsMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == cleanPreparedTransactionsStatus)
        {
            trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::boot : Succeeded in cleaning the previously Prepared Transactions(ACTIVE).");
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot : Failed to Clean the previously Prepared Transactions. Status : " + FrameworkToolKit::localize (cleanPreparedTransactionsStatus));
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (cleanPreparedTransactionsStatus);
            pWaveAsynchronousContextForBootPhases->callback();
            return;
        }
    }

    if ( (WAVE_MESSAGE_SUCCESS == FrameworkToolKit::isConnectedToHaPeerLocation ()) && (true == FrameworkToolKit::getIsLiveSyncEnabled ()) )
    {
        DatabaseObjectManagerCleanPreparedTransactionsMessage databaseObjectManagerCleanPreparedTransactionsStandbyMessage;
        ResourceId                                            cleanPreparedTransactionsStandbyStatus                        = WAVE_MESSAGE_SUCCESS;

        cleanPreparedTransactionsStandbyStatus = sendSynchronously (&databaseObjectManagerCleanPreparedTransactionsStandbyMessage, FrameworkToolKit::getHaPeerLocationId ());

        if (WAVE_MESSAGE_SUCCESS != cleanPreparedTransactionsStandbyStatus)
        {
            trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot: Could not send message to Clean the Prepared Transactions(STANDBY). Status : " + FrameworkToolKit::localize (cleanPreparedTransactionsStandbyStatus));
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
        else
        {
            cleanPreparedTransactionsStandbyStatus = databaseObjectManagerCleanPreparedTransactionsStandbyMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == cleanPreparedTransactionsStandbyStatus)
            {
                trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::boot: Succeeded in cleaning the previously Prepared Transactions(STANDBY).");
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot: Failed to Clean the previously Prepared Transactions. Status(STANDBY) : " + FrameworkToolKit::localize (cleanPreparedTransactionsStandbyStatus));
                FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
            }
        }
    }
    else if ((FrameworkToolKit::getSyncState () == IN_SYNC) && (false == FrameworkToolKit::getIsLiveSyncEnabled ()))
    {
        // DB previously in sync from sync dump. but live transaction sync not supported. So set out of sync and notify on sync update fail
        trace (TRACE_LEVEL_WARN, "PersistenceObjectManager::boot: Live sync disabled. Notify sync update failure on first update after sync dump.");
        FrameworkToolKit::notifySyncUpdateFailure(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_NOT_SUPPORTED);
    }


// Commenting the "VACUUM " done for First time boot as its takes quite a long time to finish depending upon the database state.
// Instead, WaveDatabase will be created on all the first time boots irrespective of the database already installed or not.
#if 0
    string                                          sqlForVacuum;
    bool                                            toDoVaccum;
    if (WAVE_BOOT_FIRST_TIME_BOOT == bootReason)
    {
        sqlForVacuum  = "VACUUM FULL;";
        toDoVaccum = true;
    }
    else
    {
        toDoVaccum = false;
    }

    if (toDoVaccum == true)
    {
        DatabaseObjectManagerExecuteTransactionMessage  databaseObjectManagerExecuteTransactionMessage  (sqlForVacuum);
        ResourceId                                      vacuumTransactionsStatus                        = WAVE_MESSAGE_SUCCESS;

        vacuumTransactionsStatus = sendSynchronously (&databaseObjectManagerExecuteTransactionMessage);

        if (WAVE_MESSAGE_SUCCESS != vacuumTransactionsStatus)
        {
            trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot : Could not send message to VACUUM Database. Status : " + FrameworkToolKit::localize (vacuumTransactionsStatus));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            vacuumTransactionsStatus = databaseObjectManagerExecuteTransactionMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == vacuumTransactionsStatus)
            {
                trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::boot : Succeeded in VACUUM ing Database.");
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot : Failed to VACUUM Database. Status : " + FrameworkToolKit::localize (vacuumTransactionsStatus));

                pWaveAsynchronousContextForBootPhases->setCompletionStatus (vacuumTransactionsStatus);
                pWaveAsynchronousContextForBootPhases->callback();
                return;
            }
        }
    }
#endif

    // check if last DB restore operation was complete. If not, then install the db in case database connection has failed else just restore the database

    if ((true == FrameworkToolKit::getIsDbRestoreIncomplete ()) && (false == DatabaseObjectManager::getIsDatabaseConnected()))
    {
        s_schemaCreationRequiredDuringBoot = true;
        DatabaseObjectManager::setIsDatabaseConnected (true);
    }

    // Now proceed to check if Schema Creation is required.
    // If Schema screation is not required, check if schema needs to be dropped.

    if (true == s_schemaCreationRequiredDuringBoot)
    {
        OrmRepository *pOrmRepository = OrmRepository::getInstance ();

// s_schemaDropRequiredDuringBoot used to be set to true in case of first time boot and if database is already installed. 
// But, now database will be recreated irrspective of its existence in first time boot. So, this is not required. Hence, commenting.  
#if 0
        if (true == s_schemaDropRequiredDuringBoot)
        {
            //Need to remove cfg file. This is to avoid the crash when the node is rebooted after dropping tables from previous Wave ORM schema.
            string command = string ("rm -rf " + FrameworkToolKit::getProcessInitialWorkingDirectory () +"/*.cfg");

            trace (TRACE_LEVEL_INFO, string ("removing file ") + command + string ("before dropping tables from previous Wave ORM Schema"));

            system (command.c_str ());
            
            trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::boot : Dropping Tables from previous Wave ORM Schema ...");

            string sqlForDropWaveSchemas = pOrmRepository->generateSqlToDropSchemaFromOrmDatabase (OrmRepository::getWaveCurrentSchema ());

            trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::boot : Schema Drop\n" + sqlForDropWaveSchemas);

            DatabaseObjectManagerExecuteTransactionMessage message1  (sqlForDropWaveSchemas);
            ResourceId                                     status   = WAVE_MESSAGE_SUCCESS;

            status = sendSynchronously (&message1);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot : Could not send message to Drop the Previous Database Schema. Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                status = message1.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR,   "PersistenceObjectManager::boot : Could not Drop the Previous Database Schema. Status : " + FrameworkToolKit::localize (status));
                    trace (TRACE_LEVEL_SUCCESS, "PersistenceObjectManager::boot : The above error can safely be ignored.");

                    status = WAVE_MESSAGE_SUCCESS;
                }
            }

            if (WAVE_MESSAGE_SUCCESS != status)
            {

                //Save the configuration here as we have removed the cfg file before deleting tables from previous ORM schema.
                FrameworkToolKit::savePrismConfiguration();

                pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
                pWaveAsynchronousContextForBootPhases->callback ();
                return;
            }

            //////////////////////////////////////////////////////////////////////////////////////////
            // Drop the start schema
            /////////////////////////////////////////////////////////////////////////////////////////
            sqlForDropWaveSchemas = pOrmRepository->generateSqlToDropSchemaFromOrmDatabase (OrmRepository::getWaveStartSchema ());

            DatabaseObjectManagerExecuteTransactionMessage message2  (sqlForDropWaveSchemas);
            status   = WAVE_MESSAGE_SUCCESS;

            status = sendSynchronously (&message2);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot : Could not send message to Drop the Start Database Schema. Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                status = message2.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR,   "PersistenceObjectManager::boot : Could not Drop the Previous Start Database Schema. Status : " + FrameworkToolKit::localize (status));
                    trace (TRACE_LEVEL_SUCCESS, "PersistenceObjectManager::boot : The above error can safely be ignored.");

                    status = WAVE_MESSAGE_SUCCESS;
                }
            }

            if (WAVE_MESSAGE_SUCCESS != status)
            {

                //Save the configuration here as we have removed the cfg file before deleting tables from previous ORM schema.
                FrameworkToolKit::savePrismConfiguration();

                pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
                pWaveAsynchronousContextForBootPhases->callback ();
                return;
            }
        }
#endif

        //////////////////////////////////////////////////////////////////////////////////////////
        // Install both start and running schemas
        /////////////////////////////////////////////////////////////////////////////////////////
        trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::boot : Installing Wave ORM Schema ...");

        string sqlForCreate = pOrmRepository->generateSchema (OrmRepository::getWaveCurrentSchema ());
        sqlForCreate += pOrmRepository->generateSchema (OrmRepository::getWaveStartSchema ());

        ConfigurationManagedObject *pConfigurationManagedObject = new ConfigurationManagedObject (this);

        string tempSqlForInsert;

        pConfigurationManagedObject->getSqlForInsert (tempSqlForInsert, OrmRepository::getWaveCurrentSchema ());
    
        sqlForCreate +=  tempSqlForInsert;

        trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::boot : Schema Creation\n" + sqlForCreate);

        DatabaseObjectManagerExecuteTransactionMessage message (sqlForCreate);
        ResourceId                                     status  = WAVE_MESSAGE_SUCCESS;

        status = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot : Could not send message to create Database Schema to represent the Object Relational Mapping. Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            status = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot : Could not create Database Schema to represent the Object Relational Mapping. Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                m_pConfigurationManagedObject = pConfigurationManagedObject;
            }
        }

        if (WAVE_MESSAGE_SUCCESS != status)
        {

            //Save the configuration here as we have removed the cfg file before deleting tables from previous ORM schema.
            //FrameworkToolKit::savePrismConfiguration();
            
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }

        //Save the configuration here as we have removed the cfg file before deleting tables from previous ORM schema.
        //FrameworkToolKit::savePrismConfiguration();

        s_schemaCreationRequiredDuringBoot = false;
    }
    else
    {
        if ((PrismFrameworkObjectManager::getInstance ())->getStandbySyncState () == IN_SYNC)
        {
                //  MM over with running DB synced from active MM, running DB will be used
            trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::boot : running DB from HA sync will be used at this MM failover.");
        }
        else if (LOCATION_STAND_ALONE == FrameworkToolKit::getThisLocationRole ())
        {
            // Whe management cluster is enabled, even if location role is LOCATION_STAND_ALONE,
            // do not copy startup-schema to running-schema. 
            // (copy running to startup is not done in management cluster. 
            //  i.e. config is present in running-schema, and it should not be lost)

            bool clusterEnabled = Wave::clusterEnabledCheck ();

            if ((true == clusterEnabled) && true == FrameworkToolKit::getIsStartupValid ())
            {
                //  MM over with running DB synced from active MM, running DB will be used
                trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::boot : running DB will be used at this boot in MC mode.");
            }
            else if ((false == clusterEnabled) && true == FrameworkToolKit::getIsStartupValid ())
            {
                //////////////////////////////////////////////////////////////////////////////////////////
                // Copy start schema to running schema if valid
                /////////////////////////////////////////////////////////////////////////////////////////
                trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::boot : copying start schema to running schema");
                copySchemaLocal (OrmRepository::getWaveStartSchema (), OrmRepository::getWaveCurrentSchema ());
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::boot : Emptying database..");

                //////////////////////////////////////////////////////////////////////////////////////////
                //  Empty the database
                /////////////////////////////////////////////////////////////////////////////////////////

                //trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::boot : \n" + sqlToEmpty);

                DatabaseObjectManagerEmptyMessage   message3;
                ResourceId                                     status   = WAVE_MESSAGE_SUCCESS;
                message3.addToSchemasToBeEmptied (OrmRepository::getWaveCurrentSchema ());
                message3.addToSchemasToBeEmptied (OrmRepository::getWaveStartSchema ());
                message3.setSelectivelyEmptyDatabase (true);

                status = sendSynchronously (&message3);

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot : Could not send message to empty database. Status : " + FrameworkToolKit::localize (status));
                    prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    status = message3.getCompletionStatus ();

                    if (WAVE_MESSAGE_SUCCESS != status)
                    {
                        trace (TRACE_LEVEL_ERROR,   "PersistenceObjectManager::boot : Could not empty database. Status : " + FrameworkToolKit::localize (status));
                        prismAssert (false, __FILE__, __LINE__);
                    }
                }

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
                    pWaveAsynchronousContextForBootPhases->callback ();
                    return;
                }

                //////////////////////////////////////////////////////////////////////////////////////////
                //  Wait for READY SCN
                /////////////////////////////////////////////////////////////////////////////////////////
            }
        }
    }

#if 0
    vector<WaveManagedObject *> *pResultsVec = querySynchronously (UpgradeInfoManagedObject::getClassName());
    prismAssert (pResultsVec != NULL, __FILE__, __LINE__);
    prismAssert (pResultsVec->size () != 0, __FILE__, __LINE__);

    trace (TRACE_LEVEL_DEVEL, string("PersistenceObjectManager::boot : Retrieved ") + pResultsVec->size () + string(" upgradeInfoMO from database"));

    trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::boot : Updating tableIds from UpgradeInfoMO for Orm Tables.");
    vector<pair<string, UI32> > tableClassIdTuples;
    for (vector<WaveManagedObject *>::iterator iter = pResultsVec->begin ();
          iter < pResultsVec->end (); ++iter)
    {

        UpgradeInfoManagedObject *upgradeMO = dynamic_cast<UpgradeInfoManagedObject*>(*iter);
        prismAssert (upgradeMO != NULL, __FILE__, __LINE__);

        tableClassIdTuples.push_back (pair<string, UI32>(upgradeMO->getManagedObjectName (), upgradeMO->getManagedObjectClassId ()));
    }
    (OrmRepository::getInstance ())->updateTableId (tableClassIdTuples);

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResultsVec);
#endif

    if (true == FrameworkToolKit::getIsDbRestoreIncomplete ())
    {
        trace (TRACE_LEVEL_WARN, "PersistenceObjectManager::boot : Found that DB restore was incomplete. Hence, restoring the db again");
        restoreDataBase();    
        FrameworkToolKit::rollbackCfgFile ();
    }

    updateInstanceId ();

    //create the Persistance Managed Object entry in the DB

    if ((WAVE_BOOT_FIRST_TIME_BOOT != bootReason) && (WAVE_BOOT_PERSISTENT_BOOT != bootReason))
    {
        // query Persistance Managed Object from DB and save in memory
        vector<WaveManagedObject *> *pResults = querySynchronously (ConfigurationManagedObject::getClassName ());

        prismAssert (1 == (pResults->size ()), __FILE__, __LINE__);

        ConfigurationManagedObject *pConfigurationManagedObject = dynamic_cast<ConfigurationManagedObject *> ((*pResults)[0]);

        prismAssert (NULL != pConfigurationManagedObject, __FILE__, __LINE__);

        m_pConfigurationManagedObject = pConfigurationManagedObject;    
    
        if (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason)
        {
            m_pConfigurationManagedObject->resetConfigurationManagedObject ();

            string sqlForUpdatePersistaneManagedObject = "";

            m_pConfigurationManagedObject->getSqlForUpdate (sqlForUpdatePersistaneManagedObject, OrmRepository::getWaveCurrentSchema ());

            DatabaseObjectManagerExecuteTransactionMessage messageForUpdatePersistaneManagedObject (sqlForUpdatePersistaneManagedObject);
            ResourceId                                     statusForUpdatePersistaneManagedObject  = WAVE_MESSAGE_SUCCESS;

            statusForUpdatePersistaneManagedObject = sendSynchronously (&messageForUpdatePersistaneManagedObject);

            if (WAVE_MESSAGE_SUCCESS != statusForUpdatePersistaneManagedObject)
            {
                trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot : Could not send message to update Entry for Persistance Managed Object during persistant boot with default Status : " + FrameworkToolKit::localize (statusForUpdatePersistaneManagedObject));
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                statusForUpdatePersistaneManagedObject = messageForUpdatePersistaneManagedObject.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != statusForUpdatePersistaneManagedObject)
                {
                    trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::boot : Could not update Entry For Persistance Mananged Object during persistant boot with default. Status : " + FrameworkToolKit::localize (statusForUpdatePersistaneManagedObject));
                    prismAssert (false, __FILE__, __LINE__);
                }
            }

            if (WAVE_MESSAGE_SUCCESS != statusForUpdatePersistaneManagedObject)
            {
                pWaveAsynchronousContextForBootPhases->setCompletionStatus (statusForUpdatePersistaneManagedObject);
                pWaveAsynchronousContextForBootPhases->callback ();
                return;
            }
        }
    }

    // Boot step is successful. synchronize XPathStringManagedObject cache with database.
    // Note : cache is flushed in shutdown.
    flushAndRePopulateXPathStringToXPathStringManagedObjectMapAsCache ();

    //synchronize WaveConfigManagedObject cache with database.
    flushAndRePopulateWaveConfigManagedObjectCache ();
    
    clearDelayedTransactions ();

    UI64 trackingNumber = rand ();
    PersistenceObjectManager::setAnyConfigurationChangeTrackingNumber (trackingNumber);

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void PersistenceObjectManager::getDebugInformation (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation)
{
    string debugInformation = PersistenceStatisticsTracker::getAllTransactionStatistics ();

    if (0 < debugInformation.size ())
    {
        debugInformation = "\nPERSISTENCE OBJECT MANAGER EXECUTE TRANSACTION STATISTICS:\n\n" + debugInformation;

        pWaveAsynchronousContextForDebugInformation->setDebugInformation (debugInformation);
    }

    pWaveAsynchronousContextForDebugInformation->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForDebugInformation->callback ();
}

void PersistenceObjectManager::resetDebugInformation (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation)
{
    PersistenceStatisticsTracker::clearAllTransactionStatistics ();

    pWaveAsynchronousContextForDebugInformation->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForDebugInformation->callback ();
}

void PersistenceObjectManager::initializeConfigurationManagedObjectBeforeBootCompleteMessageHandler (PersistenceObjectManagerInitializeBeforeBootCompleteMessage *pPersistenceObjectManagerInitializeBeforeBootCompleteMessage)
{
    trace (TRACE_LEVEL_INFO, "Initialized Configuration Mananged Object");

    vector<WaveManagedObject *> *pResults = querySynchronously (ConfigurationManagedObject::getClassName ());

    prismAssert (1 == (pResults->size ()), __FILE__, __LINE__);

    ConfigurationManagedObject *pConfigurationManagedObject = dynamic_cast<ConfigurationManagedObject *> ((*pResults)[0]);

    prismAssert (NULL != pConfigurationManagedObject, __FILE__, __LINE__);

    m_pConfigurationManagedObject = pConfigurationManagedObject;

    // Synchronize XPathStringManagedObject cache with database.
    // Note : cache is flushed in shutdown.
    flushAndRePopulateXPathStringToXPathStringManagedObjectMapAsCache ();

    //synchronize WaveConfigManagedObject cache with database.
    flushAndRePopulateWaveConfigManagedObjectCache ();

    clearDelayedTransactions ();

    pPersistenceObjectManagerInitializeBeforeBootCompleteMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pPersistenceObjectManagerInitializeBeforeBootCompleteMessage);
}

void PersistenceObjectManager::updateInstanceId ()
{
    trace (TRACE_LEVEL_DEVEL, "PersistenceObjectManager::updateInstanceId : Updating Latest Instance Ids for Most Base Orm Tables.");

    vector<string> mostBaseTableNames;
    UI32           numberOfMostBaseTables = 0;
    UI32           i                      = 0;
    ResourceId     status                 = WAVE_MESSAGE_SUCCESS;

    OrmRepository::getAllMostBaseTableNames (mostBaseTableNames);
    numberOfMostBaseTables = mostBaseTableNames.size ();

    for (i = 0; i < numberOfMostBaseTables; i++)
    {
        trace (TRACE_LEVEL_DEVEL, "PersistenceObjectManager::boot : Updating " + mostBaseTableNames[i]);

        // Query from Database the latest objectIdInstanceId value and set it in the table.

        DatabaseObjectManagerGetLatestInstanceIdForTableMessage message          (mostBaseTableNames[i]);
        UI32                                                    latestInstanceId = 0;

        status = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            status = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                latestInstanceId = message.getLatestInstanceId ();
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::updateInstanceId : Could not updated the Next Instance Id for table " + mostBaseTableNames[i] + string (" Completion Status : ") + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::updateInstanceId : Could not updated the Next Instance Id for table " + mostBaseTableNames[i] + string ("Status : ") + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }

        trace (TRACE_LEVEL_DEVEL, "PersistenceObjectManager::updateInstanceId : Updated " + mostBaseTableNames[i] + string (" : Latest Instance Id : ") + latestInstanceId);

        OrmRepository::updateLatestInstanceIdForTable (mostBaseTableNames[i], latestInstanceId);
    }
}

void PersistenceObjectManager::updateInstanceIdMessageHandler (PersistenceObjectManagerUpdateInstanceIdMesssage *pPersistenceObjectManagerUpdateInstanceIdMesssage)
{
    trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::updateInstanceIdMessageHandler Entering");

    updateInstanceId ();

    pPersistenceObjectManagerUpdateInstanceIdMesssage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pPersistenceObjectManagerUpdateInstanceIdMesssage);
}


ResourceId PersistenceObjectManager::copySchemaLocal (const string &sourceSchema, const string &destSchema)
{
    DatabaseObjectManagerExecuteCopySchemaMessage message (sourceSchema, destSchema);

    trace (TRACE_LEVEL_INFO, "WaveObjectManager::PersistenceObjectManager : source schema " + sourceSchema + ", dest schema " + destSchema);

    ResourceId status = sendSynchronously (&message);

    if (status != WAVE_MESSAGE_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::PersistenceObjectManager : Could not send message to Persistence Service.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::PersistenceObjectManager : Failed to execute the transaction.  Completion Status : " + FrameworkToolKit::localize (status));
        }
        else
        {
            status = FRAMEWORK_SUCCESS;
        }
    }

    return (status);
}

void PersistenceObjectManager::executeTransactionMessageHandler (PersistenceObjectManagerExecuteTransactionMessage *pPersistenceObjectManagerExecuteTransactionMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForCommittRollbackPhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::executePushWaveConfigurationToFileOnAllLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::executePushWaveConfigurationToKernelOnAllLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerFailedStep),
    };

    PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext = new PersistenceExecuteTransactionContext (pPersistenceObjectManagerExecuteTransactionMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]), pPersistenceObjectManagerExecuteTransactionMessage->getSql ());

    s_currentTransactionId++;

    pPersistenceExecuteTransactionContext->setCurrentTransactionId                  (s_currentTransactionId);

    pPersistenceExecuteTransactionContext->setIsConfigurationChange                 (pPersistenceObjectManagerExecuteTransactionMessage->getIsConfigurationChange ());
    pPersistenceExecuteTransactionContext->setIsConfigurationTimeChange             (pPersistenceObjectManagerExecuteTransactionMessage->getIsConfigurationTimeChange ());
    pPersistenceExecuteTransactionContext->setSenderServiceCode                     (pPersistenceObjectManagerExecuteTransactionMessage->getSenderServiceCode ());
    pPersistenceExecuteTransactionContext->setSenderLocationId                      (pPersistenceObjectManagerExecuteTransactionMessage->getSenderLocationId ());

    pPersistenceExecuteTransactionContext->setConfigNamesForInsert                  (pPersistenceObjectManagerExecuteTransactionMessage->getConfigNamesForInsert ());
    pPersistenceExecuteTransactionContext->setConfigValuesForInsert                 (pPersistenceObjectManagerExecuteTransactionMessage->getConfigValuesForInsert ());
    pPersistenceExecuteTransactionContext->setConfigTypesForInsert                  (pPersistenceObjectManagerExecuteTransactionMessage->getConfigTypesForInsert ());
    pPersistenceExecuteTransactionContext->setConfigNamesForUpdate                  (pPersistenceObjectManagerExecuteTransactionMessage->getConfigNamesForUpdate ());
    pPersistenceExecuteTransactionContext->setConfigValuesForUpdate                 (pPersistenceObjectManagerExecuteTransactionMessage->getConfigValuesForUpdate ());
    pPersistenceExecuteTransactionContext->setConfigTypesForUpdate                  (pPersistenceObjectManagerExecuteTransactionMessage->getConfigTypesForUpdate ());

    pPersistenceExecuteTransactionContext->holdAll ();
    pPersistenceExecuteTransactionContext->start ();
}

void PersistenceObjectManager::executePushWaveConfigurationToFileOnAllLocationsStep (PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::executePushWaveConfigurationToFileOnAllLocationsStep : Entering ...");

    ResourceId                                         status                                             = WAVE_MESSAGE_SUCCESS;

    // check if Wave configuration changed, then only update the file on all nodes
    if ( 0 == pPersistenceExecuteTransactionContext->getConfigNamesForInsert ().size () &&
         0 == pPersistenceExecuteTransactionContext->getConfigNamesForUpdate ().size ())
    {
        pPersistenceExecuteTransactionContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    // get Serialized string of Wave Config Managed objects.
    string serializedConfigs = getWaveConfigSerializedData ();
    string configurationFileName = PrismFrameworkObjectManager::getGlobalConfigurationFileName ();

    ofstream configurationFileToWrite (configurationFileName.c_str ());
                                        
    if (true == (configurationFileToWrite.is_open ()))
    {                                       
        configurationFileToWrite << serializedConfigs;
        configurationFileToWrite.close ();
        
        status = WAVE_MESSAGE_SUCCESS;
    }                                   
    else                                
    {                                       
        trace (TRACE_LEVEL_ERROR, "PersistenceObjectManager::executePushWaveConfigurationToFileOnAllLocationsStep : Cannot open file : " + configurationFileName);
        status = WAVE_MESSAGE_ERROR;        
        pPersistenceExecuteTransactionContext->executeNextStep (status);
        return ;
    }

    vector<string> sourceFilenames;
    vector<string> destFilenames;
    UI32           fileTransferFlag = FILE_OVERWRITE_DEST_FILE_IF_EXIST | FILE_DEST_PATH_ABSOLUTE | FILE_PUSH_TO_HA_PEER;
    bool           disconnectFromFailedNode = true;
    sourceFilenames.push_back (PrismFrameworkObjectManager::getGlobalConfigurationFileName ());
    destFilenames.push_back (PrismFrameworkObjectManager::getGlobalConfigurationFileName ());

    status = FrameworkToolKit::pushFilesToAllNodes (sourceFilenames, destFilenames, fileTransferFlag, disconnectFromFailedNode);
    if (WAVE_MESSAGE_SUCCESS != status)
    {   
        trace (TRACE_LEVEL_ERROR, string("PersistenceObjectManager::executePushWaveConfigurationToFileOnAllLocationsStep : Unable to sync config"));
    }

    pPersistenceExecuteTransactionContext->executeNextStep (status);
}

void PersistenceObjectManager::executePushWaveConfigurationToKernelOnAllLocationsStep (PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::executePushWaveConfigurationToKernelOnAllLocationsStep : Entering ...");

    // check if Wave configuration changed, then only update the kernel on all nodes
    if ( 0 == pPersistenceExecuteTransactionContext->getConfigNamesForInsert ().size () &&
         0 == pPersistenceExecuteTransactionContext->getConfigNamesForUpdate ().size ())
    {
        pPersistenceExecuteTransactionContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    // get Serialized string of Wave Config Managed objects.
    string serializedConfigs = getWaveConfigSerializedData ();

    PersistenceLocalObjectManagerPushConfigToKernelMessaage *pPushConfigToKernelMessage = new PersistenceLocalObjectManagerPushConfigToKernelMessaage (serializedConfigs);

    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PersistenceObjectManager::pushConfigToKernelMessageCallback), pPersistenceExecuteTransactionContext);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pPushConfigToKernelMessage);

   // Nodes will reboot and form a new cluster as a result of "copy default to startup".
    sendToWaveCluster (pWaveSendToClusterContext); 
    //pPersistenceExecuteTransactionContext->executeNextStep (status);
}

void PersistenceObjectManager::pushConfigToKernelMessageCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext = reinterpret_cast<PersistenceExecuteTransactionContext *> (pWaveSendToClusterContext->getPCallerContext ());

    trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::pushConfigToKernelMessageCallback : status " + FrameworkToolKit::localize (sendToClusterCompletionStatus));

    prismAssert (NULL != pPersistenceExecuteTransactionContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    pPersistenceExecuteTransactionContext->executeNextStep (sendToClusterCompletionStatus);
}

void PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseStep (PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext)
{
    ResourceId                                         status                                             = WAVE_MESSAGE_ERROR;
    vector<LocationId>                                &currentLocations                                   = pPersistenceExecuteTransactionContext->getCurrentLocationIds ();
    UI32                                               numberOfCurrentLocations                           = 0;
    UI32                                               currentTransactionId                               = pPersistenceExecuteTransactionContext->getCurrentTransactionId ();
    LocationId                                         thisLocationId                                     = FrameworkToolKit::getThisLocationId ();
    string                                             tempSql                                            = pPersistenceExecuteTransactionContext->getSql ();
    UI64                                               tempConfigurationNumber                            = 0;
    DateTime                                           tempDateTimeForConfiguration;
    DateTime                                           tempDateTimeForLatestUpdate;
    ConfigurationManagedObject                        *pConfigurationManagedObject                        = NULL;

    PersistenceObjectManagerExecuteTransactionMessage *pPersistenceObjectManagerExecuteTransactionMessage = dynamic_cast<PersistenceObjectManagerExecuteTransactionMessage *> (pPersistenceExecuteTransactionContext->getPPrismMessage ());
    prismAssert (NULL != pPersistenceObjectManagerExecuteTransactionMessage, __FILE__, __LINE__);

    FrameworkToolKit::getFullyConnectedLocations (currentLocations);
    currentLocations.push_back (thisLocationId);
    numberOfCurrentLocations = currentLocations.size ();

//    trace (TRACE_LEVEL_INFO, string ("PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseStep number of locations") + numberOfCurrentLocations);
    // check if configuration changed flag is true then, update the Persistance Managed Object

    vector<string> configNamesForInsert  = pPersistenceExecuteTransactionContext->getConfigNamesForInsert ();
    vector<string> configValuesForInsert = pPersistenceExecuteTransactionContext->getConfigValuesForInsert ();
    vector<ResourceId> configTypesForInsert  = pPersistenceExecuteTransactionContext->getConfigTypesForInsert ();
    vector<string> configNamesForUpdate  = pPersistenceExecuteTransactionContext->getConfigNamesForUpdate ();
    vector<string> configValuesForUpdate = pPersistenceExecuteTransactionContext->getConfigValuesForUpdate ();
    vector<ResourceId> configTypesForUpdate  = pPersistenceExecuteTransactionContext->getConfigTypesForUpdate ();

    UI32 numberOfConfigNamesToInsert = configNamesForInsert.size ();
    UI32 numberOfConfigNamesToUpdate = configNamesForUpdate.size ();
    DateTime updateTimeStamp;

    for (UI32 i = 0; i < numberOfConfigNamesToInsert; i++)
    {
        WaveConfigManagedObject *pWaveConfigManagedObject = new WaveConfigManagedObject (this);

        pWaveConfigManagedObject->setConfigName (configNamesForInsert[i]); 
        pWaveConfigManagedObject->setConfigValue (configValuesForInsert[i]);
        pWaveConfigManagedObject->setConfigAttributeType (configTypesForInsert[i]);
        pWaveConfigManagedObject->setLastModifiedTimeStamp (updateTimeStamp);

        addWaveConfigManagedObjectToCache (pWaveConfigManagedObject);
        pWaveConfigManagedObject->getSqlForInsert (tempSql, OrmRepository::getWaveCurrentSchema ());
    }

    for (UI32 i = 0; i < numberOfConfigNamesToUpdate; i++)
    {
        WaveConfigManagedObject *pWaveConfigManagedObject = getWaveConfigManagedObjectFromCache (configNamesForUpdate[i]);

        if (NULL == pWaveConfigManagedObject)
        {
            trace (TRACE_LEVEL_ERROR, string ("PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseStep : Trying to update a config which doesn't exist. ConfigName = ") + configNamesForUpdate[i]);
            prismAssert (false, __FILE__, __LINE__);
        }
        pWaveConfigManagedObject->setConfigName (configNamesForUpdate[i]);
        pWaveConfigManagedObject->setConfigValue (configValuesForUpdate[i]);
        pWaveConfigManagedObject->setConfigAttributeType (configTypesForUpdate[i]);
        pWaveConfigManagedObject->setLastModifiedTimeStamp (updateTimeStamp);

        pWaveConfigManagedObject->getSqlForUpdate (tempSql, OrmRepository::getWaveCurrentSchema ());
    }

    configNamesForInsert.clear ();
    configValuesForInsert.clear ();
    configTypesForInsert.clear ();
    configNamesForUpdate.clear ();
    configValuesForUpdate.clear ();
    configTypesForUpdate.clear ();

    if (true == pPersistenceExecuteTransactionContext->getIsConfigurationChange ())
    {
        if (NULL == m_pConfigurationManagedObject)
        {
            if (true == (PrismFrameworkObjectManager::getInstance ())->getPrimaryNodeClusterOperationFlag ())
            {
                pPersistenceExecuteTransactionContext->executeNextStep (WAVE_MESSAGE_ERROR_CLUSTER_OPERATION_IN_PROGRESS);
                return;
            }
            else
            {
                prismAssert (NULL != m_pConfigurationManagedObject, __FILE__, __LINE__);
            }
        }
		
		prismAssert (NULL != m_pConfigurationManagedObject, __FILE__, __LINE__);
		
		tempConfigurationNumber                = m_pConfigurationManagedObject->getConfigurationNumber ();
        tempDateTimeForConfiguration           = m_pConfigurationManagedObject->getConfigurationTime (); 
        tempDateTimeForLatestUpdate            = m_pConfigurationManagedObject->getLatestUpdateTime (); 
        
        UI64 configurationNumber = thisLocationId;
        configurationNumber = ((configurationNumber << 32) | (currentTransactionId));
        DateTime currentTimeStamp;
 
        m_pConfigurationManagedObject->setConfigurationNumber (configurationNumber);
        m_pConfigurationManagedObject->setConfigurationTime (currentTimeStamp);
        m_pConfigurationManagedObject->setLatestUpdateTime (currentTimeStamp);

        //creating a temporary variable to re-store the in-memory value if commit
        pConfigurationManagedObject = new ConfigurationManagedObject (this);
        pConfigurationManagedObject->setConfigurationNumber (tempConfigurationNumber);
        pConfigurationManagedObject->setConfigurationTime(tempDateTimeForConfiguration);
        pConfigurationManagedObject->setLatestUpdateTime (tempDateTimeForLatestUpdate);
       
        m_pConfigurationManagedObject->getSqlForUpdate (tempSql, OrmRepository::getWaveCurrentSchema ());

        pPersistenceExecuteTransactionContext->setConfigurationManagedObject (pConfigurationManagedObject);

    }
     
    // check if configuration time changed flag is true then, update the Persistance Managed Object
             
    else if (true == pPersistenceExecuteTransactionContext->getIsConfigurationTimeChange ())
    {
        if (NULL == m_pConfigurationManagedObject)
        {
            // For now removing the check of primary node cluster operation flag
            // If in the middle of cluster operation the in-memory object may not be populated yet
            pPersistenceExecuteTransactionContext->executeNextStep (WAVE_MESSAGE_ERROR_CLUSTER_OPERATION_IN_PROGRESS);
            return;
        }
        
        tempDateTimeForLatestUpdate = m_pConfigurationManagedObject->getLatestUpdateTime (); 
        
        DateTime currentTimeStamp;
        m_pConfigurationManagedObject->setLatestUpdateTime (currentTimeStamp);

        //creating a temporary variable to re-store the in-memory value if commit
        pConfigurationManagedObject = new ConfigurationManagedObject (this);
        pConfigurationManagedObject->setLatestUpdateTime (tempDateTimeForLatestUpdate);
       
        m_pConfigurationManagedObject->getSqlForUpdate (tempSql, OrmRepository::getWaveCurrentSchema ());

        pPersistenceExecuteTransactionContext->setConfigurationManagedObject (pConfigurationManagedObject);

    }

    // get sql to update path string objects.

    vector<string> xPathStringsVectorForTimestampUpdate = pPersistenceObjectManagerExecuteTransactionMessage->getXPathStringsVectorForTimestampUpdate ();
    map<string, DateTime> &xPathStringToOriginalDatetimestampMap = pPersistenceExecuteTransactionContext->getxPathStringToOriginalDatetimestampMap ();
    tempSql += getSqlToUpdateXPathStringObjects (xPathStringsVectorForTimestampUpdate, xPathStringToOriginalDatetimestampMap);

    // Append any delayed SQL transactions.

    tempSql += getDelayedTransactions ();

    // If there is only one location do a simple transaction.  If there is more than one location, then do two phase commit.

    if (1 == numberOfCurrentLocations || (LOCATION_STAND_ALONE == FrameworkToolKit::getThisLocationRole ()) || (LOCATION_PRIMARY_UNCONFIRMED == FrameworkToolKit::getThisLocationRole ()) || (LOCATION_SECONDARY_UNCONFIRMED == FrameworkToolKit::getThisLocationRole ()))
    {
        PersistenceStatisticsTracker::incrementCommitTransactionsRequested (pPersistenceExecuteTransactionContext->getSenderServiceCode ());

        PersistenceLocalObjectManagerExecuteTransactionMessage *pPersistenceLocalObjectManagerExecuteTransactionMessage = new PersistenceLocalObjectManagerExecuteTransactionMessage (tempSql, currentTransactionId);


        pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsCommit (true);
        pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsConfigurationChange (pPersistenceExecuteTransactionContext->getIsConfigurationChange ());
        pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsConfigurationTimeChange (pPersistenceExecuteTransactionContext->getIsConfigurationTimeChange ());

        pPersistenceLocalObjectManagerExecuteTransactionMessage->setAnyConfigurationChangeTrackingNumber (PersistenceObjectManager::getAnyConfigurationChangeTrackingNumber ());

        m_anyConfiguraitonChangeTrackingNumberMutex.lock ();
        m_anyConfiguraitonChangeTrackingNumber++;
        m_anyConfiguraitonChangeTrackingNumberMutex.unlock ();

        status = send (pPersistenceLocalObjectManagerExecuteTransactionMessage, reinterpret_cast<PrismMessageResponseHandler> (&PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseCallback), pPersistenceExecuteTransactionContext);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            if (true == pPersistenceExecuteTransactionContext->getIsConfigurationChange ())
            {
                m_pConfigurationManagedObject->setConfigurationNumber (tempConfigurationNumber);
                m_pConfigurationManagedObject->setConfigurationTime (tempDateTimeForConfiguration);  
                m_pConfigurationManagedObject->setLatestUpdateTime (tempDateTimeForLatestUpdate);
            }
            else if (true == pPersistenceExecuteTransactionContext->getIsConfigurationTimeChange ())
            {
                m_pConfigurationManagedObject->setLatestUpdateTime (tempDateTimeForLatestUpdate);
            }

            if (NULL != pConfigurationManagedObject)            
            {
                delete pConfigurationManagedObject;
            }

            pPersistenceExecuteTransactionContext->executeNextStep (status);
            return;
        }
    }
    else
    {
        PersistenceStatisticsTracker::incrementPrepareTransactionsRequested (pPersistenceExecuteTransactionContext->getSenderServiceCode (), numberOfCurrentLocations);

//        trace (TRACE_LEVEL_DEBUG, string ("PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseStep : Doing Transaction on ") + numberOfCurrentLocations + string (" Locations"));

        string prepareTransactionString ("BEGIN;");

        prepareTransactionString = prepareTransactionString + tempSql + "PREPARE TRANSACTION 'TX" + thisLocationId + string (".") + currentTransactionId + string ("';");

        //trace (TRACE_LEVEL_INFO, prepareTransactionString);

        PersistenceLocalObjectManagerExecuteTransactionMessage *pPersistenceLocalObjectManagerExecuteTransactionMessage = new PersistenceLocalObjectManagerExecuteTransactionMessage (prepareTransactionString, currentTransactionId);

        pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsPrepare (true);
        pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsConfigurationChange (pPersistenceExecuteTransactionContext->getIsConfigurationChange ());
        pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsConfigurationTimeChange (pPersistenceExecuteTransactionContext->getIsConfigurationTimeChange ());

        WaveSendMulticastContext *pWaveSendMulticastContext = new WaveSendMulticastContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseCallback2), pPersistenceExecuteTransactionContext);

        pWaveSendMulticastContext->setPrismMessage (pPersistenceLocalObjectManagerExecuteTransactionMessage);
        pWaveSendMulticastContext->setAllLocationsToSent (currentLocations);

        sendMulticast (pWaveSendMulticastContext);
    }
}

void PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseCallback (FrameworkStatus frameworkStatus, PersistenceLocalObjectManagerExecuteTransactionMessage *pPersistenceLocalObjectManagerExecuteTransactionMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseCallback : Entering ...");

    PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext = reinterpret_cast<PersistenceExecuteTransactionContext *> (pContext);
    ResourceId                            status                                = WAVE_MESSAGE_ERROR;

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        status = pPersistenceLocalObjectManagerExecuteTransactionMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            PersistenceStatisticsTracker::incrementCommitTransactionsExecuted (pPersistenceLocalObjectManagerExecuteTransactionMessage->getSenderServiceCode ());
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseCallback : Message to Database Service failed, status : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseCallback : Message to Database Service failed, frameworkStatus : " + FrameworkToolKit::localize (frameworkStatus));

        status = frameworkStatus;
    }

    delete pPersistenceLocalObjectManagerExecuteTransactionMessage;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        map<string, DateTime> &xPathStringToOriginalDatetimestampMap = pPersistenceExecuteTransactionContext->getxPathStringToOriginalDatetimestampMap ();
        restoreOriginalTimestampsForCachedXPathStringManagedObjects (xPathStringToOriginalDatetimestampMap);
    }

    pPersistenceExecuteTransactionContext->executeNextStep (status);
}

void PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseCallback2 (WaveSendMulticastContext *pWaveSendMulticastContext)
{
//    trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForPreparePhaseCallback2 : Entering ...");

    PersistenceExecuteTransactionContext               *pPersistenceExecuteTransactionContext               = reinterpret_cast<PersistenceExecuteTransactionContext *> (pWaveSendMulticastContext->getPCallerContext ());
    ResourceId                                          status                                              = WAVE_MESSAGE_ERROR;
    map<LocationId, ResourceId>                        &transactionStatusForCurrentLocations                = pPersistenceExecuteTransactionContext->getTransactionStatusForCurrentLocations ();
    UI32							                    senderLocationId	                    			= pPersistenceExecuteTransactionContext->getSenderLocationId ();
    vector<LocationId>                                  locationIdUsedInTransaction                         = pWaveSendMulticastContext->getAllLocationsToSent ();
    UI32                                                numberOfLocationUsedInTransaction                   = locationIdUsedInTransaction.size ();

//    trace (TRACE_LEVEL_INFO, string ("number of locations Used  in Transaction ") + numberOfLocationUsedInTransaction);

    for (UI32 i = 0; i < numberOfLocationUsedInTransaction; i++)
    {
        ResourceId completionStatusForLocation = pWaveSendMulticastContext->getStatusForALocation (locationIdUsedInTransaction [i]);

//        trace (TRACE_LEVEL_INFO, string ("Status for location ") + locationIdUsedInTransaction [i] + string (" is ") + FrameworkToolKit::localize (completionStatusForLocation));

        if (WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION == completionStatusForLocation || WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE == completionStatusForLocation)
        {
            if (FrameworkToolKit::isPrimaryLocation () && (senderLocationId != locationIdUsedInTransaction [i]))
            {
                transactionStatusForCurrentLocations[locationIdUsedInTransaction [i]] = WAVE_MESSAGE_SUCCESS;
        
                PersistenceStatisticsTracker::incrementPrepareTransactionsMarkedExecutedOnPrimary (pPersistenceExecuteTransactionContext->getSenderServiceCode ());
            }
            else
            {
                transactionStatusForCurrentLocations[locationIdUsedInTransaction [i]] = completionStatusForLocation;

                PersistenceStatisticsTracker::incrementPrepareTransactionsMarkedDisconnectedOnSecondary (pPersistenceExecuteTransactionContext->getSenderServiceCode ());
            }

            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationIdUsedInTransaction [i]);
        }
        else
        {
            transactionStatusForCurrentLocations[locationIdUsedInTransaction [i]] = completionStatusForLocation;
        }
    }
   
    status = WAVE_MESSAGE_SUCCESS; 

    delete pWaveSendMulticastContext;

    pPersistenceExecuteTransactionContext->executeNextStep (status);
}

void PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForCommittRollbackPhaseStep (PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext)
{
//    trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForCommittRollbackPhaseStep : Entering ...");

    vector<LocationId>                                &currentLocations                                   = pPersistenceExecuteTransactionContext->getCurrentLocationIds ();
    map<LocationId, ResourceId>                       &transactionStatusForCurrentLocations               = pPersistenceExecuteTransactionContext->getTransactionStatusForCurrentLocations ();
    UI32                                               numberOfCurrentLocations                           = currentLocations.size ();
    UI32                                               i                                                  = 0;
    UI32                                               currentTransactionId                               = pPersistenceExecuteTransactionContext->getCurrentTransactionId ();
    bool                                               allSuccessFul                                      = true;
    LocationId                                         thisLocationId                                     = FrameworkToolKit::getThisLocationId ();
    ConfigurationManagedObject                        *pConfigurationManagedObject                        = pPersistenceExecuteTransactionContext->getConfigurationManagedObject ();

    if (1 == numberOfCurrentLocations)
    {
        // there is only location - We will not do two phase commit.  So, if we reached here the transaction is already successfully committed.

        clearDelayedTransactions ();

        pPersistenceExecuteTransactionContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    for (i = 0; i < numberOfCurrentLocations; i++)
    {
        if (WAVE_MESSAGE_SUCCESS != (transactionStatusForCurrentLocations[currentLocations[i]]))
        {
            allSuccessFul = false;
            break;
        }
    }

//    trace (TRACE_LEVEL_INFO, string ("Nummber of locations ") + numberOfCurrentLocations);

    if (true == allSuccessFul)
    {
//        trace (TRACE_LEVEL_INFO, "All Succes Case");
        PersistenceStatisticsTracker::incrementCommitTransactionsRequested (pPersistenceExecuteTransactionContext->getSenderServiceCode (), numberOfCurrentLocations);

        string committPreparedSql = string ("COMMIT PREPARED 'TX") + thisLocationId + string (".") + currentTransactionId + string ("';");

        //trace (TRACE_LEVEL_INFO, committPreparedSql);

        PersistenceLocalObjectManagerExecuteTransactionMessage *pPersistenceLocalObjectManagerExecuteTransactionMessage = new PersistenceLocalObjectManagerExecuteTransactionMessage (committPreparedSql, currentTransactionId);

        pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsCommit (true);
        pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsConfigurationChange (pPersistenceExecuteTransactionContext->getIsConfigurationChange ());
        pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsConfigurationTimeChange (pPersistenceExecuteTransactionContext->getIsConfigurationTimeChange ());

        pPersistenceLocalObjectManagerExecuteTransactionMessage->setAnyConfigurationChangeTrackingNumber (PersistenceObjectManager::getAnyConfigurationChangeTrackingNumber ());

        m_anyConfiguraitonChangeTrackingNumberMutex.lock ();
        m_anyConfiguraitonChangeTrackingNumber++;
        m_anyConfiguraitonChangeTrackingNumberMutex.unlock ();

        WaveSendMulticastContext *pWaveSendMulticastContext = new WaveSendMulticastContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForCommittCallback), pPersistenceExecuteTransactionContext);

        pWaveSendMulticastContext->setPrismMessage (pPersistenceLocalObjectManagerExecuteTransactionMessage);
        pWaveSendMulticastContext->setAllLocationsToSent (currentLocations);

        sendMulticast (pWaveSendMulticastContext);
    }
    else
    {
//        trace (TRACE_LEVEL_INFO, "Some nodes failed");
        PersistenceStatisticsTracker::incrementRollbackTransactionsRequested (pPersistenceExecuteTransactionContext->getSenderServiceCode (), numberOfCurrentLocations);

        if (true == pPersistenceExecuteTransactionContext->getIsConfigurationChange ())
        {
            if (NULL != pConfigurationManagedObject)
            {
                m_pConfigurationManagedObject->setConfigurationNumber (pConfigurationManagedObject->getConfigurationNumber ());
                m_pConfigurationManagedObject->setConfigurationTime (pConfigurationManagedObject->getConfigurationTime ());
                m_pConfigurationManagedObject->setLatestUpdateTime (pConfigurationManagedObject->getLatestUpdateTime ());
            }
        }
        else if (true == pPersistenceExecuteTransactionContext->getIsConfigurationTimeChange ())
        {
            if (NULL != pConfigurationManagedObject)
            {
                m_pConfigurationManagedObject->setLatestUpdateTime (pConfigurationManagedObject->getLatestUpdateTime ());
            }
        }

        map<string, DateTime> &xPathStringToOriginalDatetimestampMap = pPersistenceExecuteTransactionContext->getxPathStringToOriginalDatetimestampMap ();
        restoreOriginalTimestampsForCachedXPathStringManagedObjects (xPathStringToOriginalDatetimestampMap);

        string rollbackPreparedSql = string ("ROLLBACK PREPARED 'TX") + thisLocationId + string (".") + currentTransactionId + string ("';");
        string rollbackSql         = string ("ROLLBACK;");

        //trace (TRACE_LEVEL_INFO, rollbackPreparedSql);

        ++(*pPersistenceExecuteTransactionContext);

        vector<LocationId> locationForRollbackPrepared;
        vector<LocationId> locationForRollback;

        for (i = 0; i < numberOfCurrentLocations; i++)
        {
            if (WAVE_MESSAGE_SUCCESS == transactionStatusForCurrentLocations[currentLocations[i]])
            {
                locationForRollbackPrepared.push_back (currentLocations[i]);
            }
            else
            {
                locationForRollback.push_back (currentLocations[i]);
            }

        }

        if (0 < locationForRollbackPrepared.size ())
        {
            ++(*pPersistenceExecuteTransactionContext);

            WaveSendMulticastContext *pRollbackPreparedSendMulticastContext = new WaveSendMulticastContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForRollbackCallback), pPersistenceExecuteTransactionContext);

            PersistenceLocalObjectManagerExecuteTransactionMessage *pPersistenceLocalObjectManagerExecuteTransactionMessage = new PersistenceLocalObjectManagerExecuteTransactionMessage (rollbackPreparedSql, currentTransactionId);
            pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsRollback (true);
            pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsConfigurationChange (pPersistenceExecuteTransactionContext->getIsConfigurationChange ());
            pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsConfigurationTimeChange (pPersistenceExecuteTransactionContext->getIsConfigurationTimeChange ());

            pRollbackPreparedSendMulticastContext->setPrismMessage (pPersistenceLocalObjectManagerExecuteTransactionMessage);
            pRollbackPreparedSendMulticastContext->setAllLocationsToSent (locationForRollbackPrepared);

            sendMulticast (pRollbackPreparedSendMulticastContext);
        }

        if (0 < locationForRollback.size ())
        {
            ++(*pPersistenceExecuteTransactionContext);

            WaveSendMulticastContext *pRollbackSendMulticastContext = new WaveSendMulticastContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForRollbackCallback), pPersistenceExecuteTransactionContext);

            PersistenceLocalObjectManagerExecuteTransactionMessage *pPersistenceLocalObjectManagerExecuteTransactionMessage = new PersistenceLocalObjectManagerExecuteTransactionMessage (rollbackSql, currentTransactionId);
            pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsRollback (true);
            pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsConfigurationChange (pPersistenceExecuteTransactionContext->getIsConfigurationChange ());
            pPersistenceLocalObjectManagerExecuteTransactionMessage->setIsConfigurationTimeChange (pPersistenceExecuteTransactionContext->getIsConfigurationTimeChange ());

            pRollbackSendMulticastContext->setPrismMessage (pPersistenceLocalObjectManagerExecuteTransactionMessage);
            pRollbackSendMulticastContext->setAllLocationsToSent (locationForRollback);

            sendMulticast (pRollbackSendMulticastContext);
        }

        --(*pPersistenceExecuteTransactionContext);
    }
}

void PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForCommittCallback (WaveSendMulticastContext *pWaveSendMulticastContext)
{
//    trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForCommittRollbackCallback : Entering ...");

    PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext = reinterpret_cast<PersistenceExecuteTransactionContext *> (pWaveSendMulticastContext->getPCallerContext ());
    vector<LocationId>                    locationIdUsedInTransaction           = pWaveSendMulticastContext->getAllLocationsToSent ();
    UI32                                  numberOfLocationUsedInTransaction     = locationIdUsedInTransaction.size ();

    for (UI32 i = 0; i < numberOfLocationUsedInTransaction; i++)
    {
        if (WAVE_MESSAGE_SUCCESS == pWaveSendMulticastContext->getStatusForALocation (locationIdUsedInTransaction [i]))
        {
            PersistenceStatisticsTracker::incrementCommitTransactionsExecuted (pPersistenceExecuteTransactionContext->getSenderServiceCode ());
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForCommittCallback : Message to Database Service failed, Location Id : ") + locationIdUsedInTransaction [i] + string (", status : ") + FrameworkToolKit::localize (pWaveSendMulticastContext->getStatusForALocation (locationIdUsedInTransaction [i])));
        }
    }
    
    delete pWaveSendMulticastContext;

    clearDelayedTransactions ();

    pPersistenceExecuteTransactionContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForRollbackCallback (WaveSendMulticastContext *pWaveSendMulticastContext)
{
//    trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForRollbackCallback : Entering ...");

    PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext = reinterpret_cast<PersistenceExecuteTransactionContext *> (pWaveSendMulticastContext->getPCallerContext ());
    vector<LocationId>                    locationIdUsedInTransaction           = pWaveSendMulticastContext->getAllLocationsToSent ();
    UI32                                  numberOfLocationUsedInTransaction     = locationIdUsedInTransaction.size ();

    --(*pPersistenceExecuteTransactionContext);

    for (UI32 i = 0; i < numberOfLocationUsedInTransaction; i++)
    {
        if (WAVE_MESSAGE_SUCCESS == pWaveSendMulticastContext->getStatusForALocation (locationIdUsedInTransaction [i]))
        {
            PersistenceStatisticsTracker::incrementRollbackTransactionsExecuted (pPersistenceExecuteTransactionContext->getSenderServiceCode ());                
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("PersistenceObjectManager::executeTransactionSendRequestToAllLocationsForRollbackCallback : Message to Database Service failed, Location Id : ") + locationIdUsedInTransaction [i] + string (", status : ") + FrameworkToolKit::localize (pWaveSendMulticastContext->getStatusForALocation (locationIdUsedInTransaction [i])));
        }
    }

    delete pWaveSendMulticastContext;
    pPersistenceExecuteTransactionContext->executeNextStep (WAVE_COMMIT_TRANSACTION_FAILED);
}

void PersistenceObjectManager::savePrismConfigurationAtAllLocationsStep (PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext)
{
    PersistenceLocalObjectManagerSavePrismConfigurationMessage  *pSavePrismConfigurationMessage = new PersistenceLocalObjectManagerSavePrismConfigurationMessage (pPersistenceExecuteTransactionContext->getIsStartupValid ());

    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PersistenceObjectManager::savePrismConfigurationAtAllLocationsCallback), pPersistenceExecuteTransactionContext);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pSavePrismConfigurationMessage);
       
    trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::savePrismConfigurationAtAllLocationsStep : sending to cluster");
    sendToWaveCluster (pWaveSendToClusterContext);
}

void PersistenceObjectManager::savePrismConfigurationAtAllLocationsCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                          sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext = reinterpret_cast<PersistenceExecuteTransactionContext *> (pWaveSendToClusterContext->getPCallerContext ());

    trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::savePrismConfigurationAtAllLocationsCallback : status " + FrameworkToolKit::localize (sendToClusterCompletionStatus));

    prismAssert (NULL != pPersistenceExecuteTransactionContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    pPersistenceExecuteTransactionContext->executeNextStep (sendToClusterCompletionStatus);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copy Default to Start
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PersistenceObjectManager::copyFileMessageHandler (PersistenceObjectManagerCopyFileMessage *pPersistenceObjectManagerCopyFileMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::copyFileSendRequestToAllLocationsForPreparePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::savePrismConfigurationAtAllLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerFailedStep),
    };

    PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext = new PersistenceExecuteTransactionContext (pPersistenceObjectManagerCopyFileMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]), "");

    s_currentTransactionId++;

    pPersistenceExecuteTransactionContext->setCurrentTransactionId (s_currentTransactionId);
    pPersistenceExecuteTransactionContext->setStartupFileName (pPersistenceObjectManagerCopyFileMessage->getStartupFileName ());
    pPersistenceExecuteTransactionContext->setStartupFileType (pPersistenceObjectManagerCopyFileMessage->getStartupFileType ());
    pPersistenceExecuteTransactionContext->setIsLocalFileCopy (pPersistenceObjectManagerCopyFileMessage->getIsLocalFileCopy ());
    pPersistenceExecuteTransactionContext->setIsStartupValid  (false);

    pPersistenceExecuteTransactionContext->holdAll ();
    pPersistenceExecuteTransactionContext->start ();
}

void PersistenceObjectManager::copyFileSendRequestToAllLocationsForPreparePhaseStep (PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::copyFileSendRequestToAllLocationsForPreparePhaseStep : Entering ...");

    ResourceId  status = WAVE_MESSAGE_ERROR;

    // If there is only one location do a simple transaction.  If there are more than one location, then do two phase commit.

    // If MC is enabled, but there is single node, still send PersistenceLocalObjectManagerCopyDefaultClusterMessage, which will reboot the switch.

    bool clusterEnabled = Wave::clusterEnabledCheck ();

    // If the message has come for a local copy (as part of vcs mode change : vcs enable, no vcs enable) then do only local send.

    bool isLocalFileCopyOperation = pPersistenceExecuteTransactionContext->getIsLocalFileCopy ();

    if (((LOCATION_STAND_ALONE == FrameworkToolKit::getThisLocationRole ()) && (false == clusterEnabled)) || (true == isLocalFileCopyOperation))
    {
        PersistenceLocalObjectManagerSetStartupFileMessage *pPersistenceLocalObjectManagerSetStartupFileMessage = new PersistenceLocalObjectManagerSetStartupFileMessage (pPersistenceExecuteTransactionContext->getStartupFileType (), pPersistenceExecuteTransactionContext->getStartupFileName ());

        status = send (pPersistenceLocalObjectManagerSetStartupFileMessage, reinterpret_cast<PrismMessageResponseHandler> (&PersistenceObjectManager::copyFileRequestCallback), pPersistenceExecuteTransactionContext);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, ("PersistenceObjectManager::copyFileSendRequestToAllLocationsForPreparePhaseStep: error : " + FrameworkToolKit::localize (status)));
            pPersistenceExecuteTransactionContext->executeNextStep (status);
            return;
        }
    }
    else
    {
        PersistenceLocalObjectManagerCopyDefaultClusterMessage  *pCopyDefaultClusterMessage = new PersistenceLocalObjectManagerCopyDefaultClusterMessage ();

        WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PersistenceObjectManager::copyFileSendRequestToAllLocationsForPreparePhaseCallback2), pPersistenceExecuteTransactionContext);

        pWaveSendToClusterContext->setPPrismMessageForPhase1 (pCopyDefaultClusterMessage);

        trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::copyFileSendRequestToAllLocationsForPreparePhaseStep : sending to cluster");
    
        // broadcast startup schema change event from Dcmd in case of MC mode and before nodes get rebooted.
        FrameworkToolKit::broadcastStartupSchemaChangeEvent (DEFAULT_SCHEMA);

       // Nodes will reboot and form a new cluster as a result of "copy default to startup".
        sendToWaveCluster (pWaveSendToClusterContext); 
    }
}

void PersistenceObjectManager::copyFileRequestCallback (FrameworkStatus frameworkStatus, PersistenceLocalObjectManagerSetStartupFileMessage *pPersistenceLocalObjectManagerSetStartupFileMessage, void *pContext)
{
    PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext = reinterpret_cast<PersistenceExecuteTransactionContext *> (pContext);
    ResourceId                            status                                = WAVE_MESSAGE_ERROR;

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        status = pPersistenceLocalObjectManagerSetStartupFileMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            // Nothing to do here for now.
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "PersistenceObjectManager::copyFileRequestCallback : Message to Database Service failed, status : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "PersistenceObjectManager::copyFileRequestCallback : Message to Database Service failed, frameworkStatus : " + FrameworkToolKit::localize (frameworkStatus));

        status = frameworkStatus;
    }

    delete pPersistenceLocalObjectManagerSetStartupFileMessage;

    pPersistenceExecuteTransactionContext->executeNextStep (status);
}

void PersistenceObjectManager::copyFileSendRequestToAllLocationsForPreparePhaseCallback2 ( WaveSendToClusterContext *pWaveSendToClusterContext )
{
    ResourceId                              sendToClusterCompletionStatus           = pWaveSendToClusterContext->getCompletionStatus ();
    PersistenceExecuteTransactionContext   *pPersistenceExecuteTransactionContext   = reinterpret_cast<PersistenceExecuteTransactionContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pPersistenceExecuteTransactionContext, __FILE__, __LINE__);

    trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::copyFileSendRequestToAllLocationsForPreparePhaseCallback2 : status " + FrameworkToolKit::localize (sendToClusterCompletionStatus));

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    pPersistenceExecuteTransactionContext->executeNextStep (sendToClusterCompletionStatus);
}

/*********************************************************************************************************************
* Name        : compareDBMessageHandler ()                                                                           *                           
* Description : handles a message PersistenceObjectManagerCompareDBMessage. The function sends a message to all the  *
*               nodes in a cluster to get the checksum for their individual databases, compares these checksums that *
*               with Primary's db checksum, then forms vectors for matching, nonMatching and failed locations.       *
* @param 1    : PersistenceObjectManagerCompareDBMessage                                                             *
* @return     : None.                                                                                                *
**********************************************************************************************************************/

void PersistenceObjectManager::compareDBMessageHandler (PersistenceObjectManagerCompareDBMessage *pPersistenceObjectManagerCompareDBMessage)
{

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerFailedStep),
    };

    PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext = new PersistenceExecuteTransactionContext (pPersistenceObjectManagerCompareDBMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]), "");

    // Before the database backup is taken on each node and calculate checksum for same, it is required to make sure that databases are not 
    // allowing any writes. this can be achieved by holding all the messages to Primary as all the writes to the db is started by Primary.
    pPersistenceExecuteTransactionContext->holdAll ();
    pPersistenceExecuteTransactionContext->start ();
}


void PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStep (PersistenceExecuteTransactionContext *pPersistenceExecuteTransactionContext)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStep: Entering ...");
    if (true == FrameworkToolKit::isStandAloneLocation ())
    {
        trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStep : It's a standalone location. No need to compare DB. exiting..");
        pPersistenceExecuteTransactionContext->executeNextStep (WAVE_MESSAGE_ERROR_NODE_NOT_IN_CLUSTER); 
        return;
    }

    DatabaseObjectManagerCalculateDbCksumMessage *pCalculateDbCksumMessage  = new DatabaseObjectManagerCalculateDbCksumMessage ();
    WaveSendToClusterContext                     *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback), pPersistenceExecuteTransactionContext);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pCalculateDbCksumMessage);

    trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStep: sending to cluster");
    
    sendToWaveCluster (pWaveSendToClusterContext); 
    
}

void PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                                  status                                     = WAVE_MESSAGE_SUCCESS;
    PersistenceExecuteTransactionContext        *pPersistenceExecuteTransactionContext     = reinterpret_cast<PersistenceExecuteTransactionContext *>(pWaveSendToClusterContext->getPCallerContext ());
    PersistenceObjectManagerCompareDBMessage    *pPersistenceObjectManagerCompareDBMessage = reinterpret_cast<PersistenceObjectManagerCompareDBMessage *>(pPersistenceExecuteTransactionContext->getPPrismMessage ());
    vector<LocationId>                          locationsForDBCompare                      = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();
    UI32                                        numberOfLocationsForDBCompare              = locationsForDBCompare.size ();       
    LocationId                                  locationIdForPrimary                       = FrameworkToolKit::getClusterPrimaryLocationId (); 
    vector<LocationId>                          locationsWithMatchingDBWithPrimary;
    vector<LocationId>                          locationsWithNonMatchingDBWithPrimary;        
    vector<LocationId>                          failedLocationsToGetDBChecksum;        
    vector<string>                              ipAndPortForLocationsWithMatchingDB;
    vector<string>                              ipAndPortForLocationsWithNonMatchingDB;
    vector<string>                              ipAndPortForFailedLocationsToGetDBChecksum;

    trace (TRACE_LEVEL_DEVEL, "PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : entering ....");
    
    status = pWaveSendToClusterContext->getCompletionStatus ();

    if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == status)
    {
        trace (TRACE_LEVEL_ERROR, "PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : Message to Database Service failed on all nodes, status : " + FrameworkToolKit::localize (status));
        trace (TRACE_LEVEL_DEVEL, "PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : exiting...");
        delete pWaveSendToClusterContext->getPPrismMessageForPhase1 ();  
        delete pWaveSendToClusterContext;
    
        pPersistenceExecuteTransactionContext->executeNextStep (status);
        return;
    }

    tracePrintf (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : Primary LocationId = %d", locationIdForPrimary);

    DatabaseObjectManagerCalculateDbCksumMessage *pCalculateDbCksumMessage = reinterpret_cast<DatabaseObjectManagerCalculateDbCksumMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1 (locationIdForPrimary));
    prismAssert (NULL != pCalculateDbCksumMessage, __FILE__, __LINE__);

    string checksumForPrimary = pCalculateDbCksumMessage->getDbCksumForNode ();
    trace (TRACE_LEVEL_DEBUG, string("PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : Primary CRC = ") + checksumForPrimary);
    locationsWithMatchingDBWithPrimary.push_back (locationIdForPrimary);        // To have Primary also in the list of matching locations.
    ipAndPortForLocationsWithMatchingDB.push_back (getIPAndPortForLocationId (locationIdForPrimary));


    for (unsigned int i = 0; i < numberOfLocationsForDBCompare; i++)
    {
        string      checksumForSecondary;
        LocationId  locationId  = 0;
     
        locationId = locationsForDBCompare[i];
        tracePrintf (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : Location Id : %d", locationId);
    
        if (locationIdForPrimary != locationId)
        {
            ResourceId                                   completionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1 (locationId);
            DatabaseObjectManagerCalculateDbCksumMessage *dbCksumMessage  = reinterpret_cast<DatabaseObjectManagerCalculateDbCksumMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1 (locationId));
            prismAssert (NULL != dbCksumMessage, __FILE__, __LINE__);

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
               tracePrintf (TRACE_LEVEL_WARN, "PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : Location Id : %d is disconnected or has returned an error", locationId);
                failedLocationsToGetDBChecksum.push_back (locationId);
                ipAndPortForFailedLocationsToGetDBChecksum.push_back (getIPAndPortForLocationId (locationId));
                continue;
            }

            checksumForSecondary =  dbCksumMessage->getDbCksumForNode ();
            trace (TRACE_LEVEL_DEBUG, string ("PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : checksumForSecondary : ") + checksumForSecondary);

        
            if (0 == checksumForPrimary.compare(checksumForSecondary))
            {
                locationsWithMatchingDBWithPrimary.push_back (locationId);
                ipAndPortForLocationsWithMatchingDB.push_back (getIPAndPortForLocationId (locationId));
                trace (TRACE_LEVEL_DEBUG, string ("PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : Location Id : ") + locationId + string ("checksumForSecondary=") + checksumForSecondary + string ("matching DB checksumForSecondary with Primary"));
            }
            else
            {
                locationsWithNonMatchingDBWithPrimary.push_back (locationId);
                ipAndPortForLocationsWithNonMatchingDB.push_back (getIPAndPortForLocationId (locationId));
                trace (TRACE_LEVEL_DEBUG, string("PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : Location Id : ") + locationId + string("checksumForSecondary=") + checksumForSecondary + string ("doesn't match DB checksumForSecondary with Primary"));
            }
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, string("PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : This is a Primary. Skipping DB compare."));
        }
    }

               
    if (0 < locationsWithNonMatchingDBWithPrimary.size ())
    {
        tracePrintf (TRACE_LEVEL_ERROR, false, false, "PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : Following locations have different DB than a Primary");
        UI32 numLocations = locationsWithNonMatchingDBWithPrimary.size ();
        for (unsigned int i = 0; i < numLocations; i++)
        {
            tracePrintf (TRACE_LEVEL_ERROR, false, false, "%d ", locationsWithNonMatchingDBWithPrimary[i]);
        }
        
       pPersistenceObjectManagerCompareDBMessage->setNonMatchingDbLocations (locationsWithNonMatchingDBWithPrimary); 
       pPersistenceObjectManagerCompareDBMessage->setIpAndPortsForNonMatchingDbLocations (ipAndPortForLocationsWithNonMatchingDB);
    }
    
    if (0 < failedLocationsToGetDBChecksum.size ())
    {
        UI32 numLocations = failedLocationsToGetDBChecksum.size ();
        trace (TRACE_LEVEL_WARN, "PersistenceObjectManager::sendGetCksumMessageRequestToAllLocationsStepCallback : Could not collect the database checksum from the following locations.");
        for (unsigned int i = 0; i < numLocations; i++)
        {
            tracePrintf (TRACE_LEVEL_ERROR, false, false, "%d ", failedLocationsToGetDBChecksum[i]);
        }
        pPersistenceObjectManagerCompareDBMessage->setFailedLocations (failedLocationsToGetDBChecksum);
        pPersistenceObjectManagerCompareDBMessage->setIpAndPortsForFailedLocations (ipAndPortForFailedLocationsToGetDBChecksum);
    }

    // Set list of Matching locations irrespective of number of nonMatching/Failed locations as this list is 
    // bound to have atleast a Primary location Id.
    pPersistenceObjectManagerCompareDBMessage->setMatchingDbLocations (locationsWithMatchingDBWithPrimary);
    pPersistenceObjectManagerCompareDBMessage->setIpAndPortsForMatchingDbLocations (ipAndPortForLocationsWithMatchingDB);
        
    delete pWaveSendToClusterContext->getPPrismMessageForPhase1 ();  
    delete pWaveSendToClusterContext;
    
    pPersistenceExecuteTransactionContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

inline string PersistenceObjectManager::getIPAndPortForLocationId (LocationId location)
{
    return ((location == FrameworkToolKit::getClusterPrimaryLocationId ()) ? 
                string (FrameworkToolKit::getIpAddressForLocationId (location) + "#" + FrameworkToolKit::getPortForLocationId (location) + "*") : 
                string (FrameworkToolKit::getIpAddressForLocationId (location) + "#" + FrameworkToolKit::getPortForLocationId (location)));    
}


void PersistenceObjectManager::restoreDataBase ()
{
    trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::restoreDataBase Entering....");

    string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName2 ());

    DatabaseObjectManagerRestoreMessage message (backupFileName);

    ResourceId                          status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {   
        trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::restoreDataBase : Could not send message to restore database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {   
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_ERROR_DB_RESTORE_FAILED == status)
        {
            trace (TRACE_LEVEL_WARN, "PersistenceObjectManager::restoreDataBase : Not Able to Restore DB from Backed Up File. Completion Status : " + FrameworkToolKit::localize (status));
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::restoreDataBase : SUCCESSFULLY rollback to the last known valid previous DB configuration.");
        }
    }
}

void PersistenceObjectManager::printOrmMessageHandler (PersistenceObjectManagerPrintOrmMessage *pPersistenceObjectManagerPrintOrmMessage)
{
    vector<string>  argv        = pPersistenceObjectManagerPrintOrmMessage->getOrmArgs ();
    UI32            argc        = argv.size ();
    string          ormOutput   = "";

    OrmRepository::printOrm (argc, argv, ormOutput);

    pPersistenceObjectManagerPrintOrmMessage->setOrmInfo (ormOutput);

    pPersistenceObjectManagerPrintOrmMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pPersistenceObjectManagerPrintOrmMessage);
}


// xPathStringsVectorForTimestampUpdate has input xpaths
// athStringToOriginalDatetimestampMap is populated by this function to store original datetimestamps. These are required for rollback of cached Managed Objects.
string PersistenceObjectManager::getSqlToUpdateXPathStringObjects (const vector<string> &xPathStringsVectorForTimestampUpdate, map<string, DateTime> &xPathStringToOriginalDatetimestampMap)
{
    string sqlString;

    UI32 numberOfXPathStringsReceived = xPathStringsVectorForTimestampUpdate.size ();

    if (0 == numberOfXPathStringsReceived)
    {
        return sqlString;
    }

    set<string> setOfAllXPathStrings;

    for (UI32 i = 0; i < numberOfXPathStringsReceived; i++)
    {
        string anXPathString = xPathStringsVectorForTimestampUpdate[i];

        UI32 lengthOfXPathString = anXPathString.size ();

        if (0 == lengthOfXPathString)
        {
            continue;
        }
        else if (1 == lengthOfXPathString)
        {
            if ("/" == anXPathString)
            {
                setOfAllXPathStrings.insert (anXPathString);
            }

            continue;
        }

        size_t startPosition = 1;

        // trace (TRACE_LEVEL_INFO, string ("Input path string -- ") + aPathString);

        while (1)
        {
            size_t slashFoundAt = anXPathString.find ('/', startPosition);

            if (string::npos == slashFoundAt)
            {
                break;
            }

            string aParentString = anXPathString.substr (0, slashFoundAt);

            setOfAllXPathStrings.insert (aParentString);

            startPosition = slashFoundAt + 1;

            if (lengthOfXPathString <= startPosition)
            {
                break;
            }

            // trace (TRACE_LEVEL_INFO, string ("A parent          : ") + aParentString);
        }

        setOfAllXPathStrings.insert (anXPathString);

        // trace (TRACE_LEVEL_INFO, string ("And original path : ") + aPathString);
    }

    DateTime updateTimeStamp;

    for (set<string>::iterator itr = setOfAllXPathStrings.begin (); itr != setOfAllXPathStrings.end (); itr++)
    {
        XPathStringManagedObject *pXPathStringManagedObject = getXPathStringManagedObjectFromCache (*itr);

        if (NULL != pXPathStringManagedObject)
        {
            DateTime originalDatetimestamp = pXPathStringManagedObject->getLastModifiedTimeStamp ();
            pXPathStringManagedObject->setLastModifiedTimeStamp (updateTimeStamp);
            pXPathStringManagedObject->getSqlForUpdate (sqlString, OrmRepository::getWaveCurrentSchema ());
            xPathStringToOriginalDatetimestampMap[*itr] = originalDatetimestamp;
        }
    }

    return sqlString;
}

void PersistenceObjectManager::addXPathStringsMessageHandler (PersistenceObjectManagerAddXPathStringsMessage *pPersistenceObjectManagerAddXPathStringsMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::createXPathStringManagedObjectsStep),

        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPersistenceObjectManagerAddXPathStringsMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void PersistenceObjectManager::createXPathStringManagedObjectsStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    PersistenceObjectManagerAddXPathStringsMessage *pPersistenceObjectManagerAddXPathStringsMessage = dynamic_cast<PersistenceObjectManagerAddXPathStringsMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    prismAssert (NULL != pPersistenceObjectManagerAddXPathStringsMessage, __FILE__, __LINE__);

    vector<string>  xPathStrings                = pPersistenceObjectManagerAddXPathStringsMessage->getXPathStrings ();
    UI32            numberOfInputXPathStrings   = xPathStrings.size ();

    if (0 == numberOfInputXPathStrings)
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    set<string>     setOfInputXPathStrings (xPathStrings.begin (), xPathStrings.end ());

    WaveManagedObjectSynchronousQueryContext waveManagedObjectSynchronousQueryContext (XPathStringManagedObject::getClassName ());

    string attributeNameString = "xPathString";

    for (set<string>::iterator itr = setOfInputXPathStrings.begin (); itr != setOfInputXPathStrings.end (); itr++)
    {
        waveManagedObjectSynchronousQueryContext.addOrAttribute (new AttributeString ((*itr), attributeNameString));
    }

    vector<WaveManagedObject *> *pWaveManagedObjectQueryResultVector = querySynchronously (&waveManagedObjectSynchronousQueryContext);

    if (NULL == pWaveManagedObjectQueryResultVector)
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    UI32 numberOfExistingMatchingXPathManagedObjects = pWaveManagedObjectQueryResultVector->size ();

    set<string> setOfExistingXPathStrings;

    for (UI32 i = 0; i < numberOfExistingMatchingXPathManagedObjects; i++)
    {
        XPathStringManagedObject *pXPathStringManagedObject = dynamic_cast<XPathStringManagedObject *> ((*pWaveManagedObjectQueryResultVector)[i]);
        prismAssert (NULL != pXPathStringManagedObject, __FILE__, __LINE__);

        setOfExistingXPathStrings.insert (pXPathStringManagedObject->getXPathString ());
    }
    
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pWaveManagedObjectQueryResultVector);

    vector<string> vectorOfNewXPathStrings (setOfInputXPathStrings.size ());

    vector<string>::iterator intersectionEnd = set_difference (setOfInputXPathStrings.begin (), setOfInputXPathStrings.end (), setOfExistingXPathStrings.begin (), setOfExistingXPathStrings.end (), vectorOfNewXPathStrings.begin ());

    vectorOfNewXPathStrings.erase (intersectionEnd, vectorOfNewXPathStrings.end ());

    UI32 numberOfNewXPathStringManagedObjectsToBeCreated = vectorOfNewXPathStrings.size ();

    string sqlString;

    for (UI32 i = 0; i < numberOfNewXPathStringManagedObjectsToBeCreated; i++)
    {
        XPathStringManagedObject xPathStringManagedObject (this);
        xPathStringManagedObject.setXPathString (vectorOfNewXPathStrings[i]);
        xPathStringManagedObject.getSqlForInsert (sqlString, OrmRepository::getWaveCurrentSchema ());
    }

    if (true == sqlString.empty ())
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    PersistenceObjectManagerExecuteTransactionMessage *pPersistenceObjectManagerExecuteTransactionMessage = new PersistenceObjectManagerExecuteTransactionMessage (sqlString);
    ResourceId sendStatus = send (pPersistenceObjectManagerExecuteTransactionMessage, reinterpret_cast<PrismMessageResponseHandler> (&PersistenceObjectManager::xPathStringManagedObjectsCommitCallback), (void *) pPrismLinearSequencerContext);

    if (WAVE_MESSAGE_SUCCESS != sendStatus)
    {
        delete pPersistenceObjectManagerExecuteTransactionMessage;
        pPrismLinearSequencerContext->executeNextStep (sendStatus);
        return;
    }
}

// This callback is common for the following async operations -
// 1. Addition of new XPath Managed Objects.
// 2. deletion of some existing XPath Managed Objects.
// 3. reseting timestamp for all XPath Managed Objects.
// When this callback gets called after a successful commit operation, (i.e. XPath Manged Objects were modified successfully), then -
// in-memory cache of XPath Managed Objects is flushed and is re-populated.
// Later, if any of the above operations are happening frequently, incremental cache updation may be implemented.
void PersistenceObjectManager::xPathStringManagedObjectsCommitCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    ResourceId nextStepStatus = WAVE_MESSAGE_SUCCESS;

    // Irrespective of framework status, delete pPrismMessage if not null.
    if (NULL != pPrismMessage)
    {
        PersistenceObjectManagerExecuteTransactionMessage *pPersistenceObjectManagerExecuteTransactionMessage = dynamic_cast<PersistenceObjectManagerExecuteTransactionMessage *> (pPrismMessage);
        prismAssert (NULL != pPersistenceObjectManagerExecuteTransactionMessage, __FILE__,__LINE__);

        if (FRAMEWORK_SUCCESS == frameworkStatus)
        {
            nextStepStatus = pPersistenceObjectManagerExecuteTransactionMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == nextStepStatus)
            {
                flushAndRePopulateXPathStringToXPathStringManagedObjectMapAsCache ();
            }
        }
        else
        {
            nextStepStatus = frameworkStatus;
        }

        delete pPersistenceObjectManagerExecuteTransactionMessage;
    }
    else
    {
        nextStepStatus = frameworkStatus;
    }

    PrismLinearSequencerContext* pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pContext);
    prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);

    pPrismLinearSequencerContext->executeNextStep (nextStepStatus);
}

void PersistenceObjectManager::deleteXPathStringsMessageHandler (PersistenceObjectManagerDeleteXPathStringsMessage *pPersistenceObjectManagerDeleteXPathStringsMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::deleteXPathStringManagedObjectsStep),

        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPersistenceObjectManagerDeleteXPathStringsMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void PersistenceObjectManager::deleteXPathStringManagedObjectsStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    PersistenceObjectManagerDeleteXPathStringsMessage *pPersistenceObjectManagerDeleteXPathStringsMessage = dynamic_cast<PersistenceObjectManagerDeleteXPathStringsMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    prismAssert (NULL != pPersistenceObjectManagerDeleteXPathStringsMessage, __FILE__, __LINE__);

    vector<string>  xPathStrings                = pPersistenceObjectManagerDeleteXPathStringsMessage->getXPathStrings ();
    UI32            numberOfInputXPathStrings   = xPathStrings.size ();

    if (0 == numberOfInputXPathStrings)
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    WaveManagedObjectSynchronousQueryContext waveManagedObjectSynchronousQueryContext (XPathStringManagedObject::getClassName ());

    string attributeNameString = "xPathString";

    for (UI32 i = 0; i < numberOfInputXPathStrings; i++)
    {
        waveManagedObjectSynchronousQueryContext.addOrAttribute (new AttributeString (xPathStrings[i], attributeNameString));
    }

    vector<WaveManagedObject *> *pWaveManagedObjectQueryResultVector = querySynchronously (&waveManagedObjectSynchronousQueryContext);

    if (NULL == pWaveManagedObjectQueryResultVector)
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    string sqlString;

    UI32 numberOfExistingMatchingXPathManagedObjects = pWaveManagedObjectQueryResultVector->size ();

    for (UI32 i = 0; i < numberOfExistingMatchingXPathManagedObjects; i++)
    {
        XPathStringManagedObject *pXPathStringManagedObject = dynamic_cast<XPathStringManagedObject *> ((*pWaveManagedObjectQueryResultVector)[i]);
        prismAssert (NULL != pXPathStringManagedObject, __FILE__, __LINE__);

        (OrmRepository::getInstance ())->getSqlForDelete (pXPathStringManagedObject->getObjectId (), sqlString, OrmRepository::getWaveCurrentSchema ());
    }
    
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pWaveManagedObjectQueryResultVector);

    if (true == sqlString.empty ())
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    PersistenceObjectManagerExecuteTransactionMessage *pPersistenceObjectManagerExecuteTransactionMessage = new PersistenceObjectManagerExecuteTransactionMessage (sqlString);
    ResourceId sendStatus = send (pPersistenceObjectManagerExecuteTransactionMessage, reinterpret_cast<PrismMessageResponseHandler> (&PersistenceObjectManager::xPathStringManagedObjectsCommitCallback), (void *) pPrismLinearSequencerContext);

    if (WAVE_MESSAGE_SUCCESS != sendStatus)
    {
        delete pPersistenceObjectManagerExecuteTransactionMessage;
        pPrismLinearSequencerContext->executeNextStep (sendStatus);
        return;
    }
}

// Cache is used. So, any cache flush/repopulate issues will get reflected in getLastUpdateTimestampsForXPathStrings output.
void PersistenceObjectManager::getLastUpdateTimestampsForXPathStringsMessageHandler (PersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage *pPersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::getLastUpdateTimestampsForXPathStringsStep),

        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void PersistenceObjectManager::getLastUpdateTimestampsForXPathStringsStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    PersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage *pPersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage = dynamic_cast<PersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    prismAssert (NULL != pPersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage, __FILE__, __LINE__);

    vector<string>  xPathStrings                = pPersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage->getXPathStrings ();
    UI32            numberOfInputXPathStrings   = xPathStrings.size ();
    vector<UI64>    lastUpdatedTimestamps;

    if (0 == numberOfInputXPathStrings)
    {
        if (0 == m_XPathStringToXPathStringManagedObjectMapAsCache.size ())
        {
            pPrismLinearSequencerContext->executeNextStep (WAVE_PERSISTENCE_XPATHS_REGISTRATION_TABLE_IS_EMPTY);
            return;
        }

        map<string, XPathStringManagedObject *>::iterator itr   = m_XPathStringToXPathStringManagedObjectMapAsCache.begin ();
        map<string, XPathStringManagedObject *>::iterator end   = m_XPathStringToXPathStringManagedObjectMapAsCache.end ();

        for (; itr != end; itr++)
        {
            XPathStringManagedObject *pXPathStringManagedObject = itr->second;
            prismAssert (NULL != pXPathStringManagedObject, __FILE__, __LINE__);

            xPathStrings.push_back (pXPathStringManagedObject->getXPathString ());
            lastUpdatedTimestamps.push_back ((pXPathStringManagedObject->getLastModifiedTimeStamp ()).getUI64Representation ());
        }

        pPersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage->setXPathStrings (xPathStrings);
        pPersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage->setLastUpdatedTimestamps (lastUpdatedTimestamps);
    }
    else
    {
        // preserve order.
        for (UI32 i = 0; i < numberOfInputXPathStrings; i++)
        {
            XPathStringManagedObject *pXPathStringManagedObject = getXPathStringManagedObjectFromCache (xPathStrings[i]);

            if (NULL != pXPathStringManagedObject)
            {
                lastUpdatedTimestamps.push_back ((pXPathStringManagedObject->getLastModifiedTimeStamp ()).getUI64Representation ());
            }
            else
            {
                lastUpdatedTimestamps.clear ();
                pPrismLinearSequencerContext->executeNextStep (WAVE_PERSISTENCE_UNKNOWN_XPATHS);
                return;
            }
        }

        pPersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage->setLastUpdatedTimestamps (lastUpdatedTimestamps);
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PersistenceObjectManager::resetXPathStringsTimestampsMessageHandler (PersistenceObjectManagerResetXPathStringsTimestampsMessage *pPersistenceObjectManagerResetXPathStringsTimestampsMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::resetXPathStringsTimestampsStep),

        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPersistenceObjectManagerResetXPathStringsTimestampsMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void PersistenceObjectManager::resetXPathStringsTimestampsStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    vector<WaveManagedObject *> *pWaveManagedObjectQueryResultVector = querySynchronously (XPathStringManagedObject::getClassName ());

    if (NULL == pWaveManagedObjectQueryResultVector)
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection (*pWaveManagedObjectQueryResultVector);

    UI32 numberOfXPathManagedObjects = pWaveManagedObjectQueryResultVector->size ();

    string sqlString;
    DateTime currentDateTime;

    for (UI32 i = 0; i < numberOfXPathManagedObjects; i++)
    {
        XPathStringManagedObject *pXPathStringManagedObject = dynamic_cast<XPathStringManagedObject *> ((*pWaveManagedObjectQueryResultVector)[i]);
        prismAssert (NULL != pXPathStringManagedObject, __FILE__, __LINE__);

        WaveManagedObject *pWaveManagedObject = (*pWaveManagedObjectQueryResultVector)[i];
        pWaveManagedObject->setLastModifiedTimeStamp (currentDateTime);
        pWaveManagedObject->getSqlForUpdate (sqlString, OrmRepository::getWaveCurrentSchema ());
    }

    delete pWaveManagedObjectQueryResultVector;

    if (true == sqlString.empty ())
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    PersistenceObjectManagerExecuteTransactionMessage *pPersistenceObjectManagerExecuteTransactionMessage = new PersistenceObjectManagerExecuteTransactionMessage (sqlString);
    ResourceId sendStatus = send (pPersistenceObjectManagerExecuteTransactionMessage, reinterpret_cast<PrismMessageResponseHandler> (&PersistenceObjectManager::xPathStringManagedObjectsCommitCallback), (void *) pPrismLinearSequencerContext);

    if (WAVE_MESSAGE_SUCCESS != sendStatus)
    {
        delete pPersistenceObjectManagerExecuteTransactionMessage;
        pPrismLinearSequencerContext->executeNextStep (sendStatus);
        return;
    }
}

// This function deletes cached XPathStringManagedObject, and re-populates the cache with a new database query.
// This function is called when -
// 1. new XPaths are registered.
// 2. any XPaths are deleted.
// 3. when a "reset all timestamps" request is received. (This request is received after config-replay only. - in general.)
// Note : all the three functions operate on database, and whenever database operation is successful, then in the callback, this function is called  to keep cache in sync with database.

void PersistenceObjectManager::flushAndRePopulateXPathStringToXPathStringManagedObjectMapAsCache ()
{
    flushXPathStringToXPathStringManagedObjectMap ();

    vector<WaveManagedObject *> *pWaveManagedObjectQueryResultVector = querySynchronously (XPathStringManagedObject::getClassName ());

    if (NULL != pWaveManagedObjectQueryResultVector)
    {
        UI32 numberOfXPathManagedObjects = pWaveManagedObjectQueryResultVector->size ();

        for (UI32 i = 0; i < numberOfXPathManagedObjects; i++)
        {
            XPathStringManagedObject *pXPathStringManagedObject = dynamic_cast<XPathStringManagedObject *> ((*pWaveManagedObjectQueryResultVector)[i]);
            prismAssert (NULL != pXPathStringManagedObject, __FILE__, __LINE__);

            addXPathStringManagedObjectToCache (pXPathStringManagedObject);
        }

        delete pWaveManagedObjectQueryResultVector;

        tracePrintf (TRACE_LEVEL_INFO, "PersistenceObjectManager::flushAndRePopulateXPathStringToXPathStringManagedObjectMapAsCache : loaded [%u] XPathStringManagedObjects to in-memory cache.", numberOfXPathManagedObjects);
    }
}

// Helper function for flushAndRePopulateXPathStringToXPathStringManagedObjectMapAsCache.
// (Clears map and deletes objects.)
void PersistenceObjectManager::flushXPathStringToXPathStringManagedObjectMap ()
{
    map<string, XPathStringManagedObject *>::iterator itr   = m_XPathStringToXPathStringManagedObjectMapAsCache.begin ();
    map<string, XPathStringManagedObject *>::iterator end   = m_XPathStringToXPathStringManagedObjectMapAsCache.end ();

    vector<XPathStringManagedObject*> xPathStringManagedObjectVector;

    for (; itr != end; itr++)
    {
        XPathStringManagedObject *pXPathStringManagedObject = itr->second;
        prismAssert (NULL != pXPathStringManagedObject, __FILE__, __LINE__);

        xPathStringManagedObjectVector.push_back (pXPathStringManagedObject);
        // objects may be deleted here directly too. Though tis map is not mutex protected, only this thread accesses it.
    }

    m_XPathStringToXPathStringManagedObjectMapAsCache.clear ();

    UI32 numberOfXPathStringManagedObjectsToBeDeleted = xPathStringManagedObjectVector.size ();

    for (UI32 i = 0; i < numberOfXPathStringManagedObjectsToBeDeleted; i++)
    {
        delete xPathStringManagedObjectVector[i];
    }

    tracePrintf (TRACE_LEVEL_INFO, "PersistenceObjectManager::flushXPathStringToXPathStringManagedObjectMap : deleted [%u] XPathStringManagedObject from in-memory cache.", numberOfXPathStringManagedObjectsToBeDeleted);
}

// Helper function for flushAndRePopulateXPathStringToXPathStringManagedObjectMapAsCache.
// (Populates the map with queried objects while checking duplicates.)
void PersistenceObjectManager::addXPathStringManagedObjectToCache (XPathStringManagedObject *pXPathStringManagedObject)
{
    string xPathString = pXPathStringManagedObject->getXPathString ();

    map<string, XPathStringManagedObject *>::iterator element   = m_XPathStringToXPathStringManagedObjectMapAsCache.find (xPathString);
    map<string, XPathStringManagedObject *>::iterator end       = m_XPathStringToXPathStringManagedObjectMapAsCache.end ();

    if (end != element)
    {
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        m_XPathStringToXPathStringManagedObjectMapAsCache[xPathString] = pXPathStringManagedObject;
    }
}

void PersistenceObjectManager::flushAndRePopulateWaveConfigManagedObjectCache ()
{
    flushWaveConfigManagedObjectMap ();

    vector<WaveManagedObject *> *pWaveManagedObjectQueryResultVector = querySynchronously (WaveConfigManagedObject::getClassName ());

    if (NULL != pWaveManagedObjectQueryResultVector)
    {
        UI32 numberOfConfigs = pWaveManagedObjectQueryResultVector->size ();

        for (UI32 i = 0; i < numberOfConfigs ; i++)
        {
            WaveConfigManagedObject *pWaveConfigManagedObject = dynamic_cast<WaveConfigManagedObject *> ((*pWaveManagedObjectQueryResultVector)[i]);
            prismAssert (NULL != pWaveConfigManagedObject, __FILE__, __LINE__);

            addWaveConfigManagedObjectToCache (pWaveConfigManagedObject);
        }

        delete pWaveManagedObjectQueryResultVector;

    }
}

void PersistenceObjectManager::flushWaveConfigManagedObjectMap ()
{
    map<string, WaveConfigManagedObject *>::iterator itr   = m_configNameToWaveConfigManagedObjectMapAsCache.begin ();
    map<string, WaveConfigManagedObject *>::iterator end   = m_configNameToWaveConfigManagedObjectMapAsCache.end ();

    for (; itr != end; itr++)
    {
        WaveConfigManagedObject *pWaveConfigManagedObject = itr->second;
        prismAssert (NULL != pWaveConfigManagedObject, __FILE__, __LINE__);

        delete pWaveConfigManagedObject;
    }

    m_configNameToWaveConfigManagedObjectMapAsCache.clear ();
}

string PersistenceObjectManager::getWaveConfigSerializedData ()
{
    string serializedConfigs;
    map<string, WaveConfigManagedObject *>::iterator itr   = m_configNameToWaveConfigManagedObjectMapAsCache.begin ();
    map<string, WaveConfigManagedObject *>::iterator end   = m_configNameToWaveConfigManagedObjectMapAsCache.end ();

    for (; itr != end; itr++)
    {
        WaveConfigManagedObject *pWaveConfigManagedObject = itr->second;
        prismAssert (NULL != pWaveConfigManagedObject, __FILE__, __LINE__);

        string serializedConfig;
        pWaveConfigManagedObject->serialize2 (serializedConfig);

        serializedConfigs = serializedConfig + "\n";
    }

    trace (TRACE_LEVEL_INFO, "PersistenceObjectManager::getWaveConfigSerializedData: Total serialized WaveConfigManagedObject is : " + serializedConfigs);
    return serializedConfigs;
}

// Note : object obtained from this function should not be stored in async contextx and should not be accessed in async callbacks later.
// Whenevr required, this function should be called again.
XPathStringManagedObject *PersistenceObjectManager::getXPathStringManagedObjectFromCache (const string &xPathString)
{
    XPathStringManagedObject* pXPathStringManagedObject = NULL;

    map<string, XPathStringManagedObject *>::iterator element   = m_XPathStringToXPathStringManagedObjectMapAsCache.find (xPathString);
    map<string, XPathStringManagedObject *>::iterator end       = m_XPathStringToXPathStringManagedObjectMapAsCache.end ();

    if (end != element)
    {
        pXPathStringManagedObject = element->second;
        prismAssert (NULL != pXPathStringManagedObject, __FILE__, __LINE__);
    }

    return pXPathStringManagedObject;
}

// This is called when -
// 1. execute transaction fails in case of single node.
// 2. prepare fails in case of multi-node. (i.e. when rollback is done.)
void PersistenceObjectManager::restoreOriginalTimestampsForCachedXPathStringManagedObjects (const map<string, DateTime> &xPathStringToOriginalDatetimestampMap)
{
    map<string, DateTime>::const_iterator itr = xPathStringToOriginalDatetimestampMap.begin ();
    map<string, DateTime>::const_iterator end = xPathStringToOriginalDatetimestampMap.end ();

    for (; itr != end; itr++)
    {
        XPathStringManagedObject* pXPathStringManagedObject = getXPathStringManagedObjectFromCache (itr->first);

        if (NULL != pXPathStringManagedObject)
        {
            pXPathStringManagedObject->setLastModifiedTimeStamp (itr->second);
        }
        else
        {
            // before restore is called, another commit happened successfully which deleted XPathManagedObjects ? (Can happen on single node ?)
            // In multi-node, the locks will prevent ?
        }
    }
}

void PersistenceObjectManager::getAllClassNamesMessageHandler (PersistenceObjectManagerGetAllClassNamesMessage *pPersistenceObjectManagerGetAllClassNamesMessage)
{
    PersistenceObjectManager::getAllClassNamesMessageStaticHandler (pPersistenceObjectManagerGetAllClassNamesMessage);

    reply (pPersistenceObjectManagerGetAllClassNamesMessage);

    return;
}

void PersistenceObjectManager::getAllClassNamesMessageStaticHandler (PersistenceObjectManagerGetAllClassNamesMessage *pPersistenceObjectManagerGetAllClassNamesMessage)
{
    set<string> allClassNames;

    OrmRepository::getAllTableNames (allClassNames);

    pPersistenceObjectManagerGetAllClassNamesMessage->setAllClassNames (allClassNames);

    pPersistenceObjectManagerGetAllClassNamesMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
}

void PersistenceObjectManager::addDelayedTransactionMessageHandler (PersistenceObjectManagerAddDelayedTransactionMessage *pPersistenceObjectManagerAddDelayedTransactionMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::addDelayedTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPersistenceObjectManagerAddDelayedTransactionMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void PersistenceObjectManager::addDelayedTransactionStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    PersistenceObjectManagerAddDelayedTransactionMessage *pPersistenceObjectManagerAddDelayedTransactionMessage = dynamic_cast<PersistenceObjectManagerAddDelayedTransactionMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    prismAssert (NULL != pPersistenceObjectManagerAddDelayedTransactionMessage, __FILE__, __LINE__);

    string      sql                     = pPersistenceObjectManagerAddDelayedTransactionMessage->getSql ();
    bool        isConfigurationChange   = pPersistenceObjectManagerAddDelayedTransactionMessage->getIsConfigurationChange ();
    ResourceId  status                  = WAVE_MESSAGE_ERROR;

    if (true == isConfigurationChange)
    {
        trace (TRACE_LEVEL_FATAL, "PersistenceObjectManager::addDelayedTransactionStep : Currently we do not support delayed commit transactions for configuration changes.");

        prismAssert (false, __FILE__, __LINE__);
    }

    if (false == sql.empty ())
    {
        if (false == PersistenceLocalObjectManager::getDisableLocalCommitTransactions ())
        {
            // Only allow adding delayed commit transactions on the active MM.

            addDelayedTransaction (sql);
        }

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "PersistenceObjectManager::addDelayedTransactionStep : Empty SQL statement detected.");

        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

UI32 PersistenceObjectManager::isDelayedTransactionsEmpty ()
{
    return (m_delayedCommitTransactions.empty ());
}

void PersistenceObjectManager::addDelayedTransaction (const string &delayedSqlTransaction)
{
    m_delayedCommitTransactions += delayedSqlTransaction;
}

string PersistenceObjectManager::getDelayedTransactions ()
{
    return (m_delayedCommitTransactions);
}

void PersistenceObjectManager::clearDelayedTransactions ()
{
    m_delayedCommitTransactions.clear ();
}

void PersistenceObjectManager::setAnyConfigurationChangeTrackingNumber (UI64 &trackingNumber)
{
    m_anyConfiguraitonChangeTrackingNumberMutex.lock ();
    m_anyConfiguraitonChangeTrackingNumber = trackingNumber;
    m_anyConfiguraitonChangeTrackingNumberMutex.unlock ();
}

UI64 PersistenceObjectManager::getAnyConfigurationChangeTrackingNumber ()
{
    m_anyConfiguraitonChangeTrackingNumberMutex.lock ();
    
    UI64 trackingNumber = m_anyConfiguraitonChangeTrackingNumber;

    m_anyConfiguraitonChangeTrackingNumberMutex.unlock ();

    return (trackingNumber);
    
}

void PersistenceObjectManager::addWaveConfigManagedObjectToCache (WaveConfigManagedObject *pWaveConfigManagedObject)
{
    string configName = pWaveConfigManagedObject->getConfigName ();

    map<string, WaveConfigManagedObject *>::iterator element   = m_configNameToWaveConfigManagedObjectMapAsCache.find (configName);
    map<string, WaveConfigManagedObject *>::iterator end       = m_configNameToWaveConfigManagedObjectMapAsCache.end ();

    if (end != element)
    {
        trace (TRACE_LEVEL_FATAL, string ("PersistenceObjectManager::addWaveConfigManagedObjectToCache: config already exist in cache, configName = : ") + configName);
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        m_configNameToWaveConfigManagedObjectMapAsCache[configName] = pWaveConfigManagedObject;
    }
}

// Note : object obtained from this function should not be stored in async contextx and should not be accessed in async callbacks later.
// Whenevr required, this function should be called again.
WaveConfigManagedObject *PersistenceObjectManager::getWaveConfigManagedObjectFromCache (const string &configName)
{
    WaveConfigManagedObject* pWaveConfigManagedObject = NULL;

    map<string, WaveConfigManagedObject *>::iterator element   = m_configNameToWaveConfigManagedObjectMapAsCache.find (configName);
    map<string, WaveConfigManagedObject *>::iterator end       = m_configNameToWaveConfigManagedObjectMapAsCache.end ();

    if (end != element)
    {
        pWaveConfigManagedObject = element->second;
        prismAssert (NULL != pWaveConfigManagedObject, __FILE__, __LINE__);
    }

    return pWaveConfigManagedObject;
}
}
