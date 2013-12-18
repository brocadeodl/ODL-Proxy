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

/***************************************************************************
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati, Brian Adaniya                              *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeConfigDistributeMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{

VcsNodeConfigDistributeMessage::VcsNodeConfigDistributeMessage ()
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_CONFIG_DISTRIBUTE),
    m_configDistributeOperation             (VCS_CONFIG_DISTRIBUTE_OPERATION_NONE),
    m_vcsClusterType                        (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC),
    m_preserveConfig                        (false),
    m_clusterGlobalOnlyConfigData           (""),
    m_clusterPrimaryLocationId              (0),
    m_force                                 (false),
    m_isDefaultConfigDetected               (false)
{
}

VcsNodeConfigDistributeMessage::VcsNodeConfigDistributeMessage (const ResourceId& vcsClusterType)
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_CONFIG_DISTRIBUTE),
    m_configDistributeOperation             (VCS_CONFIG_DISTRIBUTE_OPERATION_NONE),
    m_vcsClusterType                        (vcsClusterType),
    m_preserveConfig                        (false),
    m_clusterGlobalOnlyConfigData           (""),
    m_clusterPrimaryLocationId              (0),
    m_force                                 (false),
    m_isDefaultConfigDetected               (false)
{
}

VcsNodeConfigDistributeMessage::~VcsNodeConfigDistributeMessage ()
{
}

void VcsNodeConfigDistributeMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeResourceId   (&m_configDistributeOperation,     "configDistributeOperation"));
     addSerializableAttribute (new AttributeResourceId   (&m_vcsClusterType,                "vcsClusterType"));
     addSerializableAttribute (new AttributeBool         (&m_preserveConfig,                "preserveConfig"));
     addSerializableAttribute (new AttributeString       (&m_clusterGlobalOnlyConfigData,   "clusterGlobalOnlyConfigData"));
     addSerializableAttribute (new AttributeLocationId   (&m_clusterPrimaryLocationId,      "clusterPrimaryLocationId"));
     addSerializableAttribute (new AttributeBool         (&m_force,                         "force"));
     addSerializableAttribute (new AttributeBool         (&m_isDefaultConfigDetected,       "isDefaultConfigDetected"));
}

ResourceId VcsNodeConfigDistributeMessage::getConfigDistributeOperation () const
{
    return (m_configDistributeOperation);
}

void VcsNodeConfigDistributeMessage::setConfigDistributeOperation (ResourceId configDistributeOperation)
{
    m_configDistributeOperation = configDistributeOperation;
}

ResourceId VcsNodeConfigDistributeMessage::getVcsClusterType () const
{
    return m_vcsClusterType;
}

void VcsNodeConfigDistributeMessage::setVcsClusterType (const ResourceId& vcsClusterType)
{
    m_vcsClusterType = vcsClusterType;
}

bool VcsNodeConfigDistributeMessage::getPreserveConfig () const
{
    return (m_preserveConfig);
}

void VcsNodeConfigDistributeMessage::setPreserveConfig (bool preserveConfig)
{
    m_preserveConfig = preserveConfig;
}

const string &VcsNodeConfigDistributeMessage::getClusterGlobalOnlyConfigData () const
{
    return (m_clusterGlobalOnlyConfigData);
}

void VcsNodeConfigDistributeMessage::setClusterGlobalOnlyConfigData (const string &clusterGlobalOnlyConfigData)
{
    m_clusterGlobalOnlyConfigData = clusterGlobalOnlyConfigData;
}

LocationId VcsNodeConfigDistributeMessage::getClusterPrimaryLocationId () const
{
    return (m_clusterPrimaryLocationId);
}

void VcsNodeConfigDistributeMessage::setClusterPrimaryLocationId (LocationId clusterPrimaryLocationId)
{
    m_clusterPrimaryLocationId = clusterPrimaryLocationId;
}

bool VcsNodeConfigDistributeMessage::getForce () const
{
    return (m_force);
}

void VcsNodeConfigDistributeMessage::setForce (bool force)
{
    m_force = force;
}

bool VcsNodeConfigDistributeMessage::getIsDefaultConfigDetected () const
{
    return (m_isDefaultConfigDetected);
}

void VcsNodeConfigDistributeMessage::setIsDefaultConfigDetected (bool isDefaultConfigDetected)
{
    m_isDefaultConfigDetected = isDefaultConfigDetected;
}

const LocationRole VcsNodeConfigDistributeMessage::getLocationRole () const
{
    return (m_locationRole);
}

void VcsNodeConfigDistributeMessage::setLocationRole (const LocationRole locationRole)
{
    m_locationRole = locationRole;
}

}
