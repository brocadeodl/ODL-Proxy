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
 *  @file: NodeReadyForAllCommandsEvent.cpp
 *  Copyright (C) 2010 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *  Description:Implmentation of NodeReadyForAllCommandsEvent
 *              Class. 
 *  
 *  Author : Aashish Akhouri
 *  Date: 10/20/2010
 */

#include "Framework/Core/NodeReadyForAllCommandsEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{

NodeReadyForAllCommandsEvent::NodeReadyForAllCommandsEvent ()
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_NODE_READY_FOR_ALL_COMMANDS_EVENT),
     m_recoveryType (0)
{
}

NodeReadyForAllCommandsEvent::NodeReadyForAllCommandsEvent (const ResourceId &recoveryType)
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_NODE_READY_FOR_ALL_COMMANDS_EVENT),
     m_recoveryType (recoveryType)
{
}

NodeReadyForAllCommandsEvent::~NodeReadyForAllCommandsEvent ()
{
}

void NodeReadyForAllCommandsEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();
     addSerializableAttribute (new AttributeResourceId (&m_recoveryType, "recoveryType"));
}

ResourceId  NodeReadyForAllCommandsEvent::getRecoveryType () const
{
    return (m_recoveryType);
}

void  NodeReadyForAllCommandsEvent::setRecoveryType (const ResourceId &recoveryType)
{
    m_recoveryType = recoveryType;
}

}
