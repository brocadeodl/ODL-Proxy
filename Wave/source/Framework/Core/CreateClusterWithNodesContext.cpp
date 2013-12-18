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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/CreateClusterWithNodesContext.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

CreateClusterWithNodesContext::CreateClusterWithNodesContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps), m_isDeletion(false)
{
    m_isCreateClusterContext = false;
}

CreateClusterWithNodesContext::CreateClusterWithNodesContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),m_isDeletion(false)
{
    m_isCreateClusterContext = false;
}

CreateClusterWithNodesContext::~CreateClusterWithNodesContext ()
{
}

bool CreateClusterWithNodesContext::doesLocationExist (const LocationId &locationId)
{
    vector<LocationId>::iterator element = m_newLocationIdsVector.begin ();
    vector<LocationId>::iterator end     = m_newLocationIdsVector.end ();

    while (end != element)
    {
        if (locationId == (*element))
        {
            return (true);
        }

        element++;
    }

    return (false);
}

void CreateClusterWithNodesContext::addNewLocationId (const LocationId &locationId)
{
    if (false == (doesLocationExist (locationId)))
    {
        m_newLocationIdsVector.push_back (locationId);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "CreateClusterWithNodesContext::addNewLocationId : Trying to add already existing location.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

void CreateClusterWithNodesContext::removeNewLocationId (const LocationId &locationId)
{
    vector<LocationId>::iterator element = m_newLocationIdsVector.begin ();
    vector<LocationId>::iterator end     = m_newLocationIdsVector.end ();

    while (end != element)
    {
        if (locationId == (*element))
        {
            m_newLocationIdsVector.erase (element);
            return;
        }

        element++;
    }

    trace (TRACE_LEVEL_FATAL, "CreateClusterWithNodesContext::removeNewLocationId : Trying to remove non-existing location.");
    prismAssert (false, __FILE__, __LINE__);
}

LocationId CreateClusterWithNodesContext::getNewLocationIdAt (const UI32 &i)
{
    if (i < (m_newLocationIdsVector.size ()))
    {
        return (m_newLocationIdsVector[i]);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("CreateClusterWithNodesContext::getNewLocationIdAt : Index (") + i + ") is out of range (" + m_newLocationIdsVector.size () + ").");
        prismAssert (false, __FILE__, __LINE__);
        return (0);
    }
}

UI32 CreateClusterWithNodesContext::getNumberOfNewLocationIds ()
{
    return (m_newLocationIdsVector.size ());
}

void CreateClusterWithNodesContext::addValidationDetailsForService (const PrismServiceId &prismServiceId, void *pValidationDetials, const UI32 size)
{
    m_prismServiceIdsVector.push_back        (prismServiceId);
    m_validationDetailsVector.push_back      (pValidationDetials);
    m_validationDetailsSizesVector.push_back (size);
}

vector<PrismServiceId> &CreateClusterWithNodesContext::getPrismServiceIdsVector ()
{
    return (m_prismServiceIdsVector);
}

vector<void *> &CreateClusterWithNodesContext::getValidationDetailsVector ()
{
    return (m_validationDetailsVector);
}

vector<UI32> &CreateClusterWithNodesContext::getValidationDetailsSizesVector ()
{
    return (m_validationDetailsSizesVector);
}

void CreateClusterWithNodesContext::addValidaionResultsLocation (const string &ipAddress, const SI32 &port)
{
    m_prismLocationIpAddressesForValidationResultsVector.push_back (ipAddress);
    m_prismLocationPortsForValidationResultsVector.push_back (port);
}

UI32 CreateClusterWithNodesContext::getNumberOfResultsLocations ()
{
    prismAssert (m_prismLocationIpAddressesForValidationResultsVector.size () == m_prismLocationPortsForValidationResultsVector.size (), __FILE__, __LINE__);

    return (m_prismLocationIpAddressesForValidationResultsVector.size ());
}

void CreateClusterWithNodesContext::getResultsLocationAt (const UI32 &i, string &ipAddress, SI32 &port)
{
    ipAddress = "";
    port      = 0;

    if (i < getNumberOfResultsLocations ())
    {
        ipAddress = m_prismLocationIpAddressesForValidationResultsVector[i];
        port      = m_prismLocationPortsForValidationResultsVector[i];
    }
}

void CreateClusterWithNodesContext::addValidationResultsForService (const string &ipAddress, const SI32 &port, const PrismServiceId &prismServiceId, void *const &pValidationResults, const UI32 &validationResultsSize)
{
    string uniqueIdString = ipAddress + string (":") + port + string (":") + prismServiceId;

    m_prismLocationValidationResultsVector[uniqueIdString]      = pValidationResults;
    m_prismLocationValidationResultsSizesVector[uniqueIdString] = validationResultsSize;
}

void CreateClusterWithNodesContext::getValidationResultsForService (const string &ipAddress, const SI32 &port, const PrismServiceId &prismServiceId, void *&pValidationResults, UI32 &validationResultsSize)
{
    string uniqueIdString = ipAddress + string (":") + port + string (":") + prismServiceId;

    pValidationResults    = NULL;
    validationResultsSize = 0;

    map<string, void *>::iterator validationResultsElement = m_prismLocationValidationResultsVector.find (uniqueIdString);
    map<string, void *>::iterator validationResultsEnd     = m_prismLocationValidationResultsVector.end ();

    if (validationResultsElement != validationResultsEnd)
    {
        pValidationResults = validationResultsElement->second;
    }
    else
    {
        return;
    }

    map<string, UI32>::iterator validationResultsSizeElement = m_prismLocationValidationResultsSizesVector.find (uniqueIdString);
    map<string, UI32>::iterator validationResultsSizeEnd     = m_prismLocationValidationResultsSizesVector.end ();

    if (validationResultsSizeElement != validationResultsSizeEnd)
    {
        validationResultsSize = validationResultsSizeElement->second;
    }
    else
    {
        // We found the validation results but could not find the size of the validation results.  This must not happen.
        prismAssert (false, __FILE__, __LINE__);
    }
}

vector<PrismServiceId> &CreateClusterWithNodesContext::getPrismServiceIdsToCommunicate ()
{
    return (m_prismServiceIdsToCommunicate);
}

bool CreateClusterWithNodesContext::getIsDeletion ()    const
{
    return m_isDeletion;
}
void  CreateClusterWithNodesContext::setIsDeletion (bool deletion)
{
    m_isDeletion = deletion;
}

void CreateClusterWithNodesContext::addToSuccessfullyAddedLocationIdVector (LocationId locationId)
{
    m_successfullyAddedLocationIdVector.push_back (locationId);
}

vector<LocationId> CreateClusterWithNodesContext::getSuccessfullyAddedLocationIdVector () const
{
    return (m_successfullyAddedLocationIdVector);
}

void CreateClusterWithNodesContext::addToFailedLocationIdVector (LocationId locationId)
{
    if (isAFailedLocation (locationId))
    {
        return;
    }
   
    m_failedLocationIds.push_back (locationId); 
}

vector<LocationId> CreateClusterWithNodesContext::getFailedLocationIdVector () const
{
    return (m_failedLocationIds);
}

bool CreateClusterWithNodesContext::isAFailedLocation (LocationId locationId)
{
    vector<PrismServiceId>::iterator element = m_failedLocationIds.begin ();
    vector<PrismServiceId>::iterator end     = m_failedLocationIds.end ();

    while (end != element)
    {
        if (locationId == (*element))
        {
            return (true);
        }

        element++;
    }

    return (false);
}

void CreateClusterWithNodesContext::clearFailedLocationIdVector ()
{
    m_failedLocationIds.clear ();
}

bool CreateClusterWithNodesContext::getIsCreateClusterContext () const
{
    return (m_isCreateClusterContext);
}

void CreateClusterWithNodesContext::setIsCreateClusterContext (const bool &isCreateClusterContext)
{
    m_isCreateClusterContext = isCreateClusterContext;
}

}
