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
 * PosixThreadPool.h
 *
 *  Created on: Mar 27, 2012
 *      Author: rmadapur
 */

#ifndef POSIXTHREADPOOL_H_
#define POSIXTHREADPOOL_H_
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Utils/PrismCondition.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/MultiThreading/PrismPosixThread.h"
#include <vector>
#include <deque>
#include "discover/WorkerPool/WorkerTask.h"
#include "discover/vsphere/Debug.h"

using namespace WaveNs;
using namespace std;
namespace DcmNs {
    class WorkerThread;
    class WorkerThreadPool {
        public:
            WorkerThreadPool(int minThreads, int maxThreads,
                    int maxExecutionCount);
            virtual ~WorkerThreadPool();
            void addTask(WorkerTask * task);
            enum NextThreadState {
                Execute,
                Sleep,
                Die
            };
            typedef enum NextThreadState NextThreadStateT;
            NextThreadState rejoin(WorkerThread *pWrapper);
            static const int UnlimitedLifetime = -1;
            int getMaxExecutionCount(){
                return _maxExecutionCount;
            }
        private:
            int _minThreads;
            int _maxThreads;
            int _maxExecutionCount;
            /** The current number of threads that exist */
            int _numThreads;
            PrismMutex *_poolAccessMutex;
            vector<WorkerThread *> _threads;
            deque<WorkerTask *> tasks; ///< Vector of tasks still to be processed

            //TODO kill all threads using pthread_kill and cleanup the pending tasks.

    };

    class WorkerThreadCondition {
            PrismMutex *_mutex;
            PrismCondition *_condition;
            bool _conditionFlag;
        public:
            WorkerThreadCondition();
            virtual ~WorkerThreadCondition();
            void signalCondition();
            void waitForCondition();
    };

    class WorkerThread: public PrismPosixThread {
        public:
            WorkerThread(WorkerThreadPool *pool);
            virtual ~WorkerThread();
            static long ThreadRoutingFunction(void* pParam);
            void setTask(WorkerTask *task);
            WorkerTask *getTask();
            void awake(WorkerTask *task);


            virtual WaveThreadStatus start();
            long getExecutionCount() {
                return executionCount;
            }
        private:
            WorkerThreadPool *pPool;
            WorkerTask *task;
            long executionCount;

            /** The thread is sleeping */
            WorkerThreadCondition *sleepingCondition;
            /** The thread should awake */
            WorkerThreadCondition *awakeCondition;


            long execute();
            void Sleep();

    };

} /* namespace DCM */
#endif /* POSIXTHREADPOOL_H_ */
