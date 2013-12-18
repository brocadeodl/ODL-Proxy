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
 *@file VcsNodeConfigSnapshotWorker.cpp 
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

#include "VcsFabric/Local/VcsNodeConfigSnapshotWorker.h"
#include "VcsFabric/Local/VcsNodeLocalMessagingContext.h"
#include "VcsFabric/Local/VcsFabricLocalMessages.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Global/VcsFabConfigSnapshotWorker.h"
#include "VcsFabric/Global/VcsDeleteClusterMessage.h"
#include "VcsFabric/Global/VcsConfigSnapshotObject.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"

namespace DcmNs
{
bool        VcsNodeConfigSnapshotWorker::s_configSnapshotRestoreInProgressFlag;
PrismMutex  VcsNodeConfigSnapshotWorker::s_configSnapshotRestoreInProgressFlagMutex;

VcsNodeConfigSnapshotWorker::VcsNodeConfigSnapshotWorker ( WaveObjectManager* pWaveObjectManager )
    : WaveWorker ( pWaveObjectManager )
{
    addOperationMap (VCS_NODE_CONFIG_SNAPSHOT_RESTORE, reinterpret_cast<PrismMessageHandler> (&VcsNodeConfigSnapshotWorker::vcsNodeConfigSnapshotRestoreHandler));

    s_configSnapshotRestoreInProgressFlag = false;
}

VcsNodeConfigSnapshotWorker::~VcsNodeConfigSnapshotWorker ()
{
}

PrismMessage *VcsNodeConfigSnapshotWorker::createMessageInstance (const UI32 &operationCode)
{   
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case VCS_NODE_CONFIG_SNAPSHOT_RESTORE :
            pPrismMessage = new VcsNodeConfigSnapshotMessage (VCS_NODE_CONFIG_SNAPSHOT_RESTORE);
            break;
        default :
            pPrismMessage = NULL;
    }       

    return (pPrismMessage);    
}

ResourceId VcsNodeConfigSnapshotWorker::backupAllConfigSnapshotFiles ()
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;        
    string snapshotDirPath = (FrameworkToolKit::getProcessInitialWorkingDirectory() + "/" + VcsFabConfigSnapshotWorker::getConfigSnapshotDirectoryname());
    
    vector<string> allFilenames;
    //Get only filenames without complete path
    VcsFabConfigSnapshotWorker::getAllConfigSnapshotFilenames ( allFilenames, false );

    if ( 0 != allFilenames.size())
    {
        string temp("");
        
        for ( UI32 i = 0; i < allFilenames.size(); i++ )
        {
            temp = temp + " " + allFilenames[i] ;
        }

        string cmdToBackup = "/bin/tar -czf " + snapshotDirPath + "/" + VcsFabConfigSnapshotWorker::getConfigSnapshotBackupFilename() + " --remove-files -C "+ snapshotDirPath + (" ") + temp;

        trace (TRACE_LEVEL_DEVEL, string("VcsNodeConfigSnapshotWorker:: backupAllConfigSnapshotFiles : cmdToBackup \"") + cmdToBackup + ("\""));
        SI32 ret = 0;
        vector<string> commandOutput;

        ret = FrameworkToolKit::systemCommandOutput (cmdToBackup, commandOutput );

        if ( 0 != ret )
        {
            status = WAVE_MESSAGE_ERROR;
            trace (TRACE_LEVEL_ERROR, string("VcsNodeConfigSnapshotWorker:: backupAllConfigSnapshotFiles : Failed with ret-") + WEXITSTATUS(ret));
        }
    }
    return (status);
}

ResourceId VcsNodeConfigSnapshotWorker::restoreAllBackedupConfigSnapshotFiles ()
{   
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    string snapshotDirPath = (FrameworkToolKit::getProcessInitialWorkingDirectory() + "/" + VcsFabConfigSnapshotWorker::getConfigSnapshotDirectoryname());

    VcsFabConfigSnapshotWorker::deleteAllConfigSnapshotFiles (false);

    if ( WAVE_MESSAGE_SUCCESS == VcsFabConfigSnapshotWorker::checkIfSnapshotBackupFilePresent())
    {
        string cmdToRestore = "/bin/tar -xzf " + snapshotDirPath + "/" + VcsFabConfigSnapshotWorker::getConfigSnapshotBackupFilename() + " -C " + snapshotDirPath ;
        trace (TRACE_LEVEL_DEVEL, string("VcsNodeConfigSnapshotWorker::restoreAllBackedupConfigSnapshotFiles : cmdToToRestore \"") + cmdToRestore + ("\""));
    
        SI32 ret = 0;
        vector<string> commandOutput;

        ret = FrameworkToolKit::systemCommandOutput (cmdToRestore, commandOutput );

        if ( 0 != ret )
        {
            status = WAVE_MESSAGE_ERROR;
            trace (TRACE_LEVEL_ERROR, string("VcsNodeConfigSnapshotWorker:: restoreAllBackedupConfigSnapshotFiles : Failed with ret = ") + WEXITSTATUS(ret));
        }
    }
    return (status);
}

void VcsNodeConfigSnapshotWorker::vcsNodeConfigSnapshotRestoreHandler (VcsNodeConfigSnapshotMessage *pVcsNodeConfigSnapshotMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {   
        // DomainId and RbridgeId are the same
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeConfigSnapshotWorker::vcsNodeConfigSnapshotRestoreValidationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeConfigSnapshotWorker::copyDefaultToStartAndRebootIfNoCluster),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeConfigSnapshotWorker::vcsNodeConfigSnapshotRestoreCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeConfigSnapshotWorker::disconnectPrincipalFromAllLocationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeConfigSnapshotWorker::unconfigureThisNodeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeConfigSnapshotWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeConfigSnapshotWorker::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeConfigSnapshotMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    UI32 rBridgeId = pVcsNodeConfigSnapshotMessage->getRbridgeId ();

    pVcsNodeLocalMessagingContext->setDomainId ( rBridgeId );
    pVcsNodeLocalMessagingContext->setSnapshotId ( pVcsNodeConfigSnapshotMessage->getSnapshotId());
    pVcsNodeLocalMessagingContext->execute ();
}

ResourceId VcsNodeConfigSnapshotWorker::vcsNodeConfigSnapshotRestoreValidationStep ( VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext )
{
    trace (TRACE_LEVEL_DEVEL, string("VcsNodeConfigSnapshotWorker::vcsNodeConfigSnapshotRestoreValidationStep : Entered .."));
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    VcsNodeConfigSnapshotMessage *pVcsNodeConfigSnapshotMessage = reinterpret_cast<VcsNodeConfigSnapshotMessage*> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    ResourceId currentVcsFabricLocalState = VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ();

    // We may need to check for other state as well
    if ( VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER != currentVcsFabricLocalState )
    {
        if ( VCS_FABRIC_LOCAL_NODE_RESTORE_TRIGGERED == currentVcsFabricLocalState || true == getConfigSnapshotRestoreFlag() )
        {   
            trace (TRACE_LEVEL_ERROR, string("VcsNodeConfigSnapshotWorker::vcsNodeConfigSnapshotRestoreValidationStep : A restore is already in progress, cannot be triggered again. VcsFabricState -") + FrameworkToolKit::localizeToSourceCodeEnum(currentVcsFabricLocalState) + (", RestoreFlag-") + getConfigSnapshotRestoreFlag());
            status = VCS_CONFIG_SNAPSHOT_MULTI_RESTORE_REQUEST;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("VcsNodeConfigSnapshotWorker::vcsNodeConfigSnapshotRestoreValidationStep : Restore not allowed in transient state-") + FrameworkToolKit::localizeToSourceCodeEnum(currentVcsFabricLocalState) );
            status = VCS_CLUSTER_NODE_IN_TRANSITION;
        }
        return (status);
    }

    string snapshotLocalFileName ("");
    string snapshotGlobalFileName ("");
    
    VcsFabConfigSnapshotWorker::getConfigSnapshotCompleteFileNames( pVcsNodeConfigSnapshotMessage->getSnapshotId(), pVcsNodeConfigSnapshotMessage->getRbridgeId(), snapshotLocalFileName, snapshotGlobalFileName );

    UI32            countOfSnapshotsForSelectedRbridgeId = 0;
    vector<string>  localFileNames;
    vector<string>  globalFileNames;
    status = VcsFabConfigSnapshotWorker::checkIfSnapshotFilesPresent ( pVcsNodeConfigSnapshotMessage->getSnapshotId(), pVcsNodeConfigSnapshotMessage->getRbridgeId(), countOfSnapshotsForSelectedRbridgeId, localFileNames, globalFileNames ) ;

    if ( WAVE_MESSAGE_SUCCESS != status )
    {
         trace (TRACE_LEVEL_ERROR, string ("VcNodeConfigSnapshotWorker::vcsNodeConfigSnapshotRestoreValidationStep : SnapshotId -") + pVcsNodeConfigSnapshotMessage->getSnapshotId() + (", not present for rbridgeId - ") + pVcsNodeConfigSnapshotMessage->getRbridgeId());
        status = VCS_CONFIG_SNAPSHOT_ID_NOT_PRESENT;
        return (status);
    }

    status = VcsFabConfigSnapshotWorker::setConfigSnapshotReplayFile ( snapshotLocalFileName, snapshotGlobalFileName );

    return (status);
}

ResourceId VcsNodeConfigSnapshotWorker::copyDefaultToStartAndRebootIfNoCluster ( VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext )
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    VcsNodeConfigSnapshotMessage *pVcsNodeConfigSnapshotMessage = reinterpret_cast<VcsNodeConfigSnapshotMessage*> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    if ( true == pVcsNodeConfigSnapshotMessage->getIsNodePrimary() )
    {
        //Node is StandAlone or Primary
        VcsClusterManagedObject *pVcsClusterManagedObject = (VcsFabObjectManager::getInstance())->getVcsClusterManagedObject ();
    
        UI32 nodesInCluster = 0;
        if ( NULL != pVcsClusterManagedObject )
        {
            nodesInCluster = pVcsClusterManagedObject->getSwitchCount ();
    
            if (1 < nodesInCluster)
            {   
                trace (TRACE_LEVEL_DEBUG, string ("VcNodeConfigSnapshotWorker::copyDefaultToStartAndRebootIfNoCluster : restore of the node issued - Control Failover"));
                // this flag determines on primary whether commit and disconnect are required or not
                pVcsNodeLocalMessagingContext->setPrincipalControlFailoverFlag(true);
            }
            delete pVcsClusterManagedObject;
            pVcsNodeConfigSnapshotMessage->setCompletionStatus (status);
        }
        else
        {
            // if no cluster then we must reboot here itself after copy default
    
            //Need to handle roll back incase of failure
            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState( VCS_FABRIC_LOCAL_NODE_RESTORE_TRIGGERED );
            setConfigSnapshotRestoreFlag(true);
        
            pVcsNodeLocalMessagingContext->setPPrismMessage (NULL);
    
            pVcsNodeConfigSnapshotMessage->setCompletionStatus (status);
            reply ( pVcsNodeConfigSnapshotMessage );

            pVcsNodeConfigSnapshotMessage = NULL;
    
            string retMessage = "";
            status = (VcsFabricLocalObjectManager::getInstance())->copyDefaultToStartInModeChange (true, true, retMessage);
            
            if ( WAVE_MESSAGE_SUCCESS != status )
            {
                trace (TRACE_LEVEL_ERROR, string("VcsNodeConfigSnapshotWorker::copyDefaultToStartAndRebootIfNoCluster : Could not set copy default to start, status = \"") + FrameworkToolKit::localize (status) + ("\"")); 
            }
            
            status = (VcsFabricLocalObjectManager::getInstance())->rebootLocalNode (false);
            
            if ( WAVE_MESSAGE_SUCCESS != status )
            {
                trace (TRACE_LEVEL_ERROR, string("VcsNodeConfigSnapshotWorker::copyDefaultToStartAndRebootIfNoCluster : Could not reboot local node, status = \"") + FrameworkToolKit::localize (status) + ("\""));
            }
        }
    }
        
    return (status);
}

ResourceId VcsNodeConfigSnapshotWorker::vcsNodeConfigSnapshotRestoreCommitTransactionStep ( VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext )
{   
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    VcsNodeConfigSnapshotMessage *pVcsNodeConfigSnapshotMessage = reinterpret_cast<VcsNodeConfigSnapshotMessage*> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    if (true == pVcsNodeConfigSnapshotMessage->getIsNodePrimary()) 
    {
        //Primary node with cluster

        if ( true == pVcsNodeLocalMessagingContext->getPrincipalControlFailoverFlag() )
        {
            // Cluster with atleast 2nodes. Need to persist switch delete for primary
            trace (TRACE_LEVEL_INFO, string("VcsNodeConfigSnapshotWorker::vcsNodeConfigSnapshotRestoreCommitTransactionStep : Restore on primary having more than 1 node in cluster"));

            VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager = (VcsFabricLocalObjectManager::getInstance());

            startTransaction ();

            pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectSwitchDeleteAfterFailover (true);

            pVcsFabricLocalObjectManager->updateThisVcsNodeLocalManagedObject ();

            if (FRAMEWORK_SUCCESS != commitTransaction())
            {
                status = VCS_CONFIG_SNAPSHOT_COMMIT_DATABASE_FAIL;
                WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodeConfigSnapshotWorker::vcsNodeConfigSnapshotRestoreCommitTransactionStep : Failed to set this node's config cleanup flag in Database, Node-locationId = ") + FrameworkToolKit::getThisLocationId ());
                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else
        {
            // Single node cluster. Set the primary flag now since we will reply the message here
            // This flag tells whether the current node is primary here after
            pVcsNodeLocalMessagingContext->setPrincipalControlFailoverFlag(true);
        }
    }

    //Need to handle roll back incase of failure
    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState( VCS_FABRIC_LOCAL_NODE_RESTORE_TRIGGERED );
    setConfigSnapshotRestoreFlag(true);

    pVcsNodeLocalMessagingContext->setPPrismMessage (NULL);

    pVcsNodeConfigSnapshotMessage->setCompletionStatus (status);
    reply ( pVcsNodeConfigSnapshotMessage );
    pVcsNodeConfigSnapshotMessage = NULL;

    return (status);
}

ResourceId VcsNodeConfigSnapshotWorker::disconnectPrincipalFromAllLocationStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    trace (TRACE_LEVEL_DEBUG, "VcsNodeConfigSnapshotWorker::disconnectPrincipalFromAllLocationStep: Entering...");

    if ( true == pVcsNodeLocalMessagingContext->getPrincipalControlFailoverFlag() )
    {
        // If this node is principal
        trace (TRACE_LEVEL_DEBUG, "VcsNodeConfigSnapshotWorker::disconnectPrincipalFromAllLocationStep : Disconnect from all other secondary");
        PrismFrameworkObjectManager::disconnectFromAllKnownLocations ();
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsNodeConfigSnapshotWorker::unconfigureThisNodeStep ( VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext )
{
    trace (TRACE_LEVEL_DEVEL, string("VcsNodeConfigSnapshotWorker::unconfigureThisNodeStep : Entered .."));
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;

    if ( false ==  pVcsNodeLocalMessagingContext->getPrincipalControlFailoverFlag () )
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeConfigSnapshotWorker::unconfigureThisNodeStep : Unconfiguring a SecondaryNode");
        string ipAddress = "";
        SI32 port = 0;

        ipAddress = FrameworkToolKit::getIpAddressForLocationId (FrameworkToolKit::getThisLocationId ());
        port = FrameworkToolKit::getPortForLocationId (FrameworkToolKit::getThisLocationId ());

        FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage = new FrameworkObjectManagerUnconfigureClusterSecondaryMessage (ipAddress, port);
        status = sendSynchronously (pMessage, FrameworkToolKit::getThisLocationId ());
        completionStatus =  pMessage->getCompletionStatus ();
        delete pMessage;
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeConfigSnapshotWorker::unconfigureThisNodeStep : Unconfiguring a PrimaryNode");
        VcsDeleteClusterMessage *pMessage = new VcsDeleteClusterMessage ();
        status = sendSynchronously (pMessage);
        completionStatus =  pMessage->getCompletionStatus ();
        delete pMessage;
    }
    if (WAVE_MESSAGE_SUCCESS != status)
    {   
        trace (TRACE_LEVEL_ERROR, "VcsNodeConfigSnapshotWorker::unconfigureThisNodeStep : Error in sending message : Status : \"" + FrameworkToolKit::localize (status) + "\"");
    }
    else
    {   
        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            status = completionStatus ;
            trace (TRACE_LEVEL_ERROR, "VcsNodeConfigSnapshotWorker::unconfigureThisNodeStep : Unconfigure failed with Message-CompletionStatus : \"" + FrameworkToolKit::localize (completionStatus) + "\"");
        }
    }
    return (status);
}

void VcsNodeConfigSnapshotWorker::setConfigSnapshotRestoreFlag ( const bool &restoreFlag )
{
    s_configSnapshotRestoreInProgressFlagMutex.lock();

    s_configSnapshotRestoreInProgressFlag = restoreFlag;

    s_configSnapshotRestoreInProgressFlagMutex.unlock();
}

bool VcsNodeConfigSnapshotWorker::getConfigSnapshotRestoreFlag ()
{
    bool restoreFlag = false ;

    s_configSnapshotRestoreInProgressFlagMutex.lock();

    restoreFlag = s_configSnapshotRestoreInProgressFlag;

    s_configSnapshotRestoreInProgressFlagMutex.unlock();

    return (restoreFlag);
}

}
