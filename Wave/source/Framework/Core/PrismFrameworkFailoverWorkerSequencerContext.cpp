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

#include "Framework/Core/PrismFrameworkFailoverWorkerSequencerContext.h"
#include "Framework/Failover/PrismFailoverAgent.h"

namespace WaveNs
{

PrismFrameworkFailoverWorkerSequencerContext::PrismFrameworkFailoverWorkerSequencerContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_thisLocationRole(LOCATION_STAND_ALONE),
      m_failoverReason      (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED),
      m_pPrismFailoverAgent (NULL),
      m_serviceToBeIgnored(0)
{
}

PrismFrameworkFailoverWorkerSequencerContext::~PrismFrameworkFailoverWorkerSequencerContext ()
{
    if (NULL != m_pPrismFailoverAgent)
    {
        delete m_pPrismFailoverAgent;
        m_pPrismFailoverAgent = NULL;
    }
}

LocationRole PrismFrameworkFailoverWorkerSequencerContext::getThisLocationRole () const
{
    return (m_thisLocationRole);
}

void PrismFrameworkFailoverWorkerSequencerContext::setThisLocationRole (const LocationRole &thisLocationRole)
{
    m_thisLocationRole = thisLocationRole;
}

FrameworkObjectManagerFailoverReason PrismFrameworkFailoverWorkerSequencerContext::getFailoverReason () const
{
    return (m_failoverReason);
}

void PrismFrameworkFailoverWorkerSequencerContext::setFailoverReason (const FrameworkObjectManagerFailoverReason &failoverReason)
{
    m_failoverReason = failoverReason;
}

UI32 PrismFrameworkFailoverWorkerSequencerContext::getNumberOfFailedLocations () const
{
    return (m_failedLocationIds.size ());
}

LocationId PrismFrameworkFailoverWorkerSequencerContext::getFailedLocationAtIndex (const UI32 &index) const
{
    return (m_failedLocationIds[index]);
}

void PrismFrameworkFailoverWorkerSequencerContext::addFailedLocationId (const LocationId &failedLocationId)
{
    m_failedLocationIds.push_back (failedLocationId);
}

PrismFailoverAgent *PrismFrameworkFailoverWorkerSequencerContext::getPPrismFailoverAgent () const
{
    return (m_pPrismFailoverAgent);
}

void PrismFrameworkFailoverWorkerSequencerContext::setPPrismFailoverAgent (PrismFailoverAgent *pPrismFailoverAgent)
{
    m_pPrismFailoverAgent = pPrismFailoverAgent;
}

void PrismFrameworkFailoverWorkerSequencerContext::setServiceToBeIgnored (const PrismServiceId &serviceToBeIgnored)
{
    m_serviceToBeIgnored = serviceToBeIgnored;
}

PrismServiceId PrismFrameworkFailoverWorkerSequencerContext::getServiceToBeIgnored () const
{
    return (m_serviceToBeIgnored);
}

}
