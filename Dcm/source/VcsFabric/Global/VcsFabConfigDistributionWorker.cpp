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
 *@file VcsFabConfigDistributionWorker.cpp 
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 * 
 * Description: This file has the implementation of the
 *              Worker class which is responsible for FC to MC 
 *              mode transition.
 * Author :     Brian Adaniya
 */
#include "VcsFabric/Global/VcsFabConfigDistributionWorker.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "VcsFabric/Global/VcsFabConfigDistributeMessage.h"
#include "VcsFabric/Global/VcsFabConfigDistributeSequencerContext.h"
#include "VcsFabric/Local/VcsNodeConfigDistributeMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "Cluster/Local/HeartBeat/HeartBeatMessages.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"

#include <algorithm>

namespace DcmNs
{

VcsFabConfigDistributionWorker::VcsFabConfigDistributionWorker (VcsFabObjectManager *pVcsFabObjectManager) 
    : WaveWorker (pVcsFabObjectManager)
{
    addOperationMap (VCS_FABRIC_CONFIG_DISTRIBUTE, reinterpret_cast<PrismMessageHandler> (&VcsFabConfigDistributionWorker::configDistributeMessageHandler));
}

VcsFabConfigDistributionWorker::~VcsFabConfigDistributionWorker ()
{
}

PrismMessage *VcsFabConfigDistributionWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case VCS_FABRIC_CONFIG_DISTRIBUTE :
            pPrismMessage = new VcsFabConfigDistributeMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void VcsFabConfigDistributionWorker::configDistributeMessageHandler (VcsFabConfigDistributeMessage *pVcsFabConfigDistributeMessage)
{
    trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigDistributionWorker::configDistributeMessageHandler : Entering ..."));

    ResourceId                              vcsClusterType                          = pVcsFabConfigDistributeMessage->getVcsClusterType ();
    VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext = NULL;

    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == vcsClusterType)
    {
        PrismLinearSequencerStep sequencerManagementToFabricClusterSteps[] =
        {  
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigDistributionWorker::validateIfClusterFormationInProgressStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigDistributionWorker::prepareClusterModeTransitionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransition),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigDistributionWorker::setStartupConfigAndRebootStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigDistributionWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigDistributionWorker::prismLinearSequencerFailedStep)
        };

        pVcsFabConfigDistributeSequencerContext = new VcsFabConfigDistributeSequencerContext (pVcsFabConfigDistributeMessage, this, sequencerManagementToFabricClusterSteps, sizeof (sequencerManagementToFabricClusterSteps) / sizeof (sequencerManagementToFabricClusterSteps[0]));
    }
    else if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == vcsClusterType)
    {
        PrismLinearSequencerStep sequencerFabricToManagementClusterSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigDistributionWorker::getGlobalConfigFromPrincipalNodeOrForceNodeIfRequiredStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigDistributionWorker::distributeGlobalConfigsOnSelectedNodesIfRequiredStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigDistributionWorker::prepareClusterModeTransitionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigDistributionWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabConfigDistributionWorker::prismLinearSequencerFailedStep)
        };

        pVcsFabConfigDistributeSequencerContext = new VcsFabConfigDistributeSequencerContext (pVcsFabConfigDistributeMessage, this, sequencerFabricToManagementClusterSteps, sizeof (sequencerFabricToManagementClusterSteps) / sizeof (sequencerFabricToManagementClusterSteps[0])); 
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("VcsFabConfigDistributionWorker::configDistributeMessageHandler : Invalid cluster type : ") + FrameworkToolKit::localizeToSourceCodeEnum (vcsClusterType));
        prismAssert (false, __FILE__, __LINE__);
    }

    prismAssert (NULL != pVcsFabConfigDistributeSequencerContext, __FILE__, __LINE__);

    trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::configDistributeMessageHandler : Current cluster type : ") + FrameworkToolKit::localizeToSourceCodeEnum (vcsClusterType));

    vector<LocationId>  locationIdsForTransition            = pVcsFabConfigDistributeMessage->getLocationIdVector ();
    UI32                numberOfLocationIdsForTransition    = locationIdsForTransition.size ();
    UI32                i                                   = 0;

    trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::configDistributeMessageHandler : Location id's involved in mode transition:"));

    for (i = 0; i < numberOfLocationIdsForTransition; i++)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::configDistributeMessageHandler : LocationId[") + i + string ("] : ") + locationIdsForTransition[i]);
    }

    // Transfer message inputs to sequencer context

    pVcsFabConfigDistributeSequencerContext->setLocationIdVector            (pVcsFabConfigDistributeMessage->getLocationIdVector ());
    pVcsFabConfigDistributeSequencerContext->setVcsClusterType              (vcsClusterType);
    pVcsFabConfigDistributeSequencerContext->setPreserveConfig              (pVcsFabConfigDistributeMessage->getPreserveConfig ());
    pVcsFabConfigDistributeSequencerContext->setClusterPrimaryLocationId    (FrameworkToolKit::getClusterPrimaryLocationId ());
    pVcsFabConfigDistributeSequencerContext->setLocationRole                (FrameworkToolKit::getThisLocationRole ());
    pVcsFabConfigDistributeSequencerContext->setForce                       (pVcsFabConfigDistributeMessage->getForce ());
    pVcsFabConfigDistributeSequencerContext->start ();
}

void VcsFabConfigDistributionWorker::validateIfClusterFormationInProgressStep (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext)
{
    if (true == VcsFabricLocalObjectManager::isClusterFormationInProgress ())
    {
        pVcsFabConfigDistributeSequencerContext->executeNextStep (CLUSTER_FORMATION_IS_IN_PROGRESS);
    }
    else
    {
        pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS); 
    }
}

void VcsFabConfigDistributionWorker::determineIfConfigPreservationIsRequiredStep (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext)
{
    // The configuration transaction number can be from either DCM's DB or from CCM's DB depending on which daemon this step is executed from.

    UI64 configurationTransactionId = DcmToolKit::getConfigurationTransactionId ();

    trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::determineIfConfigPreservationIsRequiredStep : \"") + DcmToolKit::getApplicationIdentifier ()  + "\" configuration transaction number : " + configurationTransactionId);

    if (DCM_CCM == DcmToolKit::getDistributionMode ())
    {
        // If this is MC or SA mode, no configuration data is stored in CCM's DB so we only need to check DCM's DB for a configuration change.
        // The above configuration transaction number is from DCM's DB.

        if (0 == configurationTransactionId)
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::determineIfConfigPreservationIsRequiredStep : Detected that the current configuration is already the default-config.  Preserving the configuration for a default-config is not necessary and so the configuration preservation processing will be skipped."));

            pVcsFabConfigDistributeSequencerContext->setIsDefaultConfigDetected (true);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::determineIfConfigPreservationIsRequiredStep : Detected a non-default configuration.  Preserving the configuration will commence."));
        }
    }
    else if ((CCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_ONLY == DcmToolKit::getDistributionMode ()))
    {
        // If this is FC mode, then we need to check both DCM's DB and CCM's DB for a configuration change:
        // CCM_ONLY: The above configuration transaction number is from CCM's DB, so we will also need to get the configuration transaction number from DCM's DB below. 
        // DCM_ONLY: The above configuration transaction number is from DCM's DB, so we will also need to get the configuration transaction number from CCM's DB below. 

        UI64 thisDaemonsConfigurationTransactionId  = configurationTransactionId;
        UI64 otherDaemonsConfigurationTransactionId = 0;
        UI16 otherDaemonsConfigurationNumberArray[16];

        ValClientSynchronousConnection *pValClientSynchronousConnection = DcmToolKit::getInterDcmCcmClient ();

        ResourceId status = pValClientSynchronousConnection->getConfigurationNumber (otherDaemonsConfigurationNumberArray);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            // Rebuild a proper configuration number:
            // The configuration number vector is currently stored as a vector of four UI64 elements but converted to a vector of sixteen UI16 elements.
            // The actual "configuration number" is the second UI64 element or fifth through eighth UI16 elements. 
            // configurationNumberVector[4] : Lowest 16-bit nibble of the configuration number
            // configurationNumberVector[5] : Low middle 16-bit nibble of the configuration number
            // configurationNumberVector[6] : High middle 16-bit nibble of the configuration number
            // configurationNumberVector[7] : Highest 16-bit nibble of the configuration number

            for (UI32 i = 7; i > 3; i--)
            {
                otherDaemonsConfigurationTransactionId = (otherDaemonsConfigurationTransactionId << 16) | otherDaemonsConfigurationNumberArray[i];
            }
        }
        else
        {
            otherDaemonsConfigurationTransactionId = 1;

            trace (TRACE_LEVEL_WARN, string ("VcsFabConfigDistributionWorker::determineIfConfigPreservationIsRequiredStep : Failed to retrieve the configuration from the other daemon, forcing the configuration to a non-default value."));    
        }

        trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::determineIfConfigPreservationIsRequiredStep : \"other daemon's\" configuration number : ") + otherDaemonsConfigurationTransactionId);

        if ((0 == thisDaemonsConfigurationTransactionId) && (0 == otherDaemonsConfigurationTransactionId))
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::determineIfConfigPreservationIsRequiredStep : Detected that the current configuration is already the default-config.  Preserving the configuration for a default-config is not necessary and so the configuration preservation processing will be skipped."));

            pVcsFabConfigDistributeSequencerContext->setIsDefaultConfigDetected (true);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::determineIfConfigPreservationIsRequiredStep : Detected a non-default configuration.  Preserving the configuration will commence."));
        }
    }
    else
    {
        // Do not do anything for other distribution modes for now.
    }

    pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsFabConfigDistributionWorker::getGlobalConfigFromPrincipalNodeOrForceNodeIfRequiredStep (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext)
{
    bool        preserveConfig  = pVcsFabConfigDistributeSequencerContext->getPreserveConfig ();
    ResourceId  vcsClusterType  = pVcsFabConfigDistributeSequencerContext->getVcsClusterType ();

    if (false == preserveConfig)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::getGlobalConfigFromPrincipalNodeIfRequiredStep : Preserving the existing configuration is not requested.  Skipping this step."));

        pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);        

        return;
    }
   
    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == vcsClusterType)
    {
       
        LocationId selectedLocationId = FrameworkToolKit::getClusterPrimaryLocationId (); 
  
        VcsNodeConfigDistributeMessage *pVcsNodeConfigDistributeMessage = new VcsNodeConfigDistributeMessage (); 
        prismAssert (NULL != pVcsNodeConfigDistributeMessage, __FILE__, __LINE__);
        
        // Set config distribute to retreive global config on local node. 
        pVcsNodeConfigDistributeMessage->setConfigDistributeOperation (VCS_CONFIG_DISTRIBUTE_OPERATION_RETRIEVE_GLOBAL_CONFIGS);
        pVcsNodeConfigDistributeMessage->setPreserveConfig                      (pVcsFabConfigDistributeSequencerContext->getPreserveConfig ());
        pVcsNodeConfigDistributeMessage->setForce                               (pVcsFabConfigDistributeSequencerContext->getForce ());

        WaveMessageStatus status = sendSynchronously (pVcsNodeConfigDistributeMessage, selectedLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabConfigDistributionWorker::getGlobalConfigFromPrincipalNodeOrForceNodeIfRequiredStep : Unable to get global configuration from selected node : LocationId: " + selectedLocationId);
            delete pVcsNodeConfigDistributeMessage;
            pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR);
        }
        else
        {
            
            ResourceId completionStatus = pVcsNodeConfigDistributeMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, "VcsFabConfigDistributionWorker::getGlobalConfigFromPrincipalNodeOrForceNodeIfRequiredStep: Failed to get global configuration from selected node : LocationId: " + selectedLocationId);
                delete pVcsNodeConfigDistributeMessage;
                pVcsFabConfigDistributeSequencerContext->executeNextStep (completionStatus);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::getGlobalConfigFromPrincipalNodeOrForceNodeIfRequiredStep: Succesfully get global configuration from selected node: Locationid: ") + selectedLocationId);

                pVcsFabConfigDistributeSequencerContext->setIsDefaultConfigDetected (pVcsNodeConfigDistributeMessage->getIsDefaultConfigDetected ());
                pVcsFabConfigDistributeSequencerContext->setClusterGlobalOnlyConfigData (pVcsNodeConfigDistributeMessage->getClusterGlobalOnlyConfigData ());

                delete pVcsNodeConfigDistributeMessage;
                pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::getGlobalConfigFromPrincipalNodeOrForceNodeIfRequiredStep : Not in fabric cluster mode.  Skipping this step."));
        pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }

}

void VcsFabConfigDistributionWorker::distributeGlobalConfigsOnSelectedNodesIfRequiredStep (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext)
{
    bool                        preserveConfig              = pVcsFabConfigDistributeSequencerContext->getPreserveConfig ();
    bool                        force                       = pVcsFabConfigDistributeSequencerContext->getForce ();
    bool                        isDefaultConfigDetected     = pVcsFabConfigDistributeSequencerContext->getIsDefaultConfigDetected ();
    ResourceId                  vcsClusterType              = pVcsFabConfigDistributeSequencerContext->getVcsClusterType ();

    if (false == preserveConfig)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::distributeGlobalConfigsOnSelectedNodesIfRequiredStep : Preserving the existing configuration is not requested.  Skipping this step."));

        pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);

        return;
    }

    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == vcsClusterType)
    {
        if ((true == force) && (true == isDefaultConfigDetected))
        {
            // Don't distribute principle node's global config because it's already default

            trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::distributeGlobalConfigsOnSelectedNodesIfRequiredStep : Distribute Global Config is not needed.  Skipping this step."));

            pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);

            return;
        }

        vector<LocationId> locationIdVector = pVcsFabConfigDistributeSequencerContext->getLocationIdVector ();

        VcsNodeConfigDistributeMessage *pVcsNodeConfigDistributeMessage = new VcsNodeConfigDistributeMessage (vcsClusterType);
        prismAssert (NULL != pVcsNodeConfigDistributeMessage, __FILE__, __LINE__);


        // Set config distribute to distribute global-only configs on local nodes. 

        pVcsNodeConfigDistributeMessage->setConfigDistributeOperation           (VCS_CONFIG_DISTRIBUTE_OPERATION_DISTRIBUTE_GLOBAL_CONFIGS);
        pVcsNodeConfigDistributeMessage->setClusterGlobalOnlyConfigData         (pVcsFabConfigDistributeSequencerContext->getClusterGlobalOnlyConfigData ());
        pVcsNodeConfigDistributeMessage->setPreserveConfig                      (pVcsFabConfigDistributeSequencerContext->getPreserveConfig ());
        pVcsNodeConfigDistributeMessage->setIsDefaultConfigDetected             (pVcsFabConfigDistributeSequencerContext->getIsDefaultConfigDetected ());
        pVcsNodeConfigDistributeMessage->setForce                               (pVcsFabConfigDistributeSequencerContext->getForce ());

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&VcsFabConfigDistributionWorker::distributeGlobalConfigsOnSelectedNodesIfRequiredStepCallback), pVcsFabConfigDistributeSequencerContext);
        prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);

        trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::distributeGlobalConfigsOnSelectedNodesIfRequiredStep : Before sendToWaveCluster"));

        // Once status propagation handling is available for wysereaoperations, then we can enable the partial success flag below

        //pWaveSendToClusterContext->setPartialSuccessFlag (true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1 (pVcsNodeConfigDistributeMessage);

        if (locationIdVector.size () > 0)
        {
            pWaveSendToClusterContext->setLocationsToSendToForPhase1 (locationIdVector);
        }

        sendToWaveCluster (pWaveSendToClusterContext);

        return;
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::distributeGlobalConfigsOnSelectedNodesIfRequiredStep : Not in fabric cluster mode.  Skipping this step."));

        pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void VcsFabConfigDistributionWorker::distributeGlobalConfigsOnSelectedNodesIfRequiredStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext = reinterpret_cast<VcsFabConfigDistributeSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());
    prismAssert (NULL != pVcsFabConfigDistributeSequencerContext, __FILE__, __LINE__);

    ResourceId                  sendToClusterCompletionStatus   = pWaveSendToClusterContext->getCompletionStatus ();

    if (WAVE_MESSAGE_SUCCESS != sendToClusterCompletionStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigDistributionWorker::distributeGlobalConfigsOnSelectedNodesIfRequiredStepCallback : Send to wave cluster status : ") + FrameworkToolKit::localize (sendToClusterCompletionStatus));

        // TODO: Brian - Work on wysereaoperation status propagation for descriptive status on each local node.  For now we will return a semi-decriptive global error.

        sendToClusterCompletionStatus = VCS_CONFIG_DISTRIBUTE_ERROR_GLOBAL_CONFIG_VALIDATION_MISMATCH; 
    }

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete (pWaveSendToClusterContext);

    pVcsFabConfigDistributeSequencerContext->executeNextStep (sendToClusterCompletionStatus);
}

void VcsFabConfigDistributionWorker::prepareClusterModeTransitionStep (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabConfigDistributionWorker::prepareClusterModeTransitionStep: Entering.");

    vector<LocationId>  locationIdVector    = pVcsFabConfigDistributeSequencerContext->getLocationIdVector ();
    ResourceId          vcsClusterType      = pVcsFabConfigDistributeSequencerContext->getVcsClusterType ();

    VcsNodeConfigDistributeMessage *pVcsNodeConfigDistributeMessage = new VcsNodeConfigDistributeMessage (vcsClusterType);
    prismAssert (NULL != pVcsNodeConfigDistributeMessage, __FILE__, __LINE__);

    pVcsNodeConfigDistributeMessage->setConfigDistributeOperation  (VCS_CONFIG_DISTRIBUTE_OPERATION_PREPARE_CLUSTER_TRANSITION);
    pVcsNodeConfigDistributeMessage->setPreserveConfig             (pVcsFabConfigDistributeSequencerContext->getPreserveConfig ());
    pVcsNodeConfigDistributeMessage->setIsDefaultConfigDetected    (pVcsFabConfigDistributeSequencerContext->getIsDefaultConfigDetected ());
    pVcsNodeConfigDistributeMessage->setForce                      (pVcsFabConfigDistributeSequencerContext->getForce ());

    WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&VcsFabConfigDistributionWorker::prepareClusterModeTransitionStepCallback), pVcsFabConfigDistributeSequencerContext);
    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);

    trace (TRACE_LEVEL_DEVEL, "VcsFabConfigDistributionWorker::prepareClusterModeTransitionStep : SendToWaveCluster issued");

    pWaveSendToClusterContext->setPartialSuccessFlag (true);
    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pVcsNodeConfigDistributeMessage);

    if (locationIdVector.size () > 0)
    {
        pWaveSendToClusterContext->setLocationsToSendToForPhase1 (locationIdVector);
    }

    sendToWaveCluster (pWaveSendToClusterContext);
}

void VcsFabConfigDistributionWorker::prepareClusterModeTransitionStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabConfigDistributionWorker::prepareClusterModeTransitionStepCallback: Entering.");

    VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext = reinterpret_cast<VcsFabConfigDistributeSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());
    prismAssert (NULL != pVcsFabConfigDistributeSequencerContext, __FILE__, __LINE__);

    ResourceId completionStatus = pWaveSendToClusterContext->getCompletionStatus ();

    if (WAVE_MESSAGE_SUCCESS != completionStatus)
    {
        // Even if send to wave cluster didn't complete successfully, check for the individual location status
        // The remote node might have gone down while unconfiguring itself and the reply from the remote node was not processed by that time
        trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigDistributionWorker::prepareClusterModeTransitionStepCallback: SendToWaveCluster status: ") + FrameworkToolKit::localize (completionStatus));

        vector<LocationId> locationIds = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();

        for (UI32 i = 0; i < locationIds.size (); i++)
        {
            completionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1 (locationIds[i]);
        
            trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::prepareClusterModeTransitionStepCallback: location status: ") + FrameworkToolKit::localize (completionStatus));

            if (WAVE_MESSAGE_SUCCESS == completionStatus)
            {
                continue;
            }

            // Check if the location status is other than "Remote location unavailable" as the remote location might
            // have disconnected itself before we were able to process the reply from sendToWaveCluster
            // If any of the locations failed with an error other than "Remote location unavailable", we don't want to proceed further
            if (completionStatus != WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE)
            {
                trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigDistributionWorker::prepareClusterModeTransitionStepCallback: location status: ") + FrameworkToolKit::localize (completionStatus));
                break;
            }
            else
            {
                completionStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete (pWaveSendToClusterContext);

    trace (TRACE_LEVEL_DEVEL, string ("VcsFabConfigDistributionWorker::prepareClusterModeTransitionStepCallback: Final completion status: ") + FrameworkToolKit::localize (completionStatus));
    pVcsFabConfigDistributeSequencerContext->executeNextStep (completionStatus);
}

void VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransition (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransition: Entering.");

    FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage = new FrameworkObjectManagerSecondaryNodeFailureNotificationMessage ();
    prismAssert (NULL != pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage, __FILE__, __LINE__);

    vector<LocationId>        failoverLocationIds = pVcsFabConfigDistributeSequencerContext->getLocationIdVector ();
    LocationId                primaryLocationId   = pVcsFabConfigDistributeSequencerContext->getClusterPrimaryLocationId ();

    if (failoverLocationIds.size () == 0)
    {
        trace (TRACE_LEVEL_INFO, "VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransition: No failover locationIds for controlled failover.");
        pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }
    
    for (UI32 i = 0; i < failoverLocationIds.size (); i++)
    {
        // Exclude the cluster primary location from the failover location list
        if (failoverLocationIds[i] != primaryLocationId)
            pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->addFailedLocationId (failoverLocationIds[i]);
    }

    if ((pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->getFailedLocationIds ()).size () == 0)
    {
        trace (TRACE_LEVEL_INFO, "VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransition: No secondary locations for controlled failover.");
        pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    // Stop heartbeat service for all nodes except primary
    vector<LocationId> stopHeartBeatLocations = pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->getFailedLocationIds ();

    for (UI32 i = 0; i < stopHeartBeatLocations.size (); i++)
    {
        string     nodeIpAddress  = FrameworkToolKit::getIpAddressForLocationId (stopHeartBeatLocations[i]);
        UI32       nodePort       = FrameworkToolKit::getPortForLocationId      (stopHeartBeatLocations[i]);

        StopHeartBeatMessage *pStopHeartBeatMessage = new StopHeartBeatMessage ();
        pStopHeartBeatMessage->setDstIpAddress  (nodeIpAddress);
        pStopHeartBeatMessage->setDstPortNumber (nodePort);
   
        WaveMessageStatus status = sendSynchronously (pStopHeartBeatMessage);
        if (WAVE_MESSAGE_SUCCESS != status) 
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransition Not able to send stop heartbeat message.");
            //prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransition: StopHeartBeatMessage: status: ") + FrameworkToolKit::localize (pStopHeartBeatMessage->getCompletionStatus ()));
        }

        delete pStopHeartBeatMessage;
    }

    pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->setIsControlledFailoverInSpecialCase (true);

    ResourceId status = send (pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransitionCallback), pVcsFabConfigDistributeSequencerContext);

    if(WAVE_MESSAGE_SUCCESS != status) 
    {
       trace (TRACE_LEVEL_INFO, "VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransition: Failed to send FrameworkObjectManagerSecondaryNodeFailureNotificationMessage to framework " + FrameworkToolKit::localize (status));
       delete pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage;
       pVcsFabConfigDistributeSequencerContext->executeNextStep (status);
    }
    else
    {
       trace (TRACE_LEVEL_INFO, "VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransition: FrameworkObjectManagerSecondaryNodeFailureNotificationMessage sent successfully to framework ");
    }
}

void VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransitionCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransitionCallback: Starting");
    prismAssert (pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage != NULL, __FILE__, __LINE__);

    VcsFabConfigDistributeSequencerContext* pVcsFabConfigDistributeSequencerContext = static_cast <VcsFabConfigDistributeSequencerContext* > (pContext);
    prismAssert (pVcsFabConfigDistributeSequencerContext != NULL, __FILE__, __LINE__);

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        status = pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_SUCCESS, "VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransitionCallback: Successfully failedover");
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransitionCallback: Failed to failover: Status:"+ FrameworkToolKit::localize (status));
        }        
    }
    else
    {
        status = frameworkStatus;
        trace (TRACE_LEVEL_ERROR, "VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransitionCallback: Could not send SecondaryNodeFailureNotificationMessage to framework Status: " + FrameworkToolKit::localize (status));
    }

    vector<LocationId> failedLocationIds = pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->getFailedLocationIds ();

    delete pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage;

    vector<LocationId> connectedLocations;
    UI32               numberOfConnectedLocations;

    FrameworkToolKit::getConnectedLocations (connectedLocations);

    numberOfConnectedLocations = connectedLocations.size ();

    for (UI32 i = 0; i < numberOfConnectedLocations; i++)
    {
        FrameworkObjectManagerRemoveKnownLocationsMessage *pMessage = new FrameworkObjectManagerRemoveKnownLocationsMessage();

        pMessage->setFailedLocations (failedLocationIds);

        UI32   numberOfFailedLocations = failedLocationIds.size ();        
        for (UI32 j = 0; j < numberOfFailedLocations; j++)
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::sendControlledFailoverMessageToSecondariesInModeTransitionCallback:: connectedLocationId[") + i + string ("] : failed locationId ") + failedLocationIds[j]);
        }

        ResourceId sendStatus = sendSynchronously (pMessage, connectedLocations[i]);

        prismAssert (WAVE_MESSAGE_SUCCESS == sendStatus, __FILE__, __LINE__);
    }

    pVcsFabConfigDistributeSequencerContext->executeNextStep (status);
}

void VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStep (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStep: Entering.");
    
    vector<LocationId> locationIdVector    = pVcsFabConfigDistributeSequencerContext->getLocationIdVector ();
    LocationRole       locationRole        = pVcsFabConfigDistributeSequencerContext->getLocationRole ();
    LocationId         primaryLocationId   = pVcsFabConfigDistributeSequencerContext->getClusterPrimaryLocationId ();

    if (find (locationIdVector.begin (), locationIdVector.end (), primaryLocationId) == locationIdVector.end ())
    {
        trace (TRACE_LEVEL_INFO, "VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStep: Cluster primary is NOT in migration, cluster will NOT be destroyed."); 
        pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    if (LOCATION_PRIMARY == locationRole) 
    {
        trace (TRACE_LEVEL_INFO, "VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStep: Cluster will be destroyed as primary is in migration.");
        
        PrismFrameworkObjectManager::disconnectFromAllKnownLocations ();

        FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage = new FrameworkObjectManagerDestroyClusterMessage ();
        prismAssert (NULL != pFrameworkObjectManagerDestroyClusterMessage, __FILE__, __LINE__);

        ResourceId  status = WAVE_MESSAGE_SUCCESS;

        pFrameworkObjectManagerDestroyClusterMessage->setIsRebootRequired  (false);

        status = send (pFrameworkObjectManagerDestroyClusterMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStepCallback), pVcsFabConfigDistributeSequencerContext);

        if (WAVE_MESSAGE_SUCCESS != status)
        {   
            trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStep: Error in requesting framework to delete cluster. status: ") + FrameworkToolKit::localize (status));
            delete pFrameworkObjectManagerDestroyClusterMessage;
            pVcsFabConfigDistributeSequencerContext->executeNextStep (status);
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStep: Destroy cluster is NOT DONE for a ") + FrameworkToolKit::localize (locationRole));
        // If node is UNCONFIRMED, unconfigure others and reboot self, if node is STANDALONE, reboot self
        // the node can be part of distributed cluster but be in "Awaiting Rejoin" or "Standalone" status
        pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
    return;
}

void VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStepCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStepCallback: Starting");
    prismAssert (pFrameworkObjectManagerDestroyClusterMessage != NULL, __FILE__, __LINE__);

    VcsFabConfigDistributeSequencerContext* pVcsFabConfigDistributeSequencerContext = static_cast <VcsFabConfigDistributeSequencerContext* > (pContext);
    prismAssert (pVcsFabConfigDistributeSequencerContext != NULL, __FILE__, __LINE__);

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        status = pFrameworkObjectManagerDestroyClusterMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_SUCCESS, "VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStepCallback: Successful in Cluster delete issued from primary");
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStepCallback: Failed to delete cluster: Status:"+ FrameworkToolKit::localize (status));
        }
    }
    else
    {
        status = frameworkStatus;
        trace (TRACE_LEVEL_ERROR, "VcsFabConfigDistributionWorker::resetPrimaryInClusterTransitionStepCallback: Could not send Destroy cluster message to framework Status: " + FrameworkToolKit::localize (status));
    }

    delete pFrameworkObjectManagerDestroyClusterMessage;

    pVcsFabConfigDistributeSequencerContext->executeNextStep (status);
}

void VcsFabConfigDistributionWorker::setStartupConfigAndRebootStep (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabConfigDistributionWorker::setStartupConfigAndRebootStep: Entering.");
    
    ResourceId         status                          =  WAVE_MESSAGE_SUCCESS;
    vector<LocationId> locationIdVector                =  pVcsFabConfigDistributeSequencerContext->getLocationIdVector ();
    LocationId         primaryLocationId               =  pVcsFabConfigDistributeSequencerContext->getClusterPrimaryLocationId ();
    LocationRole       locationRole                    =  pVcsFabConfigDistributeSequencerContext->getLocationRole ();
    ResourceId         vcsClusterType                  =  pVcsFabConfigDistributeSequencerContext->getVcsClusterType ();
    bool               preserveConfig                  =  pVcsFabConfigDistributeSequencerContext->getPreserveConfig ();
  
    if (find (locationIdVector.begin (), locationIdVector.end (), primaryLocationId) == locationIdVector.end ())
    {
        trace (TRACE_LEVEL_INFO, "VcsFabConfigDistributionWorker::setStartupConfigAndRebootStep: Nothing to be done on primary as it is NOT part of migration."); 
        pVcsFabConfigDistributeSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    if (LOCATION_PRIMARY == locationRole)
    {
        VcsNodeConfigDistributeMessage *pVcsNodeConfigDistributeMessage = new VcsNodeConfigDistributeMessage ();
        prismAssert (NULL != pVcsNodeConfigDistributeMessage, __FILE__, __LINE__);

        pVcsNodeConfigDistributeMessage->setConfigDistributeOperation (VCS_CONFIG_DISTRIBUTE_OPERATION_PRIMARY_REBOOT);
        pVcsNodeConfigDistributeMessage->setVcsClusterType            (vcsClusterType);
        pVcsNodeConfigDistributeMessage->setLocationRole              (locationRole);
        pVcsNodeConfigDistributeMessage->setPreserveConfig            (preserveConfig);

        status = sendSynchronously (pVcsNodeConfigDistributeMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabConfigDistributionWorker::setStartupConfigAndRebootStep: Failed to send reboot message. Status = " + FrameworkToolKit::localize (status))); 
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "VcsFabConfigDistributionWorker::setStartupConfigAndRebootStep: Successfully sent the reboot message."); 
        }

        delete pVcsNodeConfigDistributeMessage;
    }

    pVcsFabConfigDistributeSequencerContext->executeNextStep (status);
}

}
