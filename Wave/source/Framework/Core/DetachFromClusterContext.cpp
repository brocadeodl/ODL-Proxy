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

#include "Framework/Core/DetachFromClusterContext.h"

namespace WaveNs
{

DetachFromClusterContext::DetachFromClusterContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext     (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_thisLocationRoleBeforeDetach  (LOCATION_STAND_ALONE),
      m_reasonForDetachingFromCluster (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED)
{
}

DetachFromClusterContext::DetachFromClusterContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_thisLocationRoleBeforeDetach  (LOCATION_STAND_ALONE),
      m_reasonForDetachingFromCluster (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED)
{
}

DetachFromClusterContext::~DetachFromClusterContext ()
{
}

LocationRole DetachFromClusterContext::getThisLocationRoleBeforeDetach () const
{
    return (m_thisLocationRoleBeforeDetach);
}

void DetachFromClusterContext::setThisLocationRoleBeforeDetach (const LocationRole &thisLocationRoleBeforeDetach)
{
    m_thisLocationRoleBeforeDetach = thisLocationRoleBeforeDetach;
}

FrameworkObjectManagerFailoverReason DetachFromClusterContext::getReasonForDetachingFromCluster () const
{
    return (m_reasonForDetachingFromCluster);
}

void DetachFromClusterContext::setReasonForDetachingFromCluster (const FrameworkObjectManagerFailoverReason &reasonForDetachingFromCluster)
{
    m_reasonForDetachingFromCluster = reasonForDetachingFromCluster;
}

}
