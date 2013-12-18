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
 *  @file: ConfigReplayCompletedEvent.h
 *  Copyright (C) 2010 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *  Description:Class Definition for ConfigReplayCompletedEvent
 *              event.This event is thrown by the framework
 *              when config replay completes.
 *  
 *  Author : Aashish Akhouri
 *  Date: 10/20/2010
 */

#ifndef CONFIGREPLAYCOMPLETEDEVENT_H
#define CONFIGREPLAYCOMPLETEDEVENT_H
#include "Framework/Messaging/Local/PrismEvent.h"

namespace WaveNs
{

class ConfigReplayCompletedEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                 ConfigReplayCompletedEvent ();
                 ConfigReplayCompletedEvent (const ResourceId &recoveryType);
        virtual ~ConfigReplayCompletedEvent ();
                 ResourceId   getRecoveryType () const;
         void    setRecoveryType (const ResourceId &recoveryType);

    private :
                 ResourceId   m_recoveryType;
    protected :
    public :
};

}

#endif //CONFIGREPLAYCOMPLETEDEVENT_H

