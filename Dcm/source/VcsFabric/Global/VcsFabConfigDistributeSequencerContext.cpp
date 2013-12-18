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
 *@file VcsFabConfigDistributeSequencerContext.cpp
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc.
 * All rights reserved. Description: This file extends the
 * LinearSequencerContext for config distribution.
 *
 * Author :     Brian Adaniya
 * Date :       07/13/2012
 */

#include "VcsFabric/Global/VcsFabConfigDistributeSequencerContext.h"

namespace DcmNs
{
VcsFabConfigDistributeSequencerContext::VcsFabConfigDistributeSequencerContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
    m_vcsClusterType                        (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC),
    m_preserveConfig                        (false),
    m_clusterPrimaryLocationId              (0),
    m_force                                 (false),       
    m_clusterGlobalOnlyConfigData           (""),
    m_isDefaultConfigDetected               (false),
    m_locationRole                          (LOCATION_STAND_ALONE)
{
}

VcsFabConfigDistributeSequencerContext::~VcsFabConfigDistributeSequencerContext ()
{
}

const vector<LocationId> &VcsFabConfigDistributeSequencerContext::getLocationIdVector () const
{
    return (m_locationIds);
}

void VcsFabConfigDistributeSequencerContext::setLocationIdVector (const vector<LocationId> &locationIds)
{
    m_locationIds = locationIds;
}

bool VcsFabConfigDistributeSequencerContext::getForce () const
{
    return (m_force);
}

void VcsFabConfigDistributeSequencerContext::setForce (const bool force)
{
    m_force = force;
}

ResourceId VcsFabConfigDistributeSequencerContext::getVcsClusterType () const
{
    return (m_vcsClusterType);
}

void VcsFabConfigDistributeSequencerContext::setVcsClusterType (ResourceId vcsClusterType)
{
    m_vcsClusterType = vcsClusterType;
}

bool VcsFabConfigDistributeSequencerContext::getPreserveConfig () const
{
    return (m_preserveConfig);
}

void VcsFabConfigDistributeSequencerContext::setPreserveConfig (const bool preserveConfig)
{
    m_preserveConfig = preserveConfig;
}

LocationId VcsFabConfigDistributeSequencerContext::getClusterPrimaryLocationId () const
{
    return (m_clusterPrimaryLocationId);
}

void VcsFabConfigDistributeSequencerContext::setClusterPrimaryLocationId (const LocationId clusterPrimaryLocationId)
{
    m_clusterPrimaryLocationId = clusterPrimaryLocationId;
}

const string &VcsFabConfigDistributeSequencerContext::getClusterGlobalOnlyConfigData () const
{
    return (m_clusterGlobalOnlyConfigData);
}

void VcsFabConfigDistributeSequencerContext::setClusterGlobalOnlyConfigData (const string &clusterGlobalOnlyConfigData)
{
    m_clusterGlobalOnlyConfigData = clusterGlobalOnlyConfigData;
}

bool VcsFabConfigDistributeSequencerContext::getIsDefaultConfigDetected () const
{
    return (m_isDefaultConfigDetected);
}

void VcsFabConfigDistributeSequencerContext::setIsDefaultConfigDetected (const bool isDefaultConfigDetected)
{
    m_isDefaultConfigDetected = isDefaultConfigDetected;
}

LocationRole VcsFabConfigDistributeSequencerContext::getLocationRole () const
{
    return (m_locationRole);
}

void VcsFabConfigDistributeSequencerContext::setLocationRole (const LocationRole locationRole)
{
    m_locationRole = locationRole;
}

}
