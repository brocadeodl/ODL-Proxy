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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "discover/User/discoverTimers.h"

namespace DcmNs
{
	DiscoverStartDiscoverTimer *DiscoverStartDiscoverTimer::s_instance = 0;

	DiscoverStartDiscoverTimer::DiscoverStartDiscoverTimer ()
	{
        m_timerHandle = 0;
        m_timerInterval = 0;
	}

	DiscoverStartDiscoverTimer::~DiscoverStartDiscoverTimer ()
	{
	
	}
		
    void DiscoverStartDiscoverTimer::setTimerHandle(TimerHandle timerHandle)
    {
        m_timerHandle = timerHandle;
    }
 	
	TimerHandle DiscoverStartDiscoverTimer::getTimerHandle()
    {
        return m_timerHandle;
    }

	void  DiscoverStartDiscoverTimer::setTimerInterval(const UI32 &timerInterval)
    {
        m_timerInterval  =  timerInterval;
    }

    UI32  DiscoverStartDiscoverTimer::getTimerInterval() const
    {
        return (m_timerInterval);
    }

	DiscoverStartDiscoverTimer* DiscoverStartDiscoverTimer::getInstance()
	{
		if (!s_instance)
			s_instance = new DiscoverStartDiscoverTimer();
	    return s_instance;
	}

}
