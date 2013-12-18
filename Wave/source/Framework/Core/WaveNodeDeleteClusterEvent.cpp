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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/


#include "Framework/Core/WaveNodeDeleteClusterEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

namespace WaveNs
{

WaveNodeDeleteClusterEvent::WaveNodeDeleteClusterEvent ()
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_DELETE_CLUSTER_EVENT),
      m_isRebootRequired (true)
{
}

WaveNodeDeleteClusterEvent::~WaveNodeDeleteClusterEvent ()
{

}

void WaveNodeDeleteClusterEvent::setupAttributesForSerialization ()
{
  PrismEvent::setupAttributesForSerialization ();
}

void WaveNodeDeleteClusterEvent::setIsRebootRequired (const bool &isRebootRequired)
{
    m_isRebootRequired = isRebootRequired;
}

bool WaveNodeDeleteClusterEvent::getIsRebootRequired () const
{
    return (m_isRebootRequired);
}

}
