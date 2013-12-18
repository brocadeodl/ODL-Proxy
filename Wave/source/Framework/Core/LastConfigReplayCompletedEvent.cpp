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
 *  @file: LastConfigReplayCompletedEvent.cpp
 *  Copyright (C) 2013 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *  Description:Implementation  for LastConfigReplayCompletedEvent
 *  
 *  Author : Brian Adaniya
 *  Date: 01/06/2013
 */ 
 
#include "Framework/Core/LastConfigReplayCompletedEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

namespace WaveNs
{

LastConfigReplayCompletedEvent::LastConfigReplayCompletedEvent ()
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_LAST_CONFIG_REPLAY_COMPLETED_EVENT),
      m_lastConfigReplayCompletedEventStatus (WAVE_MESSAGE_SUCCESS)
{
}

LastConfigReplayCompletedEvent::LastConfigReplayCompletedEvent (const ResourceId &completionStatus)
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_LAST_CONFIG_REPLAY_COMPLETED_EVENT),
      m_lastConfigReplayCompletedEventStatus (completionStatus)
{
}

LastConfigReplayCompletedEvent::~LastConfigReplayCompletedEvent ()
{
}

void LastConfigReplayCompletedEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();
}

ResourceId LastConfigReplayCompletedEvent::getLastConfigReplayCompletedEventStatus () const
{
    return (m_lastConfigReplayCompletedEventStatus);
}

void LastConfigReplayCompletedEvent::setLastConfigReplayCompletedEventStatus (const ResourceId &completionStatus)
{
    m_lastConfigReplayCompletedEventStatus = completionStatus;
}

}
