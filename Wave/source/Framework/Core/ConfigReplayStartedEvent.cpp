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
 *  @file: ConfigReplayStartedEvent.cpp
 *  Copyright (C) 2010 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *  Description:Implementation of ConfigReplayStartedEvent
 *  Author : Aashish Akhouri
 *  Date: 10/20/2010
 */

#include "Framework/Core/ConfigReplayStartedEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

namespace WaveNs
{

ConfigReplayStartedEvent::ConfigReplayStartedEvent ()
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIG_REPLAY_STARTED_EVENT),
    m_configFileReplayFlag (false)
{
}

ConfigReplayStartedEvent::ConfigReplayStartedEvent (const bool &flag)
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIG_REPLAY_STARTED_EVENT),
    m_configFileReplayFlag (flag)
{
}

ConfigReplayStartedEvent::~ConfigReplayStartedEvent ()
{
}

void ConfigReplayStartedEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();
}

bool ConfigReplayStartedEvent::getConfigFileReplayFlag () const
{
    return m_configFileReplayFlag;
}

void ConfigReplayStartedEvent::setConfigFileReplayFlag (const bool &flag)
{
    m_configFileReplayFlag = flag;
}

}
