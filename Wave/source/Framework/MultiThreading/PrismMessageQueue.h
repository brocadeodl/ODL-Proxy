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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMMESSAGEQUEUE_H
#define PRISMMESSAGEQUEUE_H

#include <deque>
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/PrismMutex.h"

using namespace std;

namespace WaveNs
{

template<class T> class PrismMessageQueue // This is a thread safe class
{
    private :
    protected :
    public :
                            PrismMessageQueue              ();
                           ~PrismMessageQueue              ();
        void                insertAtTheBack                (T *&pPrismMessage);
        void                insertAtTheFront               (T *&pPrismMessage);
        void                removeFromFront                ();
        WaveMessageStatus  remove                          (T *&pPrismMessage);
        UI32                removeTimerExpirationsForTimer (const TimerHandle &timerHandle);
        UI32                getSize                        ();
        T                  *getFromFront                   ();
        T                  *removeAndGetFromFront          ();
        bool                isEmpty                        ();

    // Now the data members

    private :
        std::deque<T *> m_messageQueue;
        PrismMutex      m_prismMutex;
    protected :
    public :
};

}

#endif //PRISMMESSAGEQUEUE_H
