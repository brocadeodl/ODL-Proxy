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
 *   Author : Geetha Aparna                                                *
 ***************************************************************************/

#include "Framework/Shutdown/PrismSecondaryNodeRejoinShutdownAgent.h"
#include "Framework/Shutdown/PrismShutdownAgentContext.h"

namespace WaveNs
{

PrismSecondaryNodeRejoinShutdownAgent::PrismSecondaryNodeRejoinShutdownAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator)
    : PrismShutdownAgent (pWaveObjectManager, currentFrameworkSequenceGenerator)
{
}

PrismSecondaryNodeRejoinShutdownAgent::~PrismSecondaryNodeRejoinShutdownAgent ()
{
}

ResourceId PrismSecondaryNodeRejoinShutdownAgent::execute ()
{
    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismSecondaryNodeRejoinShutdownAgent::getListOfEnabledServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismSecondaryNodeRejoinShutdownAgent::shutdownServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismSecondaryNodeRejoinShutdownAgent::disableServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismSecondaryNodeRejoinShutdownAgent::uninitializeServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismSecondaryNodeRejoinShutdownAgent::shutdownPostPhaseServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismSecondaryNodeRejoinShutdownAgent::disablePostPhaseServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismSecondaryNodeRejoinShutdownAgent::uninitializePostPhaseServicesStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismSecondaryNodeRejoinShutdownAgent::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismSecondaryNodeRejoinShutdownAgent::prismSynchronousLinearSequencerFailedStep)
    };

    PrismShutdownAgentContext *pPrismShutdownAgentContext = new PrismShutdownAgentContext (reinterpret_cast<PrismAsynchronousContext *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    ResourceId status = pPrismShutdownAgentContext->execute ();

    return (status);
}

}

