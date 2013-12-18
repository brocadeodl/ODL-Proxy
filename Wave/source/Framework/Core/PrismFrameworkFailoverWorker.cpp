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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/PrismFrameworkFailoverWorker.h"
#include "PrismFrameworkFailoverWorkerContext.h"
#include "Framework/Core/PrismFrameworkFailoverWorkerSequencerContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Failover/PrismFailoverAgent.h"
#include "Framework/Failover/PrismFailoverAgentContext.h"
#include "Framework/Failover/PrismPrimaryControlledFailoverAgent.h"
#include "Framework/Failover/PrismPrimaryUncontrolledFailoverAgent.h"
#include "Framework/Failover/PrismSecondaryControlledFailoverAgent.h"
#include "Framework/Failover/PrismSecondaryUncontrolledFailoverAgent.h"
#include "Framework/Core/ClusterFailoverCompleteEvent.h"

namespace WaveNs
{

PrismFrameworkFailoverWorker::PrismFrameworkFailoverWorker (WaveObjectManager *pPrismObjectMnager)
    : WaveWorker (pPrismObjectMnager)
{
}

PrismFrameworkFailoverWorker::~PrismFrameworkFailoverWorker ()
{
}

void PrismFrameworkFailoverWorker::executeFailover (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkFailoverWorker::executeFailover : Entering ...");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkFailoverWorker::executeFailoverSelectFailoverAgentStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkFailoverWorker::executeFailoverRunFailoverAgentStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkFailoverWorker::executeFailoverCompleteEventStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkFailoverWorker::executeFailoverDestroyFailoverAgentStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkFailoverWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkFailoverWorker::prismLinearSequencerFailedStep),
    };

    PrismFrameworkFailoverWorkerSequencerContext *pPrismFrameworkFailoverWorkerSequencerContext = new PrismFrameworkFailoverWorkerSequencerContext (pPrismFrameworkFailoverWorkerContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    // Setup the sequencer context with inputs from the context that we recieved.

    pPrismFrameworkFailoverWorkerSequencerContext->setThisLocationRole   (pPrismFrameworkFailoverWorkerContext->getThisLocationRole ());
    pPrismFrameworkFailoverWorkerSequencerContext->setFailoverReason     (pPrismFrameworkFailoverWorkerContext->getFailoverReason ());
    pPrismFrameworkFailoverWorkerSequencerContext->setServiceToBeIgnored (pPrismFrameworkFailoverWorkerContext->getServiceToBeIgnored ());

    UI32 numberOfFailedLocations = pPrismFrameworkFailoverWorkerContext->getNumberOfFailedLocations ();
    UI32 i                       = 0;

    for (i = 0; i < numberOfFailedLocations; i++)
    {
        pPrismFrameworkFailoverWorkerSequencerContext->addFailedLocationId (pPrismFrameworkFailoverWorkerContext->getFailedLocationAtIndex (i));
    }

    pPrismFrameworkFailoverWorkerSequencerContext->holdAll ();
    pPrismFrameworkFailoverWorkerSequencerContext->start ();
}

void PrismFrameworkFailoverWorker::executeFailoverSelectFailoverAgentStep (PrismFrameworkFailoverWorkerSequencerContext *pPrismFrameworkFailoverWorkerSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkFailoverWorker::executeFailoverSelectFailoverAgentStep : Entering ...");

    FrameworkSequenceGenerator &frameworkSequenceGenerator = PrismFrameworkObjectManager::getCurrentFrameworkSequenceGenerator ();
    ResourceId                  status                     = WAVE_MESSAGE_SUCCESS;
    PrismFailoverAgent         *pPrismFailoverAgent        = NULL;

    if (LOCATION_PRIMARY == (pPrismFrameworkFailoverWorkerSequencerContext->getThisLocationRole ()))
    {
        if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == (pPrismFrameworkFailoverWorkerSequencerContext->getFailoverReason ()))
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkFailoverWorker::executeFailoverSelectFailoverAgentStep : Selecting Primary Controlled Failover Agent.");

            pPrismFailoverAgent = new PrismPrimaryControlledFailoverAgent (m_pWaveObjectManager, frameworkSequenceGenerator);

            status = WAVE_MESSAGE_SUCCESS;
        }
        else if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED == (pPrismFrameworkFailoverWorkerSequencerContext->getFailoverReason ()))
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkFailoverWorker::executeFailoverSelectFailoverAgentStep : Selecting Primary Uncontrolled Failover Agent.");

            pPrismFailoverAgent = new PrismPrimaryUncontrolledFailoverAgent (m_pWaveObjectManager, frameworkSequenceGenerator);

            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);

            status = FRAMEWORK_INVALID_FAILOVER_REASON_ON_PRIMARY;
        }
    }
    else if (LOCATION_SECONDARY == (pPrismFrameworkFailoverWorkerSequencerContext->getThisLocationRole ()))
    {
        if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == (pPrismFrameworkFailoverWorkerSequencerContext->getFailoverReason ()))
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkFailoverWorker::executeFailoverSelectFailoverAgentStep : Selecting Secondary Controlled Failover Agent.");

            pPrismFailoverAgent = new PrismSecondaryControlledFailoverAgent (m_pWaveObjectManager, frameworkSequenceGenerator);


            PrismFrameworkFailoverWorkerContext* pPrismFailoverWorkerContext =  static_cast<PrismFrameworkFailoverWorkerContext* > (pPrismFrameworkFailoverWorkerSequencerContext->getPPrismAsynchronousContext ()); 

            if(pPrismFailoverWorkerContext->isSecondaryControlledFailoverDueToPrimaryRemoval() )
            {
               PrismSecondaryControlledFailoverAgent* pPrismSecondaryControlledFailoverAgent = static_cast<PrismSecondaryControlledFailoverAgent* > (pPrismFailoverAgent);
               pPrismSecondaryControlledFailoverAgent->setFailoverDueToPrimaryRemoval (); 
            }
             
            status = WAVE_MESSAGE_SUCCESS;
        }
        else if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED == (pPrismFrameworkFailoverWorkerSequencerContext->getFailoverReason ()))
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkFailoverWorker::executeFailoverSelectFailoverAgentStep : Selecting Secondary Uncontrolled Failover Agent.");

            pPrismFailoverAgent = new PrismSecondaryUncontrolledFailoverAgent (m_pWaveObjectManager, frameworkSequenceGenerator);

            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);

            status = FRAMEWORK_INVALID_FAILOVER_REASON_ON_SECONDARY;
        }
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);

        status = FRAMEWORK_INVALID_LOCATION_ROLE_FOR_FAILOVER;
    }

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        pPrismFrameworkFailoverWorkerSequencerContext->setPPrismFailoverAgent (pPrismFailoverAgent);
    }

    pPrismFrameworkFailoverWorkerSequencerContext->executeNextStep (status);
}

void PrismFrameworkFailoverWorker::executeFailoverRunFailoverAgentStep (PrismFrameworkFailoverWorkerSequencerContext *pPrismFrameworkFailoverWorkerSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkFailoverWorker::executeFailoverRunFailoverAgentStep : Entering ...");

    PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = reinterpret_cast<PrismFrameworkFailoverWorkerContext *> (pPrismFrameworkFailoverWorkerSequencerContext->getPPrismAsynchronousContext ());
    PrismFailoverAgent                  *pPrismFailoverAgent                  = pPrismFrameworkFailoverWorkerSequencerContext->getPPrismFailoverAgent ();
    vector<LocationId>                   failedLocationIds;
    PrismServiceId                       serviceToBeIgnored                   = 0;

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);

    pPrismFrameworkFailoverWorkerContext->getFailedLocationIds (failedLocationIds);
    serviceToBeIgnored = pPrismFrameworkFailoverWorkerSequencerContext->getServiceToBeIgnored ();

    prismAssert (NULL != pPrismFailoverAgent, __FILE__, __LINE__);

    PrismFailoverAgentContext *pPrismFailoverAgentContext = new PrismFailoverAgentContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkFailoverWorker::executeFailoverRunFailoverAgentStepCallback), pPrismFrameworkFailoverWorkerSequencerContext);

    pPrismFailoverAgentContext->setFailedLocationIds(failedLocationIds);
    pPrismFailoverAgentContext->setServiceToBeIgnored(serviceToBeIgnored);
    pPrismFailoverAgentContext->setIsConfigurationChange (pPrismFrameworkFailoverWorkerContext->getIsConfigurationChange ());
    
    pPrismFailoverAgent->execute (pPrismFailoverAgentContext);
}

void PrismFrameworkFailoverWorker::executeFailoverRunFailoverAgentStepCallback (PrismFailoverAgentContext *pPrismFailoverAgentContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkFailoverWorker::executeFailoverRunFailoverAgentStepCallback : Entering ...");

    prismAssert (NULL != pPrismFailoverAgentContext, __FILE__, __LINE__);

    PrismFrameworkFailoverWorkerSequencerContext *pPrismFrameworkFailoverWorkerSequencerContext = reinterpret_cast<PrismFrameworkFailoverWorkerSequencerContext *> (pPrismFailoverAgentContext->getPCallerContext ());

    prismAssert (NULL != pPrismFrameworkFailoverWorkerSequencerContext, __FILE__, __LINE__);

    ResourceId            status                = pPrismFrameworkFailoverWorkerSequencerContext->getCompletionStatus ();

    delete pPrismFailoverAgentContext;

    pPrismFrameworkFailoverWorkerSequencerContext->executeNextStep (status);
}

void PrismFrameworkFailoverWorker::executeFailoverDestroyFailoverAgentStep (PrismFrameworkFailoverWorkerSequencerContext *pPrismFrameworkFailoverWorkerSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkFailoverWorker::executeFailoverDestroyFailoverAgentStep : Entering ...");

    PrismFailoverAgent *pPrismFailoverAgent = pPrismFrameworkFailoverWorkerSequencerContext->getPPrismFailoverAgent ();

    prismAssert (NULL != pPrismFailoverAgent, __FILE__, __LINE__);

    delete (pPrismFailoverAgent);
    pPrismFrameworkFailoverWorkerSequencerContext->setPPrismFailoverAgent (NULL);

    pPrismFrameworkFailoverWorkerSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkFailoverWorker::executeFailoverCompleteEventStep (PrismFrameworkFailoverWorkerSequencerContext *pPrismFrameworkFailoverWorkerSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkFailoverWorker::executeFailoverCompleteEventStep : Entering ...");

    PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = reinterpret_cast<PrismFrameworkFailoverWorkerContext *> (pPrismFrameworkFailoverWorkerSequencerContext->getPPrismAsynchronousContext ());    
    vector<LocationId>                   failedLocationIds;
    FrameworkObjectManagerFailoverReason failoverReason = pPrismFrameworkFailoverWorkerSequencerContext->getFailoverReason ();
    bool                                 isPrincipalChangedWithThisFailover = false;

    pPrismFrameworkFailoverWorkerContext->getFailedLocationIds (failedLocationIds);

    //Check if primary has gone down and accordingly set the principal changed flag
    if (LOCATION_SECONDARY == (pPrismFrameworkFailoverWorkerSequencerContext->getThisLocationRole ()))
    {
        isPrincipalChangedWithThisFailover = true;
    }

    ClusterFailoverCompleteEvent *pClusterFailoverCompleteEvent = new ClusterFailoverCompleteEvent (failoverReason, failedLocationIds, isPrincipalChangedWithThisFailover);
    
    //Broadcast the event
    ResourceId status = broadcast (pClusterFailoverCompleteEvent);

    if(status != WAVE_MESSAGE_SUCCESS)
    {
       trace(TRACE_LEVEL_ERROR, "PrismFrameworkFailoverWorker::executeFailoverCompleteEventStep: Failure during broadcast..");
       pPrismFrameworkFailoverWorkerSequencerContext->executeNextStep (status);
       return;
    }
    
    pPrismFrameworkFailoverWorkerSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

}
