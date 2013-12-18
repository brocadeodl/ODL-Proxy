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

#include "Framework/Timer/TimerSignalObjectManager.h"
#include "Framework/Timer/TimerWorker.h"
#include "Framework/Utils/AssertUtils.h"
#include <signal.h>
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

namespace WaveNs
{

TimerSignalObjectManager::TimerSignalObjectManager ()
    : WaveLocalObjectManagerForUserSpecificTasks ("Framework Timer Signal")
{
}

TimerSignalObjectManager *TimerSignalObjectManager::getInstance ()
{
    static TimerSignalObjectManager *pTimerSignalObjectManager = NULL;

    if (NULL == pTimerSignalObjectManager)
    {
        pTimerSignalObjectManager = new TimerSignalObjectManager ();
        WaveNs::prismAssert (NULL != pTimerSignalObjectManager, __FILE__, __LINE__);
    }

    return (pTimerSignalObjectManager);
}

TimerSignalObjectManager::~TimerSignalObjectManager ()
{

}

PrismServiceId TimerSignalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void TimerSignalObjectManager::catchTimerAlarm ()
{
    TimerWorker::processTimeOut ();
}

void TimerSignalObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "TimerSignalObjectManager::boot : Entering ...");

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();

	prismCreateWaitableTimer ();

    while (1)
    {
        prismWaitForAlarmSignal ();
        catchTimerAlarm ();
    }
}

}
