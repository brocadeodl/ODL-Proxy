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

/**************************************************************************
*   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
*   All rights reserved.                                                  *
*   Author : Sudhakar Jha                                               *
***************************************************************************/

#include "VcsFabric/Global/VcsClusterConnectionToIpEnqSequencerContext.h"

namespace DcmNs
{

VcsClusterConnectionToIpEnqSequencerContext::VcsClusterConnectionToIpEnqSequencerContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps)
{
}

VcsClusterConnectionToIpEnqSequencerContext::~VcsClusterConnectionToIpEnqSequencerContext ()
{
}

void        VcsClusterConnectionToIpEnqSequencerContext::addLocationToMap (const LocationId& lid, const UI32 rbridgeId)
{
    m_locationToRbridgeidMap[lid] = rbridgeId;
}

UI32       VcsClusterConnectionToIpEnqSequencerContext::getRbridgeIdForLocation (LocationId lid)
{
    UI32 rbridgeId = m_locationToRbridgeidMap[lid];
    return rbridgeId;     
}


void        VcsClusterConnectionToIpEnqSequencerContext::addPingResponseToMap (const UI32 rbridgeId, IpAddressPingResponseParameters& pingResponse)
{
    m_pingResponseInfoMap[rbridgeId] = pingResponse;
}

IpAddressPingResponseParameters&    VcsClusterConnectionToIpEnqSequencerContext::getPingResponseForRbridgeId (const UI32 rbridgeId)
{
    return     m_pingResponseInfoMap[rbridgeId];
}

map<UI32, IpAddressPingResponseParameters>&  VcsClusterConnectionToIpEnqSequencerContext::getPingResponseForAllRbridgeId ()
{
    return m_pingResponseInfoMap;
}



}

