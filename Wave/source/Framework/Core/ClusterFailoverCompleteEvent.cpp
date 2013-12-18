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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/


#include "Framework/Core/ClusterFailoverCompleteEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{

ClusterFailoverCompleteEvent::ClusterFailoverCompleteEvent (FrameworkObjectManagerFailoverReason failoverReason, vector<LocationId> &failedLocationIds, bool isPrincipalChangedWithThisFailover)
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_BROADCAST_FAILOVER_COMPLETE_EVENT),
      m_failoverReason                      (failoverReason),
      m_failedLocationIds                   (failedLocationIds),
      m_isPrincipalChangedWithThisFailover  (isPrincipalChangedWithThisFailover)
{
}

ClusterFailoverCompleteEvent::~ClusterFailoverCompleteEvent ()
{
}

FrameworkObjectManagerFailoverReason ClusterFailoverCompleteEvent::getfailoverReason () const
{
    return (m_failoverReason);
}

void ClusterFailoverCompleteEvent::setFailoverReason (FrameworkObjectManagerFailoverReason failoverReason)
{
    m_failoverReason = failoverReason;
}

vector<LocationId> ClusterFailoverCompleteEvent::getFailoverReason () const
{
    return (m_failedLocationIds);
}

void ClusterFailoverCompleteEvent::setFailedLocationIds (const vector<LocationId> &failedLocationIds)
{
    m_failedLocationIds = failedLocationIds;
}

bool ClusterFailoverCompleteEvent::getIsPrincipalChangedWithThisFailover () const
{
    return (m_isPrincipalChangedWithThisFailover);
}

void ClusterFailoverCompleteEvent::setIsPrincipalChangedWithThisFailover (bool isPrincipalChangedWithThisFailover)
{
    m_isPrincipalChangedWithThisFailover = isPrincipalChangedWithThisFailover;
}

}
