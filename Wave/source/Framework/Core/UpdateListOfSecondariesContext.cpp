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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/UpdateListOfSecondariesContext.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

UpdateListOfSecondariesContext::UpdateListOfSecondariesContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),m_isThisNodeNew(false)
{
}

UpdateListOfSecondariesContext::UpdateListOfSecondariesContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),m_isThisNodeNew(false)
{
}

UpdateListOfSecondariesContext::~UpdateListOfSecondariesContext ()
{
}

void UpdateListOfSecondariesContext::addSecondaryNodeDetails (const LocationId &locationId, const string &ipAddress, const UI32 &port, bool isNodeNew)
{
    m_locationIdsForSecondaryNodes.push_back (locationId);
    m_IpAddressesForSecondaryNodes.push_back (ipAddress);
    m_portsForSecondaryNodes.push_back       (port);
    m_isNodeNewlyAdded.push_back             (isNodeNew);
}

void UpdateListOfSecondariesContext::setIsThisNodeNew(bool thisNodeNew)
{
    m_isThisNodeNew = thisNodeNew;
}

UI32 UpdateListOfSecondariesContext::getNumberOfSecondaryLocations () const
{
    UI32 numberOfSecondaryLocations1 = m_locationIdsForSecondaryNodes.size ();
    UI32 numberOfSecondaryLocations2 = m_IpAddressesForSecondaryNodes.size ();
    UI32 numberOfSecondaryLocations3 = m_portsForSecondaryNodes.size ();
    UI32 numberOfSecondaryLocations4 = m_isNodeNewlyAdded.size ();

    prismAssert (numberOfSecondaryLocations1 == numberOfSecondaryLocations2, __FILE__, __LINE__);
    prismAssert (numberOfSecondaryLocations2 == numberOfSecondaryLocations3, __FILE__, __LINE__);
    prismAssert (numberOfSecondaryLocations3 == numberOfSecondaryLocations4, __FILE__, __LINE__);

    return (numberOfSecondaryLocations1);
}

LocationId UpdateListOfSecondariesContext::getLocationIdAtIndex (const UI32 &index) const
{
    UI32 numberOfSecondaryLocations = getNumberOfSecondaryLocations ();

    prismAssert (index < numberOfSecondaryLocations, __FILE__, __LINE__);

    return (m_locationIdsForSecondaryNodes[index]);
}

string UpdateListOfSecondariesContext::getIpAddressAtIndex (const UI32 &index) const
{
    UI32 numberOfSecondaryLocations = getNumberOfSecondaryLocations ();

    prismAssert (index < numberOfSecondaryLocations, __FILE__, __LINE__);

    return (m_IpAddressesForSecondaryNodes[index]);
}

UI32 UpdateListOfSecondariesContext::getPortAtIndex (const UI32 &index) const
{
    UI32 numberOfSecondaryLocations = getNumberOfSecondaryLocations ();

    prismAssert (index < numberOfSecondaryLocations, __FILE__, __LINE__);

    return (m_portsForSecondaryNodes[index]);
}

bool UpdateListOfSecondariesContext::isNodeNewAtIndex(const UI32 &index) const
{
    UI32 numberOfSecondaryLocations = getNumberOfSecondaryLocations ();

    prismAssert (index < numberOfSecondaryLocations, __FILE__, __LINE__);

    return (m_isNodeNewlyAdded[index]);
}

bool UpdateListOfSecondariesContext::isThisNodeNew () const
{
    return m_isThisNodeNew;
}

void UpdateListOfSecondariesContext::setConnectedLocationsAtPrimary (vector<LocationId> &locationIds)
{
    m_connectedLocationsAtPrimary = locationIds;
}

void UpdateListOfSecondariesContext::getConnectedLocationsAtPrimary (vector<LocationId> &locationIds)
{
    locationIds = m_connectedLocationsAtPrimary;
}

}
