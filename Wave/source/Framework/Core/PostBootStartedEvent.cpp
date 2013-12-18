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
 *  Description:Implementation of PostBootStartedEvent
 *  
 *  Author : Aashish Akhouri
 *  Date: 10/20/2010
 */

#include "Framework/Core/PostBootStartedEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

namespace WaveNs
{

PostBootStartedEvent::PostBootStartedEvent ()
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_POST_BOOT_STARTED_EVENT),
      m_eventId (0),
      m_parameter (0)
	  
{
}

PostBootStartedEvent::PostBootStartedEvent (const UI32 &eventId, const UI32 &parameter)
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_POST_BOOT_STARTED_EVENT),
     m_eventId (eventId),
     m_parameter (parameter)
{
}

PostBootStartedEvent::~PostBootStartedEvent ()
{
}

void PostBootStartedEvent::setupAttributesForSerialization ()
{
  PrismEvent::setupAttributesForSerialization ();
}

UI32 PostBootStartedEvent::getPostBootStartedEventId () const
{
    return m_eventId;
}

void PostBootStartedEvent::setPostBootStartedEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

UI32 PostBootStartedEvent::getPostBootStartedEventParameter () const
{
    return m_parameter;
}

void PostBootStartedEvent::setPostBootStartedEventParameter (const UI32 &parameter)
{
    m_parameter = parameter;
}

}
