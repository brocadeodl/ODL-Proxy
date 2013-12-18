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

#include "Framework/Failover/PrismFailoverAgentContext.h"

namespace WaveNs
{

PrismFailoverAgentContext::PrismFailoverAgentContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext)
    : PrismAsynchronousContext (pCaller, pCallback, pCallerContext),
      m_serviceToBeIgnored     (0),
      m_isConfigurationChange  (false)
{
}

PrismFailoverAgentContext::~PrismFailoverAgentContext ()
{
}

void PrismFailoverAgentContext::setFailedLocationIds (const vector<LocationId> &failedLocationIds)
{
    m_failedLocationIds = failedLocationIds;
}

vector<LocationId> &PrismFailoverAgentContext::getFailedLocationIds ()
{
    return (m_failedLocationIds);
}

void PrismFailoverAgentContext::setServiceToBeIgnored (const PrismServiceId &serviceToBeIgnored)
{
    m_serviceToBeIgnored = serviceToBeIgnored;
}

PrismServiceId PrismFailoverAgentContext::getServiceToBeIgnored () const
{
    return (m_serviceToBeIgnored);
}

bool PrismFailoverAgentContext::getIsConfigurationChange () const
{
    return m_isConfigurationChange;
}

void PrismFailoverAgentContext::setIsConfigurationChange (const bool isConfigurationChange)
{
    m_isConfigurationChange = isConfigurationChange;
}


}
