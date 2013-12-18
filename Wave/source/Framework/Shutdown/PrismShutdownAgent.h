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

#ifndef PRISMSHUTDOWNAGENT_H
#define PRISMSHUTDOWNAGENT_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"

namespace WaveNs
{

class PrismShutdownAgentContext;

class PrismShutdownAgent : protected WaveWorker
{
    private :
    protected :
    public :
                            PrismShutdownAgent                  (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator);
        virtual            ~PrismShutdownAgent                  ();
        virtual ResourceId  execute                             ();

                ResourceId  getListOfEnabledServicesStep        (PrismShutdownAgentContext *pPrismShutdownAgentContext);
                ResourceId  shutdownServicesStep                (PrismShutdownAgentContext *pPrismShutdownAgentContext);
                ResourceId  uninstallServicesStep               (PrismShutdownAgentContext *pPrismShutdownAgentContext);
                ResourceId  uninitializeServicesStep            (PrismShutdownAgentContext *pPrismShutdownAgentContext);
                ResourceId  disableServicesStep                 (PrismShutdownAgentContext *pPrismShutdownAgentContext);
                ResourceId  shutdownPostPhaseServicesStep       (PrismShutdownAgentContext *pPrismShutdownAgentContext);
                ResourceId  uninstallPostPhaseServicesStep      (PrismShutdownAgentContext *pPrismShutdownAgentContext);
                ResourceId  uninitializePostPhaseServicesStep   (PrismShutdownAgentContext *pPrismShutdownAgentContext);
                ResourceId  disablePostPhaseServicesStep        (PrismShutdownAgentContext *pPrismShutdownAgentContext);

                bool        requiresShutdown               (const PrismServiceId &prismServiceId);

    // Now the data members

    private :
        FrameworkSequenceGenerator &m_currentFrameworkSequenceGenerator;

    protected :
    public :
};

}

#endif // PRISMSHUTDOWNAGENT_H
