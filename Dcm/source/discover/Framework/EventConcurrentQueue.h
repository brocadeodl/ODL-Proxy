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
 *   Author : Sada Malladi
 **************************************************************************/

#ifndef EVENTCONCURRENTQUEUE_H_
#define EVENTCONCURRENTQUEUE_H_
#include "Framework/Utils/PrismCondition.h"
#include "Framework/Utils/PrismMutex.h"
#include <deque>
#include "discover/vsphere/vim25/Event.h"
using namespace WaveNs;
using namespace std;

namespace DcmNs {

    class EventConcurrentQueue {
        public:
            EventConcurrentQueue();
            virtual ~EventConcurrentQueue();
            void pushAndNotify(Event *message);
            void getCurrentEvents(vector<Event *> &eventList);
            Event * waitAndPop() ;
            void clear();
            void clearAndNotify();
            void stopQueue();
            void reStart();

        private:
            UI32 size();
            bool empty();
            PrismMutex *m_pSynchronizingMutex;
            PrismCondition *m_pSynchronizingCondition;
            std::deque<Event *> queue;
            volatile bool stop;
    };

} /* namespace DcmNs */
#endif /* EVENTCONCURRENTQUEUE_H_ */
