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

#ifndef DISCOVERTIMERS_H
#define DISCOVERTIMERS_H

#include "Framework/ObjectModel/PrismElement.h"
using namespace WaveNs;


namespace DcmNs
{

class DiscoverStartDiscoverTimer
{
	protected:
	public:
		DiscoverStartDiscoverTimer ();
		virtual ~DiscoverStartDiscoverTimer ();
		TimerHandle getTimerHandle();
		void setTimerHandle(TimerHandle timerHandle);
		UI32 getTimerInterval()	const;
		void setTimerInterval(const UI32 &timerHandle);
		static DiscoverStartDiscoverTimer* getInstance();
		static DiscoverStartDiscoverTimer *s_instance;
		
		TimerHandle		m_timerHandle;
		UI32			m_timerInterval;
};

}
#endif /* DISCOVERTIMERS_H */
