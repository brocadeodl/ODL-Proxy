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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeConfigDistributeContext.h"

namespace DcmNs
{

VcsNodeConfigDistributeContext::VcsNodeConfigDistributeContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismSynchronousLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
    m_vcsClusterType              (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC),
    m_contextFilteredConfigType   (DCM_CONFIG_FILTER_CONTEXT_NONE),    
    m_contextFilteredConfigData   (""),
    m_preserveConfig              (false),
    m_isDefaultConfigDetected     (false),
    m_locationRole                (LOCATION_STAND_ALONE),
    m_force                       (false)
{
}

VcsNodeConfigDistributeContext::~VcsNodeConfigDistributeContext ()
{
}

const ResourceId VcsNodeConfigDistributeContext::getVcsClusterType () const
{
    return (m_vcsClusterType);
}

void  VcsNodeConfigDistributeContext::setVcsClusterType (const ResourceId vcsClusterType)
{
    m_vcsClusterType = vcsClusterType;
}

const ResourceId VcsNodeConfigDistributeContext::getContextFilteredConfigType () const
{
    return (m_contextFilteredConfigType);
}

void VcsNodeConfigDistributeContext::setContextFilteredConfigType (const DcmConfigFilterContextTypes contextFilteredConfigType)
{
    m_contextFilteredConfigType = contextFilteredConfigType;
}

const string &VcsNodeConfigDistributeContext::getContextFilteredConfigData () const
{
    return (m_contextFilteredConfigData);
}

void VcsNodeConfigDistributeContext::setContextFilteredConfigData (const string &contextFilteredConfigData)
{
    m_contextFilteredConfigData = contextFilteredConfigData;
}

const bool VcsNodeConfigDistributeContext::getPreserveConfig () const
{
    return (m_preserveConfig);
}

void VcsNodeConfigDistributeContext::setPreserveConfig (const bool preserveConfig)
{
    m_preserveConfig = preserveConfig;
}

const bool VcsNodeConfigDistributeContext::getIsDefaultConfigDetected () const
{
    return (m_isDefaultConfigDetected);
}

void VcsNodeConfigDistributeContext::setIsDefaultConfigDetected (const bool isDefaultConfigDetected)
{
    m_isDefaultConfigDetected = isDefaultConfigDetected;
}

const bool VcsNodeConfigDistributeContext::getForce () const
{
    return (m_force);
}

void VcsNodeConfigDistributeContext::setForce (const bool force)
{
    m_force = force;
}

const LocationRole VcsNodeConfigDistributeContext::getLocationRole () const
{
    return (m_locationRole);
}

void VcsNodeConfigDistributeContext::setLocationRole (const LocationRole locationRole)
{
    m_locationRole = locationRole;
}

}
