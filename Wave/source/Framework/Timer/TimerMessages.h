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

#ifndef TIMERMESSAGES_H
#define TIMERMESSAGES_H

#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Timer/TimerTypes.h"

namespace WaveNs
{

class TimerObjectManagerAddTimerMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual                            ~TimerObjectManagerAddTimerMessage   ();
                                            TimerObjectManagerAddTimerMessage   ();
                                            TimerObjectManagerAddTimerMessage   (timeval &startInterval, timeval &periodicInterval, timeval &startTime, PrismTimerExpirationHandler pPrismTimerExpirationCallback, void *pPrismTimerExpirationContext, PrismElement *pPrismTimerSender);
                TimerHandle                 getTimerId                          ();
                timeval                     getStartInterval                    ();
                timeval                     getPeriodicInterval                 ();
                timeval                     getStartTime                        ();
                PrismTimerExpirationHandler getTimerExpirationCallback          ();
                void                       *getTimerExpirationContext           ();
                PrismElement               *getTimerSender                      ();
                void                        setTimerId                          (TimerHandle timerId);

    // Now the data members

    private :
        timeval                     m_startInterval;
        timeval                     m_periodicInterval;
        timeval                     m_startTime;
        TimerHandle                 m_timerId;
        PrismTimerExpirationHandler m_pPrismTimerExpirationCallback;
        void                       *m_pPrismTimerExpirationContext;
        PrismElement               *m_pPrismTimerSender;
    protected :
    public :
};

class TimerObjectManagerDeleteTimerMessage : public PrismMessage
{
    private :
    protected :
    public :
                    TimerObjectManagerDeleteTimerMessage    ();
                    TimerObjectManagerDeleteTimerMessage    (TimerHandle timerId);
        UI32        getTimerId                              ();
    // Now the data members

    private :
    protected :
    public :
        UI32        m_timerId;

};

class TimerObjectManagerDeleteAllTimersForServiceMessage : public PrismMessage
{
    private :
    protected :
    public :
                    TimerObjectManagerDeleteAllTimersForServiceMessage  ();
                   ~TimerObjectManagerDeleteAllTimersForServiceMessage  ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // TIMERMESSAGES_H
