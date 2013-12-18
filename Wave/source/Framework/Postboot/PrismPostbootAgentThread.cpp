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
 *   Author : Jitendra Singh                                                *
 ***************************************************************************/

#include "Framework/Postboot/PrismPostbootAgent.h"
#include "Framework/Postboot/PrismPostbootAgentThread.h"
#include "Framework/Postboot/PrismPostbootAgentThreadContext.h"
#include "Framework/Postboot/PrismPostPersistentBootMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Utils/PrismMutex.h"

namespace WaveNs
{

    UI32        PrismPostbootAgentThread::m_parallelPostBootAgentThreads;
    PrismMutex  PrismPostbootAgentThread::m_parallelPostBootAgentThreadsMutex;

PrismPostbootAgentThread::PrismPostbootAgentThread   (WaveObjectManager *pWaveObjectManager, const UI32 &event, const UI32 &parameter, 
                                                      const ResourceId &recoveryType, PrismPostbootAgentThreadContext *pPrismPostbootAgentThreadContext)
    : WaveWorker (pWaveObjectManager, false),
    PrismPosixThread (),
    m_eventId    (event),
    m_parameter  (parameter),
    m_recoveryType (recoveryType),
    m_pPrismPostbootAgentThreadContext (pPrismPostbootAgentThreadContext)
    
{
    FrameworkToolKit::incrementLineCardPostBootCount ();

    m_parallelPostBootAgentThreadsMutex.lock();
    m_parallelPostBootAgentThreads++;
    m_parallelPostBootAgentThreadsMutex.unlock();
}

PrismPostbootAgentThread::~PrismPostbootAgentThread ()
{
    m_parallelPostBootAgentThreadsMutex.lock();
    m_parallelPostBootAgentThreads--;

    if ( ( 0 == m_parallelPostBootAgentThreads ) && ( NULL != m_pPrismPostbootAgentThreadContext ) )
    {
        if ( ( NULL == m_pPrismPostbootAgentThreadContext->getPostbootMessagePointer() ) && ( NULL != m_pPrismPostbootAgentThreadContext->getPostbootMutex()) )
        {
            (m_pPrismPostbootAgentThreadContext->getPostbootMutex())->lock();
            (m_pPrismPostbootAgentThreadContext->getPostbootSynchronizingCondition())->resume();
            (m_pPrismPostbootAgentThreadContext->getPostbootMutex())->unlock();
        }
    }

    delete (m_pPrismPostbootAgentThreadContext);

    m_parallelPostBootAgentThreadsMutex.unlock();

    FrameworkToolKit::decrementLineCardPostBootCount ();
}

WaveThreadStatus PrismPostbootAgentThread::start ()
{
    trace (TRACE_LEVEL_PERF_START, "LC Postboot.");
    PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage = m_pPrismPostbootAgentThreadContext->getPostbootMessagePointer ();

    PrismPostbootAgent *pPrismPostbootAgent = new PrismPostbootAgent(m_pWaveObjectManager, m_eventId, m_parameter, m_recoveryType);
    prismAssert (NULL != pPrismPostbootAgent, __FILE__, __LINE__);
    ResourceId status = pPrismPostbootAgent->execute ();
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("PrismPostbootAgentThread::start: PrismPostbootAgent execute returned error: ")+FrameworkToolKit::localize (status));
    }
    else
    {
        trace (TRACE_LEVEL_SUCCESS, string ("PrismPostbootAgentThread::start: PrismPostbootAgent execute succeeded"));
    }
   
    if ( NULL != pPrismPostPersistenceBootMessage )
    {    
        pPrismPostPersistenceBootMessage->setCompletionStatus (status);
        reply (pPrismPostPersistenceBootMessage);
    }

    trace (TRACE_LEVEL_PERF_END, "LC Postboot.");

    delete (pPrismPostbootAgent);
    delete (this);
    return (WAVE_THREAD_SUCCESS);
}
}

