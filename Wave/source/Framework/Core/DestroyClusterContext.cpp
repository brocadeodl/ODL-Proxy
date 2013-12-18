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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/DestroyClusterContext.h"

namespace WaveNs
{

DestroyClusterContext::DestroyClusterContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : DeleteNodesFromClusterContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_thisLocationRoleBeforeDestroy (LOCATION_STAND_ALONE),
      m_reasonForDestroyingCluster (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED),
      m_isRebootRequired (true)
{
}

DestroyClusterContext::DestroyClusterContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : DeleteNodesFromClusterContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_thisLocationRoleBeforeDestroy (LOCATION_STAND_ALONE),
      m_reasonForDestroyingCluster (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED),
      m_isRebootRequired (true)
{
}

DestroyClusterContext::~DestroyClusterContext ()
{
}

LocationRole DestroyClusterContext::getThisLocationRoleBeforeDestroy () const
{
    return (m_thisLocationRoleBeforeDestroy);
}

void DestroyClusterContext::setThisLocationRoleBeforeDestroy (const LocationRole &thisLocationRoleBeforeDestroy)
{
    m_thisLocationRoleBeforeDestroy = thisLocationRoleBeforeDestroy;
}

FrameworkObjectManagerFailoverReason DestroyClusterContext::getReasonForDestroyingCluster () const
{
    return (m_reasonForDestroyingCluster);
}

void DestroyClusterContext::setReasonForDestroyingCluster (const FrameworkObjectManagerFailoverReason &reasonForDestroyingCluster)
{
    m_reasonForDestroyingCluster = reasonForDestroyingCluster;
}

void DestroyClusterContext::setIsRebootRequired (const bool &isRebootRequired)
{
    m_isRebootRequired = isRebootRequired;
}

bool DestroyClusterContext::getIsRebootRequired () const
{
    return (m_isRebootRequired);
}

}
