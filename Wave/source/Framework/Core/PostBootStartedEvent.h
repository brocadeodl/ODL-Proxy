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
 *  @file: PostBootStartedEvent.h
 *  Copyright (C) 2010 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *  Description:Class Definition for PostBootStarted
 *              event.This event is thrown by the framework when
 *              PostBoot Starts.
 *  
 *  Author : Aashish Akhouri
 *  Date: 10/20/2010
 */
#ifndef POSTBOOTSTARTEDEVENT_H
#define POSTBOOTSTARTEDEVENT_H
#include "Framework/Messaging/Local/PrismEvent.h"

namespace WaveNs
{

class PostBootStartedEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                 PostBootStartedEvent ();
                 PostBootStartedEvent (const UI32 &eventId, const UI32 &parameter);
        virtual ~PostBootStartedEvent ();

                UI32 getPostBootStartedEventId () const; 
                UI32 getPostBootStartedEventParameter () const;
                void setPostBootStartedEventId (const UI32 &eventId);
                void setPostBootStartedEventParameter (const UI32 &parameter);

    private :
                UI32 m_eventId;
                UI32 m_parameter;
    protected :
    public :
};

}

#endif //POSTBOOTSTARTEDEVENT_H

