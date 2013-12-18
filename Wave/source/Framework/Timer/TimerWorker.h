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

#ifndef TIMERWORKER_H
#define TIMERWORKER_H

#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Timer/TimerMessages.h"
#include "Framework/Utils/PrismMutex.h"
#include <vector>
#include <map>

namespace WaveNs
{

class TimerData;

class TimerWorker : public WaveWorker
{
    private :
                void        addTimerToList                          (TimerData *pTimerInfo);
                UI32        removeTimer                             (TimerObjectManagerDeleteTimerMessage *pMessage, UI32 timerId);
                void        removeTimerFromServiceTimersMultimap    (PrismServiceId serviceId, UI32 timerId);
                int         restartTimer                            ();
                UI32        binarySearch                            (int first, int last, timeval &key);
                void        showPendingTimers                       ();
                void        addTimer                                (TimerObjectManagerAddTimerMessage *pMessage);
                void        deleteTimer                             (TimerObjectManagerDeleteTimerMessage *pMessage);
                void        deleteAllTimersForService               (TimerObjectManagerDeleteAllTimersForServiceMessage *pMessage);
                void        removeAllTimers                         ();
                ResourceId  removeAllTimersForService               (UI32 serviceId);

    protected :
    public :
                            TimerWorker                             (WaveObjectManager *pWaveObjectManager);
                           ~TimerWorker                             ();

                void        timerSendOneWay                         (PrismMessage *pPrismMessage);
                void        timerReply                              (PrismMessage *pPrismMessage);
        static  void        processTimeOut                          ();
        static  void        resetMaxDelay                           ();
        static  void        showMaxDelay                            ();
                void        deleteAllTimers                         ();

        // Now the data members

    private :
                vector<TimerData *>             m_timerList;
                multimap<PrismServiceId, UI32>  m_timerIdsByServiceId;
                UI32                            m_nTimers;
                PrismMutex                      m_mutex;
                UI32                            m_currTimerId;
                timeval                         m_maxDelay;

    protected :
    public :

};

class TimerData
{
    private :
    protected :
    public :
                TimerData                       ();
               ~TimerData                       ();

        // Now the data members

    private :
    protected :
    public :
                timeval                         m_periodicInterval;
                timeval                         m_expirationTime;
                UI32                            m_timerId;
                PrismTimerExpirationHandler     m_pPrismTimerExpirationCallback;
                void                           *m_pPrismTimerExpirationContext;
                PrismElement                   *m_pPrismTimerSender;
                PrismServiceId                  m_serviceId;
};

}
#endif //TIMERWORKER_H

