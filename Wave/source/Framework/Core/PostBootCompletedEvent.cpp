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
 *  @file: PostBootCompletedEvent.cpp
 *  Copyright (C) 2010 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *  Description:Implementation  for PostBootCompletedEvent
 *  
 *  Author : Aashish Akhouri
 *  Date: 10/20/2010
 */ 
 
#include "Framework/Core/PostBootCompletedEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

namespace WaveNs
{

PostBootCompletedEvent::PostBootCompletedEvent ()
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_POST_BOOT_COMPLETED_EVENT),
      m_eventId (0),
      m_parameter (0)
{
}

PostBootCompletedEvent::PostBootCompletedEvent (const UI32 &eventId, const UI32 &parameter)
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_POST_BOOT_COMPLETED_EVENT),
     m_eventId (eventId),
     m_parameter (parameter)
{
}

PostBootCompletedEvent::~PostBootCompletedEvent ()
{

}

void PostBootCompletedEvent::setupAttributesForSerialization ()
{
  PrismEvent::setupAttributesForSerialization ();
}

UI32 PostBootCompletedEvent::getPostBootCompletedEventId () const
{
    return m_eventId;
}

void PostBootCompletedEvent::setPostBootCompletedEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

UI32 PostBootCompletedEvent::getPostBootCompletedEventParameter () const
{
    return m_parameter;
}

void PostBootCompletedEvent::setPostBootCompletedEventParameter (const UI32 &parameter)
{
    m_parameter = parameter;
}


}
