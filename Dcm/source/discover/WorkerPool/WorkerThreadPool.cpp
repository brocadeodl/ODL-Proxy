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

/*
 * WorkerThreadPool.cpp
 *
 *  Created on: Mar 27, 2012
 *      Author: rmadapur
 */

#include "discover/WorkerPool/WorkerThreadPool.h"

#include <pthread.h>

namespace DcmNs {

    WorkerThreadPool::WorkerThreadPool(int minThreads, int maxThreads,
            int maxExecutionCount) :
            _minThreads(minThreads), _maxThreads(maxThreads), _maxExecutionCount(
                    maxExecutionCount) {

        VIC_PLUG_LOG(VIC_TRACE_INFO,"WorkerThreadPool Initialize : minThreads =%d,maxThreads = %d\n",minThreads,maxThreads);
        //create the mutex
        _poolAccessMutex = new PrismMutex();
        for ( _numThreads = 0; _numThreads < _minThreads; _numThreads++) {
            new WorkerThread(this);
        }

    }

    WorkerThreadPool::~WorkerThreadPool() {
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","WorkerThreadPool Exiting\n");
        for(size_t i =0;i<_threads.size();i++){
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Cleaning up Worker Thread\n");
            delete(_threads.at(i));
        }
        delete(_poolAccessMutex);
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","WorkerThreadPool Cleanup Completed\n");

    }
    void WorkerThreadPool::addTask(WorkerTask *task) {
        _poolAccessMutex->lock();

        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","WorkerThreadPool Scheduling a new Task\n");
        if (_threads.size() != 0) {
            WorkerThread *pWrapper = _threads.back();
            _threads.pop_back();
            _poolAccessMutex->unlock();

            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","WorkerThreadPool, Thread available for immediate execution of Task\n");
            pWrapper->awake(task);
            return;
        }

        // If we can still create new threads, we do so. This should catch the params as soon
        // as it has started.
        VIC_PLUG_LOG(VIC_TRACE_INFO,"WorkerThreadPool, number of Threads %d\n",_numThreads);
      //  printf("number of Threads %d\n",_numThreads);
        if ((_numThreads < _maxThreads)) {
            new WorkerThread(this);
            _numThreads++;
        }

        /* If no threads are available, we queue the parameters and an incoming thread
         * will manage them when rejoin() is called.
         * At this point, we could also consider launching a new thread to manage the request...
         */
        tasks.push_back(task);
        _poolAccessMutex->unlock();

        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","WorkerThreadPool, coming out of AddTask\n");
    }


     WorkerThreadPool::NextThreadStateT WorkerThreadPool::rejoin(
             WorkerThread* pWrapper) {
         _poolAccessMutex->lock();

         VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","WorkerThreadPool, Entering Rejoin\n");
        /* If there is a queued tasks, we return that to the thread */
        if (tasks.size() != 0) {
            WorkerTask *task = tasks.front();
            tasks.pop_front();
            _poolAccessMutex->unlock();

            pWrapper->setTask(task);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","WorkerThreadPool,Execute:: Task available for execution\n");
            return WorkerThreadPool::Execute;
        }

        /* If we have more than the minimum number of threads, terminate back to the minimum */
        if (_numThreads > _minThreads) {
            _numThreads--;
            _poolAccessMutex->unlock();;

            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","WorkerThreadPool,Die:: Reclaiming extra Threads\n");
            return WorkerThreadPool::Die;
        }

        /* Can check how much the thread has executed, and potentially terminate it in
         * favour of a new thread */
        if ((_maxExecutionCount != UnlimitedLifetime)
                && (pWrapper->getExecutionCount() > _maxExecutionCount)) {
            _poolAccessMutex->unlock();

            // Launch a new thread - this is enough: the thread will join us.
            new WorkerThread(this);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","WorkerThreadPool, Die:: Max execution of thread reached\n");
            return WorkerThreadPool::Die; // Indicate to the calling thread that it should exit
        }

        /* If no parameters are queued, we queue the thread */
        _threads.push_back(pWrapper);

        _poolAccessMutex->unlock();
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","WorkerThreadPool, Sleep:: No task for execution\n");
        return WorkerThreadPool::Sleep;
    }

} /* namespace DCM */
