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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef PRISMEXTERNALSTATESYNCHRONIZATIONAGENT_H
#define PRISMEXTERNALSTATESYNCHRONIZATIONAGENT_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"

namespace WaveNs
{

class PrismExternalStateSynchronizationAgentContext;

class PrismExternalStateSynchronizationAgent : protected WaveWorker
{
    private :
    protected :
    public :
                            PrismExternalStateSynchronizationAgent      (WaveObjectManager *pWaveObjectManager, UI32 fssStageNumber, ResourceId serviceType);
        virtual            ~PrismExternalStateSynchronizationAgent      ();
        virtual ResourceId  execute                                     ();

                ResourceId  getListOfEnabledServicesStep                (PrismExternalStateSynchronizationAgentContext *pPrismExternalStateSynchronizationAgentContext);
                ResourceId  externalStateSynchronizationServicesStep    (PrismExternalStateSynchronizationAgentContext *pPrismExternalStateSynchronizationAgentContext);

                bool        requiresExternalStateSynchronization        (const PrismServiceId &prismServiceId);

    // Now the data members
    private :
                UI32        m_fssStageNumber;
                ResourceId  m_serviceType;
    protected :
    public :
};

}

#endif // PRISMEXTERNALSTATESYNCHRONIZATIONAGENT_H
