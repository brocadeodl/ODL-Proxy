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

#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/Timer/TimerObjectManager.h"
#include "Framework/Timer/TimerWorker.h"
#include "Framework/Timer/TimerMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include <signal.h>
#include <time.h>

namespace WaveNs
{

typedef multimap<PrismServiceId, UI32>::iterator multimapIterator;

TimerWorker::TimerWorker (WaveObjectManager *pWaveObjectManager)
            :WaveWorker   (pWaveObjectManager), m_currTimerId (1)
{
    addOperationMap (TIMER_ADD_TIMER,                       reinterpret_cast<PrismMessageHandler> (&TimerWorker::addTimer));
    addOperationMap (TIMER_DELETE_TIMER,                    reinterpret_cast<PrismMessageHandler> (&TimerWorker::deleteTimer));
    addOperationMap (TIMER_DELETE_ALL_TIMERS_FOR_SERVICE,   reinterpret_cast<PrismMessageHandler> (&TimerWorker::deleteAllTimersForService));

    m_maxDelay.tv_sec  = 0;
    m_maxDelay.tv_usec = 0;
}

TimerWorker::~TimerWorker ()
{
}

/*
 * addTimer
 *      The handler for Add Timer Message.
 * Inputs:
 *      Timer interval and Timer period in sec and usec.
 * Outputs:
 *      Timer Id. Id starts from 1.
 *
 * Describtion:
 *     The timer will be added to the existing timer list. The list is ordered based
 *     on which timer will expire first. before inserting the new timer we stop the
 *     running timer (if any). Insert the timer, then restart the system timer for
 *     the first timer in the list.
 */

void TimerWorker::addTimer (TimerObjectManagerAddTimerMessage *pMessage)
{
    itimerval                       timerVal;

    trace (TRACE_LEVEL_DEVEL, "TimerWorker::addTimer : Entering ...");

    m_mutex.lock ();

    // stop current timer

    memset (&timerVal, 0, sizeof (itimerval));

    if (0 != prismSetITimer (&timerVal))
    {
        // Could not stop the timer. Fatal error. Can not continue.

        trace (TRACE_LEVEL_ERROR, "TimerWorker::addTimer : Could not disable timer.");
        prismAssert (false, __FILE__, __LINE__);

        pMessage->setTimerId (0);
        pMessage->setCompletionStatus (TIMER_ERROR_CAN_NOT_CANCEL_SYS_TIMER);
        reply (pMessage);
        m_mutex.unlock ();
        return;
    }

    TimerData                      *pTimerInfo  =   new TimerData ();
    prismAssert (NULL != pTimerInfo, __FILE__, __LINE__);

    if (NULL == pTimerInfo)
    {
        trace (TRACE_LEVEL_ERROR, "TimerWorker::addTimer : Could not alloc memory.");
        prismAssert (false, __FILE__, __LINE__);

        pMessage->setTimerId (0);
        pMessage->setCompletionStatus (TIMER_ERROR_CAN_NOT_ALLOC_MEMORY);
        reply (pMessage);
        m_mutex.unlock ();
        return;
    }

    // Set timer data.

    pTimerInfo->m_periodicInterval.tv_sec   =   pMessage->getPeriodicInterval ().tv_sec;
    pTimerInfo->m_periodicInterval.tv_usec  =   pMessage->getPeriodicInterval ().tv_usec;
    pTimerInfo->m_expirationTime.tv_sec     =   pMessage->getStartTime ().tv_sec  + pMessage->getStartInterval ().tv_sec;
    pTimerInfo->m_expirationTime.tv_usec    =   pMessage->getStartTime ().tv_usec + pMessage->getStartInterval ().tv_usec;

    // Adjust the expiration time in case of usec is more than 1000000.

    if ( pTimerInfo->m_expirationTime.tv_usec >= MAX_USEC)
    {
        pTimerInfo->m_expirationTime.tv_usec  -=  MAX_USEC;
        pTimerInfo->m_expirationTime.tv_sec++;
    }

    // Set the timer ID. Timer ID starts from 1.
    // Never use 0 as a timer Id.

    if (0 == m_currTimerId)
    {
        m_currTimerId++;
    }

    pTimerInfo->m_timerId =   m_currTimerId++;
    pMessage->setTimerId (pTimerInfo->m_timerId);

    // Set call back infomation to sent timer expiration message when the timer expires.

    pTimerInfo->m_pPrismTimerExpirationCallback = pMessage->getTimerExpirationCallback ();
    pTimerInfo->m_pPrismTimerExpirationContext  = pMessage->getTimerExpirationContext ();
    pTimerInfo->m_pPrismTimerSender             = pMessage->getTimerSender ();
    pTimerInfo->m_serviceId                     = pMessage->getSenderServiceCode ();

    // Put the timer in the timer list.

    addTimerToList (pTimerInfo);

    // restart the timer for the first timer in the list.

    if (0 != restartTimer ())
    {
        trace (TRACE_LEVEL_ERROR, "TimerWorker::addTimer : Could not restart timer.");
        prismAssert (false, __FILE__, __LINE__);

        pMessage->setTimerId (0);
        pMessage->setCompletionStatus (TIMER_ERROR_CAN_NOT_START_SYS_TIMER);
        reply (pMessage);
        m_mutex.unlock ();
        return;
    }

    pMessage->setCompletionStatus (TIMER_SUCCESS);
    reply (pMessage);
    m_mutex.unlock ();

}
/*
 * addTimerToList:
 *         add timer to timer list.
 * Inputs:
 *         Timer Data.
 * Outputs:
 *
 * Describtion:
 *     The timer will be added to the existing timer list. The list is ordered based
 *     on which timer will expire first. Binary search is used to insert the timer
 *     in the appropriate place.
 */

void TimerWorker::addTimerToList (TimerData *pTimerInfo)
{
    UI32                                position;
    vector<TimerData *>::iterator       theIterator;

    position = binarySearch (0, m_timerList.size () - 1, pTimerInfo->m_expirationTime);
    theIterator = m_timerList.begin() + position;

    m_timerList.insert (theIterator, pTimerInfo);

    m_timerIdsByServiceId.insert (pair<PrismServiceId, UI32> (pTimerInfo->m_serviceId, pTimerInfo->m_timerId));
}

/*
 * binarySearch:
 *          Binary search algorithm.
 * Inputs:
 *          first: First item in the list.
 *          last: Last item in the list.
 *          key: entery to be inserted.
 *
 * Outputs:
 *          return first item in the list.
 *
 * Describtion:
 *     Insert item to the timer list based on which timer will expired first.
 *
 */

UI32 TimerWorker::binarySearch (int first, int last, timeval &key)
{
    while (first <= last)
    {
        int mid = (first + last) / 2;
        if ((key.tv_sec  > m_timerList[mid]->m_expirationTime.tv_sec) ||
            ((key.tv_sec == m_timerList[mid]->m_expirationTime.tv_sec) && (key.tv_usec  > m_timerList[mid]->m_expirationTime.tv_usec)))
        {
            // repeat search in top half.
            first = mid + 1;
        }
        else
        {

            if ((key.tv_sec  < m_timerList[mid]->m_expirationTime.tv_sec) ||
                ((key.tv_sec == m_timerList[mid]->m_expirationTime.tv_sec) && (key.tv_usec  < m_timerList[mid]->m_expirationTime.tv_usec)))
            {
                // repeat search in bottom half.
                last = mid - 1;
            }
            else
            {
                // found it. return position.
                return mid;
            }
        }
    }

    // did not find key. retun position to insert it.
    return first;
}
/*
 * resetMaxDelay:
 *          Set Max Delay variable to 0.
 *
 * Describtion:
 *     This is a static function that sets Max Delay variable to 0. This variable measures the
 *     difference between when the timer should expire and when it expires.
 *
 */

void TimerWorker::resetMaxDelay ()
{
    (TimerObjectManager::getInstance ())->m_pTimerWorker->m_mutex.lock ();
    (TimerObjectManager::getInstance ())->m_pTimerWorker->m_maxDelay.tv_sec  = 0;
    (TimerObjectManager::getInstance ())->m_pTimerWorker->m_maxDelay.tv_usec = 0;
    (TimerObjectManager::getInstance ())->m_pTimerWorker->m_mutex.unlock ();
}

/*
 * showMaxDelay:
 *          Display the max delay.
 *
 * Describtion:
 *          This is a static function that can be used to show the max delay
 *          happened for any timer that was running.
 *
 */

void TimerWorker::showMaxDelay ()
{
    (TimerObjectManager::getInstance ())->m_pTimerWorker->m_mutex.lock ();
    cout << "Timer Max Delay: " << (TimerObjectManager::getInstance ())->m_pTimerWorker->m_maxDelay.tv_sec << ":" << (TimerObjectManager::getInstance ())->m_pTimerWorker->m_maxDelay.tv_usec << endl;
    (TimerObjectManager::getInstance ())->m_pTimerWorker->m_mutex.unlock ();
}

/*
 * restartTimer:
 *          Start the system timer using the first timer in the timer list.
 * Inputs:
 *
 * Outputs:
 *          0 in success
 *          -1 otherwise.
 *
 * Describtion:
 *     restert the system timer based on the first timer in the timer list.
 *     If the time should have been expired send the expiration message,
 *     remove the timer from the list and check the new first timer in the
 *     list.
 *
 */
int  TimerWorker::restartTimer ()
{
    timeval                                 currTime;
    timeval                                 runTime;
    itimerval                               timerVal;
    TimerData                              *pCurrTimerInfo;
    vector<TimerData *>::iterator           startIterator;
    vector<TimerData *>::iterator           endIterator;
    TimerData                               *pTimerInfo;

    timerVal.it_interval.tv_sec     =    0;
    timerVal.it_interval.tv_usec    =    0;

    // Check if there are more timers in the list.
    startIterator   = m_timerList.begin();
    endIterator     = m_timerList.end();

    // If there is no more timer in the list return.

    if (endIterator == startIterator)
    {
        return (0);
    }

    //pTimerInfo      = m_timerList[0];

    // Get current timer to check if any of the timers should have been expired.

    if (0 != gettimeofday (&currTime, NULL))
    {
        trace (TRACE_LEVEL_FATAL, "TimerWorker::restartTimer : error getting time.");
        prismAssert (false, __FILE__, __LINE__);
    }

    // While the list is not empty and the expiration timer is before the current time, send expiration
    // message and remove the timer from the list

    while (!m_timerList.empty () && ((currTime.tv_sec  > m_timerList[0]->m_expirationTime.tv_sec) ||
            ((currTime.tv_sec == m_timerList[0]->m_expirationTime.tv_sec) && (currTime.tv_usec  >= m_timerList[0]->m_expirationTime.tv_usec))))
    {
        // Send expiration message for the expired timer.

        PrismTimerExpiredObjectManagerMessage     *pPrismTimerExpiredObjectManagerMessage = new PrismTimerExpiredObjectManagerMessage (m_timerList[0]->m_serviceId, m_timerList[0]->m_timerId, m_timerList[0]->m_pPrismTimerExpirationCallback, m_timerList[0]->m_pPrismTimerExpirationContext, m_timerList[0]->m_pPrismTimerSender);
        prismAssert (NULL != pPrismTimerExpiredObjectManagerMessage, __FILE__, __LINE__);

        timerSendOneWay (pPrismTimerExpiredObjectManagerMessage);

        // Calculate Max Delay. This is used only for statistic.

        runTime.tv_sec  =   currTime.tv_sec  - m_timerList[0]->m_expirationTime.tv_sec;
        runTime.tv_usec =   currTime.tv_usec - m_timerList[0]->m_expirationTime.tv_usec;

        if (runTime.tv_usec < 0)
        {
            runTime.tv_usec += MAX_USEC;
            runTime.tv_sec --;
        }

        if ( (runTime.tv_sec > m_maxDelay.tv_sec) || ((runTime.tv_sec == m_maxDelay.tv_sec) && (runTime.tv_usec > m_maxDelay.tv_usec)))
        {
            m_maxDelay.tv_sec  = runTime.tv_sec;
            m_maxDelay.tv_usec = runTime.tv_usec;
        }

        pTimerInfo      = m_timerList[0];

        // Remove the first timer from the list.

        removeTimerFromServiceTimersMultimap (pTimerInfo->m_serviceId, pTimerInfo->m_timerId);

        startIterator   = m_timerList.begin();
        m_timerList.erase (startIterator);

        // Remove timer data if single timer. Add Timer again if it is periodic.DMMP_AddWWNColons.DMMP_AddWWNColons

        if ((pTimerInfo->m_periodicInterval.tv_sec != 0) || (pTimerInfo->m_periodicInterval.tv_usec != 0))
        {
            // add periodic timer.
            pTimerInfo->m_expirationTime.tv_sec  +=  pTimerInfo->m_periodicInterval.tv_sec;
            pTimerInfo->m_expirationTime.tv_usec +=  pTimerInfo->m_periodicInterval.tv_usec;

            if (pTimerInfo->m_expirationTime.tv_usec >= MAX_USEC)
            {
                pTimerInfo->m_expirationTime.tv_sec++;
                pTimerInfo->m_expirationTime.tv_usec -=  MAX_USEC;
            }

            addTimerToList (pTimerInfo);
        }
        else
        {
            // Delete timer info if it is a single timer and expired.
            delete (pTimerInfo);
        }

    }

    // If there is any timer left in the timer list start this timer.

    if (!m_timerList.empty ())
    {
        pCurrTimerInfo      = m_timerList[0];

        timerVal.it_value.tv_sec        =    pCurrTimerInfo->m_expirationTime.tv_sec  - currTime.tv_sec;
        timerVal.it_value.tv_usec       =    pCurrTimerInfo->m_expirationTime.tv_usec - currTime.tv_usec;

        // Just in case that tv_usec became "-ve". The timer values for sec and usec should be > 0.

        if (timerVal.it_value.tv_usec < 0)
        {
            timerVal.it_value.tv_usec  +=  MAX_USEC;
            timerVal.it_value.tv_sec--;
        }

        int status = prismSetITimer (&timerVal);

        if (0 != status)
        {
            // This should never happens. This is debug info, in case it happens.

            trace (TRACE_LEVEL_ERROR, string ("Error in prismSetITimer , TimeVal:") + timerVal.it_value.tv_sec + string (":") + timerVal.it_value.tv_usec);
            //cout << "Error in prismSetITimer , TimeVal:" << timerVal.it_value.tv_sec << ":" << timerVal.it_value.tv_usec << endl;
        }

        return status;
    }

    return (0);
}

/*  showPendingTimers
 *          Should the list of pending timers.
 *          This function is used only for debugging.
 */

void TimerWorker::showPendingTimers ()
{
    vector<TimerData *>::iterator       tempIterator;
    int                                 i;

    m_mutex.lock ();

    cout << "Number of Pending timers: " << m_timerList.size () << endl;

    for (tempIterator = m_timerList.begin(), i=0; tempIterator != m_timerList.end(); tempIterator++, i++)
    {
        cout << i << "- Id: " << m_timerList[i]->m_timerId << " Exp time: "  << m_timerList[i]->m_expirationTime.tv_sec    << ":" << m_timerList[i]->m_expirationTime.tv_usec;
        cout << " Periodic interval: " << m_timerList[i]->m_periodicInterval.tv_sec << ":" << m_timerList[i]->m_periodicInterval.tv_usec << endl;
    }

    m_mutex.unlock ();
}


void TimerWorker::timerReply (PrismMessage *pPrismMessage)
{
    reply (pPrismMessage);
}

void TimerWorker::timerSendOneWay (PrismMessage *pPrismMessage)
{
    ResourceId status = sendOneWay (pPrismMessage);

    if (status != WAVE_MESSAGE_SUCCESS)
    {
        delete pPrismMessage;
    }
}

/*
 * deleteTimer
 *      The handler for delete Timer Message.
 * Inputs:
 *      Timer id to be deleted.
 * Outputs:
 *      Message status is success if the timer is removed.
 *      If the timer was not found appropriate error code will be set.
 * Describtion:
 *     Remove a timer from the timer list.
 *     NOTE: If the timer is deleted successfully the caller will never
 *           get expiration message even if the message was sent before
 *           we got the delete timer request. We will make sure to
 *           delete timer message from the caller queue.
 *
 */
void TimerWorker::deleteTimer (TimerObjectManagerDeleteTimerMessage *pMessage)
{
    itimerval                       timerVal;

    trace (TRACE_LEVEL_DEVEL, "TimerWorker::deleteTimer : Entering ...");
    
    m_mutex.lock ();

    // Stop current running timer (if any).

    memset (&timerVal, 0, sizeof (itimerval));

    if (0 != prismSetITimer (&timerVal))
    {
        trace (TRACE_LEVEL_ERROR, "TimerWorker::deleteTimer : Could not disable timer.");
        prismAssert (false, __FILE__, __LINE__);

        pMessage->setCompletionStatus (TIMER_ERROR_CAN_NOT_CANCEL_SYS_TIMER);
        reply (pMessage);
        m_mutex.unlock ();
        return;
    }

    // remove requested timer from timer list.

    if (1 == removeTimer (pMessage, pMessage->getTimerId ()))
    {
        pMessage->setCompletionStatus (TIMER_SUCCESS);
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "TimerWorker::deleteTimer : Invalid Handle.");
        pMessage->setCompletionStatus (TIMER_ERROR_INVALID_HANDLE);
    }

    // If there are timers in the timer list start the first timer there.

    if (!m_timerList.empty ())
    {
        if (0 != restartTimer ())
        {
            trace (TRACE_LEVEL_ERROR, "TimerWorker::deleteTimer : Could not restart timer.");
            prismAssert (false, __FILE__, __LINE__);
            pMessage->setCompletionStatus (TIMER_ERROR_CAN_NOT_START_SYS_TIMER);
            reply (pMessage);
            m_mutex.unlock ();
            return;
        }
    }

//    showPendingTimers ();
    reply (pMessage);
    m_mutex.unlock ();
}

/*
 * removeTimer
 *          Remove a timer from the timer list.
 *
 * Inputs:
 *          timerId: Id of the timer to be removed.
 *
 * Outputs:
 *          0 on sucess and 1 if the timer was not found.
 *
 * Describtion:
 *          Search for a timer using timer Id and remove it from the timer list.
 *
 */
UI32 TimerWorker::removeTimer (TimerObjectManagerDeleteTimerMessage *pMessage, UI32 timerId)
{
    vector<TimerData *>::iterator    theIterator;
    UI32                             i;
    UI32                             found = 0;
    PrismThread                     *pPrismThread = NULL;
    UI32                             nTimersRecalled;

    for (i = 0; i < m_timerList.size(); i++)
    {
        if (m_timerList[i]->m_timerId == timerId)
        {
            removeTimerFromServiceTimersMultimap (m_timerList[i]->m_serviceId, m_timerList[i]->m_timerId);

            delete (m_timerList[i]);

            theIterator =  m_timerList.begin() + i;
            m_timerList.erase (theIterator);

            found = 1;

            break;
        }
    }

    pPrismThread = PrismThread::getPrismThreadForServiceId (pMessage->getSenderServiceCode());

    if (NULL == pPrismThread)
    {
        trace (TRACE_LEVEL_FATAL, "TimerWorker::removeTimer : Calling thread for delete timer is NULL");
        prismAssert (false, __FILE__, __LINE__);
        return 0;
    }

    nTimersRecalled = pPrismThread->recallTimerExpirationMessagesForTimer (timerId);

    if (0 != nTimersRecalled)
    {
        found = 1;
    }

    return (found);
}

void TimerWorker::removeTimerFromServiceTimersMultimap (PrismServiceId serviceId, UI32 timerId)
{
    pair<multimapIterator, multimapIterator>    iteratorPair = m_timerIdsByServiceId.equal_range (serviceId);
    multimapIterator                            iterator;

    for (iterator = iteratorPair.first; iterator != iteratorPair.second; ++iterator)
    {
        if (iterator->second == timerId)
        {
            m_timerIdsByServiceId.erase (iterator);

            break;
        }
    }
}

void TimerWorker::deleteAllTimersForService (TimerObjectManagerDeleteAllTimersForServiceMessage *pMessage)
{
    itimerval                       timerVal;

    trace (TRACE_LEVEL_DEVEL, "TimerWorker::deleteAllTimersForService : Entering ...");
  
    m_mutex.lock ();

    // Stop current running timer (if any).

    memset (&timerVal, 0, sizeof (itimerval));

    if (0 != prismSetITimer (&timerVal))
    {
        trace (TRACE_LEVEL_ERROR, "TimerWorker::deleteAllTimersForService : Could not disable timer.");
        prismAssert (false, __FILE__, __LINE__);

        pMessage->setCompletionStatus (TIMER_ERROR_CAN_NOT_CANCEL_SYS_TIMER);
        reply (pMessage);

        m_mutex.unlock ();

        return;
    }

    // Remove all timers for the service id

    UI32 serviceId = pMessage->getSenderServiceCode ();

    ResourceId status = removeAllTimersForService (serviceId);

    pMessage->setCompletionStatus (status);

    // If there are timers in the timer list start the first timer there.

    if (!m_timerList.empty ())
    {
        if (0 != restartTimer ())
        {
            trace (TRACE_LEVEL_ERROR, "TimerWorker::deleteAllTimersForService : Could not restart timer.");
            prismAssert (false, __FILE__, __LINE__);

            pMessage->setCompletionStatus (TIMER_ERROR_CAN_NOT_START_SYS_TIMER);
            reply (pMessage);

            m_mutex.unlock ();

            return;
        }
    }

    reply (pMessage);

    m_mutex.unlock ();
}

ResourceId TimerWorker::removeAllTimersForService (UI32 serviceId)
{
    pair<multimapIterator, multimapIterator>    iteratorPair    = m_timerIdsByServiceId.equal_range (serviceId);
    multimapIterator                            iterator;
    vector<TimerData *>::iterator               theIterator;
    UI32                                        i               = 0;
    PrismThread                                *pPrismThread    = NULL;
    ResourceId                                  status          = TIMER_SUCCESS;

    pPrismThread = PrismThread::getPrismThreadForServiceId (serviceId);
    
    if (NULL == pPrismThread)
    {
        trace (TRACE_LEVEL_FATAL, "TimerWorker::removeAllTimersForService : Calling thread for delete timer is NULL");
        prismAssert (false, __FILE__, __LINE__);

        return TIMER_ERROR_TIMER_SHUTDOWN;
    }

    for (iterator = iteratorPair.first; iterator != iteratorPair.second; ++iterator)
    {
        for (i = 0; i < m_timerList.size (); i++)
        {
            if (iterator->second == m_timerList[i]->m_timerId)
            {
                trace (TRACE_LEVEL_DEBUG, string ("TimerWorker::removeAllTimersForService : Removing timer for ServiceId : ") + serviceId + ", ServiceName : " + FrameworkToolKit::getServiceNameById (serviceId) + ", TimerId : " + iterator->second);

                delete (m_timerList[i]);

                theIterator =  m_timerList.begin() + i;
                m_timerList.erase (theIterator);

                break;
            }
        }   
    }

    return (status);
}

/*
 * deleteAllTimer
 *     Delete all timers
 *
 * Inputs:
 *
 * Outputs:
 *
 * Description:
 *     Remove all timers from the timer list.
 *     NOTE: If the timer is deleted successfully the caller will never
 *           get expiration message even if the message was sent before
 *           we got the delete timer request. We will make sure to
 *           delete timer message from the caller queue.
 *
 */
void TimerWorker::deleteAllTimers ()
{
    itimerval                       timerVal;

    trace (TRACE_LEVEL_DEVEL, "TimerWorker::deleteAllTimers : Entering ...");

    m_mutex.lock ();

    // Stop current running timer (if any).

    memset (&timerVal, 0, sizeof (itimerval));

    if (0 != prismSetITimer (&timerVal))
    {
        trace (TRACE_LEVEL_ERROR, "TimerWorker::deleteAllTimers : Could not disable timer.");
        prismAssert (false, __FILE__, __LINE__);

        m_mutex.unlock ();
        return;
    }

    // remove All timers from timer list.

    removeAllTimers ();

    m_mutex.unlock ();
}

/*
 * removeAllTimers
 *          Remove all timers from the timer list.
 *
 * Inputs:
 *
 *
 * Outputs:
 *
 *
 * Describtion:
 *          remove all timers from the timer list.
 *
 */
void TimerWorker::removeAllTimers ()
{
    vector<TimerData *>::iterator    theIterator;

    TimerData                       *pTimerInfo;

    while ( 0 < m_timerList.size ())
    {
        pTimerInfo      = m_timerList[0];

        prismAssert (NULL != pTimerInfo, __FILE__, __LINE__);

        delete (pTimerInfo);
        theIterator =  m_timerList.begin();
        m_timerList.erase (theIterator);
    }

    m_timerIdsByServiceId.clear ();

    return;
}

/*
 * processTimeOut()
 *      This function is called when the system timer is expired.
 *      When this happen we notify who register for this timer by
 *      sendiung expiration message. We also start the next timer
 *      in the timer list.
 *
 */
void TimerWorker::processTimeOut ()
{
    (TimerObjectManager::getInstance ())->m_pTimerWorker->m_mutex.lock ();

    if (0 != (TimerObjectManager::getInstance ())->m_pTimerWorker->restartTimer ())
    {
        (TimerObjectManager::getInstance ())->m_pTimerWorker->trace (TRACE_LEVEL_ERROR, "Could not restart timer.");
        (TimerObjectManager::getInstance ())->m_pTimerWorker->prismAssert (false, __FILE__, __LINE__);
    }

    (TimerObjectManager::getInstance ())->m_pTimerWorker->m_mutex.unlock ();
}


TimerData::TimerData ()
{
    m_expirationTime.tv_sec = 0;
    m_expirationTime.tv_usec = 0;
    m_periodicInterval.tv_sec = 0;
    m_periodicInterval.tv_usec = 0;
    m_pPrismTimerExpirationCallback = NULL;
    m_pPrismTimerExpirationContext = NULL;
    m_pPrismTimerSender = NULL;
    m_serviceId  = 0;
    m_timerId    = 0;
}

TimerData::~TimerData ()
{

}

}
