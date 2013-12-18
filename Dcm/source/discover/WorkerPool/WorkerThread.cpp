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
 * WorkerThread.cpp
 *
 *  Created on: Mar 27, 2012
 *      Author: rmadapur
 */
#include "discover/WorkerPool/WorkerThreadPool.h"

namespace DcmNs {

    WaveThreadStatus WorkerThread::start() {
        execute();
        return (WAVE_THREAD_SUCCESS);
    }
    WorkerThread::WorkerThread(WorkerThreadPool *pool) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                "WorkerThread Creating wrapper");
        this->executionCount = 0;
        this->pPool = pool;

        /* Create the Event we will use to launch and signal sleeping */

        awakeCondition = new WorkerThreadCondition();

        sleepingCondition = new WorkerThreadCondition();

        this->run();

    }

    WorkerThread::~WorkerThread() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "WorkerThread cleanup started");

        delete (awakeCondition);

        delete (sleepingCondition);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                "WorkerThread cleanup completed");

    }

    long WorkerThread::execute() {
        WorkerThreadPool::NextThreadStateT nextState;
        while ((nextState = pPool->rejoin(this)) != WorkerThreadPool::Die) {
            // Rejoin will set pParam if possible
            if (nextState == WorkerThreadPool::Sleep) {
                Sleep();
            }
            // We've been awoken, so we execute the function we're bound to.
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                    "WorkerThread :Execute the task");
            if (task){

                task->threadId = this->getId();
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                                                                        "WorkerThread : Task thread Id %d",task->threadId);
                task->run();
                    task->cleanup();
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "WorkerThread : Task requested calling cleanup");
                    executionCount =pPool->getMaxExecutionCount();
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                                                   "WorkerThread : Task completed execution");
            }
            executionCount++; // Track the number of executions


            setTask(NULL); // So we don't loop endlessly with the same parameter
        }
        // The thread exits if the pool won't accept it's rejoin - deleting itself on exit
        this->stopCallingThread();
        delete this;
        return 0;
    }
    void WorkerThread::setTask(WorkerTask *workertask) {
        this->task = workertask;
    }
    WorkerTask* WorkerThread::getTask() {
        return task;
    }
    void WorkerThread::Sleep() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s","WorkerThread, Calling Sleep");

        sleepingCondition->signalCondition();

        awakeCondition->waitForCondition();

        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                "WorkerThread, Calling Sleep Done");

    }
    void WorkerThread::awake(WorkerTask *workerTask) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "WorkerThread, Calling Awake");

        sleepingCondition->waitForCondition();

        setTask(workerTask);

        awakeCondition->signalCondition();

        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                "WorkerThread, Calling Awake done");

    }

    WorkerThreadCondition::WorkerThreadCondition() {
        _mutex = new PrismMutex();
        _condition = new PrismCondition(_mutex);
        _conditionFlag = false;
    }
    WorkerThreadCondition::~WorkerThreadCondition() {
        delete (_mutex);
        delete (_condition);

    }

    void WorkerThreadCondition::signalCondition() {
        _mutex->lock();
        _conditionFlag = true;
        _condition->resume();
        _mutex->unlock();
    }
    void WorkerThreadCondition::waitForCondition() {
        _mutex->lock();
        while (!_conditionFlag) {
            _condition->wait();
        }
        _conditionFlag = false;
        _mutex->unlock();
    }

} /* namespace DCM */
