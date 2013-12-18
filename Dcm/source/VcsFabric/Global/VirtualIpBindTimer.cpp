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
 *   Author : Kumar Ghanta                                                 *
 **************************************************************************/

#include "VcsFabric/Global/VirtualIpBindTimer.h"

namespace DcmNs
{
VirtualIpBindTimer *VirtualIpBindTimer::s_instance = NULL;

VirtualIpBindTimer::VirtualIpBindTimer () :
    m_timerHandle       (0),
    m_cacheInterval     (30000),
    m_virtualIpAddress  ("none")
{
}

VirtualIpBindTimer::~VirtualIpBindTimer ()
{
}

VirtualIpBindTimer *VirtualIpBindTimer::getInstance()
{
    if (NULL == s_instance)
    {
        s_instance = new VirtualIpBindTimer();
    }

    return (s_instance);
}

TimerHandle &VirtualIpBindTimer::getTimerHandle ()
{
    return (m_timerHandle);
}

UI32 VirtualIpBindTimer::getCacheInterval ()
{
    return (m_cacheInterval);
}

void VirtualIpBindTimer::setCacheInterval (const UI32 &cacheInterval)
{
    m_cacheInterval = cacheInterval;
}

string VirtualIpBindTimer::getVirtualIpAddress ()
{
    return (m_virtualIpAddress);
}

void VirtualIpBindTimer::setVirtualIpAddress (const string &virtualIpAddress)
{
    m_virtualIpAddress = virtualIpAddress;
}

}
