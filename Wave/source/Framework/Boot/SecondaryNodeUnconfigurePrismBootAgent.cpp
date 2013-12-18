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

#include "Framework/Boot/SecondaryNodeUnconfigurePrismBootAgent.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

SecondaryNodeUnconfigurePrismBootAgent::SecondaryNodeUnconfigurePrismBootAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator)
    : PrismBootAgent (pWaveObjectManager, currentFrameworkSequenceGenerator)
{
}

SecondaryNodeUnconfigurePrismBootAgent::~SecondaryNodeUnconfigurePrismBootAgent ()
{
}

ResourceId SecondaryNodeUnconfigurePrismBootAgent::execute (const WaveBootPhase &waveBootPhase)
{
    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeUnconfigurePrismBootAgent::initializePrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeUnconfigurePrismBootAgent::enablePrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeUnconfigurePrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeUnconfigurePrismBootAgent::bootPrismServicesDuringPrePhaseStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeUnconfigurePrismBootAgent::initializePrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeUnconfigurePrismBootAgent::enablePrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeUnconfigurePrismBootAgent::listenForEventsPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeUnconfigurePrismBootAgent::bootPrismServicesStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeUnconfigurePrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SecondaryNodeUnconfigurePrismBootAgent::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

    return (status);
}

bool SecondaryNodeUnconfigurePrismBootAgent::isAPersistentBoot ()
{
    return (false);
}

bool SecondaryNodeUnconfigurePrismBootAgent::isToBeExcludedForEnableAndBoot (const PrismServiceId &prismServiceId)
{
    if (true == (FrameworkToolKit::isALocalService (prismServiceId)))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool SecondaryNodeUnconfigurePrismBootAgent::isToBeExcludedFromInitializeDuringPrePhase  (const PrismServiceId &prismServiceId)
{
    if (true == (FrameworkToolKit::isALocalService (prismServiceId)))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool SecondaryNodeUnconfigurePrismBootAgent::isToBeExcludedFromInitializePhase (const PrismServiceId &prismServiceId)
{
    if (true == (FrameworkToolKit::isALocalService (prismServiceId)))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool SecondaryNodeUnconfigurePrismBootAgent::willBeAPrimaryLocation ()
{
    return (true);
}

WaveBootReason SecondaryNodeUnconfigurePrismBootAgent::getReason () const
{
    return (WAVE_BOOT_SECONDARY_NODE_UNCONFIGURE_BOOT);
}

}
