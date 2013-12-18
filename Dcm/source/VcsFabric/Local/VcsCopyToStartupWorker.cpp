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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsCopyToStartupWorker.h"
#include "Framework/Persistence/PersistenceObjectManagerCopyFileMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerCopySchemaMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerSetStartupFileMessage.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"
#include "DcmCore/DcmToolKit.h" 
#include "DcmResourceIdEnums.h"
#include "ValClientInterface/ValClientSynchronousConnection.h" 
#include <raslog/raslog.h>
#include <raslog/raslog_dcm.h>
#include <hasm/hasm.h>

namespace DcmNs
{

VcsCopyToStartupWorker::VcsCopyToStartupWorker (VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager)
    : WaveWorker (pVcsFabricLocalObjectManager)
{

    addOperationMap (VCS_COPY_RUNNING_TO_STARTUP_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsCopyToStartupWorker::copyRunningToStartupMessageHandler));
    addOperationMap (VCS_COPY_DEFAULT_TO_STARTUP_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsCopyToStartupWorker::copyDefaultToStartupMessageHandler)); 
    addOperationMap (VCS_COPY_TO_STARTUP_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsCopyToStartupWorker::copyToStartupConfigMessageHandler)); 
    addOperationMap (VCS_COPY_TO_STARTUP_ALLOWED_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsCopyToStartupWorker::copyToStartupAllowedMessageHandler)); 
}


VcsCopyToStartupWorker::~VcsCopyToStartupWorker ()
{
}

VcsCopyToStartupWorker *VcsCopyToStartupWorker::getInstance ()
{
    static VcsCopyToStartupWorker *pVcsCopyToStartupWorker = (VcsFabricLocalObjectManager::getInstance ())->m_pVcsCopyToStartupWorker;
    WaveNs::prismAssert (NULL != pVcsCopyToStartupWorker, __FILE__, __LINE__);
    return pVcsCopyToStartupWorker;
}

string VcsCopyToStartupWorker::getClassName ()
{
    return ("VcsCopyToStartupWorker");
}

PrismMessage *VcsCopyToStartupWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case VCS_COPY_RUNNING_TO_STARTUP_MESSAGE:
            pPrismMessage = new VcsCopyRunningToStartupMessage ();
            break;
        case VCS_COPY_DEFAULT_TO_STARTUP_MESSAGE:
            pPrismMessage = new VcsCopyDefaultToStartupMessage ();
            break;
        case VCS_COPY_TO_STARTUP_MESSAGE:
            pPrismMessage = new VcsCopyToStartupMessage ();
            break;
        case VCS_COPY_TO_STARTUP_ALLOWED_MESSAGE:
            pPrismMessage = new VcsCopyToStartupAllowedMessage ();
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void VcsCopyToStartupWorker::copyRunningToStartupMessageHandler (VcsCopyRunningToStartupMessage *pVcsCopyRunningToStartupMessage)
{

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::triggerStandbyCopyRunningToStartupIfPossible),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::copyRunningToStartupStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::changeCcmdCfgValidityForPersistRunningConfigStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::synchronizeStandbyCopyRunningToStartupStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::sendCcmdSyncUpdateMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::prismLinearSequencerFailedStep)
    };

    VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext = new VcsCopyToStartupMessagingContext (pVcsCopyRunningToStartupMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsCopyToStartupMessagingContext->setContextInfo (COPY_RUNNING_TO_STARTUP_CONTEXT);

    pVcsCopyToStartupMessagingContext->start ();
}

void VcsCopyToStartupWorker::synchronizeStandbyCopyRunningToStartupStep (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{

    UI32 context = pVcsCopyToStartupMessagingContext->getContextInfo ();

    if (context == COPY_RUNNING_TO_STARTUP_CONTEXT)
    {
        trace (TRACE_LEVEL_INFO, "VcsCopyToStartupWorker::copyRunningToStartupStep: trigger HA sync update.");

        ResourceId standbyStatus = sendHaSyncUpdateMessage (true, context);

        pVcsCopyToStartupMessagingContext->setStatusForCopyRunningToStartupOnStandby (standbyStatus);

        pVcsCopyToStartupMessagingContext->executeNextStep (pVcsCopyToStartupMessagingContext->getStatusForCopyRunningToStartupOnActive ());
    }
    else if (COPY_RUNNING_TO_STARTUP_OPTIMIZATION_CONTEXT == context)
    {
        // we won't go to next step because standby has not return yet, will continue in the call back
        return; 
    } 
}

void VcsCopyToStartupWorker::triggerStandbyCopyRunningToStartupIfPossibleCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerHaSyncUpdateMessage *pMessage, VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pMessage, __FILE__, __LINE__);

        pVcsCopyToStartupMessagingContext->setStatusForCopyRunningToStartupOnStandby (pMessage->getCompletionStatus ());

        delete pMessage;
    }
    else
    {
        pVcsCopyToStartupMessagingContext->setStatusForCopyRunningToStartupOnStandby (frameworkStatus);

        if (NULL != pMessage)
        {
            delete pMessage;
        }
    }
 
    pVcsCopyToStartupMessagingContext->executeNextStep (pVcsCopyToStartupMessagingContext->getStatusForCopyRunningToStartupOnActive ());
}

void VcsCopyToStartupWorker::triggerStandbyCopyRunningToStartupIfPossible (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{

    if (FrameworkToolKit::getIsSyncDumpCompleted ())
    {

        UI32 context = pVcsCopyToStartupMessagingContext->getContextInfo ();

        if (COPY_RUNNING_TO_STARTUP_CONTEXT == context)
        {

            if (FrameworkToolKit::getIsLiveSyncEnabled () == true)
            {

                trace (TRACE_LEVEL_INFO, "VcsCopyToStartupWorker::triggerStandbyCopyRunningToStartIfPossible: need to trigger copy running to start on standby in parallel.");

                bool isDbSyncRequired = false;

                UI32 newContext = COPY_RUNNING_TO_STARTUP_OPTIMIZATION_CONTEXT; 

                FrameworkObjectManagerHaSyncUpdateMessage *pFrameworkObjectManagerHaSyncUpdateMessage = new FrameworkObjectManagerHaSyncUpdateMessage ();

                pFrameworkObjectManagerHaSyncUpdateMessage->setIsDbSyncRequired (isDbSyncRequired);

                pFrameworkObjectManagerHaSyncUpdateMessage->addBuffer (CONTEXT_INFO, sizeof (UI32), (void *) &newContext, false);

                ResourceId status = send (pFrameworkObjectManagerHaSyncUpdateMessage,
                                          reinterpret_cast<PrismMessageResponseHandler> (&VcsCopyToStartupWorker::triggerStandbyCopyRunningToStartupIfPossibleCallback),
                                          pVcsCopyToStartupMessagingContext,
                                          0);


                if (WAVE_MESSAGE_SUCCESS != status)
                {

                    trace (TRACE_LEVEL_ERROR, string ("VcsCopyToStartupWorker::triggerStandbyCopyRunningToStartIfPossible : Failed to trigger parallel copy running to startup. Status: ") + FrameworkToolKit::localize (status));

                    pVcsCopyToStartupMessagingContext->setStatusForCopyRunningToStartupOnStandby (status);

                    delete pFrameworkObjectManagerHaSyncUpdateMessage;
                }
                else
                {
                    pVcsCopyToStartupMessagingContext->setContextInfo (newContext);

                    trace (TRACE_LEVEL_INFO, string ("VcsCopyToStartupWorker::triggerStandbyCopyRunningToStartIfPossible : Successfully trigger parallel copy running to startup.  "));
                }

            }
        }
    }

    pVcsCopyToStartupMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsCopyToStartupWorker::copyRunningToStartupStep (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{

    ResourceId schemaStatus = ConfigFileManagementToolKit::copyRunningToStartup ();

    ResourceId fileStatus = ConfigFileManagementToolKit::copyRunningToStartupFile ();
    
    trace (TRACE_LEVEL_INFO, string ("VcsCopyToStartupWorker::copyRunningToStartupStep : schemaStatus: ") + FrameworkToolKit::localize (schemaStatus) + " fileStatus: " +  FrameworkToolKit::localize (fileStatus));

    if (schemaStatus != WAVE_MESSAGE_SUCCESS)
    {
        pVcsCopyToStartupMessagingContext->setStatusForCopyRunningToStartupOnActive (schemaStatus);
        raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_FRCLOG, DCM_1102);
    }
    else if (fileStatus != WAVE_MESSAGE_SUCCESS)
    {
        pVcsCopyToStartupMessagingContext->setStatusForCopyRunningToStartupOnActive (fileStatus);
    }
    else
    {
        pVcsCopyToStartupMessagingContext->setStatusForCopyRunningToStartupOnActive (WAVE_MESSAGE_SUCCESS);
    }
 
    // we won't fail sequencer here, because standby may not return yet

    pVcsCopyToStartupMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsCopyToStartupWorker::changeCcmdCfgValidityForPersistRunningConfigStep (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{
    ResourceId distributionMode = DcmToolKit::getDistributionMode ();

    if (DCM_ONLY == distributionMode)
    {
        // In case of FC, (CCM_ONLY mode,) trigger broadcase the event on persisting running config.

        vector<string>  output;
        SI32            returnStatus = DcmToolKit::changeCcmdCfgValidity ( true, output );
        TraceLevel      traceLevel   = TRACE_LEVEL_INFO;

        if (0 != returnStatus)
        {
            traceLevel = TRACE_LEVEL_ERROR;
        }

        string outputString = "";

        if (0 < output.size ())
        {
            outputString = output[0];
        }

        WaveNs::tracePrintf (traceLevel, "VcsCopyToStartupWorker::changeCcmdCfgValidityForPersistRunningConfigStep: changeCcmdCfgValidity returned with message : %s, and  return status : %d", outputString.c_str(), returnStatus);
    }

    pVcsCopyToStartupMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}


void VcsCopyToStartupWorker::sendSyncUpdateMessage (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{

    if (FrameworkToolKit::getIsSyncDumpCompleted ())
    {
        UI32 context = pVcsCopyToStartupMessagingContext->getContextInfo ();

        trace (TRACE_LEVEL_INFO, "VcsCopyToStartupWorker::sendSyncUpdateMessage: trigger HA sync update.");
    
        if (context == COPY_RUNNING_TO_STARTUP_CONTEXT)
        {
            sendHaSyncUpdateMessage (true, context);
        }
        else
        {
            sendHaSyncUpdateMessage (false, context);
        }
    }

    pVcsCopyToStartupMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsCopyToStartupWorker::sendCcmdSyncUpdateMessageStep (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{
    UI32 context = pVcsCopyToStartupMessagingContext->getContextInfo ();

    ResourceId status = (VcsFabricLocalObjectManager::getInstance ())->sendCcmdSyncUpdateMessage (context);	

    pVcsCopyToStartupMessagingContext->executeNextStep (status);
}

void VcsCopyToStartupWorker::copyDefaultToStartupMessageHandler (VcsCopyDefaultToStartupMessage *pVcsCopyDefaultToStartupMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::sendCopyDefaultToStartupStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::cleanupPreservedConfigFilesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::changeCcmdCfgValidityForDefaultConfigStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::sendSyncUpdateMessage),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::removeCcmdCfgOnStandbyStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::prismLinearSequencerFailedStep)
    };

    VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext = new VcsCopyToStartupMessagingContext (pVcsCopyDefaultToStartupMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsCopyToStartupMessagingContext->setContextInfo (COPY_DEFAULT_TO_STARTUP_CONTEXT);
    pVcsCopyToStartupMessagingContext->start ();
}

void VcsCopyToStartupWorker::removeCcmdCfgOnStandbyStep (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{

    ResourceId distributionMode = DcmToolKit::getDistributionMode ();
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (DCM_ONLY == distributionMode)
    {
        status = VcsFabricLocalObjectManager::removeCcmdCfgOnStandby ();
    }

    pVcsCopyToStartupMessagingContext->executeNextStep (status);

}

void VcsCopyToStartupWorker::cleanupPreservedConfigFilesStep (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{
    string globalOnlyConfigFilePath = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory() + "/" + ConfigFileManagementToolKit::getConfigFileManagementGlobalOnlyFilename ();
    string localOnlyConfigFilePath = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory() + "/" + ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename ();

    ConfigFileManagementToolKit::deleteConfigurationFile (globalOnlyConfigFilePath);
    ConfigFileManagementToolKit::deleteConfigurationFile (localOnlyConfigFilePath);

    pVcsCopyToStartupMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsCopyToStartupWorker::sendCopyDefaultToStartupStep (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{
    WaveMessageStatus status           = WAVE_MESSAGE_ERROR;
    ResourceId        completionStatus = WAVE_MESSAGE_ERROR;

	PersistenceObjectManagerCopyFileMessage *pPersistenceObjectManagerCopyFileMessage;

    WaveNs::trace (TRACE_LEVEL_INFO, "VcsCopyToStartupWorker::sendCopyDefaultToStartupStep: copying from default to start schema");

    pPersistenceObjectManagerCopyFileMessage = new PersistenceObjectManagerCopyFileMessage (WAVE_PERSISTENCE_DEFAULT_FILE);
    
    status = sendSynchronously (pPersistenceObjectManagerCopyFileMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsCopyToStartupWorker::sendCopyDefaultToStartupStep: send to Persistence service failed");
        raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1104);

        delete pPersistenceObjectManagerCopyFileMessage;
        pVcsCopyToStartupMessagingContext->executeNextStep (status);

        return;
    }
    else
    {
        completionStatus = pPersistenceObjectManagerCopyFileMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsCopyToStartupWorker::sendCopyDefaultToStartupStep: Message completed with error") + FrameworkToolKit::localize (completionStatus));
            raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1104);
        }
    }

    delete pPersistenceObjectManagerCopyFileMessage;

    pVcsCopyToStartupMessagingContext->executeNextStep (completionStatus);
}

void VcsCopyToStartupWorker::changeCcmdCfgValidityForDefaultConfigStep (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{
    ResourceId distributionMode = DcmToolKit::getDistributionMode ();

    if (DCM_ONLY == distributionMode)
    {   
        // In case of FC, (DCM_ONLY mode,) delete cfg file for Ccmd.
        // Note : 1. In case of FC, node does not reboot after "copy default-config startup-config" operation.

        vector<string>  output;
        SI32            returnStatus = DcmToolKit::changeCcmdCfgValidity ( false, output );
        TraceLevel      traceLevel   = TRACE_LEVEL_INFO;

        if (0 != returnStatus)
        {   
            traceLevel = TRACE_LEVEL_ERROR;
        }

        string outputString = "";

        if (0 < output.size ())
        {
            outputString = output[0];
        }

        WaveNs::tracePrintf (traceLevel, "VcsCopyToStartupWorker::changeCcmdCfgValidityForDefaultConfigStep: changeCcmdCfgValidity returned with message : %s, and  return status : %d", outputString.c_str(), returnStatus);
    }
    else if (DCM_CCM == distributionMode)
    {
        // broadcast startup schema change event from Dcmd in case of SA/MC mode.
        FrameworkToolKit::broadcastStartupSchemaChangeEvent (DEFAULT_SCHEMA);
    }

    pVcsCopyToStartupMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsCopyToStartupWorker::copyToStartupConfigMessageHandler (VcsCopyToStartupMessage *pVcsCopyToStartupMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::sendCopyFileToStartupStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::changeCcmdCfgValidityForFileStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::sendSyncUpdateMessage),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::removeCcmdCfgOnStandbyStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsCopyToStartupWorker::prismLinearSequencerFailedStep)
    };

    VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext = new VcsCopyToStartupMessagingContext (pVcsCopyToStartupMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsCopyToStartupMessagingContext->setContextInfo (COPY_FILE_TO_STARTUP_CONTEXT);
    pVcsCopyToStartupMessagingContext->start ();
}

void VcsCopyToStartupWorker::sendCopyFileToStartupStep (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{
    WaveMessageStatus   status           = WAVE_MESSAGE_ERROR;
    ResourceId          completionStatus = WAVE_MESSAGE_ERROR;

    VcsCopyToStartupMessage *pVcsCopyToStartupMessage = dynamic_cast<VcsCopyToStartupMessage *>(pVcsCopyToStartupMessagingContext->getPPrismMessage());
    const string                   path = pVcsCopyToStartupMessage->getPath ();

	PersistenceLocalObjectManagerSetStartupFileMessage *pPersistenceLocalObjectManagerSetStartupFileMessage  = new PersistenceLocalObjectManagerSetStartupFileMessage (WAVE_PERSISTENCE_REGULAR_FILE, path);

    trace (TRACE_LEVEL_INFO, "VcsCopyToStartupWorker::sendCopyFileToStartupStep: copying from " + pVcsCopyToStartupMessage->getPath () + " to start schema");

    status = sendSynchronously (pPersistenceLocalObjectManagerSetStartupFileMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "VcsCopyToStartupWorker::sendCopyFileToStartupStep: send to Persistence service failed");

        delete pPersistenceLocalObjectManagerSetStartupFileMessage;

        pVcsCopyToStartupMessagingContext->executeNextStep (status);

        return;
    }
    else
    {
        completionStatus = pPersistenceLocalObjectManagerSetStartupFileMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, "VcsCopyToStartupWorker::sendCopyFileToStartupStep: Message completed with error");
        }
    }

    delete pPersistenceLocalObjectManagerSetStartupFileMessage;

    pVcsCopyToStartupMessagingContext->executeNextStep (status);
}

void VcsCopyToStartupWorker::changeCcmdCfgValidityForFileStep (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext)
{
    ResourceId distributionMode = DcmToolKit::getDistributionMode ();

    VcsCopyToStartupMessage *pVcsCopyToStartupMessage = dynamic_cast<VcsCopyToStartupMessage *>(pVcsCopyToStartupMessagingContext->getPPrismMessage());

    if (DCM_ONLY == distributionMode)
    {   
        // In case of FC, (DCM_ONLY mode,) delete cfg file for Ccmd.
        // Note : 1. In case of FC, node does not reboot after "copy <file> startup-config" operation.
        vector<string>  output;
        SI32        returnStatus = DcmToolKit::changeCcmdCfgValidity( false, output, pVcsCopyToStartupMessage->getPath () ); 
        TraceLevel  traceLevel   = TRACE_LEVEL_INFO;
        if (0 != returnStatus)
        {   
            traceLevel = TRACE_LEVEL_ERROR;
        }

        WaveNs::tracePrintf (traceLevel, "VcsCopyToStartupWorker::changeCcmdCfgValidityForFileStep:changeCcmdCfgValidity failed with error message  : %s, return status : %d",output[0].c_str(), returnStatus);
    }
    else if (DCM_CCM == distributionMode)
    {
        // broadcast startup schema change event from Dcmd in case of SA/MC mode.     
        FrameworkToolKit::broadcastStartupSchemaChangeEvent (SCHEMA_FILE, pVcsCopyToStartupMessage->getPath ());
    }
            
    pVcsCopyToStartupMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsCopyToStartupWorker::copyToStartupAllowedMessageHandler (VcsCopyToStartupAllowedMessage *pVcsCopyToStartupAllowedMessage)
{
    ResourceId thisVcsNodeClusterType = DcmToolKit::getThisVcsNodeClusterType ();

    if ((VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE == thisVcsNodeClusterType) || (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == thisVcsNodeClusterType))
    {
        pVcsCopyToStartupAllowedMessage->setIsAllowed (true);
    }
    else
    {
        pVcsCopyToStartupAllowedMessage->setIsAllowed (false);
    }

    pVcsCopyToStartupAllowedMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    reply(pVcsCopyToStartupAllowedMessage);
}

ResourceId VcsCopyToStartupWorker::sendHaSyncUpdateMessage (const bool &isDbSyncRequired, const UI32 &context)
{
    FrameworkObjectManagerHaSyncUpdateMessage *pFrameworkObjectManagerHaSyncUpdateMessage = new FrameworkObjectManagerHaSyncUpdateMessage ();

    prismAssert (NULL != pFrameworkObjectManagerHaSyncUpdateMessage, __FILE__, __LINE__);

    pFrameworkObjectManagerHaSyncUpdateMessage->setIsDbSyncRequired (isDbSyncRequired);

    pFrameworkObjectManagerHaSyncUpdateMessage->addBuffer (CONTEXT_INFO, sizeof (UI32), (void *) &context, false);

    ResourceId status;
    status = sendSynchronously (pFrameworkObjectManagerHaSyncUpdateMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsCopyToStartupWorker::sendHaSyncUpdateMessage: Failed send to FrameworkObjectManagerHaSyncUpdateMessage.  Status: ") + FrameworkToolKit::localize (status));
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsCopyToStartupWorker::sendHaSyncUpdateMessage: FrameworkObjectManagerHaSyncUpdateMessage sent.  "));
    }

    if (pFrameworkObjectManagerHaSyncUpdateMessage != NULL)
    {
        delete pFrameworkObjectManagerHaSyncUpdateMessage;
    }

    return status;
}

}
