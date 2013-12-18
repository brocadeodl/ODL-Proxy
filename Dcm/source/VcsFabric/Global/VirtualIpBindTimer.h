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
 *   Author : kghanta                                                      *
 **************************************************************************/

#ifndef VIRTUALIPBINDTIMER_H
#define VIRTUALIPBINDTIMER_H

#include "Framework/ObjectModel/PrismElement.h"
using namespace WaveNs;

namespace DcmNs
{

class VirtualIpBindTimer
{
    private :
	protected :
	public :
	                                        VirtualIpBindTimer  ();
	    virtual                            ~VirtualIpBindTimer  ();
	    static  VirtualIpBindTimer         *getInstance         ();

                TimerHandle                &getTimerHandle      ();

                UI32                        getCacheInterval    ();   
                void                        setCacheInterval    (const UI32 &cacheInterval);

                string                      getVirtualIpAddress ();
                void                        setVirtualIpAddress (const string &virtualIpAddress);

    // Now the data members

    private :
	    static  VirtualIpBindTimer         *s_instance;
	            TimerHandle		            m_timerHandle;
	            UI32			            m_cacheInterval;
                string                      m_virtualIpAddress;

    protected :
    public :
};

}
#endif
