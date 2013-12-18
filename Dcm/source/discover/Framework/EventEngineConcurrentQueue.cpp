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
 *   Author : Ritesh Madapurath                                            *
 **************************************************************************/

#include "discover/Framework/EventEngineConcurrentQueue.h"

namespace DcmNs {

    EventEngineConcurrentQueue::EventEngineConcurrentQueue():stop(false) {
        m_pSynchronizingMutex = new PrismMutex();
        m_pSynchronizingCondition = new PrismCondition(m_pSynchronizingMutex);
    }

    EventEngineConcurrentQueue::~EventEngineConcurrentQueue() {
        delete (m_pSynchronizingMutex);
        delete (m_pSynchronizingCondition);
    }

    void EventEngineConcurrentQueue::pushAndNotify(vnm_resp_t *message) {
        //Guard the insertion into the queue
        m_pSynchronizingMutex->lock();
        //queue push
        queue.push_back(message);
        //Notify waiting listeners.
        m_pSynchronizingCondition->resume();
        m_pSynchronizingMutex->unlock();

    }
    void EventEngineConcurrentQueue::clear() {
        //Guard the insertion into the queue
        m_pSynchronizingMutex->lock();
        //queue clear
        queue.clear();

        m_pSynchronizingMutex->unlock();

    }
    void EventEngineConcurrentQueue::clearAndNotify() {
        //Guard the insertion into the queue
        m_pSynchronizingMutex->lock();
        //queue clear
        queue.clear();
        //Notify waiting listeners.
        m_pSynchronizingCondition->resume();
        m_pSynchronizingMutex->unlock();

    }
    vnm_resp_t *EventEngineConcurrentQueue::waitAndPop() throw (int) {
        //Guard the retrieval from the queue
        m_pSynchronizingMutex->lock();
        while (!stop && queue.empty()) {
            m_pSynchronizingCondition->wait();
            //TODO move it outside the while loop
            if (stop){
                m_pSynchronizingMutex->unlock();
                  throw  20;
            }

        }
        vnm_resp_t *pTemp = queue.front();
        //Retrieve from the FRONT (FIFO)
        queue.pop_front();
        m_pSynchronizingMutex->unlock();
        return pTemp;
    }


    void EventEngineConcurrentQueue::stopQueue() {
        //Guard the insertion into the queue
        m_pSynchronizingMutex->lock();
        //queue clear
        queue.clear();
        stop = true;
        //Notify waiting listeners.
        m_pSynchronizingCondition->resume();
        m_pSynchronizingMutex->unlock();
    }
   /* const char*BlockingQueueTerminateException_C:: what() const throw(){
        return "Queue Terminated";
    }*/

    void EventEngineConcurrentQueue::reStart(){
        queue.clear();
        stop = false;
    }

}

/* namespace DcmNs */
