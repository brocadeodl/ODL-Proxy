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

#include "Framework/Core/PrismFrameworkFailoverWorkerContext.h"

namespace WaveNs
{

PrismFrameworkFailoverWorkerContext::PrismFrameworkFailoverWorkerContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext)
    : PrismAsynchronousContext (pCaller, pCallback, pCallerContext),
      m_thisLocationRole       (LOCATION_STAND_ALONE),
      m_failoverReason         (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED),
      m_serviceToBeIgnored     (0),
      m_secondaryControlledFailoverDueToPrimaryRemoval (false),
      m_isConfigurationChange  (false)
{
}

PrismFrameworkFailoverWorkerContext::~PrismFrameworkFailoverWorkerContext ()
{
}

LocationRole PrismFrameworkFailoverWorkerContext::getThisLocationRole () const
{
    return (m_thisLocationRole);
}

void PrismFrameworkFailoverWorkerContext::setThisLocationRole (const LocationRole &thisLocationRole)
{
    m_thisLocationRole = thisLocationRole;
}

FrameworkObjectManagerFailoverReason PrismFrameworkFailoverWorkerContext::getFailoverReason () const
{
    return (m_failoverReason);
}

void PrismFrameworkFailoverWorkerContext::setFailoverReason (const FrameworkObjectManagerFailoverReason &failoverReason)
{
    m_failoverReason = failoverReason;
}

UI32 PrismFrameworkFailoverWorkerContext::getNumberOfFailedLocations () const
{
    return (m_failedLocationIds.size ());
}

LocationId PrismFrameworkFailoverWorkerContext::getFailedLocationAtIndex (const UI32 &index) const
{
    return (m_failedLocationIds[index]);
}

void PrismFrameworkFailoverWorkerContext::addFailedLocationId (const LocationId &failedLocationId)
{
    m_failedLocationIds.push_back (failedLocationId);
}

void PrismFrameworkFailoverWorkerContext::getFailedLocationIds (vector<LocationId> &failedLocationIds)
{
    failedLocationIds = m_failedLocationIds;
}

void PrismFrameworkFailoverWorkerContext::setServiceToBeIgnored (const PrismServiceId &serviceToBeIgnored)
{
    m_serviceToBeIgnored = serviceToBeIgnored;
}

PrismServiceId PrismFrameworkFailoverWorkerContext::getServiceToBeIgnored () const
{
    return (m_serviceToBeIgnored);
}

void PrismFrameworkFailoverWorkerContext::setSecondaryControlledFailoverDueToPrimaryRemoval()
{
    m_secondaryControlledFailoverDueToPrimaryRemoval = true;
}

bool PrismFrameworkFailoverWorkerContext::isSecondaryControlledFailoverDueToPrimaryRemoval () const
{
   return m_secondaryControlledFailoverDueToPrimaryRemoval;
}

bool PrismFrameworkFailoverWorkerContext::getIsConfigurationChange () const
{
    return m_isConfigurationChange;
}

void PrismFrameworkFailoverWorkerContext::setIsConfigurationChange (const bool isConfigurationChange)
{
    m_isConfigurationChange = isConfigurationChange;
}

}
