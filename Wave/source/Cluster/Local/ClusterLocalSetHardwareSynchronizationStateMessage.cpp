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
 *@file ClusterLocalSetHardwareSynchronizationMessage.cpp
 * Copyright (C) 2011 BrocadeCommunications Systems,Inc.
 * All rights reserved. 
 *
 * Description: This file has the implementation of the message class
 *              handled by ClusterLocalOM which is used for updating
 *              the hardware synchronization state attribute in the
 *              WaveNode Managed Object. 
 *
 * Author :     Brian Adaniya
 * Date :       05/05/2011
 */

#include "Cluster/Local/ClusterLocalSetHardwareSynchronizationStateMessage.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{

ClusterLocalSetHardwareSynchronizationStateMessage::ClusterLocalSetHardwareSynchronizationStateMessage ()
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_SET_HARDWARE_SYNCHRONIZATION_STATE),
    m_hardwareSynchronizationState  (WAVE_NODE_HARDWARE_SYNCHRONIZATION_STATE_UNINITIALIZED)
{
}

ClusterLocalSetHardwareSynchronizationStateMessage::ClusterLocalSetHardwareSynchronizationStateMessage (ResourceId hardwareSynchronizationState)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_SET_HARDWARE_SYNCHRONIZATION_STATE),
    m_hardwareSynchronizationState  (hardwareSynchronizationState)
{
}

ClusterLocalSetHardwareSynchronizationStateMessage::~ClusterLocalSetHardwareSynchronizationStateMessage ()
{
}

void ClusterLocalSetHardwareSynchronizationStateMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeResourceId       (&m_hardwareSynchronizationState, "hardwareSynchronizationState"));
}

ResourceId ClusterLocalSetHardwareSynchronizationStateMessage::getHardwareSynchronizationState () const
{
    return (m_hardwareSynchronizationState);
}

void ClusterLocalSetHardwareSynchronizationStateMessage::setHardwareSynchronizationState (ResourceId hardwareSynchronizationState)
{
    m_hardwareSynchronizationState = hardwareSynchronizationState;
}

}
