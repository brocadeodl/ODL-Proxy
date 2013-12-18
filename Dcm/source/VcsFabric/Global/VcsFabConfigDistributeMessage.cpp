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
 *@file VcsFabConfigDistributeMessage.h
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 * 
 * Description: This file has the implementaiton of the
 *              message class for a VCS mode transition.
 * 
 * Author :     Brian Adaniya
 *  
 */

#include "VcsFabric/Global/VcsFabConfigDistributeMessage.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace DcmNs
{

VcsFabConfigDistributeMessage::VcsFabConfigDistributeMessage ()
    : PrismMessage (VcsFabObjectManager::getPrismServiceId (), VCS_FABRIC_CONFIG_DISTRIBUTE),
    m_vcsClusterType (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC),
    m_preserveConfig    (false),
    m_force (false)
{
}

VcsFabConfigDistributeMessage::VcsFabConfigDistributeMessage (const ResourceId& vcsClusterType)
    : PrismMessage (VcsFabObjectManager::getPrismServiceId (), VCS_FABRIC_CONFIG_DISTRIBUTE),
     m_vcsClusterType (vcsClusterType),
     m_preserveConfig    (false),
     m_force  (false)
{
}

VcsFabConfigDistributeMessage::~VcsFabConfigDistributeMessage ()
{
}

void VcsFabConfigDistributeMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
    
    addSerializableAttribute (new AttributeUI32Vector   (&m_locationIdVector,   "locationIdVector"));
    addSerializableAttribute (new AttributeResourceId   (&m_vcsClusterType,     "vcsClusterType"));
    addSerializableAttribute (new AttributeBool         (&m_preserveConfig,     "preserveConfig"));
    addSerializableAttribute (new AttributeBool         (&m_force,              "force"));
}

vector<UI32> VcsFabConfigDistributeMessage::getLocationIdVector () const
{
    return m_locationIdVector;
}

void VcsFabConfigDistributeMessage::setLocationIdVector (vector<UI32>& locationIdVector)
{
    m_locationIdVector = locationIdVector;
}

bool VcsFabConfigDistributeMessage::getForce () const
{
    return m_force;
}

void VcsFabConfigDistributeMessage::setForce (bool force)
{
    m_force = force;
}

ResourceId VcsFabConfigDistributeMessage::getVcsClusterType () const
{
    return m_vcsClusterType;
}

void VcsFabConfigDistributeMessage::setVcsClusterType (const ResourceId& vcsClusterType)
{
    m_vcsClusterType = vcsClusterType;
}

bool VcsFabConfigDistributeMessage::getPreserveConfig () const
{
    return (m_preserveConfig);
}

void VcsFabConfigDistributeMessage::setPreserveConfig (bool preserveConfig)
{
    m_preserveConfig = preserveConfig;
}

}
