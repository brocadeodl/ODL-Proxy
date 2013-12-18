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
 *  @file: ConfigReplayStartedEvent.h
 *  Copyright (C) 2010 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *  Description:Class Definition for ConfigReplayStart
 *              event.This event is thrown by the framework
 *              when config replay starts.
 *  
 *  Author : Aashish Akhouri
 *  Date: 10/20/2010
 */

#ifndef CONFIGREPLAYSTARTEDEVENT_H
#define CONFIGREPLAYSTARTEDEVENT_H
#include "Framework/Messaging/Local/PrismEvent.h"

namespace WaveNs
{

class ConfigReplayStartedEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                 ConfigReplayStartedEvent ();
                 ConfigReplayStartedEvent (const bool &flag);
        virtual ~ConfigReplayStartedEvent ();
                bool getConfigFileReplayFlag () const;
                void setConfigFileReplayFlag (const bool &flag);

    private :
                bool m_configFileReplayFlag;
    protected :
    public :
};

}

#endif //CONFIGREPLAYSTARTEDEVENT_H

