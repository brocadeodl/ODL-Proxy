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
 ***************************************************************************/

#include "Framework/Core/PrismFrameworkObjectManagerInitializeWorker.h"
#include "Framework/Core/PrismFrameworkInitializeWorkerStartServicesContext.h"
#include "Framework/Boot/FirstTimePrismBootAgent.h"
#include "Framework/Boot/RecoverPrismBootAgent.h"
#include "Framework/Boot/PersistentPrismBootAgent.h"
#include "Framework/Boot/PersistentWithDefaultPrismBootAgent.h"
#include "Framework/Boot/SecondaryNodeConfigurePrismBootAgent.h"
#include "Framework/Boot/SecondaryNodeUnconfigurePrismBootAgent.h"
#include "Framework/Boot/SecondaryNodeRejoinPrismBootAgent.h"
#include "Framework/Boot/HaStandbyPrismBootAgent.h"
#include "Framework/Boot/PersistentWithDefaultForHABootAgent.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/PrismFrameworkConfiguration.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/PrismFrameworkConfigurationWorker.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/HaBootCompleteForThisLocationEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Persistence/PersistenceObjectManagerInitializeBeforeBootCompleteMessage.h"
#include "Framework/Database/DatabaseStandaloneTransaction.h"
#include "Framework/Database/DatabaseObjectManagerExecuteTransactionMessage.h"
#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaInfoRepository.h"
#include "Framework/Core/Wave.h"
#include "Framework/Persistence/PersistenceToolKit.h"
#include "Framework/Shutdown/PrismShutdownAgent.h"

namespace WaveNs
{

bool PrismFrameworkObjectManagerInitializeWorker::m_initialBootcompleted = false;
PrismMutex PrismFrameworkObjectManagerInitializeWorker::m_initialBootcompletedMutex;
bool PrismFrameworkObjectManagerInitializeWorker::m_initialSchemaInfoObjectSaved = false;
PrismMutex PrismFrameworkObjectManagerInitializeWorker::m_initialSchemaInfoObjectSavedMutex;

PrismFrameworkObjectManagerInitializeWorker::PrismFrameworkObjectManagerInitializeWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker            (pWaveObjectManager),
      m_pPrismBootAgent     (NULL)
{
    addEventType (BOOT_COMPLETE_FOR_THIS_LOCATION);
    addEventType (FRAMEWORK_OBJECT_MANAGER_BROADCAST_ADDITION_OF_NEW_NODES_EVENT);
    addEventType (FRAMEWORK_OBJECT_MANAGER_NODES_ADDITION_TO_CLUSTER_COMPLETED_EVENT);
    addEventType (FRAMEWORK_OBJECT_MANAGER_LOCAL_NODE_DELETED_EVENT);
    addEventType (FRAMEWORK_OBJECT_MANAGER_DELETE_CLUSTER_EVENT);
    addEventType (FRAMEWORK_OBJECT_MANAGER_BROADCAST_PHASE3_START_EVENT);
    addEventType (FRAMEWORK_OBJECT_MANAGER_BROADCAST_PHASE3_COMPLETE_EVENT);
    addEventType (FRAMEWORK_OBJECT_MANAGER_POST_BOOT_STARTED_EVENT);
    addEventType (FRAMEWORK_OBJECT_MANAGER_POST_BOOT_COMPLETED_EVENT);
    addEventType (FRAMEWORK_OBJECT_MANAGER_CONFIG_REPLAY_STARTED_EVENT);
    addEventType (FRAMEWORK_OBJECT_MANAGER_CONFIG_REPLAY_COMPLETED_EVENT);
    addEventType (FRAMEWORK_OBJECT_MANAGER_NODE_READY_FOR_ALL_COMMANDS_EVENT);
    addEventType (FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_EVENT);
    addEventType (FRAMEWORK_OBJECT_MANAGER_BROADCAST_FAILOVER_COMPLETE_EVENT);
    addEventType (HA_BOOT_COMPLETE_FOR_THIS_LOCATION);
    
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_UPDATE_DATABASE_SCHEMA, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerInitializeWorker::FrameworkUpdateDatabaseSchema));
}

PrismFrameworkObjectManagerInitializeWorker::~PrismFrameworkObjectManagerInitializeWorker ()
{
}

ResourceId PrismFrameworkObjectManagerInitializeWorker::startPrismServices (const PrismBootMode &prismBootMode, const WaveBootPhase &waveBootPhase)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManagerInitializeWorker::startPrismServices : Entering ...");

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkObjectManagerInitializeWorker::determineNodeBootModeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkObjectManagerInitializeWorker::chooseABootAgentStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkObjectManagerInitializeWorker::runTheBootAgentStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkObjectManagerInitializeWorker::saveConfigurationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkObjectManagerInitializeWorker::informServicesToInitializeBeforeBootComplete),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkObjectManagerInitializeWorker::declareBootCompleteStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkObjectManagerInitializeWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkObjectManagerInitializeWorker::prismSynchronousLinearSequencerFailedStep),
    };

    PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext = new PrismFrameworkInitializeWorkerStartServicesContext (reinterpret_cast<PrismAsynchronousContext *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismFrameworkInitializeWorkerStartServicesContext->setPrismBootMode (prismBootMode);
    pPrismFrameworkInitializeWorkerStartServicesContext->setWaveBootPhase (waveBootPhase);

    ResourceId status = pPrismFrameworkInitializeWorkerStartServicesContext->execute ();

    // Indicate Framework bringup is completed;

    PrismFrameworkObjectManager::releaseBootSynchronizationMutex ();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "Wave is now ready to serve.");
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerInitializeWorker::startPrismServices : Prism failed to come up.  Exiting ...");
//        prismAssert (false, __FILE__, __LINE__);
    }

    //(PrismFrameworkObjectManager::getInstance ())->setTraceLevel (TRACE_LEVEL_INFO);

    trace(TRACE_LEVEL_PERF_END, "Wave Bootup.");
    return (status);
}


//void PrismFrameworkObjectManagerInitializeWorker::buildDatabaseSchemaInfoRepository (const PrismFrameworkConfiguration &prismFrameworkConfiguration, ManagedObjectSchemaInfoRepository &dBSchemaRepository, vector<pair<string, UI32> >&tableClassIdTuples)
ResourceId PrismFrameworkObjectManagerInitializeWorker::buildDatabaseSchemaInfoRepository (const PrismFrameworkConfiguration &prismFrameworkConfiguration, ManagedObjectSchemaInfoRepository &dBSchemaRepository)
{
    ResourceId status = FRAMEWORK_SUCCESS; 

    vector<string> managedObjectNames;
    vector<string> managedObjectFieldNamesStrings;
    vector<string> managedObjectFieldTypesStrings;
    vector<UI32> classIds;
    vector<UI32> parentClassIds;
    vector<string> userDefinedKeyCombinationsFieldNames;
    vector<string> userDefinedKeyCombinationsFieldTypes;
    vector<string> managedObjectFieldExpandedTypesStrings;
    vector<string> fieldTypesStrings;
//    tableClassIdTuples.clear();

    const size_t numElems = prismFrameworkConfiguration.getSchemaChangeInfo (managedObjectNames, managedObjectFieldNamesStrings, managedObjectFieldTypesStrings, classIds, parentClassIds, userDefinedKeyCombinationsFieldNames,userDefinedKeyCombinationsFieldTypes, managedObjectFieldExpandedTypesStrings);

    if (numElems)
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::buildDatabaseSchemaInfoRepository: reading from configuration file.");
        if (!managedObjectFieldExpandedTypesStrings.size ()) 
        {
            fieldTypesStrings = managedObjectFieldTypesStrings;    
        }
        else
        {
            fieldTypesStrings = managedObjectFieldExpandedTypesStrings;    
        }

        for (size_t i = 0; i < numElems; ++i)
        {
            ManagedObjectSchemaInfo *pSchemaInfoObj = new CompositeManagedObjectSchemaInfo (managedObjectNames[i], classIds[i], parentClassIds[i]);
            vector<string> fields;
            vector<string> types;

            tokenize (managedObjectFieldNamesStrings[i], fields, ':');
            tokenize (fieldTypesStrings[i], types, ':');

            for (size_t j = 0; j < fields.size (); ++j)
            {
                pSchemaInfoObj->addFieldTypeTuple (fields[j], types[j]);
            }
        
            if (userDefinedKeyCombinationsFieldNames.size () > 0)
            {
                vector<string> userDefinedFieldNames;
                vector<string> userDefinedFieldTypes;

                tokenize (userDefinedKeyCombinationsFieldNames[i], userDefinedFieldNames, ':');
                tokenize (userDefinedKeyCombinationsFieldTypes[i], userDefinedFieldTypes, ':');
        
                for (size_t k = 0; k < userDefinedFieldNames.size (); ++k)
                {
                    pSchemaInfoObj->addUserDefinedFieldTypeTuple (userDefinedFieldNames[k], userDefinedFieldTypes[k]);
                }
            }

            dBSchemaRepository.addSchemaInfoObject (pSchemaInfoObj);
 //           tableClassIdTuples.push_back (pair<string, UI32>(managedObjectNames[i], classIds[i]));
        }

    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::buildDatabaseSchemaInfoRepository: reading from upgradeMO in db.");

        DatabaseSchema  databaseSchema;
        status = DatabaseSchema::getDatabaseSchema (databaseSchema);

        if (FRAMEWORK_SUCCESS != status)   
        {
            return (status);
        }

        databaseSchema.convertDatabaseSchemaToManagedObjectSchemaInfoRepository (dBSchemaRepository); 
    }

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::buildDatabaseSchemaInfoRepository: Populate DBSchemaInfo Repository with RelationshipPointers");
    dBSchemaRepository.populateSchemaInfoObjectsWithRelationshipPointers ();

    return (status);
}

ResourceId PrismFrameworkObjectManagerInitializeWorker::performSchemaConversion (const PrismFrameworkConfiguration &prismFrameworkConfiguration, bool ignoreConfigFile)
{
    ResourceId status = FRAMEWORK_SUCCESS;
    
    //vector<pair<string, UI32> > tableClassIdTuples;

    OrmRepository *pOrmRepository = OrmRepository::getInstance ();
    prismAssert(NULL != pOrmRepository, __FILE__, __LINE__);

    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManagerInitializeWorker::performSchemaConversion : Entering ...... ");

    ManagedObjectSchemaInfoRepository & moSchemaInfoRepository  = pOrmRepository->getMOSchemaInfoRepository ();

    ManagedObjectSchemaInfoRepository dBSchemaRepository;
    //buildDatabaseSchemaInfoRepository (prismFrameworkConfiguration, dBSchemaRepository, tableClassIdTuples);    
    
    if (ignoreConfigFile)
    {
        DatabaseSchema  databaseSchema;
        status = DatabaseSchema::getDatabaseSchema (databaseSchema);

        if (FRAMEWORK_SUCCESS == status)
        {
            databaseSchema.convertDatabaseSchemaToManagedObjectSchemaInfoRepository (dBSchemaRepository); 

            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::performSchemaConversion: Populate DBSchemaInfo Repository with RelationshipPointers");
            dBSchemaRepository.populateSchemaInfoObjectsWithRelationshipPointers ();
        }
    }
    else
    {   
        status = buildDatabaseSchemaInfoRepository (prismFrameworkConfiguration, dBSchemaRepository);
    }

    //pOrmRepository->updateTableId (tableClassIdTuples);
    pOrmRepository->buildMoSchemaRepository ();

    if (FRAMEWORK_SUCCESS == status) 
    {
        status = moSchemaInfoRepository.handleDatabaseSchemaDifferences (dBSchemaRepository);
    }

    return (status);
}

ResourceId PrismFrameworkObjectManagerInitializeWorker::determineNodeBootModeStep (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManagerInitializeWorker::determineNodeBootModeStep : Entering ...");

    if (WAVE_BOOT_UNKNOWN != (pPrismFrameworkInitializeWorkerStartServicesContext->getPrismBootMode ()))
    {
        return (WAVE_MESSAGE_SUCCESS);
    }

    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    PrismFrameworkConfigurationWorker *pPrismFrameworkConfigurationWorker = (PrismFrameworkObjectManager::getInstance ())->getPConfigurationWorker ();

    prismAssert (NULL != pPrismFrameworkConfigurationWorker, __FILE__, __LINE__);

    if (WAVE_MGMT_INTF_ROLE_SERVER == (FrameworkToolKit::getManagementInterfaceRole ()))
    {
        ifstream configBackupFile(((PrismFrameworkObjectManager::getInstance ())->getConfigurationBackupFileName ()).c_str());
        if (configBackupFile.is_open())
        {
            configBackupFile.close();
            string cpBackupFileCmd  = "/bin/mv " + (PrismFrameworkObjectManager::getInstance ())->getConfigurationBackupFileName () + " " + (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
            system (cpBackupFileCmd.c_str());
        }
    }

    ResourceId status = pPrismFrameworkConfigurationWorker->loadPrismConfiguration ((PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ());

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        pPrismFrameworkInitializeWorkerStartServicesContext->setPrismBootMode (WAVE_BOOT_FIRST_TIME);

        trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManagerInitializeWorker::determineNodeBootModeStep : The Prism Framework Configuration file does not exist.  We will treat this as a first time boot for Prism.");

        pOrmRepository->buildMoSchemaRepository ();
    }
    else
    {
        string                      prismConfigurationfileName   = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
        PrismFrameworkConfiguration prismFrameworkConfiguration;
        ResourceId                  status1                      = WAVE_MESSAGE_ERROR;

        status1 = prismFrameworkConfiguration.loadConfiguration (prismConfigurationfileName);

        prismAssert (WAVE_MESSAGE_SUCCESS == status1, __FILE__, __LINE__);

        ResourceId startupFileType = prismFrameworkConfiguration.getStartupFileType();

        if (LOCATION_STAND_ALONE == (prismFrameworkConfiguration.getThisLocationRole ()))
        {
            if (((startupFileType == WAVE_PERSISTENCE_DEFAULT_FILE) || (startupFileType == WAVE_PERSISTENCE_DEFAULT_FILE_AT_LOCAL_NODE) || (startupFileType == WAVE_PERSISTENCE_REGULAR_FILE)) && (prismFrameworkConfiguration.getIsStartupValid() == false))
            {
                pPrismFrameworkInitializeWorkerStartServicesContext->setPrismBootMode (WAVE_BOOT_PERSISTENT_WITH_DEFAULT);
            }
            else
            {
                pPrismFrameworkInitializeWorkerStartServicesContext->setPrismBootMode (WAVE_BOOT_PERSISTENT);
            }
        }
        else
        {
            pPrismFrameworkInitializeWorkerStartServicesContext->setPrismBootMode (WAVE_BOOT_PERSISTENT);
        }

        if (true ==  FrameworkToolKit::getDetectSchemaChange ())
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::determineNodeBootModeStep : detectSchemaChange flag is true, checking for schema changes..");
            
            DatabaseStandaloneTransaction* dbObject = new DatabaseStandaloneTransaction ();
            if (NULL == dbObject)
            {   
                trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerInitializeWorker::determineNodeBootModeStep : failed to allocate memory for DatabaseStandaloneTransaction");
                prismAssert (false, __FILE__, __LINE__);
            }

            dbObject->establishDbConnection ();

            // 1. check the location role of a local node.
            // 2. if not SA, Primary or Secondary, rollback db,
            // 3. rollback cfg.

            //1. 
            LocationRole    thisNodeLocationRole    = prismFrameworkConfiguration.getThisLocationRole ();
            ResourceId      rollbackStatus          = FRAMEWORK_SUCCESS;

            if ((thisNodeLocationRole != LOCATION_STAND_ALONE) && (thisNodeLocationRole != LOCATION_PRIMARY) && (thisNodeLocationRole != LOCATION_PRIMARY_UNCONFIRMED) && (thisNodeLocationRole != LOCATION_SECONDARY) && (thisNodeLocationRole != LOCATION_SECONDARY_UNCONFIRMED)) 
            {
                // delete cfg file 
                vector<string>  output;
                string          cmdString = string ("/bin/rm -rf ") + prismConfigurationfileName;
                SI32            cmdStatus = FrameworkToolKit::systemCommandOutput (cmdString.c_str(), output);

                if ( 0 != cmdStatus )
                {   
                    rollbackStatus = FRAMEWORK_ERROR;
                    trace (TRACE_LEVEL_ERROR, string("PrismFrameworkObjectManagerInitializeWorker::determineNodeBootModeStep: failed to delete file ")+ prismConfigurationfileName); 
                } 
                else
                {    
                    // 2. rollback the database.
                    rollbackStatus = dbObject->rollbackDatabase ();
                
                    //3. rollback cfg file
                    if (FRAMEWORK_ERROR != rollbackStatus)
                    {
                        rollbackStatus = dbObject->rollbackConfigFile (prismFrameworkConfiguration);   
                    }
                }
            }
            
            ResourceId databaseSchemaUpgradeStatus  = FRAMEWORK_ERROR;

            if (FRAMEWORK_SUCCESS == rollbackStatus)
            {
                databaseSchemaUpgradeStatus  = performSchemaConversion (prismFrameworkConfiguration);
            }
            else
            {
                pOrmRepository->buildMoSchemaRepository ();
            }
                

            if ((databaseSchemaUpgradeStatus != FRAMEWORK_SUCCESS) && (WAVE_SCHEMA_UPGRADE_NOT_REQUIRED != databaseSchemaUpgradeStatus))
            {
                pPrismFrameworkInitializeWorkerStartServicesContext->setPrismBootMode (WAVE_BOOT_FIRST_TIME);
                pPrismFrameworkInitializeWorkerStartServicesContext->setSchemaChangedOnFwdl (true);
                
                if (FRAMEWORK_STATUS_SCHEMA_UPGRADE_FOR_FW_DOWNGRADE_NOT_SUPPORTED == databaseSchemaUpgradeStatus)
                {
                    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::determineNodeBootModeStep : FWDL is a downgrade case and schema change is detected; hence, will go for a first time boot");
                    (PrismFrameworkObjectManager::getInstance ())->setDbConversionStatus (DATABASE_SCHEMA_CONVERSION_NOT_ATTEMPTED);
                }
                else
                {    
                    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::determineNodeBootModeStep : Schema change detected are not supported by db conversion, will treat as first time boot");
                    (PrismFrameworkObjectManager::getInstance ())->setDbConversionStatus (DATABASE_SCHEMA_CONVERSION_FAILED);
                }
            }
            else
            {
                // If we have successfully upgraded the database schema then record it in the context.
                // Then after running the boot agent - meaning after Framework data structures have been initialized with the prior .cfg file
                // It can be saved again with the proper values.
                // If we were to save at this point, Framework data structures which carry uninitialized data will be captured into the .cfg file.

                if (WAVE_SCHEMA_UPGRADE_NOT_REQUIRED != databaseSchemaUpgradeStatus)
                {
                    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::determineNodeBootModeStep : Schema Upgrade Successful");
                    pPrismFrameworkInitializeWorkerStartServicesContext->setIsSchemaUpgradedSuccessfully (true);
                    (PrismFrameworkObjectManager::getInstance ())->setDbConversionStatus (DATABASE_SCHEMA_CONVERSION_SUCCEEDED);
                }
                else 
                {
                    (PrismFrameworkObjectManager::getInstance ())->setDbConversionStatus (DATABASE_SCHEMA_CONVERSION_SUCCEEDED);
                    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::determineNodeBootModeStep : Schema Upgrade Not Required, treating it as Success ");
                }
            }

            if (WAVE_BOOT_PERSISTENT != pPrismFrameworkInitializeWorkerStartServicesContext->getPrismBootMode ())
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::determineNodeBootModeStep : shutting down the database");
                if (NULL != dbObject)
                {
                    dbObject->shutdownDatabase ();
                    delete dbObject;
                    dbObject = NULL;
                }
            }
        }
        else
        {
            // update the table-ids for Orm from the database.
            //pOrmRepository->buildMoSchemaRepository (1);
            pOrmRepository->buildMoSchemaRepository ();
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismFrameworkObjectManagerInitializeWorker::chooseABootAgentStep (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::chooseABootAgentStep : Entering ...");

    FrameworkSequenceGenerator &frameworkSequenceGenerator = PrismFrameworkObjectManager::getCurrentFrameworkSequenceGenerator ();
    PrismBootMode               prismBootMode              = pPrismFrameworkInitializeWorkerStartServicesContext->getPrismBootMode ();

    trace (TRACE_LEVEL_INFO, string ("Boot Mode is ") + prismBootMode);

    if (WAVE_BOOT_FIRST_TIME == prismBootMode)
    {
        m_pPrismBootAgent = new FirstTimePrismBootAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
    }
    else if (WAVE_BOOT_PERSISTENT == prismBootMode)
    {
        m_pPrismBootAgent = new PersistentPrismBootAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
    }
    else if (WAVE_BOOT_PERSISTENT_WITH_DEFAULT == prismBootMode)
    {
        m_pPrismBootAgent = new PersistentWithDefaultPrismBootAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
    }
    else if (WAVE_BOOT_SECONDARY_CONFIGURE == prismBootMode)
    {
        m_pPrismBootAgent = new SecondaryNodeConfigurePrismBootAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
    }
    else if (WAVE_BOOT_SECONDARY_UNCONFIGURE == prismBootMode)
    {
        m_pPrismBootAgent = new SecondaryNodeUnconfigurePrismBootAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
    }
    else if (WAVE_BOOT_SECONDARY_REJOIN == prismBootMode)
    {
        m_pPrismBootAgent = new SecondaryNodeRejoinPrismBootAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
    }
    else if (WAVE_BOOT_HASTANDBY == prismBootMode)
    {
        m_pPrismBootAgent = new HaStandbyPrismBootAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
    }
    else if (WAVE_BOOT_PREPARE_FOR_HA_BOOT == prismBootMode)
    {
        m_pPrismBootAgent = new PersistentWithDefaultForHABootAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("PrismFrameworkObjectManagerInitializeWorker::chooseABootAgentStep : Unknown Prism Boot Mode : ") + (UI32) prismBootMode);
        prismAssert (false, __FILE__, __LINE__);
    }

    prismAssert (NULL != m_pPrismBootAgent, __FILE__, __LINE__);

    if (NULL == m_pPrismBootAgent)
    {
        return (WAVE_MESSAGE_ERROR);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismFrameworkObjectManagerInitializeWorker::runTheBootAgentStep (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManagerInitializeWorker::runTheBootAgentStep : Entering ...");
    FrameworkSequenceGenerator &frameworkSequenceGenerator = PrismFrameworkObjectManager::getCurrentFrameworkSequenceGenerator ();

    //return (m_pPrismBootAgent->execute (pPrismFrameworkInitializeWorkerStartServicesContext->getWaveBootPhase ()));
    ResourceId status = m_pPrismBootAgent->execute (pPrismFrameworkInitializeWorkerStartServicesContext->getWaveBootPhase ());
    if (WAVE_MESSAGE_ERROR_DATABASE_INCONSISTENT == status)
    {
        PrismShutdownAgent *pShutdownAgent = new PrismShutdownAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
        status = pShutdownAgent->execute ();
        delete pShutdownAgent; 
        if (status != WAVE_MESSAGE_SUCCESS)
        {           
            trace(TRACE_LEVEL_FATAL,"PrismFrameworkObjectManagerInitializeWorker::runTheBootAgentStep: Shutting down all services failed\n");
            return status;
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerInitializeWorker::runTheBootAgentStep : Boot Mode is ") + WAVE_BOOT_FIRST_TIME);
            delete m_pPrismBootAgent;
            m_pPrismBootAgent = new RecoverPrismBootAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
            status = m_pPrismBootAgent->execute (WAVE_BOOT_PHASE_PRE_PHASE);
            if (status != WAVE_MESSAGE_SUCCESS)
            {           
                trace(TRACE_LEVEL_FATAL,"PrismFrameworkObjectManagerInitializeWorker::runTheBootAgentStep: Shutting down all services failed\n");
                return status;
            }
            else
            {
                delete m_pPrismBootAgent;
                m_pPrismBootAgent = new RecoverPrismBootAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
                return (m_pPrismBootAgent->execute (WAVE_BOOT_PHASE_POST_PHASE));
            }
        }
    }
    else
    {
        return status;
    }
}

ResourceId PrismFrameworkObjectManagerInitializeWorker::destroyABootAgentStep (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManagerInitializeWorker::destroyABootAgentStep : Entering ...");

    if (NULL != m_pPrismBootAgent)
    {
        delete m_pPrismBootAgent;
    }

    m_pPrismBootAgent =  NULL;

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismFrameworkObjectManagerInitializeWorker::saveConfigurationStep (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManagerInitializeWorker::saveConfigurationStep : Entering ...");

    if (WAVE_MGMT_INTF_ROLE_SERVER == (FrameworkToolKit::getManagementInterfaceRole ()))
    {
        trace (TRACE_LEVEL_DEBUG, "Saving Bootstrap configuration since we successfully upgraded DB Schema in this boot cycle.");
        
        // 1. take a cfg file backup.
        string backupCfgFileCmd = "/bin/cp " + (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName () + " " + (PrismFrameworkObjectManager::getInstance ())->getConfigurationBackupFileName ();
        vector<string>  output;
        SI32            cmdStatus;

        cmdStatus = FrameworkToolKit::systemCommandOutput (backupCfgFileCmd, output);

        if (0 != cmdStatus)
        {
            trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManagerInitializeWorker::saveConfigurationStep: failed to make configuration fiel backup.");
            system ((string ("/bin/rm -rf ") + (PrismFrameworkObjectManager::getInstance ())->getConfigurationBackupFileName ()).c_str());
        }

        // 2. save prism configuration
        FrameworkToolKit::savePrismConfiguration ();

        if (true == (DatabaseObjectManager::getIsDatabaseEnabled ()))
        {
            if (false == getInitialSchemaInfoObjectSaved())
            {
                tracePrintf (TRACE_LEVEL_INFO,true,false, "PrismFrameworkObjectManagerInitializeWorker::saveConfigurationStep: Saving SchemaInfoObject in database.");
                populateSchemaUpgradeTableInDb ();

                setInitialSchemaInfoObjectSaved (true);
            }
            else
            {
                tracePrintf (TRACE_LEVEL_INFO,true,false, "PrismFrameworkObjectManagerInitializeWorker::saveConfigurationStep: SchemaInfoObject is already saved.");
            }

            // 3. rename fwdl_dump_file to declare db conversion successful.
            //string databaseFwdlBackupFile   = FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + DatabaseObjectManager::getDatabaseDirectory() + "/" + FrameworkToolKit::getDatabaseBackupFileForFwdl();

            string databaseFwdlBackupFile   = FrameworkToolKit::getDatabaseBackupFileForFwdl();
            string dbConversionCompleteCmd  = "/bin/mv " + databaseFwdlBackupFile + " " + databaseFwdlBackupFile + ".done";

            system (dbConversionCompleteCmd.c_str());

            tracePrintf (TRACE_LEVEL_INFO,true,false, "PrismFrameworkObjectManagerInitializeWorker::saveConfigurationStep: executed command [%s]", dbConversionCompleteCmd.c_str());
        }
        
        // 4. remove the cfg backup file.
        string removeCfgBackupFile = "/bin/rm -rf " + (PrismFrameworkObjectManager::getInstance ())->getConfigurationBackupFileName ();    
    
        system (removeCfgBackupFile.c_str());

//        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::saveConfigurationStep: Beginning System Call SYNC");
//        sync ();
//        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::saveConfigurationStep: Ending System Call SYNC");
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismFrameworkObjectManagerInitializeWorker::informServicesToInitializeBeforeBootComplete (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManagerInitializeWorker::informServicesToInitializeBeforeBootComplete : Entering ...");

    if ((WAVE_BOOT_PERSISTENT == pPrismFrameworkInitializeWorkerStartServicesContext->getPrismBootMode ()) 
       || ((WAVE_BOOT_HASTANDBY == pPrismFrameworkInitializeWorkerStartServicesContext->getPrismBootMode ()) && (FrameworkToolKit::isPrimaryLocation () || (LOCATION_STAND_ALONE == FrameworkToolKit::getThisLocationRole ())))) 
    {
        PersistenceObjectManagerInitializeBeforeBootCompleteMessage pPersistenceObjectManagerInitializeBeforeBootCompleteMessage;

        ResourceId status = sendSynchronously (&pPersistenceObjectManagerInitializeBeforeBootCompleteMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerInitializeWorker::informServicesToInitializeBeforeBootComplete : Could not send message to Initialize Service before boot complete. Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            if (WAVE_MESSAGE_SUCCESS != pPersistenceObjectManagerInitializeBeforeBootCompleteMessage.getCompletionStatus ())
            {
                trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerInitializeWorker::informServicesToInitializeBeforeBootComplete FAILED");
                prismAssert (false, __FILE__, __LINE__);
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismFrameworkObjectManagerInitializeWorker::declareBootCompleteStep (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManagerInitializeWorker::declareBootCompleteStep : Entering ...");

    if ((WAVE_BOOT_PHASE_PRE_PHASE != (pPrismFrameworkInitializeWorkerStartServicesContext->getWaveBootPhase ())) && (WAVE_BOOT_PHASE_POST_PHASE != (pPrismFrameworkInitializeWorkerStartServicesContext->getWaveBootPhase ())) && (WAVE_BOOT_PREPARE_FOR_HA_FAILOVER_PHASE != (pPrismFrameworkInitializeWorkerStartServicesContext->getWaveBootPhase ())))
    {
        PersistenceToolKit::clearConfigurationManagedObject ();

        BootCompleteForThisLocationEvent *pBootCompleteForThisLocationEvent = new BootCompleteForThisLocationEvent (pPrismFrameworkInitializeWorkerStartServicesContext->isSchemaChangedOnFwdl ());

        broadcast (pBootCompleteForThisLocationEvent);
    }
    else if ((WAVE_BOOT_HASTANDBY == pPrismFrameworkInitializeWorkerStartServicesContext->getPrismBootMode ()) && (WAVE_BOOT_PHASE_PRE_PHASE != pPrismFrameworkInitializeWorkerStartServicesContext->getWaveBootPhase ()))
    {
        // Need to broadcast HaBootComplete on standby only when the DB is not EMPTY and atleast Framework services are UP.
        HaBootCompleteForThisLocationEvent *pHaBootCompleteForThisLocationEvent = new HaBootCompleteForThisLocationEvent ();

        broadcast (pHaBootCompleteForThisLocationEvent);
    }
    
    setInitialBootCompleted (true);

    m_postbootDebug.initialize(); // add the debug command for postboot in shell

    return (WAVE_MESSAGE_SUCCESS);
}

bool PrismFrameworkObjectManagerInitializeWorker::getInitialBootCompleted ()
{
    bool initialBootCompleted = false;

    m_initialBootcompletedMutex.lock ();

    initialBootCompleted = m_initialBootcompleted;

    m_initialBootcompletedMutex.unlock ();

    return (initialBootCompleted);
}

void PrismFrameworkObjectManagerInitializeWorker::setInitialBootCompleted (const bool &initialBootCompleted)
{
    m_initialBootcompletedMutex.lock ();

    m_initialBootcompleted = initialBootCompleted;

    m_initialBootcompletedMutex.unlock ();
}

void PrismFrameworkObjectManagerInitializeWorker::FrameworkUpdateDatabaseSchema (FrameworkObjectManagerUpdateDatabaseSchema *pFrameworkObjectManagerUpdateDatabaseSchema)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    string prismConfigurationfileName   = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
    PrismFrameworkConfiguration prismFrameworkConfiguration;

    status = prismFrameworkConfiguration.loadConfiguration (prismConfigurationfileName);

    prismAssert (WAVE_MESSAGE_SUCCESS == status, __FILE__, __LINE__);

    status = performSchemaConversion(prismFrameworkConfiguration);

    pFrameworkObjectManagerUpdateDatabaseSchema->setCompletionStatus((status == FRAMEWORK_SUCCESS) ? WAVE_MESSAGE_SUCCESS: WAVE_MESSAGE_ERROR);

    reply(pFrameworkObjectManagerUpdateDatabaseSchema);

}

/*                                                                                                                                                                               
 Function Name: populateSchemaUpgradeTableInDb                                                                                                                                    
 Achieves     :                                                                                                                                                                   
     // 1. Delete all the rows from the table for MoSchemaInfoManagedObject/DROP and CREATE a table.                                                                              
     // 2. Get sql for populating all rows in the table from OrmRepository.                                                                                                       
     // 3. apply this sql.                                                                                                                                                        
*/  
void PrismFrameworkObjectManagerInitializeWorker::populateSchemaUpgradeTableInDb ()
{
    string sqlToDropAndCreateSchemaUpgradeTable = string ("DELETE FROM " + OrmRepository::getWaveCurrentSchema () + "." + OrmRepository::getSchemaUpgradeTableName () + ";\n"
                                                        + "DELETE FROM " + OrmRepository::getWaveStartSchema () + "." + OrmRepository::getSchemaUpgradeTableName () + ";\n");
        
    string sqlToInsertUpgradeTableInCurrentSchema = "";
    OrmRepository *pOrmRepository  = OrmRepository::getInstance ();
    pOrmRepository->generateSqlToPopulateUpgradeTable (sqlToInsertUpgradeTableInCurrentSchema, OrmRepository::getWaveCurrentSchema ());
         
    string sqlToInsertUpgradeTableInStartSchema = "";                                                                                                             
    pOrmRepository->generateSqlToPopulateUpgradeTable (sqlToInsertUpgradeTableInStartSchema, OrmRepository::getWaveStartSchema ());
                                                                      
    string sqlToDropAndRecreateUpgradeTable = sqlToDropAndCreateSchemaUpgradeTable + sqlToInsertUpgradeTableInCurrentSchema + sqlToInsertUpgradeTableInStartSchema;
            
    trace (TRACE_LEVEL_DEBUG, "populateSchemaUpgradeTableInDb : sqlToDropAndRecreateUpgradeTable = " + sqlToDropAndRecreateUpgradeTable + "\n");

    DatabaseObjectManagerExecuteTransactionMessage message (sqlToDropAndRecreateUpgradeTable);
    ResourceId                                     status  = WAVE_MESSAGE_SUCCESS;
    
    status = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {   
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerInitializeWorker::populateSchemaUpgradeTableInDb: Could not send message to create schemaInfo Object to represent the Object Relational Mapping. Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {   
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {   
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerInitializeWorker::populateSchemaUpgradeTableInDb:: failed to updated schemaInfo object in db. Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {   
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerInitializeWorker::populateSchemaUpgradeTableInDb: updated schemaInfo object in db successfully.");
        }
    }

}

bool PrismFrameworkObjectManagerInitializeWorker::getInitialSchemaInfoObjectSaved ()
{
    m_initialSchemaInfoObjectSavedMutex.lock();

    bool schemaInfoSaved = m_initialSchemaInfoObjectSaved;
    m_initialSchemaInfoObjectSavedMutex.unlock ();

    return (schemaInfoSaved);
}

void PrismFrameworkObjectManagerInitializeWorker::setInitialSchemaInfoObjectSaved (bool isSchemaInfoObjectSaved)
{
    m_initialSchemaInfoObjectSavedMutex.lock ();
    m_initialSchemaInfoObjectSaved = isSchemaInfoObjectSaved;
    m_initialSchemaInfoObjectSavedMutex.unlock ();
}

}
