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


#include "discover/Framework/EventConcurrentQueue.h"

namespace DcmNs {

    EventConcurrentQueue::EventConcurrentQueue():stop(false) {
        m_pSynchronizingMutex = new PrismMutex();
        m_pSynchronizingCondition = new PrismCondition(m_pSynchronizingMutex);
    }

    EventConcurrentQueue::~EventConcurrentQueue() {
        delete (m_pSynchronizingMutex);
        delete (m_pSynchronizingCondition);
    }

    void EventConcurrentQueue::pushAndNotify(Event *message) {
        //Guard the insertion into the queue
        m_pSynchronizingMutex->lock();
        //queue push
        queue.push_back(message);
        //Notify waiting listeners.
        m_pSynchronizingCondition->resume();
        m_pSynchronizingMutex->unlock();

    }
    void EventConcurrentQueue::clear() {
        //Guard the insertion into the queue
        m_pSynchronizingMutex->lock();
        //If the queue is cleared then the event object should be deleted
        size_t qsize = queue.size(); 
        for(size_t i = 0;i<qsize; i++){
            Event * evt = queue.front();
            queue.pop_front();
            if(evt) delete(evt);
        }              
        //queue clear
        queue.clear();

        m_pSynchronizingMutex->unlock();

    }
    void EventConcurrentQueue::clearAndNotify() {
        //Guard the insertion into the queue
        m_pSynchronizingMutex->lock();
        //queue clear
        size_t qsize = queue.size(); 
        for(size_t i = 0;i<qsize; i++){
            Event * evt = queue.front();
            queue.pop_front();
            if(evt) delete(evt);
        }              
        queue.clear();
        //Notify waiting listeners.
        m_pSynchronizingCondition->resume();
        m_pSynchronizingMutex->unlock();

    }

    Event *EventConcurrentQueue::waitAndPop() {
        //Guard the retrieval from the queue
        m_pSynchronizingMutex->lock();
        while (!stop && queue.empty()) {
            m_pSynchronizingCondition->wait();
            //TODO move it outside the while loop
            if (stop){
                m_pSynchronizingMutex->unlock();
                return NULL;
            }            
        }

        Event *pTemp = queue.front();
        //Retrieve from the FRONT (FIFO)
        queue.pop_front();
        m_pSynchronizingMutex->unlock();
        return pTemp;
    }

 void EventConcurrentQueue::getCurrentEvents(vector<Event *> &eventList) {
        //Guard the retrieval from the queue
        m_pSynchronizingMutex->lock();
        size_t qsize = queue.size(); 
        for(size_t i = 0;i<qsize; i++){
            eventList.push_back(queue.front());
            queue.pop_front();
        }              
        m_pSynchronizingMutex->unlock();
        }

    void EventConcurrentQueue::stopQueue() {
        //Guard the insertion into the queue
        m_pSynchronizingMutex->lock();
        //queue clear
        size_t qsize = queue.size(); 
        for(size_t i = 0;i<qsize; i++){
            Event * evt = queue.front();
            queue.pop_front();
            if(evt) delete(evt);
        }              
        queue.clear();
        stop = true;
        //Notify waiting listeners.
        m_pSynchronizingCondition->resume();
        m_pSynchronizingMutex->unlock();
    }
    /* const char*BlockingQueueTerminateException_C:: what() const throw(){
       return "Queue Terminated";
       }*/

    void EventConcurrentQueue::reStart(){
        queue.clear();
        stop = false;
    }

    bool EventConcurrentQueue::empty(){
        return queue.empty();

    }

    UI32 EventConcurrentQueue::size(){
        return queue.size();
    }
}

/* namespace DcmNs */
