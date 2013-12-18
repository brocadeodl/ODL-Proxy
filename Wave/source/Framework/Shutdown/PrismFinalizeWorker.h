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

#ifndef PRISMFINALIZEWORKER_H
#define PRISMFINALIZEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
namespace WaveNs
{

// Services shutdown steps will be based on the operation.
typedef enum
{
    WAVE_SHUTDOWN_SECONDARY_CONFIGURE = 0,
    WAVE_SHUTDOWN_SECONDARY_REJOIN,
    WAVE_SHUTDOWN_SECONDARY_ROLLBACK,
    WAVE_SHUTDOWN_STANDBY_CONFIGURE
} PrismShutdownMode;

class PrismShutdownAgent;
class PrismFinalizeWorkerShutdownServicesContext;

class PrismFinalizeWorker : protected WaveWorker
{
    private :
        ResourceId chooseAShutdownAgentStep      (PrismFinalizeWorkerShutdownServicesContext *pPrismFinalizeWorkerShutdownServicesContext);
        ResourceId runTheShutdownAgentStep       (PrismFinalizeWorkerShutdownServicesContext *pPrismFinalizeWorkerShutdownServicesContext);
        ResourceId destroyAShutdownAgentStep     (PrismFinalizeWorkerShutdownServicesContext *pPrismFinalizeWorkerShutdownServicesContext);

    protected :
    public :
                            PrismFinalizeWorker   (WaveObjectManager *pWaveObjectManager);
                virtual    ~PrismFinalizeWorker   ();
                ResourceId  shutdownPrismServices (const PrismShutdownMode &prismShutdownMode = WAVE_SHUTDOWN_SECONDARY_CONFIGURE);

    // Now the data members

    private :
        PrismShutdownAgent  *m_pPrismShutdownAgent;

    protected :
    public :
};

}

#endif // PRISMFINALIZEWORKER_H
