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
 *   Author : Leifang Hu                                                   *
 ***************************************************************************/

#include "Cluster/Local/ClusterLocalSetHaRoleMessage.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/WaveHaNodeTypes.h"

namespace WaveNs
{

ClusterLocalSetHaRoleMessage::ClusterLocalSetHaRoleMessage ()
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_SET_HA_ROLE),
      m_haRole                (WAVE_HA_NODE_STANDBY_UNCONFIRMED)
{
}

ClusterLocalSetHaRoleMessage::ClusterLocalSetHaRoleMessage (const UI32 &haRole)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_SET_HA_ROLE),
      m_haRole                (haRole)
{
}

ClusterLocalSetHaRoleMessage::~ClusterLocalSetHaRoleMessage ()
{
}

UI32 ClusterLocalSetHaRoleMessage::getHaRole () const
{
    return (m_haRole);
}

void ClusterLocalSetHaRoleMessage::setHaRole (const UI32 &haRole)
{
    m_haRole = haRole;
}

}
