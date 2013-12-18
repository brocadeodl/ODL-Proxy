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

#include "Framework/Shutdown/PrismFinalizeWorker.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/PrismFinalizeWorkerShutdownServicesContext.h"
#include "Framework/Shutdown/PrismSecondaryNodeRejoinShutdownAgent.h"
#include "Framework/Shutdown/PrismSecondaryNodeConfigureShutdownAgent.h"

namespace WaveNs
{

PrismFinalizeWorker::PrismFinalizeWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager),
      m_pPrismShutdownAgent (NULL)  
{
}

PrismFinalizeWorker::~PrismFinalizeWorker ()
{
   m_pPrismShutdownAgent = NULL;
}

ResourceId PrismFinalizeWorker::shutdownPrismServices (const PrismShutdownMode &prismShutdownMode)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFinalizeWorker::shutdownPrismServices : Entering ...");

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFinalizeWorker::chooseAShutdownAgentStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFinalizeWorker::runTheShutdownAgentStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFinalizeWorker::destroyAShutdownAgentStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFinalizeWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFinalizeWorker::prismSynchronousLinearSequencerFailedStep),
    };

    PrismFinalizeWorkerShutdownServicesContext *pPrismFinalizeWorkerShutdownServicesContext = new PrismFinalizeWorkerShutdownServicesContext (reinterpret_cast<PrismAsynchronousContext *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismFinalizeWorkerShutdownServicesContext->setPrismShutdownMode (prismShutdownMode);

    ResourceId status = pPrismFinalizeWorkerShutdownServicesContext->execute ();

    return (status);
}

ResourceId PrismFinalizeWorker::chooseAShutdownAgentStep (PrismFinalizeWorkerShutdownServicesContext *pPrismFinalizeWorkerShutdownServicesContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFinalizeWorker::chooseAShutdownAgentStep : Entering ...");

    FrameworkSequenceGenerator &frameworkSequenceGenerator = PrismFrameworkObjectManager::getCurrentFrameworkSequenceGenerator ();
    PrismShutdownMode           prismShutdownMode          = pPrismFinalizeWorkerShutdownServicesContext->getPrismShutdownMode ();

    if ((WAVE_SHUTDOWN_SECONDARY_CONFIGURE == prismShutdownMode) || (WAVE_SHUTDOWN_STANDBY_CONFIGURE == prismShutdownMode))
    {
        m_pPrismShutdownAgent = new PrismSecondaryNodeConfigureShutdownAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
    }
    else if ((WAVE_SHUTDOWN_SECONDARY_REJOIN == prismShutdownMode) || (WAVE_SHUTDOWN_SECONDARY_ROLLBACK == prismShutdownMode))
    {
        m_pPrismShutdownAgent = new PrismSecondaryNodeRejoinShutdownAgent (m_pWaveObjectManager, frameworkSequenceGenerator);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("PrismFinalizeWorker::chooseAShutdownAgentStep : Unknown Prism Shutdown Mode : ") + (UI32) prismShutdownMode);
        prismAssert (false, __FILE__, __LINE__);
    }

    prismAssert (NULL != m_pPrismShutdownAgent, __FILE__, __LINE__);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismFinalizeWorker::runTheShutdownAgentStep (PrismFinalizeWorkerShutdownServicesContext *pPrismFinalizeWorkerShutdownServicesContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFinalizeWorker::runTheShutdownAgentStep : Entering ...");

    ResourceId status = m_pPrismShutdownAgent->execute ();

    return (status);
}

ResourceId PrismFinalizeWorker::destroyAShutdownAgentStep (PrismFinalizeWorkerShutdownServicesContext *pPrismFinalizeWorkerShutdownServicesContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFinalizeWorker::destroyAShutdownAgentStep : Entering ...");

    if (NULL != m_pPrismShutdownAgent)
    {
        delete m_pPrismShutdownAgent;
    }

    m_pPrismShutdownAgent =  NULL;

    return (WAVE_MESSAGE_SUCCESS);
}

}
