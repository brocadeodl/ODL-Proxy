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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveDebugInformationWorker.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveDebugInformationContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForDebugInformation.h"
#include "Framework/ObjectModel/WaveObjectManagerStatisticsTracker.h"
#include "Framework/Types/Types.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

WaveDebugInformationWorker::WaveDebugInformationWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (WAVE_OBJECT_MANAGER_GET_DEBUG_INFORMATION, reinterpret_cast<PrismMessageHandler> (&WaveDebugInformationWorker::getDebugInformationHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_RESET_DEBUG_INFORMATION, reinterpret_cast<PrismMessageHandler> (&WaveDebugInformationWorker::resetDebugInformationHandler));
}

WaveDebugInformationWorker::~WaveDebugInformationWorker ()
{
}

PrismMessage *WaveDebugInformationWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case WAVE_OBJECT_MANAGER_GET_DEBUG_INFORMATION :
            pPrismMessage = new WaveObjectManagerGetDebugInformationMessage;
            prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);
            break;

        case WAVE_OBJECT_MANAGER_RESET_DEBUG_INFORMATION :
            pPrismMessage = new WaveObjectManagerResetDebugInformationMessage;
            prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void WaveDebugInformationWorker::getDebugInformationHandler (WaveObjectManagerGetDebugInformationMessage *pWaveObjectManagerGetDebugInformationMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveDebugInformationWorker::getDebugInformationWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveDebugInformationWorker::getDebugInformationObjectManagerStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveDebugInformationWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveDebugInformationWorker::prismLinearSequencerFailedStep),
    };

    WaveDebugInformationContext *pWaveDebugInformationContext = new WaveDebugInformationContext (pWaveObjectManagerGetDebugInformationMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pWaveDebugInformationContext, __FILE__, __LINE__);


    pWaveDebugInformationContext->holdAll ();
    pWaveDebugInformationContext->start   ();

}

void WaveDebugInformationWorker::getDebugInformationWorkersStep (WaveDebugInformationContext *pWaveDebugInformationContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveDebugInformationWorker::getDebugInformationWorkersStep : Entering ...");

    //WaveObjectManagerGetDebugInformationMessage *pWaveObjectManagerGetDebugInformationMessage = dynamic_cast<WaveObjectManagerGetDebugInformationMessage *> (pWaveDebugInformationContext->getPPrismMessage ());

    WaveObjectManager      *pWaveObjectManager  = getPWaveObjectManager ();
    prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);

    vector<WaveWorker *>    waveWorkers         = pWaveObjectManager->getWorkers ();
    UI32                    numberOfWorkers     = waveWorkers.size ();
    UI32                    i                   = 0;

    ++(*pWaveDebugInformationContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation = new WaveAsynchronousContextForDebugInformation (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveDebugInformationWorker::getDebugInformationWorkersStepCallback), pWaveDebugInformationContext);
        prismAssert (NULL != pWaveAsynchronousContextForDebugInformation, __FILE__, __LINE__);

        ++(*pWaveDebugInformationContext);

        waveWorkers[i]->getDebugInformation (pWaveAsynchronousContextForDebugInformation);
    }

    --(*pWaveDebugInformationContext);

    pWaveDebugInformationContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveDebugInformationWorker::getDebugInformationWorkersStepCallback (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation)
{
    trace (TRACE_LEVEL_DEVEL, "WaveDebugInformationWorker::getDebugInformationWorkersStepCallback : Entering ...");

    WaveDebugInformationContext    *pWaveDebugInformationContext    = reinterpret_cast<WaveDebugInformationContext *> (pWaveAsynchronousContextForDebugInformation->getPCallerContext ());
    prismAssert (NULL != pWaveDebugInformationContext, __FILE__, __LINE__);

    ResourceId                      status                          = pWaveAsynchronousContextForDebugInformation->getCompletionStatus ();


    pWaveDebugInformationContext->appendDebugInformation (pWaveAsynchronousContextForDebugInformation->getDebugInformation ());

    --(*pWaveDebugInformationContext);

    delete pWaveAsynchronousContextForDebugInformation;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveDebugInformationWorker::getDebugInformationWorkersStepCallback : Getting debug information for one worker failed.");

        pWaveDebugInformationContext->incrementNumberOfFailures ();
    }

    if (0 != pWaveDebugInformationContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pWaveDebugInformationContext->executeNextStep (status);
}

void WaveDebugInformationWorker::getDebugInformationObjectManagerStep (WaveDebugInformationContext *pWaveDebugInformationContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveDebugInformationWorker::getDebugInformationObjectManagerStep : Entering ...");

    //WaveObjectManagerGetDebugInformationMessage *pWaveObjectManagerGetDebugInformationMessage = dynamic_cast<WaveObjectManagerGetDebugInformationMessage *> (pWaveDebugInformationContext->getPPrismMessage ());

    WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation = new WaveAsynchronousContextForDebugInformation (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveDebugInformationWorker::getDebugInformationObjectManagerStepCallback), pWaveDebugInformationContext);
    prismAssert (NULL != pWaveAsynchronousContextForDebugInformation, __FILE__, __LINE__);

    WaveObjectManager      *pWaveObjectManager  = getPWaveObjectManager ();
    prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);

    pWaveObjectManager->getDebugInformation (pWaveAsynchronousContextForDebugInformation);
}

void WaveDebugInformationWorker::getDebugInformationObjectManagerStepCallback (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation)
{
    trace (TRACE_LEVEL_DEVEL, "WaveDebugInformationWorker::getDebugInformationObjectManagerStepCallback : Entering ...");

    WaveDebugInformationContext                    *pWaveDebugInformationContext                    = reinterpret_cast<WaveDebugInformationContext *> (pWaveAsynchronousContextForDebugInformation->getPCallerContext ());
    prismAssert (NULL != pWaveDebugInformationContext, __FILE__, __LINE__);

    WaveObjectManagerGetDebugInformationMessage    *pWaveObjectManagerGetDebugInformationMessage    = dynamic_cast<WaveObjectManagerGetDebugInformationMessage *> (pWaveDebugInformationContext->getPPrismMessage ());
    prismAssert (NULL != pWaveObjectManagerGetDebugInformationMessage, __FILE__, __LINE__);

    ResourceId                                      status                                          = pWaveAsynchronousContextForDebugInformation->getCompletionStatus ();

    // First append any Generic Object Manager Statistics that have been implemented by any specific Object Managers.

    pWaveDebugInformationContext->appendDebugInformation (pWaveAsynchronousContextForDebugInformation->getDebugInformation ());

    // Now append Generic Object Manager Statistics for Managed Objects that were created/deleted/updated during a commitTransaction

    WaveObjectManagerStatisticsTracker             *pWaveObjectManagerStatisticsTracker             = getPWaveObjectManagerStatisticsTracker ();
    prismAssert (NULL != pWaveObjectManagerStatisticsTracker, __FILE__, __LINE__);

    string managedObjectDebugInformation = pWaveObjectManagerStatisticsTracker->getAllManagedObjectStatistics ();

    if (0 < managedObjectDebugInformation.size ())
    {
        WaveObjectManager      *pWaveObjectManager  = getPWaveObjectManager ();
        prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);

        managedObjectDebugInformation = "\n(" + pWaveObjectManager->getName () + ") OBJECT MANAGER COMMIT TRANSACTION STATISTICS:\n\n" + managedObjectDebugInformation;

        pWaveDebugInformationContext->appendDebugInformation (managedObjectDebugInformation);
    }

    // No add the timings accumulated for sequencer steps so far across all message handlers

    WaveObjectManager *pWaveObjectManager = getPWaveObjectManager ();

    prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);

    string allTimingsAccumulateForSequencerSteps;

    pWaveObjectManager->getAllTimingsAccumulatedForSequencerSteps (allTimingsAccumulateForSequencerSteps);

    pWaveDebugInformationContext->appendDebugInformation (allTimingsAccumulateForSequencerSteps);

    pWaveObjectManagerGetDebugInformationMessage->setDebugInformation (pWaveDebugInformationContext->getDebugInformation ());

    delete pWaveAsynchronousContextForDebugInformation;
    pWaveDebugInformationContext->executeNextStep (status);
}

void WaveDebugInformationWorker::resetDebugInformationHandler (WaveObjectManagerResetDebugInformationMessage *pWaveObjectManagerResetDebugInformationMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveDebugInformationWorker::resetDebugInformationWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveDebugInformationWorker::resetDebugInformationObjectManagerStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveDebugInformationWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveDebugInformationWorker::prismLinearSequencerFailedStep),
    };

    WaveDebugInformationContext *pWaveDebugInformationContext = new WaveDebugInformationContext (pWaveObjectManagerResetDebugInformationMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pWaveDebugInformationContext, __FILE__, __LINE__);


    pWaveDebugInformationContext->holdAll ();
    pWaveDebugInformationContext->start   ();
}

void WaveDebugInformationWorker::resetDebugInformationWorkersStep (WaveDebugInformationContext *pWaveDebugInformationContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveDebugInformationWorker::resetDebugInformationWorkersStep : Entering ...");

    //WaveObjectManagerResetDebugInformationMessage *pWaveObjectManagerResetDebugInformationMessage = dynamic_cast<WaveObjectManagerResetDebugInformationMessage *> (pWaveDebugInformationContext->getPPrismMessage ());

    WaveObjectManager      *pWaveObjectManager  = getPWaveObjectManager ();
    prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);

    vector<WaveWorker *>    waveWorkers         = pWaveObjectManager->getWorkers ();
    UI32                    numberOfWorkers     = waveWorkers.size ();
    UI32                    i                   = 0;

    ++(*pWaveDebugInformationContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation = new WaveAsynchronousContextForDebugInformation (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveDebugInformationWorker::resetDebugInformationWorkersStepCallback), pWaveDebugInformationContext);
        prismAssert (NULL != pWaveAsynchronousContextForDebugInformation, __FILE__, __LINE__);

        ++(*pWaveDebugInformationContext);
        waveWorkers[i]->resetDebugInformation (pWaveAsynchronousContextForDebugInformation);
    }

    --(*pWaveDebugInformationContext);

    pWaveDebugInformationContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveDebugInformationWorker::resetDebugInformationWorkersStepCallback (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation)
{
    trace (TRACE_LEVEL_DEVEL, "WaveDebugInformationWorker::resetDebugInformationWorkersStepCallback : Entering ...");

    WaveDebugInformationContext    *pWaveDebugInformationContext    = reinterpret_cast<WaveDebugInformationContext *> (pWaveAsynchronousContextForDebugInformation->getPCallerContext ());
    prismAssert (NULL != pWaveDebugInformationContext, __FILE__, __LINE__);

    ResourceId                      status                          = pWaveAsynchronousContextForDebugInformation->getCompletionStatus ();

    --(*pWaveDebugInformationContext);

    delete pWaveAsynchronousContextForDebugInformation;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveDebugInformationWorker::getDebugInformationWorkersStepCallback : Getting debug information for one worker failed.");

        pWaveDebugInformationContext->incrementNumberOfFailures ();
    }

    if (0 != pWaveDebugInformationContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pWaveDebugInformationContext->executeNextStep (status);
}

void WaveDebugInformationWorker::resetDebugInformationObjectManagerStep (WaveDebugInformationContext *pWaveDebugInformationContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveDebugInformationWorker::resetDebugInformationObjectManagerStep : Entering ...");

    //WaveObjectManagerResetDebugInformationMessage *pWaveObjectManagerResetDebugInformationMessage = dynamic_cast<WaveObjectManagerResetDebugInformationMessage *> (pWaveDebugInformationContext->getPPrismMessage ());

    WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation = new WaveAsynchronousContextForDebugInformation (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveDebugInformationWorker::resetDebugInformationObjectManagerStepCallback), pWaveDebugInformationContext);
    prismAssert (NULL != pWaveAsynchronousContextForDebugInformation, __FILE__, __LINE__);

    WaveObjectManager      *pWaveObjectManager  = getPWaveObjectManager ();
    prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);

    pWaveObjectManager->resetDebugInformation (pWaveAsynchronousContextForDebugInformation);
}

void WaveDebugInformationWorker::resetDebugInformationObjectManagerStepCallback (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation)
{
    trace (TRACE_LEVEL_DEVEL, "WaveDebugInformationWorker::resetDebugInformationObjectManagerStepCallback : Entering ...");

    WaveDebugInformationContext        *pWaveDebugInformationContext        = reinterpret_cast<WaveDebugInformationContext *> (pWaveAsynchronousContextForDebugInformation->getPCallerContext ());
    prismAssert (NULL != pWaveDebugInformationContext, __FILE__, __LINE__);

    ResourceId                          status                              = pWaveAsynchronousContextForDebugInformation->getCompletionStatus ();


    // Clear all Generic Managed Object Statistics collected for create/update/delete during a commitTransaction.

    WaveObjectManagerStatisticsTracker *pWaveObjectManagerStatisticsTracker = getPWaveObjectManagerStatisticsTracker ();
    prismAssert (NULL != pWaveObjectManagerStatisticsTracker, __FILE__, __LINE__);

    pWaveObjectManagerStatisticsTracker->clearAllManagedObjectStatistics ();

    WaveObjectManager *pWaveObjectManager = getPWaveObjectManager ();

    prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);

    pWaveObjectManager->clearAllTimingsAccumulatedForSequencerSteps ();

    delete pWaveAsynchronousContextForDebugInformation;
    pWaveDebugInformationContext->executeNextStep (status);
}

}

