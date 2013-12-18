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
 * Task.h
 *
 *  Created on: Mar 27, 2012
 *      Author: rmadapur
 */

#ifndef TASK_H_
#define TASK_H_
#include "Framework/MultiThreading/PrismPosixThread.h"

using namespace WaveNs;
namespace DcmNs {

    class WorkerTask {

        public:
            volatile bool stopTask;
            PrismThreadId threadId;
            WorkerTask();
            virtual ~WorkerTask();
            //Child class should implement this method
            virtual void run();
            void stop();

            virtual void cleanup();

    };

} /* namespace DCM */
#endif /* TASK_H_ */
