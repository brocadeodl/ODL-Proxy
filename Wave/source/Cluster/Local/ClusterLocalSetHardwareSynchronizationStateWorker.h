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
 *@file ClusterLocalSetHardwareSynchronizationStateWorker.h
 * Copyright (C) 2011 BrocadeCommunications Systems,Inc.
 * All rights reserved. 
 *
 * Description: This file declares the
 *              Worker class which is responsible for updating
 *              the hardware synchronization state attribute
 *              in the WaveNode Managed Object. 
 *
 * Author :     Brian Adaniya
 * Date :       05/05/2011
 */

#ifndef CLUSTERLOCALSETHARDWARESYNCHRONIZATIONSTATEWORKER_H
#define CLUSTERLOCALSETHARDWARESYNCHRONIZATIONSTATEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class ClusterLocalObjectManager;
class ClusterLocalSetHardwareSynchronizationStateMessage;
class PrismLinearSequencerContext;

class ClusterLocalSetHardwareSynchronizationStateWorker : public WaveWorker
{
    private :
                void            setHardwareSynchronizationStateMessageHandler       (ClusterLocalSetHardwareSynchronizationStateMessage *pClusterLocalSetHardwareSynchronizationStateMessage);
                void            updateWaveNodeManagedObjectStep                     (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                PrismMessage   *createMessageInstance                               (const UI32 &operationCode);

    protected :
    public :
                                ClusterLocalSetHardwareSynchronizationStateWorker   (ClusterLocalObjectManager *pClusterLocalObjectManager);
        virtual                ~ClusterLocalSetHardwareSynchronizationStateWorker   ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // CLUSTERLOCALSETHARDWARESYNCHRONIZATIONSTATEWORKER_H
