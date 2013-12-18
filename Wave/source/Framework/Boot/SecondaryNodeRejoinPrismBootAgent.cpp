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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aashish Akhouri                                    *
 ***************************************************************************/

#include "Framework/Boot/SecondaryNodeRejoinPrismBootAgent.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Core/ApplicationSpecificServices.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{
/// Name
/// SecondaryNodeRejoinPrismBootAgent
/// Description
/// Constructor
/// Input
/// WaveObjectManager: Owner object manager
/// FrameworkSequenceGenerator: Sequence of services to be booted for 
/// rejoin in pre and post boot phase 
/// Output
/// none
/// Return
/// none
SecondaryNodeRejoinPrismBootAgent::SecondaryNodeRejoinPrismBootAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator)
    : PrismBootAgent (pWaveObjectManager, currentFrameworkSequenceGenerator),
      m_waveBootPhase (WAVE_BOOT_PHASE_ALL_PHASES)
{
}
/// Name
/// SecondaryNodeRejoinPrismBootAgent
/// Description
/// Destructor
/// Input
/// None
/// Output
/// none
/// Return
/// none
SecondaryNodeRejoinPrismBootAgent::~SecondaryNodeRejoinPrismBootAgent ()
{
}
/// Name
/// execute
/// Description
/// Calls the sequence of functions defines in the 
/// base class to execute the pre and post boot
/// Input
/// const WaveBootPhase : Pre(Phase1 of rejoin) or Post(Phase2 of rejoin)
/// Output
/// none
/// Return
/// none
ResourceId SecondaryNodeRejoinPrismBootAgent::execute (const WaveBootPhase &waveBootPhase)
{
    m_waveBootPhase = waveBootPhase;

    if (WAVE_BOOT_PHASE_PRE_PHASE == waveBootPhase)
    {
        trace (TRACE_LEVEL_DEBUG, "SecondaryNodeRejoinPrismBootAgent::execute : Pre Phase.");

        WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::initializePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::enablePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::bootPrismServicesDuringPrePhaseStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

        return (status);
    }
    else if (WAVE_BOOT_PHASE_POST_PHASE == waveBootPhase)
    {
        trace (TRACE_LEVEL_INFO, "SecondaryNodeRejoinPrismBootAgent::execute : Post Phase.");

        WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::initializeLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::enablePrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::listenForEventsPrismServicesStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::bootPrismServicesStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

        return (status);
    }
    else if (WAVE_BOOT_PHASE_AFTER_POST_PHASE == waveBootPhase)
    {
        trace (TRACE_LEVEL_INFO, "SecondaryNodeRejoinPrismBootAgent::execute : Phase 3");
        WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::initializeLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::enablePrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::listenForEventsPrismServicesStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::bootPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::backendSyncUpStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeRejoinPrismBootAgent::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

        return (status);
    }

    return (WAVE_MESSAGE_SUCCESS);
}
/// Name
/// execute
/// Description
/// Whether rejoin boot is persistent: QUESTION: SAGAR??
/// Input
/// None
/// Output
/// none
/// Return
/// bool: 
bool SecondaryNodeRejoinPrismBootAgent::isAPersistentBoot ()
{
    return (true);
}

/// Name
/// isToBeExcludedForEnableAndBoot
/// Description
/// QUESTION:SAGAR
/// Input
/// const PrismServiceId
/// Output
/// none
/// Return
/// bool:
bool SecondaryNodeRejoinPrismBootAgent::isToBeExcludedFromInitializeDuringPrePhase (const PrismServiceId &prismServiceId)
{
    if (((PrismFrameworkObjectManager::getPrismServiceId               ()) == prismServiceId) ||
        (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool SecondaryNodeRejoinPrismBootAgent::isToBeExcludedFromInitializePhase (const PrismServiceId &prismServiceId)
{
    if (((PrismFrameworkObjectManager::getPrismServiceId               ()) == prismServiceId) ||
        (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool SecondaryNodeRejoinPrismBootAgent::isToBeExcludedForEnableAndBoot (const PrismServiceId& prismServiceId)
{
    if (((PrismFrameworkObjectManager::getPrismServiceId               ()) == prismServiceId) ||
        (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}
/// Name
/// willBeAPrimaryLocation
/// Description
/// This function will always return false because 
/// the rejoining node will not be a primary location.
/// Input
/// none
/// Output
/// none
/// Return
/// bool:Always false
bool SecondaryNodeRejoinPrismBootAgent::willBeAPrimaryLocation ()
{
    return (false);
}


bool SecondaryNodeRejoinPrismBootAgent::isToBeExcludedFromCurrentBootPhase (PrismServiceId prismServiceId)
{
    ApplicationSpecificServices *pApplicationSpecificServices = ApplicationSpecificServices::getInstance ();

    if ((WAVE_BOOT_PHASE_POST_PHASE == m_waveBootPhase) && (true == pApplicationSpecificServices->isApplicationSpecificServiceId (prismServiceId)))
    {
        return (true);
    }
    else if ((WAVE_BOOT_PHASE_AFTER_POST_PHASE == m_waveBootPhase) && (false == pApplicationSpecificServices->isApplicationSpecificServiceId (prismServiceId)))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}



/// Name
/// getReason
/// Description
/// Returns reason for boot , which is Rejoin
/// Input
/// none
/// Output
/// none
/// Return
/// WaveBootReason: Rejoin of secondary node

WaveBootReason SecondaryNodeRejoinPrismBootAgent::getReason () const
{
    return (WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT);
}

}
