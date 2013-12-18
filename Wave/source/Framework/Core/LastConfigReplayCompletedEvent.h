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

/**
 *  @file: LastConfigReplayCompletedEvent.h
 *  Copyright (C) 2013 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *  Description:Class Definition for LastConfigReplayCompleted
 *              event.This event is thrown by the framework when
 *              LastConfigReplay completes.
 *  
 *  Author : Brian Adaniya
 *  Date: 01/06/2013
 */

#ifndef LASTCONFIGREPLAYCOMPLETEDEVENT_H
#define LASTCONFIGREPLAYCOMPLETEDEVENT_H

#include "Framework/Messaging/Local/PrismEvent.h"

namespace WaveNs
{

class LastConfigReplayCompletedEvent : public PrismEvent
{
    private :
    protected :
        virtual void                    setupAttributesForSerialization         ();

    public :
                                        LastConfigReplayCompletedEvent          ();
                                        LastConfigReplayCompletedEvent          (const ResourceId &completionStatus);
        virtual                        ~LastConfigReplayCompletedEvent          ();

                ResourceId              getLastConfigReplayCompletedEventStatus () const;
                void                    setLastConfigReplayCompletedEventStatus (const ResourceId &completionStatus);

    // Now the data members

    private :
        ResourceId                      m_lastConfigReplayCompletedEventStatus;

    protected :
    public :
};

}

#endif //LASTCONFIGREPLAYCOMPLETEDEVENT_H

