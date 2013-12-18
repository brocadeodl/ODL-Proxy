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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMFRAMEWORKCONFIGURATIONWORKER_H
#define PRISMFRAMEWORKCONFIGURATIONWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"

namespace WaveNs
{

class PrismFrameworkConfigurationContext;

class PrismFrameworkConfigurationWorker : public WaveWorker
{
    private :
        ResourceId validateStep                (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext);
        ResourceId initializeConfigurationStep (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext);
        ResourceId saveConfigurationStep       (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext);
        ResourceId loadConfigurationStep       (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext);
        ResourceId displayConfigurationStep    (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext);
        ResourceId sendConfigurationToStandbyStep    (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext);
        ResourceId resetStartupFileStep              (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext);
        ResourceId updatePrismConfigurationFileStep  (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext);
        ResourceId setInMemoryConfiguration          (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext);
        void       setPrismConfigurationValidity     (const bool &validity);
        void       createLockFileForConfigurationFile( ); 
        ResourceId configurationWithFlock ( const UI32 &operationType, PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext = NULL);
        void       disableLiveSync                   ();

    protected :
    public :
                            PrismFrameworkConfigurationWorker (WaveObjectManager *pWaveObjectManager);
        virtual            ~PrismFrameworkConfigurationWorker ();
                ResourceId  savePrismConfiguration            (const string &prismConfigurationFile, const bool &syncToStandby);
                ResourceId  loadPrismConfiguration            (const string &prismConfigurationFile);
                bool        getPrismConfigurationValidity     ();
                ResourceId  changePrismConfigurationValidity  (const bool &validity );      
                PrismMessage *createMessageInstance           (const UI32 &operationCode);
                ResourceId  syncPrismConfigurationMessageHandler (FrameworkObjectManagerSyncConfigurationMessage *pFrameworkObjectManagerSyncConfigurationMessage);      

    // Now the data members

    private :
        bool        m_isPrismConfigurationValid ;
        PrismMutex  m_prismConfigurationValidityMutex;
 
    protected :
    public :
};

}

#endif // PRISMFRAMEWORKCONFIGURATIONWORKER_H
