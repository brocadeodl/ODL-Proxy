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

#ifndef EVENTENGINECONCURRENTQUEUE_H_
#define EVENTENGINECONCURRENTQUEUE_H_
#include "Framework/Utils/PrismCondition.h"
#include "Framework/Utils/PrismMutex.h"
#include <deque>
#include "ClientInterface/vCenter/vCenterMessageDef.h"
using namespace WaveNs;
using namespace std;

namespace DcmNs {
  /*  class BlockingQueueTerminateException_C: public exception{
            public:
                virtual const char* what() const throw();

        }BlockingQueueTerminateException;*/


    class EventEngineConcurrentQueue {
        public:
            EventEngineConcurrentQueue();
            virtual ~EventEngineConcurrentQueue();
            void pushAndNotify(vnm_resp_t *message);
            vnm_resp_t * waitAndPop() throw  ( int);
            void clear();
            void clearAndNotify();
            void stopQueue();
            void reStart();


        private:
            PrismMutex *m_pSynchronizingMutex;
            PrismCondition *m_pSynchronizingCondition;
            std::deque<vnm_resp_t *> queue;
            volatile bool stop;
    };

} /* namespace DcmNs */
#endif /* EVENTENGINECONCURRENTQUEUE_H_ */
