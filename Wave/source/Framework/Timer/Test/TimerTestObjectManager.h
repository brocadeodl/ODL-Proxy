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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef TIMERTESTOBJECTMANAGER_H
#define TIMERTESTOBJECTMANAGER_H

#include "Framework/Timer/Test/FrameworkTimerTestContexts.h"
#include "Regression/PrismTestObjectManager.h"
#include "Framework/Timer/TimerMessages.h"
#include <time.h>

namespace WaveNs
{
#define                 MAX_TEST_TIMERS                         100000
#define                 MAX_TEST_PERIODIC_TIMERS                1000
class TimerTestData;

class TimerTestObjectManager : public PrismTestObjectManager
{
    private :
                                TimerTestObjectManager          ();
                    void        showPendingTimers               (TimerTestData *pTimerTestInfo);
                    void        timerShowStatStep               (FrameworkTimerTestContext *pFrameworkTimerTestContext);
                    void        timerAccuracyTestStep           (FrameworkTimerTestContext *pFrameworkTimerTestContext);
                    void        timerCancelTestStep             (FrameworkTimerTestContext *pFrameworkTimerTestContext);
                    void        periodicTimerCancelTestStep     (FrameworkTimerTestContext *pFrameworkTimerTestContext);
                    void        periodicTimerAccuracyTestStep   (FrameworkTimerTestContext *pFrameworkTimerTestContext);
                    void        timerExpiredCallback            (TimerHandle timerHandle, TimerTestData * pTimerTestInfo);
                    void        cancelTimerTestCallback         (TimerHandle timerHandle, TimerTestData * pTimerTestInfo);
                    void        showPendingTimers               ();

    protected :
    public :
        virtual                        ~TimerTestObjectManager ();
        static  TimerTestObjectManager *getInstance            ();
        static  PrismServiceId          getPrismServiceId      ();
                void                    testRequestHandler     (RegressionTestMessage *pMessage);
    // Now the data members

    private :
        vector<TimerTestData *> m_timerTestList;
        timeval                 m_maxDelay;
        timeval                 m_endSendTime;

    protected :
    public :
};

class TimerTestData
{
    private :
    protected :
    public :
                    TimerTestData             ();
                   ~TimerTestData             ();
        // Now the data members

    private :
    protected :
    public :
                    FrameworkTimerTestContext          *m_pFrameworkTimerTestContext;
                    timeval                             m_startInterval;
                    timeval                             m_periodicInterval;
                    timeval                             m_startTime;
                    timeval                             m_endTime;
                    timeval                             m_expirationTime;
                    UI32                                m_timerId;
                    UI32                                m_nTimes;
};


}

#endif // TIMERTESTOBJECTMANAGER_H
