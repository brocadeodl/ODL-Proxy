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
 *@file VcsFabConfigDistributionWorker.h
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc.
 * All rights reserved. Description: This file declares the
 *              Worker class which is responsible for handling 
 *              a VCS mode transition.
 *
 * Author :     Brian Adaniya
 * Date :       06/12/2012
 */

#ifndef VCSFABCONFIGDISTRIBUTIONWORKER_H
#define VCSFABCONFIGDISTRIBUTIONWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsFabObjectManager;
class VcsFabConfigDistributeMessage;
class VcsFabConfigDistributeSequencerContext;

class VcsFabConfigDistributionWorker : public WaveWorker
{
    private :
                                    VcsFabConfigDistributionWorker                                      (VcsFabObjectManager *pVcsFabObjectManager);

                PrismMessage       *createMessageInstance                                               (const UI32 &operationCode);

                void                configDistributeMessageHandler                                      (VcsFabConfigDistributeMessage *pVcsFabConfigDistributeMessage);

                void                determineIfConfigPreservationIsRequiredStep                         (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext);
                void                getGlobalConfigFromPrincipalNodeOrForceNodeIfRequiredStep           (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext);
                void                distributeGlobalConfigsOnSelectedNodesIfRequiredStep                (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext);
                void                distributeGlobalConfigsOnSelectedNodesIfRequiredStepCallback        (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                validateIfClusterFormationInProgressStep                            (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext);
                void                prepareClusterModeTransitionStep                                    (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext);
                void                prepareClusterModeTransitionStepCallback                            (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                sendControlledFailoverMessageToSecondariesInModeTransition          (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext);
                void                sendControlledFailoverMessageToSecondariesInModeTransitionCallback  (FrameworkStatus frameworkStatus, FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage, void *pContext);
                void                resetPrimaryInClusterTransitionStep                                 (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext);
                void                resetPrimaryInClusterTransitionStepCallback                         (FrameworkStatus frameworkStatus, FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage, void *pContext);
                void                setStartupConfigAndRebootStep                                       (VcsFabConfigDistributeSequencerContext *pVcsFabConfigDistributeSequencerContext);

    protected :
    public :
        virtual                    ~VcsFabConfigDistributionWorker ();

    // Now the data memebers

    private :
    protected :
    public :
        friend class VcsFabObjectManager;
};

}

#endif // VCSFABCONFIGDISTRIBUTIONWORKER_H
