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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Boot/FirstTimePrismBootAgent.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

FirstTimePrismBootAgent::FirstTimePrismBootAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator)
    : PrismBootAgent (pWaveObjectManager, currentFrameworkSequenceGenerator)
{
}

FirstTimePrismBootAgent::~FirstTimePrismBootAgent ()
{
}

WaveBootReason FirstTimePrismBootAgent::getReason () const
{
    return (WAVE_BOOT_FIRST_TIME_BOOT);
}

ResourceId FirstTimePrismBootAgent::execute (const WaveBootPhase &waveBootPhase)
{
    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::initializePrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::enablePrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::installPrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::bootPrismServicesDuringPrePhaseStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::initializeLocalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::enableLocalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::listenForEventsLocalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::installLocalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::bootLocalPrismServicesStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::initializeGlobalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::enableGlobalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::listenForEventsGlobalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::installGlobalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::bootGlobalPrismServicesStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::savePrismConfigurationStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FirstTimePrismBootAgent::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

    return (status);
}

ResourceId FirstTimePrismBootAgent::savePrismConfigurationStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    if (WAVE_MGMT_INTF_ROLE_SERVER != (FrameworkToolKit::getManagementInterfaceRole ()))
    {
        return (WAVE_MESSAGE_SUCCESS);
    }

    trace (TRACE_LEVEL_INFO,   "FirstTimePrismBootAgent::savePrismConfigurationStep : replay file name " + FrameworkToolKit::getStartupFileName ());

    FrameworkToolKit::setStartupFileType (WAVE_PERSISTENCE_REGULAR_FILE);
    FrameworkToolKit::setIsStartupValid (false);

    ResourceId status = FrameworkToolKit::savePrismConfiguration ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,   "FirstTimePrismBootAgent::savePrismConfigurationStep : Could not save configuration");
    }

    return (status);
}

}
