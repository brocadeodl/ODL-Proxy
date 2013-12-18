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

#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerSetStartupFileMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerSavePrismConfigurationMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerCopySchemaMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerCopyDefaultClusterMessage.h"
#include "Framework/Database/DatabaseObjectManagerExecuteCopySchemaMessage.h"
#include "Framework/Database/DatabaseObjectManagerGetLatestInstanceIdForTableMessage.h"
#include "Framework/Database/DatabaseObjectManagerExecuteTransactionMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerSetConfigurationValidMessage.h"    
#include "Framework/Persistence/PersistenceLocalObjectManagerDebugSchemaChangeMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerExecuteTransactionMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerEnableTransactionMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerLiveSyncEnableMessages.h"
#include "Framework/Persistence/PersistenceLocalExecuteTransactionContext.h"
#include "Framework/Persistence/PersistenceToolKit.h"
#include "Framework/Persistence/ConfigurationManagedObject.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Core/Wave.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/ObjectModel/WaveClientSessionContext.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"

namespace WaveNs
{

bool            PersistenceLocalObjectManager::m_liveSyncEnabled                        = false;
PrismMutex      PersistenceLocalObjectManager::m_liveSyncEnabledMutex;

bool            PersistenceLocalObjectManager::m_disableLocalCommitTransactions         = false;
PrismMutex      PersistenceLocalObjectManager::m_disableLocalCommitTransactionsMutex;

string          PersistenceLocalObjectManager::m_unsyncedPendingPrepareTransaction      = "";

UI64            PersistenceLocalObjectManager::m_anyConfiguraitonChangeTrackingNumber   = 0;
PrismMutex      PersistenceLocalObjectManager::m_anyConfiguraitonChangeTrackingNumberMutex;

UpdateGlobalConfigInKernelHandlerFunction PersistenceLocalObjectManager::m_updateGlobalConfigInKernelHandlerFunction = NULL;

PersistenceLocalObjectManager::PersistenceLocalObjectManager ()
    : WaveLocalObjectManager (getServiceName ())
{
    addOperationMap (PERSISTENCE_SET_STARTUP_FILE,   reinterpret_cast<PrismMessageHandler> (&PersistenceLocalObjectManager::setStartupFileMessageHandler));
    addOperationMap (PERSISTENCE_COPY_DEFAULT_CLUSTER,   reinterpret_cast<PrismMessageHandler> (&PersistenceLocalObjectManager::copyDefaultClusterMessageHandler));
    addOperationMap (PERSISTENCE_SAVE_PRISM_CONFIGURATION,   reinterpret_cast<PrismMessageHandler> (&PersistenceLocalObjectManager::savePrismConfigurationMessageHandler));
    addOperationMap (PERSISTENCE_COPY_SCHEMA, reinterpret_cast<PrismMessageHandler> (&PersistenceLocalObjectManager::copySchemaMessageHandler));
    addOperationMap (PERSISTENCE_SET_CONFIGURATION_VALID, reinterpret_cast<PrismMessageHandler> (&PersistenceLocalObjectManager::setConfigurationValidMessageHandler));
    addOperationMap (PERSISTENCE_DEBUG_SCHEMA_CHANGE, reinterpret_cast<PrismMessageHandler> (&PersistenceLocalObjectManager::debugSchemaChangeMessageHandler));
    addOperationMap (PERSISTENCE_LOCAL_OBJECT_MANAGER_UPDATE_INSTANCE_ID, reinterpret_cast<PrismMessageHandler> (&PersistenceLocalObjectManager::updateInstanceIdMessageHandler));
    addOperationMap (PERSISTENCE_LOCAL_OBJECT_MANAGER_EXECUTE_TRANSACTION, reinterpret_cast<PrismMessageHandler> (&PersistenceLocalObjectManager::executeTransactionMessageHandler));
    addOperationMap (PERSISTENCE_LOCAL_OBJECT_MANAGER_ENABLE_TRANSACTION, reinterpret_cast<PrismMessageHandler> (&PersistenceLocalObjectManager::enableTransactionMessageHandler));
    addOperationMap (PERSISTENCE_SET_LIVE_SYNC_ENABLE, reinterpret_cast<PrismMessageHandler> (&PersistenceLocalObjectManager::setLiveSyncEnableMessageHandler));
    addOperationMap (PERSISTENCE_GET_LIVE_SYNC_ENABLE, reinterpret_cast<PrismMessageHandler> (&PersistenceLocalObjectManager::getLiveSyncEnableMessageHandler));
    addOperationMap (PERSISTENCE_PUSH_CONFIG_TO_KERNEL, reinterpret_cast<PrismMessageHandler> (&PersistenceLocalObjectManager::pushConfigToKernelMessageHandler));
    addDebugFunction ((ShellCmdFunction) (&ManagedObjectSchemaInfoRepository::printMoRepositoryDebugShellWrapper), "printmorepository");
    addDebugFunction ((ShellCmdFunction) (&ManagedObjectSchemaInfoRepository::removeSchemaObjectDebugShellWrapper), "removeschemaobject");

    addEventType (STARTUP_SCHEMA_CHANGE_EVENT);
}

PersistenceLocalObjectManager *PersistenceLocalObjectManager::getInstance ()
{
    static PersistenceLocalObjectManager *pPersistenceLocalObjectManager = new PersistenceLocalObjectManager ();

    WaveNs::prismAssert (NULL != pPersistenceLocalObjectManager, __FILE__, __LINE__);

    return (pPersistenceLocalObjectManager);
}

PrismServiceId PersistenceLocalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string PersistenceLocalObjectManager::getServiceName ()
{
    return ("Persistence Local Object Manager");
}

PrismMessage *PersistenceLocalObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case PERSISTENCE_SET_STARTUP_FILE :
            pPrismMessage = new PersistenceLocalObjectManagerSetStartupFileMessage ();
            break;

        case PERSISTENCE_SAVE_PRISM_CONFIGURATION :
            pPrismMessage = new PersistenceLocalObjectManagerSavePrismConfigurationMessage ();
            break;

        case PERSISTENCE_COPY_SCHEMA :
            pPrismMessage = new PersistenceLocalObjectManagerCopySchemaMessage ();
            break;

        case PERSISTENCE_COPY_DEFAULT_CLUSTER :
            pPrismMessage = new PersistenceLocalObjectManagerCopyDefaultClusterMessage ();
            break;

        case PERSISTENCE_SET_CONFIGURATION_VALID :
            pPrismMessage = new PersistenceLocalObjectManagerSetConfigurationValidMessage ();
            break;

        case PERSISTENCE_DEBUG_SCHEMA_CHANGE :
            pPrismMessage = new PersistenceLocalObjectManagerDebugSchemaChangeMessage ();
            break;

        case PERSISTENCE_LOCAL_OBJECT_MANAGER_UPDATE_INSTANCE_ID :
            pPrismMessage = new PersistenceLocalObjectManagerUpdateInstanceIdMesssage ();
            break;

        case PERSISTENCE_LOCAL_OBJECT_MANAGER_EXECUTE_TRANSACTION :
            pPrismMessage = new PersistenceLocalObjectManagerExecuteTransactionMessage ();
            break;

        case PERSISTENCE_LOCAL_OBJECT_MANAGER_ENABLE_TRANSACTION :
            pPrismMessage = new PersistenceLocalObjectManagerEnableTransactionMessage ();
            break;

        case PERSISTENCE_SET_LIVE_SYNC_ENABLE :
            pPrismMessage = new PersistenceLocalObjectManagerSetLiveSyncEnableMessage ();
            break;

        case PERSISTENCE_GET_LIVE_SYNC_ENABLE :
            pPrismMessage = new PersistenceLocalObjectManagerGetLiveSyncEnableMessage ();
            break;

        case PERSISTENCE_PUSH_CONFIG_TO_KERNEL :
            pPrismMessage = new PersistenceLocalObjectManagerPushConfigToKernelMessaage ();
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

PrismEvent *PersistenceLocalObjectManager::createEventInstance (const UI32 &eventOperationCode)
{
    PrismEvent *pPrismEvent = NULL;

    switch (eventOperationCode)
    {
        case STARTUP_SCHEMA_CHANGE_EVENT:
            pPrismEvent = new StartupSchemaChangeEvent ();
            break;

        default:
            trace (TRACE_LEVEL_FATAL, string ("PersistenceLocalObjectManager::createEventInstance : Unkown Event Operation Code : ") + eventOperationCode);
            prismAssert (false, __FILE__, __LINE__);
    }

    return (pPrismEvent);
}

/*********************************************************************************
 * Name         : copyDefaultClusterMessageHandler                               *
 * Description  : It handles copy default to startup in a cluster. It removes    *
 *                the cluster config file and reboots all nodes to form a new    *
 *                cluster.                                                       *
 * @param1      : This is a dummy message for copy default in a cluster.         *
 *********************************************************************************/
  
void PersistenceLocalObjectManager::copyDefaultClusterMessageHandler( PersistenceLocalObjectManagerCopyDefaultClusterMessage *pMessage )
{
        ResourceId  status  = WAVE_MESSAGE_SUCCESS;

        string cmdToExecute;
        UI32  waveNativeClientId, waveUserClientId;

        trace (TRACE_LEVEL_INFO, string("PersistenceLocalObjectManager::copyDefaultClusterMessageHandler : entering.. "));
        
        //Note: defaultFile has been hardcoded. 
        string defaultFileName = FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + "defaultconfig.vcs";
        FrameworkToolKit::setStartupFileName( defaultFileName );

        status = FrameworkToolKit::changePrismConfigurationValidity ( false );
        if ( status != WAVE_MESSAGE_SUCCESS )
        {
            trace (TRACE_LEVEL_FATAL, string("PersistenceLocalObjectManager::copyDefaultClusterMessageHandler : Removing Cfg File failed")); 
        }
        trace (TRACE_LEVEL_INFO, string("PersistenceLocalObjectManager::copyDefaultClusterMessageHandler : System will reboot now.. "));
    //Note : All the nodes in the cluster will reboot here.
    
        waveNativeClientId = pMessage->getWaveNativeClientId(); 
        waveUserClientId = pMessage->getWaveUserClientId();
        const   LocationId thisLocationId = FrameworkToolKit::getThisLocationId() ; 
        WaveClientSessionContext thisWaveClientSessionContext ( thisLocationId, waveNativeClientId, waveUserClientId );

        string userMessage(" \n copy default-config to startup-config is being processed...\n All nodes will reboot and form the cluster newly.\n");

        printfToAllWaveClientSessions( thisWaveClientSessionContext, "%s", userMessage.c_str());

    Wave::logOperationStatus (COPY_DEFAULT_TO_STARTUP_SUCCESS);
    
    system ("/fabos/sbin/chassisReboot");

    pMessage->setCompletionStatus (status);

//    reply (pMessage); Explicitly not replying the message, as reboot is called above.
}

void PersistenceLocalObjectManager::setStartupFileMessageHandler (PersistenceLocalObjectManagerSetStartupFileMessage *pMessage)
{
    string      fileName;
    ResourceId  status  = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, string ("PersistenceLocalObjectManager::setStartupFileMessageHandler: setting the startup file type to: ") + pMessage->getStartupFileType ());

    if (WAVE_PERSISTENCE_DEFAULT_FILE == pMessage->getStartupFileType ())
    {
        string defaultFile;

        if (WAVE_PERSISTENCE_CLUSTER_ENABLED == Wave::persistencePostBootCheck ())
        {
            defaultFile = "defaultconfig.vcs";
        }
        else
        {
            defaultFile = "defaultconfig.novcs";
        }

        fileName = FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + defaultFile; 

        // copy default-config file to startup-config file (for display purposes)
        string cmd = string("cp -f " + fileName + " " + ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory () + "/startup-config");

        int ret = system (cmd.c_str ());
        trace (TRACE_LEVEL_INFO, string("PersistenceLocalObjectManager::setStartupFileMessageHandler: " + cmd + " ret = " + WEXITSTATUS(ret)));
    }
    else if (WAVE_PERSISTENCE_DEFAULT_FILE_AT_LOCAL_NODE == pMessage->getStartupFileType ())
    {
        fileName = FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" +  pMessage->getStartupFileName (); 
    }
    else
    {
        fileName = pMessage->getStartupFileName ();

    }

    trace (TRACE_LEVEL_INFO, string ("PersistenceLocalObjectManager::setStartupFileMessageHandler: setting the startup file name to: ") + fileName);

    FrameworkToolKit::setStartupFileType ( pMessage->getStartupFileType ());
    FrameworkToolKit::setIsStartupValid (false);
    FrameworkToolKit::setStartupFileName (fileName);

    // Setting of regular file is a local operation only. Hence, save the prism configuration
    if (WAVE_PERSISTENCE_REGULAR_FILE == pMessage->getStartupFileType ())
    {
        status = FrameworkToolKit::savePrismConfiguration (false);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, string ("PersistenceLocalObjectManager::setStartupFileMessageHandler : savePrismConfiguration status  : ") + FrameworkToolKit::localize (status));
        }
    }

    pMessage->setCompletionStatus (status);
    reply (pMessage);
}

void PersistenceLocalObjectManager::savePrismConfigurationMessageHandler (PersistenceLocalObjectManagerSavePrismConfigurationMessage *pMessage)
{
    ResourceId  status = WAVE_MESSAGE_ERROR;

    // Indicate whether to use this startup schema on next reboot
    FrameworkToolKit::setIsStartupValid (pMessage->getIsStartupValid ());

    status = FrameworkToolKit::savePrismConfiguration (false);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_WARN, string ("PersistenceLocalObjectManager::savePrismConfigurationMessageHandler : savePrismConfiguration status  : ") + FrameworkToolKit::localize (status));
    }

    pMessage->setCompletionStatus (status);
    reply (pMessage);
}

void PersistenceLocalObjectManager::copySchemaMessageHandler (PersistenceLocalObjectManagerCopySchemaMessage *pPersistenceLocalObjectManagerCopySchemaMessage)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceLocalObjectManager::copySchemaMessageHandler : Entering ...");

    ResourceId                                         status                                             = WAVE_MESSAGE_SUCCESS;

    DatabaseObjectManagerExecuteCopySchemaMessage *pDatabaseObjectManagerExecuteCopySchemaMessage = new DatabaseObjectManagerExecuteCopySchemaMessage (pPersistenceLocalObjectManagerCopySchemaMessage->getSourceSchema (), pPersistenceLocalObjectManagerCopySchemaMessage->getDestSchema ());

    status = sendSynchronously (pDatabaseObjectManagerExecuteCopySchemaMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_WARN, string ("PersistenceLocalObjectManager::copySchemaMessageHandler : Error sending message to DatabaseObjectManager  : ") + FrameworkToolKit::localize (status));
    }
    else
    {
        if (WAVE_MESSAGE_SUCCESS != pDatabaseObjectManagerExecuteCopySchemaMessage->getCompletionStatus ())
        {
            trace (TRACE_LEVEL_WARN, string ("PersistenceLocalObjectManager::copySchemaMessageHandler : Error processing DatabaseObjectManagerExecuteCopySchemaMessage  : ") + FrameworkToolKit::localize (status));
        }
        else
        {
            FrameworkToolKit::setIsStartupValid (true);
            status = FrameworkToolKit::savePrismConfiguration (false);
        }
    }
    delete pDatabaseObjectManagerExecuteCopySchemaMessage;
    pPersistenceLocalObjectManagerCopySchemaMessage->setCompletionStatus (status);
    reply (pPersistenceLocalObjectManagerCopySchemaMessage);
}

void PersistenceLocalObjectManager::setConfigurationValidMessageHandler( PersistenceLocalObjectManagerSetConfigurationValidMessage *pPersistenceLocalObjectManagerSetConfigurationValidMessage)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceLocalObjectManager::setConfigurationValidMessageHandler : Entering ...");

    ResourceId  status = WAVE_MESSAGE_SUCCESS;
    bool validity = pPersistenceLocalObjectManagerSetConfigurationValidMessage->getConfigurationValidity ();
    status = FrameworkToolKit::changePrismConfigurationValidity( validity);     
    if (WAVE_MESSAGE_SUCCESS != status)
    {   
        trace (TRACE_LEVEL_ERROR, string ("PersistenceLocalObjectManager::setConfigurationValidMessageHandler : Changing prismConfiguration validity failed with status : ") + FrameworkToolKit::localize (status));
    }
    
    pPersistenceLocalObjectManagerSetConfigurationValidMessage->setCompletionStatus (status);

    // define a copy to startup schema event and broadcast 

    string schemaFile = pPersistenceLocalObjectManagerSetConfigurationValidMessage->getSchemaFile ();
    trace (TRACE_LEVEL_DEBUG, string ("PersistenceLocalObjectManager::setConfigurationValidMessageHandler : schemaFile : ") + schemaFile);

    SchemaType originSchema;
    if (!schemaFile.empty ())
    {
        trace (TRACE_LEVEL_DEBUG, string ("PersistenceLocalObjectManager::setConfigurationValidMessageHandler : schemaFile is not empty"));
        originSchema = SCHEMA_FILE;
    }
    else
    {
        originSchema = ((validity == true) ? RUNNING_SCHEMA:DEFAULT_SCHEMA);
    }
    broadcastSchemaChangeEvent (originSchema, schemaFile);

    reply ( pPersistenceLocalObjectManagerSetConfigurationValidMessage);
}

void PersistenceLocalObjectManager::broadcastSchemaChangeEvent (SchemaType fromSchema, string fromSchemaPath, SchemaType toSchema, string toSchemaPath , bool operationStatus)
{
    // broadcast startup schema change event from Dcmd in case of MC mode and before nodes get rebooted.
    StartupSchemaChangeEvent *pEvent = new StartupSchemaChangeEvent ();
    prismAssert (NULL != pEvent, __FILE__, __LINE__);

    pEvent->setOperationStatus (operationStatus);
    pEvent->setFromSchema (fromSchema);
    pEvent->setFromFileSchemaPath (fromSchemaPath);
    pEvent->setToSchema (toSchema);
    pEvent->setToFileSchemaPath (toSchemaPath);

    broadcast (pEvent);
}
void PersistenceLocalObjectManager::debugSchemaChangeMessageHandler (PersistenceLocalObjectManagerDebugSchemaChangeMessage *pPersistenceLocalObjectManagerDebugSchemaChangeMessage)
{
    vector <string>  cmdArg  = pPersistenceLocalObjectManagerDebugSchemaChangeMessage->getCmdArgs ();
    string command           = cmdArg [0];

    OrmRepository *orm                                = OrmRepository::getInstance ();
    ManagedObjectSchemaInfoRepository &moRepository   = orm->getMOSchemaInfoRepository ();

    string result = "";
    if (0 == command.compare ("printMORepository"))
    {
        moRepository.printMORepositoryForDebugging (result);
    }
    else if (0 == command.compare("getAllInstanceIds"))
    {
        tracePrintf (TRACE_LEVEL_INFO, true, false, "PersistenceLocalObjectManager::debugSchemaChangeMessageHandler : getting instanceIds for table \"%s\"", cmdArg[1].c_str()); 
        moRepository.getAllInstanceIds (cmdArg[1], result);
    }

    pPersistenceLocalObjectManagerDebugSchemaChangeMessage->setResultPrint (result);
    pPersistenceLocalObjectManagerDebugSchemaChangeMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pPersistenceLocalObjectManagerDebugSchemaChangeMessage);
}

void PersistenceLocalObjectManager::updateInstanceIdMessageHandler (PersistenceLocalObjectManagerUpdateInstanceIdMesssage *pPersistenceLocalObjectManagerUpdateInstanceIdMesssage)
{
    trace (TRACE_LEVEL_INFO, "PersistenceLocalObjectManager::updateInstanceIdMessageHandler Entering");

    updateInstanceId ();

    pPersistenceLocalObjectManagerUpdateInstanceIdMesssage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pPersistenceLocalObjectManagerUpdateInstanceIdMesssage);
}

void PersistenceLocalObjectManager::updateInstanceId ()
{
    trace (TRACE_LEVEL_INFO, "PersistenceLocalObjectManager::updateInstanceId : Updating Latest Instance Ids for Most Base Orm Tables.");

    vector<string> mostBaseTableNames;
    UI32           numberOfMostBaseTables = 0;
    UI32           i                      = 0;
    ResourceId     status                 = WAVE_MESSAGE_SUCCESS;

    OrmRepository::getAllMostBaseTableNames (mostBaseTableNames);
    numberOfMostBaseTables = mostBaseTableNames.size ();

    for (i = 0; i < numberOfMostBaseTables; i++)
    {
        trace (TRACE_LEVEL_INFO, "PersistenceLocalObjectManager::updateInstanceId : Updating " + mostBaseTableNames[i]);

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
                trace (TRACE_LEVEL_FATAL, "PersistenceLocalObjectManager::updateInstanceId : Could not updated the Next Instance Id for table " + mostBaseTableNames[i] + string (" Completion Status : ") + FrameworkToolKit::localize (status));

                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "PersistenceLocalObjectManager::updateInstanceId : Could not updated the Next Instance Id for table " + mostBaseTableNames[i] + string ("Status : ") + FrameworkToolKit::localize (status));

            prismAssert (false, __FILE__, __LINE__);
        }

        trace (TRACE_LEVEL_INFO, "PersistenceLocalObjectManager::updateInstanceId : Updated " + mostBaseTableNames[i] + string (" : Latest Instance Id : ") + latestInstanceId);

        OrmRepository::updateLatestInstanceIdForTable (mostBaseTableNames[i], latestInstanceId);
    }
}

void PersistenceLocalObjectManager::executeTransactionMessageHandler (PersistenceLocalObjectManagerExecuteTransactionMessage *pPersistenceLocalObjectManagerExecuteTransactionMessage)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceLocalObjectManager::executeTransactionMessageHandler Entering");

    prismAssert (pPersistenceLocalObjectManagerExecuteTransactionMessage != NULL, __FILE__, __LINE__);

    const string &sqlString = pPersistenceLocalObjectManagerExecuteTransactionMessage->getSql ();

    //if ((getDisableLocalCommitTransactions () == false) || (FrameworkToolKit::getSyncState () == OUT_OF_SYNC))
    if (getDisableLocalCommitTransactions () == false) 
    {
        UI32   transactionId             = pPersistenceLocalObjectManagerExecuteTransactionMessage->getTransactionId ();
        bool   isCommit                  = pPersistenceLocalObjectManagerExecuteTransactionMessage->getIsCommit ();
        bool   isRollback                = pPersistenceLocalObjectManagerExecuteTransactionMessage->getIsRollback ();
        bool   isPrepare                 = pPersistenceLocalObjectManagerExecuteTransactionMessage->getIsPrepare ();
        bool   isConfigurationChange     = pPersistenceLocalObjectManagerExecuteTransactionMessage->getIsConfigurationChange ();
        bool   isConfigurationTimeChange = pPersistenceLocalObjectManagerExecuteTransactionMessage->getIsConfigurationTimeChange ();
        LocationId senderLocationId      = pPersistenceLocalObjectManagerExecuteTransactionMessage->getSenderLocationId (); 

        if (senderLocationId != FrameworkToolKit::getClusterPrimaryLocationId ())
        {
            // If primary of this node is not same as one which initiated this transaction, this can cause lot of issue. Currently system is in bad state. in fabric layer two node in a fabric see themselves as primary node. This condition can cause pending prepare transaction in DB. Returning to sender as this node is not available. Once fabric recover, one node will takeover handle the situation properly.
            pPersistenceLocalObjectManagerExecuteTransactionMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE);
            reply (pPersistenceLocalObjectManagerExecuteTransactionMessage);
            return;
        }

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PersistenceLocalObjectManager::sendTransactionToDatabase),
            reinterpret_cast<PrismLinearSequencerStep> (&PersistenceLocalObjectManager::handleTransactionResult),
            reinterpret_cast<PrismLinearSequencerStep> (&PersistenceLocalObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PersistenceLocalObjectManager::prismLinearSequencerFailedStep),
        };
    
        PersistenceLocalExecuteTransactionContext *pPersistenceLocalExecuteTransactionContext = new PersistenceLocalExecuteTransactionContext (pPersistenceLocalObjectManagerExecuteTransactionMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    
        pPersistenceLocalExecuteTransactionContext->setCurrentTransactionId (transactionId);
        pPersistenceLocalExecuteTransactionContext->setSenderLocationId (senderLocationId);
        pPersistenceLocalExecuteTransactionContext->setIsCommit (isCommit);
        pPersistenceLocalExecuteTransactionContext->setIsRollback (isRollback);
        pPersistenceLocalExecuteTransactionContext->setIsPrepare (isPrepare);
        pPersistenceLocalExecuteTransactionContext->setIsConfigurationChange (isConfigurationChange);
        pPersistenceLocalExecuteTransactionContext->setIsConfigurationTimeChange (isConfigurationTimeChange);

        if (true == isCommit)
        {
            UI64 trackingNumber = pPersistenceLocalObjectManagerExecuteTransactionMessage->getAnyConfigurationChangeTrackingNumber ();

            PersistenceLocalObjectManager::setAnyConfigurationChangeTrackingNumber (trackingNumber);
        }

        pPersistenceLocalExecuteTransactionContext->holdAll ();
        pPersistenceLocalExecuteTransactionContext->start ();
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("PersistenceLocalObjectManager::executeTransactionMessageHandler Should not be processing local commitTransactions. Sql: ") + sqlString);

        pPersistenceLocalObjectManagerExecuteTransactionMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pPersistenceLocalObjectManagerExecuteTransactionMessage);
    }
}

void PersistenceLocalObjectManager::sendTransactionToDatabase (PersistenceLocalExecuteTransactionContext *pPersistenceLocalExecuteTransactionContext)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::sendTransactionToDatabase Entering");

    ResourceId status        = WAVE_MESSAGE_SUCCESS;
    bool       isCommit      = pPersistenceLocalExecuteTransactionContext->getIsCommit ();
    bool       isPrepare     = pPersistenceLocalExecuteTransactionContext->getIsPrepare ();
    bool       isConfigurationChange = pPersistenceLocalExecuteTransactionContext->getIsConfigurationChange ();

    PersistenceLocalObjectManagerExecuteTransactionMessage *pPersistenceLocalObjectManagerExecuteTransactionMessage = reinterpret_cast<PersistenceLocalObjectManagerExecuteTransactionMessage *> (pPersistenceLocalExecuteTransactionContext->getPPrismMessage ());

    const string &sqlString = pPersistenceLocalObjectManagerExecuteTransactionMessage->getSql ();

    ++(*pPersistenceLocalExecuteTransactionContext);

    DatabaseObjectManagerExecuteTransactionMessage *pDatabaseObjectManagerExecuteTransactionMessage = new DatabaseObjectManagerExecuteTransactionMessage (sqlString);

    pDatabaseObjectManagerExecuteTransactionMessage->setIsCommit (isCommit);
    pDatabaseObjectManagerExecuteTransactionMessage->setIsPrepare (isPrepare);

    status = send (pDatabaseObjectManagerExecuteTransactionMessage, reinterpret_cast<PrismMessageResponseHandler        > (&PersistenceLocalObjectManager::executeTransactionSendRequestCallback), pPersistenceLocalExecuteTransactionContext); 

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("PersistenceLocalObjectManager::sendTransactionToDatabase: failed to send database transaction ") + FrameworkToolKit::localize (status));

        pPersistenceLocalObjectManagerExecuteTransactionMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
        --(*pPersistenceLocalExecuteTransactionContext);
        pPersistenceLocalExecuteTransactionContext->executeNextStep (status);
        return;
    }
    else
    {
        ++(*pPersistenceLocalExecuteTransactionContext);
    }

    if (PersistenceLocalObjectManager::getLiveSyncEnabled ())
    {
        LocationId haPeerLocationId = FrameworkToolKit::getHaPeerLocationId ();

        DatabaseObjectManagerExecuteTransactionMessage *pDatabaseObjectManagerExecuteTransactionMessage2 = new DatabaseObjectManagerExecuteTransactionMessage (sqlString);

        pDatabaseObjectManagerExecuteTransactionMessage2->setIsCommit (isCommit);
        pDatabaseObjectManagerExecuteTransactionMessage2->setIsPrepare (isPrepare);

        status = send (pDatabaseObjectManagerExecuteTransactionMessage2, reinterpret_cast<PrismMessageResponseHandler> (&PersistenceLocalObjectManager::executeTransactionSendRequestStandbyCallback), pPersistenceLocalExecuteTransactionContext, 0, haPeerLocationId); 

        if (status != WAVE_MESSAGE_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, string ("PersistenceLocalObjectManager::sendTransactionToDatabase: failed to execute database transaction on the standby ") + FrameworkToolKit::localize (status));

            PersistenceLocalObjectManager::setLiveSyncEnabled (false);
            FrameworkToolKit::notifySyncUpdateFailure(WAVE_MESSAGE_ERROR_LIVE_CONFIG_HASYNC_FAILED);
        }
        else
        {
            ++(*pPersistenceLocalExecuteTransactionContext);
        }
    }
    else if (isConfigurationChange && (FrameworkToolKit::getSyncState () == IN_SYNC))
    {
        // DB previously in sync from sync dump. but live transaction sync not supported. So set out of sync and notify on sync update fail
        trace (TRACE_LEVEL_WARN, string ("PersistenceLocalObjectManager::sendTransactionToDatabase: Live sync disabled on dual MM when new transaction received: Sql:  ") + sqlString);
        FrameworkToolKit::notifySyncUpdateFailure(WAVE_MESSAGE_ERROR_LIVE_CONFIG_HASYNC_NOT_SUPPORTED);
    }
    else if (FrameworkToolKit::getIsSyncDumpCompleted() == false)
    {
        if (isPrepare == true)
        {
            m_unsyncedPendingPrepareTransaction = sqlString;
        }
    }

    if ((isCommit == true) && (m_unsyncedPendingPrepareTransaction.size() != 0))
    {
        m_unsyncedPendingPrepareTransaction.clear();
    }

    --(*pPersistenceLocalExecuteTransactionContext);

    pPersistenceLocalExecuteTransactionContext->executeNextStep (status);

    return;
}

void PersistenceLocalObjectManager::executeTransactionSendRequestCallback (FrameworkStatus frameworkStatus, DatabaseObjectManagerExecuteTransactionMessage *pDatabaseObjectManagerExecuteTransactionMessage, PersistenceLocalExecuteTransactionContext *pPersistenceLocalExecuteTransactionContext)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceLocalObjectManager::executeTransactionSendRequestCallback : Entering ...");

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    --(*pPersistenceLocalExecuteTransactionContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pDatabaseObjectManagerExecuteTransactionMessage != NULL, __FILE__, __LINE__);

        ResourceId completionStatus = pDatabaseObjectManagerExecuteTransactionMessage->getCompletionStatus ();

        status = completionStatus;
        pPersistenceLocalExecuteTransactionContext->setActiveResult(status);
        if (WAVE_MESSAGE_SUCCESS != completionStatus) 
        {
            trace (TRACE_LEVEL_ERROR, string ("PersistenceLocalObjectManager::executeTransactionSendRequestCallback  database transaction failed: ") + FrameworkToolKit::localize (completionStatus));
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("PersistenceLocalObjectManager::executeTransactionSendRequestCallback frameworkStatus error: ")+FrameworkToolKit::localize (frameworkStatus));

        status = WAVE_MESSAGE_ERROR;
        pPersistenceLocalExecuteTransactionContext->setActiveResult(status);
        prismAssert (false, __FILE__, __LINE__);
    }

    if (NULL != pDatabaseObjectManagerExecuteTransactionMessage)
    {
        delete pDatabaseObjectManagerExecuteTransactionMessage;
    }

    pPersistenceLocalExecuteTransactionContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

void PersistenceLocalObjectManager::executeTransactionSendRequestStandbyCallback (FrameworkStatus frameworkStatus, DatabaseObjectManagerExecuteTransactionMessage *pDatabaseObjectManagerExecuteTransactionMessage, PersistenceLocalExecuteTransactionContext *pPersistenceLocalExecuteTransactionContext)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceLocalObjectManager::executeTransactionSendRequestStandbyCallback : Entering ...");

    --(*pPersistenceLocalExecuteTransactionContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pDatabaseObjectManagerExecuteTransactionMessage != NULL, __FILE__, __LINE__);

        ResourceId completionStatus = pDatabaseObjectManagerExecuteTransactionMessage->getCompletionStatus ();

        pPersistenceLocalExecuteTransactionContext->setStandbyResult(completionStatus);

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PersistenceLocalObjectManager::executeTransactionSendRequestStandbyCallback  database transaction failed: ") + FrameworkToolKit::localize (completionStatus));
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("PersistenceLocalObjectManager::executeTransactionSendRequestStandbyCallback request send failed: ") + FrameworkToolKit::localize (frameworkStatus));

        ResourceId status = WAVE_MESSAGE_ERROR;
        pPersistenceLocalExecuteTransactionContext->setStandbyResult(status);
    }

    if (NULL != pDatabaseObjectManagerExecuteTransactionMessage)
    {
        delete pDatabaseObjectManagerExecuteTransactionMessage;
    }

    pPersistenceLocalExecuteTransactionContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

void PersistenceLocalObjectManager::handleTransactionResult (PersistenceLocalExecuteTransactionContext *pPersistenceLocalExecuteTransactionContext)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceLocalObjectManager::handleTransactionResult Entering");

    ResourceId activeResult       = pPersistenceLocalExecuteTransactionContext->getActiveResult();
    ResourceId standbyResult      = pPersistenceLocalExecuteTransactionContext->getStandbyResult();
    bool       isPrepare          = pPersistenceLocalExecuteTransactionContext->getIsPrepare ();
    UI32       transactionId      = pPersistenceLocalExecuteTransactionContext->getCurrentTransactionId ();
    LocationId senderLocationId   = pPersistenceLocalExecuteTransactionContext->getSenderLocationId (); 
    ResourceId status             = WAVE_MESSAGE_SUCCESS;

    // If live sync is disabled, or live sync is enabled with active and standby get the same transaction result
	if ((false == getLiveSyncEnabled ()) || ((true == getLiveSyncEnabled ()) && (((activeResult == WAVE_MESSAGE_SUCCESS) && (standbyResult == WAVE_MESSAGE_SUCCESS)) || ((activeResult != WAVE_MESSAGE_SUCCESS) && (standbyResult != WAVE_MESSAGE_SUCCESS)))))
    {
         status = activeResult;
    }
    else if ((true == PersistenceLocalObjectManager::getLiveSyncEnabled ()) && (activeResult != WAVE_MESSAGE_SUCCESS) && (standbyResult == WAVE_MESSAGE_SUCCESS) && isPrepare)
    {
         trace (TRACE_LEVEL_INFO, string ("PersistenceLocalObjectManager::handleTransactionResult: notify standby to rollback last transaction."));
         // TODO: does the location id need to be the principal's location id
         LocationId haPeerLocationId = FrameworkToolKit::getHaPeerLocationId ();
         string rollbackPreparedSql  = string ("ROLLBACK PREPARED 'TX") + senderLocationId + string (".") + transactionId + string ("';");

         DatabaseObjectManagerExecuteTransactionMessage *pDatabaseObjectManagerExecuteTransactionMessage2 = new DatabaseObjectManagerExecuteTransactionMessage (rollbackPreparedSql);

         ResourceId retStatus = sendSynchronously (pDatabaseObjectManagerExecuteTransactionMessage2, haPeerLocationId);

         // TODO: separate print return status
         if ((WAVE_MESSAGE_SUCCESS != retStatus) || (pDatabaseObjectManagerExecuteTransactionMessage2->getCompletionStatus () != WAVE_MESSAGE_SUCCESS))
         {
             trace (TRACE_LEVEL_ERROR, string ("PersistenceLocalObjectManager::handleTransactionResult - failed to rollback transaction to standby ") + FrameworkToolKit::localize (retStatus));
             trace (TRACE_LEVEL_INFO, string ("PersistenceLocalObjectManager::handleTransactionResult: Mark standby MM out of sync and stop live sync."));

             PersistenceLocalObjectManager::setLiveSyncEnabled (false);

             // Set sync state to out of sync and notify on sync update fail
             FrameworkToolKit::notifySyncUpdateFailure(WAVE_MESSAGE_ERROR_CONFIG_UPDATE_FAILED);
         }
         status = activeResult;
    }
    else if (true == PersistenceLocalObjectManager::getLiveSyncEnabled ())
    {
         trace (TRACE_LEVEL_ERROR, string ("PersistenceLocalObjectManager::handleTransactionResult: Active and standby return different result on commit transaction at non-prepare stage."));
         trace (TRACE_LEVEL_INFO, string ("PersistenceLocalObjectManager::handleTransactionResult: Mark standby MM out of sync and stop live sync."));
         PersistenceLocalObjectManager::setLiveSyncEnabled (false);
         // Set sync state to out of sync and notify on sync update fail
         FrameworkToolKit::notifySyncUpdateFailure(WAVE_MESSAGE_ERROR_CONFIG_UPDATE_FAILED);
         status = activeResult;
    }

    PersistenceLocalObjectManagerExecuteTransactionMessage *pPersistenceLocalObjectManagerExecuteTransactionMessage = reinterpret_cast<PersistenceLocalObjectManagerExecuteTransactionMessage *> (pPersistenceLocalExecuteTransactionContext->getPPrismMessage ());

    pPersistenceLocalObjectManagerExecuteTransactionMessage->setSql ("");

    pPersistenceLocalExecuteTransactionContext->executeNextStep (status);
}

bool PersistenceLocalObjectManager::getLiveSyncEnabled ()
{
    bool liveSyncEnabled;

    m_liveSyncEnabledMutex.lock ();

    liveSyncEnabled = m_liveSyncEnabled;

    m_liveSyncEnabledMutex.unlock ();

    return liveSyncEnabled;
}

void PersistenceLocalObjectManager::setLiveSyncEnabled (const bool &liveSyncEnabled)
{
    m_liveSyncEnabledMutex.lock ();

    m_liveSyncEnabled = liveSyncEnabled;

    m_liveSyncEnabledMutex.unlock ();
}

bool PersistenceLocalObjectManager::getDisableLocalCommitTransactions ()
{
    bool disableLocalCommitTransactions;

    m_disableLocalCommitTransactionsMutex.lock ();

    disableLocalCommitTransactions = m_disableLocalCommitTransactions;

    m_disableLocalCommitTransactionsMutex.unlock ();

    return disableLocalCommitTransactions;
}

void PersistenceLocalObjectManager::setDisableLocalCommitTransactions (const bool &disableLocalCommitTransactions)
{
    m_disableLocalCommitTransactionsMutex.lock ();

    m_disableLocalCommitTransactions = disableLocalCommitTransactions;

    m_disableLocalCommitTransactionsMutex.unlock ();
}

void PersistenceLocalObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_INFO, "PersistenceLocalObjectManager::boot : Entering ...");

    setDisableLocalCommitTransactions (false);

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void PersistenceLocalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_INFO, "PersistenceLocalObjectManager::install : Entering ...");

    setDisableLocalCommitTransactions (false);

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void PersistenceLocalObjectManager::haboot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_INFO, "PersistenceLocalObjectManager::haboot : Entering ...");

    setDisableLocalCommitTransactions (true);

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void PersistenceLocalObjectManager::hainstall (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_INFO, "PersistenceLocalObjectManager::hainstall : Entering ...");

    setDisableLocalCommitTransactions (true);

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
} 

void PersistenceLocalObjectManager::shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    //trace (TRACE_LEVEL_DEVEL, "PersistenceLocalObjectManager::shutdown : Entering ...");

    vector<WaveManagedObject *>    *pResults    = NULL;

    // Query for the Global MO "ConfigurationManagedObject".  This Global MO will be cached in the PersistenceToolKit.  The cached information will be used to still provide configuration information during add/rejoin phase 0/1 where local DB will be emptied.   

    pResults = WaveObjectManagerToolKit::querySynchronously (ConfigurationManagedObject::getClassName ());

    if (NULL != pResults)
    {
        prismAssert ((0 == pResults->size ()) || (1 == pResults->size ()), __FILE__, __LINE__);

        if (1 == pResults->size ())
        {
            // Get the configuration number for queried result

            ConfigurationManagedObject *pConfigurationManagedObject = dynamic_cast<ConfigurationManagedObject *> ((*pResults)[0]);
            prismAssert (NULL != pConfigurationManagedObject, __FILE__, __LINE__);

            PersistenceToolKit::setConfigurationManagedObject (pConfigurationManagedObject);
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "PersistenceLocalObjectManager::shutdown : Query for the global configuration managed object returned no results.");
        }
    }
    else
    {   
        trace (TRACE_LEVEL_ERROR, "PersistenceLocalObjectManager::shutdown : Query for the global configuration managed object failed.");
    }

    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void PersistenceLocalObjectManager::enableTransactionMessageHandler (PersistenceLocalObjectManagerEnableTransactionMessage *pPersistenceLocalObjectManagerEnableTransactionMessage)
{
    trace (TRACE_LEVEL_INFO, "PersistenceLocalObjectManager::enableTransactionMessageHandler Entering");

    prismAssert (pPersistenceLocalObjectManagerEnableTransactionMessage != NULL, __FILE__, __LINE__);

    if (getDisableLocalCommitTransactions () == true)
    {
        updateInstanceId ();
        setDisableLocalCommitTransactions (false);
    }

    pPersistenceLocalObjectManagerEnableTransactionMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pPersistenceLocalObjectManagerEnableTransactionMessage);
    return;
}

void PersistenceLocalObjectManager::setLiveSyncEnableMessageHandler (PersistenceLocalObjectManagerSetLiveSyncEnableMessage *pPersistenceLocalObjectManagerSetLiveSyncEnableMessage)
{
    bool livesyncEnabled;
    ResourceId  status  = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEBUG, "PersistenceLocalObjectManager::setLiveSyncEnableMessageHandler Entering");

    prismAssert (pPersistenceLocalObjectManagerSetLiveSyncEnableMessage != NULL, __FILE__, __LINE__);

    livesyncEnabled = pPersistenceLocalObjectManagerSetLiveSyncEnableMessage->getIsLiveSyncEnabled ();

    if ((livesyncEnabled == true) && (m_unsyncedPendingPrepareTransaction.size() != 0))
    {
       trace (TRACE_LEVEL_INFO, string("PersistenceLocalObjectManager::send unsynced pending prepare transaction to standby"));
       LocationId haPeerLocationId = FrameworkToolKit::getHaPeerLocationId ();

       DatabaseObjectManagerExecuteTransactionMessage *pDatabaseObjectManagerExecuteTransactionMessage = new DatabaseObjectManagerExecuteTransactionMessage (m_unsyncedPendingPrepareTransaction);

        pDatabaseObjectManagerExecuteTransactionMessage->setIsCommit (false);
        pDatabaseObjectManagerExecuteTransactionMessage->setIsPrepare (true);

        status = sendOneWay (pDatabaseObjectManagerExecuteTransactionMessage, haPeerLocationId); 

        if (status != WAVE_MESSAGE_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, string ("PersistenceLocalObjectManager:: failed to sync the pending prepare transaction to standby. Disable live sync ") + FrameworkToolKit::localize (status)); 
            livesyncEnabled = false;
            FrameworkToolKit::notifySyncUpdateFailure(WAVE_MESSAGE_ERROR_LIVE_CONFIG_HASYNC_FAILED);
        }

        m_unsyncedPendingPrepareTransaction.clear();
    }

    setLiveSyncEnabled (livesyncEnabled);

    trace (TRACE_LEVEL_INFO, string("PersistenceLocalObjectManager::setLiveSyncEnableMessageHandler: Live Sync: ") + PersistenceLocalObjectManager::getLiveSyncEnabled ());

    pPersistenceLocalObjectManagerSetLiveSyncEnableMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pPersistenceLocalObjectManagerSetLiveSyncEnableMessage);
    return;
}

void PersistenceLocalObjectManager::getLiveSyncEnableMessageHandler (PersistenceLocalObjectManagerGetLiveSyncEnableMessage *pPersistenceLocalObjectManagerGetLiveSyncEnableMessage)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceLocalObjectManager::getLiveSyncEnableMessageHandler Entering");

    prismAssert (pPersistenceLocalObjectManagerGetLiveSyncEnableMessage != NULL, __FILE__, __LINE__);

    pPersistenceLocalObjectManagerGetLiveSyncEnableMessage->setIsLiveSyncEnabled (PersistenceLocalObjectManager::getLiveSyncEnabled ());

    reply (pPersistenceLocalObjectManagerGetLiveSyncEnableMessage);
    return;
}

void PersistenceLocalObjectManager::setAnyConfigurationChangeTrackingNumber (UI64 &trackingNumber)
{
    m_anyConfiguraitonChangeTrackingNumberMutex.lock ();

    m_anyConfiguraitonChangeTrackingNumber = trackingNumber;
    
    m_anyConfiguraitonChangeTrackingNumberMutex.unlock ();
}

UI64 PersistenceLocalObjectManager::getAnyConfigurationChangeTrackingNumber ()
{
    UI64 trackingNumber;

    m_anyConfiguraitonChangeTrackingNumberMutex.lock ();
    
    trackingNumber = m_anyConfiguraitonChangeTrackingNumber;

    m_anyConfiguraitonChangeTrackingNumberMutex.unlock ();

    return (trackingNumber);
}

void PersistenceLocalObjectManager::setUpdateGlobalConfigInKernelHandlerFunction (UpdateGlobalConfigInKernelHandlerFunction updateGlobalConfigInKernelHandlerFunction)
{   
    m_updateGlobalConfigInKernelHandlerFunction = updateGlobalConfigInKernelHandlerFunction;
} 

ResourceId PersistenceLocalObjectManager::updateGlobalConfigInKernel (string &globalConfigs)
{
    if (NULL != m_updateGlobalConfigInKernelHandlerFunction)
    {   
        return ((*m_updateGlobalConfigInKernelHandlerFunction) (globalConfigs));
    }
    trace (TRACE_LEVEL_ERROR, "PersistenceLocalObjectManager::updateGlobalConfigInKernel : The Function pointer UpdateGlobalConfigInKernelHandlerFunction is NULL");
    return (WAVE_MESSAGE_ERROR);
}

void PersistenceLocalObjectManager::pushConfigToKernelMessageHandler (PersistenceLocalObjectManagerPushConfigToKernelMessaage *pPersistenceLocalObjectManagerPushConfigToKernelMessaage)
{
    trace (TRACE_LEVEL_DEBUG, "PersistenceLocalObjectManager::pushConfigToKernelMessageHandler Entering");

    prismAssert (pPersistenceLocalObjectManagerPushConfigToKernelMessaage != NULL, __FILE__, __LINE__);

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    string     globalConfigs = pPersistenceLocalObjectManagerPushConfigToKernelMessaage->getGlobalConfigs ();

    status = updateGlobalConfigInKernel (globalConfigs);
    pPersistenceLocalObjectManagerPushConfigToKernelMessaage->setCompletionStatus (status);
    reply (pPersistenceLocalObjectManagerPushConfigToKernelMessaage);
    return;
}

}
