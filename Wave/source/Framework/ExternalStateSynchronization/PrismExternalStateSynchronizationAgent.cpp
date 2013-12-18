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

#include "Framework/ExternalStateSynchronization/PrismExternalStateSynchronizationAgent.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/ExternalStateSynchronization/PrismExternalStateSynchronizationAgentContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

namespace WaveNs
{

PrismExternalStateSynchronizationAgent::PrismExternalStateSynchronizationAgent (WaveObjectManager *pWaveObjectManager, UI32 fssStageNumber, ResourceId serviceType)
    : WaveWorker (pWaveObjectManager),
      m_fssStageNumber (fssStageNumber),
      m_serviceType (serviceType)    
{
}

PrismExternalStateSynchronizationAgent::~PrismExternalStateSynchronizationAgent ()
{
}

ResourceId PrismExternalStateSynchronizationAgent::execute ()
{
    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismExternalStateSynchronizationAgent::getListOfEnabledServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismExternalStateSynchronizationAgent::externalStateSynchronizationServicesStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismExternalStateSynchronizationAgent::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismExternalStateSynchronizationAgent::prismSynchronousLinearSequencerFailedStep)
    };

    PrismExternalStateSynchronizationAgentContext *pPrismExternalStateSynchronizationAgentContext = new PrismExternalStateSynchronizationAgentContext (reinterpret_cast<PrismAsynchronousContext *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    ResourceId status = pPrismExternalStateSynchronizationAgentContext->execute ();

    return (status);
}

ResourceId PrismExternalStateSynchronizationAgent::getListOfEnabledServicesStep (PrismExternalStateSynchronizationAgentContext *pPrismExternalStateSynchronizationAgentContext)
{
    vector<PrismServiceId> &enabledServices = pPrismExternalStateSynchronizationAgentContext->getEnabledServices ();

    WaveObjectManager::getListOfEnabledServices (enabledServices);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismExternalStateSynchronizationAgent::externalStateSynchronizationServicesStep (PrismExternalStateSynchronizationAgentContext *pPrismExternalStateSynchronizationAgentContext)
{
    vector<PrismServiceId> &serviceIdsToExternalStateSynchronization = pPrismExternalStateSynchronizationAgentContext->getEnabledServices ();
    UI32                    i                    = 0;
    UI32                    numberOfServices     = 0;

    numberOfServices = serviceIdsToExternalStateSynchronization.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (requiresExternalStateSynchronization (serviceIdsToExternalStateSynchronization[i])))
        {
            continue;
        }

        PrismExternalStateSynchronizationObjectManagerMessage prismExternalStateSynchronizationObjectManagerMessage (serviceIdsToExternalStateSynchronization[i], m_fssStageNumber, m_serviceType);

        ResourceId status = sendSynchronously (&prismExternalStateSynchronizationObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismBootAgent::ExternalStateSynchronizationServicesStep : Could not ExternalStateSynchronization a service : " + FrameworkToolKit::getServiceNameById (serviceIdsToExternalStateSynchronization[i]));
            prismAssert (false, __FILE__, __LINE__);
            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, "ExternalStateSynchronization " + FrameworkToolKit::getServiceNameById (serviceIdsToExternalStateSynchronization[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}
                
bool PrismExternalStateSynchronizationAgent::requiresExternalStateSynchronization (const PrismServiceId &prismServiceId)
{
    if (((PrismFrameworkObjectManager::getPrismServiceId               ()) == prismServiceId) ||
        (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))) ||
        (false == PrismFrameworkObjectManager::isExternalStateSynchronizationRequired (prismServiceId)))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

}
