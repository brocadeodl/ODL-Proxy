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

#include "Framework/Timer/Test/FrameworkTimerTestContexts.h"
#include "Framework/Timer/Test/TimerTestObjectManager.h"
#include "Framework/Core/Test/FrameworkTestabilityMessages.h"
#include "Framework/Types/Types.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Timer/TimerWorker.h"

namespace WaveNs
{

TimerTestObjectManager::TimerTestObjectManager ()
    : PrismTestObjectManager ("Framwork Timer Test")
{
}

TimerTestObjectManager *TimerTestObjectManager::getInstance ()
{
    static TimerTestObjectManager *pTimerTestObjectManager = NULL;

    if (NULL == pTimerTestObjectManager)
    {
        pTimerTestObjectManager = new TimerTestObjectManager ();
        WaveNs::prismAssert (NULL != pTimerTestObjectManager, __FILE__, __LINE__);
    }

    return (pTimerTestObjectManager);
}

TimerTestObjectManager::~TimerTestObjectManager ()
{
}

PrismServiceId TimerTestObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void TimerTestObjectManager::testRequestHandler (RegressionTestMessage *pMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&TimerTestObjectManager::timerAccuracyTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&TimerTestObjectManager::timerShowStatStep),
        reinterpret_cast<PrismLinearSequencerStep> (&TimerTestObjectManager::periodicTimerAccuracyTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&TimerTestObjectManager::timerShowStatStep),
        reinterpret_cast<PrismLinearSequencerStep> (&TimerTestObjectManager::timerCancelTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&TimerTestObjectManager::periodicTimerCancelTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&TimerTestObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&TimerTestObjectManager::prismLinearSequencerFailedStep),
    };

    FrameworkTimerTestContext *pFrameworkTimerTestContext = new FrameworkTimerTestContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pFrameworkTimerTestContext->start ();
}

void TimerTestObjectManager::timerAccuracyTestStep (FrameworkTimerTestContext *pFrameworkTimerTestContext)
{
    TimerTestData                                  *pTimerTestInfo;
    UI32                                            i;
    ResourceId                                      retCode;

    TimerWorker::resetMaxDelay ();
    m_maxDelay.tv_sec   = 0;
    m_maxDelay.tv_usec  = 0;
    srand (time(NULL));
    ++(*pFrameworkTimerTestContext);
    trace (TRACE_LEVEL_INFO, string ("Timer Accuracy. Test takes about 2min. Starting ") +  MAX_TEST_TIMERS + " timers and run them for random intervals.");

    for (i = 0; i < MAX_TEST_TIMERS; i++)
    {
        pTimerTestInfo = new TimerTestData ();
        pTimerTestInfo->m_pFrameworkTimerTestContext = pFrameworkTimerTestContext;

        pTimerTestInfo->m_startInterval.tv_sec      =   rand () % 10;
        pTimerTestInfo->m_startInterval.tv_usec     =   rand () % 999999 + 1;
        pTimerTestInfo->m_periodicInterval.tv_sec   =   0;
        pTimerTestInfo->m_periodicInterval.tv_usec  =   0;
        pTimerTestInfo->m_nTimes                    =   0;

        gettimeofday (&pTimerTestInfo->m_startTime, NULL);

        retCode = startTimer (pTimerTestInfo->m_timerId, pTimerTestInfo->m_startInterval, pTimerTestInfo->m_periodicInterval, reinterpret_cast<PrismTimerExpirationHandler> (&TimerTestObjectManager::timerExpiredCallback), (void*) pTimerTestInfo);

        if (FRAMEWORK_SUCCESS != retCode)
        {
            pFrameworkTimerTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("TimerTestObjectManager::testRequestHandler: startTimerMessage FAILED.") + retCode + string ("Timer:") + pTimerTestInfo->m_startInterval.tv_sec + string (".") + pTimerTestInfo->m_startInterval.tv_usec);

            delete (pTimerTestInfo);
            continue;
        }
        else
        {
            ++(*pFrameworkTimerTestContext);
        }

        pTimerTestInfo->m_expirationTime.tv_sec  =   pTimerTestInfo->m_startTime.tv_sec  + pTimerTestInfo->m_startInterval.tv_sec;
        pTimerTestInfo->m_expirationTime.tv_usec =   pTimerTestInfo->m_startTime.tv_usec + pTimerTestInfo->m_startInterval.tv_usec;
        if (pTimerTestInfo->m_expirationTime.tv_usec >= 1000000)
        {
            pTimerTestInfo->m_expirationTime.tv_usec -= 1000000;
            pTimerTestInfo->m_expirationTime.tv_sec++;
        }


     }
     gettimeofday (&m_endSendTime, NULL);
     trace (TRACE_LEVEL_INFO, "Timer Accuracy. Sent All timer messages and now waiting for timers to expire.");

     --(*pFrameworkTimerTestContext);
    pFrameworkTimerTestContext->executeNextStep (((pFrameworkTimerTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void TimerTestObjectManager::periodicTimerAccuracyTestStep (FrameworkTimerTestContext *pFrameworkTimerTestContext)
{
    TimerTestData                                  *pTimerTestInfo;
    UI32                                            i;
    ResourceId                                      retCode;

    TimerWorker::resetMaxDelay ();
    m_maxDelay.tv_sec   = 0;
    m_maxDelay.tv_usec  = 0;
    srand (time(NULL));
    ++(*pFrameworkTimerTestContext);
    trace (TRACE_LEVEL_INFO, string ("Timer Periodic Accuracy. Test takes long time. Starting ") + MAX_TEST_PERIODIC_TIMERS + " timers and run them periodically for random periods.");

    for (i=0; i< MAX_TEST_PERIODIC_TIMERS; i++)
    {
        pTimerTestInfo = new TimerTestData ();
        pTimerTestInfo->m_pFrameworkTimerTestContext = pFrameworkTimerTestContext;

        pTimerTestInfo->m_startInterval.tv_sec      =   rand () % 10;
        pTimerTestInfo->m_startInterval.tv_usec     =   rand () % 999999 + 1;
        pTimerTestInfo->m_periodicInterval.tv_sec   =   rand () % 10;
        pTimerTestInfo->m_periodicInterval.tv_usec  =   rand () % 1000000;
        pTimerTestInfo->m_nTimes                    =   rand () % 10;

        gettimeofday (&(pTimerTestInfo->m_startTime), NULL);

        retCode = startTimer (pTimerTestInfo->m_timerId, pTimerTestInfo->m_startInterval, pTimerTestInfo->m_periodicInterval, reinterpret_cast<PrismTimerExpirationHandler> (&TimerTestObjectManager::timerExpiredCallback), (void*) pTimerTestInfo);

        if (FRAMEWORK_SUCCESS != retCode)
        {
            pFrameworkTimerTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, "TimerTestObjectManager::testRequestHandler: startTimerMessage FAILED.");
        }
        else
        {
            ++(*pFrameworkTimerTestContext);
        }

        pTimerTestInfo->m_expirationTime.tv_sec  =   pTimerTestInfo->m_startTime.tv_sec  + pTimerTestInfo->m_startInterval.tv_sec;
        pTimerTestInfo->m_expirationTime.tv_usec =   pTimerTestInfo->m_startTime.tv_usec + pTimerTestInfo->m_startInterval.tv_usec;
        if (pTimerTestInfo->m_expirationTime.tv_usec >= 1000000)
        {
            pTimerTestInfo->m_expirationTime.tv_usec -= 1000000;
            pTimerTestInfo->m_expirationTime.tv_sec++;
        }

    }

    trace (TRACE_LEVEL_INFO, "Timer Periodic Accuracy. Sent All timer messages and now waiting for timers to expire.");
    gettimeofday (&m_endSendTime, NULL);

    --(*pFrameworkTimerTestContext);
    pFrameworkTimerTestContext->executeNextStep (((pFrameworkTimerTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}



void TimerTestObjectManager::timerExpiredCallback (TimerHandle timerHandle, TimerTestData * pTimerTestInfo)
{
    if ((NULL == pTimerTestInfo) || (0 == pTimerTestInfo->m_timerId) || (pTimerTestInfo->m_timerId != timerHandle))
    {
        return;
    }

    FrameworkTimerTestContext *pFrameworkTimerTestContext = pTimerTestInfo->m_pFrameworkTimerTestContext;
    timeval                                  runTime;



    gettimeofday (&(pTimerTestInfo->m_endTime), NULL);

//    runTime.tv_sec  =   pTimerTestInfo->m_endTime.tv_sec  - (pTimerTestInfo->m_expirationTime.tv_sec + (m_endSendTime.tv_sec - pTimerTestInfo->m_startTime.tv_sec));
//    runTime.tv_usec =   pTimerTestInfo->m_endTime.tv_usec - (pTimerTestInfo->m_expirationTime.tv_usec + (m_endSendTime.tv_usec - pTimerTestInfo->m_startTime.tv_usec));

    runTime.tv_sec  =   pTimerTestInfo->m_endTime.tv_sec  - pTimerTestInfo->m_expirationTime.tv_sec;
    runTime.tv_usec =   pTimerTestInfo->m_endTime.tv_usec - pTimerTestInfo->m_expirationTime.tv_usec;


    if (runTime.tv_usec < 0)
    {
        runTime.tv_usec +=  1000000;
        runTime.tv_sec --;
    }

    if ((pTimerTestInfo->m_endTime.tv_sec  < pTimerTestInfo->m_expirationTime.tv_sec) || ((pTimerTestInfo->m_endTime.tv_sec  == pTimerTestInfo->m_expirationTime.tv_sec) && ((pTimerTestInfo->m_endTime.tv_usec/1000) < (pTimerTestInfo->m_expirationTime.tv_usec/1000))))
    {
        printf ("Got Timer (%d) Callback before we should. Exp: %ld.%ld Act:%ld.%ld\n", pTimerTestInfo->m_timerId, pTimerTestInfo->m_expirationTime.tv_sec, pTimerTestInfo->m_expirationTime.tv_usec, pTimerTestInfo->m_endTime.tv_sec, pTimerTestInfo->m_endTime.tv_usec);
    }


    if ( (runTime.tv_sec > m_maxDelay.tv_sec) || ((runTime.tv_sec == m_maxDelay.tv_sec) && (runTime.tv_usec > m_maxDelay.tv_usec)))
    {
        m_maxDelay.tv_sec  = runTime.tv_sec;
        m_maxDelay.tv_usec = runTime.tv_usec;
    }

//    pTimerTestInfo->m_expirationTime.tv_sec  =  pTimerTestInfo->m_endTime.tv_sec + pTimerTestInfo->m_periodicInterval.tv_sec;
//    pTimerTestInfo->m_expirationTime.tv_usec =  pTimerTestInfo->m_endTime.tv_usec + pTimerTestInfo->m_periodicInterval.tv_usec;

    pTimerTestInfo->m_expirationTime.tv_sec  +=  pTimerTestInfo->m_periodicInterval.tv_sec;
    pTimerTestInfo->m_expirationTime.tv_usec +=  pTimerTestInfo->m_periodicInterval.tv_usec;

    if (pTimerTestInfo->m_expirationTime.tv_usec >= 1000000)
    {
        pTimerTestInfo->m_expirationTime.tv_usec -= 1000000;
        pTimerTestInfo->m_expirationTime.tv_sec++;
    }

    if (((0 != pTimerTestInfo->m_periodicInterval.tv_sec) || (0 != pTimerTestInfo->m_periodicInterval.tv_usec)) && (0 == pTimerTestInfo->m_nTimes))
    {

        if (FRAMEWORK_SUCCESS != deleteTimer (pTimerTestInfo->m_timerId))
        {
            printf("Del Timer failed for id: %d data address:%lx\n", pTimerTestInfo->m_timerId, (ULI) pTimerTestInfo);
            //trace (TRACE_LEVEL_ERROR, "TimerTestObjectManager::timerExpiredCallback: delete timer FAILED.");
        }
    }

    if (((0 == pTimerTestInfo->m_periodicInterval.tv_sec) && (0 == pTimerTestInfo->m_periodicInterval.tv_usec)) || (0 == pTimerTestInfo->m_nTimes))
    {
        pTimerTestInfo->m_timerId = 0;
        if (NULL != pTimerTestInfo)
        {
            delete (pTimerTestInfo);
        }

        --(*pFrameworkTimerTestContext);
        pFrameworkTimerTestContext->executeNextStep (((pFrameworkTimerTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        if (pTimerTestInfo->m_nTimes)
        {
            pTimerTestInfo->m_nTimes--;
        }
        else
        {
            printf("pTimerTestInfo->m_nTimes==0, something wrong\n");
        }
    }
}

void TimerTestObjectManager::showPendingTimers (TimerTestData *pTimerTestInfo)
{

    cout << pTimerTestInfo->m_timerId << "- First interval: " << pTimerTestInfo->m_startInterval.tv_sec    << ":" << pTimerTestInfo->m_startInterval.tv_usec;
    cout << " Periodic interval: " << pTimerTestInfo->m_periodicInterval.tv_sec << ":" << pTimerTestInfo->m_periodicInterval.tv_usec;
    cout << " Start time: " << pTimerTestInfo->m_startTime.tv_sec << ":" << pTimerTestInfo->m_startTime.tv_usec;
    cout << " End Time: " << pTimerTestInfo->m_endTime.tv_sec << ":" << pTimerTestInfo->m_endTime.tv_usec;
    cout << " Exp Time: " << pTimerTestInfo->m_expirationTime.tv_sec << ":" << pTimerTestInfo->m_expirationTime.tv_usec;
    cout << " nTimes: " << pTimerTestInfo->m_nTimes<< endl;

}

void TimerTestObjectManager::timerShowStatStep (FrameworkTimerTestContext *pFrameworkTimerTestContext)
{
    // cout << "Max Delay:" << m_maxDelay.tv_sec << ":" << m_maxDelay.tv_usec << endl;
    TimerWorker::showMaxDelay ();
    pFrameworkTimerTestContext->executeNextStep (((pFrameworkTimerTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);

}

void TimerTestObjectManager::showPendingTimers ()
{
    vector<TimerTestData *>::iterator       tempIterator;
    int                                 i;

    cout << "Number of Pending timers: " << m_timerTestList.size () << endl;
    for( tempIterator = m_timerTestList.begin(), i=0; tempIterator != m_timerTestList.end(); tempIterator++, i++ ) {
        cout << i << "- Exp time: " << m_timerTestList[i]->m_expirationTime.tv_sec    << ":" << m_timerTestList[i]->m_expirationTime.tv_usec;
        cout << " First interval: " << m_timerTestList[i]->m_startInterval.tv_sec    << ":" << m_timerTestList[i]->m_startInterval.tv_usec;
        cout << " Periodic interval: " << m_timerTestList[i]->m_periodicInterval.tv_sec << ":" << m_timerTestList[i]->m_periodicInterval.tv_usec << endl;
    }
}

void TimerTestObjectManager::timerCancelTestStep (FrameworkTimerTestContext *pFrameworkTimerTestContext)
{
    TimerTestData           *pTimerTestInfo;
    UI32                     i;
    ResourceId               retCode;
    UI32                     nTimers = 1000;
    vector <TimerTestData *> timeDataList;

    srand (time(NULL));
    ++(*pFrameworkTimerTestContext);
    trace (TRACE_LEVEL_INFO, string ("Starting Timer Cancel Test. ") + string ("Starting 1000 one shot timers and cancel them after being expired."));

    for (i = 0; i < nTimers; i++)
    {
        pTimerTestInfo = new TimerTestData ();
        timeDataList.push_back (pTimerTestInfo);

        pTimerTestInfo->m_pFrameworkTimerTestContext = pFrameworkTimerTestContext;

        pTimerTestInfo->m_startInterval.tv_sec      =   rand () % 5;
        pTimerTestInfo->m_startInterval.tv_usec     =   rand () % 999999 + 1;
        pTimerTestInfo->m_periodicInterval.tv_sec   =   0;
        pTimerTestInfo->m_periodicInterval.tv_usec  =   0;
        pTimerTestInfo->m_nTimes                    =   0;

        gettimeofday (&pTimerTestInfo->m_startTime, NULL);

        retCode = startTimer (pTimerTestInfo->m_timerId, pTimerTestInfo->m_startInterval, pTimerTestInfo->m_periodicInterval, reinterpret_cast<PrismTimerExpirationHandler> (&TimerTestObjectManager::cancelTimerTestCallback), (void*) pTimerTestInfo);

        if (FRAMEWORK_SUCCESS != retCode)
        {
            pFrameworkTimerTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("TimerTestObjectManager::testRequestHandler: startTimerMessage FAILED.") + retCode + string ("Timer:") + pTimerTestInfo->m_startInterval.tv_sec + string (".") + pTimerTestInfo->m_startInterval.tv_usec);

            delete (pTimerTestInfo);
            continue;
        }
        else
        {
            ++(*pFrameworkTimerTestContext);
        }

        pTimerTestInfo->m_expirationTime.tv_sec  =   pTimerTestInfo->m_startTime.tv_sec  + pTimerTestInfo->m_startInterval.tv_sec;
        pTimerTestInfo->m_expirationTime.tv_usec =   pTimerTestInfo->m_startTime.tv_usec + pTimerTestInfo->m_startInterval.tv_usec;
        if (pTimerTestInfo->m_expirationTime.tv_usec >= 1000000)
        {
            pTimerTestInfo->m_expirationTime.tv_usec -= 1000000;
            pTimerTestInfo->m_expirationTime.tv_sec++;
        }


    }
    gettimeofday (&m_endSendTime, NULL);
    trace (TRACE_LEVEL_INFO, "Timer Cancel. Sent All timer messages and now waiting for timers to expire.");

    prismSleep (10);
    trace (TRACE_LEVEL_INFO, "Timer Cancel. Timer should have expired by now. We will try to cancel them");

    while (timeDataList.size () > 0)
    {
        trace (TRACE_LEVEL_DEBUG, string ("Timer Cancel. Timer Id: ") + timeDataList[0]->m_timerId);

        if (FRAMEWORK_SUCCESS !=deleteTimer (timeDataList[0]->m_timerId))
        {
            trace (TRACE_LEVEL_ERROR, "Could not delete timer!");
        }
        --(*pFrameworkTimerTestContext);
        delete (timeDataList [0]);
        timeDataList.erase (timeDataList.begin ());
    }
    trace (TRACE_LEVEL_INFO, "Timer Cancel. Cacnelled all timer successfully");

    --(*pFrameworkTimerTestContext);
    pFrameworkTimerTestContext->executeNextStep (((pFrameworkTimerTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}


void TimerTestObjectManager::periodicTimerCancelTestStep (FrameworkTimerTestContext *pFrameworkTimerTestContext)
{
    TimerTestData           *pTimerTestInfo;
    UI32                     i;
    ResourceId               retCode;
    UI32                     nTimers = 1000;
    vector <TimerTestData *> timeDataList;

    srand (time(NULL));
    ++(*pFrameworkTimerTestContext);
    trace (TRACE_LEVEL_INFO, string ("Starting Periodic Timer Cancel Test. ") + string ("Starting 1000 timers and cancel them after being expired."));

    for (i = 0; i < nTimers; i++)
    {
        pTimerTestInfo = new TimerTestData ();
        timeDataList.push_back (pTimerTestInfo);

        pTimerTestInfo->m_pFrameworkTimerTestContext = pFrameworkTimerTestContext;

        pTimerTestInfo->m_startInterval.tv_sec      =   rand () % 5;
        pTimerTestInfo->m_startInterval.tv_usec     =   rand () % 999999 + 1;
        pTimerTestInfo->m_periodicInterval.tv_sec   =   5; //rand () % 10;
        pTimerTestInfo->m_periodicInterval.tv_usec  =   0; //rand () % 1000000;
        pTimerTestInfo->m_nTimes                    =   rand () % 10;

        gettimeofday (&pTimerTestInfo->m_startTime, NULL);

        retCode = startTimer (pTimerTestInfo->m_timerId, pTimerTestInfo->m_startInterval, pTimerTestInfo->m_periodicInterval, reinterpret_cast<PrismTimerExpirationHandler> (&TimerTestObjectManager::cancelTimerTestCallback), (void*) pTimerTestInfo);

        if (FRAMEWORK_SUCCESS != retCode)
        {
            pFrameworkTimerTestContext->incrementNumberOfFailures ();
            trace (TRACE_LEVEL_ERROR, string ("TimerTestObjectManager::testRequestHandler: startTimerMessage FAILED.") + retCode + string ("Timer:") + pTimerTestInfo->m_startInterval.tv_sec + string (".") + pTimerTestInfo->m_startInterval.tv_usec);

            delete (pTimerTestInfo);
            continue;
        }
        else
        {
            ++(*pFrameworkTimerTestContext);
        }

        pTimerTestInfo->m_expirationTime.tv_sec  =   pTimerTestInfo->m_startTime.tv_sec  + pTimerTestInfo->m_startInterval.tv_sec;
        pTimerTestInfo->m_expirationTime.tv_usec =   pTimerTestInfo->m_startTime.tv_usec + pTimerTestInfo->m_startInterval.tv_usec;
        if (pTimerTestInfo->m_expirationTime.tv_usec >= 1000000)
        {
            pTimerTestInfo->m_expirationTime.tv_usec -= 1000000;
            pTimerTestInfo->m_expirationTime.tv_sec++;
        }


    }
    gettimeofday (&m_endSendTime, NULL);
    trace (TRACE_LEVEL_INFO, "Timer Cancel. Sent All timer messages and now waiting for timers to expire.");

    prismSleep (10);
    trace (TRACE_LEVEL_INFO, "Timer Cancel. Timer should have expired by now. We will try to cancel them");

    while (timeDataList.size () > 0)
    {
        trace (TRACE_LEVEL_DEBUG, string ("Timer Cancel. Timer Id: ") + timeDataList[0]->m_timerId);

        if (FRAMEWORK_SUCCESS !=deleteTimer (timeDataList[0]->m_timerId))
        {
            trace (TRACE_LEVEL_ERROR, "Could not delete timer!");
        }
        --(*pFrameworkTimerTestContext);
        delete (timeDataList [0]);
        timeDataList.erase (timeDataList.begin ());
    }
    trace (TRACE_LEVEL_INFO, "Timer Cancel. Cacnelled all timer successfully");

    --(*pFrameworkTimerTestContext);
    pFrameworkTimerTestContext->executeNextStep (((pFrameworkTimerTestContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}
void TimerTestObjectManager::cancelTimerTestCallback (TimerHandle timerHandle, TimerTestData * pTimerTestInfo)
{
    static UI32 count = 0;
    trace (TRACE_LEVEL_ERROR, string ("Got callback in timer expiered call back!!!") + count);
    count++;
}

TimerTestData::TimerTestData ()
    : m_pFrameworkTimerTestContext (NULL), 
      m_timerId (0),
      m_nTimes (0)
{
    m_startInterval.tv_sec = 0;
    m_startInterval.tv_usec = 0;

    m_periodicInterval.tv_sec = 0;
    m_periodicInterval.tv_usec = 0;

    m_startTime.tv_sec = 0;
    m_startTime.tv_usec = 0;

    m_endTime.tv_sec = 0;
    m_endTime.tv_usec = 0;

    m_expirationTime.tv_sec = 0;
    m_expirationTime.tv_usec = 0;
}

TimerTestData::~TimerTestData ()
{

}


}
