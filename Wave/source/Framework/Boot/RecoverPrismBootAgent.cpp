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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Mithun B S                                                   *
 ***************************************************************************/

#include "Framework/Boot/RecoverPrismBootAgent.h"
#include "Framework/Boot/FirstTimePrismBootAgent.h"
#include "Framework/Timer/TimerSignalObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/LocationManagement/Location.h"

namespace WaveNs
{

RecoverPrismBootAgent::RecoverPrismBootAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator)
    : FirstTimePrismBootAgent (pWaveObjectManager, currentFrameworkSequenceGenerator),
      m_waveBootPhase (WAVE_BOOT_PHASE_PRE_PHASE)
{
}

RecoverPrismBootAgent::~RecoverPrismBootAgent ()
{
}

ResourceId RecoverPrismBootAgent::execute (const WaveBootPhase &waveBootPhase)
{
    m_waveBootPhase = waveBootPhase;

    if (WAVE_BOOT_PHASE_PRE_PHASE == waveBootPhase)
    {
        WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::shutdownDatabaseStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::updateFrameworkConfigurationToDefaultStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::initializePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::enablePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::installPrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::bootPrismServicesDuringPrePhaseStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

        return (status);
    }
    else if (WAVE_BOOT_PHASE_POST_PHASE == waveBootPhase)
    {
        WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::initializeLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::enableLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::listenForEventsLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::installLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::bootLocalPrismServicesStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::initializeGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::enableGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::listenForEventsGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::installGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::bootGlobalPrismServicesStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::savePrismConfigurationStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RecoverPrismBootAgent::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

        return (status);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId RecoverPrismBootAgent::shutdownDatabaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    bool                                        isShutdownOnReboot  = false;
    DatabaseObjectManagerShutdownMessage       *pMessage            = new DatabaseObjectManagerShutdownMessage (isShutdownOnReboot);

    ResourceId status = sendSynchronously (pMessage, FrameworkToolKit::getThisLocationId ());

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, string ("RecoverPrismBootAgent::shutdownDatabaseStep : Could not send a message to stop Database Server: Status : ") + FrameworkToolKit::localize (status));
        return (status);
    }

    status = pMessage->getCompletionStatus ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, string ("RecoverPrismBootAgent::shutdownDatabaseStep : Could not stop Database Server: Status : ") + FrameworkToolKit::localize (status));
        return (status);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "Database Server Stopped");
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId RecoverPrismBootAgent::updateFrameworkConfigurationToDefaultStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "RecoverPrismBootAgent::updateFrameworkConfigurationToDefaultStep: Reverting the cfg to default values as we go back to First Time boot.");

    PrismFrameworkObjectManager *pTemp         = PrismFrameworkObjectManager::getInstance ();
    pTemp->resetFrameworkConfigurationToDefault ();

    trace (TRACE_LEVEL_INFO, "RecoverPrismBootAgent::updateFrameworkConfigurationToDefaultStep: saving default configuration.");
    FrameworkToolKit::savePrismConfiguration ();

    return (WAVE_MESSAGE_SUCCESS);
}

bool RecoverPrismBootAgent::isToBeExcludedFromInitializeDuringPrePhase (const PrismServiceId &prismServiceId)
{
    if ((TimerSignalObjectManager::getPrismServiceId               ()) == prismServiceId ||
        ((true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))) && (WAVE_BOOT_PHASE_PRE_PHASE == m_waveBootPhase)))
    {   
        return (true);
    }
    else
    {   
        return (false);
    }
}

bool RecoverPrismBootAgent::isToBeExcludedForEnableAndBoot (const PrismServiceId &prismServiceId)
{
    if ((TimerSignalObjectManager::getPrismServiceId               ()) == prismServiceId ||
        ((true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))) && (WAVE_BOOT_PHASE_PRE_PHASE == m_waveBootPhase)))
    {   
        return (true);
    }
    else
    {   
        return (false);
    }
}

bool RecoverPrismBootAgent::isToBeExcludedFromInstallDuringPrePhase (const PrismServiceId &prismServiceId)
{
    if ((TimerSignalObjectManager::getPrismServiceId               ()) == prismServiceId ||
        ((true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))) && (WAVE_BOOT_PHASE_PRE_PHASE == m_waveBootPhase)))
    {   
        return (true);
    }
    else
    {   
        return (false);
    }
}

}
