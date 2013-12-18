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
 *  @file: PostBootCompletedEvent.h
 *  Copyright (C) 2010 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *  Description:Class Definition for PostBootCompleted
 *              event.This event is thrown by the framework when
 *              PostBoot completes.
 *  
 *  Author : Aashish Akhouri
 *  Date: 10/20/2010
 */
#ifndef POSTBOOTCOMPLETEDEVENT_H
#define POSTBOOTCOMPLETEDEVENT_H
#include "Framework/Messaging/Local/PrismEvent.h"

namespace WaveNs
{

class PostBootCompletedEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                 PostBootCompletedEvent ();
				 PostBootCompletedEvent (const UI32 &eventId, const UI32 &parameter);
        virtual ~PostBootCompletedEvent ();

                UI32 getPostBootCompletedEventId () const;
                UI32 getPostBootCompletedEventParameter () const;
                void setPostBootCompletedEventId (const UI32 &eventId);
                void setPostBootCompletedEventParameter (const UI32 &parameter);

    // Now the data members

    private :
                UI32 m_eventId;
                UI32 m_parameter;
    protected :
    public :


};

}

#endif //POSTBOOTCOMPLETEDEVENT_H

