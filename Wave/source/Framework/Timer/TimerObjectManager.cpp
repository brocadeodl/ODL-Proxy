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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#include "Framework/Timer/TimerObjectManager.h"
#include "Framework/Timer/TimerSignalObjectManager.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

TimerObjectManager::TimerObjectManager ()
    : WaveLocalObjectManager ("Framework Timer")
{
    m_pTimerWorker = new TimerWorker (this);
}

TimerObjectManager *TimerObjectManager::getInstance ()
{
    static TimerObjectManager *pTimerObjectManager = NULL;

    if (NULL == pTimerObjectManager)
    {
        pTimerObjectManager = new TimerObjectManager ();
        WaveNs::prismAssert (NULL != pTimerObjectManager, __FILE__, __LINE__);
    }

    return (pTimerObjectManager);
}

TimerObjectManager::~TimerObjectManager ()
{
    if (NULL != m_pTimerWorker)
    {
        delete m_pTimerWorker;
    }
}

PrismServiceId TimerObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}


void TimerObjectManager::disable (PrismAsynchronousContext *pPrismAsynchronousContext)
{

    trace (TRACE_LEVEL_DEVEL, "TimerObjectManager::disable : Entering ...");

    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();

    m_pTimerWorker->deleteAllTimers ();
}

}
