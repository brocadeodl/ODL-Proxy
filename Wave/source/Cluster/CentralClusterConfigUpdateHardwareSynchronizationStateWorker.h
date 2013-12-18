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
 *@file CentralClusterConfigUpdateHardwareSynchronizationStateWorker.h
 * Copyright (C) 2011 BrocadeCommunications Systems,Inc.
 * All rights reserved. 
 *
 * Description: This file declares the
 *              Worker class which is responsible for updating
 *              the hardware synchronization state attribute
 *              in the WaveNode Managed Object. 
 *
 * Author :     Brian Adaniya
 * Date :       05/13/2011
 */

#ifndef CENTRALCLUSTERCONFIGUPDATEHARDWARESYNCHRONIZATIONWORKER_H
#define CENTRALCLUSTERCONFIGUPDATEHARDWARESYNCHRONIZATIONWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class CentralClusterConfigObjectManager;
class CentralClusterConfigUpdateHardwareSynchronizationStateMessage;
class PrismLinearSequencerContext;
class WaveSendToClusterContext;

class CentralClusterConfigUpdateHardwareSynchronizationStateWorker : public WaveWorker
{
    private :
                void            updateHardwareSynchronizationStateMessageHandler                (CentralClusterConfigUpdateHardwareSynchronizationStateMessage *pCentralClusterConfigUpdateHardwareSynchronizationStateMessage);
                void            sendUpdateWaveNodesToClusterLocalStep                           (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void            sendUpdateWaveNodesToClusterLocalStepCallback                   (WaveSendToClusterContext *pWaveSendToClusterContext);
                PrismMessage   *createMessageInstance                                           (const UI32 &operationCode);
    protected :
    public :
                                CentralClusterConfigUpdateHardwareSynchronizationStateWorker    (CentralClusterConfigObjectManager *pCentralClusterConfigObjectManager);
        virtual                ~CentralClusterConfigUpdateHardwareSynchronizationStateWorker    ();

    // Now the data members
    private :
    protected :
    public :
};

}

#endif // CENTRALCLUSTERCONFIGUPDATEHARDWARESYNCHRONIZATIONWORKER_H
