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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Bao Vu                                                       *
 ***************************************************************************/


#include "VcsFabric/Local/VcsNodeTimerHandler.h"

namespace DcmNs
{
    VcsNodeTimerHandler::VcsNodeTimerHandler (UI32 domainId, LocationId locationId)
    {
        m_domainId = domainId;
        m_locationId = locationId;
        m_timerHandle = 0;
    }
    
    VcsNodeTimerHandler::~VcsNodeTimerHandler ()
    {
        m_domainId = 0;
        m_locationId = 0;
        m_timerHandle = 0;
    }

    UI32 VcsNodeTimerHandler::getDomainId()
    {
        return m_domainId;
    }

    TimerHandle VcsNodeTimerHandler::getTimerHandle()
    {
        return m_timerHandle;
    }

    LocationId	VcsNodeTimerHandler::getLocationId()
    {
        return m_locationId;
    }

    void VcsNodeTimerHandler::setTimerHandle(TimerHandle timerHandle)
    {
        m_timerHandle = timerHandle;
    }

    string VcsNodeTimerHandler::getServerIpAddress () const
    {
        return (m_serverIpAddress);
    }

    void VcsNodeTimerHandler::setServerIpAddress (const string &serverIpAddress)
    {
        m_serverIpAddress = serverIpAddress;
    }

    UI32 VcsNodeTimerHandler::getServerPort () const
    {
        return (m_serverPort);
    }

    void VcsNodeTimerHandler::setServerPort (const UI32 &serverPort)
    {
        m_serverPort = serverPort;
    }
}
