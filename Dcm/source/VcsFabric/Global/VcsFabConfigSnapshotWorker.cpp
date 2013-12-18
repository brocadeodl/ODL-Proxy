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
 *@file VcsFabConfigSnapshotWorker.cpp 
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc. All
 * rights reserved.
 * Description: This file declares the
 *              Worker class which is responsible for processing
 *              Create,Restore,delete Snapshot request
 *         
 *              
 * Author :     Anil ChannaveeraSetty 
 *  
 */
#include "VcsFabric/Global/VcsFabConfigSnapshotWorker.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "VcsFabric/Global/VcsFabricGlobalMessages.h"
#include "VcsFabric/Global/VcsSnapshotFromFtpMessage.h"
#include "VcsFabric/Global/VcsFabConfigSnapshotContext.h"
#include "VcsFabric/Global/VcsConfigSnapshotObject.h"
#include "VcsFabric/Local/VcsFabricLocalMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "DcmCore/DcmToolKit.h"
#include "File/Local/FileLocalTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Shutdown/PrismFinalizeWorker.h"
#include "Cluster/ClusterMessages.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Types/DateTime.h"
#include "Framework/Utils/StringUtils.h"
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>

extern "C"
{
    #include "raslog/raslog.h"
    #include "raslog/raslog_vcs.h"
}

namespace DcmNs
{

string  VcsFabConfigSnapshotWorker::s_configSnapshotFilenameExtension       (".cs");    
string  VcsFabConfigSnapshotWorker::s_configSnapshotLocalFilenameSuffix     ("local");
string  VcsFabConfigSnapshotWorker::s_configSnapshotGlobalFilenameSuffix    ("global");
string  VcsFabConfigSnapshotWorker::s_configSnapshotDirectoryname           ("ConfigSnapshot");
string  VcsFabConfigSnapshotWorker::s_configSnapshotBackupFilename          ("configSnapshotFiles.tgz");
char    VcsFabConfigSnapshotWorker::s_configSnapshotFilenameTokenSeparator = ('-');
UI32    VcsFabConfigSnapshotWorker::s_maxSnapshotsPerRbridgeId = 4;

VcsFabConfigSnapshotWorker::VcsFabConfigSnapshotWorker (WaveObjectManager* pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    createSnapshotDirectory();

    addOperationMap (VCS_FAB_CONFIG_SNAPSHOT_CREATE, reinterpret_cast<PrismMessageHandler> (&VcsFabConfigSnapshotWorker::configSnapshotCreateMessageHandler));
    addOperationMap (VCS_FAB_CONFIG_SNAPSHOT_RESTORE, reinterpret_cast<PrismMessageHandler> (&VcsFabConfigSnapshotWorker::configSnapshotRestoreMessageHandler));
    addOperationMap (VCS_FAB_CONFIG_SNAPSHOT_DELETE, reinterpret_cast<PrismMessageHandler> (&VcsFabConfigSnapshotWorker::configSnapshotDeleteMessageHandler));
    addOperationMap (VCS_FAB_CONFIG_SNAPSHOT_COPY_FROM_FTP, reinterpret_cast<PrismMessageHandler> (&VcsFabConfigSnapshotWorker::configSnapshotCopyFromFtpMessageHandler));
}    

VcsFabConfigSnapshotWorker::~VcsFabConfigSnapshotWorker ()
{
}

void VcsFabConfigSnapshotWorker::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_NODES_ADDITION_TO_CLUSTER_COMPLETED_EVENT, reinterpret_cast<PrismEventHandler> (&VcsFabConfigSnapshotWorker::WaveNodesAdditionToClusterCompletedEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

PrismMessage *VcsFabConfigSnapshotWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {   
        case VCS_FAB_CONFIG_SNAPSHOT_CREATE :
        case VCS_FAB_CONFIG_SNAPSHOT_RESTORE :
        case VCS_FAB_CONFIG_SNAPSHOT_DELETE :    
            pPrismMessage = new VcsFabConfigSnapshotMessage(operationCode);
            break;
        case VCS_FAB_CONFIG_SNAPSHOT_COPY_FROM_FTP :
            pPrismMessage = new VcsCopySnapshotFromFtpMessage ();    
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

string VcsFabConfigSnapshotWorker::getConfigSnapshotLocalFilenameSuffix ()
{
    return (s_configSnapshotLocalFilenameSuffix);
}  

string VcsFabConfigSnapshotWorker::getConfigSnapshotGlobalFilenameSuffix ()
{
    return (s_configSnapshotGlobalFilenameSuffix);
}
    
string VcsFabConfigSnapshotWorker::getConfigSnapshotDirectoryname ()
{
    return (s_configSnapshotDirectoryname);
}   

string VcsFabConfigSnapshotWorker::getConfigSnapshotFilenameExtension ()
{
    return (s_configSnapshotFilenameExtension);
}

string VcsFabConfigSnapshotWorker::getConfigSnapshotBackupFilename ()
{
    return (s_configSnapshotBackupFilename);
}   

char VcsFabConfigSnapshotWorker::getConfigSnapshotFilenameSeparator ()
{
    return (s_configSnapshotFilenameTokenSeparator);
}

void VcsFabConfigSnapshotWorker::createSnapshotDirectory ()
{
    string fullDirName = (FrameworkToolKit::getProcessInitialWorkingDirectory()) + "/" + getConfigSnapshotDirectoryname();

    SI32 status = 0;
    status = mkdir(fullDirName.c_str(), 0755);
    
    if (0 != status)
    {
        if (EEXIST != errno)
        {
            WaveNs::trace (TRACE_LEVEL_FATAL, string("VcsFabConfigSnapshotWorker::createSnapshotDirectory : Unable to create snapshot directory - ") + fullDirName + (", with error - ") + string(strerror(errno)));
            WaveNs::prismAssert (false, __FILE__, __LINE__);       
        }
    }
}

void VcsFabConfigSnapshotWorker::parseConfigSnapshotFilenames (const vector<string> &configSnapshotFilenames, map <UI32, set<string> > &rbridgeIdSnapshotIdsMap)
{
    UI32 filenamesSize = configSnapshotFilenames.size();

    for (UI32 i = 0; i < filenamesSize; i++)
    {
        WaveNs::trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::parseConfigSnapshotFilenames : Filename = ") + configSnapshotFilenames[i]);
        // RBID + separator + SSID + separator + Suffix + extension
        //if file is copied from ftp server than file name is RBID + separator + SSID + separator + Suffix + ftp + extension
        // RBID, SSID, Suffix + extension -> form 3 tokens
        string filename, firstPart;
        vector<string> tokens;

        // Split the directory path from name if it exists
        StringUtils::splitBasedOnLastOccuranceOf (configSnapshotFilenames[i], '/', firstPart, filename);

        //tokenize the snapshot file name
        StringUtils::tokenize (filename, tokens, getConfigSnapshotFilenameSeparator());
    
        WaveNs::prismAssert (2 < tokens.size(), __FILE__, __LINE__);

        UI32 rbridgeId = strtoul((tokens[0]).c_str(), NULL, 0);
        string snapshotId = tokens[1];

        rbridgeIdSnapshotIdsMap[rbridgeId].insert(snapshotId) ;
    }
}

void VcsFabConfigSnapshotWorker::WaveNodesAdditionToClusterCompletedEventHandler(const WaveNodesAdditionToClusterCompletedEvent* &pEvent)
{
    // return if mode is not MC
    if (!((VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && (DCM_CCM == DcmToolKit::getDistributionMode ())))
    {
        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
        return;
    }

    trace (TRACE_LEVEL_INFO, string ("VcsFabConfigSnapshotWorker::WaveNodesAdditionToClusterCompletedEventHandler : Entering ..."));    
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    vector<LocationId> newReachableNodes;
   
    UI32 number_of_nodes = pEvent->getNumberOfNewLocations();
 
    //Reset the restore flag if required for new nodes
    // reset flag for all connected node as well as self node
    for(UI32 i=0; i<number_of_nodes+1; i++) 
    {
        UI32 rbridgeId;

        if (i == number_of_nodes)
        {
            // set the rbidgeId of the self node.
            rbridgeId = DcmToolKit::getDomainIdFromLocationId (FrameworkToolKit::getThisLocationId());
        }
        else
        {
            LocationId location = pEvent->getLocationIdAtIndex(i);
            newReachableNodes.push_back (location);
            rbridgeId = DcmToolKit::getDomainIdFromLocationId (location);
        }
       
        string snapshotFilename("");
        getRestoreTriggeredConfigSnapshotFilenameForDomainId(rbridgeId, snapshotFilename);
 
        if (true == snapshotFilename.empty())
        {
            continue;
        } 
        else
        {
            VcsConfigSnapshotObject *pVcsConfigSnapshotObject = new VcsConfigSnapshotObject ();
            status = pVcsConfigSnapshotObject->loadConfiguration (snapshotFilename);

            if (WAVE_MESSAGE_SUCCESS != status)
            {   
                trace (TRACE_LEVEL_ERROR, string ("VcsNodeConfigSnapshotWorker::VcsFabConfigSnapshotWorker::WaveNodesAdditionToClusterCompletedEventHandler : Unable to load configuration present for rbridgeId - ") + rbridgeId + (", filename - ") + snapshotFilename);
            }
            else
            {   
                // Need to reset the restore flag
                    pVcsConfigSnapshotObject->setRestoreOnDisconnectedNode(false);
                    status = pVcsConfigSnapshotObject->saveConfiguration (snapshotFilename);

                    if (WAVE_MESSAGE_SUCCESS != status)
                    {   
                        trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker::WaveNodesAdditionToClusterCompletedEventHandler : Unable to reset the restore flag, for rbridgeId - ") + rbridgeId + (", filename - ") + snapshotFilename);
                    }
                    else
                    {
                        trace (TRACE_LEVEL_INFO, string("VcsFabConfigSnapshotWorker::WaveNodesAdditionToClusterCompletedEventHandler : Successfully reset the restore flag, for rbridgeId - ") + rbridgeId + (", filename - ") + snapshotFilename);
                    }
            }
            delete pVcsConfigSnapshotObject;
        }
    }
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));

    if (0 != newReachableNodes.size())
    {
        vector<string> filenames;
        string snapshotDirPath = (FrameworkToolKit::getProcessInitialWorkingDirectory() + "/" + VcsFabConfigSnapshotWorker::getConfigSnapshotDirectoryname()) + "/*";
        filenames.push_back (snapshotDirPath);
    
        // delete all snapshot files on secondary nodes
        status = FrameworkToolKit::deleteFilesOnAllNodes (filenames, FILE_DEST_PATH_ABSOLUTE | FILE_PUSH_TO_HA_PEER, true, newReachableNodes);
 
        if (WAVE_MESSAGE_SUCCESS != status )
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotDeleteConfigFileFromAllStep : Failed to delete files from all nodes with status - ") + FrameworkToolKit::localize (status));
            status = VCS_CONFIG_SNAPSHOT_SYNC_CONFIG_FILE_FAIL;
        }

        //ToDo : Send only changed files to the existing nodes
        // Send all files to new nodes    
        vector<string> sourceFilenames;
        getAllConfigSnapshotFilenames (sourceFilenames);

        if (0 != sourceFilenames.size())
        {
            vector<string> destFilenames = sourceFilenames;

            // sync all snapshot files of primary nodes to other nodes in the cluster
            status = FrameworkToolKit::pushFilesToAllNodes (sourceFilenames, destFilenames, FILE_OVERWRITE_DEST_FILE_IF_EXIST | FILE_DEST_PATH_ABSOLUTE | FILE_PUSH_TO_HA_PEER, true, newReachableNodes);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker::WaveNodesAdditionToClusterCompletedEventHandler : Unable to sync files"));
            }
        }
    }
}

void VcsFabConfigSnapshotWorker::configSnapshotCopyFromFtpMessageHandler (VcsCopySnapshotFromFtpMessage *pVcsCopySnapshotFromFtpMessage)
{
    trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::configSnapshotCopyFromFtpMessageHandler : Entering ..."));
    VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext = NULL;

    PrismLinearSequencerStep sequencerSteps[] =
    {   
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::configSnapshotCreateValidationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::configSnapshotCopyFtpFilePushToAllStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::prismLinearSequencerFailedStep)
    };
        
    pVcsFabConfigSnapshotContext = new VcsFabConfigSnapshotContext (reinterpret_cast<PrismMessage *> (pVcsCopySnapshotFromFtpMessage),  this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pVcsFabConfigSnapshotContext->setSnapshotId (pVcsCopySnapshotFromFtpMessage->getSnapshotId());
    pVcsFabConfigSnapshotContext->setRbridgeId (pVcsCopySnapshotFromFtpMessage->getRbridgeId());
    pVcsFabConfigSnapshotContext->setSnapshotCreatedTime (pVcsCopySnapshotFromFtpMessage->getSnapshotCreatedTime());
    pVcsFabConfigSnapshotContext->holdAll();
    pVcsFabConfigSnapshotContext->start();
}

void VcsFabConfigSnapshotWorker::configSnapshotCopyFtpFilePushToAllStep (VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext)
{
    trace (TRACE_LEVEL_INFO, string ("VcsFabConfigSnapshotWorker::configSnapshotCopyFtpFilePushToAllStep : Entering ..."));

    VcsCopySnapshotFromFtpMessage *pVcsCopySnapshotFromFtpMessage = reinterpret_cast<VcsCopySnapshotFromFtpMessage*> (pVcsFabConfigSnapshotContext->getPPrismMessage ());
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    //Need to add extra validation on the file being copied
    //1.File metadata need to match the user input
    //2.Restore flag should be reset to false if required
    // Any more cases left out

    string localConfigFileFullPath ("");
    string globalConfigFileFullPath ("");

    getConfigSnapshotCompleteFileNames (pVcsFabConfigSnapshotContext->getSnapshotId(), pVcsFabConfigSnapshotContext->getRbridgeId(), localConfigFileFullPath, globalConfigFileFullPath);    

    //move the local config snapshot file
    string cmdToRename = "/bin/mv " + pVcsCopySnapshotFromFtpMessage->getLocalConfigFilename() + " " + localConfigFileFullPath;
    trace (TRACE_LEVEL_DEVEL, string("VcsFabConfigSnapshotWorker:: configSnapshotCopyFtpFilePushToAllStep : cmdToRename = ") + cmdToRename);

    SI32 ret = 0;
    vector<string> commandOutput;

    ret = FrameworkToolKit::systemCommandOutput (cmdToRename, commandOutput);

    if (0 != ret)
    {   
        status = WAVE_MESSAGE_ERROR;
        trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker:: configSnapshotCopyFtpFilePushToAllStep : Rename of local-file Failed with ret-") + WEXITSTATUS(ret));
        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;
    }
    else
    {
        //move the global config snapshot file
        cmdToRename = "/bin/mv " + pVcsCopySnapshotFromFtpMessage->getGlobalConfigFilename() + " " + globalConfigFileFullPath;
        trace (TRACE_LEVEL_DEVEL, string("VcsFabConfigSnapshotWorker:: configSnapshotCopyFtpFilePushToAllStep : cmdToRename = ") + cmdToRename);
        commandOutput.clear ();

        ret =  FrameworkToolKit::systemCommandOutput (cmdToRename, commandOutput);

        if (0 != ret)
        {
            status = WAVE_MESSAGE_ERROR;
            trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker:: configSnapshotCopyFtpFilePushToAllStep : Rename of global-file Failed with ret-") + WEXITSTATUS(ret));
            pVcsFabConfigSnapshotContext->executeNextStep (status);
            return;
        }
    }

    vector<string> sourceFilenames;
    sourceFilenames.push_back (localConfigFileFullPath);
    sourceFilenames.push_back (globalConfigFileFullPath);

    vector<string> destFilenames = sourceFilenames;

    status = FrameworkToolKit::pushFilesToAllNodes (sourceFilenames, destFilenames, FILE_OVERWRITE_DEST_FILE_IF_EXIST | FILE_DEST_PATH_ABSOLUTE | FILE_PUSH_TO_HA_PEER);

    if (WAVE_MESSAGE_SUCCESS != status )
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotCopyFtpFilePushToAllStep : Failed to push files to all nodes with status - ") + FrameworkToolKit::localize (status));
        status = VCS_CONFIG_SNAPSHOT_SYNC_CONFIG_FILE_FAIL;
    } 

    pVcsFabConfigSnapshotContext->executeNextStep (status);
}

void VcsFabConfigSnapshotWorker::configSnapshotCreateMessageHandler (VcsFabConfigSnapshotMessage *pVcsFabConfigSnapshotMessage)
{
    trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::configSnapshotCreateMessageHandler : Entering ..."));   
    VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext = NULL;

    PrismLinearSequencerStep sequencerSteps[] =
    {  
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::configSnapshotCreateValidationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::configSnapshotCreateConfigFilePushToAllStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::prismLinearSequencerFailedStep)    
    };

    pVcsFabConfigSnapshotContext = new VcsFabConfigSnapshotContext (reinterpret_cast<PrismMessage *> (pVcsFabConfigSnapshotMessage),  this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));    
    pVcsFabConfigSnapshotContext->setSnapshotId (pVcsFabConfigSnapshotMessage->getSnapshotId());
    pVcsFabConfigSnapshotContext->setRbridgeId (pVcsFabConfigSnapshotMessage->getRbridgeId());
    pVcsFabConfigSnapshotContext->setSnapshotCreatedTime (pVcsFabConfigSnapshotMessage->getSnapshotCreatedTime());
    pVcsFabConfigSnapshotContext->holdAll();
    pVcsFabConfigSnapshotContext->start();
}

void VcsFabConfigSnapshotWorker::configSnapshotCreateValidationStep (VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext)
{
    trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::configSnapshotCreateValidationStep : Entering ..."));
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    LocationRole thisNodeRole = FrameworkToolKit::getThisLocationRole();

    if ((LOCATION_PRIMARY != thisNodeRole) && (LOCATION_STAND_ALONE != thisNodeRole))
    {   
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotCreateValidationStep : Cannot Create Snapshot since this node is yet to become Coordinator, current role - ") + FrameworkToolKit::localize (thisNodeRole));
        status = VCS_CLUSTER_PRIMARY_ELECTION_IN_PROGRESS;   
        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;
    }

    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = DcmToolKit::getLocalManagedObjectFromDomainId (pVcsFabConfigSnapshotContext->getRbridgeId());

    LocationId locationOfInputDomain = 0;
    ResourceId specificStatusOfNode = 0;
    SI32 swbdModel = -1;

    if (NULL == pVcsNodeLocalManagedObject)
    {
        trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker::configSnapshotCreateValidationStep: Node with RbridgeId - ") + pVcsFabConfigSnapshotContext->getRbridgeId() + (" is not part of cluster"));
        status = VCS_CLUSTER_NODE_NOT_PRESENT;
        pVcsFabConfigSnapshotContext->executeNextStep (status);    
        return;
    }
    else
    {
        locationOfInputDomain = pVcsNodeLocalManagedObject->getOwnerLocationId ();
        specificStatusOfNode = pVcsNodeLocalManagedObject->getSpecificStatus ();
        swbdModel = pVcsNodeLocalManagedObject->getSWBDModel();
        delete (pVcsNodeLocalManagedObject);
    }

    
    if ((VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY != specificStatusOfNode) && (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_DISCONNECTED_FROM_CLUSTER != specificStatusOfNode) && (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_STAND_ALONE != specificStatusOfNode) && (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_PRIMARY != specificStatusOfNode))
    {   
        status = VCS_CLUSTER_NODE_IN_TRANSITION;
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotCreateValidationStep : RbridgeId - ") + pVcsFabConfigSnapshotContext->getRbridgeId() + (", is in transition with VcsNodeLocalMOSpecificStatus -") + FrameworkToolKit::localizeToSourceCodeEnum(specificStatusOfNode));
        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;
    }
    
    pVcsFabConfigSnapshotContext->setNodeLocationId(locationOfInputDomain);
    pVcsFabConfigSnapshotContext->setSWBDModel (swbdModel);        
   
    UI32            countOfSnapshotsForSelectedRbridgeId = 0;
    vector<string>  localFileNames;
    vector<string>  globalFileNames;
    status = VcsFabConfigSnapshotWorker::checkIfSnapshotFilesPresent (pVcsFabConfigSnapshotContext->getSnapshotId(), pVcsFabConfigSnapshotContext->getRbridgeId(), countOfSnapshotsForSelectedRbridgeId, localFileNames, globalFileNames);

    if (s_maxSnapshotsPerRbridgeId <= (countOfSnapshotsForSelectedRbridgeId/2)) //Since for each snapshotId, there are 2 files (global + local)
    {
        trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker::configSnapshotCreateValidationStep: Max allowed snapshots reached, No of existing snapshots - ") + countOfSnapshotsForSelectedRbridgeId/2);
        status = VCS_CONFIG_SNAPSHOT_MAX_LIMIT_REACHED;
        pVcsFabConfigSnapshotContext->executeNextStep (status);    
        return;
    }
    else if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker::configSnapshotCreateValidationStep: Snapshot already present with same SnapshotID - ") + pVcsFabConfigSnapshotContext->getSnapshotId() + (", for RbridgeId - ") + pVcsFabConfigSnapshotContext->getRbridgeId());
        status = VCS_CONFIG_SNAPSHOT_ID_PRESENT;
        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;
    } 

    trace (TRACE_LEVEL_DEVEL, string("VcsFabConfigSnapshotWorker::configSnapshotCreateValidationStep: Snapshot will be created with SnapshotID - ") + pVcsFabConfigSnapshotContext->getSnapshotId() + (", for RbridgeId - ") + pVcsFabConfigSnapshotContext->getRbridgeId());

    pVcsFabConfigSnapshotContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsFabConfigSnapshotWorker::configSnapshotCreateConfigFilePushToAllStep (VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext)
{
    trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::configSnapshotCreateConfigFilePushToAllStep : Entering ..."));

    string localConfigFileFullPath ("");
    string globalConfigFileFullPath ("");

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    getConfigSnapshotCompleteFileNames (pVcsFabConfigSnapshotContext->getSnapshotId(), pVcsFabConfigSnapshotContext->getRbridgeId(), localConfigFileFullPath, globalConfigFileFullPath);    
    
    string configData;
    DateTime configCreatedTime;
    bool isLocalOnlyConfig = true;

    //retrieve node specific local-only config
    status = retrieveNodeSpecificConfig (pVcsFabConfigSnapshotContext->getRbridgeId(), configData, configCreatedTime, isLocalOnlyConfig);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotCreateConfigFilePushToAllStep : Unable to retrieve the config from running-config"));
        status = VCS_CONFIG_SNAPSHOT_FETCH_LOCAL_CONFIG_FAIL;
        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;
    }
    else if (true == configData.empty())
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotCreateConfigFilePushToAllStep : Local-config is empty"));
        status = VCS_CONFIG_SNAPSHOT_LOCAL_CONFIG_EMPTY;
        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;
    }

    VcsConfigSnapshotObject *pVcsConfigSnapshotObject = new VcsConfigSnapshotObject();
    prismAssert (NULL != pVcsConfigSnapshotObject, __FILE__, __LINE__);

    pVcsConfigSnapshotObject->setRbridgeId (pVcsFabConfigSnapshotContext->getRbridgeId());
    pVcsConfigSnapshotObject->setSnapshotId (pVcsFabConfigSnapshotContext->getSnapshotId());
    pVcsConfigSnapshotObject->setSWBDModel (pVcsFabConfigSnapshotContext->getSWBDModel());
    pVcsConfigSnapshotObject->setSnapshotCreatedTime (configCreatedTime);
    pVcsConfigSnapshotObject->setRestoreOnDisconnectedNode (false);
    pVcsConfigSnapshotObject->setConfigData (configData);
    
    status = pVcsConfigSnapshotObject->saveConfiguration (localConfigFileFullPath);

    configData.clear();
    
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        status = VCS_CONFIG_SNAPSHOT_CREATE_LOCAL_CONFIG_FILE_FAIL;
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotCreateConfigFilePushToAllStep : Failure to save LocalConfigFile - ") + localConfigFileFullPath);    

        delete pVcsConfigSnapshotObject;
        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;        
    }
    else
    {
        //Retrieve node specific global-only config 

        isLocalOnlyConfig = false; //Means global config

        status = retrieveNodeSpecificConfig (pVcsFabConfigSnapshotContext->getRbridgeId(), configData, configCreatedTime, isLocalOnlyConfig);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotCreateConfigFilePushToAllStep : Unable to retrieve the global config from running-config"));
            status = VCS_CONFIG_SNAPSHOT_FETCH_GLOBAL_CONFIG_FAIL;

            delete pVcsConfigSnapshotObject;
            pVcsFabConfigSnapshotContext->executeNextStep (status);
            return;
        }
        else if (true == configData.empty())
        {
            //Ideally we cannot have global config as empty as atleast VLAN 1 will be there by default. Hence when global-config is empty remove the local snapshot file created and error.
            trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotCreateConfigFilePushToAllStep : global-config is empty"));
            status = VCS_CONFIG_SNAPSHOT_GLOBAL_CONFIG_EMPTY;

            ConfigFileManagementToolKit::deleteConfigurationFile (localConfigFileFullPath);

            delete pVcsConfigSnapshotObject;
            pVcsFabConfigSnapshotContext->executeNextStep (status);
            return;
        }

    }

    pVcsConfigSnapshotObject->setConfigData (configData);

    status = pVcsConfigSnapshotObject->saveConfiguration (globalConfigFileFullPath);

    configData.clear();
    delete pVcsConfigSnapshotObject;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        status = VCS_CONFIG_SNAPSHOT_CREATE_GLOBAL_CONFIG_FILE_FAIL;
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotCreateConfigFilePushToAllStep : Failure to save globalConfigFile - ") + globalConfigFileFullPath);

        //Since creating global configuration file failed, remove the local configuration file that was created earlier
        ConfigFileManagementToolKit::deleteConfigurationFile (localConfigFileFullPath);

        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;
    }

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        vector<string> sourceFilenames;
        sourceFilenames.push_back (localConfigFileFullPath);
        sourceFilenames.push_back (globalConfigFileFullPath);

        vector<string> destFilenames = sourceFilenames;

        status = FrameworkToolKit::pushFilesToAllNodes (sourceFilenames, destFilenames, FILE_OVERWRITE_DEST_FILE_IF_EXIST | FILE_DEST_PATH_ABSOLUTE | FILE_PUSH_TO_HA_PEER);

        if (WAVE_MESSAGE_SUCCESS != status )
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotCreateConfigFilePushToAllStep : Failed to push files to all nodes with status - ") + FrameworkToolKit::localize (status));
            status = VCS_CONFIG_SNAPSHOT_SYNC_CONFIG_FILE_FAIL;
        } 
    }
    
    pVcsFabConfigSnapshotContext->executeNextStep (status);
}

void VcsFabConfigSnapshotWorker::configSnapshotDeleteMessageHandler (VcsFabConfigSnapshotMessage *pVcsFabConfigSnapshotMessage)
{
    trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::configSnapshotDeleteMessageHandler : Entering ..."));
    VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext = NULL;

    PrismLinearSequencerStep sequencerSteps[] =
    {   
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::configSnapshotDeleteValidationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::configSnapshotDeleteConfigFileFromAllStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::prismLinearSequencerFailedStep)
    };

    pVcsFabConfigSnapshotContext = new VcsFabConfigSnapshotContext (reinterpret_cast<PrismMessage *> (pVcsFabConfigSnapshotMessage),  this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pVcsFabConfigSnapshotContext->setRbridgeId(pVcsFabConfigSnapshotMessage->getRbridgeId());
    pVcsFabConfigSnapshotContext->setSnapshotId(pVcsFabConfigSnapshotMessage->getSnapshotId());
    pVcsFabConfigSnapshotContext->holdAll();
    pVcsFabConfigSnapshotContext->start();

}

void VcsFabConfigSnapshotWorker::configSnapshotDeleteValidationStep (VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext)
{
    trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::configSnapshotDeleteValidationStep : Entering ..."));

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    LocationRole thisNodeRole = FrameworkToolKit::getThisLocationRole();

    if ((LOCATION_PRIMARY != thisNodeRole) && (LOCATION_STAND_ALONE != thisNodeRole))
    {   
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotDeleteValidationStep : Cannot delete snapshot since this node is yet to become Coordinator, current role-") + FrameworkToolKit::localize(thisNodeRole));
        status = VCS_CLUSTER_PRIMARY_ELECTION_IN_PROGRESS;
        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;
    }

    UI32            countOfSnapshotsForSelectedRbridgeId = 0;
    vector<string>  localFileNames;
    vector<string>  globalFileNames;
    status = VcsFabConfigSnapshotWorker::checkIfSnapshotFilesPresent (pVcsFabConfigSnapshotContext->getSnapshotId(), pVcsFabConfigSnapshotContext->getRbridgeId(), countOfSnapshotsForSelectedRbridgeId, localFileNames, globalFileNames);

    //Need to check if restore has been triggered for the same snapshotID. Handle accordingly
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotDeleteValidationStep : SnapshotId -") + pVcsFabConfigSnapshotContext->getSnapshotId() + (", not present for rbridgeId - ") + pVcsFabConfigSnapshotContext->getRbridgeId());
        status = VCS_CONFIG_SNAPSHOT_ID_NOT_PRESENT;
        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;
    }

    pVcsFabConfigSnapshotContext->executeNextStep (status);
}

void VcsFabConfigSnapshotWorker::configSnapshotDeleteConfigFileFromAllStep (VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext)
{
    trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::configSnapshotDeleteConfigFileFromAllStep : Entering ..."));
        
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    string localConfigFileFullPath ("");
    string globalConfigFileFullPath ("");

    getConfigSnapshotCompleteFileNames (pVcsFabConfigSnapshotContext->getSnapshotId(), pVcsFabConfigSnapshotContext->getRbridgeId(), localConfigFileFullPath, globalConfigFileFullPath);

    vector<string> filenames;
    filenames.push_back (localConfigFileFullPath);
    filenames.push_back (globalConfigFileFullPath);

    status = FrameworkToolKit::deleteFilesOnAllNodes (filenames, FILE_DEST_PATH_ABSOLUTE | FILE_PUSH_TO_HA_PEER, true);

    if (WAVE_MESSAGE_SUCCESS != status )
    {   
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotDeleteConfigFileFromAllStep : Failed to delete files from all nodes with status - ") + FrameworkToolKit::localize (status)+ (", rbridgeId-") + pVcsFabConfigSnapshotContext->getRbridgeId() + (", snapshotId -") + pVcsFabConfigSnapshotContext->getSnapshotId());
        status = VCS_CONFIG_SNAPSHOT_SYNC_CONFIG_FILE_FAIL;
    }
    else
    {
        UI32 ret = ConfigFileManagementToolKit::deleteConfigurationFile (localConfigFileFullPath); 

        if (WAVE_MESSAGE_SUCCESS != ret)
        {
            status = VCS_CONFIG_SNAPSHOT_SYNC_CONFIG_FILE_FAIL;
            trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker::configSnapshotDeleteConfigFileFromAllStep : Delete Failed on local node"));
        }
        else
        {
            ret = ConfigFileManagementToolKit::deleteConfigurationFile (globalConfigFileFullPath);

            if (WAVE_MESSAGE_SUCCESS != ret)
            {
                status = VCS_CONFIG_SNAPSHOT_SYNC_CONFIG_FILE_FAIL;
                trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker::configSnapshotDeleteConfigFileFromAllStep : Delete of global snapshot failed on local node"));
            }

        }
    }

    pVcsFabConfigSnapshotContext->executeNextStep (status);
}

void VcsFabConfigSnapshotWorker::configSnapshotRestoreMessageHandler (VcsFabConfigSnapshotMessage *pVcsFabConfigSnapshotMessage)
{
    trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreMessageHandler : Entering ..."));
    VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext = NULL;

    PrismLinearSequencerStep sequencerSteps[] =
    {   
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::configSnapshotRestoreSendMessageToNodeStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::configSnapshotRestoreCleanupNodeConfig),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigSnapshotWorker::prismLinearSequencerFailedStep)
    };

    pVcsFabConfigSnapshotContext = new VcsFabConfigSnapshotContext (reinterpret_cast<PrismMessage *> (pVcsFabConfigSnapshotMessage),  this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pVcsFabConfigSnapshotContext->setRbridgeId(pVcsFabConfigSnapshotMessage->getRbridgeId());
    pVcsFabConfigSnapshotContext->setSnapshotId(pVcsFabConfigSnapshotMessage->getSnapshotId());
    pVcsFabConfigSnapshotContext->holdAll();
    pVcsFabConfigSnapshotContext->start();
}

void VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep (VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext)
{
    trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep : Entering ..."));

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    LocationRole thisNodeRole = FrameworkToolKit::getThisLocationRole();
    
    if ((LOCATION_PRIMARY != thisNodeRole) && (LOCATION_STAND_ALONE != thisNodeRole))
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep : Cannot restore snapshot since this node is yet to become Coordinator, current role-") + FrameworkToolKit::localize(thisNodeRole));
        status = VCS_CLUSTER_PRIMARY_ELECTION_IN_PROGRESS;
        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;
    }

    LocationId locationOfInputDomain = 0;
    ResourceId specificStatusOfNode = 0;
    SI32 nodeSWBD = -1;

    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = DcmToolKit::getLocalManagedObjectFromDomainId (pVcsFabConfigSnapshotContext->getRbridgeId());

    if (NULL == pVcsNodeLocalManagedObject)
    {   
        trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep: Node with RbridgeId - ") + pVcsFabConfigSnapshotContext->getRbridgeId() + (" is not part of cluster"));
        status = VCS_CLUSTER_NODE_NOT_PRESENT;
        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;
    }
    else
    {
        locationOfInputDomain = pVcsNodeLocalManagedObject->getOwnerLocationId ();
        specificStatusOfNode = pVcsNodeLocalManagedObject->getSpecificStatus ();
        nodeSWBD = pVcsNodeLocalManagedObject->getSWBDModel();
        delete (pVcsNodeLocalManagedObject);
    }
    
    pVcsFabConfigSnapshotContext->setNodeLocationId(locationOfInputDomain);
    pVcsFabConfigSnapshotContext->setSWBDModel(nodeSWBD);

    string snapshotFilename("");
    getRestoreTriggeredConfigSnapshotFilenameForDomainId(pVcsFabConfigSnapshotContext->getRbridgeId(), snapshotFilename);

    if (false == snapshotFilename.empty())
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep : Restore is already set for domainId = ") + pVcsFabConfigSnapshotContext->getRbridgeId() + (", filename = ") + snapshotFilename);
        status = VCS_CONFIG_SNAPSHOT_MULTI_RESTORE_REQUEST;
        pVcsFabConfigSnapshotContext->executeNextStep (status);
        return;
    }

    trace (TRACE_LEVEL_INFO, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep : Restoring Node's role - ") + FrameworkToolKit::localizeToSourceCodeEnum(specificStatusOfNode));

    if ((VCS_FABRIC_LOCAL_SPECIFIC_STATUS_STAND_ALONE == specificStatusOfNode)
            ||(VCS_FABRIC_LOCAL_SPECIFIC_STATUS_PRIMARY == specificStatusOfNode))
    {
        pVcsFabConfigSnapshotContext->setIsNodePrimary(true);
    }
    else if (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY == specificStatusOfNode)
    {
        pVcsFabConfigSnapshotContext->setIsNodePrimary(false);
    }
    else if (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_DISCONNECTED_FROM_CLUSTER == specificStatusOfNode)
    {
        status = VCS_CLUSTER_NODE_NOT_REACHABLE;
        pVcsFabConfigSnapshotContext->setIsNodePrimary(false);
    }
    else
    {
        status = VCS_CLUSTER_NODE_IN_TRANSITION;
    }

    if (WAVE_MESSAGE_SUCCESS == status || VCS_CLUSTER_NODE_NOT_REACHABLE == status)
    {
        UI32            countOfSnapshotsForSelectedRbridgeId = 0;
        vector<string>  localFileNames;
        vector<string>  globalFileNames;
        ResourceId fileStatus = VcsFabConfigSnapshotWorker::checkIfSnapshotFilesPresent (pVcsFabConfigSnapshotContext->getSnapshotId(), pVcsFabConfigSnapshotContext->getRbridgeId(), countOfSnapshotsForSelectedRbridgeId, localFileNames, globalFileNames);

        if (WAVE_MESSAGE_SUCCESS != fileStatus)
        {   
            trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep : SnapshotId -") + pVcsFabConfigSnapshotContext->getSnapshotId() + (", not present for rbridgeId - ") + pVcsFabConfigSnapshotContext->getRbridgeId());
            status = VCS_CONFIG_SNAPSHOT_ID_NOT_PRESENT;
            pVcsFabConfigSnapshotContext->executeNextStep (status);
            return;
        }
        else
        {
            string localConfigFileFullPath ("");
            string globalConfigFileFullPath ("");

            getConfigSnapshotCompleteFileNames (pVcsFabConfigSnapshotContext->getSnapshotId(), pVcsFabConfigSnapshotContext->getRbridgeId(), localConfigFileFullPath, globalConfigFileFullPath);

            VcsConfigSnapshotObject *pVcsConfigSnapshotObject = new VcsConfigSnapshotObject();
            prismAssert (NULL != pVcsConfigSnapshotObject, __FILE__, __LINE__);

            fileStatus = pVcsConfigSnapshotObject->loadConfiguration (localConfigFileFullPath);

            if (WAVE_MESSAGE_SUCCESS != fileStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep : SnapshotId -") + pVcsFabConfigSnapshotContext->getSnapshotId() + (", Unable to load configuration present for rbridgeId - ") + pVcsFabConfigSnapshotContext->getRbridgeId());
                //Need to add proper error message if validation of snapshot file has failed
                status = VCS_CONFIG_SNAPSHOT_OPEN_LOCAL_CONFIG_FILE_FAIL;
                delete pVcsConfigSnapshotObject;
                pVcsFabConfigSnapshotContext->executeNextStep (status);
                return;
            }
            
            SI32 snapshotSWBD = pVcsConfigSnapshotObject->getSWBDModel();
 
            //check for swbd type
            if (snapshotSWBD != nodeSWBD)
            {
                trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep : SWBD mismatch, nodeSWBD-") + nodeSWBD + (", snapshotSWBD-") + snapshotSWBD + (", snapshot_timestamp-") + (pVcsConfigSnapshotObject->getSnapshotCreatedTime()).toString());
                status = VCS_CONFIG_SNAPSHOT_MODEL_MATCH_FAIL;
            }
            else 
            {
                // This flag allows snapshot files not to be deleted during controlled failover
                // for the restoring node. Once bidirectional sync is complete, set this flag only
                // for a disconnected node. Need to handle resetting of this flag in all cases        
                pVcsConfigSnapshotObject->setRestoreOnDisconnectedNode (true);

                if(WAVE_MESSAGE_SUCCESS != pVcsConfigSnapshotObject->saveConfiguration (localConfigFileFullPath))
                {   
                    trace(TRACE_LEVEL_ERROR, "VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep : Failed to save Configuration" + FrameworkToolKit::localize (status));
                    
                    status = VCS_CONFIG_SNAPSHOT_CREATE_LOCAL_CONFIG_FILE_FAIL;
                }
                else
                {
                    fileStatus = pVcsConfigSnapshotObject->loadConfiguration (globalConfigFileFullPath);

                    if (WAVE_MESSAGE_SUCCESS != fileStatus)
                    {
                        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep : SnapshotId -") + pVcsFabConfigSnapshotContext->getSnapshotId () + (", Unable to load global configuration present for rbridgeId - ") + pVcsFabConfigSnapshotContext->getRbridgeId());
                        status = VCS_CONFIG_SNAPSHOT_OPEN_GLOBAL_CONFIG_FILE_FAIL;

                        delete pVcsConfigSnapshotObject;
                        pVcsFabConfigSnapshotContext->executeNextStep (status);
                        return;
                    }

                    if(WAVE_MESSAGE_SUCCESS != pVcsConfigSnapshotObject->saveConfiguration (globalConfigFileFullPath))
                    {
                        trace(TRACE_LEVEL_ERROR, "VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep : Failed to save Configuration" + FrameworkToolKit::localize (status));
                        status = VCS_CONFIG_SNAPSHOT_CREATE_GLOBAL_CONFIG_FILE_FAIL;
                    }

                    trace(TRACE_LEVEL_INFO, "VcsFabConfigSnapshotWorker::configSnapshotRestoreValidationStep : Node disconnected, so persisted flag in ConfigFile");
                    if (VCS_CLUSTER_NODE_NOT_REACHABLE == status)
                    {
                        status = WAVE_MESSAGE_SUCCESS;
                        pVcsFabConfigSnapshotContext->setRestoreOnDisconnectedNode (true);
                    }
                }  

            }
            delete pVcsConfigSnapshotObject;
        }
    }

    pVcsFabConfigSnapshotContext->executeNextStep (status);    
}

void VcsFabConfigSnapshotWorker::configSnapshotRestoreSendMessageToNodeStep (VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;    
    VcsFabConfigSnapshotMessage *pVcsFabConfigSnapshotMessage = reinterpret_cast<VcsFabConfigSnapshotMessage*> (pVcsFabConfigSnapshotContext->getPPrismMessage ());

    //we need to sync files across all nodes

    string localConfigFileFullPath ("");
    string globalConfigFileFullPath ("");

    getConfigSnapshotCompleteFileNames (pVcsFabConfigSnapshotContext->getSnapshotId(), pVcsFabConfigSnapshotContext->getRbridgeId(), localConfigFileFullPath, globalConfigFileFullPath);

    vector<string> sourceFilenames;
    sourceFilenames.push_back (localConfigFileFullPath);
    sourceFilenames.push_back (globalConfigFileFullPath);

    vector<string> destFilenames = sourceFilenames;

    status = FrameworkToolKit::pushFilesToAllNodes (sourceFilenames, destFilenames, FILE_OVERWRITE_DEST_FILE_IF_EXIST | FILE_DEST_PATH_ABSOLUTE | FILE_PUSH_TO_HA_PEER);

    if (WAVE_MESSAGE_SUCCESS != status )
    {   
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreSendMessageToNodeStep : Failed to push files to all nodes with status - ") + FrameworkToolKit::localize (status));
        status = VCS_CONFIG_SNAPSHOT_SYNC_CONFIG_FILE_FAIL;
    }        
    else if (false == pVcsFabConfigSnapshotContext->getRestoreOnDisconnectedNode())
    {
        //Send message if node is connected
        VcsNodeConfigSnapshotMessage *pVcsNodeConfigSnapshotMessage = new VcsNodeConfigSnapshotMessage (VCS_NODE_CONFIG_SNAPSHOT_RESTORE);
        pVcsNodeConfigSnapshotMessage->setIsNodePrimary(pVcsFabConfigSnapshotContext->getIsNodePrimary());
        pVcsNodeConfigSnapshotMessage->setSnapshotId (pVcsFabConfigSnapshotContext->getSnapshotId());
        pVcsNodeConfigSnapshotMessage->setRbridgeId (pVcsFabConfigSnapshotContext->getRbridgeId());
        
        status = sendSynchronously (pVcsNodeConfigSnapshotMessage, pVcsFabConfigSnapshotContext->getNodeLocationId());
    
        if (WAVE_MESSAGE_SUCCESS != status)
        {   
            trace (TRACE_LEVEL_ERROR, "VcsFabConfigSnapshotWorker::configSnapshotRestoreSendMessageToNodeStep : Error in sending message with Status-\"" + FrameworkToolKit::localize (status) + "\"");
        }
        else
        {   
            status = pVcsNodeConfigSnapshotMessage->getCompletionStatus ();

            //Even if the remote done is disconnected we should still go ahead with the restore operation by going ahead with the next step.
            if ((WAVE_MESSAGE_SUCCESS != status))
            {   
                trace (TRACE_LEVEL_ERROR, "VcsFabConfigSnapshotWorker::configSnapshotRestoreSendMessageToNodeStep : Erron in Message-CompletionStatus-\"" + FrameworkToolKit::localize (status) + "\"");
            }
            else
            {
                status = WAVE_MESSAGE_SUCCESS;
            }
        }

        switch (status)
        {
            case WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE:
            case WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION:
            case WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE_DUE_TO_PRINCIPAL_FAILOVER:
                pVcsFabConfigSnapshotContext->setRestoreOnDisconnectedNode (true);
                //Even if the remote node is disconnected we should still go ahead with the restore operation by going ahead with the next step.
                status = WAVE_MESSAGE_SUCCESS;
                break;
        }


        delete (pVcsNodeConfigSnapshotMessage);
    }
   
    pVcsFabConfigSnapshotContext->setPPrismMessage (NULL);
    
    //if the restore is on a disconnected node send VCS_CLUSTER_NODE_NOT_REACHABLE as the completion status so that we will print appropriate message to the front end.
    if (pVcsFabConfigSnapshotContext->getRestoreOnDisconnectedNode())
    {
        pVcsFabConfigSnapshotMessage->setCompletionStatus (VCS_CLUSTER_NODE_NOT_REACHABLE);   
    }
    else
    {
        pVcsFabConfigSnapshotMessage->setCompletionStatus (status);
    }

    reply (pVcsFabConfigSnapshotMessage); 

    pVcsFabConfigSnapshotContext->executeNextStep (status);
}

void VcsFabConfigSnapshotWorker::configSnapshotRestoreCleanupNodeConfig (VcsFabConfigSnapshotContext *pVcsFabConfigSnapshotContext)
{
    if (true == pVcsFabConfigSnapshotContext->getIsNodePrimary())
    {
        //IF restore is on primary i.e local node, then unconfigure will be done by local service
        pVcsFabConfigSnapshotContext->executeNextStep (WAVE_MESSAGE_SUCCESS);                
        return;
    }
    
    trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreCleanupNodeConfig : Entering ..."));

    VcsFabObjectManager* pVcsFabObjectManager = VcsFabObjectManager::getInstance();
    UI32 domainId = pVcsFabConfigSnapshotContext->getRbridgeId();

    //Instantiate the remove switch message 
    ClusterObjectManagerDeleteNodeMessage *pClusterObjectManagerDeleteNodeMessage = new  ClusterObjectManagerDeleteNodeMessage ();

    //Set the configuration change flag to true so that if the node is disconnected we do not allow the node to add back without going to default config
    //Else the node for which we performed a controlled failover will be able to come back and join the cluster if the principal went down and some other disconnected node 
    //becomes the principal
    pClusterObjectManagerDeleteNodeMessage->setIsConfigurationChange (true);

    VcsNodeLocalManagedObject* pVcsNodeLocalManagedObject = pVcsFabObjectManager->getSwitchObjectFromDomainId(domainId);

    prismAssert(NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);

    UI32 switchPort = pVcsNodeLocalManagedObject->getSwitchPort ();
    vector<string> ipAddressess;

    if (false == DcmToolKit::isIpotEnabled ())
    {
        ipAddressess = pVcsNodeLocalManagedObject->getPublicIpAddress ();
           
        trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreCleanupNodeConfig : Ipot is disabled, pubIpAddr = ")+ ipAddressess[0]+" switchPort = "+switchPort);
    }
    else
    {
        ipAddressess.push_back(DcmToolKit::getIpAddressFromDomainId (domainId));
        trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreCleanupNodeConfig : Ipot is enabled, switchIpAddr = ")+ipAddressess[0]+" switchPort = "+switchPort);
    }

    pClusterObjectManagerDeleteNodeMessage->setSecondaryNodeNameAndPort (ipAddressess[0], switchPort);
    pClusterObjectManagerDeleteNodeMessage->setNNodesToDelete(1);

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    status = sendOneWay (pClusterObjectManagerDeleteNodeMessage); 

    // send RASLOGs for deleted switches
    vector<string>  principalPublicIpAddress    = (VcsFabricLocalObjectManager::getInstance())->getThisVcsNodeLocalManagedObjectPublicIpAddress ();
    UI32            vcsId                       = (VcsFabricLocalObjectManager::getInstance())->getThisVcsNodeLocalManagedObjectVcsId ();

    if(status != WAVE_MESSAGE_SUCCESS) 
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::configSnapshotRestoreCleanupNodeConfig : Failed to send delete-node-message to the Cluster, status-") + FrameworkToolKit::localize (status));
        if ((VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && (DCM_CCM == DcmToolKit::getDistributionMode ()))
        {
            raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, VCS_1008, principalPublicIpAddress[0].c_str (), vcsId, domainId, ipAddressess[0].c_str (), FrameworkToolKit::localize (status).c_str ());
        }
    }
    else
    {
        if ((VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && (DCM_CCM == DcmToolKit::getDistributionMode ()))
        {
            // Commenting out below (duplicate entry on console)
            //raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, VCS_1007, principalPublicIpAddress[0].c_str (), vcsId, domainId, ipAddressess[0].c_str ());
        }
    }

    delete (pVcsNodeLocalManagedObject);
    pVcsFabConfigSnapshotContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsFabConfigSnapshotWorker::getConfigSnapshotCompleteFileNames (const string &snapshotID, const UI32 &rbridgeId, string &localFilename, string &globalFilename)
{
    string      separator;
    string      prefix;
    const char *snapshot = snapshotID.c_str();
    separator = getConfigSnapshotFilenameSeparator();

    if (0 == strncmp(snapshot, "all", strlen("all")))
    {
         prefix = rbridgeId + separator + "*";
    }
    else
    {
         prefix = rbridgeId + separator + snapshotID + separator;
    }
    
    localFilename = (FrameworkToolKit::getProcessInitialWorkingDirectory() + "/" + VcsFabConfigSnapshotWorker::getConfigSnapshotDirectoryname() + "/" + prefix + VcsFabConfigSnapshotWorker::getConfigSnapshotLocalFilenameSuffix() + VcsFabConfigSnapshotWorker::getConfigSnapshotFilenameExtension());
    globalFilename = (FrameworkToolKit::getProcessInitialWorkingDirectory() + "/" + VcsFabConfigSnapshotWorker::getConfigSnapshotDirectoryname() + "/" + prefix + VcsFabConfigSnapshotWorker::getConfigSnapshotGlobalFilenameSuffix() + VcsFabConfigSnapshotWorker::getConfigSnapshotFilenameExtension());

}

void VcsFabConfigSnapshotWorker::getConfigSnapshotFileNames (const string &snapshotID, const UI32 &rbridgeId, string &localFilename, string &globalFilename)
{
    string separator;
    separator = getConfigSnapshotFilenameSeparator();
    string prefix = rbridgeId + separator + snapshotID + separator;
    
    localFilename = prefix + VcsFabConfigSnapshotWorker::getConfigSnapshotLocalFilenameSuffix() + VcsFabConfigSnapshotWorker::getConfigSnapshotFilenameExtension();
    globalFilename = prefix + VcsFabConfigSnapshotWorker::getConfigSnapshotGlobalFilenameSuffix() + VcsFabConfigSnapshotWorker::getConfigSnapshotFilenameExtension();
}

void VcsFabConfigSnapshotWorker::deleteAllConfigSnapshotFiles (const bool &includeBackupFile)
{
    string snapshotDirPath = FrameworkToolKit::getProcessInitialWorkingDirectory() + "/" + VcsFabConfigSnapshotWorker::getConfigSnapshotDirectoryname();
    string cmdToDelete;

    if (true == includeBackupFile)
    {
        cmdToDelete = "/bin/rm -rf " + snapshotDirPath + "/*";
    }
    else
    {
        cmdToDelete = "/bin/rm -rf " + snapshotDirPath + "/*" + getConfigSnapshotFilenameExtension();
    }

    SI32 ret = 0;
    vector<string> commandOutput;
        
    WaveNs::trace (TRACE_LEVEL_DEVEL, string("VcsFabConfigSnapshotWorker::deleteAllConfigSnapshotFiles : cmdToDelete \"") + cmdToDelete + ("\""));
    ret = FrameworkToolKit::systemCommandOutput (cmdToDelete, commandOutput);    

    if (0 != ret)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker:: deleteAllConfigSnapshotFiles : Failed with ret-") + WEXITSTATUS(ret));
    }    
}   

void VcsFabConfigSnapshotWorker::getAllConfigSnapshotFilenames (vector<string> &configSnapshotFilenames, const bool &completePath)
{
    vector<string> allConfigSnapshotFilenames;
    vector<string> tokens;
    UI32 i = 0;

    string snapshotDirPath = (FrameworkToolKit::getProcessInitialWorkingDirectory() + "/" + VcsFabConfigSnapshotWorker::getConfigSnapshotDirectoryname());
    FrameworkToolKit::getAllFilesInTheDirectory (snapshotDirPath, allConfigSnapshotFilenames, getConfigSnapshotFilenameExtension());

    for (i = 0; i < allConfigSnapshotFilenames.size(); i++)
    {
        tokens.clear();

        /*
            Dont include the files which have "-ftp-" keyword.
            Structure of file copied from ftp server : rbridgeId-snapshotId-local-ftp.cs
            Tokenizing this string will return 4 tokens, so dont include this file
        */
        StringUtils::tokenize (allConfigSnapshotFilenames[i],tokens, getConfigSnapshotFilenameSeparator());

        if (3 == tokens.size())
        {
            configSnapshotFilenames.push_back (allConfigSnapshotFilenames[i]);
        }
    }
    
    if ((0 != configSnapshotFilenames.size()) && (false == completePath))
    {
        for (i = 0; i < configSnapshotFilenames.size(); i++)
        {
            string firstPart, onlyFilename;
            // Split the last '/' to remove the dir path from filename
            StringUtils::splitBasedOnLastOccuranceOf (configSnapshotFilenames[i], '/', firstPart, onlyFilename);
            configSnapshotFilenames[i] = onlyFilename;
        }
    }
}

void VcsFabConfigSnapshotWorker::getAllConfigSnapshotFilenamesForDomainId (const UI32 &selectedRbridgeId, set<string> &configSnapshotFilenames, vector<string> &snapshotIds)
{
    vector<string>           allFilenames;
    map <UI32, set<string> > rbridgeIdSnapshotIdsMap;

    configSnapshotFilenames.clear();

    getAllConfigSnapshotFilenames(allFilenames);

    if (0 < allFilenames.size())
    {
        parseConfigSnapshotFilenames (allFilenames, rbridgeIdSnapshotIdsMap);

        if (rbridgeIdSnapshotIdsMap.end() != rbridgeIdSnapshotIdsMap.find(selectedRbridgeId))
        {
            set <string> snapshotIdset  = rbridgeIdSnapshotIdsMap[selectedRbridgeId];
            set <string>::iterator it ;

            for(it = snapshotIdset.begin() ; it != snapshotIdset.end(); ++it)
            {
                string localFileName , globalFileName;

                getConfigSnapshotCompleteFileNames (*it, selectedRbridgeId , localFileName, globalFileName);

                configSnapshotFilenames.insert(localFileName);
                configSnapshotFilenames.insert(globalFileName);

                snapshotIds.push_back(*it);
            }     
        }
    }
}

void VcsFabConfigSnapshotWorker::deleteAllConfigSnapshotFilesForDomainId (const UI32 &selectedRbridgeId , const bool &syncOnAllNodes)
{
    set<string>     configSnapshotFilenames;
    string          cmdToDelete;
    vector<string>  snapshotIds;

    getAllConfigSnapshotFilenamesForDomainId (selectedRbridgeId, configSnapshotFilenames, snapshotIds);

    if (0 != configSnapshotFilenames.size())
    {
        cmdToDelete = "/bin/rm ";
        
        set<string>::iterator element = configSnapshotFilenames.begin();
        set<string>::iterator endElement = configSnapshotFilenames.end();
        
        while (endElement != element)
        {
            cmdToDelete = cmdToDelete + " " + (*element);
            element++;
        }
    
        SI32 ret = 0;
        vector<string> commandOutput;

        ret = FrameworkToolKit::systemCommandOutput (cmdToDelete, commandOutput);

        if (0 != ret)
        {   
            WaveNs::trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker:: deleteAllConfigSnapshotFilesForDomainId : Failed with ret-") + WEXITSTATUS(ret));
        }
        else
        {
            if (true == syncOnAllNodes)
            {
                ResourceId status = WAVE_MESSAGE_SUCCESS;
                vector<string> vecFilenames (configSnapshotFilenames.begin(), configSnapshotFilenames.end());
                configSnapshotFilenames.clear();

                status = FrameworkToolKit::deleteFilesOnAllNodes (vecFilenames, FILE_DEST_PATH_ABSOLUTE | FILE_PUSH_TO_HA_PEER, true);

                if (WAVE_MESSAGE_SUCCESS != status )
                {   
                    WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker:: deleteAllConfigSnapshotFilesForDomainId : Failed to delete files from all nodes with status - ") + FrameworkToolKit::localize (status)+ (", rbridgeId-") + selectedRbridgeId);
                }
            }
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabConfigSnapshotWorker::deleteAllConfigSnapshotFilesForDomainId : DomainId = ") + selectedRbridgeId + (", No Snapshot files present"));
    }
}

ResourceId VcsFabConfigSnapshotWorker::setConfigSnapshotReplayFile (const string &snapshotLocalFilename, const string &snapshotGlobalFilename)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    string configMgmtScriptsPath    = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory ();
    string configMgmtLocalFileName = configMgmtScriptsPath + "/" + ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename ();

    // copy local snapshot-config file to configMgmtFile (since it will be replayed during cluster reformation)
    string cmd = string("/bin/cp -f " + snapshotLocalFilename + " " + configMgmtLocalFileName);
    WaveNs::trace (TRACE_LEVEL_DEVEL, string("VcsFabConfigSnapshotWorker::setConfigSnapshotReplayFile : cmdToSetReplay \"") + cmd + ("\""));  
    vector<string>  output;

    SI32 ret =  FrameworkToolKit::systemCommandOutput (cmd, output);

    if (0 != ret)
    {   
        status = VCS_CONFIG_SNAPSHOT_CREATE_REPLAY_FILE_FAIL;
        WaveNs::trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker::setConfigSnapshotReplayFile : Copy failed for LocalConfig  Command-" + cmd + ", ret = " + WEXITSTATUS(ret)));
    }
    else
    {
        //copy global snapshot-config file to configMgmtFile
        string configMgmtFileName = configMgmtScriptsPath + "/" + ConfigFileManagementToolKit::getConfigFileManagementGlobalOnlyFilename ();
        cmd = string("/bin/cp -f " + snapshotGlobalFilename + " " + configMgmtFileName);
        WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabConfigSnapshotWorker::setConfigSnapshotReplayFile : cmdToSetReplay \"") + cmd + ("\""));
        output.clear ();

        ret =  FrameworkToolKit::systemCommandOutput (cmd, output);

        if (0 != ret)
        {
            status = VCS_CONFIG_SNAPSHOT_CREATE_REPLAY_FILE_FAIL;
            WaveNs::trace (TRACE_LEVEL_ERROR, string("VcsFabConfigSnapshotWorker::setConfigSnapshotReplayFile : Copy failed for LocalConfig  Command-" + cmd + ", ret = " + WEXITSTATUS(ret)));

            //Since copy of global config failed remove the local config that was copied earlier.

            ConfigFileManagementToolKit::deleteConfigurationFile (configMgmtLocalFileName);;
        }

    }

    return (status);
}

ResourceId VcsFabConfigSnapshotWorker::checkIfSnapshotFilesPresent (const string &snapshotId, const UI32 &rbridgeId, UI32 &countOfSnapshotsForSelectedRbridgeId, vector<string> &localFileNames, vector<string> &globalFileNames)
{
    countOfSnapshotsForSelectedRbridgeId = 0;

    //check for the presence by just listing the filenames in that directory
    ResourceId      status = WAVE_MESSAGE_ERROR;
    string          localFileName;
    string          globalFileName;
    vector<string>  snapshotIds;
    set<string>     configSnapshotFilenames;

    getAllConfigSnapshotFilenamesForDomainId (rbridgeId, configSnapshotFilenames, snapshotIds);

    countOfSnapshotsForSelectedRbridgeId = configSnapshotFilenames.size();

    if (0 < countOfSnapshotsForSelectedRbridgeId)
    {
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabConfigSnapshotWorker::checkIfSnapshotFilesPresent : no snapshots present\n"));
        return (status);
    }    

    if ( "all" == snapshotId )
    {
        for (UI32 snapshotIdIndex=0; snapshotIdIndex < snapshotIds.size(); snapshotIdIndex++)
        {
            getConfigSnapshotCompleteFileNames (snapshotIds[snapshotIdIndex], rbridgeId, localFileName, globalFileName);

            localFileNames.push_back(localFileName);
            globalFileNames.push_back(globalFileName);

        }
    }
    else
    {
        getConfigSnapshotCompleteFileNames (snapshotId, rbridgeId, localFileName, globalFileName);

        set<string>::iterator limitingElement = configSnapshotFilenames.end();
                 
        if ((limitingElement != configSnapshotFilenames.find(localFileName)) && (limitingElement != configSnapshotFilenames.find(globalFileName)))
        {    
            localFileNames.push_back(localFileName);
            globalFileNames.push_back(globalFileName);

            status = WAVE_MESSAGE_SUCCESS;
        }    
        else
        {
            status = WAVE_MESSAGE_ERROR;
        }
    }
    
    return (status);
}

void VcsFabConfigSnapshotWorker::getRestoreTriggeredConfigSnapshotFilenameForDomainId  (const UI32 &rbridgeId, string &configSnapshotFilename)
{
    set<string>     filenames;
    vector<string>  snapshotIds;
    getAllConfigSnapshotFilenamesForDomainId (rbridgeId, filenames, snapshotIds);

    set<string>::iterator element = filenames.begin();
    set<string>::iterator endElement = filenames.end();
    
    while (endElement != element)
    {
        /*
            Now the config snapshot file is splitted into global config and local config. So whenever we need to restore on the disconnected node we just need to push the local config 
            snapshot file . So for global config file , dont do anything.      
        */

        if (StringUtils::endsWith(*element, "global.cs")) 
        {
            //skip if the file is global 
            element++;
            continue;
        }

        VcsConfigSnapshotObject *pVcsConfigSnapshotObject = new VcsConfigSnapshotObject ();
        WaveNs::prismAssert (NULL != pVcsConfigSnapshotObject, __FILE__, __LINE__);
        pVcsConfigSnapshotObject->setMetadataOnly (true);
        ResourceId status = pVcsConfigSnapshotObject->loadConfiguration ((*element));
    
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::getRestoreTriggeredConfigSnapshotFilenameForDomainId : Unable to load configuration present for rbridgeId - ") + rbridgeId + (", filename - ") + (*element));
            status = VCS_CONFIG_SNAPSHOT_OPEN_LOCAL_CONFIG_FILE_FAIL;
        }
        else
        {
            if (true == pVcsConfigSnapshotObject->getRestoreOnDisconnectedNode()) 
            {
                    configSnapshotFilename = (*element);
                    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsFabConfigSnapshotWorker::getRestoreTriggeredConfigSnapshotFilenameForDomainId : Restore flag is set, for rbridgeId - ") + rbridgeId + (", filename - ") + (*element));
                    delete pVcsConfigSnapshotObject;
                    pVcsConfigSnapshotObject = NULL;
                    break; // we will break once we get a file with "restoreonDisconnectednode" flag as true because we dont allow multiple restore ata time.
            }
        }

        if (NULL != pVcsConfigSnapshotObject)
        {
            delete pVcsConfigSnapshotObject;
        }
        element++;
    }    
}


ResourceId VcsFabConfigSnapshotWorker::retrieveNodeSpecificConfig (const UI32 &rbridgeId, string &configData, DateTime &configCreatedTime, bool isLocalOnlyConfig)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    // Acquire lock to allow only single thread at any time
    // This is to prevent unnecessary CPU cycle consumption
    m_configSnapshotMutex.lock ();

    DateTime currentTimeStamp;
    configCreatedTime = currentTimeStamp;

    string configCmd = ""; 
    
    if (isLocalOnlyConfig)
    {
        configCmd = string("show rbridge-local-running-config rbridge-id ")+ rbridgeId;
    }
    else
    {
        configCmd = string ("show global-running-config");
    }

    vector<string>  commandOutput;
    SI32            systemCommandReturnCode     = 0;
    string          filteredConfigData ("");
    // get this node's filtered config from noscli

    systemCommandReturnCode = ConfigFileManagementToolKit::configFileManagementClientCommandOutput (configCmd, commandOutput);

    // Unlock now since the cmd execution is complete
    m_configSnapshotMutex.unlock ();

    if (0 != systemCommandReturnCode)
    {   
        if (127 != systemCommandReturnCode)
        {
            status = WAVE_MESSAGE_ERROR;
            trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigSnapshotWorker::retrieveNodeSpecificConfig : system command : \n \"") + configCmd + ("\" ") + (" failed, with return code : ") + systemCommandReturnCode);
            return (status);
        }
    }

    UI32 commandOutputSize  = commandOutput.size ();

    for (UI32 j = 0; j < commandOutputSize; j++)
    {
        trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigSnapshotWorker::retrieveNodeSpecificConfig : command output : \"") + commandOutput[j] + ("\" "));
        filteredConfigData += commandOutput[j];
    }

    configData = filteredConfigData;

    return (status);
}

ResourceId VcsFabConfigSnapshotWorker::checkIfSnapshotBackupFilePresent ()
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    
    ifstream configSnapshotBackupFile ((VcsFabConfigSnapshotWorker::getConfigSnapshotBackupFilename()).c_str());

    if (false == (configSnapshotBackupFile.is_open ()))
    {   
        status = WAVE_MESSAGE_ERROR;
    }
    return (status);
} 

UI32 VcsFabConfigSnapshotWorker::getMaxSnapshotsPerRbridgeId ()
{
    return (s_maxSnapshotsPerRbridgeId);
}    

}
